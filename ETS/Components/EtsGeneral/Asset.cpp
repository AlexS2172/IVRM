#include "StdAfx.h"
#include "Asset.h"
#include "SpotContract.h"
#include "EtsRateColl.h"
#include "Curve.h"
//-----------------------------------------------------------------------------------------------------------------------//
CAsset::CAsset(void):	m_nID(0L), m_enUndType(enCtStock), m_bIsAmerican(VARIANT_FALSE),
						m_bIsHTB(VARIANT_FALSE), m_dYield(0.), m_bIsTraderContract(VARIANT_FALSE),
						m_bHaveSyntheticRoots(VARIANT_FALSE), m_bHaveOptions(VARIANT_FALSE), m_bIsActive(VARIANT_FALSE),
						m_bIsBasketIndex(VARIANT_FALSE), m_nVolume10Day(0), m_nVolume5Expiration(0),
						m_bHaveFutures(VARIANT_FALSE), m_enDivType(enDivMarket), m_dSOQ(0.), 
						m_bIsAllowLending(VARIANT_FALSE), m_bIsHedgeSymbol(VARIANT_FALSE), m_fIsManualVol(FALSE),
						m_dCoeff(1.0), m_bPriceByHead(VARIANT_FALSE), m_bIsHead(VARIANT_FALSE),
						price_option_using_fwd_curve(false)
{
	Clear();

	this->forward_curve = new ForwardCurve();
};
//-----------------------------------------------------------------------------------------------------------------------//
CAsset::~CAsset(void)
{
	CComGITPtr<IVolaControl> pVolaControl(dwVolaControlCooke);
};
//-----------------------------------------------------------------------------------------------------------------------//
void CAsset::Clear()
{
	//CAsset
	m_spContracts = NULL;
	m_spSpot = NULL;
	m_spBorrowingRatesCurve = NULL;
	m_spVolatilitySource = NULL;
	m_spAssets = NULL;

	//CUndAtom
	m_spSyntheticRoots = NULL;
	m_spUndPriceProfile = NULL;
	m_spOptPriceProfile = NULL;

	m_spFutRoots = NULL;
	m_spRoots = NULL;

	m_spCustomDivs = NULL;
	m_spDividend = NULL;
	m_spBasketIndex = NULL;

	m_spActiveFuture = NULL;
	m_spHTBRates = NULL;
	m_spHeadComponent = NULL;

	m_bVolatilityInitialized = false;
};
//-----------------------------------------------------------------------------------------------------------------------//
IVolaControl* CAsset::GetVolatilitySource()
{
	IVolaControl* pRet = NULL;
	try
	{
		CComGITPtr<IVolaControl> pVolaControl(dwVolaControlCooke);	
		pVolaControl.CopyTo(&pRet);
		pVolaControl.Detach();
	}
	catch (_com_error& e)
	{
		e.Description();
		ATLASSERT(false);
		return NULL;
	}
	catch (...)
	{
		ATLASSERT(false);
		return NULL;
	}
	return pRet;
};
//-----------------------------------------------------------------------------------------------------------------------//
double CAsset::GetDividends(double dtDate, double dtExpiryOV, double dtTradingClose, 
							CSafeArrayWrapper<double> &saDates, 
							CSafeArrayWrapper<double> &saAmounts, 
							long &lDividendsCount)
{
	try
	{
		if (static_cast<bool>(m_spDividend) == false)	return 0.;

		EtsDivTypeEnum	enDividendType = enDivCustomStream;	
		m_spDividend->get_DivType(&enDividendType);

		switch (enDividendType)
		{
		case enDivMarket:
		case enDivCustomPeriodical:
		case enDivCustomStream:
			{
				m_spDividend->GetDividendCount2(dtDate, dtExpiryOV, dtTradingClose, &lDividendsCount);
				if(lDividendsCount > 0L) 
				{
					LPSAFEARRAY psaAmounts	= NULL;
					LPSAFEARRAY psaDates	= NULL;

					m_spDividend->GetDividends2(dtDate, dtExpiryOV, dtTradingClose, lDividendsCount, &psaAmounts, &psaDates, &lDividendsCount);
					saAmounts.Attach(psaAmounts);
					saDates.Attach(psaDates);
				}
			}break;
		case enDivStockBasket:
			{
				VARIANT_BOOL bIsBasket = VARIANT_FALSE;

				if(m_spBasketIndex != NULL)
				{
					_CHK((m_spBasketIndex->get_IsBasket(&bIsBasket)));
					if(bIsBasket)
					{
						IEtsIndexDivCollPtr spBasketDivs;
						m_spBasketIndex->get_BasketDivs(&spBasketDivs);

						lDividendsCount = 0;

						LPSAFEARRAY psaAmounts = NULL;
						LPSAFEARRAY psaDates = NULL;

						spBasketDivs->GetDividends2(dtDate, dtExpiryOV, dtTradingClose, lDividendsCount, &psaAmounts, &psaDates, &lDividendsCount);

						saAmounts.Attach(psaAmounts);
						saDates.Attach(psaDates);
					}
				}
			}break;
		case enDivIndexYield:
			{
				return m_dYield;
			}break;	
		}
	}
	catch (_com_error& err)
	{
		TRACE_COM_ERROR(err);
		ATLASSERT(FALSE);
	}
	catch (...)
	{
		TRACE_UNKNOWN_ERROR();
		ATLASSERT(FALSE);
	}
	return 0.;
}
//-----------------------------------------------------------------------------------------------------------------------//
double	CAsset::GetBorrowingRate(double dtDate, double dtExpiryOV)
{

	try
	{
		CBorrowingRatesCurve*		pCurveBase		= NULL;
		CComObject<CEtsRateColl>*	pCurve			= NULL;
		CSpotContract*				pSpotContract	= NULL;

		bool	bUseNeutralRate		= false;
		double	dAssetPosThershold	= 0.;
		double	dCurvePosThershold	= 0.;
		double	dReturnRate			= BAD_DOUBLE_VALUE;

		if (static_cast<bool>(m_spBorrowingRatesCurve))
		{
			pCurveBase = dynamic_cast<CBorrowingRatesCurve*>(m_spBorrowingRatesCurve.GetInterfacePtr());
			pCurve = dynamic_cast<CComObject<CEtsRateColl>*>(m_spBorrowingRatesCurve.GetInterfacePtr());

			if (pCurveBase)
			{
				//select asset pos for rate 
				if (static_cast<bool>(m_spSpot))
				{
					pSpotContract = dynamic_cast<CSpotContract*>(m_spSpot.GetInterfacePtr());

					if (pSpotContract)	
						dAssetPosThershold = pSpotContract->m_dUndPosForRates;
				}

				//select rate return mode
				if (pCurveBase->m_enRule == enRtBasedOnPositionRate)
				{	
					dCurvePosThershold = pCurveBase->m_dPosThreshold;

					if (fabs(dAssetPosThershold) > dCurvePosThershold || fabs(dCurvePosThershold) < DBL_EPSILON)
						bUseNeutralRate = true;				
				}
				else if (pCurveBase->m_enRule == enRtNeutralRate)
				{
					bUseNeutralRate = true;
				}

				//select rates
				long	lPointsCount = 0;
				_CHK(pCurve->get_Count(&lPointsCount), _T("Fail to get count of curve points."));
				if (m_bIsHTB == VARIANT_TRUE)
				{
					if (lPointsCount > 0)
					{
						if (m_bIsAllowLending == VARIANT_TRUE)
						{
							if (dAssetPosThershold < 0.)
								_CHK(pCurve->GetShortRate(dtDate, dtExpiryOV, &dReturnRate), _T("Fail to get ShortRate."));
							else
								_CHK(pCurve->GetLongRate(dtDate, dtExpiryOV, &dReturnRate), _T("Fail to get LongRate."));
						}
						else
						{
							if (dAssetPosThershold < 0.)
								_CHK(pCurve->GetShortRate(dtDate, dtExpiryOV, &dReturnRate), _T("Fail to get ShortRate."));
						}
					}
					else
						dReturnRate = BAD_DOUBLE_VALUE;
				}
				else //not HTB
				{
					if (lPointsCount > 0)
					{
						if (bUseNeutralRate)
							_CHK(pCurve->GetNeutralRate(dtDate, dtExpiryOV, &dReturnRate), _T("Fail to get NeutralRate."));
						else
						{
							if (dAssetPosThershold < 0.)
								_CHK(pCurve->GetShortRate(dtDate, dtExpiryOV, &dReturnRate), _T("Fail to get ShortRate."));
							else
								_CHK(pCurve->GetLongRate(dtDate, dtExpiryOV, &dReturnRate), _T("Fail to get LongRate."));
						}
					}
					else
						dReturnRate = BAD_DOUBLE_VALUE;
				}
			}
		}
		return dReturnRate;
	}
	catch (_com_error& err)
	{
		TRACE_COM_ERROR(err);
		ATLASSERT(FALSE);
	}
	catch (...)
	{
		TRACE_UNKNOWN_ERROR();
		ATLASSERT(FALSE);
	}
	return BAD_DOUBLE_VALUE;
}
//-----------------------------------------------------------------------------------------------------------------------//

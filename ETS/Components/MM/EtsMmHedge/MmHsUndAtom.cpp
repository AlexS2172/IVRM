// MmHsUndAtom.cpp : Implementation of CMmHsUndAtom

#include "stdafx.h"
#include "MmHsUndAtom.h"


// CMmHsUndAtom

STDMETHODIMP CMmHsUndAtom::ClearValues()
{
	__MmHsUndAtom::ClearValues();
	return S_OK;
}

STDMETHODIMP CMmHsUndAtom::CalcGreeks( LONG nGreekMask, EtsCalcModelTypeEnum enCalcModel, 
									  VARIANT_BOOL vbVegaWeighted, VARIANT_BOOL bUseTheoVolatility, 
									  VARIANT_BOOL bUseTheoVolaNoBid, VARIANT_BOOL bUseTheoVolaBadMarket, 
									  DOUBLE dUndPriceTolerance, EtsPriceRoundingRuleEnum enPriceRoundingRule,
									  ICalculationParametrs* pParams)
{
	try
	{
		if( nGreekMask == GT_NOTHING )
			return S_OK;

		HRESULT hr;
		GREEKS aGreeks;
		LONG nModel = (LONG)enCalcModel;
		LONG nIsAmerican = 0L;

		CSafeArrayWrapper<double> saDates;
		CSafeArrayWrapper<double> saAmounts;

		EtsReplacePriceStatusEnum enRpsStatus = enRpsNone;

		if (m_vbIsAmerican == VARIANT_TRUE)
			nIsAmerican = 1L;

		DOUBLE  dSpotBid = m_dPriceBid, 
			dSpotAsk = m_dPriceAsk, 
			dSpotLast = m_dPriceLast;

		if(dSpotBid > DBL_EPSILON || dSpotAsk > DBL_EPSILON || dSpotLast > DBL_EPSILON)
		{
			enRpsStatus = enRpsNone;
			DOUBLE dSpotPrice = m_spUndPriceProfile->GetUndPriceMid(dSpotBid, dSpotAsk, dSpotLast, dUndPriceTolerance, enPriceRoundingRule, &enRpsStatus, FALSE);
			if(dSpotPrice > DBL_EPSILON)
			{
				DOUBLE dOptionsSumDelta = 0.;
				DOUBLE dOptionsSumVega = 0.;

				IUnknownPtr spUnk;
				_variant_t varItem;
				ULONG nFetched = 0L;
				IMmHsOptAtomPtr spOptAtom;

				__CHECK_HRESULT(m_spOpt->get__NewEnum(&spUnk), _T("Fail to get options collection"));		

				IEnumVARIANTPtr spOptEnum(spUnk);
				__CHECK_HRESULT(spOptEnum->Reset(), _T("Fail to reset options collection"));
				while((hr = spOptEnum->Next(1L, &varItem, &nFetched)) == S_OK)
				{
					ATLASSERT(varItem.vt == VT_DISPATCH);
					spOptAtom = varItem;
					if(nFetched > 0 && spOptAtom != NULL)
					{
						memset(&aGreeks, 0, sizeof(GREEKS));
						aGreeks.nMask = nGreekMask;

						DOUBLE  dOptBid = 0., dOptAsk = 0., dOptLast = 0., dOptPrice = 0.;
						DATE	dtExpiry = 0.;
						DOUBLE	dStrike = 0.;
						DOUBLE	dOptVola = 0.;
						DOUBLE	dOptBidForIv = 0.;

						__CHECK_HRESULT3(spOptAtom->get_Expiry(&dtExpiry) );
						__CHECK_HRESULT3(spOptAtom->get_Strike(&dStrike) );

						__CHECK_HRESULT3(spOptAtom->get_PriceAsk(&dOptAsk));
						__CHECK_HRESULT3(spOptAtom->get_PriceBid(&dOptBid));
						__CHECK_HRESULT3(spOptAtom->get_PriceLast(&dOptLast));
						dOptBidForIv = dOptBid;

						enRpsStatus = enRpsNone;
						if(dOptBid > DBL_EPSILON || dOptAsk > DBL_EPSILON || dOptLast > DBL_EPSILON)
							dOptPrice = m_spOptPriceProfile->GetOptPriceMid(dOptBid, dOptAsk, dOptLast, enPriceRoundingRule, bUseTheoVolatility, 0, &enRpsStatus );

						if(aGreeks.nMask != GT_NOTHING)
						{
							LONG nDivCount = 0L, nRetCount = 0L;
							DATE dtTemp = 0., dtExpiryOV = 0., tmCloseTime = 0., dtNow = 0., dtExpiryOVOrig = 0.;
							DOUBLE	dYTE = 0.;

							__CHECK_HRESULT3(spOptAtom->get_ExpiryOV(&dtExpiryOV));
							__CHECK_HRESULT3(spOptAtom->get_TradingClose(&tmCloseTime));
							::GetNYDateTimeAsDATE(&dtNow);
							dYTE = (dtExpiryOV - dtNow) / 365.;
							dtExpiryOVOrig = dtExpiryOV;

							::GetCalculationParams(dtNow, dtExpiryOV, tmCloseTime, pParams->UseTimePrecision != VARIANT_FALSE, &dtNow, &dtExpiryOV, &tmCloseTime, &dYTE);
							
							if (m_spDividend != NULL)
								nDivCount = 0;
							m_spDividend->GetDividendCount2(dtNow, dtExpiryOV, tmCloseTime, &nDivCount);
							if (nDivCount< 0)
								nDivCount = 0;

							if (nDivCount> 0)
							{
								LPSAFEARRAY psaDates = NULL;
								LPSAFEARRAY psaAmounts = NULL;

								m_spDividend->GetDividends2(dtNow, dtExpiryOV, tmCloseTime, nDivCount, &psaAmounts, &psaDates, &nRetCount);
								saDates.Attach(psaDates);
								saAmounts.Attach(psaAmounts);
							}

							if (nDivCount< 0)
								nDivCount = 0;

							DOUBLE	dRate = 0.;
							DOUBLE	dHTBRate = BAD_DOUBLE_VALUE;
							EtsOptionTypeEnum	enOptType;
							LONG lSurfaceID;
							LONG lOptionRootID;
							__CHECK_HRESULT3(spOptAtom->get_Rate(&dRate));
							__CHECK_HRESULT3(spOptAtom->get_HTBRate(&dHTBRate));
							__CHECK_HRESULT3(spOptAtom->get_OptType(&enOptType));
							__CHECK_HRESULT3(spOptAtom->get_OptionRootID(&lOptionRootID));

							lSurfaceID = m_spVolaSrv->GetSurfaceByRoot(lOptionRootID);
							if (bUseTheoVolatility == VARIANT_TRUE)
							{
								dOptVola = m_spVolaSrv->GetOptionVola(dtExpiryOVOrig, dStrike, lSurfaceID);
								__CHECK_HRESULT3(spOptAtom->put_Vola(dOptVola) );
							}
							else
							{
								if (bUseTheoVolaNoBid == VARIANT_TRUE && dOptBidForIv <= 0.)
								{
									dOptVola = m_spVolaSrv->GetOptionVola(dtExpiryOVOrig, dStrike, lSurfaceID);
									__CHECK_HRESULT3(spOptAtom->put_Vola(dOptVola) );
								}
								else
								{
									LONG nFlag = VF_OK;
									if(dOptPrice > DBL_EPSILON)
									{
										dOptVola = ::CalcVolatilityMM3(dRate, m_dYield, dHTBRate, dSpotPrice, dOptPrice, dStrike, 
											dYTE, enOptType, nIsAmerican, nDivCount, saAmounts.GetPlainData(), saDates.GetPlainData(), 
											100L, 0, 0, nModel, &nFlag);

									}
									else if (bUseTheoVolaBadMarket == VARIANT_TRUE)
									{
										dOptVola = m_spVolaSrv->GetOptionVola(dtExpiryOVOrig, dStrike, lSurfaceID);
										__CHECK_HRESULT3(spOptAtom->put_Vola(dOptVola) );
									}

									if (bUseTheoVolaBadMarket == VARIANT_TRUE && nFlag != VF_OK)
									{
										dOptVola = m_spVolaSrv->GetOptionVola(dtExpiryOVOrig, dStrike, lSurfaceID);
										__CHECK_HRESULT3(spOptAtom->put_Vola(dOptVola) );
									}
								}
							}

							nRetCount = ::CalcGreeksMM2(dRate, m_dYield, dHTBRate, dSpotPrice, dStrike, dOptVola, 
								dYTE, enOptType, nIsAmerican, nDivCount, saAmounts.GetPlainData(), saDates.GetPlainData(), 
								100L, 0, 0, nModel, &aGreeks);

							if(nRetCount != 0L)
							{
								LONG nOptQty = 0L;
								LONG nOptLotSize = 0L;
								__CHECK_HRESULT3(spOptAtom->get_Qty(&nOptQty));
								__CHECK_HRESULT3(spOptAtom->get_LotSize(&nOptLotSize));

								if((aGreeks.nMask & GT_DELTA) && _finite(aGreeks.dDelta))
								{
									dOptionsSumDelta += aGreeks.dDelta * nOptQty * nOptLotSize;
									__CHECK_HRESULT3(spOptAtom->put_Delta(aGreeks.dDelta));
								}

								if((aGreeks.nMask & GT_VEGA) && _finite(aGreeks.dVega))
								{
									DOUBLE dVegaWeight = 1.;
									if (vbVegaWeighted == VARIANT_TRUE)
										__CHECK_HRESULT3(spOptAtom->get_VegaWeight(&dVegaWeight));

									dOptionsSumVega += aGreeks.dVega * dVegaWeight * nOptQty * nOptLotSize;
									__CHECK_HRESULT3(spOptAtom->put_Vega(aGreeks.dVega));
								}
							}
						}
					}
				}
				if((nGreekMask & GT_DELTA) && m_nQty > BAD_LONG_VALUE)
					m_dDelta = dOptionsSumDelta + m_nQty;
				if((nGreekMask & GT_VEGA) && !DoubleEQZero(dOptionsSumVega))
					m_dVega = dOptionsSumVega;
			}
		}
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)CComErrorWrapper::ErrorDescription(e), IID_IMmHsUndAtom, e.Error());
	}

	return S_OK;
}

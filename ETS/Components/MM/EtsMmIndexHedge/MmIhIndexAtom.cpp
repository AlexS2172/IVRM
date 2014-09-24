// MmIhIndexAtom.cpp : Implementation of CMmIhIndexAtom

#include "stdafx.h"
#include "MmIhIndexAtom.h"

// CMmIhIndexAtom

/////////////////////////////////////////////////////////////////////////////
//
void CMmIhIndexAtom::_GetBasketIndexDividends(REGULAR_DIVIDENDS* pDivs, LONG nMaxCount) throw()
{
	EtsRegularDividend aDiv;

	for(LONG i = 0; i < nMaxCount; i++)
	{
		_CHK(m_spBasketIndex->get_BasketDiv(i, &aDiv), _T("Fail to get basket dividend."));
		pDivs[i].nLastDivDate = static_cast<LONG>(aDiv.LastDate);
		pDivs[i].nFrequency = aDiv.Freq;
		pDivs[i].dAmount = aDiv.Amt;
	}
}

HRESULT CMmIhIndexAtom::CalcOptionGreeks(IMmIhOptAtomPtr aOpt, DOUBLE dSpotPriceMid,
									   EtsCalcModelTypeEnum enCalcModel, VARIANT_BOOL bUseTheoVolatility, 
									   VARIANT_BOOL bUseTheoVolaNoBid, VARIANT_BOOL bUseTheoVolaBadMarket,
									   DOUBLE dUndPriceTolerance, EtsPriceRoundingRuleEnum enPriceRoundingRule,
									   ICalculationParametrs* pParams)
{
	if(aOpt == NULL || pParams == NULL)
		return Error(L"Invalid objects passed.", IID_IMmIhIndexAtom, E_INVALIDARG);

	try
	{
		IMmIhOptAtomPtr		spOpt(aOpt);

		CSafeArrayWrapper<double> saDates;
		CSafeArrayWrapper<double> saAmounts;

		DOUBLE dOptPriceMid = 0., dDelta = BAD_DOUBLE_VALUE;
		EtsReplacePriceStatusEnum enMidPriceStatus = enRpsNone;

		DOUBLE dOptBid = 0., dOptAsk = 0., dOptLast = 0.;					

		_CHK(spOpt->get_PriceBid(&dOptBid));
		_CHK(spOpt->get_PriceAsk(&dOptAsk));
		_CHK(spOpt->get_PriceLast(&dOptLast));

		dOptPriceMid = m_spOptPriceProfile->GetOptPriceMid(dOptBid, dOptAsk, dOptLast,
			enPriceRoundingRule, bUseTheoVolatility, 0, &enMidPriceStatus);

		if(dSpotPriceMid > DBL_EPSILON)
		{
			GREEKS aGreeks;
			memset(&aGreeks, 0, sizeof(aGreeks));
			aGreeks.nMask = GT_DELTA;

			LONG nModel = static_cast<LONG>(enCalcModel);
			LONG nIsAmerican = (m_bIsAmerican ? 1L : 0L);
			DOUBLE dYield = 0.;

			LONG nDivCount = 0L, nRetCount = 0L;

			DOUBLE dRate = 0.;
			_CHK(spOpt->get_Rate(&dRate));

			DOUBLE dHTBRate = BAD_DOUBLE_VALUE;
			_CHK(spOpt->get_HTBRate(&dHTBRate));

			DOUBLE dStrike = 0.;
			_CHK(spOpt->get_Strike(&dStrike));

			EtsOptionTypeEnum enOptType = enOtPut;
			_CHK(spOpt->get_OptType(&enOptType));

			DATE dtTemp = 0., dtExpiryOV = 0., tmCloseTime = 0., dtNow = 0.;
			DOUBLE dYTE = 0.;

			_CHK(spOpt->get_ExpiryOV(&dtExpiryOV));
			_CHK(spOpt->get_TradingClose(&tmCloseTime));
			::GetNYDateTimeAsDATE(&dtNow);

			::GetCalculationParams(dtNow, dtExpiryOV, tmCloseTime, pParams->UseTimePrecision != VARIANT_FALSE, &dtNow, &dtExpiryOV, &tmCloseTime, &dYTE);

			if(m_spBasketIndex != NULL)
			{
				LONG nBaskDivCount = 0L; VARIANT_BOOL bIsBasket = VARIANT_FALSE;

				_CHK(m_spBasketIndex->get_IsBasket(&bIsBasket));

				if(bIsBasket /*&& nBaskDivCount > 0L*/)
				{

					IEtsIndexDivCollPtr spBasketDivs;
					m_spBasketIndex->get_BasketDivs(&spBasketDivs);
					nDivCount = 0;
					spBasketDivs->GetDividendCount2(dtNow, dtExpiryOV, tmCloseTime, &nDivCount);
					if(nDivCount > 0L) 
					{
						LPSAFEARRAY psaDates = NULL;
						LPSAFEARRAY psaAmounts = NULL;

						spBasketDivs->GetDividends2(dtNow, dtExpiryOV, tmCloseTime, nDivCount, &psaAmounts, &psaDates, &nDivCount);
						saDates.Attach(psaDates);
						saAmounts.Attach(psaAmounts);
					}
				}
			}
			else
				if(nDivCount <= 0.)
					dYield = m_dYield;

			// volatility calculation
			DOUBLE dVola = 0.;

			if (bUseTheoVolatility)
				_CHK(spOpt->get_Vola(&dVola));
			else
			{
				dVola = 0;
				if (!bUseTheoVolaNoBid || (bUseTheoVolaNoBid && dOptBid > 0)) 
				{
					if (dOptPriceMid > DBL_EPSILON)
					{
						LONG nFlag = VF_OK;
						dVola = ::CalcVolatilityMM3(dRate, dYield, dHTBRate, dSpotPriceMid, dOptPriceMid, dStrike, 
							dYTE, enOptType, nIsAmerican, nDivCount,
							saAmounts.GetPlainData(), saDates.GetPlainData(), 100L, m_dSkew, m_dKurt, nModel, &nFlag);
						
						if (bUseTheoVolaBadMarket && nFlag != VF_OK)
							_CHK(spOpt->get_Vola(&dVola));
					}
					else if (bUseTheoVolaBadMarket)
						_CHK(spOpt->get_Vola(&dVola));
				}
				else
					_CHK(spOpt->get_Vola(&dVola));
			}

			nRetCount = ::CalcGreeksMM2(dRate, dYield, dHTBRate, dSpotPriceMid, dStrike, dVola, dYTE,
				enOptType, nIsAmerican, nDivCount, saAmounts.GetPlainData(), saDates.GetPlainData(), 100L, m_dSkew, m_dKurt, nModel, &aGreeks);
			
			if(nRetCount != 0L)
			{
				if((aGreeks.nMask & GT_DELTA) && _finite(aGreeks.dDelta))
					dDelta = aGreeks.dDelta;
			}

		}

		_CHK(spOpt->put_Delta(dDelta));

	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)CComErrorWrapper::ErrorDescription(e), IID_IMmIhIndexAtom, e.Error());
	}

	return S_OK;
}

////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////

void CMmIhIndexAtom::_CalcSleep(LONG nCalcSleepFreq, LONG nCalcSleepAmt)
{
	if(nCalcSleepAmt > 0L && nCalcSleepFreq > 0L)
	{
		if((m_nSleepStep % nCalcSleepFreq) == 0L)
		{
			if(m_hCalcSleepTimer)
			{
				LARGE_INTEGER liDueTime;
				liDueTime.LowPart = -10000L * nCalcSleepAmt;
				liDueTime.HighPart = -1L;

				if(::SetWaitableTimer(m_hCalcSleepTimer, &liDueTime, 0L, NULL, NULL, FALSE))
				{
					MSG msg;

					while(WAIT_OBJECT_0 != ::MsgWaitForMultipleObjects(1, &m_hCalcSleepTimer, FALSE, INFINITE, QS_ALLEVENTS))
					{
						while(::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
						{
							::TranslateMessage(&msg);
							::DispatchMessage(&msg);
						}
						::Sleep(0L);
					}
				}
				else
					::Sleep(nCalcSleepAmt);
			}
			else
				::Sleep(nCalcSleepAmt);

			m_nSleepStep = 0L;
		}
		m_nSleepStep++;
	}
}

///////////////////////////////////////////////////////////////////////////////////
////////////////////

STDMETHODIMP CMmIhIndexAtom::CalcAllOptions(EtsCalcModelTypeEnum enCalcModel,
						  VARIANT_BOOL bUseTheoVolatility,
						  VARIANT_BOOL bUseTheoVolaNoBid,
						  VARIANT_BOOL bUseTheoVolaBadMarket,
						  DOUBLE dUndPriceTolerance,
						  enum EtsPriceRoundingRuleEnum enPriceRoundingRule,
						  LONG nCalcSleepFreq,
						  LONG nCalcSleepAmt,
						  ICalculationParametrs* pParams)
{
	try
	{
		IUnknownPtr spUnk;
		_variant_t varItem;
		ULONG nFetched = 0L;
		HRESULT hr;

		EtsReplacePriceStatusEnum enMidPriceStatus = enRpsNone;
		DOUBLE dSpotPriceMid = m_spUndPriceProfile->GetUndPriceMid(m_dPriceBid, m_dPriceAsk, m_dPriceLast, dUndPriceTolerance, enPriceRoundingRule, &enMidPriceStatus, FALSE);

		IMmIhOptAtomPtr spOption;
		_CHK(m_spOpt->get__NewEnum(&spUnk), _T("Fail to get options collection."));

		IEnumVARIANTPtr spOptionEnum(spUnk);
		_CHK(spOptionEnum->Reset(), _T("Fail to reset options collection."));

		while((hr = spOptionEnum->Next(1L, &varItem, &nFetched)) == S_OK)
		{
			ATLASSERT(varItem.vt == VT_DISPATCH);
			spOption = varItem;
			if(nFetched > 0 && spOption != NULL)
			{
				_CHK(CalcOptionGreeks(spOption, dSpotPriceMid,
					enCalcModel, bUseTheoVolatility, 
					bUseTheoVolaNoBid, bUseTheoVolaBadMarket,
					dUndPriceTolerance, enPriceRoundingRule, pParams),
					_T("Fail to calculate option greeks.")); 

				_CalcSleep(nCalcSleepFreq, nCalcSleepAmt);
			}
		}
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)CComErrorWrapper::ErrorDescription(e), IID_IMmIhIndexAtom, e.Error());
	}

	return S_OK;
}

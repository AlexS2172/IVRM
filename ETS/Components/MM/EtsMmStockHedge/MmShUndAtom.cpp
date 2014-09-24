// MmShUndAtom.cpp : Implementation of CMmShUndAtom

#include "stdafx.h"
#include "MmShUndAtom.h"


// CMmShUndAtom
/////////////////////////////////////////////////////////////////////////////
HRESULT CMmShUndAtom::_GetSyntheticUnderlyingPrice(
				IMmShUndCollPtr aUndColl, 
				ISynthRootAtomPtr aSynthRoot, DOUBLE& dSpotBid, DOUBLE& dSpotAsk, 
				DOUBLE& dSpotLast, DOUBLE dUndPriceTolerance, EtsPriceRoundingRuleEnum enPriceRoundingRule)
{

	if(aUndColl == NULL)
		return Error(L"Invalid object passed.", IID_IMmShUndAtom, E_INVALIDARG);


	dSpotBid = BAD_DOUBLE_VALUE;
	dSpotAsk = BAD_DOUBLE_VALUE;
	dSpotLast = BAD_DOUBLE_VALUE;

	//if(!m_bHasSynthetic || m_spSynthRoots == NULL || aSynthRoot == NULL)
	//	return S_OK;

	try
	{
		IUnknownPtr spUnk;
		_variant_t varItem;
		ULONG nFetched = 0L;
		HRESULT hr;

		ISynthRootAtomPtr spSynthRoot(aSynthRoot);

		IMmShUndCollPtr spUndColl(aUndColl);
		IMmShUndAtomPtr spSynthUnd;

		bool bBadSpotBid = false, bBadSpotAsk = false, bBadSpotLast = false;

		dSpotBid = spSynthRoot->CashValue;
		dSpotAsk = dSpotBid;
		dSpotLast = dSpotBid;

		ISynthRootCompAtomPtr spSRComp;
		ISynthRootCompCollPtr spSRCompColl;

		spSRCompColl = spSynthRoot->SynthRootComponents;


		_CHK(spSRCompColl->get__NewEnum(&spUnk), _T("Fail to get synthetic underlying component collection enum."));
		IEnumVARIANTPtr spSynthRootEnum(spUnk);
		_CHK(spSynthRootEnum->Reset(), _T("Fail to reset synthetic underlying component collection."));
		while((hr = spSynthRootEnum->Next(1L, &varItem, &nFetched)) == S_OK)
		{
			ATLASSERT(varItem.vt == VT_DISPATCH);
			spSRComp = varItem;
			if(spSRComp != NULL)
			{
				LONG nUndID = 0L;
				DOUBLE dWeight = 0.;
				DOUBLE dPrice = 0.;

				nUndID = spSRComp->UndID;
				_CHK(spUndColl->get_Item(nUndID, &spSynthUnd), _T("Fail to get synthetic underlying."));

				if(spSynthUnd != NULL)
				{
					dWeight = spSRComp->Weight;

					_CHK(spSynthUnd->get_PriceBid(&dPrice));
					if(!bBadSpotBid && dPrice > DBL_EPSILON)
						dSpotBid += dPrice * dWeight;
					else
					{
						bBadSpotBid = true;
						dSpotBid = BAD_DOUBLE_VALUE;
					}

					_CHK(spSynthUnd->get_PriceAsk(&dPrice));
					if(!bBadSpotAsk && dPrice > DBL_EPSILON)
						dSpotAsk += dPrice * dWeight;
					else
					{
						bBadSpotAsk = true;
						dSpotAsk = BAD_DOUBLE_VALUE;
					}

					_CHK(spSynthUnd->get_PriceLast(&dPrice));
					if(!bBadSpotLast && dPrice > DBL_EPSILON)
						dSpotLast += dPrice * dWeight;
					else
					{
						bBadSpotLast = true;
						dSpotLast = BAD_DOUBLE_VALUE;
					}
				}
			}
		}
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)CComErrorWrapper::ErrorDescription(e), IID_IMmShUndAtom, e.Error());
	}
	return S_OK;
}

/////////////////////////////////////////////////////////////////////////////
//
void CMmShUndAtom::_GetSyntheticRootBasketDividends(ISynthRootAtomPtr aSynthRoot, REGULAR_DIVIDENDS* pDivs, 
													LONG nMaxCount) throw()
{
	EtsRegularDividend aDiv;

	for(LONG i = 0; i < nMaxCount; i++)
	{
		_CHK(aSynthRoot->get_BasketDiv(i, &aDiv), _T("Fail to get basket dividend."));
		pDivs[i].nLastDivDate = static_cast<LONG>(aDiv.LastDate);
		pDivs[i].nFrequency = aDiv.Freq;
		pDivs[i].dAmount = aDiv.Amt;
	}
}

HRESULT CMmShUndAtom::CalcOptionGreeks(IMmShUndCollPtr collUndColl, IMmShOptAtomPtr aOpt, DOUBLE dSpotPriceMid,
		EtsCalcModelTypeEnum enCalcModel, VARIANT_BOOL bUseTheoVolatility, 
		VARIANT_BOOL bUseTheoVolaNoBid, VARIANT_BOOL bUseTheoVolaBadMarket,
		DOUBLE dUndPriceTolerance, EtsPriceRoundingRuleEnum enPriceRoundingRule,
		ICalculationParametrsPtr spParams)
{
	if(aOpt == NULL || collUndColl == NULL || !(bool)spParams)
		return Error(L"Invalid objects passed.", IID_IMmShUndAtom, E_INVALIDARG);

	try
	{
		CSafeArrayWrapper<double> saDates;
		CSafeArrayWrapper<double> saAmounts;

		IMmShOptAtomPtr		spOpt(aOpt);
		IMmShOptRootAtomPtr spOptRoot;
		_CHK(spOpt->get_OptRoot(&spOptRoot));

		if(spOptRoot == NULL)
			return Error(L"Option has no root assigned.", IID_IMmShUndAtom, E_INVALIDARG);

		VARIANT_BOOL bIsRootSynthetic = VARIANT_FALSE;
		DOUBLE dOptPriceMid = 0., dDelta = BAD_DOUBLE_VALUE;
		EtsReplacePriceStatusEnum enMidPriceStatus = enRpsNone;

		_CHK(spOptRoot->get_IsSynth(&bIsRootSynthetic));
		if(bIsRootSynthetic)
		{
			ISynthRootAtomPtr spSynthOptRoot;
			_CHK(spOptRoot->get_SynthOptRoot(&spSynthOptRoot));

			DOUBLE dSpotBid = 0., dSpotAsk = 0., dSpotLast = 0.;
			_CHK(_GetSyntheticUnderlyingPrice(collUndColl, spSynthOptRoot, dSpotBid, dSpotAsk, dSpotLast, dUndPriceTolerance, enPriceRoundingRule));
			
			dSpotPriceMid = m_spUndPriceProfile->GetUndPriceMid(dSpotBid, dSpotAsk, dSpotLast, dUndPriceTolerance, enPriceRoundingRule, NULL, FALSE);
		}

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
			aGreeks.nMask = GT_DELTA|GT_THEOPRICE;

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

			DATE			dtTemp = 0., dtExpiryOV = 0., tmCloseTime = 0., dtNow = 0.;
			DOUBLE			dYTE = 0.;
			
			_CHK(spOpt->get_ExpiryOV(&dtExpiryOV));
			_CHK(spOpt->get_TradingClose(&tmCloseTime));
			
			ICalculationParametrs* pParams = (ICalculationParametrs*)(spParams);

			::GetNYDateTimeAsDATE(&dtNow);
			::GetCalculationParams(dtNow, dtExpiryOV, tmCloseTime, pParams->UseTimePrecision != VARIANT_FALSE, &dtNow, &dtExpiryOV, &tmCloseTime, &dYTE);

			if(!bIsRootSynthetic)
			{
				EtsDivTypeEnum enDivType = enDivCustomStream;
				IEtsIndexDivAtomPtr spDiv = m_spDividend;
				if(spDiv)
					spDiv->get_DivType(&enDivType);

				switch(enDivType)
				{
				case enDivMarket:
				case enDivCustomPeriodical:
				case enDivCustomStream:
					{
						if (m_spDividend != NULL)
							nDivCount = 0;
						m_spDividend->GetDividendCount2(dtNow, dtExpiryOV, tmCloseTime, &nDivCount);
						if (nDivCount< 0)
							nDivCount = 0;

						if (nDivCount> 0)
						{
							LPSAFEARRAY psaDates = NULL;
							LPSAFEARRAY psaAmounts = NULL;

							m_spDividend->GetDividends2(dtNow, dtExpiryOV, tmCloseTime, nDivCount, &psaAmounts, &psaDates, &nDivCount);
							saDates.Attach(psaDates);
							saAmounts.Attach(psaAmounts);
						}
					}
					break;
				case enDivStockBasket:
					{
						/*dYield = m_dYield;
						if(m_spBasketIndex != NULL)
						{
							LONG nBaskDivCount = 0L; VARIANT_BOOL bIsBasket = VARIANT_FALSE;
							nDivCount = 0;
							IEtsIndexDivCollPtr spDivColl = NULL;
							m_spBasketIndex->get_BasketDivs(&spDivColl);
							_CHK(m_spBasketIndex->get_IsBasket(&bIsBasket));

							if(bIsBasket)
								dYield = 0.0;

							if ( bIsBasket && spDivColl != NULL)
							{
								spDivColl->GetDividendCount(nToday, nExpiry, &nDivCount);
								if(nDivCount > 0L)
								{
									LPSAFEARRAY psaDates = NULL;
									LPSAFEARRAY psaAmounts = NULL;
									spDivColl->GetDividends(nToday, nExpiry, nDivCount, &psaAmounts, &psaDates, &nDivCount);
									saDates.Attach(psaDates);
									saAmounts.Attach(psaAmounts);
								}
							}
						}*/
					}
					break;
				case enDivIndexYield:
					{
						//dYield = m_dYield;
					}
					break;	
				}

				DOUBLE dVola = 0.;
				if (nDivCount < 0 )
					nDivCount = 0;

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
			else // synthetic
			{
				ISynthRootAtomPtr spSynthOptRoot;
				_CHK(spOptRoot->get_SynthOptRoot(&spSynthOptRoot));

				VARIANT_BOOL bIsBasket = VARIANT_FALSE;
				_CHK(spSynthOptRoot->get_Basket(&bIsBasket));
				_CHK(spSynthOptRoot->get_Yield(&dYield));

				DOUBLE dSkew = 0., dKurt = 0.;
				_CHK(spSynthOptRoot->get_Skew(&dSkew));
				_CHK(spSynthOptRoot->get_Kurt(&dKurt));

				if (bIsBasket)
				{
					//_CHK(spSynthOptRoot->get_BasketDivsCount(&nDivCount));

					IEtsIndexDivCollPtr spBasketDivs;
					spSynthOptRoot->get_BasketDivs(&spBasketDivs);
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

				DOUBLE dVola = 0.;

                if (bUseTheoVolatility)
					_CHK(spOpt->get_Vola(&dVola));
                else
				{
                    dVola = 0.;

					if (!bUseTheoVolaNoBid || (bUseTheoVolaNoBid && dOptBid > 0)) 
					{
						if (dOptPriceMid > DBL_EPSILON)
						{
							LONG nFlag = VF_OK;
							dVola = ::CalcVolatilityMM3(dRate, dYield, dHTBRate, dSpotPriceMid, dOptPriceMid, dStrike, 
								dYTE, enOptType, nIsAmerican, nDivCount,
								saAmounts.GetPlainData(), saDates.GetPlainData(), 100L, dSkew, dKurt, nModel, &nFlag);
							

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
					enOptType, nIsAmerican, nDivCount, saAmounts.GetPlainData(), saDates.GetPlainData(), 100L, dSkew, dKurt, nModel, &aGreeks);
				
                
				if(nRetCount != 0L)
				{
					if((aGreeks.nMask & GT_DELTA) && _finite(aGreeks.dDelta))
						dDelta = aGreeks.dDelta;
				}
			}
		}

		_CHK(spOpt->put_Delta(dDelta));
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)CComErrorWrapper::ErrorDescription(e), IID_IMmShUndAtom, e.Error());
	}

	return S_OK;
}

/////////////////////////////////////////////////////////////////////////////
//
void CMmShUndAtom::_CalcSleep(LONG nCalcSleepFreq, LONG nCalcSleepAmt)
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

STDMETHODIMP CMmShUndAtom::CalcAllOptions(IMmShUndColl* collUndColl,
										  EtsCalcModelTypeEnum enCalcModel,
										  VARIANT_BOOL bUseTheoVolatility,
										  VARIANT_BOOL bUseTheoVolaNoBid,
										  VARIANT_BOOL bUseTheoVolaBadMarket,
										  DOUBLE dUndPriceTolerance,
										  enum EtsPriceRoundingRuleEnum enPriceRoundingRule,
										  LONG nCalcSleepFreq,
										  LONG nCalcSleepAmt,
										  ICalculationParametrs* Params)
{
	try
	{
		IUnknownPtr spUnk;
		_variant_t varItem;
		ULONG nFetched = 0L;
		HRESULT hr;

		EtsReplacePriceStatusEnum enMidPriceStatus = enRpsNone;
		DOUBLE dSpotPriceMid = m_spUndPriceProfile->GetUndPriceMid(m_dPriceBid, m_dPriceAsk, m_dPriceLast, dUndPriceTolerance, enPriceRoundingRule, &enMidPriceStatus, FALSE);

		IMmShUndCollPtr spUndColl(collUndColl);
		ICalculationParametrsPtr spParams(Params);

		IMmShOptAtomPtr spOption;
		_CHK(m_spOpt->get__NewEnum(&spUnk), _T("Fail to get options collection."));

		IEnumVARIANTPtr spOptionEnum(spUnk);
		_CHK(spOptionEnum->Reset(), _T("Fail to reset options collection."));

		while((hr = spOptionEnum->Next(1L, &varItem, &nFetched)) == S_OK)
		{
			ATLASSERT(varItem.vt == VT_DISPATCH);
			spOption = varItem;
			if(nFetched > 0 && spOption != NULL)
			{
				_CHK(CalcOptionGreeks(spUndColl, spOption, dSpotPriceMid,
						enCalcModel, bUseTheoVolatility, 
						bUseTheoVolaNoBid, bUseTheoVolaBadMarket,
						dUndPriceTolerance, enPriceRoundingRule,
						spParams),
						_T("Fail to calculate option greeks."));

				_CalcSleep(nCalcSleepFreq, nCalcSleepAmt);
			}
		}
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)CComErrorWrapper::ErrorDescription(e), IID_IMmShUndAtom, e.Error());
	}


	return S_OK;
}

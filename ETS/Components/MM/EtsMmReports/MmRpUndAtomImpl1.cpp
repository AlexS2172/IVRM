// MmRpUndAtomImpl1.cpp : Implementation of CMmRpUndAtom

#include "stdafx.h"
#include "MmRpUndAtom.h"
#include "MmRpGreeksSummaryColl.h"
#include "MmRpGreeksSummaryAtom.h"
#include "MmRpPnLColl.h"
#include "MmRpPnLAtom.h"
#include "MmRpGreeksByMonthColl.h"
#include "MmRpGreeksByMonthAtom.h"
#include "MmRpOptAtom.h"
#include "MmRpExercisedStockColl.h"
#include "MmRpExercisedStockAtom.h"

// CMmRpUndAtom

STDMETHODIMP CMmRpUndAtom::CalcGreeksSummary(EtsCalcModelTypeEnum enCalcModel, 
					SAFEARRAY** psaRates, 
					SAFEARRAY** psaYTEs, 
					SAFEARRAY** psaHTBRates, 
					SAFEARRAY** psaHTBYTEs, 
					VARIANT_BOOL bUseTheoVolatility, 
					VARIANT_BOOL bUseTheoVolaNoBid, 
					VARIANT_BOOL bUseTheoVolaBadMarket, 
					IMmRpUndColl* pUndColl,
					DOUBLE dUndPriceTolerance,
			        EtsPriceRoundingRuleEnum enPriceRoundingRule,
					ICalculationParametrs* pParams,
					IMmRpGreeksSummaryColl** pRetVal)
{
	if(!pUndColl || pParams == NULL)
		return E_INVALIDARG;

	__CHECK_POINTER(pRetVal);

	try
	{
		if(*pRetVal)
		{
			(*pRetVal)->Release();
			*pRetVal = 0;
		}

		HRESULT	hr;
		IMmRpUndCollPtr	spUndColl(pUndColl);

		IMmRpGreeksSummaryCollPtr	spGreeksSummaryColl;
		__CHECK_HRESULT(spGreeksSummaryColl.CreateInstance(CLSID_MmRpGreeksSummaryColl), 
				_T("Failed to create MmRpGreeksSummaryColl object."));

		DOUBLE	dQtyDailyPrevDateBuy = 0.;
		DOUBLE	dQtyDailyPrevDateSell = 0.;
		DOUBLE	dQtyDailyTodayBuy = 0.;
		DOUBLE	dQtyDailyTodaySell = 0.;

		DOUBLE	dPosDailyPrevDateBuy = 0.;
		DOUBLE	dPosDailyPrevDateSell = 0.;
		DOUBLE	dPosDailyTodayBuy = 0.;
		DOUBLE	dPosDailyTodaySell = 0.;		

		__CHECK_HRESULT3(m_spPos->get_QtyDailyPrevDateBuy(&dQtyDailyPrevDateBuy));
		__CHECK_HRESULT3(m_spPos->get_QtyDailyPrevDateSell(&dQtyDailyPrevDateSell));
		__CHECK_HRESULT3(m_spPos->get_QtyDailyTodayBuy(&dQtyDailyTodayBuy));
		__CHECK_HRESULT3(m_spPos->get_QtyDailyTodaySell(&dQtyDailyTodaySell));

		__CHECK_HRESULT3(m_spPos->get_PosDailyPrevDateBuy(&dPosDailyPrevDateBuy));
		__CHECK_HRESULT3(m_spPos->get_PosDailyPrevDateSell(&dPosDailyPrevDateSell));
		__CHECK_HRESULT3(m_spPos->get_PosDailyTodayBuy(&dPosDailyTodayBuy));
		__CHECK_HRESULT3(m_spPos->get_PosDailyTodaySell(&dPosDailyTodaySell));

		DOUBLE	dStockPos = dQtyDailyPrevDateBuy + dQtyDailyTodayBuy - dQtyDailyPrevDateSell - dQtyDailyTodaySell;
		DOUBLE	dStockPosPrice = dPosDailyPrevDateBuy + dPosDailyTodayBuy - dPosDailyPrevDateSell - dPosDailyTodaySell;
		
		DOUBLE dUndBid, dUndAsk, dUndLast;
		m_spPrice->get_Bid ( &dUndBid);
		m_spPrice->get_Ask ( &dUndAsk );
		m_spPrice->get_Last( &dUndLast );
		
		DOUBLE	dSpotPrice;
		VARIANT_BOOL	futureUsed = VARIANT_FALSE;
		 __CHECK_HRESULT3(GetUnderlyingPrice(dUndPriceTolerance, enPriceRoundingRule, NULL, &futureUsed, &dSpotPrice));
		
		DOUBLE	dPriceBid = 0.;
		DOUBLE	dPriceAsk = 0.;
		DOUBLE	dPriceLast = 0.;
		DOUBLE	dPrice = 0.;
		
		EtsOptionTypeEnum	enOptType;

		DATE	dtExpiryOV = 0., tmCloseTime = 0.;
		DOUBLE	dStrike = 0.;
		LONG	nOptID = 0L;
		LONG	nOptRootID = 0L;
		DOUBLE	dVegaWeight = 1.;

		DOUBLE	dCallMktValue = 0.;
		DOUBLE	dPutMktValue = 0.;
		DOUBLE	dCallPoP = 0.;
		DOUBLE	dPutPoP = 0.;

		DOUBLE	dOptionsDelta = 0.;
		DOUBLE	dTotalGamma = 0.;
		DOUBLE	dTotalGammaInShares = 0.;
		DOUBLE	dTotalNetGamma = 0.;
		DOUBLE	dTotalRho = 0.;
		DOUBLE	dTotalVega = 0.;
		DOUBLE	dTotalWeightedVega = 0.;
		DOUBLE	dTotalTheta = 0.;
		DOUBLE	dYTE = 0.;
		
		//DATE	nToday = (DATE)COleDateTime::GetCurrentTime();
		DATE	dtNow = 0.;
		GetNYDateTimeAsDATE(&dtNow);

		GREEKS	aGreeks;
		memset(&aGreeks, 0, sizeof(GREEKS));

		__SynthTotalValuesMap	__SynthTotalValues;

		IUnknownPtr		spOptUnk;
		_variant_t		varItem;
		ULONG			nFetched = 0L;
		IMmRpOptAtomPtr	spOptAtom;

		__CHECK_HRESULT(m_spOpt->get__NewEnum(&spOptUnk), _T("Failed to get options collection."));
		
		IEnumVARIANTPtr spOptEnum(spOptUnk);
		__CHECK_HRESULT(spOptEnum->Reset(), _T("Failed to reset options collection."));

		double savedSpotPrice = dSpotPrice;

		while((hr = spOptEnum->Next(1L, &varItem, &nFetched)) == S_OK)
		{
			ATLASSERT(varItem.vt == VT_DISPATCH);
			spOptAtom = varItem;

			double futurePrice = 0.;
			EtsContractTypeEnum optionType;
			_CHK(spOptAtom->get_ContractType(&optionType) );
			if ( enCtFutOption  == optionType )	{
				IMmRpFutAtomPtr spFut = NULL;
				_CHK( spOptAtom->get_Fut( &spFut) );
				if ( NULL != spFut)
				{
					
					VARIANT_BOOL	futureUsed;
					_CHK(spFut->GetFuturePrice(dUndPriceTolerance,enPriceRoundingRule, NULL, &futureUsed, &futurePrice));
				}
				dSpotPrice = futurePrice;
			} 
			else 
				dSpotPrice = savedSpotPrice;

			IMmRpPosAtomPtr	spOptPosAtom;
			__CHECK_HRESULT3(spOptAtom->get_Pos(&spOptPosAtom));

			__CHECK_HRESULT3(spOptPosAtom->get_QtyInSharesDailyPrevDateBuy(&dQtyDailyPrevDateBuy));
			__CHECK_HRESULT3(spOptPosAtom->get_QtyInSharesDailyPrevDateSell(&dQtyDailyPrevDateSell));
			__CHECK_HRESULT3(spOptPosAtom->get_QtyInSharesDailyTodayBuy(&dQtyDailyTodayBuy));
			__CHECK_HRESULT3(spOptPosAtom->get_QtyInSharesDailyTodaySell(&dQtyDailyTodaySell));

			DOUBLE dOptPos = dQtyDailyPrevDateBuy + dQtyDailyTodayBuy - dQtyDailyPrevDateSell - dQtyDailyTodaySell;
			if (DoubleEQZero(dOptPos))
				continue;

			__CHECK_HRESULT3(spOptAtom->get_ID(&nOptID));
			__CHECK_HRESULT3(spOptAtom->get_ExpiryOV(&dtExpiryOV));
			__CHECK_HRESULT3(spOptAtom->get_TradingClose(&tmCloseTime));
			__CHECK_HRESULT3(spOptAtom->get_OptType(&enOptType));			
			__CHECK_HRESULT3(spOptAtom->get_RootID(&nOptRootID));
			__CHECK_HRESULT3(spOptAtom->get_Strike(&dStrike));


			::GetCalculationParams(dtNow, dtExpiryOV, tmCloseTime, pParams->UseTimePrecision != VARIANT_FALSE, &dtNow, &dtExpiryOV, &tmCloseTime, &dYTE);

			IMMRpPricePtr spPrice ;
			spOptAtom->get_Price( &spPrice ) ;
			ATLASSERT ( spPrice ) ;

			__CHECK_HRESULT3(spPrice->get_Bid(&dPriceBid));
			__CHECK_HRESULT3(spPrice->get_Ask(&dPriceAsk));
			__CHECK_HRESULT3(spPrice->get_Last(&dPriceLast));

			dStrike = _AdjustFractionalDigits(dStrike, 4);

			DOUBLE dOptPriceMid = m_spOptPriceProfile->GetOptPriceMid(dPriceBid, dPriceAsk, dPriceLast, enPriceRoundingRule, bUseTheoVolatility, 0., NULL);

			DOUBLE dOptBidForPnL = m_spOptPriceProfile->GetOptPriceBidForPnL(dPriceBid, dPriceAsk, dPriceLast, enPriceRoundingRule, bUseTheoVolatility, 0., NULL);
			DOUBLE dOptAskForPnL = m_spOptPriceProfile->GetOptPriceAskForPnL(dPriceBid, dPriceAsk, dPriceLast, enPriceRoundingRule, bUseTheoVolatility, 0., NULL);

			DOUBLE dOptPrice = 0.;

			VARIANT_BOOL bUseMidMarketForPnL = m_spOptPriceProfile->UseMidMarketForPnL;
			VARIANT_BOOL bUseZeroBidRule = m_spOptPriceProfile->UseZeroBidRule;
			if (bUseMidMarketForPnL == VARIANT_TRUE && bUseZeroBidRule == VARIANT_FALSE)
			{
				dOptPrice = dOptPriceMid;
			}
			else
			{
				if(dOptPos > 0.)
				{
					dOptPrice = dOptBidForPnL;
				}
				else
				{
					dOptPrice = dOptAskForPnL;
				}
			}

			double dSaveCallMktValue = dCallMktValue;
			double dSavePutMktValue  = dPutMktValue;
			if (enOptType == enOtCall)
                dCallMktValue += dOptPrice * dOptPos;
			else
				dPutMktValue += dOptPrice * dOptPos;

			bool					bIsRootSynthetic = false;
			ISynthRootAtomPtr		spSynthRootAtom;
			ISynthRootCompCollPtr	spSynthRootCompColl;
			ISynthRootCompAtomPtr	spSynthRootCompAtom;
				
			IUnknownPtr				spSynthUnk;
			_variant_t				varSynthItem;
			ULONG					nSynthFetched = 0L;
			IEnumVARIANTPtr			spSynthEnum;
				
			LONG					nSynthUndID = 0L;
			DOUBLE					dSynthWeight = 0L;
			DOUBLE					dCashValue = 0.;
			
			if (m_vbHasSynthetic == VARIANT_TRUE)
			{
				spSynthRootAtom = m_spSyntheticRoots->Item[nOptRootID];
				if (spSynthRootAtom != NULL) //is synthetic
				{
					__CHECK_HRESULT3(spSynthRootAtom->get_CashValue(&dCashValue));

					bIsRootSynthetic = true;
					spSynthRootCompColl = spSynthRootAtom->SynthRootComponents;
					ATLASSERT(spSynthRootCompColl!=NULL);

					spSynthUnk = spSynthRootCompColl->_NewEnum;
					spSynthEnum = spSynthUnk;
				}
			}

			LONG nRetCount = 0L;

			if (dtNow <= dtExpiryOV)
			{
				if (bIsRootSynthetic)
				{
					DOUBLE dTempPriceBid = dPriceBid;
					DOUBLE dTempPriceAsk = dPriceAsk;
					DOUBLE dTempPriceLast = dPriceLast;
					_GetSyntheticPrice(spSynthRootAtom, pUndColl, dTempPriceBid, dTempPriceAsk, dTempPriceLast);
					dPrice = m_spUndPriceProfile->GetUndPriceMid(dTempPriceBid, dTempPriceAsk, dTempPriceLast, dUndPriceTolerance, enPriceRoundingRule, NULL, VARIANT_FALSE );
				}
				else
				{	
					dPrice = dSpotPrice;
				}

				if (m_vbIsAmerican != VARIANT_TRUE)
				{
					DOUBLE	dYield = 0.;

					if (bIsRootSynthetic)
					{
						dYield = spSynthRootAtom->Yield;						
						dPrice = _CalcSyntheticForwardPrice(spSynthRootAtom,
															dPrice, 
															dtExpiryOV, 
															tmCloseTime, 
															dtNow, 
															dYield, 
															*psaRates, 
															*psaYTEs,
															ICalculationParametrsPtr(pParams));
					}
					else
					{
						dYield = m_dYield;
						dPrice = _CalcRegularForwardPrice(	dPrice, 
															dtExpiryOV, 
															tmCloseTime, 
															dtNow, 
															dYield, 
															*psaRates, 
															*psaYTEs,
															ICalculationParametrsPtr(pParams));
					}
				}

				double dSaveCallPoP = dCallPoP;
				double dSavePutPoP = dPutPoP;
				if (dPrice > 0. && dOptPrice > 0.)
				{
					DOUBLE	dTimeValue = dOptPrice;
					if (enOptType == enOtCall)
					{
						if (dPrice - dStrike > 0.)
						{
							dTimeValue -= dPrice - dStrike;
						}

						dCallPoP += dOptPos * max(0., dTimeValue);
					}
					else
					{
						if (dStrike - dPrice > 0.)
						{
							dTimeValue -= dStrike - dPrice;
						}

						dPutPoP += dOptPos * max(0, dTimeValue);
					}
				}				

				memset(&aGreeks, 0, sizeof(GREEKS));
				aGreeks.nMask = GT_ALL;

				nRetCount = _CalcOptionGreeks(	enCalcModel, 
												spOptAtom, 
												_InterpolateRate(dYTE, *psaRates, *psaYTEs, ICalculationParametrsPtr(pParams)), 
												_InterpolateRate(dYTE, *psaHTBRates, *psaHTBYTEs, ICalculationParametrsPtr(pParams)),
												bUseTheoVolatility, 
												bUseTheoVolaNoBid, 
												bUseTheoVolaBadMarket, 
												pUndColl, 
												dUndPriceTolerance,
												enPriceRoundingRule, 
												aGreeks,
												ICalculationParametrsPtr(pParams));

				__CHECK_HRESULT3(spOptAtom->get_VegaWeight(&dVegaWeight));

				if(dPriceBid<=0 || dPriceAsk <= 0 || dPriceLast <= 0 || dOptPriceMid <= 0)
				{
					dOptPriceMid = m_spOptPriceProfile->GetOptPriceMid(dPriceBid, dPriceAsk, dPriceLast, enPriceRoundingRule, bUseTheoVolatility, aGreeks.dTheoPrice, NULL);

					dOptBidForPnL = m_spOptPriceProfile->GetOptPriceBidForPnL(dPriceBid, dPriceAsk, dPriceLast, enPriceRoundingRule, bUseTheoVolatility, aGreeks.dTheoPrice, NULL);
					dOptAskForPnL = m_spOptPriceProfile->GetOptPriceAskForPnL(dPriceBid, dPriceAsk, dPriceLast, enPriceRoundingRule, bUseTheoVolatility, aGreeks.dTheoPrice, NULL);

					dCallPoP = dSaveCallPoP;
					dPutPoP = dSavePutPoP;

					dCallMktValue = dSaveCallMktValue;
					dPutMktValue  = dSavePutMktValue;

					if (bUseMidMarketForPnL == VARIANT_TRUE && bUseZeroBidRule == VARIANT_FALSE)
						dOptPrice = dOptPriceMid;
					else
					{
						if(dOptPos > 0.)
							dOptPrice = dOptBidForPnL;
						else
							dOptPrice = dOptAskForPnL;
					}

					if (enOptType == enOtCall)
						dCallMktValue += dOptPrice * dOptPos;
					else
						dPutMktValue += dOptPrice * dOptPos;

					if (dPrice > 0. && dOptPrice > 0.)
					{
						DOUBLE	dTimeValue = dOptPrice;
						if (enOptType == enOtCall)
						{
							if (dPrice - dStrike > 0.)
							{
								dTimeValue -= dPrice - dStrike;
							}

							dCallPoP += dOptPos * max(0., dTimeValue);
						}
						else
						{
							if (dStrike - dPrice > 0.)
							{
								dTimeValue -= dStrike - dPrice;
							}

							dPutPoP += dOptPos * max(0, dTimeValue);
						}
					}				

				}
			}

			if (!bIsRootSynthetic)
			{
				if (nRetCount)
				{
					if (_finite(aGreeks.dDelta))
					{
						dOptionsDelta += aGreeks.dDelta * dOptPos;
					}
				
					if (_finite(aGreeks.dGamma))
					{
						dTotalGamma += aGreeks.dGamma * dOptPos;
					}
				}
			}
			else // synthetic
			{
				if (dCashValue > 0.)
				{
					DOUBLE dSynthOptionsDelta = 0.;
					DOUBLE dSynthTotalGamma = 0.;

					if(nRetCount)
					{
						if(_finite(aGreeks.dDelta))
						{
							dSynthOptionsDelta = aGreeks.dDelta * dOptPos * dCashValue;
						}

						if(_finite(aGreeks.dGamma))
						{
							dSynthTotalGamma = aGreeks.dGamma * dOptPos * dCashValue;
						}
					}

					__SynthTotalValuesMap::iterator iter = __SynthTotalValues.find(__USD_ID__);
					if (iter == __SynthTotalValues.end())
					{
						__MmRpSynthTotalValues tValues;
						tValues.m_nID = __USD_ID__;
						tValues.m_dPrice = __USD_PRICE__;
					
						tValues.m_bstrSymbol.Append(m_bstrSymbol);
						tValues.m_bstrSymbol.Append(L"(");
						tValues.m_bstrSymbol.Append(__USD_SYMBOL__);
						tValues.m_bstrSymbol.Append(L"*)");

						tValues.m_dOptionsDelta = dSynthOptionsDelta;
						tValues.m_dTotalGamma = dSynthTotalGamma;

						__SynthTotalValues.insert(std::make_pair(__USD_ID__, tValues));
					}
					else
					{
						__MmRpSynthTotalValues& tValues = iter->second;
						tValues.m_dOptionsDelta += dSynthOptionsDelta;
						tValues.m_dTotalGamma += dSynthTotalGamma;					
					}
				}

				__CHECK_HRESULT(spSynthEnum->Reset(), _T("Fail to reset synthetic roots components collection."));
				while((hr = spSynthEnum->Next(1L, &varSynthItem, &nSynthFetched)) == S_OK)
				{
					ATLASSERT(varItem.vt == VT_DISPATCH);
					spSynthRootCompAtom = varSynthItem;

					__CHECK_HRESULT3(spSynthRootCompAtom->get_UndID(&nSynthUndID));
					__CHECK_HRESULT3(spSynthRootCompAtom->get_Weight(&dSynthWeight));

					if (nSynthUndID != m_nID)
					{
						IMmRpUndAtomPtr spUndAtom;
						__CHECK_HRESULT3(spUndColl->get_Item(_bstr_t(nSynthUndID), &spUndAtom));
						if (spUndAtom != NULL)
						{
							DOUBLE dSynthOptionsDelta = 0.;
							DOUBLE dSynthTotalGamma = 0.;

							if(nRetCount)
							{
								if(_finite(aGreeks.dDelta))
								{
									dSynthOptionsDelta = aGreeks.dDelta * dOptPos * dSynthWeight;									
								}

								if(_finite(aGreeks.dGamma))
								{
									dSynthTotalGamma = aGreeks.dGamma * dOptPos * dSynthWeight;
								}
							}

							__SynthTotalValuesMap::iterator iter = __SynthTotalValues.find(nSynthUndID);
							if (iter == __SynthTotalValues.end())
							{
								__MmRpSynthTotalValues tValues;
								tValues.m_nID = nSynthUndID;
								tValues.m_dOptionsDelta = dSynthOptionsDelta;
								tValues.m_dTotalGamma = dSynthTotalGamma;

								IMMRpPricePtr spPrice ;
								spUndAtom->get_Price ( &spPrice ) ;

								__CHECK_HRESULT3(spPrice->get_Bid(&dPriceBid));
								__CHECK_HRESULT3(spPrice->get_Ask(&dPriceAsk));
								__CHECK_HRESULT3(spPrice->get_Last(&dPriceLast));

								tValues.m_dPrice = m_spUndPriceProfile->GetUndPriceMid(dPriceBid, dPriceAsk, dPriceLast, dUndPriceTolerance, enPriceRoundingRule, NULL, VARIANT_FALSE );
							
								CComBSTR bstrSynthCompSymbol;

								__CHECK_HRESULT3(spUndAtom->get_Symbol(&bstrSynthCompSymbol));
								__CHECK_HRESULT3(spUndAtom->get_UndType(&tValues.m_enUndType));

								tValues.m_bstrSymbol.Append(m_bstrSymbol);
								tValues.m_bstrSymbol.Append(L"(");
								tValues.m_bstrSymbol.Append(bstrSynthCompSymbol);
								tValues.m_bstrSymbol.Append(L"*)");

								__SynthTotalValues.insert(std::make_pair(nSynthUndID, tValues));
							}
							else
							{
								__MmRpSynthTotalValues& tValues = iter->second;
								tValues.m_dOptionsDelta += dSynthOptionsDelta;
								tValues.m_dTotalGamma += dSynthTotalGamma;
							}
						}
					}
					else
					{
						if (nRetCount)
						{
							if(_finite(aGreeks.dDelta))
							{
								dOptionsDelta += aGreeks.dDelta*dOptPos*dSynthWeight;
							}
							if(_finite(aGreeks.dGamma))
							{
								dTotalGamma += aGreeks.dGamma*dOptPos*dSynthWeight;
							}
						}
					}
				}
			}

			if (nRetCount)
			{
				dTotalVega += aGreeks.dVega * dOptPos;
                dTotalWeightedVega += dVegaWeight * aGreeks.dVega * dOptPos;
                dTotalTheta += aGreeks.dTheta * dOptPos;
                dTotalRho += aGreeks.dRho * dOptPos;
			}	
		}

		IMmRpGreeksSummaryAtomPtr spGreeksSummaryAtom;
		__CHECK_HRESULT3(spGreeksSummaryColl->Add(m_nID, NULL, &spGreeksSummaryAtom));

		__CHECK_HRESULT3(spGreeksSummaryAtom->put_ID(m_nID));
		__CHECK_HRESULT3(spGreeksSummaryAtom->put_UndType(m_enUndType));
		__CHECK_HRESULT3(spGreeksSummaryAtom->put_Symbol(m_bstrSymbol));
		__CHECK_HRESULT3(spGreeksSummaryAtom->put_CallMarketValue(dCallMktValue));
		__CHECK_HRESULT3(spGreeksSummaryAtom->put_PutMarketValue(dPutMktValue));
		__CHECK_HRESULT3(spGreeksSummaryAtom->put_NetMarketValue(dCallMktValue + dPutMktValue));
		__CHECK_HRESULT3(spGreeksSummaryAtom->put_CallPoP(dCallPoP));
		__CHECK_HRESULT3(spGreeksSummaryAtom->put_PutPoP(dPutPoP));
		__CHECK_HRESULT3(spGreeksSummaryAtom->put_NetPoP(dCallPoP + dPutPoP));
		__CHECK_HRESULT3(spGreeksSummaryAtom->put_OptionsDeltaInShares(dOptionsDelta));
		__CHECK_HRESULT3(spGreeksSummaryAtom->put_OptionsDelta(dOptionsDelta * dSpotPrice));
		__CHECK_HRESULT3(spGreeksSummaryAtom->put_UndPosInShares(dStockPos));
		__CHECK_HRESULT3(spGreeksSummaryAtom->put_UndPos(dStockPos * dSpotPrice));
		__CHECK_HRESULT3(spGreeksSummaryAtom->put_NetPosInShares(dOptionsDelta + dStockPos));
		__CHECK_HRESULT3(spGreeksSummaryAtom->put_NetPos((dOptionsDelta + dStockPos)*dSpotPrice));
		__CHECK_HRESULT3(spGreeksSummaryAtom->put_TotalGammaInShares(dTotalGamma*dSpotPrice / 100.));
		__CHECK_HRESULT3(spGreeksSummaryAtom->put_TotalGamma(dTotalGamma*dSpotPrice*dSpotPrice / 100.));
		__CHECK_HRESULT3(spGreeksSummaryAtom->put_TotalRho(dTotalRho));
		__CHECK_HRESULT3(spGreeksSummaryAtom->put_TotalVega(dTotalVega));
		__CHECK_HRESULT3(spGreeksSummaryAtom->put_TotalWeightedVega(dTotalWeightedVega));		
		__CHECK_HRESULT3(spGreeksSummaryAtom->put_TotalTheta(dTotalTheta));

		__SynthTotalValuesMap::iterator iter = __SynthTotalValues.begin();
		while (iter != __SynthTotalValues.end())
		{	
			__MmRpSynthTotalValues& tValues = iter->second;

			spGreeksSummaryAtom = 0;
			__CHECK_HRESULT3(spGreeksSummaryColl->Add(tValues.m_nID, NULL, &spGreeksSummaryAtom));

			__CHECK_HRESULT3(spGreeksSummaryAtom->put_ID(tValues.m_nID));
			
			if (iter->first != __USD_ID__)
			{
				__CHECK_HRESULT3(spGreeksSummaryAtom->put_UndType(tValues.m_enUndType));
			}

			__CHECK_HRESULT3(spGreeksSummaryAtom->put_Symbol(tValues.m_bstrSymbol));
			__CHECK_HRESULT3(spGreeksSummaryAtom->put_OptionsDeltaInShares(tValues.m_dOptionsDelta));
			__CHECK_HRESULT3(spGreeksSummaryAtom->put_OptionsDelta(tValues.m_dPrice * tValues.m_dOptionsDelta));
			__CHECK_HRESULT3(spGreeksSummaryAtom->put_NetPosInShares(tValues.m_dOptionsDelta));
			__CHECK_HRESULT3(spGreeksSummaryAtom->put_NetPos(tValues.m_dPrice * tValues.m_dOptionsDelta));
			__CHECK_HRESULT3(spGreeksSummaryAtom->put_TotalGammaInShares(tValues.m_dTotalGamma * tValues.m_dPrice / 100.));
			__CHECK_HRESULT3(spGreeksSummaryAtom->put_TotalGamma(tValues.m_dTotalGamma * tValues.m_dPrice * tValues.m_dPrice / 100.));
			__CHECK_HRESULT3(spGreeksSummaryAtom->put_TotalNetGamma(tValues.m_dTotalGamma));

			iter++;
		}

		*pRetVal = spGreeksSummaryColl.Detach();
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)CComErrorWrapper::ErrorDescription(e), IID_IMmRpUndAtom, e.Error());
	}

	return S_OK;
}

STDMETHODIMP CMmRpUndAtom::CalcGreeksByMonth(EtsCalcModelTypeEnum enCalcModel, 
					SAFEARRAY** psaRates, 
					SAFEARRAY** psaYTEs, 
					SAFEARRAY** psaHTBRates, 
					SAFEARRAY** psaHTBYTEs, 
					VARIANT_BOOL bUseTheoVolatility, 
					VARIANT_BOOL bUseTheoVolaNoBid, 
					VARIANT_BOOL bUseTheoVolaBadMarket, 
					IMmRpUndColl* pUndColl,
					DOUBLE dUndPriceTolerance,
			        EtsPriceRoundingRuleEnum enPriceRoundingRule,
					ICalculationParametrs* pParams,
					IMmRpGreeksByMonthColl** pRetVal)
{
	HRESULT hr;
	hr = CalcGreeksByMonthExt(	enCalcModel,
								psaRates,
								psaYTEs,
                                                                psaHTBRates,
                                                                psaHTBYTEs,
								bUseTheoVolatility,
								bUseTheoVolaNoBid,
								bUseTheoVolaBadMarket,
								pUndColl,
								dUndPriceTolerance,
								enPriceRoundingRule,
								1.0,
								pParams,
								pRetVal);

	return hr;
}

STDMETHODIMP CMmRpUndAtom::CalcGreeksByMonthExt(EtsCalcModelTypeEnum enCalcModel, 
					SAFEARRAY** psaRates, 
					SAFEARRAY** psaYTEs, 
					SAFEARRAY** psaHTBRates, 
					SAFEARRAY** psaHTBYTEs, 
					VARIANT_BOOL bUseTheoVolatility, 
					VARIANT_BOOL bUseTheoVolaNoBid, 
					VARIANT_BOOL bUseTheoVolaBadMarket, 
					IMmRpUndColl* pUndColl,
					DOUBLE dUndPriceTolerance,
			        EtsPriceRoundingRuleEnum enPriceRoundingRule,
					DOUBLE dUndPriceMultiplier,
					ICalculationParametrs* pParams,
					IMmRpGreeksByMonthColl** pRetVal)
{
	if(!pUndColl || pParams == NULL)
		return E_INVALIDARG;

	__CHECK_POINTER(pRetVal);

	try
	{
		if(*pRetVal)
		{
			(*pRetVal)->Release();
			*pRetVal = 0;
		}

		HRESULT	hr;
		IMmRpUndCollPtr	spUndColl(pUndColl);

		IMmRpGreeksByMonthCollPtr	spGreeksByMonthColl;
		__CHECK_HRESULT(spGreeksByMonthColl.CreateInstance(CLSID_MmRpGreeksByMonthColl), 
				_T("Failed to create MmRpGreeksByMonthColl object."));

		DOUBLE	dPriceBid = 0.;
		DOUBLE	dPriceAsk = 0.;
		DOUBLE	dPriceLast = 0.;
		DOUBLE	dPrice = 0.;

		m_spPrice->get_Ask ( &dPriceAsk ) ;
		m_spPrice->get_Bid ( &dPriceBid ) ;
		m_spPrice->get_Last ( &dPriceLast ) ;


		DOUBLE dSpotPrice;
		VARIANT_BOOL futureUsed = VARIANT_FALSE;
		__CHECK_HRESULT3(GetUnderlyingPrice(dUndPriceTolerance, enPriceRoundingRule, NULL, &futureUsed, &dSpotPrice));
		dSpotPrice = dSpotPrice*dUndPriceMultiplier;

		DOUBLE	dQtyDailyPrevDateBuy = 0.;
		DOUBLE	dQtyDailyPrevDateSell = 0.;
		DOUBLE	dQtyDailyTodayBuy = 0.;
		DOUBLE	dQtyDailyTodaySell = 0.;

		EtsOptionTypeEnum	enOptType;

		DATE	dtExpiry = 0., dtExpiryOV = 0.,tmCloseTime = 0.;
		DOUBLE	dStrike = 0.;
		LONG	nOptID = 0L;
		LONG	nOptRootID = 0L;
		DOUBLE	dVegaWeight = 1.;

		//DATE	nToday = (DATE)COleDateTime::GetCurrentTime();
		DATE	dtNow = 0.0;
		GetNYDateTimeAsDATE(&dtNow);
		DOUBLE	dYTE = 0.;

		GREEKS	aGreeks;
		memset(&aGreeks, 0, sizeof(GREEKS));

		IUnknownPtr			spOptUnk;
		_variant_t			varItem;
		ULONG				nFetched = 0L;
		IMmRpOptAtomPtr		spOptAtom;

		__CHECK_HRESULT(m_spOpt->get__NewEnum(&spOptUnk), _T("Failed to get options collection."));
		
		IEnumVARIANTPtr spOptEnum(spOptUnk);
		__CHECK_HRESULT(spOptEnum->Reset(), _T("Failed to reset options collection."));
		double savedSpotPrice = dSpotPrice;

		while((hr = spOptEnum->Next(1L, &varItem, &nFetched)) == S_OK)
		{
			ATLASSERT(varItem.vt == VT_DISPATCH);
			spOptAtom = varItem;

			/* ------------------------------------------------------------------------*/
			//		Get price of base asset for further calculation:
			//			index/stock for option 
			//			future for future option
			//
			double futurePrice = 0.;
			EtsContractTypeEnum optionType;
			_CHK(spOptAtom->get_ContractType(&optionType) );
			if ( enCtFutOption  == optionType )	{
				IMmRpFutAtomPtr spFut = NULL;
				_CHK( spOptAtom->get_Fut( &spFut) );
				if ( NULL != spFut)
				{
					
					VARIANT_BOOL	futureUsed;
					_CHK(spFut->GetFuturePrice(dUndPriceTolerance,enPriceRoundingRule, NULL, &futureUsed, &futurePrice));
				}
				dSpotPrice = futurePrice;
			} 
			else 
				dSpotPrice = savedSpotPrice;
			/*  -------------------- end of base asset price definition -----------------*/

			IMmRpPosAtomPtr	spOptPosAtom;
			__CHECK_HRESULT3(spOptAtom->get_Pos(&spOptPosAtom));

			__CHECK_HRESULT3(spOptPosAtom->get_QtyInSharesDailyPrevDateBuy(&dQtyDailyPrevDateBuy));
			__CHECK_HRESULT3(spOptPosAtom->get_QtyInSharesDailyPrevDateSell(&dQtyDailyPrevDateSell));
			__CHECK_HRESULT3(spOptPosAtom->get_QtyInSharesDailyTodayBuy(&dQtyDailyTodayBuy));
			__CHECK_HRESULT3(spOptPosAtom->get_QtyInSharesDailyTodaySell(&dQtyDailyTodaySell));

			DOUBLE dOptPos = dQtyDailyPrevDateBuy + dQtyDailyTodayBuy - dQtyDailyPrevDateSell - dQtyDailyTodaySell;
			if (DoubleEQZero(dOptPos))
				continue;

			__CHECK_HRESULT3(spOptAtom->get_ID(&nOptID));
			__CHECK_HRESULT3(spOptAtom->get_Expiry(&dtExpiry));
			__CHECK_HRESULT3(spOptAtom->get_ExpiryOV(&dtExpiryOV));
			__CHECK_HRESULT3(spOptAtom->get_TradingClose(&tmCloseTime));
			__CHECK_HRESULT3(spOptAtom->get_OptType(&enOptType));
			__CHECK_HRESULT3(spOptAtom->get_RootID(&nOptRootID));
			__CHECK_HRESULT3(spOptAtom->get_Strike(&dStrike));

			IMMRpPricePtr spPrice ;
			spOptAtom->get_Price( &spPrice ) ;
			ATLASSERT ( spPrice ) ;

			__CHECK_HRESULT3(spPrice->get_Bid(&dPriceBid));
			__CHECK_HRESULT3(spPrice->get_Ask(&dPriceAsk));
			__CHECK_HRESULT3(spPrice->get_Last(&dPriceLast));

			dStrike = _AdjustFractionalDigits(dStrike, 4);

			DOUBLE dOptPriceMid = m_spOptPriceProfile->GetOptPriceMid(dPriceBid, dPriceAsk, dPriceLast, enPriceRoundingRule, bUseTheoVolatility, 0., NULL);

			DOUBLE dOptBidForPnL = m_spOptPriceProfile->GetOptPriceBidForPnL(dPriceBid, dPriceAsk, dPriceLast, enPriceRoundingRule, bUseTheoVolatility, 0., NULL);
			DOUBLE dOptAskForPnL = m_spOptPriceProfile->GetOptPriceAskForPnL(dPriceBid, dPriceAsk, dPriceLast, enPriceRoundingRule, bUseTheoVolatility, 0., NULL);

			DOUBLE	dCallMktValue = 0.;
			DOUBLE	dPutMktValue = 0.;
			DOUBLE	dCallPoP = 0.;
			DOUBLE	dPutPoP = 0.;

			DOUBLE dOptionsDelta = 0.;
			DOUBLE dTotalGamma = 0.;
			DOUBLE dTotalRho = 0.;
			DOUBLE dTotalVega = 0.;
			DOUBLE dTotalWeightedVega = 0.;
			DOUBLE dTotalTheta = 0.;

			DOUBLE dOptPrice = 0.;

			VARIANT_BOOL bUseMidMarketForPnL = m_spOptPriceProfile->UseMidMarketForPnL;
			VARIANT_BOOL bUseZeroBidRules = m_spOptPriceProfile->UseZeroBidRule;
			if (bUseMidMarketForPnL == VARIANT_TRUE && bUseZeroBidRules == VARIANT_FALSE)
			{
				dOptPrice = dOptPriceMid;
			}
			else
			{
				if(dOptPos > 0.)
				{
					dOptPrice = dOptBidForPnL;
				}
				else
				{
					dOptPrice = dOptAskForPnL;
				}
			}

			if (enOptType == enOtCall)
			{
                dCallMktValue = dOptPrice * dOptPos;
				dPutMktValue = 0.;
			}
			else
			{
				dPutMktValue = dOptPrice * dOptPos;
				dCallMktValue = 0.;
			}

			bool					bIsRootSynthetic = false;
			ISynthRootAtomPtr		spSynthRootAtom;
			ISynthRootCompCollPtr	spSynthRootCompColl;
			ISynthRootCompAtomPtr	spSynthRootCompAtom;
				
			IUnknownPtr				spSynthUnk;
			_variant_t				varSynthItem;
			ULONG					nSynthFetched = 0L;
			IEnumVARIANTPtr			spSynthEnum;
				
			LONG					nSynthUndID = 0L;
			DOUBLE					dSynthWeight = 0L;
			DOUBLE					dCashValue = 0.;
			
			if (m_vbHasSynthetic == VARIANT_TRUE)
			{
				spSynthRootAtom = m_spSyntheticRoots->Item[nOptRootID];
				if (spSynthRootAtom != NULL) //is synthetic
				{
					__CHECK_HRESULT3(spSynthRootAtom->get_CashValue(&dCashValue));

					bIsRootSynthetic = true;
					spSynthRootCompColl = spSynthRootAtom->SynthRootComponents;
					ATLASSERT(spSynthRootCompColl!=NULL);

					spSynthUnk = spSynthRootCompColl->_NewEnum;
					spSynthEnum = spSynthUnk;
				}
			}

			LONG	nRetCount = 0L;
			
			::GetCalculationParams(dtNow, dtExpiryOV, tmCloseTime, pParams->UseTimePrecision != VARIANT_FALSE, &dtNow, &dtExpiryOV, &tmCloseTime, &dYTE);

			if (dtNow <= dtExpiryOV)
			{

				if (bIsRootSynthetic)
				{
					DOUBLE dTempPriceBid = dPriceBid;
					DOUBLE dTempPriceAsk = dPriceAsk;
					DOUBLE dTempPriceLast = dPriceLast;
					_GetSyntheticPrice(spSynthRootAtom, pUndColl, dTempPriceBid, dTempPriceAsk, dTempPriceLast);
					dPrice = m_spUndPriceProfile->GetUndPriceMid(dTempPriceBid, dTempPriceAsk, dTempPriceLast, dUndPriceTolerance, enPriceRoundingRule, NULL, VARIANT_FALSE );
				}
				else

				{
					dPrice = dSpotPrice;
				}

				if (m_vbIsAmerican != VARIANT_TRUE)
				{
					DOUBLE dYield = 0.;

					if (bIsRootSynthetic)
					{
						dYield = spSynthRootAtom->Yield;						
						dPrice = _CalcSyntheticForwardPrice(spSynthRootAtom, 
															dPrice, 
															dtExpiryOV, 
															tmCloseTime, 
															dtNow, 
															dYield, 
															*psaRates, 
															*psaYTEs,
															ICalculationParametrsPtr(pParams));
					}
					else
					{
						dYield = m_dYield;
						dPrice = _CalcRegularForwardPrice(	dPrice,
															dtExpiryOV,
															tmCloseTime, 
															dtNow, 
															dYield, 
															*psaRates, 
															*psaYTEs,
															ICalculationParametrsPtr(pParams));
					}
				}

				if (dPrice > 0. && dOptPrice > 0.)
				{
					DOUBLE dTimeValue = dOptPrice;
                    if (enOptType == enOtCall)
					{
						if (dPrice - dStrike > 0.)
						{
							dTimeValue -= dPrice - dStrike;
						}

						dCallPoP = dOptPos * max(0.,dTimeValue);
						dPutPoP = 0.;
					}
					else
					{
						if (dStrike - dPrice > 0.)
						{
							dTimeValue -= dStrike - dPrice;
						}

						dPutPoP = dOptPos * max(0., dTimeValue);
						dCallPoP = 0.;
					}
				}
				else
				{
					dCallPoP = 0.;
					dPutPoP = 0.;
				}

				memset(&aGreeks, 0, sizeof(GREEKS));
				aGreeks.nMask = GT_ALL;

				nRetCount = _CalcOptionGreeks(	enCalcModel, 
												spOptAtom, 
												_InterpolateRate(dYTE, *psaRates, *psaYTEs, ICalculationParametrsPtr(pParams)), 
												_InterpolateRate(dYTE, *psaHTBRates, *psaHTBYTEs, ICalculationParametrsPtr(pParams)),
												bUseTheoVolatility, 
												bUseTheoVolaNoBid, 
												bUseTheoVolaBadMarket, 
												spUndColl, 
												dUndPriceTolerance,
												enPriceRoundingRule, 
												aGreeks,
												ICalculationParametrsPtr(pParams),
												dPrice);

				__CHECK_HRESULT3(spOptAtom->get_VegaWeight(&dVegaWeight));
				//if(DoubleEQZero(dOptPriceMid) && DoubleEQZero(dOptPrice))
				{
					dOptPriceMid = m_spOptPriceProfile->GetOptPriceMid(dPriceBid, dPriceAsk, dPriceLast, enPriceRoundingRule, bUseTheoVolatility, aGreeks.dTheoPrice, NULL);
					if (bUseMidMarketForPnL == VARIANT_TRUE && 	bUseZeroBidRules == VARIANT_FALSE)
					{
						dOptPrice = dOptPriceMid;
					}
					else
					{
						if(dOptPos > 0.)
						{
							dOptPrice = dOptBidForPnL = m_spOptPriceProfile->GetOptPriceBidForPnL(dPriceBid, dPriceAsk, dPriceLast, enPriceRoundingRule, bUseTheoVolatility, aGreeks.dTheoPrice, NULL);
						}
						else
						{
							 dOptPrice = dOptAskForPnL = m_spOptPriceProfile->GetOptPriceAskForPnL(dPriceBid, dPriceAsk, dPriceLast, enPriceRoundingRule, bUseTheoVolatility, aGreeks.dTheoPrice, NULL);
						}
					}

					if (dPrice > 0. && dOptPrice > 0.)
					{
						DOUBLE dTimeValue = dOptPrice;
						if (enOptType == enOtCall)
						{
							if (dPrice - dStrike > 0.)
							{
								dTimeValue -= dPrice - dStrike;
							}

							dCallPoP = max(0., dTimeValue)*dOptPos ;
							dPutPoP = 0.;
						}
						else
						{
							if (dStrike - dPrice > 0.)
								dTimeValue -= dStrike - dPrice;

							dPutPoP = max(0., dTimeValue)*dOptPos ;
							dCallPoP = 0.;
						}
						if (enOptType == enOtCall)
						{
							dCallMktValue = dOptPrice * dOptPos;
							dPutMktValue = 0.;
						}
						else
						{
							dPutMktValue = dOptPrice * dOptPos;
							dCallMktValue = 0.;
						}
					}
					else
					{
						dCallPoP = 0.;
						dPutPoP = 0.;
					}
				}

			}

			if (!bIsRootSynthetic)
			{
				if (nRetCount)
				{
					if (_finite(aGreeks.dDelta))
					{
						dOptionsDelta = aGreeks.dDelta * dOptPos;
					}

					if (_finite(aGreeks.dGamma))
					{
						dTotalGamma = aGreeks.dGamma * dOptPos;
						

					}
				}
			}
			else // synthetic
			{	
				IMmRpGreeksByMonthAtomPtr spGreeksByMonthAtom;
				_bstr_t bsKey;

				if (dCashValue > 0.)
				{
					bsKey = (_bstr_t)_variant_t(nOptRootID) + L"_" + (_bstr_t)_variant_t(nOptID) + L"_" + _bstr_t(__USD_SYMBOL__);
				
					__CHECK_HRESULT3(spGreeksByMonthColl->Add(bsKey, NULL, &spGreeksByMonthAtom));
					__CHECK_HRESULT3(spGreeksByMonthAtom->put_ID(__USD_ID__));

					CComBSTR	bstrSymbol(m_bstrSymbol);
					bstrSymbol.Append(L"(");
					bstrSymbol.Append(__USD_SYMBOL__);
					bstrSymbol.Append(L"*)");
					
					__CHECK_HRESULT3(spGreeksByMonthAtom->put_Symbol(bstrSymbol));

					CComBSTR	bstrSeries;
					_FormatSeries(dtExpiry, bstrSeries);

					__CHECK_HRESULT3(spGreeksByMonthAtom->put_Series(bstrSeries));
					__CHECK_HRESULT3(spGreeksByMonthAtom->put_Expiry(dtExpiry));

					if (nRetCount)
					{
						if(_finite(aGreeks.dDelta))
						{
							DOUBLE dSynthOptionsDelta = aGreeks.dDelta * dOptPos * dCashValue;
							__CHECK_HRESULT3(spGreeksByMonthAtom->put_OptionsDelta(dSynthOptionsDelta * __USD_PRICE__));
							__CHECK_HRESULT3(spGreeksByMonthAtom->put_OptionsDeltaInShares(dSynthOptionsDelta));
						}

						if(_finite(aGreeks.dGamma))
						{
							DOUBLE dSynthTotalGamma = aGreeks.dGamma * dOptPos * dCashValue;
							__CHECK_HRESULT3(spGreeksByMonthAtom->put_TotalGamma(dSynthTotalGamma * __USD_PRICE__ * __USD_PRICE__ /100.));
							__CHECK_HRESULT3(spGreeksByMonthAtom->put_TotalGammaInShares(dSynthTotalGamma * __USD_PRICE__ /100.));
							__CHECK_HRESULT3(spGreeksByMonthAtom->put_TotalNetGamma(dSynthTotalGamma));
						}
					}
				}

				__CHECK_HRESULT(spSynthEnum->Reset(), _T("Fail to reset synthetic roots components collection."));
				while((hr = spSynthEnum->Next(1L, &varSynthItem, &nSynthFetched)) == S_OK)
				{
					ATLASSERT(varItem.vt == VT_DISPATCH);
					spSynthRootCompAtom = varSynthItem;

					__CHECK_HRESULT3(spSynthRootCompAtom->get_UndID(&nSynthUndID));
					__CHECK_HRESULT3(spSynthRootCompAtom->get_Weight(&dSynthWeight));

					if (nSynthUndID != m_nID)
					{
						spGreeksByMonthAtom = 0;

						bsKey = (_bstr_t)_variant_t(nSynthUndID) + L"_" + (_bstr_t)_variant_t(nOptID);
						__CHECK_HRESULT3(spGreeksByMonthColl->Add(bsKey, NULL, &spGreeksByMonthAtom));
						__CHECK_HRESULT3(spGreeksByMonthAtom->put_ID(nSynthUndID));

						IMmRpUndAtomPtr spUndAtom;
						__CHECK_HRESULT3(spUndColl->get_Item(_bstr_t(nSynthUndID), &spUndAtom));
						if (spUndAtom != NULL)
						{
							CComBSTR	bstrSymbol(m_bstrSymbol);
							CComBSTR	bstrSynthCompSymbol;
							CComBSTR	bstrSeries;

							__CHECK_HRESULT3(spUndAtom->get_Symbol(&bstrSynthCompSymbol));

							bstrSymbol.Append(L"(");
							bstrSymbol.Append(bstrSynthCompSymbol);
							bstrSymbol.Append(L"*)");

							__CHECK_HRESULT3(spGreeksByMonthAtom->put_Symbol(bstrSymbol));

							_FormatSeries(dtExpiry, bstrSeries);
							__CHECK_HRESULT3(spGreeksByMonthAtom->put_Series(bstrSeries));
							__CHECK_HRESULT3(spGreeksByMonthAtom->put_Expiry(dtExpiry));

							IMMRpPricePtr	spPrice ;
							spUndAtom->get_Price ( &spPrice ) ;
							ATLASSERT ( spPrice ) ;


							__CHECK_HRESULT3(spPrice->get_Bid(&dPriceBid));
							__CHECK_HRESULT3(spPrice->get_Ask(&dPriceAsk));
							__CHECK_HRESULT3(spPrice->get_Last(&dPriceLast));

							dPrice = m_spUndPriceProfile->GetUndPriceMid(dPriceBid, dPriceAsk, dPriceLast, dUndPriceTolerance, enPriceRoundingRule, NULL, VARIANT_FALSE );

							if (nRetCount)
							{
								if(_finite(aGreeks.dDelta))
								{
									DOUBLE dSynthOptionsDelta = aGreeks.dDelta * dOptPos * dSynthWeight;

									__CHECK_HRESULT3(spGreeksByMonthAtom->put_OptionsDelta(dSynthOptionsDelta * dPrice));
									__CHECK_HRESULT3(spGreeksByMonthAtom->put_OptionsDeltaInShares(dSynthOptionsDelta));
								}

								if(_finite(aGreeks.dGamma))
								{
									DOUBLE dSynthTotalGamma = aGreeks.dGamma * dOptPos * dSynthWeight;

									__CHECK_HRESULT3(spGreeksByMonthAtom->put_TotalGamma(dSynthTotalGamma * dPrice * dPrice /100.));
									__CHECK_HRESULT3(spGreeksByMonthAtom->put_TotalGammaInShares(dSynthTotalGamma * dPrice /100.));
									__CHECK_HRESULT3(spGreeksByMonthAtom->put_TotalNetGamma(dSynthTotalGamma));
								}
							}
						}
					}
					else
					{
						if (nRetCount)
						{
							if(_finite(aGreeks.dDelta))
							{
								dOptionsDelta = aGreeks.dDelta * dOptPos * dSynthWeight;
							}

							if(_finite(aGreeks.dGamma))
							{
								dTotalGamma = aGreeks.dGamma * dOptPos * dSynthWeight;
							}
						}
					}
				}
			}
			//endof synthetic
			if (nRetCount)
			{
				if(_finite(aGreeks.dVega))
				{
					dTotalVega = aGreeks.dVega * dOptPos;
					dTotalWeightedVega = dVegaWeight * aGreeks.dVega * dOptPos;
				}

				if(_finite(aGreeks.dTheta))
				{
					dTotalTheta = aGreeks.dTheta * dOptPos;
				}

				if(_finite(aGreeks.dRho))
				{
					dTotalRho = aGreeks.dRho * dOptPos;
				}
			}
			
			_bstr_t bsKey = (_bstr_t)_variant_t(m_nID) + L"_" + (_bstr_t)_variant_t(nOptID);

			IMmRpGreeksByMonthAtomPtr spGreeksByMonthAtom;
			__CHECK_HRESULT3(spGreeksByMonthColl->Add(bsKey, NULL, &spGreeksByMonthAtom));

			__CHECK_HRESULT3(spGreeksByMonthAtom->put_ID(m_nID));
			__CHECK_HRESULT3(spGreeksByMonthAtom->put_Expiry(dtExpiry));
			__CHECK_HRESULT3(spGreeksByMonthAtom->put_Symbol(m_bstrSymbol));

			CComBSTR bstrSeries;
			_FormatSeries(dtExpiry, bstrSeries);

			__CHECK_HRESULT3(spGreeksByMonthAtom->put_Series(bstrSeries));

			__CHECK_HRESULT3(spGreeksByMonthAtom->put_CallMarketValue(dCallMktValue));
			__CHECK_HRESULT3(spGreeksByMonthAtom->put_PutMarketValue(dPutMktValue));
			__CHECK_HRESULT3(spGreeksByMonthAtom->put_NetMarketValue(dCallMktValue + dPutMktValue));
			__CHECK_HRESULT3(spGreeksByMonthAtom->put_CallPoP(dCallPoP));
			__CHECK_HRESULT3(spGreeksByMonthAtom->put_PutPoP(dPutPoP));
			__CHECK_HRESULT3(spGreeksByMonthAtom->put_NetPoP(dCallPoP + dPutPoP));
			__CHECK_HRESULT3(spGreeksByMonthAtom->put_OptionsDelta(dOptionsDelta * dSpotPrice));
			__CHECK_HRESULT3(spGreeksByMonthAtom->put_OptionsDeltaInShares(dOptionsDelta));
			__CHECK_HRESULT3(spGreeksByMonthAtom->put_TotalGamma(dTotalGamma * dSpotPrice * dSpotPrice /100.));
			__CHECK_HRESULT3(spGreeksByMonthAtom->put_TotalGammaInShares(dTotalGamma * dSpotPrice /100.));
			__CHECK_HRESULT3(spGreeksByMonthAtom->put_TotalRho(dTotalRho));
			__CHECK_HRESULT3(spGreeksByMonthAtom->put_TotalVega(dTotalVega));
			__CHECK_HRESULT3(spGreeksByMonthAtom->put_TotalTheta(dTotalTheta));
			__CHECK_HRESULT3(spGreeksByMonthAtom->put_TotalWeightedVega(dTotalWeightedVega));
		}
		
		LONG nCount = 0L;
		spGreeksByMonthColl->get_Count(&nCount);

		*pRetVal = spGreeksByMonthColl.Detach();
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)CComErrorWrapper::ErrorDescription(e), IID_IMmRpUndAtom, e.Error());
	}

	return S_OK;
}

STDMETHODIMP CMmRpUndAtom::CalcPnLs(EtsCalcModelTypeEnum enCalcModel, 
					SAFEARRAY** psaRates, 
					SAFEARRAY** psaYTEs, 
					SAFEARRAY** psaHTBRates, 
					SAFEARRAY** psaHTBYTEs, 
					VARIANT_BOOL bUseTheoVolatility, 
					VARIANT_BOOL bUseTheoVolaNoBid, 
					VARIANT_BOOL bUseTheoVolaBadMarket, 
					IMmRpUndColl* pUndColl,
					DOUBLE dUndPriceTolerance, 
					EtsPriceRoundingRuleEnum enPriceRoundingRule,
					ICalculationParametrs* pParams,
					IMmRpPnLColl** pRetVal)
{
	if(!pUndColl || pParams == NULL)
		return E_INVALIDARG;

	__CHECK_POINTER(pRetVal);

	try
	{
		if (*pRetVal)
		{
			(*pRetVal)->Release();
			*pRetVal = 0;
		}

		HRESULT	hr;
		IMmRpUndCollPtr	spUndColl(pUndColl);
		IMmRpPnLCollPtr	spPnLColl;

		DOUBLE dUndBid , dUndAsk , dUndLast ;
		m_spPrice->get_Bid ( &dUndBid) ;
		m_spPrice->get_Ask ( &dUndAsk ) ;
		m_spPrice->get_Last( &dUndLast ) ;

		__CHECK_HRESULT(spPnLColl.CreateInstance(CLSID_MmRpPnLColl), _T("Failed to create MmRpPnLColl object."));

		DOUBLE	dPosDailyPrevDateBuy = 0.;
		DOUBLE	dPosDailyPrevDateSell = 0.;
		DOUBLE	dPosDailyTodayBuy = 0.;
		DOUBLE	dPosDailyTodaySell = 0.;
		DOUBLE	dQtyDailyPrevDateBuy = 0.;
		DOUBLE	dQtyDailyPrevDateSell = 0.;
		DOUBLE	dQtyDailyTodayBuy = 0.;
		DOUBLE	dQtyDailyTodaySell = 0.;

		DOUBLE	dQtyInSharesLTDBuy = 0.;
		DOUBLE	dQtyInSharesLTDSell = 0.;
		DOUBLE	dPosLTDBuy = 0.;
		DOUBLE	dPosLTDSell = 0.;

		__CHECK_HRESULT3(m_spPos->get_QtyDailyPrevDateBuy(&dQtyDailyPrevDateBuy));
		__CHECK_HRESULT3(m_spPos->get_QtyDailyPrevDateSell(&dQtyDailyPrevDateSell));
		__CHECK_HRESULT3(m_spPos->get_QtyDailyTodayBuy(&dQtyDailyTodayBuy));
		__CHECK_HRESULT3(m_spPos->get_QtyDailyTodaySell(&dQtyDailyTodaySell));
		__CHECK_HRESULT3(m_spPos->get_PosDailyPrevDateBuy(&dPosDailyPrevDateBuy));
		__CHECK_HRESULT3(m_spPos->get_PosDailyPrevDateSell(&dPosDailyPrevDateSell));
		__CHECK_HRESULT3(m_spPos->get_PosDailyTodayBuy(&dPosDailyTodayBuy));
		__CHECK_HRESULT3(m_spPos->get_PosDailyTodaySell(&dPosDailyTodaySell));
		
		__CHECK_HRESULT3(m_spPos->get_QtyInSharesLTDBuy(&dQtyInSharesLTDBuy));
		__CHECK_HRESULT3(m_spPos->get_QtyInSharesLTDSell(&dQtyInSharesLTDSell));
		__CHECK_HRESULT3(m_spPos->get_PosLTDBuy(&dPosLTDBuy));
		__CHECK_HRESULT3(m_spPos->get_PosLTDSell(&dPosLTDSell));

		DOUBLE dStockPos = dQtyDailyPrevDateBuy + dQtyDailyTodayBuy - dQtyDailyPrevDateSell - dQtyDailyTodaySell;

		/*
		DOUBLE dStockPriceBidForPnL = 
			m_spUndPriceProfile->GetUndPriceBidForPnL(dUndBid, dUndAsk, dUndLast, dUndPriceTolerance, enPriceRoundingRule, NULL);
		DOUBLE dStockPriceAskForPnL = 
			m_spUndPriceProfile->GetUndPriceAskForPnL(dUndBid, dUndAsk, dUndLast, dUndPriceTolerance, enPriceRoundingRule, NULL);
		DOUBLE dSpotPrice = 
			m_spUndPriceProfile->GetUndPriceMid ( dUndBid, dUndAsk, dUndLast, dUndPriceTolerance, enPriceRoundingRule, NULL );*/

		DOUBLE dStockPriceBidForPnL,dStockPriceAskForPnL, dSpotPrice ;
		VARIANT_BOOL	futureUsed = VARIANT_FALSE;
		__CHECK_HRESULT3(GetUnderlyingPrice(dUndPriceTolerance, enPriceRoundingRule, NULL, &futureUsed, &dSpotPrice));
		if (futureUsed) {
			dStockPriceBidForPnL = dStockPriceAskForPnL = dSpotPrice;
		} 
		else {
			dStockPriceBidForPnL = 
				m_spUndPriceProfile->GetUndPriceBidForPnL(dUndBid, dUndAsk, dUndLast, dUndPriceTolerance, enPriceRoundingRule, NULL);
			dStockPriceAskForPnL = 
				m_spUndPriceProfile->GetUndPriceAskForPnL(dUndBid, dUndAsk, dUndLast, dUndPriceTolerance, enPriceRoundingRule, NULL);
		}

		VARIANT_BOOL	bUseMidMarketForPnL = m_spUndPriceProfile->UseMidMarketForPnL;
		VARIANT_BOOL    bUseZeroBidRules    = m_spOptPriceProfile->UseZeroBidRule;

		DOUBLE			dMTMPrice = 0.;

// Futures 
		DOUBLE	dFutPos = 0.;
		DOUBLE	dFutPnLDaily = 0.;
		bool		badPnLValue  = false;
		DOUBLE	dFutMTMLTDPnL = 0;
		bool		badMTMLTDPnLValue = false;
		{ /***********************  collect futures position data *****************************/
			IUnknownPtr		spFutUnk;
			_variant_t		varItem;
			ULONG			nFetched = 0L;	
			IMmRpFutAtomPtr	spFutAtom;

			DOUBLE	dFutQtyDailyPrevDateBuy = 0.;
			DOUBLE	dFutQtyDailyPrevDateSell = 0.;
			DOUBLE	dFutQtyDailyTodayBuy = 0.;
			DOUBLE	dFutQtyDailyTodaySell = 0.;
			DOUBLE	dFutPosDailyPrevDateBuy = 0.;
			DOUBLE	dFutPosDailyPrevDateSell = 0.;
			DOUBLE	dFutPosDailyTodayBuy = 0.;
			DOUBLE	dFutPosDailyTodaySell = 0.;
			DOUBLE	dFutQtyInSharesLTDBuy = 0.;
			DOUBLE	dFutQtyInSharesLTDSell = 0.;
			DOUBLE	dFutPosLTDBuy = 0.;
			DOUBLE	dFutPosLTDSell = 0.;

			DOUBLE	futureBid = 0., futureAsk = 0., futureLast = 0.; 
			__CHECK_HRESULT(m_spFut->get__NewEnum(&spFutUnk), _T("Failed to get futures collection."));		
			IEnumVARIANTPtr spFutEnum(spFutUnk);		

			__CHECK_HRESULT(spFutEnum->Reset(), _T("Failed to reset futures collection."));
			while((hr = spFutEnum->Next(1L, &varItem, &nFetched)) == S_OK)
			{
				ATLASSERT(varItem.vt == VT_DISPATCH);
				spFutAtom = varItem;

				IMmRpPosAtomPtr	spFutPosAtom;
				__CHECK_HRESULT3(spFutAtom->get_Pos(&spFutPosAtom));

				__CHECK_HRESULT3(spFutPosAtom->get_QtyInSharesDailyPrevDateBuy(&dFutQtyDailyPrevDateBuy));
				__CHECK_HRESULT3(spFutPosAtom->get_QtyInSharesDailyPrevDateSell(&dFutQtyDailyPrevDateSell));
				__CHECK_HRESULT3(spFutPosAtom->get_QtyInSharesDailyTodayBuy(&dFutQtyDailyTodayBuy));
				__CHECK_HRESULT3(spFutPosAtom->get_QtyInSharesDailyTodaySell(&dFutQtyDailyTodaySell));

				__CHECK_HRESULT3(spFutPosAtom->get_PosDailyPrevDateBuy(&dFutPosDailyPrevDateBuy));
				__CHECK_HRESULT3(spFutPosAtom->get_PosDailyPrevDateSell(&dFutPosDailyPrevDateSell));
				__CHECK_HRESULT3(spFutPosAtom->get_PosDailyTodayBuy(&dFutPosDailyTodayBuy));
				__CHECK_HRESULT3(spFutPosAtom->get_PosDailyTodaySell(&dFutPosDailyTodaySell));

				__CHECK_HRESULT3(spFutPosAtom->get_QtyInSharesLTDBuy(&dFutQtyInSharesLTDBuy));
				__CHECK_HRESULT3(spFutPosAtom->get_QtyInSharesLTDSell(&dFutQtyInSharesLTDSell));
				__CHECK_HRESULT3(spFutPosAtom->get_PosLTDBuy(&dFutPosLTDBuy));
				__CHECK_HRESULT3(spFutPosAtom->get_PosLTDSell(&dFutPosLTDSell));

				//IMMRpPricePtr spPrice ;
				//spFutAtom->get_Price( &spPrice ) ;
				//ATLASSERT ( spPrice ) ;

				//__CHECK_HRESULT3(spPrice->get_Bid(&futureBid));
				//__CHECK_HRESULT3(spPrice->get_Ask(&futureAsk));
				//__CHECK_HRESULT3(spPrice->get_Last(&futureLast));

				//IEtsPriceProfileAtomPtr spFutPriceProfile = NULL;
				//__CHECK_HRESULT3( spFutAtom->get_UndPriceProfile(&spFutPriceProfile));
				//DOUBLE dFuturePriceBidForPnL = 
				//	m_spUndPriceProfile->GetUndPriceBidForPnL(futureBid, futureAsk, futureLast, dUndPriceTolerance, enPriceRoundingRule, NULL);
				//DOUBLE dFuturePriceAskForPnL = 
				//	m_spUndPriceProfile->GetUndPriceAskForPnL(futureBid, futureAsk, futureLast, dUndPriceTolerance, enPriceRoundingRule, NULL);
				//DOUBLE dFuturePrice = 
				//	m_spUndPriceProfile->GetUndPriceMid(futureBid, futureAsk, futureLast, dUndPriceTolerance, enPriceRoundingRule, NULL );

				DOUBLE dFuturePriceBidForPnL, dFuturePriceAskForPnL, dFuturePrice;
				//__CHECK_HRESULT3(GetUnderlyingPrice(dUndPriceTolerance, enPriceRoundingRule, NULL, &futureUsed, &dFuturePrice ));
				__CHECK_HRESULT3(spFutAtom->GetFuturePrice(dUndPriceTolerance, enPriceRoundingRule, NULL, &futureUsed, &dFuturePrice));
				if (futureUsed) {
					dFuturePriceBidForPnL = dFuturePriceAskForPnL = dFuturePrice;
				} 
				else {
					IEtsPriceProfileAtomPtr spFutPriceProfile = NULL;
					__CHECK_HRESULT3( spFutAtom->get_UndPriceProfile(&spFutPriceProfile));
					if ( spFutPriceProfile) {
						dFuturePriceBidForPnL = 
							m_spUndPriceProfile->GetUndPriceBidForPnL(futureBid, futureAsk, futureLast, dUndPriceTolerance, enPriceRoundingRule, NULL);
						dFuturePriceAskForPnL = 
							m_spUndPriceProfile->GetUndPriceAskForPnL(futureBid, futureAsk, futureLast, dUndPriceTolerance, enPriceRoundingRule, NULL);
					}
				}

				// calc daily PnL
				if (dFutQtyDailyPrevDateBuy > 0.0)
				{
					if (dFuturePriceBidForPnL > BAD_DOUBLE_VALUE && dFutPosDailyPrevDateBuy > BAD_DOUBLE_VALUE)
						dFutPnLDaily = dFuturePriceBidForPnL * dFutQtyDailyPrevDateBuy - dFutPosDailyPrevDateBuy;
					else
						badPnLValue = true;
				}

				if (!badPnLValue && dFutQtyDailyPrevDateSell > 0.0)
				{
					if (dFuturePriceAskForPnL > BAD_DOUBLE_VALUE && dFutPosDailyPrevDateSell > BAD_DOUBLE_VALUE)
						dFutPnLDaily -= dFuturePriceAskForPnL * dFutQtyDailyPrevDateSell - dFutPosDailyPrevDateSell;
					else
						badPnLValue = true;
				}

				if (!badPnLValue && dFutQtyDailyTodayBuy > 0.0)
				{
					if (dFuturePriceBidForPnL > BAD_DOUBLE_VALUE && dFutPosDailyTodayBuy > BAD_DOUBLE_VALUE)
						dFutPnLDaily += dFuturePriceBidForPnL * dFutQtyDailyTodayBuy - dFutPosDailyTodayBuy;
					else
						badPnLValue = true;
				}

				if (!badPnLValue && dFutQtyDailyTodaySell > 0.0)
				{
					if (dFuturePriceAskForPnL > BAD_DOUBLE_VALUE && dFutPosDailyTodaySell > BAD_DOUBLE_VALUE)
						dFutPnLDaily -= dFuturePriceAskForPnL * dFutQtyDailyTodaySell - dFutPosDailyTodaySell;
					else
						badPnLValue = true;
				}

				if (badPnLValue)
					dFutPnLDaily = 0.;

				// calc MTM PnL
				if (dFutQtyInSharesLTDBuy > 0.0)
				{
					if (dFuturePriceBidForPnL > BAD_DOUBLE_VALUE&& dFutPosLTDBuy)
						dFutMTMLTDPnL = dFuturePriceBidForPnL * dFutQtyInSharesLTDBuy - dFutPosLTDBuy;
					else
						badMTMLTDPnLValue = true;
				}

				if (!badMTMLTDPnLValue && dFutQtyInSharesLTDSell > 0.0)
				{
					if (dFuturePriceAskForPnL >BAD_DOUBLE_VALUE && dFutPosLTDSell)
						dFutMTMLTDPnL -= dFuturePriceAskForPnL * dFutQtyInSharesLTDSell - dFutPosLTDSell;
					else
						badMTMLTDPnLValue = true;
				}

				if (badMTMLTDPnLValue)
					dFutMTMLTDPnL = 0.;

				dFutPos += (dFutQtyDailyPrevDateBuy + dFutQtyDailyTodayBuy - dFutQtyDailyPrevDateSell - dFutQtyDailyTodaySell);		
			}

			/***********************************************************************************/}
			dStockPos += dFutPos;

		if (bUseMidMarketForPnL == VARIANT_TRUE && bUseZeroBidRules == VARIANT_FALSE)
		{
			dMTMPrice = dSpotPrice;
		}
		else
		{
			if (dStockPos > 0.)
			{
				dMTMPrice = dStockPriceBidForPnL;
			}
			else
			{
				dMTMPrice = dStockPriceAskForPnL;
			}
		}

// Stock PnL Daily
		bool bBadVal = false;
		DOUBLE dUndPnLDaily = badPnLValue ? 0. : dFutPnLDaily;

		if (dQtyDailyPrevDateBuy > 0.0)
		{
			if (dStockPriceBidForPnL > BAD_DOUBLE_VALUE && dPosDailyPrevDateBuy > BAD_DOUBLE_VALUE)
				dUndPnLDaily = dStockPriceBidForPnL * dQtyDailyPrevDateBuy - dPosDailyPrevDateBuy;
			else
				bBadVal = true;
		}

		if (!bBadVal && dQtyDailyPrevDateSell > 0.0)
		{
			if (dStockPriceAskForPnL > BAD_DOUBLE_VALUE && dPosDailyPrevDateSell > BAD_DOUBLE_VALUE)
				dUndPnLDaily -= dStockPriceAskForPnL * dQtyDailyPrevDateSell - dPosDailyPrevDateSell;
			else
				bBadVal = true;
		}

		if (!bBadVal && dQtyDailyTodayBuy > 0.0)
		{
			if (dStockPriceBidForPnL > BAD_DOUBLE_VALUE && dPosDailyTodayBuy > BAD_DOUBLE_VALUE)
				dUndPnLDaily += dStockPriceBidForPnL * dQtyDailyTodayBuy - dPosDailyTodayBuy;
			else
				bBadVal = true;
		}

		if (!bBadVal && dQtyDailyTodaySell > 0.0)
		{
			if (dStockPriceAskForPnL > BAD_DOUBLE_VALUE && dPosDailyTodaySell > BAD_DOUBLE_VALUE)
				dUndPnLDaily -= dStockPriceAskForPnL * dQtyDailyTodaySell - dPosDailyTodaySell;
			else
				bBadVal = true;
		}

		if (bBadVal)
			dUndPnLDaily = 0.;

// Stock PnL LTD
		DOUBLE dUndMTMLTDPnL = badMTMLTDPnLValue ? 0. : dFutMTMLTDPnL;
		bBadVal = false;

		if (dQtyInSharesLTDBuy > 0.0)
		{
			if (dStockPriceBidForPnL > BAD_DOUBLE_VALUE && dPosLTDBuy)
				dUndMTMLTDPnL = dStockPriceBidForPnL * dQtyInSharesLTDBuy - dPosLTDBuy;
			else
				bBadVal = true;
		}
		
		if (!bBadVal && dQtyInSharesLTDSell > 0.0)
		{
			if (dStockPriceAskForPnL > BAD_DOUBLE_VALUE && dPosLTDSell)
				dUndMTMLTDPnL -= dStockPriceAskForPnL * dQtyInSharesLTDSell - dPosLTDSell;
			else
				bBadVal = true;
		}

		if (bBadVal)
			dUndMTMLTDPnL = 0.;

		//DATE nToday = vt_date::GetCurrentDate(true);
		DATE	dtNow = 0.;
		GetNYDateTimeAsDATE(&dtNow);

		IUnknownPtr		spUnk;
		_variant_t		varItem;
		ULONG			nFetched = 0L;

		DOUBLE	dCallMktValue = 0.;
		DOUBLE	dPutMktValue = 0.;
		DOUBLE	dCallPoP = 0.;
		DOUBLE	dPutPoP = 0.;
		DOUBLE	dOptionsMTMPnLDaily = 0.;
		DOUBLE	dOptionsTheoPnLDaily = 0.;
		DOUBLE	dOptionsMTMLTDPnL = 0.;
		DOUBLE	dOptionsTheoLTDPnL = 0.;

		__CHECK_HRESULT(m_spOpt->get__NewEnum(&spUnk), _T("Failed to get options collection."));
		
		IEnumVARIANTPtr spOptEnum(spUnk);
		__CHECK_HRESULT(spOptEnum->Reset(), _T("Failed to reset options collection."));
		double savedSpotPrice = dSpotPrice;		//save stock spot for options 
		while((hr = spOptEnum->Next(1L, &varItem, &nFetched)) == S_OK)
		{
			dSpotPrice = savedSpotPrice;
			ATLASSERT(varItem.vt == VT_DISPATCH);
			IMmRpOptAtomPtr	spOptAtom = varItem;

			// if this is future option - get future price ///////////////////////////
			EtsContractTypeEnum optionContractType;
			__CHECK_HRESULT3(spOptAtom->get_ContractType(&optionContractType));
			if ( enCtFutOption == optionContractType ) {
				IMmRpFutAtomPtr spFutAtom;
				__CHECK_HRESULT3(spOptAtom->get_Fut(&spFutAtom));
				if (!spFutAtom) 
					EgLib::CComErrorWrapper::ThrowError( E_POINTER, _T("No future object attached to future option"));
				VARIANT_BOOL	dummy;
				__CHECK_HRESULT3(spFutAtom->GetFuturePrice(dUndPriceTolerance, enPriceRoundingRule, NULL, &dummy, &dSpotPrice));
			}
			//////////////////////////////////////////////////////////////////////////

			DOUBLE	dPriceBid = 0.;
			DOUBLE	dPriceAsk = 0.;
			DOUBLE	dPriceLast = 0.;
			DOUBLE	dPriceClose = 0.;
			DOUBLE	dPrice = 0.;
			DOUBLE	dStrike = 0.;
			DATE	dtExpiryOV = 0., tmCloseTime = 0.;
			LONG	nOptRootID = 0L;
			DOUBLE	dYTE = 0.;

			EtsOptionTypeEnum	enOptType = enOtPut;
			IMmRpPosAtomPtr		spOptPosAtom;

			GREEKS	aGreeks;
			memset(&aGreeks, 0, sizeof(aGreeks));

			dPosDailyPrevDateBuy = 0.;
			dPosDailyPrevDateSell = 0.;
			dPosDailyTodayBuy = 0.;
			dPosDailyTodaySell = 0.;
			dQtyDailyPrevDateBuy = 0.;
			dQtyDailyPrevDateSell = 0.;
			dQtyDailyTodayBuy = 0.;
			dQtyDailyTodaySell = 0.;

			dQtyInSharesLTDBuy = 0.;
			dQtyInSharesLTDSell = 0.;
			dPosLTDBuy = 0.;
			dPosLTDSell = 0.;

			__CHECK_HRESULT3(spOptAtom->get_Pos(&spOptPosAtom));

			__CHECK_HRESULT3(spOptPosAtom->get_PosDailyPrevDateBuy(&dPosDailyPrevDateBuy));
			__CHECK_HRESULT3(spOptPosAtom->get_PosDailyPrevDateSell(&dPosDailyPrevDateSell));
			__CHECK_HRESULT3(spOptPosAtom->get_PosDailyTodayBuy(&dPosDailyTodayBuy));
			__CHECK_HRESULT3(spOptPosAtom->get_PosDailyTodaySell(&dPosDailyTodaySell));
			__CHECK_HRESULT3(spOptPosAtom->get_QtyInSharesDailyPrevDateBuy(&dQtyDailyPrevDateBuy));
			__CHECK_HRESULT3(spOptPosAtom->get_QtyInSharesDailyPrevDateSell(&dQtyDailyPrevDateSell));
			__CHECK_HRESULT3(spOptPosAtom->get_QtyInSharesDailyTodayBuy(&dQtyDailyTodayBuy));
			__CHECK_HRESULT3(spOptPosAtom->get_QtyInSharesDailyTodaySell(&dQtyDailyTodaySell));

			__CHECK_HRESULT3(spOptPosAtom->get_QtyInSharesLTDBuy(&dQtyInSharesLTDBuy));
			__CHECK_HRESULT3(spOptPosAtom->get_QtyInSharesLTDSell(&dQtyInSharesLTDSell));
			__CHECK_HRESULT3(spOptPosAtom->get_PosLTDBuy(&dPosLTDBuy));
			__CHECK_HRESULT3(spOptPosAtom->get_PosLTDSell(&dPosLTDSell));

			__CHECK_HRESULT3(spOptAtom->get_RootID(&nOptRootID));
			__CHECK_HRESULT3(spOptAtom->get_Strike(&dStrike));
			__CHECK_HRESULT3(spOptAtom->get_ExpiryOV(&dtExpiryOV));
			__CHECK_HRESULT3(spOptAtom->get_TradingClose(&tmCloseTime));
			__CHECK_HRESULT3(spOptAtom->get_OptType(&enOptType));

			IMMRpPricePtr spPrice ;
			spOptAtom->get_Price( &spPrice ) ;
			ATLASSERT ( spPrice ) ;

			__CHECK_HRESULT3(spPrice->get_Bid(&dPriceBid));
			__CHECK_HRESULT3(spPrice->get_Ask(&dPriceAsk));
			__CHECK_HRESULT3(spPrice->get_Last(&dPriceLast));
			__CHECK_HRESULT3(spPrice->get_Close(&dPriceClose));

#ifdef _DEBUG
			CComBSTR bsSym;
			__CHECK_HRESULT3(spOptAtom->get_Symbol(&bsSym));
#endif
			dStrike = _AdjustFractionalDigits(dStrike, 4);

			bool				bIsRootSynthetic = false;
			ISynthRootAtomPtr	spSynthRootAtom;
			
			if (m_vbHasSynthetic == VARIANT_TRUE)
			{
				spSynthRootAtom = m_spSyntheticRoots->Item[nOptRootID];
				if (spSynthRootAtom != NULL) //is synthetic
					bIsRootSynthetic = true;
			}
			
			::GetCalculationParams(dtNow, dtExpiryOV, tmCloseTime, pParams->UseTimePrecision != VARIANT_FALSE, &dtNow, &dtExpiryOV, &tmCloseTime, &dYTE);

			LONG nRetCount = 0L;			
			if (dYTE <= dtExpiryOV)
			{
				memset(&aGreeks, 0, sizeof(GREEKS));
				aGreeks.nMask = GT_THEOPRICE;

				nRetCount = _CalcOptionGreeks(	enCalcModel, 
												spOptAtom, 
												_InterpolateRate(dYTE, *psaRates, *psaYTEs, ICalculationParametrsPtr(pParams)), 
												_InterpolateRate(dYTE, *psaHTBRates, *psaHTBYTEs, ICalculationParametrsPtr(pParams)),
												bUseTheoVolatility, 
												bUseTheoVolaNoBid, 
												bUseTheoVolaBadMarket, 
												pUndColl, 
												dUndPriceTolerance,
												enPriceRoundingRule, 
												aGreeks,
												ICalculationParametrsPtr(pParams));
			}
            
			DOUBLE dOptPriceMid = m_spOptPriceProfile->GetOptPriceMid(dPriceBid, dPriceAsk, dPriceLast, enPriceRoundingRule, bUseTheoVolatility, aGreeks.dTheoPrice, NULL);
			DOUBLE dOptBidForPnL = m_spOptPriceProfile->GetOptPriceBidForPnL(dPriceBid, dPriceAsk, dPriceLast, enPriceRoundingRule, bUseTheoVolatility, aGreeks.dTheoPrice, NULL);
			DOUBLE dOptAskForPnL = m_spOptPriceProfile->GetOptPriceAskForPnL(dPriceBid, dPriceAsk, dPriceLast, enPriceRoundingRule, bUseTheoVolatility, aGreeks.dTheoPrice, NULL);

			DOUBLE dOptPos = dQtyDailyPrevDateBuy + dQtyDailyTodayBuy - dQtyDailyPrevDateSell - dQtyDailyTodaySell;
			VARIANT_BOOL bUseMidMarketForPnL = m_spOptPriceProfile->UseMidMarketForPnL;
			bool bEbableZeroPrice = false;


// Calculate MarketValue
			if (bUseMidMarketForPnL == VARIANT_TRUE && bUseZeroBidRules == VARIANT_FALSE)
			{
				dMTMPrice = dOptPriceMid;
			}
			else
			{
				if(dOptPos > 0.)
				{
					dMTMPrice = dOptBidForPnL;
					bEbableZeroPrice = (enObsprReplaceWithZero ==m_spOptPriceProfile->BadOptSinglePriceRule);

				}
				else
				{
					dMTMPrice = dOptAskForPnL;
					bEbableZeroPrice = (enObsprReplaceWithZero ==m_spOptPriceProfile->BadOptSinglePriceRule);
				}
			}

			if (enOptType == enOtCall)
			{
				dCallMktValue += dMTMPrice * dOptPos;
			}
			else //put
			{
				dPutMktValue += dMTMPrice * dOptPos;
			}

// Calculate PoP
			if (dtNow <= dtExpiryOV)
			{
				if (bIsRootSynthetic)
				{
					_GetSyntheticPrice(spSynthRootAtom, spUndColl, dPriceBid, dPriceAsk, dPriceLast);					
					dPrice = m_spUndPriceProfile->GetUndPriceMid(dPriceBid, dPriceAsk, dPriceLast, dUndPriceTolerance, enPriceRoundingRule, NULL, VARIANT_FALSE );
				}
				else
				{
					dPrice = dSpotPrice;
				}

				if (m_vbIsAmerican != VARIANT_TRUE)
				{	
					DOUBLE	dYield = 0.;

					if (bIsRootSynthetic)
					{
						dYield = spSynthRootAtom->Yield;
						dPrice = _CalcSyntheticForwardPrice(spSynthRootAtom,
															dPrice, 
															dtExpiryOV, 
															tmCloseTime, 
															dtNow, 
															dYield, 
															*psaRates, 
															*psaYTEs,
															ICalculationParametrsPtr(pParams));
					}
					else
					{
						dYield = m_dYield;
						dPrice = _CalcRegularForwardPrice(	dPrice,
															dtExpiryOV,
															tmCloseTime, 
															dtNow, 
															dYield, 
															*psaRates, 
															*psaYTEs,
															ICalculationParametrsPtr(pParams));
					}
				}

				if (dPrice > 0. && (dMTMPrice > 0. || bEbableZeroPrice))
				{	
					DOUBLE dTimeValue = dMTMPrice;
					if (enOptType == enOtCall)
					{	
						if (dPrice - dStrike > 0.)
						{
							dTimeValue -= dPrice - dStrike;
						}

						dCallPoP += dOptPos * max(0., dTimeValue);
					}
					else
					{
						if (dStrike - dPrice > 0.)
						{
							dTimeValue -= dStrike - dPrice;
						}

						dPutPoP += dOptPos * max(0., dTimeValue);
					}
				}
			}

// Options PnL MTM Daily
			DOUBLE dVal = 0.;
			bBadVal = false;
			if (dQtyDailyPrevDateBuy > 0.0)
			{
				if ((dOptBidForPnL > BAD_DOUBLE_VALUE  || bEbableZeroPrice) && dPosDailyPrevDateBuy > BAD_DOUBLE_VALUE)
					dVal += dOptBidForPnL * dQtyDailyPrevDateBuy - dPosDailyPrevDateBuy;
				else
					bBadVal = true;
			}

			if (!bBadVal && dQtyDailyPrevDateSell > 0.0)
			{
				if ((dOptAskForPnL > BAD_DOUBLE_VALUE || bEbableZeroPrice) && dPosDailyPrevDateSell > BAD_DOUBLE_VALUE)
					dVal -= dOptAskForPnL * dQtyDailyPrevDateSell - dPosDailyPrevDateSell;
				else
					bBadVal = true;
			}

			if (!bBadVal && dQtyDailyTodayBuy > 0.0)
			{
				if ((dOptBidForPnL > BAD_DOUBLE_VALUE  || bEbableZeroPrice) && dPosDailyTodayBuy > BAD_DOUBLE_VALUE)
					dVal += dOptBidForPnL * dQtyDailyTodayBuy - dPosDailyTodayBuy;
				else
					bBadVal = true;
			}

			if (!bBadVal && dQtyDailyTodaySell > 0.0)
			{
				if ((dOptAskForPnL > BAD_DOUBLE_VALUE || bEbableZeroPrice) && dPosDailyTodaySell > BAD_DOUBLE_VALUE)
					dVal -= dOptAskForPnL * dQtyDailyTodaySell - dPosDailyTodaySell;
				else
					bBadVal = true;
			}

			if (!bBadVal)
				dOptionsMTMPnLDaily += dVal;

// Options PnL MTM LTD
			dVal = 0.;
			bBadVal = false;
			if (dQtyInSharesLTDBuy > 0.0)
			{
				if ((dOptBidForPnL > 0.0 || bEbableZeroPrice) && dPosLTDBuy > BAD_DOUBLE_VALUE)
					dVal += dOptBidForPnL * dQtyInSharesLTDBuy - dPosLTDBuy;
				else
					bBadVal = true;
			}

			if (!bBadVal && dQtyInSharesLTDSell > 0.0)
			{
				if ((dOptAskForPnL > 0.0 || bEbableZeroPrice) && dPosLTDSell > BAD_DOUBLE_VALUE)
					dVal -= dOptAskForPnL * dQtyInSharesLTDSell - dPosLTDSell;
				else
					bBadVal = true;
			}

			if (!bBadVal)
				dOptionsMTMLTDPnL += dVal;


			if ( _finite(aGreeks.dTheoPrice) /*&& !DoubleEQZero(aGreeks.dTheoPrice)*/ )
			{	
// Options PnL Theo Daily
				dVal = 0.;
				bBadVal = false;
				if (dQtyDailyPrevDateBuy > 0.0)
				{
					if (dPosDailyPrevDateBuy > BAD_DOUBLE_VALUE)
						dVal += aGreeks.dTheoPrice * dQtyDailyPrevDateBuy - dPosDailyPrevDateBuy;
					else
						bBadVal = true;
				}

				if (!bBadVal && dQtyDailyPrevDateSell > 0.0)
				{
					if (dPosDailyPrevDateSell > BAD_DOUBLE_VALUE)
						dVal -= aGreeks.dTheoPrice * dQtyDailyPrevDateSell - dPosDailyPrevDateSell;
					else
						bBadVal = true;
				}

				if (!bBadVal && dQtyDailyTodayBuy > 0.0)
				{
					if (dPosDailyTodayBuy > BAD_DOUBLE_VALUE)
						dVal += aGreeks.dTheoPrice * dQtyDailyTodayBuy - dPosDailyTodayBuy;
					else
						bBadVal = true;
				}

				if (!bBadVal && dQtyDailyTodaySell > 0.0)
				{
					if (dPosDailyTodaySell > BAD_DOUBLE_VALUE)
						dVal -= aGreeks.dTheoPrice * dQtyDailyTodaySell - dPosDailyTodaySell;
					else
						bBadVal = true;
				}

				if (!bBadVal)
					dOptionsTheoPnLDaily += dVal;

// Options PnL Theo LTD
				dVal = 0.;
				bBadVal = false;
				if (dQtyInSharesLTDBuy > 0.0)
				{
					if (dPosLTDBuy > BAD_DOUBLE_VALUE)
						dVal += aGreeks.dTheoPrice * dQtyInSharesLTDBuy - dPosLTDBuy;
					else
						bBadVal = true;
				}

				if (!bBadVal && dQtyInSharesLTDSell > 0.0)
				{
					if (dPosLTDSell > BAD_DOUBLE_VALUE)
						dVal -= aGreeks.dTheoPrice * dQtyInSharesLTDSell - dPosLTDSell;
					else
						bBadVal = true;
				}				

				if (!bBadVal)
					dOptionsTheoLTDPnL += dVal;
			}
		}

		IMmRpPnLAtomPtr spPnLAtom;
		__CHECK_HRESULT3(spPnLColl->Add(m_nID, NULL, &spPnLAtom));

		__CHECK_HRESULT3(spPnLAtom->put_ID(m_nID));
		__CHECK_HRESULT3(spPnLAtom->put_UndType(m_enUndType));
		__CHECK_HRESULT3(spPnLAtom->put_Symbol(m_bstrSymbol));
		__CHECK_HRESULT3(spPnLAtom->put_CallMarketValue(dCallMktValue));
		__CHECK_HRESULT3(spPnLAtom->put_PutMarketValue(dPutMktValue));
		__CHECK_HRESULT3(spPnLAtom->put_CallPoP(dCallPoP));
		__CHECK_HRESULT3(spPnLAtom->put_PutPoP(dPutPoP));
		__CHECK_HRESULT3(spPnLAtom->put_NetMarketValue(dCallMktValue + dPutMktValue));
		__CHECK_HRESULT3(spPnLAtom->put_NetPoP(dCallPoP + dPutPoP));
		__CHECK_HRESULT3(spPnLAtom->put_OptionsMTMLTDPnL(dOptionsMTMLTDPnL));
		__CHECK_HRESULT3(spPnLAtom->put_OptionsMTMPnLDaily(dOptionsMTMPnLDaily));
		__CHECK_HRESULT3(spPnLAtom->put_OptionsTheoLTDPnL(dOptionsTheoLTDPnL));
		__CHECK_HRESULT3(spPnLAtom->put_OptionsTheoPnLDaily(dOptionsTheoPnLDaily));
		__CHECK_HRESULT3(spPnLAtom->put_UndMTMLTDPnL(dUndMTMLTDPnL));
		__CHECK_HRESULT3(spPnLAtom->put_UndMTMPnLDaily(dUndPnLDaily));

		*pRetVal = spPnLColl.Detach();
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)CComErrorWrapper::ErrorDescription(e), IID_IMmRpUndAtom, e.Error());
	}

	return S_OK;
}

STDMETHODIMP CMmRpUndAtom::CalcExercisedStocks(IMmRpUndColl* pUndColl, 
					DOUBLE dUndPriceTolerance, EtsPriceRoundingRuleEnum enPriceRoundingRule,
					ICalculationParametrs* pParams,
					IMmRpExercisedStockColl** pRetVal)
{
 	if(!pUndColl)
		return E_INVALIDARG;

	__CHECK_POINTER(pRetVal);

	try
	{
		if(*pRetVal)
		{
			(*pRetVal)->Release();
			*pRetVal = 0;
		}

		DOUBLE dUndBid , dUndAsk , dUndLast ;
		m_spPrice->get_Bid ( &dUndBid) ;
		m_spPrice->get_Ask ( &dUndAsk ) ;
		m_spPrice->get_Last( &dUndLast ) ;

		HRESULT	hr;
		IMmRpUndCollPtr				spUndColl(pUndColl);
		IMmRpExercisedStockCollPtr	spExercisedStockColl;

		__CHECK_HRESULT(spExercisedStockColl.CreateInstance(CLSID_MmRpExercisedStockColl), 
				_T("Failed to create MmRpExercisedStockColl object."));

		DOUBLE	dSpotPrice = m_spUndPriceProfile->GetUndPriceMid(dUndBid, dUndAsk, dUndLast, dUndPriceTolerance, enPriceRoundingRule, NULL, VARIANT_FALSE );
		
		LONG					nOptID = 0L;
		LONG					nOptRootID = 0L;
		DATE					dtExpiry = 0.;
		DOUBLE					dStrike = 0.;
		EtsOptionTypeEnum		enOptType;

		DOUBLE					dQtyDailyPrevDateBuy = 0.;
		DOUBLE					dQtyDailyPrevDateSell = 0.;
		DOUBLE					dQtyDailyTodayBuy = 0.;
		DOUBLE					dQtyDailyTodaySell = 0.;

		DOUBLE					dSpotBid = 0.;
		DOUBLE					dSpotAsk = 0.;
		DOUBLE					dSpotLast = 0.;

		DATE					nToday = (DATE)COleDateTime::GetCurrentTime();		

		IUnknownPtr				spOptUnk;		
		_variant_t				varOptItem;
		ULONG					nOptFetched = 0L;
		IMmRpOptAtomPtr			spOptAtom;

		__CHECK_HRESULT(m_spOpt->get__NewEnum(&spOptUnk), _T("Failed to get options collection."));		
		
		IEnumVARIANTPtr	spOptEnum(spOptUnk);
		__CHECK_HRESULT(spOptEnum->Reset(), _T("Fail to reset options collection."));
		while((hr = spOptEnum->Next(1L, &varOptItem, &nOptFetched)) == S_OK)
		{
			ATLASSERT(varOptItem.vt == VT_DISPATCH);
			spOptAtom = varOptItem;

			__CHECK_HRESULT3(spOptAtom->get_Expiry(&dtExpiry));
			if (dtExpiry < nToday)
				continue;
			
			IMmRpPosAtomPtr spOptPosAtom;
			__CHECK_HRESULT3(spOptAtom->get_Pos(&spOptPosAtom));

			__CHECK_HRESULT3(spOptPosAtom->get_QtyInSharesDailyPrevDateBuy(&dQtyDailyPrevDateBuy));
			__CHECK_HRESULT3(spOptPosAtom->get_QtyInSharesDailyPrevDateSell(&dQtyDailyPrevDateSell));
			__CHECK_HRESULT3(spOptPosAtom->get_QtyInSharesDailyTodayBuy(&dQtyDailyTodayBuy));
			__CHECK_HRESULT3(spOptPosAtom->get_QtyInSharesDailyTodaySell(&dQtyDailyTodaySell));

			DOUBLE dOptPos = dQtyDailyPrevDateBuy + dQtyDailyTodayBuy - dQtyDailyPrevDateSell - dQtyDailyTodaySell;
			if (DoubleEQZero(dOptPos))
				continue;
			
			CComBSTR bstrOptSymbol;
			__CHECK_HRESULT3(spOptAtom->get_Symbol(&bstrOptSymbol));

			__CHECK_HRESULT3(spOptAtom->get_OptType(&enOptType));
			__CHECK_HRESULT3(spOptAtom->get_Strike(&dStrike));
			__CHECK_HRESULT3(spOptAtom->get_ID(&nOptID));
			__CHECK_HRESULT3(spOptAtom->get_RootID(&nOptRootID));			

			dStrike = _AdjustFractionalDigits(dStrike, 4);
			dOptPos = dOptPos * (enOptType == enOtCall ? 1. : -1.);

			CComBSTR bstrSeries;
			_FormatSeries(dtExpiry, bstrSeries);

			bool					bIsRootSynthetic = false;
			ISynthRootAtomPtr		spSynthRootAtom;
			ISynthRootCompCollPtr	spSynthRootCompColl;
			ISynthRootCompAtomPtr	spSynthRootCompAtom;
				
			IUnknownPtr				spSynthUnk;
			_variant_t				varSynthItem;
			ULONG					nSynthFetched = 0L;
			IEnumVARIANTPtr			spSynthEnum;
				
			LONG					nSynthUndID = 0L;
			DOUBLE					dSynthWeight = 0L;
			DOUBLE					dCashValue = 0.;
			DOUBLE					dSynthPrice = 0.;
			
			if (m_vbHasSynthetic == VARIANT_TRUE)
			{
				spSynthRootAtom = m_spSyntheticRoots->Item[nOptRootID];
				if (spSynthRootAtom != NULL) //is synthetic
				{
					__CHECK_HRESULT3(spSynthRootAtom->get_CashValue(&dCashValue));
					
					_GetSyntheticPrice(spSynthRootAtom, pUndColl, dSpotBid, dSpotAsk, dSpotLast);
					dSynthPrice = m_spUndPriceProfile->GetUndPriceMid(dSpotBid, dSpotAsk, dSpotLast, dUndPriceTolerance, enPriceRoundingRule, NULL, VARIANT_FALSE );

					bIsRootSynthetic = true;
					spSynthRootCompColl = spSynthRootAtom->SynthRootComponents;
					ATLASSERT(spSynthRootCompColl!=NULL);

					spSynthUnk = spSynthRootCompColl->_NewEnum;
					spSynthEnum = spSynthUnk;
				}
			}

			DOUBLE dPrice = bIsRootSynthetic ? dSynthPrice : dSpotPrice;

			if (dPrice < 0. || DoubleEQZero(dPrice))
				continue;

			bool bIsITM = (enOptType == enOtCall) ? (dPrice - dStrike > 0.) : (dStrike - dPrice > 0.);
			if (!bIsITM)
			{
				dOptPos = 0.;
			}

			if (!bIsRootSynthetic)
			{
				_bstr_t bsKey = (_bstr_t)_variant_t(m_nID) + L"_" + (_bstr_t)_variant_t(nOptID);

				IMmRpExercisedStockAtomPtr	spExercisedStockAtom;
				__CHECK_HRESULT3(spExercisedStockColl->Add(bsKey, NULL, &spExercisedStockAtom));

				__CHECK_HRESULT3(spExercisedStockAtom->put_ID(m_nID));
				__CHECK_HRESULT3(spExercisedStockAtom->put_Symbol(m_bstrSymbol));
				__CHECK_HRESULT3(spExercisedStockAtom->put_UndType(m_enUndType));
				__CHECK_HRESULT3(spExercisedStockAtom->put_Expiry(dtExpiry));
                __CHECK_HRESULT3(spExercisedStockAtom->put_Series(bstrSeries));
				__CHECK_HRESULT3(spExercisedStockAtom->put_Pos(dOptPos));

				if (m_nPrev5ExpAvgVol > 0.)
				{
					__CHECK_HRESULT3(spExercisedStockAtom->put_Prev5ExpAvgVol(m_nPrev5ExpAvgVol));
				}

				if (m_nPrev10TrdAvgVol > 0.)
				{
					__CHECK_HRESULT3(spExercisedStockAtom->put_Prev10TrdAvgVol(m_nPrev10TrdAvgVol));
				}
			}
			else // synthetic
			{	
				IMmRpExercisedStockAtomPtr	spExercisedStockAtom;
				_bstr_t bsKey;
				
				if (dCashValue > 0.)
				{
					bsKey = (_bstr_t)_variant_t(nOptRootID) + L"_" + (_bstr_t)_variant_t(nOptID) + L"_" + _bstr_t(__USD_SYMBOL__);

					__CHECK_HRESULT3(spExercisedStockColl->Add(bsKey, NULL, &spExercisedStockAtom));
					__CHECK_HRESULT3(spExercisedStockAtom->put_ID(__USD_ID__));

					CComBSTR bstrSymbol(m_bstrSymbol);					
					bstrSymbol.Append(_T("("));
					bstrSymbol.Append(__USD_SYMBOL__);
					bstrSymbol.Append(_T("*)"));

					__CHECK_HRESULT3(spExercisedStockAtom->put_Symbol(bstrSymbol));
					__CHECK_HRESULT3(spExercisedStockAtom->put_Expiry(dtExpiry));
					__CHECK_HRESULT3(spExercisedStockAtom->put_Series(bstrSeries));
					__CHECK_HRESULT3(spExercisedStockAtom->put_Pos(dOptPos * dCashValue));
				}
				
				__CHECK_HRESULT(spSynthEnum->Reset(), _T("Fail to reset synthetic roots components collection."));
				while((hr = spSynthEnum->Next(1L, &varSynthItem, &nSynthFetched)) == S_OK)
				{
					ATLASSERT(varSynthItem.vt == VT_DISPATCH);
					spSynthRootCompAtom = varSynthItem;

					__CHECK_HRESULT3(spSynthRootCompAtom->get_UndID(&nSynthUndID));
					__CHECK_HRESULT3(spSynthRootCompAtom->get_Weight(&dSynthWeight));

					bsKey = (_bstr_t)_variant_t(nSynthUndID) + L"_" + (_bstr_t)_variant_t(nOptID);

					spExercisedStockAtom = 0;
					__CHECK_HRESULT3(spExercisedStockColl->Add(bsKey, NULL, &spExercisedStockAtom));
					__CHECK_HRESULT3(spExercisedStockAtom->put_ID(nSynthUndID));

					if (m_nID != nSynthUndID)
					{
						IMmRpUndAtomPtr spUndAtom;

						__CHECK_HRESULT3(spUndColl->get_Item(_bstr_t(nSynthUndID), &spUndAtom));							
						if (spUndAtom != 0)
						{
							CComBSTR				bstrSymbol(m_bstrSymbol);
							CComBSTR				bstrSynthSymbol;							
							EtsContractTypeEnum		enSynthUndType;

							__CHECK_HRESULT3(spUndAtom->get_Symbol(&bstrSynthSymbol));
							__CHECK_HRESULT3(spUndAtom->get_UndType(&enSynthUndType));

							bstrSymbol.Append(_T("("));
							bstrSymbol.Append(bstrSynthSymbol);
							bstrSymbol.Append(_T("*)"));

							__CHECK_HRESULT3(spExercisedStockAtom->put_Symbol(bstrSymbol));
							__CHECK_HRESULT3(spExercisedStockAtom->put_UndType(enSynthUndType));
						}
					}
					else
					{
						__CHECK_HRESULT3(spExercisedStockAtom->put_Symbol(m_bstrSymbol));

						if (m_nPrev5ExpAvgVol > 0.)
						{
							__CHECK_HRESULT3(spExercisedStockAtom->put_Prev5ExpAvgVol(m_nPrev5ExpAvgVol));
						}

						if (m_nPrev10TrdAvgVol > 0.)
						{
							__CHECK_HRESULT3(spExercisedStockAtom->put_Prev10TrdAvgVol(m_nPrev10TrdAvgVol));
						}

						__CHECK_HRESULT3(spExercisedStockAtom->put_UndType(m_enUndType));
					}
						
					__CHECK_HRESULT3(spExercisedStockAtom->put_Expiry(dtExpiry));
					__CHECK_HRESULT3(spExercisedStockAtom->put_Series(bstrSeries));
					__CHECK_HRESULT3(spExercisedStockAtom->put_Pos(dOptPos * dSynthWeight));
				}                    
			}
		}

		*pRetVal = spExercisedStockColl.Detach();
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)CComErrorWrapper::ErrorDescription(e), IID_IMmRpUndAtom, e.Error());
	}

	return S_OK;
}



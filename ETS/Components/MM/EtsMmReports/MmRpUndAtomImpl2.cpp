// MmRpUndAtomImpl2.cpp : Implementation of CMmRpUndAtom

#include "stdafx.h"
#include "MmRpUndAtom.h"
#include "MmRpRiskMatrixColl.h"
#include "MmRpRiskMatrixAtom.h"
#include "MmRpRiskMatrixShiftAtom.h"
#include "MmRpPosWithEarlyExerciseColl.h"
#include "MmRpPosWithEarlyExerciseAtom.h"
#include "MmRpPosWithEarlyExerciseOptColl.h"
#include "MmRpPosWithEarlyExerciseOptAtom.h"
#include "MmRpMatrixByStockAtom.h"
#include "MmRpMatrixByStockColl.h"
#include "MmRpOptAtom.h"
#include "MmRpMatrixByStockOptAtom.h"
#include "MmRpSyntheticColl.h"
#include "MmRpSyntheticAtom.h"
#include "MmRpSyntheticOptAtom.h"
#include "MmRpSyntheticOptColl.h"

// CMmRpUndAtom

STDMETHODIMP CMmRpUndAtom::CalcRiskMatrix(EtsCalcModelTypeEnum enCalcModel, 
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
					IMmRpRiskMatrixColl** pRetVal)
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

		DOUBLE dUndBid , dUndAsk , dUndLast ;
		m_spPrice->get_Bid ( &dUndBid) ;
		m_spPrice->get_Ask ( &dUndAsk ) ;
		m_spPrice->get_Last( &dUndLast ) ;

		HRESULT	hr;
		IMmRpUndCollPtr			spUndColl(pUndColl);
		IMmRpRiskMatrixCollPtr	spRiskMatrixColl;

		__CHECK_HRESULT(spRiskMatrixColl.CreateInstance(CLSID_MmRpRiskMatrixColl), _T("Failed to create MmRpRiskMatrixColl object."));

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
		
		DOUBLE	dFutPos = 0.;
		{ /***********************  collect futures position data *****************************/
			IUnknownPtr		spFutUnk;
			_variant_t		varItem;
			ULONG			nFetched = 0L;	
			IMmRpFutAtomPtr	spFutAtom;

			DOUBLE	dFutQtyDailyPrevDateBuy = 0.;
			DOUBLE	dFutQtyDailyPrevDateSell = 0.;
			DOUBLE	dFutQtyDailyTodayBuy = 0.;
			DOUBLE	dFutQtyDailyTodaySell = 0.;
			

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
				dFutPos += (dFutQtyDailyPrevDateBuy + dFutQtyDailyTodayBuy - dFutQtyDailyPrevDateSell - dFutQtyDailyTodaySell);		
			}
		/***********************************************************************************/}
		
		DOUBLE	dStockPos = dFutPos + dQtyDailyPrevDateBuy + dQtyDailyTodayBuy - dQtyDailyPrevDateSell - dQtyDailyTodaySell;		
		DOUBLE	dSpotPrice;
		VARIANT_BOOL futureUsed = VARIANT_FALSE;
		__CHECK_HRESULT3(GetUnderlyingPrice(dUndPriceTolerance, enPriceRoundingRule, NULL, &futureUsed, &dSpotPrice) );

		DOUBLE	dHV30d = m_dHV * 100.;
		DOUBLE	dSTD1 = dHV30d / sqrt(252.);

		IMmRpRiskMatrixAtomPtr	spRiskMatrixAtom;
		__CHECK_HRESULT3(spRiskMatrixColl->Add(m_nID, m_bstrSymbol, NULL, &spRiskMatrixAtom));

		__CHECK_HRESULT3(spRiskMatrixAtom->put_ID(m_nID));
		__CHECK_HRESULT3(spRiskMatrixAtom->put_Symbol(m_bstrSymbol));
		__CHECK_HRESULT3(spRiskMatrixAtom->put_Price(dSpotPrice));
		__CHECK_HRESULT3(spRiskMatrixAtom->put_STD(dSTD1));
		__CHECK_HRESULT3(spRiskMatrixAtom->put_HV(dHV30d));

		DOUBLE	dStrike = 0.;
		LONG	nOptID = 0L;
		LONG	nOptRootID = 0L;
		DATE	dtExpiryOV = 0., tmCloseTime = 0., dYTE = 0.;

		EtsOptionTypeEnum	enOptType = enOtPut;

		DOUBLE	dPriceBid = 0.;
		DOUBLE	dPriceAsk = 0.;
		DOUBLE	dPriceLast = 0.;
		DOUBLE	dPrice = 0.;

		DATE	dtNow;
		GetNYDateTimeAsDATE(&dtNow);

		IUnknownPtr		spOptUnk;
		_variant_t		varItem;
		ULONG			nFetched = 0L;	
		IMmRpOptAtomPtr	spOptAtom;

		__CHECK_HRESULT(m_spOpt->get__NewEnum(&spOptUnk), _T("Failed to get options collection."));		
		IEnumVARIANTPtr spOptEnum(spOptUnk);		

		for (LONG nShift = -20; nShift <= 20; nShift += 2)
		{
			DOUBLE dShift = nShift;
			DOUBLE dStockPrice = dSpotPrice * (1. + dShift / 100.);
			DOUBLE dMarketValue = dStockPos * dStockPrice;

			DOUBLE dOptionsTheoPnLDaily = 0.;
            DOUBLE dOptionsDelta = 0.;
            DOUBLE dTotalGamma = 0.;
			DOUBLE dNetGamma = 0.;
            DOUBLE dTotalVega = 0.;
            DOUBLE dTotalTheta = 0.;
            DOUBLE dPoP = 0.;

			__CHECK_HRESULT(spOptEnum->Reset(), _T("Failed to reset options collection."));
			while((hr = spOptEnum->Next(1L, &varItem, &nFetched)) == S_OK)
			{
				ATLASSERT(varItem.vt == VT_DISPATCH);
				spOptAtom = varItem;

				IMmRpPosAtomPtr	spOptPosAtom;
				__CHECK_HRESULT3(spOptAtom->get_Pos(&spOptPosAtom));

				__CHECK_HRESULT3(spOptPosAtom->get_QtyInSharesDailyPrevDateBuy(&dQtyDailyPrevDateBuy));
				__CHECK_HRESULT3(spOptPosAtom->get_QtyInSharesDailyPrevDateSell(&dQtyDailyPrevDateSell));
				__CHECK_HRESULT3(spOptPosAtom->get_QtyInSharesDailyTodayBuy(&dQtyDailyTodayBuy));
				__CHECK_HRESULT3(spOptPosAtom->get_QtyInSharesDailyTodaySell(&dQtyDailyTodaySell));

				__CHECK_HRESULT3(spOptPosAtom->get_PosDailyPrevDateBuy(&dPosDailyPrevDateBuy));
				__CHECK_HRESULT3(spOptPosAtom->get_PosDailyPrevDateSell(&dPosDailyPrevDateSell));
				__CHECK_HRESULT3(spOptPosAtom->get_PosDailyTodayBuy(&dPosDailyTodayBuy));
				__CHECK_HRESULT3(spOptPosAtom->get_PosDailyTodaySell(&dPosDailyTodaySell));

				DOUBLE dOptPos = dQtyDailyPrevDateBuy + dQtyDailyTodayBuy - dQtyDailyPrevDateSell - dQtyDailyTodaySell;
				
				__CHECK_HRESULT3(spOptAtom->get_ID(&nOptID));
				__CHECK_HRESULT3(spOptAtom->get_RootID(&nOptRootID));
				__CHECK_HRESULT3(spOptAtom->get_Strike(&dStrike));
				__CHECK_HRESULT3(spOptAtom->get_ExpiryOV(&dtExpiryOV));
				__CHECK_HRESULT3(spOptAtom->get_TradingClose(&tmCloseTime));
				__CHECK_HRESULT3(spOptAtom->get_OptType(&enOptType));

				::GetCalculationParams(dtNow, dtExpiryOV, tmCloseTime, pParams->UseTimePrecision != VARIANT_FALSE, &dtNow, &dtExpiryOV, &tmCloseTime, &dYTE);

				dStrike = _AdjustFractionalDigits(dStrike, 4);

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

				DOUBLE dStockPricePoP = 0.;

				if (!bIsRootSynthetic)
				{
					dStockPricePoP = dStockPrice;
				}
				else //synthetic
				{
					dStockPricePoP = dCashValue;

					__CHECK_HRESULT(spSynthEnum->Reset(), _T("Fail to reset synthetic roots components collection."));
					while((hr = spSynthEnum->Next(1L, &varSynthItem, &nSynthFetched)) == S_OK)
					{
						ATLASSERT(varSynthItem.vt == VT_DISPATCH);
						spSynthRootCompAtom = varSynthItem;

						__CHECK_HRESULT3(spSynthRootCompAtom->get_UndID(&nSynthUndID));
						__CHECK_HRESULT3(spSynthRootCompAtom->get_Weight(&dSynthWeight));

						if (nSynthUndID != m_nID)
						{
							IMmRpUndAtomPtr spUndAtom;
							__CHECK_HRESULT3(spUndColl->get_Item(_bstr_t(nSynthUndID), &spUndAtom));
							if (spUndAtom != NULL)
							{
								IMMRpPricePtr spPrice ;
								spUndAtom->get_Price ( &spPrice ) ;
								ATLASSERT ( spPrice ) ;

								__CHECK_HRESULT3(spPrice->get_Bid(&dPriceBid));
								__CHECK_HRESULT3(spPrice->get_Ask(&dPriceAsk));
								__CHECK_HRESULT3(spPrice->get_Last(&dPriceLast));

								dStockPricePoP += m_spUndPriceProfile->GetUndPriceMid(dPriceBid, dPriceAsk, dPriceLast, 
									dUndPriceTolerance, enPriceRoundingRule, NULL, VARIANT_FALSE) * dSynthWeight * (1. + dShift / 100.);
							}
						}
						else
						{
							__CHECK_HRESULT3(m_spPrice->get_Bid(&dPriceBid));
							__CHECK_HRESULT3(m_spPrice->get_Ask(&dPriceAsk));
							__CHECK_HRESULT3(m_spPrice->get_Last(&dPriceLast));
							dStockPricePoP += m_spUndPriceProfile->GetUndPriceMid(dPriceBid, dPriceAsk, dPriceLast, 
									dUndPriceTolerance, enPriceRoundingRule, NULL, VARIANT_FALSE) * (1. + dShift / 100.);
						}
					}
				}

				EtsContractTypeEnum optionType;
				_CHK(spOptAtom->get_ContractType(&optionType) );

				GREEKS	aGreeks;
				memset(&aGreeks, 0, sizeof(GREEKS));
				LONG nRetCount = 0L;
				if (dtNow <= dtExpiryOV)
				{
					DOUBLE dHTBRate = BAD_DOUBLE_VALUE;

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
													dStockPricePoP, NULL, true, /*dShift*/0 );

					if (nRetCount && _finite(aGreeks.dTheoPrice) && !DoubleEQZero(aGreeks.dTheoPrice) )
					{
						dMarketValue += aGreeks.dTheoPrice * dOptPos;
						
						bool bBadVal = false;
						if (dQtyDailyPrevDateBuy > 0)
						{
							if (dPosDailyPrevDateBuy > 0)
								dOptionsTheoPnLDaily += aGreeks.dTheoPrice * dQtyDailyPrevDateBuy - dPosDailyPrevDateBuy;
							else
								bBadVal = true;
						}

						if (!bBadVal && dQtyDailyPrevDateSell > 0)
						{
							if (dPosDailyPrevDateSell > 0)
								dOptionsTheoPnLDaily -= aGreeks.dTheoPrice * dQtyDailyPrevDateSell - dPosDailyPrevDateSell;
							else
								bBadVal = true;
						}

						if (!bBadVal && dQtyDailyTodayBuy > 0)
						{
							if (dPosDailyTodayBuy > 0)
								dOptionsTheoPnLDaily += aGreeks.dTheoPrice * dQtyDailyTodayBuy - dPosDailyTodayBuy;
							else
								bBadVal = true;
						}

						if (!bBadVal && dQtyDailyTodaySell > 0)
						{
							if (dPosDailyTodaySell > 0)
								dOptionsTheoPnLDaily -= aGreeks.dTheoPrice * dQtyDailyTodaySell - dPosDailyTodaySell;
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

							if (_finite(aGreeks.dGamma))
							{
								dNetGamma += aGreeks.dGamma * dOptPos * dStockPricePoP * dStockPricePoP / 100.0;
							}

						}
					}
					else //synthetic
					{
						if (dCashValue > 0. && nRetCount > 0)
						{
							if (_finite(aGreeks.dDelta))
								dOptionsDelta +=  aGreeks.dDelta * dOptPos * dCashValue;

							if (_finite(aGreeks.dGamma))
								dTotalGamma += aGreeks.dGamma * dOptPos * dCashValue;
								
							if (_finite(aGreeks.dGamma))
								dNetGamma += aGreeks.dGamma * dOptPos * dStockPricePoP * dStockPricePoP / 100.0;
						}

						__CHECK_HRESULT(spSynthEnum->Reset(), _T("Fail to reset synthetic roots components collection."));
						while((hr = spSynthEnum->Next(1L, &varSynthItem, &nSynthFetched)) == S_OK)
						{
							ATLASSERT(varSynthItem.vt == VT_DISPATCH);
							spSynthRootCompAtom = varSynthItem;

							__CHECK_HRESULT3(spSynthRootCompAtom->get_Weight(&dSynthWeight));

							if (nRetCount)
							{
								if (_finite(aGreeks.dDelta))
								{
									dOptionsDelta += aGreeks.dDelta * dOptPos * dSynthWeight;
									
								}

								if (_finite(aGreeks.dGamma))
								{
									dTotalGamma += aGreeks.dGamma * dOptPos * dSynthWeight;
									dNetGamma += aGreeks.dGamma * dOptPos * dSynthWeight * dStockPricePoP * dStockPricePoP / 100.0;
								}
							}
						} 

					} 

					if (nRetCount)
					{
						if (_finite(aGreeks.dVega))
						{
							dTotalVega += aGreeks.dVega * dOptPos;
						}	

						if (_finite(aGreeks.dTheta))
						{
							dTotalTheta += aGreeks.dTheta * dOptPos;
						}	
					}

					if (m_vbIsAmerican != VARIANT_TRUE)
					{	
						DOUBLE	dYield = 0.;

						if (bIsRootSynthetic)
						{
							dYield = spSynthRootAtom->Yield;
							dStockPricePoP = _CalcSyntheticForwardPrice(spSynthRootAtom, 
																		dStockPricePoP, 
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
							dStockPricePoP = _CalcRegularForwardPrice(	dStockPricePoP,
																		dtExpiryOV,
																		tmCloseTime,
																		dtNow,
																		dYield,
																		*psaRates,
																		*psaYTEs,
																		ICalculationParametrsPtr(pParams));
						}
					}

					if (dStockPricePoP > 0.)
					{
                         DOUBLE dTimeValue = 0.;

                         if (enOptType == enOtCall)
						 {
							if (dStrike - dStockPricePoP > 0. || DoubleEQZero(dStrike - dStockPricePoP) )
							{
								if (nRetCount && _finite(aGreeks.dTheoPrice))
								{
									dTimeValue = aGreeks.dTheoPrice;
								}
							}
							else
							{
								if (nRetCount && _finite(aGreeks.dTheoPrice))
								{
									dTimeValue = aGreeks.dTheoPrice - dStockPricePoP + dStrike;
								}
							}
						 }
						 else
						 {
							if (dStrike - dStockPricePoP < 0. || DoubleEQZero(dStrike - dStockPricePoP))
							{
								if (nRetCount && _finite(aGreeks.dTheoPrice))
								{
									dTimeValue = aGreeks.dTheoPrice;
								}
							}
							else
							{
                                if (nRetCount && _finite(aGreeks.dTheoPrice))
								{
									dTimeValue = aGreeks.dTheoPrice + dStockPricePoP - dStrike;
								}
							}
						 }
                         
						 dPoP += dOptPos * dTimeValue;
					}
				} 
			} 

			spRiskMatrixAtom = 0;
			__CHECK_HRESULT3(spRiskMatrixColl->get_Item(m_nID, &spRiskMatrixAtom));
		
			IMmRpRiskMatrixShiftCollPtr	spRiskMatrixShiftColl;
			__CHECK_HRESULT3(spRiskMatrixAtom->get_Shift(&spRiskMatrixShiftColl));
			if (spRiskMatrixShiftColl != 0)
			{	
				_bstr_t bsKey = (_bstr_t)_variant_t(m_nID) + L"_shift" + (_bstr_t)_variant_t(nShift);
			
				IMmRpRiskMatrixShiftAtomPtr	spRiskMatrixShiftAtom;
				__CHECK_HRESULT3(spRiskMatrixShiftColl->Add(bsKey, NULL, &spRiskMatrixShiftAtom));

				__CHECK_HRESULT3(spRiskMatrixShiftAtom->put_ID(m_nID));
				__CHECK_HRESULT3(spRiskMatrixShiftAtom->put_Symbol(m_bstrSymbol));
				__CHECK_HRESULT3(spRiskMatrixShiftAtom->put_GroupID(0L));
				__CHECK_HRESULT3(spRiskMatrixShiftAtom->put_Price(dStockPrice));
				__CHECK_HRESULT3(spRiskMatrixShiftAtom->put_UndType(m_enUndType));
				__CHECK_HRESULT3(spRiskMatrixShiftAtom->put_Shift(dShift));
				__CHECK_HRESULT3(spRiskMatrixShiftAtom->put_MarketValue(dMarketValue));
				__CHECK_HRESULT3(spRiskMatrixShiftAtom->put_PoP(dPoP));
				__CHECK_HRESULT3(spRiskMatrixShiftAtom->put_OptionsTheoPnLDaily(dOptionsTheoPnLDaily));
				__CHECK_HRESULT3(spRiskMatrixShiftAtom->put_Delta(dOptionsDelta + dStockPos));
				__CHECK_HRESULT3(spRiskMatrixShiftAtom->put_Gamma(dTotalGamma));
				__CHECK_HRESULT3(spRiskMatrixShiftAtom->put_Theta(dTotalTheta));
				__CHECK_HRESULT3(spRiskMatrixShiftAtom->put_Vega(dTotalVega));
                __CHECK_HRESULT3(spRiskMatrixShiftAtom->put_MarketMove(dShift / 100.));
				__CHECK_HRESULT3(spRiskMatrixShiftAtom->put_NetGamma(dNetGamma));
			}
		}

// STD part
		if (!DoubleEQZero(dSTD1))
		{	
			for (LONG nMarketMove = -3; nMarketMove <= 3.; nMarketMove++ )
			{
				DOUBLE dShift = dSTD1 * nMarketMove;

				DOUBLE dStockPrice = dSpotPrice > 0 ? dSpotPrice * (1. + dShift / 100.) : BAD_DOUBLE_VALUE ;
				DOUBLE dMarketValue = (dStockPos /*+ dFutPos*/) * dStockPrice;

				DOUBLE dOptionsTheoPnLDaily = 0.;
				DOUBLE dOptionsDelta = 0.;
				DOUBLE dTotalGamma = 0.;
				DOUBLE dNetGamma = 0.;
				DOUBLE dTotalVega = 0.;
				DOUBLE dTotalTheta = 0.;
				DOUBLE dPoP = 0.;

				__CHECK_HRESULT(spOptEnum->Reset(), _T("Failed to reset options collection."));
				while((hr = spOptEnum->Next(1L, &varItem, &nFetched)) == S_OK)
				{
					ATLASSERT(varItem.vt == VT_DISPATCH);
					spOptAtom = varItem;

					IMmRpPosAtomPtr	spOptPosAtom;
					__CHECK_HRESULT3(spOptAtom->get_Pos(&spOptPosAtom));

					__CHECK_HRESULT3(spOptPosAtom->get_QtyInSharesDailyPrevDateBuy(&dQtyDailyPrevDateBuy));
					__CHECK_HRESULT3(spOptPosAtom->get_QtyInSharesDailyPrevDateSell(&dQtyDailyPrevDateSell));
					__CHECK_HRESULT3(spOptPosAtom->get_QtyInSharesDailyTodayBuy(&dQtyDailyTodayBuy));
					__CHECK_HRESULT3(spOptPosAtom->get_QtyInSharesDailyTodaySell(&dQtyDailyTodaySell));

					__CHECK_HRESULT3(spOptPosAtom->get_PosDailyPrevDateBuy(&dPosDailyPrevDateBuy));
					__CHECK_HRESULT3(spOptPosAtom->get_PosDailyPrevDateSell(&dPosDailyPrevDateSell));
					__CHECK_HRESULT3(spOptPosAtom->get_PosDailyTodayBuy(&dPosDailyTodayBuy));
					__CHECK_HRESULT3(spOptPosAtom->get_PosDailyTodaySell(&dPosDailyTodaySell));				

					DOUBLE dOptPos = dQtyDailyPrevDateBuy + dQtyDailyTodayBuy - dQtyDailyPrevDateSell - dQtyDailyTodaySell;

					__CHECK_HRESULT3(spOptAtom->get_ID(&nOptID));
					__CHECK_HRESULT3(spOptAtom->get_RootID(&nOptRootID));
					__CHECK_HRESULT3(spOptAtom->get_Strike(&dStrike));
					__CHECK_HRESULT3(spOptAtom->get_ExpiryOV(&dtExpiryOV));
					__CHECK_HRESULT3(spOptAtom->get_TradingClose(&tmCloseTime));
					__CHECK_HRESULT3(spOptAtom->get_OptType(&enOptType));
					
					::GetCalculationParams(dtNow, dtExpiryOV, tmCloseTime, pParams->UseTimePrecision != VARIANT_FALSE, &dtNow, &dtExpiryOV, &tmCloseTime, &dYTE);

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
					DOUBLE					dCashValue = 0L;

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

					DOUBLE dStockPricePoP = 0.;

					if (!bIsRootSynthetic)
					{
						dStockPricePoP = dSpotPrice;
					}
					else //synthetic
					{	
						dStockPricePoP = dCashValue;

						__CHECK_HRESULT(spSynthEnum->Reset(), _T("Fail to reset synthetic roots components collection."));
						while((hr = spSynthEnum->Next(1L, &varSynthItem, &nSynthFetched)) == S_OK)
						{
							ATLASSERT(varSynthItem.vt == VT_DISPATCH);
							spSynthRootCompAtom = varSynthItem;

							__CHECK_HRESULT3(spSynthRootCompAtom->get_UndID(&nSynthUndID));
							__CHECK_HRESULT3(spSynthRootCompAtom->get_Weight(&dSynthWeight));

							//if (nSynthUndID != m_nID)
							//{
								IMmRpUndAtomPtr spUndAtom;
								__CHECK_HRESULT3(spUndColl->get_Item(_bstr_t(nSynthUndID), &spUndAtom));
								if (spUndAtom != NULL)
								{
									IMMRpPricePtr spPrice ;
									spUndAtom->get_Price ( &spPrice ) ;
									ATLASSERT ( spPrice ) ;

									__CHECK_HRESULT3(spPrice->get_Bid(&dPriceBid));
									__CHECK_HRESULT3(spPrice->get_Ask(&dPriceAsk));
									__CHECK_HRESULT3(spPrice->get_Last(&dPriceLast));

									dStockPricePoP += m_spUndPriceProfile->GetUndPriceMid(dPriceBid, dPriceAsk, dPriceLast, 
										dUndPriceTolerance, enPriceRoundingRule, NULL, VARIANT_FALSE) * dSynthWeight * (1. + dShift / 100.);
								}
						}
					}

					GREEKS	aGreeks;
					memset(&aGreeks, 0, sizeof(GREEKS));
					LONG nRetCount = 0L;
					EtsContractTypeEnum optionType;
					_CHK(spOptAtom->get_ContractType(&optionType) );
					if (dtNow <= dtExpiryOV)
					{
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
														dStockPricePoP, NULL, true, /*dShift*/0);

						if (nRetCount > 0 && _finite(aGreeks.dTheoPrice) && !DoubleEQZero(aGreeks.dTheoPrice) )
						{
							dMarketValue += aGreeks.dTheoPrice * dOptPos;

							bool bBadVal = false;
							if (dQtyDailyPrevDateBuy > 0)
							{
								if (dPosDailyPrevDateBuy > 0)
									dOptionsTheoPnLDaily += aGreeks.dTheoPrice * dQtyDailyPrevDateBuy - dPosDailyPrevDateBuy;
								else
									bBadVal = true;
							}

							if (!bBadVal && dQtyDailyPrevDateSell > 0)
							{	
								if (dPosDailyPrevDateSell > 0)
									dOptionsTheoPnLDaily -= aGreeks.dTheoPrice * dQtyDailyPrevDateSell - dPosDailyPrevDateSell;
								else
									bBadVal = true;
							}

							if (!bBadVal && dQtyDailyTodayBuy > 0)
							{
								if (dPosDailyTodayBuy > 0)
									dOptionsTheoPnLDaily += aGreeks.dTheoPrice * dQtyDailyTodayBuy - dPosDailyTodayBuy;
								else
									bBadVal = true;
							}

							if (!bBadVal && dQtyDailyTodaySell > 0)
							{	
								if (dPosDailyTodaySell > 0)
									dOptionsTheoPnLDaily -= aGreeks.dTheoPrice * dQtyDailyTodaySell - dPosDailyTodaySell;
								else
									bBadVal = true;
							}
						}

						if (!bIsRootSynthetic)
						{
							if (nRetCount > 0 && _finite(aGreeks.dDelta))
							{
							    dOptionsDelta += aGreeks.dDelta * dOptPos;
							}

							if (nRetCount > 0 && _finite(aGreeks.dGamma))
							{
								dTotalGamma += aGreeks.dGamma * dOptPos;
								dNetGamma += aGreeks.dGamma * dOptPos * dStockPricePoP * dStockPricePoP / 100.0;
							}


						}
						else //synthetic
						{
							if (dCashValue > 0 && nRetCount > 0)
							{
								if (_finite(aGreeks.dDelta))
								{
									dOptionsDelta += aGreeks.dDelta * dOptPos *dCashValue;
								}

								if (_finite(aGreeks.dGamma))
								{
									dTotalGamma += aGreeks.dGamma * dOptPos *dCashValue;
									dNetGamma	+= aGreeks.dGamma * dOptPos *dCashValue  * dStockPricePoP * dStockPricePoP / 100.0;
								}
							}

							__CHECK_HRESULT(spSynthEnum->Reset(), _T("Fail to reset synthetic roots components collection."));
							while((hr = spSynthEnum->Next(1L, &varSynthItem, &nSynthFetched)) == S_OK)
							{
								ATLASSERT(varSynthItem.vt == VT_DISPATCH);
								spSynthRootCompAtom = varSynthItem;

								__CHECK_HRESULT3(spSynthRootCompAtom->get_UndID(&nSynthUndID));
								__CHECK_HRESULT3(spSynthRootCompAtom->get_Weight(&dSynthWeight));

								if (nRetCount && _finite(aGreeks.dDelta))
									dOptionsDelta += aGreeks.dDelta * dOptPos * dSynthWeight;

								if (nRetCount && _finite(aGreeks.dGamma))
									dTotalGamma += aGreeks.dGamma * dOptPos * dSynthWeight;

								if (nRetCount && _finite(aGreeks.dGamma))
									dNetGamma += aGreeks.dGamma * dOptPos * dSynthWeight* dStockPricePoP * dStockPricePoP / 100.0;

								
							} //while
						} //if (!bIsRootSynthetic)

						if (nRetCount && _finite(aGreeks.dVega))
						{
							dTotalVega += aGreeks.dVega * dOptPos;
						}

						if (nRetCount && _finite(aGreeks.dTheta))
						{
							dTotalTheta += aGreeks.dTheta * dOptPos;
						}

						if (m_vbIsAmerican != VARIANT_TRUE)
						{
							DOUBLE	dYield = 0.;

							if (bIsRootSynthetic)
							{
								dYield = spSynthRootAtom->Yield;
								dStockPricePoP = _CalcSyntheticForwardPrice(spSynthRootAtom, 
																			dStockPricePoP, 
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
								dStockPricePoP = _CalcRegularForwardPrice(	dStockPricePoP,
																			dtExpiryOV,
																			tmCloseTime,
																			dtNow, 
																			dYield,
																			*psaRates,
																			*psaYTEs,
																			ICalculationParametrsPtr(pParams));
							}
						}

						if (dStockPricePoP > 0.)
						{
							DOUBLE dTimeValue = 0.;

							if (enOptType == enOtCall)
							{
								if (dStrike - dStockPricePoP > 0. || DoubleEQZero(dStrike - dStockPricePoP) )
								{
									if (nRetCount && _finite(aGreeks.dTheoPrice))
									{
										dTimeValue = aGreeks.dTheoPrice;
									}
								}
								else
								{
									if (nRetCount && _finite(aGreeks.dTheoPrice))
									{
										dTimeValue = aGreeks.dTheoPrice - dStockPricePoP + dStrike;
									}
								}
							}
							else
							{
								if (dStrike - dStockPricePoP < 0. || DoubleEQZero(dStrike - dStockPricePoP))
								{
									if (nRetCount && _finite(aGreeks.dTheoPrice))
									{
										dTimeValue = aGreeks.dTheoPrice;
									}
								}
								else
								{
									if (nRetCount && _finite(aGreeks.dTheoPrice))
									{
										dTimeValue = aGreeks.dTheoPrice + dStockPricePoP - dStrike;
									}
								}
							}
                         
							dPoP += dOptPos * dTimeValue;
						}
					}
				} // while options

				spRiskMatrixAtom = 0;
				__CHECK_HRESULT3(spRiskMatrixColl->get_Item(m_nID, &spRiskMatrixAtom));
			
				IMmRpRiskMatrixShiftCollPtr	spRiskMatrixShiftColl;
				__CHECK_HRESULT3(spRiskMatrixAtom->get_Shift(&spRiskMatrixShiftColl));
				if (spRiskMatrixShiftColl != 0)
				{	
					_bstr_t bsKey = (_bstr_t)_variant_t(m_nID) + L"_mmove" + (_bstr_t)_variant_t(nMarketMove);
				
					IMmRpRiskMatrixShiftAtomPtr	spRiskMatrixShiftAtom;
					__CHECK_HRESULT3(spRiskMatrixShiftColl->Add(bsKey, NULL, &spRiskMatrixShiftAtom));

					__CHECK_HRESULT3(spRiskMatrixShiftAtom->put_ID(m_nID));
					__CHECK_HRESULT3(spRiskMatrixShiftAtom->put_Symbol(m_bstrSymbol));
					__CHECK_HRESULT3(spRiskMatrixShiftAtom->put_GroupID(1L));
					__CHECK_HRESULT3(spRiskMatrixShiftAtom->put_Price(dStockPrice));
					__CHECK_HRESULT3(spRiskMatrixShiftAtom->put_UndType(m_enUndType));				
					__CHECK_HRESULT3(spRiskMatrixShiftAtom->put_Shift(dShift));
					__CHECK_HRESULT3(spRiskMatrixShiftAtom->put_MarketValue(dMarketValue));
					__CHECK_HRESULT3(spRiskMatrixShiftAtom->put_PoP(dPoP));
					__CHECK_HRESULT3(spRiskMatrixShiftAtom->put_OptionsTheoPnLDaily(dOptionsTheoPnLDaily));
					__CHECK_HRESULT3(spRiskMatrixShiftAtom->put_Delta(dOptionsDelta + dStockPos));
					__CHECK_HRESULT3(spRiskMatrixShiftAtom->put_Gamma(dTotalGamma));
					__CHECK_HRESULT3(spRiskMatrixShiftAtom->put_Theta(dTotalTheta));
					__CHECK_HRESULT3(spRiskMatrixShiftAtom->put_Vega(dTotalVega));
					__CHECK_HRESULT3(spRiskMatrixShiftAtom->put_MarketMove(nMarketMove));
					__CHECK_HRESULT3(spRiskMatrixShiftAtom->put_NetGamma(dNetGamma));
				}
			} 
		}
        
// Fill Market Change Value
		IUnknownPtr	spRiskMatrixUnk;
		IMmRpRiskMatrixShiftCollPtr	spRiskMatrixShiftColl;
		IMmRpRiskMatrixShiftAtomPtr	spRiskMatrixShiftAtom;

		__CHECK_HRESULT(spRiskMatrixColl->get__NewEnum(&spRiskMatrixUnk), _T("Failed to get risk matrix collection."));		
		IEnumVARIANTPtr spRiskMatrixEnum(spRiskMatrixUnk);
		
		varItem.Clear();

		__CHECK_HRESULT(spRiskMatrixEnum->Reset(), _T("Fail to reset risk matrix collection."));
		while((hr = spRiskMatrixEnum->Next(1L, &varItem, &nFetched)) == S_OK)
		{
			ATLASSERT(varItem.vt == VT_DISPATCH);
			spRiskMatrixAtom = varItem;

			spRiskMatrixShiftColl = 0;
			__CHECK_HRESULT3(spRiskMatrixAtom->get_Shift(&spRiskMatrixShiftColl));

			LONG nID = 0L;
			__CHECK_HRESULT3(spRiskMatrixAtom->get_ID(&nID));

			if (nID != __USD_ID__)
			{
				_bstr_t	bsKey = (_bstr_t)_variant_t(nID) + L"_shift" + (_bstr_t)_variant_t(0L);

				spRiskMatrixShiftAtom = 0;
				__CHECK_HRESULT3(spRiskMatrixShiftColl->get_Item(bsKey, &spRiskMatrixShiftAtom));
				if (spRiskMatrixShiftAtom != 0)
				{
					DOUBLE	dMarketValue = 0.;
					DOUBLE	dMarketValueZeroShift = 0.;
					__CHECK_HRESULT3(spRiskMatrixShiftAtom->get_MarketValue(&dMarketValueZeroShift));

					for (LONG nShift = -20; nShift <= 20; nShift+=2)
					{
						if (nShift != 0L)
						{
							bsKey = (_bstr_t)_variant_t(nID) + L"_shift" + (_bstr_t)_variant_t(nShift);

							spRiskMatrixShiftAtom = 0;
							__CHECK_HRESULT3(spRiskMatrixShiftColl->get_Item(bsKey, &spRiskMatrixShiftAtom));
							if (spRiskMatrixShiftAtom != 0)
							{
								__CHECK_HRESULT3(spRiskMatrixShiftAtom->get_MarketValue(&dMarketValue));
								__CHECK_HRESULT3(spRiskMatrixShiftAtom->put_MarketValueChange(dMarketValue - dMarketValueZeroShift));
							}
						}
					}
				}

				bsKey = (_bstr_t)_variant_t(nID) + L"_mmove" + (_bstr_t)_variant_t(0L);

				spRiskMatrixShiftAtom = 0;
				__CHECK_HRESULT3(spRiskMatrixShiftColl->get_Item(bsKey, &spRiskMatrixShiftAtom));
				if (spRiskMatrixShiftAtom != 0)
				{
					DOUBLE	dMarketValue = 0.;
					DOUBLE	dMarketValueZeroShift = 0.;
					__CHECK_HRESULT3(spRiskMatrixShiftAtom->get_MarketValue(&dMarketValueZeroShift));

					for (LONG nMarketMove = -3; nMarketMove <= 3.; nMarketMove++ )
					{
						if (nMarketMove != 0L)
						{
							bsKey = (_bstr_t)_variant_t(nID) + L"_mmove" + (_bstr_t)_variant_t(nMarketMove);

							spRiskMatrixShiftAtom = 0;
							__CHECK_HRESULT3(spRiskMatrixShiftColl->get_Item(bsKey, &spRiskMatrixShiftAtom));
							if (spRiskMatrixShiftAtom != 0)
							{
								__CHECK_HRESULT3(spRiskMatrixShiftAtom->get_MarketValue(&dMarketValue));
								__CHECK_HRESULT3(spRiskMatrixShiftAtom->put_MarketValueChange(dMarketValue - dMarketValueZeroShift));
							}
						}
					}
				}

			}
		}

		*pRetVal = spRiskMatrixColl.Detach();
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)CComErrorWrapper::ErrorDescription(e), IID_IMmRpUndAtom, e.Error());
	}

	return S_OK;
}

STDMETHODIMP CMmRpUndAtom::CalcRiskMatrixTotals(IMmRpRiskMatrixColl* pVal)
{
	if (!pVal)
		return E_INVALIDARG;

	__CHECK_POINTER(pVal);

	try
	{
		IMmRpRiskMatrixCollPtr	spColl = pVal;

		IUnknownPtr		spUnk;
		_variant_t		varItem;
		ULONG			nFetched = 0L;
		HRESULT			hr;

		__CHECK_HRESULT(spColl->get__NewEnum(&spUnk), _T("Failed to get stocks collection."));
		IEnumVARIANTPtr spEnum(spUnk);		
		IMmRpRiskMatrixAtomPtr	spTotalAtom;

		while((hr = spEnum->Next(1L, &varItem, &nFetched)) == S_OK)
		{
			ATLASSERT(varItem.vt == VT_DISPATCH);
			IMmRpRiskMatrixAtomPtr	spAtom = varItem;

			LONG nID = -1L;
			if (spTotalAtom == 0)
			{
				__CHECK_HRESULT3(spColl->Add(nID, L"0", NULL, &spTotalAtom));
				__CHECK_HRESULT3(spTotalAtom->put_ID(nID));
				__CHECK_HRESULT3(spTotalAtom->put_Symbol(L"Totals"));
			}

			IMmRpRiskMatrixShiftCollPtr spShiftColl;
			__CHECK_HRESULT3(spAtom->get_Shift(&spShiftColl));
			IMmRpRiskMatrixShiftCollPtr spTotalShiftColl;
			__CHECK_HRESULT3(spTotalAtom->get_Shift(&spTotalShiftColl));

			__CHECK_HRESULT3(spAtom->get_ID(&nID));

			if (spShiftColl != 0 && spTotalShiftColl != 0)
			{
				for (LONG nShift = -20; nShift <= 20; nShift += 2)
				{
					_bstr_t bsKey = (_bstr_t)_variant_t(nID) + L"_shift" + (_bstr_t)_variant_t(nShift);
					IMmRpRiskMatrixShiftAtomPtr	spShiftAtom;
					__CHECK_HRESULT3(spShiftColl->get_Item(bsKey, &spShiftAtom));
					
					bsKey = L"0" L"_shift" + (_bstr_t)_variant_t(nShift);
					IMmRpRiskMatrixShiftAtomPtr	spTotalShiftAtom;
					__CHECK_HRESULT3(spTotalShiftColl->get_Item(bsKey, &spTotalShiftAtom));

					if (spTotalShiftAtom == 0)
					{
						__CHECK_HRESULT3(spTotalShiftColl->Add(bsKey, NULL, &spTotalShiftAtom));
						__CHECK_HRESULT3(spTotalShiftAtom->put_ID(0));
						__CHECK_HRESULT3(spTotalShiftAtom->put_Symbol(L"Total"));
						__CHECK_HRESULT3(spTotalShiftAtom->put_GroupID(0L));
						__CHECK_HRESULT3(spTotalShiftAtom->put_Shift(nShift));
						__CHECK_HRESULT3(spTotalShiftAtom->put_MarketMove((double)nShift / 100.));
					}

					if (spShiftAtom != 0 && spTotalShiftAtom != 0)
					{
						DOUBLE dMktVal = 0;
						DOUBLE dMktValTot = 0;
						DOUBLE dMktValChg = 0;
						DOUBLE dMktValChgTot = 0;
						DOUBLE dPoP = 0;
						DOUBLE dPoPTot = 0;
						DOUBLE dPnL = 0;
						DOUBLE dPnLTot = 0;
						DOUBLE dDlt = 0;
						DOUBLE dDltTot = 0;
						DOUBLE dGma = 0;
						DOUBLE dNetGma = 0;
						DOUBLE dNetGmaTot = 0;
						DOUBLE dGmaTot = 0;
						DOUBLE dTht = 0;
						DOUBLE dThtTot = 0;
						DOUBLE dVga = 0;
						DOUBLE dVgaTot = 0;

						__CHECK_HRESULT3(spTotalShiftAtom->get_MarketValue(&dMktValTot));
						__CHECK_HRESULT3(spTotalShiftAtom->get_MarketValueChange(&dMktValChgTot));
						__CHECK_HRESULT3(spTotalShiftAtom->get_PoP(&dPoPTot));
						__CHECK_HRESULT3(spTotalShiftAtom->get_OptionsTheoPnLDaily(&dPnLTot));
						__CHECK_HRESULT3(spTotalShiftAtom->get_Delta(&dDltTot));
						__CHECK_HRESULT3(spTotalShiftAtom->get_Gamma(&dGmaTot));
						__CHECK_HRESULT3(spTotalShiftAtom->get_Theta(&dThtTot));
						__CHECK_HRESULT3(spTotalShiftAtom->get_Vega(&dVgaTot));
						__CHECK_HRESULT3(spTotalShiftAtom->get_NetGamma(&dNetGmaTot));

						__CHECK_HRESULT3(spShiftAtom->get_MarketValue(&dMktVal));
						__CHECK_HRESULT3(spShiftAtom->get_MarketValueChange(&dMktValChg));
						__CHECK_HRESULT3(spShiftAtom->get_PoP(&dPoP));
						__CHECK_HRESULT3(spShiftAtom->get_OptionsTheoPnLDaily(&dPnL));
						__CHECK_HRESULT3(spShiftAtom->get_Delta(&dDlt));
						__CHECK_HRESULT3(spShiftAtom->get_Gamma(&dGma));
						__CHECK_HRESULT3(spShiftAtom->get_Theta(&dTht));
						__CHECK_HRESULT3(spShiftAtom->get_Vega(&dVga));
						__CHECK_HRESULT3(spShiftAtom->get_NetGamma(&dNetGma));

						__CHECK_HRESULT3(spTotalShiftAtom->put_MarketValue(dMktValTot + dMktVal));
						__CHECK_HRESULT3(spTotalShiftAtom->put_MarketValueChange(dMktValChgTot + dMktValChg));
						__CHECK_HRESULT3(spTotalShiftAtom->put_PoP(dPoPTot + dPoP));
						__CHECK_HRESULT3(spTotalShiftAtom->put_OptionsTheoPnLDaily(dPnLTot + dPnL));
						__CHECK_HRESULT3(spTotalShiftAtom->put_Delta(dDltTot + dDlt));
						__CHECK_HRESULT3(spTotalShiftAtom->put_Gamma(dGmaTot + dGma));
						__CHECK_HRESULT3(spTotalShiftAtom->put_Theta(dThtTot + dTht));
						__CHECK_HRESULT3(spTotalShiftAtom->put_Vega(dVgaTot + dVga));
						__CHECK_HRESULT3(spTotalShiftAtom->put_NetGamma(dNetGmaTot + dNetGma));
					}
				}
			}
		}
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)CComErrorWrapper::ErrorDescription(e), IID_IMmRpUndAtom, e.Error());
	}

	return S_OK;
}

STDMETHODIMP CMmRpUndAtom::CalcPosWithEarlyExercise(EtsCalcModelTypeEnum enCalcModel, 
													SAFEARRAY** psaRates, 
													SAFEARRAY** psaYTEs, 
													SAFEARRAY** psaHTBRates, 
													SAFEARRAY** psaHTBYTEs, 
													VARIANT_BOOL bUseTheoVolatility, 
													VARIANT_BOOL bUseTheoVolaNoBid, 
													VARIANT_BOOL bUseTheoVolaBadMarket,
													IMmRpUndColl* _pUndColl,
													DOUBLE dUndPriceTolerance,
													EtsPriceRoundingRuleEnum enPriceRoundingRule,
													IMmRpPosWithEarlyExerciseAtom* pPosWithEarlyExerciseAtom,
													VARIANT_BOOL EAOptWithPos,
													VARIANT_BOOL EAUpComDivs,
													LONG _EADaysToDiv,
													//IMmRpPosWithEarlyExerciseAtom* pPosWithEarlyExerciseAtom,
													ICalculationParametrs* pParams,
													IMmRpPosWithEarlyExerciseAtom** ppRetVal)
{ 
	if(!_pUndColl || pParams == NULL)
		return E_INVALIDARG;

	__CHECK_POINTER(ppRetVal);

	try
	{
		if(*ppRetVal)
		{
			(*ppRetVal)->Release();
			*ppRetVal = 0;
		}

		DOUBLE dUndBid , dUndAsk , dUndLast ;
		m_spPrice->get_Bid ( &dUndBid) ;
		m_spPrice->get_Ask ( &dUndAsk ) ;
		m_spPrice->get_Last( &dUndLast ) ;

		HRESULT	hr = S_OK;
		IMmRpUndCollPtr	sp_UndColl(_pUndColl);
		IMmRpPosWithEarlyExerciseAtomPtr	spPosWithEarlyExAtom = pPosWithEarlyExerciseAtom;
		IMmRpPosWithEarlyExerciseOptCollPtr	spPosWithEarlyExOptColl;

		if (spPosWithEarlyExAtom == 0)
		{
			__CHECK_HRESULT(spPosWithEarlyExAtom.CreateInstance(CLSID_MmRpPosWithEarlyExerciseAtom), 
				_T("Failed to create MmRpPosWithEarlyExerciseAtom object."));

			__CHECK_HRESULT3(spPosWithEarlyExAtom->put_ID(m_nID));
			__CHECK_HRESULT3(spPosWithEarlyExAtom->put_Symbol(m_bstrSymbol));
			__CHECK_HRESULT3(spPosWithEarlyExAtom->put_UndType(m_enUndType));
		}

		__CHECK_HRESULT3(spPosWithEarlyExAtom->get_Opt(&spPosWithEarlyExOptColl));
		ATLASSERT(spPosWithEarlyExOptColl != 0);

		DOUBLE	dQtyDailyPrevDateBuy = 0.;
		DOUBLE	dQtyDailyPrevDateSell = 0.;
		DOUBLE	dQtyDailyTodayBuy = 0.;
		DOUBLE	dQtyDailyTodaySell = 0.;

		DOUBLE	dQtyInSharesDailyPrevDateBuy = 0.;
		DOUBLE	dQtyInSharesDailyPrevDateSell = 0.;
		DOUBLE	dQtyInSharesDailyTodayBuy = 0.;
		DOUBLE	dQtyInSharesDailyTodaySell = 0.;

		DOUBLE	dPosDailyPrevDateBuy = 0.;
		DOUBLE	dPosDailyPrevDateSell = 0.;
		DOUBLE	dPosDailyTodayBuy = 0.;
		DOUBLE	dPosDailyTodaySell = 0.;


		CSafeArrayWrapper<double> saDates;
		CSafeArrayWrapper<double> saAmounts;


		long   nDivCount = 0;


		__CHECK_HRESULT3(m_spPos->get_QtyDailyPrevDateBuy(&dQtyDailyPrevDateBuy));
		__CHECK_HRESULT3(m_spPos->get_QtyDailyPrevDateSell(&dQtyDailyPrevDateSell));
		__CHECK_HRESULT3(m_spPos->get_QtyDailyTodayBuy(&dQtyDailyTodayBuy));
		__CHECK_HRESULT3(m_spPos->get_QtyDailyTodaySell(&dQtyDailyTodaySell));

		__CHECK_HRESULT3(m_spPos->get_PosDailyPrevDateBuy(&dPosDailyPrevDateBuy));
		__CHECK_HRESULT3(m_spPos->get_PosDailyPrevDateSell(&dPosDailyPrevDateSell));
		__CHECK_HRESULT3(m_spPos->get_PosDailyTodayBuy(&dPosDailyTodayBuy));
		__CHECK_HRESULT3(m_spPos->get_PosDailyTodaySell(&dPosDailyTodaySell));

		DOUBLE	dFutPos = 0.;
		{ /***********************  collect futures position data *****************************/
			IUnknownPtr		spFutUnk;
			_variant_t		varItem;
			ULONG			nFetched = 0L;	
			IMmRpFutAtomPtr	spFutAtom;

			DOUBLE	dFutQtyDailyPrevDateBuy = 0.;
			DOUBLE	dFutQtyDailyPrevDateSell = 0.;
			DOUBLE	dFutQtyDailyTodayBuy = 0.;
			DOUBLE	dFutQtyDailyTodaySell = 0.;


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
				dFutPos += (dFutQtyDailyPrevDateBuy + dFutQtyDailyTodayBuy - dFutQtyDailyPrevDateSell - dFutQtyDailyTodaySell);		
			}
			/***********************************************************************************/}

		DOUBLE	dStockPos = dFutPos + dQtyDailyPrevDateBuy + dQtyDailyTodayBuy - dQtyDailyPrevDateSell - dQtyDailyTodaySell;		
		DOUBLE	dSpotPrice = m_spUndPriceProfile->GetUndPriceMid(dUndBid, dUndAsk, dUndLast, dUndPriceTolerance, enPriceRoundingRule, NULL, VARIANT_FALSE);

		DOUBLE	d_PriceBid = 0.;
		DOUBLE	d_PriceAsk = 0.;
		DOUBLE	d_PriceLast = 0.;
		DOUBLE	d_Price = 0.;

		DOUBLE	d_Strike = 0.;
		LONG	nOptID = 0L;
		LONG	nOptRootID = 0L;
		DATE	dtExpiry = 0., dtExpiryOV = 0., tmCloseTime = 0.;

		EtsOptionTypeEnum	enOptType;

		DOUBLE	dOptCallPos = 0.;
		DOUBLE	dOptPutPos = 0.;
		DOUBLE	dOptionsDelta = 0.;

		//DATE	nToday = (DATE)COleDateTime::GetCurrentTime();
		DATE dtNow = 0.0, dYTE = 0.0;
		GetNYDateTimeAsDATE(&dtNow);

		__SynthTotalValuesMap	__SynthTotalValues;

		GREEKS	aGreeks;
		memset(&aGreeks, 0, sizeof(GREEKS));

		IUnknownPtr		spOptUnk;
		_variant_t		varItem;
		ULONG			nFetched = 0L;	
		IMmRpOptAtomPtr	spOptAtom;

		__CHECK_HRESULT(m_spOpt->get__NewEnum(&spOptUnk), _T("Failed to get options collection."));		
		IEnumVARIANTPtr spOptEnum(spOptUnk);

		__CHECK_HRESULT(spOptEnum->Reset(), _T("Failed to reset options collection."));
		
		while((hr = spOptEnum->Next(1L, &varItem, &nFetched)) == S_OK)
		{
		
			ATLASSERT(varItem.vt == VT_DISPATCH);
			spOptAtom = varItem;

			IMmRpPosAtomPtr	spOptPosAtom;
			__CHECK_HRESULT3(spOptAtom->get_Pos(&spOptPosAtom));

			__CHECK_HRESULT3(spOptPosAtom->get_QtyDailyPrevDateBuy(&dQtyDailyPrevDateBuy));
			__CHECK_HRESULT3(spOptPosAtom->get_QtyDailyPrevDateSell(&dQtyDailyPrevDateSell));
			__CHECK_HRESULT3(spOptPosAtom->get_QtyDailyTodayBuy(&dQtyDailyTodayBuy));
			__CHECK_HRESULT3(spOptPosAtom->get_QtyDailyTodaySell(&dQtyDailyTodaySell));

			__CHECK_HRESULT3(spOptPosAtom->get_QtyInSharesDailyPrevDateBuy(&dQtyInSharesDailyPrevDateBuy));
			__CHECK_HRESULT3(spOptPosAtom->get_QtyInSharesDailyPrevDateSell(&dQtyInSharesDailyPrevDateSell));
			__CHECK_HRESULT3(spOptPosAtom->get_QtyInSharesDailyTodayBuy(&dQtyInSharesDailyTodayBuy));
			__CHECK_HRESULT3(spOptPosAtom->get_QtyInSharesDailyTodaySell(&dQtyInSharesDailyTodaySell));

			DOUBLE dOptPosInShares = dQtyInSharesDailyPrevDateBuy + dQtyInSharesDailyTodayBuy - dQtyInSharesDailyPrevDateSell - dQtyInSharesDailyTodaySell;
			DOUBLE dOptPos = dQtyDailyPrevDateBuy + dQtyDailyTodayBuy - dQtyDailyPrevDateSell - dQtyDailyTodaySell;

			__CHECK_HRESULT3(spOptAtom->get_ID(&nOptID));
			__CHECK_HRESULT3(spOptAtom->get_RootID(&nOptRootID));
			__CHECK_HRESULT3(spOptAtom->get_Strike(&d_Strike));
			__CHECK_HRESULT3(spOptAtom->get_Expiry(&dtExpiry));
			__CHECK_HRESULT3(spOptAtom->get_ExpiryOV(&dtExpiryOV));
			__CHECK_HRESULT3(spOptAtom->get_TradingClose(&tmCloseTime));
			__CHECK_HRESULT3(spOptAtom->get_OptType(&enOptType));

			::GetCalculationParams(dtNow, dtExpiryOV, tmCloseTime, pParams->UseTimePrecision != VARIANT_FALSE, &dtNow, &dtExpiryOV, &tmCloseTime, &dYTE);

			d_Strike = _AdjustFractionalDigits(d_Strike, 4);
			
			CComBSTR bstrOptSymbol;
			__CHECK_HRESULT3(spOptAtom->get_Symbol(&bstrOptSymbol));

			IMMRpPricePtr spPrice ;
			spOptAtom->get_Price( &spPrice ) ;
			ATLASSERT ( spPrice ) ;

			__CHECK_HRESULT3(spPrice->get_Bid(&d_PriceBid));
			__CHECK_HRESULT3(spPrice->get_Ask(&d_PriceAsk));
			__CHECK_HRESULT3(spPrice->get_Last(&d_PriceLast));
			
			DOUBLE d_OptPrice = m_spOptPriceProfile->GetOptPriceMid(d_PriceBid, d_PriceAsk, d_PriceLast, enPriceRoundingRule, bUseTheoVolatility, 0., NULL);

			if (enOptType == enOtCall)
			{
				dOptCallPos = dOptPos;
				dOptPutPos = 0.;
			}
			else
			{
				dOptCallPos = 0.;
				dOptPutPos = dOptPos;
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
			DOUBLE					dSynthWeight = 0.;				
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

			if (bIsRootSynthetic)
			{
				double dd_PriceBid = d_PriceBid , dd_PriceAsk = d_PriceAsk, dd_PriceLast = d_PriceLast ;
				_GetSyntheticPrice(spSynthRootAtom, sp_UndColl, dd_PriceBid, dd_PriceAsk, dd_PriceLast);
				d_Price = m_spUndPriceProfile->GetUndPriceMid(d_PriceBid, dd_PriceAsk, dd_PriceLast, dUndPriceTolerance, enPriceRoundingRule, NULL, VARIANT_FALSE );
			}
			if ( !bIsRootSynthetic || d_Price<=0 )
			{
				d_Price = dSpotPrice;
			}

			// check whether this option is for future
			double futureBid = 0., futureAsk = 0., futureLast = 0.;
			EtsContractTypeEnum	optionType;
			__CHECK_HRESULT3(spOptAtom->get_ContractType(&optionType));
			if ( enCtFutOption == optionType)	{
				IMmRpFutAtomPtr	future;
				__CHECK_HRESULT3( spOptAtom->get_Fut(&future));
				if (future)	{
					IEtsPriceProfileAtomPtr	futurePriceProfile;
					__CHECK_HRESULT3(future->get_UndPriceProfile(&futurePriceProfile) );
					if ( futurePriceProfile )	{
						IMMRpPricePtr futurePrice ;
						future->get_Price ( &futurePrice ) ;
						if ( futurePrice)	{
							futurePrice->get_Bid(&futureBid);
							futurePrice->get_Bid(&futureAsk);
							futurePrice->get_Bid(&futureLast);
							d_Price = futurePriceProfile->GetUndPriceMid(futureBid, futureAsk, futureLast, 
																		dUndPriceTolerance, enPriceRoundingRule, NULL, VARIANT_FALSE );
						}
					}
				}
			}
			LONG nRetCount = 0L;

			if (dtNow <= dtExpiryOV && !DoubleEQZero(dOptPosInShares))
			{
				memset(&aGreeks, 0, sizeof(aGreeks));
				aGreeks.nMask = GT_DELTA;

				nRetCount = _CalcOptionGreeks(	enCalcModel, 
					spOptAtom, 
					_InterpolateRate(dYTE, *psaRates, *psaYTEs, ICalculationParametrsPtr(pParams)), 
					_InterpolateRate(dYTE, *psaHTBRates, *psaHTBYTEs, ICalculationParametrsPtr(pParams)),
					bUseTheoVolatility, 
					bUseTheoVolaNoBid, 
					bUseTheoVolaBadMarket, 
					sp_UndColl, 
					dUndPriceTolerance,
					enPriceRoundingRule, 
					aGreeks,
					ICalculationParametrsPtr(pParams),
					0, NULL, false);
			}

			if (!bIsRootSynthetic)
			{
				if (nRetCount && _finite(aGreeks.dDelta))
				{
					dOptionsDelta += aGreeks.dDelta * dOptPosInShares;
				}
			}
			else //synthetic
			{
				if (dCashValue > 0.)
				{
					DOUBLE dSynthOptionsDelta = 0.;

					if(nRetCount && _finite(aGreeks.dDelta))
					{
						dSynthOptionsDelta = aGreeks.dDelta * dOptPosInShares * dCashValue;
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

						__SynthTotalValues.insert(std::make_pair(__USD_ID__, tValues));
					}
					else
					{
						__MmRpSynthTotalValues& tValues = iter->second;
						tValues.m_dOptionsDelta += dSynthOptionsDelta;				
					}
				}

				__CHECK_HRESULT(spSynthEnum->Reset(), _T("Fail to reset synthetic roots components collection."));
				while((hr = spSynthEnum->Next(1L, &varSynthItem, &nSynthFetched)) == S_OK)
				{
					ATLASSERT(varSynthItem.vt == VT_DISPATCH);
					spSynthRootCompAtom = varSynthItem;

					__CHECK_HRESULT3(spSynthRootCompAtom->get_UndID(&nSynthUndID));
					__CHECK_HRESULT3(spSynthRootCompAtom->get_Weight(&dSynthWeight));

					if (nSynthUndID != m_nID)
					{
						IMmRpUndAtomPtr spUndAtom;
						__CHECK_HRESULT3(sp_UndColl->get_Item(_bstr_t(nSynthUndID), &spUndAtom));
						if (spUndAtom != NULL)
						{
							DOUBLE	dSynthOptionsDelta = 0.;

							if (nRetCount && _finite(aGreeks.dDelta))
							{
								dSynthOptionsDelta = dOptPosInShares * dSynthWeight * aGreeks.dDelta;
							}

							__SynthTotalValuesMap::iterator iter = __SynthTotalValues.find(nSynthUndID);
							if (iter == __SynthTotalValues.end())
							{
								__MmRpSynthTotalValues tValues;
								tValues.m_nID = nSynthUndID;
								tValues.m_dOptionsDelta = dSynthOptionsDelta;								
								tValues.m_dPrice = d_Price;

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
							}
						}
					}
					else 
					{
						if (nRetCount && _finite(aGreeks.dDelta))
						{
							dOptionsDelta += dOptPosInShares * dSynthWeight * aGreeks.dDelta;
						}
					}
				} 
			}

			// check
			VARIANT_BOOL bCallEarlyExer = VARIANT_FALSE;
			VARIANT_BOOL bPutEarlyExer = VARIANT_FALSE;
			DOUBLE dPutPrice = 0. , dCallPrice = 0. , dInterest = 0. ;
			DOUBLE dDivAmt = 0.;
			
			if ( (enOptType == enOtCall && d_Price <= d_Strike) || (enOptType == enOtPut && d_Price >= d_Strike)  )//IS ATM
			{
				if (dtNow <= dtExpiryOV )
				{	
					DOUBLE	dYield = 0.;

					if (bIsRootSynthetic)
					{
						dYield = spSynthRootAtom->Yield;

						VARIANT_BOOL	bIsBasket = spSynthRootAtom->Basket;

						if(bIsBasket == VARIANT_TRUE)
						{	
							dYield  = 0;
							IEtsIndexDivCollPtr spBasketDivs;
							spSynthRootAtom->get_BasketDivs(&spBasketDivs);
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
						else
							nDivCount = 0;
					}
					else
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
								{
									nDivCount = 0;
									m_spDividend->GetDividendCount2(dtNow, dtExpiryOV, tmCloseTime, &nDivCount);
									if (nDivCount<= 0)
										nDivCount = 0;
									else
									{
										LPSAFEARRAY psaDates = NULL;
										LPSAFEARRAY psaAmounts = NULL;

										m_spDividend->GetDividends2(dtNow, dtExpiryOV, tmCloseTime, nDivCount, &psaAmounts, &psaDates, &nDivCount);

										saDates.Attach(psaDates);
										saAmounts.Attach(psaAmounts);
									}
								}
								else
									nDivCount = 0;

							}
							break;
						case enDivStockBasket:
							{
								VARIANT_BOOL bIsBasket = VARIANT_FALSE;
								if(m_spBasketIndex != NULL)
								{
									nDivCount = 0;
									IEtsIndexDivCollPtr spDivColl = NULL;
									m_spBasketIndex->get_BasketDivs(&spDivColl);
									_CHK(m_spBasketIndex->get_IsBasket(&bIsBasket));

									if (bIsBasket && spDivColl != NULL)
									{
										spDivColl->GetDividendCount2(dtNow, dtExpiryOV, tmCloseTime, &nDivCount);
										if(nDivCount > 0L)
										{
											LPSAFEARRAY psaDates = NULL;
											LPSAFEARRAY psaAmounts = NULL;

											spDivColl->GetDividends2(dtNow, dtExpiryOV, tmCloseTime, nDivCount, &psaAmounts, &psaDates, &nDivCount);

											saDates.Attach(psaDates);
											saAmounts.Attach(psaAmounts);
										}
									}
								}
							}
							break;
						case enDivIndexYield:
							{
								dYield = m_dYield;
							}
							break;	
						}
					}

					if(nDivCount <= 0.)
						nDivCount = 0;

					DOUBLE dRate = 0 ;
					if ( *psaRates ) 
					{
						dRate = _InterpolateRate(dYTE, *psaRates, *psaYTEs, ICalculationParametrsPtr(pParams));
					}
					DOUBLE dHTBRate = BAD_DOUBLE_VALUE;
					if ( *psaHTBRates )
					{
						dHTBRate = _InterpolateRate(dYTE, *psaHTBRates, *psaHTBYTEs, ICalculationParametrsPtr(pParams));
					}
					
					bool bDividentsAre = false ;
					bool bAreDividentsTillDays = ( EAUpComDivs == VARIANT_FALSE ) ;

					if (nDivCount || m_enUndType == enCtStock)
					{
						dDivAmt = 0 ;
						bool bUpComDivCalc = EAUpComDivs == VARIANT_TRUE ;
						for(INT i = 0; i < nDivCount; i++)
						{	
							DATE dt = floor ( saDates.GetPlainData()[i] * 365.0 ) + dtNow ;
							EgLib::vt_date dt1 ( dt ) ;

							if ( bUpComDivCalc )
							{
								EgLib::vt_date dt2 ( EgLib::vt_date::GetCurrentDate(true) ) ;

								vt_date_span dts = dt1 - dt2 ;
								long lL = dts.get_days() ;
								ATLASSERT ( lL >=0  ) ;
								if ( lL <= _EADaysToDiv )
								{
									bAreDividentsTillDays = true ;
								}
								
								if ( dtExpiryOV >= dt )
								{
								dDivAmt += saAmounts.GetPlainData()[i];
								bDividentsAre = true ;
								}
								
							}
							else	
							{
								if ( dtExpiryOV >= dt )
								{
								dDivAmt += saAmounts.GetPlainData()[i];
								bDividentsAre = true ;
							}
						}
					}
					}
					else
					{
						bDividentsAre = true ;
						dDivAmt = d_Price * dYield * (dtExpiryOV - dtNow) / 360.;
					}
					if(d_OptPrice <= 0. || d_PriceBid<=0.0 || d_PriceAsk<=0.0)
					{
						// Calc TheoPrice
						GREEKS	aGreeks;
						memset(&aGreeks, 0, sizeof(aGreeks));		
						aGreeks.nMask = GT_ALL;
						EtsOptionTypeEnum	enOptType = enOtCall ;
							
						nRetCount = _CalcOptionGreeks(	enCalcModel, 
							spOptAtom, 
							dRate, 
							dHTBRate,
							bUseTheoVolatility, 
							bUseTheoVolaNoBid, 
							bUseTheoVolaBadMarket, 
							sp_UndColl, 
							dUndPriceTolerance,
							enPriceRoundingRule, 
							aGreeks, 
							ICalculationParametrsPtr(pParams),
							0, 
							( d_Price < d_Strike && m_vbIsAmerican ) ? &enOptType : NULL, false );

						if (nRetCount > 0 && _finite(aGreeks.dTheoPrice) && !DoubleEQZero(aGreeks.dTheoPrice) )						
							d_OptPrice = m_spOptPriceProfile->GetOptPriceMid(d_PriceBid, d_PriceAsk, d_PriceLast, enPriceRoundingRule, bUseTheoVolatility, aGreeks.dTheoPrice, NULL );
					}

					
					
					//////////////////
					{
						if ( d_Price < d_Strike ) //( ( (enOptType == enOtCall && dPrice < d_Strike) || (enOptType == enOtPut && dPrice > d_Strike)  ) ) //(enOptType == enOtCall) 
						{	
							dPutPrice = d_OptPrice ;
							dCallPrice = -1 ;
							dInterest = (d_Strike * dRate * (dtExpiryOV - dtNow) / 360.) ;
						}
						else // put
						{
							dInterest = ( d_Strike * dRate * (dtExpiryOV - dtNow) / 360.) ;
							dPutPrice = -1 ;
							dCallPrice = d_OptPrice  ;
						}
					}
					if ( m_vbIsAmerican == VARIANT_TRUE && bAreDividentsTillDays )
					{						
						//if ( ( (enOptType == enOtCall && dPrice < d_Strike) || (enOptType == enOtPut && dPrice > d_Strike)  ) ) //(enOptType == enOtCall) 
						{
							DOUBLE dCoastOfCarry = dInterest - dDivAmt;
							bPutEarlyExer = dCoastOfCarry >= max ( 0, dPutPrice ) ? VARIANT_TRUE : VARIANT_FALSE;
						}
						//else // put
						{
							DOUBLE dCoastOfCarry = dDivAmt - dInterest;
							bCallEarlyExer = dCoastOfCarry >= max ( 0, dCallPrice ) ? VARIANT_TRUE : VARIANT_FALSE;
						}
					}
 //				Are Calc Early report
				}
			} 


			// end of check

			IMmRpPosWithEarlyExerciseOptAtomPtr spPosWithEarlyExOptAtom;

			__CHECK_HRESULT3(spPosWithEarlyExOptColl->get_Item(nOptRootID, d_Strike, dtExpiry, &spPosWithEarlyExOptAtom));
			if (spPosWithEarlyExOptAtom == 0)
			{
				__CHECK_HRESULT3(spPosWithEarlyExOptColl->Add(nOptRootID, d_Strike, dtExpiry, NULL, &spPosWithEarlyExOptAtom));

				__CHECK_HRESULT3(spPosWithEarlyExOptAtom->put_StockID(m_nID));
				__CHECK_HRESULT3(spPosWithEarlyExOptAtom->put_RootID(nOptRootID));
				__CHECK_HRESULT3(spPosWithEarlyExOptAtom->put_Strike(d_Strike));
				__CHECK_HRESULT3(spPosWithEarlyExOptAtom->put_Expiry(dtExpiry));

				__CHECK_HRESULT3(spPosWithEarlyExOptAtom->put_RootSymbol(_GetOptionRoot(bstrOptSymbol)));
				__CHECK_HRESULT3(spPosWithEarlyExOptAtom->put_Symbol(m_bstrSymbol));

				if (enOptType == enOtCall) 
				{
					if (!DoubleEQZero(dOptCallPos))
					{
						__CHECK_HRESULT3(spPosWithEarlyExOptAtom->put_CallPos(dOptCallPos));
					}

					__CHECK_HRESULT3(spPosWithEarlyExOptAtom->put_CallEarlyExercise(bCallEarlyExer));
				}
				else //put
				{
					if (!DoubleEQZero(dOptPutPos))
					{
						__CHECK_HRESULT3(spPosWithEarlyExOptAtom->put_PutPos(dOptPutPos));
					}

					__CHECK_HRESULT3(spPosWithEarlyExOptAtom->put_PutEarlyExercise(bPutEarlyExer));
				}
			}
			else //exists
			{
				if (enOptType == enOtCall) 
				{
					if (!DoubleEQZero(dOptCallPos))
					{
						DOUBLE dOptCallPosComp = 0.;

						__CHECK_HRESULT3(spPosWithEarlyExOptAtom->get_CallPos(&dOptCallPosComp));
						__CHECK_HRESULT3(spPosWithEarlyExOptAtom->put_CallPos(dOptCallPosComp + dOptCallPos));
					}

					__CHECK_HRESULT3(spPosWithEarlyExOptAtom->put_PutEarlyExercise(bPutEarlyExer));
				}
				else //put
				{
					if (!DoubleEQZero(dOptPutPos))
					{
						DOUBLE dOptPutPosComp = 0.;

						__CHECK_HRESULT3(spPosWithEarlyExOptAtom->get_PutPos(&dOptPutPosComp));
						__CHECK_HRESULT3(spPosWithEarlyExOptAtom->put_PutPos(dOptPutPosComp + dOptPutPos));
					}

					__CHECK_HRESULT3(spPosWithEarlyExOptAtom->put_CallEarlyExercise(bCallEarlyExer));
				}
			}

			if ( dDivAmt > 0 )
				__CHECK_HRESULT3 ( spPosWithEarlyExOptAtom->put_Dividents( dDivAmt ) ) ;
			if ( dInterest > 0 )
				__CHECK_HRESULT3(spPosWithEarlyExOptAtom->put_Interest ( dInterest ) ) ;
			
			if  ( dPutPrice > 0 || dPutPrice == -1 )
				__CHECK_HRESULT3(spPosWithEarlyExOptAtom->put_pOTM ( dPutPrice ) ) ;
			if ( dCallPrice > 0 || dCallPrice == -1 )
				__CHECK_HRESULT3(spPosWithEarlyExOptAtom->put_cOTM ( dCallPrice ) ) ;
		}	

		DOUBLE	dNetDelta = dStockPos + dOptionsDelta;

		CComBSTR	bstrNetDelta;
		_variant_t	varNetDelta((DOUBLE) dNetDelta ) ;

		__CHECK_HRESULT(::VarFormatNumber(&varNetDelta, -1, 0, 0, -1, 0, &bstrNetDelta), _T("Failed to format a number."));
		_bstr_t	bsNetDelta = _bstr_t((BSTR)bstrNetDelta) + L" " + (BSTR)m_bstrSymbol;

		__SynthTotalValuesMap::iterator iter = __SynthTotalValues.begin();
		while (iter != __SynthTotalValues.end())
		{
			__MmRpSynthTotalValues& tValues = iter->second;

			if (tValues.m_dOptionsDelta < 0.)
			{
				bsNetDelta += L" -";
			}
			else
			{
				bsNetDelta += L" +";
			}

			CComBSTR	bstrOptDelta;
			_variant_t	varOptDelta((DOUBLE)fabs(tValues.m_dOptionsDelta));

			__CHECK_HRESULT(::VarFormatNumber(&varOptDelta, -1, 0, 0, -1, 0, &bstrOptDelta), _T("Failed to format a number."));
			bsNetDelta += _bstr_t((BSTR)bstrOptDelta) + L" " + (BSTR)tValues.m_bstrSymbol;

			iter++;
		}

		bstrNetDelta.Empty();
		bstrNetDelta = bsNetDelta.copy();

		__CHECK_HRESULT3(spPosWithEarlyExAtom->put_Price(dSpotPrice));
		__CHECK_HRESULT3(spPosWithEarlyExAtom->put_PosInShares(dStockPos));
		__CHECK_HRESULT3(spPosWithEarlyExAtom->put_NetDelta(bstrNetDelta));

		*ppRetVal = spPosWithEarlyExAtom.Detach();

	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)CComErrorWrapper::ErrorDescription(e), IID_IMmRpUndAtom, e.Error());
	}

	return S_OK;
}

STDMETHODIMP CMmRpUndAtom::CalcMatrixByStock(DOUBLE dUndPriceTolerance, 
							EtsPriceRoundingRuleEnum enPriceRoundingRule,
							IMmRpMatrixByStockAtom* pMatrixByStockAtom,
							ICalculationParametrs* pParams,
							IMmRpMatrixByStockAtom** ppRetVal)
{
	__CHECK_POINTER(ppRetVal);

	try
	{
		if(*ppRetVal)
		{
			(*ppRetVal)->Release();
			*ppRetVal = 0;
		}

		DOUBLE dUndBid , dUndAsk , dUndLast ;
		m_spPrice->get_Bid ( &dUndBid) ;
		m_spPrice->get_Ask ( &dUndAsk ) ;
		m_spPrice->get_Last( &dUndLast ) ;

		HRESULT	hr;
		IMmRpMatrixByStockAtomPtr spMatrixByStockAtom(pMatrixByStockAtom);

		if (spMatrixByStockAtom == 0)
		{
			__CHECK_HRESULT(spMatrixByStockAtom.CreateInstance(CLSID_MmRpMatrixByStockAtom), 
				_T("Failed to create MmRpMatrixByStockAtom object."));

			__CHECK_HRESULT3(spMatrixByStockAtom->put_ID(m_nID));
			__CHECK_HRESULT3(spMatrixByStockAtom->put_Symbol(m_bstrSymbol));
			__CHECK_HRESULT3(spMatrixByStockAtom->put_UndType(m_enUndType));
		}

		DOUBLE dSpotPrice = m_spUndPriceProfile->GetUndPriceMid ( dUndBid, dUndAsk, 
				dUndLast, dUndPriceTolerance, enPriceRoundingRule, NULL, VARIANT_FALSE );
		
		__CHECK_HRESULT3(spMatrixByStockAtom->put_Price(dSpotPrice));

		IMmRpMatrixByStockOptCollPtr spMatrixByStockOptColl;
		__CHECK_HRESULT3(spMatrixByStockAtom->get_Opt(&spMatrixByStockOptColl));

		LONG				nOptID = 0L;
		LONG				nRootID = 0L;
		DOUBLE				dStrike = 0.;
		DATE				dtExpiry = 0.;
		EtsOptionTypeEnum	enOptType;

		DOUBLE				dQtyDailyPrevDateBuy = 0.;
		DOUBLE				dQtyDailyPrevDateSell = 0.;
		DOUBLE				dQtyDailyTodayBuy = 0.;
		DOUBLE				dQtyDailyTodaySell = 0.;

		IUnknownPtr			spUnk;
		_variant_t			varItem;
		ULONG				nFetched = 0L;
		IMmRpOptAtomPtr		spOptAtom;

		__CHECK_HRESULT(m_spOpt->get__NewEnum(&spUnk), _T("Failed to get options collection."));
		
		IEnumVARIANTPtr spOptEnum(spUnk);
		__CHECK_HRESULT(spOptEnum->Reset(), _T("Failed to reset options collection."));
		while((hr = spOptEnum->Next(1L, &varItem, &nFetched)) == S_OK)
		{
			ATLASSERT(varItem.vt == VT_DISPATCH);
			spOptAtom = varItem;

			CComBSTR		bstrSymbol;
			CComBSTR		bstrRootSymbol;
			
			IMmRpPosAtomPtr	spPosAtom;
			__CHECK_HRESULT3(spOptAtom->get_Pos(&spPosAtom));

			__CHECK_HRESULT3(spPosAtom->get_QtyDailyPrevDateBuy(&dQtyDailyPrevDateBuy));
			__CHECK_HRESULT3(spPosAtom->get_QtyDailyPrevDateSell(&dQtyDailyPrevDateSell));
			__CHECK_HRESULT3(spPosAtom->get_QtyDailyTodayBuy(&dQtyDailyTodayBuy));
			__CHECK_HRESULT3(spPosAtom->get_QtyDailyTodaySell(&dQtyDailyTodaySell));

			DOUBLE	dOptPos = dQtyDailyPrevDateBuy + dQtyDailyTodayBuy - dQtyDailyPrevDateSell - dQtyDailyTodaySell;

			__CHECK_HRESULT3(spOptAtom->get_ID(&nOptID));
			__CHECK_HRESULT3(spOptAtom->get_RootID(&nRootID));
			__CHECK_HRESULT3(spOptAtom->get_Strike(&dStrike));
			__CHECK_HRESULT3(spOptAtom->get_Expiry(&dtExpiry));			
			__CHECK_HRESULT3(spOptAtom->get_Symbol(&bstrSymbol));
			__CHECK_HRESULT3(spOptAtom->get_OptType(&enOptType));			

			dStrike = _AdjustFractionalDigits(dStrike, 4);

			IMmRpMatrixByStockOptAtomPtr spMatrixByStockOptAtom;
			__CHECK_HRESULT3(spMatrixByStockOptColl->get_Item(nOptID, &spMatrixByStockOptAtom));

			if (spMatrixByStockOptAtom == 0)
			{
				__CHECK_HRESULT3(spMatrixByStockOptColl->Add(nOptID, NULL, &spMatrixByStockOptAtom));

				__CHECK_HRESULT3(spMatrixByStockOptAtom->put_StockID(m_nID));
				__CHECK_HRESULT3(spMatrixByStockOptAtom->put_ID(nOptID));
				__CHECK_HRESULT3(spMatrixByStockOptAtom->put_RootID(nRootID));
				__CHECK_HRESULT3(spMatrixByStockOptAtom->put_Symbol(bstrSymbol));
				__CHECK_HRESULT3(spMatrixByStockOptAtom->put_OptType(enOptType));
				__CHECK_HRESULT3(spMatrixByStockOptAtom->put_Strike(dStrike));
				__CHECK_HRESULT3(spMatrixByStockOptAtom->put_Expiry(dtExpiry));
			}

			__CHECK_HRESULT3(spMatrixByStockOptAtom->put_Pos(dOptPos));
			__CHECK_HRESULT3(spMatrixByStockOptAtom->put_RootSymbol(_GetOptionRoot(bstrSymbol)));
		}

		*ppRetVal = spMatrixByStockAtom.Detach();
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)CComErrorWrapper::ErrorDescription(e), IID_IMmRpUndAtom, e.Error());
	}

	return S_OK;
}

STDMETHODIMP CMmRpUndAtom::CalcSynthetics(EtsCalcModelTypeEnum enCalcModel,
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
							IMmRpSyntheticColl* pSyntheticColl,
							ICalculationParametrs* pParams,
							IMmRpSyntheticColl** ppRetVal)
{
	if(!pUndColl || pParams == NULL)
		return E_INVALIDARG;

	__CHECK_POINTER(ppRetVal);

	try
	{
		if(*ppRetVal)
		{
			(*ppRetVal)->Release();
			*ppRetVal = 0;
		}

		DOUBLE dUndBid , dUndAsk , dUndLast ;
		m_spPrice->get_Bid ( &dUndBid) ;
		m_spPrice->get_Ask ( &dUndAsk ) ;
		m_spPrice->get_Last( &dUndLast ) ;

		HRESULT	hr;
		IMmRpUndCollPtr				spUndColl(pUndColl);
		IMmRpSyntheticCollPtr		spSyntheticColl = pSyntheticColl;
		IMmRpSyntheticAtomPtr		spSyntheticAtom;		

		if (spSyntheticColl == 0)
			__CHECK_HRESULT(spSyntheticColl.CreateInstance(CLSID_MmRpSyntheticColl), _T("Failed to create MmRpSyntheticColl object."));
		
		__CHECK_HRESULT3(spSyntheticColl->get_Item(m_nID, m_nID, &spSyntheticAtom));

		if (spSyntheticAtom == 0)
		{
			__CHECK_HRESULT3(spSyntheticColl->Add(m_nID, m_nID, m_bstrSymbol, NULL, &spSyntheticAtom));

			__CHECK_HRESULT3(spSyntheticAtom->put_ID(m_nID));
			__CHECK_HRESULT3(spSyntheticAtom->put_Symbol(m_bstrSymbol));
			__CHECK_HRESULT3(spSyntheticAtom->put_UndType(m_enUndType));
		}

		ATLASSERT(spSyntheticAtom!=NULL);

		DOUBLE	dSpotPrice;
		VARIANT_BOOL	futureUsed = VARIANT_FALSE;
		__CHECK_HRESULT3(GetUnderlyingPrice( dUndPriceTolerance, enPriceRoundingRule, NULL, &futureUsed, &dSpotPrice ) );

		DOUBLE	dPosDailyPrevDateBuy = 0.;
		DOUBLE	dPosDailyPrevDateSell = 0.;
		DOUBLE	dPosDailyTodayBuy = 0.;
		DOUBLE	dPosDailyTodaySell = 0.;

		DOUBLE	dQtyDailyPrevDateBuy = 0.;
		DOUBLE	dQtyDailyPrevDateSell = 0.;
		DOUBLE	dQtyDailyTodayBuy = 0.;
		DOUBLE	dQtyDailyTodaySell = 0.;

		DOUBLE	dQtyInSharesDailyPrevDateBuy = 0.;
		DOUBLE	dQtyInSharesDailyPrevDateSell = 0.;
		DOUBLE	dQtyInSharesDailyTodayBuy = 0.;
		DOUBLE	dQtyInSharesDailyTodaySell = 0.;

		__CHECK_HRESULT3(m_spPos->get_QtyDailyPrevDateBuy(&dQtyDailyPrevDateBuy));
		__CHECK_HRESULT3(m_spPos->get_QtyDailyPrevDateSell(&dQtyDailyPrevDateSell));
		__CHECK_HRESULT3(m_spPos->get_QtyDailyTodayBuy(&dQtyDailyTodayBuy));
		__CHECK_HRESULT3(m_spPos->get_QtyDailyTodaySell(&dQtyDailyTodaySell));

		DOUBLE	dFutPos = 0.;
		{ /***********************  collect futures position data *****************************/
			IUnknownPtr		spFutUnk;
			_variant_t		varItem;
			ULONG			nFetched = 0L;	
			IMmRpFutAtomPtr	spFutAtom;

			DOUBLE	dFutQtyDailyPrevDateBuy = 0.;
			DOUBLE	dFutQtyDailyPrevDateSell = 0.;
			DOUBLE	dFutQtyDailyTodayBuy = 0.;
			DOUBLE	dFutQtyDailyTodaySell = 0.;


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
				dFutPos += (dFutQtyDailyPrevDateBuy + dFutQtyDailyTodayBuy - dFutQtyDailyPrevDateSell - dFutQtyDailyTodaySell);		
			}
			/***********************************************************************************/}

		DOUBLE	dStockPos = dFutPos + dQtyDailyPrevDateBuy + dQtyDailyTodayBuy - dQtyDailyPrevDateSell - dQtyDailyTodaySell;

		DOUBLE	dPriceBid = 0.;
		DOUBLE	dPriceAsk = 0.;
		DOUBLE	dPriceLast = 0.;
		DOUBLE	dPrice = 0.;
		
		LONG	nOptID = 0L;
		LONG	nOptRootID = 0L;

		EtsOptionTypeEnum	enOptType;

		DOUBLE	dStrike = 0.;
		DATE	dtExpiry = 0., dtExpiryOV = 0., tmCloseTime = 0.;		
		DOUBLE	dNetDelta = 0.;
        DOUBLE	dSyntheticStock = 0.;
        DOUBLE	dNetStock = 0.;
		
		//DATE	nToday = (DATE)COleDateTime::GetCurrentTime();
		DATE dtNow = 0.0;
		DOUBLE	dYTE = 0.;
		GetNYDateTimeAsDATE(&dtNow);
		
		__SynthTotalValuesMap	__SynthTotalValues;
		__SynthTotalValuesMap	__SynthTotalValuesAll;

		GREEKS	aGreeks;
		memset(&aGreeks, 0, sizeof(aGreeks));		

		//bool		bATMFlag = false;		

		IUnknownPtr		spUnk;
		_variant_t		varItem;
		ULONG			nFetched = 0L;
		IMmRpOptAtomPtr	spOptAtom;

		__CHECK_HRESULT(m_spOpt->get__NewEnum(&spUnk), _T("Failed to get options collection."));
		
		IEnumVARIANTPtr spOptEnum(spUnk);
		__CHECK_HRESULT(spOptEnum->Reset(), _T("Failed to reset options collection."));
		while((hr = spOptEnum->Next(1L, &varItem, &nFetched)) == S_OK)
		{
			ATLASSERT(varItem.vt == VT_DISPATCH);
			spOptAtom = varItem;

			IMmRpPosAtomPtr	spOptPosAtom;
			__CHECK_HRESULT3(spOptAtom->get_Pos(&spOptPosAtom));

			__CHECK_HRESULT3(spOptPosAtom->get_QtyDailyPrevDateBuy(&dQtyDailyPrevDateBuy));
			__CHECK_HRESULT3(spOptPosAtom->get_QtyDailyPrevDateSell(&dQtyDailyPrevDateSell));
			__CHECK_HRESULT3(spOptPosAtom->get_QtyDailyTodayBuy(&dQtyDailyTodayBuy));
			__CHECK_HRESULT3(spOptPosAtom->get_QtyDailyTodaySell(&dQtyDailyTodaySell));

			__CHECK_HRESULT3(spOptPosAtom->get_QtyInSharesDailyPrevDateBuy(&dQtyInSharesDailyPrevDateBuy));
			__CHECK_HRESULT3(spOptPosAtom->get_QtyInSharesDailyPrevDateSell(&dQtyInSharesDailyPrevDateSell));
			__CHECK_HRESULT3(spOptPosAtom->get_QtyInSharesDailyTodayBuy(&dQtyInSharesDailyTodayBuy));
			__CHECK_HRESULT3(spOptPosAtom->get_QtyInSharesDailyTodaySell(&dQtyInSharesDailyTodaySell));

			DOUBLE dOptPos = dQtyDailyPrevDateBuy + dQtyDailyTodayBuy - dQtyDailyPrevDateSell - dQtyDailyTodaySell;
			DOUBLE dOptPosInShares = dQtyInSharesDailyPrevDateBuy + dQtyInSharesDailyTodayBuy - 
				dQtyInSharesDailyPrevDateSell - dQtyInSharesDailyTodaySell;

			CComBSTR bstrOptSymbol;
			__CHECK_HRESULT3(spOptAtom->get_Symbol(&bstrOptSymbol));

			__CHECK_HRESULT3(spOptAtom->get_ID(&nOptID));
			__CHECK_HRESULT3(spOptAtom->get_RootID(&nOptRootID));
			__CHECK_HRESULT3(spOptAtom->get_Strike(&dStrike));
			__CHECK_HRESULT3(spOptAtom->get_Expiry(&dtExpiry));
			__CHECK_HRESULT3(spOptAtom->get_ExpiryOV(&dtExpiryOV));
			__CHECK_HRESULT3(spOptAtom->get_TradingClose(&tmCloseTime));
			__CHECK_HRESULT3(spOptAtom->get_OptType(&enOptType));

			::GetCalculationParams(dtNow, dtExpiryOV, tmCloseTime, pParams->UseTimePrecision != VARIANT_FALSE, &dtNow, &dtExpiryOV, &tmCloseTime, &dYTE);

			dStrike = _AdjustFractionalDigits(dStrike, 4);
			
			DOUBLE		dPositionN = dOptPos;
			DOUBLE		dNetPosition = dOptPosInShares;

			DOUBLE		dOptionsDelta = 0.;
			DOUBLE		dOTMDelta = 0.;
			DOUBLE		dTotalGamma = 0.;
			DOUBLE		dTotalVega = 0.;
			DOUBLE		dTotalTheta = 0.;

			__SynthTotalValues.clear();

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
			DOUBLE					dCashValue = 0L;
			LONG					nDivCount = 0;

			
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

			if (bIsRootSynthetic)
			{
				//_GetSyntheticPrice(spSynthRootAtom, spUndColl, dPriceBid, dPriceAsk, dPriceLast);
				dPrice = m_spUndPriceProfile->GetUndPriceMid(dPriceBid, dPriceAsk, dPriceLast, dUndPriceTolerance, enPriceRoundingRule, NULL, VARIANT_FALSE );
			}
			else
			{
				dPrice = dSpotPrice;
			}

			LONG nRetCount = 0L;

			if (dtNow <= dtExpiryOV && !DoubleEQZero(dNetPosition))
			{
				memset(&aGreeks, 0, sizeof(aGreeks));
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
												ICalculationParametrsPtr(pParams));
			}

			if (!bIsRootSynthetic)
			{
				if (nRetCount)
				{
					if ( (enOptType == enOtCall && (dStrike - dPrice > 0. || DoubleEQZero(dStrike - dPrice))) ||
						(enOptType == enOtPut && (dStrike - dPrice < 0. || DoubleEQZero(dStrike - dPrice))) )
					{
						if (_finite(aGreeks.dDelta))
						{
							dOTMDelta = aGreeks.dDelta * dNetPosition;
						}
					}

					if (_finite(aGreeks.dDelta))
					{
						dOptionsDelta = aGreeks.dDelta * dNetPosition;
					}

					if (_finite(aGreeks.dGamma))
					{
						dTotalGamma = aGreeks.dGamma * dNetPosition;
					}
				}
			}
			else // synthetic
			{
				if (dCashValue > 0.)
				{
					DOUBLE dSynthOptionsDelta = 0.;
					DOUBLE dSynthTotalGamma = 0.;
					DOUBLE dSynthOTMDelta = 0.;

					if (nRetCount)
					{
						if (_finite(aGreeks.dDelta))
						{
							if ( (enOptType == enOtCall && (dStrike - dPrice > 0. || DoubleEQZero(dStrike - dPrice))) ||
								(enOptType == enOtPut && (dStrike - dPrice < 0. || DoubleEQZero(dStrike - dPrice))) )
							{
								dSynthOTMDelta = aGreeks.dDelta * dNetPosition *dCashValue;
							}
							
							dSynthOptionsDelta =  aGreeks.dDelta * dNetPosition *dCashValue;
						}

						if (_finite(aGreeks.dGamma))
						{
							dSynthTotalGamma = aGreeks.dGamma * dNetPosition *dCashValue;
						}
					}

					__MmRpSynthTotalValues tValues;
					tValues.m_nID = __USD_ID__;
					tValues.m_dPrice = __USD_PRICE__;

					tValues.m_dOptionsDelta = dSynthOptionsDelta;
					tValues.m_dTotalGamma = dSynthTotalGamma;
					tValues.m_dOTMDelta = dSynthOTMDelta;

					tValues.m_bstrSymbol.Append(m_bstrSymbol);
					tValues.m_bstrSymbol.Append(L"(");
					tValues.m_bstrSymbol.Append(__USD_SYMBOL__);
					tValues.m_bstrSymbol.Append(L"*)");

					__SynthTotalValues.insert(std::make_pair(__USD_ID__, tValues));
				}

				__CHECK_HRESULT(spSynthEnum->Reset(), _T("Fail to reset synthetic roots components collection."));
				while((hr = spSynthEnum->Next(1L, &varSynthItem, &nSynthFetched)) == S_OK)
				{
					ATLASSERT(varSynthItem.vt == VT_DISPATCH);
					spSynthRootCompAtom = varSynthItem;

					__CHECK_HRESULT3(spSynthRootCompAtom->get_UndID(&nSynthUndID));
					__CHECK_HRESULT3(spSynthRootCompAtom->get_Weight(&dSynthWeight));

					if (nSynthUndID != m_nID)
					{
						IMmRpUndAtomPtr spUndAtom;
						__CHECK_HRESULT3(spUndColl->get_Item(_bstr_t(nSynthUndID), &spUndAtom));
						if (spUndAtom != NULL)
						{

							IMMRpPricePtr spPrice ;
							spUndAtom->get_Price ( &spPrice ) ;
							ATLASSERT ( spPrice ) ;

							__CHECK_HRESULT3(spPrice->get_Bid(&dPriceBid));
							__CHECK_HRESULT3(spPrice->get_Ask(&dPriceAsk));
							__CHECK_HRESULT3(spPrice->get_Last(&dPriceLast));

							dPrice = m_spUndPriceProfile->GetUndPriceMid(dPriceBid, dPriceAsk, dPriceLast, dUndPriceTolerance, enPriceRoundingRule, NULL, VARIANT_FALSE );
									
							DOUBLE dSynthOptionsDelta = 0.;
							DOUBLE dSynthTotalGamma = 0.;
							DOUBLE dSynthOTMDelta = 0.;

							if (nRetCount)
							{
								if (_finite(aGreeks.dDelta))
								{
									if ( (enOptType == enOtCall && (dStrike - dPrice > 0. || DoubleEQZero(dStrike - dPrice))) ||
										(enOptType == enOtPut && (dStrike - dPrice < 0. || DoubleEQZero(dStrike - dPrice))) )
									{
										dSynthOTMDelta = aGreeks.dDelta * dNetPosition *dSynthWeight;
									}
							
									dSynthOptionsDelta =  aGreeks.dDelta * dNetPosition *dSynthWeight;
								}

								if (_finite(aGreeks.dGamma))
								{
									dSynthTotalGamma = aGreeks.dGamma * dNetPosition *dSynthWeight;
								}
							}
							
							__MmRpSynthTotalValues tValues;
							tValues.m_nID = nSynthUndID;
							tValues.m_dOptionsDelta = dSynthOptionsDelta;
							tValues.m_dTotalGamma = dSynthTotalGamma;
							tValues.m_dOTMDelta = dSynthOTMDelta;
							tValues.m_dPrice = dPrice;
							
							CComBSTR bstrSynthCompSymbol;

							__CHECK_HRESULT3(spUndAtom->get_Symbol(&bstrSynthCompSymbol));
							__CHECK_HRESULT3(spUndAtom->get_UndType(&tValues.m_enUndType));
								
							tValues.m_bstrSymbol.Append(m_bstrSymbol);
							tValues.m_bstrSymbol.Append(L"(");
							tValues.m_bstrSymbol.Append(bstrSynthCompSymbol);
							tValues.m_bstrSymbol.Append(L"*)");

							__SynthTotalValues.insert(std::make_pair(nSynthUndID, tValues));
						}
					}
					else
					{
						if (nRetCount)
						{
							if (_finite(aGreeks.dDelta))
							{
								if ( (enOptType == enOtCall && (dStrike - dPrice > 0. || DoubleEQZero(dStrike - dPrice))) ||
									(enOptType == enOtPut && (dStrike - dPrice < 0. || DoubleEQZero(dStrike - dPrice))) )
								{
									dOTMDelta += aGreeks.dDelta * dNetPosition * dSynthWeight;
								}

								dOptionsDelta += aGreeks.dDelta * dNetPosition * dSynthWeight;
							}

							if (_finite(aGreeks.dGamma))
							{
								dTotalGamma += aGreeks.dGamma * dNetPosition * dSynthWeight;
							}
						}
					}
				} 
			}

			if (nRetCount)
			{
				if (_finite(aGreeks.dVega))
				{
					dTotalVega = aGreeks.dVega * dNetPosition;
				}

				if (_finite(aGreeks.dTheta))
				{
					dTotalTheta = aGreeks.dTheta * dNetPosition;
				}
			}
		
			/*if (!bATMFlag)
			{	
				DOUBLE	dEqStrike = _AdjustFractionalDigits(dSpotPrice,4) + .0001;
				_bstr_t	bsKey = (_bstr_t)_variant_t(m_nID) + 
								L"_" + 
								(_bstr_t)_variant_t((LONG)dtExpiry) + 
								L"_" + 
								(_bstr_t)_variant_t(dEqStrike);

				__CHECK_HRESULT3(spSyntheticOptColl->Add(bsKey, NULL, &spSyntheticOptAtom));

				__CHECK_HRESULT3(spSyntheticOptAtom->put_StockID(m_nID));
				__CHECK_HRESULT3(spSyntheticOptAtom->put_RootSymbol(CComBSTR(L"")));
				__CHECK_HRESULT3(spSyntheticOptAtom->put_Expiry(dtExpiry));
				__CHECK_HRESULT3(spSyntheticOptAtom->put_Strike(dEqStrike));
				__CHECK_HRESULT3(spSyntheticOptAtom->put_ATMMark(1.));
				
				bATMFlag = true;
			}*/			
			
			DOUBLE	dCompStrike = 0.;
			DATE	dtCompExpiry = 0.;
			LONG	nCompRootID = 0L;
			DOUBLE	dCompNetPosition = 0.;
			DOUBLE	dCompOptionsDelta = 0.;
			DOUBLE	dCompTotalGamma = 0.;
			DOUBLE	dCompTotalVega = 0.;
			DOUBLE	dCompTotalTheta = 0.;
			DOUBLE	dCompOTMDelta = 0.;
			
			IMmRpSyntheticOptCollPtr spSyntheticOptColl;
			IMmRpSyntheticOptAtomPtr spSyntheticOptAtom;

			spSyntheticAtom = 0;
			__CHECK_HRESULT3(spSyntheticColl->get_Item(m_nID, m_nID, &spSyntheticAtom));
			ATLASSERT(spSyntheticAtom!=NULL);

			__CHECK_HRESULT3(spSyntheticAtom->get_Opt(&spSyntheticOptColl));
			ATLASSERT(spSyntheticOptColl!=NULL);

			__CHECK_HRESULT3(spSyntheticOptColl->get_Item(nOptID, &spSyntheticOptAtom));
			if (spSyntheticOptAtom == 0)
			{
				__CHECK_HRESULT3(spSyntheticOptColl->Add(nOptID, NULL, &spSyntheticOptAtom));

				__CHECK_HRESULT3(spSyntheticOptAtom->put_StockID(m_nID));
				
				__CHECK_HRESULT3(spSyntheticOptAtom->put_ID(nOptID));
				__CHECK_HRESULT3(spSyntheticOptAtom->put_RootSymbol(_GetOptionRoot(bstrOptSymbol)));
				__CHECK_HRESULT3(spSyntheticOptAtom->put_RootID(nOptRootID));
				__CHECK_HRESULT3(spSyntheticOptAtom->put_Expiry(dtExpiry));
				__CHECK_HRESULT3(spSyntheticOptAtom->put_Strike(dStrike));
				__CHECK_HRESULT3(spSyntheticOptAtom->put_NetPos(dPositionN));
				__CHECK_HRESULT3(spSyntheticOptAtom->put_Delta(dOptionsDelta));
				__CHECK_HRESULT3(spSyntheticOptAtom->put_Gamma(dTotalGamma));
				__CHECK_HRESULT3(spSyntheticOptAtom->put_OTMDelta(dOTMDelta));
				__CHECK_HRESULT3(spSyntheticOptAtom->put_Vega(dTotalVega));
				__CHECK_HRESULT3(spSyntheticOptAtom->put_Theta(dTotalTheta));
			}
			else
			{	
				__CHECK_HRESULT3(spSyntheticOptAtom->get_Strike(&dCompStrike));
				__CHECK_HRESULT3(spSyntheticOptAtom->get_Expiry(&dtCompExpiry));
				__CHECK_HRESULT3(spSyntheticOptAtom->get_RootID(&nCompRootID));				

				dCompStrike = _AdjustFractionalDigits(dCompStrike,4);

				if ( DoubleEQZero(dStrike-dCompStrike) && (LONG)dtExpiryOV == (LONG)dtCompExpiry && nOptRootID == nCompRootID )
				{
					__CHECK_HRESULT3(spSyntheticOptAtom->get_NetPos(&dCompNetPosition));
					__CHECK_HRESULT3(spSyntheticOptAtom->put_NetPos(dCompNetPosition + dPositionN));

					__CHECK_HRESULT3(spSyntheticOptAtom->get_Delta(&dCompOptionsDelta));
					__CHECK_HRESULT3(spSyntheticOptAtom->put_Delta(dCompOptionsDelta + dOptionsDelta));

					__CHECK_HRESULT3(spSyntheticOptAtom->get_Gamma(&dCompTotalGamma));
					__CHECK_HRESULT3(spSyntheticOptAtom->put_Gamma(dCompTotalGamma + dTotalGamma));

					__CHECK_HRESULT3(spSyntheticOptAtom->get_OTMDelta(&dCompOTMDelta));
					__CHECK_HRESULT3(spSyntheticOptAtom->put_OTMDelta(dCompOTMDelta + dOTMDelta));

					__CHECK_HRESULT3(spSyntheticOptAtom->get_Vega(&dCompTotalVega));
					__CHECK_HRESULT3(spSyntheticOptAtom->put_Vega(dCompTotalVega + dTotalVega));

					__CHECK_HRESULT3(spSyntheticOptAtom->get_Theta(&dCompTotalTheta));
					__CHECK_HRESULT3(spSyntheticOptAtom->put_Theta(dCompTotalTheta + dTotalTheta));
				}
			}

			dNetDelta += dOptionsDelta;
            dSyntheticStock += dOptionsDelta - dOTMDelta;

			__SynthTotalValuesMap::iterator iter = __SynthTotalValues.begin();
			while (iter != __SynthTotalValues.end())
			{
				__MmRpSynthTotalValues& tValues = iter->second;

				spSyntheticAtom = 0;
				__CHECK_HRESULT3(spSyntheticColl->get_Item(m_nID, tValues.m_nID, &spSyntheticAtom));
				if (spSyntheticAtom == 0)
				{
					__CHECK_HRESULT3(spSyntheticColl->Add(m_nID, tValues.m_nID, tValues.m_bstrSymbol, NULL, &spSyntheticAtom));

					__CHECK_HRESULT3(spSyntheticAtom->put_ID(tValues.m_nID));
					__CHECK_HRESULT3(spSyntheticAtom->put_Symbol(tValues.m_bstrSymbol));
					__CHECK_HRESULT3(spSyntheticAtom->put_UndType(tValues.m_enUndType));

					_CopyOptions(spSyntheticAtom, spSyntheticColl, nOptRootID);
				}

				spSyntheticOptColl = 0;
				__CHECK_HRESULT3(spSyntheticAtom->get_Opt(&spSyntheticOptColl));
				ATLASSERT(spSyntheticOptColl!=0);

				spSyntheticOptAtom = 0;
				__CHECK_HRESULT3(spSyntheticOptColl->get_Item(nOptID, &spSyntheticOptAtom));
				if (spSyntheticOptAtom == 0)
				{
					__CHECK_HRESULT3(spSyntheticOptColl->Add(nOptID, NULL, &spSyntheticOptAtom));

					__CHECK_HRESULT3(spSyntheticOptAtom->put_StockID(tValues.m_nID));

					__CHECK_HRESULT3(spSyntheticOptAtom->put_ID(nOptID));
					__CHECK_HRESULT3(spSyntheticOptAtom->put_RootSymbol(_GetOptionRoot(bstrOptSymbol)));
					__CHECK_HRESULT3(spSyntheticOptAtom->put_RootID(nOptRootID));
					__CHECK_HRESULT3(spSyntheticOptAtom->put_Expiry(dtExpiry));
					__CHECK_HRESULT3(spSyntheticOptAtom->put_Strike(dStrike));
					//__CHECK_HRESULT3(spSyntheticOptAtom->put_ATMMark(0.));
					__CHECK_HRESULT3(spSyntheticOptAtom->put_NetPos(dPositionN));
					__CHECK_HRESULT3(spSyntheticOptAtom->put_Delta(tValues.m_dOptionsDelta));
					__CHECK_HRESULT3(spSyntheticOptAtom->put_Gamma(tValues.m_dTotalGamma));
					__CHECK_HRESULT3(spSyntheticOptAtom->put_OTMDelta(tValues.m_dOTMDelta));
				}
				else 
				{
					__CHECK_HRESULT3(spSyntheticOptAtom->get_Strike(&dCompStrike));
					__CHECK_HRESULT3(spSyntheticOptAtom->get_Expiry(&dtCompExpiry));
					__CHECK_HRESULT3(spSyntheticOptAtom->get_RootID(&nCompRootID));	

					dCompStrike = _AdjustFractionalDigits(dCompStrike,4);

					if ( DoubleEQZero(dStrike-dCompStrike) && (LONG)dtExpiryOV == (LONG)dtCompExpiry && nOptRootID == nCompRootID )
					{
						__CHECK_HRESULT3(spSyntheticOptAtom->get_NetPos(&dCompNetPosition));
						__CHECK_HRESULT3(spSyntheticOptAtom->put_NetPos(dCompNetPosition + dPositionN));

						__CHECK_HRESULT3(spSyntheticOptAtom->get_Delta(&dCompOptionsDelta));
						__CHECK_HRESULT3(spSyntheticOptAtom->put_Delta(dCompOptionsDelta + tValues.m_dOptionsDelta));
						__CHECK_HRESULT3(spSyntheticOptAtom->get_Gamma(&dCompTotalGamma));
						__CHECK_HRESULT3(spSyntheticOptAtom->put_Gamma(dCompTotalGamma + tValues.m_dTotalGamma));

						__CHECK_HRESULT3(spSyntheticOptAtom->get_OTMDelta(&dCompOTMDelta));
						__CHECK_HRESULT3(spSyntheticOptAtom->put_OTMDelta(dCompOTMDelta + tValues.m_dOTMDelta));
					}
				}

				__SynthTotalValuesMap::iterator iter_all = __SynthTotalValuesAll.find(tValues.m_nID);
				if (iter_all == __SynthTotalValuesAll.end())
				{
					__SynthTotalValuesAll.insert(std::make_pair(tValues.m_nID, tValues));
				}
				else
				{
					__MmRpSynthTotalValues& tValuesAll = iter_all->second;
					tValuesAll.m_dOptionsDelta += tValues.m_dOptionsDelta;
					tValuesAll.m_dOTMDelta += tValues.m_dOTMDelta;
				}

				iter++;
			} 

		} 

		dNetDelta = dStockPos + dNetDelta;
        dNetStock = dStockPos + dSyntheticStock;

		spSyntheticAtom = 0;
		__CHECK_HRESULT3(spSyntheticColl->get_Item(m_nID, m_nID, &spSyntheticAtom));
		ATLASSERT(spSyntheticAtom!=NULL);

		__CHECK_HRESULT3(spSyntheticAtom->put_Net(dNetStock));
		__CHECK_HRESULT3(spSyntheticAtom->put_NetDelta(dNetDelta));
		__CHECK_HRESULT3(spSyntheticAtom->put_Price(dSpotPrice));
		__CHECK_HRESULT3(spSyntheticAtom->put_Pos(dStockPos));
		__CHECK_HRESULT3(spSyntheticAtom->put_Synthetic(dSyntheticStock));

		__SynthTotalValuesMap::iterator iter_all = __SynthTotalValuesAll.begin();
		while (iter_all != __SynthTotalValuesAll.end())
		{
			__MmRpSynthTotalValues& tValuesAll = iter_all->second;
			
			spSyntheticAtom = 0;
			__CHECK_HRESULT3(spSyntheticColl->get_Item(m_nID, tValuesAll.m_nID, &spSyntheticAtom));
			if (spSyntheticAtom == 0)
			{
				__CHECK_HRESULT3(spSyntheticColl->Add(m_nID, tValuesAll.m_nID, tValuesAll.m_bstrSymbol, NULL, &spSyntheticAtom));

				__CHECK_HRESULT3(spSyntheticAtom->put_ID(tValuesAll.m_nID));
				__CHECK_HRESULT3(spSyntheticAtom->put_Symbol(tValuesAll.m_bstrSymbol));
				__CHECK_HRESULT3(spSyntheticAtom->put_UndType(tValuesAll.m_enUndType));
			}

			__CHECK_HRESULT3(spSyntheticAtom->put_Price(tValuesAll.m_dPrice));
			__CHECK_HRESULT3(spSyntheticAtom->put_NetDelta(tValuesAll.m_dOptionsDelta));
			__CHECK_HRESULT3(spSyntheticAtom->put_Synthetic(tValuesAll.m_dOptionsDelta - tValuesAll.m_dOTMDelta));
			__CHECK_HRESULT3(spSyntheticAtom->put_Net(tValuesAll.m_dOptionsDelta - tValuesAll.m_dOTMDelta));

			iter_all++;
		}

		*ppRetVal = spSyntheticColl.Detach();
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)CComErrorWrapper::ErrorDescription(e), IID_IMmRpUndAtom, e.Error());
	}

	return S_OK;
}

void CMmRpUndAtom::_CopyOptions(IMmRpSyntheticAtom* pSyntheticAtom, IMmRpSyntheticColl* pSyntheticColl, LONG nRootID)
{
	IMmRpSyntheticAtomPtr spSyntheticAtom = pSyntheticAtom;
	IMmRpSyntheticCollPtr spSyntheticColl = pSyntheticColl;
	IMmRpSyntheticAtomPtr spMainSyntheticAtom;
	IMmRpSyntheticOptCollPtr spMainSyntheticOptColl;
	IMmRpSyntheticOptCollPtr spSyntheticOptColl;
	
	__CHECK_HRESULT3(spSyntheticColl->get_Item(m_nID, m_nID, &spMainSyntheticAtom));
	if (spMainSyntheticAtom == 0) return;

	__CHECK_HRESULT3(spMainSyntheticAtom->get_Opt(&spMainSyntheticOptColl));
	ATLASSERT(spMainSyntheticOptColl != 0);
	__CHECK_HRESULT3(spSyntheticAtom->get_Opt(&spSyntheticOptColl));
	ATLASSERT(spSyntheticOptColl != 0);

	__CHECK_HRESULT3(spSyntheticOptColl->Append(spMainSyntheticOptColl, nRootID));
}

void CMmRpUndAtom::_FormatSeries(DATE dtExpiry, CComBSTR& bstrSeries) throw()
{
	try
	{
		COleDateTime dt(dtExpiry);
		CString strDate = dt.Format(_T("%b%y"));
		strDate.MakeUpper();
		bstrSeries.Attach(strDate.AllocSysString());
	}
	catch(...)
	{
		CComErrorWrapper::ThrowError(E_FAIL, _T("Failed to format series."));
	}
}

DOUBLE	CMmRpUndAtom::_AdjustFractionalDigits(DOUBLE dValue, UINT uDigits)
{
    DOUBLE	dPower = pow(10., (int)uDigits);
	DOUBLE	dNewVal = dValue * dPower + .5;    
    dNewVal = floor(dNewVal);
    return dNewVal/dPower;
}

DOUBLE	CMmRpUndAtom::_InterpolateRate(DOUBLE dYTE,
									   SAFEARRAY* psaRates,
									   SAFEARRAY* psaYTEs,
									   ICalculationParametrsPtr spParams) throw()
{
	if (psaRates == NULL || psaYTEs == NULL || !(bool)spParams)
	{
		ATLASSERT ( !"(psaRates == NULL || psaDTEs == NULL)" ) ;
		CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Invalid array pointer."));
	}

	__CHECK_HRESULT(::SafeArrayLock(psaRates), _T("Unable to lock rates array."));
	__CHECK_HRESULT(::SafeArrayLock(psaYTEs), _T("Unable to lock YTEs array."));

	if (::SafeArrayGetDim(psaRates) != 1 || ::SafeArrayGetDim(psaYTEs) != 1)
		CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Invalid array dimension."));

#ifdef _DEBUG
	VARTYPE vType;
	::SafeArrayGetVartype(psaRates, &vType);
	ATLASSERT(vType == VT_R8);
	::SafeArrayGetVartype(psaYTEs, &vType);
	ATLASSERT(vType == VT_R8);
#endif
	
	if (psaRates->rgsabound[0].cElements ==0 || psaYTEs->rgsabound[0].cElements == 0)
		CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Array has zero length."));

	if (psaRates->rgsabound[0].cElements != psaYTEs->rgsabound[0].cElements)
		CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Arrays should have equal lengths."));
	
	if (dYTE < 0) dYTE = 0;

	dYTE = ceil(dYTE * 365.) / 365.;

	DOUBLE	dRate = ::InterpolateRates2(psaRates->rgsabound[0].cElements, (DOUBLE*)psaRates->pvData, (DOUBLE*)psaYTEs->pvData, dYTE);
	
	__CHECK_HRESULT(::SafeArrayUnlock(psaRates), _T("Unable to unlock rates array."));
	__CHECK_HRESULT(::SafeArrayUnlock(psaYTEs), _T("Unable to unlock YTEs array."));

	return dRate;
}

void CMmRpUndAtom::_GetBasketIndexDividends(REGULAR_DIVIDENDS* pDivs, LONG nMaxCount) throw()
{
	EtsRegularDividend aDiv;

	for(LONG i = 0; i < nMaxCount; i++)
	{
		__CHECK_HRESULT(m_spBasketIndex->get_BasketDiv(i, &aDiv), _T("Failed to get basket dividend."));
		pDivs[i].nLastDivDate = static_cast<LONG>(aDiv.LastDate);
		pDivs[i].nFrequency = aDiv.Freq;
		pDivs[i].dAmount = aDiv.Amt;
	}
}

void CMmRpUndAtom::_GetSyntheticRootBasketDividends(ISynthRootAtomPtr& spSynthRoot, REGULAR_DIVIDENDS* pDivs, 
													LONG nMaxCount) throw()
{
	EtsRegularDividend aDiv;

	for(LONG i = 0; i < nMaxCount; i++)
	{
		__CHECK_HRESULT(spSynthRoot->get_BasketDiv(i, &aDiv), _T("Failed to get basket dividends."));
		pDivs[i].nLastDivDate = static_cast<LONG>(aDiv.LastDate);
		pDivs[i].nFrequency = aDiv.Freq;
		pDivs[i].dAmount = aDiv.Amt;
	}
}

/*LONG CMmRpUndAtom::_AllocDividendsArrays(LONG nToday, LONG nExpiry, LONG nDivDate, 
			CUserArray<DOUBLE>& dDivDtes, CUserArray<DOUBLE>& dDivAmts) throw()
{
	HRESULT	hr;
	LONG	nRetCount = 0L;
	LONG	nDivCount = 0L;
	

	if(m_enUndType == enCtStock)
	{
		if(m_nDivFreq != 0L)
		{
			LONG nDivCount = ::GetDividendsCount(nToday, nExpiry - nToday, nDivDate, m_nDivFreq);
			if(nDivCount > 0L)
			{
				bool bRes = dDivDtes.Alloc(nDivCount);
				if (!bRes)
					CComErrorWrapper::ThrowError(E_FAIL, _T("Failed to alloc dividends."));

				bRes = dDivAmts.Alloc(nDivCount);
				if (!bRes)
					CComErrorWrapper::ThrowError(E_FAIL, _T("Failed to alloc dividends."));
				::GetDividends2(nToday, nExpiry - nToday, nDivDate, m_nDivFreq, m_dDivAmt, nDivCount, (DOUBLE*)dDivAmts, (DOUBLE*)dDivDtes, &nRetCount);
			}
		}
	}
	else
	{
		if(m_spBasketIndex != NULL)
		{
			LONG			nBaskDivCount = 0L; 
			VARIANT_BOOL	bIsBasket = VARIANT_FALSE;

			__CHECK_HRESULT3(m_spBasketIndex->get_IsBasket(&bIsBasket));
			__CHECK_HRESULT3(m_spBasketIndex->get_BasketDivsCount(&nBaskDivCount));

			if(VARIANT_FALSE != bIsBasket && nBaskDivCount > 0L)
			{
				REGULAR_DIVIDENDS* pBaskDivs = static_cast<REGULAR_DIVIDENDS*>(_alloca(nBaskDivCount * sizeof(REGULAR_DIVIDENDS)));
				ATLASSERT(pBaskDivs);

				_GetBasketIndexDividends(pBaskDivs, nBaskDivCount);

				nDivCount = ::GetBasketDividendsCount(nToday, nExpiry - nToday, pBaskDivs, nBaskDivCount);
				if(nDivCount > 0L)
				{
					bool bRes = dDivDtes.Alloc(nDivCount);
					if (!bRes)
						CComErrorWrapper::ThrowError(E_FAIL, _T("Failed to alloc dividends."));

					bRes = dDivAmts.Alloc(nDivCount);
					if (!bRes)
						CComErrorWrapper::ThrowError(E_FAIL, _T("Failed to alloc dividends."));

					::GetBasketDividends(nToday, nExpiry - nToday, pBaskDivs, nBaskDivCount, (DOUBLE*)dDivAmts, (DOUBLE*)dDivDtes, nDivCount, &nRetCount);
				}
			}
		}
	}

	return nRetCount;
}*/

/*LONG CMmRpUndAtom::_AllocSyntheticRootDividendsArrays(ISynthRootAtom* pSynthRoot, LONG nToday, LONG nExpiry, 
					CUserArray<DOUBLE>& dDivDtes, CUserArray<DOUBLE>& dDivAmts) throw()
{
	if (!pSynthRoot)
		CComErrorWrapper::ThrowError(E_INVALIDARG, _T(""));

	LONG	nRetCount = 0L;
	LONG	nDivCount = 0L;

	DOUBLE	dSpotBid = 0.;
	DOUBLE	dSpotAsk = 0.;
	DOUBLE	dSpotLast = 0.;	

	ISynthRootAtomPtr	spSynthRootAtom(pSynthRoot);
	ATLASSERT(spSynthRootAtom!=NULL);

	VARIANT_BOOL	bIsBasket = spSynthRootAtom->Basket;

	if(bIsBasket == VARIANT_TRUE)
	{
		LONG	nBaskDivCount = spSynthRootAtom->BasketDivsCount;
		if(nBaskDivCount > 0L)
		{
			REGULAR_DIVIDENDS* pBaskDivs = static_cast<REGULAR_DIVIDENDS*>(_alloca(nBaskDivCount * sizeof(REGULAR_DIVIDENDS)));
			ATLASSERT(pBaskDivs);

			_GetSyntheticRootBasketDividends(spSynthRootAtom, pBaskDivs, nBaskDivCount);

			nDivCount = ::GetBasketDividendsCount(nToday, 
				nExpiry - nToday, 
				pBaskDivs, 
				nBaskDivCount);
			if(nDivCount > 0L)
			{
				bool bRes = dDivDtes.Alloc(nDivCount);
				if (!bRes)
					CComErrorWrapper::ThrowError(E_FAIL, _T("Failed to alloc dividends."));

				bRes = dDivAmts.Alloc(nDivCount);
				if (!bRes)
					CComErrorWrapper::ThrowError(E_FAIL, _T("Failed to alloc dividends."));

				::GetBasketDividends(nToday, 
					nExpiry - nToday, 
					pBaskDivs, 
					nBaskDivCount, 
					(DOUBLE*)dDivAmts, 
					(DOUBLE*)dDivDtes, 
					nDivCount, 
					&nRetCount);
			}
		}
	}

	return nRetCount;
}*/

DOUBLE	CMmRpUndAtom::_CalcRegularForwardPrice(DOUBLE dSpotPrice, DATE dtExpiryOV, DATE tmCloseTime, DATE dtNow,
											   DOUBLE dForeignRate, SAFEARRAY* psaRates, SAFEARRAY* psaYTEs,
											   ICalculationParametrsPtr spParams) throw()
{
	if (psaRates == NULL || psaYTEs == NULL || !(bool)spParams)
		CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Invalid array pointer."));

	LPSAFEARRAY psaDates = NULL;
	LPSAFEARRAY psaAmounts = NULL;
	DOUBLE * pdDates = NULL;	
	DOUBLE * pdAmounts = NULL;
	LPVOID lpDateData = NULL;
	LPVOID lpAmountData = NULL;


	__CHECK_HRESULT(::SafeArrayLock(psaRates), _T("Unable to lock rates array."));
	__CHECK_HRESULT(::SafeArrayLock(psaYTEs), _T("Unable to lock DTEs array."));

	if (::SafeArrayGetDim(psaRates) != 1 || ::SafeArrayGetDim(psaYTEs) != 1)
		CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Invalid array dimension."));

#ifdef _DEBUG
	VARTYPE vType;
	::SafeArrayGetVartype(psaRates, &vType);
	ATLASSERT(vType == VT_R8);
	::SafeArrayGetVartype(psaYTEs, &vType);
	ATLASSERT(vType == VT_R8);
#endif

	if (psaRates->rgsabound[0].cElements ==0 || psaYTEs->rgsabound[0].cElements == 0)
		CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Array has zero length."));

	if (psaRates->rgsabound[0].cElements != psaYTEs->rgsabound[0].cElements)
		CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Arrays should have equal lengths."));
	
	ICalculationParametrs* pParams = (ICalculationParametrs*)(spParams);
	DOUBLE dYTE;
	::GetCalculationParams(dtNow, dtExpiryOV, tmCloseTime, pParams->UseTimePrecision != VARIANT_FALSE, &dtNow, &dtExpiryOV, &tmCloseTime, &dYTE);

	if (dYTE < 0)	dYTE = 0.;

	LONG	nRetCount = 0L;
	LONG	nDivCount = 0L;	
	DOUBLE*	pdDivDte = 0;
	DOUBLE*	pdDivAmt = 0;


	EtsDivTypeEnum enDivType = enDivCustomStream;
	IEtsIndexDivAtomPtr spDiv = m_spDividend;
	if (spDiv)
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
				m_spDividend->GetDividends2(dtNow, dtExpiryOV, tmCloseTime, nDivCount, &psaAmounts, &psaDates, &nRetCount);
				::SafeArrayLock(psaDates);
				::SafeArrayLock(psaAmounts);
				::SafeArrayAccessData(psaDates, &lpDateData);
				::SafeArrayAccessData(psaAmounts, &lpAmountData);
				pdDivDte = reinterpret_cast<DOUBLE *>(lpDateData);
				pdDivAmt = reinterpret_cast<DOUBLE *>(lpAmountData);
			}
		}
		break;
	case enDivStockBasket:
		{
			if(m_spBasketIndex != NULL)
			{
				LONG nBaskDivCount = 0L; VARIANT_BOOL bIsBasket = VARIANT_FALSE;
				nDivCount = 0;
				IEtsIndexDivCollPtr spDivColl = NULL;
				m_spBasketIndex->get_BasketDivs(&spDivColl);
				_CHK(m_spBasketIndex->get_IsBasket(&bIsBasket));

				if (bIsBasket && spDivColl != NULL)
				{
					spDivColl->GetDividendCount2(dtNow, dtExpiryOV, tmCloseTime, &nDivCount);
					if(nDivCount > 0L)
					{
						spDivColl->GetDividends2(dtNow, dtExpiryOV, tmCloseTime, nDivCount, &psaAmounts, &psaDates, &nDivCount);
						::SafeArrayLock(psaDates);
						::SafeArrayLock(psaAmounts);
						::SafeArrayAccessData(psaDates, &lpDateData);
						::SafeArrayAccessData(psaAmounts, &lpAmountData);
						pdDivDte	 =	 reinterpret_cast<DOUBLE *>(lpDateData);
						pdDivAmt	= reinterpret_cast<DOUBLE *>(lpAmountData);
					}
				}
			}
		}
		break;
	case enDivIndexYield:
		{
			//dYield = m_dYield;
		}
		break;	
	}

	if(nDivCount < 0)
		nDivCount = 0;

	DOUBLE	dPrice = ::CalcForwardPrice2(dSpotPrice, dYTE, nDivCount, pdDivAmt, pdDivDte, 
		dForeignRate, psaRates->rgsabound[0].cElements, (DOUBLE*)psaRates->pvData, reinterpret_cast<DOUBLE*>(psaYTEs->pvData) );

	__CHECK_HRESULT(::SafeArrayUnlock(psaRates), _T("Unable to unlock rates array."));
	__CHECK_HRESULT(::SafeArrayUnlock(psaYTEs), _T("Unable to unlock DTEs array."));

	if (psaAmounts !=NULL)
	{
		::SafeArrayUnaccessData(psaAmounts);
		::SafeArrayUnlock(psaAmounts);
		::SafeArrayDestroy(psaAmounts);
	}

	if (psaDates !=NULL)
	{
		::SafeArrayUnaccessData(psaDates);
		::SafeArrayUnlock(psaDates);
		::SafeArrayDestroy(psaDates);
	}
	return dPrice;
}
DOUBLE	CMmRpUndAtom::_CalcSyntheticForwardPrice(ISynthRootAtom* pSynthRoot, 
												 DOUBLE dSpotPrice, 
												 DATE dtExpiryOV, 
												 DATE tmCloseTime, 
												 DATE dtNow, 
												 DOUBLE dForeignRate, 
												 SAFEARRAY* psaRates, 
												 SAFEARRAY* psaYTEs,
												 ICalculationParametrsPtr spParams) throw()
{
	if (!pSynthRoot || !((bool)spParams))
		CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Synthetic Root Ptr or Calculation Parametrs Ptr"));

	if (psaRates == NULL || psaYTEs == NULL)
		CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Invalid array pointer."));

	__CHECK_HRESULT(::SafeArrayLock(psaRates), _T("Unable to lock rates array."));
	__CHECK_HRESULT(::SafeArrayLock(psaYTEs), _T("Unable to lock DTEs array."));

	if (::SafeArrayGetDim(psaRates) != 1 || ::SafeArrayGetDim(psaYTEs) != 1)
		CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Invalid array dimension."));

#ifdef _DEBUG
	VARTYPE vType;
	::SafeArrayGetVartype(psaRates, &vType);
	ATLASSERT(vType == VT_R8);
	::SafeArrayGetVartype(psaYTEs, &vType);
	ATLASSERT(vType == VT_R8);
#endif

	if (psaRates->rgsabound[0].cElements ==0 || psaYTEs->rgsabound[0].cElements == 0)
		CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Array has zero length."));

	if (psaRates->rgsabound[0].cElements != psaYTEs->rgsabound[0].cElements)
		CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Arrays should have equal lengths."));

	ICalculationParametrs* pParams = (ICalculationParametrs*)(spParams);
	DATE dYTE;
	::GetCalculationParams(dtNow, dtExpiryOV, tmCloseTime, pParams->UseTimePrecision != VARIANT_FALSE, &dtNow, &dtExpiryOV, &tmCloseTime, &dYTE);

	if (dYTE < 0)	dYTE = 0.;

	LONG	nRetCount = 0L;
	LONG	nDivCount = 0L;
	DOUBLE*	pdDivDte = 0;
	DOUBLE*	pdDivAmt = 0;

	DOUBLE	dSpotBid = 0.;
	DOUBLE	dSpotAsk = 0.;
	DOUBLE	dSpotLast = 0.;	

	LPSAFEARRAY psaDates = NULL;
	LPSAFEARRAY psaAmounts = NULL;
	DOUBLE * pdDates = NULL;	
	DOUBLE * pdAmounts = NULL;
	LPVOID lpDateData = NULL;
	LPVOID lpAmountData = NULL;


	ISynthRootAtomPtr	spSynthRootAtom(pSynthRoot);
	ATLASSERT(spSynthRootAtom!=NULL);

	VARIANT_BOOL	bIsBasket = spSynthRootAtom->Basket;

	if(bIsBasket == VARIANT_TRUE)
	{
		LONG nBaskDivCount = 0L; VARIANT_BOOL bIsBasket = VARIANT_FALSE;
		nDivCount = 0;
		IEtsIndexDivCollPtr spDivColl = NULL;
		spSynthRootAtom->get_BasketDivs(&spDivColl);
		if (spDivColl != NULL)
		{
			spDivColl->GetDividendCount2(dtNow, dtExpiryOV, tmCloseTime, &nDivCount);
			if(nDivCount > 0L)
			{
				spDivColl->GetDividends2(dtNow, dtExpiryOV, tmCloseTime, nDivCount, &psaAmounts, &psaDates, &nRetCount);
				::SafeArrayLock(psaDates);
				::SafeArrayLock(psaAmounts);
				::SafeArrayAccessData(psaDates, &lpDateData);
				::SafeArrayAccessData(psaAmounts, &lpAmountData);
				pdDivDte	 =	 reinterpret_cast<DOUBLE *>(lpDateData);
				pdDivAmt	= reinterpret_cast<DOUBLE *>(lpAmountData);

			}
		}
	}
	if (nRetCount < 0)
		nRetCount = 0;

	if (nDivCount < 0)
		nDivCount = 0;

	DOUBLE	dPrice = ::CalcForwardPrice2(dSpotPrice, dYTE, nRetCount, pdDivAmt, pdDivDte, 
		dForeignRate, psaRates->rgsabound[0].cElements, (DOUBLE*)psaRates->pvData, reinterpret_cast<DOUBLE*>(psaYTEs->pvData) );

	__CHECK_HRESULT(::SafeArrayUnlock(psaRates), _T("Unable to unlock rates array."));
	__CHECK_HRESULT(::SafeArrayUnlock(psaYTEs), _T("Unable to unlock DTEs array."));

	if (psaAmounts !=NULL)
	{
		::SafeArrayUnaccessData(psaAmounts);
		::SafeArrayUnlock(psaAmounts);
		::SafeArrayDestroy(psaAmounts);
	}

	if (psaDates !=NULL)
	{
		::SafeArrayUnaccessData(psaDates);
		::SafeArrayUnlock(psaDates);
		::SafeArrayDestroy(psaDates);
	}

	return dPrice;
}

void CMmRpUndAtom::_GetSyntheticPrice(ISynthRootAtom* pRootAtom, IMmRpUndColl* pUndColl,
			 		DOUBLE& dSpotBid, DOUBLE& dSpotAsk, DOUBLE& dSpotLast) throw()
{
	if (!pRootAtom)
	{
		CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Invalid synthetic root."));
	}

	if (!pUndColl)
	{
		CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Invalid underlyings collection."));
	}	

	dSpotBid = 0.;
	dSpotAsk = 0.;
	dSpotLast = 0.;

	HRESULT	hr;

	bool	bBadSpotBid = false;
	bool	bBadSpotAsk = false;
	bool	bBadSpotLast = false;

	ISynthRootAtomPtr	spRootAtom(pRootAtom);
	IMmRpUndCollPtr		spUndColl(pUndColl);

	DOUBLE dUndBid , dUndAsk , dUndLast ;
	m_spPrice->get_Bid ( &dUndBid) ;
	m_spPrice->get_Ask ( &dUndAsk ) ;
	m_spPrice->get_Last( &dUndLast ) ;

	__CHECK_HRESULT3(pRootAtom->get_CashValue(&dSpotBid));
	dSpotAsk = dSpotBid;
	dSpotLast = dSpotBid;

	_variant_t	varItem;
	ULONG		nFetched = 0L;	

	ISynthRootCompCollPtr spSRCompColl = spRootAtom->SynthRootComponents;
	ATLASSERT(spSRCompColl!=0);
	IUnknownPtr	spUnk = spSRCompColl->_NewEnum;

	IEnumVARIANTPtr	spSynthEnum(spUnk);
	__CHECK_HRESULT(spSynthEnum->Reset(), _T("Fail to reset synthetic roots components collection."));
	while((hr = spSynthEnum->Next(1L, &varItem, &nFetched)) == S_OK)
	{
		ATLASSERT(varItem.vt == VT_DISPATCH);
		ISynthRootCompAtomPtr spSRCompAtom = varItem;

		LONG	nSynthUndID = 0L;
		DOUBLE	dSynthWeight = 0.;	

		__CHECK_HRESULT3(spSRCompAtom->get_UndID(&nSynthUndID));
		__CHECK_HRESULT3(spSRCompAtom->get_Weight(&dSynthWeight));

		if(nSynthUndID != m_nID)
		{
			IMmRpUndAtomPtr spUndAtom;
			__CHECK_HRESULT3(spUndColl->get_Item(_bstr_t(nSynthUndID), &spUndAtom));
			if (spUndAtom==0)
				continue;

			DOUBLE	dUndPriceBid = 0.;
			DOUBLE	dUndPriceAsk = 0.;
			DOUBLE	dUndPriceLast = 0.;

			IMMRpPricePtr spPrice ;
			spUndAtom->get_Price ( &spPrice ) ;
			ATLASSERT ( spPrice ) ;


			__CHECK_HRESULT3(spPrice->get_Bid(&dUndPriceBid));
			__CHECK_HRESULT3(spPrice->get_Ask(&dUndPriceAsk));
			__CHECK_HRESULT3(spPrice->get_Last(&dUndPriceLast));

			if(!bBadSpotBid && dUndPriceBid > 0.)
			{
				dSpotBid += dUndPriceBid * dSynthWeight;
			}
			else
			{
				bBadSpotBid = true;
				dSpotBid = 0.;
			}

			if(!bBadSpotAsk && dUndPriceAsk > 0.)
			{
				dSpotAsk += dUndPriceAsk * dSynthWeight;
			}
			else
			{
				bBadSpotAsk = true;
				dSpotAsk = 0.;
			}

			if(!bBadSpotLast && dUndPriceLast > 0.)
			{
				dSpotLast += dUndPriceLast * dSynthWeight;
			}
			else
			{
				bBadSpotLast = true;
				dSpotLast = 0.;
			}
		}
		else
		{
			if(!bBadSpotBid && dUndBid > 0.)
			{
				dSpotBid += dUndBid * dSynthWeight;
			}
			else
			{
				bBadSpotBid = true;
				dSpotBid = 0.;
			}
		
			if(!bBadSpotAsk && dUndAsk > 0.)
			{
				dSpotAsk += dUndAsk * dSynthWeight;
			}
			else
			{
				bBadSpotAsk = true;
				dSpotAsk = 0.;
			}

			if(!bBadSpotLast && dUndLast > 0.)
			{
				dSpotLast += dUndLast * dSynthWeight;
			}
			else
			{
				bBadSpotLast = true;
				dSpotLast = 0.;
			}
		}
	}
}

LONG CMmRpUndAtom::_CalcOptionGreeks(EtsCalcModelTypeEnum enCalcModel, IMmRpOptAtom* pOptAtom, 
						DOUBLE dRate, DOUBLE dHTBRate, VARIANT_BOOL bUseTheoVolatility, VARIANT_BOOL bUseTheoVolaNoBid, 
						VARIANT_BOOL bUseTheoVolaBadMarket, IMmRpUndColl* pUndColl, DOUBLE dUndPriceTolerance,
						EtsPriceRoundingRuleEnum enPriceRoundingRule, GREEKS &aGreeks,
						ICalculationParametrsPtr spParams,
						DOUBLE dUnderlyingPrice,
						EtsOptionTypeEnum*	_penOptType, bool useFuture, DOUBLE dUndPriceShift )
{
	if (!pOptAtom || !(bool)spParams)
		CComErrorWrapper::ThrowError(E_POINTER, _T(""));


	DOUBLE dUndBid , dUndAsk , dUndLast ;
	m_spPrice->get_Bid ( &dUndBid) ;
	m_spPrice->get_Ask ( &dUndAsk ) ;
	m_spPrice->get_Last( &dUndLast ) ;

	IMmRpOptAtomPtr		spOptAtom(pOptAtom);
	IMmRpUndCollPtr		spUndColl(pUndColl);

	DATE				dtExpiryOV = 0., tmCloseTime = 0.;
	DOUBLE				dStrike			= 0.;	
	DOUBLE				dOptBid			= 0.;
	DOUBLE				dOptAsk			= 0.;
	DOUBLE				dOptLast		= 0.;
	LONG				nOptRootID		= 0L;
	EtsOptionTypeEnum	enOptType;
	LPSAFEARRAY			psaDates		= NULL;
	LPSAFEARRAY			psaAmounts		= NULL;
	DOUBLE*				pdDates			= NULL;	
	DOUBLE*				pdAmounts		= NULL;
	LPVOID				lpDateData		= NULL;
	LPVOID				lpAmountData	= NULL;


	__CHECK_HRESULT3(spOptAtom->get_Strike(&dStrike));
	__CHECK_HRESULT3(spOptAtom->get_OptType(&enOptType));
	__CHECK_HRESULT3(spOptAtom->get_ExpiryOV(&dtExpiryOV));
	__CHECK_HRESULT3(spOptAtom->get_TradingClose(&tmCloseTime));

	IMMRpPricePtr spPrice ;
	spOptAtom->get_Price( &spPrice ) ;
	ATLASSERT ( spPrice ) ;

	__CHECK_HRESULT3(spPrice->get_Bid(&dOptBid));	
	__CHECK_HRESULT3(spPrice->get_Ask(&dOptAsk));
	__CHECK_HRESULT3(spPrice->get_Last(&dOptLast));

	__CHECK_HRESULT3(spOptAtom->get_RootID(&nOptRootID));
	
	DOUBLE	dOptBidForIv = dOptBid;	
	DOUBLE	dOptPrice = m_spOptPriceProfile->GetOptPriceMid(dOptBid, dOptAsk, dOptLast, 
		enPriceRoundingRule, bUseTheoVolatility, 0., NULL);

	ICalculationParametrs* pParams = (ICalculationParametrs*)spParams;

	DATE	dtNow; 
	::GetNYDateTimeAsDATE(&dtNow);
	DOUBLE	dYTE;
	DOUBLE	dtExpiryOVOrig = dtExpiryOV;
	::GetCalculationParams(dtNow, dtExpiryOV, tmCloseTime, pParams->UseTimePrecision != VARIANT_FALSE, &dtNow, &dtExpiryOV, &tmCloseTime, &dYTE);
	
	bool				bIsRootSynthetic = false;
	ISynthRootAtomPtr	spSynthRootAtom;

	if (m_vbHasSynthetic == VARIANT_TRUE)
	{
		spSynthRootAtom = m_spSyntheticRoots->Item[nOptRootID];
		if (spSynthRootAtom != NULL) //is synthetic
			bIsRootSynthetic = true;
	}

	DOUBLE	dPrice = 0.;
	DOUBLE	dPriceForGreeks = 0.;
	DOUBLE	dYield = 0.;
	DOUBLE	dSkew = 0.;
	DOUBLE	dKurt = 0.;	
	LONG	nRetCount = 0L;
	DOUBLE*	pdDivDte = 0;
	DOUBLE*	pdDivAmt = 0;
	LONG	nDivCount = 0L;
	bool	isFutureOption = false;
	EtsOptionTypeEnum futureOptionType = enOtCall;
	VARIANT_BOOL	futureOptionStyle = VARIANT_FALSE;
	if (!bIsRootSynthetic)
	{
		dSkew = m_dSkew;
		dKurt = m_dKurt;
		EtsContractTypeEnum optionType;
		_CHK(spOptAtom->get_ContractType(&optionType) );
		if ( enCtFutOption  == optionType )
		{
			DOUBLE futureBid = 0., futureAsk = 0., futureLast = 0.;
			// this is future option, so we use future's price instead of base asset
			IMmRpFutAtomPtr spFut = NULL;
			_CHK( spOptAtom->get_Fut( &spFut) );
			if ( NULL != spFut)
			{
				IEtsPriceProfileAtomPtr spFutPriceProfile = NULL;
				_CHK( spFut->get_UndPriceProfile(&spFutPriceProfile));
				if ( NULL != spFutPriceProfile )
				{
					VARIANT_BOOL	futureUsed;
					_CHK(spFut->GetFuturePrice(dUndPriceTolerance,enPriceRoundingRule, NULL, &futureUsed, &dPrice));

					dPrice *= ( 1. + dUndPriceShift/ 100.);
					dPriceForGreeks  = dPrice;
					isFutureOption = true;
					_CHK(spOptAtom->get_OptType(&futureOptionType));
					_CHK(spFut->get_IsAmerican(&futureOptionStyle));
				}
				else
					CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Future price profile not initialized."));
			}
			else
				CComErrorWrapper::ThrowError(E_INVALIDARG, _T("No future found for future option."));

		}
		else{
			VARIANT_BOOL	futureUsed = VARIANT_FALSE;
			__CHECK_HRESULT3(GetUnderlyingPrice(dUndPriceTolerance, enPriceRoundingRule, NULL, &futureUsed, &dPrice ));
			if ( futureUsed && dPrice > 0. ) {
				dUndBid = dUndAsk = dUndLast = dPrice;
			}
		dPriceForGreeks = dUnderlyingPrice>0 ? dUnderlyingPrice:dPrice;

		}

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
					m_spDividend->GetDividends2(dtNow, dtExpiryOV, tmCloseTime, nDivCount, &psaAmounts, &psaDates, &nRetCount);
					::SafeArrayLock(psaDates);
					::SafeArrayLock(psaAmounts);
					::SafeArrayAccessData(psaDates, &lpDateData);
					::SafeArrayAccessData(psaAmounts, &lpAmountData);
					pdDivDte	 =	 reinterpret_cast<DOUBLE *>(lpDateData);
					pdDivAmt	= reinterpret_cast<DOUBLE *>(lpAmountData);
				}

			}
			break;
		case enDivStockBasket:
			{
				VARIANT_BOOL bIsBasket = VARIANT_FALSE;
				if(m_spBasketIndex != NULL)
				{
					LONG nBaskDivCount = 0L; 
					nDivCount = 0;
					IEtsIndexDivCollPtr spDivColl = NULL;
					m_spBasketIndex->get_BasketDivs(&spDivColl);
					_CHK(m_spBasketIndex->get_IsBasket(&bIsBasket));

					if (bIsBasket && spDivColl != NULL)
					{
						spDivColl->GetDividendCount2(dtNow, dtExpiryOV, tmCloseTime, &nDivCount);
						if(nDivCount > 0L)
						{
							spDivColl->GetDividends2(dtNow, dtExpiryOV, tmCloseTime, nDivCount, &psaAmounts, &psaDates, &nDivCount);
							::SafeArrayLock(psaDates);
							::SafeArrayLock(psaAmounts);
							::SafeArrayAccessData(psaDates, &lpDateData);
							::SafeArrayAccessData(psaAmounts, &lpAmountData);
							pdDivDte	 =	 reinterpret_cast<DOUBLE *>(lpDateData);
							pdDivAmt	= reinterpret_cast<DOUBLE *>(lpAmountData);

						}
					}

				}
			}
			break;
		case enDivIndexYield:
			{
				dYield = m_dYield;
			}
			break;	
		}
		if(nDivCount < 0)
			nDivCount = 0;
	}
	else // synthetic
	{
		dSkew = spSynthRootAtom->Skew;
		dKurt = spSynthRootAtom->Kurt;
		dYield = spSynthRootAtom->Yield;

		DOUBLE	dSpotBid = 0.;
		DOUBLE	dSpotAsk = 0.;
		DOUBLE	dSpotLast = 0.;

		_GetSyntheticPrice(spSynthRootAtom, spUndColl, dSpotBid, dSpotAsk, dSpotLast);
		dPrice = m_spUndPriceProfile->GetUndPriceMid(dSpotBid, dSpotAsk, dSpotLast, 
			dUndPriceTolerance, enPriceRoundingRule, NULL, VARIANT_FALSE );

		dPriceForGreeks = dUnderlyingPrice>0 ? dUnderlyingPrice:dPrice;

		VARIANT_BOOL	bIsBasket = spSynthRootAtom->Basket;

		if(bIsBasket == VARIANT_TRUE)
		{
			dYield = 0.0;
			LONG nBaskDivCount = 0L; VARIANT_BOOL bIsBasket = VARIANT_FALSE;
			nDivCount = 0;
			IEtsIndexDivCollPtr spDivColl = NULL;
			spSynthRootAtom->get_BasketDivs(&spDivColl);
			if (spDivColl != NULL)
			{
				spDivColl->GetDividendCount2(dtNow, dtExpiryOV, tmCloseTime, &nDivCount);
				if(nDivCount > 0L)
				{
					spDivColl->GetDividends2(dtNow, dtExpiryOV, tmCloseTime, nDivCount, &psaAmounts, &psaDates, &nDivCount);
					::SafeArrayLock(psaDates);
					::SafeArrayLock(psaAmounts);
					::SafeArrayAccessData(psaDates, &lpDateData);
					::SafeArrayAccessData(psaAmounts, &lpAmountData);
					pdDivDte	 =	 reinterpret_cast<DOUBLE *>(lpDateData);
					pdDivAmt	= reinterpret_cast<DOUBLE *>(lpAmountData);

				}
			}
		}
	}

	if ( dPriceForGreeks <= 0. ) return	-1;

	dStrike = _AdjustFractionalDigits(dStrike, 4);
	DOUBLE	dVola = 0.;

	if (bUseTheoVolatility == VARIANT_TRUE)
	{
		LONG lSurfaceID = m_spVolaSrv->GetSurfaceByRoot(nOptRootID);
		dVola = m_spVolaSrv->GetOptionVola(dtExpiryOV, dStrike, lSurfaceID);
	}
	else 
	{
		if(VARIANT_TRUE == bUseTheoVolaNoBid && (DoubleEQZero(dOptBidForIv) || dOptBidForIv < 0.))
		{
			LONG lSurfaceID = m_spVolaSrv->GetSurfaceByRoot(nOptRootID);
			dVola = m_spVolaSrv->GetOptionVola(dtExpiryOV, dStrike, lSurfaceID);
		}
		else
		{
			LONG	nFlag = VF_OK;
			if(dOptPrice > 0.)
			{
				dVola = isFutureOption	? ::CalcFutureOptionVolatility(	dRate, dPrice, dOptPrice, dStrike, 
																		dYTE, futureOptionType, 
																		futureOptionStyle, 100, 
																		dSkew, dKurt, enCalcModel, &nFlag)
										:  
										::CalcVolatilityMM3(	dRate, dYield, dHTBRate, dPrice, dOptPrice, dStrike, 
																		dYTE, enOptType, 
																		m_vbIsAmerican == VARIANT_TRUE, nRetCount, 
																		pdDivAmt, pdDivDte, 100L, dSkew, dKurt, 
																		enCalcModel, &nFlag);
			}
            else if (bUseTheoVolaBadMarket == VARIANT_TRUE)
			{
				LONG lSurfaceID = m_spVolaSrv->GetSurfaceByRoot(nOptRootID);
				dVola = m_spVolaSrv->GetOptionVola(dtExpiryOV, dStrike, lSurfaceID);
			}

            if (bUseTheoVolaBadMarket == VARIANT_TRUE && nFlag != VF_OK)
			{
				LONG lSurfaceID = m_spVolaSrv->GetSurfaceByRoot(nOptRootID);
				dVola = m_spVolaSrv->GetOptionVola(dtExpiryOV, dStrike, lSurfaceID);
			}
		}
    }
	
	long lRet = -1;

	if(isFutureOption)
	{
			if(m_enUndType==enCtFutUnd)
			{
					lRet = ::CalcFutureOptionGreeks2(	dRate, dPriceForGreeks, false, dStrike, dVola, 
																				dYTE, futureOptionType, 
																				futureOptionStyle, 100L, 
																				dSkew, dKurt,enCalcModel, &aGreeks);
			}
			else /*enCtIndex*/
			{
					lRet = ::CalcFutureOptionGreeks3(dRate, dYield, dPriceForGreeks,true/*false*/, dStrike, dVola, dYTE,
					futureOptionType, futureOptionStyle, 100L, dSkew, dKurt, enCalcModel, lRet, pdDivAmt, pdDivDte, &aGreeks);
			}
	}
	else
	{
			lRet = ::CalcGreeksMM2(	dRate, dYield, dHTBRate, dPriceForGreeks, dStrike, dVola, 
																	dYTE, _penOptType ? *_penOptType : enOptType , 
																	m_vbIsAmerican == VARIANT_TRUE, nDivCount, pdDivAmt, pdDivDte, 
																	100L, dSkew, dKurt, enCalcModel, &aGreeks);
	}

	
	if (psaAmounts !=NULL)
	{
		::SafeArrayUnaccessData(psaAmounts);
		::SafeArrayUnlock(psaAmounts);
		::SafeArrayDestroy(psaAmounts);
	}

	if (psaDates !=NULL)
	{
		::SafeArrayUnaccessData(psaDates);
		::SafeArrayUnlock(psaDates);
		::SafeArrayDestroy(psaDates);
	}
	return lRet;
}

_bstr_t CMmRpUndAtom::_GetOptionRoot(BSTR bsSymbol)
{
	CComBSTR bsSym = bsSymbol;
	CComBSTR bsRootSym;
	UINT nLen = bsSym.Length();
	if (nLen <= 2)
	{
		ATLASSERT(0);
		return bsSymbol;
	}
	else
	{
		bsRootSym.Append(bsSym, nLen - 2);
		return (BSTR)bsRootSym;
	}
}
STDMETHODIMP CMmRpUndAtom::GetUnderlyingPrice(DOUBLE dTolerance, EtsPriceRoundingRuleEnum enPriceRound,EtsReplacePriceStatusEnum * penPriceStatus, VARIANT_BOOL *bFutureUsed, DOUBLE *pPrice){
	HRESULT	hr = S_OK;

	if( !pPrice || !bFutureUsed)
		return E_POINTER;
	try{
		*pPrice = 0;
		*bFutureUsed = VARIANT_FALSE;

		bool dontUseFuture = true;
		if ( m_spActiveFuture )	{

			// this underlying - is index with active future 
			DOUBLE dActiveFutureMid = 0., dActiveFutureBid = 0., dActiveFutureAsk = 0., dActiveFutureLast = 0.;
			DOUBLE dActiveFutureBasis = 0.;
			IEtsPriceProfileAtomPtr spUndPriceProfile;

			m_spActiveFuture->get_UndPriceProfile(&spUndPriceProfile);
			if ( NULL != spUndPriceProfile)
			{
				IMMRpPricePtr	activeFuturePrice;
				_CHK(m_spActiveFuture->get_FutureBasis(&dActiveFutureBasis));
				_CHK(m_spActiveFuture->get_Price(&activeFuturePrice));
				
				if ( activeFuturePrice )	{
					_CHK(activeFuturePrice->get_Bid(&dActiveFutureBid) );
					_CHK(activeFuturePrice->get_Ask(&dActiveFutureAsk) );
					_CHK(activeFuturePrice->get_Last(&dActiveFutureLast) );

					dActiveFutureMid = spUndPriceProfile->GetUndPriceMid(	dActiveFutureBid,
																							dActiveFutureAsk,
																							dActiveFutureLast,
																							dTolerance,
																							enPriceRound,
																							penPriceStatus, VARIANT_FALSE);
					if ( dActiveFutureMid > 0.)
					{
						dActiveFutureMid += dActiveFutureBasis;
						_CHK(m_spActiveFuture->put_ActiveFuturePrice(dActiveFutureMid));
						*pPrice = dActiveFutureMid;
						dontUseFuture = false;
						*bFutureUsed = VARIANT_TRUE;
					}
				}
			}
		}
		if ( dontUseFuture && m_spUndPriceProfile ){
			double futureBid = 0., futureAsk = 0., futureLast = 0.;
			if ( m_spPrice )	{
				_CHK(m_spPrice->get_Bid(&futureBid) );
				_CHK(m_spPrice->get_Ask(&futureAsk) );
				_CHK(m_spPrice->get_Last(&futureLast) );
				*pPrice = m_spUndPriceProfile->GetUndPriceMid(	futureBid, futureAsk, 
																				futureLast, dTolerance, 
																				enPriceRound, penPriceStatus, VARIANT_FALSE );
			}
		}
	}
	catch ( _com_error& e ) {
		hr = Error((PTCHAR)CComErrorWrapper::ErrorDescription(e), __uuidof(IMmRpUndAtom), e.Error());
	}
	return hr;
}

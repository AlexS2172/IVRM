// MmRvPosAtom.cpp : Implementation of CMmRvPosAtom
#include "stdafx.h"
#include "MmRvPosAtom.h"
#include "MmRvUndColl.h"
#include "MmRvUndAtom.h"

/////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CMmRvPosAtom::CalcOptionGreeks(IMmRvUndColl* aUndColl,
											IMmRvUndAtom* aUnd,
											DOUBLE dUndMidPrice,
											LONG nMask,
											VARIANT_BOOL bIsPnlLTD,
											VARIANT_BOOL bFitToImpFlatNoBid,
											MmRvFitToImpCurveTypeEnum enFitToImpCurve,
											EtsCalcModelTypeEnum enCalcModel,
											VARIANT_BOOL bUseTheoVolatility,
											VARIANT_BOOL bUseTheoVolaNoBid,
											VARIANT_BOOL bUseTheoVolaBadMarket,
											DOUBLE dUndPriceTolerance,
											EtsPriceRoundingRuleEnum enPriceRoundingRule,
											IMmRvUndColl* aSimulatedUndColl,
											VARIANT_BOOL bSimulate,
											VARIANT_BOOL bCalcSimulation,
											ICalculationParametrs* pParams)
{
	 if(aUnd == NULL || aUndColl == NULL || pParams == NULL)
		return Error(L"Invalid objects passed.", IID_IMmRvPosAtom, E_INVALIDARG);

	if(m_enContractType != enCtOption)
		return S_OK;

	if(!bSimulate)
	{
		m_simulationParams.m_dDate = 0;
		m_simulationParams.m_dSpotChangePercent = 0.;
		m_simulationParams.m_dVolaChangePercent = 0.;
	}

	try
	{
		IMmRvUndCollPtr spUndColl(aUndColl);
		IMmRvUndAtomPtr spUnd(aUnd);
		IEtsPriceProfileAtomPtr spUndPriceProfile;
		IEtsPriceProfileAtomPtr spOptPriceProfile;

		CSafeArrayWrapper<double> saDates;
		CSafeArrayWrapper<double> saAmounts;

		_CHK(spUnd->get_UndPriceProfile(&spUndPriceProfile));
		_CHK(spUnd->get_OptPriceProfile(&spOptPriceProfile));

		DOUBLE dUndPrice = dUndMidPrice;
		DOUBLE sUndPriceSave = dUndMidPrice;
		ISynthRootAtomPtr spSynthRoot;
		DOUBLE dSynthUndPrice = BAD_DOUBLE_VALUE;

		DOUBLE dtNYTime;
		::GetNYDateTimeAsDATE(&dtNYTime);

		DOUBLE dtCalcDate = bSimulate ? dtCalcDate = m_simulationParams.m_dDate : dtNYTime;

		if(m_bIsSynthetic)
		{
			ISynthRootCollPtr spSynthRootColl;
			_CHK(spUnd->get_SynthRoots(&spSynthRootColl));
			if(spSynthRootColl != NULL)
			{
				spSynthRoot = spSynthRootColl->Item[m_nOptionRootID];
				if(spSynthRoot != NULL)
				{
					DOUBLE dSynthBid = 0., dSynthAsk = 0., dSynthLast = 0.;
					DOUBLE dCashValue = spSynthRoot->CashValue < 0 ? 0. : spSynthRoot->CashValue;
					_CHK(spUnd->GetSyntheticUnderlyingPrice(spUndColl, spSynthRoot, &dSynthBid, &dSynthAsk, &dSynthLast));
					dUndMidPrice = spUndPriceProfile->GetUndPriceMid(dSynthBid, dSynthAsk, dSynthLast, dUndPriceTolerance, enPriceRoundingRule, NULL, VARIANT_FALSE);
					if ( bSimulate )
					{
						dUndMidPrice -= dCashValue;
						dUndMidPrice += ( dUndMidPrice/100 * static_cast<double>(m_simulationParams.m_dSpotChangePercent) ); 
						dUndMidPrice += dCashValue;
					}
					dSynthUndPrice = dUndMidPrice;
				}
			}
		}

		
		EtsReplacePriceStatusEnum enPriceStatusMid = enRpsNone;
		DOUBLE dOptPriceMid = 0.;
		dOptPriceMid = spOptPriceProfile->GetOptPriceMid(m_dPriceBid, m_dPriceAsk, m_dPriceLast, enPriceRoundingRule, bUseTheoVolatility, 0., &enPriceStatusMid);

		m_enReplacePriceStatus = enPriceStatusMid;
		__MmRvPosAtom::ClearValuesInShares();
		if(nMask != GT_NOTHING && dUndMidPrice > DBL_EPSILON)
		{
			GREEKS aGreeks;
			memset(&aGreeks, 0, sizeof(aGreeks));
			aGreeks.nMask = nMask;
			__MmRvPosAtom::ClearValues();

			m_dSynthUndPrice = dSynthUndPrice;

			LONG nModel = static_cast<LONG>(enCalcModel);
			VARIANT_BOOL bIsAmerican = VARIANT_FALSE;
			_CHK(spUnd->get_IsAmerican(&bIsAmerican));
			LONG nIsAmerican = (bIsAmerican ? 1L : 0L);

			DOUBLE dYield = 0., dVola = 0.;

			LONG nDivCount = 0L, nRetCount = 0L;
			EtsContractTypeEnum enUndType = enCtNone;
			_CHK(spUnd->get_ContractType(&enUndType));


			DOUBLE dtExpiry = m_dtExpiry;
			DOUBLE dtExpiryOV = m_dtExpiryOV;
			DOUBLE dtCloseTime = m_dtTradingClose;
			VARIANT_BOOL vbUseTimePrecision = VARIANT_TRUE;
			DOUBLE dYTE;

			::GetCalculationParams(dtCalcDate, dtExpiryOV, dtCloseTime, pParams->UseTimePrecision != VARIANT_FALSE, &dtCalcDate, &dtExpiryOV, &dtCloseTime, &dYTE);			

			DOUBLE dSkew = 0., dKurt = 0.;

			// prepare parameters
			if(!m_bIsSynthetic)
			{
				_CHK(spUnd->get_Skew(&dSkew));
				_CHK(spUnd->get_Kurt(&dKurt));

				EtsDivTypeEnum enDivType = enDivCustomStream;
				IEtsIndexDivAtomPtr spDiv;
				_CHK(spUnd->get_Dividend(&spDiv));
				if(spDiv)
					spDiv->get_DivType(&enDivType);

				switch(enDivType)
				{
				case enDivMarket:
				case enDivCustomPeriodical:
				case enDivCustomStream:
					{
						nDivCount = 0;

						if (spDiv != NULL)
						{
							spDiv->GetDividendCount2(dtCalcDate, dtExpiryOV, dtCloseTime, &nDivCount);
							if(nDivCount > 0L) 
							{
								LPSAFEARRAY psaAmounts = NULL;
								LPSAFEARRAY psaDates = NULL;

								spDiv->GetDividends2(dtCalcDate, dtExpiryOV, dtCloseTime, nDivCount, &psaAmounts, &psaDates, &nRetCount);

								saAmounts.Attach(psaAmounts);
								saDates.Attach(psaDates);
							}
						}

					}
					break;
				case enDivStockBasket:
					{
						VARIANT_BOOL bIsBasket = VARIANT_FALSE;

						IIndexAtomPtr spBasketIndex;
						_CHK(spUnd->get_BasketIndex(&spBasketIndex));

						if(spBasketIndex != NULL)
						{
							_CHK((spBasketIndex->get_IsBasket(&bIsBasket)));
							if(bIsBasket)
							{
								IEtsIndexDivCollPtr spBasketDivs;
								spBasketIndex->get_BasketDivs(&spBasketDivs);
								nDivCount = 0;
								LPSAFEARRAY psaAmounts = NULL;
								LPSAFEARRAY psaDates = NULL;

								spBasketDivs->GetDividends2(dtCalcDate, dtExpiryOV, dtCloseTime, nDivCount, &psaAmounts, &psaDates, &nDivCount);

								saAmounts.Attach(psaAmounts);
								saDates.Attach(psaDates);
							}
						}
					}
					break;
				case enDivIndexYield:
					{
						_CHK(spUnd->get_Yield(&dYield));
					}
					break;	
				}
			}
			else if(spSynthRoot != NULL) // synthetic
			{
				VARIANT_BOOL bIsBasket = spSynthRoot->Basket;

				dSkew  = spSynthRoot->Skew;
				dKurt  = spSynthRoot->Kurt;

				if(bIsBasket)
				{
					dYield  = 0.0;
					IEtsIndexDivCollPtr spBasketDivs;
					LONG nBaskDivCount = 0L;
					spSynthRoot->get_BasketDivs(&spBasketDivs);
					spBasketDivs->get_Count(&nBaskDivCount);

					if((spBasketDivs != NULL)&&(nBaskDivCount > 0L))
					{
						nDivCount = 0;

						LPSAFEARRAY psaAmounts = NULL;
						LPSAFEARRAY psaDates = NULL;

						spBasketDivs->GetDividends2(dtCalcDate, dtExpiryOV, dtCloseTime, nDivCount, &psaAmounts, &psaDates, &nDivCount);

						saAmounts.Attach(psaAmounts);
						saDates.Attach(psaDates);
					}
				}
				else
					dYield = spSynthRoot->Yield;
			}

			
			
			bool bExercised = false;
			bool bExpired = false;

			if ( bSimulate ){
				if ( dtCalcDate > dtExpiryOV ) {
					LONG nUndID = BAD_LONG_VALUE;
					ISynthRootCompCollPtr spSRCompColl;
					ISynthRootCompAtomPtr spSRComp;
					
					DOUBLE dWeight = 1;
					if(spSynthRoot != NULL)
					{
						_CHK(spUnd->get_ID(&nUndID));
						
						spSRCompColl = spSynthRoot->SynthRootComponents;
						if(spSRCompColl != NULL)
						{
							spSRComp = spSRCompColl->Item[nUndID];
							if(spSRComp != NULL)
								dWeight *= spSRComp->Weight;
						}
					}
						
					if(enUndType == enCtIndex)
						m_dDelta = 0;
					else
					{
						if((m_enOptType == enOtPut && dUndMidPrice < m_dStrike) || ((m_enOptType == enOtCall && dUndMidPrice > m_dStrike)))
							m_dDelta = ( m_enOptType == enOtCall ? 1 : -1 );
						else
							m_dDelta = 0;						
					}
					m_dDeltaInShares = m_dDelta * m_nQtyInShares * dWeight;


					if(m_spSynthGreeks)
					{
						LONG nSynthUndID;

						CComObject<CMmRvSynthGreeksColl>* pSynthGreeksColl = dynamic_cast<CComObject<CMmRvSynthGreeksColl>*>(m_spSynthGreeks.GetInterfacePtr());
						CMmRvSynthGreeksColl::EnumIterType itSynthGreeks	= pSynthGreeksColl->m_coll.begin();
						CMmRvSynthGreeksColl::EnumIterType itSynthGreeksEnd = pSynthGreeksColl->m_coll.end();

						for(; itSynthGreeks != itSynthGreeksEnd; ++itSynthGreeks)
						{
							IMmRvSynthGreeksAtomPtr spSynthGreeks = itSynthGreeks->second;

							if(spSynthGreeks != NULL)
							{
								_CHK(spSynthGreeks->get_SynthUndID(&nSynthUndID));
								spSRComp = spSRCompColl->Item[nSynthUndID];
								if(spSRComp != NULL)
								{
									DOUBLE dWeight = spSRComp->Weight;
									if(m_dDelta > BAD_DOUBLE_VALUE)
									{
										_CHK(spSynthGreeks->put_DeltaInShares(m_dDelta * m_nQtyInShares * dWeight));
										if(dUndPrice > DBL_EPSILON)
											_CHK(spSynthGreeks->put_DeltaInMoney(m_dDelta * m_nQtyInShares * dWeight * dUndPrice));
									}
								}
								else if(nSynthUndID == 0L) // USD_ID
								{
									DOUBLE dCashValue = spSynthRoot->CashValue;
									if(m_dDelta > BAD_DOUBLE_VALUE)
									{
										_CHK(spSynthGreeks->put_DeltaInShares(m_dDelta * m_nQtyInShares * dCashValue));
										if(dUndPrice > DBL_EPSILON)
											_CHK(spSynthGreeks->put_DeltaInMoney(m_dDelta * m_nQtyInShares * dUndPrice * dCashValue));
									}
								}
							}
						}
					}

					//calc Pnl
					{
						DOUBLE dUndPriceBid = BAD_DOUBLE_VALUE;
						DOUBLE dUndPriceAsk = BAD_DOUBLE_VALUE;
						DOUBLE dUndPriceLast = BAD_DOUBLE_VALUE;

						_CHK(spUnd->get_PriceBid(&dUndPriceBid));
						_CHK(spUnd->get_PriceAsk(&dUndPriceAsk));
						_CHK(spUnd->get_PriceLast(&dUndPriceLast));

						EtsReplacePriceStatusEnum enUndPriceStatusBid = enRpsNone;
						EtsReplacePriceStatusEnum enUndPriceStatusAsk = enRpsNone;

						DOUBLE dUndPriceBidForPnL = spUndPriceProfile->GetUndPriceBidForPnL(dUndPriceBid, dUndPriceAsk,
								dUndPriceLast, dUndPriceTolerance, enPriceRoundingRule, &enUndPriceStatusBid);

						DOUBLE dUndPriceAskForPnL = spUndPriceProfile->GetUndPriceAskForPnL(dUndPriceBid, dUndPriceAsk,
								dUndPriceLast, dUndPriceTolerance, enPriceRoundingRule, &enUndPriceStatusAsk);

						dUndPriceBidForPnL += ( dUndPriceBidForPnL/100 * static_cast<double>(m_simulationParams.m_dSpotChangePercent) );
						dUndPriceAskForPnL += ( dUndPriceAskForPnL/100 * static_cast<double>(m_simulationParams.m_dSpotChangePercent) );

						_CalcPnlTheoExerc(dUndPrice, dUndPriceBidForPnL, dUndPriceAskForPnL, bIsPnlLTD);
					}
						return S_OK;
				}
			}			

			// calc vola
			if(bUseTheoVolatility)
				dVola = m_dVola;
			else
			{
				dVola = 0.;
				if(!bUseTheoVolaNoBid || bUseTheoVolaNoBid && m_dPriceBid > DBL_EPSILON) 
				{
					if(dOptPriceMid > DBL_EPSILON)
					{
						LONG nFlag = VF_OK;
						dVola = ::CalcVolatilityMM3(m_dRate, dYield, m_dHTBRate, dUndMidPrice, dOptPriceMid, m_dStrike, 
							dYTE, m_enOptType, nIsAmerican, nDivCount,
							saAmounts.GetPlainData(), saDates.GetPlainData(), 100L, dSkew, dKurt, nModel, &nFlag);


						if(bUseTheoVolaBadMarket && nFlag != VF_OK)
							dVola = m_dVola;
					}
					else if (bUseTheoVolaBadMarket)
						dVola = m_dVola;
				}
				else
					dVola = m_dVola;
			}

			if ( bSimulate ) {
				dVola += ( m_dVola/100 * m_simulationParams.m_dVolaChangePercent );
				if( bUseTheoVolatility )
					m_dVola = dVola;	// save for custom variable engine
			}


			// calc greeks
			nRetCount = ::CalcGreeksMM2(m_dRate, dYield, m_dHTBRate, dUndMidPrice, m_dStrike, dVola, dYTE,
				m_enOptType, nIsAmerican, nDivCount, saAmounts.GetPlainData(), saDates.GetPlainData(), 100L, dSkew, dKurt, nModel, &aGreeks);


			if(nRetCount != 0L)
			{
				if ((aGreeks.dTheoPrice > BAD_DOUBLE_VALUE)&&(aGreeks.dTheoPrice < 0))
					aGreeks.dTheoPrice = 0;

				if((aGreeks.nMask & GT_THEOPRICE) && _finite(aGreeks.dTheoPrice) && aGreeks.dTheoPrice >= 0.)
				
					m_dPriceTheo = aGreeks.dTheoPrice;
				if((aGreeks.nMask & GT_DELTA) && _finite(aGreeks.dDelta))
					m_dDelta = aGreeks.dDelta;
				if((aGreeks.nMask & GT_GAMMA) && _finite(aGreeks.dGamma))
					m_dGamma = aGreeks.dGamma;
				if((aGreeks.nMask & GT_THETA) && _finite(aGreeks.dTheta))
					m_dTheta = aGreeks.dTheta;
				if((aGreeks.nMask & GT_VEGA) && _finite(aGreeks.dVega))
					m_dVega = aGreeks.dVega;
				if((aGreeks.nMask & GT_RHO) && _finite(aGreeks.dRho))
					m_dRho = aGreeks.dRho;
				if((aGreeks.nMask & GT_DELTA_VEGA) && _finite(aGreeks.dDeltaVega))
					m_dVegaDelta = aGreeks.dDeltaVega;
				if((aGreeks.nMask & GT_GAMMA_VEGA) && _finite(aGreeks.dGammaVega))
					m_dVegaGamma = aGreeks.dGammaVega;
				if((aGreeks.nMask & GT_DELTA_THETA) && _finite(aGreeks.dDeltaTheta))
					m_dThetaDelta = aGreeks.dDeltaTheta;
				if((aGreeks.nMask & GT_GAMMA_THETA) && _finite(aGreeks.dGammaTheta))
					m_dThetaGamma = aGreeks.dGammaTheta;
				if((aGreeks.nMask & GT_VOLGA) && _finite(aGreeks.dVolga))
					m_dVolga = aGreeks.dVolga;
			}

			// calc MIV
			{
				double dMIV = 0.;
				double dOptBid = m_dPriceBid;
				double dOptAsk = m_dPriceAsk;
				double dOptLast = m_dPriceLast;
				double dOptTheo = m_dPriceTheo;
				EtsOptionTypeEnum enOptType = m_enOptType;

				bool bUseOpposite = false;

				switch(enFitToImpCurve)
				{
				case enFitToImpCurveCalls:
					if(m_enOptType == enOtPut)
						bUseOpposite = true;
					break;

				case enFitToImpCurvePuts:
					if(m_enOptType == enOtCall)
						bUseOpposite = true;
					break;

				case enFitToImpCurveOTM:
					if(m_enOptType == enOtCall)
						bUseOpposite = (m_dStrike < dUndMidPrice);
					else
						bUseOpposite = (m_dStrike >= dUndMidPrice);
					break;

					//default:
					//break;
				}

				if(bUseOpposite)
				{
					dOptBid = m_dOppPriceBid;
					dOptAsk = m_dOppPriceAsk;
					dOptLast = m_dOppPriceLast;

					enOptType = (m_enOptType == enOtPut ? enOtCall : enOtPut);
				}

				if((bFitToImpFlatNoBid && DoubleLEQZero(dOptBid)) || enFitToImpCurve == enFitToImpCurveATM)
				{
					IVolaControlPtr spVolaSrv;
					_CHK(spUnd->get_VolaSrv(&spVolaSrv));
					LONG lSurfaceID = spVolaSrv->GetSurfaceByRoot(m_nOptionRootID);
					dMIV = spVolaSrv->GetOptionVola(m_dtExpiryOV, dUndMidPrice, lSurfaceID);
				}
				else
				{
					if(bUseOpposite)
					{
						GREEKS aOppGreeks;
						memset(&aOppGreeks, 0, sizeof(aOppGreeks));
						aOppGreeks.nMask = GT_THEOPRICE;

						nRetCount = ::CalcGreeksMM2(m_dRate, dYield, m_dHTBRate, dUndMidPrice, m_dStrike, dVola, dYTE,
										enOptType, nIsAmerican, nDivCount, saAmounts.GetPlainData(), saDates.GetPlainData(), 
										100L, dSkew, dKurt, nModel, &aOppGreeks);
						//m_PerfObj->CalcOptionGreeks();

						if(nRetCount != 0L && _finite(aOppGreeks.dTheoPrice) && aOppGreeks.dTheoPrice >= 0.)
							dOptTheo = aOppGreeks.dTheoPrice;
					}

					EtsReplacePriceStatusEnum enOptPriceStatus = enRpsNone;
					double dOptPrice = 0.;
					if(spOptPriceProfile != NULL)
						dOptPrice = spOptPriceProfile->GetOptPriceMid(dOptBid, dOptAsk, dOptLast, enPriceRoundingRule, 
																	  bUseTheoVolatility, dOptTheo, &enOptPriceStatus);

					if(DoubleGTZero(dOptPrice))
					{
						LONG nFlag = VF_OK;
						dMIV = ::CalcVolatilityMM3(m_dRate, dYield, m_dHTBRate, dUndMidPrice, dOptPrice, m_dStrike, 
							dYTE, enOptType, nIsAmerican, nDivCount,
							saAmounts.GetPlainData(), saDates.GetPlainData(), 100L, 
							dSkew, dKurt, nModel, &nFlag);

						//m_PerfObj->CalcOptionVolatility();
					}
				}
				m_dMIV = dMIV;
			}

		}

		// clear values in shares
		__MmRvPosAtom::ClearValuesInShares();
		if(m_bIsSynthetic && m_spSynthGreeks != NULL)
		{
			CComObject<CMmRvSynthGreeksColl>* pSynthGreeksColl = dynamic_cast<CComObject<CMmRvSynthGreeksColl>*>(m_spSynthGreeks.GetInterfacePtr());
			CMmRvSynthGreeksColl::EnumIterType itSynthGreeks	= pSynthGreeksColl->m_coll.begin();
			CMmRvSynthGreeksColl::EnumIterType itSynthGreeksEnd = pSynthGreeksColl->m_coll.end();

			for(; itSynthGreeks != itSynthGreeksEnd; ++itSynthGreeks)
			{
				IMmRvSynthGreeksAtomPtr spSynthGreeks = itSynthGreeks->second;
				if(spSynthGreeks != NULL)
					_CHK(spSynthGreeks->ClearValuesInShares());
			}
		}

		// calc values in shares
		EtsReplacePriceStatusEnum enPriceStatusBid = enRpsNone;
		DOUBLE dOptPriceBid = spOptPriceProfile->GetOptPriceBidForPnL(m_dPriceBid, m_dPriceAsk, m_dPriceLast, enPriceRoundingRule, bUseTheoVolatility, m_dPriceTheo, &enPriceStatusBid);

		EtsReplacePriceStatusEnum enPriceStatusAsk = enRpsNone;
		DOUBLE dOptPriceAsk = spOptPriceProfile->GetOptPriceAskForPnL(m_dPriceBid, m_dPriceAsk, m_dPriceLast, enPriceRoundingRule, bUseTheoVolatility, m_dPriceTheo, &enPriceStatusAsk);

		enPriceStatusMid = enRpsNone;
		dOptPriceMid = spOptPriceProfile->GetOptPriceMid(m_dPriceBid, m_dPriceAsk, m_dPriceLast, enPriceRoundingRule, bUseTheoVolatility, m_dPriceTheo, &enPriceStatusMid);
		m_dOptPrice = dOptPriceMid;
		m_enReplacePriceStatus = static_cast<EtsReplacePriceStatusEnum>(enPriceStatusMid | enPriceStatusBid | enPriceStatusAsk);

		

		// pnl
		if (m_bIsFlex == VARIANT_FALSE)
			_CalcPnlMtm(bIsPnlLTD, dOptPriceBid, dOptPriceAsk, false/*spOptPriceProfile->GetBadOptSinglePriceRule() == enObsprReplaceWithZero*/);
		else
			_CalcPnlMtm(bIsPnlLTD, m_dPriceTheo, m_dPriceTheo, false/*spOptPriceProfile->GetBadOptSinglePriceRule() == enObsprReplaceWithZero*/);

		_CalcPnlTheo(bIsPnlLTD);

		// delta & gamma
		if(m_dDelta > BAD_DOUBLE_VALUE || m_dGamma > BAD_DOUBLE_VALUE)
		{
			if(!m_bIsSynthetic || m_spSynthGreeks == NULL || spSynthRoot == NULL)
			{
				if(m_dDelta > BAD_DOUBLE_VALUE)
					m_dDeltaInShares = m_dDelta * m_nQtyInShares;

				if(m_dGamma > BAD_DOUBLE_VALUE)
				{
					m_dGammaInShares = m_dGamma * m_nQtyInShares;

					if(dUndMidPrice > DBL_EPSILON)
						m_dGammaInSharesPerc = m_dGamma * m_nQtyInShares * dUndMidPrice / 100.;

					m_dNetGamma = m_dGamma * m_nQtyInShares  / 100.;

					if(dUndMidPrice > DBL_EPSILON)
						m_dNetGamma = m_dGamma * m_nQtyInShares  * dUndMidPrice * dUndMidPrice / 100.;

				}


			}
			else // synthetic
			{
				LONG nUndID = 0L, nSynthUndID = 0L;
				_CHK(spUnd->get_ID(&nUndID));
				ISynthRootCompAtomPtr spSRComp;
				ISynthRootCompCollPtr spSRCompColl;

				spSRCompColl = spSynthRoot->SynthRootComponents;
				if(spSRCompColl != NULL)
				{
					spSRComp = spSRCompColl->Item[nUndID];
					if(spSRComp != NULL)
					{
						if(m_dDelta > BAD_DOUBLE_VALUE)
							m_dDeltaInShares = m_dDelta * m_nQtyInShares * spSRComp->Weight;

						if(m_dGamma > BAD_DOUBLE_VALUE)
							m_dGammaInShares = m_dGamma * m_nQtyInShares * spSRComp->Weight;


					}

					if(dUndPrice > DBL_EPSILON && m_dGammaInShares > BAD_DOUBLE_VALUE)
						m_dGammaInSharesPerc = m_dGammaInShares * dUndPrice / 100.;

					CComObject<CMmRvSynthGreeksColl>* pSynthGreeksColl = dynamic_cast<CComObject<CMmRvSynthGreeksColl>*>(m_spSynthGreeks.GetInterfacePtr());
					CMmRvSynthGreeksColl::EnumIterType itSynthGreeks	= pSynthGreeksColl->m_coll.begin();
					CMmRvSynthGreeksColl::EnumIterType itSynthGreeksEnd = pSynthGreeksColl->m_coll.end();

					for(; itSynthGreeks != itSynthGreeksEnd; ++itSynthGreeks)
					{
						IMmRvSynthGreeksAtomPtr spSynthGreeks = itSynthGreeks->second;

						if(spSynthGreeks != NULL)
						{
							DOUBLE dGammaInShares = BAD_DOUBLE_VALUE;

							_CHK(spSynthGreeks->get_SynthUndID(&nSynthUndID));

							IMmRvUndAtomPtr spUnd2;
							_CHK(spUndColl->get_Item(nSynthUndID, &spUnd2));
							if(spUnd2 != NULL)
							{
								IEtsPriceProfileAtomPtr spUndPriceProfile2;
								_CHK(spUnd2->get_UndPriceProfile(&spUndPriceProfile2));

								enPriceStatusMid = enRpsNone;
								DOUBLE dUndPriceBid = 0., dUndPriceAsk = 0., dUndPriceLast = 0.;
								_CHK(spUnd2->get_PriceBid(&dUndPriceBid));
								_CHK(spUnd2->get_PriceAsk(&dUndPriceAsk));
								_CHK(spUnd2->get_PriceLast(&dUndPriceLast));

								dUndPrice = spUndPriceProfile2->GetUndPriceMid(dUndPriceBid, dUndPriceAsk,
									dUndPriceLast, dUndPriceTolerance, enPriceRoundingRule, &enPriceStatusMid, VARIANT_FALSE);

								_CHK(spUnd2->put_ReplacePriceStatus(enPriceStatusMid));
							}

							//_CHK(spSynthGreeks->get_SynthUndID(&nSynthUndID));
							spSRComp = spSRCompColl->Item[nSynthUndID];
							if(spSRComp != NULL)
							{
								DOUBLE dWeight = spSRComp->Weight;
								if(m_dDelta > BAD_DOUBLE_VALUE)
								{
									_CHK(spSynthGreeks->put_DeltaInShares(m_dDelta * m_nQtyInShares * dWeight));
									if(dUndPrice > DBL_EPSILON)
										_CHK(spSynthGreeks->put_DeltaInMoney(m_dDelta * m_nQtyInShares * dWeight * dUndPrice));
								}

								if(m_dGamma > BAD_DOUBLE_VALUE)
								{
									dGammaInShares = m_dGamma * m_nQtyInShares * dWeight;
									_CHK(spSynthGreeks->put_GammaInShares(dGammaInShares));
								}
							}
							else if(nSynthUndID == 0L) // USD_ID
							{
								DOUBLE dCashValue = spSynthRoot->CashValue;
								if(m_dDelta > BAD_DOUBLE_VALUE)
								{
									_CHK(spSynthGreeks->put_DeltaInShares(m_dDelta * m_nQtyInShares * dCashValue));
									if(dUndPrice > DBL_EPSILON)
										_CHK(spSynthGreeks->put_DeltaInMoney(m_dDelta * m_nQtyInShares * dUndPrice * dCashValue));
								}

								if(m_dGamma > BAD_DOUBLE_VALUE)
								{
									dGammaInShares = m_dGamma * m_nQtyInShares * dCashValue;
									_CHK(spSynthGreeks->put_GammaInShares(dGammaInShares));
								}
							}

							if(m_dGamma > BAD_DOUBLE_VALUE)
							{
								//IMmRvUndAtomPtr spUnd2;
								//_CHK(spUndColl->get_Item(nSynthUndID, &spUnd2));
								//if(spUnd2 != NULL)
								//{
								//	IEtsPriceProfileAtomPtr spUndPriceProfile2;
								//	_CHK(spUnd2->get_UndPriceProfile(&spUndPriceProfile2));

								//	enPriceStatusMid = enRpsNone;
								//	DOUBLE dUndPriceBid = 0., dUndPriceAsk = 0., dUndPriceLast = 0.;
								//	_CHK(spUnd2->get_PriceBid(&dUndPriceBid));
								//	_CHK(spUnd2->get_PriceAsk(&dUndPriceAsk));
								//	_CHK(spUnd2->get_PriceLast(&dUndPriceLast));

								//	dUndPrice = spUndPriceProfile2->GetUndPriceMid(dUndPriceBid, dUndPriceAsk,
								//			dUndPriceLast, dUndPriceTolerance, enPriceRoundingRule, &enPriceStatusMid, TRUE);

								//	_CHK(spUnd2->put_ReplacePriceStatus(enPriceStatusMid));

									if(dUndPrice > DBL_EPSILON && dGammaInShares > BAD_DOUBLE_VALUE)
									{
										_CHK(spSynthGreeks->put_GammaInSharesPerc(dGammaInShares * dUndPrice / 100.));
										_CHK(spSynthGreeks->put_NetGamma(dGammaInShares * dUndPrice * dUndPrice / 100));
									}
								//}
							}
						}
					}
				}
				if(dUndMidPrice > DBL_EPSILON)
					m_dNetGamma = m_dGammaInShares * sUndPriceSave * sUndPriceSave / 100.;

			}
		}

		// other theos
		if(m_dVega > BAD_DOUBLE_VALUE)
		{
			m_dVegaInShares = m_dVega * m_nQtyInShares;
			m_dWtdVega = m_dVegaInShares * m_dVegaWeight;
		}

		if(m_dTheta > BAD_DOUBLE_VALUE)
			m_dThetaInShares = m_dTheta * m_nQtyInShares;

		if(m_dRho > BAD_DOUBLE_VALUE)
			m_dRhoInShares = m_dRho * m_nQtyInShares;

		if(m_dVegaDelta > BAD_DOUBLE_VALUE)
			m_dVegaDeltaInShares = m_dVegaDelta * m_nQtyInShares;

		if(m_dVegaGamma > BAD_DOUBLE_VALUE && dUndMidPrice > DBL_EPSILON)
			m_dVegaGammaInShares = m_dVegaGamma * m_nQtyInShares * dUndMidPrice / 100.;

		if(m_dThetaDelta > BAD_DOUBLE_VALUE)
			m_dThetaDeltaInShares = m_dThetaDelta * m_nQtyInShares;

		if(m_dThetaGamma > BAD_DOUBLE_VALUE && dUndMidPrice > DBL_EPSILON)
			m_dThetaGammaInShares = m_dThetaGamma * m_nQtyInShares * dUndMidPrice / 100.;


		DOUBLE dOptPricePnL = 0.;
		VARIANT_BOOL bUseMidMarketForPnL = spOptPriceProfile->UseMidMarketForPnL;
		VARIANT_BOOL bUseZeroBidRule = spOptPriceProfile->UseZeroBidRule;
		if (bUseMidMarketForPnL == VARIANT_TRUE && bUseZeroBidRule == VARIANT_FALSE)
			dOptPricePnL = dOptPriceMid;
		else
		{
			if(m_nQty > 0.)
				dOptPricePnL = spOptPriceProfile->GetOptPriceBidForPnL(m_dPriceBid, m_dPriceAsk, m_dPriceLast, enPriceRoundingRule, bUseTheoVolatility, m_dPriceTheo, NULL);
			else
				dOptPricePnL = spOptPriceProfile->GetOptPriceAskForPnL(m_dPriceBid, m_dPriceAsk, m_dPriceLast, enPriceRoundingRule, bUseTheoVolatility, m_dPriceTheo, NULL);
		}

		if(dUndMidPrice > DBL_EPSILON && dOptPricePnL >= 0/*DBL_EPSILON*/)
		{
//			formula for time value:
//			for calls strike >=spot: time value= call price
//			for calls strike <spot, time value= max(0, call price -(spot-strike))
//			for puts strike >spot: time value= max(0, put price -(strike-spot))
//			for puts strike <=spot, time value= put price
			if(m_enOptType == enOtCall)
			{
				if(m_dStrike >= dUndMidPrice)
					m_dTimeValue = dOptPricePnL;
				else
					m_dTimeValue = max(0.0, dOptPricePnL - (dUndMidPrice-m_dStrike));
			}
			else
			{
				if(m_dStrike <= dUndMidPrice)
					m_dTimeValue = dOptPricePnL;
				else
					m_dTimeValue = max(0.0, dOptPricePnL- (m_dStrike-dUndMidPrice));
			}
//			m_dTimeValue = dOptPriceMid - (m_enOptType == enOtCall
//							? (dUndMidPrice - m_dStrike > DBL_EPSILON ? dUndMidPrice - m_dStrike : 0.)
//							: (m_dStrike - dUndMidPrice > DBL_EPSILON ? m_dStrike - dUndMidPrice : 0.));
		}

		if(m_dNetGamma <= BAD_DOUBLE_VALUE)
		{
			m_dNetGamma = m_dGamma * m_nQtyInShares  / 100.;
			if(dUndMidPrice > DBL_EPSILON)
				m_dNetGamma = m_dGamma * m_nQtyInShares  * dUndMidPrice * dUndMidPrice / 100.;

		}

		/*if ( m_spRTContext != NULL && m_nPoolID != BAD_LONG_VALUE ) 
			_CHK(m_spRTContext->Recalculate(m_nPoolID));*/


		if ( m_spSimulation != NULL && aSimulatedUndColl && bCalcSimulation &&
				!( 
					(m_simulationParams.m_dDate == dtCalcDate || m_simulationParams.m_dDate == 0)
					&& m_simulationParams.m_dSpotChangePercent == 0 
					&& m_simulationParams.m_dVolaChangePercent == 0 
				)
			){
			
			m_spSimulation->CalcOptionGreeks(  aSimulatedUndColl,
															aUnd,
															dUndMidPrice,
															nMask,
															bIsPnlLTD,
															bFitToImpFlatNoBid,
															enFitToImpCurve,
															enCalcModel,
															bUseTheoVolatility,
															bUseTheoVolaNoBid,
															bUseTheoVolaBadMarket,
															dUndPriceTolerance,
															enPriceRoundingRule,
															NULL,
															VARIANT_TRUE,
															VARIANT_FALSE,
															pParams);
		}

		m_bIsRecalculated = VARIANT_TRUE;
		//_CalcSleep(10, 1);
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_IMmRvPosAtom, e.Error());
	}
	_CATCH_UNHANDLED_EXCEPTION_RETURN_COM;

	return S_OK;
}

/////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CMmRvPosAtom::CalcFutOptionGreeks(IMmRvUndAtom* aUnd,
											DOUBLE dUndMidPrice,
											LONG nMask,
											VARIANT_BOOL bIsPnlLTD,
											EtsCalcModelTypeEnum enCalcModel,
											VARIANT_BOOL bUseTheoVolatility,
											VARIANT_BOOL bUseTheoVolaNoBid,
											VARIANT_BOOL bUseTheoVolaBadMarket,
											DOUBLE dUndPriceTolerance,
											EtsPriceRoundingRuleEnum enPriceRoundingRule,
											VARIANT_BOOL bSimulate,
											VARIANT_BOOL bCalcSimulation,
											ICalculationParametrs* pParams)
{
	if(aUnd == NULL || pParams == NULL)
		return Error(L"Invalid object passed.", IID_IMmRvPosAtom, E_INVALIDARG);

	if(m_enContractType != enCtFutOption)
		return S_OK;

	if(!bSimulate)
	{
		m_simulationParams.m_dDate = 0;
		m_simulationParams.m_dSpotChangePercent = 0.;
		m_simulationParams.m_dVolaChangePercent = 0.;
	}
	
	DOUBLE dtNYTime;
	::GetNYDateTimeAsDATE(&dtNYTime);
	DOUBLE dtCalcDate = bSimulate ? dtCalcDate = m_simulationParams.m_dDate : dtNYTime;

	try
	{
		IMmRvUndAtomPtr spUnd(aUnd);
		IEtsPriceProfileAtomPtr spUndPriceProfile;
		IEtsPriceProfileAtomPtr spOptPriceProfile;

		_CHK(m_spFut->get_UndPriceProfile(&spUndPriceProfile));
		_CHK(m_spFut->get_OptPriceProfile(&spOptPriceProfile));

		EtsReplacePriceStatusEnum enPriceStatusMid = enRpsNone;
		DOUBLE dFutMidPrice = 0., dOptPriceMid = 0.;
		DOUBLE dFutPriceBid = 0., dFutPriceAsk = 0., dFutPriceLast = 0.;

		_CHK(m_spFut->get_PriceBid(&dFutPriceBid));
		_CHK(m_spFut->get_PriceAsk(&dFutPriceAsk));
		_CHK(m_spFut->get_PriceLast(&dFutPriceLast));

		dFutMidPrice = spUndPriceProfile->GetUndPriceMid(dFutPriceBid, dFutPriceAsk, dFutPriceLast, dUndPriceTolerance, enPriceRoundingRule, NULL, VARIANT_FALSE);
		if ( bSimulate ) {
			dFutMidPrice += ( dFutMidPrice /100 * static_cast<double>(m_simulationParams.m_dSpotChangePercent) );
			dFutPriceAsk += ( dFutPriceAsk /100 * static_cast<double>(m_simulationParams.m_dSpotChangePercent) );
			dFutPriceBid += ( dFutPriceBid /100 * static_cast<double>(m_simulationParams.m_dSpotChangePercent) );

		}
		dOptPriceMid = spOptPriceProfile->GetOptPriceMid(m_dPriceBid, m_dPriceAsk, m_dPriceLast, enPriceRoundingRule, bUseTheoVolatility, 0., &enPriceStatusMid);

		m_enReplacePriceStatus = enPriceStatusMid;

		CSafeArrayWrapper<double> saDates;
		CSafeArrayWrapper<double> saAmounts;

		DOUBLE dYield = 0;
		LPVOID lpDateData = NULL;
		LPVOID lpAmountData = NULL;
		IEtsIndexDivAtomPtr			spDividend = NULL;
		IIndexAtomPtr				spBasketIndex = NULL;
		EtsContractTypeEnum			m_enUndType = enCtStock;
		if (spUnd != NULL)
		{
			spUnd->get_Dividend(&spDividend);
			spUnd->get_ContractType(&m_enUndType);
			spUnd->get_BasketIndex(&spBasketIndex);
		}

		if(nMask != GT_NOTHING && dFutMidPrice > DBL_EPSILON)
		{
			GREEKS aGreeks;
			memset(&aGreeks, 0, sizeof(aGreeks));
			aGreeks.nMask = nMask;
			__MmRvPosAtom::ClearValues();

			LONG nModel = static_cast<LONG>(enCalcModel);
			VARIANT_BOOL bIsAmerican = VARIANT_FALSE;
			_CHK(m_spFut->get_IsAmerican(&bIsAmerican));
			LONG nIsAmerican = (bIsAmerican ? 1L : 0L);

			DOUBLE dVola = 0.;
			LONG nRetCount = 0L;

			DOUBLE dtExpiry = m_dtExpiry;
			DOUBLE dtExpiryOV = m_dtExpiryOV;
			DOUBLE dtCloseTime = m_dtTradingClose;
			DOUBLE dYTE;

			::GetCalculationParams(dtCalcDate, dtExpiryOV, dtCloseTime, pParams->UseTimePrecision != VARIANT_FALSE, &dtCalcDate, &dtExpiryOV, &dtCloseTime, &dYTE);

			DOUBLE dSkew = 0., dKurt = 0.;
			_CHK(spUnd->get_Skew(&dSkew));
			_CHK(spUnd->get_Kurt(&dKurt));

			////////////////////////
			LONG nDivCount = 0L;
			nRetCount = 0L;

			EtsDivTypeEnum enDivType = enDivCustomStream;
			IEtsIndexDivAtomPtr spDiv;
			_CHK(spUnd->get_Dividend(&spDiv));
			if(spDiv)
				spDiv->get_DivType(&enDivType);

			switch(enDivType)
			{
			case enDivMarket:
			case enDivCustomPeriodical:
			case enDivCustomStream:
				{
					nDivCount = 0;

					if (spDiv != NULL)
					{
						spDiv->GetDividendCount2(dtCalcDate, dtExpiryOV, dtCloseTime, &nDivCount);
						if(nDivCount > 0L) 
						{
							LPSAFEARRAY psaAmounts = NULL;
							LPSAFEARRAY psaDates = NULL;

							spDiv->GetDividends2(dtCalcDate, dtExpiryOV, dtCloseTime, nDivCount, &psaAmounts, &psaDates, &nRetCount);

							saAmounts.Attach(psaAmounts);
							saDates.Attach(psaDates);
						}
					}

				}
				break;
			case enDivStockBasket:
				{
					VARIANT_BOOL bIsBasket = VARIANT_FALSE;

					IIndexAtomPtr spBasketIndex;
					_CHK(spUnd->get_BasketIndex(&spBasketIndex));

					if(spBasketIndex != NULL)
					{
						_CHK((spBasketIndex->get_IsBasket(&bIsBasket)));
						if(bIsBasket)
						{
							IEtsIndexDivCollPtr spBasketDivs;
							spBasketIndex->get_BasketDivs(&spBasketDivs);
							nDivCount = 0;
							LPSAFEARRAY psaAmounts = NULL;
							LPSAFEARRAY psaDates = NULL;

							spBasketDivs->GetDividends2(dtCalcDate, dtExpiryOV, dtCloseTime, nDivCount, &psaAmounts, &psaDates, &nDivCount);

							saAmounts.Attach(psaAmounts);
							saDates.Attach(psaDates);
						}
					}
				}
				break;
			case enDivIndexYield:
				{
					_CHK(spUnd->get_Yield(&dYield));
				}
				break;	
			}


			
			if(nDivCount < 0)
				nDivCount = 0;

			/*if ( bSimulate ){
				if ( nToday >= nExpiry ) {
					// check would be this option exercised?
					if ( (m_enOptType == enOtCall && dUndMidPrice < m_dStrike) ||(m_enOptType == enOtPut && dUndMidPrice > m_dStrike) ){
						m_dDelta = ( m_enOptType == enOtCall ? 1 : -1 );
						m_dDeltaInShares = m_dDelta * m_nQtyInShares;
						return S_OK;
					}
					else
						return S_OK;
				}
			}*/
			if ( bSimulate ){
				if ( dtCalcDate >= dtExpiryOV ) {
					DOUBLE dWeight = 1;
					EtsContractTypeEnum enUndType = enCtNone;
					_CHK(spUnd->get_ContractType(&enUndType));
					
					if(enUndType == enCtIndex)
						m_dDelta = 0;
					else
					{
						if((m_enOptType == enOtPut && dUndMidPrice < m_dStrike) || ((m_enOptType == enOtCall && dUndMidPrice > m_dStrike)))
							m_dDelta = ( m_enOptType == enOtCall ? 1 : -1 );
						else
							m_dDelta = 0;						
					}
					m_dDeltaInShares = m_dDelta * m_nQtyInShares * dWeight;

					//calc Pnl
					{
						DOUBLE dUndPriceBid = BAD_DOUBLE_VALUE;
						DOUBLE dUndPriceAsk = BAD_DOUBLE_VALUE;
						DOUBLE dUndPriceLast = BAD_DOUBLE_VALUE;

						_CHK(spUnd->get_PriceBid(&dUndPriceBid));
						_CHK(spUnd->get_PriceAsk(&dUndPriceAsk));
						_CHK(spUnd->get_PriceLast(&dUndPriceLast));

						EtsReplacePriceStatusEnum enUndPriceStatusBid = enRpsNone;
						EtsReplacePriceStatusEnum enUndPriceStatusAsk = enRpsNone;

						DOUBLE dUndPriceBidForPnL = spUndPriceProfile->GetUndPriceBidForPnL(dUndPriceBid, dUndPriceAsk,
							dUndPriceLast, dUndPriceTolerance, enPriceRoundingRule, &enUndPriceStatusBid);

						DOUBLE dUndPriceAskForPnL = spUndPriceProfile->GetUndPriceAskForPnL(dUndPriceBid, dUndPriceAsk,
							dUndPriceLast, dUndPriceTolerance, enPriceRoundingRule, &enUndPriceStatusAsk);

						dUndPriceBidForPnL += ( dUndPriceBidForPnL/100 * static_cast<double>(m_simulationParams.m_dSpotChangePercent) );
						dUndPriceAskForPnL += ( dUndPriceAskForPnL/100 * static_cast<double>(m_simulationParams.m_dSpotChangePercent) );

						_CalcPnlTheoExerc(dUndMidPrice, dUndPriceBidForPnL, dUndPriceAskForPnL, bIsPnlLTD);
					}
					return S_OK;
				}
			}			

			/////////////////////////
			// calc vola
			if(bUseTheoVolatility)
				dVola = m_dVola;
			else
			{
				dVola = 0.;
				if(!bUseTheoVolaNoBid || bUseTheoVolaNoBid && m_dPriceBid > DBL_EPSILON) 
				{
					if(dOptPriceMid > DBL_EPSILON)
					{
						LONG nFlag = VF_OK;
						dVola = ::CalcFutureOptionVolatility(m_dRate, dFutMidPrice, dOptPriceMid, m_dStrike, 
							dYTE, m_enOptType, nIsAmerican, 100L, dSkew, dKurt, nModel, &nFlag);
						

						if(bUseTheoVolaBadMarket && nFlag != VF_OK)
							dVola = m_dVola;
					}
					else if (bUseTheoVolaBadMarket)
						dVola = m_dVola;
				}
				else
					dVola = m_dVola;
			}

			if ( bSimulate ) 
				dVola += ( m_dVola/100 * m_simulationParams.m_dVolaChangePercent );

			// calc greeks
			nRetCount = ::CalcFutureOptionGreeks(m_dRate, dFutMidPrice, m_dStrike, dVola, dYTE,
				m_enOptType, nIsAmerican, 100L, dSkew, dKurt, nModel, &aGreeks);
			
			if ((aGreeks.dTheoPrice > BAD_DOUBLE_VALUE)&&(aGreeks.dTheoPrice < 0))
				aGreeks.dTheoPrice = 0;

			if (aGreeks.dTheoPrice < 0)	aGreeks.dTheoPrice = 0;
			if((aGreeks.nMask & GT_THEOPRICE) && _finite(aGreeks.dTheoPrice) && aGreeks.dTheoPrice >= 0.)
					m_dPriceTheo = aGreeks.dTheoPrice;
				if((aGreeks.nMask & GT_DELTA) && _finite(aGreeks.dDelta))
					m_dDelta = aGreeks.dDelta;
				if((aGreeks.nMask & GT_GAMMA) && _finite(aGreeks.dGamma))
					m_dGamma = aGreeks.dGamma;
				if((aGreeks.nMask & GT_THETA) && _finite(aGreeks.dTheta))
					m_dTheta = aGreeks.dTheta;
				if((aGreeks.nMask & GT_VEGA) && _finite(aGreeks.dVega))
					m_dVega = aGreeks.dVega;
				if((aGreeks.nMask & GT_RHO) && _finite(aGreeks.dRho))
					m_dRho = aGreeks.dRho;
				if((aGreeks.nMask & GT_DELTA_VEGA) && _finite(aGreeks.dDeltaVega))
					m_dVegaDelta = aGreeks.dDeltaVega;
				if((aGreeks.nMask & GT_GAMMA_VEGA) && _finite(aGreeks.dGammaVega))
					m_dVegaGamma = aGreeks.dGammaVega;
				if((aGreeks.nMask & GT_DELTA_THETA) && _finite(aGreeks.dDeltaTheta))
					m_dThetaDelta = aGreeks.dDeltaTheta;
				if((aGreeks.nMask & GT_GAMMA_THETA) && _finite(aGreeks.dGammaTheta))
					m_dThetaGamma = aGreeks.dGammaTheta;
				if((aGreeks.nMask & GT_VOLGA) && _finite(aGreeks.dVolga))
					m_dVolga = aGreeks.dVolga;
			//}
		}

		// clear values in shares
		__MmRvPosAtom::ClearValuesInShares();

		// calc values in shares
		EtsReplacePriceStatusEnum enPriceStatusBid = enRpsNone;
		DOUBLE dOptPriceBid = spOptPriceProfile->GetOptPriceBidForPnL(m_dPriceBid, m_dPriceAsk, m_dPriceLast, enPriceRoundingRule, bUseTheoVolatility, m_dPriceTheo, &enPriceStatusBid);

		EtsReplacePriceStatusEnum enPriceStatusAsk = enRpsNone;
		DOUBLE dOptPriceAsk = spOptPriceProfile->GetOptPriceAskForPnL(m_dPriceBid, m_dPriceAsk, m_dPriceLast, enPriceRoundingRule, bUseTheoVolatility, m_dPriceTheo, &enPriceStatusAsk);

		enPriceStatusMid = enRpsNone;
		dOptPriceMid = spOptPriceProfile->GetOptPriceMid(m_dPriceBid, m_dPriceAsk, m_dPriceLast, enPriceRoundingRule, bUseTheoVolatility, m_dPriceTheo, &enPriceStatusMid);

		m_enReplacePriceStatus = static_cast<EtsReplacePriceStatusEnum>(enPriceStatusMid | enPriceStatusBid | enPriceStatusAsk);

		// pnl
		_CalcPnlMtm(bIsPnlLTD, dOptPriceBid, dOptPriceAsk, false/*spOptPriceProfile->GetBadOptSinglePriceRule() == enObsprReplaceWithZero*/);
		_CalcPnlTheo(bIsPnlLTD);

		// delta
		if(m_dDelta > BAD_DOUBLE_VALUE)
			m_dDeltaInShares = m_dDelta * m_nQtyInShares;

		// gamma
		if(m_dGamma > BAD_DOUBLE_VALUE)
		{
			m_dGammaInShares = m_dGamma * m_nQtyInShares;

			if(dUndMidPrice > DBL_EPSILON)
				m_dGammaInSharesPerc = m_dGamma * m_nQtyInShares * dUndMidPrice / 100.;

			m_dNetGamma = m_dGamma * m_nQtyInShares   / 100.;
			if(dUndMidPrice > DBL_EPSILON)
					m_dNetGamma = m_dGamma * m_nQtyInShares  * dUndMidPrice * dUndMidPrice / 100.;

		}

		// other theos
		if(m_dVega > BAD_DOUBLE_VALUE)
		{
			m_dVegaInShares = m_dVega * m_nQtyInShares;
			m_dWtdVega = m_dVegaInShares * m_dVegaWeight;
		}

		if(m_dTheta > BAD_DOUBLE_VALUE)
			m_dThetaInShares = m_dTheta * m_nQtyInShares;

		if(m_dRho > BAD_DOUBLE_VALUE)
			m_dRhoInShares = m_dRho * m_nQtyInShares;

		if(m_dVegaDelta > BAD_DOUBLE_VALUE)
			m_dVegaDeltaInShares = m_dVegaDelta * m_nQtyInShares;

		if(m_dVegaGamma > BAD_DOUBLE_VALUE && dUndMidPrice > DBL_EPSILON)
			m_dVegaGammaInShares = m_dVegaGamma * m_nQtyInShares * dUndMidPrice / 100.;

		if(m_dThetaDelta > BAD_DOUBLE_VALUE)
			m_dThetaDeltaInShares = m_dThetaDelta * m_nQtyInShares;

		if(m_dThetaGamma > BAD_DOUBLE_VALUE && dUndMidPrice > DBL_EPSILON)
			m_dThetaGammaInShares = m_dThetaGamma * m_nQtyInShares * dUndMidPrice / 100.;

		if(dUndMidPrice > DBL_EPSILON && dOptPriceMid > DBL_EPSILON)
		{
			if(m_enOptType == enOtCall)
			{
				if(m_dStrike >= dUndMidPrice)
					m_dTimeValue = dOptPriceMid;
				else
					m_dTimeValue = max(0.0, dOptPriceMid - (dUndMidPrice-m_dStrike));
			}
			else
			{
				if(m_dStrike <= dUndMidPrice)
					m_dTimeValue = dOptPriceMid;
				else
					m_dTimeValue = max(0.0, dOptPriceMid- (m_dStrike-dUndMidPrice)) ;
			}

//			m_dTimeValue = dOptPriceMid - (m_enOptType == enOtCall
//							? (dFutMidPrice - m_dStrike > DBL_EPSILON ? dFutMidPrice - m_dStrike : 0.)
//							: (m_dStrike - dFutMidPrice > DBL_EPSILON ? m_dStrike - dFutMidPrice : 0.));
		}

		if(m_dNetGamma > BAD_DOUBLE_VALUE)
		{
			m_dNetGamma = m_dGamma * m_nQtyInShares  / 100.;
			if(dUndMidPrice > DBL_EPSILON)
				m_dNetGamma = m_dGamma * m_nQtyInShares  * dUndMidPrice * dUndMidPrice / 100.;
		}

		/*if ( m_spRTContext != NULL && m_nPoolID != BAD_LONG_VALUE ) 
			_CHK(m_spRTContext->Recalculate(m_nPoolID));*/

		if ( m_spSimulation != NULL && bCalcSimulation &&
					(m_simulationParams.m_dDate == dtCalcDate || m_simulationParams.m_dDate == 0)
					&& m_simulationParams.m_dSpotChangePercent == 0 
					&& m_simulationParams.m_dVolaChangePercent == 0 
			){

				m_spSimulation->CalcFutOptionGreeks(  aUnd,
					dUndMidPrice,
					nMask,
					bIsPnlLTD,
					enCalcModel,
					bUseTheoVolatility,
					bUseTheoVolaNoBid,
					bUseTheoVolaBadMarket,
					dUndPriceTolerance,
					enPriceRoundingRule,
					VARIANT_TRUE,
					VARIANT_FALSE,
					pParams);
			}
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_IMmRvPosAtom, e.Error());
	}

	return S_OK;
}

/////////////////////////////////////////////////////////////////////////////
//
void CMmRvPosAtom::_CalcPnlMtm(VARIANT_BOOL bIsPnlLTD, DOUBLE dPriceBid, DOUBLE dPriceAsk, bool ReplaceWithZero)
{
	m_dPnlMtm = BAD_DOUBLE_VALUE;

	if(bIsPnlLTD)
	{
		if(m_nQtyLTDBuy > BAD_LONG_VALUE)
		{
			if((dPriceAsk > 0 || dPriceBid > 0 || ReplaceWithZero ) && m_dPosLTDBuy > BAD_DOUBLE_VALUE)
				m_dPnlMtm = dPriceBid * m_nQtyLTDBuy - m_dPosLTDBuy;
			else
				return;
		}
			
		if(m_nQtyLTDSell > BAD_LONG_VALUE)
		{
			if((dPriceAsk > 0 || dPriceBid > 0|| ReplaceWithZero ) && m_dPosLTDSell > BAD_DOUBLE_VALUE)
			{
				if(m_dPnlMtm <= BAD_DOUBLE_VALUE) m_dPnlMtm = 0.;
				m_dPnlMtm += dPriceAsk * m_nQtyLTDSell - m_dPosLTDSell;
			}
			else
				return;
		}
	}
	else
	{
		if(m_nQtyDailyPrevDateBuy > BAD_LONG_VALUE)
		{
			if((dPriceAsk > 0 || dPriceBid > 0 || ReplaceWithZero ) && m_dPosDailyPrevDateBuy > BAD_DOUBLE_VALUE)
				m_dPnlMtm = dPriceBid * m_nQtyDailyPrevDateBuy - m_dPosDailyPrevDateBuy;
			else
				return;
		}

		if(m_nQtyDailyPrevDateSell > BAD_LONG_VALUE)
		{
			if((dPriceAsk > BAD_DOUBLE_VALUE || dPriceBid >0 || ReplaceWithZero ) && m_dPosDailyPrevDateSell > BAD_DOUBLE_VALUE)
			{
				if(m_dPnlMtm <= BAD_DOUBLE_VALUE) m_dPnlMtm = 0.;
				m_dPnlMtm += dPriceAsk * m_nQtyDailyPrevDateSell - m_dPosDailyPrevDateSell;
			}
			else
				return;
		}

		if(m_nQtyDailyTodayBuy > BAD_LONG_VALUE)
		{
			if( (dPriceBid > BAD_DOUBLE_VALUE || dPriceAsk > BAD_DOUBLE_VALUE || ReplaceWithZero ) && m_dPosDailyTodayBuy > BAD_DOUBLE_VALUE)
			{
				if(m_dPnlMtm <= BAD_DOUBLE_VALUE) m_dPnlMtm = 0.;
				m_dPnlMtm += dPriceBid * m_nQtyDailyTodayBuy - m_dPosDailyTodayBuy;
			}
			else
				return;
		}

		if(m_nQtyDailyTodaySell > BAD_LONG_VALUE)
		{
			if((dPriceBid > BAD_DOUBLE_VALUE || dPriceAsk > BAD_DOUBLE_VALUE || ReplaceWithZero ) && m_dPosDailyTodaySell > BAD_DOUBLE_VALUE)
			{
				if(m_dPnlMtm <= BAD_DOUBLE_VALUE) m_dPnlMtm = 0.;
				m_dPnlMtm += dPriceAsk * m_nQtyDailyTodaySell - m_dPosDailyTodaySell;
			}
			else
				return;
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
//
void CMmRvPosAtom::_CalcPnlTheo(VARIANT_BOOL bIsPnlLTD)
{
	m_dPnlTheo = BAD_DOUBLE_VALUE;

	if(m_dPriceTheo >= 0/*DBL_EPSILON*/)
	{
		if(bIsPnlLTD)
		{
			if(m_nQtyLTDBuy > BAD_LONG_VALUE)
			{
				if(m_dPosLTDBuy > BAD_DOUBLE_VALUE)
					m_dPnlTheo = m_dPriceTheo * m_nQtyLTDBuy - m_dPosLTDBuy;
				else
					return;
			}
				
			if(m_nQtyLTDSell > BAD_LONG_VALUE)
			{
				if(m_dPosLTDSell > BAD_DOUBLE_VALUE)
				{
					if(m_dPnlTheo <= BAD_DOUBLE_VALUE) m_dPnlTheo = 0.;
					m_dPnlTheo += m_dPriceTheo * m_nQtyLTDSell - m_dPosLTDSell;
				}
				else
					return;
			}
		}
		else
		{
			if(m_nQtyDailyPrevDateBuy > BAD_LONG_VALUE)
			{
				if(m_dPosDailyPrevDateBuy > BAD_DOUBLE_VALUE)
					m_dPnlTheo = m_dPriceTheo * m_nQtyDailyPrevDateBuy - m_dPosDailyPrevDateBuy;
				else
					return;
			}

			if(m_nQtyDailyPrevDateSell > BAD_LONG_VALUE)
			{
				if(m_dPosDailyPrevDateSell > BAD_DOUBLE_VALUE)
				{
					if(m_dPnlTheo <= BAD_DOUBLE_VALUE) m_dPnlTheo = 0.;
					m_dPnlTheo += m_dPriceTheo * m_nQtyDailyPrevDateSell - m_dPosDailyPrevDateSell;
				}
				else
					return;
			}

			if(m_nQtyDailyTodayBuy > BAD_LONG_VALUE)
			{
				if(m_dPosDailyTodayBuy > BAD_DOUBLE_VALUE)
				{
					if(m_dPnlTheo <= BAD_DOUBLE_VALUE) m_dPnlTheo = 0.;
					m_dPnlTheo += m_dPriceTheo * m_nQtyDailyTodayBuy - m_dPosDailyTodayBuy;
				}
				else
					return;
			}

			if(m_nQtyDailyTodaySell > BAD_LONG_VALUE)
			{
				if(m_dPosDailyTodaySell > BAD_DOUBLE_VALUE)
				{
					if(m_dPnlTheo <= BAD_DOUBLE_VALUE) m_dPnlTheo = 0.;
					m_dPnlTheo += m_dPriceTheo * m_nQtyDailyTodaySell - m_dPosDailyTodaySell;
				}
				else
					return;
			}
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
//
void CMmRvPosAtom::_CalcPnlTheoExercItmDailyToday(DOUBLE dUndPriceBid, DOUBLE dUndPriceAsk)
{
	//m_dPnlTheo = BAD_DOUBLE_VALUE;
	bool bBadPnl = false;

	if((dUndPriceBid >= DBL_EPSILON && m_nQtyDailyTodayBuy  > BAD_LONG_VALUE) ||
	   (dUndPriceAsk >= DBL_EPSILON && m_nQtyDailyTodaySell > BAD_LONG_VALUE))
	{
		if(m_enOptType == enOtCall)
		{
			if(m_nQtyDailyTodayBuy > BAD_LONG_VALUE)
			{
				if(dUndPriceBid > DBL_EPSILON)
				{
					if(m_dPnlTheo <= BAD_DOUBLE_VALUE)
						m_dPnlTheo = 0.;
					m_dPnlTheo += m_nQtyDailyTodayBuy * (dUndPriceBid - m_dStrike) - m_dPosDailyTodayBuy;
				}
				else
					bBadPnl = true;
			}

			if(!bBadPnl && m_nQtyDailyTodaySell > BAD_LONG_VALUE)
			{
				if(dUndPriceAsk > DBL_EPSILON)
				{
					if(m_dPnlTheo <= BAD_DOUBLE_VALUE)
						m_dPnlTheo = 0.;
					m_dPnlTheo += m_nQtyDailyTodaySell * (dUndPriceAsk - m_dStrike) - m_dPosDailyTodaySell;

				}
				else
					bBadPnl = true;
			}
		}
		else
		{
			if(m_nQtyDailyTodayBuy > BAD_LONG_VALUE)
			{
				if(dUndPriceBid > DBL_EPSILON)
				{
					if(m_dPnlTheo <= BAD_DOUBLE_VALUE)
						m_dPnlTheo = 0.;
					m_dPnlTheo += m_nQtyDailyTodayBuy * (m_dStrike - dUndPriceBid) - m_dPosDailyTodayBuy;
				}
				else
					bBadPnl = true;
			}

			if(!bBadPnl && m_nQtyDailyTodaySell > BAD_LONG_VALUE)
			{
				if(dUndPriceAsk > DBL_EPSILON)
				{
					if(m_dPnlTheo <= BAD_DOUBLE_VALUE)
						m_dPnlTheo = 0.;
					m_dPnlTheo += m_nQtyDailyTodaySell * (m_dStrike - dUndPriceAsk) - m_dPosDailyTodaySell;
				}
				else
					bBadPnl = true;
			}
		}
	}

	if(bBadPnl)
		m_dPnlTheo = BAD_DOUBLE_VALUE;

	return;
}

/////////////////////////////////////////////////////////////////////////////
//
void CMmRvPosAtom::_CalcPnlTheoExercItmDailyPrevDate(DOUBLE dUndPriceBid, DOUBLE dUndPriceAsk)
{
	//m_dPnlTheo = BAD_DOUBLE_VALUE;
	bool bBadPnl = false;

	if((m_dPriceClose >= DBL_EPSILON) && ((dUndPriceBid >= DBL_EPSILON && m_nQtyDailyPrevDateBuy > BAD_LONG_VALUE) ||
										  (dUndPriceAsk >= DBL_EPSILON && m_nQtyDailyPrevDateSell > BAD_LONG_VALUE))    
		)
	{
		if(m_enOptType == enOtCall)
		{
			if(m_nQtyDailyPrevDateBuy > BAD_LONG_VALUE)
			{
				if(dUndPriceBid > DBL_EPSILON)
				{
					if(m_dPnlTheo <= BAD_DOUBLE_VALUE)
						m_dPnlTheo = 0.;
					m_dPnlTheo += m_nQtyDailyPrevDateBuy * (dUndPriceBid - m_dStrike - m_dPriceClose);
				}
				else
					bBadPnl = true;
			}

			if(!bBadPnl && m_nQtyDailyPrevDateSell > BAD_LONG_VALUE)
			{
				if(dUndPriceAsk > DBL_EPSILON)
				{
					if(m_dPnlTheo <= BAD_DOUBLE_VALUE)
						m_dPnlTheo = 0.;
					m_dPnlTheo += m_nQtyDailyPrevDateSell * (dUndPriceAsk - m_dStrike - m_dPriceClose);

				}
				else
					bBadPnl = true;
			}
		}
		else
		{
			if(m_nQtyDailyPrevDateBuy > BAD_LONG_VALUE)
			{
				if(dUndPriceBid > DBL_EPSILON)
				{
					if(m_dPnlTheo <= BAD_DOUBLE_VALUE)
						m_dPnlTheo = 0.;
					m_dPnlTheo += m_nQtyDailyPrevDateBuy * (m_dStrike - dUndPriceBid - m_dPriceClose);
				}
				else
					bBadPnl = true;
			}

			if(!bBadPnl && m_nQtyDailyPrevDateSell > BAD_LONG_VALUE)
			{
				if(dUndPriceAsk > DBL_EPSILON)
				{
					if(m_dPnlTheo <= BAD_DOUBLE_VALUE)
						m_dPnlTheo = 0.;
					m_dPnlTheo += m_nQtyDailyPrevDateSell * (m_dStrike - dUndPriceAsk - m_dPriceClose);
				}
				else
					bBadPnl = true;
			}
		}
	}
	
	if(bBadPnl)
		m_dPnlTheo = BAD_DOUBLE_VALUE;

	return;
}

/////////////////////////////////////////////////////////////////////////////
//
void CMmRvPosAtom::_CalcPnlTheoExercItmLTD(DOUBLE dUndPriceBid, DOUBLE dUndPriceAsk)
{
	m_dPnlTheo = BAD_DOUBLE_VALUE;
	bool bBadPnl = false;

	if((dUndPriceBid > DBL_EPSILON && m_nQtyLTDBuy > BAD_LONG_VALUE) ||
	   (dUndPriceAsk > DBL_EPSILON && m_nQtyLTDSell > BAD_LONG_VALUE))
	{
		if(m_enOptType == enOtCall)
		{
			if(m_nQtyLTDBuy > BAD_LONG_VALUE)
			{
				if(dUndPriceBid > DBL_EPSILON)
					m_dPnlTheo = m_nQtyLTDBuy * (dUndPriceBid - m_dStrike) - m_dPosLTDBuy;
				else
					bBadPnl = true;
			}

			if(!bBadPnl && m_nQtyLTDSell > BAD_LONG_VALUE)
			{
				if(dUndPriceAsk > DBL_EPSILON)
				{
					if(m_dPnlTheo <= BAD_DOUBLE_VALUE)
						m_dPnlTheo = 0.;
					m_dPnlTheo += m_nQtyLTDSell * (dUndPriceAsk - m_dStrike) - m_dPosLTDSell;
				}
				else
					bBadPnl = true;
			}

		}
		else
		{
			if(m_nQtyLTDBuy > BAD_LONG_VALUE)
			{
				if(dUndPriceBid > DBL_EPSILON)
					m_dPnlTheo = m_nQtyLTDBuy * (m_dStrike - dUndPriceBid) - m_dPosLTDBuy;
				else
					bBadPnl = true;
			}

			if(!bBadPnl && m_nQtyLTDSell > BAD_LONG_VALUE)
			{
				if(dUndPriceAsk > DBL_EPSILON)
				{
					if(m_dPnlTheo <= BAD_DOUBLE_VALUE)
						m_dPnlTheo = 0.;
					m_dPnlTheo += m_nQtyLTDSell * (m_dStrike - dUndPriceAsk) - m_dPosLTDSell;
				}
				else
					bBadPnl = true;
			}
		}

		if(bBadPnl)
			m_dPnlTheo  = BAD_DOUBLE_VALUE;
	}

	return;
}

/////////////////////////////////////////////////////////////////////////////
//
void CMmRvPosAtom::_CalcPnlTheoExercOtm(VARIANT_BOOL bIsPnlLTD)
{
	m_dPnlTheo = BAD_DOUBLE_VALUE;

	if(!bIsPnlLTD && (m_nQtyDailyTodayBuy > BAD_LONG_VALUE || m_nQtyDailyTodaySell > BAD_LONG_VALUE))
	{
		if(m_dPriceClose > DBL_EPSILON)
		{
			if(m_nQtyDailyTodayBuy > BAD_LONG_VALUE)
				m_dPnlTheo = -m_nQtyDailyTodayBuy * m_dPriceClose;

			if(m_nQtyDailyTodaySell > BAD_LONG_VALUE)
			{
				if(m_dPnlTheo <= BAD_DOUBLE_VALUE)
					m_dPnlTheo = 0.;
				m_dPnlTheo -= m_nQtyDailyTodaySell * m_dPriceClose;
			}
		}
	}
	else
	{
		if(m_nQtyLTDBuy > BAD_LONG_VALUE)
			m_dPnlTheo = - m_dPosLTDBuy;

		if(m_nQtyLTDSell > BAD_LONG_VALUE)
		{
			if(m_dPnlTheo <= BAD_DOUBLE_VALUE)
				m_dPnlTheo = 0.;
			m_dPnlTheo -= m_dPosLTDSell;
		}
	}

	return;
}

/////////////////////////////////////////////////////////////////////////////
//
void CMmRvPosAtom::_CalcPnlTheoExerc(DOUBLE dUndSpot, DOUBLE dUndPriceBid, DOUBLE dUndPriceAsk, VARIANT_BOOL bIsPnlLTD)
{
	
	if((m_enOptType == enOtCall && m_dStrike < dUndSpot) ||
	   (m_enOptType == enOtPut  && m_dStrike > dUndSpot))
	{
		if(bIsPnlLTD)
			_CalcPnlTheoExercItmLTD			 (dUndPriceBid, dUndPriceAsk);
		else
		{
			_CalcPnlTheoExercItmDailyPrevDate(dUndPriceBid, dUndPriceAsk);
			_CalcPnlTheoExercItmDailyToday	 (dUndPriceBid, dUndPriceAsk);
		}

	}
	else
		_CalcPnlTheoExercOtm(bIsPnlLTD);

	return;
}

/////////////////////////////////////////////////////////////////////////////
//
void CMmRvPosAtom::_GetBasketIndexDividends(IIndexAtomPtr spBasketIndex, REGULAR_DIVIDENDS* pDivs, LONG nMaxCount) throw()
{
	ATLASSERT(spBasketIndex != NULL);
	EtsRegularDividend aDiv;

	for(LONG i = 0; i < nMaxCount; i++)
	{
		aDiv = spBasketIndex->BasketDiv[i];
		pDivs[i].nLastDivDate = static_cast<LONG>(aDiv.LastDate);
		pDivs[i].nFrequency = aDiv.Freq;
		pDivs[i].dAmount = aDiv.Amt;
	}
}

/////////////////////////////////////////////////////////////////////////////
//
void CMmRvPosAtom::_GetSyntheticRootBasketDividends(ISynthRootAtomPtr spSynthRoot, REGULAR_DIVIDENDS* pDivs, LONG nMaxCount) throw()
{
	ATLASSERT(spSynthRoot != NULL);
	EtsRegularDividend aDiv;

	for(LONG i = 0; i < nMaxCount; i++)
	{
		aDiv = spSynthRoot->BasketDiv[i];
		pDivs[i].nLastDivDate = static_cast<LONG>(aDiv.LastDate);
		pDivs[i].nFrequency = aDiv.Freq;
		pDivs[i].dAmount = aDiv.Amt;
	}
}

STDMETHODIMP CMmRvPosAtom::AddTrade(IMmTradeInfoAtom* pTrade, IMmRvUndAtom* pUnd)
{
	long nQty = 0;
	long nQtyInShares = 0;
	HRESULT hr = S_OK;
	IMmTradeInfoAtomPtr spTrade(pTrade);

	try
	{
		if(spTrade!=NULL)
		{
			bool bIsBuy  = spTrade->IsBuy!=VARIANT_FALSE?true:false;
			nQty		 = spTrade->Quantity * (bIsBuy?1:-1);
            nQtyInShares = nQty * m_nLotSize;

			if(bIsBuy)
			{
				if (m_nQtyLTDBuy == BAD_LONG_VALUE)
					m_nQtyLTDBuy = 0 ;

				m_nQtyLTDBuy += nQtyInShares;
				if(spTrade->IsPosition)
				{
					if (m_dPriceClose >= DBL_EPSILON )
					{
						if(m_dPosLTDBuy == BAD_DOUBLE_VALUE ) 
							m_dPosLTDBuy = 0;
                        
						m_dPosLTDBuy += (m_dPriceClose * nQtyInShares);
					}
				}else
				{
					if (m_dPosLTDBuy == BAD_DOUBLE_VALUE)
						m_dPosLTDBuy = 0;

					m_dPosLTDBuy += (spTrade->Price * nQtyInShares);
				}
		    
				if( spTrade->TradeDate < vt_date::GetCurrentDate(true))
				{
					if (m_nQtyDailyPrevDateBuy == BAD_LONG_VALUE) 
						m_nQtyDailyPrevDateBuy = 0;

					m_nQtyDailyPrevDateBuy +=  nQtyInShares;

					if (m_dPriceClose >= DBL_EPSILON) 
					{
						if(m_dPosDailyPrevDateBuy == BAD_DOUBLE_VALUE) 
							m_dPosDailyPrevDateBuy = 0;
							m_dPosDailyPrevDateBuy +=  (m_dPriceClose * nQtyInShares);
					}
					else
						if(!spTrade->IsPosition)
						{
							if(m_dPosDailyPrevDateBuy == BAD_DOUBLE_VALUE) m_dPosDailyPrevDateBuy = 0;
                            
							m_dPosDailyPrevDateBuy += (spTrade->Price * nQtyInShares);
						}
				}else
				{
					if(m_nQtyDailyTodayBuy == BAD_LONG_VALUE) m_nQtyDailyTodayBuy = 0;

					m_nQtyDailyTodayBuy += nQtyInShares;
					if(spTrade->IsPosition) 
					{
						if(m_dPriceClose >= DBL_EPSILON)
						{
							if (m_dPosDailyTodayBuy == BAD_DOUBLE_VALUE) m_dPosDailyTodayBuy = 0;
							m_dPosDailyTodayBuy += (m_dPriceClose * nQtyInShares);
						}
					}
					else
					{
						if (m_dPosDailyTodayBuy == BAD_DOUBLE_VALUE) m_dPosDailyTodayBuy = 0;
						m_dPosDailyTodayBuy += (spTrade->Price * nQtyInShares);
					}
				}
			}
			else
			{
				if(m_nQtyLTDSell == BAD_LONG_VALUE)
					m_nQtyLTDSell = 0;
				m_nQtyLTDSell += nQtyInShares;

				if (spTrade->IsPosition) 
				{
					if(m_dPriceClose >= DBL_EPSILON)
					{
						if(m_dPosLTDSell == BAD_DOUBLE_VALUE) 
							m_dPosLTDSell = 0;
						m_dPosLTDSell += (m_dPriceClose * nQtyInShares);
					}
				}
				else
				{
					if (m_dPosLTDSell == BAD_DOUBLE_VALUE) 
						m_dPosLTDSell = 0;
					m_dPosLTDSell += (spTrade->Price * nQtyInShares);
				}
		    
				if (spTrade->TradeDate < vt_date::GetCurrentDate(true))
				{
					if (m_nQtyDailyPrevDateSell == BAD_LONG_VALUE) 
						m_nQtyDailyPrevDateSell = 0; 
					m_nQtyDailyPrevDateSell += nQtyInShares;
					if (m_dPriceClose >= DBL_EPSILON)
					{
						if (m_dPosDailyPrevDateSell == BAD_DOUBLE_VALUE) m_dPosDailyPrevDateSell = 0;
						m_dPosDailyPrevDateSell +=  (m_dPriceClose * nQtyInShares);
					}
					else if(!spTrade->IsPosition)
					{
						if (m_dPosDailyPrevDateSell == BAD_DOUBLE_VALUE) m_dPosDailyPrevDateSell = 0;
						m_dPosDailyPrevDateSell += (spTrade->Price * nQtyInShares);
					}
				}
				else
				{
					if(m_nQtyDailyTodaySell == BAD_LONG_VALUE) m_nQtyDailyTodaySell = 0;
					m_nQtyDailyTodaySell += nQtyInShares;
					if (spTrade->IsPosition)
					{
						if (m_dPriceClose >= DBL_EPSILON) 
						{
							if(m_dPosDailyTodaySell == BAD_DOUBLE_VALUE) 
								m_dPosDailyTodaySell = 0;
                            
							m_dPosDailyTodaySell += (m_dPriceClose * nQtyInShares);
						}
					}
					else
					{
						if (m_dPosDailyTodaySell == BAD_DOUBLE_VALUE) m_dPosDailyTodaySell = 0;
						m_dPosDailyTodaySell += (spTrade->Price * nQtyInShares);
					}
				}
			}
		    
			m_nQty +=  nQty;
			m_nQtyInShares += nQtyInShares;

			if(pUnd != NULL)
			{
				if(enCtOption==m_enContractType || enCtFutOption ==m_enContractType)
				{
					long lUndOptQty = BAD_LONG_VALUE;
					pUnd->get_OptQty(&lUndOptQty);

					if (lUndOptQty == BAD_LONG_VALUE) 
					{
						lUndOptQty = 0L;
						pUnd->put_OptQty(0L);
					}
					pUnd->put_OptQty(lUndOptQty + nQty);
				}
				else 
				if (m_enContractType == enCtFuture)
				{
					long lUndFutQty = BAD_LONG_VALUE;
					pUnd->get_FutQty(&lUndFutQty);

					if(lUndFutQty == BAD_LONG_VALUE) 
					{
						lUndFutQty = 0L;
						pUnd->put_FutQty(0L);
					}
					pUnd->put_FutQty(lUndFutQty + nQty);

					long lUndQty = BAD_LONG_VALUE;
					pUnd->get_Qty(&lUndQty);

					if(lUndQty == BAD_LONG_VALUE){
						pUnd->put_Qty(0);
						lUndQty = 0;
					}
					pUnd->put_Qty(lUndQty+ nQtyInShares);

				}
				else
				{
					long lUndQty = BAD_LONG_VALUE;
					pUnd->get_Qty(&lUndQty);

					if(lUndQty == BAD_LONG_VALUE){
						pUnd->put_Qty(0);
						lUndQty = 0;
					}
					pUnd->put_Qty(lUndQty+ nQtyInShares);
				}
			}

			if (m_spSimulation != NULL ) {
				m_spSimulation->AddTrade( pTrade, pUnd );
			}

		}
		else
			hr = E_POINTER;
	}
	catch (_com_error& e)
	{
		hr = Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_IMmRvPosAtom, e.Error());
	}
	/*catch (...) 
	{
		hr = Error((LPCOLESTR)_bstr_t(L"Unknown error while adding trade to position"), IID_IMmRvPosAtom, E_FAIL);
	}*/

	return hr;
}

//////////////////////////////////////////////////////////////////////////
// IcvDataProvider 
//////////////////////////////////////////////////////////////////////////

vector<cvProvidedID>		CMmRvPosAtom::m_vecInternalVarID;
cvIDToIDMap					CMmRvPosAtom::m_mapSupportedCVID;
vector<cvProvidedID>		CMmRvPosAtom::m_vecInternalFuncID;
cvIDToIDMap					CMmRvPosAtom::m_mapSupportedFuncID;

STDMETHODIMP CMmRvPosAtom::raw_GetData(LONG lDataID, VARIANT* Value){
	int iLocalDataID = GetDataID( lDataID );
	VARIANT vtValue;

	if ( iLocalDataID == CV_DATAPROVIDER_BAD_ID ) 
		return E_FAIL;
		switch( iLocalDataID ) {
			case CV_DP_DATA_POSBID :
				VariantInit( &vtValue );
				vtValue.vt = VT_R8;
				vtValue.dblVal = ( ( BAD_DOUBLE_VALUE == m_dPriceBid )  ? 0.0 : m_dPriceBid );
				VariantCopy( Value, &vtValue );
				break;
			case CV_DP_DATA_POSASK :
				VariantInit( &vtValue );
				vtValue.vt = VT_R8;
				vtValue.dblVal = ( ( BAD_DOUBLE_VALUE == m_dPriceAsk)  ? 0.0 : m_dPriceAsk);
				VariantCopy( Value, &vtValue );
				break;
			case CV_DP_DATA_POSLAST:
				VariantInit( &vtValue );
				vtValue.vt = VT_R8;
				vtValue.dblVal = ( ( BAD_DOUBLE_VALUE == m_dPriceLast)  ? 0.0 : m_dPriceLast);
				VariantCopy( Value, &vtValue );
				break;
			case CV_DP_DATA_POSMARK:
				VariantInit( &vtValue );
				vtValue.vt = VT_R8;
				vtValue.dblVal = ( ( BAD_DOUBLE_VALUE == m_dPriceClose)  ? 0.0 : m_dPriceClose);
				VariantCopy( Value, &vtValue );
				break;
			case CV_DP_DATA_POSSTRIKE :
				VariantInit( &vtValue );
				vtValue.vt = VT_R8;
				vtValue.dblVal = ( ( BAD_DOUBLE_VALUE == m_dStrike )  ? 0.0 : m_dStrike );
				VariantCopy( Value, &vtValue );
				break;
			case CV_DP_DATA_POSTV :
				VariantInit( &vtValue );
				vtValue.vt = VT_R8;
				vtValue.dblVal = ( ( BAD_DOUBLE_VALUE == m_dTimeValue)  ? 0.0 : m_dTimeValue);
				VariantCopy( Value, &vtValue );
				break;
			case CV_DP_DATA_POSVEGA:
				VariantInit( &vtValue );
				vtValue.vt = VT_R8;
				vtValue.dblVal = ( ( BAD_DOUBLE_VALUE == m_dVega)  ? 0.0 : m_dVega*100);
				VariantCopy( Value, &vtValue );
				break;
			case CV_DP_DATA_POSDELTA:
				VariantInit( &vtValue );
				vtValue.vt = VT_R8;
				vtValue.dblVal = ( ( BAD_DOUBLE_VALUE == m_dDelta)  ? 0.0 : m_dDelta*100);
				VariantCopy( Value, &vtValue );
				break;
			case CV_DP_DATA_POSCHANGE :
				VariantInit( &vtValue );
				vtValue.vt = VT_R8;
				vtValue.dblVal = ( ( BAD_DOUBLE_VALUE == m_dNetChange)  ? 0.0 : m_dNetChange);
				VariantCopy( Value, &vtValue );
				break;
			case CV_DP_DATA_POSQTY:
				VariantInit( &vtValue );
				vtValue.vt = VT_R8;
				vtValue.dblVal = ( ( BAD_DOUBLE_VALUE == m_nQty)  ? 0.0 : m_nQty);
				VariantCopy( Value, &vtValue );
				break;
			case CV_DP_DATA_POSLOTSIZE:
				VariantInit( &vtValue );
				vtValue.vt = VT_R8;
				vtValue.dblVal = ( ( BAD_DOUBLE_VALUE == m_nLotSize)  ? 0.0 : m_nLotSize);
				VariantCopy( Value, &vtValue );
				break;

			case CV_DP_DATA_POSTHETA:
				VariantInit( &vtValue );
				vtValue.vt = VT_R8;
				vtValue.dblVal = ( ( BAD_DOUBLE_VALUE == m_dTheta)  ? 0.0 : m_dTheta*100);
				VariantCopy( Value, &vtValue );
				break;
			case CV_DP_DATA_POSRHO:
				VariantInit( &vtValue );
				vtValue.vt = VT_R8;
				vtValue.dblVal = ( ( BAD_DOUBLE_VALUE == m_dRho)  ? 0.0 : m_dRho*100);
				VariantCopy( Value, &vtValue );
				break;
			case CV_DP_DATA_POSOPTPRICE:
				VariantInit( &vtValue );
				vtValue.vt = VT_R8;
				vtValue.dblVal = ( ( BAD_DOUBLE_VALUE == m_dOptPrice)  ? 0.0 : m_dOptPrice);
				VariantCopy( Value, &vtValue );
				break;
			case CV_DP_DATA_POSGAMMA:
				VariantInit( &vtValue );
				vtValue.vt = VT_R8;
				vtValue.dblVal = ( ( BAD_DOUBLE_VALUE == m_dGamma)  ? 0.0 : m_dGamma*100);
				VariantCopy( Value, &vtValue );
				break;
			case CV_DP_DATA_POSTHEOVOL:
				VariantInit( &vtValue );
				vtValue.vt = VT_R8;
				vtValue.dblVal = ( ( BAD_DOUBLE_VALUE == m_dVola)  ? 0.0 : m_dVola*100);
				VariantCopy( Value, &vtValue );
				break;	
			case CV_DP_DATA_POSPNLMTM:
				VariantInit( &vtValue );
				vtValue.vt = VT_R8;
				vtValue.dblVal = ( ( BAD_DOUBLE_VALUE == m_dPnlMtm)  ? 0.0 : m_dPnlMtm);
				VariantCopy( Value, &vtValue );
				break;
			case CV_DP_DATA_POSPNLTHEO:
				VariantInit( &vtValue );
				vtValue.vt = VT_R8;
				vtValue.dblVal = ( ( BAD_DOUBLE_VALUE == m_dPnlTheo)  ? 0.0 : m_dPnlTheo);
				VariantCopy( Value, &vtValue );
				break;
			case CV_DP_DATA_POSRATE:
				VariantInit( &vtValue );
				vtValue.vt = VT_R8;
				vtValue.dblVal = ( ( BAD_DOUBLE_VALUE == m_dRate)  ? 0.0 : m_dRate);
				VariantCopy( Value, &vtValue );
				break;	
			case CV_DP_DATA_POSPRICETHEO:
				VariantInit( &vtValue );
				vtValue.vt = VT_R8;
				vtValue.dblVal = ( ( BAD_DOUBLE_VALUE == m_dPriceTheo)  ? 0.0 : m_dPriceTheo);
				VariantCopy( Value, &vtValue );
				break;	
			case CV_DP_DATA_POSTHETADELTA:
				VariantInit( &vtValue );
				vtValue.vt = VT_R8;
				vtValue.dblVal = ( ( BAD_DOUBLE_VALUE == m_dThetaDeltaInShares)  ? 0.0 : m_dThetaDeltaInShares);
				VariantCopy( Value, &vtValue );
				break;
			case CV_DP_DATA_POSTHETAGAMMA:
				VariantInit( &vtValue );
				vtValue.vt = VT_R8;
				vtValue.dblVal = ( ( BAD_DOUBLE_VALUE == m_dThetaGammaInShares)  ? 0.0 : m_dThetaGammaInShares);
				VariantCopy( Value, &vtValue );
				break;
			case CV_DP_DATA_POSVEGADELTA:
				VariantInit( &vtValue );
				vtValue.vt = VT_R8;
				vtValue.dblVal = ( ( BAD_DOUBLE_VALUE == m_dVegaDeltaInShares)  ? 0.0 : m_dVegaDeltaInShares);
				VariantCopy( Value, &vtValue );
				break;
			case CV_DP_DATA_POSVEGAGAMMA :
				VariantInit( &vtValue );
				vtValue.vt = VT_R8;
				vtValue.dblVal = ( ( BAD_DOUBLE_VALUE == m_dVegaGammaInShares)  ? 0.0 : m_dVegaGammaInShares);
				VariantCopy( Value, &vtValue );
				break;
			case CV_DP_DATA_POSSYNTHPRICE:
				VariantInit( &vtValue );
				vtValue.vt = VT_R8;
				vtValue.dblVal = ( ( BAD_DOUBLE_VALUE == m_dSynthUndPrice)  ? 0.0 : m_dSynthUndPrice);
				VariantCopy( Value, &vtValue );
				break;
			case CV_DP_DATA_POSHASSYNTH:
				VariantInit( &vtValue );
				vtValue.vt = VT_BOOL;
				vtValue.boolVal = m_bIsSynthetic;
				VariantCopy( Value, &vtValue );
				break;
			case CV_DP_DATA_POSMIV:
				VariantInit( &vtValue );
				vtValue.vt = VT_R8;
				vtValue.dblVal = ( ( BAD_DOUBLE_VALUE == m_dMIV)  ? 0.0 : m_dMIV * 100.);
				VariantCopy( Value, &vtValue );
				break;
			case CV_DP_DATA_POSVOLGA:
				VariantInit( &vtValue );
				vtValue.vt = VT_R8;
				vtValue.dblVal = ( ( BAD_DOUBLE_VALUE == m_dVolga)  ? 0.0 : m_dVolga * 100.0);
				VariantCopy( Value, &vtValue );
				break;
			default:;
	}
	return S_OK;
}

STDMETHODIMP CMmRvPosAtom::raw_CallFunction(LONG lFunctionID, SAFEARRAY** arrParameters, VARIANT *Value){
	int iLocalFuncID = GetFuncID( lFunctionID );
	VARIANT vtValue;

	if ( iLocalFuncID == CV_DATAPROVIDER_BAD_ID ) 
		return E_FAIL;
	switch( iLocalFuncID ) {
		case CV_DP_FUNC_POSDTE:
			{
				VariantInit( &vtValue );
				vtValue.vt = VT_R8;
				if( m_enContractType == enCtFutOption || m_enContractType == enCtOption ){
					
					DATE dtCalcDate;
					::GetNYDateTimeAsDATE(&dtCalcDate); 
					if ( m_simulationParams.m_dDate >= dtCalcDate ) 
						dtCalcDate = m_simulationParams.m_dDate;
					DOUBLE DTE = m_dtExpiryOV - dtCalcDate;
					vtValue.dblVal = DTE;
				}
				else
   				vtValue.dblVal = 0.0;
				VariantCopy( Value, &vtValue );
			}
			break;
		case CV_DP_FUNC_POSTYPE:
			{
				if (!Value)
					return E_POINTER;

				VariantInit( Value );
				Value->vt = VT_BSTR;
				switch( m_enContractType ) {
					case enCtStock:
						Value->bstrVal = ::SysAllocString( L"STOCK" );
						break;
					case enCtIndex:
						Value->bstrVal = ::SysAllocString( L"INDEX" );
						break;
					case enCtOption:
						Value->bstrVal = ::SysAllocString( L"OPTION" );
						break;	
					case enCtFuture:
						Value->bstrVal = ::SysAllocString( L"FUTURE" );
						break;
					case enCtFutOption:
						Value->bstrVal = ::SysAllocString( L"FUTURE_OPTION" );
						break;
					default:
						Value->bstrVal = ::SysAllocString( L"UNKNOWN" );
				}
				//VariantCopy( Value, &vtValue );
			}
			break;
		case CV_DP_FUNC_POSISBUY:
			{
				VariantInit( &vtValue );
				vtValue.vt = VT_BOOL;
				m_nQty > 0 ? vtValue.boolVal = VARIANT_TRUE : VARIANT_FALSE;
				VariantCopy( Value, &vtValue );
			}
			break;
		case CV_DP_FUNC_POSISSELL:
			{
				VariantInit( &vtValue );
				vtValue.vt = VT_BOOL;
				m_nQty < 0 ? vtValue.boolVal = VARIANT_TRUE : VARIANT_FALSE;
				VariantCopy( Value, &vtValue );

			}
			break;
		case CV_DP_FUNC_POSISCALL:
			{
				VariantInit( &vtValue );
				vtValue.vt = VT_BOOL;
				m_enOptType == enOtCall ? vtValue.boolVal = VARIANT_TRUE : VARIANT_FALSE;
				VariantCopy( Value, &vtValue );

			}
			break;
		case CV_DP_FUNC_POSISPUT:
			{
				VariantInit( &vtValue );
				vtValue.vt = VT_BOOL;
				m_enOptType == enOtPut ? vtValue.boolVal = VARIANT_TRUE : VARIANT_FALSE;
				VariantCopy( Value, &vtValue );

			}
			break;

		default:;
	}
	return S_OK;
}

STDMETHODIMP CMmRvPosAtom::raw_Check(SAFEARRAY **arrSysVars, SAFEARRAY **arrSysFuncs, LONG *pRetVal){
	long lUBound;
	long lLBound;
	cvInfo *pInfo = NULL;

	lUBound = lLBound = 0;
	SafeArrayGetUBound( *arrSysVars, 1, &lUBound );
	SafeArrayGetLBound( *arrSysVars, 1, &lLBound );
	SafeArrayAccessData( *arrSysVars, (void**)&pInfo);
	size_t UnsupportedCount = lUBound -lLBound + 1;
	if( pInfo ) {
		for ( long i = lLBound; i <= lUBound;i++) {
			// add every required system variable into vector
			string strName = (LPCSTR)_bstr_t(pInfo->Name);
			size_t s = FindDataByName(strName);
			if ( s != CV_DATANOTSUPPORTED ) {
				m_mapSupportedCVID[pInfo->ID] = m_vecInternalVarID[s].m_DataId;
				UnsupportedCount--;
			}
			pInfo++;
		}
	}
	SafeArrayUnaccessData(*arrSysVars);

	if ( UnsupportedCount ) {
		*pRetVal = static_cast<LONG>(UnsupportedCount) ;
		return S_OK;
	}


	lUBound = lLBound = 0;
	SafeArrayGetUBound( *arrSysFuncs, 1, &lUBound );
	SafeArrayGetLBound( *arrSysFuncs, 1, &lLBound );
	pInfo = NULL;
	SafeArrayAccessData( *arrSysFuncs, (void**)&pInfo);
	if( pInfo ) {
		UnsupportedCount = lUBound -lLBound + 1;
		for ( long i = lLBound; i <= lUBound;i++) {
			// add every required system function into vector
			string strName = (LPCSTR)_bstr_t(pInfo->Name);
			size_t s = FindFuncByName(strName);
			if ( s != CV_DATANOTSUPPORTED ) {
				m_mapSupportedFuncID[pInfo->ID] = m_vecInternalFuncID[s].m_DataId;
				UnsupportedCount--;
			}
			pInfo++;
		}
	}
	SafeArrayUnaccessData(*arrSysFuncs);
	*pRetVal = static_cast<LONG>(UnsupportedCount);

	return S_OK;
}

STDMETHODIMP CMmRvPosAtom::CalcCV(void)
{
	try
	{
		if ( m_spRTContext != NULL && m_nPoolID != BAD_LONG_VALUE ) 
		{
			if(!m_bVisible)
				_CHK(m_spRTContext->RemoveCVPool(m_nPoolID, TRUE));
			else
				_CHK(m_spRTContext->Recalculate(m_nPoolID));
		}
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_IMmRvUndAtom, e.Error());
	}

	return S_OK;
}

void CMmRvPosAtom::_CalcSleep(LONG nCalcSleepFreq, LONG nCalcSleepAmt)
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
				bool bBreak = false;

				if(::SetWaitableTimer(m_hCalcSleepTimer, &liDueTime, 0L, NULL, NULL, FALSE))
				{
					MSG msg;

					while(WAIT_OBJECT_0 != ::MsgWaitForMultipleObjects(1, &m_hCalcSleepTimer, FALSE, INFINITE, QS_ALLINPUT)) // QS_ALLEVENTS
					{
						while(::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
						{
							::TranslateMessage(&msg);
							::DispatchMessage(&msg);

							if(WAIT_OBJECT_0 == ::WaitForSingleObject(m_hCalcSleepTimer, 0L))
							{
								bBreak = true;
								break;
							}
						}
						::SwitchToThread();

						if(bBreak)
							break;
					}
					::CancelWaitableTimer(m_hCalcSleepTimer);
				}
				else
					::Sleep(nCalcSleepAmt);
			}

			m_nSleepStep = 0L;
		}
		m_nSleepStep++;
	}
}

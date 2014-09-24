// MmQvUndAtom.cpp : Implementation of CMmQvUndAtom

#include "stdafx.h"
#include "MmQvUndAtom.h"
#include "MmQvGrpAtom.h"
#include <math.h>
/////////////////////////////////////////////////////////////////////////////
//
HRESULT CMmQvUndAtom::rawCalcOptionGreeks(

									   CMmQvOptAtom*     pOpt, 
									   CMmQvQuoteAtom*   pQuote, 
									   const SQuoteData& undQuote,
									   CMmQvExpAtom*     pExp,
									   CMmQvOptRootAtom* pOptRoot, 
									   LONG nGreekMask, 
									   MmQvIvCalcEnum enCalcIV, 
									   EtsCalcModelTypeEnum enCalcModel, 
									   VARIANT_BOOL bUseTheoVolatility, 
									   VARIANT_BOOL bUseTheoVolaNoBid, 
									   VARIANT_BOOL bUseTheoVolaBadMarket,
									   DOUBLE dUndPriceTolerance, 
									   EtsPriceRoundingRuleEnum enPriceRoundingRule,
									   VARIANT_BOOL bUseCustomRates,
									   long						  lDivCount,
									   CSafeArrayWrapper<double>& saDates,
									   CSafeArrayWrapper<double>& saAmounts,
									   DOUBLE					  dYield,
									   DOUBLE					  dSkew,
									   DOUBLE					  dKurt,
									   bool                       bCalcDelta,
									   DATE						  dtCalcDate,
									   CVolasMapPtr				  spVolaCache,
									   VARIANT_BOOL ManualEdit,
									   ICalculationParametrs* pParams)
{
	HRESULT hr = S_OK;
	try
	{

		LONG nExchID 					= pQuote->GetExchangeID();
		LONG nQtyInShares				= pOpt->m_nQtyInShares;
		LONG nRetCount = 0;

		
		DOUBLE	dYTE		= 0.;	
		DOUBLE	dtExpiryOV	= pOpt->m_dtExpiryOV;
		DOUBLE	dtCloseTime	= pOpt->m_dtTradingClose;
		::GetCalculationParams(dtCalcDate, dtExpiryOV, dtCloseTime, pParams->UseTimePrecision != VARIANT_FALSE, &dtCalcDate, &dtExpiryOV, &dtCloseTime, &dYTE);


		if(!IsBadValue(nQtyInShares))
			nGreekMask |= GT_THEOPRICE | GT_DELTA | GT_GAMMA | GT_VEGA | GT_THETA | GT_RHO;

		if(nGreekMask != GT_NOTHING || enCalcIV != enMmQvCalcIvNone)
		{
			const CMmQvQuoteAtom* pDefQuote = pOpt->GetDefaultQuote();
			if( pDefQuote                   && 
				pQuote != pDefQuote         &&
				pDefQuote->m_dPriceBid > 0. &&
				pDefQuote->m_dPriceAsk > 0. &&
				pDefQuote->m_dPriceBid == pQuote->m_dPriceBid && 
				pDefQuote->m_dPriceAsk == pQuote->m_dPriceAsk)
			{
				pQuote->m_dPriceTheo	 = pDefQuote->m_dPriceTheo;
				pQuote->m_dDelta		 = pDefQuote->m_dDelta;
				pQuote->m_dGamma		 = pDefQuote->m_dGamma;
				pQuote->m_dTheta		 = pDefQuote->m_dTheta;
				pQuote->m_dVega			 = pDefQuote->m_dVega;
				pQuote->m_dRho			 = pDefQuote->m_dRho;
				pQuote->m_dVegaDelta	 = pDefQuote->m_dVegaDelta;
				pQuote->m_dThetaDelta	 = pDefQuote->m_dThetaDelta;
				pQuote->m_dVegaGamma	 = pDefQuote->m_dVegaGamma;
				pQuote->m_dThetaGamma	 = pDefQuote->m_dThetaGamma;
				pQuote->m_dGammaPerTheta = pDefQuote->m_dGammaPerTheta;
				pQuote->m_dIV			 = pDefQuote->m_dIV;
				pQuote->m_dIVBid		 = pDefQuote->m_dIVBid;
				pQuote->m_dIVAsk		 = pDefQuote->m_dIVAsk;
				pQuote->m_dIVSpread		 = pDefQuote->m_dIVSpread;
			}
			else
			{
				GREEKS aGreeks;
				ZeroMemory(&aGreeks, sizeof(aGreeks));
				aGreeks.nMask = nGreekMask;

				LONG nModel = static_cast<LONG>(enCalcModel);
				LONG nIsAmerican = (m_bIsAmerican ? 1L : 0L);
				DOUBLE dSpot = 0.; 

				 double* pAmmounts = saAmounts.GetPlainData();
				 double* pDates    = saDates.GetPlainData();

				VARIANT_BOOL bIsRootSynthetic = pOptRoot->m_bSynthetic;

				if(undQuote.m_dBid > 0. || undQuote.m_dAsk > 0. || undQuote.m_dPrice > 0.)
				{
					DOUBLE dOptBid = 0., dOptAsk = 0., dOptLast = 0., dOptPrice = 0., dOptBidForIv = 0.;
					EtsReplacePriceStatusEnum enMidPriceStatus = enRpsNone;

					if(enCalcIV != enMmQvCalcIvNone || VARIANT_TRUE != bUseTheoVolatility)
					{
						dOptBid			 = pQuote->m_dPriceBid;
						dOptAsk			 = pQuote->m_dPriceAsk;
						dOptLast		 = pQuote->m_dPriceLast;
						dOptBidForIv	 = dOptBid;

						if(dOptBid > 0. || dOptAsk > 0. || dOptLast > 0.)
							dOptPrice = m_spOptPriceProfile->GetOptPriceMid(dOptBid, dOptAsk, dOptLast,
							enPriceRoundingRule, bUseTheoVolatility, 0, &enMidPriceStatus);

						enMidPriceStatus = pQuote->m_enReplacePriceStatus;
					}

					if(undQuote.m_dPrice > 0. && (aGreeks.nMask != GT_NOTHING || enCalcIV != enMmQvCalcIvNone))
					{
						DOUBLE dRate = 0., dIVBid = 0., dIVAsk = 0.;
						if(bUseCustomRates)
							dRate = pExp->m_dRateCust;
						else
							dRate = pExp->m_dRate;

						DOUBLE dHTBRate = pExp->m_dHTBRate;

						DOUBLE dStrike = 0., dVolaForCalcs = 0.;
						EtsOptionTypeEnum enOptType = enOtPut;

						dStrike   = pOpt->m_dStrike;
						enOptType = pOpt->m_enOptType;

						// Sharky: new version of IV calcs
						if((enCalcIV & enMmQvCalcIv) || !bUseTheoVolatility)
						{
							DOUBLE dIV = 0.;

							if(!bUseTheoVolatility)
							{
								if(DoubleGTZero(dOptPrice))
								{
									if(!bUseTheoVolaNoBid || bUseTheoVolaNoBid && DoubleGTZero(dOptBidForIv))
									{
										LONG nFlag = VF_OK;

										if(spVolaCache==NULL || 
											!spVolaCache->GetVola(undQuote.m_dPrice, dOptPrice, dIV))
										{
											dIV = ::CalcVolatilityMM3(dRate, dYield, dHTBRate, undQuote.m_dPrice, dOptPrice, dStrike, 
												dYTE, enOptType, nIsAmerican, lDivCount,
												pAmmounts, pDates, 100L, dSkew, dKurt, nModel, &nFlag);

											if(nFlag == VF_OK && DoubleGTZero(dIV) && spVolaCache!=NULL)
												spVolaCache->PutVola(undQuote.m_dPrice, dOptPrice, dIV);
										}
								
										if((!bUseTheoVolaBadMarket || bUseTheoVolaBadMarket && nFlag == VF_OK)
											&& DoubleGTZero(dIV))
										{
											pQuote->m_nCalcGreeksIVType = enMmQvCalcGreeksIVMarket;
										}
										else if(bUseTheoVolaBadMarket)
										{
											dIV						    = pOpt->m_dVola;
											pQuote->m_nCalcGreeksIVType = enMmQvCalcGreeksIVTheo;
										}
										else
											pQuote->m_nCalcGreeksIVType = enMmQvCalcGreeksIVNone;
									}
									else
									{
										dIV						    = pOpt->m_dVola;
										pQuote->m_nCalcGreeksIVType = enMmQvCalcGreeksIVTheo;
									}
								}
								else if(bUseTheoVolaBadMarket)
								{
									dIV						    = pOpt->m_dVola;
									pQuote->m_nCalcGreeksIVType = enMmQvCalcGreeksIVTheo;
								}
								else
									pQuote->m_nCalcGreeksIVType = enMmQvCalcGreeksIVNone;

								dVolaForCalcs = (DoubleGTZero(dIV) ? dIV : BAD_DOUBLE_VALUE);
								pQuote->m_dIV = dVolaForCalcs;
							}
							else
							{
								if(DoubleGTZero(dOptPrice))
								{
									LONG nFlag = VF_OK;
									if(spVolaCache==NULL || 
										!spVolaCache->GetVola(undQuote.m_dPrice, dOptPrice, dIV))
									{
										dIV = ::CalcVolatilityMM3(dRate, dYield, dHTBRate, undQuote.m_dPrice, dOptPrice, dStrike, 
											dYTE, enOptType, nIsAmerican, lDivCount,
											pAmmounts, pDates, 100L, dSkew, dKurt, nModel, &nFlag);

										if(nFlag == VF_OK && DoubleGTZero(dIV) && spVolaCache !=NULL)
											spVolaCache->PutVola(undQuote.m_dPrice, dOptPrice, dIV);
									}
								}
								pQuote->m_dIV				= (DoubleGTZero(dIV) ? dIV : BAD_DOUBLE_VALUE);
								pQuote->m_nCalcGreeksIVType = enMmQvCalcGreeksIVTheo;
								dVolaForCalcs = pOpt->m_dVola;
							}
						}
						else
						{
							dVolaForCalcs = pOpt->m_dVola;
							pQuote->m_nCalcGreeksIVType = enMmQvCalcGreeksIVTheo;
						}


						// Calculate IV in case of Use Theo Surface and no bid & ask & last
						DOUBLE dOptBidOld = dOptBid;
						DOUBLE dOptAskOld = dOptAsk;

						if(aGreeks.nMask != GT_NOTHING && (VARIANT_TRUE == bUseTheoVolatility && nExchID == 0L || VARIANT_FALSE == bUseTheoVolatility))
						{
							nRetCount = ::CalcGreeksMM2(dRate, dYield, dHTBRate, undQuote.m_dPrice, dStrike, dVolaForCalcs, dYTE,
								enOptType, nIsAmerican, lDivCount, pAmmounts, pDates, 100L, dSkew, dKurt, nModel, &aGreeks);
						}

						if(enCalcIV & enMmQvCalcIv && bUseTheoVolatility && DoubleLEQZero(dOptPrice))
						{
							//ithreadNum2  = omp_get_thread_num();

							EtsOptBadBothPricesRuleEnum BadBothPriceRule = enObbprUseLast;
							_CHK(m_spOptPriceProfile->get_BadOptBothPriceRule(&BadBothPriceRule));

							if (BadBothPriceRule == enObbprUseTheo)
							{
								DOUBLE dIV = 0;
								if(nExchID == 0L)
								{
									const DOUBLE& dTheoPrice = aGreeks.dTheoPrice;

									if(DoubleGTZero(dTheoPrice))
									{
										LONG nFlag = 0L;

										if(spVolaCache==NULL || 
											!spVolaCache->GetVola(undQuote.m_dPrice, dTheoPrice, dIV))
										{
											dIV = ::CalcVolatilityMM3(dRate, dYield, dHTBRate, undQuote.m_dPrice, dTheoPrice, dStrike,
												dYTE, enOptType, nIsAmerican, lDivCount, pAmmounts, pDates, 100L,
												dSkew, dKurt, nModel, &nFlag);

											if(nFlag == VF_OK && DoubleGTZero(dIV) && spVolaCache!=NULL)
												spVolaCache->PutVola(undQuote.m_dPrice, dTheoPrice, dIV);
										}
									}
								}
								else
								{
									const CMmQvQuoteAtom* pDefQuote = pOpt->GetDefaultQuote();
									ATLASSERT(pDefQuote != NULL);
									dIV = pDefQuote->m_dIV;// >get_IV(&dIV));
								}
								pQuote->m_dIV = DoubleGTZero(dIV) ? dIV : BAD_DOUBLE_VALUE;
							}
						}
						if(enCalcIV & enMmQvCalcIvBid)
						{
							if(DoubleGTZero(undQuote.m_dBid))
							{	
								EtsReplacePriceStatusEnum enOldPriceStatus = enRpsNone;
								EtsReplacePriceStatusEnum enBidPriceStatus = enRpsNone;

								dOptBid = m_spOptPriceProfile->GetOptPriceBid(dOptBidOld, dOptAskOld, dOptLast, enPriceRoundingRule, 
									bUseTheoVolatility, (_finite(aGreeks.dTheoPrice) && aGreeks.dTheoPrice > 0.) ? aGreeks.dTheoPrice : 0., &enBidPriceStatus);

								if(enBidPriceStatus != enRpsNone)
								{
									enOldPriceStatus =  pQuote->m_enReplacePriceStatus;
									pQuote->m_enReplacePriceStatus = static_cast<EtsReplacePriceStatusEnum>(enOldPriceStatus | enBidPriceStatus);
								}

								if(DoubleGTZero(dOptBid))
								{
									LONG nFlag = VF_OK;

									if(spVolaCache==NULL || 
										!spVolaCache->GetVola(undQuote.m_dBid, dOptBid, dIVBid))
									{
										dIVBid = ::CalcVolatilityMM3(dRate, dYield, dHTBRate, undQuote.m_dBid, dOptBid, dStrike, 
											dYTE, enOptType, nIsAmerican,
											lDivCount, pAmmounts, pDates, 100L,
											dSkew, dKurt, nModel, &nFlag);

										if(nFlag == VF_OK && DoubleGTZero(dOptBid) && spVolaCache!=NULL)
											spVolaCache->PutVola(undQuote.m_dBid, dOptBid, dIVBid);
									}

									if (VARIANT_FALSE == bUseTheoVolatility && bUseTheoVolaBadMarket == VARIANT_TRUE && nFlag != VF_OK)
										dIVBid = pOpt->m_dVola;
								}
							}

							pQuote->m_dIVBid = DoubleGTZero(dIVBid) ? dIVBid : BAD_DOUBLE_VALUE;
						}

						if(enCalcIV & enMmQvCalcIvAsk)
						{
							if(DoubleGTZero(undQuote.m_dAsk))
							{
								EtsReplacePriceStatusEnum enOldPriceStatus = enRpsNone;
								EtsReplacePriceStatusEnum enAskPriceStatus = enRpsNone;

								dOptAsk = m_spOptPriceProfile->GetOptPriceAsk(dOptBidOld, dOptAskOld, dOptLast,
									enPriceRoundingRule, bUseTheoVolatility,
									(_finite(aGreeks.dTheoPrice) && DoubleGTZero(aGreeks.dTheoPrice))
									? aGreeks.dTheoPrice : 0., &enAskPriceStatus);

								if(enAskPriceStatus != enRpsNone)
								{
									enOldPriceStatus = pQuote->m_enReplacePriceStatus;
									pQuote->m_enReplacePriceStatus = static_cast<EtsReplacePriceStatusEnum>(enOldPriceStatus | enAskPriceStatus);
								}

								if(DoubleGTZero(dOptAsk))
								{
									LONG nFlag = VF_OK;

									if(spVolaCache==NULL || 
										!spVolaCache->GetVola(undQuote.m_dAsk, dOptAsk, dIVAsk))
									{
										dIVAsk = ::CalcVolatilityMM3(dRate, dYield, dHTBRate, undQuote.m_dAsk, dOptAsk, dStrike, 
											dYTE, enOptType, nIsAmerican,
											lDivCount, pAmmounts, pDates, 100L,
											dSkew, dKurt, nModel, &nFlag);

										if(nFlag == VF_OK && DoubleGTZero(dIVAsk) && spVolaCache!=NULL)
											spVolaCache->PutVola(undQuote.m_dAsk, dOptAsk, dIVAsk);

									}

									if(VARIANT_FALSE == bUseTheoVolatility && bUseTheoVolaBadMarket == VARIANT_TRUE && nFlag != VF_OK)
										dIVAsk = pOpt->m_dVola;
								}
							}

							pQuote->m_dIVAsk = DoubleGTZero(dIVAsk) ? dIVAsk : BAD_DOUBLE_VALUE;
						}

						if(DoubleGTZero(dIVBid) && DoubleGTZero(dIVAsk))
							pQuote->m_dIVSpread = dIVBid - dIVAsk;
						else
							pQuote->m_dIVSpread = BAD_DOUBLE_VALUE;

						if(aGreeks.nMask != GT_NOTHING &&
							(VARIANT_FALSE != bUseTheoVolatility && nExchID == 0L || VARIANT_FALSE == bUseTheoVolatility))
						{
							pQuote->ClearValues(VARIANT_FALSE, VARIANT_FALSE);


							if ((aGreeks.dTheoPrice > BAD_DOUBLE_VALUE)&&(aGreeks.dTheoPrice < 0.))
								aGreeks.dTheoPrice = 0;

							if((aGreeks.nMask & GT_THEOPRICE) && _finite(aGreeks.dTheoPrice) && aGreeks.dTheoPrice >= 0.)
								pQuote->m_dPriceTheo = aGreeks.dTheoPrice;
							if((aGreeks.nMask & GT_DELTA) && _finite(aGreeks.dDelta))
							{	 
								if(m_enUndType == enCtIndex && dYTE <=0 )
									pQuote->m_dDelta = 0.;
								else
								if(bCalcDelta && DoubleEQZero(aGreeks.dDelta)&&
									((enOptType == enOtPut && undQuote.m_dPrice < dStrike) || (enOptType == enOtCall && undQuote.m_dPrice > dStrike)))
								{
									if (enOptType == enOtPut)
										pQuote->m_dDelta = -100.;
									else
										pQuote->m_dDelta = 100.;
								}
								else
									pQuote->m_dDelta = aGreeks.dDelta * 100.;
							}
							if((aGreeks.nMask & GT_GAMMA) && _finite(aGreeks.dGamma))
								pQuote->m_dGamma = aGreeks.dGamma * 100.;
							if((aGreeks.nMask & GT_THETA) && _finite(aGreeks.dTheta))
								pQuote->m_dTheta = aGreeks.dTheta * 100.;
							if((aGreeks.nMask & GT_VEGA) && _finite(aGreeks.dVega))
								pQuote->m_dVega = aGreeks.dVega * 100.;
							if((aGreeks.nMask & GT_RHO) && _finite(aGreeks.dRho))
								pQuote->m_dRho = aGreeks.dRho * 100.;
							if((aGreeks.nMask & GT_DELTA_VEGA) && _finite(aGreeks.dDeltaVega))
								pQuote->m_dVegaDelta = aGreeks.dDeltaVega * 100.;
							if((aGreeks.nMask & GT_GAMMA_VEGA) && _finite(aGreeks.dGammaVega))
								pQuote->m_dVegaGamma = aGreeks.dGammaVega * 100.;
							if((aGreeks.nMask & GT_DELTA_THETA) && _finite(aGreeks.dDeltaTheta))
								pQuote->m_dThetaDelta = aGreeks.dDeltaTheta * 100.;
							if((aGreeks.nMask & GT_GAMMA_THETA) && _finite(aGreeks.dGammaTheta))
								pQuote->m_dThetaGamma = aGreeks.dGammaTheta * 100.;
							if((aGreeks.nMask & GT_GAMMA) && (aGreeks.nMask & GT_THETA) 
								&& _finite(aGreeks.dGamma) && _finite(aGreeks.dTheta)
								&& fabs(aGreeks.dTheta) >= .0001)
								pQuote->m_dGammaPerTheta = aGreeks.dGamma / aGreeks.dTheta;
							//}
						}
						else if(aGreeks.nMask != GT_NOTHING && VARIANT_FALSE != bUseTheoVolatility && nExchID != 0)
						{
							const CMmQvQuoteAtom* pDefQuote = pOpt->GetDefaultQuote();
							ATLASSERT(pDefQuote != NULL);

							pQuote->m_dPriceTheo	 = pDefQuote->m_dPriceTheo;
							pQuote->m_dDelta		 = pDefQuote->m_dDelta;
							pQuote->m_dGamma		 = pDefQuote->m_dGamma;
							pQuote->m_dTheta		 = pDefQuote->m_dTheta;
							pQuote->m_dVega			 = pDefQuote->m_dVega;
							pQuote->m_dRho			 = pDefQuote->m_dRho;
							pQuote->m_dVegaDelta	 = pDefQuote->m_dVegaDelta;
							pQuote->m_dThetaDelta	 = pDefQuote->m_dThetaDelta;
							pQuote->m_dVegaGamma	 = pDefQuote->m_dVegaGamma;
							pQuote->m_dThetaGamma	 = pDefQuote->m_dThetaGamma;
							pQuote->m_dGammaPerTheta = pDefQuote->m_dGammaPerTheta;
							pQuote->m_dIVBid		 = pDefQuote->m_dIVBid;
							pQuote->m_dIVAsk		 = pDefQuote->m_dIVAsk;
							pQuote->m_dIVSpread		 = pDefQuote->m_dIVSpread;
						}
					}
				}				
			}
		}
		pQuote->SetProcessed();
	}
	catch(const _com_error& e)
	{
		hr = Error((PTCHAR)CComErrorWrapper::ErrorDescription(e), IID_IMmQvUndAtom, e.Error());
	}
	catch(...)
	{
		HRESULT _hr = HRESULT_FROM_WIN32(::GetLastError());
		if(SUCCEEDED(_hr)) _hr = E_FAIL;
		hr = Error((PTCHAR)"System Error", IID_IMmQvUndAtom, _hr);
	}

	return hr;
}

STDMETHODIMP CMmQvUndAtom::GetUnderlyingPrice(DOUBLE dTolerance, 
												 EtsPriceRoundingRuleEnum enPriceRound,
												 EtsReplacePriceStatusEnum * penPriceStatus,
												 VARIANT_BOOL *bFutureUsed, 
												 DOUBLE	*pPrice)
{
	HRESULT						hr = S_OK;
	CComBSTR					futureSymbol;
	bool						dontUseFuture = true;
	IEtsPriceProfileAtomPtr		activeFuturePriceProfile;
	double						activeFuturePrice = 0.;

	if (!bFutureUsed || !pPrice )		return E_POINTER;

	*bFutureUsed = VARIANT_FALSE;
	*pPrice = 0.;

	try
	{

		if ( m_bPriceByHead && m_spHeadComponent )
		{
			_CHK(m_spHeadComponent->GetUnderlyingPrice(dTolerance, enPriceRound, penPriceStatus, bFutureUsed, pPrice));
			if ( m_dCoeff > 0. ){	
				*pPrice *= m_dCoeff;
				SetDirty();
			}
			dontUseFuture	=	false;
			*bFutureUsed	=	VARIANT_TRUE;
		}
		else if ( m_spActiveFuture )
		{
			_CHK(m_spActiveFuture->get_Symbol(&futureSymbol));

			if ( futureSymbol.Length() ){
				double activeFuturePriceBid = 0., activeFuturePriceAsk = 0., activeFuturePriceLast = 0.;
				VARIANT_BOOL bUseManualPrice = VARIANT_FALSE;

				// we have active future for this underlying
				_CHK(m_spActiveFuture->get_UndPriceProfile(&activeFuturePriceProfile));

				if ( activeFuturePriceProfile ){
					IMmQvQuoteCollPtr futureQuoteColl;
					_CHK( m_spActiveFuture->get_Quote(&futureQuoteColl) ); 

					IMmQvQuoteAtomPtr futureQuote;
					_CHK( futureQuoteColl->get_Item(0L, &futureQuote) );

					_CHK(futureQuote->get_PriceBid(&activeFuturePriceBid));
					_CHK(futureQuote->get_PriceAsk(&activeFuturePriceAsk));
					_CHK(futureQuote->get_PriceLast(&activeFuturePriceLast));
					activeFuturePrice = activeFuturePriceProfile->GetUndPriceMid(	activeFuturePriceBid,
																										activeFuturePriceAsk,
																										activeFuturePriceLast,
																										dTolerance,
																										enPriceRound,
																										penPriceStatus, VARIANT_FALSE);

					m_spActiveFuture->get_IsUseManualActivePrice(&bUseManualPrice);

					if ( bUseManualPrice )
						m_spActiveFuture->get_ActivePrice(&activeFuturePrice);
					else
						m_spActiveFuture->put_ActivePrice(activeFuturePrice);

					if ( activeFuturePrice > 0)
					{
						double dFutureBasis = BAD_DOUBLE_VALUE;
						if(m_spActiveFuture)
							m_spActiveFuture->get_Basis(&dFutureBasis);
					
						*pPrice = activeFuturePrice + ( dFutureBasis > BAD_DOUBLE_VALUE ? dFutureBasis : 0.);
						*bFutureUsed = VARIANT_TRUE;
						dontUseFuture = false;
					}
					else
					{
						*pPrice = BAD_DOUBLE_VALUE;
						*bFutureUsed = VARIANT_TRUE;
						dontUseFuture = false;						
					}
				}
			}
		}
		else if ( m_bUseManualActivePrice )
		{
			*pPrice			=	m_dActivePrice;
			*bFutureUsed	=	VARIANT_TRUE;
			dontUseFuture	=	false;
			SetDirty();
		}
		//use quote for underlying
		if ( dontUseFuture )	
		{
			IMmQvQuoteAtomPtr undQuote;
			double spotPriceBid = 0., spotPriceAsk = 0., spotPriceLast = 0.;

			_CHK(m_spQuote->get_Item(m_nPrimaryExchangeID, &undQuote));

			_CHK(undQuote->get_PriceBid(&spotPriceBid ));
			_CHK(undQuote->get_PriceAsk(&spotPriceAsk ));
			_CHK(undQuote->get_PriceLast(&spotPriceLast));

			*pPrice = m_spUndPriceProfile->GetUndPriceMid(	spotPriceBid,
																			spotPriceAsk,
																			spotPriceLast,
																			dTolerance,
																			enPriceRound,
																			penPriceStatus, VARIANT_FALSE);
		}
		if(SUCCEEDED(hr) && *pPrice > 0)
			m_dActivePrice = *pPrice;
	}
	catch ( _com_error& e) {
		hr = Error((PTCHAR)CComErrorWrapper::ErrorDescription(e), __uuidof(IMmQvUndAtom), e.Error());
	}
	return hr;
}


STDMETHODIMP CMmQvUndAtom::CalcOptionGreeks(IMmQvOptAtom* aOpt, 
											IMmQvQuoteAtom* aQuote, 
											IMmQvExpAtom* aExp,
											IMmQvOptRootAtom* aOptRoot, 
											LONG nGreekMask, 
											MmQvIvCalcEnum enCalcIV, 
											EtsCalcModelTypeEnum enCalcModel, 
											VARIANT_BOOL bUseTheoVolatility, 
											VARIANT_BOOL bUseTheoVolaNoBid, 
											VARIANT_BOOL bUseTheoVolaBadMarket,
											DOUBLE dUndPriceTolerance, 
											EtsPriceRoundingRuleEnum enPriceRoundingRule,
											VARIANT_BOOL bUseCustomRates, 
											DOUBLE dtCalcDate,
											VARIANT_BOOL ManualEdit,
											ICalculationParametrs* pParams)
{
	if(aOpt == NULL || aQuote == NULL || aExp == NULL || aOptRoot == NULL)
		return Error(L"Invalid objects passed.", IID_IMmQvUndAtom, E_INVALIDARG);

	try
	{
		CMmQvOptAtom*     pOptAtom   = dynamic_cast<CMmQvOptAtom*>(aOpt);
		CMmQvQuoteAtom*	  pQuoteAtom = dynamic_cast<CMmQvQuoteAtom*>(aQuote); 
		CMmQvExpAtom*	  pExp       = dynamic_cast<CMmQvExpAtom*>(aExp);
		CMmQvOptRootAtom* pOptRoot   = dynamic_cast<CMmQvOptRootAtom*>(aOptRoot); 
		CSafeArrayWrapper<double> saDates;
		CSafeArrayWrapper<double> saAmounts;

		DOUBLE	dtExpiryOV		=	pExp->m_dtExpiryOV;
		DOUBLE	dtTradingClose	=	pExp->m_dtTradingClose;
		DOUBLE	dtCalcNow		=	dtCalcDate;
		DOUBLE	dYTE			;

		::GetCalculationParams(dtCalcNow, dtExpiryOV, dtTradingClose, pParams->UseTimePrecision != VARIANT_FALSE, &dtCalcNow, &dtExpiryOV, &dtTradingClose, &dYTE);

		bool	bCalcDelta		=	(dYTE <= 0.);


		long   nDivCount = 0L;
		double dYield    = 0.;

		VARIANT_BOOL bIsRootSynthetic = pOptRoot->m_bSynthetic;
		if(!bIsRootSynthetic)
		{

			EtsDivTypeEnum enDivType = enDivCustomStream;
			if(m_spDividend)
				m_spDividend->get_DivType(&enDivType);

			switch(enDivType)
			{
			case enDivMarket:
			case enDivCustomPeriodical:
			case enDivCustomStream:
				{
					if (m_spDividend != NULL)
					{
						nDivCount = 0;
						m_spDividend->GetDividendCount2(dtCalcNow, dtExpiryOV, dtTradingClose, &nDivCount);
						if (nDivCount< 0)
							nDivCount = 0;

						if (nDivCount> 0)
						{
							LPSAFEARRAY psaAmounts = NULL;
							LPSAFEARRAY psaDates   = NULL;
							m_spDividend->GetDividends2(dtCalcNow, dtExpiryOV, dtTradingClose, nDivCount, &psaAmounts, &psaDates, &nDivCount);

							saAmounts.Attach(psaAmounts);
							saDates.Attach(psaDates);
						}
					}
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

						if ( bIsBasket && spDivColl != NULL)
						{
							spDivColl->GetDividendCount2(dtCalcNow, dtExpiryOV, dtTradingClose, &nDivCount);
							if(nDivCount > 0L)
							{
								LPSAFEARRAY psaAmounts = NULL;
								LPSAFEARRAY psaDates   = NULL;

								spDivColl->GetDividends2(dtCalcNow, dtExpiryOV, dtTradingClose,  nDivCount, &psaAmounts, &psaDates, &nDivCount);

								saAmounts.Attach(psaAmounts);
								saDates.Attach(psaDates);
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
		else
		{
			VARIANT_BOOL bIsBasket = VARIANT_FALSE;
			ISynthRootAtomPtr spSynthOptRoot;
			spSynthOptRoot = pOptRoot->m_spSynthOptRoot;

			_CHK(spSynthOptRoot->get_Basket(&bIsBasket));
			LONG nDivCount = 0L, nRetCount = 0L;

			if(bIsBasket)
			{
				LONG nBaskDivCount = 0L; VARIANT_BOOL bIsBasket = VARIANT_FALSE;

				dYield = 0.;

				IEtsIndexDivCollPtr spBasketDivs;
				spSynthOptRoot->get_BasketDivs(&spBasketDivs);
				nDivCount = 0;
				spBasketDivs->GetDividendCount2(dtCalcNow, dtExpiryOV, dtTradingClose, &nDivCount);
				if(nDivCount > 0L) 
				{

					LPSAFEARRAY psaAmounts = NULL;
					LPSAFEARRAY psaDates   = NULL;

					spBasketDivs->GetDividends2(dtCalcNow, dtExpiryOV, dtTradingClose, nDivCount, &psaAmounts, &psaDates, &nDivCount);

					saAmounts.Attach(psaAmounts);
					saDates.Attach(psaDates);
				} 

			}
		}

		IMmQvQuoteAtomPtr spUndQuote = m_pQuote->GetQuote(m_nPrimaryExchangeID);
		CMmQvQuoteAtom* pUndQuote = dynamic_cast<CMmQvQuoteAtom*>(spUndQuote.GetInterfacePtr());
		SQuoteData undQuote;


		double dCalcYield    = 0.0;
		double dCalcSkew     = 0.0;
		double dCalcCurtosis = 0.0;

		if(!pOptRoot->m_bSynthetic)
		{
			dCalcYield    = dYield;
			dCalcSkew     = m_dSkew;
			dCalcCurtosis = m_dKurt;


			VARIANT_BOOL futureUsed = VARIANT_FALSE;
			EtsReplacePriceStatusEnum enMidPriceStatus = enRpsNone;
			double dFutSpotPrice = 0;

			_CHK(GetUnderlyingPrice(dUndPriceTolerance, enPriceRoundingRule, &enMidPriceStatus, &futureUsed, &dFutSpotPrice ));

			if(futureUsed)
			{
				undQuote.m_dBid   = dFutSpotPrice;
				undQuote.m_dAsk   = dFutSpotPrice;
				undQuote.m_dPrice = dFutSpotPrice;
				_CHK(spUndQuote->put_ReplacePriceStatus((EtsReplacePriceStatusEnum)(enMidPriceStatus)));
			}
			else
			{
				undQuote.m_dBid   = pUndQuote->m_dPriceBid;
				undQuote.m_dAsk   = pUndQuote->m_dPriceAsk;
				undQuote.m_dPrice = pUndQuote->m_dPriceLast;

				if(undQuote.m_dBid > 0. || undQuote.m_dAsk > 0. || undQuote.m_dPrice > 0.)
				{
					EtsReplacePriceStatusEnum enBidPriceStatus = enRpsNone;
					EtsReplacePriceStatusEnum enAskPriceStatus = enRpsNone;

					enMidPriceStatus = enRpsNone;
					DOUBLE dSpotPrice = m_spUndPriceProfile->GetUndPriceMid(undQuote.m_dBid, undQuote.m_dAsk, undQuote.m_dPrice, dUndPriceTolerance, enPriceRoundingRule, &enMidPriceStatus, ManualEdit);

					DOUBLE dSpotBidOld = undQuote.m_dBid;

					undQuote.m_dBid = m_spUndPriceProfile->GetUndPriceBid(undQuote.m_dBid, undQuote.m_dAsk, undQuote.m_dPrice, dUndPriceTolerance, enPriceRoundingRule, &enBidPriceStatus);
					undQuote.m_dAsk = m_spUndPriceProfile->GetUndPriceAsk(dSpotBidOld,     undQuote.m_dAsk, undQuote.m_dPrice, dUndPriceTolerance, enPriceRoundingRule, &enAskPriceStatus);
					undQuote.m_dPrice = dSpotPrice;
					_CHK(pUndQuote->put_ReplacePriceStatus((EtsReplacePriceStatusEnum)(enMidPriceStatus | enBidPriceStatus | enAskPriceStatus)));
				}
			}
		}
		else
		{
			_CHK(_GetSyntheticUnderlyingPrice(pOptRoot->m_spSynthOptRoot, undQuote.m_dBid, undQuote.m_dAsk, undQuote.m_dPrice, dUndPriceTolerance, enPriceRoundingRule));
	
			if(undQuote.m_dBid > 0. || undQuote.m_dAsk > 0. || undQuote.m_dPrice > 0.)
			{
				DOUBLE dSpotPrice = m_spUndPriceProfile->GetUndPriceMid(undQuote.m_dBid, undQuote.m_dAsk, undQuote.m_dPrice, dUndPriceTolerance, enPriceRoundingRule, NULL, ManualEdit);

				if(dSpotPrice > 0 )
					pOptRoot->m_dSU_Price = dSpotPrice;
				else
					pOptRoot->m_dSU_Price = BAD_DOUBLE_VALUE;

				DOUBLE dSpotBidOld = undQuote.m_dBid;

				undQuote.m_dBid = m_spUndPriceProfile->GetUndPriceBid(undQuote.m_dBid, undQuote.m_dAsk, undQuote.m_dPrice, dUndPriceTolerance, enPriceRoundingRule, NULL);
				undQuote.m_dAsk = m_spUndPriceProfile->GetUndPriceAsk(dSpotBidOld,     undQuote.m_dAsk, undQuote.m_dPrice, dUndPriceTolerance, enPriceRoundingRule, NULL);
				undQuote.m_dPrice = dSpotPrice;
			}

			_CHK(pOptRoot->m_spSynthOptRoot->get_Skew(&dCalcSkew));
			_CHK(pOptRoot->m_spSynthOptRoot->get_Kurt(&dCalcCurtosis));

			if(nDivCount!=0)
				_CHK(pOptRoot->m_spSynthOptRoot->get_Yield(&dCalcYield));

		}
		CVolasMapPtr  volasCache(new CVolasMap);
		return rawCalcOptionGreeks
			(
				pOptAtom,
				pQuoteAtom, 
				undQuote,
				pExp,
				pOptRoot, 
				nGreekMask, 
				enCalcIV, 
				enCalcModel, 
				bUseTheoVolatility, 
				bUseTheoVolaNoBid, 
				bUseTheoVolaBadMarket,
				dUndPriceTolerance, 
				enPriceRoundingRule,
				bUseCustomRates, 
				nDivCount,
				saDates,
				saAmounts,
				dCalcYield,
				dCalcSkew,
				dCalcCurtosis,
				bCalcDelta,
				dtCalcNow,
				volasCache,
				ManualEdit,
				pParams);

	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)CComErrorWrapper::ErrorDescription(e), IID_IMmQvUndAtom, e.Error());
	}
	catch(...)
	{
		HRESULT _hr = HRESULT_FROM_WIN32(::GetLastError());
		if(SUCCEEDED(_hr)) _hr = E_FAIL;
		return Error((PTCHAR)"System Error", IID_IMmQvUndAtom, _hr);
	}
	return S_OK;
}

/////////////////////////////////////////////////////////////////////////////
//
void CMmQvUndAtom::_GetBasketIndexDividends(REGULAR_DIVIDENDS* pDivs, LONG nMaxCount) throw()
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

/*
///////////////////////////////////////////////////////////////////////////

void CMmQvUndAtom::_GetSyntheticRootBasketDividends(ISynthRootAtomPtr aSynthRoot, REGULAR_DIVIDENDS* pDivs, 
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
*/

/////////////////////////////////////////////////////////////////////////////
//
HRESULT CMmQvUndAtom::_GetSyntheticUnderlyingPrice(ISynthRootAtomPtr aSynthRoot, DOUBLE& dSpotBid, DOUBLE& dSpotAsk, DOUBLE& dSpotLast,
				DOUBLE dUndPriceTolerance, EtsPriceRoundingRuleEnum enPriceRoundingRule)
{
	try
	{
		HRESULT hr;
		IMmQvUndAtomPtr spSynthUnd;
		ISynthRootCompCollPtr spSynthRootCompColl;
		IMmQvQuoteAtomPtr spQuote;
		IMmQvQuoteCollPtr spQuotes;
		bool bBadSpotBid = false, bBadSpotAsk = false, bBadSpotLast = false;
		
		dSpotBid = 0.;
		dSpotAsk = 0.;
		dSpotLast = 0.;

		_CHK(aSynthRoot->get_CashValue(&dSpotBid));
		dSpotAsk = dSpotBid;
		dSpotLast = dSpotBid;

		IUnknownPtr spUnk;
		_variant_t varItem;
		ULONG nFetched = 0L;
			
		_CHK(aSynthRoot->get_SynthRootComponents(&spSynthRootCompColl), _T("Fail to get synthetic underlying component collection."));

		ISynthRootCompAtomPtr spSRComp;
	
		_CHK(spSynthRootCompColl->get__NewEnum(&spUnk), _T("Fail to get synthetic underlying component collection enum."));
		IEnumVARIANTPtr spSynthUndCompEnum(spUnk);
		_CHK(spSynthUndCompEnum->Reset(), _T("Fail to reset synthetic underlying component collection."));
		while((hr = spSynthUndCompEnum->Next(1L, &varItem, &nFetched)) == S_OK)
		{
			LONG nUndID;
			DOUBLE dWeight;
			DOUBLE dPriceBid = 0.;
			DOUBLE dPriceAsk = 0.;
			DOUBLE dPriceLast = 0.;
			DOUBLE dPriceMid = 0.;

			ATLASSERT(varItem.vt == VT_DISPATCH);
			spSRComp = varItem;
			
			spSRComp->get_UndID(&nUndID);
			spSRComp->get_Weight(&dWeight);
			LONG	lPrimaryExchangeID	=	0;

			if(nUndID != m_nID)
			{
				if(m_spSynthUnd == NULL)
					CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Null synthetic underlying collection ."));

				_CHK(m_spSynthUnd->get_Item(nUndID,&spSynthUnd),_T("Fail to get synthetic underlying."));
				
				if(spSynthUnd != NULL)
				{

					spSynthUnd->get_PrimaryExchangeID(&lPrimaryExchangeID);
					_CHK(spSynthUnd->get_Quote(&spQuotes), _T("Fail to get quotes collection."));
					_CHK(spQuotes->get_Item(lPrimaryExchangeID, &spQuote)); // get default exchange quote

					VARIANT_BOOL bIsManual = VARIANT_FALSE;
					VARIANT_BOOL bPriceByHead = VARIANT_FALSE;
					double dManulPrice = BAD_DOUBLE_VALUE;
					_CHK(spSynthUnd->get_UseManualActivePrice(&bIsManual));
					_CHK(spSynthUnd->get_PriceByHead(&bPriceByHead));
					_CHK(spSynthUnd->get_ActivePrice(&dManulPrice));

					_CHK(spQuote->get_PriceBid(&dPriceBid));
					dPriceBid = (bIsManual || bPriceByHead) ? dManulPrice : dPriceBid;
					if(!bBadSpotBid && dPriceBid > 0.)
						dSpotBid += dPriceBid * dWeight;
					else
					{
						bBadSpotBid = true;
						dSpotBid = 0.;
					}

					_CHK(spQuote->get_PriceAsk(&dPriceAsk));
					dPriceAsk = (bIsManual || bPriceByHead) ? dManulPrice : dPriceAsk;
					if(!bBadSpotAsk && dPriceAsk > 0.)
						dSpotAsk += dPriceAsk * dWeight;
					else
					{
						bBadSpotAsk = true;
						dSpotAsk = 0.;
					}

					_CHK(spQuote->get_PriceLast(&dPriceLast));
					dPriceLast = (bIsManual || bPriceByHead) ? dManulPrice : dPriceLast;
					if(!bBadSpotLast && dPriceLast > 0.)
						dSpotLast += dPriceLast * dWeight;
					else
					{
						bBadSpotLast = true;
						dSpotLast = 0.;
					}

					EtsReplacePriceStatusEnum enMidPriceStatus = enRpsNone;

					dPriceMid = m_spUndPriceProfile->GetUndPriceMid(dPriceBid, dPriceAsk, dPriceLast, 
						dUndPriceTolerance, enPriceRoundingRule, &enMidPriceStatus, FALSE);

					_CHK(spQuote->put_ReplacePriceStatus(enMidPriceStatus));
				}
			}
			else
			{

				_CHK(m_spQuote->get_Item(m_nPrimaryExchangeID, &spQuote));
				//_CHK(m_spQuote->get_Item(0L, &spQuote)); // get default exchange quote

				_CHK(spQuote->get_PriceBid(&dPriceBid));
				dPriceBid = (m_bUseManualActivePrice || m_bPriceByHead) ? m_dActivePrice : dPriceBid;
				if(!bBadSpotBid && dPriceBid > 0.)
					dSpotBid += dPriceBid * dWeight;
				else
				{
					bBadSpotBid = true;
					dSpotBid = 0.;
				}

				_CHK(spQuote->get_PriceAsk(&dPriceAsk));
				dPriceAsk = (m_bUseManualActivePrice || m_bPriceByHead) ? m_dActivePrice : dPriceAsk;
				if(!bBadSpotAsk && dPriceAsk > 0.)
					dSpotAsk += dPriceAsk * dWeight;
				else
				{
					bBadSpotAsk = true;
					dSpotAsk = 0.;
				}

				_CHK(spQuote->get_PriceLast(&dPriceLast));
				dPriceLast =(m_bUseManualActivePrice || m_bPriceByHead)? m_dActivePrice : dPriceLast;
				if(!bBadSpotLast && dPriceLast > 0.)
					dSpotLast += dPriceLast * dWeight;
				else
				{
					bBadSpotLast = true;
					dSpotLast = 0.;
				}

				EtsReplacePriceStatusEnum enMidPriceStatus = enRpsNone;

				dPriceMid = m_spUndPriceProfile->GetUndPriceMid(dPriceBid, dPriceAsk, dPriceLast, 
					dUndPriceTolerance, enPriceRoundingRule, &enMidPriceStatus, FALSE);

				_CHK(spQuote->put_ReplacePriceStatus(enMidPriceStatus));
			}
		}
		return S_OK;
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)CComErrorWrapper::ErrorDescription(e), IID_IMmQvUndAtom, e.Error());
	}
	catch(...)
	{
		HRESULT _hr = HRESULT_FROM_WIN32(::GetLastError());
		if(SUCCEEDED(_hr)) _hr = E_FAIL;
		return Error((PTCHAR)"System Error", IID_IMmQvUndAtom, _hr);
	}
	return S_OK;
}

/////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CMmQvUndAtom::CalcAllOptions(LONG nCallGreekMask, LONG nPutGreekMask, MmQvIvCalcEnum enCallCalcIV,
										  MmQvIvCalcEnum enPutCalcIV, EtsCalcModelTypeEnum enCalcModel,
										  VARIANT_BOOL bUseTheoVolatility, VARIANT_BOOL bUseTheoVolaNoBid, 
										  VARIANT_BOOL bUseTheoVolaBadMarket, VARIANT_BOOL bRecalcGreeks, 
										  IMmQvOptRootColl* collSynthRoots, DOUBLE dUndPriceTolerance, 
										  EtsPriceRoundingRuleEnum enPriceRoundingRule, VARIANT_BOOL bUseCustomRates,
										  LONG nCalcSleepFreq, LONG nCalcSleepAmt, DOUBLE dtCalcDate, VARIANT_BOOL ManualEdit,
										  VARIANT_BOOL bForceRecalc, ICalculationParametrs* pParams)
{
	HRESULT hr = S_OK;
	//check calculations in progress
	try
	{
		if(IsInCalc())
		{
			const DWORD dwCurrentTime = GetTickCount();
			// if calculations was denied for at least 30 seconds - ignore abort rule
			if(m_dwAbortTime == 0 || (dwCurrentTime-m_dwAbortTime) <= 30000)
			{
				if(m_dwAbortTime == 0)
					m_dwAbortTime = dwCurrentTime;

				CComErrorWrapper::ThrowError(HRESULT_FROM_WIN32(ERROR_REQUEST_ABORTED), _T("Calculation is in progress"));
			}
		}
		::InterlockedExchange(&m_nCalculations, 1L);

		m_bStopCalc = FALSE;
		ResetEvent(m_eventComplete);
		ResetEvent(m_eventStopCalc);


		m_dNetDelta   = BAD_DOUBLE_VALUE;
		m_dNetDeltaEq = BAD_DOUBLE_VALUE;
		m_dTotalDelta = BAD_DOUBLE_VALUE;
		m_dTotalGamma = BAD_DOUBLE_VALUE;
		m_dTotalTheta = BAD_DOUBLE_VALUE;
		m_dTotalVega = BAD_DOUBLE_VALUE;
		m_dTotalRho = BAD_DOUBLE_VALUE;

		switch(m_enUndType)
		{
		case enCtFutUnd:
			{
				hr = CalcFutUnderlyingOptions(nCallGreekMask, nPutGreekMask, enCallCalcIV, enPutCalcIV, enCalcModel,
					bUseTheoVolatility, bUseTheoVolaNoBid, bUseTheoVolaBadMarket, bRecalcGreeks, 
					dUndPriceTolerance, enPriceRoundingRule, bUseCustomRates,
					nCalcSleepFreq, nCalcSleepAmt, dtCalcDate, ManualEdit, bForceRecalc, pParams);

			}break;
		case enCtIndex:
			{
				if(m_pGrp && m_pGrp->m_bShowIndexFutureOptions)
					hr = CalcFutUnderlyingOptions(nCallGreekMask, nPutGreekMask, enCallCalcIV, enPutCalcIV, enCalcModel,
					bUseTheoVolatility, bUseTheoVolaNoBid, bUseTheoVolaBadMarket, bRecalcGreeks, 
					dUndPriceTolerance, enPriceRoundingRule, bUseCustomRates,
					nCalcSleepFreq, nCalcSleepAmt, dtCalcDate, ManualEdit, bForceRecalc, pParams);
				else
					hr = CalcEquityOptions( nCallGreekMask, nPutGreekMask, enCallCalcIV,
					enPutCalcIV, enCalcModel,  bUseTheoVolatility, bUseTheoVolaNoBid, 
					bUseTheoVolaBadMarket, bRecalcGreeks, collSynthRoots, dUndPriceTolerance, 
					enPriceRoundingRule, bUseCustomRates, nCalcSleepFreq, nCalcSleepAmt, dtCalcDate, ManualEdit, bForceRecalc, pParams);
			}break;
		case enCtStock:
			{
				hr = CalcEquityOptions( nCallGreekMask, nPutGreekMask, enCallCalcIV,
					enPutCalcIV, enCalcModel,  bUseTheoVolatility, bUseTheoVolaNoBid, 
					bUseTheoVolaBadMarket, bRecalcGreeks, collSynthRoots, dUndPriceTolerance, 
					enPriceRoundingRule, bUseCustomRates, nCalcSleepFreq, nCalcSleepAmt, dtCalcDate, ManualEdit, bForceRecalc, pParams);

			}break;
		}
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)CComErrorWrapper::ErrorDescription(e), IID_IMmQvUndAtom, e.Error());
	}
	catch(...)
	{
		HRESULT _hr = HRESULT_FROM_WIN32(::GetLastError());
		if(SUCCEEDED(_hr)) _hr = E_FAIL;
		return Error((PTCHAR)"System Error", IID_IMmQvUndAtom, _hr);
	}

	::InterlockedExchange(&m_nCalculations, 0L);
	return hr;
}



HRESULT CMmQvUndAtom::CalcEquityOptions(LONG nCallGreekMask, LONG nPutGreekMask, MmQvIvCalcEnum enCallCalcIV,
										MmQvIvCalcEnum enPutCalcIV, EtsCalcModelTypeEnum enCalcModel,
										VARIANT_BOOL bUseTheoVolatility, VARIANT_BOOL bUseTheoVolaNoBid, 
										VARIANT_BOOL bUseTheoVolaBadMarket, VARIANT_BOOL bRecalcGreeks, 
										IMmQvOptRootColl* collSynthRoots, DOUBLE dUndPriceTolerance, 
										EtsPriceRoundingRuleEnum enPriceRoundingRule, VARIANT_BOOL bUseCustomRates,
										LONG nCalcSleepFreq, LONG nCalcSleepAmt, DOUBLE dtCalcDate, VARIANT_BOOL ManualEdit,
										VARIANT_BOOL bForceRecalc, ICalculationParametrs* pParams)
{

	HRESULT hr = S_OK;
	try
	{
		std::map <LONG, SQuoteData> SynthQuotes;

		m_dwAbortTime = 0L;


		m_dNetDelta = BAD_DOUBLE_VALUE;
		m_dTotalDelta = BAD_DOUBLE_VALUE;
		m_dTotalGamma = BAD_DOUBLE_VALUE;
		m_dTotalTheta = BAD_DOUBLE_VALUE;
		m_dTotalVega = BAD_DOUBLE_VALUE;
		m_dTotalRho = BAD_DOUBLE_VALUE;
		
		bool bVisible = true;

		IMmQvUndAtomPtr spSynthUnd;
		IMmQvOptRootCollPtr spSynthRoots(collSynthRoots);

		if(m_spSynthUnd != NULL)
		{
			IUnknownPtr spUnk;
			_variant_t varItem;
			ULONG nFetched = 0L;
			_CHK(m_spSynthUnd->get__NewEnum(&spUnk), _T("Fail to get synthetic underlying collection."));
		
			IEnumVARIANTPtr spSynthUndEnum(spUnk);
			_CHK(spSynthUndEnum->Reset(), _T("Fail to reset synthetic underlying collection."));
			while((hr = spSynthUndEnum->Next(1L, &varItem, &nFetched)) == S_OK)
			{
				ATLASSERT(varItem.vt == VT_DISPATCH);
				spSynthUnd = varItem;

				_CHK(spSynthUnd->put_TotalGamma(BAD_DOUBLE_VALUE));
				_CHK(spSynthUnd->put_NetDelta(BAD_DOUBLE_VALUE));
				_CHK(spSynthUnd->put_TotalDelta(BAD_DOUBLE_VALUE));
			}
		}
		
		IMmQvQuoteAtomPtr spUndQuote = m_pQuote->GetQuote(m_nPrimaryExchangeID);
		//IMmQvQuoteAtomPtr spUndQuote = m_pQuote->GetQuote(0L);
		CMmQvQuoteAtom* pUndQuote = dynamic_cast<CMmQvQuoteAtom*>(spUndQuote.GetInterfacePtr());

		SQuoteData undQuoteData;

		VARIANT_BOOL				driverUsed = VARIANT_FALSE;
		double						dFutSpotPrice = 0.0;
		EtsReplacePriceStatusEnum	enMidPriceStatus = enRpsNone;
		_CHK(GetUnderlyingPrice(dUndPriceTolerance, enPriceRoundingRule, &enMidPriceStatus, &driverUsed, &dFutSpotPrice ));
		bool   bIsDirtyUnderlying = false;

		if(driverUsed)
		{
			undQuoteData.m_dBid   = dFutSpotPrice;
			undQuoteData.m_dAsk   = dFutSpotPrice;
			undQuoteData.m_dPrice = dFutSpotPrice;
			pUndQuote->m_enReplacePriceStatus = static_cast<EtsReplacePriceStatusEnum>(enMidPriceStatus);
			if(m_spActiveFuture)
			{
				IMmQvQuoteCollPtr futureQuoteColl;
				_CHK( m_spActiveFuture->get_Quote(&futureQuoteColl) ); 

				IMmQvQuoteAtomPtr futureQuote;
				_CHK( futureQuoteColl->get_Item(0L, &futureQuote) );

				VARIANT_BOOL vbDirty = VARIANT_FALSE;
				futureQuote->get_IsDirty(&vbDirty);
				bIsDirtyUnderlying = (bForceRecalc ? true :(vbDirty!=VARIANT_FALSE?true:false));
				if(!bIsDirtyUnderlying)
					bIsDirtyUnderlying = (bForceRecalc ? true :pUndQuote->IsDirty());
			}
			else if (m_spHeadComponent)
			{
				IMmQvQuoteCollPtr driverQuoteColl;
				_CHK( m_spHeadComponent->get_Quote(&driverQuoteColl) ); 

				IMmQvQuoteAtomPtr driverQuote;
				_CHK( driverQuoteColl->get_Item(0L, &driverQuote) );

				VARIANT_BOOL vbDirty = VARIANT_FALSE;
				driverQuote->get_IsDirty(&vbDirty);
				bIsDirtyUnderlying = (bForceRecalc ? true :(vbDirty!=VARIANT_FALSE?true:false));
				if(!bIsDirtyUnderlying)
					bIsDirtyUnderlying = (bForceRecalc ? true :pUndQuote->IsDirty());	
			}
			else
			{
				if(!bIsDirtyUnderlying)
					bIsDirtyUnderlying = (bForceRecalc ? true :pUndQuote->IsDirty());	
			}

		}
		else
		{
			undQuoteData.m_dBid   = pUndQuote->m_dPriceBid;
			undQuoteData.m_dAsk   = pUndQuote->m_dPriceAsk;
			undQuoteData.m_dPrice = pUndQuote->m_dPriceLast;

			if(undQuoteData.m_dBid > 0. || undQuoteData.m_dAsk > 0. || undQuoteData.m_dPrice > 0.)
			{
				EtsReplacePriceStatusEnum enBidPriceStatus = enRpsNone;
				EtsReplacePriceStatusEnum enAskPriceStatus = enRpsNone;
				enMidPriceStatus = enRpsNone;

				DOUBLE dSpotPrice = m_spUndPriceProfile->GetUndPriceMid(undQuoteData.m_dBid, undQuoteData.m_dAsk, undQuoteData.m_dPrice, dUndPriceTolerance, enPriceRoundingRule, &enMidPriceStatus, ManualEdit);
				m_dActivePrice = dSpotPrice;

				DOUBLE dSpotBidOld = undQuoteData.m_dBid;

				undQuoteData.m_dBid = m_spUndPriceProfile->GetUndPriceBid(undQuoteData.m_dBid, undQuoteData.m_dAsk, undQuoteData.m_dPrice, dUndPriceTolerance, enPriceRoundingRule, &enBidPriceStatus);
				undQuoteData.m_dAsk = m_spUndPriceProfile->GetUndPriceAsk(dSpotBidOld,         undQuoteData.m_dAsk, undQuoteData.m_dPrice, dUndPriceTolerance, enPriceRoundingRule, &enAskPriceStatus);
				undQuoteData.m_dPrice = dSpotPrice;

				_CHK(pUndQuote->put_ReplacePriceStatus((EtsReplacePriceStatusEnum)(enMidPriceStatus | enBidPriceStatus | enAskPriceStatus)));
			}
			bIsDirtyUnderlying = (bForceRecalc ? true :pUndQuote->IsDirty());
		}

		DOUBLE dSpotBid = pUndQuote->m_dPriceBid;
		DOUBLE dSpotAsk = pUndQuote->m_dPriceAsk;
		DOUBLE dSpotLast = pUndQuote->m_dPriceLast;
		
		enMidPriceStatus = enRpsNone;
		DOUBLE dSpotPrice = m_spUndPriceProfile->GetUndPriceMid(dSpotBid, dSpotAsk, dSpotLast, dUndPriceTolerance, enPriceRoundingRule, &enMidPriceStatus, ManualEdit);		
		pUndQuote->m_enReplacePriceStatus = enMidPriceStatus;
		
		spUndQuote = NULL;

		CMmQvExpColl::CollType::iterator itExpCollectionBegin = m_pExp->m_coll.begin();
		CMmQvExpColl::CollType::iterator itExpCollectionEnd = m_pExp->m_coll.end();


		for(CMmQvExpColl::CollType::iterator itExpCollection = itExpCollectionBegin;
			itExpCollection != itExpCollectionEnd; ++itExpCollection)
		{
			CDividendsCache  divsCache;

			CMmQvExpAtom* pExpiry = dynamic_cast<CMmQvExpAtom*>(itExpCollection->second);

			if(pExpiry != NULL)
			{
				_CHK(pExpiry->FindAtmStrike(dSpotPrice));
				if(bVisible && VARIANT_FALSE == pExpiry->m_bVisible)
					bVisible = false;

				DOUBLE dtExpiryOV   = pExpiry->m_dtExpiryOV;
				DOUBLE dYte			= (dtExpiryOV - dtCalcDate) / 365.;
				DOUBLE dtCloseTime	= pExpiry->m_dtTradingClose;

				::GetCalculationParams(dtCalcDate, dtExpiryOV, dtCloseTime, pParams->UseTimePrecision != VARIANT_FALSE, &dtCalcDate, &dtExpiryOV, &dtCloseTime, &dYte);
				bool	bCalcDelta	=	(dYte <= 0.);

				CMmQvStrikeColl::CollType::iterator itStrikeBegin = pExpiry->m_pStrike->m_coll.begin();
				CMmQvStrikeColl::CollType::iterator itStrikeEnd   = pExpiry->m_pStrike->m_coll.end();

				for(CMmQvStrikeColl::CollType::iterator itStrike = itStrikeBegin; itStrike!=itStrikeEnd; ++itStrike)
				{
					IMmQvStrikeAtomPtr spStrike = itStrike->second;
					CMmQvStrikeAtom* pStrike = dynamic_cast<CMmQvStrikeAtom*>(spStrike.GetInterfacePtr());

					if(!pStrike)
						continue;

					if(m_bStopCalc)
						break;

					CMmQvOptPairColl::EnumCollType::iterator itOptPairBegin = pStrike->m_pOptPair->m_coll.begin();
					CMmQvOptPairColl::EnumCollType::iterator itOptPairEnd   = pStrike->m_pOptPair->m_coll.end();

					DOUBLE dVola = 0.0;

					for(CMmQvOptPairColl::EnumCollType::iterator itOptPair = itOptPairBegin; 
						itOptPair != itOptPairEnd; 
						++itOptPair)
					{
						CMmQvOptPairAtom* pOptPair = dynamic_cast<CMmQvOptPairAtom*>(itOptPair->second);
						if(m_bStopCalc)
							break;
						if(m_bStopCalc)
							break;

						if(!pOptPair)
							continue;

						VARIANT_BOOL bCalculateOption = bRecalcGreeks;
						LONG nRootID				  = pOptPair->m_nRootID;


						if(bRecalcGreeks && spSynthRoots != NULL)
						{
							CMmQvOptRootColl* pColl = dynamic_cast<CMmQvOptRootColl*>(spSynthRoots.GetInterfacePtr());
							if(pColl!=NULL)
								bCalculateOption = pColl->GetOptionRoot(nRootID)!=NULL;
						}

						CMmQvOptRootAtom* pOptRoot = pOptRoot = m_pOptRoot->GetOptionRoot(nRootID);
						if(!pOptRoot)
							continue;

						DOUBLE dVola = 0.0;
						LONG lSurfaceID = m_spVolaSrv->GetSurfaceByRoot(pOptRoot->m_nID);
						_CHK(m_spVolaSrv->get_OptionVola(pExpiry->m_dtExpiryOV, pStrike->m_dStrike, lSurfaceID, &dVola));


						VARIANT_BOOL bIsRootSynthetic = pOptRoot->m_bSynthetic;

						SQuoteData undQuote;
						if(bIsRootSynthetic)
						{
							long lSynthRoot = pOptRoot->m_spSynthOptRoot->OptRootID;
							std::map <LONG, SQuoteData>::iterator itrSynth = SynthQuotes.find(lSynthRoot);
							if (itrSynth == SynthQuotes.end())
							{
								SQuoteData synthUndQuoteData;
								_CHK(_GetSyntheticUnderlyingPrice(pOptRoot->m_spSynthOptRoot, synthUndQuoteData.m_dBid, synthUndQuoteData.m_dAsk, synthUndQuoteData.m_dPrice, dUndPriceTolerance, enPriceRoundingRule));

								if(synthUndQuoteData.m_dBid > 0. || synthUndQuoteData.m_dAsk > 0. || synthUndQuoteData.m_dPrice > 0.)
								{
									DOUBLE dSpotPrice = m_spUndPriceProfile->GetUndPriceMid(synthUndQuoteData.m_dBid, synthUndQuoteData.m_dAsk, synthUndQuoteData.m_dPrice, dUndPriceTolerance, enPriceRoundingRule, NULL, ManualEdit);

									if(dSpotPrice > 0 )
										pOptRoot->m_dSU_Price = dSpotPrice;
									else
										pOptRoot->m_dSU_Price = BAD_DOUBLE_VALUE;

									DOUBLE dSpotBidOld = synthUndQuoteData.m_dBid;

									synthUndQuoteData.m_dBid = m_spUndPriceProfile->GetUndPriceBid(synthUndQuoteData.m_dBid, synthUndQuoteData.m_dAsk, synthUndQuoteData.m_dPrice, dUndPriceTolerance, enPriceRoundingRule, NULL);
									synthUndQuoteData.m_dAsk = m_spUndPriceProfile->GetUndPriceAsk(dSpotBidOld,				 synthUndQuoteData.m_dAsk, synthUndQuoteData.m_dPrice, dUndPriceTolerance, enPriceRoundingRule, NULL);
									synthUndQuoteData.m_dPrice = dSpotPrice;
								}	

								SynthQuotes[lSynthRoot] = synthUndQuoteData;

								undQuote.m_dAsk = synthUndQuoteData.m_dAsk;
								undQuote.m_dBid = synthUndQuoteData.m_dBid;
								undQuote.m_dPrice = synthUndQuoteData.m_dPrice;

							}
							else
							{
								undQuote.m_dAsk = itrSynth->second.m_dAsk;
								undQuote.m_dBid = itrSynth->second.m_dBid;
								undQuote.m_dPrice = itrSynth->second.m_dPrice;
							}
						}
						else{
							undQuote.m_dAsk = undQuoteData.m_dAsk;
							undQuote.m_dBid = undQuoteData.m_dBid;
							undQuote.m_dPrice = undQuoteData.m_dPrice;

						}

						CDividendsCache::iterator itrDivs;

						itrDivs = divsCache.find(nRootID);
						if(itrDivs == divsCache.end())
						{
							itrDivs = divsCache.insert(std::make_pair(nRootID, _DividendsCachePtr (new _DividendsCache))).first;
							_DividendsCachePtr& divsNew = itrDivs->second;

							if(!bIsRootSynthetic)
							{
								EtsDivTypeEnum enDivType = enDivCustomStream;
								if(m_spDividend)
									m_spDividend->get_DivType(&enDivType);

								switch(enDivType)
								{
								case enDivMarket:
								case enDivCustomPeriodical:
								case enDivCustomStream:
									{
										if (m_spDividend != NULL)
										{
											divsNew->m_nDivCount = 0;
											m_spDividend->GetDividendCount2(dtCalcDate, dtExpiryOV, dtCloseTime, &divsNew->m_nDivCount);
											if (divsNew->m_nDivCount< 0)
												divsNew->m_nDivCount = 0;

											if (divsNew->m_nDivCount> 0)
											{
												LPSAFEARRAY psaAmounts = NULL;
												LPSAFEARRAY psaDates   = NULL;
												m_spDividend->GetDividends2(dtCalcDate, dtExpiryOV, dtCloseTime, divsNew->m_nDivCount, &psaAmounts, &psaDates, &divsNew->m_nDivCount);

												divsNew->m_saAmounts.Attach(psaAmounts);
												divsNew->m_saDates.Attach(psaDates);
											}
										}
									}
									break;
								case enDivStockBasket:
									{
										VARIANT_BOOL bIsBasket = VARIANT_FALSE;
										if(m_spBasketIndex != NULL)
										{
											divsNew->m_nDivCount = 0;
											IEtsIndexDivCollPtr spDivColl = NULL;
											m_spBasketIndex->get_BasketDivs(&spDivColl);
											_CHK(m_spBasketIndex->get_IsBasket(&bIsBasket));

											if ( bIsBasket && spDivColl != NULL)
											{
												spDivColl->GetDividendCount2(dtCalcDate, dtExpiryOV, dtCloseTime, &divsNew->m_nDivCount);
												if(divsNew->m_nDivCount > 0L)
												{
													LPSAFEARRAY psaAmounts = NULL;
													LPSAFEARRAY psaDates   = NULL;

													spDivColl->GetDividends2(dtCalcDate, dtExpiryOV, dtCloseTime,  divsNew->m_nDivCount, &psaAmounts, &psaDates, &divsNew->m_nDivCount);

													divsNew->m_saAmounts.Attach(psaAmounts);
													divsNew->m_saDates.Attach(psaDates);
												}
											}
										}
									}
									break;
								case enDivIndexYield:
									{
										divsNew->m_dYield = m_dYield;
									}
									break;	
								}



								//if(m_enUndType == enCtStock)
								//{
								//	if (m_spDividend != NULL)
								//	{
								//		divsNew->m_nDivCount = 0;
								//		m_spDividend->GetDividendCount(nToday, nExpiry, &divsNew->m_nDivCount);
								//		if (divsNew->m_nDivCount< 0)
								//			divsNew->m_nDivCount = 0;

								//		if (divsNew->m_nDivCount> 0)
								//		{
								//			LPSAFEARRAY psaAmounts = NULL;
								//			LPSAFEARRAY psaDates   = NULL;
								//			m_spDividend->GetDividends(nToday, nExpiry, divsNew->m_nDivCount, &psaAmounts, &psaDates, &divsNew->m_nDivCount);

								//			divsNew->m_saAmounts.Attach(psaAmounts);
								//			divsNew->m_saDates.Attach(psaDates);
								//		}
								//	}
								//}
								//else
								//{
								//	VARIANT_BOOL bIsBasket = VARIANT_FALSE;
								//	if(m_spBasketIndex != NULL)
								//	{
								//		divsNew->m_nDivCount = 0;
								//		IEtsIndexDivCollPtr spDivColl = NULL;
								//		m_spBasketIndex->get_BasketDivs(&spDivColl);
								//		_CHK(m_spBasketIndex->get_IsBasket(&bIsBasket));

								//		if ( bIsBasket && spDivColl != NULL)
								//		{
								//			spDivColl->GetDividendCount(nToday, nExpiry, &divsNew->m_nDivCount);
								//			if(divsNew->m_nDivCount > 0L)
								//			{
								//				LPSAFEARRAY psaAmounts = NULL;
								//				LPSAFEARRAY psaDates   = NULL;

								//				spDivColl->GetDividends(nToday, nExpiry,  divsNew->m_nDivCount, &psaAmounts, &psaDates, &divsNew->m_nDivCount);

								//				divsNew->m_saAmounts.Attach(psaAmounts);
								//				divsNew->m_saDates.Attach(psaDates);
								//			}
								//		}
								//	}
								//	divsNew->m_dYield = bIsBasket? 0.0 : m_dYield;
								//}
							}
							else
							{
								VARIANT_BOOL bIsBasket = VARIANT_FALSE;
								ISynthRootAtomPtr spSynthOptRoot;
								spSynthOptRoot = pOptRoot->m_spSynthOptRoot;

								_CHK(spSynthOptRoot->get_Basket(&bIsBasket));
								
								if(bIsBasket)
								{
									LONG nBaskDivCount = 0L; VARIANT_BOOL bIsBasket = VARIANT_FALSE;

									divsNew->m_dYield = 0.;

									IEtsIndexDivCollPtr spBasketDivs;
									spSynthOptRoot->get_BasketDivs(&spBasketDivs);
									divsNew->m_nDivCount = 0;
									spBasketDivs->GetDividendCount2(dtCalcDate, dtExpiryOV, dtCloseTime, &divsNew->m_nDivCount);
									if(divsNew->m_nDivCount > 0L) 
									{

										LPSAFEARRAY psaAmounts = NULL;
										LPSAFEARRAY psaDates   = NULL;

										spBasketDivs->GetDividends2(dtCalcDate, dtExpiryOV, dtCloseTime, divsNew->m_nDivCount, &psaAmounts, &psaDates, &divsNew->m_nDivCount);

										divsNew->m_saAmounts.Attach(psaAmounts);
										divsNew->m_saDates.Attach(psaDates);
									} 
								}
							}
						}
						_DividendsCachePtr& divs = itrDivs->second;


						ATLASSERT(pOptRoot != NULL);

						//bIsRootSynthetic = pOptRoot->m_bSynthetic;

						CComObject<CMmQvOptAtom>* pOptForVola = NULL;

						CQuotesCalculationAtomPtr spCalcAtom( new CQuotesCalculationAtom(this, m_strPoolId.c_str()));

						if(!m_bStopCalc && spCalcAtom!=NULL && 
							spCalcAtom->Initialize(
											itOptPair->second,
											pExpiry,
											pOptRoot,
											m_spSynthUnd.GetInterfacePtr(),
											dVola,
											divs,														
											undQuote,
											nPutGreekMask,
											nCallGreekMask,
											enPutCalcIV,
											enCallCalcIV,
											enCalcModel,
											bUseTheoVolatility,
											dUndPriceTolerance, 
											enPriceRoundingRule,
											bUseCustomRates,
											bCalcDelta,
											bIsRootSynthetic==VARIANT_TRUE?true:false,
											bCalculateOption==VARIANT_TRUE?true:false,
											bVisible,
											bIsDirtyUnderlying,
											dtCalcDate,
											ManualEdit, 
											bUseTheoVolaNoBid,
											bUseTheoVolaBadMarket,
											pParams)
							)
						{
							InterlockedIncrement(&m_nCalculations);
							m_Pool.PushBack(CMmQvUndAtomThreadPool::CStrategyThread::PoolDataTypePtr(spCalcAtom));
						}
					}
				}
			}
			_CalcSleep(nCalcSleepFreq, nCalcSleepAmt);
			if (m_bStopCalc)
				CComErrorWrapper::ThrowError(HRESULT_FROM_WIN32(ERROR_CANCELLED), _T("Calculation is stopped"));
		}


		HANDLE hEvents[2] =	{m_eventStopCalc,m_eventComplete};

		long lSavedCalculations = m_nCalculations;
		while( InterlockedExchange(&m_nCalculations, m_nCalculations) > 1)
		{
			DWORD dw = WaitWithEvents(2, hEvents, 1500);
			switch(dw)
			{
			case WAIT_TIMEOUT:
				{
					if(lSavedCalculations == m_nCalculations)
						m_nCalculations = 0;
					else
						lSavedCalculations = m_nCalculations;
					// check progress;

				} break;
			case WAIT_OBJECT_0: // StopCalc
				CComErrorWrapper::ThrowError(HRESULT_FROM_WIN32(ERROR_CANCELLED), _T("Calculation is stopped"));
				break;

			case WAIT_OBJECT_0+1: // done
			default:
				break;
			}
		}
		pUndQuote->SetProcessed();

		if(!IsBadValue(m_nQtyInShares))
			m_dNetDelta = m_nQtyInShares;

		if(!IsBadValue(m_dTotalDelta))
		{
			if(IsBadValue(m_dNetDelta))
				m_dNetDelta = 0.;

			m_dNetDelta += m_dTotalDelta;
		}

		if(!IsBadValue(m_nQtyInShares))
		{
			if(IsBadValue(m_dNetDeltaEq))
				m_dNetDeltaEq = 0.0;

			m_dNetDeltaEq += undQuoteData.m_dPrice * m_nQtyInShares;
		}

	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)CComErrorWrapper::ErrorDescription(e), IID_IMmQvUndAtom, e.Error());
	}
	catch(...)
	{
		HRESULT _hr = HRESULT_FROM_WIN32(::GetLastError());
		if(SUCCEEDED(_hr)) _hr = E_FAIL;
		hr = Error((PTCHAR)"System Error", IID_IMmQvUndAtom, _hr);
	}
	return hr;
}
//  [4/26/2006 agrom]-----------------------------------------------------------------------------------------------
HRESULT CMmQvUndAtom::CalcFutUnderlyingOptions(LONG nCallGreekMask, LONG nPutGreekMask, MmQvIvCalcEnum enCallCalcIV,
										MmQvIvCalcEnum enPutCalcIV, EtsCalcModelTypeEnum enCalcModel,
										VARIANT_BOOL bUseTheoVolatility, VARIANT_BOOL bUseTheoVolaNoBid, 
										VARIANT_BOOL bUseTheoVolaBadMarket, VARIANT_BOOL bRecalcGreeks, 
										DOUBLE dUndPriceTolerance, 
										EtsPriceRoundingRuleEnum enPriceRoundingRule, VARIANT_BOOL bUseCustomRates,
										LONG nCalcSleepFreq, LONG nCalcSleepAmt, DOUBLE dtCalcDate, VARIANT_BOOL ManualEdit,
										VARIANT_BOOL bForceRecalc, ICalculationParametrs* pParams)
{
	HRESULT hr = S_OK;
	m_nQty = BAD_LONG_VALUE;

	try
	{
		if(m_pFut!=NULL && !m_pFut->m_coll.empty())
		{
			double						dFutSpotPrice = 0.0;
			EtsReplacePriceStatusEnum	enMidPriceStatus = enRpsNone;
			VARIANT_BOOL futureUsed = VARIANT_FALSE;

			_CHK(GetUnderlyingPrice(dUndPriceTolerance, enPriceRoundingRule, &enMidPriceStatus, &futureUsed, &dFutSpotPrice ));


			CMmQvFutCollection::CollType::iterator itr    = m_pFut->m_coll.begin();
			CMmQvFutCollection::CollType::iterator itrEnd = m_pFut->m_coll.end();
			for(; itr!=itrEnd; ++itr)
			{
				if(IsStopCalc()) break;

				CMmQvFutAtom* pFutAtom = dynamic_cast<CMmQvFutAtom*>(itr->second);
				if(pFutAtom)
				{
					if(bForceRecalc)
						pFutAtom->SetDirty();

					hr = pFutAtom->CalcAllOptions(this,
												  nCallGreekMask, 
												  nPutGreekMask,
												  enCallCalcIV, 
												  enPutCalcIV, 
												  enCalcModel, 
												  bUseTheoVolatility,
												  bUseTheoVolaNoBid, 
												  bUseTheoVolaBadMarket,
												  bRecalcGreeks, 
												  dUndPriceTolerance,
												  enPriceRoundingRule, 
												  bUseCustomRates,
												  nCalcSleepFreq, 
												  nCalcSleepAmt, 
												  dtCalcDate, 
												  ManualEdit,
												  pParams);
					if(IsStopCalc()) break;
				}
				if(pFutAtom->m_dNetDelta >BAD_DOUBLE_VALUE)
				{
					if(m_dNetDelta == BAD_DOUBLE_VALUE) m_dNetDelta = 0.;
					m_dNetDelta += pFutAtom->m_dNetDelta;
				}
				if(pFutAtom->m_dNetDeltaEq >BAD_DOUBLE_VALUE)
				{
					if(m_dNetDeltaEq == BAD_DOUBLE_VALUE) m_dNetDeltaEq = 0.;
					m_dNetDeltaEq += pFutAtom->m_dNetDeltaEq;
				}
				if(pFutAtom->m_dTotalDelta > BAD_DOUBLE_VALUE)
				{
					if(m_dTotalDelta == BAD_DOUBLE_VALUE) 
						m_dTotalDelta = pFutAtom->m_dTotalDelta;
					else
						m_dTotalDelta += pFutAtom->m_dTotalDelta;
				}
				if(pFutAtom->m_dTotalGamma > BAD_DOUBLE_VALUE)
				{
					if(m_dTotalGamma == BAD_DOUBLE_VALUE)
						m_dTotalGamma = pFutAtom->m_dTotalGamma;
					else
						m_dTotalGamma += pFutAtom->m_dTotalGamma;
				}
				if(pFutAtom->m_dTotalTheta > BAD_DOUBLE_VALUE)
				{
					if(m_dTotalTheta == BAD_DOUBLE_VALUE)
						m_dTotalTheta = pFutAtom->m_dTotalTheta;
					else
						m_dTotalTheta += pFutAtom->m_dTotalTheta;
				}
				if(pFutAtom->m_dTotalVega > BAD_DOUBLE_VALUE)
				{
					if(m_dTotalVega == BAD_DOUBLE_VALUE)
						m_dTotalVega = pFutAtom->m_dTotalVega;
					else
						m_dTotalVega += pFutAtom->m_dTotalVega;
				}
				if(pFutAtom->m_dTotalRho > BAD_DOUBLE_VALUE)
				{
					if(m_dTotalRho == BAD_DOUBLE_VALUE)
						m_dTotalRho = pFutAtom->m_dTotalRho;
					else
						m_dTotalRho += pFutAtom->m_dTotalRho;
				}
				if(pFutAtom->m_nQty > BAD_LONG_VALUE)
				{
					if(m_nQty == BAD_LONG_VALUE)
						m_nQty = pFutAtom->m_nQty;
					else 
						m_nQty += pFutAtom->m_nQty;
				}
			}
		}
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)CComErrorWrapper::ErrorDescription(e), IID_IMmQvUndAtom, e.Error());
	}
	catch(...)
	{
		return Error(_T("Unknown Error."), IID_IMmQvUndAtom, E_FAIL);
	}
	return hr;
}


/////////////////////////////////////////////////////////////////////////////
//
void CMmQvUndAtom::_CalcSleep(LONG nCalcSleepFreq, LONG nCalcSleepAmt)
{
	if(m_bStopCalc)
		return;

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

							if(m_bStopCalc)
							{
								::CancelWaitableTimer(m_hCalcSleepTimer);
								return;
							}

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

/////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CMmQvUndAtom::IsValidDivs(VARIANT_BOOL CheckCustom, VARIANT_BOOL* pVal)
{
	__CHECK_POINTER(pVal);
	HRESULT hr = S_OK;
	ObjectLock lock(this);
	try
	{
		EtsDivTypeEnum enTmpType = enDivMarket;
		m_spDividend->get_DivType(&enTmpType);
		if (!CheckCustom)
			enTmpType = enDivMarket;

		*pVal =  m_spDividend->IsValidDivs(enTmpType);
	}
	catch (_com_error& e) 
	{
		hr = Error((PTCHAR)CComErrorWrapper::ErrorDescription(e), IID_IMmQvUndAtom, e.Error());

	}
	catch(...)
	{
		HRESULT _hr = HRESULT_FROM_WIN32(::GetLastError());
		if(SUCCEEDED(_hr)) _hr = E_FAIL;
		hr = Error((PTCHAR)"System Error", IID_IMmQvUndAtom, _hr);
	}

	return hr;
}

STDMETHODIMP CMmQvUndAtom::CalcFutureOptionGreeks(IMmQvOptAtom* aOpt, IMmQvQuoteAtom* aQuote, IMmQvExpAtom* aExp, 
												  LONG nGreekMask, MmQvIvCalcEnum enCalcIV, 
												  EtsCalcModelTypeEnum enCalcModel, VARIANT_BOOL bUseTheoVolatility, 
												  VARIANT_BOOL bUseTheoVolaNoBid, VARIANT_BOOL bUseTheoVolaBadMarket, 
												  DOUBLE dUndPriceTolerance, EtsPriceRoundingRuleEnum enPriceRoundingRule,
												  VARIANT_BOOL bUseCustomRates, DOUBLE dtCalcDate, VARIANT_BOOL ManualEdit,
												  ICalculationParametrs* pParams)
{
	HRESULT hr = S_OK;
	try
	{
		long nFutId = 0;
		aOpt->get_RootID(&nFutId);
		if(nFutId)
		{
			IMmQvFutAtomPtr spFutAtom = m_pFut->GetFutures(nFutId);
			if(spFutAtom!=NULL)
				spFutAtom->CalcOptionGreeks(this, aOpt, aQuote, aExp, nGreekMask, enCalcIV, enCalcModel,
				bUseTheoVolatility, bUseTheoVolaNoBid, bUseTheoVolaBadMarket,
				dUndPriceTolerance, enPriceRoundingRule, bUseCustomRates, dtCalcDate, ManualEdit, pParams);
		}


	}
	catch (_com_error& e) 
	{
		hr = Error((PTCHAR)CComErrorWrapper::ErrorDescription(e), IID_IMmQvUndAtom, e.Error());
	}
	return hr;
}

/////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CMmQvUndAtom::StopCalc()
{
	m_bStopCalc = TRUE;
	SetEvent(m_eventStopCalc);
	::SwitchToThread();

	if(IsInCalc())
		::Sleep(50);

	return S_OK;
}

/////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CMmQvUndAtom::CleanUp()
{
	HRESULT hr = S_OK;
	try
	{
		StopCalc();

		m_nID = 0L;
		m_bstrSymbol=L"";
		m_enUndType = enCtStock;
		m_dYield = 0.;
		m_bIsHTB = VARIANT_FALSE;
		m_nQty = BAD_LONG_VALUE;
		m_nQtyInShares = BAD_LONG_VALUE;
		m_dNetDelta = BAD_DOUBLE_VALUE;
		m_dTotalDelta = BAD_DOUBLE_VALUE;
		m_dTotalGamma = BAD_DOUBLE_VALUE;
		m_dTotalVega = BAD_DOUBLE_VALUE;
		m_dTotalTheta = BAD_DOUBLE_VALUE;
		m_dTotalRho = BAD_DOUBLE_VALUE;
		m_bIsAmerican = VARIANT_FALSE;
		m_nExpCalendarID = 0L;
		m_bIsTraderContract = VARIANT_FALSE;
		m_dSkew = 0.;
		m_dKurt = 0.;
		m_bUseMidRates = VARIANT_FALSE;
		m_bstrSymbolName=L"";
		m_nPosExchID = 0L;
		m_dUndPosForRates = 0.;
		m_nOptPosExchID = 0L;
		m_nSleepStep = 0L;

		m_spBasketIndex = NULL;
		m_spSynthUnd = NULL;
		m_spUndPriceProfile = NULL;
		m_spOptPriceProfile = NULL;
		m_spActiveFuture	= NULL;

		if(m_spVolaSrv != NULL)		m_spVolaSrv->CleanUp();
		if(m_pExp != NULL)			m_pExp->Clear();
		if(m_pQuote != NULL)		m_pQuote->Clear();
		if(m_pOptRoot != NULL)		m_pOptRoot->Clear();
		if(m_spUndExch != NULL)		m_spUndExch->Clear();
		if(m_spOptExch != NULL)		m_spOptExch->Clear();
		if(m_pOpt != NULL)			m_pOpt->Clear();
		if (m_pFut)					m_pFut->Clear();
		if(m_spHeadComponent)		m_spHeadComponent->CleanUp();
		m_spHeadComponent	= NULL;
		
	}
	catch(const _com_error& e)
	{
		hr = Error((PTCHAR)CComErrorWrapper::ErrorDescription(e), IID_IMmQvUndAtom, e.Error());
	}
	catch(...)
	{
		HRESULT _hr = HRESULT_FROM_WIN32(::GetLastError());
		if(SUCCEEDED(_hr)) _hr = E_FAIL;
		hr = Error((PTCHAR)"System Error", IID_IMmQvUndAtom, _hr);
	}
	return hr;
}
STDMETHODIMP CMmQvUndAtom::get_AtmStrike(DOUBLE dUndPriceTolerance, enum EtsPriceRoundingRuleEnum enPriceRoundingRule, VARIANT_BOOL ManualEdit, DOUBLE* dStrike)
{
	HRESULT hr = S_OK;
	__CHECK_POINTER(dStrike);
	*dStrike = 0;
	try
	{
		IMmQvQuoteAtomPtr spUndQuote;
		_CHK(m_spQuote->get_Item(m_nPrimaryExchangeID, &spUndQuote));
		//_CHK(m_spQuote->get_Item(0L, &spUndQuote));
		//DOUBLE dSpotBid = 0., dSpotAsk = 0., dSpotLast = 0.;
		EtsReplacePriceStatusEnum enMidPriceStatus = enRpsNone;
		VARIANT_BOOL bDriverUsed = VARIANT_FALSE;

		/*_CHK(spUndQuote->get_PriceBid(&dSpotBid));
		_CHK(spUndQuote->get_PriceAsk(&dSpotAsk));
		_CHK(spUndQuote->get_PriceLast(&dSpotLast));

		*dStrike = m_spUndPriceProfile->GetUndPriceMid(
			dSpotBid, 
			dSpotAsk, 
			dSpotLast, 
			dUndPriceTolerance, 
			enPriceRoundingRule, 
			&enMidPriceStatus, ManualEdit);*/

		_CHK(GetUnderlyingPrice(dUndPriceTolerance, enPriceRoundingRule, &enMidPriceStatus, &bDriverUsed, dStrike));

	}
	catch (_com_error& e) 
	{
		return Error((PTCHAR)CComErrorWrapper::ErrorDescription(e), IID_IMmQvUndAtom, e.Error());
	}
	catch(...)
	{
		HRESULT _hr = HRESULT_FROM_WIN32(::GetLastError());
		if(SUCCEEDED(_hr)) _hr = E_FAIL;
		hr = Error((PTCHAR)"System Error", IID_IMmQvUndAtom, _hr);
	}

	return hr;
}

STDMETHODIMP CMmQvUndAtom::SetDirty()
{
	HRESULT hr = S_OK;
	try
	{
		if(m_pQuote)
		{
			IMmQvQuoteAtomPtr spQuote = m_pQuote->GetQuote(m_nPrimaryExchangeID);
			//IMmQvQuoteAtomPtr spQuote = m_pQuote->GetQuote(0L);
			CMmQvQuoteAtom* pQuote = dynamic_cast<CMmQvQuoteAtom*>(spQuote.GetInterfacePtr());
			if(pQuote!=NULL)
				pQuote->SetDirty();
		}
	}
	catch (_com_error& e)
	{
		hr =  Error((PTCHAR)CComErrorWrapper::ErrorDescription(e), IID_IMmQvUndAtom, e.Error());
	}
	catch(...)
	{
		HRESULT _hr = HRESULT_FROM_WIN32(::GetLastError());
		if(SUCCEEDED(_hr)) _hr = E_FAIL;
		hr = Error((PTCHAR)"System Error", IID_IMmQvUndAtom, _hr);
	}
	return hr;
}

STDMETHODIMP CMmQvUndAtom::get_AtmVola(IMmQvExpAtom* pExp, DOUBLE dUndPriceTolerance, EtsPriceRoundingRuleEnum enPriceRoundingRule, VARIANT_BOOL ManualEdit, LONG SurfaceID, DOUBLE* pVola)
{
	__CHECK_POINTER(pVola);
	HRESULT hr = S_OK;
	*pVola = BAD_DOUBLE_VALUE;
	try
	{
		double dSpotPrice = 0.;
		_CHK(get_AtmStrike(dUndPriceTolerance, enPriceRoundingRule, ManualEdit,  &dSpotPrice));
		if(dSpotPrice > 0)
		{
			DATE dtExpiryOV;
			_CHK(pExp->get_ExpiryOV(&dtExpiryOV));
			*pVola =m_spVolaSrv->GetOptionVola(dtExpiryOV, dSpotPrice, SurfaceID);
		}
	}
	catch (_com_error& e) 
	{
		return Error((PTCHAR)CComErrorWrapper::ErrorDescription(e), IID_IMmQvUndAtom, e.Error());
	}
	catch(...)
	{
		HRESULT _hr = HRESULT_FROM_WIN32(::GetLastError());
		if(SUCCEEDED(_hr)) _hr = E_FAIL;
		hr = Error((PTCHAR)"System Error", IID_IMmQvUndAtom, _hr);
	}

	return hr;
}

//---------------------------------------------------------------------------------------------------

void CQuotesCalculationAtom::rawCalculate(CMmQvUndAtom*	 pUndAtom)
{
	try
	{

		CMmQvExpAtom*	  pExpiry   = dynamic_cast<CMmQvExpAtom*>(m_spExpiry.GetInterfacePtr());
		CMmQvOptRootAtom* pOptRoot  = dynamic_cast<CMmQvOptRootAtom*>(m_spOptRoot.GetInterfacePtr());
		CMmQvOptPairAtom* pOptPair  = dynamic_cast<CMmQvOptPairAtom*>(m_spOptPair.GetInterfacePtr());

		for(LONG i = 0L; i < 2L; ++i)
		{
			if(pUndAtom->IsStopCalc())
				break;

			EtsOptionTypeEnum enOptType = static_cast<EtsOptionTypeEnum>(i);
			long			nGreekMask = (enOptType != enOtCall ? m_nPutGreekMask : m_nCallGreekMask);
			MmQvIvCalcEnum	enCalcIV = (enOptType != enOtCall ? m_enPutCalcIV : m_enCallCalcIV);

			CComObject<CMmQvOptAtom>* pOpt =  pOptPair->GetOpt(enOptType);
			if(pOpt != NULL)
			{
				LONG nOptID = pOpt->m_nID;

				if(nOptID != 0L)
				{
					if(m_bCalculateOption)
						pOpt->m_dVola = m_dVola;

					CComObject<CMmQvQuoteColl>* pQuotes = pOpt->m_pQuote;
					CVolasMapPtr volasCache(new CVolasMap);

					CMmQvQuoteColl::EnumCollType::iterator itrQuote      = pQuotes->m_coll.begin();
					CMmQvQuoteColl::EnumCollType::iterator itrQuoteEnd   = pQuotes->m_coll.end();
					DWORD dwTimer = GetTickCount();

					for(;!pUndAtom->IsStopCalc() && itrQuote != itrQuoteEnd; ++itrQuote)
					{
						CMmQvQuoteAtom*  pQuote = NULL;
						pQuote = dynamic_cast<CMmQvQuoteAtom*>(itrQuote->second);

						LONG nExchID = pQuote->GetExchangeID();
						bool bFinalVisible = true;
						if(m_bVisible)
						{
							VARIANT_BOOL vb;
							if(nExchID)
							{
								pQuote->m_spExch->get_Visible(&vb);
								if(vb == VARIANT_FALSE)
									bFinalVisible = false;
							}
						}

						if(m_bCalculateOption && bFinalVisible && (m_bIsDirtyUnderlying || pQuote->IsDirty()) && !pUndAtom->IsStopCalc())
						{
							double dCalcYield    = (m_divs->m_nDivCount!=0)?0.0:m_divs->m_dYield;
							double dCalcSkew     = pUndAtom->m_dSkew;
							double dCalcCurtosis = pUndAtom->m_dKurt;

							if(m_bIsRootSynthetic)
							{
								_CHK(pOptRoot->m_spSynthOptRoot->get_Skew(&dCalcSkew));
								_CHK(pOptRoot->m_spSynthOptRoot->get_Kurt(&dCalcCurtosis));

								if(m_divs->m_nDivCount!=0)
									_CHK(pOptRoot->m_spSynthOptRoot->get_Yield(&dCalcYield));
							}

							_CHK(pUndAtom->rawCalcOptionGreeks(
								pOpt, 
								pQuote, 
								m_undQuote,
								pExpiry, 
								pOptRoot,
								nGreekMask, 
								enCalcIV, 
								m_enCalcModel,
								m_bUseTheoVolatility,														 														
								m_bUseTheoVolaNoBid, 
								m_bUseTheoVolaBadMarket,
								m_dUndPriceTolerance, 
								m_enPriceRoundingRule,
								m_bUseCustomRates,
								m_divs->m_nDivCount,
								m_divs->m_saDates,
								m_divs->m_saAmounts,
								dCalcYield,
								dCalcSkew,
								dCalcCurtosis,
								m_bCalcDelta,
								m_dtCalcDate,
								volasCache,
								m_bManualEdit,
								m_spCalculationParametrs), _T("Fail to calculate option Greeks."));
						}
						if(nExchID == 0L && !pUndAtom->IsStopCalc())
						{
							LONG nQtyInShares = pOpt->m_nQtyInShares;
							if(!IsBadValue(nQtyInShares))
							{
								CAutoLock lc(pUndAtom->m_csCalcTotals);

								if(!pUndAtom->IsStopCalc())
								{
									DOUBLE dValue = 0.;

									if(!m_bIsRootSynthetic)
									{
										dValue = pQuote->m_dGamma;
										if(!IsBadValue(dValue))	
										{
											if(IsBadValue(pUndAtom->m_dTotalGamma))
												pUndAtom->m_dTotalGamma = 0.;

											pUndAtom->m_dTotalGamma += nQtyInShares * dValue / 100.;
										}

										dValue = pQuote->m_dDelta;
										if(!IsBadValue(dValue))
										{
											if(IsBadValue(pUndAtom->m_dTotalDelta))
												pUndAtom->m_dTotalDelta = 0.;

											pUndAtom->m_dTotalDelta += nQtyInShares * dValue / 100.;

											if(m_undQuote.m_dPrice>0)
											{
												if(IsBadValue(pUndAtom->m_dNetDeltaEq)) pUndAtom->m_dNetDeltaEq = 0.0;
												pUndAtom->m_dNetDeltaEq += ( nQtyInShares * pQuote->m_dDelta / 100.* m_undQuote.m_dPrice );
											}
										}

									}
									else // Synthetic Root
									{
										HRESULT hr = S_OK;

										ISynthRootAtomPtr spSynthOptRoot = pOptRoot->m_spSynthOptRoot;

										if(spSynthOptRoot != NULL)
										{
											IUnknownPtr spUnk;
											_variant_t  varItem;
											ULONG       nFetched = 0;

											DOUBLE dValue2 = 0.;
											ISynthRootCompCollPtr spSRCompColl;
											ISynthRootCompAtomPtr spSRComp;
											_CHK(spSynthOptRoot->get_SynthRootComponents(&spSRCompColl));

											_CHK(spSRCompColl->get__NewEnum(&spUnk), _T("Fail to get synthetic underlying component collection."));
											IEnumVARIANTPtr spSynthUndCompEnum(spUnk);
											_CHK(spSynthUndCompEnum->Reset(), _T("Fail to reset synthetic underlying component collection."));

											dValue  = pQuote->m_dGamma;
											dValue2 = pQuote->m_dDelta;
											if(!IsBadValue(dValue) || !IsBadValue(dValue2))	
											{
												DOUBLE dCash;
												DOUBLE dTotalDelta, dTotalGamma, dTotalNetDeltaEq;
												IMmQvUndAtomPtr spSynthUnd;

												_CHK(spSynthOptRoot->get_CashValue(&dCash));
												if(!IsBadValue(dCash) && dCash > 0.)
												{
													_CHK(m_spSynthUnd->get_Item(0L/*USD_ID*/, &spSynthUnd));

													if(spSynthUnd != NULL)
													{
														if(!IsBadValue(dValue))
														{
															spSynthUnd->get_TotalGamma(&dTotalGamma);

															if(IsBadValue(dTotalGamma))
																dTotalGamma = 0.;

															dTotalGamma += nQtyInShares * dValue * dCash / 100.;

															_CHK(spSynthUnd->put_TotalGamma(dTotalGamma));
														}

														if(!IsBadValue(dValue2))
														{
															_CHK(spSynthUnd->get_TotalDelta(&dTotalDelta));

															if(IsBadValue(dTotalDelta))
																dTotalDelta = 0.;

															dTotalDelta += nQtyInShares * dValue2 * dCash / 100.;

															_CHK(spSynthUnd->put_TotalDelta(dTotalDelta));
															_CHK(spSynthUnd->put_NetDelta(dTotalDelta));
															_CHK(spSynthUnd->put_NetDeltaEq(dTotalDelta));
														}
													}

												}

												while((hr = spSynthUndCompEnum->Next(1L, &varItem, &nFetched)) == S_OK)
												{
													LONG nUndID;
													DOUBLE dWeight;
													DOUBLE dPriceMid;

													EtsReplacePriceStatusEnum enMidPriceStatus = enRpsNone;
													VARIANT_BOOL bFutureUsed = VARIANT_FALSE;

													ATLASSERT(varItem.vt == VT_DISPATCH);
													spSRComp = varItem;
													_CHK(spSRComp->get_UndID(&nUndID));
													_CHK(spSRComp->get_Weight(&dWeight));

													dTotalGamma = 0.;
													dTotalDelta = 0.;

													if(nUndID != pUndAtom->m_nID)
													{
														IMmQvUndAtomPtr spSynthUnd;

														_CHK(m_spSynthUnd->get_Item(nUndID,&spSynthUnd));

														if(spSynthUnd != NULL)
														{
															if(!IsBadValue(dValue))
															{
																_CHK(spSynthUnd->get_TotalGamma(&dTotalGamma));

																if(IsBadValue(dTotalGamma))
																	dTotalGamma = 0.;

																dTotalGamma += nQtyInShares * dValue * dWeight / 100.;

																_CHK(spSynthUnd->put_TotalGamma(dTotalGamma));

															}
															if(!IsBadValue(dValue2))
															{
																_CHK(spSynthUnd->get_TotalDelta(&dTotalDelta));

																if(IsBadValue(dTotalDelta))
																	dTotalDelta = 0.;

																dTotalDelta += nQtyInShares * dValue2 * dWeight / 100.;

																_CHK(spSynthUnd->put_TotalDelta(dTotalDelta));
																_CHK(spSynthUnd->put_NetDelta(dTotalDelta));
																

																_CHK(spSynthUnd->get_NetDeltaEq(&dTotalNetDeltaEq));
																
																if(IsBadValue(dTotalNetDeltaEq))
																	dTotalNetDeltaEq = 0.;

																spSynthUnd->GetUnderlyingPrice( m_dUndPriceTolerance,
																								m_enPriceRoundingRule,
																								&enMidPriceStatus,
																								&bFutureUsed,
																								&dPriceMid);

																if(dPriceMid > 0.)
																	dTotalNetDeltaEq += nQtyInShares * dValue2 * dWeight / 100. * dPriceMid;

																_CHK(spSynthUnd->put_NetDeltaEq(dTotalNetDeltaEq));
															}
														}
													}
													else
													{
														if(!IsBadValue(dValue))
														{
															if(IsBadValue(pUndAtom->m_dTotalGamma))
																pUndAtom->m_dTotalGamma = 0.;

															pUndAtom->m_dTotalGamma += nQtyInShares * dValue * dWeight / 100.;
														}

														if(!IsBadValue(dValue2))
														{
															if(IsBadValue(pUndAtom->m_dTotalDelta))
																pUndAtom->m_dTotalDelta = 0.;

															pUndAtom->m_dTotalDelta += nQtyInShares * dValue2 * dWeight / 100.;
															
															if(IsBadValue(pUndAtom->m_dNetDeltaEq))
																pUndAtom->m_dNetDeltaEq = 0.;
															
															pUndAtom->GetUnderlyingPrice( m_dUndPriceTolerance,
																							m_enPriceRoundingRule,
																							&enMidPriceStatus,
																							&bFutureUsed,
																							&dPriceMid);

															if (dPriceMid > 0.)
																pUndAtom->m_dNetDeltaEq += nQtyInShares * dValue2 * dWeight / 100. * dPriceMid;

														}
													}
												}

											}
										}
									}

									dValue = pQuote->m_dTheta;
									if(!IsBadValue(dValue))
									{
										if(IsBadValue(pUndAtom->m_dTotalTheta))
											pUndAtom->m_dTotalTheta = 0.;

										pUndAtom->m_dTotalTheta += nQtyInShares * dValue / 100.;
									}

									dValue = pQuote->m_dVega;
									if(!IsBadValue(dValue))
									{
										if(IsBadValue(pUndAtom->m_dTotalVega))
											pUndAtom->m_dTotalVega = 0.;

										pUndAtom->m_dTotalVega += nQtyInShares * dValue / 100.;
									}

									dValue = pQuote->m_dRho;
									if(!IsBadValue(dValue))
									{
										if(IsBadValue(pUndAtom->m_dTotalRho))
											pUndAtom->m_dTotalRho = 0.;

										pUndAtom->m_dTotalRho += nQtyInShares * dValue / 100.;
									}
								}
							}		
						}
					}
				}
			}
		}
	}
	catch (_com_error& /*e*/){	}
 	catch (...) {	}
}

void CQuotesCalculationAtom::DoTask()  
{
	if(m_pUndAtom)
	{
		try
		{
			if(!m_pUndAtom->IsStopCalc())
				rawCalculate(m_pUndAtom);
		}
		catch (...){}

		if(m_pUndAtom && !m_pUndAtom->IsStopCalc())
		{
			if( !InterlockedDecrement(&m_pUndAtom->m_nCalculations))
				SetEvent(m_pUndAtom->m_eventComplete);
		}
	}
}



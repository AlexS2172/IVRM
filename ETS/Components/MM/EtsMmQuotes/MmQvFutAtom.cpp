// MmQvFutAtom.cpp : Implementation of CMmQvFutAtom
#include "stdafx.h"
#include "MmQvFutAtom.h"
#include "MmQvUndAtom.h" 
#include "MmQvOptAtom.h" 
#include "MmQvQuoteAtom.h" 
#include "MmQvExpAtom.h" 

/////////////////////////////////////////////////////////////////////////////
//
HRESULT CMmQvFutAtom::rawCalcOptionGreeks(
							CMmQvUndAtom*	pUnd,
							SQuoteData*     pFutPrice,
							_DividendsCachePtr  spDividend,
							CMmQvOptAtom*	pOpt, 
							CMmQvQuoteAtom* pQuote,
							CMmQvExpAtom*	pExp, 
							LONG nGreekMask, 
							MmQvIvCalcEnum enCalcIV,
							EtsCalcModelTypeEnum enCalcModel, 
							VARIANT_BOOL bUseTheoVolatility,
							VARIANT_BOOL bUseTheoVolaNoBid, 
							VARIANT_BOOL bUseTheoVolaBadMarket,
							DOUBLE dUndPriceTolerance, 
							EtsPriceRoundingRuleEnum enPriceRoundingRule,
							double  dRate, 
							DATE  dtCalcDate, 
							VARIANT_BOOL ManualEdit,
							ICalculationParametrs* pParams)
{
	if(!pUnd || !pOpt || !pQuote || !pExp || spDividend==NULL || pParams == NULL)
		return Error(L"Invalid objects passed.", IID_IMmQvFutAtom, E_INVALIDARG);

	try
	{
//		IExchAtomPtr spExch;

		pQuote->m_enReplacePriceStatus = enRpsNone;

		LONG nExchID = pQuote->GetExchangeID();
		LONG nQtyInShares = pOpt->m_nQtyInShares;

		if(!IsBadValue(nQtyInShares))
			nGreekMask |= GT_THEOPRICE | GT_DELTA | GT_GAMMA | GT_VEGA | GT_THETA | GT_RHO;

		if(nGreekMask != GT_NOTHING || enCalcIV != enMmQvCalcIvNone)
		{
			GREEKS aGreeks;
			memset(&aGreeks, 0, sizeof(aGreeks));
			aGreeks.nMask = nGreekMask;

			LONG nModel = static_cast<LONG>(enCalcModel);

			LONG nIsAmerican = (m_bIsAmerican ? 1L : 0L);

			if(pFutPrice->IsValid())
			{
				DOUBLE dOptPriceBid = 0., dOptPriceAsk = 0., dOptPriceLast = 0., dOptPriceMid = 0., dOptPriceBidForIv = 0.;				
				EtsReplacePriceStatusEnum enMidPriceStatus = enRpsNone;
				EtsReplacePriceStatusEnum enBidPriceStatus = enRpsNone;
				EtsReplacePriceStatusEnum enAskPriceStatus = enRpsNone;

				if(enCalcIV != enMmQvCalcIvNone || !bUseTheoVolatility)
				{
					dOptPriceBid  = pQuote->m_dPriceBid;
					dOptPriceAsk  = pQuote->m_dPriceAsk;
					dOptPriceLast = pQuote->m_dPriceLast;

					dOptPriceBidForIv = dOptPriceBid;

					enMidPriceStatus = enRpsNone;
					if(dOptPriceBid > 0. || dOptPriceAsk > 0. || dOptPriceLast > 0.)
						dOptPriceMid = m_spOptPriceProfile->GetOptPriceMid(dOptPriceBid, dOptPriceAsk, dOptPriceLast,
						enPriceRoundingRule, bUseTheoVolatility, 0, &enMidPriceStatus);

					pQuote->m_enReplacePriceStatus = enMidPriceStatus;
				}

				if(pFutPrice->m_dPrice > 0. && (aGreeks.nMask != GT_NOTHING || enCalcIV != enMmQvCalcIvNone))
				{
					LONG nRetCount = 0L;
					DOUBLE dSkew = 0., dKurt = 0.;
					EtsContractTypeEnum	m_enUndType = enCtStock;

					m_enUndType = pUnd->m_enUndType;
					dSkew		= pUnd->m_dSkew;
					dKurt		= pUnd->m_dKurt;

					DATE dtExpiryOV = pOpt->m_dtExpiryOV;
					DATE dtCloseTime = pOpt->m_dtTradingClose;	
					DOUBLE dYTE = (dtExpiryOV - dtCalcDate) / 365.;
					
					::GetCalculationParams(dtCalcDate, dtExpiryOV, dtCloseTime, pParams->UseTimePrecision != VARIANT_FALSE, &dtCalcDate, &dtExpiryOV, &dtCloseTime, &dYTE);


					DOUBLE dIVBid = 0., dIVAsk = 0.;

					DOUBLE dVolaForCalcs = 0.;
					EtsOptionTypeEnum	enOptType	= pOpt->m_enOptType;
					DOUBLE				dStrike		= pOpt->m_dStrike;

					// Sharky: new version of IV calcs
					if((enCalcIV & enMmQvCalcIv) || !bUseTheoVolatility)
					{
						DOUBLE dIV = 0.;

						if(!bUseTheoVolatility)
						{
							if(DoubleGTZero(dOptPriceMid))
							{
								if(!bUseTheoVolaNoBid || bUseTheoVolaNoBid && DoubleGTZero(dOptPriceBidForIv))
								{
									LONG nFlag = VF_OK;
									dIV = ::CalcFutureOptionVolatility(dRate, pFutPrice->m_dPrice, dOptPriceMid, dStrike, 
											dYTE, enOptType, nIsAmerican,
											100L, dSkew, dKurt, nModel, &nFlag);

									if((!bUseTheoVolaBadMarket || bUseTheoVolaBadMarket && nFlag == VF_OK)
										&& DoubleGTZero(dIV))
										pQuote->m_nCalcGreeksIVType = enMmQvCalcGreeksIVMarket;
									else if(bUseTheoVolaBadMarket)
									{
										dIV = pOpt->m_dVola;
										pQuote->m_nCalcGreeksIVType =enMmQvCalcGreeksIVTheo;
									}
									else
										pQuote->m_nCalcGreeksIVType =enMmQvCalcGreeksIVNone;
								}
								else
								{
									dIV = pOpt->m_dVola;
									pQuote->m_nCalcGreeksIVType =enMmQvCalcGreeksIVTheo;
								}
							}
							else if(bUseTheoVolaBadMarket)
							{
								dIV = pOpt->m_dVola;
								pQuote->m_nCalcGreeksIVType =enMmQvCalcGreeksIVTheo;
							}
							else
								pQuote->m_nCalcGreeksIVType =enMmQvCalcGreeksIVNone;

							dVolaForCalcs = (DoubleGTZero(dIV) ? dIV : BAD_DOUBLE_VALUE);
							pQuote->m_dIV = dVolaForCalcs;
						}
						else
						{
							if(DoubleGTZero(dOptPriceMid))
							{
								LONG nFlag = VF_OK;
								dIV = ::CalcFutureOptionVolatility(dRate, pFutPrice->m_dPrice, dOptPriceMid, dStrike, 
											dYTE, enOptType, nIsAmerican,
											100L, dSkew, dKurt, nModel, &nFlag);
							}

							pQuote->m_dIV = (DoubleGTZero(dIV) ? dIV : BAD_DOUBLE_VALUE);
							dVolaForCalcs = pOpt->m_dVola;
							pQuote->m_nCalcGreeksIVType =enMmQvCalcGreeksIVTheo;
						}
					}
					else
					{
						dVolaForCalcs = pOpt->m_dVola;
						pQuote->m_nCalcGreeksIVType =enMmQvCalcGreeksIVTheo;
					}

					if(aGreeks.nMask != GT_NOTHING && (bUseTheoVolatility && nExchID == 0L || !bUseTheoVolatility))
					{
						if(m_enUndType == enCtFutUnd)
							nRetCount = ::CalcFutureOptionGreeks2(	dRate,
																	pFutPrice->m_dPrice, 
																	false, 
																	dStrike, 
																	dVolaForCalcs, 
																	dYTE,
																	enOptType, 
																	nIsAmerican, 
																	100L, 
																	dSkew, 
																	dKurt, 
																	nModel, 
																	&aGreeks);
						else
						{
							nRetCount = ::CalcFutureOptionGreeks3(dRate,
									spDividend->m_dYield, 
									pFutPrice->m_dPrice, 
									true, 
									dStrike, 
									dVolaForCalcs,
									dYTE,
									enOptType, 
									nIsAmerican,
									100L, 
									dSkew, 
									dKurt, 
									nModel, 
									spDividend->m_nDivCount, 
									spDividend->m_saAmounts.GetPlainData(),
									spDividend->m_saDates.GetPlainData(),												  																  
									&aGreeks);
						}

					}

					// Calculate IV in case of Use Theo Surface and no bid & ask & last
					if(enCalcIV & enMmQvCalcIv && bUseTheoVolatility && DoubleLEQZero(dOptPriceMid))
					{
						EtsOptBadBothPricesRuleEnum BadBothPriceRule = enObbprUseLast;
						_CHK(m_spOptPriceProfile->get_BadOptBothPriceRule(&BadBothPriceRule));

						if(BadBothPriceRule == enObbprUseTheo)
						{
							DOUBLE dIV = 0.;
							if(nExchID == 0L)
							{
								if(_finite(aGreeks.dTheoPrice) && DoubleGTZero(aGreeks.dTheoPrice))
								{
									LONG nFlag = 0L;
									dIV = ::CalcFutureOptionVolatility(dRate, pFutPrice->m_dPrice, aGreeks.dTheoPrice,
													dStrike, dYTE, enOptType, nIsAmerican, 100L,
													dSkew, dKurt, nModel, &nFlag);
								}
							}
							else
							{
								const CMmQvQuoteAtom* pDefQuote = pOpt->GetDefaultQuote();
								ATLASSERT(pDefQuote != NULL);
								dIV = pDefQuote->m_dIV;
							}

							pQuote->m_dIV = (DoubleGTZero(dIV) ? dIV : BAD_DOUBLE_VALUE);
						}
					}

					DOUBLE dOptBidOld = dOptPriceBid;

					if(enCalcIV & enMmQvCalcIvBid)
					{
						if(DoubleGTZero(pFutPrice->m_dBid))
						{	
							EtsReplacePriceStatusEnum enOldPriceStatus = enRpsNone;
							enBidPriceStatus = enRpsNone;

							dOptPriceBid = m_spOptPriceProfile->GetOptPriceBid(dOptPriceBid, dOptPriceAsk, dOptPriceLast, enPriceRoundingRule, 
								bUseTheoVolatility, (_finite(aGreeks.dTheoPrice) && aGreeks.dTheoPrice > 0.) ? aGreeks.dTheoPrice : 0., &enBidPriceStatus);

							if(enBidPriceStatus != enRpsNone)
							{
								enOldPriceStatus = pQuote->m_enReplacePriceStatus;
								pQuote->m_enReplacePriceStatus = (EtsReplacePriceStatusEnum)(enOldPriceStatus | enBidPriceStatus);
							}

							if(DoubleGTZero(dOptPriceBid))
							{
								LONG nFlag = VF_OK;
								dIVBid = ::CalcFutureOptionVolatility(dRate, pFutPrice->m_dBid, dOptPriceBid, dStrike, 
											dYTE, enOptType, nIsAmerican,
											100L, dSkew, dKurt, nModel, &nFlag);

								if(!bUseTheoVolatility && bUseTheoVolaBadMarket && nFlag != VF_OK)
									dIVBid = pOpt->m_dVola;
							}
						}

						pQuote->m_dIVBid = (DoubleGTZero(dIVBid) ? dIVBid : BAD_DOUBLE_VALUE);
					}

					if(enCalcIV & enMmQvCalcIvAsk)
					{
						if(DoubleGTZero(pFutPrice->m_dAsk))
						{
							EtsReplacePriceStatusEnum enOldPriceStatus = enRpsNone;
							enAskPriceStatus = enRpsNone;

							dOptPriceAsk = m_spOptPriceProfile->GetOptPriceAsk(dOptBidOld, dOptPriceAsk, dOptPriceLast,
								enPriceRoundingRule, bUseTheoVolatility,
								(_finite(aGreeks.dTheoPrice) && DoubleGTZero(aGreeks.dTheoPrice))
								? aGreeks.dTheoPrice : 0., &enAskPriceStatus);

							if(enAskPriceStatus != enRpsNone)
							{
								enOldPriceStatus = pQuote->m_enReplacePriceStatus;
								pQuote->m_enReplacePriceStatus = (EtsReplacePriceStatusEnum)(enOldPriceStatus | enAskPriceStatus);
							}

							if(DoubleGTZero(dOptPriceAsk))
							{
								LONG nFlag = VF_OK;
								dIVAsk = ::CalcFutureOptionVolatility(dRate, pFutPrice->m_dAsk, dOptPriceAsk, dStrike, 
											dYTE, enOptType, nIsAmerican,
											100L, dSkew, dKurt, nModel, &nFlag);

								if(!bUseTheoVolatility && bUseTheoVolaBadMarket && nFlag != VF_OK)
									dIVAsk = pOpt->m_dVola;
							}
						}

						pQuote->m_dIVAsk = DoubleGTZero(dIVAsk) ? dIVAsk : BAD_DOUBLE_VALUE;

						if(DoubleGTZero(dIVBid) && DoubleGTZero(dIVAsk))
							pQuote->m_dIVSpread = dIVBid - dIVAsk;
						else
							pQuote->m_dIVSpread = BAD_DOUBLE_VALUE;
					}

					if(aGreeks.nMask != GT_NOTHING &&
						(bUseTheoVolatility && nExchID == 0L || !bUseTheoVolatility))
					{
						_CHK(pQuote->ClearValues(VARIANT_FALSE, VARIANT_FALSE));

						if ((aGreeks.dTheoPrice > BAD_DOUBLE_VALUE)&&(aGreeks.dTheoPrice < 0))
							aGreeks.dTheoPrice = 0;

						if((aGreeks.nMask & GT_THEOPRICE) && _finite(aGreeks.dTheoPrice) && aGreeks.dTheoPrice >= 0.)
							pQuote->m_dPriceTheo = aGreeks.dTheoPrice;

						if((aGreeks.nMask & GT_DELTA) && _finite(aGreeks.dDelta))
						{
							if(	dYTE <= 0. && DoubleEQZero(aGreeks.dDelta)&&
								((enOptType == enOtPut && pFutPrice->m_dPrice < dStrike) || (enOptType == enOtCall && pFutPrice->m_dPrice > dStrike))
							)
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

						if((aGreeks.nMask & GT_GAMMA) && (aGreeks.nMask & GT_THETA) &&
							_finite(aGreeks.dGamma) && _finite(aGreeks.dTheta) 
							&& fabs(aGreeks.dTheta) >= .0001
						   )
							pQuote->m_dGammaPerTheta = (aGreeks.dGamma / aGreeks.dTheta);
						//}
					}
					else if(aGreeks.nMask != GT_NOTHING && bUseTheoVolatility && nExchID != 0)
					{
						const CMmQvQuoteAtom* pDefQuote = pOpt->GetDefaultQuote();
						ATLASSERT(pDefQuote != NULL);

						DOUBLE dValue = 0.;
					
						pQuote->m_dPriceTheo = pDefQuote->m_dPriceTheo;
						pQuote->m_dDelta	 = pDefQuote->m_dDelta;
						pQuote->m_dGamma	 = pDefQuote->m_dGamma;
						pQuote->m_dTheta	 = pDefQuote->m_dTheta;
						pQuote->m_dVega		 = pDefQuote->m_dVega;
						pQuote->m_dRho		 = pDefQuote->m_dRho;
						pQuote->m_dVegaDelta = pDefQuote->m_dVegaDelta;
						pQuote->m_dVegaGamma = pDefQuote->m_dVegaGamma;
						pQuote->m_dThetaDelta = pDefQuote->m_dThetaDelta;
						pQuote->m_dThetaGamma = pDefQuote->m_dThetaGamma;
						pQuote->m_dGammaPerTheta = pDefQuote->m_dGammaPerTheta;
						pQuote->m_dIVBid = pDefQuote->m_dIVBid;
						pQuote->m_dIVAsk = pDefQuote->m_dIVAsk;
						pQuote->m_dIVSpread = pDefQuote->m_dIVSpread;
					}
				}
			}
		}
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)CComErrorWrapper::ErrorDescription(e), IID_IMmQvFutAtom, e.Error());
	}
	return S_OK;
}


STDMETHODIMP CMmQvFutAtom::CalcOptionGreeks(IMmQvUndAtom* aUnd, IMmQvOptAtom* aOpt, IMmQvQuoteAtom* aQuote,
											IMmQvExpAtom* aExp, LONG nGreekMask, MmQvIvCalcEnum enCalcIV,
											EtsCalcModelTypeEnum enCalcModel, VARIANT_BOOL bUseTheoVolatility,
											VARIANT_BOOL bUseTheoVolaNoBid, VARIANT_BOOL bUseTheoVolaBadMarket,
											DOUBLE dUndPriceTolerance, EtsPriceRoundingRuleEnum enPriceRoundingRule,
											VARIANT_BOOL bUseCustomRates, DOUBLE dtCalcDate, VARIANT_BOOL ManualEdit,
											ICalculationParametrs* pParams)
{
	if(aUnd == NULL || aOpt == NULL || aQuote == NULL || aExp == NULL)
		return Error(L"Invalid objects passed.", IID_IMmQvFutAtom, E_INVALIDARG);

	CMmQvUndAtom*	pUnd  =dynamic_cast<CMmQvUndAtom*>(aUnd); 
	CMmQvOptAtom*	pOpt  =dynamic_cast<CMmQvOptAtom*>(aOpt);  
	CMmQvQuoteAtom* pQuote=dynamic_cast<CMmQvQuoteAtom*>(aQuote); 
	CMmQvExpAtom*	pExp  =dynamic_cast<CMmQvExpAtom*>(aExp);  

	_DividendsCachePtr  spDividend(new _DividendsCache);

	IMmQvQuoteAtomPtr	spFutQuote;
	SQuoteData			futQuote;
	double				dRate = 0.0;
	bool                bDirty = false;

	DOUBLE dtExpiryOV	= pOpt->m_dtExpiryOV;
	DOUBLE dtCloseTime	= pOpt->m_dtTradingClose;
	DOUBLE dYTE			= 0.;
	::GetCalculationParams(dtCalcDate, dtExpiryOV, dtCloseTime, pParams->UseTimePrecision != VARIANT_FALSE, &dtCalcDate, &dtExpiryOV, &dtCloseTime, &dYTE);

	if (pUnd)
	{

		DOUBLE dFutPriceMid = 0.;
		VARIANT_BOOL	futureUsed = VARIANT_FALSE;
		EtsReplacePriceStatusEnum enMidPriceStatus = enRpsNone;
		_CHK(GetFuturePrice(aUnd, dUndPriceTolerance, enPriceRoundingRule, &enMidPriceStatus, &futureUsed, &dFutPriceMid));

		DOUBLE dFutPriceBidOld = 0.;
		if ( futureUsed ) 
		{
			futQuote.m_dPrice = dFutPriceMid;
			futQuote.m_dBid   = dFutPriceMid;
			futQuote.m_dAsk   = dFutPriceMid;
		}
		else{
			IMmQvQuoteAtomPtr spFutQuote;
			_CHK(m_spQuote->get_Item(0L, &spFutQuote));

			DOUBLE dFutPriceBid = 0., dFutPriceAsk = 0., dFutPriceLast = 0.;			
			_CHK(spFutQuote->get_PriceBid(&dFutPriceBid));
			_CHK(spFutQuote->get_PriceAsk(&dFutPriceAsk));
			_CHK(spFutQuote->get_PriceLast(&dFutPriceLast));			
			if(dFutPriceBid > 0. || dFutPriceAsk > 0. || dFutPriceLast > 0.)
			{
				enMidPriceStatus = enRpsNone;
				EtsReplacePriceStatusEnum enBidPriceStatus = enRpsNone;
				EtsReplacePriceStatusEnum enAskPriceStatus = enRpsNone;

				futQuote.m_dPrice = m_spUndPriceProfile->GetUndPriceMid(dFutPriceBid, dFutPriceAsk, dFutPriceLast, dUndPriceTolerance, enPriceRoundingRule, &enMidPriceStatus, ManualEdit);
				futQuote.m_dBid   = m_spUndPriceProfile->GetUndPriceBid(dFutPriceBid, dFutPriceAsk, dFutPriceLast, dUndPriceTolerance, enPriceRoundingRule, &enBidPriceStatus);
				futQuote.m_dAsk   = m_spUndPriceProfile->GetUndPriceAsk(dFutPriceBid, dFutPriceAsk, dFutPriceLast, dUndPriceTolerance, enPriceRoundingRule, &enAskPriceStatus);

				_CHK(spFutQuote->put_ReplacePriceStatus((EtsReplacePriceStatusEnum)(enMidPriceStatus | enBidPriceStatus | enAskPriceStatus)));
			}
		}

		CSafeArrayWrapper<double> saDates;
		CSafeArrayWrapper<double> saAmounts;

		VARIANT_BOOL bIsBasket = VARIANT_FALSE;

		IIndexAtomPtr spBasketIndex;
		spBasketIndex = pUnd->m_spBasketIndex;

		EtsDivTypeEnum enDivType = enDivCustomStream;
		if(pUnd->m_spDividend)
			pUnd->m_spDividend->get_DivType(&enDivType);

		switch(enDivType)
		{
		case enDivMarket:
		case enDivCustomPeriodical:
		case enDivCustomStream:
			{
				if (pUnd->m_spDividend != NULL)
				{
					spDividend->m_nDivCount = 0;
					pUnd->m_spDividend->GetDividendCount2(dtCalcDate, dtExpiryOV, dtCloseTime, &spDividend->m_nDivCount);
					if (spDividend->m_nDivCount< 0)
						spDividend->m_nDivCount = 0;

					if (spDividend->m_nDivCount> 0)
					{
						LPSAFEARRAY psaAmounts = NULL;
						LPSAFEARRAY psaDates   = NULL;
						pUnd->m_spDividend->GetDividends2(dtCalcDate, dtExpiryOV, dtCloseTime, spDividend->m_nDivCount, &psaAmounts, &psaDates, &spDividend->m_nDivCount);

						spDividend->m_saAmounts.Attach(psaAmounts);
						spDividend->m_saDates.Attach(psaDates);
					}
				}
			}
			break;
		case enDivStockBasket:
			{
				VARIANT_BOOL bIsBasket = VARIANT_FALSE;
				if(pUnd->m_spBasketIndex != NULL)
				{
					spDividend->m_nDivCount = 0;
					IEtsIndexDivCollPtr spDivColl = NULL;
					pUnd->m_spBasketIndex->get_BasketDivs(&spDivColl);
					_CHK(pUnd->m_spBasketIndex->get_IsBasket(&bIsBasket));

					if ( bIsBasket && spDivColl != NULL)
					{

						spDivColl->GetDividendCount2(dtCalcDate, dtExpiryOV, dtCloseTime, &spDividend->m_nDivCount);
						if(spDividend->m_nDivCount > 0L)
						{
							LPSAFEARRAY psaAmounts = NULL;
							LPSAFEARRAY psaDates   = NULL;

							spDivColl->GetDividends2(dtCalcDate, dtExpiryOV, dtCloseTime, spDividend->m_nDivCount, &psaAmounts, &psaDates, &spDividend->m_nDivCount);

							spDividend->m_saAmounts.Attach(psaAmounts);
							spDividend->m_saDates.Attach(psaDates);
						}
					}
				}
			}
			break;
		case enDivIndexYield:
			{
				spDividend->m_dYield = pUnd->m_dYield;
			}
			break;	
		}
	}

	if(bUseCustomRates)
		dRate = (pExp!=NULL)?pExp->m_dRateCust:0.0;
	else
		dRate = (pExp!=NULL)?pExp->m_dRate:0.0;


	return rawCalcOptionGreeks(
		pUnd, 
		&futQuote,
		spDividend,
		pOpt,
		pQuote, 
		pExp, 
		nGreekMask, 
		enCalcIV, 
		enCalcModel, 
		bUseTheoVolatility, 
		bUseTheoVolaNoBid, 
		bUseTheoVolaBadMarket, 
		dUndPriceTolerance, 
		enPriceRoundingRule, 
		dRate, 
		dtCalcDate, 
		ManualEdit,
		pParams);
}

/////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CMmQvFutAtom::CalcAllOptions(IMmQvUndAtom* aUnd, LONG nCallGreekMask, LONG nPutGreekMask,
											MmQvIvCalcEnum enCallCalcIV, MmQvIvCalcEnum enPutCalcIV, 
											EtsCalcModelTypeEnum enCalcModel, VARIANT_BOOL bUseTheoVolatility,
											VARIANT_BOOL bUseTheoVolaNoBid, VARIANT_BOOL bUseTheoVolaBadMarket,
											VARIANT_BOOL bRecalcGreeks, DOUBLE dUndPriceTolerance,
											EtsPriceRoundingRuleEnum enPriceRoundingRule, VARIANT_BOOL bUseCustomRates,
											LONG nCalcSleepFreq, LONG nCalcSleepAmt, DOUBLE dtCalcDate, VARIANT_BOOL ManualEdit,
											ICalculationParametrs* pParams)
{
	if(aUnd == NULL || pParams == NULL)
		return Error(L"Invalid objects passed.", IID_IMmQvFutAtom, E_INVALIDARG);

	try
	{
		m_bStopCalc = FALSE;

		m_dNetDelta = BAD_DOUBLE_VALUE;
		m_dNetDeltaEq = BAD_DOUBLE_VALUE;
		m_dTotalDelta = BAD_DOUBLE_VALUE;
		m_dTotalGamma = BAD_DOUBLE_VALUE;
		m_dTotalTheta = BAD_DOUBLE_VALUE;
		m_dTotalVega = BAD_DOUBLE_VALUE;
		m_dTotalRho = BAD_DOUBLE_VALUE;

		IUnknownPtr spUnk;
		_variant_t varItem;
		ULONG nFetched = 0L;
		CMmQvUndAtom* pUnd = dynamic_cast<CMmQvUndAtom*>(aUnd);

	
	        m_spVolaSrv = pUnd->m_spVolaSrv;

		DOUBLE dFutPriceBid = 0., dFutPriceAsk = 0., dFutPriceLast = 0., dFutPriceMid = 0., dUndPriceMid = 0.;
		SQuoteData futQuote;

		if (pUnd)
		{
			VARIANT_BOOL	futureUsed = VARIANT_FALSE;
			EtsReplacePriceStatusEnum enMidPriceStatus = enRpsNone;

			_CHK(GetFuturePrice(aUnd, dUndPriceTolerance, enPriceRoundingRule, &enMidPriceStatus, &futureUsed, &dFutPriceMid));

			DOUBLE dFutPriceBidOld = 0.;
			if ( futureUsed ) 
			{
				futQuote.m_dPrice = dFutPriceMid;
				futQuote.m_dBid   = dFutPriceMid;
				futQuote.m_dAsk   = dFutPriceMid;
			}
			else
			{
				IMmQvQuoteAtomPtr spFutQuote;
				_CHK(m_spQuote->get_Item(0L, &spFutQuote));

				DOUBLE dFutPriceBid = 0., dFutPriceAsk = 0., dFutPriceLast = 0.;			
				_CHK(spFutQuote->get_PriceBid(&dFutPriceBid));
				_CHK(spFutQuote->get_PriceAsk(&dFutPriceAsk));
				_CHK(spFutQuote->get_PriceLast(&dFutPriceLast));			
				if(dFutPriceBid > 0. || dFutPriceAsk > 0. || dFutPriceLast > 0.)
				{
					enMidPriceStatus = enRpsNone;
					EtsReplacePriceStatusEnum enBidPriceStatus = enRpsNone;
					EtsReplacePriceStatusEnum enAskPriceStatus = enRpsNone;

					futQuote.m_dPrice = m_spUndPriceProfile->GetUndPriceMid(dFutPriceBid, dFutPriceAsk, dFutPriceLast, dUndPriceTolerance, enPriceRoundingRule, &enMidPriceStatus, ManualEdit);
					futQuote.m_dBid   = m_spUndPriceProfile->GetUndPriceBid(dFutPriceBid, dFutPriceAsk, dFutPriceLast, dUndPriceTolerance, enPriceRoundingRule, &enBidPriceStatus);
					futQuote.m_dAsk   = m_spUndPriceProfile->GetUndPriceAsk(dFutPriceBid, dFutPriceAsk, dFutPriceLast, dUndPriceTolerance, enPriceRoundingRule, &enAskPriceStatus);
					if(enCtFutUnd != pUnd->m_enUndType)
						dFutPriceMid = futQuote.m_dPrice;
					else
						_CHK(spFutQuote->put_ReplacePriceStatus((EtsReplacePriceStatusEnum)(enMidPriceStatus | enBidPriceStatus | enAskPriceStatus)));
				}
			}
			_CHK(pUnd->GetUnderlyingPrice(dUndPriceTolerance, enPriceRoundingRule, &enMidPriceStatus, &futureUsed, &dUndPriceMid));
		}



		_DividendsCachePtr  spDividend;
		long lExpDateSave = 0;
		//spFutQuote = NULL;
		double dDeltaGammaCalcKoeff = 1.;
		double dDeltaGammaCalcKoeff_Eq = 1.;
		if(m_pFutureRoot!=NULL)
			dDeltaGammaCalcKoeff_Eq = m_pFutureRoot->GetDeltaGammaKoeff();
		if(m_bShowInFutureContractEq==VARIANT_FALSE)
			dDeltaGammaCalcKoeff = dDeltaGammaCalcKoeff_Eq;

		if(m_pExp)
		{
			CMmQvExpColl::CollType::iterator itrExpEnd  = m_pExp->m_coll.end();
			for(CMmQvExpColl::CollType::iterator itrExp = m_pExp->m_coll.begin(); itrExp !=itrExpEnd; ++itrExp)
			{
				CMmQvExpAtom* pExpiry = dynamic_cast<CMmQvExpAtom*>(itrExp->second);
				if(pExpiry)
				{
					double dRate = 0;

					if(bUseCustomRates)
						dRate = pExpiry->m_dRateCust;
					else
						dRate = pExpiry->m_dRate;


					_CHK(pExpiry->FindAtmStrike(dFutPriceMid));

					CComObject<CMmQvStrikeColl>* pStrikes = pExpiry->m_pStrike;
					CMmQvStrikeColl::CollType::iterator itrStrikes    = pStrikes->m_coll.begin();
					CMmQvStrikeColl::CollType::iterator itrStrikesEnd = pStrikes->m_coll.end();

					for( ;itrStrikes!=itrStrikesEnd; ++itrStrikes)
					{
						CMmQvStrikeAtom* pStrikeAtom = dynamic_cast<CMmQvStrikeAtom*>(itrStrikes->second);
						if(pStrikeAtom)
						{
							CMmQvOptPairColl* pOptPairs = pStrikeAtom->m_pOptPair;

							CMmQvOptPairColl::EnumCollType::iterator itrOptPair    = pOptPairs->m_coll.begin();
							CMmQvOptPairColl::EnumCollType::iterator itrOptPairEnd = pOptPairs->m_coll.end();
							for(; itrOptPair!=itrOptPairEnd; ++itrOptPair)
							{
								CMmQvOptPairAtom* pOptPair = dynamic_cast<CMmQvOptPairAtom*>(itrOptPair->second);
								if(pOptPair)
								{
									DOUBLE dVola            = 0.;
									bool   bVolaInitialized = false;
									LONG nGreekMask; MmQvIvCalcEnum enCalcIV;

									for(LONG i = 0L; i < 2L; i++)
									{
										EtsOptionTypeEnum enOptType = static_cast<EtsOptionTypeEnum>(i);
										nGreekMask = (enOptType != enOtCall ? nPutGreekMask : nCallGreekMask);
										enCalcIV = (enOptType != enOtCall ? enPutCalcIV : enCallCalcIV);

										CMmQvOptAtom* pOpt = pOptPair->GetOpt(enOptType);
										if(pOpt != NULL)
										{
											LONG nOptID = pOpt->m_nID;

											if(nOptID != 0L)
											{
												if(bRecalcGreeks)
												{
													DATE   dtExpiryOV  = pOpt->m_dtExpiryOV;
													DOUBLE dtCloseTime = pOpt->m_dtTradingClose;
													DOUBLE dtYTE	   = 0.;

													::GetCalculationParams(	dtCalcDate, 
																			dtExpiryOV, 
																			dtCloseTime, 
																			pParams->UseTimePrecision != VARIANT_FALSE, 
																			&dtCalcDate, 
																			&dtExpiryOV, 
																			&dtCloseTime, 
																			&dtYTE);

													double dStrike     = pOpt->m_dStrike;
													if(lExpDateSave!=static_cast<long>(dtExpiryOV))
													{
														lExpDateSave = static_cast<long>(dtExpiryOV);

														spDividend = _DividendsCachePtr(new _DividendsCache);
														CSafeArrayWrapper<double> saDates;
														CSafeArrayWrapper<double> saAmounts;

														VARIANT_BOOL bIsBasket = VARIANT_FALSE;

														IIndexAtomPtr spBasketIndex;
														spBasketIndex = pUnd->m_spBasketIndex;

														EtsDivTypeEnum enDivType = enDivCustomStream;
														if(pUnd->m_spDividend)
															pUnd->m_spDividend->get_DivType(&enDivType);

														switch(enDivType)
														{
														case enDivMarket:
														case enDivCustomPeriodical:
														case enDivCustomStream:
															{
																if (pUnd->m_spDividend != NULL)
																{
																	spDividend->m_nDivCount = 0;
																	pUnd->m_spDividend->GetDividendCount2(dtCalcDate, dtExpiryOV, dtCloseTime, &spDividend->m_nDivCount);
																	if (spDividend->m_nDivCount< 0)
																		spDividend->m_nDivCount = 0;

																	if (spDividend->m_nDivCount> 0)
																	{
																		LPSAFEARRAY psaAmounts = NULL;
																		LPSAFEARRAY psaDates   = NULL;
																		pUnd->m_spDividend->GetDividends2(dtCalcDate, dtExpiryOV, dtCloseTime, spDividend->m_nDivCount, &psaAmounts, &psaDates, &spDividend->m_nDivCount);

																		spDividend->m_saAmounts.Attach(psaAmounts);
																		spDividend->m_saDates.Attach(psaDates);
																	}
																}
															}
															break;
														case enDivStockBasket:
															{
																VARIANT_BOOL bIsBasket = VARIANT_FALSE;
																if(pUnd->m_spBasketIndex != NULL)
																{
																	spDividend->m_nDivCount = 0;
																	IEtsIndexDivCollPtr spDivColl = NULL;
																	pUnd->m_spBasketIndex->get_BasketDivs(&spDivColl);
																	_CHK(pUnd->m_spBasketIndex->get_IsBasket(&bIsBasket));

																	if ( bIsBasket && spDivColl != NULL)
																	{

																		spDivColl->GetDividendCount2(dtCalcDate, dtExpiryOV, dtCloseTime, &spDividend->m_nDivCount);
																		if(spDividend->m_nDivCount > 0L)
																		{
																			LPSAFEARRAY psaAmounts = NULL;
																			LPSAFEARRAY psaDates   = NULL;

																			spDivColl->GetDividends2(dtCalcDate, dtExpiryOV, dtCloseTime,  spDividend->m_nDivCount, &psaAmounts, &psaDates, &spDividend->m_nDivCount);

																			spDividend->m_saAmounts.Attach(psaAmounts);
																			spDividend->m_saDates.Attach(psaDates);
																		}
																	}
																}
															}
															break;
														case enDivIndexYield:
															{
																spDividend->m_dYield = pUnd->m_dYield;
															}
															break;	
														}
													}

													if(!bVolaInitialized)
													{
														IMmQvOptRootAtomPtr spOptRoot = pUnd->GetOptRoot()->GetItem(pOpt->m_nRootID);
														if (spOptRoot)
														{
															LONG lRootID, lSurfaceID;
															_CHK(spOptRoot->get_ID(&lRootID));
															lSurfaceID = m_spVolaSrv->GetSurfaceByRoot(lRootID);
															_CHK(m_spVolaSrv->get_OptionVola(dtExpiryOV, dStrike, lSurfaceID, &dVola));
														bVolaInitialized = true;
													}
													}
													pOpt->m_dVola = dVola;
												}

												CMmQvQuoteColl* pQuotes = pOpt->m_pQuote;

												CMmQvQuoteColl::EnumCollType::iterator itrQuotes    = pQuotes->m_coll.begin();
												CMmQvQuoteColl::EnumCollType::iterator itrQuotesEnd = pQuotes->m_coll.end();
												for(;itrQuotes!=itrQuotesEnd; ++itrQuotes)
												{
													CMmQvQuoteAtom* pQuote = dynamic_cast<CMmQvQuoteAtom*>(itrQuotes->second);
													if(pQuote)
													{
														if(bRecalcGreeks)
														{
															_CHK(rawCalcOptionGreeks(pUnd, 
																					&futQuote, 
																					spDividend,
																					pOpt, 
																					pQuote, 
																					pExpiry, 
																					nGreekMask, 
																					enCalcIV, 
																					enCalcModel, 
																					bUseTheoVolatility, 
																					bUseTheoVolaNoBid, 
																					bUseTheoVolaBadMarket,
																					dUndPriceTolerance, 
																					enPriceRoundingRule,
																					dRate, 
																					dtCalcDate, 
																					ManualEdit,
																					pParams), _T("Fail to calculate option greeks."));

														}

														LONG        nExchID = 0L;
														IExchAtomPtr spExch = pQuote->m_spExch;
														_CHK(spExch->get_ID(&nExchID));

														if(nExchID == 0L)
														{
															LONG nQtyInShares = pOpt->m_nQty;

															if(!IsBadValue(nQtyInShares))
															{
																EtsContractTypeEnum	enUndType = pUnd->m_enUndType;

																if(!IsBadValue(pQuote->m_dGamma))	
																{
																	if(IsBadValue(m_dTotalGamma))
																		m_dTotalGamma = 0.;

																	if(enUndType == enCtFutUnd)
																		m_dTotalGamma += nQtyInShares * pQuote->m_dGamma / 100.;
																	else
																		m_dTotalGamma += nQtyInShares * m_nLotSize * pQuote->m_dGamma / 100;//MUST BE MULTIPLIED BY CONTRACT SIZE
																}

																if(!IsBadValue(pQuote->m_dDelta))
																{
																	if(IsBadValue(m_dTotalDelta))
																		m_dTotalDelta = 0.;

																	if(enUndType == enCtFutUnd)
																		m_dTotalDelta += nQtyInShares * pQuote->m_dDelta / 100.;
																	else
																		m_dTotalDelta += nQtyInShares * m_nLotSize * pQuote->m_dDelta / 100;//MUST BE MULTIPLIED BY CONTRACT SIZE

																}

																if(!IsBadValue(pQuote->m_dTheta))
																{
																	if(IsBadValue(m_dTotalTheta))
																		m_dTotalTheta = 0.;

																	if(enUndType == enCtFutUnd)
																		m_dTotalTheta += nQtyInShares * pQuote->m_dTheta / 100. * dDeltaGammaCalcKoeff_Eq;
																	else
																		m_dTotalTheta += nQtyInShares * m_nLotSize * pQuote->m_dTheta / 100;//MUST BE MULTIPLIED BY CONTRACT SIZE
																}

																if(!IsBadValue(pQuote->m_dVega))
																{
																	if(IsBadValue(m_dTotalVega))
																		m_dTotalVega = 0.;
																	
																	if(enUndType == enCtFutUnd)
																		m_dTotalVega += nQtyInShares * pQuote->m_dVega / 100. * dDeltaGammaCalcKoeff_Eq;
																	else
																		m_dTotalVega += nQtyInShares * m_nLotSize * pQuote->m_dVega / 100;//MUST BE MULTIPLIED BY CONTRACT SIZE
																}

																if(!IsBadValue(pQuote->m_dRho))
																{
																	if(IsBadValue(m_dTotalRho))
																		m_dTotalRho = 0.;

																	if(enUndType == enCtFutUnd)
																		m_dTotalRho += nQtyInShares * pQuote->m_dRho / 100. * dDeltaGammaCalcKoeff_Eq;
																	else
																		m_dTotalRho += nQtyInShares * m_nLotSize * pQuote->m_dRho / 100;//MUST BE MULTIPLIED BY CONTRACT SIZE
																}
															}		
														}
													}													
												}
											}
										}
									}
									_CalcSleep(nCalcSleepFreq, nCalcSleepAmt);
									if(m_bStopCalc)
										CComErrorWrapper::ThrowError(HRESULT_FROM_WIN32(ERROR_CANCELLED), _T("Calculation is stopped"));
								}
							}
						}
					}
				}
			}
		}

		if(!IsBadValue(m_nQty))
		{
			EtsContractTypeEnum	enUndType;
			enUndType = pUnd->m_enUndType;
			if(enUndType == enCtFutUnd)
				m_dNetDelta = m_nQty;//Will be multiplied later by Contract Size
			else
				m_dNetDelta = m_nQty * m_nLotSize;
		}

		if(!IsBadValue(m_dTotalDelta))
		{
			if(IsBadValue(m_dNetDelta))
				m_dNetDelta = 0.;

			m_dNetDelta += m_dTotalDelta;
		}

		EtsContractTypeEnum	enUndType = pUnd->m_enUndType;

		if((dFutPriceMid > 0 && enUndType == enCtFutUnd) || (dUndPriceMid > 0 && enUndType== enCtIndex)) 
		{
			LONG nMultiplier = 1;
			if(m_bShowInFutureContractEq==VARIANT_FALSE)
			{
				nMultiplier = m_nLotSize;
			}

			if(m_dNetDelta > BAD_DOUBLE_VALUE)
			{
				if(enUndType == enCtFutUnd)
				{
					m_dNetDeltaEq = m_dNetDelta * dFutPriceMid * dDeltaGammaCalcKoeff_Eq; 
					//m_dNetDelta *= dDeltaGammaCalcKoeff;
					m_dNetDelta *= nMultiplier;
				}
				else
				{
					m_dNetDelta *= dDeltaGammaCalcKoeff;
					m_dNetDeltaEq = m_dNetDelta * dUndPriceMid;
				}
			}
			if(m_dTotalDelta > BAD_DOUBLE_VALUE)
			{
				if(enUndType == enCtFutUnd)
				{
					m_dTotalDeltaEq = m_dTotalDelta * dFutPriceMid * dDeltaGammaCalcKoeff_Eq;
					//m_dTotalDelta *= dDeltaGammaCalcKoeff;
					m_dTotalDelta *= nMultiplier;
				}
				else
				{
					m_dTotalDelta *= dDeltaGammaCalcKoeff;
					m_dTotalDeltaEq = m_dTotalDelta * dUndPriceMid;
				}
			}
			if(m_dTotalGamma > BAD_DOUBLE_VALUE)
			{
				if(enUndType == enCtFutUnd)
				{
					m_dTotalGammaEq = m_dTotalGamma * dDeltaGammaCalcKoeff_Eq * dFutPriceMid * dFutPriceMid / 100;
					//m_dTotalGamma *= dDeltaGammaCalcKoeff;
					m_dTotalGamma *= nMultiplier;
				}
				else
				{
					m_dTotalGamma *= dDeltaGammaCalcKoeff;
					//m_dTotalGammaEq = m_dTotalGamma * dFutPriceMid;
					m_dTotalGammaEq = m_dTotalGamma * dUndPriceMid * dUndPriceMid / 100;
				}
			}
		}
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)CComErrorWrapper::ErrorDescription(e), IID_IMmQvFutAtom, e.Error());
	}

	return S_OK;
}

/////////////////////////////////////////////////////////////////////////////
//
void CMmQvFutAtom::_CalcSleep(LONG nCalcSleepFreq, LONG nCalcSleepAmt)
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
STDMETHODIMP CMmQvFutAtom::StopCalc()
{
	m_bStopCalc = TRUE;

	return S_OK;
}

/////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CMmQvFutAtom::CleanUp()
{
	try
	{
		m_nID = 0L;
		m_bstrSymbol       = _bstr_t();
		m_bstrContractName = _bstr_t();
		m_bstrQuotationUnit = _bstr_t();
		m_nFutRootID = 0L;
		m_nExpCalendarID = 0L;
		m_dtMaturityDate = 0.;
		m_dtMaturityMonth = 0.;
		m_bIsAmerican = VARIANT_FALSE;
		m_nQty = BAD_LONG_VALUE;
		m_nQtyInShares = BAD_LONG_VALUE;
		m_nLotSize = BAD_LONG_VALUE;
		m_dNetDelta = BAD_DOUBLE_VALUE;
		m_dTotalDelta = BAD_DOUBLE_VALUE;
		m_dTotalGamma = BAD_DOUBLE_VALUE;
		m_dTotalVega = BAD_DOUBLE_VALUE;
		m_dTotalTheta = BAD_DOUBLE_VALUE;
		m_dTotalRho = BAD_DOUBLE_VALUE;
		m_nPosExchID = 0L;
		m_nOptPosExchID = 0L;

		m_spUndPriceProfile = NULL;
		m_spOptPriceProfile = NULL;

        if(m_spVolaSrv != NULL)
			m_spVolaSrv->CleanUp();

		if(m_spExp != NULL)
			m_spExp->Clear();

		if(m_spQuote != NULL)
			m_spQuote->Clear();

		if(m_spFutExch != NULL)
			m_spFutExch->Clear();

		if(m_spOptExch != NULL)
			m_spOptExch->Clear();

		if(m_spOpt != NULL)
			m_spOpt->Clear();
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)CComErrorWrapper::ErrorDescription(e), IID_IMmQvFutAtom, e.Error());
	}
	return S_OK;
}

STDMETHODIMP CMmQvFutAtom::SetDirty()
{
	HRESULT hr = S_OK;
	try
	{
		if(m_pQuote)
		{
			IMmQvQuoteAtomPtr spQuote = m_pQuote->GetQuote(0L);
			CMmQvQuoteAtom* pQuote = dynamic_cast<CMmQvQuoteAtom*>(spQuote.GetInterfacePtr());
			if(pQuote!=NULL)
				pQuote->SetDirty();
		}
	}
	catch (_com_error& e)
	{
		hr =  Error((PTCHAR)CComErrorWrapper::ErrorDescription(e), IID_IMmQvFutAtom, e.Error());
	}
/*
	catch(...)
	{
		hr =  Error(_T("Unknown Error"), IID_IMmQvFutAtom, E_FAIL);
	}
*/
	return hr;
}
STDMETHODIMP CMmQvFutAtom::get_AtmStrike(DOUBLE dUndPriceTolerance, enum EtsPriceRoundingRuleEnum enPriceRoundingRule, DOUBLE* dStrike)
{
	HRESULT hr = S_OK;
	__CHECK_POINTER(dStrike);
	*dStrike = 0;
	try
	{
		IMmQvQuoteAtomPtr spFutQuote;
		_CHK(m_spQuote->get_Item(0L, &spFutQuote));
		DOUBLE dSpotBid = 0., dSpotAsk = 0., dSpotLast = 0.;
		EtsReplacePriceStatusEnum enMidPriceStatus = enRpsNone;

		_CHK(spFutQuote->get_PriceBid(&dSpotBid));
		_CHK(spFutQuote->get_PriceAsk(&dSpotAsk));
		_CHK(spFutQuote->get_PriceLast(&dSpotLast));

		*dStrike = m_spUndPriceProfile->GetUndPriceMid(
			dSpotBid, 
			dSpotAsk, 
			dSpotLast, 
			dUndPriceTolerance, 
			enPriceRoundingRule, 
			&enMidPriceStatus, VARIANT_FALSE);

	}
	catch (_com_error& e) 
	{
		return Error((PTCHAR)CComErrorWrapper::ErrorDescription(e), IID_IMmQvFutAtom, e.Error());
	}
	return hr;
}

STDMETHODIMP CMmQvFutAtom::get_AtmVola(IMmQvExpAtom* pExp, DOUBLE dUndPriceTolerance, EtsPriceRoundingRuleEnum enPriceRoundingRule, LONG SurfaceID, DOUBLE* pVola)
{
	__CHECK_POINTER(pVola);
	HRESULT hr = S_OK;
	*pVola = BAD_DOUBLE_VALUE;
	try
	{
		double dSpotPrice = 0.;
		_CHK(get_AtmStrike(dUndPriceTolerance, enPriceRoundingRule, &dSpotPrice));
		if(dSpotPrice > 0)
		{
			DATE dtExpiryOV;
			_CHK(pExp->get_ExpiryOV(&dtExpiryOV));
			*pVola =m_spVolaSrv->GetOptionVola(dtExpiryOV, dSpotPrice, SurfaceID);
		}
	}
	catch (_com_error& e) 
	{
		return Error((PTCHAR)CComErrorWrapper::ErrorDescription(e), IID_IMmQvFutAtom, e.Error());
	}
	return hr;
}

STDMETHODIMP CMmQvFutAtom::GetFuturePrice(	IMmQvUndAtom* aUnd,DOUBLE dTolerance, 
											 EtsPriceRoundingRuleEnum enPriceRound,
											 EtsReplacePriceStatusEnum * penPriceStatus,
											 VARIANT_BOOL *bFutureUsed, 
											 DOUBLE	*pPrice)
{
	 HRESULT hr = S_OK;
	 CComBSTR					activeFutureSymbol;
	 bool						dontUseFuture = true;
	 IEtsPriceProfileAtomPtr	activeFuturePriceProfile;
	 double						activeFuturePrice = 0.;
	 IMmQvFutAtomPtr			activeFuture;
	 IMmQvQuoteCollPtr			futureQuoteColl;
	 IMmQvQuoteAtomPtr			futureQuote;

	 double						activeFuturePriceBid = 0., activeFuturePriceAsk = 0.,
								activeFuturePriceLast = 0.,activeFutureClosePrice = 0.,
								activeFutureCurrentPrice = 0., futureClosePrice = 0.,
								futurePriceBid = 0., futurePriceAsk = 0.,
								futurePriceLast = 0.;

	 IMmQvQuoteCollPtr			activeFutureQuoteColl;
	 IMmQvQuoteAtomPtr			activeFutureQuote;

	 if (!bFutureUsed || !pPrice || !aUnd)		return E_POINTER;

	 *bFutureUsed = VARIANT_FALSE;
	 *pPrice = 0.;
	 VARIANT_BOOL	bUseManualPrice = VARIANT_FALSE;

	 try{

		 //use market price or price () of active future
		 _CHK(aUnd->get_ActiveFuture(&activeFuture ));
		 if (activeFuture) 
		 {
			 _CHK(activeFuture->get_Symbol(&activeFutureSymbol));
			 if (activeFutureSymbol.Length() )
			 {
				 _CHK(activeFuture->get_UndPriceProfile(&activeFuturePriceProfile));
				 if (activeFuturePriceProfile)	
				 {
					 // we have active future - try to get it price
					 _CHK( activeFuture->get_Quote(&activeFutureQuoteColl) ); 
					 _CHK( activeFutureQuoteColl->get_Item(0L, &activeFutureQuote) );
					 if( activeFutureQuote )
					 {

						 _CHK(activeFutureQuote->get_PriceBid(&activeFuturePriceBid));
						 _CHK(activeFutureQuote->get_PriceAsk(&activeFuturePriceAsk));
						 _CHK(activeFutureQuote->get_PriceLast(&activeFuturePriceLast));

						 activeFuturePrice = activeFuturePriceProfile->GetUndPriceMid(	activeFuturePriceBid,
																						activeFuturePriceAsk,
																						activeFuturePriceLast,
																						dTolerance,
																						enPriceRound,
																						penPriceStatus, VARIANT_FALSE);

						 activeFuture->get_IsUseManualActivePrice(&bUseManualPrice);

						 if ( bUseManualPrice )
							activeFuture->get_ActivePrice(&activeFuturePrice);

						 if ( activeFuturePrice > 0)
						 {
							 // we have price of active future
							 if ( m_dRatio <= 0.)
							 {
								 double dRatio = 1.;
								 // we have no ratio, try to compute it on the fly
								 _CHK( m_spQuote->get_Item(0L, &futureQuote) );
								 _CHK( futureQuote->get_PriceClose(&futureClosePrice));
								 _CHK( activeFutureQuote->get_PriceClose(&activeFutureClosePrice));
								 if ( futureClosePrice > 0. && activeFutureClosePrice > 0)	
									 dRatio = futureClosePrice / activeFutureClosePrice;
								 else
									 dRatio = 1.;	// couldn't compute ratio
						
								 *pPrice = activeFuturePrice * dRatio;
							 }
							 else
								 *pPrice = activeFuturePrice * m_dRatio;
						 }
						 dontUseFuture = false;
						 *bFutureUsed = VARIANT_TRUE;
					 }
				 }
			 }
		 }
		 else if ( m_bUseManualActivePrice )
		 {
			 *pPrice		=	m_dActivePrice;
			 dontUseFuture	=	false;
			 *bFutureUsed	=	VARIANT_TRUE;
		 }
		 // use quote for this future  if no manual and active price
		 if (dontUseFuture){
			 ATLASSERT ( m_spQuote ) ;
			 _CHK( m_spQuote->get_Item(0L, &futureQuote) );
			 if (futureQuote) {
				 _CHK( futureQuote->get_PriceBid(&futurePriceBid));
				 _CHK( futureQuote->get_PriceAsk(&futurePriceAsk));
				 _CHK( futureQuote->get_PriceLast(&futurePriceLast));
				 *pPrice = m_spUndPriceProfile->GetUndPriceMid(	futurePriceBid,
					 futurePriceAsk,
					 futurePriceLast,
					 dTolerance,
					 enPriceRound,
					 penPriceStatus, VARIANT_FALSE);
			 }
		 }
		 //set active price
		 if(SUCCEEDED(hr) && *pPrice > 0)
			 m_dActivePrice = *pPrice;

	 }
	 catch(_com_error& e){
		 hr = Error((PTCHAR)CComErrorWrapper::ErrorDescription(e), __uuidof(IMmQvFutAtom), e.Error());
	 }
	 return hr;
}

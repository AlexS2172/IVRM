// MmQvFutAtom.cpp : Implementation of CMmQvFutAtom
#include "stdafx.h"
#include "MmQvFutAtom.h"

/////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CMmQvFutAtom::CalcOptionGreeks(IMmQvUndAtom* aUnd, IMmQvOptAtom* aOpt, IMmQvQuoteAtom* aQuote,
											IMmQvExpAtom* aExp, LONG nGreekMask, MmQvIvCalcEnum enCalcIV,
											EtsCalcModelTypeEnum enCalcModel, VARIANT_BOOL bUseTheoVolatility,
											VARIANT_BOOL bUseTheoVolaNoBid, VARIANT_BOOL bUseTheoVolaBadMarket,
											DOUBLE dUndPriceTolerance, EtsPriceRoundingRuleEnum enPriceRoundingRule,
											VARIANT_BOOL bUseCustomRates, DOUBLE dtCalcDate, VARIANT_BOOL ManualEdit,
											ICalculationParametrs* pParams)
{
	if(aUnd == NULL || aOpt == NULL || aQuote == NULL || aExp == NULL || pParams == NULL)
		return Error(L"Invalid objects passed.", IID_IMmQvUndAtom, E_INVALIDARG);

	try
	{
		IMmQvUndAtomPtr spUnd(aUnd);
		IMmQvQuoteAtomPtr spQuote(aQuote);
		IMmQvOptAtomPtr spOpt(aOpt);
		IMmQvExpAtomPtr spExp(aExp);
		IExchAtomPtr spExch;
		LONG nExchID = 0L;
		
		_CHK(spQuote->put_ReplacePriceStatus(enRpsNone));

		_CHK(spQuote->get_Exch(&spExch));
		_CHK(spExch->get_ID(&nExchID));
		spExch = NULL;

		LONG nQtyInShares = 0L;
		_CHK(spOpt->get_QtyInShares(&nQtyInShares));

		if(!IsBadValue(nQtyInShares))
			nGreekMask |= GT_THEOPRICE | GT_DELTA | GT_GAMMA | GT_VEGA | GT_THETA | GT_RHO;

		if(nGreekMask != GT_NOTHING || enCalcIV != enMmQvCalcIvNone)
		{
			GREEKS aGreeks;
			memset(&aGreeks, 0, sizeof(aGreeks));
			aGreeks.nMask = nGreekMask;

			LONG nModel = static_cast<LONG>(enCalcModel);
			LONG nIsAmerican = (m_bIsAmerican ? 1L : 0L);

			IMmQvQuoteAtomPtr spFutQuote;
			_CHK(m_spQuote->get_Item(0L, &spFutQuote));

			DOUBLE dFutPriceBid = 0., dFutPriceAsk = 0., dFutPriceLast = 0.;
			_CHK(spFutQuote->get_PriceBid(&dFutPriceBid));
			_CHK(spFutQuote->get_PriceAsk(&dFutPriceAsk));
			_CHK(spFutQuote->get_PriceLast(&dFutPriceLast));
			
			if(dFutPriceBid > 0. || dFutPriceAsk > 0. || dFutPriceLast > 0.)
			{
				DOUBLE dOptPriceBid = 0., dOptPriceAsk = 0., dOptPriceLast = 0., dOptPriceMid = 0., dOptPriceBidForIv = 0.;
				EtsReplacePriceStatusEnum enMidPriceStatus = enRpsNone;
				EtsReplacePriceStatusEnum enBidPriceStatus = enRpsNone;
				EtsReplacePriceStatusEnum enAskPriceStatus = enRpsNone;

				DOUBLE dFutPriceMid = m_spUndPriceProfile->GetUndPriceMid(dFutPriceBid, dFutPriceAsk, dFutPriceLast, dUndPriceTolerance, enPriceRoundingRule, &enMidPriceStatus, VARIANT_FALSE/*ManualEdit*/);
				
				DOUBLE dFutPriceBidOld = dFutPriceBid;

				dFutPriceBid = m_spUndPriceProfile->GetUndPriceBid(dFutPriceBid, dFutPriceAsk, dFutPriceLast, dUndPriceTolerance, enPriceRoundingRule, &enBidPriceStatus);
				dFutPriceAsk = m_spUndPriceProfile->GetUndPriceAsk(dFutPriceBidOld, dFutPriceAsk, dFutPriceLast, dUndPriceTolerance, enPriceRoundingRule, &enAskPriceStatus);
				_CHK(spFutQuote->put_ReplacePriceStatus((EtsReplacePriceStatusEnum)(enMidPriceStatus | enBidPriceStatus | enAskPriceStatus)));

				if(enCalcIV != enMmQvCalcIvNone || !bUseTheoVolatility)
				{
					_CHK(spQuote->get_PriceBid(&dOptPriceBid));
					_CHK(spQuote->get_PriceAsk(&dOptPriceAsk));
					_CHK(spQuote->get_PriceLast(&dOptPriceLast));
					dOptPriceBidForIv = dOptPriceBid;

					enMidPriceStatus = enRpsNone;
					if(dOptPriceBid > 0. || dOptPriceAsk > 0. || dOptPriceLast > 0.)
						dOptPriceMid = m_spOptPriceProfile->GetOptPriceMid(dOptPriceBid, dOptPriceAsk, dOptPriceLast,
												enPriceRoundingRule, bUseTheoVolatility, 0, &enMidPriceStatus);

					_CHK(spQuote->put_ReplacePriceStatus(enMidPriceStatus));
				}

				if(dFutPriceMid > 0. && (aGreeks.nMask != GT_NOTHING || enCalcIV != enMmQvCalcIvNone))
				{
					LONG nRetCount = 0L;
					DATE dtTemp = 0.;
					DOUBLE dSkew = 0., dKurt = 0.;

					if(MM_EGAR_VSKLOG == nModel)
					{
						_CHK(spUnd->get_Skew(&dSkew));
						_CHK(spUnd->get_Kurt(&dKurt));
					}

					_CHK(spOpt->get_ExpiryOV(&dtTemp));
					DOUBLE dtExpiryOV = dtTemp;
					_CHK(spOpt->get_TradingClose(&dtTemp));
					DOUBLE dtCloseTime = dtTemp;

					DOUBLE dYTE;
					::GetCalculationParams(dtCalcDate, dtExpiryOV, dtCloseTime, pParams->UseTimePrecision != VARIANT_FALSE, &dtCalcDate, &dtExpiryOV, &dtCloseTime, &dYTE);

					DOUBLE dRate = 0., dIVBid = 0., dIVAsk = 0.;

					if(bUseCustomRates)
						_CHK(spExp->get_RateCust(&dRate));
					else
						_CHK(spExp->get_Rate(&dRate));

					DOUBLE dStrike = 0., dVolaForCalcs = 0.;
					EtsOptionTypeEnum enOptType = enOtPut;
					
					_CHK(spOpt->get_Strike(&dStrike));
					_CHK(spOpt->get_OptType(&enOptType));

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
									dIV = ::CalcFutureOptionVolatility(dRate, dFutPriceMid, dOptPriceMid, dStrike, 
														dYTE, enOptType, nIsAmerican,
														100L, dSkew, dKurt, nModel, &nFlag);

									if((!bUseTheoVolaBadMarket || bUseTheoVolaBadMarket && nFlag == VF_OK)
										&& DoubleGTZero(dIV))
									{
										_CHK(spQuote->put_CalcGreeksIVType(enMmQvCalcGreeksIVMarket));
									}
									else if(bUseTheoVolaBadMarket)
									{
										_CHK(spOpt->get_Vola(&dIV));
										_CHK(spQuote->put_CalcGreeksIVType(enMmQvCalcGreeksIVTheo));
									}
									else
										_CHK(spQuote->put_CalcGreeksIVType(enMmQvCalcGreeksIVNone));
								}
								else
								{
									_CHK(spOpt->get_Vola(&dIV));
									_CHK(spQuote->put_CalcGreeksIVType(enMmQvCalcGreeksIVTheo));
								}
							}
							else if(bUseTheoVolaBadMarket)
							{
								_CHK(spOpt->get_Vola(&dIV));
								_CHK(spQuote->put_CalcGreeksIVType(enMmQvCalcGreeksIVTheo));
							}
							else
								_CHK(spQuote->put_CalcGreeksIVType(enMmQvCalcGreeksIVNone));

							dVolaForCalcs = (DoubleGTZero(dIV) ? dIV : BAD_DOUBLE_VALUE);
							_CHK(spQuote->put_IV(dVolaForCalcs));
						}
						else
						{
							if(DoubleGTZero(dOptPriceMid))
							{
								LONG nFlag = VF_OK;
								dIV = ::CalcFutureOptionVolatility(dRate, dFutPriceMid, dOptPriceMid, dStrike, 
													dYTE, enOptType, nIsAmerican,
													100L, dSkew, dKurt, nModel, &nFlag);
							}

							_CHK(spQuote->put_IV(DoubleGTZero(dIV) ? dIV : BAD_DOUBLE_VALUE));

							_CHK(spOpt->get_Vola(&dVolaForCalcs));
							_CHK(spQuote->put_CalcGreeksIVType(enMmQvCalcGreeksIVTheo));
						}
					}
					else
					{
						_CHK(spOpt->get_Vola(&dVolaForCalcs));
						_CHK(spQuote->put_CalcGreeksIVType(enMmQvCalcGreeksIVTheo));
					}

					if(aGreeks.nMask != GT_NOTHING && (bUseTheoVolatility && nExchID == 0L || !bUseTheoVolatility))
					{
						nRetCount = ::CalcFutureOptionGreeks(dRate, dFutPriceMid, dStrike, dVolaForCalcs, dYTE,
									enOptType, nIsAmerican, 100L, dSkew, dKurt, nModel, &aGreeks);

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
									dIV = ::CalcFutureOptionVolatility(dRate, dFutPriceMid, aGreeks.dTheoPrice,
										dStrike, dYTE, enOptType, nIsAmerican, 100L,
										dSkew, dKurt, nModel, &nFlag);
								}
							}
							else
							{
								IMmQvQuoteAtomPtr spDefQuote;
								_CHK(spOpt->get_DefQuote(&spDefQuote));
								ATLASSERT(spDefQuote != NULL);
								_CHK(spDefQuote->get_IV(&dIV));
							}

							_CHK(spQuote->put_IV(DoubleGTZero(dIV) ? dIV : BAD_DOUBLE_VALUE));
						}
					}

					DOUBLE dOptBidOld = dOptPriceBid;

					if(enCalcIV & enMmQvCalcIvBid)
					{
						if(DoubleGTZero(dFutPriceBid))
						{	
							EtsReplacePriceStatusEnum enOldPriceStatus = enRpsNone;
							enBidPriceStatus = enRpsNone;

							dOptPriceBid = m_spOptPriceProfile->GetOptPriceBid(dOptPriceBid, dOptPriceAsk, dOptPriceLast, enPriceRoundingRule, 
								bUseTheoVolatility, (_finite(aGreeks.dTheoPrice) && aGreeks.dTheoPrice > 0.) ? aGreeks.dTheoPrice : 0., &enBidPriceStatus);

							if(enBidPriceStatus != enRpsNone)
							{
								_CHK(spQuote->get_ReplacePriceStatus(&enOldPriceStatus));
								_CHK(spQuote->put_ReplacePriceStatus((EtsReplacePriceStatusEnum)(enOldPriceStatus | enBidPriceStatus)));
							}

							if(DoubleGTZero(dOptPriceBid))
							{
								LONG nFlag = VF_OK;
								dIVBid = ::CalcFutureOptionVolatility(dRate, dFutPriceBid, dOptPriceBid, dStrike, 
															dYTE, enOptType, nIsAmerican,
															100L, dSkew, dKurt, nModel, &nFlag);

								if(!bUseTheoVolatility && bUseTheoVolaBadMarket && nFlag != VF_OK)
									_CHK(spOpt->get_Vola(&dIVBid));
							}
						}
						
						_CHK(spQuote->put_IVBid(DoubleGTZero(dIVBid) ? dIVBid : BAD_DOUBLE_VALUE));
					}
					
					if(enCalcIV & enMmQvCalcIvAsk)
					{
						if(DoubleGTZero(dFutPriceAsk))
						{
							EtsReplacePriceStatusEnum enOldPriceStatus = enRpsNone;
							enAskPriceStatus = enRpsNone;

							dOptPriceAsk = m_spOptPriceProfile->GetOptPriceAsk(dOptBidOld, dOptPriceAsk, dOptPriceLast,
												enPriceRoundingRule, bUseTheoVolatility,
												(_finite(aGreeks.dTheoPrice) && DoubleGTZero(aGreeks.dTheoPrice))
														? aGreeks.dTheoPrice : 0., &enAskPriceStatus);

							if(enAskPriceStatus != enRpsNone)
							{
								_CHK(spQuote->get_ReplacePriceStatus(&enOldPriceStatus));
								_CHK(spQuote->put_ReplacePriceStatus((EtsReplacePriceStatusEnum)(enOldPriceStatus | enAskPriceStatus)));
							}

							if(DoubleGTZero(dOptPriceAsk))
							{
								LONG nFlag = VF_OK;
								dIVAsk = ::CalcFutureOptionVolatility(dRate, dFutPriceAsk, dOptPriceAsk, dStrike, 
															dYTE, enOptType, nIsAmerican,
															100L, dSkew, dKurt, nModel, &nFlag);

								if(!bUseTheoVolatility && bUseTheoVolaBadMarket && nFlag != VF_OK)
									_CHK(spOpt->get_Vola(&dIVAsk));
							}
						}
					
						_CHK(spQuote->put_IVAsk(DoubleGTZero(dIVAsk) ? dIVAsk : BAD_DOUBLE_VALUE));

						if(DoubleGTZero(dIVBid) && DoubleGTZero(dIVAsk))
							_CHK(spQuote->put_IVSpread(dIVBid - dIVAsk));
						else
							_CHK(spQuote->put_IVSpread(BAD_DOUBLE_VALUE));
					}

					if(aGreeks.nMask != GT_NOTHING &&
						(bUseTheoVolatility && nExchID == 0L || !bUseTheoVolatility))
					{
						_CHK(spQuote->ClearValues(VARIANT_FALSE, VARIANT_FALSE));

							if ((aGreeks.dTheoPrice > BAD_DOUBLE_VALUE)&&(aGreeks.dTheoPrice < 0))
								aGreeks.dTheoPrice = 0;

							if((aGreeks.nMask & GT_THEOPRICE) && _finite(aGreeks.dTheoPrice) && aGreeks.dTheoPrice >= 0.)

								_CHK(spQuote->put_PriceTheo(aGreeks.dTheoPrice));
							if((aGreeks.nMask & GT_DELTA) && _finite(aGreeks.dDelta))
							{
								if( //lDayShift &&
									(dYTE) < 0 && 
									//(nExpiry - nTodayPure)>0 && 
									DoubleEQZero(aGreeks.dDelta)&&
									((enOptType == enOtPut && dFutPriceMid < dStrike) || (enOptType == enOtCall && dFutPriceMid > dStrike))
									)
								{
									
									
									if (enOptType == enOtPut)
										_CHK(spQuote->put_Delta(-100.));
									else
									_CHK(spQuote->put_Delta(100.));
								}
								else
								{
									_CHK(spQuote->put_Delta(aGreeks.dDelta * 100.));
								}

								//_CHK(spQuote->put_Delta(aGreeks.dDelta * 100.));
							}
							if((aGreeks.nMask & GT_GAMMA) && _finite(aGreeks.dGamma))
								_CHK(spQuote->put_Gamma(aGreeks.dGamma * 100.));
							if((aGreeks.nMask & GT_THETA) && _finite(aGreeks.dTheta))
								_CHK(spQuote->put_Theta(aGreeks.dTheta * 100.));
							if((aGreeks.nMask & GT_VEGA) && _finite(aGreeks.dVega))
								_CHK(spQuote->put_Vega(aGreeks.dVega * 100.));
							if((aGreeks.nMask & GT_RHO) && _finite(aGreeks.dRho))
								_CHK(spQuote->put_Rho(aGreeks.dRho * 100.));
							if((aGreeks.nMask & GT_DELTA_VEGA) && _finite(aGreeks.dDeltaVega))
								_CHK(spQuote->put_VegaDelta(aGreeks.dDeltaVega * 100.));
							if((aGreeks.nMask & GT_GAMMA_VEGA) && _finite(aGreeks.dGammaVega))
								_CHK(spQuote->put_VegaGamma(aGreeks.dGammaVega * 100.));
							if((aGreeks.nMask & GT_DELTA_THETA) && _finite(aGreeks.dDeltaTheta))
								_CHK(spQuote->put_ThetaDelta(aGreeks.dDeltaTheta * 100.));
							if((aGreeks.nMask & GT_GAMMA_THETA) && _finite(aGreeks.dGammaTheta))
								_CHK(spQuote->put_ThetaGamma(aGreeks.dGammaTheta * 100.));
							if((aGreeks.nMask & GT_GAMMA) && (aGreeks.nMask & GT_THETA) 
								&& _finite(aGreeks.dGamma) && _finite(aGreeks.dTheta)
								&& fabs(aGreeks.dTheta) >= .0001)
								_CHK(spQuote->put_GammaPerTheta(aGreeks.dGamma / aGreeks.dTheta));
						//}
					}
					else if(aGreeks.nMask != GT_NOTHING && bUseTheoVolatility && nExchID != 0)
					{
						IMmQvQuoteAtomPtr spDefQuote;
						_CHK(spOpt->get_DefQuote(&spDefQuote));
						ATLASSERT(spDefQuote != NULL);

						DOUBLE dValue = 0.;
						_CHK(spDefQuote->get_PriceTheo(&dValue));
						_CHK(spQuote->put_PriceTheo(dValue));
						_CHK(spDefQuote->get_Delta(&dValue));
						_CHK(spQuote->put_Delta(dValue));
						_CHK(spDefQuote->get_Gamma(&dValue));
						_CHK(spQuote->put_Gamma(dValue));
						_CHK(spDefQuote->get_Theta(&dValue));
						_CHK(spQuote->put_Theta(dValue));
						_CHK(spDefQuote->get_Vega(&dValue));
						_CHK(spQuote->put_Vega(dValue));
						_CHK(spDefQuote->get_Rho(&dValue));
						_CHK(spQuote->put_Rho(dValue));
						_CHK(spDefQuote->get_VegaDelta(&dValue));
						_CHK(spQuote->put_VegaDelta(dValue));
						_CHK(spDefQuote->get_VegaGamma(&dValue));
						_CHK(spQuote->put_VegaGamma(dValue));
						_CHK(spDefQuote->get_ThetaDelta(&dValue));
						_CHK(spQuote->put_ThetaDelta(dValue));
						_CHK(spDefQuote->get_ThetaGamma(&dValue));
						_CHK(spQuote->put_ThetaGamma(dValue));
						_CHK(spDefQuote->get_GammaPerTheta(&dValue));
						_CHK(spQuote->put_GammaPerTheta(dValue));
						_CHK(spDefQuote->get_IVBid(&dValue));
						_CHK(spQuote->put_IVBid(dValue));
						_CHK(spDefQuote->get_IVAsk(&dValue));
						_CHK(spQuote->put_IVAsk(dValue));
						_CHK(spDefQuote->get_IVSpread(&dValue));
						_CHK(spQuote->put_IVSpread(dValue));
					}
				}
			}
		}
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)CComErrorWrapper::ErrorDescription(e), IID_IMmQvUndAtom, e.Error());
	}

	return S_OK;
}

/////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CMmQvFutAtom::CalcAllOptions(IMmQvUndAtom* aUnd, LONG nCallGreekMask, LONG nPutGreekMask,
											MmQvIvCalcEnum enCallCalcIV, MmQvIvCalcEnum enPutCalcIV, 
											EtsCalcModelTypeEnum enCalcModel, VARIANT_BOOL bUseTheoVolatility,
											VARIANT_BOOL bUseTheoVolaNoBid, VARIANT_BOOL bUseTheoVolaBadMarket,
											VARIANT_BOOL bRecalcGreeks, DOUBLE dUndPriceTolerance,
											EtsPriceRoundingRuleEnum enPriceRoundingRule, VARIANT_BOOL bUseCustomRates,
											LONG nCalcSleepFreq, LONG nCalcSleepAmt, DOUBLE dtCalcDate,
											VARIANT_BOOL ManualEdit, ICalculationParametrs* pParams)
{
	if(aUnd == NULL || pParams == NULL)
		return Error(L"Invalid objects passed.", IID_IMmQvUndAtom, E_INVALIDARG);

	try
	{
		HRESULT hr;

		m_bStopCalc = FALSE;

		m_dNetDelta = BAD_DOUBLE_VALUE;
		m_dTotalDelta = BAD_DOUBLE_VALUE;
		m_dTotalGamma = BAD_DOUBLE_VALUE;
		m_dTotalTheta = BAD_DOUBLE_VALUE;
		m_dTotalVega = BAD_DOUBLE_VALUE;
		m_dTotalRho = BAD_DOUBLE_VALUE;

		IUnknownPtr spUnk;
		_variant_t varItem;
		ULONG nFetched = 0L;
		IMmQvUndAtomPtr spUnd(aUnd);
		ICalculationParametrsPtr spParams(pParams);

		IVolaControlPtr spVolaSrv;
		_CHK(spUnd->get_VolaSrv(&spVolaSrv));

		IMmQvQuoteAtomPtr spFutQuote;
		_CHK(m_spQuote->get_Item(0L, &spFutQuote));

		DOUBLE dFutPriceBid = 0., dFutPriceAsk = 0., dFutPriceLast = 0.;
		_CHK(spFutQuote->get_PriceBid(&dFutPriceBid));
		_CHK(spFutQuote->get_PriceAsk(&dFutPriceAsk));
		_CHK(spFutQuote->get_PriceLast(&dFutPriceLast));
		
		EtsReplacePriceStatusEnum enMidPriceStatus = enRpsNone;
		DOUBLE dFutPriceMid = m_spUndPriceProfile->GetUndPriceMid(dFutPriceBid, dFutPriceAsk, dFutPriceLast, dUndPriceTolerance, enPriceRoundingRule, &enMidPriceStatus, VARIANT_FALSE/*ManualEdit*/);		
		_CHK(spFutQuote->put_ReplacePriceStatus(enMidPriceStatus));
		
		spFutQuote = NULL;

		IMmQvExpAtomPtr spExpiry;

		_CHK(m_spExp->get__NewEnum(&spUnk), _T("Fail to get expiry collection."));
		
		IEnumVARIANTPtr spExpEnum(spUnk);
		_CHK(spExpEnum->Reset(), _T("Fail to reset expiry collection."));
		while((hr = spExpEnum->Next(1L, &varItem, &nFetched)) == S_OK)
		{
			ATLASSERT(varItem.vt == VT_DISPATCH);
			spExpiry = varItem;
			if(nFetched > 0 && spExpiry != NULL)
			{
				_CHK(spExpiry->FindAtmStrike(dFutPriceMid));

				IMmQvStrikeAtomPtr spStrike;
				IMmQvStrikeCollPtr spStrikes;
				_CHK(spExpiry->get_Strike(&spStrikes), _T("Fail to get strikes collection."));
				_CHK(spStrikes->get__NewEnum(&spUnk), _T("Fail to get strikes collection."));
	
				IEnumVARIANTPtr spStrEnum(spUnk);
				_CHK(spStrEnum->Reset(), _T("Fail to reset strikes collection."));

				while((hr = spStrEnum->Next(1L, &varItem, &nFetched)) == S_OK)
				{
					ATLASSERT(varItem.vt == VT_DISPATCH);
					spStrike = varItem;
					if(nFetched > 0 && spStrike != NULL)
					{
						IMmQvOptPairAtomPtr spOptPair;
						IMmQvOptPairCollPtr spOptPairs;
						_CHK(spStrike->get_OptPair(&spOptPairs), _T("Fail to get option pairs collection."));
						_CHK(spOptPairs->get__NewEnum(&spUnk), _T("Fail to get option pairs collection."));

						IEnumVARIANTPtr spOptPairEnum(spUnk);
						_CHK(spOptPairEnum->Reset(), _T("Fail to reset option pairs collection."));

						while((hr = spOptPairEnum->Next(1L, &varItem, &nFetched)) == S_OK)
						{
							ATLASSERT(varItem.vt == VT_DISPATCH);
							spOptPair = varItem;
							if(nFetched > 0 && spOptPair != NULL)
							{
								IMmQvOptAtomPtr spOpt;
								LONG nGreekMask; MmQvIvCalcEnum enCalcIV;

								for(LONG i = 0L; i < 2L; i++)
								{
									EtsOptionTypeEnum enOptType = static_cast<EtsOptionTypeEnum>(i);
									nGreekMask = (enOptType != enOtCall ? nPutGreekMask : nCallGreekMask);
									enCalcIV = (enOptType != enOtCall ? enPutCalcIV : enCallCalcIV);

									_CHK(spOptPair->get_Opt(enOptType, &spOpt));
									if(spOpt != NULL)
									{
										LONG nOptID = 0L;
										_CHK(spOpt->get_ID(&nOptID));

										if(nOptID != 0L)
										{
											if(bRecalcGreeks)
											{
												DOUBLE dVola = 0., dStrike = 0.; DATE dtExpiryOV = 0.;
												LONG lSurfaceID = 0;
												_CHK(spOpt->get_ExpiryOV(&dtExpiryOV));
												_CHK(spOpt->get_Strike(&dStrike));

												lSurfaceID = spVolaSrv->GetSurfaceByRoot(m_nFutRootID);
												_CHK(spVolaSrv->get_OptionVola(dtExpiryOV, dStrike, lSurfaceID, &dVola));
												_CHK(spOpt->put_Vola(dVola));
											}

											IMmQvQuoteAtomPtr spQuote;
											IMmQvQuoteCollPtr spQuotes;
											_CHK(spOpt->get_Quote(&spQuotes), _T("Fail to get quotes collection."));
											_CHK(spQuotes->get__NewEnum(&spUnk), _T("Fail to get quotes collection."));
								
											IEnumVARIANTPtr spQuoteEnum(spUnk);
											_CHK(spQuoteEnum->Reset(), _T("Fail to reset quotes collection."));

											while((hr = spQuoteEnum->Next(1L, &varItem, &nFetched)) == S_OK)
											{
												ATLASSERT(varItem.vt == VT_DISPATCH);
												spQuote = varItem;
												if(nFetched > 0 && spQuote != NULL)
												{
													if(bRecalcGreeks)
													{
														_CHK(CalcOptionGreeks(spUnd, spOpt, spQuote, spExpiry, 
																	nGreekMask, enCalcIV, enCalcModel, bUseTheoVolatility, 
																	bUseTheoVolaNoBid, bUseTheoVolaBadMarket,
																	dUndPriceTolerance, enPriceRoundingRule,
																	bUseCustomRates, dtCalcDate, ManualEdit,
																	spParams), _T("Fail to calculate option greeks."));
														
														_CalcSleep(nCalcSleepFreq, nCalcSleepAmt);
														if(m_bStopCalc)
															CComErrorWrapper::ThrowError(HRESULT_FROM_WIN32(ERROR_CANCELLED), _T("Calculation is stopped"));
													}

													IExchAtomPtr spExch;
													LONG nExchID = 0L;
													
													_CHK(spQuote->get_Exch(&spExch));
													_CHK(spExch->get_ID(&nExchID));
													spExch = NULL;

													if(nExchID == 0L)
													{
														LONG nQtyInShares = 0L;
														_CHK(spOpt->get_QtyInShares(&nQtyInShares));

														if(!IsBadValue(nQtyInShares))
														{
															DOUBLE dValue = 0.;

															_CHK(spQuote->get_Gamma(&dValue));
															if(!IsBadValue(dValue))	
															{
																if(IsBadValue(m_dTotalGamma))
																	m_dTotalGamma = 0.;

																m_dTotalGamma += nQtyInShares * dValue / 100.;
															}

															_CHK(spQuote->get_Delta(&dValue));
															if(!IsBadValue(dValue))
															{
																if(IsBadValue(m_dTotalDelta))
																	m_dTotalDelta = 0.;

																m_dTotalDelta += nQtyInShares * dValue / 100.;
															}

															_CHK(spQuote->get_Theta(&dValue));
															if(!IsBadValue(dValue))
															{
																if(IsBadValue(m_dTotalTheta))
																	m_dTotalTheta = 0.;

																m_dTotalTheta += nQtyInShares * dValue / 100.;
															}

															_CHK(spQuote->get_Vega(&dValue));
															if(!IsBadValue(dValue))
															{
																if(IsBadValue(m_dTotalVega))
																	m_dTotalVega = 0.;

																m_dTotalVega += nQtyInShares * dValue / 100.;
															}

															_CHK(spQuote->get_Rho(&dValue));
															if(!IsBadValue(dValue))
															{
																if(IsBadValue(m_dTotalRho))
																	m_dTotalRho = 0.;

																m_dTotalRho += nQtyInShares * dValue / 100.;
															}
														}		
													}
												}
												spQuote = 0;
												varItem.Clear();
											}
											_CHK(hr, _T("Fail to get next quote."));
										}
									}
								}
							}
							spOptPair = 0;
							varItem.Clear();
						}
						_CHK(hr, _T("Fail to get next option pair."));
					}
					spStrike = 0;
					varItem.Clear();
				}
				_CHK(hr, _T("Fail to get next strike."));
			}
			spExpiry = 0;
			varItem.Clear();
		}
		_CHK(hr, _T("Fail to get next expiry."));

		if(!IsBadValue(m_nQtyInShares))
			m_dNetDelta = m_nQtyInShares;

		if(!IsBadValue(m_dTotalDelta))
		{
			if(IsBadValue(m_dNetDelta))
				m_dNetDelta = 0.;

			m_dNetDelta += m_dTotalDelta;
		}
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)CComErrorWrapper::ErrorDescription(e), IID_IMmQvUndAtom, e.Error());
	}

	return S_OK;
}

/////////////////////////////////////////////////////////////////////////////
//
void CMmQvFutAtom::_CalcSleep(LONG nCalcSleepFreq, LONG nCalcSleepAmt)
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
		m_nFutRootID = 0L;
		//m_nExpCalendarID = 0L;
		m_dtMaturityDate = 0.;
		m_dtMaturityMonth = 0.;
		m_bIsAmerican = VARIANT_FALSE;
		m_nQty = BAD_LONG_VALUE;
		m_nQtyInShares = BAD_LONG_VALUE;
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
	/*catch(...)
	{
		hr =  Error(_T("Unknown Error"), IID_IMmQvFutAtom, E_FAIL);
	}*/
	return hr;
}

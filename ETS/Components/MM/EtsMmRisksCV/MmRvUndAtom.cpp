// MmRvUndAtom.cpp : Implementation of CMmRvUndAtom

#include "stdafx.h"
#include "MmRvUndAtom.h"
#include "MmRvUndColl.h"
#include "MmRvUndAtom.h"

/////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CMmRvUndAtom::GetSyntheticUnderlyingPrice(IMmRvUndColl* aUndColl,
														ISynthRootAtom* aSynthRoot,
														DOUBLE* pdSynthBid,
														DOUBLE* pdSynthAsk,
														DOUBLE* pdSynthLast)
{
	if(aUndColl == NULL)
		return Error(L"Invalid object passed.", IID_IMmRvUndAtom, E_INVALIDARG);

	__CHECK_POINTER(pdSynthBid);
	__CHECK_POINTER(pdSynthAsk);
	__CHECK_POINTER(pdSynthLast);

	*pdSynthBid = BAD_DOUBLE_VALUE;
	*pdSynthAsk = BAD_DOUBLE_VALUE;
	*pdSynthLast = BAD_DOUBLE_VALUE;

	if(!m_bHasSynthetic || m_spSynthRoots == NULL || aSynthRoot == NULL)
		return S_OK;

	try
	{
		IUnknownPtr spUnk;
		_variant_t varItem;
		ULONG nFetched = 0L;
		HRESULT hr;

		ISynthRootAtomPtr spSynthRoot(aSynthRoot);

		IMmRvUndCollPtr spUndColl(aUndColl);
		IMmRvUndAtomPtr spSynthUnd;

		bool bBadSpotBid = false, bBadSpotAsk = false, bBadSpotLast = false;


		*pdSynthBid = spSynthRoot->CashValue;
		*pdSynthAsk = *pdSynthBid;
		*pdSynthLast = *pdSynthBid;

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
						*pdSynthBid += dPrice * dWeight;
					else
					{
						bBadSpotBid = true;
						*pdSynthBid = BAD_DOUBLE_VALUE;
					}

					_CHK(spSynthUnd->get_PriceAsk(&dPrice));
					if(!bBadSpotAsk && dPrice > DBL_EPSILON)
						*pdSynthAsk += dPrice * dWeight;
					else
					{
						bBadSpotAsk = true;
						*pdSynthAsk = BAD_DOUBLE_VALUE;
					}

					_CHK(spSynthUnd->get_PriceLast(&dPrice));
					if(!bBadSpotLast && dPrice > DBL_EPSILON)
						*pdSynthLast += dPrice * dWeight;
					else
					{
						bBadSpotLast = true;
						*pdSynthLast = BAD_DOUBLE_VALUE;
					}
				}
			}
		}
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_IMmRvUndAtom, e.Error());
	}
	return S_OK;
}

/////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CMmRvUndAtom::Calc(IMmRvUndColl* aUndColl,
								LONG nMask,
								VARIANT_BOOL bCalcGreeks,
								VARIANT_BOOL bUpdateVola,
								VARIANT_BOOL bRealtimeCalc,
								VARIANT_BOOL bClearSynthGreeks,
								IEtsProcessDelay* aDelay,
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
								VARIANT_BOOL bCalcOnlyDirty,
								ICalculationParametrs* pParams,
								LONG* pnOptUpdated,
								LONG* pnUndUpdated,
								LONG* pnFutUpdated)
{
	if(aUndColl == NULL)
		return Error(L"Invalid object passed.", IID_IMmRvUndAtom, E_INVALIDARG);

	if(!bSimulate)
	{
		m_simulationParams.m_dDate = 0;
		m_simulationParams.m_dSpotChangePercent = 0.;
		m_simulationParams.m_dVolaChangePercent = 0.;
	}

   try
	{
		IMmRvUndCollPtr spUndColl(aUndColl);

		LONG nOptUpdated = 0L, nUndUpdated = 0L, nFutUpdated = 0L;

		IEtsProcessDelayPtr spDelay(aDelay);
		if(spDelay != NULL && spDelay->IsInterrupted)
			return S_OK;

		ICalculationParametrsPtr spParams(pParams);
		
		if(bUpdateVola)
			bCalcGreeks = VARIANT_TRUE;

		__MmRvUndAtom::ClearValues();

		if(bClearSynthGreeks)
			_ClearSynthGreeks();

		EtsReplacePriceStatusEnum enUndPriceStatusMid = enRpsNone;
		EtsReplacePriceStatusEnum enUndPriceStatusBid = enRpsNone;
		EtsReplacePriceStatusEnum enUndPriceStatusAsk = enRpsNone;
		DOUBLE dUndPriceMid = 0., dUndPriceBid = 0., dUndPriceAsk = 0.;

		if(m_spUndPriceProfile != NULL)
		{
			dUndPriceMid = m_spUndPriceProfile->GetUndPriceMid(m_dPriceBid, m_dPriceAsk,
				m_dPriceLast, dUndPriceTolerance, enPriceRoundingRule, &enUndPriceStatusMid, VARIANT_FALSE);

			dUndPriceBid = m_spUndPriceProfile->GetUndPriceBidForPnL(m_dPriceBid, m_dPriceAsk,
				m_dPriceLast, dUndPriceTolerance, enPriceRoundingRule, &enUndPriceStatusBid);

			dUndPriceAsk = m_spUndPriceProfile->GetUndPriceAskForPnL(m_dPriceBid, m_dPriceAsk,
				m_dPriceLast, dUndPriceTolerance, enPriceRoundingRule, &enUndPriceStatusAsk);
		}
		
		//double dPriceRatio = 1.;
		//double dShiftValue = 0.; 

		//if ( bSimulate ){
		//	dShiftValue = static_cast<double>(m_simulationParams.m_lSpotChangePercent) / 100.;
		//	if ( /*CorrelatedShift*/ true ){
		//		if ( dShiftValue + 1. > 0.)
		//			dPriceRatio = exp( log ( dShiftValue + 1.) * m_dBeta) - 1.;
		//	}
		//}

		//dUndPriceMid += (dUndPriceMid * dPriceRatio);
		//dUndPriceBid += (dUndPriceBid * dPriceRatio);
		//dUndPriceAsk += (dUndPriceAsk * dPriceRatio);

		if ( bSimulate ) {
			bUpdateVola = VARIANT_FALSE;
			dUndPriceMid += ( (dUndPriceMid / 100.) * static_cast<double>(m_simulationParams.m_dSpotChangePercent) );
			dUndPriceAsk += ( (dUndPriceAsk / 100.) * static_cast<double>(m_simulationParams.m_dSpotChangePercent) );
			dUndPriceBid += ( (dUndPriceBid / 100.) * static_cast<double>(m_simulationParams.m_dSpotChangePercent) );
		}
		m_dPrice = dUndPriceMid ;



		m_enReplacePriceStatus = static_cast<EtsReplacePriceStatusEnum>(enUndPriceStatusMid | enUndPriceStatusBid | enUndPriceStatusAsk);
	                
		DATE dtToday = vt_date::GetCurrentDate(true);
		VARIANT_BOOL bNeedRecalc = VARIANT_FALSE;
		VARIANT_BOOL bPosCalcGreeks = VARIANT_FALSE;
		VARIANT_BOOL bCalcTheos = VARIANT_FALSE;
		DOUBLE dPosValue = BAD_DOUBLE_VALUE;

		bool bHasItems = false;
		m_vecPos.clear();

		CComObject<CMmRvPosColl>* pPosColl = dynamic_cast<CComObject<CMmRvPosColl>*>(m_spPos.GetInterfacePtr());
		CMmRvPosColl::EnumIterType itPos	= pPosColl->m_coll.begin();
		CMmRvPosColl::EnumIterType itPosEnd = pPosColl->m_coll.end();

		for(; itPos != itPosEnd; ++itPos)
		{
			bHasItems = true;
			IMmRvPosAtomPtr spPos = itPos->second;

			if(spPos != NULL)
			{
				m_vecPos.push_back(spPos);

				VARIANT_BOOL bVisible = VARIANT_FALSE;
				_CHK(spPos->get_Visible(&bVisible));
				if(bVisible)
				{
					EtsContractTypeEnum enContractType = enCtNone;
					_CHK(spPos->get_ContractType(&enContractType));
					LONG nPosQty = 0L;
					_CHK(spPos->get_Qty(&nPosQty));

					LONG nPosQtyInShares = 0L;
					_CHK(spPos->get_QtyInShares(&nPosQtyInShares));

					// options
					if(enContractType == enCtOption || enContractType == enCtFutOption)
					{
						if(m_nOptQty <= BAD_LONG_VALUE) m_nOptQty = 0L;
						m_nOptQty += nPosQty;

						DATE dtExpiry = 0.;
						_CHK(spPos->get_Expiry(&dtExpiry));
						DOUBLE dStrike = 0.;
						_CHK(spPos->get_Strike(&dStrike));
						DATE dtExpiryOV = 0.;
						_CHK(spPos->get_ExpiryOV(&dtExpiryOV));
						LONG nOptionRootID = 0;
						_CHK(spPos->get_OptionRootID(&nOptionRootID));

						if(dtExpiryOV >= dtToday)
						{
							_CHK(spPos->get_CalcGreeks(&bPosCalcGreeks));

							if(bRealtimeCalc)
							{
								bCalcTheos = m_bCalcGreeks || m_bCalcTotals || bPosCalcGreeks;

								if(!bCalcTheos && enContractType == enCtFutOption)
								{
									IMmRvFutAtomPtr spFut;
									_CHK(spPos->get_Fut(&spFut));

									if(spFut != NULL)
									{
										VARIANT_BOOL bFutCalcGreeks = VARIANT_FALSE;
										_CHK(spFut->get_CalcGreeks(&bFutCalcGreeks));
										bCalcTheos = bCalcTheos || bFutCalcGreeks;
									}
								}

								bUpdateVola = m_bCalcGreeks;
								bNeedRecalc = m_bCalcTotals;
							}
							else
							{
								bCalcTheos = bCalcGreeks;
								bNeedRecalc = VARIANT_TRUE;
							}

							VARIANT_BOOL bIsDirty = VARIANT_FALSE;
							_CHK(spPos->get_IsDirty(&bIsDirty));

							if(bNeedRecalc /*&& (bCalcOnlyDirty == VARIANT_FALSE || (bCalcOnlyDirty && bIsDirty))*/)
							{
								nOptUpdated++;

								if(bUpdateVola)
								{
									LONG lSurfaceID = m_spVolaSrv->GetSurfaceByRoot(nOptionRootID);
									_CHK(spPos->put_Vola(m_spVolaSrv->GetOptionVola(dtExpiryOV, dStrike, lSurfaceID)));
								}

								if(enContractType == enCtOption)
								{
									_CHK(spPos->CalcOptionGreeks(spUndColl, this, dUndPriceMid,
										(bCalcTheos ? nMask : GT_NOTHING), bIsPnlLTD,
										bFitToImpFlatNoBid, enFitToImpCurve, enCalcModel,
										bUseTheoVolatility, bUseTheoVolaNoBid, bUseTheoVolaBadMarket,
										dUndPriceTolerance, enPriceRoundingRule, NULL, 
										bSimulate, VARIANT_FALSE, spParams) );
								}
								else
								{
									_CHK(spPos->CalcFutOptionGreeks(this, dUndPriceMid,
										(bCalcTheos ? nMask : GT_NOTHING), bIsPnlLTD, enCalcModel,
										bUseTheoVolatility, bUseTheoVolaNoBid, bUseTheoVolaBadMarket,
										dUndPriceTolerance, enPriceRoundingRule, bSimulate, VARIANT_FALSE, spParams));
								}

								if(spDelay != NULL)
									spDelay->Sleep();
							}

							_CHK(spPos->put_IsDirty(VARIANT_FALSE));

							_CalcTotalsForOptionPos(spPos, dUndPriceMid);
						}
						else
							__MmRvUndAtom::SetAllValuesAsBad();
					}

					// underlyings
					else if(enContractType == enCtStock || enContractType == enCtIndex)
					{
						if(m_nQty <= BAD_LONG_VALUE) m_nQty = 0L;
						m_nQty += nPosQtyInShares;

						nUndUpdated++;

						_CHK(spPos->CalcPnlMtm(bIsPnlLTD, dUndPriceBid, dUndPriceAsk, VARIANT_TRUE));

						if(dUndPriceMid > DBL_EPSILON)
						{
							if(m_dDeltaEq <= BAD_DOUBLE_VALUE) m_dDeltaEq = 0.;
							m_dDeltaEq += nPosQtyInShares * dUndPriceMid;
						}
						else
							m_bBadDeltaEq = VARIANT_TRUE;

						if(m_dNetDelta <= BAD_DOUBLE_VALUE) m_dNetDelta = 0.;
						m_dNetDelta += nPosQtyInShares;
					}

					// futures
					else if(enContractType == enCtFuture)
					{
						if(m_nFutQty <= BAD_LONG_VALUE) m_nFutQty = 0L;
						m_nFutQty += nPosQty;

						if(m_nQty <= BAD_LONG_VALUE) m_nQty = 0L;
						m_nQty += nPosQtyInShares;

						nFutUpdated++;

						IMmRvFutAtomPtr spFut;
						_CHK(spPos->get_Fut(&spFut));

						if(spFut != NULL)
						{
							EtsReplacePriceStatusEnum enFutPriceStatusMid = enRpsNone;
							EtsReplacePriceStatusEnum enFutPriceStatusBid = enRpsNone;
							EtsReplacePriceStatusEnum enFutPriceStatusAsk = enRpsNone;
							DOUBLE dFutPriceMid = 0., dFutPriceBid = 0., dFutPriceAsk = 0.;
							IEtsPriceProfileAtomPtr spUndPriceProfile;

							_CHK(spFut->get_UndPriceProfile(&spUndPriceProfile));

							if(spUndPriceProfile != NULL)
							{
								DOUBLE dPriceBid = 0., dPriceAsk = 0., dPriceLast = 0.;

								_CHK(spFut->get_PriceBid(&dPriceBid));
								_CHK(spFut->get_PriceAsk(&dPriceAsk));
								_CHK(spFut->get_PriceLast(&dPriceLast));

								dFutPriceMid = spUndPriceProfile->GetUndPriceMid(dPriceBid, dPriceAsk,
									dPriceLast, dUndPriceTolerance, enPriceRoundingRule, &enFutPriceStatusMid, VARIANT_FALSE);

								dFutPriceBid = spUndPriceProfile->GetUndPriceBidForPnL(dPriceBid, dPriceAsk,
									dPriceLast, dUndPriceTolerance, enPriceRoundingRule, &enFutPriceStatusBid);

								dFutPriceAsk = spUndPriceProfile->GetUndPriceAskForPnL(dPriceBid, dPriceAsk,
									dPriceLast, dUndPriceTolerance, enPriceRoundingRule, &enFutPriceStatusAsk);
								if ( bSimulate ) {
									bUpdateVola = VARIANT_FALSE;
									dFutPriceMid += ( dFutPriceMid /100 * static_cast<double>(m_simulationParams.m_dSpotChangePercent) );
									dFutPriceAsk += ( dFutPriceAsk /100 * static_cast<double>(m_simulationParams.m_dSpotChangePercent) );
									dFutPriceBid += ( dFutPriceBid /100 * static_cast<double>(m_simulationParams.m_dSpotChangePercent) );
								}
							}
						          
							_CHK(spPos->put_ReplacePriceStatus(static_cast<EtsReplacePriceStatusEnum>(enFutPriceStatusMid | enFutPriceStatusBid | enFutPriceStatusAsk)));

							_CHK(spPos->CalcPnlMtm(bIsPnlLTD, dFutPriceBid, dFutPriceAsk, VARIANT_TRUE));
						}

						if(dUndPriceMid > DBL_EPSILON)
						{
							if(m_dDeltaEq <= BAD_DOUBLE_VALUE) m_dDeltaEq = 0.;
							m_dDeltaEq += nPosQtyInShares * dUndPriceMid;
						}
						else
							m_bBadDeltaEq = VARIANT_TRUE;

						if(m_dNetDelta <= BAD_DOUBLE_VALUE) m_dNetDelta = 0.;
						m_dNetDelta += nPosQtyInShares;
					}

					// PnlMtm
					_CHK(spPos->get_PnlMtm(&dPosValue));
					if(dPosValue > BAD_DOUBLE_VALUE)
					{
						if(m_dPnlMtm <= BAD_DOUBLE_VALUE) m_dPnlMtm = 0.;
						m_dPnlMtm += dPosValue;
					}
					else
						m_bBadPnlMtm = VARIANT_TRUE;

					// PnlTheo
					_CHK(spPos->get_PnlTheo(&dPosValue));
					if(dPosValue > BAD_DOUBLE_VALUE)
					{
						if(m_dPnlTheo <= BAD_DOUBLE_VALUE) m_dPnlTheo = 0.;
						m_dPnlTheo += dPosValue;
					}
					else
						m_bBadPnlTheo = VARIANT_TRUE;
				}
			}
		}

		if(m_dDeltaEq > BAD_DOUBLE_VALUE && m_dBeta > BAD_DOUBLE_VALUE && DoubleNEQZero(m_dBeta))
		{
			m_dBetaWtdDeltaEq = m_dDeltaEq * m_dBeta;
			if(m_bBadDeltaEq) m_bBadBetaWtdDeltaEq = VARIANT_TRUE;
		}
		else
			m_bBadBetaWtdDeltaEq = VARIANT_TRUE;



		if(pnOptUpdated)
			*pnOptUpdated += nOptUpdated;

		if(pnUndUpdated)
			*pnUndUpdated += nUndUpdated;

		if(pnFutUpdated)
			*pnFutUpdated += nFutUpdated;

		/*if ( m_spRTContext != NULL && m_nPoolID != BAD_LONG_VALUE ) 
			_CHK(m_spRTContext->Recalculate(m_nPoolID));*/

		// Greeks view
		if(m_enGroupingType != enGroupingNone)
			CalcGroupingTotals(dUndPriceMid);

		if (	m_spSimulation != NULL && aSimulatedUndColl != NULL && bCalcSimulation &&
				!( 
					m_simulationParams.m_dDate ==  vt_date::GetCurrentDate() 	&& 
					!m_simulationParams.m_dSpotChangePercent && 
					!m_simulationParams.m_dVolaChangePercent
				) 
			) {
			_CHK(m_spSimulation->Calc(	aSimulatedUndColl,
											nMask,
											bCalcGreeks,
											bUpdateVola,
											bRealtimeCalc,
											bClearSynthGreeks,
											aDelay,
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
											bCalcOnlyDirty,
											spParams,
											pnOptUpdated,
											pnUndUpdated,
											pnFutUpdated ));

		}

		m_bIsRecalculated = VARIANT_TRUE;
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_IMmRvUndAtom, e.Error());
	}
	_CATCH_UNHANDLED_EXCEPTION_RETURN_COM;
	return S_OK;
}

/////////////////////////////////////////////////////////////////////////////
//
void CMmRvUndAtom::_CalcTotalsForOptionPos(IMmRvPosAtomPtr spPos, DOUBLE dUndPriceMid) throw()
{
	DOUBLE dPosValue = BAD_DOUBLE_VALUE;
	// DeltaInShares
	_CHK(spPos->get_DeltaInShares(&dPosValue));
	if(dPosValue > BAD_DOUBLE_VALUE)
	{
		if(m_dOptDelta <= BAD_DOUBLE_VALUE) m_dOptDelta = 0.;
		m_dOptDelta += dPosValue;
		if(m_dNetDelta <= BAD_DOUBLE_VALUE) m_dNetDelta = 0.;
		m_dNetDelta += dPosValue;
		if(dUndPriceMid > DBL_EPSILON)
		{
			if(m_dDeltaEq <= BAD_DOUBLE_VALUE) m_dDeltaEq = 0.;
			m_dDeltaEq += dPosValue * dUndPriceMid;

			if(m_dOptDeltaEq <= BAD_DOUBLE_VALUE) m_dOptDeltaEq = 0.;
			m_dOptDeltaEq += dPosValue * dUndPriceMid;
		}
		else
			m_bBadDeltaEq = VARIANT_TRUE;
	}
	else
	{
		m_bBadNetDelta = VARIANT_TRUE;
		m_bBadOptDelta = VARIANT_TRUE;
		m_bBadOptDeltaEq = VARIANT_TRUE;
		m_bBadDeltaEq = VARIANT_TRUE;
	}

	// GammaInSharesPerc
	_CHK(spPos->get_GammaInSharesPerc(&dPosValue));
	if(dPosValue > BAD_DOUBLE_VALUE)
	{
		if(m_dGammaPerc <= BAD_DOUBLE_VALUE) m_dGammaPerc = 0.;
		m_dGammaPerc += dPosValue;
		if(dUndPriceMid > DBL_EPSILON)
		{
			if(m_dGammaEq <= BAD_DOUBLE_VALUE) m_dGammaEq = 0.;
			m_dGammaEq += dPosValue * dUndPriceMid;
		}
		else
			m_bBadGammaEq = VARIANT_TRUE;
	}
	else
	{
		m_bBadGammaPerc = VARIANT_TRUE;
		m_bBadGammaEq = VARIANT_TRUE;
	}

	// GammaInShares
	_CHK(spPos->get_GammaInShares(&dPosValue));
	if(dPosValue > BAD_DOUBLE_VALUE)
	{
		if(m_dGamma <= BAD_DOUBLE_VALUE) m_dGamma = 0.;
		m_dGamma += dPosValue;
	}
	else
		m_bBadGamma = VARIANT_TRUE;


	// Net Gamma
	_CHK(spPos->get_NetGamma(&dPosValue));
	if(dPosValue > BAD_DOUBLE_VALUE)
	{
		if(m_dNetGamma <= BAD_DOUBLE_VALUE) m_dNetGamma = 0.;
		m_dNetGamma += dPosValue;
	}
	else
		m_bBadNetGamma = VARIANT_TRUE;


	// VegaInShares
	_CHK(spPos->get_VegaInShares(&dPosValue));
	if(dPosValue > BAD_DOUBLE_VALUE)
	{
		if(m_dVega <= BAD_DOUBLE_VALUE) m_dVega = 0.;
		m_dVega += dPosValue;
	}
	else
		m_bBadVega = VARIANT_TRUE;

	// WtdVega
	_CHK(spPos->get_WtdVega(&dPosValue));
	if(dPosValue > BAD_DOUBLE_VALUE)
	{
		if(m_dWtdVega <= BAD_DOUBLE_VALUE) m_dWtdVega = 0.;
		m_dWtdVega += dPosValue;
	}
	else
		m_bBadWtdVega = VARIANT_TRUE;

	// ThetaInShares
	_CHK(spPos->get_ThetaInShares(&dPosValue));
	if(dPosValue > BAD_DOUBLE_VALUE)
	{
		if(m_dTheta <= BAD_DOUBLE_VALUE) m_dTheta = 0.;
		m_dTheta += dPosValue;
	}
	else
		m_bBadTheta = VARIANT_TRUE;

	// RhoInShares
	_CHK(spPos->get_RhoInShares(&dPosValue));
	if(dPosValue > BAD_DOUBLE_VALUE)
	{
		if(m_dRho <= BAD_DOUBLE_VALUE) m_dRho = 0.;
		m_dRho += dPosValue;
	}
	else
		m_bBadRho = VARIANT_TRUE;

	// ThetaDeltaInShares
	_CHK(spPos->get_ThetaDeltaInShares(&dPosValue));
	if(dPosValue > BAD_DOUBLE_VALUE)
	{
		if(m_dThetaDelta <= BAD_DOUBLE_VALUE) m_dThetaDelta = 0.;
		m_dThetaDelta += dPosValue;
	}
	else
		m_bBadThetaDelta = VARIANT_TRUE;

	// ThetaGammaInShares
	_CHK(spPos->get_ThetaGammaInShares(&dPosValue));
	if(dPosValue > BAD_DOUBLE_VALUE)
	{
		if(m_dThetaGamma <= BAD_DOUBLE_VALUE) m_dThetaGamma = 0.;
		m_dThetaGamma += dPosValue;
	}
	else
		m_bBadThetaGamma = VARIANT_TRUE;

	// VegaDeltaInShares
	_CHK(spPos->get_VegaDeltaInShares(&dPosValue));
	if(dPosValue > BAD_DOUBLE_VALUE)
	{
		if(m_dVegaDelta <= BAD_DOUBLE_VALUE) m_dVegaDelta = 0.;
		m_dVegaDelta += dPosValue;
	}
	else
		m_bBadVegaDelta = VARIANT_TRUE;

	// VegaGammaInShares
	_CHK(spPos->get_VegaGammaInShares(&dPosValue));
	if(dPosValue > BAD_DOUBLE_VALUE)
	{
		if(m_dVegaGamma <= BAD_DOUBLE_VALUE) m_dVegaGamma = 0.;
		m_dVegaGamma += dPosValue;
	}
	else
		m_bBadVegaGamma = VARIANT_TRUE;

	// TimeValue
	_CHK(spPos->get_TimeValue(&dPosValue));
	if(dPosValue > BAD_DOUBLE_VALUE)
	{
		if(m_dTimeValue <= BAD_DOUBLE_VALUE) m_dTimeValue = 0.;
		m_dTimeValue += dPosValue;
	}
	else
		m_bBadTimeValue = VARIANT_TRUE;

	VARIANT_BOOL bPosIsSynthetic = VARIANT_FALSE;
	_CHK(spPos->get_IsSynthetic(&bPosIsSynthetic));
	if(bPosIsSynthetic)
	{
		IMmRvSynthGreeksCollPtr spPosSynthGreeksColl;
		_CHK(spPos->get_SynthGreeks(&spPosSynthGreeksColl));
		
		if(spPosSynthGreeksColl != NULL)
		{
			IMmRvSynthGreeksAtomPtr spPosSynthGreeks;
			LONG nSynthUndID = 0L;
			DOUBLE dSelfValue = BAD_DOUBLE_VALUE;

			CComObject<CMmRvSynthGreeksColl>* pPosSynthGreeksColl = dynamic_cast<CComObject<CMmRvSynthGreeksColl>*>(spPosSynthGreeksColl.GetInterfacePtr());
			CMmRvSynthGreeksColl::EnumIterType itPosSynthGreeks	= pPosSynthGreeksColl->m_coll.begin();
			CMmRvSynthGreeksColl::EnumIterType itPosSynthGreeksEnd = pPosSynthGreeksColl->m_coll.end();

			for(; itPosSynthGreeks != itPosSynthGreeksEnd; ++itPosSynthGreeks)
			{
				IMmRvSynthGreeksAtomPtr spPosSynthGreeks = itPosSynthGreeks->second;

				if(spPosSynthGreeks != NULL)
				{
					_CHK(spPosSynthGreeks->get_SynthUndID(&nSynthUndID));
					IMmRvSynthGreeksAtomPtr spSynthGreeks;
					_CHK(m_spSynthGreeks->get_Item(nSynthUndID, &spSynthGreeks));

					if(spSynthGreeks != NULL)
					{
						// DeltaInShares
						_CHK(spPosSynthGreeks->get_DeltaInShares(&dPosValue));
						if(dPosValue > BAD_DOUBLE_VALUE)
						{
							_CHK(spSynthGreeks->get_DeltaInShares(&dSelfValue));
							if(dSelfValue <= BAD_DOUBLE_VALUE) dSelfValue = 0.;
							_CHK(spSynthGreeks->put_DeltaInShares(dSelfValue + dPosValue));
						}
						else
							_CHK(spSynthGreeks->put_BadDelta(VARIANT_TRUE));

						// GammaInSharesPerc
						_CHK(spPosSynthGreeks->get_GammaInSharesPerc(&dPosValue));
						if(dPosValue > BAD_DOUBLE_VALUE)
						{
							_CHK(spSynthGreeks->get_GammaInSharesPerc(&dSelfValue));
							if(dSelfValue <= BAD_DOUBLE_VALUE) dSelfValue = 0.;
							_CHK(spSynthGreeks->put_GammaInSharesPerc(dSelfValue + dPosValue));
						}
						else
							_CHK(spSynthGreeks->put_BadGammaPerc(VARIANT_TRUE));

						// GammaInShares
						_CHK(spPosSynthGreeks->get_GammaInShares(&dPosValue));
						if(dPosValue > BAD_DOUBLE_VALUE)
						{
							_CHK(spSynthGreeks->get_GammaInShares(&dSelfValue));
							if(dSelfValue <= BAD_DOUBLE_VALUE) dSelfValue = 0.;
							_CHK(spSynthGreeks->put_GammaInShares(dSelfValue + dPosValue));
						}
						else
							_CHK(spSynthGreeks->put_BadGamma(VARIANT_TRUE));

						// Net Gamma
						_CHK(spPosSynthGreeks->get_NetGamma(&dPosValue));
						if(dPosValue > BAD_DOUBLE_VALUE)
						{
							_CHK(spSynthGreeks->get_NetGamma(&dSelfValue));
							if(dSelfValue <= BAD_DOUBLE_VALUE) dSelfValue = 0.;
							_CHK(spSynthGreeks->put_NetGamma(dSelfValue + dPosValue));
						}
						else
							_CHK(spSynthGreeks->put_BadNetGamma(VARIANT_TRUE));

					}
				}
			}
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
//
void CMmRvUndAtom::_ClearSynthGreeks() throw()
{
	if(m_bHasSynthetic && m_spSynthGreeks != NULL)
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
}

/////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CMmRvUndAtom::CalcWtdVega(IEtsExpCalColl* aExpCalColl)
{
	if(aExpCalColl == NULL)
		return Error(L"Invalid objects passed.", IID_IMmRvUndAtom, E_INVALIDARG);

	try
	{
		m_bBadWtdVega = VARIANT_FALSE;
		m_dWtdVega = BAD_DOUBLE_VALUE;

		DATE dtToday;
		::GetNYDateTimeAsDATE(&dtToday);
		dtToday = floor(dtToday);
		IEtsExpCalCollPtr spExpCalColl(aExpCalColl);
				
		CComObject<CMmRvPosColl>* pPosColl = dynamic_cast<CComObject<CMmRvPosColl>*>(m_spPos.GetInterfacePtr());
		CMmRvPosColl::EnumIterType itPos	= pPosColl->m_coll.begin();
		CMmRvPosColl::EnumIterType itPosEnd = pPosColl->m_coll.end();

		for(; itPos != itPosEnd; ++itPos)
		{
			IMmRvPosAtomPtr spPos = itPos->second;
			if(spPos != NULL)
			{
				EtsContractTypeEnum enContractType = enCtNone;
				_CHK(spPos->get_ContractType(&enContractType));
				
				if(enContractType == enCtOption || enContractType == enCtFutOption)
				{
					DOUBLE dVegaWeight = BAD_DOUBLE_VALUE;
					DATE dtExpiryOV = 0.;

					if(enContractType == enCtOption)
					{
						_CHK(spPos->get_ExpiryOV(&dtExpiryOV));
						dVegaWeight = spExpCalColl->GetVegaWeight(dtExpiryOV);
					}
					else
					{
						IMmRvFutAtomPtr spFut;
						_CHK(spPos->get_Fut(&spFut));

						if(spFut != NULL)
						{
							_CHK(spFut->get_Maturity(&dtExpiryOV));
							dVegaWeight = spExpCalColl->GetVegaWeight(dtExpiryOV);
						}
					}

					_CHK(spPos->put_VegaWeight(dVegaWeight));

					VARIANT_BOOL bVisible = VARIANT_FALSE;
					_CHK(spPos->get_Visible(&bVisible));
					if(bVisible && floor(dtExpiryOV) >= dtToday && dVegaWeight > BAD_DOUBLE_VALUE)
					{
						DOUBLE dPosValue = BAD_DOUBLE_VALUE;
						// VegaInShares
						_CHK(spPos->get_VegaInShares(&dPosValue));
						if(dPosValue > BAD_DOUBLE_VALUE)
						{
							_CHK(spPos->put_WtdVega(dPosValue * dVegaWeight));

							if(m_dWtdVega <= BAD_DOUBLE_VALUE) m_dWtdVega = 0.;
							m_dWtdVega += dPosValue * dVegaWeight;
						}
						else
							m_bBadWtdVega = VARIANT_TRUE;
					}
				}
			}
		}
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_IMmRvUndAtom, e.Error());
	}
	_CATCH_UNHANDLED_EXCEPTION_RETURN_COM;
	return S_OK;
}

/////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CMmRvUndAtom::CalcSyntheticUnderlying(IMmRvUndColl* aUndColl,
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
													ICalculationParametrs* pParams)
{
	if(aUndColl == NULL)
		return Error(L"Invalid objects passed.", IID_IMmRvUndAtom, E_INVALIDARG);

	try
	{
		DOUBLE dUndPriceMid = 0.;
		if(m_spUndPriceProfile != NULL)
		{
			dUndPriceMid = m_spUndPriceProfile->GetUndPriceMid(m_dPriceBid, m_dPriceAsk,
				m_dPriceLast, dUndPriceTolerance, enPriceRoundingRule, NULL, VARIANT_FALSE);
		}

		IMmRvUndCollPtr spUndColl(aUndColl);
		IMmRvUndCollPtr spUndColl2;
		IMmRvUndAtomPtr spUnd, spUnd2;

		ICalculationParametrsPtr spParams(pParams);

		_CHK(spUndColl2.CreateInstance(CLSID_MmRvUndColl), _T("Fail to create underlings."));
		if(spUndColl2 == NULL)
			return Error(L"Fail to create underlings.", IID_IMmRvUndAtom, E_INVALIDARG);

		spUndColl2.AddRef();

		CComObject<CMmRvPosColl>* pPosColl = dynamic_cast<CComObject<CMmRvPosColl>*>(m_spSynthPos.GetInterfacePtr());
		CMmRvPosColl::EnumIterType itPos	= pPosColl->m_coll.begin();
		CMmRvPosColl::EnumIterType itPosEnd = pPosColl->m_coll.end();

		for(; itPos != itPosEnd; ++itPos)
		{
			IMmRvPosAtomPtr spPos = itPos->second;
			if(spPos != NULL)
			{
				EtsContractTypeEnum enContractType = enCtNone;
				_CHK(spPos->get_ContractType(&enContractType));

				if(enContractType == enCtOption)
				{
					LONG nUndID = 0L;
					_CHK(spPos->get_UndID(&nUndID));
					_CHK(spUndColl->get_Item(nUndID, &spUnd));

					if(spUnd != NULL)
					{
						_CHK(spUndColl2->get_Item(nUndID, &spUnd2));
						if(spUnd2 == NULL)
							_CHK(spUndColl2->Add(nUndID, L"", spUnd, NULL));

						_CHK(spUnd->ClearValues());
						_CHK(spPos->CalcOptionGreeks(spUndColl, this, dUndPriceMid,
							nMask, bIsPnlLTD, bFitToImpFlatNoBid, enFitToImpCurve, enCalcModel,
							bUseTheoVolatility, bUseTheoVolaNoBid, bUseTheoVolaBadMarket,
							dUndPriceTolerance, enPriceRoundingRule, NULL, FALSE, FALSE, spParams));
					}
				}
			}
		}

		CComObject<CMmRvUndColl>* pUndColl = dynamic_cast<CComObject<CMmRvUndColl>*>(spUndColl2.GetInterfacePtr());
		CMmRvUndColl::EnumIterType itUnd	= pUndColl->m_coll.begin();
		CMmRvUndColl::EnumIterType itUndEnd = pUndColl->m_coll.end();

		for(; itUnd != itUndEnd; ++itUnd)
		{
			IMmRvUndAtomPtr spUnd = itUnd->second;
			if(spUnd != NULL)
			{
				LONG nUndID = 0L;
				_CHK(spUnd->get_ID(&nUndID));
				if(nUndID != m_nID)
				{
					_CHK(spUnd->Calc(spUndColl,
									nMask,
									VARIANT_TRUE,
									VARIANT_TRUE,
									VARIANT_FALSE,
									VARIANT_TRUE,
									NULL,
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
									VARIANT_FALSE,
									VARIANT_FALSE, 
									VARIANT_FALSE,
									spParams));
				}
			}
		}

		spUndColl2 = NULL;
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_IMmRvUndAtom, e.Error());
	}
	_CATCH_UNHANDLED_EXCEPTION_RETURN_COM;
	return S_OK;
}

// Greeks view
HRESULT CMmRvUndAtom::CalcGroupingTotals(double dUndPriceMid)
{
	try
	{
		HRESULT hr;

		IUnknownPtr spUnk;
		_variant_t varItem;
		ULONG nFetched = 0L;
		IMmRvGroupingAtomPtr spExp;

		// walk through expiries
		__CHECK_HRESULT(m_spDataGrouping->get__NewEnum(&spUnk), _T("Fail to get expiry collection."));

		IEnumVARIANTPtr spExpEnum(spUnk);
		__CHECK_HRESULT(spExpEnum->Reset(), _T("Fail to reset expiry collection."));
		while((hr = spExpEnum->Next(1L, &varItem, &nFetched)) == S_OK)
		{
			ATLASSERT(varItem.vt == VT_DISPATCH);
			spExp = varItem;
			if(nFetched > 0 && spExp != NULL)
			{
				__CHECK_HRESULT3(spExp->ClearValues());

				IMmRvSynthGreeksCollPtr spExpSynthGreeks;
				IMmRvPosCollPtr spCollPos;
				IMmRvPosAtomPtr spPos;
				VARIANT_BOOL bExpHasSynth = VARIANT_FALSE;
				__CHECK_HRESULT3(spExp->get_HasSynth(&bExpHasSynth));
				__CHECK_HRESULT(spExp->get_SynthGreek(&spExpSynthGreeks), _T("Fail to get expiry synthetic greeks."));

				DOUBLE dTemp = 0., dTemp2 = 0.;
				LONG nTemp = 0;
				DOUBLE dExpDeltaInShares = BAD_DOUBLE_VALUE, dExpGammaInShares = BAD_DOUBLE_VALUE;
				DOUBLE dExpNetDeltaInShares = BAD_DOUBLE_VALUE, dExpNetDeltaEq = BAD_DOUBLE_VALUE;
				DOUBLE dExpDeltaEq = BAD_DOUBLE_VALUE, dExpGammaEq = BAD_DOUBLE_VALUE;
				DOUBLE dExpVegaInShares = BAD_DOUBLE_VALUE, dExpTimeValueInShares = BAD_DOUBLE_VALUE;
				VARIANT_BOOL bExpBadDeltaInShares = VARIANT_FALSE, bExpBadGammaInShares = VARIANT_FALSE;
				VARIANT_BOOL bExpBadNetDeltaInShares = VARIANT_FALSE, bExpBadNetDeltaEq = VARIANT_FALSE;
				VARIANT_BOOL bExpBadDeltaEq = VARIANT_FALSE, bExpBadGammaEq = VARIANT_FALSE;
				VARIANT_BOOL bExpBadVegaInShares = VARIANT_FALSE, bExpBadTimeValueInShares = VARIANT_FALSE;

				// clear expiry synthetic components values
				if(VARIANT_FALSE != bExpHasSynth && spExpSynthGreeks != NULL)
				{
					IMmRvSynthGreeksAtomPtr spExpSynthGreek;
					__CHECK_HRESULT(spExpSynthGreeks->get__NewEnum(&spUnk), _T("Fail to get expiry synthetic greeks."));

					IEnumVARIANTPtr spExpSynthGreeksEnum(spUnk);
					__CHECK_HRESULT(spExpSynthGreeksEnum->Reset(), _T("Fail to reset expiry synthetic greeks."));
					while((hr = spExpSynthGreeksEnum->Next(1L, &varItem, &nFetched)) == S_OK)
					{
						ATLASSERT(varItem.vt == VT_DISPATCH);
						spExpSynthGreek = varItem;
						if(nFetched > 0 && spExpSynthGreek != NULL)
						{
							__CHECK_HRESULT3(spExpSynthGreek->ClearValuesInShares());
						}
						spExpSynthGreek = NULL;
						varItem.Clear();
					}
					__CHECK_HRESULT(hr, _T("Fail to get next expiry synthetic greek."));
				}

				// walk through expiry positions
				__CHECK_HRESULT(spExp->get_Pos(&spCollPos), _T("Fail to get expiry positions."));
				__CHECK_HRESULT(spCollPos->get__NewEnum(&spUnk), _T("Fail to get expiry positions collection."));

				IEnumVARIANTPtr spPosEnum(spUnk);
				__CHECK_HRESULT(spPosEnum->Reset(), _T("Fail to reset expiry positions collection."));
				while((hr = spPosEnum->Next(1L, &varItem, &nFetched)) == S_OK)
				{
					ATLASSERT(varItem.vt == VT_DISPATCH);
					spPos = varItem;
					if(nFetched > 0 && spPos != NULL)
					{
						EtsContractTypeEnum	enContractType = enCtOption;
						VARIANT_BOOL bPosIsSynth = VARIANT_FALSE;
						IMmRvSynthGreeksCollPtr spPosSynthGreeks;

						__CHECK_HRESULT3(spPos->get_ContractType(&enContractType));
						__CHECK_HRESULT3(spPos->get_IsSynthetic(&bPosIsSynth));
						__CHECK_HRESULT(spPos->get_SynthGreeks(&spPosSynthGreeks), _T("Fail to get position synthetic greeks."));

						if(enContractType == enCtStock || enContractType == enCtIndex)
						{
							// NetDeltaInShares (underlying part)
							__CHECK_HRESULT3(spPos->get_QtyInShares(&nTemp));
							if(nTemp > BAD_LONG_VALUE)
							{
								if(dExpNetDeltaInShares <= BAD_DOUBLE_VALUE) dExpNetDeltaInShares = 0.;
								dExpNetDeltaInShares += nTemp;
							}
							else
							{
								bExpBadNetDeltaInShares = VARIANT_TRUE;
							}

							// NetDeltaEq (underlying part)
							__CHECK_HRESULT3(spPos->get_QtyInShares(&nTemp));
							if(nTemp > BAD_LONG_VALUE)
							{
								dTemp = nTemp * dUndPriceMid;
								if(dExpNetDeltaEq <= BAD_DOUBLE_VALUE) dExpNetDeltaEq = 0.;
								dExpNetDeltaEq += dTemp;
							}
							else
							{
								bExpBadNetDeltaEq = VARIANT_TRUE;
							}

							continue;
						}


						// DeltaInShares total
						__CHECK_HRESULT3(spPos->get_DeltaInShares(&dTemp));
						if(dTemp > BAD_DOUBLE_VALUE)
						{
							if(dExpDeltaInShares <= BAD_DOUBLE_VALUE) dExpDeltaInShares = 0.;
							dExpDeltaInShares += dTemp;
						}
						else
						{
							bExpBadDeltaInShares = VARIANT_TRUE;
						}

						// DeltaEq total
						__CHECK_HRESULT3(spPos->get_DeltaInShares(&dTemp));
						if(dTemp > BAD_DOUBLE_VALUE)
						{
							dTemp *= dUndPriceMid;
							if(dExpDeltaEq <= BAD_DOUBLE_VALUE) dExpDeltaEq = 0.;
							dExpDeltaEq += dTemp;
						}
						else
						{
							bExpBadDeltaEq = VARIANT_TRUE;
						}

						// GammaInShares total
						__CHECK_HRESULT3(spPos->get_GammaInShares(&dTemp));
						if(dTemp > BAD_DOUBLE_VALUE)
						{
							if(dExpGammaInShares <= BAD_DOUBLE_VALUE) dExpGammaInShares = 0.;
							dExpGammaInShares += dTemp;
						}
						else
						{
							bExpBadGammaInShares = VARIANT_TRUE;
						}

						// GammaEq total
						__CHECK_HRESULT3(spPos->get_NetGamma(&dTemp));
						if(dTemp > BAD_DOUBLE_VALUE)
						{
							if(dExpGammaEq <= BAD_DOUBLE_VALUE) dExpGammaEq = 0.;
							dExpGammaEq += dTemp;
						}
						else
						{
							bExpBadGammaEq = VARIANT_TRUE;
						}

						// VegaInShares total
						__CHECK_HRESULT3(spPos->get_VegaInShares(&dTemp));
						if(dTemp > BAD_DOUBLE_VALUE)
						{
							if(dExpVegaInShares <= BAD_DOUBLE_VALUE) dExpVegaInShares = 0.;
							dExpVegaInShares += dTemp;
						}
						else
						{
							bExpBadVegaInShares = VARIANT_TRUE;
						}

						// TimeValueInShares total
						__CHECK_HRESULT3(spPos->get_TimeValue(&dTemp));
						if(dTemp > BAD_DOUBLE_VALUE)
						{
							if(dExpTimeValueInShares <= BAD_DOUBLE_VALUE) dExpTimeValueInShares = 0.;
							dExpTimeValueInShares += dTemp;
						}
						else
						{
							bExpBadTimeValueInShares = VARIANT_TRUE;
						}

						// expiry synthetic components
						if(VARIANT_FALSE != bPosIsSynth && spPosSynthGreeks != NULL && spExpSynthGreeks != NULL)
						{
							IMmRvSynthGreeksAtomPtr spExpSynthGreek;
							IMmRvSynthGreeksAtomPtr spPosSynthGreek;
							__CHECK_HRESULT(spPosSynthGreeks->get__NewEnum(&spUnk), _T("Fail to get position synthetic greeks."));

							IEnumVARIANTPtr spPosSynthGreeksEnum(spUnk);
							__CHECK_HRESULT(spPosSynthGreeksEnum->Reset(), _T("Fail to reset position synthetic greeks."));
							while((hr = spPosSynthGreeksEnum->Next(1L, &varItem, &nFetched)) == S_OK)
							{
								ATLASSERT(varItem.vt == VT_DISPATCH);
								spPosSynthGreek = varItem;
								if(nFetched > 0 && spPosSynthGreek != NULL)
								{
									LONG nSynthUndID = 0L;
									__CHECK_HRESULT3(spPosSynthGreek->get_SynthUndID(&nSynthUndID));
									__CHECK_HRESULT(spExpSynthGreeks->get_Item(nSynthUndID, &spExpSynthGreek), _T("Fail to get expiry synthetic greeks."));

									if(spExpSynthGreek != NULL)
									{
										// DeltaInShares total
										__CHECK_HRESULT3(spPosSynthGreek->get_DeltaInShares(&dTemp));
										if(dTemp > BAD_DOUBLE_VALUE)
										{
											__CHECK_HRESULT3(spExpSynthGreek->get_DeltaInShares(&dTemp2));
											if(dTemp2 <= BAD_DOUBLE_VALUE) dTemp2 = 0.;
											__CHECK_HRESULT3(spExpSynthGreek->put_DeltaInShares(dTemp2 + dTemp));
										}
										else
										{
											__CHECK_HRESULT3(spExpSynthGreek->put_BadDelta(VARIANT_TRUE));
										}

										// DeltaEq total
										__CHECK_HRESULT3(spPosSynthGreek->get_DeltaInMoney(&dTemp));
										if(dTemp > BAD_DOUBLE_VALUE)
										{
											__CHECK_HRESULT3(spExpSynthGreek->get_DeltaInMoney(&dTemp2));
											if(dTemp2 <= BAD_DOUBLE_VALUE) dTemp2 = 0.;
											__CHECK_HRESULT3(spExpSynthGreek->put_DeltaInMoney(dTemp2 + dTemp));
										}
										else
										{
											__CHECK_HRESULT3(spExpSynthGreek->put_BadDeltaInMoney(VARIANT_TRUE));
										}

										// GammaInShares total
										__CHECK_HRESULT3(spPosSynthGreek->get_GammaInShares(&dTemp));
										if(dTemp > BAD_DOUBLE_VALUE)
										{
											__CHECK_HRESULT3(spExpSynthGreek->get_GammaInShares(&dTemp2));
											if(dTemp2 <= BAD_DOUBLE_VALUE) dTemp2 = 0.;
											__CHECK_HRESULT3(spExpSynthGreek->put_GammaInShares(dTemp2 + dTemp));
										}
										else
										{
											__CHECK_HRESULT3(spExpSynthGreek->put_BadGamma(VARIANT_TRUE));
										}

										// GammaEq total
										__CHECK_HRESULT3(spPosSynthGreek->get_NetGamma(&dTemp));
										if(dTemp > BAD_DOUBLE_VALUE)
										{
											__CHECK_HRESULT3(spExpSynthGreek->get_NetGamma(&dTemp2));
											if(dTemp2 <= BAD_DOUBLE_VALUE) dTemp2 = 0.;
											__CHECK_HRESULT3(spExpSynthGreek->put_NetGamma(dTemp2 + dTemp));
										}
										else
										{
											__CHECK_HRESULT3(spExpSynthGreek->put_BadNetGamma(VARIANT_TRUE));
										}
									}
								}
								spPosSynthGreek = NULL;
								varItem.Clear();
							}
							__CHECK_HRESULT(hr, _T("Fail to get next position synthetic greek."));
						}
					}
					spPos = NULL;
					varItem.Clear();
				}
				__CHECK_HRESULT(hr, _T("Fail to get next expiry position."));

				// DeltaInShares total
				__CHECK_HRESULT3(spExp->put_DeltaInShares(dExpDeltaInShares));
				__CHECK_HRESULT3(spExp->put_BadDeltaInShares(bExpBadDeltaInShares));
				
				// DeltaEq total
				__CHECK_HRESULT3(spExp->put_DeltaEq(dExpDeltaEq));
				__CHECK_HRESULT3(spExp->put_BadDeltaEq(bExpBadDeltaEq));

				// NetDeltaInShares total
				bExpBadNetDeltaInShares &= bExpBadDeltaInShares;
				if(!bExpBadDeltaInShares)
				{
					if(dExpNetDeltaInShares <= BAD_DOUBLE_VALUE) dExpNetDeltaInShares = 0.;
					dExpNetDeltaInShares += dExpDeltaInShares;
				}
				__CHECK_HRESULT3(spExp->put_NetDeltaInShares(dExpNetDeltaInShares));
				__CHECK_HRESULT3(spExp->put_BadNetDeltaInShares(bExpBadNetDeltaInShares));

				// NetDeltaEq total
				bExpBadNetDeltaEq &= bExpBadDeltaEq;
				if(!bExpBadDeltaEq)
				{
					if(dExpNetDeltaEq <= BAD_DOUBLE_VALUE) dExpNetDeltaEq = 0.;
					dExpNetDeltaEq += dExpDeltaEq;
				}
				__CHECK_HRESULT3(spExp->put_NetDeltaEq(dExpNetDeltaEq));
				__CHECK_HRESULT3(spExp->put_BadNetDeltaEq(bExpBadNetDeltaEq));
				
				// GammaInShares total
				__CHECK_HRESULT3(spExp->put_GammaInShares(dExpGammaInShares));
				__CHECK_HRESULT3(spExp->put_BadGammaInShares(bExpBadGammaInShares));
				
				// GammaEq total
				__CHECK_HRESULT3(spExp->put_GammaEq(dExpGammaEq));
				__CHECK_HRESULT3(spExp->put_BadGammaEq(bExpBadGammaEq));
				
				// VegaInShares total
				__CHECK_HRESULT3(spExp->put_VegaInShares(dExpVegaInShares));
				__CHECK_HRESULT3(spExp->put_BadVegaInShares(bExpBadVegaInShares));
				
				// TimeValueInShares total
				__CHECK_HRESULT3(spExp->put_TimeValueInShares(dExpTimeValueInShares));
				__CHECK_HRESULT3(spExp->put_BadTimeValueInShares(bExpBadTimeValueInShares));
			}
			spExp = NULL;
			varItem.Clear();
		}
		__CHECK_HRESULT(hr, _T("Fail to get next expiry."));
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)CComErrorWrapper::ErrorDescription(e), IID_IMmRvUndAtom, e.Error());
	}
	_CATCH_UNHANDLED_EXCEPTION_RETURN_COM;
	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
// IcvDataProvider 
//////////////////////////////////////////////////////////////////////////

vector<cvProvidedID>		CMmRvUndAtom::m_vecInternalVarID;
cvIDToIDMap					CMmRvUndAtom::m_mapSupportedCVID;
vector<cvProvidedID>		CMmRvUndAtom::m_vecInternalFuncID;
cvIDToIDMap					CMmRvUndAtom::m_mapSupportedFuncID;

STDMETHODIMP CMmRvUndAtom::raw_GetData(LONG lDataID, VARIANT* Value){
	int iLocalDataID = GetDataID( lDataID );
	VARIANT vtValue;

	if ( iLocalDataID == CV_DATAPROVIDER_BAD_ID ) 
		return E_FAIL;
		switch( iLocalDataID ) {
			case CV_DP_DATA_UNDBID :
				VariantInit( &vtValue );
				vtValue.vt = VT_R8;
				vtValue.dblVal = ( ( BAD_DOUBLE_VALUE == m_dPriceBid )  ? 0.0 : m_dPriceBid );
				VariantCopy( Value, &vtValue );
				break;
			case CV_DP_DATA_UNDASK :
				VariantInit( &vtValue );
				vtValue.vt = VT_R8;
				vtValue.dblVal = ( ( BAD_DOUBLE_VALUE == m_dPriceAsk)  ? 0.0 : m_dPriceAsk);
				VariantCopy( Value, &vtValue );
				break;
			case CV_DP_DATA_UNDLAST:
				VariantInit( &vtValue );
				vtValue.vt = VT_R8;
				vtValue.dblVal = ( ( BAD_DOUBLE_VALUE == m_dPriceLast)  ? 0.0 : m_dPriceLast);
				VariantCopy( Value, &vtValue );
				break;
			case CV_DP_DATA_UNDMARK:
				VariantInit( &vtValue );
				vtValue.vt = VT_R8;
				vtValue.dblVal = ( ( BAD_DOUBLE_VALUE == m_dPriceClose)  ? 0.0 : m_dPriceClose);
				VariantCopy( Value, &vtValue );
				break;
			case CV_DP_DATA_UNDBETA:
				VariantInit( &vtValue );
				vtValue.vt = VT_R8;
				vtValue.dblVal = ( ( BAD_DOUBLE_VALUE == m_dBeta)  ? 0.0 : m_dBeta);
				VariantCopy( Value, &vtValue );
				break;
			case CV_DP_DATA_UNDPNLMTM :
				VariantInit( &vtValue );
				vtValue.vt = VT_R8;
				vtValue.dblVal = ( ( BAD_DOUBLE_VALUE == m_dPnlMtm )  ? 0.0 : m_dPnlMtm );
				VariantCopy( Value, &vtValue );
				break;
			case CV_DP_DATA_UNDPNLTHEO:
				VariantInit( &vtValue );
				vtValue.vt = VT_R8;
				vtValue.dblVal = ( ( BAD_DOUBLE_VALUE == m_dPnlTheo)  ? 0.0 : m_dPnlTheo);
				VariantCopy( Value, &vtValue );
				break;
			case CV_DP_DATA_UNDPRICE:
				VariantInit( &vtValue );
				vtValue.vt = VT_R8;
				if ( m_simulationParams.m_dDate || m_simulationParams.m_dSpotChangePercent || m_simulationParams.m_dVolaChangePercent ) {
					DOUBLE dPrice = 0.;
					if (BAD_DOUBLE_VALUE != m_dPrice) {
						dPrice= m_dPrice+ ( m_dPrice / 100 * m_simulationParams.m_dSpotChangePercent);
						vtValue.dblVal = dPrice*100;
					}
					else
						dPrice= BAD_DOUBLE_VALUE;
					
				}
				vtValue.dblVal = ( ( BAD_DOUBLE_VALUE == m_dPrice)  ? 0.0 : m_dPrice);
				VariantCopy( Value, &vtValue );
				break;
			case CV_DP_DATA_UNDPOS:
				VariantInit( &vtValue );
				vtValue.vt = VT_R8;
				vtValue.dblVal = ( ( BAD_LONG_VALUE == m_nQty)  ? 0.0 : static_cast<double>(m_nQty) );
				VariantCopy( Value, &vtValue );
				break;
			case CV_DP_DATA_UNDOPTQTY:
				VariantInit( &vtValue );
				vtValue.vt = VT_R8;
				vtValue.dblVal = ( ( BAD_LONG_VALUE == m_nOptQty)  ? 0.0 : static_cast<double>(m_nOptQty) );
				VariantCopy( Value, &vtValue );
				break;
			case CV_DP_DATA_UNDTV:
				VariantInit( &vtValue );
				vtValue.vt = VT_R8;
				vtValue.dblVal = ( ( BAD_DOUBLE_VALUE == m_dTimeValue)  ? 0.0 : m_dTimeValue);
				VariantCopy( Value, &vtValue );
				break;
			case CV_DP_DATA_UNDTHETA:
				VariantInit( &vtValue );
				vtValue.vt = VT_R8;
				vtValue.dblVal = ( ( BAD_DOUBLE_VALUE == m_dTheta)  ? 0.0 : m_dTheta);
				VariantCopy( Value, &vtValue );
				break;
			case CV_DP_DATA_UNDTHETADELTA:
				VariantInit( &vtValue );
				vtValue.vt = VT_R8;
				vtValue.dblVal = ( ( BAD_DOUBLE_VALUE == m_dThetaDelta)  ? 0.0 : m_dThetaDelta);
				VariantCopy( Value, &vtValue );
				break;
			case CV_DP_DATA_UNDTHETAGAMMA:
				VariantInit( &vtValue );
				vtValue.vt = VT_R8;
				vtValue.dblVal = ( ( BAD_DOUBLE_VALUE == m_dThetaGamma)  ? 0.0 : m_dThetaGamma);
				VariantCopy( Value, &vtValue );
				break;
			case CV_DP_DATA_UNDVEGA:
				VariantInit( &vtValue );
				vtValue.vt = VT_R8;
				vtValue.dblVal = ( ( BAD_DOUBLE_VALUE == m_dVega)  ? 0.0 : m_dVega);
				VariantCopy( Value, &vtValue );
				break;
			case CV_DP_DATA_UNDVEGADELTA:
				VariantInit( &vtValue );
				vtValue.vt = VT_R8;
				vtValue.dblVal = ( ( BAD_DOUBLE_VALUE == m_dVegaDelta)  ? 0.0 : m_dVegaDelta);
				VariantCopy( Value, &vtValue );
				break;
			case CV_DP_DATA_UNDVEGAGAMMA:
				VariantInit( &vtValue );
				vtValue.vt = VT_R8;
				vtValue.dblVal = ( ( BAD_DOUBLE_VALUE == m_dVegaGamma)  ? 0.0 : m_dVegaGamma);
				VariantCopy( Value, &vtValue );
				break;
			case CV_DP_DATA_UNDWTDVEGA:
				VariantInit( &vtValue );
				vtValue.vt = VT_R8;
				vtValue.dblVal = ( ( BAD_DOUBLE_VALUE == m_dWtdVega)  ? 0.0 : m_dWtdVega);
				VariantCopy( Value, &vtValue );
				break;
			case CV_DP_DATA_UNDDIVS:
				{
/*					DOUBLE	dDividendsAmount = 0.0;
					DOUBLE	dDividendsDate = 0.0;
					if ( m_spDividend != NULL ) {
						try{
							m_spDividend->GetNearest( vt_date::GetCurrentDate(true), static_cast<DATE>(99999), &dDividendsAmount,&dDividendsDate);
						}
						catch (...) {
						}
						
					}-*/
					VariantInit( &vtValue );
					vtValue.vt = VT_R8;
					//vtValue.dblVal = ( ( BAD_DOUBLE_VALUE == dDividendsAmount)  ? 0.0 : dDividendsAmount);
					_GetDividendsForCV(vtValue);
					VariantCopy( Value, &vtValue );
				}
				break;
			case CV_DP_DATA_UNDDIVSDATE:
				{
					if ( !Value) 
						return E_POINTER;

					VariantInit( Value );
					Value->vt = VT_R8;
					_GetDividendsForCV(*Value, false);
				
					CComBSTR sbsDivsDate = "";
					if(Value->dblVal != BAD_DOUBLE_VALUE)
					{
						LONG nToday = static_cast<LONG>(m_simulationParams.m_dDate)!=0 ? static_cast<LONG>(m_simulationParams.m_dDate) : static_cast<LONG>(vt_date::GetCurrentDate(true));
						Value->dblVal = floor(Value->dblVal * OPM::cdDaysPerYear365 + nToday);

						vt_date DivsDate(Value->dblVal);

						wchar_t buff[5];
						swprintf_s(buff, L"%02u", DivsDate.get_month());
						sbsDivsDate = buff;
						sbsDivsDate += L"/";
						swprintf_s(buff, L"%02u", DivsDate.get_day());
						sbsDivsDate += buff;
						sbsDivsDate += L"/";
						swprintf_s(buff, L"%u", DivsDate.get_year());
						sbsDivsDate += buff;
					}

					VariantInit( Value );
					Value->vt = VT_BSTR;
					Value->bstrVal = sbsDivsDate.Copy();
					//VariantCopy(Value, &vtValue);
				}
				break;
			default:;
	}
	return S_OK;
}

void CMmRvUndAtom::_GetDividendsForCV(VARIANT& dividendsValue, bool bIsDivValue){
	LONG nDivCount = 0;

	DOUBLE dtNow;
	::GetNYDateTimeAsDATE(&dtNow);
	DOUBLE dtToday = m_simulationParams.m_dDate  ? m_simulationParams.m_dDate  : dtNow;
	CSafeArrayWrapper<double> saDates;
	CSafeArrayWrapper<double> saAmounts;

	/*if(!m_bHasSynthetic)
	{*/
	if(m_enContractType == enCtStock)
	{
		if (m_spDividend != NULL)
		{
			m_spDividend->GetDividendCount2(dtNow, 99999., 0.5, &nDivCount);
			if(nDivCount > 0L) 
			{
				LPSAFEARRAY psaAmounts = NULL;
				LPSAFEARRAY psaDates = NULL;

				m_spDividend->GetDividends2(dtNow, 99999., 0.5, nDivCount, &psaAmounts, &psaDates, &nDivCount);

				saAmounts.Attach(psaAmounts);
				saDates.Attach(psaDates);
			}
		}
	}
	else
	{
		VARIANT_BOOL bIsBasket = VARIANT_FALSE;

		if(m_spBasketIndex != NULL)
		{
			_CHK((m_spBasketIndex->get_IsBasket(&bIsBasket)));
			if(bIsBasket)
			{
				IEtsIndexDivCollPtr spBasketDivs;
				m_spBasketIndex->get_BasketDivs(&spBasketDivs);
				nDivCount = 0;
				LPSAFEARRAY psaAmounts = NULL;
				LPSAFEARRAY psaDates = NULL;

				spBasketDivs->GetDividends2(dtNow, 99999.0, 0.5, nDivCount, &psaAmounts, &psaDates, &nDivCount);

				saAmounts.Attach(psaAmounts);
				saDates.Attach(psaDates);
			}
		}
		/*if(bIsBasket)
		dYield = 0.0;
		else 
		_CHK(spUnd->get_Yield(&dYield));*/
	}
	if ( nDivCount ) {
		dividendsValue.dblVal = bIsDivValue ? *saAmounts.GetPlainData() : *saDates.GetPlainData();
	}
	else{
		dividendsValue.dblVal = BAD_DOUBLE_VALUE;
	}
	/*	}
	else if(spSynthRoot != NULL) // synthetic
	{
	VARIANT_BOOL bIsBasket = spSynthRoot->Basket;

	if(bIsBasket)
	{
	//dYield  = 0.0;
	IEtsIndexDivCollPtr spBasketDivs;
	LONG nBaskDivCount = 0L;
	spSynthRoot->get_BasketDivs(&spBasketDivs);
	spBasketDivs->get_Count(&nBaskDivCount);

	if((spBasketDivs != NULL)&&(nBaskDivCount > 0L))
	{
	nDivCount = 0;

	LPSAFEARRAY psaAmounts = NULL;
	LPSAFEARRAY psaDates = NULL;

	spBasketDivs->GetDividends(nToday, 99999, nDivCount, &psaAmounts, &psaDates, &nDivCount);

	saAmounts.Attach(psaAmounts);
	saDates.Attach(psaDates);
	}
	}
	else
	dYield = spSynthRoot->Yield;
	}*/
}

STDMETHODIMP CMmRvUndAtom::raw_CallFunction(LONG lFunctionID, SAFEARRAY** arrParameters, VARIANT *Value){
	return E_FAIL;
	/*
	int iLocalFuncID = GetFuncID( lFunctionID );
	long lLBound,lUBound;

	if ( iLocalFuncID == CV_DATAPROVIDER_BAD_ID ) 
		return E_FAIL;
	switch( iLocalFuncID ) {
		case CV_DP_FUNC_SQRT:
			{
				if ( (*arrParameters)->cDims != 1 )
					EgLib::CComErrorWrapper::ThrowError( E_FAIL, _T("Bad parameters count passed to system function") );
				lLBound = lUBound = 0;

				::SafeArrayGetUBound(*arrParameters, 1, &lUBound );
				::SafeArrayGetLBound(*arrParameters, 1, &lLBound );
				if ( lUBound != 1 && lLBound != 0 ) 
					EgLib::CComErrorWrapper::ThrowError( E_FAIL, _T("Bad parameters count passed to system function") );
				VARIANT *pvtParam;
				::SafeArrayAccessData( *arrParameters, (void**) &pvtParam );
				if ( pvtParam->vt != VT_R8 ) 
					EgLib::CComErrorWrapper::ThrowError( E_FAIL, _T("Parameter of wrong typed passed to system function") );
				double dParamValue = pvtParam->dblVal;
				::SafeArrayUnaccessData( *arrParameters );
				Value->vt = VT_R8;
				Value->dblVal = sqrt(dParamValue);
			}
			break;
		case CV_DP_FUNC_EXP:
			{
			}
			break;
		default:;
	}
	return S_OK;  */
}

STDMETHODIMP CMmRvUndAtom::raw_Check(SAFEARRAY **arrSysVars, SAFEARRAY **arrSysFuncs, LONG *pRetVal){
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
	if( pInfo ){
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

STDMETHODIMP CMmRvUndAtom::CalcCV(void)
{
	try
	{
		for(cvPosVectorIt it = m_vecPos.begin(); it != m_vecPos.end(); ++it)
			_CHK((*it)->CalcCV());

		if ( m_spRTContext != NULL && m_nPoolID != BAD_LONG_VALUE ) 
			_CHK(m_spRTContext->Recalculate(m_nPoolID));
	}
	catch(const _com_error& e)
	{
		m_vecPos.clear();
		return Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_IMmRvUndAtom, e.Error());
	}
	_CATCH_UNHANDLED_EXCEPTION_RETURN_COM;

	m_vecPos.clear();

	return S_OK;
}

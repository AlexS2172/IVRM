// MmRvGrpAtom.cpp : Implementation of CMmRvGrpAtom

#include "stdafx.h"
#include "MmRvGrpAtom.h"
#include "MmRvUndColl.h"
#include <math.h>

/////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CMmRvGrpAtom::Calc(IMmRvUndColl* aUndColl,
								IMmRvUndAtom* aIdx,
								VARIANT_BOOL  bTotals,
								LONG nMask,
								VARIANT_BOOL bCalcGreeks,
								VARIANT_BOOL bUpdateVola,
								VARIANT_BOOL bRealtimeCalc,
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
	if(aUndColl == NULL || aIdx == NULL || pParams == NULL)
		return Error(L"Invalid objects passed.", IID_IMmRvGrpAtom, E_INVALIDARG);

	try
	{
		__MmRvGrpAtom::ClearValues();

		IEtsProcessDelayPtr spDelay(aDelay);
		if(spDelay != NULL && spDelay->IsInterrupted)
			return S_OK;

		ICalculationParametrsPtr spParams(pParams);

		if(pnOptUpdated)
			*pnOptUpdated = 0L;

		if(pnUndUpdated)
			*pnUndUpdated = 0L;

		if(pnFutUpdated)
			*pnFutUpdated = 0L;

		DOUBLE dIdxPrice = 0.;
		IMmRvUndAtomPtr spIdx(aIdx);
		LONG nIdxID = 0L;
		_CHK(spIdx->get_ID(&nIdxID));
		
		if(nIdxID != 0L)
		{
			EtsReplacePriceStatusEnum enPriceStatusMid = enRpsNone;
			DOUBLE dPriceBid = 0., dPriceAsk = 0., dPriceLast = 0.;

			IEtsPriceProfileAtomPtr spUndPriceProfile;
			_CHK(spIdx->get_UndPriceProfile(&spUndPriceProfile));

			if(spUndPriceProfile != NULL)
			{
				_CHK(spIdx->get_PriceBid(&dPriceBid));
				_CHK(spIdx->get_PriceAsk(&dPriceAsk));
				_CHK(spIdx->get_PriceLast(&dPriceLast));

				dIdxPrice = spUndPriceProfile->GetUndPriceMid(dPriceBid, dPriceAsk,
					dPriceLast, dUndPriceTolerance, enPriceRoundingRule, &enPriceStatusMid, VARIANT_FALSE);
			}

			_CHK(spIdx->put_ReplacePriceStatus(enPriceStatusMid));
		}

		
		std::vector<IMmRvUndAtomPtr> vecUnds;

		IMmRvUndCollPtr spUndColl(aUndColl);
		CComObject<CMmRvUndColl>* pUndColl = dynamic_cast<CComObject<CMmRvUndColl>*>(spUndColl.GetInterfacePtr());
		CMmRvUndColl::EnumIterType itUnd	= pUndColl->m_coll.begin();
		CMmRvUndColl::EnumIterType itUndEnd = pUndColl->m_coll.end();

		for(; itUnd != itUndEnd; ++itUnd)
		{
			IMmRvUndAtomPtr spUnd = itUnd->second;
			
			if(spUnd != NULL)
			{
				vecUnds.push_back(spUnd);

				VARIANT_BOOL bIsDirty = VARIANT_FALSE;
				_CHK(spUnd->get_IsDirty(&bIsDirty));

				if(bTotals == VARIANT_FALSE && (bCalcOnlyDirty == VARIANT_FALSE || (bCalcOnlyDirty && bIsDirty)))
				{
					_CHK(spUnd->Calc(spUndColl,
						nMask,
						bCalcGreeks,
						bUpdateVola,
						bRealtimeCalc,
						VARIANT_TRUE,
						spDelay,
						bIsPnlLTD,
						bFitToImpFlatNoBid,
						enFitToImpCurve,
						enCalcModel,
						bUseTheoVolatility,
						bUseTheoVolaNoBid,
						bUseTheoVolaBadMarket,
						dUndPriceTolerance,
						enPriceRoundingRule,
						NULL/*aSimulatedUndColl*/, 
						bSimulate,
						VARIANT_FALSE,
						bCalcOnlyDirty,
						spParams,
						pnOptUpdated,
						pnUndUpdated,
						pnFutUpdated));
				}

				_CHK(spUnd->put_IsDirty(VARIANT_FALSE));

				DOUBLE dUndMidPrice = 0.;

				IEtsPriceProfileAtomPtr spUndPriceProfile;
				_CHK(spUnd->get_UndPriceProfile(&spUndPriceProfile));

				if(spUndPriceProfile != NULL)
				{
					DOUBLE dPriceBid = 0., dPriceAsk = 0., dPriceLast = 0.;
					_CHK(spUnd->get_PriceBid(&dPriceBid));
					_CHK(spUnd->get_PriceAsk(&dPriceAsk));
					_CHK(spUnd->get_PriceLast(&dPriceLast));

					dUndMidPrice = spUndPriceProfile->GetUndPriceMid(dPriceBid, dPriceAsk,
						dPriceLast, dUndPriceTolerance, enPriceRoundingRule, NULL, VARIANT_FALSE);
				}

				DOUBLE dUndValue = BAD_DOUBLE_VALUE;
				VARIANT_BOOL bBadValue = VARIANT_FALSE;
				// PnlMtm
				_CHK(spUnd->get_PnlMtm(&dUndValue));
				if(dUndValue > BAD_DOUBLE_VALUE)
				{
					if(m_dPnlMtm <= BAD_DOUBLE_VALUE) m_dPnlMtm = 0.;
					m_dPnlMtm += dUndValue;
				}
				_CHK(spUnd->get_BadPnlMtm(&bBadValue));
				if(bBadValue) m_bBadPnlMtm = VARIANT_TRUE;

				// PnlTheo
				_CHK(spUnd->get_PnlTheo(&dUndValue));
				if(dUndValue > BAD_DOUBLE_VALUE)
				{
					if(m_dPnlTheo <= BAD_DOUBLE_VALUE) m_dPnlTheo = 0.;
					m_dPnlTheo += dUndValue;
				}
				_CHK(spUnd->get_BadPnlTheo(&bBadValue));
				if(bBadValue) m_bBadPnlTheo = VARIANT_TRUE;

				// Vega
				_CHK(spUnd->get_Vega(&dUndValue));
				if(dUndValue > BAD_DOUBLE_VALUE)
				{
					if(m_dVega <= BAD_DOUBLE_VALUE) m_dVega = 0.;
					m_dVega += dUndValue;
				}
				_CHK(spUnd->get_BadVega(&bBadValue));
				if(bBadValue) m_bBadVega = VARIANT_TRUE;

				// WtdVega
				_CHK(spUnd->get_WtdVega(&dUndValue));
				if(dUndValue > BAD_DOUBLE_VALUE)
				{
					if(m_dWtdVega <= BAD_DOUBLE_VALUE) m_dWtdVega = 0.;
					m_dWtdVega += dUndValue;
				}
				_CHK(spUnd->get_BadWtdVega(&bBadValue));
				if(bBadValue) m_bBadWtdVega = VARIANT_TRUE;

				// Theta
				_CHK(spUnd->get_Theta(&dUndValue));
				if(dUndValue > BAD_DOUBLE_VALUE)
				{
					if(m_dTheta <= BAD_DOUBLE_VALUE) m_dTheta = 0.;
					m_dTheta += dUndValue;
				}
				_CHK(spUnd->get_BadTheta(&bBadValue));
				if(bBadValue) m_bBadTheta = VARIANT_TRUE;

				// DeltaEq
				_CHK(spUnd->get_DeltaEq(&dUndValue));
				if(dUndValue > BAD_DOUBLE_VALUE)
				{
					if(m_dDeltaEq <= BAD_DOUBLE_VALUE) m_dDeltaEq = 0.;
					m_dDeltaEq += dUndValue;
				}
				_CHK(spUnd->get_BadDeltaEq(&bBadValue));
				if(bBadValue) m_bBadDeltaEq = VARIANT_TRUE;

				// GammaEq
				_CHK(spUnd->get_GammaEq(&dUndValue));
				if(dUndValue > BAD_DOUBLE_VALUE)
				{
					if(m_dGammaEq <= BAD_DOUBLE_VALUE) m_dGammaEq = 0.;
					m_dGammaEq += dUndValue;
				}
				_CHK(spUnd->get_BadGammaEq(&bBadValue));
				if(bBadValue) m_bBadGammaEq = VARIANT_TRUE;

				// Rho
				_CHK(spUnd->get_Rho(&dUndValue));
				if(dUndValue > BAD_DOUBLE_VALUE)
				{
					if(m_dRho <= BAD_DOUBLE_VALUE) m_dRho = 0.;
					m_dRho += dUndValue;
				}
				_CHK(spUnd->get_BadRho(&bBadValue));
				if(bBadValue) m_bBadRho = VARIANT_TRUE;

				// OptDelta
				_CHK(spUnd->get_OptDelta(&dUndValue));
				if(dUndValue > BAD_DOUBLE_VALUE && dUndMidPrice > DBL_EPSILON)
				{
					if(m_dOptDelta <= BAD_DOUBLE_VALUE) m_dOptDelta = 0.;
					m_dOptDelta += dUndValue * dUndMidPrice;
				}
				_CHK(spUnd->get_BadOptDelta(&bBadValue));
				if(bBadValue) m_bBadOptDelta = VARIANT_TRUE;

				// NetDelta & BetaWtdDelta
				_CHK(spUnd->get_NetDelta(&dUndValue));
				if(dUndValue > BAD_DOUBLE_VALUE)
				{
					if(m_dNetDelta <= BAD_DOUBLE_VALUE) m_dNetDelta = 0.;
					m_dNetDelta += dUndValue;

					DOUBLE dBeta = BAD_DOUBLE_VALUE;
					_CHK(spUnd->get_Beta(&dBeta));
					if(dBeta > BAD_DOUBLE_VALUE && DoubleNEQZero(dBeta) && dUndMidPrice > DBL_EPSILON)
					{
						if(m_dBetaWtdDelta <= BAD_DOUBLE_VALUE) m_dBetaWtdDelta = 0.;
						m_dBetaWtdDelta += dUndValue * dBeta * dUndMidPrice;
					}
				}
				_CHK(spUnd->get_BadNetDelta(&bBadValue));
				if(bBadValue)
				{
					m_bBadNetDelta = VARIANT_TRUE;
					m_bBadBetaWtdDelta = VARIANT_TRUE;
				}

				// NetGamma
				_CHK(spUnd->get_Gamma(&dUndValue));
				if(dUndValue > BAD_DOUBLE_VALUE)
				{
					if(m_dNetGamma <= BAD_DOUBLE_VALUE) m_dNetGamma = 0.;
					m_dNetGamma += dUndValue;
				}
				_CHK(spUnd->get_BadGamma(&bBadValue));
				if(bBadValue) m_bBadNetGamma = VARIANT_TRUE;

				// BetaWtdDeltaEq
				_CHK(spUnd->get_BetaWtdDeltaEq(&dUndValue));
				if(dUndValue > BAD_DOUBLE_VALUE)
				{
					if(m_dBetaWtdDeltaEq <= BAD_DOUBLE_VALUE) m_dBetaWtdDeltaEq = 0.;
					m_dBetaWtdDeltaEq += dUndValue;
				}
				_CHK(spUnd->get_BadBetaWtdDeltaEq(&bBadValue));
				if(bBadValue && m_bBadBetaWtdDelta) m_bBadBetaWtdDeltaEq = VARIANT_TRUE;

				// TimeValue
				_CHK(spUnd->get_TimeValue(&dUndValue));
				if(dUndValue > BAD_DOUBLE_VALUE)
				{
					if(m_dTimeValue <= BAD_DOUBLE_VALUE) m_dTimeValue = 0.;
					m_dTimeValue += dUndValue;
				}
				_CHK(spUnd->get_BadTimeValue(&bBadValue));
				if(bBadValue) m_bBadTimeValue = VARIANT_TRUE;

				// DeltaInShares
				_CHK(spUnd->get_OptDelta(&dUndValue));
				if(dUndValue > BAD_DOUBLE_VALUE)
				{
					if(m_dDeltaInShares <= BAD_DOUBLE_VALUE) m_dDeltaInShares = 0.;
					m_dDeltaInShares += dUndValue;
				}
				_CHK(spUnd->get_BadOptDelta(&bBadValue));
				if(bBadValue) m_bBadDeltaInShares = VARIANT_TRUE;

				// synthetic greeks
				VARIANT_BOOL bHasSynthetic = VARIANT_FALSE;
				_CHK(spUnd->get_HasSynthetic(&bHasSynthetic));
				if(bHasSynthetic)
					_CalcUndSynthValues(spUndColl, spUnd, dIdxPrice, dUndPriceTolerance, enPriceRoundingRule);


			}
		}

		if(dIdxPrice > DBL_EPSILON)
		{
			if(m_dDeltaEq > BAD_DOUBLE_VALUE)
			{
				if(m_dIdxDeltaEq <= BAD_DOUBLE_VALUE) m_dIdxDeltaEq = 0.;
				m_dIdxDeltaEq = m_dDeltaEq / dIdxPrice;
			}
	        
			if(m_bBadDeltaEq) m_bBadIdxDeltaEq = VARIANT_TRUE;
	        
			if(m_dBetaWtdDeltaEq > BAD_DOUBLE_VALUE)
				m_dBetaWtdDelta = m_dBetaWtdDeltaEq / dIdxPrice;
			else
			{
				m_dBetaWtdDelta = BAD_DOUBLE_VALUE;
				m_bBadBetaWtdDelta = VARIANT_TRUE;
			}
		}
		else
		{
			m_dBetaWtdDelta = BAD_DOUBLE_VALUE;
			m_bBadBetaWtdDelta = VARIANT_TRUE;
			m_bBadIdxDeltaEq = VARIANT_TRUE;
		}
		if ( m_spRTContext != NULL && m_nPoolID != BAD_LONG_VALUE ) 
			_CHK(m_spRTContext->Recalculate(m_nPoolID));

		for(std::vector<IMmRvUndAtomPtr>::iterator it = vecUnds.begin(); it != vecUnds.end(); ++it)
			(*it)->CalcCV();

		// Greeks View
		if(m_enGroupingType != enGroupingNone)
			CalcGroupingTotals();

		//////////////////////////////////////////////////////////////////////////
		// simulation stuff

		if ( m_spSimulation != NULL &&  aSimulatedUndColl != NULL && bCalcSimulation ) {
			
			// copy group info into simulated one
			m_spSimulation->ClearValues();


			_CHK(m_spSimulation->Calc(aSimulatedUndColl, aIdx,bTotals,nMask, bCalcGreeks,
											bUpdateVola,bRealtimeCalc,aDelay,bIsPnlLTD,
											bFitToImpFlatNoBid, enFitToImpCurve,
											enCalcModel,bUseTheoVolatility, bUseTheoVolaNoBid,
											bUseTheoVolaBadMarket,dUndPriceTolerance,enPriceRoundingRule,
											NULL, VARIANT_TRUE, VARIANT_FALSE, bCalcOnlyDirty, pParams, pnOptUpdated, pnUndUpdated, pnFutUpdated));

		}


		//////////////////////////////////////////////////////////////////////////
		


	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_IMmRvGrpAtom, e.Error());
	}
	_CATCH_UNHANDLED_EXCEPTION_RETURN_COM;
	return S_OK;
}

/////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CMmRvGrpAtom::CalcWtdVega(IMmRvUndColl* aUndColl, IEtsExpCalColl* aExpCalColl)
{
	if(aUndColl == NULL || aExpCalColl == NULL)
		return Error(L"Invalid objects passed.", IID_IMmRvGrpAtom, E_INVALIDARG);

	try
	{
		m_bBadWtdVega = VARIANT_FALSE;
		m_dWtdVega = BAD_DOUBLE_VALUE;

		IEtsExpCalCollPtr spExpCalColl(aExpCalColl);

		IMmRvUndCollPtr spUndColl(aUndColl);
		CComObject<CMmRvUndColl>* pUndColl = dynamic_cast<CComObject<CMmRvUndColl>*>(spUndColl.GetInterfacePtr());
		CMmRvUndColl::EnumIterType itUnd	= pUndColl->m_coll.begin();
		CMmRvUndColl::EnumIterType itUndEnd = pUndColl->m_coll.end();

		for(; itUnd != itUndEnd; ++itUnd)
		{
			IMmRvUndAtomPtr spUnd = itUnd->second;
			
			if(spUnd != NULL)
			{
				_CHK(spUnd->CalcWtdVega(spExpCalColl));

				DOUBLE dUndValue = BAD_DOUBLE_VALUE;
				VARIANT_BOOL bBadValue = VARIANT_FALSE;
				// WtdVega
				_CHK(spUnd->get_WtdVega(&dUndValue));
				if(dUndValue > BAD_DOUBLE_VALUE)
				{
					if(m_dWtdVega <= BAD_DOUBLE_VALUE) m_dWtdVega = 0.;
					m_dWtdVega += dUndValue;
				}
				_CHK(spUnd->get_BadWtdVega(&bBadValue));
				if(bBadValue) m_bBadWtdVega = VARIANT_TRUE;
			}
		}
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_IMmRvGrpAtom, e.Error());
	}
	return S_OK;
}

/////////////////////////////////////////////////////////////////////////////
//
void CMmRvGrpAtom::_CalcUndSynthValues(IMmRvUndCollPtr spUndColl, IMmRvUndAtomPtr spUnd, DOUBLE dIdxPrice, 
				   DOUBLE dUndPriceTolerance, EtsPriceRoundingRuleEnum enPriceRoundingRule) throw()
{
	IMmRvSynthGreeksCollPtr spUndSynthGreeksColl;
	_CHK(spUnd->get_SynthGreeks(&spUndSynthGreeksColl));
	
	if(spUndSynthGreeksColl != NULL)
	{
		LONG nSynthUndID = 0L;
		DOUBLE dSelfValue = BAD_DOUBLE_VALUE;
		IMmRvUndAtomPtr spSynthUnd;

		CComObject<CMmRvSynthGreeksColl>* pUndSynthGreeksColl = dynamic_cast<CComObject<CMmRvSynthGreeksColl>*>(spUndSynthGreeksColl.GetInterfacePtr());
		CMmRvSynthGreeksColl::EnumIterType itUndSynthGreeks	= pUndSynthGreeksColl->m_coll.begin();
		CMmRvSynthGreeksColl::EnumIterType itUndSynthGreeksEnd = pUndSynthGreeksColl->m_coll.end();

		for(; itUndSynthGreeks != itUndSynthGreeksEnd; ++itUndSynthGreeks)
		{
			IMmRvSynthGreeksAtomPtr spUndSynthGreeks = itUndSynthGreeks->second;

			if(spUndSynthGreeks != NULL)
			{
				_CHK(spUndSynthGreeks->get_SynthUndID(&nSynthUndID));
				_CHK(spUndColl->get_Item(nSynthUndID, &spSynthUnd));

				if(spSynthUnd != NULL)
				{
					EtsReplacePriceStatusEnum enPriceStatusMid = enRpsNone;
					DOUBLE dSynthUndMidPrice = 0.;

					IEtsPriceProfileAtomPtr spUndPriceProfile;
					_CHK(spSynthUnd->get_UndPriceProfile(&spUndPriceProfile));

					if(spUndPriceProfile != NULL)
					{
						DOUBLE dPriceBid = 0., dPriceAsk = 0., dPriceLast = 0.;
						_CHK(spSynthUnd->get_PriceBid(&dPriceBid));
						_CHK(spSynthUnd->get_PriceAsk(&dPriceAsk));
						_CHK(spSynthUnd->get_PriceLast(&dPriceLast));

						dSynthUndMidPrice = spUndPriceProfile->GetUndPriceMid(dPriceBid, dPriceAsk,
							dPriceLast, dUndPriceTolerance, enPriceRoundingRule, &enPriceStatusMid, VARIANT_FALSE);
					}
					_CHK(spSynthUnd->put_ReplacePriceStatus(enPriceStatusMid));

					DOUBLE dUndValue = BAD_DOUBLE_VALUE;
					// DeltaInShares
					_CHK(spUndSynthGreeks->get_DeltaInShares(&dUndValue));
					if(dUndValue > BAD_DOUBLE_VALUE)
					{
						if(dSynthUndMidPrice > DBL_EPSILON)
						{
							if(m_dDeltaEq <= BAD_DOUBLE_VALUE) m_dDeltaEq = 0.;
							m_dDeltaEq += dUndValue * dSynthUndMidPrice;

							if(m_dOptDelta <= BAD_DOUBLE_VALUE) m_dOptDelta = 0.;
							m_dOptDelta += dUndValue * dSynthUndMidPrice;

							DOUBLE dDeltaInMoney = dUndValue * dSynthUndMidPrice;
							_CHK(spUndSynthGreeks->put_DeltaInMoney(dDeltaInMoney));

							DOUBLE dBeta = BAD_DOUBLE_VALUE, dBetaWtdDeltaInMoney = BAD_DOUBLE_VALUE;
							_CHK(spSynthUnd->get_Beta(&dBeta));

							if(nSynthUndID == 0L) // USD_ID
							{
								if(m_dBetaWtdDelta <= BAD_DOUBLE_VALUE) m_dBetaWtdDelta = 0.;
								m_dBetaWtdDelta += dUndValue;
								dBetaWtdDeltaInMoney = dDeltaInMoney;
							}
							else if(dBeta > BAD_DOUBLE_VALUE && DoubleNEQZero(dBeta) && dIdxPrice > DBL_EPSILON)
							{
								if(m_dBetaWtdDelta <= BAD_DOUBLE_VALUE) m_dBetaWtdDelta = 0.;
								m_dBetaWtdDelta += dUndValue * dBeta * dSynthUndMidPrice;
								dBetaWtdDeltaInMoney = dDeltaInMoney * dBeta;
							}
							else
							{
								m_bBadBetaWtdDelta = VARIANT_TRUE;
								_CHK(spUndSynthGreeks->put_BadBetaWtdDeltaInMoney(VARIANT_TRUE));
							}

							_CHK(spUndSynthGreeks->put_BetaWtdDeltaInMoney(dBetaWtdDeltaInMoney));

							if(dBetaWtdDeltaInMoney > BAD_DOUBLE_VALUE)
							{
								if(m_dBetaWtdDeltaEq <= BAD_DOUBLE_VALUE) m_dBetaWtdDeltaEq = 0.;
								m_dBetaWtdDeltaEq += dBetaWtdDeltaInMoney;
							}

							if(m_dNetDelta <= BAD_DOUBLE_VALUE) m_dNetDelta = 0.;
							m_dNetDelta += dUndValue;
							dUndValue = 0.0;


						}
						else
						{
							m_bBadDeltaEq = VARIANT_TRUE;
							m_bBadOptDelta = VARIANT_TRUE;
							m_bBadBetaWtdDelta = VARIANT_TRUE;
							_CHK(spUndSynthGreeks->put_BadDeltaInMoney(VARIANT_TRUE));
							_CHK(spUndSynthGreeks->put_BadBetaWtdDeltaInMoney(VARIANT_TRUE));

							if(m_dNetDelta <= BAD_DOUBLE_VALUE) m_dNetDelta = 0.;
							m_dNetDelta += dUndValue;
							dUndValue = 0.0;

						}
					}

					// GammaInSharesPerc
					_CHK(spUndSynthGreeks->get_GammaInSharesPerc(&dUndValue));
					if(dUndValue > BAD_DOUBLE_VALUE && dSynthUndMidPrice > DBL_EPSILON)
					{
						if(m_dGammaEq <= BAD_DOUBLE_VALUE) m_dGammaEq = 0.;
						m_dGammaEq += dUndValue * dSynthUndMidPrice;
					}
					else
						m_bBadGammaEq = VARIANT_TRUE;

					// GammaInShares
					_CHK(spUndSynthGreeks->get_GammaInShares(&dUndValue));
					if(dUndValue > BAD_DOUBLE_VALUE)
					{
						if(m_dNetGamma <= BAD_DOUBLE_VALUE) m_dNetGamma = 0.;
						m_dNetGamma += dUndValue;
					}
					else
						m_bBadNetGamma = VARIANT_TRUE;

					// DeltaInShares
					_CHK(spUndSynthGreeks->get_DeltaInShares(&dUndValue));
					if(dUndValue > BAD_DOUBLE_VALUE)
					{
						if(m_dDeltaInShares <= BAD_DOUBLE_VALUE) m_dDeltaInShares = 0.;
						m_dDeltaInShares += dUndValue;
					}
					else
						m_bBadDeltaInShares = VARIANT_TRUE;

					// BadDelta
					VARIANT_BOOL bUndValue = VARIANT_FALSE;
					_CHK(spUndSynthGreeks->get_BadDelta(&bUndValue));
					if(bUndValue)
					{
						m_bBadDeltaEq = VARIANT_TRUE;
						m_bBadOptDelta = VARIANT_TRUE;
						m_bBadBetaWtdDelta = VARIANT_TRUE;
						_CHK(spUndSynthGreeks->put_BadDeltaInMoney(VARIANT_TRUE));
						_CHK(spUndSynthGreeks->put_BadBetaWtdDeltaInMoney(VARIANT_TRUE));
					}

					// BadGammaPerc
					_CHK(spSynthUnd->get_BadGammaPerc(&bUndValue));
					if(bUndValue)
						m_bBadGammaEq = VARIANT_TRUE;

					// BadGamma
					_CHK(spSynthUnd->get_BadGamma(&bUndValue));
					if(bUndValue)
						m_bBadNetGamma = VARIANT_TRUE;
				}
			}
		}
	}

	if(dIdxPrice > DBL_EPSILON)
	{
		if(m_dDeltaEq > BAD_DOUBLE_VALUE)
		{
			if(m_dIdxDeltaEq <= BAD_DOUBLE_VALUE) m_dIdxDeltaEq = 0.;
			m_dIdxDeltaEq += m_dDeltaEq / dIdxPrice;
		}

		if(m_bBadDeltaEq)
			m_bBadIdxDeltaEq = VARIANT_TRUE;

		if(m_dBetaWtdDelta > BAD_DOUBLE_VALUE)
			m_dBetaWtdDelta /= dIdxPrice;
		else
			m_bBadBetaWtdDelta = VARIANT_TRUE;
	}
	else
	{
		m_dBetaWtdDelta = BAD_DOUBLE_VALUE;
		m_bBadBetaWtdDelta = VARIANT_TRUE;
		m_bBadIdxDeltaEq = VARIANT_TRUE;
	}
}

// Greeks view
HRESULT CMmRvGrpAtom::CalcGroupingTotals()
{
	try
	{
		HRESULT hr;

		IUnknownPtr spUnk;
		_variant_t varItem;
		ULONG nFetched = 0L;
		IMmRvGroupingTotalAtomPtr spExpTotal;

		__CHECK_HRESULT(m_spDataGrouping->get__NewEnum(&spUnk), _T("Fail to get expiry totals collection."));

		IEnumVARIANTPtr spExpTotalEnum(spUnk);
		__CHECK_HRESULT(spExpTotalEnum->Reset(), _T("Fail to reset expiry totals collection."));
		while((hr = spExpTotalEnum->Next(1L, &varItem, &nFetched)) == S_OK)
		{
			ATLASSERT(varItem.vt == VT_DISPATCH);
			spExpTotal = varItem;
			if(nFetched > 0 && spExpTotal != NULL)
			{
				__CHECK_HRESULT(spExpTotal->CalcTotals(), _T("Fail to calculate expiry totals."));
			}
			spExpTotal = NULL;
			varItem.Clear();
		}
		__CHECK_HRESULT(hr, _T("Fail to get next expiry total."));
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)CComErrorWrapper::ErrorDescription(e), IID_IMmRvGrpAtom, e.Error());
	}

	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
// IcvDataProvider 
//////////////////////////////////////////////////////////////////////////

vector<cvProvidedID>		CMmRvGrpAtom::m_vecInternalVarID;
cvIDToIDMap					CMmRvGrpAtom::m_mapSupportedCVID;
vector<cvProvidedID>		CMmRvGrpAtom::m_vecInternalFuncID;
cvIDToIDMap					CMmRvGrpAtom::m_mapSupportedFuncID;

STDMETHODIMP CMmRvGrpAtom::raw_GetData(LONG lDataID, VARIANT* Value){
	int iLocalDataID = GetDataID( lDataID );
	VARIANT vtValue;

	if ( iLocalDataID == CV_DATAPROVIDER_BAD_ID ) 
		return E_FAIL;
	switch( iLocalDataID ) {
			case CV_DP_DATA_GRPPNLMTM :
				VariantInit( &vtValue );
				vtValue.vt = VT_R8;
				vtValue.dblVal = ( ( BAD_DOUBLE_VALUE == m_dPnlMtm )  ? 0.0 : m_dPnlMtm );
				VariantCopy( Value, &vtValue );
				break;
			case CV_DP_DATA_GRPPNLTHEO:
				VariantInit( &vtValue );
				vtValue.vt = VT_R8;
				vtValue.dblVal = ( ( BAD_DOUBLE_VALUE == m_dPnlTheo)  ? 0.0 : m_dPnlTheo);
				VariantCopy( Value, &vtValue );
				break;
			case CV_DP_DATA_GRPSTRATEGY:
				if ( !Value )
					return E_POINTER;

				VariantInit( Value );
				Value->vt = VT_BSTR;
				Value->bstrVal = ::SysAllocString(m_bstrName.m_str );
				//VariantCopy( Value, &vtValue );
				break;
			case CV_DP_DATA_GRPNETDELTA:
				VariantInit( &vtValue );
				vtValue.vt = VT_R8;
				vtValue.dblVal = ( ( BAD_DOUBLE_VALUE == m_dNetDelta)  ? 0.0 : m_dNetDelta);
				VariantCopy( Value, &vtValue );
				break;
			case CV_DP_DATA_GRPNETDELTA$:
				VariantInit( &vtValue );
				vtValue.vt = VT_R8;
				vtValue.dblVal = ( ( BAD_DOUBLE_VALUE == m_dDeltaEq)  ? 0.0 : m_dDeltaEq);
				VariantCopy( Value, &vtValue );
				break;
			case CV_DP_DATA_GRPVEGA:
				VariantInit( &vtValue );
				vtValue.vt = VT_R8;
				vtValue.dblVal = ( ( BAD_DOUBLE_VALUE == m_dVega)  ? 0.0 : m_dVega);
				VariantCopy( Value, &vtValue );
				break;
			case CV_DP_DATA_GRPNETGAMMA$:
				VariantInit( &vtValue );
				vtValue.vt = VT_R8;
				vtValue.dblVal = ( ( BAD_DOUBLE_VALUE == m_dNetGamma)  ? 0.0 : m_dNetGamma);
				VariantCopy( Value, &vtValue );
				break;
			case CV_DP_DATA_GRPNETGAMMA$1:
				VariantInit( &vtValue );
				vtValue.vt = VT_R8;
				vtValue.dblVal = ( ( BAD_DOUBLE_VALUE == m_dGammaEq)  ? 0.0 : m_dGammaEq);
				VariantCopy( Value, &vtValue );
				break;
			case CV_DP_DATA_GRPOPTDELTA$:
				VariantInit( &vtValue );
				vtValue.vt = VT_R8;
				vtValue.dblVal = ( ( BAD_DOUBLE_VALUE == m_dOptDelta)  ? 0.0 : m_dOptDelta);
				VariantCopy( Value, &vtValue );
				break;
			case CV_DP_DATA_GRPWTDVEGA:
				VariantInit( &vtValue );
				vtValue.vt = VT_R8;
				vtValue.dblVal = ( ( BAD_DOUBLE_VALUE == m_dVega)  ? 0.0 : m_dVega);
				VariantCopy( Value, &vtValue );
				break;
			case CV_DP_DATA_GRPTHETA:
				VariantInit( &vtValue );
				vtValue.vt = VT_R8;
				vtValue.dblVal = ( ( BAD_DOUBLE_VALUE == m_dTheta)  ? 0.0 : m_dTheta);
				VariantCopy( Value, &vtValue );
				break;
			case CV_DP_DATA_GRPRHO:
				VariantInit( &vtValue );
				vtValue.vt = VT_R8;
				vtValue.dblVal = ( ( BAD_DOUBLE_VALUE == m_dRho)  ? 0.0 : m_dRho);
				VariantCopy( Value, &vtValue );
				break;
			case CV_DP_DATA_GRPINDXDELTAEQ:
				VariantInit( &vtValue );
				vtValue.vt = VT_R8;
				vtValue.dblVal = ( ( BAD_DOUBLE_VALUE == m_dIdxDeltaEq)  ? 0.0 : m_dIdxDeltaEq);
				VariantCopy( Value, &vtValue );
				break;
			case CV_DP_DATA_GRPBETAWTDDELTA$:
				VariantInit( &vtValue );
				vtValue.vt = VT_R8;
				vtValue.dblVal = ( ( BAD_DOUBLE_VALUE == m_dBetaWtdDeltaEq)  ? 0.0 : m_dBetaWtdDeltaEq);
				VariantCopy( Value, &vtValue );
				break;
			default:;
	}
	return S_OK;
}

STDMETHODIMP CMmRvGrpAtom::raw_CallFunction(LONG lFunctionID, SAFEARRAY** arrParameters, VARIANT *Value){
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
		case CV_DP_FUNC_ABS:
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
				Value->dblVal = abs(dParamValue);
			}
			break;
		case CV_DP_FUNC_EXP:
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
				try{
					Value->dblVal = exp(dParamValue);
				}
				catch(...){
					Value->dblVal = 0;
				}
			}
			break;
		case CV_DP_FUNC_INT:
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
				try{
					double v1 = floor( abs(dParamValue) );
					double frac = abs(dParamValue ) - v1;
					if(dParamValue > 0 ){
						v1 += ( frac > 0.5 ? 1. : 0 );
					}
					else{
						v1 -= ( frac > 0.5 ? 1. : 0 );
					}
					Value->dblVal = v1;
				}
				catch(...){
					Value->dblVal = 0;
				}
			}
			break;
		case CV_DP_FUNC_LOG10:
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
				try{
					Value->dblVal = log10(dParamValue);
				}
				catch(...){
					Value->dblVal = 0;
				}
			}
			break;
		case CV_DP_FUNC_LOG:
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
				try{
					Value->dblVal = log(dParamValue);
				}
				catch(...){
					Value->dblVal = 0;
				}
			}
			break;
		default:;
	}
	return S_OK;
}

STDMETHODIMP CMmRvGrpAtom::raw_Check(SAFEARRAY **arrSysVars, SAFEARRAY **arrSysFuncs, LONG *pRetVal){
	long lUBound;
	long lLBound;
	cvInfo *pInfo = NULL;

	lUBound = lLBound = 0;
	SafeArrayGetUBound( *arrSysVars, 1, &lUBound );
	SafeArrayGetLBound( *arrSysVars, 1, &lLBound );
	SafeArrayAccessData( *arrSysVars, (void**)&pInfo);
	size_t UnsupportedCount = lUBound -lLBound + 1;
	if( pInfo ){
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

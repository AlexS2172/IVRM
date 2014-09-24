// MmRvGrpAtom.cpp : Implementation of CMmRvGrpAtom

#include "stdafx.h"
#include "MmRvGrpAtom.h"
#include "MmRvUndColl.h"

/////////////////////////////////////////////////////////////////////////////
//
HRESULT CMmRvGrpAtom::CalcGroup( CMmRvUndColl* pUndColl,
										  CMmRvUndAtom* pIdx,
										  VARIANT_BOOL  bTotals,
										  LONG nMask,
										  VARIANT_BOOL bCalcGreeks,
										  VARIANT_BOOL bUpdateVola,
										  VARIANT_BOOL bRealtimeCalc,
										  IEtsProcessDelay* aDelay,
										  VARIANT_BOOL bIsPnlLTD,
										  EtsCalcModelTypeEnum enCalcModel,
										  VARIANT_BOOL bUseTheoVolatility,
										  VARIANT_BOOL bUseTheoVolaNoBid,
										  VARIANT_BOOL bUseTheoVolaBadMarket,
										  DOUBLE dUndPriceTolerance,
										  EtsPriceRoundingRuleEnum enPriceRoundingRule,
										  LONG* pnOptUpdated,
										  LONG* pnUndUpdated,
										  LONG* pnFutUpdated,
										  DATE dtCalcDate,
										  ICalculationParametrs* pParams,
										  IMarketSimulationScenarioPtr SimulationScenario)
{
	if(pUndColl == NULL || pIdx == NULL)
		return Error(L"Invalid objects passed.", IID_IMmRvGrpAtom, E_INVALIDARG);

	try
	{
		__MmRvGrpAtom::ClearValues();

		IEtsProcessDelayPtr spDelay(aDelay);
		/*if(spDelay != NULL && spDelay->IsInterrupted)
			return S_OK;*/

		ICalculationParametrsPtr spParams(pParams);

		if(pnOptUpdated)
			*pnOptUpdated = 0L;

		if(pnUndUpdated)
			*pnUndUpdated = 0L;

		if(pnFutUpdated)
			*pnFutUpdated = 0L;

		IUnknownPtr spUnk;
		_variant_t varItem;
		ULONG nFetched = 0L;

		DOUBLE		  dIdxPrice = 0.;
		LONG nIdxID = pIdx->m_nID;
		DOUBLE activeFuturePrice = 0.;

		if(nIdxID != 0L){
			EtsReplacePriceStatusEnum enPriceStatusMid = enRpsNone;
			VARIANT_BOOL	bDriverUsed = VARIANT_FALSE;
			pIdx->UpdateQuotes();
			pIdx->GetUnderlyingPrice(dUndPriceTolerance, enPriceRoundingRule, &enPriceStatusMid, &bDriverUsed, &dIdxPrice);
		}

		CMmRvUndColl::EnumCollType::iterator itUnd    =  pUndColl->m_coll.begin();
		CMmRvUndColl::EnumCollType::iterator itUndEnd =  pUndColl->m_coll.end();

		for(; itUnd != itUndEnd; ++itUnd)
		{
			CMmRvUndAtom* pUnd = dynamic_cast<CMmRvUndAtom*>(itUnd->second);
			if(pUnd != NULL)
			{
				
				m_vecUnds.push_back(itUnd->second);

				pUnd->UpdateQuotes();
				if(bTotals == VARIANT_FALSE)
				{
					_CHK(pUnd->CalcEx(	pUndColl,
										nMask,
										bCalcGreeks,
										bUpdateVola,
										bRealtimeCalc,
										VARIANT_TRUE,
										spDelay,
										bIsPnlLTD,
										enCalcModel,
										bUseTheoVolatility,
										bUseTheoVolaNoBid,
										bUseTheoVolaBadMarket,
										dUndPriceTolerance,
										enPriceRoundingRule,
										pnOptUpdated,
										pnUndUpdated,
										pnFutUpdated,
										dtCalcDate,
										spParams,
										SimulationScenario));
				}

				DOUBLE dUndMidPrice = 0.;

				IEtsPriceProfileAtomPtr spUndPriceProfile = pUnd->m_spUndPriceProfile;

				if(spUndPriceProfile != NULL)
				{
					VARIANT_BOOL	futureUsed = VARIANT_FALSE;
					EtsReplacePriceStatusEnum	dummy;
					_CHK(pUnd->GetUnderlyingPrice(dUndPriceTolerance, enPriceRoundingRule, &dummy ,&futureUsed, &dUndMidPrice) );
				}

				//asset group component computation coefficient
				DOUBLE	dCoeff = 1.0;
				if (pUnd->m_bIsHead == VARIANT_TRUE || pUnd->m_spHeadComponent)
					dCoeff	=	pUnd->m_dCoeff;

				// PnlMtm
				if(pUnd->PnlMTM_ > BAD_DOUBLE_VALUE)
				{
					if(m_dPnlMtm <= BAD_DOUBLE_VALUE) m_dPnlMtm = 0.;
					m_dPnlMtm += pUnd->PnlMTM_;
				}
				if(VARIANT_FALSE != pUnd->BadPnlMtm_) m_bBadPnlMtm = VARIANT_TRUE;

				// PnlTheo
				if(pUnd->PnLTheo_ > BAD_DOUBLE_VALUE)
				{
					if(m_dPnlTheo <= BAD_DOUBLE_VALUE) m_dPnlTheo = 0.;
					m_dPnlTheo += pUnd->PnLTheo_;
				}
				if(VARIANT_FALSE != pUnd->BadPnlTheo_) m_bBadPnlTheo = VARIANT_TRUE;

				// Vega
				if(pUnd->Vga$_ > BAD_DOUBLE_VALUE)
				{
					if(m_dVega <= BAD_DOUBLE_VALUE) m_dVega = 0.;
					m_dVega += pUnd->Vga$_;
				}
				if(VARIANT_FALSE != pUnd->BadVga$_) m_bBadVega = VARIANT_TRUE;

				// WtdVega
				if(pUnd->WtdVga$_ > BAD_DOUBLE_VALUE)
				{
					if(m_dWtdVega <= BAD_DOUBLE_VALUE) m_dWtdVega = 0.;
					m_dWtdVega += pUnd->WtdVga$_;
				}
				if(VARIANT_FALSE != pUnd->BadWtdVga$_) m_bBadWtdVega = VARIANT_TRUE;

				// Theta
				if(pUnd->Tht$_ > BAD_DOUBLE_VALUE)
				{
					if(m_dTheta <= BAD_DOUBLE_VALUE) m_dTheta = 0.;
					m_dTheta += pUnd->Tht$_;
				}
				if(VARIANT_FALSE != pUnd->BadTht$_) m_bBadTheta = VARIANT_TRUE;

				// DeltaEq
				if(pUnd->NetDlt$_ > BAD_DOUBLE_VALUE)
				{
					if(m_dDeltaEq <= BAD_DOUBLE_VALUE) m_dDeltaEq = 0.;
					m_dDeltaEq += pUnd->NetDlt$_ /** dCoeff*/;
				}

				if (pUnd->OptDlt$_ > BAD_DOUBLE_VALUE){
					if (m_dOptDeltaEq <= BAD_DOUBLE_VALUE) m_dOptDeltaEq = 0.;

					m_dOptDeltaEq += pUnd->OptDlt$_;
				};
				if (pUnd->BadOptDlt$_ == VARIANT_TRUE) m_bBadOptDeltaEq = VARIANT_TRUE;

				if (pUnd->TmValInShares_ > BAD_DOUBLE_VALUE){
					if (m_dTimeValue <= BAD_DOUBLE_VALUE) m_dTimeValue = 0.;

					m_dTimeValue += pUnd->TmValInShares_;
				};
				if (pUnd->BadTmValInShares_ == VARIANT_TRUE) m_bBadTimeValue = VARIANT_TRUE;

				
				if(pUnd->NetExposureSort_ > BAD_DOUBLE_VALUE)
				{
					if(m_dNetExposureShort ==BAD_DOUBLE_VALUE) m_dNetExposureShort = 0.0;
					m_dNetExposureShort += pUnd->NetExposureSort_ ; 
				}
				if(pUnd->NetExposureLong_ > BAD_DOUBLE_VALUE)
				{
					if(m_dNetExposureLong ==BAD_DOUBLE_VALUE) m_dNetExposureLong = 0.0;
					m_dNetExposureLong += pUnd->NetExposureLong_ ; 
				}

				if(pUnd->BadNetExposure_!=VARIANT_FALSE)
					m_bBadNetExposure = VARIANT_TRUE;

				if(VARIANT_FALSE != pUnd->BadNetDlt$_) m_bBadDeltaEq = VARIANT_TRUE;

				// m_dNetGammaEq
				if(pUnd->NetGma$_ > BAD_DOUBLE_VALUE)
				{
					if(m_dNetGammaEq <= BAD_DOUBLE_VALUE) m_dNetGammaEq = 0.;
					m_dNetGammaEq += pUnd->NetGma$_ /** dCoeff * dCoeff*/;
				}
				if(VARIANT_FALSE != pUnd->BadNetGma$_) m_bBadNetGammaEq = VARIANT_TRUE;

				// Rho
				if(pUnd->Rho$_ > BAD_DOUBLE_VALUE)
				{
					if(m_dRho <= BAD_DOUBLE_VALUE) m_dRho = 0.;
					m_dRho += pUnd->Rho$_;
				}
				if(VARIANT_FALSE != pUnd->BadRho$_) m_bBadRho = VARIANT_TRUE;

				// OptDelta
				if(pUnd->OptDlt_ > BAD_DOUBLE_VALUE)
				{
					if(m_dOptDelta <= BAD_DOUBLE_VALUE) m_dOptDelta = 0.;

					m_dOptDelta += pUnd->OptDlt_ /** dCoeff*/;
				}
				if(VARIANT_FALSE != pUnd->BadOptDelta_) m_bBadOptDelta = VARIANT_TRUE;

				// NetDelta & BetaWtdDelta
				if(pUnd->NetDlt_ > BAD_DOUBLE_VALUE)
				{
					if(m_dNetDelta <= BAD_DOUBLE_VALUE) m_dNetDelta = 0.;
					m_dNetDelta += pUnd->NetDlt_ * dCoeff;

					DOUBLE dBeta = pUnd->Beta_;
					if (pUnd->m_spHeadComponent)
						pUnd->m_spHeadComponent->get_Beta(&dBeta);

					if(dBeta > BAD_DOUBLE_VALUE && DoubleNEQZero(dBeta) && dUndMidPrice > DBL_EPSILON)
					{
						if(m_dBetaWtdDelta <= BAD_DOUBLE_VALUE) m_dBetaWtdDelta = 0.;
						m_dBetaWtdDelta += pUnd->NetDlt_ * dBeta * dUndMidPrice;
					}
				}
				if(VARIANT_FALSE != pUnd->BadNetDelta_ )
				{
					m_bBadNetDelta = VARIANT_TRUE;
					m_bBadBetaWtdDelta = VARIANT_TRUE;
				}

				// NetGamma
				if(pUnd->Gma1$_ > BAD_DOUBLE_VALUE)
				{
					if(m_dNetGamma <= BAD_DOUBLE_VALUE) m_dNetGamma = 0.;
					m_dNetGamma += pUnd->Gma1$_ * dCoeff /** dCoeff*/;
				}
				if(VARIANT_FALSE != pUnd->BadGma1$_) m_bBadNetGamma = VARIANT_TRUE;

				// BetaWtdDeltaEq
				if(pUnd->BetaWtdDelta$_ > BAD_DOUBLE_VALUE)
				{
					if(m_dBetaWtdDeltaEq <= BAD_DOUBLE_VALUE) m_dBetaWtdDeltaEq = 0.;
					m_dBetaWtdDeltaEq += pUnd->BetaWtdDelta$_;
				}
				if(VARIANT_FALSE != pUnd->BadBetaWtdDelta$_) m_bBadBetaWtdDeltaEq = VARIANT_TRUE;

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
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_IMmRvGrpAtom, e.Error());
	}
	return S_OK;
}


HRESULT CMmRvGrpAtom::CalcCV(){
	try {
		if ( m_spRTContext != NULL && m_nPoolID != BAD_LONG_VALUE ) 
			_CHK(m_spRTContext->Recalculate(m_nPoolID));

		for(std::vector<IMmRvUndAtomPtr>::iterator it = m_vecUnds.begin(); it != m_vecUnds.end(); ++it)
			(*it)->CalcCV();
		
		m_vecUnds.clear();
	}
	catch (const _com_error& e){
		m_vecUnds.clear();
		return Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_IMmRvGrpAtom, e.Error());
	};
	return S_OK;
};
/////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CMmRvGrpAtom::CalcWtdVega(IMmRvUndColl* aUndColl, IEtsExpCalColl* aExpCalColl)
{
	HRESULT hr = S_OK;

	if(aUndColl == NULL || aExpCalColl == NULL)
		return Error(L"Invalid objects passed.", IID_IMmRvGrpAtom, E_INVALIDARG);

	try
	{
		m_bBadWtdVega = VARIANT_FALSE;
		m_dWtdVega = BAD_DOUBLE_VALUE;

		IUnknownPtr spUnk;
		_variant_t varItem;
		ULONG nFetched = 0L;

		IEtsExpCalCollPtr spExpCalColl(aExpCalColl);
		CMmRvUndColl* pUndColl = dynamic_cast<CMmRvUndColl*>(aUndColl);
		if(pUndColl)
		{
			CMmRvUndColl::EnumCollType::iterator itUnd    =  pUndColl->m_coll.begin();
			CMmRvUndColl::EnumCollType::iterator itUndEnd =  pUndColl->m_coll.end();

			for(; itUnd != itUndEnd; ++itUnd)
			{
				CMmRvUndAtom* pUnd = dynamic_cast<CMmRvUndAtom*>(itUnd->second);
				if(pUnd)
				{
					IMmRvAggregationDataAtomPtr spUndData(pUnd);

					if(spUndData != NULL)
					{
						_CHK(pUnd->CalcWtdVega(spExpCalColl));

						// WtdVega
						if(pUnd->WtdVga$_ > BAD_DOUBLE_VALUE)
						{
							if(m_dWtdVega <= BAD_DOUBLE_VALUE) m_dWtdVega = 0.;
							m_dWtdVega += pUnd->WtdVga$_;
						}

						if(pUnd->BadWtdVga$_!=VARIANT_FALSE) 
							m_bBadWtdVega = VARIANT_TRUE;
					}
				}
			}
		}
	}
	catch(const _com_error& e)
	{
		hr = Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_IMmRvGrpAtom, e.Error());
	}
	return hr;
}

/////////////////////////////////////////////////////////////////////////////
//
void CMmRvGrpAtom::_CalcUndSynthValues(IMmRvUndCollPtr spUndColl, IMmRvUndAtomPtr spUnd, DOUBLE dIdxPrice, 
													DOUBLE dUndPriceTolerance, EtsPriceRoundingRuleEnum enPriceRoundingRule) throw()
{
	IMmRvSynthGreeksCollPtr spUndSynthGreeksColl;
	_CHK(spUnd->get_SynthGreeks(&spUndSynthGreeksColl));


	//-----------------------------------------------------------------------------------//
	DOUBLE	dTotalSynthNetExpLong	=	BAD_DOUBLE_VALUE;
	DOUBLE	dTotalSynthNetExpShort	=	BAD_DOUBLE_VALUE;
	DOUBLE	dAUM	=	BAD_DOUBLE_VALUE;
	DOUBLE	dCoeff	=	1.0;

	CMmRvUndAtom *pMainUnd = static_cast<CMmRvUndAtom*>(spUnd.GetInterfacePtr());
	
	if (pMainUnd)
	{
		dAUM	=	pMainUnd->GetNetExposureAUM();

		if (pMainUnd->m_spHeadComponent) 
			dCoeff = pMainUnd->m_dCoeff; //asset group component coeff
	}

 	if (pMainUnd->NetDlt$_ != BAD_DOUBLE_VALUE && dAUM != 0.0 && dAUM > BAD_DOUBLE_VALUE)
 	{
		DOUBLE	dNetExposure = pMainUnd->NetDlt$_ / dAUM * 100.0;
 		if (pMainUnd->NetDlt$_ < 0.0)
		{
			m_dNetExposureShort	-=	dNetExposure;
 			dTotalSynthNetExpShort	=	dNetExposure;
		}
 		else 
		{
			m_dNetExposureLong	-=	dNetExposure;
 			dTotalSynthNetExpLong = dNetExposure;
		}
 	}
	//-------------------------------------------------------------------------------------//
	if(spUndSynthGreeksColl != NULL)
	{
		LONG nSynthUndID = 0L;
		DOUBLE dSelfValue = BAD_DOUBLE_VALUE;
		IMmRvUndAtomPtr spSynthUnd;

		IUnknownPtr spUnk;
		_variant_t varItem;
		ULONG nFetched = 0L;
		HRESULT hr;

		_CHK(spUndSynthGreeksColl->get__NewEnum(&spUnk));
		IEnumVARIANTPtr spUndSynthGreekEnum(spUnk);
		_CHK(spUndSynthGreekEnum->Reset());
		while((hr = spUndSynthGreekEnum->Next(1L, &varItem, &nFetched)) == S_OK)
		{
			ATLASSERT(varItem.vt == VT_DISPATCH);
			IMmRvSynthGreeksAtomPtr spUndSynthGreeks(varItem);

			if(spUndSynthGreeks != NULL)
			{
				_CHK(spUndSynthGreeks->get_SynthUndID(&nSynthUndID));
				_CHK(spUndColl->get_Item(nSynthUndID, &spSynthUnd));
				IMmRvAggregationDataAtomPtr spSynthUndData(spSynthUnd);

				if(spSynthUnd != NULL && spSynthUndData != NULL)
				{
					EtsReplacePriceStatusEnum enPriceStatusMid = enRpsNone;
					DOUBLE dSynthUndMidPrice = 0.;

					IEtsPriceProfileAtomPtr spUndPriceProfile;
					_CHK(spSynthUnd->get_UndPriceProfile(&spUndPriceProfile));

					if(spUndPriceProfile != NULL)
					{

						enPriceStatusMid = enRpsNone;
						VARIANT_BOOL	bFutureUsed = VARIANT_FALSE;
						_CHK(spSynthUnd->GetUnderlyingPrice(dUndPriceTolerance,
														enPriceRoundingRule,
														&enPriceStatusMid,
														&bFutureUsed,
														&dSynthUndMidPrice));

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
							m_dDeltaEq += dUndValue * dSynthUndMidPrice /** dCoeff*/;

							if(m_dOptDelta <= BAD_DOUBLE_VALUE) m_dOptDelta = 0.;
							m_dOptDelta += dUndValue * dSynthUndMidPrice /** dCoeff*/;

							DOUBLE dDeltaInMoney = dUndValue * dSynthUndMidPrice;
							_CHK(spUndSynthGreeks->put_DeltaInMoney(dDeltaInMoney));

							DOUBLE dBeta = BAD_DOUBLE_VALUE, dBetaWtdDeltaInMoney = BAD_DOUBLE_VALUE;
							_CHK(spSynthUndData->get_Beta(&dBeta));
							if (pMainUnd->m_spHeadComponent)
								pMainUnd->m_spHeadComponent->get_Beta(&dBeta);

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
							m_dNetDelta += dUndValue /** dCoeff*/;
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
							m_dNetDelta += dUndValue /** dCoeff*/;
							dUndValue = 0.0;

						}
					}

					// NetExposure
					_CHK(spUndSynthGreeks->get_NetExposure(&dUndValue));
					if (dUndValue != BAD_DOUBLE_VALUE)
					{
						if (dUndValue < 0.0)
						{
							if (dTotalSynthNetExpShort==BAD_DOUBLE_VALUE) dTotalSynthNetExpShort = 0.0;
							dTotalSynthNetExpShort += dUndValue;
						}
						else
						{
							if (dTotalSynthNetExpLong==BAD_DOUBLE_VALUE) dTotalSynthNetExpLong = 0.0;
							dTotalSynthNetExpLong += dUndValue;
						}
					}

					// GammaInSharesPerc
					_CHK(spUndSynthGreeks->get_GammaInSharesPerc(&dUndValue));
					if(dUndValue > BAD_DOUBLE_VALUE && dSynthUndMidPrice > DBL_EPSILON)
					{
						if(m_dGammaEq <= BAD_DOUBLE_VALUE) m_dGammaEq = 0.;
						m_dGammaEq += dUndValue * dSynthUndMidPrice * dCoeff /** dCoeff*/;
					}
					else
						m_bBadGammaEq = VARIANT_TRUE;

					// GammaInShares
					_CHK(spUndSynthGreeks->get_GammaInShares(&dUndValue));
					if(dUndValue > BAD_DOUBLE_VALUE)
					{
						if(m_dNetGamma <= BAD_DOUBLE_VALUE) m_dNetGamma = 0.;
						m_dNetGamma += dUndValue * dCoeff * dCoeff;
					}
					else
						m_bBadNetGamma = VARIANT_TRUE;

					// NetGammaEq
					_CHK(spUndSynthGreeks->get_NetGamma(&dUndValue));
					if(dUndValue > BAD_DOUBLE_VALUE)
					{
						if(m_dNetGammaEq <= BAD_DOUBLE_VALUE) m_dNetGammaEq = 0.;
						m_dNetGammaEq += dUndValue /** dCoeff * dCoeff*/;
					}
					else
						m_bBadNetGammaEq = VARIANT_TRUE;

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
					_CHK(spSynthUndData->get_BadGammaPerc(&bUndValue));
					if(bUndValue)
						m_bBadGammaEq = VARIANT_TRUE;

					// BadGamma
					_CHK(spSynthUndData->get_BadGamma(&bUndValue));
					if(bUndValue)
						m_bBadNetGamma = VARIANT_TRUE;
				}
			}
		}

		if (m_dNetExposureLong == BAD_DOUBLE_VALUE) 
			m_dNetExposureLong = 0.0;

		if (dTotalSynthNetExpLong!=BAD_DOUBLE_VALUE)
			m_dNetExposureLong += dTotalSynthNetExpLong;

		if (m_dNetExposureShort == BAD_DOUBLE_VALUE) 
			m_dNetExposureShort = 0.0;

		if (dTotalSynthNetExpShort != BAD_DOUBLE_VALUE)
			m_dNetExposureShort+= dTotalSynthNetExpShort;
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

STDMETHODIMP CMmRvGrpAtom::put_NetExposureAUM(DOUBLE dNewVal)
{
	HRESULT hr = S_OK;
	if(m_dNetExposureAUM != dNewVal)
	{
		m_dNetExposureAUM = dNewVal;
	}
	return hr;
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


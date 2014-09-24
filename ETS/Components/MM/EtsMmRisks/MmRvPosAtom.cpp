// MmRvPosAtom.cpp : Implementation of CMmRvPosAtom
#include "stdafx.h"
#include "MmRvPosAtom.h"
#include "MmRvUndColl.h"
#include "MmRvUndAtom.h"

/////////////////////////////////////////////////////////////////////////////
HRESULT CMmRvPosAtom::UpdateQuotes()
{
	try
	{
		if (m_spContract != NULL)
		{
			QuoteUpdateInfo retQuotes;
			_CHK(m_spContract->raw_GetQuotes(&retQuotes), _T("Fail to get contract quotes."));

			if (retQuotes.BidPrice > BAD_DOUBLE_VALUE)
				m_pQuote->m_spPrice->put_Bid(retQuotes.BidPrice);

			if (retQuotes.AskPrice > BAD_DOUBLE_VALUE)
				m_pQuote->m_spPrice->put_Ask(retQuotes.AskPrice);

			if (retQuotes.LastPrice > BAD_DOUBLE_VALUE)
				m_pQuote->m_spPrice->put_Last(retQuotes.LastPrice);

			if (retQuotes.NetChange > BAD_DOUBLE_VALUE)
				m_pQuote->m_pPrice->put_NetChange(retQuotes.NetChange);

			SysFreeString(retQuotes.BidExchange);
			SysFreeString(retQuotes.AskExchange);
			SysFreeString(retQuotes.Currency);
			SysFreeString(retQuotes.Exchange);
		}
	}
	catch(_com_error& e)
	{
		return Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_IMmRvPosAtom, e.Error());
	}

	return S_OK;
};
/////////////////////////////////////////////////////////////////////////////
HRESULT 
CMmRvPosAtom::UpdateRisks(IMarketSimulationScenarioPtr SimulationScenario){
	
	try{

		IRisks retRisks;
		if (m_spContract){

			if (SimulationScenario){
				_CHK(m_spContract->raw_Calculate(NULL, SimulationScenario, &retRisks),
					_T("Fail to calculate contract."));
			}
			else{
				_CHK(m_spContract->raw_GetRisks(&retRisks),
					_T("Fail to get contract risks."));
			}

			if ((retRisks.TheoPrice > BAD_DOUBLE_VALUE)&&(retRisks.TheoPrice < 0))
				retRisks.TheoPrice = 0;

			if ( _finite(retRisks.Delta) )
				m_pQuote->m_dPriceTheo = retRisks.TheoPrice;

			if ( _finite(retRisks.Delta) )
				m_pQuote->m_dDelta = retRisks.Delta;

			if ( _finite(retRisks.Gamma) )
				m_pQuote->m_dGamma = retRisks.Gamma;

			if ( _finite(retRisks.Theta) )
				m_pQuote->m_dTheta = retRisks.Theta;

			if ( _finite(retRisks.Vega) )
				m_pQuote->m_dVega = retRisks.Vega;

			if ( _finite(retRisks.Rho) )
				m_pQuote->m_dRho = retRisks.Rho;

			if ( _finite(retRisks.DeltaVega) )
				m_pQuote->m_dVegaDelta = retRisks.DeltaVega;

			if ( _finite(retRisks.GammaVega) )
				m_pQuote->m_dVegaGamma = retRisks.GammaVega;

			if ( _finite(retRisks.DeltaTheta) )
				m_pQuote->m_dThetaDelta = retRisks.DeltaTheta;

			if ( _finite(retRisks.GammaTheta) )
				m_pQuote->m_dThetaGamma = retRisks.GammaTheta;

			if ( _finite(retRisks.IVola) )
				m_pQuote->m_dImpliedVola = retRisks.IVola;

			if ( _finite(retRisks.TheoVola) )
				m_pQuote->m_dVola = retRisks.TheoVola;

			if ( _finite(retRisks.TheoPriceBid) )
				m_pQuote->m_dPriceTheoBid = retRisks.TheoPriceBid;

			if ( _finite(retRisks.TheoPriceAsk) )
				m_pQuote->m_dPriceTheoAsk = retRisks.TheoPriceAsk;

			if ( _finite(retRisks.ContractPrice) )
				m_pQuote->m_dContractPrice = retRisks.ContractPrice;

			if ( _finite(retRisks.ContractPriceBid) )
				m_pQuote->m_dContractPriceBid = retRisks.ContractPriceBid;

			if ( _finite(retRisks.ContractPriceAsk) )
				m_pQuote->m_dContractPriceAsk = retRisks.ContractPriceAsk;

			if ( _finite(retRisks.BaseContractPrice) )
				m_pQuote->m_dBaseContractPrice = retRisks.BaseContractPrice;

			if ( _finite(retRisks.BaseContractPriceBid) )
				m_pQuote->m_dBaseContractPriceBid = retRisks.BaseContractPriceBid;

			if ( _finite(retRisks.BaseContractPriceAsk) )
				m_pQuote->m_dBaseContractPriceAsk = retRisks.BaseContractPriceAsk;

			if ( _finite(retRisks.IntrinsicValue) )
				m_pQuote->m_dIntrinsicValue = retRisks.IntrinsicValue;

			m_pQuote->m_spPrice->put_Active(retRisks.ContractPrice);
		}
	}
	catch(_com_error& e){
		return Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_IMmRvPosAtom, e.Error());
	}
	return S_OK;
};
/////////////////////////////////////////////////////////////////////////////
STDMETHODIMP CMmRvPosAtom::FitToMarketVolatility(DOUBLE Value)
{
	try
	{
		if (static_cast<bool>(m_spContract))
		{
			if (m_enContractType == enCtOption || m_enContractType == enCtFutOption)
				_CHK(m_spContract->FitToMarketVolatility(Value), _T("Error: while trying to fit volatility to market."));
		}
	}
	catch (_com_error& e)
	{
		return Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_IMmRvPosAtom, e.Error());
	};
	return S_OK;
};
/////////////////////////////////////////////////////////////////////////////
/*virtual*/
//void
//AggregationStrategy::AggregateTo(CMmRvAggData *aggregation, __MmRvPosAtom* position){
//	if (aggregation && position){
//
//		if (IsBadValue(aggregation->OptDlt_)) 
//			aggregation->OptDlt_ = 0.;
//		aggregation->OptDlt_ += position->m_dDeltaInShares;
//
//		if (IsBadValue(aggregation->OptDlt$_)) 
//			aggregation->OptDlt$_ = 0.;
//		aggregation->OptDlt$_ += position->m_dDeltaEq;
//
//		if (IsBadValue(aggregation->NetDlt_))
//			aggregation->NetDlt_ = 0.;
//		aggregation->NetDlt_ += position->m_dDeltaInShares;
//
//		if (IsBadValue(aggregation->NetDlt$_))
//			aggregation->NetDlt$_ = 0.;
//		aggregation->NetDlt$_ += position->m_dDeltaEq;
//
//		if (IsBadValue(aggregation->NetExposure_)) 
//			aggregation->NetExposure_ = 0.;
//		aggregation->NetExposure_ += position->m_dNetExposure;
//	};
//};
//
///*virtual*/
//void
//SpotAggregationStrategy::AggregateTo(CMmRvAggData* aggregation, __MmRvPosAtom* position){
//	if (aggregation && position){
//
//		if (IsBadValue(aggregation->NetDlt_))
//			aggregation->NetDlt_ = 0.;
//		aggregation->NetDlt_ += position->m_dDeltaInShares;
//
//		if (IsBadValue(aggregation->NetDlt$_))
//			aggregation->NetDlt$_ = 0.;
//		aggregation->NetDlt$_ += position->m_dDeltaEq;
//
//		if (IsBadValue(aggregation->NetExposure_)) 
//			aggregation->NetExposure_ = 0.;
//		aggregation->NetExposure_ += position->m_dNetExposure;
//	};
//};
/////////////////////////////////////////////////////////////////////////////
STDMETHODIMP CMmRvPosAtom::CalcOptionGreeks(IMmRvUndColl* aUndColl,
														  IMmRvUndAtom* aUnd,
														  DOUBLE dUndMidPrice,
														  LONG nMask,
														  VARIANT_BOOL bIsPnlLTD,
														  EtsCalcModelTypeEnum enCalcModel,
														  VARIANT_BOOL bUseTheoVolatility,
														  VARIANT_BOOL bUseTheoVolaNoBid,
														  VARIANT_BOOL bUseTheoVolaBadMarket,
														  DOUBLE dUndPriceTolerance,
														  EtsPriceRoundingRuleEnum enPriceRoundingRule,
														  DATE dtCalcDate,
														  ICalculationParametrs* pParams)
{

	if(aUnd == NULL || aUndColl == NULL || pParams == NULL)
		return Error(L"Invalid objects passed.", IID_IMmRvPosAtom, E_INVALIDARG);

	if(m_enContractType != enCtOption)
		return S_OK;

	ICalculationParametrsPtr spParams(pParams);

	try
	{
		IMmRvUndCollPtr spUndColl(aUndColl);

		CMmRvUndAtom* pUnd     = dynamic_cast<CMmRvUndAtom*>(aUnd);
		CMmRvAggData* pUndData = static_cast<CMmRvAggData*>(pUnd);
		if(pUnd && pUndData){

			IEtsPriceProfileAtomPtr spUndPriceProfile(pUnd->m_spUndPriceProfile);
			IEtsPriceProfileAtomPtr spOptPriceProfile(pUnd->m_spOptPriceProfile);
			
			//DOUBLE dUndPrice = dUndMidPrice;

			//DOUBLE sUndPriceSave = dUndMidPrice;
			//ISynthRootAtomPtr spSynthRoot;

			//if(m_bIsSynthetic)
			//{
			//	ISynthRootCollPtr spSynthRootColl = pUnd->m_spSynthRoots;
			//	if(spSynthRootColl != NULL)
			//	{
			//		spSynthRoot = spSynthRootColl->Item[m_nOptionRootID];
			//		if(spSynthRoot != NULL)
			//		{
			//			DOUBLE dSynthBid = 0., dSynthAsk = 0., dSynthLast = 0.;

			//			_CHK(pUnd->GetSyntheticUnderlyingPrice(spUndColl, spSynthRoot, &dSynthBid, &dSynthAsk, &dSynthLast));

			//			dUndMidPrice = spUndPriceProfile->GetUndPriceMid(dSynthBid, dSynthAsk, dSynthLast, dUndPriceTolerance, enPriceRoundingRule, NULL, VARIANT_FALSE);

			//			m_pQuote->m_pSuPrice->put_Last(dUndMidPrice);
			//			m_pQuote->m_pSuPrice->put_Bid(dSynthBid);
			//			m_pQuote->m_pSuPrice->put_Ask(dSynthAsk);
			//		}
			//	}
			//}


			_CalcOptPositionData(spOptPriceProfile,
								 NULL,
								 bIsPnlLTD,
								 bUseTheoVolatility,
								 bUseTheoVolaNoBid,
								 bUseTheoVolaBadMarket,
								 dUndPriceTolerance,
								 enPriceRoundingRule,
								 spUndColl,
								 0., 
								 pUnd->m_nID,
								 dtCalcDate,
								 spParams);

		}
		else
		{
			return E_POINTER;
		}
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_IMmRvPosAtom, e.Error());
	}

	return S_OK;
}

void CMmRvPosAtom::_CalcOptPositionData(
													 IEtsPriceProfileAtomPtr spOptPriceProfile,
													 ISynthRootAtomPtr spSynthRoot,
													 VARIANT_BOOL bIsPnlLTD,
													 VARIANT_BOOL bUseTheoVolatility,
													 VARIANT_BOOL bUseTheoVolaNoBid,
													 VARIANT_BOOL bUseTheoVolaBadMarket,
													 DOUBLE dUndPriceTolerance,
													 EtsPriceRoundingRuleEnum enPriceRoundingRule, 
													 IMmRvUndCollPtr spUndColl,
													 DOUBLE dUndMidPrice, 
													 LONG   nUndID,
													 DATE dtCalcDate,
													 ICalculationParametrsPtr spParams
													 )
{
	// clear values in shares
	__MmRvPosAtom::ClearValuesInShares();
	if(m_bIsSynthetic && m_spSynthGreeks != NULL)
	{
		IUnknownPtr spUnk;
		_variant_t varItem;
		ULONG nFetched = 0L;
		HRESULT hr;
		IMmRvSynthGreeksAtomPtr spSynthGreeks;

		_CHK(m_spSynthGreeks->get__NewEnum(&spUnk));
		IEnumVARIANTPtr spSynthGreekEnum(spUnk);
		_CHK(spSynthGreekEnum->Reset());
		while((hr = spSynthGreekEnum->Next(1L, &varItem, &nFetched)) == S_OK)
		{
			ATLASSERT(varItem.vt == VT_DISPATCH);
			spSynthGreeks = varItem;
			if(spSynthGreeks != NULL)
				_CHK(spSynthGreeks->ClearValuesInShares());
		}
	}

	// calc values in shares
	EtsReplacePriceStatusEnum enPriceStatusMid = enRpsNone;
	EtsReplacePriceStatusEnum enPriceStatusBid = enRpsNone;
	EtsReplacePriceStatusEnum enPriceStatusAsk = enRpsNone;

	double base_contract_price = m_pQuote->m_dBaseContractPrice;

	double contract_price_bid = m_pQuote->m_dContractPriceBid;
	double contract_price_ask = m_pQuote->m_dContractPriceAsk;
	double contract_price_mid = m_pQuote->m_dContractPrice;


	// pnl
	if (m_bIsFlex == VARIANT_FALSE){
		_CalcPnlMtm(bIsPnlLTD, contract_price_bid, contract_price_ask, false, dtCalcDate);
	}
	else{
		_CalcPnlMtm(bIsPnlLTD, m_pQuote->m_dPriceTheo, m_pQuote->m_dPriceTheo, false, dtCalcDate);
	}

	_CalcPnlTheo(bIsPnlLTD, dtCalcDate);

	//AUM
	DOUBLE	dAUM		=	BAD_DOUBLE_VALUE;
	IMmRvUndAtomPtr spMainUnd;
	_CHK(spUndColl->get_Item(nUndID, &spMainUnd));
	CMmRvUndAtom *pMainUnd = static_cast<CMmRvUndAtom*>(spMainUnd.GetInterfacePtr());
	if (pMainUnd){
		dAUM	=	pMainUnd->GetNetExposureAUM();
	}

	DOUBLE dMainComponentWeight = 1.0;

	/*// delta & gamma
	if(m_pQuote->m_dDelta > BAD_DOUBLE_VALUE || m_pQuote->m_dGamma > BAD_DOUBLE_VALUE)
	{
		if(!m_bIsSynthetic || m_spSynthGreeks == NULL || spSynthRoot == NULL)
		{
			if(m_pQuote->m_dDelta > BAD_DOUBLE_VALUE){
				m_dDeltaInShares = m_pQuote->m_dDelta * m_nQtyInShares;
			}

			if(m_pQuote->m_dGamma > BAD_DOUBLE_VALUE){
				m_dGammaInShares = m_pQuote->m_dGamma * m_nQtyInShares;

				if( base_contract_price> DBL_EPSILON )
					m_dGammaInSharesPerc = m_pQuote->m_dGamma * m_nQtyInShares * base_contract_price / 100.;

				m_dNetGamma = m_pQuote->m_dGamma * m_nQtyInShares  / 100.;

				if( base_contract_price > DBL_EPSILON )
					m_dNetGamma = m_pQuote->m_dGamma * m_nQtyInShares  * base_contract_price * base_contract_price / 100.;

			}
		}
		else // synthetic
		{
			LONG nSynthUndID = 0L;
			ISynthRootCompAtomPtr spSRComp;
			ISynthRootCompCollPtr spSRCompColl;
			

			spSRCompColl = spSynthRoot->SynthRootComponents;
			if(spSRCompColl != NULL)
			{
				spSRComp = spSRCompColl->Item[nUndID];
				if(spSRComp != NULL)
				{
					if(m_pQuote->m_dDelta > BAD_DOUBLE_VALUE)
						m_dDeltaInShares = m_pQuote->m_dDelta * m_nQtyInShares * spSRComp->Weight;

					if(m_pQuote->m_dGamma > BAD_DOUBLE_VALUE)
						m_dGammaInShares = m_pQuote->m_dGamma * m_nQtyInShares * spSRComp->Weight;

					dMainComponentWeight = spSRComp->Weight;
				}

				if(dUndMidPrice > DBL_EPSILON && m_dGammaInShares > BAD_DOUBLE_VALUE)
					m_dGammaInSharesPerc = m_dGammaInShares * dUndMidPrice / 100.;

				IUnknownPtr spUnk;
				_variant_t varItem;
				ULONG nFetched = 0L;
				HRESULT hr;
				IMmRvSynthGreeksAtomPtr spSynthGreeks;

				_CHK(m_spSynthGreeks->get__NewEnum(&spUnk));
				IEnumVARIANTPtr spSynthGreekEnum(spUnk);
				_CHK(spSynthGreekEnum->Reset());
				while((hr = spSynthGreekEnum->Next(1L, &varItem, &nFetched)) == S_OK)
				{
					ATLASSERT(varItem.vt == VT_DISPATCH);
					spSynthGreeks = varItem;

					if(spSynthGreeks != NULL)
					{
						DOUBLE dGammaInShares = BAD_DOUBLE_VALUE;
						DOUBLE dDeltaInShares = BAD_DOUBLE_VALUE;
						DOUBLE dUndPrice = BAD_DOUBLE_VALUE;

						_CHK(spSynthGreeks->get_SynthUndID(&nSynthUndID));
						spSRComp = spSRCompColl->Item[nSynthUndID];

						IMmRvUndAtomPtr spUnd2;
						_CHK(spUndColl->get_Item(nSynthUndID, &spUnd2));
						IMmRvAggregationDataAtomPtr spUndData2(spUnd2);
						if(spUnd2 != NULL)
						{
							enPriceStatusMid = enRpsNone;

							VARIANT_BOOL bFutureUsed = VARIANT_FALSE;
							spUnd2->GetUnderlyingPrice(	dUndPriceTolerance,
														enPriceRoundingRule,
														&enPriceStatusMid,
														&bFutureUsed,
														&dUndPrice);
						}


						if(spSRComp != NULL)
						{
							DOUBLE dWeight = spSRComp->Weight;
							if(m_pQuote->m_dDelta > BAD_DOUBLE_VALUE)
							{
								dDeltaInShares = m_pQuote->m_dDelta * m_nQtyInShares * dWeight;
								_CHK(spSynthGreeks->put_DeltaInShares(dDeltaInShares));
								_CHK(spSynthGreeks->put_NetDelta(dDeltaInShares));

								if ( (dAUM != BAD_DOUBLE_VALUE || dAUM != 0.0) && dUndPrice != BAD_DOUBLE_VALUE)
									_CHK(spSynthGreeks->put_NetExposure(dDeltaInShares * dUndPrice / dAUM * 100.0));
								else
									_CHK(spSynthGreeks->put_NetExposure(BAD_DOUBLE_VALUE));
							}

							if(m_pQuote->m_dRho > BAD_DOUBLE_VALUE)
								_CHK(spSynthGreeks->put_RhoInShares(m_pQuote->m_dRho * m_nQtyInShares * dWeight));
							else
								_CHK(spSynthGreeks->put_BadRhoInShares(VARIANT_TRUE));

							if(m_pQuote->m_dGamma > BAD_DOUBLE_VALUE)
							{
								dGammaInShares = m_pQuote->m_dGamma * m_nQtyInShares * dWeight;
								_CHK(spSynthGreeks->put_GammaInShares(dGammaInShares));
							}

							if(m_pQuote->m_dTheta > BAD_DOUBLE_VALUE)
								_CHK(spSynthGreeks->put_ThetaInShares(m_pQuote->m_dTheta * m_nQtyInShares * dWeight));
							else
								_CHK(spSynthGreeks->put_BadThetaInShares(VARIANT_TRUE));
						}
						else if(nSynthUndID == 0L) // USD_ID
						{
							DOUBLE dCashValue = spSynthRoot->CashValue;
							if(m_pQuote->m_dDelta > BAD_DOUBLE_VALUE)
							{
								_CHK(spSynthGreeks->put_DeltaInShares(m_pQuote->m_dDelta * m_nQtyInShares * dCashValue));
								_CHK(spSynthGreeks->put_NetDelta(m_pQuote->m_dDelta * m_nQtyInShares * dCashValue));

								if ( dAUM != BAD_DOUBLE_VALUE || dAUM != 0.0)
									_CHK(spSynthGreeks->put_NetExposure(m_pQuote->m_dDelta * m_nQtyInShares * dCashValue / dAUM * 100.0));
								else
									_CHK(spSynthGreeks->put_NetExposure(BAD_DOUBLE_VALUE));
							}

							if(m_pQuote->m_dGamma > BAD_DOUBLE_VALUE)
							{
								dGammaInShares = m_pQuote->m_dGamma * m_nQtyInShares * dCashValue;
								_CHK(spSynthGreeks->put_GammaInShares(dGammaInShares));
							}
							_CHK(spSynthGreeks->put_DeltaInMoney(m_pQuote->m_dDelta * m_nQtyInShares * dCashValue));
						}


						if(dUndPrice >= 0.0)
						{
							if( dGammaInShares > BAD_DOUBLE_VALUE && m_pQuote->m_dGamma > BAD_DOUBLE_VALUE)
							{
								_CHK(spSynthGreeks->put_GammaInSharesPerc(dGammaInShares * dUndPrice / 100.));
								_CHK(spSynthGreeks->put_NetGamma(dGammaInShares * dUndPrice * dUndPrice / 100));
							}
							else
								spSynthGreeks->put_BadNetGamma(VARIANT_TRUE);

							if(dDeltaInShares > BAD_DOUBLE_VALUE)
							{
								_CHK(spSynthGreeks->put_NetDelta(dDeltaInShares)); //* dUndMidPrice));

								if ( dAUM != BAD_DOUBLE_VALUE || dAUM != 0.0)
									_CHK(spSynthGreeks->put_NetExposure(dDeltaInShares * dUndPrice / dAUM * 100.0));
								else
									_CHK(spSynthGreeks->put_NetExposure(BAD_DOUBLE_VALUE));

								if(nSynthUndID != 0L)
									_CHK(spSynthGreeks->put_DeltaInMoney(dDeltaInShares * dUndPrice));
							}
							else 
								if(nSynthUndID != 0L)
								{
									spSynthGreeks->put_BadDeltaInMoney(VARIANT_TRUE);
									spSynthGreeks->put_BadNetDelta(VARIANT_TRUE);
									spSynthGreeks->put_BadNetExposure(VARIANT_TRUE);
								}

						}
					}
				}
			}

		}
	}  
	*/

	//if(m_pQuote->m_dDelta > BAD_DOUBLE_VALUE){
	//	m_dDeltaInShares = m_pQuote->m_dDelta * m_nQty InShares;
	//}

	if(m_pQuote->m_dGamma > BAD_DOUBLE_VALUE){
		m_dGammaInShares = m_pQuote->m_dGamma * m_nQtyInShares;

		if( base_contract_price> DBL_EPSILON )
			m_dGammaInSharesPerc = m_pQuote->m_dGamma * m_nQtyInShares * base_contract_price / 100.;

		m_dNetGamma = m_pQuote->m_dGamma * m_nQtyInShares  / 100.;

		if( base_contract_price > DBL_EPSILON )
			m_dNetGamma = m_pQuote->m_dGamma * m_nQtyInShares  * base_contract_price * base_contract_price / 100.;

	}

	// other theos
	if(m_pQuote->m_dVega > BAD_DOUBLE_VALUE){
		m_dVegaInShares = m_pQuote->m_dVega * m_nQtyInShares;
		m_dWtdVega = m_dVegaInShares * m_dVegaWeight;
	}

	if(m_pQuote->m_dTheta > BAD_DOUBLE_VALUE)
		m_dThetaInShares = m_pQuote->m_dTheta * m_nQtyInShares;

	if(m_pQuote->m_dRho > BAD_DOUBLE_VALUE)
		m_dRhoInShares = m_pQuote->m_dRho * m_nQtyInShares;

	if(m_pQuote->m_dVegaDelta > BAD_DOUBLE_VALUE)
		m_dVegaDeltaInShares = m_pQuote->m_dVegaDelta * m_nQtyInShares;

	if(m_pQuote->m_dThetaDelta > BAD_DOUBLE_VALUE)
		m_dThetaDeltaInShares = m_pQuote->m_dThetaDelta * m_nQtyInShares;

	VARIANT_BOOL bUseMidMarketForPnL = spOptPriceProfile->UseMidMarketForPnL;
	VARIANT_BOOL bUseZeroBidRule = spOptPriceProfile->UseZeroBidRule;
	
	/*DOUBLE dUndMidPrice4Calc = (((m_bIsSynthetic || m_spSynthGreeks ) && m_pQuote->m_pSuPrice->m_dPriceLast > DBL_EPSILON) ?
								m_pQuote->m_pSuPrice->m_dPriceLast : base_contract_price);

	if(dUndMidPrice4Calc > DBL_EPSILON && dOptPricePnL >= 0)
	{*/
		//			formula for time value:
		//			for calls strike >=spot: time value= call price
		//			for calls strike <spot, time value= max(0, call price -(spot-strike))
		//			for puts strike >spot: time value= max(0, put price -(strike-spot))
		//			for puts strike <=spot, time value= put price
				
/*
		if(m_enOptType == enOtCall)
		{
			if(m_dStrike >= dUndMidPrice4Calc)
				m_dTimeValue = dOptPricePnL;
			else
				m_dTimeValue = max(0., dOptPricePnL - (dUndMidPrice4Calc - m_dStrike));
		}
		else
		{
			if(m_dStrike <= dUndMidPrice4Calc)
				m_dTimeValue = dOptPricePnL;
			else
				m_dTimeValue = max(0., dOptPricePnL- (m_dStrike-dUndMidPrice4Calc));
		}
	}
  */

	DOUBLE dPrice = 0.;
	if (bUseMidMarketForPnL == VARIANT_TRUE && bUseZeroBidRule == VARIANT_FALSE){
		dPrice = contract_price_mid;
	}
	else{
		dPrice = (m_nQty > 0.) ? contract_price_bid : contract_price_ask;
	}

	double dIntrinsicValue = max((base_contract_price - m_dStrike) * (m_enOptType == enOtCall ? 1 : -1), 0.);
	m_dTimeValue = max(0., dPrice - dIntrinsicValue);


	if(m_pQuote->m_dVegaGamma > BAD_DOUBLE_VALUE && base_contract_price > DBL_EPSILON)
		m_dVegaGammaInShares = m_pQuote->m_dVegaGamma * m_nQtyInShares * base_contract_price / 100.;

	if(m_pQuote->m_dThetaGamma > BAD_DOUBLE_VALUE && base_contract_price > DBL_EPSILON)
		m_dThetaGammaInShares = m_pQuote->m_dThetaGamma * m_nQtyInShares * base_contract_price / 100.;


	if(m_dNetGamma == BAD_DOUBLE_VALUE && m_pQuote->m_dGamma > BAD_DOUBLE_VALUE )
	{
			m_dNetGamma = m_pQuote->m_dGamma * m_nQtyInShares  / 100.;
			if( base_contract_price > DBL_EPSILON )
				m_dNetGamma =	m_pQuote->m_dGamma * m_nQtyInShares / 100. *
								(base_contract_price * base_contract_price) *				
								dMainComponentWeight;				   
	}
	
}


/////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CMmRvPosAtom::CalcFutOptionGreeks( IMmRvUndAtom* aUnd,
												DOUBLE dUndMidPrice,
												LONG nMask,
												VARIANT_BOOL bIsPnlLTD,
												EtsCalcModelTypeEnum enCalcModel,
												VARIANT_BOOL bUseTheoVolatility,
												VARIANT_BOOL bUseTheoVolaNoBid,
												VARIANT_BOOL bUseTheoVolaBadMarket,
												DOUBLE dUndPriceTolerance,
												EtsPriceRoundingRuleEnum enPriceRoundingRule,
												DATE _dtCalcDate,
												ICalculationParametrs* pParams)
{
	if(aUnd == NULL || pParams == NULL)
		return Error(L"Invalid object passed.", IID_IMmRvPosAtom, E_INVALIDARG);

	if(m_enContractType != enCtFutOption)
		return S_OK;

	try
	{
		IMmRvUndAtomPtr spUnd(aUnd);
		IMmRvAggregationDataAtomPtr spUndData(aUnd);
		IEtsPriceProfileAtomPtr spUndPriceProfile;
		IEtsPriceProfileAtomPtr spOptPriceProfile;
		ICalculationParametrsPtr spParams(pParams);

		_CHK(m_spFut->get_UndPriceProfile(&spUndPriceProfile));
		_CHK(m_spFut->get_OptPriceProfile(&spOptPriceProfile));


		EtsContractTypeEnum			m_enUndType = enCtStock;
		if (spUnd != NULL)
		{
			spUnd->get_ContractType(&m_enUndType);
		}

		_CalcFutPositionData	(	
										spOptPriceProfile, 
										bIsPnlLTD, bUseTheoVolatility, 
										bUseTheoVolaNoBid, bUseTheoVolaBadMarket, 
										dUndPriceTolerance, enPriceRoundingRule, 
										/*m_enUndType == enCtFutUnd ? dFutMidPrice :*/ dUndMidPrice,
										_dtCalcDate, m_enUndType, spParams
									);
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_IMmRvPosAtom, e.Error());
	}

	return S_OK;
}

void CMmRvPosAtom::_CalcFutPositionData(
													 IEtsPriceProfileAtomPtr spOptPriceProfile,
													 VARIANT_BOOL bIsPnlLTD,
													 VARIANT_BOOL bUseTheoVolatility,
													 VARIANT_BOOL bUseTheoVolaNoBid,
													 VARIANT_BOOL bUseTheoVolaBadMarket,
													 DOUBLE dUndPriceTolerance,
													 EtsPriceRoundingRuleEnum enPriceRoundingRule, 
													 DOUBLE dUndMidPrice,
													 DATE dtCalcDate,
													 EtsContractTypeEnum enUndType,
													 ICalculationParametrsPtr spParams
													 )
{
	// clear values in shares
	__MmRvPosAtom::ClearValuesInShares();

	// calc values in shares
	EtsReplacePriceStatusEnum enPriceStatusBid = enRpsNone;
	EtsReplacePriceStatusEnum enPriceStatusMid = enRpsNone;
	EtsReplacePriceStatusEnum enPriceStatusAsk = enRpsNone;

	DOUBLE dOptPriceBid = spOptPriceProfile->GetOptPriceBidForPnL(m_pQuote->m_pPrice->m_dPriceBid, m_pQuote->m_pPrice->m_dPriceAsk, m_pQuote->m_pPrice->m_dPriceLast, enPriceRoundingRule, bUseTheoVolatility, m_pQuote->m_dPriceTheo, &enPriceStatusBid);
	DOUBLE dOptPriceAsk = spOptPriceProfile->GetOptPriceAskForPnL(m_pQuote->m_pPrice->m_dPriceBid, m_pQuote->m_pPrice->m_dPriceAsk, m_pQuote->m_pPrice->m_dPriceLast, enPriceRoundingRule, bUseTheoVolatility, m_pQuote->m_dPriceTheo, &enPriceStatusAsk);
	DOUBLE dOptPriceMid = spOptPriceProfile->GetOptPriceMid(m_pQuote->m_pPrice->m_dPriceBid, m_pQuote->m_pPrice->m_dPriceAsk, m_pQuote->m_pPrice->m_dPriceLast, enPriceRoundingRule, bUseTheoVolatility, m_pQuote->m_dPriceTheo, &enPriceStatusMid);

	m_pQuote->m_enReplacePriceStatus = static_cast<EtsReplacePriceStatusEnum>(enPriceStatusMid | enPriceStatusBid | enPriceStatusAsk);

	if (m_pQuote->m_pPrice->m_bManualActive == VARIANT_TRUE)	
		dOptPriceAsk = dOptPriceBid = dOptPriceMid = m_pQuote->m_pPrice->m_dActivePrice;
	else
		m_pQuote->m_pPrice->m_dActivePrice = dOptPriceMid;

	if ( m_enContractType == enCtFutOption && enUndType == enCtFutUnd && m_spFut ) {
		CMmRvFutAtom* pFut = static_cast<CMmRvFutAtom*>(m_spFut.GetInterfacePtr() );
		if ( pFut )
		{
			m_dKEq = pFut->m_dKEq;
			m_bMultOptDltEq = pFut->m_bMultOptDltEq;
		}
	}

	// pnl
	_CalcPnlMtm ( bIsPnlLTD, dOptPriceBid, dOptPriceAsk, false, dtCalcDate ) ;
	_CalcPnlTheo ( bIsPnlLTD,dtCalcDate ) ;

	if ( m_enContractType == enCtFutOption && enUndType == enCtFutUnd && m_spFut ) {
		CMmRvFutAtom* pFut = static_cast<CMmRvFutAtom*>(m_spFut.GetInterfacePtr() );
		if ( pFut ){
			if(m_dPnlMtm <= BAD_DOUBLE_VALUE)
				m_dPnlMtm = 0.;
			else
				m_dPnlMtm *= pFut->m_dKEq / pFut->m_nFutLotSize;
			
			if(m_dPnlTheo <= BAD_DOUBLE_VALUE)
				m_dPnlTheo = 0.;
			else
				m_dPnlTheo *= pFut->m_dKEq / pFut->m_nFutLotSize;
		}
	}
	// delta
	if(m_pQuote->m_dDelta > BAD_DOUBLE_VALUE){
		if ( m_enContractType == enCtFutOption && enUndType == enCtFutUnd && m_spFut ) {
			CMmRvFutAtom* pFut = static_cast<CMmRvFutAtom*>(m_spFut.GetInterfacePtr() );
			if ( pFut ){
				LONG nDeltaMult = 1;
				if(!pFut->m_bMultOptDltEq)
					nDeltaMult = pFut->m_nFutLotSize;
				m_dDeltaInShares = m_pQuote->m_dDelta * m_nQty * nDeltaMult;
				//m_dDeltaInShares = m_pQuote->m_dDelta * m_nQty * contract->sizeInDealingAsset()
				m_dDeltaEq = m_pQuote->m_dDelta * m_nQty * dUndMidPrice * pFut->m_dKEq;
				pFut->m_dUndMidPrice = dUndMidPrice;
			}
		}
		else{
			m_dDeltaInShares = m_pQuote->m_dDelta * m_nQtyInShares;
			m_dDeltaEq = m_dDeltaInShares * dUndMidPrice;
		}
	}

	// gamma
	if(m_pQuote->m_dGamma > BAD_DOUBLE_VALUE)
	{
		if ( m_enContractType == enCtFutOption && enUndType == enCtFutUnd && m_spFut ) {
			CMmRvFutAtom* pFut = static_cast<CMmRvFutAtom*>(m_spFut.GetInterfacePtr() );
			if ( pFut ){
				LONG nCSzMult = 1;
				if(!pFut->m_bMultOptDltEq)
					nCSzMult = pFut->m_nFutLotSize;
				m_dGammaInShares = m_pQuote->m_dGamma * m_nQty * nCSzMult;
				if ( dUndMidPrice > DBL_EPSILON ) {
					m_dNetGamma          = m_pQuote->m_dGamma * m_nQty * dUndMidPrice * dUndMidPrice * pFut->m_dKEq / 100;
					m_dGammaInSharesPerc = m_pQuote->m_dGamma * m_nQty * dUndMidPrice * nCSzMult / 100.;
				}
			}
		}
		else{
			m_dGammaInShares = m_pQuote->m_dGamma * m_nQtyInShares;

			if(dUndMidPrice > DBL_EPSILON)
				m_dGammaInSharesPerc = m_pQuote->m_dGamma * m_nQtyInShares * dUndMidPrice / 100.;

			m_dNetGamma = m_pQuote->m_dGamma * m_nQtyInShares   / 100.;
			if(dUndMidPrice > DBL_EPSILON)
				m_dNetGamma = m_pQuote->m_dGamma * m_nQtyInShares  * dUndMidPrice * dUndMidPrice / 100.;
		}
	}

	// other theos
	if(m_pQuote->m_dVega > BAD_DOUBLE_VALUE){
		if ( m_enContractType == enCtFutOption && enUndType == enCtFutUnd && m_spFut ) {
			CMmRvFutAtom* pFut = static_cast<CMmRvFutAtom*>(m_spFut.GetInterfacePtr() );
			if ( pFut ){
				m_dVegaInShares = m_pQuote->m_dVega * m_nQty * pFut->m_dKEq;
				m_dWtdVega = m_dVegaInShares * m_dVegaWeight;
			}
		}
		else{
			m_dVegaInShares = m_pQuote->m_dVega * m_nQtyInShares;
			m_dWtdVega = m_dVegaInShares * m_dVegaWeight;
		}
	}

	if(m_pQuote->m_dTheta > BAD_DOUBLE_VALUE){
		if ( m_enContractType == enCtFutOption && enUndType == enCtFutUnd && m_spFut ) {
			CMmRvFutAtom* pFut = static_cast<CMmRvFutAtom*>(m_spFut.GetInterfacePtr() );
			if ( pFut )
				m_dThetaInShares = m_pQuote->m_dTheta * m_nQty * pFut->m_dKEq;
		}
		else
			m_dThetaInShares = m_pQuote->m_dTheta * m_nQtyInShares;
	}

	if(m_pQuote->m_dRho > BAD_DOUBLE_VALUE){
		if ( m_enContractType == enCtFutOption && enUndType == enCtFutUnd && m_spFut ) {
			CMmRvFutAtom* pFut = static_cast<CMmRvFutAtom*>(m_spFut.GetInterfacePtr() );
			if ( pFut )
				m_dRhoInShares = m_pQuote->m_dRho * m_nQty * pFut->m_dKEq;
		}
		else
			m_dRhoInShares = m_pQuote->m_dRho * m_nQtyInShares;
	}

	if(m_pQuote->m_dVegaDelta > BAD_DOUBLE_VALUE)
		if ( m_enContractType == enCtFutOption && enUndType == enCtFutUnd && m_spFut ) {
			CMmRvFutAtom* pFut = static_cast<CMmRvFutAtom*>(m_spFut.GetInterfacePtr() );
			if ( pFut )
			{
				LONG nVegaDeltaMult = 1;
				if(!pFut->m_bMultOptDltEq)
					nVegaDeltaMult = pFut->m_nFutLotSize;
				m_dVegaDeltaInShares = m_pQuote->m_dVegaDelta * m_nQty * nVegaDeltaMult;
			}
		}
		else
			m_dVegaDeltaInShares = m_pQuote->m_dVegaDelta * m_nQtyInShares;


	if(m_pQuote->m_dVegaGamma > BAD_DOUBLE_VALUE && dUndMidPrice > DBL_EPSILON)
		if ( m_enContractType == enCtFutOption && enUndType == enCtFutUnd && m_spFut ) {
			CMmRvFutAtom* pFut = static_cast<CMmRvFutAtom*>(m_spFut.GetInterfacePtr() );
			if ( pFut )
			{
				LONG nVegaGammaMult = 1;
				if(!pFut->m_bMultOptDltEq)
					nVegaGammaMult = pFut->m_nFutLotSize;
				m_dVegaGammaInShares = m_pQuote->m_dVegaGamma * m_nQty * dUndMidPrice / 100. * nVegaGammaMult;
			}
		}
		else
		m_dVegaGammaInShares = m_pQuote->m_dVegaGamma * m_nQtyInShares * dUndMidPrice / 100.;

	if(m_pQuote->m_dThetaDelta > BAD_DOUBLE_VALUE)
		if ( m_enContractType == enCtFutOption && enUndType == enCtFutUnd && m_spFut ) {
			CMmRvFutAtom* pFut = static_cast<CMmRvFutAtom*>(m_spFut.GetInterfacePtr() );
			if ( pFut )
			{
				LONG nThetaDeltaMult = 1;
				if(!pFut->m_bMultOptDltEq)
					nThetaDeltaMult = pFut->m_nFutLotSize;
				m_dThetaDeltaInShares = m_pQuote->m_dThetaDelta * m_nQty * nThetaDeltaMult;
			}
		}
		else
			m_dThetaDeltaInShares = m_pQuote->m_dThetaDelta * m_nQtyInShares;

	if(m_pQuote->m_dThetaGamma > BAD_DOUBLE_VALUE && dUndMidPrice > DBL_EPSILON)
		if ( m_enContractType == enCtFutOption && enUndType == enCtFutUnd && m_spFut ) {
			CMmRvFutAtom* pFut = static_cast<CMmRvFutAtom*>(m_spFut.GetInterfacePtr() );
			if ( pFut )
			{
				LONG nThetaGammaMult = 1;
				if(!pFut->m_bMultOptDltEq)
					nThetaGammaMult = pFut->m_nFutLotSize;
				m_dThetaGammaInShares = m_pQuote->m_dThetaGamma * m_nQty * dUndMidPrice / 100. * nThetaGammaMult;
			}
		}
		else
		m_dThetaGammaInShares = m_pQuote->m_dThetaGamma * m_nQtyInShares * dUndMidPrice / 100.;

	if(dOptPriceMid > DBL_EPSILON)
	{
		if ( m_enContractType == enCtFutOption /*&& enUndType == enCtFutUnd*/ && m_spFut )
		{
			CMmRvFutAtom* pFut = static_cast<CMmRvFutAtom*>(m_spFut.GetInterfacePtr() );
			if ( pFut )
			{
				EtsReplacePriceStatusEnum penPriceStatus;	VARIANT_BOOL dummy; double dFutPrice;
				pFut->GetFuturePrice(dUndPriceTolerance, enPriceRoundingRule, &penPriceStatus, &dummy, &dFutPrice );
				if(dFutPrice > DBL_EPSILON)
				{
					if(m_enOptType == enOtCall)
					{
						if(m_dStrike > dFutPrice)
							m_dTimeValue = dOptPriceMid;
						else
							m_dTimeValue = max(0., (dOptPriceMid - (dFutPrice - m_dStrike)));
					}
					else
					{
						if(m_dStrike <= dFutPrice)
							m_dTimeValue = dOptPriceMid;
						else
							m_dTimeValue = max(0., (dOptPriceMid - (m_dStrike - dFutPrice)));
					}
				}
			}
		}
		else
		{
			if(dUndMidPrice > DBL_EPSILON)
			{
				if(m_enOptType == enOtCall)
				{
					if(m_dStrike >= dUndMidPrice)
						m_dTimeValue = dOptPriceMid;
					else
						m_dTimeValue = max(0., dOptPriceMid - (dUndMidPrice-m_dStrike));
				}
				else
				{
					if(m_dStrike <= dUndMidPrice)
						m_dTimeValue = dOptPriceMid;
					else
						m_dTimeValue = max(0., dOptPriceMid- (m_dStrike-dUndMidPrice)) ;
				}
			}
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
//
void CMmRvPosAtom::_CalcPnlMtm(VARIANT_BOOL bIsPnlLTD, DOUBLE dPriceBid, DOUBLE dPriceAsk, bool ReplaceWithZero, DATE _dtCalculationDate)
{
	m_dPnlMtm = BAD_DOUBLE_VALUE;

	_dtCalculationDate = (DWORD)_dtCalculationDate ;
	DATE dtCurr = EgLib::vt_date::GetCurrentDate ( true ) ;

	if ( bIsPnlLTD /*|| dtCurr != _dtCalculationDate*/ )
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
			if((dPriceAsk > 0 /*BAD_DOUBLE_VALUE*/ || dPriceBid > 0 || ReplaceWithZero ) && m_dPosDailyPrevDateSell > BAD_DOUBLE_VALUE)
			{
				if(m_dPnlMtm <= BAD_DOUBLE_VALUE) m_dPnlMtm = 0.;
				m_dPnlMtm += dPriceAsk * m_nQtyDailyPrevDateSell - m_dPosDailyPrevDateSell;
			}
			else
				return;
		}

		if(m_nQtyDailyTodayBuy > BAD_LONG_VALUE)
		{
			if( (dPriceBid > 0/*BAD_DOUBLE_VALUE*/ || dPriceAsk > 0 /*BAD_DOUBLE_VALUE*/ || ReplaceWithZero ) && m_dPosDailyTodayBuy > BAD_DOUBLE_VALUE)
			{
				if(m_dPnlMtm <= BAD_DOUBLE_VALUE) m_dPnlMtm = 0.;
				m_dPnlMtm += dPriceBid * m_nQtyDailyTodayBuy - m_dPosDailyTodayBuy;
			}
			else
				return;
		}

		if(m_nQtyDailyTodaySell > BAD_LONG_VALUE)
		{
			if((dPriceBid > 0 /*BAD_DOUBLE_VALUE*/ || dPriceAsk > 0 /*BAD_DOUBLE_VALUE*/ || ReplaceWithZero ) && m_dPosDailyTodaySell > BAD_DOUBLE_VALUE)
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
void CMmRvPosAtom::_CalcPnlTheo(VARIANT_BOOL bIsPnlLTD,DATE _dtCalculationDate)
{

	DATE dtCurr = EgLib::vt_date::GetCurrentDate ( true ) ;
	_dtCalculationDate  = (DWORD)_dtCalculationDate ;
	m_dPnlTheo = BAD_DOUBLE_VALUE;
	
	bool use_mid_market = true;
	double theo_price_bid = (use_mid_market ? m_pQuote->m_dPriceTheo : m_pQuote->m_dPriceTheoBid);
	double theo_price_ask = (use_mid_market ? m_pQuote->m_dPriceTheo : m_pQuote->m_dPriceTheoAsk);

	
	if(bIsPnlLTD /*|| dtCurr != _dtCalculationDate*/ )
	{
		if(m_nQtyLTDBuy > BAD_LONG_VALUE)
		{
			if(m_dPosLTDBuy > BAD_DOUBLE_VALUE)
				m_dPnlTheo = theo_price_bid * m_nQtyLTDBuy - m_dPosLTDBuy;
			else
				return;
		}

		if(m_nQtyLTDSell > BAD_LONG_VALUE)
		{
			if(m_dPosLTDSell > BAD_DOUBLE_VALUE)
			{
				if(m_dPnlTheo <= BAD_DOUBLE_VALUE) m_dPnlTheo = 0.;
				m_dPnlTheo += theo_price_ask * m_nQtyLTDSell - m_dPosLTDSell;
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
				m_dPnlTheo = theo_price_bid * m_nQtyDailyPrevDateBuy - m_dPosDailyPrevDateBuy;
			else
				return;
		}

		if(m_nQtyDailyPrevDateSell > BAD_LONG_VALUE)
		{
			if(m_dPosDailyPrevDateSell > BAD_DOUBLE_VALUE)
			{
				if(m_dPnlTheo <= BAD_DOUBLE_VALUE) m_dPnlTheo = 0.;
				m_dPnlTheo += theo_price_ask * m_nQtyDailyPrevDateSell - m_dPosDailyPrevDateSell;
			}
			else
				return;
		}

		if(m_nQtyDailyTodayBuy > BAD_LONG_VALUE)
		{
			if(m_dPosDailyTodayBuy > BAD_DOUBLE_VALUE)
			{
				if(m_dPnlTheo <= BAD_DOUBLE_VALUE) m_dPnlTheo = 0.;
				m_dPnlTheo += theo_price_bid * m_nQtyDailyTodayBuy - m_dPosDailyTodayBuy;
			}
			else
				return;
		}

		if(m_nQtyDailyTodaySell > BAD_LONG_VALUE)
		{
			if(m_dPosDailyTodaySell > BAD_DOUBLE_VALUE)
			{
				if(m_dPnlTheo <= BAD_DOUBLE_VALUE) m_dPnlTheo = 0.;
				m_dPnlTheo += theo_price_ask * m_nQtyDailyTodaySell - m_dPosDailyTodaySell;
			}
			else
				return;
		}
	}
};
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

STDMETHODIMP CMmRvPosAtom::GetOpositOption(BSTR*pVal)
{
	try
	{
		if(!pVal)
		{
			return E_POINTER;
		}

		if ( !m_bstrSymbolOposit.length() )
		{

			CString strTmp((LPCTSTR)m_bstrSymbol) ;
			*pVal = L"" ;
			long lOffset = 2 ;
			char chSymZv = strTmp.GetAt ( strTmp.GetLength() - 1  ) ;
			if ( chSymZv == '*' )
			{
				lOffset = 3 ;
			}
			if ( strTmp.GetLength() > 2 )
			{
				long lPos = strTmp.GetLength() - lOffset  ;
				char chSym = strTmp.GetAt ( lPos ) ;
				if ( chSym <= 'L' )
					chSym +=12 ;
				else
					chSym -=12 ;
				strTmp.SetAt( lPos , chSym ) ;
				m_bstrSymbolOposit = CComBSTR ( strTmp ) ;
			}
		}
		ObjectLock lock(this);
		*pVal = m_bstrSymbolOposit.copy();
	}
	catch ( ... )
	{
		ATLASSERT ( !"catch ..." ) ;
	}
	return S_OK ;
}

IMmRvPosAtomPtr CMmRvPosAtom::MakeCopy(bool bStaticDataOnly, bool bAddNewQuote)
{
	IMmRvPosAtomPtr spCopy ;
	try
	{
		CComObject<CMmRvPosAtom>* pCopy = NULL;

		HRESULT hr = CComObject<CMmRvPosAtom>::CreateInstance(&pCopy) ;
		if ( SUCCEEDED( hr ) )
		{
			spCopy.Attach(pCopy, TRUE);

			if ( !bAddNewQuote )
			{
				pCopy->m_pQuote  =  m_pQuote;
				pCopy->m_spQuote =  m_spQuote;
			}

			pCopy->InitSyntAndFut			( m_spSynthGreeks  , m_spFut, m_spActiveFuture ) ;

			pCopy->m_nID			= m_nID;
			pCopy->m_enContractType = m_enContractType;
			pCopy->m_bstrSymbol		= m_bstrSymbol;

			pCopy->m_enOptType		= m_enOptType;
			pCopy->m_dtExpiry		= m_dtExpiry;
			pCopy->m_dtExpiryOV		= m_dtExpiryOV;
			pCopy->m_dtTradingClose = m_dtTradingClose;
			pCopy->m_dStrike		= m_dStrike;
			pCopy->m_dRate			= m_dRate;
			pCopy->m_dVegaWeight	= m_dVegaWeight;
			pCopy->m_nOptionRootID	= m_nOptionRootID;

			pCopy->m_bVisible		= m_bVisible;
			pCopy->m_bCalcGreeks    = m_bCalcGreeks;
			pCopy->m_bstrImportId	= m_bstrImportId;

			pCopy->m_dTimeValue		= m_dTimeValue;
			pCopy->m_bIsSynthetic	= m_bIsSynthetic;
			pCopy->m_bVisible       = m_bVisible;  

			pCopy->m_nUndID			= m_nUndID;

			if(!bStaticDataOnly)
			{
				pCopy->m_nQty			= m_nQty;
				pCopy->m_nQtyInShares	= m_nQtyInShares;

				pCopy->m_nQtyLTDBuy		= m_nQtyLTDBuy;
				pCopy->m_nQtyLTDSell	= m_nQtyLTDSell;
				pCopy->m_dPosLTDBuy     = m_dPosLTDBuy;
				pCopy->m_dPosLTDSell	= m_dPosLTDSell;

				pCopy->m_nQtyDailyPrevDateBuy	= m_nQtyDailyPrevDateBuy;
				pCopy->m_nQtyDailyPrevDateSell	= m_nQtyDailyPrevDateSell  ;
				pCopy->m_dPosDailyPrevDateBuy	= m_dPosDailyPrevDateBuy  ;
				pCopy->m_dPosDailyPrevDateSell	= m_dPosDailyPrevDateSell ;

				pCopy->m_nQtyDailyTodayBuy	= m_nQtyDailyTodayBuy  ;
				pCopy->m_nQtyDailyTodaySell	= m_nQtyDailyTodaySell  ;
				pCopy->m_dPosDailyTodayBuy	= m_dPosDailyTodayBuy  ;
				pCopy->m_dPosDailyTodaySell	= m_dPosDailyTodaySell  ;

				pCopy->m_dPnlMtm			= m_dPnlMtm ;
				pCopy->m_dPnlTheo			= m_dPnlTheo ;

				pCopy->m_dDeltaInShares		= m_dDeltaInShares ;
				pCopy->m_dGammaInShares		= m_dGammaInShares ;
				pCopy->m_dGammaInSharesPerc	= m_dGammaInSharesPerc ;
				pCopy->m_dVegaInShares		= m_dVegaInShares  ;
				pCopy->m_dWtdVega			= m_dWtdVega  ;
				pCopy->m_dThetaInShares		= m_dThetaInShares ;
				pCopy->m_dRhoInShares		= m_dRhoInShares;
				pCopy->m_dThetaDeltaInShares= m_dThetaDeltaInShares ;
				pCopy->m_dThetaGammaInShares= m_dThetaGammaInShares ;
				pCopy->m_dVegaDeltaInShares	= m_dVegaDeltaInShares ;
				pCopy->m_dVegaGammaInShares	= m_dVegaGammaInShares;

				pCopy->m_dNetGamma				= m_dNetGamma;
				pCopy->m_bBadNetGamma			= m_bBadNetGamma;
			}
		}
		else
		{
			ATLASSERT ( !"Can't generate object" ) ;
		}
	}
	catch ( ... )
	{
		ATLASSERT ( !"catch ..." ) ;
	}
	return spCopy;
}

STDMETHODIMP CMmRvPosAtom::GetCopyOfObject(VARIANT_BOOL bGetCopyOpQuote , IMmRvPosAtom**ppCopy)
{

	try
	{
		IMmRvPosAtomPtr spCopy = MakeCopy ( false , bGetCopyOpQuote = VARIANT_TRUE ? true : false ) ;
		if(spCopy!=NULL)
			*ppCopy = spCopy.Detach();
	}
	catch ( ... )
	{
		ATLASSERT ( !"catch ..." ) ;
	}
	return S_OK ;
}

STDMETHODIMP CMmRvPosAtom::InitSyntAndFut ( IMmRvSynthGreeksColl* _pSynth, IMmRvFutAtom* _pFut, IMmRvFutAtom* _pActiveFuture ) 
{
	m_spFut				= _pFut ;
	m_spSynthGreeks		= _pSynth ;
	m_spActiveFuture = _pActiveFuture;
	return S_OK ;
}

STDMETHODIMP CMmRvPosAtom::GetFormatedXMLForDBUpdate(LONG vUndID, BSTR*pVal)
{	
	try
	{
		if(*pVal)
			::SysFreeString(*pVal);

		CHAR str[500] ={0};
		_bstr_t bsvt(_variant_t(m_dtExpiry, VT_DATE));

		_snprintf_s ( str, sizeof(str), 400 , "<Vl UndID=\"%i\" Strike=\"%f\" Expiry=\"%s\" Vola=\"%f\"/>", vUndID, m_dStrike, (LPCSTR)bsvt, m_pQuote->m_dImpliedVola) ;

		*pVal = SysAllocString(CA2W(str));
	}
	catch ( ... )
	{
		ATLASSERT ( "catch ..." ) ;
	}
	return S_OK ;
}

STDMETHODIMP CMmRvPosAtom::IsOTM ( DOUBLE dUndPriceTolerance, EtsPriceRoundingRuleEnum enPriceRoundingRule , VARIANT_BOOL*pVal )
{	
	DOUBLE dUndMidPrice = m_pQuote->m_dVolaCalcPrice ;
	if ( !pVal )
		return E_POINTER ;
	*pVal = VARIANT_TRUE ;
	if ( m_enOptType == enOtPut && dUndMidPrice  <= m_dStrike  )
		*pVal = VARIANT_FALSE ;
	if ( m_enOptType == enOtCall && dUndMidPrice >= m_dStrike )
		*pVal = VARIANT_FALSE ;
	return S_OK ;
}

STDMETHODIMP CMmRvPosAtom::DistanceToStrike(DOUBLE _Price,DOUBLE*pDistance)
{

	if ( !pDistance )
		return E_POINTER ;

	if ( _Price > 0 )
		*pDistance  = _Price - m_dStrike ;
	else
		*pDistance = BAD_DOUBLE_VALUE ;
	return S_OK ;
}

void CMmRvPosAtom::SimpleCopyForETS_VME(CComObject<CMmRvPosAtom>* pCopy)
{

	pCopy->InitSyntAndFut			( m_spSynthGreeks  , m_spFut, m_spActiveFuture ) ;

	pCopy->m_nID			= m_nID;
	pCopy->m_enContractType = m_enContractType;
	pCopy->m_bstrSymbol		= m_bstrSymbol;

	pCopy->m_enOptType		= m_enOptType;
	pCopy->m_dtExpiry		= m_dtExpiry;
	pCopy->m_dtExpiryOV		= m_dtExpiryOV;
	pCopy->m_dtTradingClose = m_dtTradingClose;
	pCopy->m_dStrike		= m_dStrike;
	pCopy->m_dRate			= m_dRate;
	pCopy->m_dVegaWeight	= m_dVegaWeight;
	pCopy->m_nOptionRootID	= m_nOptionRootID;

	pCopy->m_bVisible		= m_bVisible;
	pCopy->m_bCalcGreeks    = m_bCalcGreeks;
	pCopy->m_bstrImportId	= m_bstrImportId;

	pCopy->m_dTimeValue		= m_dTimeValue;
	pCopy->m_bIsSynthetic	= m_bIsSynthetic;

	pCopy->m_nUndID			= m_nUndID;

	{
		pCopy->m_nQty			= m_nQty;
		pCopy->m_nQtyInShares	= m_nQtyInShares;

		pCopy->m_nQtyLTDBuy		= m_nQtyLTDBuy;
		pCopy->m_nQtyLTDSell	= m_nQtyLTDSell;
		pCopy->m_dPosLTDBuy     = m_dPosLTDBuy;
		pCopy->m_dPosLTDSell	= m_dPosLTDSell;

		pCopy->m_nQtyDailyPrevDateBuy	= m_nQtyDailyPrevDateBuy;
		pCopy->m_nQtyDailyPrevDateSell	= m_nQtyDailyPrevDateSell  ;
		pCopy->m_dPosDailyPrevDateBuy	= m_dPosDailyPrevDateBuy  ;
		pCopy->m_dPosDailyPrevDateSell	= m_dPosDailyPrevDateSell ;

		pCopy->m_nQtyDailyTodayBuy	= m_nQtyDailyTodayBuy  ;
		pCopy->m_nQtyDailyTodaySell	= m_nQtyDailyTodaySell  ;
		pCopy->m_dPosDailyTodayBuy	= m_dPosDailyTodayBuy  ;
		pCopy->m_dPosDailyTodaySell	= m_dPosDailyTodaySell  ;

		pCopy->m_dPnlMtm			= m_dPnlMtm ;
		pCopy->m_dPnlTheo			= m_dPnlTheo ;

		pCopy->m_dDeltaInShares		= m_dDeltaInShares ;
		pCopy->m_dGammaInShares		= m_dGammaInShares ;
		pCopy->m_dGammaInSharesPerc	= m_dGammaInSharesPerc ;
		pCopy->m_dVegaInShares		= m_dVegaInShares  ;
		pCopy->m_dWtdVega			= m_dWtdVega  ;
		pCopy->m_dThetaInShares		= m_dThetaInShares ;
		pCopy->m_dRhoInShares		= m_dRhoInShares;
		pCopy->m_dThetaDeltaInShares= m_dThetaDeltaInShares ;
		pCopy->m_dThetaGammaInShares= m_dThetaGammaInShares ;
		pCopy->m_dVegaDeltaInShares	= m_dVegaDeltaInShares ;
		pCopy->m_dVegaGammaInShares	= m_dVegaGammaInShares;

		pCopy->m_dNetGamma				= m_dNetGamma;
		pCopy->m_bBadNetGamma			= m_bBadNetGamma;
	}
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

	double dPrice = BAD_DOUBLE_VALUE;

	switch( iLocalDataID ) {
			case CV_DP_DATA_POSBID :
				VariantInit( &vtValue );
				vtValue.vt = VT_R8;
				dPrice = m_pQuote->m_pPrice->m_dPriceBid;
				vtValue.dblVal = ( ( BAD_DOUBLE_VALUE == dPrice )  ? 0.0 : dPrice );
				VariantCopy( Value, &vtValue );
				break;
			case CV_DP_DATA_POSASK :
				VariantInit( &vtValue );
				vtValue.vt = VT_R8;
				dPrice = m_pQuote->m_pPrice->m_dPriceAsk;
				vtValue.dblVal = ( ( BAD_DOUBLE_VALUE == dPrice)  ? 0.0 : dPrice);
				VariantCopy( Value, &vtValue );
				break;
			case CV_DP_DATA_POSLAST:
				VariantInit( &vtValue );
				vtValue.vt = VT_R8;
				dPrice = m_pQuote->m_pPrice->m_dPriceLast;
				vtValue.dblVal = ( ( BAD_DOUBLE_VALUE == dPrice)  ? 0.0 : dPrice);
				VariantCopy( Value, &vtValue );
				break;
			case CV_DP_DATA_POSMARK:
				VariantInit( &vtValue );
				vtValue.vt = VT_R8;
				dPrice = m_pQuote->m_pPrice->m_dPriceClose;
				vtValue.dblVal = ( ( BAD_DOUBLE_VALUE == dPrice)  ? 0.0 : dPrice);
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
				vtValue.dblVal = ( ( BAD_DOUBLE_VALUE == m_dTimeValue)  ? 0.0 :m_dTimeValue);
				VariantCopy( Value, &vtValue );
				break;
			case CV_DP_DATA_POSVEGA:
				VariantInit( &vtValue );
				vtValue.vt = VT_R8;
				vtValue.dblVal = ( ( BAD_DOUBLE_VALUE == m_pQuote->m_dVega)  ? 0.0 : m_pQuote->m_dVega*100);
				VariantCopy( Value, &vtValue );
				break;
			case CV_DP_DATA_POSDELTA:
				VariantInit( &vtValue );
				vtValue.vt = VT_R8;
				vtValue.dblVal = ( ( BAD_DOUBLE_VALUE == m_pQuote->m_dDelta)  ? 0.0 : m_pQuote->m_dDelta*100);
				VariantCopy( Value, &vtValue );
				break;
			case CV_DP_DATA_POSCHANGE :
				VariantInit( &vtValue );
				vtValue.vt = VT_R8;
				vtValue.dblVal = ( ( BAD_DOUBLE_VALUE == m_pQuote->m_dNetChange)  ? 0.0 : m_pQuote->m_dNetChange);
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
				vtValue.dblVal = ( ( BAD_DOUBLE_VALUE == m_pQuote->m_nLotSize)  ? 0.0 : m_pQuote->m_nLotSize);
				VariantCopy( Value, &vtValue );
				break;

			case CV_DP_DATA_POSTHETA:
				VariantInit( &vtValue );
				vtValue.vt = VT_R8;
				vtValue.dblVal = ( ( BAD_DOUBLE_VALUE == m_pQuote->m_dTheta)  ? 0.0 : m_pQuote->m_dTheta*100);
				VariantCopy( Value, &vtValue );
				break;
			case CV_DP_DATA_POSRHO:
				VariantInit( &vtValue );
				vtValue.vt = VT_R8;
				vtValue.dblVal = ( ( BAD_DOUBLE_VALUE == m_pQuote->m_dRho)  ? 0.0 : m_pQuote->m_dRho*100);
				VariantCopy( Value, &vtValue );
				break;
			case CV_DP_DATA_POSOPTPRICE:
				VariantInit( &vtValue );
				vtValue.vt = VT_R8;
				dPrice = m_pQuote->m_pPrice->m_dActivePrice;
				vtValue.dblVal = ( ( BAD_DOUBLE_VALUE == dPrice)  ? 0.0 : dPrice);
				VariantCopy( Value, &vtValue );
				break;
			case CV_DP_DATA_POSGAMMA:
				VariantInit( &vtValue );
				vtValue.vt = VT_R8;
				vtValue.dblVal = ( ( BAD_DOUBLE_VALUE == m_pQuote->m_dGamma)  ? 0.0 : m_pQuote->m_dGamma*100);
				VariantCopy( Value, &vtValue );
				break;
			case CV_DP_DATA_POSTHEOVOL:
				VariantInit( &vtValue );
				vtValue.vt = VT_R8;
				vtValue.dblVal = ( ( BAD_DOUBLE_VALUE == m_pQuote->m_dVola)  ? 0.0 : m_pQuote->m_dVola*100);
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
				vtValue.dblVal = ( ( BAD_DOUBLE_VALUE == m_pQuote->m_dPriceTheo)  ? 0.0 : m_pQuote->m_dPriceTheo);
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
				if (m_pQuote->m_pSuPrice)
					dPrice = m_pQuote->m_pSuPrice->m_dActivePrice;
				vtValue.dblVal = ( ( BAD_DOUBLE_VALUE == dPrice)  ? 0.0 : dPrice);
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
				vtValue.dblVal = ( ( BAD_DOUBLE_VALUE == m_pQuote->m_dMIV)  ? 0.0 : m_pQuote->m_dMIV * 100.);
				VariantCopy( Value, &vtValue );
				break;
			case CV_DP_DATA_POSVOLGA:
				VariantInit( &vtValue );
				vtValue.vt = VT_R8;
				vtValue.dblVal = ( ( BAD_DOUBLE_VALUE == BAD_DOUBLE_VALUE/*m_pQuote->m_dVolga*/)  ? 0.0 : /*m_pQuote->m_dVolga **/ 100.0);
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
					//if ( m_simulationParams.m_dDate >= dtCalcDate ) 
					//	dtCalcDate = m_simulationParams.m_dDate;
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
//////////////////////////////////////////////////////////////////////////
STDMETHODIMP
CMmRvPosAtom::Calculate(bool CalcLTDPnL){
	HRESULT hrStatus = S_OK;
	try{

		bool use_mid_market_for_pnl = true;

		_CHK(m_spContract ? S_OK : E_FAIL, 
			_T("Invalid Contract pointer."));

		double pricing_unit = 1.;
		_CHK(m_spContract->raw_GetPricingUnit(&pricing_unit),
			_T("Fail to get pricing unit."));

		double contract_size_in_asset = 1.;
		_CHK(m_spContract->raw_GetContractSizeInAsset(&contract_size_in_asset),
			_T("Fail to get Contract size in asset."));

		double base_contract_price = m_pQuote->m_dBaseContractPrice * pricing_unit;

		double contract_price_bid = m_pQuote->m_dContractPriceBid;
		double contract_price_ask = m_pQuote->m_dContractPriceAsk;
		double contract_price_mid = m_pQuote->m_dContractPrice;

		
		_CalcPnlMtm(CalcLTDPnL,
					use_mid_market_for_pnl ? contract_price_mid : contract_price_bid, 
					use_mid_market_for_pnl ? contract_price_mid : contract_price_ask, 
					false, 0);

		_CalcPnlTheo(CalcLTDPnL, 0);

		if(m_pQuote->m_dDelta > BAD_DOUBLE_VALUE){
			m_dDeltaInShares = m_pQuote->m_dDelta * m_nQty * contract_size_in_asset;
			m_dDeltaEq = m_dDeltaInShares * base_contract_price;
		};

		if(m_pQuote->m_dGamma > BAD_DOUBLE_VALUE){
			m_dGammaInShares		= m_pQuote->m_dGamma * m_nQty * contract_size_in_asset;
			m_dGammaInSharesPerc	= m_pQuote->m_dGamma * m_nQty * contract_size_in_asset * base_contract_price / 100.;
			m_dNetGamma				= m_pQuote->m_dGamma * m_nQty * contract_size_in_asset * base_contract_price * base_contract_price / 100.;
		};

		if(m_pQuote->m_dVega > BAD_DOUBLE_VALUE){
			m_dVegaInShares = m_pQuote->m_dVega * m_nQty * contract_size_in_asset;
			m_dWtdVega = m_dVegaInShares * m_dVegaWeight;
		};

		if(m_pQuote->m_dTheta > BAD_DOUBLE_VALUE){
			m_dThetaInShares =	m_pQuote->m_dTheta * contract_size_in_asset * m_nQty;
		};

		if(m_pQuote->m_dRho > BAD_DOUBLE_VALUE){
			m_dRhoInShares =	m_pQuote->m_dRho * contract_size_in_asset * m_nQty;
		};

		if(m_pQuote->m_dVegaDelta > BAD_DOUBLE_VALUE){
			m_dVegaDeltaInShares =	m_pQuote->m_dVegaDelta * contract_size_in_asset * m_nQty;
		};

		if(m_pQuote->m_dThetaDelta > BAD_DOUBLE_VALUE){
			m_dThetaDeltaInShares = m_pQuote->m_dThetaDelta * contract_size_in_asset * m_nQty;
		};

		if (m_pQuote->m_dThetaGamma > BAD_DOUBLE_VALUE){
			m_dThetaGammaInShares = m_pQuote->m_dThetaGamma * contract_size_in_asset * 
									m_nQty * base_contract_price / 100.;
		};
		
		if (m_pQuote->m_dVegaGamma > BAD_DOUBLE_VALUE){
			m_dVegaGammaInShares =	m_pQuote->m_dVegaGamma * contract_size_in_asset * 
									m_nQty  * base_contract_price / 100.;
		};

		DOUBLE dPrice = 0.;
		if (use_mid_market_for_pnl){
			dPrice = contract_price_mid;
		}
		else{
			dPrice = (m_nQty > 0.) ? contract_price_bid : contract_price_ask;
		}

		m_dTimeValue = max(0., dPrice - m_pQuote->m_dIntrinsicValue);
		
		IContractPtr base_contract = 0;
		_CHK(m_spContract->get_BaseContract(&base_contract), 
			_T("Fail to get base contract."));

		if (!base_contract){
			m_dTimeValue = 0.;
		};

		return hrStatus;
	}
	catch(_com_error&){
		return E_FAIL;
	}
	catch(...){
		return E_FAIL;
	};
	return hrStatus;
};
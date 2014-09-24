// MmRvUndAtom.cpp : Implementation of CMmRvUndAtom+

#include "stdafx.h"
#include "MmRvUndAtom.h"
#include "MmRvUndColl.h"
#include <algorithm>
#include <functional>
#include <boost/bind.hpp>
#include <boost/mem_fn.hpp>

using namespace std;
using namespace boost;

const WCHAR* CMmRvUndAtom::bsMonth[12] ={
	L"Jan",
	L"Feb",
	L"Mar",
	L"Apr",
	L"May",
	L"Jun",
	L"Jul",
	L"Aug",
	L"Sep",
	L"Oct",
	L"Nov",
	L"Dec"
};

void __MmRvUndAtom::AddPosToAggregations( MmRvPosAtomPtr& pPos){
	CComBSTR						bstrFutName;
	_bstr_t						aggName, intName;
	CMmRvAggData*				pAgg, *pAggExp;
	unsigned					cntrAggregations = 0;
	
	if (!pPos) return;

	// first, add position to aggregations of this underlying
	pPos->AggregationsPtr_[cntrAggregations++] = static_cast<CMmRvAggData*>(this);
	intName = Name_;
	// try to find appropriate aggregation by future for this position
	if (pPos->m_spFut){									// if we have future related for this position
		pPos->m_spFut->get_Symbol(&bstrFutName);	// get it name
		aggName = bstrFutName;

		// try to find future aggregation
		Aggregations::iterator itrCrnt = Aggregations_.begin();
		for ( Aggregations::iterator itrEnd = Aggregations_.end(); itrCrnt != itrEnd; ++itrCrnt ) 
			if ( (*itrCrnt)->Name_ == aggName ) 
				break;

		if (itrCrnt == Aggregations_.end() ) {
			// future aggregation for this position hasn't been created yet, do this now
			pAgg = new CMmRvAggData(CMmRvAggData::enFutAgg, aggName );

			pAgg->pFut_ = static_cast<CMmRvFutAtom*>(pPos->m_spFut.GetInterfacePtr());
			pAgg->pUnd_ = static_cast<CMmRvUndAtom*>(this);
			intName = pAgg->internalName_ = intName + aggName;			// underlying symbol + future symbol
			Aggregations_.push_back(MmRvAggDataPtr(pAgg));
		}
		else
			pAgg = (*itrCrnt).get();
		pPos->AggregationsPtr_[cntrAggregations++] = pAgg;
	}
	else{
		//pAgg = static_cast<CMmRvAggData*>(this);	// no related future for this position
		//															// add it directly to underlying or expiry aggregation 
		//															// for this underlying
		Positions_.push_back(pPos);
		return;
	}
	if (pPos->m_dtExpiry ) {		// if position has expiry date  ( i.e. it's a kind of option position )
		// get appropriate expiry aggregation name
		CMmRvUndAtom::Date2Text(pPos->m_dtExpiry, aggName);

		// try to find expiry aggregation for this option position within aggregation pointed by pAgg
		// (it's a future aggregation or underlying itself)
		Aggregations::iterator itrCrnt = pAgg->Aggregations_.begin();
		for ( Aggregations::iterator itrEnd = pAgg->Aggregations_.end(); itrCrnt != itrEnd; ++itrCrnt ) 
			if ( (*itrCrnt)->Name_ == aggName ) 
				break;

		if (itrCrnt == pAgg->Aggregations_.end() ) {
			// expiry aggregation for this position hasn't been created yet, do this now
			pAggExp =  new CMmRvAggData(CMmRvAggData::enExpAgg, aggName );
			pAgg->Aggregations_.push_back(MmRvAggDataPtr(pAggExp));
			pAgg->pUnd_ = static_cast<CMmRvUndAtom*>(this);
			pAggExp->internalName_ = intName + aggName;
			pAgg = pAggExp;
		}
		else
			pAgg = (*itrCrnt).get();
		pPos->AggregationsPtr_[cntrAggregations] = pAgg;
	}

	// add position to appropriate aggregation
	pAgg->Positions_.push_back( pPos );
}
void __MmRvUndAtom::AggregatePosition(CMmRvPosAtom* pPos, double underlyingPtrice,DOUBLE dUndPriceTolerance, EtsPriceRoundingRuleEnum enPriceRoundingRule){

	AggregatePositionQuantity(pPos);
	AggregatePositionDeltas(pPos);
	AggregatePositionGammas(pPos, underlyingPtrice, dUndPriceTolerance, enPriceRoundingRule);
	AggregatePositionPnLs(pPos);
	AggregatePositionVegas(pPos);
	AggregatePositionThetas(pPos);
	AggregatePositionRhos(pPos);
	AggregatePositionTmVal(pPos);
}

void __MmRvUndAtom::AggregatePositionQuantity(CMmRvPosAtom* pPos){
	if ( !IsBadValue(pPos->m_nQty) && pPos->AggregationsPtr_[0] ){
		// if we have quantity in position and where to aggregate it
		for (unsigned i = 0; i < 3 && pPos->AggregationsPtr_[i]; ++i ) {
			switch(pPos->m_enContractType) {
				case enCtFutOption:
				case enCtOption:
					if( IsBadValue(pPos->AggregationsPtr_[i]->OptQty_) )
						pPos->AggregationsPtr_[i]->OptQty_ = 0;
					pPos->AggregationsPtr_[i]->OptQty_ += pPos->m_nQty;		// aggregate options quantity
					break;
				case enCtFuture:
					if( IsBadValue(pPos->AggregationsPtr_[i]->FutQty_) )
						pPos->AggregationsPtr_[i]->FutQty_ = 0;
					pPos->AggregationsPtr_[i]->FutQty_ += pPos->m_nQty;		// aggregate futures quantity
					if (/*pPos->AggregationsPtr_[i]->Type_ != enUndAgg &&*/ pPos->m_pQuote && !IsBadValue(pPos->m_pQuote->m_nLotSize) ) {
						if( IsBadValue(pPos->AggregationsPtr_[i]->UndPos_) )
							pPos->AggregationsPtr_[i]->UndPos_ = 0;
						pPos->AggregationsPtr_[i]->UndPos_ += (pPos->m_nQty * pPos->m_pQuote->m_nLotSize);	// adjust underlying position
					}
					break;
				default:
					/*if (pPos->AggregationsPtr_[i]->Type_ != enUndAgg )*/ {		// do not aggregate in underlying aggregation
						if( IsBadValue(pPos->AggregationsPtr_[i]->UndPos_) )
							pPos->AggregationsPtr_[i]->UndPos_ = 0;
						pPos->AggregationsPtr_[i]->UndPos_ += pPos->m_nQty;		// aggregate underlying quantity
					}
					break;
			}
		}
	}
}

void __MmRvUndAtom::AggregatePositionDeltas(CMmRvPosAtom* pPos){
	if ( !IsBadValue(pPos->m_dDeltaInShares) && pPos->AggregationsPtr_[0] && pPos->m_dDeltaInShares){
		// if we have delta in position and where to aggregate it
		for (unsigned i = 0; i < 3 && pPos->AggregationsPtr_[i]; ++i ) {
			// aggregate option delta

			EtsContractTypeEnum contract_type = pPos->m_enContractType;
			if ( contract_type == enCtOption ||
				 contract_type == enCtFutOption ){

				if (IsBadValue(pPos->AggregationsPtr_[i]->OptDlt_) ) 
					pPos->AggregationsPtr_[i]->OptDlt_ = 0.;
				pPos->AggregationsPtr_[i]->OptDlt_ += pPos->m_dDeltaInShares;

				if (IsBadValue(pPos->AggregationsPtr_[i]->OptDlt$_) ) 
					pPos->AggregationsPtr_[i]->OptDlt$_ = 0.;
				pPos->AggregationsPtr_[i]->OptDlt$_ += pPos->m_dDeltaEq;
			};
			
			if (IsBadValue(pPos->AggregationsPtr_[i]->NetDlt_) ) 
				pPos->AggregationsPtr_[i]->NetDlt_ = 0.;
			pPos->AggregationsPtr_[i]->NetDlt_ += pPos->m_dDeltaInShares;

			if (IsBadValue(pPos->AggregationsPtr_[i]->NetDlt$_) ) 
				pPos->AggregationsPtr_[i]->NetDlt$_ = 0.;
			pPos->AggregationsPtr_[i]->NetDlt$_ += pPos->m_dDeltaEq;
	
			if (IsBadValue(pPos->AggregationsPtr_[i]->NetExposure_) ) 
				pPos->AggregationsPtr_[i]->NetExposure_ = 0.;
			pPos->AggregationsPtr_[i]->NetExposure_ += pPos->m_dNetExposure;
		}
	}
}

void __MmRvUndAtom::AggregatePositionGammas(CMmRvPosAtom* pPos, double underlyingPtrice, DOUBLE dUndPriceTolerance, EtsPriceRoundingRuleEnum enPriceRoundingRule){
	if ( !IsBadValue(pPos->m_dGammaInShares) && pPos->AggregationsPtr_[0] && pPos->m_dGammaInShares){
		// if we have gamma in position and where to aggregate it
		for (unsigned i = 0; i < 3 && pPos->AggregationsPtr_[i]; ++i ) {
			// aggregate gamma1%
			//double undPrice;
			if (IsBadValue(pPos->AggregationsPtr_[i]->Gma1P_) ) 
				pPos->AggregationsPtr_[i]->Gma1P_ = 0.;
			if(m_enContractType == enCtFutUnd && pPos){
				//EtsReplacePriceStatusEnum penPriceStatus;	VARIANT_BOOL dummy;
				CMmRvFutAtom* pFut = static_cast<CMmRvFutAtom*>(pPos->m_spFut.GetInterfacePtr() );
				if( pFut ){
					try{	
						// NOTE: pPos->m_dGammaInShares already multiplied by FutureContractSize
						// if this needed
						//pFut->GetFuturePrice(dUndPriceTolerance, enPriceRoundingRule, &penPriceStatus, &dummy, &undPrice ); 	
						//if (IsBadValue(undPrice) || !undPrice)
						//	continue;
						// aggregate gamma1%
						pPos->AggregationsPtr_[i]->Gma1P_ += pPos->m_dGammaInSharesPerc;
						// aggregate net gamma$
						//if (!IsBadValue(undPrice) && undPrice && pPos->m_pQuote) {
							if (IsBadValue(pPos->AggregationsPtr_[i]->NetGma$_) ) 
								pPos->AggregationsPtr_[i]->NetGma$_ = 0.;
							pPos->AggregationsPtr_[i]->NetGma$_ += pPos->m_dNetGamma;
						//}
						// aggregate gamma 1$
						if (IsBadValue(pPos->AggregationsPtr_[i]->Gma1$_) )
							pPos->AggregationsPtr_[i]->Gma1$_ = 0.;
						pPos->AggregationsPtr_[i]->Gma1$_+= pPos->m_dGammaInShares;
					}
					catch(...){}
				}
			}
			else{
				// aggregate gamma 1$
				if (IsBadValue(pPos->AggregationsPtr_[i]->Gma1$_) )
					pPos->AggregationsPtr_[i]->Gma1$_ = 0.;
				pPos->AggregationsPtr_[i]->Gma1$_ += pPos->m_dGammaInShares;

				//if (IsBadValue(underlyingPtrice) || !underlyingPtrice)
				//	continue;

				// aggregate gamma1%
				pPos->AggregationsPtr_[i]->Gma1P_ += pPos->m_dGammaInSharesPerc;
				
				// aggregate net gamma$
				if (IsBadValue(pPos->AggregationsPtr_[i]->NetGma$_) ) 
					pPos->AggregationsPtr_[i]->NetGma$_ = 0.;
				pPos->AggregationsPtr_[i]->NetGma$_ += pPos->m_dNetGamma;
			}
		}
	}
}

void __MmRvUndAtom::AggregatePositionPnLs(CMmRvPosAtom* pPos){
	for (unsigned i = 0; i < 3 && pPos->AggregationsPtr_[i]; ++i ) {
		bool  goodTheo = false, goodMTM = false;
		if ( !IsBadValue(pPos->m_dPnlMtm) /*&& DoubleNEQZero(pPos->m_dPnlMtm)*/ ) {
			if (IsBadValue(pPos->AggregationsPtr_[i]->PnlMTM_) )
				pPos->AggregationsPtr_[i]->PnlMTM_ = 0.;
			pPos->AggregationsPtr_[i]->PnlMTM_ += pPos->m_dPnlMtm;
			goodMTM = true;
		}
		if ( !IsBadValue(pPos->m_dPnlTheo) /*&& DoubleNEQZero(pPos->m_dPnlTheo)*/ ) {
			if (IsBadValue(pPos->AggregationsPtr_[i]->PnLTheo_) )
				pPos->AggregationsPtr_[i]->PnLTheo_ = 0.;
			pPos->AggregationsPtr_[i]->PnLTheo_ += pPos->m_dPnlTheo;
			goodTheo = true;
		}
		

		double tmpTheo = IsBadValue(pPos->AggregationsPtr_[i]->PnLTheo_) ? 0. : pPos->AggregationsPtr_[i]->PnLTheo_;
		double tmpMTM = IsBadValue(pPos->AggregationsPtr_[i]->PnlMTM_) ? 0. : pPos->AggregationsPtr_[i]->PnlMTM_;

		if ( goodTheo && goodMTM ) {
			if (IsBadValue(pPos->AggregationsPtr_[i]->PnlEdge_) )
				pPos->AggregationsPtr_[i]->PnlEdge_ = 0.;
			pPos->AggregationsPtr_[i]->PnlEdge_ += (pPos->m_dPnlTheo /*tmpTheo*/- pPos->m_dPnlMtm/*tmpMTM*/);
		}
	}
}
void __MmRvUndAtom::AggregatePositionVegas(CMmRvPosAtom* pPos)
{
	for (unsigned i = 0; i < 3 && pPos->AggregationsPtr_[i]; ++i ) 
	{
		// aggregate Vega$
		// NOTE: pPos->m_dVegaInShares is already Vega * OptQty * (* CSz if !enCtFutUnd) * (* K if == enCtFutUnd)
		if (!IsBadValue(pPos->m_dVegaInShares))
		{
			if (IsBadValue(pPos->AggregationsPtr_[i]->Vga$_)) 
				pPos->AggregationsPtr_[i]->Vga$_= 0.;
			pPos->AggregationsPtr_[i]->Vga$_ += pPos->m_dVegaInShares;
		}

		// aggregate WtdVga
		// NOTE: pPos->m_dWtdVega is already (Weight of Vega) * Vega * OptQty * (* CSz if !enCtFutUnd) * (* K if == enCtFutUnd)
		if (!IsBadValue(pPos->m_dWtdVega))
		{
			if (IsBadValue(pPos->AggregationsPtr_[i]->WtdVga$_)) 
				pPos->AggregationsPtr_[i]->WtdVga$_= 0.;
			pPos->AggregationsPtr_[i]->WtdVga$_ += pPos->m_dWtdVega;
		}

		// aggregate VegaGma
		if (!IsBadValue(pPos->m_dVegaGammaInShares))
		{
			if (IsBadValue(pPos->AggregationsPtr_[i]->VgaGma_)) 
				pPos->AggregationsPtr_[i]->VgaGma_= 0.;
			pPos->AggregationsPtr_[i]->VgaGma_ += pPos->m_dVegaGammaInShares;
		}

		// aggregate VegaDlt
		if (!IsBadValue(pPos->m_dVegaDeltaInShares))
		{
			if (IsBadValue(pPos->AggregationsPtr_[i]->VgaDlt_)) 
				pPos->AggregationsPtr_[i]->VgaDlt_ = 0.;
			pPos->AggregationsPtr_[i]->VgaDlt_ += pPos->m_dVegaDeltaInShares;
		}
	}
}
void __MmRvUndAtom::AggregatePositionThetas(CMmRvPosAtom* pPos)
{
	for (unsigned i = 0; i < 3 && pPos->AggregationsPtr_[i]; ++i ) 
	{
		// aggregate Theta$
		// NOTE: pPos->m_dThetaInShares is already Theta * OptQty * (* CSz if !enCtFutUnd) * (* K if == enCtFutUnd)
		if(!IsBadValue(pPos->m_dThetaInShares))
		{
			if (IsBadValue(pPos->AggregationsPtr_[i]->Tht$_)) 
				pPos->AggregationsPtr_[i]->Tht$_= 0.;
			pPos->AggregationsPtr_[i]->Tht$_ += pPos->m_dThetaInShares;
		}

		// aggregate ThtDlt
		if(!IsBadValue(pPos->m_dThetaDeltaInShares))
		{
			if (IsBadValue(pPos->AggregationsPtr_[i]->ThtDlt_)) 
				pPos->AggregationsPtr_[i]->ThtDlt_= 0.;
			pPos->AggregationsPtr_[i]->ThtDlt_ += pPos->m_dThetaDeltaInShares;
		}

		// aggregate ThtGma
		if(!IsBadValue(pPos->m_dThetaGammaInShares))
		{
			if (IsBadValue(pPos->AggregationsPtr_[i]->ThtGma_)) 
				pPos->AggregationsPtr_[i]->ThtGma_= 0.;
			pPos->AggregationsPtr_[i]->ThtGma_ += pPos->m_dThetaGammaInShares;
		}
	}
}
void __MmRvUndAtom::AggregatePositionRhos(CMmRvPosAtom* pPos)
{
	for (unsigned i = 0; i < 3 && pPos->AggregationsPtr_[i]; ++i ) 
	{
		// aggregate Rho$
		// NOTE: pPos->m_dRhoInShares is already Rho * OptQty * (* CSz if !enCtFutUnd) * (* K if == enCtFutUnd)
		if (!IsBadValue(pPos->m_dRhoInShares))
		{
			if (IsBadValue(pPos->AggregationsPtr_[i]->Rho$_))
				pPos->AggregationsPtr_[i]->Rho$_= 0.;
			pPos->AggregationsPtr_[i]->Rho$_ += pPos->m_dRhoInShares;
		}
	}
}
void __MmRvUndAtom::AggregatePositionTmVal(CMmRvPosAtom* pPos)
{
	for (unsigned i = 0; i < 3 && pPos->AggregationsPtr_[i]; ++i ) {
		/*if(m_enContractType == enCtFutUnd)
		{}
		else*/
		if (!IsBadValue(pPos->m_dTimeValue))	{
			if (IsBadValue(pPos->AggregationsPtr_[i]->TmVal_))
				pPos->AggregationsPtr_[i]->TmVal_= 0.;
			pPos->AggregationsPtr_[i]->TmVal_ += pPos->m_dTimeValue;
			if (IsBadValue(pPos->AggregationsPtr_[i]->TmValInShares_))
				pPos->AggregationsPtr_[i]->TmValInShares_= 0.;
			pPos->AggregationsPtr_[i]->TmValInShares_ += (pPos->m_dTimeValue*pPos->m_nQtyInShares);
		}
	}
}
//----------------------------------------------------------------------------------------------//
STDMETHODIMP CMmRvUndAtom::UpdateQuotes()
{
	try
	{
		if (m_spContract != NULL)
		{
			QuoteUpdateInfo retQuotes;
			_CHK(m_spContract->raw_GetQuotes(&retQuotes), _T("Fail to get contract quotes."));

			if (retQuotes.BidPrice > BAD_DOUBLE_VALUE)
				m_pPrice->put_Bid(retQuotes.BidPrice);

			if (retQuotes.AskPrice > BAD_DOUBLE_VALUE)
				m_pPrice->put_Ask(retQuotes.AskPrice);

			if (retQuotes.LastPrice > BAD_DOUBLE_VALUE)
				m_pPrice->put_Last(retQuotes.LastPrice);

			if (retQuotes.NetChange > BAD_DOUBLE_VALUE)
				m_pPrice->put_NetChange(retQuotes.NetChange);

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
//----------------------------------------------------------------------------------------------//
STDMETHODIMP CMmRvUndAtom::UpdateRisks(){
	//try{
	//	
	//	IRisks retRisks;

	//	if (m_spContract != NULL){

	//		MarketSimulationScenario simulation_scenario;
	//		_CHK(m_spContract->raw_Calculate(NULL, &simulation_scenario, &retRisks),
	//			_T("Fail to calculate contract."));

	//		_CHK(m_spContract->raw_GetRisks(&retRisks),
	//			_T("Fail to get contract risks."));

	//		if ((retRisks.TheoPrice > BAD_DOUBLE_VALUE)&&(retRisks.TheoPrice < 0))
	//			retRisks.TheoPrice = 0;

	//		if ( _finite(retRisks.Delta) )
	//			m_pQuote->m_dPriceTheo = retRisks.TheoPrice;

	//		if ( _finite(retRisks.Delta) )
	//			m_pQuote->m_dDelta = retRisks.Delta;

	//		if ( _finite(retRisks.Gamma) )
	//			m_pQuote->m_dGamma = retRisks.Gamma;

	//		if ( _finite(retRisks.Theta) )
	//			m_pQuote->m_dTheta = retRisks.Theta;

	//		if ( _finite(retRisks.Vega) )
	//			m_pQuote->m_dVega = retRisks.Vega;

	//		if ( _finite(retRisks.Rho) )
	//			m_pQuote->m_dRho = retRisks.Rho;

	//		if ( _finite(retRisks.DeltaVega) )
	//			m_pQuote->m_dVegaDelta = retRisks.DeltaVega;

	//		if ( _finite(retRisks.GammaVega) )
	//			m_pQuote->m_dVegaGamma = retRisks.GammaVega;

	//		if ( _finite(retRisks.DeltaTheta) )
	//			m_pQuote->m_dThetaDelta = retRisks.DeltaTheta;

	//		if ( _finite(retRisks.GammaTheta) )
	//			m_pQuote->m_dThetaGamma = retRisks.GammaTheta;

	//		if ( _finite(retRisks.IVola) )
	//			m_pQuote->m_dImpliedVola = retRisks.IVola;

	//		if ( _finite(retRisks.TheoVola) )
	//			m_pQuote->m_dVola = retRisks.TheoVola;

	//		if ( _finite(retRisks.TheoPriceBid) )
	//			m_pQuote->m_dPriceTheoBid = retRisks.TheoPriceBid;

	//		if ( _finite(retRisks.TheoPriceAsk) )
	//			m_pQuote->m_dPriceTheoAsk = retRisks.TheoPriceAsk;

	//		if ( _finite(retRisks.ContractPrice) )
	//			m_pQuote->m_dContractPrice = retRisks.ContractPrice;

	//		if ( _finite(retRisks.ContractPriceBid) )
	//			m_pQuote->m_dContractPriceBid = retRisks.ContractPriceBid;

	//		if ( _finite(retRisks.ContractPriceAsk) )
	//			m_pQuote->m_dContractPriceAsk = retRisks.ContractPriceAsk;

	//		if ( _finite(retRisks.BaseContractPrice) )
	//			m_pQuote->m_dBaseContractPrice = retRisks.BaseContractPrice;

	//		if ( _finite(retRisks.BaseContractPriceBid) )
	//			m_pQuote->m_dBaseContractPriceBid = retRisks.BaseContractPriceBid;

	//		if ( _finite(retRisks.BaseContractPriceAsk) )
	//			m_pQuote->m_dBaseContractPriceAsk = retRisks.BaseContractPriceAsk;

	//		m_pQuote->m_spPrice->put_Active(retRisks.ContractPrice);
	//	}
	//}
	//catch(_com_error& e){
	//	return Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_IMmRvPosAtom, e.Error());
	//}

	return S_OK;
};
//----------------------------------------------------------------------------------------------//
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
				IMmRvAggregationDataAtomPtr spSynthUndData(spSynthUnd);
				if(spSynthUnd != NULL && spSynthUndData!=NULL)
				{
					dWeight = spSRComp->Weight;
					IMmRvPricePtr spPrice;
					_CHK(spSynthUndData->get_Price(&spPrice));

					VARIANT_BOOL	bPriceByDriver = VARIANT_FALSE;
					VARIANT_BOOL	bUseActivePrice = VARIANT_FALSE;
					_CHK(spPrice->get_IsUseManualActive(&bUseActivePrice));

					VARIANT_BOOL	bPriceByHead	=	VARIANT_FALSE;
					spSynthUnd->get_PriceByHead(&bPriceByHead);

					IMmRvUndAtomPtr spHeadComponent;
					spSynthUnd->get_HeadComponent(&spHeadComponent);
					if ((spHeadComponent && bPriceByHead) || bUseActivePrice)
					{
						bPriceByDriver = VARIANT_TRUE;
						spPrice->get_Active(&dPrice);
					}

					if (bPriceByDriver  == VARIANT_FALSE)
						_CHK(spPrice->get_Bid(&dPrice));

					if(!bBadSpotBid && dPrice > DBL_EPSILON)
						*pdSynthBid += dPrice * dWeight;
					else
					{
						bBadSpotBid = true;
						*pdSynthBid = BAD_DOUBLE_VALUE;
					}

					if (bPriceByDriver == VARIANT_FALSE)
						_CHK(spPrice->get_Ask(&dPrice));

					if(!bBadSpotAsk && dPrice > DBL_EPSILON)
						*pdSynthAsk += dPrice * dWeight;
					else
					{
						bBadSpotAsk = true;
						*pdSynthAsk = BAD_DOUBLE_VALUE;
					}

					if (bPriceByDriver  == VARIANT_FALSE)
						_CHK(spPrice->get_Last(&dPrice));

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
HRESULT
CMmRvUndAtom::CalcEx(IMmRvUndColl* aUndColl, 
					 LONG nMask, 
					 VARIANT_BOOL bCalcGreeks, 
					 VARIANT_BOOL bUpdateVola, 
					 VARIANT_BOOL bRealtimeCalc, 
					 VARIANT_BOOL bClearSynthGreeks, 
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

	if(aUndColl == NULL || pParams == NULL)
		return Error(L"Invalid object passed.", IID_IMmRvUndAtom, E_INVALIDARG);

	try{

		UpdateQuotes();

		IMmRvUndCollPtr spUndColl(aUndColl);
		ICalculationParametrsPtr spParams(pParams);

		LONG nOptUpdated = 0L, nUndUpdated = 0L, nFutUpdated = 0L;

		ClearAggregatedValues();
		if(bClearSynthGreeks)
			_ClearSynthGreeks();


		CMmRvPosColl::EnumCollType::iterator itrPos    = m_pPos->m_coll.begin();
		const CMmRvPosColl::EnumCollType::iterator itrPosEnd = m_pPos->m_coll.end();

		m_vecPos.clear();

		for(;itrPos!=itrPosEnd; ++itrPos)
		{
			CMmRvPosAtom* pPos = dynamic_cast<CMmRvPosAtom*>(itrPos->second);

			m_vecPos.push_back(itrPos->second);

			if(pPos != NULL){

				pPos->ClearValues();
				pPos->UpdateRisks(SimulationScenario);
				pPos->UpdateQuotes();

				if(pPos->m_bVisible){

				EtsContractTypeEnum enContractType = pPos->m_enContractType;

				pPos->Calculate(bIsPnlLTD == VARIANT_TRUE);

				m_pPrice->m_dActivePrice = pPos->m_pQuote->m_dBaseContractPrice;
				// options
				if(enContractType == enCtOption || enContractType == enCtFutOption){
					nOptUpdated++;
				}
				else if (enContractType == enCtFuture){
					nFutUpdated++;
				}
				else{
					nUndUpdated++;
				}

				// PnlMtm
				if ( IsBadValue(pPos->m_dPnlMtm)) 
					BadPnlMtm_ = VARIANT_TRUE;
				if ( IsBadValue(pPos->m_dPnlTheo)) 
					BadPnlTheo_ = VARIANT_TRUE;

					// aggregate
					AggregatePosition(pPos, 0., 0., enPrrRoundNone);
				}
			}
		};

		DOUBLE	dBeta = Beta_;
		if (m_spHeadComponent)
			m_spHeadComponent->get_Beta(&dBeta);

		if(NetDlt$_ > BAD_DOUBLE_VALUE && dBeta > BAD_DOUBLE_VALUE && DoubleNEQZero(dBeta)){
			BetaWtdDelta$_ = NetDlt$_ * dBeta;
			if(BadNetDlt$_) BadBetaWtdDelta$_ = VARIANT_TRUE;
		}
		else{
			BadBetaWtdDelta$_ = VARIANT_TRUE;
		}

		if(pnOptUpdated)
			*pnOptUpdated += nOptUpdated;

		if(pnUndUpdated)
			*pnUndUpdated += nUndUpdated;

		if(pnFutUpdated)
			*pnFutUpdated += nFutUpdated;

		m_pPrice->m_bDirty   = VARIANT_FALSE;
		m_pSuPrice->m_bDirty = VARIANT_FALSE;
	}
	catch(const _com_error& e){
		return Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_IMmRvUndAtom, e.Error());
	}
	return S_OK;
};

STDMETHODIMP CMmRvUndAtom::Calc(IMmRvUndColl* aUndColl,
								LONG nMask,
								VARIANT_BOOL bCalcGreeks,
								VARIANT_BOOL bUpdateVola,
								VARIANT_BOOL bRealtimeCalc,
								VARIANT_BOOL bClearSynthGreeks,
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
								ICalculationParametrs* pParams)
{
	 return CalcEx(aUndColl, nMask, bCalcGreeks, bUpdateVola, bRealtimeCalc,
					bClearSynthGreeks, aDelay, bIsPnlLTD, enCalcModel, 
					bUseTheoVolatility, bUseTheoVolaNoBid, bUseTheoVolaBadMarket,
					dUndPriceTolerance, enPriceRoundingRule, pnOptUpdated, 
					pnUndUpdated, pnFutUpdated, dtCalcDate, pParams, NULL);
}

/////////////////////////////////////////////////////////////////////////////
//
void CMmRvUndAtom::_CalcTotalsForOptionPos(CMmRvPosAtom* pPos) throw()
{
	DOUBLE dOptDltEqOnePos;
	CMmRvFutAtom* pFut = static_cast<CMmRvFutAtom*>(pPos->m_spFut.GetInterfacePtr());

	DOUBLE dUndPriceMid = m_pPrice->m_dActivePrice;
	if((pFut && m_enContractType == enCtFutUnd))
		dUndPriceMid = pFut->m_pPrice->m_dActivePrice;

	if ( pPos )
		pPos->m_dUndPriceSave = dUndPriceMid;

	// DeltaInShares
	if(pPos->m_dDeltaInShares > BAD_DOUBLE_VALUE)
	{
		if(dUndPriceMid > DBL_EPSILON)
		{
			
			pPos->m_dInternalEqDelta = pPos->m_dDeltaInShares * dUndPriceMid;

			if(m_dNetExposureAUM >BAD_DOUBLE_VALUE && m_dNetExposureAUM != 0.0)
				pPos->m_dNetExposure = pPos->m_dInternalEqDelta / m_dNetExposureAUM * 100;

			if(m_enContractType != enCtFutUnd)
			{
				pPos->m_dDeltaEq = pPos->m_dInternalEqDelta;
			}
			else
			{
				if(pPos->m_spFut!=NULL)
				{
					CMmRvFutAtom* pFut = static_cast<CMmRvFutAtom*>(pPos->m_spFut.GetInterfacePtr());
					if(pFut && pFut->m_bMultOptDltEq)
						pPos->m_dNetExposure *= pFut->m_nFutLotSize;
				}
			}


			if( m_enContractType == enCtFutUnd)
			{
				dOptDltEqOnePos = pPos->m_dInternalEqDelta * pPos->m_dKEq;
				if (!pPos->m_bMultOptDltEq && pPos->m_pQuote && !IsBadValue(pPos->m_pQuote->m_nLotSize) && pPos->m_pQuote->m_nLotSize) 
					dOptDltEqOnePos /= pPos->m_pQuote->m_nLotSize;
			}
			else
				dOptDltEqOnePos = pPos->m_dInternalEqDelta;
			
		}
		else
		{
			BadOptDlt$_ = VARIANT_TRUE;
			BadOptDlt$_ = VARIANT_TRUE;
		}
	}
	else
	{
		BadNetDelta_ = VARIANT_TRUE;
		BadOptDelta_ = VARIANT_TRUE;
		BadNetDlt$_ = VARIANT_TRUE;
	}

	if(IsBadValue(pPos->m_dGammaInSharesPerc)) /*else*/
	{
		BadGma1P_ = VARIANT_TRUE;
		BadNetGma$_ = VARIANT_TRUE;
	}

	if(IsBadValue(pPos->m_dGammaInShares) )/*else*/
		BadGma1$_ = VARIANT_TRUE;


	if(IsBadValue(pPos->m_dNetGamma))
		BadNetGma_ = VARIANT_TRUE;

	if(IsBadValue(pPos->m_dVegaInShares))
		BadVga$_ = VARIANT_TRUE;


	if(IsBadValue(pPos->m_dWtdVega))
		BadWtdVga$_ = VARIANT_TRUE;


	if(IsBadValue(pPos->m_dThetaInShares))
		BadTht$_ = VARIANT_TRUE;


	if(IsBadValue(pPos->m_dRhoInShares))
		BadRho$_ = VARIANT_TRUE;


	if(IsBadValue(pPos->m_dThetaDeltaInShares))
		BadThtDlt_ = VARIANT_TRUE;

	if(IsBadValue(pPos->m_dThetaGammaInShares))
		BadThtGma_ = VARIANT_TRUE;


	if(IsBadValue(pPos->m_dVegaDeltaInShares))
		BadVgaDlt_ = VARIANT_TRUE;


	if(IsBadValue(pPos->m_dVegaGammaInShares))
		BadVgaGma_ = VARIANT_TRUE;

	// TimeValue

	if(IsBadValue(pPos->m_dTimeValue))
		BadTmVal_ = VARIANT_TRUE;

	VARIANT_BOOL bPosIsSynthetic = pPos->m_bIsSynthetic;
	if(bPosIsSynthetic)
	{
		IMmRvSynthGreeksCollPtr spPosSynthGreeksColl = pPos->m_spSynthGreeks;

		if(spPosSynthGreeksColl != NULL)
		{
			IMmRvSynthGreeksAtomPtr spPosSynthGreeks;
			LONG nSynthUndID = 0L;
			DOUBLE dSelfValue = BAD_DOUBLE_VALUE;

			IUnknownPtr spUnk;
			_variant_t varItem;
			ULONG nFetched = 0L;
			HRESULT hr;

			_CHK(spPosSynthGreeksColl->get__NewEnum(&spUnk));
			IEnumVARIANTPtr spPosSynthGreekEnum(spUnk);
			_CHK(spPosSynthGreekEnum->Reset());
			while((hr = spPosSynthGreekEnum->Next(1L, &varItem, &nFetched)) == S_OK)
			{
				ATLASSERT(varItem.vt == VT_DISPATCH);
				spPosSynthGreeks = varItem;
				if(spPosSynthGreeks != NULL)
				{
					_CHK(spPosSynthGreeks->get_SynthUndID(&nSynthUndID));
					IMmRvSynthGreeksAtomPtr spSynthGreeks;
					_CHK(m_spSynthGreeks->get_Item(nSynthUndID, &spSynthGreeks));

					if(spSynthGreeks != NULL)
					{
						double dPosValue =BAD_DOUBLE_VALUE;

						//NetExp
						_CHK(spPosSynthGreeks->get_NetExposure(&dPosValue));
						if (dPosValue > BAD_DOUBLE_VALUE)
						{
							_CHK(spSynthGreeks->get_NetExposure(&dSelfValue));
							if(dSelfValue <= BAD_DOUBLE_VALUE) dSelfValue = 0.;
							_CHK(spSynthGreeks->put_NetExposure(dSelfValue + dPosValue));
						}
						else
							_CHK(spSynthGreeks->put_BadNetExposure(VARIANT_TRUE));

						//NetDelta
						_CHK(spPosSynthGreeks->get_NetDelta(&dPosValue));
						if (dPosValue > BAD_DOUBLE_VALUE)
						{
							_CHK(spSynthGreeks->get_NetDelta(&dSelfValue));
							if(dSelfValue <= BAD_DOUBLE_VALUE) dSelfValue = 0.;
							_CHK(spSynthGreeks->put_NetDelta(dSelfValue + dPosValue));
						}
						else
							_CHK(spSynthGreeks->put_BadNetDelta(VARIANT_TRUE));

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
}

/////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CMmRvUndAtom::CalcWtdVega(IEtsExpCalColl* aExpCalColl)
{
	if(aExpCalColl == NULL)
		return Error(L"Invalid objects passed.", IID_IMmRvUndAtom, E_INVALIDARG);

	HRESULT hr = S_OK;

	try
	{
		BadWtdVga$_ = VARIANT_FALSE;
		WtdVga$_ = BAD_DOUBLE_VALUE;

		IUnknownPtr spUnk;
		_variant_t varItem;
		ULONG nFetched = 0L;
 		DATE dtToday = vt_date::GetCurrentDate(true);
		IEtsExpCalCollPtr spExpCalColl(aExpCalColl);

		if(m_pPos)
		{
			CMmRvPosColl::CollType::iterator itr = m_pPos->m_collRef.begin();
			CMmRvPosColl::CollType::iterator itrEnd = m_pPos->m_collRef.end();
			for(;itr!=itrEnd;++itr)
			{
				CMmRvPosAtom* pPosAtom = dynamic_cast<CMmRvPosAtom*>(itr->second->second);
				if(pPosAtom)
				{
					EtsContractTypeEnum enContractType = pPosAtom->m_enContractType;

					if(enContractType == enCtOption || enContractType == enCtFutOption)
					{
						DOUBLE dVegaWeight = BAD_DOUBLE_VALUE;
						DATE dtExpiry = pPosAtom->m_dtExpiry;

						dVegaWeight = spExpCalColl->GetVegaWeight(pPosAtom->m_dtExpiryOV);
						pPosAtom->m_dVegaWeight = dVegaWeight;


						VARIANT_BOOL bVisible = pPosAtom->m_bVisible;
						if(bVisible && dtExpiry >= dtToday && dVegaWeight > BAD_DOUBLE_VALUE)
						{
							// VegaInShares							
							if(pPosAtom->m_dVegaInShares > BAD_DOUBLE_VALUE)
							{
								pPosAtom->m_dWtdVega = pPosAtom->m_dVegaInShares * dVegaWeight;

								if(WtdVga$_ <= BAD_DOUBLE_VALUE) WtdVga$_ = 0.;
								WtdVga$_ += pPosAtom->m_dVegaInShares * pPosAtom->m_dVegaInShares;
							}
							else
								BadWtdVga$_ = VARIANT_TRUE;
						}
					}

				}
			}
		}
	}
	catch(const _com_error& e)
	{
		hr = Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_IMmRvUndAtom, e.Error());
	}
	return hr;
}

/////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CMmRvUndAtom::CalcSyntheticUnderlying(	IMmRvUndColl* aUndColl,
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
	if(aUndColl == NULL)
		return Error(L"Invalid objects passed.", IID_IMmRvUndAtom, E_INVALIDARG);

	try
	{
		IUnknownPtr spUnk;
		_variant_t varItem;
		ULONG nFetched = 0L;
		HRESULT hr;

		DOUBLE dUndPriceMid = 0.;
		if(m_spUndPriceProfile != NULL)
		{
			dUndPriceMid = m_spUndPriceProfile->GetUndPriceMid(	m_pPrice->m_dPriceBid,
																m_pPrice->m_dPriceAsk,
																m_pPrice->m_dPriceLast, 
																dUndPriceTolerance,
																enPriceRoundingRule,
																NULL, VARIANT_FALSE);
		};

		IMmRvUndCollPtr spUndColl(aUndColl);
		IMmRvUndCollPtr spUndColl2;
		IMmRvPosAtomPtr spPos;
		IMmRvUndAtomPtr spUnd, spUnd2;

		ICalculationParametrsPtr spParams(pParams);

		_CHK(spUndColl2.CreateInstance(CLSID_MmRvUndColl), _T("Fail to create underlings."));
		if(spUndColl2 == NULL)
			return Error(L"Fail to create underlings.", IID_IMmRvUndAtom, E_INVALIDARG);

		spUndColl2.AddRef();

		_CHK(m_spSynthPos->get__NewEnum(&spUnk));
		IEnumVARIANTPtr spPosEnum(spUnk);
		_CHK(spPosEnum->Reset());
		while((hr = spPosEnum->Next(1L, &varItem, &nFetched)) == S_OK)
		{
			ATLASSERT(varItem.vt == VT_DISPATCH);
			spPos = varItem;
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
							nMask, bIsPnlLTD, enCalcModel,
							bUseTheoVolatility, bUseTheoVolaNoBid, bUseTheoVolaBadMarket,
							dUndPriceTolerance, enPriceRoundingRule,dtCalcDate, spParams));
					}
				}
			}
		}

		_CHK(spUndColl2->get__NewEnum(&spUnk));
		IEnumVARIANTPtr spUndEnum(spUnk);
		_CHK(spUndEnum->Reset());
		while((hr = spUndEnum->Next(1L, &varItem, &nFetched)) == S_OK)
		{
			ATLASSERT(varItem.vt == VT_DISPATCH);
			spUnd = varItem;
			if(spUnd != NULL)
			{
				LONG nUndID = 0L;
				_CHK(spUnd->get_ID(&nUndID));
				if(nUndID != m_nID)
				{
					_CHK(spUnd->Calc(	spUndColl,
										nMask,
										VARIANT_TRUE,
										VARIANT_TRUE,
										VARIANT_FALSE,
										VARIANT_TRUE,
										NULL,
										bIsPnlLTD,
										enCalcModel,
										bUseTheoVolatility,
										bUseTheoVolaNoBid,
										bUseTheoVolaBadMarket,
										dUndPriceTolerance,
										enPriceRoundingRule,
										NULL, NULL, NULL,
										dtCalcDate,
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
	return S_OK;
}

/*
This function returns index( or stock) "mid" price, according to active future or price profile.
Sets bFutureUsed to VARIANT_TRUE if active future was used.
*/

STDMETHODIMP CMmRvUndAtom::GetUnderlyingPrice(DOUBLE dTolerance, EtsPriceRoundingRuleEnum enPriceRound,EtsReplacePriceStatusEnum * penPriceStatus, VARIANT_BOOL *bFutureUsed, DOUBLE *pPrice){
	HRESULT	hr = S_OK;

	if( !pPrice || !bFutureUsed)
		return E_POINTER;
	try{
		*pPrice = 0;
		*bFutureUsed = VARIANT_FALSE;

		if ( m_bPriceByHead && m_spHeadComponent )
		{
			//is this underlying is driven by his asset head component
			m_spHeadComponent->GetUnderlyingPrice(dTolerance, enPriceRound, penPriceStatus, bFutureUsed, pPrice);
			if (*pPrice > .0 && m_dCoeff > .0){
				*pPrice *= m_dCoeff;
				m_pPrice->m_bDirty	=	VARIANT_TRUE;
			}
			*bFutureUsed	=	VARIANT_TRUE;
		}
		else if ( m_spActiveFuture )	
		{
			// this underlying - is index with active future 
			DOUBLE dActiveFutureMid = 0., dActiveFutureBid = 0., dActiveFutureAsk = 0., dActiveFutureLast = 0.;
			DOUBLE dActiveFutureBasis = 0.;
			IEtsPriceProfileAtomPtr spUndPriceProfile;

			m_spActiveFuture->get_UndPriceProfile(&spUndPriceProfile);
			if ( NULL != spUndPriceProfile)
			{
				IMmRvPricePtr spFutPrice;
				_CHK(m_spActiveFuture->get_Price(&spFutPrice));

				_CHK(m_spActiveFuture->get_Basis(&dActiveFutureBasis));
				_CHK(spFutPrice->get_Bid(&dActiveFutureBid) );
				_CHK(spFutPrice->get_Ask(&dActiveFutureAsk) );
				_CHK(spFutPrice->get_Last(&dActiveFutureLast) );

				//get active future price
				dActiveFutureMid = spUndPriceProfile->GetUndPriceMid(dActiveFutureBid, dActiveFutureAsk,
					dActiveFutureLast, dTolerance, enPriceRound, penPriceStatus, VARIANT_FALSE);

				//overwrite active future price if it is manual
				VARIANT_BOOL	bCalcByManual = VARIANT_FALSE;
				spFutPrice->get_IsUseManualActive(&bCalcByManual);
				if (bCalcByManual == VARIANT_TRUE) 
					spFutPrice->get_Active(&dActiveFutureMid);

				//set this future driven underlying price dirty, because of active future price changed
				VARIANT_BOOL	bIsDirty = VARIANT_FALSE;
				spFutPrice->get_IsDirty(&bIsDirty); 
				if (bIsDirty == VARIANT_TRUE)
					m_pPrice->m_bDirty = VARIANT_TRUE;

				if ( dActiveFutureMid > 0.)
				{
					dActiveFutureMid += dActiveFutureBasis;
					*pPrice = dActiveFutureMid;
				}
				else   //if we have no active future market price return 0.0
				{
					*pPrice = 0.0;
				}
				*bFutureUsed = VARIANT_TRUE;
			}
		}
		else if ( m_spUndPriceProfile )
		{
			//asset group head component without driver (active future) or simple Underlying
			if (m_pPrice->m_bManualActive == VARIANT_TRUE){
				*pPrice = m_pPrice->m_dActivePrice;
				*bFutureUsed	=	VARIANT_TRUE;
			}
			else
				*pPrice = m_spUndPriceProfile->GetUndPriceMid(	m_pPrice->m_dPriceBid, 
																m_pPrice->m_dPriceAsk,
																m_pPrice->m_dPriceLast,
																dTolerance, enPriceRound, penPriceStatus, VARIANT_FALSE );
			m_pPrice->m_bDirty	=	VARIANT_TRUE;
		}
	
		m_pPrice->m_dActivePrice = *pPrice;
	}
	catch ( _com_error& e ) {
		hr = Error((PTCHAR)CComErrorWrapper::ErrorDescription(e), __uuidof(IMmRvUndAtom), e.Error());
	}

	return hr;

}


STDMETHODIMP CMmRvUndAtom::FindPosition(DOUBLE dStrike, DATE dtExpiration, EtsOptionTypeEnum enType, IMmRvPosAtom** pPosAtom)
{
	HRESULT hr = S_OK;
	if(!pPosAtom)
		return E_POINTER;
	try
	{
		IMmRvPosAtomPtr spPosAtom;
		if(m_pPos && !m_pPos->m_coll.empty())
		{
			CMmRvPosColl::EnumCollType::iterator itrPos    = m_pPos->m_coll.begin();
			CMmRvPosColl::EnumCollType::iterator itrPosEnd = m_pPos->m_coll.end();
			for(;itrPos!=itrPosEnd; ++itrPos)
			{
				CMmRvPosAtom* pPos = dynamic_cast<CMmRvPosAtom*>(itrPos->second);
				if((pPos->m_enContractType == enCtOption || pPos->m_enContractType == enCtFutOption) 
					&& pPos->m_dStrike == dStrike && pPos->m_enOptType == enType && static_cast<long>(pPos->m_dtExpiry) == static_cast<long>(dtExpiration))
				{
					spPosAtom.Attach(pPos, TRUE);
					break;
				}
			}			
		}
		if(spPosAtom!=NULL)
			*pPosAtom = spPosAtom.Detach();
	}
	catch ( _com_error& e ) {
		hr = Error((PTCHAR)CComErrorWrapper::ErrorDescription(e), __uuidof(IMmRvUndAtom), e.Error());
	}
	return hr;
}

STDMETHODIMP CMmRvUndAtom::ClearRatios()
{
	if (m_spFut != NULL)
	{
		CComObject<CMmRvFutColl>* futColl = dynamic_cast<CComObject<CMmRvFutColl>*>(m_spFut.GetInterfacePtr());

		CMmRvFutColl::EnumCollType::iterator itrFut    = futColl->m_coll.begin();
		CMmRvFutColl::EnumCollType::iterator itrFutEnd = futColl->m_coll.end();

		for(;itrFut!=itrFutEnd; ++itrFut)
		{
			CMmRvFutAtom* pFut = dynamic_cast<CMmRvFutAtom*>(itrFut->second);
			if (pFut != NULL)
				pFut->m_dRatio	=	BAD_DOUBLE_VALUE;
		}
	}
	else
		return E_POINTER;

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
				vtValue.dblVal = ( ( BAD_DOUBLE_VALUE == m_pPrice->m_dPriceBid )  ? 0.0 : m_pPrice->m_dPriceBid );
				VariantCopy( Value, &vtValue );
				break;
			case CV_DP_DATA_UNDASK :
				VariantInit( &vtValue );
				vtValue.vt = VT_R8;
				vtValue.dblVal = ( ( BAD_DOUBLE_VALUE == m_pPrice->m_dPriceAsk)  ? 0.0 : m_pPrice->m_dPriceAsk);
				VariantCopy( Value, &vtValue );
				break;
			case CV_DP_DATA_UNDLAST:
				VariantInit( &vtValue );
				vtValue.vt = VT_R8;
				vtValue.dblVal = ( ( BAD_DOUBLE_VALUE == m_pPrice->m_dPriceLast)  ? 0.0 : m_pPrice->m_dPriceLast);
				VariantCopy( Value, &vtValue );
				break;
			case CV_DP_DATA_UNDMARK:
				VariantInit( &vtValue );
				vtValue.vt = VT_R8;
				vtValue.dblVal = ( ( BAD_DOUBLE_VALUE == m_pPrice->m_dPriceClose)  ? 0.0 : m_pPrice->m_dPriceClose);
				VariantCopy( Value, &vtValue );
				break;
			case CV_DP_DATA_UNDBETA:
				VariantInit( &vtValue );
				vtValue.vt = VT_R8;
				vtValue.dblVal = ( ( BAD_DOUBLE_VALUE == Beta_)  ? 0.0 : Beta_);
				VariantCopy( Value, &vtValue );
				break;
			case CV_DP_DATA_UNDPNLMTM :
				VariantInit( &vtValue );
				vtValue.vt = VT_R8;
				vtValue.dblVal = ( ( BAD_DOUBLE_VALUE == PnlMTM_ )  ? 0.0 : PnlMTM_ );
				VariantCopy( Value, &vtValue );
				break;
			case CV_DP_DATA_UNDPNLTHEO:
				VariantInit( &vtValue );
				vtValue.vt = VT_R8;
				vtValue.dblVal = ( ( BAD_DOUBLE_VALUE == PnLTheo_)  ? 0.0 : PnLTheo_);
				VariantCopy( Value, &vtValue );
				break;
			case CV_DP_DATA_UNDPRICE:
				VariantInit( &vtValue );
				vtValue.vt = VT_R8;
				vtValue.dblVal = ( ( BAD_DOUBLE_VALUE == m_pPrice->m_dActivePrice)  ? 0.0 : m_pPrice->m_dActivePrice);
				VariantCopy( Value, &vtValue );
				break;
			case CV_DP_DATA_UNDPOS:
				VariantInit( &vtValue );
				vtValue.vt = VT_R8;
				vtValue.dblVal = ( ( BAD_LONG_VALUE == UndPos_)  ? 0.0 : static_cast<double>(UndPos_) );
				VariantCopy( Value, &vtValue );
				break;
			case CV_DP_DATA_UNDOPTQTY:
				VariantInit( &vtValue );
				vtValue.vt = VT_R8;
				vtValue.dblVal = ( ( BAD_LONG_VALUE == OptQty_)  ? 0.0 : static_cast<double>(OptQty_) );
				VariantCopy( Value, &vtValue );
				break;
			case CV_DP_DATA_UNDTV:
				VariantInit( &vtValue );
				vtValue.vt = VT_R8;
				vtValue.dblVal = ( ( BAD_DOUBLE_VALUE == TmVal_)  ? 0.0 : TmVal_);
				VariantCopy( Value, &vtValue );
				break;
			case CV_DP_DATA_UNDTHETA:
				VariantInit( &vtValue );
				vtValue.vt = VT_R8;
				vtValue.dblVal = ( ( BAD_DOUBLE_VALUE == Tht$_)  ? 0.0 : Tht$_);
				VariantCopy( Value, &vtValue );
				break;
			case CV_DP_DATA_UNDTHETADELTA:
				VariantInit( &vtValue );
				vtValue.vt = VT_R8;
				vtValue.dblVal = ( ( BAD_DOUBLE_VALUE == ThtDlt_)  ? 0.0 : ThtDlt_);
				VariantCopy( Value, &vtValue );
				break;
			case CV_DP_DATA_UNDTHETAGAMMA:
				VariantInit( &vtValue );
				vtValue.vt = VT_R8;
				vtValue.dblVal = ( ( BAD_DOUBLE_VALUE == ThtGma_)  ? 0.0 : ThtGma_);
				VariantCopy( Value, &vtValue );
				break;
			case CV_DP_DATA_UNDVEGA:
				VariantInit( &vtValue );
				vtValue.vt = VT_R8;
				vtValue.dblVal = ( ( BAD_DOUBLE_VALUE == Vga$_)  ? 0.0 : Vga$_);
				VariantCopy( Value, &vtValue );
				break;
			case CV_DP_DATA_UNDVEGADELTA:
				VariantInit( &vtValue );
				vtValue.vt = VT_R8;
				vtValue.dblVal = ( ( BAD_DOUBLE_VALUE == VgaDlt_)  ? 0.0 : VgaDlt_);
				VariantCopy( Value, &vtValue );
				break;
			case CV_DP_DATA_UNDVEGAGAMMA:
				VariantInit( &vtValue );
				vtValue.vt = VT_R8;
				vtValue.dblVal = ( ( BAD_DOUBLE_VALUE == VgaGma_)  ? 0.0 : VgaGma_);
				VariantCopy( Value, &vtValue );
				break;
			case CV_DP_DATA_UNDWTDVEGA:
				VariantInit( &vtValue );
				vtValue.vt = VT_R8;
				vtValue.dblVal = ( ( BAD_DOUBLE_VALUE == WtdVga$_)  ? 0.0 : WtdVga$_);
				VariantCopy( Value, &vtValue );
				break;
			case CV_DP_DATA_UNDDIVS:
				{
					VariantInit( &vtValue );
					vtValue.vt = VT_R8;
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
						LONG nToday = static_cast<LONG>(vt_date::GetCurrentDate(true));
						Value->dblVal = floor(Value->dblVal);/* * OPM::cdDaysPerYear365 + nToday);*/

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
	DOUBLE dtToday = /*m_simulationParams.m_dDate  ? m_simulationParams.m_dDate  :*/ dtNow;
	CSafeArrayWrapper<double> saDates;
	CSafeArrayWrapper<double> saAmounts;

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
	}
	if ( nDivCount ) {
		dividendsValue.dblVal = bIsDivValue ? *saAmounts.GetPlainData() : *saDates.GetPlainData();
	}
	else{
		dividendsValue.dblVal = BAD_DOUBLE_VALUE;
	}

}

STDMETHODIMP CMmRvUndAtom::raw_CallFunction(LONG lFunctionID, SAFEARRAY** arrParameters, VARIANT *Value){
	return E_FAIL;
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
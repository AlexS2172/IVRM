#include "StdAfx.h"
#include "SpotContract.h"
#include "Asset.h"
#include "ContractColl.h"
#include "Curve.h"
//------------------------------------------------------------------------//
CSpotContract::CSpotContract(void): m_nLotSize(0L),
									m_dSkew(0.), 
									m_dKurt(0.), 
									m_dUndPosForRates(0.), 
									m_nTotalQtyInShares(BAD_LONG_VALUE), 
									m_nPrimaryExchangeID(0)
{
	m_enType = enCtStock;
}
//------------------------------------------------------------------------//
CSpotContract::~CSpotContract(void){
}
//------------------------------------------------------------------------//
void CSpotContract::Clear(){	
	CAbstractContract::Clear();
};
//------------------------------------------------------------------------//
/*virtual*/
void
CSpotContract::AssignGREEKS(GREEKS& resultGreeks, CRisksPtr Risks){

	Risks->dDelta = 1.;

	Risks->dBaseContractPrice = Risks->dContractPrice;
	Risks->dBaseContractPriceAsk = Risks->dContractPriceAsk;
	Risks->dBaseContractPriceBid  = Risks->dContractPriceBid;

	Risks->dTheoPrice = Risks->dContractPrice;
	Risks->dTheoPriceBid = Risks->dContractPriceBid;
	Risks->dTheoPriceAsk = Risks->dContractPriceAsk;

	Risks->dBidPrice = m_spQuote->BidPrice;
	Risks->dtBidDate = m_spQuote->BidDateTime;
	Risks->lBidSize = m_spQuote->BidSize;

	Risks->dAskPrice = m_spQuote->AskPrice;
	Risks->dtAskDate = m_spQuote->AskDateTime;
	Risks->lAskSize = m_spQuote->AskSize;

	Risks->dLastPrice = m_spQuote->LastPrice;
	Risks->dtLastDate = m_spQuote->LastDateTime;

	Risks->lVolume = m_spQuote->Volume;

	Risks->dIntrinsicValue = 0.;

	Risks->dtUpdateDate = EgLib::vt_date::GetCurrentDate(false);

};
//------------------------------------------------------------------------//
CRisksPtr
CSpotContract::Calculate(CMarketSimulator::ptr MarketSimulator){	

	try {

		ATLASSERT((bool)MarketSimulator);
		
		CRisksPtr risks = 
			(MarketSimulator == CMarketSimulator::getDefault()) ? m_spRisks : CRisksPtr(new CRisks);

		double	price_shift = MarketSimulator->getPriceShift(*this);

		risks->dContractPrice	 = max(getPriceMid() + price_shift, 0.);
		risks->dContractPriceAsk = max(getPriceAsk() + price_shift, 0.);
		risks->dContractPriceBid = max(getPriceBid() + price_shift, 0.);

		GREEKS greeks;
		AssignGREEKS(greeks, risks);

		return risks;
	}
	catch (const IvRmException::AbstractException& err){

		std::string description =	std::string("Can't calculate contract: ")
			+ std::string(GetTicker()->m_sSymbol);

		throw IvRmException::ContractNotCalculated(description, err);
	}
	catch (_com_error err){
		TRACE_COM_ERROR(err);
		ATLASSERT(FALSE);
		throw err;
	}
	catch (...){
		TRACE_UNKNOWN_ERROR();
		ATLASSERT(FALSE);
		throw;
	};

	return m_spRisks;
};
//------------------------------------------------------------------------//
void 
CSpotContract::FitToMarketVolatility(){
	try {

		if (static_cast<bool>(m_spAsset)){

			CContractColl* pContracts =	dynamic_cast<CContractColl*>(m_spDerivativeContracts.GetInterfacePtr());
			if ( pContracts ){

				CContractColl::CollType::iterator itr = pContracts->m_collRef.begin();
				CContractColl::CollType::iterator itrEnd = pContracts->m_collRef.end();
				for(; itr != itrEnd; ++itr){

					CAbstractContract* pContract = dynamic_cast<CAbstractContract*>(itr->second->second);
					if(pContract){

						try {
							if (pContract->m_enType == enCtOption)
								pContract->FitToMarketVolatility();
						}
						catch(_com_error err){
							TRACE_COM_ERROR(err);
							ATLASSERT(FALSE);
						}
						catch (...){
							TRACE_UNKNOWN_ERROR();
							ATLASSERT(FALSE);
						}
					}
				}
			}
		}
	}
	catch(_com_error err){
		TRACE_COM_ERROR(err);
		ATLASSERT(FALSE);
	}
	catch (...){
		TRACE_UNKNOWN_ERROR();
		ATLASSERT(FALSE);
	};
};
//------------------------------------------------------------------------//

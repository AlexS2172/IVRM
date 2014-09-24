#include "StdAfx.h"
#include "FutureContract.h"
#include "ContractColl.h"
#include "SpotContract.h"
//----------------------------------------------------------------------------------------//
CFutureContract::CFutureContract(void): 
										 m_nFutRootID(0L)
										, m_dtMaturityDate(0.)
										, m_bHaveOptions(VARIANT_FALSE)
										, m_bIsAmerican(VARIANT_FALSE)
										, m_bIsActive(VARIANT_FALSE)
										, m_bShowInFutureContractEq(VARIANT_FALSE)
										, m_dTickSize(BAD_DOUBLE_VALUE)
										, m_dTickValue(BAD_DOUBLE_VALUE)
										, m_dPriceQuotationUnit(BAD_DOUBLE_VALUE)
										, m_dQuoteFormat(BAD_DOUBLE_VALUE)
										, m_dBasis(0)
										, m_bUseInCalc( VARIANT_FALSE )
										, m_dActiveFutureRatio(BAD_DOUBLE_VALUE)
										, m_dK(1.)
										, m_dKEq(1.)
										, m_bMultOptDltEq(VARIANT_FALSE)
{
	m_enType = enCtFuture;
};
//----------------------------------------------------------------------------------------//
CFutureContract::~CFutureContract(void){
};
//----------------------------------------------------------------------------------------//
void CFutureContract::Clear(){
	CAbstractContract::Clear();
	m_spOptPriceProfile = NULL;
	m_spUndPriceProfile = NULL;
};
//----------------------------------------------------------------------------------------//
/*virtual*/
IEtsPriceProfileAtomPtr
CFutureContract::getPriceProfile(){
	return m_spUndPriceProfile;
};
//----------------------------------------------------------------------------------------//
CRisksPtr 
CFutureContract::Calculate(CMarketSimulator::ptr MarketSimulator){

	try {

		ATLASSERT((bool)MarketSimulator);

		CRisksPtr risks = 
			(MarketSimulator == CMarketSimulator::getDefault()) ? m_spRisks : CRisksPtr(new CRisks);

		double	price_shift = MarketSimulator->getPriceShift(*this);

		risks->dContractPrice	 = max(getPriceMid() + price_shift, 0.);
		risks->dContractPriceAsk = max(getPriceAsk() + price_shift, 0.);
		risks->dContractPriceBid = max(getPriceBid() + price_shift, 0.);

		if (m_spBaseContract){

			CSpotContract* base_contract = 
				dynamic_cast<CSpotContract*>(m_spBaseContract.GetInterfacePtr());

			ATLASSERT(base_contract != 0);

			price_shift = MarketSimulator->getPriceShift(*base_contract);

			risks->dBaseContractPrice	 = max(base_contract->getPriceMid() + price_shift, 0.);
			risks->dBaseContractPriceAsk = max(base_contract->getPriceAsk() + price_shift, 0.);
			risks->dBaseContractPriceBid = max(base_contract->getPriceBid() + price_shift, 0.);
		};

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
/*virtual*/
void
CFutureContract::AssignGREEKS(GREEKS& resultGreeks, CRisksPtr Risks){

	Risks->dDelta = 1.;

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
//----------------------------------------------------------------------------------------//
/*virtual*/
double
CFutureContract::getMaturityWeight(double Period, double CurrentDate){

	modf(CurrentDate, &CurrentDate);

	if(Period > 0. && m_dtMaturityDate > CurrentDate){	
		return sqrt (Period / /*ceill*/(m_dtMaturityDate - CurrentDate));
	};

	return BAD_DOUBLE_VALUE;
};
//----------------------------------------------------------------------------------------//
/*virtual*/
double
CFutureContract::getContractSizeInAsset(){

	if (m_spRoot){
		long lot_size = 1L;

		_CHK(m_spRoot->get_LotSize(&lot_size),
			_T("Fail to get LotSize."));

		return static_cast<double>(lot_size);
	}
	return 1.;
};
//----------------------------------------------------------------------------------------//

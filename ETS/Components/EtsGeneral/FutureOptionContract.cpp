#include "StdAfx.h"
#include "SpotContract.h"
#include "FutureContract.h"
#include "FutureOptionContract.h"
//---------------------------------------------------------------------------------------//
CFutureOptionContract::CFutureOptionContract(void)
{
	m_nBaseContractID = 0L;
	m_enType  = enCtFutOption;
}
//---------------------------------------------------------------------------------------//
CFutureOptionContract::~CFutureOptionContract(void)
{
}
//---------------------------------------------------------------------------------------//
void CFutureOptionContract::Clear()
{
	COptionContract::Clear();
};
//---------------------------------------------------------------------------------------//
/*virtual*/
IEtsPriceProfileAtomPtr
CFutureOptionContract::getPriceProfile(){
	IEtsPriceProfileAtomPtr spOptionPriceProfile = NULL;
	IEtsFutAtomPtr spBaseContract = m_spBaseContract;
	if (spBaseContract)
		spBaseContract->get_OptPriceProfile(&spOptionPriceProfile);
	return spOptionPriceProfile;
};
//---------------------------------------------------------------------------------------//
CRisksPtr
CFutureOptionContract::Calculate(CMarketSimulator::ptr MarketSimulator)
{
	try	{

		ATLASSERT((bool)MarketSimulator);

		CRisksPtr risks = 
			(MarketSimulator == CMarketSimulator::getDefault()) ? m_spRisks : CRisksPtr(new CRisks);

		//set calculation settings from global
		CCalculationSettingsPtr spCalculationSettings = 
			CCalculationSettingsSingleton::Instance();

		//prepare contract params
		double	dtCalculationDate	=	MarketSimulator->getTime(*this);
		double	dtExpiryOV			=	m_dtExpiryOV;
		double	dtTradingClose		=	m_dtTradingClose;
		double	dYte = 0.;

		::GetCalculationParams(	dtCalculationDate, dtExpiryOV, dtTradingClose,
								spCalculationSettings->GetUseTimePrecision(), 
								&dtCalculationDate, &dtExpiryOV, &dtTradingClose, &dYte );

		CSafeArrayWrapper<double> saDates;
		CSafeArrayWrapper<double> saAmounts;

		//init calculation settings
		bool bad_market_volatility = false;

		bool use_theo_volatility = 
			(spCalculationSettings->GetGreeksCalculationMode() == enGcmTheoreticalVola);
		
		bool bUseTheoVolaNoBid = 
			spCalculationSettings->GetUseTheoVolaIfNoBid();

		bool bUseTheoVolaBadMarket = 
			spCalculationSettings->GetUseTheoVolaIfBadMarketVola();

		long enModel = 
			spCalculationSettings->GetGreeksCalculationModel();

		long nMask = GT_ALL;	
		//----------------------------------------------------------------------------------//
		//get raw pointers
		CFutureContract* pBaseContract = 
			dynamic_cast<CFutureContract*>(m_spBaseContract.GetInterfacePtr());
		ATLASSERT(pBaseContract != NULL);

		CAsset* pAsset = 
			dynamic_cast<CAsset*>(m_spAsset.GetInterfacePtr());		
		ATLASSERT(pAsset != NULL);

		CSpotContract* pSpot =	  //because of spot contain Skew & Kurt
			dynamic_cast<CSpotContract*>(pAsset->m_spSpot.GetInterfacePtr());
		ATLASSERT(pSpot != NULL); 

		CAsset* pPricingAsset = 
			dynamic_cast<CAsset*>(m_spPricingAsset.GetInterfacePtr());
		ATLASSERT(pPricingAsset != NULL);

		//----------------------------------------------------------------------------------//
		long	lIsAmerican	= (pAsset->m_bIsAmerican ? 1L : 0L);
		long	lDividendsCount = 0;
		double	dVolaCalculated = 0.;

		//clear greeks
		GREEKS aGreeks;
		memset(&aGreeks, 0, sizeof(aGreeks));
		aGreeks.nMask = nMask;

		//calculate price 
		double	price_shift = MarketSimulator->getPriceShift(*pBaseContract);

		risks->dBaseContractPrice	=  
			max(pBaseContract->getPriceMid() + price_shift, 0.);

		risks->dBaseContractPriceAsk = 
			max(pBaseContract->getPriceAsk() + price_shift, 0.);

		risks->dBaseContractPriceBid = 
			max(pBaseContract->getPriceBid() + price_shift, 0.);

		double	dBaseContractPrice = risks->dBaseContractPrice;

		if( dtExpiryOV < dtCalculationDate ){

			risks->dTheoPrice = 
				max((risks->dBaseContractPrice - m_dStrike) * (m_enOptType == enOtCall ? 1 : -1), 0.);

			risks->dTheoPriceAsk = 
				max((risks->dBaseContractPriceAsk - m_dStrike) * (m_enOptType == enOtCall ? 1 : -1), 0.);

			risks->dTheoPriceBid = 
				max((risks->dBaseContractPriceBid - m_dStrike) * (m_enOptType == enOtCall ? 1 : -1), 0.);

			risks->dDelta = (risks->dTheoPrice > 0. ? (m_enOptType == enOtCall ? 1. : -1.) : 0.);

			risks->dIntrinsicValue = risks->dTheoPrice;

			return risks;
		};
		
		price_shift = MarketSimulator->getPriceShift(*this);

		risks->dContractPrice	=  
			max(getPriceMid() + price_shift, 0.);

		risks->dContractPriceAsk = 
			max(getPriceAsk() + price_shift, 0.);

		risks->dContractPriceBid = 
			max(getPriceBid() + price_shift, 0.);

		double dOptionPrice = risks->dContractPrice;

		//calculate borrowing rate

		double	current_date = 
			CCalculationSettingsSingleton::Instance()->getCurrentDate();

		double	dAssetBorrowingRate = 
			pAsset->GetBorrowingRate(current_date, dtExpiryOV);

		double	dPricingAssetBorrowingRate = 
			pPricingAsset->GetBorrowingRate(current_date, dtExpiryOV);
		
		//get theoretical volatility
		risks->dTheoVola = MarketSimulator->getVolatility(*this);
		
		//calculate dividends for contract
		double dYield = pAsset->GetDividends(dtCalculationDate, dtExpiryOV, dtTradingClose, saDates, saAmounts, lDividendsCount);

		//calculate volatility
		LONG nFlag = VF_OK;
		dVolaCalculated = ::CalcFutureOptionVolatility(	dPricingAssetBorrowingRate,
														risks->dBaseContractPrice,
														risks->dContractPrice, 
														m_dStrike, 
														dYte, m_enOptType, lIsAmerican, 100L,
														pSpot->GetSkew(),
														pSpot->GetKurt(), enModel, &nFlag);

		if(nFlag == VF_UNRELIABLE){
			if ( bUseTheoVolaBadMarket == false){	
				std::string description =	"Can't calculate volatility for: " 
											+ std::string(GetTicker()->m_sSymbol);

				throw IvRmException::NoContractVolatilityCalculated(description);
			}
			else{
				TRACE_INFO(_T("TheoreticalVolatility will be used for contract: [%s]"),
							GetTicker()->m_sSymbol.c_str());

				bad_market_volatility = true;
			}
		};

		risks->dIVola = dVolaCalculated;

		dVolaCalculated = (use_theo_volatility || bad_market_volatility) ? risks->dTheoVola : risks->dIVola;

		//calculate contract greeks
		long lResult = 0L;
		lResult = ::CalcFutureOptionGreeks3(	dPricingAssetBorrowingRate,
												dYield, 
												dBaseContractPrice,
												true, 
												m_dStrike, 
												dVolaCalculated,
												dYte,
												m_enOptType, 
												lIsAmerican, 
												100L, 
												pSpot->GetSkew(),
												pSpot->GetKurt(),
												enModel, 
												lDividendsCount,
												saAmounts.GetPlainData(), saDates.GetPlainData(),
												&aGreeks);
		
		//check result
		if(lResult == 0L){
			std::string description =	"Can't calculate greeks for: " 
										+ std::string(GetTicker()->m_sSymbol);

			throw IvRmException::NoContractGreeksCalculated(description);
		};

		CAbstractContract::AssignGREEKS(aGreeks, risks);

		return risks;
	}
	catch (const IvRmException::AbstractException& err){
		throw IvRmException::ContractNotCalculated("Contract can't be calculated.", err);
	}
	catch(_com_error err){
		TRACE_COM_ERROR(err);
		ATLASSERT(FALSE);
		throw err;
	}
	catch (...){
		TRACE_UNKNOWN_ERROR();
		ATLASSERT(FALSE);
		throw;
	}
	return m_spRisks;
}
//---------------------------------------------------------------------------------------//

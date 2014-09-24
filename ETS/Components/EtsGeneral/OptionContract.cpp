#include "StdAfx.h"
#include "OptionContract.h"
#include "SpotContract.h"
#include "MarketSimulator.h"
//------------------------------------------------------------------//
COptionContract::COptionContract(void): 
m_enOptType(enOtPut), 
m_dtExpiry(0.), 
m_dStrike(0.), 
m_IsFlex(0), 
m_dtExpiryOV(0), 
m_dtTradingClose(0){
};
//------------------------------------------------------------------//
COptionContract::~COptionContract(void)
{
};
//------------------------------------------------------------------//
void 
COptionContract::Clear(){
	CAbstractContract::Clear();
	m_spRoot = NULL;
};
//------------------------------------------------------------------//
/*virtual*/
double
COptionContract::getMaturityWeight(double Period, double CurrentDate){
	
	modf(CurrentDate, &CurrentDate);
	double	expiry(m_dtExpiryOV);
	modf(expiry, &expiry);

	if( Period > 0. && expiry > CurrentDate){		
		return sqrt (Period / /*ceill*/(expiry - CurrentDate));
	};

	return 1.;
};
//---------------------------------------------------------------------------------------//
double 
COptionContract::GetTheoVolatility()
{
	double dVolatility = 0.;
	try
	{
		if (static_cast<bool>(m_spAsset))
		{
			CAsset* pAsset = dynamic_cast<CAsset*>(m_spAsset.GetInterfacePtr());
			ATLASSERT(pAsset != NULL);

			if (pAsset)
			{
				IVolaControl* pVolatilitySource = NULL;
				pVolatilitySource = pAsset->GetVolatilitySource();

				if (pVolatilitySource)
				{
					long lSurfaceID = -1;
					pVolatilitySource->GetSurfaceByRoot(-1, &lSurfaceID);
					_CHK(pVolatilitySource->get_OptionVola(m_dtExpiryOV, m_dStrike, lSurfaceID, &dVolatility), 
						_T("Fail to get volatility from Volatility Source."));
				}
			}
		}
	}
	catch (_com_error err)
	{
		ATLASSERT(false);
		std::string description =	"Can't get theoretical volatility for: " 
			+ std::string(GetTicker()->m_sSymbol);

		throw IvRmException::NoContractTheoreticalVolatility(description);	
	}
	catch (...)
	{
		ATLASSERT(false);
		throw;
	}
	return dVolatility;
};	
//---------------------------------------------------------------------------------------//
void 
COptionContract::FitToMarketVolatility()
{
	try
	{
		if (static_cast<bool>(m_spAsset))
		{
			CAsset* pAsset = dynamic_cast<CAsset*>(m_spAsset.GetInterfacePtr());
			ATLASSERT(pAsset != NULL);

			if (pAsset)
			{
				IVolaControl* pVolatilitySource = NULL;
				pVolatilitySource = pAsset->GetVolatilitySource();

				if (pVolatilitySource && m_spRisks->dIVola > 0.)
				{
					long lSurfaceID = 0;
					pVolatilitySource->GetSurfaceByRoot(-1, &lSurfaceID);
					_CHK(pVolatilitySource->put_OptionVola(m_dtExpiryOV, m_dStrike, lSurfaceID, m_spRisks->dIVola), 
						_T("Fail to get volatility from Volatility Source."));
				}
			}
		}
	}
	catch (_com_error err)
	{
		TRACE_COM_ERROR(err);
		ATLASSERT(false);
		throw err;
	}
	catch (...)
	{
		TRACE_UNKNOWN_ERROR();
		ATLASSERT(false);
		throw;
	}
};
//---------------------------------------------------------------------------------------//
/*virtual*/
double 
COptionContract::getPriceMid(){

	double	dPrice = 0.;

	try {
	
		if ( m_dManualPrice > 0. ) {
			return m_dManualPrice;
		}
		else{


			IEtsPriceProfileAtomPtr spPriceProfile = getPriceProfile();

			if (spPriceProfile) {

				//set to global
				CCalculationSettingsPtr spUsedSettings = 
					CCalculationSettingsSingleton::Instance();

				//get price according to price profile
				spPriceProfile->GetOptPriceMid(	m_spQuote->BidPrice, 
												m_spQuote->AskPrice, 
												m_spQuote->LastPrice,
												static_cast<EtsPriceRoundingRuleEnum>(spUsedSettings->GetPriceRoundingRule()), 
												false,
												0.,
												NULL,
												&dPrice);

				//check price
				if (dPrice < 0./*DBL_EPSILON*/) {
					std::string description =	"No option contract price for: "
						+ std::string(GetTicker()->m_sSymbol);

					throw IvRmCore::IvRmException::NoContractPrice(description);
				}
			}
		}
	}
	catch (const IvRmException::AbstractException& err) {
		throw err;
	}
	catch (_com_error err) {
		TRACE_COM_ERROR(err);
		ATLASSERT(FALSE);
		throw err;
	}
	catch (...) {
		TRACE_UNKNOWN_ERROR();
		ATLASSERT(FALSE);
		throw;
	}
	return dPrice;
}
//---------------------------------------------------------------------------------------//
/*virtual*/
double 
COptionContract::getPriceBid()
{
	double	dPrice = 0.;

	try{

		if ( m_dManualPrice > 0. ){
			return m_dManualPrice;
		}
		else{

			IEtsPriceProfileAtomPtr spPriceProfile = getPriceProfile();

			if (spPriceProfile){
				//set to global
				CCalculationSettingsPtr spUsedSettings = 
					CCalculationSettingsSingleton::Instance();

				//get price according to price profile
				spPriceProfile->GetOptPriceBid(	m_spQuote->BidPrice, 
												m_spQuote->AskPrice, 
												m_spQuote->LastPrice,
												static_cast<EtsPriceRoundingRuleEnum>(spUsedSettings->GetPriceRoundingRule()), 
												false,
												0.,
												NULL,
												&dPrice);

				//check price
				if (dPrice < 0./*DBL_EPSILON*/){
					std::string description =	"No option contract price Bid for: "
						+ std::string(GetTicker()->m_sSymbol);

					throw IvRmCore::IvRmException::NoContractPrice(description);
				}
			}
		}
	}
	catch (const IvRmException::AbstractException& err){
		throw err;
	}
	catch (_com_error err){
		TRACE_COM_ERROR(err);
		ATLASSERT(FALSE);
		throw err;
	}
	catch (...)
	{
		TRACE_UNKNOWN_ERROR();
		ATLASSERT(FALSE);
		throw;
	}
	return dPrice;
}
//---------------------------------------------------------------------------------------//
/*virtual*/
double 
COptionContract::getPriceAsk() {
	double	dPrice = 0.;

	try {

		if ( m_dManualPrice > 0. ) {
			return m_dManualPrice;
		}
		else{
			
			IEtsPriceProfileAtomPtr spPriceProfile = getPriceProfile();

			if (spPriceProfile) {

				//set to global
				CCalculationSettingsPtr spUsedSettings = 
					CCalculationSettingsSingleton::Instance();

				//get price according to price profile
				spPriceProfile->GetOptPriceAsk(	m_spQuote->BidPrice, 
												m_spQuote->AskPrice, 
												m_spQuote->LastPrice,
												static_cast<EtsPriceRoundingRuleEnum>(spUsedSettings->GetPriceRoundingRule()), 
												false,
												0.,
												NULL,
												&dPrice);

				//check price
				if ( dPrice < 0./*DBL_EPSILON*/ ) {
				
					std::string description =	"No option contract price Ask for: "
						+ std::string(GetTicker()->m_sSymbol);

					throw IvRmCore::IvRmException::NoContractPrice(description);
				}
			}
		}
	}
	catch (const IvRmException::AbstractException& err) {
		throw err;
	}
	catch (_com_error err) {
		TRACE_COM_ERROR(err);
		ATLASSERT(FALSE);
		throw err;
	}
	catch (...)	{
		TRACE_UNKNOWN_ERROR();
		ATLASSERT(FALSE);
		throw;
	}
	return dPrice;
};
//---------------------------------------------------------------------------------------//
/*virtual*/
double
COptionContract::getContractSizeInAsset() {
	if (m_spRoot) {
		LONG lot_size = 1L;

		_CHK(m_spRoot->get_LotSize(&lot_size), 
			_T("Fail to get LotSize."));

		return static_cast<double>(lot_size);
	}
	return 1.;
};
//---------------------------------------------------------------------------------------//
/*virtual*/
bool
SpotOptionModel::calculate_greeks(CalculationParametersPtr parameters, 
								  CCalculationSettingsPtr settings, 
								  CRisksPtr anx) {

	OptionCalculationParameters* pmts = 
		dynamic_cast<OptionCalculationParameters*>(parameters.get());

	ATLASSERT(pmts != 0);

	long calculation_mask = GT_ALL;
	long steps = 100L;
	long calculation_model = settings->GetGreeksCalculationModel();
	long calculation_status = 1L;

	GREEKS aGreeks;
	aGreeks.nMask = calculation_mask;

	pmts->copy_to(anx);

	if (pmts->simulation_date > pmts->expiration_date) {
		
		anx->dTheoPrice = 
			max((pmts->base_contract_price.mid() - pmts->strike) * (pmts->is_call ? 1 : -1), 0.);

		anx->dTheoPriceAsk = 
			max((pmts->base_contract_price.ask() - pmts->strike) * (pmts->is_call ? 1 : -1), 0.);

		anx->dTheoPriceBid = 
			max((pmts->base_contract_price.bid() - pmts->strike) * (pmts->is_call ? 1 : -1), 0.);

		anx->dDelta = (anx->dTheoPrice > 0. ? (pmts->is_call ? 1. : -1.) : 0.);

		anx->dIntrinsicValue = anx->dTheoPrice;		
	}
	else {
		
		bool use_theo_volatility = (settings->GetGreeksCalculationMode() == enGcmTheoreticalVola);
		bool use_theo_bad_market = settings->GetUseTheoVolaIfBadMarketVola();
		
		double volatility = 
			(use_theo_volatility || (use_theo_bad_market && pmts->volatility.implied() == BAD_DOUBLE_VALUE) ? 
			pmts->volatility.theo() : pmts->volatility.implied());

		calculation_status = 
			::CalcGreeksMM2(	pmts->pricing_asset_borrowing_rate, 
								pmts->yield,
								pmts->asset_borrowing_rate,
								pmts->base_contract_price.mid(),
								pmts->strike,
								volatility,
								pmts->yte, 
								pmts->is_call, 
								pmts->is_american, 
								pmts->dividend.count(),
								pmts->dividend.ammount(), 
								pmts->dividend.dates(), 
								steps, 
								pmts->skew, 
								pmts->kurt, 
								calculation_model, 
								&aGreeks);

		anx->dIntrinsicValue = 
			max((pmts->base_contract_price.mid() - pmts->strike) * (pmts->is_call ? 1 : -1), 0.);

		anx->fill_by(aGreeks);
	};

	return (calculation_status != 0);
};
//---------------------------------------------------------------------------------------//
/*virtual*/
bool
SpotOptionModel::calculate_volatility(CalculationParametersPtr parameters, 
									  CCalculationSettingsPtr settings, 
									  CRisksPtr anx) {
	
	OptionCalculationParameters* pmts = 
		dynamic_cast<OptionCalculationParameters*>(parameters.get());

	ATLASSERT(pmts != 0);

	long steps = 100L;
	long model = settings->GetGreeksCalculationModel();
	long result_flag = 0;

	double implied_volatility = 
		::CalcVolatilityMM3(	pmts->pricing_asset_borrowing_rate, 
								pmts->yield, 
								pmts->asset_borrowing_rate, 
								pmts->base_contract_price.mid(),
								pmts->price.mid(), 
								pmts->strike,
								pmts->yte, 
								pmts->is_call, 
								pmts->is_american, 
								pmts->dividend.count(),
								pmts->dividend.ammount(), 
								pmts->dividend.dates(), 
								steps,
								pmts->skew,
								pmts->kurt, 
								model, 
								&result_flag);

	anx->dIVola = (result_flag != VF_UNRELIABLE ? implied_volatility : BAD_DOUBLE_VALUE);

	parameters->volatility.implied_vola = anx->dIVola;

	return (result_flag != VF_UNRELIABLE);
};
//---------------------------------------------------------------------------------------//
/*virtual*/
bool
FutureOptionModel::calculate_greeks( CalculationParametersPtr parameters, 
									 CCalculationSettingsPtr settings, 
									 CRisksPtr anx) {
	
	OptionCalculationParameters* pmts = 
		dynamic_cast<OptionCalculationParameters*>(parameters.get());

	ATLASSERT(pmts != 0);

	long calculation_mask = GT_ALL;
	long steps = 100L;
	long calculation_model = settings->GetGreeksCalculationModel();
	long calculation_status = 1L;

	GREEKS aGreeks;
	aGreeks.nMask = calculation_mask;

	pmts->copy_to(anx);

	if ( pmts->simulation_date > pmts->expiration_date ) {

		anx->dTheoPrice = 
			max((pmts->base_contract_price.mid() - pmts->strike) * (pmts->is_call ? 1 : -1), 0.);

		anx->dTheoPriceAsk = 
			max((pmts->base_contract_price.ask() - pmts->strike) * (pmts->is_call ? 1 : -1), 0.);

		anx->dTheoPriceBid = 
			max((pmts->base_contract_price.bid() - pmts->strike) * (pmts->is_call ? 1 : -1), 0.);

		anx->dDelta = (anx->dTheoPrice > 0. ? (pmts->is_call ? 1. : -1.) : 0.);

		anx->dIntrinsicValue = anx->dTheoPrice;		
	}
	else {

		bool use_theo_volatility = (settings->GetGreeksCalculationMode() == enGcmTheoreticalVola);
		bool use_theo_bad_market = settings->GetUseTheoVolaIfBadMarketVola();

		double volatility = 
			(use_theo_volatility || (use_theo_bad_market && pmts->volatility.implied() == BAD_DOUBLE_VALUE) ? 
			pmts->volatility.theo() : pmts->volatility.implied());

		calculation_status = 
			::CalcFutureOptionGreeks3(	pmts->pricing_asset_borrowing_rate,
										pmts->yield, 
										pmts->base_contract_price.mid(),
										true, 
										pmts->strike, 
										volatility,
										pmts->yte,
										pmts->is_call, 
										pmts->is_american, 
										steps, 
										pmts->skew,
										pmts->kurt,
										calculation_model, 
										pmts->dividend.count(),
										pmts->dividend.ammount(),
										pmts->dividend.dates(),
										&aGreeks);

		anx->dIntrinsicValue = 
			max((pmts->base_contract_price.mid() - pmts->strike) * (pmts->is_call ? 1 : -1), 0.);

		anx->fill_by(aGreeks);
	};

	return (calculation_status != 0);
};
//---------------------------------------------------------------------------------------//
/*virtual*/
bool
FutureOptionModel::calculate_volatility(CalculationParametersPtr parameters, 
									    CCalculationSettingsPtr settings, 
									    CRisksPtr anx) {

	OptionCalculationParameters* pmts = 
		dynamic_cast<OptionCalculationParameters*>(parameters.get());

	ATLASSERT(pmts != 0);

	long steps = 100L;
	long model = settings->GetGreeksCalculationModel();
	long result_flag = 0;

	double implied_volatility = 
		::CalcFutureOptionVolatility(	pmts->pricing_asset_borrowing_rate,
										pmts->base_contract_price.mid(),
										pmts->price.mid(), 
										pmts->strike, 
										pmts->yte, 
										pmts->is_call, 
										pmts->is_american, 
										steps,
										pmts->skew,
										pmts->kurt, 
										model, 
										&result_flag);

	anx->dIVola = (result_flag != VF_UNRELIABLE ? implied_volatility : BAD_DOUBLE_VALUE);

	parameters->volatility.implied_vola = anx->dIVola;

	return (result_flag != VF_UNRELIABLE);
};
//---------------------------------------------------------------------------------------//
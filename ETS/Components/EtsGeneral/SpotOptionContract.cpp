#include "StdAfx.h"
#include "SpotOptionContract.h"
#include "SpotContract.h"
#include "Asset.h"
#include "Curve.h"
//---------------------------------------------------------------------------------------//
CSpotOptionContract::CSpotOptionContract(void)
{
	m_nRootID = 0L;
	m_enType = enCtOption;	
};
//---------------------------------------------------------------------------------------//
CSpotOptionContract::~CSpotOptionContract(void)
{
};
//---------------------------------------------------------------------------------------//
void	
CSpotOptionContract::Clear() {
	COptionContract::Clear();
};
//---------------------------------------------------------------------------------------//
CRisksPtr 
CSpotOptionContract::Calculate(CMarketSimulator::ptr MarketSimulator) {
	try {
		
		ATLASSERT((bool)MarketSimulator);

		CRisksPtr risks = 
			(MarketSimulator == CMarketSimulator::getDefault()) ? m_spRisks : CRisksPtr(new CRisks);

		OptionCalculationParameters* calc_params 
			= new OptionCalculationParameters();

		CalculationParametersPtr sp_cp(calc_params);

		CalculationModelPtr model;

		//set calculation settings from global
		CCalculationSettingsPtr settings = 
			CCalculationSettingsSingleton::Instance();

		//prepare contract params
		double	simulation_date_time = MarketSimulator->getTime(*this);
		double	expiration_date_time = m_dtExpiryOV;
		double	trading_close_time = m_dtTradingClose;
		double	yte = 0.;

		::GetCalculationParams(	simulation_date_time, 
								expiration_date_time, 
								trading_close_time,
								settings->GetUseTimePrecision(), 
								&simulation_date_time, 
								&expiration_date_time, 
								&trading_close_time, &yte);

		CSafeArrayWrapper<double> saDates;
		CSafeArrayWrapper<double> saAmounts;
		
		calc_params->yte = yte;

		//---------------------------------------------------------------------------//
		//get raw pointers
		CSpotContract* pBaseContract = 
			dynamic_cast<CSpotContract*>(get_base_contract());
		ATLASSERT(pBaseContract != NULL);

		CAsset* asset = get_asset();
		ATLASSERT(asset != NULL);

		CAsset* pricing_asset = get_pricing_asset();
		ATLASSERT(pricing_asset != NULL);

		//---------------------------------------------------------------------------//
		calc_params->is_american = (asset->m_bIsAmerican ? 1L : 0L);
		calc_params->strike = m_dStrike;
		calc_params->is_call = (m_enOptType == enOtCall);
		calc_params->expiration_date = expiration_date_time;
		calc_params->simulation_date = simulation_date_time;
		
		//calculate price
		double	current_date = 
			CCalculationSettingsSingleton::Instance()->getCurrentDate();

		AccuralBasis acb;
		double term = acb.get_term(current_date, calc_params->expiration_date);

		Interpolation interpolation_type = (Interpolation)settings->interpolation_type();
		Extrapolation extrapolation_type = (Extrapolation)settings->extrapolation_type();

		double price_shift = MarketSimulator->getPriceShift(*pBaseContract, 
															term,
															interpolation_type,
															extrapolation_type);
			
		calc_params->base_contract_price.mid_price = 
			max(pBaseContract->getPriceMid() + price_shift, 0.);

		calc_params->base_contract_price.ask_price = 
			max(pBaseContract->getPriceAsk() + price_shift, 0.);

		calc_params->base_contract_price.bid_price = 
			max(pBaseContract->getPriceBid() + price_shift, 0.);

		model = CalculationModelPtr(new SpotOptionModel);

		bool use_forward_price = asset->price_option_using_fwd_curve;

		if (use_forward_price){
			double forward_price = 0.;
			bool correct_fp = 
				asset->forward_curve->get_value(	coordinate(term), 
													interpolation_type, 
													extrapolation_type, 
													forward_price);

			if (correct_fp){
				calc_params->base_contract_price.mid_price = 
					calc_params->base_contract_price.ask_price = 
					calc_params->base_contract_price.bid_price = 
						max(forward_price + price_shift, 0.);

				model = CalculationModelPtr(new FutureOptionModel);
			};
		};
	
		//calculate borrowing rate
		calc_params->asset_borrowing_rate = 
			asset->GetBorrowingRate (current_date, expiration_date_time);

		calc_params->pricing_asset_borrowing_rate =  
			pricing_asset->GetBorrowingRate (current_date, expiration_date_time);

		//get theoretical volatility
		calc_params->volatility.theo_vola = MarketSimulator->getVolatility(*this);

		//calculate dividends for contract
		long dividends_count = 0;
		double	dYield = asset->GetDividends(	simulation_date_time, 
												expiration_date_time, 
												trading_close_time, 
												saDates, 
												saAmounts, 
												dividends_count);

		calc_params->dividend.count_div = dividends_count;
		calc_params->dividend.ammount_div = saAmounts.GetPlainData();
		calc_params->dividend.dates_div = saDates.GetPlainData();

		calc_params->skew = pBaseContract->m_dSkew;
		calc_params->kurt = pBaseContract->m_dKurt;
		calc_params->yield = dYield;
				
		// if option is not flex - calculate implied volatility
		if ( m_IsFlex != 1 ) {
		
			price_shift = MarketSimulator->getPriceShift(*this);

			calc_params->price.mid_price =
				max(getPriceMid() + price_shift, 0.);

			calc_params->price.ask_price = 
				max(getPriceAsk() + price_shift, 0.);

			calc_params->price.bid_price = 
				max(getPriceBid() + price_shift, 0.);
			
			risks->fill_by(m_spQuote);

			model->calculate_volatility(sp_cp, settings, risks);

			/*if(nFlag == VF_UNRELIABLE){
				if ( bUseTheoVolaBadMarket == false){	
					std::string description =	"Can't calculate volatility for: " 
												+ std::string(GetTicker()->m_sSymbol);

					throw IvRmException::NoContractVolatilityCalculated(description);
				}
				else{
					TRACE_INFO(_T("TheoreticalVolatility will be used for contract: [%s]"),
								GetTicker()->m_sSymbol.c_str());

					bad_market_valatility = true;
				}
			};*/
		}
		else {
			calc_params->volatility.implied_vola = calc_params->volatility.theo();
		};
		
		model->calculate_greeks(sp_cp, settings, risks);

		risks->dtUpdateDate = current_date;

		//if(lResult == 0L){
		//	std::string description =	"Can't calculate greeks for: " 
		//								+ std::string(GetTicker()->m_sSymbol);

		//	throw IvRmException::NoContractGreeksCalculated(description);
		//};


		//std::ostringstream debug_stream;
		//debug_stream<<"Calculated: " << GetTicker()->m_sSymbol
		//			<<"\nTheoPrice: " << risks->dTheoPrice
		//			<<"\nTheoVola: " << risks->dTheoVola
		//			<<"\nDelta: " << risks->dDelta;

		//TRACE_INFO(_T("%s"), std::string(debug_stream.str()).c_str());

		return risks;

	}
	catch (const IvRmException::AbstractException& err) {

		std::string description =	std::string("Can't calculate contract: ")
									+ std::string(GetTicker()->m_sSymbol);

		throw IvRmException::ContractNotCalculated(description, err);
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
	return m_spRisks;
};
//---------------------------------------------------------------------------------------//
/*virtual*/
IEtsPriceProfileAtomPtr
CSpotOptionContract::getPriceProfile(){
	IEtsPriceProfileAtomPtr spPriceProfile = NULL;
	if (m_spAsset){
		m_spAsset->get_OptPriceProfile(&spPriceProfile);
	}
	return spPriceProfile;
};
//---------------------------------------------------------------------------------------//
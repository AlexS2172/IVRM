#include "StdAfx.h"
#include "MarketSimulator.h"

#include "SpotContract.h"
#include "OptionContract.h"
#include "SpotOptionContract.h"
#include "FutureContract.h"
#include "FutureOptionContract.h"

#include "ContractsCache.h"
#include "Asset.h"
#include "Curve.h"

//----------------------------------------------------------------//
CMarketSimulator::CMarketSimulator(void):
m_priceScenario(Scenario::ptr()),
m_timeScenario(Scenario::ptr()),
m_volatilityScenario(Scenario::ptr()){
};
//----------------------------------------------------------------//
CMarketSimulator::~CMarketSimulator(void){
};
//----------------------------------------------------------------//
/*virtual*/
double
CMarketSimulator::getPrice(CSpotContract& contract){
	double contract_price = contract.getPriceMid();
	double price_shift = 0.;
	
	if (m_priceScenario)
		price_shift = m_priceScenario->getShift(contract);

	if (price_shift + contract_price >= 0.)
		return price_shift + contract_price;

	return 0.;
};
//----------------------------------------------------------------//
/*virtual*/
double
CMarketSimulator::getPrice(CSpotOptionContract& contract){
	return contract.getPriceMid();
};
//----------------------------------------------------------------//
/*virtual*/
double
CMarketSimulator::getPrice(CFutureContract& contract){
	double contract_price = contract.getPriceMid();
	double price_shift = 0.;

	if (m_priceScenario)
		price_shift = m_priceScenario->getShift(contract);

	if (price_shift + contract_price >= 0.)
		return price_shift + contract_price;

	return 0.;
};
//----------------------------------------------------------------//
/*virtual*/
double
CMarketSimulator::getPrice(CFutureOptionContract& contract){
	return contract.getPriceMid();
};
//----------------------------------------------------------------//
/*virtual*/
double
CMarketSimulator::getPriceShift(CSpotContract& contract, 
								double term,
								Interpolation interpolation,
								Extrapolation extrapolation){
	double price_shift = 0.;

	if (m_priceScenario)
		price_shift = m_priceScenario->getShift(contract, 
												term, 
												interpolation, 
												extrapolation);

	return price_shift;
};
//----------------------------------------------------------------//
/*virtual*/
double
CMarketSimulator::getPriceShift(CSpotOptionContract& contract){
	return 0.;
};
//----------------------------------------------------------------//
/*virtual*/
double
CMarketSimulator::getPriceShift(CFutureContract& contract){
	double price_shift = 0.;

	if (m_priceScenario)
		price_shift = m_priceScenario->getShift(contract);

	return price_shift;
};
//----------------------------------------------------------------//
/*virtual*/
double
CMarketSimulator::getPriceShift(CFutureOptionContract& contract){
	return 0.;
};
//----------------------------------------------------------------//
/*virtual*/
double
CMarketSimulator::getVolatility(CSpotOptionContract& contract){
	double contract_volatility = contract.GetTheoVolatility();
	double volatility_shift = 0.;

	if (m_volatilityScenario)
		volatility_shift = m_volatilityScenario->getShift(contract);

	if (volatility_shift + contract_volatility >= 0.)
		return volatility_shift + contract_volatility;

	return 0.001;
};
//----------------------------------------------------------------//
/*virtual*/
double
CMarketSimulator::getVolatility(CFutureOptionContract &contract){
	double contract_volatility = contract.GetTheoVolatility();
	double volatility_shift = 0.;

	if (m_volatilityScenario)
		volatility_shift = m_volatilityScenario->getShift(contract);

	if (volatility_shift + contract_volatility >= 0.)
		return volatility_shift + contract_volatility;

	return 0.001;
};
//----------------------------------------------------------------//
/*virtual*/
double
CMarketSimulator::getTime(CFutureOptionContract &contract){
	
	double current_date = 
		CCalculationSettingsSingleton::Instance()->getCurrentDate();

	double time_shift = 0.;
	
	if (m_timeScenario)
		time_shift = m_timeScenario->getShift(contract);

	return time_shift + current_date;
};
//----------------------------------------------------------------//
/*virtual*/
double
CMarketSimulator::getTime(CSpotOptionContract& contract){

	double current_date = 
		CCalculationSettingsSingleton::Instance()->getCurrentDate();

	double time_shift = 0.;

	if (m_timeScenario)
		time_shift = m_timeScenario->getShift(contract);

	return time_shift + current_date;	
};
//----------------------------------------------------------------//
/*static*/
CMarketSimulator::ptr
CMarketSimulator::getDefault(){
	static CMarketSimulator::ptr __instance = 
		CMarketSimulator::ptr(new CMarketSimulator);

	ATLASSERT((bool)__instance);

	return __instance;
};
//----------------------------------------------------------------//
void
CMarketSimulator::setPriceScenario(Scenario::ptr scenario_ptr){
	m_priceScenario = scenario_ptr;
};
//----------------------------------------------------------------//
void
CMarketSimulator::setTimeScenario(Scenario::ptr scenario_ptr){
	m_timeScenario = scenario_ptr;
};
//----------------------------------------------------------------//
void
CMarketSimulator::setVolatilityScenario(Scenario::ptr scenario_ptr){
	m_volatilityScenario = scenario_ptr;
};
//----------------------------------------------------------------//
//----------------------------------------------------------------//
Scenario::Scenario():
m_shiftUnit(0),
shifter(Shift::ptr()){
};
//----------------------------------------------------------------//
Scenario::~Scenario(){
};
//----------------------------------------------------------------//
void
Scenario::setShifter(Shift::ptr shifter){
	this->shifter = shifter;
};
//----------------------------------------------------------------//
void
Scenario::setShiftUnit(long ShiftUnit){
	m_shiftUnit = ShiftUnit;
};
//----------------------------------------------------------------//
long
Scenario::getShiftUnit(){
	return m_shiftUnit;
};
//----------------------------------------------------------------//
double
Scenario::getShift(CFutureContract &contract){
	return get_shift(&contract);
};
//----------------------------------------------------------------//
double
Scenario::getShift(CFutureOptionContract &contract){
	return get_shift(&contract);;
};
//----------------------------------------------------------------//
double
Scenario::getShift(CSpotContract &contract, 
				   double term,
				   Interpolation interpolation,
				   Extrapolation extrapolation){

	return get_shift(&contract, term, interpolation, extrapolation);
};
//----------------------------------------------------------------//
double
Scenario::getShift(CSpotOptionContract &contract){
	return get_shift(&contract);;
};
//----------------------------------------------------------------//
/*virtual*/
double
Scenario::getAbstractShiftValue(CAbstractContract* contract){
	return shifter ? shifter->getShift(contract) : 0.;
};
//----------------------------------------------------------------//
double
Scenario::getShift(double value, double shift){
	long shift_unit = getShiftUnit();

	if (shift_unit == SHIFT_UNIT_PERCENT){
		return value * shift / 100.;
	}
	else if (shift_unit == SHIFT_UNIT_ABSOLUTE){
		return shift;
	}
	return 0.;
};
//----------------------------------------------------------------//
/*virtual*/
double
Scenario::get_shift(CAbstractContract* contract, 
					double term,
					Interpolation interpolation,
					Extrapolation extrapolation){
	return 0.;
};
//----------------------------------------------------------------//
CorrelatedPriceScenario::CorrelatedPriceScenario()
:Scenario(),
m_shiftDriverAsset(0){
};
//----------------------------------------------------------------//
CorrelatedPriceScenario::~CorrelatedPriceScenario(){
	m_shiftDriverAsset = 0;
};
//----------------------------------------------------------------//
/*virtual*/
double
CorrelatedPriceScenario::getShift(CSpotOptionContract& contract){
	return 0.;
};
//----------------------------------------------------------------//
/*virtual*/
double
CorrelatedPriceScenario::getShift(CFutureOptionContract& contract){
	return 0.;
};
//----------------------------------------------------------------//
CAsset*
CorrelatedPriceScenario::getShiftDriverAsset(){
	return m_shiftDriverAsset;
};
//----------------------------------------------------------------//
void
CorrelatedPriceScenario::setShiftDriverAsset(CAsset::ptr Asset){
	m_shiftDriverAsset = Asset;
};
//----------------------------------------------------------------//
CAsset*
CorrelatedPriceScenario::get_contract_asset(CAbstractContract *contract){
	if (contract){
		CAsset::ptr asset_ptr = 
			dynamic_cast<CAsset::ptr>(contract->GetAsset().GetInterfacePtr());

		ATLASSERT(asset_ptr);

		return asset_ptr;
	}
	return 0;
};
//----------------------------------------------------------------//
double
CorrelatedPriceScenario::get_driver_price(){

	if (m_shiftDriverAsset && m_shiftDriverAsset->m_spSpot){

		CAbstractContract* driver = 
			dynamic_cast<CAbstractContract*>(m_shiftDriverAsset->m_spSpot.GetInterfacePtr());

		if (driver){
			return driver->getPriceMid();
		}
	}
	return BAD_DOUBLE_VALUE;
};
//----------------------------------------------------------------//
double
CorrelatedPriceScenario::get_correlation(CAbstractContract* contract){
	CAsset::ptr contract_asset = get_contract_asset(contract);
	return CAssetCorrelation::getInstance()->getCorrelation(*contract_asset,
															*m_shiftDriverAsset);
};
//----------------------------------------------------------------//
/*virtual*/
double
CorrelatedPriceScenario::get_shift(CAbstractContract* contract, 
								   double term,
								   Interpolation interpolation,
								   Extrapolation extrapolation){
	
	long	shift_unit	= getShiftUnit();
	double	default_shift = getAbstractShiftValue(contract);	
	double	contract_price = contract->getPriceMid();

	//refactoring needed
	CAsset* asset = 
		dynamic_cast<CAsset*>(contract->m_spAsset.GetInterfacePtr());

	ATLASSERT(asset != 0);

	bool use_forward_price = asset->price_option_using_fwd_curve;
	if (use_forward_price && term > 0){
		double forward_price = 0.;
		bool correct_fp = 
			asset->forward_curve->get_value(coordinate(term), 
											interpolation, 
											extrapolation, 
											forward_price);
		if (correct_fp){
			contract_price = forward_price;
		};
	};
	//refactoring needed

	double	dBeta = get_correlation(contract);

	if (shift_unit == SHIFT_UNIT_PERCENT){
		default_shift /= 100.;
	}
	else if (shift_unit == SHIFT_UNIT_ABSOLUTE){

		double driver_price = get_driver_price();

		if (driver_price > 0)
			default_shift /= driver_price;
	}
	return	(default_shift > -1. ? 
			 contract_price * (exp (log (1. + default_shift) * dBeta) - 1.) : 0.);
};
//----------------------------------------------------------------//
GeneralPriceScenario::GeneralPriceScenario()
:Scenario(){
};
//----------------------------------------------------------------//
GeneralPriceScenario::~GeneralPriceScenario(){
};
//----------------------------------------------------------------//
double
GeneralPriceScenario::get_shift(CAbstractContract *contract, 
								double term,
								Interpolation interpolation,
								Extrapolation extrapolation){

	double default_shift = getAbstractShiftValue(contract);
	
	//refactoring needed
	CAsset* asset = 
		dynamic_cast<CAsset*>(contract->m_spAsset.GetInterfacePtr());

	ATLASSERT(asset != 0);

	double original_value = contract->getPriceMid();

	bool use_forward_price = asset->price_option_using_fwd_curve;
	if (use_forward_price && term > 0){
		double forward_price = 0.;
		bool correct_fp = 
			asset->forward_curve->get_value(coordinate(term), 
											interpolation, 
											extrapolation, 
											forward_price);
		if (correct_fp){
			original_value = forward_price;
		};
	};
	//refactoring needed

	return Scenario::getShift(original_value, default_shift);
};
//----------------------------------------------------------------//
/*virtual*/
double
GeneralPriceScenario::getShift(CSpotOptionContract& contract){
	return 0.;
};
//----------------------------------------------------------------//
/*virtual*/
double
GeneralPriceScenario::getShift(CFutureOptionContract& contract){
	return 0.;
};
//----------------------------------------------------------------//
GeneralVolatilityScenario::GeneralVolatilityScenario()
:Scenario(){
};
//----------------------------------------------------------------//
GeneralVolatilityScenario::~GeneralVolatilityScenario(){
};
//----------------------------------------------------------------//
double
GeneralVolatilityScenario::get_shift(CAbstractContract *contract,
									 double term,
									 Interpolation interpolation,
									 Extrapolation extrapolation){

	COptionContract* c = dynamic_cast<COptionContract*>(contract);
	ATLASSERT(c);
	double default_shift = getAbstractShiftValue(c);
	return Scenario::getShift(c->GetTheoVolatility(), default_shift);
};
//----------------------------------------------------------------//
/*virtual*/
double
GeneralVolatilityScenario::getShift(CSpotContract &contract){
	return 0.;
};
//----------------------------------------------------------------//
/*virtual*/
double
GeneralVolatilityScenario::getShift(CFutureContract &contract){
	return 0.;
};
//----------------------------------------------------------------//
GeneralTimeScenario::GeneralTimeScenario():
Scenario(){
};
//----------------------------------------------------------------//
/*virtual*/
double
GeneralTimeScenario::get_shift(	CAbstractContract* contract,
								double term,
								Interpolation interpolation,
								Extrapolation extrapolation){
	return getAbstractShiftValue(contract);
};
//----------------------------------------------------------------//
DefaultShift::DefaultShift(double default_shift){
	this->default_shift = default_shift;
};
//----------------------------------------------------------------//
/*virtual*/
double
DefaultShift::getShift(CAbstractContract *contract){
	return default_shift;
};
//----------------------------------------------------------------//
ShiftDecorator::ShiftDecorator(Shift::ptr shift){
	this->shift = shift;
};
//----------------------------------------------------------------//
/*virtual*/
double
ShiftDecorator::getShift(CAbstractContract *contract){
	return shift ? shift->getShift(contract) : 0.;
};
//----------------------------------------------------------------//
Shift::ptr
ShiftDecorator::setDecorator(Shift::ptr decorator){
	shift = decorator;
	return shift;
};
//----------------------------------------------------------------//
/*virtual*/
ShiftDecorator::~ShiftDecorator(){
};
//----------------------------------------------------------------//
WeightedShiftDecorator::WeightedShiftDecorator(Shift::ptr shift):
ShiftDecorator(shift){
};
//----------------------------------------------------------------//
/*virtual*/
double
WeightedShiftDecorator::getShift(CAbstractContract *contract){
	
	double maturity_weight = 1.;
	if (contract){

		double current_date = 
			CCalculationSettingsSingleton::Instance()->getCurrentDate();

		double maturity_normal = 
			CCalculationSettingsSingleton::Instance()->getNormal();

		maturity_weight = 
			contract->getMaturityWeight(maturity_normal, current_date);	
	};

	return (shift ? maturity_weight * shift->getShift(contract) : maturity_weight);
};
//----------------------------------------------------------------//
SpecificShiftDecorator::SpecificShiftDecorator(Shift::ptr shift, long shift_type):
ShiftDecorator(shift){
	this->shift_type = shift_type;
};
//----------------------------------------------------------------//
SpecificShiftDecorator::~SpecificShiftDecorator(){
	clear();
};
//----------------------------------------------------------------//
void
SpecificShiftDecorator::clear(){
	container.clear();
};
//----------------------------------------------------------------//
double
SpecificShiftDecorator::getValue(container_key key){

	value_container::iterator it = container.find(key);

	if (it != container.end()){
		return it->second;
	};

	return 0.;
};
//----------------------------------------------------------------//
/*virtual*/
SpecificShiftDecorator::container_key
SpecificShiftDecorator::getKey(CAbstractContract *contract){
	container_key key = "";

	if (contract){
		CAsset::ptr asset = 
			dynamic_cast<CAsset::ptr>(contract->GetAsset().GetInterfacePtr());

		if (asset){
			BSTR& bsAssetSymbol = asset->m_bstrSymbol.GetBSTR();

			if (bsAssetSymbol){
				char* __symbol = _com_util::ConvertBSTRToString(bsAssetSymbol);
				key = __symbol;
				delete [] __symbol;
			}
		}
	}

	return key;
};
//----------------------------------------------------------------//
void
SpecificShiftDecorator::setValue(container_key key, double value){
	container[key] = value;
};
//----------------------------------------------------------------//
/*virtual*/
double
SpecificShiftDecorator::getShift(CAbstractContract *contract){

	double specific_value = getValue(getKey(contract));

	if (shift_type == 0/*value*/){
		return specific_value;
	}
	else if (shift_type == 1 /*ratio*/){
		return (shift ? 
				shift->getShift(contract) * specific_value : 
				specific_value);
	};

	return 0.;
};
//----------------------------------------------------------------//
//----------------------------------------------------------------//
//----------------------------------------------------------------//
//----------------------------------------------------------------//
//----------------------------------------------------------------//
//----------------------------------------------------------------//
//----------------------------------------------------------------//
//----------------------------------------------------------------//
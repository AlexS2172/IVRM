// MarketSimulationScenario.cpp : Implementation of CMarketSimulationScenario

#include "stdafx.h"
#include "MarketSimulationScenario.h"
#include "Asset.h"

// CMarketSimulationScenario
//--------------------------------------------------------//
STDMETHODIMP
CMarketSimulationScenario::RebuildScenario(){

	if (!m_Simulator)
		m_Simulator = CMarketSimulator::ptr(new CMarketSimulator);

	if (m_TimeShiftScenario == 1/*common shift*/){
		GeneralTimeScenario::ptr time_scenario 
			= GeneralTimeScenario::ptr(new GeneralTimeScenario);

		Shift::ptr default_shift = Shift::ptr(new DefaultShift(m_TimeShiftValue));
		time_scenario->setShifter(default_shift);

		m_Simulator->setTimeScenario(time_scenario);
	}
	else{
		m_Simulator->setTimeScenario(Scenario::ptr());
	};

	if (m_PriceShiftScenario == 1/*common shift*/){
		GeneralPriceScenario::ptr price_scenario
			= GeneralPriceScenario::ptr(new GeneralPriceScenario);

		price_scenario->setShiftUnit(m_PriceShiftUnit);

		Shift::ptr default_shift = Shift::ptr(new DefaultShift(m_PriceShiftValue));
		price_scenario->setShifter(default_shift);

		m_Simulator->setPriceScenario(price_scenario);
	}
	else if (m_PriceShiftScenario == 2 /*correlated shift*/){
		CorrelatedPriceScenario::ptr price_scenario
			= CorrelatedPriceScenario::ptr(new CorrelatedPriceScenario);
		
		Shift::ptr default_shift = Shift::ptr(new DefaultShift(m_PriceShiftValue));
		price_scenario->setShifter(default_shift);

		boost::shared_ptr<CorrelatedPriceScenario> ps = 
			boost::dynamic_pointer_cast<CorrelatedPriceScenario>(price_scenario);

		CAsset* shift_driver = 0;
		if (m_PriceShiftDriver){
			shift_driver = dynamic_cast<CAsset*>(m_PriceShiftDriver.GetInterfacePtr());

			ATLASSERT(shift_driver);

			ps->setShiftDriverAsset(shift_driver);
		};

		price_scenario->setShiftUnit(m_PriceShiftUnit);

		m_Simulator->setPriceScenario(price_scenario);

	}
	else if (m_PriceShiftScenario == 3 /*specific shift*/){
		
		GeneralPriceScenario::ptr price_scenario
			= GeneralPriceScenario::ptr(new GeneralPriceScenario);

		price_scenario->setShiftUnit(m_PriceShiftUnit);
		
		Shift::ptr default_shift = Shift::ptr(new DefaultShift(m_PriceShiftValue));
		SpecificShiftDecorator* shift_decorator = new SpecificShiftDecorator(default_shift, 1);
		
		container& values = m_container[0];
		
		container::iterator it = values.begin();
		container::iterator itEnd = values.end();

		shift_decorator->clear();
		for ( ;it != itEnd; it++){
			shift_decorator->setValue(it->first, it->second);
		};

		price_scenario->setShifter(Shift::ptr(shift_decorator));

		m_Simulator->setPriceScenario(price_scenario);
	}
	else{
		m_Simulator->setPriceScenario(Scenario::ptr());
	};

	if (m_VolatilityShiftScenario == 1 /*common shift*/){
		
		GeneralVolatilityScenario::ptr volatility_scenario
			= GeneralVolatilityScenario::ptr(new GeneralVolatilityScenario);

		Shift::ptr default_shift = 
			Shift::ptr(new DefaultShift(m_VolatilityShiftValue));

		if (m_MaturityWeighted){
			WeightedShiftDecorator*	maturity_weighted_decorator = 
				new WeightedShiftDecorator(Shift::ptr(default_shift));

			volatility_scenario->setShifter(Shift::ptr(maturity_weighted_decorator));
		}
		else{
			volatility_scenario->setShifter(default_shift);
		}

		volatility_scenario->setShiftUnit(m_VolatilityShiftUnit);

		m_Simulator->setVolatilityScenario(volatility_scenario);
	}
	else if (m_VolatilityShiftScenario == 2 /*specific*/){

		GeneralVolatilityScenario::ptr volatility_scenario
			= GeneralVolatilityScenario::ptr(new GeneralVolatilityScenario);
		
		DefaultShift* default_shift = 
			new DefaultShift(m_VolatilityShiftValue);
		
		SpecificShiftDecorator* specific_shift_decorator = 
			new SpecificShiftDecorator(Shift::ptr(default_shift), 1);

		container& values = m_container[1];

		container::iterator it = values.begin();
		container::iterator itEnd = values.end();

		specific_shift_decorator->clear();
		for ( ;it != itEnd; it++){
			specific_shift_decorator->setValue(it->first, it->second);
		};

		if (m_MaturityWeighted){
			WeightedShiftDecorator*	maturity_weighted_decorator = 
				new WeightedShiftDecorator(Shift::ptr(specific_shift_decorator));
			
			volatility_scenario->setShifter(Shift::ptr(maturity_weighted_decorator));
		}
		else{
			volatility_scenario->setShifter(Shift::ptr(specific_shift_decorator));
		}

		volatility_scenario->setShiftUnit(m_VolatilityShiftUnit);

		m_Simulator->setVolatilityScenario(volatility_scenario);
	}
	else{
		m_Simulator->setVolatilityScenario(Scenario::ptr());
	};

	return S_OK;
};
//--------------------------------------------------------//
CMarketSimulator::ptr
CMarketSimulationScenario::getMarketSimulator(){

	if (!m_Simulator)
		RebuildScenario();

	return m_Simulator;
};
//--------------------------------------------------------//
STDMETHODIMP
CMarketSimulationScenario::SetSpecificValues(long Scenario, 
											 SAFEARRAY** SpecificValue){
	try {

		if (!SpecificValue)
			return E_POINTER;

		KeyValue* array_item = NULL;

		long sizeOut = (*SpecificValue)->rgsabound[0].cElements;					

		if (SUCCEEDED(SafeArrayAccessData(*SpecificValue, (void**)&array_item))){
			
			container& values = m_container[Scenario];
			values.clear();

			for (int i = 0; i < sizeOut; i++){

				if (array_item[i].Key){

					char* key = _com_util::ConvertBSTRToString(array_item[i].Key);
					std::string __key = key;
					delete [] key;

					values[__key] = array_item[i].Value;
				}
			};

			SafeArrayUnaccessData(*SpecificValue);

			//SafeArrayDestroy(*SpecificValue);
			//*SpecificValue = NULL;
		};
	}
	catch (...){
		return E_FAIL;
	};
	return S_OK;
};
//--------------------------------------------------------//
STDMETHODIMP
CMarketSimulationScenario::GetSpecificValues(long Scenario, 
											 SAFEARRAY** SpecificValue){
	try{
		
		if( !SpecificValue )
			return E_POINTER;

		if( *SpecificValue ){
			::SafeArrayDestroy(*SpecificValue);
			*SpecificValue = NULL;
		};

		__KeyValue key_value_intial;
		key_value_intial.Init();

		container& values = m_container[Scenario];
		
		SAFEARRAYBOUND sa_bound = {(long)values.size(), 0};
		LPSAFEARRAY safe_array = SafeArrayCreateEx(	VT_RECORD, 
														1, &sa_bound, 
														(PVOID)key_value_intial.GetRecordInfo());
		
		if (safe_array == NULL) 
			return E_FAIL;

		KeyValue* array_item = NULL;

		if(SUCCEEDED(SafeArrayAccessData(safe_array, (void**)&array_item))){

			container::iterator it = values.begin();
			container::iterator itEnd = values.end();

			long i = 0;
			for( ; it != itEnd; ++it){
				array_item[i].Key = _com_util::ConvertStringToBSTR(it->first.c_str()); 	
				array_item[i].Value = it->second;
				i++;
			};

			SafeArrayUnaccessData(safe_array);

			*SpecificValue = safe_array;
		};

	}
	catch (...){
		return E_FAIL;
	};
	return S_OK;
};
//--------------------------------------------------------//


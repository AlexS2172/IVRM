// EtsFutAtom.cpp : Implementation of CEtsFutAtom
#include "stdafx.h"
#include "EtsFutAtom.h"
#include "MarketSimulationScenario.h"
//--------------------------------------------------------------//
STDMETHODIMP 
CEtsFutAtom::putref_Asset(IAsset* NewVal){																	
	ObjectLock lock(this);											
	SetAsset(NewVal);
	return S_OK;													
};																	
//--------------------------------------------------------------//
STDMETHODIMP 
CEtsFutAtom::get_Asset(IAsset** pVal){																	
	if(!pVal){																
		return E_POINTER;											
	};
	
	if(*pVal){																
		(*pVal)->Release();											
		*pVal = NULL;												
	};														
	
	ObjectLock lock(this);											
	*pVal = (IAsset*)m_spAsset;											
	if(*pVal){																
		(*pVal)->AddRef();											
	};

	return S_OK;													
};
//--------------------------------------------------------------//
STDMETHODIMP 
CEtsFutAtom::putref_BaseContract(IContract* NewVal){

	ObjectLock lock(this);											
	SetBaseContract(NewVal);
	return S_OK;													
};																	
//--------------------------------------------------------------//
STDMETHODIMP 
CEtsFutAtom::get_BaseContract(IContract** pVal){

	if(!pVal){																
		return E_POINTER;											
	};

	if(*pVal){																
		(*pVal)->Release();											
		*pVal = NULL;												
	};														

	ObjectLock lock(this);											
	*pVal = (IContract*)m_spBaseContract;											
	if(*pVal){																
		(*pVal)->AddRef();											
	};
	return S_OK;													
};
//--------------------------------------------------------------//
STDMETHODIMP 
CEtsFutAtom::Calculate(/*[in]*/ ICalculationSettings* Settings,
						/*[in]*/ IMarketSimulationScenario* SimulationScenario,
						/*[out, retval]*/ struct IRisks* RetVal)

{
	if (RetVal == NULL) 
		return E_POINTER;

	try {

		CMarketSimulator::ptr simulator = CMarketSimulator::getDefault();

		if (SimulationScenario){
			CMarketSimulationScenario* simulation_scenario = 
				dynamic_cast<CMarketSimulationScenario*>(SimulationScenario);

			ATLASSERT(simulation_scenario != 0);

			simulator = simulation_scenario->getMarketSimulator();		
		};

		CFutureContract::Calculate(simulator)->CopyTo(*RetVal);
	}
	catch (const IvRmException::AbstractException&){
		return E_FAIL;
	}
	catch (...){
		return E_FAIL;
	};

	return S_OK;
};
//--------------------------------------------------------------//
STDMETHODIMP 
CEtsFutAtom::GetRisks(/*[out, retval]*/ struct IRisks* RetVal){

	if (RetVal == NULL)
		return E_POINTER;

	CFutureContract::m_spRisks->CopyTo(*RetVal);

	return S_OK;
};
//------------------------------------------------------------------------------------------------//
STDMETHODIMP 
CEtsFutAtom::GetQuotes(/*[out, retval]*/ struct QuoteUpdateInfo* RetVal){
	
	if (RetVal == NULL)
		return E_POINTER;

	CFutureContract::m_spQuote->CopyTo(*RetVal);

	return S_OK;
};
//------------------------------------------------------------------------------------------------//
STDMETHODIMP 
CEtsFutAtom::FitToMarketVolatility(DOUBLE Value){
	return S_OK;
};
//------------------------------------------------------------------------------------------------//
STDMETHODIMP 
CEtsFutAtom::GetPricingUnit(/*out*/ DOUBLE* Value){
	
	if (Value == NULL){
		return E_POINTER;
	};

	*Value = getPricingUnit();

	return S_OK;
};
//------------------------------------------------------------------------------------------------//
STDMETHODIMP 
CEtsFutAtom::GetContractSizeInAsset(/*out*/ DOUBLE* Value){
	
	if (Value == NULL){
		return E_POINTER;
	};

	*Value = getContractSizeInAsset();

	return S_OK;
};
//------------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------------//
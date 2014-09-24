// SpotContractAtom.cpp : Implementation of CSpotContractAtom

#include "stdafx.h"
#include "SpotContractAtom.h"
#include "MarketSimulationScenario.h"

// CSpotContractAtom
//----------------------------------------------------------------------------------------------//
STDMETHODIMP 
CSpotContractAtom::InterfaceSupportsErrorInfo(REFIID riid){
	static const IID* arr[] = 
	{
		&IID_ISpotContractAtom
	};

	for (int i=0; i < sizeof(arr) / sizeof(arr[0]); i++)
	{
		if (InlineIsEqualGUID(*arr[i],riid))
			return S_OK;
	}
	return S_FALSE;
}
//----------------------------------------------------------------------------------------------//
STDMETHODIMP 
CSpotContractAtom::putref_Asset(IAsset* NewVal){																	
	ObjectLock lock(this);											
	SetAsset(NewVal);
	return S_OK;													
};																	
//----------------------------------------------------------------------------------------------//
STDMETHODIMP 
CSpotContractAtom::get_Asset(IAsset** pVal){																	
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

//------------------------------------------------------------------------------------------------//
STDMETHODIMP 
CSpotContractAtom::putref_BaseContract(IContract* NewVal){																	
	ObjectLock lock(this);											
	SetBaseContract(NewVal);
	return S_OK;													
};																	
//----------------------------------------------------------------------------------------------//
STDMETHODIMP 
CSpotContractAtom::get_BaseContract(IContract** pVal){

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
//------------------------------------------------------------------------------------------------//
STDMETHODIMP 
CSpotContractAtom::Calculate(/*[in]*/ ICalculationSettings* Settings,
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

		CSpotContract::Calculate(simulator)->CopyTo(*RetVal);

	}
	catch (const IvRmException::AbstractException&){
		return E_FAIL;
	}
	catch (...){
		return E_FAIL;
	};

	return S_OK;
};
//------------------------------------------------------------------------------------------------//
STDMETHODIMP 
CSpotContractAtom::GetRisks(/*[out, retval]*/ struct IRisks* RetVal){

	if (RetVal == NULL)
		return E_POINTER;

	CSpotContract::m_spRisks->CopyTo(*RetVal);

	return S_OK;
};
//------------------------------------------------------------------------------------------------//
STDMETHODIMP 
CSpotContractAtom::GetQuotes(/*[out, retval]*/ struct QuoteUpdateInfo* RetVal){

	if (RetVal == NULL)
		return E_POINTER;

	CSpotContract::m_spQuote->CopyTo(*RetVal);

	return S_OK;
};
//------------------------------------------------------------------------------------------------//
STDMETHODIMP 
CSpotContractAtom::FitToMarketVolatility(DOUBLE Value){
	return S_OK;
};	
//------------------------------------------------------------------------------------------------//
STDMETHODIMP 
CSpotContractAtom::GetPricingUnit(/*out*/ DOUBLE* Value){

	if (Value == NULL){
		return E_POINTER;
	};

	*Value = getPricingUnit();

	return S_OK;
};
//------------------------------------------------------------------------------------------------//
STDMETHODIMP 
CSpotContractAtom::GetContractSizeInAsset(/*out*/ DOUBLE* Value){

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
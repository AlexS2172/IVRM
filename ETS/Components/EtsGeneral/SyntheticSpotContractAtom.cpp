// SyntheticSpotContractAtom.cpp : Implementation of CSyntheticSpotContractAtom

#include "stdafx.h"
#include "SyntheticSpotContractAtom.h"

//----------------------------------------------------------------------------------------------//
// CSyntheticSpotContractAtom
//----------------------------------------------------------------------------------------------//
STDMETHODIMP 
CSyntheticSpotContractAtom::putref_Asset(IAsset* NewVal){
	ObjectLock lock(this);											
	SetAsset(NewVal);
	return S_OK;													
};																	
//----------------------------------------------------------------------------------------------//
STDMETHODIMP 
CSyntheticSpotContractAtom::get_Asset(IAsset** pVal){																	

	if(!pVal){																
		return E_POINTER;											
	}																

	if(*pVal){																
		(*pVal)->Release();											
		*pVal = NULL;												
	}																

	ObjectLock lock(this);											
	*pVal = (IAsset*)m_spAsset;											
	if(*pVal)														
	{																
		(*pVal)->AddRef();											
	}																
	return S_OK;													
};
//------------------------------------------------------------------------------------------------//
STDMETHODIMP 
CSyntheticSpotContractAtom::putref_BaseContract(IContract* NewVal){																	
	ObjectLock lock(this);											
	SetBaseContract(NewVal);
	return S_OK;													
};																	
//----------------------------------------------------------------------------------------------//
STDMETHODIMP 
CSyntheticSpotContractAtom::get_BaseContract(IContract** pVal){																	
	
	if(!pVal){																
		return E_POINTER;											
	}																

	if(*pVal){																
		(*pVal)->Release();											
		*pVal = NULL;												
	}																

	ObjectLock lock(this);											
	*pVal = (IContract*)m_spBaseContract;											
	if(*pVal){																
		(*pVal)->AddRef();											
	}																
	return S_OK;													
};
//------------------------------------------------------------------------------------------------//
STDMETHODIMP 
CSyntheticSpotContractAtom::Calculate(	/*[in]*/ ICalculationSettings* Settings,
										/*[in]*/ IMarketSimulationScenario* SimulationScenario,
										/*[out, retval]*/ struct IRisks* RetVal){
	return S_OK;
};
//------------------------------------------------------------------------------------------------//
STDMETHODIMP 
CSyntheticSpotContractAtom::GetRisks(/*[out, retval]*/ struct IRisks* RetVal){
	return S_OK;
};
//------------------------------------------------------------------------------------------------//
STDMETHODIMP 
CSyntheticSpotContractAtom::GetQuotes(/*[out, retval]*/ struct QuoteUpdateInfo* RetVal){
	return S_OK;
};
//------------------------------------------------------------------------------------------------//
STDMETHODIMP 
CSyntheticSpotContractAtom::FitToMarketVolatility(DOUBLE Value){
	return S_OK;
};
//------------------------------------------------------------------------------------------------//
STDMETHODIMP 
CSyntheticSpotContractAtom::GetPricingUnit(/*out*/ DOUBLE* Value){
	return S_OK;
};
//------------------------------------------------------------------------------------------------//
STDMETHODIMP 
CSyntheticSpotContractAtom::GetContractSizeInAsset(/*out*/ DOUBLE* Value){
	return S_OK;
};
//------------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------------//
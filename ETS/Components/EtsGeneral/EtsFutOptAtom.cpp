// EtsFutOptAtom.cpp : Implementation of CEtsFutOptAtom
#include "stdafx.h"
#include "EtsFutOptAtom.h"
#include "MarketSimulationScenario.h"
/////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP 
CEtsFutOptAtom::CopyTo(IEtsFutOptAtom* pDest){
	try {
		if(!pDest)
			EgLib::CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Invalid argument"));

		HRESULT hr = S_OK;
		IEtsFutOptAtomPtr spDest(pDest);

		_CHK(spDest->put_ID(m_nID));
		_CHK(spDest->put_Symbol(m_bstrSymbol));
		_CHK(spDest->put_FutID(m_nBaseContractID));
		_CHK(spDest->put_OptType(m_enOptType));
		_CHK(spDest->put_Expiry(m_dtExpiry));
		_CHK(spDest->put_Strike(m_dStrike));
		_CHK(spDest->put_Series(m_bstrSeries));
		_CHK(spDest->put_ContractName(m_bstrContractName));
		_CHK(spDest->put_ClosePrice(m_dClosePrice));
		_CHK(spDest->put_TheoClosePrice(m_dTheoClosePrice));
		_CHK(spDest->put_ManualPrice(m_dManualPrice));
		_CHK(spDest->put_ImportID(m_bstrImportID));
		_CHK(spDest->put_ExpiryOV(m_dtExpiryOV));
		_CHK(spDest->put_TradingClose(m_dtTradingClose));

		_CHK(spDest->putref_BaseContract(m_spBaseContract));
		_CHK(spDest->putref_Asset(m_spAsset));
	}
	catch(const _com_error& e){
		return Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_IEtsFutOptAtom, e.Error());
	};

	return S_OK;
}
//-------------------------------------------------------------------------------------------//
STDMETHODIMP 
CEtsFutOptAtom::putref_Asset(IAsset* NewVal){																	
	ObjectLock lock(this);											
	SetAsset(NewVal);
	return S_OK;													
};																	
//-------------------------------------------------------------------------------------------//
STDMETHODIMP 
CEtsFutOptAtom::get_Asset(IAsset** pVal){

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
//-------------------------------------------------------------------------------------------//
STDMETHODIMP 
CEtsFutOptAtom::putref_BaseContract(IContract* NewVal){

	ObjectLock lock(this);											
	SetBaseContract(NewVal);
	return S_OK;													
};																	
//-------------------------------------------------------------------------------------------//
STDMETHODIMP 
CEtsFutOptAtom::get_BaseContract(IContract** pVal){

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
//-------------------------------------------------------------------------------------------//
STDMETHODIMP 
CEtsFutOptAtom::Calculate(/*[in]*/ ICalculationSettings* Settings,
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

		CFutureOptionContract::Calculate(simulator)->CopyTo(*RetVal);
	}
	catch (const IvRmException::AbstractException&){
		return E_FAIL;
	}
	catch (...){
		return E_FAIL;
	};

	return S_OK;
}
//-------------------------------------------------------------------------------------------//
STDMETHODIMP 
CEtsFutOptAtom::GetRisks(/*[out, retval]*/ struct IRisks* RetVal){

	if (RetVal == NULL)
		return E_POINTER;

	CFutureOptionContract::m_spRisks->CopyTo(*RetVal);

	return S_OK;
};
//------------------------------------------------------------------------------------------------//
STDMETHODIMP 
CEtsFutOptAtom::GetQuotes(/*[out, retval]*/ struct QuoteUpdateInfo* RetVal){

	if (RetVal == NULL)
		return E_POINTER;

	CFutureOptionContract::m_spQuote->CopyTo(*RetVal);

	return S_OK;
};
//------------------------------------------------------------------------------------------------//
STDMETHODIMP 
CEtsFutOptAtom::FitToMarketVolatility(DOUBLE Value){
	CFutureOptionContract::FitToMarketVolatility();
	return S_OK;
};
//------------------------------------------------------------------------------------------------//
STDMETHODIMP 
CEtsFutOptAtom::get_Root(IContractRoot **pValue){

	HRESULT hrStatus = S_OK;
	try{
		_CHK(pValue != 0 ? S_OK : E_POINTER,
			_T("Invalid pointer of pValue"));

		if(*pValue){																
			(*pValue)->Release();											
			*pValue = NULL;												
		};

		ObjectLock lock(this);											
		*pValue = (IContractRoot*)m_spRoot;											
		if(*pValue){																
			(*pValue)->AddRef();											
		}
	}
	catch (_com_error& err){
		TRACE_COM_ERROR(err);
	}
	catch (...){
		TRACE_UNKNOWN_ERROR();
	}
	return hrStatus;
}
//------------------------------------------------------------------------------------------------//
STDMETHODIMP 
CEtsFutOptAtom::GetPricingUnit(/*out*/ DOUBLE* Value){
	
	if (Value == NULL){
		return E_POINTER;
	};

	*Value = getPricingUnit();

	return S_OK;
};
//------------------------------------------------------------------------------------------------//
STDMETHODIMP 
CEtsFutOptAtom::GetContractSizeInAsset(/*out*/ DOUBLE* Value){

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
//------------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------------//
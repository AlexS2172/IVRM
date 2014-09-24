// EtsContractAtom.cpp : Implementation of CEtsContractAtom

#include "stdafx.h"
#include "EtsContractAtom.h"

/////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CEtsContractAtom::get_TheoClosePrice(DOUBLE* pRetVal)
{
	try
	{
		HRESULT hr = S_OK;

		if(!pRetVal)
			EgLib::CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Invalid argument"));

		*pRetVal = GetTheoClosePrice();

	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_IEtsContractAtom, e.Error());
	}

	return S_OK;
}
/////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CEtsContractAtom::get_ClosePrice(DOUBLE* pRetVal)
{
	try
	{
		HRESULT hr = S_OK;

		if(!pRetVal)
			EgLib::CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Invalid argument"));

		*pRetVal = GetClosePrice();

	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_IEtsContractAtom, e.Error());
	}

	return S_OK;
}
/////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CEtsContractAtom::get_ManualPrice(DOUBLE* pRetVal)
{
	try
	{
		HRESULT hr = S_OK;

		if(!pRetVal)
			EgLib::CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Invalid argument"));

		*pRetVal = GetManualPrice();

	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_IEtsContractAtom, e.Error());
	}

	return S_OK;
}

/////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CEtsContractAtom::get_ID(LONG* pRetVal)
{
	try
	{
		HRESULT hr = S_OK;

		if(!pRetVal)
			EgLib::CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Invalid argument"));

		*pRetVal = GetID();

	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_IEtsContractAtom, e.Error());
	}

	return S_OK;
}

/////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CEtsContractAtom::get_ContractType(enum EtsContractTypeEnum* pRetVal)
{
	try
	{
		HRESULT hr = S_OK;

		if(!pRetVal)
			EgLib::CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Invalid argument"));
		
		*pRetVal  = GetType();

	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_IEtsContractAtom, e.Error());
	}

	return S_OK;
}

/////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CEtsContractAtom::get_Symbol(BSTR* pRetVal)
{
	try
	{
		HRESULT hr = S_OK;

		if(!pRetVal)
			EgLib::CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Invalid argument"));

		if(*pRetVal)
			::SysFreeString(*pRetVal);

		*pRetVal = GetSymbol();
		
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_IEtsContractAtom, e.Error());
	}

	return S_OK;
}

/////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CEtsContractAtom::get_ContractName(BSTR* pRetVal)
{
	try
	{
		HRESULT hr = S_OK;

		if(!pRetVal)
			EgLib::CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Invalid argument"));

		if(*pRetVal)
			::SysFreeString(*pRetVal);

		*pRetVal = GetContractName();
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_IEtsContractAtom, e.Error());
	}

	return S_OK;
}
/////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CEtsContractAtom::put_ID(LONG Val)
{
	try
	{
		HRESULT hr = S_OK;

		/*if(!pRetVal)
			EgLib::CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Invalid argument"));

		*pRetVal = GetID();*/

	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_IEtsContractAtom, e.Error());
	}

	return S_OK;
}

/////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CEtsContractAtom::put_ContractType(enum EtsContractTypeEnum Val)
{
	try
	{
		HRESULT hr = S_OK;

		//if(!pRetVal)
		//	EgLib::CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Invalid argument"));

		//*pRetVal  = GetType();

	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_IEtsContractAtom, e.Error());
	}

	return S_OK;
}

/////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CEtsContractAtom::put_Symbol(BSTR Val)
{
	try
	{
		HRESULT hr = S_OK;

		//if(!pRetVal)
		//	EgLib::CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Invalid argument"));

		//if(*pRetVal)
		//	::SysFreeString(*pRetVal);

		//*pRetVal = GetSymbol();

	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_IEtsContractAtom, e.Error());
	}

	return S_OK;
}

/////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CEtsContractAtom::put_ContractName(BSTR Val)
{
	try
	{
		HRESULT hr = S_OK;

		/*if(!pRetVal)
			EgLib::CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Invalid argument"));

		if(*pRetVal)
			::SysFreeString(*pRetVal);

		*pRetVal = GetContractName();*/
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_IEtsContractAtom, e.Error());
	}

	return S_OK;
}
/////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CEtsContractAtom::put_ManualPrice(DOUBLE Val)
{
	try
	{
		HRESULT hr = S_OK;

		SetManualPrice(Val);
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_IEtsContractAtom, e.Error());
	}

	return S_OK;
}
/////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CEtsContractAtom::put_ClosePrice(DOUBLE Val)
{
	try
	{
		HRESULT hr = S_OK;

		SetClosePrice(Val);
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_IEtsContractAtom, e.Error());
	}

	return S_OK;
}
/////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CEtsContractAtom::put_TheoClosePrice(DOUBLE Val)
{
	try
	{
		HRESULT hr = S_OK;

		SetTheoClosePrice(Val);
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_IEtsContractAtom, e.Error());
	}

	return S_OK;
}
//-----------------------------------------------------------------------------//
STDMETHODIMP CEtsContractAtom::Calculate(/*[in]*/ ICalculationSettings* Settings,
										 /*[in]*/ IMarketSimulationScenario* SimulationScenario,
										/*[out, retval]*/ struct IRisks* RetVal)
{
	return E_FAIL;
};
//-----------------------------------------------------------------------------//
STDMETHODIMP CEtsContractAtom::GetRisks(/*[out, retval]*/ struct IRisks* RetVal)
{
	return S_OK;
};
//-----------------------------------------------------------------------------//
STDMETHODIMP CEtsContractAtom::GetQuotes(/*[out, retval]*/ struct QuoteUpdateInfo* RetVal)
{
	return S_OK;
};
//-----------------------------------------------------------------------------//
STDMETHODIMP CEtsContractAtom::FitToMarketVolatility(DOUBLE Value)
{
	return S_OK;
};
//-----------------------------------------------------------------------------//
STDMETHODIMP CEtsContractAtom::putref_Asset(IAsset* NewVal)								
{																	
	ObjectLock lock(this);											
	SetAsset(NewVal);
	return S_OK;													
};																	
//-----------------------------------------------------------------------------//
STDMETHODIMP CEtsContractAtom::get_Asset(IAsset** pVal)									
{																	
	if(!pVal)														
	{																
		return E_POINTER;											
	}																

	if(*pVal)														
	{																
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
//-----------------------------------------------------------------------------//
STDMETHODIMP CEtsContractAtom::putref_BaseContract(IContract* NewVal)								
{																	
	ObjectLock lock(this);											
	SetBaseContract(NewVal);
	return S_OK;													
};																	
//-----------------------------------------------------------------------------//
STDMETHODIMP CEtsContractAtom::get_BaseContract(IContract** pVal)									
{																	
	if(!pVal)														
	{																
		return E_POINTER;											
	}																

	if(*pVal)														
	{																
		(*pVal)->Release();											
		*pVal = NULL;												
	}																

	ObjectLock lock(this);											
	*pVal = (IContract*)m_spBaseContract;											
	if(*pVal)														
	{																
		(*pVal)->AddRef();											
	}																
	return S_OK;													
};
//------------------------------------------------------------------------------------------------//
STDMETHODIMP 
CEtsContractAtom::GetPricingUnit(/*out*/ DOUBLE* Value){
	if (Value == NULL){
		return E_POINTER;
	};
	*Value = getPricingUnit();
	return S_OK;
};
//------------------------------------------------------------------------------------------------//
STDMETHODIMP 
CEtsContractAtom::GetContractSizeInAsset(/*out*/ DOUBLE* Value){
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
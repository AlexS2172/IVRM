// MmOrderAtom.cpp : Implementation of CMmOrderAtom

#include "stdafx.h"
#include "MmOrderAtom.h"

/////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CMmOrderAtom::get_ContractID(LONG* pRetVal)
{
	try
	{
		HRESULT hr = S_OK;

		if(!pRetVal)
			EgLib::CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Invalid argument"));

		if(m_spOpt != NULL)
		{
			__CHECK_HRESULT3(m_spOpt->get_ID(pRetVal));
		}
		else if(m_spUnd != NULL)
		{
			__CHECK_HRESULT3(m_spUnd->get_ID(pRetVal));
		}
		else
			*pRetVal = 0;
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_IMmOrderAtom, e.Error());
	}

	return S_OK;
}

/////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CMmOrderAtom::get_ContractType(enum EtsContractTypeEnum* pRetVal)
{
	try
	{
		HRESULT hr = S_OK;

		if(!pRetVal)
			EgLib::CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Invalid argument"));

		if(m_spOpt != NULL)
			*pRetVal = enCtOption;
		else
			__CHECK_HRESULT3(m_spUnd->get_UndType(pRetVal));
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_IMmOrderAtom, e.Error());
	}

	return S_OK;
}

/////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CMmOrderAtom::get_Symbol(BSTR* pRetVal)
{
	try
	{
		HRESULT hr = S_OK;

		if(!pRetVal)
			EgLib::CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Invalid argument"));

		if(*pRetVal)
			::SysFreeString(*pRetVal);

		if(m_spOpt != NULL)
		{
			__CHECK_HRESULT3(m_spOpt->get_Symbol(pRetVal));
		}
		else if(m_spUnd != NULL)
		{
			__CHECK_HRESULT3(m_spUnd->get_Symbol(pRetVal));
		}
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_IMmOrderAtom, e.Error());
	}

	return S_OK;
}

/////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CMmOrderAtom::get_UnderlyingID(LONG* pRetVal)
{
	try
	{
		HRESULT hr = S_OK;

		if(!pRetVal)
			EgLib::CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Invalid argument"));

		if(m_spUnd != NULL)
		{
			__CHECK_HRESULT3(m_spUnd->get_ID(pRetVal));
		}
		else
			*pRetVal = 0;
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_IMmOrderAtom, e.Error());
	}

	return S_OK;
}

/////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CMmOrderAtom::get_OptRootID(LONG* pRetVal)
{
	try
	{
		HRESULT hr = S_OK;

		if(!pRetVal)
			EgLib::CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Invalid argument"));

		if(m_spOptRoot != NULL)
		{
			__CHECK_HRESULT3(m_spOptRoot->get_ID(pRetVal));
		}
		else
			*pRetVal = 0;
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_IMmOrderAtom, e.Error());
	}

	return S_OK;
}

/////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CMmOrderAtom::get_OrderTypeString(BSTR* pRetVal)
{
	try
	{
		if(!pRetVal)
			EgLib::CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Invalid argument"));

		if(*pRetVal)
			::SysFreeString(*pRetVal);

		switch(m_enOrderType)
		{
			case enOrtMarket:
				*pRetVal = ::SysAllocString(L"MKT");
				break;

			case enOrtLimit:
				*pRetVal = ::SysAllocString(L"LMT");
				break;

			default:
				*pRetVal = ::SysAllocString(L"???");
		}
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_IMmOrderAtom, e.Error());
	}

	return S_OK;
}

/////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CMmOrderAtom::get_OrderStatusString(BSTR* pRetVal)
{
	try
	{
		if(!pRetVal)
			EgLib::CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Invalid argument"));

		if(*pRetVal)
			::SysFreeString(*pRetVal);

		switch(m_enStatus)
		{
			case enOrsNew:
				*pRetVal = ::SysAllocString(L"NEW");
				break;

			case enOrsPartiallyFilled:
				*pRetVal = ::SysAllocString(L"PART");
				break;

			case enOrsFilled:
				*pRetVal = ::SysAllocString(L"FILL");
				break;

			case enOrsDoneForDay:
				*pRetVal = ::SysAllocString(L"DFD");
				break;

			case enOrsCanceled:
				*pRetVal = ::SysAllocString(L"CXL");
				break;

			case enOrsReplaced:
				*pRetVal = ::SysAllocString(L"CXLR");
				break;

			case enOrsPendingCancelReplace:
				*pRetVal = ::SysAllocString(L"@CXLR");
				break;

			case enOrsStopped:
				*pRetVal = ::SysAllocString(L"STOP");
				break;

			case enOrsRejected:
				*pRetVal = ::SysAllocString(L"RJCT");
				break;

			case enOrsSuspended:
				*pRetVal = ::SysAllocString(L"SPND");
				break;

			case enOrsPendingNew:
				*pRetVal = ::SysAllocString(L"@NEW");
				break;

			case enOrsCalculated:
				*pRetVal = ::SysAllocString(L"CALC");
				break;

			case enOrsExpired:
				*pRetVal = ::SysAllocString(L"EXP");
				break;

			default:
				*pRetVal = ::SysAllocString(L"???");
		}
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_IMmOrderAtom, e.Error());
	}

	return S_OK;
}



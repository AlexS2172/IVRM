// EtsOptRootByNameColl.cpp : Implementation of CEtsOptRootByNameColl

#include "stdafx.h"
#include "EtsOptRootByNameColl.h"

/////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CEtsOptRootByNameColl::Add(BSTR Key, IEtsOptRootAtom* Value, IEtsOptRootAtom** pRetVal)
{
	__CHECK_POINTER(pRetVal);

	try
	{
		if(m_coll.find(CComBSTR(Key)) != m_coll.end())
		{
			EgLib::CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Option root with the same key is already exists."));
		}

		CComObject<CEtsOptRootAtom>* pUnd;

		if(!Value)
		{
			_CHK(CComObject<CEtsOptRootAtom>::CreateInstance(&pUnd), _T("Fail to add option root."));
			pUnd->AddRef();
			if(FAILED(IEtsOptRootByNameCollImpl::Add(Key, pUnd)))
			{
				pUnd->Release();
				EgLib::CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Fail to add option root."));
			}
			*pRetVal = pUnd;
		}
		else
		{
			_CHK(IEtsOptRootByNameCollImpl::Add(Key, Value), _T("Fail to add option root."));
			Value->AddRef();
			*pRetVal = Value;
		}
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_IEtsOptRootByNameColl, e.Error());
	}

	return S_OK;
}


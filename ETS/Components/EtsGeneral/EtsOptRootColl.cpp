// EtsOptRootColl.cpp : Implementation of CEtsOptRootColl

#include "stdafx.h"
#include "EtsOptRootColl.h"

/////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CEtsOptRootColl::Add(LONG Key, BSTR SortKey, IEtsOptRootAtom* Value, IEtsOptRootAtom** pRetVal)
{
	try
	{
		if(m_collRef.find(Key) != m_collRef.end())
		{
			EgLib::CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Option root with the same key is already exists."));
		}

		if(!Value)
		{
			CComObject<CEtsOptRootAtom>* pNewVal;

			_CHK(CComObject<CEtsOptRootAtom>::CreateInstance(&pNewVal), _T("Fail to add option root."));
			pNewVal->AddRef();
			if(FAILED(IEtsOptRootCollImpl::Add(Key, CComBSTR(SortKey), pNewVal)))
			{
				pNewVal->Release();
				EgLib::CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Fail to add option root."));
			}
			*pRetVal = pNewVal;
		}
		else
		{
			_CHK(IEtsOptRootCollImpl::Add(Key, CComBSTR(SortKey), Value), _T("Fail to add option root."));
			Value->AddRef();
			*pRetVal = Value;
		}
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_IEtsOptRootColl, e.Error());
	}

	return S_OK;
}


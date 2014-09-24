// MmHsUndColl.cpp : Implementation of CMmHsUndColl

#include "stdafx.h"
#include "MmHsUndColl.h"


// CMmHsUndColl

STDMETHODIMP CMmHsUndColl::Add(LONG Key, IMmHsUndAtom* Value, IMmHsUndAtom** pRetVal)
{
    __CHECK_POINTER(pRetVal);

	try
	{
		if(m_coll.find(Key) != m_coll.end())
		{
			CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Value with the same key already exists"));
		}

		CComObject<CMmHsUndAtom>* pValue;

		if(!Value)
		{
			__CHECK_HRESULT(CComObject<CMmHsUndAtom>::CreateInstance(&pValue), _T("Failed to add value"));
			pValue->AddRef();
			if(FAILED(IMmHsUndCollImpl::Add(Key, pValue)))
			{
				pValue->Release();
				CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Failed to add value"));
			}
			*pRetVal = pValue;
		}
		else
		{
			__CHECK_HRESULT(IMmHsUndCollImpl::Add(Key, Value), _T("Failed to add value"));
			Value->AddRef();
			*pRetVal = Value;
		}
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)CComErrorWrapper::ErrorDescription(e), IID_IMmHsUndColl, e.Error());
	}

	return S_OK;
}
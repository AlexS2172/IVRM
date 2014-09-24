// MmHsOptColl.cpp : Implementation of CMmHsOptColl

#include "stdafx.h"
#include "MmHsOptColl.h"


// CMmHsOptColl

STDMETHODIMP CMmHsOptColl::Add (LONG Key, IMmHsOptAtom* Value, IMmHsOptAtom** pRetVal)
{
	__CHECK_POINTER(pRetVal);

	try
	{
		if(m_coll.find(Key) != m_coll.end())
		{
			CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Value with the same key already exists"));
		}

		CComObject<CMmHsOptAtom>* pValue;

		if(!Value)
		{
			__CHECK_HRESULT(CComObject<CMmHsOptAtom>::CreateInstance(&pValue), _T("Fail to add value"));
			pValue->AddRef();
			if(FAILED(IMmHsOptCollImpl::Add(Key, pValue)))
			{
				pValue->Release();
				CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Fail to add value"));
			}
			*pRetVal = pValue;
		}
		else
		{
			__CHECK_HRESULT(IMmHsOptCollImpl::Add(Key, Value), _T("Fail to add value"));
			Value->AddRef();
			*pRetVal = Value;
		}
	}
	catch(const _com_error& e)
	{
		return Error( (PTCHAR)CComErrorWrapper::ErrorDescription(e), IID_IMmHsOptColl, e.Error() );
	}

	return S_OK;
}

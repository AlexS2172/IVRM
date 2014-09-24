// MmVaExpColl.cpp : Implementation of CMmVaExpColl

#include "stdafx.h"
#include "MmVaExpColl.h"


/////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CMmVaExpColl::Add(DATE Key, IMmVaExpAtom* Value, IMmVaExpAtom** pRetVal)
{
	__CHECK_POINTER(pRetVal);

	try
	{
		if(m_coll.find(Key) != m_coll.end())
		{
			CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Expiry with the same key is already exists."));
		}

		CComObject<CMmVaExpAtom>* pExp;

		if(!Value)
		{
			__CHECK_HRESULT(CComObject<CMmVaExpAtom>::CreateInstance(&pExp), _T("Fail to add expiry."));
			pExp->AddRef();
			if(FAILED(IMmVaExpCollImpl::Add(Key, pExp)))
			{
				pExp->Release();
				CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Fail to add expiry."));
			}
			*pRetVal = pExp;
		}
		else
		{
			__CHECK_HRESULT(IMmVaExpCollImpl::Add(Key, Value), _T("Fail to add expiry."));
			Value->AddRef();
			*pRetVal = Value;
		}
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)CComErrorWrapper::ErrorDescription(e), IID_IMmVaExpColl, e.Error());
	}

	return S_OK;
}


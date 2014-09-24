// MmGaExpColl.cpp : Implementation of CMmGaExpColl

#include "stdafx.h"
#include "MmGaExpColl.h"


/////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CMmGaExpColl::Add(LONG Key, IMmGaExpAtom* Value, IMmGaExpAtom** pRetVal)
{
	__CHECK_POINTER(pRetVal);

	try
	{
		if(m_coll.find(Key) != m_coll.end())
		{
			CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Expiry with the same key is already exists."));
		}

		CComObject<CMmGaExpAtom>* pNewVal;

		if(!Value)
		{
			__CHECK_HRESULT(CComObject<CMmGaExpAtom>::CreateInstance(&pNewVal), _T("Fail to add expiry."));
			pNewVal->AddRef();
			if(FAILED(IMmGaExpCollImpl::Add(Key, pNewVal)))
			{
				pNewVal->Release();
				CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Fail to add expiry."));
			}
			*pRetVal = pNewVal;
		}
		else
		{
			__CHECK_HRESULT(IMmGaExpCollImpl::Add(Key, Value), _T("Fail to add expiry."));
			Value->AddRef();
			*pRetVal = Value;
		}
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)CComErrorWrapper::ErrorDescription(e), IID_IMmGaExpColl, e.Error());
	}

	return S_OK;
}



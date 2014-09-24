// MmGaExpTotalColl.cpp : Implementation of CMmGaExpTotalColl

#include "stdafx.h"
#include "MmGaExpTotalColl.h"


/////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CMmGaExpTotalColl::Add(LONG Key, IMmGaExpTotalAtom* Value, IMmGaExpTotalAtom** pRetVal)
{
	__CHECK_POINTER(pRetVal);

	try
	{
		if(m_coll.find(Key) != m_coll.end())
		{
			CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Expiry with the same key is already exists."));
		}

		CComObject<CMmGaExpTotalAtom>* pNewVal;

		if(!Value)
		{
			__CHECK_HRESULT(CComObject<CMmGaExpTotalAtom>::CreateInstance(&pNewVal), _T("Fail to add expiry."));
			pNewVal->AddRef();
			if(FAILED(IMmGaExpTotalCollImpl::Add(Key, pNewVal)))
			{
				pNewVal->Release();
				CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Fail to add expiry."));
			}
			*pRetVal = pNewVal;
		}
		else
		{
			__CHECK_HRESULT(IMmGaExpTotalCollImpl::Add(Key, Value), _T("Fail to add expiry."));
			Value->AddRef();
			*pRetVal = Value;
		}
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)CComErrorWrapper::ErrorDescription(e), IID_IMmGaExpTotalColl, e.Error());
	}

	return S_OK;
}



// MmRvGroupingColl.cpp : Implementation of CMmRvGroupingColl

#include "stdafx.h"
#include "MmRvGroupingColl.h"


/////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CMmRvGroupingColl::Add(LONG Key, IMmRvGroupingAtom* Value, IMmRvGroupingAtom** pRetVal)
{
	__CHECK_POINTER(pRetVal);

	try
	{
		if(m_coll.find(Key) != m_coll.end())
		{
			CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Expiry with the same key is already exists."));
		}

		CComObject<CMmRvGroupingAtom>* pNewVal;

		if(!Value)
		{
			__CHECK_HRESULT(CComObject<CMmRvGroupingAtom>::CreateInstance(&pNewVal), _T("Fail to add expiry."));
			pNewVal->AddRef();
			if(FAILED(IMmRvGroupingCollImpl::Add(Key, pNewVal)))
			{
				pNewVal->Release();
				CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Fail to add expiry."));
			}
			*pRetVal = pNewVal;
		}
		else
		{
			__CHECK_HRESULT(IMmRvGroupingCollImpl::Add(Key, Value), _T("Fail to add expiry."));
			Value->AddRef();
			*pRetVal = Value;
		}
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)CComErrorWrapper::ErrorDescription(e), IID_IMmRvGroupingColl, e.Error());
	}

	return S_OK;
}

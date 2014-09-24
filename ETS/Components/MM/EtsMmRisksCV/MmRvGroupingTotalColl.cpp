// MmRvGroupingTotalColl.cpp : Implementation of CMmRvGroupingTotalColl

#include "stdafx.h"
#include "MmRvGroupingTotalColl.h"


/////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CMmRvGroupingTotalColl::Add(LONG Key, IMmRvGroupingTotalAtom* Value, IMmRvGroupingTotalAtom** pRetVal)
{
	__CHECK_POINTER(pRetVal);

	try
	{
		if(m_coll.find(Key) != m_coll.end())
		{
			CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Expiry with the same key is already exists."));
		}

		CComObject<CMmRvGroupingTotalAtom>* pNewVal;

		if(!Value)
		{
			__CHECK_HRESULT(CComObject<CMmRvGroupingTotalAtom>::CreateInstance(&pNewVal), _T("Fail to add expiry."));
			pNewVal->AddRef();
			if(FAILED(IMmRvGroupingTotalCollImpl::Add(Key, pNewVal)))
			{
				pNewVal->Release();
				CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Fail to add expiry."));
			}
			*pRetVal = pNewVal;
		}
		else
		{
			__CHECK_HRESULT(IMmRvGroupingTotalCollImpl::Add(Key, Value), _T("Fail to add expiry."));
			Value->AddRef();
			*pRetVal = Value;
		}
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)CComErrorWrapper::ErrorDescription(e), IID_IMmRvGroupingTotalColl, e.Error());
	}

	return S_OK;
}

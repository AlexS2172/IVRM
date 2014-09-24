// MmOrderByReqColl.cpp : Implementation of CMmOrderByReqColl

#include "stdafx.h"
#include "MmOrderByReqColl.h"


/////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CMmOrderByReqColl::Add(BSTR Key, DATE SortKey, IMmOrderAtom* Value, IMmOrderAtom** pRetVal)
{
	__CHECK_POINTER(pRetVal);

	try
	{
		_bstr_t bsKey(Key);
		if(m_collRef.find(bsKey) != m_collRef.end())
		{
			EgLib::CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Order with the same key already exists."));
		}

		if(!Value)
		{
			CComObject<CMmOrderAtom>* pNewVal;
			__CHECK_HRESULT(CComObject<CMmOrderAtom>::CreateInstance(&pNewVal), _T("Failed to add order."));
			pNewVal->AddRef();
			if(FAILED(IMmOrderByReqCollImpl::Add(bsKey, SortKey, pNewVal)))
			{
				pNewVal->Release();
				EgLib::CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Failed to add order."));
			}
			*pRetVal = pNewVal;
		}
		else
		{
			__CHECK_HRESULT(IMmOrderByReqCollImpl::Add(bsKey, SortKey, Value), _T("Failed to add order."));
			Value->AddRef();
			*pRetVal = Value;
		}
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_IMmOrderByReqColl, e.Error());
	}

	return S_OK;
}
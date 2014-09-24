// MmOrderColl.cpp : Implementation of CMmOrderColl

#include "stdafx.h"
#include "MmOrderAtom.h"
#include "MmOrderColl.h"


STDMETHODIMP CMmOrderColl::Add(LONG Key, DATE SortKey, IMmOrderAtom* Value, IMmOrderAtom** pRetVal)
{
	__CHECK_POINTER(pRetVal);

	try
	{
		if(m_coll.find(Key) != m_coll.end())
		{
			EgLib::CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Order with the same key already exists."));
		}

		if(!Value)
		{
			CComObject<CMmOrderAtom>* pNewVal;
			__CHECK_HRESULT(CComObject<CMmOrderAtom>::CreateInstance(&pNewVal), _T("Failed to add order."));
			pNewVal->AddRef();
			if(FAILED(IMmOrderCollImpl::Add(Key, SortKey, pNewVal)))
			{
				pNewVal->Release();
				EgLib::CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Failed to add order."));
			}
			*pRetVal = pNewVal;
		}
		else
		{
			__CHECK_HRESULT(IMmOrderCollImpl::Add(Key, SortKey, Value), _T("Failed to add order."));
			Value->AddRef();
			*pRetVal = Value;
		}
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_IMmOrderColl, e.Error());
	}

	return S_OK;
}
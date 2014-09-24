// MmOrderExecColl.cpp : Implementation of CMmOrderExecColl

#include "stdafx.h"
#include "MmOrderExecColl.h"


STDMETHODIMP CMmOrderExecColl::Add(LONG Key, DATE SortKey, IMmOrderExecAtom* Value, IMmOrderExecAtom** pRetVal)
{
	__CHECK_POINTER(pRetVal);

	try
	{
		if(m_coll.find(Key) != m_coll.end())
		{
			EgLib::CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Order execution with the same key already exists."));
		}

		if(!Value)
		{
			CComObject<CMmOrderExecAtom>* pNewVal;
			__CHECK_HRESULT(CComObject<CMmOrderExecAtom>::CreateInstance(&pNewVal), _T("Failed to add order execution."));
			pNewVal->AddRef();
			if(FAILED(IMmOrderExecCollImpl::Add(Key, SortKey, pNewVal)))
			{
				pNewVal->Release();
				EgLib::CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Failed to add order execution."));
			}
			*pRetVal = pNewVal;
		}
		else
		{
			__CHECK_HRESULT(IMmOrderExecCollImpl::Add(Key, SortKey, Value), _T("Failed to add order execution."));
			Value->AddRef();
			*pRetVal = Value;
		}
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_IMmOrderExecColl, e.Error());
	}

	return S_OK;
}
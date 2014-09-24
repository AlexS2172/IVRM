// MmOrderExecDestDestColl.cpp : Implementation of CMmOrderExecDestDestColl

#include "stdafx.h"
#include "MmOrderExecDestColl.h"


STDMETHODIMP CMmOrderExecDestColl::Add(LONG Key, BSTR SortKey, IMmOrderExecDestAtom* Value, IMmOrderExecDestAtom** pRetVal)
{
	__CHECK_POINTER(pRetVal);

	try
	{
		if(m_collRef.find(Key) != m_collRef.end())
		{
			EgLib::CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Order execution destination with the same key already exists."));
		}

		if(!Value)
		{
			CComObject<CMmOrderExecDestAtom>* pNewVal;
			__CHECK_HRESULT(CComObject<CMmOrderExecDestAtom>::CreateInstance(&pNewVal), _T("Failed to add order execution destination."));
			pNewVal->AddRef();
			if(FAILED(IMmOrderExecDestCollImpl::Add(Key, CComBSTR(SortKey), pNewVal)))
			{
				pNewVal->Release();
				EgLib::CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Failed to add order execution destination."));
			}
			*pRetVal = pNewVal;
		}
		else
		{
			__CHECK_HRESULT(IMmOrderExecDestCollImpl::Add(Key, CComBSTR(SortKey), Value), _T("Failed to add order execution destination."));
			Value->AddRef();
			*pRetVal = Value;
		}
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_IMmOrderExecDestColl, e.Error());
	}

	return S_OK;
}
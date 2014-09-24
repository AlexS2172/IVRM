// ContractColl.cpp : Implementation of CContractColl

#include "stdafx.h"
#include "ContractColl.h"

// CContractColl
/////////////////////////////////////////////////////////////////////////////
STDMETHODIMP CContractColl::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* arr[] = 
	{
		&IID_IContractColl
	};

	for (int i=0; i < sizeof(arr) / sizeof(arr[0]); i++)
	{
		if (InlineIsEqualGUID(*arr[i],riid))
			return S_OK;
	}
	return S_FALSE;
}

/////////////////////////////////////////////////////////////////////////////
STDMETHODIMP CContractColl::Add(LONG Key, BSTR SortKey, IContract* Value, IContract** pRetVal)
{
	try
	{
		if(m_collRef.find(Key) == m_collRef.end())
		{
			//EgLib::CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Contract with the same key is already exists."));

			_bstr_t bsSortKey(SortKey);
			if (Value != NULL)
			{
				_CHK(IContractCollImpl::Add(Key, bsSortKey, Value), _T("Fail to add contract."));
				Value->AddRef();
				*pRetVal = Value;
			}
			else
				return E_POINTER;
		}
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_IContractColl, e.Error());
	}
	return S_OK;
}
/////////////////////////////////////////////////////////////////////////////

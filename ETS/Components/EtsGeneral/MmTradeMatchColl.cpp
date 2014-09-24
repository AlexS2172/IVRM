// MmTradeMatchColl.cpp : Implementation of CMmTradeMatchColl

#include "stdafx.h"

//#include "MmTradeInfoAtom.h"
#include "MmTradeInfoColl.h"
#include "MmTradeMatchAtom.h"
#include "MmTradeMatchColl.h"

// CMmTradeMatchColl

STDMETHODIMP CMmTradeMatchColl::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* arr[] = 
	{
		&IID_IMmTradeMatchColl
	};

	for (int i=0; i < sizeof(arr) / sizeof(arr[0]); i++)
	{
		if (InlineIsEqualGUID(*arr[i],riid))
			return S_OK;
	}
	return S_FALSE;
}

STDMETHODIMP CMmTradeMatchColl::Add(LONG Key, DATE SortKey, IMmTradeMatchAtom* Value, IMmTradeMatchAtom** pRetVal)
{
	__CHECK_POINTER(pRetVal);

	try
	{

		if(m_coll.find(Key) != m_coll.end())
		{
			EgLib::CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Trade with the same key already exists."));
		}

		if(!Value)
		{
			CComObject<CMmTradeMatchAtom>* pNewVal;
			__CHECK_HRESULT(CComObject<CMmTradeMatchAtom>::CreateInstance(&pNewVal), _T("Failed to add trade."));
			pNewVal->AddRef();
			if(FAILED(IMmTradeMatchCollImpl::Add(Key, SortKey, pNewVal)))
			{
				pNewVal->Release();
				EgLib::CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Failed to add trade."));
			}
			*pRetVal = pNewVal;
		}
		else
		{
			__CHECK_HRESULT(IMmTradeMatchCollImpl::Add(Key, SortKey, Value), _T("Failed to add trade."));
			Value->AddRef();
			*pRetVal = Value;
		}
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_IMmTradeInfoColl, e.Error());
	}

	return S_OK;
}


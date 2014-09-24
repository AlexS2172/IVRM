// MmShStrategyGroupColl.cpp : Implementation of CMmShStrategyGroupColl

#include "stdafx.h"
#include "MmShStrategyGroupColl.h"


// CMmShStrategyGroupColl
/////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CMmShStrategyGroupColl::Add(LONG Key, BSTR SortKey, IMmShStrategyGroupAtom* Value, IMmShStrategyGroupAtom** pRetVal)
{
	try
	{
		if(m_collRef.find(Key) != m_collRef.end())
		{
			CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Strategy group with the same key is already exists."));
		}

		if(!Value)
		{
			CComObject<CMmShStrategyGroupAtom>* pNewVal;

			_CHK(CComObject<CMmShStrategyGroupAtom>::CreateInstance(&pNewVal), _T("Fail to add strategy group."));
			pNewVal->AddRef();
			if(FAILED(IMmShStrategyGroupCollImpl::Add(Key, CComBSTR(SortKey), pNewVal)))
			{
				pNewVal->Release();
				CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Fail to add strategy group."));
			}
			*pRetVal = pNewVal;
		}
		else
		{
			_CHK(IMmShStrategyGroupCollImpl::Add(Key, CComBSTR(SortKey), Value), _T("Fail to add strategy group."));
			Value->AddRef();
			*pRetVal = Value;
		}
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)CComErrorWrapper::ErrorDescription(e), IID_IMmShStrategyGroupColl, e.Error());
	}

	return S_OK;
}

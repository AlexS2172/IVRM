// MmShTraderGroupColl.cpp : Implementation of CMmShTraderGroupColl

#include "stdafx.h"
#include "MmShTraderGroupColl.h"


// CMmShTraderGroupColl
/////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CMmShTraderGroupColl::Add(LONG Key, BSTR SortKey, IMmShTraderGroupAtom* Value, IMmShTraderGroupAtom** pRetVal)
{
	try
	{
		if(m_collRef.find(Key) != m_collRef.end())
		{
			CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Trader group with the same key is already exists."));
		}

		if(!Value)
		{
			CComObject<CMmShTraderGroupAtom>* pNewVal;

			_CHK(CComObject<CMmShTraderGroupAtom>::CreateInstance(&pNewVal), _T("Fail to add trader group."));
			pNewVal->AddRef();
			if(FAILED(IMmShTraderGroupCollImpl::Add(Key, CComBSTR(SortKey), pNewVal)))
			{
				pNewVal->Release();
				CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Fail to add trader group."));
			}
			*pRetVal = pNewVal;
		}
		else
		{
			_CHK(IMmShTraderGroupCollImpl::Add(Key, CComBSTR(SortKey), Value), _T("Fail to add trader group."));
			Value->AddRef();
			*pRetVal = Value;
		}
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)CComErrorWrapper::ErrorDescription(e), IID_IMmShTraderGroupColl, e.Error());
	}

	return S_OK;
}

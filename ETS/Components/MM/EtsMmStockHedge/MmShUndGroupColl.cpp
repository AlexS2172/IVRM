// MmShUndGroupColl.cpp : Implementation of CMmShUndGroupColl

#include "stdafx.h"
#include "MmShUndGroupColl.h"


// CMmShUndGroupColl
/////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CMmShUndGroupColl::Add(LONG Key, BSTR SortKey, IMmShUndGroupAtom* Value, IMmShUndGroupAtom** pRetVal)
{
	try
	{
		if(m_collRef.find(Key) != m_collRef.end())
		{
			CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Underlying group with the same key is already exists."));
		}

		if(!Value)
		{
			CComObject<CMmShUndGroupAtom>* pNewVal;

			_CHK(CComObject<CMmShUndGroupAtom>::CreateInstance(&pNewVal), _T("Fail to add underlying group."));
			pNewVal->AddRef();
			if(FAILED(IMmShUndGroupCollImpl::Add(Key, CComBSTR(SortKey), pNewVal)))
			{
				pNewVal->Release();
				CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Fail to add underlying group."));
			}
			*pRetVal = pNewVal;
		}
		else
		{
			_CHK(IMmShUndGroupCollImpl::Add(Key, CComBSTR(SortKey), Value), _T("Fail to add underlying group."));
			Value->AddRef();
			*pRetVal = Value;
		}
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)CComErrorWrapper::ErrorDescription(e), IID_IMmShUndGroupColl, e.Error());
	}

	return S_OK;
}

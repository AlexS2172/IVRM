// MmIhUndColl.cpp : Implementation of CMmIhUndColl

#include "stdafx.h"
#include "MmIhUndColl.h"


// CMmIhUndColl
/////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CMmIhUndColl::Add(LONG Key, BSTR SortKey, IMmIhUndAtom* Value, IMmIhUndAtom** pRetVal)
{
	try
	{
		if(m_collRef.find(Key) != m_collRef.end())
		{
			CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Underlying with the same key is already exists."));
		}

		if(!Value)
		{
			CComObject<CMmIhUndAtom>* pNewVal;

			_CHK(CComObject<CMmIhUndAtom>::CreateInstance(&pNewVal), _T("Fail to add underlying."));
			pNewVal->AddRef();
			if(FAILED(IMmIhUndCollImpl::Add(Key, CComBSTR(SortKey), pNewVal)))
			{
				pNewVal->Release();
				CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Fail to add underlying."));
			}
			*pRetVal = pNewVal;
		}
		else
		{
			_CHK(IMmIhUndCollImpl::Add(Key, CComBSTR(SortKey), Value), _T("Fail to add underlying."));
			Value->AddRef();
			*pRetVal = Value;
		}
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)CComErrorWrapper::ErrorDescription(e), IID_IMmIhUndColl, e.Error());
	}

	return S_OK;
}


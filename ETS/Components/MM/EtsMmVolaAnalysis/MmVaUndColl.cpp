// MmVaUndColl.cpp : Implementation of CMmVaUndColl

#include "stdafx.h"
#include "MmVaUndColl.h"


/////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CMmVaUndColl::Add(LONG Key, BSTR SortKey, IMmVaUndAtom* Value, IMmVaUndAtom** pRetVal)
{
	try
	{
		if(!Value)
		{
			CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Cann't insert empty underlying."));
		}

		if(m_collRef.find(Key) != m_collRef.end())
		{
			CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Underlying with the same key is already exists."));
		}

		if(!Value)
		{
			CComObject<CMmVaUndAtom>* pNewVal;
			__CHECK_HRESULT(CComObject<CMmVaUndAtom>::CreateInstance(&pNewVal), _T("Fail to add underlying."));
			pNewVal->AddRef();
			if(FAILED(IMmVaUndCollImpl::Add(Key, CComBSTR(SortKey), pNewVal)))
			{
				pNewVal->Release();
				CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Fail to add underlying."));
			}
			*pRetVal = pNewVal;
		}
		else
		{
			__CHECK_HRESULT(IMmVaUndCollImpl::Add(Key, CComBSTR(SortKey), Value), _T("Fail to add underlying."));
			Value->AddRef();
			*pRetVal = Value;
		}
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)CComErrorWrapper::ErrorDescription(e), IID_IMmVaUndColl, e.Error());
	}

	return S_OK;
}


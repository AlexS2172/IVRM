// MmVaOptRootColl.cpp : Implementation of CMmVaOptRootColl

#include "stdafx.h"
#include "MmVaOptRootColl.h"


/////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CMmVaOptRootColl::Add(LONG Key, BSTR SortKey, IMmVaOptRootAtom* Value, IMmVaOptRootAtom** pRetVal)
{
	try
	{
		if(!Value)
		{
			CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Cann't insert empty option root."));
		}

		if(m_collRef.find(Key) != m_collRef.end())
		{
			CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Option root with the same key is already exists."));
		}

		if(!Value)
		{
			CComObject<CMmVaOptRootAtom>* pNewVal;
			__CHECK_HRESULT(CComObject<CMmVaOptRootAtom>::CreateInstance(&pNewVal), _T("Fail to add option root."));
			pNewVal->AddRef();
			if(FAILED(IMmVaOptRootCollImpl::Add(Key, CComBSTR(SortKey), pNewVal)))
			{
				pNewVal->Release();
				CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Fail to add option root."));
			}
			*pRetVal = pNewVal;
		}
		else
		{
			__CHECK_HRESULT(IMmVaOptRootCollImpl::Add(Key, CComBSTR(SortKey), Value), _T("Fail to add option root."));
			Value->AddRef();
			*pRetVal = Value;
		}
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)CComErrorWrapper::ErrorDescription(e), IID_IMmVaOptRootColl, e.Error());
	}

	return S_OK;
}


// MmVaOptPairColl.cpp : Implementation of CMmVaOptPairColl

#include "stdafx.h"
#include "MmVaOptPairColl.h"


/////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CMmVaOptPairColl::Add(LONG Key, BSTR SortKey, IMmVaOptPairAtom* Value, IMmVaOptPairAtom** pRetVal)
{
	try
	{
		if(!Value)
		{
			CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Cann't insert empty option pair."));
		}

		if(m_collRef.find(Key) != m_collRef.end())
		{
			CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Option pair with the same key is already exists."));
		}

		if(!Value)
		{
			CComObject<CMmVaOptPairAtom>* pNewVal;
			__CHECK_HRESULT(CComObject<CMmVaOptPairAtom>::CreateInstance(&pNewVal), _T("Fail to add option pair."));
			pNewVal->AddRef();
			if(FAILED(IMmVaOptPairCollImpl::Add(Key, CComBSTR(SortKey), pNewVal)))
			{
				pNewVal->Release();
				CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Fail to add option pair."));
			}
			*pRetVal = pNewVal;
		}
		else
		{
			__CHECK_HRESULT(IMmVaOptPairCollImpl::Add(Key, CComBSTR(SortKey), Value), _T("Fail to add option pair."));
			Value->AddRef();
			*pRetVal = Value;
		}
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)CComErrorWrapper::ErrorDescription(e), IID_IMmVaOptPairColl, e.Error());
	}

	return S_OK;
}


// MmIhOptColl.cpp : Implementation of CMmIhOptColl

#include "stdafx.h"
#include "MmIhOptColl.h"


// CMmIhOptColl
STDMETHODIMP CMmIhOptColl::Add(LONG Key, IMmIhOptAtom* Value, IMmIhOptAtom** pRetVal)
{
	__CHECK_POINTER(pRetVal);

	try
	{
		if(m_coll.find(Key) != m_coll.end())
		{
			CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Option with the same key is already exists."));
		}

		CComObject<CMmIhOptAtom>* pExp;

		if(!Value)
		{
			_CHK(CComObject<CMmIhOptAtom>::CreateInstance(&pExp), _T("Fail to add option."));
			pExp->AddRef();
			if(FAILED(IMmIhOptCollImpl::Add(Key, pExp)))
			{
				pExp->Release();
				CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Fail to add option."));
			}
			*pRetVal = pExp;
		}
		else
		{
			_CHK(IMmIhOptCollImpl::Add(Key, Value), _T("Fail to add option."));
			Value->AddRef();
			*pRetVal = Value;
		}
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)CComErrorWrapper::ErrorDescription(e), IID_IMmIhOptColl, e.Error());
	}

	return S_OK;
}

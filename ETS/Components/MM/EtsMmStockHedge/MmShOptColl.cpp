// MmShOptColl.cpp : Implementation of CMmShOptColl

#include "stdafx.h"
#include "MmShOptColl.h"


// CMmShOptColl
STDMETHODIMP CMmShOptColl::Add(LONG Key, IMmShOptAtom* Value, IMmShOptAtom** pRetVal)
{
	__CHECK_POINTER(pRetVal);

	try
	{
		if(m_coll.find(Key) != m_coll.end())
		{
			CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Option with the same key is already exists."));
		}

		CComObject<CMmShOptAtom>* pExp;

		if(!Value)
		{
			_CHK(CComObject<CMmShOptAtom>::CreateInstance(&pExp), _T("Fail to add option."));
			pExp->AddRef();
			if(FAILED(IMmShOptCollImpl::Add(Key, pExp)))
			{
				pExp->Release();
				CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Fail to add option."));
			}
			*pRetVal = pExp;
		}
		else
		{
			_CHK(IMmShOptCollImpl::Add(Key, Value), _T("Fail to add option."));
			Value->AddRef();
			*pRetVal = Value;
		}
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)CComErrorWrapper::ErrorDescription(e), IID_IMmShOptColl, e.Error());
	}

	return S_OK;
}

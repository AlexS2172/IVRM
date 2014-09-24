// MmShUndColl.cpp : Implementation of CMmShUndColl

#include "stdafx.h"
#include "MmShUndColl.h"


// CMmShUndColl
STDMETHODIMP CMmShUndColl::Add(LONG Key, IMmShUndAtom* Value, IMmShUndAtom** pRetVal)
{
	__CHECK_POINTER(pRetVal);

	try
	{
		if(m_coll.find(Key) != m_coll.end())
		{
			CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Underlying with the same key is already exists."));
		}

		CComObject<CMmShUndAtom>* pExp;

		if(!Value)
		{
			_CHK(CComObject<CMmShUndAtom>::CreateInstance(&pExp), _T("Fail to add underlying."));
			pExp->AddRef();
			if(FAILED(IMmShUndCollImpl::Add(Key, pExp)))
			{
				pExp->Release();
				CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Fail to add underlying."));
			}
			*pRetVal = pExp;
		}
		else
		{
			_CHK(IMmShUndCollImpl::Add(Key, Value), _T("Fail to add underlying."));
			Value->AddRef();
			*pRetVal = Value;
		}
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)CComErrorWrapper::ErrorDescription(e), IID_IMmShUndColl, e.Error());
	}

	return S_OK;
}

// MmShPosColl.cpp : Implementation of CMmShPosColl

#include "stdafx.h"
#include "MmShPosColl.h"


// CMmShPosColl
STDMETHODIMP CMmShPosColl::Add(LONG Key, IMmShPosAtom* Value, IMmShPosAtom** pRetVal)
{
	__CHECK_POINTER(pRetVal);

	try
	{
		if(m_coll.find(Key) != m_coll.end())
		{
			CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Value with the same key is already exists."));
		}

		CComObject<CMmShPosAtom>* pExp;

		if(!Value)
		{
			_CHK(CComObject<CMmShPosAtom>::CreateInstance(&pExp), _T("Fail to add value."));
			pExp->AddRef();
			if(FAILED(IMmShPosCollImpl::Add(Key, pExp)))
			{
				pExp->Release();
				CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Fail to add value."));
			}
			*pRetVal = pExp;
		}
		else
		{
			_CHK(IMmShPosCollImpl::Add(Key, Value), _T("Fail to add value."));
			Value->AddRef();
			*pRetVal = Value;
		}
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)CComErrorWrapper::ErrorDescription(e), IID_IMmShPosColl, e.Error());
	}

	return S_OK;
}

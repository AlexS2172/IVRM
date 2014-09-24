// MmRpOptRootColl.cpp : Implementation of CMmRpOptRootColl

#include "stdafx.h"
#include "MmRpOptRootColl.h"
#include "MmRpOptRootAtom.h"


// CMmRpOptRootColl

STDMETHODIMP CMmRpOptRootColl::Add(LONG Key, IMmRpOptRootAtom* Value, IMmRpOptRootAtom** pRetVal)
{
	__CHECK_POINTER(pRetVal);

	try
	{
		if(m_coll.find(Key) != m_coll.end())
		{
			CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Value with the same key already exists."));
		}

		CComObject<CMmRpOptRootAtom>* pOpt;

		if(!Value)
		{
			__CHECK_HRESULT(CComObject<CMmRpOptRootAtom>::CreateInstance(&pOpt), _T("Failed to add value."));
			pOpt->AddRef();
			if(FAILED(IMmRpOptRootCollImpl::Add(Key, pOpt)))
			{
				pOpt->Release();
				CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Failed to add value."));
			}
			*pRetVal = pOpt;
		}
		else
		{
			__CHECK_HRESULT(IMmRpOptRootCollImpl::Add(Key, Value), _T("Failed to add value."));
			Value->AddRef();
			*pRetVal = Value;
		}
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)CComErrorWrapper::ErrorDescription(e), IID_IMmRpOptRootColl, e.Error());
	}

	return S_OK;
}

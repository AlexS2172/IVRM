// MmRpPnLColl.cpp : Implementation of CMmRpPnLColl

#include "stdafx.h"
#include "MmRpPnLColl.h"
#include "MmRpPnLAtom.h"


// CMmRpPnLColl

STDMETHODIMP CMmRpPnLColl::Add(LONG Key, IMmRpPnLAtom* Value, IMmRpPnLAtom** pRetVal)
{
	__CHECK_POINTER(pRetVal);

	try
	{
		if(m_coll.find(Key) != m_coll.end())
		{
			CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Value with the same key already exists."));
		}

		CComObject<CMmRpPnLAtom>* pPnL;

		if(!Value)
		{
			__CHECK_HRESULT(CComObject<CMmRpPnLAtom>::CreateInstance(&pPnL), _T("Failed to add value."));
			pPnL->AddRef();
			if(FAILED(IMmRpPnLCollImpl::Add(Key, pPnL)))
			{
				pPnL->Release();
				CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Failed to add value."));
			}
			*pRetVal = pPnL;
		}
		else
		{
			__CHECK_HRESULT(IMmRpPnLCollImpl::Add(Key, Value), _T("Failed to add value."));
			Value->AddRef();
			*pRetVal = Value;
		}
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)CComErrorWrapper::ErrorDescription(e), IID_IMmRpPnLColl, e.Error());
	}

	return S_OK;
}

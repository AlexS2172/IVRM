// MmRpPosColl.cpp : Implementation of CMmRpPosColl

#include "stdafx.h"
#include "MmRpPosColl.h"
#include "MmRpPosAtom.h"


// CMmRpPosColl

STDMETHODIMP CMmRpPosColl::Add(LONG Key, IMmRpPosAtom* Value, IMmRpPosAtom** pRetVal)
{
	__CHECK_POINTER(pRetVal);

	try
	{
		if(m_coll.find(Key) != m_coll.end())
		{
			CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Value with the same key already exists."));
		}

		CComObject<CMmRpPosAtom>* pPos;

		if(!Value)
		{
			__CHECK_HRESULT(CComObject<CMmRpPosAtom>::CreateInstance(&pPos), _T("Failed to add value."));
			pPos->AddRef();
			if(FAILED(IMmRpPosCollImpl::Add(Key, pPos)))
			{
				pPos->Release();
				CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Failed to add value."));
			}
			*pRetVal = pPos;
		}
		else
		{
			__CHECK_HRESULT(IMmRpPosCollImpl::Add(Key, Value), _T("Failed to add value."));
			Value->AddRef();
			*pRetVal = Value;
		}
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)CComErrorWrapper::ErrorDescription(e), IID_IMmRpPosColl, e.Error());
	}

	return S_OK;
}


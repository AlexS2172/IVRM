// MmRpPosWithEarlyExerciseColl.cpp : Implementation of CMmRpPosWithEarlyExerciseColl

#include "stdafx.h"
#include "MmRpPosWithEarlyExerciseColl.h"
#include "MmRpPosWithEarlyExerciseAtom.h"


// CMmRpPosWithEarlyExerciseColl

STDMETHODIMP CMmRpPosWithEarlyExerciseColl::Add(LONG Key, BSTR SortKey, IMmRpPosWithEarlyExerciseAtom* Value, IMmRpPosWithEarlyExerciseAtom** pRetVal)
{
	__CHECK_POINTER(pRetVal);

	try
	{
		if(m_collRef.find(Key) != m_collRef.end())
		{
			CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Value with the same key already exists."));
		}

		CComObject<CMmRpPosWithEarlyExerciseAtom>* pOpt;

		if(!Value)
		{
			__CHECK_HRESULT(CComObject<CMmRpPosWithEarlyExerciseAtom>::CreateInstance(&pOpt), _T("Failed to add value."));
			pOpt->AddRef();
			if(FAILED(IMmRpPosWithEarlyExCollImpl::Add(Key, CComBSTR(SortKey), pOpt)))
			{
				pOpt->Release();
				CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Failed to add value."));
			}
			*pRetVal = pOpt;
		}
		else
		{
			__CHECK_HRESULT(IMmRpPosWithEarlyExCollImpl::Add(Key, CComBSTR(SortKey), Value), _T("Failed to add value."));
			Value->AddRef();
			*pRetVal = Value;
		}
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)CComErrorWrapper::ErrorDescription(e), IID_IMmRpPosWithEarlyExerciseColl, e.Error());
	}

	return S_OK;
}
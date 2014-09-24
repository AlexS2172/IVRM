// MmRpPosWithEarlyExerciseOptColl.cpp : Implementation of CMmRpPosWithEarlyExerciseOptColl

#include "stdafx.h"
#include "MmRpPosWithEarlyExerciseOptColl.h"
#include "MmRpPosWithEarlyExerciseOptAtom.h"


// CMmRpPosWithEarlyExerciseOptColl

STDMETHODIMP CMmRpPosWithEarlyExerciseOptColl::Add(LONG nRootID, DOUBLE dStrike, DATE dtExpiry, IMmRpPosWithEarlyExerciseOptAtom* Value, IMmRpPosWithEarlyExerciseOptAtom** pRetVal)
{
	__CHECK_POINTER(pRetVal);

	_bstr_t			bsKey;
	COleDateTime	dt((DATE)(LONG)dtExpiry);

	dt.SetDate(dt.GetYear(), dt.GetMonth(), 1);

	try
	{
		bsKey = (_bstr_t)_variant_t(nRootID) + L"_" + (_bstr_t)_variant_t((LONG)dStrike) + L"_" + 
			(_bstr_t)_variant_t((LONG)(DATE)dt);

		if(m_coll.find(bsKey) != m_coll.end())
		{
			CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Value with the same key already exists."));
		}

		CComObject<CMmRpPosWithEarlyExerciseOptAtom>* pOpt;

		if(!Value)
		{
			__CHECK_HRESULT(CComObject<CMmRpPosWithEarlyExerciseOptAtom>::CreateInstance(&pOpt), _T("Failed to add value."));
			pOpt->AddRef();
			if(FAILED(IMmRpPosWithEarlyExOptCollImpl::Add(bsKey, pOpt)))
			{
				pOpt->Release();
				CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Failed to add value."));
			}
			*pRetVal = pOpt;
		}
		else
		{
			__CHECK_HRESULT(IMmRpPosWithEarlyExOptCollImpl::Add(bsKey, Value), _T("Failed to add value."));
			Value->AddRef();
			*pRetVal = Value;
		}
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)CComErrorWrapper::ErrorDescription(e), IID_IMmRpPosWithEarlyExerciseOptColl, e.Error());
	}

	return S_OK;
}

STDMETHODIMP CMmRpPosWithEarlyExerciseOptColl::get_Item(LONG nRootID, DOUBLE dStrike, DATE dtExpiry, IMmRpPosWithEarlyExerciseOptAtom** pValue)
{
	_bstr_t			bsKey;
	COleDateTime	dt((DATE)(LONG)dtExpiry);
	
	dt.SetDate(dt.GetYear(), dt.GetMonth(), 1);

	try
	{
		bsKey = (_bstr_t)_variant_t(nRootID) + L"_" + (_bstr_t)_variant_t((LONG)dStrike) + L"_" + 
			(_bstr_t)_variant_t((LONG)(DATE)dt);
	}
	catch(_com_error& e)
	{
		return Error((PTCHAR)CComErrorWrapper::ErrorDescription(e), IID_IMmRpPosWithEarlyExerciseOptColl, e.Error());
	}

	return IMmRpPosWithEarlyExOptCollImpl::get_Item(bsKey, pValue);
}

STDMETHODIMP CMmRpPosWithEarlyExerciseOptColl::Remove(LONG nRootID, DOUBLE dStrike, DATE dtExpiry)
{
	_bstr_t			bsKey;
	COleDateTime	dt((DATE)(LONG)dtExpiry);
	
	dt.SetDate(dt.GetYear(), dt.GetMonth(), 1);

	try
	{
		bsKey = (_bstr_t)_variant_t(nRootID) + L"_" + (_bstr_t)_variant_t((LONG)dStrike) + L"_" + 
			(_bstr_t)_variant_t((LONG)(DATE)dt);
	}
	catch(_com_error& e)
	{
		return Error((PTCHAR)CComErrorWrapper::ErrorDescription(e), IID_IMmRpPosWithEarlyExerciseOptColl, e.Error());
	}

	return IMmRpPosWithEarlyExOptCollImpl::Remove(bsKey);
}
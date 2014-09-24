// MmRpExercisedStockColl.cpp : Implementation of CMmRpExercisedStockColl

#include "stdafx.h"
#include "MmRpExercisedStockColl.h"
#include "MmRpExercisedStockAtom.h"


// CMmRpExercisedStockColl

STDMETHODIMP CMmRpExercisedStockColl::Add(BSTR Key, IMmRpExercisedStockAtom* Value, IMmRpExercisedStockAtom** pRetVal)
{
	__CHECK_POINTER(pRetVal);

	try
	{
		if(m_coll.find(Key) != m_coll.end())
		{
			CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Value with the same key already exists."));
		}

		CComObject<CMmRpExercisedStockAtom>* pStk;

		if(!Value)
		{
			__CHECK_HRESULT(CComObject<CMmRpExercisedStockAtom>::CreateInstance(&pStk), _T("Failed to add value."));
			pStk->AddRef();
			if(FAILED(IMmRpExercisedStockCollImpl::Add(Key, pStk)))
			{
				pStk->Release();
				CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Failed to add value."));
			}
			*pRetVal = pStk;
		}
		else
		{
			__CHECK_HRESULT(IMmRpExercisedStockCollImpl::Add(Key, Value), _T("Failed to add value."));
			Value->AddRef();
			*pRetVal = Value;
		}
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)CComErrorWrapper::ErrorDescription(e), IID_IMmRpMatrixByStockColl, e.Error());
	}

	return S_OK;
}

STDMETHODIMP CMmRpExercisedStockColl::get_Item(BSTR Key, IMmRpExercisedStockAtom** pValue)
{
	return IMmRpExercisedStockCollImpl::get_Item(Key, pValue);
}

STDMETHODIMP CMmRpExercisedStockColl::Remove(BSTR Key)
{
    return IMmRpExercisedStockCollImpl::Remove(Key);
}

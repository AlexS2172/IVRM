// MmRpMatrixByStockOptColl.cpp : Implementation of CMmRpMatrixByStockOptColl

#include "stdafx.h"
#include "MmRpMatrixByStockOptColl.h"
#include "MmRpMatrixByStockOptAtom.h"


// CMmRpMatrixByStockOptColl

STDMETHODIMP CMmRpMatrixByStockOptColl::Add(LONG Key, IMmRpMatrixByStockOptAtom* Value, IMmRpMatrixByStockOptAtom** pRetVal)
{
	__CHECK_POINTER(pRetVal);

	try
	{
		if(m_coll.find(Key) != m_coll.end())
		{
			CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Value with the same key already exists."));
		}

		CComObject<CMmRpMatrixByStockOptAtom>* pOpt;

		if(!Value)
		{
			__CHECK_HRESULT(CComObject<CMmRpMatrixByStockOptAtom>::CreateInstance(&pOpt), _T("Failed to add value."));
			pOpt->AddRef();
			if(FAILED(IMmRpMatrixByStockOptCollImpl::Add(Key, pOpt)))
			{
				pOpt->Release();
				CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Failed to add value."));
			}
			*pRetVal = pOpt;
		}
		else
		{
			__CHECK_HRESULT(IMmRpMatrixByStockOptCollImpl::Add(Key, Value), _T("Failed to add value."));
			Value->AddRef();
			*pRetVal = Value;
		}
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)CComErrorWrapper::ErrorDescription(e), IID_IMmRpMatrixByStockOptColl, e.Error());
	}

	return S_OK;
}

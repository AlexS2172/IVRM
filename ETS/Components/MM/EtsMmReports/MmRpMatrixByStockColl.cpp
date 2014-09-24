// MmRpMatrixByStockColl.cpp : Implementation of CMmRpMatrixByStockColl

#include "stdafx.h"
#include "MmRpMatrixByStockColl.h"
#include "MmRpMatrixByStockAtom.h"


// CMmRpMatrixByStockColl

STDMETHODIMP CMmRpMatrixByStockColl::Add(LONG Key, BSTR SortKey, IMmRpMatrixByStockAtom* Value, IMmRpMatrixByStockAtom** pRetVal)
{
	__CHECK_POINTER(pRetVal);

	try
	{
		if(m_collRef.find(Key) != m_collRef.end())
		{
			//CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Value with the same key already exists."));
                        return S_OK;
		}

		CComObject<CMmRpMatrixByStockAtom>* pExp;

		if(!Value)
		{
			__CHECK_HRESULT(CComObject<CMmRpMatrixByStockAtom>::CreateInstance(&pExp), _T("Failed to add value."));
			pExp->AddRef();
			if(FAILED(IMmRpMatrixByStockCollImpl::Add(Key, CComBSTR(SortKey), pExp)))
			{
				pExp->Release();
				CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Failed to add value."));
			}
			*pRetVal = pExp;
		}
		else
		{
			__CHECK_HRESULT(IMmRpMatrixByStockCollImpl::Add(Key, CComBSTR(SortKey), Value), _T("Failed to add value."));
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

// MmRpRiskMatrixColl.cpp : Implementation of CMmRpRiskMatrixColl

#include "stdafx.h"
#include "MmRpRiskMatrixColl.h"
#include "MmRpRiskMatrixAtom.h"


// CMmRpRiskMatrixColl

STDMETHODIMP CMmRpRiskMatrixColl::Add(LONG Key, BSTR SortKey, IMmRpRiskMatrixAtom* Value, IMmRpRiskMatrixAtom** pRetVal)
{
	__CHECK_POINTER(pRetVal);

	try
	{
		if(m_collRef.find(Key) != m_collRef.end())
		{
			CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Value with the same key already exists."));
		}

		CComObject<CMmRpRiskMatrixAtom>* pMatrix;

		if(!Value)
		{
			__CHECK_HRESULT(CComObject<CMmRpRiskMatrixAtom>::CreateInstance(&pMatrix), _T("Failed to add value."));
			pMatrix->AddRef();
			if(FAILED(IMmRpRiskMatrixCollImpl::Add(Key, CComBSTR(SortKey), pMatrix)))
			{
				pMatrix->Release();
				CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Failed to add value."));
			}
			*pRetVal = pMatrix;
		}
		else
		{
			__CHECK_HRESULT(IMmRpRiskMatrixCollImpl::Add(Key, CComBSTR(SortKey), Value), _T("Failed to add value."));
			Value->AddRef();
			*pRetVal = Value;
		}
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)CComErrorWrapper::ErrorDescription(e), IID_IMmRpRiskMatrixColl, e.Error());
	}

	return S_OK;
}
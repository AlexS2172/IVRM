// MmRpRiskMatrixShiftColl.cpp : Implementation of CMmRpRiskMatrixShiftColl

#include "stdafx.h"
#include "MmRpRiskMatrixShiftColl.h"
#include "MmRpRiskMatrixShiftAtom.h"


// CMmRpRiskMatrixShiftColl

STDMETHODIMP CMmRpRiskMatrixShiftColl::Add(BSTR Key, IMmRpRiskMatrixShiftAtom* Value, IMmRpRiskMatrixShiftAtom** pRetVal)
{
	__CHECK_POINTER(pRetVal);

	try
	{
		if(m_coll.find(Key) != m_coll.end())
		{
			CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Value with the same key already exists."));
		}

		CComObject<CMmRpRiskMatrixShiftAtom>* pShift;

		if(!Value)
		{
			__CHECK_HRESULT(CComObject<CMmRpRiskMatrixShiftAtom>::CreateInstance(&pShift), _T("Failed to add value."));
			pShift->AddRef();
			if(FAILED(IMmRpRiskMatrixShiftCollImpl::Add(Key, pShift)))
			{
				pShift->Release();
				CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Failed to add value."));
			}
			*pRetVal = pShift;
		}
		else
		{
			__CHECK_HRESULT(IMmRpRiskMatrixShiftCollImpl::Add(Key, Value), _T("Failed to add value."));
			Value->AddRef();
			*pRetVal = Value;
		}
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)CComErrorWrapper::ErrorDescription(e), IID_IMmRpRiskMatrixShiftColl, e.Error());
	}

	return S_OK;
}

STDMETHODIMP CMmRpRiskMatrixShiftColl::get_Item(BSTR Key, IMmRpRiskMatrixShiftAtom** pValue)
{
	return IMmRpRiskMatrixShiftCollImpl::get_Item(Key, pValue);
}

STDMETHODIMP CMmRpRiskMatrixShiftColl::Remove(BSTR Key)
{
	return IMmRpRiskMatrixShiftCollImpl::Remove(Key);
}

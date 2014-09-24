// MmRpGreeksByMonthColl.cpp : Implementation of CMmRpGreeksByMonthColl

#include "stdafx.h"
#include "MmRpGreeksByMonthColl.h"
#include "MmRpGreeksByMonthAtom.h"


// CMmRpGreeksByMonthColl

STDMETHODIMP CMmRpGreeksByMonthColl::Add(BSTR Key, IMmRpGreeksByMonthAtom* Value, IMmRpGreeksByMonthAtom** pRetVal)
{
	__CHECK_POINTER(pRetVal);

	try
	{
		if(m_coll.find(Key) != m_coll.end())
		{
			CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Value with the same key already exists."));
		}

		CComObject<CMmRpGreeksByMonthAtom>* pGreeks;

		if(!Value)
		{
			__CHECK_HRESULT(CComObject<CMmRpGreeksByMonthAtom>::CreateInstance(&pGreeks), _T("Failed to add value."));
			pGreeks->AddRef();
			if(FAILED(IMmRpGreeksByMonthCollImpl::Add(Key, pGreeks)))
			{
				pGreeks->Release();
				CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Failed to add value."));
			}
			*pRetVal = pGreeks;
		}
		else
		{
			__CHECK_HRESULT(IMmRpGreeksByMonthCollImpl::Add(Key, Value), _T("Failed to add value."));
			Value->AddRef();
			*pRetVal = Value;
		}
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)CComErrorWrapper::ErrorDescription(e), IID_IMmRpGreeksByMonthColl, e.Error());
	}

	return S_OK;
}

STDMETHODIMP CMmRpGreeksByMonthColl::get_Item(BSTR Key, IMmRpGreeksByMonthAtom** pValue)
{
	return IMmRpGreeksByMonthCollImpl::get_Item(Key, pValue);
}

STDMETHODIMP CMmRpGreeksByMonthColl::Remove(BSTR Key)
{
	return IMmRpGreeksByMonthCollImpl::Remove(Key);
}
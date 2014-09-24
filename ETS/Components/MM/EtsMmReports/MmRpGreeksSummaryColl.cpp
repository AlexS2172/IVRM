// MmRpGreeksSummaryColl.cpp : Implementation of CMmRpGreeksSummaryColl

#include "stdafx.h"
#include "MmRpGreeksSummaryColl.h"
#include "MmRpGreeksSummaryAtom.h"


// CMmRpGreeksSummaryColl

STDMETHODIMP CMmRpGreeksSummaryColl::Add(LONG Key, IMmRpGreeksSummaryAtom* Value, IMmRpGreeksSummaryAtom** pRetVal)
{
	__CHECK_POINTER(pRetVal);

	try
	{
		if(m_coll.find(Key) != m_coll.end())
		{
			CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Value with the same key already exists."));
		}

		CComObject<CMmRpGreeksSummaryAtom>* pGreeks;

		if(!Value)
		{
			__CHECK_HRESULT(CComObject<CMmRpGreeksSummaryAtom>::CreateInstance(&pGreeks), _T("Failed to add value."));
			pGreeks->AddRef();
			if(FAILED(IMmRpGreeksSummaryCollImpl::Add(Key, pGreeks)))
			{
				pGreeks->Release();
				CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Failed to add value."));
			}
			*pRetVal = pGreeks;
		}
		else
		{
			__CHECK_HRESULT(IMmRpGreeksSummaryCollImpl::Add(Key, Value), _T("Failed to add value."));
			Value->AddRef();
			*pRetVal = Value;
		}
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)CComErrorWrapper::ErrorDescription(e), IID_IMmRpGreeksSummaryColl, e.Error());
	}

	return S_OK;
}
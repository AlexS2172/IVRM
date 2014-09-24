// MmRpSyntheticColl.cpp : Implementation of CMmRpSyntheticColl

#include "stdafx.h"
#include "MmRpSyntheticColl.h"
#include "MmRpSyntheticAtom.h"


// CMmRpSyntheticColl

STDMETHODIMP CMmRpSyntheticColl::Add(LONG Key, LONG SubKey, BSTR SortKey, IMmRpSyntheticAtom* Value, IMmRpSyntheticAtom** pRetVal)
{
	__CHECK_POINTER(pRetVal);

	LONGLONG nKey = MAKELONGLONG(Key, SubKey);

	try
	{
		if(m_collRef.find(nKey) != m_collRef.end())
		{
			//CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Value with the same key already exists."));
			return S_OK;
		}

		CComObject<CMmRpSyntheticAtom>* pOpt;

		if(!Value)
		{
			__CHECK_HRESULT(CComObject<CMmRpSyntheticAtom>::CreateInstance(&pOpt), _T("Failed to add value."));
			pOpt->AddRef();
			if(FAILED(IMmRpSyntheticCollImpl::Add(nKey, CComBSTR(SortKey), pOpt)))
			{
				pOpt->Release();
				CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Failed to add value."));
			}
			*pRetVal = pOpt;
		}
		else
		{
			__CHECK_HRESULT(IMmRpSyntheticCollImpl::Add(nKey, CComBSTR(SortKey), Value), _T("Failed to add value."));
			Value->AddRef();
			*pRetVal = Value;
		}
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)CComErrorWrapper::ErrorDescription(e), IID_IMmRpSyntheticColl, e.Error());
	}

	return S_OK;
}

STDMETHODIMP CMmRpSyntheticColl::get_Item(LONG Key,LONG SubKey, IMmRpSyntheticAtom** ppVal)
{
	LONGLONG nKey = MAKELONGLONG(Key, SubKey);
	return IMmRpSyntheticCollImpl::get_Item(nKey, ppVal);
}

STDMETHODIMP CMmRpSyntheticColl::Remove(LONG Key,LONG SubKey)
{
	LONGLONG nKey = MAKELONGLONG(Key, SubKey);
	return IMmRpSyntheticCollImpl::Remove(nKey);
}

// MmTntCounterPartyColl.cpp : Implementation of CMmTntCounterPartyColl
#include "stdafx.h"
#include "MmTntCounterPartyColl.h"


STDMETHODIMP CMmTntCounterPartyColl::Add(LONG Key, BSTR SortKey, IMmTntCounterPartyAtom* Value, IMmTntCounterPartyAtom** pRetVal)
{
	__CHECK_POINTER(pRetVal);

	try
	{
		if(m_collRef.find(Key) != m_collRef.end())
		{
			EgLib::CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Counterparty with the same key already exists."));
		}

		if(!Value)
		{
			CComObject<CMmTntCounterPartyAtom>* pNewVal;
			__CHECK_HRESULT(CComObject<CMmTntCounterPartyAtom>::CreateInstance(&pNewVal), _T("Failed to add counterparty."));
			pNewVal->AddRef();
			if(FAILED(IMmTntCounterPartyCollImpl::Add(Key, CComBSTR(SortKey), pNewVal)))
			{
				pNewVal->Release();
				EgLib::CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Failed to add counterparty."));
			}
			*pRetVal = pNewVal;
		}
		else
		{
			__CHECK_HRESULT(IMmTntCounterPartyCollImpl::Add(Key, CComBSTR(SortKey), Value), _T("Failed to add counterparty."));
			Value->AddRef();
			*pRetVal = Value;
		}
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_IMmTntCounterPartyColl, e.Error());
	}

	return S_OK;
}
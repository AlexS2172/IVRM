// MmTntCounterPartyByNameColl.cpp : Implementation of CMmTntCounterPartyByNameColl

#include "stdafx.h"
#include "MmTntCounterPartyByNameColl.h"


// CMmTntCounterPartyByNameColl

STDMETHODIMP CMmTntCounterPartyByNameColl::Add(BSTR Key, IMmTntCounterPartyAtom* Value, IMmTntCounterPartyAtom** pRetVal)
{
	__CHECK_POINTER(pRetVal);

	try
	{
		if(m_coll.find(CComBSTR(Key)) != m_coll.end())
		{
			EgLib::CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Counterparty with the same key already exists"));
		}

		CComObject<CMmTntCounterPartyAtom>* pExp;

		if(!Value)
		{
			__CHECK_HRESULT(CComObject<CMmTntCounterPartyAtom>::CreateInstance(&pExp), _T("Failed to add counterparty"));
			pExp->AddRef();
			if(FAILED(IMmTntCounterPartyByNameCollImpl::Add(Key, pExp)))
			{
				pExp->Release();
				EgLib::CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Failed to add counterparty"));
			}
			*pRetVal = pExp;
		}
		else
		{
			__CHECK_HRESULT(IMmTntCounterPartyByNameCollImpl::Add(Key, Value), _T("Failed to add counterparty"));
			Value->AddRef();
			*pRetVal = Value;
		}
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_IMmTntCounterPartyByNameColl, e.Error());
	}

	return S_OK;
}

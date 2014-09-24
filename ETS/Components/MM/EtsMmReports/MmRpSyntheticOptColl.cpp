// MmRpSyntheticOptColl.cpp : Implementation of CMmRpSyntheticOptColl

#include "stdafx.h"
#include "MmRpSyntheticOptColl.h"
#include "MmRpSyntheticOptAtom.h"


// CMmRpSyntheticOptColl

STDMETHODIMP CMmRpSyntheticOptColl::Add(LONG Key, IMmRpSyntheticOptAtom* Value, IMmRpSyntheticOptAtom** pRetVal)
{
	__CHECK_POINTER(pRetVal);
	
	try
	{
		if(m_coll.find(Key) != m_coll.end())
		{
			CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Value with the same key already exists."));
		}

		CComObject<CMmRpSyntheticOptAtom>* pOpt;

		if(!Value)
		{
			__CHECK_HRESULT(CComObject<CMmRpSyntheticOptAtom>::CreateInstance(&pOpt), _T("Failed to add value."));
			pOpt->AddRef();
			if(FAILED(IMmRpSyntheticOptCollImpl::Add(Key, pOpt)))
			{
				pOpt->Release();
				CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Failed to add value."));
			}
			*pRetVal = pOpt;
		}
		else
		{
			__CHECK_HRESULT(IMmRpSyntheticOptCollImpl::Add(Key, Value), _T("Failed to add value."));
			Value->AddRef();
			*pRetVal = Value;
		}
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)CComErrorWrapper::ErrorDescription(e), IID_IMmRpSyntheticOptColl, e.Error());
	}

	return S_OK;
}

STDMETHODIMP CMmRpSyntheticOptColl::Append(IMmRpSyntheticOptColl* pValue, LONG RootID)
{
	__CHECK_POINTER(pValue);
	HRESULT			hr = S_OK;

	try
	{
		IMmRpSyntheticOptCollPtr	spColl = pValue;
		IUnknownPtr					spUnk;

		__CHECK_HRESULT(spColl->get__NewEnum(&spUnk), _T("Failed to get options collection."));

		IEnumVARIANTPtr spOptCollEnum(spUnk);
		_variant_t		vItem;
		ULONG			nFetched = 0L;

		IMmRpSyntheticOptAtomPtr	spOptAtom;
		IMmRpSyntheticOptAtomPtr	spOptCloneAtom;
		IMmRpSyntheticOptAtomPtr	spOptCloneRetAtom;

		__CHECK_HRESULT(spOptCollEnum->Reset(), _T("Failed to reset options collection."));

		while(spOptCollEnum->Next(1L, &vItem, &nFetched) == S_OK)
		{
			ATLASSERT(V_VT(&vItem) == VT_DISPATCH && V_DISPATCH(&vItem) != 0);
			spOptAtom = vItem;

			long nID = 0;
			long nRootID;

			__CHECK_HRESULT3(spOptAtom->get_RootID(&nRootID));

			if (RootID != 0 && RootID != nRootID)
				continue;

			__CHECK_HRESULT3(spOptAtom->get_ID(&nID));
			__CHECK_HRESULT3(get_Item(nID, &spOptCloneAtom))

			if (spOptCloneAtom == 0)
			{
				__CHECK_HRESULT3(spOptAtom->Clone(&spOptCloneAtom));
				__CHECK_HRESULT3(spOptCloneAtom->ClearValues(VARIANT_TRUE));
				__CHECK_HRESULT3(Add(nID, spOptCloneAtom, &spOptCloneRetAtom));
			}

			spOptAtom = 0;
			spOptCloneAtom = 0;
			spOptCloneRetAtom = 0;
		}
	}
	catch(const _com_error& e)
	{
		hr = Error((PTCHAR)CComErrorWrapper::ErrorDescription(e), IID_IMmRpSyntheticOptColl, e.Error());
	}

	return hr;
}

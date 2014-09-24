// MmTePriceColl.cpp : Implementation of CMmTePriceColl

#include "stdafx.h"
#include "MmTePriceColl.h"
#include "TePriceAtom.h"


// CMmTePriceColl
STDMETHODIMP CMmTePriceColl::Add(LONG Key, BSTR SortKey, IMmTePriceAtom* Value, IMmTePriceAtom** pRetVal)
{
	try
	{
		if(m_collRef.find(Key) != m_collRef.end())
		{
			CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Price Atom with the same key is already exists."));
		}

		if(!Value)
		{
			CComObject<CMmTePriceAtom>* pNewVal;

			_CHK(CComObject<CMmTePriceAtom>::CreateInstance(&pNewVal), _T("Fail to add price Atom."));
			
			pNewVal->AddRef();
			if(FAILED(IMmTePriceCollCollImpl::Add(Key, _bstr_t(SortKey), pNewVal)))
			{
				pNewVal->Release();
				CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Fail to add price atom."));
			}
			*pRetVal = pNewVal;
		}
		else
		{
			_CHK(IMmTePriceCollCollImpl::Add(Key, _bstr_t(SortKey), Value), _T("Fail to add underlying group."));
			Value->AddRef();
			*pRetVal = Value;
		}
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)CComErrorWrapper::ErrorDescription(e), IID_IMmTePriceColl, e.Error());
	}

	return S_OK;

}

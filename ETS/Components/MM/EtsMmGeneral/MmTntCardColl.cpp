// MmTntCardColl.cpp : Implementation of CMmTntCardColl

#include "stdafx.h"
#include "MmTntCardColl.h"

/////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CMmTntCardColl::Add(BSTR Key, DATE SortKey, IMmTntCardAtom* Value, IMmTntCardAtom** pRetVal)
{
	__CHECK_POINTER(pRetVal);

	try
	{
		_bstr_t bsKey(Key);
		if(m_collRef.find(bsKey) != m_collRef.end())
		{
			EgLib::CComErrorWrapper::ThrowError(E_INVALIDARG, _T("TNT card with the same key already exists."));
		}

		if(!Value)
		{
			CComObject<CMmTntCardAtom>* pNewVal;
			__CHECK_HRESULT(CComObject<CMmTntCardAtom>::CreateInstance(&pNewVal), _T("Failed to add TNT card."));
			pNewVal->AddRef();
			if(FAILED(IMmTntCardCollImpl::Add(bsKey, SortKey, pNewVal)))
			{
				pNewVal->Release();
				EgLib::CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Failed to add TNT card."));
			}
			*pRetVal = pNewVal;
		}
		else
		{
			__CHECK_HRESULT(IMmTntCardCollImpl::Add(bsKey, SortKey, Value), _T("Failed to add TNT card."));
			Value->AddRef();
			*pRetVal = Value;
		}
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_IMmTntCardColl, e.Error());
	}

	return S_OK;
}
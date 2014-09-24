// MmTntCardBySeqColl.cpp : Implementation of CMmTntCardBySeqColl

#include "stdafx.h"
#include "MmTntCardBySeqColl.h"


STDMETHODIMP CMmTntCardBySeqColl::Add(LONG Key, DATE SortKey, IMmTntCardAtom* Value, IMmTntCardAtom** pRetVal)
{
	__CHECK_POINTER(pRetVal);

	try
	{
		if(m_coll.find(Key) != m_coll.end())
		{
			EgLib::CComErrorWrapper::ThrowError(E_INVALIDARG, _T("TNT card with the same key already exists."));
		}

		if(!Value)
		{
			CComObject<CMmTntCardAtom>* pNewVal;
			__CHECK_HRESULT(CComObject<CMmTntCardAtom>::CreateInstance(&pNewVal), _T("Failed to add TNT card."));
			pNewVal->AddRef();
			if(FAILED(IMmTntCardBySeqCollImpl::Add(Key, SortKey, pNewVal)))
			{
				pNewVal->Release();
				EgLib::CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Failed to add TNT card."));
			}
			*pRetVal = pNewVal;
		}
		else
		{
			__CHECK_HRESULT(IMmTntCardBySeqCollImpl::Add(Key, SortKey, Value), _T("Failed to add TNT card."));
			Value->AddRef();
			*pRetVal = Value;
		}
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_IMmTntCardBySeqColl, e.Error());
	}

	return S_OK;
}
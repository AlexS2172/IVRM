// MmQvFutCollection.cpp : Implementation of CMmQvFutCollection

#include "stdafx.h"
#include "MmQvFutCollection.h"


/////////////////////////////////////////////////////////////////////////////
//
IMmQvFutAtomPtr CMmQvFutCollection::AddNew(LONG lId, CComObject<CMmQvFutAtom>** ppAtom)
{
	IMmQvFutAtomPtr			  spNewItem;
	CComObject<CMmQvFutAtom>* pValue = NULL;

	_CHK(CComObject<CMmQvFutAtom>::CreateInstance(&pValue), _T("Fail to add future."));
	spNewItem.Attach(pValue, TRUE);

	if(FAILED(IMmQvFutCollectionImpl::Add(lId, pValue)))
		CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Fail to add future."));

	pValue->m_nID = lId;
	pValue->m_bVisible = VARIANT_TRUE;
	if(ppAtom)
		*ppAtom = pValue;
	return spNewItem;
}


STDMETHODIMP CMmQvFutCollection::Add(LONG Key, IMmQvFutAtom* Value, IMmQvFutAtom** pRetVal)
{
	__CHECK_POINTER(pRetVal);

	try
	{
		if(m_coll.find(Key) != m_coll.end())
			CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Future with the same key is already exists."));


		if(!Value)
			*pRetVal = AddNew(Key).Detach();
		else
		{
			_CHK(IMmQvFutCollectionImpl::Add(Key, Value), _T("Fail to add future."));
			Value->AddRef();
			*pRetVal = Value;
		}
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)CComErrorWrapper::ErrorDescription(e), IID_IMmQvFutCollection, e.Error());
	}

	return S_OK;
}


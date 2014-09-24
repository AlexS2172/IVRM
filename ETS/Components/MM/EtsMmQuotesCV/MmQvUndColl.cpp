// MmQvUndColl.cpp : Implementation of CMmQvUndColl

#include "stdafx.h"
#include "MmQvUndColl.h"
#include "MmQvUndAtom.h"


IMmQvUndAtomPtr CMmQvUndColl::AddNew(long lUndID, _bstr_t bsUnd, CComObject<CMmQvUndAtom>** ppUndAtom)
{
	CComObject<CMmQvUndAtom>* pNewVal = NULL;
	IMmQvUndAtomPtr spVal;

	_CHK(CComObject<CMmQvUndAtom>::CreateInstance(&pNewVal), _T("Fail to add underlying."));
	spVal.Attach(pNewVal, TRUE);

	if(FAILED(IMmQvUndCollImpl::Add(lUndID, bsUnd, pNewVal)))
		CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Fail to add underlying."));

	pNewVal->m_nID = lUndID;
	pNewVal->m_bstrSymbol = bsUnd;

	if(ppUndAtom)
		*ppUndAtom = pNewVal;
	return spVal;
}

/////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CMmQvUndColl::Add(LONG Key, BSTR SortKey, IMmQvUndAtom* Value, IMmQvUndAtom** pRetVal)
{
	try
	{
		if(!Value)
			CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Cann't insert empty underlying."));

		_bstr_t bsKey(SortKey);

		if(m_collRef.find(Key) != m_collRef.end())
			CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Underlying with the same key is already exists."));

		if(!Value)
			*pRetVal = AddNew(Key, bsKey).Detach();
		else
		{
			_CHK(IMmQvUndCollImpl::Add(Key, bsKey, Value), _T("Fail to add underlying."));
			Value->AddRef();
			*pRetVal = Value;
		}
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)CComErrorWrapper::ErrorDescription(e), IID_IMmQvUndColl, e.Error());
	}

	return S_OK;
}




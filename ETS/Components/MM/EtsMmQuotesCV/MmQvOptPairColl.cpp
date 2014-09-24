// MmQvOptPairColl.cpp : Implementation of CMmQvOptPairColl

#include "stdafx.h"
#include "MmQvOptPairColl.h"

/////////////////////////////////////////////////////////////////////////////
//
IMmQvOptPairAtomPtr CMmQvOptPairColl::AddNew(long lRootID, _bstr_t bsRootName, CComObject<CMmQvOptPairAtom>** ppPairAtom)
{
	IMmQvOptPairAtomPtr spPairAtom;

	CComObject<CMmQvOptPairAtom>* pNewVal = NULL;

	_CHK(CComObject<CMmQvOptPairAtom>::CreateInstance(&pNewVal), _T("Fail to add option pair."));
	spPairAtom.Attach(pNewVal, TRUE);

	if(FAILED(IMmQvOptPairCollImpl::Add(lRootID, bsRootName, pNewVal)))
		CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Fail to add option pair."));

	pNewVal->m_nRootID = lRootID;

	if(ppPairAtom)
		*ppPairAtom = pNewVal;
	return spPairAtom;
}



STDMETHODIMP CMmQvOptPairColl::Add(LONG Key, BSTR SortKey, IMmQvOptPairAtom* Value, IMmQvOptPairAtom** pRetVal)
{
	try
	{
		if(!Value)
			CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Cann't insert empty option pair."));

		if(m_collRef.find(Key) != m_collRef.end())
			CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Option pair with the same key is already exists."));

		_bstr_t bsSortKey(SortKey);
		if(!Value)
			*pRetVal = AddNew(Key, bsSortKey).Detach();
		else
		{
			_CHK(IMmQvOptPairCollImpl::Add(Key, bsSortKey, Value), _T("Fail to add option pair."));
			Value->AddRef();
			*pRetVal = Value;
		}
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)CComErrorWrapper::ErrorDescription(e), IID_IMmQvOptPairColl, e.Error());
	}

	return S_OK;
}


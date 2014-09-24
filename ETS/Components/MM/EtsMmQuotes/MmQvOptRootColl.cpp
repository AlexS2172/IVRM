// MmQvOptRootColl.cpp : Implementation of CMmQvOptRootColl

#include "stdafx.h"
#include "MmQvOptRootColl.h"

CMmQvOptRootAtom* CMmQvOptRootColl::GetOptionRoot(long nOptRootID)
{
	CMmQvOptRootAtom* pAtom = NULL;
	CollType::iterator itrFind = m_collRef.find(nOptRootID); 
	if(itrFind != m_collRef.end())
		pAtom = dynamic_cast<CMmQvOptRootAtom*>(itrFind->second->second);
	return pAtom;
}

IMmQvOptRootAtomPtr CMmQvOptRootColl::GetItem(long lItemKey)
{
	IMmQvOptRootAtomPtr spReturn;
	CollType::iterator itrFind = m_collRef.find(lItemKey); 
	if(itrFind != m_collRef.end())
		spReturn.Attach(itrFind->second->second, TRUE);
	return spReturn;
}

IMmQvOptRootAtomPtr CMmQvOptRootColl::AddNew(long lOptRootID, _bstr_t bsRootName, CComObject<CMmQvOptRootAtom>** pRoot)
{
	IMmQvOptRootAtomPtr           spRetVal;
	CComObject<CMmQvOptRootAtom>* pNewVal = NULL;

	_CHK(CComObject<CMmQvOptRootAtom>::CreateInstance(&pNewVal), _T("Fail to add option root."));
	spRetVal.Attach(pNewVal, TRUE);

	if(FAILED(IMmQvOptRootCollImpl::Add(lOptRootID, bsRootName, pNewVal)))
		CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Fail to add option root."));

	pNewVal->m_nID		= lOptRootID; 
	pNewVal->m_bstrName	=  bsRootName;
	pNewVal->m_bVisible	= VARIANT_TRUE;

	if(pRoot)
		*pRoot = pNewVal;

	return spRetVal;
}
/////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CMmQvOptRootColl::Add(LONG Key, BSTR SortKey, IMmQvOptRootAtom* Value, IMmQvOptRootAtom** pRetVal)
{
	try
	{
		_bstr_t bsSortKey(SortKey);
		if(!Value)
			CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Cann't insert empty option root."));

		if(m_collRef.find(Key) != m_collRef.end())
			CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Option root with the same key is already exists."));

		if(!Value)
			*pRetVal = AddNew(Key,bsSortKey).Detach();
		else
		{
			_CHK(IMmQvOptRootCollImpl::Add(Key, bsSortKey, Value), _T("Fail to add option root."));
			Value->AddRef();
			*pRetVal = Value;
		}
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)CComErrorWrapper::ErrorDescription(e), IID_IMmQvOptRootColl, e.Error());
	}

	return S_OK;
}


// MmQvFutRootCollection.cpp : Implementation of CMmQvFutRootCollection

#include "stdafx.h"
#include "MmQvFutRootCollection.h"


// CMmQvFutRootCollection

IMmQvFutRootAtomPtr CMmQvFutRootCollection::AddNew(long lRootID,  CComObject<CMmQvFutRootAtom>** ppFutRoot)
{
	CComObject<CMmQvFutRootAtom>* pValue = NULL;
	IMmQvFutRootAtomPtr spFutRoot;

	_CHK(CComObject<CMmQvFutRootAtom>::CreateInstance(&pValue), _T("Fail to add future Root."));
	spFutRoot.Attach(pValue, TRUE);

	if(FAILED(IMmQvFutRootCollectionImpl::Add(lRootID, pValue)))
		CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Fail to add futures root."));

	pValue->m_nID = lRootID;

	if(ppFutRoot)
		*ppFutRoot = pValue;
	return spFutRoot;
}

/////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CMmQvFutRootCollection::Add(LONG Key, IMmQvFutRootAtom* Value, IMmQvFutRootAtom** pRetVal)
{
	__CHECK_POINTER(pRetVal);

	try
	{
		if(m_coll.find(Key) != m_coll.end())
			CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Futures Root with the same key is already exists."));

		
		if(!Value)
			*pRetVal = AddNew(Key).Detach();
		else
		{
			_CHK(IMmQvFutRootCollectionImpl::Add(Key, Value), _T("Fail to add future."));
			Value->AddRef();
			*pRetVal = Value;
		}
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)CComErrorWrapper::ErrorDescription(e), IID_IMmQvFutRootCollection, e.Error());
	}

	return S_OK;
}


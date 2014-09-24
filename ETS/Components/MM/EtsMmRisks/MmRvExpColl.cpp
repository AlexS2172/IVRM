// MmRvExpColl.cpp : Implementation of CMmRvExpColl

#include "stdafx.h"
#include "MmRvExpColl.h"


// CMmRvExpColl
IMmRvExpAtomPtr CMmRvExpColl::AddNew(DATE dtExpiry, CComObject<CMmRvExpAtom>** pObject)
{
	CComObject<CMmRvExpAtom>* pNewVal = NULL;
	IMmRvExpAtomPtr spNewVal;

	__CHECK_HRESULT(CComObject<CMmRvExpAtom>::CreateInstance(&pNewVal), _T("Fail to add expiry."));
	spNewVal.Attach(pNewVal, TRUE);

	if(FAILED(IMmRvExpCollImpl::Add(dtExpiry, pNewVal)))
		CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Fail to add expiry."));

	if(pObject)
		*pObject = pNewVal;

	pNewVal->m_dtExpiry = dtExpiry;
	pNewVal->m_nExpiryMonth = static_cast<LONG>(dtExpiry);

	return spNewVal;
}

/////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CMmRvExpColl::Add(DATE Key, IMmRvExpAtom* Value, IMmRvExpAtom** pRetVal)
{
	__CHECK_POINTER(pRetVal);

	try
	{
		if(m_coll.find(Key) != m_coll.end())
			CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Expiry with the same key is already exists."));


		if(!Value)
			*pRetVal = AddNew(Key).Detach();
		else
		{
			__CHECK_HRESULT(IMmRvExpCollImpl::Add(Key, Value), _T("Fail to add expiry."));
			Value->AddRef();
			*pRetVal = Value;
		}
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)CComErrorWrapper::ErrorDescription(e), IID_IMmRvExpColl, e.Error());
	}

	return S_OK;
}

// MmQvExpColl.cpp : Implementation of CMmQvExpColl

#include "stdafx.h"
#include "MmQvExpColl.h"


IMmQvExpAtomPtr CMmQvExpColl::GetItem(DATE Key)
{
	IMmQvExpAtomPtr spReturn;
	CollType::iterator itFind =  m_coll.find(Key);
	if(itFind != m_coll.end())
		spReturn = itFind->second;
	return spReturn;
}

IMmQvExpAtomPtr CMmQvExpColl::GetExpiry(DATE dtExpiration)
{
	IMmQvExpAtomPtr spReturn;
	CollType::iterator itFind = m_coll.find(dtExpiration);
	if(itFind != m_coll.end())
		spReturn = itFind->second;

	return spReturn;
}

IMmQvExpAtomPtr CMmQvExpColl::AddNew(DATE Key, CComObject<CMmQvExpAtom>** pAtom)
{
	IMmQvExpAtomPtr spRet;
	
	CComObject<CMmQvExpAtom>* pExp = NULL;
	_CHK(CComObject<CMmQvExpAtom>::CreateInstance(&pExp), _T("Fail to add expiry."));
	spRet.Attach(pExp, TRUE);

	if(FAILED(IMmQvExpCollImpl::Add(Key, pExp)))
		CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Fail to add expiry."));

	if(pExp)
	{
		pExp->m_dtExpiryMonth = Key;
		pExp->m_bVisible	  = VARIANT_TRUE;	
	}

	if(pAtom)
		*pAtom = pExp;

	return spRet;
}

/////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CMmQvExpColl::Add(DATE Key, IMmQvExpAtom* Value, IMmQvExpAtom** pRetVal)
{
	__CHECK_POINTER(pRetVal);

	DATE dtExp = (DATE)(static_cast<long>(Key));
	try
	{
		if(m_coll.find(dtExp) != m_coll.end())
			CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Expiry with the same key is already exists."));

		CComObject<CMmQvExpAtom>* pExp = NULL;

		if(!Value)
			*pRetVal = AddNew(dtExp).Detach();
		else
		{
			_CHK(IMmQvExpCollImpl::Add(dtExp, Value), _T("Fail to add expiry."));
			Value->AddRef();
			*pRetVal = Value;
		}
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)CComErrorWrapper::ErrorDescription(e), IID_IMmQvExpColl, e.Error());
	}

	return S_OK;
}





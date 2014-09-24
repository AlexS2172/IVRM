// EtsBrokerColl.cpp : Implementation of CEtsBrokerColl

#include "stdafx.h"
#include "EtsBrokerColl.h"

/////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CEtsBrokerColl::Add(LONG Key, BSTR SortKey, IEtsBrokerAtom* Value, IEtsBrokerAtom** pRetVal)
{
	try
	{
		if(m_collRef.find(Key) != m_collRef.end())
			EgLib::CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Broker with the same key is already exists."));

		_bstr_t bsKey(SortKey);

		if(!Value)
			*pRetVal = AddNew(Key, bsKey).Detach();
		else
		{
			_CHK(IEtsBrokerCollImpl::Add(Key, CComBSTR(SortKey), Value), _T("Fail to add broker."));
			Value->AddRef();
			*pRetVal = Value;
		}
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_IEtsBrokerColl, e.Error());
	}

	return S_OK;
}

IEtsBrokerAtomPtr CEtsBrokerColl::AddNew(long lID, _bstr_t bsName, _bstr_t bsDescription, CComObject<CEtsBrokerAtom>** pAtom )
{
	CComObject<CEtsBrokerAtom>* pNewVal;
	IEtsBrokerAtomPtr spRet;

	_CHK(CComObject<CEtsBrokerAtom>::CreateInstance(&pNewVal), _T("Fail to add broker."));
	spRet.Attach(pNewVal, TRUE);

	if(FAILED(IEtsBrokerCollImpl::Add(lID , bsName, pNewVal)))
		EgLib::CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Fail to add broker."));

	pNewVal->m_nID		= lID;
	
	CStringW wsName((BSTR)bsName);
	wsName.Trim();

	pNewVal->m_bstrName	= wsName;
	pNewVal->m_bstrDesc = bsDescription;
	if(pAtom)
		*pAtom = pNewVal;
	return spRet;
}


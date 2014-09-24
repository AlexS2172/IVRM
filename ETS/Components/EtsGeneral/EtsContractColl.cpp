// EtsContractColl.cpp : Implementation of CEtsContractColl

#include "stdafx.h"
#include "EtsContractColl.h"

/////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CEtsContractColl::Add(LONG Key, BSTR SortKey, IEtsContractAtom* Value, IEtsContractAtom** pRetVal)
{
	try
	{
		if(m_collRef.find(Key) != m_collRef.end())
			EgLib::CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Contract with the same key is already exists."));

		_bstr_t bsSortKey(SortKey);
		if(!Value)
			*pRetVal = AddNew(Key, bsSortKey).Detach();
		else
		{
			_CHK(IEtsContractCollImpl::Add(Key, bsSortKey, Value), _T("Fail to add contract."));
			Value->AddRef();
			*pRetVal = Value;
		}
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_IEtsContractColl, e.Error());
	}
	return S_OK;
}


IEtsContractAtomPtr CEtsContractColl::AddNew(long lID, _bstr_t bsSymbol, CComObject<CEtsContractAtom>** pObject)
{
	CComObject<CEtsContractAtom>* pNewVal;
	IEtsContractAtomPtr spAtom;

	_CHK(CComObject<CEtsContractAtom>::CreateInstance(&pNewVal), _T("Fail to add contract."));
	spAtom.Attach(pNewVal, TRUE);

	if(FAILED(IEtsContractCollImpl::Add(lID, bsSymbol, pNewVal)))
		EgLib::CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Fail to add contract."));

    if(pObject)
		*pObject = pNewVal;
	return spAtom;
}

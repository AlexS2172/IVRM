// MmRvPosColl.cpp : Implementation of CMmRvPosColl
#include "stdafx.h"
#include "MmRvPosColl.h"

/////////////////////////////////////////////////////////////////////////////
//
IMmRvPosAtomPtr CMmRvPosColl::AddNew(long lContractId, _bstr_t bsSymbol, CComObject<CMmRvPosAtom>** ppObject)
{
	IMmRvPosAtomPtr spNew;
	CComObject<CMmRvPosAtom>* pNewVal = NULL;

	_CHK(CComObject<CMmRvPosAtom>::CreateInstance(&pNewVal), _T("Fail to add position."));
	spNew.Attach(pNewVal, TRUE);

	if(FAILED(IMmRvPosCollImpl::Add(lContractId, bsSymbol, pNewVal)))
		EgLib::CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Fail to add position."));

	pNewVal->m_nID        = lContractId;
	pNewVal->m_bstrSymbol = bsSymbol;

	if(ppObject)
		*ppObject = pNewVal;
	return spNew;
}

STDMETHODIMP CMmRvPosColl::Add(LONG Key, BSTR SortKey, IMmRvPosAtom* Value, IMmRvPosAtom** pRetVal)
{
	try
	{
		_bstr_t bsSortKey(SortKey);

		if(m_collRef.find(Key) != m_collRef.end())
			EgLib::CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Position with the same key is already exists."));

		if(!Value)
			*pRetVal = AddNew(Key, bsSortKey).Detach();
		else
		{
			_CHK(IMmRvPosCollImpl::Add(Key, bsSortKey, Value), _T("Fail to add position."));
			Value->AddRef();
			*pRetVal = Value;
		}
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_IMmRvPosColl, e.Error());
	}

	return S_OK;
}

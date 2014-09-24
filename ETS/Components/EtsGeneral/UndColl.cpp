// UndColl.cpp : Implementation of CUndColl

#include "stdafx.h"
#include "UndColl.h"

/////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CUndColl::Add(LONG Key, BSTR SortKey, IUndAtom* Value, IUndAtom** pRetVal)
{
	try
	{
		if(m_collRef.find(Key) != m_collRef.end())
			EgLib::CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Underlying with the same key is already exists."));

		_bstr_t bsSortKey(SortKey);

		if(!Value)
			*pRetVal = AddNew(Key, bsSortKey).Detach();
		else
		{
			_CHK(IUndCollImpl::Add(Key, bsSortKey, Value), _T("Fail to add underlying."));
			Value->AddRef();
			*pRetVal = Value;
		}
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_IUndColl, e.Error());
	}

	return S_OK;
}


IUndAtomPtr CUndColl::AddNew(long lID, _bstr_t bsSymbol, CComObject<CUndAtom>** pAtom )
{
	CComObject<CUndAtom>* pNewVal;
	IUndAtomPtr spRet;

	_CHK(CComObject<CUndAtom>::CreateInstance(&pNewVal), _T("Fail to create Underlying."));
	spRet.Attach(pNewVal, TRUE);

	pNewVal->put_ID(lID);
	pNewVal->put_Symbol(bsSymbol);

	if(FAILED(IUndCollImpl::Add(lID, bsSymbol, pNewVal)))
		EgLib::CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Fail to add Underlying."));

	if(pAtom)
		*pAtom = pNewVal;
	return spRet;

}

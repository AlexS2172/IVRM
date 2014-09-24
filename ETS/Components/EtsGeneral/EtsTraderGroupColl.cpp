// EtsTraderGroupColl.cpp : Implementation of CEtsTraderGroupGroupColl

#include "stdafx.h"
#include "EtsTraderGroupColl.h"

/////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CEtsTraderGroupColl::Add(LONG Key, BSTR SortKey, IEtsTraderGroupAtom* Value, IEtsTraderGroupAtom** pRetVal)
{
	try
	{
		if(m_collRef.find(Key) != m_collRef.end())
		{
			EgLib::CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Trader group with the same key is already exists."));
		}

		_bstr_t bsSortKey(SortKey);

		if(!Value)
			*pRetVal = AddNew(Key, bsSortKey).Detach();
		else
		{
			_CHK(IEtsTraderGroupCollImpl::Add(Key, CComBSTR(SortKey), Value), _T("Fail to add trader group."));
			Value->AddRef();
			*pRetVal = Value;
		}
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_IEtsTraderGroupColl, e.Error());
	}

	return S_OK;
}

IEtsTraderGroupAtomPtr CEtsTraderGroupColl::AddNew(long lID, _bstr_t bsName, _bstr_t bsDescription , CComObject<CEtsTraderGroupAtom>** pAtom )
{
	IEtsTraderGroupAtomPtr spRetVal;
	CComObject<CEtsTraderGroupAtom>* pNewVal;

	_CHK(CComObject<CEtsTraderGroupAtom>::CreateInstance(&pNewVal), _T("Fail to add trader group."));
	spRetVal.Attach(pNewVal,TRUE);

	if(FAILED(IEtsTraderGroupCollImpl::Add(lID, bsName, pNewVal)))
		EgLib::CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Fail to add trader."));
	
	CStringW wsName((BSTR)bsName);
	wsName.Trim();

	pNewVal->m_nID = lID;
	pNewVal->m_bstrDesc = bsDescription;
	pNewVal->m_bstrName =  wsName;

	if(pAtom)
		*pAtom = pNewVal; 
	return spRetVal;

}


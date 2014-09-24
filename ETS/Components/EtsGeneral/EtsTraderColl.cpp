// EtsTraderColl.cpp : Implementation of CEtsTraderColl

#include "stdafx.h"
#include "EtsTraderColl.h"
#define  TNT_ACCOUNT_MAXLEN 4

/////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CEtsTraderColl::Add(LONG Key, BSTR SortKey, IEtsTraderAtom* Value, IEtsTraderAtom** pRetVal)
{
	try
	{
		if(m_collRef.find(Key) != m_collRef.end())
		{
			EgLib::CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Trader with the same key is already exists."));
		}

		_bstr_t bsKey(SortKey);
		if(!Value)
			*pRetVal = AddNew(Key, bsKey).Detach();
		else
		{
			_CHK(IEtsTraderCollImpl::Add(Key, bsKey, Value), _T("Fail to add trader."));
			Value->AddRef();
			*pRetVal = Value;
		}
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_IEtsTraderColl, e.Error());
	}

	return S_OK;
}

IEtsTraderAtomPtr CEtsTraderColl::AddNew(long lKey, const _bstr_t& bsName, CComObject<CEtsTraderAtom>** pAtom )
{
	CComObject<CEtsTraderAtom>* pNewVal;
	IEtsTraderAtomPtr spTrader;

	CStringW strName((BSTR)bsName);
	strName.Trim();

	_bstr_t bsProcessedName(strName);

	_CHK(CComObject<CEtsTraderAtom>::CreateInstance(&pNewVal), _T("Fail to add trader."));
	spTrader.Attach(pNewVal, TRUE);
	if(FAILED(IEtsTraderCollImpl::Add(lKey, bsProcessedName, pNewVal)))
		EgLib::CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Fail to add trader."));
	 
	pNewVal->m_nID				= lKey;
	pNewVal->m_bstrName			= bsProcessedName;
	pNewVal->m_bstrTntAccount	= strName.Left(TNT_ACCOUNT_MAXLEN).MakeUpper();

	if(pAtom)
		*pAtom = pNewVal;
	return spTrader;

}


// MmQvQuoteColl.cpp : Implementation of CMmQvQuoteColl

#include "stdafx.h"
#include "MmQvQuoteColl.h"

IMmQvQuoteAtomPtr CMmQvQuoteColl::AddNew(long lExchangeID, _bstr_t bsExchangeCode, CComObject<CMmQvQuoteAtom>** ppAtom)
{
	IMmQvQuoteAtomPtr spQuoteAtom;
	CComObject<CMmQvQuoteAtom>* pNewVal = NULL;

	_CHK(CComObject<CMmQvQuoteAtom>::CreateInstance(&pNewVal), _T("Fail to add quote."));
	
	spQuoteAtom.Attach(pNewVal, TRUE);
	if(FAILED(IMmQvQuoteCollImpl::Add(lExchangeID, bsExchangeCode, pNewVal)))
		CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Fail to add quote."));

	pNewVal->SetExchangeID(lExchangeID);
	pNewVal->SetExchangeCode(bsExchangeCode);

	if(ppAtom)
		*ppAtom = pNewVal;

	return spQuoteAtom;
}

/////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CMmQvQuoteColl::Add(LONG Key, BSTR SortKey, IMmQvQuoteAtom* Value, IMmQvQuoteAtom** pRetVal)
{
	try
	{
		if(!Value)
			CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Cann't insert empty quote."));

		if(m_collRef.find(Key) != m_collRef.end())
			CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Quote with the same key is already exists."));

		_bstr_t bsQuote(SortKey);

		if(!Value)
			*pRetVal = AddNew(Key, bsQuote).Detach();
		else
		{
			IMmQvQuoteAtomPtr spValue(Value);
			_CHK(IMmQvQuoteCollImpl::Add(Key, bsQuote, Value), _T("Fail to add quote."));
			*pRetVal = spValue.Detach();			
		}
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)CComErrorWrapper::ErrorDescription(e), IID_IMmQvQuoteColl, e.Error());
	}

	return S_OK;
}


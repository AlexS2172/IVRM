// EtsStrategyColl.cpp : Implementation of CEtsStrategyColl

#include "stdafx.h"
#include "EtsStrategyColl.h"

/////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CEtsStrategyColl::Add(LONG Key, BSTR SortKey, IEtsStrategyAtom* Value, IEtsStrategyAtom** pRetVal)
{
	try
	{
		if(m_collRef.find(Key) != m_collRef.end())
			EgLib::CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Strategy with the same key is already exists."));

		_bstr_t bsKey(SortKey);


		if(!Value)
			*pRetVal = AddNew(Key, bsKey).Detach();
		else
		{
			_CHK(IEtsStrategyCollImpl::Add(Key, CComBSTR(SortKey), Value), _T("Fail to add strategy."));
			Value->AddRef();
			*pRetVal = Value;
		}
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_IEtsStrategyColl, e.Error());
	}

	return S_OK;
}

IEtsStrategyAtomPtr CEtsStrategyColl::AddNew(long lKey, _bstr_t bsKey, _bstr_t bsDescription, CComObject<CEtsStrategyAtom>** pAtom)
{
	IEtsStrategyAtomPtr spRetVal;
	CComObject<CEtsStrategyAtom>* pNewVal;
	_CHK(CComObject<CEtsStrategyAtom>::CreateInstance(&pNewVal), _T("Fail to add strategy."));
	spRetVal.Attach(pNewVal ,TRUE);

	if(FAILED(IEtsStrategyCollImpl::Add(lKey, bsKey, pNewVal)))
		EgLib::CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Fail to add strategy."));

	pNewVal->m_nID		= lKey;
	pNewVal->m_bstrName = bsKey;
	pNewVal->m_bstrDesc = bsDescription;
	if(pAtom)
		*pAtom = pNewVal;
	return spRetVal;
}



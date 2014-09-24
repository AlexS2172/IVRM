// EtsTraderUndColl.cpp : Implementation of CEtsTraderUndColl

#include "stdafx.h"
#include "EtsTraderUndColl.h"

/////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CEtsTraderUndColl::Add(LONG Key, BSTR SortKey, IEtsTraderUndAtom* Value, IEtsTraderUndAtom** pRetVal)
{
	try
	{
		if(m_collRef.find(Key) != m_collRef.end())
		{
			EgLib::CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Trader underlying with the same key is already exists."));
		}

		if(!Value)
		{
			CComObject<CEtsTraderUndAtom>* pNewVal;
			_CHK(CComObject<CEtsTraderUndAtom>::CreateInstance(&pNewVal), _T("Fail to add trader underlying."));
			pNewVal->AddRef();
			if(FAILED(IEtsTraderUndCollImpl::Add(Key, CComBSTR(SortKey), pNewVal)))
			{
				pNewVal->Release();
				EgLib::CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Fail to add trader underlying."));
			}
			*pRetVal = pNewVal;
		}
		else
		{
			_CHK(IEtsTraderUndCollImpl::Add(Key, CComBSTR(SortKey), Value), _T("Fail to add trader underlying."));
			Value->AddRef();
			*pRetVal = Value;
		}
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_IEtsTraderUndColl, e.Error());
	}

	return S_OK;
}


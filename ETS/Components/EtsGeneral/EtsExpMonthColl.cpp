// EtsExpMonthColl.cpp : Implementation of CEtsExpMonthColl

#include "stdafx.h"
#include "EtsExpMonthColl.h"

/////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CEtsExpMonthColl::Add(LONG Key, IEtsExpMonthAtom* Value, IEtsExpMonthAtom** pRetVal)
{
	__CHECK_POINTER(pRetVal);

	try
	{
		if(m_coll.find(Key) != m_coll.end())
		{
			EgLib::CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Expiry month with the same key is already exists."));
		}

		CComObject<CEtsExpMonthAtom>* pNewVal;

		if(!Value)
		{
			_CHK(CComObject<CEtsExpMonthAtom>::CreateInstance(&pNewVal), _T("Fail to add expiry month."));
			pNewVal->AddRef();
			if(FAILED(IEtsExpMonthCollImpl::Add(Key, pNewVal)))
			{
				pNewVal->Release();
				EgLib::CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Fail to add expiry month."));
			}
			*pRetVal = pNewVal;
		}
		else
		{
			_CHK(IEtsExpMonthCollImpl::Add(Key, Value), _T("Fail to add expiry month."));
			Value->AddRef();
			*pRetVal = Value;
		}
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_IEtsExpMonthColl, e.Error());
	}

	return S_OK;
}


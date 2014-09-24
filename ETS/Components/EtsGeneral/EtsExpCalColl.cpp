// EtsExpCalColl.cpp : Implementation of CEtsExpCalColl

#include "stdafx.h"
#include "EtsExpCalColl.h"
#include "EtsExpMonthAtom.h"

/////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CEtsExpCalColl::Add(LONG Key, IEtsExpMonthAtom* Value, IEtsExpMonthAtom** pRetVal)
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
			if(FAILED(IEtsExpCalCollImpl::Add(Key, pNewVal)))
			{
				pNewVal->Release();
				EgLib::CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Fail to add expiry month."));
			}
			*pRetVal = pNewVal;
		}
		else
		{
			_CHK(IEtsExpCalCollImpl::Add(Key, Value), _T("Fail to add expiry month."));
			Value->AddRef();
			*pRetVal = Value;
		}
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_IEtsExpCalColl, e.Error());
	}

	return S_OK;
}

/////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CEtsExpCalColl::GetVegaWeight(DATE dtExpiry, DOUBLE* pdRetVal)
{
	DOUBLE dVegaWeight = 0.;
	try
	{
		if (!pdRetVal)
			EgLib::CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Invalid argument"));

		LONG nExpiry = static_cast<LONG>(floor(dtExpiry));	// truncate time
		DOUBLE dtToday; 
		::GetNYDateTimeAsDATE(&dtToday);
		LONG nToday = static_cast<LONG>(floor(dtToday));

		if(nExpiry > nToday)
		{
			CollType::iterator itrExp = m_coll.find(nExpiry);
			if(itrExp != m_coll.end())
			{
				IEtsExpMonthAtomPtr spExpMonth = itrExp->second;
				if(spExpMonth != NULL)
				{
					_CHK(spExpMonth->get_VegaWeight(&dVegaWeight), _T("Fail to get vega weight."));
				}

			}
			if(DoubleLEQZero(dVegaWeight))
			{
				dVegaWeight = sqrt(((double)m_nMaturity / (double)((nExpiry - nToday))));
			}
		}
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_IEtsExpCalColl, e.Error());
	}

	*pdRetVal = dVegaWeight;
	return S_OK;
}

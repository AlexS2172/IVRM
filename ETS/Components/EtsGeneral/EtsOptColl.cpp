// EtsOptColl.cpp : Implementation of CEtsOptColl

#include "stdafx.h"
#include "EtsOptColl.h"

/////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CEtsOptColl::Add(LONG Key, BSTR SortKey, IEtsOptAtom* Value, IEtsOptAtom** pRetVal)
{
	try
	{
		if(m_collRef.find(Key) != m_collRef.end())
		{
			EgLib::CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Option with the same key is already exists."));
		}

		if(!Value)
		{
			CComObject<CEtsOptAtom>* pNewVal;
			_CHK(CComObject<CEtsOptAtom>::CreateInstance(&pNewVal), _T("Fail to add option."));
			pNewVal->AddRef();
			if(FAILED(IEtsOptCollImpl::Add(Key, CComBSTR(SortKey), pNewVal)))
			{
				pNewVal->Release();
				EgLib::CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Fail to add option."));
			}
			*pRetVal = pNewVal;
		}
		else
		{
			_CHK(IEtsOptCollImpl::Add(Key, CComBSTR(SortKey), Value), _T("Fail to add option."));
			Value->AddRef();
			*pRetVal = Value;
		}
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_IEtsOptColl, e.Error());
	}

	return S_OK;
}


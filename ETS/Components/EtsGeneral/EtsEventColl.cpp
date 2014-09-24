// EtsEventColl.cpp : Implementation of CEtsEventColl

#include "stdafx.h"
#include "EtsEventColl.h"

/////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CEtsEventColl::Add(LONG Key, DATE SortKey, IEtsEventAtom* Value, IEtsEventAtom** pRetVal)
{
	try
	{
		if(m_collRef.find(Key) != m_collRef.end())
		{
			EgLib::CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Event with the same key is already exists."));
		}

		if(!Value)
		{
			CComObject<CEtsEventAtom>* pNewVal;
			_CHK(CComObject<CEtsEventAtom>::CreateInstance(&pNewVal), _T("Fail to add event."));
			pNewVal->AddRef();
			if(FAILED(IEtsEventCollImpl::Add(Key, SortKey, pNewVal)))
			{
				pNewVal->Release();
				EgLib::CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Fail to add event."));
			}
			*pRetVal = pNewVal;
		}
		else
		{
			_CHK(IEtsEventCollImpl::Add(Key, SortKey, Value), _T("Fail to add event."));
			Value->AddRef();
			*pRetVal = Value;
		}
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_IEtsEventColl, e.Error());
	}

	return S_OK;
}


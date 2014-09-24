// BasketAssetComponentsColl.cpp : Implementation of CBasketAssetComponentsColl

#include "stdafx.h"
#include "BasketAssetComponentsColl.h"


// CBasketAssetComponentsColl
//-------------------------------------------------------------------------------------------------------------------//
STDMETHODIMP CBasketAssetComponentsColl::Add(LONG Key, BSTR SortKey, IBasketAssetComponent* Value, IBasketAssetComponent** pRetVal)
{
	try
	{
		if(m_collRef.find(Key) == m_collRef.end())
		{
			_bstr_t bsSortKey(SortKey);
			if (Value != NULL)
			{
				_CHK(IBasketAssetComponentsCollImpl::Add(Key, bsSortKey, Value), _T("Fail to add Component."));
				Value->AddRef();
				*pRetVal = Value;
			}
			else
				return E_POINTER;
		}
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_IBasketAssetComponentsColl, e.Error());
	}
	return S_OK;
}
//-------------------------------------------------------------------------------------------------------------------//

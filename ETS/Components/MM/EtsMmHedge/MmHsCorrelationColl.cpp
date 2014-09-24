// MmHsCorrelationColl.cpp : Implementation of CMmHsCorrelationColl

#include "stdafx.h"
#include "MmHsCorrelationColl.h"

// CMmHsCorrelationColl

STDMETHODIMP CMmHsCorrelationColl::Add( LONG Id1, LONG Id2, DOUBLE Value )
{
	try
	{
		if ( Id1==Id2 )
			CComErrorWrapper::ThrowError(E_FAIL, _T("Invalid key"));

		LONG nId1 = Id1;
		LONG nId2 = Id2;

		if ( Id1 > Id2 )
		{
			nId1 = Id2;
			nId2 = Id1;
		}

		CMmHsKey _key(nId1, nId2);
		DOUBLE _item = Value;

		IterType it = m_coll.find(_key);
		if ( it != m_coll.end() )
			CComErrorWrapper::ThrowError(E_FAIL, _T("Value with the same key already exists"));

		m_coll[_key] = _item;
	}
	catch(const _com_error& e)
	{
		return Error( (PTCHAR)CComErrorWrapper::ErrorDescription(e), IID_IMmHsCorrelationColl, e.Error());
	}

	return S_OK;
}

STDMETHODIMP CMmHsCorrelationColl::Remove( LONG Id1, LONG Id2 )
{	
	try
	{
		if ( Id1==Id2 )
			CComErrorWrapper::ThrowError(E_FAIL, _T("Invalid key"));

		LONG nId1 = Id1;
		LONG nId2 = Id2;

		if ( Id1 > Id2 )
		{
			nId1 = Id2;
			nId2 = Id1;
		}

		CMmHsKey _key(nId1, nId2);
		__CHECK_HRESULT(IMmHsCorrelationCollImpl::Remove(_key), _T("Failed to remove item"));
	}
	catch(const _com_error& e)
	{
		return Error( (PTCHAR)CComErrorWrapper::ErrorDescription(e), IID_IMmHsCorrelationColl, e.Error());
	}

	return S_OK;
}

STDMETHODIMP CMmHsCorrelationColl::get_Item( LONG Id1, LONG Id2, DOUBLE* pVal)
{	
	try
	{
		if (!pVal)
			CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Invalid argument"));		

		if (Id1==Id2)
		{
            *pVal = 1.;
			return S_OK;
		}

		LONG nId1 = Id1;
		LONG nId2 = Id2;

		if ( Id1 > Id2 )
		{
			nId1 = Id2;
			nId2 = Id1;
		}

		CMmHsKey _key(nId1, nId2);

		IterType it = m_coll.find(_key);
		if ( it != m_coll.end() )
			*pVal = it->second;
		else
			*pVal = BAD_DOUBLE_VALUE;
	}
	catch(const _com_error& e)
	{
		return Error( (PTCHAR)CComErrorWrapper::ErrorDescription(e), IID_IMmHsCorrelationColl, e.Error());
	}

	return S_OK;
}

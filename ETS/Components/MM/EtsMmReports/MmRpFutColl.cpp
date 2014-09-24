// MmRpFutColl.cpp : Implementation of CMmRpFutColl

#include "stdafx.h"
#include "MmRpFutColl.h"
#include "MmRpFutAtom.h"

// CMmRpFutColl

STDMETHODIMP CMmRpFutColl::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* arr[] = 
	{
		&IID_IMmRpFutColl
	};

	for (int i=0; i < sizeof(arr) / sizeof(arr[0]); i++)
	{
		if (InlineIsEqualGUID(*arr[i],riid))
			return S_OK;
	}
	return S_FALSE;
}

STDMETHODIMP CMmRpFutColl::Add(LONG Key, IMmRpFutAtom* Value, IMmRpFutAtom** pRetVal)
{
	__CHECK_POINTER(pRetVal);

	try
	{
		if(m_coll.find(Key) != m_coll.end())
		{
			CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Value with the same key already exists."));
		}

		CComObject<CMmRpFutAtom>* pFut = NULL;

		if(!Value)
		{
			__CHECK_HRESULT(CComObject<CMmRpFutAtom>::CreateInstance(&pFut), _T("Failed to add value."));
			pFut->AddRef();
			if(FAILED(IMmRpFutCollImpl::Add(Key, pFut)))
			{
				pFut->Release();
				CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Failed to add value."));
			}
			*pRetVal = pFut;
		}
		else
		{
			__CHECK_HRESULT(IMmRpFutCollImpl::Add(Key, Value), _T("Failed to add value."));
			Value->AddRef();
			*pRetVal = Value;
		}
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)CComErrorWrapper::ErrorDescription(e), IID_IMmRpFutColl, e.Error());
	}

	return S_OK;
}


STDMETHODIMP CMmRpFutColl::GetPriceProperty_ ( BSTR _Symbol , IMMRpPrice** _ppPrice )
{
	CollType::iterator it = m_coll.begin() ;
	_bstr_t sSymbolFind ( _Symbol ) ;
	for ( ; it != m_coll.end() ; it++ )
	{
		IMmRpFutAtom* pUnd = it->second ;
		CComBSTR bsSym ;
		pUnd->get_Symbol ( &bsSym ) ;
		_bstr_t sSymLoop ( bsSym ) ;
		if ( sSymLoop == sSymbolFind )
		{
			pUnd->get_Price ( _ppPrice ) ;
			if ( *_ppPrice )
			{
				(*_ppPrice)->AddRef() ;
				break ;
			}
		}
	}
	return S_OK ;
}
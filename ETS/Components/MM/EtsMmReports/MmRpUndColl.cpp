// MmRpUndColl.cpp : Implementation of CMmRpUndColl

#include "stdafx.h"
#include "MmRpUndColl.h"
#include "MmRpUndAtom.h"


// CMmRpUndColl

STDMETHODIMP CMmRpUndColl::Add(BSTR Key, IMmRpUndAtom* Value, IMmRpUndAtom** pRetVal)
{
	__CHECK_POINTER(pRetVal);

	try
	{
		_bstr_t bsKey(Key);
		if(m_coll.find(bsKey) != m_coll.end())
		{
			CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Value with the same key already exists."));
		}

		CComObject<CMmRpUndAtom>* pUnd;

		if(!Value)
		{
			__CHECK_HRESULT(CComObject<CMmRpUndAtom>::CreateInstance(&pUnd), _T("Failed to add value."));
			pUnd->AddRef();
			if(FAILED(IMmRpUndCollImpl::Add(bsKey, pUnd)))
			{
				pUnd->Release();
				CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Failed to add value."));
			}
			*pRetVal = pUnd;
		}
		else
		{
			__CHECK_HRESULT(IMmRpUndCollImpl::Add(bsKey, Value), _T("Failed to add value."));
			Value->AddRef();
			*pRetVal = Value;
		}
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)CComErrorWrapper::ErrorDescription(e), IID_IMmRpUndColl, e.Error());
	}

	return S_OK;
}

STDMETHODIMP CMmRpUndColl::GetPriceProperty(BSTR _Symbol , IMMRpPrice** _ppPrice) 
{
	
	CollType::iterator it = m_coll.begin() ;
	_bstr_t sSymbolFind ( _Symbol ) ;
	for ( ; it != m_coll.end() ; it++ )
	{
		IMmRpUndAtom* pUnd = it->second ;
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

STDMETHODIMP CMmRpUndColl::GetPricePropertyOpt( BSTR _Underlying, BSTR _Symbol , IMMRpPrice** _pPrice) 
{
	CollType::iterator it = m_coll.begin() ;
	_bstr_t sUndFind ( _Underlying ) ;
	for ( ; it != m_coll.end() ; it++ )
	{
		IMmRpUndAtom* pUnd = it->second ;
		CComBSTR bsSym ;
		pUnd->get_Symbol ( &bsSym ) ;
		_bstr_t sSymLoop ( bsSym ) ;
		if ( sSymLoop == sUndFind )
		{
			IMmRpOptCollPtr spOptColl ;
			pUnd->get_Opt( &spOptColl ) ;
			ATLASSERT ( spOptColl ) ;
			spOptColl->GetPriceProperty_ ( _Symbol , _pPrice ) ;
			if ( *_pPrice )
				break ;
		}
	}
	return S_OK ;
}
STDMETHODIMP CMmRpUndColl::GetPricePropertyFut( BSTR _Underlying, BSTR _Symbol , IMMRpPrice** _pPrice) 
{
	CollType::iterator it = m_coll.begin() ;
	_bstr_t sUndFind ( _Underlying ) ;
	CComBSTR sSymFut ( _Symbol ) ;
	for ( ; it != m_coll.end() ; it++ )
	{
		IMmRpUndAtom* pUnd = it->second ;
		CComBSTR bsSym ;
		pUnd->get_Symbol ( &bsSym ) ;
		_bstr_t sSymLoop ( bsSym ) ;
		if ( sSymLoop == sUndFind )
		{
			IMmRpFutCollPtr spFutColl ;
			pUnd->get_Fut( &spFutColl ) ;
			ATLASSERT ( spFutColl ) ;
			spFutColl->GetPriceProperty_ ( _Symbol , _pPrice ) ;
			if ( *_pPrice )
				break ;
			else
			{
				IMmRpFutAtomPtr	activeFuture;
				_CHK(pUnd->get_ActiveFuture(&activeFuture));
				if ( activeFuture ) 
				{
					CComBSTR bsSymAct ;
					activeFuture->get_Symbol ( &bsSymAct ) ;
					if ( bsSymAct == sSymFut )
					{
						activeFuture->get_Price ( _pPrice ) ;
					}
				}
			}
		}
	}
	return S_OK ;
}
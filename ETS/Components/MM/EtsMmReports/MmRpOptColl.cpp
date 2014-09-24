// MmRpOptColl.cpp : Implementation of CMmRpOptColl

#include "stdafx.h"
#include "MmRpOptColl.h"
#include "MmRpOptAtom.h"


// CMmRpOptColl

STDMETHODIMP CMmRpOptColl::Add(LONG Key, IMmRpOptAtom* Value, IMmRpOptAtom** pRetVal)
{
	__CHECK_POINTER(pRetVal);

	try
	{
		if(m_coll.find(Key) != m_coll.end())
		{
			CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Value with the same key already exists."));
		}

		CComObject<CMmRpOptAtom>* pOpt;

		if(!Value)
		{
			__CHECK_HRESULT(CComObject<CMmRpOptAtom>::CreateInstance(&pOpt), _T("Failed to add value."));
			pOpt->AddRef();
			if(FAILED(IMmRpOptCollImpl::Add(Key, pOpt)))
			{
				pOpt->Release();
				CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Failed to add value."));
			}
			*pRetVal = pOpt;
		}
		else
		{
			__CHECK_HRESULT(IMmRpOptCollImpl::Add(Key, Value), _T("Failed to add value."));
			Value->AddRef();
			*pRetVal = Value;
		}
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)CComErrorWrapper::ErrorDescription(e), IID_IMmRpOptColl, e.Error());
	}

	return S_OK;
}

STDMETHODIMP CMmRpOptColl::GetPriceProperty_(BSTR _Symbol , IMMRpPrice** _ppPrice) 
{
	CollType::iterator it = m_coll.begin() ;
	_bstr_t sSymbolFind ( _Symbol ) ;
	for ( ; it != m_coll.end() ; it++ )
	{
		IMmRpOptAtom* pOpt = it->second ;
		CComBSTR bsSym ;
		pOpt->get_Symbol ( &bsSym ) ;
		_bstr_t sSymLoop ( bsSym ) ;
		if ( sSymLoop == sSymbolFind )
		{
			pOpt->get_Price ( _ppPrice ) ;
			if ( *_ppPrice )
			{
				(*_ppPrice)->AddRef() ;
				break ;
			}
		}
	}
	return S_OK ;
}

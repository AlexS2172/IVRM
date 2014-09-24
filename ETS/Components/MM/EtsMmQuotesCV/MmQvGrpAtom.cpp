// MmQvGrpAtom.cpp : Implementation of CMmQvGrpAtom
#include "stdafx.h"
#include "MmQvGrpAtom.h"

/////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CMmQvGrpAtom::CleanUp(VARIANT_BOOL vbAll)
{
	HRESULT hr = S_OK;
	try
	{
		m_nID = 0L;
		m_bstrSymbol.Empty();
		m_enContractType = enCtNone;
		m_bUseCustRates = VARIANT_FALSE;

		if(m_pUnd != NULL)									m_pUnd->CleanUp();
		if(m_pFut != NULL)									m_pFut->CleanUp();
		if(m_pFutRoot != NULL)								m_pFutRoot->CleanUp();
		if ( vbAll == VARIANT_TRUE && m_pExpAll != NULL )	m_pExpAll->Clear();
		if(m_pStrikeAll != NULL)							m_pStrikeAll->Clear();
		if(m_spUndExchAll != NULL)							m_spUndExchAll->Clear();
		if(m_spOptExchAll != NULL)							m_spOptExchAll->Clear();
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)CComErrorWrapper::ErrorDescription(e), IID_IMmQvGrpAtom, e.Error());
	}
	/*catch (...)
	{
		DWORD dwErrorCode = GetLastError();
		hr = HRESULT_FROM_WIN32(dwErrorCode);
		if(FAILED(hr))
		{
			_bstr_t bsMessage(L"Error ");
			bsMessage += _bstr_t(dwErrorCode);

			hr = Error((LPCSTR)bsMessage, IID_IEtsMmQuotesView, E_FAIL);
		}
		else
			hr = Error(_T("Unknown Error"), IID_IEtsMmQuotesView, E_FAIL);
	}*/
	return hr;
}

/////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CMmQvGrpAtom::get_IsStockOrIndex(VARIANT_BOOL* pRetVal)
{
	try
	{
		if(!pRetVal)
			CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Invalid argument"));

		*pRetVal = (m_enContractType == enCtStock || m_enContractType == enCtIndex) ? VARIANT_TRUE : VARIANT_FALSE;
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)CComErrorWrapper::ErrorDescription(e), IID_IMmQvGrpAtom, e.Error());
	}

	return S_OK;
}

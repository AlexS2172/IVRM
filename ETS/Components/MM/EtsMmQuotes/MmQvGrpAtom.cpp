// MmQvGrpAtom.cpp : Implementation of CMmQvGrpAtom
#include "stdafx.h"
#include "MmQvGrpAtom.h"
#include "MmQvSpreadColl.h"

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
		m_bShowIndexFutureOptions = VARIANT_FALSE;

		if(m_spFutRootColl != NULL)
			m_spFutRootColl->Clear();
		if(m_pUnd != NULL)									m_pUnd->CleanUp();
		if(vbAll == VARIANT_TRUE && m_pExpAll != NULL )		m_pExpAll->Clear();
		if(vbAll == VARIANT_TRUE && m_pFutExpAll != NULL )	m_pFutExpAll->Clear();
		if(m_pStrikeAll != NULL)							m_pStrikeAll->Clear();
		if(m_spUndExchAll != NULL)							m_spUndExchAll->Clear();
		if(m_spOptExchAll != NULL)							m_spOptExchAll->Clear();
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)CComErrorWrapper::ErrorDescription(e), IID_IMmQvGrpAtom, e.Error());
	}
	catch (...)
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
	}
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

		*pRetVal = (m_enContractType == enCtStock || (m_enContractType == enCtIndex && (m_bShowIndexFutureOptions==VARIANT_FALSE || m_pFutRootColl->m_coll.empty()))   ) ? VARIANT_TRUE : VARIANT_FALSE;
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)CComErrorWrapper::ErrorDescription(e), IID_IMmQvGrpAtom, e.Error());
	}

	return S_OK;
}

STDMETHODIMP CMmQvGrpAtom::get_ExpAll(IMmQvExpColl **pVal)
{
	if(!pVal)
	{
		return E_POINTER;
	}
	if(*pVal)
	{
		(*pVal)->Release();
		*pVal = NULL;
	}

	ObjectLock lock(this);
	*pVal = (m_enContractType == enCtStock || (m_enContractType == enCtIndex && ( m_bShowIndexFutureOptions == VARIANT_FALSE || m_pFutRootColl->m_coll.empty()))) ? (IMmQvExpColl*)m_spExpAll : (IMmQvExpColl*)m_spFutExpAll;
	if(*pVal)
	{
		(*pVal)->AddRef();
	}
	return S_OK;
}

HRESULT CMmQvGrpAtom::CreateSpreadCollection()
{
	HRESULT hr = S_OK;
	_CHK(CComObject<CMmQvSpreadColl>::CreateInstance(&m_pSpreadColl), _T("Fail to create Spread Collection."));
	m_spSpreadColl.Attach(m_pSpreadColl, TRUE);
	return hr;
}

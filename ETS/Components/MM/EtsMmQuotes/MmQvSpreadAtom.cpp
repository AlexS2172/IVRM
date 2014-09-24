// MmQvSpreadAtom.cpp : Implementation of CMmQvSpreadAtom

#include "stdafx.h"
#include "MmQvSpreadAtom.h"
#include "MmQvSpreadData.h"

// CMmQvSpreadAtom
STDMETHODIMP CMmQvSpreadAtom::get_OptionItemPositiion(LONG* pVal)
{
	if(!pVal)
		return E_POINTER;

	HRESULT hr = S_OK;
	try
	{
		if(m_spUndAtom==NULL)
		{
			CMmQvRowData* pRow = dynamic_cast<CMmQvRowData*>(m_spRowData.GetInterfacePtr());
			if(pRow)
			{
				*pVal = pRow->GetRowPosition();

			}
		}
		else
			*pVal = BAD_LONG_VALUE;		
	}
	catch (_com_error& e)
	{
		hr = Error((PTCHAR)CComErrorWrapper::ErrorDescription(e), IID_IMmQvSpreadAtom, e.Error());
	}
	return hr;
}


STDMETHODIMP CMmQvSpreadAtom::putref_RowData(EtsOptionTypeEnum enType, IMmQvRowData* newVal)
{
	HRESULT hr = S_OK;
	try
	{
		m_enOptionType = enType;
		m_spRowData    = newVal;
		m_spUndAtom    = NULL;
	}
	catch (_com_error& e)
	{
		hr = Error((PTCHAR)CComErrorWrapper::ErrorDescription(e), IID_IMmQvSpreadAtom, e.Error());
	}
	return hr;
}


STDMETHODIMP CMmQvSpreadAtom::get_Opt(IMmQvOptAtom** ppVal)
{
	if(!ppVal)
		return E_POINTER;
	HRESULT hr = S_OK;
	try
	{
		if(m_spUndAtom!=NULL)
			*ppVal = NULL;
		else
		{
			CMmQvRowData* pRow = dynamic_cast<CMmQvRowData*>(m_spRowData.GetInterfacePtr());
			if(pRow)
			{
				IMmQvOptAtomPtr spOptAtom;				
				spOptAtom.Attach(pRow->m_pOpt[static_cast<long>(m_enOptionType)], TRUE);
				*ppVal = spOptAtom.Detach();				
			}
		}
	}
	catch (_com_error& e)
	{
		hr = Error((PTCHAR)CComErrorWrapper::ErrorDescription(e), IID_IMmQvSpreadAtom, e.Error());
	}
	catch(...)
	{
		HRESULT _hr = HRESULT_FROM_WIN32(::GetLastError());
		if(SUCCEEDED(_hr)) _hr = E_FAIL;
		hr = Error((PTCHAR)"System Error", IID_IMmQvSpreadAtom, _hr);
	}

	return hr;
}

STDMETHODIMP CMmQvSpreadAtom::get_Quote(IMmQvQuoteAtom** ppVal)
{
	HRESULT hr = S_OK;
	if(!ppVal)
		return E_POINTER;

	try
	{
		CMmQvUndAtom* pUndAtom = dynamic_cast<CMmQvUndAtom*>(m_spUndAtom.GetInterfacePtr());
		if(pUndAtom)
		{
			IMmQvQuoteAtomPtr spQuote = pUndAtom->m_pQuote->GetQuote(m_lExchangeId==BAD_LONG_VALUE?0:m_lExchangeId);
			*ppVal = spQuote.Detach();
		}
		else
		{
			CMmQvRowData* pRow = dynamic_cast<CMmQvRowData*>(m_spRowData.GetInterfacePtr());
			if(pRow)
			{
				IMmQvQuoteAtomPtr spQuote;	
				spQuote.Attach(pRow->m_pOptQuote[static_cast<long>(m_enOptionType)],true);
				*ppVal = spQuote.Detach();				
			}
			else
			{
				CMmQvFutAtom* pFutAtom = dynamic_cast<CMmQvFutAtom*>(m_spFutAtom.GetInterfacePtr());
				if(pFutAtom)
				{
					IMmQvQuoteAtomPtr spQuote = pFutAtom->m_pQuote->GetQuote(m_lExchangeId==BAD_LONG_VALUE?0:m_lExchangeId);
					*ppVal = spQuote.Detach();		
				}
			}
		}
	}
	catch (_com_error& e)
	{
		hr = Error((PTCHAR)CComErrorWrapper::ErrorDescription(e), IID_IMmQvSpreadAtom, e.Error());
	}
	catch(...)
	{
		HRESULT _hr = HRESULT_FROM_WIN32(::GetLastError());
		if(SUCCEEDED(_hr)) _hr = E_FAIL;
		hr = Error((PTCHAR)"System Error", IID_IMmQvSpreadAtom, _hr);
	}
	return hr;
}

STDMETHODIMP CMmQvSpreadAtom::get_SpreadData(IMmQvSpreadData** ppVal)
{
	HRESULT hr     = S_OK;

	if(!ppVal)
		return E_POINTER;
	try
	{
		IMmQvQuoteAtomPtr spQuote;
		hr = get_Quote(&spQuote);
		if(SUCCEEDED(hr) && spQuote!=NULL)
		{
			CMmQvQuoteAtom* pQuote = dynamic_cast<CMmQvQuoteAtom*>(spQuote.GetInterfacePtr());
			if(pQuote)
			{
				CComObject<CMmQvSpreadData>* pSpreadData = NULL;
				hr = CComObject<CMmQvSpreadData>::CreateInstance(&pSpreadData);
				if(SUCCEEDED(hr) && pSpreadData)
				{
					IMmQvSpreadDataPtr spSpread;
					spSpread.Attach(pSpreadData, true);

					switch(m_enSpreadType)
					{
					case SPT_BUY: 
						pSpreadData->m_nQty = 1;
						break;
					case SPT_SELL: 
						pSpreadData->m_nQty = -1;
						break;
					default:
						pSpreadData->m_nQty = 0;
						break;
					}
					pSpreadData->m_dPrice		= pQuote->m_dPriceLast;
					pSpreadData->m_dAsk			= pQuote->m_dPriceAsk;
					pSpreadData->m_dBid			= pQuote->m_dPriceBid;
					pSpreadData->m_dTheoPrice	= pQuote->m_dPriceTheo;
					pSpreadData->m_dDelta		= pQuote->m_dDelta;
					pSpreadData->m_dGamma		= pQuote->m_dGamma;
					pSpreadData->m_dVega		= pQuote->m_dVega;
					pSpreadData->m_dTheta		= pQuote->m_dTheta;
					pSpreadData->m_dRho			= pQuote->m_dRho;
					if(m_spUndAtom!=NULL)
					{
						CMmQvUndAtom* pUndAtom = dynamic_cast<CMmQvUndAtom*>(m_spUndAtom.GetInterfacePtr());
						if(pUndAtom)
						{
							pSpreadData->m_enType       = pUndAtom->m_enUndType;
							pSpreadData->m_bsSeries     = pUndAtom->m_bstrSymbol;
							pSpreadData->m_nContractID  = pUndAtom->m_nID;
						}
					}
					if(m_spRowData != NULL)
					{
						CMmQvRowData* pRow = dynamic_cast<CMmQvRowData*>(m_spRowData.GetInterfacePtr());
						if(pRow)
						{
							pSpreadData->m_enType       = enCtOption;

							pSpreadData->m_bsSeries     = pRow->m_pOpt[static_cast<long>(m_enOptionType)]->m_bstrSymbol;
							pSpreadData->m_nContractID  = pRow->m_pOpt[static_cast<long>(m_enOptionType)]->m_nID;
							if(pRow->m_pFut != NULL)
							{
								pSpreadData->m_nFutID		= pRow->m_pFut->m_nID ;
								pSpreadData->m_nFutRootID	= pRow->m_pFut->m_nFutRootID; 	
							}
							get_Opt(&pSpreadData->m_spOpt);
						}

					}
					if(m_spFutAtom != NULL)
					{
						CMmQvFutAtom* pFutAtom = dynamic_cast<CMmQvFutAtom*>(m_spFutAtom.GetInterfacePtr());
						if (pFutAtom != NULL)
						{
							pSpreadData->m_enType		= enCtFuture;
							pSpreadData->m_bsSeries		= pFutAtom->m_bstrSymbol;
							pSpreadData->m_nFutID		= pFutAtom->m_nID;
							pSpreadData->m_nFutRootID   = pFutAtom->m_nFutRootID;
							pSpreadData->m_nContractID	= pFutAtom->m_nID;
						}
					}
					*ppVal = spSpread.Detach();
				}
			}
		}
	}
	catch (_com_error& e)
	{
		hr = Error((PTCHAR)CComErrorWrapper::ErrorDescription(e), IID_IMmQvSpreadAtom, e.Error());
	}
	catch(...)
	{
		HRESULT _hr = HRESULT_FROM_WIN32(::GetLastError());
		if(SUCCEEDED(_hr)) _hr = E_FAIL;
		hr = Error((PTCHAR)"System Error", IID_IMmQvSpreadAtom, _hr);
	}
	return hr;
}

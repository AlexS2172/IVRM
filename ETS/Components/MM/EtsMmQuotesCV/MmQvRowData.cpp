// MmQvRowData.cpp : Implementation of CMmQvRowData

#include "stdafx.h"
#include "MmQvRowData.h"
#include "MmQvGrpAtom.h"
#include <atlcomtime.h>
const _variant_t STR_NA(L"--");
//const _variant_t STR_DIRTY(L"...");
const _variant_t STR_MISSING(L"");

// CMmQvRowData
STDMETHODIMP CMmQvRowData::putref_Opt(EtsOptionTypeEnum enType, IMmQvOptAtom* pOptAtom)
{
	HRESULT hr = S_OK;

	try
	{
		switch(enType) 
		{
		case enOtPut:
			m_spOpt[0] = pOptAtom;
			m_pOpt[0] = dynamic_cast<CComObject<CMmQvOptAtom>*>(pOptAtom);
			break;
		case enOtCall:
			m_spOpt[1] = pOptAtom;
			m_pOpt[1] = dynamic_cast<CComObject<CMmQvOptAtom>*>(pOptAtom);
			break;
		default:
			hr = E_INVALIDARG;
		}
	}
	catch (_com_error& err)
	{
		hr = Error((PTCHAR)CComErrorWrapper::ErrorDescription(err), IID_IMmQvRowData, err.Error());
	}
	/*catch(...)
	{
		DWORD dwErrorCode = GetLastError();
		hr = HRESULT_FROM_WIN32(dwErrorCode);
		if(FAILED(hr))
		{
			_bstr_t bsMessage(L"Error ");
			bsMessage += _bstr_t(dwErrorCode);

			hr = Error((LPCSTR)bsMessage, IID_IMmQvRowData, hr);
		}
		else
			hr = Error(_T("Unknown Error"), IID_IMmQvRowData, E_FAIL);
	}*/

	return hr;
}
STDMETHODIMP CMmQvRowData::get_Opt(EtsOptionTypeEnum enType,  IMmQvOptAtom** ppOptAtom)
{
	HRESULT hr = S_OK;

	try
	{
		IMmQvOptAtomPtr spOptAtom;
		
		switch(enType) 
		{
		case enOtPut:
			spOptAtom = m_spOpt[0];
			break;
		case enOtCall:
			spOptAtom = m_spOpt[1];
			break;
		default:
			hr = E_INVALIDARG;
		}
		if(SUCCEEDED(hr) && spOptAtom!=NULL && ppOptAtom!=NULL)
			*ppOptAtom  = spOptAtom.Detach();
	}
	catch (_com_error& err)
	{
		hr = Error((PTCHAR)CComErrorWrapper::ErrorDescription(err), IID_IMmQvRowData, err.Error());
	}
	/*catch(...)
	{
		DWORD dwErrorCode = GetLastError();
		hr = HRESULT_FROM_WIN32(dwErrorCode);
		if(FAILED(hr))
		{
			_bstr_t bsMessage(L"Error ");
			bsMessage += _bstr_t(dwErrorCode);

			hr = Error((LPCSTR)bsMessage, IID_IMmQvRowData, hr);
		}
		else
			hr = Error(_T("Unknown Error"), IID_IMmQvRowData, E_FAIL);
	}*/

	return hr;
}

STDMETHODIMP CMmQvRowData::putref_OptQuote(EtsOptionTypeEnum enType,  IMmQvQuoteAtom* pOptQuoteAtom)
{
	HRESULT hr = S_OK;
	ObjectLock lokc(this);

	try
	{
		switch(enType) 
		{
		case enOtPut:
			m_spOptQuote[0].Attach(pOptQuoteAtom, TRUE);
			m_pOptQuote[0] = dynamic_cast<CComObject<CMmQvQuoteAtom>*>(pOptQuoteAtom);
			break;
		case enOtCall:
			m_spOptQuote[1].Attach(pOptQuoteAtom, TRUE);
			m_pOptQuote[1] = dynamic_cast<CComObject<CMmQvQuoteAtom>*>(pOptQuoteAtom);
			break;
		default:
			hr = E_INVALIDARG;
		}
	}
	catch (_com_error& err)
	{
		hr = Error((PTCHAR)CComErrorWrapper::ErrorDescription(err), IID_IMmQvRowData, err.Error());
	}
	/*catch(...)
	{
		DWORD dwErrorCode = GetLastError();
		hr = HRESULT_FROM_WIN32(dwErrorCode);
		if(FAILED(hr))
		{
			_bstr_t bsMessage(L"Error ");
			bsMessage += _bstr_t(dwErrorCode);

			hr = Error((LPCSTR)bsMessage, IID_IMmQvRowData, hr);
		}
		else
			hr = Error(_T("Unknown Error"), IID_IMmQvRowData, E_FAIL);
	}*/

	return hr;

}
STDMETHODIMP CMmQvRowData::get_OptQuote(EtsOptionTypeEnum enType,  IMmQvQuoteAtom** ppOptQuoteAtom)
{
	HRESULT hr = S_OK;
	if(!ppOptQuoteAtom)
		return E_POINTER;
	
	try
	{
		IMmQvQuoteAtomPtr spQuoteAtom;

		switch(enType) 
		{
		case enOtPut:
			spQuoteAtom = m_spOptQuote[0];
			break;
		case enOtCall:
			spQuoteAtom = m_spOptQuote[1];
			break;
		default:
			hr = E_INVALIDARG;
		}
		if(SUCCEEDED(hr) && spQuoteAtom!=NULL && ppOptQuoteAtom!=NULL)
			*ppOptQuoteAtom  = spQuoteAtom.Detach();
	}
	catch (_com_error& err)
	{
		hr = Error((PTCHAR)CComErrorWrapper::ErrorDescription(err), IID_IMmQvRowData, err.Error());
	}
	/*catch(...)
	{
		DWORD dwErrorCode = GetLastError();
		hr = HRESULT_FROM_WIN32(dwErrorCode);
		if(FAILED(hr))
		{
			_bstr_t bsMessage(L"Error ");
			bsMessage += _bstr_t(dwErrorCode);

			hr = Error((LPCSTR)bsMessage, IID_IMmQvRowData, hr);
		}
		else
			hr = Error(_T("Unknown Error"), IID_IMmQvRowData, E_FAIL);
	}*/

	return hr;
}
bool CMmQvRowData::GetField(MmQvOptColumnEnum enCol, _variant_t& vtRet, bool bForSorting, CMmQvOptAtom* pOptAtom, CMmQvQuoteAtom* pQuote)
{
	switch(enCol)
	{
	case QOC_C_SYMBOL:
		if(pOptAtom!=NULL)
			vtRet = pOptAtom->m_bstrSymbol;
		else
			vtRet = STR_MISSING;
		break;
	case QOC_C_EXCHANGE:
		if(pQuote)
			vtRet = pQuote->GetExchangeCode();
		else
			vtRet = STR_MISSING;
		break;
	case QOC_C_SERIES:
		if(pQuote)
			vtRet = pQuote->m_bstrSeries;
		else
			vtRet = STR_MISSING;
		break;
	case QOC_C_EXPIRY:
		if(pOptAtom)
			vtRet = static_cast<DATE>(pOptAtom->m_dtExpiry);
		else
			if(bForSorting) 
				vtRet = (DATE)0.; 
			else 
				vtRet = STR_MISSING;
		break;
	case QOC_C_STRIKE:
		if(pOptAtom)
			vtRet = static_cast<DOUBLE>(pOptAtom->m_dStrike);
		else
			if(bForSorting) 
				vtRet = (DOUBLE)0.; 
			else 
				vtRet = STR_MISSING;
		break;
	case QOC_C_BID:
		{
			if(pQuote)
			{
				if(pQuote->m_dPriceBid > BAD_DOUBLE_VALUE)
					vtRet = static_cast<DOUBLE>(pQuote->m_dPriceBid);
				else
				{
					if(bForSorting) 
						vtRet = (DOUBLE)-1.;
					else
						vtRet = STR_NA;
				}
			}
			else
			{
				if(bForSorting) 
					vtRet = (DOUBLE)-1.;
				else
					vtRet = STR_MISSING;

			}
		}break;
	case QOC_C_ASK:
		{
			if(pQuote)
			{
				if(pQuote->m_dPriceAsk > BAD_DOUBLE_VALUE)
					vtRet = (DOUBLE)pQuote->m_dPriceAsk;
				else
				{
					if(bForSorting)  vtRet = (DOUBLE)-1.0; else	vtRet = STR_NA;
				}
			}
			else
			{
				if(bForSorting) 
					vtRet = (DOUBLE)-1.;
				else
					vtRet = STR_MISSING;

			}
		}break;
	case QOC_C_LAST:
		{
			if(pQuote)
			{
				if(pQuote->m_dPriceLast > BAD_DOUBLE_VALUE)
					vtRet = (DOUBLE)pQuote->m_dPriceLast;
				else
				{
					if(bForSorting) 
						vtRet = (DOUBLE)(-1.);
					else
						vtRet = STR_NA;
				}
			}
			else
			{
				if(bForSorting) 
					vtRet = (DOUBLE)(-1.);
				else
					vtRet = STR_MISSING;
			}
		}break;
	case QOC_C_VOLUME:
		{
			if(pQuote)
			{
				if(pQuote->m_nVolume > BAD_LONG_VALUE)
					vtRet = (LONG)(pQuote->m_nVolume);
				else
				{
					if(bForSorting) 
						vtRet = (LONG)(-1);
					else
						vtRet = STR_NA;
				}
			}
			else
			{
				if(bForSorting) 
					vtRet = (LONG)(-1);
				else
					vtRet = STR_MISSING;
			}
		}break;
	case QOC_C_QTY:
		{
			if(pQuote)
			{
				VARIANT_BOOL bISStockORIndex;
				m_pGrp->get_IsStockOrIndex(&bISStockORIndex);
				if(bISStockORIndex)
				{
					if(pQuote->m_spExch->ID == m_pGrp->m_pUnd->m_nOptPosExchID)
					{
						if(pOptAtom->m_nQty > BAD_LONG_VALUE)
							vtRet = static_cast<LONG>(pOptAtom->m_nQty);
						else
						{
							if(bForSorting)
								vtRet = (LONG)0L;
							else
								vtRet = STR_NA;
						}
					}
					else
					{
						if(bForSorting)
							vtRet = (LONG)0L;
						else
							vtRet = STR_NA;
					}
				}
				else
				{
					if(pQuote->m_spExch->ID == m_pGrp->m_pFut->m_nOptPosExchID)
					{
						if(pOptAtom->m_nQty > BAD_LONG_VALUE)
							vtRet = static_cast<LONG>(pOptAtom->m_nQty);
						else
						{
							if(bForSorting)
								vtRet = (LONG)0L;
							else
								vtRet = STR_NA;
						}
					}
					else
					{
						if(bForSorting)
							vtRet = (LONG)0L;
					else
							vtRet = STR_NA;
					}
				}
			}
			else
			{
				if(bForSorting) 
					vtRet = (LONG)0L;
				else
					vtRet = STR_MISSING;
			}
		}break;
	case QOC_C_VOLA:
		{
			if(pOptAtom && pOptAtom->m_dVola > BAD_DOUBLE_VALUE )
				vtRet = static_cast<DOUBLE>(pOptAtom->m_dVola*100.);
			else
				vtRet = bForSorting?_variant_t(0.):pOptAtom!=NULL?STR_NA:STR_MISSING;
		}break;
	case QOC_C_THEO_PRICE:
		{
			if(pQuote && pQuote->m_dPriceTheo > BAD_DOUBLE_VALUE )
				vtRet = static_cast<DOUBLE>(pQuote->m_dPriceTheo);
			else
				vtRet = bForSorting?_variant_t(0.):pQuote!=NULL?STR_NA:STR_MISSING;

		}break;
	case QOC_C_IV:
		{
			if(pQuote && pQuote->m_dIV > BAD_DOUBLE_VALUE )
				vtRet = static_cast<DOUBLE>(pQuote->m_dIV*100.0);
			else
				vtRet = bForSorting?_variant_t(0.):pQuote!=NULL?STR_NA:STR_MISSING;
		}break;
	case QOC_C_IV_BID:
		{
			if(pQuote && pQuote->m_dIVBid > BAD_DOUBLE_VALUE)
				vtRet = static_cast<DOUBLE>(pQuote->m_dIVBid*100.0);
			else
				vtRet = bForSorting?_variant_t(0.):pQuote!=NULL?STR_NA:STR_MISSING;
		}break;
	case QOC_C_IV_ASK:
		{
			if(pQuote && pQuote->m_dIVAsk > BAD_DOUBLE_VALUE)
				vtRet = static_cast<DOUBLE>(pQuote->m_dIVAsk*100.0);
			else
				vtRet = bForSorting?_variant_t(0.):pQuote!=NULL?STR_NA:STR_MISSING;

		}break;
	case QOC_C_OPEN_INTEREST:
		{
			if(pQuote && pQuote->m_nOpenInterest > BAD_LONG_VALUE)
				vtRet = static_cast<LONG>(pQuote->m_nOpenInterest);
			else
				vtRet = bForSorting?_variant_t(0L):pQuote!=NULL?STR_NA:STR_MISSING;
		}break;
	case QOC_C_DELTA:
		{
			if(pQuote && pQuote->m_dDelta > BAD_DOUBLE_VALUE)
				vtRet = static_cast<DOUBLE>(pQuote->m_dDelta);
			else
				vtRet = bForSorting?_variant_t(0.):pQuote!=NULL?STR_NA:STR_MISSING;
		}break;
	case QOC_C_GAMMA:
		{
			if(pQuote && pQuote->m_dGamma > BAD_DOUBLE_VALUE )
				vtRet = static_cast<DOUBLE>(pQuote->m_dGamma);
			else
				vtRet = bForSorting?_variant_t(0.):pQuote!=NULL?STR_NA:STR_MISSING;
		}break;
	case QOC_C_VEGA:
		{
			if(pQuote && pQuote->m_dVega > BAD_DOUBLE_VALUE )
				vtRet = static_cast<DOUBLE>(pQuote->m_dVega);
			else
				vtRet = bForSorting?_variant_t(0.):pQuote!=NULL?STR_NA:STR_MISSING;

		}break;
	case QOC_C_THETA:
		{
			if(pQuote && pQuote->m_dTheta > BAD_DOUBLE_VALUE)
				vtRet = static_cast<DOUBLE>(pQuote->m_dTheta);
			else
				vtRet = bForSorting?_variant_t(0.):pQuote!=NULL?STR_NA:STR_MISSING;

		}break;
	case QOC_C_RHO:
		{
			if(pQuote && pQuote->m_dRho > BAD_DOUBLE_VALUE)
				vtRet = static_cast<DOUBLE>(pQuote->m_dRho);
			else
				vtRet = bForSorting?_variant_t(0.):pQuote!=NULL?STR_NA:STR_MISSING;

		}break;
	case QOC_C_VEGA_DELTA:
		{
			if(pQuote && pQuote->m_dVegaDelta > BAD_DOUBLE_VALUE)
				vtRet = static_cast<DOUBLE>(pQuote->m_dVegaDelta);
			else
				vtRet = bForSorting?_variant_t(0.):pQuote!=NULL?STR_NA:STR_MISSING;

		}break;
	case QOC_C_VEGA_GAMMA:
		{
			if(pQuote && pQuote->m_dVegaGamma > BAD_DOUBLE_VALUE)
				vtRet = static_cast<DOUBLE>(pQuote->m_dVegaGamma);
			else
				vtRet = bForSorting?_variant_t(0.):pQuote!=NULL?STR_NA:STR_MISSING;

		}break;
	case QOC_C_THETA_DELTA:
		{
			if(pQuote && pQuote->m_dThetaDelta > BAD_DOUBLE_VALUE)
				vtRet = static_cast<DOUBLE>(pQuote->m_dThetaDelta);
			else
				vtRet = bForSorting?_variant_t(0.):pQuote!=NULL?STR_NA:STR_MISSING;

		}break;
	case QOC_C_THETA_GAMMA:
		{
			if(pQuote && pQuote->m_dThetaGamma > BAD_DOUBLE_VALUE)
				vtRet = static_cast<DOUBLE>(pQuote->m_dThetaGamma);
			else
				vtRet = bForSorting?_variant_t(0.):pQuote!=NULL?STR_NA:STR_MISSING;

		}break;
	case QOC_C_GAMMA_PER_THETA:
		{
			if(pQuote && pQuote->m_dGammaPerTheta > BAD_DOUBLE_VALUE)
				vtRet = static_cast<DOUBLE>(pQuote->m_dGammaPerTheta);
			else
				vtRet = bForSorting?_variant_t(0.):pQuote!=NULL?STR_NA:STR_MISSING;
		}break;
	case QOC_C_NET_CHANGE:
		{
			if(pQuote && pQuote->m_dNetChange > BAD_DOUBLE_VALUE)
				vtRet = static_cast<DOUBLE>(pQuote->m_dNetChange);
			else
				vtRet = bForSorting?_variant_t(0.):pQuote!=NULL?STR_NA:STR_MISSING;
		}break;
	case QOC_C_BID_SIZE:
		{
			if(pQuote && pQuote->m_nSizeBid > BAD_LONG_VALUE)
				vtRet = static_cast<LONG>(pQuote->m_nSizeBid);
			else
				vtRet = bForSorting?_variant_t(0L):pQuote!=NULL?STR_NA:STR_MISSING;

		}break;
	case QOC_C_ASK_SIZE:
		{
			if(pQuote && pQuote->m_nSizeAsk > BAD_LONG_VALUE)
				vtRet = static_cast<LONG>(pQuote->m_nSizeAsk);
			else
				vtRet = bForSorting?_variant_t(0L):pQuote!=NULL?STR_NA:STR_MISSING;
		}break;
	case QOC_C_IV_SPREAD:
		{
			if(pQuote && pQuote->m_dIVSpread > BAD_DOUBLE_VALUE )
				vtRet = static_cast<DOUBLE>(pQuote->m_dIVSpread*100.);
			else
				vtRet = bForSorting?_variant_t(0.):pQuote!=NULL?STR_NA:STR_MISSING;

		}break;
	case QOC_C_BA_EXCHANGE:
		{
			if(pQuote )
			{
				_bstr_t bsBidExchange(STR_NA);
				_bstr_t bsAskExchange(STR_NA);

				if(pQuote->m_dPriceBid > BAD_DOUBLE_VALUE && pQuote->m_sBidExchange.length())
					bsBidExchange =  pQuote->m_sBidExchange;
				if(pQuote->m_dPriceAsk > BAD_DOUBLE_VALUE && pQuote->m_sAskExchange.length())
					bsAskExchange =  pQuote->m_sAskExchange;
				bsBidExchange += _bstr_t(L"/") + bsAskExchange;
				vtRet = bsBidExchange;
			}
			else
				vtRet = STR_MISSING;

		}break;
	case QOC_C_UPDATE_TIME:
		{
			if(pQuote && pQuote->m_dtUpdateTime > 0)
				vtRet = static_cast<DATE>(pQuote->m_dtUpdateTime);
			else
				vtRet = bForSorting?_variant_t((DATE)0.):pQuote!=NULL?STR_NA:STR_MISSING;
		}break;
	case QOC_C_VOLGA:
		{
			if(pQuote && pQuote->m_dVolga > BAD_DOUBLE_VALUE )
				vtRet = static_cast<DOUBLE>(pQuote->m_dVolga * 100.0);
			else
				vtRet = bForSorting?_variant_t(0.):pQuote!=NULL?STR_NA:STR_MISSING;
		}break;
	case QOC_C_KEY:
		{
			if(pQuote && pOptAtom)
				vtRet = _bstr_t(pQuote->GetExchangeCode() + pOptAtom->m_bstrSymbol);
			else
				vtRet = STR_MISSING;
		}break;
	default:
		vtRet = vtMissing;
		break;
	}
	return utils::HasValue(vtRet);
}

bool CMmQvRowData::SetField(long lColumnId, _variant_t& vtData)
{
	switch(lColumnId)
	{
	case QOC_C_BID:
		{
			CMmQvQuoteAtom* pQuoteCall  = m_pOptQuote[enOtCall];
			if(pQuoteCall!=NULL)
			{
				double dVal    = vtData;
				double dOldVal = pQuoteCall->m_dPriceBid;
				bool bChanged  = dOldVal!=dVal;
				
				pQuoteCall->put_PriceBid(dVal);
				if(dVal >= 0. && bChanged) 
					pQuoteCall->m_dPriceBidTick  = dVal - dOldVal;

			}
		}
		break;
	case QOC_C_ASK:
		{
			CMmQvQuoteAtom* pQuoteCall  = m_pOptQuote[enOtCall];
			if(pQuoteCall!=NULL)
			{
				double dVal    = vtData;
				double dOldVal = pQuoteCall->m_dPriceAsk;
				bool bChanged  = dOldVal!=dVal;

				pQuoteCall->put_PriceAsk(dVal);
				if(dVal >= 0. && bChanged) 
					pQuoteCall->m_dPriceAskTick  = dVal - dOldVal;
			}
		}
		break;
	case QOC_C_LAST:
		{
			CMmQvQuoteAtom* pQuoteCall  = m_pOptQuote[enOtCall];
			if(pQuoteCall!=NULL)
			{
				double dVal    = vtData;
			
				pQuoteCall->put_PriceLast(dVal);
			}
		}
		break;
	case QOC_C_VOLA:
		{
			CMmQvOptAtom*   pOptCall   = m_pOpt[enOtCall];
			CMmQvQuoteAtom* pQuoteCall  = m_pOptQuote[enOtCall];
			if(pOptCall !=NULL && pQuoteCall!=NULL)
			{
				double dVola = vtData;
				dVola /=100.;
				if(pOptCall->m_dVola!=dVola)
				{
					pOptCall->m_dVola=dVola;
					pQuoteCall->SetDirty();

				}
			}
		}
		break;
	case QOC_P_BID:
		{
			CMmQvQuoteAtom* pQuotePut  = m_pOptQuote[enOtPut];
			if(pQuotePut!=NULL)
			{
				double dVal    = vtData;
				double dOldVal = pQuotePut->m_dPriceBid;
				bool bChanged  = dOldVal!=dVal;

				pQuotePut->put_PriceBid(dVal);
				if(dVal >= 0. && bChanged) 
					pQuotePut->m_dPriceBidTick  = dVal - dOldVal;

			}
		}
		break;
	case QOC_P_ASK:
		{
			CMmQvQuoteAtom* pQuotePut  = m_pOptQuote[enOtPut];
			if(pQuotePut!=NULL)
			{
				double dVal    = vtData;
				double dOldVal = pQuotePut->m_dPriceAsk;
				bool bChanged  = dOldVal!=dVal;

				pQuotePut->put_PriceAsk(dVal);
				if(dVal >= 0. && bChanged) 
					pQuotePut->m_dPriceAskTick  = dVal - dOldVal;
			}
		}
		break;
	case QOC_P_LAST:
		{
			CMmQvQuoteAtom* pQuotePut  = m_pOptQuote[enOtPut];
			if(pQuotePut!=NULL)
			{
				double dVal    = vtData;
				pQuotePut->put_PriceLast(dVal);
			}
		}
		break;
	case QOC_P_VOLA:
		{
			CMmQvOptAtom*   pOptPut   = m_pOpt[enOtPut];
			CMmQvQuoteAtom* pQuotePut = m_pOptQuote[enOtPut];
			if(pOptPut !=NULL && pQuotePut!=NULL)
			{
				double dVola = vtData;
				dVola /=100.;
				if(pOptPut->m_dVola!=dVola)
				{
					pOptPut->m_dVola=dVola;
					pQuotePut->SetDirty();

				}
			}
		}
		break;
	default:
		return false;
	}
	return true;
}
bool CMmQvRowData::GetField(MmQvOptColumnEnum enCol, _variant_t& vtRet, bool bForSorting)
{
	if(enCol < QOC_P_SYMBOL)
	{
		CMmQvOptAtom*   pOptCall   = m_pOpt[enOtCall];
		CMmQvQuoteAtom* pQuoteCall = m_pOptQuote[enOtCall];

		return GetField(enCol, vtRet, bForSorting, pOptCall, pQuoteCall);
	}
	else
		if(enCol > QOC_C_UPDATE_TIME && enCol <= QOC_P_UPDATE_TIME)
		{
			CMmQvOptAtom*   pOptPut    = m_pOpt[enOtPut];
			CMmQvQuoteAtom* pQuotePut  = m_pOptQuote[enOtPut];
			return GetField(static_cast<MmQvOptColumnEnum>(1+enCol-QOC_C_UPDATE_TIME), vtRet, bForSorting, pOptPut, pQuotePut);
		}
		else
			switch(enCol)
		{
			case QOC_ROOT:
				{
					VARIANT_BOOL bISStockORIndex;
					m_pGrp->get_IsStockOrIndex(&bISStockORIndex);
					if( bISStockORIndex )
						vtRet =  m_pOptRoot ? m_pOptRoot->m_bstrName : _bstr_t(STR_NA) ;
					else
						vtRet = m_pFut ? m_pFut->m_bstrSymbol : bstr_t(STR_NA);
				}break;
			case QOC_IS_SYNTH:
				vtRet = _variant_t(m_pOptRoot ? m_pOptRoot->m_bSynthetic : VARIANT_FALSE);
				break;
			case QOC_DPC:
				{
					VARIANT_BOOL bISStockORIndex;
					m_pGrp->get_IsStockOrIndex(&bISStockORIndex);
					if(bISStockORIndex)
						vtRet = m_pOptRoot->m_bstrDPC;
					else
					{
						WCHAR buffer[1024];
						_snwprintf_s(buffer, sizeof(buffer),L"%d %s", m_pFutRoot->m_nFutLotSize*m_pFutRoot->m_nOptLotSize, (LPCOLESTR)m_pGrp->m_pUnd->m_bstrSymbol);
						vtRet = _bstr_t(buffer);
					}
				}break;
			case QOC_SU_PRICE:
				{
					if(m_pOptRoot && m_pOptRoot->m_dSU_Price > BAD_DOUBLE_VALUE)
						vtRet = _variant_t(m_pOptRoot->m_dSU_Price);
					else
						vtRet = bForSorting?_variant_t(0.):STR_NA;
				}break;
			case QOC_DTE:
				{
					if ( m_spExp ) {
						DATE expiryDate, todayDate;

						_CHK(m_spExp->get_Expiry(&expiryDate));
						todayDate = (DATE)((LONG)COleDateTime::GetCurrentTime());
						expiryDate = (DATE)((LONG)expiryDate);
						LONG nDTE = (LONG)expiryDate - (LONG)todayDate;
						vtRet = _variant_t(nDTE);
					}
					else
						vtRet = bForSorting?_variant_t(0):STR_NA;
				}
		}
		return utils::HasValue(vtRet);
}

STDMETHODIMP CMmQvRowData::putref_Und(IMmQvUndAtom* pUnd)
{
	if(pUnd)
	{
		m_spUnd = pUnd;
		m_pUnd = dynamic_cast<CComObject<CMmQvUndAtom>*>(pUnd);
	}
	else
	{
		m_spUnd = NULL;
		m_pUnd  = NULL;
	}
	return S_OK;

}
STDMETHODIMP CMmQvRowData::putref_UndQuote(IMmQvQuoteAtom* pUndQuote)
{
	if(pUndQuote)
	{
		m_spUndQuote = pUndQuote;
		m_pUndQuote = dynamic_cast<CComObject<CMmQvQuoteAtom>*>(pUndQuote);
	}
	else
	{
		m_spUndQuote = NULL;
		m_pUndQuote  = NULL;
	}
	return S_OK;

}
STDMETHODIMP CMmQvRowData::putref_Exp(IMmQvExpAtom* pExp)
{
	if(pExp)
	{
		m_spExp = pExp;
		m_pExp = dynamic_cast<CComObject<CMmQvExpAtom>*>(pExp);
	}
	else
	{
		m_spExp = NULL;
		m_pExp  = NULL;
	}
	return S_OK;
}
STDMETHODIMP CMmQvRowData::putref_Strike(IMmQvStrikeAtom* pStrike)
{
	if(pStrike)
	{
		m_spStrike = pStrike;
		m_pStrike = dynamic_cast<CComObject<CMmQvStrikeAtom>*>(pStrike);
	}
	else
	{
		m_spStrike = NULL;
		m_pStrike  = NULL;
	}
	return S_OK;

}
STDMETHODIMP CMmQvRowData::putref_OptRoot	(IMmQvOptRootAtom* pOptRoot)
{
	if(pOptRoot)
	{
		m_spOptRoot = pOptRoot;
		m_pOptRoot = dynamic_cast<CComObject<CMmQvOptRootAtom>*>(pOptRoot);
	}
	else
	{
		m_spOptRoot = NULL;
		m_pOptRoot  = NULL;
	}
	return S_OK;
}
STDMETHODIMP CMmQvRowData::putref_Fut		(IMmQvFutAtom* pFut)
{
	if(pFut)
	{
		m_spFut = pFut;
		m_pFut = dynamic_cast<CComObject<CMmQvFutAtom>*>(pFut);
	}
	else
	{
		m_spFut = NULL;
		m_pFut  = NULL;
	}
	return S_OK;

}
STDMETHODIMP CMmQvRowData::putref_FutRoot	(IMmQvFutRootAtom* pFutRoot)
{
	if(pFutRoot)
	{
		m_spFutRoot = pFutRoot;
		m_pFutRoot = dynamic_cast<CComObject<CMmQvFutRootAtom>*>(pFutRoot);
	}
	else
	{
		m_spFutRoot = NULL;
		m_pFutRoot  = NULL;
	}
	return S_OK;
}

// MyTrackPriceInfo.cpp : Implementation of CMyTrackPriceInfo
#include "stdafx.h"
#include "MyTrackProviders.h"
#include "MyTrackPriceInfo.h"

/////////////////////////////////////////////////////////////////////////////
// CMyTrackPriceInfo
//-----------------------------------------------------------------------------------------------//
CMyTrackPriceInfo::SYMBOLSTORE CMyTrackPriceInfo::m_SymbolsCache;
CComAutoCriticalSection CMyTrackPriceInfo::m_csCache;

STDMETHODIMP CMyTrackPriceInfo::RequestLastQuote(QuoteUpdateParams *Params)
{
	ATLTRACE(_T("CMyTrackPriceInfo::RequestLastQuote\n"));
	CComVariant varParams;
	try
	{
//		ResetEvent(m_hDisconnect);

		_bstr_t bstrSymbol;
		_bstr_t bstrExchange;
		GetParams(varParams,Params);
		bstrSymbol = Params->Symbol;

		if(Params->Exchange)
			bstrExchange = Params->Exchange;

		_Module.GetMyTrackCore ()->RequestLastQuote(static_cast<CBaseNotifier*>(this),
										Params->Type,
										(LPCSTR)bstrSymbol,
										(LPCSTR)bstrExchange,
										varParams);
	}
	catch (_com_error &err)
	{
		return eg_lib::utils::ComError2ErrInfo (err,this);
	}
	varParams.Clear ();
	return S_OK;
}
//-----------------------------------------------------------------------------------------------//
STDMETHODIMP CMyTrackPriceInfo::CancelLastQuote(VARIANT Params)
{
	ATLTRACE(_T("CMyTrackPriceInfo::CancelLastQuote\n"));
	try
	{
		if (V_ERROR(&Params) == DISP_E_PARAMNOTFOUND)
		{
			_Module.GetMyTrackCore ()->CancelLastQuote (static_cast<CBaseNotifier*>(this),NULL);
			return S_OK;
		}
		else
			if (V_RECORD(&Params))
			{
				
				_bstr_t bstrSymbol;
				_QuoteUpdateParams qup(Params);
				bstrSymbol = qup->Symbol;	
				_Module.GetMyTrackCore ()->CancelLastQuote (static_cast<CBaseNotifier*>(this),(LPCSTR)bstrSymbol);
				{
					_QuoteUpdateParams params(Params);
					_bstr_t bsFullSymbol  = params->Symbol;
							bsFullSymbol +=_T("_");
							bsFullSymbol += params->Exchange;
							if(params->Type == enOPT)
								bsFullSymbol +=_T("_");

					CCriticalSectionWrapper r(m_csResponce);

					RESPONCE::iterator iter = m_Responce.begin();
					while(iter!=m_Responce.end())
					{
						if(iter->m_enType == CResponce::enLastQuote && iter->m_bsFullSymbol == bstrSymbol)
						{
							m_Responce.erase(iter);
							iter = m_Responce.begin();
						}
						iter++;
					}
				}

				return S_OK;
			}
		return DISP_E_BADVARTYPE;
	}
	catch (_com_error &err)
	{
		return eg_lib::utils::ComError2ErrInfo (err,this);
	}
	catch (...)
	{
		return DISP_E_BADVARTYPE;
	}
}
//-----------------------------------------------------------------------------------------------//
STDMETHODIMP CMyTrackPriceInfo::SubscribeQuote(QuoteUpdateParams *Params)
{
	ATLTRACE(_T("CMyTrackPriceInfo::SubscribeQuote\n"));
	CComVariant varParams;
	try
	{
//		ResetEvent(m_hDisconnect);

		_bstr_t bstrSymbol;
		_bstr_t bstrExchange;
		GetParams(varParams,Params);
		bstrSymbol = Params->Symbol;
		if(Params->Exchange)
			bstrExchange = Params->Exchange;

		_Module.GetMyTrackCore ()->SubscribeQuote(static_cast<CBaseNotifier*>(this),
									Params->Type,
									(LPCSTR)bstrSymbol,
									(LPCSTR)bstrExchange,
									varParams);
	}
	catch (_com_error &err)
	{
		return eg_lib::utils::ComError2ErrInfo (err,this);
	}
	varParams.Clear ();
	return S_OK;
}
//-----------------------------------------------------------------------------------------------//
STDMETHODIMP CMyTrackPriceInfo::UnSubscribeQuote(VARIANT Params)
{
	ATLTRACE(_T("CMyTrackPriceInfo::UnSubscribeQuote\n"));
	try
	{
		if (V_ERROR(&Params) == DISP_E_PARAMNOTFOUND)
		{
			_Module.GetMyTrackCore ()->UnsubscribeQuote(static_cast<CBaseNotifier*>(this),NULL);
			return S_OK;
		}
		else
			if (V_RECORD(&Params))
			{
				_bstr_t bstrSymbol;
				_QuoteUpdateParams qup(Params);
				bstrSymbol = qup->Symbol;
				_Module.GetMyTrackCore ()->UnsubscribeQuote(static_cast<CBaseNotifier*>(this),(LPCSTR)bstrSymbol);
				{
					_QuoteUpdateParams params(Params);
					_bstr_t bsFullSymbol  = params->Symbol;
							bsFullSymbol +=_T("_");
							bsFullSymbol += params->Exchange;
							if(params->Type == enOPT)
								bsFullSymbol +=_T("_");

					CCriticalSectionWrapper r(m_csResponce);

					RESPONCE::iterator iter = m_Responce.begin();
					while(iter!=m_Responce.end())
					{
						if(iter->m_enType == CResponce::enQuoteUpdate && iter->m_bsFullSymbol == bstrSymbol)
						{
							m_Responce.erase(iter);
							iter = m_Responce.begin();
						}
						iter++;
					}
				}
				return S_OK;
			}
			return DISP_E_BADVARTYPE;
	}
	catch (_com_error &err)
	{
		return eg_lib::utils::ComError2ErrInfo (err,this);
	}
	catch (...)
	{
		return DISP_E_BADVARTYPE;
	}
}
//-----------------------------------------------------------------------------------------------//
STDMETHODIMP CMyTrackPriceInfo::Connect()
{
	ATLTRACE(_T("CMyTrackPriceInfo::Connect\n"));
	try
	{
//		ResetEvent(m_hDisconnect);
		_Module.GetMyTrackCore ()->Connect (static_cast<CBaseNotifier*>(this)); 
	}
	catch (_com_error &err)
	{
		return eg_lib::utils::ComError2ErrInfo (err,this);
	}
	return S_OK;
}
//-----------------------------------------------------------------------------------------------//
STDMETHODIMP CMyTrackPriceInfo::Disconnect()
{
	ATLTRACE(_T("CMyTrackPriceInfo::Disconnect\n"));
//	SetEvent(m_hDisconnect);
	_Module.GetMyTrackCore ()->Disconnect (static_cast<CBaseNotifier*>(this));
	return S_OK;
}
//-----------------------------------------------------------------------------------------------//
HRESULT CMyTrackPriceInfo::OnError(ErrorNumberEnum enumError, _bstr_t bstrDescription, 
									RequestsTypeEnum enumRequest, CComVariant &varRequest)
{
//	CCriticalSectionWrapper cs(m_csEvent);
	ATLTRACE(_T("CMyTrackPriceInfo::OnError\n"));
	CComVariant varResult;
	int nConnectionIndex;
	CComVariant* pvars = new CComVariant[4];
	int nConnections = m_vec.GetSize();
	HRESULT hr = S_OK;
	
	for (nConnectionIndex = 0; nConnectionIndex < nConnections; nConnectionIndex++)
	{
		Lock();
		CComPtr<IUnknown> sp = m_vec.GetAt(nConnectionIndex);
		Unlock();
		IDispatch* pDispatch = reinterpret_cast<IDispatch*>(sp.p);
		if (pDispatch != NULL)
		{
			VariantClear(&varResult);
			pvars[3] = enumError;
			pvars[2] = (BSTR)bstrDescription;
			pvars[1] = enumRequest;
			pvars[0] = varRequest;
			DISPPARAMS disp = { pvars, NULL, 4, 0 };
			hr = pDispatch->Invoke(0x3, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &disp, &varResult, NULL, NULL);
		}
	}
	delete[] pvars;

	if(FAILED(hr)) return hr;
	return varResult.scode;
}

STDMETHODIMP CMyTrackPriceInfo::get_PriceProviderMode(PriceProviderTypeEnum *pVal)
{
	if(!pVal) return E_POINTER;

	m_enMode = _Module.GetMyTrackCore ()->GetPriceProviderMode();
	*pVal = m_enMode;
	
	return S_OK;
}

STDMETHODIMP CMyTrackPriceInfo::put_PriceProviderMode(PriceProviderTypeEnum newVal)
{
	if(newVal != m_enMode)
	{
		m_enMode = newVal;
		_Module.GetMyTrackCore ()->SetPriceProviderMode(m_enMode);
	}
	return S_OK;
}


void CMyTrackPriceInfo::LastQuote(CComVariant &varParams, CComVariant &varResults)
{
	ATLTRACE(_T("CMyTrackPriceInfo::OnLastQuote\n"));
		CComVariant varResult;
		int nConnectionIndex;
		CComVariant* pvars = new CComVariant[2];
		int nConnections = m_vec.GetSize();
		HRESULT hr = S_OK;
		
		for (nConnectionIndex = 0; nConnectionIndex < nConnections; nConnectionIndex++)
		{
			Lock();
			CComPtr<IUnknown> sp = m_vec.GetAt(nConnectionIndex);
			Unlock();
			IDispatch* pDispatch = reinterpret_cast<IDispatch*>(sp.p);
			if (pDispatch != NULL)
			{
				VariantClear(&varResult);
				pvars[1] = varParams;
				pvars[0] = varResults;
				DISPPARAMS disp = { pvars, NULL, 2, 0 };
				hr = pDispatch->Invoke(0x1, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &disp, &varResult, NULL, NULL);
			}
		}
		delete[] pvars;

}

void CMyTrackPriceInfo::QuoteUpdate(CComVariant &varParams, CComVariant &varResults)
{
	ATLTRACE(_T("CMyTrackPriceInfo::OnQuoteUpdate\n"));
		CComVariant varResult;
		int nConnectionIndex;
		CComVariant* pvars = new CComVariant[2];
		int nConnections = m_vec.GetSize();
		HRESULT hr = S_OK;
		
		for (nConnectionIndex = 0; nConnectionIndex < nConnections; nConnectionIndex++)
		{
			Lock();
			CComPtr<IUnknown> sp = m_vec.GetAt(nConnectionIndex);
			Unlock();
			IDispatch* pDispatch = reinterpret_cast<IDispatch*>(sp.p);
			if (pDispatch != NULL)
			{
				VariantClear(&varResult);
				pvars[1] = varParams;
				pvars[0] = varResults;
				DISPPARAMS disp = { pvars, NULL, 2, 0 };
				hr = pDispatch->Invoke(0x2, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &disp, &varResult, NULL, NULL);
			}
		}
		delete[] pvars;

}


HRESULT CMyTrackPriceInfo::OnLastQuote(CComVariant &varParams, CComVariant &varResults)
{
	_QuoteUpdateParams params(varParams);
	_QuoteUpdateInfo   results(varResults);

	_bstr_t bsFullSymbol  = params->Symbol;
			bsFullSymbol +=_T("_");
			bsFullSymbol += params->Exchange;
			if(params->Type == enOPT)
				bsFullSymbol +=_T("_");

	{
		CCriticalSectionWrapper wr(m_csCache);
		m_SymbolsCache[bsFullSymbol] = varResults;
	}
	CResponce resp;

	resp.m_enType = CResponce::enLastQuote;
	resp.m_bsFullSymbol = bsFullSymbol;
	resp.m_vtRequest = varParams;
	resp.m_vtResponce = varResults;

	{
		CCriticalSectionWrapper d(m_csResponce);
		m_Responce.insert(m_Responce.end(), resp);		
	}
	SetEvent(m_hResponce);

	return S_OK;
}
//-----------------------------------------------------------------------------------------------//

HRESULT CMyTrackPriceInfo::OnQuoteUpdate(CComVariant &varParams, CComVariant &varResults)
{
	_QuoteUpdateParams params(varParams);
	_QuoteUpdateInfo   results(varResults);

	_bstr_t bsFullSymbol  = params->Symbol;
			bsFullSymbol +=_T("_");
			bsFullSymbol += params->Exchange;
			if(params->Type == enOPT)
				bsFullSymbol +=_T("_");

			try
			{
				if(results->LastPrice>0.0001)
				{
					// Update Last, Min, Max prices
					CCriticalSectionWrapper wr(m_csCache);
					if(m_SymbolsCache.find(bsFullSymbol)!=m_SymbolsCache.end())
					{
		  				_QuoteUpdateInfo cache(m_SymbolsCache[bsFullSymbol]);
						CComVariant vtReq;

						cache->LastPrice = results->LastPrice;

						if(cache->HighPrice < results->HighPrice)
							cache->HighPrice = results->HighPrice;
						 
						if(cache->LowPrice <0.001 || cache->LowPrice > results->LowPrice)
							cache->LowPrice = results->LowPrice;
						
						results->HighPrice = cache->HighPrice;
						results->LowPrice = cache->LowPrice;

						cache.CopyTo(vtReq);
						m_SymbolsCache[bsFullSymbol] = vtReq;
					}
				}
				else
				{
					CCriticalSectionWrapper wr(m_csCache);
					if(m_SymbolsCache.find(bsFullSymbol)!=m_SymbolsCache.end())
					{
		  				_QuoteUpdateInfo cache(m_SymbolsCache[bsFullSymbol]);

						results->HighPrice = cache->HighPrice;
						results->LowPrice = cache->LowPrice;
						results->OpenPrice = cache->OpenPrice;
						results->ClosePrice = cache->ClosePrice;
					}
				}
			}
			catch(...){}

			CComVariant vtUpdate;
			results.CopyTo(vtUpdate);

			{
				CCriticalSectionWrapper d(m_csResponce);

				for(RESPONCE::iterator iter = m_Responce.begin(); iter!=m_Responce.end(); iter++)
				{
					if(iter->m_enType == CResponce::enQuoteUpdate && iter->m_bsFullSymbol == bsFullSymbol)
					{
						iter->m_vtResponce = vtUpdate;
						iter->m_lSkip++;
						return S_OK;
					}
				}

				CResponce resp;

				resp.m_enType = CResponce::enQuoteUpdate;
				resp.m_bsFullSymbol = bsFullSymbol;
				resp.m_vtRequest = varParams;
				resp.m_vtResponce = vtUpdate;

				m_Responce.insert(m_Responce.end(), resp);		

			}
			SetEvent(m_hResponce);

	return S_OK;
}

UINT CMyTrackPriceInfo::MsgThread(LPVOID pData)
{
	if(pData)
	{
		CoInitializeEx(NULL, COINIT_MULTITHREADED);
		CMyTrackPriceInfo* pThis = reinterpret_cast<CMyTrackPriceInfo*>(pData);

		HANDLE hEvent[] = 
		{
			pThis->m_hResponce,
			pThis->m_hStop
		};
		while(1)
		{
			if(WAIT_OBJECT_0 != WaitWithEvents(2,hEvent))
				break;

			long lSize = 1;
			while(lSize)
			{
				CResponce  resp;
				{
					CCriticalSectionWrapper d(pThis->m_csResponce);
					if(pThis->m_Responce.empty())
						break;
					resp = *pThis->m_Responce.begin();
					pThis->m_Responce.erase(pThis->m_Responce.begin());
					lSize = pThis->m_Responce.size();
				}

				if(WAIT_TIMEOUT != WaitForSingleObject(pThis->m_hStop,0))
					break;

				if(resp.m_enType == CResponce::enLastQuote)
				{
					_QuoteUpdateInfo info(resp.m_vtResponce);
					info->Skipped = 0;
					info->WaitTime = GetTickCount()-resp.m_lStartTime;
					info->TotalRequests = lSize;

					CComVariant vt;
					info.CopyTo(vt);
					pThis->LastQuote(resp.m_vtRequest, vt);
				}

				if(resp.m_enType == CResponce::enQuoteUpdate)
				{
					_QuoteUpdateInfo info(resp.m_vtResponce);
					info->Skipped = resp.m_lSkip;
					info->WaitTime = GetTickCount()-resp.m_lStartTime;
					info->TotalRequests = lSize;

					CComVariant vt;
					info.CopyTo(vt);
					pThis->QuoteUpdate(resp.m_vtRequest, vt);
				}
			}
		}

		CoUninitialize();
	}
	return 0;
}
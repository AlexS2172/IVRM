// MyTrackPriceProvider.cpp : Implementation of CMyTrackPriceProvider
#include "stdafx.h"
#include "MyTrackProviders.h"
#include "MyTrackPriceProvider.h"
//-----------------------------------------------------------------------------------------------//
STDMETHODIMP CMyTrackPriceProvider::RequestLastQuote(QuoteUpdateParams *Params)
{
	ATLTRACE(_T("CMyTrackPriceProvider::RequestLastQuote\n"));
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
										(LPCSTR) bstrSymbol,
										(LPCSTR) bstrExchange,
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
STDMETHODIMP CMyTrackPriceProvider::CancelLastQuote(VARIANT Params)
{
	ATLTRACE(_T("CMyTrackPriceProvider::CancelLastQuote\n"));
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
STDMETHODIMP CMyTrackPriceProvider::SubscribeQuote(QuoteUpdateParams *Params)
{
	ATLTRACE(_T("CMyTrackPriceProvider::SubscribeQuote\n"));
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
STDMETHODIMP CMyTrackPriceProvider::UnSubscribeQuote(VARIANT Params)
{
	ATLTRACE(_T("CMyTrackPriceProvider::UnSubscribeQuote\n"));
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
STDMETHODIMP CMyTrackPriceProvider::Connect()
{
	ATLTRACE(_T("CMyTrackPriceProvider::Connect\n"));
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
STDMETHODIMP CMyTrackPriceProvider::Disconnect()
{
	ATLTRACE(_T("CMyTrackPriceProvider::Disconnect\n"));
//	SetEvent(m_hDisconnect);
	_Module.GetMyTrackCore ()->Disconnect (static_cast<CBaseNotifier*>(this));
	return S_OK;
}
HRESULT CMyTrackPriceProvider::OnLastQuote(CComVariant &varParams, CComVariant &varResults)
{
//	CCriticalSectionWrapper cs(m_csEvent);
	ATLTRACE(_T("CMyTrackPriceProvider::OnLastQuote\n"));
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

	if(FAILED(hr)) return hr;
	return varResult.scode;
}
//-----------------------------------------------------------------------------------------------//

HRESULT CMyTrackPriceProvider::OnQuoteUpdate(CComVariant &varParams, CComVariant &varResults)
{
//	CCriticalSectionWrapper cs(m_csEvent);
	ATLTRACE(_T("CMyTrackPriceProvider::OnQuoteUpdate\n"));
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

		if(FAILED(hr)) return hr;
		return varResult.scode;
}
//-----------------------------------------------------------------------------------------------//
HRESULT CMyTrackPriceProvider::OnError(ErrorNumberEnum enumError, _bstr_t bstrDescription, 
									RequestsTypeEnum enumRequest, CComVariant &varRequest)
{
//	CCriticalSectionWrapper cs(m_csEvent);
	ATLTRACE(_T("CMyTrackPriceProvider::OnError\n"));
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
//-----------------------------------------------------------------------------------------------//
/*
HRESULT CMyTrackPriceProvider::OnLastQuote(CComVariant &varParams, CComVariant &varResults)
{
	HRESULT hr = S_OK;
	ResponceStructure buffer(ResponceStructure::enLast,varParams, varResults);
	{
		CCriticalSectionWrapper cs(m_csSend);
		m_Buffer.insert(m_Buffer.end(),buffer);
	}
	SetEvent(m_hResponce);
	return hr;
}

HRESULT CMyTrackPriceProvider::OnQuoteUpdate(CComVariant &varParams, CComVariant &varResults)
{
	HRESULT hr = S_OK;
	ResponceStructure buffer(ResponceStructure::enUpdate, varParams, varResults);
	{
		CCriticalSectionWrapper cs(m_csSend);
		m_Buffer.insert(m_Buffer.end(),buffer);
	}
	SetEvent(m_hResponce);
	return hr;
}

UINT CMyTrackPriceProvider::_RespThread(LPVOID pData)
{
	if(!pData) return 0;
    HRESULT hr = CoInitializeEx(NULL, COINIT_MULTITHREADED);
	CMyTrackPriceProvider* pThis = reinterpret_cast<CMyTrackPriceProvider*>(pData);
	bool bBreak = false;

	HANDLE hEvents[] = 
	{
		pThis->m_hStop,
		pThis->m_hDisconnect,
		pThis->m_hResponce
	};

	while(!bBreak)
	{
		switch(WaitWithEvents(3,hEvents))
		{
		case WAIT_OBJECT_0:
			pThis->m_Buffer.clear();
			bBreak = true;
			break;
		case  WAIT_OBJECT_0+1:
			pThis->m_Buffer.clear();
			ResetEvent(pThis->m_hResponce);
			ResetEvent(pThis->m_hDisconnect);
			break;
		case WAIT_OBJECT_0+2:
			{
				ResponceStructure buffer;
				{
					CCriticalSectionWrapper cs(pThis->m_csSend);
					if(pThis->m_Buffer.size())
					{
						buffer =  *(pThis->m_Buffer.begin());
						pThis->m_Buffer.erase(pThis->m_Buffer.begin());
						if(pThis->m_Buffer.size())
							SetEvent(pThis->m_hResponce);
					}
				}

				if(buffer.m_type == ResponceStructure::enLast)
				{
					pThis->Send_LastQuote(buffer.m_vtRequest, buffer.m_vtResult) ;
				}
				if(buffer.m_type == ResponceStructure::enUpdate)
				{
					pThis->Send_QuoteUpdate(buffer.m_vtRequest, buffer.m_vtResult) ;
				}
				break;
			}
		}
	}


	CoUninitialize();
	return 0;
}
*/

STDMETHODIMP CMyTrackPriceProvider::get_PriceProviderMode(PriceProviderTypeEnum *pVal)
{
	if(!pVal) return E_POINTER;

	m_enMode = _Module.GetMyTrackCore ()->GetPriceProviderMode();
	*pVal = m_enMode;
	
	return S_OK;
}

STDMETHODIMP CMyTrackPriceProvider::put_PriceProviderMode(PriceProviderTypeEnum newVal)
{
	if(newVal != m_enMode)
	{
		m_enMode = newVal;
		_Module.GetMyTrackCore ()->SetPriceProviderMode(m_enMode);
	}
	return S_OK;
}

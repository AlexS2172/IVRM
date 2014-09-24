// TrackDataPriceInfo.cpp : Implementation of CTrackDataPriceInfo
#include "stdafx.h"
#include "TrackDataProviders.h"
#include "TrackDataPriceInfo.h"

/////////////////////////////////////////////////////////////////////////////
// CTrackDataPriceInfo

STDMETHODIMP CTrackDataPriceInfo::RequestLastQuote(QuoteUpdateParams *Params)
{
	CComVariant varParams;
	try
	{
		//ResetEvent(m_hDisconnect);
		USES_CONVERSION;

		_bstr_t bstrSymbol;
		GetParams(varParams,Params);
		bstrSymbol = Params->Symbol;
		RT_SYMBOL symbol;
		ZeroMemory(&symbol, sizeof(RT_SYMBOL));
		if(Params->Symbol)
			strncpy(symbol.name,W2A(Params->Symbol),10);

		if(Params->Exchange)
			strncpy(symbol.exchange, W2A(Params->Exchange),2);

		if(Params->Type == enOPT)
		{
			symbol.u.options.series[0]='A';
			symbol.u.options.series[1]='A';
		}
		
		m_Client.RequestQuote(this, &symbol, varParams, false);
	}
	catch (_com_error &err)
	{
		return utils::ComError2ErrInfo (err,this);
	}
	varParams.Clear ();
	return S_OK;
}
STDMETHODIMP CTrackDataPriceInfo::CancelLastQuote(VARIANT Params)
{
	ATLTRACE(_T("CTrackDataPriceInfo::CancelLastQuote\n"));
	try
	{
		if (V_ERROR(&Params) == DISP_E_PARAMNOTFOUND)
		{
			m_Client.UnsubscribeQuote(this, NULL);
			return S_OK;
		}
		else
			if (V_RECORD(&Params))
			{
				_bstr_t bstrSymbol;
				_QuoteUpdateParams qup(Params);
				bstrSymbol = qup->Symbol;
				RT_SYMBOL symbol;
				ZeroMemory(&symbol, sizeof(RT_SYMBOL));

				strncpy(symbol.name,(LPCSTR)bstrSymbol, sizeof(symbol.name));
				if(qup->Exchange)
					strncpy(symbol.exchange,(LPCSTR)_bstr_t(qup->Exchange), sizeof(qup->Exchange));

				m_Client.UnsubscribeQuote(this, &symbol);
				return S_OK;
			}
			return DISP_E_BADVARTYPE;
	}
	catch (_com_error &err)
	{
		return utils::ComError2ErrInfo (err,this);
	}
	catch (...)
	{
		return DISP_E_BADVARTYPE;
	}
}
STDMETHODIMP CTrackDataPriceInfo::SubscribeQuote(QuoteUpdateParams *Params)
{
	CComVariant varParams;
	try
	{
		ATLTRACE(_T("CTrackDataPriceInfo::SubscribeQuote\n"));
		//ResetEvent(m_hDisconnect);
		USES_CONVERSION;


		_bstr_t bstrSymbol;
		GetParams(varParams,Params);
		bstrSymbol = Params->Symbol;
		RT_SYMBOL symbol;
		ZeroMemory(&symbol, sizeof(RT_SYMBOL));
		if(Params->Symbol)
			strncpy(symbol.name,W2A(Params->Symbol),10);

		if(Params->Exchange)
			strncpy(symbol.exchange, W2A(Params->Exchange),2);

		if(Params->Type == enOPT)
		{
			symbol.u.options.series[0]='A';
			symbol.u.options.series[1]='A';
		}
		
		m_Client.RequestQuote(this, &symbol, varParams, true);
	}
	catch (_com_error &err)
	{
		return utils::ComError2ErrInfo (err,this);
	}
	varParams.Clear ();
	return S_OK;
}
STDMETHODIMP CTrackDataPriceInfo::UnSubscribeQuote(VARIANT Params)
{
	ATLTRACE(_T("CTrackDataPriceInfo::UnSubscribeQuote\n"));
	try
	{
		if (V_ERROR(&Params) == DISP_E_PARAMNOTFOUND)
		{
			m_Client.UnsubscribeQuote(this, NULL);
			return S_OK;
		}
		else
			if (V_RECORD(&Params))
			{
				_bstr_t bstrSymbol;
				_QuoteUpdateParams qup(Params);
				bstrSymbol = qup->Symbol;
				RT_SYMBOL symbol;
				ZeroMemory(&symbol, sizeof(RT_SYMBOL));

				strncpy(symbol.name,(LPCSTR)bstrSymbol, sizeof(symbol.name));
				if(qup->Exchange)
					strncpy(symbol.exchange,(LPCSTR)_bstr_t(qup->Exchange), sizeof(qup->Exchange));

				m_Client.UnsubscribeQuote(this, &symbol);
				return S_OK;
			}
			return DISP_E_BADVARTYPE;
	}
	catch (_com_error &err)
	{
		return utils::ComError2ErrInfo (err,this);
	}
	catch (...)
	{
		return DISP_E_BADVARTYPE;
	}
}
STDMETHODIMP CTrackDataPriceInfo::Connect()
{
	try 
	{
		m_Client.Connect(this);
	}
	catch(_com_error& err)
	{
		return utils::ComError2ErrInfo (err,this);		
	}
	return S_OK;
}
STDMETHODIMP CTrackDataPriceInfo::Disconnect()
{
	try 
	{
		m_Client.Disconnect(this);
	}
	catch(_com_error& err)
	{
		return utils::ComError2ErrInfo (err,this);		
	}
	return S_OK;
}
 

HRESULT CTrackDataPriceInfo::OnQuoteUpdate(CComVariant &varParams, CComVariant &varResults)
{
//	CCriticalSectionWrapper cs(m_csEvent);
	ATLTRACE(_T("CTrackDataPriceInfo::OnQuoteUpdate\n"));
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
HRESULT CTrackDataPriceInfo::OnError(ErrorNumberEnum enumError, _bstr_t bstrDescription, 
									RequestsTypeEnum enumRequest, CComVariant &varRequest)
{
//	CCriticalSectionWrapper cs(m_csEvent);
	ATLTRACE(_T("CTrackDataPriceInfo::OnError\n"));
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

HRESULT CTrackDataPriceInfo::OnLastQuote(CComVariant &varParams, CComVariant &varResults)
{
//	CCriticalSectionWrapper cs(m_csEvent);
	ATLTRACE(_T("CTrackDataPriceInfo::OnLastQuote\n"));
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

// ATFPriceInfoWithNotify.cpp : Implementation of CATFPriceInfoWithNotifyWithNotify

#include "stdafx.h"
#include "ATFPriceInfoWithNotify.h"


STDMETHODIMP CATFPriceInfoWithNotify::RequestLastQuote(QuoteUpdateParams *Params)
{
	ATFPROVTRACE(__FUNCTION__, _T("Start"));
	try
	{
		_QuoteUpdateParams param;
		param.Init();

		CComVariant vt;
		param->Symbol = SysAllocString(Params->Symbol);
		param->Type = Params->Type;
		param->Exchange = SysAllocString(Params->Exchange);

		ATFPROVTRACE(__FUNCTION__, _T("Symbol %s"), (LPCTSTR)param->Symbol);

		param.CopyTo(vt);
		AddRequest(vt, _enRequestLast);
	}
	catch (_com_error &err)
	{
		ATFPROVTRACE(__FUNCTION__, _T("End"));
		return eg_lib::utils::ComError2ErrInfo (err,this);
	}
	ATFPROVTRACE(__FUNCTION__, _T("End"));
	return S_OK;
}
STDMETHODIMP CATFPriceInfoWithNotify::CancelLastQuote(VARIANT Params)
{
	ATFPROVTRACE(__FUNCTION__, _T("Start"));
	try
	{
		RemoveRequest(Params, _enRequestLast);
	}
	catch (_com_error &err)
	{
		ATFPROVTRACE(__FUNCTION__, _T("End"));
		return eg_lib::utils::ComError2ErrInfo (err,this);
	}
	catch (...)
	{
		ATFPROVTRACE(__FUNCTION__, _T("End"));
		return DISP_E_BADVARTYPE;
	}
	ATFPROVTRACE(__FUNCTION__, _T("End"));
	return S_OK;
}
STDMETHODIMP CATFPriceInfoWithNotify::SubscribeQuote(QuoteUpdateParams *Params)
{
	ATFPROVTRACE(__FUNCTION__, _T("Start"));
	try
	{
		_QuoteUpdateParams param;
		param.Init();
		CComVariant vt;
		param->Symbol = SysAllocString(Params->Symbol);
		param->Type = Params->Type;
		param->Exchange = SysAllocString(Params->Exchange);
		ATFPROVTRACE(__FUNCTION__, _T("Symbol %s"), (LPCTSTR)param->Symbol);
		param.CopyTo(vt);
		AddRequest(vt, _enRequestRealtimeNotification);
	}
	catch (_com_error &err)
	{
		ATFPROVTRACE(__FUNCTION__, _T("End"));
		return eg_lib::utils::ComError2ErrInfo (err,this);
	}
	ATFPROVTRACE(__FUNCTION__, _T("End"));
	return S_OK;
}
STDMETHODIMP CATFPriceInfoWithNotify::UnSubscribeQuote(VARIANT Params)
{
	ATFPROVTRACE(__FUNCTION__, _T("Start"));
	try
	{
		RemoveRequest(Params, _enRequestRealtime);
	}
	catch (_com_error &err)
	{
		ATFPROVTRACE(__FUNCTION__, _T("End"));
		return eg_lib::utils::ComError2ErrInfo (err,this);
	}
	catch (...)
	{
		ATFPROVTRACE(__FUNCTION__, _T("End"));
		return DISP_E_BADVARTYPE;
	}

	ATFPROVTRACE(__FUNCTION__, _T("End"));
	return S_OK;
}
STDMETHODIMP CATFPriceInfoWithNotify::Connect()
{
	ATFPROVTRACE4(__FUNCTION__, _T("Start"));
	try 
	{
		ConnectProvider();
	}
	catch(_com_error& err)
	{
		ATFPROVTRACE4(__FUNCTION__, _T("End"));
		return eg_lib::utils::ComError2ErrInfo (err,this);		
	}
	ATFPROVTRACE4(__FUNCTION__, _T("End"));
	return S_OK;
}
STDMETHODIMP CATFPriceInfoWithNotify::Disconnect()
{
	ATFPROVTRACE4(__FUNCTION__, _T("Start"));
	try 
	{
		DisconnectProvider();
	}
	catch(_com_error& err)
	{
		ATFPROVTRACE4(__FUNCTION__, _T("End"));
		return eg_lib::utils::ComError2ErrInfo (err,this);		
	}
	ATFPROVTRACE4(__FUNCTION__, _T("End"));
	return S_OK;
}
STDMETHODIMP CATFPriceInfoWithNotify::RequestLastGroupQuotes(QuoteUpdateParams * Params)
{
	ATFPROVTRACE(__FUNCTION__, _T("Start"));
	try
	{
		if(Params->Type != enGrSTK && Params->Type != enGrIDX)
			return E_INVALIDARG;

		CComVariant vt;
		_QuoteUpdateParams param;
		param.Init(Params);
		param.CopyTo(vt);
		ATFPROVTRACE(__FUNCTION__, _T("Symbol %s"), (LPCTSTR)param->Symbol);

		AddRequest(vt, _enRequestGroup, enGroupRequestLastQuote);
	}
	catch (_com_error &err)
	{
		ATFPROVTRACE(__FUNCTION__, _T("End"));
		return eg_lib::utils::ComError2ErrInfo (err,this);
	}
	ATFPROVTRACE(__FUNCTION__, _T("End"));
	return S_OK;
}

STDMETHODIMP CATFPriceInfoWithNotify::CancelLastGroupQuotes(VARIANT Params)
{
	ATFPROVTRACE(__FUNCTION__, _T("Start"));
	try
	{
		RemoveRequest(Params, _enRequestGroup);
	}
	catch (_com_error &err)
	{
		ATFPROVTRACE(__FUNCTION__, _T("End"));
		return eg_lib::utils::ComError2ErrInfo (err,this);
	}
	ATFPROVTRACE(__FUNCTION__, _T("End"));
	return S_OK;
}

STDMETHODIMP CATFPriceInfoWithNotify::SubscribeGroupQuotes(QuoteUpdateParams * Params)
{
	ATFPROVTRACE(__FUNCTION__, _T("Start"));
	try
	{
		if(Params->Type != enGrSTK && Params->Type != enGrIDX)
			return E_INVALIDARG;

		CComVariant vt;
		_QuoteUpdateParams param;
		param.Init(Params);
		param.CopyTo(vt);
		ATFPROVTRACE(__FUNCTION__, _T(" symbol %s"), (LPCTSTR)param->Symbol);

		AddRequest(vt, _enRequestGroup, enGroupRequestAllNotify);
	}
	catch (_com_error &err)
	{
		ATFPROVTRACE(__FUNCTION__, _T("End"));
		return eg_lib::utils::ComError2ErrInfo (err,this);
	}
	ATFPROVTRACE(__FUNCTION__, _T("End"));
	return S_OK;
}

STDMETHODIMP CATFPriceInfoWithNotify::UnSubscribeGroupQuotes(VARIANT Params)
{
	ATFPROVTRACE4(__FUNCTION__, _T("Start"));
	try
	{
		RemoveRequest(Params, _enRequestGroup);
	}
	catch (_com_error &err)
	{
		ATFPROVTRACE4(__FUNCTION__, _T("End"));
		return eg_lib::utils::ComError2ErrInfo (err,this);
	}
	ATFPROVTRACE4(__FUNCTION__, _T("End"));
	return S_OK;
}


 
HRESULT CATFPriceInfoWithNotify::OnSubscribed(const CComVariant &varParams)
{
	//	CCriticalSectionWrapper cs(m_csEvent);
	ATFPROVTRACE(__FUNCTION__, _T("Start"));
	CComVariant varResult;
	int nConnectionIndex;
	CComVariant* pvars = new CComVariant[1];
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
			pvars[0] = varParams;
			DISPPARAMS disp = { pvars, NULL, 1, 0 };
			hr = pDispatch->Invoke(0x4, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &disp, &varResult, NULL, NULL);
		}
	}
	delete[] pvars;

	ATFPROVTRACE(__FUNCTION__, _T("End"));
	if(FAILED(hr)) return hr;
	return varResult.scode;
}

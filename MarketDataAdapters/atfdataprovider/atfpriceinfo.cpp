// ATFPriceInfo.cpp : Implementation of CATFPriceInfo
#include "stdafx.h"
#include "ATFDataProvider.h"
#include "ATFPriceInfo.h"

/////////////////////////////////////////////////////////////////////////////
// CATFPriceInfo

STDMETHODIMP CATFPriceInfo::RequestLastQuote(QuoteUpdateParams *Params)
{
	try
	{
		_QuoteUpdateParams param;
		param.Init();

		CComVariant vt;
		param->Symbol = SysAllocString(Params->Symbol);
		param->Type = Params->Type;
		param->Exchange = SysAllocString(Params->Exchange);
		param.CopyTo(vt);
		AddRequest(vt, _enRequestLast);
	}
	catch (_com_error &err)
	{
		return eg_lib::utils::ComError2ErrInfo (err,this);
	}
	return S_OK;
}
STDMETHODIMP CATFPriceInfo::CancelLastQuote(VARIANT Params)
{
	ATFPROVTRACE(__FUNCTION__,_T("Start"));
	try
	{
		RemoveRequest(Params, _enRequestLast);
	}
	catch (_com_error &err)
	{
		return eg_lib::utils::ComError2ErrInfo (err,this);
	}
	catch (...)
	{
		return DISP_E_BADVARTYPE;
	}
	return S_OK;
}
STDMETHODIMP CATFPriceInfo::SubscribeQuote(QuoteUpdateParams *Params)
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
		param.CopyTo(vt);
		AddRequest(vt, _enRequestRealtime);
	}
	catch (_com_error &err)
	{
		return eg_lib::utils::ComError2ErrInfo (err,this);
	}
	ATFPROVTRACE(__FUNCTION__, _T("End"));
	return S_OK;
}
STDMETHODIMP CATFPriceInfo::UnSubscribeQuote(VARIANT Params)
{
	ATFPROVTRACE(__FUNCTION__, _T("Start"));
	try
	{
		RemoveRequest(Params, _enRequestRealtime);
	}
	catch (_com_error &err)
	{
		return eg_lib::utils::ComError2ErrInfo (err,this);
	}
	catch (...)
	{
		return DISP_E_BADVARTYPE;
	}

	return S_OK;
}
STDMETHODIMP CATFPriceInfo::Connect()
{
	try 
	{
		ConnectProvider();
	}
	catch(_com_error& err)
	{
		return eg_lib::utils::ComError2ErrInfo (err,this);		
	}
	return S_OK;
}
STDMETHODIMP CATFPriceInfo::Disconnect()
{
	try 
	{
		DisconnectProvider();
	}
	catch(_com_error& err)
	{
		return eg_lib::utils::ComError2ErrInfo (err,this);		
	}
	return S_OK;
}
 


 

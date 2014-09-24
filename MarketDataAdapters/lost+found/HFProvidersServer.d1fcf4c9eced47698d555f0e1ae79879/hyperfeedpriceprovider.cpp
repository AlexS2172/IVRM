// HyperFeedPriceProvider.cpp : Implementation of CHyperFeedPriceProvider
#include "stdafx.h"
#include "HyperFeedPriceProvider.h"

/////////////////////////////////////////////////////////////////////////////
// CHyperFeedPriceProvider
STDMETHODIMP CHyperFeedPriceProvider::RequestLastQuote(QuoteUpdateParams *Params)
{
	try
	{
		CRequestBasePtr pRequest = CRequestBasePtr((CRequestBase*)new CQuoteRequest(_enRequestLast, Params));
		AddRequest(pRequest);
	}
	catch (_com_error &err)
	{
		return EgLib::utils::ComError2ErrInfo (err,this);
	}
	return S_OK;
}
STDMETHODIMP CHyperFeedPriceProvider::CancelLastQuote(VARIANT Params)
{
	ATLTRACE(_T("CHyperFeedPriceProvider::CancelLastQuote\n"));
	try
	{
		if(V_VT(&Params) == VT_ERROR)
			RemoveAllRequest();
	}
	catch (_com_error &err)
	{
		return EgLib::utils::ComError2ErrInfo (err,this);
	}
	catch (...)
	{
		return DISP_E_BADVARTYPE;
	}
	return S_OK;
}
STDMETHODIMP CHyperFeedPriceProvider::SubscribeQuote(QuoteUpdateParams *Params)
{
	try
	{
		CRequestBasePtr pRequest = CRequestBasePtr((CRequestBase*)new CQuoteRequest(_enRequestRealtime, Params));
		AddRequest(pRequest);
	}
	catch (_com_error &err)
	{
		return EgLib::utils::ComError2ErrInfo (err,this);
	}
	return S_OK;
}
STDMETHODIMP CHyperFeedPriceProvider::UnSubscribeQuote(VARIANT Params)
{
	ATLTRACE(_T("CHyperFeedPriceProvider::UnSubscribeQuote\n"));
	try
	{
		CGroupPrice::Unsubscribe(Params);//The main change
									// Made by Artem Shilaev 23-03-2004
		if(V_VT(&Params) == VT_ERROR)
			RemoveAllRequest();
	}
	catch (_com_error &err)
	{
		return EgLib::utils::ComError2ErrInfo (err,this);
	}
	catch (...)
	{
		return DISP_E_BADVARTYPE;
	}
	return S_OK;
}
STDMETHODIMP CHyperFeedPriceProvider::Connect()
{
	try 
	{
		ConnectProvider();
	}
	catch(_com_error& err)
	{
		return EgLib::utils::ComError2ErrInfo (err,this);		
	}
	return S_OK;
}
STDMETHODIMP CHyperFeedPriceProvider::Disconnect()
{
	try 
	{
		UnsubscribeAll();
		DisconnectProvider();
	}
	catch(_com_error& err)
	{
		return EgLib::utils::ComError2ErrInfo (err,this);		
	}
	return S_OK;
}

STDMETHODIMP CHyperFeedPriceProvider::RequestGroup(QuoteUpdateParams * Params,  GroupRequestType enOperationType)
{
	try
	{
		if(Params->Type != enGrSTK && Params->Type != enGrIDX && Params->Type != enSTK && Params->Type != enIDX)
			return E_INVALIDARG;

		CRequestBasePtr pRequest = CRequestBasePtr((CRequestBase*)new CGroupRequest(enOperationType, Params));
		AddRequest(pRequest);
	}
	catch (_com_error &err)
	{
		return EgLib::utils::ComError2ErrInfo (err,this);
	}
	return S_OK;
}

STDMETHODIMP CHyperFeedPriceProvider::CancelGroup(VARIANT Params)
{
	try
	{
		CGroupPrice::CancelGroupPrice(Params);
		CGroupPrice::UnsubscribeGroup(Params);
		if(V_VT(&Params) == VT_ERROR)
			RemoveAllRequest();
	}
	catch (_com_error &err)
	{
		return EgLib::utils::ComError2ErrInfo (err,this);
	}
	return S_OK;
}

void CHyperFeedPriceProvider::OnResponse(CResponseBasePtr pResponse)
{
	switch(pResponse->m_enType)
	{
	case _enResponseLast:
		{
			CQuoteResultResponse* pQuoteResponse = dynamic_cast<CQuoteResultResponse*>(pResponse.get());
			if(pQuoteResponse)
				OnLastQuote(pQuoteResponse->m_vtParams, pQuoteResponse->m_vtResult);
		}
		break;
	case _enResponseRealtime:
		{
			CQuoteResultResponse* pQuoteResponse = dynamic_cast<CQuoteResultResponse*>(pResponse.get());
			if(pQuoteResponse)
				OnQuoteUpdate(pQuoteResponse->m_vtParams, pQuoteResponse->m_vtResult);
		}
		break;
	}
}

void CHyperFeedPriceProvider::OnRequest(CRequestBasePtr pRequest)
{
	switch(pRequest->m_enType)
	{
	case _enRequestLast:
		GetPrice(pRequest);
		break;
	case _enRequestRealtime:
		Subscribe(pRequest, false);
		break;
	case _enRequestGroup:
		{
			CGroupRequestPtr pGroupRequest = boost::shared_dynamic_cast<CGroupRequest>(pRequest);
			if(pGroupRequest)
			{
				if(enGroupRequestLastQuote == pGroupRequest->m_enGroupRequestType)
					GetGroupPrice(pGroupRequest->m_recParams);
				else
					SubscribeGroup(pGroupRequest->m_recParams, pGroupRequest->m_enGroupRequestType);
			}
		}
		break;
	}
}

HRESULT CHyperFeedPriceProvider::OnQuoteUpdate(const CComVariant &varParams,const  CComVariant &varResults)
{
	ATLTRACE(_T("CHyperFeedPriceProvider::OnQuoteUpdate\n"));
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

HRESULT CHyperFeedPriceProvider::OnLastQuote(const CComVariant &varParams,const CComVariant &varResults)
{
	ATLTRACE(_T("CHyperFeedPriceProvider::OnLastQuote\n"));
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
long CHyperFeedPriceProvider::Attach()
{
	long nRes = CGroupPrice::Attach();
	if(nRes != DBA_ERR_NO_ERROR)
	{
		HRESULT hr = MAKE_HRESULT (SEVERITY_ERROR, FACILITY_ITF, nRes);
		EgLib::CComErrorWrapper::ThrowError(hr, _T("Failed to attach to price fundamental database."));
	}
	return nRes ;
}
long CHyperFeedPriceProvider::Detach()
{
	long nRes = CGroupPrice::Detach();
	if(nRes != DBA_ERR_NO_ERROR)
	{
		HRESULT hr = MAKE_HRESULT (SEVERITY_ERROR, FACILITY_ITF, nRes);
		EgLib::CComErrorWrapper::ThrowError(hr, _T("Failed to detach from price fundamental database."));
	}
	return nRes ;
}

void CHyperFeedPriceProvider::PublicLastQuote(const _QuoteUpdateParams& Params, const DBA_RECORD_3& rec)
{
	_QuoteUpdateResult results;
	CopyExtendedToResult(rec, results);
	results->Exchange = Params->Exchange;
	CResponseBasePtr pResponse = CResponseBasePtr((CResponseBase*)new CQuoteResultResponse(_enResponseLast, Params, results));
	PublicResponse(pResponse);
}

void CHyperFeedPriceProvider::PublicQuoteUpdate(const _QuoteUpdateParams& Params, const DBA_RECORD_3& rec)
{
	_QuoteUpdateResult results;
	CopyExtendedToResult(rec, results);
	results->Exchange = Params->Exchange;
	CResponseBasePtr pResponse =CResponseBasePtr((CResponseBase*) new CQuoteResultResponse(_enResponseRealtime, Params, results));
	PublicResponse(pResponse);
}
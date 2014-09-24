// HyperFeedPriceInfo.cpp : Implementation of CHyperFeedPriceInfo
#include "stdafx.h"
#include "HyperFeedPriceInfo.h"

/////////////////////////////////////////////////////////////////////////////
// CHyperFeedPriceInfo

STDMETHODIMP CHyperFeedPriceInfo::RequestLastQuote(QuoteUpdateParams *Params)
{
	ATLTRACE(_T("CHyperFeedPriceInfo::RequestLastQuote\n"));
	try
	{

		CRequestBasePtr pRequest = CRequestBasePtr((CRequestBase*)new CQuoteRequest(_enRequestLast, Params));
		AddRequest(pRequest);
	}
	_CATCH_COM_EXCEPTION_RETURN_COM( L"RequestLastQuote Failed")
	_CATCH_UNHANDLED_EXCEPTION_RETURN_COM;
	return S_OK;
}
STDMETHODIMP CHyperFeedPriceInfo::CancelLastQuote(VARIANT Params)
{
	ATLTRACE(_T("CHyperFeedPriceInfo::CancelLastQuote\n"));
	try
	{
		if(V_VT(&Params) == VT_ERROR)
			RemoveAllRequest();
	}
	_CATCH_COM_EXCEPTION_RETURN_COM( L"Failed to CancelLastQuote")
	_CATCH_UNHANDLED_EXCEPTION_RETURN_COM;
	return S_OK;
}
STDMETHODIMP CHyperFeedPriceInfo::SubscribeQuote(QuoteUpdateParams *Params)
{
	ATLTRACE(_T("CHyperFeedPriceInfoWithNotify::SubscribeQuote\n"));
	try
	{
		CRequestBasePtr pRequest = CRequestBasePtr(new CQuoteRequest(_enRequestRealtime, Params));
		AddRequest(pRequest);
	}
	_CATCH_COM_EXCEPTION_RETURN_COM( L"SubscribeQuote Failed")
	_CATCH_UNHANDLED_EXCEPTION_RETURN_COM;
	return S_OK;
}
STDMETHODIMP CHyperFeedPriceInfo::UnSubscribeQuote(VARIANT Params)
{
	ATLTRACE(_T("CHyperFeedPriceInfo::UnSubscribeQuote\n"));
	try
	{
		CGroupPrice::Unsubscribe(Params);
		if(V_VT(&Params) == VT_ERROR)
			RemoveAllRequest();
	}
	_CATCH_COM_EXCEPTION_RETURN_COM( L"UnsubscribeQuote Failed")
	_CATCH_UNHANDLED_EXCEPTION_RETURN_COM;

	return S_OK;
}
STDMETHODIMP CHyperFeedPriceInfo::Connect()
{
	try 
	{
		ConnectProvider();
	}
	_CATCH_COM_EXCEPTION_RETURN_COM( L"Failed to connect to provider")
	_CATCH_UNHANDLED_EXCEPTION_RETURN_COM;
	return S_OK;
}
STDMETHODIMP CHyperFeedPriceInfo::Disconnect()
{
	try 
	{
		UnsubscribeAll();
		DisconnectProvider();
	}
	_CATCH_COM_EXCEPTION_RETURN_COM( L"Failed to disconnect from provider")
	_CATCH_UNHANDLED_EXCEPTION_RETURN_COM;
	return S_OK;
}
STDMETHODIMP CHyperFeedPriceInfo::RequestGroup(QuoteUpdateParams * Params,  GroupRequestType enOperationType)
{
	try
	{
		if(Params->Type != enGrSTK && Params->Type != enGrIDX && Params->Type != enSTK && Params->Type != enIDX)
			return E_INVALIDARG;

		CRequestBasePtr pRequest = CRequestBasePtr(new CGroupRequest(enOperationType, Params));
		AddRequest(pRequest);
	}
	_CATCH_COM_EXCEPTION_RETURN_COM( L"RequestGroup Failed")
	_CATCH_UNHANDLED_EXCEPTION_RETURN_COM;
	return S_OK;
}

STDMETHODIMP CHyperFeedPriceInfo::CancelGroup(VARIANT Params)
{
	try
	{
		CGroupPrice::CancelGroupPrice(Params);
		CGroupPrice::UnsubscribeGroup(Params);
		if(V_VT(&Params) == VT_ERROR)
			RemoveAllRequest();
	}
	_CATCH_COM_EXCEPTION_RETURN_COM( L"Failed to CancelGroup")
	_CATCH_UNHANDLED_EXCEPTION_RETURN_COM;
	return S_OK;
}

void CHyperFeedPriceInfo::OnResponse(CResponseBasePtr pResponse)
{
	try
	{
		switch(pResponse->m_enType)
		{
		case _enResponseLast:
			{
				CQuoteInfoResponsePtr pQuoteResponse = boost::shared_static_cast<CQuoteInfoResponse>(pResponse);
				if(pQuoteResponse)
					OnLastQuote(pQuoteResponse->m_vtParams, pQuoteResponse->m_vtInfo);
			}
			break;
		case _enResponseRealtime:
			{
				CQuoteInfoResponsePtr pQuoteResponse = boost::shared_static_cast<CQuoteInfoResponse>(pResponse);
				if(pQuoteResponse)
					OnQuoteUpdate(pQuoteResponse->m_vtParams, pQuoteResponse->m_vtInfo);
			}
			break;
		}
	}
	_CATCH_UNHANDLED_EXCEPTION;
}

void CHyperFeedPriceInfo::OnRequest(CRequestBasePtr pRequest)
{
	try
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
				CGroupRequestPtr pGroupRequest = boost::shared_static_cast<CGroupRequest>(pRequest);
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
	_CATCH_UNHANDLED_EXCEPTION;
}

 
HRESULT CHyperFeedPriceInfo::OnQuoteUpdate(const CComVariant &varParams, const CComVariant &varResults)
{
	ATLTRACE(_T("CHyperFeedPriceInfo::OnQuoteUpdate\n"));
	HRESULT hr = S_OK;
	CComVariant varResult;

	try
	{
		int nConnectionIndex;
		CComVariant* pvars = new CComVariant[2];
		int nConnections = m_vec.GetSize();
		
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
	_CATCH_COM_EXCEPTION_RETURN_COM( L"Failed to CancelLastQuote")
	_CATCH_UNHANDLED_EXCEPTION_RETURN_COM;
				
	if(FAILED(hr)) return hr;
	return varResult.scode;
}
HRESULT CHyperFeedPriceInfo::OnLastQuote(const CComVariant &varParams, const CComVariant &varResults)
{
	HRESULT hr = S_OK;
	CComVariant varResult;
	try
	{
		ATLTRACE(_T("CHyperFeedPriceInfo::OnLastQuote\n"));
		int nConnectionIndex;
		CComVariant* pvars = new CComVariant[2];
		int nConnections = m_vec.GetSize();
		
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
	_CATCH_COM_EXCEPTION_RETURN_COM( L"OnLastQuote failed")
	_CATCH_UNHANDLED_EXCEPTION_RETURN_COM;

	if(FAILED(hr)) return hr;
	return varResult.scode;
}
long CHyperFeedPriceInfo::Attach()
{
	long nRes;
	try
	{
		nRes = CGroupPrice::Attach();
		if(nRes != DBA_ERR_NO_ERROR)
		{
			HRESULT hr = MAKE_HRESULT (SEVERITY_ERROR, FACILITY_ITF, nRes);
			EgLib::CComErrorWrapper::ThrowError(hr, _T("Failed to attach to price fundamental database."));
		}
	}
	_CATCH_UNHANDLED_EXCEPTION;
	return nRes ;
}
long CHyperFeedPriceInfo::Detach()
{
	long nRes;
	try
	{
		nRes = CGroupPrice::Detach();
		if(nRes != DBA_ERR_NO_ERROR)
		{
			HRESULT hr = MAKE_HRESULT (SEVERITY_ERROR, FACILITY_ITF, nRes);
			EgLib::CComErrorWrapper::ThrowError(hr, _T("Failed to detach from price fundamental database."));
		}
	}
	_CATCH_UNHANDLED_EXCEPTION;
	return nRes ;
}

void CHyperFeedPriceInfo::PublicLastQuote(const _QuoteUpdateParams* pParams, const CDBARecordPtr& pData)
{
	try
	{
		_QuoteUpdateInfo info;
		CopyExtendedToInfo((*pParams)->Type, *pData, info);
		info->Exchange = SysAllocString((*pParams)->Exchange);
		PublicResponse(
			boost::shared_static_cast<CResponseBase>(
				CQuoteInfoResponsePtr(new CQuoteInfoResponse(_enResponseLast, pParams, info))));
		
		if(info->Currency)
		{
			SysFreeString(info->Currency);
			info->Currency;
		}
		if(info->Exchange)
		{
			SysFreeString(info->Exchange);
			info->Exchange;
		}
	}
_CATCH_UNHANDLED_EXCEPTION;
}

void CHyperFeedPriceInfo::PublicQuoteUpdate(const _QuoteUpdateParams* pParams, const CSubscriptionInfoPtr& pRec)
{
	try
	{
		_QuoteUpdateInfo info;
		CopyExtendedToInfo((*pParams)->Type, pRec, info);
		info->Exchange = SysAllocString((*pParams)->Exchange);
		
		PublicResponse(
			boost::shared_static_cast<CResponseBase>(
				CQuoteInfoResponsePtr(new CQuoteInfoResponse(_enResponseRealtime, pParams, info))));

		if(info->Currency)
		{
			SysFreeString(info->Currency);
			info->Currency;
		}
		if(info->Exchange)
		{
			SysFreeString(info->Exchange);
			info->Exchange;
		}
	}
_CATCH_UNHANDLED_EXCEPTION;
}
// HyperFeedBatchPriceInfo.cpp : Implementation of CHyperFeedBatchPriceInfo

#include "stdafx.h"
#include "HyperFeedBatchPriceInfo.h"

/////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CHyperFeedBatchPriceInfo::RequestLastQuote(QuoteUpdateParams *Params)
{
	EgStd::CEgTracingClass::TraceStatic(enlogDebug, __FUNCTION__, _T("Entry"));
	
	if (Params == NULL)
	{
		EgStd::CEgTracingClass::TraceStatic(enlogFaults, __FUNCTION__, _T("Exit with error - Params is NULL!!!"));
		return _ERRSET(E_POINTER, L"RequestLastQuote", L"Params is NULL!!!");
	}

	if (EgStd::g_pTrace->CheckLevel(enlogDebug-1))
	{
		EgStd::CEgTracingClass::TraceStatic(enlogDebug, __FUNCTION__, _T("Symbol: %s, Exchange: %s, Type: %d"), 
			(const TCHAR*)_bstr_t(Params->Symbol), 
			(const TCHAR*)_bstr_t(Params->Exchange), 
			Params->Type);
	}
	
	try
	{
		CRequestBasePtr pRequest = CRequestBasePtr((CRequestBase*)new CQuoteRequest(_enRequestLast, Params));
		AddRequest(pRequest);
	}
	_CATCH_COM_EXCEPTION_RETURN_COM( L"RequestLastQuote failed")
	_CATCH_UNHANDLED_EXCEPTION_RETURN_COM;
	EgStd::CEgTracingClass::TraceStatic(enlogDebug, __FUNCTION__, _T("Exit"));

	return S_OK;
}

/////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CHyperFeedBatchPriceInfo::CancelLastQuote(VARIANT Params)
{
	EgStd::CEgTracingClass::TraceStatic(enlogDebug, __FUNCTION__, _T("Entry"));

	try
	{
		if(V_VT(&Params) == VT_ERROR)
			RemoveAllRequest();
	}
	_CATCH_COM_EXCEPTION_RETURN_COM( L"CancelLastQuote failed")
	_CATCH_UNHANDLED_EXCEPTION_RETURN_COM;

	EgStd::CEgTracingClass::TraceStatic(enlogDebug, __FUNCTION__, _T("Exit"));

	return S_OK;
}

/////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CHyperFeedBatchPriceInfo::SubscribeQuote(QuoteUpdateParams *Params)
{
	EgStd::CEgTracingClass::TraceStatic(enlogDebug, __FUNCTION__, _T("Entry"));

	try
	{
		CRequestBasePtr pRequest = CRequestBasePtr((CRequestBase*)new CQuoteRequest(_enRequestRealtime, Params));
		AddRequest(pRequest);
	}
	_CATCH_COM_EXCEPTION_RETURN_COM( L"SubscribeQuote failed")
	_CATCH_UNHANDLED_EXCEPTION_RETURN_COM;

	EgStd::CEgTracingClass::TraceStatic(enlogDebug, __FUNCTION__, _T("Exit"));

	return S_OK;
}

/////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CHyperFeedBatchPriceInfo::UnSubscribeQuote(VARIANT Params)
{
	EgStd::CEgTracingClass::TraceStatic(enlogDebug, __FUNCTION__, _T("Entry"));

	try
	{
		CGroupPrice::Unsubscribe(Params);
		if(V_VT(&Params) == VT_ERROR)
		 {
			ATLTRACE(_T("CHyperFeedBatchPriceInfo::UnSubscribeQuote RemoveAllRequest\n"));
			RemoveAllRequest(); 
		 }
		
	}
	_CATCH_COM_EXCEPTION_RETURN_COM( L"UnSubscribeQuote failed")
	_CATCH_UNHANDLED_EXCEPTION_RETURN_COM;
	EgStd::CEgTracingClass::TraceStatic(enlogDebug, __FUNCTION__, _T("Exit"));

	return S_OK;
}

/////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CHyperFeedBatchPriceInfo::Connect()
{
	EgStd::CEgTracingClass::TraceStatic(enlogDebug, __FUNCTION__, _T("Entry"));

	try 
	{
		ConnectProvider();
	}
	_CATCH_COM_EXCEPTION_RETURN_COM( L"Connect failed")
	_CATCH_UNHANDLED_EXCEPTION_RETURN_COM;

	EgStd::CEgTracingClass::TraceStatic(enlogDebug, __FUNCTION__, _T("Exit"));

	return S_OK;
}

/////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CHyperFeedBatchPriceInfo::Disconnect()
{
	EgStd::CEgTracingClass::TraceStatic(enlogDebug, __FUNCTION__, _T("Entry"));

	try 
	{
		UnsubscribeAll();
		DisconnectProvider();
	}
	_CATCH_COM_EXCEPTION_RETURN_COM( L"Disconnect failed")
	_CATCH_UNHANDLED_EXCEPTION_RETURN_COM;

	EgStd::CEgTracingClass::TraceStatic(enlogDebug, __FUNCTION__, _T("Exit"));

	return S_OK;
}

/////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CHyperFeedBatchPriceInfo::get_IsQuotesUpdated(VARIANT_BOOL *pVal)
{
	EgStd::CEgTracingClass::TraceStatic(enlogDebug, __FUNCTION__, _T("Entry"));

	if(!pVal)
		return E_POINTER;

	EgLib::CAutoLock lock(m_csData);
	*pVal = m_FullInfoData.empty() ? VARIANT_FALSE : VARIANT_TRUE;

	EgStd::CEgTracingClass::TraceStatic(enlogDebug, __FUNCTION__, _T("Exit"));

	return S_OK;
}

/////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CHyperFeedBatchPriceInfo::GetQuotesUpdates(SAFEARRAY** pRetVal)
{
	EgStd::CEgTracingClass::TraceStatic(enlogDebug, __FUNCTION__, _T("Entry"));

	HRESULT hr = S_OK;

	if(!pRetVal)
	{
		ATLTRACE(_T("CHyperFeedBatchPriceInfo::GetQuotesUpdates Exit E_POINTER\n"));
		return E_POINTER;
	}
	
	try		
	{

		if(*pRetVal)
		{
			::SafeArrayDestroy(*pRetVal);
			*pRetVal = NULL;
		}

	
		EgLib::CAutoLock lock(m_csData);
		ULONG nSize = m_FullInfoData.size();

		if(nSize > 0)
		{
			CFullInfoDataCollection::iterator it = m_FullInfoData.begin();
			SAFEARRAYBOUND sab = { nSize, 0 };
			LPSAFEARRAY psa = ::SafeArrayCreateEx(VT_RECORD, 1, &sab, (PVOID)it->second.GetRecordInfo());

			if(psa)
			{
				QuoteUpdateFullInfo* pvData = NULL;
				if(SUCCEEDED(hr = ::SafeArrayAccessData(psa, (void**)&pvData)))
				{
					memset(pvData, 0, nSize * sizeof(QuoteUpdateFullInfo));
					for(ULONG i = 0; i < nSize; i++, it++)
						it->second.CopyTo(&pvData[i]);

					::SafeArrayUnaccessData(psa);

					m_FullInfoData.clear();
					*pRetVal = psa;
				}
			}
			else
			{
				hr = HRESULT_FROM_WIN32(GetLastError());
			}
		}
	}
	_CATCH_COM_EXCEPTION_RETURN_COM( L"GetQuotesUpdates failed")
	_CATCH_UNHANDLED_EXCEPTION_RETURN_COM;


	EgStd::CEgTracingClass::TraceStatic(enlogDebug, __FUNCTION__, _T("Exit"));

	return hr;
}

/////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CHyperFeedBatchPriceInfo::RequestLastGroupQuotes(QuoteUpdateParams * Params)
{
	EgStd::CEgTracingClass::TraceStatic(enlogDebug, __FUNCTION__, _T("Entry"));

	try
	{
		if(Params->Type != enGrSTK && Params->Type != enGrIDX)
			return E_INVALIDARG;

		CRequestBasePtr pRequest = CRequestBasePtr((CRequestBase*)new CGroupRequest(enGroupRequestLastQuote, Params));
		AddRequest(pRequest);
	}
	_CATCH_COM_EXCEPTION_RETURN_COM( L"RequestLastGroupQuotes failed")
	_CATCH_UNHANDLED_EXCEPTION_RETURN_COM;

	EgStd::CEgTracingClass::TraceStatic(enlogDebug, __FUNCTION__, _T("Exit"));

	return S_OK;
}

/////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CHyperFeedBatchPriceInfo::CancelLastGroupQuotes(VARIANT Params)
{
	EgStd::CEgTracingClass::TraceStatic(enlogDebug, __FUNCTION__, _T("Entry"));

	try
	{
		CGroupPrice::CancelGroupPrice(Params);
		if(V_VT(&Params) == VT_ERROR)
			RemoveAllRequest();
	}
	_CATCH_COM_EXCEPTION_RETURN_COM( L"CancelLastGroupQuotes failed")
	_CATCH_UNHANDLED_EXCEPTION_RETURN_COM;

	EgStd::CEgTracingClass::TraceStatic(enlogDebug, __FUNCTION__, _T("Exit"));

	return S_OK;
}

/////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CHyperFeedBatchPriceInfo::SubscribeGroupQuotes(QuoteUpdateParams * Params)
{
	EgStd::CEgTracingClass::TraceStatic(enlogDebug, __FUNCTION__, _T("Entry"));

	try
	{
		if(!Params || Params->Type != enGrSTK && Params->Type != enGrIDX)
			return E_INVALIDARG;

		CRequestBasePtr pRequest = CRequestBasePtr((CRequestBase*)new CGroupRequest(enGroupRequestAllNotify, Params));
		AddRequest(pRequest);
	}
	_CATCH_COM_EXCEPTION_RETURN_COM( L"SubscribeGroupQuotes")
	_CATCH_UNHANDLED_EXCEPTION_RETURN_COM;

	EgStd::CEgTracingClass::TraceStatic(enlogDebug, __FUNCTION__, _T("Exit"));

	return S_OK;
}

/////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CHyperFeedBatchPriceInfo::UnSubscribeGroupQuotes(VARIANT Params)
{
	EgStd::CEgTracingClass::TraceStatic(enlogDebug, __FUNCTION__, _T("Entry"));

	try
	{
		CGroupPrice::UnsubscribeGroup(Params);
		if(V_VT(&Params) == VT_ERROR)
			RemoveAllRequest();
	}
	_CATCH_COM_EXCEPTION_RETURN_COM( L"UnSubscribeGroupQuotes")
	_CATCH_UNHANDLED_EXCEPTION_RETURN_COM;

	EgStd::CEgTracingClass::TraceStatic(enlogDebug, __FUNCTION__, _T("Exit"));

	return S_OK;
}

/////////////////////////////////////////////////////////////////////////////
//
void CHyperFeedBatchPriceInfo::OnResponse(CResponseBasePtr pResponse)
{
	EgStd::CEgTracingClass::TraceStatic(enlogDebug, __FUNCTION__, _T("Entry"));

	try
	{
		switch(pResponse->m_enType)
		{
		case _enResponseLast:
			{
				EgStd::CEgTracingClass::TraceStatic(enlogDebug, __FUNCTION__, _T("Response type is _enResponseLast"));
				
				CQuoteInfoResponse* pQuoteResponse = dynamic_cast<CQuoteInfoResponse*>(pResponse.get());
				if(pQuoteResponse)
					OnLastQuote(pQuoteResponse->m_vtParams, pQuoteResponse->m_vtInfo);
			}
			break;

		case _enResponseRealtime:
			ATLASSERT(FALSE);
			break;

		case _enResponseRealtimeNotification:
			{
				EgStd::CEgTracingClass::TraceStatic(enlogDebug, __FUNCTION__, _T("Response type is _enResponseRealtimeNotification"));
				CSubscribedResponse* pSubsResponse = dynamic_cast<CSubscribedResponse*>(pResponse.get());
				if(pSubsResponse)
					OnSubscribed(pSubsResponse->m_vtParams);
			}
			break;
		}
	}
	_CATCH_UNHANDLED_EXCEPTION;	

	EgStd::CEgTracingClass::TraceStatic(enlogDebug, __FUNCTION__, _T("Exit"));
}

/////////////////////////////////////////////////////////////////////////////
//
void CHyperFeedBatchPriceInfo::OnRequest(CRequestBasePtr pRequest)
{
	EgStd::CEgTracingClass::TraceStatic(enlogDebug, __FUNCTION__, _T("Entry"));

	switch(pRequest->m_enType)
	{
	case _enRequestLast:
		EgStd::CEgTracingClass::TraceStatic(enlogDebug, __FUNCTION__, _T("Request type is _enRequestLast"));
		GetPrice(pRequest);
		break;
	case _enRequestRealtime:
		EgStd::CEgTracingClass::TraceStatic(enlogDebug, __FUNCTION__, _T("Request type is _enRequestRealtime"));
		Subscribe(pRequest, true);
		break;
	case _enRequestGroup:
	{
		EgStd::CEgTracingClass::TraceStatic(enlogDebug, __FUNCTION__, _T("Request type is _enRequestGroup"));
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

	EgStd::CEgTracingClass::TraceStatic(enlogDebug, __FUNCTION__, _T("Exit"));
}

/////////////////////////////////////////////////////////////////////////////
//
HRESULT CHyperFeedBatchPriceInfo::OnLastQuote(const CComVariant &varParams, const CComVariant &varResults)
{
	EgStd::CEgTracingClass::TraceStatic(enlogDebug, __FUNCTION__, _T("Entry"));
	
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

	if(FAILED(hr)) 
	{
		EgStd::CEgTracingClass::TraceStatic(enlogFaults, __FUNCTION__, _T("Exit with error hr = %d"), hr);
		return hr;
	}
		
	EgStd::CEgTracingClass::TraceStatic(enlogDebug, __FUNCTION__, _T("Exit"));
	
	return varResult.scode;
}

/////////////////////////////////////////////////////////////////////////////
//
HRESULT CHyperFeedBatchPriceInfo::OnQuoteUpdate()
{
	EgStd::CEgTracingClass::TraceStatic(enlogDebug, __FUNCTION__, _T("Entry"));

	CComVariant varResult;
	int nConnectionIndex;
	int nConnections = m_vec.GetSize();
	HRESULT hr = S_OK;

	for(nConnectionIndex = 0; nConnectionIndex < nConnections; nConnectionIndex++)
	{
		Lock();
		CComPtr<IUnknown> sp = m_vec.GetAt(nConnectionIndex);
		Unlock();
		IDispatch* pDispatch = reinterpret_cast<IDispatch*>(sp.p);
		if(pDispatch != NULL)
		{
			VariantClear(&varResult);
			DISPPARAMS disp = { NULL, NULL, 0, 0 };
			hr = pDispatch->Invoke(0x2, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &disp, &varResult, NULL, NULL);
		}
	}

	if(FAILED(hr)) 
	{
		EgStd::CEgTracingClass::TraceStatic(enlogFaults, __FUNCTION__, _T("Exit with error hr = %d"), hr);
		return hr;
	}

	EgStd::CEgTracingClass::TraceStatic(enlogDebug, __FUNCTION__, _T("Exit"));

	return varResult.scode;
}

/////////////////////////////////////////////////////////////////////////////
//
HRESULT CHyperFeedBatchPriceInfo::OnSubscribed(const CComVariant &varParams)
{
	EgStd::CEgTracingClass::TraceStatic(enlogDebug, __FUNCTION__, _T("Entry"));

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

	if(FAILED(hr)) 
	{
		EgStd::CEgTracingClass::TraceStatic(enlogFaults, __FUNCTION__, _T("Exit with error hr = "), hr);
		return hr;
	}
	
	EgStd::CEgTracingClass::TraceStatic(enlogDebug, __FUNCTION__, _T("Exit"));

	return varResult.scode;
}

/////////////////////////////////////////////////////////////////////////////
//
long CHyperFeedBatchPriceInfo::Attach()
{
	EgStd::CEgTracingClass::TraceStatic(enlogDebug, __FUNCTION__, _T("Entry"));

	long nRes = CGroupPrice::Attach();
	if(nRes != DBA_ERR_NO_ERROR)
	{
		HRESULT hr = MAKE_HRESULT (SEVERITY_ERROR, FACILITY_ITF, nRes);
		EgLib::CComErrorWrapper::ThrowError(hr, _T("Failed to attach to price fundamental database."));
	}

	EgStd::CEgTracingClass::TraceStatic(enlogDebug, __FUNCTION__, _T("Exit"));

	return nRes;
}

/////////////////////////////////////////////////////////////////////////////
//
long CHyperFeedBatchPriceInfo::Detach()
{
	EgStd::CEgTracingClass::TraceStatic(enlogDebug, __FUNCTION__, _T("Entry"));

	long nRes = CGroupPrice::Detach();
	if(nRes != DBA_ERR_NO_ERROR)
	{
		HRESULT hr = MAKE_HRESULT (SEVERITY_ERROR, FACILITY_ITF, nRes);
		EgLib::CComErrorWrapper::ThrowError(hr, _T("Failed to detach from price fundamental database."));
	}

	EgStd::CEgTracingClass::TraceStatic(enlogDebug, __FUNCTION__, _T("Exit"));

	return nRes;
}

/////////////////////////////////////////////////////////////////////////////
//
void CHyperFeedBatchPriceInfo::PublicLastQuote(const _QuoteUpdateParams& Params, const DBA_RECORD_3& rec)
{
	EgStd::CEgTracingClass::TraceStatic(enlogDebug, __FUNCTION__, _T("Entry"));

	_QuoteUpdateInfo info;
	CopyExtendedToInfo(rec, info);
	info->Exchange = SysAllocString(Params->Exchange);
	CResponseBasePtr pResponse = CResponseBasePtr((CResponseBase*)new CQuoteInfoResponse(_enResponseLast, Params, info));
	PublicResponse(pResponse);

	if(info->Currency)
		SysFreeString(info->Currency);

	if(info->Exchange)
		SysFreeString(info->Exchange);

	EgStd::CEgTracingClass::TraceStatic(enlogDebug, __FUNCTION__, _T("Exit"));
}

/////////////////////////////////////////////////////////////////////////////
//
void CHyperFeedBatchPriceInfo::PublicQuoteUpdate(const _QuoteUpdateParams& Params, const DBA_RECORD_3& rec)
{
	EgStd::CEgTracingClass::TraceStatic(enlogDebug, __FUNCTION__, _T("Entry"));

	_QuoteUpdateFullInfo fullinfo;
	fullinfo.Init();

	fullinfo->Params.Type = Params->Type;
	fullinfo->Params.Symbol = ::SysAllocString(Params->Symbol);

	CopyExtendedToInfo(rec, &fullinfo->Info);

	if(Params->Exchange)
	{
		fullinfo->Params.Exchange = ::SysAllocString(Params->Exchange);
		fullinfo->Info.Exchange = ::SysAllocString(Params->Exchange);
	}

	{
		EgLib::CAutoLock lock(m_csData);
		m_FullInfoData[_GetKey(Params)] = fullinfo;
	}

	OnQuoteUpdate();

	EgStd::CEgTracingClass::TraceStatic(enlogDebug, __FUNCTION__, _T("Exit"));
}

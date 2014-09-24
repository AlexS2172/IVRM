// HyperFeedBatchPriceInfo.cpp : Implementation of CHyperFeedBatchPriceInfo

#include "stdafx.h"
#include "HyperFeedBatchPriceInfo.h"
#include <ATLComTime.h>

/////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CHyperFeedBatchPriceInfo::put_UserName(BSTR userName)
{
	_bstr_t bsUserName(userName);
	m_strUserName = (LPCSTR)bsUserName;
	return S_OK;
}

STDMETHODIMP CHyperFeedBatchPriceInfo::RequestLastQuote(QuoteUpdateParams *Params)
{
	EgStd::CEgTracingClass::TraceStatic(enlogDebug, __FUNCTION__, _T("[%s]\t Entry"), m_strUserName.c_str());
	
	if (Params == NULL)
	{
		EgStd::CEgTracingClass::TraceStatic(enlogFaults, __FUNCTION__, _T("[%s]\t Exit with error - Params is NULL!!!"), m_strUserName.c_str());
		return _ERRSET(E_POINTER, L"RequestLastQuote", L"Params is NULL!!!");
	}

		EgStd::CEgTracingClass::TraceStatic(enlogInfo, __FUNCTION__, _T("[%s]\t Symbol: %s, Exchange: %s, Type: %d"), m_strUserName.c_str() , 
			Params->Symbol?CW2T(Params->Symbol):_T(""), 
			Params->Exchange?CW2T(Params->Exchange):_T(""), 
			Params->Type);
	
	try
	{
		CRequestBasePtr pRequest = CRequestBasePtr((CRequestBase*)new CQuoteRequest(_enRequestLast, Params));
		AddRequest(pRequest);
	}
	_CATCH_COM_EXCEPTION_RETURN_COM( L"RequestLastQuote failed")
	_CATCH_UNHANDLED_EXCEPTION_RETURN_COM;
	EgStd::CEgTracingClass::TraceStatic(enlogDebug, __FUNCTION__, _T("[%s]\t Exit"), m_strUserName.c_str());

	return S_OK;
}

/////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CHyperFeedBatchPriceInfo::CancelLastQuote(VARIANT Params)
{
	EgStd::CEgTracingClass::TraceStatic(enlogDebug, __FUNCTION__, _T("[%s]\t Entry"), m_strUserName.c_str());

	try
	{
		EgStd::CEgTracingClass::TraceStatic(enlogInfo, __FUNCTION__, _T("[%s]\t "), m_strUserName.c_str());
		if(V_VT(&Params) == VT_ERROR)
			RemoveAllRequest();
	}
	_CATCH_COM_EXCEPTION_RETURN_COM( L"CancelLastQuote failed")
	_CATCH_UNHANDLED_EXCEPTION_RETURN_COM;

	EgStd::CEgTracingClass::TraceStatic(enlogDebug, __FUNCTION__, _T("[%s]\t Exit"), m_strUserName.c_str());

	return S_OK;
}

/////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CHyperFeedBatchPriceInfo::SubscribeQuote(QuoteUpdateParams *Params)
{
	EgStd::CEgTracingClass::TraceStatic(enlogDebug, __FUNCTION__, _T("[%s]\t Entry"), m_strUserName.c_str());

	try
	{
		CRequestBasePtr pRequest = CRequestBasePtr((CRequestBase*)new CQuoteRequest(_enRequestRealtime, Params));
		EgStd::CEgTracingClass::TraceStatic(enlogInfo, __FUNCTION__, _T("[%s]\t Symbol: %s, Exchange: %s, Type: %d"), m_strUserName.c_str(), 
			Params->Symbol?CW2T(Params->Symbol):_T(""), 
			Params->Exchange?CW2T(Params->Exchange):_T(""), 
			Params->Type);

		AddRequest(pRequest);
	}
	_CATCH_COM_EXCEPTION_RETURN_COM( L"SubscribeQuote failed")
	_CATCH_UNHANDLED_EXCEPTION_RETURN_COM;

	EgStd::CEgTracingClass::TraceStatic(enlogDebug, __FUNCTION__, _T("[%s]\t Exit"), m_strUserName.c_str());

	return S_OK;
}

/////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CHyperFeedBatchPriceInfo::UnSubscribeQuote(VARIANT Params)
{
	EgStd::CEgTracingClass::TraceStatic(enlogDebug, __FUNCTION__, _T("[%s]\t Entry"), m_strUserName.c_str());

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
	EgStd::CEgTracingClass::TraceStatic(enlogDebug, __FUNCTION__, _T("[%s]\t Exit"), m_strUserName.c_str());

	return S_OK;
}

/////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CHyperFeedBatchPriceInfo::Connect()
{
	EgStd::CEgTracingClass::TraceStatic(enlogDebug, __FUNCTION__, _T("[%s]\t Entry"), m_strUserName.c_str());

	try 
	{
		ConnectProvider();
	}
	_CATCH_COM_EXCEPTION_RETURN_COM( L"Connect failed")
	_CATCH_UNHANDLED_EXCEPTION_RETURN_COM;

	EgStd::CEgTracingClass::TraceStatic(enlogDebug, __FUNCTION__, _T("[%s]\t Exit"), m_strUserName.c_str());

	return S_OK;
}

/////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CHyperFeedBatchPriceInfo::Disconnect()
{
	EgStd::CEgTracingClass::TraceStatic(enlogDebug, __FUNCTION__, _T("[%s]\t Entry"), m_strUserName.c_str());

	try 
	{
		UnsubscribeAll();
		DisconnectProvider();
	}
	_CATCH_COM_EXCEPTION_RETURN_COM( L"Disconnect failed")
	_CATCH_UNHANDLED_EXCEPTION_RETURN_COM;

	EgStd::CEgTracingClass::TraceStatic(enlogDebug, __FUNCTION__, _T("[%s]\t Exit"), m_strUserName.c_str());

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
		return E_POINTER;

	if(!pRetVal)
	{
		ATLTRACE(_T("CHyperFeedBatchPriceInfo::GetQuotesUpdates Exit E_POINTER\n"));
		return E_POINTER;
	}
	try		
	{
		std::vector<CSubscriptionInfoPtr> updates;
		if(*pRetVal)
		{
			::SafeArrayDestroy(*pRetVal);
			*pRetVal = NULL;
		}

		{
			EgLib::CAutoLock lock(m_csData);
			if(!m_FullInfoData.empty())
			{
				updates.reserve(m_FullInfoData.size());

				for(CFullInfoDataCollection::iterator itr = m_FullInfoData.begin(); itr!=m_FullInfoData.end(); ++itr)
					updates.insert(updates.end(), *itr);
				m_FullInfoData.clear();
			}
		}

		UINT nSize = updates.size();

		_QuoteUpdateFullInfo fullinfoInit;
		fullinfoInit.Init();

		if(nSize > 0)
		{
			std::vector<CSubscriptionInfoPtr>::iterator it    = updates.begin();

			SAFEARRAYBOUND sab = { nSize, 0 };
			LPSAFEARRAY psa = ::SafeArrayCreateEx(VT_RECORD, 1, &sab, (PVOID)fullinfoInit.GetRecordInfo());
			if(psa)
			{
				QuoteUpdateFullInfo* pvData = NULL;
				if(SUCCEEDED(hr = ::SafeArrayAccessData(psa, (void**)&pvData)))
				{
					memset(pvData, 0, nSize * sizeof(QuoteUpdateFullInfo));
					for(ULONG i = 0; i < nSize; i++, it++)
					{
						AddClientUpdate();
						CSubscriptionInfoPtr pData = *it;
						_QuoteUpdateFullInfo fullinfo;
						fullinfo.Init();
						if(pData!=NULL)
						{
							fullinfo->Params.Type = pData->GetType();
							{
								CAutoLock lc(pData->GetSubscriptionLock());
								if(pData->GetData())
								{
									DBA_KEYS* pDbaKey = (DBA_KEYS*)&pData->GetData()->item;	 
									ParamFromKey(pDbaKey->dbaKey, &fullinfo->Params);
									pData->GetData()->close.price = pData->GetClosePrice();
								}
							}
							CopyExtendedToInfo(pData->GetType(), pData, &fullinfo->Info);
						}
						fullinfo.CopyTo(&pvData[i]);
					}

					::SafeArrayUnaccessData(psa);
					*pRetVal = psa;
				}
			}
			else
			{
				hr = HRESULT_FROM_WIN32(GetLastError());
			}
		}
		{
			CAutoLock Lock(m_csUpdate);
			m_bUpdate = false;
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
		EgStd::CEgTracingClass::TraceStatic(enlogInfo, __FUNCTION__, _T("[%s]\t Symbol: %s, Exchange: %s, Type: %d"), m_strUserName.c_str(), 
			Params->Symbol?CW2T(Params->Symbol):_T(""), 
			Params->Exchange?CW2T(Params->Exchange):_T(""), 
			Params->Type);

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
		EgStd::CEgTracingClass::TraceStatic(enlogInfo, __FUNCTION__, _T("[%s]\t Symbol: %s, Exchange: %s, Type: %d"), m_strUserName.c_str(), 
			Params->Symbol?CW2T(Params->Symbol):_T(""), 
			Params->Exchange?CW2T(Params->Exchange):_T(""), 
			Params->Type);
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
				EgStd::CEgTracingClass::TraceStatic(enlogDebug, __FUNCTION__, _T("[%s]\t Response type is _enResponseLast"), m_strUserName.c_str());
				
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
				EgStd::CEgTracingClass::TraceStatic(enlogDebug, __FUNCTION__, _T("[%s]\t Response type is _enResponseRealtimeNotification"), m_strUserName.c_str());
				CSubscribedResponse* pSubsResponse = dynamic_cast<CSubscribedResponse*>(pResponse.get());
				if(pSubsResponse)
				{
					OnSubscribed(pSubsResponse->m_vtParams);
					CAutoLock Lock(m_csUpdate);
					m_bUpdate = true;
				}
			}
			break;
		}
		if(m_pPerfMon)
		{
//			LARGE_INTEGER li;
//			QueryPerformanceCounter(&li);
//			m_pPerfMon->AveStaticTimer			 = (li.QuadPart - pResponse->GetPerformanceCounter().QuadPart) / CHFServer::m_lPerformanceFrequeny.QuadPart;
//			m_pPerfMon->StaticUpdatesQueueLength = GetResponseQueueLen();
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
		EgStd::CEgTracingClass::TraceStatic(enlogDebug, __FUNCTION__, _T("[%s]\t Request type is _enRequestLast"), m_strUserName.c_str());
		GetPrice(pRequest);
		break;
	case _enRequestRealtime:
		EgStd::CEgTracingClass::TraceStatic(enlogDebug, __FUNCTION__, _T("[%s]\t Request type is _enRequestRealtime"), m_strUserName.c_str());
		Subscribe(pRequest, true);
		break;
	case _enRequestGroup:
	{
		EgStd::CEgTracingClass::TraceStatic(enlogDebug, __FUNCTION__, _T("[%s]\t Request type is _enRequestGroup"), m_strUserName.c_str());
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
				AddClientUpdate();
			}
	}
	delete[] pvars;

	if(FAILED(hr)) 
	{
		EgStd::CEgTracingClass::TraceStatic(enlogFaults, __FUNCTION__, _T("[%s]\t Exit with error hr = %d"), m_strUserName.c_str(), hr);
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
			AddClientUpdate();
		}
	}

	if(FAILED(hr)) 
	{
		EgStd::CEgTracingClass::TraceStatic(enlogFaults, __FUNCTION__, _T("[%s]\t Exit with error hr = %d"), m_strUserName.c_str(), hr);
		return hr;
	}

	m_dtLastUpdate = vt_date::GetCurrentDate();
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
			AddClientUpdate();
		}
	}
	delete[] pvars;

	if(FAILED(hr)) 
	{
		EgStd::CEgTracingClass::TraceStatic(enlogFaults, __FUNCTION__, _T("[%s]\t Exit with error hr = "), m_strUserName.c_str(), hr);
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

	EgLib::CThread::Start();
	long nRes = CGroupPrice::Attach();
	if(nRes != DBA_ERR_NO_ERROR)
	{
		HRESULT hr = MAKE_HRESULT (SEVERITY_ERROR, FACILITY_ITF, nRes);
		EgLib::CComErrorWrapper::ThrowError(hr, _T("Failed to attach to price fundamental database."));
	}
	try
	{
		if(m_pPerfMon==NULL)
			__CHECK_HRESULT3(m_pPerfMon.CreateInstance(__uuidof(PerfMon::CPPBatchInfo)));
		__CHECK_HRESULT3(m_pPerfMon->Initialize(_bstr_t(m_strUserName.c_str())));
		m_pPerfMon->Clients = 1;
	}
	catch (_com_error& e)
	{
		EgStd::CEgTracingClass::TraceStatic(enlogWarning, __FUNCTION__, _T("[%s]\t Unable to create Performance object %i: %s"), m_strUserName.c_str(), e.Error(), e.Description().length()?(LPCSTR)e.Description():e.ErrorMessage());
	}
	EgStd::CEgTracingClass::TraceStatic(enlogDebug, __FUNCTION__, _T("Exit"));
	return nRes;
}

/////////////////////////////////////////////////////////////////////////////
//
long CHyperFeedBatchPriceInfo::Detach()
{
	EgStd::CEgTracingClass::TraceStatic(enlogDebug, __FUNCTION__, _T("Entry"));
	EgLib::CThread::Stop();

	long nRes = CGroupPrice::Detach();
	if(nRes != DBA_ERR_NO_ERROR)
	{
		HRESULT hr = MAKE_HRESULT (SEVERITY_ERROR, FACILITY_ITF, nRes);
		EgLib::CComErrorWrapper::ThrowError(hr, _T("Failed to detach from price fundamental database."));
	}
	if(m_pPerfMon)
	{
		m_pPerfMon->Clients = 0;
		m_pPerfMon = NULL;
	}

	EgStd::CEgTracingClass::TraceStatic(enlogDebug, __FUNCTION__, _T("Exit"));

	return nRes;
}

/////////////////////////////////////////////////////////////////////////////
//
void CHyperFeedBatchPriceInfo::PublicLastQuote(const _QuoteUpdateParams* pParams, const CDBARecordPtr& pData)
{
	EgStd::CEgTracingClass::TraceStatic(enlogDebug, __FUNCTION__, _T("Entry"));

	_QuoteUpdateInfo info;
	CopyExtendedToInfo((*pParams)->Type, *pData.get(), info);
	if((*pParams)->Exchange)
		info->Exchange = SysAllocString((*pParams)->Exchange);
	CResponseBasePtr pResponse = CResponseBasePtr((CResponseBase*)new CQuoteInfoResponse(_enResponseLast, pParams, info));
	PublicResponse(pResponse);
	EgStd::CEgTracingClass::TraceStatic(enlogDebug, __FUNCTION__, _T("Exit"));
}

/////////////////////////////////////////////////////////////////////////////
//
void CHyperFeedBatchPriceInfo::PublicQuoteUpdate(const _QuoteUpdateParams* pParams, const CSubscriptionInfoPtr& pData)
{
	EgStd::CEgTracingClass::TraceStatic(enlogDebug, __FUNCTION__, _T("Entry"));
	bool bUpdate = false;

	if(pData!=NULL)
	{
		CAutoLock lk(m_csData);
		m_FullInfoData.insert(pData);
		SetEvent(m_hBatchDataReady);
	}
	EgStd::CEgTracingClass::TraceStatic(enlogDebug, __FUNCTION__, _T("Exit"));
}

UINT CHyperFeedBatchPriceInfo::Run()
{
	::CoInitializeEx(NULL,COINIT_MULTITHREADED);
	DWORD dwTicks   = GetTickCount();
	DWORD dwTimeout = INFINITE;
	bool bContinue = true;

	while(bContinue)
	{
		HANDLE hEvents[] = 
		{
			m_hBatchDataReady,
			EgLib::CThread::GetStopEventHandle()

		};

		DWORD dwResult =WaitForMultipleObjects(2, hEvents, FALSE, INFINITE);
		switch(dwResult)
		{
		case WAIT_OBJECT_0:
			{
				DWORD dwCurrent = GetTickCount();
				DWORD dwElapsed = dwCurrent-dwTicks;
				if( dwElapsed >= _Module.GetBatchNotificationDelay())
				{
					OnQuoteUpdate();
					dwTicks = GetTickCount();
				}
				else
				{
					dwTimeout = _Module.GetBatchNotificationDelay()- dwElapsed;
					if(WAIT_TIMEOUT == WaitForSingleObject(EgLib::CThread::GetStopEventHandle(), dwTimeout))
					{
						OnQuoteUpdate();
						dwTicks = GetTickCount();
					}
					else
						bContinue = false;
				}
			}break;
		default:
			bContinue = false;
			break;
		}
	}
	CoUninitialize();
	return 0;
}

void CHyperFeedBatchPriceInfo::OnAddResponse()
{
//	if(m_spGlobalPerfCounter)
//		m_spGlobalPerfCounter->IncrementStaticUpdates();
//	if(m_pPerfMon)
//		m_pPerfMon->IncrementStaticUpdates();
}


void CHyperFeedBatchPriceInfo::ProcessMessage(CSubscriptionInfoPtr& pExtRec)
{
	if( pExtRec == NULL || pExtRec->GetData()==NULL )
		return;

	try
	{
		EgStd::CEgTracingClass::TraceStatic(enlogDebug, __FUNCTION__ , _T("ProccessMessages Enter %s"), pExtRec->GetKey().c_str());
		PublicQuoteUpdate(NULL, pExtRec);
		if(m_pPerfMon)
			m_pPerfMon->AddProcessedUpdate();
		if(m_spGlobalPerfCounter)
			m_spGlobalPerfCounter->AddProcessedUpdate();

	}
	_CATCH_UNHANDLED_EXCEPTION;
	EgStd::CEgTracingClass::TraceStatic(enlogDebug, __FUNCTION__ , _T("ProccessMessages Exit %s"),pExtRec->GetKey().c_str());

}

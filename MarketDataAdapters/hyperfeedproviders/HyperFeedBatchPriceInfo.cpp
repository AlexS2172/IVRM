// HyperFeedBatchPriceInfo.cpp : Implementation of CHyperFeedBatchPriceInfo

#include "stdafx.h"
#include "HyperFeedBatchPriceInfo.h"
#include <ATLComTime.h>

/////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CHyperFeedBatchPriceInfo::put_UserName(BSTR userName)
{
	_bstr_t bsName(userName);
	m_strUserName = (LPCSTR)bsName;
	//_bstr_t bsMutexName (bsName);
	//bsMutexName+=L"7CFB99B3-491E-4c41-A517-1BF9B9890280";
	//m_hCustomerSyncro.SetLockName((LPCTSTR)bsMutexName);

	return S_OK;
}

STDMETHODIMP CHyperFeedBatchPriceInfo::RequestLastQuote(QuoteUpdateParams *Params)
{
	HRESULT hr  = S_OK;

	EgLib::CEgLibTraceManager::Trace(LogCustom, __FUNCTION__, _T("[%s]\t Entry"), m_strUserName.c_str());
	
	if (Params == NULL)
	{
		EgLib::CEgLibTraceManager::Trace(LogFaults, __FUNCTION__, _T("[%s]\t Exit with error - Params is NULL!!!"), m_strUserName.c_str());
		return _ERRSET(E_POINTER, L"RequestLastQuote", L"Params is NULL!!!");
	}

		EgLib::CEgLibTraceManager::Trace(LogInfo, __FUNCTION__, _T("[%s]\t Symbol: %s, Exchange: %s, Type: %d"), m_strUserName.c_str(), 
			Params->Symbol?(LPCSTR)_bstr_t(Params->Symbol):_T(""), 
			Params->Exchange?(LPCSTR)_bstr_t(Params->Exchange):_T(""), 
			Params->Type);
	
	try
	{
		CRequestBasePtr pRequest = boost::shared_dynamic_cast<CRequestBase>(CQuoteRequestPtr(new CQuoteRequest(_enRequestLast, Params)));
		if(pRequest!=NULL)
			AddRequest(pRequest);
		else
		{
			hr = E_POINTER;
			EgLib::CEgLibTraceManager::Trace(LogFaults, __FUNCTION__, _T("[%s]\t RequestLastQuote failed: Unable to allocate memory"), m_strUserName.c_str());
		}
	}
	catch (_com_error& Err)
	{
		EgLib::CEgLibTraceManager::Trace(LogFaults, __FUNCTION__, _T("[%s]\t RequestLastQuote failed: %d %s"), m_strUserName.c_str(), Err.Error(), (LPCSTR)Err.Description());
		_ERRTRACE(Err);			
		hr = Err.Error();
	}
	catch(...)
	{
		EgLib::CEgLibTraceManager::Trace(LogFaults, __FUNCTION__, _T("[%s]\t RequestLastQuote failed: Unknown Exception"), m_strUserName.c_str());
		_HANDLE_EXCEPTION;										
		hr = _ERRSET((HRESULT)E_UNEXPECTED, L"", L"Unknown Exception");		
	}
	EgLib::CEgLibTraceManager::Trace(LogCustom, __FUNCTION__, _T("[%s]\t Exit"), m_strUserName.c_str());
	return hr;
}

/////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CHyperFeedBatchPriceInfo::CancelLastQuote(VARIANT Params)
{
	EgLib::CEgLibTraceManager::Trace(LogCustom, __FUNCTION__, _T("[%s]\t Entry"), m_strUserName.c_str());
	HRESULT hr = S_OK;

	try
	{
		EgLib::CEgLibTraceManager::Trace(LogInfo, __FUNCTION__, _T("[%s]\t"), m_strUserName.c_str());
		if(V_VT(&Params) == VT_ERROR)
			RemoveAllRequest();
	}
	catch (_com_error& Err)
	{
		EgLib::CEgLibTraceManager::Trace(LogFaults, __FUNCTION__, _T("[%s]\t CancelLastQuote failed: %d %s"), m_strUserName.c_str(), Err.Error(), (LPCSTR)Err.Description());
		_ERRTRACE(Err);			
		hr = Err.Error();
	}
	catch(...)
	{
		EgLib::CEgLibTraceManager::Trace(LogFaults, __FUNCTION__, _T("[%s]\t CancelLastQuote failed: Unknown Exception"), m_strUserName.c_str());
		_HANDLE_EXCEPTION;										
		hr = _ERRSET((HRESULT)E_UNEXPECTED, L"", L"Unknown Exception");		
	}


	EgLib::CEgLibTraceManager::Trace(LogCustom, __FUNCTION__, _T("[%s]\t Exit"), m_strUserName.c_str());

	return hr;
}

/////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CHyperFeedBatchPriceInfo::SubscribeQuote(QuoteUpdateParams *Params)
{
	EgLib::CEgLibTraceManager::Trace(LogCustom, __FUNCTION__, _T("[%s]\t Entry"), m_strUserName.c_str());
	HRESULT hr = S_OK;

	try
	{
		CRequestBasePtr pRequest = CRequestBasePtr((CRequestBase*)new CQuoteRequest(_enRequestRealtime, Params));
		EgLib::CEgLibTraceManager::Trace(LogInfo, __FUNCTION__, _T("[%s]\t Symbol: %s, Exchange: %s, Type: %d"), m_strUserName.c_str() ,
			Params->Symbol?CW2T(Params->Symbol):_T(""), 
			Params->Exchange?CW2T(Params->Exchange):_T(""), 
			Params->Type);

		AddRequest(pRequest);
	}
	catch (_com_error& Err)
	{
		EgLib::CEgLibTraceManager::Trace(LogFaults, __FUNCTION__, _T("[%s]\t SubscribeQuote failed: %d %s"), m_strUserName.c_str(), Err.Error(), (LPCSTR)Err.Description());
		_ERRTRACE(Err);			
		hr = Err.Error();
	}
	catch(...)
	{
		EgLib::CEgLibTraceManager::Trace(LogFaults, __FUNCTION__, _T("[%s]\t SubscribeQuote failed: Unknown Exception"), m_strUserName.c_str());
		_HANDLE_EXCEPTION;										
		hr = _ERRSET((HRESULT)E_UNEXPECTED, L"", L"Unknown Exception");		
	}


	EgLib::CEgLibTraceManager::Trace(LogCustom, __FUNCTION__, _T("[%s]\t Exit"), m_strUserName.c_str());

	return hr;
}

/////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CHyperFeedBatchPriceInfo::UnSubscribeQuote(VARIANT Params)
{
	EgLib::CEgLibTraceManager::Trace(LogCustom, __FUNCTION__, _T("[%s]\t Entry"), m_strUserName.c_str());
	HRESULT hr = S_OK;

	try
	{
		_variant_t vtParams(Params);
		CGroupPrice::Unsubscribe(vtParams);
		if(!utils::HasValue(vtParams))
		 {
			ATLTRACE(_T("CHyperFeedBatchPriceInfo::UnSubscribeQuote RemoveAllRequest\n"));
			EgLib::CEgLibTraceManager::Trace(LogDebug, __FUNCTION__, _T("Entry 1"));
			RemoveAllRequest(); 

			ClearRealtimeData();
		 }		
	}
	catch (_com_error& Err)
	{
		EgLib::CEgLibTraceManager::Trace(LogFaults, __FUNCTION__, _T("[%s]\t UnSubscribeQuote failed: %d %s"), m_strUserName.c_str(), Err.Error(), (LPCSTR)Err.Description());
		_ERRTRACE(Err);			
		hr = Err.Error();
	}
	catch(...)
	{
		EgLib::CEgLibTraceManager::Trace(LogFaults, __FUNCTION__, _T("[%s]\t UnSubscribeQuote failed: Unknown Exception"), m_strUserName.c_str());
		_HANDLE_EXCEPTION;										
		hr = _ERRSET((HRESULT)E_UNEXPECTED, L"", L"Unknown Exception");		
	}

	EgLib::CEgLibTraceManager::Trace(LogCustom, __FUNCTION__, _T("[%s]\t Exit"), m_strUserName.c_str());

	return hr;
}

/////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CHyperFeedBatchPriceInfo::Connect()
{
	EgLib::CEgLibTraceManager::Trace(LogCustom, __FUNCTION__, _T("[%s]\t Entry"), m_strUserName.c_str());
	HRESULT hr = S_OK;

	try 
	{
		ConnectProvider();
	}
	catch (_com_error& Err)
	{
		EgLib::CEgLibTraceManager::Trace(LogFaults, __FUNCTION__, _T("[%s]\t Connect failed: %d %s"), m_strUserName.c_str(), Err.Error(), (LPCSTR)Err.Description());
		_ERRTRACE(Err);			
		hr = Err.Error();
	}
	catch(...)
	{
		EgLib::CEgLibTraceManager::Trace(LogFaults, __FUNCTION__, _T("[%s]\t Connect failed: Unknown Exception"), m_strUserName.c_str());
		_HANDLE_EXCEPTION;										
		hr = _ERRSET((HRESULT)E_UNEXPECTED, L"", L"Unknown Exception");		
	}

	EgLib::CEgLibTraceManager::Trace(LogCustom, __FUNCTION__, _T("[%s]\t Exit"), m_strUserName.c_str());

	return hr;
}

/////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CHyperFeedBatchPriceInfo::Disconnect()
{
	EgLib::CEgLibTraceManager::Trace(LogCustom, __FUNCTION__, _T("[%s]\t Entry"), m_strUserName.c_str());
	HRESULT hr = S_OK;

	try 
	{
		if(IsConnected())
		{
			UnsubscribeAll();
			DisconnectProvider();
		}
	}
	catch (_com_error& Err)
	{
		EgLib::CEgLibTraceManager::Trace(LogFaults, __FUNCTION__, _T("[%s]\t Disconnect failed: %d %s"), m_strUserName.c_str(), Err.Error(), (LPCSTR)Err.Description());
		_ERRTRACE(Err);			
		hr = Err.Error();
	}
	catch(...)
	{
		EgLib::CEgLibTraceManager::Trace(LogFaults, __FUNCTION__, _T("[%s]\t Disconnect failed: Unknown Exception"), m_strUserName.c_str());
		_HANDLE_EXCEPTION;										
		hr = _ERRSET((HRESULT)E_UNEXPECTED, L"", L"Unknown Exception");		
	}


	EgLib::CEgLibTraceManager::Trace(LogCustom, __FUNCTION__, _T("[%s]\t Exit"), m_strUserName.c_str());

	return hr;
}

/////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CHyperFeedBatchPriceInfo::get_IsQuotesUpdated(VARIANT_BOOL *pVal)
{
	EgLib::CEgLibTraceManager::Trace(LogCustom, __FUNCTION__, _T("[%s]\t Entry"), m_strUserName.c_str());
	HRESULT hr = S_OK;

	if(!pVal)
		return E_POINTER;

	SetEvent(m_hEventOnUpdate);
	*pVal = m_nUpdatesCount!=0 ? VARIANT_TRUE : VARIANT_FALSE;

	EgLib::CEgLibTraceManager::Trace(LogCustom, __FUNCTION__, _T("[%s]\t Exit"), m_strUserName.c_str());

	return hr;
}
/////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CHyperFeedBatchPriceInfo::GetQuotesUpdates(SAFEARRAY** pRetVal)
{
	EgLib::CEgLibTraceManager::Trace(LogCustom, __FUNCTION__, _T("[%s]\t Entry"), m_strUserName.c_str());
	SetEvent(m_hEventOnUpdate);
	HRESULT hr = S_OK;
	if(!pRetVal)
		return E_POINTER;


	size_t nSize = 0;
	long nUpdatesCount = 0;
	if(!pRetVal)
	{
		EgLib::CEgLibTraceManager::Trace(LogWarning, __FUNCTION__, _T("[%s]\t Exit: E_POINTER"), m_strUserName.c_str());
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


		nUpdatesCount = InterlockedExchange(&m_nUpdatesCount, 0);

		CUpdatesList listData;
		GetUpdatesList(m_dtLastUpdateSent, listData);

		m_dtLastUpdateSent = vt_date::GetCurrentDate();

		_QuoteUpdateFullInfo fullinfoInit;
		fullinfoInit.Init();

		nSize = listData.size();
		if(!listData.empty())
		{
			SAFEARRAYBOUND sab = { nSize, 0 };
			LPSAFEARRAY psa = ::SafeArrayCreateEx(VT_RECORD, 1, &sab, (PVOID)fullinfoInit.GetRecordInfo());
			if(psa)
			{
				QuoteUpdateFullInfo* pvData = NULL;
				if(SUCCEEDED(hr = ::SafeArrayAccessData(psa, (void**)&pvData)))
				{
					memset(pvData, 0, nSize * sizeof(QuoteUpdateFullInfo));
					CUpdatesList::iterator it;;
					ULONG i = 0;

					for(it = listData.begin(); it != listData.end(); ++i, ++it)
					{
						if( i >= nSize )
							break;
#ifdef __PERFMON
						AddClientUpdate();
#endif // __PERFMON
						CSubscriptionInfoPtr pData = *it;
						_QuoteUpdateFullInfo fullinfo;
						fullinfo.Init();
						if(pData!=NULL)
						{
							fullinfo->Params.Type = pData->GetType();
							CDBARecordPtr pDataRec = pData->GetData();
							if(pDataRec != NULL)
							{
								DBA_KEYS* pDbaKey = (DBA_KEYS*)&pDataRec->item;	 
								ParamFromKey(pDbaKey->dbaKey, pData->GetType(), &fullinfo->Params);
								pDataRec->close.price = pData->GetClosePrice();
							}
							CopyExtendedToInfo(pData->GetType(), pDataRec.get(), &fullinfo->Info);
						}
						fullinfo.CopyTo(&pvData[i]);
					}

					::SafeArrayUnaccessData(psa);
					*pRetVal = psa;
				}
				else
				{
					DWORD dwError = GetLastError();
					hr = HRESULT_FROM_WIN32(dwError);
					EgLib::CEgLibTraceManager::Trace(LogFaults, __FUNCTION__, _T("[%s]\t %d Unable to access Data Buffer"), m_strUserName.c_str(), dwError);
				}
			}
			else
			{
				DWORD dwError = GetLastError();
				hr = HRESULT_FROM_WIN32(dwError);
				EgLib::CEgLibTraceManager::Trace(LogFaults, __FUNCTION__, _T("[%s]\t %d Unable to create update Data Buffer"), m_strUserName.c_str(), dwError);
			}
		}
		else
		{
			EgLib::CEgLibTraceManager::Trace(LogFaults, __FUNCTION__, _T("[%s]\t No updates put into the Data Buffer"), m_strUserName.c_str());
		}
	}
	catch(_com_error& err)
	{
		
		EgLib::CEgLibTraceManager::Trace(LogFaults, __FUNCTION__, _T("[%s]\t COM Exception %d: %s"), m_strUserName.c_str(), err.Error(), err.Description().length()?(LPCSTR)err.Description():"");
		return _ERRSET(err, L"", L"GetQuotesUpdates failed");
	}
	catch(...)
	{
		EgLib::CEgLibTraceManager::Trace(LogFaults, __FUNCTION__, _T("[%s]\t Unknown exception"), m_strUserName.c_str());
		return _ERRSET((HRESULT)E_UNEXPECTED, L"", L"UNHANDLED EXCEPTION!!!");
	}

	if(FAILED(hr))
		EgLib::CEgLibTraceManager::Trace(LogError, __FUNCTION__, _T("[%s]\t No updates put into the Data Buffer Error %d"), m_strUserName.c_str(), hr);

	EgLib::CEgLibTraceManager::Trace(LogInfoExt, __FUNCTION__, _T("[%s]\t Sending %d Unique Quote Updates from %d received"), m_strUserName.c_str(), nSize, nUpdatesCount);
	EgLib::CEgLibTraceManager::Trace(LogCustom, __FUNCTION__, _T("[%s]\t Exit"), m_strUserName.c_str());
	return hr;
}


/////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CHyperFeedBatchPriceInfo::RequestLastGroupQuotes(QuoteUpdateParams * Params)
{
	EgLib::CEgLibTraceManager::Trace(LogCustom, __FUNCTION__, _T("[%s]\t Entry"), m_strUserName.c_str());
	HRESULT hr = S_OK;

	try
	{
		if(Params->Type != enGrSTK && Params->Type != enGrIDX)
			return E_INVALIDARG;

		CRequestBasePtr pRequest = boost::shared_dynamic_cast<CRequestBase>(CGroupRequestPtr(new CGroupRequest(enGroupRequestLastQuote, Params)));
		EgLib::CEgLibTraceManager::Trace(LogInfo, __FUNCTION__, _T("[%s]\t Symbol: %s, Exchange: %s, Type: %d"), m_strUserName.c_str(),
			Params->Symbol?(LPCSTR)_bstr_t(Params->Symbol):_T(""), 
			Params->Exchange?(LPCSTR)_bstr_t(Params->Exchange):_T(""), 
			Params->Type);

		if(pRequest!=NULL)
			AddRequest(pRequest);
		else
		{
			hr = E_POINTER;
			EgLib::CEgLibTraceManager::Trace(LogFaults, __FUNCTION__, _T("[%s]\t Unable to allocate memory"), m_strUserName.c_str());
		}
	}
	catch (_com_error& Err)
	{
		EgLib::CEgLibTraceManager::Trace(LogFaults, __FUNCTION__, _T("[%s]\t RequestLastGroupQuotes failed: %d %s"), m_strUserName.c_str(), Err.Error(), (LPCSTR)Err.Description());
		_ERRTRACE(Err);			
		hr = Err.Error();
	}
	catch(...)
	{
		EgLib::CEgLibTraceManager::Trace(LogFaults, __FUNCTION__, _T("[%s]\t RequestLastGroupQuotes failed: Unknown Exception"), m_strUserName.c_str());
		_HANDLE_EXCEPTION;										
		hr = _ERRSET((HRESULT)E_UNEXPECTED, L"", L"Unknown Exception");		
	}


	EgLib::CEgLibTraceManager::Trace(LogCustom, __FUNCTION__, _T("[%s]\t Exit"), m_strUserName.c_str());

	return hr;
}

/////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CHyperFeedBatchPriceInfo::CancelLastGroupQuotes(VARIANT Params)
{
	EgLib::CEgLibTraceManager::Trace(LogCustom, __FUNCTION__, _T("[%s]\t Entry"), m_strUserName.c_str());
	HRESULT hr = S_OK;

	try
	{
		CGroupPrice::CancelGroupPrice(Params);
		if(V_VT(&Params) == VT_ERROR)
			RemoveAllRequest();
	}
	catch (_com_error& Err)
	{
		EgLib::CEgLibTraceManager::Trace(LogFaults, __FUNCTION__, _T("[%s]\t CancelLastGroupQuotes failed: %d %s"), m_strUserName.c_str(), Err.Error(), (LPCSTR)Err.Description());
		_ERRTRACE(Err);			
		hr = Err.Error();
	}
	catch(...)
	{
		EgLib::CEgLibTraceManager::Trace(LogFaults, __FUNCTION__, _T("[%s]\t CancelLastGroupQuotes failed: Unknown Exception"), m_strUserName.c_str());
		_HANDLE_EXCEPTION;										
		hr = _ERRSET((HRESULT)E_UNEXPECTED, L"", L"Unknown Exception");		
	}

	EgLib::CEgLibTraceManager::Trace(LogCustom, __FUNCTION__, _T("[%s]\t Exit"), m_strUserName.c_str());
	return hr;
}

/////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CHyperFeedBatchPriceInfo::SubscribeGroupQuotes(QuoteUpdateParams * Params)
{
	EgLib::CEgLibTraceManager::Trace(LogCustom, __FUNCTION__, _T("[%s]\t Entry"), m_strUserName.c_str());
	HRESULT hr = S_OK;

	try
	{
		if(!Params || Params->Type != enGrSTK && Params->Type != enGrIDX)
			return E_INVALIDARG;

		CRequestBasePtr pRequest = CRequestBasePtr((CRequestBase*)new CGroupRequest(enGroupRequestAllNotify, Params));
		EgLib::CEgLibTraceManager::Trace(LogInfo, __FUNCTION__, _T("[%s]\t Symbol: %s, Exchange: %s, Type: %d"), m_strUserName.c_str(),
			Params->Symbol?CW2T(Params->Symbol):_T(""), 
			Params->Exchange?CW2T(Params->Exchange):_T(""), 
			Params->Type);
		AddRequest(pRequest);
	}
	catch (_com_error& Err)
	{
		EgLib::CEgLibTraceManager::Trace(LogFaults, __FUNCTION__, _T("[%s]\t SubscribeGroupQuotes failed: %d %s"), m_strUserName.c_str(), Err.Error(), (LPCSTR)Err.Description());
		_ERRTRACE(Err);			
		hr = Err.Error();
	}
	catch(...)
	{
		EgLib::CEgLibTraceManager::Trace(LogFaults, __FUNCTION__, _T("[%s]\t SubscribeGroupQuotes failed: Unknown Exception"), m_strUserName.c_str());
		_HANDLE_EXCEPTION;										
		hr = _ERRSET((HRESULT)E_UNEXPECTED, L"", L"Unknown Exception");		
	}

	EgLib::CEgLibTraceManager::Trace(LogCustom, __FUNCTION__, _T("[%s]\t Exit"), m_strUserName.c_str());
	return hr;
}

/////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CHyperFeedBatchPriceInfo::UnSubscribeGroupQuotes(VARIANT Params)
{
	EgLib::CEgLibTraceManager::Trace(LogInfo, __FUNCTION__, _T("Entry"));
	HRESULT hr = S_OK;

	try
	{
		CGroupPrice::UnsubscribeGroup(Params);
		if(V_VT(&Params) == VT_ERROR)
			RemoveAllRequest();
	}
	catch (_com_error& Err)
	{
		EgLib::CEgLibTraceManager::Trace(LogFaults, __FUNCTION__, _T("[%s]\t UnSubscribeGroupQuotes failed: %d %s"), m_strUserName.c_str(), Err.Error(), (LPCSTR)Err.Description());
		_ERRTRACE(Err);			
		hr = Err.Error();
	}
	catch(...)
	{
		EgLib::CEgLibTraceManager::Trace(LogFaults, __FUNCTION__, _T("[%s]\t UnSubscribeGroupQuotes failed: Unknown Exception"), m_strUserName.c_str());
		_HANDLE_EXCEPTION;										
		hr = _ERRSET((HRESULT)E_UNEXPECTED, L"", L"Unknown Exception");		
	}


	EgLib::CEgLibTraceManager::Trace(LogCustom, __FUNCTION__, _T("[%s]\t Exit"), m_strUserName.c_str());
	return hr;
}

/////////////////////////////////////////////////////////////////////////////
//
void CHyperFeedBatchPriceInfo::OnResponse(CResponseBasePtr pResponse)
{
	EgLib::CEgLibTraceManager::Trace(LogDebug, __FUNCTION__, _T("Entry"));

	try
	{
		switch(pResponse->m_enType)
		{
		case _enResponseLast:
			{
				EgLib::CEgLibTraceManager::Trace(LogDebug, __FUNCTION__, _T("[%s]\t Response type is _enResponseLast"), m_strUserName.c_str());
				
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
				EgLib::CEgLibTraceManager::Trace(LogDebug, __FUNCTION__, _T("[%s]\t Response type is _enResponseRealtimeNotification"), m_strUserName.c_str());
				CSubscribedResponse* pSubsResponse = dynamic_cast<CSubscribedResponse*>(pResponse.get());
				if(pSubsResponse)
				{
					OnSubscribed(pSubsResponse->m_vtParams);
				}
			}
			break;
		}
	}
	catch (_com_error& Err)
	{
		EgLib::CEgLibTraceManager::Trace(LogFaults, __FUNCTION__, _T("[%s]\t OnResponse failed: %d %s"), m_strUserName.c_str(), Err.Error(), (LPCSTR)Err.Description());
		_ERRTRACE(Err);			
	}
	catch(...)
	{
		EgLib::CEgLibTraceManager::Trace(LogFaults, __FUNCTION__, _T("[%s]\t OnResponse failed: Unknown Exception"), m_strUserName.c_str());
		_HANDLE_EXCEPTION;										
	}

	EgLib::CEgLibTraceManager::Trace(LogCustom, __FUNCTION__, _T("[%s]\t Exit"), m_strUserName.c_str());
}

/////////////////////////////////////////////////////////////////////////////
//
void CHyperFeedBatchPriceInfo::OnRequest(CRequestBasePtr pRequest)
{
	EgLib::CEgLibTraceManager::Trace(LogDebug, __FUNCTION__, _T("Entry"));

	switch(pRequest->m_enType)
	{
	case _enRequestLast:
		EgLib::CEgLibTraceManager::Trace(LogSubsExt, __FUNCTION__, _T("[%s]\t Request type is _enRequestLast"),m_strUserName.c_str());
		GetPrice(pRequest);
		break;
	case _enRequestRealtime:
		EgLib::CEgLibTraceManager::Trace(LogSubsExt, __FUNCTION__, _T("[%s]\t Request type is _enRequestRealtime"),m_strUserName.c_str());
		Subscribe(pRequest, true);
		break;
	case _enRequestGroup:
	{
		EgLib::CEgLibTraceManager::Trace(LogSubsExt, __FUNCTION__, _T("[%s]\t Request type is _enRequestGroup"),m_strUserName.c_str());
		CGroupRequestPtr pGroupRequest = boost::shared_dynamic_cast<CGroupRequest>(pRequest);
		if(pGroupRequest)
		{
			if(enGroupRequestLastQuote == pGroupRequest->m_enGroupRequestType)
				GetGroupPrice(pGroupRequest->m_recParams);
			else
				SubscribeGroup(pGroupRequest->m_recParams, pGroupRequest->m_enGroupRequestType);
		}
		else
			EgLib::CEgLibTraceManager::Trace(LogWarning, __FUNCTION__, _T("[%s]\t Group request has a wrong type"),m_strUserName.c_str());

	}
	break;
	}

	EgLib::CEgLibTraceManager::Trace(LogCustom, __FUNCTION__, _T("[%s]\t Exit"), m_strUserName.c_str());
}

/////////////////////////////////////////////////////////////////////////////
//
HRESULT CHyperFeedBatchPriceInfo::OnLastQuote(const CComVariant &varParams, const CComVariant &varResults)
{
	EgLib::CEgLibTraceManager::Trace(LogDebug, __FUNCTION__, _T("Entry"));
	SetEvent(m_hEventOnUpdate);
	
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
				//CAutoNamedLock nmLock(m_hCustomerSyncro, 500);
				hr = pDispatch->Invoke(0x1, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &disp, &varResult, NULL, NULL);
#ifdef __PERFMON
				AddClientUpdate();
#endif // __PERFMON
			}
	}
	delete[] pvars;

	if(FAILED(hr)) 
	{
		EgLib::CEgLibTraceManager::Trace(LogFaults, __FUNCTION__, _T("[%s]\t Exit with error hr = %d"),m_strUserName.c_str(), hr);
		return hr;
	}
		
	EgLib::CEgLibTraceManager::Trace(LogCustom, __FUNCTION__, _T("[%s]\t Exit"), m_strUserName.c_str());
	return varResult.scode;
}

/////////////////////////////////////////////////////////////////////////////
//
HRESULT CHyperFeedBatchPriceInfo::OnQuoteUpdate()
{
	EgLib::CEgLibTraceManager::Trace(LogDebug, __FUNCTION__, _T("Entry"));
	//SetEvent(m_hEventOnUpdate);

	CComVariant varResult;
	int nConnectionIndex;
	int nConnections = m_vec.GetSize();
	try
	{
//		size_t szUpdate = m_pFullInfoData->size();
//		if(!szUpdate)
//			InterlockedExchange(&m_nUpdatesCount, 0);

		if(!nConnections)
			EgLib::CEgLibTraceManager::Trace(LogWarning, __FUNCTION__, _T("[%s]\t %d Quote updates received, No Event sinks found"), m_strUserName.c_str(), m_nUpdatesCount);
		else
			EgLib::CEgLibTraceManager::Trace(LogInfo, __FUNCTION__, _T("[%s]\t Total %d quote updates received"), m_strUserName.c_str(), m_nUpdatesCount);
	}
	catch (...)
	{
		EgLib::CEgLibTraceManager::Trace(LogWarning, __FUNCTION__, _T("[%s]\t %d Exception in Getting Buffer Size"));
	}

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
			//CAutoNamedLock nmLock(m_hCustomerSyncro, 500);
			hr = pDispatch->Invoke(0x2, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &disp, &varResult, NULL, NULL);
#ifdef __PERFMON
			AddClientUpdate();
#endif // __PERFMON
		}
	}

	if(FAILED(hr)) 
	{
		if((HRESULT)0x800706BA == hr)
		{
			EgLib::CEgLibTraceManager::Trace(LogFaults, __FUNCTION__, _T("[%s]\t Exit with error RPC hr = %d"),m_strUserName.c_str(), hr);
			m_hfServer.OnIdle(true);
		}
		else
			EgLib::CEgLibTraceManager::Trace(LogFaults, __FUNCTION__, _T("[%s]\t Exit with error hr = %d"),m_strUserName.c_str(), hr);
		return hr;
	}

	m_dtLastUpdate = vt_date::GetCurrentDate();
	EgLib::CEgLibTraceManager::Trace(LogCustom, __FUNCTION__, _T("[%s]\t Exit"), m_strUserName.c_str());
	return varResult.scode;
}

//HRESULT CHyperFeedBatchPriceInfo::OnUnsubscribed()
//{
//	EgLib::CEgLibTraceManager::Trace(LogDebug, __FUNCTION__, _T("Entry"));
//
//	CComVariant varResult;
//	int nConnectionIndex;
//	int nConnections = m_vec.GetSize();
//
//	HRESULT hr = S_OK;
//	for(nConnectionIndex = 0; nConnectionIndex < nConnections; nConnectionIndex++)
//	{
//		Lock();
//		CComPtr<IUnknown> sp = m_vec.GetAt(nConnectionIndex);
//		Unlock();
//		IDispatch* pDispatch = reinterpret_cast<IDispatch*>(sp.p);
//		if(pDispatch != NULL)
//		{
//			VariantClear(&varResult);
//			DISPPARAMS disp = { NULL, NULL, 0, 0 };
//			//CAutoNamedLock nmLock(m_hCustomerSyncro, 500);
//			hr = pDispatch->Invoke(0x5, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &disp, &varResult, NULL, NULL);
//		}
//	}
//
//	if(FAILED(hr)) 
//	{
//		EgLib::CEgLibTraceManager::Trace(LogFaults, __FUNCTION__, _T("[%s]\t Exit with error hr = %d"),m_strUserName.c_str(), hr);
//		return hr;
//	}
//
//	m_dtLastUpdate = vt_date::GetCurrentDate();
//	EgLib::CEgLibTraceManager::Trace(LogDebug, __FUNCTION__, _T("Exit"));
//	return varResult.scode;
//}

/////////////////////////////////////////////////////////////////////////////
//
HRESULT CHyperFeedBatchPriceInfo::OnSubscribed(const CComVariant &varParams)
{
	EgLib::CEgLibTraceManager::Trace(LogDebug, __FUNCTION__, _T("Entry"));
	SetEvent(m_hEventOnUpdate);

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
			//CAutoNamedLock nmLock(m_hCustomerSyncro, 500);
			hr = pDispatch->Invoke(0x4, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &disp, &varResult, NULL, NULL);
#ifdef __PERFMON
			AddClientUpdate();
#endif // __PERFMON
		}
	}
	delete[] pvars;

	if(FAILED(hr)) 
	{
		EgLib::CEgLibTraceManager::Trace(LogFaults, __FUNCTION__, _T("[%s]\t Exit with error hr = %d"),m_strUserName.c_str(), hr);
		return hr;
	}
	
	EgLib::CEgLibTraceManager::Trace(LogDebug, __FUNCTION__, _T("Exit"));
	return varResult.scode;
}
/////////////////////////////////////////////////////////////////////////////
//
long CHyperFeedBatchPriceInfo::Attach()
{
	EgLib::CEgLibTraceManager::Trace(LogDebug, __FUNCTION__, _T("Entry"));

	EgLib::CThread::Start();
	long nRes = CGroupPrice::Attach();
	if(nRes != DBA_ERR_NO_ERROR)
	{
		HRESULT hr = MAKE_HRESULT (SEVERITY_ERROR, FACILITY_ITF, nRes);
		EgLib::CComErrorWrapper::ThrowError(hr, _T("Failed to attach to price fundamental database."));
	}
#ifdef __PERFMON
	try
	{
		if(m_pPerfMon==NULL)
			__CHECK_HRESULT3(m_pPerfMon.CreateInstance(__uuidof(PerfMon::CPPBatchInfo)));
		if(m_pPerfMon==NULL )
		{
			if(m_strUserName.length())
			{
				__CHECK_HRESULT3(m_pPerfMon->Initialize(_bstr_t(m_strUserName.c_str())));
			}
			else
			{
				__CHECK_HRESULT3(m_pPerfMon->Initialize(_bstr_t(L"Unknown")));
			}
		}
		m_pPerfMon->Clients = 1;
	}
	catch (_com_error& e)
	{
		EgLib::CEgLibTraceManager::Trace(LogWarning, __FUNCTION__, _T("Unable to create Performance object %i: %s"), e.Error(), e.Description().length()?(LPCSTR)e.Description():e.ErrorMessage());
		m_pPerfMon = NULL;
	}
	catch(...)
	{
		EgLib::CEgLibTraceManager::Trace(LogWarning, __FUNCTION__, _T("Unable to create Performance object %d"), GetLastError());
		m_pPerfMon = NULL;
	}
#endif // __PERFMON

	EgLib::CEgLibTraceManager::Trace(LogDebug, __FUNCTION__, _T("Exit"));
	return nRes;
}

/////////////////////////////////////////////////////////////////////////////
//
long CHyperFeedBatchPriceInfo::Detach()
{
	EgLib::CEgLibTraceManager::Trace(LogDebug, __FUNCTION__, _T("Entry"));
	EgLib::CThread::Stop();

	long nRes = CGroupPrice::Detach();
	if(nRes != DBA_ERR_NO_ERROR)
	{
		HRESULT hr = MAKE_HRESULT (SEVERITY_ERROR, FACILITY_ITF, nRes);
		EgLib::CComErrorWrapper::ThrowError(hr, _T("Failed to detach from price fundamental database."));
	}
#ifdef __PERFMON
	if(m_pPerfMon)
	{
		m_pPerfMon->Clients = 0;
		m_pPerfMon = NULL;
	}
#endif // __PERFMON

	EgLib::CEgLibTraceManager::Trace(LogDebug, __FUNCTION__, _T("Exit"));

	return nRes;
}

/////////////////////////////////////////////////////////////////////////////
//
void CHyperFeedBatchPriceInfo::PublicLastQuote(const _QuoteUpdateParams* pParams, const CDBARecordPtr& pData)
{
	EgLib::CEgLibTraceManager::Trace(LogDebug, __FUNCTION__, _T("Entry"));

	_QuoteUpdateInfo info;
	CopyExtendedToInfo((*pParams)->Type, pData.get(), info);
	if((*pParams)->Exchange)
		info->Exchange = SysAllocString((*pParams)->Exchange);
	CResponseBasePtr pResponse = boost::shared_static_cast<CResponseBase>(CQuoteInfoResponsePtr(new CQuoteInfoResponse(_enResponseLast, pParams, info)));
	PublicResponse(pResponse);
	EgLib::CEgLibTraceManager::Trace(LogDebug, __FUNCTION__, _T("Exit"));
}

/////////////////////////////////////////////////////////////////////////////
//
void CHyperFeedBatchPriceInfo::PublicQuoteUpdate(const _QuoteUpdateParams* pParams, const CSubscriptionInfoPtr& pData)
{
	EgLib::CEgLibTraceManager::Trace(LogDebug, __FUNCTION__, _T("Entry"));
	try
	{
		if(pData!=NULL)
		{
			InterlockedIncrement( &m_nUpdatesCount );
//			CAutoLock lock(m_csInsertDataLock);
//			CFullInfoDataCollectionPtr pDataUpdate(m_pFullInfoData);
//			if(pDataUpdate!=NULL)
//				pDataUpdate->insert(pData);		
		}
		SetEvent(m_hBatchDataReady);
	}
	catch(...)
	{
		EgLib::CEgLibTraceManager::Trace(LogWarning, __FUNCTION__, _T("Exception in putting quote update"));
	}

	EgLib::CEgLibTraceManager::Trace(LogDebug, __FUNCTION__, _T("Exit"));
}

UINT CHyperFeedBatchPriceInfo::Run()
{
	::CoInitializeEx(NULL,COINIT_MULTITHREADED);
	DWORD dwTicks   = GetTickCount();
	bool bContinue = true;

	DWORD dwTimeout =   _Module.GetBatchNotificationDelay();
	while(bContinue)
	{
		HANDLE hEvents[] = 
		{
			m_hBatchDataReady,
			m_hEventOnUpdate,
			EgLib::CThread::GetStopEventHandle()

		};

		DWORD dwResult = WaitWithEvents(3, hEvents, dwTimeout);
		switch(dwResult)
		{
		case WAIT_TIMEOUT:
			{
				if(m_nUpdatesCount)
					OnQuoteUpdate();

				dwTimeout =   static_cast<DWORD>(static_cast<double>(dwTimeout)*1.1);
				dwTicks = GetTickCount();
			}break;
		case WAIT_OBJECT_0:
			{
				DWORD dwCurrent = GetTickCount();
				DWORD dwElapsed = dwCurrent-dwTicks;
				if( dwElapsed >= dwTimeout && WaitWithEvents(m_hEventOnUpdate, 0) == WAIT_TIMEOUT)
				{
					OnQuoteUpdate();
					dwTimeout =   static_cast<DWORD>(static_cast<double>(dwTimeout)*1.1);
				}
				else
				{
					DWORD dwShortTimeout = dwTimeout - dwElapsed;
					if(dwTimeout < dwElapsed)
					{
						dwTimeout =   static_cast<DWORD>(static_cast<double>(dwTimeout)*1.1);
						continue;
					}


					HANDLE hEventsUpdate[] = 
					{
							m_hEventOnUpdate,
							EgLib::CThread::GetStopEventHandle()

					};

					switch(WaitWithEvents(2, hEventsUpdate, dwShortTimeout))
					{
					case WAIT_OBJECT_0:
						dwTimeout =   _Module.GetBatchNotificationDelay();
						break;
					case WAIT_TIMEOUT:
						OnQuoteUpdate();
						dwTimeout =   static_cast<DWORD>(static_cast<double>(dwTimeout)*1.1);

						break;
					default:
						bContinue = false;
						break;
					}
				}
				dwTicks = GetTickCount();
			}break;
		case WAIT_OBJECT_0+1:
			{
				dwTimeout =   _Module.GetBatchNotificationDelay();
				dwTicks   = GetTickCount();
				continue;
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
		EgLib::CEgLibTraceManager::Trace(LogDebug, __FUNCTION__ , _T("[%s]\t ProccessMessages Enter %s"), m_strUserName.c_str(), pExtRec->GetKey().c_str());
		PublicQuoteUpdate(NULL, pExtRec);
#ifdef __PERFMON
		if(m_pPerfMon)
			m_pPerfMon->AddProcessedUpdate();
		if(m_spGlobalPerfCounter)
			m_spGlobalPerfCounter->AddProcessedUpdate();
#endif // __PERFMON

	}
	_CATCH_UNHANDLED_EXCEPTION;
	EgLib::CEgLibTraceManager::Trace(LogDebug, __FUNCTION__ , _T("[%s]\t ProccessMessages Exit %s"),m_strUserName.c_str(), pExtRec->GetKey().c_str());

}

// FixGServer.cpp : Implementation of CFixGServer
#include "stdafx.h"
#include "FixGServer.h"

_ATL_FUNC_INFO CFixGServer::m_OnRequestNewOrder = 
{CC_STDCALL, VT_EMPTY, 1, {VT_DISPATCH} };
_ATL_FUNC_INFO CFixGServer::m_OnAlterOrder = 
{CC_STDCALL, VT_EMPTY, 1, {VT_DISPATCH} };
_ATL_FUNC_INFO CFixGServer::m_OnCancelOrder = 
{CC_STDCALL, VT_EMPTY, 1, {VT_DISPATCH} };
_ATL_FUNC_INFO CFixGServer::m_OnRequestOrderStatus = 
{CC_STDCALL, VT_EMPTY, 1, {VT_DISPATCH} };
_ATL_FUNC_INFO CFixGServer::m_OnFixGatewayStatusRequest = 
{CC_STDCALL, VT_EMPTY, 1, {VT_DISPATCH} };
_ATL_FUNC_INFO CFixGServer::m_OnFixGatewayStatus = 
{CC_STDCALL, VT_EMPTY, 1, {VT_DISPATCH} };

_ATL_FUNC_INFO CFixGServer::m_OnCancelReject = 
{CC_STDCALL, VT_EMPTY, 2, {VT_BSTR, VT_RECORD} };
_ATL_FUNC_INFO CFixGServer::m_OnExecutionReport = 
{CC_STDCALL, VT_EMPTY, 1, {VT_RECORD} };
_ATL_FUNC_INFO CFixGServer::m_OnOrderError = 
{CC_STDCALL, VT_EMPTY, 2, {VT_BSTR, VT_BSTR} };
_ATL_FUNC_INFO CFixGServer::m_OnDisconnect = 
{CC_STDCALL, VT_EMPTY, 1, {VT_I4} };
_ATL_FUNC_INFO CFixGServer::m_OnReLogon = 
{CC_STDCALL, VT_EMPTY, 1, {VT_I4} };
_ATL_FUNC_INFO CFixGServer::m_OnLogonFailed = 
{CC_STDCALL, VT_EMPTY, 2, {VT_I4, VT_BSTR} };


/////////////////////////////////////////////////////////////////////////////
// CFixGServer
CFixGServer* CFixGServer::s_pFixGServer = NULL;


HRESULT CFixGServer::FinalConstruct()
{
	s_pFixGServer = this;

	m_hStartFixEvent = CreateEvent(NULL,FALSE,FALSE,NULL);
	if(INVALID_HANDLE_VALUE == m_hStartFixEvent)
		return HRESULT_FROM_WIN32(GetLastError());

	m_hStopFixEvent = CreateEvent(NULL,FALSE,FALSE,NULL);
	if(INVALID_HANDLE_VALUE == m_hStopFixEvent)
		return HRESULT_FROM_WIN32(GetLastError());

	m_hGatewaysCheckTimer = CreateWaitableTimer(NULL, FALSE, NULL);
	if(INVALID_HANDLE_VALUE == m_hGatewaysCheckTimer)
		return HRESULT_FROM_WIN32(GetLastError());

	m_hGatewaysRuns = CreateEvent(NULL,FALSE,FALSE,NULL);
	if(INVALID_HANDLE_VALUE == m_hGatewaysRuns)
		return HRESULT_FROM_WIN32(GetLastError());

	m_hStartFixTimer = CreateWaitableTimer(NULL, FALSE, NULL);
	if(INVALID_HANDLE_VALUE == m_hStartFixTimer)
		return HRESULT_FROM_WIN32(GetLastError());

	m_hStopFixTimer = CreateWaitableTimer(NULL, FALSE, NULL);
	if(INVALID_HANDLE_VALUE == m_hStopFixTimer)
		return HRESULT_FROM_WIN32(GetLastError());

	try
	{
		m_spSettingsMgr = FXGSM :: CSmSettingsMgrPtr(new FXGSM::CSmSettingsMgr);
		m_spSettingsMgr->Init();

		if(m_spSettingsMgr->AutoStartStopFix())
		{
			_QueueSetFixStartTimer(m_spSettingsMgr->StartFixTime());
			_QueueSetFixStopTimer(m_spSettingsMgr->StopFixTime());

			if(_CheckIsWorkingTime())				
			{
				ObjectLock lock(this);
				m_enStatus = enFgSsStartPending;
				SetEvent(m_hStartFixEvent);
			}
		}

		CThread::Start();
	}
	catch(exception&)
	{
		return E_FAIL;
	}
    return S_OK;
}

void CFixGServer::FinalRelease()
{
	CThread::Stop();

	if(m_hStartFixEvent)
		CloseHandle(m_hStartFixEvent);
	if(m_hStopFixEvent)
		CloseHandle(m_hStopFixEvent);
	if(m_hGatewaysRuns)
		CloseHandle(m_hGatewaysRuns);

	if(m_hGatewaysCheckTimer)
	{
		CancelWaitableTimer(m_hGatewaysCheckTimer);
		CloseHandle(m_hGatewaysCheckTimer);
	}

	if(m_hStartFixTimer)
	{
		CancelWaitableTimer(m_hStartFixTimer);
		CloseHandle(m_hStartFixTimer);
	}

	if(m_hStopFixTimer)
	{
		CancelWaitableTimer(m_hStopFixTimer);
		CloseHandle(m_hStopFixTimer);
	}

	m_hStartFixEvent = NULL;
	m_hStopFixEvent = NULL;
	m_hGatewaysRuns = NULL;
	m_hGatewaysCheckTimer = NULL;
	m_hStartFixTimer = NULL;
	m_hStopFixTimer = NULL;
	s_pFixGServer = NULL;

}

/////////////////////////////////////////////////////////////////////////////
void CFixGServer::_ConnectProvider(FX::IOrderProviderPtr& spProvider,
                                  MsFixProviderEnum enLoc)
{
	HRESULT hr = spProvider.CreateInstance(__uuidof(FX::FixProvider));
    if (FAILED(hr))
		CComErrorWrapper::ThrowError(hr, _T("Failed to create IOrderProvider object."));

	FX::IFixProviderInitializerPtr spInitializer = spProvider;
    switch(enLoc)
    {
        case enMsFpOConnor:
            spInitializer->SetType((long)enMsFpOConnor);
            break;
        default:
			CComErrorWrapper::ThrowError(E_FAIL, _T("Invalid location."));
    }

    spProvider->Connect();

    hr = CProvidersDispEvent::DispEventAdvise(spProvider);
    if(FAILED(hr))
		CComErrorWrapper::ThrowError(hr, _T("Failed to advise OrderProvider object events."));
}

/////////////////////////////////////////////////////////////////////////////
void CFixGServer::_DisconnectProvider(MsFixProviderEnum enLoc)
{
    ObjectLock Lock(this);

    TProvidersMap::iterator it;
    if ( (it = m_mapProviders.find(enLoc)) != m_mapProviders.end())
    {
        try
        {
			CFgProvider& aProv = it->second;
            FX::IOrderProviderPtr spProvider(aProv.m_spProvider);
			
			if(spProvider)
			{
				CProvidersDispEvent::DispEventUnadvise(spProvider);
				spProvider->Disconnect();
				aProv.m_spProvider = NULL;
			}
        }
        catch(_com_error&) {}
    }
}

/////////////////////////////////////////////////////////////////////////////
void CFixGServer::_ValidateProvidersMap(MsFixProviderEnum enLoc)
{
    ObjectLock Lock(this);

    try
    {		
		TProvidersMap::iterator it = m_mapProviders.find(enLoc);
		if (it == m_mapProviders.end())
			CComErrorWrapper::ThrowError(E_FAIL, _T("Invalid location."));

		CFgProvider& aProv = it->second;
		FX::IOrderProviderPtr spProvider(aProv.m_spProvider);

        if (spProvider == NULL)
        {
            _ConnectProvider(spProvider, enLoc);
			aProv.m_spProvider = spProvider;
			aProv.m_bDisconnected = false;
			
			CString strMessage;
			strMessage.Format(_T("Successfully connect to '%s'"), (LPCTSTR)aProv.m_bsProviderName);

			Fire_OnLogMessage(COleDateTime::GetCurrentTime(), enFgLogInfo, _bstr_t(strMessage));
        }
    }
    catch(_com_error& e)
    {
		CComErrorWrapper::ThrowError(utils::ComError2ErrInfo(e, this, __uuidof(IFixGServer)));
	}
}

/////////////////////////////////////////////////////////////////////////////
void CFixGServer::_ClearProvidersMap()
{
    ObjectLock Lock(this);

    while(!m_mapProviders.empty())
    {
		TProvidersMap::iterator it = m_mapProviders.begin();
        _DisconnectProvider(it->first);
		m_mapProviders.erase(it);
    }
}

/////////////////////////////////////////////////////////////////////////////
void CFixGServer::_InitProvidersMap()
{
	ObjectLock Lock(this);
        
	FX::IFixProvidersPtr spProviders(__uuidof(FX::FixProviders));
	spProviders->Initialize();
	
	HRESULT hr;
	_variant_t varItem;
	ULONG nFetched = 0L;
	IUnknownPtr spUnk;
	FX::IFixProviderSpecificsPtr spProviderSpec;

	if(FAILED(hr = spProviders->get__NewEnum(&spUnk)))
		CComErrorWrapper::ThrowError(hr, _T("Can't enum Fix providers"));

	IEnumVARIANTPtr spProvEnum(spUnk);
	if(FAILED(hr = spProvEnum->Reset()))
		CComErrorWrapper::ThrowError(hr, _T("Can't reset enum of Fix providers"));

	while((hr = spProvEnum->Next(1L, &varItem, &nFetched)) == S_OK)
	{
		ATLASSERT(varItem.vt == VT_DISPATCH);
		spProviderSpec = varItem;

		if(nFetched > 0 && spProviderSpec != NULL)
		{
			long lProvID = 0;
			BSTR bsProgId = NULL;
			BSTR bsDescr = NULL;
			VARIANT_BOOL bNeedLogin = VARIANT_FALSE;
			BSTR bsAccount = NULL;
			BSTR bsPassword = NULL;

			spProviderSpec->GetSpecifics(&lProvID, &bsProgId, &bsDescr, &bNeedLogin, &bsAccount, &bsPassword);

			MsFixProviderEnum enLoc = static_cast<MsFixProviderEnum>(lProvID);

			try
			{
				FX::IOrderProviderPtr spProvider;

				if (m_mapProviders.find(enLoc) == m_mapProviders.end())
				{
					CFgProvider aProv;
					aProv.m_bsProviderName = bsDescr;
					aProv.m_spProvider = NULL;
					
					pair <TProvidersMap::iterator, bool> pair = m_mapProviders.insert(make_pair(enLoc, aProv));
					_ConnectProvider(spProvider, enLoc);

					CFgProvider& aProvider =  pair.first->second;
					aProvider.m_spProvider = spProvider;

					CString strMessage;
					strMessage.Format(_T("Successfully connect to '%s'"), (LPCTSTR)aProv.m_bsProviderName);

					Fire_OnLogMessage(COleDateTime::GetCurrentTime(), enFgLogInfo, _bstr_t(strMessage));
				}
			}
			catch(_com_error& e)
			{
				CString strMessage;
				strMessage.Format(_T("Error occured while initialize Fix providers. %s"), (LPCTSTR)e.Description());

				Fire_OnLogMessage(COleDateTime::GetCurrentTime(), enFgLogError, _bstr_t(strMessage));
			}

			SysFreeString(bsProgId);
			SysFreeString(bsDescr);
			SysFreeString(bsAccount);
			SysFreeString(bsPassword);
		}
		spProviderSpec = NULL;
		varItem.Clear();
	}
}
/////////////////////////////////////////////////////////////////////////////
void CFixGServer::_GetProvider(FX::IOrderProviderPtr& spProvider, MsFixProviderEnum enLoc)
{
    ObjectLock Lock(this);

    TProvidersMap::iterator it;
    if ( (it = m_mapProviders.find(enLoc)) != m_mapProviders.end())
    {
		CFgProvider& aProv = it->second;
        spProvider = aProv.m_spProvider;
    }
    else
    {
		CComErrorWrapper::ThrowError(E_FAIL, _T("Unable to find specified provider"));
    }
}

/////////////////////////////////////////////////////////////////////////////
HANDLE CFixGServer::_GetEventHandle(DISP_IDS dispid)
{
    ObjectLock Lock(this);

    HANDLE hEvent;
    TEventsMap::iterator it = m_mapEvents.find(dispid);
    if (it != m_mapEvents.end())
        hEvent = it->second;
    else
        hEvent = INVALID_HANDLE_VALUE;

    return hEvent;
}

/////////////////////////////////////////////////////////////////////////////
STDMETHODIMP CFixGServer::OnRequestNewOrder(IOrder* Order)
{
	MsFixProviderEnum enLoc = Order->FixProvider;

	try
	{
		_ValidateProvidersMap(enLoc);
	}
	catch(_com_error& e)
	{
		_PubError(Order, e, _T("Can't send new order."));
		
		CString strError;
		strError.Format(_T("Can't send new order. %s"), (LPCTSTR)e.Description());
		Fire_OnLogMessage(COleDateTime::GetCurrentTime(), enFgLogError, _bstr_t(strError));

		return utils::ComError2ErrInfo(e, this, L"Can't send new order.", __uuidof(IFixGServer));
	}

	CFgThreadPoolParams aParam;
	aParam.m_dispID = DISPID_OnRequestNewOrder;
	aParam.m_enLoc = enLoc;

	try
	{
		aParam.m_spOrder = Order;
	}
	catch(_com_error& e)
	{
		return utils::ComError2ErrInfo(e, this, L"Can't send new order.", __uuidof(IFixGServer));
	}

	HANDLE hEvent = _GetEventHandle(aParam.m_dispID);
	if (hEvent == INVALID_HANDLE_VALUE) return E_HANDLE;

	if (FXGCO::CThreadPool::QueueUserWorkItem(_Thread_Order, &aParam, 0))
	{
		::WaitForSingleObject(hEvent,10*1000);
		::ResetEvent(hEvent);
	}
	return S_OK;
}

/////////////////////////////////////////////////////////////////////////////
STDMETHODIMP CFixGServer::OnAlterOrder(IOrder* Order)
{
	MsFixProviderEnum enLoc = Order->FixProvider;

	try
	{
		_ValidateProvidersMap(enLoc);
	}
	catch(_com_error& e)
	{
		_PubError(Order, e, _T("Can't Cancel/Replace order."));
		CString strError;
		strError.Format(_T("Can't Cancel/Replace order. %s"), (LPCTSTR)e.Description());
		Fire_OnLogMessage(COleDateTime::GetCurrentTime(), enFgLogError, _bstr_t(strError));

		return utils::ComError2ErrInfo(e, this, L"Can't Cancel/Replace order.", __uuidof(IFixGServer));
	}

	CFgThreadPoolParams aParam;
	aParam.m_dispID = DISPID_OnAlterOrder;
	aParam.m_enLoc = enLoc;

	try
	{
		aParam.m_spOrder = Order;
	}
	catch(_com_error& e)
	{
		return utils::ComError2ErrInfo(e, this, L"Can't Cancel/Replace order.", __uuidof(IFixGServer));
	}

	HANDLE hEvent = _GetEventHandle(aParam.m_dispID);
	if (hEvent == INVALID_HANDLE_VALUE) return E_HANDLE;

	if (FXGCO::CThreadPool::QueueUserWorkItem(_Thread_Order, &aParam, 0))
	{
		::WaitForSingleObject(hEvent,10*1000);
		::ResetEvent(hEvent);
	}
	return S_OK;
}

/////////////////////////////////////////////////////////////////////////////
STDMETHODIMP CFixGServer::OnCancelOrder(IOrder* Order)
{
	MsFixProviderEnum enLoc = Order->FixProvider;

	try
	{
		_ValidateProvidersMap(enLoc);
	}
	catch(_com_error& e)
	{
		_PubError(Order, e, _T("Can't Cancel order."));
		CString strError;
		strError.Format(_T("Can't Cancel order. %s"), (LPCTSTR)e.Description());
		Fire_OnLogMessage(COleDateTime::GetCurrentTime(), enFgLogError, _bstr_t(strError));

		return utils::ComError2ErrInfo(e, this, L"Can't Cancel order.", __uuidof(IFixGServer));
	}

	CFgThreadPoolParams aParam;
	aParam.m_dispID = DISPID_OnCancelOrder;
	aParam.m_enLoc = enLoc;

	try
	{
		aParam.m_spOrder = Order;
	}
	catch(_com_error& e)
	{
		return utils::ComError2ErrInfo(e, this, L"Can't Cancel order.", __uuidof(IFixGServer));
	}

	HANDLE hEvent = _GetEventHandle(aParam.m_dispID);
	if (hEvent == INVALID_HANDLE_VALUE) return E_HANDLE;

	if (FXGCO::CThreadPool::QueueUserWorkItem(_Thread_Order, &aParam, 0))
	{
		::WaitForSingleObject(hEvent,10*1000);
		::ResetEvent(hEvent);
	}
	return S_OK;
}

/////////////////////////////////////////////////////////////////////////////
STDMETHODIMP CFixGServer::OnRequestOrderStatus(IOrder* Order)
{
	MsFixProviderEnum enLoc = Order->FixProvider;

	try
	{
		_ValidateProvidersMap(enLoc);
	}
	catch(_com_error& e)
	{
		_PubError(Order, e, _T("Can't request order status."));
		CString strError;
		strError.Format(_T("Can't request order status. %s"), (LPCTSTR)e.Description());
		Fire_OnLogMessage(COleDateTime::GetCurrentTime(), enFgLogError, _bstr_t(strError));

		return utils::ComError2ErrInfo(e, this, L"Can't request order status.", __uuidof(IFixGServer));
	}

	CFgThreadPoolParams aParam;
	aParam.m_dispID = DISPID_OnRequestOrderStatus;
	aParam.m_enLoc = enLoc;

	try
	{
		aParam.m_spOrder = Order;
	}
	catch(_com_error& e)
	{
		return utils::ComError2ErrInfo(e, this, L"Can't request order status.", __uuidof(IFixGServer));
	}

	HANDLE hEvent = _GetEventHandle(aParam.m_dispID);
	if (hEvent == INVALID_HANDLE_VALUE) return E_HANDLE;

	if (FXGCO::CThreadPool::QueueUserWorkItem(_Thread_Order, &aParam, 0))
	{
		::WaitForSingleObject(hEvent,10*1000);
		::ResetEvent(hEvent);
	}
	return S_OK;
}

/////////////////////////////////////////////////////////////////////////////
STDMETHODIMP CFixGServer::OnFixGatewayStatusRequest(IFixGatewayStatusRequest* Request)
{
	IPubManagerPtr  spPubManager;
	{
		ObjectLock lock(this);
		if(m_enStatus != enFgSsStarted)
			return S_OK;

		spPubManager = m_spPubManager;
	}


	if(spPubManager)
	{
		IFixGatewayStatusPtr spStatus(__uuidof(FixGatewayStatus));
		
		spStatus->PutIP(_bstr_t(m_strIP));
		spStatus->PutHost(_bstr_t(m_strHost));
		spStatus->PutIsOnline(VARIANT_TRUE);
		spPubManager->PubFixGatewayStatus(spStatus);
	}

	return S_OK;
}
/////////////////////////////////////////////////////////////////////////////
STDMETHODIMP CFixGServer::OnFixGatewayStatus(IFixGatewayStatus* Status)
{
	if(VARIANT_TRUE == Status->GetIsOnline())
	{
		CString strMessage;
		strMessage.Format(_T("FixGateway server already runned at : '%s' (%s)"), (LPCTSTR)Status->Host, (LPCTSTR)Status->IP );
		Fire_OnLogMessage(COleDateTime::GetCurrentTime(), enFgLogWarning, _bstr_t(strMessage));
		SetEvent(m_hGatewaysRuns);
	}

	return S_OK;
}

/////////////////////////////////////////////////////////////////////////////
void CFixGServer::_PubError(IOrder* pOrder, _com_error& e, LPCTSTR lpszCaption)
{
    if (pOrder)
    {
		try
		{
			if(lpszCaption)
			{
				CString strError;
				strError.Format(_T("%s %s"), lpszCaption, (LPCTSTR)e.Description());
				pOrder->PutText(_bstr_t(strError));
			}
			else
			{
				pOrder->PutText(e.Description());
			}
		}
		catch(_com_error&)
		{
		}

		pOrder->PutExecType(enMsOsRejected);

		FXG::CFgOrderPtr spOrd(m_OrderStorage.Find(pOrder->GetClOrdID()));
		if(spOrd)
			pOrder->PutOrdStatus(spOrd->m_enConfirmedStatus);

		m_spPubManager->PubOrder(pOrder);
    }
}
/////////////////////////////////////////////////////////////////////////////
DWORD WINAPI CFixGServer::_Thread_Order(LPVOID lpParam)
{
    CoInitializeEx(NULL, COINIT_MULTITHREADED);

    CFgThreadPoolParams* pThreadParam = reinterpret_cast<CFgThreadPoolParams*>(lpParam);

	DISP_IDS dispID = pThreadParam->m_dispID;
    MsFixProviderEnum enLoc = pThreadParam->m_enLoc;

    HANDLE hEvent = s_pFixGServer->_GetEventHandle(dispID);
    if (hEvent == INVALID_HANDLE_VALUE)
        return ERROR_INVALID_HANDLE;

    FX::IOrderProviderPtr spProvider;
    IPubManagerPtr  spPubManager = s_pFixGServer->m_spPubManager;
    IOrderPtr spOrder;

    try
    {
        spOrder = pThreadParam->m_spOrder;
        if (spOrder == NULL)
            _com_issue_error(E_NOINTERFACE);

        s_pFixGServer->_GetProvider(spProvider, enLoc);

        ::SetEvent(hEvent);

        _bstr_t bsOrderID;

        switch(dispID)
        {
            case DISPID_OnRequestNewOrder:
                {
					FX::FoOrder Order;
					memset(&Order,0,sizeof(FX::FoOrder));
					s_pFixGServer->_FillOrder(spOrder,&Order);
					bsOrderID = spProvider->SubmitNewOrder(&Order);
					spOrder->PutClOrdID(Order.ClOrdID);
					spOrder->PutOrdStatus(enMsOsPendingNew);

					s_pFixGServer->m_OrderStorage.Insert(spOrder, true);

					spPubManager->PubOrder(spOrder);

					CString strMessage;
					strMessage.Format(_T("Sent New Order, OrderID: '%s'"), (LPCTSTR)bsOrderID);
					s_pFixGServer->Fire_OnLogMessage(COleDateTime::GetCurrentTime(), enFgLogInfo, _bstr_t(strMessage));
                }
                break;

            case DISPID_OnAlterOrder:
                {
					_bstr_t bsOldOrderID = spOrder->GetClOrdID();

					if(!s_pFixGServer->m_OrderStorage.Find(bsOldOrderID))
						s_pFixGServer->m_OrderStorage.Insert(spOrder);

					FX::FoOrderCancelReplaceRequest Request;
					memset(&Request,0,sizeof(FX::FoOrderCancelReplaceRequest));
					s_pFixGServer->_FillOrderCancelReplaceRequest(spOrder, &Request);
					bsOrderID = spProvider->AlterOrder(bsOldOrderID, &Request);

					spOrder->PutOrdStatus(enMsOsPendingCancelReplace);
					s_pFixGServer->m_OrderStorage.UpdateStatus(spOrder);

					spPubManager->PubOrder(spOrder);
					
					CString strMessage;
					strMessage.Format(_T("Sent Order Cancel/Replace Request, OrderID: '%s', Original OrderID: '%s'"), (LPCTSTR)bsOrderID, (LPCTSTR)bsOldOrderID );
					s_pFixGServer->Fire_OnLogMessage(COleDateTime::GetCurrentTime(), enFgLogInfo, _bstr_t(strMessage));
                }
                break;

            case DISPID_OnCancelOrder:
                {
					_bstr_t bsOldOrderID = spOrder->GetClOrdID();

					if(!s_pFixGServer->m_OrderStorage.Find(bsOldOrderID))
						s_pFixGServer->m_OrderStorage.Insert(spOrder);

					FX::FoOrderCancelRequest Request;
					memset(&Request,0,sizeof(FX::FoOrderCancelRequest));
					s_pFixGServer->_FillOrderCancelRequest(spOrder, &Request);

					bsOrderID = spProvider->CancelOrder(bsOldOrderID, &Request);
					   
					spOrder->PutOrdStatus(enMsOsPendingCancelReplace);
					s_pFixGServer->m_OrderStorage.UpdateStatus(spOrder);

					spPubManager->PubOrder(spOrder);

					CString strMessage;
					strMessage.Format(_T("Sent Order Cancel Request, OrderID: '%s', Original OrderID: '%s'"), (LPCTSTR)bsOrderID, (LPCTSTR)bsOldOrderID );
					s_pFixGServer->Fire_OnLogMessage(COleDateTime::GetCurrentTime(), enFgLogInfo, _bstr_t(strMessage));
                }
                break;

            case DISPID_OnRequestOrderStatus:
                {
					bsOrderID = spOrder->GetClOrdID();
					if(!s_pFixGServer->m_OrderStorage.Find(bsOrderID))
						s_pFixGServer->m_OrderStorage.Insert(spOrder);

					FX::FoOrderStatusRequest Request;
					memset(&Request,0,sizeof(FX::FoOrderStatusRequest));
					s_pFixGServer->_FillOrderStatusRequest(spOrder, &Request);
					spProvider->RequestOrderStatus(bsOrderID, &Request);

					CString strMessage;
					strMessage.Format(_T("Sent Order Status Request, OrderID: '%s'"), (LPCTSTR)bsOrderID );
					s_pFixGServer->Fire_OnLogMessage(COleDateTime::GetCurrentTime(), enFgLogInfo, _bstr_t(strMessage));
                }
        }
    }
    catch(_com_error& e)
    {
        s_pFixGServer->_PubError(spOrder, e);
    }

    spOrder = 0;
    CoUninitialize();
    return ERROR_SUCCESS;
}

/////////////////////////////////////////////////////////////////////////////
STDMETHODIMP CFixGServer::OnCancelReject(BSTR Reason, FX::FoOrderCancelReject* Report)
{
    try
    {
		CString strMessage;
		strMessage.Format(_T("Order Cancel Reject received, OrderID: '%s', Original OrderID: '%s', OrdStatus: '%s', (%s)"),
			(LPCTSTR)_bstr_t(Report->ClOrdID), (LPCTSTR)_bstr_t(Report->OrigClOrdID), 
			_OrderStatusDescription(Report->OrdStatus), (LPCTSTR)_bstr_t(Report->Text));

		Fire_OnLogMessage(COleDateTime::GetCurrentTime(), enFgLogWarning, _bstr_t(strMessage));

		_bstr_t bsKeyOrig = Report->OrigClOrdID;

		IOrderPtr  spOrder;

		FXG::CFgOrderPtr spOrd(m_OrderStorage.Find(bsKeyOrig));
		if(!spOrd)
			CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Cancel reject on unknown order."));

		HRESULT hr = spOrder.CreateInstance(CLSID_Order);
		if (FAILED(hr))
			CComErrorWrapper::ThrowError(hr, _T("Failed to create Order object."));

		spOrder->PutId(spOrd->m_nId);

		_ExtractOrderCancelReject(Report, spOrder);

		m_OrderStorage.UpdateStatus(spOrder, true);

		m_spPubManager->PubOrder(spOrder);
    }
    catch(_com_error& e)
    {
		return utils::ComError2ErrInfo(e, this, L"OnCancelReject failed.", __uuidof(IFixGServer));
    }

    return S_OK;
}

/////////////////////////////////////////////////////////////////////////////
STDMETHODIMP CFixGServer::OnExecutionReport(FX::FoExecutionReport* Report)
{
    try
    {
		CString strMessage;
		strMessage.Format(_T("Execution report received, ExecID: '%s', OrderID: '%s', Original OrderID: '%s', ExecType: '%s', OrdStatus: '%s', (%s)"), (LPCTSTR)_bstr_t(Report->ExecID), 
			(LPCTSTR)_bstr_t(Report->ClOrdID), (LPCTSTR)_bstr_t(Report->OrigClOrdID), _OrderStatusDescription(Report->ExecType),  
			_OrderStatusDescription(Report->OrdStatus), (LPCTSTR)_bstr_t(Report->Text));

		Fire_OnLogMessage(COleDateTime::GetCurrentTime(), enFgLogInfo, _bstr_t(strMessage));

        _bstr_t bsKey = Report->ClOrdID;
		_bstr_t bsKeyOrig = Report->OrigClOrdID;
		_bstr_t bsKeyExec = Report->ExecRefID;

		long nExecId = 0L;

		bool IsCorrectReport = false, IsReport = false;
		HRESULT hr = S_OK;
	
		if(FX::enFoTrnCancel == Report->ExecTransType || FX::enFoTrnCorrect == Report->ExecTransType)
			IsCorrectReport = true;
		else if(FX::enFoTrnNew == Report->ExecTransType && (FX::enFoOsFilled == Report->ExecType || FX::enFoOsPartiallyFilled == Report->ExecType))
			IsReport = true;

		IExecutionReportPtr  spReport;
		ITradeUpdatePtr spTradeUpdate;
		IOrderPtr  spOrder;
		DWORD dwRes;

		if(IsCorrectReport || IsReport)
		{
			hr = spOrder.CreateInstance(CLSID_Order);
			if (FAILED(hr))
				CComErrorWrapper::ThrowError(hr, _T("Failed to create Order object."));

			hr = spTradeUpdate.CreateInstance(CLSID_TradeUpdate);
			if (FAILED(hr))
				CComErrorWrapper::ThrowError(hr, _T("Failed to create TradeUpdate object."));
		}
		else
		{
			hr = spOrder.CreateInstance(CLSID_Order);
			if (FAILED(hr))
				CComErrorWrapper::ThrowError(hr, _T("Failed to create Order object."));
		}

		if(IsCorrectReport)
		{
			//if(!bCorrection)
			//	m_Publisher.PushData(pTrade);
			//else
			//{
			//	pTrade->cExecTransType = FIX::ExecTransType_CANCEL;
			//	m_Publisher.PushData(pTrade);

			//	SwitchToThread();
			//	CTradePtr pNewTrade = CTradePtr(new CTrade(pTrade.get()));
			//	pNewTrade->cExecTransType = FIX::ExecTransType_NEW;
			//	pNewTrade->sExecID = sCorrectionID;
			//	m_Publisher.PushData(pNewTrade);

			//}
			_bstr_t bsCorrectionID = Report->ExecID;
			Report->ExecID = bsKeyExec.copy();

			dwRes = ERROR_SUCCESS;
			if(Report->ExecTransType == FX::enFoTrnCancel)
			{
				dwRes = m_ExecStorage.InsertTrade(Report, spTradeUpdate);
				if(dwRes == ERROR_SUCCESS)
					m_spPubManager->PubTradeUpdate(spTradeUpdate);
			}
			// enFoTrnCorrect
			else
			{
				Report->ExecTransType = FX::enFoTrnCancel;
				dwRes = m_ExecStorage.InsertTrade(Report, spTradeUpdate);
				if(dwRes == ERROR_SUCCESS)
				{
					m_spPubManager->PubTradeUpdate(spTradeUpdate);

					SwitchToThread();
					dwRes = ERROR_SUCCESS;
					Report->ExecTransType = FX::enFoTrnNew;
					Report->ExecID = bsCorrectionID.Detach();
					
					dwRes = m_ExecStorage.InsertTrade(Report, spTradeUpdate);
					if(dwRes == ERROR_SUCCESS)
						m_spPubManager->PubTradeUpdate(spTradeUpdate);
					else
						CComErrorWrapper::ThrowError(E_FAIL, _T("Failed to Correct Execution Report."));
				}
				else
					CComErrorWrapper::ThrowError(E_FAIL, _T("Failed to Cancel Trade for correction report."));
			}

			//if(!(nExecId = m_ExecStorage.Find(bsKeyExec)))
			//	CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Correct Execution report with unknown reference."));

			//spReport->PutId(nExecId);

			//_ExtractExecutionReport(Report, spReport);

			//m_ExecStorage.Update(spReport);

			//m_spPubManager->PubExecutionReport(spReport);
		}
		else
		{
			FXG::CFgOrderPtr spOrd(m_OrderStorage.Find(bsKey));

			if(!spOrd)
				if(!(spOrd = m_OrderStorage.Find(bsKeyOrig)))
					CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Execution report on unknown order."));

			if(IsReport)
			{
				dwRes = ERROR_SUCCESS;
				//spReport->PutOrderId(spOrd->m_nId);

				// Try to insert trade into db
				// if fails, flush trade in log, fire log message
				// else publish trade and send execution report
				dwRes = m_ExecStorage.InsertTrade(Report, spTradeUpdate);
				if(dwRes == ERROR_SUCCESS)
				{
					m_spPubManager->PubTradeUpdate(spTradeUpdate);

					SwitchToThread();
					spOrder->PutId(spOrd->m_nId);

					_ExtractExecutionReportToOrder(Report, spOrder);

					m_OrderStorage.UpdateStatus(spOrder);

					m_spPubManager->PubOrder(spOrder);
				}
			}
			else
			{
				spOrder->PutId(spOrd->m_nId);

				_ExtractExecutionReportToOrder(Report, spOrder);

				m_OrderStorage.Update(bsKeyOrig, spOrder);

				m_spPubManager->PubOrder(spOrder);
			}
		}

    }
    catch(_com_error& e)
    {
		return utils::ComError2ErrInfo(e, this, L"OnExecutionReport failed", __uuidof(IFixGServer));
    }

    return S_OK;
}

/////////////////////////////////////////////////////////////////////////////
STDMETHODIMP CFixGServer::OnOrderError(BSTR ClOrdID,	BSTR Description)
{
    try
    {
		CString strMessage;
		strMessage.Format(_T("Error in report, OrderID: %s (%s)"), (LPCTSTR)_bstr_t(ClOrdID),  (LPCTSTR)_bstr_t(Description));
		Fire_OnLogMessage(COleDateTime::GetCurrentTime(), enFgLogError, _bstr_t(strMessage));

		FXG::CFgOrderPtr spOrd(m_OrderStorage.Find(_bstr_t(ClOrdID)));

		if(spOrd)
		{
			IOrderPtr spOrder(CLSID_Order);
			spOrder->PutClOrdID(spOrd->m_strClOrdID);
			spOrder->PutId(spOrd->m_nId);
			spOrder->PutOrdStatus(spOrd->m_enConfirmedStatus);
			spOrder->PutText(Description);
			spOrder->PutExecType(enMsOsRejected);
			
			m_OrderStorage.UpdateStatus(spOrder, true);

			m_spPubManager->PubOrder(spOrder);
		}
	}
    catch(_com_error& e)
    {
		return utils::ComError2ErrInfo(e, this, L"OnOrderError failed", __uuidof(IFixGServer));
    }

    return S_OK;
}

/////////////////////////////////////////////////////////////////////////////
STDMETHODIMP CFixGServer::OnDisconnect(long ProviderType)
{
    try
    {
		MsFixProviderEnum enLoc = static_cast<MsFixProviderEnum>(ProviderType);

		ObjectLock Lock(this);

		TProvidersMap::iterator it;
		if ( (it = m_mapProviders.find(enLoc)) != m_mapProviders.end())
		{
			CFgProvider& aProv = it->second;

			aProv.m_bDisconnected = true;

			CString strMessage;
			strMessage.Format(_T("'%s' disconnected."), (LPCTSTR)aProv.m_bsProviderName);

			Fire_OnLogMessage(COleDateTime::GetCurrentTime(), enFgLogWarning, _bstr_t(strMessage));
		}
	}
    catch(_com_error& e)
    {
		return utils::ComError2ErrInfo(e, this, L"OnDisconnect failed", __uuidof(IFixGServer));
    }

    return S_OK;
}

/////////////////////////////////////////////////////////////////////////////
STDMETHODIMP CFixGServer::OnReLogon(long ProviderType)
{
	try
	{
		MsFixProviderEnum enLoc = static_cast<MsFixProviderEnum>(ProviderType);

		ObjectLock Lock(this);

		TProvidersMap::iterator it;
		if ( (it = m_mapProviders.find(enLoc)) != m_mapProviders.end())
		{
			CFgProvider& aProv = it->second;

			aProv.m_bDisconnected = false;

			CString strMessage;
			strMessage.Format(_T("'%s' successfully relogonned."), (LPCTSTR)aProv.m_bsProviderName);

			Fire_OnLogMessage(COleDateTime::GetCurrentTime(), enFgLogInfo, _bstr_t(strMessage));
		}
	}
	catch(_com_error& e)
	{
		return utils::ComError2ErrInfo(e, this, L"OnReLogon failed", __uuidof(IFixGServer));
	}

	return S_OK;
}

/////////////////////////////////////////////////////////////////////////////
STDMETHODIMP CFixGServer::OnLogonFailed(long ProviderType, BSTR Description)
{
	try
	{
		MsFixProviderEnum enLoc = static_cast<MsFixProviderEnum>(ProviderType);

		ObjectLock Lock(this);

		TProvidersMap::iterator it;
		if ( (it = m_mapProviders.find(enLoc)) != m_mapProviders.end())
		{
			CFgProvider& aProv = it->second;

			aProv.m_bDisconnected = true;

			CString strMessage;
			strMessage.Format(_T("'%s' logon failed : %s."), (LPCTSTR)aProv.m_bsProviderName, (LPCTSTR)_bstr_t(Description));

			Fire_OnLogMessage(COleDateTime::GetCurrentTime(), enFgLogError, _bstr_t(strMessage));
		}
	}
	catch(_com_error& e)
	{
		return utils::ComError2ErrInfo(e, this, L"OnReLogon failed", __uuidof(IFixGServer));
	}

	return S_OK;
}
/////////////////////////////////////////////////////////////////////////////
void	CFixGServer::_FillOrder(IOrder* pOrderSrc, FX::FoOrder* pOrderDst)
{
	if(!pOrderSrc || !pOrderDst)
		throw E_POINTER;

	SYMBOL_TYPE Type = pOrderSrc->GetSymbolType();
	if(enStOption == Type)
	{
		pOrderDst->Symbol = pOrderSrc->GetOptionRootSymbol().copy();
		pOrderDst->SecurityType = FX::enFoStOPT;
	}
	else if(enStStock == Type)
	{
		pOrderDst->Symbol = pOrderSrc->GetSymbol().copy();
		pOrderDst->SecurityType = FX::enFoStSTK;
	}
	else if(enStIndex == Type)
	{
		pOrderDst->Symbol = pOrderSrc->GetSymbol().copy();
		pOrderDst->SecurityType = FX::enFoStIDX;
	}

	pOrderDst->ExDestination = pOrderSrc->GetExDestination().copy();
	pOrderDst->Account = pOrderSrc->GetAccount().copy();
	pOrderDst->ExecBroker = pOrderSrc->GetExecBroker().copy();
	pOrderDst->StrikePrice = pOrderSrc->GetStrike();
	pOrderDst->ExpirationDate = pOrderSrc->GetExpiry();
	pOrderDst->IsCall = pOrderSrc->GetIsCall() > 0 ? VARIANT_TRUE : VARIANT_FALSE;
	pOrderDst->Side = pOrderSrc->GetIsBuy()? FX::enFoOsdBuy : FX::enFoOsdSell ;
	pOrderDst->OrderQty = pOrderSrc->GetQty();
	pOrderDst->Price = pOrderSrc->GetPrice();
	pOrderDst->OrdType = (FX::FoOrderTypeEnum)pOrderSrc->GetOrderType();

	pOrderDst->HandlInst = FX::enFoOhiAutoNoBrokerIntervention;
	pOrderDst->TimeInForce = FX::enFoTifDay;
	pOrderDst->PriceProtectionScope = FX::enFoPpsNone;
}

void	CFixGServer::_FillOrderCancelReplaceRequest(IOrder* pOrder, FX::FoOrderCancelReplaceRequest* pRequest)
{
	if(!pOrder || !pRequest)
		throw E_POINTER;

	pRequest->OrigClOrdID = pOrder->GetClOrdID().copy();
	SYMBOL_TYPE Type = pOrder->GetSymbolType();
	if(enStOption == Type)
	{
		pRequest->Symbol = pOrder->GetOptionRootSymbol().copy();
		pRequest->SecurityType = FX::enFoStOPT;
	}
	else if(enStStock == Type)
	{
		pRequest->Symbol = pOrder->GetSymbol().copy();
		pRequest->SecurityType = FX::enFoStSTK;
	}
	else if(enStIndex == Type)
	{
		pRequest->Symbol = pOrder->GetSymbol().copy();
		pRequest->SecurityType = FX::enFoStIDX;
	}
	pRequest->OrdType = (FX::FoOrderTypeEnum)pOrder->GetOrderType();

	pRequest->ExDestination = pOrder->GetExDestination().copy();
	pRequest->Account = pOrder->GetAccount().copy();
	pRequest->ExecBroker = pOrder->GetExecBroker().copy();
	pRequest->StrikePrice = pOrder->GetStrike();
	pRequest->ExpirationDate = pOrder->GetExpiry();
	pRequest->IsCall = pOrder->GetIsCall()> 0 ? VARIANT_TRUE : VARIANT_FALSE;
	pRequest->Side = pOrder->GetIsBuy()? FX::enFoOsdBuy : FX::enFoOsdSell ;
	pRequest->OrderQty = pOrder->GetQty();
	pRequest->Price = pOrder->GetPrice();

	pRequest->HandlInst = FX::enFoOhiManualBestExecution;
	pRequest->TimeInForce = FX::enFoTifDay;
}

void	CFixGServer::_FillOrderCancelRequest(IOrder* pOrder, FX::FoOrderCancelRequest* pRequest)
{
	if(!pOrder || !pRequest)
		throw E_POINTER;

	pRequest->OrigClOrdID = pOrder->GetClOrdID().copy();
	SYMBOL_TYPE Type = pOrder->GetSymbolType();
	if(enStOption == Type)
	{
		pRequest->Symbol = pOrder->GetOptionRootSymbol().copy();
		pRequest->SecurityType = FX::enFoStOPT;
	}
	else if(enStStock == Type)
	{
		pRequest->Symbol = pOrder->GetSymbol().copy();
		pRequest->SecurityType = FX::enFoStSTK;
	}
	else if(enStIndex == Type)
	{
		pRequest->Symbol = pOrder->GetSymbol().copy();
		pRequest->SecurityType = FX::enFoStIDX;
	}

	pRequest->ExecBroker = pOrder->GetExecBroker().copy();
	pRequest->StrikePrice = pOrder->GetStrike();
	pRequest->ExpirationDate = pOrder->GetExpiry();
	pRequest->IsCall = pOrder->GetIsCall() > 0 ? VARIANT_TRUE : VARIANT_FALSE;
	pRequest->Side = pOrder->GetIsBuy()? FX::enFoOsdBuy : FX::enFoOsdSell ;
	pRequest->OrderQty = pOrder->GetQty();

}

void	CFixGServer::_FillOrderStatusRequest(IOrder* pOrder, FX::FoOrderStatusRequest* pRequest)
{
	if(!pOrder || !pRequest)
		throw E_POINTER;

	pRequest->ClOrdID = pOrder->GetClOrdID().copy();
	SYMBOL_TYPE Type = pOrder->GetSymbolType();
	if(enStOption == Type)
	{
		pRequest->Symbol = pOrder->GetOptionRootSymbol().copy();
		pRequest->SecurityType = FX::enFoStOPT;
	}
	else if(enStStock == Type)
	{
		pRequest->Symbol = pOrder->GetSymbol().copy();
		pRequest->SecurityType = FX::enFoStSTK;
	}
	else if(enStIndex == Type)
	{
		pRequest->Symbol = pOrder->GetSymbol().copy();
		pRequest->SecurityType = FX::enFoStIDX;
	}

	pRequest->ExecBroker = pOrder->GetExecBroker().copy();
	pRequest->StrikePrice = pOrder->GetStrike();
	pRequest->ExpirationDate = pOrder->GetExpiry();
	pRequest->IsCall = pOrder->GetIsCall() > 0 ? VARIANT_TRUE : VARIANT_FALSE;
	pRequest->Side = pOrder->GetIsBuy()? FX::enFoOsdBuy : FX::enFoOsdSell ;
}

void	CFixGServer::_ExtractExecutionReport(FX::FoExecutionReport* pReportSrc, IExecutionReport* pReportDest)
{
	if(!pReportSrc || !pReportDest)
		throw E_POINTER;

	pReportDest->PutExecID(pReportSrc->ExecID);
	pReportDest->PutExecRefID(pReportSrc->ExecRefID);
	pReportDest->PutExecType((MsOrderStatusEnum)pReportSrc->ExecType);
	pReportDest->PutOrdStatus((MsOrderStatusEnum)pReportSrc->OrdStatus);
	pReportDest->PutLastPrice(pReportSrc->LastPrice);
	pReportDest->PutLastShares(pReportSrc->LastShares);
	pReportDest->PutAvgPrice(pReportSrc->AvgPrice);
	pReportDest->PutCumQty(pReportSrc->CumQuantity);
	pReportDest->PutLeavesQty(pReportSrc->LeavesQuantity);
	pReportDest->PutTransactTime(pReportSrc->TransactTime);
	pReportDest->PutText(pReportSrc->Text);
}

void	CFixGServer::_ExtractExecutionReportToOrder(FX::FoExecutionReport* pReportSrc, IOrder* pOrder)
{
	if(!pReportSrc || !pOrder)
		throw E_POINTER;

	pOrder->PutClOrdID(pReportSrc->ClOrdID);
	pOrder->PutExecType((MsOrderStatusEnum)pReportSrc->ExecType);
	pOrder->PutOrdStatus((MsOrderStatusEnum)pReportSrc->OrdStatus);
	pOrder->PutOrderType((MsOrderTypeEnum)pReportSrc->OrderType);
	pOrder->PutQty(pReportSrc->OrderQty);
	pOrder->PutPrice(pReportSrc->Price);
	pOrder->PutAvgPrice(pReportSrc->AvgPrice);
	pOrder->PutCumQty(pReportSrc->CumQuantity);
	pOrder->PutLeavesQty(pReportSrc->LeavesQuantity);
	pOrder->PutTransactTime(pReportSrc->TransactTime);
	pOrder->PutText(pReportSrc->Text);
}

void	CFixGServer::_ExtractOrderCancelReject(FX::FoOrderCancelReject* pReject, IOrder* pOrder)
{
	if(!pReject || !pOrder)
		throw E_POINTER;

	pOrder->PutExecType((MsOrderStatusEnum)enMsOsRejected);
	pOrder->PutOrdStatus((MsOrderStatusEnum)pReject->OrdStatus);
	pOrder->PutClOrdID(pReject->OrigClOrdID);

	pOrder->PutText(pReject->Text);
}

STDMETHODIMP CFixGServer::StartFix(void)
{
	{
		ObjectLock lock(this);
		if(m_enStatus == enFgSsStopped)
		{
			m_enStatus = enFgSsStartPending;
			SetEvent(m_hStartFixEvent);
		}
	}
	return S_OK;
}

STDMETHODIMP CFixGServer::StopFix(void)
{
	{
		ObjectLock lock(this);
		if(m_enStatus == enFgSsStarted)
		{
			m_enStatus = enFgSsStopPending;
			SetEvent(m_hStopFixEvent);
		}
	}
	return S_OK;
}

STDMETHODIMP CFixGServer::get_Status(FgServerStatusEnum* pVal)
{
	*pVal = _Status();

	return S_OK;
}

STDMETHODIMP CFixGServer::get_StartTime(DATE* pVal)
{
	try
	{
		*pVal = m_spSettingsMgr->StartFixTime();
	}
	catch (exception e) 
	{
		return Error((LPCWSTR)_bstr_t(e.what()), __uuidof(IFixGServer), E_FAIL);
	}

	return S_OK;
}

STDMETHODIMP CFixGServer::put_StartTime(DATE dtStart)
{
	COleDateTime odtStart(dtStart);
	try
	{
		m_spSettingsMgr->StartFixTime(odtStart);
	}
	catch (exception e) 
	{
		return Error((LPCWSTR)_bstr_t(e.what()), __uuidof(IFixGServer), E_FAIL);
	}

	if(m_spSettingsMgr->AutoStartStopFix())
		_QueueSetFixStartTimer(odtStart);

	return S_OK;
}

STDMETHODIMP CFixGServer::get_StopTime(DATE* pVal)
{
	try
	{
		*pVal = m_spSettingsMgr->StopFixTime();
	}
	catch (exception e) 
	{
		return Error((LPCWSTR)_bstr_t(e.what()), __uuidof(IFixGServer), E_FAIL);
	}

	return S_OK;
}

STDMETHODIMP CFixGServer::put_StopTime(DATE dtStop)
{
	COleDateTime odtStop(dtStop);
	try
	{
		m_spSettingsMgr->StopFixTime(odtStop);
	}
	catch (exception e) 
	{
		return Error((LPCWSTR)_bstr_t(e.what()), __uuidof(IFixGServer), E_FAIL);
	}

	if(m_spSettingsMgr->AutoStartStopFix())
		_QueueSetFixStopTimer(odtStop);

	return S_OK;
}

STDMETHODIMP CFixGServer::get_AutoStartStop(VARIANT_BOOL* pVal)
{
	try
	{
		*pVal = m_spSettingsMgr->AutoStartStopFix() ? VARIANT_TRUE : VARIANT_FALSE;
	}
	catch (exception e) 
	{
		return Error((LPCWSTR)_bstr_t(e.what()), __uuidof(IFixGServer), E_FAIL);
	}

	return S_OK;
}

STDMETHODIMP CFixGServer::put_AutoStartStop(VARIANT_BOOL bVal)
{
	try
	{
		bool boolVal = (bVal != VARIANT_FALSE);
		m_spSettingsMgr->AutoStartStopFix(boolVal);
	}
	catch (exception e) 
	{
		return Error((LPCWSTR)_bstr_t(e.what()), __uuidof(IFixGServer), E_FAIL);
	}

	if(bVal)
	{
		_QueueSetFixStartTimer(m_spSettingsMgr->StopFixTime());
		_QueueSetFixStopTimer(m_spSettingsMgr->StopFixTime());
	}
	else
	{
		CancelWaitableTimer(m_hStartFixTimer);
		CancelWaitableTimer(m_hStopFixTimer);
	}

	return S_OK;
}

UINT CFixGServer::Run()
{
	::CoInitializeEx(NULL,COINIT_MULTITHREADED);

	{
		_GetComputerInfo();

		HRESULT hr;
		bool bStop = false;
		while(!bStop)
		{
			vector<HANDLE> vecHandles;
			vecHandles.push_back(GetStopEventHandle());
			vecHandles.push_back(m_hStopFixEvent);
			vecHandles.push_back(m_hStartFixEvent);
			vecHandles.push_back(m_hGatewaysCheckTimer);
			vecHandles.push_back(m_hGatewaysRuns);
			vecHandles.push_back(m_hStartFixTimer);
			vecHandles.push_back(m_hStopFixTimer);

			DWORD dwRes = ::WaitForMultipleObjects(static_cast<DWORD>(vecHandles.size()),&vecHandles.front(),FALSE,INFINITE);

			switch(dwRes)
			{
				case WAIT_OBJECT_0: //stop
				{
					Fire_OnLogMessage(COleDateTime::GetCurrentTime(), enFgLogWarning, _bstr_t(_T("Quit from FixGateway App")));

					bStop = true;
				}
				case WAIT_OBJECT_0 + 6: //stop FIX by timer
				{
					if(!m_spSettingsMgr->AutoStartStopFix())
						break;
					_QueueSetFixStopTimer(m_spSettingsMgr->StopFixTime());
				}
				case WAIT_OBJECT_0 + 1: //stop FIX
				{
					Fire_OnLogMessage(COleDateTime::GetCurrentTime(), enFgLogWarning, _bstr_t(_T("Stopping FixGateway Server...")));

					try
					{
						IPubManagerPtr spPubManager;
						{
							ObjectLock Lock(this);
							spPubManager = m_spPubManager;
						}

						if (spPubManager)
						{
							IFixGatewayStatusPtr spStatus(__uuidof(FixGatewayStatus));

							spStatus->PutIP(_bstr_t(m_strIP));
							spStatus->PutHost(_bstr_t(m_strHost));
							spStatus->PutIsOnline(VARIANT_FALSE);
							spPubManager->PubFixGatewayStatus(spStatus);

							ISubManagerPtr spSubManager = spPubManager;
							try
							{
								if (spSubManager)
								{
									spSubManager->UnsubOrderMessages();
									spSubManager->UnsubFixGatewayStatus();
									spSubManager->UnsubFixGatewayStatusRequest();
								}
							} catch (_com_error&) { }

							CPubDispEvent::DispEventUnadvise(spPubManager);
						}

						_ClearProvidersMap();

						{
							ObjectLock lock(this);
							m_spPubManager = 0;
							m_enStatus = enFgSsStopped;
						}
						
						Fire_OnLogMessage(COleDateTime::GetCurrentTime(), enFgLogWarning, _bstr_t(_T("FixGateway Server stopped...")));
					}
					catch(_com_error&){}
				}
				break;
				case WAIT_OBJECT_0 + 5: //start FIX by timer
				{
					if(!m_spSettingsMgr->AutoStartStopFix())
						break;
					
					_QueueSetFixStartTimer(m_spSettingsMgr->StartFixTime());
				}
				case WAIT_OBJECT_0 + 2: //start FIX
				{					
					Fire_OnLogMessage(COleDateTime::GetCurrentTime(), enFgLogInfo, _bstr_t(_T("Starting FixGateway Server...")));
					try
					{
						m_spSettingsMgr->Init();
						m_OrderStorage.Init(m_spSettingsMgr);
						m_ExecStorage.Init(m_spSettingsMgr);

						m_OrderStorage.Clear();
						m_ExecStorage.Clear();

						m_OrderStorage.Load();
						m_ExecStorage.Load();
					}
					catch (exception&) 
					{
						_Status(enFgSsStopped);
						break;
					}

					try
					{
						IPubManagerPtr  spPubManager;
						{
							ObjectLock Lock(this);
							spPubManager = m_spPubManager;
						}
						
						if(spPubManager == NULL)
						{
							hr = spPubManager.CreateInstance(__uuidof (MsgManager));
							if (FAILED(hr))
								CComErrorWrapper::ThrowError(hr, _T("Failed to create MsgManager object."));

							hr = CPubDispEvent::DispEventAdvise(spPubManager);
							if(FAILED(hr))
								CComErrorWrapper::ThrowError(hr, _T("Failed to advise MsgManager object events."));
						}

						ISubManagerPtr spSubManager = spPubManager;
						if(spSubManager == NULL)
							CComErrorWrapper::ThrowError(E_NOINTERFACE, _T("Failed to get ISubManager interface."));

						spSubManager->SubFixGatewayStatus();
						spSubManager->SubFixGatewayStatusRequest();

						IFixGatewayStatusRequestPtr  spRequest(__uuidof (FixGatewayStatusRequest));
						spRequest->PutClientType(1);

						spPubManager->PubFixGatewayStatusRequest(spRequest);

						Fire_OnLogMessage(COleDateTime::GetCurrentTime(), enFgLogInfo, _bstr_t(_T("Try to find already runned FixGateway Servers...")));

						_QueueSetGatewaysCheckTimer(m_spSettingsMgr->CheckGatewaysTime() * 1000L);

						ObjectLock Lock(this);
						m_spPubManager = spPubManager;
					}
					catch(_com_error& )
					{
						_Status(enFgSsStopped);
						break;
					}
				}
				break;
				case WAIT_OBJECT_0 + 3: //check Timer
				{
					Fire_OnLogMessage(COleDateTime::GetCurrentTime(), enFgLogInfo, _bstr_t(_T("No one FixGateway Server starting at this time...")));
					try
					{
						CancelWaitableTimer(m_hGatewaysCheckTimer);

						_InitProvidersMap();

						ObjectLock Lock(this);

						ISubManagerPtr spSubManager = m_spPubManager;
						spSubManager->SubOrderMessages();

						m_mapEvents.clear();
						m_mapEvents.insert(make_pair(DISPID_OnRequestNewOrder, INVALID_HANDLE_VALUE));
						m_mapEvents.insert(make_pair(DISPID_OnAlterOrder, INVALID_HANDLE_VALUE));
						m_mapEvents.insert(make_pair(DISPID_OnCancelOrder, INVALID_HANDLE_VALUE));
						m_mapEvents.insert(make_pair(DISPID_OnRequestOrderStatus, INVALID_HANDLE_VALUE));

						TEventsMap::iterator it = m_mapEvents.begin();
						while (it != m_mapEvents.end())
						{
							HANDLE& hEvent = it->second;
							hEvent = ::CreateEvent(NULL, TRUE, FALSE, NULL);
							if (hEvent == INVALID_HANDLE_VALUE)
							{
								CComErrorWrapper::ThrowError(HRESULT_FROM_WIN32(GetLastError()), _T("Failed to create kernel object."));
							}
							it++;
						}
					}
					catch(_com_error&)
					{
						_Status(enFgSsStopped);
						break;
					}

					IPubManagerPtr spPubManager;
					{
						ObjectLock Lock(this);
						spPubManager = m_spPubManager;
					}

					if(spPubManager)
					{
						IFixGatewayStatusPtr spStatus(__uuidof(FixGatewayStatus));

						spStatus->PutIP(_bstr_t(m_strIP));
						spStatus->PutHost(_bstr_t(m_strHost));
						spStatus->PutIsOnline(VARIANT_TRUE);
						spPubManager->PubFixGatewayStatus(spStatus);
					}

					_Status(enFgSsStarted);

					Fire_OnLogMessage(COleDateTime::GetCurrentTime(), enFgLogInfo, _bstr_t(_T("FixGateway Server started...")));
				}
				break;
				case WAIT_OBJECT_0 + 4: //Gateway already running
				{
					CancelWaitableTimer(m_hGatewaysCheckTimer);

					_Status(enFgSsStopped);
					
					Fire_OnLogMessage(COleDateTime::GetCurrentTime(), enFgLogWarning, _bstr_t(_T("Can't start FixGateway Server.")));

					break;
				}
				break;
			}

			Fire_OnStatusChanged(_Status());
		}
	}
	::CoUninitialize();
	return 0;
}

void CFixGServer::_GetComputerInfo()
{
	ObjectLock lock(this);

	TCHAR	szLocalName[MAX_COMPUTERNAME_LENGTH + 1];
	u_long	nLocalNameSize = MAX_COMPUTERNAME_LENGTH + 1;

	if (!GetComputerName(szLocalName, &nLocalNameSize))
	{
		DWORD dwErr = GetLastError();
		return;
	}
	m_strHost = szLocalName;

	LPHOSTENT   lpHostEnt;

	SOCKADDR_IN addr_srv;

	USES_CONVERSION;
	if (NULL == (lpHostEnt = gethostbyname(T2A(szLocalName))))
	{
		return;
	}
	else
	{
		int i = 0;
		while(lpHostEnt->h_addr_list[i])
		{
			ZeroMemory(&addr_srv,sizeof(SOCKADDR_IN));
			CopyMemory(&addr_srv.sin_addr, 
				lpHostEnt->h_addr_list[i++],
				lpHostEnt->h_length);

			m_strIP += inet_ntoa(*(in_addr*)&addr_srv.sin_addr);
			m_strIP += _T(" ");
		}
	} 
}
bool CFixGServer::_QueueSetGatewaysCheckTimer(DWORD dwMilliseconds)
{
	LARGE_INTEGER liDueTime;
	liDueTime.QuadPart = ((ULONGLONG)dwMilliseconds) * 10000; // 100 ns discontinuity
	liDueTime.QuadPart *= -1;

	if(SetWaitableTimer(m_hGatewaysCheckTimer, &liDueTime, 0, NULL, NULL, 0) == TRUE)
		return true;
	else return false;
}

FgServerStatusEnum CFixGServer::_Status()
{
	ObjectLock lock(this);
	return m_enStatus;
}

void CFixGServer::_Status(FgServerStatusEnum enStatus)
{
	ObjectLock lock(this);
	m_enStatus = enStatus;
}

LPCTSTR	CFixGServer::_OrderStatusDescription(FX::FoOrderStatusEnum enStatus)
{
	switch(enStatus)
	{
		case FX::enFoOsNew:
			return _T("NEW");
		case FX::enFoOsPartiallyFilled:
			return _T("PART");
		case FX::enFoOsFilled:
			return _T("FILL");
		case FX::enFoOsDoneForDay:
			return _T("DFD");
		case FX::enFoOsCanceled:
			return _T("CXL");
		case FX::enFoOsReplaced:
			return _T("CXLR");
		case FX::enFoOsPendingCancelReplace:
			return _T("@CXLR");
		case FX::enFoOsStopped:
			return _T("STOP");
		case FX::enFoOsRejected:
			return _T("RJCT");
		case FX::enFoOsSuspended:
			return _T("SPND");
		case FX::enFoOsPendingNew:
			return _T("@NEW");
		case FX::enFoOsCalculating:
			return _T("CALC");
		case FX::enFoOsExpired:
			return _T("EXP");
	}

	return _T("???");
}

bool CFixGServer::_QueueSetFixStartTimer(const COleDateTime& dtStartTime)
{
	COleDateTime dtCurrentTime(COleDateTime::GetCurrentTime());
	COleDateTime dtStartDateTime;

	dtStartDateTime.SetDateTime(dtCurrentTime.GetYear(), dtCurrentTime.GetMonth(), dtCurrentTime.GetDay(),
		dtStartTime.GetHour(), dtStartTime.GetMinute(), dtStartTime.GetSecond());

	COleDateTimeSpan dtSpanOneDay(1L,0,0,0);
	COleDateTimeSpan dtSpanOneMinute(0L,0,1,0);
	if(dtCurrentTime + dtSpanOneMinute > dtStartDateTime)
		dtStartDateTime += dtSpanOneDay;


	SYSTEMTIME sysTime;
	dtStartDateTime.GetAsSystemTime(sysTime);
	FILETIME ftLoc, ftUTC;
	SystemTimeToFileTime(&sysTime, &ftLoc);
	LocalFileTimeToFileTime(&ftLoc,&ftUTC);

	LARGE_INTEGER liDueTime;
	memcpy(&liDueTime.QuadPart,&ftUTC,sizeof(FILETIME));

	if(SetWaitableTimer(m_hStartFixTimer, &liDueTime, 0, NULL, NULL, 0) == TRUE)
		return true;
	else return false;
}

bool CFixGServer::_QueueSetFixStopTimer(const COleDateTime& dtStopTime)
{
	COleDateTime dtCurrentTime(COleDateTime::GetCurrentTime());
	COleDateTime dtStopDateTime;

	dtStopDateTime.SetDateTime(dtCurrentTime.GetYear(), dtCurrentTime.GetMonth(), dtCurrentTime.GetDay(),
		dtStopTime.GetHour(), dtStopTime.GetMinute(), dtStopTime.GetSecond());

	COleDateTimeSpan dtSpanOneDay(1L,0,0,0);
	COleDateTimeSpan dtSpanOneMinute(0L,0,1,0);
	if(dtCurrentTime + dtSpanOneMinute > dtStopDateTime)
		dtStopDateTime += dtSpanOneDay;


	SYSTEMTIME sysTime;
	dtStopDateTime.GetAsSystemTime(sysTime);
	FILETIME ftLoc, ftUTC;
	SystemTimeToFileTime(&sysTime, &ftLoc);
	LocalFileTimeToFileTime(&ftLoc,&ftUTC);

	LARGE_INTEGER liDueTime;
	memcpy(&liDueTime.QuadPart,&ftUTC,sizeof(FILETIME));

	if(SetWaitableTimer(m_hStopFixTimer, &liDueTime, 0, NULL, NULL, 0) == TRUE)
		return true;
	else return false;
}

bool CFixGServer::_CheckIsWorkingTime()
{
	COleDateTime dtCurrentTime(COleDateTime::GetCurrentTime());
	COleDateTime dtStartTime(m_spSettingsMgr->StartFixTime());
	COleDateTime dtStopTime(m_spSettingsMgr->StopFixTime());
	COleDateTime dtStartDateTime, dtStopDateTime;

	dtStartDateTime.SetDateTime(dtCurrentTime.GetYear(), dtCurrentTime.GetMonth(), dtCurrentTime.GetDay(),
		dtStartTime.GetHour(), dtStartTime.GetMinute(), dtStartTime.GetSecond());

	dtStopDateTime.SetDateTime(dtCurrentTime.GetYear(), dtCurrentTime.GetMonth(), dtCurrentTime.GetDay(),
		dtStopTime.GetHour(), dtStopTime.GetMinute(), dtStopTime.GetSecond());

	if(dtStartDateTime < dtStopDateTime)
	{
		if(dtCurrentTime > dtStartDateTime && dtCurrentTime < dtStopDateTime)
			return true;
	}
	else
	{
		COleDateTimeSpan dtSpanOneDay(1L,0,0,0);

		if(dtCurrentTime > dtStartDateTime && dtCurrentTime < dtStopDateTime + dtSpanOneDay)
			return true;
		else if(dtCurrentTime > dtStartDateTime -  dtSpanOneDay && dtCurrentTime < dtStopDateTime)
			return true;
	}

	return false;
}
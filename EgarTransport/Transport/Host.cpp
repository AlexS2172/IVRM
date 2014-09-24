// Host.cpp : Implementation of CHost
#include "stdafx.h"
#include "Transport.h"
#include "Host.h"
#include "Router.h"
#include "Session.h"
#include "HostStatisticData.h"

/////////////////////////////////////////////////////////////////////////////
// CHost
CHost* g_pHost = NULL;

HRESULT CHost::FinalConstruct()
{
	CEgLibTraceManager::TraceText(LogError,_T("Host"), _T("Start FinalConstruct"));
	
	m_hDisconnectEvent.Attach(CreateEvent(NULL, FALSE, FALSE, NULL));
	if(m_hDisconnectEvent==NULL)
	{
		CEgLibTraceManager::Trace(LogError,_T("Host"), _T("Host can't initialize. Failed to create Disconnect Event %d"),GetLastError());
		return Error(L"Internal error.", IID_IHost, E_FAIL);
	}
	m_hConnectEvent.Attach(CreateEvent(NULL, FALSE, FALSE, NULL));
	if(m_hConnectEvent==NULL)
	{
		CEgLibTraceManager::Trace(LogError,_T("Host"), _T("Host can't initialize. Failed to create Connect Event %d"),GetLastError());
		return Error(L"Internal error.", IID_IHost, E_FAIL);
	}

	m_hReconnectTimeout.Attach(CreateWaitableTimer(NULL, FALSE, NULL));
	if(m_hReconnectTimeout==NULL)
	{
		CEgLibTraceManager::Trace(LogError,_T("Host"),_T("Host can't initialize. Failed to create Reconnect Timer %d"),GetLastError());
		return Error(L"Internal error.", IID_IHost, E_FAIL);
	}

	m_hSendTestRequest.Attach(CreateWaitableTimer(NULL, FALSE, NULL));
	if(m_hSendTestRequest==NULL)
	{
		CEgLibTraceManager::Trace(LogError,_T("Host"),_T("Host can't initialize. Failed to create Send TestRequest Timer %d"),GetLastError());
		return Error(L"Internal error.", IID_IHost, E_FAIL);
	}


	SetIsStopping(false);

	CEgLibTraceManager::TraceText(LogInfo,_T("Host"),_T("Start tcp driver"));
	DWORD dwErr;
	if (dwErr = m_Driver.Start(this, COINIT_MULTITHREADED))
	{
		CEgLibTraceManager::Trace(LogError,_T("Host"),_T("Host can't initialize. Failed to start TCP/IP Driver %d"),dwErr);
		return Error(L"Can't start TCP/IP Driver.", IID_IHost, E_FAIL);
	}
	
	if (dwErr = m_GrantedMessProcessor.Start())
	{
		CEgLibTraceManager::Trace(LogError,_T("Host"),_T("Host can't initialize. Failed to Start Granted Message Processor"));
		return Error(L"Transport can't start.", IID_IHost, HRESULT_FROM_WIN32(dwErr));
	}

	if (dwErr = m_BufferedMessProcessor.Start())
	{
		CEgLibTraceManager::TraceText(LogError,_T("Host"),_T("Host can't initialize. Failed to Start Buffered Message Processor"));
		return Error(L"Transport can't start.", IID_IHost, HRESULT_FROM_WIN32(dwErr));
	}

	if (dwErr = m_NotBuffMessProcessor.Start())
	{
		CEgLibTraceManager::TraceText(LogError,_T("Host"),_T("Host can't initialize. Failed to Start Notbuffered Message Processor"));
		return Error(L"Transport can't start.", IID_IHost, HRESULT_FROM_WIN32(dwErr));
	}

	if (dwErr = m_LocalMessProcessor.Start())
	{
		CEgLibTraceManager::TraceText(LogError,_T("Host"),_T("Host can't initialize. Failed to Start Local Message Processor"));
		return Error(L"Transport can't start.", IID_IHost, HRESULT_FROM_WIN32(dwErr));
	}

	InterlockedExchange((long*)&m_Router.m_Socket, INVALID_SOCKET);

	m_SessionSubjectDatabase.ClearAllStorage();
	SetIsLogoned(false);

	EndLogon();
	ResetAllCounters();
	memset(&m_tmLastMessageRec,0,sizeof(time_t));
	memset(&m_tmLogon,0,sizeof(time_t));
	m_bstrServerName = _T("");

	m_EventThread.Start();
	m_SessionEventThread.Start();

	CTpEventPtr pEvent (new CTpEvent());
	pEvent->m_enDispId = DISPID_IHostEventsOnDisabled;
	pEvent->m_bsParams = NULL;

	m_EventThread.PushEvent(pEvent);

	m_ConnTimeout = CRouter::GetSettingsFromReg(SETTINGS_CONNECT_TIMEOUT,CONNECT_TIMEOUT);			
	m_ConnLoseTimeout = CRouter::GetSettingsFromReg(SETTINGS_CONNECTION_LOSE_TIMEOUT,CONNECTION_LOSE_TIMEOUT);
	m_TestRequestTimeout = CRouter::GetSettingsFromReg(SETTINGS_TESTREQUEST_TIMEOUT,TESTREQUEST_TIMEOUT);
	m_StatisticTimer = CRouter::GetSettingsFromReg(SETTINGS_STATISTIC_TIMER,STATISTIC_TIMER);
	m_ReconnectTime = CRouter::GetSettingsFromReg(SETTINGS_RECONNECT_TIME,RECONNECT_TIME);


	if(dwErr = Start(NULL))
		return HRESULT_FROM_WIN32(dwErr);

	g_pHost = this;

	CEgLibTraceManager::TraceText(LogError,_T("Host"), _T("FinalConstruct completed"));

	return S_OK;
}

void CHost::FinalRelease()
{		
	CEgLibTraceManager::TraceText(LogError,_T("Host"), _T("Start FinalRelease"));

	if(IsLogoned())
		Logout();

	m_SessionSubjectDatabase.ClearAllStorage();
	
	m_GrantedMessProcessor.Stop();

	m_BufferedMessProcessor.Stop();

	m_NotBuffMessProcessor.Stop();

	m_LocalMessProcessor.Stop();

	m_listSessions.Clear();
	
	CTpEventPtr pEvent (new CTpEvent());
	pEvent->m_enDispId = DISPID_IHostEventsOnTransportStopped;
	pEvent->m_bsParams = NULL;

	m_EventThread.PushEvent(pEvent);
	
	Stop();
	
	m_Driver.Stop();

	CEgLibTraceManager::TraceText(LogInfo,_T("Host"),_T("Tcp/ip driver stopped."));

	m_hDisconnectEvent.Close();
	m_hConnectEvent.Close();
	m_hReconnectTimeout.Close();
	m_hSendTestRequest.Close();

	m_EventThread.Stop(true, 15);
	m_SessionEventThread.Stop(true, 15);

	g_pHost = NULL;

	CEgLibTraceManager::TraceText(LogError,_T("Host"), _T("FinalRelease completed"));
}


STDMETHODIMP CHost::CreateSession(ISession **ppSession)
{
	if(IsStopping())
		return E_FAIL;
	
	CEgLibTraceManager::TraceText(LogInfo,_T("Host"), _T("Start CreateSession"));
	
	{
		ObjectLock lock(this);

		if(IsStopping())
			return E_FAIL;

		CComObject<CSession>* pSession = NULL;
		CComObject<CSession>::CreateInstance(&pSession);
		pSession->AddRef();
		pSession->m_pHost = this;

		*ppSession = pSession;

		m_listSessions.InsertSession(pSession);
	}

	if(IsLogoned()) 
	{
		CTpEventPtr pEvent (new CTpEvent());
		pEvent->m_enDispId = DISPID_IHostEventsOnLogoned;
		pEvent->m_bsParams = ::SysAllocStringLen(m_bstrServerName, SysStringLen(m_bstrServerName));
		m_EventThread.PushEvent(pEvent);
	}
	else
	{
		CTpEventPtr pEvent (new CTpEvent());
		pEvent->m_enDispId = DISPID_IHostEventsOnLogout;
		pEvent->m_bsParams = ::SysAllocStringLen(m_bstrServerName, SysStringLen(m_bstrServerName));
		m_EventThread.PushEvent(pEvent);
	}

	CEgLibTraceManager::TraceText(LogInfo,_T("Host"), _T("CreateSession completed"));
	return S_OK;
}

STDMETHODIMP CHost::Logon(BSTR bsServerName)
{
	if(IsStopping())
		return E_FAIL;

	CEgLibTraceManager::TraceText(LogInfo,_T("Host"), _T("Start Logon"));

	if(IsLogoned()) /* we already logoned*/
	{
		
		CEgGuid SessionID;
		if(m_listSessions.First(SessionID))
		{
			do
			{
				CSession* pSession = m_listSessions.GetDataByKey(SessionID);
				
				pSession->AddRef();

				CTpSessionEventPtr pEvent ( new CTpSessionEvent());
				pEvent->m_enDispId = DISPID_ISessionEventsOnLogon;
				pEvent->m_bsParams = ::SysAllocStringLen(m_bstrServerName, SysStringLen(m_bstrServerName));
				pEvent->m_pSession = pSession;
				m_SessionEventThread.PushEvent(boost::dynamic_pointer_cast<CTpEvent>(pEvent));
				
			}while(m_listSessions.Next(SessionID));
		}

	
		CEgLibTraceManager::TraceText(LogInfo,_T("Host"), _T("Host already Logoned."));
		return S_OK;
	}

	if(!BeginLogon())
	{
		CEgLibTraceManager::TraceText(LogInfo,_T("Host"), _T("Host already start login procedure."));
		return S_FALSE;//Logon operation already begun.	
	}

	CTpEventPtr pEvent (new CTpEvent());
	pEvent->m_enDispId = DISPID_IHostEventsOnLogout;
	pEvent->m_bsParams = ::SysAllocStringLen(m_bstrServerName, SysStringLen(m_bstrServerName));
	m_EventThread.PushEvent(pEvent);

	ObjectLock lock(this);
	
	DWORD dwErr;
	
	try
	{
		m_bstrServerName = bsServerName;

		if (m_bstrServerName.length() == 0)
		{
			dwErr = CRouter::GetRouterHostName(m_bstrServerName);
				
			if (dwErr != 0 || m_bstrServerName.length() == 0)
			{
				CEgLibTraceManager::Trace(LogError,_T("Host"),_T("Logon failed. Router host name is not found in registry. Error %d"),dwErr);
				throw Error(L"Router host name is not found in registry.", IID_IHost, E_FAIL);
			}
		}
			

		unsigned short RouterPort = static_cast<unsigned short>(CRouter::GetSettingsFromReg(SETTINGS_NETWORK_ROUTER_PORT,ROUTER_PORT_NUMBER));

		dwErr = CEgTcpDriver::ResolveAddress(	m_bstrServerName,
												RouterPort,
												m_Router.m_RemoteAddr);
		if (dwErr != 0)
		{
			CEgLibTraceManager::Trace(LogError,_T("Host"),_T("Logon failed. Router host (%s) not found. Error %d"), (LPTSTR)m_bstrServerName, dwErr);
			throw Error(L"Router host not found.", IID_IHost, E_FAIL);
		}
		queue<SOCKADDR_IN> qAddr_srv;

		if (m_Router.m_RemoteAddr.sin_addr.S_un.S_addr == 16777343)
		{
			TCHAR	szLocalName[MAX_COMPUTERNAME_LENGTH + 1];
			u_long	nLocalNameSize = MAX_COMPUTERNAME_LENGTH + 1;

			if (!GetComputerName(szLocalName, &nLocalNameSize))
			{
				CEgLibTraceManager::Trace(LogError,_T("Host"),_T("Logon failed. GetComputerName failed. Error %d"), (LPTSTR)m_bstrServerName, GetLastError());
				throw E_FAIL;
			}
				
			USES_CONVERSION;

			dwErr = CEgTcpDriver::ResolveAddress2(	T2A(szLocalName),
														RouterPort,
														qAddr_srv);
			if (dwErr != 0)
			{
				CEgLibTraceManager::Trace(LogError,_T("Host"),_T("Logon failed. Router host (%s) not found. Error %d"), szLocalName, dwErr);
				throw Error(L"Router host not found.", IID_IHost, E_FAIL);
			}

			if(qAddr_srv.empty())
			{
					CEgLibTraceManager::Trace(LogError,_T("Host"),_T("Logon failed. Router host (%s) not found. Error %d"), (LPTSTR)m_bstrServerName, dwErr);
					throw Error(L"Router host not found.", IID_IHost, E_FAIL);
			}
		}
		else
		{
			dwErr = CEgTcpDriver::ResolveAddress2(	m_bstrServerName,
														RouterPort,
														qAddr_srv);
			if (dwErr != 0)
			{
				CEgLibTraceManager::Trace(LogError,_T("Host"),_T("Logon failed. Router host (%s) not found. Error %d"), (LPTSTR)m_bstrServerName, dwErr);
				throw Error(L"Router host not found.", IID_IHost, E_FAIL);
			}

		}
		
		while(!qAddr_srv.empty())
		{
			m_Router.m_RemoteAddr = qAddr_srv.front();
			qAddr_srv.pop();
			
			dwErr = m_Driver.Connect(m_Router.m_RemoteAddr);
			
			if (!dwErr) 
				break;
		}
		
		if (dwErr != 0)
		{
			CEgLibTraceManager::Trace(LogError,_T("Host"),_T("Logon failed. Can't connect to router. Error %d"),dwErr);
			throw Error(L"Can't connect to router. Please try later.", IID_IHost, E_FAIL);
		}

	}
	catch(HRESULT hr)
	{
		CEgLibTraceManager::TraceText(LogError,_T("Host"),_T("Logon failed"));
		EndLogon();
		if(m_Router.m_Socket != INVALID_SOCKET)
		{
			SOCKET oldSocket;
			InterlockedExchange((long*)&oldSocket, m_Router.m_Socket);
			InterlockedExchange((long*)&m_Router.m_Socket, INVALID_SOCKET);
			m_Driver.Close(oldSocket);
		}
		return hr;

	}
	CEgLibTraceManager::TraceText(LogInfo,_T("Host"), _T("Logon completed."));
	return S_OK;
}

STDMETHODIMP CHost::ReLogon()
{

	if(IsStopping())
		return E_FAIL;

	ObjectLock lock(this);

	CEgLibTraceManager::TraceText(LogInfo,_T("Host"), _T("Start ReLogon."));

	if(IsLogoned()) /* we already logined -> can't religon*/ 
	{
		CEgLibTraceManager::TraceText(LogInfo,_T("Host"), _T("Host already logined."));
		return Error(L"Host already logined.", IID_IHost, E_FAIL);
	}

	if(!BeginLogon())
	{
		CEgLibTraceManager::TraceText(LogInfo,_T("Host"), _T("Host already start logined procedure."));
		return Error(L"Logon operation already begun.", IID_IHost, E_FAIL);
	}
	// close old connection
	if (m_Router.m_Socket != INVALID_SOCKET)
	{
		SOCKET oldSocket;
		InterlockedExchange((long*)&oldSocket, m_Router.m_Socket);
		InterlockedExchange((long*)&m_Router.m_Socket, INVALID_SOCKET);
		m_Driver.Close(oldSocket);
	}

	DWORD dwErr;
	try
	{				
		dwErr = m_Driver.Connect(m_Router.m_RemoteAddr);
		if (dwErr != 0)
		{
			CEgLibTraceManager::Trace(LogError,_T("Host"),_T("ReLogon failed. Can't connect to router. Error %d"), dwErr);
			throw Error(L"Can't connect to router. Please try later.", IID_IHost, E_FAIL);
		}
	}
	catch(HRESULT hr)
	{
		CEgLibTraceManager::TraceText(LogError,_T("Host"),_T("ReLogon failed."));
		EndLogon();
		if(m_Router.m_Socket != INVALID_SOCKET)
		{
			SOCKET oldSocket;
			InterlockedExchange((long*)&oldSocket, m_Router.m_Socket);
			InterlockedExchange((long*)&m_Router.m_Socket, INVALID_SOCKET);
			m_Driver.Close(oldSocket);
		}
		QueueReconnectAttempt(m_ReconnectTime);
		return hr;
	}
	CEgLibTraceManager::TraceText(LogInfo,_T("Host"),_T("Relogon completed."));
	return S_OK;
}

STDMETHODIMP CHost::Logout()
{
	if(IsStopping())
		return E_FAIL;

	CEgLibTraceManager::TraceText(LogInfo,_T("Host"),_T("Start Logout."));

	{

		ObjectLock lock(this);

		
		ClearAllMessages();

		if(IsLogoned())
		{
			SetIsLogoned(false);
			// send logout message
			CEgTpLogoutMsg	Msg;

			Msg.m_enType			= enLogout;
			Msg.m_enMsgProp			= enMpUnspecified;
			Msg.m_HostID			= m_guidKey;
			Msg.m_ulMessageReceived	= IncRecMessNum();

			CEgPacket Packet;
			if(!Msg.Pack(Packet))
			{
				CEgLibTraceManager::TraceText(LogError,_T("Host"),_T("Error during Logout. Can't pack Logout message"));
				return E_FAIL;
			}

			u_long dwSize;
			void* pData = Packet.GetBuffer(dwSize);
								
			if (m_Router.m_Socket != INVALID_SOCKET && dwSize)
			{
				DWORD dwRes = m_Driver.Send(m_Router.m_Socket, pData, dwSize);
				if(dwRes)
					CEgLibTraceManager::Trace(LogInfo,_T("Host"),_T("Host can't send logout message. Error (%d)"),dwRes);
				else			
					CEgLibTraceManager::TraceText(LogInfo,_T("Host"),_T("Host send logout message."));
			}		
		}

		EndLogon();
	}

	CTpEventPtr pEvent (new CTpEvent());
	pEvent->m_enDispId = DISPID_IHostEventsOnLogout;
	pEvent->m_bsParams = ::SysAllocStringLen(m_bstrServerName, SysStringLen(m_bstrServerName));
	m_EventThread.PushEvent(pEvent);

	CEgGuid SessionID;
	if(m_listSessions.First(SessionID))
	{
		do
		{
			CSession* pSession = m_listSessions.GetDataByKey(SessionID);
				
			pSession->AddRef();
			CTpSessionEventPtr pEvent ( new CTpSessionEvent());
			pEvent->m_enDispId = DISPID_ISessionEventsOnLogoff;
			pEvent->m_bsParams = ::SysAllocStringLen(m_bstrServerName, SysStringLen(m_bstrServerName));
			pEvent->m_pSession = pSession;

			m_SessionEventThread.PushEvent(boost::dynamic_pointer_cast<CTpEvent>(pEvent));

		}while(m_listSessions.Next(SessionID));
	}
	else
	{
		CTpEventPtr pEvent (new CTpEvent());
		pEvent->m_enDispId = DISPID_IHostEventsOnDisabled;
		pEvent->m_bsParams = NULL;
		m_EventThread.PushEvent(pEvent);
	}

	{

		ObjectLock lock(this);

		ResetAllCounters();
	
		m_guidKey.Clear();

		if (m_Router.m_Socket != INVALID_SOCKET)
		{
			SOCKET oldSocket;
			InterlockedExchange((long*)&oldSocket, m_Router.m_Socket);
			InterlockedExchange((long*)&m_Router.m_Socket, INVALID_SOCKET);
			m_Driver.Close(oldSocket);
		}

		CEgLibTraceManager::TraceText(LogInfo,_T("Host"),_T("Logout completed."));
	}

	return S_OK;
}

STDMETHODIMP CHost::get_Status(HostStatusEnum *pVal)
{
	EgAssert(this);
	if(!pVal)
	{
		CEgLibTraceManager::TraceText(LogError,_T("Host"),_T("get_Status failed. Bad Pointer"));
		return E_POINTER;
	}	

	if(IsStopping())
		return E_FAIL;

	CEgLibTraceManager::TraceText(LogDebug,_T("Host"),_T("Start get_Status."));

	if(IsLogoned())
		*pVal = enHSOnline;
	else
	{
		if(!m_listSessions.Count())
			*pVal = enHSDisabled;
		else
			*pVal = enHSOffline;
	}
	
	CEgLibTraceManager::TraceText(LogDebug, _T("Host"), _T("get_Status completed."));
	return S_OK;
}

STDMETHODIMP CHost::get_Statistic(IHostStatisticData** ppStatistic)
{
	EgAssert(this);
	
	if(IsStopping())
		return E_FAIL;

	CEgLibTraceManager::TraceText(LogDebug,_T("Host"),_T("Start get_Statistic."));

	if(!ppStatistic)
	{
		CEgLibTraceManager::TraceText(LogError,_T("Host"),_T("get_Statistic failed. Bad Pointer"));
		return E_POINTER;
	}

	try
	{
		IHostStatisticDataPtr spStatistic;
		CComObject<CHostStatisticData>* pStat = NULL;
		if(FAILED(CComObject<CHostStatisticData>::CreateInstance(&pStat)))
			throw Error(L"Can't create HostStatisticData object.", IID_IHost, E_FAIL);

		if(pStat == NULL)
			throw Error(L"Can't create HostStatisticData object.", IID_IHost, E_FAIL);

		spStatistic.Attach(pStat, true);

		if(IsLogoned())
		{
			pStat->put_ConfirmedMessageNum(GetConfirmedMessNum());
			pStat->put_ReceivedMessageNum(GetRecMessNum());
			pStat->put_SentMessageNum(GetSendMessNum());
			pStat->put_UnsentMessageNum(GetUnsentMessNum());
			pStat->put_HostName(m_bstrServerName);
							
			// convert to file time
			LONGLONG llLogonTime = Int32x32To64(GetLogonTime(), 10000000) + 116444736000000000;

			FILETIME ftUTC;
			ftUTC.dwLowDateTime = (DWORD)llLogonTime;
			ftUTC.dwHighDateTime = static_cast<DWORD>(llLogonTime >> 32);
			SYSTEMTIME sTime;
			if(!FileTimeToSystemTime(&ftUTC,&sTime))
				throw Error(L"FileTimeToSystemTime failed.", IID_IHost, E_FAIL);
							
			DATE dtLogonTime;
			if(!SystemTimeToVariantTime(&sTime,&dtLogonTime))
				throw Error(L"SystemTimeToVariantTime failed.", IID_IHost, E_FAIL);

			pStat->put_LogonTime(dtLogonTime);
		}
		else
		{
			pStat->put_ConfirmedMessageNum(0);
			pStat->put_ReceivedMessageNum(0);
			pStat->put_SentMessageNum(0);
			pStat->put_UnsentMessageNum(0);
			pStat->put_LogonTime(DATE(0));
		}

		*ppStatistic = spStatistic.Detach();
	}
	catch(HRESULT hr)
	{
		CEgLibTraceManager::TraceText(LogError, _T("Host"),_T("get_Statistic failed"));
		return hr;
	}	

	CEgLibTraceManager::TraceText(LogDebug, _T("Host"),_T("get_Statistic completed"));
	return S_OK;
}
STDMETHODIMP CHost::get_RouterName(BSTR* pRouterName)
{
	EgAssert(this);

	if(IsStopping())
		return E_FAIL;

	CEgLibTraceManager::TraceText(LogDebug, _T("Host"), _T("Start get_RouterName."));

	if(!pRouterName)
	{
		CEgLibTraceManager::TraceText(LogError,_T("Host"),_T("get_RouterName failed. Bad Pointer"));
		return E_POINTER;
	}

	ObjectLock lock(this);


	try
	{
		*pRouterName = m_bstrServerName.copy();
	}
	catch(_com_error& er)
	{
		CEgLibTraceManager::TraceText(LogError,_T("Host"),_T("get_RouterName failed"));
		return er.Error();
	}	
	CEgLibTraceManager::TraceText(LogDebug, _T("Host"),_T("get_RouterName completed."));

	return S_OK;
}

STDMETHODIMP CHost::SendMessage(BSTR Subject, MsgPropertiesEnum MsgProp, 
		BSTR Message, CSession* pSession)
{
	EgAssert(this);

	if(IsStopping())
		return E_FAIL;

	CEgLibTraceManager::TraceText(LogDebug,_T("Host"),_T("Start SendMessage."));

	try
	{
		if(!IsLogoned() && MsgProp != enMpGuarantee)
		{
			CEgLibTraceManager::TraceText(LogInfo,_T("Host"),_T("SendMessage failed. Host offline"));
			return Error(L"Can't send message. Host offline.", IID_IHost, S_FALSE);
		}

		tstring	sSubject = _bstr_t(Subject);

		CEgTpDataMsgPtr pMsg(new CEgTpDataMsg);

			// Fill Message header
		pMsg->m_enType = enData;
		pMsg->m_enMsgProp = MsgProp;
			
		pMsg->m_strSubject = CT2A((LPTSTR)sSubject.c_str());		
		pMsg->m_blobData = Message;

		{
			set< CSession* > sSession;
			m_SessionSubjectDatabase.GetDataBySubjectWithMask(sSubject,sSession);

			for(set< CSession* >::iterator it = sSession.begin(); it != sSession.end(); it++)
			{
				if (memcmp(&((*it)->getSessionKey()), &(pSession->getSessionKey()), sizeof(GUID)) == 0)
					continue;

				CSession* pSess = *it;
				pSess->AddRef();
				pSess->PushMessage(pMsg);
				m_LocalMessProcessor.SetReceiverActive(pSess->getSessionKey());
				pSess->Release();
			}
		}

		switch(MsgProp)
		{
		case enMpGuarantee:
			{
				PushMessage(pMsg);
				if(IsLogoned())
					m_GrantedMessProcessor.SetReceiverActive(0);
			}
			break;
		case enMpBuffered:
			{
				PushMessage(pMsg);
				if(IsLogoned())
					m_BufferedMessProcessor.SetReceiverActive(0);
			}
			break;
		default:
			{
				PushMessage(pMsg);
				if(IsLogoned())
					m_NotBuffMessProcessor.SetReceiverActive(0);
			}
		}
	}
	catch(_com_error er)
	{
		CEgLibTraceManager::Trace(LogError,_T("Host"),_T("Error occured while host send message. HRESULT : (%d)"),er.Error());
		return Error(L"SendMessage failed.", IID_IHost, er.Error());
	}
	
	CEgLibTraceManager::TraceText(LogDebug, _T("Host"),_T("SendMessage completed."));
	return S_OK;
}

UINT CHost::Run()
{
	::CoInitializeEx(NULL, COINIT_MULTITHREADED);
	
	HANDLE hStopEvent = GetStopEventHandle();
	
	const HANDLE hEvents[] = {hStopEvent, m_hConnectEvent,  m_hDisconnectEvent,m_hReconnectTimeout, m_hSendTestRequest};

	while(true)
	{
		DWORD dwRes = WaitForMultipleObjects(sizeof(hEvents) / sizeof(HANDLE), 
			hEvents, FALSE, INFINITE );

		switch(dwRes)
		{
			case WAIT_OBJECT_0: /* Stop event */				
				CEgLibTraceManager::TraceText(LogInfo,_T("Host"),_T("Stop Event Fired."));
				CoUninitialize();
				return 0;
			case  WAIT_OBJECT_0 + 1:/* Server connected*/
				CEgLibTraceManager::TraceText(LogInfo,_T("Host"),_T("Connect Event Fired."));
				if(!IsLogoned())
					SendFirstMessage();
				break;
			case  WAIT_OBJECT_0 + 2:/* Server disconnected*/
				{
					CEgLibTraceManager::TraceText(LogInfo,_T("Host"),_T("Disconnect Event Fired."));
					CTpEventPtr pEvent (new CTpEvent());
					pEvent->m_enDispId = DISPID_IHostEventsOnLogout;
					pEvent->m_bsParams = ::SysAllocStringLen(m_bstrServerName, SysStringLen(m_bstrServerName));
					m_EventThread.PushEvent(pEvent);
					
					SetIsLogoned(false);				
	
					CEgGuid SessionID;
					if(m_listSessions.First(SessionID))
					{
						do
						{
							CSession* pSession = m_listSessions.GetDataByKey(SessionID);
							
							pSession->AddRef();
							CTpSessionEventPtr pEvent ( new CTpSessionEvent());
							pEvent->m_enDispId = DISPID_ISessionEventsOnLogoff;
							pEvent->m_bsParams = ::SysAllocStringLen(m_bstrServerName, SysStringLen(m_bstrServerName));
							pEvent->m_pSession = pSession;
							m_SessionEventThread.PushEvent(boost::dynamic_pointer_cast<CTpEvent>(pEvent));
				
						}while(m_listSessions.Next(SessionID));
					}
					else
					{
						CTpEventPtr pEvent (new CTpEvent());
						pEvent->m_enDispId = DISPID_IHostEventsOnDisabled;
						pEvent->m_bsParams = NULL;
						m_EventThread.PushEvent(pEvent);
						break;
					}
	
					ReLogon();
				}
				break;
			case WAIT_OBJECT_0 + 3:/* Time to relogon!*/
				{
					CEgLibTraceManager::TraceText(LogInfo,_T("Host"),_T("Host trying Reconnect to Router"));
					SetIsLogoned(false);
					EndLogon();
					SetEvent(m_hDisconnectEvent);
				}
				break;
			case WAIT_OBJECT_0 + 4:			
				CEgLibTraceManager::TraceText(LogInfo,_T("Host"),_T("Send TestRequest Event Fired."));
				if(IsLogoned())
				{
					SendTestRequest();
				}
				break;
			default:
				CEgLibTraceManager::TraceText(LogError,_T("Host"),_T("WaitForMultiple object error"));
				break;
					
		}
	}
}

BOOL WINAPI CHost::OnConnect(const CONNECTION_ID& Connection, BOOL bIncoming)
{
	u_long nID = Connection.m_RemoteAddr.sin_addr.S_un.S_addr;

	try
	{
		CEgLibTraceManager::Trace(LogInfo,_T("Host"),_T("OnConnect event. Router IP : (%s)"), (TCHAR*)CRouter::GetIPByAddr(nID));
	}
	catch(_com_error)
	{
	}

	m_SocketBuffer.Clear();

	if(IsStopping())
		return FALSE;

	if (m_Router.m_Socket != INVALID_SOCKET)
	{
		CEgLibTraceManager::TraceText(LogInfo,_T("Host"),_T("OnConnect failed. Host already connect to Router."));
		return FALSE;		// We're already connected to router (What's a s...???)
	}

	// Save socket connected to Router
	InterlockedExchange((long*)&m_Router.m_Socket, Connection.m_Socket);

	u_long	nLocalAddr = Connection.m_LocalAddr.sin_addr.S_un.S_addr;
	u_short nLocalPort = Connection.m_LocalAddr.sin_port;

	m_Router.m_LocalAddr.sin_addr.S_un.S_addr = nLocalAddr;
	m_Router.m_LocalAddr.sin_port = nLocalPort;

	SetEvent(m_hConnectEvent);

	CEgLibTraceManager::TraceText(LogDebug,_T("Host"),_T("OnConnect completed."));
	
	return TRUE;
}

void WINAPI CHost::OnClose(const CONNECTION_ID& Connection, const DWORD & dwError)
{
	u_long nID = Connection.m_RemoteAddr.sin_addr.S_un.S_addr;

	try
	{
		CEgLibTraceManager::Trace(LogInfo,_T("Host"),_T("OnClose event. dwError (%d). Router IP : (%s)"),(long) dwError,(TCHAR*)CRouter::GetIPByAddr(nID));
	}
	catch(_com_error)
	{
	}

	m_SocketBuffer.Clear();

	if(IsStopping())
		return;

	bool bIsThisRouter = (memcmp(&Connection.m_RemoteAddr, &m_Router.m_RemoteAddr,
																	sizeof(SOCKADDR_IN)) == 0);
	if (!bIsThisRouter)
	{
		CEgLibTraceManager::TraceText(LogInfo,_T("Host"),_T("OnClose event for unknown connection."));
		return;
	}

	if(m_Router.m_Socket != Connection.m_Socket && m_Router.m_Socket != INVALID_SOCKET)
	{
		CEgLibTraceManager::TraceText(LogInfo,_T("Host"),_T("OnClose event for unknown socket."));
		return;
	}

	InterlockedExchange((long*)&m_Router.m_Socket, INVALID_SOCKET);
	
	if(IsLogoned() || IsLogonStart())
	{
		EndLogon();
		QueueReconnectAttempt(m_ReconnectTime);
	}
	CEgLibTraceManager::TraceText(LogDebug, _T("Host"),_T("OnClose completed."));
}

void WINAPI CHost::OnConnectionFailed(const CONNECTION_ID& Connection)
{
	u_long nID = Connection.m_RemoteAddr.sin_addr.S_un.S_addr;

	try
	{
		CEgLibTraceManager::Trace(LogDebug, _T("Host"),_T("OnConnectionFailed event. Router IP : (%s)"),(TCHAR*)CRouter::GetIPByAddr(nID));
	}
	catch(_com_error)
	{
	}

	m_SocketBuffer.Clear();

	if(IsStopping())
		return;

	bool bIsThisRouter = (memcmp(&Connection.m_RemoteAddr, &m_Router.m_RemoteAddr,
																	sizeof(SOCKADDR_IN)) == 0);

	if (!bIsThisRouter)
	{
		CEgLibTraceManager::TraceText(LogInfo,_T("Host"),_T("OnConnectionFailed event for unknown connection."));
		return;
	}

	if(m_Router.m_Socket != Connection.m_Socket && m_Router.m_Socket != INVALID_SOCKET)
	{
		CEgLibTraceManager::TraceText(LogInfo,_T("Host"),_T("OnConnectionFailed event for unknown socket."));
		return;
	}

	InterlockedExchange((long*)&m_Router.m_Socket, INVALID_SOCKET);
	
	if(IsLogoned() || IsLogonStart())
	{
		EndLogon();
		QueueReconnectAttempt(m_ReconnectTime);
	}
	CEgLibTraceManager::TraceText(LogDebug, _T("Host"),_T("OnConnectionFailed completed."));
}

void WINAPI CHost::OnReceive(	const CONNECTION_ID&	Connection, 
								const LPVOID			pData,
								const DWORD				dwDataLen)
{
	EgAssert(this);

	if(IsStopping())
		return;

	CEgLibTraceManager::TraceText(LogDebug,_T("Host"),_T("OnReceive event."));

	if(m_Router.m_Socket != Connection.m_Socket)
	{
		CEgLibTraceManager::TraceText(LogError,_T("Host"),_T("OnReceive event from unknown socket."));
		return;
	}

	m_SocketBuffer.Insert(pData, dwDataLen);

	while(true)
	{
		// is there a room for MsgSize?
		if(m_SocketBuffer.GetLength() < sizeof(u_long))
			return;

		u_long nSize = *(u_long*)m_SocketBuffer.GetData();

		// is there a room for Msg?
		if(m_SocketBuffer.GetLength() < nSize)
			return;

		OnMessage(m_SocketBuffer.GetData(),nSize);
		
		m_SocketBuffer.ShiftLeft(nSize);

		if(IsStopping())
			return;
	}
	CEgLibTraceManager::TraceText(LogDebug,_T("Host"),_T("OnReceive completed."));
}

void CHost::OnMessage(	const LPVOID 		pData,
						const DWORD			dwDataLen)
{
	EgAssert(this);

	if(IsStopping())
		return;
	
	CEgLibTraceManager::TraceText(LogDebug,_T("Host"),_T("OnMessage event."));

	if(!IsLogoned())
	{

		switch(GetMessageType(pData))
		{
			case enLogonConfirm:
			{			
				CEgTpLogonConfirmMsg LogonCnfMsg;
				
				CEgPacket packet(pData,dwDataLen);
				if(!LogonCnfMsg.Unpack(packet))
				{
					CEgLibTraceManager::TraceText(LogError,_T("Host"),_T("Host can't unpack LogonConfirm message"));
					return;
				}

				ResendGrantedMessages(LogonCnfMsg.m_ulMessageReceived);

				if(m_guidKey != LogonCnfMsg.m_HostID.GetGUID())
				{
					CAutoLock	Lock(m_MembersLock);
					m_tmLogon = time(NULL);
				}
				m_guidKey = LogonCnfMsg.m_HostID.GetGUID();

				CEgLibTraceManager::Trace(LogInfo,_T("Host"),_T("Host Received logon confirm message. Router Receive:(%d) message from host. Host send (%d) message."), LogonCnfMsg.m_ulMessageReceived,GetSendMessNum());

				CTpEventPtr pEvent (new CTpEvent());
				pEvent->m_enDispId = DISPID_IHostEventsOnLogoned;
				pEvent->m_bsParams = ::SysAllocStringLen(m_bstrServerName, SysStringLen(m_bstrServerName));
				m_EventThread.PushEvent(pEvent);

				SetIsLogoned(true);

				SetIsLocalRouter(m_Router.m_LocalAddr.sin_addr.S_un.S_addr == m_Router.m_RemoteAddr.sin_addr.S_un.S_addr);

				m_GrantedMessProcessor.SetReceiverActive(0);
				m_BufferedMessProcessor.SetReceiverActive(0);
				m_NotBuffMessProcessor.SetReceiverActive(0);

				CEgGuid SessionID;
				if(m_listSessions.First(SessionID))
				{
					do
					{
						CSession* pSession = m_listSessions.GetDataByKey(SessionID);
							
						pSession->AddRef();
						
						CTpSessionEventPtr pEvent ( new CTpSessionEvent());
						pEvent->m_enDispId = DISPID_ISessionEventsOnLogon;
						pEvent->m_bsParams = ::SysAllocStringLen(m_bstrServerName, SysStringLen(m_bstrServerName));
						pEvent->m_pSession = pSession;
						m_SessionEventThread.PushEvent(boost::dynamic_pointer_cast<CTpEvent>(pEvent));

					}while(m_listSessions.Next(SessionID));
				}

				EndLogon();

				vector<tstring> vSubjects;
				if(m_SessionSubjectDatabase.GetAllSubjects(vSubjects))
				{
					while(!vSubjects.empty())
					{
						tstring Subject = vSubjects.back();
						vSubjects.pop_back();

						CEgTpUpdateSubjectMsgPtr pMsg(new CEgTpUpdateSubjectMsg);

						pMsg->m_enType = enSetSubject;
						pMsg->m_enMsgProp = enMpGuarantee;

						pMsg->m_strSubject = CT2A((LPTSTR)Subject.c_str());

						PushMessage(pMsg);
						m_GrantedMessProcessor.SetReceiverActive(0);
					}
				}


				QueueReconnectAttempt(m_ConnLoseTimeout);
				QueueSendTestRequestTimer(m_TestRequestTimeout);
			}
				break;
			case enLogout: // time to relogon !!!
			{
				CEgTpLogoutMsg	LogoutMsg;

				CEgPacket Packet(pData,dwDataLen);
				if(!LogoutMsg.Unpack(Packet))
				{
					CEgLibTraceManager::TraceText(LogError,_T("Host"),_T("Host can't unpack Logout message"));
					return;
				}
	
				if(!(LogoutMsg.m_HostID != m_guidKey))
				{
					CEgLibTraceManager::TraceText(LogInfo,_T("Host"),_T("Host Received Logout message "));

					Logout();
					QueueReconnectAttempt(m_ReconnectTime);
				}
			}
			break;
		}				
	}
	else
	{

		QueueReconnectAttempt(m_ConnLoseTimeout);
		QueueSendTestRequestTimer(m_TestRequestTimeout);

		switch(GetMessageType(pData))
		{
			case enLogonConfirm:
				CEgLibTraceManager::TraceText(LogInfo,_T("Host"),_T("Host Received UNEXPECTED logon confirm message."));
				break;
			case enData:
			{
				CEgTpDataMsgPtr pDataMsg(new CEgTpDataMsg);
				CEgPacket Packet(pData,dwDataLen);
				if(!pDataMsg->Unpack(Packet))
				{
					CEgLibTraceManager::TraceText(LogError,_T("Host"),_T("Host can't unpack Data message"));
					return;
				}
				ULongF ulMessSend,ulMessReceived;
				Packet >> ulMessSend;
				Packet >> ulMessReceived;

				OnDataMessage(pDataMsg, ulMessSend,ulMessReceived );
			}
			break;
			case enTestRequest:
			{	
				CEgTpTestRequestMsg TestRequestMsg;

				CEgPacket Packet(pData,dwDataLen);
				if(!TestRequestMsg.Unpack(Packet))
				{
					CEgLibTraceManager::TraceText(LogError,_T("Host"),_T("Host can't unpack TestRequest message"));
					return;
				}

				OnTestRequestMessage(TestRequestMsg.m_ulMessageReceived,TestRequestMsg.m_ulMessageSend);
			}
			break;
			case enHeartBeat:
			{	
				CEgTpHeartBeatMsg HeartBeatMsg;

				CEgPacket Packet(pData,dwDataLen);
				if(!HeartBeatMsg.Unpack(Packet))
				{
					CEgLibTraceManager::TraceText(LogError,_T("Host"),_T("Host can't unpack HeartBeat message"));
					return;
				}

				OnHeartBeatMessage(HeartBeatMsg.m_ulMessageReceived,HeartBeatMsg.m_ulMessageSend);
			}
			break;
			case enCheckSubject:
			{	
				CEgTpCheckSubjectMsgPtr pCheckSubjectMsg (new CEgTpCheckSubjectMsg);

				CEgPacket Packet(pData,dwDataLen);
				if(!pCheckSubjectMsg->Unpack(Packet))
				{
					CEgLibTraceManager::TraceText(LogError,_T("Host"),_T("Host can't unpack CheckSubject message"));
					return;
				}
				ULongF ulMessSend,ulMessReceived;
				Packet >> ulMessSend;
				Packet >> ulMessReceived;

				OnCheckSubjectMessage(pCheckSubjectMsg, ulMessSend, ulMessReceived);
			}
			break;
			case enLogout: // time to relogon !!!
			{
				CEgTpLogoutMsg	LogoutMsg;
	
				CEgPacket Packet(pData,dwDataLen);
				if(!LogoutMsg.Unpack(Packet))
				{
					CEgLibTraceManager::TraceText(LogError,_T("Host"),_T("Host can't unpack Logout message"));
					return;
				}

				if(!(LogoutMsg.m_HostID != m_guidKey))
				{
					CEgLibTraceManager::TraceText(LogInfo,_T("Host"),_T("Host Received Logout message "));

					Logout();
					QueueReconnectAttempt(m_ReconnectTime);
				}
			}
			break;
			default:
			{
				CEgLibTraceManager::Trace(LogError,_T("Host"),_T("Host can't process message of type (%d)"),GetMessageType(pData));
				return;
			}
		}
	}
	CEgLibTraceManager::TraceText(LogDebug,_T("Host"),_T("OnMessage completed."));
};

void CHost::OnDataMessage(CEgTpDataMsgPtr pDataMsg, u_long ulMessSend,u_long ulMessReceived )
{
	if(IsStopping())
		return;
	
	CEgLibTraceManager::TraceText(LogDebug,_T("Host"),_T("OnDataMessage."));

/*	if(pDataMsg->m_ulSize >= MAXIMUM_MESSAGE_LENGTH)// Check Data lenght
	{
		CEgLibTraceManager::TraceText(LogError,_T("Host"),_T("OnDataMessage failed. Too long data message"));
		return;
	}
*/	
	u_long ulHostMessReceive = GetRecMessNum();
	if(pDataMsg->m_enMsgProp == enMpGuarantee)
	{
		if(ulMessSend == ulHostMessReceive + 1)
		{
			IncRecMessNum();
						
			if( !(ulMessSend % 10000) ) // trace every 10000s message
				CEgLibTraceManager::Trace(LogInfo,_T("Host"),_T("Host receive expected granted data message. MessNum : (%d)"), ulMessSend);
			else
				CEgLibTraceManager::Trace(LogError,_T("Host"),_T("Host receive expected granted data message. MessNum : (%d)"), ulMessSend);
		}
		else if(ulMessSend > ulHostMessReceive + 1)
		{// GAPS detected
	
			CEgLibTraceManager::Trace(LogError,_T("Host"),_T("Host detect GAP of message !!! MessNum : (%d), MessReceive : (%d)"), ulMessSend, ulHostMessReceive);
			
			SendResendRequest(ulHostMessReceive);

			return;			
		}
		else
		{
			CEgLibTraceManager::Trace(LogInfo,_T("Host"),_T("Host already receive this message.  MessNum : (%d), MessReceive : (%d)"), ulMessSend, ulHostMessReceive);
			return;
		}
	}
	else if(ulMessSend > ulHostMessReceive)
	{// GAPS detected
		
		CEgLibTraceManager::Trace(LogError,_T("Host"),_T("Host detect GAP of message !!! MessNum : (%d), MessReceive : (%d)"), ulMessSend, ulHostMessReceive);
			
		SendResendRequest(ulHostMessReceive);
	}

	// here delete from storage receiving granted message
	RemoveConfirmedGrantedMessages(ulMessReceived);
	
	USES_CONVERSION;

	set< CSession* > sSession;
	m_SessionSubjectDatabase.GetDataBySubjectWithMask(A2T((LPSTR)pDataMsg->m_strSubject.GetStr()),sSession);

	for(set< CSession* >::iterator it = sSession.begin(); it != sSession.end(); it++)
	{
		CSession* pSession = *it;
		pSession->AddRef();
		pSession->PushMessage(pDataMsg);
		m_LocalMessProcessor.SetReceiverActive(pSession->getSessionKey());
		pSession->Release();
	}
	CEgLibTraceManager::TraceText(LogDebug,_T("Host"),_T("OnDataMessage completed."));
}

void CHost::OnCheckSubjectMessage(CEgTpCheckSubjectMsgPtr pCheckSubjectMsg,   u_long ulMessSend,u_long ulMessReceived )
{
	if(IsStopping())
		return;
	
	CEgLibTraceManager::TraceText(LogDebug,_T("Host"),_T("Start OnCheckSubjectMessage."));
	u_long ulHostMessReceive = GetRecMessNum();

	if(ulMessSend == ulHostMessReceive + 1)
	{
		IncRecMessNum();
	}
	else if(ulMessSend > ulHostMessReceive + 1)
	{// GAPS detected
	
		CEgLibTraceManager::Trace(LogError,_T("Host"),_T("Host detect GAP of message !!! MessNum : (%d), MessReceive : (%d)"), ulMessSend, ulHostMessReceive);
			
		SendResendRequest(ulHostMessReceive);

		return;			
	}

	// here delete from storage receiving granted message
	RemoveConfirmedGrantedMessages(ulMessReceived);

	CSession* pSession = m_listSessions.GetDataByKey(pCheckSubjectMsg->m_guidSession.GetGUID());
	if(!pSession)
	{
		CEgLibTraceManager::Trace(LogError,_T("Host"),_T("Host can't delivered CheckSubject message ! Subject (%s). Session doesn't exist."), pCheckSubjectMsg->m_strSubject.GetStr() );
		return;
	}
	pSession->PushMessage(pCheckSubjectMsg);
	m_LocalMessProcessor.SetReceiverActive(pSession->getSessionKey());
	CEgLibTraceManager::TraceText(LogDebug,_T("Host"),_T("OnCheckSubjectMessage completed."));
}

void CHost::SendResendRequest(u_long ulNumLastReceiveMess)
{
	if(IsStopping())
		return;

	CEgLibTraceManager::Trace(LogDebug,_T("Host"),_T("Start SendResendRequest."));

	CEgTpReSendMsg	Msg;
			
	Msg.m_enType			= enResend;
	Msg.m_enMsgProp			= enMpUnspecified;
	Msg.m_ulMessageReceived	= ulNumLastReceiveMess;
	Msg.m_HostID			= m_guidKey;
				
	CEgPacket Packet;
	if(!Msg.Pack(Packet))
	{
		CEgLibTraceManager::TraceText(LogError,_T("Host"),_T("Host can't pack Resend message"));
		return;
	}

	u_long dwSize;
	void* pData = Packet.GetBuffer(dwSize);

	if (m_Router.m_Socket != INVALID_SOCKET && dwSize)	
	{
		DWORD dwRes = m_Driver.Send(m_Router.m_Socket, pData, dwSize);
		if(dwRes)
			CEgLibTraceManager::Trace(LogError,_T("Host"),_T("Host can't send Resend message. Error(%d)"),dwRes);
		else
			CEgLibTraceManager::TraceText(LogInfo,_T("Host"),_T("Host send ReSend message."));
	}			
	CEgLibTraceManager::TraceText(LogDebug,_T("Host"),_T("SendResendRequest completed"));
}

void CHost::SendTestRequest()
{
	if(IsStopping())
		return;

	CEgLibTraceManager::TraceText(LogDebug,_T("Host"),_T("Start SendTestRequest"));

	CEgTpTestRequestMsg Msg;

	Msg.m_enType			= enTestRequest;
	Msg.m_enMsgProp			= enMpUnspecified;
	Msg.m_ulMessageReceived	= GetRecMessNum();
	Msg.m_ulMessageSend		= GetSendMessNum();
				
	CEgPacket Packet;
	if(!Msg.Pack(Packet))
	{
		CEgLibTraceManager::TraceText(LogError,_T("Host"),_T("Host can't pack TestRequest message"));
		return;
	}

	u_long dwSize;
	void* pData = Packet.GetBuffer(dwSize);

	if(m_Router.m_Socket != INVALID_SOCKET && dwSize)
	{
		DWORD dwRes = m_Driver.Send(m_Router.m_Socket,pData,dwSize);
		if(dwRes)
			CEgLibTraceManager::Trace(LogError,_T("Host"),_T("Host can't send TestRequest message. Error (%d)"),dwRes);
		else
			CEgLibTraceManager::TraceText(LogInfo,_T("Host"),_T("Host send TestRequest message"));
	}		
	QueueSendTestRequestTimer(m_TestRequestTimeout);
	
	CEgLibTraceManager::TraceText(LogDebug, _T("Host"),_T("SendTestRequest completed"));
}

void CHost::SendHeartBeatMsg()
{
	if(IsStopping())
		return;

	CEgLibTraceManager::TraceText(LogDebug, _T("Host"),_T("Start SendHeartBeatMsg"));

	CEgTpHeartBeatMsg Msg;

	Msg.m_enType			= enHeartBeat;
	Msg.m_enMsgProp			= enMpUnspecified;
	Msg.m_ulMessageReceived	= GetRecMessNum();
	Msg.m_ulMessageSend		= GetSendMessNum();
				
	CEgPacket Packet;
	if(!Msg.Pack(Packet))
	{
		CEgLibTraceManager::TraceText(LogError,_T("Host"),_T("Host can't pack Heart Beat message"));
		return;
	}

	u_long dwSize;
	void* pData = Packet.GetBuffer(dwSize);

	if(m_Router.m_Socket != INVALID_SOCKET && dwSize)
	{
		DWORD dwRes = m_Driver.Send(m_Router.m_Socket,pData,dwSize);
		if(dwRes)
			CEgLibTraceManager::Trace(LogError,_T("Host"),_T("Host can't send HeartBeat message. Error (%d)"),dwRes);
		else
			CEgLibTraceManager::TraceText(LogInfo,_T("Host"),_T("Host send HeartBeat message"));
	}

	CEgLibTraceManager::TraceText(LogDebug,_T("Host"),_T("SendHeartBeatMsg completed"));
}

void CHost::OnTestRequestMessage(u_long RouterMsgReceived,u_long RouterMsgSend)
{
	if(IsStopping())
		return;

	CEgLibTraceManager::TraceText(LogDebug,_T("Host"),_T("Start OnTestRequestMessage"));

	u_long ulHostMessReceive = GetRecMessNum();
	if(RouterMsgSend > ulHostMessReceive)
	{// GAPS detected
	
		CEgLibTraceManager::Trace(LogError,_T("Host"),_T("Host detect GAP of message !!! RouterMsgSend : (%d), HostMessReceive : (%d)"), RouterMsgSend, ulHostMessReceive);
			
		SendResendRequest(ulHostMessReceive);
	}
	else
	{
		SendHeartBeatMsg();
	}

	// here delete from storage receiving granted message
	RemoveConfirmedGrantedMessages(RouterMsgReceived);
	
	CEgLibTraceManager::TraceText(LogDebug,_T("Host"),_T("OnTestRequestMessage completed"));
}

void CHost::OnHeartBeatMessage(u_long RouterMsgReceived,u_long RouterMsgSend)
{
	if(IsStopping())
		return;

	CEgLibTraceManager::TraceText(LogDebug,_T("Host"),_T("Start OnHeartBeatMessage"));

	u_long ulHostMessReceive = GetRecMessNum();
	if(RouterMsgSend > ulHostMessReceive)
	{// GAPS detected
	
		CEgLibTraceManager::Trace(LogError,_T("Host"),_T("Host detect GAP of message !!! RouterMsgSend : (%d), HostMessReceive : (%d)"), RouterMsgSend, ulHostMessReceive);
			
		SendResendRequest(ulHostMessReceive);
	}

	// here delete from storage receiving granted message
	RemoveConfirmedGrantedMessages(RouterMsgReceived);

	CEgLibTraceManager::TraceText(LogDebug,_T("Host"),_T("OnHeartBeatMessage completed"));
}


bool CHost::CGrantedMessageProcessor::OnMessage(   /*in*/ CHost* pcxtReceiver,
						/*in*/ CEgTpBaseMessagePtr pMessage, u_long keyMessage )
{
	ATLASSERT(m_pcxtSender != NULL);
	EgAssert(this);

	if (m_pcxtSender == NULL)
		return false;

	if(m_pcxtSender->IsStopping())
		return false;
	
	CEgLibTraceManager::TraceText(LogDebug, _T("Host granted processor"),_T("Start OnMessage"));

	if(m_pcxtSender->IsLogoned())
	{
		SOCKET Socket = pcxtReceiver->m_Router.m_Socket;

		if (Socket != INVALID_SOCKET)
		{
			u_long SendMess = pcxtReceiver->GetSendMessNum();
			if(keyMessage != SendMess + 1)
			{
				CEgLibTraceManager::Trace(LogError,_T("Host granted processor"),_T("Can't send unexpected granted message. MessageNum (%d), SendMessage (%d)"),keyMessage,SendMess);
				return false;
			}

			CEgPacket pack;
			if(!pMessage->Pack(pack))
			{
				CEgLibTraceManager::Trace(LogError,_T("Host granted processor" ),_T("Host can't pack granted message Num : (%d)"),keyMessage);
				return false;
			}
			ULongF ulMessSend = keyMessage;
			ULongF ulMessageReceived = pcxtReceiver->GetRecMessNum();
			pack << ulMessSend;
			pack << ulMessageReceived;

			void* pData = NULL;
			u_long dwSize = 0;
			
			pData = pack.GetBuffer(dwSize);
			
			if(pData && dwSize)
			{

				DWORD dwRes = m_pcxtSender->m_Driver.Send(Socket, pData, dwSize);

				if(dwRes)
					CEgLibTraceManager::Trace(LogInfo,_T("Host granted processor"),_T("Error (%d) occured while Host send granted message (%d)"),dwRes, keyMessage);

				long count = 1;
				if(dwRes == WSAEWOULDBLOCK)
				{
					while(dwRes == WSAEWOULDBLOCK)
					{
						if(IsStopping())
							return false;

						CEgLibTraceManager::Trace(LogInfo,_T("Host granted processor"),_T("(%d) Try to resend granted message (%d)"),count, keyMessage);

						dwRes = m_pcxtSender->m_Driver.Send(Socket, pData, dwSize);
						count++;
					}
				}
				if(!dwRes)
				{
					if( !(keyMessage % 10000) ) // trace every 10000s message
						CEgLibTraceManager::Trace(LogDebug,_T("Host granted processor"),_T("Host successfully send (%d) granted message to router"), keyMessage);
					else
						CEgLibTraceManager::Trace(LogInfo,_T("Host granted processor"),_T("Host successfully send (%d) granted message to router"), keyMessage);


					CEgLibTraceManager::TraceText(LogDebug,_T("Host granted processor"),_T("OnMessage completed"));
					return true;
				}
			}
		}
	}

	CEgLibTraceManager::TraceText(LogDebug,_T("Host granted processor"),_T("OnMessage failed. Host offline"));
	return false;
}


bool CHost::CBufferedMessageProcessor::OnMessage(  /*in*/ CHost* pcxtReceiver,
						/*in*/ CEgTpBaseMessagePtr pMessage,u_long)
{
	ATLASSERT(m_pcxtSender != NULL);
	EgAssert(this);
	
	if (m_pcxtSender == NULL)
		return false;

	if(m_pcxtSender->IsStopping())
		return false;

	CEgLibTraceManager::TraceText(LogDebug, _T("Host buffered processor"), _T("Start OnMessage"));

	if(m_pcxtSender->IsLogoned())
	{		
		SOCKET Socket = pcxtReceiver->m_Router.m_Socket;

		if (Socket != INVALID_SOCKET)
		{

			CEgPacket pack;
			if(!pMessage->Pack(pack))
			{
				CEgLibTraceManager::TraceText(LogInfo,_T("Host buffered processor"),_T("Host can't pack buffered message."));
				return false;
			}

			ULongF ulMessageReceived = pcxtReceiver->GetRecMessNum();
			ULongF ulMessageSend = pcxtReceiver->GetSendMessNum();
			pack << ulMessageSend;
			pack << ulMessageReceived;

			void* pData = NULL;
			u_long dwSize = 0;
			
			pData = pack.GetBuffer(dwSize);
			
			if(pData && dwSize)
			{
				DWORD dwRes = m_pcxtSender->m_Driver.Send(Socket, pData, dwSize);
				if(dwRes)
					CEgLibTraceManager::Trace(LogInfo,_T("Host buffered processor"),_T("Host can't send buffered message. Error (%d)"),dwRes);
				else 
				{
					CEgLibTraceManager::Trace(LogError,_T("Host buffered processor"),_T("OnMessage completed"));
					return true;
				}
			}
		}		
	}

	CEgLibTraceManager::TraceText(LogDebug,_T("Host buffered processor"),_T("OnMessage failed. Host offline"));
	return false;
}

bool CHost::CNotBufferedMessageProcessor::OnMessage(/*in*/ CHost* pcxtReceiver,
						/*in*/ CEgTpBaseMessagePtr pMessage,u_long )
{
	ATLASSERT(m_pcxtSender != NULL);
	EgAssert(this);

	if (m_pcxtSender == NULL)
		return false;

	if(m_pcxtSender->IsStopping())
		return false;

	CEgLibTraceManager::TraceText(LogDebug,_T("Host notbuffered processor"),_T("Start OnMessage."));
	if(pcxtReceiver->IsLogoned())
	{	
		SOCKET Socket = pcxtReceiver->m_Router.m_Socket;

		if (Socket != INVALID_SOCKET)
		{				
			CEgPacket pack;

			if(!pMessage->Pack(pack))
			{
				CEgLibTraceManager::TraceText(LogInfo,_T("Host notbuffered processor"),_T("Host can't pack notbuffered message"));
				return false;
			}
			ULongF ulMessageReceived = pcxtReceiver->GetRecMessNum();
			ULongF ulMessageSend = pcxtReceiver->GetSendMessNum();
			pack << ulMessageSend;
			pack << ulMessageReceived;

			void* pData = NULL;
			u_long dwSize = 0;
			
			pData = pack.GetBuffer(dwSize);

			if(pData && dwSize)
			{

				DWORD dwRes = m_pcxtSender->m_Driver.Send(Socket, pData, dwSize);
				if(dwRes)
					CEgLibTraceManager::Trace(LogInfo, _T("Host notbuffered processor"),_T("Host can't send notbuffered message. Error (%d)"),dwRes);
				else 
				{
					CEgLibTraceManager::TraceText(LogError,_T("Host notbuffered processor"),_T("OnMessage completed."));
					return true;
				}						
			}
		}		
	}

	CEgLibTraceManager::TraceText(LogDebug,_T("Host notbuffered processor"),_T("OnMessage failed. Host Offline"));
	return false;
}

bool CHost::CLocalMessageProcessor::OnMessage(  /*in*/ CSession* pcxtReceiver,
												/*in*/ CEgTpBaseMessagePtr pMessageHdr, u_long )
{
	ATLASSERT(m_pcxtSender != NULL);
	EgAssert(this);
	
	CEgLibTraceManager::TraceText(LogDebug,_T("Host local processor"),_T("Start OnMessage"));

	if(pMessageHdr->m_enType == enData)
	{
		CEgTpDataMsgPtr pMessage = boost::dynamic_pointer_cast<CEgTpDataMsg>(pMessageHdr);

		if (m_pcxtSender == NULL || pMessage == NULL)
			return false;

		if(m_pcxtSender->IsStopping())
			return false;

		try
		{
			_bstr_t bsMessage;
			bsMessage.Attach((BSTR)pMessage->m_blobData);

			pcxtReceiver->Fire_OnMessage(_bstr_t(pMessage->m_strSubject.GetStr()), bsMessage);
		}
		catch(_com_error er)
		{
			CEgLibTraceManager::Trace(LogInfo,_T("Host local processor"),_T("OnMessage failed. Error %d"), er.Error());
			return false;
		}
	}
	else if(pMessageHdr->m_enType == enCheckSubject)
	{
		CEgTpCheckSubjectMsgPtr pMessage = boost::dynamic_pointer_cast<CEgTpCheckSubjectMsg>(pMessageHdr);

		if (m_pcxtSender == NULL)
			return false;

		if(m_pcxtSender->IsStopping())
			return false;

		try
		{			
			pcxtReceiver->Fire_OnSubjectStatus(_bstr_t(pMessage->m_strSubject.GetStr()), pMessage->m_ulNumSubscribers);
		}
		catch(_com_error er)
		{
			CEgLibTraceManager::Trace(LogInfo,_T("Host local processor"),_T("OnMessage failed. Error %d"), er.Error());
			return false;
		}

	}
	CEgLibTraceManager::TraceText(LogDebug,_T("Host local processor"),_T("OnMessage completed"));

	return true;
}

STDMETHODIMP CHost::Subscribe(BSTR Subject, CSession * pSession)
{
	if(IsStopping())
		return E_FAIL;

	CEgLibTraceManager::TraceText(LogInfo, _T("Host"), _T("Start Subscribe"));
	try
	{
 		tstring sSubject = CW2T(Subject);
 		
 		m_SessionSubjectDatabase.RegisterDataSubject(sSubject, pSession);
		//push set subject message in Granted Message Processor
		CEgTpUpdateSubjectMsgPtr pMessage(new CEgTpUpdateSubjectMsg);

		pMessage->m_enType = enSetSubject;
		pMessage->m_enMsgProp = enMpGuarantee;

		pMessage->m_strSubject = CT2A((LPTSTR)sSubject.c_str());

		CEgLibTraceManager::Trace(LogInfo,_T("Host"),_T("Host pushing SetSubject message. Subject : (%s)"), sSubject.c_str());
		PushMessage(pMessage);

		if(IsLogoned())
			m_GrantedMessProcessor.SetReceiverActive(0);

	}
	catch(_com_error er)
	{
		CEgLibTraceManager::Trace(LogError,_T("Host"),_T("Exception occured while host send Subscribe message. HRESULT : (%d)"),er.Error());
		return Error(L"Subscribe failed.", IID_IHost, er.Error());
	}
	CEgLibTraceManager::TraceText(LogInfo,_T("Host"),_T("Subscribe completed"));

	return S_OK;
}

STDMETHODIMP CHost::Unsubscribe(BSTR Subject, CSession * pSession)
{
	if(IsStopping())
		return E_FAIL;

	CEgLibTraceManager::TraceText(LogInfo,_T("Host"),_T("Start Unsubscribe"));

	try
	{

		tstring sSubject = CW2T(Subject);
		
		if(m_SessionSubjectDatabase.UnregisterDataSubject(sSubject,pSession))
		{
			//push remove subject message in Granted Message Processor
			CEgTpUpdateSubjectMsgPtr pMessage (new CEgTpUpdateSubjectMsg);


			pMessage->m_enType = enRemoveSubject;
			pMessage->m_enMsgProp = enMpGuarantee;

			pMessage->m_strSubject = CT2A(sSubject.c_str());

			CEgLibTraceManager::Trace(LogInfo,_T("Host"),_T("Host pushing RemoveSubject message. Subject : (%s)"), sSubject.c_str());
			
			PushMessage(pMessage);


			if(IsLogoned())
				m_GrantedMessProcessor.SetReceiverActive(0);

		}
	}
	catch(_com_error er)
	{
		CEgLibTraceManager::Trace(LogError,_T("Host"),_T("Exception occured while host send Unsubsribe message. HRESULT : (%d)"),er.Error());
		return Error(L"Unsubscribe failed.", IID_IHost, er.Error());
	}
	CEgLibTraceManager::Trace(LogInfo,_T("Host"),_T("Unsubscribe completed"));

	return S_OK;
}

STDMETHODIMP CHost::CheckSubjectStatus(BSTR Subject, CSession * pSession)
{
	if(IsStopping())
		return E_FAIL;

	CEgLibTraceManager::TraceText(LogInfo, _T("Host"), _T("Start CheckSubjectStatus"));

	try
	{
		tstring sSubject = CW2T(Subject);
		
		CEgTpCheckSubjectMsgPtr pMessage(new CEgTpCheckSubjectMsg);

		pMessage->m_enType      = enCheckSubject;
		pMessage->m_enMsgProp   = enMpGuarantee;
		pMessage->m_guidSession = pSession->getSessionKey();

		pMessage->m_strSubject = CT2A(sSubject.c_str());

		CEgLibTraceManager::Trace(LogInfo,_T("Host"),_T("Host pushing CheckSubjectStatus message. Subject : (%s)"), sSubject.c_str());
			
		PushMessage(pMessage);

		if(IsLogoned())
			m_GrantedMessProcessor.SetReceiverActive(0);

	}
	catch(_com_error er)
	{
		CEgLibTraceManager::Trace(LogError,_T("Host"),_T("Exception occured while host send CheckSubject message. HRESULT : (%d)"),er.Error());
		return Error(L"Unsubscribe failed.", IID_IHost, er.Error());
	}
	CEgLibTraceManager::TraceText(LogInfo,_T("Host"),_T("CheckSubjectStatus completed"));

	return S_OK;
}

void CHost::DestroySession(CSession * pSession)
{ 
	vector<tstring> vSubjects;
	
	{
		ObjectLock lock(this);

		if(IsStopping())
			return;

		CEgLibTraceManager::TraceText(LogInfo,_T("Host"),_T("Start DestroySession"));

		m_SessionSubjectDatabase.UnregisterData(pSession,vSubjects);

		bool IsEmpty = false;

		pSession->ClearAllMessages();
		m_listSessions.Remove(pSession->getSessionKey());
	}

	if(!m_listSessions.Count())
	{
		Logout();
		CTpEventPtr pEvent (new CTpEvent());
		pEvent->m_enDispId = DISPID_IHostEventsOnDisabled;
		pEvent->m_bsParams = NULL;
		m_EventThread.PushEvent(pEvent);
	}
	else
	{
		while(!vSubjects.empty())
		{
			tstring Subject = vSubjects.back();
			vSubjects.pop_back();

			//push remove subject message in Granted Message Processor
			CEgTpUpdateSubjectMsgPtr pMessage (new CEgTpUpdateSubjectMsg);

			pMessage->m_enType = enRemoveSubject;
			pMessage->m_enMsgProp = enMpGuarantee;

			pMessage->m_strSubject = CT2A((LPTSTR)Subject.c_str());

			CEgLibTraceManager::Trace(LogInfo,_T("Host"),_T("Host push RemoveSubject message. Subject : (%s)"), Subject.c_str());

			PushMessage(pMessage);
			
			if(IsLogoned())
				m_GrantedMessProcessor.SetReceiverActive(0);
		}
	}
	CEgLibTraceManager::TraceText(LogInfo,_T("Host"),_T("DestroySession completed"));
}
void CHost::SendFirstMessage()
{
	if(IsStopping())
		return;

	CEgLibTraceManager::TraceText(LogInfo,_T("Host"),_T("Start SendFirstMessage"));

	CEgGuid emptyGUID;

	try
	{
		if(!memcmp(&m_guidKey,&emptyGUID, sizeof(GUID)))// need logon
		{
			CEgTpLogonMsg	Msg;
				
			Msg.m_enType			= enLogon;
			Msg.m_enMsgProp			= enMpUnspecified;
			Msg.m_ulMessageReceived	= 0;

			CEgPacket Packet;
			if(!Msg.Pack(Packet))
			{
				CEgLibTraceManager::TraceText(LogError,_T("Host"),_T("Host can't pack Logon message"));
				return;
			}

			u_long dwSize;
			void* pData = Packet.GetBuffer(dwSize);
				
			if (m_Router.m_Socket == INVALID_SOCKET || !dwSize )
			{
				CEgLibTraceManager::TraceText(LogError,_T("Host"),_T("Host can't send first message. Connection failed."));
				throw Error(L"Connection failed.", IID_IHost, E_FAIL);
			}
			DWORD dwErr = m_Driver.Send(m_Router.m_Socket, pData, dwSize);
			if(dwErr !=0)
			{
				CEgLibTraceManager::Trace(LogError,_T("Host"),_T("Host can't send logon message. Error %d"),dwErr);
				throw Error(L"Can't send logon message.", IID_IHost, E_FAIL);
			}
			CEgLibTraceManager::TraceText(LogInfo,_T("Host"),_T("Host send logon message."));
		}
		else // need relogon
		{

			CEgTpReLogonMsg	Msg;
				
			Msg.m_enType			= enReLogon;
			Msg.m_enMsgProp			= enMpUnspecified;
			Msg.m_HostID			= m_guidKey;
			Msg.m_ulMessageReceived	= GetRecMessNum();
				
			CEgPacket Packet;
			if(!Msg.Pack(Packet))
			{
				CEgLibTraceManager::TraceText(LogError,_T("Host"),_T("Host can't pack ReLogon message"));
				return;
			}

			u_long dwSize;
			void* pData = Packet.GetBuffer(dwSize);

			if (m_Router.m_Socket == INVALID_SOCKET || !dwSize)
			{
				CEgLibTraceManager::TraceText(LogError,_T("Host"),_T("Host can't send first message. Connection failed."));
				throw Error(L"Connection failed.", IID_IHost, E_FAIL);
			}
			DWORD dwErr = m_Driver.Send(m_Router.m_Socket, pData, dwSize);
			if(dwErr !=0)
			{
				CEgLibTraceManager::Trace(LogError,_T("Host"),_T("Host can't send relogin. Error %d"), dwErr);
				throw Error(L"Can't send relogin message.", IID_IHost, E_FAIL);
			}
			CEgLibTraceManager::Trace(LogInfo,_T("Host"),_T("Host send relogin message. MessReceived (%d)"),Msg.m_ulMessageReceived);
		}
	}
	catch(HRESULT)
	{
		CEgLibTraceManager::TraceText(LogError,_T("Host"),_T("SendFirstMessage failed"));
		EndLogon();
		if(m_Router.m_Socket != INVALID_SOCKET)
		{
			SOCKET oldSocket;
			InterlockedExchange((long*)&oldSocket, m_Router.m_Socket);
			InterlockedExchange((long*)&m_Router.m_Socket, INVALID_SOCKET);
			m_Driver.Close(oldSocket);
		}
		QueueReconnectAttempt(m_ReconnectTime);
	}
	CEgLibTraceManager::TraceText(LogInfo,_T("Host"),_T("SendFirstMessage completed"));
}

void CHost::QueueReconnectAttempt(long lTimer)
{
	CEgLibTraceManager::TraceText(LogInfo,_T("Host"),_T("Start QueueReconnectAttempt"));
	LARGE_INTEGER liDueTime;
	liDueTime.QuadPart = ((ULONGLONG)lTimer) * 1000 * 10000; // 100 ns discontinuity
	liDueTime.QuadPart *= -1;

	if (!SetWaitableTimer(m_hReconnectTimeout, &liDueTime, 0, NULL, NULL, 0))
	{
		CEgLibTraceManager::Trace(LogError,_T("Host"),_T("Failed to queue reconnection request : waitable timer error (%d)"),GetLastError());
	}
	CEgLibTraceManager::TraceText(LogInfo,_T("Host"),_T("QueueReconnectAttempt completed"));
}

void CHost::QueueSendTestRequestTimer(long lTimer)
{
	CEgLibTraceManager::TraceText(LogDebug,_T("Host"),_T("Start QueueSendTestRequestTimer"));
	LARGE_INTEGER liDueTime;
	liDueTime.QuadPart = ((ULONGLONG)lTimer) * 1000 * 10000; // 100 ns discontinuity
	liDueTime.QuadPart *= -1;

	if (!SetWaitableTimer(m_hSendTestRequest, &liDueTime, 0, NULL, NULL, 0))
	{
		CEgLibTraceManager::Trace(LogError,_T("Host"),_T("Failed to queue Send TestRequest timer : waitable timer error (%d)"),GetLastError());
	}
	CEgLibTraceManager::TraceText(LogDebug,_T("Host"),_T("QueueSendTestRequestTimer completed"));
}

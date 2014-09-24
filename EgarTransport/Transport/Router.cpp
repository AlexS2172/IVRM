// Router.cpp : Implementation of CRouter
#include "stdafx.h"
#include "Transport.h"
#include "Router.h"

#include "EgLib\EgLibDbg.h"
#include "EgLib\EgLibReg.h"
#include ".\router.h"
#include "HostStatisticData.h"

CRouter* g_pRouter = NULL;

/************************************************************************************************
	Object construction and destruction handling
************************************************************************************************/
HRESULT CRouter::FinalConstruct()
{

	//DebugBreak();
	CEgLibTraceManager::Trace(LogError,_T("Router"),_T("Start FinalConstruct"));
	
//	m_hStopEvent.Attach(::CreateEvent(NULL, FALSE, FALSE, NULL));
//	if(m_hStopEvent == NULL)
//	{
//		CEgLibTraceManager::Trace(LogError,_T("Router"),_T("FinalConstruct failed. Error create Stop event. Error %d"),GetLastError());
//		return Error(L"Internal error.", IID_IRouter, E_FAIL);
//	}

	m_hCheckHostsConnTimer.Attach(::CreateWaitableTimer(NULL, FALSE, NULL));
	if (m_hCheckHostsConnTimer == NULL)
	{
		CEgLibTraceManager::Trace(LogError,_T("Router"),_T("FinalConstruct failed. Error create Check Host connections timer. Error %d"),GetLastError());
		return Error(L"Internal error.", IID_IRouter, E_FAIL);
	}
	m_bIsStarted = FALSE;
		
	g_pRouter = this;

    m_hServiceStatus = NULL;
    m_status.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
    m_status.dwCurrentState = SERVICE_STOPPED;
    m_status.dwControlsAccepted = SERVICE_ACCEPT_STOP;
    m_status.dwWin32ExitCode = 0;
    m_status.dwServiceSpecificExitCode = 0;
    m_status.dwCheckPoint = 0;
    m_status.dwWaitHint = 0;

	m_EventThread.Start();

	CEgLibTraceManager::Trace(LogError,_T("Router"),_T("FinalConstruct completed"));
	return S_OK;		
}

void CRouter::FinalRelease()
{
	CEgLibTraceManager::Trace(LogError,_T("Router"),_T("Start FinalRelease"));
	CTpEventPtr pEvent (new CTpEvent());
	pEvent->m_enDispId = DISPID_IRouterEventsOnTransportStopped;
	pEvent->m_bsParams = NULL;
	m_EventThread.PushEvent(pEvent);
	
	ObjectLock lock(this);
	

//	m_hStopEvent.Close();
	m_hCheckHostsConnTimer.Close();

	m_EventThread.Stop(true, 150);
	g_pRouter = NULL;
	Stop();

	CEgLibTraceManager::Trace(LogError,_T("Router"),_T("FinalRelease completed"));
}


/////////////////////////////////////////////////////////////////////////////
// IRouter interface imlementation

STDMETHODIMP CRouter::get_HostStatistic(/*in*/ BSTR bsHostIP, /*[out, retval]*/ IHostStatisticData** ppStatistic)
{
	if(IsStopping())
		return E_FAIL;

	CEgLibTraceManager::Trace(LogDebug,_T("Router"),_T("Start get_HostStatistic"));

	if(!ppStatistic)
	{
		CEgLibTraceManager::Trace(LogError,_T("Router"),_T("get_HostStatistic failed. Bad pointer"));
		return E_POINTER;
	}	
	try
	{
		CEgSmartPtr<CEgConnection> spConnection = m_listConnections.FindConnectionByIP(bsHostIP);
		if((!spConnection) == false)
		{

			CComObject<CHostStatisticData>* pHostStatistics = NULL; 
			IHostStatisticDataPtr pStatistic;
			
			if(FAILED(CComObject<CHostStatisticData>::CreateInstance(&pHostStatistics)))
				throw Error(L"Can't create HostStatisticData object.", IID_IRouter, E_FAIL);

			if(pStatistic == NULL)
				throw Error(L"Can't create HostStatisticData object.", IID_IRouter, E_FAIL);

			pStatistic.Attach(pHostStatistics, TRUE);
			CEgSmartPtr<CEgHostInfo> spHostInfo = spConnection->GetHostInfo();

			if(!spHostInfo)
				throw Error(L"No one active hosts with this IP.", IID_IRouter, E_FAIL);


			pHostStatistics->put_ConfirmedMessageNum(spHostInfo->GetConfirmedMessNum());
			pHostStatistics->put_ReceivedMessageNum(spHostInfo->GetRecMessNum());
			pHostStatistics->put_SentMessageNum(spHostInfo->GetSendMessNum());
			pHostStatistics->put_UnsentMessageNum(spHostInfo->GetUnsentMessNum());
								
			// convert to file time
			LONGLONG llLogonTime = Int32x32To64(spHostInfo->GetLogonTime(), 10000000) + 116444736000000000;

			FILETIME ftUTC;
			ftUTC.dwLowDateTime = DWORD(llLogonTime);
			ftUTC.dwHighDateTime = static_cast<DWORD>(llLogonTime >> 32);
			SYSTEMTIME sTime;
			if(!FileTimeToSystemTime(&ftUTC,&sTime))
			{
				pStatistic->Release();
				throw Error(L"FileTimeToSystemTime failed.", IID_IRouter, E_FAIL);
			}
								
			DATE dtLogonTime;
			if(!SystemTimeToVariantTime(&sTime,&dtLogonTime))
				throw Error(L"SystemTimeToVariantTime failed.", IID_IRouter, E_FAIL);

			pHostStatistics->put_LogonTime(dtLogonTime);

			*ppStatistic = pStatistic.Detach();
		}
		else
			throw Error(L"No one active hosts with this IP.", IID_IRouter, E_FAIL);
	}
	catch(HRESULT hr)
	{
		CEgLibTraceManager::Trace(LogError,_T("Router"),_T("get_HostStatistic failed"));
		return hr;
	}	
	CEgLibTraceManager::Trace(LogDebug,_T("Router"),_T("get_HostStatistic completed"));
	return S_OK;	
}

STDMETHODIMP CRouter::get_ActiveHosts(/*[out, retval]*/LPSAFEARRAY* ppHostsIPList)
{
	if(IsStopping())
		return E_FAIL;
	
	CEgLibTraceManager::Trace(LogDebug,_T("Router"),_T("Start get_ActiveHosts"));
	if(!ppHostsIPList)
	{
		CEgLibTraceManager::Trace(LogError,_T("Router"),_T("get_ActiveHosts failed. Bad Pointer"));
		return E_POINTER;
	}

	if(*ppHostsIPList)
	{
		CEgLibTraceManager::Trace(LogError,_T("Router"),_T("get_ActiveHosts failed. Invalid argument"));
		return E_INVALIDARG;
	}
	
	try
	{
		long count = m_listConnections.Count();
		if(!count)
			throw std::exception(_T("No one active hosts"));

		SAFEARRAYBOUND rgsabound[1];
		rgsabound[0].lLbound = 0;
		rgsabound[0].cElements = count;
		*ppHostsIPList = SafeArrayCreate(VT_BSTR,1,rgsabound);
		long idx[1] = {0};

		SOCKET sSocket;
		if(m_listConnections.First(sSocket))
		{
			do
			{
				CEgSmartPtr<CEgConnection> spConnection = m_listConnections.GetDataByKey(sSocket);

				if((!spConnection) == false)
				{
					u_long nID = spConnection->GetConnectionID().m_RemoteAddr.sin_addr.S_un.S_addr;
					if(idx[0] >= count)
					{
						rgsabound[0].lLbound = 0;
						rgsabound[0].cElements = ++count;
						_com_util::CheckError(SafeArrayRedim(*ppHostsIPList, rgsabound));
					}
					_com_util::CheckError(SafeArrayPutElement(*ppHostsIPList, idx, GetIPByAddr(nID).copy()));					
					idx[0]++;
				}				
			}while(m_listConnections.Next(sSocket));
		}
		
		rgsabound[0].lLbound = 0;
		rgsabound[0].cElements = idx[0];
		_com_util::CheckError(SafeArrayRedim(*ppHostsIPList,rgsabound));
	}
	catch (std::exception& ex)
	{
		CEgLibTraceManager::Trace(LogError,_T("Router"),"Get ActiveHosts: %s",ex.what());
		return E_FAIL;
	}
	catch(HRESULT hr)
	{
		CEgLibTraceManager::Trace(LogError,_T("Router"),_T("get_ActiveHosts failed"));
		return hr;
	}	
	catch(_com_error& er)
	{
		CEgLibTraceManager::Trace(LogError,_T("Router"),_T(" get_ActiveHosts failed"));
		return er.Error();
	}	

	CEgLibTraceManager::Trace(LogDebug,_T("Router"),_T("get_ActiveHosts completed"));

	return S_OK;	
}

/************************************************************************************************
	There is CEgTcpDriver callback interface implementation
************************************************************************************************/
BOOL WINAPI CRouter::OnConnect(const CONNECTION_ID& Connection, BOOL bIncome)
{	
	if(IsStopping())
		return FALSE;

	try
	{
		u_long nID = Connection.m_RemoteAddr.sin_addr.S_un.S_addr;
	
		CEgLibTraceManager::Trace(LogInfo,_T("Router"),_T("Router OnConnect event. Client IP : (%s)"),(TCHAR*)GetIPByAddr(nID));

		if(!bIncome)
			return FALSE;// router not connected to any servers !

		CEgSmartPtr<CEgConnection> pConnection = m_listConnections.CreateConnection(Connection,&m_Driver);

		if(!pConnection)
			return FALSE;

		pConnection->m_Buffer.Clear();

		CTpEventPtr pEvent (new CTpEvent());
		pEvent->m_enDispId = DISPID_IRouterEventsOnClientConnected;
		pEvent->m_bsParams = ::SysAllocStringLen(GetIPByAddr(nID), SysStringLen(GetIPByAddr(nID)));
		m_EventThread.PushEvent(pEvent);
	}
	catch(_com_error er)
	{
		CEgLibTraceManager::Trace(LogError,_T("Router"),_T("Exception occured while router process OnConnect event. HRESULT : (%d)"),er.Error());
		return FALSE;
	}

	return TRUE;
};

void WINAPI CRouter::OnConnectionFailed(const CONNECTION_ID& Connection)
{
	if(IsStopping())
		return;

	SOCKET sSocket = Connection.m_Socket;

	CEgSmartPtr<CEgConnection> pConnection = m_listConnections.GetDataByKey(sSocket);

	if(!pConnection)
		return;

	try
	{
		pConnection->ClearAllMessages();
		
		u_long nID = Connection.m_RemoteAddr.sin_addr.S_un.S_addr;
		CEgSmartPtr<CEgHostInfo> pHostInfo = pConnection->GetHostInfo();

		if ((!pHostInfo) == false)
		{
			CEgLibTraceManager::Trace(LogInfo,_T("Router"),_T("Router OnConnectionFailed event.  Client IP : (%s) .Host : {%.8X-...}"),(TCHAR*)GetIPByAddr(nID), pHostInfo->getHostKey().Data1);

			switch (pHostInfo->GetStatus())
			{
			case CEgHostInfo::enConnected:
			case CEgHostInfo::enOnline:
				pHostInfo->SetStatus(CEgHostInfo::enDisconnected);
				pHostInfo->SetConnection(NULL);
				m_listConnections.Remove(sSocket);
				break;

			default:
				break;
			}
		}
		else
		{
			CEgLibTraceManager::Trace(LogInfo,_T("Router"),_T("Router OnConnectionFailed event. Client IP : (%s)."),(TCHAR*)GetIPByAddr(nID));

			m_listConnections.Remove(sSocket);
			return; // ??? unspecified
		}

		CTpEventPtr pEvent (new CTpEvent());
		pEvent->m_enDispId = DISPID_IRouterEventsOnClientDisconnected;
		pEvent->m_bsParams = ::SysAllocStringLen(GetIPByAddr(nID), SysStringLen(GetIPByAddr(nID)));
		m_EventThread.PushEvent(pEvent);
	}
	catch(_com_error er)
	{
		CEgLibTraceManager::Trace(LogError,_T("Router"),_T("Exception occured while router process OnConnectionFailed event. HRESULT : (%d)"),er.Error());
	}
};

void WINAPI CRouter::OnClose(const CONNECTION_ID& Connection, const DWORD & dwError)
{
	if(IsStopping())
		return;

	SOCKET sSocket = Connection.m_Socket;

	CEgSmartPtr<CEgConnection> pConnection = m_listConnections.GetDataByKey(sSocket);

	if(!pConnection)
		return;
	try
	{

		pConnection->ClearAllMessages();
	
		u_long nID = Connection.m_RemoteAddr.sin_addr.S_un.S_addr;

		CEgSmartPtr<CEgHostInfo> pHostInfo = pConnection->GetHostInfo();

		if ((!pHostInfo) == false)
		{
			CEgLibTraceManager::Trace(LogInfo,_T("Router"),_T("Router OnClose event. dwError (%d). Client IP : (%s), Host : {%.8X-...}"),dwError,(TCHAR*)GetIPByAddr(nID), pHostInfo->getHostKey().Data1);

			switch (pHostInfo->GetStatus())
			{
			case CEgHostInfo::enConnected:
			case CEgHostInfo::enOnline:
				pHostInfo->SetStatus(CEgHostInfo::enDisconnected);
				pHostInfo->SetConnection(NULL);
				m_listConnections.Remove(sSocket);
				break;

			default:
				break;
			}
		}
		else
		{
			CEgLibTraceManager::Trace(LogInfo,_T("Router"),_T("Router OnClose event. dwError (%d), Client IP : (%s)"),dwError,(TCHAR*)GetIPByAddr(nID));

			m_listConnections.Remove(sSocket);
			return; // ??? unspecified
		}

		CTpEventPtr pEvent (new CTpEvent());
		pEvent->m_enDispId = DISPID_IRouterEventsOnClientDisconnected;
		pEvent->m_bsParams = ::SysAllocStringLen(GetIPByAddr(nID), SysStringLen(GetIPByAddr(nID)));
		m_EventThread.PushEvent(pEvent);
	}
	catch(_com_error er)
	{
		CEgLibTraceManager::Trace(LogError,_T("Router"),_T("Exception occured while router process OnClose event. HRESULT : (%d)"),er.Error());
	}
};

void WINAPI CRouter::OnReceive(	const CONNECTION_ID&	Connection, 
								const LPVOID 			pData,
								const DWORD				dwDataLen)
{
	EgAssert(this);

	if(IsStopping())
		return;

	CEgLibTraceManager::Trace(LogInfo,_T("Router"),_T("OnReceive event."));

	SOCKET sSocket = Connection.m_Socket;
	u_long nID = Connection.m_RemoteAddr.sin_addr.S_un.S_addr;

	CEgSmartPtr<CEgConnection> pConnection = m_listConnections.GetDataByKey(sSocket);
			
	if(!pConnection)
	{
		CEgLibTraceManager::Trace(LogError,_T("Router"),_T("OnReceive event error. Unknown connection."));
		return;// serious error connection doesn't exist !!!
	}
	pConnection->m_Buffer.Insert(pData, dwDataLen);

	while(true)
	{
		// is there a room for MsgSize?
		if(pConnection->m_Buffer.GetLength() < sizeof(u_long))
			return;

		u_long nSize = *(u_long*)pConnection->m_Buffer.GetData();

/*		if(nSize > MAXIMUM_MESSAGE_LENGTH)
		{
			CEgLibTraceManager::Trace(LogError,_T("Router"),"Router received too long message from IP : %s . MsgSize = %d. Connection will be closed.",(TCHAR*)GetIPByAddr(nID),nSize);
		
			CEgSmartPtr<CEgHostInfo> pHostInfo = pConnection->GetHostInfo();
			if(pHostInfo)
			{
				pHostInfo->SetStatus(CEgHostInfo::enDisconnected);
				pHostInfo->SetConnection(NULL);
			}
			m_listConnections.Remove(sSocket,true);
			return;
		}
*/		
		// is there a room for Msg?
		if(pConnection->m_Buffer.GetLength() < nSize)
			return;

		OnMessage(Connection,pConnection->m_Buffer.GetData(),nSize);
		
		pConnection->m_Buffer.ShiftLeft(nSize);

		if(WaitForSingleObject(GetStopEventHandle(), 0) == WAIT_OBJECT_0)
		{// need to stop service -> exit and set event again
			SetEvent(GetStopEventHandle());
			return;
		}
	}
	CEgLibTraceManager::Trace(LogDebug,_T("Router"),_T("OnReceive event completed."));
}

void CRouter::OnMessage(const CONNECTION_ID&	Connection,
						const LPVOID			pData,
						const DWORD				dwDataLen)
{
	EgAssert(this);

	if(IsStopping())
		return;

	CEgLibTraceManager::Trace(LogDebug,_T("Router"),_T("Start OnMessage"));

	SOCKET sSocket = Connection.m_Socket;
	u_long nID = Connection.m_RemoteAddr.sin_addr.S_un.S_addr;

	CEgSmartPtr<CEgConnection> pConnection = m_listConnections.GetDataByKey(sSocket);
			
	if(!pConnection)
	{
		CEgLibTraceManager::Trace(LogError,_T("Router"),_T("OnMessage failed. Unknown connection"));
		return;// serious error connection doesn't exist !!!
	}
	MsgTypeEnum MsgType = GetMessageType(pData);

	if(MsgType == enTestRequest) // answer on TestRequest message !
	{

		CEgSmartPtr<CEgHostInfo> pHostInfo = pConnection->GetHostInfo();
		if(!pHostInfo)
			return;
	
		CEgTpHeartBeatMsg Msg;
		Msg.m_enMsgProp = (MsgPropertiesEnum)enMpUnspecified;
		Msg.m_enType = (MsgTypeEnum)enHeartBeat;

		Msg.m_ulMessageReceived = pHostInfo->GetRecMessNum();
		Msg.m_ulMessageSend = pHostInfo->GetSendMessNum();

		CEgPacket Packet;
		if(!Msg.Pack(Packet))
			CEgLibTraceManager::Trace(LogError,_T("Router"),_T("Router can't pack HeartBeat message for host {%.8X-...}."),pHostInfo->getHostKey().Data1);

		u_long nSize;
		void* pData = Packet.GetBuffer(nSize);
		
		if(sSocket != INVALID_SOCKET && nSize)
		{
			DWORD dwRes = m_Driver.Send(sSocket, pData, nSize);
			if(dwRes)
				CEgLibTraceManager::Trace(LogError,_T("Router"),_T("Router can't send HeartBeat message to host {%.8X-...}. Error (%d)"),pHostInfo->getHostKey().Data1,dwRes);
		}
	}

	CEgTpBaseMessagePtr pBaseMessage;
	switch(MsgType)
	{
		case enLogon:
		{
			CEgTpLogonMsgPtr pLogonMsg(new CEgTpLogonMsg());
			
	
			CEgPacket Packet(pData, dwDataLen);
			if(!pLogonMsg->Unpack(Packet))
			{
				CEgLibTraceManager::Trace(LogError,_T("Router"),_T("Router can't parse Logon message"));
				return;
			}
			pBaseMessage = boost::static_pointer_cast<CEgTpBaseMessage>(pLogonMsg);
		}				
		break;
		case enReLogon:
		{
			CEgTpReLogonMsgPtr pReLogonMsg(new CEgTpReLogonMsg);

			CEgPacket Packet(pData,dwDataLen);
			if(!pReLogonMsg->Unpack(Packet))
			{
				CEgLibTraceManager::Trace(LogError,_T("Router"),_T("Router can't parse ReLogon message"));
				return;
			}
			pBaseMessage = boost::static_pointer_cast<CEgTpBaseMessage>(pReLogonMsg);
		}
		break;
		case enResend:
		{
			CEgTpReSendMsgPtr pReSendMsg(new CEgTpReSendMsg);

			CEgPacket Packet(pData,dwDataLen);
			if(!pReSendMsg->Unpack(Packet))
			{
				CEgLibTraceManager::Trace(LogError,_T("Router"),_T("Router can't parse ReSend message"));
				return;
			}
			
			pBaseMessage = boost::static_pointer_cast<CEgTpBaseMessage>(pReSendMsg);
		}
		break;
		case enLogout:
		{
			CEgTpLogoutMsgPtr pLogoutMsg(new CEgTpLogoutMsg);

			CEgPacket Packet(pData,dwDataLen);
			if(!pLogoutMsg->Unpack(Packet))
			{
				CEgLibTraceManager::Trace(LogError,_T("Router"),_T("Router can't parse Logout message"));
				return;
			}
			
			pBaseMessage = boost::static_pointer_cast<CEgTpBaseMessage>(pLogoutMsg);
		}
		break;
		case enSetSubject:
		{
			CEgTpUpdateSubjectWithNumMsgPtr pSetSubjMsg (new CEgTpUpdateSubjectWithNumMsg);

			CEgPacket Packet(pData,dwDataLen);
			if(!pSetSubjMsg->Unpack(Packet))
			{
				CEgLibTraceManager::Trace(LogError,_T("Router"),_T("Router can't parse SetSubject message"));
				return;
			}
			
			pBaseMessage = boost::static_pointer_cast<CEgTpBaseMessage>(pSetSubjMsg);
		}
		break;
		case enRemoveSubject:
		{
			CEgTpUpdateSubjectWithNumMsgPtr pRemoveSubjMsg(new CEgTpUpdateSubjectWithNumMsg);

			CEgPacket Packet(pData,dwDataLen);
			if(!pRemoveSubjMsg->Unpack(Packet))
			{
				CEgLibTraceManager::Trace(LogError,_T("Router"),_T("Router can't parse RemoveSubject message"));
				return;
			}
			pBaseMessage = boost::static_pointer_cast<CEgTpBaseMessage>(pRemoveSubjMsg);
		}
		break;
		case enData:
		{
			CEgTpDataWithNumMsgPtr pDataMsg(new CEgTpDataWithNumMsg);

			CEgPacket Packet(pData,dwDataLen);
			if(!pDataMsg->Unpack(Packet))
			{
				CEgLibTraceManager::Trace(LogError,_T("Router"),_T("Router can't parse Data message"));
				return;
			}
			pBaseMessage = boost::static_pointer_cast<CEgTpBaseMessage>(pDataMsg);
		}
		break;

		case enTestRequest:
		{
			CEgTpTestRequestMsgPtr pTestRequestMsg(new CEgTpTestRequestMsg);

			CEgPacket Packet(pData,dwDataLen);
			if(!pTestRequestMsg->Unpack(Packet))
			{
				CEgLibTraceManager::Trace(LogError,_T("Router"),_T("Router can't parse TestRequest message"));
				return;
			}
			pBaseMessage = boost::static_pointer_cast<CEgTpBaseMessage>(pTestRequestMsg);
		}
		break;

		case enHeartBeat:
		{
			CEgTpHeartBeatMsgPtr pHeartBeatMsg (new CEgTpHeartBeatMsg);

			CEgPacket Packet(pData,dwDataLen);
			if(!pHeartBeatMsg->Unpack(Packet))
			{
				CEgLibTraceManager::Trace(LogError,_T("Router"),_T("Router can't parse HeartBeat message"));
				return;
			}
			pBaseMessage = boost::static_pointer_cast<CEgTpBaseMessage>(pHeartBeatMsg);
		}
		break;

		case enCheckSubject:
		{
			CEgTpCheckSubjectWithNumMsgPtr pCheckSubjectMsg(new CEgTpCheckSubjectWithNumMsg);

			CEgPacket Packet(pData,dwDataLen);
			if(!pCheckSubjectMsg->Unpack(Packet))
			{
				CEgLibTraceManager::Trace(LogError,_T("Router"),_T("Router can't parse CheckSubject message"));
				return;
			}
			pBaseMessage = boost::static_pointer_cast<CEgTpBaseMessage>(pCheckSubjectMsg);
		}
		break;

		default:
			CEgLibTraceManager::Trace(LogError,_T("Router"),_T("Router didn't process this type of message : (%d)"),MsgType);
			return;
	}
	
	if(pBaseMessage==NULL)return;

	pConnection->PushMessage(pBaseMessage);
	m_IncomMessProcessor.SetReceiverActive(sSocket);
	
	CEgLibTraceManager::Trace(LogDebug,_T("Router"),_T("OnMessage completed"));
};

void CRouter::OnLogonHostMessage(const CONNECTION_ID& Connection)
{
	if(IsStopping())
		return;

	CEgLibTraceManager::Trace(LogInfo,_T("Router"),_T("Start OnLogonHostMessage"));

	SOCKET sSocket = Connection.m_Socket;
	u_long nID = Connection.m_RemoteAddr.sin_addr.S_un.S_addr;

	CEgSmartPtr<CEgConnection> pConnection = m_listConnections.GetDataByKey(sSocket);
	
	if(!pConnection)
	{
		CEgLibTraceManager::Trace(LogInfo,_T("Router"),_T("OnLogonHostMessage failed. Unknown connection"));
		return;// serious error connection doesn't exist !!!
	}
	if(pConnection->GetHostInfo())
	{// this host already logoned -> logout stuff first !
		CEgSmartPtr<CEgHostInfo> pHostInfo = pConnection->GetHostInfo();

		if((!pHostInfo) == false)
			OnLogoutHostMessage(Connection,pHostInfo->getHostKey(),0);
	}
	
	CEgSmartPtr<CEgHostInfo> pHostInfo = m_listHosts.CreateHostInfo();
	
	if(!pHostInfo)
		return;

	pHostInfo->SetConnection(pConnection);
	pConnection->SetHostInfo(pHostInfo);

	CEgLibTraceManager::Trace(LogInfo,_T("Router"),_T("Host (%s) logoned. GUID : {%.8X-...}"), (TCHAR*)GetIPByAddr(nID),pHostInfo->getHostKey().Data1);

	CEgTpLogonConfirmMsg Msg;

	Msg.m_enMsgProp = (MsgPropertiesEnum)enMpUnspecified;
	Msg.m_enType = (MsgTypeEnum)enLogonConfirm;
	
	Msg.m_HostID = pHostInfo->getHostKey();
	Msg.m_ulMessageReceived = (u_long)pHostInfo->GetRecMessNum();
	
	CEgPacket Packet;
	if(!Msg.Pack(Packet))
	{
		CEgLibTraceManager::Trace(LogError,_T("Router"),_T("Router can't pack LogonConfirm message for Host {%.8X-...} ."),pHostInfo->getHostKey().Data1);
		return;
	}

	u_long dwSize;
	void* pData = Packet.GetBuffer(dwSize);
				
	if(!dwSize)
		return;

	CEgLibTraceManager::Trace(LogInfo,_T("Router"),_T("Router send logon confirm message to host {%.8X-...}"),pHostInfo->getHostKey().Data1);
	DWORD dwRes = m_Driver.Send(pConnection->GetSocket(), pData, dwSize);
	if(dwRes)
		CEgLibTraceManager::Trace(LogError,_T("Router"),_T("Router can't send logon confirm message to host {%.8X-...}. Error (%d)"),pHostInfo->getHostKey().Data1,dwRes);

	pHostInfo->SetStatus(CEgHostInfo::enOnline);

	m_GrantedMessProcessor.SetReceiverActive(pHostInfo->getHostKey());
	m_BufferedMessProcessor.SetReceiverActive(pHostInfo->getHostKey());
	m_NotBuffMessProcessor.SetReceiverActive(pHostInfo->getHostKey());

	CTpEventPtr pEvent (new CTpEvent());
	pEvent->m_enDispId = DISPID_IRouterEventsOnClientConnected;
	pEvent->m_bsParams = ::SysAllocStringLen(GetIPByAddr(nID), SysStringLen(GetIPByAddr(nID)));
	m_EventThread.PushEvent(pEvent);

	CEgLibTraceManager::Trace(LogInfo,_T("Router"),_T("OnLogonHostMessage completed"));
}

void CRouter::OnReLogonMessage(const CONNECTION_ID& Connection, const CEgGuid& HostID, u_long HostMsgReceived)
{
	if(IsStopping())
		return;

	CEgLibTraceManager::Trace(LogInfo,_T("Router"),_T("Start OnReLogonMessage"));
	SOCKET sSocket = Connection.m_Socket;
	u_long nID = Connection.m_RemoteAddr.sin_addr.S_un.S_addr;

	CEgSmartPtr<CEgConnection> pConnection = m_listConnections.GetDataByKey(sSocket);

	if(!pConnection)
	{
		CEgLibTraceManager::Trace(LogError,_T("Router"),_T("OnReLogonMessage failed. Unknown connection"));
		return;// serious error connection doesn't exist !!!
	}	
	CEgSmartPtr<CEgHostInfo> pHostInfo = m_listHosts.GetDataByKey(HostID);
	
	if(!CheckHostValidity(pHostInfo,HostMsgReceived))
	{
		CEgTpLogoutMsg Msg;
		Msg.m_enMsgProp = (MsgPropertiesEnum)enMpUnspecified;
		Msg.m_enType = (MsgTypeEnum)enLogout;
		Msg.m_ulMessageReceived = (u_long)0;
		
		Msg.m_HostID = HostID;

		CEgPacket Packet;
		if(!Msg.Pack(Packet))
		{
			CEgLibTraceManager::Trace(LogError,_T("Router"),_T("Router can't pack Logout message for Host {%.8X-...} ."),HostID.Data1);
			return;
		}

		u_long dwSize;
		void* pData = Packet.GetBuffer(dwSize);
				
		if(!dwSize)
			return;

		if(m_Driver.Send(pConnection->GetSocket(), pData, dwSize) != 0)
			return;
		
		CEgLibTraceManager::Trace(LogInfo,_T("Router"),_T("Router send logout message to host {%.8X-...}"),HostID.Data1);

		return;
	}
	CEgSmartPtr<CEgHostInfo> pOldHost = pConnection->GetHostInfo();

	if((!pOldHost) == false && pOldHost != pHostInfo)
	{// this connection already logoned for another host -> serious error		
		return;
	}

	// killed old connection
	CEgSmartPtr<CEgConnection> pOldConn = pHostInfo->GetConnection();
	if((!pOldConn) == false && pOldConn != pConnection)
	{
		pHostInfo->SetConnection(NULL);
		m_listConnections.Remove(pOldConn->GetSocket(),true);
	}
	
	pHostInfo->SetConnection(pConnection);
	
	pConnection->SetHostInfo(pHostInfo);

	try
	{
		CEgLibTraceManager::Trace(LogInfo,_T("Router"),_T("Host (%s) relogoned. GUID : {%.8X-...}"), (TCHAR*)GetIPByAddr(nID), HostID.Data1);

		CTpEventPtr pEvent (new CTpEvent());
		pEvent->m_enDispId = DISPID_IRouterEventsOnClientConnected;
		pEvent->m_bsParams = ::SysAllocStringLen(GetIPByAddr(nID), SysStringLen(GetIPByAddr(nID)));
		m_EventThread.PushEvent(pEvent);
	}
	catch(_com_error er)
	{
		CEgLibTraceManager::Trace(LogError,_T("Router"),_T("Exception occured in ReLogonHost. HRESULT : (%d)"),er.Error());
		return;
	}

	CEgTpLogonConfirmMsg Msg;
	Msg.m_enMsgProp = (MsgPropertiesEnum)enMpUnspecified;
	Msg.m_enType = (MsgTypeEnum)enLogonConfirm;
	Msg.m_HostID = pHostInfo->getHostKey();
	Msg.m_ulMessageReceived = (u_long)pHostInfo->GetRecMessNum();
	
	CEgPacket Packet;
	if(!Msg.Pack(Packet))
	{
		CEgLibTraceManager::Trace(LogError,_T("Router"),_T("Router can't pack LogonConfirm message for Host {%.8X-...} ."),pHostInfo->getHostKey().Data1);
		return;
	}

	u_long dwSize;
	void* pData = Packet.GetBuffer(dwSize);
				
	if(!dwSize)
		return;

	DWORD dwRes = m_Driver.Send(pConnection->GetSocket(), pData, dwSize);
	if(dwRes)
		CEgLibTraceManager::Trace(LogError,_T("Router"),_T("Router can't send logon confirm message to host {%.8X-...}. Error (%d)"),pHostInfo->getHostKey().Data1,dwRes);
	else
		CEgLibTraceManager::Trace(LogInfo,_T("Router"),_T("Router send logon confirm message to host {%.8X-...}. Router receive:(%d), send:(%d) messages. Host receive (%d) message."),pHostInfo->getHostKey().Data1, Msg.m_ulMessageReceived, pHostInfo->GetSendMessNum(), HostMsgReceived);


	pHostInfo->SetStatus(CEgHostInfo::enOnline);

	pHostInfo->ResendGrantedMessages(HostMsgReceived);
	m_GrantedMessProcessor.SetReceiverActive(pHostInfo->getHostKey());
	m_BufferedMessProcessor.SetReceiverActive(pHostInfo->getHostKey());
	m_NotBuffMessProcessor.SetReceiverActive(pHostInfo->getHostKey());

	CEgLibTraceManager::Trace(LogInfo,_T("Router"),_T("OnReLogonMessage completed"));
}

void CRouter::OnReSendMsg(const CONNECTION_ID& Connection, const CEgGuid& HostID, u_long HostMsgReceived)
{
	if(IsStopping())
		return;

	CEgLibTraceManager::Trace(LogInfo,_T("Router"),_T("Start OnReSendMsg"));
	SOCKET sSocket = Connection.m_Socket;
	u_long nID = Connection.m_RemoteAddr.sin_addr.S_un.S_addr;

	CEgSmartPtr<CEgConnection> pConnection = m_listConnections.GetDataByKey(sSocket);

	if(!pConnection)
	{
		CEgLibTraceManager::Trace(LogError,_T("Router"),_T("OnReSendMsg failed. Unknown connection"));
		return;// serious error connection doesn't exist !!!
	}			
	CEgSmartPtr<CEgHostInfo> pHostInfo = m_listHosts.GetDataByKey(HostID);
	
	if(!CheckHostValidity(pHostInfo,HostMsgReceived))
	{
		return;
	}
	CEgSmartPtr<CEgHostInfo> pOldHost = pConnection->GetHostInfo();

	if(pOldHost && pOldHost != pHostInfo)
	{// this connection already logoned for another host -> serious error
		return;
	}

	CEgLibTraceManager::Trace(LogInfo,_T("Router"),_T("Router received ReSend message from host {%.8X-...}. RouterMessageSend (%d). HostMessageReceived (%d)  "), pHostInfo->getHostKey().Data1,pHostInfo->GetSendMessNum(),HostMsgReceived);

	pHostInfo->ResendGrantedMessages(HostMsgReceived);
	CEgLibTraceManager::Trace(LogInfo,_T("Router"),_T("OnReSendMsg completed"));
}

void CRouter::OnLogoutHostMessage(const CONNECTION_ID& Connection, const CEgGuid& HostID, u_long HostMsgReceived)
{
	if(IsStopping())
		return;

	CEgLibTraceManager::Trace(LogDebug,_T("Router"),_T("Start OnLogoutHostMessage"));

	SOCKET sSocket = Connection.m_Socket;
	u_long nID = Connection.m_RemoteAddr.sin_addr.S_un.S_addr;

	CEgSmartPtr<CEgConnection> pConnection = m_listConnections.GetDataByKey(sSocket);
			
	CEgSmartPtr<CEgHostInfo> pHostInfo = m_listHosts.GetDataByKey(HostID);
	
	if(!pHostInfo)
	{
		// unknown host -> error
		m_listConnections.Remove(sSocket,true);
		return;
	}

	CEgLibTraceManager::Trace(LogInfo,_T("Router"),_T("Router received logout message from host {%.8X-...}."), pHostInfo->getHostKey().Data1);

	// unregister subjects from host
	m_HostInfoSubjectDatabase.UnregisterData(pHostInfo);
	
	m_listHosts.Remove(HostID);
	
	if((!pConnection) == false)
	{
		m_listConnections.Remove(sSocket);
		CTpEventPtr pEvent (new CTpEvent());
		pEvent->m_enDispId = DISPID_IRouterEventsOnClientDisconnected;
		pEvent->m_bsParams = ::SysAllocStringLen(GetIPByAddr(nID), SysStringLen(GetIPByAddr(nID)));
		m_EventThread.PushEvent(pEvent);
	}

	// clear unsend message
	pHostInfo->ClearAllMessages();

	CEgLibTraceManager::Trace(LogDebug, _T("Router"),_T("OnLogoutHostMessage completed"));
}

void CRouter::OnSetSubjectMessage(const CONNECTION_ID& Connection, const tstring& szSubject, u_long HostMsgReceived,u_long MsgNum)
{
	if(IsStopping())
		return;

	CEgLibTraceManager::Trace(LogDebug, _T("Router"),_T("Start OnSetSubjectMessage"));

	SOCKET sSocket = Connection.m_Socket;
	u_long nID = Connection.m_RemoteAddr.sin_addr.S_un.S_addr;

	CEgSmartPtr<CEgConnection> pConnection = m_listConnections.GetDataByKey(sSocket);

	if(!pConnection)
	{
		CEgLibTraceManager::Trace(LogInfo,_T("Router"),_T("OnSetSubjectMessage failed. Unknown connection"));
		return;// serious error connection doesn't exist !!!
	}
	
	CEgSmartPtr<CEgHostInfo> pHostInfo = pConnection->GetHostInfo();
	
	if(!CheckHostValidity(pHostInfo,HostMsgReceived, MsgNum,enMpGuarantee))
	{
		return;
	}
	CEgLibTraceManager::Trace(LogInfo,_T("Router"),_T("Router SetSubject message from host {%.8X-...}. Subject : (%s)"),pHostInfo->getHostKey().Data1, szSubject.c_str());

	// register new subjects for host
	m_HostInfoSubjectDatabase.RegisterDataSubject(szSubject,pHostInfo);


	// here delete from storage receiving granted message
	pHostInfo->RemoveConfirmedGrantedMessages(HostMsgReceived);

	CEgLibTraceManager::Trace(LogDebug, _T("Router"),_T("OnSetSubjectMessage completed"));
}

void CRouter::OnRemoveSubjectMessage(const CONNECTION_ID& Connection, const tstring& szSubject, u_long HostMsgReceived,u_long MsgNum)
{
	if(IsStopping())
		return;

	CEgLibTraceManager::Trace(LogDebug,_T("Router"),_T("Start OnRemoveSubjectMessage"));
	SOCKET sSocket = Connection.m_Socket;
	u_long nID = Connection.m_RemoteAddr.sin_addr.S_un.S_addr;

	CEgSmartPtr<CEgConnection> pConnection = m_listConnections.GetDataByKey(sSocket);
			
	if(!pConnection)
	{
		CEgLibTraceManager::Trace(LogInfo,_T("Router"),_T("OnRemoveSubjectMessage failed. Unknown connection"));
		return;// serious error connection doesn't exist !!!
	}

	CEgSmartPtr<CEgHostInfo> pHostInfo = pConnection->GetHostInfo();
	
	if(!CheckHostValidity(pHostInfo,HostMsgReceived,MsgNum,enMpGuarantee))
	{
		return;
	}

	CEgLibTraceManager::Trace(LogInfo,_T("Router"),_T("Router RemoveSubject message from host {%.8X-...}. Subject : (%s)"),pHostInfo->getHostKey().Data1, szSubject.c_str());	

	// unregister subjects from host
	m_HostInfoSubjectDatabase.UnregisterDataSubject(szSubject,pHostInfo);

	// here delete from storage receiving granted message
	pHostInfo->RemoveConfirmedGrantedMessages(HostMsgReceived);
	
	CEgLibTraceManager::Trace(LogDebug, _T("Router"),_T("OnRemoveSubjectMessage completed"));
}

void CRouter::OnDataMessage(const CONNECTION_ID& Connection, CEgTpDataWithNumMsgPtr pDataMsg)
{
	if(IsStopping())
		return;

	CEgLibTraceManager::Trace(LogDebug, _T("Router"),_T("Start OnDataMessage"));
/*	if(pDataMsg->m_ulSize >= MAXIMUM_MESSAGE_LENGTH)					// Check Data length
	{
		CEgLibTraceManager::Trace(LogInfo,_T("Router"),_T("OnDataMessage failed. Too long message"));
		return;
	}*/
	SOCKET sSocket = Connection.m_Socket;

	CEgSmartPtr<CEgConnection> pConnection = m_listConnections.GetDataByKey(sSocket);
			
	if(!pConnection)
	{
		CEgLibTraceManager::Trace(LogInfo,_T("Router"),_T("OnDataMessage failed. Unknown connection"));
		return;// serious error connection doesn't exist !!!
	}

	CEgSmartPtr<CEgHostInfo> pHostServer = pConnection->GetHostInfo();
	
	if(!CheckHostValidity(pHostServer,pDataMsg->m_ulMessageReceived,pDataMsg->m_ulMessageSend,pDataMsg->m_enMsgProp))
	{
		return;
	}
	// here delete from storage receiving granted message
	pHostServer->RemoveConfirmedGrantedMessages(pDataMsg->m_ulMessageReceived);
	
	USES_CONVERSION;

	set<CEgSmartPtr<CEgHostInfo> > sHosts;
	m_HostInfoSubjectDatabase.GetDataBySubjectWithMask(A2T((LPSTR)pDataMsg->m_strSubject.GetStr()),sHosts);

	CEgTpDataMsgPtr pResponceMsg;
	if(sHosts.size())
	{
		pResponceMsg = CEgTpDataMsgPtr(new CEgTpDataMsg);

		pResponceMsg->m_blobData = pDataMsg->m_blobData;
		pResponceMsg->m_enMsgProp = pDataMsg->m_enMsgProp;
		pResponceMsg->m_enType = pDataMsg->m_enType;
		pResponceMsg->m_strSubject = pDataMsg->m_strSubject;
	
		for(set<CEgSmartPtr<CEgHostInfo> >::iterator it = sHosts.begin(); it != sHosts.end(); it++)
		{
			CEgHostInfo* pHostClient = *it;

			if(pHostServer == pHostClient)
				continue;
					
			switch(pDataMsg->m_enMsgProp)
			{
				case enMpGuarantee:
				{				
					pHostClient->PushMessage(pResponceMsg);
					if(pHostClient->GetStatus() == CEgHostInfo::enOnline)
						m_GrantedMessProcessor.SetReceiverActive(pHostClient->getHostKey());

				}
				break;
				case enMpBuffered:
				{						
					pHostClient->PushMessage(pResponceMsg);
					if(pHostClient->GetStatus() == CEgHostInfo::enOnline)
						m_BufferedMessProcessor.SetReceiverActive(pHostClient->getHostKey());
				}
				break;
				default:
				{						
					pHostClient->PushMessage(pResponceMsg);
					if(pHostClient->GetStatus() == CEgHostInfo::enOnline)
						m_NotBuffMessProcessor.SetReceiverActive(pHostClient->getHostKey());
				}
				break;
			}
		}
	}
	CEgLibTraceManager::Trace(LogDebug, _T("Router"),_T("OnDataMessage completed"));
}

void CRouter::OnTestRequestMessage(const CONNECTION_ID& Connection, u_long HostMsgReceived, u_long HostMsgSend)
{
	if(IsStopping())
		return;

	CEgLibTraceManager::Trace(LogDebug,_T("Router"),_T("Start OnTestRequestMessage"));
	SOCKET sSocket = Connection.m_Socket;
	u_long nID = Connection.m_RemoteAddr.sin_addr.S_un.S_addr;

	CEgSmartPtr<CEgConnection> pConnection = m_listConnections.GetDataByKey(sSocket);
			
	if(!pConnection)
	{
		CEgLibTraceManager::Trace(LogInfo,_T("Router"),_T("OnTestRequestMessage failed. Unknown connection"));
		return;// serious error connection doesn't exist !!!
	}

	CEgSmartPtr<CEgHostInfo> pHostInfo = pConnection->GetHostInfo();
	if(!pHostInfo)
		return;

	u_long ulRouterMsgReceive = pHostInfo->GetRecMessNum();

	pHostInfo->SetLastMessRecTime(time(NULL));

	if(HostMsgSend > ulRouterMsgReceive)
	{//GAPS detected
		
		pHostInfo->SetStatus(CEgHostInfo::enDisconnected);
		pHostInfo->SetConnection(NULL);
		m_listConnections.Remove(sSocket,true);

		CEgLibTraceManager::Trace(LogError,_T("Router"),_T("Router detect GAP of message from host {%.8X-...}. HostMsgSend : (%d), RouterMsgReceive : (%d)"),pHostInfo->getHostKey().Data1, HostMsgSend, ulRouterMsgReceive);	
	}

	// here delete from storage recieving granted message
	pHostInfo->RemoveConfirmedGrantedMessages(HostMsgReceived);

	CEgLibTraceManager::Trace(LogDebug,_T("Router"),_T("OnTestRequestMessage completed"));
}

void CRouter::OnHeartBeatMessage(const CONNECTION_ID& Connection, u_long HostMsgReceived, u_long HostMsgSend)
{
	if(IsStopping())
		return;

	CEgLibTraceManager::Trace(LogDebug,_T("Router"),_T("Start OnHeartBeatMessage"));
	SOCKET sSocket = Connection.m_Socket;
	u_long nID = Connection.m_RemoteAddr.sin_addr.S_un.S_addr;

	CEgSmartPtr<CEgConnection> pConnection = m_listConnections.GetDataByKey(sSocket);
			
	if(!pConnection)
	{
		CEgLibTraceManager::Trace(LogInfo,_T("Router"),_T("OnHeartBeatMessage failed. Unknown connection."));
		return;// serious error connection doesn't exist !!!
	}
	CEgSmartPtr<CEgHostInfo> pHostInfo = pConnection->GetHostInfo();
	if(!pHostInfo)
	{
		CEgLibTraceManager::Trace(LogInfo,_T("Router"),_T("OnHeartBeatMessage failed. Unknown host."));
		return;
	}
	u_long ulRouterMsgReceive = pHostInfo->GetRecMessNum();

	pHostInfo->SetLastMessRecTime(time(NULL));

	if(HostMsgSend > ulRouterMsgReceive)
	{//GAPS detected
		
		pHostInfo->SetStatus(CEgHostInfo::enDisconnected);
		pHostInfo->SetConnection(NULL);
		m_listConnections.Remove(sSocket,true);

		CEgLibTraceManager::Trace(LogError,_T("Router"),_T("Router detect GAP of message from host {%.8X-...}. HostMsgSend : (%d), RouterMsgReceive : (%d)"),pHostInfo->getHostKey().Data1, HostMsgSend, ulRouterMsgReceive);	
	}

	// here delete from storage recieving granted message
	pHostInfo->RemoveConfirmedGrantedMessages(HostMsgReceived);

	CEgLibTraceManager::Trace(LogDebug,_T("Router"),_T("OnHeartBeatMessage completed."));
}

void CRouter::OnCheckSubjectMessage(const CONNECTION_ID& Connection, CEgTpCheckSubjectWithNumMsgPtr pCheckSubjectMsg)
{
	if(IsStopping())
		return;

	CEgLibTraceManager::Trace(LogDebug,_T("Router"),_T("Start OnCheckSubjectMessage."));

	SOCKET sSocket = Connection.m_Socket;
	u_long nID = Connection.m_RemoteAddr.sin_addr.S_un.S_addr;

	CEgSmartPtr<CEgConnection> pConnection = m_listConnections.GetDataByKey(sSocket);
			
	if(!pConnection)
	{
		CEgLibTraceManager::Trace(LogInfo,_T("Router"),_T("OnCheckSubjectMessage failed. Unknown connection"));
		return;// serious error connection doesn't exist !!!
	}

	CEgSmartPtr<CEgHostInfo> pHostInfo = pConnection->GetHostInfo();
	if(!pHostInfo)
	{
		CEgLibTraceManager::Trace(LogInfo,_T("Router"),_T("OnCheckSubjectMessage failed. Unknown host"));
		return;
	}
	pHostInfo->SetLastMessRecTime(time(NULL));

	if(!CheckHostValidity(pHostInfo,pCheckSubjectMsg->m_ulMessageReceived,pCheckSubjectMsg->m_ulMessageSend,enMpGuarantee))
	{
		return;
	}
	CEgLibTraceManager::Trace(LogInfo,_T("Router"),_T("Router receive CheckSubject message from host {%.8X-...}. Subject : (%s)"),pHostInfo->getHostKey().Data1, pCheckSubjectMsg->m_strSubject.GetStr());
	
	// here delete from storage receiving granted message
	pHostInfo->RemoveConfirmedGrantedMessages(pCheckSubjectMsg->m_ulMessageReceived);

	set<CEgSmartPtr<CEgHostInfo> > sHosts;
	m_HostInfoSubjectDatabase.GetDataBySubjectWithMask(A2T((LPSTR)pCheckSubjectMsg->m_strSubject.GetStr()),sHosts);
	
	CEgTpCheckSubjectMsgPtr pResponceMsg(new CEgTpCheckSubjectMsg());
	pResponceMsg->m_enMsgProp = pCheckSubjectMsg->m_enMsgProp;
	pResponceMsg->m_enType = pCheckSubjectMsg->m_enType;
	pResponceMsg->m_guidSession = pCheckSubjectMsg->m_guidSession;
	pResponceMsg->m_strSubject = pCheckSubjectMsg->m_strSubject;
	pResponceMsg->m_ulNumSubscribers = sHosts.size();
	
	pHostInfo->PushMessage(pResponceMsg);
	
	if(pHostInfo->GetStatus() == CEgHostInfo::enOnline)
		m_GrantedMessProcessor.SetReceiverActive(pHostInfo->getHostKey());

	CEgLibTraceManager::Trace(LogDebug,_T("Router"),_T("OnCheckSubjectMessage completed"));
}

BOOL CRouter::CheckHostValidity( CEgHostInfo* pHostInfo, u_long HostMsgReceived, 
								u_long MsgNum, MsgPropertiesEnum enProp)
{
	if(IsStopping())
		return FALSE;

	CEgLibTraceManager::Trace(LogDebug, _T("Router"),_T("Start CheckHostValidity"));

	if(!pHostInfo)
	{
		CEgLibTraceManager::Trace(LogError,_T("Router"),_T("CheckHostValidity failed. Unknown host"));
		// unknown host -> failed !!!
		return FALSE;
	}

	pHostInfo->SetLastMessRecTime(time(NULL));

	u_long RouterMsgReceive = pHostInfo->GetRecMessNum();
	
	if(MsgNum > RouterMsgReceive && enProp != enMpGuarantee)
	{// GAPS detected
		CEgSmartPtr<CEgConnection> pConn = pHostInfo->GetConnection();
		
		if(!pConn)
		{
			CEgLibTraceManager::Trace(LogError,_T("Router"),_T("CheckHostValidity failed. Unknown connection"));
			return FALSE;
		}
		SOCKET sSocket = pConn->GetSocket();
		
		if(sSocket == INVALID_SOCKET)
		{
			CEgLibTraceManager::Trace(LogError,_T("Router"),_T("CheckHostValidity failed. Unknown socket"));
			return FALSE;
		}
		// GAPS detected
		pHostInfo->SetStatus(CEgHostInfo::enDisconnected);
		pHostInfo->SetConnection(NULL);
		m_listConnections.Remove(sSocket,true);

		CEgLibTraceManager::Trace(LogError,_T("Router"),_T("Router detect GAP of message from host {%.8X-...}. HostMsgSend : (%d), RouterMsgReceive : (%d)"),pHostInfo->getHostKey().Data1, MsgNum, RouterMsgReceive);

		return FALSE;
	}
	else if(MsgNum > RouterMsgReceive + 1 && enProp == enMpGuarantee)
	{
		CEgSmartPtr<CEgConnection> pConn = pHostInfo->GetConnection();
		
		if(!pConn)
		{
			CEgLibTraceManager::Trace(LogError,_T("Router"),_T("CheckHostValidity failed. Unknown connection"));
			return FALSE;
		}

		SOCKET sSocket = pConn->GetSocket();
		
		if(sSocket == INVALID_SOCKET)
		{
			CEgLibTraceManager::Trace(LogError,_T("Router"),_T("CheckHostValidity failed. Unknown socket"));
			return FALSE;
		}
		// GAPS detected
		pHostInfo->SetStatus(CEgHostInfo::enDisconnected);
		pHostInfo->SetConnection(NULL);
		m_listConnections.Remove(sSocket,true);

		CEgLibTraceManager::Trace(LogError,_T("Router"),_T("Router detect GAP of message from host {%.8X-...}. MessNum : (%d), RouterMsgReceive : (%d)"),pHostInfo->getHostKey().Data1, MsgNum, RouterMsgReceive);	

		return FALSE;
	}
	else if(MsgNum < RouterMsgReceive + 1 && enProp == enMpGuarantee)
	{
		CEgLibTraceManager::Trace(LogInfo,_T("Router"),_T("Router already receive this message from host {%.8X-...}. MessNum : (%d), RouterMsgReceive : (%d)"),pHostInfo->getHostKey().Data1, MsgNum, RouterMsgReceive);
		return FALSE;// we already received this message
	}
	else if(enProp == enMpGuarantee)
	{
		if( !(MsgNum % 10000) ) // trace every 10000s message
			CEgLibTraceManager::Trace(LogInfo,_T("Router"),_T("Router receive expected granted data message from host {%.8X-...}. MessNum : (%d)"),pHostInfo->getHostKey().Data1, MsgNum);
		else
			CEgLibTraceManager::Trace(LogError,_T("Router"),_T("Router receive expected granted data message from host {%.8X-...}. MessNum : (%d)"),pHostInfo->getHostKey().Data1, MsgNum);

		pHostInfo->IncRecMessNum();
	}
	CEgLibTraceManager::Trace(LogDebug, _T("Router"),_T("CheckHostValidity completed"));

	return TRUE;
}

bool CRouter::CGrantedMessageProcessor::OnMessage(/*in*/ CEgHostInfo* pcxtHost,
						/*in*/ CEgTpBaseMessagePtr pMessage, u_long keyMessage )
{
	ATLASSERT(m_pcxtSender != NULL);
	EgAssert(this);

	if(m_pcxtSender->IsStopping())
		return false;

	CEgLibTraceManager::Trace(LogDebug, _T("Router granted processor"),_T("Start OnMessage"));
	if (m_pcxtSender == NULL)
		return false;
	
	if(!pcxtHost)
		return false; // serious error!!!

	CEgSmartPtr<CEgConnection> pConnection = pcxtHost->GetConnection();

	if((!pConnection) == false)
	{		
		SOCKET Socket = pConnection->GetSocket();

		if (Socket != INVALID_SOCKET)
		{
			u_long SendMess = pcxtHost->GetSendMessNum();
			if(keyMessage != SendMess + 1)
			{
				CEgLibTraceManager::Trace(LogError,_T("Router granted processor"),_T("Can't send unexpected granted message to Host {%.8X-...}. MessageNum (%d), SendMessage (%d)"),pcxtHost->getHostKey().Data1,keyMessage,SendMess);
				return false;
			}
			void* pData = NULL;
			u_long dwSize = 0;
			
			CEgPacket pack;

			if(!pMessage->Pack(pack))
			{
				CEgLibTraceManager::Trace(LogError,_T("Router granted processor"),_T("Router can't pack granted message. Num (%d) for Host {%.8X-...}"),keyMessage,pcxtHost->getHostKey().Data1);
				return false;
			}
			
			ULongF ulMessSend = keyMessage;
			ULongF ulMessageReceived = pcxtHost->GetRecMessNum();
			pack << ulMessSend;
			pack << ulMessageReceived;

			pData = pack.GetBuffer(dwSize);
			
			if(pData && dwSize)
			{
				
				DWORD dwRes = m_pcxtSender->m_Driver.Send(Socket, pData, dwSize);

				if(dwRes)
					CEgLibTraceManager::Trace(LogInfo,_T("Router granted processor"),_T("Error (%d) occured while Router send granted message (%d) for host {%.8X-...}"),dwRes,keyMessage,pcxtHost->getHostKey().Data1);
				else
				{
					if( !(keyMessage % 10000) ) // trace every 10000s message
						CEgLibTraceManager::Trace(LogInfo,_T("Router granted processor"),_T("Router successfully send (%d) granted message to host {%.8X-...}"), keyMessage,pcxtHost->getHostKey().Data1);
					else
						CEgLibTraceManager::Trace(LogError,_T("Router granted processor"),_T("Router successfully send (%d) granted message to host {%.8X-...}"), keyMessage,pcxtHost->getHostKey().Data1);

					return true;
				}
			}
		}		
	}

	CEgLibTraceManager::Trace(LogWarning,_T("Router granted processor"),_T("OnMessage failed. Unknown connection"));

	return false;
}

bool CRouter::CBufferedMessageProcessor::OnMessage( CEgHostInfo* pcxtHost, 
													CEgTpBaseMessagePtr pMessage, u_long )
{
	ATLASSERT(m_pcxtSender != NULL);

	EgAssert(this);

	if(m_pcxtSender->IsStopping())
		return false;

	CEgLibTraceManager::Trace(LogDebug,_T("Router buffered processor"),_T("Start OnMessage"));

	if (m_pcxtSender == NULL)
		return false;
	
	if(!pcxtHost)
		return false;

	CEgSmartPtr<CEgConnection> pConnection = pcxtHost->GetConnection();

	if((!pConnection) == false)
	{		
		SOCKET Socket = pConnection->GetSocket();

		if (Socket != INVALID_SOCKET)
		{
			void* pData = NULL;
			u_long dwSize = 0;
			
			CEgPacket pack;

			if(!pMessage->Pack(pack))
			{
				CEgLibTraceManager::Trace(LogError,_T("Router buffered processor"),_T("Router can't pack buffered message for Host {%.8X-...}"),pcxtHost->getHostKey().Data1);
				return false;
			}
			ULongF ulMessageReceived = pcxtHost->GetRecMessNum();
			ULongF ulMessageSend = pcxtHost->GetSendMessNum();
			pack << ulMessageSend;
			pack << ulMessageReceived;

			
			pData = pack.GetBuffer(dwSize);
			if(pData && dwSize)
			{				
				DWORD dwRes = m_pcxtSender->m_Driver.Send(Socket, pData, dwSize);
				if(dwRes)
					CEgLibTraceManager::Trace(LogInfo,_T("Router buffered processor"),_T("Router can't send buffered message to host {%.8X-...}. Error (%d)"),pcxtHost->getHostKey().Data1,dwRes);
				else
				{
					CEgLibTraceManager::Trace(LogError,_T("Router buffered processor"),_T("OnMessage completed"));
					return true;
				}
			}
		}		
	}
	CEgLibTraceManager::Trace(LogWarning,_T("Router buffered processor"),_T("OnMessage failed. Unknown connection"));

	return false;
}

bool CRouter::CNotBufferedMessageProcessor::OnMessage(CEgHostInfo* pcxtHost, 
												  CEgTpBaseMessagePtr pMessage,u_long )
{
	ATLASSERT(m_pcxtSender != NULL);

	EgAssert(this);

	if(m_pcxtSender->IsStopping())
		return false;

	CEgLibTraceManager::Trace(LogError,_T("Router notbuffered processor"),_T("Start OnMessage"));

	if (m_pcxtSender == NULL)
		return false;

	if(!pcxtHost)
		return false; 

	CEgSmartPtr<CEgConnection> pConnection = pcxtHost->GetConnection();

	if((!pConnection) == false)
	{
		
		SOCKET Socket = pConnection->GetSocket();

		if (Socket != INVALID_SOCKET)
		{
			void* pData = NULL;
			u_long dwSize = 0;
			
			CEgPacket pack;

			if(!pMessage->Pack(pack))
			{
				CEgLibTraceManager::Trace(LogError,_T("Router notbuffered processor"),_T("Router can't pack notbuffered message for Host {%.8X-...}"),pcxtHost->getHostKey().Data1);
				return false;
			}

			ULongF ulMessageReceived = pcxtHost->GetRecMessNum();
			ULongF ulMessageSend = pcxtHost->GetSendMessNum();
			pack << ulMessageSend;
			pack << ulMessageReceived;

			pData = pack.GetBuffer(dwSize);
			if(pData && dwSize)
			{
				DWORD dwRes = m_pcxtSender->m_Driver.Send(Socket, pData, dwSize);
				if(dwRes)
					CEgLibTraceManager::Trace(LogInfo,_T("Router notbuffered processor"),_T("Router can't send notbuffered message to host {%.8X-...}. Error (%d)"),pcxtHost->getHostKey().Data1,dwRes);
				else
				{
					CEgLibTraceManager::Trace(LogError,_T("Router notbuffered processor"),_T("OnMessage completed"));
					return true;
				}
			}
		}		
	}
	CEgLibTraceManager::Trace(LogWarning,_T("Router notbuffered processor"),_T("OnMessage failed. Unknown connection"));

	return false;
}

bool CRouter::CIncomingMessageProcessor::OnMessage(CEgConnection* pcxtConnection,  CEgTpBaseMessagePtr pMessage, u_long)
{
	ATLASSERT(m_pcxtSender != NULL);

	EgAssert(this);

	if (m_pcxtSender == NULL)
		return false;
	
	if(m_pcxtSender->IsStopping())
		return false;

	CEgLibTraceManager::Trace(LogDebug, _T("Router incoming processor"),_T("Start OnMessage"));
		
	if(!pcxtConnection)
	{
		CEgLibTraceManager::Trace(LogError,_T("Router incoming processor"),_T("OnMessage failed. Unknown connection"));
		return false;
	}		
	USES_CONVERSION;

	switch(pMessage->m_enType)
	{
		case enLogon:
			
			m_pcxtSender->OnLogonHostMessage(pcxtConnection->GetConnectionID());
			return true;

			break;
				
		case enReLogon:
		{
			CEgTpReLogonMsgPtr pReLogonMsg = boost::dynamic_pointer_cast<CEgTpReLogonMsg>(pMessage);

			m_pcxtSender->OnReLogonMessage(pcxtConnection->GetConnectionID(),pReLogonMsg->m_HostID.GetGUID(),pReLogonMsg->m_ulMessageReceived);
			return true;
		}
		break;
		case enResend:
		{
			CEgTpReSendMsgPtr pReSendMsg = boost::dynamic_pointer_cast<CEgTpReSendMsg>(pMessage);

			m_pcxtSender->OnReSendMsg(pcxtConnection->GetConnectionID(),pReSendMsg->m_HostID.GetGUID(),pReSendMsg->m_ulMessageReceived);
			return true;
		}
		break;
		case enLogout:
		{
			CEgTpLogoutMsgPtr pLogoutMsg = boost::dynamic_pointer_cast<CEgTpLogoutMsg>(pMessage);

			m_pcxtSender->OnLogoutHostMessage(pcxtConnection->GetConnectionID(),pLogoutMsg->m_HostID.GetGUID(),pLogoutMsg->m_ulMessageReceived);
			return true;
		}
		break;
		case enSetSubject:
		{
			CEgTpUpdateSubjectWithNumMsgPtr pSetSubjMsg = boost::dynamic_pointer_cast<CEgTpUpdateSubjectWithNumMsg>(pMessage);

			m_pcxtSender->OnSetSubjectMessage(pcxtConnection->GetConnectionID(),A2T((LPSTR)pSetSubjMsg->m_strSubject.GetStr()),pSetSubjMsg->m_ulMessageReceived,pSetSubjMsg->m_ulMessageSend);
			return true;
		}
		break;
		case enRemoveSubject:
		{
			CEgTpUpdateSubjectWithNumMsgPtr pRemoveSubjMsg = boost::dynamic_pointer_cast<CEgTpUpdateSubjectWithNumMsg>(pMessage);

			m_pcxtSender->OnRemoveSubjectMessage(pcxtConnection->GetConnectionID(),A2T((LPSTR)pRemoveSubjMsg->m_strSubject.GetStr()),pRemoveSubjMsg->m_ulMessageReceived,pRemoveSubjMsg->m_ulMessageSend);
			return true;
		}
		break;
		case enData:
		{
			m_pcxtSender->OnDataMessage(pcxtConnection->GetConnectionID(), boost::dynamic_pointer_cast<CEgTpDataWithNumMsg>(pMessage));
			return true;
		}
		break;
		case enTestRequest:
		{
			CEgTpTestRequestMsgPtr pTestRequestMsg = boost::dynamic_pointer_cast<CEgTpTestRequestMsg>(pMessage);

			m_pcxtSender->OnTestRequestMessage(pcxtConnection->GetConnectionID(),pTestRequestMsg->m_ulMessageReceived,pTestRequestMsg->m_ulMessageSend);
			return true;
		}
		break;
		case enHeartBeat:
		{
			CEgTpHeartBeatMsgPtr pHeartBeatMsg = boost::dynamic_pointer_cast<CEgTpHeartBeatMsg>(pMessage);

			m_pcxtSender->OnHeartBeatMessage(pcxtConnection->GetConnectionID(),pHeartBeatMsg->m_ulMessageReceived,pHeartBeatMsg->m_ulMessageSend);
			return true;
		}
		break;

		case enCheckSubject:
		{
			CEgTpCheckSubjectWithNumMsgPtr pCheckSubjectMsg = boost::dynamic_pointer_cast<CEgTpCheckSubjectWithNumMsg>(pMessage);

			m_pcxtSender->OnCheckSubjectMessage(pcxtConnection->GetConnectionID(), pCheckSubjectMsg);
			return true;
		}
		break;

		default:
			return false;
	}			
	
	CEgLibTraceManager::Trace(LogDebug,_T("Router incoming processor"),_T("OnMessage completed"));
	return false;
}


/************************************************************************************************
	Utility methods
************************************************************************************************/
DWORD CRouter::GetRouterHostName(_bstr_t& bstrHostName)
{
	try
	{
		bstrHostName = _T("");

		CEgRegKey key;
		LONG lResult;
			
		lResult = key.Open(HKEY_LOCAL_MACHINE, SETTINGS_NETWORK_REGISTRY_KEY, KEY_READ);

		if (lResult == ERROR_SUCCESS)
		{
			lResult = key.QueryValue (bstrHostName,
				SETTINGS_NETWORK_ROUTER_HOST);
		}

		if (lResult != ERROR_SUCCESS)
		{
			bstrHostName = _T("localhost"); // use 'localhost' by default.
			lResult = ERROR_SUCCESS;
		}
		return lResult;
	}
	catch(_com_error er)
	{
		CEgLibTraceManager::Trace(LogError, _T("Router"),_T("Exception occured in GetRouterHostName. HRESULT : (%d)"),er.Error());
		return -1;
	}
}

DWORD CRouter::SetRouterHostName(_bstr_t bstrHostName)
{
	CEgRegKey key;

	LONG lResult = key.Open(HKEY_LOCAL_MACHINE, SETTINGS_NETWORK_REGISTRY_KEY, KEY_WRITE);
	
	if (lResult == ERROR_SUCCESS)
	{
		lResult = key.SetValue (bstrHostName,
			SETTINGS_NETWORK_ROUTER_HOST);
	}
	else
	{
		lResult = key.Create(HKEY_LOCAL_MACHINE, SETTINGS_NETWORK_REGISTRY_KEY);
		if (lResult == ERROR_SUCCESS)
		{
			lResult = key.SetValue (bstrHostName,
				SETTINGS_NETWORK_ROUTER_HOST);
		}
	}
	return lResult;
}

DWORD CRouter::GetSettingsFromReg(LPCTSTR KeyName,DWORD dwDefaultValue)
{
	CEgRegKey key;
	
	DWORD Value;
	LONG lResult;

	lResult = key.Open(HKEY_LOCAL_MACHINE, SETTINGS_NETWORK_REGISTRY_KEY, KEY_READ);

	if (lResult == ERROR_SUCCESS)
	{
		lResult = key.QueryValue (Value, KeyName);
	}

	if (lResult != ERROR_SUCCESS)
	{
		Value = dwDefaultValue;
	}
	return Value;
}

DWORD CRouter::GetTraceSettingsFromReg(LPCTSTR KeyName,LPCTSTR strDefaultValue,tstring& strValue)
{
	try
	{
		_bstr_t bstrValue;
		CEgRegKey key;
	
		LONG lResult;

		lResult = key.Open(HKEY_LOCAL_MACHINE, SETTINGS_LOGS_REGISTRY_KEY, KEY_READ);

		if (lResult == ERROR_SUCCESS)
		{
			lResult = key.QueryValue (bstrValue, KeyName);
		}

		if (lResult != ERROR_SUCCESS)
		{
			bstrValue = strDefaultValue;
		}
		strValue = (TCHAR*)bstrValue;
	}
	catch(_com_error)
	{
		return -1;
	}

	return ERROR_SUCCESS;
}

DWORD CRouter::GetTraceSettingsFromReg(LPCTSTR KeyName,DWORD dwDefaultValue)
{
	CEgRegKey key;
	
	DWORD Value;
	LONG lResult;

	lResult = key.Open(HKEY_LOCAL_MACHINE, SETTINGS_LOGS_REGISTRY_KEY, KEY_READ);

	if (lResult == ERROR_SUCCESS)
	{
		lResult = key.QueryValue (Value, KeyName);
	}

	if (lResult != ERROR_SUCCESS)
	{
		Value = dwDefaultValue;
	}
	return Value;
}


DWORD CRouter::SetSettingsToReg(LPCTSTR KeyName,DWORD dwValue)
{
	CEgRegKey key;
	
	LONG lResult = key.Open(HKEY_LOCAL_MACHINE, SETTINGS_NETWORK_REGISTRY_KEY, KEY_WRITE);
	
	if (lResult == ERROR_SUCCESS)
	{
		lResult = key.SetValue (dwValue, KeyName);
	}
	else
	{
		lResult = key.Create(HKEY_LOCAL_MACHINE, SETTINGS_NETWORK_REGISTRY_KEY);
		if (lResult == ERROR_SUCCESS)
		{
			lResult = key.SetValue (dwValue, KeyName);	
		}
	}

	return lResult;
}
DWORD CRouter::SetSettingsToReg(LPCTSTR KeyName,LPCTSTR strValue)
{
	CEgRegKey key;
	
	LONG lResult = key.Open(HKEY_LOCAL_MACHINE, SETTINGS_NETWORK_REGISTRY_KEY, KEY_WRITE);
	
	if (lResult == ERROR_SUCCESS)
	{
		lResult = key.SetValue (strValue, KeyName);
	}
	else
	{
		lResult = key.Create(HKEY_LOCAL_MACHINE, SETTINGS_NETWORK_REGISTRY_KEY);
		if (lResult == ERROR_SUCCESS)
		{
			lResult = key.SetValue (strValue, KeyName);	
		}
	}

	return lResult;
}

void CRouter::SetCheckHostsConnTimer(long lTimer)
{
	if(IsStopping())
		return;

	LARGE_INTEGER liDueTime;
	liDueTime.QuadPart = ((ULONGLONG)lTimer) * 1000 * 10000; // 100 ns discontinuity
	liDueTime.QuadPart *= -1;

	if (!SetWaitableTimer(m_hCheckHostsConnTimer, &liDueTime, 0, NULL, NULL, 0))
	{
		CEgLibTraceManager::Trace(LogInfo,_T("Router"),_T("Failed to set check hosts connection timer : waitable timer error (%d)"),(long)GetLastError());
	}
}

void CRouter::CheckHostsConnection()
{
	if(IsStopping())
		return;
	
	CEgLibTraceManager::Trace(LogInfo,_T("Router"),_T("Start CheckHostsConnection"));

	CEgGuid HostID;
	time_t CurTime = time(NULL);
	
	long CheckHostConnTimer = GetSettingsFromReg(SETTINGS_CHECK_HOSTS_CONN_PERIOD,CHECK_HOSTS_CONN_PERIOD);
	long CheckHostConnTimout = GetSettingsFromReg(SETTINGS_HOST_CONN_EXP_TIMEOUT,HOST_CONN_EXP_TIMEOUT);
		
	if(!m_listHosts.First(HostID))
	{
		CEgLibTraceManager::Trace(LogInfo,_T("Router"),_T("There are no one hosts connected to router"));
		SetCheckHostsConnTimer(CheckHostConnTimer);
		return;
	}

	do
	{
		CEgSmartPtr<CEgHostInfo> pHostInfo = m_listHosts.GetDataByKey(HostID);
		if((!pHostInfo) == false)
		{

			if( CurTime - pHostInfo->GetLastMessRecTime() > CheckHostConnTimer)
			{
			
				CEgConnection* pConnection = pHostInfo->GetConnection();
				if((!pConnection) == false)
				{
					CEgTpTestRequestMsg Msg;
					Msg.m_enMsgProp = (MsgPropertiesEnum)enMpUnspecified;
					Msg.m_enType = (MsgTypeEnum)enTestRequest;

					Msg.m_ulMessageReceived = pHostInfo->GetRecMessNum();
					Msg.m_ulMessageSend = pHostInfo->GetSendMessNum();

					CEgPacket Packet;
					if(!Msg.Pack(Packet))
						CEgLibTraceManager::Trace(LogError,_T("Router"),_T("Router can't pack TestRequest message for host {%.8X-...}."),pHostInfo->getHostKey().Data1);

					u_long nSize;
					void* pData = Packet.GetBuffer(nSize);
					
					if(pConnection->GetSocket() != INVALID_SOCKET && nSize)
					{
						DWORD dwRes = m_Driver.Send(pConnection->GetSocket(), pData, nSize);
						if(dwRes)
							CEgLibTraceManager::Trace(LogInfo,_T("Router"),_T("Router can't send testrequest message to host {%.8X-...}. Error (%d)"),pHostInfo->getHostKey().Data1,dwRes);
						else
							CEgLibTraceManager::Trace(LogInfo,_T("Router"),_T("Router send testrequest message to host {%.8X-...}. Error (%d)"),pHostInfo->getHostKey().Data1,dwRes);
					}
				}
			}

			if( CurTime - pHostInfo->GetLastMessRecTime() > CheckHostConnTimout)
			{
				CEgLibTraceManager::Trace(LogInfo,_T("Router"),_T("Connection for host {%.8X-...} expired. Logout host."), pHostInfo->getHostKey().Data1);
				
				CEgConnection* pConnection = pHostInfo->GetConnection();

				// unregister subjects from host
				m_HostInfoSubjectDatabase.UnregisterData(pHostInfo);
				
				m_listHosts.Remove(HostID);				
				
				if((!pConnection) == false)
				{
					try
					{
						u_long nID = pConnection->GetConnectionID().m_RemoteAddr.sin_addr.S_un.S_addr;
						m_listConnections.Remove(pConnection->GetSocket());
						CTpEventPtr pEvent (new CTpEvent());
						pEvent->m_enDispId = DISPID_IRouterEventsOnClientDisconnected;
						pEvent->m_bsParams = ::SysAllocStringLen(GetIPByAddr(nID), SysStringLen(GetIPByAddr(nID)));
						m_EventThread.PushEvent(pEvent);
					}
					catch(_com_error er)
					{
						CEgLibTraceManager::Trace(LogError,_T("Router"),_T("Exception occured in CheckHostsConnection. HRESULT : (%d)"),er.Error());
					}
				}
				
				// clear host message
				pHostInfo->ClearAllMessages();
			}
		}

	}while(m_listHosts.Next(HostID));

	SetCheckHostsConnTimer(CheckHostConnTimer);
	CEgLibTraceManager::Trace(LogInfo,_T("Router"),_T("CheckHostsConnection completed"));
}

/************************************************************************************************
	Service staff
************************************************************************************************/

inline void CRouter::ServiceMain(DWORD /* dwArgc */, LPTSTR* /* lpszArgv */)
{
    // Register the control request handler
    m_status.dwCurrentState = SERVICE_START_PENDING;
    m_hServiceStatus = RegisterServiceCtrlHandler(ROUTER_SERVICE, _Handler);
    if (m_hServiceStatus == NULL)
    {
        CEgLibTraceManager::Trace(LogError,_T("Router"),_T("Router ServiceMain Error. Handler not installed"));
        return;
    }
    SetServiceStatus(SERVICE_START_PENDING);

    m_status.dwWin32ExitCode = S_OK;
    m_status.dwCheckPoint = 0;
    m_status.dwWaitHint = 0;

    // When the ServiceDo function returns, the service has stopped.
    ServiceDo();

    SetServiceStatus(SERVICE_STOPPED);
    CEgLibTraceManager::Trace(LogError,_T("Router"),_T("Router Service stopped"));
}

inline void CRouter::Handler(DWORD dwOpcode)
{
    switch (dwOpcode)
    {
    case SERVICE_CONTROL_STOP:
		SetIsStopping();
        SetServiceStatus(SERVICE_STOP_PENDING);
		SetEvent(GetStopEventHandle());
        break;
    case SERVICE_CONTROL_PAUSE:
        break;
    case SERVICE_CONTROL_CONTINUE:
        break;
    case SERVICE_CONTROL_INTERROGATE:
        break;
    case SERVICE_CONTROL_SHUTDOWN:
        break;
    default:
        CEgLibTraceManager::Trace(LogError,_T("Router"),_T("Router Service Handler error. Bad service request"));
    }
}

_COM_SMARTPTR_TYPEDEF(IRouter, __uuidof(IRouter));

void WINAPI CRouter::_ServiceMain(DWORD dwArgc, LPTSTR* lpszArgv)
{	

#if _WIN32_WINNT >= 0x0400 & defined(_ATL_FREE_THREADED)
    HRESULT hRes = CoInitializeEx(NULL, COINIT_MULTITHREADED);
#else
    HRESULT hRes = CoInitialize(NULL);
#endif

	if(g_pRouter)
		g_pRouter->ServiceMain(dwArgc, lpszArgv);

	CoUninitialize();
}
void WINAPI CRouter::_Handler(DWORD dwOpcode)
{
	if(g_pRouter)
		g_pRouter->Handler(dwOpcode);
}

void CRouter::SetServiceStatus(DWORD dwState)
{
    m_status.dwCurrentState = dwState;
    ::SetServiceStatus(m_hServiceStatus, &m_status);
}

void CRouter::ServiceDo()
{  
	try
	{
		DWORD dwErr;
		if (dwErr = m_GrantedMessProcessor.Start())
		{
			CEgLibTraceManager::Trace(LogError,_T("Router"),_T("Router server can't start. Failed of granted message processor start."));
			throw Error(L"Transport can't start.", IID_IRouter, HRESULT_FROM_WIN32(dwErr));
		}

		if (dwErr = m_BufferedMessProcessor.Start())
		{
			CEgLibTraceManager::Trace(LogError,_T("Router"),_T("Router server can't start. Failed of buffered message processor start."));
			throw Error(L"Transport can't start.", IID_IRouter, HRESULT_FROM_WIN32(dwErr));
		}
		
		if (dwErr = m_NotBuffMessProcessor.Start())
		{
			CEgLibTraceManager::Trace(LogError,_T("Router"),_T("Router server can't start. Failed of not buffered message processor start."));
			throw Error(L"Transport can't start.", IID_IRouter, HRESULT_FROM_WIN32(dwErr));
		}
		
		if (dwErr = m_IncomMessProcessor.Start())
		{
			CEgLibTraceManager::Trace(LogError,_T("Router"),_T("Can't restart router server. Start of Incoming Message processor failed"));
			throw Error(L"Transport can't start.", IID_IRouter, HRESULT_FROM_WIN32(dwErr));
		}

		long CheckHostConnTimer = GetSettingsFromReg(SETTINGS_CHECK_HOSTS_CONN_PERIOD,CHECK_HOSTS_CONN_PERIOD);
		long RouterPort = GetSettingsFromReg(SETTINGS_LOCAL_ROUTER_PORT,ROUTER_PORT_NUMBER);
		long RouterAddr = GetSettingsFromReg(SETTINGS_LOCAL_ROUTER_ADDR,INADDR_ANY);		

	
		SetCheckHostsConnTimer(CheckHostConnTimer);
		m_bIsStarted = true;
		SetIsStopping(false);

		DWORD dwExtendedErrorCode = 0L;
		dwErr = m_Driver.Start(this, COINIT_MULTITHREADED, static_cast<unsigned short>(RouterPort), RouterAddr);
		if(dwErr != 0)
		{			
			CEgLibTraceManager::Trace(LogError,_T("Router"),_T("Router server can't start. Can't start TCP/IP Driver (error: %d, ext: %d)."), dwErr, dwExtendedErrorCode);
			// Also we should stop all other services which were stared before
			throw Error(L"Can't start TCP/IP Driver.", IID_IRouter, E_FAIL);
		}
	}
	catch(HRESULT)
	{
		CEgLibTraceManager::Trace(LogError,_T("Router"),_T("Router Start failed."));
		m_GrantedMessProcessor.Stop();

		m_BufferedMessProcessor.Stop();

		m_NotBuffMessProcessor.Stop();

		m_IncomMessProcessor.Stop();
		
		return;
	}
	
	CEgLibTraceManager::Trace(LogError,_T("Router"),_T("Router Service Started"));

    SetServiceStatus(SERVICE_RUNNING);

	HANDLE	hEvents[] = {GetStopEventHandle(), m_hCheckHostsConnTimer};
	
	bool IsStop = false;

	while (!IsStop)
	{
		DWORD dwRes = WaitForMultipleObjects(sizeof(hEvents) / sizeof(HANDLE),
			hEvents, FALSE, INFINITE);

		switch (dwRes)
		{
		case WAIT_OBJECT_0:			// Stop

			//SetServiceStatus(SERVICE_STOP_PENDING);			
			CEgLibTraceManager::Trace(LogError, _T("Router"), _T("Router Service stop pending."));
			IsStop = true;
			break;
				
		case WAIT_OBJECT_0 + 1:		// Check hosts connection

			CheckHostsConnection();
			break;

		default:
			break;
		}
	}


	m_HostInfoSubjectDatabase.ClearAllStorage();
	m_listHosts.Clear();
	m_listConnections.Clear();

	m_GrantedMessProcessor.Stop();

	m_BufferedMessProcessor.Stop();

	m_NotBuffMessProcessor.Stop();

	m_IncomMessProcessor.Stop();

	m_Driver.Stop();
}

BOOL CRouter::IsInstalled()
{
    BOOL bResult = FALSE;

    SC_HANDLE hSCM = ::OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);

    if (hSCM != NULL)
    {
        SC_HANDLE hService = ::OpenService(hSCM, ROUTER_SERVICE, SERVICE_QUERY_CONFIG);
        if (hService != NULL)
        {
            bResult = TRUE;
            ::CloseServiceHandle(hService);
        }
        ::CloseServiceHandle(hSCM);
    }
    return bResult;
}

BOOL CRouter::Install()
{
    if (IsInstalled())
        return TRUE;

    SC_HANDLE hSCM = ::OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
    if (hSCM == NULL)
    {
        MessageBox(NULL, _T("Couldn't open service manager"), ROUTER_SERVICE, MB_OK);
        return FALSE;
    }

    // Get the executable file path
    TCHAR szFilePath[_MAX_PATH];
    ::GetModuleFileName(NULL, szFilePath, _MAX_PATH);

    SC_HANDLE hService = ::CreateService(
        hSCM, ROUTER_SERVICE, ROUTER_SERVICE,
        SERVICE_ALL_ACCESS, SERVICE_WIN32_SHARE_PROCESS,
        SERVICE_DEMAND_START, SERVICE_ERROR_NORMAL,
        szFilePath, NULL, NULL, ROUTER_SERVICE_DEPEND, NULL, NULL);

    if (hService == NULL)
    {
        ::CloseServiceHandle(hSCM);
        MessageBox(NULL, _T("Couldn't create service"), ROUTER_SERVICE, MB_OK);
        return FALSE;
    }
	else
	{
		HMODULE hAdvApi = ::LoadLibrary(_T("Advapi32"));
		LPFN_ChangeServiceConfig2 pfnChangeServiceConfig2 = NULL; 
		if(hAdvApi)
		{
			pfnChangeServiceConfig2 = (LPFN_ChangeServiceConfig2)GetProcAddress(hAdvApi,ChangeServiceConfig2Name);
		
			if(pfnChangeServiceConfig2)
			{
				SERVICE_DESCRIPTION sd = { (PTSTR) ROUTER_SERVICE_DESC };
				pfnChangeServiceConfig2(hService, SERVICE_CONFIG_DESCRIPTION, &sd);
			}

			::FreeLibrary(hAdvApi);
		}
	}

    ::CloseServiceHandle(hService);
    ::CloseServiceHandle(hSCM);
    return TRUE;
}

BOOL CRouter::Uninstall()
{
    if (!IsInstalled())
        return TRUE;

    SC_HANDLE hSCM = ::OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);

    if (hSCM == NULL)
    {
        MessageBox(NULL, _T("Couldn't open service manager"), ROUTER_SERVICE, MB_OK);
        return FALSE;
    }

    SC_HANDLE hService = ::OpenService(hSCM, ROUTER_SERVICE, SERVICE_STOP | DELETE);

    if (hService == NULL)
    {
        ::CloseServiceHandle(hSCM);
        MessageBox(NULL, _T("Couldn't open service"), ROUTER_SERVICE, MB_OK);
        return FALSE;
    }
    SERVICE_STATUS status;
    ::ControlService(hService, SERVICE_CONTROL_STOP, &status);

    BOOL bDelete = ::DeleteService(hService);

	if(!bDelete)
	{
		DWORD dwErr = GetLastError();
		if(ERROR_SERVICE_MARKED_FOR_DELETE == dwErr)
			bDelete = TRUE;
	}
    ::CloseServiceHandle(hService);
    ::CloseServiceHandle(hSCM);

    if(bDelete)
        return TRUE;

    MessageBox(NULL, _T("Service could not be deleted"), ROUTER_SERVICE, MB_OK);
    return FALSE;
}


STDMETHODIMP CRouter::Start()
{
	if(!m_bIsStarted)
	{
		EgLib::CThread::Start();
		m_bIsStarted = true;
	}

	return S_OK;
}
STDMETHODIMP CRouter::Stop()
{
	if(!IsStopping() && m_bIsStarted)
	{
		EgLib::CThread::Stop();
		SetIsStopping();
		Sleep(500);
		m_bIsStarted = false;
	}
	return S_OK;
}


UINT CRouter::Run()
{
	HRESULT hr = CoInitializeEx(NULL, COINIT_MULTITHREADED);
	if(SUCCEEDED(hr))
	{
		try
		{
			m_EventThread.PushEvent(CTpEventPtr(new CTpEvent(DISPID_IRouterEventsOnRouterStarted, NULL)));

			ServiceDo();
			m_EventThread.PushEvent(CTpEventPtr(new CTpEvent(DISPID_IRouterEventsOnRouterStarted, NULL)));

			Sleep(100);
			m_EventThread.PushEvent(CTpEventPtr(new CTpEvent(DISPID_IRouterEventsOnTransportStopped, NULL)));

			Sleep(100);
			CEgLibTraceManager::Trace(LogError,_T("Router"),_T("Router Service stopped"));
		}
		catch(_com_error& e)
		{
			CEgLibTraceManager::Trace(LogError,_T("Router"),_T("Router Service Could not be started %s"), (LPCSTR)e.Description());
		}
	}
	else
	{
		CEgLibTraceManager::Trace(LogError,_T("Router"),_T("Router Service Could not be started: Unable to initialize object %d"), hr);
	}
	CoUninitialize();
	return 0;
}

// Router.h : Declaration of the CRouter

#ifndef __ROUTER_H_
#define __ROUTER_H_

#include "EgTcpDriver\egtcpdriver.h"
#include "CRtClasses.h"
#include "Message.h"

#include "MessageProcessor.h"
#include "EventThread.h"

using namespace __dev__;

#include "resource.h"       // main symbols
#include "TransportCP.h"

class CEgConnection;

class CEgHostInfo : public IEgTpRefCounter
{
private:
	friend class CEgTpHostInfoList;

private:
	CEgHostInfo( const CEgGuid& gGUID ) : m_guidKey(gGUID)
	{
		m_ulSendMessages = 0;
		m_ulReceiveMessageNum = 0;
		m_tmLastMessageReceive = time(NULL);
		m_tmLogon = time(NULL);
		m_ulSendConfirmMessages = 0;
		m_pConnection = NULL;
	};

	~CEgHostInfo()
	{
		m_storGuarantee.Clear();
		m_storBuffered.Clear();
		m_storNotBuffered.Clear();
		m_pConnection = NULL;
	}

public:
	enum StatusEnum 
	{ 
		enOnline,
		enOffline,
		enConnected,
		enDisconnected 
	};

public:

	CEgConnection* GetConnection() 
	{ 
		CAutoLock	Lock(m_Lock);
		return m_pConnection;
	}

	void SetConnection(CEgConnection* pConnection)
	{
		CAutoLock	Lock(m_Lock);
		m_pConnection = pConnection;		
	}

	void	SetStatus(StatusEnum Status)
	{ 
		CAutoLock	Lock(m_Lock);
		m_Status = Status;
	}
	
	const StatusEnum	GetStatus() 
	{ 
		CAutoLock	Lock(m_Lock);
		return m_Status; 
	}


	time_t GetLogonTime() const
	{
		CAutoLock	Lock(m_Lock);
		return m_tmLogon;
	}

	u_long GetRecMessNum() const
	{
		CAutoLock	Lock(m_Lock);
		return m_ulReceiveMessageNum; 
	}

	u_long IncRecMessNum()
	{
		CAutoLock	Lock(m_Lock);
		m_ulReceiveMessageNum++;
		return m_ulReceiveMessageNum;
	}

	const u_long GetSendMessNum() const
	{
		CAutoLock	Lock(m_Lock);
		return m_ulSendMessages;
	}
	
	const u_long GetConfirmedMessNum() const
	{
		CAutoLock lock(m_Lock);
		return m_ulSendConfirmMessages;
	}

	const u_long GetUnsentMessNum() const
	{
		CAutoLock lock(m_Lock);
		u_long ulUnsentMsg = m_storGuarantee.GetSize() - (m_ulSendMessages - m_ulSendConfirmMessages);
		return ulUnsentMsg;
	}

	time_t GetLastMessRecTime() const
	{
		CAutoLock	Lock(m_Lock);
		return m_tmLastMessageReceive;
	}

	time_t SetLastMessRecTime(time_t tmLastMessRec)
	{
		CAutoLock	Lock(m_Lock);
		m_tmLastMessageReceive = tmLastMessRec;
		return m_tmLastMessageReceive;
	}
	
	bool GetNextMessage(MsgProcessorTypeEnum Type,CEgTpBaseMessagePtr* ppMessage, u_long& keyMessage)
	{
		switch(Type)
		{
			case enMPTGuarantee:
			{
				CAutoLock	Lock(m_Lock);

				bool bRet = m_storGuarantee.GetMessage(m_ulSendMessages - m_ulSendConfirmMessages, ppMessage);
					
				if(bRet)
					keyMessage = m_ulSendMessages + 1;
				
				return bRet;
			}
			break;
			case enMPTBuffered:
				return m_storBuffered.GetMessage(ppMessage);
			break;
			case enMPTNotBuffered:
				return m_storNotBuffered.GetMessage(ppMessage);
			break;
		}
		return false;
	}
	void SetMessageSend(MsgProcessorTypeEnum Type, u_long keyMessage)
	{
		if(Type == enMPTGuarantee)
		{
			CAutoLock	Lock(m_Lock);				
			m_ulSendMessages = keyMessage;
		}
	}
	void ResendGrantedMessages(u_long ulMessageReceived)
	{
		CAutoLock	Lock(m_Lock);
	
		if(ulMessageReceived > m_ulSendConfirmMessages )
		{
			m_storGuarantee.RemoveBelow(ulMessageReceived - m_ulSendConfirmMessages);				
		}
		m_ulSendConfirmMessages = ulMessageReceived;
		m_ulSendMessages = ulMessageReceived;
	}

	void RemoveConfirmedGrantedMessages(u_long ulMessageReceived)
	{
		CAutoLock	Lock(m_Lock);
	
		if(ulMessageReceived > m_ulSendConfirmMessages )
		{
			m_storGuarantee.RemoveBelow(ulMessageReceived - m_ulSendConfirmMessages);				
		}
		m_ulSendConfirmMessages = ulMessageReceived;
	}
	
	bool PushMessage(CEgTpBaseMessagePtr pMessage)
	{
		switch(pMessage->m_enMsgProp)
		{
			case enMpGuarantee:
			{
				m_storGuarantee.Push(pMessage);
				return true;
			}
			break;
			case enMpBuffered:
			{
				m_storBuffered.Push(pMessage);
				return true;
			}
			break;
			case enMpNotBuffered:
			{
				CEgTpDataMsgPtr pDataMsg = boost::static_pointer_cast<CEgTpDataMsg>(pMessage);
				tstring tsSubject = CA2T((LPSTR)pDataMsg->m_strSubject.GetStr());
				m_storNotBuffered.Push(tsSubject, pMessage);
				return true;
			}
			break;
			default:
				return false;
			break;
		}
		return false;
	}
	
	void ClearAllMessages()
	{
		m_storGuarantee.Clear();
		m_storBuffered.Clear();
		m_storNotBuffered.Clear();
	}
	const CEgGuid& getHostKey() const
	{
		CAutoLock	Lock(m_Lock);
		return m_guidKey;
	}

private:		
	CEgGuid							m_guidKey;
	CEgSmartPtr<CEgConnection>		m_pConnection;
	mutable CLock					m_Lock;
	StatusEnum						m_Status;
	tstring							m_szName;	
	time_t							m_tmLastMessageReceive;
	time_t							m_tmLogon;
	
	// counters for Granted Messages ///////////////////////
	u_long							m_ulSendMessages;
	u_long							m_ulReceiveMessageNum;
	u_long							m_ulSendConfirmMessages;
	////////////////////////////////////////////////////////
	CEgTpGuaranteeMessageStorage	m_storGuarantee;
	CEgTpBufferedMessageStorage		m_storBuffered;
	CEgTpNotBufferedMessageStorage	m_storNotBuffered;
};

class CEgTpHostInfoList : public CTEgTpListSmartPtr< CEgGuid, CEgHostInfo >
{
public:
	CEgHostInfo* CreateHostInfo()
	{
		CAutoLock	Lock(m_Lock);

		GUID guid;
		HRESULT hr = CoCreateGuid(&guid);
		if(FAILED(hr))
			return NULL;

		CEgHostInfo* pNewEntry = new CEgHostInfo(guid);

		if (pNewEntry == NULL)
			return NULL;

		m_ListMap[guid] = pNewEntry;

		return m_ListMap[guid];
	}
};


class CEgConnection : public IEgTpRefCounter
{
private:
	friend class CEgTpConnectionList;

private:
	CEgConnection( const CONNECTION_ID& Connection, CEgTcpDriver* pDriver ) : m_ConnectionID(Connection),m_pDriver(pDriver)
	{
		m_pHostInfo = NULL;
		m_pHostInfo = NULL;
	};

	~CEgConnection()
	{
		m_storIncomingMsg.Clear();
		m_pHostInfo = NULL;
	}

	explicit CEgConnection(const CEgConnection& r) { memcpy(this, &r, sizeof(CEgConnection)); }

public:
	CEgHostInfo* GetHostInfo()
	{
		CAutoLock	Lock(m_Lock);
		return m_pHostInfo;
	}
	void SetHostInfo(CEgHostInfo* pHost)
	{
		CAutoLock	Lock(m_Lock);
		m_Buffer.Clear();
		m_pHostInfo = pHost;
	}
	void CloseSocket()
	{
		CAutoLock	Lock(m_Lock);
		m_Buffer.Clear();
		m_pDriver->Close(m_ConnectionID.m_Socket);
	}
	
	SOCKET GetSocket()
	{
		CAutoLock	Lock(m_Lock);
		return m_ConnectionID.m_Socket;
	}
	
	const CONNECTION_ID& GetConnectionID() const
	{
		CAutoLock	Lock(m_Lock);
		return	m_ConnectionID;
	}
	
	bool PushMessage(CEgTpBaseMessagePtr pMessage)
	{
		m_storIncomingMsg.Push(pMessage);
		return true;
	}
	
	bool GetNextMessage(MsgProcessorTypeEnum Type,CEgTpBaseMessagePtr* ppMessage, u_long keyMessage)
	{
		if(Type == enMPTIncoming)
			return m_storIncomingMsg.GetMessage(ppMessage);
		
		return false;
	}
	void SetMessageSend(MsgProcessorTypeEnum Type, u_long){};

	void ClearAllMessages()
	{
		m_storIncomingMsg.Clear();
		m_Buffer.Clear();
	}
	
public:
	CEgSocketBuffer					m_Buffer;
private:
	CONNECTION_ID					m_ConnectionID;
	CEgSmartPtr<CEgHostInfo>		m_pHostInfo;
	CEgTcpDriver*					m_pDriver;
	mutable EgLib::CLock			m_Lock;
	CEgTpBufferedMessageStorage		m_storIncomingMsg;
};

class CEgTpConnectionList : public CTEgTpListSmartPtr< SOCKET, CEgConnection >
{
private:
	typedef CTEgTpListSmartPtr< SOCKET, CEgConnection > _Parent;
public:
	CEgConnection* CreateConnection(const CONNECTION_ID& Connection, CEgTcpDriver* pDriver)
	{
		CAutoLock	Lock(m_Lock);

		CEgConnection* pNewEntry = new CEgConnection(Connection,pDriver);

		if (pNewEntry == NULL)
			return NULL;

		m_ListMap[Connection.m_Socket] = pNewEntry;

		return m_ListMap[Connection.m_Socket];
	}
	bool Remove(SOCKET sSocket, bool bRemoveAndClose = false)
	{
		if(bRemoveAndClose)
		{
			CEgSmartPtr<CEgConnection> spConnection = GetDataByKey(sSocket);
			if(!spConnection)
				return false;
			spConnection->CloseSocket();
		}
		return _Parent::Remove(sSocket);
	}
	CEgConnection* FindConnectionByIP(BSTR bsIP)
	{
		try
		{
			_bstr_t bstrIP = bsIP;
			u_long ulAddr = inet_addr((char*)bstrIP);

			CAutoLock	Lock(m_Lock);

			for(TKeyDataMap::iterator iter = m_ListMap.begin(); iter != m_ListMap.end(); iter++)
			{
				CEgSmartPtr<CEgConnection> spConnection = iter->second;
				if(ulAddr == spConnection->GetConnectionID().m_RemoteAddr.sin_addr.S_un.S_addr)
					return iter->second;
			}
		}
		catch(_com_error)
		{
			return NULL;
		}

		return NULL;
	}

};

/**************************************************************************************
	
	  Server COM Object of the Transport module.

**************************************************************************************/
class CRouter;

extern CRouter* g_pRouter;

/////////////////////////////////////////////////////////////////////////////
// CRouter
class ATL_NO_VTABLE CRouter : 
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CRouter, &CLSID_Router>,
	public ISupportErrorInfoImpl<&IID_IRouter>,
	public IConnectionPointContainerImpl<CRouter>,
	public IDispatchImpl<IRouter, &IID_IRouter, &LIBID_TRANSPORTLib>,
	public IEgTcpDriverEvents,	// TCP Driver events
	public CProxy_IRouterEvents< CRouter >,
	public EgLib::CThread
{
private:
	class CGrantedMessageProcessor : public CTEgTpBaseMessageProcessor<CRouter,CEgGuid,CEgHostInfo,enMPTGuarantee>
	{
	public:
		CGrantedMessageProcessor(CRouter* pRouter) : CTEgTpBaseMessageProcessor<CRouter,CEgGuid,CEgHostInfo,enMPTGuarantee>(pRouter){};

	private:		
		bool OnMessage( /*in*/ CEgHostInfo* pcxtHost,
						/*in*/ CEgTpBaseMessagePtr pMessage,
						/*in*/ u_long keyMessage);

	};

	class CBufferedMessageProcessor : public CTEgTpBaseMessageProcessor<CRouter,CEgGuid,CEgHostInfo, enMPTBuffered>
	{

	public:
		CBufferedMessageProcessor(CRouter* pRouter) : CTEgTpBaseMessageProcessor<CRouter, CEgGuid, CEgHostInfo,enMPTBuffered>(pRouter){};

	private:		
		bool OnMessage( CEgHostInfo* pcxtHost, CEgTpBaseMessagePtr pBaseMessage,u_long);
	};

	class CNotBufferedMessageProcessor : public CTEgTpBaseMessageProcessor<CRouter,CEgGuid,CEgHostInfo,enMPTNotBuffered>
	{
	public:
		CNotBufferedMessageProcessor(CRouter* pRouter) : CTEgTpBaseMessageProcessor<CRouter,CEgGuid,CEgHostInfo,enMPTNotBuffered>(pRouter){};

	private:		
		bool OnMessage(CEgHostInfo* pcxtHost, CEgTpBaseMessagePtr pBaseMessage,u_long);
	};

	class CIncomingMessageProcessor : public CTEgTpBaseMessageProcessor<CRouter,SOCKET,CEgConnection,enMPTIncoming>
	{
	public:
		CIncomingMessageProcessor(CRouter* pRouter) : CTEgTpBaseMessageProcessor<CRouter,SOCKET,CEgConnection,enMPTIncoming>(pRouter){};		
	private:
		bool OnMessage(	CEgConnection* pcxtConnection, CEgTpBaseMessagePtr pBaseMessage,u_long);
	};

	friend class CGrantedMessageProcessor;
	friend class CBufferedMessageProcessor;
	friend class CNotBufferedMessageProcessor;
	friend class CIncomingMessageProcessor;

	typedef CTEgTpDataSubjectsDatabase< CEgSmartPtr< CEgHostInfo > >	THostInfo2SubjectDB;

public:
	CRouter():m_GrantedMessProcessor(this),m_BufferedMessProcessor(this),m_NotBuffMessProcessor(this), 
		m_IncomMessProcessor(this),m_IsStopping(false), m_EventThread(this)
	{
	}

	HRESULT FinalConstruct();
	void	FinalRelease();

DECLARE_CLASSFACTORY_SINGLETON(CRouter)

DECLARE_REGISTRY_RESOURCEID(IDR_ROUTER)

DECLARE_NOT_AGGREGATABLE(CRouter)
DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CRouter)
	COM_INTERFACE_ENTRY(IRouter)
 	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY(IConnectionPointContainer)
	COM_INTERFACE_ENTRY_IMPL(IConnectionPointContainer)
END_COM_MAP()
BEGIN_CONNECTION_POINT_MAP(CRouter)
CONNECTION_POINT_ENTRY(DIID__IRouterEvents)
END_CONNECTION_POINT_MAP()

public:
// IRouter
	STDMETHOD(get_HostStatistic)(/*in*/ BSTR bsHostIP, /*[out, retval]*/ IHostStatisticData** ppStatistic);
	STDMETHOD(get_ActiveHosts)(/*[out, retval]*/ LPSAFEARRAY* ppHostsIPList);

	STDMETHOD(Start)(void);
	STDMETHOD(Stop)(void);

/************************************************************************************************
	Service staff
************************************************************************************************/
public:
	static void WINAPI _ServiceMain(DWORD dwArgc, LPTSTR* lpszArgv);
    static void WINAPI _Handler(DWORD dwOpcode);

    static BOOL IsInstalled();    
	static BOOL Install();
    static BOOL Uninstall();

	void ServiceMain(DWORD dwArgc, LPTSTR* lpszArgv);
    void Handler(DWORD dwOpcode);
	void ServiceDo();
    void SetServiceStatus(DWORD dwState);
private:

    SERVICE_STATUS_HANDLE m_hServiceStatus;
    SERVICE_STATUS m_status;


/************************************************************************************************
	Utility methods
************************************************************************************************/
public:
	static DWORD GetRouterHostName(_bstr_t& bstrHostName);
	static DWORD SetRouterHostName(_bstr_t bstrHostName);
	static DWORD GetSettingsFromReg(LPCTSTR KeyName,DWORD dwDefaultValue);
	static DWORD SetSettingsToReg(LPCTSTR KeyName,DWORD dwValue);
	static DWORD SetSettingsToReg(LPCTSTR KeyName,LPCTSTR strValue);
	static DWORD GetTraceSettingsFromReg(LPCTSTR KeyName,LPCTSTR strDefaultValue,tstring& strValue);
	static DWORD GetTraceSettingsFromReg(LPCTSTR KeyName,DWORD dwDefaultValue);


// IEgTcpDriverEvents
private:
	virtual BOOL WINAPI OnConnect(const CONNECTION_ID& Connection, BOOL bIncome);

	virtual void WINAPI OnConnectionFailed(const CONNECTION_ID& Connection) ;

	virtual void WINAPI OnClose(const CONNECTION_ID& Connection, const DWORD & dwError);

	virtual void WINAPI OnReceive(	const CONNECTION_ID&	Connection, 
									const LPVOID 			pData,
									const DWORD				dwDataLen);
// Internal functions
	void OnMessage(	const CONNECTION_ID&	Connection,
					const LPVOID			pData,
					const DWORD				dwDataLen);

	void OnLogonHostMessage(const CONNECTION_ID& Connection);
	void OnReLogonMessage(const CONNECTION_ID& Connection, const CEgGuid& HostID, u_long HostMsgReceived);
	void OnReSendMsg(const CONNECTION_ID& Connection, const CEgGuid& HostID, u_long HostMsgReceived);
	void OnLogoutHostMessage(const CONNECTION_ID& Connection, const CEgGuid& HostID, u_long HostMsgReceived);
	void OnSetSubjectMessage(const CONNECTION_ID& Connection, const tstring& szSubject, u_long HostMsgReceived,u_long MessNum);	
	void OnRemoveSubjectMessage(const CONNECTION_ID& Connection, const tstring& szSubject, u_long HostMsgReceived,u_long MessNum);
	void OnDataMessage(const CONNECTION_ID& Connection, CEgTpDataWithNumMsgPtr pDataMsg);
	void OnTestRequestMessage(const CONNECTION_ID& Connection, u_long HostMsgReceived, u_long HostMsgSend);
	void OnHeartBeatMessage(const CONNECTION_ID& Connection, u_long HostMsgReceived, u_long HostMsgSend);
	void OnCheckSubjectMessage(const CONNECTION_ID& Connection, CEgTpCheckSubjectWithNumMsgPtr pCheckSubjectMsg);

	BOOL CheckHostValidity(CEgHostInfo* pHostInfo, u_long HostMsgReceived,
		u_long MsgNum = 0,MsgPropertiesEnum enProp = enMpUnspecified);

	void SetCheckHostsConnTimer(long lTimer);
	void CheckHostsConnection();

	void SetIsStopping(bool IsStopping = true)
	{
		CAutoLock	Lock(m_MembersLock);
		m_IsStopping = IsStopping;
	}
	bool IsStopping() const
	{
		CAutoLock	Lock(m_MembersLock);
		return m_IsStopping;
	}

public:

	bool GetReceiverCxt(const CEgGuid& guidKey,CEgHostInfo** ppcxtHost)
	{
		if(!ppcxtHost)
			return false;

		*ppcxtHost = m_listHosts.GetDataByKey(guidKey);
		if(!*ppcxtHost)
			return false;
	
		(*ppcxtHost)->AddRef();
		
		return true;
	};

	bool GetReceiverCxt(const SOCKET& socket,CEgConnection** ppcxtConnection)
	{
		if(!ppcxtConnection)
			return false;

		*ppcxtConnection = m_listConnections.GetDataByKey(socket);
		if(!*ppcxtConnection)
			return false;

		(*ppcxtConnection)->AddRef();

		return true;
	};

/************************************************************************************************
	CEgTcpDriver methods wrap
************************************************************************************************/
public:
	static _bstr_t GetHostByAddr(long lAddress)
	{
		char szHostName[MAX_HOST_NAME_LEN];
		
		DWORD dwRes = CEgTcpDriver::ResolveName(lAddress, 
													szHostName, MAX_HOST_NAME_LEN);
		
		if (dwRes == 0)
			return szHostName;
		else 
			return inet_ntoa(*(in_addr*)&lAddress);
	}

	static _bstr_t GetIPByAddr(long lAddress)
	{
		return inet_ntoa(*(in_addr*)&lAddress);
	}

protected:
	virtual UINT Run();

// Internal members
private:
	CEgTcpDriver					m_Driver;
	
	//CHandle							m_hStopEvent;
	CHandle							m_hCheckHostsConnTimer;
	
	BOOL							m_bIsStarted;
	CEgTpConnectionList				m_listConnections;
	CEgTpHostInfoList				m_listHosts;
	THostInfo2SubjectDB				m_HostInfoSubjectDatabase;
	CGrantedMessageProcessor		m_GrantedMessProcessor;
	CBufferedMessageProcessor		m_BufferedMessProcessor;
	CNotBufferedMessageProcessor	m_NotBuffMessProcessor;
	CIncomingMessageProcessor		m_IncomMessProcessor;

	CRouterEventThread				m_EventThread;

	bool							m_IsStopping;
	mutable	CLock					m_MembersLock;

};

#endif //__ROUTER_H_

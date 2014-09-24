// Host.h : Declaration of the CHost

#ifndef __HOST_H_
#define __HOST_H_

#include "resource.h"       // main symbols

#include "EgTcpDriver\egtcpdriver.h"
#include "CRtClasses.h"
#include "Message.h"
#include "Session.h"

#include "MessageProcessor.h"
#include "TransportCP.h"
#include "EventThread.h"

using namespace __dev__;

class CHost;

extern CHost* g_pHost;


/////////////////////////////////////////////////////////////////////////////
// CHost
class ATL_NO_VTABLE CHost : 
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CHost, &CLSID_Host>,
	public ISupportErrorInfoImpl<&IID_IHost>,
	public IConnectionPointContainerImpl<CHost>,
	public IDispatchImpl<IHost, &IID_IHost, &LIBID_TRANSPORTLib>,
	public IEgTcpDriverEvents,	// TCP Driver events
	public CProxy_IHostEvents< CHost >,
	public EgLib::CThread
{
private:

	class CGrantedMessageProcessor : public CTEgTpBaseMessageProcessor<CHost,int, CHost,enMPTGuarantee>
	{
	public:
		CGrantedMessageProcessor(CHost* pHost) : CTEgTpBaseMessageProcessor<CHost,int, CHost,enMPTGuarantee>(pHost) {};

	private:
		bool OnMessage( /*in*/ CHost* cxtReceiver,
						/*in*/ CEgTpBaseMessagePtr pMessage,
						/*in*/ u_long keyMessage);

	};

	class CBufferedMessageProcessor : public CTEgTpBaseMessageProcessor<CHost,int,CHost, enMPTBuffered>
	{

	public:
		CBufferedMessageProcessor(CHost* pHost) : CTEgTpBaseMessageProcessor<CHost,int,CHost, enMPTBuffered>(pHost) {};

	private:
		bool OnMessage( CHost* cxtReceiver, CEgTpBaseMessagePtr pBaseMessage,u_long);
	};

	class CNotBufferedMessageProcessor : public CTEgTpBaseMessageProcessor<CHost,int,CHost, enMPTNotBuffered>
	{
	public:
		CNotBufferedMessageProcessor(CHost* pHost) : CTEgTpBaseMessageProcessor<CHost,int,CHost, enMPTNotBuffered>(pHost){};
	
	private:
		bool OnMessage(CHost* cxtReceiver, CEgTpBaseMessagePtr pBaseMessage,u_long);
	};

	class CLocalMessageProcessor : public CTEgTpBaseMessageProcessor<CHost,CEgGuid,CSession, enMPTLocal>
	{
	public:
		CLocalMessageProcessor(CHost* pHost) : CTEgTpBaseMessageProcessor<CHost,CEgGuid,CSession, enMPTLocal>(pHost){};
	
	private:
		bool OnMessage(CSession* cxtReceiver, CEgTpBaseMessagePtr pBaseMessage,u_long);
	};


	friend class CGrantedMessageProcessor;
	friend class CBufferedMessageProcessor;
	friend class CNotBufferedMessageProcessor;
	friend class CLocalMessageProcessor;
	friend class CSession;

	typedef CTEgTpDataSubjectsDatabase<CSession*>	TSession2SubjectDB;

public:
	CHost():m_GrantedMessProcessor(this),m_BufferedMessProcessor(this),
		m_NotBuffMessProcessor(this), m_LocalMessProcessor(this),
		m_IsStopping(false), m_IsLocalRouter(false), m_EventThread(this), m_SessionEventThread()
	{
	}

	HRESULT FinalConstruct();
	void	FinalRelease();

DECLARE_CLASSFACTORY_SINGLETON(CHost)

DECLARE_REGISTRY_RESOURCEID(IDR_HOST)
DECLARE_NOT_AGGREGATABLE(CHost)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CHost)
	COM_INTERFACE_ENTRY(IHost)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY(IConnectionPointContainer)
	COM_INTERFACE_ENTRY_IMPL(IConnectionPointContainer)
END_COM_MAP()

BEGIN_CONNECTION_POINT_MAP(CHost)
CONNECTION_POINT_ENTRY(DIID__IHostEvents)
END_CONNECTION_POINT_MAP()



// IHost
public:
	STDMETHOD(CreateSession)(/*[out]*/ISession** ppSession);
	STDMETHOD(Logout)();
	STDMETHOD(Logon)(BSTR bsServerName);
	STDMETHOD(get_Status)(/*[out, retval]*/ HostStatusEnum* pVal);
	STDMETHOD(get_Statistic)(/*[out, retval]*/ IHostStatisticData** ppStatistic);
	STDMETHOD(get_RouterName)(/*[out, retval]*/ BSTR* pRouterName);

// IEgTcpDriverEvents
private:
	virtual BOOL WINAPI OnConnect(const CONNECTION_ID& Connection, BOOL bIncome);

	virtual void WINAPI OnConnectionFailed(const CONNECTION_ID& Connection);

	virtual void WINAPI OnClose(const CONNECTION_ID& Connection, const DWORD & dwError);

	virtual void WINAPI OnReceive(	const CONNECTION_ID&	Connection, 
									const LPVOID			pData,
									const DWORD				dwDataLen);
//ISession wrap

	STDMETHOD(Subscribe)(BSTR Subject, CSession* pSession);

	STDMETHOD(Unsubscribe)(BSTR Subject, CSession* pSession);
	
	STDMETHOD(SendMessage)(BSTR Subject, MsgPropertiesEnum MsgProp, 
		BSTR Message, CSession* pSession);

	STDMETHOD(CheckSubjectStatus)(BSTR Subject, CSession* pSession);

	void DestroySession(CSession* pSession);

public:
	time_t GetLogonTime() const
	{
		CAutoLock	Lock(m_MembersLock);
		return m_tmLogon;
	}

	time_t GetLastMessRecTime() const
	{
		CAutoLock	Lock(m_MembersLock);
		return m_tmLastMessageRec;
	}
	bool IsLogoned() const
	{
		CAutoLock	Lock(m_MembersLock);
		return m_IsLogoned;
	}

	bool SetIsLogoned(bool IsLogoned)
	{
		CAutoLock	Lock(m_MembersLock);
		m_IsLogoned = IsLogoned;
		return m_IsLogoned;
	}


	bool BeginLogon()
	{
		CAutoLock	Lock(m_MembersLock);
		
		if(m_IsLogonStart)
			return false;
		else
		{
			QueueReconnectAttempt(m_ConnTimeout);
			m_IsLogonStart = true;
			return true;
		}
	}

	bool IsLogonStart() const
	{
		CAutoLock	Lock(m_MembersLock);
		return m_IsLogonStart;
	}
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

	void EndLogon()
	{
		CAutoLock	Lock(m_MembersLock);
		CancelWaitableTimer(m_hReconnectTimeout);
		CancelWaitableTimer(m_hSendTestRequest);
		m_IsLogonStart = false;
	}

	void LockSend()
	{
		m_SendLock.Lock();
	}

	void UnLockSend()
	{
		m_SendLock.Unlock();
	}

	bool IsLocalRouter() const
	{
		CAutoLock	Lock(m_MembersLock);
		return m_IsLocalRouter;
	}

	bool SetIsLocalRouter(bool IsLocal)
	{
		CAutoLock	Lock(m_MembersLock);
		m_IsLocalRouter = IsLocal;
		return m_IsLocalRouter;
	}
private:
	STDMETHOD(ReLogon)();
	UINT Run();

	void OnMessage(	const LPVOID 		pData,
					const DWORD			dwDataLen);
	void OnDataMessage(CEgTpDataMsgPtr pDataMsg, u_long ulMessSend,u_long ulMessReceived );
	void OnTestRequestMessage(u_long RouterMsgReceived,u_long RouterMsgSend);
	void OnHeartBeatMessage(u_long RouterMsgReceived,u_long RouterMsgSend);
	void OnCheckSubjectMessage(CEgTpCheckSubjectMsgPtr pCheckSubjectMsg,  u_long ulMessSend,u_long ulMessReceived );

	void SendTestRequest();
	void SendHeartBeatMsg();
	void SendFirstMessage();
	void SendResendRequest(u_long ulNumLastReceiveMess);
	void QueueReconnectAttempt(long lTimer); // set reconnect timer
	void QueueSendTestRequestTimer(long lTimer);

private:
	CHandle							m_hDisconnectEvent;
	CHandle							m_hConnectEvent;
	CHandle							m_hReconnectTimeout;
	CHandle							m_hSendTestRequest;

	CEgTcpDriver					m_Driver;
	CONNECTION_ID					m_Router;
	bool							m_IsLogoned;
	bool							m_IsLogonStart;/* block variable
												   to deny simulteniously access to 
												   logon stuff*/
	bool							m_IsStopping;

	bool							m_IsLocalRouter;
	
	CHostEventThread				m_EventThread;
	CSessionEventThread				m_SessionEventThread;


	u_long							m_ConnTimeout;
	u_long							m_ConnLoseTimeout;
	u_long							m_TestRequestTimeout;
	u_long							m_StatisticTimer;
	u_long							m_ReconnectTime;

	CEgGuid							m_guidKey;
	time_t							m_tmLastMessageRec;
	time_t							m_tmLogon;
	mutable	CLock					m_MembersLock;
	mutable	CLock					m_SendLock;
	CGrantedMessageProcessor		m_GrantedMessProcessor;
	CBufferedMessageProcessor		m_BufferedMessProcessor;
	CNotBufferedMessageProcessor	m_NotBuffMessProcessor;
	CLocalMessageProcessor			m_LocalMessProcessor;

	TSession2SubjectDB				m_SessionSubjectDatabase;
	CEgTpSessionList				m_listSessions;
	_bstr_t							m_bstrServerName;

	CEgSocketBuffer					m_SocketBuffer;

// router context member/////////////////
	CEgTpGuaranteeMessageStorage	m_storGuarantee;
	CEgTpBufferedMessageStorage		m_storBuffered;
	CEgTpNotBufferedMessageStorage	m_storNotBuffered;

	u_long							m_ulSendMessages;
	u_long							m_ulSendConfirmMessages;
	u_long							m_ulReceiveMessages;

	mutable	CLock					m_RouterCxtLock;

public:
	
	bool GetReceiverCxt(int iKey,CHost** ppHost)
	{
		if(!ppHost)
			return false;

		if(IsLogoned())
		{
			*ppHost = this;

			(*ppHost)->AddRef();
		
			return true;
		}
		*ppHost = NULL;
		return false;
	};

	bool GetReceiverCxt(const CEgGuid& keySession,CSession** ppSession)
	{
		if(!ppSession)
			return false;

		*ppSession = m_listSessions.GetDataByKey(keySession);
		if(!*ppSession)
			return false;
	
		(*ppSession)->AddRef();

		return true;
	};

// router context methods/////////////////
	bool GetNextMessage(MsgProcessorTypeEnum Type, CEgTpBaseMessagePtr* ppMessage, u_long& keyMessage)
	{
		switch(Type)
		{
			case enMPTGuarantee:
			{
				CAutoLock lock(m_RouterCxtLock);
	
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
	
	void SetMessageSend(MsgProcessorTypeEnum Type,  u_long keyMessage)
	{
		if(Type == enMPTGuarantee)
		{
			CAutoLock lock(m_RouterCxtLock);				
			m_ulSendMessages = keyMessage;
		}
	}


	void ResendGrantedMessages(u_long ulMessageReceived)
	{
		CAutoLock lock(m_RouterCxtLock);
	
		if(ulMessageReceived > m_ulSendConfirmMessages )
		{
			m_storGuarantee.RemoveBelow(ulMessageReceived - m_ulSendConfirmMessages);				
		}
		m_ulSendConfirmMessages = ulMessageReceived;
		m_ulSendMessages = ulMessageReceived;
	}

	void RemoveConfirmedGrantedMessages(u_long ulMessageReceived)
	{
		CAutoLock lock(m_RouterCxtLock);
	
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
				CEgTpDataMsgPtr pDataMsg = boost::dynamic_pointer_cast<CEgTpDataMsg>(pMessage);
				tstring tsSubject = CA2T((LPSTR)pDataMsg->m_strSubject.GetStr());

				m_storNotBuffered.Push(tsSubject,pMessage);
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

	void ResetAllCounters()
	{
		CAutoLock	Lock(m_RouterCxtLock);
		m_ulSendMessages = 0;
		m_ulReceiveMessages = 0;
		m_ulSendConfirmMessages = 0;
	}


	u_long GetRecMessNum() const
	{
		CAutoLock lock(m_RouterCxtLock);
		return m_ulReceiveMessages; 
	}

	u_long IncRecMessNum()
	{
		CAutoLock lock(m_RouterCxtLock);
		m_ulReceiveMessages++;
		return m_ulReceiveMessages;
	}

	const u_long GetSendMessNum() const
	{
		CAutoLock lock(m_RouterCxtLock);
		return m_ulSendMessages;
	}
	const u_long GetConfirmedMessNum() const
	{
		CAutoLock lock(m_RouterCxtLock);
		return m_ulSendConfirmMessages;
	}
	const u_long GetUnsentMessNum() const
	{
		CAutoLock lock(m_RouterCxtLock);
		u_long ulUnsentMsg = m_storGuarantee.GetSize() - (m_ulSendMessages - m_ulSendConfirmMessages);
		return ulUnsentMsg;
	}

////////////////////////////////////////

};

#endif //__HOST_H_

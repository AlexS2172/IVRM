// MMHHTAdapter.h : Declaration of the CMMHHTAdapter

#pragma once
#include "resource.h"       // main symbols
#include "egtcpdriver.h"
#include "DataQueue.h"
#include "MMHHMessage.h"
using namespace tnt_msg;

#include "settings.h"

// IMMMHHTAdapter
[
	object,
	uuid("BCEE8E02-8EB6-4B5C-9D81-33BC9B71AEBF"),
	dual,	helpstring("IMMMHHTAdapter Interface"),
	pointer_default(unique)
]
__interface IMMHHTAdapter : IDispatch
{
	[id(1), helpstring("method Logon")] HRESULT Logon([in]struct ConnectionSettings* ConnSettings);
	[id(2), helpstring("method Logoff")] HRESULT Logoff();
	[id(3), helpstring("method TradeAdd")] HRESULT TradeAdd([in]struct TNTData* Data, [out, retval]struct MessageDescriptor* TradeID );
};


// _IMMMHHTAdapterEvents
[
	dispinterface,
	uuid("3644090F-E7D6-48B1-B77A-43AC68820CC1"),
	helpstring("_IMMHHTAdapterEvents Interface")
]

__interface _IMMHHTAdapterEvents
{
	[id(1), helpstring("method TerminateConnection")]	HRESULT TerminateConnection();
	[id(2), helpstring("method TradeNotification")]		HRESULT TradeNotification([in] struct TNTData* pData);
	[id(3), helpstring("method LogonResponse")]			HRESULT LogonResponse([in]LONG sCode, [in , satype("struct ErrorDescription")] SAFEARRAY** saErrors);
	[id(4), helpstring("method LogoffResponse")]		HRESULT LogoffResponse([in]LONG sCode, [in , satype("struct ErrorDescription")] SAFEARRAY** saErrors);
	[id(5), helpstring("method TradeAddResponse")]		HRESULT TradeAddResponse([in]struct ACKData* pData, [in]LONG sCode, [in, satype("struct ErrorDescription")] SAFEARRAY** saErrors);
	[id(6), helpstring("method ConnectorError")]		HRESULT ConnectorError([in]LONG sCode, [in]BSTR bsMessage);
};


// CMMHHTAdapter
	
[
	coclass,
	threading("free"),
	support_error_info("IMMHHTAdapter"),
	event_source("com"),
	vi_progid("TNTConnector.MMHHTAdapter"),
	progid("TNTConnector.MMHHTAdapter.1"),
	version(1.0),
	uuid("4FCA0EDC-37FE-4D17-B8C5-46E1E2E58B9B"),
	helpstring("MMHHTAdapter Class")
]
class ATL_NO_VTABLE CMMHHTAdapter : 
	public IMMHHTAdapter,
	public IEgTcpDriverEvents,
	public CTNTRoot
{
private:
	enum enErrmessageResponseType
	{
		enLogonResponse,
		enLogoffResponse,
		enTradeAddResponse
//		,enConnectorError
	};

	enum enLoggingStatus
	{
		enLoggedOff = 0,
		enLoggedOn = 1
//		,enLoggingOn = 2
		//		,enConnectorError
	};
public:
	CMMHHTAdapter();
	~CMMHHTAdapter();
	__event __interface _IMMHHTAdapterEvents;

	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct();
	void FinalRelease() ;	
	typedef std::vector<CMMHHMessage*> COMMANDS_TYPE;
public:
	// IMMHHTAdapter implementation
	STDMETHOD(Logon)(ConnectionSettings* pSettings);
	STDMETHOD(Logoff)();
	STDMETHOD(TradeAdd)(TNTData* pData,/* BSTR* */MessageDescriptor* tradeID);

	//IEgTcpDriverEvents implementation
	virtual BOOL WINAPI OnConnect(const CONNECTION_ID& Connection, BOOL bIncoming);
	virtual void WINAPI OnConnectionFailed(const CONNECTION_ID& Connection);
	virtual void WINAPI OnClose(const CONNECTION_ID& Connection, const DWORD & dwError);
	virtual void WINAPI OnReceive(const CONNECTION_ID& Connection, const LPVOID pData, const DWORD dwDataLen);

public:
	virtual bool OnAcknowlegement(CAcknowlegementMessage* pData);
	virtual bool OnTNTData(CTNTMessage* pData);
	virtual bool OnTerminateConnection();

private:
	bool GetErrorDescription(DWORD dwErr, _bstr_t *pbsErrDesc);
	void SendMessage(CMMHHMessage*pMessage); // Send message to POETS 
	void OnMessage(CMMHHMessage*pMessage); // Get Message from the PORTS
	void PostMessage(CMMHHMessage* pMessage); // PutMessage to outgoing queue

	HRESULT LaunchWorkingThreads();

	void SetLoggingStatus(enLoggingStatus newStatus = enLoggedOn);
	bool IsStopped();
	enLoggingStatus GetLoggingStatus();

	static UINT WINAPI _ListenerThread(LPVOID pData);
	static UINT WINAPI _SenderThread(LPVOID pData);

	void RaiseErrorMessage(enErrmessageResponseType ResponseType, long lSeqNumber, HRESULT sCode, BSTR Description);
	DWORD InitializeAndConnectDriver(_bstr_t* pbMess);

	DWORD PostLogOnMessage();
	DWORD PostLogOffMessage(_bstr_t *pbsErrMess);
	DWORD PostTradeAddMessage(CTNTMessage& Data, _bstr_t *pbsErrMess);

	HRESULT raise__TradeAddResponse (CComRecPtr<ACKData>& dataRec, ::LONG sCode, ::SAFEARRAY* errDescriptions);
	HRESULT raise__TradeNotification( CComRecPtr<TNTData>& dataRec); 
	HRESULT raise__LogonResponse(::LONG sCode,::SAFEARRAY* errDescription); 
	HRESULT raise__LogoffResponse(::LONG sCode,::SAFEARRAY* errDescription);

	HRESULT raise__ConnectorError(::LONG sCode,::BSTR errDescription);
	
	CEgTcpDriver m_TcpDriver;
//	HANDLE m_hEventConnectionStatusChanged;
//	HANDLE m_hEventDataReceived;
	SOCKET m_MMHHISocket;
//	SOCKET m_PrevoiusMMHHISocket;
	HANDLE m_hEventStop;
	HANDLE m_hTCPConnectedEvent;

	HANDLE m_hListenerThread;
	HANDLE m_hSenderThread;


	LONG m_lLoggingStatus;
	volatile LONG m_LastSequenceNo;
	CFixBuffer	m_InputStream;
	CDataQueueBuffered<CMMHHMessage*>	m_queIncoming;
	CDataQueueBuffered<CMMHHMessage*>   m_queOutgoing;
	COMMANDS_TYPE m_ValidIncomingCommands;
//Settings
	CSettings m_Settings;

	bool m_bTCPDriverInitialized;
	long m_lLogOnSequenceNumber;
	long m_lLogOffSequenceNumber;
	bool WaitUntilTCPConnected();

};


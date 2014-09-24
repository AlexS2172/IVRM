// TNTServer.h : Declaration of the CTNTServer

#pragma once
#include "resource.h"       // main symbols
#include "..\TNT Connector\egtcpdriver.h"
#include "..\TNT Connector\DataQueue.h"
#include "..\TNT Connector\PeerInfo.h"

using namespace tnt_msg;
// ITNTServer



[
	uuid(92473FAC-0768-44e5-8808-1E761637BCCF),
	export

]
struct TNTServerData
{
	[helpstring("Id")]							BSTR			Id;
	[helpstring("TradeID (7 characters)")]		BSTR			TradeID;
	[helpstring("isBuy")]						VARIANT_BOOL	isBuy;
	[helpstring("TradeDateTime")]				DATE			TradeDateTime;
	[helpstring("TradeQuantity")]				LONG			TradeQuantity;
	[helpstring("isCall")]						VARIANT_BOOL	isCall;
	[helpstring("OptionSymbol (6 characters)")]	BSTR			OptionSymbol;
	[helpstring("OptionExpDate")]				DATE			OptionExpDate;
	[helpstring("Strike")]						DOUBLE			Strike;
	[helpstring("PremiumPrice")]				DOUBLE			PremiumPrice;
	[helpstring("ExecutingFirm (4 characters)")]BSTR			ExecutingFirm;
	[helpstring("ExecutingMember (4 characters)")]	BSTR		ExecutingMember;
	[helpstring("CMTA (4 characters)")]			BSTR			CMTA;
	[helpstring("Quantifier (1 character)")]	BSTR			Quantifier;
	[helpstring("ContraFirm (4 characters)")]   BSTR			ContraFirm;
	[helpstring("ContraMember (4 characters)")] BSTR			ContraMember;	
	[helpstring("SpreadIndicator(1 character)")]BSTR			SpreadIndicator;
	[helpstring("OptionalData (13 characters)")]BSTR			OptionalData;
};


[
	object,
	uuid("F056562D-4EDB-4541-95DB-EC71B84A8E89"),
	dual,	helpstring("ITNTServer Interface"),
	pointer_default(unique)
]
__interface ITNTServer : IDispatch
{
	[id(1), helpstring("method Start")] HRESULT Start([in] LONG iPort);
	[id(2), helpstring("method Stop")] HRESULT Stop(void);
	[id(3), helpstring("property AcceptLogon")] HRESULT get_RejectLogon([out] BYTE* pVal, [out]BSTR *bsReason);
	[id(4), helpstring("property AcceptLogon")] HRESULT put_RejectLogon([in] BYTE newVal, [in] BSTR bsReason);
	[id(5), helpstring("property AcceptTradeAdd")] HRESULT get_RejectTradeAdd([out] BYTE* pVal, [out]BSTR *bsReason);
	[id(6), helpstring("property AcceptTradeAdd")] HRESULT put_RejectTradeAdd([in] BYTE newVal, [in] BSTR bsReason);
	[id(7), helpstring("method SendTNT")] HRESULT SendTNT([in] TNTServerData *pData);
};


// _ITNTServerEvents
[
	dispinterface,
	uuid("CA7DA9E8-DEA7-4A75-80FF-FF54109AC191"),
	helpstring("_ITNTServerEvents Interface")
]
__interface _ITNTServerEvents
{
	[id(1), helpstring("method Message")] HRESULT LogMessage([in] BSTR bsMessage);
};


// CTNTServer

[
	coclass,
	threading("free"),
	support_error_info("ITNTServer"),
	event_source("com"),
	vi_progid("TNTServer.TNTServer"),
	progid("TNTServer.TNTServer.1"),
	version(1.0),
	uuid("34FF89CE-5F75-468F-B989-7AF83C8D094B"),
	helpstring("TNTServer Class")
]
class ATL_NO_VTABLE CTNTServer : 
	public ITNTServer, 
	public IEgTcpDriverEvents
{

	typedef std::vector<CMMHHMessage*> COMMANDS_TYPE;
public:
	CTNTServer():
		m_bStarted(false),
		m_bRejectTradeAdd(false),
		m_bRejectLogon(false),
		m_chTradeAddRejectionReason('R'),
		m_chLogonRejectionReason('2')
	{
	}

	__event __interface _ITNTServerEvents;

	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		m_hEventStop = CreateEvent(NULL, TRUE, FALSE, NULL);
		m_ValidIncomingCommands.push_back( new CTNTMessage(true));
		m_ValidIncomingCommands.push_back( new CLogOffData);
		m_ValidIncomingCommands.push_back( new CLogOnData);
		return S_OK;
	}
	
	void FinalRelease() 
	{
		for(COMMANDS_TYPE::iterator index = m_ValidIncomingCommands.begin(); index != m_ValidIncomingCommands.end(); index++)
		{
			CMMHHMessage* pitem= *index;
			delete pitem;
		}
		m_ValidIncomingCommands.clear();
	}

public:

private:
	BOOL WINAPI OnConnect(const CONNECTION_ID& Connection, BOOL bIncoming);
	void WINAPI OnConnectionFailed(const CONNECTION_ID& Connection);
	void WINAPI OnClose(const CONNECTION_ID& Connection, const DWORD& dwError);
	void WINAPI OnReceive(const CONNECTION_ID&	Connection,	const LPVOID pData,	const DWORD	dwDataLen);

	bool IsStopped(){return WaitForSingleObject(m_hEventStop,0)!=WAIT_TIMEOUT;}

	static UINT WINAPI _ListenerThread(LPVOID pData);

private:
	SOCKET m_LastSocket;
	bool m_bRejectTradeAdd;
	bool m_bRejectLogon;
	char m_chTradeAddRejectionReason;
	char m_chLogonRejectionReason;

	HANDLE			m_hEventStop;
	bool            m_bStarted; 

	mutable CLock	m_PeersLock;
	mutable CLock   m_csRequestLock;

	TMapPeers		m_mapPeers;

	CEgTcpDriver	m_TcpDriver;

	CDataQueueBuffered<CMessageItem>	m_queIncoming;
	CDataQueueBuffered<CMMHHMessage*>   m_queOutgoing;

	COMMANDS_TYPE m_ValidIncomingCommands;
	void GetPeerAddressString(const CONNECTION_ID& Connection, string& sAddress);
	void OnMessage(CMessageItem& Item);
	
public:
	STDMETHOD(Start)(LONG iPort);
	STDMETHOD(Stop)(void);
	STDMETHOD(get_RejectLogon)(BYTE* pVal, BSTR *bsReason);
	STDMETHOD(put_RejectLogon)(BYTE newVal, BSTR bsrReason);
	STDMETHOD(get_RejectTradeAdd)(BYTE* pVal, BSTR *bsReason);
	STDMETHOD(put_RejectTradeAdd)(BYTE newVal, BSTR bsrReason);
	STDMETHOD(SendTNT)(TNTServerData* pData);
};


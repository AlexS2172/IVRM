// TNTServer.cpp : Implementation of CTNTServer

#include "stdafx.h"
#include "TNTServer.h"
#include "Trace.h"
#include ".\tntserver.h"
#include "WaitWithEventFunction.h"

#define MESSAGE_HEADER_SIZE 28
// CTNTServer


void CTNTServer::GetPeerAddressString(const CONNECTION_ID& Connection, string& sAddress)
{

	const hostent* pAddr = ::gethostbyaddr((const char*)&Connection.m_RemoteAddr.sin_addr.S_un, 
		sizeof(Connection.m_RemoteAddr.sin_addr.S_un), PF_INET );

	if(pAddr)
	{
		sAddress = pAddr->h_name;
	}
	else
	{
		char szBuf[256];
		sprintf(szBuf, "%d.%d.%d.%d", 
			Connection.m_RemoteAddr.sin_addr.S_un.S_un_b.s_b1, 
			Connection.m_RemoteAddr.sin_addr.S_un.S_un_b.s_b2, 
			Connection.m_RemoteAddr.sin_addr.S_un.S_un_b.s_b3, 
			Connection.m_RemoteAddr.sin_addr.S_un.S_un_b.s_b4); 

		sAddress = szBuf;
	}
};

//-----------------------------------[ Driver events ]------------------------------------

BOOL WINAPI CTNTServer::OnConnect(const CONNECTION_ID& Connection, BOOL bIncoming)
{
	CPeerContext Peer;
	GetPeerAddressString(Connection, Peer.m_sAddress);

	CPeerIdentity Client;
	Client.m_Socket = Connection.m_Socket;
	Client.m_Guid = CGuidWrapper();

	CAutoLock  AutoLock(&m_PeersLock);

	m_mapPeers[Client] = Peer;

	if(bIncoming)
		Trace(enTraceInfo, _T("[%s] %s"), Peer.m_sAddress.c_str(), _T("Connected."));

	return TRUE;
};

void WINAPI CTNTServer::OnClose(const CONNECTION_ID& Connection, const DWORD& dwError)
{
	CAutoLock  AutoLock(&m_PeersLock);
	Trace(enTraceInfo, _T("Performing disconnection ..."));

	Trace(enTraceInfo, _T("[%s] %s"), m_mapPeers[Connection.m_Socket].m_sAddress.c_str(), 
		_T("Disconnected."));

	TMapPeers::iterator it = m_mapPeers.find(Connection.m_Socket);
	if(it != m_mapPeers.end())
		m_mapPeers.erase(it);
};

void WINAPI CTNTServer::OnReceive(const CONNECTION_ID&	Connection, 
									   const LPVOID			pData,
									   const DWORD			dwDataLen)
{
	CFixBuffer&	InputStream = m_mapPeers[Connection.m_Socket].m_InputStream;

	InputStream.Put(pData, dwDataLen);

	while(true)
	{
		// is there a room for MsgSize?
		if(InputStream.GetLength() < MESSAGE_HEADER_SIZE)
			return;

		CString strSize;
		char* pSizeString = strSize.GetBufferSetLength(4);
		memcpy(pSizeString,((char*)InputStream.GetData() + sizeof(char)*25),3);
		pSizeString[3]=0;
		strSize.ReleaseBuffer();
		DWORD dwSize = (DWORD)atol(strSize);

		// is there a room for Msg?
		if(InputStream.GetLength() < dwSize)
			return;

		for(COMMANDS_TYPE::iterator index = m_ValidIncomingCommands.begin(); index != m_ValidIncomingCommands.end(); index++)
		{
			CMMHHMessage* pitem= *index;
			if(pitem->Parse(InputStream) == ERROR_SUCCESS)
			{
				CMessageItem	Item;
				if(IsStopped())
					break;
				Item.m_pMessage = pitem->GetNewCopy();
				TMapPeers::iterator it = m_mapPeers.find(Connection.m_Socket);
				if(it == m_mapPeers.end())
				{
					ATLASSERT(false);
					return;
				}

				Item.m_Client = it->first;
				m_queIncoming.Push(Item);
				break;
			}
		}
		if(IsStopped())
			return;
	}
}


void WINAPI CTNTServer::OnConnectionFailed(const CONNECTION_ID& Connection)
{
	Trace(enTraceError, _T("Failed to connect to Midas server."));
};

STDMETHODIMP CTNTServer::Start(LONG iPort)
{
	HRESULT hr = S_OK;
	if(!m_bStarted)
	{
		ResetEvent(m_hEventStop);
		m_TcpDriver.Init();
		if(0!=(long)m_TcpDriver.Start(this, COINIT_MULTITHREADED,(unsigned short) iPort))
			hr = Error(L"TCP/IP Driver Error", __uuidof(ITNTServer), E_FAIL);		
		else
			m_bStarted = true;

		unsigned dwID = 0;
		_beginthreadex(NULL, 0, _ListenerThread, this, 0, &dwID);
	}

	return hr;
}

STDMETHODIMP CTNTServer::Stop(void)
{
	if(m_bStarted)
	{
		SetEvent(m_hEventStop);
		m_TcpDriver.Stop();
		m_TcpDriver.Exit();
		m_bStarted = false;
	}
	return S_OK;
}

UINT CTNTServer::_ListenerThread(LPVOID pData)
{
	ATLASSERT(pData);
	CTNTServer* pThis = reinterpret_cast<CTNTServer*>(pData);
	CoInitializeEx(NULL, COINIT_MULTITHREADED);
	if(pThis)
	{
		HANDLE hEvents[]=
		{
			pThis->m_queIncoming.GetNotificationEvent(),
			pThis->m_hEventStop
		};
		while(1)
		{
			if(WWE::WaitWithEvents(2,hEvents)!=WAIT_OBJECT_0)
				break;

			while(!pThis->m_queIncoming.IsEmpty())
			{
				CMessageItem Item;
				pThis->m_queIncoming.Pop(Item);
				if(!pThis->IsStopped())
                    pThis->OnMessage(Item);
				delete Item.m_pMessage;
			}
		}
	}
	CoUninitialize();
	return 0;

}
void CTNTServer::OnMessage(CMessageItem& Item)
{


	TMapPeers::iterator it = m_mapPeers.find(Item.m_Client);
	if(it!=m_mapPeers.end())
	{
		m_LastSocket = Item.m_Client.m_Socket;
		bool bSend = false;
		CPeerContext& Context = it->second;
		long lSequence = Item.m_pMessage->GetSequenceNumber();
		if(lSequence<Context.m_lIncomingSeqNum)
		{
			// Wrong Sequence Number
			_bstr_t bsMessage ;
			bsMessage = L"Wrong sequence Number";
			__raise _ITNTServerEvents_LogMessage(bsMessage);

			//send Wrong sequence Number
			CAcknowlegementMessage Message;
			Message.SetSequenceNumber(Context.m_lIncomingSeqNum);
			Message.AssignString(Message.GetBody().m_ExpectSequenceNo,Context.m_lIncomingSeqNum,sizeof(Message.GetBody().m_ExpectSequenceNo),true,'0');
			Message.AssignString(Message.GetBody().m_OriginalSequenceNo,lSequence,sizeof(Message.GetBody().m_OriginalSequenceNo),true,'0');
			
			if(m_bRejectLogon)
				*Message.GetBody().m_MmhtErrorCodeEnum1=m_chLogonRejectionReason;
			else
				*Message.GetBody().m_MmhtErrorCodeEnum1='4';

			*Message.GetBody().m_MmhtErrorCodeEnum2=' ';
			*Message.GetBody().m_MmhtErrorCodeEnum3=' ';
			m_TcpDriver.Send(Item.m_Client.m_Socket, Message.GetBuffer(),Message.GetBufferLength());
			bSend =true;
/*
			_bstr_t bsMessage ;
			bsMessage = L"Wrong sequence Number";
			__raise _ITNTServerEvents_Message(bsMessage);
*/
		}
		else
		{
			// Sequence Number Is OK
			Context.m_lIncomingSeqNum++;
			CLogOnData* pLogOnData = dynamic_cast<CLogOnData*>(Item.m_pMessage);
			if(pLogOnData)
			{
				// LogOnRequest;
				Context.m_sLogonID = (LPCTSTR)pLogOnData->GetPassword();
				Context.m_bLoggedOn = true;

				CComBSTR bsMessage = Context.m_sLogonID.c_str();
				bsMessage += L": Is Logged In";
				__raise _ITNTServerEvents_LogMessage(bsMessage);
			}
			CLogOffData *pLogOffData = dynamic_cast<CLogOffData*>(Item.m_pMessage);
			if(pLogOffData)
			{
				if(pLogOffData->GetPassword()!=_bstr_t(Context.m_sLogonID.c_str()))
				{
					//Error
					_bstr_t bsMessage;
					bsMessage = L"Unexpected Password in Logoff Message";
					__raise _ITNTServerEvents_LogMessage(bsMessage);

					CAcknowlegementMessage Message;
					Message.SetSequenceNumber(Context.m_lIncomingSeqNum);
					Message.AssignString(Message.GetBody().m_ExpectSequenceNo,0,sizeof(Message.GetBody().m_ExpectSequenceNo),true,'0');
					Message.AssignString(Message.GetBody().m_OriginalSequenceNo,lSequence,sizeof(Message.GetBody().m_OriginalSequenceNo),true,'0');
					*Message.GetBody().m_MmhtErrorCodeEnum1='6';
					*Message.GetBody().m_MmhtErrorCodeEnum2=' ';
					*Message.GetBody().m_MmhtErrorCodeEnum3=' ';
					m_TcpDriver.Send(Item.m_Client.m_Socket, Message.GetBuffer(),Message.GetBufferLength());
					bSend = true;
				}
				else
				{
					Context.m_bLoggedOn = false;
					_bstr_t bsMessage = pLogOffData->GetPassword();
					bsMessage += L": Is Logged Out";
					__raise _ITNTServerEvents_LogMessage(bsMessage);
				}
			}

			CTNTMessage* pTNTData = dynamic_cast<CTNTMessage*>(Item.m_pMessage);
			if(pTNTData)
			{
				TMapPeers::iterator itSend;
				for(itSend= m_mapPeers.begin(); itSend!=m_mapPeers.end();itSend++)
				{
					if(it!=itSend)
					{
						m_TcpDriver.Send(itSend->first.m_Socket,pTNTData->GetBuffer(),pTNTData->GetBufferLength());
					}
				}
				CString strMessage;
				strMessage.Format(_T("%s: Trade Add Message Seq Number [%d]"),Context.m_sLogonID.c_str(),pTNTData->GetHeader().GetSequenceNumber());
				_bstr_t bsMessage = (LPCTSTR)strMessage;
				__raise _ITNTServerEvents_LogMessage(bsMessage);
			}
			if(!bSend)
			{
				CAcknowlegementMessage Message;
				Message.SetSequenceNumber(Context.m_lIncomingSeqNum);
				Message.AssignString(Message.GetBody().m_ExpectSequenceNo,0,sizeof(Message.GetBody().m_ExpectSequenceNo),true,'0');
				Message.AssignString(Message.GetBody().m_OriginalSequenceNo,lSequence,sizeof(Message.GetBody().m_OriginalSequenceNo),true,'0');

				*Message.GetBody().m_MmhtErrorCodeEnum1=' ';

				if((pLogOnData)&&(m_bRejectLogon) )
				{	
					*Message.GetBody().m_MmhtErrorCodeEnum1=m_chLogonRejectionReason;
				}else
				{
					if((pTNTData)&&(m_bRejectTradeAdd) )
					{	
						*Message.GetBody().m_MmhtErrorCodeEnum1=m_chTradeAddRejectionReason;
					}
				}

				*Message.GetBody().m_MmhtErrorCodeEnum2=' ';
				*Message.GetBody().m_MmhtErrorCodeEnum3=' ';
				m_TcpDriver.Send(Item.m_Client.m_Socket, Message.GetBuffer(),Message.GetBufferLength());
			}
		}
	}
}


STDMETHODIMP CTNTServer::get_RejectLogon(BYTE* pVal, BSTR *bsReason)
{
	*pVal = m_bRejectLogon? 1:0;

	char chOut[2];
	memset(chOut,0,2);
	chOut[0] = m_chLogonRejectionReason;
	_bstr_t strOut(chOut);
	*bsReason = strOut.copy();

	return S_OK;
}

STDMETHODIMP CTNTServer::put_RejectLogon(BYTE newVal, BSTR bsrReason)
{
	m_bRejectLogon	 = (newVal != 0);
	CString str = CW2A(bsrReason);
	str.Trim();
	m_chLogonRejectionReason = str[0];
	return S_OK;
}

STDMETHODIMP CTNTServer::get_RejectTradeAdd(BYTE* pVal, BSTR *bsReason)
{
	*pVal = m_bRejectTradeAdd? 1:0;
	char chOut[2];
	memset(chOut,0,2);
	chOut[0] = m_chTradeAddRejectionReason;
	_bstr_t strOut(chOut);
	*bsReason = strOut.copy();

	return S_OK;
}

STDMETHODIMP CTNTServer::put_RejectTradeAdd(BYTE newVal, BSTR bsrReason)
{
	m_bRejectTradeAdd = (newVal != 0);
	CString str = CW2A(bsrReason);
	str.Trim();
	m_chTradeAddRejectionReason = str[0];
	return S_OK;
}
/*
BSTR			Id;
BSTR			TradeID;
VARIANT_BOOL	isBuy;
DATE			TradeDateTime;
LONG			TradeQuantity;
VARIANT_BOOL	isCall;
BSTR			OptionSymbol;
DATE			OptionExpDate;
DOUBLE			Strike;
DOUBLE			PremiumPrice;
BSTR			ExecutingFirm;
BSTR		ExecutingMember;
BSTR			CMTA;
BSTR			Quantifier;
BSTR			ContraFirm;
BSTR			ContraMember;	
BSTR			SpreadIndicator;
BSTR			OptionalData;
*/


STDMETHODIMP CTNTServer::SendTNT(TNTServerData *pData)
{
	

	TNTData TD;

	
	
	TD.TradeID = pData->TradeID;
	TD.isBuy =			pData->isBuy;
	TD.TradeDateTime =  pData->TradeDateTime;
	TD.TradeQuantity =  pData->TradeQuantity;
	TD.isCall =			pData->isCall;
	TD.OptionSymbol =	pData->OptionSymbol;
	TD.OptionExpDate =  pData->OptionExpDate;
	TD.Strike =			pData->Strike;
	TD.PremiumPrice =	pData->PremiumPrice;
	TD.ExecutingFirm =  pData->ExecutingFirm;
	TD.ExecutingMember = pData->ExecutingMember;
	TD.CMTA =			pData->CMTA;
	TD.Quantifier =		pData->Quantifier;
	TD.ContraFirm =		pData->ContraFirm;
	TD.ContraMember =	pData->ContraMember;	
	TD.SpreadIndicator = pData->SpreadIndicator;
	TD.OptionalData =	pData->OptionalData;
	
//	CString tmp;
//	tmp=pData->Id;
//	tmp.Trim();
	CTNTMessage tntMessage;
	if( tntMessage.SetMessage(&TD/*, CT2W((LPCTSTR)tmp)*/) )
	{
		m_TcpDriver.Send(m_LastSocket, tntMessage.GetBuffer(),tntMessage.GetBufferLength());		
	}else
	{
		__raise _ITNTServerEvents_LogMessage(L"failed to construct TNT data:Invalid parameters.");
	}

	return S_OK;
}


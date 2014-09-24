#include "stdafx.h"

#include "SgConst.h"
#include "FixConnector.h"
#include "Trace.h"

extern _bstr_t	g_bsTargetCompID;
extern _bstr_t	g_bsSenderCompID;

DWORD CFixConnector::ReadSettings()
{
	USES_CONVERSION;

    try {GetSetting(SETTINGS_IN_SEQNUM_KEY, m_nIncomingSeqNo);}
    catch(_com_error&) {}
    try {GetSetting(SETTINGS_OUT_SEQNUM_KEY, m_nOutgoingSeqNo);}
    catch(_com_error&) {}

    try
    {
        GetSetting(SETTINGS_SERVER_ADDR_KEY, m_bsServerAddr);
        GetSetting(SETTINGS_SERVER_PORT_KEY, m_nServerPort);
        GetSetting(SETTINGS_TARGET_COMP_ID_KEY, g_bsTargetCompID);
        GetSetting(SETTINGS_SENDER_COMP_ID_KEY, g_bsSenderCompID);
        GetSetting(SETTINGS_RECONNECT_TIME_KEY, m_nReconnectTime);
    }
    catch (_com_error& e)
    {
		return CTracer::TraceError(e.Error(), W2T(e.Description()) );
    }

    return 0;
}

DWORD CFixConnector::SaveSettings()
{
	USES_CONVERSION;

    try
    {
        SetSetting(SETTINGS_IN_SEQNUM_KEY, m_nIncomingSeqNo);
        SetSetting(SETTINGS_OUT_SEQNUM_KEY, m_nOutgoingSeqNo);
    }
    catch (_com_error& e)
    {
		return CTracer::TraceError(e.Error(), W2T(e.Description()) );
    }

    return 0;
}


DWORD __stdcall CFixConnector::Start(void* pParam)
{
	DWORD dwErr;
	if(dwErr = ReadSettings() != 0)
		return dwErr;

	m_hReconnectTimer = CreateWaitableTimer(NULL, FALSE, NULL);
	if (m_hReconnectTimer == NULL)
		return CTracer::TraceError(GetLastError(), _T("Failed to create waitable timer."));

	m_hForcedShutdownTimer = CreateWaitableTimer(NULL, FALSE, NULL);
	if (m_hForcedShutdownTimer == NULL)
		return CTracer::TraceError(GetLastError(), _T("Failed to create waitable timer."));

	m_hHeartBeatTimer = CreateWaitableTimer(NULL, FALSE, NULL);
	if (m_hHeartBeatTimer == NULL)
		return CTracer::TraceError(GetLastError(), _T("Failed to create waitable timer."));

	m_hLogoutEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	if (m_hLogoutEvent == NULL)
		return CTracer::TraceError(GetLastError(), _T("Failed to create logout event."));

	dwErr = CThreadWrapper::Start(this);
	if (dwErr)
		return CTracer::TraceError(dwErr, _T("ThreadWrapper error."));

	dwErr = m_Driver.Init();
	if (dwErr)
		return CTracer::TraceError(dwErr, _T("TcpDriver initialization error."));

	dwErr = m_Driver.Start(this, COINIT_MULTITHREADED);
	if (dwErr)
		return CTracer::TraceError(dwErr, _T("TcpDriver starting error."));

	// Here is place for tests...
/*
	{
		fstream fLog("_log.txt", ios::in);

		while(!fLog.eof())
		{
			char szBuff[512];
			fLog.getline(szBuff, 512);
			if(*szBuff == 0)
				break;

			CFixBuffer Buf;
			Buf.Put(szBuff + strlen("[0x05bc]20:29:21 "), strlen(szBuff) - strlen("[0x05bc]20:29:21 "));
			CFixMessage Msg;
			DWORD eee = Msg.Parse(Buf);

			if(eee == 0)
				OnApplicationMessage(Msg);
			else
				__asm int 3;
		}
	}

	Sleep(1000000);
*/
	if (m_Driver.Connect(m_bsServerAddr, m_nServerPort))
	{
		CTracer::Trace(_T("Connection to Sage server could not be established. Reconnection attempt will be in %d seconds."),
			m_nReconnectTime);

		QueueReconnectAttempt(m_nReconnectTime);
	}

	return dwErr;
}

DWORD __stdcall CFixConnector::Stop()
{
	DWORD dwResult = 0;
	DWORD dwErr;

	dwErr = CThreadWrapper::Stop();
	if (dwErr)
		dwResult = CTracer::TraceError(dwErr, _T("ThreadWrapper error."));

	if (m_hReconnectTimer)
		::CloseHandle(m_hReconnectTimer);
	
	if (m_hHeartBeatTimer)
		::CloseHandle(m_hHeartBeatTimer);

	if (m_hForcedShutdownTimer)
		::CloseHandle(m_hForcedShutdownTimer);

	if (m_hLogoutEvent)
		::CloseHandle(m_hLogoutEvent);

	dwErr = SaveSettings();
	if (dwErr)
		dwResult = CTracer::TraceError(dwErr, _T("Failed to save settings."));

	dwErr = m_Driver.Stop();
	if (dwErr)
		dwResult = CTracer::TraceError(dwErr, _T("TcpDriver stopping error."));

	dwErr = m_Driver.Exit();
	if (dwErr)
		dwResult = CTracer::TraceError(dwErr, _T("TcpDriver uninitialization error."));

	return dwResult;
}


UINT  __stdcall CFixConnector::Thread(void* pParam)
{
	CFixConnector* pThis = (CFixConnector*)pParam;
	CoInitializeEx(NULL, COINIT_MULTITHREADED);

	const HANDLE hEvents[] = {GetStopEventHandle(), 
							  pThis->m_hReconnectTimer,
							  pThis->m_hHeartBeatTimer,
							  pThis->m_hLogoutEvent,
							  pThis->m_hForcedShutdownTimer};

	bool bRunning = true;

	while(bRunning )
	{
		DWORD dwRes = WaitForMultipleObjects(sizeof(hEvents) / sizeof(HANDLE), hEvents, FALSE, INFINITE);

		switch(dwRes)
		{
			case WAIT_OBJECT_0: // Stopping by user request
				ResetEvent(GetStopEventHandle());
				
				// trying to send logout message. If success, then setting timeout for 
				// receiving that message. If Fix server does not reply for a long time
				// then shutdown by timeout.
				m_Lock.Lock();
				bRunning = Logout();
				if(bRunning)
				{
					bRunning = QueueForcedShutdown();
				}
				m_Lock.Unlock();
				break;

			case WAIT_OBJECT_0 + 1: // Disconnection, attempt to connect again
				CTracer::Trace(_T("Time to reconnect."));
				if (pThis->m_Driver.Connect(m_bsServerAddr, m_nServerPort) != 0)
				{
					CTracer::Trace(_T("Connection to Sage server could not be established. Reconnection attempt will be in %d seconds."),
						m_nReconnectTime);

					QueueReconnectAttempt(m_nReconnectTime);
				}
				break;

			case WAIT_OBJECT_0 + 2: // time to send heartbeat
				m_Lock.Lock();
				if (m_bConnected)
				{
					CTracer::Trace(_T("Heartbeat received, time to send a reply."));
					CFixMessage Msg;
					Msg.MakeHeartBeat(m_nOutgoingSeqNo++);
					Send(Msg);
				}
				m_Lock.Unlock();
				break;

			case WAIT_OBJECT_0 + 3: // logged out
				bRunning = false;
				break;

			case WAIT_OBJECT_0 + 4: // forced shutdown (by timeout)
				CTracer::Trace(_T("Logout message has not been received for a long time. Performing forced shutdown."));
				DropConnection();
				bRunning = false;
				break;

			default:
				CTracer::TraceError(GetLastError(),
						_T("CFixConnector::WaitForMultipleObjects error"));
				bRunning = false;
				break;
		}
	}

	CoUninitialize();
	return 0;
}

/*------------------------------------------------------------------------------------
	TCP Driver Events
------------------------------------------------------------------------------------*/

BOOL WINAPI CFixConnector::OnConnect(const CONNECTION_ID & Connection, BOOL bIncoming)
{
	m_bConnected = true;

	CTracer::Trace(_T("Connection to Sage server is established."));
	CTracer::Trace(_T("Logging on..."));

	m_Socket = Connection.m_Socket;

	Logon();

	return TRUE;
}

void CFixConnector::OnDropConnection()
{
	m_bConnected = false;

	m_Gap.Clear();

	if (!m_bLoggingOut)
	{
		QueueReconnectAttempt(m_nReconnectTime);
	}
	else
	{
		SetEvent(m_hLogoutEvent);
		CTracer::Trace(_T("Connection dropped."));
	}
}

void WINAPI CFixConnector::OnClose(const CONNECTION_ID & Connection, const DWORD & dwError)
{
	if (!m_bLoggingOut)
	{
		CTracer::Trace(_T("Connection to Sage server is dropped. Reconnection attempt will be in %d seconds."),
			m_nReconnectTime); 
	}

	OnDropConnection();
}

void WINAPI CFixConnector::OnConnectionFailed(const CONNECTION_ID& Connection)
{
	if (!m_bLoggingOut)
	{
		CTracer::Trace(_T("Connection to Sage server could not be established. Reconnection attempt will be in %d seconds."),
			m_nReconnectTime);
	}

	OnDropConnection();
}

void WINAPI CFixConnector::OnReceive(const CONNECTION_ID & Connection, const LPVOID const pData, const DWORD dwDataLen)
{
	// check data from previous session
	if (Connection.m_Socket != m_Socket)
		return;

	CAutoLock Lock(&m_Lock);

	//CTracer::Trace(_T("New data arrived from Sage server : Length = %d"), dwDataLen);

	CFixMessage	Msg;
	bool		bCompleteMessage = true;

	m_IncomingDataQueue.Put(pData, dwDataLen);

	while(bCompleteMessage)
	{
		DWORD dwParseResult = Msg.Parse(m_IncomingDataQueue);

		switch(dwParseResult)
		{
			case ERROR_SUCCESS:
				bCompleteMessage = true;
				break;

			case ERROR_INVALID_DATA:
				CTracer::Trace(_T("Terminating the connection..."));
				bCompleteMessage = false;
				DropConnection();
				break;

			default:	// ERROR_INSUFFICIENT_BUFFER
				bCompleteMessage = false;
				break;
		}
		
		if (bCompleteMessage)
		{
			m_IncomingDataQueue.ShiftLeft(Msg.GetLength());

			bool bOK = OnMessage(Msg);

			if (!bOK)
			{
				bCompleteMessage = false;
				DropConnection();
			}
		}
	}
}

/************************************************************************************
	Internal methods
************************************************************************************/
void CFixConnector::Logon()
{
	CAutoLock Lock(&m_Lock);

	m_IncomingDataQueue.Clear();

	CFixMessage Msg;

	// check whether we are connecting for the first time
	// if so than we are reset seg no in logon message
	// another case - when we trying to logon several times, but remote server
	// is dropping connection.
	bool bNeedResetSeqNo = false;

	if(++m_dwLogonTries == MAX_LOGON_SAME_SESSION_TRIES)
	{
		CTracer::Trace(_T("WARNING. Failed to continue session after reconnection because remote server is dropping it. Trying to reset sequence number and logon again."));
		m_nOutgoingSeqNo = 1;
		m_dwLogonTries = 0;
	}


	// seqnum = 1 means that we are reseting session or started first
	if(m_nOutgoingSeqNo == 1)
	{
		bNeedResetSeqNo = true;
	}

	Msg.MakeLogon(m_nOutgoingSeqNo++, bNeedResetSeqNo);

	Send(Msg);
}

bool CFixConnector::Logout()
{
	m_bLoggingOut = true;

	CFixMessage Msg;

	Msg.MakeLogout(m_nOutgoingSeqNo++);

	DWORD dwRes = Send(Msg);
	
	if (dwRes == 0)
	{
		CTracer::Trace(_T("Logging out..."));
	}

	return (dwRes == 0);
}

void CFixConnector::DropConnection()
{
	m_Gap.Clear();

	CTracer::Trace(_T("Dropping connection..."));
	m_Driver.Close(m_Socket);
	m_Socket = NULL;
}

void CFixConnector::QueueReconnectAttempt(long lTimer)
{
	LARGE_INTEGER liDueTime;
	liDueTime.QuadPart = ((ULONGLONG)lTimer) * 1000 * 10000; // 100 ns discontinuity
	liDueTime.QuadPart *= -1;

	if (!SetWaitableTimer(m_hReconnectTimer, &liDueTime, 0, NULL, NULL, 0))
	{
		CTracer::TraceError(GetLastError(), 
			_T("Failed to queue reconnection request : waitable timer error"));
	}
}

void CFixConnector::QueueHeartBeat(long lInterval)
{
	LARGE_INTEGER liDueTime;
	liDueTime.QuadPart = ((ULONGLONG)lInterval) * 1000 * 10000; // 100 ns discontinuity
	liDueTime.QuadPart *= -1;

	if (!SetWaitableTimer(m_hHeartBeatTimer, &liDueTime, 0, NULL, NULL, 0))
	{
		CTracer::TraceError(GetLastError(), 
			_T("Failed to queue heart beat : waitable timer error"));
	}
}

bool CFixConnector::QueueForcedShutdown(long lInterval)
{
	LARGE_INTEGER liDueTime;
	liDueTime.QuadPart = ((ULONGLONG)lInterval) * 1000 * 10000; // 100 ns discontinuity
	liDueTime.QuadPart *= -1;

	if (!SetWaitableTimer(m_hForcedShutdownTimer, &liDueTime, 0, NULL, NULL, 0))
	{
		CTracer::TraceError(GetLastError(), 
			_T("Failed to queue forced shutdown : waitable timer error"));

		return false;
	}

	return true;
}

bool CFixConnector::CheckSeqNum(const CFixMessage& Msg)
{
	// if it is seq-reset message (not gap-fill) we should ingore seqnum (spec)
	CFixTagValue Value;

	if(Msg.GetType() == enFmtSequenceReset)
	{
		char	cGapFill = 'N';

		if (Msg.GetTagValue(enFtGapFillFlag, Value) == TRUE)
			Value.GetChar(cGapFill);

		if(cGapFill == 'N')
			return true;
	}

	// if duplicate, then check gap
	if(Msg.IsPossDup() || Msg.IsPossResend())
	{
		m_Gap.FillBySeqNum(Msg.GetSeqNum());

		if(m_Gap.IsFilled())
		{
			CTracer::Trace(_T("The whole gap filled."));
		}

		m_nIncomingSeqNo = Msg.GetSeqNum();

		return true;
	}
			
	int nExpectedIncomingSeqNo = m_nIncomingSeqNo + 1;
	m_nIncomingSeqNo = Msg.GetSeqNum();

	// if session was not reset
	if(m_nIncomingSeqNo != 1)
	{
		if(m_nIncomingSeqNo < nExpectedIncomingSeqNo)
		{
			CTracer::Trace(_T("WARNING. Incoming sequence number %d is less then expected %d."),
				m_nIncomingSeqNo, nExpectedIncomingSeqNo);

			return true;
		}

		if(nExpectedIncomingSeqNo != m_nIncomingSeqNo)
		{
			CTracer::Trace(_T("Gap detected from %d to %d. Trying to fill..."), 
					nExpectedIncomingSeqNo, m_nIncomingSeqNo - 1);

			for(int iSeqNo = nExpectedIncomingSeqNo; 
				iSeqNo <= m_nIncomingSeqNo - 1; iSeqNo++)
			{
				m_Gap.AddSeqNumToBeFilled(iSeqNo);
			}

			CFixMessage Msg2;
			Msg2.MakeResendRequest(m_nOutgoingSeqNo++, 
				nExpectedIncomingSeqNo, m_nIncomingSeqNo - 1);
			Send(Msg2);
		}
	}

	return true;
}

bool CFixConnector::OnMessage(CFixMessage& Msg)
{
	CTracer::Trace(_T("OnMessage : SeqNum = %d, Type = %c."), Msg.GetSeqNum(), Msg.GetType());

	if(CheckSeqNum(Msg) == false)
		return false;

	// if we've received message with DupFlag == NO, we should process it after filling gap
	if(m_Gap.IsFilled() == false)
	{
		if(!Msg.IsPossDup() && !Msg.IsPossResend())
		{
			CTracer::Trace(_T("Message has come out-of-order. Pushed to queue so as to be processed later."));
			m_Gap.Push(Msg);
			return true;
		}
	}

	bool bRes;

	switch(Msg.GetType())
	{
		case enFmtLogon:
			bRes = OnLogonMessage(Msg);
			break;

		case enFmtLogout:
			bRes = OnLogoutMessage(Msg);
			break;

		case enFmtHeartBeat:
			bRes = OnHeartBeatMessage(Msg);
			break;

		case enFmtTestRequest:
			bRes = OnTestRequestMessage(Msg);
			break;

		case enFmtResendRequest:
			bRes = OnResendRequestMessage(Msg);
			break;

		case enFmtSequenceReset:
			bRes = OnSequenceResetMessage(Msg);
			break;

		case enFmtReject:
			bRes = OnRejectMessage(Msg);
			break;

		default:
			bRes = OnApplicationMessage(Msg);
			break;
	}

	// if ok
	if(bRes)
	{
		if(m_Gap.IsFilled())
		{
			CFixMessage Msg;

			if(m_Gap.Pop(Msg))
			{
				CTracer::Trace(_T("Process message from queue. SeqNum = %d"), Msg.GetSeqNum());
				bRes = OnMessage(Msg);
			}
		}
	}

	return bRes;
}

bool CFixConnector::OnLogoutMessage(CFixMessage& Msg)
{
	CTracer::Trace(_T("FIX Logout message."));
	DropConnection();
	return TRUE;
}

bool CFixConnector::OnSequenceResetMessage(CFixMessage& Msg)
{
	int		nNewSeqNum = -1;
	char	cGapFill = 'N';
 
	CFixTagValue Value;
	if (Msg.GetTagValue(enFtNewSeqNo, Value) == TRUE)
		Value.GetInt(nNewSeqNum);

	if (Msg.GetTagValue(enFtGapFillFlag, Value) == TRUE)
		Value.GetChar(cGapFill);

	CTracer::Trace(_T("FIX SequenceReset message : NewSeqNum = %d, GapFillFlag = %c."),
		nNewSeqNum, (int)cGapFill);

	if (nNewSeqNum == -1)
	{
		CTracer::Trace(_T("WARNING. Fix server sent message of type SeqReset with empty NewSeqNum field."));
		return TRUE;
	}

	CTracer::Trace(_T("Incoming SeqNum is set to new value : %d"), nNewSeqNum - 1);
	m_nIncomingSeqNo = nNewSeqNum - 1;

	// delete all messages in gap prio to this
	m_Gap.FillByReset(nNewSeqNum - 1);

	if(m_Gap.IsFilled())
	{
		CTracer::Trace(_T("The whole gap filled."));
	}

	return TRUE;
};

bool CFixConnector::OnRejectMessage(CFixMessage& Msg)
{
	int nSeqNum;
	string sText;

	CFixTagValue Value;
	if (Msg.GetTagValue(enFtRefSeqNum, Value) == TRUE)
		Value.GetInt(nSeqNum);

	if (Msg.GetTagValue(enFtText, Value) == TRUE)
		Value.GetString(sText);

	CTracer::Trace(_T("FIX Reject message : RefSeqNum = %d Text = '%s'."),
		nSeqNum, sText.c_str());

	return TRUE;
};

bool CFixConnector::OnLogonMessage(CFixMessage& Msg)
{
	CFixTagValue Value;

	// successfully logged on. Reset attempts counter.
	m_dwLogonTries = 0;

	CTracer::Trace(_T("FIX Logon message."));

	QueueHeartBeat();

	return true;
}

bool CFixConnector::OnResendRequestMessage(CFixMessage& Msg)
{
	CFixTagValue Value;
	int nBeginSeqNum = -1;
	int nEndSeqNum = -1;

	if (Msg.GetTagValue(enFtBeginSeqNo, Value) == TRUE)
		Value.GetInt(nBeginSeqNum);

	if (Msg.GetTagValue(enFtEndSeqNo, Value) == TRUE)
		Value.GetInt(nEndSeqNum);


	CTracer::Trace(_T("FIX ResendRequest message : SeqNum From %d To %d."), 
		nBeginSeqNum, nEndSeqNum);

	CFixMessage Msg2;
	m_nOutgoingSeqNo++;

	CTracer::Trace(_T("Setting new SeqNum to %d"), m_nOutgoingSeqNo);
	Msg2.MakeSequenceReset(m_nOutgoingSeqNo, m_nOutgoingSeqNo);
	Send(Msg2);

	return TRUE;
}

bool CFixConnector::OnTestRequestMessage(CFixMessage& Msg)
{
	CFixTagValue Value;
	string sText;
	if (Msg.GetTagValue(enFtTestReqID, Value) == TRUE)
	{
		Value.GetString(sText);
	}

	CTracer::Trace(_T("FIX TestRequest message : Text='%s'."), sText.c_str());

	CFixMessage Msg2;
	Msg2.MakeHeartBeat(m_nOutgoingSeqNo++, sText.c_str());
	Send(Msg2);

	return TRUE;
}

bool CFixConnector::OnHeartBeatMessage(CFixMessage& Msg)
{
	string sTime;

	CFixTagValue Value;
	if (Msg.GetTagValue(enFtSendingTime, Value) == TRUE)
	{
		Value.GetString(sTime);
	}

	CTracer::Trace(_T("FIX HeartBeat message : Time = '%s'."), sTime.c_str());

	return TRUE;
}

DWORD CFixConnector::Send(CFixMessage& Msg)
{
	if (m_Socket)
	{
		CTracer::Trace(_T("Sending message : SeqNum = %d Type = %c."), 
			Msg.GetSeqNum(), (int)Msg.GetType());

		// Save outgoing seqnum
		SaveSettings();

		// SPEC: ...The heartbeat interval timer should be reset after every 
		// message is transmitted (not just heartbeats)
		// for logon we sending heartbeat after answer
		if((Msg.GetType() != enFmtLogon) && (Msg.GetType() != enFmtLogout))
			QueueHeartBeat();

		return m_Driver.Send(m_Socket, Msg.GetData(), Msg.GetLength());
	}

	return WSAENOTSOCK;
}
#include "stdafx.h"

#include "RttfClient.h"
#include "crs.h"
#include "trace.h"

#define ENABLE_QUICK_EDIT				0x0040L
#define ENABLE_INSERT_MODE				0x0020L

CRttfClient::CRttfClient() :
	m_Socket(INVALID_SOCKET), m_hShutdownEvent(NULL),
	m_bRttfAvailable(FALSE), m_bFocAvailable(FALSE), m_bTradesAreInOrder(FALSE)

{
#ifdef USE_SMART_LASTTRADETIME_ALGORITHM
	m_SmartLastTime = 0;
#endif

}

CRttfClient::~CRttfClient()
{
	Clear();
}

void  CRttfClient::Clear()
{
	deque<FocCsMsg*>::iterator it;

	{
		CAutoLock Lock(&m_InQueLock);
		while(!m_InQue.empty())
		{
			delete m_InQue.front();
			m_InQue.pop();
		}
	}
	
	{
		CAutoLock Lock(&m_OutQueLock);
		while(!m_OutQue.empty())
		{
			delete m_OutQue.front();
			m_OutQue.pop();
		}
	}

	{
		CAutoLock Lock(&m_TradesMapLock);
		m_mmapTrades.clear();
	}
}

DWORD	 CRttfClient::GetKeepAliveSettings(tcp_keepalive & KeepAlive)
{
	CEgRegKey key;

	KeepAlive.onoff = true;
	KeepAlive.keepaliveinterval = DEFAULT_KEEP_ALIVE_INTERVAL;
	KeepAlive.keepalivetime		= DEFAULT_KEEP_ALIVE_TIME;

	LONG lResult = key.Open(HKEY_CURRENT_USER, SETTINGS_REGISTRY_KEY, KEY_READ);
	if (lResult != ERROR_SUCCESS )
		lResult = key.Open (HKEY_LOCAL_MACHINE, SETTINGS_REGISTRY_KEY, KEY_READ);

	if (lResult == ERROR_SUCCESS)
	{
		key.QueryValue (KeepAlive.onoff, SETTINGS_KEEP_ALIVE_ON);
		key.QueryValue (KeepAlive.keepalivetime, SETTINGS_KEEP_ALIVE_TIME);
		key.QueryValue (KeepAlive.keepaliveinterval, SETTINGS_KEEP_ALIVE_INTERVAL);
		key.Close();
	}

	return 0;
}



DWORD  CRttfClient::Start(const char* szHost, unsigned short usPort, 
						const char* szUserId, const char* szPassword, const char* szSecurityId)
{
	if( !m_LastTrade.Init() )
		return ERROR_REMOTE_STORAGE_NOT_ACTIVE;

	m_strUserId = szUserId ;
	m_strPassword = szPassword;
	m_strSecurityId = szSecurityId;
	m_strHost = szHost;
	m_usPort = usPort;

	WSADATA wsaData = {0};
	WORD wVersionRequested = MAKEWORD(2, 0);
	::WSAStartup(wVersionRequested, &wsaData);

	DWORD dwFlags;
	if( GetConsoleMode(GetStdHandle(STD_INPUT_HANDLE), &dwFlags))
	{
		dwFlags &= ~(ENABLE_QUICK_EDIT | ENABLE_INSERT_MODE);
		SetConsoleMode(GetStdHandle(STD_INPUT_HANDLE), dwFlags);
	}

	if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 0)
	{
		return TraceError(ERROR_OLD_WIN_VERSION, "Error");
	}

	DWORD dwErr = Connect(szHost, usPort);
	if(dwErr)
		return dwErr;

	dwErr = Login(szUserId, szPassword, szSecurityId);
	if(dwErr)
	{
		Disconnect();
		return dwErr;
	}

	m_hShutdownEvent = ::CreateEvent(NULL, TRUE, FALSE, NULL);
	if(!m_hShutdownEvent)
	{
		Disconnect();
		return GetLastError();
	}

	try
	{
		UINT ID = 0;
		
		m_hThreads[0] = (HANDLE)_beginthreadex(NULL, 0, ReadThread, (void *) this, 0, &ID);
		if (!m_hThreads[0])
			throw 0;

		m_hThreads[1] = (HANDLE)_beginthreadex(NULL, 0, WriteThread, (void *) this, 0, &ID);
		if (!m_hThreads[0])
			throw 1;
		
		m_hThreads[2] = (HANDLE)_beginthreadex(NULL, 0, InputThread, (void *) this, 0, &ID);
		if (!m_hThreads[0])
			throw 2;

		m_hThreads[3] = (HANDLE)_beginthreadex(NULL, 0, OutputThread, (void *) this, 0, &ID);
		if (!m_hThreads[0])
			throw 3;
	}
	catch(const int nThread)
	{
		TCHAR lpszErrMsg[100] = {0};		
		switch(nThread)
		{
			case 0:
				_tcscpy(lpszErrMsg, "Failed to start ReadThread" );
				break;
			case 1:
				_tcscpy(lpszErrMsg, "Failed to start WriteThread" );
				break;
			case 2:
				_tcscpy(lpszErrMsg, "Failed to start InputThread" );
				break;
			case 3:
				_tcscpy(lpszErrMsg, "Failed to start OutputThread" );
				break;
		}

		Trace( _T("%s : %s"), lpszErrMsg, A2T(strerror(errno)) );
		
		Disconnect();

		return ERROR_INVALID_HANDLE;
	}

	xdrrec_create(&m_xdrRead, 4096, 4096, (caddr_t) this, XdrReadFunc, XdrWriteFunc);
	xdrrec_create(&m_xdrWrite, 4096, 4096, (caddr_t) this, XdrReadFunc, XdrWriteFunc);

	m_bFocAvailable = true;

	return 0;
}

DWORD  CRttfClient::Stop()
{
	if (m_hShutdownEvent == NULL)
		return ERROR_SERVICE_NOT_ACTIVE;

	SetEvent(m_hShutdownEvent);

	WaitForMultipleObjects(4, m_hThreads, TRUE, INFINITE);

	CloseHandle(m_hShutdownEvent);
	CloseHandle(m_hThreads[0]);
	CloseHandle(m_hThreads[1]);
	CloseHandle(m_hThreads[2]);
	CloseHandle(m_hThreads[3]);

	Logout();

	xdr_destroy(&m_xdrRead);
	xdr_destroy(&m_xdrWrite);

	shutdown(m_Socket, SD_BOTH);
	closesocket(m_Socket);
	m_Socket = INVALID_SOCKET;
	
	WSACleanup();

	Clear();

	return 0;
}

DWORD CRttfClient::Connect(const char* host, unsigned short port)
{
	try
	{
		if ( (m_Socket = ::socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
      		throw (DWORD)WSAGetLastError();
		
		tcp_keepalive KeepAlive;
		DWORD dwErr = GetKeepAliveSettings(KeepAlive);
		if(!dwErr)
		{
			DWORD nBytes;
			int wsaErr = ::WSAIoctl( m_Socket, 
									 SIO_KEEPALIVE_VALS, 
									 &KeepAlive, 
									 sizeof(KeepAlive),
									 NULL, 
									 0, 
									 &nBytes, 
									 NULL, 
									 NULL);
		}
   		
		sockaddr_in serveraddress = {0};
		hostent* entry = 0;

   		serveraddress.sin_family = AF_INET;
   		serveraddress.sin_port = htons(port);
   		serveraddress.sin_addr.s_addr = inet_addr(host);
		
		if (serveraddress.sin_addr.S_un.S_addr == -1)
   		{
      		if ((entry = gethostbyname(host)) == NULL)
	         	throw (DWORD)WSAGetLastError();
      		memcpy(&serveraddress.sin_addr, entry->h_addr, sizeof(struct in_addr));
   		}

   		if (::connect(m_Socket, (struct sockaddr *) &serveraddress, sizeof(serveraddress)) < 0)
      		throw (DWORD)WSAGetLastError();
	}
	catch (const DWORD dwErr)
	{
		Disconnect();
		return TraceError(dwErr, _T("Failed to connect to First Options server.") );
	}

	return 0;
}

void  CRttfClient::Disconnect()
{
	if(m_Socket != INVALID_SOCKET)
	{
		::closesocket(m_Socket);
		m_Socket = INVALID_SOCKET;
	}
}

DWORD  CRttfClient::Reconnect()
{
	if(m_bFocAvailable)
		return 0;

	Disconnect();

	DWORD dwErr = Connect(m_strHost.c_str(), m_usPort);
	if(dwErr)
		return dwErr;

	dwErr = Login(m_strUserId.c_str(), m_strPassword.c_str(), m_strSecurityId.c_str());
	if(dwErr)
	{
		Disconnect();
		return dwErr;
	}

	m_bFocAvailable = true;

	return 0;
}

DWORD  CRttfClient::Login(const char* user_id, const char* password, const char* security_id)
{
	// Create and initialize login message
	FocCsLoginMsg* pMsg = new FocCsLoginMsg(user_id);

	strncpy((char*)pMsg->user_id, user_id, FOC_USER_ID_LEN - 1);
	strncpy((char*)pMsg->password, password, FOC_PASSWORD_LEN - 1);
	strncpy((char*)pMsg->security_id, security_id, FOC_SECURITY_ID_LEN - 1);

	m_OutQue.push(pMsg);

	return 0;
}

DWORD  CRttfClient::Logout()
{
	// Create and initialize login message
	FocCsLogoutMsg* pMsg = new FocCsLogoutMsg(m_strUserId.c_str());

	if (!pMsg->xdr_convert(&m_xdrWrite, XDR_ENCODE))
		Trace(_T("Failed to encode logout message!"));

	if (!xdrrec_endofrecord(&m_xdrWrite, TRUE))
		Trace("Failed to send logout message!"); 

	Trace("Logged out."); 

	delete pMsg;

	return 0;
}


void CRttfClient::Replay(const int iRequestType, const FOC_DATETIME* pTime)
{
	if(iRequestType == RTTF_OPENING_POS_AND_TRADES)
	{
		DWORD dwResult;
		dwResult = m_PostionFile.DeleteFile();
		if (ERROR_SUCCESS != dwResult)
		{
			TraceError(dwResult, "Failed to delete position file.");
		}
	}

	// Create and initialize trade request message
	FocRttfTradeRequestMsg* pMsg = new FocRttfTradeRequestMsg(m_strUserId.c_str());
	pMsg->request_type = iRequestType;
	if (pTime)
		pMsg->last_upd_date = *pTime;
	else
		m_LastTrade.GetDate(pMsg->last_upd_date);

	time_t temp = ((FocRttfTradeRequestMsg*) pMsg)->last_upd_date;
	
	Trace(_T("Start updating '%s' data from %s"), 
		iRequestType == RTTF_OPENING_POS_AND_TRADES ? _T("Trades and positions") : _T("Trades only"),
		ctime(&temp));

	m_OutQue.push(pMsg);
}


int CRttfClient::XdrReadFunc(char* read_handle, char* buf, int len)
{
	CRttfClient* pthis = (CRttfClient *) read_handle;

	int iErr;

	iErr = recv(pthis->m_Socket, buf, len, 0);

	if(iErr == SOCKET_ERROR)
	{
		pthis->m_bFocAvailable = false;
		pthis->m_bTradesAreInOrder = false;
	}

	return iErr;
}

int CRttfClient::XdrWriteFunc(char* write_handle, char* buf, int len)
{
	CRttfClient* pthis = (CRttfClient *) write_handle;

	int iErr;

	iErr = send(pthis->m_Socket, buf, len, 0);

	if(iErr == SOCKET_ERROR)
	{
		pthis->m_bFocAvailable = false;
		pthis->m_bTradesAreInOrder = false;
	}

	return iErr;
}

BOOL  CRttfClient::IsStopping()
{
	return (WAIT_OBJECT_0 == ::WaitForSingleObject(m_hShutdownEvent, 10));
}
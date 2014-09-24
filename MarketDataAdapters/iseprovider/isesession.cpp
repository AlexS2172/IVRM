#include "stdafx.h"

#include "ISESession.h"
#include "OmniApiUtils.h"

/*--------------------------------[ SessionBase ]--------------------------------------------*/

CISESessionBase::CISESessionBase()
{
	m_hEnableLogonEvent = CreateEvent(NULL, TRUE, TRUE, NULL);
}

CISESessionBase::~CISESessionBase()
{
	CloseHandle(m_hEnableLogonEvent);
}

void CISESessionBase::EnableLogon(bool bEnable)
{
	if(bEnable) 
		ResetEvent(m_hEnableLogonEvent);
	else 
		SetEvent(m_hEnableLogonEvent);
}

bool CISESessionBase::IsLogonEnabled() const
{
	DWORD dwRes = WaitForSingleObject(m_hEnableLogonEvent, 0);

	return dwRes == WAIT_TIMEOUT;
}

bool CISESessionBase::LogonSleep(const DWORD dwMillisecs) const
{
	DWORD dwRes = WaitForSingleObject(m_hEnableLogonEvent, dwMillisecs);

	return dwRes == WAIT_TIMEOUT;
}


/*--------------------------------[ Transaction & Query ]--------------------------------------------*/

void CISESession::SendTx(const void* pMsgData,
						 const uint32 uiFacility,
						 uint64_t& uiTransactionID,
						 uint64_t& uiOrderID)
{
	omni_message* pMessages[2]; /* only use the first item */

	pMessages[0] = (omni_message*)pMsgData;
	pMessages[1] = NULL;

	uiTransactionID = 0;
	uiOrderID = 0;

	int32  iTranStatus = 0;

	if(OMNI_FACTYP_LOGOUT != uiFacility && IsLogonEnabled() == false)
		throw CISEException(0, 0, this, "Operation cancelled.");

	CAutoLock	OmniApiLock(&m_OmniApiLock);

	int32  iStatus = omniapi_tx_ex(m_hSession, &iTranStatus, uiFacility,
								   pMessages, (unsigned *)&uiTransactionID, (unsigned *)&uiOrderID);


	if(CSTATUS_FAILED(iStatus))
	{
		throw CISEException(iStatus, iTranStatus, this, "Transaction error");
	}
};

void CISESession::SendTx(const CISEMessage& Msg,
 						 const uint32 uiFacility,
						 uint64_t& uiTransactionID,
						 uint64_t& uiOrderID)
{
	SendTx(&Msg, uiFacility, uiTransactionID, uiOrderID);
}



void CISESession::SendQuery(const void* pQueryData,
							const uint32 uiFacility,
							void* pAnswerData,
							uint32& uiAnswerLen,
							uint64_t& uiTransactionID,
							uint64_t& uiOrderID)
{
	int32  iTranStatus = 0;

	if(IsLogonEnabled() == false)
		throw CISEException(0, 0, this, "Operation cancelled.");

	CAutoLock	OmniApiLock(&m_OmniApiLock);

	int32  iStatus = omniapi_query_ex(m_hSession, &iTranStatus, 
			uiFacility, (omni_message*)pQueryData, TRUE, 
			(int8*) pAnswerData, &uiAnswerLen, 
			(unsigned*)&uiTransactionID, (unsigned*)&uiOrderID);

	if(CSTATUS_FAILED(iStatus))
	{
		throw CISEException(iStatus, iTranStatus, this, "Query error");
	}
}

void CISESession::SendQuery(const CISEMessage& Query,
							const uint32 uiFacility,
							CISEAnswer& Answer,
							uint64_t& uiTransactionID,
							uint64_t& uiOrderID)
{
	SendQuery(&Query, uiFacility, &Answer.m_Data, Answer.m_uiLen, uiTransactionID, uiOrderID);
}

CISESession::CISESession()
{
	m_hApiTimeoutEvent = NULL;
	m_hSession = omniapi_create_session();
	m_bLoggedOn = false;
}

CISESession::~CISESession()
{
	omniapi_close_session(m_hSession);
}

/*--------------------------------[ Logon & Logout ]--------------------------------------------*/

VOID CALLBACK CISESession::ApiTimeoutProc(LPVOID pArg, DWORD, DWORD)
{
	CISESession* pThis = (CISESession*)pArg;

	SOCKET s = ((WORD*)pThis->m_hSession)[3];
	if(s)
	{
		closesocket(s);
	}
};

void CISESession::Logon(const bool bPrimary,
						const char* szAddr, const char* szPort,
						const char* szUser, const char* szPassword,
						const char* szProgID, const char* szNewPassword)
{
	CAutoLock	LogonLock(&m_LogonLock);

	if(m_bLoggedOn == true)
		return;

	m_bPrimary = bPrimary;

	if (szUser == NULL || szPassword == NULL)
	{
		throw CISEException(OMNIAPI_INVARG, 0, this, "Failed to logon");
	}

	IseTrace(enInfo, "[%x] Initializing session...", m_hSession); 

	CISELogonMessage	Msg;

	// Save user credentials
	strncpy(Msg.user_s, szUser, sizeof(Msg.user_s));
	strncpy(Msg.pass_s, szPassword, sizeof(Msg.pass_s));
	if(szProgID)
		strncpy(Msg.idnt_s, szProgID, sizeof(Msg.idnt_s));
	if(szNewPassword)
		strncpy(Msg.new_pass_s, szProgID, sizeof(Msg.new_pass_s));

	strncpy(Msg.node_s, szAddr, sizeof(Msg.node_s) - 1);
	Msg.node_s[sizeof(Msg.node_s)] = 0;
	strncpy(Msg.sock_s, szPort, sizeof(Msg.sock_s) - 1);
	Msg.sock_s[sizeof(Msg.sock_s)] = 0;
	
	uint64_t uiTransactionID;
	uint64_t uiOrderID;

	try
	{
		IseTrace(enInfo, "[%x] Logging into '%s:%s' as user '%s'...", m_hSession, Msg.node_s, Msg.sock_s, Msg.user_s);

		m_hApiTimeoutEvent = CreateWaitableTimer(NULL, FALSE, NULL);

		{
			// under timeout
			CISEApiTimeout ApiTimeout(m_hApiTimeoutEvent, ISE_API_TIMEOUT, ApiTimeoutProc, this);

			// Logon transaction
			SendTx(Msg, OMNI_FACTYP_LOGIN, uiTransactionID, uiOrderID);
		}

		IseTrace(enInfo, "[%x] Logged on successfully.", m_hSession);

		// wait 5 secs after logon
		IseTrace(enInfo, "[%x] Waiting 5 seconds...", m_hSession);
		
		if(LogonSleep(5000) == false)
			throw CISEException(0, 0, this, "Operation cancelled.");

		// Set keep alive
		tcp_keepalive KeepAlive;
		KeepAlive.onoff = true;
		KeepAlive.keepalivetime = 3000;
		KeepAlive.keepaliveinterval = 3000;
		SOCKET s = ((WORD*)m_hSession)[3];

		DWORD nBytes;
		int nRet = WSAIoctl(s, SIO_KEEPALIVE_VALS, 
						(LPVOID)&KeepAlive, sizeof(KeepAlive),
						NULL, 0, 
						&nBytes, 
						NULL, NULL);

		// Collect user info
		IseTrace(enInfo, "[%x] Collecting user information...", m_hSession);
		QueryUserInfo(szUser, szNewPassword ? szNewPassword : szPassword);
		IseTrace(enInfo, "[%x] Collected.", m_hSession);

		// Start broadcast thread
		DWORD dwErr = Start(NULL);
		if(dwErr)
		{
			throw CISEException(0, 0, this, "Failed to create thread");
		}

		// Subscribe broadcasts
		IseTrace(enInfo, "[%x] Subscribing general broadcasts...", m_hSession);
		SubscribeAll();
		IseTrace(enInfo, "[%x] Subscribed.", m_hSession);

		if(bPrimary)
		{
			// wait 2 secs after getting info
			IseTrace(enInfo, "[%x] Waiting 2 seconds...", m_hSession);
			
			if(LogonSleep(2000) == false)
				throw CISEException(0, 0, this, "Operation cancelled.");

			// Load static data
			Load(this);

			// restore subscriptions
			RestoreSubscriptions();
		}

		ReadyToTrade();

		IseTrace(enInfo, "[%x] Session initizalized.", m_hSession);
	}
	catch(CISEException& e)
	{
		IseTrace(enError, "[%x] Failed to initialize : %s.", m_hSession, e.GetDescription());

		try
		{
			Logoff();
		}
		catch(CISEException&)
		{
		}

		throw e;
	}

	m_bLoggedOn = true;
};

void CISESession::Logoff()
{
	CAutoLock	LogonLock(&m_LogonLock);
	
	m_bLoggedOn = false;

	Stop();

	if(m_hApiTimeoutEvent)
	{
		CloseHandle(m_hApiTimeoutEvent);
		m_hApiTimeoutEvent = NULL;
	}

	if(m_bPrimary)
	{
		Clear();
	}

	CISELogoffMessage	Msg;

	try
	{
		uint64_t uiTransactionID;
		uint64_t uiOrderID;

		SendTx(Msg, OMNI_FACTYP_LOGOUT, uiTransactionID, uiOrderID);
	}
	catch(CISEException&)
	{
		throw;
	}
};


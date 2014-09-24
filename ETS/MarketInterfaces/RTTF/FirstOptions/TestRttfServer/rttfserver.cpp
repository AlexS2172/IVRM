#include "stdafx.h"

#include "rttfserver.h"
#include "data.h"

int CRttfServer::ReadIt(char* read_handle, char* buf, int len)
{
	SOCKET * sock = (SOCKET *) read_handle;
	return (recv(*sock, buf, len, 0));
}

int CRttfServer::WriteIt(char* write_handle, char* buf, int len)
{
	SOCKET * sock = (SOCKET *) write_handle;
	return (send(*sock, buf, len, 0));
}


DWORD _METH_ CRttfServer::Start(const tstring & strPort)
{
	DWORD dwErr;
	WSADATA wsaData;
	WORD wVersionRequested = MAKEWORD(2, 0);

	try
	{

		dwErr = WSAStartup(wVersionRequested, &wsaData);
		if(dwErr)
			throw dwErr;
		if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 0)
		{
			return ERROR_OLD_WIN_VERSION;
		}

  		if ( (m_Socket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
   		{
			dwErr = WSAGetLastError();
			if(dwErr) throw dwErr;
   		}

		SOCKADDR_IN serveraddress;
   		memset(&serveraddress, 0, sizeof(serveraddress));
   		serveraddress.sin_family = AF_INET;
   		serveraddress.sin_port = htons(_ttoi(strPort.c_str()));
		serveraddress.sin_addr.s_addr = htonl(INADDR_ANY);       
		
		if(bind(m_Socket, (struct sockaddr *)&serveraddress, sizeof(serveraddress)))
		{
			dwErr = WSAGetLastError();
			if(dwErr) throw dwErr;
		}

		if(listen(m_Socket, SOMAXCONN))
		{
			dwErr = WSAGetLastError();
			if(dwErr) throw dwErr;
		}

		m_hStopEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
		if(NULL == m_hStopEvent)
		{
			dwErr = GetLastError();
			if(dwErr) throw dwErr;
		}

		unsigned int uiID;
		m_hAcceptThread = (HANDLE)_beginthreadex(NULL, 0, AcceptThread, this, 0, &uiID);
		if(m_hAcceptThread == (void*)-1)
		{
			dwErr = GetLastError();
			if(dwErr) throw dwErr;
		}
	}
	catch(DWORD dwErr)
	{
		if(m_Socket != INVALID_SOCKET)
			closesocket(m_Socket);

		WSACleanup();
		return dwErr;
	}

	return 0;
};

UINT __stdcall CRttfServer::AcceptThread( void *pThisClass)
{
	CRttfServer * pThis = (CRttfServer*) pThisClass;

	while(true)
	{
		if(WAIT_OBJECT_0 == ::WaitForSingleObject(pThis->m_hStopEvent, 10))
			return 0;

		fd_set ReadFds;
        FD_ZERO(&ReadFds);
		FD_SET(pThis->m_Socket, &ReadFds);
		timeval tmVal;
		tmVal.tv_sec = 0;
		tmVal.tv_usec = 0;
		DWORD  dwRes = select(0, &ReadFds, NULL, NULL, &tmVal);
		if(dwRes > 0)
		{
			SOCKET sockClient = accept(pThis->m_Socket, NULL, NULL);
			if(INVALID_SOCKET != sockClient)
			{
				unsigned int uiID;
				HANDLE hThread = (HANDLE)_beginthreadex(NULL, 0, WorkThread, pThisClass, CREATE_SUSPENDED , &uiID);
				if(hThread != (void*)-1)
				{
					pThis->m_ThreadId2Socket[uiID] = sockClient;
					ResumeThread(hThread);
					CloseHandle(hThread);
				}
			}
			else
			{
				_tprintf(_T("Socket error occured : %d"), WSAGetLastError());
			}
		}
	}

	return 0;
}

FocCsMsg* _METH_ CRttfServer::MakeResponse(const FocCsMsg* const pMsg)
{
	FocCsMsg* pMsgRes = NULL;

	switch (pMsg->header.msg_class)
	{
		// CS messages
		case FOC_CLASS_SYSTEM:
		{
			switch (pMsg->header.service_id)
			{
				case FOC_SRVC_LOGIN_RQST:
					// [cheching pwd is omitted..]
					pMsgRes = new FocCsLoginResponseMsg(ORIG_ID);

					// servers needed are up and subscribed by user
					((FocCsLoginResponseMsg*)pMsgRes)->subscription = (1 << FOC_SRVC_RTTF);
					((FocCsLoginResponseMsg*)pMsgRes)->servers_up = (1 << FOC_SRVC_RTTF);
					break;
			}
			break;
		}

		// RTTF messages
		case FOC_CLASS_APPLICATION:
		{
			switch (pMsg->header.service_id)
			{
				case FOC_SRVC_RTTF:
					switch (pMsg->header.msg_type)
					{
						case FOC_RTTF_TRADEREQ_MSG:
							pMsgRes = new FocRttfTradeMsg(ORIG_ID);
					}
					break;
			}
		}
	}

	return pMsgRes;
}

FocCsMsg* _METH_ CRttfServer::ParseRequest(const FocCsHeader & header)
{
	FocCsMsg* pMsg = NULL;

	// Decode the rest of the message using the header
	switch (header.msg_class)
	{
		// CS messages
		case FOC_CLASS_SYSTEM:
		{
			switch (header.service_id)
			{
				case FOC_SRVC_LOGIN_RQST:
					pMsg = new FocCsLoginMsg(header);
					break;

				default:
					fprintf(stderr, "Unsupported system message: service_id <%ld> msg_type <%ld>\n", header.service_id, header.msg_type);
					break;
			}

			break;
		}

		// RTTF messages
		case FOC_CLASS_APPLICATION:
		{
			switch (header.service_id)
			{
				case FOC_SRVC_RTTF:
					switch (header.msg_type)
					{
						case FOC_RTTF_TRADEREQ_MSG:
							pMsg = new FocRttfTradeRequestMsg(header);
							break;

						default:
							_ftprintf(stderr, 
								_T("Unsupported application message: msg_type <%ld>\n"), 
								header.msg_type);
							break;
					}
					break;

				default:
					_ftprintf(stderr, 
						_T("Ignoring application message from service_id <%ld>\n"), 
						header.service_id);
			}
		}

		default:
			return FALSE;
	}//switch class

	return pMsg;
}

int nTrades = 5000;

BOOL _METH_ CRttfServer::SendNewTrade(XDR & xdrWrite)
{
	BOOL bRes = TRUE;
/*
	static int nTrade = 0;
	if(nTrade ++ == nTrades)
		exit(0);
*/
	if(g_Trades.size() == 0)
		return FALSE;

	static int iIndex = 0;

	FocRttfTradeMsg * pMsg;
	if(pMsg = new FocRttfTradeMsg(ORIG_ID))
	{
		*(foc_rttf_trade*)pMsg = g_Trades[iIndex];
#if 0	// for test
		time_t tTime = time(NULL) % 100 + 613100;
		sprintf(pMsg->trade_id, "%d", tTime);
		pMsg->ins_upd_del_flag = 'Y';
#endif

		// Encode the message
		if (!pMsg->xdr_convert(&xdrWrite, XDR_ENCODE))
		{
			_ftprintf(stderr, _T("Failed to encode queued message!\n"));
			bRes = FALSE;
		}

		if (!xdrrec_endofrecord(&xdrWrite, TRUE))
		{
			_ftprintf(stderr, _T("Failed to send queued message!\n")); 
			bRes = FALSE;
		}

		delete pMsg;

		FocRttfTradeRequestMsg* pMsg2 = new FocRttfTradeRequestMsg(ORIG_ID);
		// Encode the message
		if (!pMsg2->xdr_convert(&xdrWrite, XDR_ENCODE))
		{
			_ftprintf(stderr, _T("Failed to encode queued message!\n"));
			bRes = FALSE;
		}

		if (!xdrrec_endofrecord(&xdrWrite, TRUE))
		{
			_ftprintf(stderr, _T("Failed to send queued message!\n")); 
			bRes = FALSE;
		}

		delete pMsg2;


	}
	else
		bRes = FALSE;

	if(++iIndex >= g_Trades.size())
		iIndex = 0;

	return bRes;
}


UINT __stdcall CRttfServer::WorkThread( void *pThisClass)
{
	CRttfServer * pThis = (CRttfServer*) pThisClass;
	InterlockedIncrement(&pThis->m_nWorkThreads);

	SOCKET sock = pThis->m_ThreadId2Socket[GetCurrentThreadId()];

	bool bLoggedIn = false;

	XDR xdrRead;
	XDR xdrWrite;
	FocCsHeader header;
	FocCsMsg* pMsg = NULL;

	xdrrec_create(&xdrRead, 4096, 4096, (caddr_t) &sock, ReadIt, WriteIt);
	xdrrec_create(&xdrWrite, 4096, 4096, (caddr_t) &sock, ReadIt, WriteIt);

	while(true)
	{
		if(WAIT_OBJECT_0 == ::WaitForSingleObject(pThis->m_hStopEvent, 1000))
			break;

		if(bLoggedIn)
		{
			// if user logged in 
			// send him the trades each N secs
			if(time(NULL) % 2 == 0)
			{
				if(pThis->SendNewTrade(xdrWrite) == FALSE)
				{
					break;
				}
				Sleep(1000);
			}
		}



		fd_set ReadFds;
        FD_ZERO(&ReadFds);
		FD_SET(sock, &ReadFds);
		timeval tmVal;
		tmVal.tv_sec = 0;
		tmVal.tv_usec = 0;
		DWORD  dwRes = select(0, &ReadFds, NULL, NULL, &tmVal);
		if(dwRes <= 0)
			continue;

		do
		{
			// Skip any fragmented records
			if (!xdrrec_skiprecord(&xdrRead))
			{
				fprintf(stderr, "xdrrec_skiprecord failed!\n");
				break;
			}

			// Get the header of the message 
			if (!header.xdr_convert(&xdrRead, XDR_DECODE))
			{
				fprintf(stderr, "Failed to decode message header!\n");
				continue;
			}

			pMsg = pThis->ParseRequest(header);

			if (pMsg)
			{
					// Decode the body of the message
				if (!pMsg->xdr_convert(&xdrRead, XDR_DECODE))
					_ftprintf(stderr, _T("Failed to decode message body!\n"));
				else
				{
					// Make response
					FocCsMsg* pMsg2;
					if(pMsg2 = pThis->MakeResponse(pMsg))
					{
						// Encode the message
						if (!pMsg2->xdr_convert(&xdrWrite, XDR_ENCODE))
							_ftprintf(stderr, _T("Failed to encode queued message!\n"));

						if (!xdrrec_endofrecord(&xdrWrite, TRUE))
							_ftprintf(stderr, _T("Failed to send queued message!\n")); 

						if(	header.msg_class == FOC_CLASS_SYSTEM && 
							header.service_id == FOC_SRVC_LOGIN_RQST && 
							((FocCsLoginResponseMsg*)pMsg2)->reason == 0
						  )
						bLoggedIn = true;

						delete pMsg2;
					}
				}
				delete pMsg;
			}
		}while (!xdrrec_eof(&xdrRead));


	}// while(true)

	xdr_destroy(&xdrRead);
	xdr_destroy(&xdrWrite);

	InterlockedDecrement(&pThis->m_nWorkThreads);
	return 0;
}


DWORD _METH_ CRttfServer::Stop()
{
	if(NULL == m_hStopEvent)
		return ERROR_SERVICE_NOT_ACTIVE;

	SetEvent(m_hStopEvent);

	WaitForSingleObject(m_hAcceptThread, INFINITE);

	CloseHandle(m_hAcceptThread);

	while(InterlockedCompareExchange(&m_nWorkThreads, 0, 0) != 0)
		Sleep(100);

	CloseHandle(m_hStopEvent);

	if(m_Socket != INVALID_SOCKET)
		closesocket(m_Socket);

	WSACleanup();

	return 0;
}


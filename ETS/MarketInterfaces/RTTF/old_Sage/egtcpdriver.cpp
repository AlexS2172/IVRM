#include "stdafx.h"

#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "Mswsock.lib")

#include "egtcpdriver.h"

DWORD WINAPI CEgTcpDriver::Init()
{
	WSADATA	wsaData;
	WORD	wVersion = MAKEWORD(2,2);


    int nRet = WSAStartup(wVersion, &wsaData); 
	if (nRet != 0)
		return nRet;

#ifdef CHECK_WSA_VERSION

	if (wVersion != wsaData.wVersion)
		return WSAVERNOTSUPPORTED;

#endif

	return 0;
};

DWORD WINAPI CEgTcpDriver::Exit()
{
	int iErr = WSACleanup();
	if (iErr == SOCKET_ERROR)
		return WSAGetLastError();

	return 0;
};

DWORD WINAPI CEgTcpDriver::Start(IEgTcpDriverEvents * pEvents, 
								 const COINIT ThreadingModel,
								 const unsigned short usServerPort)
{
	if (pEvents == NULL)
		return ERROR_INVALID_PARAMETER;

	CAutoLock lock(&m_InitLock);

	if (m_bInited)
		return ERROR_ALREADY_INITIALIZED;

	m_ThreadingModel = ThreadingModel;

	m_pEvents = pEvents;

	m_usServerPort = usServerPort;

	for(int i = 0; i < WSATOTAL_EVENTS; i++)
	{
		m_hWsaEvents[i] = WSACreateEvent();
		if (WSA_INVALID_EVENT == m_hWsaEvents[i])
			return WSAGetLastError();
	}

	m_hIocp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
    if (NULL == m_hIocp) 
		return GetLastError();

	SYSTEM_INFO	systemInfo;
    GetSystemInfo(&systemInfo);
    m_dwThreadCount = 1/*systemInfo.dwNumberOfProcessors * 2*/;


	// Create worker threads to service the overlapped I/O requests.  The decision
    // to create 1 worker threads per CPU in the system is a heuristic.  Also,
    // note that thread handles are closed right away, because we will not need them
    // and the worker threads will continue to execute.
    unsigned long  hThread;
    UINT   dwThreadId;

	for (DWORD dwCPU=0; dwCPU < m_dwThreadCount; dwCPU++) 
    {
		hThread = _beginthreadex(NULL, 0, WorkerThread, this, 0, &dwThreadId);
		if (hThread == -1) 
			return GetLastError();

		m_hThreads.push_back((HANDLE)hThread);
	}

	hThread = _beginthreadex(NULL, 0, WsaEventThread, this, 0, &dwThreadId);

	if (hThread == -1) 
			return GetLastError();

	m_hWSAEventThread = (HANDLE)hThread;

	if (m_usServerPort != CLIENT_ONLY)
	{
		DWORD dwErr = CreateListenSocket();
		if (dwErr != 0)
			return dwErr;

		dwErr = CreateAcceptSocket(TRUE);
		if (dwErr != 0)
			return dwErr;
	}
           
	m_bInited = TRUE;

	return 0;
}

DWORD WINAPI CEgTcpDriver::Stop()
{
	CAutoLock lock(&m_InitLock);

	if (!m_bInited)
		return ERROR_SERVICE_NOT_ACTIVE;

	m_bInited = FALSE;

	WSASetEvent(m_hWsaEvents[WSASHUTDOWN_EVENT]);

	WaitForSingleObject(m_hWSAEventThread, INFINITE);
	CloseHandle(m_hWSAEventThread);

    for (DWORD i = 0; i < m_dwThreadCount; i++)
        PostQueuedCompletionStatus(m_hIocp, 0, 0, NULL);

	HANDLE * ThreadHandles = new HANDLE[m_dwThreadCount];
	for (i = 0; i < m_dwThreadCount; i++)
		ThreadHandles[i] = m_hThreads[i];

	WaitForMultipleObjects(m_dwThreadCount,  ThreadHandles, TRUE, INFINITE);

	delete [] ThreadHandles;
	for (i = 0; i < m_dwThreadCount; i++)
		CloseHandle(m_hThreads[i]);
	
	m_hThreads.clear();

	if (m_usServerPort != CLIENT_ONLY)
	{
		closesocket(m_ListenSocket);                                
		closesocket(m_ListenSocketContext.m_pIOContext.m_SocketAccept);

		while (!HasOverlappedIoCompleted((LPOVERLAPPED)&m_ListenSocketContext.m_pIOContext.m_SendOverlapped)) 
		   Sleep(0);

		while (!HasOverlappedIoCompleted((LPOVERLAPPED)&m_ListenSocketContext.m_pIOContext.m_ReceiveOverlapped)) 
		   Sleep(0);
	}

	CloseHandle(m_hIocp);

	for(i = 0; i < WSATOTAL_EVENTS; i++)
		WSACloseEvent(m_hWsaEvents[i]);

	for(set<PER_SOCKET_CONTEXT>::iterator
		it = m_SocketContexts.begin(); 
		it != m_SocketContexts.end();
		)
	{
		set<PER_SOCKET_CONTEXT>::iterator DelIt = it;
		it++;
		CloseClient(*DelIt);
	}

	return 0;
}

DWORD WINAPI CEgTcpDriver::Connect(const SOCKADDR_IN & Server)
{
	if (!m_bInited)
		return 0;//0 is needed for Transport

	SOCKET Sock = CreateSocket();

	if (Sock == INVALID_SOCKET)
		return WSAGetLastError();

    int iRes = WSAEventSelect(Sock, m_hWsaEvents[WSAEVENTSELECT_EVENT], FD_CONNECT);
	if (iRes == SOCKET_ERROR )
	{
		DWORD dwErr = WSAGetLastError();
		closesocket(Sock);
		return dwErr;
	}

	PER_SOCKET_CONTEXT	ConnectContext;
	
	ConnectContext.m_ConnectioID.m_Socket = Sock;	
	ConnectContext.m_pIOContext.m_IoOperation = ClientIoRead;
	ConnectContext.m_ConnectioID.m_RemoteAddr = Server;

	m_SocketContextsLock.Lock();
	
	pair<set<PER_SOCKET_CONTEXT>::iterator, bool> InsIt;
	
	while(true)
	{
		InsIt = m_SocketContexts.insert(ConnectContext);
		if (InsIt.second == true)
			break;
		
		SwitchToThread();
	}

	PER_SOCKET_CONTEXT & SocketContext = *(InsIt.first);

	// ConnectEx lives on XP only ;((
    int iRet = WSAConnect(SocketContext.m_ConnectioID.m_Socket, 
        (LPSOCKADDR)&Server, sizeof(Server), 
        NULL, NULL, NULL, NULL);

	DWORD dwErr = 0;
    if (SOCKET_ERROR == iRet && (WSAEWOULDBLOCK != WSAGetLastError()))
    {
        dwErr = WSAGetLastError();

		if (dwErr == WSAECONNREFUSED) // Microsoft "feature"
		{
			WSANETWORKEVENTS NetworkEvents;
			int iRes = WSAEnumNetworkEvents(Sock, m_hWsaEvents[WSAEVENTSELECT_EVENT], &NetworkEvents);

			if (FD_CONNECT & NetworkEvents.lNetworkEvents)
			{
				CloseClient(SocketContext, CONNECTION_CANT_ESTABLISH);	
			}
		}
		else
			CloseClient(SocketContext, CONNECTION_CANT_ESTABLISH);	
	}

	m_SocketContextsLock.Unlock();

	return dwErr;
}

DWORD WINAPI CEgTcpDriver::ResolveAddress(const char * const pServer, 
										  const unsigned short usPort,
										  SOCKADDR_IN & addr_srv)
{
    LPHOSTENT   lpHostEnt;

	ZeroMemory((PVOID)&addr_srv, sizeof(addr_srv));
    
	addr_srv.sin_family = PF_INET;
    addr_srv.sin_port = htons(usPort);
    addr_srv.sin_addr.s_addr = inet_addr(pServer);

    if (INADDR_NONE == addr_srv.sin_addr.s_addr)
    {
	    if (NULL == (lpHostEnt = gethostbyname(pServer)))
		{
			return WSAGetLastError();
		}
		else
			CopyMemory(&addr_srv.sin_addr, 
               lpHostEnt->h_addr_list[0],
               lpHostEnt->h_length);
    }

	return 0;
}

DWORD WINAPI CEgTcpDriver::ResolveName(long lAddress, char* const pName, int iLen)
{
	// Get the host entry address 
	const hostent * pAddr = ::gethostbyaddr((const char*)&lAddress, 
		sizeof(lAddress), PF_INET );

	if (!pAddr)
		return WSAGetLastError();
	
	int iHostLen = strlen(pAddr->h_name);

	if (iHostLen >= iLen)
		return ERROR_INSUFFICIENT_BUFFER;

	strcpy(pName, pAddr->h_name);

	return 0;
};


DWORD WINAPI CEgTcpDriver::Connect(const char * const pServer, const unsigned short usPort)
{
	if (!m_bInited)
		return 0;//0 is needed for Transport

	if (pServer == NULL || usPort == 0)
		return ERROR_INVALID_PARAMETER;

	SOCKADDR_IN addr_srv;

	DWORD dwRes = ResolveAddress(pServer, usPort, addr_srv);
	if (dwRes)
		return dwRes;

	return Connect(addr_srv);
}

DWORD WINAPI CEgTcpDriver::Send(SOCKET Sock, const LPVOID const pData, const DWORD dwDataLen, BOOL bSync)
{
	if (!m_bInited)
		return 0;//0 is needed for Transport

	if (pData == NULL || dwDataLen == 0)
		return ERROR_INVALID_PARAMETER;

	PER_SOCKET_CONTEXT  FindSocketContext;
	FindSocketContext.m_ConnectioID.m_Socket = Sock;

	m_SocketContextsLock.Lock();

	set_contexts_it_t it = m_SocketContexts.find(FindSocketContext);
	if (it == m_SocketContexts.end())
	{
		m_SocketContextsLock.Unlock();
		return WSAENOTSOCK;
	}

	PER_SOCKET_CONTEXT & SocketContext = *it;
	SocketContext.m_pIOContext.m_IoOperation = ClientIoWrite;

	DWORD dwErr = 0;

	SocketContext.m_pIOContext.m_SendBuffer.Resize(dwDataLen);
	SocketContext.m_pIOContext.m_SendBuffer.m_dwTransferred = 0;

	memcpy(SocketContext.m_pIOContext.m_SendBuffer.buf, 
			pData, dwDataLen);

	dwErr = QueueSendOperation(SocketContext, bSync);

	m_SocketContextsLock.Unlock();

	return dwErr;
}

DWORD WINAPI CEgTcpDriver::QueueSendOperation(PER_SOCKET_CONTEXT & SocketContext, BOOL bSync)
{
	DWORD dwSendNumBytes = 0;
	DWORD dwFlags = 0;
	DWORD dwNeedSendBytes = SocketContext.m_pIOContext.m_SendBuffer.len;

	WSAOVERLAPPED Save = SocketContext.m_pIOContext.m_SendOverlapped;

	int nRet = WSASend(	SocketContext.m_ConnectioID.m_Socket,
					&SocketContext.m_pIOContext.m_SendBuffer, 1,
					&dwSendNumBytes,
					dwFlags,
					bSync ? NULL : &(SocketContext.m_pIOContext.m_SendOverlapped), NULL);

	DWORD dwErr = 0;

	if (bSync)
	{
		while(SocketContext.m_pIOContext.m_SendBuffer.m_dwTransferred != dwNeedSendBytes)
		{
			if (SOCKET_ERROR == nRet)
				break;

			SocketContext.m_pIOContext.m_SendBuffer.m_dwTransferred += dwSendNumBytes;
			if (SocketContext.m_pIOContext.m_SendBuffer.m_dwTransferred == dwNeedSendBytes)
				return 0;

			SocketContext.m_pIOContext.m_SendBuffer.buf += dwSendNumBytes;
			SocketContext.m_pIOContext.m_SendBuffer.len -= dwSendNumBytes;

			nRet = WSASend(	SocketContext.m_ConnectioID.m_Socket,
							&SocketContext.m_pIOContext.m_SendBuffer, 1,
							&dwSendNumBytes,
							dwFlags,
							NULL, NULL);
		}
	}

	if (SOCKET_ERROR == nRet && (WSA_IO_PENDING != WSAGetLastError())) 
    {
		dwErr = WSAGetLastError();

		SocketContext.m_pIOContext.m_SendOverlapped = Save;

		return dwErr;
    }

	return 0;
}

DWORD WINAPI CEgTcpDriver::Close(SOCKET Sock)
{
	if (!m_bInited)
		return 0;//0 is needed for Transport

	DWORD dwRes = 0;

	int iRes = shutdown(Sock, SD_SEND);// initiates gracefull shutdown

	if (iRes == SOCKET_ERROR)
		dwRes = WSAGetLastError();

	return dwRes;
}

unsigned int __stdcall CEgTcpDriver::WsaEventThread(void * pThisClass)
{
	CEgTcpDriver * pThis = (CEgTcpDriver*)pThisClass;

	CoInitializeEx(NULL, pThis->m_ThreadingModel);

	while(TRUE)
	{
		DWORD dwEvent = WSAWaitForMultipleEvents(WSATOTAL_EVENTS, 
			pThis->m_hWsaEvents, FALSE, WSA_INFINITE, FALSE);

		switch(dwEvent)
		{
			case WSA_WAIT_FAILED:
			case WAIT_IO_COMPLETION:
			case WSA_WAIT_TIMEOUT:
				break;

			default:
				dwEvent -= WSA_WAIT_EVENT_0;
				switch(dwEvent)
				{
					case WSASHUTDOWN_EVENT:
						CoUninitialize();
					    return(0);

					case WSAEVENTSELECT_EVENT:
						set<SOCKET>	ConnectedSockets;
						set<SOCKET>	ClosedSockets;

						pThis->m_SocketContextsLock.Lock();

						for(set_contexts_it_t it = pThis->m_SocketContexts.begin(); 
							it != pThis->m_SocketContexts.end(); it++)
						{
								WSANETWORKEVENTS NetworkEvents;
								const PER_SOCKET_CONTEXT & SocketContext = *it;
								int iRes = WSAEnumNetworkEvents(SocketContext.m_ConnectioID.m_Socket, 
									pThis->m_hWsaEvents[WSAEVENTSELECT_EVENT], &NetworkEvents);

								if (iRes != SOCKET_ERROR)
								{
									if (FD_CONNECT & NetworkEvents.lNetworkEvents)
									{
										// trying to perform operation to find out socket status

										DWORD dwClosed = FD_CLOSE & NetworkEvents.lNetworkEvents;

										int iSize = sizeof(SocketContext.m_ConnectioID.m_LocalAddr);
										int iErr = getsockname(SocketContext.m_ConnectioID.m_Socket, 
													(struct sockaddr FAR *)&SocketContext.m_ConnectioID.m_LocalAddr, 
													&iSize);

										if (iErr != SOCKET_ERROR &&
											SocketContext.m_ConnectioID.m_LocalAddr.sin_addr.S_un.S_addr != 0 &&
											dwClosed == 0)
											ConnectedSockets.insert(SocketContext.m_ConnectioID.m_Socket);
										else
											ClosedSockets.insert(SocketContext.m_ConnectioID.m_Socket);
									}
								}
						}

						PER_SOCKET_CONTEXT FindSocketContext;

						for(set<SOCKET>::iterator it2 = ClosedSockets.begin();
												  it2 != ClosedSockets.end(); it2++)
						{
							FindSocketContext.m_ConnectioID.m_Socket = *it2;

							it = pThis->m_SocketContexts.find(FindSocketContext);

							if (it == pThis->m_SocketContexts.end())
								continue;

							PER_SOCKET_CONTEXT & SocketContext = *it;
							pThis->CloseClient(SocketContext, CONNECTION_CANT_ESTABLISH);
						}

						for(it2 = ConnectedSockets.begin();
												  it2 != ConnectedSockets.end(); it2++)
						{
							FindSocketContext.m_ConnectioID.m_Socket = *it2;
							
							it = pThis->m_SocketContexts.find(FindSocketContext);

							if (it == pThis->m_SocketContexts.end())
								continue;

							PER_SOCKET_CONTEXT & SocketContext = *it;

							DWORD dwErr = pThis->BindCompletionPort(SocketContext);

							if (dwErr != 0)
							{
								pThis->CloseClient(SocketContext);
								continue;
							}

							BOOL bConnectionEnabled = 
									(m_bInited ?
									pThis->m_pEvents->OnConnect(SocketContext.m_ConnectioID, FALSE):
									FALSE);

							if (bConnectionEnabled)
							{
								pThis->QueueReceiveOperation(SocketContext, DEFAULT_BUFFER_SIZE);
							}
							else
							{
								pThis->CloseClient(SocketContext, CONNECTION_DISCARDED);
							}
						}

						pThis->m_SocketContextsLock.Unlock();
						
						break;
				}
				break;
		}
	}
}


unsigned int __stdcall CEgTcpDriver::WorkerThread(void * pThisClass)
{
	CEgTcpDriver * pThis = (CEgTcpDriver*)pThisClass;

    BOOL            bSuccess = FALSE;
    int             nRet;
    DWORD           dwIoSize;
	DWORD			dwRes = 0;

    LPOVERLAPPED    lpOverlapped = NULL;
	PPER_SOCKET_CONTEXT lpPerSocketContext = NULL;
    PPER_IO_CONTEXT lpIOContext = NULL; 

	CoInitializeEx(NULL, pThis->m_ThreadingModel);

    while (TRUE) 
    {

        // continually loop to service io completion packets
        bSuccess = GetQueuedCompletionStatus(
                       pThis->m_hIocp,
                       &dwIoSize,
                       (PDWORD_PTR)&lpPerSocketContext,
                       &lpOverlapped,
                       INFINITE 
                       );

		// exit condition
        if (lpPerSocketContext == NULL)
			break;

		lpIOContext = (PPER_IO_CONTEXT)&lpPerSocketContext->m_pIOContext;

        //We should never skip the loop and not post another AcceptEx if the current
        //completion packet is for previous AcceptEx
        if (lpIOContext->m_IoOperation != ClientIoAccept)
        {
            if (!bSuccess || (bSuccess && (0 == dwIoSize)) )
            {

#ifdef _DEBUG
				DWORD dwErr = GetLastError();
#endif
                // client connection dropped
				pThis->m_SocketContextsLock.Lock();
                pThis->CloseClient(*lpPerSocketContext); 
				pThis->m_SocketContextsLock.Unlock();
                continue;
            }
        }


         switch (lpIOContext->m_IoOperation)
         {
            case ClientIoAccept:
			{
				SOCKADDR_IN *LocalSockaddr, *RemoteSockaddr;
				INT		 LocalSockaddrLength, RemoteSockaddrLength;

				assert(dwIoSize == 0);

				GetAcceptExSockaddrs(	lpPerSocketContext->m_pIOContext.m_ReceiveBuffer.buf,
										0,  
										sizeof(SOCKADDR_IN) + 16,  
										sizeof(SOCKADDR_IN) + 16,  
										(SOCKADDR**)&LocalSockaddr,  
										&LocalSockaddrLength,  
										(SOCKADDR**)&RemoteSockaddr,  
										&RemoteSockaddrLength  
									);

				PER_SOCKET_CONTEXT AcceptSocketContext;
				AcceptSocketContext.m_ConnectioID.m_LocalAddr = *LocalSockaddr;
				AcceptSocketContext.m_ConnectioID.m_RemoteAddr = *RemoteSockaddr;
				AcceptSocketContext.m_pIOContext.m_IoOperation = ClientIoRead;
				AcceptSocketContext.m_ConnectioID.m_Socket = lpPerSocketContext->m_pIOContext.m_SocketAccept;

				pair<set<PER_SOCKET_CONTEXT>::iterator, bool> InsIt;

				pThis->m_SocketContextsLock.Lock();

				while(true)
				{
					InsIt = pThis->m_SocketContexts.insert(AcceptSocketContext);
					if (InsIt.second == true)
						break;
					
					SwitchToThread();
				}
				
				nRet = setsockopt(lpPerSocketContext->m_pIOContext.m_SocketAccept, 
								  SOL_SOCKET,
								  SO_UPDATE_ACCEPT_CONTEXT,
								  (char *)&(pThis->m_ListenSocket),
								  sizeof(pThis->m_ListenSocket)
								 );

                if (nRet == SOCKET_ERROR)
				{
#ifdef _DEBUG
					DWORD e = WSAGetLastError();
#endif
					pThis->CloseClient(AcceptSocketContext);
					pThis->m_SocketContextsLock.Unlock();
					break;
				}


				PER_SOCKET_CONTEXT & SocketContext = *(InsIt.first);

				dwRes = pThis->BindCompletionPort(SocketContext);
				if (dwRes != 0)
				{
					pThis->CloseClient(AcceptSocketContext);
					pThis->m_SocketContextsLock.Unlock();
					break;
				}

				// while Send method is not called we are waiting for client's data
				
				BOOL bConnectionEnabled = 
						m_bInited ?
						pThis->m_pEvents->OnConnect(SocketContext.m_ConnectioID, TRUE) :
						FALSE;

				if (bConnectionEnabled)
				{
					pThis->QueueReceiveOperation(SocketContext, DEFAULT_BUFFER_SIZE);
				}
				else
				{
					pThis->CloseClient(SocketContext, CONNECTION_DISCARDED);
				}

				pThis->m_SocketContextsLock.Unlock();

				// pending for another connection
				dwRes = pThis->CreateAcceptSocket(FALSE);
			}
			break;

			case ClientIoRead:
			case ClientIoWrite:

				DWORD cbTransferReceive;
				DWORD cbTransferSend;
				DWORD dwFlags;
				BOOL  bRes;

				bRes = WSAGetOverlappedResult(lpPerSocketContext->m_ConnectioID.m_Socket,
									&lpPerSocketContext->m_pIOContext.m_SendOverlapped,
									&cbTransferSend,FALSE, &dwFlags);

				if (bRes == TRUE && cbTransferSend == dwIoSize)
				{
				}
#ifdef _DEBUG
				if (bRes == FALSE)
				{
					DWORD dwErr = WSAGetLastError();
				}
#endif
				bRes = WSAGetOverlappedResult(lpPerSocketContext->m_ConnectioID.m_Socket,
									&lpPerSocketContext->m_pIOContext.m_ReceiveOverlapped,
									&cbTransferReceive,FALSE, &dwFlags);

				if (bRes == TRUE && cbTransferReceive == dwIoSize)
				{

					if (pThis->m_bInited)
						pThis->m_pEvents->OnReceive(lpPerSocketContext->m_ConnectioID, 
							lpPerSocketContext->m_pIOContext.m_ReceiveBuffer.m_pData, 
							dwIoSize);

					lpPerSocketContext->m_pIOContext.m_ReceiveBuffer.m_dwTransferred = 0;

					pThis->QueueReceiveOperation(*lpPerSocketContext, DEFAULT_BUFFER_SIZE);
				}
#ifdef _DEBUG
				if (bRes == FALSE)
				{
					DWORD dwErr = WSAGetLastError();
				}
#endif
				break;
		 }

	}

	CoUninitialize();
    return(0);
}

DWORD WINAPI CEgTcpDriver::QueueReceiveOperation(PER_SOCKET_CONTEXT & SocketContext, DWORD dwLen, DWORD dwOffset)
{

	DWORD dwRecvNumBytes = 0;
	DWORD dwFlags = 0;

	WSAOVERLAPPED Save = SocketContext.m_pIOContext.m_ReceiveOverlapped;

	SocketContext.m_pIOContext.m_ReceiveBuffer.len = dwLen;
	SocketContext.m_pIOContext.m_ReceiveBuffer.Shift(dwOffset);

	int nRet = WSARecv(	SocketContext.m_ConnectioID.m_Socket,
					&SocketContext.m_pIOContext.m_ReceiveBuffer, 1,
					&dwRecvNumBytes,
					&dwFlags,
					&(SocketContext.m_pIOContext.m_ReceiveOverlapped), NULL);

    if (SOCKET_ERROR == nRet && (WSA_IO_PENDING != WSAGetLastError())) 
    {
		DWORD dwErr = WSAGetLastError();
		SocketContext.m_pIOContext.m_ReceiveOverlapped = Save;
		return dwErr;
    }

	return 0;
}

DWORD WINAPI CEgTcpDriver::CreateListenSocket()
{
    SOCKADDR_IN si_addrlocal;
    int         nRet;
    LINGER      lingerStruct;

    lingerStruct.l_onoff = 0;
    lingerStruct.l_linger = 0;

    m_ListenSocket = CreateSocket();
    if (INVALID_SOCKET == m_ListenSocket)
        return WSAGetLastError();

    si_addrlocal.sin_family = AF_INET;
    si_addrlocal.sin_port = htons(m_usServerPort);
    si_addrlocal.sin_addr.s_addr = htonl(INADDR_ANY);       
    nRet = bind(m_ListenSocket, (struct sockaddr *)&si_addrlocal, sizeof(si_addrlocal));

    if (SOCKET_ERROR == nRet) 
        return WSAGetLastError();

    nRet = listen(m_ListenSocket, SOMAXCONN);
    if (SOCKET_ERROR == nRet) 
        return WSAGetLastError();
    
	return 0;
}

DWORD CEgTcpDriver::CreateAcceptSocket(BOOL bBindIocp)
{
    DWORD       dwRecvNumBytes = 0;     
	DWORD		dwRes;

    if (bBindIocp)
    {
		m_ListenSocketContext.m_ConnectioID.m_Socket = m_ListenSocket;
		m_ListenSocketContext.m_pIOContext.m_IoOperation = ClientIoAccept;
        dwRes = BindCompletionPort(m_ListenSocketContext);
        if (dwRes != 0)
            return dwRes;
    }

    m_ListenSocketContext.m_pIOContext.m_SocketAccept = CreateSocket();
    if (INVALID_SOCKET == m_ListenSocketContext.m_pIOContext.m_SocketAccept)
        return WSAGetLastError();

    int nRet = AcceptEx(
            m_ListenSocket, 
            m_ListenSocketContext.m_pIOContext.m_SocketAccept,
            (LPVOID)(m_ListenSocketContext.m_pIOContext.m_ReceiveBuffer.buf),
            0,
            sizeof(SOCKADDR_IN) + 16,
            sizeof(SOCKADDR_IN) + 16,
            &dwRecvNumBytes,
            (LPOVERLAPPED) &(m_ListenSocketContext.m_pIOContext.m_SendOverlapped)
            );
    if (nRet == SOCKET_ERROR && (ERROR_IO_PENDING != WSAGetLastError())) 
		return WSAGetLastError();

	return 0;
}

DWORD WINAPI CEgTcpDriver::BindCompletionPort(PER_SOCKET_CONTEXT & Context)
 {
	Context.m_pIOContext.m_SendOverlapped.Internal = 0;
	Context.m_pIOContext.m_SendOverlapped.InternalHigh = 0;
	Context.m_pIOContext.m_SendOverlapped.Offset = 0;
	Context.m_pIOContext.m_SendOverlapped.OffsetHigh = 0;
	Context.m_pIOContext.m_SendOverlapped.hEvent = NULL;

	Context.m_pIOContext.m_ReceiveOverlapped.Internal = 0;
	Context.m_pIOContext.m_ReceiveOverlapped.InternalHigh = 0;
	Context.m_pIOContext.m_ReceiveOverlapped.Offset = 0;
	Context.m_pIOContext.m_ReceiveOverlapped.OffsetHigh = 0;
	Context.m_pIOContext.m_ReceiveOverlapped.hEvent = NULL;

	Context.m_pIOContext.m_SocketAccept = INVALID_SOCKET;

	HANDLE hSave = m_hIocp;
    m_hIocp = CreateIoCompletionPort((HANDLE)Context.m_ConnectioID.m_Socket, m_hIocp, (DWORD_PTR)&Context, 0);
    if (NULL == m_hIocp) 
	{
		DWORD s = GetLastError();
		return GetLastError();
	}

	return 0;
}



SOCKET WINAPI CEgTcpDriver::CreateSocket()
{
    int         nRet;
    int         nZero = 0;
    LINGER      lingerStruct;
    SOCKET      Socket = INVALID_SOCKET;


    Socket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_IP, NULL, 0, WSA_FLAG_OVERLAPPED); 
    if (INVALID_SOCKET == Socket)
        return(Socket);
/*
    // Disable send and recv buffering on the socket.  Setting SO_SNDBUF
    // to 0 causes winsock to stop bufferring sends and perform
    // sends directly from our buffers, thereby save one memory copy.
    nZero = 0;
    nRet = setsockopt(Socket, SOL_SOCKET, SO_SNDBUF, (char *)&nZero, sizeof(nZero));

    nZero = 0;
    nRet = setsockopt(Socket, SOL_SOCKET, SO_RCVBUF, (char *)&nZero, sizeof(nZero));
*/        
    lingerStruct.l_onoff = 0;
    lingerStruct.l_linger = 0;
    nRet = setsockopt(Socket, SOL_SOCKET, SO_LINGER,
                  (char *)&lingerStruct, sizeof(lingerStruct));

	tcp_keepalive KeepAlive;
	DWORD dwErr = GetKeepAliveSettings(KeepAlive);

	if (dwErr == 0)
	{
		DWORD nBytes;
		nRet = WSAIoctl(Socket, SIO_KEEPALIVE_VALS, 
						(LPVOID)&KeepAlive, sizeof(KeepAlive),
						NULL, 0, 
						&nBytes, 
						NULL, NULL);
	}
   
	return(Socket);
}

VOID WINAPI CEgTcpDriver::CloseClient(PER_SOCKET_CONTEXT & PerSocketContext, 
		const close_notify_t NotifyKind)
{
	DWORD dwErr = WSAGetLastError();

#ifdef _DEBUG
	
//	printf("On Close ThId = 0x%x addr = 0x%x\n", GetCurrentThreadId(), &PerSocketContext);

#endif

	while (!HasOverlappedIoCompleted((LPOVERLAPPED)&PerSocketContext.m_pIOContext.m_SendOverlapped)) 
       Sleep(0);

	while (!HasOverlappedIoCompleted((LPOVERLAPPED)&PerSocketContext.m_pIOContext.m_ReceiveOverlapped)) 
       Sleep(0);

	if (m_bInited)
		switch(NotifyKind)
		{
			case CONNECTION_DISCONNECTED:
				m_pEvents->OnClose(PerSocketContext.m_ConnectioID, dwErr);
				break;

			case CONNECTION_CANT_ESTABLISH:
				m_pEvents->OnConnectionFailed(PerSocketContext.m_ConnectioID);
				break;

			default:
				break;
		}

	if (PerSocketContext.m_pIOContext.m_SocketAccept != INVALID_SOCKET)
		closesocket(PerSocketContext.m_pIOContext.m_SocketAccept);

	int iErr = closesocket(PerSocketContext.m_ConnectioID.m_Socket);

#ifdef _DEBUG
	DWORD Err = WSAGetLastError();
	assert(iErr == 0);

#endif

	m_SocketContextsLock.Lock();

	set<PER_SOCKET_CONTEXT>::iterator it = m_SocketContexts.find(PerSocketContext);
	if (it != m_SocketContexts.end())
	{
		m_SocketContexts.erase(it);
	}

#ifdef _DEBUG

//	printf("count = %d\n",m_SocketContexts.size());

#endif

	m_SocketContextsLock.Unlock();

    return;    
}

void WINAPI CEgTcpDriver::DumpConnectionsTable()
{

	for(set<PER_SOCKET_CONTEXT>::iterator it = m_SocketContexts.begin(); 
		it != m_SocketContexts.end(); it++)
	{
			const PER_SOCKET_CONTEXT & ctx = *it;
			printf("%d.%d.%d.%d:%d<-%d.%d.%d.%d:%d\n\n",
				ctx.m_ConnectioID.m_LocalAddr.sin_addr.S_un.S_un_b.s_b1,
				ctx.m_ConnectioID.m_LocalAddr.sin_addr.S_un.S_un_b.s_b2,
				ctx.m_ConnectioID.m_LocalAddr.sin_addr.S_un.S_un_b.s_b3,
				ctx.m_ConnectioID.m_LocalAddr.sin_addr.S_un.S_un_b.s_b4,
				ctx.m_ConnectioID.m_LocalAddr.sin_port,
				ctx.m_ConnectioID.m_RemoteAddr.sin_addr.S_un.S_un_b.s_b1,
				ctx.m_ConnectioID.m_RemoteAddr.sin_addr.S_un.S_un_b.s_b2,
				ctx.m_ConnectioID.m_RemoteAddr.sin_addr.S_un.S_un_b.s_b3,
				ctx.m_ConnectioID.m_RemoteAddr.sin_addr.S_un.S_un_b.s_b4,
				ctx.m_ConnectioID.m_RemoteAddr.sin_port);
	}
}


DWORD	WINAPI GetKeepAliveSettings(tcp_keepalive & KeepAlive)
{
	CEgRegKey key;

	KeepAlive.onoff = true;
	KeepAlive.keepaliveinterval = DEFAULT_KEEP_ALIVE_INTERVAL;
	KeepAlive.keepalivetime		= DEFAULT_KEEP_ALIVE_TIME;

	LONG lResult = key.Open(HKEY_CURRENT_USER, SETTINGS_TRANSPORT_REGISTRY_KEY, KEY_READ);
	if (lResult != ERROR_SUCCESS )
		lResult = key.Open (HKEY_LOCAL_MACHINE, SETTINGS_TRANSPORT_REGISTRY_KEY, KEY_READ);

	if (lResult == ERROR_SUCCESS)
	{
		key.QueryValue (KeepAlive.onoff, 
			SETTINGS_TRANSPORT_KEEP_ALIVE_ON);

		key.QueryValue (KeepAlive.keepalivetime, 
			SETTINGS_TRANSPORT_KEEP_ALIVE_TIME);

		key.QueryValue (KeepAlive.keepaliveinterval, 
			SETTINGS_TRANSPORT_KEEP_ALIVE_INTERVAL);

		key.Close();
	}

	return 0;
}


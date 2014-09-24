#ifndef __EGSOCKET_H__
#define __EGSOCKET_H__

#include "lock.h"
#include "win32error.h"

#define DEFAULT_TCP_DRIVER_SERVER_PORT	5001

#define SETTINGS_TRANSPORT_REGISTRY_KEY			_T("Software\\Egar\\Common\\Transport")
#define SETTINGS_TRANSPORT_KEEP_ALIVE_INTERVAL	_T("KeepAliveInterval")
#define SETTINGS_TRANSPORT_KEEP_ALIVE_TIME		_T("KeepAliveTime")
#define SETTINGS_TRANSPORT_KEEP_ALIVE_ON		_T("KeepAliveOn")

#define DEFAULT_KEEP_ALIVE_INTERVAL		1000
#define DEFAULT_KEEP_ALIVE_TIME			1000

#define WSAEVENTSELECT_EVENT			0
#define WSASHUTDOWN_EVENT				1
#define WSATOTAL_EVENTS					2

typedef enum _SOCK_IO_OPERATION 
{
    ClientIoAccept,
    ClientIoRead,
    ClientIoWrite,
	ClientIoNothing
} SOCK_IO_OPERATION, *PSOCK_IO_OPERATION;

#define	DEFAULT_BUFFER_SIZE				8192

class CSockBuffer : public WSABUF
{
	void WINAPI Copy(const CSockBuffer & Origin)
	{
		len = Origin.len;

		m_pData = (char*)realloc((char*)m_pData, len);
		memcpy(m_pData, Origin.m_pData, len);

		m_dwTransferred = Origin.m_dwTransferred;
		buf = m_pData + (Origin.buf - Origin.m_pData);
	}

public:

	CSockBuffer()
	{
		m_pData = (char*)malloc(DEFAULT_BUFFER_SIZE);
		len = DEFAULT_BUFFER_SIZE;
		m_dwTransferred = 0;
		buf = m_pData;
	}

	CSockBuffer(const CSockBuffer & Origin)
	{
		m_pData = NULL;
		Copy(Origin);
	}

	CSockBuffer & operator=(const CSockBuffer & Origin)
	{
		Copy(Origin);
		return *this;
	}

	~CSockBuffer()
	{
		free(m_pData);
	}

	void WINAPI Resize(DWORD dwSize)
	{
		m_pData = (char*)realloc((char*)m_pData, dwSize);
		len = dwSize;
		buf = m_pData;
	}

	void Shift(DWORD Offset)
	{
		buf = m_pData + Offset;
	}

	DWORD   m_dwTransferred;
	char *  m_pData;
};


// data to be associated for every I/O operation on a socket
typedef struct _PER_IO_CONTEXT 
{
    WSAOVERLAPPED               m_SendOverlapped;	// must be first
	WSAOVERLAPPED               m_ReceiveOverlapped;

    CSockBuffer                 m_SendBuffer;
	CSockBuffer                 m_ReceiveBuffer;
    
	SOCK_IO_OPERATION           m_IoOperation;
    SOCKET                      m_SocketAccept; 

	WINAPI _PER_IO_CONTEXT()
	{
		m_SendOverlapped.hEvent = NULL;
		m_ReceiveOverlapped.hEvent = NULL;
	}
/*
	WINAPI ~_PER_IO_CONTEXT()
	{
		CloseHandle(m_SendOverlapped.hEvent);
		CloseHandle(m_ReceiveOverlapped.hEvent);
	}
*/
} PER_IO_CONTEXT, *PPER_IO_CONTEXT;

typedef struct _CONNECTION_ID
{
	SOCKADDR_IN					m_LocalAddr;
	SOCKADDR_IN					m_RemoteAddr;
    SOCKET                      m_Socket;
} CONNECTION_ID, *PCONNECTION_ID;

typedef struct _PER_SOCKET_CONTEXT 
{
    PER_IO_CONTEXT				m_pIOContext;  
	CONNECTION_ID				m_ConnectioID;

#ifdef _DEBUG
	bool						bR;
	bool						bS;
	int							nRS;
	char						m_History[10];
//	_PER_SOCKET_CONTEXT(){memset(m_History, 0, 10);bR = false; bS = false; nRS = 0;}
#endif

	bool operator < (const struct _PER_SOCKET_CONTEXT & Ctx) const
	{
		return m_ConnectioID.m_Socket < Ctx.m_ConnectioID.m_Socket;
	}

} PER_SOCKET_CONTEXT, *PPER_SOCKET_CONTEXT;

typedef set<PER_SOCKET_CONTEXT>		set_contexts_t;
typedef set_contexts_t::iterator	set_contexts_it_t;

/************************************************************************************

  NOTE TO IMPLEMENTORS :

	It is FORBIDDEN to call CEgTcpDriver methods inside methods of IEgTcpDriverEvents

************************************************************************************/

class IEgTcpDriverEvents
{
public:
	
	//		On connect - app can return FALSE thus disabling connection
	//	App must TRUE to accept connection
	virtual BOOL WINAPI OnConnect(const CONNECTION_ID& Connection, BOOL bIncoming) = 0;

	virtual void WINAPI OnConnectionFailed(const CONNECTION_ID& Connection) = 0;

	virtual void WINAPI OnClose(const CONNECTION_ID& Connection, const DWORD & dwError) = 0;

	virtual void WINAPI OnReceive(	const CONNECTION_ID&	Connection, 
									const LPVOID			pData,
									const DWORD				dwDataLen) = 0;
};

#define CLIENT_ONLY	0

class CEgTcpDriver
{
public:
	CEgTcpDriver():m_bInited(FALSE){}

	DWORD WINAPI Start(IEgTcpDriverEvents * pEvents, const COINIT ThreadingModel,
		const unsigned short usServerPort = CLIENT_ONLY);

	DWORD WINAPI Stop();

	DWORD WINAPI Connect(const SOCKADDR_IN & Server);

	DWORD WINAPI Connect(const char * const szServer, const DWORD usPort);

	DWORD WINAPI Send(SOCKET Sock, const LPVOID pData, const DWORD dwDataLen, BOOL bSync = TRUE);

	DWORD WINAPI Close(SOCKET Sock);

	static DWORD WINAPI Init();

	static DWORD WINAPI Exit();

public:

	// helpers

	static DWORD WINAPI ResolveAddress(const char * const pServer, 
								const DWORD usPort,
								SOCKADDR_IN & addr_srv);

	static DWORD WINAPI ResolveName(long lAddress, char* const pName, int iLen);

private:

	DWORD	WINAPI	CreateListenSocket();
	
	DWORD	WINAPI	CreateAcceptSocket(BOOL bBindIocp = FALSE);
	
	SOCKET	WINAPI	CreateSocket();


	typedef enum
	{
		CONNECTION_CANT_ESTABLISH,		// cant connect 
		CONNECTION_DISCONNECTED,		// connection was disconnected
		CONNECTION_DISCARDED			// dont notify client
	} close_notify_t;
	
	VOID	WINAPI	CloseClient(PER_SOCKET_CONTEXT & SocketContext, 
		const close_notify_t NotifyKind = CONNECTION_DISCONNECTED);
	
	DWORD	WINAPI	BindCompletionPort(PER_SOCKET_CONTEXT & SocketContext);

	DWORD	WINAPI	QueueReceiveOperation(PER_SOCKET_CONTEXT & SocketContext, DWORD dwLen, DWORD dwOffset = 0);

	DWORD	WINAPI	QueueSendOperation(PER_SOCKET_CONTEXT & SocketContext, BOOL bSync);

	static unsigned int __stdcall WorkerThread(void * pThisClass);
	static unsigned int __stdcall WsaEventThread(void * pThisClass);

private:

	IEgTcpDriverEvents *		m_pEvents;
	SOCKET						m_ListenSocket;
	PER_SOCKET_CONTEXT			m_ListenSocketContext;
	set_contexts_t				m_SocketContexts;
	CLock						m_SocketContextsLock;
	unsigned short				m_usServerPort;
	
	HANDLE			m_hIocp;
	HANDLE			m_hWSAEventThread;
	HANDLE			m_hWsaEvents[WSATOTAL_EVENTS];

	BOOL			m_bInited;
	CLock			m_InitLock;
	DWORD			m_dwThreadCount;

	COINIT			m_ThreadingModel;

	vector<HANDLE>	m_hThreads;

	// test
	void WINAPI DumpConnectionsTable();

};

DWORD	WINAPI GetKeepAliveSettings(tcp_keepalive & KeepAlive);

#endif //__EGSOCKET_H__


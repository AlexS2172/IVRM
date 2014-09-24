#include "stdafx.h"

#ifdef _WIN32
#include <process.h>
#else
#include <strings.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#endif

#include <errno.h>
#include "RttfClient.h"

// Constants
#ifndef _WIN32
#define INVALID_SOCKET -1
#endif

RttfClient::RttfClient() :
	m_socket(INVALID_SOCKET),
	m_connected(FALSE),
	m_available(FALSE),
	m_running(FALSE),
#ifndef _WIN32
	m_tidRead(0),
	m_tidWrite(0),
	m_tidQueue(0),
#endif
	m_pInQ(NULL),
	m_pOutQ(NULL)
{
#ifdef _WIN32
	memset(m_user_id, 0, sizeof(m_user_id));
	memset(&m_tids, 0, sizeof(m_tids));
	memset(&m_threads, 0, sizeof(m_threads));

	// Initialize WinSock
	// Note, this really shouldn't be in the constructor!
	WSADATA wsaData;
	memset(&wsaData, 0, sizeof(wsaData));
	WORD wVersionRequested = MAKEWORD(2, 0);
	WSAStartup(wVersionRequested, &wsaData);

	if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 0)
	{
		fprintf(stderr, "Failed to initialize Winsock 2.0!\n");
		exit(-1);
	}
#endif

	// Initialize XDR (read and write side)
	xdrrec_create(&m_xdrread, 4096, 4096, (caddr_t) this, readit, writeit);
	xdrrec_create(&m_xdrwrite, 4096, 4096, (caddr_t) this, readit, writeit);

	// Initialize Queues
	m_pInQ = new PQueue<FocCsMsg*> ();
	m_pOutQ = new PQueue<FocCsMsg*> ();
}

RttfClient::~RttfClient()
{
	// Close Connection
	if (m_connected)
	{
#ifdef _WIN32
		closesocket(m_socket);

		// Terminate Winsock
		// Note, this shouldn't be in destructor!
		WSACleanup();
#else
		close(m_socket);
#endif
		m_socket = INVALID_SOCKET;
		m_connected = FALSE;
	}

	// Destroy Queues
	if (m_pInQ)
		delete(m_pInQ);
	
	if (m_pOutQ)
		delete(m_pOutQ);

	// Release XDR structures
	xdr_destroy(&m_xdrread);
	xdr_destroy(&m_xdrwrite);
}

bool RttfClient::connect(const char* host, unsigned short port)
{
	bool rc = TRUE;
	struct sockaddr_in serveraddress;
	struct hostent* entry = NULL;

	try
	{
   		if ( (m_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
   		{
      		fprintf(stderr, "failed to open tcp socket (%s)\n", strerror(errno));
			throw errno;
   		}

   		memset(&serveraddress, 0, sizeof(serveraddress));
   		serveraddress.sin_family = AF_INET;
   		serveraddress.sin_port = htons(port);
   		serveraddress.sin_addr.s_addr = inet_addr(host);
#ifdef _WIN32
		if (serveraddress.sin_addr.S_un.S_addr == -1)
#else
   		if (serveraddress.sin_addr.s_addr == (in_addr_t) -1)
#endif
   		{
      		if ((entry = gethostbyname(host)) == NULL)
      		{
         		fprintf(stderr, "failed to lookup host name (%s)\n", host);
         		throw errno;
      		}

      		memcpy(&serveraddress.sin_addr, entry->h_addr, sizeof(struct in_addr));
   		}

   		if (::connect(m_socket, (struct sockaddr *) &serveraddress, sizeof(serveraddress)) < 0)
   		{
			int  dwErr = WSAGetLastError ();
			throw dwErr;
		}
	}
	catch (int err)
	{
		fprintf(stderr, "failed to connect to host %s on port %ld (Error %ld)\n", host, port, err);
		rc = FALSE;
	}

	if (rc == FALSE && m_socket > 0)
#ifdef _WIN32
		closesocket(m_socket);
#else
		close(m_socket);
#endif
	else
		m_connected = TRUE;

	return rc;
}

void RttfClient::login(const char* user_id, const char* password, const char* security_id)
{
	// Cache the user_id
	strncpy(m_user_id, user_id, USER_ID_LEN - 1);

	// Create and initialize login message
	FocCsLoginMsg* pMsg = new FocCsLoginMsg(user_id);
	strncpy((char *) pMsg->user_id, user_id, FOC_USER_ID_LEN - 1);
	strncpy((char *) pMsg->password, password, FOC_PASSWORD_LEN - 1);
	strncpy((char *) pMsg->security_id, security_id, FOC_SECURITY_ID_LEN - 1);

	fprintf(stdout, "Queueing login message: %s\n", user_id);

	// Put the message on the outbound queue
	m_pOutQ->push(pMsg);
}

void RttfClient::replay(int request_type, FOC_DATETIME* pTime)
{
	// Create and initialize trade request message
	FocRttfTradeRequestMsg* pMsg = new FocRttfTradeRequestMsg(m_user_id);
	pMsg->request_type = request_type;
	if (pTime)
		pMsg->last_upd_date = *pTime;
	else
		pMsg->last_upd_date = m_LastTrade.getDate();

	fprintf(stdout, "Queueing trade request message: %02d:%02d:%02d\n",
			pMsg->last_upd_date.hour,
			pMsg->last_upd_date.min, 
			pMsg->last_upd_date.sec);

	// Put the message on the outbound queue
	m_pOutQ->push(pMsg);
}

bool RttfClient::run(void)
{
#ifdef _WIN32
	m_threads[0] = (HANDLE) _beginthreadex(NULL, 0, read_thread, (void *) this, 0, &m_tids[0]);
	m_threads[1] = (HANDLE) _beginthreadex(NULL, 0, write_thread, (void *) this, 0, &m_tids[1]);
	m_threads[2] = (HANDLE) _beginthreadex(NULL, 0, queue_thread, (void *) this, 0, &m_tids[2]);

	WaitForMultipleObjects(3, m_threads, TRUE, INFINITE);
	CloseHandle(m_threads[0]);
	CloseHandle(m_threads[1]);
	CloseHandle(m_threads[2]);
#else
	// Start read thread
	if (pthread_create(&m_tidRead, NULL, read_thread, this))
	{
		fprintf(stderr, "Failed to create read thread\n");
		return FALSE;
	}
 
	// Start write thread
	if (pthread_create(&m_tidWrite, NULL, write_thread, this))
	{
		fprintf(stderr, "Failed to create write thread\n");
		return FALSE;
	}

	// Start queue thread
	if (pthread_create(&m_tidQueue, NULL, queue_thread, this))
	{
		fprintf(stderr, "Failed to create queue thread\n");
		return FALSE;
	}

	// Wait for threads to return
	pthread_join(m_tidRead, NULL);
	pthread_join(m_tidWrite, NULL);
	pthread_join(m_tidQueue, NULL);
#endif

	return TRUE;
}

#ifdef _WIN32
int RttfClient::readit(char* read_handle, char* buf, int len)
{
	RttfClient* pthis = (RttfClient *) read_handle;
	return (recv(pthis->m_socket, buf, len, 0));
}

int RttfClient::writeit(char* write_handle, char* buf, int len)
{
	RttfClient* pthis = (RttfClient *) write_handle;
	return (send(pthis->m_socket, buf, len, 0));
}
#else
int RttfClient::readit(void* read_handle, char* buf, int len)
{
	RttfClient* pthis = (RttfClient *) read_handle;
	return (read(pthis->m_socket, buf, len));
}

int RttfClient::writeit(void* write_handle, char* buf, int len)
{
	RttfClient* pthis = (RttfClient *) write_handle;
	return (write(pthis->m_socket, buf, len));
}
#endif

#ifdef _WIN32
unsigned WINAPI RttfClient::read_thread(void* arg)
#else
void* RttfClient::read_thread(void* arg)
#endif
{
	RttfClient* pthis = (RttfClient *) arg;
	FocCsHeader header;
	FocCsMsg* pMsg = NULL;
	fd_set readfds;
	int ready;

	while (1)
	{
		// Initialize read set
		FD_ZERO(&readfds);
		FD_SET(pthis->m_socket, &readfds);

		// Wait until the socket is readable 
		ready = select(pthis->m_socket + 1, &readfds, NULL, NULL, NULL);
		if (ready < 0)
		{
			if (errno == EINTR)
				continue;
			else
				break;
		}
		else
		{
			// Process records while there is data available
			do	
			{
				// Skip any fragmented records
				if (!xdrrec_skiprecord(&pthis->m_xdrread))
				{
					fprintf(stderr, "xdrrec_skiprecord failed!\n");
					break;
				}

				// Get the header of the message 
				if (!header.xdr_convert(&pthis->m_xdrread, XDR_DECODE))
				{
					fprintf(stderr, "Failed to decode message header!\n");
					continue;
				}

				// Initialize message pointer to NULL
				pMsg = NULL;

				// Decode the rest of the message using the header
				switch (header.msg_class)
				{
				// CS messages
				case FOC_CLASS_SYSTEM:
					switch (header.service_id)
					{
					case FOC_SRVC_LOGIN_RESPONSE:
						pMsg = new FocCsLoginResponseMsg(header);
						break;
					case FOC_SRVC_INFO:
						pMsg = new FocCsInfoMsg(header);
						break;
					default:
						fprintf(stderr, "Unsupported system message: service_id <%ld> msg_type <%ld>\n", header.service_id, header.msg_type);
						break;
					}
					break;

				// RTTF messages
				case FOC_CLASS_APPLICATION:
					switch (header.service_id)
					{
					case FOC_SRVC_RTTF:
						switch (header.msg_type)
						{
						case FOC_RTTF_TRADE_MSG:
							pMsg = new FocRttfTradeMsg(header);
							break;	
						case FOC_RTTF_TRADEREQ_MSG:
							pMsg = new FocRttfTradeRequestMsg(header);
							break;
						case FOC_RTTF_LOGINRESP_MSG:
							pMsg = new FocRttfLoginResponseMsg(header);
							break;
						default:
							fprintf(stderr, "Unsupported application message: msg_type <%ld>\n", header.msg_type);
							break;
						}
						break;

					default:
						fprintf(stderr, "Ignoring application message from service_id <%ld>\n", header.service_id);
						break;
					}
				}

				if (pMsg)
				{
					// Decode the body of the message
					if (!pMsg->xdr_convert(&pthis->m_xdrread, XDR_DECODE))
						fprintf(stderr, "Failed to decode message body!\n");
					else
					{
						// Queue the message
						pthis->m_pInQ->push(pMsg);
					}
				}
			}
			while (!xdrrec_eof(&pthis->m_xdrread));
		}
	}

	fprintf(stdout, "read_thread exiting\n");
	
#ifdef _WIN32
	return 0;
#else
	return NULL;
#endif
}

#ifdef _WIN32
unsigned WINAPI RttfClient::write_thread(void* arg)
#else
void* RttfClient::write_thread(void* arg)
#endif
{
	RttfClient* pthis = (RttfClient *) arg;
	FocCsMsg* pMsg = NULL;
	fd_set writefds;
	int ready;

	while (1)
	{
		// Get a message from the outbound queue
		pMsg = pthis->m_pOutQ->pop();

		// Initialize write set
		FD_ZERO(&writefds);
		FD_SET(pthis->m_socket, &writefds);

		// Wait until the socket is writeable 
		ready = select(pthis->m_socket + 1, NULL, &writefds, NULL, NULL);
		if (ready < 0)
		{
			if (errno == EINTR)
				continue;
			else
				break;
		}
		else
		{
			// Encode the message
			if (!pMsg->xdr_convert(&pthis->m_xdrwrite, XDR_ENCODE))
				fprintf(stderr, "Failed to encode queued message!\n");

			if (!xdrrec_endofrecord(&pthis->m_xdrwrite, TRUE))
				fprintf(stderr, "Failed to send queued message!\n"); 
		}

		// Remove the message
		delete pMsg;
	}
	
	fprintf(stdout, "write_thread exiting\n");
	
#ifdef _WIN32
	return 0;
#else
	return NULL;
#endif
}

#ifdef _WIN32
unsigned WINAPI RttfClient::queue_thread(void* arg)
#else
void* RttfClient::queue_thread(void* arg)
#endif
{
	RttfClient* pthis = (RttfClient *) arg;
	FocCsMsg* pMsg = NULL;

	while (1)
	{
		pMsg = pthis->m_pInQ->pop();

		switch (pMsg->header.msg_class)
		{
		case FOC_CLASS_SYSTEM:
			switch (pMsg->header.service_id)
			{
			case FOC_SRVC_LOGIN_RESPONSE:
				pthis->process((FocCsLoginResponseMsg *) pMsg);
				break;
			case FOC_SRVC_INFO:
				pthis->process((FocCsInfoMsg *) pMsg);
				break;	
			}
			break;
		
		case FOC_CLASS_APPLICATION:
			// Only process application messages from the RTTF service
			if (pMsg->header.service_id == FOC_SRVC_RTTF)
			{
				switch (pMsg->header.msg_type)
				{
				case FOC_RTTF_TRADE_MSG:
					if (!strchr(((FocRttfTradeMsg *) pMsg)->trade_id, '-'))
					{
   					// Keep track of the last trade update
   					if (pthis->m_running)
      					pthis->m_LastTrade.setDate(((FocRttfTradeMsg *) pMsg)->last_upd_date);

						pthis->process((FocRttfTradeMsg *) pMsg);
					}
					else
						pthis->process((FocRttfPositionMsg *) pMsg);
					break;	
				case FOC_RTTF_TRADEREQ_MSG:
					// Update state to running
					pthis->m_running = TRUE;
					break;
				case FOC_RTTF_LOGINRESP_MSG:
					pthis->process((FocRttfLoginResponseMsg *) pMsg);
					break;
				}
			}
			break;
		}
			
		// Remove the message
		delete pMsg;
	}
	
	fprintf(stdout, "queue_thread exiting\n");
	
#ifdef _WIN32
	return 0;
#else
	return NULL;
#endif
}

void RttfClient::process(FocCsLoginResponseMsg* pMsg)
{
	switch (pMsg->reason)
	{
	case FOC_ERR_NONE:
		fprintf(stdout, "Successfully connected to CS server\n");

		// Make sure user has an RTTF subscription
		if (pMsg->subscription & (1 << FOC_SRVC_RTTF))
		{
			// See if RTTF is currently available
			if (pMsg->servers_up & (1 << FOC_SRVC_RTTF))
				fprintf(stdout, "RTTF server is available\n");
			else
				fprintf(stderr, "RTTF server is currently unavailable\n");
		} 
		else
		{
			fprintf(stderr, "User %s is not subscribed to RTTF\n", m_user_id);
			exit(-1);
		}	
		break;
	
	case FOC_ERR_INVALID_USERID:
		fprintf(stderr, "User %s invalid\n", m_user_id);
		exit(-1);
	case FOC_ERR_INVALID_PASSWD:
		fprintf(stderr, "Invalid password for user %s\n", m_user_id);
		exit(-1);
	case FOC_ERR_INVALID_SECURE_ID:
		fprintf(stderr, "Invalid security ID for user %s\n", m_user_id);
		exit(-1); 
	case FOC_ERR_NO_SUBSCRIPTION:
		fprintf(stderr, "User %s is not subscribed to RTTF\n", m_user_id);
		exit(-1);
	case FOC_ERR_RELOGON_ATTEMPTED:
		fprintf(stderr, "User %s is already connected to CS\n", m_user_id);
		exit(-1);
	default:
		fprintf(stderr, "Failed to connect to CS (error %ld)\n", pMsg->reason);
		break;
	}
}

void RttfClient::process(FocCsInfoMsg* pMsg)
{
	switch (pMsg->what)
	{
	case FOC_ERR_SERVER_NOT_RESPONDING:
		if (pMsg->value & (1 << FOC_SRVC_RTTF))
		{
			// Update state variables
			m_available = FALSE;
			m_running = FALSE;

			fprintf(stderr, "RTTF server is no longer responding\n");
		}
		break;
	case FOC_SERVER_UP:
		if (pMsg->value & (1 << FOC_SRVC_RTTF))
		{
			// Update available state
			m_available = TRUE;

			fprintf(stdout, "RTTF server is back up\n");

			// Request the trades that were missed
			if (m_LastTrade.isRestart())
				replay(RTTF_TRADES_ONLY);
			else
				replay(RTTF_OPENING_POS_AND_TRADES);
		}
		break;
	default:
		fprintf(stdout, "Received CS Info Message: what %ld, value %ld, value_str %d\n", pMsg->what, pMsg->value, pMsg->value_str);
		break;
	}
}

void RttfClient::process(FocRttfLoginResponseMsg* pMsg)
{
	if (pMsg->status == RTTF_NO_ERROR)
	{
		// Set the available state
		m_available = TRUE;
		
		fprintf(stdout, "Successfully connected to RTTF server\n");
	
		// Request trades
		if (m_LastTrade.isRestart())
			replay(RTTF_TRADES_ONLY);
		else
			replay(RTTF_OPENING_POS_AND_TRADES);
	}
	else
	{
		fprintf(stderr, "Failed to login to RTTF (error %ld)\n", pMsg->status);
	}
}

void RttfClient::process(FocRttfTradeMsg* pMsg)
{
	// TO DO:
	fprintf(stdout, "Add your own logic here to process trade: %s\n", pMsg->trade_id);
}

void RttfClient::process(FocRttfPositionMsg* pMsg)
{
	// TO DO:
	fprintf(stdout, "Add your own logic here to process position: %s\n", pMsg->trade_id);
}

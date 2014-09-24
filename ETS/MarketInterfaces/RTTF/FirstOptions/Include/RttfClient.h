#ifndef __RTTFCLIENT_H__
#define __RTTFCLIENT_H__

#ifndef _WIN32
#include <rpc/rpc.h>
#else
#include "XDR.h"
#endif

#include "CsMsgs.h"
#include "RttfMsgs.h"
#include "LastTrade.h"
#include "PQueue.h"

#ifdef _WIN32
#include <winsock2.h>
#endif

class RttfClient
{
public:
	RttfClient();
	virtual ~RttfClient();

	bool connect(const char* host, unsigned short port);
	void login(const char* user_id, const char* password, const char* security_id);
	void replay(int request_type = RTTF_TRADES_ONLY, FOC_DATETIME* pTime = NULL);
	bool run(void);

	// Message callbacks
	void process(FocCsLoginResponseMsg* pMsg);
	void process(FocCsInfoMsg* pMsg);
	void process(FocRttfLoginResponseMsg* pMsg);
	void process(FocRttfTradeMsg* pMsg);
	void process(FocRttfPositionMsg* pMsg);
	
	// Last Active Trade	
	LastTrade m_LastTrade;

private:
	// State information	
	bool m_connected;
	bool m_available;
	bool m_running;
	char m_user_id[FOC_USER_ID_LEN];
	
	// Socket
#ifdef _WIN32
	SOCKET m_socket;
#else
	int m_socket;
#endif

	// XDR (one per thread)
	XDR m_xdrread;
	XDR m_xdrwrite;
	
	// Threads
#ifdef _WIN32
	unsigned m_tids[3];
	HANDLE m_threads[3];
#else
	pthread_t m_tidRead;
	pthread_t m_tidWrite;
	pthread_t m_tidQueue;
#endif

	// In/Out Message Queues
	PQueue<FocCsMsg*> *m_pInQ;
	PQueue<FocCsMsg*> *m_pOutQ;

	// Static XDR Callbacks
#ifdef _WIN32
	static int readit(char* read_handle, char* buf, int len);
	static int writeit(char* write_handle, char* buf, int len);
#else
	static int readit(void* read_handle, char* buf, int len);
	static int writeit(void* write_handle, char* buf, int len);
#endif

	// Thread Functions
#ifdef _WIN32
	static unsigned WINAPI read_thread(void *arg);
	static unsigned WINAPI write_thread(void *arg);
	static unsigned WINAPI queue_thread(void *arg);
#else
	static void* read_thread(void *arg);
	static void* write_thread(void *arg);
	static void* queue_thread(void *arg);
#endif
};

#endif

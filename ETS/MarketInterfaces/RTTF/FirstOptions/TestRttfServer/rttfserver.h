#ifndef RTTF_SERVER_H
#define RTTF_SERVER_H

#include "clearingbase.h"
#include "RttfClient.h"

#define ORIG_ID		"Test"

typedef map<unsigned int, SOCKET>	threadid2socket_t;

class CRttfServer
{
public:

	DWORD _METH_ Start(const tstring & strPort);
	DWORD _METH_ Stop();

	_METH_ CRttfServer():m_Socket(INVALID_SOCKET), 
		m_hStopEvent(NULL), m_nWorkThreads(0){}

private:

	static UINT __stdcall AcceptThread( void *pThisClass);
	static UINT __stdcall WorkThread( void *pThisClass);

	static int ReadIt(char* read_handle, char* buf, int len);
	static int WriteIt(char* write_handle, char* buf, int len);

	FocCsMsg* _METH_ ParseRequest(const FocCsHeader & header);
	FocCsMsg* _METH_ MakeResponse(const FocCsMsg* const pMsg);
	BOOL _METH_ SendNewTrade(XDR & xdrWrite);

	SOCKET		m_Socket;
	HANDLE		m_hAcceptThread;
	HANDLE		m_hStopEvent;
	LONG		m_nWorkThreads;
	threadid2socket_t	m_ThreadId2Socket;
};

#endif
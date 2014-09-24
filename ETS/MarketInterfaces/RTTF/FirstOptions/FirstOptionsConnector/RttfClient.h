#ifndef __RTTFCLIENT_H__
#define __RTTFCLIENT_H__

#include "clearingbase.h"
#include "lasttrade.h"
#include "portfolio.h"

#include "PositionFile.h"

//##ModelId=3C1DD43302A1
class CRttfClient
{
public:
	CRttfClient();
	//##ModelId=3C1DD43400F4
	virtual ~CRttfClient();

	//##ModelId=3C1DD43400BA
	DWORD Start( const char* szHost, unsigned short usPort, 
				const char* szUserId, const char* szPassword, const char* szSecurityId);
	//##ModelId=3C1DD43400B9
	DWORD Stop();

private:

	DWORD GetKeepAliveSettings(tcp_keepalive & KeepAlive);

	//##ModelId=3C1DD43400B8
	BOOL IsStopping();

	// Connection & Login USHORT
	//##ModelId=3C1DD43400A4
	DWORD Connect(const char* szHost, unsigned short usPort);
	//##ModelId=3C1DD4340086
	DWORD Login(const char* szUserId, const char* szPassword, const char* szSecurityId);

	DWORD Logout();
	//##ModelId=3C1DD434007D
	void  Disconnect();
	//##ModelId=3C1DD434007C
	DWORD Reconnect();

	//##ModelId=3C1DD4340067
	void Replay(const int iRequestType = RTTF_OPENING_POS_AND_TRADES,
		const FOC_DATETIME* pTime = NULL);

	void Clear();

	// Message callbacks
	//##ModelId=3C1DD434002B
	void Process(const FocCsLoginResponseMsg* pMsg);
	//##ModelId=3C1DD4340035
	void Process(const FocCsInfoMsg* pMsg);
	//##ModelId=3C1DD434003F
	void Process(const FocRttfLoginResponseMsg* pMsg);
	//##ModelId=3C1DD434004A
	void Process(FocRttfTradeMsgEx* pMsg);
	//##ModelId=3C1DD434005D
	void Process(FocRttfPositionMsgEx* pMsg);
	
private:

#ifdef USE_SMART_LASTTRADETIME_ALGORITHM
	time_t	m_SmartLastTime;
#endif

	CPositionFile m_PostionFile;

	//##ModelId=3C1DD433039B
	CLastTrade	m_LastTrade;

	//##ModelId=3C1DD4330387
	BOOL	m_bFocAvailable;
	//##ModelId=3C1DD4330373
	BOOL	m_bRttfAvailable;
	//##ModelId=3C1DD4330369
	BOOL	m_bTradesAreInOrder;

	//##ModelId=3C1DD4330361
	tstring m_strUserId;
	//##ModelId=3C1DD4330357
	tstring m_strPassword;
	//##ModelId=3C1DD433034B
	tstring m_strSecurityId;
	//##ModelId=3C1DD4330339
	tstring m_strHost;
	//##ModelId=3C1DD433032D
	unsigned short m_usPort;

	//##ModelId=3C1DD4330325
	SOCKET m_Socket;

	//##ModelId=3C1DD4330319
	HANDLE m_hShutdownEvent;
	//##ModelId=3C1DD433030F
	HANDLE m_hThreads[4];

	//##ModelId=3C1DD4330307
	XDR m_xdrRead;
	//##ModelId=3C1DD43302FB
	XDR m_xdrWrite;
	
	// In/Out Message Queues
	//##ModelId=3C1DD43302E9
	CLock			 m_InQueLock;
	//##ModelId=3C1DD43302DF
	queue<FocCsMsg*> m_InQue;

	multimap<FOC_DATETIME, FocRttfTradeMsgEx>	m_mmapTrades;
	CLock			 m_TradesMapLock;

	//##ModelId=3C1DD43302CB
	CLock			 m_OutQueLock;
	//##ModelId=3C1DD43302C2
	queue<FocCsMsg*> m_OutQue;

	//##ModelId=3C1DD434000D
	static int XdrReadFunc(char* hRead, char* buf, int len);
	//##ModelId=3C1DD43303D7
	static int XdrWriteFunc(char* hWrite, char* buf, int len);

	// Threads
	//##ModelId=3C1DD43303CD
	static unsigned __stdcall ReadThread(void *pThisClass);
	//##ModelId=3C1DD43303B9
	static unsigned __stdcall WriteThread(void *pThisClass);
	//##ModelId=3C1DFEEC029C
	static unsigned __stdcall OutputThread(void *pThisClass);
	//##ModelId=3C1DFEEC0238
	static unsigned __stdcall InputThread(void *pThisClass);
};

#endif

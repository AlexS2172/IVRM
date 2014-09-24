#ifndef CRS_H
#define CRS_H

#include "clearingbase.h"
#include "Publisher.h"
#include "rttfclient.h"
#include "portfolio.h"

#define SETTINGS_REGISTRY_KEY			_T("SOFTWARE\\Egar\\ETS\\FirstOptionsConnector")
#define SETTINGS_KEEP_ALIVE_INTERVAL	_T("KeepAliveInterval")
#define SETTINGS_KEEP_ALIVE_TIME		_T("KeepAliveTime")
#define SETTINGS_KEEP_ALIVE_ON			_T("KeepAliveOn")
#define SETTINGS_SERVERADDR				_T("ServerAddr")
#define SETTINGS_SERVERPORT				_T("ServerPort")
#define SETTINGS_USERID					_T("UserID")
#define SETTINGS_PASSWORD				_T("Password")
#define SETTINGS_SECURITYID				_T("SecurityID")

#define DEFAULT_KEEP_ALIVE_INTERVAL		60000
#define DEFAULT_KEEP_ALIVE_TIME			60000

#define	MAX_DBRECONNECTION_TRIES		10
#define	DBRECONNECTION_DELAY			10



//##ModelId=3C1DD4310233
class CCrs
{
	static HANDLE	m_hStopApplication;

	static BOOL __stdcall HandlerRoutine(DWORD dwCtrlType);

public:

	//##ModelId=3C1DD4310282
	DWORD Start();

	//##ModelId=3C1DD4310281
	DWORD Run();

	//##ModelId=3C1DD4310280
	DWORD Stop();

	void Shutdown();

	//##ModelId=3C1DD4310278
	CPublisher		m_Publisher;
	//##ModelId=3C1DD431026E
	CRttfClient		m_RttfClient;

	CPortfolio		m_Portfolio;
};

extern CCrs theApp;

#endif

// ServiceMonitor.h: interface for the CServiceMonitor class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(_SERVICEMONITOR_H_)
#define _SERVICEMONITOR_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000



class CServiceMonitor : public EgLib::CThread
{
public:
	CServiceMonitor();
	~CServiceMonitor();

	DWORD Start(void* pParam);
	DWORD Stop();
	

	DWORD StartTransport();
	DWORD StopTransport();
	DWORD RestartTransport();
	DWORD StartRouter();	
	DWORD StopRouter();
	DWORD GetTransportStatus();
	DWORD GetRouterStatus();
protected:
	UINT  Run();

private:

	DWORD _StopService( SC_HANDLE hService, DWORD dwTimeout );
	DWORD _StartService( SC_HANDLE hService, DWORD dwTimeout );	
	bool _QueueTimerCheckStatus(long lTimer);
	
	static LPCTSTR			m_ptchRtServiceName;
	static LPCTSTR			m_ptchTrServiceName;
 	CHandle					m_hTimerCheckService;
	CHandle					m_hStartTransport;
 	CHandle					m_hStopTransport;
 	CHandle					m_hRestartTransport;
	CHandle					m_hStartRouter;
 	CHandle					m_hStopRouter;
    SC_HANDLE				m_hSCM;
    SC_HANDLE				m_hTransport;
	DWORD					m_dwTransportStatus;
	DWORD					m_dwRouterStatus;
    SC_HANDLE				m_hRouter; 
	
	bool					m_bIsStarted;
	CLock					m_Lock;
};

extern CServiceMonitor g_ServiceMonitor;

#endif // !defined(_SERVICEMONITOR_H_)

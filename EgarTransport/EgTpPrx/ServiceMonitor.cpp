// ServiceMonitor.cpp: implementation of the CServiceMonitor class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ServiceMonitor.h"
#include "EgTpPrx.h"
#include "HostProxy.h"
#include "RouterProxy.h"

#define  DEF_CHECKTIMER 3
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CServiceMonitor g_ServiceMonitor;

LPCTSTR CServiceMonitor::m_ptchTrServiceName = _T("Egar Transport");
LPCTSTR CServiceMonitor::m_ptchRtServiceName = _T("Egar Router");


CServiceMonitor::CServiceMonitor() :
	m_hSCM(NULL),
	m_hTransport(NULL),
	m_hRouter(NULL),
	m_bIsStarted(false),
	m_dwRouterStatus(0),
	m_dwTransportStatus(0)
{

}

CServiceMonitor::~CServiceMonitor()
{
	m_hTimerCheckService.Close();
	m_hStartTransport.Close();
	m_hStopTransport.Close();
	m_hRestartTransport.Close();
	m_hStartRouter.Close();
	m_hStopRouter.Close();
	if (m_hRouter)
	{
		::CloseServiceHandle(m_hRouter);
		m_hRouter = NULL;
	}
	if (m_hTransport)
	{
		::CloseServiceHandle(m_hTransport);
		m_hTransport = NULL;
	}
	if (m_hSCM)
	{
		::CloseServiceHandle(m_hSCM);
		m_hSCM = NULL;
	}
}

DWORD CServiceMonitor::Start(void* pParam)
{
	CAutoLock	lock(m_Lock);
	
	if(m_bIsStarted)
		return ERROR_SERVICE_ALREADY_RUNNING;

	DWORD dwErr = 0;

	m_hSCM = ::OpenSCManager(NULL, NULL, SC_MANAGER_CONNECT | SC_MANAGER_ENUMERATE_SERVICE);
	if (m_hSCM == NULL)
		return ::GetLastError();
    
	m_hTransport = ::OpenService(m_hSCM, m_ptchTrServiceName, SERVICE_QUERY_CONFIG);
		
	if (m_hTransport == NULL)
	{
		dwErr = ::GetLastError();
		
		if(ERROR_SERVICE_DOES_NOT_EXIST == dwErr)
			throw HRESULT_FROM_WIN32(dwErr); // serios error, Transport is not properly installed

		return dwErr;
	}
	::CloseServiceHandle(m_hTransport);

	m_hTransport = NULL;
		
	m_hTransport = ::OpenService(m_hSCM, m_ptchTrServiceName, GENERIC_READ);

	DWORD dwTransportAccess = 0;
	if (m_hTransport)
	{	
		dwTransportAccess |= GENERIC_READ;
		::CloseServiceHandle(m_hTransport);
		m_hTransport = NULL;
	}
	else
	{
		dwErr = ::GetLastError();
		if(ERROR_ACCESS_DENIED != dwErr)
			return dwErr;
	}

	m_hTransport = ::OpenService(m_hSCM, m_ptchTrServiceName, GENERIC_EXECUTE);
		
	if (m_hTransport)
	{	
		dwTransportAccess |= GENERIC_EXECUTE;
		::CloseServiceHandle(m_hTransport);
		m_hTransport = NULL;
	}
	else
	{
		dwErr = ::GetLastError();
		if(ERROR_ACCESS_DENIED != dwErr)
			return dwErr;
	}

	m_hTransport = ::OpenService(m_hSCM, m_ptchTrServiceName, dwTransportAccess);
	if(!m_hTransport)
		return ::GetLastError();
    
	DWORD dwRouterAccess = 0;
	m_hRouter = ::OpenService(m_hSCM, m_ptchRtServiceName, GENERIC_READ);
	if (m_hRouter)
	{	
		dwRouterAccess |= GENERIC_READ;
		::CloseServiceHandle(m_hRouter);
		m_hRouter = NULL;
	}    
	m_hRouter = ::OpenService(m_hSCM, m_ptchRtServiceName, GENERIC_EXECUTE);
	if (m_hRouter)
	{	
		dwRouterAccess |= GENERIC_EXECUTE;
		::CloseServiceHandle(m_hRouter);
		m_hRouter = NULL;
	}
    
	m_hRouter = ::OpenService(m_hSCM, m_ptchRtServiceName, SERVICE_CHANGE_CONFIG);
	if (m_hRouter)
	{	
		dwRouterAccess |= SERVICE_CHANGE_CONFIG;
		::CloseServiceHandle(m_hRouter);
		m_hRouter = NULL;
	}

	if(dwRouterAccess)
	{
		m_hRouter = ::OpenService(m_hSCM, m_ptchRtServiceName, dwRouterAccess);
		if(!m_hRouter)
			return ::GetLastError();
	}

	if (m_hTimerCheckService == NULL)
		m_hTimerCheckService.Attach(CreateWaitableTimer(NULL, FALSE, NULL));

	if (m_hTimerCheckService == NULL)
		return ::GetLastError();

	m_hStartTransport.Attach(CreateEvent(NULL, FALSE, FALSE, NULL));
	if (m_hStartTransport == NULL)
		return ::GetLastError();

	m_hStopTransport.Attach(CreateEvent(NULL, FALSE, FALSE, NULL));
	if (m_hStopTransport == NULL)
		return ::GetLastError();

	m_hRestartTransport.Attach(CreateEvent(NULL, FALSE, FALSE, NULL));
	if (m_hRestartTransport == NULL)
		return ::GetLastError();

	m_hStartRouter.Attach(CreateEvent(NULL, FALSE, FALSE, NULL));
	if (m_hStartRouter == NULL)
		return ::GetLastError();

	m_hStopRouter.Attach(CreateEvent(NULL, FALSE, FALSE, NULL));
	if (m_hStopRouter == NULL)
		return ::GetLastError();

	_QueueTimerCheckStatus(DEF_CHECKTIMER);

	m_bIsStarted = true;

	return CThread::Start();
}

DWORD CServiceMonitor::Stop()
{
	CAutoLock	lock(m_Lock);
	
	if(!m_bIsStarted)
		return ERROR_SERVICE_NOT_ACTIVE;
	
	DWORD dwErr = CThread::Stop();
			
	if(ERROR_SUCCESS != dwErr)
		return dwErr;

	m_hTimerCheckService.Close();
	m_hStartTransport.Close();
	m_hStopTransport.Close();
	m_hRestartTransport.Close();
	m_hStartRouter.Close();
	m_hStopRouter.Close();
	if (m_hRouter)
	{
		::CloseServiceHandle(m_hRouter);
		m_hRouter = NULL;
	}
	if (m_hTransport)
	{
		::CloseServiceHandle(m_hTransport);
		m_hTransport = NULL;
	}
	if (m_hSCM)
	{
		::CloseServiceHandle(m_hSCM);
		m_hSCM = NULL;
	}
	m_bIsStarted = false;
	return dwErr;
}

DWORD CServiceMonitor::_StopService( SC_HANDLE hService, DWORD dwTimeout ) 
{
	SERVICE_STATUS ss;
	DWORD dwStartTime = GetTickCount();

   	if(!m_hSCM)
		return ERROR_INVALID_HANDLE;
   
	if(!hService)
		return ERROR_INVALID_HANDLE;

    
	// Make sure the service is not already stopped
	if ( !QueryServiceStatus( hService, &ss ) )
		return GetLastError();

	if ( ss.dwCurrentState == SERVICE_STOPPED ) 
		return ERROR_SUCCESS;

	// If a stop is pending, just wait for it
	
	while ( ss.dwCurrentState == SERVICE_STOP_PENDING ) 
	{	
		if(IsStopping())
			return ERROR_SUCCESS;
        
		DWORD dwWaitTime = ss.dwWaitHint / 10;

        if( dwWaitTime < 1000 )
            dwWaitTime = 1000;
        else if ( dwWaitTime > 10000 )
            dwWaitTime = 10000;

		Sleep(dwWaitTime);
		
		if ( !QueryServiceStatus( hService, &ss ) )
			return GetLastError();

		if ( ss.dwCurrentState == SERVICE_STOPPED )
			return ERROR_SUCCESS;

		if ( GetTickCount() - dwStartTime > dwTimeout )
			return ERROR_TIMEOUT;

	}

	// If the service is running, dependencies must be stopped first
	DWORD i;
	DWORD dwBytesNeeded;
    DWORD dwCount;

    LPENUM_SERVICE_STATUS   lpDependencies = NULL;
    ENUM_SERVICE_STATUS     ess;
    SC_HANDLE               hDepService;

    // Pass a zero-length buffer to get the required buffer size
    if ( EnumDependentServices( hService, SERVICE_ACTIVE, 
         lpDependencies, 0, &dwBytesNeeded, &dwCount ) ) 
	{
		// If the Enum call succeeds, then there are no dependent
        // services so do nothing
    } 
	else 
	{         
		if ( GetLastError() != ERROR_MORE_DATA )
			return GetLastError(); // Unexpected error

		// Allocate a buffer for the dependencies
        lpDependencies = (LPENUM_SERVICE_STATUS) HeapAlloc( 
               GetProcessHeap(), HEAP_ZERO_MEMORY, dwBytesNeeded );

        if ( !lpDependencies )
			return GetLastError();

		// Enumerate the dependencies

        if ( !EnumDependentServices( hService, SERVICE_ACTIVE,         
			lpDependencies, dwBytesNeeded, &dwBytesNeeded, &dwCount ) )
               return GetLastError();

		for ( i = 0; i < dwCount; i++ ) 
		{

			if(IsStopping())
				return ERROR_SUCCESS;

			ess = *(lpDependencies + i);

			// Open the service
			hDepService = OpenService( m_hSCM, ess.lpServiceName, 
				SERVICE_STOP | SERVICE_QUERY_STATUS );
			if ( !hDepService )
				return GetLastError();

			// Send a stop code
			if ( !ControlService( hDepService, SERVICE_CONTROL_STOP,&ss ) )
				return GetLastError();

			// Wait for the service to stop
			while ( ss.dwCurrentState != SERVICE_STOPPED ) 
			{
				if(IsStopping())
				{
					::CloseServiceHandle( hDepService );
					return ERROR_SUCCESS;
				}

				DWORD dwWaitTime = ss.dwWaitHint / 10;

				if( dwWaitTime < 1000 )
					dwWaitTime = 1000;
				else if ( dwWaitTime > 10000 )
					dwWaitTime = 10000;

				Sleep(dwWaitTime);

				if ( !QueryServiceStatus( hDepService, &ss ) )
				{
					DWORD dwErr = GetLastError(); 
						
					::CloseServiceHandle( hDepService );

					return dwErr;
				}

				if ( ss.dwCurrentState == SERVICE_STOPPED )
	                break;

				if ( GetTickCount() - dwStartTime > dwTimeout )
				{
					::CloseServiceHandle( hDepService );

					return ERROR_TIMEOUT;
				}
			} 
			// Always release the service handle
			::CloseServiceHandle( hDepService );
		} 
		// Always free the enumeration buffer
		HeapFree( GetProcessHeap(), 0, lpDependencies );
	} 

	// Send a stop code to the main service
	if ( !ControlService( hService, SERVICE_CONTROL_STOP, &ss ) )
		  return GetLastError();

	// Wait for the service to stop
	while ( ss.dwCurrentState != SERVICE_STOPPED ) 
	{
		if(IsStopping())
			return ERROR_SUCCESS;

		DWORD dwWaitTime = ss.dwWaitHint / 10;

        if( dwWaitTime < 1000 )
            dwWaitTime = 1000;
        else if ( dwWaitTime > 10000 )
            dwWaitTime = 10000;

		Sleep(dwWaitTime);

		if ( !QueryServiceStatus( hService, &ss ) )
			return GetLastError();

		if ( ss.dwCurrentState == SERVICE_STOPPED )
			return ERROR_SUCCESS;

		if ( GetTickCount() - dwStartTime > dwTimeout )
			return ERROR_TIMEOUT;
	}

	// Return success
	return ERROR_SUCCESS;
}

DWORD CServiceMonitor::_StartService( SC_HANDLE hService, DWORD dwTimeout )
{

	SERVICE_STATUS ss;
	DWORD dwStartTime = GetTickCount();

   	if(!m_hSCM)
		return ERROR_INVALID_HANDLE;
   
	if(!hService)
		return ERROR_INVALID_HANDLE;

    
	if ( !QueryServiceStatus( hService, &ss ) )
		return GetLastError();

	if ( ss.dwCurrentState == SERVICE_RUNNING ) 
		return ERROR_SUCCESS;

	while ( ss.dwCurrentState == SERVICE_START_PENDING ) 
	{	
		if(IsStopping())
			return ERROR_SUCCESS;

        
		DWORD dwWaitTime = ss.dwWaitHint / 10;

        if( dwWaitTime < 1000 )
            dwWaitTime = 1000;
        else if ( dwWaitTime > 10000 )
            dwWaitTime = 10000;

		Sleep(dwWaitTime);
		
		if ( !QueryServiceStatus( hService, &ss ) )
			return GetLastError();

		if ( ss.dwCurrentState == SERVICE_RUNNING )
			return ERROR_SUCCESS;

		if ( GetTickCount() - dwStartTime > dwTimeout )
			return ERROR_TIMEOUT;

	}
	
	if(!::StartService(hService,0,NULL))
		return GetLastError();

	while ( ss.dwCurrentState != SERVICE_RUNNING ) 
	{
		if(IsStopping())
			return ERROR_SUCCESS;


		DWORD dwWaitTime = ss.dwWaitHint / 10;

        if( dwWaitTime < 1000 )
            dwWaitTime = 1000;
        else if ( dwWaitTime > 10000 )
            dwWaitTime = 10000;

		Sleep(dwWaitTime);

		if ( !QueryServiceStatus( hService, &ss ) )
			return GetLastError();

		if ( ss.dwCurrentState == SERVICE_RUNNING )
			return ERROR_SUCCESS;

		if ( GetTickCount() - dwStartTime > dwTimeout )
			return ERROR_TIMEOUT;
	}

	// Return success
	return ERROR_SUCCESS;
}


UINT CServiceMonitor::Run()
{
	HANDLE hStopEvent = GetStopEventHandle();

	const HANDLE hEvents[] = {	hStopEvent,
								m_hTimerCheckService,
								m_hStartTransport, 
								m_hStopTransport, 
								m_hRestartTransport,
								m_hStartRouter,
								m_hStopRouter};
	
#if _WIN32_WINNT >= 0x0400
	HRESULT hRes = ::CoInitializeEx(NULL, COINIT_MULTITHREADED);
#else
	HRESULT hRes = ::CoInitialize(NULL);
#endif

	while(true)
	{
		DWORD dwRes = WaitForMultipleObjects(sizeof(hEvents) / sizeof(HANDLE), 
			hEvents, FALSE, INFINITE);

		switch(dwRes)
		{
			case WAIT_OBJECT_0: // Stop event
				CoUninitialize();
				return 0;
			break;
			case WAIT_OBJECT_0 + 1: //Service check
			{
				if (m_hTransport)
				{

					SERVICE_STATUS ss = { 0 };
					if (::QueryServiceStatus(m_hTransport, &ss)) 
					{
						m_dwTransportStatus = ss.dwCurrentState;
							

						CHostProxy* pHost = CHostProxy::GetNextHost(NULL);
						while(pHost)
						{
							pHost->ServiceState(m_dwTransportStatus);
							pHost->Release();
							pHost = CHostProxy::GetNextHost(pHost);
						}

						CRouterProxy* pRouter = CRouterProxy::GetNextRouter(NULL);							
						while(pRouter)
						{
							pRouter->TransportServiceState(m_dwTransportStatus);
							pRouter->Release();
							pRouter = CRouterProxy::GetNextRouter(pRouter);
						}							
					}
				} 
			
				if (m_hRouter) 
				{
					SERVICE_STATUS ss = { 0 };
					if (::QueryServiceStatus(m_hRouter, &ss)) 
					{
						m_dwRouterStatus = ss.dwCurrentState;
							
						CRouterProxy* pRouter = CRouterProxy::GetNextRouter(NULL);
							
						while(pRouter)
						{
							pRouter->ServiceState(m_dwRouterStatus);
							pRouter->Release();
							pRouter = CRouterProxy::GetNextRouter(pRouter);
						}
					}
				}
				_QueueTimerCheckStatus(DEF_CHECKTIMER);
			}
			break;
			case WAIT_OBJECT_0 + 2: //Start Transport
			{
				if(m_hTransport)
				{
					DWORD dwErr = _StartService(m_hTransport,30000);

					if(dwErr != ERROR_SUCCESS)
					{
						CHostProxy* pHost = CHostProxy::GetNextHost(NULL);
						while(pHost)
						{
							pHost->ServiceError(_T("Start Egar Transport"), dwErr);
							pHost->Release();
							pHost = CHostProxy::GetNextHost(pHost);
						}
					}
				}
			}
			break;
			case WAIT_OBJECT_0 + 3: //Stop transport
			{
				if(m_hTransport)
				{
					DWORD dwErr = _StopService(m_hTransport,30000);
					if(dwErr != ERROR_SUCCESS)
					{
						CHostProxy* pHost = CHostProxy::GetNextHost(NULL);
						while(pHost)
						{
							pHost->ServiceError(_T("Stop Egar Transport"), dwErr);
							pHost->Release();
							pHost = CHostProxy::GetNextHost(pHost);
						}
					}
				}
			}
			break;
			case WAIT_OBJECT_0 + 4: //Restart service
			{
				if(m_hTransport)
				{
					DWORD dwRouterStat = m_dwRouterStatus;
					DWORD dwErr = _StopService(m_hTransport,30000);
					if(dwErr != ERROR_SUCCESS)
					{
						CHostProxy* pHost = CHostProxy::GetNextHost(NULL);
						while(pHost)
						{
							pHost->ServiceError(_T("Stop Egar Transport"), dwErr);
							pHost->Release();
							pHost = CHostProxy::GetNextHost(pHost);
						}
						break;
					}
					else
					{
						m_dwTransportStatus = SERVICE_STOPPED;
						CHostProxy* pHost = CHostProxy::GetNextHost(NULL);
						while(pHost)
						{
							pHost->ServiceState(m_dwTransportStatus);
							pHost->Release();
							pHost = CHostProxy::GetNextHost(pHost);
						}

						CRouterProxy* pRouter = CRouterProxy::GetNextRouter(NULL);							
						while(pRouter)
						{
							pRouter->TransportServiceState(m_dwTransportStatus);
							pRouter->Release();
							pRouter = CRouterProxy::GetNextRouter(pRouter);
						}

					}

					dwErr = _StartService(m_hTransport,30000);
					if(dwErr != ERROR_SUCCESS)
					{
						CHostProxy* pHost = CHostProxy::GetNextHost(NULL);
						while(pHost)
						{
							pHost->ServiceError(_T("Start Egar Transport"), dwErr);
							pHost->Release();
							pHost = CHostProxy::GetNextHost(pHost);
						}
						break;
					}
					else
					{
						m_dwTransportStatus = SERVICE_RUNNING;
						CHostProxy* pHost = CHostProxy::GetNextHost(NULL);
						while(pHost)
						{
							pHost->ServiceState(m_dwTransportStatus);
							pHost->Release();
							pHost = CHostProxy::GetNextHost(pHost);
						}

						CRouterProxy* pRouter = CRouterProxy::GetNextRouter(NULL);							
						while(pRouter)
						{
							pRouter->TransportServiceState(m_dwTransportStatus);
							pRouter->Release();
							pRouter = CRouterProxy::GetNextRouter(pRouter);
						}
					}
					
					if(SERVICE_RUNNING == dwRouterStat && m_hRouter)
					{
						dwErr = _StartService(m_hRouter,30000);
						if(dwErr != ERROR_SUCCESS)
						{
							CRouterProxy* pRouter = CRouterProxy::GetNextRouter(NULL);
							
							while(pRouter)
							{
								pRouter->ServiceError(_T("Start Egar Router"), dwErr);
								pRouter->Release();
								pRouter = CRouterProxy::GetNextRouter(pRouter);
							}
							break;
						}
						else
						{
							CRouterProxy* pRouter = CRouterProxy::GetNextRouter(NULL);
							
							while(pRouter)
							{
								m_dwRouterStatus = SERVICE_RUNNING;
								pRouter->ServiceState(m_dwRouterStatus);
								pRouter->Release();
								pRouter = CRouterProxy::GetNextRouter(pRouter);
							}
						}

					}
				}
			}
			break;
			case WAIT_OBJECT_0 + 5: //Start Router
			{
				if(m_hRouter)
				{
					DWORD dwErr = _StartService(m_hRouter,30000);
					if(dwErr != ERROR_SUCCESS)
					{
						CRouterProxy* pRouter = CRouterProxy::GetNextRouter(NULL);
							
						while(pRouter)
						{
							pRouter->ServiceError(_T("Start Egar Router"), dwErr);
							pRouter->Release();
							pRouter = CRouterProxy::GetNextRouter(pRouter);
						}
					}
				}
				else
				{
					CRouterProxy* pRouter = CRouterProxy::GetNextRouter(NULL);
					
					while(pRouter)
					{
						pRouter->ServiceError(_T("Start Egar Router"), ERROR_INVALID_HANDLE);
						pRouter->Release();
						pRouter = CRouterProxy::GetNextRouter(pRouter);
					}
				}
			}
			break;
			case WAIT_OBJECT_0 + 6: //Stop Router
			{
				if(m_hRouter)
				{
					DWORD dwErr = _StopService(m_hRouter,30000);
					if(dwErr != ERROR_SUCCESS)
					{
						CRouterProxy* pRouter = CRouterProxy::GetNextRouter(NULL);
						
						while(pRouter)
						{
							pRouter->ServiceError(_T("Stop Egar Router"), dwErr);
							pRouter->Release();
							pRouter = CRouterProxy::GetNextRouter(pRouter);
						}
					}
				}
				else
				{
					CRouterProxy* pRouter = CRouterProxy::GetNextRouter(NULL);
						
					while(pRouter)
					{
						pRouter->ServiceError(_T("Stop Egar Router"), ERROR_INVALID_HANDLE);
						pRouter->Release();
						pRouter = CRouterProxy::GetNextRouter(pRouter);
					}
				}
			}
			break;
		}
	}

	CoUninitialize();
}

DWORD CServiceMonitor::GetTransportStatus()
{
	if(m_hTransport)
	{
		SERVICE_STATUS ss = { 0 };
		if (::QueryServiceStatus(m_hTransport, &ss)) 
			return ss.dwCurrentState;
	}

	return 0;
}

DWORD CServiceMonitor::GetRouterStatus()
{
	if(m_hRouter)
	{
		SERVICE_STATUS ss = { 0 };
		if (::QueryServiceStatus(m_hRouter, &ss)) 
			return ss.dwCurrentState;
	}
	return 0;
}

DWORD CServiceMonitor::StartTransport()
{
	DWORD dwStatus = GetTransportStatus();
	if( dwStatus == SERVICE_RUNNING )
		return ERROR_SERVICE_ALREADY_RUNNING;
	
	if( dwStatus != SERVICE_STOPPED )
		return ERROR_SERVICE_CANNOT_ACCEPT_CTRL;

	if(!GetThreadHandle())
		return ERROR_INVALID_HANDLE;

	if(!m_hStartTransport)
		return ERROR_INVALID_HANDLE;

	if(!::SetEvent(m_hStartTransport))
		return ::GetLastError();

	return ERROR_SUCCESS;
}
	
DWORD CServiceMonitor::StopTransport()
{
	DWORD dwStatus = GetTransportStatus();
	if( dwStatus == SERVICE_STOPPED )
		return ERROR_SERVICE_NOT_ACTIVE;
	
	if( dwStatus != SERVICE_RUNNING )
		return ERROR_SERVICE_CANNOT_ACCEPT_CTRL;

	if(!GetThreadHandle())
		return ERROR_INVALID_HANDLE;

	if(!m_hStopTransport)
		return ERROR_INVALID_HANDLE;

	if(!::SetEvent(m_hStopTransport))
		return ::GetLastError();

	return ERROR_SUCCESS;
}

DWORD CServiceMonitor::RestartTransport()
{
	DWORD dwStatus = GetTransportStatus();
	if( dwStatus == SERVICE_STOPPED )
		return ERROR_SERVICE_NOT_ACTIVE;
	
	if( dwStatus != SERVICE_RUNNING )
		return ERROR_SERVICE_CANNOT_ACCEPT_CTRL;

	if(!GetThreadHandle())
		return ERROR_INVALID_HANDLE;

	if(!m_hRestartTransport)
		return ERROR_INVALID_HANDLE;

	if(!::SetEvent(m_hRestartTransport))
		return ::GetLastError();

	return ERROR_SUCCESS;
}

DWORD CServiceMonitor::StartRouter()
{
	DWORD dwStatus = GetRouterStatus();
	if( dwStatus == SERVICE_RUNNING )
		return ERROR_SERVICE_ALREADY_RUNNING;
	
	if( dwStatus != SERVICE_STOPPED )
		return ERROR_SERVICE_CANNOT_ACCEPT_CTRL;

	if(!GetThreadHandle())
		return ERROR_INVALID_HANDLE;

	if(!m_hStartRouter)
		return ERROR_INVALID_HANDLE;

	if(!::SetEvent(m_hStartRouter))
		return ::GetLastError();

	return ERROR_SUCCESS;
}
	
DWORD CServiceMonitor::StopRouter()
{
	DWORD dwStatus = GetRouterStatus();
	if( dwStatus == SERVICE_STOPPED )
		return ERROR_SERVICE_NOT_ACTIVE;
	
	if( dwStatus != SERVICE_RUNNING )
		return ERROR_SERVICE_CANNOT_ACCEPT_CTRL;

	if(!GetThreadHandle())
		return ERROR_INVALID_HANDLE;

	if(!m_hStopRouter)
		return ERROR_INVALID_HANDLE;

	if(!::SetEvent(m_hStopRouter))
		return ::GetLastError();

	return ERROR_SUCCESS;
}

bool CServiceMonitor::_QueueTimerCheckStatus(long lTimer)
{
	if(!m_hTimerCheckService)
		return false;
		
	LARGE_INTEGER liDueTime;
	liDueTime.QuadPart = ((ULONGLONG)lTimer) * 1000 * 10000; // 100 ns discontinuity
	liDueTime.QuadPart *= -1;

	if (!SetWaitableTimer(m_hTimerCheckService, &liDueTime, 0, NULL, NULL, 0))
		return false;

	return true;
}

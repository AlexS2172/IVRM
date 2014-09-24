// Transport : Implementation of WinMain


// Note: Proxy/Stub Information
//      To build a separate proxy/stub DLL, 
//      run nmake -f Transportps.mk in the project directory.

#define __EG_TCP_DRIVER_IMPORT__

#include "stdafx.h"
#include <initguid.h>
#include "Transport.h"

#include "Transport_i.c"

#include "Router.h"
#include "Host.h"
#include "Session.h"
#include "HostStatisticData.h"

CServiceModule _Module;

BEGIN_OBJECT_MAP(ObjectMap)
OBJECT_ENTRY(CLSID_Router, CRouter)
OBJECT_ENTRY(CLSID_Host, CHost)
OBJECT_ENTRY(CLSID_Session, CSession)
OBJECT_ENTRY(CLSID_HostStatisticData, CHostStatisticData)
END_OBJECT_MAP()

LPCTSTR FindOneOf(LPCTSTR p1, LPCTSTR p2)
{
    while (p1 != NULL && *p1 != NULL)
    {
        LPCTSTR p = p2;
        while (p != NULL && *p != NULL)
        {
            if (*p1 == *p)
                return CharNext(p1);
            p = CharNext(p);
        }
        p1 = CharNext(p1);
    }
    return NULL;
}

// Although some of these functions are big they are declared inline since they are only used once


inline HRESULT CServiceModule::RegisterServer(BOOL bRegTypeLib, BOOL bService)
{
    HRESULT hr;

    // Remove any previous service since it may point to
    // the incorrect file
	//CRouter::Uninstall();
    Uninstall();

    // Add service entries
    UpdateRegistryFromResource(IDR_Transport, TRUE);

    // Adjust the AppID for Local Server or Service
	CEgRegKey keyAppID;
    LONG lRes = keyAppID.Open(HKEY_CLASSES_ROOT, _T("AppID"), KEY_WRITE);
    if (lRes != ERROR_SUCCESS)
        return lRes;

	CEgRegKey key;
    lRes = key.Open(keyAppID, _T("{3E3E8035-5318-47A7-8E6E-4DB1C16E21C2}"), KEY_WRITE);
    if (lRes != ERROR_SUCCESS)
        return lRes;
    key.DeleteValue(_T("LocalService"));
    
    if (bService)
    {
        key.SetValue(TRANSPORT_SERVICE, _T("LocalService"));
        key.SetValue(_T("-Service"), _T("ServiceParameters"));
        // Create service
        Install();

//		CRouter::Install();
    }

    // Add object entries
    hr = CComModule::RegisterServer(bRegTypeLib);

	::CoUninitialize();

    return hr;
}

inline HRESULT CServiceModule::UnregisterServer()
{
    // Remove service entries
    UpdateRegistryFromResource(IDR_Transport, FALSE);
    // Remove service
//	CRouter::Uninstall();

    Uninstall();
    // Remove object entries
    CComModule::UnregisterServer(TRUE);
	::CoUninitialize();

    return S_OK;
}

inline void CServiceModule::Init(_ATL_OBJMAP_ENTRY* p, HINSTANCE h, UINT nServiceNameID, const GUID* plibid)
{
    CComModule::Init(p, h, plibid);

    m_bService = TRUE;

    // set up the initial service status 
    m_hServiceStatus = NULL;
    m_status.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
    m_status.dwCurrentState = SERVICE_STOPPED;
    m_status.dwControlsAccepted = SERVICE_ACCEPT_STOP;
    m_status.dwWin32ExitCode = 0;
    m_status.dwServiceSpecificExitCode = 0;
    m_status.dwCheckPoint = 0;
    m_status.dwWaitHint = 0;
}

LONG CServiceModule::Unlock()
{
    LONG l = CComModule::Unlock();
    if (l == 0 && !m_bService)
        PostThreadMessage(dwThreadID, WM_QUIT, 0, 0);
    return l;
}

LONG CServiceModule::Lock()
{
    LONG l = CComModule::Lock();
    return l;
}

BOOL CServiceModule::IsInstalled()
{
    BOOL bResult = FALSE;

    SC_HANDLE hSCM = ::OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);

    if (hSCM != NULL)
    {
        SC_HANDLE hService = ::OpenService(hSCM, TRANSPORT_SERVICE, SERVICE_QUERY_CONFIG);
        if (hService != NULL)
        {
            bResult = TRUE;
            ::CloseServiceHandle(hService);
        }
        ::CloseServiceHandle(hSCM);
    }
    return bResult;
}

inline BOOL CServiceModule::Install()
{
    if (IsInstalled())
        return TRUE;

    SC_HANDLE hSCM = ::OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
    if (hSCM == NULL)
    {
        MessageBox(NULL, _T("Couldn't open service manager"), TRANSPORT_SERVICE, MB_OK);
        return FALSE;
    }

    // Get the executable file path
    TCHAR szFilePath[_MAX_PATH];
    ::GetModuleFileName(NULL, szFilePath, _MAX_PATH);

    SC_HANDLE hService = ::CreateService(
        hSCM, TRANSPORT_SERVICE, TRANSPORT_SERVICE,
        SERVICE_ALL_ACCESS, SERVICE_WIN32_SHARE_PROCESS,
        SERVICE_DEMAND_START, SERVICE_ERROR_NORMAL,
        szFilePath, NULL, NULL, TRANSPORT_SERVICE_DEPEND, NULL, NULL);

    if (hService == NULL)
    {
        ::CloseServiceHandle(hSCM);
        MessageBox(NULL, _T("Couldn't create service"), TRANSPORT_SERVICE, MB_OK);
        return FALSE;
    }
	else
	{

		HMODULE hAdvApi = ::LoadLibrary(_T("Advapi32"));
		LPFN_ChangeServiceConfig2 pfnChangeServiceConfig2 = NULL; 
		if(hAdvApi)
		{
			pfnChangeServiceConfig2 = (LPFN_ChangeServiceConfig2)GetProcAddress(hAdvApi,ChangeServiceConfig2Name);
		
			if(pfnChangeServiceConfig2)
			{
				SERVICE_DESCRIPTION sd = { (PTSTR) TRANSPORT_SERVICE_DESC };
				pfnChangeServiceConfig2(hService, SERVICE_CONFIG_DESCRIPTION, &sd);
			}

			::FreeLibrary(hAdvApi);
		}
	}
    ::CloseServiceHandle(hService);
    ::CloseServiceHandle(hSCM);
    return TRUE;
}

inline BOOL CServiceModule::Uninstall()
{
    if (!IsInstalled())
        return TRUE;

    SC_HANDLE hSCM = ::OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);

    if (hSCM == NULL)
    {
        MessageBox(NULL, _T("Couldn't open service manager"), TRANSPORT_SERVICE, MB_OK);
        return FALSE;
    }

    SC_HANDLE hService = ::OpenService(hSCM, TRANSPORT_SERVICE, SERVICE_STOP | DELETE);

    if (hService == NULL)
    {
        ::CloseServiceHandle(hSCM);
        MessageBox(NULL, _T("Couldn't open service"), TRANSPORT_SERVICE, MB_OK);
        return FALSE;
    }
    SERVICE_STATUS status;
    ::ControlService(hService, SERVICE_CONTROL_STOP, &status);

    BOOL bDelete = ::DeleteService(hService);

	if(!bDelete)
	{
		DWORD dwErr = GetLastError();
		if(ERROR_SERVICE_MARKED_FOR_DELETE == dwErr)
			bDelete = TRUE;
	}
    ::CloseServiceHandle(hService);
    ::CloseServiceHandle(hSCM);

    if(bDelete)
        return TRUE;

    MessageBox(NULL, _T("Service could not be deleted"), TRANSPORT_SERVICE, MB_OK);
    return FALSE;
}

//////////////////////////////////////////////////////////////////////////////////////////////
// Service startup and registration
inline void CServiceModule::Start()
{
	if(m_bService)
	{
		SERVICE_TABLE_ENTRY st[] =
		{
			{ TRANSPORT_SERVICE, _ServiceMain },
			{ NULL, NULL }
		};
		if (m_bService && !::StartServiceCtrlDispatcher(st))
		{
			m_bService = FALSE;
		}
		if (m_bService == FALSE)
		{
			m_status.dwWin32ExitCode = 	GetLastError();
		}	
	}
	else
	{
		Run();
	}
}

inline void CServiceModule::ServiceMain(DWORD /* dwArgc */, LPTSTR* /* lpszArgv */)
{
    // Register the control request handler
    m_status.dwCurrentState = SERVICE_START_PENDING;
    m_hServiceStatus = RegisterServiceCtrlHandler(TRANSPORT_SERVICE, _Handler);
    if (m_hServiceStatus == NULL)
        return;
    SetServiceStatus(SERVICE_START_PENDING);

    m_status.dwWin32ExitCode = S_OK;
    m_status.dwCheckPoint = 0;
    m_status.dwWaitHint = 0;

#if _WIN32_WINNT >= 0x0400 & defined(_ATL_FREE_THREADED)
    HRESULT hRes = CoInitializeEx(NULL, COINIT_MULTITHREADED);
#else
    HRESULT hRes = CoInitialize(NULL);
#endif

    _ASSERTE(SUCCEEDED(hRes));
	
    // When the Run function returns, the service has stopped.
    Run();

    SetServiceStatus(SERVICE_STOPPED);

    CoUninitialize();
}

inline void CServiceModule::Handler(DWORD dwOpcode)
{
    switch (dwOpcode)
    {
    case SERVICE_CONTROL_STOP:
		
		g_pHost->SetIsStopping();

        SetServiceStatus(SERVICE_STOP_PENDING);
        PostThreadMessage(dwThreadID, WM_QUIT, 0, 0);
        break;
    case SERVICE_CONTROL_PAUSE:
        break;
    case SERVICE_CONTROL_CONTINUE:
        break;
    case SERVICE_CONTROL_INTERROGATE:
        break;
    case SERVICE_CONTROL_SHUTDOWN:
        break;
    default:

		CEgLibTraceManager::Trace(LogError,_T("Transport Service Handler"),_T("Bad service request"));
    }
}

void WINAPI CServiceModule::_ServiceMain(DWORD dwArgc, LPTSTR* lpszArgv)
{	
    _Module.ServiceMain(dwArgc, lpszArgv);
}
void WINAPI CServiceModule::_Handler(DWORD dwOpcode)
{
    _Module.Handler(dwOpcode); 
}

void CServiceModule::SetServiceStatus(DWORD dwState)
{
	if(m_bService)
	{
		m_status.dwCurrentState = dwState;
		::SetServiceStatus(m_hServiceStatus, &m_status);
	}
}

_COM_SMARTPTR_TYPEDEF(IHost, __uuidof(IHost));
_COM_SMARTPTR_TYPEDEF(IRouter, __uuidof(IRouter));

void CServiceModule::Run()
{  
    _Module.dwThreadID = GetCurrentThreadId();

	
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	CEgTcpDriver::Init();
	
	if(!CEgLibTraceManager::g_spTrace)
	{
		CEgTpTracePtr spTrace = CEgTpTracePtr(new CEgTpTrace());
		if(spTrace)
		{
			spTrace->ReadTraceSettings();
			CEgLibTraceManager::g_spTrace = boost::shared_static_cast<CEgLibTraceManager>(spTrace);
		}
	}
	
	
//  If you are running on NT 4.0 or higher you can use the following call
//  instead to make the EXE free threaded.
//  This means that calls come in on a random RPC thread
//  HRESULT hr = CoInitializeEx(NULL, COINIT_MULTITHREADED);

	HRESULT hRes;

	if(m_bService)
	{
		// This provides a NULL DACL which will allow access to everyone.
		CSecurityDescriptor sd;
		hRes = sd.InitializeFromThreadToken();
		if(FAILED(hRes))
		{
			Trace(1,_T("Transport Service Run"),_T("InitializeFromThreadToken error %X"),hRes);
		}		


		hRes = CoInitializeSecurity(sd, -1, NULL, NULL,
				RPC_C_AUTHN_LEVEL_PKT, RPC_C_IMP_LEVEL_IMPERSONATE, NULL, EOAC_NONE, NULL);

		if(FAILED(hRes))
		{
			Trace(1,_T("Transport Service Run"),_T("CoInitializeSecurity error %X"),hRes);
		}		
		_ASSERTE(SUCCEEDED(hRes));

		hRes = _Module.RegisterClassObjects(CLSCTX_LOCAL_SERVER | CLSCTX_REMOTE_SERVER, REGCLS_MULTIPLEUSE | REGCLS_SUSPENDED);

		if(FAILED(hRes))
		{
			Trace(1,_T("Transport Service Run"),_T("RegisterClassObjects error %X"),hRes);
		}		
		
		_ASSERTE(SUCCEEDED(hRes));	

		hRes = CoResumeClassObjects();
		if(FAILED(hRes))
		{
			Trace(1,_T("Transport Service Run"),_T("CoResumeClassObjects error %X"),hRes);
		}		

	}
	else
	{

#if _WIN32_WINNT >= 0x0400 & defined(_ATL_FREE_THREADED)
        hRes = _Module.RegisterClassObjects(CLSCTX_LOCAL_SERVER, REGCLS_MULTIPLEUSE | REGCLS_SUSPENDED);
        _ASSERTE(SUCCEEDED(hRes));
        hRes = CoResumeClassObjects();
#else
        hRes = _Module.RegisterClassObjects(CLSCTX_LOCAL_SERVER, REGCLS_MULTIPLEUSE);
        _ASSERTE(SUCCEEDED(hRes));
#endif

	}
	
	DoSettings();
	
	//EgAssert(g_pHost);

	//EgAssert(g_pRouter);

    if (m_bService)
        SetServiceStatus(SERVICE_RUNNING);

	CEgLibTraceManager::Trace(LogError,_T("Transport Service Run"),_T("Transport Service started"));

	m_isRunning = TRUE;	

	
    MSG msg;
    while (GetMessage(&msg, 0, 0, 0))
        DispatchMessage(&msg);
	
	m_isRunning = FALSE;

    _Module.RevokeClassObjects();

    CEgLibTraceManager::Trace(LogError,_T("Transport Service Run"),_T("Transport Service stopped"));

	if(CEgLibTraceManager::g_spTrace!=NULL)
		CEgLibTraceManager::g_spTrace.reset();

    if (m_bService)
        SetServiceStatus(SERVICE_STOPPED);

	CEgTcpDriver::Exit();
}

HRESULT CServiceModule::DoSettings()
{
	return S_OK;
}
void CServiceModule::Trace(long lLogLevel, const TCHAR* lpszCategory, const TCHAR* lpszMessage, ...)
{
	if(!lpszMessage || CEgLibTraceManager::g_spTrace == NULL)
		return;
	
	va_list arglist;
    va_start(arglist, lpszMessage);
	CEgLibTraceManager::g_spTrace->LogInfo2(lLogLevel, lpszCategory, lpszMessage, arglist);
	va_end(arglist);
}

/////////////////////////////////////////////////////////////////////////////
// 
extern "C" int WINAPI _tWinMain(HINSTANCE hInstance, 
    HINSTANCE /*hPrevInstance*/, LPTSTR lpCmdLine, int /*nShowCmd*/)
{
#if _WIN32_WINNT >= 0x0400 & defined(_ATL_FREE_THREADED)
    HRESULT hr = CoInitializeEx(NULL, COINIT_MULTITHREADED);
#else
    HRESULT hr = CoInitialize(NULL);
#endif
	
//	SetMiniDumpDefaultCrashHandler();

    _ASSERTE(SUCCEEDED(hr));

    lpCmdLine = GetCommandLine(); //this line necessary for _ATL_MIN_CRT
    _Module.m_bService = FALSE;

    TCHAR szTokens[] = _T("-/");

    LPCTSTR lpszToken = FindOneOf(lpCmdLine, szTokens);

    while (lpszToken != NULL)
    {
        if (lstrcmpi(lpszToken, _T("Install"))==0)
        {

			_Module.Init(ObjectMap, hInstance, IDS_SERVICENAME, &LIBID_TRANSPORTLib);
			_Module.m_bService = TRUE;
//			CRouter::Install();
            return _Module.Install();
        }
        if (lstrcmpi(lpszToken, _T("Uninstall"))==0)
        {
			_Module.Init(ObjectMap, hInstance, IDS_SERVICENAME, &LIBID_TRANSPORTLib);
			_Module.m_bService = TRUE;
//			CRouter::Uninstall();
            return _Module.Uninstall();
        }
        if (lstrcmpi(lpszToken, _T("UnregServer"))==0)
        {
			_Module.Init(ObjectMap, hInstance, IDS_SERVICENAME, &LIBID_TRANSPORTLib);
			_Module.m_bService = TRUE;
            _Module.UpdateRegistryFromResource(IDR_Transport, FALSE);
            return _Module.UnregisterServer();
        }
        if (lstrcmpi(lpszToken, _T("RegServer"))==0)
        {

			_Module.Init(ObjectMap, hInstance, IDS_SERVICENAME, &LIBID_TRANSPORTLib);
			_Module.m_bService = TRUE;
            _Module.UpdateRegistryFromResource(IDR_Transport, TRUE);
            return _Module.RegisterServer(TRUE,TRUE);
        }
        if (lstrcmpi(lpszToken, _T("Debug"))==0)
        {
			_Module.CComModule::Init(ObjectMap, hInstance, &LIBID_TRANSPORTLib);

		    _Module.Start();
			_Module.Term();
	
			::CoUninitialize();

			return 0;

        }
        lpszToken = FindOneOf(lpszToken, szTokens);
    }

	_Module.Init(ObjectMap, hInstance, IDS_SERVICENAME, &LIBID_TRANSPORTLib);
    _Module.m_bService = TRUE;

    _Module.Start();
	_Module.Term();
	
	::CoUninitialize();

    // When we get here, the service has been stopped
    return _Module.m_status.dwWin32ExitCode;
}




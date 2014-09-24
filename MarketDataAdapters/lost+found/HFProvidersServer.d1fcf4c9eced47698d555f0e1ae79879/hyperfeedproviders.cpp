// HyperFeedProviders.cpp : Implementation of WinMain


// Note: Proxy/Stub Information
//      To build a separate proxy/stub DLL,
//      run nmake -f HyperFeedProvidersps.mk in the project directory.

#include "stdafx.h"
#include "resource.h"
#include <initguid.h>
#include "HyperFeedProviders.h"

#include "HyperFeedProviders_i.c"
#include "HyperFeedStructureProvider.h"
#include "HyperFeedStructureProviderEx.h"
#include "HyperFeedStructureInfo.h"
#include "HyperFeedPriceProvider.h"
#include "HyperFeedPriceInfo.h"
#include "ProviderInfo.h"
#include "HyperFeedPriceInfoWithNotify.h"
#include "HyperFeedMarketDataForIv.h"
#include "EgDebugUtil.h"

const DWORD dwTimeOut = 5000; // time for EXE to be idle before shutting down
const DWORD dwPause = 1000; // time to wait for threads to finish up

// Passed to CreateThread to monitor the shutdown event
static DWORD WINAPI MonitorProc(void* pv)
{
    CServiceModule* p = (CServiceModule*)pv;
    p->MonitorShutdown();
    return 0;
}


//Monitors the shutdown event
void CServiceModule::MonitorShutdown()
{
    while (1)
    {
        WaitForSingleObject(hEventShutdown, INFINITE);
        DWORD dwWait=0;
        do
        {
            bActivity = false;
            dwWait = WaitForSingleObject(hEventShutdown, dwTimeOut);
        } while (dwWait == WAIT_OBJECT_0);
        // timed out
        if (!bActivity && m_nLockCnt == 0) // if no activity let's really bail
        {
#if _WIN32_WINNT >= 0x0400 & defined(_ATL_FREE_THREADED)
            CoSuspendClassObjects();
            if (!bActivity && m_nLockCnt == 0)
#endif
                break;
        }
    }
    CloseHandle(hEventShutdown);
    PostThreadMessage(dwThreadID, WM_QUIT, 0, 0);
}

/*

inline HRESULT CServiceModule::UnregisterServer()
{
	HRESULT hr = CoInitialize(NULL);
    if (FAILED(hr))
        return hr;

    // Remove service entries
    UpdateRegistryFromResource(IDR_EgarDataFeedService, FALSE);
    // Remove service
    Uninstall();
    // Remove object entries

	CComPtr<ICatRegister> spcr;
	hr = spcr.CoCreateInstance(CLSID_StdComponentCategoriesMgr, NULL, CLSCTX_INPROC);
	if(SUCCEEDED(hr) && spcr!=NULL)
	{
			CATID catid = CATID_PriceProviders;
			CComBSTR bs;
			ProgIDFromCLSID(CLSID_ProviderInfo, &bs);
			spcr->UnRegisterClassImplCategories( CLSID_ProviderInfo, 1, &catid);
	}

	CComModule::UnregisterServer(TRUE);

    CoUninitialize();
    return S_OK;
}


inline void CServiceModule::Init(_ATL_OBJMAP_ENTRY* p, HINSTANCE h, UINT nServiceNameID, const GUID* plibid)
{
    _Module.Init(p, h, plibid);

    m_bService = TRUE;

    LoadString(h, nServiceNameID, m_szServiceName, sizeof(m_szServiceName) / sizeof(TCHAR));

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
	LONG l = CComModule.Unlock();
    if (l == 0 && !m_bService)
        PostThreadMessage(dwThreadID, WM_QUIT, 0, 0);
    return l;
}
*/
BOOL CServiceModule::IsInstalled()
{
    BOOL bResult = FALSE;

    SC_HANDLE hSCM = ::OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);

    if (hSCM != NULL)
    {
        SC_HANDLE hService = ::OpenService(hSCM, m_szServiceName, SERVICE_QUERY_CONFIG);
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
        MessageBox(NULL, _T("Couldn't open service manager"), m_szServiceName, MB_OK);
        return FALSE;
    }

    // Get the executable file path
    TCHAR szFilePath[_MAX_PATH];
    ::GetModuleFileName(NULL, szFilePath, _MAX_PATH);

	SC_HANDLE hService = ::CreateService(
        hSCM, m_szServiceName, m_szServiceName,
        SERVICE_ALL_ACCESS, SERVICE_WIN32_OWN_PROCESS,
        SERVICE_DEMAND_START, SERVICE_ERROR_NORMAL,
        szFilePath, NULL, NULL, _T("RPCSS\0"), NULL, NULL);

    if (hService == NULL)
    {
        ::CloseServiceHandle(hSCM);
        MessageBox(NULL, _T("Couldn't create service"), m_szServiceName, MB_OK);
        return FALSE;
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
        MessageBox(NULL, _T("Couldn't open service manager"), m_szServiceName, MB_OK);
        return FALSE;
    }

    SC_HANDLE hService = ::OpenService(hSCM, m_szServiceName, SERVICE_STOP | DELETE);

    if (hService == NULL)
    {
        ::CloseServiceHandle(hSCM);
        MessageBox(NULL, _T("Couldn't open service"), m_szServiceName, MB_OK);
        return FALSE;
    }
    SERVICE_STATUS status;
    ::ControlService(hService, SERVICE_CONTROL_STOP, &status);

    BOOL bDelete = ::DeleteService(hService);
    ::CloseServiceHandle(hService);
    ::CloseServiceHandle(hSCM);

    if (bDelete)
        return TRUE;

    MessageBox(NULL, _T("Service could not be deleted"), m_szServiceName, MB_OK);
    return FALSE;
}

///////////////////////////////////////////////////////////////////////////////////////
// Logging functions
void CServiceModule::LogEvent(LPCTSTR pFormat, ...)
{
    TCHAR    chMsg[256];
    HANDLE  hEventSource;
    LPTSTR  lpszStrings[1];
    va_list pArg;

    va_start(pArg, pFormat);
    _vstprintf(chMsg, pFormat, pArg);
    va_end(pArg);

    lpszStrings[0] = chMsg;

    if (m_bService)
    {
        /* Get a handle to use with ReportEvent(). */
        hEventSource = RegisterEventSource(NULL, m_szServiceName);
        if (hEventSource != NULL)
        {
            /* Write to event log. */
            ReportEvent(hEventSource, EVENTLOG_INFORMATION_TYPE, 0, 0, NULL, 1, 0, (LPCTSTR*) &lpszStrings[0], NULL);
            DeregisterEventSource(hEventSource);
        }
    }
    else
    {
        // As we are not running as a service, just write the error to the console.
        _putts(chMsg);
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////
// Service startup and registration
inline void CServiceModule::Start()
{
    SERVICE_TABLE_ENTRY st[] =
    {
        { m_szServiceName, _ServiceMain },
        { NULL, NULL }
    };
    if (m_bService && !::StartServiceCtrlDispatcher(st))
    {
        m_bService = FALSE;
    }
    if (m_bService == FALSE)
        Run();
}

HRESULT CServiceModule::PreMessageLoop(  int nShowCmd )
{
	HRESULT hr = __super::PreMessageLoop(nShowCmd);
	#if _ATL_VER == 0x0700
		if (SUCCEEDED(hr) && (!m_bDelayShutdown))
			hr = CoResumeClassObjects();
	#endif

	return hr;
}

HRESULT CServiceModule::PostMessageLoop( )
{
	//
	return __super::PostMessageLoop();
}

inline void CServiceModule::ServiceMain(DWORD /* dwArgc */, LPTSTR* /* lpszArgv */)
{
    // Register the control request handler
    m_status.dwCurrentState = SERVICE_START_PENDING;
    m_hServiceStatus = RegisterServiceCtrlHandler(m_szServiceName, _Handler);
    if (m_hServiceStatus == NULL)
    {
        LogEvent(_T("Handler not installed"));
        return;
    }
    SetServiceStatus(SERVICE_START_PENDING);

    m_status.dwWin32ExitCode = S_OK;
    m_status.dwCheckPoint = 0;
    m_status.dwWaitHint = 0;

    // When the Run function returns, the service has stopped.
    Run();

    SetServiceStatus(SERVICE_STOPPED);
    LogEvent(_T("Service stopped"));
}

inline void CServiceModule::Handler(DWORD dwOpcode)
{
    switch (dwOpcode)
    {
    case SERVICE_CONTROL_STOP:
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
        LogEvent(_T("Bad service request"));
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
    m_status.dwCurrentState = dwState;
    ::SetServiceStatus(m_hServiceStatus, &m_status);
}

void CServiceModule::Run()
{
	
	_Module.dwThreadID = GetCurrentThreadId();
	HRESULT hr = CoInitialize(NULL);


	_Module.StartMonitor();

    // This provides a NULL DACL which will allow access to everyone.
    CSecurityDescriptor sd;
    sd.InitializeFromThreadToken();

	hr = CoInitializeSecurity(sd, -1, NULL, NULL,
		RPC_C_AUTHN_LEVEL_PKT , RPC_C_IMP_LEVEL_IMPERSONATE, NULL, EOAC_NONE, NULL);


	_ASSERTE(SUCCEEDED(hr));
	#ifndef _DEBUG 
	SetMiniDumpDefaultCrashHandler();
	#endif
	//ATLVERIFY(SUCCEEDED(_Module.LoadExchangeCodes()));

	_Module.StartMonitor();

//	#if _WIN32_WINNT >= 0x0400 & defined(_ATL_FREE_THREADED)
//	hr = _Module.RegisterClassObjects(CLSCTX_LOCAL_SERVER , REGCLS_MULTIPLEUSE | REGCLS_SUSPENDED);
//	EgStd::CEgTracingClass::TraceStatic(enlogFaults, __FUNCTION__, _T("_Module.RegisterClassObjects")); 
//	#else
	hr = _Module.RegisterClassObjects(CLSCTX_LOCAL_SERVER , REGCLS_MULTIPLEUSE | REGCLS_SUSPENDED);
	EgStd::CEgTracingClass::TraceStatic(enlogFaults, __FUNCTION__, _T("_Module.RegisterClassObjects %d"), hr); 
	_ASSERTE(SUCCEEDED(hr));	
	hr = CoResumeClassObjects();
	EgStd::CEgTracingClass::TraceStatic(enlogFaults, __FUNCTION__, _T("_Module.CoResumeClassObjects %d"), hr); 
	_ASSERTE(SUCCEEDED(hr));	
//	#endif

	EgStd::CEgTracingClass::TraceStatic(enlogFaults, __FUNCTION__, _T("Service Run Attempt") ); 


	_ASSERTE(SUCCEEDED(hr));
	if FAILED(hr)
	{
		EgStd::CEgTracingClass::TraceStatic(enlogFaults, __FUNCTION__, _T("Can't register class object!") );
	}
    
	_ASSERTE(SUCCEEDED(hr));

	LogEvent(_T("Service started"));
    if (m_bService)
        SetServiceStatus(SERVICE_RUNNING);


	EgStd::CEgTracingClass::TraceStatic(enlogFaults, __FUNCTION__, _T("Service Run") );

	MSG msg;
    while (GetMessage(&msg, 0, 0, 0))
        DispatchMessage(&msg);

	EgStd::CEgTracingClass::TraceStatic(enlogFaults, __FUNCTION__, _T("Service Stop") );
	// Exit the tracing
	EgStd::g_pTrace->Exit();

	_Module.RevokeClassObjects();

    Sleep(dwPause); //wait for any threads to finish ?

    CoUninitialize();

}

/*
//--------------------------------------------------------------------------------------------------//
inline HRESULT CServiceModule::RegisterServer(BOOL bRegTypeLib, BOOL bService)
{

	HRESULT hr = CoInitialize(NULL);

    if (FAILED(hr))
        return hr;

    // Remove any previous service since it may point to
    // the incorrect file
    Uninstall();

    // Add service entries
    UpdateRegistryFromResource(IDR_EgarDataFeedService, TRUE);

    // Adjust the AppID for Local Server or Service
    CRegKey keyAppID;
    LONG lRes = keyAppID.Open(HKEY_CLASSES_ROOT, _T("AppID"), KEY_WRITE);
    if (lRes != ERROR_SUCCESS)
        return lRes;

    CRegKey key;
	lRes = key.Open(keyAppID, _T("{1F999AB9-E140-4005-9180-958D72670A36}"), KEY_WRITE);
    if (lRes != ERROR_SUCCESS)
        return lRes;
    key.DeleteValue(_T("LocalService"));
    
    if (bService)
    {
        key.SetStringValue(_T("EgarDataFeedService"), _T("LocalService"));
        key.SetStringValue(_T("-Service"), _T("ServiceParameters"));
        // Create service
        Install();
    }
	
	hr = RegisterServer(bRegTypeLib);

	if(SUCCEEDED(hr))
	{
		CComPtr<ICatRegister> spcr;
		hr = spcr.CoCreateInstance(CLSID_StdComponentCategoriesMgr, NULL, CLSCTX_INPROC);
		
		if(SUCCEEDED(hr) && spcr!=NULL)
		{
			CATEGORYINFO catinfo;
			catinfo.catid = CATID_PriceProviders;
			catinfo.lcid = 0x0409;  // locale=english

			USES_CONVERSION;        // uses A2W
			wcscpy(catinfo.szDescription,  A2W("EGAR Price Providers"));
			hr = spcr->RegisterCategories(1, &catinfo);
			if(SUCCEEDED(hr))
			{
				CATID catid = CATID_PriceProviders;
				hr = spcr->RegisterClassImplCategories( CLSID_ProviderInfo, 1, &catid);
				if(SUCCEEDED(hr))
				{
					CATID catid = CATID_PriceProviders;
					LPOLESTR pDaya ;

					StringFromCLSID(CLSID_ProviderInfo, &pDaya);

					hr = spcr->RegisterClassImplCategories( CLSID_ProviderInfo, 1, &catid);
					if(SUCCEEDED(hr))
					{
						_bstr_t str(_T("SOFTWARE\\Egar\\Providers\\"));
						str += pDaya;
						EgLib::CEgRegKey reg;
						if(ERROR_SUCCESS==reg.Create(HKEY_LOCAL_MACHINE,(LPCTSTR)str))
						{
							reg.SetValue(DEF_PROVIDER_ID, _T(""));
							reg.SetValue(W2T(DEF_PROVIDER_NAME), _T("Name"));
						}
					}
					::CoTaskMemFree(pDaya);
				}
			}
		}
	}

    

	CoUninitialize();
	return hr;
}
*/

/*
HRESULT CServiceModule::UnregisterServer(BOOL bUnRegTypeLib, const CLSID* pCLSID)
{
	CComPtr<ICatRegister> spcr;
	HRESULT hr = spcr.CoCreateInstance(CLSID_StdComponentCategoriesMgr, NULL, CLSCTX_INPROC);
	if(SUCCEEDED(hr) && spcr!=NULL)
	{
			CATID catid = CATID_PriceProviders;
			CComBSTR bs;
			ProgIDFromCLSID(CLSID_ProviderInfo, &bs);
			spcr->UnRegisterClassImplCategories( CLSID_ProviderInfo, 1, &catid);
	}
	CServiceModule::UnregisterServer();
	hr = CComModule::UnregisterServer(bUnRegTypeLib, pCLSID);
	return hr;
}
*/

bool CServiceModule::StartMonitor()
{
    hEventShutdown = CreateEvent(NULL, false, false, NULL);
    if (hEventShutdown == NULL)
        return false;
    DWORD dwThreadID;
    HANDLE h = CreateThread(NULL, 0, MonitorProc, this, 0, &dwThreadID);
    return (h != NULL);
}

CServiceModule _Module;

BEGIN_OBJECT_MAP(ObjectMap)
OBJECT_ENTRY(CLSID_HyperFeedStructureProvider, CHyperFeedStructureProvider)
OBJECT_ENTRY(CLSID_HyperFeedStructureProviderEx, CHyperFeedStructureProviderEx)
OBJECT_ENTRY(CLSID_HyperFeedStructureInfo, CHyperFeedStructureInfo)
OBJECT_ENTRY(CLSID_HyperFeedPriceProvider, CHyperFeedPriceProvider)
OBJECT_ENTRY(CLSID_HyperFeedPriceInfo, CHyperFeedPriceInfo)
OBJECT_ENTRY(CLSID_ProviderInfo, CProviderInfo)
OBJECT_ENTRY(CLSID_HyperFeedPriceInfoWithNotify, CHyperFeedPriceInfoWithNotify)
OBJECT_ENTRY(CLSID_HyperFeedMarketDataForIv, CHyperFeedMarketDataForIv)
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

/////////////////////////////////////////////////////////////////////////////
//
extern "C" int WINAPI _tWinMain(HINSTANCE hInstance,
    HINSTANCE /*hPrevInstance*/, LPTSTR lpCmdLine, int /*nShowCmd*/)
{
	EgStd::g_pTrace = new CHyperFeedTrace();
	EgStd::g_pTrace->Init();
	EgStd::CEgTracingClass::TraceStatic(enlogFaults, __FUNCTION__, _T("Enter") ); 
	
	///////////////
	////Service

   lpCmdLine = GetCommandLine(); //this line necessary for _ATL_MIN_CRT

   HRESULT hr = CoInitialize(NULL);
   //HRESULT hr = ::CoInitializeEx(NULL, COINIT_MULTITHREADED);



    _ASSERTE(SUCCEEDED(hr));
    CComModule.Init(ObjectMap, hInstance, IDR_EgarDataFeedService, &LIBID_HYPERFEEDPROVIDERSLib);
    
    //_Module.dwThreadID = GetCurrentThreadId();

	_Module.m_bService = TRUE;

    TCHAR szTokens[] = _T("-/");

    LPCTSTR lpszToken = FindOneOf(lpCmdLine, szTokens);
    while (lpszToken != NULL)
    {
        if (lstrcmpi(lpszToken, _T("UnregServer"))==0)
		{
			EgStd::CEgTracingClass::TraceStatic(enlogDebug, __FUNCTION__, _T("UnRegister")); 
			hr = CComModule.UnregisterServer();
		    EgStd::CEgTracingClass::TraceStatic(enlogDebug, __FUNCTION__, _T("Unregistered %d"), hr ); 
			return hr;
		}

        // Register as Local Server
        if (lstrcmpi(lpszToken, _T("RegServer"))==0)
		{
			EgStd::CEgTracingClass::TraceStatic(enlogDebug, __FUNCTION__, _T("Register")); 
			hr = CComModule.RegisterServer(TRUE, FALSE);
		    EgStd::CEgTracingClass::TraceStatic(enlogDebug, __FUNCTION__, _T("Registered %d"), hr ); 
			return hr;
		}
        
        
        // Register as Service
        if (lstrcmpi(lpszToken, _T("Service"))==0)
		{
			EgStd::CEgTracingClass::TraceStatic(enlogDebug, __FUNCTION__, _T("Register as Service")); 
			HRESULT hr = CServiceModule::RegisterServer(TRUE, TRUE);
			EgStd::CEgTracingClass::TraceStatic(enlogDebug, __FUNCTION__, _T("Registered as Service %d"), hr ); 
			return hr;
		}

        lpszToken = FindOneOf(lpszToken, szTokens);
    }

    // Are we Service or Local Server
    CRegKey keyAppID;
    LONG lRes = keyAppID.Open(HKEY_CLASSES_ROOT, _T("AppID"), KEY_READ);
    if (lRes != ERROR_SUCCESS)
        return lRes;

    CRegKey key;
    lRes = key.Open(keyAppID, _T("{1F999AB9-E140-4005-9180-958D72670A36}"), KEY_READ);
    if (lRes != ERROR_SUCCESS)
        return lRes;

    TCHAR szValue[_MAX_PATH];
    DWORD dwLen = _MAX_PATH;
    lRes = key.QueryStringValue(szValue, _T("LocalService"), &dwLen);

    _Module.m_bService = FALSE;
    if (lRes == ERROR_SUCCESS)
        _Module.m_bService = TRUE;
	EgStd::CEgTracingClass::TraceStatic(enlogFaults, __FUNCTION__, _T("Start Attempt") ); 
    _Module.Start();

    // When we get here, the service has been stopped
	EgStd::CEgTracingClass::TraceStatic(enlogFaults, __FUNCTION__, _T("Exit") ); 

	if (EgStd::g_pTrace != NULL)
		delete EgStd::g_pTrace;

    return _Module.m_status.dwWin32ExitCode;
}

/////////////////////////////////////////////////////////////////////////////
HRESULT CServiceModule::LoadExchangeCodes()
{
	ATLVERIFY(SUCCEEDED(_LoadExchangeCodes(STOCKEXCHANGECODES_SETTINGS, m_StockExchangeCodes)));
	ATLVERIFY(SUCCEEDED(_LoadExchangeCodes(OPTIONEXCHANGECODES_SETTINGS, m_OptionExchangeCodes)));

	return S_OK;
}

HRESULT CServiceModule::_LoadExchangeCodes(LPCTSTR lpszKey, _ExchangeCodes& Exchanges)
{
	CRegKey key;
	HRESULT hr = S_OK;

	_bstr_t bsKey = EXCHANGECODES_SETTINGS_ROOT _T("\\") + _bstr_t(lpszKey);
	DWORD dwRes = key.Open(HKEY_LOCAL_MACHINE, bsKey, KEY_READ);

	if (dwRes == ERROR_SUCCESS)
	{
		DWORD nIndex = 0;
		TCHAR lpValueName[256] = {0};
		const DWORD cnValueName = sizeof(lpValueName) / sizeof(*lpValueName);
		DWORD nValueName = cnValueName;
		DWORD nType = REG_NONE;
		BYTE  lpValue[256] = {0};
		const DWORD cnValue = sizeof(lpValue);
		DWORD nValue = cnValue;

		while ( ERROR_NO_MORE_ITEMS != (dwRes = ::RegEnumValue(key.m_hKey, nIndex, lpValueName, &nValueName, NULL, &nType, lpValue, &nValue)) )
		{
			if (dwRes != ERROR_SUCCESS)
				continue;

			if (nType == REG_SZ && nValueName == 1 && nValue == 2)
				Exchanges[lpValueName[0]] = lpValue[0];

			nIndex++;
			::ZeroMemory(lpValueName, sizeof(lpValueName));
			nValueName = cnValueName;
			::ZeroMemory(lpValue, cnValue);
			nValue = cnValue;
		}

		key.Close();
	}
	else
	{
		hr = HRESULT_FROM_WIN32(dwRes);
	}

	return hr;
}
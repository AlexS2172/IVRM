// EgRouterSrv.cpp : Implementation of WinMain

#include "stdafx.h"
#include "resource.h"
#include "EgRouterSrv.h"
#import "libid:814797FA-CE13-44EC-A03C-840C36A980F6" rename_namespace("Transport") rename("SendMessage", "SendMessageX")

#define ROUTER_SERVICE						_T("Egar Router")
#define ROUTER_SERVICE_DESC					_T("Provides routing of messages and delivered they to the subscribers hosts")
#define ROUTER_SERVICE_DEPEND				_T("Egar Transport\0")

typedef WINADVAPI BOOL ( WINAPI *LPFN_ChangeServiceConfig2)(
	SC_HANDLE    hService,
	DWORD        dwInfoLevel,
	LPVOID       lpInfo);	

#ifdef UNICODE
#define ChangeServiceConfig2Name "ChangeServiceConfig2W"
#else
#define ChangeServiceConfig2Name "ChangeServiceConfig2A"
#endif// UNICODE
#define __EG_TCP_DRIVER_IMPORT__

class CEgRouterSrvModule : 
	public CAtlServiceModuleT< CEgRouterSrvModule, IDS_SERVICENAME >
{
public :
	DECLARE_LIBID(LIBID_EgRouterSrvLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_EGROUTERSRV, "{71F55EB2-6B8B-4abb-B078-6815FA1CB4A6}")
	HRESULT InitializeSecurity() throw()
	{
		CSecurityDescriptor sd;
		HRESULT hRes = sd.InitializeFromThreadToken();
		if(SUCCEEDED(hRes))
		{
			hRes = CoInitializeSecurity(sd, -1, NULL, NULL,
				RPC_C_AUTHN_LEVEL_PKT, RPC_C_IMP_LEVEL_IMPERSONATE, NULL, EOAC_NONE, NULL);
		}		
// 		else
// 			Trace(1,_T("Router Service Run"),_T("InitializeSecurity error %X"),hRes);

		return hRes;
	}

	inline HRESULT RegisterAppId(bool bService = false) throw()
	{
		if (!Uninstall())
			return E_FAIL;

		HRESULT hr = UpdateRegistryAppId(TRUE);
		if (FAILED(hr))
			return hr;

		CRegKey keyAppID;
		LONG lRes = keyAppID.Open(HKEY_CLASSES_ROOT, _T("AppID"), KEY_WRITE);
		if (lRes != ERROR_SUCCESS)
			return AtlHresultFromWin32(lRes);

		CRegKey key;

		lRes = key.Create(keyAppID, GetAppIdT());
		if (lRes != ERROR_SUCCESS)
			return AtlHresultFromWin32(lRes);

		key.DeleteValue(_T("LocalService"));

		if (!bService)
			return S_OK;

		key.SetStringValue(_T("LocalService"), m_szServiceName);

		// Create service
		if (!Install())
			return E_FAIL;
		return S_OK;
	}

	BOOL Install() throw()
	{
		if (IsInstalled())
			return TRUE;

		// Get the executable file path
		TCHAR szFilePath[MAX_PATH + _ATL_QUOTES_SPACE];
		DWORD dwFLen = ::GetModuleFileName(NULL, szFilePath + 1, MAX_PATH);
		if( dwFLen == 0 || dwFLen == MAX_PATH )
			return FALSE;

		// Quote the FilePath before calling CreateService
		szFilePath[0] = _T('\"');
		szFilePath[dwFLen + 1] = _T('\"');
		szFilePath[dwFLen + 2] = 0;

		SC_HANDLE hSCM = ::OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
		if (hSCM == NULL)
		{
			TCHAR szBuf[1024];
			if (AtlLoadString(ATL_SERVICE_MANAGER_OPEN_ERROR, szBuf, 1024) == 0)
#ifdef UNICODE
				Checked::wcscpy_s(szBuf, _countof(szBuf), _T("Could not open Service Manager"));
#else
				Checked::strcpy_s(szBuf, _countof(szBuf), _T("Could not open Service Manager"));
#endif
			MessageBox(NULL, szBuf, m_szServiceName, MB_OK);
			return FALSE;
		}

		SC_HANDLE hService = ::CreateService(
			hSCM, ROUTER_SERVICE, ROUTER_SERVICE,
			SERVICE_ALL_ACCESS, SERVICE_WIN32_SHARE_PROCESS,
			SERVICE_DEMAND_START, SERVICE_ERROR_NORMAL,
			szFilePath, NULL, NULL, ROUTER_SERVICE_DEPEND, NULL, NULL);

		if (hService == NULL)
		{
			::CloseServiceHandle(hSCM);
			TCHAR szBuf[1024];
			if (AtlLoadString(ATL_SERVICE_START_ERROR, szBuf, 1024) == 0)
#ifdef UNICODE
				Checked::wcscpy_s(szBuf, _countof(szBuf), _T("Could not start service"));
#else
				Checked::strcpy_s(szBuf, _countof(szBuf), _T("Could not start service"));
#endif
			MessageBox(NULL, szBuf, m_szServiceName, MB_OK);
			return FALSE;
		}
		else
		{
			HMODULE hAdvApi = ::LoadLibrary(_T("Advapi32"));
			LPFN_ChangeServiceConfig2 pfnChangeServiceConfig2 = NULL; 
			if(hAdvApi)
			{
				pfnChangeServiceConfig2 = (LPFN_ChangeServiceConfig2)GetProcAddress(hAdvApi, ChangeServiceConfig2Name);

				if(pfnChangeServiceConfig2)
				{
					SERVICE_DESCRIPTION sd = { (PTSTR) ROUTER_SERVICE_DESC };
					pfnChangeServiceConfig2(hService, SERVICE_CONFIG_DESCRIPTION, &sd);
				}

				::FreeLibrary(hAdvApi);
			}
		}

		::CloseServiceHandle(hService);
		::CloseServiceHandle(hSCM);
		return TRUE;
	}



	HRESULT Run(int nShowCmd = SW_HIDE) throw()
	{
		HRESULT hr = S_OK;
// 		if(!EgStd::g_pTrace)
// 			EgStd::g_pTrace = new CEgTraceManager;
// 		EgStd::g_pTrace->Init();

		hr = PreMessageLoop(nShowCmd);

		if (SUCCEEDED(hr))
		{
			hr = m_spRouter.CreateInstance(__uuidof(Transport::Router));
			if(FAILED(hr))
			{
//				Trace(1, _T("Router Service Run"), _T("Router Server starting error %X"), hr);
				SetServiceStatus(SERVICE_STOPPED);
			}
			else
			{
				hr = m_spRouter->raw_Start();
				if(SUCCEEDED(hr))
				{
					if (m_bService)
					{
//						Trace(1, _T("Router Service Run"), _T("Router Server Started"));
						SetServiceStatus(SERVICE_RUNNING);
					}
					RunMessageLoop();
					hr =m_spRouter->raw_Stop();
				}
				else
				{
//					Trace(1, _T("Router Service Run"), _T("Router Server Stopped"));
					SetServiceStatus(SERVICE_STOPPED);
				}

			}
		}
		if (SUCCEEDED(hr))
		{
			hr = PostMessageLoop();
		}

// 		if(EgStd::g_pTrace)
// 		{
// 			EgStd::g_pTrace->Exit();
// 			delete EgStd::g_pTrace;
// 			EgStd::g_pTrace = NULL;
// 		}

		return hr;
	}

public:

// 	void Trace(long lLogLevel, const TCHAR* lpszCategory, const TCHAR* lpszMessage, ...)
// 	{
// 		if(!lpszMessage || !EgStd::g_pTrace)
// 			return;
// 
// 		va_list arglist;
// 		va_start(arglist, lpszMessage);
// 
// 		EgStd::g_pTrace->Trace(lLogLevel, lpszCategory, lpszMessage, arglist);
// 	}

private:
	Transport::IRouterPtr  m_spRouter;
};

CEgRouterSrvModule _AtlModule;


//
extern "C" int WINAPI _tWinMain(HINSTANCE /*hInstance*/, HINSTANCE /*hPrevInstance*/, 
                                LPTSTR /*lpCmdLine*/, int nShowCmd)
{
    return _AtlModule.WinMain(nShowCmd);
}


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

CHandle g_hMultilock;
// Passed to CreateThread to monitor the shutdown event
static DWORD WINAPI MonitorProc(void* pv)
{
    CExeModule* p = (CExeModule*)pv;
    p->MonitorShutdown();
    return 0;
}

LONG CExeModule::Unlock()
{
    LONG l = CComModule::Unlock();
    if (l == 0)
    {
        bActivity = true;
        SetEvent(hEventShutdown); // tell monitor that we transitioned to zero
    }
    return l;
}

//Monitors the shutdown event
void CExeModule::MonitorShutdown()
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

//--------------------------------------------------------------------------------------------------//
HRESULT CExeModule::RegisterServer(BOOL bRegTypeLib, const CLSID* pCLSID)
{
	HRESULT hr = CComModule::RegisterServer(bRegTypeLib, pCLSID);
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

	return hr;
}

HRESULT CExeModule::UnregisterServer(BOOL bUnRegTypeLib, const CLSID* pCLSID)
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
	return CComModule::UnregisterServer(bUnRegTypeLib, pCLSID);
}

bool CExeModule::StartMonitor()
{
    hEventShutdown = CreateEvent(NULL, false, false, NULL);
    if (hEventShutdown == NULL)
        return false;
    DWORD dwThreadID;
    HANDLE h = CreateThread(NULL, 0, MonitorProc, this, 0, &dwThreadID);
    return (h != NULL);
}

CExeModule _Module;

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

inline DWORD ModifyDefaultDacl()
{
	HANDLE hProcess =GetCurrentProcess();
	int                      i;
	ACL_SIZE_INFORMATION     asi;
	ACCESS_ALLOWED_ACE       *pace;
	DWORD                    dwNewAclSize;
	DWORD                    dwSize            = 0;
	DWORD                    dwTokenInfoLength = 0;
	DWORD                    dwResult          = 0;
	HANDLE                   hToken            = NULL;
	PACL                     pacl              = NULL;
	PSID                     psidEveryone      = NULL;
	SID_IDENTIFIER_AUTHORITY siaWorld = SECURITY_WORLD_SID_AUTHORITY;
	TOKEN_DEFAULT_DACL       tddNew;
	TOKEN_DEFAULT_DACL       *ptdd    = NULL;
	TOKEN_INFORMATION_CLASS  tic      = TokenDefaultDacl;

	__try
	{
		// 
		// Obtain an access token.
		// 
		if (!OpenProcessToken(hProcess, TOKEN_QUERY |
			TOKEN_ADJUST_DEFAULT, &hToken))
		{
			dwResult = GetLastError();
			__leave;
		}

		// 
		// Obtain buffer size for DACL information.
		// 
		if (!GetTokenInformation(hToken, tic, (LPVOID)NULL,
			dwTokenInfoLength, &dwSize))
		{
			if (GetLastError() == ERROR_INSUFFICIENT_BUFFER)
			{
				ptdd = (TOKEN_DEFAULT_DACL *) LocalAlloc(LPTR, dwSize);
				if (ptdd == NULL)
				{
					dwResult = GetLastError();
					__leave;
				}

				if (!GetTokenInformation(hToken, tic, (LPVOID)ptdd, dwSize,
					&dwSize))
				{
					dwResult = GetLastError();
					__leave;
				}
			}
			else
			{
				dwResult = GetLastError();
				__leave;
			}
		}

		// 
		// Obtain ACL information.
		// 
		if (!GetAclInformation(ptdd->DefaultDacl, (LPVOID)&asi,
			(DWORD)sizeof(ACL_SIZE_INFORMATION),
			AclSizeInformation))
		{
			dwResult = GetLastError();
			__leave;
		}

		// 
		// Create SID for the Everyone group.
		// 
		if (!AllocateAndInitializeSid(&siaWorld, 1, SECURITY_WORLD_RID, 0,
			0, 0, 0, 0, 0, 0, &psidEveryone))
		{
			dwResult = GetLastError();
			__leave;
		}

		// 
		// Compute the size of the new ACL.
		// 
		dwNewAclSize = asi.AclBytesInUse + sizeof(ACCESS_ALLOWED_ACE) +
			GetLengthSid(psidEveryone) - sizeof(DWORD);

		// 
		// Allocate buffer for the new ACL.
		// 
		pacl = (PACL) LocalAlloc(LPTR, dwNewAclSize);
		if (pacl == NULL)
		{
			dwResult = GetLastError();
			__leave;
		}

		// 
		// Intialize the ACL.
		// 
		if (!InitializeAcl(pacl, dwNewAclSize, ACL_REVISION))
		{
			dwResult = GetLastError();
			__leave;
		}

		// 
		// Loop through all the ACEs.
		// 
		for (i = 0; i < (int) asi.AceCount; i++)
		{
			// 
			// Get current ACE.
			// 
			if (!GetAce(ptdd->DefaultDacl, i, (LPVOID *)&pace))
			{
				dwResult = GetLastError();
				__leave;
			}

			// 
			// Build the new ACL.
			// 
			if (!AddAce(pacl, ACL_REVISION, MAXDWORD, pace,
				((PACE_HEADER)pace)->AceSize))
			{
				dwResult = GetLastError();
				__leave;
			}
		}

		// 
		// Add the new ACE.
		// 
		if (!AddAccessAllowedAce(pacl, ACL_REVISION, GENERIC_ALL,
			psidEveryone))
		{
			dwResult = GetLastError();
			__leave;
		}

		// 
		// Set the new Default DACL.
		// 
		tddNew.DefaultDacl = pacl;

		if (!SetTokenInformation(hToken, tic, (LPVOID)&tddNew,
			dwNewAclSize))
		{
			dwResult = GetLastError();
			__leave;
		}
	}

	__finally
	{
		// 
		// Free the buffer for the sid.
		// 
		if (psidEveryone)
		{
			FreeSid(psidEveryone);
		}

		// 
		// Free the buffers.
		// 
		if (pacl)
		{
			LocalFree((HLOCAL)pacl);
		}

		if (ptdd)
		{
			LocalFree((HLOCAL)ptdd);
		}

		// 
		// Close the access token.
		// 
		if (hToken)
		{
			CloseHandle(hToken);
		}
	}

	return dwResult;
}

/////////////////////////////////////////////////////////////////////////////
//
extern "C" int WINAPI _tWinMain(HINSTANCE hInstance,
    HINSTANCE /*hPrevInstance*/, LPTSTR lpCmdLine, int /*nShowCmd*/)
{
	
	//ModifyDefaultDacl();
	g_hMultilock.Attach(::CreateMutex(NULL,FALSE,_T("HyperFeedPriceProvider_F2109AFC_9751_4d7f_A4B6_411E30F05F8C")));
	if(g_hMultilock != NULL && GetLastError() == ERROR_ALREADY_EXISTS)
		return ERROR_ALREADY_EXISTS;

	lpCmdLine = GetCommandLine(); //this line necessary for _ATL_MIN_CRT

    HRESULT hRes = CoInitializeEx(NULL, COINIT_MULTITHREADED);

    _ASSERTE(SUCCEEDED(hRes));
    _Module.Init(ObjectMap, hInstance, &LIBID_HYPERFEEDPROVIDERSLib);
    _Module.dwThreadID = GetCurrentThreadId();
    TCHAR szTokens[] = _T("-/");

    int nRet = 0;
    BOOL bRun = TRUE;
    LPCTSTR lpszToken = FindOneOf(lpCmdLine, szTokens);
    while (lpszToken != NULL)
    {
        if (lstrcmpi(lpszToken, _T("UnregServer"))==0)
        {
            _Module.UpdateRegistryFromResource(IDR_HyperFeedProviders, FALSE);
            nRet = _Module.UnregisterServer(TRUE);
            bRun = FALSE;
            break;
        }
        if (lstrcmpi(lpszToken, _T("RegServer"))==0)
        {
            _Module.UpdateRegistryFromResource(IDR_HyperFeedProviders, TRUE);
            nRet = _Module.RegisterServer(TRUE);
            bRun = FALSE;
            break;
        }
        lpszToken = FindOneOf(lpszToken, szTokens);
    }

    if (bRun)
    {
		// Starting with trace
		EgLib::CEgLibTraceManager::g_spTrace = EgLib::CEgLibTraceManagerPtr(static_cast<EgLib::CEgLibTraceManager*>(new CHyperFeedTrace()));


#ifndef _DEBUG 
		SetMiniDumpDefaultCrashHandler();
#endif
		ATLVERIFY(SUCCEEDED(_Module.LoadExchangeCodes()));
		ATLVERIFY(SUCCEEDED(_Module.LoadProviderSettings()));

       _Module.StartMonitor();
#if _WIN32_WINNT >= 0x0400 & defined(_ATL_FREE_THREADED)
        hRes = _Module.RegisterClassObjects(CLSCTX_LOCAL_SERVER,
            REGCLS_MULTIPLEUSE | REGCLS_SUSPENDED);
        _ASSERTE(SUCCEEDED(hRes));
        hRes = CoResumeClassObjects();
#else
        hRes = _Module.RegisterClassObjects(CLSCTX_LOCAL_SERVER,
            REGCLS_MULTIPLEUSE);
#endif
        _ASSERTE(SUCCEEDED(hRes));

        MSG msg;
        while (GetMessage(&msg, 0, 0, 0))
            DispatchMessage(&msg);

        _Module.RevokeClassObjects();
		EgLib::CEgLibTraceManager::Trace(EgLib::LogInfoExt, _T("HyperFeed Provider"), _T("FINISH"));
        Sleep(dwPause); //wait for any threads to finish
		EgLib::CEgLibTraceManager::g_spTrace.reset();
    }

    _Module.Term();
    CoUninitialize();
    return nRet;
}

/////////////////////////////////////////////////////////////////////////////
HRESULT CExeModule::LoadExchangeCodes()
{
	ATLVERIFY(SUCCEEDED(_LoadExchangeCodes(STOCKEXCHANGECODES_SETTINGS, m_StockExchangeCodes)));
	ATLVERIFY(SUCCEEDED(_LoadExchangeCodes(OPTIONEXCHANGECODES_SETTINGS, m_OptionExchangeCodes)));

	return S_OK;
}
HRESULT CExeModule::LoadProviderSettings()
{
	m_dwBatchNotificationDelay = 1000;


	CEgRegKey key;
	HRESULT hr = S_OK;
	_bstr_t bsKey = PROVIDER_SETTINGS_ROOT;
	DWORD dwRes = key.Open(HKEY_LOCAL_MACHINE, bsKey, KEY_READ);
	if (dwRes == ERROR_SUCCESS)
	{
		DWORD dw = 0;
		if(ERROR_SUCCESS == key.QueryValue(dw, _T("BatchNotificationDelay")))
			m_dwBatchNotificationDelay = dw;

		m_dwStartTime = 0;
		if(ERROR_SUCCESS == key.QueryValue(dw, _T("Idle Checking Start Time")))
			m_dwStartTime = dw;

		m_dwStopTime = 0;
		if(ERROR_SUCCESS == key.QueryValue(dw, _T("Idle Checking Stop Time")))
			m_dwStopTime = dw;

		m_dwInterval = 0;
		if(ERROR_SUCCESS == key.QueryValue(dw, _T("Idle Checking Intervals")))
			m_dwInterval = dw;

		m_dwMaxQueueSize = 50000;
		if(ERROR_SUCCESS == key.QueryValue(dw, _T("Max Queue Size")))
		{
			if(dw >= 5000)
				m_dwMaxQueueSize = dw;
		}

		m_lThreadPoolCapacity = -2;
		_bstr_t bsCapacityString;
		if(ERROR_SUCCESS == key.QueryValue(bsCapacityString, "Thread Pool Capacity"))
		{
			long lCapacity = static_cast<long>(_variant_t(bsCapacityString));
			if(lCapacity <= 10 && lCapacity >=-5)
			{
				m_lThreadPoolCapacity = lCapacity;
			}
		}

#ifdef __PERFTEST
		m_dwSimulatedDataFlow = 0;
		if(ERROR_SUCCESS == key.QueryDWORDValue("Simulated Data Flow", dw))
			m_dwSimulatedDataFlow = dw;
#endif // __PERFTEST

	}
	return hr;
}


HRESULT CExeModule::_LoadExchangeCodes(LPCTSTR lpszKey, _ExchangeCodes& Exchanges)
{
	CRegKey key;
	HRESULT hr = S_OK;

	_bstr_t bsKey = PROVIDER_SETTINGS_ROOT _T("\\") + _bstr_t(lpszKey);
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
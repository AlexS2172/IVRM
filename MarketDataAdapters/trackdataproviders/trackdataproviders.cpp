// TrackDataProviders.cpp : Implementation of WinMain


// Note: Proxy/Stub Information
//      To build a separate proxy/stub DLL, 
//      run nmake -f TrackDataProvidersps.mk in the project directory.

#include "stdafx.h"
#include "resource.h"
#include <initguid.h>
#include "TrackDataProviders.h"

#include "TrackDataProviders_i.c"
#include "TrackDataStructureProvider.h"
#include "TrackDataPriceProvider.h"
#include "TrackDataStructureInfo.h"
#include "TrackDataPriceInfo.h"
#include "ProviderInfo.h"


const DWORD dwTimeOut = 5000; // time for EXE to be idle before shutting down
const DWORD dwPause = 1000; // time to wait for threads to finish up

// {69677561-186E-42b5-8C95-40234E8BF57A}

// Passed to CreateThread to monitor the shutdown event
static DWORD WINAPI MonitorProc(void* pv)
{
    CExeModule* p = (CExeModule*)pv;
    p->MonitorShutdown();
    return 0;
}

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
			catinfo.catid = CATID_PiceProviders;
			catinfo.lcid = 0x0409;  // locale=english
			
			USES_CONVERSION;        // uses A2W
			wcscpy(catinfo.szDescription,  A2W("EGAR Price Providers"));
			hr = spcr->RegisterCategories(1, &catinfo);
			if(SUCCEEDED(hr))
			{
				CATID catid = CATID_PiceProviders;
				hr = spcr->RegisterClassImplCategories( CLSID_ProviderInfo, 1, &catid);			
				if(SUCCEEDED(hr))
				{
					CATID catid = CATID_PiceProviders;
					LPOLESTR pDaya ;

					StringFromCLSID(CLSID_ProviderInfo, &pDaya);

					hr = spcr->RegisterClassImplCategories( CLSID_ProviderInfo, 1, &catid);			
					if(SUCCEEDED(hr))
					{
						_bstr_t str(_T("SOFTWARE\\Egar\\Providers\\"));
						str += pDaya;
						eg_reg::CEgRegKey reg;
						if(ERROR_SUCCESS==reg.Create(HKEY_LOCAL_MACHINE,(LPCTSTR)str))
						{
							reg.SetDWORDValue(_T(""),DEF_PROVIDER_ID);
							reg.SetStringValue(_T("Name"),W2T(DEF_PROVIDER_NAME));
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
			CATID catid = CATID_PiceProviders;
			spcr->UnRegisterClassImplCategories( CLSID_ProviderInfo, 1, &catid);			
	}
	return CComModule::UnregisterServer(bUnRegTypeLib, pCLSID);
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
OBJECT_ENTRY(CLSID_TrackDataStructureProvider, CTrackDataStructureProvider)
OBJECT_ENTRY(CLSID_TrackDataPriceProvider, CTrackDataPriceProvider)
OBJECT_ENTRY(CLSID_TrackDataStructureInfo, CTrackDataStructureInfo)
OBJECT_ENTRY(CLSID_TrackDataPriceInfo, CTrackDataPriceInfo)
OBJECT_ENTRY(CLSID_ProviderInfo, CProviderInfo)
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
    lpCmdLine = GetCommandLine(); //this line necessary for _ATL_MIN_CRT

    HRESULT hRes = CoInitializeEx(NULL, COINIT_MULTITHREADED);

    _ASSERTE(SUCCEEDED(hRes));
    _Module.Init(ObjectMap, hInstance, &LIBID_TRACKDATAPROVIDERSLib);
    _Module.dwThreadID = GetCurrentThreadId();
    TCHAR szTokens[] = _T("-/");

    int nRet = 0;
    BOOL bRun = TRUE;
    LPCTSTR lpszToken = FindOneOf(lpCmdLine, szTokens);
    while (lpszToken != NULL)
    {
        if (lstrcmpi(lpszToken, _T("UnregServer"))==0)
        {
            _Module.UpdateRegistryFromResource(IDR_TrackDataProviders, FALSE);
            nRet = _Module.UnregisterServer(TRUE);
            bRun = FALSE;
            break;
        }
        if (lstrcmpi(lpszToken, _T("RegServer"))==0)
        {
            _Module.UpdateRegistryFromResource(IDR_TrackDataProviders, TRUE);
            nRet = _Module.RegisterServer(TRUE);
            bRun = FALSE;
            break;
        }
        lpszToken = FindOneOf(lpszToken, szTokens);
    }

    if (bRun)
    {
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
        Sleep(dwPause); //wait for any threads to finish
    }

    _Module.Term();
    CoUninitialize();
    return nRet;
}

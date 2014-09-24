// ATFDataProvider.cpp : Implementation of WinMain


// Note: Proxy/Stub Information
//      To build a separate proxy/stub DLL, 
//      run nmake -f ATFDataProviderps.mk in the project directory.

#include "stdafx.h"
#include "resource.h"
#include <initguid.h>
#include "ATFDataProvider.h"

#include "ATFDataProvider_i.c"
#include "ATFPriceInfo.h"
#include "ATFPriceProvider.h"
#include "ATFStructureInfo.h"
#include "ATFStructureProvider.h"
#include "ProviderInfo.h"
#include "ATFPriceInfoWithNotify.h"
#include "EgDebugUtil.h"

const DWORD dwTimeOut = 5000; // time for EXE to be idle before shutting down
const DWORD dwPause = 1000; // time to wait for threads to finish up

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
			catinfo.catid = CATID_PriceProviders;
			catinfo.lcid = 0x0409;  // locale=english
			
			USES_CONVERSION;        // uses A2W
			wcscpy(catinfo.szDescription,  A2W("EGAR Price Providers"));
			hr = spcr->RegisterCategories(1, &catinfo);
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
	return hr;
}

HRESULT CExeModule::UnregisterServer(BOOL bUnRegTypeLib, const CLSID* pCLSID)
{
	CComPtr<ICatRegister> spcr;
	HRESULT hr = spcr.CoCreateInstance(CLSID_StdComponentCategoriesMgr, NULL, CLSCTX_INPROC);
	if(SUCCEEDED(hr) && spcr!=NULL)
	{
			CATID catid = CATID_PriceProviders;
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
OBJECT_ENTRY(CLSID_ATFPriceInfo, CATFPriceInfo)
OBJECT_ENTRY(CLSID_ATFPriceProvider, CATFPriceProvider)
OBJECT_ENTRY(CLSID_ATFStructureInfo, CATFStructureInfo)
OBJECT_ENTRY(CLSID_ATFStructureProvider, CATFStructureProvider)
OBJECT_ENTRY(CLSID_ProviderInfo, CProviderInfo)
OBJECT_ENTRY(CLSID_ATFPriceInfoWithNotify, CATFPriceInfoWithNotify)
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
    _Module.Init(ObjectMap, hInstance, &LIBID_ATFDATAPROVIDERLib);
    _Module.dwThreadID = GetCurrentThreadId();
    TCHAR szTokens[] = _T("-/");

    int nRet = 0;
    BOOL bRun = TRUE;
    LPCTSTR lpszToken = FindOneOf(lpCmdLine, szTokens);
    while (lpszToken != NULL)
    {
        if (lstrcmpi(lpszToken, _T("UnregServer"))==0)
        {
            _Module.UpdateRegistryFromResource(IDR_ATFDataProvider, FALSE);
            nRet = _Module.UnregisterServer(TRUE);
            bRun = FALSE;
            break;
        }
        if (lstrcmpi(lpszToken, _T("RegServer"))==0)
        {
            _Module.UpdateRegistryFromResource(IDR_ATFDataProvider, TRUE);
            nRet = _Module.RegisterServer(TRUE);
            bRun = FALSE;
            break;
        }
        lpszToken = FindOneOf(lpszToken, szTokens);
    }

    if (bRun)
    {
		SetMiniDumpDefaultCrashHandler();
		BEGIN_TRACE()
		ATLVERIFY(SUCCEEDED(_Module.LoadExchangeCodes()));
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
		END_TRACE()
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

HRESULT CExeModule::_LoadExchangeCodes(LPCTSTR lpszKey, _ExchangeCodes& Exchanges)
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
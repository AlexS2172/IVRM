// FIXGateway.cpp : Implementation of WinMain


// Note: Proxy/Stub Information
//      To build a separate proxy/stub DLL,
//      run nmake -f FIXGatewayps.mk in the project directory.

#include "stdafx.h"
#include <locale.h>
#include "resource.h"

#include "FIXGateway_i.c"

#include "FixGServer.h"
#include "EgDebugUtil.h"

class CFixGServerModule : public CAtlExeModuleT< CFixGServerModule >
{
public :
	DECLARE_LIBID(LIBID_FIXGATEWAYLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_FIXGateway, "{080EA18E-762A-4212-8524-7124036C8223}")
	HRESULT PreMessageLoop(int nShowCmd);
	HRESULT PostMessageLoop();
};

CFixGServerModule _AtlModule;

extern "C" int WINAPI _tWinMain(HINSTANCE /*hInstance*/, HINSTANCE /*hPrevInstance*/,
                                LPTSTR /*lpCmdLine*/, int nShowCmd)
{
	SetMiniDumpDefaultCrashHandler();
    return _AtlModule.WinMain(nShowCmd);
}

HRESULT CFixGServerModule::PreMessageLoop(int nShowCmd)
{
	setlocale(LC_ALL, "English");
	WSADATA	wsaData;
	WORD	wVersion = MAKEWORD(2,2);

    int nRet = WSAStartup(wVersion, &wsaData);
	if(nRet != 0)
		return E_FAIL;

	HRESULT hr = __super::PreMessageLoop(nShowCmd);
#if _ATL_VER == 0x0700
	if (SUCCEEDED (hr) && !m_bDelayShutdown)
		hr = CoResumeClassObjects();
#endif

	return hr;
}
HRESULT CFixGServerModule::PostMessageLoop()
{
	int iErr = WSACleanup();

	return CAtlExeModuleT< CFixGServerModule >::PostMessageLoop();
}

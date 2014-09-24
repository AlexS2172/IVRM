// EtsEodServer.cpp : Implementation of WinMain

#include "stdafx.h"
#include "resource.h"
#include "EtsEodServer.h"
#include <stdio.h>
#include "EgDebugUtil.h"

//class CEtsEodServerModule : public CAtlServiceModuleT< CEtsEodServerModule, IDS_SERVICENAME >
class CEtsEodServerModule : public CAtlExeModuleT< CEtsEodServerModule >
{
public :
	DECLARE_LIBID(LIBID_EtsEodServerLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_ETSEodServer, "{1660C934-AC45-417D-8AE3-68858FE9FA9A}")
	HRESULT PreMessageLoop(int nShowCmd);
	HRESULT PostMessageLoop();
};

CEtsEodServerModule _AtlModule;

//
extern "C" int WINAPI _tWinMain(HINSTANCE /*hInstance*/, HINSTANCE /*hPrevInstance*/, 
                                LPTSTR /*lpCmdLine*/, int nShowCmd)
{
	SetMiniDumpDefaultCrashHandler();
    return _AtlModule.WinMain(nShowCmd);
}

HRESULT CEtsEodServerModule::PreMessageLoop(int nShowCmd)
{
	setlocale(LC_ALL, "English");

	EODCO::CEgTraceManagerPtr spTrace(new EODCO::CEgTraceManager);
	EODCO::CEgTraceManager::s_spTrace = spTrace;
	EODCO::CEgTraceManager::s_spTrace->Start();

	HRESULT hr = __super::PreMessageLoop(nShowCmd);
#if _ATL_VER == 0x0700
	if (SUCCEEDED (hr) && !m_bDelayShutdown)
		hr = CoResumeClassObjects(); 
#endif

	return hr;
}

HRESULT CEtsEodServerModule::PostMessageLoop()
{
	EODCO::CEgTraceManager::s_spTrace->Stop();
	EODCO::CEgTraceManager::s_spTrace.reset();

	return CAtlExeModuleT< CEtsEodServerModule >::PostMessageLoop();
//	return CAtlServiceModuleT< CEtsEodServerModule,IDS_SERVICENAME >::PostMessageLoop();
}

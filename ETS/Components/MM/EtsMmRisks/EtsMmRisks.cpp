// EtsMmRisks.cpp : Implementation of DLL Exports.

#include "stdafx.h"
#include "resource.h"
#include "EtsMmRisks.h"

class CEtsMmRisksModule : public CAtlDllModuleT< CEtsMmRisksModule >
{
public :
	DECLARE_LIBID(LIBID_EtsMmRisksLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_ETSMMRISKS, "{2FB52C10-9A15-4A99-AFA5-85CAA039D98C}")
};

CEtsMmRisksModule _AtlModule;

// DLL Entry Point
extern "C" BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{	
	if (EgLib::CEgLibTraceManager::g_spTrace == NULL)
	{
		EgLib::CEgLibTraceManager::g_spTrace = EgLib::CEgLibTraceManagerPtr(static_cast<EgLib::CEgLibTraceManager*>
								(new EgLib::CEgLibTraceManager(EgLib::CEgLibTraceManager::enRelativeMyDocuments, _T("core"))));
								
		EgLib::CEgLibTraceManager::g_spTrace->FileName = L"ivrm_riskview";										
	}
									
	hInstance;
    return _AtlModule.DllMain(dwReason, lpReserved); 
}


// Used to determine whether the DLL can be unloaded by OLE
STDAPI DllCanUnloadNow(void)
{
    return _AtlModule.DllCanUnloadNow();
}


// Returns a class factory to create an object of the requested type
STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID* ppv)
{
    return _AtlModule.DllGetClassObject(rclsid, riid, ppv);
}


// DllRegisterServer - Adds entries to the system registry
STDAPI DllRegisterServer(void)
{
    // registers object, typelib and all interfaces in typelib
    HRESULT hr = _AtlModule.DllRegisterServer();
	return hr;
}


// DllUnregisterServer - Removes entries from the system registry
STDAPI DllUnregisterServer(void)
{
	HRESULT hr = _AtlModule.DllUnregisterServer();
	return hr;
}

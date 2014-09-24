// EtsMmGreeks.cpp : Implementation of DLL Exports.

#include "stdafx.h"
#include "resource.h"
#include "EtsMmGreeks.h"

class CEtsMmGreeksModule : public CAtlDllModuleT< CEtsMmGreeksModule >
{
public :
	DECLARE_LIBID(LIBID_EtsMmGreeksLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_ETSMMGREEKS, "{4889AA90-FE80-4451-B217-0C07FE7A8338}")
};

CEtsMmGreeksModule _AtlModule;


// DLL Entry Point
extern "C" BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
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

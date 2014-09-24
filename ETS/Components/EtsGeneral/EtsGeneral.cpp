// EtsGeneral.cpp : Implementation of DLL Exports.

#include "stdafx.h"
#include "resource.h"
#include "EtsGeneral.h"

CCalculationSettingsPtr CCalculationSettingsSingleton::m_spSettings = CCalculationSettingsPtr();
_bstr_t	CGenegalSettings::m_DBConnectionString = _bstr_t("");

class CEtsGeneralModule : public CAtlDllModuleT< CEtsGeneralModule >
{
public :
	DECLARE_LIBID(LIBID_EtsGeneralLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_ETSGENERAL, "{0CB1C469-F350-45B2-8731-0629A0F17F99}")
};

CEtsGeneralModule _AtlModule;


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

// PriceProviders.cpp : Implementation of DLL Exports.


// Note: Proxy/Stub Information
//      To build a separate proxy/stub DLL, 
//      run nmake -f PriceProvidersps.mk in the project directory.

#include "stdafx.h"
#include "resource.h"
#include <initguid.h>
#include "PriceProviders.h"

#include "PriceProviders_i.c"
#include "PriceProvider.h"
#include "PriceInfo.h"
#include "StructureProvider.h"
#include "StructureProviderEx.h"
#include "StructureInfo.h"
#include "Providers.h"
#include "Provider.h"
#include "BatchPriceInfo.h"
#include "PriceInfoWithNotify.h"
#include "ProvidersCache.h"

CComModule _Module;
CProvidersCollection g_Providers;

BEGIN_OBJECT_MAP(ObjectMap)
OBJECT_ENTRY(CLSID_PriceProvider, CPriceProvider)
OBJECT_ENTRY(CLSID_PriceInfo, CPriceInfo)
OBJECT_ENTRY(CLSID_StructureProvider, CStructureProvider)
OBJECT_ENTRY(CLSID_StructureProviderEx, CStructureProviderEx)
OBJECT_ENTRY(CLSID_StructureInfo, CStructureInfo)
OBJECT_ENTRY(CLSID_Providers, CProviders)
OBJECT_ENTRY(CLSID_BatchPriceInfo, CBatchPriceInfo)
OBJECT_ENTRY(CLSID_PriceInfoWithNotify, CPriceInfoWithNotify)
OBJECT_ENTRY(CLSID_ProvidersCache, CProvidersCache)
END_OBJECT_MAP()

/////////////////////////////////////////////////////////////////////////////
// DLL Entry Point

extern "C"
BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID /*lpReserved*/)
{
    if (dwReason == DLL_PROCESS_ATTACH)
    {
        _Module.Init(ObjectMap, hInstance, &LIBID_PRICEPROVIDERSLib);
        DisableThreadLibraryCalls(hInstance);
    }
    else if (dwReason == DLL_PROCESS_DETACH)
        _Module.Term();
    return TRUE;    // ok
}

/////////////////////////////////////////////////////////////////////////////
// Used to determine whether the DLL can be unloaded by OLE

STDAPI DllCanUnloadNow(void)
{
    return (_Module.GetLockCount()==0) ? S_OK : S_FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// Returns a class factory to create an object of the requested type

STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID* ppv)
{
    return _Module.GetClassObject(rclsid, riid, ppv);
}

/////////////////////////////////////////////////////////////////////////////
// DllRegisterServer - Adds entries to the system registry

STDAPI DllRegisterServer(void)
{
    // registers object, typelib and all interfaces in typelib
    return _Module.RegisterServer(TRUE);
}

/////////////////////////////////////////////////////////////////////////////
// DllUnregisterServer - Removes entries from the system registry

STDAPI DllUnregisterServer(void)
{
    return _Module.UnregisterServer(TRUE);
}



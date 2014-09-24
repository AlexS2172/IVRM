// ETSPerfObjects.cpp : Implementation of DLL Exports.

#include "stdafx.h"
#include "resource.h"
#include "EtsPerformance.h"

// The module attribute causes DllMain, DllRegisterServer and DllUnregisterServer to be automatically implemented for you
[ module(dll, uuid = "{5B766CF9-FD31-4014-8501-164F7C7A61AE}", 
		 name = "ETSPerfObjects", 
		 helpstring = "ETSPerfObjects 1.0 Type Library",
		 resource_name = "IDR_ETSPERFOBJECTS") ]
class CETSPerfObjectsModule
{
public:
// Override CAtlDllModuleT members
};
		 

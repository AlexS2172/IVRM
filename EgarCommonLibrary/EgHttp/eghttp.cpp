// EgHttp.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "EgHttp.h"
#include "resource.h"

#define __countof(arr) (sizeof(arr) / sizeof(arr[0]))

/////////////////////////////////////////////////////////////////////////////
//
BOOL APIENTRY DllMain(HANDLE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
	hModule;
	ul_reason_for_call;
	lpReserved;

	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
    return TRUE;
}


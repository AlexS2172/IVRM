// stdafx.h : include file for standard system include files,
//      or project specific include files that are used frequently,
//      but are changed infrequently

#if !defined(AFX_STDAFX_H__451BC436_F10D_4E5B_AE5F_BE98D6552F69__INCLUDED_)
#define AFX_STDAFX_H__451BC436_F10D_4E5B_AE5F_BE98D6552F69__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define STRICT
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0400
#endif
#define _ATL_FREE_THREADED

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <Mswsock.h>
#include <mstcpip.h>

#include <atlbase.h>
//You may derive a class from CComModule and use it if you want to override
//something, but do not change the name of _Module
class CExeModule : public CComModule
{
public:
	LONG Unlock();
	DWORD dwThreadID;
	HANDLE hEventShutdown;
	void MonitorShutdown();
	bool StartMonitor();
	bool bActivity;
    
	HRESULT RegisterServer(BOOL bRegTypeLib = FALSE, const CLSID* pCLSID = NULL);
	HRESULT UnregisterServer(BOOL bUnRegTypeLib, const CLSID* pCLSID = NULL);
};
extern CExeModule _Module;
#include <atlcom.h>

#include <comutil.h>
#include <comdef.h>

#include "eg_lib\Lock.h"
#include "eg_lib\EgMisc.h"
#include "eg_lib\RecPtr.h"
#include "eg_lib\EgRegistry.h"

#pragma warning(disable:4786)
#pragma warning(disable:4503)
#pragma warning(disable:4290)
#pragma warning(disable:4244)
#pragma warning(disable:4018)

#include <string>
#include <vector>
#include <map>
#include <queue>
#include <set>
#include <deque>
#include <fstream>
using namespace std;

#include "OmexVer.h"

#include "Omni/oal_align_packed.h"

#include "Omni/om_inttypes.h"
#include "Omni/omni.h"
#include "Omni/omniapi.h"
#include "Omni/omnifact.h"
#include "Omni/omex_usa.h"
#include "Omni/const.h"

#include "Omni/oal_align_reset.h"

#pragma comment(lib, "../lib/omni/omntmt.lib")

#include <assert.h>
#include <process.h>
#include <time.h>
#include <sys/timeb.h>
#include <sys/types.h>

#ifdef _EGDEBUG
#	define EgDbgBreakPoint		__asm int 3;
#else
#	define EgDbgBreakPoint		;
#endif

#ifdef _EGDEBUG
#define    EgAssert(e)			if(!(e)){ __asm int 3};
#else
#define    EgAssert(e)			((void)0)
#endif
/*
#ifdef _EGDEBUG
#define EgTrace(s)				OutputDebugString(s);
#else
#define EgTrace(s)				((void)0)
#endif
*/

#include "Trace.h"

// {240A82B8-1403-45e8-92F6-E255C5DBF6E7}
static const GUID CATID_PiceProviders = 
{ 0x240a82b8, 0x1403, 0x45e8, { 0x92, 0xf6, 0xe2, 0x55, 0xc5, 0xdb, 0xf6, 0xe7 } };

#define DEF_PATH			_T("SOFTWARE\\eGAR\\{DCA02808-8BA0-4ca1-87D2-630DE73AF05D}")
#define DEF_PROVIDER_ID     5L
#define DEF_PROVIDER_NAME   L"ISE Price Provider"

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__451BC436_F10D_4E5B_AE5F_BE98D6552F69__INCLUDED)

// stdafx.h : include file for standard system include files,
//      or project specific include files that are used frequently,
//      but are changed infrequently

#if !defined(AFX_STDAFX_H__D2A72CAB_F6AA_429D_8AB8_B103A9956378__INCLUDED_)
#define AFX_STDAFX_H__D2A72CAB_F6AA_429D_8AB8_B103A9956378__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define STRICT
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0400
#endif

#define _ATL_FREE_THREADED

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

	void csLock(){m_csLock.Lock();}
	void csUnlock(){m_csLock.Unlock();}

	HRESULT RegisterServer(BOOL bRegTypeLib = FALSE, const CLSID* pCLSID = NULL);
	HRESULT UnregisterServer(BOOL bUnRegTypeLib, const CLSID* pCLSID = NULL);

private:
	CComAutoCriticalSection m_csLock;
};

extern CExeModule _Module;
#include <atlcom.h>
#include <comutil.h>
#include <comdef.h>

#include <vector>
#include <list>
#include <queue>
#include <map>
#include <set>
using namespace std;


extern "C" {
#include "..\rt_Sim\rt_share.h"
#include "..\rt_Sim\rt_Api.h"
}

#pragma comment(lib, "..\\lib\\rt_Sim.lib")

#include <EgLib/EgLibDbg.h>
#include <EgLib/EgLibSync.h>
#include <EgLib/EgLibThread.h>
#include <EgLib/EgLibMisc.h>
#include <EgLib/EgLibReg.h>
#include <EgLib/EgLibException.h>
#include <EgLib/EgLibStrategy.h>
#include <EgLib/EgLibComError.h>
#include <EgLib/EgLibMath.h>


#include <boost/smart_ptr.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/bind.hpp>
using namespace EgLib;

#include "PriceProviders.h"
#include "BaseNotifier.h"

// {240A82B8-1403-45e8-92F6-E255C5DBF6E7}
static const GUID CATID_PiceProviders = 
{ 0x240a82b8, 0x1403, 0x45e8, { 0x92, 0xf6, 0xe2, 0x55, 0xc5, 0xdb, 0xf6, 0xe7 } };

#define DEF_PROVIDER_ID 2L
#define DEF_PROVIDER_NAME L"TrackData Price Provider (Sim)"

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__D2A72CAB_F6AA_429D_8AB8_B103A9956378__INCLUDED)

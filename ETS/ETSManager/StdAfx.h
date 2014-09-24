// stdafx.h : include file for standard system include files,
//      or project specific include files that are used frequently,
//      but are changed infrequently

#if !defined(AFX_STDAFX_H__55E23CC4_6D77_4EF3_9545_26CAFB5E7BBA__INCLUDED_)
#define AFX_STDAFX_H__55E23CC4_6D77_4EF3_9545_26CAFB5E7BBA__INCLUDED_

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
};
extern CExeModule _Module;
#include <atlcom.h>
#include <atlstr.h>
#include <comutil.h>
#include <comdef.h>

#include <EgLib/EgLibDbg.h>
#include <EgLib/EgLibThread.h>
#include <EgLib/EgLibSync.h>
#include <EgLib/EgLibreg.h>
#include <EgLib/EgLibMisc.h>
using namespace EgLib;

#import "../tlb/ETSXMLParams.tlb" no_namespace named_guids

#define DEF_ERROR_REGISTRY _T("Unable to read settings from registry")


//#define    DEF_APPLICATION_ROOT   _T("SOFTWARE\\Egar\\ETS")
//#define    DEF_PROVIDER_SETTINGS			_T("SOFTWARE\\Egar\\ETS\\PriceProvider")
//#define    DEF_DATABASE_SETTINGS			_T("SOFTWARE\\Egar\\ETS\\Database")
//#define    DEF_IV_SETTINGS					_T("SOFTWARE\\Egar\\ETS\\IVFeed")
//#define    DEF_VOLATILITYSOURCE_SETTINGS	_T("SOFTWARE\\Egar\\ETS\\VolatilitySource")

#define    XML_PROVIDER_SETTINGS			_T("ETS\\PriceProvider")
#define    XML_IV_SETTINGS					_T("ETS\\IVFeed")
#define    XML_VOLATILITYSOURCE_SETTINGS	_T("ETS\\VolatilitySource")

#define    DEF_PRODUCT_KEY				_T("Key")
#define    DEF_CONNECTION_STRING		_T("")
#define    DEF_PROVIDER_TYPE			_T("Type")
#define    DEF_VOLATILITYSOURCE_TYPE	_T("Type")

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__55E23CC4_6D77_4EF3_9545_26CAFB5E7BBA__INCLUDED)

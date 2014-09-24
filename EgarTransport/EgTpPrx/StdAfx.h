// stdafx.h : include file for standard system include files,
//      or project specific include files that are used frequently,
//      but are changed infrequently

#if !defined(AFX_STDAFX_H__78CF5327_F330_418A_AD18_89176B4BECAF__INCLUDED_)
#define AFX_STDAFX_H__78CF5327_F330_418A_AD18_89176B4BECAF__INCLUDED_

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
extern CComModule _Module;

#include <string>
#include <map>
#include <set>

using namespace std;

#include <atlcom.h>
#include <egatlcom.h>

#import "..\tlb\Transport.tlb" no_namespace named_guids raw_interfaces_only rename("SendMessage", "SendMessageX")


#include <EgLib/EgLibDbg.h>
#include <EgLib/EgLibSync.h>
#include <EgLib/EgLibThread.h>
#include <EgLib/EgLibMisc.h>
#include <EgLib/EgLibStrategy.h>
#include <EgLib/EgLibDynamicGITArray.h>
using namespace EgLib;

#include "Constants.h"

#include <atlwin.h>
#define WM_ATTACH_TRANSPORT  (WM_USER + 101)
#define WM_DETTACH_TRANSPORT  (WM_USER + 102)

//Forward declarations//
class CHostProxy;
class CSessionProxy;
class CRouterProxy;
//Forward declarations//

_COM_SMARTPTR_TYPEDEF(IGlobalInterfaceTable, __uuidof(IGlobalInterfaceTable));



//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__78CF5327_F330_418A_AD18_89176B4BECAF__INCLUDED)

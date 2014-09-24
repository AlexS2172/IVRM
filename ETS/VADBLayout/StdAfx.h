// stdafx.h : include file for standard system include files,
//      or project specific include files that are used frequently,
//      but are changed infrequently

#if !defined(AFX_STDAFX_H__CEB292D1_0FB1_4D44_AA1E_8DEA1F26D46D__INCLUDED_)
#define AFX_STDAFX_H__CEB292D1_0FB1_4D44_AA1E_8DEA1F26D46D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define STRICT
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0400
#endif
#define _ATL_APARTMENT_THREADED

#include <atlbase.h>
//You may derive a class from CComModule and use it if you want to override
//something, but do not change the name of _Module
extern CComModule _Module;
#include <atlcom.h>
#include <comdef.h>


// MTS stuff
#include <mtx.h>
_COM_SMARTPTR_TYPEDEF( IObjectContext, __uuidof(IObjectContext) );


// ADO stuff
#pragma warning( disable: 4146 )
#import "C:\Program Files\Common Files\System\ado\msado25.tlb" no_namespace, named_guids, rename("EOF", "Eof")


// Helper stuff
#include <ComErrorWrapper.h>

#include <EgLib/EgLibDbg.h>
#include <EgLib/EgLibThread.h>
#include <EgLib/EgLibSync.h>
#include <EgLib/EgLibMisc.h>
using namespace EgLib;


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__CEB292D1_0FB1_4D44_AA1E_8DEA1F26D46D__INCLUDED)

// stdafx.h : include file for standard system include files,
//      or project specific include files that are used frequently,
//      but are changed infrequently

#if !defined(AFX_STDAFX_H__62651287_59AF_4F06_A69F_C54B4B749103__INCLUDED_)
#define AFX_STDAFX_H__62651287_59AF_4F06_A69F_C54B4B749103__INCLUDED_

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
#include <comutil.h>

#include <EgLib/EgLibDbg.h>
#include <EgLib/EgLibSync.h>
#include <EgLib/EgLibThread.h>
#include <EgLib/EgLibMisc.h>
#include <EgLib/EgLibReg.h>
#include <EgLib/EgLibDynamicGITArray.h>
using namespace EgLib;

#include "FixProvidersCollection.h"
extern CFixProvidersCollection g_FixProviders;

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__62651287_59AF_4F06_A69F_C54B4B749103__INCLUDED)

// stdafx.h : include file for standard system include files,
//      or project specific include files that are used frequently,
//      but are changed infrequently

#if !defined(AFX_STDAFX_H__1ED02B96_F912_4BE4_B1E6_FC995EFDDF59__INCLUDED_)
#define AFX_STDAFX_H__1ED02B96_F912_4BE4_B1E6_FC995EFDDF59__INCLUDED_

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
#include <atlcom.h>

#include <comdef.h>

#include <vector>
#include <string>
#include <map>
using namespace std;

// ADO stuff
#pragma warning( disable: 4146 )
#import "C:\Program Files\Common Files\System\ado\msado25.tlb" no_namespace, named_guids, rename("EOF", "Eof")

#include "INetPackingImpl.h"
#define BAD_DOUBLE_VALUE	-1E+308
#define BAD_LONG_VALUE		-2147483647

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__1ED02B96_F912_4BE4_B1E6_FC995EFDDF59__INCLUDED)

// stdafx.h : include file for standard system include files,
//      or project specific include files that are used frequently,
//      but are changed infrequently

#if !defined(AFX_STDAFX_H__5EAC9975_1A13_4847_A41B_6C2277DC9CD5__INCLUDED_)
#define AFX_STDAFX_H__5EAC9975_1A13_4847_A41B_6C2277DC9CD5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#	pragma warning(disable:4192)

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

#	define	ASSERT ATLASSERT

#include <vector>
#include <map>

#include <process.h>
#include <assert.h>

#	import "..\..\..\..\Services\ETXRuntime\ETXRuntime.tlb" no_namespace, named_guids
#	import "tlb\Transport.tlb" no_namespace named_guids raw_interfaces_only

#	include "common\common.h"
#	include "common\smart_ptr.h"
#	include "etx\common\etx_ex.h"

using namespace std;

#define BEGIN_PARAMS	{ HRESULT _hr = S_OK;
#define OUT_PARAM(p)	if (p)	*p = 0; else _hr = E_POINTER; 
#define OUT_PARAM_DEF(p, defaultVal)	if (p)	*p = defaultVal; else _hr = E_POINTER; 
#define OUT_PARAMBSTR(p)	if (p) { if (*p) { try { ::SysFreeString(*p); } catch(...) {} *p = 0; } }
#define OUT_PARAMVARIANT(p)	if (p)	{ ::VariantClear(p); (*p).vt = VT_EMPTY; } else _hr = E_POINTER; 
#define IN_PARAM_REF(p)		if (0 == p)	_hr = E_POINTER;
#define END_PARAMS	if (FAILED(_hr)) return _hr; }

//	Message header tag
#define MSG_HEADER_CLIENTSUBJ 	OLESTR("etxClientSubject")

//	Message business types
#define MSG_TYPE_ONLOGON	OLESTR("etxOnLogon")
#define MSG_TYPE_ONLOGOFF	OLESTR("etxOnLogoff")


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__5EAC9975_1A13_4847_A41B_6C2277DC9CD5__INCLUDED)

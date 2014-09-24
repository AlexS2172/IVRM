// stdafx.h : include file for standard system include files,
//      or project specific include files that are used frequently,
//      but are changed infrequently

#if !defined(AFX_STDAFX_H__A4467271_C6F5_4381_99A9_89E29D03D951__INCLUDED_)
#define AFX_STDAFX_H__A4467271_C6F5_4381_99A9_89E29D03D951__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define STRICT
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0400	// NT4 SP3!!!
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
#include <comdef.h>
#include <math.h>


// STL
#pragma warning(disable:4786)
#include <map>
#include <vector>
#include <algorithm>


// ADO stuff
#pragma warning( disable: 4146 )
#import "C:\Program Files\Common Files\System\ado\msado25.tlb" no_namespace, named_guids, rename("EOF", "Eof")


// VA DB Layout
#import "..\tlb\VADBLayout.tlb" no_namespace, named_guids


// Messages structure
#import "..\tlb\MsgStruct.tlb" no_namespace, named_guids
#import "..\tlb\MMW.tlb" no_namespace, named_guids


// Helper stuff
#include "..\..\EgarCommonLibrary\Include\EgLib\EgLibComError.h"
using namespace EgLib;

inline void EgarExceptionReporting(_bstr_t bsDescription)
{
	ShellExecuteW(
		0, 
		L"open", 
		L"EgSendMail.exe", 
		_bstr_t("-DS=\"") + bsDescription + _bstr_t("\n[Module: VME.exe] \""),
		NULL, 0);
};

#define REPORT_ERR_TO_MAIL(__Err)						\
	_bstr_t bsException = _bstr_t(__Err.Description()) + \
	_bstr_t("\nFile: ") + _bstr_t(__FILE__) +			\
	_bstr_t("\nFuncdname: ") + _bstr_t(__FUNCDNAME__) + \
	_bstr_t("\nLine: ") + _bstr_t(__LINE__);			\
	EgarExceptionReporting(bsException);

#define REPORT_UNHANDLED_EXCEPTION						\
	_bstr_t bsException = _bstr_t("\nFile: ") + _bstr_t(__FILE__) +			\
	_bstr_t("\nFuncdname: ") + _bstr_t(__FUNCDNAME__) + \
	_bstr_t("\nLine: ") + _bstr_t(__LINE__);			\
	EgarExceptionReporting(bsException);

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__A4467271_C6F5_4381_99A9_89E29D03D951__INCLUDED)

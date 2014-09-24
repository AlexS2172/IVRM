// stdafx.h : include file for standard system include files,
//      or project specific include files that are used frequently,
//      but are changed infrequently

#if !defined(AFX_STDAFX_H__BF3A94B7_81C7_4302_AD60_02D2AACD2FDD__INCLUDED_)
#define AFX_STDAFX_H__BF3A94B7_81C7_4302_AD60_02D2AACD2FDD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define STRICT
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0400
#endif

#define _ATL_FREE_THREADED
#pragma warning( disable : 4290 )

#include <atlbase.h>
//You may derive a class from CComModule and use it if you want to override
//something, but do not change the name of _Module

extern CComModule _Module;
#include <atlcom.h>
#include <comdef.h>
#include <atlwin.h>

#include <map>
#include <queue>
#include <string>
using namespace std;

//#include <eg_lib/egRegistry.h>
//#include <eg_lib/egMisc.h>
//#include <Adds/ComErrorWrapper.h>
//#include <csWrapper.h>
#define SECURITY_WIN32
#include <Security.h>

#include <DoubleBuffer.h>
#include <recPtr.h>
#include <egatlcom.h>

#include <EgLib/EgLibDbg.h>
#include <EgLib/EgLibSync.h>
#include <EgLib/EgLibReg.h>
#include <EgLib/EgLibthread.h>
#include <EgLib/EgLibMisc.h>
#include <EgLib/EgLibComError.h>
using namespace EgLib;

#include "EgLib/EgLibComError.h"

// {240A82B8-1403-45e8-92F6-E255C5DBF6E7}
static const GUID CATID_PriceProviders = 
{ 0x240a82b8, 0x1403, 0x45e8, { 0x92, 0xf6, 0xe2, 0x55, 0xc5, 0xdb, 0xf6, 0xe7 } };

#include "PriceProviders.h"
#include "ProvidersCollection.h"
#include <Process.h>
extern CProvidersCollection g_Providers;

_COM_SMARTPTR_TYPEDEF(IPriceProvider, __uuidof(IPriceProvider));
_COM_SMARTPTR_TYPEDEF(_IPriceProviderEvents, __uuidof(_IPriceProviderEvents));
_COM_SMARTPTR_TYPEDEF(IStructureProvider, __uuidof(IStructureProvider));
_COM_SMARTPTR_TYPEDEF(_IStructureProviderEvents, __uuidof(_IStructureProviderEvents));
_COM_SMARTPTR_TYPEDEF(_IStructureProviderExEvents, __uuidof(_IStructureProviderExEvents));
_COM_SMARTPTR_TYPEDEF(_IMyTrackStructureInfoEvents, __uuidof(_IMyTrackStructureInfoEvents));
_COM_SMARTPTR_TYPEDEF(_IPriceInfoEvents, __uuidof(_IPriceInfoEvents));
_COM_SMARTPTR_TYPEDEF(IProviderInfo, __uuidof(IProviderInfo));
_COM_SMARTPTR_TYPEDEF(IStructureProvider2, __uuidof(IStructureProvider2));

typedef EgLib::CComRecPtr<QuoteUpdateInfo, &LIBID_PRICEPROVIDERSLib>	_QuoteUpdateInfo;
typedef EgLib::CComRecPtr<QuoteUpdateParams, &LIBID_PRICEPROVIDERSLib >	_QuoteUpdateParams;
typedef EgLib::CComRecPtr<QuoteUpdateResult, &LIBID_PRICEPROVIDERSLib>	_QuoteUpdateResult;
typedef EgLib::CComRecPtr<StockResults, &LIBID_PRICEPROVIDERSLib>		_StockResults;
typedef EgLib::CComRecPtr<StockResultsEx, &LIBID_PRICEPROVIDERSLib>		_StockResultsEx;
typedef EgLib::CComRecPtr<StockInfo, &LIBID_PRICEPROVIDERSLib>			_StockInfo;
typedef EgLib::CComRecPtr<OptionResults, &LIBID_PRICEPROVIDERSLib>		_OptionResults;
typedef EgLib::CComRecPtr<OptionResultsEx, &LIBID_PRICEPROVIDERSLib>		_OptionResultsEx;
typedef EgLib::CComRecPtr<OptionInfo, &LIBID_PRICEPROVIDERSLib>		_OptionInfo;
typedef EgLib::CComRecPtr<StockParams, &LIBID_PRICEPROVIDERSLib>		_StockParams;
typedef EgLib::CComRecPtr<OptionParams, &LIBID_PRICEPROVIDERSLib>		_OptionParams;
typedef EgLib::CComRecPtr<FutureParams, &LIBID_PRICEPROVIDERSLib>		_FutureParams;
typedef EgLib::CComRecPtr<FutureRootParams, &LIBID_PRICEPROVIDERSLib>	_FutureRootParams;
typedef EgLib::CComRecPtr<FuturesOptionParams, &LIBID_PRICEPROVIDERSLib>	_FuturesOptionParams;
typedef EgLib::CComRecPtr<FutureResults, &LIBID_PRICEPROVIDERSLib>		_FutureResults;
typedef EgLib::CComRecPtr<FutureResultsEx, &LIBID_PRICEPROVIDERSLib>		_FutureResultsEx;
typedef EgLib::CComRecPtr<FuturesOptionResults, &LIBID_PRICEPROVIDERSLib>	_FuturesOptionResults;
typedef EgLib::CComRecPtr<FuturesOptionResultsEx, &LIBID_PRICEPROVIDERSLib>	_FuturesOptionResultsEx;
typedef EgLib::CComRecPtr<QuoteUpdateFullInfo, &LIBID_PRICEPROVIDERSLib>	_QuoteUpdateFullInfo;

long _bstr_cmp(const _bstr_t& cmp1, const _bstr_t& cmp2);

//////////////////////////////////////////////////////////////////////////////
#ifdef _LOGTOFILE

#include <egstdlib\egstdlib70.h>
#include <egstdlib\trace.h>
#define BEGIN_TRACE() 	CAutoLock _lock(&g_TraceLock); 	\
	if (!g_TraceCount) { EgStd::g_pTrace = new EgStd::CEgTraceManager; \
	EgStd::g_pTrace->Init(); } g_TraceCount++;
#define END_TRACE()	CAutoLock _lock(&g_TraceLock); g_TraceCount--; \
	if (!g_TraceCount) { EgStd::g_pTrace->Exit(); \
	delete EgStd::g_pTrace; EgStd::g_pTrace = NULL; }
extern class CPriceTracer	{ public: \
	void operator () ( void* pClass, const TCHAR* pszFmt, ...) { va_list ptr; va_start(ptr, pszFmt); \
	EgStd::g_pTrace->Trace(enlogNone, L"PriceProviders_" + _bstr_t(long(pClass)), pszFmt, ptr); va_end(ptr); }} g_Tracer;
extern CLock g_TraceLock; 
extern long g_TraceCount;
#define PRICETRACE g_Tracer

#else // _LOGTOFILE

#define BEGIN_TRACE()
#define END_TRACE()
extern class CPriceTracer	{ public: \
	void operator () ( void* pClass, const TCHAR* pszFmt, ...) { }} g_Tracer;
#define PRICETRACE if (0) g_Tracer

#endif // _LOGTOFILE

#define IMPLEMENT_PRICETRACE() CPriceTracer g_Tracer;	\
	CLock g_TraceLock; long g_TraceCount = 0;

//#define DEF_GLOBAL_MUTEX_NAME "Global\\C5E2FA75-83F2-476c-8158-166B884B0175"
//#define DEF_GLOBAL_MUTEX_NAME "C5E2FA75-83F2-476c-8158-166B884B0175"

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.


#endif // !defined(AFX_STDAFX_H__BF3A94B7_81C7_4302_AD60_02D2AACD2FDD__INCLUDED)

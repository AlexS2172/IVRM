// stdafx.h : include file for standard system include files,
//      or project specific include files that are used frequently,
//      but are changed infrequently

#if !defined(AFX_STDAFX_H__DE7825F6_03F9_418B_808B_37397AD1EE56__INCLUDED_)
#define AFX_STDAFX_H__DE7825F6_03F9_418B_808B_37397AD1EE56__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define STRICT
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0400
#endif
#define _ATL_FREE_THREADED

#include <vector>
#include <list>
#include <queue>
#include <map>
#include <set>
using namespace std;

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

	typedef map<char, char> _ExchangeCodes;
	_ExchangeCodes m_StockExchangeCodes;
	_ExchangeCodes m_OptionExchangeCodes;

	HRESULT LoadExchangeCodes();
	HRESULT _LoadExchangeCodes(LPCTSTR lpszKey, _ExchangeCodes& Exchanges);
};
extern CExeModule _Module;
#include <atlcom.h>

#include <comutil.h>
#include <comdef.h>

#include <math.h>

#include <eg_lib/egRegistry.h>
#include <eg_lib/egMisc.h>
#include <csWrapper.h>
#include <DoubleBuffer.h>
using namespace eg_reg;
using namespace eg_lib;
//#import <PriceProviders.tlb> no_namespace named_guids

#pragma comment(lib,"..\\lib\\ATFinancial\\atcapi.lib")
#pragma comment(lib,"..\\lib\\ATFinancial\\atfi32.lib")
#pragma comment(lib,"..\\lib\\ATFinancial\\qsdskapi.lib")
#pragma comment(lib,"..\\lib\\ATFinancial\\atdmapi.lib")

#include <atfi.h>

#define DEF_PATH				_T("SOFTWARE\\eGAR\\{CE5C875E-B734-42af-BC85-D14077F8D3F9}")
#define ATF_REGISTRY_ROOT		_T("SOFTWARE\\eGAR\\Providers\\{4A6B9DEE-CA83-44BB-AA41-5C4BD0B0D4E8}")
#define ATF_LOG_LEVEL_SETTING	_T("LogLevel")
#define EXCHANGECODES_SETTINGS_ROOT	ATF_REGISTRY_ROOT
#define STOCKEXCHANGECODES_SETTINGS _T("StockExchangeCodes")
#define OPTIONEXCHANGECODES_SETTINGS _T("OptionExchangeCodes")

#include "PriceProviders.h"
// {240A82B8-1403-45e8-92F6-E255C5DBF6E7}
static const GUID CATID_PriceProviders = 
{ 0x240a82b8, 0x1403, 0x45e8, { 0x92, 0xf6, 0xe2, 0x55, 0xc5, 0xdb, 0xf6, 0xe7 } };

#define DEF_PROVIDER_ID    3L
#define DEF_PROVIDER_NAME  L"AT Financial Price Provider" 

_bstr_t GetATStockExchange(char cExchange);
_bstr_t GetATOptionExchange(char cExchange);

#define C2BSTR(a) FromCharToBSTR(a)
inline _bstr_t FromCharToBSTR(char c) { WCHAR pBuf[2] = {c, 0}; return pBuf; }

/////////////////////////////////////////////////////////////////////////////
#define BAD_DOUBLE_VALUE -1E+308
#define BAD_LONG_VALUE   -2147483647

inline double SetDoubleValue(double Val)
{
	return  Val >= .01 ? Val : BAD_DOUBLE_VALUE;
}

inline bool IsInvalidDoubleValue(double Val)
{
	return  Val == BAD_DOUBLE_VALUE;
}

/////////////////////////////////////////////////////////////////////////////
#ifdef _LOGTOFILE

#include <egstdlib\egstdlib7.h>
#include <egstdlib\trace.h>
#define BEGIN_TRACE() 	bool _bOwnMng = false;	\
	if (!EgStd::g_pTrace) { EgStd::g_pTrace = new CAtfTraceManager; \
	EgStd::g_pTrace->Init(); _bOwnMng = true; }
#define END_TRACE()	if (_bOwnMng) { EgStd::g_pTrace->Exit(); \
	delete EgStd::g_pTrace; EgStd::g_pTrace = NULL; }
#define ATFPROVTRACE AtfTrace<enlogDebug>
#define ATF_SUBLEVEL 4
#define ATFPROVTRACE4 AtfTrace<ATF_SUBLEVEL>

class CAtfTraceManager : public EgStd::CEgTraceManager
{
public:
	CAtfTraceManager() : EgStd::CEgTraceManager()
	{
	}
protected:
	DWORD WINAPI ReadTraceSettings()
	{
		DWORD dwRes = EgStd::CEgTraceManager::ReadTraceSettings();
		if (dwRes == ERROR_SUCCESS)
		{
			CEgRegKey key;
			dwRes = key.Open(HKEY_LOCAL_MACHINE, ATF_REGISTRY_ROOT, KEY_READ);
			if (dwRes == ERROR_SUCCESS)
			{
				DWORD dwValue = enlogDebug;
				dwRes = key.QueryValue(dwValue, ATF_LOG_LEVEL_SETTING);
				if (dwRes == ERROR_SUCCESS)
				{
					m_ulMinLogLevel = dwValue;
				}

				key.Close();
			}
		}

		return dwRes;
	}
};

#else // _LOGTOFILE

#define BEGIN_TRACE()
#define END_TRACE()
#define ATFPROVTRACE if (0) _Terminator3
#define ATFPROVTRACE4 if (0) _Terminator3
inline void _Terminator3(...) { }

#endif // _LOGTOFILE

/////////////////////////////////////////////////////////////////////////////
#ifdef _LOGTOFILE
template<long loglevel>
inline void AtfTrace( const TCHAR* pszCategory, const TCHAR* pszFmt, ...)
{
	CAtfTraceManager* tracer = reinterpret_cast<CAtfTraceManager*>(EgStd::g_pTrace);
	va_list ptr;
	va_start(ptr, pszFmt);
	tracer->Trace(loglevel, pszCategory, pszFmt, ptr); 
	va_end(ptr);
}
#endif // _LOGTOFILE

/////////////////////////////////////////////////////////////////////////////
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__DE7825F6_03F9_418B_808B_37397AD1EE56__INCLUDED)

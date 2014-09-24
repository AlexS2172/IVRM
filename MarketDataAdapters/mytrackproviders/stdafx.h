// stdafx.h : include file for standard system include files,
//      or project specific include files that are used frequently,
//      but are changed infrequently

#if !defined(AFX_STDAFX_H__B86F0A60_42ED_41F9_AB72_0B65718F200F__INCLUDED_)
#define AFX_STDAFX_H__B86F0A60_42ED_41F9_AB72_0B65718F200F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define STRICT
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0400
#endif
#define _ATL_FREE_THREADED
#define _NO_CANCEL_ERROR

#include <map>
using namespace std;

#include <atlbase.h>
//You may derive a class from CComModule and use it if you want to override
//something, but do not change the name of _Module
class CMyTrackCore;

class CExeModule : public CComModule
{
public:
	LONG Unlock();
	DWORD dwThreadID;
	HANDLE hEventShutdown;
	void MonitorShutdown();
	bool StartMonitor();
	bool bActivity;
	CMyTrackCore *GetMyTrackCore()
	{
		return pCore;
	}
	HRESULT Init(_ATL_OBJMAP_ENTRY* p, HINSTANCE h, const GUID* plibid = NULL);
	void Term();

	HRESULT RegisterServer(BOOL bRegTypeLib = FALSE, const CLSID* pCLSID = NULL);
	HRESULT UnregisterServer(BOOL bUnRegTypeLib, const CLSID* pCLSID = NULL);

	typedef map<char, char> _ExchangeCodes;
	_ExchangeCodes m_StockExchangeCodes;
	_ExchangeCodes m_OptionExchangeCodes;

	HRESULT LoadExchangeCodes();
	HRESULT _LoadExchangeCodes(LPCTSTR lpszKey, _ExchangeCodes& Exchanges);

protected:
	CMyTrackCore *pCore;
};
extern CExeModule _Module;
#include <atlcom.h>
#include <comutil.h>
#include <comdef.h>

#include <process.h>
#include <MyTrack.h>
#include <MyTrack_func.h>
#include <eg_lib/egmisc.h>
#include <eg_lib/egRegistry.h>
#include <csWrapper.h>
#include "PriceProviders.h"

#define DEF_PATH			_T("SOFTWARE\\eGAR\\{FD68F05A-247C-4D19-B12D-852691429151}")
#define EXCHANGECODES_SETTINGS_ROOT	_T("SOFTWARE\\eGAR\\Providers\\{6A80D291-101F-49C0-B91D-A19ECBC14D62}")
#define STOCKEXCHANGECODES_SETTINGS _T("StockExchangeCodes")
#define OPTIONEXCHANGECODES_SETTINGS _T("OptionExchangeCodes")

// {240A82B8-1403-45e8-92F6-E255C5DBF6E7}
static const GUID CATID_PriceProviders = 
{ 0x240a82b8, 0x1403, 0x45e8, { 0x92, 0xf6, 0xe2, 0x55, 0xc5, 0xdb, 0xf6, 0xe7 } };

#define DEF_PROVIDER_ID 0L
#define DEF_PROVIDER_NAME L"ProTrack Price Provider"

char GetMTStockExchange(char cExchange);
char GetMTOptionExchange(char cExchange);

#define BAD_DOUBLE_VALUE -1E+308
#define BAD_LONG_VALUE   -2147483647

inline double SetDoubleValue(double Val)
{
	return  Val > .051 ? Val : BAD_DOUBLE_VALUE;
}

inline bool IsInvalidDoubleValue(double Val)
{
	return  Val == BAD_DOUBLE_VALUE;
}

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__B86F0A60_42ED_41F9_AB72_0B65718F200F__INCLUDED)

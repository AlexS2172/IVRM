// stdafx.h : include file for standard system include files,
//      or project specific include files that are used frequently,
//      but are changed infrequently

#if !defined(AFX_STDAFX_H__2175418C_73D3_48DD_AC42_C54052179F12__INCLUDED_)
#define AFX_STDAFX_H__2175418C_73D3_48DD_AC42_C54052179F12__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define STRICT
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0400
#endif
#define _ATL_APARTMENT_THREADED

#include <vector>
#include <list>
#include <queue>
#include <map>
#include <set>
#include <string>
#include <hash_map>
#include <hash_set>
using namespace std;
using namespace stdext;
#include <atlbase.h>
#include <atlcom.h>
#include <crtdbg.h>
#include <atlutil.h>
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
	
	typedef std::map<char, char> _ExchangeCodes;
	_ExchangeCodes m_StockExchangeCodes;
	_ExchangeCodes m_OptionExchangeCodes;


	HRESULT LoadExchangeCodes();
	HRESULT LoadProviderSettings();

	const DWORD GetBatchNotificationDelay() const {return m_dwBatchNotificationDelay;}
	const bool  IsAutorestartEnabled()		const {return m_dwInterval>0 && m_dwStartTime>0 && m_dwStopTime>0; }
	const DWORD GetAutorestartBeginTime()	const {return m_dwStartTime;             }
	const DWORD GetAutorestartEndTime()		const {return m_dwStopTime;              }
	const DWORD AutorestartInterval()		const {return m_dwInterval;				 }
	const DWORD GetMaxQueueSize()           const {return m_dwMaxQueueSize;          }
	const long  GetThreadPoolCapacity()     const {return m_lThreadPoolCapacity;	 }


#ifdef __PERFTEST   
	const DWORD SimulatedDataflow()         const {return m_dwSimulatedDataFlow; } 
#endif // __PERFTEST
private:
	HRESULT _LoadExchangeCodes(LPCTSTR lpszKey, _ExchangeCodes& Exchanges);

private:
	DWORD   m_dwBatchNotificationDelay;
	DWORD   m_dwStartTime;
	DWORD   m_dwStopTime;
	DWORD   m_dwInterval;
	DWORD   m_dwMaxQueueSize;
	long    m_lThreadPoolCapacity;

#ifdef __PERFTEST
	DWORD m_dwSimulatedDataFlow;
#endif // __PERFTEST

};
extern CExeModule _Module;



#include <comutil.h>
#include <comdef.h>

#include <float.h>
#include <math.h>

#include <edbx.h>
#include <etix.h>

#include <dbpblk.h>
#include <pcqw32.h>
#pragma comment(lib, "PcqW32.lib")

#include "PriceProviders.h"

#include <EgLib/EgLibDbg.h>
#include <EgLib/EgLibSync.h>
#include <EgLib/EgLibThread.h>
#include <EgLib/EgLibMisc.h>
#include <EgLib/EgLibReg.h>
#include <EgLib/EgLibException.h>
#include <EgLib/EgLibStrategy.h>
#include <EgLib/EgLibComError.h>
#include <EgLib/EgLibMath.h>
#include <EgLib/EgLibTrace.h>
using namespace EgLib;

#include <EgStdLib/EgStdLib7.h>

#include <boost/smart_ptr.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/bind.hpp>

#include <EgStdLib/EgStdLib7.h>
#include "HyperFeedTrace.h"
//#include <EgStdLib/Trace.h>

// {240A82B8-1403-45e8-92F6-E255C5DBF6E7}
static const GUID CATID_PriceProviders = 
{ 0x240a82b8, 0x1403, 0x45e8, { 0x92, 0xf6, 0xe2, 0x55, 0xc5, 0xdb, 0xf6, 0xe7 } };

#define DEF_PROVIDER_ID    1L
#define DEF_PROVIDER_NAME  L"HyperFeed Price Provider" 
#define PROVIDER_SETTINGS_ROOT	_T("SOFTWARE\\eGAR\\Providers\\{42FE7587-9CB4-46A8-A2BB-3FAED4B00840}")
#define STOCKEXCHANGECODES_SETTINGS _T("StockExchangeCodes")
#define OPTIONEXCHANGECODES_SETTINGS _T("OptionExchangeCodes")

#define MAX_TICKER 12
#define MAX_DESCRIPTION 36
#define HFC_TIMEOUT 30000
#define MAX_LENGTH 128
#define MAX_FUTURENAME 32
#define MAX_COUNTRY 2
#define MAX_CALENDARS 24

char GetHFStockExchange(char cExchange);
char GetHFOptionExchange(char cExchange);
char GetETSStockExchange(char cExchange);
char GetETSOptionExchange(char cExchange);

#define C2BSTR(a) FromCharToBSTR(a)
inline _bstr_t FromCharToBSTR(char c) { WCHAR pBuf[2] = {c, 0}; return pBuf; }

long _bstr_cmp(const _bstr_t& cmp1, const _bstr_t& cmp2);

//////////////////////////////////////////////////////////////////////////////
#define BAD_DOUBLE_VALUE -1E+308
#define BAD_LONG_VALUE   -2147483647

inline double SetDoubleValue(double Val)
{
	return  Val >= .0001 ? Val : BAD_DOUBLE_VALUE;
}
inline long InitByLong(long lValue)
{
	if (lValue == -1L)
		return BAD_LONG_VALUE;
	else
		return lValue;
}
inline bool IsInvalidDoubleValue(double Val)
{
	return  Val == BAD_DOUBLE_VALUE;
}

#ifdef __PERFMON
	#import "libid:15499058-814D-4ABD-AC1A-70161F4B1695" rename_namespace("PerfMon")
#endif
//#define DLLS_CALLS_SYNCHRONIZE
extern CHandle g_hMultilock;

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__2175418C_73D3_48DD_AC42_C54052179F12__INCLUDED)

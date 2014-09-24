#include "stdafx.h"

#pragma hdrstop

#include "Trace.h"
#include "Lock.h"
#include "Win32Error.h"
#include "io.h"

CLock g_csLog;
//#define LOG_FILENAME	   	      "Logs/SageTrace.log"

#define LOG_FILE_BEGIN     _T("Logs/STrace")
#define LOG_FILE_END       _T(".log")


BOOL	g_bDailyLog = FALSE;
BOOL	g_bEnableConsoleLog = TRUE;
BOOL	g_bEnableFileLog = TRUE;

void APIENTRY TraceInit()
{
	CEgRegKey key;

	LONG lResult = key.Open(HKEY_CURRENT_USER, TRACE_REGISTRY_KEY, KEY_READ);
	if (lResult != ERROR_SUCCESS )
		lResult = key.Open (HKEY_LOCAL_MACHINE, TRACE_REGISTRY_KEY, KEY_READ);

	if (lResult == ERROR_SUCCESS)
	{
		key.QueryValue (g_bDailyLog, TRACE_DAILY);
		key.QueryValue (g_bEnableConsoleLog, TRACE_TO_CONSOLE);
		key.QueryValue (g_bEnableFileLog, TRACE_TO_FILE);
		key.Close();
	}
}


void APIENTRY Trace(const TCHAR * lpszMessage, ...)
{
	if (!lpszMessage)
		return;

	va_list arglist;
    va_start(arglist, lpszMessage);

	SYSTEMTIME st;
	::GetLocalTime(&st);

	TCHAR szBuf[4096] = { 0 };

	_vsntprintf(szBuf, 4096, lpszMessage, arglist);

	TCHAR szTime[100];
	_stprintf(szTime, _T("[0x%04x] %-2.2i/%-2.2i/%-4.4i %-2.2i:%-2.2i:%-2.2i "), 
		GetCurrentThreadId(), st.wDay , st.wMonth , st.wYear , st.wHour, st.wMinute, st.wSecond);

	CAutoLock cs(&g_csLog);

	if(g_bEnableConsoleLog)
	{
		_tprintf(szTime);
		_tprintf(szBuf);
		_tprintf(_T("\n"));
	}

	if(g_bEnableFileLog)
	{		
		USES_CONVERSION;

		_TCHAR pLogFile[30] = { _T("") };
		_TCHAR logDate[11] = { _T("") };		

        _stprintf(logDate, _T("_%2.2i_%2.2i_%4.4i"), st.wDay, st.wMonth, st.wYear);
				
		_tcscpy(pLogFile, LOG_FILE_BEGIN);
		_tcscat(pLogFile, logDate);
		_tcscat(pLogFile, LOG_FILE_END);        
		
		fstream fLog( T2A(pLogFile), ios::app | ios::out);
		fLog << T2A(szTime) << T2A(szBuf) << endl;
        fLog.close();
	}
}


DWORD APIENTRY TraceError(const DWORD dwErr, 
						  const TCHAR* const szDescription)
{
	CWin32Error Err(dwErr);

	Trace(_T("%s : (%d) %s"), szDescription, dwErr, (const TCHAR*)Err);

	return dwErr;
}

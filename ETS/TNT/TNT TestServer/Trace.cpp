#include "stdafx.h"

#pragma hdrstop

#include "Trace.h"
#include <iostream>
#include <iomanip>

#include <Lock.h>
#include <Win32Error.h>
#include <eg_lib\egMisc.h>


CLock g_csLog;

#define LOG_FILE_BEGIN     _T("logs\\TNT_Server")
#define LOG_FILE_     _T("\\TNT_SERVER")
#define LOG_FILE_END       _T(".log")


SYSTEMTIME g_st;
BOOL	g_bDailyLog = FALSE;
BOOL	g_bEnableConsoleLog = TRUE;
BOOL	g_bEnableFileLog = TRUE;
_bstr_t g_bsLogFileCalalog;
void APIENTRY TraceInit()
{
	ZeroMemory(&g_st, sizeof(g_st));
}
void PrepareFileName(DWORD dwDay, DWORD dwMonth, DWORD dwYear, LPTSTR pLogFile)
{
	USES_CONVERSION;

	_TCHAR logDate[22] = { _T("") };		

    _stprintf(logDate, _T("_%2.2i_%2.2i_%4.4i"), dwDay, dwMonth, dwYear);
			
	if(!g_bsLogFileCalalog.length())
		_tcscpy(pLogFile, LOG_FILE_BEGIN);
	else
	{
		_tcscpy(pLogFile, (LPCTSTR)g_bsLogFileCalalog);
		_tcscat(pLogFile, LOG_FILE_);     
	}

	_tcscat(pLogFile, logDate);
	_tcscat(pLogFile, LOG_FILE_END);        

}
void CleanOldLogFiles(long lNumOfDays)
{
	eg_lib::vt_date dt(eg_lib::vt_date::GetCurrentDate(true));
	dt -= eg_lib::vt_date_span(lNumOfDays);

	for(long i = 0; i<=5;i++)
	{
		_TCHAR pLogFile[255]= { _T("") };		
		PrepareFileName(dt.get_day() , dt.get_month(), dt.get_year(), pLogFile);
		DeleteFile(pLogFile);
		dt -= eg_lib::vt_date_span(1L);
	}
}

#ifdef  _UNICODE
#define _tcout wcout
#else
#define _tcout cout
#endif

void APIENTRY Trace(enTrace enType, LPCTSTR lpszMessage, ...)
{
	if (!lpszMessage)
		return;

	va_list arglist;
    va_start(arglist, lpszMessage);

	SYSTEMTIME st;
	::GetLocalTime(&st);

	size_t iSizeString = _tcslen(lpszMessage);
	if(!iSizeString)
		return;

	CString szBuffer;
	szBuffer.FormatV(lpszMessage,arglist);

	CAutoLock cs(&g_csLog);

	if(g_bEnableConsoleLog)
	{
		_tcout << _T("[");
		_tcout << std::setw(2)<<std::setfill(_T('0'));
		if(st.wHour>12)
			_tcout <<st.wHour-12;
		else
			_tcout << st.wHour;

		cout <<_T(":") << setw(2)<<setfill(_T('0'))<< st.wMinute << _T(":") << setw(2)<<setfill(_T('0'))<< st.wSecond;
		if(st.wHour>=12)
			_tcout << _T("PM");
		else
			_tcout << _T("AM");
		switch(enType) {
		case enTraceInfo:
			_tcout << _T("");
			break;
		case enTraceWarning:
			_tcout << _T("Warning");
			break;
		case enTraceError:
			_tcout << _T("*ERROR*");
			break;
		}
		_tcout << _T("] ");
		_tcout << (LPCTSTR)szBuffer << endl;
	}


	if( g_st.wDay != st.wDay || g_st.wMonth != st.wMonth || g_st.wYear != st.wYear)
	{
		::GetLocalTime(&g_st);
		CleanOldLogFiles(3L);
	}
	if(g_bEnableFileLog)
	{		 
		_TCHAR pLogFile[255]= { _T("") };		
		PrepareFileName(st.wDay, st.wMonth, st.wYear, pLogFile);

		fstream fLog( T2A(pLogFile), ios::app | ios::out);
		fLog << _T("[");

		if(st.wHour>12)
			fLog <<setw(2)<<setfill(_T('0'))<< st.wHour-12;
		else
			fLog <<setw(2)<<setfill(_T('0'))<< st.wHour;

		fLog <<_T(":") <<setw(2)<<setfill(_T('0'))<< st.wMinute << _T(":") <<setw(2)<<setfill(_T('0'))<< st.wSecond;
		if(st.wHour>=12)
			fLog << _T("PM");
		else
			fLog << _T("AM");

		switch(enType) {
		case enTraceInfo:
			fLog << _T("Info");
			break;
		case enTraceWarning:
			fLog << _T("Warning");
			break;
		case enTraceError:
			fLog << _T("*ERROR*");
			break;
		}
		fLog << _T("]\t");
		fLog << (LPCTSTR)szBuffer << endl;
	}
}


DWORD APIENTRY TraceError(const TCHAR* const szDescription,
						  const DWORD dwErr)
{
	CWin32Error Err(dwErr);

	Trace(enTraceError,_T("%s : (%d) %s"), szDescription, dwErr, (const TCHAR*)Err);

	return dwErr;
}

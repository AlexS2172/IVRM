#include "stdafx.h"

#pragma hdrstop

#include "Trace.h"
#include <time.h>
CLock g_csLog;

#define LOG_LIFE_DAYS 5

void EgKillOldTrace(PCTSTR pszFileName)
{
	HANDLE hFile = ::CreateFile(
		pszFileName, 
		GENERIC_READ, 
		FILE_SHARE_READ, 
		NULL, 
		OPEN_EXISTING, 
		FILE_ATTRIBUTE_NORMAL, 
		NULL);

	if (INVALID_HANDLE_VALUE != hFile)
	{
		::CloseHandle(hFile);
		return;
	}
	
	time_t t;
	time(&t);
	t = t - LOG_LIFE_DAYS * 24 * 60 * 60;
	tm* pt = localtime(&t);

	SYSTEMTIME st = {0};
	st.wYear = pt->tm_year + 1900;
	st.wMonth = pt->tm_mon + 1;
	st.wDay = pt->tm_mday;

	FILETIME ft;
	::SystemTimeToFileTime(&st, &ft);

	TCHAR szBuf[4096] = { 0 };

	DWORD dwErr = GetModuleFileName(NULL, szBuf, sizeof(szBuf));
	if(dwErr == 0)	return;
	char szDrive[_MAX_DRIVE];
	char szDir[_MAX_DIR];
	char szFname[_MAX_FNAME];
	char szExt[_MAX_EXT];
	_splitpath( szBuf, szDrive, szDir, szFname, szExt );
    char szLogsPathMask[_MAX_PATH];
	sprintf(szLogsPathMask, "%s%sLogs/iselog_*.txt", szDrive, szDir);

    WIN32_FIND_DATA fd;
    HANDLE hFind = ::FindFirstFile(szLogsPathMask, &fd);

	while (INVALID_HANDLE_VALUE != hFind)
	{
			if (0 > ::CompareFileTime(&fd.ftCreationTime, &ft))
		{
			sprintf(szLogsPathMask, "%s%sLogs/%s", szDrive, szDir, fd.cFileName);
			::DeleteFile(szLogsPathMask);
		}

		if (FALSE == ::FindNextFile(hFind, &fd))
		{
			::FindClose(hFind);
			hFind = INVALID_HANDLE_VALUE;
		}
	}
}

void EgTrace(const TCHAR * lpszMessage, ...)
{
	if(!lpszMessage)
		return;

	TCHAR szBuf[4096] = { 0 };

	DWORD dwErr = GetModuleFileName(NULL, szBuf, sizeof(szBuf));
	if(dwErr == 0)
		return;

	char szDrive[_MAX_DRIVE];
	char szDir[_MAX_DIR];
	char szFname[_MAX_FNAME];
	char szExt[_MAX_EXT];
	_splitpath( szBuf, szDrive, szDir, szFname, szExt );

	SYSTEMTIME st;
	::GetLocalTime(&st);

	char szLogsPath[_MAX_PATH];
	sprintf(szLogsPath, "%s%sLogs/iselog_%4.4i_%2.2i_%2.2i.txt", szDrive, szDir, st.wYear, st.wMonth, st.wDay);

	EgKillOldTrace(szLogsPath);

	fstream fLog(szLogsPath, ios::app | ios::out | 0x20);
	
	va_list arglist;
    va_start(arglist, lpszMessage);
    
	_vsntprintf(szBuf, 4096, lpszMessage, arglist);

	TCHAR szTime[100];
	_stprintf(szTime, _T("[0x%04x]%-2.2i:%-2.2i:%-2.2i "), GetCurrentThreadId(), st.wHour, st.wMinute, st.wSecond);

	CAutoLock cs(&g_csLog);

	fLog << szTime << szBuf << endl;
}

void EgTrace2(const TCHAR * lpszMessage)
{
	if(!lpszMessage)
		return;

	TCHAR szBuf[4096] = { 0 };

	DWORD dwErr = GetModuleFileName(NULL, szBuf, sizeof(szBuf));
	if(dwErr == 0)
		return;

	char szDrive[_MAX_DRIVE];
	char szDir[_MAX_DIR];
	char szFname[_MAX_FNAME];
	char szExt[_MAX_EXT];
	_splitpath( szBuf, szDrive, szDir, szFname, szExt );

	SYSTEMTIME st;
	::GetLocalTime(&st);

	char szLogsPath[_MAX_PATH];
	sprintf(szLogsPath, "%s%sLogs/iselog_%4.4i_%2.2i_%2.2i.txt", szDrive, szDir, st.wYear, st.wMonth, st.wDay);

	EgKillOldTrace(szLogsPath);
	
	fstream fLog(szLogsPath, ios::app | ios::out);
	
	TCHAR szTime[100];
	_stprintf(szTime, _T("[0x%04x]%-2.2i:%-2.2i:%-2.2i "), GetCurrentThreadId(), st.wHour, st.wMinute, st.wSecond);

	CAutoLock cs(&g_csLog);
	fLog << szTime << lpszMessage << endl;
}

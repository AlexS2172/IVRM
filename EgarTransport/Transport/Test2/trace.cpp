#include "stdafx.h"

#include "trace.h"
#include "lock.h"
#include <fstream.h>

CLock g_csLog;
#define LOG_FILENAME		"Logs/Test2.log"

DWORD	CheckLogsFolder()
{
	char szBuf[_MAX_PATH];
	DWORD dwErr = GetModuleFileName(NULL, szBuf, sizeof(szBuf));
	if(dwErr == 0)
		return GetLastError();

	char szDrive[_MAX_DRIVE];
	char szDir[_MAX_DIR];
	char szFname[_MAX_FNAME];
	char szExt[_MAX_EXT];
	_splitpath( szBuf, szDrive, szDir, szFname, szExt );

	char szLogsPath[_MAX_PATH];
	char szCurrentPath[_MAX_PATH];
	sprintf(szLogsPath, "%s%sLogs", szDrive, szDir);
	sprintf(szCurrentPath, "%s%s", szDrive, szDir);

	BOOL bRes = CreateDirectory(szLogsPath, NULL);
	if(!bRes)
	{
		dwErr = GetLastError();
		if(ERROR_ALREADY_EXISTS != dwErr)
			return dwErr;
	}

	bRes = SetCurrentDirectory(szCurrentPath);
	if(!bRes)
		return GetLastError();

	return 0;
}


void Trace(const TCHAR * lpszMessage, ...)
{
	if(!lpszMessage)
		return;

	va_list arglist;
    va_start(arglist, lpszMessage);

	SYSTEMTIME st;
	::GetLocalTime(&st);
	TCHAR szBuf[4096] = { 0 };

	_vsntprintf(szBuf, 4096, lpszMessage, arglist);

	TCHAR szTime[100];
	_stprintf(szTime, _T("[0x%04x]%-2.2i:%-2.2i:%-2.2i "), GetCurrentThreadId(), st.wHour, st.wMinute, st.wSecond);

	CReentry cs(&g_csLog);

	_tprintf(szTime);
	_tprintf(szBuf);
	_tprintf(_T("\n"));
	
	fstream fLog(LOG_FILENAME, ios::app | ios::out);
	fLog << szTime << szBuf << endl;
}
#include "stdafx.h"
#include "trace.h"
#include <iomanip>

CLock g_csLog;
fstream fLog;

void InitMainLog()
{
	char Path[MAX_PATH] = {0};	
	::GetModuleFileName(NULL, Path, MAX_PATH);

	for (int Counter = strlen(Path); Counter != 0 && Path[Counter] != '\\'; Counter--);
	memset(Path + Counter, 0, strlen(Path) - Counter);

	std::string LogPath(Path);
	std::string Slash("\\logs\\focconnector.log");

	LogPath += Slash;

	fLog.open(LogPath.c_str(), ios::app | ios::out);
}

void Trace(const TCHAR * lpszMessage, ...)
{
	if(!lpszMessage)
		return;

	va_list arglist;
    va_start(arglist, lpszMessage);

	SYSTEMTIME st;
	::GetLocalTime(&st);
	char szBuf[4096] = { 0 };

	_vsntprintf(szBuf, 4096, lpszMessage, arglist);

	CAutoLock Lock(&g_csLog);

	cout	<< std::setfill('0') << std::setw(2) <<  st.wDay << "/"
			<< std::setfill('0') << std::setw(2) <<  st.wMonth << "/"
			<< st.wYear << " "
			<< std::setfill('0') << std::setw(2) <<  st.wHour << ":"
			<< std::setfill('0') << std::setw(2) <<  st.wMinute << ":"
			<< std::setfill('0') << std::setw(2) <<  st.wSecond << " "
			<< szBuf << endl;

	fLog	<< std::setfill('0') << std::setw(2) <<  st.wDay << "/"
			<< std::setfill('0') << std::setw(2) <<  st.wMonth << "/"
			<< st.wYear << " "
			<< std::setfill('0') << std::setw(2) <<  st.wHour << ":"
			<< std::setfill('0') << std::setw(2) <<  st.wMinute << ":"
			<< std::setfill('0') << std::setw(2) <<  st.wSecond << " "
			<< szBuf << endl;
}

DWORD TraceError(const DWORD dwErr, const TCHAR* const szDescription)
{
	CWin32Error Err(dwErr);

	Trace(_T("%s : (%d) %s"), szDescription, dwErr, (const TCHAR*)Err);

	return dwErr;
}

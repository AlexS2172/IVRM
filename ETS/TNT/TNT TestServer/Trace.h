#ifndef TRACE_H
#define TRACE_H

extern	BOOL	g_bDailyLog;
extern	BOOL	g_bEnableConsoleLog;
extern	BOOL	g_bEnableFileLog;

#define TRACE_DAILY					_T("LogDaily")
#define TRACE_TO_CONSOLE			_T("LogToConsole")
#define TRACE_TO_FILE				_T("LogToFile")
#define TRACE_FILE_PATH				_T("LogPath")

enum enTrace { enTraceInfo, enTraceWarning, enTraceError};

void APIENTRY TraceInit();

void APIENTRY Trace(enTrace enType, LPCTSTR lpszMessage, ...);

DWORD APIENTRY TraceError(const TCHAR* const = _T("Internal error"),
						  const DWORD dwErr = GetLastError());

#endif
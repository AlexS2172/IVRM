#ifndef __TRACE_H__
#define __TRACE_H__

//////////////////////////////////////////////////////////////////////////////////
#define TRACE_REGISTRY_KEY			_T("Software\\Egar\\MainLine\\Sage")
#define TRACE_DAILY					_T("LogDaily")
#define TRACE_TO_CONSOLE			_T("LogToConsole")
#define TRACE_TO_FILE				_T("LogToFile")

//////////////////////////////////////////////////////////////////////////////////
class CPublisher;
class CTracer
{	
public:
	static void __stdcall CreateFileName();
	static BOOL __stdcall IsDateChanged();
	static void __stdcall Trace(const TCHAR* lpszMessage, ...);
	static DWORD __stdcall TraceError(const DWORD dwErr = GetLastError(), 
						  const TCHAR* const = _T("Internal error"));
	static void __stdcall TraceInit();
	static void SetPublisher(CPublisher* pPublisher);

private:
	static CPublisher*	m_pPublisher;

	static TCHAR	m_pFileName[_MAX_FNAME];
	static ULONG	m_nFileDate;
	
	static BOOL	m_bEnableConsoleLog;
	static BOOL	m_bEnableFileLog;
	static BOOL	m_bDailyLog;
};

//////////////////////////////////////////////////////////////////////////////////
#endif //__TRACE_H__
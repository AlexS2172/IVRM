#ifndef EGTRACE_H
#define EGTRACE_H

#include <EgStdLib\EgStdLibCommon.h>
#include <EgLib/EgLibSync.h>
#include <EgLib/EgLibThread.h>
#include <EgLib/EgLibMisc.h>
using namespace EgLib;


#define SECOND_IN_DAY 60*60*24

typedef enum EGSTD_EXT_CLASS ELogLevels
{
	enlogNone		= 0,
	enlogFaults		= 1,
	enlogSystem		= 2,
	enlogWarning		= 3,
	enlogInfoExt		= 4,
	enlogInfo		= 5,
	enlogSubsExt		= 6,
	enlogSubs		= 7,
	enlogCustomExt		= 8,
	enlogCustom		= 9,
	enlogDebug		= 10,
	enlogAll		= enlogDebug,
} ELogLevels;

namespace EgStd
{

class EGSTD_EXT_CLASS CEgTraceManager
{
public:
	CEgTraceManager(	LPCTSTR pstrLogFileNamePrefix = _T(""),
					LPCTSTR pstrLogDirectory = _T("")) :
											m_hThread(NULL),
											m_hStopEvent(NULL),
											m_hDailyTimer(NULL),
											m_strLogDirectory(pstrLogDirectory),
											m_strLogFileNamePrefix(pstrLogFileNamePrefix),
											m_strCurrentLogFile(_T(""))
	{
	}
	  
	virtual ~CEgTraceManager()
	{
	}

	virtual bool WINAPI	Init();
	virtual void WINAPI	Exit();
	
	virtual bool WINAPI CheckLevel(long logLevel);

	virtual void WINAPI	Trace(	long			lLogLevel, 
							const TCHAR*	lpszCategory, 
							const TCHAR*	lpszMessage, 
							va_list			argptr);

	virtual void WINAPI	TraceSystemInfo();
	virtual void WINAPI	TraceProgramInfo();
	

protected:
	virtual DWORD WINAPI	TraceFileInit();
	virtual DWORD WINAPI	ReadTraceSettings();

	void WINAPI			WriteString(LPCTSTR lpString);

	//////thread staff///
	
	bool WINAPI			QueueSetDailyTimer();

	static unsigned int __stdcall ThreadFunction(void* pVoid)
	{
		CEgTraceManager*		pThis = (CEgTraceManager*)pVoid;
		pThis->Do();
		return 0;
	}

	virtual void WINAPI	Do();

	HANDLE					m_hThread;
	HANDLE					m_hStopEvent;
	HANDLE					m_hDailyTimer;
	CLock					m_LockMembers;	//Locked access to class members
		
	/////////Log File Members//////////
	tfstream				m_fLog;
	
	CLock					m_LogLock;		//Locked access to log file

	tstring					m_strLogDirectory;
	tstring					m_strLogFileNamePrefix;
	tstring					m_strCurrentLogFile;

	u_long					m_ulDaysInHistory;
public:
	u_long					m_ulMinLogLevel;
};
extern EGSTD_EXT_DATA CEgTraceManager* g_pTrace;

class EGSTD_EXT_CLASS CEgTracingClass
{
public:
	CEgTracingClass(){};
protected:
	void WINAPI		Trace(long lLogLevel, const TCHAR* lpszCategory, const TCHAR* lpszMessage, ...);
public:	
	static void WINAPI TraceStatic(long lLogLevel, const TCHAR* lpszCategory, const TCHAR* lpszMessage, ...);
	static long WINAPI GetMinLogLevel();
	static void WINAPI SetMinLogLevel(long lLevel);

};


};

#endif // EGTRACE_H
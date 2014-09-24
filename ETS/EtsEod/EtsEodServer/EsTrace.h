#pragma once
#ifndef __ESTRACE_H__
#define __ESTRACE_H__

#include <io.h>
#include <iostream>
#include <time.h>
#include <atlbase.h>
#include <fstream>

#include "EodGeneral.h"

namespace EODCO
{

enum TrLogLevelEnum
{
	enTrLogNone = 0,
	enTrLogFaults,
	enTrLogSystem,
	enTrLogError,
	enTrLogWarning,
	enTrLogLevel5,
	enTrLogLevel6,
	enTrLogLevel7,
	enTrLogLevel8,
	enTrLogLevel9,
	enTrLogDebug,
	enTrLogAll = enTrLogDebug
};

const unsigned long SECOND_IN_DAY = 24 * 60 * 60;

class CEgTraceManager;
typedef boost::shared_ptr<CEgTraceManager> CEgTraceManagerPtr;

class CEgTraceManager : virtual public CThread
{
public:
	CEgTraceManager() :	CThread(), m_bInitialized(false), m_hDailyTimer(NULL)
	{
	}

	virtual ~CEgTraceManager()
	{
		_TraceFileClose();
	}
	
	DWORD Start();

	DWORD Stop();

	virtual void Trace(	TrLogLevelEnum	enLogLevel, 
						LPCTSTR			lpszCategory, 
						LPCTSTR			lpszMessage, 
						va_list			argptr);
											
	virtual UINT Run();
protected:
	void _TraceFileInit() throw (exception);
	void _TraceSystemInfo();
	void _TraceProgramInfo();
	void _TraceFileClose() throw (exception);
	bool _QueueSetDailyTimer();

	void _WriteString(LPCTSTR lpString);

	/////////Log File Members//////////
	fstream					m_fLog;

	CLock					m_LockMembers;	//Locked access to class members
	CLock					m_LockLog;		//Locked access to log file

	CString					m_strLogDirectory;
	CString					m_strLogFileNamePrefix;
	CString					m_strCurrentLogFile;
	HANDLE					m_hDailyTimer;

	unsigned long			m_ulDaysInHistory;
	unsigned long			m_ulMinLogLevel;

private:
	bool					m_bInitialized;

public:
	static CEgTraceManagerPtr s_spTrace;
};

typedef boost::shared_ptr<CEgTraceManager> CEgTraceManagerPtr;

class CEgTraceable
{
public:
	CEgTraceable(){};
protected:
	void Trace(TrLogLevelEnum enLogLevel, LPCTSTR lpszCategory, LPCTSTR lpszMessage, ...) const;
	static void TraceStatic(TrLogLevelEnum enLogLevel, LPCTSTR lpszCategory, LPCTSTR lpszMessage, ...);
};

class CEgReportManager;
typedef boost::shared_ptr<CEgReportManager> CEgReportManagerPtr;

class CEgReportManager
{
public:
	CEgReportManager() : m_bInitialized(false)
	{
	}

	virtual ~CEgReportManager()
	{
	}
	void Init()
	{
		_TraceFileInit();
	}

	virtual void Report( long nEodStep, EodRpMessageTypeEnum enMessageType, LPCTSTR	lpszMessage, va_list argptr);
protected:
	virtual void _TraceFileInit() throw (exception);

	CLock					m_LockMembers;	//Locked access to class members
	CLock					m_LockLog;		//Locked access to log file

	CString					m_strLogDirectory;
private:
	bool					m_bInitialized;

public:
	static CEgReportManagerPtr s_spEodReport;
};
typedef boost::shared_ptr<CEgReportManager> CEgReportManagerPtr;

class CEgReportable
{
public:
	CEgReportable(){};
protected:
	void Report(long nEodStep, EodRpMessageTypeEnum enMessageType, LPCTSTR lpszMessage, ...) const;
};

};

#endif// __ESTRACE_H__

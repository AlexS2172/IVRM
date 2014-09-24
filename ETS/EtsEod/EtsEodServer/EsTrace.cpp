#include "StdAfx.h"
#include "EsTrace.h"
#include "EsModuleVersion.h"

namespace EODCO
{

CEgTraceManagerPtr CEgTraceManager::s_spTrace;

DWORD  CEgTraceManager::Start()
{
	{
		CAutoLock lock(m_LockMembers);
		if (m_hDailyTimer == NULL)
			m_hDailyTimer = CreateWaitableTimer(NULL, FALSE, NULL);
	}

	_TraceFileInit();
	_QueueSetDailyTimer();

	DWORD dwRes = CThread::Start();
	{
		CAutoLock lock(m_LockMembers);
		m_bInitialized = true;
	}
	return dwRes;
}

DWORD  CEgTraceManager::Stop()
{
	DWORD dwRes = CThread::Stop();

	if(m_hDailyTimer)
	{
		CloseHandle(m_hDailyTimer);
		m_hDailyTimer = NULL;
	}

	_TraceFileClose();

	return dwRes;
}

void CEgTraceManager::_TraceFileInit()
{
	_TraceFileClose();

	TCHAR szBuf[_MAX_PATH];
	unsigned long dwErr = GetModuleFileName(NULL, szBuf, sizeof(szBuf));
	if(dwErr == 0)
		throw exception(_T("Win32 error"));

	TCHAR szDrive[_MAX_DRIVE];
	TCHAR szDir[_MAX_DIR];
	TCHAR szFname[_MAX_FNAME];
	TCHAR szExt[_MAX_EXT];
	_tsplitpath_s( szBuf, szDrive, sizeof(szDrive), szDir, sizeof(szDir), szFname, sizeof(szFname), szExt, sizeof(szExt));

	TCHAR szLogsPath[_MAX_PATH];
	_stprintf_s(szLogsPath, sizeof(szLogsPath), _T("%s%sLogs"), szDrive, szDir);

	m_strLogDirectory = szLogsPath;
	m_strLogFileNamePrefix = szFname;

	m_ulDaysInHistory = DAYS_IN_HISTORY_DEFAULT;
	m_ulMinLogLevel = enTrLogNone;

	CXMLParamsHelper XMLParams;
	XMLParams.LoadXMLParams();

	long nVal = DAYS_IN_HISTORY_DEFAULT;
	XMLParams.GetXMLLong(c_strSettingsXMLKey, _T("LogHistory"), &nVal, DAYS_IN_HISTORY_DEFAULT);
	m_ulDaysInHistory = nVal;

	nVal = enTrLogNone;
	XMLParams.GetXMLLong(c_strSettingsXMLKey, _T("LogLevel"), &nVal, enTrLogNone);
	m_ulMinLogLevel = static_cast<TrLogLevelEnum>(nVal);

	CString strFileName,strFileMask;
	CString strFileDir;

	{
		CAutoLock lock(m_LockMembers);

		BOOL bRes = CreateDirectory(m_strLogDirectory, NULL);
		if(!bRes)
		{
			DWORD dwRet = GetLastError();
			if(ERROR_ALREADY_EXISTS != dwRet)
				throw exception(_T("Win32 error"));
		}
		strFileDir = m_strLogDirectory;
		strFileMask = m_strLogDirectory;
		strFileMask += _T("\\");
		strFileMask += m_strLogFileNamePrefix;
		strFileName = m_strLogFileNamePrefix;
	}

	COleDateTime dtToday = COleDateTime::GetCurrentTime();

	{
		CAutoLock lock(m_LockMembers);

		strFileMask += _T("*");

		struct _tfinddata_t fileinfo;

		intptr_t lRes = _tfindfirst(strFileMask, &fileinfo );

		// remove old log files...
		int iRes = 0;
		if(lRes != -1)
		{
			while(!iRes)
			{
				struct tm tmFile;
				localtime_s(&tmFile, &fileinfo.time_write);

				COleDateTime dtFileModified;
				dtFileModified.SetDateTime( tmFile.tm_year + 1900, tmFile.tm_mon + 1, 
													tmFile.tm_mday, tmFile.tm_hour, tmFile.tm_min, tmFile.tm_sec );

				COleDateTimeSpan dtFileAge = dtToday - dtFileModified;

				if( static_cast<unsigned long>(dtFileAge.GetSeconds()) > SECOND_IN_DAY * m_ulDaysInHistory)
				{
					strFileMask = strFileDir;
					strFileMask += _T("\\");
					strFileMask += fileinfo.name;

					_tremove(strFileMask);
				}
				iRes = _tfindnext(lRes,&fileinfo);
			}
			_findclose(lRes);
		}

		TCHAR szTime[20];
		_stprintf_s(szTime, sizeof(szTime), _T("_%-2.2i%-2.2i%-4.4i"), dtToday.GetMonth(), dtToday.GetDay(), dtToday.GetYear());

		strFileName += szTime;
		strFileName += _T(".log");
	}

	{
		CAutoLock lock(m_LockMembers);
		m_strCurrentLogFile = m_strLogDirectory;
		m_strCurrentLogFile += _T("\\");
		m_strCurrentLogFile += 	strFileName;
	}

	{
		CAutoLock lock(m_LockLog);

		m_fLog.open(m_strCurrentLogFile, ios::app | ios::out);
	}

	TCHAR szBufLog[4096] = { 0 };

	_stprintf_s(szBufLog, sizeof(szBufLog), _T("<LogSession date=\"%-2.2i/%-2.2i/%-4.4i\" time=\"%-2.2i:%-2.2i:%-2.2i\">"),dtToday.GetMonth(), dtToday.GetDay(), dtToday.GetYear(), dtToday.GetHour(), dtToday.GetMinute(), dtToday.GetSecond() );

	_WriteString(szBufLog);

	_WriteString(_T("<SystemInfo>"));

	_TraceSystemInfo();

	_WriteString(_T("</SystemInfo>"));
	_WriteString(_T("<ProgramInfo>"));

	_TraceProgramInfo();

	_WriteString(_T("</ProgramInfo>"));

	{
		CAutoLock lock(m_LockMembers);
		m_bInitialized = true;
	}
}

void  CEgTraceManager::_TraceFileClose()
{
	CAutoLock lock(m_LockMembers);

	if(!m_strCurrentLogFile.IsEmpty())
	{			
		_WriteString(_T("</LogSession>"));

		m_strCurrentLogFile = _T("");

		CAutoLock lock(m_LockLog);

		m_fLog.close();
	}
}

void   CEgTraceManager::_TraceSystemInfo()
{
	OSVERSIONINFOEX osvi;
	BOOL bOsVersionInfoEx;
	CString strOSInfo;

	// Try calling GetVersionEx using the OSVERSIONINFOEX structure.
	//
	// If that fails, try using the OSVERSIONINFO structure.

	ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX));
	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);

	if( !(bOsVersionInfoEx = GetVersionEx ((OSVERSIONINFO *) &osvi)) )
	{
		// If OSVERSIONINFOEX doesn't work, try OSVERSIONINFO.

		osvi.dwOSVersionInfoSize = sizeof (OSVERSIONINFO);
		if (! GetVersionEx ( (OSVERSIONINFO *) &osvi) ) 
			return;
	}

	strOSInfo = _T("<OperatingSystem>");
	switch (osvi.dwPlatformId)
	{
	case VER_PLATFORM_WIN32_NT:
		{
			// Test for the product.
			if ( osvi.dwMajorVersion <= 4 )
				strOSInfo += _T("Microsoft Windows NT");

			if ( osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 0 )
				strOSInfo += _T("Microsoft Windows 2000");

			if ( osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 1 )
				strOSInfo += _T("Microsoft Windows XP");

			// Test for product type.

			if( bOsVersionInfoEx )
			{
				if (osvi.wProductType == VER_NT_WORKSTATION)
				{
					if (osvi.wSuiteMask & VER_SUITE_PERSONAL)
						strOSInfo += _T(" Personal");
					else
						strOSInfo += _T(" Professional");
				}
				else if (osvi.wProductType == VER_NT_SERVER)
				{
					if (osvi.wSuiteMask & VER_SUITE_DATACENTER)
						strOSInfo += _T(" DataCenter Server");
					else if (osvi.wSuiteMask & VER_SUITE_ENTERPRISE)
						strOSInfo += _T(" Advanced Server");
					else
						strOSInfo += _T(" Server");
				}
			}
			else
			{
				CRegKey keyReg;
				CString strProdType;

				LONG lResult = keyReg.Open(HKEY_CURRENT_USER, 
					_T("SYSTEM\\CurrentControlSet\\Control\\ProductOptions"), KEY_READ);

				if (lResult == ERROR_SUCCESS)
				{
					unsigned long  ulSize = 0;
					lResult = keyReg.QueryStringValue(_T("ProductType"),NULL,&ulSize);
					if (lResult == ERROR_SUCCESS && ulSize > 0)
					{
						long nLen = ulSize / sizeof (TCHAR);
						TCHAR* szBuff = new TCHAR [nLen + 1 ];

						lResult = keyReg.QueryStringValue(_T("ProductType"), szBuff, &ulSize );
						if (lResult == ERROR_SUCCESS)
							strProdType = szBuff;
						delete [] szBuff;
					}
				}

				if (!strProdType.CompareNoCase(_T("WINNT")))
					strOSInfo += _T(" Professional");
				else if(!strProdType.CompareNoCase(_T("LANMANNT")))
					strOSInfo += _T(" Server");
				else if(!strProdType.CompareNoCase(_T("SERVERNT")))
					strOSInfo += _T(" Advanced Server");
			}

			// Display version, Server pack (if any), and build number.
			if ( osvi.dwMajorVersion <= 4 )
			{
				TCHAR OSBuf[100] = {0};
				_stprintf_s(OSBuf, sizeof(OSBuf), _T(" version %d.%d %s (Build %d)"),
					osvi.dwMajorVersion,
					osvi.dwMinorVersion,
					osvi.szCSDVersion,
					osvi.dwBuildNumber & 0xFFFF);

				strOSInfo += OSBuf;
			}
			else
			{ 
				TCHAR OSBuf[100] = {0};
				_stprintf_s (OSBuf, sizeof(OSBuf),
					_T(" %s (Build %d)"),
					osvi.szCSDVersion,
					osvi.dwBuildNumber & 0xFFFF);

				strOSInfo += OSBuf;
			}
		}
		break;
	case VER_PLATFORM_WIN32_WINDOWS:
		{
			if (osvi.dwMajorVersion == 4 && osvi.dwMinorVersion == 0)
			{
				strOSInfo += _T("Microsoft Windows 95");
				if ( osvi.szCSDVersion[1] == _T('C') || osvi.szCSDVersion[1] == _T('B') )
					strOSInfo += _T(" OSR2");
			} 
			if (osvi.dwMajorVersion == 4 && osvi.dwMinorVersion == 10)
			{
				strOSInfo += _T("Microsoft Windows 98");
				if ( osvi.szCSDVersion[1] == _T('A') )
					strOSInfo += _T(" SE");
			} 
			if (osvi.dwMajorVersion == 4 && osvi.dwMinorVersion == 90)
			{
				strOSInfo += _T("Microsoft Windows Me");
			} 
		}
		break;
	case VER_PLATFORM_WIN32s:
		strOSInfo += _T("Microsoft Win32s");
		break;
	}
	strOSInfo += _T("</OperatingSystem>");

	SYSTEM_INFO siSysInfo;   // struct. for hardware information 

	ZeroMemory(&siSysInfo, sizeof(SYSTEM_INFO));

	// Copy the hardware information to the SYSTEM_INFO structure. 
	GetSystemInfo(&siSysInfo);

	TCHAR szHWInfo[1024] = {0};
	_stprintf_s(szHWInfo, sizeof(szHWInfo), _T("<Processor Numbers=\"%d\" Architecture=\"%d\" Level=\"%d\" Revision=\"%d\"/>"), siSysInfo.dwNumberOfProcessors,
		siSysInfo.wProcessorArchitecture,siSysInfo.wProcessorLevel,siSysInfo.wProcessorRevision);

	// get memory status
	MEMORYSTATUS memstat;
	ZeroMemory(&memstat, sizeof(MEMORYSTATUS));

	GlobalMemoryStatus(&memstat);
	TCHAR szMemInfo[1024] = {0};
	_stprintf_s(szMemInfo,sizeof(szMemInfo), _T("<Memory Physical=\"%ld\" Pagefile=\"%ld\" Virtual=\"%ld\"/>"),
		memstat.dwTotalPhys/(1024*1024),
		memstat.dwTotalPageFile/(1024*1024),
		memstat.dwTotalVirtual/(1024*1024));


	TCHAR szCompName[_MAX_PATH] = {0};

	TCHAR szUserName[_MAX_PATH] = {0};

	unsigned long cchBuf = _MAX_PATH;

	GetComputerName(szCompName, &cchBuf);

	GetUserName(szUserName, &cchBuf); 

	{
		CAutoLock lock_log(m_LockLog);

		if(!m_fLog.is_open())
			return;

		TCHAR Buf[4096] = {0};

		_stprintf_s(Buf, sizeof(Buf), _T("<ComputerName>%s</ComputerName>"), szCompName);

		m_fLog << Buf << endl;

		// hardware info
		m_fLog << szHWInfo << endl;

		//memory
		m_fLog << szMemInfo << endl;

		// OS info //
		m_fLog << (LPCTSTR)strOSInfo << endl;

		_stprintf_s(Buf, sizeof(Buf), _T("<CurrentUser>%s</CurrentUser>"), szUserName);
		m_fLog << Buf << endl;
	}
}

void   CEgTraceManager::_TraceProgramInfo()
{
	TCHAR	szFileName[_MAX_PATH];
	CString sFmt, sVer = _T("N/A");

	if (GetModuleFileName(NULL, szFileName, sizeof(szFileName)))
	{
		CEgModuleVersion ver;
		ver.GetModuleVersionInfo(szFileName);

		try
		{
			_bstr_t bsVer;
			ver.GetValue(_T("FileVersion"), bsVer);

			sVer = (LPCTSTR)bsVer;
		}
		catch(_com_error er)
		{
			_WriteString(er.Description());
		}

		TCHAR szFname[_MAX_FNAME] = {0};
		TCHAR szExt[_MAX_EXT] = {0};
		_tsplitpath_s( szFileName, NULL,0, NULL,0, szFname,sizeof(szFname), szExt, sizeof(szExt));

		TCHAR Buf[4096] = {0};
		_stprintf_s(Buf, sizeof(Buf), _T("<File name=\"%s%s\" version=\"%s\"/>"), szFname,szExt,sVer);

		_WriteString(Buf);
	}
}

void   CEgTraceManager::Trace(TrLogLevelEnum	enLogLevel, 
							  LPCTSTR			lpszCategory, 
							  LPCTSTR			lpszMessage, 
							  va_list			argptr)
{
	{
		CAutoLock	lock(m_LockMembers);

		if(!m_bInitialized)
			throw exception( _T("Trace manager is not initialized."));
	}

	CString strFileName;

	{
		CAutoLock	lock(m_LockMembers);
		if(static_cast<unsigned long>(enLogLevel) > m_ulMinLogLevel)
			return;
	}

	if(!lpszMessage)
		return;

	COleDateTime dtToday = COleDateTime::GetCurrentTime();

	TCHAR szBuf[4096] = { 0 };

	_vsntprintf_s(szBuf, sizeof(szBuf), 4096, lpszMessage, argptr);

	TCHAR szHeader[1000] = {0};
	TCHAR szHeaderCout[1000] = {0};
	_stprintf_s(szHeader, sizeof(szHeader), _T("%-2d 0x%04x %-2.2i:%-2.2i:%-2.2i %s\t"), 
				enLogLevel, ::GetCurrentThreadId(), dtToday.GetHour(), 
				dtToday.GetMinute(), dtToday.GetSecond(), lpszCategory);
	_stprintf_s(szHeaderCout, sizeof(szHeaderCout), _T("%-2d 0x%04x %-2.2i:%-2.2i:%-2.2i %s\t"), 
		enLogLevel, ::GetCurrentThreadId(), 
		dtToday.GetHour(), 
		dtToday.GetMinute(), dtToday.GetSecond(), lpszCategory);
	TCHAR szFooter[20] = {0};
	{
		CAutoLock	lock(m_LockLog);

		if(!m_fLog.is_open())
			return;

		m_fLog << szHeader << szBuf  << endl;
		cout << szHeaderCout << szBuf << endl;
	}

}

UINT CEgTraceManager::Run()
{
	while(true)
	{
		HANDLE hStopEvent = CThread::GetStopEventHandle();

		const HANDLE hEvents[] = {m_hDailyTimer, hStopEvent};

		DWORD dwRes = WaitForMultipleObjects(sizeof(hEvents) / sizeof(HANDLE), 
			hEvents, FALSE, INFINITE);

		if (dwRes == WAIT_OBJECT_0 + 1) /* Stop event */
		{
			return 0;
		}
		else if(dwRes == WAIT_OBJECT_0) /* daily timer*/
		{
			_TraceFileInit();
			_QueueSetDailyTimer();
		}
	}

	return 0;
}

bool CEgTraceManager::_QueueSetDailyTimer()
{
	time_t CurTime = time(NULL);
	struct tm CurTM;
	localtime_s(&CurTM, &CurTime);

	struct tm ReloadTM;
	time_t ReloadTime;

	memcpy(&ReloadTM,&CurTM,sizeof(tm));

	ReloadTM.tm_hour = 0;
	ReloadTM.tm_min = 0;
	ReloadTM.tm_sec = 1;

	ReloadTime = mktime(&ReloadTM);

	ReloadTime += SECOND_IN_DAY; // second in day;

	// convert to file time
	LONGLONG llReloadTime = Int32x32To64(ReloadTime, 10000000) + 116444736000000000;

	FILETIME ftUTC;
	ftUTC.dwLowDateTime = (DWORD)llReloadTime;
	ftUTC.dwHighDateTime = (DWORD)(llReloadTime >> 32);

	LARGE_INTEGER liDueTime;
	memcpy(&liDueTime.QuadPart,&ftUTC,sizeof(FILETIME));

	if(SetWaitableTimer(m_hDailyTimer, &liDueTime, 0, NULL, NULL, 0) == TRUE)
		return true;
	else return false;
}

void   CEgTraceManager::_WriteString(LPCTSTR lpString)
{
	CAutoLock lock_log(m_LockLog);

	if(!m_fLog.is_open())
		return;

	m_fLog << lpString << endl;
}

void  CEgTraceable::Trace(TrLogLevelEnum lLogLevel, LPCTSTR lpszCategory, LPCTSTR lpszMessage, ...) const
{
	if(!lpszMessage || !CEgTraceManager::s_spTrace)
		return;

	va_list arglist;
	va_start(arglist, lpszMessage);

	CEgTraceManager::s_spTrace->Trace(lLogLevel,lpszCategory,lpszMessage,arglist);
}

void  CEgTraceable::TraceStatic(TrLogLevelEnum lLogLevel, LPCTSTR lpszCategory, LPCTSTR lpszMessage, ...)
{
	if(!lpszMessage || !CEgTraceManager::s_spTrace)
		return;

	va_list arglist;
	va_start(arglist, lpszMessage);

	CEgTraceManager::s_spTrace->Trace(lLogLevel,lpszCategory,lpszMessage,arglist);
}
 
CEgReportManagerPtr CEgReportManager::s_spEodReport;

void CEgReportManager::_TraceFileInit()
{
	TCHAR szBuf[_MAX_PATH];
	unsigned long dwErr = GetModuleFileName(NULL, szBuf, sizeof(szBuf));
	if(dwErr == 0)
		throw exception(_T("Win32 error"));

	TCHAR szDrive[_MAX_DRIVE];
	TCHAR szDir[_MAX_DIR];
	_tsplitpath_s(szBuf, szDrive,sizeof(szDrive), szDir, sizeof(szDir), NULL,0,  NULL,0 );

	TCHAR szLogsPath[_MAX_PATH];
	_stprintf_s(szLogsPath, sizeof(szLogsPath), _T("%s%sLogs"), szDrive, szDir);

	m_strLogDirectory = szLogsPath;

	{
		CAutoLock lock(m_LockMembers);

		CString strCurrentLogFile = m_strLogDirectory;
		strCurrentLogFile += _T("\\EodReport.EtsEodRpt");

		CAutoLock lock_log(m_LockLog);

		fstream fLog(strCurrentLogFile, ios::trunc | ios::out);
	}

	m_bInitialized = true;
}

void   CEgReportManager::Report(long nEodStep, EodRpMessageTypeEnum enMessageType, LPCTSTR lpszMessage, va_list argptr)
{
	{
		CAutoLock	lock(m_LockMembers);

		if(!m_bInitialized)
			throw exception( _T("Report manager is not initialized."));
	}

	if(!lpszMessage)
		return;

	COleDateTime dtToday = COleDateTime::GetCurrentTime();

	TCHAR szBuf[5*1024] = { 0 };

	_vsntprintf_s(szBuf, sizeof(szBuf), 5*1024, lpszMessage, argptr);

	TCHAR szHeader[2*1024] = {0};
	_stprintf_s(szHeader, sizeof(szHeader), _T("%s\t%d\t%d\t"), dtToday.Format((DWORD)LOCALE_NOUSEROVERRIDE, LANG_SYSTEM_DEFAULT), nEodStep, enMessageType);

	{
		CAutoLock	lock(m_LockLog);

		CString strCurrentLogFile = m_strLogDirectory;
		strCurrentLogFile += _T("\\EodReport.EtsEodRpt");

		fstream fLog(strCurrentLogFile, ios::app | ios::out);

		fLog << szHeader << szBuf << endl;
	}

}
void  CEgReportable::Report(long nEodStep, EodRpMessageTypeEnum enMessageType, LPCTSTR lpszMessage, ...) const
{

	if(!lpszMessage || !CEgReportManager::s_spEodReport)
		return;

	try
	{
		va_list arglist;
		va_start(arglist, lpszMessage);

		CEgReportManager::s_spEodReport->Report(nEodStep,enMessageType, lpszMessage,arglist);
	}
	catch(...)
	{}
}

};
//**************************************************************************************************************//
// solution:	OConnor
// project:		OConnor
// filename:	Tracer.cpp
// created:		28-Dec-2002 15:32 by Suchkov Dmitry
//
// purpose:		implementation of CTracer
//
//**************************************************************************************************************//
#pragma once

//**************************************************************************************************************//
// includes
//**************************************************************************************************************//
#include "StdAfx.h"
#include "Win32Error.h"
#include "Tracer.h"
#include "OConst.h"

//**************************************************************************************************************//
// Static data members init
//**************************************************************************************************************//
HWND CTracer::m_hWnd = NULL;
UINT CTracer::m_uMsg = 0;
CTracer::MessageDestinationsEnum CTracer::m_Destination = CTracer::enMdAll;
TCHAR CTracer::m_szPath[MAX_PATH] = {_T('\n')};
CRITICAL_SECTION CTracer::m_csDataMembers = {0};
CRITICAL_SECTION CTracer::m_csFileWrite = {0};

//**************************************************************************************************************//
// defines
//**************************************************************************************************************//
#define LOG_FILE_BEGIN     _T("Logs/OTrace")
#define LOG_FILE_END       _T(".log")

//--------------------------------------------------------------------------------------------------------------//
CTracer::CTracer()
{
	::InitializeCriticalSection(&m_csDataMembers);
	::InitializeCriticalSection(&m_csFileWrite);
}

//--------------------------------------------------------------------------------------------------------------//
CTracer::~CTracer()
{
	::DeleteCriticalSection(&m_csDataMembers);
	::DeleteCriticalSection(&m_csFileWrite);
}

//--------------------------------------------------------------------------------------------------------------//
void CTracer::TraceMessage(MessageTypeEnum Type, LPCTSTR szSource, LPCTSTR pszMessage, ...)
{
	TCHAR szDate[64];
	if (0 == ::GetDateFormat(
		LOCALE_USER_DEFAULT, 
		DATE_SHORTDATE, 
		NULL, 
		NULL, 
		szDate, 
		sizeof(szDate)/sizeof(TCHAR)))
	{
		szDate[0] = _T('?');
		szDate[1] = _T('\0');
	}

	TCHAR szTime[32];
	if (0 == ::GetTimeFormat(
		LOCALE_USER_DEFAULT,
		LOCALE_NOUSEROVERRIDE,
		NULL,
		NULL,
		szTime,
		sizeof(szTime)/sizeof(TCHAR)))
	{
		szTime[0] = _T('?');
		szTime[1] = _T('\0');
	}

	TCHAR cType;
	switch (Type)
	{
		case enMtInformation:
			cType = _T('I');
			break;

		case enMtWarning:
			cType = _T('W');
			break;

		case enMtError:
			cType = _T('E');
			break;

		default:
			cType = _T('?');
	}

	va_list arglist;
	va_start(arglist, pszMessage);
			
	TCHAR szMessage[4096];
	_vsntprintf_s(szMessage,sizeof(szMessage), 4096, pszMessage, arglist);

	
	TCHAR szOutputLine[4096];
	::wsprintf(
		szOutputLine, 
		_T("[%#04X] %s %s %c %s %s"), 
		::GetCurrentThreadId(),
		szDate,
		szTime,
		cType,
		szMessage, szSource!=NULL?szSource:_T(""));

	if (m_Destination & enMdWindow)
	{
		HWND hWnd = GetWindowHandle();
		sMessage Msg;
		Msg.szMessage = szMessage;
		Msg.szSource = szSource;
		if (hWnd) ::SendMessage(hWnd, GetMessageId(), Type, reinterpret_cast<LPARAM>(&Msg));
	}

	::lstrcat(szOutputLine, _T("\n"));
	
	if (m_Destination & enMdDebug) 
		::OutputDebugString(szOutputLine);
	if (m_Destination & enMdFile) 
		WriteMessageToFile(szOutputLine);
}

//--------------------------------------------------------------------------------------------------------------//
DWORD CTracer::TraceWin32Error(
	DWORD dwError /*= ::GetLastError()*/, 
	LPCTSTR const szDescription /*= _T("Internal error")*/)
{
	CWin32Error E(dwError);
		
	TraceMessage(enMtError,NULL, _T("%s : (%d) %s"), szDescription, dwError, (LPCTSTR)E);
	return dwError;
}

//--------------------------------------------------------------------------------------------------------------//
void CTracer::SetWindowHandle(HWND hWnd)
{
	::InterlockedExchange(reinterpret_cast<volatile long*>(&m_hWnd), reinterpret_cast<long>(hWnd));
}

//--------------------------------------------------------------------------------------------------------------//
HWND CTracer::GetWindowHandle()
{
	return m_hWnd;
}

//--------------------------------------------------------------------------------------------------------------//
void CTracer::SetMessageId(UINT uMsg)
{
	::InterlockedExchange(reinterpret_cast<volatile long*>(&m_uMsg), uMsg);
}

//--------------------------------------------------------------------------------------------------------------//
UINT CTracer::GetMessageId()
{
	return m_uMsg;
}

//--------------------------------------------------------------------------------------------------------------//
void CTracer::SetPath(LPCTSTR pszPath)
{
	::EnterCriticalSection(&m_csDataMembers);
	::lstrcpyn(m_szPath, pszPath, sizeof(m_szPath) / sizeof (TCHAR));
	::LeaveCriticalSection(&m_csDataMembers);
}

//--------------------------------------------------------------------------------------------------------------//
void CTracer::GetPath(LPTSTR pszPath, DWORD nLength)
{
	::EnterCriticalSection(&m_csDataMembers);
	::lstrcpyn(pszPath, m_szPath, nLength);
	::LeaveCriticalSection(&m_csDataMembers);
}

//--------------------------------------------------------------------------------------------------------------//
DWORD CTracer::WriteMessageToFile(LPTSTR pszMessage)
{
	
	TCHAR pLogFile[0x100] = {0};
	TCHAR logDate[0x100] = {0};	

	_bstr_t sbsUserGroup;
	_bstr_t sbsKey = SETTINGS_XML_KEY;
	_bstr_t sbsVal;

	SYSTEMTIME st;
	::GetLocalTime(&st);

	CXMLParamsHelper XMLParams;
	XMLParams.LoadXMLParams();
	XMLParams.GetUserGroup(sbsUserGroup.GetAddress());

	sbsKey += "\\";
	sbsKey += sbsUserGroup;

	XMLParams.GetMainXMLString(sbsKey, "LogNameID", &sbsVal);

	CString strSubjectPrefix = (LPTSTR)sbsUserGroup;
	if (sbsVal.length() > 0)
		strSubjectPrefix = (LPTSTR)sbsVal;

    _stprintf_s(logDate, sizeof(logDate), _T("_%s_%2.2i_%2.2i_%4.4i"), strSubjectPrefix.GetString(), st.wDay, st.wMonth, st.wYear);
				
	_tcscpy_s(pLogFile, sizeof(pLogFile), LOG_FILE_BEGIN);
	_tcscat_s(pLogFile, sizeof(pLogFile), logDate);
	_tcscat_s(pLogFile, sizeof(pLogFile), LOG_FILE_END);        
		
	fstream fLog( CT2A(pLogFile), ios::app | ios::out);
	fLog << CT2A(pszMessage);
    fLog.close();

	return ERROR_SUCCESS;
}



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
#include <Win32Error.h>
#include "Tracer.h"
#include <fstream>

//**************************************************************************************************************//
// Static data members init
//**************************************************************************************************************//

HWND CTracer::m_hWnd = NULL;
UINT CTracer::m_uMsg = 0;
CTracer::MessageDestinationsEnum CTracer::m_Destination = CTracer::enMdAll;
TCHAR CTracer::m_szPath[MAX_PATH] = {_T('\n')};
EgLib::CLock CTracer::m_csDataMembers;

//**************************************************************************************************************//
// defines
//**************************************************************************************************************//
#define LOG_FILE_BEGIN     _T("Logs\\CIBCTradeFeedAdapter")
#define LOG_FILE_END       _T(".log")

//--------------------------------------------------------------------------------------------------------------//
CTracer::CTracer()
{
}

//--------------------------------------------------------------------------------------------------------------//
CTracer::~CTracer()
{
}

//--------------------------------------------------------------------------------------------------------------//
void CTracer::Trace(MessageTypeEnum Type, LPCTSTR pszMessage, ...)
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
	_vsntprintf(szMessage, 4096, pszMessage, arglist);
	
	TCHAR szOutputLine[4096];
	::wsprintf(
		szOutputLine, 
		_T("[%#04X] %s %s %c %s"), 
		::GetCurrentThreadId(),
		szDate,
		szTime,
		cType,
		szMessage);

	if (m_Destination & enMdWindow)
	{
		HWND hWnd = GetWindowHandle();
		//if (hWnd) ::SendMessage(hWnd, GetMessageId(), Type, reinterpret_cast<LPARAM>(szMessage));
		if (hWnd) ::PostMessage(hWnd, GetMessageId(), Type, reinterpret_cast<LPARAM>( new CString(szMessage) ));
	}

	::lstrcat(szOutputLine, _T("\n"));
	
	if (m_Destination & enMdDebug) 
		::OutputDebugString(szOutputLine);
	if (m_Destination & enMdFile) 
		WriteMessageToFile(szOutputLine);

    va_end(arglist);
}

//--------------------------------------------------------------------------------------------------------------//
DWORD CTracer::TraceWin32Error(
	DWORD dwError /*= ::GetLastError()*/, 
	LPCTSTR const szDescription /*= _T("Internal error")*/)
{
	CWin32Error E(dwError);
		
	Trace(enMtError, _T("%s : (%d) %s"), szDescription, dwError, (LPCTSTR)E);
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
    m_csDataMembers.Lock();
	::lstrcpyn(m_szPath, pszPath, sizeof(m_szPath) / sizeof (TCHAR));
    m_csDataMembers.Unlock();
}

//--------------------------------------------------------------------------------------------------------------//
void CTracer::GetPath(LPTSTR pszPath, DWORD nLength)
{
    m_csDataMembers.Lock();
	::lstrcpyn(pszPath, m_szPath, nLength);
    m_csDataMembers.Unlock();
}

//--------------------------------------------------------------------------------------------------------------//
DWORD CTracer::WriteMessageToFile(LPTSTR pszMessage)
{
	USES_CONVERSION;
	
	TCHAR pLogFile[0x100] = {0};
	TCHAR logDate[0x100] = {0};		

	SYSTEMTIME st;
	::GetLocalTime(&st);

    _stprintf(logDate, _T("_%2.2i_%2.2i_%4.4i"), st.wDay, st.wMonth, st.wYear);
				
	_tcscpy(pLogFile, LOG_FILE_BEGIN);
	_tcscat(pLogFile, logDate);
	_tcscat(pLogFile, LOG_FILE_END);        
		
	fstream fLog( T2A(pLogFile), ios::app | ios::out);
	fLog << T2A(pszMessage);
    fLog.close();

	return ERROR_SUCCESS;
}


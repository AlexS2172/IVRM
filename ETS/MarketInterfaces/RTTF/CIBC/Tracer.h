//**************************************************************************************************************//
// solution:	OConnor
// project:		OConnor
// filename:	Tracer.h
// created:		27-Dec-2002 13:03 by Suchkov Dmitry
//
// purpose:		interface of trace routines
//
//**************************************************************************************************************//
#pragma once

#include "StdAfx.h"
//**************************************************************************************************************//
// defines
//**************************************************************************************************************//
#define LOG_FILE_DATE_SUFFIX_USE true

//**************************************************************************************************************//
// class CTracer
//**************************************************************************************************************//
class CTracer
{
public:

	enum MessageTypeEnum {
		enMtInformation	= 0,
		enMtWarning		= 1,
		enMtError		= 2
	};

	enum MessageDestinationsEnum {
		enMdDebug	= 0x01,
		enMdFile	= 0x02,
		enMdWindow	= 0x04,
		enMdAll		= enMdDebug | enMdFile | enMdWindow
	};

	// Constructors / Destructors
	CTracer();
	~CTracer();
	
	// Operations
	static void Trace(MessageTypeEnum Type, LPCTSTR pszMessage, ...);
	
	static DWORD TraceWin32Error(
		DWORD dwError = ::GetLastError(), 
		LPCTSTR const szDescription = _T("Internal error"));
	
	// Data members accessors
	static void SetWindowHandle(HWND hWnd);
	static HWND GetWindowHandle();
	
	static void SetMessageId(UINT uMsg);
	static UINT GetMessageId();
	
	static void SetPath(LPCTSTR pszPath);
	static void GetPath(LPTSTR pszPath, DWORD nLength);
	
protected:

	// Implementation
	static DWORD WriteMessageToFile(LPTSTR pszMessage);

private:
	
	// Data members
	static HWND m_hWnd;
	static UINT m_uMsg;
	static MessageDestinationsEnum m_Destination;
	static TCHAR m_szPath[MAX_PATH];
	static EgLib::CLock m_csDataMembers;
};

#define TraceError CTracer::TraceWin32Error
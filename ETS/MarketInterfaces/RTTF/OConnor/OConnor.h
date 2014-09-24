//**************************************************************************************************************//
// solution:	OConnor
// project:		OConnor
// filename:	OConnor.h
// created:		26-Dec-2002 19:23 by Suchkov Dmitry
//
// purpose:		interface of main module
//
//**************************************************************************************************************//
#pragma once

//**************************************************************************************************************//
// logic guardians
//**************************************************************************************************************//
#ifndef __AFXWIN_H__
	#error 'include "stdafx.h" before including this file for PCH'
#endif

//**************************************************************************************************************//
// includes
//**************************************************************************************************************//
#include "resource.h"
#include "OConnorConnector.h"

class COConnor
{
public:

	DWORD Start();
	
	DWORD Stop();

	DWORD Run();

public:

	COConnor(std::string strSessionID):
	  m_OConnorConnector(strSessionID){}

	static HANDLE	m_hStopApplication;

private:

	COConnorConnector	m_OConnorConnector;

	static BOOL WINAPI HandlerRoutine(DWORD dwCtrlType);
};


//**************************************************************************************************************//
// class COConnorApp
//**************************************************************************************************************//
class COConnorApp : public CWinApp
{
private:

// Data members
	HANDLE m_hSingleInstanceMutex;

public:

// Contructors/Destructors
	COConnorApp();
	~COConnorApp();

// Overrides
	//{{AFX_VIRTUAL(COConnor)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

private:
	std::string m_strSessionId;
	std::string m_stopEventId;
public:

	std::string GetStopEventName(){return m_stopEventId;}
// Implementation
	//{{AFX_MSG(COConnor)
	afx_msg void OnAppAbout();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
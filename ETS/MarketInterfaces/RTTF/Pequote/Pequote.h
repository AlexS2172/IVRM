//**************************************************************************************************************//
// solution:	Pequote
// project:		Pequote
// filename:	Pequote.h
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
#include "PequoteConnector.h"

class CPequote
{
public:
	CPequote(std::string strSessionID)
		:m_PequoteConnector(strSessionID){};

	DWORD Start();
	
	DWORD Stop();

	DWORD Run();

public:

	static HANDLE	m_hStopApplication;

private:

	CPequoteConnector	m_PequoteConnector;

	static BOOL WINAPI HandlerRoutine(DWORD dwCtrlType);
};


//**************************************************************************************************************//
// class CPequoteApp
//**************************************************************************************************************//
class CPequoteApp : public CWinApp
{
private:

// Data members
	HANDLE m_hSingleInstanceMutex;

public:

// Contructors/Destructors
	CPequoteApp();
	~CPequoteApp();

// Overrides
	//{{AFX_VIRTUAL(CPequote)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

private:
	std::string m_strSessionId;
	std::string m_stopEventId;
public:

	std::string GetStopEventName(){return m_stopEventId;}
// Implementation
	//{{AFX_MSG(CPequote)
	afx_msg void OnAppAbout();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
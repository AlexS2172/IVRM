// ProvidersTest.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "ProvidersTest.h"
#include "ProvidersTestDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CProvidersTestApp

BEGIN_MESSAGE_MAP(CProvidersTestApp, CWinApp)
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()


// CProvidersTestApp construction

CProvidersTestApp::CProvidersTestApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CProvidersTestApp object

CProvidersTestApp theApp;


// CProvidersTestApp initialization

BOOL CProvidersTestApp::InitInstance()
{
    CoInitializeEx(0, COINIT_MULTITHREADED);
	InitCommonControls();

	CWinApp::InitInstance();

	CProvidersTestDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
    CoUninitialize();
	return FALSE;
}

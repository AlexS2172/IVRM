// SageApp.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "SageApp.h"
#include "Trace.h"
#include "SageDlg.h"
#include "Publisher.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSageApp

BEGIN_MESSAGE_MAP(CSageApp, CWinApp)
	//{{AFX_MSG_MAP(CSageApp)
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

IMPLEMENT_APPREG(CSageApp, CWinApp)
/////////////////////////////////////////////////////////////////////////////
// CSageApp construction

CSageApp::CSageApp()
	: m_hSingleInstanceMutex(NULL)
{
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CSageApp object

CSageApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CSageApp initialization

BOOL CSageApp::InitInstance()
{
#ifndef _DEBUG 
	SetMiniDumpDefaultCrashHandler();
#endif

	CSageDlg dlg;

	CString sParams(m_lpCmdLine);
	
	if( sParams.Find(_T("-Stop")) >= 0 )
	{
		dlg.AutoStop(true);
		return FALSE;
	}


	LPCTSTR szSingleInstanceMutex = _T("EGAR SAGE {1772A390-F4B7-4d4c-8949-1A11955CC803}");
	m_hSingleInstanceMutex = OpenMutex(MUTEX_ALL_ACCESS, FALSE, szSingleInstanceMutex);
	if (m_hSingleInstanceMutex)
	{
		return FALSE;
	}
	m_hSingleInstanceMutex = CreateMutex(NULL, FALSE, szSingleInstanceMutex);

	// Change the registry key under which our settings are stored.
	SetRegistryKey(APP_KEY);

    CoInitialize(NULL);

	CTracer::CreateFileName();
	
    m_pMainWnd = &dlg;
	dlg.AutoStart( sParams.Find(_T("-Start")) >= 0 );
    dlg.DoModal();

    CoUninitialize();

	if (m_hSingleInstanceMutex)
		CloseHandle(m_hSingleInstanceMutex);

    return FALSE;
}

void CSageApp::PersistWindowPlacement(CWnd*  pWnd, LPCTSTR RelativePath, BOOL bDirection)
    // bDirection TRUE for Save, FALSE for Load
{
    ASSERT_VALID(pWnd);
    ASSERT_KINDOF(CWnd, pWnd);

    if (!pWnd || !::IsWindow(pWnd->m_hWnd) || !RelativePath )
        return;

    WINDOWPLACEMENT     wndpl = {0};
    wndpl.length = sizeof(WINDOWPLACEMENT);

	CXMLParamsHelper XMLParams;
	XMLParams.LoadXMLParams();

    if (bDirection)
    {
        if (GetWindowPlacement(pWnd->m_hWnd, &wndpl))
        {
			XMLParams.SetXMLLong(_T("ETS\\SageConnector\\MainWindow"), _T("Flags"), wndpl.flags);
			XMLParams.SetXMLLong(_T("ETS\\SageConnector\\MainWindow"), _T("MaxPositionX"), wndpl.ptMaxPosition.x);
			XMLParams.SetXMLLong(_T("ETS\\SageConnector\\MainWindow"), _T("MaxPositionY"), wndpl.ptMaxPosition.y);
			XMLParams.SetXMLLong(_T("ETS\\SageConnector\\MainWindow"), _T("MinPositionX"), wndpl.ptMinPosition.x);
			XMLParams.SetXMLLong(_T("ETS\\SageConnector\\MainWindow"), _T("MinPositionY"), wndpl.ptMinPosition.y);
			XMLParams.SetXMLLong(_T("ETS\\SageConnector\\MainWindow"), _T("NormalPositionLeft"), wndpl.rcNormalPosition.left);
			XMLParams.SetXMLLong(_T("ETS\\SageConnector\\MainWindow"), _T("NormalPositionTop"), wndpl.rcNormalPosition.top);
			XMLParams.SetXMLLong(_T("ETS\\SageConnector\\MainWindow"), _T("NormalPositionRight"), wndpl.rcNormalPosition.right);
			XMLParams.SetXMLLong(_T("ETS\\SageConnector\\MainWindow"), _T("NormalPositionBottom"), wndpl.rcNormalPosition.bottom);
			XMLParams.SetXMLLong(_T("ETS\\SageConnector\\MainWindow"), _T("ShowCmd"), wndpl.showCmd);

			XMLParams.SaveXMLParams();
        }
    }
    else
    {
		XMLParams.GetXMLLong(_T("ETS\\SageConnector\\MainWindow"), _T("MaxPositionX"), &wndpl.ptMaxPosition.x, 0xffff);
		XMLParams.GetXMLLong(_T("ETS\\SageConnector\\MainWindow"), _T("MaxPositionY"), &wndpl.ptMaxPosition.y, 0xffff);
		XMLParams.GetXMLLong(_T("ETS\\SageConnector\\MainWindow"), _T("MinPositionX"), &wndpl.ptMinPosition.x, 0xffff);
		XMLParams.GetXMLLong(_T("ETS\\SageConnector\\MainWindow"), _T("MinPositionY"), &wndpl.ptMinPosition.y, 0xffff);

		XMLParams.GetXMLLong(_T("ETS\\SageConnector\\MainWindow"), _T("NormalPositionLeft"), &wndpl.rcNormalPosition.left, 0xffff);
		XMLParams.GetXMLLong(_T("ETS\\SageConnector\\MainWindow"), _T("NormalPositionTop"), &wndpl.rcNormalPosition.top, 0xffff);
		XMLParams.GetXMLLong(_T("ETS\\SageConnector\\MainWindow"), _T("NormalPositionRight"), &wndpl.rcNormalPosition.right, 0xffff);
		XMLParams.GetXMLLong(_T("ETS\\SageConnector\\MainWindow"), _T("NormalPositionBottom"), &wndpl.rcNormalPosition.bottom, 0xffff);

		long nVal = 0xffff;
		XMLParams.GetXMLLong(_T("ETS\\SageConnector\\MainWindow"), _T("Flags"), &nVal, 0xffff);
		wndpl.flags = nVal;
		XMLParams.GetXMLLong(_T("ETS\\SageConnector\\MainWindow"), _T("ShowCmd"), &nVal, 0xffff);
		wndpl.showCmd = nVal;

		if(0xffff == wndpl.rcNormalPosition.left
			|| 0xffff == wndpl.rcNormalPosition.top || 0xffff == wndpl.rcNormalPosition.right
			|| 0xffff == wndpl.rcNormalPosition.bottom || 0xffff == wndpl.showCmd)
		{
			pWnd->ShowWindow(SW_SHOW);
			return;
		}

        SetWindowPlacement(pWnd->m_hWnd, &wndpl);
    }
}


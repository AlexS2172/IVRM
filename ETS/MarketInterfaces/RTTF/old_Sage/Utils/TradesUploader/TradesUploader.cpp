// TradesUploader.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "TradesUploader.h"
#include "TradesUploaderDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTradesUploaderApp

BEGIN_MESSAGE_MAP(CTradesUploaderApp, CWinApp)
	//{{AFX_MSG_MAP(CTradesUploaderApp)
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTradesUploaderApp construction

CTradesUploaderApp::CTradesUploaderApp()
{
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CTradesUploaderApp object

CTradesUploaderApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CTradesUploaderApp initialization

BOOL CTradesUploaderApp::InitInstance()
{
	// Standard initialization
	
    if ( FAILED(CoInitializeEx(NULL, COINIT_MULTITHREADED)) )
    {
        ::MessageBox(NULL, _T("Failed to initialize COM."), _T("Error"), 0);
        return FALSE;
    }

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	CTradesUploaderDlg dlg;
	m_pMainWnd = &dlg;
	int nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
	}
	else if (nResponse == IDCANCEL)
	{
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}

int CTradesUploaderApp::ExitInstance()
{
	::CoUninitialize();
	return 0;
}

void CTradesUploaderApp::PersistWindowPlacement(CWnd*  pWnd, LPCTSTR RelativePath, BOOL bDirection)
    // bDirection TRUE for Save, FALSE for Load
{
    ASSERT_VALID(pWnd);
    ASSERT_KINDOF(CWnd, pWnd);

    if (!pWnd || !::IsWindow(pWnd->m_hWnd) || !RelativePath )
        return;

    WINDOWPLACEMENT     wndpl = {0};
    wndpl.length = sizeof(WINDOWPLACEMENT);

    if (bDirection)
    {
        if (GetWindowPlacement(pWnd->m_hWnd, &wndpl))
        {
        	WriteProfileInt(RelativePath, _T("Flags"), wndpl.flags);
        	WriteProfileInt(RelativePath, _T("MaxPositionX"), wndpl.ptMaxPosition.x);
        	WriteProfileInt(RelativePath, _T("MaxPositionY"), wndpl.ptMaxPosition.y);
        	WriteProfileInt(RelativePath, _T("MinPositionX"), wndpl.ptMinPosition.x);
        	WriteProfileInt(RelativePath, _T("MinPositionY"), wndpl.ptMinPosition.y);
        	WriteProfileInt(RelativePath, _T("NormalPositionLeft"), wndpl.rcNormalPosition.left);
        	WriteProfileInt(RelativePath, _T("NormalPositionTop"), wndpl.rcNormalPosition.top);
        	WriteProfileInt(RelativePath, _T("NormalPositionRight"), wndpl.rcNormalPosition.right);
        	WriteProfileInt(RelativePath, _T("NormalPositionBottom"), wndpl.rcNormalPosition.bottom);
        	WriteProfileInt(RelativePath, _T("ShowCmd"), wndpl.showCmd);
        }
    }
    else
    {
       	wndpl.ptMaxPosition.x = GetProfileInt(RelativePath, _T("MaxPositionX"), 0xffff);
       	wndpl.ptMaxPosition.y = GetProfileInt(RelativePath, _T("MaxPositionY"), 0xffff);
       	wndpl.ptMinPosition.x = GetProfileInt(RelativePath, _T("MinPositionX"), 0xffff);
       	wndpl.ptMinPosition.y = GetProfileInt(RelativePath, _T("MinPositionY"), 0xffff);

        if ((wndpl.flags = GetProfileInt(RelativePath, _T("Flags"), 0xffff)) == 0xffff ||
        	(wndpl.rcNormalPosition.left = GetProfileInt(RelativePath, _T("NormalPositionLeft"), 0xffff)) == 0xffff ||
        	(wndpl.rcNormalPosition.top = GetProfileInt(RelativePath, _T("NormalPositionTop"), 0xffff)) == 0xffff ||
        	(wndpl.rcNormalPosition.right = GetProfileInt(RelativePath, _T("NormalPositionRight"), 0xffff)) == 0xffff ||
        	(wndpl.rcNormalPosition.bottom = GetProfileInt(RelativePath, _T("NormalPositionBottom"), 0xffff)) == 0xffff ||
        	(wndpl.showCmd = GetProfileInt(RelativePath, _T("ShowCmd"), 0xffff)) == 0xffff)
        {
           	wndpl.ptMaxPosition.x = GetProfileInt(RelativePath, _T("MaxPositionX"), 0xffff);
           	wndpl.ptMaxPosition.y = GetProfileInt(RelativePath, _T("MaxPositionY"), 0xffff);
       	    wndpl.ptMinPosition.x = GetProfileInt(RelativePath, _T("MinPositionX"), 0xffff);
       	    wndpl.ptMinPosition.y = GetProfileInt(RelativePath, _T("MinPositionY"), 0xffff);

            if ((wndpl.flags = GetProfileInt(RelativePath, _T("Flags"), 0xffff)) == 0xffff ||
            	(wndpl.rcNormalPosition.left = GetProfileInt(RelativePath, _T("NormalPositionLeft"), 0xffff)) == 0xffff ||
        	    (wndpl.rcNormalPosition.top = GetProfileInt(RelativePath, _T("NormalPositionTop"), 0xffff)) == 0xffff ||
            	(wndpl.rcNormalPosition.right = GetProfileInt(RelativePath, _T("NormalPositionRight"), 0xffff)) == 0xffff ||
            	(wndpl.rcNormalPosition.bottom = GetProfileInt(RelativePath, _T("NormalPositionBottom"), 0xffff)) == 0xffff ||
            	(wndpl.showCmd = GetProfileInt(RelativePath, _T("ShowCmd"), 0xffff)) == 0xffff)
            {
        	    pWnd->ShowWindow(SW_SHOW);
                return;
            }
        }

        SetWindowPlacement(pWnd->m_hWnd, &wndpl);
    }
}
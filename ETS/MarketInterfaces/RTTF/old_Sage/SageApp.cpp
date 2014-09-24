// SageApp.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "SageApp.h"
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
	
	CSageDlg dlg;
    m_pMainWnd = &dlg;
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


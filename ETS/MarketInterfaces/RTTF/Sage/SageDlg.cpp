// SageDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SageApp.h"
#include "SageDlg.h"
#include "Sage.h"
#include "SettingsDlg.h"
#include "AboutDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CSageApp    theApp;

/////////////////////////////////////////////////////////////////////////////
// CSageDlg dialog

CSageDlg::CSageDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSageDlg::IDD, pParent)
    ,m_hThread(INVALID_HANDLE_VALUE)
    ,m_bWaitStopping(FALSE)
    ,m_hAccel(0)
	,m_bAutoStart(false)
	,m_hStopEvent(CreateEvent(NULL, TRUE, FALSE, _T("E39F38E6-900E-4c18-8E79-27415A858C6D")))
{
	//{{AFX_DATA_INIT(CSageDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	unsigned int uiID = 0;
	
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
    m_Menu.LoadMenu(IDR_MAINFRAME);
	_beginthreadex(NULL, 0, _threadStop, this, 0, &uiID);

}

void CSageDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSageDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CSageDlg, CDialog)
	//{{AFX_MSG_MAP(CSageDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	ON_COMMAND(IDC_START, OnStart)
	ON_UPDATE_COMMAND_UI(IDC_START, OnUpdateControls)
    ON_WM_INITMENUPOPUP()
    ON_MESSAGE_VOID(WM_KICKIDLE, OnKickIdle)
	ON_COMMAND(IDC_STOP, OnStop)
	ON_WM_CLOSE()
	ON_COMMAND(IDC_PROPERTIES, OnProperties)
	ON_WM_SIZE()
	ON_COMMAND(ID_CLEAR_LOG, OnClearLog)
	ON_WM_KEYUP()
	ON_UPDATE_COMMAND_UI(IDC_STOP, OnUpdateControls)
	ON_UPDATE_COMMAND_UI(IDC_PROPERTIES, OnUpdateControls)
	ON_WM_CHAR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSageDlg message handlers

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

BOOL CSageDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetMenu(&m_Menu);
	
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
        return TRUE;
	}

	CRect rcClientStart;
	CRect rcClientNow;
	GetClientRect(rcClientStart);
	RepositionBars(AFX_IDW_CONTROLBAR_FIRST, AFX_IDW_CONTROLBAR_LAST,
				   0, reposQuery, rcClientNow);

	// Now move all the controls so they are in the same relative
	// position within the remaining client area as they would be
	// with no control bars.
	CPoint ptOffset(rcClientNow.left - rcClientStart.left,
					rcClientNow.top - rcClientStart.top);

	CRect   rcChild;
	CWnd*   pwndChild = GetWindow(GW_CHILD);

	while (pwndChild)
	{
		pwndChild->GetWindowRect(rcChild);
		ScreenToClient(rcChild);
		rcChild.OffsetRect(ptOffset);
		pwndChild->MoveWindow(rcChild, FALSE);
		pwndChild = pwndChild->GetNextWindow();
	}

	// Adjust the dialog window dimensions
	CRect rcWindow;
	GetWindowRect(rcWindow);
	rcWindow.right += rcClientStart.Width() - rcClientNow.Width();
	rcWindow.bottom += rcClientStart.Height() - rcClientNow.Height();
	MoveWindow(rcWindow, FALSE);

    theApp.PersistWindowPlacement(this, _T("MainWindow"), FALSE);
    m_hAccel = LoadAccelerators(0, MAKEINTRESOURCE(IDR_MAINFRAME));

    static_cast<CEdit*>(GetDlgItem(IDC_EDLOG))->SetLimitText(0);
	if(m_bAutoStart)
		OnStart();
    
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CSageDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
        OnAppAbout();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CSageDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

HCURSOR CSageDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CSageDlg::OnAppAbout() 
{
	CAboutDlg dlgAbout;
	dlgAbout.DoModal();
}

void CSageDlg::OnInitMenuPopup(CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu)
{
	if (!bSysMenu)
	{
		ASSERT(pPopupMenu != NULL);

		// check the enabled state of various menu items
		CCmdUI state;
		state.m_pMenu = pPopupMenu;
		ASSERT(state.m_pOther == NULL);

		state.m_nIndexMax = pPopupMenu->GetMenuItemCount();
		for (state.m_nIndex = 0; state.m_nIndex < state.m_nIndexMax;
			 state.m_nIndex++)
		{
			state.m_nID = pPopupMenu->GetMenuItemID(state.m_nIndex);
			if (state.m_nID == 0)
				continue; // menu separator or invalid cmd - ignore it

			ASSERT(state.m_pOther == NULL);
			ASSERT(state.m_pMenu != NULL);
			if (state.m_nID == (UINT)-1)
			{
				// possibly a popup menu, route to first item of that popup
				state.m_pSubMenu = pPopupMenu->GetSubMenu(state.m_nIndex);
				if (state.m_pSubMenu == NULL ||
					(state.m_nID = state.m_pSubMenu->GetMenuItemID(0)) == 0 ||
					state.m_nID == (UINT)-1)
				{
					continue; // first item of popup can't be routed to
				}
				state.DoUpdate(this, FALSE);  // popups are never auto disabled
			}
			else
			{
				// normal menu item
				// Auto enable/disable if command is _not_ a system command
				state.m_pSubMenu = NULL;
				state.DoUpdate(this, state.m_nID < 0xF000);
			}
		}
	}
}

void CSageDlg::OnKickIdle()
{
    UpdateToolbar();
}

void CSageDlg::OnUpdateControls(CCmdUI* pCmdUI) 
{
    BOOL bStarted = IsStarted();
    switch (pCmdUI->m_nID)
    {
    case IDC_START:
    case IDC_PROPERTIES:
        pCmdUI->Enable(!bStarted);
        break;
    case IDC_STOP:
        pCmdUI->Enable(bStarted);
        break;
    }
}

void CSageDlg::UpdateToolbar() 
{
    BOOL bStarted = IsStarted();
    ::SendMessage(m_wndToolBar, TB_ENABLEBUTTON, IDC_START, MAKELPARAM(!bStarted, 0));
    ::SendMessage(m_wndToolBar, TB_ENABLEBUTTON, IDC_STOP, MAKELPARAM(bStarted, 0));
    ::SendMessage(m_wndToolBar, TB_ENABLEBUTTON, IDC_PROPERTIES, MAKELPARAM(!bStarted, 0));

    CString sTitle;
    CString sRequiredTitle = bStarted ? _T(" Online-FixInterface") : _T(" Offline-FixInterface");

    GetWindowText(sTitle);
    if (sTitle != sRequiredTitle)
        SetWindowText(sRequiredTitle);
}

int main(int argc, char* argv[]);
UINT __stdcall CSageDlg::_thread(void* p)
{
   CSageDlg* pDlg = reinterpret_cast<CSageDlg*>(p);
   main(0, 0);
   if(pDlg && WaitForSingleObject(pDlg->m_hStopEvent,0)!=WAIT_TIMEOUT)
		pDlg->PostMessage(WM_CLOSE);
   return 0;
}
UINT __stdcall CSageDlg::_threadStop(void* p)
{
	CSageDlg* pDlg = reinterpret_cast<CSageDlg*>(p);
	if(pDlg && WaitForSingleObject(pDlg->m_hStopEvent, INFINITE)!=WAIT_TIMEOUT)
		pDlg->PostMessage(WM_CLOSE);
	return 0;
}

void CSageDlg::OnStart()
{
    if (m_hThread != INVALID_HANDLE_VALUE)
    {
        ASSERT(::WaitForSingleObject(m_hThread, 0) == WAIT_OBJECT_0);
        ::CloseHandle(m_hThread);
        m_hThread = INVALID_HANDLE_VALUE;
    }

    AfxCallWndProc(this, this->m_hWnd, WM_COMMAND, ID_CLEAR_LOG, 0);

    unsigned int uiID = 0;
	m_hThread = (HANDLE)_beginthreadex(NULL, 0, _thread, this, 0, &uiID);
}

void CSageDlg::OnStop()
{
    if (CSage::m_hStopApplication)
        SetEvent(CSage::m_hStopApplication);
	CTracer::Trace(_T("Server stopping..."));
}

void CSageDlg::OnProperties() 
{
    CSettingsDlg dlg;
	//dlg.m_FixConfigName = GetFixConfigName();
    dlg.DoModal();
}

void CSageDlg::OnClose()
{
    AfxCallWndProc(this, this->m_hWnd, WM_COMMAND, IDC_STOP, 0);

    if (m_bWaitStopping)
        return;

	SetEvent(m_hStopEvent);
    m_bWaitStopping = TRUE;

    if (m_hThread != INVALID_HANDLE_VALUE)
    { 
        while (::MsgWaitForMultipleObjects(1, &m_hThread,
            FALSE, INFINITE, QS_ALLINPUT) != WAIT_OBJECT_0)
        {
            MSG msg = {0};
			while (PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE))
			{
                TRACE( _T("Process message\n"));
                if (msg.message != WM_CLOSE)
                {
	    			TranslateMessage(&msg);
    				DispatchMessage(&msg);
                }
			}
        }

        ::CloseHandle(m_hThread);
        m_hThread = INVALID_HANDLE_VALUE;
    }
	
    theApp.PersistWindowPlacement(this, _T("MainWindow"), TRUE);
	CDialog::OnClose();
}

void CSageDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);

    CWnd* pWnd = GetDlgItem(IDC_EDLOG);
    if (!pWnd)
        return;

    CRect rcChild;
	pWnd->GetWindowRect(rcChild);
	ScreenToClient(rcChild);
    rcChild.right = cx - rcChild.left;
    rcChild.bottom = cy - rcChild.left;
	pWnd->MoveWindow(rcChild);

	// And position the control bars
	RepositionBars(AFX_IDW_CONTROLBAR_FIRST, AFX_IDW_CONTROLBAR_LAST, 0);
}

void CSageDlg::OnClearLog() 
{
    CWnd* pWnd = 0;
    if (pWnd = GetDlgItem(IDC_EDLOG))
    {
        CEdit* pEdit = static_cast<CEdit*>(pWnd);
        pEdit->SetSel(0, -1);
        pEdit->ReplaceSel(_T(""));
    }
}

void CSageDlg::AutoStart(bool bEnable)
{
	m_bAutoStart = bEnable;
}
void CSageDlg::AutoStop(bool bEnable)
{
	::SetEvent(m_hStopEvent);
	::SetEvent(CSage::m_hStopApplication);
}


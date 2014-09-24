// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "CIBCTradeFeedAdapter.h"

#include "MainFrm.h"
#include "Settings.h"
#include "SettingsDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_WM_SETFOCUS()
	ON_COMMAND(ID_SETTINGS, OnSettings)
	ON_COMMAND(ID_ACTION_START, OnActionStart)
	ON_MESSAGE(WM_INSERT_TRACE_MESSAGE, OnInsertTraceMessage)
	ON_MESSAGE(WM_STOP_ENDOFDAY_PROCESS, OnStopEndofdayProcess)
	ON_COMMAND(ID_ACTION_STOP, OnActionStop)
	ON_UPDATE_COMMAND_UI(ID_ACTION_START, OnUpdateActionStart)
	ON_UPDATE_COMMAND_UI(ID_ACTION_STOP, OnUpdateActionStop)
	ON_UPDATE_COMMAND_UI(ID_SETTINGS, OnUpdateSettings)
	ON_COMMAND(ID_ACTION_SS, OnActionEndofdayStart)
	ON_UPDATE_COMMAND_UI(ID_ACTION_SS, OnUpdateActionSs)
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
	// Global help commands
	ON_COMMAND(ID_HELP_FINDER, CFrameWnd::OnHelpFinder)
	ON_COMMAND(ID_HELP, CFrameWnd::OnHelp)
	ON_COMMAND(ID_CONTEXT_HELP, CFrameWnd::OnContextHelp)
	ON_COMMAND(ID_DEFAULT_HELP, CFrameWnd::OnHelpFinder)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
    m_Publisher.SetMode(MODE_ENDOFADAY);
    m_bIsRunning = FALSE;
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	// create a view to occupy the client area of the frame
	if (!m_wndView.Create(WS_VISIBLE | WS_CHILD, CRect(0, 0, 0, 0), this, AFX_IDW_PANE_FIRST))
	{
		TRACE0("Failed to create view window\n");
		return -1;
	}
	
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

    HICON hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	if (NULL != hIcon)
	{
		SetIcon(hIcon, TRUE);
		SetIcon(hIcon, FALSE);
		::DeleteObject(hIcon);
	}
	else
	{
		TRACE0("Failed to load icon\n");
	}

	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	cs.lpszClass = AfxRegisterWndClass(0);
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers
void CMainFrame::OnSetFocus(CWnd* pOldWnd)
{
	// forward focus to the view window
	m_wndView.SetFocus();
}

BOOL CMainFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	// let the view have first crack at the command
	if (m_wndView.OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
		return TRUE;

	// otherwise, do default handling
	return CFrameWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}


void CMainFrame::OnSettings() 
{
	CSettingsDlg dlg;
	dlg.DoModal();
}

LRESULT CMainFrame::OnInsertTraceMessage(WPARAM wParam, LPARAM lParam)
{
	//if (NULL == lParam) return 0;
    
	ASSERT( AfxIsValidAddress((void*)lParam, sizeof(CString)) );
	if(lParam)
	{
		CString *psParam = (CString*) reinterpret_cast<CString*>(lParam);
        
	    m_wndView.InsertTraceMessage(
		static_cast<CTracer::MessageTypeEnum>(wParam),
		(LPCTSTR)(*psParam));

        delete psParam;
	}
	
	return 0;
}

LRESULT CMainFrame::OnStopEndofdayProcess(WPARAM wParam, LPARAM lParam)
{
    OnActionStop();
	return 0;
}

void CMainFrame::NeedToUpdateSettings() 
{
    try
    {
        if ( !CSettings::IsLoaded() )
        {
            if (!CSettings::LoadSettings())
                OnSettings();
        }
    }
	catch(_com_error& e)
    {
		CTracer::Trace(CTracer::enMtWarning, e.Description());
    }
    catch(...)
    {
        TraceError(E_UNEXPECTED, _T("Unexpected error"));
    }
}

void CMainFrame::OnActionEndofdayStart() 
{
    CString sMessage;
    BOOL bIsIntradayRunning = (TRUE == m_bIsRunning && m_Publisher.GetMode() == MODE_INTRADAY);

    sMessage += "Note:\n";
    sMessage += "   During this process the trades won't dynamically be updated in ETS.\n";
    sMessage += "   Please restart all ETS workstations then it finished.\n\n";

    if (bIsIntradayRunning)
        sMessage += "Intraday process will be stopped.\n";

    sMessage += "Trade feed will be cleared.\n\n";
    sMessage += "Do You wish to start End-of-Day synchronization process anyway?\n";

    if (IDCANCEL == AfxMessageBox(sMessage, MB_OKCANCEL))
        return;

    /* try to load settings */
    NeedToUpdateSettings();

    if (bIsIntradayRunning)
    {
        OnActionStop();
    }

    m_Publisher.SetMode(MODE_ENDOFADAY);

    DWORD dwRes = m_Publisher.Start(NULL);
	if (!dwRes)
	{
        m_bIsRunning = TRUE;
	}
}

void CMainFrame::OnActionStart() 
{
    if (TRUE == m_bIsRunning && m_Publisher.GetMode() == MODE_ENDOFADAY)
        return;

    /* try to load settings */
    NeedToUpdateSettings();

    m_Publisher.SetMode(MODE_INTRADAY);
    
    DWORD dwRes = m_Publisher.Start(NULL);
	if (!dwRes)
	{
        m_bIsRunning = TRUE;
	}
}

void CMainFrame::OnActionStop() 
{
    if (TRUE == m_bIsRunning)
    {
        m_Publisher.Stop();
        m_bIsRunning = FALSE;
    }
}


void CMainFrame::OnUpdateActionStart(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(FALSE == m_bIsRunning);
}

void CMainFrame::OnUpdateActionStop(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(TRUE == m_bIsRunning);
}

void CMainFrame::OnUpdateSettings(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(FALSE == m_bIsRunning);
}

void CMainFrame::OnUpdateActionSs(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!(TRUE == m_bIsRunning && m_Publisher.GetMode() == MODE_ENDOFADAY));
}

void CMainFrame::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	OnActionStop();
    
	CFrameWnd::OnClose();
}

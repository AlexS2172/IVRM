// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "RtControl.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const UINT WM_TASKBARCREATED = ::RegisterWindowMessage(_T("TaskbarCreated"));

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_REGISTERED_MESSAGE(WM_TASKBARCREATED, OnTaskbarCreated)
	ON_WM_CLOSE()
	ON_MESSAGE(WM_ROUTER_SERVICE_ERROR, OnRouterServiceError)		
	ON_MESSAGE(WM_ROUTER_UPDATE_STATUS, OnRouterUpdateStatus)		
	ON_WM_QUERYENDSESSION()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
	
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	SetMenu(NULL);

	if(!m_stTray.Create(NULL, WM_ICON_NOTIFY, _T(""), NULL, IDR_MAINFRAME))
		return -1;

	UpdateRtStatus();

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;

	cs.style = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;
	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;

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

void CMainFrame::UpdateRtStatus()
{
	if(g_App.m_nRtStatus == SERVICE_RUNNING )
		g_App.GetActiveHosts();
	else
		RemoveAllClients();

	m_stTray.SetIcon(g_App.m_hSmallIcon[g_App.m_nRtStatus == SERVICE_RUNNING ? 1 : 0]);
	m_stTray.SetTooltipText(g_App.m_strStatus[g_App.m_nRtStatus == SERVICE_RUNNING ? 1 : 0]);
	
	SetIcon(g_App.m_hAppIcon[g_App.m_nRtStatus == SERVICE_RUNNING ? 1 : 0], TRUE);
	SetIcon(g_App.m_hAppIcon[g_App.m_nRtStatus == SERVICE_RUNNING ? 1 : 0], FALSE);
	SetWindowText(g_App.m_strStatus[g_App.m_nRtStatus == SERVICE_RUNNING ? 1 : 0]);

	CRtControlView* m_pView = (CRtControlView*)GetActiveView();
	if(m_pView->GetSafeHwnd())
	{
		m_pView->UpdateRtStatus();		
	}
}

void CMainFrame::AddClient(LPCTSTR lpszClient, unsigned long ulHostAddr)
{
	CRtControlView* m_pView = (CRtControlView*)GetActiveView();
	if(m_pView->GetSafeHwnd())
		m_pView->AddClient(lpszClient,ulHostAddr);
}

void CMainFrame::RemoveClient(LPCTSTR lpszClient)
{
	CRtControlView* m_pView = (CRtControlView*)GetActiveView();
	if(m_pView->GetSafeHwnd())
		m_pView->RemoveClient(lpszClient);
}

void CMainFrame::RemoveAllClients()
{
	CRtControlView* m_pView = (CRtControlView*)GetActiveView();
	if(m_pView->GetSafeHwnd())
		m_pView->RemoveAllClients();
}

LRESULT CMainFrame::OnTaskbarCreated(WPARAM wp, LPARAM lp)
{
	m_stTray.AddIcon();
	UpdateRtStatus();
	return 0;
}

void CMainFrame::OnClose() 
{
	if(!g_App.m_bExitNow)
	{
		ShowWindow(SW_HIDE);
		ModifyStyleEx(0, WS_EX_TOOLWINDOW);
	}
	else
	{
		CFrameWnd::OnClose();
	}
}
LRESULT CMainFrame::OnRouterUpdateStatus(WPARAM wParam, LPARAM lParam)
{
	UpdateRtStatus();

	return 0;
}
LRESULT CMainFrame::OnRouterServiceError(WPARAM wParam, LPARAM lParam)
{
	CString strError,strCaption;
			
	strCaption.LoadString(IDS_PROJNAME);

	strError = (LPCTSTR)lParam;
			
		
	MessageBox(strError,strCaption,MB_OK | MB_ICONSTOP);
			
	return 0;
}

BOOL CMainFrame::OnQueryEndSession() 
{
	if (!CFrameWnd::OnQueryEndSession())
		return FALSE;
	
	g_App.DetachFromRouter();
	
	return TRUE;
}

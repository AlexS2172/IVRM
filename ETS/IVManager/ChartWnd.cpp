// ChartWnd.cpp : implementation file
//

#include "stdafx.h"
#include "IVAnalyzer.h"
#include "ChartWnd.h"

#include "StrikeSkewDlg.h"
#include "TimeSkewDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CChartWnd

IMPLEMENT_DYNAMIC (CChartWnd, CWnd)


CChartWnd::CChartWnd()
{
	m_pDlg = new CTimeSkewDlg();
}

CChartWnd::CChartWnd(DATE dtMonth)
{
	m_pDlg = new CStrikeSkewDlg(dtMonth);
}

CChartWnd::~CChartWnd()
{
	delete m_pDlg;
}

BEGIN_MESSAGE_MAP(CChartWnd, CWnd)
	//{{AFX_MSG_MAP(CChartWnd)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_WM_CLOSE()
	ON_WM_ACTIVATE()
	ON_WM_GETMINMAXINFO()
//	ON_WM_HELPINFO()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CChartWnd message handlers

int CChartWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (! m_pDlg->Create (this) )
	{
		GetDesktopWindow()->
			MessageBox ("Unable to initialize chart control - setup incomplete",
			NULL, MB_OK | MB_ICONERROR  | MB_TOPMOST);

		return -1;
	}
	m_pDlg->ShowWindow(SW_SHOWNA);

	
	CString str;
	m_pDlg->GetWindowText (str);
	
	SetWindowText (str);
	
	ResizeCtrls();
	
	return 0;
}

void CChartWnd::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);

	ResizeCtrls();
}

BOOL CChartWnd::Create()
{
	return CreateEx(0, AfxRegisterWndClass (0,0, 0, theApp.LoadIcon(IDI_IV_APP)), _T("Chart"),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		CW_USEDEFAULT, CW_USEDEFAULT,
		*CWnd::GetDesktopWindow(), 0 );
}

void CChartWnd::Show()
{
	bool bUpdate = !IsWindowVisible();
	SetWindowPos( &wndTop, 0, 0, 0, 0, SWP_SHOWWINDOW | SWP_NOMOVE | SWP_NOSIZE );
	
	WINDOWPLACEMENT wpls;
	wpls.length = sizeof( WINDOWPLACEMENT );
	GetWindowPlacement( &wpls );
	if( wpls.showCmd == SW_SHOWMINIMIZED )
	{
		wpls.showCmd = SW_SHOWNORMAL;
		SetWindowPlacement (&wpls);
	}

	if( bUpdate )
		UpdateChart( false );
}

void CChartWnd::ResizeCtrls()
{
	CRect rc;
	GetClientRect (rc);

	m_pDlg->MoveWindow (0, 0, rc.Width(), rc.Height());
}

void CChartWnd::OnDestroy() 
{
	if (m_pDlg->m_hWnd)	
		m_pDlg->DestroyWindow();

	CWnd::OnDestroy();
}

void CChartWnd::OnClose() 
{
	ShowWindow (SW_HIDE);
	
	//CWnd::OnClose();
}

void CChartWnd::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized) 
{
	CWnd::OnActivate(nState, pWndOther, bMinimized);
	
	if (m_pDlg  && IsWindow(*m_pDlg) && IsWindow(m_pDlg->m_wndPopupText))
	{
		m_pDlg->m_wndPopupText.Hide();
	}
}


void CChartWnd::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI) 
{
	if( ::IsWindow(m_pDlg->m_hWnd) )
		m_pDlg->OnGetMinMaxInfo( lpMMI );

	CWnd::OnGetMinMaxInfo(lpMMI);
}

/*
BOOL CChartWnd::OnHelpInfo( HELPINFO* lpHelpInfo )
{
	return TRUE;
}
*/
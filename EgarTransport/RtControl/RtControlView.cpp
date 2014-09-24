// RtControlView.cpp : implementation of the CRtControlView class
//

#include "stdafx.h"
#include "RtControl.h"

#include "RtControlView.h"
#include "HostStatistics.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRtControlView

IMPLEMENT_DYNCREATE(CRtControlView, CFormView)

BEGIN_MESSAGE_MAP(CRtControlView, CFormView)
	//{{AFX_MSG_MAP(CRtControlView)
	ON_BN_CLICKED(IDC_START_BUTTON, OnStartButton)
	ON_BN_CLICKED(IDC_STOP_BUTTON, OnStopButton)
	ON_BN_CLICKED(IDC_AUTOSTART_CHECK, OnAutostartCheck)
	ON_BN_CLICKED(IDC_START_MIN_CHECK, OnStartMinCheck)
	ON_BN_CLICKED(IDC_CLOSE_BUTTON, OnCloseButton)
	ON_LBN_DBLCLK(IDC_HOST_LIST, OnDblclkHostList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRtControlView construction/destruction

CRtControlView::CRtControlView()
	: CFormView(CRtControlView::IDD)
{
	//{{AFX_DATA_INIT(CRtControlView)
	//}}AFX_DATA_INIT
	// TODO: add construction code here

}

CRtControlView::~CRtControlView()
{
}

void CRtControlView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRtControlView)
	DDX_Control(pDX, IDC_START_MIN_CHECK, m_chkStartMin);
	DDX_Control(pDX, IDC_AUTOSTART_CHECK, m_chkAutoStart);
	DDX_Control(pDX, IDC_HOST_LIST, m_lstHost);
	DDX_Control(pDX, IDC_STOP_BUTTON, m_btnStop);
	DDX_Control(pDX, IDC_START_BUTTON, m_btnStart);
	DDX_Control(pDX, IDC_STATE_IMAGE, m_stcStateImage);
	//}}AFX_DATA_MAP
}

BOOL CRtControlView::PreCreateWindow(CREATESTRUCT& cs)
{
	if(!CFormView::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	return TRUE;
}

void CRtControlView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();

	m_btnStart.SetIcon(g_App.LoadIcon(IDI_START));
	m_btnStop.SetIcon(g_App.LoadIcon(IDI_STOP));

	if(g_App.m_nAutoStart != -1)
	{
		m_chkAutoStart.EnableWindow(TRUE);
		m_chkAutoStart.SetCheck(g_App.m_nAutoStart);
	}
	else
	{
		m_chkAutoStart.EnableWindow(FALSE);
		m_chkAutoStart.SetCheck(0);
	}

	m_chkStartMin.SetCheck(g_App.m_nStartMin);
	
	UpdateRtStatus();
}

/////////////////////////////////////////////////////////////////////////////
// CRtControlView diagnostics

#ifdef _DEBUG
void CRtControlView::AssertValid() const
{
	CFormView::AssertValid();
}

void CRtControlView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CRtControlDoc* CRtControlView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CRtControlDoc)));
	return (CRtControlDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CRtControlView message handlers

void CRtControlView::UpdateRtStatus()
{
	if(GetSafeHwnd())
	{
		m_stcStateImage.SetIcon(g_App.m_hAppIcon[g_App.m_nRtStatus == SERVICE_RUNNING ? 1 : 0]);

		m_btnStart.EnableWindow(g_App.m_nRtStatus == SERVICE_STOPPED);
		GetDlgItem(IDC_START_STATIC)->EnableWindow(g_App.m_nRtStatus == SERVICE_STOPPED);
		
		m_btnStop.EnableWindow(g_App.m_nRtStatus == SERVICE_RUNNING);
		GetDlgItem(IDC_STOP_STATIC)->EnableWindow(g_App.m_nRtStatus == SERVICE_RUNNING);
	}
}

void CRtControlView::AddClient(LPCTSTR lpszClient, unsigned long ulHostAddr)
{
	CString strTemp(lpszClient);
	strTemp.MakeLower();

	int nIdx = m_lstHost.FindStringExact(-1, strTemp);
	if(LB_ERR == nIdx)
		nIdx = m_lstHost.AddString(strTemp);

	if(LB_ERR != nIdx)
		m_lstHost.SetItemData(nIdx,ulHostAddr);

}

void CRtControlView::RemoveClient(LPCTSTR lpszClient)
{
	CString strTemp(lpszClient);
	strTemp.MakeLower();

	int nIdx = m_lstHost.FindStringExact(-1, strTemp);
	if(LB_ERR != nIdx)
	{
		m_lstHost.DeleteString(nIdx);
	}
}

void CRtControlView::RemoveAllClients()
{
	m_lstHost.ResetContent();
}

void CRtControlView::OnStartButton() 
{
	HWND hwndMain = g_App.GetMainWnd()->GetSafeHwnd();
	if(hwndMain)
	{
		::PostMessage(hwndMain, WM_COMMAND, MAKEWPARAM(ID_RT_START, 0), 0);
	}
}

void CRtControlView::OnStopButton() 
{
	HWND hwndMain = g_App.GetMainWnd()->GetSafeHwnd();
	if(hwndMain)
	{
		::PostMessage(hwndMain, WM_COMMAND, MAKEWPARAM(ID_RT_STOP, 0), 0);
	}
}

void CRtControlView::OnAutostartCheck() 
{
	g_App.m_nAutoStart = m_chkAutoStart.GetCheck();
	g_App.SaveStartType();
}

void CRtControlView::OnStartMinCheck() 
{
	g_App.m_nStartMin = m_chkStartMin.GetCheck();
}

void CRtControlView::OnCloseButton() 
{
	HWND hwndMain = g_App.GetMainWnd()->GetSafeHwnd();
	if(hwndMain)
	{
		::PostMessage(hwndMain, WM_CLOSE, 0, 0);
	}
}

void CRtControlView::OnDblclkHostList() 
{
	int nIdx = m_lstHost.GetCurSel();
	if(LB_ERR != nIdx)
	{
		unsigned long Addr = m_lstHost.GetItemData(nIdx);
		_bstr_t bsIP = inet_ntoa(*(in_addr*)&Addr);
	
		if(!g_App.GetHostStatistic(bsIP))
		{
			m_lstHost.DeleteString(nIdx);
		}
		else
		{
			CHostStatistics dlgStat(bsIP);
			
			dlgStat.DoModal();			
		}
	}
}

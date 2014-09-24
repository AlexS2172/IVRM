// TpServiceDlg.cpp : implementation file
//

#include "stdafx.h"
#include "transportcontrol.h"
#include "TpServiceDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTpServiceDlg property page

IMPLEMENT_DYNCREATE(CTpServiceDlg, CPropertyPage)

CTpServiceDlg::CTpServiceDlg() : CPropertyPage(CTpServiceDlg::IDD)
{
	//{{AFX_DATA_INIT(CTpServiceDlg)
	//}}AFX_DATA_INIT
}

CTpServiceDlg::~CTpServiceDlg()
{
}

void CTpServiceDlg::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTpServiceDlg)
	DDX_Control(pDX, IDC_STATE_IMAGE, m_stcStateImage);
	DDX_Control(pDX, IDC_STOP_BUTTON, m_btnStop);
	DDX_Control(pDX, IDC_START_BUTTON, m_btnStart);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTpServiceDlg, CPropertyPage)
	//{{AFX_MSG_MAP(CTpServiceDlg)
	ON_BN_CLICKED(IDC_STOP_BUTTON, OnStopButton)
	ON_BN_CLICKED(IDC_START_BUTTON, OnStartButton)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTpServiceDlg message handlers

void CTpServiceDlg::OnStartButton() 
{
	HWND hwndMain = theApp.GetMainWnd()->GetSafeHwnd();
	if(hwndMain)
	{
		::PostMessage(hwndMain, WM_COMMAND, MAKEWPARAM(ID_TP_START, 0), 0);
	}
}

void CTpServiceDlg::OnStopButton() 
{
	HWND hwndMain = theApp.GetMainWnd()->GetSafeHwnd();
	if(hwndMain)
	{
		::PostMessage(hwndMain, WM_COMMAND, MAKEWPARAM(ID_TP_STOP, 0), 0);
	}
	
}
void CTpServiceDlg::UpdateTpStatus()
{
	if(GetSafeHwnd())
	{
		m_stcStateImage.SetIcon(theApp.m_hAppIcon[theApp.getTpSrvStatus() == SERVICE_RUNNING ? 1 : 0]);

		m_btnStart.EnableWindow(theApp.getTpSrvStatus() == SERVICE_STOPPED);
		GetDlgItem(IDC_START_STATIC)->EnableWindow(theApp.getTpSrvStatus() == SERVICE_STOPPED);
		
		m_btnStop.EnableWindow(theApp.getTpSrvStatus() == SERVICE_RUNNING );
		GetDlgItem(IDC_STOP_STATIC)->EnableWindow(theApp.getTpSrvStatus() == SERVICE_RUNNING);
	}
}

BOOL CTpServiceDlg::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	m_btnStart.SetIcon(theApp.LoadIcon(IDI_START));
	m_btnStop.SetIcon(theApp.LoadIcon(IDI_STOP));

	UpdateTpStatus();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

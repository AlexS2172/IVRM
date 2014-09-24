// SplashDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ivanalyzer.h"
#include "SplashDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSplashDlg dialog


CSplashDlg::CSplashDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSplashDlg::IDD, pParent)
	, m_hAppearance(NULL)
{
	//{{AFX_DATA_INIT(CSplashDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CSplashDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSplashDlg)
	DDX_Control(pDX, IDC_SPLASH_PICTURE, m_picSplash);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSplashDlg, CDialog)
	//{{AFX_MSG_MAP(CSplashDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSplashDlg message handlers

BOOL CSplashDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	BOOL bResult = m_picSplash.Load (MAKEINTRESOURCE (IDR_SPLASH), _T("JPEG"), m_hAppearance);
	ASSERT (bResult);
	
	CSize szSplash = m_picSplash.GetSize();
	SetWindowPos (NULL, 0, 0, szSplash.cx, szSplash.cy, SWP_NOMOVE | SWP_NOZORDER );
	m_picSplash.SetWindowPos (NULL, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOZORDER );
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}



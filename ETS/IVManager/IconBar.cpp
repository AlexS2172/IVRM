// IconBar.cpp : implementation file
//

#include "stdafx.h"
#include "ivanalyzer.h"
#include "IconBar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CIconBar dialog


CIconBar::CIconBar(CWnd* pParent /*=NULL*/)
	: CDialog(CIconBar::IDD, pParent)
{
	//{{AFX_DATA_INIT(CIconBar)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CIconBar::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CIconBar)
	DDX_Control(pDX, IDC_ICON_GIF, m_picIcon);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CIconBar, CDialog)
	//{{AFX_MSG_MAP(CIconBar)
	ON_WM_SIZE()
	ON_WM_NCHITTEST()
	ON_WM_CTLCOLOR()
	ON_WM_CREATE()
	ON_WM_SYSCOLORCHANGE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIconBar message handlers
BOOL CIconBar::OnInitDialog() 
{
	CDialog::OnInitDialog();


	m_picIcon.Load (MAKEINTRESOURCE (IDR_ICON_GIF), _T("GIF"), theApp.GetAppearance());

	CSize sizeIcon = m_picIcon.GetSize();

	
	const int nMargin = 0;
	SetWindowPos (NULL, 0, 0, sizeIcon.cx + nMargin*2, sizeIcon.cy + nMargin*2, SWP_NOMOVE | SWP_NOZORDER);

	m_picIcon.SetWindowPos (NULL, nMargin, nMargin, 0, 0, SWP_NOSIZE | SWP_NOZORDER);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CIconBar::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
}

LRESULT CIconBar::OnNcHitTest(CPoint point) 
{
	return HTTRANSPARENT;
}

HBRUSH CIconBar::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	return hbr;
}



int CIconBar::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	
	return 0;
}




void CIconBar::OnSysColorChange() 
{
	CDialog::OnSysColorChange();
	
	m_picIcon.SetBkColor ( GetSysColor(COLOR_3DFACE) );

}

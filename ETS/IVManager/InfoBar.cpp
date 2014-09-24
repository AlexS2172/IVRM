// InfoBar.cpp : implementation file
//

#include "stdafx.h"
#include "ivanalyzer.h"
#include "InfoBar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CInfoBar dialog


CInfoBar::CInfoBar(CWnd* pParent /*=NULL*/)
	: CDialog(CInfoBar::IDD, pParent)
{
	//{{AFX_DATA_INIT(CInfoBar)
	//}}AFX_DATA_INIT
}


void CInfoBar::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInfoBar)
	DDX_Control(pDX, IDC_LOGO_GIF, m_picLogo);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CInfoBar, CDialog)
	//{{AFX_MSG_MAP(CInfoBar)
	ON_WM_SIZE()
//	ON_WM_NCHITTEST()
	ON_WM_CTLCOLOR()
	ON_WM_CREATE()
	ON_WM_SYSCOLORCHANGE()
	//}}AFX_MSG_MAP
	ON_WM_NCHITTEST()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInfoBar message handlers

BOOL CInfoBar::OnInitDialog() 
{
	CDialog::OnInitDialog();


	m_picLogo.Load (MAKEINTRESOURCE (IDR_IVRM_LOGO_GIF), _T("GIF"),theApp.GetAppearance() );

	CSize sizeLogo = m_picLogo.GetSize();

	
	const int nMargin = 4;
	SetWindowPos (NULL, 0, 0, sizeLogo.cx + nMargin*2, sizeLogo.cy + nMargin*2, SWP_NOMOVE | SWP_NOZORDER);

	m_picLogo.SetWindowPos (NULL, nMargin, nMargin, 0, 0, SWP_NOSIZE | SWP_NOZORDER);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CInfoBar::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
}

//LRESULT CInfoBar::OnNcHitTest(CPoint point) 
//{
//}

HBRUSH CInfoBar::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	switch (pWnd->GetDlgCtrlID() )
	{
	case IDC_CAPTION:
		pDC->SetTextColor ( GetSysColor(COLOR_GRAYTEXT));
		break;
	case IDC_LOGO:
		hbr =  (HBRUSH) GetStockObject (NULL_BRUSH);		
		break;
	}
	return hbr;
}



int CInfoBar::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	
	return 0;
}




void CInfoBar::OnSysColorChange() 
{
	CDialog::OnSysColorChange();
	
	m_picLogo.SetBkColor ( GetSysColor(COLOR_3DFACE) );

}

LRESULT CInfoBar::OnNcHitTest(CPoint point)
{
	return HTTRANSPARENT;
}

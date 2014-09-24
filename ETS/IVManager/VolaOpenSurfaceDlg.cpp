// VolaOpenSurfaceDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ivanalyzer.h"
#include "VolaOpenSurfaceDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CVolaOpenSurfaceDlg dialog


CVolaOpenSurfaceDlg::CVolaOpenSurfaceDlg(CSurfacesVector& vecSurfaces, long& nCurrentSurface, CWnd* pParent /*=NULL*/): 
	CDialog(CVolaOpenSurfaceDlg::IDD, pParent),
	m_vecSurfaces(vecSurfaces),
	m_nCurrentSurface(nCurrentSurface)
{
	//{{AFX_DATA_INIT(CVolaOpenSurfaceDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CVolaOpenSurfaceDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CVolaOpenSurfaceDlg)
	DDX_Control(pDX, IDC_LIST_SURFACE, m_lbsSurfaces);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CVolaOpenSurfaceDlg, CDialog)
	//{{AFX_MSG_MAP(CVolaOpenSurfaceDlg)
	ON_LBN_DBLCLK(IDC_LIST_SURFACE, OnDblclkListSurface)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CVolaOpenSurfaceDlg message handlers

BOOL CVolaOpenSurfaceDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	if( m_vecSurfaces.size() > 0 )
	{
		for( size_t i = 0; i < m_vecSurfaces.size(); i++ )
			m_lbsSurfaces.AddString( m_vecSurfaces[i].m_strName );

		m_lbsSurfaces.SetCurSel( m_nCurrentSurface );
	}
	else
		GetDlgItem( IDOK )->EnableWindow( FALSE );

	return TRUE;
}


void CVolaOpenSurfaceDlg::OnOK() 
{
	long nSel = m_lbsSurfaces.GetCurSel();

	if( nSel > -1 )
		m_nCurrentSurface = nSel;
	
	CDialog::OnOK();
}


void CVolaOpenSurfaceDlg::OnDblclkListSurface() 
{
	OnOK();
}

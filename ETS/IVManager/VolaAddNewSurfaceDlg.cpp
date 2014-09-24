// VolaAddNewSurfaceDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ivanalyzer.h"
#include "VolaAddNewSurfaceDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CVolaAddNewSurfaceDlg dialog


CVolaAddNewSurfaceDlg::CVolaAddNewSurfaceDlg(Mode mode, CSurfacesVector& vecSurfaces, long nCurrentSurface, CWnd* pParent /*=NULL*/): 
	CDialog(CVolaAddNewSurfaceDlg::IDD, pParent),
	m_enMode(mode),
	m_vecSurfaces(vecSurfaces),
	m_nCurrentSurface(nCurrentSurface)
{
	//{{AFX_DATA_INIT(CVolaAddNewSurfaceDlg)
	m_strName = _T("");
	//}}AFX_DATA_INIT

	switch( m_enMode )
	{
		case enNew:
			m_strCaption = _T("Add new surface");
			break;
		case enEdit:
			m_strCaption = _T("Modify surface");
			break;
		case enSaveAs:
			m_strCaption = _T("Save as");
			break;
	}
}


void CVolaAddNewSurfaceDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CVolaAddNewSurfaceDlg)
	DDX_Text(pDX, IDC_SURFACE_NAME, m_strName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CVolaAddNewSurfaceDlg, CDialog)
	//{{AFX_MSG_MAP(CVolaAddNewSurfaceDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CVolaAddNewSurfaceDlg message handlers

BOOL CVolaAddNewSurfaceDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	SetWindowText( m_strCaption );

	if( m_enMode == enNew )
	{
		m_strName = _T("<New Surface>");
	}
	else if( m_enMode == enEdit )
	{
		m_strName  = m_vecSurfaces[m_nCurrentSurface].m_strName;
	}
	else if( m_enMode == enSaveAs )
	{
		m_strName = _T("Copy of ") + m_vecSurfaces[m_nCurrentSurface].m_strName;
	}

	UpdateData( FALSE );
	
	return TRUE;
}


void CVolaAddNewSurfaceDlg::OnOK() 
{
	UpdateData( TRUE );

	for( size_t i = 0; i < m_vecSurfaces.size(); i++ )
	{
		if( m_vecSurfaces[i].m_strName == m_strName && ( m_enMode != enEdit || i != m_nCurrentSurface ) 
		  )
		{
			MessageBox( _T("This symbol already has surface with this name."), NULL, MB_OK|MB_ICONSTOP );
			return;
		}
	}

	if( m_enMode == enEdit )
		m_vecSurfaces[m_nCurrentSurface].m_strName = m_strName;

	CDialog::OnOK();
}

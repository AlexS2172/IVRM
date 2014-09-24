//**************************************************************************************************************//
// solution:	Pequote
// project:		Pequote
// filename:	DlgAbout.cpp
// created:		27-Dec-2002 17:08 by Suchkov Dmitry
//
// purpose:		implementation of CDlgAbout
//
//**************************************************************************************************************//

//**************************************************************************************************************//
// includes
//**************************************************************************************************************//
#include "StdAfx.h"
#include "DlgAbout.h"

//--------------------------------------------------------------------------------------------------------------//
BEGIN_MESSAGE_MAP(CDlgAbout, CDialog)
	//{{AFX_MSG_MAP(CDlgAbout)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//--------------------------------------------------------------------------------------------------------------//
CDlgAbout::CDlgAbout() : CDialog(CDlgAbout::IDD)
{
	//{{AFX_DATA_INIT(CDlgAbout)
	m_sLegalCopyRight = _T("");
	m_sProductName = _T("");
	m_sProductVersion = _T("");
	//}}AFX_DATA_INIT
}

//--------------------------------------------------------------------------------------------------------------//
void CDlgAbout::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgAbout)
	DDX_Text(pDX, IDC_ED_LEGAL_COPYRIGHT, m_sLegalCopyRight);
	DDX_Text(pDX, IDC_ED_PRODUCT_NAME, m_sProductName);
	DDX_Text(pDX, IDC_ED_PRODUCT_VERSION, m_sProductVersion);
	//}}AFX_DATA_MAP
}

//--------------------------------------------------------------------------------------------------------------//
BOOL CDlgAbout::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CModuleVersion ModuleVersion;
	ModuleVersion.GetModuleVersionInfo(AfxGetInstanceHandle());

	ModuleVersion.GetValue(_T("ProductName"), m_sProductName);
	ModuleVersion.GetValue(_T("ProductVersion"), m_sProductVersion);
	ModuleVersion.GetValue(_T("LegalCopyright"), m_sLegalCopyRight);
	
	
	CenterWindow(AfxGetMainWnd());
	UpdateData(FALSE);

	return TRUE;
}


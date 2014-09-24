// LicenseDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ivanalyzer.h"
#include "LicenseDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLicenseDlg dialog


CLicenseDlg::CLicenseDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLicenseDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLicenseDlg)
	m_strLicenseKey = _T("");
	m_strProductID = _T("");
	//}}AFX_DATA_INIT
}


void CLicenseDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	m_strProductID.TrimLeft();
	m_strProductID.TrimRight();

	//{{AFX_DATA_MAP(CLicenseDlg)
	DDX_Text(pDX, IDC_LICENSE_KEY, m_strLicenseKey);
	DDX_Text(pDX, IDC_PRODUCT_ID, m_strProductID);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLicenseDlg, CDialog)
	//{{AFX_MSG_MAP(CLicenseDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLicenseDlg message handlers

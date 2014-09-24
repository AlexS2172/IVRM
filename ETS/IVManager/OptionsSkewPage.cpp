// OptionsSkewPage.cpp : implementation file
//

#include "stdafx.h"
#include "ivanalyzer.h"
#include "OptionsSkewPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COptionsSkewPage property page

IMPLEMENT_DYNCREATE(COptionsSkewPage, CPropertyPage)

COptionsSkewPage::COptionsSkewPage() : CPropertyPage(COptionsSkewPage::IDD)
{
	//{{AFX_DATA_INIT(COptionsSkewPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

COptionsSkewPage::~COptionsSkewPage()
{
}

void COptionsSkewPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COptionsSkewPage)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COptionsSkewPage, CPropertyPage)
	//{{AFX_MSG_MAP(COptionsSkewPage)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COptionsSkewPage message handlers

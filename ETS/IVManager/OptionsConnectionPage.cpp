// OptionsConnectionPage.cpp : implementation file
//

#include "stdafx.h"
#include "ivanalyzer.h"
#include "OptionsConnectionPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COptionsConnectionPage property page

IMPLEMENT_DYNCREATE(COptionsConnectionPage, CPropertyPage)

COptionsConnectionPage::COptionsConnectionPage() : CPropertyPage(COptionsConnectionPage::IDD)
{
	//{{AFX_DATA_INIT(COptionsConnectionPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

COptionsConnectionPage::~COptionsConnectionPage()
{
}

void COptionsConnectionPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COptionsConnectionPage)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COptionsConnectionPage, CPropertyPage)
	//{{AFX_MSG_MAP(COptionsConnectionPage)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COptionsConnectionPage message handlers

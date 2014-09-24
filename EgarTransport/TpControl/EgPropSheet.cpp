// EgPropSheet.cpp : implementation file
//

#include "stdafx.h"
#include "TransportControl.h"
#include "EgPropSheet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEgPropSheet

IMPLEMENT_DYNAMIC(CEgPropSheet, CPropertySheet)

CEgPropSheet::CEgPropSheet(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
}

CEgPropSheet::CEgPropSheet(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
}

CEgPropSheet::~CEgPropSheet()
{
}


BEGIN_MESSAGE_MAP(CEgPropSheet, CPropertySheet)
	//{{AFX_MSG_MAP(CEgPropSheet)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEgPropSheet message handlers

BOOL CEgPropSheet::OnInitDialog() 
{
   m_bModeless = FALSE;   
   m_nFlags |= WF_CONTINUEMODAL;

   BOOL bResult = CPropertySheet::OnInitDialog();

   m_bModeless = TRUE;
   m_nFlags &= ~WF_CONTINUEMODAL;
   return bResult;
}

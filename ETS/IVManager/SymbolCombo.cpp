// SymbolCombo.cpp : implementation file
//

#include "stdafx.h"
#include "ivanalyzer.h"
#include "SymbolCombo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSymbolCombo

CSymbolCombo::CSymbolCombo(): m_bClosing (false) 
{
}

CSymbolCombo::~CSymbolCombo()
{
}


BEGIN_MESSAGE_MAP(CSymbolCombo, CFlatComboBox)
	//{{AFX_MSG_MAP(CSymbolCombo)
	ON_CONTROL_REFLECT(CBN_SELENDOK, OnSelendokSymbols)
	ON_CONTROL_REFLECT(CBN_SELENDCANCEL, OnSelendcancelSymbols)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSymbolCombo message handlers

BOOL CSymbolCombo::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->message == WM_KEYDOWN  )
	{
		m_bAutoComp = false;
		switch (pMsg->wParam)
		{
		case VK_RETURN:
			{
				CString strText;
				GetWindowText(strText);

				
				m_bClosing = true;
				ShowDropDown (FALSE);
				
				SetWindowText (strText);
				m_bClosing = false;
				
				SetEditSel (0, -1);
				UpdateWindow();

				AfxGetMainWnd()->PostMessage (WM_COMMAND, ID_SELECT_SYMBOL);
				return TRUE;
			}
		}
		
	}

	if (pMsg->message == WM_CHAR  && pMsg->wParam >= 32 )
	{
		m_bAutoComp = true;
	}
	
	return CFlatComboBox::PreTranslateMessage(pMsg);
}


void CSymbolCombo::OnSelendokSymbols() 
{
	if (! m_bClosing)
	{
		TRACE (_T("Select\n"));
		AfxGetMainWnd()->PostMessage (WM_COMMAND, ID_SELECT_SYMBOL);
	}
}

void CSymbolCombo::OnSelendcancelSymbols() 
{
	if (! m_bClosing)
	{
		TRACE (_T("Cancel\n"));
		AfxGetMainWnd()->PostMessage (WM_COMMAND, ID_CANCEL_SYMBOL);
	}
}
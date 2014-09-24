// ToolTipCtrlEx.cpp : implementation file
//

#include "stdafx.h"
#include "ToolTipCtrlEx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CToolTipCtrlEx

CToolTipCtrlEx::CToolTipCtrlEx()
{
}

CToolTipCtrlEx::~CToolTipCtrlEx()
{
}


BEGIN_MESSAGE_MAP(CToolTipCtrlEx, CToolTipCtrl)
	//{{AFX_MSG_MAP(CToolTipCtrlEx)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CToolTipCtrlEx message handlers


int CToolTipCtrlEx::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CToolTipCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;

	SendMessage(TTM_SETMAXTIPWIDTH, 0, ::GetSystemMetrics(SM_CXSCREEN));
	
	return 0;
}

// FlatComboBox.cpp : implementation file
//

#include "stdafx.h"
#include "FlatComboBox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFlatComboBox

const UINT CFlatComboBox::m_nTimerID = 500; 


CFlatComboBox::CFlatComboBox()
{
	m_bLBtnDown = FALSE;
	m_bPainted = FALSE;
	m_bHasFocus = FALSE;
	m_bAutoComp = FALSE;
	m_clrHilite = ::GetSysColor(COLOR_BTNHIGHLIGHT);
	m_clrShadow = ::GetSysColor(COLOR_BTNSHADOW);
	m_clrNormal = ::GetSysColor(COLOR_BTNFACE);
	m_clrText = ::GetSysColor(COLOR_WINDOWTEXT); 
	m_clrBack = ::GetSysColor(COLOR_WINDOW); 
}

CFlatComboBox::~CFlatComboBox()
{
}

IMPLEMENT_DYNAMIC(CFlatComboBox, CComboBox)

BEGIN_MESSAGE_MAP(CFlatComboBox, CComboBox)
	//{{AFX_MSG_MAP(CFlatComboBox)
	ON_WM_CTLCOLOR_REFLECT()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_TIMER()
	ON_WM_PAINT()
	ON_CONTROL_REFLECT(CBN_SETFOCUS,   OnSetFocus)
	ON_CONTROL_REFLECT(CBN_KILLFOCUS,  OnKillFocus)
	ON_CONTROL_REFLECT(CBN_EDITUPDATE, OnEditUpdate)
	ON_WM_SYSCOLORCHANGE()
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFlatComboBox message handlers

void CFlatComboBox::OnMouseMove(UINT nFlags, CPoint point) 
{
	SetTimer(m_nTimerID, 10,NULL);
	OnTimer(m_nTimerID);
	CComboBox::OnMouseMove(nFlags, point);
}

void CFlatComboBox::OnLButtonDown(UINT nFlags, CPoint point) 
{
	m_bLBtnDown = TRUE;
	CComboBox::OnLButtonDown(nFlags, point);
}

void CFlatComboBox::OnLButtonUp(UINT nFlags, CPoint point) 
{
	m_bLBtnDown = FALSE;
	Invalidate();
	CComboBox::OnLButtonUp(nFlags, point);
}

void CFlatComboBox::OnTimer(UINT nIDEvent) 
{
	if (nIDEvent != m_nTimerID)
		return;

	if (m_bLBtnDown==TRUE) { // OnLButtonDown, show enPressed.
		KillTimer (m_nTimerID);
		if (m_bPainted == TRUE) {
			DrawCombo(enPressed, m_clrShadow,	m_clrHilite);
			m_bPainted = FALSE;
		}
	} else if (!PointInRect() && !m_bHasFocus) { // If mouse leaves, show flat.
		KillTimer (m_nTimerID);
		if (m_bPainted == TRUE) {
			DrawCombo(enNormal, m_clrNormal, m_clrNormal);
			m_bPainted = FALSE;
		}
	} else { // On mouse over, show enRaised.
		if (m_bPainted == TRUE) {
			return;
		} else {
			m_bPainted = TRUE;
			DrawCombo(enRaised, m_clrShadow, m_clrHilite);
		}
	}
}

void CFlatComboBox::OnPaint() 
{
	ModifyStyleEx (WS_EX_DLGMODALFRAME | WS_EX_CLIENTEDGE | WS_EX_STATICEDGE,
		0, SWP_FRAMECHANGED);

	CPaintDC dc(this);

	CRect rcClient;
	GetClientRect( &rcClient );

	CDC memDC;
	CBitmap bitmap;
	memDC.CreateCompatibleDC(&dc);
	bitmap.CreateCompatibleBitmap(&dc, rcClient.Width(), rcClient.Height());
	CBitmap* pOldBitmap = memDC.SelectObject(&bitmap);

	CWnd::DefWindowProc( WM_PAINT, (WPARAM)memDC.m_hDC, 0 );
	
	if (PointInRect() || m_bHasFocus) 
	{
		DrawCombo(enRaised, m_clrShadow, m_clrHilite, &memDC);
	} 
	else 
	{
		DrawCombo(enNormal, m_clrNormal, m_clrNormal, &memDC);
	}

	dc.BitBlt(rcClient.left, rcClient.top, rcClient.Width(), rcClient.Height(), &memDC, 0,0, SRCCOPY);

	memDC.SelectObject(pOldBitmap);
	memDC.DeleteDC();
	bitmap.DeleteObject();
}

void CFlatComboBox::DrawCombo(StateEnum eState, COLORREF clrTopLeft, COLORREF clrBottomRight, CDC* pDC)
{
	CRect rcItem;
	GetClientRect(&rcItem);
	bool bRelease = false;

	if(pDC == NULL) {
		pDC = GetDC();
		bRelease = true;
	}

	pDC->Draw3dRect(rcItem, clrTopLeft, clrBottomRight);
	rcItem.DeflateRect(1,1);
	
	if (!IsWindowEnabled()) {
		pDC->Draw3dRect(rcItem, m_clrHilite, m_clrHilite);
	} else {
		pDC->Draw3dRect(rcItem, m_clrNormal, m_clrNormal);
	}

	rcItem.DeflateRect(1,1);
	rcItem.left = rcItem.right-::GetSystemMetrics(SM_CXHTHUMB);;
	pDC->Draw3dRect(rcItem, m_clrNormal, m_clrNormal);
	
	rcItem.DeflateRect(1,1);
	pDC->Draw3dRect(rcItem, m_clrNormal, m_clrNormal);
	
	if (!IsWindowEnabled()) {
		if(bRelease == true) {
			ReleaseDC(pDC);
		}
		return;
	}

	rcItem.InflateRect(0,1);
	switch (eState)
	{
	case enNormal:
		pDC->Draw3dRect(rcItem, m_clrHilite, m_clrHilite);
		rcItem.left -= 1;
		pDC->Draw3dRect(rcItem, m_clrHilite, m_clrHilite);
		break;

	case enRaised:
		pDC->Draw3dRect(rcItem, m_clrHilite, m_clrShadow);
		break;

	case enPressed:
		rcItem.OffsetRect(1,1);
		pDC->Draw3dRect(rcItem, m_clrShadow, m_clrHilite);
		break;
	}

	if(bRelease == true) {
		ReleaseDC(pDC);
	}
}

void CFlatComboBox::OnSetFocus()
{
	m_bHasFocus = TRUE;
	DrawCombo(enRaised, m_clrShadow, m_clrHilite);
}

void CFlatComboBox::OnKillFocus() 
{
	m_bHasFocus = FALSE;
	DrawCombo(enNormal, m_clrNormal, m_clrNormal);	
}

BOOL CFlatComboBox::PreTranslateMessage(MSG* pMsg) 
{
	return CComboBox::PreTranslateMessage(pMsg);
}

void CFlatComboBox::OnEditUpdate() 
{
	if (!m_bAutoComp) 
		return;
	
	CString str;
	GetWindowText(str);
	int nLength = str.GetLength();
	
	DWORD dwCurSel = GetEditSel();
	WORD dStart = LOWORD(dwCurSel);
	WORD dEnd   = HIWORD(dwCurSel);
	
	if (SelectString(-1, str) == CB_ERR)
	{
		SetWindowText(str);
		if (dwCurSel != CB_ERR)
			SetEditSel(dStart, dEnd);
	}
	
	if (dEnd < nLength && dwCurSel != CB_ERR)
		SetEditSel(dStart, dEnd);
	else
		SetEditSel(nLength, -1);
}

void CFlatComboBox::OnSysColorChange() 
{
	CComboBox::OnSysColorChange();

	m_clrHilite	= ::GetSysColor(COLOR_BTNHIGHLIGHT);
	m_clrShadow	= ::GetSysColor(COLOR_BTNSHADOW);
	m_clrNormal	= ::GetSysColor(COLOR_BTNFACE);
}

BOOL CFlatComboBox::OnEraseBkgnd(CDC* pDC) 
{
	return TRUE;
}

HBRUSH CFlatComboBox::CtlColor(CDC* pDC, UINT nCtlColor)
{
	UNREFERENCED_PARAMETER(nCtlColor);

	COLORREF clrText=GetTextColor();
	COLORREF clrBk=GetBkColor();

	pDC->SetTextColor(clrText);
	pDC->SetBkColor(clrBk);

	static CBrush brush;
	if((HBRUSH)brush!=NULL)
		brush.DeleteObject();
	brush.CreateSolidBrush(clrBk);

	return (HBRUSH)brush;
}

void CFlatComboBox::SetTextColor(COLORREF clrText, BOOL bRedraw/*=TRUE*/)
{ 
	if(m_clrText!=clrText)
	{
		m_clrText=clrText; 
		if(bRedraw)
			RedrawWindow();
	}
}

void CFlatComboBox::SetBkColor(COLORREF clrBack, BOOL bRedraw/*=TRUE*/)
{ 
	if(m_clrBack!=clrBack)
	{
		m_clrBack=clrBack; 
		if(bRedraw)
			RedrawWindow();
	}
}


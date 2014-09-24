// EGSpin.cpp : implementation file
//

#include "stdafx.h"
#include "EGSpin.h"
#include <float.h>
#include <afxpriv.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static const TCHAR g_szCEGSpinWndClassName[] = _T("CEGSpin::WndClassName");

UINT EG_SM_VALUECHANGED = ::RegisterWindowMessage(_T("CEGSpinButtonCtrl::EG_SM_VALUECHANGED"));
UINT EG_SM_MOUSEENTER = ::RegisterWindowMessage(_T("CEGSpinButtonCtrl::EG_SM_MOUSEENTER"));
UINT EG_SM_MOUSEEXIT = ::RegisterWindowMessage(_T("CEGSpinButtonCtrl::EG_SM_MOUSEEXIT"));

/////////////////////////////////////////////////////////////////////////////
// CEGSpin

IMPLEMENT_DYNAMIC(CEGSpin, CWnd)

CEGSpin::CEGSpin()
	: m_hwndBuddy(NULL),
	m_hwndParent(NULL),

	m_enAlignment(enAlignmentRightInside),
	m_enOrientation(enOrientationVertical),
	m_enPressedState(enPressedNone),

	m_bWrap(FALSE),
	m_bFlat(FALSE),

	m_dMin(0.),
	m_dMax(100.),
	m_dStep(1.),
	m_dValue(0.),

	m_uiPresision(0),
	m_dEps(0.1),

	m_uiSpinAccelCount(0),
	m_pSpinAccel(NULL),

	m_hCursor(NULL),

	m_uiTimerPeriod(500),
	m_bMouseDown(FALSE),
	m_bMouseOver(FALSE),
	m_enActivePressedState(enPressedNone),
	m_enBuddyState(enBuddyStateNormal)
{
	/*::SystemParametersInfo(SPI_GETKEYBOARDDELAY, 0, &m_nDelay, 0);
	m_nDelay = (m_nDelay + 1) * 250;
	::SystemParametersInfo(SPI_GETKEYBOARDSPEED, 0, &m_dwSpeed, 0);
	m_dwSpeed = (DWORD)(1000 / (2.5 + m_dwSpeed * .0275));*/
}

CEGSpin::~CEGSpin()
{
	if(m_pSpinAccel)
		delete [] m_pSpinAccel;
}

/////////////////////////////////////////////////////////////////////////////
//
BEGIN_MESSAGE_MAP(CEGSpin, CWnd)
	//{{AFX_MSG_MAP(CEGSpin)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_ERASEBKGND()
	ON_WM_SETCURSOR()
	ON_WM_SYSCOLORCHANGE()
	ON_WM_TIMER()
	ON_WM_ENABLE()
	ON_WM_DESTROY()
	ON_WM_MOUSEMOVE()
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	ON_WM_MOUSEACTIVATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
//
BOOL CEGSpin::PreCreateWindow(CREATESTRUCT& cs) 
{
	WNDCLASS wndcls = { 0 };

	HINSTANCE hInst = ::AfxGetInstanceHandle();

	wndcls.style = CS_HREDRAW | CS_VREDRAW;
	wndcls.lpfnWndProc = ::DefWindowProc;
	wndcls.cbClsExtra = wndcls.cbWndExtra = 0;
	wndcls.hInstance = ::AfxGetInstanceHandle();
	wndcls.hIcon = NULL;
	wndcls.hCursor = ::LoadCursor(NULL, IDC_ARROW);
	wndcls.hbrBackground = NULL;
	wndcls.lpszMenuName = NULL;
	wndcls.lpszClassName = g_szCEGSpinWndClassName;

	if(!::AfxRegisterClass(&wndcls))
		return FALSE;

	cs.lpszClass = g_szCEGSpinWndClassName;
	cs.style = WS_CHILD | WS_VISIBLE;

	return CWnd::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
//
BOOL CEGSpin::Create(CWnd* pParentWnd, CWnd* pBuddyWnd, UINT uiWidth /*= 0*/,
					EGSpinAlignmentEnum enAlignment /*= enAlignmentRightInside*/,
					EGSpinOrientationEnum enOrientation /*= enOrientationVertical*/)
{
	HWND hwndParent = pParentWnd->GetSafeHwnd();
	HWND hwndBuddy = pBuddyWnd->GetSafeHwnd();

	ASSERT(::IsWindow(hwndParent));
	ASSERT(::IsWindow(hwndBuddy));

	if(!::IsWindow(hwndParent)
		|| !::IsWindow(hwndBuddy)
		|| (pBuddyWnd->GetStyle() & WS_CHILD) != WS_CHILD)
	{
		return FALSE;
	}

	if(!CWnd::Create(g_szCEGSpinWndClassName, NULL, 0, CRect(0, 0, 0, 0), pParentWnd, 0, NULL))
		return FALSE;

	m_hwndParent = hwndParent;
	m_hwndBuddy = hwndBuddy;

	SetAlignment(enAlignment, FALSE);
	SetOrientation(enOrientation, FALSE);

	m_uiWidth = uiWidth;

	if(m_uiWidth == 0)
		m_uiWidth = ::GetSystemMetrics(SM_CXVSCROLL);

	_AlignWindow(TRUE);

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//
void CEGSpin::_AlignWindow(BOOL bSet)
{
	ASSERT(::IsWindow(m_hwndParent));
	ASSERT(::IsWindow(m_hwndBuddy));
	ASSERT(m_uiWidth > 0);

	CRect rcBuddy;
	::GetWindowRect(m_hwndBuddy, &rcBuddy);
	::ScreenToClient(m_hwndParent, (LPPOINT)&rcBuddy);
	::ScreenToClient(m_hwndParent, ((LPPOINT)&rcBuddy) + 1);
	if(::GetWindowLong(m_hwndParent, GWL_EXSTYLE) & WS_EX_LAYOUTRTL)
		CRect::SwapLeftRight(rcBuddy);

	int nEdge = ::GetSystemMetrics(SM_CXEDGE);
	if(bSet)
	{
		CRect rc;
		rc = rcBuddy;
		switch(m_enAlignment)
		{
		case enAlignmentRightInside:
			rcBuddy.right -= m_uiWidth;
			rc.left = rcBuddy.right - nEdge;
			break;

		case enAlignmentRightOutside:
			rc.right += m_uiWidth;
			rc.left += m_uiWidth;
			break;

		case enAlignmentLeftInside:
			rcBuddy.left += m_uiWidth;
			rc.right = rcBuddy.left + nEdge;
			break;

		case enAlignmentLeftOutside:
			rc.left -= m_uiWidth;
			rc.right -= m_uiWidth;
			break;

		default:
			ASSERT(FALSE);
		}

		::MoveWindow(m_hwndBuddy, rcBuddy.left, rcBuddy.top, rcBuddy.Width(), rcBuddy.Height(), TRUE);
		::SetWindowPos(m_hWnd, m_hwndBuddy, rc.left, rc.top, rc.Width(), rc.Height(), 0/*SWP_NOREDRAW*/);
	}
	else
	{
		switch(m_enAlignment)
		{
		case enAlignmentRightInside:
			rcBuddy.right += m_uiWidth - nEdge;
			break;

		case enAlignmentLeftInside:
			rcBuddy.left -= m_uiWidth - nEdge;
			break;

		default:
			ASSERT(FALSE);
		}

		::MoveWindow(m_hwndBuddy, rcBuddy.left, rcBuddy.top, rcBuddy.Width(), rcBuddy.Height(), TRUE);
	}
}

/////////////////////////////////////////////////////////////////////////////
//
BOOL CEGSpin::SetAlignment(EGSpinAlignmentEnum enAlignment /*= enAlignmentRightInside*/, BOOL bRedraw /*= TRUE*/)
{
	ASSERT(::IsWindow(m_hwndParent));
	ASSERT(::IsWindow(m_hwndBuddy));

	if(enAlignment != m_enAlignment)
	{
		_AlignWindow(FALSE);
		m_enAlignment = enAlignment;
		_AlignWindow(TRUE);

		if(bRedraw)
			RedrawWindow();
	}

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//
BOOL CEGSpin::SetOrientation(EGSpinOrientationEnum enOrientation /*= enOrientationVertical*/, BOOL bRedraw /*= TRUE*/)
{
	ASSERT(::IsWindow(m_hwndParent));
	ASSERT(::IsWindow(m_hwndBuddy));

	if(m_enOrientation != enOrientation)
	{
		m_enOrientation = enOrientation;
		if(bRedraw)
			RedrawWindow();
	}

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//
BOOL CEGSpin::SetWrap(BOOL bWrap /*= TRUE*/)
{
	ASSERT(::IsWindow(m_hwndParent));
	ASSERT(::IsWindow(m_hwndBuddy));

	if(m_bWrap != bWrap)
	{
		m_bWrap = bWrap;
	}

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//
BOOL CEGSpin::SetFlat(BOOL bFlat /*= TRUE*/, BOOL bRedraw /*= TRUE*/)
{
	ASSERT(::IsWindow(m_hwndParent));
	ASSERT(::IsWindow(m_hwndBuddy));

	if(m_bFlat != bFlat)
	{
		m_bFlat = bFlat;

		if(bRedraw)
			RedrawWindow();
	}

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//
BOOL CEGSpin::SetRange(double dMin, double dMax, BOOL bRedraw /*= TRUE*/)
{
	ASSERT(::IsWindow(m_hwndParent));
	ASSERT(::IsWindow(m_hwndBuddy));

	m_dMin = dMin;
	m_dMax = dMax;

	if(bRedraw)
		RedrawWindow();

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//
BOOL CEGSpin::SetStep(double dStep)
{
	ASSERT(::IsWindow(m_hwndParent));
	ASSERT(::IsWindow(m_hwndBuddy));

	m_dStep = dStep;

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//
BOOL CEGSpin::SetValue(double dValue, BOOL bRedraw /*= TRUE*/)
{
	ASSERT(::IsWindow(m_hwndParent));
	ASSERT(::IsWindow(m_hwndBuddy));

	m_dValue = dValue;

	if(bRedraw)
		RedrawWindow();

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//
BOOL CEGSpin::SetPrecision(UINT uiPresision)
{
	ASSERT(::IsWindow(m_hwndParent));
	ASSERT(::IsWindow(m_hwndBuddy));

	if(uiPresision != m_uiPresision)
	{
		ASSERT(uiPresision <= DBL_MAX_10_EXP);

		if(uiPresision > DBL_MAX_10_EXP)
			return FALSE;

		m_uiPresision = uiPresision;
		m_dEps = 0.1;
		while(uiPresision > 0)
		{
			m_dEps /= 10.;
			uiPresision--;
		}
	}

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//
BOOL CEGSpin::SetAccel(UINT uiSpinAccelCount, LPEGSpinAccel pSpinAccel)
{
	ASSERT(::IsWindow(m_hwndParent));
	ASSERT(::IsWindow(m_hwndBuddy));

	if(m_pSpinAccel)
	{
		m_uiSpinAccelCount = 0;
		delete [] m_pSpinAccel;
		m_pSpinAccel = NULL;
	}

	if(uiSpinAccelCount > 0)
	{
		if(pSpinAccel)
		{
			m_pSpinAccel = new EGSpinAccel[uiSpinAccelCount];
			::memcpy(m_pSpinAccel, pSpinAccel, uiSpinAccelCount * sizeof(EGSpinAccel));
			m_uiSpinAccelCount = uiSpinAccelCount;
		}
		else
			return FALSE;
	}

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//
void CEGSpin::_GetButtonRects(CRect& rcClient, CRect& rcUp, CRect& rcDn)
{
	rcUp = rcClient;

	int nXEdge = ::GetSystemMetrics(SM_CXEDGE);
	int nYEdge = ::GetSystemMetrics(SM_CYEDGE);

	rcUp.top += nYEdge;
	rcUp.bottom -= nYEdge;

	if(m_enAlignment == enAlignmentRightInside
		|| m_enAlignment == enAlignmentRightOutside)
	{
		rcUp.right -= nXEdge;
	}
	else
	{
		rcUp.left += nXEdge;
	}

	rcDn = rcUp;

	if(m_enOrientation == enOrientationVertical)
	{
		rcUp.bottom = rcUp.top + rcUp.Height() / 2;
		rcDn.top = rcDn.bottom - rcUp.Height();
	}
	else
	{
		rcUp.right = rcUp.left + rcUp.Width() / 2;
		rcDn.left = rcDn.right - rcUp.Width();
	}
}

/////////////////////////////////////////////////////////////////////////////
//
void CEGSpin::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	CRect rcClient;
	CRect rcUp, rcDn;

	GetClientRect(rcClient);

	CDC memdc;
	CBitmap bmpMem, *pOldMemBmp;
	memdc.CreateCompatibleDC(&dc);
	bmpMem.CreateCompatibleBitmap(&dc, rcClient.Width(), rcClient.Height());
	pOldMemBmp = memdc.SelectObject(&bmpMem);

	memdc.FillSolidRect(&rcClient, ::GetSysColor(COLOR_BTNFACE));

	_GetButtonRects(rcClient, rcUp, rcDn);

	UINT uiFlatState = (m_bFlat ? DFCS_FLAT : 0);
	BOOL bEnable = IsWindowEnabled();

	if(bEnable && _CompareDouble(m_dValue, m_dMax) < 0)
	{
		if(m_enPressedState == enPressedUpRight)
		{
			memdc.DrawFrameControl(&rcUp, DFC_SCROLL, DFCS_SCROLLUP | uiFlatState | (m_enPressedState == enPressedUpRight ? DFCS_PUSHED : 0));
			if(m_bFlat)
				memdc.Draw3dRect(rcUp, ::GetSysColor(COLOR_BTNSHADOW), ::GetSysColor(COLOR_BTNHILIGHT));
		}
		else
		{
			memdc.DrawFrameControl(&rcUp, DFC_SCROLL, DFCS_SCROLLUP | uiFlatState);
			if(m_bFlat)
				memdc.Draw3dRect(rcUp, ::GetSysColor(COLOR_BTNHILIGHT), ::GetSysColor(COLOR_BTNSHADOW));
		}
	}
	else
	{
		memdc.DrawFrameControl(&rcUp, DFC_SCROLL, DFCS_SCROLLUP | DFCS_INACTIVE | uiFlatState);
		if(m_bFlat)
			memdc.Draw3dRect(rcUp, ::GetSysColor(COLOR_BTNHILIGHT), ::GetSysColor(COLOR_BTNSHADOW));
	}

	if(bEnable && _CompareDouble(m_dValue, m_dMin) > 0)
	{
		if(m_enPressedState == enPressedDnLeft)
		{
			memdc.DrawFrameControl(&rcDn, DFC_SCROLL, DFCS_SCROLLDOWN | uiFlatState | (m_enPressedState == enPressedDnLeft ? DFCS_PUSHED : 0));
			if(m_bFlat)
				memdc.Draw3dRect(rcDn, ::GetSysColor(COLOR_BTNSHADOW), ::GetSysColor(COLOR_BTNHILIGHT));
		}
		else
		{
			memdc.DrawFrameControl(&rcDn, DFC_SCROLL, DFCS_SCROLLDOWN | uiFlatState);
			if(m_bFlat)
				memdc.Draw3dRect(rcDn, ::GetSysColor(COLOR_BTNHILIGHT), ::GetSysColor(COLOR_BTNSHADOW));
		}
	}
	else
	{
		memdc.DrawFrameControl(&rcDn, DFC_SCROLL, DFCS_SCROLLDOWN | DFCS_INACTIVE | uiFlatState);
		if(m_bFlat)
			memdc.Draw3dRect(rcDn, ::GetSysColor(COLOR_BTNHILIGHT), ::GetSysColor(COLOR_BTNSHADOW));
	}

	if(m_enBuddyState != enBuddyStateNormal && m_enBuddyState != enBuddyStateDisabled || !m_bFlat)
	{
		memdc.FillSolidRect(rcClient.left, rcClient.top + 1, rcClient.right - rcClient.left - 1, -1, ::GetSysColor(COLOR_BTNSHADOW));
		memdc.FillSolidRect(rcClient.left, rcClient.bottom, rcClient.right - rcClient.left, -1, ::GetSysColor(COLOR_BTNHILIGHT));
		memdc.FillSolidRect(rcClient.right, rcClient.top, -1, rcClient.bottom - rcClient.top, ::GetSysColor(COLOR_BTNHILIGHT));
		if(!m_bFlat)
		{
			memdc.FillSolidRect(rcClient.left, rcClient.top + 2, rcClient.right - rcClient.left - 2, -1, ::GetSysColor(COLOR_WINDOWFRAME));
			memdc.FillSolidRect(rcClient.right - 1, rcClient.top + 1, -1, rcClient.bottom - rcClient.top - 3, ::GetSysColor(COLOR_BTNFACE));
		}
	}

	dc.BitBlt(rcClient.left, rcClient.top, rcClient.Width(),
				rcClient.Height(), &memdc, rcClient.left, rcClient.top, SRCCOPY);

	memdc.SelectObject(pOldMemBmp);
}

/////////////////////////////////////////////////////////////////////////////
//
void CEGSpin::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if(!m_bMouseDown)
	{
		m_bMouseDown = TRUE;

		CRect rcClient, rcUp, rcDn;
		GetClientRect(rcClient);
		_GetButtonRects(rcClient, rcUp, rcDn);

		if(rcUp.PtInRect(point))
			m_enPressedState = enPressedUpRight;
		else if(rcDn.PtInRect(point))
			m_enPressedState = enPressedDnLeft;
		else
			m_enPressedState = enPressedNone;

		if(m_enPressedState != enPressedNone)
		{
			m_enActivePressedState = m_enPressedState;
			m_uiTimerPeriod = 500;
			SetTimer(1, m_uiTimerPeriod, NULL);
			_SendValueChanged();
		}
		::SetFocus(m_hwndBuddy);
	}
	
	CWnd::OnLButtonDown(nFlags, point);
}

/////////////////////////////////////////////////////////////////////////////
//
void CEGSpin::OnMouseMove(UINT nFlags, CPoint point) 
{
	CRect rcClient;
	GetClientRect(rcClient);

	BOOL bMouseOver = _PointInRect();

	if(!m_bMouseDown && m_bMouseOver != bMouseOver)
	{
		if(bMouseOver)
		{
			m_bMouseOver = TRUE;
			SetCapture();
			::SendMessage(m_hwndBuddy, EG_SM_MOUSEENTER, (WPARAM)0, (LPARAM)0);
		}
		else
		{
			m_bMouseOver = FALSE;
			ReleaseCapture();
			::SendMessage(m_hwndBuddy, EG_SM_MOUSEEXIT, (WPARAM)0, (LPARAM)0);
		}
	}

	if(m_bMouseDown)
	{
		if(bMouseOver)
		{
			EGSpinPressedStateEnum enCurPressedState;

			CRect rcUp, rcDn;
			_GetButtonRects(rcClient, rcUp, rcDn);

			if(rcUp.PtInRect(point))
				enCurPressedState = enPressedUpRight;
			else if(rcDn.PtInRect(point))
				enCurPressedState = enPressedDnLeft;

			if(m_enActivePressedState != m_enPressedState)
			{
				m_enPressedState = enCurPressedState;
				m_uiTimerPeriod = 500;
				SetTimer(1, m_uiTimerPeriod, NULL);
				_SendValueChanged();
			}
		}
		else
		{
			m_enPressedState = enPressedNone;
			RedrawWindow();
			KillTimer(1);
		}
	}
	CWnd::OnMouseMove(nFlags, point);
}

/////////////////////////////////////////////////////////////////////////////
//
void CEGSpin::OnLButtonUp(UINT nFlags, CPoint point) 
{
	if(m_bMouseDown)
	{
		m_bMouseDown = FALSE;
		//ReleaseCapture();
		KillTimer(1);
		m_enPressedState = enPressedNone;
		m_enActivePressedState = enPressedNone;
		RedrawWindow();
	}
	
	CWnd::OnLButtonUp(nFlags, point);
}

/////////////////////////////////////////////////////////////////////////////
//
BOOL CEGSpin::OnEraseBkgnd(CDC* pDC) 
{
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//
BOOL CEGSpin::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	if(IsWindowEnabled())
	{
		if(m_hCursor != NULL)
		{
			::SetCursor(m_hCursor);
			return TRUE;
		}
	}
	return CWnd::OnSetCursor(pWnd, nHitTest, message);
}

/////////////////////////////////////////////////////////////////////////////
//
void CEGSpin::OnSysColorChange() 
{
	CWnd::OnSysColorChange();
	
	RedrawWindow();	
}

/////////////////////////////////////////////////////////////////////////////
//
BOOL CEGSpin::_GetValidValue(EGSpinPressedStateEnum enPressedState, double& dValue, UINT nRepCnt /*= 1*/)
{
	if(enPressedState == enPressedUpRight)
	{
		if(_CompareDouble(m_dMax, m_dValue + m_dStep * nRepCnt) >= 0)
		{
			dValue = m_dValue + m_dStep * nRepCnt;
			return TRUE;
		}
		else if(_CompareDouble(m_dMax, m_dValue) != 0)
		{
			dValue = m_dMax;
			return TRUE;
		}
	}
	else
	{
		if(_CompareDouble(m_dMin, m_dValue - m_dStep * nRepCnt) <= 0)
		{
			dValue = m_dValue - m_dStep * nRepCnt;
			return TRUE;
		}
		else if(_CompareDouble(m_dMin, m_dValue) != 0)
		{
			dValue = m_dMin;
			return TRUE;
		}
	}

	return FALSE;
}

/////////////////////////////////////////////////////////////////////////////
//
void CEGSpin::_SendValueChanged()
{
	double dValue;
	if(_GetValidValue(m_enActivePressedState, dValue))
	{
		m_dValue = dValue;
		EG_SM_VALUECHANGED_DATA sm;
		sm.hwndFrom = GetSafeHwnd();
		sm.dValue = dValue;

		::SendMessage(m_hwndBuddy, EG_SM_VALUECHANGED, (WPARAM)0, (LPARAM)&sm);
		RedrawWindow();
	}
}

/////////////////////////////////////////////////////////////////////////////
//
void CEGSpin::OnTimer(UINT nIDEvent) 
{
	if(m_enPressedState != enPressedNone)
	{
		if(m_uiTimerPeriod == 500)
		{
			KillTimer(1);
			m_uiTimerPeriod = 100;
			SetTimer(1, m_uiTimerPeriod, NULL);
		}
		_SendValueChanged();
	}
	
	CWnd::OnTimer(nIDEvent);
}

/////////////////////////////////////////////////////////////////////////////
//
void CEGSpin::OnEnable(BOOL bEnable)
{
	CWnd::OnEnable(bEnable);
	
	RedrawWindow();	
}

/////////////////////////////////////////////////////////////////////////////
//
void CEGSpin::OnDestroy()
{
	_AlignWindow(FALSE);

	CWnd::OnDestroy();
}

/////////////////////////////////////////////////////////////////////////////
//
void CEGSpin::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if(nChar == VK_UP || nChar == VK_DOWN)
	{
		double dValue;
		EGSpinPressedStateEnum enCurPressedState;
		if(nChar == VK_UP)
			enCurPressedState = enPressedUpRight;
		else
			enCurPressedState = enPressedDnLeft;

		if(_GetValidValue(enCurPressedState, dValue, nRepCnt))
		{
			m_enPressedState = enCurPressedState;
			RedrawWindow();

			m_dValue = dValue;
			EG_SM_VALUECHANGED_DATA sm;
			sm.hwndFrom = GetSafeHwnd();
			sm.dValue = dValue;

			::SendMessage(m_hwndBuddy, EG_SM_VALUECHANGED, (WPARAM)0, (LPARAM)&sm);
		}
		return;
	}
	CWnd::OnKeyDown(nChar, nRepCnt, nFlags);
}

/////////////////////////////////////////////////////////////////////////////
//
void CEGSpin::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if(nChar == VK_UP || nChar == VK_DOWN)
	{
		m_enPressedState = enPressedNone;
		RedrawWindow();
		return;
	}
	CWnd::OnKeyUp(nChar, nRepCnt, nFlags);
}

/////////////////////////////////////////////////////////////////////////////
//
int CEGSpin::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message) 
{
	ASSERT(::IsWindow(m_hwndBuddy));

	if(::IsWindow(m_hwndBuddy))
	{
		::SetFocus(m_hwndBuddy);
	}
	return CWnd::OnMouseActivate(pDesktopWnd, nHitTest, message);
}

/////////////////////////////////////////////////////////////////////////////
//
void CEGSpin::OnBuddyStateChanged(EGSpinBuddyStateEnum enBuddyState, BOOL bRedraw /*= TRUE*/)
{
	ASSERT(::IsWindow(m_hwndBuddy));

	if(::IsWindow(m_hwndBuddy))
	{
		if((m_enBuddyState == enBuddyStateFocused || m_enBuddyState == enBuddyStateTracked) && enBuddyState == enBuddyStateNormal)
		{
			m_bMouseOver = FALSE;
			m_bMouseDown = FALSE;
			::SendMessage(m_hwndBuddy, EG_SM_MOUSEEXIT, (WPARAM)0, (LPARAM)0);
			KillTimer(1);
			if(::GetCapture() == m_hWnd)
				ReleaseCapture();
		}

		if(m_enBuddyState != enBuddyState)
		{
			m_enBuddyState = enBuddyState;

			if(bRedraw)
				RedrawWindow();
		}
	}
	else
	{
		m_enBuddyState = enBuddyStateNormal;
	}
}



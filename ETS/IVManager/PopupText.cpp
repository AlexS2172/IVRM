// PopupText.cpp : implementation file
//

#include "stdafx.h"
#include "ivanalyzer.h"
#include "PopupText.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPopupText


BEGIN_MESSAGE_MAP(CPopupText,CWnd)
	//{{AFX_MSG_MAP(CPopupText)
	ON_WM_PAINT()
	ON_MESSAGE(WM_SETTEXT, OnSetText)
	ON_MESSAGE (WM_GETTEXT, OnGetText)
	ON_MESSAGE (WM_GETTEXTLENGTH, OnGetTextLength)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CPopupText::CPopupText()
{
	m_szMargins = CSize(2,2);
	// create font -- use system tooltip font
	CNonClientMetrics ncm;
	m_font.CreateFontIndirect(&ncm.lfStatusFont);
}

CPopupText::~CPopupText()
{
}

//////////////////
// Create window. pt is upper left or upper right corner depending on nStyle.
//
BOOL
CPopupText::Create( CWnd* pParentWnd)
{
	m_nStyle = 0;
	return CreateEx(0,
		NULL,
		NULL,
		WS_POPUP,
		CRect(0,0,0,0),
		pParentWnd,
		0);
}

//////////////////
// Someone changed the text: resize to fit new text
//
LRESULT CPopupText::OnSetText(WPARAM wp, LPARAM lp)
{
	m_strText = (LPCTSTR) lp;

	CClientDC dc(this);
	CFont* pOldFont = dc.SelectObject(&m_font);
	CRect rc;
	
	GetClientRect(&rc);
	char* szContext = NULL;
	
	CString strText = m_strText;
	LPTSTR szText = strText.GetBuffer(1);
	LPTSTR szToken = _tcstok_s (szText, _T("\n"), &szContext);
	
	int nMaxWidth = 0;
	for ( ;szToken; szToken = _tcstok_s (NULL, _T("\n"), &szContext))
	{
		dc.DrawText(szToken, &rc, DT_SINGLELINE | DT_LEFT | DT_CALCRECT );
		if ( rc.Width() > nMaxWidth )
			nMaxWidth = rc.Width();
	}
	strText.ReleaseBuffer(0);

	rc.right = nMaxWidth;
	dc.DrawText(m_strText, &rc, DT_LEFT | DT_CALCRECT );

	rc.InflateRect(m_szMargins);

	AdjustWindowRect (rc, GetStyle(), FALSE);

	SetWindowPos (NULL, 0, 0, rc.Width(),rc.Height(), SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOZORDER);
	
	Invalidate(FALSE);
	UpdateWindow();
	return TRUE;
}

LRESULT CPopupText::OnGetText(WPARAM wParam, LPARAM lParam)
{
	if (! lParam)
		return 0;

	LPTSTR szBuffer = (LPTSTR) lParam;
	_tcsncpy_s (szBuffer,wParam, m_strText, wParam);
	if (wParam < static_cast<WPARAM>(m_strText.GetLength() + 1) )
		szBuffer[wParam] = _T('\0');
	
	return _tcslen (szBuffer);
}

LRESULT CPopupText::OnGetTextLength(WPARAM wParam, LPARAM lParam)
{
	return m_strText.GetLength();
}


//////////////////
// Paint the text. Use system colors
//
void CPopupText::OnPaint()
{
	CPaintDC dcTarget(this);

	CRect rc;
	GetClientRect (&rc);

	CDC dc;	
	dc.CreateCompatibleDC (&dcTarget);

	CBitmap bmp;
	bmp.CreateCompatibleBitmap (&dcTarget, rc.Width(), rc.Height());
	
	dc.SelectObject (&bmp);

	/////////////////////////


	CBrush b(GetSysColor(COLOR_INFOBK)); // use tooltip bg color
	dc.FillRect(&rc, &b);

	// draw text
	dc.SetBkMode(TRANSPARENT);
	CFont* pOldFont = dc.SelectObject(&m_font);
	dc.SetTextColor(GetSysColor(COLOR_INFOTEXT)); // tooltip text color
	
	CRect rcDraw = rc;
	rcDraw.DeflateRect (m_szMargins);
	dc.DrawText(m_strText, &rcDraw, DT_LEFT);
	
	dc.SelectObject(pOldFont);

	dcTarget.BitBlt(rc.left,rc.top,rc.Width(),rc.Height(),&dc,0,0,SRCCOPY);
}

//////////////////
// Register class if needed
//
BOOL CPopupText::PreCreateWindow(CREATESTRUCT& cs) 
{
	static CString sClassName;
	if (sClassName.IsEmpty())
		sClassName = AfxRegisterWndClass(0);
	cs.lpszClass = sClassName;
	cs.style |= WS_BORDER;
	cs.dwExStyle = WS_EX_TOOLWINDOW | WS_EX_TOPMOST;

	return CWnd::PreCreateWindow(cs);
}

//////////////////
// CPopupText is intended to be used on the stack,
// not heap, so don't auto-delete.
//
void CPopupText::PostNcDestroy()
{
	// don't delete this
}


BOOL CPopupText::PreTranslateMessage(MSG* pMsg) 
{
	CWnd *pWnd;
	int hittest;
	switch (pMsg->message)
	{
	case WM_LBUTTONDOWN:
	case WM_RBUTTONDOWN:
	case WM_MBUTTONDOWN:
		{
			POINTS pts = MAKEPOINTS( pMsg->lParam );
			POINT  point;
			point.x = pts.x;
			point.y = pts.y;
			ClientToScreen( &point );
			pWnd = WindowFromPoint( point );
			if( pWnd == this ) 
				pWnd = GetParent();

			hittest = (int)pWnd->SendMessage(WM_NCHITTEST,0,MAKELONG(point.x,point.y));

			if (hittest == HTCLIENT)
			{
				pWnd->ScreenToClient( &point );
				pMsg->lParam = MAKELONG(point.x,point.y);
			} 
			else 
			{
				switch (pMsg->message) 
				{
				case WM_LBUTTONDOWN: 
					pMsg->message = WM_NCLBUTTONDOWN;
					break;
				case WM_RBUTTONDOWN: 
					pMsg->message = WM_NCRBUTTONDOWN;
					break;
				case WM_MBUTTONDOWN: 
					pMsg->message = WM_NCMBUTTONDOWN;
					break;
				}
				pMsg->wParam = hittest;
				pMsg->lParam = MAKELONG(point.x,point.y);
			}

			ShowWindow(SW_HIDE);
			pWnd->PostMessage(pMsg->message,pMsg->wParam,pMsg->lParam);
			return TRUE;	
		}  break;		
	case WM_KEYDOWN:
	case WM_SYSKEYDOWN:
		{
			ShowWindow(SW_HIDE);
			GetParent()->PostMessage( pMsg->message, pMsg->wParam, pMsg->lParam );
			return TRUE;
		}break;
	}

	if( GetFocus() == NULL )
	{
		ShowWindow (SW_HIDE);
		return TRUE;
	}

	return CWnd::PreTranslateMessage(pMsg);
}
	

void CPopupText::Show(CPoint pt)
{
	CRect rc;
	GetWindowRect (rc);

	CSize szScreen;
	CWindowDC wdc(NULL);
	
	szScreen.cx = GetDeviceCaps(wdc, HORZRES) - 2;
	szScreen.cy = GetDeviceCaps(wdc, VERTRES) - 2;
	
	rc.OffsetRect ( pt - rc.TopLeft());

	if (rc.right > szScreen.cx )
		rc.OffsetRect ( szScreen.cx - rc.right, 0 );
	
	if (rc.bottom > szScreen.cy)
		rc.OffsetRect ( 0, szScreen.cy - rc.bottom);

	SetWindowPos (&wndTop, rc.left, rc.top, rc.Width(), rc.Height(), SWP_NOACTIVATE | SWP_SHOWWINDOW );
}

void CPopupText::Hide ()
{
	ShowWindow(SW_HIDE);
}
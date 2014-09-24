// IVControls.cpp : implementation file
//

#include "stdafx.h"
#include "IVAnalyzer.h"
#include "IVControls.h"
#include <afxpriv.h>
#include "ModulVer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CIVToolBar

CIVToolBar::CIVToolBar()
{
}

CIVToolBar::~CIVToolBar()
{
}


BEGIN_MESSAGE_MAP(CIVToolBar, CToolBar)
	//{{AFX_MSG_MAP(CIVToolBar)
//	ON_WM_NCHITTEST()
	ON_MESSAGE(WM_IDLEUPDATECMDUI, OnIdleUpdateCmdUI)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
	ON_WM_NCHITTEST()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIVToolBar message handlers

//UINT CIVToolBar::OnNcHitTest(CPoint point) 
//{	
//	CPoint ptCli = point;
//	ScreenToClient (&ptCli);
//	if ( GetToolBarCtrl().HitTest (&ptCli) < 0)
//		return HTTRANSPARENT;
//	else
//		return CToolBar::OnNcHitTest(point);
//}

LRESULT CIVToolBar::OnIdleUpdateCmdUI(WPARAM wParam, LPARAM)
{
     if (IsWindowVisible())
     {
        CFrameWnd* pParent = (CFrameWnd*)AfxGetMainWnd ();
        if (pParent)
           OnUpdateCmdUI(pParent, (BOOL)wParam);
     }
	 return 0L;
}

#define VERSION_WIN4    MAKELONG(0, 4)
#define VERSION_IE3     MAKELONG(70, 4)
#define VERSION_IE4     MAKELONG(71, 4)
#define VERSION_IE401   MAKELONG(72, 4)

int _afxComCtlVersion = -1;

DWORD  _AfxGetComCtlVersion()
{
	// return cached version if already determined...
	if (_afxComCtlVersion != -1)
		return _afxComCtlVersion;

	CModuleVersion ver;
	ver.GetModuleVersionInfo (_T("COMCTL32.DLL"));
	
	DLLVERSIONINFO dvi;
	DWORD dwVersion = VERSION_WIN4;
	if (ver.DllGetVersion (_T("COMCTL32.DLL"), dvi))
	{
		dwVersion = MAKELONG(dvi.dwMinorVersion, dvi.dwMajorVersion);
	}
	_afxComCtlVersion = dwVersion;

	return dwVersion;
}

int _AfxGetDropDownWidth()
{
	int _afxDropDownWidth = -1;  // This value must be not cached!

	// otherwise calculate it...
	HDC hDC = GetDC(NULL);
	ASSERT(hDC != NULL);
	HFONT hFont;
	if ((hFont = CreateFont(GetSystemMetrics(SM_CYMENUCHECK), 0, 0, 0,
		FW_NORMAL, 0, 0, 0, SYMBOL_CHARSET, 0, 0, 0, 0, _T("Marlett"))) != NULL)
		hFont = (HFONT)SelectObject(hDC, hFont);
	VERIFY(GetCharWidth(hDC, '6', '6', &_afxDropDownWidth));
	if (hFont != NULL)
	{
		SelectObject(hDC, hFont);
		DeleteObject(hFont);
	}
	ReleaseDC(NULL, hDC);
	ASSERT(_afxDropDownWidth != -1);
	return _afxDropDownWidth;
}


#define CX_OVERLAP 0

// Will work only for unwrappable hosizontal toolbars  
// Fixes MFC bug with drop-down buttons when system metrics are changed
// It's a copy of CalcSize() protected function: fixed drop-down width
CSize CIVToolBar::CalcFixedLayout(BOOL bStretch, BOOL bHorz)
{
	CToolBarCtrl & tb = GetToolBarCtrl();
	int nCount = tb.GetButtonCount();

	ASSERT(nCount > 0);

	CPoint cur(0,0);
	CSize sizeResult(0,0);

	DWORD dwExtendedStyle = tb.GetExtendedStyle();

	for (int i = 0; i < nCount; i++)
	{
		TBBUTTON btn;
		tb.GetButton(i, &btn);
		
		//WINBUG: The IE4 version of COMCTL32.DLL calculates the separation
		//  on a TBSTYLE_WRAP button as 100% of the value in iBitmap compared
		//  to the other versions which calculate it at 2/3 of that value.
		//  This is actually a bug which should be fixed in IE 4.01, so we
		//  only do the 100% calculation specifically for IE4.
		int cySep = btn.iBitmap;
		if (!(GetStyle() & TBSTYLE_FLAT) && _AfxGetComCtlVersion() != VERSION_IE4)
			cySep = cySep * 2 / 3;

		if (btn.fsState & TBSTATE_HIDDEN)
			continue;

		int cx = m_sizeButton.cx;
		if (btn.fsStyle & TBSTYLE_SEP)
		{
			// a separator represents either a height or width
			if (btn.fsState & TBSTATE_WRAP)
				sizeResult.cy = max(cur.y + m_sizeButton.cy + cySep, sizeResult.cy);
			else
				sizeResult.cx = max(cur.x + btn.iBitmap, sizeResult.cx);
		}
		else
		{
			// check for dropdown style, but only if the buttons are being drawn
			if ((btn.fsStyle & TBSTYLE_DROPDOWN) &&
				(dwExtendedStyle & TBSTYLE_EX_DRAWDDARROWS))
			{
				cx += _AfxGetDropDownWidth();
			}
			sizeResult.cx = max(cur.x + cx, sizeResult.cx);
			sizeResult.cy = max(cur.y + m_sizeButton.cy, sizeResult.cy);
		}

		if (btn.fsStyle & TBSTYLE_SEP)
			cur.x += btn.iBitmap;
		else
			cur.x += cx - CX_OVERLAP;

		if (btn.fsState & TBSTATE_WRAP)
		{
			cur.x = 0;
			cur.y += m_sizeButton.cy;
			if (btn.fsStyle & TBSTYLE_SEP)
				cur.y += cySep;
		}
	}
	return sizeResult;
	
}
CSize CIVToolBar::CalcDynamicLayout(int nLength, DWORD dwMode)
{
	return CalcFixedLayout(dwMode & LM_STRETCH, dwMode & LM_HORZDOCK);
}

//////////////////// CIVRebar

void CIVReBar::AdjustBorders ()
{
	DWORD dwStyle = GetBarStyle();
	dwStyle &= ~CBRS_BORDER_ANY;
	
	dwStyle |= CBRS_BORDER_LEFT | CBRS_BORDER_3D;

	SetBarStyle(dwStyle);
}


///////////////////////////////////////////////////////////////////////

CIVInfoLabel::CIVInfoLabel()
{
}

CIVInfoLabel::~CIVInfoLabel()
{
}


BEGIN_MESSAGE_MAP(CIVInfoLabel, CStatic)
	//{{AFX_MSG_MAP(CIVInfoLabel)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
	ON_MESSAGE (WM_SETTEXT, OnSetText)
	ON_MESSAGE (WM_GETTEXT, OnGetText)
	ON_MESSAGE (WM_GETTEXTLENGTH, OnGetTextLength)
END_MESSAGE_MAP()


/////////////////////////////////////////////////
// CIVInfoLabel message handlers



void CIVInfoLabel::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	DrawLabel (dc);	
}


LRESULT CIVInfoLabel::OnSetText(WPARAM wParam, LPARAM lParam)
{
	m_strText = (LPCTSTR) lParam;

	Invalidate(FALSE);
	UpdateWindow();

	return TRUE;
}

LRESULT CIVInfoLabel::OnGetText(WPARAM wParam, LPARAM lParam)
{
	if (! lParam)
		return 0;

	LPTSTR szBuffer = (LPTSTR) lParam;
	_tcsncpy_s (szBuffer, wParam, m_strText, wParam);
	if (wParam < static_cast<WPARAM>(m_strText.GetLength() +1) )
		szBuffer[wParam] = _T('\0');
	
	return _tcslen (szBuffer);
}

LRESULT CIVInfoLabel::OnGetTextLength(WPARAM wParam, LPARAM lParam)
{
	return m_strText.GetLength();
}


void CIVInfoLabel::DrawLabel(CDC & dcTarget)
{
	CRect rc;
	GetClientRect (&rc);

	CDC dc;	
	dc.CreateCompatibleDC (&dcTarget);

	CBitmap bmp;
	bmp.CreateCompatibleBitmap (&dcTarget, rc.Width(), rc.Height());
	
	dc.SelectObject (&bmp);
	
	CBrush brush;
	brush.CreateSysColorBrush (COLOR_3DFACE);

	dc.SelectObject (&brush);
	dc.PatBlt (0,0, rc.Width(), rc.Height(), PATCOPY);
	

	CFont font;
	font.CreateStockObject (DEFAULT_GUI_FONT);
	dc.SelectObject (&font);

	dc.SetBkColor ( GetSysColor (COLOR_3DFACE));
	dc.SetTextColor ( GetSysColor (COLOR_WINDOWTEXT));

	CRect rcCalc = rc;
	
	LPTSTR szStr =  (LPTSTR) (LPCTSTR) m_strText;

	DrawTextEx (dc, szStr, -1, &rcCalc,  DT_CALCRECT | DT_CENTER | DT_WORDBREAK, NULL);
	
	int nOffset = rcCalc.Height() < rc.Height() 
		? ( rc.Height() - rcCalc.Height() ) / 2 : 0;

	rcCalc = rc;
	rcCalc.top += nOffset;

	DrawTextEx (dc, szStr, -1, &rcCalc,  DT_CENTER | DT_WORDBREAK, NULL);
	
	dcTarget.BitBlt(rc.left,rc.top,rc.Width(),rc.Height(),&dc,0,0,SRCCOPY);
}

////////////////////////////////////////////////////////////

static LPCTSTR m_szPrefixes [] = {_T("http://"), _T("www."), NULL };


BEGIN_MESSAGE_MAP(CIVStatusBar, CStatusBar)
	//{{AFX_MSG_MAP(CIVStatusBar)
//	ON_WM_NCHITTEST()
	ON_WM_SETCURSOR()
	ON_WM_LBUTTONDOWN()
	ON_MESSAGE(WM_IDLEUPDATECMDUI, OnIdleUpdateCmdUI)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
	ON_WM_NCHITTEST()
END_MESSAGE_MAP()

CIVStatusBar::CIVStatusBar():
	m_hHandCursor (NULL)
{
	m_hHandCursor = LoadCursor (NULL, MAKEINTRESOURCE(IDC_HAND));
	if (m_hHandCursor == NULL )
		m_hHandCursor = AfxGetApp()->LoadCursor (IDC_HAND_PRE_WIN2K);
}

CIVStatusBar::~CIVStatusBar()
{
}

//LRESULT CIVStatusBar::OnNcHitTest(CPoint point) 
//{
//}


void CIVStatusBar::DrawItem(LPDRAWITEMSTRUCT lpdis)
{
	CDC dc;
	dc.Attach(lpdis->hDC);
	
	CString strText = GetPaneText( lpdis->itemID);

	CRect rc = lpdis->rcItem; 
	CRect rcTmp = rc;

	COLORREF clrBack = dc.SetBkColor (GetSysColor (COLOR_3DFACE));
	COLORREF clrText = dc.SetTextColor (GetSysColor (COLOR_WINDOWTEXT));

	CFont * pFont =  GetFont();

	CFont fntURL;
	GetLinkFont(fntURL);
	CFont * pOldFont =  dc.SelectObject (pFont);
	
	int x = CalcX (&dc, rc, strText);
	int y = rc.top; 

	for (int i = 0; i < strText.GetLength();)
	{
		CSize szPos = FindURL (strText, i );
		if (szPos.cx != -1)
		{
			CString strStart  = strText.Mid (i, szPos.cx-i);
			
			dc.TextOut (x, y, strStart);
			x += dc.GetTextExtent (strStart).cx;

			COLORREF clrText = dc.SetTextColor ( GetSysColor(COLOR_HIGHLIGHT));

			pFont = dc.SelectObject (&fntURL);
			
			
			CString strURL = strText.Mid(szPos.cx, szPos.cy);
			dc.TextOut (x, y, strURL);
			
			x += dc.GetTextExtent (strURL).cx;
			
			dc.SelectObject (pFont);
			dc.SetTextColor (clrText);

			i = szPos.cx + szPos.cy;
		}
		else
		{
			CString strStart  = strText.Mid (i);
			dc.TextOut (x, y, strStart);
			break;
		}

	}

	dc.SelectObject (pOldFont);

	dc.SetBkColor (clrBack);
	dc.SetTextColor (clrText);
	
	dc.Detach ();	

}


CSize CIVStatusBar::FindURL(const CString & str, int nStart)
{
	static LPCTSTR szPrefixes [] = {_T("http://"), _T("www."), NULL };


	int nPos = -1;
	for (int i = 0; szPrefixes[i];i++  )
	{
		nPos = str.Find (szPrefixes[i], nStart);
		int nCnt = str.GetLength()-nPos;
		
		if (nPos != -1)
		{
			bool bContinue= true;
			int nEnd = nPos;

			for (; nEnd < str.GetLength() && bContinue; nEnd++ )
			{	
				switch (str.GetAt(nEnd) )
				{
				case _T('.'):
				case _T(','):
					if (  (nEnd < str.GetLength()-1  && str.GetAt(nEnd+1) == _T(' ')) ||
						 nEnd == str.GetLength()-1 )
					bContinue = false;
					break;
				case _T(' '):
					bContinue = false;
					break;
				}
			}

			if (! bContinue)
				nCnt = nEnd - 1 - nPos;

			return CSize (nPos, nCnt);
		}
	}
	
	return CSize(-1, -1);
}


bool CIVStatusBar::IsOverURL(CPoint pt, CString & rstr )
{

	if ( ! IsWindowEnabled()  || !GetTopLevelParent()->IsWindowEnabled()   )
		return false;

	CStatusBarCtrl & sb = GetStatusBarCtrl();
	int iPart = -1;
	CRect rc;
	for (int i=0; i < sb.GetParts(0, NULL); i++ )
	{
		sb.GetRect (i, rc);
		if (rc.PtInRect(pt) && ( GetPaneStyle(i) & SBT_OWNERDRAW )  )
		{
			iPart = i;
			break;
		}
	}

	if (iPart == -1)
		return false;

	if ( ! (GetPaneStyle(iPart)  & SBPS_NOBORDERS) )
		rc.InflateRect(-GetSystemMetrics(SM_CYBORDER), -GetSystemMetrics(SM_CXBORDER));

	if (!rc.PtInRect(pt))
		return false;

	CString strText = GetPaneText(iPart);
	CDC * pDC = GetDC();

	CFont * pFont = GetFont();
	CFont fntURL;
	GetLinkFont(fntURL);
	
	CFont * pOldFont =  pDC->SelectObject (pFont);

	int x = CalcX (pDC, rc, strText);

	bool bFound = false;
	for (int i = 0; i < strText.GetLength();)
	{
		CSize szPos = FindURL (strText, i );
		if (szPos.cx != -1)
		{
			CString strStart = strText.Mid (i, szPos.cx-i);
			CRect rcURL;
			x += pDC->GetTextExtent (strStart).cx;
			rcURL.left = x;
	
			pFont = pDC->SelectObject (&fntURL);
		
			CString strURL = strText.Mid(szPos.cx, szPos.cy);
			
			CSize szURL =  pDC->GetTextExtent (strURL);
			x += szURL.cx;
			
			rcURL.right = x;
			rcURL.top = rc.top;
			rcURL.bottom = rc.top + szURL.cy;

			pDC->SelectObject (pFont);
			
			if (rcURL.PtInRect(pt))
			{
				rstr = strURL;
				bFound = true;
				break;
			}

			i = szPos.cx + szPos.cy;
		}
		else
			break;

	}

	pDC->SelectObject (pOldFont);


	return bFound;
}

void CIVStatusBar::GetLinkFont(CFont &fnt)
{
	CFont * pFont =  GetFont();
	ASSERT_VALID (pFont);

	LOGFONT lf;
	pFont->GetLogFont (&lf);

	lf.lfUnderline = TRUE;
	
	fnt.CreateFontIndirect (&lf);
}

BOOL CIVStatusBar::SetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	if  (pWnd == this )
	{
		CPoint pt = GetMessagePos ();
		ScreenToClient(&pt);

		if (IsOverURL(pt))
		{
			::SetCursor (m_hHandCursor);
			return TRUE;
		}
	}

	::SetCursor (LoadCursor (NULL, MAKEINTRESOURCE(IDC_ARROW)));
	return TRUE;
}

BOOL CIVStatusBar::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	return SetCursor(pWnd, nHitTest, message);
}

void CIVStatusBar::OnLButtonDown(UINT nFlags, CPoint point) 
{
	LButtonDown(nFlags, point);
}


void CIVStatusBar::LButtonDown(UINT nFlags, CPoint point) 
{
	CString strURL;
	if ( IsOverURL(point, strURL) )
	{
		ShellExecute(NULL, _T("open"), strURL, NULL,NULL, SW_SHOW);
	}
}


LRESULT CIVStatusBar::OnIdleUpdateCmdUI(WPARAM wParam, LPARAM)
{
	 if (IsWindowVisible())
     {
        CFrameWnd* pParent = (CFrameWnd*)AfxGetMainWnd ();
        if (pParent)
           OnUpdateCmdUI(pParent, (BOOL)wParam);
     }
	 return 0L;
}

void CIVStatusBar::OnPaint() 
{
	// Removed UpdateAllPanes because of unexpected issues with owner-draw panes
	Default();
}

void CIVToolBar::OnDestroy() 
{
	CToolBar::OnDestroy();
	
	if (m_pStringMap)
		m_pStringMap->RemoveAll();
	
}

LRESULT CIVToolBar::OnNcHitTest(CPoint point)
{
	CPoint ptCli = point;
	ScreenToClient (&ptCli);
	if ( GetToolBarCtrl().HitTest (&ptCli) < 0)
		return HTTRANSPARENT;
	else
		return CToolBar::OnNcHitTest(point);
}

LRESULT CIVStatusBar::OnNcHitTest(CPoint point)
{
	return HTTRANSPARENT;
}

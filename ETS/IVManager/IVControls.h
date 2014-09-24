#if !defined(AFX_IVCONTROLS_H__D717894A_0CC4_40C8_AFAE_7519012801B0__INCLUDED_)
#define AFX_IVCONTROLS_H__D717894A_0CC4_40C8_AFAE_7519012801B0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// IVControls.h : header file
//


class CIVReBar: public CReBar
{
public:
	void AdjustBorders ();
};


/////////////////////////////////////////////////////////////////////////////
// CIVToolBar window

class CIVToolBar : public CToolBar
{
// Construction
public:
	CIVToolBar();

// Attributes
public:

// Operations
public:
	virtual CSize CalcFixedLayout(BOOL bStretch, BOOL bHorz);
	virtual CSize CalcDynamicLayout(int nLength, DWORD dwMode);

	void AdjustCount()
	{
		m_nCount = (int)DefWindowProc(TB_BUTTONCOUNT, 0, 0);
		m_bDelayedButtonLayout = TRUE;
	}

	void AdjustSizes(int iButton = 0)
	{
		CRect rect;
		GetItemRect(iButton, &rect);
		
        SetSizes(rect.Size(), m_sizeImage);
 	}
	void SetButtonTextByCmd (UINT nID )
	{
		int i = GetToolBarCtrl().CommandToIndex(nID);
		SetButtonText (i, CString (MAKEINTRESOURCE (nID) ) );
	}
	void SetButtonStyleByCmd (UINT nID, DWORD dwStyle)
	{
		int i = GetToolBarCtrl().CommandToIndex(nID);

		SetButtonStyle(i, dwStyle);
	}

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIVToolBar)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CIVToolBar();

	// Generated message map functions
protected:
	//{{AFX_MSG(CIVToolBar)
//	afx_msg UINT OnNcHitTest(CPoint point);
	afx_msg LRESULT OnIdleUpdateCmdUI(WPARAM wParam, LPARAM);
	afx_msg void OnDestroy();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
public:
	afx_msg LRESULT OnNcHitTest(CPoint point);
};

////////////////////////////////////////////////////////////////////////////////

class CIVStatusBar: public CStatusBar
{
public:
	CIVStatusBar();

	CSize CalcFixedLayout(BOOL bStretch, BOOL bHorz)
	{
		CSize size =  CStatusBar::CalcFixedLayout (bStretch, bHorz);
		size.cx = 0;
		return size;
	}

	bool IsOverURL(CPoint pt, CString & rstr = CString () );
	int CalcX (CDC * pDC, const CRect & rc, LPCTSTR szText)
	{
		CRect rcTmp;
		pDC->DrawText (szText, &rcTmp, DT_RIGHT | DT_CALCRECT );
		int x = rc.left + ( rc.Width() - rcTmp.Width() );
		return x;
	}

	BOOL SetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	void LButtonDown(UINT nFlags, CPoint point); 
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIVStatusBar)
   virtual void DrawItem(LPDRAWITEMSTRUCT lpdis);
   	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CIVStatusBar();
	
	// Generated message map functions
protected:
	HCURSOR m_hHandCursor;
	void GetLinkFont(CFont &fnt);
	static LPCTSTR m_szPrefixes [];

	CSize FindURL(const CString & str, int nStart = -1);

	//{{AFX_MSG(CIVStatusBar)
//	afx_msg UINT OnNcHitTest(CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg LRESULT OnIdleUpdateCmdUI(WPARAM wParam, LPARAM);
	afx_msg void OnPaint();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

public:
	afx_msg LRESULT OnNcHitTest(CPoint point);
};

/////////////////////////////////////////////////////////////////////////////

class CIVInfoLabel : public CStatic
{
// Construction
public:
	CIVInfoLabel();

// Attributes
public:

// Operations
public:


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIVInfoLabel)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CIVInfoLabel();

protected:
	CString m_strText;
	// Generated message map functions
protected:
	void DrawLabel ( CDC & dcTarget );
	afx_msg LRESULT OnSetText(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnGetText(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnGetTextLength(WPARAM wParam, LPARAM lParam);

	//{{AFX_MSG(CIVInfoLabel)
	afx_msg void OnPaint();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IVCONTROLS_H__D717894A_0CC4_40C8_AFAE_7519012801B0__INCLUDED_)

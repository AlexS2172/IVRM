#if !defined(AFX_FLATCOMBOBOX_H__F891EC0F_743E_44F3_960C_06075A8BF906__INCLUDED_)
#define AFX_FLATCOMBOBOX_H__F891EC0F_743E_44F3_960C_06075A8BF906__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FlatComboBox.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFlatComboBox window

class CFlatComboBox : public CComboBox
{
public:
	
	CFlatComboBox();
	virtual ~CFlatComboBox();

	void SetTextColor(COLORREF clrText, BOOL bRedraw = TRUE);
	inline COLORREF GetTextColor() const { return m_clrText; }

	void SetBkColor(COLORREF clrBack, BOOL bRedraw = TRUE);
	inline COLORREF GetBkColor() const { return m_clrBack; }

	virtual void EnableAutoCompletion(BOOL bEnable=TRUE)
		{ ASSERT(::IsWindow(m_hWnd)); m_bAutoComp = bEnable; }

	virtual BOOL PointInRect()
		{ ASSERT(::IsWindow(m_hWnd)); CPoint pt; GetCursorPos(&pt);
		CRect rcItem; GetWindowRect(&rcItem); return rcItem.PtInRect(pt); }

protected:

	enum StateEnum { enNormal = 1, enRaised = 2, enPressed = 3 };
	virtual void DrawCombo(StateEnum eState, COLORREF clrTopLeft, COLORREF clrBottomRight, CDC* pDC=NULL);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFlatComboBox)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

	// Generated message map functions
protected:
	//{{AFX_MSG(CFlatComboBox)
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnPaint();
	afx_msg void OnSetFocus();
	afx_msg void OnKillFocus();
	afx_msg void OnEditUpdate();
	afx_msg void OnSysColorChange();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

protected:
	DECLARE_DYNAMIC(CFlatComboBox)

	BOOL		m_bLBtnDown;
	BOOL		m_bPainted;
	BOOL		m_bHasFocus;
	BOOL		m_bAutoComp;
	COLORREF	m_clrHilite;
	COLORREF	m_clrShadow;
	COLORREF	m_clrNormal;
	COLORREF	m_clrText;	
	COLORREF	m_clrBack;	

	static const UINT m_nTimerID; 
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FLATCOMBOBOX_H__F891EC0F_743E_44F3_960C_06075A8BF906__INCLUDED_)

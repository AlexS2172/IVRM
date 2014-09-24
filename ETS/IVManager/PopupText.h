#if !defined(AFX_POPUPTEXT_H__EE8D5FB0_6FE8_406B_8811_923F4F098CAC__INCLUDED_)
#define AFX_POPUPTEXT_H__EE8D5FB0_6FE8_406B_8811_923F4F098CAC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PopupText.h : header file
//

//////////////////
// Get NONCLIENTMETRICS info: ctor calls SystemParametersInfo.
//
class CNonClientMetrics : public NONCLIENTMETRICS {
public:
	CNonClientMetrics() {
		cbSize = sizeof(NONCLIENTMETRICS);
		SystemParametersInfo(SPI_GETNONCLIENTMETRICS,0,this,0);
	}
};

//////////////////
// Popup text window, like tooltip but no timer stuff.
// Can be right or left justified relative to creation point.
//
class CPopupText : public CWnd {
public:
	void Show(CPoint pt);
	void Hide();
	CSize m_szMargins;		// extra space around text: change if you like
	CPopupText();
	virtual ~CPopupText();
	BOOL Create(CWnd* pParentWnd);


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPopupText)
	public:
	virtual void PostNcDestroy();
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

protected:
	CFont	m_font;				// font to use (same as tooltips)
	UINT	m_nStyle;			// style (see enum below)
	CString m_strText;
	
	// Generated message map functions
protected:
	//{{AFX_MSG(CPopupText)
	afx_msg void OnPaint();
	afx_msg LRESULT OnSetText(WPARAM wp, LPARAM lp);
	afx_msg LRESULT OnGetText(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnGetTextLength(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG
	
	
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_POPUPTEXT_H__EE8D5FB0_6FE8_406B_8811_923F4F098CAC__INCLUDED_)

#if !defined(AFX_INFOBAR_H__84733258_5EC5_4496_8324_F256712A28AC__INCLUDED_)
#define AFX_INFOBAR_H__84733258_5EC5_4496_8324_F256712A28AC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// InfoBar.h : header file
//
#include "IVControls.h"
#include "PictureEx.h"
/////////////////////////////////////////////////////////////////////////////
// CInfoBar dialog

class CInfoBar : public CDialog
{
// Construction
public:
	CInfoBar(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CInfoBar)
	enum { IDD = IDD_INFO_BAR };
	CPictureEx	m_picLogo;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInfoBar)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CFont m_fnt;
	// Generated message map functions
	//{{AFX_MSG(CInfoBar)
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
//	afx_msg UINT OnNcHitTest(CPoint point);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSysColorChange();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg LRESULT OnNcHitTest(CPoint point);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INFOBAR_H__84733258_5EC5_4496_8324_F256712A28AC__INCLUDED_)

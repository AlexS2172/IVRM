#if !defined(AFX_ICONBAR_H__3920C6F6_C6D0_495A_93E7_7BD5B2420BE7__INCLUDED_)
#define AFX_ICONBAR_H__3920C6F6_C6D0_495A_93E7_7BD5B2420BE7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// IconBar.h : header file
//
#include "IVControls.h"
#include "PictureEx.h"

/////////////////////////////////////////////////////////////////////////////
// CIconBar dialog

class CIconBar : public CDialog
{
// Construction
public:
	CIconBar(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CIconBar)
	enum { IDD = IDD_ICON_BAR };
	CPictureEx	m_picIcon;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIconBar)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CIconBar)
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg LRESULT OnNcHitTest(CPoint point);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSysColorChange();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ICONBAR_H__3920C6F6_C6D0_495A_93E7_7BD5B2420BE7__INCLUDED_)

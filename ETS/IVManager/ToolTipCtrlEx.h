#if !defined(AFX_TOOLTIPCTRLEX_H__7E1C3EB4_FFBE_414A_A114_1D93962A113B__INCLUDED_)
#define AFX_TOOLTIPCTRLEX_H__7E1C3EB4_FFBE_414A_A114_1D93962A113B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ToolTipCtrlEx.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CToolTipCtrlEx window

class CToolTipCtrlEx : public CToolTipCtrl
{
// Construction
public:
	CToolTipCtrlEx();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CToolTipCtrlEx)
	public:
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CToolTipCtrlEx();

	// Generated message map functions
protected:
	//{{AFX_MSG(CToolTipCtrlEx)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TOOLTIPCTRLEX_H__7E1C3EB4_FFBE_414A_A114_1D93962A113B__INCLUDED_)

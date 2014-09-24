#if !defined(AFX_CHARTWND_H__2B664A9F_2C66_40FD_8342_A9EEB644AA76__INCLUDED_)
#define AFX_CHARTWND_H__2B664A9F_2C66_40FD_8342_A9EEB644AA76__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ChartWnd.h : header file
//

#include "ChartDlg.h"
/////////////////////////////////////////////////////////////////////////////
// CChartWnd window

// Overlapped window that contains chart dialog as child
// Used instead of CFormView to avoid numerous issues in doc/view absence

class CChartWnd : public CWnd
{
// Construction
public:
	CChartWnd();
	CChartWnd(DATE dtMonth);
public:

	void InitializeChart()
	{
		if (IsWindow(m_hWnd) && IsWindowVisible () )
		{
			ASSERT_VALID (m_pDlg);
			m_pDlg->InitializeChart();
		}
	}

	void UpdateChart( bool bReinitialize )
	{
		if (IsWindow(m_hWnd) && IsWindowVisible () )
		{
			ASSERT_VALID (m_pDlg);

			m_pDlg->UpdateChart( bReinitialize );
		}
	}

	
#ifdef _VOLA_MANAGER
	void UpdateChart( UINT nMessage, CCustomVolaNotification *pNotification )
	{
		if (IsWindow(m_hWnd) && IsWindowVisible () )
		{
			ASSERT_VALID (m_pDlg);

			m_pDlg->UpdateCustomVolatility( nMessage, pNotification );
		}
	}
#endif //_VOLA_MANAGER

public:
	DECLARE_DYNAMIC (CChartWnd)

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChartWnd)
	public:
	//}}AFX_VIRTUAL

protected:
	CChartDlg * m_pDlg;
// Implementation
public:
	void Show();
	BOOL Create();
	virtual ~CChartWnd();

	// Generated message map functions
protected:
	void ResizeCtrls();
	//{{AFX_MSG(CChartWnd)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	afx_msg void OnClose();
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI);
//	afx_msg BOOL OnHelpInfo( HELPINFO* lpHelpInfo );
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHARTWND_H__2B664A9F_2C66_40FD_8342_A9EEB644AA76__INCLUDED_)

// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__B132627C_9C06_44DC_B75E_8F566C714A5F__INCLUDED_)
#define AFX_MAINFRM_H__B132627C_9C06_44DC_B75E_8F566C714A5F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ChildView.h"
#include "Publisher.h"

#define WM_INSERT_TRACE_MESSAGE     WM_USER + 0x040
#define WM_STOP_ENDOFDAY_PROCESS    WM_USER + 0x041

class CMainFrame : public CFrameWnd
{
	
public:
	CMainFrame();
protected: 
	DECLARE_DYNAMIC(CMainFrame)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CStatusBar  m_wndStatusBar;
	CToolBar    m_wndToolBar;
	CChildView  m_wndView;

	//CAdapter*   m_pAdapter;
	//static UINT __stdcall AdapterProc(PVOID);
	//HANDLE		m_hThread;
    
	CPublisher	m_Publisher;
	BOOL    	m_bIsRunning;

    void NeedToUpdateSettings(); 

// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSetFocus(CWnd *pOldWnd);
	afx_msg void OnSettings();
	afx_msg void OnActionStart();
    afx_msg LRESULT OnInsertTraceMessage(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT OnStopEndofdayProcess(WPARAM wParam, LPARAM lParam);
	afx_msg void OnActionStop();
	afx_msg void OnUpdateActionStart(CCmdUI* pCmdUI);
	afx_msg void OnUpdateActionStop(CCmdUI* pCmdUI);
	afx_msg void OnUpdateSettings(CCmdUI* pCmdUI);
	afx_msg void OnActionEndofdayStart();
	afx_msg void OnUpdateActionSs(CCmdUI* pCmdUI);
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	BOOL IsRunning();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__B132627C_9C06_44DC_B75E_8F566C714A5F__INCLUDED_)

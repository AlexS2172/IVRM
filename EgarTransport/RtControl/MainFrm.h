// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__E16ACE74_4622_4207_BDF1_2E77115BCFDC__INCLUDED_)
#define AFX_MAINFRM_H__E16ACE74_4622_4207_BDF1_2E77115BCFDC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RtControl.h"
#include "SystemTray.h"
#include "RtControlView.h"

class CMainFrame : public CFrameWnd
{
	
protected: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg LRESULT OnTaskbarCreated(WPARAM wp, LPARAM lp);
	afx_msg void OnClose();
	afx_msg LRESULT OnRouterServiceError(WPARAM, LPARAM);	
	afx_msg LRESULT OnRouterUpdateStatus(WPARAM, LPARAM);
	afx_msg BOOL OnQueryEndSession();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	void UpdateRtStatus();
	void AddClient(LPCTSTR lpszClient, unsigned long ulHostAddr);
	void RemoveClient(LPCTSTR lpszClient);
	void RemoveAllClients();

protected:
	CSystemTray				m_stTray;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__E16ACE74_4622_4207_BDF1_2E77115BCFDC__INCLUDED_)

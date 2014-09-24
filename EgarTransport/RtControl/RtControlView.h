// RtControlView.h : interface of the CRtControlView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_RTCONTROLVIEW_H__E6B13C08_AD4C_4144_AA66_FD84C2032E6E__INCLUDED_)
#define AFX_RTCONTROLVIEW_H__E6B13C08_AD4C_4144_AA66_FD84C2032E6E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RtControlDoc.h"

class CRtControlView : public CFormView
{
protected: // create from serialization only
	CRtControlView();
	DECLARE_DYNCREATE(CRtControlView)

public:
	//{{AFX_DATA(CRtControlView)
	enum { IDD = IDD_RTCONTROL };
	CButton	m_chkStartMin;
	CButton	m_chkAutoStart;
	CListBox	m_lstHost;
	CButton		m_btnStop;
	CButton		m_btnStart;
	CStatic		m_stcStateImage;
	//}}AFX_DATA

// Attributes
public:
	CRtControlDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRtControlView)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnInitialUpdate(); // called first time after construct
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CRtControlView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

public:
	void UpdateRtStatus();
	void AddClient(LPCTSTR lpszClient, unsigned long ulHostAddr);
	void RemoveClient(LPCTSTR lpszClient);
	void RemoveAllClients();

// Generated message map functions
protected:
	//{{AFX_MSG(CRtControlView)
	afx_msg void OnStartButton();
	afx_msg void OnStopButton();
	afx_msg void OnAutostartCheck();
	afx_msg void OnStartMinCheck();
	afx_msg void OnCloseButton();
	afx_msg void OnDblclkHostList();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in RtControlView.cpp
inline CRtControlDoc* CRtControlView::GetDocument()
   { return (CRtControlDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RTCONTROLVIEW_H__E6B13C08_AD4C_4144_AA66_FD84C2032E6E__INCLUDED_)

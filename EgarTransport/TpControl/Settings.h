#if !defined(AFX_SETTINGS_H__20318A2A_0ED1_42F2_85B3_CA0F0EE7C69D__INCLUDED_)
#define AFX_SETTINGS_H__20318A2A_0ED1_42F2_85B3_CA0F0EE7C69D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Settings.h : header file
//
#include "HostSettings.h"
#include "RouterSettings.h"
#include "TraceSettings.h"
#include "AboutBox.h"
#include "HostStatistic.h"
#include "EgPropSheet.h"
#include "TpServiceDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CSettings

class CSettings : public CEgPropSheet
{
	DECLARE_DYNAMIC(CSettings)

// Construction
public:
	CSettings(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CSettings(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSettings)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CSettings();

	CHostSettings		m_HostPage;
	CRouterSettings		m_RouterPage;
	CHostStatistic		m_HostStPage;
	CAboutBox			m_AboutPage;
	CTpServiceDlg		m_ServicePage;
	CTraceSettings		m_TracePage;
	bool				m_IsOK;
	bool				m_IsChanged;

	// Generated message map functions
protected:

	void DisableCancel();
	void EnableCancel();
	void AddPages();

	//{{AFX_MSG(CSettings)
	afx_msg void OnApplyNow();
	afx_msg void OnOK();
	afx_msg void OnCancel();
	afx_msg void OnClose();
	afx_msg LRESULT OnRefresh(WPARAM, LPARAM);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETTINGS_H__20318A2A_0ED1_42F2_85B3_CA0F0EE7C69D__INCLUDED_)

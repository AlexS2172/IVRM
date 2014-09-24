#if !defined(AFX_HOSTSETTINGS_H__75DDBA3C_D2E4_46E1_903F_9D0E3A612318__INCLUDED_)
#define AFX_HOSTSETTINGS_H__75DDBA3C_D2E4_46E1_903F_9D0E3A612318__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// HostSettings.h : header file
//
#include "resource.h"

/////////////////////////////////////////////////////////////////////////////
// CHostSettings dialog

class CHostSettings : public CPropertyPage
{
	DECLARE_DYNCREATE(CHostSettings)

// Construction
public:
	CHostSettings();
	~CHostSettings();

// Dialog Data
	//{{AFX_DATA(CHostSettings)
	enum { IDD = IDD_HOST_SETTINGS };
	DWORD	m_dwConnLoseTm;
	DWORD	m_dwConnTimeout;
	DWORD	m_dwTestRequestTm;
	DWORD	m_dwReconnTm;
	CString	m_strRouterHost;
	DWORD	m_dwRouterPort;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CHostSettings)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CHostSettings)
	afx_msg void OnChange();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HOSTSETTINGS_H__75DDBA3C_D2E4_46E1_903F_9D0E3A612318__INCLUDED_)

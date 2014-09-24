#if !defined(AFX_TRACESETTINGS_H__0C38EC5D_CF6F_4390_8444_343446D0AB82__INCLUDED_)
#define AFX_TRACESETTINGS_H__0C38EC5D_CF6F_4390_8444_343446D0AB82__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TraceSettings.h : header file
//
#include "resource.h"

/////////////////////////////////////////////////////////////////////////////
// TraceSettings dialog

class CTraceSettings : public CPropertyPage
{
	DECLARE_DYNCREATE(CTraceSettings)

// Construction
public:
	CTraceSettings();
	~CTraceSettings();

// Dialog Data
	//{{AFX_DATA(CTraceSettings)
	enum { IDD = IDD_TRACESETTINGS };
	CString	m_strLogDir;
	long	m_lLogHistory;
	long	m_lLogLevel;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CTraceSettings)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CTraceSettings)
	afx_msg void OnChange();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TRACESETTINGS_H__0C38EC5D_CF6F_4390_8444_343446D0AB82__INCLUDED_)

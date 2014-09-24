#ifndef __HOST_STATISTIC_H__
#define __HOST_STATISTIC_H__

#include "resource.h"

/////////////////////////////////////////////////////////////////////////////
// CHostStatistic dialog
class CHostStatistic : public CPropertyPage
{

	DECLARE_DYNCREATE(CHostStatistic)
		
// Construction
public:
	CHostStatistic(CWnd* pParent = NULL);   // standard constructor
	
	void Update();

// Dialog Data
	//{{AFX_DATA(CHostStatistic)
	enum { IDD = IDD_HOST_STATISTIC };
	CString	m_sStartLogonTime;
	CString	m_sConfirmedMsg;
	CString	m_sReceivedMsg;
	CString	m_sUnsentMsg;
	CString	m_sSentMsg;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHostStatistic)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CHostStatistic)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	CLock m_Lock;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif //__HOST_STATISTIC_H__

#ifndef __ABOUT_BOX_H__
#define __ABOUT_BOX_H__

#include "resource.h"

/////////////////////////////////////////////////////////////////////////////
// CAboutBox dialog

class CAboutBox : public CDialog
{
// Construction
public:
	CAboutBox(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CAboutBox)
	enum { IDD = IDD_ABOUTBOX };
	CString	m_sVersion;
	CString	m_sVersion2;
	CString	m_sVersion3;
	CString	m_sProxyName;
	CString	m_sServiceName;
	CString	m_sName;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutBox)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void FillObjectVersion(LPTSTR szProgID,CString& strFile, CString& strVer);
	void FillObjectVersion (CLSID clsid,CString& strFile, CString& strVer);

	// Generated message map functions
	//{{AFX_MSG(CAboutBox)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif //__ABOUT_BOX_H__

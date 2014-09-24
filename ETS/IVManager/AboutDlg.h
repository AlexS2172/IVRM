#include "afxwin.h"
#if !defined(AFX_ABOUTDLG_H__51F221CB_D6EB_4532_A446_DB1A03BBFFF2__INCLUDED_)
#define AFX_ABOUTDLG_H__51F221CB_D6EB_4532_A446_DB1A03BBFFF2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AboutDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog

class CAboutDlg : public CDialog
{
public:
	void FillDBVersion();
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	CEdit	m_edProductInfo;
	CString	m_strCaption;
	CString	m_strCopyRight;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void FillObjectVersion(LPCTSTR szProgID);
	void FillObjectVersion (CLSID clsid);
	void FillDllVersion (LPCTSTR szDll);
	void AddInfoStr ( LPCTSTR szStr);
	void FillProductInfo();
	//{{AFX_MSG(CAboutDlg)
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	CStatic m_staticCotyright;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ABOUTDLG_H__51F221CB_D6EB_4532_A446_DB1A03BBFFF2__INCLUDED_)

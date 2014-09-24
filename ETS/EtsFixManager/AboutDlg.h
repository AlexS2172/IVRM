// AboutDlg.h : main header file for the CAboutDlg dialog
//
#pragma once

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg
class CAboutDlg : public CDialog
{
	DECLARE_DYNAMIC(CAboutDlg)

public:
	CAboutDlg(CWnd* pParent = NULL);
	virtual ~CAboutDlg();

	enum DlgResEnum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);

	CString	GetDatabaseDescription();
	CString GetModuleDescription(LPCTSTR szFileName);
	CString GetModuleDescription(CLSID clsid, LPCTSTR szFileName);

protected:
	DECLARE_MESSAGE_MAP()

	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnSysInfoClicked();
public:
	virtual BOOL OnInitDialog();
};




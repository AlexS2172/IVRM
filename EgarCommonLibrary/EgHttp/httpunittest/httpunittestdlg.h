#ifndef __HTTPUNITTESTDLG_H__
#define __HTTPUNITTESTDLG_H__

// HttpUnitTestDlg.h : header file
//

#pragma once
#include "afxwin.h"


// CHttpUnitTestDlg dialog
class CHttpUnitTestDlg : public CDialog
{
// Construction
public:
	CHttpUnitTestDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_HTTPUNITTEST_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonGo();
	CString m_strProxyURL;
	int m_iProxyPort;
	CString m_strProxyLogin;
	CString m_strProxyPassword;
	CButton m_chkProxyAuth;
	CButton m_chkUseProxy;

	CEgHttpConnectionParam m_cpParams;
	CString m_strServerAddress;
	int m_iServerPort;
	afx_msg void OnBnClickedCheckProxyAuth();
	afx_msg void OnBnClickedCheckUseProxy();
	CString m_strServerPath;
	CString m_strServerLogin;
	CString m_strServerPassword;
};

#endif // __HTTPUNITTESTDLG_H__

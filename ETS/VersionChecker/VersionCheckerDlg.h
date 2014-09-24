// VersionCheckerDlg.h : header file
//

#pragma once
#include "afxwin.h"


// CVersionCheckerDlg dialog
class CVersionCheckerDlg : public CDialog
{
// Construction
public:
	CVersionCheckerDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_VERSIONCHECKER_DIALOG };

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
	// // Log Control
	CEdit m_edLog;
	CFont m_edFont;
private:
	// // Add string to log
	void AddLog(const LPCTSTR strStatus, const CString& strLog);
	void _FixVersionString(CString& strVer);
public:
	CString n_strValue;
private:
	void FillInfo(void);
public:
	CString m_strInstallPath;
private:
	CString FillObjectVersion(LPOLESTR lpszProgID);
	CString FillDllVersion(LPCTSTR szDll);
	CString FillFileVersion(LPCTSTR szFile);
	void    LogDatabase();
	void	SplitConnectionString(const CString& str);

public:
	afx_msg void OnBnClickedButtonCopy();
};

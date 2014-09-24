#if !defined(AFX_SETTINGSDLG_H__BBBDB016_2DB6_4674_AAC2_513AE5C0132B__INCLUDED_)
#define AFX_SETTINGSDLG_H__BBBDB016_2DB6_4674_AAC2_513AE5C0132B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SettingsDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSettingsDlg dialog

class CSettingsDlg : public CDialog
{
// Construction
public:
	CSettingsDlg(CWnd* pParent = NULL);

// Dialog Data
	//{{AFX_DATA(CSettingsDlg)
	enum { IDD = IDD_CONNECTIONS_SETTINGS };
	CString	m_DBConnectionString;
	CString	m_Address;
	UINT	m_Port;
	CString	m_ClientLogin;
	CString	m_ServerLogin;
	UINT	m_ReconnectTimeout;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSettingsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	CString m_DBConnectionStringFull;

	CString GetShortStringFromFull(_bstr_t& bsFull );
	// Generated message map functions
	//{{AFX_MSG(CSettingsDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSetup();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
    BOOL    m_bFixConnectionModified;
    BOOL    m_bDBConnectionModified;

protected:
    CString     m_RenegadeDBConnectionString;
	CString	    m_RenegadeAddress;
	UINT	    m_RenegadePort;
	CString	    m_RenegadeClientLogin;
	CString	    m_RenegadeServerLogin;
	UINT	    m_RenegadeReconnectTimeout;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETTINGSDLG_H__BBBDB016_2DB6_4674_AAC2_513AE5C0132B__INCLUDED_)

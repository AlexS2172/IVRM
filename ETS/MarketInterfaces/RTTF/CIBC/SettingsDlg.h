#if !defined(AFX_SETTINGSDLG_H__2648842F_3E50_4D5C_9F56_A41F9667FAD7__INCLUDED_)
#define AFX_SETTINGSDLG_H__2648842F_3E50_4D5C_9F56_A41F9667FAD7__INCLUDED_

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
	CSettingsDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSettingsDlg)
	enum { IDD = IDD_SETTINGS };
	CString	m_strDBFrom;
	CString	m_strDBTo;
	UINT	m_nTradeCount;
	UINT	m_nWaitingTime;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSettingsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
protected:
	BOOL m_bSettingChanged;
	void BtnSetup(CString& strDB, BOOL bUseSP);
	bool SaveSettings();
    bool CheckDBPaths();
// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSettingsDlg)
	afx_msg void OnBtnFromSetup();
	afx_msg void OnBtnToSetup();
	afx_msg void OnChangeEdits();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};



//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETTINGSDLG_H__2648842F_3E50_4D5C_9F56_A41F9667FAD7__INCLUDED_)

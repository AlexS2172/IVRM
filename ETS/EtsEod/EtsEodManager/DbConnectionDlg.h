#pragma once
#include "DbConnectionSettings.h"

// CDbConnectionDlg dialog

class CDbConnectionDlg : public CDialog
{
	DECLARE_DYNAMIC(CDbConnectionDlg)

public:
	CDbConnectionDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDbConnectionDlg();

// Dialog Data
	enum DlgResEnum { IDD = IDD_DBCONNECTION };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnServerDropdown();
	afx_msg void OnDatabaseDropdown();
	afx_msg void OnNtSecurityClicked();

protected:
	virtual void OnOK();
public:
	virtual BOOL OnInitDialog();

public:
	CComboBox					m_cmbServer;
	CComboBox					m_cmbDatabase;

	CDbConnectionSettings		m_dbConnSettings;

private:
	bool						m_bServersLoaded;

private:
	void _LoadServersList();
	void _LoadDatabasesList();
	void _SaveSettings();

	__declspec(property(get = _ControlEnabledGet, put = _ControlEnabledSet)) BOOL _ControlEnabled[];
		BOOL _ControlEnabledSet(int nID)
		{
			return GetDlgItem(nID)->IsWindowEnabled();
		}
		void _ControlEnabledSet(int nID, BOOL bEnable)
		{
			GetDlgItem(nID)->EnableWindow(bEnable);
		}

	__declspec(property(get = _ControlCheckedGet, put = _ControlCheckedSet)) BOOL _ControlChecked[];
		BOOL _ControlCheckedGet(int nID)
		{
			return (GetDlgItem(nID)->SendMessage(BM_GETCHECK, 0, 0) != BST_UNCHECKED);
		}
		void _ControlCheckedSet(int nID, BOOL bChecked)
		{
			GetDlgItem(nID)->SendMessage(BM_SETCHECK, bChecked ? BST_CHECKED : BST_UNCHECKED);
		}

	__declspec(property(get = _ControlTextGet, put = _ControlTextSet)) CString _ControlText[];
		CString _ControlTextGet(int nID)
		{
			CString strRet;
			GetDlgItem(nID)->GetWindowText(strRet);
			return strRet;
		}
		void _ControlTextSet(int nID, CString strText)
		{
			GetDlgItem(nID)->SetWindowText(strText);
		}
};

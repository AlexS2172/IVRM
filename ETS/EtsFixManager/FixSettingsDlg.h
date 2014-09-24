#pragma once
#include "atlcomtime.h"
#include "afxwin.h"


// CFixSettingsDlg dialog

class CFixSettingsDlg : public CDialog
{
	DECLARE_DYNAMIC(CFixSettingsDlg)

public:
	CFixSettingsDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CFixSettingsDlg();

// Dialog Data
	enum DlgResEnum { IDD = IDD_FIXSETTINGS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnRunAtStartupBnClicked();	
	afx_msg void OnAutoStartStopBnClicked();	
	afx_msg void OnCheckGatewaysTimeEnKillFocus();		

private:
	void _LoadData();
	void _SaveData();

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

protected:
	virtual void OnOK();

public:
	COleDateTime	m_dtStartTime;
	COleDateTime	m_dtStopTime;
	bool			m_bAutoStartStop;
protected:
	bool			m_bRunAtStartup;
	long			m_nCheckGatewaysTime;
	bool			m_bIsChanged;
};

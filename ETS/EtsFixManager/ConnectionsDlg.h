#pragma once
#include "afxwin.h"
#include "DbConnectionSettings.h"

// CConnectionsDlg dialog

class CConnectionsDlg : public CDialog
{
	DECLARE_DYNAMIC(CConnectionsDlg)

public:
	CConnectionsDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CConnectionsDlg();

// Dialog Data
	enum DlgResEnum { IDD = IDD_CONNECTIONS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnDbSetupClicked();
	afx_msg void OnFpTypeSelChange();
	afx_msg void OnFpLatencyBnClicked();	
	afx_msg void OnFpResetOnDisconnectYesBnClicked();	
	afx_msg void OnFpResetOnDisconnectNoBnClicked();	
	afx_msg void OnFpResetOnLogoutYesBnClicked();	
	afx_msg void OnFpResetOnLogoutNoBnClicked();	
	afx_msg void OnFpSocketConnHostEnKillFocus();	
	afx_msg void OnFpSocketConnPortEnKillFocus();		
	afx_msg void OnFpSenderIdEnKillFocus();	
	afx_msg void OnFpTargetIdEnKillFocus();					
	afx_msg void OnFpLogonTimeoutEnKillFocus();	
	afx_msg void OnFpReconnectTimeEnKillFocus();					
	afx_msg void OnFpHeartbeatEnKillFocus();					
	afx_msg void OnFpMaxLatencyEnKillFocus();					
	afx_msg void OnFpFilePathEnKillFocus();						
	afx_msg void OnDbFilePathClicked();
	afx_msg void OnFpOrderIDPrefixEnKillFocus();

private:
	void _LoadData();
	void _SaveData();

	void _LoadDbData(EM::ISettingsPtr& spSettings);
	void _SaveDbData(EM::ISettingsPtr& spSettings);

	void _LoadFpData();
	void _LoadFpData(long nProvID, _bstr_t bsKey, _bstr_t bsDescr);
	void _UpdateFpControls();
	void _UpdateFpControlsState();
	void _DisableFpControls();
	void _SaveFpData();

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

public:
	static int _stdcall BrowseCallbackProc(HWND hwnd, 
		UINT uMsg,
		LPARAM lp, 
		LPARAM pData);


public:
	bool		m_bIsDbChanged;
	bool		m_bIsFpChanged;

	CComboBox	m_cmbFpType;

public:
	class CFixProviderData
	{
	public:
		CFixProviderData()
			: m_nID(0L),
			m_nConnPort(0L),
			m_nLogonTimeOut(0L),
			m_nReconnectInt(0L),
			m_nHeartbeatInt(0L),
			m_bCheckLatency(false),
			m_nMaxLatency(0L),
			m_bResetOnDisconnect(false),
			m_bResetOnLogout(false)
		{
		}

		~CFixProviderData()
		{
		}

		long						m_nID;
		CString						m_strName;
		CString						m_strProgID;
		CString						m_strConnHost;
		long						m_nConnPort;
		CString						m_strSendCompID;
		CString						m_strTargCompID;
		long						m_nLogonTimeOut;
		long						m_nReconnectInt;
		long						m_nHeartbeatInt;
		bool						m_bCheckLatency;
		long						m_nMaxLatency;
		bool						m_bResetOnDisconnect;
		bool						m_bResetOnLogout;
		CString						m_strFilePath;
		CString						m_strLastOrderId;
	};

	typedef vector<CFixProviderData>		CFixProviderDataVec;
private:
	CDbConnectionSettings		m_dbConnSettings;
	CFixProviderDataVec			m_vecFpData;

protected:
	virtual void OnOK();
};

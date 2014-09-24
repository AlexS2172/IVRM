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
	//afx_msg void OnVsTypeSelChange();
	afx_msg void OnPpTypeSelChange();
	afx_msg void OnPpProxyNoneBnClicked();
	afx_msg void OnPpProxySystemBnClicked();
	afx_msg void OnPpProxyCustomBnClicked();
	afx_msg void OnPpProxyAuthBnClicked();
	afx_msg void OnIvProxyNoneBnClicked();
	afx_msg void OnIvProxySystemBnClicked();
	afx_msg void OnIvProxyCustomBnClicked();
	afx_msg void OnIvProxyAuthBnClicked();
	afx_msg void OnIvAddressEnKillFocus();
	afx_msg void OnIvAccountEnKillFocus();
	afx_msg void OnIvPasswordEnKillFocus();
	afx_msg void OnIvProxyAddressEnKillFocus();
	afx_msg void OnIvProxyPortEnKillFocus();
	afx_msg void OnIvProxyLoginEnKillFocus();
	afx_msg void OnIvProxyPasswordEnKillFocus();
	afx_msg void OnPpAccountEnKillFocus();
	afx_msg void OnPpPasswordEnKillFocus();
	afx_msg void OnPpProxyAddressEnKillFocus();
	afx_msg void OnPpProxyPortEnKillFocus();
	afx_msg void OnPpProxyLoginEnKillFocus();
	afx_msg void OnPpProxyPasswordEnKillFocus();

private:
	void _LoadData();
	void _SaveData();

	void _LoadDbData(EM::ISettingsPtr& spSettings);
	void _SaveDbData(EM::ISettingsPtr& spSettings);

	void _LoadIvData();
	void _UpdateIvControls();
	void _SaveIvData();

	void _LoadPpData(EM::ISettingsPtr& spSettings);
	void _LoadPpData(long nIdx, PP::IProvidersPtr& spProviders);
	void _UpdatePpControls();
	void _UpdatePpControlsState();
	void _SavePpData(EM::ISettingsPtr& spSettings);

	//void _LoadVsData(EM::ISettingsPtr& spSettings);
	//void _LoadVsData(long nIdx/*, VS::IVolatilitySourcesPtr& spVolaSources*/);
	//void _SaveVsData(EM::ISettingsPtr& spSettings);

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
	//CComboBox	m_cmbVsType;
	CComboBox	m_cmbPpType;
	CEdit		m_edtPpProxyPort;

	bool		m_bIsDbChanged;
	bool		m_bIsPpChanged;
	bool		m_bIsIvChanged;
	bool		m_bIsVsChanged;

public:
	class CPriceProviderData
	{
	public:
		CPriceProviderData()
			: m_nID(0L), m_bUseLogin(FALSE),
			m_enUseProxy(PP::enProxyNone), m_nProxyPort(0L),
			m_enProxyAuthType(PP::enProxyAuthNone)
		{
		}

		~CPriceProviderData()
		{
			m_spProvider = NULL;
		}

		long						m_nID;
		CString						m_strName;
		BOOL						m_bUseLogin;
		CString						m_strProgID;
		CString						m_strAccount;
		CString						m_strPassword;
		PP::ProviderUseProxyType	m_enUseProxy;
		CString						m_strProxyAddress;
		long						m_nProxyPort;
		PP::ProviderProxyAuthType	m_enProxyAuthType;
		CString						m_strProxyLogin;
		CString						m_strProxyPassword;

		PP::IProviderDataPtr		m_spProvider;
	};

	typedef vector<CPriceProviderData>		CPriceProviderDataVec;

	class CIvData
	{
	public:
		CIvData()
			: m_enUseProxy(EM::enProxyDefault), m_nProxyPort(0L),
			m_enProxyAuthType(EM::enProxyAuthDisable)
		{
		}

		CString							m_strAddress;
		CString							m_strAccount;
		CString							m_strPassword;
		EM::UseProxyType				m_enUseProxy;
		CString							m_strProxyAddress;
		long							m_nProxyPort;
		EM::ProxyAuthentificationType	m_enProxyAuthType;
		CString							m_strProxyLogin;
		CString							m_strProxyPassword;
	};

	class CVolaSourceData
	{
	public:
		CVolaSourceData()
			: m_nID(0L)
		{
		}

		long		m_nID;
		CString		m_strName;
		CString		m_strProgID;
	};

	typedef vector<CVolaSourceData>		CVolaSourceDataVec;

private:
	CDbConnectionSettings		m_dbConnSettings;

	long						m_nVolaSourceID;
	CVolaSourceDataVec			m_vecVsData;

	long						m_nPriceProviderID;
	CPriceProviderDataVec		m_vecPpData;

	CIvData						m_ivData;
protected:
	virtual void OnOK();
};

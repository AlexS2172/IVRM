// ConnectionsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "EtsEodManager.h"
#include "ConnectionsDlg.h"
#include "DbConnectionDlg.h"

// CConnectionsDlg dialog

IMPLEMENT_DYNAMIC(CConnectionsDlg, CDialog)
CConnectionsDlg::CConnectionsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CConnectionsDlg::IDD, pParent),
	m_bIsDbChanged(false),
	m_bIsPpChanged(false),
	m_bIsIvChanged(false),
	m_bIsVsChanged(false),
	m_nVolaSourceID(0L),
	m_nPriceProviderID(0L)
{
}

CConnectionsDlg::~CConnectionsDlg()
{
}

void CConnectionsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
//	DDX_Control(pDX, IDC_COMBO_VS_TYPE, m_cmbVsType);
	DDX_Control(pDX, IDC_COMBO_PP_TYPE, m_cmbPpType);
	DDX_Control(pDX, IDC_EDIT_PP_PROXY_PORT, m_edtPpProxyPort);
}

BEGIN_MESSAGE_MAP(CConnectionsDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_DB_SETUP, OnDbSetupClicked)
	//ON_CBN_SELCHANGE(IDC_COMBO_VS_TYPE, OnVsTypeSelChange)
	ON_CBN_SELCHANGE(IDC_COMBO_PP_TYPE, OnPpTypeSelChange)
	ON_BN_CLICKED(IDC_RADIO_PP_PROXY_NONE, OnPpProxyNoneBnClicked)
	ON_BN_CLICKED(IDC_RADIO_PP_PROXY_SYSTEM, OnPpProxySystemBnClicked)
	ON_BN_CLICKED(IDC_RADIO_PP_PROXY_CUSTOM, OnPpProxyCustomBnClicked)
	ON_BN_CLICKED(IDC_CHECK_PP_PROXY_AUTH, OnPpProxyAuthBnClicked)
	ON_BN_CLICKED(IDC_RADIO_IV_PROXY_NONE, OnIvProxyNoneBnClicked)
	ON_BN_CLICKED(IDC_RADIO_IV_PROXY_SYSTEM, OnIvProxySystemBnClicked)
	ON_BN_CLICKED(IDC_RADIO_IV_PROXY_CUSTOM, OnIvProxyCustomBnClicked)
	ON_BN_CLICKED(IDC_CHECK_IV_PROXY_AUTH, OnIvProxyAuthBnClicked)
	ON_EN_KILLFOCUS(IDC_EDIT_IV_ADDRESS, OnIvAddressEnKillFocus)
	ON_EN_KILLFOCUS(IDC_EDIT_IV_ACCOUNT, OnIvAccountEnKillFocus)
	ON_EN_KILLFOCUS(IDC_EDIT_IV_PASSWORD, OnIvPasswordEnKillFocus)
	ON_EN_KILLFOCUS(IDC_EDIT_IV_PROXY_ADDRESS, OnIvProxyAddressEnKillFocus)
	ON_EN_KILLFOCUS(IDC_EDIT_IV_PROXY_PORT, OnIvProxyPortEnKillFocus)
	ON_EN_KILLFOCUS(IDC_EDIT_IV_PROXY_LOGIN, OnIvProxyLoginEnKillFocus)
	ON_EN_KILLFOCUS(IDC_EDIT_IV_PROXY_PASSWORD, OnIvProxyPasswordEnKillFocus)
	ON_EN_KILLFOCUS(IDC_EDIT_PP_ACCOUNT, OnPpAccountEnKillFocus)
	ON_EN_KILLFOCUS(IDC_EDIT_PP_PASSWORD, OnPpPasswordEnKillFocus)
	ON_EN_KILLFOCUS(IDC_EDIT_PP_PROXY_ADDRESS, OnPpProxyAddressEnKillFocus)
	ON_EN_KILLFOCUS(IDC_EDIT_PP_PROXY_PORT, OnPpProxyPortEnKillFocus)
	ON_EN_KILLFOCUS(IDC_EDIT_PP_PROXY_LOGIN, OnPpProxyLoginEnKillFocus)
	ON_EN_KILLFOCUS(IDC_EDIT_PP_PROXY_PASSWORD, OnPpProxyPasswordEnKillFocus)
END_MESSAGE_MAP()


// CConnectionsDlg message handlers

BOOL CConnectionsDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	_LoadData();
	return TRUE;
}

void CConnectionsDlg::_LoadData()
{
	CWaitCursor wait;

	EM::ISettingsPtr spSettings;
	EM::ISettings2Ptr spSettings2;
	try
	{
		__CHECK_HRESULT(spSettings.CreateInstance(EM::CLSID_Settings), _T("Fail to create ETS settings object."));
		try
		{
			__CHECK_HRESULT(spSettings2.CreateInstance(EM::CLSID_Settings), _T("Can't get ISettings2 Interface"));
			spSettings2->InitForProfile("EOD");
		}
		catch(_com_error& e)
		{
			TRACE2(_T("%s: %s\n"), __FUNCTION__, (LPCTSTR)e.Description() ? (LPCTSTR)e.Description() : e.ErrorMessage());
		}

	}
	catch(_com_error& e)
	{
		CString strError;
		strError.Format(_T("Settings loading failed: %s (%X)"), (LPCTSTR)e.Description() ? (LPCTSTR)e.Description() : e.ErrorMessage(), e.Error());
		MessageBox(strError, NULL, MB_ICONSTOP);
	}

	_LoadDbData(spSettings);
	//_LoadVsData(spSettings);
	_LoadPpData(spSettings);
	_LoadIvData();
}

void CConnectionsDlg::_SaveData()
{
	CWaitCursor wait;

	EM::ISettingsPtr spSettings;
	EM::ISettings2Ptr spSettings2;
	try
	{
		__CHECK_HRESULT(spSettings.CreateInstance(EM::CLSID_Settings), _T("Fail to create ETS settings object."));
	}
	catch(_com_error& e)
	{
		CString strError;
		strError.Format(_T("Settings saving failed: %s (%X)"), (LPCTSTR)e.Description() ? (LPCTSTR)e.Description() : e.ErrorMessage(), e.Error());
		MessageBox(strError, NULL, MB_ICONSTOP);
	}



	_SaveDbData(spSettings);
	//_SaveVsData(spSettings);
	_SavePpData(spSettings);
	_SaveIvData();
}

void CConnectionsDlg::_LoadDbData(EM::ISettingsPtr& spSettings)
{
		try
		{
			m_dbConnSettings.m_strConnectionString = (LPCTSTR)spSettings->DatabaseConnection;		
			m_dbConnSettings.ParseConnectionString();
		}
		catch(_com_error& e)
		{
			CString strError;
			strError.Format(_T("Database settings loading failed: %s (%X)"),
				(LPCTSTR)e.Description() ? (LPCTSTR)e.Description() : e.ErrorMessage(), e.Error());
			MessageBox(strError, NULL, MB_ICONSTOP);
		}
		catch(...)
		 {
			MessageBox("Database settings loading failed", "Unhandled Exception Catched", MB_ICONSTOP);
		 }

	_ControlText[IDC_EDIT_DB] = m_dbConnSettings.GetDbPath();
}

void CConnectionsDlg::_SaveDbData(EM::ISettingsPtr& spSettings)
{
	if(spSettings != NULL)
	{
		try
		{
			m_dbConnSettings.MakeConnectionString();
			spSettings->DatabaseConnection = _bstr_t(m_dbConnSettings.m_strConnectionString);
			try
			{
				EM::ISettings2Ptr spSettings2;
				__CHECK_HRESULT(spSettings2.CreateInstance(EM::CLSID_Settings), _T("Can't get ISettings2 Interface"));
				spSettings2->UpdateForProfile("EOD");
			}
			catch(_com_error& e)
			{
				TRACE2(_T("%s: %s\n"), __FUNCTION__, (LPCTSTR)e.Description() ? (LPCTSTR)e.Description() : e.ErrorMessage());
			}


		}
		catch(_com_error& e)
		{
			CString strError;
			strError.Format(_T("Database settings saving failed: %s (%X)"),
				(LPCTSTR)e.Description() ? (LPCTSTR)e.Description() : e.ErrorMessage(), e.Error());
			MessageBox(strError, NULL, MB_ICONSTOP);
		}
	}
}

void CConnectionsDlg::_LoadIvData()
{
	try
	{
		EM::IIVSettingsPtr spIvSettings;
		__CHECK_HRESULT(spIvSettings.CreateInstance(EM::CLSID_IVSettings),
						_T("Fail to create IVolatility.com settings object."));

		m_ivData.m_strAddress = (LPCTSTR)spIvSettings->VolServerName;
		m_ivData.m_strAccount = (LPCTSTR)spIvSettings->VolLogin;
		m_ivData.m_strPassword = (LPCTSTR)spIvSettings->VolPassword;
		m_ivData.m_enUseProxy = spIvSettings->UseProxy;
		m_ivData.m_strProxyAddress = (LPCTSTR)spIvSettings->ProxyName;
		m_ivData.m_nProxyPort = spIvSettings->ProxyPort;
		m_ivData.m_enProxyAuthType = spIvSettings->ProxyAuthType;
		m_ivData.m_strProxyLogin = (LPCTSTR)spIvSettings->ProxyLogin;
		m_ivData.m_strProxyPassword = (LPCTSTR)spIvSettings->ProxyPassword;
	}
	catch(_com_error& e)
	{
		CString strError;
		strError.Format(_T("IVolatility.com settings loading failed: %s (%X)"),
			(LPCTSTR)e.Description() ? (LPCTSTR)e.Description() : e.ErrorMessage(), e.Error());
		MessageBox(strError, NULL, MB_ICONSTOP);
	}

	_ControlText[IDC_EDIT_IV_ADDRESS] = m_ivData.m_strAddress;
	_ControlText[IDC_EDIT_IV_ACCOUNT] = m_ivData.m_strAccount;
	_ControlText[IDC_EDIT_IV_PASSWORD] = m_ivData.m_strPassword;

	_ControlChecked[IDC_RADIO_IV_PROXY_NONE] = (m_ivData.m_enUseProxy == EM::enProxyDisable);
	_ControlChecked[IDC_RADIO_IV_PROXY_SYSTEM] = (m_ivData.m_enUseProxy == EM::enProxyDefault);
	_ControlChecked[IDC_RADIO_IV_PROXY_CUSTOM] = (m_ivData.m_enUseProxy == EM::enProxyCustom);

	_ControlText[IDC_EDIT_IV_PROXY_ADDRESS] = m_ivData.m_strProxyAddress;
	CString strTemp;
	strTemp.Format(_T("%d"), m_ivData.m_nProxyPort);
	_ControlText[IDC_EDIT_IV_PROXY_PORT] = strTemp;

	_ControlChecked[IDC_CHECK_IV_PROXY_AUTH] = (m_ivData.m_enProxyAuthType == EM::enProxyAuthEnable);
	_ControlText[IDC_EDIT_IV_PROXY_LOGIN] = m_ivData.m_strProxyLogin;
	_ControlText[IDC_EDIT_IV_PROXY_PASSWORD] = m_ivData.m_strProxyPassword;

	_UpdateIvControls();
}

void CConnectionsDlg::_SaveIvData()
{
	try
	{
		EM::IIVSettingsPtr spIvSettings;
		__CHECK_HRESULT(spIvSettings.CreateInstance(EM::CLSID_IVSettings),
						_T("Fail to create IVolatility.com settings object."));

		spIvSettings->VolServerName = _bstr_t(m_ivData.m_strAddress);
		spIvSettings->VolLogin = _bstr_t(m_ivData.m_strAccount);
		spIvSettings->VolPassword = _bstr_t(m_ivData.m_strPassword);
		spIvSettings->UseProxy = m_ivData.m_enUseProxy;
		spIvSettings->ProxyName = _bstr_t(m_ivData.m_strProxyAddress);
		spIvSettings->ProxyPort = m_ivData.m_nProxyPort;
		spIvSettings->ProxyAuthType = m_ivData.m_enProxyAuthType;
		spIvSettings->ProxyLogin = _bstr_t(m_ivData.m_strProxyLogin);
		spIvSettings->ProxyPassword = _bstr_t(m_ivData.m_strProxyPassword);
	}
	catch(_com_error& e)
	{
		CString strError;
		strError.Format(_T("IVolatility.com settings saving failed: %s (%X)"),
			(LPCTSTR)e.Description() ? (LPCTSTR)e.Description() : e.ErrorMessage(), e.Error());
		MessageBox(strError, NULL, MB_ICONSTOP);
	}
}

void CConnectionsDlg::_UpdateIvControls()
{
	BOOL bEnabled = (m_ivData.m_enUseProxy == EM::enProxyCustom);

	_ControlEnabled[IDC_STATIC_IV_PROXY_ADDRESS] = bEnabled;
	_ControlEnabled[IDC_EDIT_IV_PROXY_ADDRESS] = bEnabled;

	_ControlEnabled[IDC_STATIC_IV_PROXY_PORT] = bEnabled;
	_ControlEnabled[IDC_EDIT_IV_PROXY_PORT] = bEnabled;

	_ControlEnabled[IDC_CHECK_IV_PROXY_AUTH] = bEnabled;

	bEnabled = bEnabled && (m_ivData.m_enProxyAuthType == EM::enProxyAuthEnable);

	_ControlEnabled[IDC_STATIC_IV_PROXY_LOGIN] = bEnabled;
	_ControlEnabled[IDC_EDIT_IV_PROXY_LOGIN] = bEnabled;

	_ControlEnabled[IDC_STATIC_IV_PROXY_PASSWORD] = bEnabled;
	_ControlEnabled[IDC_EDIT_IV_PROXY_PASSWORD] = bEnabled;
}

void CConnectionsDlg::_LoadPpData(EM::ISettingsPtr& spSettings)
{
	m_cmbPpType.ResetContent();
	m_vecPpData.clear();

	if(spSettings != NULL)
	{
		try
		{
			m_nPriceProviderID = spSettings->PriceProvider;

			PP::IProvidersPtr spProviders;
			__CHECK_HRESULT(spProviders.CreateInstance(PP::CLSID_Providers),
							_T("Fail to create price providers object."));
			
			spProviders->Initialize();
			long nCount = spProviders->Count;

			for(long i = 0; i < nCount; i++)
				_LoadPpData(i, spProviders);
		}
		catch(_com_error& e)
		{
			CString strError;
			strError.Format(_T("Price provider settings loading failed: %s (%X)"),
				(LPCTSTR)e.Description() ? (LPCTSTR)e.Description() : e.ErrorMessage(), e.Error());
			MessageBox(strError, NULL, MB_ICONSTOP);
		}
	}

	_UpdatePpControls();
}

void CConnectionsDlg::_LoadPpData(long nIdx, PP::IProvidersPtr& spProviders)
{
	try
	{
		int nListIdx;

		CPriceProviderData ppd;
		ppd.m_spProvider = spProviders->Item[nIdx + 1];
		ppd.m_nID = ppd.m_spProvider->ProviderID;
		ppd.m_strName = (LPCTSTR)ppd.m_spProvider->Description;
		ppd.m_bUseLogin = (ppd.m_spProvider->NeedLogin != VARIANT_FALSE);
		ppd.m_strProgID = (LPCTSTR)ppd.m_spProvider->ProgId[PP::enPriceInfo];
		
		if(ppd.m_bUseLogin)
		{
			ppd.m_strAccount = (LPCTSTR)ppd.m_spProvider->UserAccount;
			ppd.m_strPassword = (LPCTSTR)ppd.m_spProvider->UserPassword;
			ppd.m_enUseProxy = ppd.m_spProvider->UseProxy;

			if(ppd.m_enUseProxy != PP::enProxyNone)
			{
				ppd.m_strProxyAddress = (LPCTSTR)ppd.m_spProvider->ProxyName;
				ppd.m_nProxyPort = ppd.m_spProvider->ProxyPort;
				ppd.m_enProxyAuthType = ppd.m_spProvider->ProxyAuthorization;

				if(ppd.m_enProxyAuthType != PP::enProxyAuthNone)
				{
					ppd.m_strProxyLogin = (LPCTSTR)ppd.m_spProvider->ProxyLogin;
					ppd.m_strProxyPassword = (LPCTSTR)ppd.m_spProvider->ProxyPassword;
				}
			}
		}

		m_vecPpData.push_back(ppd);
		nIdx = (long)m_vecPpData.size() - 1L;

		nListIdx = m_cmbPpType.AddString(ppd.m_strName);
		m_cmbPpType.SetItemData(nListIdx, nIdx);

		if(m_nPriceProviderID == ppd.m_nID)
			m_cmbPpType.SetCurSel(nListIdx);
	}
	catch(_com_error& e)
	{
		CString strError;
		strError.Format(_T("Price provider settings loading failed: %s (%X)"),
			(LPCTSTR)e.Description() ? (LPCTSTR)e.Description() : e.ErrorMessage(), e.Error());
		TRACE2(_T("%s: %s"), __FUNCTION__, strError);
	}
}

void CConnectionsDlg::_SavePpData(EM::ISettingsPtr& spSettings)
{
	if(spSettings != NULL)
	{
		try
		{
			spSettings->PriceProvider = m_nPriceProviderID;

			size_t nPpIdx = m_cmbPpType.GetItemData(m_cmbPpType.GetCurSel());
			if(nPpIdx >= 0L && nPpIdx < m_vecPpData.size())
			{
				const CPriceProviderData& ppd = m_vecPpData[nPpIdx];
				if(ppd.m_bUseLogin)
				{
					ppd.m_spProvider->UserAccount = _bstr_t(ppd.m_strAccount);
					ppd.m_spProvider->UserPassword = _bstr_t(ppd.m_strPassword);

					if(ppd.m_enUseProxy != PP::enProxyNone)
					{
						ppd.m_spProvider->UseProxy = ppd.m_enUseProxy;
						ppd.m_spProvider->ProxyName = _bstr_t(ppd.m_strProxyAddress);
						ppd.m_spProvider->ProxyPort = ppd.m_nProxyPort;

						if(ppd.m_enProxyAuthType != PP::enProxyAuthNone)
						{
							ppd.m_spProvider->ProxyAuthorization = ppd.m_enProxyAuthType;
							ppd.m_spProvider->ProxyLogin = _bstr_t(ppd.m_strProxyLogin);
							ppd.m_spProvider->ProxyPassword = _bstr_t(ppd.m_strProxyPassword);
						}
					}
				}
			}
		}
		catch(_com_error& e)
		{
			CString strError;
			strError.Format(_T("Price provider settings saving failed: %s (%X)"),
				(LPCTSTR)e.Description() ? (LPCTSTR)e.Description() : e.ErrorMessage(), e.Error());
			MessageBox(strError, NULL, MB_ICONSTOP);
		}
	}
}

void CConnectionsDlg::_UpdatePpControls()
{
	size_t nPpIdx = m_cmbPpType.GetItemData(m_cmbPpType.GetCurSel());
	if(nPpIdx >= 0L && nPpIdx < m_vecPpData.size())
	{
		const CPriceProviderData& ppd = m_vecPpData[nPpIdx];
		if(ppd.m_bUseLogin)
		{
			_ControlText[IDC_EDIT_PP_ACCOUNT] = ppd.m_strAccount;
			_ControlText[IDC_EDIT_PP_PASSWORD] = ppd.m_strPassword;
			if(ppd.m_enUseProxy != PP::enProxyNone)
			{
				_ControlChecked[IDC_RADIO_PP_PROXY_NONE] = (ppd.m_enUseProxy == PP::enProxyDisable);
				_ControlChecked[IDC_RADIO_PP_PROXY_SYSTEM] = (ppd.m_enUseProxy == PP::enProxyDefault);
				_ControlChecked[IDC_RADIO_PP_PROXY_CUSTOM] = (ppd.m_enUseProxy == PP::enProxyCustom);

				_ControlText[IDC_EDIT_PP_PROXY_ADDRESS] = ppd.m_strProxyAddress;
				CString strTemp;
				strTemp.Format(_T("%d"), ppd.m_nProxyPort);
				_ControlText[IDC_EDIT_PP_PROXY_PORT] = strTemp;

				if(ppd.m_enProxyAuthType != PP::enProxyAuthNone)
				{
					_ControlChecked[IDC_CHECK_PP_PROXY_AUTH] = (ppd.m_enProxyAuthType == PP::enProxyAuthEnable);
					_ControlText[IDC_EDIT_PP_PROXY_PASSWORD] = ppd.m_strProxyLogin;
					_ControlText[IDC_EDIT_PP_PROXY_LOGIN] = ppd.m_strProxyPassword;
				}
				else
				{
					_ControlChecked[IDC_CHECK_PP_PROXY_AUTH] = FALSE;
					_ControlText[IDC_EDIT_PP_PROXY_PASSWORD] = _T("");
					_ControlText[IDC_EDIT_PP_PROXY_LOGIN] = _T("");
				}
			}
			else
			{
				_ControlChecked[IDC_RADIO_PP_PROXY_NONE] = FALSE;
				_ControlChecked[IDC_RADIO_PP_PROXY_SYSTEM] = FALSE;
				_ControlChecked[IDC_RADIO_PP_PROXY_CUSTOM] = FALSE;

				_ControlText[IDC_EDIT_PP_PROXY_ADDRESS] = _T("");
				_ControlText[IDC_EDIT_PP_PROXY_PORT] = _T("");

				_ControlChecked[IDC_CHECK_PP_PROXY_AUTH] = FALSE;
				_ControlText[IDC_EDIT_PP_PROXY_PASSWORD] = _T("");
				_ControlText[IDC_EDIT_PP_PROXY_LOGIN] = _T("");
			}
		}
		else
		{
			_ControlText[IDC_EDIT_PP_ACCOUNT] = _T("");
			_ControlText[IDC_EDIT_PP_PASSWORD] = _T("");

			_ControlChecked[IDC_RADIO_PP_PROXY_NONE] = FALSE;
			_ControlChecked[IDC_RADIO_PP_PROXY_SYSTEM] = FALSE;
			_ControlChecked[IDC_RADIO_PP_PROXY_CUSTOM] = FALSE;

			_ControlText[IDC_EDIT_PP_PROXY_ADDRESS] = _T("");
			_ControlText[IDC_EDIT_PP_PROXY_PORT] = _T("");

			_ControlChecked[IDC_CHECK_PP_PROXY_AUTH] = FALSE;
			_ControlText[IDC_EDIT_PP_PROXY_PASSWORD] = _T("");
			_ControlText[IDC_EDIT_PP_PROXY_LOGIN] = _T("");
		}
	}

	_UpdatePpControlsState();
}

void CConnectionsDlg::_UpdatePpControlsState()
{
	size_t nPpIdx = m_cmbPpType.GetItemData(m_cmbPpType.GetCurSel());
	if(nPpIdx >= 0L && nPpIdx < m_vecPpData.size())
	{
		const CPriceProviderData& ppd = m_vecPpData[nPpIdx];
		BOOL bEnabled = ppd.m_bUseLogin;

		_ControlEnabled[IDC_STATIC_PP_ACCOUNT] = bEnabled;
		_ControlEnabled[IDC_EDIT_PP_ACCOUNT] = bEnabled;

		_ControlEnabled[IDC_STATIC_PP_PASSWORD] = bEnabled;
		_ControlEnabled[IDC_EDIT_PP_PASSWORD] = bEnabled;

		bEnabled = bEnabled && (ppd.m_enUseProxy != PP::enProxyNone);

		_ControlEnabled[IDC_STATIC_PP_PROXY] = bEnabled;
		_ControlEnabled[IDC_RADIO_PP_PROXY_NONE] = bEnabled;
		_ControlEnabled[IDC_RADIO_PP_PROXY_SYSTEM] = bEnabled;
		_ControlEnabled[IDC_RADIO_PP_PROXY_CUSTOM] = bEnabled;

		bEnabled = bEnabled && (ppd.m_enUseProxy == PP::enProxyCustom);

		_ControlEnabled[IDC_STATIC_PP_PROXY_ADDRESS] = bEnabled;
		_ControlEnabled[IDC_EDIT_PP_PROXY_ADDRESS] = bEnabled;

		_ControlEnabled[IDC_STATIC_PP_PROXY_PORT] = bEnabled;
		_ControlEnabled[IDC_EDIT_PP_PROXY_PORT] = bEnabled;

		bEnabled = bEnabled && (ppd.m_enProxyAuthType != PP::enProxyAuthNone);

		_ControlEnabled[IDC_CHECK_PP_PROXY_AUTH] = bEnabled;

		bEnabled = bEnabled && (ppd.m_enProxyAuthType == PP::enProxyAuthEnable);

		_ControlEnabled[IDC_STATIC_PP_PROXY_LOGIN] = bEnabled;
		_ControlEnabled[IDC_EDIT_PP_PROXY_LOGIN] = bEnabled;

		_ControlEnabled[IDC_STATIC_PP_PROXY_PASSWORD] = bEnabled;
		_ControlEnabled[IDC_EDIT_PP_PROXY_PASSWORD] = bEnabled;
	}
}

//void CConnectionsDlg::_LoadVsData(EM::ISettingsPtr& spSettings)
//{
//	m_vecVsData.clear();
//	m_cmbVsType.ResetContent();
//
//	if(spSettings != NULL)
//	{
//		try
//		{
//			m_nVolaSourceID = spSettings->VolatilitySource;
//		}
//		catch(_com_error& e)
//		{
//			CString strError;
//			strError.Format(_T("Volatility sources settings loading failed: %s (%X)"),
//				(LPCTSTR)e.Description() ? (LPCTSTR)e.Description() : e.ErrorMessage(), e.Error());
//			MessageBox(strError, NULL, MB_ICONSTOP);
//		}
//	}
//
//	/*try
//	{
//		VS::IVolatilitySourcesPtr spVolaSources;
//		__CHECK_HRESULT(spVolaSources.CreateInstance(VS::CLSID_VolatilitySources),
//						_T("Fail to create volatility sources object."));
//
//		long nCount = spVolaSources->Count;
//
//		for(long i = 0; i < nCount; i++)
//			_LoadVsData(i, spVolaSources);
//	}
//	catch(_com_error& e)
//	{
//		CString strError;
//		strError.Format(_T("Volatility sources settings loading failed: %s (%X)"),
//			(LPCTSTR)e.Description() ? (LPCTSTR)e.Description() : e.ErrorMessage(), e.Error());
//		MessageBox(strError, NULL, MB_ICONSTOP);
//	}
//	*/
//}
//
//void CConnectionsDlg::_SaveVsData(EM::ISettingsPtr& spSettings)
//{
//	if(spSettings != NULL)
//	{
//		try
//		{
//			spSettings->VolatilitySource = m_nVolaSourceID;
//		}
//		catch(_com_error& e)
//		{
//			CString strError;
//			strError.Format(_T("Volatility sources settings saving failed: %s (%X)"),
//				(LPCTSTR)e.Description() ? (LPCTSTR)e.Description() : e.ErrorMessage(), e.Error());
//			MessageBox(strError, NULL, MB_ICONSTOP);
//		}
//	}
//}
//
//void CConnectionsDlg::_LoadVsData(long nIdx/*, VS::IVolatilitySourcesPtr& spVolaSources*/)
//{
//	/*try
//	{
//		int nListIdx;
//		long nID;
//		_bstr_t bstrTitle, bstrFileName, bstrVersion, bstrProgID;
//
//		spVolaSources->GetVolatilitySourceInfo(nIdx + 1L, &nID,
//											bstrTitle.GetAddress(), bstrFileName.GetAddress(),
//											bstrVersion.GetAddress(), bstrProgID.GetAddress());
//		CVolaSourceData vsd;
//		vsd.m_nID = nID;
//		vsd.m_strName = (LPCTSTR)bstrTitle;
//		vsd.m_strProgID = (LPCTSTR)bstrProgID;
//
//		m_vecVsData.push_back(vsd);
//		nIdx = (long)m_vecVsData.size() - 1L;
//		nListIdx = m_cmbVsType.AddString(vsd.m_strName);
//		m_cmbVsType.SetItemData(nListIdx, nIdx);
//
//		if(m_nVolaSourceID == vsd.m_nID)
//			m_cmbVsType.SetCurSel(nListIdx);
//	}
//	catch(_com_error& e)
//	{
//		CString strError;
//		strError.Format(_T("Volatility source settings loading failed: %s (%X)"),
//			(LPCTSTR)e.Description() ? (LPCTSTR)e.Description() : e.ErrorMessage(), e.Error());
//		TRACE2(_T("%s: %s"), __FUNCTION__, strError);
//	}
//	*/
//}

void CConnectionsDlg::OnDbSetupClicked()
{
	CDbConnectionDlg dlg;
	dlg.m_dbConnSettings = m_dbConnSettings;
	if(dlg.DoModal() == IDOK)
	{
		if(m_dbConnSettings != dlg.m_dbConnSettings)
		{
			m_bIsDbChanged = true;
			m_dbConnSettings = dlg.m_dbConnSettings;
			_ControlText[IDC_EDIT_DB] = m_dbConnSettings.GetDbPath();
		}
	}
}
/*
void CConnectionsDlg::OnVsTypeSelChange()
{
	size_t nVsIdx = m_cmbVsType.GetItemData(m_cmbVsType.GetCurSel());
	if(nVsIdx >= 0L && nVsIdx < m_vecVsData.size())
	{
		long nID = m_vecVsData[nVsIdx].m_nID;
		if(m_nVolaSourceID != nID)
		{
			m_nVolaSourceID = nID;
			m_bIsVsChanged = true;
		}
	}
}
*/
void CConnectionsDlg::OnPpTypeSelChange()
{
	size_t nPpIdx = m_cmbPpType.GetItemData(m_cmbPpType.GetCurSel());
	if(nPpIdx >= 0L && nPpIdx < m_vecPpData.size())
	{
		long nID = m_vecPpData[nPpIdx].m_nID;
		if(m_nPriceProviderID != nID)
		{
			m_nPriceProviderID = nID;
			m_bIsPpChanged = true;
		}
	}
	_UpdatePpControls();
}

void CConnectionsDlg::OnPpProxyNoneBnClicked()
{
	size_t nPpIdx = m_cmbPpType.GetItemData(m_cmbPpType.GetCurSel());
	if(nPpIdx >= 0L && nPpIdx < m_vecPpData.size())
	{
		m_vecPpData[nPpIdx].m_enUseProxy = PP::enProxyDisable;
		m_bIsPpChanged = true;
	}
	_UpdatePpControlsState();
}

void CConnectionsDlg::OnPpProxySystemBnClicked()
{
	size_t nPpIdx = m_cmbPpType.GetItemData(m_cmbPpType.GetCurSel());
	if(nPpIdx >= 0L && nPpIdx < m_vecPpData.size())
	{
		m_vecPpData[nPpIdx].m_enUseProxy = PP::enProxyDefault;
		m_bIsPpChanged = true;
	}
	_UpdatePpControlsState();
}

void CConnectionsDlg::OnPpProxyCustomBnClicked()
{
	size_t nPpIdx = m_cmbPpType.GetItemData(m_cmbPpType.GetCurSel());
	if(nPpIdx >= 0L && nPpIdx < m_vecPpData.size())
	{
		m_vecPpData[nPpIdx].m_enUseProxy = PP::enProxyCustom;
		m_bIsPpChanged = true;
	}
	_UpdatePpControlsState();
}

void CConnectionsDlg::OnPpProxyAuthBnClicked()
{
	size_t nPpIdx = m_cmbPpType.GetItemData(m_cmbPpType.GetCurSel());
	if(nPpIdx >= 0L && nPpIdx < m_vecPpData.size())
	{
		m_vecPpData[nPpIdx].m_enProxyAuthType = (_ControlChecked[IDC_CHECK_PP_PROXY_AUTH]
												? PP::enProxyAuthEnable :  PP::enProxyAuthDisable);
		m_bIsPpChanged = true;
	}
	_UpdatePpControlsState();
}

void CConnectionsDlg::OnPpAccountEnKillFocus()
{
	size_t nPpIdx = m_cmbPpType.GetItemData(m_cmbPpType.GetCurSel());
	if(nPpIdx >= 0L && nPpIdx < m_vecPpData.size())
	{
		m_vecPpData[nPpIdx].m_strAccount = _ControlText[IDC_EDIT_PP_ACCOUNT];
		m_bIsPpChanged = true;
	}
}

void CConnectionsDlg::OnPpPasswordEnKillFocus()
{
	size_t nPpIdx = m_cmbPpType.GetItemData(m_cmbPpType.GetCurSel());
	if(nPpIdx >= 0L && nPpIdx < m_vecPpData.size())
	{
		m_vecPpData[nPpIdx].m_strPassword = _ControlText[IDC_EDIT_PP_PASSWORD];
		m_bIsPpChanged = true;
	}
}

void CConnectionsDlg::OnPpProxyAddressEnKillFocus()
{
	size_t nPpIdx = m_cmbPpType.GetItemData(m_cmbPpType.GetCurSel());
	if(nPpIdx >= 0L && nPpIdx < m_vecPpData.size())
	{
		m_vecPpData[nPpIdx].m_strProxyAddress = _ControlText[IDC_EDIT_PP_PROXY_ADDRESS];
		m_bIsPpChanged = true;
	}
}

void CConnectionsDlg::OnPpProxyPortEnKillFocus()
{
	size_t nPpIdx = m_cmbPpType.GetItemData(m_cmbPpType.GetCurSel());
	if(nPpIdx >= 0L && nPpIdx < m_vecPpData.size())
	{
		m_vecPpData[nPpIdx].m_nProxyPort = _ttol(_ControlText[IDC_EDIT_PP_PROXY_PORT].Trim());
		m_bIsPpChanged = true;
	}
}

void CConnectionsDlg::OnPpProxyLoginEnKillFocus()
{
	size_t nPpIdx = m_cmbPpType.GetItemData(m_cmbPpType.GetCurSel());
	if(nPpIdx >= 0L && nPpIdx < m_vecPpData.size())
	{
		m_vecPpData[nPpIdx].m_strProxyLogin = _ControlText[IDC_EDIT_PP_PROXY_LOGIN];
		m_bIsPpChanged = true;
	}
}

void CConnectionsDlg::OnPpProxyPasswordEnKillFocus()
{
	size_t nPpIdx = m_cmbPpType.GetItemData(m_cmbPpType.GetCurSel());
	if(nPpIdx >= 0L && nPpIdx < m_vecPpData.size())
	{
		m_vecPpData[nPpIdx].m_strProxyPassword = _ControlText[IDC_EDIT_PP_PROXY_PASSWORD];
		m_bIsPpChanged = true;
	}
}

void CConnectionsDlg::OnIvProxyNoneBnClicked()
{
	m_ivData.m_enUseProxy = EM::enProxyDisable;
	m_bIsIvChanged = true;
	_UpdateIvControls();
}

void CConnectionsDlg::OnIvProxySystemBnClicked()
{
	m_ivData.m_enUseProxy = EM::enProxyDefault;
	m_bIsIvChanged = true;
	_UpdateIvControls();
}

void CConnectionsDlg::OnIvProxyCustomBnClicked()
{
	m_ivData.m_enUseProxy = EM::enProxyCustom;
	m_bIsIvChanged = true;
	_UpdateIvControls();
}

void CConnectionsDlg::OnIvProxyAuthBnClicked()
{
	m_ivData.m_enProxyAuthType = (_ControlChecked[IDC_CHECK_IV_PROXY_AUTH] 
										? EM::enProxyAuthEnable : EM::enProxyAuthDisable);
	m_bIsIvChanged = true;
	_UpdateIvControls();
}

void CConnectionsDlg::OnIvAddressEnKillFocus()
{
	m_ivData.m_strAddress = _ControlText[IDC_EDIT_IV_ADDRESS];
	m_bIsIvChanged = true;
}

void CConnectionsDlg::OnIvAccountEnKillFocus()
{
	m_ivData.m_strAccount = _ControlText[IDC_EDIT_IV_ACCOUNT];
	m_bIsIvChanged = true;
}

void CConnectionsDlg::OnIvPasswordEnKillFocus()
{
	m_ivData.m_strPassword = _ControlText[IDC_EDIT_IV_PASSWORD];
	m_bIsIvChanged = true;
}

void CConnectionsDlg::OnIvProxyAddressEnKillFocus()
{
	m_ivData.m_strProxyAddress = _ControlText[IDC_EDIT_IV_PROXY_ADDRESS];
	m_bIsIvChanged = true;
}

void CConnectionsDlg::OnIvProxyPortEnKillFocus()
{
	m_ivData.m_nProxyPort = _ttol(_ControlText[IDC_EDIT_IV_PROXY_PORT].Trim());
	m_bIsIvChanged = true;
}

void CConnectionsDlg::OnIvProxyLoginEnKillFocus()
{
	m_ivData.m_strProxyLogin = _ControlText[IDC_EDIT_IV_PROXY_LOGIN];
	m_bIsIvChanged = true;
}

void CConnectionsDlg::OnIvProxyPasswordEnKillFocus()
{
	m_ivData.m_strProxyPassword = _ControlText[IDC_EDIT_IV_PROXY_PASSWORD];
	m_bIsIvChanged = true;
}

void CConnectionsDlg::OnOK()
{
	if(m_bIsDbChanged || m_bIsPpChanged || m_bIsIvChanged || m_bIsVsChanged)
		_SaveData();

	CDialog::OnOK();
}

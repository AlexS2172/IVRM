// ConnectionsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "EtsFixManager.h"
#include "ConnectionsDlg.h"
#include "DbConnectionDlg.h"
#include "egregreader.h"

static const _bstr_t    SETTINGS_MAIN_XML_KEY(L"ETS\\Asp\\Groups\\");
static const _bstr_t    SETTINGS_USER_XML_KEY(L"ETS\\OrdersGateway\\");
static const _bstr_t    SETTINGS_XML_ORDID(L"LastOrderID");

// CConnectionsDlg dialog

IMPLEMENT_DYNAMIC(CConnectionsDlg, CDialog)
CConnectionsDlg::CConnectionsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CConnectionsDlg::IDD, pParent),
	m_bIsDbChanged(false),
	m_bIsFpChanged(false)
{
}

CConnectionsDlg::~CConnectionsDlg()
{
}

void CConnectionsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_FP_TYPE, m_cmbFpType);
}

BEGIN_MESSAGE_MAP(CConnectionsDlg, CDialog)
	//ON_BN_CLICKED(IDC_BUTTON_DB_SETUP, OnDbSetupClicked)
	//ON_BN_CLICKED(IDC_BUTTON_FP_FILEPATH, OnDbFilePathClicked)	
	ON_CBN_SELCHANGE(IDC_COMBO_FP_TYPE, OnFpTypeSelChange)
	//ON_BN_CLICKED(IDC_CHECK_FP_LATENCY, OnFpLatencyBnClicked)
	//ON_BN_CLICKED(IDC_RADIO_FP_RESETONDIS_YES, OnFpResetOnDisconnectYesBnClicked)
	//ON_BN_CLICKED(IDC_RADIO_FP_RESETONDIS_NO, OnFpResetOnDisconnectNoBnClicked)
	//ON_BN_CLICKED(IDC_RADIO_FP_RESETONLOG_YES, OnFpResetOnLogoutYesBnClicked)
	//ON_BN_CLICKED(IDC_RADIO_FP_RESETONLOG_NO, OnFpResetOnLogoutNoBnClicked)
	//ON_EN_KILLFOCUS(IDC_EDIT_FP_SOCKCONNHOST, OnFpSocketConnHostEnKillFocus)
	//ON_EN_KILLFOCUS(IDC_EDIT_FP_SOCKCONNPORT, OnFpSocketConnPortEnKillFocus)
	//ON_EN_KILLFOCUS(IDC_EDIT_FP_SENDERID, OnFpSenderIdEnKillFocus)
	//ON_EN_KILLFOCUS(IDC_EDIT_FP_TARGETID, OnFpTargetIdEnKillFocus)
	//ON_EN_KILLFOCUS(IDC_EDIT_FP_LOGONTIMEOUT, OnFpLogonTimeoutEnKillFocus)
	//ON_EN_KILLFOCUS(IDC_EDIT_FP_RECONNECTINT, OnFpReconnectTimeEnKillFocus)	
	//ON_EN_KILLFOCUS(IDC_EDIT_FP_HEARTBEAT, OnFpHeartbeatEnKillFocus)
	//ON_EN_KILLFOCUS(IDC_EDIT_FP_MAXLATENCY, OnFpMaxLatencyEnKillFocus)	
	//ON_EN_KILLFOCUS(IDC_EDIT_FP_FILEPATH, OnFpFilePathEnKillFocus)		
	ON_EN_KILLFOCUS(IDC_EDIT_FP_ORDERIDPREF, OnFpOrderIDPrefixEnKillFocus)
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
	try
	{
		__CHECK_HRESULT(spSettings.CreateInstance(EM::CLSID_Settings), _T("Fail to create ETS seetings object."));
	}
	catch(_com_error& e)
	{
		CString strError;
		strError.Format(_T("Settings loading failed: %s (%X)"), (LPCTSTR)e.Description() ? (LPCTSTR)e.Description() : e.ErrorMessage(), e.Error());
		MessageBox(strError, NULL, MB_ICONSTOP);
	}

	_LoadDbData(spSettings);
	_LoadFpData();
}

void CConnectionsDlg::_SaveData()
{
	CWaitCursor wait;

	EM::ISettingsPtr spSettings;
	try
	{
		__CHECK_HRESULT(spSettings.CreateInstance(EM::CLSID_Settings), _T("Fail to create ETS seetings object."));
	}
	catch(_com_error& e)
	{
		CString strError;
		strError.Format(_T("Settings saving failed: %s (%X)"), (LPCTSTR)e.Description() ? (LPCTSTR)e.Description() : e.ErrorMessage(), e.Error());
		MessageBox(strError, NULL, MB_ICONSTOP);
	}

	if(m_bIsDbChanged)
		_SaveDbData(spSettings);

	if(m_bIsFpChanged)
		_SaveFpData();
}

void CConnectionsDlg::_LoadDbData(EM::ISettingsPtr& spSettings)
{
	if(spSettings != NULL)
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
void CConnectionsDlg::_LoadFpData()
{
	m_cmbFpType.ResetContent();
	m_vecFpData.clear();

	HRESULT hr;
	_variant_t varItem;
	ULONG nFetched = 0L;
	IUnknownPtr spUnk;
	FP::IFixProviderSpecificsPtr spProviderSpec;

	try
	{
		FP::IFixProvidersPtr spProviders(FP::CLSID_FixProviders);
		spProviders->Initialize();

		if(!spProviders->GetCount())
			CComErrorWrapper::ThrowError(E_FAIL, _T("No one FIX provider properly installed."));

		if(FAILED(spProviders->get__NewEnum(&spUnk)))
			return;

		IEnumVARIANTPtr spProvEnum(spUnk);
		if(FAILED(spProvEnum->Reset()))
			return;

		while((hr = spProvEnum->Next(1L, &varItem, &nFetched)) == S_OK)
		{
			ATLASSERT(varItem.vt == VT_DISPATCH);
			spProviderSpec = varItem;

			if(nFetched > 0 && spProviderSpec != NULL)
			{
				long lProvID = 0;
				BSTR bsProgId = NULL;
				BSTR bsDescr = NULL;
				VARIANT_BOOL bNeedLogin = VARIANT_FALSE;
				BSTR bsAccount = NULL;
				BSTR bsPassword = NULL;

				spProviderSpec->GetSpecifics(&lProvID, &bsProgId, &bsDescr, &bNeedLogin, &bsAccount, &bsPassword);

				CLSID clsid;
				CLSIDFromProgID(bsProgId, &clsid);

				LPOLESTR bsClsid;
				StringFromCLSID(clsid, &bsClsid);

				_LoadFpData(lProvID, bsClsid, bsDescr);
				CoTaskMemFree(bsClsid);

				SysFreeString(bsProgId);
				SysFreeString(bsDescr);
				SysFreeString(bsAccount);
				SysFreeString(bsPassword);
			}
			varItem.Clear();
			spProviderSpec = NULL;
		}
	}
	catch(_com_error& e)
	{
		_DisableFpControls();
		CString strError;
		strError.Format(_T("FIX provider settings loading failed: %s (%X)"),
			(LPCTSTR)e.Description() ? (LPCTSTR)e.Description() : e.ErrorMessage(), e.Error());
		MessageBox(strError, NULL, MB_ICONSTOP);

		return;
	}

	_UpdateFpControls();
}

void CConnectionsDlg::_LoadFpData(long nProvID, _bstr_t bsKey, _bstr_t bsDescr)
{
	//try
	//{
	//	DWORD dwResult = S_OK;

	//	CXMLParamsHelper XMLParams;
	//	XMLParams.LoadXMLParams();

	//	_bstr_t sbsUserGroup;
	//	XMLParams.GetUserGroup(sbsUserGroup.GetAddress());

	//	_bstr_t sbsValue;
	//	XMLParams.GetMainXMLString(SETTINGS_XML_KEY, sbsUserGroup, &sbsValue);
	//	
	//	const _bstr_t bsEmptyString(_T(""));

	//	XMLParams.GetMainXMLString(SETTINGS_XML_KEY, SETTINGS_XML_CLIENT, &m_bsClientID, bsEmptyString);
	//	XMLParams.GetMainXMLString(SETTINGS_XML_KEY, SETTINGS_XML_EXECBROKER, &m_bsExecBroker, bsEmptyString);

	//	CString ConfigurationFile = (LPTSTR)sbsValue;

	//	if(ConfigurationFile.GetLength())
	//	{

	//		m_pSettings     = boost::shared_ptr<FIX::SessionSettings>(new FIX::SessionSettings(ConfigurationFile.GetString()));
	//		m_pStoreFactory = boost::shared_ptr<FIX::FileStoreFactory>(new FIX::FileStoreFactory(*m_pSettings));
	//		m_pLogFactory   = boost::shared_ptr<FIX::FileLogFactory>(new FIX::FileLogFactory(*m_pSettings));     

	//		boost::shared_ptr<FIX::SocketInitiator> pInitiator = boost::shared_ptr<FIX::SocketInitiator>(new FIX::SocketInitiator(*this, *m_pStoreFactory, *m_pSettings, *m_pLogFactory));
	//		if( pInitiator != NULL )
	//		{
	//			m_pInitiator = pInitiator;
	//			m_pInitiator->start();
	//			dwResult = ERROR_SUCCESS;
	//		}
	//		else
	//			dwResult = GetLastError();
	//	}
	//	else
	//		dwResult = E_FAIL;
	//}
	//catch(FIX::ConfigError& re)
	//{
	//	std::string sMsg("FIX Configuration Error:");
	//	sMsg += re.what();
	//	return Error(sMsg.c_str(), __uuidof(IOrderProvider), E_FAIL);

	//}
	//catch(FIX::RuntimeError& re)
	//{
	//	std::string sMsg("FIX Runtime Error:");
	//	sMsg += re.what();
	//	return Error(sMsg.c_str(), __uuidof(IOrderProvider), E_FAIL);
	//}

	//catch(_com_error& e)
	//{
	//	return utils::ComError2ErrInfo(e, this, L"O'Connor FIX Provider Connect failed.", __uuidof(IOrderProvider));
	//}

	//return S_OK;
	
	try
	{
		_bstr_t bsLastOrderID;
		CXMLParamsHelper XMLParams;

		XMLParams.LoadXMLParams();
		XMLParams.GetXMLString(SETTINGS_USER_XML_KEY, SETTINGS_XML_ORDID, &bsLastOrderID, _T(""));

		CFixProviderData fpd;
		fpd.m_nID = nProvID;
		fpd.m_strName = (LPCTSTR)bsDescr;
		fpd.m_strProgID = (LPCTSTR)bsKey;

		//CString strKey = REG_KEY;
		//strKey += _T("\\");
		//strKey += (LPCTSTR)bsKey;

		//CEgRegReader key(strKey);

		//fpd.m_strConnHost = key.Read(_T("SocketConnectHost"), _T(""));
		//fpd.m_nConnPort = _tcstol(key.Read(_T("SocketConnectPort"), _T("")), NULL, 10);
		//fpd.m_strSendCompID = key.Read(_T("SenderCompID"), _T("EGAR"));
		//fpd.m_strTargCompID = key.Read(_T("TargetCompID"), _T("OCON"));
		//fpd.m_nLogonTimeOut = _tcstol(key.Read(_T("LogonTimeout"), _T("5")), NULL, 10);
		//fpd.m_nReconnectInt = _tcstol(key.Read(_T("ReconnectInterval"), _T("10")), NULL, 10);
		//fpd.m_nHeartbeatInt = _tcstol(key.Read(_T("HeartBtInt"), _T("7")), NULL, 10);
		//fpd.m_bCheckLatency = key.Read(_T("CheckLatency"), _T("N")).CompareNoCase(_T("Y")) == 0;
		//fpd.m_nMaxLatency = _tcstol(key.Read(_T("MaxLatency"), _T("0")), NULL, 10);
		//fpd.m_bResetOnDisconnect = key.Read(_T("ResetOnDisconnect"), _T("N")).CompareNoCase(_T("Y")) == 0;
		//fpd.m_bResetOnLogout = key.Read(_T("ResetOnLogout"), _T("N")).CompareNoCase(_T("Y")) == 0;
		//fpd.m_strFilePath = key.Read(_T("FileStorePath"), _T("C:\\FIXLOGS"));
		//fpd.m_strLastOrderId = key.Read(_T("LastOrderID"), _T("AAAAA00001030101"));
		
		if (bsLastOrderID.length() > 0)
			fpd.m_strLastOrderId = (LPCTSTR)bsLastOrderID;
		else
			fpd.m_strLastOrderId = L"AAAAA00001030101";
	
		m_vecFpData.push_back(fpd);
		
		long nListIdx = m_cmbFpType.AddString(fpd.m_strName);
		m_cmbFpType.SetItemData(nListIdx, nProvID);

		if(!nListIdx) m_cmbFpType.SetCurSel(nListIdx);
	}
	catch(_com_error& e)
	{
		CString strError;
		strError.Format(_T("Fix provider settings loading failed: %s (%X)"),
			(LPCTSTR)e.Description() ? (LPCTSTR)e.Description() : e.ErrorMessage(), e.Error());
		TRACE2(_T("%s: %s"), __FUNCTION__, strError);
	}
}

void CConnectionsDlg::_SaveFpData()
{
	try
	{
		while(!m_vecFpData.empty())
		{
			CFixProviderData& fpd = m_vecFpData.back();

			//CString strKey = REG_KEY;
			//strKey += _T("\\");
			//strKey += (LPCTSTR)fpd.m_strProgID;

			//CRegKey aKey;
			//LONG lResult = aKey.Open(HKEY_CURRENT_USER, (LPCTSTR)strKey, KEY_WRITE);
			//if(lResult == ERROR_FILE_NOT_FOUND)
			//	lResult = aKey.Create(HKEY_CURRENT_USER,(LPCTSTR)strKey);
			//if (lResult != ERROR_SUCCESS)
			//	throw _com_error(HRESULT_FROM_WIN32(lResult));

			//aKey.SetStringValue(_T("SocketConnectHost"), fpd.m_strConnHost);

			//CString strVal;
			//strVal.Format(_T("%d"), fpd.m_nConnPort);
			//aKey.SetStringValue(_T("SocketConnectPort"), strVal);

			//aKey.SetStringValue(_T("SenderCompID"), fpd.m_strSendCompID);

			//aKey.SetStringValue(_T("TargetCompID"), fpd.m_strTargCompID);

			//strVal.Format(_T("%d"), fpd.m_nLogonTimeOut);
			//aKey.SetStringValue(_T("LogonTimeout"), strVal);

			//strVal.Format(_T("%d"), fpd.m_nReconnectInt);
			//aKey.SetStringValue(_T("ReconnectInterval"), strVal);

			//strVal.Format(_T("%d"), fpd.m_nHeartbeatInt);
			//aKey.SetStringValue(_T("HeartBtInt"), strVal);

			//if(fpd.m_bCheckLatency)
			//{
			//	aKey.SetStringValue(_T("CheckLatency"), _T("Y"));

			//	strVal.Format(_T("%d"), fpd.m_nMaxLatency);
			//	aKey.SetStringValue(_T("MaxLatency"), strVal);
			//}
			//else
			//	aKey.SetStringValue(_T("CheckLatency"), _T("N"));

			//if(fpd.m_bResetOnDisconnect)
			//	aKey.SetStringValue(_T("ResetOnDisconnect"), _T("Y"));
			//else
			//	aKey.SetStringValue(_T("ResetOnDisconnect"), _T("N"));

			//if(fpd.m_bResetOnLogout)
			//	aKey.SetStringValue(_T("ResetOnLogout"), _T("Y"));
			//else
			//	aKey.SetStringValue(_T("ResetOnLogout"), _T("N"));

			//aKey.SetStringValue(_T("FileStorePath"), fpd.m_strFilePath);
			//aKey.SetStringValue(_T("LastOrderID"), fpd.m_strLastOrderId);

			CXMLParamsHelper XMLParams;
			XMLParams.LoadXMLParams();
			XMLParams.SetXMLString(SETTINGS_USER_XML_KEY, SETTINGS_XML_ORDID, (LPCTSTR)fpd.m_strLastOrderId);
			XMLParams.SaveXMLParams();

			m_vecFpData.pop_back();
		}
	}
	catch(_com_error& e)
	{
		CString strError;
		strError.Format(_T("Fix provider settings saving failed: %s (%X)"),
			(LPCTSTR)e.Description() ? (LPCTSTR)e.Description() : e.ErrorMessage(), e.Error());
		TRACE2(_T("%s: %s"), __FUNCTION__, strError);
	}
}

void CConnectionsDlg::_UpdateFpControls()
{
	size_t nFpIdx = m_cmbFpType.GetCurSel();

	if(nFpIdx >= 0L && nFpIdx < m_vecFpData.size())
	{
		const CFixProviderData& fpd = m_vecFpData[nFpIdx];
		//_ControlText[IDC_EDIT_FP_SOCKCONNHOST] = fpd.m_strConnHost;

		//CString strVal;
		//strVal.Format(_T("%d"), fpd.m_nConnPort);
		//_ControlText[IDC_EDIT_FP_SOCKCONNPORT] = strVal;

		//_ControlText[IDC_EDIT_FP_SENDERID] = fpd.m_strSendCompID;
		//_ControlText[IDC_EDIT_FP_TARGETID] = fpd.m_strTargCompID;

		//strVal.Format(_T("%d"), fpd.m_nLogonTimeOut);
		//_ControlText[IDC_EDIT_FP_LOGONTIMEOUT] = strVal;

		//strVal.Format(_T("%d"), fpd.m_nReconnectInt);
		//_ControlText[IDC_EDIT_FP_RECONNECTINT] = strVal;

		//strVal.Format(_T("%d"), fpd.m_nHeartbeatInt);
		//_ControlText[IDC_EDIT_FP_HEARTBEAT] = strVal;

		//_ControlChecked[IDC_CHECK_FP_LATENCY] = fpd.m_bCheckLatency;

		//strVal.Format(_T("%d"), fpd.m_nMaxLatency);
		//_ControlText[IDC_EDIT_FP_MAXLATENCY] = strVal;

		//_ControlChecked[IDC_RADIO_FP_RESETONDIS_YES] = fpd.m_bResetOnDisconnect;
		//_ControlChecked[IDC_RADIO_FP_RESETONDIS_NO] = !fpd.m_bResetOnDisconnect;
		//_ControlChecked[IDC_RADIO_FP_RESETONLOG_YES] = fpd.m_bResetOnLogout;
		//_ControlChecked[IDC_RADIO_FP_RESETONLOG_NO] = !fpd.m_bResetOnLogout;

		//_ControlText[IDC_EDIT_FP_FILEPATH] = fpd.m_strFilePath;
		_ControlText[IDC_EDIT_FP_ORDERIDPREF] = fpd.m_strLastOrderId.Left(5);
	}

	//_UpdateFpControlsState();
}

void CConnectionsDlg::_UpdateFpControlsState()
{
	//size_t nFpIdx = m_cmbFpType.GetCurSel();
	//if(nFpIdx >= 0L && nFpIdx < m_vecFpData.size())
	//{
	//	const CFixProviderData& fpd = m_vecFpData[nFpIdx];

	//	_ControlEnabled[IDC_EDIT_FP_MAXLATENCY] = fpd.m_bCheckLatency;
	//}
}

void CConnectionsDlg::_DisableFpControls()
{
	_ControlEnabled[IDC_COMBO_FP_TYPE] = false;
	_ControlEnabled[IDC_EDIT_FP_SOCKCONNHOST] = false;
	_ControlEnabled[IDC_EDIT_FP_SOCKCONNPORT] = false;
	_ControlEnabled[IDC_EDIT_FP_SENDERID] = false;
	_ControlEnabled[IDC_EDIT_FP_TARGETID] = false;
	_ControlEnabled[IDC_EDIT_FP_LOGONTIMEOUT] = false;
	_ControlEnabled[IDC_EDIT_FP_RECONNECTINT] = false;
	_ControlEnabled[IDC_EDIT_FP_HEARTBEAT] = false;	
	_ControlEnabled[IDC_CHECK_FP_LATENCY] = false;
	_ControlEnabled[IDC_EDIT_FP_MAXLATENCY] = false;
	_ControlEnabled[IDC_RADIO_FP_RESETONDIS_YES] = false;
	_ControlEnabled[IDC_RADIO_FP_RESETONDIS_NO] = false;
	_ControlEnabled[IDC_RADIO_FP_RESETONLOG_YES] = false;
	_ControlEnabled[IDC_RADIO_FP_RESETONLOG_NO] = false;
	_ControlEnabled[IDC_EDIT_FP_FILEPATH] = false;
	_ControlEnabled[IDC_BUTTON_FP_FILEPATH] = false;

	_ControlEnabled[IDC_STATIC_FP_TYPE] = false;
	_ControlEnabled[IDC_STATIC_FP_SOCKCONNHOST] = false;
	_ControlEnabled[IDC_STATIC_FP_SOCKCONNPORT] = false;
	_ControlEnabled[IDC_STATIC_FP_SENDERID] = false;
	_ControlEnabled[IDC_STATIC_FP_TARGETID] = false;
	_ControlEnabled[IDC_STATIC_FP_LOGONTIMEOUT] = false;
	_ControlEnabled[IDC_STATIC_FP_RECONNECTINT] = false;
	_ControlEnabled[IDC_STATIC_FP_HEARTBEAT] = false;
	_ControlEnabled[IDC_STATIC_FP_RESETONDISC] = false;
	_ControlEnabled[IDC_STATIC_FP_RESETONLOGOUT] = false;
	_ControlEnabled[IDC_STATIC_FP_FILEPATH] = false;
	_ControlEnabled[IDC_EDIT_FP_ORDERIDPREF] = false;
}

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

void CConnectionsDlg::OnFpTypeSelChange()
{
	_UpdateFpControls();
}

void CConnectionsDlg::OnFpLatencyBnClicked()
{
	//size_t nFpIdx = m_cmbFpType.GetCurSel();
	//if(nFpIdx >= 0L && nFpIdx < m_vecFpData.size())
	//{
	//	m_vecFpData[nFpIdx].m_bCheckLatency = _ControlChecked[IDC_CHECK_FP_LATENCY] ? true : false;
	//	m_bIsFpChanged = true;
	//}
	//_UpdateFpControlsState();
}

void CConnectionsDlg::OnFpResetOnDisconnectYesBnClicked()
{
	//size_t nFpIdx = m_cmbFpType.GetCurSel();
	//if(nFpIdx >= 0L && nFpIdx < m_vecFpData.size())
	//{
	//	m_vecFpData[nFpIdx].m_bResetOnDisconnect = true;
	//	m_bIsFpChanged = true;
	//}
}

void CConnectionsDlg::OnFpResetOnDisconnectNoBnClicked()
{
	//size_t nFpIdx = m_cmbFpType.GetCurSel();
	//if(nFpIdx >= 0L && nFpIdx < m_vecFpData.size())
	//{
	//	m_vecFpData[nFpIdx].m_bResetOnDisconnect = false;
	//	m_bIsFpChanged = true;
	//}
}

void CConnectionsDlg::OnFpResetOnLogoutYesBnClicked()
{
	//size_t nFpIdx = m_cmbFpType.GetCurSel();
	//if(nFpIdx >= 0L && nFpIdx < m_vecFpData.size())
	//{
	//	m_vecFpData[nFpIdx].m_bResetOnLogout = true;
	//	m_bIsFpChanged = true;
	//}
}

void CConnectionsDlg::OnFpResetOnLogoutNoBnClicked()
{
	//size_t nFpIdx = m_cmbFpType.GetCurSel();
	//if(nFpIdx >= 0L && nFpIdx < m_vecFpData.size())
	//{
	//	m_vecFpData[nFpIdx].m_bResetOnLogout = false;
	//	m_bIsFpChanged = true;
	//}
}

void CConnectionsDlg::OnFpSocketConnHostEnKillFocus()
{
	//size_t nFpIdx = m_cmbFpType.GetCurSel();
	//if(nFpIdx >= 0L && nFpIdx < m_vecFpData.size())
	//{
	//	m_vecFpData[nFpIdx].m_strConnHost = _ControlText[IDC_EDIT_FP_SOCKCONNHOST];
	//	m_bIsFpChanged = true;
	//}
}

void CConnectionsDlg::OnFpSocketConnPortEnKillFocus()
{
	//size_t nFpIdx = m_cmbFpType.GetCurSel();
	//if(nFpIdx >= 0L && nFpIdx < m_vecFpData.size())
	//{
	//	m_vecFpData[nFpIdx].m_nConnPort = _ttol(_ControlText[IDC_EDIT_FP_SOCKCONNPORT].Trim());
	//	m_bIsFpChanged = true;
	//}
}

void CConnectionsDlg::OnFpSenderIdEnKillFocus()
{
	//size_t nFpIdx = m_cmbFpType.GetCurSel();
	//if(nFpIdx >= 0L && nFpIdx < m_vecFpData.size())
	//{
	//	m_vecFpData[nFpIdx].m_strSendCompID = _ControlText[IDC_EDIT_FP_SENDERID];
	//	m_bIsFpChanged = true;
	//}
}

void CConnectionsDlg::OnFpTargetIdEnKillFocus()
{
	//size_t nFpIdx = m_cmbFpType.GetCurSel();
	//if(nFpIdx >= 0L && nFpIdx < m_vecFpData.size())
	//{
	//	m_vecFpData[nFpIdx].m_strTargCompID = _ControlText[IDC_EDIT_FP_TARGETID];
	//	m_bIsFpChanged = true;
	//}
}

void CConnectionsDlg::OnFpLogonTimeoutEnKillFocus()
{
	//size_t nFpIdx = m_cmbFpType.GetCurSel();
	//if(nFpIdx >= 0L && nFpIdx < m_vecFpData.size())
	//{
	//	m_vecFpData[nFpIdx].m_nLogonTimeOut = _ttol(_ControlText[IDC_EDIT_FP_LOGONTIMEOUT].Trim());
	//	m_bIsFpChanged = true;
	//}
}

void CConnectionsDlg::OnFpReconnectTimeEnKillFocus()
{
	//size_t nFpIdx = m_cmbFpType.GetCurSel();
	//if(nFpIdx >= 0L && nFpIdx < m_vecFpData.size())
	//{
	//	m_vecFpData[nFpIdx].m_nReconnectInt = _ttol(_ControlText[IDC_EDIT_FP_RECONNECTINT].Trim());
	//	m_bIsFpChanged = true;
	//}
}

void CConnectionsDlg::OnFpHeartbeatEnKillFocus()
{
	//size_t nFpIdx = m_cmbFpType.GetCurSel();
	//if(nFpIdx >= 0L && nFpIdx < m_vecFpData.size())
	//{
	//	m_vecFpData[nFpIdx].m_nHeartbeatInt = _ttol(_ControlText[IDC_EDIT_FP_HEARTBEAT].Trim());
	//	m_bIsFpChanged = true;
	//}
}

void CConnectionsDlg::OnFpMaxLatencyEnKillFocus()
{
	//size_t nFpIdx = m_cmbFpType.GetCurSel();
	//if(nFpIdx >= 0L && nFpIdx < m_vecFpData.size())
	//{
	//	m_vecFpData[nFpIdx].m_nMaxLatency = _ttol(_ControlText[IDC_EDIT_FP_MAXLATENCY].Trim());
	//	m_bIsFpChanged = true;
	//}
}

void CConnectionsDlg::OnFpFilePathEnKillFocus()
{
	//size_t nFpIdx = m_cmbFpType.GetCurSel();
	//if(nFpIdx >= 0L && nFpIdx < m_vecFpData.size())
	//{
	//	m_vecFpData[nFpIdx].m_strFilePath = _ControlText[IDC_EDIT_FP_FILEPATH];
	//	m_bIsFpChanged = true;
	//}
}

void CConnectionsDlg::OnFpOrderIDPrefixEnKillFocus()
{
	size_t nFpIdx = m_cmbFpType.GetCurSel();
	if(nFpIdx >= 0L && nFpIdx < m_vecFpData.size())
	{

		CString strTemp = m_vecFpData[nFpIdx].m_strLastOrderId.Right(m_vecFpData[nFpIdx].m_strLastOrderId.GetLength() - 5);
		m_vecFpData[nFpIdx].m_strLastOrderId = _ControlText[IDC_EDIT_FP_ORDERIDPREF].Left(5) + strTemp;
		m_bIsFpChanged = true;
	}
}

void CConnectionsDlg::OnDbFilePathClicked()
{
	//BROWSEINFO bi;
	//LPITEMIDLIST pidl;
	//LPMALLOC pMalloc;
	//ZeroMemory( &bi, sizeof(bi) );

	//if (SUCCEEDED(SHGetMalloc(&pMalloc)))
	//{
	//	ZeroMemory(&bi,sizeof(bi));
	//	bi.hwndOwner = m_hWnd;
	//	bi.pszDisplayName = 0;
	//	bi.pidlRoot = 0;
	//	bi.ulFlags = BIF_RETURNONLYFSDIRS;
	//	bi.lpfn = CConnectionsDlg::BrowseCallbackProc;
	//	bi.lParam = (LPARAM)this;

	//	pidl = SHBrowseForFolder(&bi); 
	//	if (pidl) 
	//	{ 

	//		size_t nFpIdx = m_cmbFpType.GetCurSel();
	//		if(nFpIdx >= 0L && nFpIdx < m_vecFpData.size())
	//		{
	//			TCHAR szDir[MAX_PATH];
	//			if(SHGetPathFromIDList(pidl, szDir))
	//			{
	//				m_vecFpData[nFpIdx].m_strFilePath = szDir;
	//				m_bIsFpChanged = true;
	//			}
	//		}

	//		pMalloc->Free(pidl);
	//	} 
	//	pMalloc->Release();
	//}

	//_UpdateFpControls();
}

void CConnectionsDlg::OnOK()
{
	if(m_bIsDbChanged || m_bIsFpChanged)
		_SaveData();


	CDialog::OnOK();
}

int CConnectionsDlg::BrowseCallbackProc(HWND hwnd, UINT uMsg, LPARAM lp, LPARAM pData)
{
	TCHAR szDir[MAX_PATH];

	switch(uMsg) 
	{
		case BFFM_INITIALIZED: 
		{
			CConnectionsDlg* pDlg = reinterpret_cast<CConnectionsDlg*>(pData);

			if(pDlg)
			{
				size_t nFpIdx = pDlg->m_cmbFpType.GetCurSel();
				if(nFpIdx >= 0L && nFpIdx < pDlg->m_vecFpData.size() && pDlg->m_vecFpData[nFpIdx].m_strFilePath.GetLength())
				{
					// WParam is TRUE since you are passing a path.
					// It would be FALSE if you were passing a pidl.
					::SendMessage(hwnd, BFFM_SETSELECTION, TRUE, (LPARAM)(LPCTSTR)pDlg->m_vecFpData[nFpIdx].m_strFilePath);
				}
			}
		}
		break;

		case BFFM_SELCHANGED: 
		// Set the status window to the currently selected path.
		if (SHGetPathFromIDList((LPITEMIDLIST) lp ,szDir))
		{
			::SendMessage(hwnd,BFFM_SETSTATUSTEXT,0,(LPARAM)szDir);
		}
		break;
	}
	return 0;
}


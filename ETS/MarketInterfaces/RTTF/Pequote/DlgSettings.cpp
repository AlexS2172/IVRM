//**************************************************************************************************************//
// solution:	Pequote
// project:		
// filename:	DlgSettings.cpp
// created:		28-Dec-2002 17:47 by Suchkov Dmitry
//
// purpose:		implementation of CDlgSettings
//
//**************************************************************************************************************//

//**************************************************************************************************************//
// includes
//**************************************************************************************************************//
#include "stdafx.h"
#include "Pequote.h"
#include "DlgSettings.h"
#include "FrmMain.h"
#include ".\dlgsettings.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//--------------------------------------------------------------------------------------------------------------//
BEGIN_MESSAGE_MAP(CDlgSettings, CDialog)
	//{{AFX_MSG_MAP(CDlgSettings)
	ON_BN_CLICKED(IDC_BT_SETUP, OnBtSetup)
	ON_EN_CHANGE(IDC_ED_ADDRESS, OnChangeEdits)
	ON_EN_CHANGE(IDC_ED_USER, OnChangeEdits)
	ON_EN_CHANGE(IDC_ED_PASSWORD, OnChangeEdits)
	ON_EN_CHANGE(IDC_ED_TRADES_FILE, OnChangeEdits)
	ON_EN_CHANGE(IDC_ED_READ_TRADES_PERIOD, OnChangeEdits)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_DT_DOWNLOADPOSITIONTIME, OnDatetimechangeDtDownloadpositiontime)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_CHECK_PASSIVE, OnBnClickedCheckPassive)
	ON_BN_CLICKED(IDC_CHK_ENCRYPTED, OnBnClickedChkEncrypted)
	ON_BN_CLICKED(IDC_PQ_SETUP, OnPQBtnSetup)
END_MESSAGE_MAP()

//--------------------------------------------------------------------------------------------------------------//
CDlgSettings::CDlgSettings(const std::string& sSessionID, CWnd* pParent /*=NULL*/)
:	CDialog(CDlgSettings::IDD, pParent),
	m_bSettingChanged(false)
	, m_bIsFtpPassive(TRUE)
	, m_bEncryptedSource(FALSE)
	, m_strPassPhrase(_T(""))
	, m_SessionID(sSessionID)
{
	//{{AFX_DATA_INIT(CDlgSettings)
	m_sAddress = _T("");
	m_sDatabase = _T("");
	m_sPQDatabase = _T("");
	m_sPassword = _T("");
	m_sTradesFile = _T("");
	m_sUser = _T("");
	m_nReadTradesPeriod = 0;
	//}}AFX_DATA_INIT
}

//--------------------------------------------------------------------------------------------------------------//
void CDlgSettings::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgSettings)
	DDX_Text(pDX, IDC_ED_ADDRESS, m_sAddress);
	DDV_MaxChars(pDX, m_sAddress, 255);
	DDX_Text(pDX, IDC_ED_DATABASE, m_sDatabase);
	DDX_Text(pDX, IDC_PQ_DATABASE, m_sPQDatabase);
	DDX_Text(pDX, IDC_ED_PASSWORD, m_sPassword);
	DDV_MaxChars(pDX, m_sPassword, 64);
	DDX_Text(pDX, IDC_ED_TRADES_FILE, m_sTradesFile);
	DDV_MaxChars(pDX, m_sTradesFile, 255);
	DDX_Text(pDX, IDC_ED_USER, m_sUser);
	DDV_MaxChars(pDX, m_sUser, 64);
	DDX_Text(pDX, IDC_ED_READ_TRADES_PERIOD, m_nReadTradesPeriod);
	DDV_MinMaxUInt(pDX, m_nReadTradesPeriod, 0, 86399);
	//}}AFX_DATA_MAP
	DDX_Check(pDX, IDC_CHECK_PASSIVE, m_bIsFtpPassive);
	DDX_Check(pDX, IDC_CHK_ENCRYPTED, m_bEncryptedSource);
	DDX_Text(pDX, IDC_ED_PASSPHRASE, m_strPassPhrase);
}

//--------------------------------------------------------------------------------------------------------------//
BOOL CDlgSettings::OnInitDialog() 
{
	CDialog::OnInitDialog();

	USES_CONVERSION;

	try
    {
        ISettingsPtr spSettings;
        HRESULT hr = spSettings.CreateInstance(__uuidof(Settings));
        if (FAILED(hr))
            utils::ThrowErrorNoSetErrorInfo(hr, L"Failed to create Settings object.");

        m_sDatabase = (LPCTSTR)spSettings->DatabaseConnection;


		//CString sVal;
        //DWORD   nVal;
		_bstr_t sbsVal;
		long nVal;

		_bstr_t sbsKey = SETTINGS_XML_KEY;
		sbsKey += "\\";
		sbsKey += m_SessionID.c_str();

		CXMLParamsHelper XMLParams;
		XMLParams.LoadXMLParams();

		XMLParams.GetMainXMLString(sbsKey, SETTINGS_PQ_CONNECT, &sbsVal);
		if (sbsVal.length() > 0)
			m_sPQDatabase = (LPCSTR)sbsVal;
		else
			m_bSettingChanged = true;

		m_sAddress = _T("ftp.ePequote.com");
		m_sPassword = _T("anonymous");
		m_sUser = _T("anonymous");
		m_sTradesFile = _T("trades.out");
		m_nReadTradesPeriod = 60;
		//m_dtDownloadPositionTime.SetTime(19, 0, 0);

		XMLParams.GetMainXMLString(sbsKey, SETTINGS_SERVER_ADDRESS, &sbsVal);
		if(sbsVal.length() > 0)
		{
			m_sAddress = (LPCSTR)sbsVal;
			m_sPassword = _T("");
			m_sUser = _T("");
		}
		else
		{
			m_bSettingChanged = true;
		}

		XMLParams.GetMainXMLString(sbsKey, SETTINGS_PASSWORD, &sbsVal);
		if(sbsVal.length() > 0)
			m_sPassword = (LPCSTR)sbsVal;
		else
			m_bSettingChanged = true;

		XMLParams.GetMainXMLString(sbsKey, SETTINGS_USER, &sbsVal);
		if(sbsVal.length() > 0)
			m_sUser = (LPCSTR)sbsVal;
		else
			m_bSettingChanged = true;

		XMLParams.GetMainXMLLong(sbsKey, SETTINGS_PASSIVEFTP, &nVal);
		if(sbsVal.length() > 0)
			m_bIsFtpPassive = nVal!=0;
		else
			m_bSettingChanged = true;	

		XMLParams.GetMainXMLString(sbsKey, SETTINGS_TRADES_FILENAME, &sbsVal);
		if(sbsVal.length() > 0)
			m_sTradesFile = (LPCSTR)sbsVal;
		else
			m_bSettingChanged = true;   

		//XMLParams.GetMainXMLString(sbsKey, SETTINGS_POSITIONS_FILENAME, &sbsVal);
		//if(sbsVal.length() > 0)
			//m_sPositionFile = (LPCSTR)sbsVal;
		//else
			//m_bSettingChanged = true;

		XMLParams.GetMainXMLLong(sbsKey, SETTINGS_READ_TRADES_PERIOD, &nVal);
		if (nVal > 0) 
			m_nReadTradesPeriod = nVal;
		else
			m_bSettingChanged = true;

		//XMLParams.GetMainXMLString(sbsKey, SETTINGS_POSITIONS_DOWNLOAD_TIME, &sbsVal);
		//COleDateTime dtTime;
		//if (dtTime.ParseDateTime(sbsVal, VAR_TIMEVALUEONLY))
			//m_dtDownloadPositionTime = dtTime;
		//else
			//m_bSettingChanged = true;

		long nEncSrc = 0;
		XMLParams.GetMainXMLLong(sbsKey, SETTINGS_READ_ENCRYPTED, &nEncSrc);
		m_bEncryptedSource = nEncSrc != 0;

		XMLParams.GetMainXMLString(sbsKey, SETTINGS_READ_PASSPHRASE, &sbsVal);
		if (sbsVal.length() > 0) 
			m_strPassPhrase = (LPCSTR)sbsVal;
		else
			m_bSettingChanged = true;
		
    }
    catch(_com_error& e)
    {
		m_bSettingChanged = true;

        CString strErr;
        strErr.Format( _T("Error occured while read data from registry. ")
            _T("Error = 0x%.8X, (%s)"), e.Error(), (LPCTSTR)(_bstr_t)e.Description());
        AfxMessageBox(strErr);
    }
		
	if (m_bSettingChanged)
		SaveSettings();

	CWnd* pItem = GetDlgItem(IDC_ED_PASSPHRASE);
	if(pItem)
		pItem->EnableWindow(m_bEncryptedSource);

	UpdateData(FALSE);
	return TRUE;
}

//--------------------------------------------------------------------------------------------------------------//
void CDlgSettings::OnOK() 
{
	if (FALSE == UpdateData(TRUE)) return;

	if (SaveSettings())
	{
		CFrmMain* pFrmMain = static_cast<CFrmMain*>(AfxGetMainWnd());
		if (pFrmMain && pFrmMain->IsRunning())
			AfxMessageBox(IDS_CHANGES_APPLY);			
	}
        
    CDialog::OnOK();
}

bool CDlgSettings::SaveSettings()
{
	bool bRet = false;
    try
    {

        if (m_bSettingChanged)
		{
			ISettingsPtr spSettings;
            HRESULT hr = spSettings.CreateInstance(__uuidof(Settings));
            if (FAILED(hr))
                utils::ThrowErrorNoSetErrorInfo(hr, L"Failed to create Settings object.");

            spSettings->DatabaseConnection = (LPCTSTR)m_sDatabase;

			CXMLParamsHelper XMLParams;
			XMLParams.LoadXMLParams();

			_bstr_t sbsKey = SETTINGS_XML_KEY;
			sbsKey += "\\";
			sbsKey += m_SessionID.c_str();

			XMLParams.SetMainXMLString(sbsKey, SETTINGS_PQ_CONNECT, (LPCTSTR)m_sPQDatabase);
			XMLParams.SetMainXMLString(sbsKey, SETTINGS_SERVER_ADDRESS, (LPCTSTR)m_sAddress);
			XMLParams.SetMainXMLString(sbsKey, SETTINGS_USER, (LPCTSTR)m_sUser);
			XMLParams.SetMainXMLLong(sbsKey, SETTINGS_PASSIVEFTP, (long)m_bIsFtpPassive ? 1 : 0);
			XMLParams.SetMainXMLString(sbsKey, SETTINGS_PASSWORD, (LPCTSTR)m_sPassword);
			XMLParams.SetMainXMLString(sbsKey, SETTINGS_TRADES_FILENAME, (LPCTSTR)m_sTradesFile);
			//XMLParams.SetMainXMLString(sbsKey, SETTINGS_POSITIONS_FILENAME, (LPCTSTR)m_sPositionFile);
			XMLParams.SetMainXMLLong(sbsKey, SETTINGS_READ_TRADES_PERIOD, (long)m_nReadTradesPeriod);
			//XMLParams.SetMainXMLString(sbsKey, SETTINGS_POSITIONS_DOWNLOAD_TIME, (LPCTSTR)m_dtDownloadPositionTime.Format(VAR_TIMEVALUEONLY));
			XMLParams.SetMainXMLLong(sbsKey, SETTINGS_READ_ENCRYPTED, (long)m_bEncryptedSource ? 1 : 0);
			XMLParams.SetMainXMLString(sbsKey, SETTINGS_READ_PASSPHRASE, (LPCTSTR)m_strPassPhrase);

			XMLParams.SaveMainXMLParams();

			m_bSettingChanged = false;
			bRet = true;
		}
    }
    catch (_com_error& e)
    {
        CString strErr;
        strErr.Format( _T("Error occured while set data to service ")
            _T("settings object. Error = 0x%.8X"), e.Error());
        AfxMessageBox(strErr);
    }

	return bRet;
}

//--------------------------------------------------------------------------------------------------------------//
void CDlgSettings::OnCancel() 
{
	CDialog::OnCancel();
}

//--------------------------------------------------------------------------------------------------------------//
void CDlgSettings::OnBtSetup() 
{
	m_bSettingChanged = true;

    CDBConnection db;
    try 
    {
		Oledb32::IDataSourceLocatorPtr spLocator;

		HRESULT hr = spLocator.CreateInstance (__uuidof (Oledb32::DataLinks));
        if (FAILED(hr))
            utils::ThrowErrorNoSetErrorInfo(hr, L"Failed to create DataLinks object.");
      
        spLocator->hWnd = (long)m_hWnd;

        _ConnectionPtr  spConnection;
    	hr = spConnection.CreateInstance (__uuidof (Connection));
        if (FAILED(hr))
            utils::ThrowErrorNoSetErrorInfo(hr, L"Failed to create Connection object.");
    
        spConnection->ConnectionString = (LPCTSTR)m_sDatabase;

        CComPtr<IDispatch> spDispatch = spConnection;
        if (spDispatch == 0)
            utils::ThrowErrorNoSetErrorInfo(E_NOINTERFACE, L"Failed to get IDispatch interface.");

        if (spLocator->PromptEdit(&spDispatch.p) == VARIANT_TRUE)
        {
            _ConnectionPtr  spNewConnection = NULL;
			
			spDispatch.QueryInterface(&spNewConnection);

            if (spNewConnection == 0)
                utils::ThrowErrorNoSetErrorInfo(E_NOINTERFACE, L"Failed to get IDispatch interface.");

            CString strConnectionString = (LPCTSTR)spNewConnection->ConnectionString;
            
            try
            {
                db.Open((LPCTSTR)strConnectionString, 120, 120);
                CStoredProc<CDefaultRecordset> spVer(db, L"usp_DataInfo_Get");	
                spVer << 1;
                spVer.Open();
                spVer[L"vcKeyValue"];
                spVer.Close();
                db.Close();

                if (m_sDatabase != strConnectionString)
                {
                    m_sDatabase = strConnectionString;
                    UpdateData(FALSE);
                }
            }
            catch (_com_error& e)
            {
                try
                {
                    if (db.IsOpened())
                        db.Close();
                } catch (_com_error&) { }

                CString err = _T("Invalid database. ");
                err += (LPTSTR) e.Description();
                AfxMessageBox(err);
            }
        }
    }
    catch (_com_error& e)
    {
        CString err = _T("Can't show database connection dialog.");
        err += (LPTSTR) e.Description();
        AfxMessageBox(err);
    }	
}

//--------------------------------------------------------------------------------------------------------------//
void CDlgSettings::OnChangeEdits() 
{
	m_bSettingChanged = true;	
}

//--------------------------------------------------------------------------------------------------------------//
void CDlgSettings::OnDatetimechangeDtDownloadpositiontime(NMHDR* pNMHDR, LRESULT* pResult) 
{
	m_bSettingChanged = true;
	*pResult = 0;
}

void CDlgSettings::OnBnClickedCheckPassive()
{
	m_bSettingChanged = true;
}

void CDlgSettings::OnBnClickedChkEncrypted()
{
	m_bSettingChanged = true;
	UpdateData(TRUE);
	CWnd* pItem = GetDlgItem(IDC_ED_PASSPHRASE);
	if(pItem)
		pItem->EnableWindow(m_bEncryptedSource);
}

void CDlgSettings::OnPQBtnSetup()
{
	m_bSettingChanged = true;

	CDBConnection db;
	try 
	{
		Oledb32::IDataSourceLocatorPtr spLocator;

		HRESULT hr = spLocator.CreateInstance (__uuidof (Oledb32::DataLinks));
		if (FAILED(hr))
			utils::ThrowErrorNoSetErrorInfo(hr, L"Failed to create DataLinks object.");

		spLocator->hWnd = (long)m_hWnd;

		_ConnectionPtr  spConnection;
		hr = spConnection.CreateInstance (__uuidof (Connection));
		if (FAILED(hr))
			utils::ThrowErrorNoSetErrorInfo(hr, L"Failed to create Connection object.");

		spConnection->ConnectionString = (LPCTSTR)m_sPQDatabase;

		CComPtr<IDispatch> spDispatch = spConnection;
		if (spDispatch == 0)
			utils::ThrowErrorNoSetErrorInfo(E_NOINTERFACE, L"Failed to get IDispatch interface.");

		if (spLocator->PromptEdit(&spDispatch.p) == VARIANT_TRUE)
		{
			_ConnectionPtr  spNewConnection = NULL;

			spDispatch.QueryInterface(&spNewConnection);

			if (spNewConnection == 0)
				utils::ThrowErrorNoSetErrorInfo(E_NOINTERFACE, L"Failed to get IDispatch interface.");

			CString strConnectionString = (LPCTSTR)spNewConnection->ConnectionString;

			try
			{
				db.Open((LPCTSTR)strConnectionString, 120, 120);
				CStoredProc<CDefaultRecordset> spVer(db, L"usp_DataInfo_Get");	
				spVer << 1;
				spVer.Open();
				spVer[L"vcKeyValue"];
				spVer.Close();
				db.Close();

				if (m_sPQDatabase != strConnectionString)
				{
					m_sPQDatabase = strConnectionString;
					UpdateData(FALSE);
				}
			}
			catch (_com_error& e)
			{
				try
				{
					if (db.IsOpened())
						db.Close();
				} catch (_com_error&) { }

				CString err = _T("Invalid database. ");
				err += (LPTSTR) e.Description();
				AfxMessageBox(err);
			}
		}
	}
	catch (_com_error& e)
	{
		CString err = _T("Can't show database connection dialog.");
		err += (LPTSTR) e.Description();
		AfxMessageBox(err);
	}	
	
}

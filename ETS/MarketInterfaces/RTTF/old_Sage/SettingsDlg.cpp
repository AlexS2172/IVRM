// SettingsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "resource_GUI.h"
#include "SettingsDlg.h"
#include "FixConnector.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSettingsDlg dialog


CSettingsDlg::CSettingsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSettingsDlg::IDD, pParent),
    m_bFixConnectionModified(FALSE),
    m_bDBConnectionModified(FALSE)
{
	//{{AFX_DATA_INIT(CSettingsDlg)
	m_DBConnectionString = _T("");
	m_DBConnectionStringFull = _T("");
	m_Address = _T("");
	m_Port = 0;
	m_ClientLogin = _T("");
	m_ServerLogin = _T("");
	m_ReconnectTimeout = 0;
	//}}AFX_DATA_INIT
}


void CSettingsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSettingsDlg)
	DDX_Text(pDX, IDC_CONNECTION_STRING, m_DBConnectionString);
	DDX_Text(pDX, IDC_ADDRESS, m_Address);
	DDX_Text(pDX, IDC_PORT, m_Port);
	DDX_Text(pDX, IDC_CLIENT_LOGIN, m_ClientLogin);
	DDX_Text(pDX, IDC_SERVER_LOGIN, m_ServerLogin);
	DDX_Text(pDX, IDC_RECONNECT_TIMEOUT, m_ReconnectTimeout);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CSettingsDlg, CDialog)
	//{{AFX_MSG_MAP(CSettingsDlg)
	ON_BN_CLICKED(IDC_SETUP, OnSetup)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSettingsDlg message handlers

BOOL CSettingsDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
    try
    {
        ISettingsPtr spSettings;
        HRESULT hr = spSettings.CreateInstance(__uuidof(Settings));
        if (FAILED(hr))
            utils::ThrowErrorNoSetErrorInfo(hr, L"Failed to create Settings object.");

        m_RenegadeDBConnectionString = m_DBConnectionStringFull = 
            (LPCTSTR)spSettings->DatabaseConnection;		
		
		m_DBConnectionString = GetShortStringFromFull( spSettings->GetDatabaseConnection() );

        CString sVal;
        DWORD   nVal;

        GetSetting(SETTINGS_SERVER_ADDR_KEY, sVal);
        m_RenegadeAddress = m_Address = sVal;
        GetSetting(SETTINGS_SERVER_PORT_KEY, nVal);
        m_RenegadePort = m_Port = nVal;
        GetSetting(SETTINGS_TARGET_COMP_ID_KEY, sVal);
        m_RenegadeServerLogin = m_ServerLogin = sVal;
        GetSetting(SETTINGS_SENDER_COMP_ID_KEY, sVal);
        m_RenegadeClientLogin = m_ClientLogin = sVal;
        GetSetting(SETTINGS_RECONNECT_TIME_KEY, nVal);
        m_RenegadeReconnectTimeout = m_ReconnectTimeout = nVal;
    }
    catch(_com_error& e)
    {
        CString strErr;
        strErr.Format( _T("Error occured while get data from service ")
            _T("settings object. Error = 0x%.8X"), e.Error());
        AfxMessageBox(strErr);
    }
	
    UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
 
//////////////////////////////////////////////////////////////////////////////

void CSettingsDlg::OnSetup() 
{
    CDBConnection db;
    try 
    {
        IDataSourceLocatorPtr spLocator;

        HRESULT hr = spLocator.CreateInstance (__uuidof (DataLinks));
        if (FAILED(hr))
            utils::ThrowErrorNoSetErrorInfo(hr, L"Failed to create DataLinks object.");
      
        spLocator->hWnd = (long)m_hWnd;

        _ConnectionPtr  spConnection;
    	hr = spConnection.CreateInstance (__uuidof (Connection));
        if (FAILED(hr))
            utils::ThrowErrorNoSetErrorInfo(hr, L"Failed to create Connection object.");
    
        spConnection->ConnectionString = (LPCTSTR)m_DBConnectionStringFull;

        CComPtr<IDispatch> spDispatch = spConnection;
        if (spDispatch == 0)
            utils::ThrowErrorNoSetErrorInfo(E_NOINTERFACE, L"Failed to get IDispatch interface.");

        if (spLocator->PromptEdit(&spDispatch.p) == VARIANT_TRUE)
        {
            _ConnectionPtr  spNewConnection = spDispatch;

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

                if (m_DBConnectionStringFull != strConnectionString)
                {
                    m_DBConnectionStringFull = strConnectionString;
					m_DBConnectionString = 
						    GetShortStringFromFull(spNewConnection->ConnectionString);
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

//////////////////////////////////////////////////////////////////////////////

void CSettingsDlg::OnOK()
{
    UpdateData(TRUE);

    try
    {
        if (m_DBConnectionStringFull != m_RenegadeDBConnectionString)
        {
            ISettingsPtr spSettings;
            HRESULT hr = spSettings.CreateInstance(__uuidof(Settings));
            if (FAILED(hr))
                utils::ThrowErrorNoSetErrorInfo(hr, L"Failed to create Settings object.");

            spSettings->DatabaseConnection = (LPCTSTR)m_DBConnectionStringFull;
            m_bDBConnectionModified = TRUE;
        }

        if (m_RenegadeAddress != m_Address)
        {
            SetSetting(SETTINGS_SERVER_ADDR_KEY, m_Address);
            m_bFixConnectionModified = TRUE;
        }

        if (m_RenegadePort != m_Port)
        {
            SetSetting(SETTINGS_SERVER_PORT_KEY, m_Port);
            m_bFixConnectionModified = TRUE;
        }

        if (m_RenegadeServerLogin != m_ServerLogin)
        {
            SetSetting(SETTINGS_TARGET_COMP_ID_KEY, m_ServerLogin);
            m_bFixConnectionModified = TRUE;
        }

        if (m_RenegadeClientLogin != m_ClientLogin)
        {
            SetSetting(SETTINGS_SENDER_COMP_ID_KEY, m_ClientLogin);
            m_bFixConnectionModified = TRUE;
        }

        if (m_RenegadeReconnectTimeout != m_ReconnectTimeout)
        {
            SetSetting(SETTINGS_RECONNECT_TIME_KEY, m_ReconnectTimeout);
            m_bFixConnectionModified = TRUE;
        }

    	CDialog::OnOK();
    }
    catch (_com_error& e)
    {
        CString strErr;
        strErr.Format( _T("Error occured while set data to service ")
            _T("settings object. Error = 0x%.8X"), e.Error());
        AfxMessageBox(strErr);
    }
}

CString CSettingsDlg::GetShortStringFromFull(_bstr_t& bsFull )
{	
	_ConnectionPtr spConn;
	HRESULT hr = spConn.CreateInstance( __uuidof(Connection) );
    if ( FAILED(hr) )
	     utils::ThrowErrorNoSetErrorInfo(hr, L"Failed to create Connection object");

    spConn->ConnectionString = bsFull;
	PropertiesPtr spParams = spConn->Properties;
 		
	_bstr_t bs = spParams->Item[L"Data Source"]->Value;
	bs += L"\\";
	bs += (_bstr_t)spParams->Item[L"Initial Catalog"]->Value;

	return CString( (LPCTSTR)bs );
}
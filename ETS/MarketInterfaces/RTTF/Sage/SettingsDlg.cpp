// SettingsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "resource_GUI.h"
#include "SettingsDlg.h"
#include "SgConst.h"
#include ".\settingsdlg.h"

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
	, m_ConfigurationFile(_T(""))
{
	//{{AFX_DATA_INIT(CSettingsDlg)
	m_DBConnectionString = _T("");
	m_DBConnectionStringFull = _T("");
	//}}AFX_DATA_INIT
}


void CSettingsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSettingsDlg)
	DDX_Text(pDX, IDC_CONNECTION_STRING, m_DBConnectionString);
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_ED_CONFIGURATION, m_ConfigurationFile);
}

BEGIN_MESSAGE_MAP(CSettingsDlg, CDialog)
	//{{AFX_MSG_MAP(CSettingsDlg)
	ON_BN_CLICKED(IDC_SETUP, OnSetup)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BT_CONFIG, OnBnClickedBtConfig)
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

		CXMLParamsHelper XMLParams;
		XMLParams.LoadXMLParams();

		_bstr_t sbsUserGroup;
		XMLParams.GetUserGroup(sbsUserGroup.GetAddress());

		_bstr_t sbsValue;
		XMLParams.GetMainXMLString(SETTINGS_XML_KEY, sbsUserGroup, &sbsValue);
		m_ConfigurationFile = (LPTSTR)sbsValue;

		m_RenegadeConfigFile = m_ConfigurationFile;
    }
    catch(_com_error& e)
    {
        CString strErr;
        strErr.Format( _T("Error occured while get data from service settings object. Error = 0x%.8X"), e.Error());
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
		Oledb32::IDataSourceLocatorPtr spLocator;

		HRESULT hr = spLocator.CreateInstance (__uuidof (Oledb32::DataLinks));
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
            _ConnectionPtr  spNewConnection;
			spDispatch->QueryInterface(__uuidof(_Connection), (void**)&spNewConnection);

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

		if(m_RenegadeConfigFile != m_ConfigurationFile)
        {
			CXMLParamsHelper XMLParams;
			XMLParams.LoadXMLParams();

			_bstr_t sbsUserGroup;
			XMLParams.GetUserGroup(sbsUserGroup.GetAddress());
			XMLParams.SetMainXMLString(SETTINGS_XML_KEY, sbsUserGroup, (LPCTSTR)m_ConfigurationFile);

			XMLParams.SaveMainXMLParams();
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
void CSettingsDlg::OnBnClickedBtConfig()
{
	UpdateData(TRUE);

	char szFilename[MAX_PATH];
	::lstrcpy(szFilename, m_ConfigurationFile.GetString());
	OPENFILENAME ofn = {0};
	ofn.lStructSize = sizeof(ofn);
	ofn.lpstrFile = szFilename;
	ofn.nMaxFile = sizeof(szFilename);
	ofn.hwndOwner = GetSafeHwnd();
	ofn.Flags = OFN_FILEMUSTEXIST;
	if (::GetOpenFileName(&ofn)) 
		m_ConfigurationFile = szFilename;
	UpdateData(FALSE);

}



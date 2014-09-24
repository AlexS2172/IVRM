// DbConnectionDlg.cpp : implementation file
//

#include "stdafx.h"
#include "EtsFixManager.h"
#include "DbConnectionDlg.h"

// CDbConnectionDlg dialog

IMPLEMENT_DYNAMIC(CDbConnectionDlg, CDialog)
CDbConnectionDlg::CDbConnectionDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDbConnectionDlg::IDD, pParent),
	m_bServersLoaded(false)
{
}

CDbConnectionDlg::~CDbConnectionDlg()
{
}

void CDbConnectionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_COMBO_SERVER, m_cmbServer);
	DDX_Control(pDX, IDC_COMBO_DATABASE, m_cmbDatabase);
}


BEGIN_MESSAGE_MAP(CDbConnectionDlg, CDialog)
	ON_CBN_DROPDOWN(IDC_COMBO_SERVER, OnServerDropdown)
	ON_CBN_DROPDOWN(IDC_COMBO_DATABASE, OnDatabaseDropdown)
	ON_BN_CLICKED(IDC_CHECK_NT_SECURITY, OnNtSecurityClicked)
END_MESSAGE_MAP()


// CDbConnectionDlg message handlers

void CDbConnectionDlg::OnServerDropdown()
{
	if(!m_bServersLoaded)
	{
		_LoadServersList();
		m_bServersLoaded = true;
	}
}

void CDbConnectionDlg::_LoadServersList()
{
	SQLHENV hEnv = 0;
	SQLHDBC hDbc = 0;

	CWaitCursor wait;
	m_cmbServer.ResetContent();

	try
	{
		SQLRETURN rc;

		rc = ::SQLAllocEnv(&hEnv);
		if(rc == SQL_INVALID_HANDLE || rc == SQL_ERROR)
			throw 1L;

		rc = ::SQLAllocConnect(hEnv, &hDbc);
		if(rc == SQL_INVALID_HANDLE || rc == SQL_ERROR)
			throw 2L;

		SQLTCHAR tszQuery[] = _T("DRIVER={SQL Server};");
		SQLSMALLINT cbSize = 0;
		SQLTCHAR tszTemp[10];
		rc = ::SQLBrowseConnect(hDbc, tszQuery, SQL_NTS, tszTemp, sizeof(tszTemp), &cbSize);
		if(rc == SQL_INVALID_HANDLE || rc == SQL_ERROR || cbSize == 0)
			throw 3L;

		int iSize = cbSize * sizeof(SQLTCHAR);
		SQLTCHAR *pszBuffer = (SQLTCHAR*)_alloca(iSize);
		memset(pszBuffer, 0, iSize);

		rc = ::SQLBrowseConnect(hDbc, tszQuery, SQL_NTS, pszBuffer, iSize, &cbSize);
		if(rc == SQL_INVALID_HANDLE || rc == SQL_ERROR)
			throw 4L;

		static const TCHAR szServer[] = _T("Server={");
		LPTSTR pcszStart = _tcsstr((LPTSTR)pszBuffer, szServer);
		
		if(pcszStart)
		{
			pcszStart += sizeof(szServer) - 1;

			LPTSTR pcszEnd = _tcspbrk(pcszStart, _T(",}"));
			while(pcszEnd)
			{
				*pcszEnd = 0;
				
				m_cmbServer.AddString(pcszStart);

				pcszStart = pcszEnd + 1;
				pcszEnd = _tcspbrk(pcszStart, _T(",}"));
			}
		}
	}
	catch(long)
	{
	}

	if(hDbc)
		::SQLFreeConnect(hDbc);

	if(hEnv)
		::SQLFreeEnv(hEnv);

	m_cmbServer.SetWindowText(m_dbConnSettings.m_strServer);
}

void CDbConnectionDlg::OnDatabaseDropdown()
{
	_LoadDatabasesList();
}

void CDbConnectionDlg::_SaveSettings()
{
	m_dbConnSettings.m_strServer = _ControlText[IDC_COMBO_SERVER].Trim();
	m_dbConnSettings.m_bIntegratedSecurity = _ControlChecked[IDC_CHECK_NT_SECURITY];
	m_dbConnSettings.m_strUserName = _ControlText[IDC_EDIT_USERNAME].Trim();
	m_dbConnSettings.m_strPassword = _ControlText[IDC_EDIT_PASSWORD];
	m_dbConnSettings.m_strDatabase = _ControlText[IDC_COMBO_DATABASE].Trim();
	m_dbConnSettings.MakeConnectionString();
}

void CDbConnectionDlg::_LoadDatabasesList()
{
	m_cmbDatabase.ResetContent();

	try
	{
		CWaitCursor wait;

		CDbConnectionSettings dbConnSettings;
		dbConnSettings.m_strServer = _ControlText[IDC_COMBO_SERVER].Trim();
		dbConnSettings.m_bIntegratedSecurity = _ControlChecked[IDC_CHECK_NT_SECURITY];
		dbConnSettings.m_strUserName = _ControlText[IDC_EDIT_USERNAME];
		dbConnSettings.m_strPassword = _ControlText[IDC_EDIT_PASSWORD];
		dbConnSettings.m_strDatabase = _T("master");
		dbConnSettings.MakeConnectionString();

		_ConnectionPtr spConn;
		__CHECK_HRESULT(spConn.CreateInstance(CLSID_Connection), _T("Fail to create connection object."));

		spConn->ConnectionString = _bstr_t(dbConnSettings.m_strConnectionString);
		spConn->ConnectionTimeout = 120;
		spConn->Open(L"", L"", L"", adConnectUnspecified);

		_RecordsetPtr spRs;
		_variant_t vtRows = vtMissing;
		spRs = spConn->Execute(L"SELECT [name] FROM master.dbo.sysdatabases "
							L"WHERE [name] NOT IN ('master','tempdb','model','msdb','pubs')",
							&vtRows, adCmdText);

		while(!spRs->AdoEof)
		{
			m_cmbDatabase.AddString((LPCTSTR)_bstr_t(spRs->Fields->Item[0L]->Value));
			spRs->MoveNext();
		}
		spRs = NULL;
		spConn->Close();
	}
	catch(_com_error& e)
	{
		CString strError;
		strError.Format(_T("Connection failed: %s (%X)"), (LPCTSTR)e.Description() ? (LPCTSTR)e.Description() : e.ErrorMessage(), e.Error());
		MessageBox(strError, NULL, MB_ICONSTOP);
	}

	m_cmbDatabase.SetWindowText(m_dbConnSettings.m_strDatabase);
}

void CDbConnectionDlg::OnNtSecurityClicked()
{
	BOOL bEnabled = !_ControlChecked[IDC_CHECK_NT_SECURITY];

	_ControlEnabled[IDC_STATIC_USERNAME] = bEnabled;
	_ControlEnabled[IDC_EDIT_USERNAME] = bEnabled;

	_ControlEnabled[IDC_STATIC_PASSWORD] = bEnabled;
	_ControlEnabled[IDC_EDIT_PASSWORD] = bEnabled;
}

void CDbConnectionDlg::OnOK()
{
	_SaveSettings();
	CString strError;
	bool bValid;
	
	{
		CWaitCursor wait;
		bValid = m_dbConnSettings.CheckConnection(strError);
	}

	if(!bValid)
	{
		strError += _T(" Save changes?");
		int iResult = MessageBox(strError, NULL, MB_ICONSTOP | MB_YESNOCANCEL);

		if(iResult == IDCANCEL)
			return;

		if(iResult == IDNO)
		{
			CDialog::OnCancel();
			return;
		}
	}

	CDialog::OnOK();
}

BOOL CDbConnectionDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	_ControlText[IDC_COMBO_SERVER] = m_dbConnSettings.m_strServer;
	if(m_dbConnSettings.m_bAllowIntegratedSecurity)
	{
		_ControlEnabled[IDC_CHECK_NT_SECURITY] = TRUE;
		_ControlChecked[IDC_CHECK_NT_SECURITY] = m_dbConnSettings.m_bIntegratedSecurity;
		_ControlEnabled[IDC_EDIT_USERNAME] = !m_dbConnSettings.m_bIntegratedSecurity;
		_ControlEnabled[IDC_EDIT_PASSWORD] = !m_dbConnSettings.m_bIntegratedSecurity;
	}
	else
	{
		_ControlEnabled[IDC_CHECK_NT_SECURITY] = FALSE;
		_ControlChecked[IDC_CHECK_NT_SECURITY] = FALSE;
		_ControlEnabled[IDC_EDIT_USERNAME] = TRUE;
		_ControlEnabled[IDC_EDIT_PASSWORD] = TRUE;
	}

	_ControlText[IDC_EDIT_USERNAME] = m_dbConnSettings.m_strUserName;
	_ControlText[IDC_EDIT_PASSWORD] = m_dbConnSettings.m_strPassword;
	_ControlText[IDC_COMBO_DATABASE] = m_dbConnSettings.m_strDatabase;

	return TRUE;
}

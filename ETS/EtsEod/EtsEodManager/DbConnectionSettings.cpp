#include "StdAfx.h"
#include "DbConnectionSettings.h"

CDbConnectionSettings::CDbConnectionSettings(void)
	: m_bIntegratedSecurity(FALSE),
	m_bAllowIntegratedSecurity(TRUE)
{
}

CDbConnectionSettings::~CDbConnectionSettings(void)
{
}

void CDbConnectionSettings::ParseConnectionString()
{
	m_strServer.Empty();
	m_strUserName.Empty();
	m_strPassword.Empty();
	m_strDatabase.Empty();
	m_bIntegratedSecurity = FALSE;

	int nCurPos = 0, nValPos = 0;
	CString strValue;

	strValue = m_strConnectionString.Tokenize(_T(";"), nCurPos);
	while(nCurPos >= 0)
	{
		if(!strValue.Find(_T("Data Source")))
		{
			if((nValPos = strValue.Find(_T("="))) >= 0)
				m_strServer = strValue.Mid(nValPos + 1);
		}
		else if(!strValue.Find(_T("Initial Catalog")))
		{
			if((nValPos = strValue.Find(_T("="))) >= 0)
				m_strDatabase = strValue.Mid(nValPos + 1);
		}
		else if(!strValue.Find(_T("Integrated Security")))
		{
			if((nValPos = strValue.Find(_T("="))) >= 0)
				m_bIntegratedSecurity = m_bAllowIntegratedSecurity
								&& !strValue.Mid(nValPos + 1).Trim().CompareNoCase(_T("SSPI"));
		}
		else if(!strValue.Find(_T("User ID")))
		{
			if((nValPos = strValue.Find(_T("="))) >= 0)
				m_strUserName = strValue.Mid(nValPos + 1);
		}
		else if(!strValue.Find(_T("Password")))
		{
			if((nValPos = strValue.Find(_T("="))) >= 0)
				m_strPassword = strValue.Mid(nValPos + 1);
		}

		strValue = m_strConnectionString.Tokenize(_T(";"), nCurPos);
	}
}

void CDbConnectionSettings::MakeConnectionString()
{
	m_strConnectionString.Empty();
	m_strConnectionString = _T("Provider=SQLOLEDB.1");

	m_strConnectionString += _T(";Data Source=");
	m_strConnectionString += m_strServer;

	m_strConnectionString += _T(";Initial Catalog=");
	m_strConnectionString += m_strDatabase;

	if(!m_bIntegratedSecurity || !m_bAllowIntegratedSecurity)
	{
		m_strConnectionString += _T(";User ID=");
		m_strConnectionString += m_strUserName;

		m_strConnectionString += _T(";Password=");
		m_strConnectionString += m_strPassword;

		m_strConnectionString += _T(";Persist Security Info=True");
	}
	else
		m_strConnectionString += _T(";Integrated Security=SSPI;Persist Security Info=False");
}

bool CDbConnectionSettings::CheckConnection(CString& strError)
{
	strError.Empty();
	try
	{
		_ConnectionPtr spConn;
		__CHECK_HRESULT(spConn.CreateInstance(CLSID_Connection), _T("Fail to create connection object."));

		spConn->ConnectionString = _bstr_t(m_strConnectionString);
		spConn->ConnectionTimeout = 120;
		spConn->Open(L"", L"", L"", adConnectUnspecified);
		spConn->Close();
	}
	catch(_com_error& e)
	{
		strError.Format(_T("Connection failed: %s (%X)."), (LPCTSTR)e.Description() ? (LPCTSTR)e.Description() : e.ErrorMessage(), e.Error());
		return false;
	}
	return true;
}

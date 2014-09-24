#pragma once

class CDbConnectionSettings
{
public:
	CDbConnectionSettings(void);
	virtual ~CDbConnectionSettings(void);

	void ParseConnectionString();
	void MakeConnectionString();

	bool CheckConnection(CString& strError);

	CString GetDbPath() const
	{
		CString strRet;
		if(m_strServer.GetLength() > 0)
			strRet = m_strServer;

		if(m_strDatabase.GetLength() > 0)
		{
			strRet += _T("\\");
			strRet += m_strDatabase;
		}
		return strRet;
	}

	CDbConnectionSettings& operator = (CDbConnectionSettings& rhs)
	{
		m_strConnectionString = rhs.m_strConnectionString;
		m_strServer = rhs.m_strServer;
		m_strUserName = rhs.m_strUserName;
		m_strPassword = rhs.m_strPassword;
		m_strDatabase = rhs.m_strDatabase;
		m_bIntegratedSecurity = rhs.m_bIntegratedSecurity;
		m_bAllowIntegratedSecurity = rhs.m_bAllowIntegratedSecurity;
		return *this;
	}

	bool operator != (CDbConnectionSettings& rhs)
	{
		if(m_strConnectionString.Compare(rhs.m_strConnectionString))
			return true;

		if(m_strServer.Compare(rhs.m_strServer))
			return true;

		if(m_strUserName.Compare(rhs.m_strUserName))
			return true;

		if(m_strPassword.Compare(rhs.m_strPassword))
			return true;

		if(m_strDatabase.Compare(rhs.m_strDatabase))
			return true;

		return (m_bIntegratedSecurity != rhs.m_bIntegratedSecurity);
	}

public:
	CString		m_strConnectionString;
	CString		m_strServer;
	CString		m_strUserName;
	CString		m_strPassword;
	CString		m_strDatabase;
	BOOL		m_bIntegratedSecurity;

	BOOL		m_bAllowIntegratedSecurity;
};

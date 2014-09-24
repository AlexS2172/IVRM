#pragma once

class CGenegalSettings
{
private:
	static	_bstr_t	m_DBConnectionString;
public:
	static	_bstr_t	GetDBConnectionString()
	{
		return m_DBConnectionString;
	};

	static	void SetConnectionString(_bstr_t ConnectionString)
	{
		m_DBConnectionString = ConnectionString;
	};
};

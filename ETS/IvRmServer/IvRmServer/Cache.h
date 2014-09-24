#pragma once

class CCache
{
public:
	CCache(void);
	~CCache(void);

private:
	CComBSTR				m_bstrDBConnectionString;
	CDBConnection			m_DbConnection;
	//**********************************************//
	IEtsMain*				m_pMain;

public:
	bool					SetConnectionString(BSTR bsConnectionString);
	bool					InitializeDB();
	CDBConnection&			GetDBConnection();
	//**********************************************//
	bool					SetMain(IEtsMainPtr spMain);
	IEtsMainPtr				GetMain();

	virtual			void	Clear();
};

#pragma once


[
	uuid(8054C968-931A-4503-9C75-9DCFF94F94AF),
	export
]
struct ConnectionSettings
{
	[helpstring("UserID (3 or 4 characters)")]	BSTR ID;
	[helpstring("IPPrimary")]					BSTR PrimaryIP;
	[helpstring("IPSecondary")]					BSTR SecondaryIP;
	[helpstring("IPPort")]						LONG Port;
	[helpstring("UserPassword (10 characters)")]BSTR Password;
};

class CSettings
{
public:
	CSettings(void);
	virtual ~CSettings(void);
	bool CreateFrom(const ConnectionSettings* pSettings, _bstr_t* pStrOut);

	_bstr_t GetID(){return m_bsID;};
	_bstr_t GetPrimaryIP(){return m_bsPrimaryIP;};
	_bstr_t GetSecondaryIP(){return m_bsSecondaryIP;};
	_bstr_t GetPassword(){return m_bsPassword;};
	LONG GetPort(){return m_lPort;};
	bool operator != (const ConnectionSettings*  pSettings) const ;
	
protected:
	_bstr_t m_bsID;
	_bstr_t m_bsPrimaryIP;
	_bstr_t m_bsSecondaryIP;
	_bstr_t m_bsPassword;
	LONG	m_lPort;

};

#ifndef ISE_SETTINGS_MANAGER_H
#define ISE_SETTINGS_MANAGER_H

#define ISE_CONNECTOR_KEY		L"Software\\Egar\\Common\\Connectivity\\ISE Connector"
#define GATEWAYS_KEY			L"Gateways"
#define BINS_KEY				L"Bins"
#define USER_KEY				L"User"
#define PASSWORD_KEY			L"Password"
#define ADDRESS_KEY				L"Addr"
#define PORT_KEY				L"Port"
#define WORK_LOGON_TIME_KEY		L"Logon Time"
#define WORK_LOGOFF_TIME_KEY	L"Logoff Time"

struct CGateway
{
	string  m_sAddr;
	string	m_sPort;
};

struct CAccount
{
	string	m_sUser;
	string	m_sPassword;
};

class CISESettingManager
{
	DWORD ReadValue(const HKEY hKey, const _bstr_t& bsKey, const _bstr_t& bsValueName, _bstr_t& bsValue);
	DWORD ReadValue(const _bstr_t& bsKey, const _bstr_t& bsValueName, _bstr_t& bsValue);
	DWORD EnumSubkeys(const HKEY hKey, const _bstr_t& bsKey, set<_bstr_t>& bsKeys);
	DWORD EnumSubkeys(const _bstr_t& bsKey, set<_bstr_t>& bsKeys);

public:

	DWORD GetGateways(vector<CGateway>& vecGateways);
	DWORD GetAccounts(map<int, vector<CAccount> >& mapBinToAccounts);

	DWORD GetWorkTime(PLARGE_INTEGER pliLogon, PLARGE_INTEGER pliLogoff);

private:

	DWORD ReadTimeValue(
		const _bstr_t& bsKey, 
		const _bstr_t& bsValueName, 
		int* pnHour, 
		int* pnMinute,
		bool* pbDefault);
};

#endif
#include "stdafx.h"

#include "ISESettingsManager.h"

DWORD CISESettingManager::ReadValue(const HKEY hKey, const _bstr_t& bsKey, const _bstr_t& bsValueName, _bstr_t& bsValue)
{
	eg_reg::CEgRegKey Key;

	DWORD dwErr = Key.Open(hKey, bsKey, KEY_READ);
	if(dwErr == 0)
		dwErr = Key.QueryValue(bsValue, bsValueName);

	return dwErr;
}

DWORD CISESettingManager::ReadValue(const _bstr_t& bsKey, const _bstr_t& bsValueName, _bstr_t& bsValue)
{
	DWORD dwErr = ReadValue(HKEY_CURRENT_USER, bsKey, bsValueName, bsValue);
	if(dwErr)
		dwErr = ReadValue(HKEY_LOCAL_MACHINE, bsKey, bsValueName, bsValue);
	
	return dwErr;
}

DWORD CISESettingManager::EnumSubkeys(const HKEY hKey, const _bstr_t& bsKey, set<_bstr_t>& bsKeys)
{
	eg_reg::CEgRegKey Key;

	DWORD dwErr = Key.Open(hKey, bsKey, KEY_ENUMERATE_SUB_KEYS);
	if(dwErr == 0)
	{
		int iIndex = 0;
		while(true)
		{
			char szBuf[256];
			DWORD dwLen = sizeof(szBuf);

			dwErr = RegEnumKey(Key, iIndex++, szBuf, dwLen);
			if(dwErr)
				break;

			_bstr_t bsSubkey = szBuf;
			bsKeys.insert(bsSubkey);
		}

		if(ERROR_NO_MORE_ITEMS == dwErr)
			dwErr = 0;
	}

	return dwErr;
}

DWORD CISESettingManager::EnumSubkeys(const _bstr_t& bsKey, set<_bstr_t>& bsKeys)
{
	bsKeys.clear();

	DWORD dwErr = EnumSubkeys(HKEY_CURRENT_USER, bsKey, bsKeys);
	if(dwErr)
		dwErr = EnumSubkeys(HKEY_LOCAL_MACHINE, bsKey, bsKeys);

	return dwErr;
}

DWORD CISESettingManager::GetGateways(vector<CGateway>& vecGateways)
{
	vecGateways.clear();

	set<_bstr_t> setGws;

	DWORD dwErr = EnumSubkeys(_bstr_t(ISE_CONNECTOR_KEY) + "\\" + GATEWAYS_KEY, setGws);
	if(dwErr)
		return dwErr;

	for(set<_bstr_t>::iterator itGw = setGws.begin(); itGw != setGws.end(); itGw++)
	{
		const _bstr_t bsGw = _bstr_t(ISE_CONNECTOR_KEY) + "\\" + GATEWAYS_KEY + "\\" + *itGw;

		_bstr_t bsAddr;
		_bstr_t bsPort;

		if(ReadValue(bsGw, ADDRESS_KEY, bsAddr) == 0 && ReadValue(bsGw, PORT_KEY, bsPort) == 0
			&& bsAddr.length() && bsPort.length())
		{
			CGateway Gateway;
			Gateway.m_sAddr = bsAddr;
			Gateway.m_sPort = bsPort;

			vecGateways.push_back(Gateway);
		}
	}


	return dwErr;
};
	
DWORD CISESettingManager::GetAccounts(map<int, vector<CAccount> >& mapBinToAccounts)
{
	mapBinToAccounts.clear();
	
	set<_bstr_t> setBins;

	DWORD dwErr = EnumSubkeys(_bstr_t(ISE_CONNECTOR_KEY) + "\\" + BINS_KEY, setBins);
	if(dwErr)
		return dwErr;

	for(set<_bstr_t>::iterator itBin = setBins.begin(); itBin != setBins.end(); itBin++)
	{
		set<_bstr_t> setAccounts;

		DWORD dwErr = EnumSubkeys(_bstr_t(ISE_CONNECTOR_KEY) + "\\" + BINS_KEY + "\\" +  *itBin, setAccounts);
		if(dwErr)
			return dwErr;

		for(set<_bstr_t>::iterator itAccount = setAccounts.begin(); itAccount != setAccounts.end(); itAccount++)
		{
			_bstr_t bsUser;
			_bstr_t bsPassword;

			const _bstr_t bsAccount = _bstr_t(ISE_CONNECTOR_KEY) + "\\" + BINS_KEY + "\\" +  *itBin + "\\" + *itAccount;

			if(ReadValue(bsAccount, USER_KEY, bsUser) == 0 && ReadValue(bsAccount, PASSWORD_KEY, bsPassword) == 0
				&& bsUser.length() && bsPassword.length())
			{
				CAccount Account;
				Account.m_sUser = bsUser;
				Account.m_sPassword = bsPassword;

				vector<CAccount>& vecAccounts = mapBinToAccounts[atoi(*itBin)];
				vecAccounts.push_back(Account);
			}
		}
	}

	return dwErr;
};

//--------------------------------------------------------------------------------------------------------------//
DWORD CISESettingManager::GetWorkTime(PLARGE_INTEGER pliLogon, PLARGE_INTEGER pliLogoff)
{
	int wHour[2];
	int wMinute[2];
	bool bDefault[2];
	DWORD dwResult;

	dwResult = ReadTimeValue(ISE_CONNECTOR_KEY, WORK_LOGON_TIME_KEY, &wHour[0], &wMinute[0], &bDefault[0]);
	if(ERROR_SUCCESS != dwResult) return dwResult;
	
	dwResult = ReadTimeValue(ISE_CONNECTOR_KEY, WORK_LOGOFF_TIME_KEY, &wHour[1], &wMinute[1], &bDefault[1]);
	if(ERROR_SUCCESS != dwResult) return dwResult;

	if (bDefault[0] != bDefault[1]) return ERROR_INVALID_DATA;

	if(true == bDefault[0])
	{
		// If values are not exists or they are have zero values, then using standart NY work time
		// 08:00 NY is 13:00 GMT
		// 18:00 NY is 23:00 GMT

		TIME_ZONE_INFORMATION tzi;
		if(TIME_ZONE_ID_INVALID == ::GetTimeZoneInformation(&tzi)) return ::GetLastError();

		SYSTEMTIME stU;
		::GetSystemTime(&stU);

		SYSTEMTIME stL;

		// Convert logon time
		stU.wHour = 13; 
		if(FALSE == ::SystemTimeToTzSpecificLocalTime(&tzi, &stU, &stL)) return ::GetLastError();
		wHour[0] = stL.wHour;

		// Convert logoff time
		stU.wHour = 23; 
		if(FALSE == ::SystemTimeToTzSpecificLocalTime(&tzi, &stU, &stL)) return ::GetLastError();
		wHour[1] = stL.wHour;
	}

	pliLogon->QuadPart = (LONGLONG)(wHour[0] * 60 + wMinute[0]) * 60 * 1000 * 10000;
	pliLogoff->QuadPart = (LONGLONG)(wHour[1] * 60 + wMinute[1]) * 60 * 1000 * 10000;
	
	return dwResult;
}

//--------------------------------------------------------------------------------------------------------------//
DWORD CISESettingManager::ReadTimeValue(
	const _bstr_t& bsKey, 
	const _bstr_t& bsValueName, 
	int* pnHour, 
	int* pnMinute,
	bool* pbDefault)
{
	_bstr_t bsTime;

	DWORD dwResult = ERROR_SUCCESS;

	*pnHour = 0;
	*pnMinute = 0;
	*pbDefault = false;
	
	dwResult = ReadValue(bsKey, bsValueName, bsTime);

	if(ERROR_SUCCESS != dwResult || 0 == bsTime.length())
	{
		*pbDefault = true;
		return ERROR_SUCCESS;
	}

	int nResult;
	TCHAR c;
	nResult = _stscanf(bsTime, _T("%d:%d %c"), pnHour, pnMinute, &c);
	if(3 == nResult)
	{
		c = (TCHAR)CharUpper((PTSTR)c);
		if (_T('P') == c)
		{
			if(12 == *pnHour) *pnHour = 0; else *pnHour += 12;
		}
		else if (_T('A') != c) return ERROR_INVALID_DATA;
	}
	
	if(nResult < 2 || *pnHour > 23 || *pnMinute > 59) return ERROR_INVALID_DATA;
	
	return ERROR_SUCCESS;
}
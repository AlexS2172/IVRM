// Settings.cpp : Implementation of CSettings
#include "stdafx.h"
#include "ETSManager.h"
#include "Settings.h"

/////////////////////////////////////////////////////////////////////////////
// CSettings

STDMETHODIMP CSettings::InitForProfile(BSTR bsProfileName)
{
	m_dwProductKey = 0L;

	LoadDatabaseForProfile(bsProfileName);	//	Load for profile
	LoadProvider(false);					//	Load from HKLM
	LoadVolatilitySource(false);			//	Load from HKLM

	return S_OK;
}

STDMETHODIMP CSettings::get_DatabaseConnection(BSTR *pVal)
{
	if(!pVal)
		return E_POINTER;

	ObjectLock lock(this);

	if(*pVal)
	{
		::SysFreeString(*pVal);
	}

	*pVal = m_bsConnectionString.copy();
	return S_OK;
}

STDMETHODIMP CSettings::put_DatabaseConnection(BSTR newVal)
{
	ObjectLock lock(this);
	_bstr_t bsNew = newVal;
	if(bsNew.length() && bsNew != m_bsConnectionString)
	{
		m_bsConnectionString = newVal;
		if(SaveDatabase())
			Fire_DatabaseConnectionChanged(m_bsConnectionString);
		m_bRequiresSave = true;
	}
	return S_OK;
}

STDMETHODIMP CSettings::get_PriceProvider(long *pVal)
{
	if(!pVal)
		return E_POINTER;
	ObjectLock lock(this);
	*pVal = m_lProviderType;
	return S_OK;
}

STDMETHODIMP CSettings::put_PriceProvider(long newVal)
{
	ObjectLock lock(this);
	if(newVal>=-1 && newVal!=m_lProviderType)
	{
		m_lProviderType = newVal;
		if(SaveProvider())
			Fire_PriceProviderChanged(m_lProviderType);		
		m_bRequiresSave = true;
	}
	return S_OK;
}

STDMETHODIMP CSettings::get_VolatilitySource(long *pVal)
{
	if(!pVal)
		return E_POINTER;
	ObjectLock lock(this);
	*pVal = m_nVolatilitySourceType;
	return S_OK;
}

STDMETHODIMP CSettings::put_VolatilitySource(long newVal)
{
	ObjectLock lock(this);
	if(newVal >= -1 && newVal != m_nVolatilitySourceType)
	{
		m_nVolatilitySourceType = newVal;
		if(SaveVolatilitySource())
			Fire_VolatilitySourceChanged(m_nVolatilitySourceType);		
		m_bRequiresSave = true;
	}
	return S_OK;
}

/*
STDMETHODIMP CSettings::GetDBConnectionString(LONG lType, BSTR bsName, BSTR *pVal)
{
if(!pVal)
return E_POINTER;

ObjectLock lock(this);

if(*pVal)
{
::SysFreeString(*pVal);
}

//*pVal = m_bsConnectionString.copy();
return S_OK;

}
*/

STDMETHODIMP CSettings::UpdateForProfile(BSTR bstrProfileName)
{
	HRESULT hr = S_OK;
	/*long lUNLen = MAX_BUF;
	long lGotBytes = lUNLen;
	long lDBNameLen = MAX_BUF;
	HKEY hKey;
	HKEY hSubKey;
	LPTSTR lpPRODUCT_REG_KEY = _T("SOFTWARE\\Egar\\ETS\\Asp");
	LPTSTR lpDBKEY = _T("Database");

	TCHAR lpProfileGroup[MAX_BUF];

	ZeroMemory(lpProfileGroup, sizeof(lpProfileGroup));

	_bstr_t bsProfileName(L"");
	if(bstrProfileName)
		bsProfileName= bstrProfileName;

	if (bsProfileName.length() < 1) // Profile will be set as current user profile
	{
		TCHAR lpProfileName[MAX_BUF];
		if (!::GetUserName(lpProfileName, (DWORD *)&lUNLen))
			return E_FAIL;				
		bsProfileName = lpProfileName;
	}

	if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, lpPRODUCT_REG_KEY,  0, KEY_READ , &hKey) == ERROR_SUCCESS)
	{
		if (RegQueryValueEx(hKey, (LPCTSTR)bsProfileName, NULL, NULL, (LPBYTE)lpProfileGroup, (LPDWORD)&lGotBytes) == ERROR_SUCCESS)
		{
			if (RegOpenKeyEx(hKey, lpDBKEY,  0,  KEY_ALL_ACCESS , &hSubKey) == ERROR_SUCCESS)
			{
				if (RegSetValueEx(hSubKey, lpProfileGroup, NULL, REG_SZ, (LPBYTE)((LPCTSTR)m_bsConnectionString), (DWORD)(m_bsConnectionString.length() + 1)) == ERROR_SUCCESS)
				{
					hr = S_OK;
				}

				RegCloseKey(hSubKey);  
			}
		}

		RegCloseKey(hKey);  
	}*/
	return hr;
}

// IVTrace.cpp: implementation of the CIVTrace class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "IVTrace.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CIVTrace::CIVTrace()
{
	m_ulDaysInHistory = 1;
	m_ulMinLogLevel = enlogAll;
}

CIVTrace::~CIVTrace()
{

}


bool WINAPI CIVTrace::Init()
{
	bool bRes = EgStd::CEgTraceManager.Init();

	if (!bRes)
		return bRes;

	EgStd::CAutoLock Lock(&m_ObjectLock);

	// We have to override the values
	m_ulDaysInHistory = 1;
	m_ulMinLogLevel = enlogAll; //enlogFaults;

/*
	long lUNLen = 256;
	long lGotBytes = lUNLen;
	long lRegDebugLevel = 0;
	HKEY hKey;
	HKEY hSubKey;
	LPTSTR lpPRODUCT_REG_KEY = "SOFTWARE\\Egar\\ETS\\Asp";
	LPTSTR lpDebugLevelKEY = "DebugLevel";
	LPTSTR lpUserName = (LPTSTR)malloc(lUNLen);
	LPTSTR lpUserGroup = (LPTSTR)malloc(lUNLen);
	ZeroMemory(lpUserName, sizeof(lpUserName));
	ZeroMemory(lpUserGroup, sizeof(lpUserGroup));
	if (::GetUserName(lpUserName, (DWORD *)&lUNLen))
	{

		if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, lpPRODUCT_REG_KEY,  0, KEY_READ , &hKey) == ERROR_SUCCESS)
		{
			if (RegQueryValueEx(hKey, lpUserName, NULL, NULL, (LPBYTE)lpUserGroup, (LPDWORD)&lGotBytes) == ERROR_SUCCESS)
			{
				if (RegOpenKeyEx(hKey, lpDebugLevelKEY,  0,  KEY_READ , &hSubKey) == ERROR_SUCCESS)
				{
					if (RegQueryValueEx(hSubKey, lpUserGroup, NULL, NULL, (LPBYTE)&lRegDebugLevel, (LPDWORD)&lGotBytes) == ERROR_SUCCESS)
						m_ulMinLogLevel =lRegDebugLevel;
				RegCloseKey(hSubKey);  
				}
			}
			RegCloseKey(hKey);  
		}
	}
	free(lpUserName);
	free(lpUserGroup);

*/		
	
	return true;
}

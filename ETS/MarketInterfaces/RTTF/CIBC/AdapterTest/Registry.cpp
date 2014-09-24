
#include "stdafx.h"

LONG GetRegistrySetting(LPCTSTR lpszValue, CString& Value)
{
   	CEgRegKey key;
	LONG lResult = key.Open(HKEY_LOCAL_MACHINE, SETTINGS_REGISTRY_KEY, KEY_READ);
    if (lResult == ERROR_SUCCESS)
    {
       	lResult = key.QueryValue(Value, lpszValue);
        key.Close();
    }
 
    return lResult;
}

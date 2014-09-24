#ifndef __APPREG_H__
#define __APPREG_H__

#define DECLARE_APPREG()                                                        \
    void SetRegistryKey(LPCTSTR lpszRegistryKey);                               \
    HKEY GetAppRegistryKey();                                                   \
    HKEY GetSectionKey(LPCTSTR lpszSection);                                    \
    UINT GetProfileInt(LPCTSTR lpszSection, LPCTSTR lpszEntry, int nDefault);   \
    CString GetProfileString(LPCTSTR lpszSection, LPCTSTR lpszEntry, LPCTSTR lpszDefault);  \
    BOOL GetProfileBinary(LPCTSTR lpszSection, LPCTSTR lpszEntry, BYTE** ppData, UINT* pBytes);\

#define IMPLEMENT_APPREG(Class, Base)                                           \
    void Class::SetRegistryKey(LPCTSTR lpszRegistryKey)                         \
    {                                                                           \
        if (m_pszRegistryKey)                                                   \
        {                                                                       \
            free((void*)m_pszRegistryKey);                                      \
            m_pszRegistryKey = 0;                                               \
        }                                                                       \
                                                                                \
        if (m_pszProfileName)                                                   \
        {                                                                       \
            free((void*)m_pszProfileName);                                      \
            m_pszProfileName = 0;                                               \
        }                                                                       \
                                                                                \
	    ASSERT(lpszRegistryKey != NULL);                                        \
	    ASSERT(m_pszAppName != NULL);                                           \
                                                                                \
	    BOOL bEnable = AfxEnableMemoryTracking(FALSE);                          \
	    m_pszRegistryKey = _tcsdup(lpszRegistryKey);                            \
        m_pszProfileName = _tcsdup(m_pszAppName);                               \
                                                                                \
	    AfxEnableMemoryTracking(bEnable);                                       \
    }                                                                           \
                                                                                \
    HKEY Class::GetAppRegistryKey()                                             \
    {                                                                           \
	    ASSERT(m_pszRegistryKey != NULL);                                       \
	    ASSERT(m_pszProfileName != NULL);                                       \
                                                                                \
	    HKEY hAppKey = NULL;                                                    \
	    HKEY hSoftKey = NULL;                                                   \
	    HKEY hCompanyKey = NULL;                                                \
	    if (RegOpenKeyEx(HKEY_CURRENT_USER, _T("software"), 0, KEY_WRITE|KEY_READ,\
		    &hSoftKey) == ERROR_SUCCESS ||                                      \
    	    RegOpenKeyEx(HKEY_LOCAL_MACHINE, _T("software"), 0, KEY_WRITE|KEY_READ,\
	    	&hSoftKey) == ERROR_SUCCESS)                                        \
	    {                                                                       \
		    DWORD dw;                                                           \
		    if (RegCreateKeyEx(hSoftKey, m_pszRegistryKey, 0, REG_NONE,         \
			    REG_OPTION_NON_VOLATILE, KEY_WRITE|KEY_READ, NULL,              \
			    &hCompanyKey, &dw) == ERROR_SUCCESS)                            \
		    {                                                                   \
			    RegCreateKeyEx(hCompanyKey, m_pszProfileName, 0, REG_NONE,      \
    				REG_OPTION_NON_VOLATILE, KEY_WRITE|KEY_READ, NULL,          \
	    			&hAppKey, &dw);                                             \
		    }                                                                   \
	    }                                                                       \
	    if (hSoftKey != NULL)                                                   \
		    RegCloseKey(hSoftKey);                                              \
	    if (hCompanyKey != NULL)                                                \
		    RegCloseKey(hCompanyKey);                                           \
                                                                                \
	    return hAppKey;                                                         \
    }                                                                           \
                                                                                \
    HKEY Class::GetSectionKey(LPCTSTR lpszSection)                              \
    {                                                                           \
	    ASSERT(lpszSection != NULL);                                            \
                                                                                \
    	HKEY hSectionKey = NULL;                                                \
	    HKEY hAppKey = GetAppRegistryKey();                                     \
	    if (hAppKey == NULL)                                                    \
		    return NULL;                                                        \
                                                                                \
	    DWORD dw;                                                               \
	    RegCreateKeyEx(hAppKey, lpszSection, 0, REG_NONE,                       \
		    REG_OPTION_NON_VOLATILE, KEY_WRITE|KEY_READ, NULL,                  \
		    &hSectionKey, &dw);                                                 \
	    RegCloseKey(hAppKey);                                                   \
	    return hSectionKey;                                                     \
    }                                                                           \
                                                                                \
    UINT Class::GetProfileInt(LPCTSTR lpszSection, LPCTSTR lpszEntry,           \
	    int nDefault)                                                           \
    {                                                                           \
	    ASSERT(lpszSection != NULL);                                            \
	    ASSERT(lpszEntry != NULL);                                              \
	    if (m_pszRegistryKey != NULL)                                           \
	    {                                                                       \
		    HKEY hSecKey = GetSectionKey(lpszSection);                          \
		    if (hSecKey == NULL)                                                \
    			return nDefault;                                                \
	    	DWORD dwValue;                                                      \
    		DWORD dwType;                                                       \
	    	DWORD dwCount = sizeof(DWORD);                                      \
		    LONG lResult = RegQueryValueEx(hSecKey, (LPTSTR)lpszEntry, NULL, &dwType,\
			    (LPBYTE)&dwValue, &dwCount);                                    \
		    RegCloseKey(hSecKey);                                               \
		    if (lResult == ERROR_SUCCESS)                                       \
		    {                                                                   \
			    ASSERT(dwType == REG_DWORD);                                    \
			    ASSERT(dwCount == sizeof(dwValue));                             \
			    return (UINT)dwValue;                                           \
		    }                                                                   \
		    return nDefault;                                                    \
	    }                                                                       \
	    else                                                                    \
	    {                                                                       \
		    ASSERT(m_pszProfileName != NULL);                                   \
		        return ::GetPrivateProfileInt(lpszSection, lpszEntry, nDefault, \
			        m_pszProfileName);                                          \
	    }                                                                       \
    }                                                                           \
                                                                                \
    CString Class::GetProfileString(LPCTSTR lpszSection, LPCTSTR lpszEntry,     \
	    LPCTSTR lpszDefault)                                                    \
    {                                                                           \
	    ASSERT(lpszSection != NULL);                                            \
	    ASSERT(lpszEntry != NULL);                                              \
	    if (m_pszRegistryKey != NULL)                                           \
	    {                                                                       \
		    HKEY hSecKey = GetSectionKey(lpszSection);                          \
		    if (hSecKey == NULL)                                                \
			    return lpszDefault;                                             \
		    CString strValue;                                                   \
		    DWORD dwType, dwCount;                                              \
		    LONG lResult = RegQueryValueEx(hSecKey, (LPTSTR)lpszEntry, NULL, &dwType,\
			    NULL, &dwCount);                                                \
		    if (lResult == ERROR_SUCCESS)                                       \
		    {                                                                   \
			    ASSERT(dwType == REG_SZ);                                       \
			    lResult = RegQueryValueEx(hSecKey, (LPTSTR)lpszEntry, NULL, &dwType,\
    				(LPBYTE)strValue.GetBuffer(dwCount/sizeof(TCHAR)), &dwCount);\
	    		strValue.ReleaseBuffer();                                       \
    		}                                                                   \
	    	RegCloseKey(hSecKey);                                               \
		    if (lResult == ERROR_SUCCESS)                                       \
		    {                                                                   \
			    ASSERT(dwType == REG_SZ);                                       \
		    	    return strValue;                                            \
		    }                                                                   \
		    return lpszDefault;                                                 \
	    }                                                                       \
	    else                                                                    \
	    {                                                                       \
		    ASSERT(m_pszProfileName != NULL);                                   \
                                                                                \
	    	if (lpszDefault == NULL)                                            \
			    lpszDefault = NULL;                                        \
		    TCHAR szT[4096];                                                    \
		    DWORD dw = ::GetPrivateProfileString(lpszSection, lpszEntry,        \
    			lpszDefault, szT, sizeof(szT)/sizeof(*szT), m_pszProfileName);  \
		    ASSERT(dw < 4095);                                                  \
		    return szT;                                                         \
	    }                                                                       \
    }                                                                           \
                                                                                \
    BOOL Class::GetProfileBinary(LPCTSTR lpszSection, LPCTSTR lpszEntry,        \
	    BYTE** ppData, UINT* pBytes)                                            \
    {                                                                           \
	    ASSERT(lpszSection != NULL);                                            \
	    ASSERT(lpszEntry != NULL);                                              \
	    ASSERT(ppData != NULL);                                                 \
	    ASSERT(pBytes != NULL);                                                 \
	    *ppData = NULL;                                                         \
	    *pBytes = 0;                                                            \
    	if (m_pszRegistryKey != NULL)                                           \
	    {                                                                       \
		    HKEY hSecKey = GetSectionKey(lpszSection);                          \
		    if (hSecKey == NULL)                                                \
			    return FALSE;                                                   \
                                                                                \
    		DWORD dwType, dwCount;                                              \
	    	LONG lResult = RegQueryValueEx(hSecKey, (LPTSTR)lpszEntry, NULL, &dwType,\
		    	NULL, &dwCount);                                                \
		    *pBytes = dwCount;                                                  \
		    if (lResult == ERROR_SUCCESS)                                       \
		    {                                                                   \
			    ASSERT(dwType == REG_BINARY);                                   \
			    *ppData = new BYTE[*pBytes];                                    \
			    lResult = RegQueryValueEx(hSecKey, (LPTSTR)lpszEntry, NULL, &dwType,\
    				*ppData, &dwCount);                                         \
	    	}                                                                   \
		    RegCloseKey(hSecKey);                                               \
		    if (lResult == ERROR_SUCCESS)                                       \
		    {                                                                   \
			    ASSERT(dwType == REG_BINARY);                                   \
			    return TRUE;                                                    \
		    }                                                                   \
		    else                                                                \
		    {                                                                   \
    			delete [] *ppData;                                              \
	    		*ppData = NULL;                                                 \
		    }                                                                   \
		    return FALSE;                                                       \
	    }                                                                       \
	    else                                                                    \
	    {                                                                       \
    		ASSERT(m_pszProfileName != NULL);                                   \
                                                                                \
	    	CString str = GetProfileString(lpszSection, lpszEntry, NULL);       \
		    if (str.IsEmpty())                                                  \
			    return FALSE;                                                   \
    		ASSERT(str.GetLength()%2 == 0);                                     \
	    	int nLen = str.GetLength();                                         \
		    *pBytes = nLen/2;                                                   \
    		*ppData = new BYTE[*pBytes];                                        \
	    	for (int i=0;i<nLen;i+=2)                                           \
		    {                                                                   \
			    (*ppData)[i/2] = (BYTE)                                         \
    				(((str[i+1] - 'A') << 4) + (str[i] - 'A'));                 \
	    	}                                                                   \
		    return TRUE;                                                        \
	    }                                                                       \
    }                                                                           \

#endif
/////////////////////////////////////////////////////////////////////////////

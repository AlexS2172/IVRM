//**************************************************************************************************************//
// abstract:	interface & implementation of 
//
// content:		CRegValue
//				CRegValuesMap
//				CRegKeyNamesSet
//
//				RegEnumerateValues				
//				RegWriteValues
//				RegEnumerateKeyNames
//
//**************************************************************************************************************//
#pragma once
//**************************************************************************************************************//
// logic guardians
//**************************************************************************************************************//
#if !defined(__EGLIBDBG_H__)
	#error 'EgLibReg.h requires EgLibDbg.h to be included first'
#endif

//**************************************************************************************************************//
// header identifier
//**************************************************************************************************************//
#define __EGLIBREG_H__

//**************************************************************************************************************//
// includes
//**************************************************************************************************************//
#include <set>
#include <map>

//**************************************************************************************************************//
// namespace EgLib
//**************************************************************************************************************//
namespace EgLib {

//**************************************************************************************************************//
// class CRegValue
//**************************************************************************************************************//
class CRegValue
{
public:

// Data members

	DWORD  m_dwType;
	PBYTE  m_pData;
	size_t m_cbData;

// Constructor/Destructor

	CRegValue()
		:	m_dwType(REG_NONE),
			m_pData(NULL),
			m_cbData(0)
	{
	}

	CRegValue(DWORD dwType, const PBYTE pData, DWORD cbData)
	{
		EGLIBASSERT(NULL != pData);
		m_dwType = dwType;
		m_cbData = cbData;
		m_pData = new BYTE[m_cbData];
		::memcpy(m_pData, pData, m_cbData);
	}

	CRegValue(const CRegValue& rhs)
		:	m_dwType(REG_NONE),
			m_pData(NULL),
			m_cbData(0)
	{
		Copy(rhs);
	}

	~CRegValue()
	{
		Dispose();
	}

// Operators

	CRegValue& operator=(const CRegValue& rhs)
	{
		Copy(rhs);
		return *this;
	}

// Attributes

	void SetBinary(const PBYTE pData, DWORD cbData)
	{
		EGLIBASSERT(NULL != pData);
		EGLIBASSERT(0 != cbData);

		delete [] m_pData;
		m_cbData = cbData;
		m_pData = new BYTE[m_cbData];
		::memcpy(m_pData, pData, m_cbData);
		m_dwType = REG_BINARY;
	}

	void SetString(PCTSTR pszValue)
	{
		EGLIBASSERT(NULL != pszValue);

		delete [] m_pData;
		m_cbData = ::_tcslen(pszValue) + sizeof(TCHAR);
		m_pData = new BYTE[m_cbData];
		::memcpy(m_pData, pszValue, m_cbData);
		m_dwType = REG_SZ;
	}

	void SetDword(DWORD dwValue)
	{
		delete [] m_pData;
		m_cbData = sizeof(DWORD);
		m_pData = new BYTE[m_cbData];
		::memcpy(m_pData, &dwValue, m_cbData);
		m_dwType = REG_DWORD;
	}

	void GetBinary(PBYTE pData, DWORD cbData) const
	{
		EGLIBASSERT(NULL != m_pData);
       		EGLIBASSERT(NULL != pData);
		EGLIBASSERT(m_cbData == cbData);

		::memcpy(pData, m_pData, m_cbData > cbData ? cbData : m_cbData);
	}

	void GetString(PTSTR pszValue, DWORD dwLength) const
	{
		dwLength *= sizeof(TCHAR);

		EGLIBASSERT(NULL != m_pData);
		EGLIBASSERT(NULL != pszValue);
		//EGLIBASSERT(m_cbData < dwLength);

		::memcpy(pszValue, m_pData, m_cbData > dwLength ? dwLength : m_cbData);
	}

	void GetDword(DWORD* pdwValue) const
	{
		EGLIBASSERT(NULL != m_pData);
		EGLIBASSERT(m_cbData == sizeof(DWORD));

		::memcpy(pdwValue, m_pData, m_cbData > sizeof(DWORD) ? sizeof(DWORD) : m_cbData);
	}

	DWORD GetType() const
	{
		return m_dwType;
	}

	const PBYTE GetData() const
	{
		return m_pData;
	}

	DWORD GetSize() const
	{
		return (DWORD)m_cbData;
	}

	bool IsNull() const
	{
		return NULL == m_pData;
	}

// Operations

// Implementation

protected:

	void Copy(const CRegValue& rhs)
	{
		m_dwType = rhs.m_dwType;
		if (m_cbData == rhs.m_cbData)
		{
			::memcpy(m_pData, rhs.m_pData, m_cbData);
		}
		else
		{
			delete [] m_pData;
			m_cbData = rhs.m_cbData;
			m_pData = new BYTE[m_cbData];
			::memcpy(m_pData, rhs.m_pData, m_cbData);
		}
	}

	void Dispose()
	{
		delete [] m_pData;
		m_dwType = REG_NONE;
		m_cbData = 0;
		m_pData = NULL;
	}
};

//**************************************************************************************************************//
// typedefs
//**************************************************************************************************************//
typedef std::map<std::basic_string<TCHAR>, CRegValue>		CRegValuesMap;
typedef std::set<std::basic_string<TCHAR>/**/>					CRegKeyNamesSet;

//**************************************************************************************************************//
// forward functions declaration
//**************************************************************************************************************//
inline DWORD RegEnumerateValues(
	HKEY hKeyRoot, 
	const std::basic_string<TCHAR>& rsKeyName, 
	CRegValuesMap& rmapValues,
	LPCTSTR lpMachineName = NULL);

inline DWORD RegWriteValues(
	HKEY hKeyRoot, 
	const std::basic_string<TCHAR>& rsKeyName, 
	const CRegValuesMap& rmapValues,
	PSECURITY_ATTRIBUTES pSa = NULL,
	LPCTSTR lpMachineName = NULL);

inline DWORD RegEnumerateKeyNames(
	HKEY hKeyRoot, 
	const std::basic_string<TCHAR>& rsKeyName, 
	CRegKeyNamesSet& rsetNames,
	LPCTSTR lpMachineName = NULL);

//--------------------------------------------------------------------------------------------------------------//
inline DWORD RegEnumerateValues(
	HKEY hKeyRoot, 
	const std::basic_string<TCHAR>& rsKeyName, 
	CRegValuesMap& rmapValues,
	LPCTSTR lpMachineName /*= NULL*/)
{
	rmapValues.clear();

	HKEY hKey;
	HKEY hKeyRemote;

	DWORD dwResult = ERROR_SUCCESS;
	bool bIsNetwork = (lpMachineName != NULL && _tcslen(lpMachineName) > 2);
	if (bIsNetwork)
	{
		TCHAR szMachineName[1024];
#if (_MSC_VER >= 1400)
		if (lpMachineName[0] != _T('\\') && lpMachineName[1] != _T('\\'))
			_stprintf_s(szMachineName, _T("\\\\%s"), lpMachineName);
		else
			_tcscpy_s(szMachineName, lpMachineName);
#else
		if (lpMachineName[0] != _T('\\') && lpMachineName[1] != _T('\\'))
			_stprintf(szMachineName, _T("\\\\%s"), lpMachineName);
		else
			_tcscpy(szMachineName, lpMachineName);
#endif
		dwResult = ::RegConnectRegistry(szMachineName, hKeyRoot, &hKeyRemote);
		if (dwResult != ERROR_SUCCESS)
			return dwResult;

		hKeyRoot = hKeyRemote;
	}

	dwResult = ::RegOpenKeyEx(hKeyRoot, rsKeyName.c_str(), 0, KEY_READ, &hKey);
	if (ERROR_SUCCESS == dwResult)
	{
		int nIndex = 0;
		do
		{
			TCHAR szName[MAX_PATH];
			DWORD nName = sizeof(szName) / sizeof(TCHAR);
			DWORD cbData = 0;
			DWORD dwType;
			dwResult = ::RegEnumValue(hKey, nIndex, szName, &nName, NULL, &dwType, NULL, &cbData);
			if (ERROR_SUCCESS == dwResult)
			{
				PBYTE pData = new BYTE[cbData];
				nName = sizeof(szName) / sizeof(TCHAR);
				dwResult = ::RegEnumValue(hKey, nIndex, szName, &nName, NULL, &dwType, pData, &cbData);
				if (ERROR_SUCCESS == dwResult)
				{
					rmapValues[szName] = CRegValue(dwType, pData, cbData);
				}
				delete [] pData;
			}
			nIndex++;
		} 
		while (ERROR_SUCCESS == dwResult);
		::RegCloseKey(hKey);
	}

	if (bIsNetwork)
		::RegCloseKey(hKeyRemote);

	return (dwResult == ERROR_NO_MORE_ITEMS ?  ERROR_SUCCESS : dwResult);
}

//--------------------------------------------------------------------------------------------------------------//
inline DWORD RegWriteValues(
	HKEY hKeyRoot, 
	const std::basic_string<TCHAR>& rsKeyName, 
	const CRegValuesMap& rmapValues,
	PSECURITY_ATTRIBUTES pSa /*= NULL*/,
	LPCTSTR lpMachineName /*= NULL*/)
{
	HKEY hKey;
	HKEY hKeyRemote;

	DWORD dwResult = ERROR_SUCCESS;
	bool bIsNetwork = (lpMachineName != NULL && _tcslen(lpMachineName) > 2);
	if (bIsNetwork)
	{
		TCHAR szMachineName[1024];
#if (_MSC_VER >= 1400)
		if (lpMachineName[0] != _T('\\') && lpMachineName[1] != _T('\\'))
			_stprintf_s(szMachineName, _T("\\\\%s"), lpMachineName);
		else
			_tcscpy_s(szMachineName, lpMachineName);
#else
		if (lpMachineName[0] != _T('\\') && lpMachineName[1] != _T('\\'))
			_stprintf(szMachineName, _T("\\\\%s"), lpMachineName);
		else
			_tcscpy(szMachineName, lpMachineName);
#endif

		dwResult = ::RegConnectRegistry(szMachineName, hKeyRoot, &hKeyRemote);
		if (dwResult != ERROR_SUCCESS)
			return dwResult;

		hKeyRoot = hKeyRemote;
	}

	dwResult = ::RegCreateKeyEx(
		hKeyRoot, 
		rsKeyName.c_str(), 
		0, 
		NULL, 
		REG_OPTION_VOLATILE, 
		KEY_WRITE, 
		pSa, 
		&hKey, 
		NULL);
	if (ERROR_SUCCESS == dwResult)
	{
		for (CRegValuesMap::const_iterator it = rmapValues.begin(); it != rmapValues.end(); it++)
		{
			const CRegValue& rRegValue = it->second;
			const std::basic_string<TCHAR>& rsKey = it->first;

			EGLIBASSERT(NULL != rRegValue.GetData());
			DWORD dwSetResult = ::RegSetValueEx(
				hKey, 
				rsKey.c_str(), 
				0, 
				rRegValue.GetType(), 
				rRegValue.GetData(), 
				rRegValue.GetSize());

			if (ERROR_SUCCESS != dwSetResult) dwResult = dwSetResult;
		}
		::RegCloseKey(hKey);
	}

	if (bIsNetwork)
		::RegCloseKey(hKeyRemote);

	return dwResult;
}

//--------------------------------------------------------------------------------------------------------------//
inline DWORD RegEnumerateKeyNames(
	HKEY hKeyRoot, 
	const std::basic_string<TCHAR>& rsKeyName, 
	CRegKeyNamesSet& rsetNames,
	LPCTSTR lpMachineName /*= NULL*/)
{
	rsetNames.clear();

	HKEY hKey;
	HKEY hKeyRemote;

	DWORD dwResult = ERROR_SUCCESS;
	bool bIsNetwork = (lpMachineName != NULL && _tcslen(lpMachineName) > 2);
	if (bIsNetwork)
	{
		TCHAR szMachineName[1024];
#if (_MSC_VER >= 1400)
		if (lpMachineName[0] != _T('\\') && lpMachineName[1] != _T('\\'))
			_stprintf_s(szMachineName, _T("\\\\%s"), lpMachineName);
		else
			_tcscpy_s(szMachineName, lpMachineName);
#else
		if (lpMachineName[0] != _T('\\') && lpMachineName[1] != _T('\\'))
			_stprintf(szMachineName, _T("\\\\%s"), lpMachineName);
		else
			_tcscpy(szMachineName, lpMachineName);
#endif

		dwResult = ::RegConnectRegistry(szMachineName, hKeyRoot, &hKeyRemote);
		if (dwResult != ERROR_SUCCESS)
			return dwResult;

		hKeyRoot = hKeyRemote;
	}


    dwResult = ::RegOpenKeyEx(hKeyRoot, rsKeyName.c_str(), 0, KEY_READ, &hKey);
	if (ERROR_SUCCESS == dwResult)
	{
		int nIndex = 0;
		do
		{
			TCHAR szName[MAX_PATH];
			DWORD nName = sizeof(szName) / sizeof(TCHAR);
			dwResult = ::RegEnumKeyEx(hKey, nIndex, szName, &nName, NULL, NULL, NULL, NULL);
			if (ERROR_SUCCESS == dwResult)
			{
				rsetNames.insert(szName);
			}
			nIndex++;
		} 
		while (ERROR_SUCCESS == dwResult);
		::RegCloseKey(hKey);
	}

	if (bIsNetwork)
		::RegCloseKey(hKeyRemote);

	return (dwResult == ERROR_NO_MORE_ITEMS ?  ERROR_SUCCESS : dwResult);
}

class CEgRegKey : public CRegKey
{
protected:
public:
	CEgRegKey () {}
	LONG SetValue(LPCTSTR strValue, LPCTSTR lpszValueName)
	{
#if _MSC_VER > 1200
		return SetStringValue(lpszValueName, strValue); 
#else
		return CRegKey::SetValue (strValue, lpszValueName); 
#endif

	}

	LONG SetValue(DWORD dwValue, LPCTSTR lpszValueName)
	{
#if _MSC_VER > 1200
		return SetDWORDValue(lpszValueName, dwValue); 
#else
		return CRegKey::SetValue (dwValue, lpszValueName); 
#endif
	}
	LONG QueryValue(DWORD& dwValue, LPCTSTR lpszValueName)
	{ 
#if _MSC_VER > 1200
		return QueryDWORDValue(lpszValueName, dwValue); 
#else
		return CRegKey::QueryValue (dwValue, lpszValueName); 
#endif
	}

	LONG QueryValue(int& iValue, LPCTSTR lpszValueName)
	{
		DWORD dwValue=0;
		long ret = QueryValue(dwValue, lpszValueName); 
		if(ERROR_SUCCESS==ret)
			iValue = dwValue;
		return ret;
	}

	LONG GetSize(DWORD& dwSize, LPCTSTR szName)
	{
#if _MSC_VER > 1200
		return QueryStringValue(szName, NULL, &dwSize );			
#else
		return CRegKey::QueryValue (NULL, szName, &dwSize );			
#endif
	}

#ifdef __AFX_H__		
	LONG QueryValue (::CString & rVal, LPCTSTR szName )
	{
		DWORD dwSize =0;

#if _MSC_VER > 1200
		LONG lResult = CRegKey::QueryStringValue(szName,NULL, &dwSize);
#else	
		LONG lResult = CRegKey::QueryValue (NULL, szName, &dwSize );
#endif
		if (lResult == ERROR_SUCCESS && dwSize > 0)
		{
			int nLen = dwSize / sizeof (TCHAR)-1 ;
			LPTSTR szBuff = rVal.GetBuffer (nLen );
#if _MSC_VER > 1200
			lResult = QueryStringValue(szName, szBuff, &dwSize );
#else
			lResult = CRegKey::QueryValue (szBuff, szName, &dwSize );
#endif
			rVal.ReleaseBuffer (nLen);
		}
		return lResult;
	}
#endif
#ifdef __ATLSTR_H__

//#if _MSC_VER > 1200
//	LONG QueryValue (CString & rVal, LPCTSTR szName )
//#else
//	LONG QueryValue (WTL::CString & rVal, LPCTSTR szName )
//#endif
//	{
//		DWORD dwSize =0;
//#if _MSC_VER > 1200
//		LONG lResult = QueryStringValue (const_cast<LPTSTR>(szName), NULL, &dwSize );
//#else
//		LONG lResult = CRegKey::QueryValue (NULL, szName, &dwSize );
//#endif
//		if (lResult == ERROR_SUCCESS && dwSize > 0)
//		{
//			int nLen = dwSize / sizeof (TCHAR)-1;
//			LPTSTR szBuff = rVal.GetBuffer (nLen );
//#if _MSC_VER > 1200
//			lResult = QueryStringValue(szName, szBuff, &dwSize );
//#else
//			lResult = CRegKey::QueryValue (szBuff, szName, &dwSize );
//#endif
//			rVal.ReleaseBuffer (nLen);
//		}
//		return lResult;
//	}
#endif
	LONG QueryValue (CComBSTR & rVal, LPCTSTR szName )
	{
		USES_CONVERSION;
		DWORD dwSize =0;

		LONG lResult = GetSize(dwSize, szName);
		if (lResult == ERROR_SUCCESS && dwSize > 0)
		{
			int nLen = dwSize / sizeof (TCHAR);
			LPTSTR szBuff = new TCHAR [nLen + 1 ];
#if _MSC_VER > 1200
			lResult = QueryStringValue(szName, szBuff, &dwSize );
#else
			lResult = CRegKey::QueryValue (szBuff, szName, &dwSize );
#endif
			if (lResult == ERROR_SUCCESS)
				rVal.Attach ( T2BSTR (szBuff)) ;
			delete [] szBuff;
		}
		return lResult;
	}

#ifdef  _INC_COMUTIL
	LONG QueryValue (_bstr_t & rVal, LPCTSTR szName )
	{
		USES_CONVERSION;
		DWORD dwSize =0;
		LONG lResult = GetSize(dwSize, szName);
		if (lResult == ERROR_SUCCESS && dwSize > 0)
		{
			int nLen = dwSize / sizeof (TCHAR);
			LPTSTR szBuff = new TCHAR [nLen + 1 ];
#if _MSC_VER > 1200
			lResult = QueryStringValue(szName, szBuff, &dwSize );
#else
			lResult = CRegKey::QueryValue (szBuff, szName, &dwSize );
#endif
			if (lResult == ERROR_SUCCESS)
				rVal = szBuff;
			delete [] szBuff;
		}
		return lResult;
	}

	LONG QueryBinaryValue (_bstr_t & rVal, LPCTSTR szName)
	{
		DWORD dwSize=0;
		LONG lResult = ::RegQueryValueEx (m_hKey, szName, NULL, NULL, NULL, &dwSize);
		if (lResult == ERROR_SUCCESS && dwSize >0 )
		{
			BSTR bstrConnect = ::SysAllocStringByteLen (NULL, dwSize);
			lResult =::RegQueryValueEx (m_hKey, szName, NULL, NULL, (BYTE*)bstrConnect, &dwSize);
			if (lResult == ERROR_SUCCESS)
			{
				_bstr_t bstrTmp (bstrConnect, false);
				rVal = bstrTmp;
			}
			else
				::SysFreeString (bstrConnect);
		}
		return lResult;
	}

	LONG SetBinaryValue (const _bstr_t & val, LPCTSTR szName)
	{
		return ::RegSetValueEx (m_hKey, szName, 0, REG_BINARY, (BYTE*) (BSTR) val,
			sizeof (OLECHAR) * val.length() );
	}
#endif
};

} //namespace EgLib

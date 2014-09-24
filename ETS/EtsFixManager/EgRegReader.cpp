#include "StdAfx.h"
#include "egregreader.h"

CEgRegReader::CEgRegReader(LPCTSTR lpszKeyName)
{
	if(ERROR_SUCCESS == m_kLM.Open(HKEY_LOCAL_MACHINE, lpszKeyName, KEY_READ))
		m_bIsLMOpen = true;

	if(ERROR_SUCCESS == m_kCU.Open(HKEY_CURRENT_USER, lpszKeyName, KEY_READ))
		m_bIsCUOpen = true;
}

CEgRegReader::~CEgRegReader(void)
{
	if(m_bIsLMOpen)
		m_kLM.Close();
	if(m_bIsCUOpen)
		m_kCU.Close();
}

long CEgRegReader::Read(LPCTSTR lpszValueName, long nDefault)
{
	DWORD dwVal;
	if(m_bIsCUOpen && ERROR_SUCCESS == m_kCU.QueryDWORDValue( lpszValueName, dwVal))
		return dwVal;
	else if(m_bIsLMOpen && ERROR_SUCCESS == m_kLM.QueryDWORDValue( lpszValueName, dwVal))
		return dwVal;

	return nDefault;
}

CString CEgRegReader::Read(LPCTSTR lpszValueName, LPCTSTR lpszDefault)
{
	DWORD dwSize = 0;
	long nResult;
	CString strValue;

	if(m_bIsCUOpen)
	{
		nResult = m_kCU.QueryStringValue (lpszValueName, NULL, &dwSize );
		if(ERROR_SUCCESS == nResult && dwSize > 0)
		{
			LPTSTR pszBuf = strValue.GetBuffer(static_cast<int>(dwSize));
			nResult = m_kCU.QueryStringValue (lpszValueName, pszBuf, &dwSize );
			strValue.ReleaseBuffer();
			if(ERROR_SUCCESS == nResult)
				return strValue;
		}		
	}

	if(m_bIsLMOpen)
	{
		dwSize = 0;
		nResult = m_kLM.QueryStringValue (lpszValueName, NULL, &dwSize );
		if(ERROR_SUCCESS == nResult && dwSize > 0)
		{
			LPTSTR pszBuf = strValue.GetBuffer(static_cast<int>(dwSize));
			nResult = m_kLM.QueryStringValue (lpszValueName, pszBuf, &dwSize );
			strValue.ReleaseBuffer();
			if(ERROR_SUCCESS == nResult)
				return strValue;
		}		
	}

	return CString(lpszDefault);
}

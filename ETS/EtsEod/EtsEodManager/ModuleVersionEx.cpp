#include "StdAfx.h"
#include "ModuleVersionEx.h"

#pragma comment(linker, "/defaultlib:version.lib")

/////////////////////////////////////////////////////////////////////////////
//
CModuleVersionEx::CModuleVersionEx()
	: m_pVersionInfo(NULL)
{
}

/////////////////////////////////////////////////////////////////////////////
//
CModuleVersionEx::~CModuleVersionEx()
{
	if(m_pVersionInfo)
		delete [] m_pVersionInfo;
}

/////////////////////////////////////////////////////////////////////////////
//
void CModuleVersionEx::CleanUp()
{
	if(m_pVersionInfo)
	{
		delete [] m_pVersionInfo;
		m_pVersionInfo = NULL;
	}

	m_Translation.dwCharset = 1252;		// default = ANSI code page
	memset((VS_FIXEDFILEINFO*)this, 0, sizeof(VS_FIXEDFILEINFO));

	m_strFilePath = _T("N/A");
	m_strFileName = _T("N/A");
}

/////////////////////////////////////////////////////////////////////////////
//
BOOL CModuleVersionEx::SetModuleHandle(HMODULE hModule)
{
	CleanUp();

	TCHAR szPath[4096] = {0};
	if(::GetModuleFileName(hModule, szPath, 4096) <= 0)
		return FALSE;

	AdjustFilePathAndName(szPath);

	return LoadFileVersionInfo();
}

/////////////////////////////////////////////////////////////////////////////
//
void CModuleVersionEx::AdjustFilePathAndName(LPCTSTR szPath)
{
	m_strFilePath = CModuleVersionEx::GetLongPathName(szPath);
	m_strFileName = CModuleVersionEx::GetFileName(m_strFilePath);
}

/////////////////////////////////////////////////////////////////////////////
//
BOOL CModuleVersionEx::SetModuleFileName(LPCTSTR szFileName)
{
	CleanUp();

	if(!szFileName)
		return FALSE;

	CString strFileName(szFileName);
	if(strFileName.FindOneOf(_T("\\/:")) < 0)
	{
		TCHAR szPath[4096] = {0};
		DWORD dwRes = 0;
		HMODULE hModule = ::GetModuleHandle(strFileName);
		if(!hModule)
		{
			hModule = ::LoadLibrary(strFileName);
			dwRes = ::GetModuleFileName(hModule, szPath, 4096);
			::FreeLibrary(hModule);
		}
		else
			dwRes = ::GetModuleFileName(hModule, szPath, 4096);

		if(!dwRes)
			return FALSE;

		AdjustFilePathAndName(szPath);
	}
	else
		AdjustFilePathAndName(szFileName);

	return LoadFileVersionInfo();
}

/////////////////////////////////////////////////////////////////////////////
//
BOOL CModuleVersionEx::SetModulePROGID(LPCTSTR szProgID)
{
	USES_CONVERSION;
	CLSID clsid = { 0 };

	if(FAILED(::CLSIDFromProgID(T2W(szProgID), &clsid)))
		return FALSE;

	return SetModuleCLSID(clsid);
}

/////////////////////////////////////////////////////////////////////////////
//
BOOL CModuleVersionEx::SetModuleCLSID(CLSID& clsid)
{
	LPOLESTR szCLSID = NULL;
	::StringFromCLSID(clsid, &szCLSID);

	CString strKey = _T("CLSID\\");
	strKey += CString(szCLSID);

	::CoTaskMemFree(szCLSID);

	HKEY hKey = 0L;
	long nResult = ::RegOpenKeyEx(HKEY_CLASSES_ROOT, strKey + _T("\\LocalServer32"), 0, KEY_QUERY_VALUE, &hKey);
	if(ERROR_SUCCESS != nResult)
		nResult = ::RegOpenKeyEx(HKEY_CLASSES_ROOT, strKey + _T("\\InprocServer32"), 0, KEY_QUERY_VALUE, &hKey);

	if(ERROR_SUCCESS == nResult)
	{
		TCHAR szBuf[4096] = { 0 };
		DWORD dwBufLen = 4096;
		nResult = ::RegQueryValueEx(hKey, _T(""), NULL, NULL, (LPBYTE)szBuf, &dwBufLen);
		::RegCloseKey(hKey);

		if(ERROR_SUCCESS == nResult)
			return SetModuleFileName(szBuf);
	}

	return FALSE;
}

/////////////////////////////////////////////////////////////////////////////
//
BOOL CModuleVersionEx::LoadFileVersionInfo()
{
	// read	file version info
	DWORD dwDummyHandle;

	// will	always be set to zero
	DWORD dwLen = ::GetFileVersionInfoSize(m_strFilePath.GetBuffer(), &dwDummyHandle);
	
	if(dwLen <= 0)
		return FALSE;

	if(!m_pVersionInfo)
		m_pVersionInfo = new BYTE[dwLen]; // allocate version info

	if(!::GetFileVersionInfo(m_strFilePath.GetBuffer(), 0, dwLen, m_pVersionInfo))
		return FALSE;

	LPVOID pv;
	UINT iLen;

	if(!VerQueryValue(m_pVersionInfo, _T("\\"), &pv, &iLen))
		return FALSE;

	// copy	fixed info to myself, which	am derived from	VS_FIXEDFILEINFO
	*(VS_FIXEDFILEINFO*)this = *(VS_FIXEDFILEINFO*)pv;

	// Get translation info
	if(VerQueryValue(m_pVersionInfo, _T("\\VarFileInfo\\Translation"), &pv, &iLen) && iLen >= 4)
		m_Translation = *(TRANSLATION*)pv;
	
	return (dwSignature == VS_FFI_SIGNATURE);
}

/////////////////////////////////////////////////////////////////////////////
//
BOOL CModuleVersionEx::GetValue(LPCTSTR lpKeyName, CString& strVal)
{
	BOOL bRet = FALSE;
	TCHAR szTemp[MAX_PATH];
	if(m_pVersionInfo)
	{
		// To get a	string value must pass query in the form
		//		"\StringFileInfo\<langID><codepage>\keyname"
		// where <langID><codepage> is the languageID concatenated with the
		// code page, in hex. Wow.

		wsprintf(szTemp, _T("\\StringFileInfo\\%04x%04x\\%s"),
				m_Translation.dwLangID, m_Translation.dwCharset, lpKeyName);

		LPCTSTR	pVal;
		UINT iLenVal;
		if(VerQueryValue(m_pVersionInfo, szTemp, (LPVOID*)&pVal, &iLenVal))
		{
			LPTSTR szValue = strVal.GetBuffer(iLenVal);
			_tcsncpy_s(szValue,iLenVal,pVal,	iLenVal);
			strVal.ReleaseBuffer();
			bRet = TRUE	;
		}
	}

	return bRet;
}

/////////////////////////////////////////////////////////////////////////////
//
CString CModuleVersionEx::GetFixedFileVersion()
{
	CString sRetVal;
	if(GetValue(_T("FileVersion"), sRetVal))
	{
		sRetVal.Replace(_T(","), _T("."));
		sRetVal.Replace(_T(" "), _T(""));
		return sRetVal;
	}
	else
		return _T("N/A");
}

/////////////////////////////////////////////////////////////////////////////
//
CString CModuleVersionEx::GetFileDescription()
{
	CString sRetVal;
	if(GetValue(_T("FileDescription"), sRetVal))
		return sRetVal;
	else
		return _T("N/A");
}

/////////////////////////////////////////////////////////////////////////////
//
CString CModuleVersionEx::GetLegalCopyright()
{
	CString sRetVal;
	if(GetValue(_T("LegalCopyright"), sRetVal))
		return sRetVal;
	else
		return _T("N/A");
}

/////////////////////////////////////////////////////////////////////////////
//
CString CModuleVersionEx::GetLongPathName(CString strShortPathName)
{
    //HINSTANCE hModKernel = GetModuleHandle( "kernel32.dll" );
    //typedef DWORD (WINAPI *PFNGETLONGPATHNAMEA)(LPCTSTR lpszShortPath, LPTSTR lpszLongPath, DWORD cchBuffer);
    //PFNGETLONGPATHNAMEA pfnGetLongPathName = (PFNGETLONGPATHNAMEA)GetProcAddress(hModKernel, "GetLongPathNameA");
    //if(pfnGetLongPathName)
    //    return pfnGetLongPathName(lpszShortPath, lpszLongPath, cchBuffer);
    //else
    //    return _GetLongPathName(lpszShortPath, lpszLongPath, cchBuffer);


	CString strLongPathName;
	CString strTemp;

	int nSlashPos = 3, Offset = 0;
	WIN32_FIND_DATA FindFileData;
	HANDLE hSearch = INVALID_HANDLE_VALUE;

	// Start from 3 to ignore the "[Drive Letter]:" characters
	strLongPathName = strShortPathName.Left(2);

	// Pull out each string between \ character for conversion
	while(nSlashPos > 0 && nSlashPos < strShortPathName.GetLength())
	{
		strLongPathName += "\\";
		Offset = strShortPathName.Find("\\", nSlashPos);
		if(Offset >= 0)
		{
			strTemp = strShortPathName.Left(Offset);
		}
		else
			strTemp = strShortPathName;

		if((hSearch = FindFirstFile(strTemp, &FindFileData)) != INVALID_HANDLE_VALUE)
		{
			strLongPathName += FindFileData.cFileName;
			nSlashPos = Offset + 1;
			FindClose(hSearch);
		}
		else
			return strShortPathName;
	}

	return strLongPathName;
}

/////////////////////////////////////////////////////////////////////////////
//
CString CModuleVersionEx::GetFileName(CString strPathName)
{
	int nPos = CModuleVersionEx::GetFileNamePos(strPathName);
	
	if(nPos >= 0)
		return strPathName.Mid(nPos);

	return _T("");
}

/////////////////////////////////////////////////////////////////////////////
//
CString CModuleVersionEx::GetFilePath(CString strPathName)
{
	int nPos = CModuleVersionEx::GetFileNamePos(strPathName);

 	if(nPos > 0)
		return strPathName.Left(nPos);

	return _T("");
}

/////////////////////////////////////////////////////////////////////////////
//
int CModuleVersionEx::GetFileNamePos(CString strPathName)
{
	int nPos = -1;

	if((nPos = strPathName.ReverseFind(_T('\\'))) >= 0)
		return nPos + 1;
	if((nPos = strPathName.ReverseFind(_T(':'))) >= 0)
		return nPos + 1;
	if((nPos = strPathName.ReverseFind(_T('/'))) >= 0)
		return nPos + 1;

	return nPos;
}

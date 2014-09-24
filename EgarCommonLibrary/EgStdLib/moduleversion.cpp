#include "stdafx.h"

#include "ModuleVersion.h"

// tell linker to link with version.lib for VerQueryValue, etc.
#pragma comment(linker, "/defaultlib:version.lib")

namespace EgStd
{

CModuleVersion::CModuleVersion()
{
    m_pVersionInfo = NULL; // raw version info data
}

//////////////////
// Destroy: delete version info
CModuleVersion::~CModuleVersion()
{
    delete [] m_pVersionInfo;
}

//////////////////
// Get file version info for a given module
// Allocates storage for all info, fills "this" with
// VS_FIXEDFILEINFO, and sets codepage.
BOOL CModuleVersion::GetModuleVersionInfo(HINSTANCE hInstance)
{
    m_Translation.dwCharset = 1252;    // default = ANSI code page
    memset((VS_FIXEDFILEINFO*)this, 0, sizeof(VS_FIXEDFILEINFO));

    // get module handle
    TCHAR filename[_MAX_PATH];

 
   // get module file name
    DWORD len = GetModuleFileName(hInstance, filename,
                                  sizeof(filename)/sizeof(filename[0]));

    if ( len <= 0 )
        return (FALSE);
	
	return GetFileVersionInfo (filename);
}

BOOL CModuleVersion::GetModuleVersionInfo(LPCTSTR szModuleName)
{
    HMODULE hModule = ::GetModuleHandle(szModuleName);

    if (hModule == NULL && szModuleName != NULL)
	{
		return FALSE;
	}

    return GetModuleVersionInfo(hModule);
}

BOOL CModuleVersion::GetFileVersionInfo(LPCTSTR szFileName)
{
    // read file version info
    DWORD dwDummyHandle;

    // will always be set to zero
    DWORD dwLen = GetFileVersionInfoSize((LPTSTR)szFileName, &dwDummyHandle);
    
	if (dwLen <= 0)
        return FALSE;

    m_pVersionInfo = new BYTE[dwLen]; // allocate version info

    if (!::GetFileVersionInfo((LPTSTR)szFileName, 0, dwLen, m_pVersionInfo))
        return FALSE;

    LPVOID	pv;
    UINT	iLen;

    if (!VerQueryValue(m_pVersionInfo, _T("\\"), &pv, &iLen))
        return FALSE;

    // copy fixed info to myself, which am derived from VS_FIXEDFILEINFO
    *(VS_FIXEDFILEINFO*)this = *(VS_FIXEDFILEINFO*)pv;

    // Get translation info
	if (VerQueryValue(m_pVersionInfo, _T("\\VarFileInfo\\Translation"),
                       &pv, &iLen) && iLen >= 4)
    {
        m_Translation = *(TRANSLATION*)pv;
        //TRACE("code page = %d\n", m_translation.charset);
    }
	
	return (dwSignature == VS_FFI_SIGNATURE);	
}

//////////////////
// Get string file info.
// Key name is something like "CompanyName".
// returns the value in szValue.
BOOL CModuleVersion::GetValue(LPCTSTR szKeyName, _bstr_t& sVal)
{
    BOOL	bRet = FALSE;
    TCHAR	szTemp[MAX_PATH];
    
	if (m_pVersionInfo)
    {
        // To get a string value must pass query in the form
        //
        //    "\StringFileInfo\<langID><codepage>\keyname"
        //
        // where <langID><codepage> is the languageID concatenated with the
        // code page, in hex. Wow.
        //

        wsprintf ( szTemp ,
                  _T("\\StringFileInfo\\%04x%04x\\%s"),
                  m_Translation.dwLangID,
                  m_Translation.dwCharset,
                  szKeyName);

        LPCTSTR szVal;
        UINT	iLenVal;

        if ( VerQueryValue(m_pVersionInfo, szTemp,
                           (LPVOID*)&szVal, &iLenVal) )
        {
            sVal = (LPCTSTR)szVal;
            bRet = TRUE ;
        }
    }

    return bRet;
}

// typedef for DllGetVersion proc
typedef HRESULT (CALLBACK* DLLGETVERSIONPROC)(DLLVERSIONINFO *);

/////////////////
// Get DLL Version by calling DLL's DllGetVersion proc
BOOL CModuleVersion::DllGetVersion(LPCTSTR szModuleName, DLLVERSIONINFO& dvi)
{
    HINSTANCE hInst = LoadLibrary(szModuleName);
    
	if (!hInst)
        return FALSE;

    // Must use GetProcAddress because the DLL might not implement
    // DllGetVersion. Depending upon the DLL, the lack of implementation
    // of the function may be a version marker in itself.
    //
    DLLGETVERSIONPROC pDllGetVersion = (DLLGETVERSIONPROC)GetProcAddress(hInst, "DllGetVersion");
    
	if (!pDllGetVersion)
        return FALSE;

    memset(&dvi, 0, sizeof(dvi));        // clear
    dvi.cbSize = sizeof(dvi);            // set size for Windows

    return SUCCEEDED((*pDllGetVersion)(&dvi));
}

}
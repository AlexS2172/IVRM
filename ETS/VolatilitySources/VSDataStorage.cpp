#include "StdAfx.h"
#include "vsdatastorage.h"
#include "VolatilitySourcesImpl.h"

CVSDataStorage g_VSDataStorage;

//Helper Classes Definitions
//////////////////////////////////////////////////////////////////////////
class CModuleVersion : public VS_FIXEDFILEINFO
{
protected:
    BYTE* m_pVersionInfo;   // all version info

    struct TRANSLATION
    {
        WORD langID;         // language ID
        WORD charset;        // character set (code page)
    } m_translation;

public:
    CModuleVersion();
    virtual ~CModuleVersion();

    BOOL     GetModuleVersionInfo(HINSTANCE hInstance);
    BOOL     GetModuleVersionInfo(LPCTSTR modulename);
    BOOL     GetFileVersionInfo(LPCTSTR filename);

	BOOL	 GetValue(LPCTSTR lpKeyName, _bstr_t& strVal);
    static BOOL DllGetVersion(LPCTSTR modulename, DLLVERSIONINFO& dvi);
};


typedef std::vector<_bstr_t> SOURCES;
class CCatInformation : public SOURCES {
public:
	CCatInformation(){}
	HRESULT Initialize();
};
/////////////////////////////////////////////////////////////////////////////



CVSDataStorage::CVSDataStorage()
//	:m_lCurrentID(0),
	:m_bInitialized(false)
{
	Reload();
}

bool CVSDataStorage::GetItembyID(long ID, VSData * pData)
{
	CAutoLock cs(m_csDataAccess);
	iterator iter = find(ID);
	if(iter != end())
	{
		*pData = iter->second;
		return true;
	}
	return false;
}

void CVSDataStorage::LoadDefaultItem()
{
	VSData newItem;
	newItem.m_clsid = CLSID_NULL;
	newItem.m_Title = _T("Default volatility source; CLSID = VME.VolatilitySource");
	HRESULT hr = CLSIDFromString(L"VME.VolatilitySource", &newItem.m_clsid);
	if(FAILED(hr))
		return;
	if(FillFileNameAndVersionW(&newItem) || FillFileNameAndVersionA(&newItem))
		insert(CVSDataStorage::value_type(0, newItem));
}

void CVSDataStorage::Reload()
{
	CAutoLock cs(m_csDataAccess);

	clear();
//	m_lCurrentID = 0;
	
	try
	{
		CCatInformation catInfo;
		HRESULT hr = catInfo.Initialize();
		if(FAILED(hr))
			return;
		
		if(SUCCEEDED(hr))	
		{
			ULONG nRet=0;
			SOURCES::iterator iter;
			for(iter= catInfo.begin(); iter!=catInfo.end(); iter++)
			{
				 CEgRegKey reg;
				_bstr_t str(_T("SOFTWARE\\Egar\\VolatilitySources\\"));
				str += *iter;
				bool bRead = false;
				if(ERROR_SUCCESS == reg.Open(HKEY_LOCAL_MACHINE,str,KEY_READ))
				{
					DWORD dwID = 0;
					_bstr_t bsName;
					if(ERROR_SUCCESS == reg.QueryValue(dwID,  _T("")) &&
					ERROR_SUCCESS == reg.QueryValue(bsName,_T("Name")))
					{
						CLSID clsid = GUID_NULL;
						HRESULT _hr = CLSIDFromString(*iter, &clsid);
						if(SUCCEEDED(_hr))
						{
							VSData newItem;
							newItem.m_clsid = clsid;
							newItem.m_Title = bsName;
							if(FillFileNameAndVersionW(&newItem) || FillFileNameAndVersionA(&newItem))
								insert(CVSDataStorage::value_type(dwID, newItem));
						}
					}
				
				}
			}	

		}
		//Load Current Source ID
/*
		DWORD ret = m_lCurrentID;
		 CEgRegKey reg;
		_bstr_t str(_T("SOFTWARE\\Egar\\ETS\\VolatilitySource"));
		if(reg.Open(HKEY_LOCAL_MACHINE,str,KEY_READ)== ERROR_SUCCESS)
		{	
			if(reg.QueryDWORDValue(_T("Type"), ret) == ERROR_SUCCESS)
			{
				m_lCurrentID = ret;
			}
		}*/

		//Load default Source
		LoadDefaultItem();
	}
	catch (...)
	{
		m_bInitialized = false;
	}
	m_bInitialized = true;
}

bool CVSDataStorage::FillFileNameAndVersionA(VSData* pData)
{
	bool bret = false;
	ATLASSERT(pData);
	pData->m_Version = _T("N/A");
	pData->m_FileName=_T("");

	if(pData->m_clsid == GUID_NULL)
	{
		return false;
	}
	LPOLESTR szCLSID=NULL;
	StringFromCLSID (pData->m_clsid, &szCLSID);

	_bstr_t strKey = _T("CLSID\\");
	strKey += szCLSID;
	CoTaskMemFree (szCLSID);

	CEgRegKey key;	
	long lResult =  key.Open(HKEY_CLASSES_ROOT, strKey , KEY_READ );
	if (lResult == ERROR_SUCCESS)
	{
		CEgRegKey keyModule;
		long lResult = keyModule.Open( key,  _T("LocalServer32"), KEY_READ );
		if (lResult != ERROR_SUCCESS)
			lResult = keyModule.Open( key,  _T("InprocServer32"), KEY_READ );

		if (lResult == ERROR_SUCCESS)
		{
			_bstr_t strVal;
			keyModule.QueryValue (strVal, _T(""));

			if (strVal.length())
			{
				CModuleVersion ver;

				if (ver.GetFileVersionInfo (strVal))
					ver.GetValue(_T("FileVersion"), pData->m_Version);

				WIN32_FIND_DATA data;
				HANDLE hFind = FindFirstFile (strVal, &data);

				if( hFind != INVALID_HANDLE_VALUE )
				{
					FindClose(hFind);
					pData->m_FileName =  data.cFileName;	
					bret = true;
				}

			}
			keyModule.Close();
		}

		lResult = keyModule.Open( key, _T("ProgId"), KEY_READ );
		if(lResult == ERROR_SUCCESS)
		{
			_bstr_t strVal;
			keyModule.QueryValue (strVal, _T(""));

			if (strVal.length())
			{
				pData->m_ProgID = strVal;	
			}

			keyModule.Close();
		}

		key.Close();
	}
	return bret ;
}

bool CVSDataStorage::FillFileNameAndVersionW(VSData* pData)
{
	bool bret = false;
	ATLASSERT(pData);
	pData->m_Version = _T("N/A");
	pData->m_FileName=_T("");

	if(pData->m_clsid == GUID_NULL)
	{
		return false;
	}
	LPOLESTR szCLSID=NULL;
	StringFromCLSID (pData->m_clsid, &szCLSID);

	_bstr_t strKey = _T("CLSID\\");
	strKey += szCLSID;
	CoTaskMemFree (szCLSID);

	CEgRegKey key;	
	long lResult =  key.Open(HKEY_CLASSES_ROOT, strKey , KEY_READ );
	if (lResult == ERROR_SUCCESS)
	{
		CEgRegKey keyModule;
		long lResult = keyModule.Open( key,  _T("LocalServer32"), KEY_READ );
		if (lResult != ERROR_SUCCESS)
			lResult = keyModule.Open( key,  _T("InprocServer32"), KEY_READ );

		if (lResult == ERROR_SUCCESS)
		{
			_bstr_t strFile = L"\\\\?\\";
			_bstr_t strVal;
			keyModule.QueryValue (strVal, _T(""));


			if (strVal.length())
			{
				std::string sVal = (LPCTSTR)strVal;
				if(sVal[0]=='\"')
					strVal = sVal.substr(1,sVal.length()-2).c_str();

				strFile += strVal;
				CModuleVersion ver;

				if (ver.GetFileVersionInfo (strVal))
					ver.GetValue(_T("FileVersion"), pData->m_Version);

				WIN32_FIND_DATAW  data;

				HANDLE hFind = FindFirstFileW (strFile, &data);
				if( hFind != INVALID_HANDLE_VALUE )
				{
					FindClose(hFind);
					pData->m_FileName =  data.cFileName;	
					bret = true;
				}
				else
				{
					DWORD dw = GetLastError();
					int k=0;
				}

			}
			keyModule.Close();
		}

		lResult = keyModule.Open( key, _T("ProgId"), KEY_READ );
		if(lResult == ERROR_SUCCESS)
		{
			_bstr_t strVal;
			keyModule.QueryValue (strVal, _T(""));

			if (strVal.length())
			{
				pData->m_ProgID = strVal;	
			}

			keyModule.Close();
		}

		key.Close();
	}

	return bret ;
}


bool CVSDataStorage::GetItembyIndex(long index, long *ID, VSData * pData)
{
	CAutoLock cs(m_csDataAccess);
	if(index < 1 || index > (long)size())
		return false;

	iterator iter = begin();
	for(long i = 1; i < index; i++, iter++){}

	if(iter != end())
	{
		*ID = iter->first;
		*pData = iter->second;
		return true;
	}
	return false;
}

//HRESULT CVSDataStorage::GetItembyIndex(long index, CLSID* pClsid)
//{
//	long ID = 0;
//	VSData Data;
//	if(GetItembyIndex(index, &ID, &Data))
//	{
//		*pClsid = Data.m_clsid;
//		return S_OK;
//	}
//	return E_INVALIDARG;
//}

/*

HRESULT  CVSDataStorage::GetCurrentSource(CLSID* pClsid)
{
	CCriticalSectionWrapper cs(m_csDataAccess);
	iterator index = find(m_lCurrentID);
	if(index == end())
		return E_FAIL;

	*pClsid = index->second.m_clsid;
	return S_OK;
}

HRESULT  CVSDataStorage::GetCurrentSource(long* pID)
{
	CCriticalSectionWrapper cs(m_csDataAccess);
	*pID = m_lCurrentID;
	return S_OK;
}

HRESULT CVSDataStorage::SetCurrentSource(long ID)
{
	CCriticalSectionWrapper cs(m_csDataAccess);
	iterator index = find(ID);
	if(index == end())
		return E_FAIL;
	
	bool bOK = false;
	DWORD ret = ID;
	LONG Err = ERROR_SUCCESS;
	try
	{
	
		 CEgRegKey reg;
		_bstr_t str(_T("SOFTWARE\\Egar\\ETS\\VolatilitySource"));
		Err = reg.Open(HKEY_LOCAL_MACHINE,str,KEY_WRITE);
		if(Err == ERROR_SUCCESS)
		{	
			Err = reg.SetDWORDValue(_T("Type"), ret);
			if(Err == ERROR_SUCCESS)
			{
				m_lCurrentID = ID;
				bOK = true;
			}
		}
	}catch(...)
	{
		m_lCurrentID = 0;
	}

	return Err;
}
*/

//Helper Classes Implementation
///////////////////////////////////////////////////////////////////
HRESULT CCatInformation::Initialize()
{
	clear();
	CEgRegKey key;
	if(ERROR_SUCCESS!=key.Open(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\Egar\\VolatilitySources"), KEY_READ ))
		return E_FAIL;
	int i=0;
	TCHAR chBuffer[256]={0};
	DWORD dwSize = 255;
	while(ERROR_SUCCESS == key.EnumKey(i, chBuffer, &dwSize))
	{
		if(dwSize)
			insert(end(), chBuffer);
		dwSize= 255;
		i++;
	}
	if(!size())
		return E_FAIL;
	return S_OK;
}



CModuleVersion::CModuleVersion()
{
    m_pVersionInfo = NULL; // raw version info data
}

//////////////////
// Destroy: delete version info
//
CModuleVersion::~CModuleVersion()
{
    delete [] m_pVersionInfo;
}


//////////////////
// Get file version info for a given module
// Allocates storage for all info, fills "this" with
// VS_FIXEDFILEINFO, and sets codepage.
//


BOOL CModuleVersion::GetModuleVersionInfo(HINSTANCE hInstance)
{
    m_translation.charset = 1252;    // default = ANSI code page
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

BOOL CModuleVersion::GetModuleVersionInfo(LPCTSTR modulename)
{
    HMODULE hModule = ::GetModuleHandle(modulename);
    if ( hModule==NULL && modulename!=NULL )
        return (FALSE);

    return (GetModuleVersionInfo(hModule));
}

BOOL CModuleVersion::GetFileVersionInfo(LPCTSTR filename)
{
    // read file version info
    DWORD dwDummyHandle;
    // will always be set to zero
    DWORD len = GetFileVersionInfoSize( (LPTSTR)filename, &dwDummyHandle);
    if ( len <= 0 )
        return (FALSE);
    m_pVersionInfo = new BYTE[len]; // allocate version info
    if ( !::GetFileVersionInfo( (LPTSTR) filename, 0, len, m_pVersionInfo) )
        return (FALSE);

    LPVOID lpvi;
    UINT iLen;

    if ( !VerQueryValue(m_pVersionInfo, _T("\\"), &lpvi, &iLen) )
        return (FALSE);

    // copy fixed info to myself, which am derived from VS_FIXEDFILEINFO
    *(VS_FIXEDFILEINFO*)this = *(VS_FIXEDFILEINFO*)lpvi;

    // Get translation info
    if ( VerQueryValue(m_pVersionInfo, _T("\\VarFileInfo\\Translation"),
                       &lpvi, &iLen) && iLen >= 4 )
    {
        m_translation = *(TRANSLATION*)lpvi;
        //TRACE("code page = %d\n", m_translation.charset);
    }
    return (dwSignature == VS_FFI_SIGNATURE);	
}



//////////////////
// Get string file info.
// Key name is something like "CompanyName".
// returns the value in szValue.
//
BOOL CModuleVersion::GetValue(LPCTSTR lpKeyName, _bstr_t& strVal)
{
    BOOL bRet = FALSE ;
    TCHAR szTemp [ MAX_PATH ] ;
    if ( m_pVersionInfo )
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
                  m_translation.langID,
                  m_translation.charset,
                  lpKeyName);

        LPCTSTR pVal;
        UINT iLenVal;
        if ( VerQueryValue(m_pVersionInfo, szTemp,
                           (LPVOID*)&pVal, &iLenVal) )
        {
			strVal = pVal;
            bRet = TRUE ;
        }
    }
    return ( bRet );
}

// typedef for DllGetVersion proc
typedef HRESULT (CALLBACK* DLLGETVERSIONPROC)(DLLVERSIONINFO *);

/////////////////
// Get DLL Version by calling DLL's DllGetVersion proc
//
BOOL CModuleVersion::DllGetVersion(LPCTSTR modulename,
                                   DLLVERSIONINFO& dvi)
{
    HINSTANCE hinst = LoadLibrary(modulename);
    if ( !hinst )
        return (FALSE);

    // Must use GetProcAddress because the DLL might not implement
    // DllGetVersion. Depending upon the DLL, the lack of implementation
    // of the function may be a version marker in itself.
    //
    DLLGETVERSIONPROC pDllGetVersion =
    (DLLGETVERSIONPROC)GetProcAddress(hinst, _T("DllGetVersion"));
    if ( !pDllGetVersion )
        return (FALSE);

    memset(&dvi, 0, sizeof(dvi));        // clear
    dvi.cbSize = sizeof(dvi);            // set size for Windows

    return (SUCCEEDED((*pDllGetVersion)(&dvi)));
}
///////////////////////////////////////////////////////////////////

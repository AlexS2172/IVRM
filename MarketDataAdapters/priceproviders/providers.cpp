// Providers.cpp : Implementation of CProviders
#include "stdafx.h"
#include "PriceProviders.h"
#include "Providers.h"

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

/////////////////////////////////////////////////////////////////////////////
// CProviders

CProviders::CProviders()
{
		m_bInitialized = false;
}

CProviders::~CProviders()
{
	Clear();
}

STDMETHODIMP CProviders::Initialize()
{
	if(m_bInitialized)
		return S_OK;
	Clear();

	HRESULT hr = CComObject<CProvidersCache>::CreateInstance(&m_pCache);
	if(FAILED(hr) || m_pCache == NULL)
		return hr;

	m_pCache->Attach();
	long lCount = 0;
	m_pCache->get_Count(&lCount);
	for(long iL=1; iL<=lCount; iL++)
	{
		IProviderData* pData;
		m_pCache->get_Item(iL, &pData);
		m_coll.push_back(pData);
	}
	if(SUCCEEDED(hr))
		m_bInitialized = true;
	return hr;
}

void CProviders::Clear()
{
	collection::iterator iter;
	for(iter = m_coll.begin(); iter!=m_coll.end(); iter++)
	{
		(*iter)->Release();
	}
	m_coll.clear();
}

STDMETHODIMP CProviders::GetProvider(long ProviderID, IProviderData **ppProvider)
{
	if(!ppProvider)
		return E_POINTER;

	
	HRESULT hr = Initialize();
	if(FAILED(hr))
		return hr;

	collection::iterator iter = m_coll.begin();
	while (iter != m_coll.end())
	{
		long lProv = -10;

		(*iter)->get_ProviderID(&lProv);

		if(lProv == ProviderID)
			break;

		iter++;
	}
	if (iter != m_coll.end())
	{
		CProviderData* pData = dynamic_cast<CProviderData*>(*iter);
		if(pData && !pData->m_bInitialized)
		{
			// Load Info
			IProviderInfoPtr spInfo;
			hr = spInfo.CreateInstance(pData->m_clsidProvider);
			if(SUCCEEDED(hr))
			{
			
				BSTR  bsData;
				try
				{
					__CHECK_HRESULT3(spInfo->get_ProviderID(&pData->m_lID));
					__CHECK_HRESULT3(spInfo->get_Description(&pData->m_bsDescription));
					__CHECK_HRESULT3(spInfo->get_IsGroup(&pData->m_bGroup));
					__CHECK_HRESULT3(spInfo->get_NeedLogin(&pData->m_bLogin));

					/*__CHECK_HRESULT3(spInfo->get_Provider(enStructureProvider,&bsData));
					pData->m_Progs[enStructureProvider] = bsData;
					SysFreeString(bsData);			
					bsData = NULL;

					__CHECK_HRESULT3(spInfo->get_Provider(enStructureInfo, &bsData));
					pData->m_Progs[enStructureInfo] = bsData;
					SysFreeString(bsData);			
					bsData = NULL;

					__CHECK_HRESULT3(spInfo->get_Provider(enPriceProvider, &bsData));
					pData->m_Progs[enPriceProvider] = bsData;
					SysFreeString(bsData);			
					bsData = NULL;

					__CHECK_HRESULT3(spInfo->get_Provider(enPriceInfo, &bsData));
					pData->m_Progs[enPriceInfo] = bsData;
					SysFreeString(bsData);	
					bsData = NULL;

					__CHECK_HRESULT3(spInfo->get_Provider(enPriceInfoWithNotify, &bsData));
					pData->m_Progs[enPriceInfoWithNotify] = bsData;
					SysFreeString(bsData);	
					bsData = NULL;
*/
					__CHECK_HRESULT3(spInfo->get_Provider(enBatchPriceInfo, &bsData));
					pData->m_Progs[enBatchPriceInfo] = bsData;
					SysFreeString(bsData);	
					bsData = NULL;

					__CHECK_HRESULT3(spInfo->get_Provider(enStructureProviderEx,&bsData));
					pData->m_Progs[enStructureProviderEx] = bsData;
					SysFreeString(bsData);			
					bsData = NULL;

					pData->m_bInitialized = true;
				}
				catch(_com_error& e)
				{
					if(bsData)
					{
						SysFreeString(bsData);
						bsData = NULL;
					}
					hr = e.Error();
					return hr;
				}
			}
		}
		hr = _CopyProvider::copy(ppProvider, &*iter);
	}
	return hr;
}

STDMETHODIMP CProviders::GetProviderInfo(long ProviderID, BSTR *FileName, BSTR *Version)
{
	if(!FileName || !Version)
		return E_POINTER;

	_bstr_t strVer = _T("N/A");
	_bstr_t strFile =_T("");

	CComPtr<IProviderData> spData;
	HRESULT hr = /*GetProvider(ProviderID, &spData);*/ S_OK;
	if(SUCCEEDED(hr))
	{
		CComBSTR bsProg;
		CProviderData* pData = dynamic_cast<CProviderData*>(spData.p);

		if(SUCCEEDED(hr))
		{
			if(pData->m_clsidProvider != GUID_NULL)
			{
				LPOLESTR szCLSID=NULL;
				StringFromCLSID (pData->m_clsidProvider, &szCLSID);
				
				_bstr_t strKey = _T("CLSID\\");
				strKey += szCLSID;
				CoTaskMemFree (szCLSID);

				EgLib::CEgRegKey key;	
				long lResult =  key.Open(HKEY_CLASSES_ROOT, strKey , KEY_READ );
				if (lResult == ERROR_SUCCESS)
				{
					EgLib::CEgRegKey keyModule;
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
								ver.GetValue(_T("FileVersion"), strVer);
							
							WIN32_FIND_DATA data;
							HANDLE hFind = FindFirstFile (strVal, &data);
							if( hFind != INVALID_HANDLE_VALUE )
							{
								FindClose(hFind);
								strFile =  data.cFileName;						
							}
								
						}
					}
					key.Close();
				}
			}
		}		
	}
	*FileName =  strFile.copy();
	*Version  =  strVer.copy();
	return hr;
}



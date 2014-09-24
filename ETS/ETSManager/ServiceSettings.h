// ServiceSettings.h : Declaration of the CServiceSettings

#ifndef __SERVICESETTINGS_H_
#define __SERVICESETTINGS_H_

#include "resource.h"       // main symbols
#include "ETSManagerCP.h"
using namespace eg_reg;

/////////////////////////////////////////////////////////////////////////////
// CServiceSettings
class ATL_NO_VTABLE CServiceSettings : 
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CServiceSettings, &CLSID_ServiceSettings>,
	public ISupportErrorInfoImpl<&IID_ISettings>,
	public IConnectionPointContainerImpl<CServiceSettings>,
	public IDispatchImpl<ISettings, &IID_ISettings, &LIBID_ETSMANAGERLib>,
	public IPersistStreamInitImpl<CServiceSettings>,
	public IPersistStorageImpl<CServiceSettings>,
	public CProxy_ISettingsEvents< CServiceSettings >

{
public:
	CServiceSettings()
		: m_bInitialized(false),
		 m_lProviderType(0L),
		 m_dwProductKey(0L),
		 m_nVolatilitySourceType(0L),
		 m_bChanged(false),
		 m_bRequiresSave(false)
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_SERVICESETTINGS)
DECLARE_CLASSFACTORY_SINGLETON(CServiceSettings)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CServiceSettings)
	COM_INTERFACE_ENTRY(ISettings)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY(IConnectionPointContainer)
	COM_INTERFACE_ENTRY(IPersistStorage)
	COM_INTERFACE_ENTRY(IPersistStreamInit)
	COM_INTERFACE_ENTRY_IMPL(IConnectionPointContainer)
END_COM_MAP()

BEGIN_CONNECTION_POINT_MAP(CServiceSettings)
CONNECTION_POINT_ENTRY(DIID__ISettingsEvents)
END_CONNECTION_POINT_MAP()

BEGIN_PROP_MAP(CServiceSettings)
	PROP_DATA_ENTRY("DatabaseConnection",     m_bsConnectionString,    VT_BSTR)
	PROP_DATA_ENTRY("PriceProvider",          m_lProviderType, VT_I4)
END_PROP_MAP()

// ISettings
public:
	STDMETHOD(InitForProfile)(/*[in]*/ BSTR bsProfileName);
	STDMETHOD(GetDBConnectionString)(/*[in]*/ LONG lType, /*[in]*/ BSTR bsName, /*[out, retval]*/  BSTR * pVal);
	STDMETHOD(get_PriceProvider)(/*[out, retval]*/ long *pVal);
	STDMETHOD(put_PriceProvider)(/*[in]*/ long newVal);
	STDMETHOD(get_DatabaseConnection)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_DatabaseConnection)(/*[in]*/ BSTR newVal);
	STDMETHOD(get_VolatilitySource)(/*[out, retval]*/ long *pVal);
	STDMETHOD(put_VolatilitySource)(/*[in]*/ long newVal);

	HRESULT FinalConstruct()
	{
		Load();
		return S_OK;
	}
	void FinalRelease()
	{
		if(m_bChanged)
			SaveProductVersion();
	}

	bool		m_bRequiresSave;
protected:

	_bstr_t		m_bsConnectionString;
	long		m_lProviderType;
	long		m_nVolatilitySourceType;
	DWORD		m_dwProductKey;
	bool		m_bChanged;

private:
	bool      m_bInitialized;


	bool LoadDatabase()
	{

		bool bRes = false;
		long lUNLen = 256;
		long lGotBytes = lUNLen;
		long lDBNameLen = 1000;
		HKEY hKey;
		HKEY hSubKey;
		LPTSTR lpPRODUCT_REG_KEY = "SOFTWARE\\Egar\\ETS\\Asp";
		LPTSTR lpDBKEY = "Database";
		LPTSTR lpUserName = (LPTSTR)malloc(lUNLen);
		LPTSTR lpUserGroup = (LPTSTR)malloc(lUNLen);
		LPTSTR lpDbName = (LPTSTR)malloc(lDBNameLen);
		ZeroMemory(lpUserName, sizeof(lpUserName));
		ZeroMemory(lpUserGroup, sizeof(lpUserGroup));
		ZeroMemory(lpDbName, sizeof(lpDbName));

		
		if (::GetUserName(lpUserName, (DWORD *)&lUNLen))
		{

			if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, lpPRODUCT_REG_KEY,  0, KEY_READ , &hKey) == ERROR_SUCCESS)
			{
				if (RegQueryValueEx(hKey, lpUserName, NULL, NULL, (LPBYTE)lpUserGroup, (LPDWORD)&lGotBytes) == ERROR_SUCCESS)
				{
					if (RegOpenKeyEx(hKey, lpDBKEY,  0,  KEY_READ , &hSubKey) == ERROR_SUCCESS)
					{
						if (RegQueryValueEx(hSubKey, lpUserGroup, NULL, NULL, (LPBYTE)lpDbName, (LPDWORD)&lDBNameLen) == ERROR_SUCCESS)
							bRes = true;
						RegCloseKey(hSubKey);  
					}
				}
				RegCloseKey(hKey);  
			}
		}
		
		USES_CONVERSION;
		if (bRes)
			m_bsConnectionString = A2W( lpDbName );
		
		

		free(lpUserName);
		free(lpUserGroup);
		free(lpDbName);
		
		return bRes;
	}

	bool LoadProvider()
	{
		CEgRegKey key;
		key.Open(HKEY_LOCAL_MACHINE,DEF_PROVIDER_SETTINGS,KEY_READ);

		DWORD dwType = 0;
		if(SUCCEEDED(GetRgDword(dwType, DEF_PROVIDER_TYPE, key)))
		{
			m_lProviderType = dwType;
			return true;
		}
		return false;
	}
	
	bool LoadVolatilitySource()
	{
		CEgRegKey key;
		key.Open(HKEY_LOCAL_MACHINE,DEF_VOLATILITYSOURCE_SETTINGS,KEY_READ);

		DWORD dwType = 0;
		if(SUCCEEDED(GetRgDword(dwType, DEF_VOLATILITYSOURCE_TYPE, key)))
		{
			m_nVolatilitySourceType = dwType;
			return true;
		}
		return false;
	}
	
	bool SaveDatabase()
	{
		bool bRet = false;
		CEgRegKey reg;
		if(m_bsConnectionString.length())
		{
			if(ERROR_SUCCESS==reg.Create(HKEY_LOCAL_MACHINE, DEF_DATABASE_SETTINGS))
			{
				if(ERROR_SUCCESS==reg.SetValue(m_bsConnectionString, DEF_CONNECTION_STRING))
				{
					bRet = true;
					m_bChanged = true;
				}
			}
		}
		else
			bRet = true;
		return bRet;
	}

	bool SaveProductVersion()
	{
		CEgRegKey reg;
		if(ERROR_SUCCESS==reg.Create(HKEY_LOCAL_MACHINE, DEF_DATABASE_SETTINGS))
		{
			reg.SetValue(m_dwProductKey, DEF_PRODUCT_KEY);
			m_bChanged = true;
			return true;
		}
		return false;
	}

	bool SaveProvider()
	{
		CEgRegKey reg;
		if(ERROR_SUCCESS==reg.Create(HKEY_LOCAL_MACHINE, DEF_PROVIDER_SETTINGS))
		{
			reg.SetValue((DWORD)m_lProviderType, DEF_PROVIDER_TYPE);
			m_bChanged = true;
			return true;
		}
		return false;
	}

	bool SaveVolatilitySource()
	{
		CEgRegKey reg;
		if(ERROR_SUCCESS==reg.Create(HKEY_LOCAL_MACHINE, DEF_VOLATILITYSOURCE_SETTINGS))
		{
			reg.SetValue((DWORD)m_nVolatilitySourceType, DEF_VOLATILITYSOURCE_TYPE);
			m_bChanged = true;
			return true;
		}
		return false;
	}

	HRESULT GetRgString(_bstr_t& bsRet, LPCTSTR szKey, CEgRegKey &rgKey)
	{

		if(HKEY(rgKey) && ERROR_SUCCESS == rgKey.QueryValue(bsRet,szKey))
		{
			return S_OK;
		}

		return Error(DEF_ERROR_REGISTRY ,IID_ISettings, E_FAIL);
	}

	HRESULT GetRgDword(DWORD& dwRet,LPCTSTR szKey,CEgRegKey &rgKey)
	{

		if(HKEY(rgKey) && ERROR_SUCCESS == rgKey.QueryValue(dwRet,szKey))
		{
			return S_OK;
		}


		return Error(DEF_ERROR_REGISTRY,IID_ISettings, E_FAIL);
	}

	HRESULT GetRgLong(long& lRet,LPCTSTR szKey,eg_reg::CEgRegKey &rgKey)
	{
		DWORD dwRet;
		HRESULT hr =  GetRgDword(dwRet,szKey,rgKey);
		if(FAILED(hr)) return hr;
		lRet = (long) dwRet;
		return S_OK;
	}

};

#endif //__SERVICESETTINGS_H_


// Settings.h : Declaration of the CSettings

#ifndef __SETTINGS_H_
#define __SETTINGS_H_

#include "resource.h"       // main symbols
#include "ETSManagerCP.h"

#include "../ETSXMLParams/Helper/XMLParamsHelper.h"

#define MAX_BUF 1024
/////////////////////////////////////////////////////////////////////////////
// CSettings
class ATL_NO_VTABLE CSettings : 
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CSettings, &CLSID_Settings>,
	public ISupportErrorInfoImpl<&IID_ISettings>,
	public IConnectionPointContainerImpl<CSettings>,
	public IDispatchImpl<ISettings2, &IID_ISettings, &LIBID_ETSMANAGERLib>,
	public IPersistStreamInitImpl<CSettings>,
	public IPersistStorageImpl<CSettings>,
	public CProxy_ISettingsEvents< CSettings >,
	public CXMLParamsHelper
{
public:
	CSettings()
		: m_bInitialized(false),
		m_lProviderType(0L),
		m_dwProductKey(0L),
		m_nVolatilitySourceType(0L),		 
		m_bChanged(false),
		m_bRequiresSave(false)
	{
	}

	DECLARE_REGISTRY_RESOURCEID(IDR_SETTINGS)
	DECLARE_CLASSFACTORY_SINGLETON(CSettings)

	DECLARE_PROTECT_FINAL_CONSTRUCT()

	BEGIN_COM_MAP(CSettings)
		COM_INTERFACE_ENTRY(IDispatch)
		COM_INTERFACE_ENTRY(ISettings)
		COM_INTERFACE_ENTRY(ISettings2)
		COM_INTERFACE_ENTRY(ISupportErrorInfo)
		COM_INTERFACE_ENTRY(IConnectionPointContainer)
		COM_INTERFACE_ENTRY(IPersistStorage)
		COM_INTERFACE_ENTRY(IPersistStreamInit)
		COM_INTERFACE_ENTRY_IMPL(IConnectionPointContainer)
	END_COM_MAP()

	BEGIN_CONNECTION_POINT_MAP(CSettings)
		CONNECTION_POINT_ENTRY(DIID__ISettingsEvents)
	END_CONNECTION_POINT_MAP()

	BEGIN_PROP_MAP(CSettings)
		PROP_DATA_ENTRY("DatabaseConnection",     m_bsConnectionString,    VT_BSTR)
		PROP_DATA_ENTRY("PriceProvider",          m_lProviderType, VT_I4)
	END_PROP_MAP()

	// ISettings
public:
	STDMETHOD(UpdateForProfile)(/*[in]*/ BSTR bsProfileName);
	STDMETHOD(InitForProfile)(/*[in]*/ BSTR bsProfileName);
	//STDMETHOD(GetDBConnectionString)(/*[in]*/ LONG lType, /*[in]*/ BSTR bsName, /*[out, retval]*/  BSTR * pVal);
	STDMETHOD(get_PriceProvider)(/*[out, retval]*/ long *pVal);
	STDMETHOD(put_PriceProvider)(/*[in]*/ long newVal);
	STDMETHOD(get_DatabaseConnection)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_DatabaseConnection)(/*[in]*/ BSTR newVal);
	STDMETHOD(get_VolatilitySource)(/*[out, retval]*/ long *pVal);
	STDMETHOD(put_VolatilitySource)(/*[in]*/ long newVal);

	HRESULT FinalConstruct()
	{
		LoadXMLParams();
		Load();
		return S_OK;
	}
	void FinalRelease()
	{
		if(m_bChanged)
			SaveProductVersion();
		SaveMainXMLParams();

	}

	bool       m_bRequiresSave;
protected:

	_bstr_t		m_bsConnectionString;
	long		m_lProviderType;
	long		m_nVolatilitySourceType;
	DWORD		m_dwProductKey;
	bool		m_bChanged;

private:
	bool      m_bInitialized;
	bool      m_bLoc;
	bool      m_bGotProfileDB;
	LPTSTR	  m_lpProfileDBKey;

	bool   Load()
	{
		if(m_bInitialized)
			return true;
		m_bGotProfileDB = false;
		bool      bCurr = false;

		m_dwProductKey = 0L;

		m_bInitialized = true;

		LoadDatabase(bCurr);
		LoadProvider(bCurr);
		LoadVolatilitySource(bCurr);

		return true;
	}

	bool LoadDatabase(bool bCurr)
	{
		CComBSTR sbsDbConnect;
		HRESULT hr = GetDbConnect(&sbsDbConnect);
		m_bsConnectionString = (BSTR)sbsDbConnect;

		return SUCCEEDED(hr) ? true : false;
	}


	bool LoadDatabaseForProfile(BSTR bstrProfileName)
	{
		bool bRes = false;
		long lUNLen = MAX_BUF;
		
		_bstr_t bsProfileName(L"");
		if(bstrProfileName)
			bsProfileName = bstrProfileName;

		if (bsProfileName.length() < 1) // Profile will be set as current user profile
		{
			TCHAR lpProfileName[MAX_BUF];
			if (!::GetUserName(lpProfileName, (DWORD *)&lUNLen))
				return false;				
			bsProfileName = lpProfileName;
		}

		_bstr_t sbsDbConnect;
		_bstr_t sbsUserGroup;
		_bstr_t sbsXMLKey = DEF_XML_KEY;
		sbsXMLKey += L"\\";
		sbsXMLKey += bsProfileName;
		if(SUCCEEDED(GetMainXMLString(sbsXMLKey, UG_XML_PARAM, &sbsUserGroup)))
		{
			if(SUCCEEDED(GetMainXMLString(DB_XML_KEY, sbsUserGroup, &sbsDbConnect)))
			{
				bRes = true;
			}
		}

		if (bRes)
		{
			m_bsConnectionString = sbsDbConnect;
		}

		return bRes;
	}

	bool LoadProvider(bool bCurr)
	{
		long nType = 0;
		if(FAILED(GetMainXMLLong(XML_PROVIDER_SETTINGS, DEF_PROVIDER_TYPE, &nType)))
			return false;

		m_lProviderType = nType;

		return true;
	}

	bool LoadVolatilitySource(bool bCurr)
	{
		long nType = 0;
		if(FAILED(GetMainXMLLong(XML_VOLATILITYSOURCE_SETTINGS, DEF_VOLATILITYSOURCE_TYPE, &nType)))
			return false;

		m_nVolatilitySourceType = nType;

		return true;
	}

	bool SaveDatabase()
	{
		//SetDbConnect(m_bsConnectionString);
		//m_bChanged = true;
		return true;
	}

	bool SaveProductVersion()
	{
		return true;
	}

	bool SaveProvider()
	{
		SetMainXMLLong(XML_PROVIDER_SETTINGS, DEF_PROVIDER_TYPE, m_lProviderType);
		SetMainXMLLong(XML_PROVIDER_SETTINGS, _T("Key"), ::GetTickCount());
		m_bChanged = true;
		return true;
	}

	bool SaveVolatilitySource()
	{
		SetMainXMLLong(XML_VOLATILITYSOURCE_SETTINGS, DEF_VOLATILITYSOURCE_TYPE, m_nVolatilitySourceType);
		m_bChanged = true;
		return true;
	}

	/*HRESULT GetRgString(_bstr_t& bsRet, LPCTSTR szKey, CEgRegKey &rgKey1, CEgRegKey &rgKey2)
	{

		if((HKEY(rgKey1) && ERROR_SUCCESS == rgKey1.QueryValue(bsRet,szKey)) ||
			(HKEY(rgKey2) && ERROR_SUCCESS == rgKey2.QueryValue(bsRet,szKey)))
		{
			return S_OK;
		}

		return Error(DEF_ERROR_REGISTRY ,IID_ISettings, E_FAIL);
	}*/

	/*HRESULT GetRgDword(DWORD& dwRet,LPCTSTR szKey,CEgRegKey &rgKey1,CEgRegKey &rgKey2)
	{

		if((HKEY(rgKey1) && ERROR_SUCCESS == rgKey1.QueryValue(dwRet,szKey)) ||
			(HKEY(rgKey2) && ERROR_SUCCESS == rgKey2.QueryValue(dwRet,szKey)))
		{
			return S_OK;
		}


		return Error(DEF_ERROR_REGISTRY,IID_ISettings, E_FAIL);
	}*/

	/*HRESULT GetRgLong(long& lRet,LPCTSTR szKey, CEgRegKey &rgKey1, CEgRegKey &rgKey2)
	{
		DWORD dwRet;
		HRESULT hr =  GetRgDword(dwRet,szKey,rgKey1,rgKey2);
		if(FAILED(hr)) return hr;
		lRet = (long) dwRet;
		return S_OK;
	}*/

};

#endif //__SETTINGS_H_

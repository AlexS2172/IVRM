// IVSettings.h : Declaration of the CIVSettings

#ifndef __IVSETTINGS_H_
#define __IVSETTINGS_H_

#include "resource.h"       // main symbols
#include "ETSManagerCP.h"

#include "../ETSXMLParams/Helper/XMLParamsHelper.h"

using namespace EgLib;
/////////////////////////////////////////////////////////////////////////////
// CIVSettings
class ATL_NO_VTABLE CIVSettings : 
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CIVSettings, &CLSID_IVSettings>,
	public ISupportErrorInfoImpl<&IID_IIVSettings>,
	public IConnectionPointContainerImpl<CIVSettings>,
	public IDispatchImpl<IIVSettings, &IID_IIVSettings, &LIBID_ETSMANAGERLib>,
	public IPersistStreamInitImpl<CIVSettings>,
	public IPersistStorageImpl<CIVSettings>,
	public CProxy_IIVSettingsEvents< CIVSettings >,
	public CXMLParamsHelper
{
public:
	CIVSettings():
	  m_bRequiresSave(FALSE),
	  m_iVolProtocol(0),
	  m_lVolServerPort(0),
	  m_lProxyPort(0)
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_IVSETTINGS)
DECLARE_CLASSFACTORY_SINGLETON(CIVSettings)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CIVSettings)
	COM_INTERFACE_ENTRY(IIVSettings)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY(IConnectionPointContainer)
	COM_INTERFACE_ENTRY(IPersistStorage)
	COM_INTERFACE_ENTRY(IPersistStreamInit)

COM_INTERFACE_ENTRY_IMPL(IConnectionPointContainer)
END_COM_MAP()
BEGIN_CONNECTION_POINT_MAP(CIVSettings)
CONNECTION_POINT_ENTRY(DIID__IIVSettingsEvents)
END_CONNECTION_POINT_MAP()

BEGIN_PROP_MAP(CIVSettings)
	PROP_DATA_ENTRY("VolServerAppPath",  m_bsVolServerAppPath, VT_BSTR)
	PROP_DATA_ENTRY("VolServerPort",     m_lVolServerPort,     VT_I4)

	PROP_DATA_ENTRY("VolPassword",		 m_bsVolPassword,      VT_BSTR)
	PROP_DATA_ENTRY("VolLogin",			 m_bsVolLogin,         VT_BSTR)
	PROP_DATA_ENTRY("VolProtocol",		 m_iVolProtocol,       VT_I4)

	PROP_DATA_ENTRY("ProxyName",		 m_bsProxyName,        VT_BSTR)
	PROP_DATA_ENTRY("ProxyPort",		 m_lProxyPort,         VT_I4)    
	PROP_DATA_ENTRY("ProxyLogin",		 m_bsProxyLogin,       VT_BSTR)
	PROP_DATA_ENTRY("ProxyPassword",     m_bsProxyPassword,    VT_BSTR)

	PROP_DATA_ENTRY("UseProxy",	         m_enUseProxy,     VT_I4)
	PROP_DATA_ENTRY("ProxyAuthType",     m_enProxyAuthType,VT_I4)

END_PROP_MAP()

	HRESULT FinalConstruct()
	{
		m_dwProductKey = 0L;
		LoadXMLParams();
		LoadInet();
		return S_OK;
	}

	HRESULT FinalRelease()
	{
		if(m_bRequiresSave)
			SaveInet();
		SaveMainXMLParams();
		return S_OK; 
	}
	
// IIVSettings
public:
	STDMETHOD(get_UseProxy)(/*[out, retval]*/ UseProxyType *pVal);
	STDMETHOD(put_UseProxy)(/*[in]*/ UseProxyType newVal);
	STDMETHOD(get_ProxyPassword)(/*[out, retval]*/ BSTR *pVal);// Proxy server password 
	STDMETHOD(put_ProxyPassword)(/*[in]*/ BSTR newVal);
	STDMETHOD(get_ProxyLogin)(/*[out, retval]*/ BSTR *pVal);   // Proxy server login 
	STDMETHOD(put_ProxyLogin)(/*[in]*/ BSTR newVal);
	STDMETHOD(get_ProxyPort)(/*[out, retval]*/ long *pVal);    // Proxy server port 
	STDMETHOD(put_ProxyPort)(/*[in]*/ long newVal);
	STDMETHOD(get_ProxyName)(/*[out, retval]*/ BSTR *pVal);    // Proxy server
	STDMETHOD(put_ProxyName)(/*[in]*/ BSTR newVal);
	STDMETHOD(get_VolProtocol)(/*[out, retval]*/ short *pVal); // http or https (SSL) protocol
	STDMETHOD(put_VolProtocol)(/*[in]*/ short newVal);
	STDMETHOD(get_VolLogin)(/*[out, retval]*/ BSTR *pVal);     // IV server login
	STDMETHOD(put_VolLogin)(/*[in]*/ BSTR newVal);
	STDMETHOD(get_VolPassword)(/*[out, retval]*/ BSTR *pVal);  // IV server password
	STDMETHOD(put_VolPassword)(/*[in]*/ BSTR newVal);
	STDMETHOD(get_VolServerPort)(/*[out, retval]*/ long *pVal);// IV server port
	STDMETHOD(put_VolServerPort)(/*[in]*/ long newVal);
	STDMETHOD(get_VolServerAppPath)(/*[out, retval]*/ BSTR *pVal); // App execution path related /
	STDMETHOD(put_VolServerAppPath)(/*[in]*/ BSTR newVal);
	STDMETHOD(get_VolServerName)(/*[out, retval]*/ BSTR *pVal);  // IV server name
	STDMETHOD(put_VolServerName)(/*[in]*/ BSTR newVal);

	STDMETHOD(get_ProxyAuthType)(/*[out, retval]*/ ProxyAuthentificationType *pVal); //Proxy authentificationType
	STDMETHOD(put_ProxyAuthType)(/*[in]*/ ProxyAuthentificationType newVal);

	BOOL  m_bRequiresSave;

private:
	_bstr_t m_bsVolServerName;
	_bstr_t m_bsVolServerAppPath;
	LONG	m_lVolServerPort;

	_bstr_t m_bsVolPassword;
	_bstr_t m_bsVolLogin;

	LONG   m_iVolProtocol;
	_bstr_t m_bsProxyName;
	LONG    m_lProxyPort;
	_bstr_t m_bsProxyLogin;
	_bstr_t m_bsProxyPassword;

	DWORD   m_dwProductKey;


	ProxyAuthentificationType m_enProxyAuthType;
	UseProxyType              m_enUseProxy;     


	/*void PutRgDword(LPCTSTR szKey,const DWORD dwValue, CEgRegKey &rgKey)
	{
		rgKey.SetValue(dwValue,szKey);
	}*/

	/*void PutRgString(LPCTSTR szKey,_bstr_t& bsValue, CEgRegKey &rgKey)
	{
		LPCTSTR pStr = (LPCTSTR)bsValue;
		if(pStr)
			rgKey.SetValue(pStr,szKey);
		else
			rgKey.SetValue(_T(""),szKey);
	}*/

	/*HRESULT GetRgString(_bstr_t& bsRet, LPCTSTR szKey,  CEgRegKey &rgKey1,  CEgRegKey &rgKey2)
	{

		if((HKEY(rgKey1) && ERROR_SUCCESS == rgKey1.QueryValue(bsRet,szKey)) ||
		   (HKEY(rgKey2) && ERROR_SUCCESS == rgKey2.QueryValue(bsRet,szKey)))
		{
			return S_OK;
		}


		return Error(DEF_ERROR_REGISTRY,IID_IIVSettings, E_FAIL);
	}*/

	/*HRESULT GetRgDword(DWORD& dwRet,LPCTSTR szKey, CEgRegKey &rgKey1, CEgRegKey &rgKey2)
	{

		if((HKEY(rgKey1) && ERROR_SUCCESS == rgKey1.QueryValue(dwRet,szKey)) ||
		   (HKEY(rgKey2) && ERROR_SUCCESS == rgKey2.QueryValue(dwRet,szKey)))
		{
			return S_OK;
		}

		return Error(DEF_ERROR_REGISTRY,IID_IIVSettings, E_FAIL);
	}*/

	/*HRESULT GetRgLong(long& lRet,LPCTSTR szKey, CEgRegKey &rgKey1, CEgRegKey &rgKey2)
	{
		DWORD dwRet;
		HRESULT hr =  GetRgDword(dwRet,szKey,rgKey1,rgKey2);
		if(FAILED(hr)) return hr;
		lRet = (long) dwRet;
		return S_OK;
	}*/

private:
	STDMETHOD(LoadInet)(void);
	STDMETHOD(SaveInet)(void);

};

#endif //__IVSETTINGS_H_

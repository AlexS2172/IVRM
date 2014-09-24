// IVSettings.cpp : Implementation of CIVSettings
#include "stdafx.h"
#include "ETSManager.h"
#include "IVSettings.h"

#define   DEF_IV_SERVERNAME      _T("VolServerName")
#define   DEF_IV_SERVERPATH      _T("VolServerAppPath")
#define   DEF_IV_SERVERPORT      _T("VolServerPort")

#define   DEF_IV_PASSWORD   _T("VolPassword")
#define   DEF_IV_LOGIN      _T("VolLogin")
#define   DEF_IV_PROTOCOL   _T("VolProtocol")
#define   DEF_IV_PROXYNAME  _T("ProxyName")
#define   DEF_IV_PROXYPORT  _T("ProxyPort")
#define   DEF_IV_PROXYAUTH  _T("ProxyAuthType")
#define   DEF_IV_PROXYUSE   _T("UseProxy")


#define   DEF_IV_PROXYLOGIN     _T("ProxyLogin")
#define   DEF_IV_PROXYPASSWORD  _T("ProxyPassword")

/////////////////////////////////////////////////////////////////////////////
// CIVSettings
#define CheckResultHR(x) { hr = x; if(FAILED(hr)) return hr;}
/////////////////////////////////////////////////////////////////////////////
// CIVSettings

STDMETHODIMP CIVSettings::get_VolServerName(BSTR *pVal)
{
	if(!pVal) return E_POINTER;

	if(*pVal)
	{
		::SysFreeString(*pVal);
	}

	*pVal = m_bsVolServerName.copy();
	return S_OK;
}

STDMETHODIMP CIVSettings::put_VolServerName(BSTR newVal)
{
	_bstr_t bs = newVal;
	if(m_bsVolServerName!=bs)
	{
		m_bsVolServerName = bs;
		
		if(FAILED(SetMainXMLString(XML_IV_SETTINGS, DEF_IV_SERVERNAME, m_bsVolServerName)))
		{
			return Error(L"Can't open xml settings file", IID_IIVSettings, E_FAIL);
		}
		
		Fire_IVSettingsChanged(enVolServerName);
		m_bRequiresSave = TRUE;
	}

	return S_OK;
}

STDMETHODIMP CIVSettings::get_VolServerAppPath(BSTR *pVal)
{
	if(!pVal) return E_POINTER;

	if(*pVal)
	{
		::SysFreeString(*pVal);
	}

	*pVal = m_bsVolServerAppPath.copy();
	return S_OK;
}

STDMETHODIMP CIVSettings::put_VolServerAppPath(BSTR newVal)
{
	_bstr_t bs = newVal;
	if(bs!= m_bsVolServerAppPath)
	{
		m_bsVolServerAppPath= bs;
		
		if(FAILED(SetMainXMLString(XML_IV_SETTINGS, DEF_IV_SERVERPATH, m_bsVolServerAppPath)))
		{
			return Error(L"Can't open xml settings file", IID_IIVSettings, E_FAIL);
		}
		
		Fire_IVSettingsChanged(enVolServerAppPath);
		m_bRequiresSave = TRUE;
	}

	return S_OK;
}

STDMETHODIMP CIVSettings::get_VolServerPort(long *pVal)
{
	if(!pVal) return E_POINTER;
	*pVal = m_lVolServerPort;
	return S_OK;
}

STDMETHODIMP CIVSettings::put_VolServerPort(long newVal)
{

	if(newVal != m_lVolServerPort)
	{
		m_lVolServerPort = newVal;
		
		if(FAILED(SetMainXMLLong(XML_IV_SETTINGS, DEF_IV_SERVERPORT, m_lVolServerPort)))
		{
			return Error(L"Can't open xml settings file", IID_IIVSettings, E_FAIL);
		}
		
		Fire_IVSettingsChanged(enVolServerPort);
		m_bRequiresSave = TRUE;
	}

	return S_OK;
}

STDMETHODIMP CIVSettings::get_VolPassword(BSTR *pVal)
{
	if(!pVal) return E_POINTER;

	if(*pVal)
	{
		::SysFreeString(*pVal);
	}

	*pVal = m_bsVolPassword.copy();
	return S_OK;
}

STDMETHODIMP CIVSettings::put_VolPassword(BSTR newVal)
{
	_bstr_t bs = newVal;
	if(bs!= m_bsVolPassword)
	{
		m_bRequiresSave = TRUE;
		m_bsVolPassword = newVal;
		
		if(FAILED(SetMainXMLString(XML_IV_SETTINGS, DEF_IV_PASSWORD, m_bsVolPassword)))
		{
			return Error(L"Can't open xml settings file", IID_IIVSettings, E_FAIL);
		}
		
		Fire_IVSettingsChanged(enVolPassword);
	}

	return S_OK;
}

STDMETHODIMP CIVSettings::get_VolLogin(BSTR *pVal)
{
	if(!pVal) return E_POINTER;

	if(*pVal)
	{
		::SysFreeString(*pVal);
	}

	*pVal = m_bsVolLogin.copy();
	return S_OK;
}

STDMETHODIMP CIVSettings::put_VolLogin(BSTR newVal)
{
	_bstr_t bs = newVal;
	if(bs!=m_bsVolLogin)
	{
		m_bRequiresSave = TRUE;
		m_bsVolLogin = bs;
		
		if(FAILED(SetMainXMLString(XML_IV_SETTINGS, DEF_IV_LOGIN, m_bsVolLogin)))
		{
			return Error(L"Can't open xml settings file", IID_IIVSettings, E_FAIL);
		}

		Fire_IVSettingsChanged(enVolLogin);
	}

	return S_OK;
}

STDMETHODIMP CIVSettings::get_VolProtocol(short *pVal)
{
	if(!pVal) return E_POINTER;
	*pVal = (short) m_iVolProtocol;

	return S_OK;
}

STDMETHODIMP CIVSettings::put_VolProtocol(short newVal)
{
	if( newVal != m_iVolProtocol)
	{
		m_bRequiresSave = TRUE;
		m_iVolProtocol = newVal;
		
		if(FAILED(SetMainXMLLong(XML_IV_SETTINGS, DEF_IV_PROTOCOL, m_iVolProtocol)))
		{
			return Error(L"Can't open xml settings file", IID_IIVSettings, E_FAIL);
		}
		
		Fire_IVSettingsChanged(enVolProtocol);
	}

	return S_OK;
}

STDMETHODIMP CIVSettings::get_ProxyName(BSTR *pVal)
{
	if(!pVal) return E_POINTER;

	if(*pVal)
	{
		::SysFreeString(*pVal);
	}

	*pVal = m_bsProxyName.copy();
	return S_OK;
}

STDMETHODIMP CIVSettings::put_ProxyName(BSTR newVal)
{
	_bstr_t bs = newVal;
	if(bs!= m_bsProxyName)
	{
		m_bRequiresSave = TRUE;
		m_bsProxyName = bs;
		
		if(FAILED(SetMainXMLString(XML_IV_SETTINGS, DEF_IV_PROXYNAME, m_bsProxyName)))
		{
			return Error(L"Can't open xml settings file", IID_IIVSettings, E_FAIL);
		}
		
		Fire_IVSettingsChanged(enProxyName);
	}

	return S_OK;
}

STDMETHODIMP CIVSettings::get_ProxyPort(long *pVal)
{
	if(!pVal) return E_POINTER;
	*pVal = m_lProxyPort;

	return S_OK;
}

STDMETHODIMP CIVSettings::put_ProxyPort(long newVal)
{
	if(newVal != m_lProxyPort)
	{
		m_bRequiresSave = TRUE;
		m_lProxyPort = newVal;
		if(FAILED(SetMainXMLLong(XML_IV_SETTINGS, DEF_IV_PROXYPORT, m_lProxyPort)))
		{
			return Error(L"Can't open xml settings file", IID_IIVSettings, E_FAIL);
		}
		
		Fire_IVSettingsChanged(enProxyPort);
	}

	return S_OK;
}

STDMETHODIMP CIVSettings::get_ProxyLogin(BSTR *pVal)
{
	if(!pVal) return E_POINTER;

	if(*pVal)
	{
		::SysFreeString(*pVal);
	}

	*pVal = m_bsProxyLogin.copy();
	return S_OK;
}

STDMETHODIMP CIVSettings::put_ProxyLogin(BSTR newVal)
{
	_bstr_t bs = newVal;
	if(bs != m_bsProxyLogin)
	{
		m_bRequiresSave = TRUE;
		m_bsProxyLogin = bs;
	
		if(FAILED(SetMainXMLString(XML_IV_SETTINGS, DEF_IV_PROXYLOGIN, m_bsProxyLogin)))
		{
			return Error(L"Can't open xml settings file", IID_IIVSettings, E_FAIL);
		}

		Fire_IVSettingsChanged(enProxyLogin);
	}

	return S_OK;
}

STDMETHODIMP CIVSettings::get_ProxyPassword(BSTR *pVal)
{
	if(!pVal) return E_POINTER;

	if(*pVal)
	{
		::SysFreeString(*pVal);
	}

	*pVal = m_bsProxyPassword.copy();
	return S_OK;
}

STDMETHODIMP CIVSettings::put_ProxyPassword(BSTR newVal)
{
	_bstr_t bs = newVal;
	if(bs!=m_bsProxyPassword)
	{
		m_bRequiresSave = TRUE;
		m_bsProxyPassword = bs;
		
		if(FAILED(SetMainXMLString(XML_IV_SETTINGS, DEF_IV_PROXYPASSWORD, m_bsProxyPassword)))
		{
			return Error(L"Can't open xml settings file", IID_IIVSettings, E_FAIL);
		}

		Fire_IVSettingsChanged(enProxyPassword);
	}

	return S_OK;
}

STDMETHODIMP CIVSettings::get_ProxyAuthType(ProxyAuthentificationType *pVal)
{
	if(!pVal) return E_POINTER;
	*pVal = m_enProxyAuthType;
	return S_OK;
}

STDMETHODIMP CIVSettings::put_ProxyAuthType(ProxyAuthentificationType newVal)
{
	if(m_enProxyAuthType != newVal)
	{
		m_enProxyAuthType = newVal;
		m_bRequiresSave = TRUE;
		
		if(FAILED(SetMainXMLLong(XML_IV_SETTINGS, DEF_IV_PROXYAUTH, m_enProxyAuthType)))
		{
			return Error(L"Can't open xml settings file", IID_IIVSettings, E_FAIL);
		}

		Fire_IVSettingsChanged(enProxyAuthType);
	}

	return S_OK;
}

STDMETHODIMP CIVSettings::LoadInet()
{
	HRESULT hr = S_OK;
	
	GetMainXMLLong(XML_IV_SETTINGS, DEF_PRODUCT_KEY, (long*)&m_dwProductKey);

	GetMainXMLString(XML_IV_SETTINGS, DEF_IV_SERVERNAME, &m_bsVolServerName);
	GetMainXMLString(XML_IV_SETTINGS, DEF_IV_SERVERPATH, &m_bsVolServerAppPath);

	GetMainXMLLong(XML_IV_SETTINGS, DEF_IV_SERVERPORT, &m_lVolServerPort);
	GetMainXMLString(XML_IV_SETTINGS, DEF_IV_PASSWORD, &m_bsVolPassword);
	GetMainXMLString(XML_IV_SETTINGS, DEF_IV_LOGIN, &m_bsVolLogin);
	GetMainXMLLong(XML_IV_SETTINGS, DEF_IV_PROTOCOL, &m_iVolProtocol);

	GetMainXMLString(XML_IV_SETTINGS, DEF_IV_PROXYNAME, &m_bsProxyName);
	GetMainXMLLong(XML_IV_SETTINGS, DEF_IV_PROXYPORT, &m_lProxyPort);
	GetMainXMLString(XML_IV_SETTINGS, DEF_IV_PROXYLOGIN, &m_bsProxyLogin);
	GetMainXMLString(XML_IV_SETTINGS, DEF_IV_PROXYPASSWORD, &m_bsProxyPassword);

	long nRet = 0;
	hr = GetMainXMLLong(XML_IV_SETTINGS, DEF_IV_PROXYAUTH, &nRet, enProxyAuthDisable);
	if(SUCCEEDED(hr) && (nRet >=0 && nRet<=1))
		m_enProxyAuthType = (ProxyAuthentificationType)nRet;
	else
		m_enProxyAuthType = enProxyAuthDisable;

	nRet = 0;
	hr = GetMainXMLLong(XML_IV_SETTINGS, DEF_IV_PROXYUSE, &nRet, enProxyDisable);
	if(SUCCEEDED(hr) && (nRet >=0 && nRet<3))
		m_enUseProxy = (UseProxyType)nRet;
	else
		m_enUseProxy = enProxyDisable;

	return hr;
}

STDMETHODIMP CIVSettings::SaveInet()
{
	SetMainXMLString(XML_IV_SETTINGS, DEF_IV_SERVERNAME, m_bsVolServerName);
	SetMainXMLString(XML_IV_SETTINGS, DEF_IV_SERVERPATH, m_bsVolServerAppPath);
	SetMainXMLLong(XML_IV_SETTINGS, DEF_IV_SERVERPORT, m_lVolServerPort);

	SetMainXMLString(XML_IV_SETTINGS, DEF_IV_PASSWORD, m_bsVolPassword);
	SetMainXMLString(XML_IV_SETTINGS, DEF_IV_LOGIN, m_bsVolLogin);
	SetMainXMLLong(XML_IV_SETTINGS, DEF_IV_PROTOCOL, m_iVolProtocol);
	SetMainXMLString(XML_IV_SETTINGS, DEF_IV_PROXYNAME, m_bsProxyName);
	SetMainXMLLong(XML_IV_SETTINGS, DEF_IV_PROXYPORT, m_lProxyPort);
	SetMainXMLLong(XML_IV_SETTINGS, DEF_IV_PROXYAUTH, m_enProxyAuthType);
	SetMainXMLLong(XML_IV_SETTINGS, DEF_IV_PROXYUSE, m_enUseProxy);

	SetMainXMLString(XML_IV_SETTINGS, DEF_IV_PROXYLOGIN, m_bsProxyLogin);
	SetMainXMLString(XML_IV_SETTINGS, DEF_IV_PROXYPASSWORD, m_bsProxyPassword);

	SetMainXMLLong(XML_IV_SETTINGS, DEF_PRODUCT_KEY, m_dwProductKey);
	
	return S_OK;
}




STDMETHODIMP CIVSettings::get_UseProxy(UseProxyType *pVal)
{
	if(!pVal) return E_POINTER;
	*pVal = m_enUseProxy;

	return S_OK;
}

STDMETHODIMP CIVSettings::put_UseProxy(UseProxyType newVal)
{
	if(m_enUseProxy != newVal)
	{
		m_enUseProxy = newVal;
		m_bRequiresSave = TRUE;
		
		if(FAILED(SetMainXMLLong(XML_IV_SETTINGS, DEF_IV_PROXYUSE, m_enUseProxy)))
		{
			return Error(L"Can't open xml settings file", IID_IIVSettings, E_FAIL);
		}

		Fire_IVSettingsChanged(enUseProxy);
	}

	return S_OK;
}

// DataFeedProviderInfo.cpp : Implementation of CDataFeedProviderInfo

#include "stdafx.h"
#include "DataFeedProviderInfo.h"


// CDataFeedProviderInfo

STDMETHODIMP CDataFeedProviderInfo::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* arr[] = 
	{
		&IID_IDataFeedProviderInfo
	};

	for (int i=0; i < sizeof(arr) / sizeof(arr[0]); i++)
	{
		if (InlineIsEqualGUID(*arr[i],riid))
			return S_OK;
	}
	return S_FALSE;
}

STDMETHODIMP CDataFeedProviderInfo::get_ProviderID (long * pVal )
{
	return S_OK;
}
STDMETHODIMP CDataFeedProviderInfo::get_Provider (enum ProviderRole Type,BSTR * pProgID )
{
	return S_OK;
}
STDMETHODIMP CDataFeedProviderInfo::get_Description (BSTR * pVal )
{
	return S_OK;
}
STDMETHODIMP CDataFeedProviderInfo::get_IsGroup (VARIANT_BOOL * pVal )
{
	return S_OK;
}
STDMETHODIMP CDataFeedProviderInfo::get_NeedLogin (VARIANT_BOOL * pVal )
{
	return S_OK;
}
STDMETHODIMP  CDataFeedProviderInfo::get_UseProxy (enum ProviderUseProxyType * pVal ) 
{
	return S_OK;
}
STDMETHODIMP  CDataFeedProviderInfo::put_UseProxy (enum ProviderUseProxyType pVal )
{
	return S_OK;
}
STDMETHODIMP  CDataFeedProviderInfo::get_ProxyName (BSTR * pVal )
{
	return S_OK;
}
STDMETHODIMP  CDataFeedProviderInfo::put_ProxyName (BSTR pVal )
{
	return S_OK;
}
STDMETHODIMP  CDataFeedProviderInfo::get_ProxyPort (long * pVal )
{
	return S_OK;
}
STDMETHODIMP  CDataFeedProviderInfo::put_ProxyPort (long pVal )
{
	return S_OK;
}
STDMETHODIMP  CDataFeedProviderInfo::get_UserAccount (BSTR * pVal )
{
	return S_OK;
}
STDMETHODIMP  CDataFeedProviderInfo::put_UserAccount (BSTR pVal )
{
	return S_OK;
}
STDMETHODIMP  CDataFeedProviderInfo::get_UserPassword (BSTR * pVal )
{
	return S_OK;
}
STDMETHODIMP  CDataFeedProviderInfo::put_UserPassword (BSTR pVal )
{
	return S_OK;
}
STDMETHODIMP  CDataFeedProviderInfo::get_ProxyAuthorization (enum ProviderProxyAuthType * pVal )
{
	return S_OK;
}
STDMETHODIMP  CDataFeedProviderInfo::put_ProxyAuthorization (enum ProviderProxyAuthType pVal )
{
	return S_OK;
}
STDMETHODIMP  CDataFeedProviderInfo::get_ProxyLogin (BSTR * pVal )
{
	return S_OK;
}
STDMETHODIMP  CDataFeedProviderInfo::put_ProxyLogin (BSTR pVal )
{
	return S_OK;
}
STDMETHODIMP  CDataFeedProviderInfo::get_ProxyPassword (BSTR * pVal )
{
	return S_OK;
}
STDMETHODIMP  CDataFeedProviderInfo::put_ProxyPassword (BSTR pVal )
{
	return S_OK;
}
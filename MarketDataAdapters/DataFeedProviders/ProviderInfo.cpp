// DataFeedProviderInfo.cpp : Implementation of CDataFeedProviderInfo

#include "stdafx.h"
#include "ProviderInfo.h"


// CDataFeedProviderInfo

STDMETHODIMP CProviderInfo::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* arr[] = 
	{
		&__uuidof(IProviderInfo)
	};

	for (int i=0; i < sizeof(arr) / sizeof(arr[0]); i++)
	{
		if (InlineIsEqualGUID(*arr[i],riid))
			return S_OK;
	}
	return S_FALSE;
}

STDMETHODIMP CProviderInfo::get_ProviderID (long * pVal )
{
	if(!pVal) 
		return E_POINTER;
	*pVal = DEF_PROVIDER_ID;
	return S_OK;
}
STDMETHODIMP CProviderInfo::get_Provider (enum ProviderRole Type,BSTR * pProgID )
{
	if(!pProgID) return E_POINTER;
	if(*pProgID) SysFreeString(*pProgID);
	HRESULT hr = S_OK;
	CLSID clType= GUID_NULL;

	switch(Type)
	{
	case enBatchPriceInfo:
		{
			clType = __uuidof( DataFeedBatchPriceInfo );
			break;
		}
	case enStructureProviderEx:
		{
			clType = __uuidof( DataFeedStructureInfo);
			break;
		}
	}

	BSTR bsData; 
	hr = ProgIDFromCLSID(clType, &bsData);

	if(SUCCEEDED(hr))
		*pProgID = SysAllocString((BSTR)bsData);
	else
		*pProgID = SysAllocString(L"");

	return hr;
}

STDMETHODIMP CProviderInfo::get_Description (BSTR * pVal )
{
	if(!pVal)
		return E_POINTER;
	if(*pVal) 
		SysFreeString(*pVal);

	*pVal = SysAllocString(DEF_PROVIDER_NAME);	

	return S_OK;
}
STDMETHODIMP CProviderInfo::get_IsGroup (VARIANT_BOOL * pVal )
{
	if(!pVal)
		return E_POINTER;
	*pVal = VARIANT_FALSE; 
	return S_OK;
}
STDMETHODIMP CProviderInfo::get_NeedLogin (VARIANT_BOOL * pVal )
{
	if(!pVal)
		return E_POINTER;
	*pVal = VARIANT_FALSE; 
	return S_OK;
}
STDMETHODIMP  CProviderInfo::get_UseProxy (enum ProviderUseProxyType * pVal ) 
{
	return S_OK;
}
STDMETHODIMP  CProviderInfo::put_UseProxy (enum ProviderUseProxyType pVal )
{
	return S_OK;
}
STDMETHODIMP  CProviderInfo::get_ProxyName (BSTR * pVal )
{
	return S_OK;
}
STDMETHODIMP  CProviderInfo::put_ProxyName (BSTR pVal )
{
	return S_OK;
}
STDMETHODIMP  CProviderInfo::get_ProxyPort (long * pVal )
{
	return S_OK;
}
STDMETHODIMP  CProviderInfo::put_ProxyPort (long pVal )
{
	return S_OK;
}
STDMETHODIMP  CProviderInfo::get_UserAccount (BSTR * pVal )
{
	return S_OK;
}
STDMETHODIMP  CProviderInfo::put_UserAccount (BSTR pVal )
{
	return S_OK;
}
STDMETHODIMP  CProviderInfo::get_UserPassword (BSTR * pVal )
{
	return S_OK;
}
STDMETHODIMP  CProviderInfo::put_UserPassword (BSTR pVal )
{
	return S_OK;
}
STDMETHODIMP  CProviderInfo::get_ProxyAuthorization (enum ProviderProxyAuthType * pVal )
{
	return S_OK;
}
STDMETHODIMP  CProviderInfo::put_ProxyAuthorization (enum ProviderProxyAuthType pVal )
{
	return S_OK;
}
STDMETHODIMP  CProviderInfo::get_ProxyLogin (BSTR * pVal )
{
	return S_OK;
}
STDMETHODIMP  CProviderInfo::put_ProxyLogin (BSTR pVal )
{
	return S_OK;
}
STDMETHODIMP  CProviderInfo::get_ProxyPassword (BSTR * pVal )
{
	return S_OK;
}
STDMETHODIMP  CProviderInfo::put_ProxyPassword (BSTR pVal )
{
	return S_OK;
}
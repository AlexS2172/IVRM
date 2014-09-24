// ProviderInfo.cpp : Implementation of CProviderInfo
#include "stdafx.h"
#include "MyTrackProviders.h"
#include "ProviderInfo.h"
using namespace eg_reg;
/////////////////////////////////////////////////////////////////////////////
// CProviderInfo


STDMETHODIMP CProviderInfo::get_ProviderID(long *pVal)
{
	if(!pVal) 
		return E_POINTER;
	*pVal = DEF_PROVIDER_ID;
	return S_OK;
}

STDMETHODIMP CProviderInfo::get_Provider(ProviderRole type, BSTR *pProgID)
{
	if(!pProgID) return E_POINTER;

	CLSID clType= GUID_NULL;
	HRESULT hr = S_OK;

	switch(type)
	{
	case enStructureProvider:
		clType = CLSID_MyTrackStructureProvider;
		break;
	case enStructureInfo:
		clType = CLSID_MyTrackStructureInfo;
		break;
	case enPriceProvider: 
		clType = CLSID_MyTrackPriceProvider;
		break;
	case enPriceInfo:
		clType = CLSID_MyTrackPriceInfo;
		break;
	case enPriceInfoWithNotify:
		clType = CLSID_MyTrackPriceInfoWithNotify;
		break;
	}

	BSTR bsData;
	hr = ProgIDFromCLSID(clType, &bsData);

	if(SUCCEEDED(hr))
		*pProgID = SysAllocString((BSTR)bsData);
	else
		*pProgID = SysAllocString(L"");

	return hr;

}

STDMETHODIMP CProviderInfo::get_Description(BSTR *pVal)
{
	if(!pVal)
		return E_POINTER;
	*pVal = SysAllocString(DEF_PROVIDER_NAME);	

	return S_OK;
}

STDMETHODIMP CProviderInfo::get_IsGroup(VARIANT_BOOL *pVal)
{
	if(!pVal)
		return E_POINTER;
	*pVal = VARIANT_FALSE; 
	return S_OK;
}

STDMETHODIMP CProviderInfo::get_NeedLogin(VARIANT_BOOL *pVal)
{
	if(!pVal)
		return E_POINTER;
	*pVal = VARIANT_TRUE; 
	return S_OK;
}

STDMETHODIMP CProviderInfo::get_UseProxy(ProviderUseProxyType* pVal)
{
	if(!pVal)
		return E_POINTER;

	CEgRegKey regLoc;
	CEgRegKey regCur;
	regLoc.Open(HKEY_LOCAL_MACHINE, DEF_PATH, KEY_READ);
	regCur.Open(HKEY_CURRENT_USER,  DEF_PATH, KEY_READ);
	DWORD dwValue = 0;
	GetRgDword(dwValue, _T("UseProxy"), regCur, regLoc);

	switch(dwValue)
	{
	case 1:
		*pVal = enProxyDefault;
		break;
	case 2:
		*pVal = enProxyCustom;
		break;
	default:
		*pVal = enProxyDisable;
	}

	 (ProviderUseProxyType)dwValue;
	return S_OK;
}

STDMETHODIMP CProviderInfo::put_UseProxy(ProviderUseProxyType newVal)
{
	CEgRegKey regCur;
	regCur.Create(HKEY_CURRENT_USER,  DEF_PATH);
	long lVal = 0;

	switch(newVal)
	{
	case enProxyDefault:
		lVal = 1;
		break;
	case enProxyCustom:
		lVal = 2;
		break;
	}

	PutRgDword(_T("UseProxy"), (DWORD)lVal, regCur);
	return S_OK;
}

STDMETHODIMP CProviderInfo::get_ProxyName(BSTR* pVal)
{
	if(!pVal)
		return E_POINTER;

	CEgRegKey regLoc;
	CEgRegKey regCur;

	regLoc.Open(HKEY_LOCAL_MACHINE, DEF_PATH, KEY_READ);
	regCur.Open(HKEY_CURRENT_USER,  DEF_PATH, KEY_READ);
	_bstr_t bsValue = _T("");
	GetRgString(bsValue, _T("Proxy"), regCur, regLoc);
	*pVal = bsValue.copy();

	return S_OK;
}

STDMETHODIMP CProviderInfo::put_ProxyName(BSTR newVal)
{
	CEgRegKey regCur;
	regCur.Create(HKEY_CURRENT_USER,  DEF_PATH);
	PutRgString(_T("Proxy"), _bstr_t(newVal), regCur);
	return S_OK;
}

STDMETHODIMP CProviderInfo::get_ProxyPort(LONG* pVal)
{
	if(!pVal)
		return E_POINTER;

	CEgRegKey regLoc;
	CEgRegKey regCur;
	regLoc.Open(HKEY_LOCAL_MACHINE, DEF_PATH, KEY_READ);
	regCur.Open(HKEY_CURRENT_USER,  DEF_PATH, KEY_READ);
	DWORD dwValue = 0;
	GetRgDword(dwValue, _T("Port"), regCur, regLoc);
	*pVal = dwValue;
	return S_OK;
}

STDMETHODIMP CProviderInfo::put_ProxyPort(LONG newVal)
{
	CEgRegKey regCur;
	regCur.Create(HKEY_CURRENT_USER,  DEF_PATH);
	PutRgDword(_T("Port"), (DWORD)newVal, regCur);

	return S_OK;
}

STDMETHODIMP CProviderInfo::get_UserAccount(BSTR* pVal)
{
	if(!pVal)
		return E_POINTER;

	CEgRegKey regLoc;
	CEgRegKey regCur;

	regLoc.Open(HKEY_LOCAL_MACHINE, DEF_PATH, KEY_READ);
	regCur.Open(HKEY_CURRENT_USER,  DEF_PATH, KEY_READ);
	_bstr_t bsValue = _T("");
	GetRgString(bsValue, _T("Account"), regCur, regLoc);
	*pVal = bsValue.copy();

	return S_OK;
}

STDMETHODIMP CProviderInfo::put_UserAccount(BSTR newVal)
{
	CEgRegKey regCur;
	regCur.Create(HKEY_CURRENT_USER,  DEF_PATH);
	PutRgString(_T("Account"), _bstr_t(newVal), regCur);
	return S_OK;
}

STDMETHODIMP CProviderInfo::get_UserPassword(BSTR* pVal)
{
	if(!pVal)
		return E_POINTER;

	CEgRegKey regLoc;
	CEgRegKey regCur;

	regLoc.Open(HKEY_LOCAL_MACHINE, DEF_PATH, KEY_READ);
	regCur.Open(HKEY_CURRENT_USER,  DEF_PATH, KEY_READ);
	_bstr_t bsValue = _T("");
	GetRgString(bsValue, _T("Password"), regCur, regLoc);
	*pVal = bsValue.copy();

	return S_OK;
}

STDMETHODIMP CProviderInfo::put_UserPassword(BSTR newVal)
{
	CEgRegKey regCur;
	regCur.Create(HKEY_CURRENT_USER,  DEF_PATH);
	PutRgString(_T("Password"), _bstr_t(newVal), regCur);
	return S_OK;
}

STDMETHODIMP CProviderInfo::get_ProxyAuthorization(ProviderProxyAuthType* pVal)
{
	if(!pVal)
		return E_POINTER;
	*pVal = enProxyAuthNone;
	return S_OK;
}

STDMETHODIMP CProviderInfo::put_ProxyAuthorization(ProviderProxyAuthType newVal)
{
	return S_OK;
}

STDMETHODIMP CProviderInfo::get_ProxyLogin(BSTR* pVal)
{
	if(!pVal)
		return E_POINTER;
	*pVal = SysAllocString(L"");
	return S_OK;
}

STDMETHODIMP CProviderInfo::put_ProxyLogin(BSTR newVal)
{
	return S_OK;
}

STDMETHODIMP CProviderInfo::get_ProxyPassword(BSTR* pVal)
{
	if(!pVal)
		return E_POINTER;
	*pVal = SysAllocString(L"");
	return S_OK;
}

STDMETHODIMP CProviderInfo::put_ProxyPassword(BSTR newVal)
{
	return S_OK;
}

// ProviderInfo.cpp : Implementation of CProviderInfo
#include "stdafx.h"
#include "ATFDataProvider.h"
#include "ProviderInfo.h"

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
	HRESULT hr =S_OK;

	CLSID clType= GUID_NULL;

	switch(type)
	{
	case enStructureProvider:
		clType = CLSID_ATFStructureProvider;
		break;
	case enStructureInfo:
		clType = CLSID_ATFStructureInfo;
		break;
	case enPriceProvider: 
		clType = CLSID_ATFPriceProvider;
		break;
	case enPriceInfo:
		clType = CLSID_ATFPriceInfo;
		break;
	case enPriceInfoWithNotify:
		clType = CLSID_ATFPriceInfoWithNotify;
		break;
	}
	BSTR bsData; 
	hr = ProgIDFromCLSID(clType, &bsData);

	if(SUCCEEDED(hr))
		*pProgID = ::SysAllocString(bsData);
	else
		*pProgID = ::SysAllocString(L"");

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
	*pVal = VARIANT_TRUE; 
	return S_OK;
}

STDMETHODIMP CProviderInfo::get_NeedLogin(VARIANT_BOOL *pVal)
{
	if(!pVal)
		return E_POINTER;
	*pVal = VARIANT_FALSE; 
	return S_OK;
}

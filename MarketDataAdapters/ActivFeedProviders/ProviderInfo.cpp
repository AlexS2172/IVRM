// ProviderInfo.cpp : Implementation of CProviderInfo

#include "stdafx.h"
#include "ProviderInfo.h"


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
	if(*pProgID) SysFreeString(*pProgID);
	HRESULT hr = S_OK;
	CLSID clType= GUID_NULL;

		switch(type)
		{
//		case enStructureProvider:
//			clType = CLSID_HyperFeedStructureProvider;
//			break;
		case enBatchPriceInfo:
			clType = __uuidof( ActivFeedBatchPriceInfo );
			break;
		case enStructureProviderEx:
			clType = __uuidof(ActivFeedStructureInfo);
			break;
//		case enPriceProvider: 
//			clType = CLSID_HyperFeedPriceProvider;
//			break;
//		case enPriceInfo:
//			clType = CLSID_HyperFeedPriceInfo;
//			break;
//		case enPriceInfoWithNotify:
//			clType = CLSID_HyperFeedPriceInfoWithNotify;
//			break;
//		case enBatchPriceInfo:
//			clType = CLSID_HyperFeedBatchPriceInfo;
//			break;
//		case enStructureProviderEx:
//			clType = CLSID_HyperFeedStructureProviderEx;
//			break;
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
	if(*pVal) 
		SysFreeString(*pVal);

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
	*pVal = VARIANT_FALSE; 
	return S_OK;
}
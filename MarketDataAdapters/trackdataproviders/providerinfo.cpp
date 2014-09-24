// ProviderInfo.cpp : Implementation of CProviderInfo
#include "stdafx.h"
#include "TrackDataProviders.h"
#include "ProviderInfo.h"

/////////////////////////////////////////////////////////////////////////////
// CProviderInfo


STDMETHODIMP CProviderInfo::get_ProviderID(long *pVal)
{
	if(!pVal) 
		return E_POINTER;
	*pVal = 2L;
	return S_OK;
}

STDMETHODIMP CProviderInfo::get_Provider(ProviderRole type, BSTR *pProgID)
{
	if(!pProgID) return E_POINTER;
	HRESULT hr = S_OK;

	CLSID clType= GUID_NULL;

	switch(type)
	{
	case enStructureProvider:
		clType = CLSID_TrackDataStructureProvider;
		break;
	case enStructureInfo:
		clType = CLSID_TrackDataStructureInfo;
		break;
	case enPriceProvider: 
		clType = CLSID_TrackDataPriceProvider;
		break;
	case enPriceInfo:
		clType = CLSID_TrackDataPriceInfo;
		break;
	case enPriceInfoWithNotify:
		clType = CLSID_TrackDataPriceInfo;
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
	*pVal = SysAllocString(L"TrackData Price Provider (Sim)");	

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

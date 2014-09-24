// FixProvidersC.cpp : Implementation of CFixProviders
#include "stdafx.h"
#include "FixProviders.h"
#include "FixProvidersC.h"

/////////////////////////////////////////////////////////////////////////////
// CFixProviders

STDMETHODIMP CFixProviders::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* arr[] =
	{
		&IID_IFixProviders
	};
	for (int i=0; i < sizeof(arr) / sizeof(arr[0]); i++)
	{
		if (::InlineIsEqualGUID(*arr[i],riid))
			return E_NOTIMPL;
	}
	return S_FALSE;
}

STDMETHODIMP CFixProviders::get_Specifics(long FixProviderId, IFixProviderSpecifics **pVal)
{
	// TODO: Add your implementation code here

	return E_NOTIMPL;
}

STDMETHODIMP CFixProviders::GetFixProvider(long FixProviderId, IFixProviderSpecifics ** ppFixProvider)
{
	return get_Specifics(FixProviderId, ppFixProvider);
}

STDMETHODIMP CFixProviders::GetFixProviderInfo(long FixProviderId, BSTR * FileName, BSTR * Version)
{
	return E_NOTIMPL;
}

STDMETHODIMP CFixProviders::Initialize()
{
	if(m_bInitialized)
		return S_OK;

	Clear();

	HRESULT hr = m_spCache.CoCreateInstance(CLSID_FixProvidersCache);
	if(FAILED(hr))
		return hr;

	m_spCache->Attach();
	long lCount = 0;
	m_spCache->get_Count(&lCount);

	for(long c = 1; c <= lCount; c++)
	{
		IFixProviderSpecifics* pData = 0;
		m_spCache->get_Item(c, &pData);
		m_coll.push_back(pData);
	}

	if(SUCCEEDED(hr))
		m_bInitialized = true;

	return hr;
}

void CFixProviders::Clear()
{
	collection::iterator iter;

	for(iter = m_coll.begin(); iter!=m_coll.end(); iter++)
		(*iter)->Release();

	m_coll.clear();
}

HRESULT CFixProviders::FinalConstruct()
{
	m_bInitialized = false;
	m_spCache = 0;

	return S_OK;
}

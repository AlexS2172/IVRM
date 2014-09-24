// Source.cpp : Implementation of CSource

#include "stdafx.h"
#include "Source.h"
//#include "vsdatastorage.h"

// CSource

/*
STDMETHODIMP CSource::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* arr[] = 
	{
		&IID_IVolatilitySource
	};

	for (int i=0; i < sizeof(arr) / sizeof(arr[0]); i++)
	{
		if (InlineIsEqualGUID(*arr[i],riid))
			return S_OK;
	}
	return S_FALSE;
}*/

/*

DWORD CSource::GetCurrentSource()
{
	DWORD ret = -1;
	try
	{
		 CEgRegKey reg;
		_bstr_t str(_T("SOFTWARE\\Egar\\ETS\\VolatilitySource"));
		if(reg.Open(HKEY_LOCAL_MACHINE,str,KEY_READ) != ERROR_SUCCESS)
			return -1; //Default source

		LONG id = reg.QueryDWORDValue(_T("Type"), ret);
	}catch (...)
	{
		return -1;
	}
	return ret;
}
*/

HRESULT CSource::Initialize(CLSID clsid)
{
	if(m_spVolatilitySource != NULL)
		return S_OK;

	HRESULT hr = m_spVolatilitySource.CoCreateInstance(clsid);
	if(SUCCEEDED(hr))
		hr = Advise();

/*	another version
	CComPtr<IUnknown> spUnk;
	HRESULT hr = spUnk.CoCreateInstance(clsid);
	if(SUCCEEDED(hr))
	{
		hr = spUnk.QueryInterface(&m_spVolatilitySource);
		if(SUCCEEDED(hr))
			hr = Advise();
	}
	spUnk = NULL;
*/

	if(FAILED(hr))
		m_spVolatilitySource = NULL;

	return hr;
}

/*

HRESULT CSource::CreateSource()
{
	if(m_spVolatilitySource != NULL)
		return S_OK;
	
	CLSID clsid = CLSID_NULL;
	HRESULT hr = g_VSDataStorage.GetCurrentSource(&clsid);
	if(SUCCEEDED(hr)&&(clsid!= CLSID_NULL))
		return m_spVolatilitySource.CreateInstance(clsid);
	
	return hr;
}*/


HRESULT CSource::Advise()
{
	if(m_spVolatilitySource != NULL)
		return m_Notify.Initialize(m_spVolatilitySource);
		//return DispEventAdvise(m_spVolatilitySource);
	else
		return Error(_T("Unable to create underlying volatility source object."),__uuidof(IVSSymbolVolatility), E_FAIL);
}

HRESULT CSource::Unadvise()
{	
	m_Notify.Destroy();
	return S_OK;
		//return DispEventUnadvise(m_spVolatilitySource);	
}


_ATL_FUNC_INFO CVSNotify::m_ArrivedVolatilityChanged = {CC_STDCALL, VT_EMPTY, 1, {VT_BSTR} };

STDMETHODIMP CVSNotify::OnVolatilityChanged(BSTR Symbol)
{
	return m_pClient->Fire_VolatilityChanged(Symbol);
};

HRESULT CVSNotify::Initialize(IVolatilitySource* pClient)
{
	HRESULT hr = E_FAIL;
	if(pClient)
	{
		m_spEvent = pClient;
		hr = DispEventAdvise(m_spEvent);
	}
	return hr;
}

void CVSNotify::Destroy()
{
	if (m_spEvent)
	{
		DispEventUnadvise (m_spEvent);
		m_spEvent = NULL;
	}

}

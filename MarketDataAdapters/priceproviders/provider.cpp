// Provider.cpp : Implementation of CProviderData
#include "stdafx.h"
#include "PriceProviders.h"
#include "Provider.h"

/////////////////////////////////////////////////////////////////////////////
// CProviderData
STDMETHODIMP CProviderData::get_UseProxy(ProviderUseProxyType* pVal)
{
	IProviderInfoPtr spInfo;
	if(!pVal)
		return E_POINTER;

	HRESULT hr = spInfo.CreateInstance(m_clsidProvider);
	if(SUCCEEDED(hr))
	{
		hr = spInfo->get_UseProxy(pVal);
		if(*pVal == enProxyNone)
		{
			m_bProxy = false;
			m_bProxyAuth = false;
		}
		else
			m_bProxy = true;
	}
	return hr;
}

STDMETHODIMP CProviderData::put_UseProxy(ProviderUseProxyType newVal)
{
	IProviderInfoPtr spInfo;
	HRESULT hr = spInfo.CreateInstance(m_clsidProvider);
	if(SUCCEEDED(hr))
	{
		hr = spInfo->put_UseProxy(newVal);
		if(newVal == enProxyNone)
		{
			m_bProxy = false;
			m_bProxyAuth = false;
		}
		else
			m_bProxy = true;
	}

	return hr;
}

STDMETHODIMP CProviderData::get_ProxyName(BSTR* pVal)
{
	if(!pVal)
		return E_POINTER;
	if(!m_bProxy)
	{
		*pVal = SysAllocString(L"");
		return S_OK;
	}

	IProviderInfoPtr spInfo;
	HRESULT hr = spInfo.CreateInstance(m_clsidProvider);
	if(SUCCEEDED(hr))
		hr = spInfo->get_ProxyName(pVal);
	return hr;
}

STDMETHODIMP CProviderData::put_ProxyName(BSTR newVal)
{
	if(!m_bProxy)
		return S_OK;

	IProviderInfoPtr spInfo;
	HRESULT hr = spInfo.CreateInstance(m_clsidProvider);
	if(SUCCEEDED(hr))
		hr = spInfo->put_ProxyName(newVal);
	return hr;
}

STDMETHODIMP CProviderData::get_ProxyPort(LONG* pVal)
{
	if(!pVal)
		return E_POINTER;
	if(!m_bProxy)
	{
		*pVal = 0L;
		return S_OK;
	}

	IProviderInfoPtr spInfo;
	HRESULT hr = spInfo.CreateInstance(m_clsidProvider);
	if(SUCCEEDED(hr))
		hr = spInfo->get_ProxyPort(pVal);
	return hr;

}
STDMETHODIMP CProviderData::put_ProxyPort(LONG newVal)
{
	if(!m_bProxy)
		return S_OK;

	IProviderInfoPtr spInfo;
	HRESULT hr = spInfo.CreateInstance(m_clsidProvider);
	if(SUCCEEDED(hr))
		hr = spInfo->put_ProxyPort(newVal);
	return hr;
}

STDMETHODIMP CProviderData::get_UserAccount(BSTR* pVal)
{
	if(!pVal)
		return E_POINTER;

	if(!m_bLogin)
	{
		*pVal = SysAllocString(L"");
		return S_OK;
	}

	IProviderInfoPtr spInfo;
	HRESULT hr = spInfo.CreateInstance(m_clsidProvider);
	if(SUCCEEDED(hr))
		hr = spInfo->get_UserAccount(pVal);
	return hr;
}

STDMETHODIMP CProviderData::put_UserAccount(BSTR newVal)
{
	if(!m_bLogin)
		return S_OK;

	IProviderInfoPtr spInfo;
	HRESULT hr = spInfo.CreateInstance(m_clsidProvider);
	if(SUCCEEDED(hr))
		hr = spInfo->put_UserAccount(newVal);
	return hr;

}
STDMETHODIMP CProviderData::get_UserPassword(BSTR* pVal)
{
	if(!pVal)
		return E_POINTER;

	if(!m_bLogin)
	{
		*pVal = SysAllocString(L"");
		return S_OK;
	}

	IProviderInfoPtr spInfo;
	HRESULT hr = spInfo.CreateInstance(m_clsidProvider);
	if(SUCCEEDED(hr))
		hr = spInfo->get_UserPassword(pVal);
	return hr;
}

STDMETHODIMP CProviderData::put_UserPassword(BSTR newVal)
{
	if(!m_bLogin)
		return S_OK;

	IProviderInfoPtr spInfo;
	HRESULT hr = spInfo.CreateInstance(m_clsidProvider);
	if(SUCCEEDED(hr))
		hr = spInfo->put_UserPassword(newVal);
	return hr;

}

STDMETHODIMP CProviderData::get_ProxyAuthorization(ProviderProxyAuthType* pVal)
{
	if(!pVal)
		return E_POINTER;

	if(!m_bProxy)
	{
		*pVal = enProxyAuthNone;
		return S_OK;
	}

	IProviderInfoPtr spInfo;
	HRESULT hr = spInfo.CreateInstance(m_clsidProvider);
	if(SUCCEEDED(hr))
	{
		hr = spInfo->get_ProxyAuthorization(reinterpret_cast<ProviderProxyAuthType*>(pVal));
		if(SUCCEEDED(hr) && *pVal == enProxyAuthNone)
			m_bProxyAuth = false;
		else
			m_bProxyAuth = true;

	}
	return hr;
}
STDMETHODIMP CProviderData::put_ProxyAuthorization(ProviderProxyAuthType newVal)
{
	if(!m_bProxy)
		return S_OK;

	IProviderInfoPtr spInfo;
	HRESULT hr = spInfo.CreateInstance(m_clsidProvider);
	if(SUCCEEDED(hr))
		hr = spInfo->put_ProxyAuthorization(static_cast<ProviderProxyAuthType>(newVal));

	if(SUCCEEDED(hr))
	{
		if(newVal == enProxyAuthNone)
			m_bProxyAuth = false;
		else
			m_bProxyAuth = true;
	}

	return hr;

}
STDMETHODIMP CProviderData::get_ProxyLogin(BSTR* pVal)
{
	if(!pVal)
		return E_POINTER;

	if(!m_bProxy || !m_bProxyAuth)
	{
		*pVal = SysAllocString(L"");
		return S_OK;
	}
	IProviderInfoPtr spInfo;
	HRESULT hr = spInfo.CreateInstance(m_clsidProvider);
	if(SUCCEEDED(hr))
		hr = spInfo->get_ProxyLogin(pVal);
	return hr;
}
STDMETHODIMP CProviderData::put_ProxyLogin(BSTR newVal)
{
	if(!m_bProxy || !m_bProxyAuth)	return S_OK;

	IProviderInfoPtr spInfo;
	HRESULT hr = spInfo.CreateInstance(m_clsidProvider);
	if(SUCCEEDED(hr))
		hr = spInfo->put_ProxyLogin(newVal);
	return hr;

}
STDMETHODIMP CProviderData::get_ProxyPassword(BSTR* pVal)
{
	if(!pVal)
		return E_POINTER;

	if(!m_bProxy || !m_bProxyAuth)
	{
		*pVal = SysAllocString(L"");
		return S_OK;
	}

	IProviderInfoPtr spInfo;
	HRESULT hr = spInfo.CreateInstance(m_clsidProvider);
	if(SUCCEEDED(hr))
		hr = spInfo->get_ProxyPassword(pVal);
	return hr;
}
STDMETHODIMP CProviderData::put_ProxyPassword(BSTR newVal)
{
	if(!m_bProxy || !m_bProxyAuth)	return S_OK;

	IProviderInfoPtr spInfo;
	HRESULT hr = spInfo.CreateInstance(m_clsidProvider);
	if(SUCCEEDED(hr))
		hr = spInfo->put_ProxyPassword(newVal);
	return hr;
}


STDMETHODIMP CProviderData::get_ProviderID(long *pVal)
{
	if(!pVal)
		return E_POINTER;
	*pVal = m_lID;	

	return S_OK;
}

STDMETHODIMP CProviderData::get_Description(BSTR *pVal)
{
	if(!pVal)
		return E_POINTER;

	*pVal = m_bsDescription.Copy();
	return S_OK;
}


STDMETHODIMP CProviderData::get_ProgID(ProviderRole role, BSTR *pVal)
{
	if(!pVal)
		return E_POINTER;

	HRESULT hr =Initialize();
	if(FAILED(hr))
		return hr;
		

	*pVal = m_Progs[role].copy();
	return S_OK;
}


STDMETHODIMP CProviderData::get_IsGroupRequestSupported(VARIANT_BOOL *pVal)
{
	if(!pVal)
		return E_POINTER;

	HRESULT hr =Initialize();
	if(FAILED(hr))
		return hr;
	
	*pVal = m_bGroup;
	return S_OK;
}

STDMETHODIMP CProviderData::get_NeedLogin(VARIANT_BOOL *pVal)
{
	
	if(!pVal)
		return E_POINTER;
	HRESULT hr =Initialize();
	if(FAILED(hr))
		return hr;

	*pVal = m_bLogin;
	return S_OK;
}

HRESULT CProviderData::Initialize()
{
	HRESULT hr = S_OK;
	if(m_bInitialized)
		return hr;

	try
	{
		IProviderInfoPtr spInfo;
		HRESULT hr = spInfo.CreateInstance(m_clsidProvider);
		if(SUCCEEDED(hr))
		{
			__CHECK_HRESULT3(spInfo->get_ProviderID(&m_lID));
			__CHECK_HRESULT3(spInfo->get_Description((BSTR*)&m_bsDescription));
			__CHECK_HRESULT3(spInfo->get_IsGroup(&m_bGroup));
			__CHECK_HRESULT3(spInfo->get_NeedLogin(&m_bLogin));

			BSTR  bsData;
			spInfo->get_Provider(enStructureProvider,&bsData);
			m_Progs[enStructureProvider] = bsData;
			SysFreeString(bsData);			

			spInfo->get_Provider(enStructureInfo,&bsData);
			m_Progs[enStructureInfo] = bsData;
			SysFreeString(bsData);			

			spInfo->get_Provider(enPriceProvider,&bsData);
			m_Progs[enPriceProvider] = bsData;
			SysFreeString(bsData);			

			spInfo->get_Provider(enPriceInfo,&bsData);
			m_Progs[enPriceInfo] = bsData;
			SysFreeString(bsData);	

			spInfo->get_Provider(enPriceInfoWithNotify,&bsData);
			m_Progs[enPriceInfoWithNotify] = bsData;
			SysFreeString(bsData);	

			spInfo->get_Provider(enBatchPriceInfo,&bsData);
			m_Progs[enBatchPriceInfo] = bsData;
			SysFreeString(bsData);	

			spInfo->get_Provider(enStructureProviderEx,&bsData);
			m_Progs[enStructureProviderEx] = bsData;
			SysFreeString(bsData);			

			m_bInitialized = true;

		}
	}
	catch(_com_error& e)
	{
		hr = e.Error();
	}
	return hr;
}

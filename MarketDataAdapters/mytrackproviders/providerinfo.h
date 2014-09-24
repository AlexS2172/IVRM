// ProviderInfo.h : Declaration of the CProviderInfo

#ifndef __PROVIDERINFO_H_
#define __PROVIDERINFO_H_

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CProviderInfo
class ATL_NO_VTABLE CProviderInfo : 
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CProviderInfo, &CLSID_ProviderInfo>,
	public ISupportErrorInfoImpl<&IID_IProviderInfo>,
	public IDispatchImpl<IProviderInfo, &IID_IProviderInfo, &LIBID_MYTRACKPROVIDERSLib>
{
public:
	CProviderInfo()
	{
	}

DECLARE_CLASSFACTORY_SINGLETON(CProviderInfo)
DECLARE_REGISTRY_RESOURCEID(IDR_PROVIDERINFO)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CProviderInfo)
	COM_INTERFACE_ENTRY(IProviderInfo)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
END_COM_MAP()

// IProviderInfo
public:
	STDMETHOD(get_NeedLogin)(/*[out, retval]*/ VARIANT_BOOL *pVal);
	STDMETHOD(get_IsGroup)(/*[out, retval]*/ VARIANT_BOOL *pVal);
	STDMETHOD(get_Description)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(get_Provider)(/*[in]*/ ProviderRole type, /*[out]*/ BSTR* pProgID);
	STDMETHOD(get_ProviderID)(/*[out, retval]*/ long *pVal);
	STDMETHOD(get_UseProxy)(ProviderUseProxyType* pVal);
	STDMETHOD(put_UseProxy)(ProviderUseProxyType newVal);
	STDMETHOD(get_ProxyName)(BSTR* pVal);
	STDMETHOD(put_ProxyName)(BSTR newVal);
	STDMETHOD(get_ProxyPort)(LONG* pVal);
	STDMETHOD(put_ProxyPort)(LONG newVal);
	STDMETHOD(get_UserAccount)(BSTR* pVal);
	STDMETHOD(put_UserAccount)(BSTR newVal);
	STDMETHOD(get_UserPassword)(BSTR* pVal);
	STDMETHOD(put_UserPassword)(BSTR newVal);
	STDMETHOD(get_ProxyAuthorization)(ProviderProxyAuthType* pVal);
	STDMETHOD(put_ProxyAuthorization)(ProviderProxyAuthType newVal);
	STDMETHOD(get_ProxyLogin)(BSTR* pVal);
	STDMETHOD(put_ProxyLogin)(BSTR newVal);
	STDMETHOD(get_ProxyPassword)(BSTR* pVal);
	STDMETHOD(put_ProxyPassword)(BSTR newVal);

private:
	void PutRgDword(LPCTSTR szKey,const DWORD dwValue,eg_reg::CEgRegKey &rgKey)
	{
		rgKey.SetValue(dwValue,szKey);
	}

	void PutRgString(LPCTSTR szKey,_bstr_t& bsValue,eg_reg::CEgRegKey &rgKey)
	{
		LPCTSTR pStr = (LPCTSTR)bsValue;
		if(pStr)
			rgKey.SetValue(pStr,szKey);
		else
			rgKey.SetValue(_T(""),szKey);
	}

	bool GetRgString(_bstr_t& bsRet, LPCTSTR szKey, eg_reg::CEgRegKey &rgKey1, eg_reg::CEgRegKey &rgKey2)
	{

		if((HKEY(rgKey1) && ERROR_SUCCESS == rgKey1.QueryValue(bsRet,szKey)) ||
		   (HKEY(rgKey2) && ERROR_SUCCESS == rgKey2.QueryValue(bsRet,szKey)))
		{
			return true;
		}


		return false;
	}

	bool GetRgDword(DWORD& dwRet,LPCTSTR szKey,eg_reg::CEgRegKey &rgKey1,eg_reg::CEgRegKey &rgKey2)
	{

		if((HKEY(rgKey1) && ERROR_SUCCESS == rgKey1.QueryValue(dwRet,szKey)) ||
		   (HKEY(rgKey2) && ERROR_SUCCESS == rgKey2.QueryValue(dwRet,szKey)))
		{
			return true;
		}

		return false;
	}

	bool GetRgLong(long& lRet,LPCTSTR szKey,eg_reg::CEgRegKey &rgKey1,eg_reg::CEgRegKey &rgKey2)
	{
		DWORD dwRet;
		HRESULT hr =  GetRgDword(dwRet,szKey,rgKey1,rgKey2);
		if(FAILED(hr)) return false;
		lRet = (long) dwRet;
		return true;
	}

};

#endif //__PROVIDERINFO_H_

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
	public IDispatchImpl<IProviderInfo, &IID_IProviderInfo, &LIBID_HYPERFEEDPROVIDERSLib>
{
public:
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

	STDMETHOD(get_UseProxy)(ProviderUseProxyType* pVal){if(!pVal) return E_POINTER; *pVal = enProxyNone; return S_OK;}
	STDMETHOD(put_UseProxy)(ProviderUseProxyType newVal){ return S_OK;}
	STDMETHOD(get_ProxyName)(BSTR* pVal){if(!pVal) return E_POINTER; *pVal = SysAllocString(L""); return S_OK;}
	STDMETHOD(put_ProxyName)(BSTR newVal){ return S_OK;}
	STDMETHOD(get_ProxyPort)(LONG* pVal){if(!pVal) return E_POINTER; *pVal = 0L; return S_OK;}
	STDMETHOD(put_ProxyPort)(LONG newVal){ return S_OK;}
	STDMETHOD(get_UserAccount)(BSTR* pVal){if(!pVal) return E_POINTER; *pVal = SysAllocString(L""); return S_OK;}
	STDMETHOD(put_UserAccount)(BSTR newVal){ return S_OK;}
	STDMETHOD(get_UserPassword)(BSTR* pVal){if(!pVal) return E_POINTER; *pVal = SysAllocString(L""); return S_OK;}
	STDMETHOD(put_UserPassword)(BSTR newVal){ return S_OK;}

	STDMETHOD(get_ProxyAuthorization)(ProviderProxyAuthType* pVal){if(!pVal) return E_POINTER; *pVal = enProxyAuthNone; return S_OK;}
	STDMETHOD(put_ProxyAuthorization)(ProviderProxyAuthType newVal){ return S_OK;}
	STDMETHOD(get_ProxyLogin)(BSTR* pVal){if(!pVal) return E_POINTER; *pVal = SysAllocString(L""); return S_OK;}
	STDMETHOD(put_ProxyLogin)(BSTR newVal){ return S_OK;}
	STDMETHOD(get_ProxyPassword)(BSTR* pVal){if(!pVal) return E_POINTER; *pVal = SysAllocString(L""); return S_OK;}
	STDMETHOD(put_ProxyPassword)(BSTR newVal){ return S_OK;}

};

#endif //__PROVIDERINFO_H_

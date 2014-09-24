// Provider.h : Declaration of the CProvider

#ifndef __PROVIDER_H_
#define __PROVIDER_H_

#include "resource.h"       // main symbols
#include <map>
/////////////////////////////////////////////////////////////////////////////
// CProvider
class ATL_NO_VTABLE CProviderData : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public ISupportErrorInfoImpl<&IID_IProvider>,
	public IDispatchImpl<IProviderData, &IID_IProviderData, &LIBID_PRICEPROVIDERSLib>
{
		typedef std::map<ProviderRole,_bstr_t> PROGS;
public:
	CProviderData()
	{
		m_bInitialized = false;
		m_clsidProvider = GUID_NULL;
		m_bGroup = VARIANT_FALSE;
		m_bLogin = VARIANT_TRUE;
		m_lID = -1;
		m_bProxy = true;
		m_bProxyAuth = true;
		//ModifyDefaultDacl();

	}

DECLARE_REGISTRY_RESOURCEID(IDR_PROVIDER)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CProviderData)
	COM_INTERFACE_ENTRY(IProviderData)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()

// IProvider
public:
	STDMETHOD(get_NeedLogin)(/*[out, retval]*/ VARIANT_BOOL *pVal);
	STDMETHOD(get_IsGroupRequestSupported)(/*[out, retval]*/ VARIANT_BOOL *pVal);
	STDMETHOD(get_ProgID)(/*[in]*/ ProviderRole role, /*[out, retval]*/ BSTR *pVal);
	STDMETHOD(get_Description)(/*[out, retval]*/ BSTR *pVal);
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


	long      m_lID;
	CComBSTR  m_bsDescription;
	PROGS     m_Progs;
	VARIANT_BOOL m_bGroup;
	VARIANT_BOOL m_bLogin;

	bool     m_bInitialized;
	bool     m_bProxy;
	bool     m_bProxyAuth;

	GUID     m_clsidProvider; 

private:
	HRESULT Initialize();
};

#endif //__PROVIDER_H_

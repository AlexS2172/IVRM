// DataFeedProviderInfo.h : Declaration of the CDataFeedProviderInfo

#pragma once
#include "resource.h"       // main symbols

#include "DataFeedProviders.h"
#include "_IDataFeedProviderInfoEvents_CP.h"




// CDataFeedProviderInfo

class ATL_NO_VTABLE CDataFeedProviderInfo :
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CDataFeedProviderInfo, &CLSID_DataFeedProviderInfo>,
	public ISupportErrorInfoImpl<&__uuidof(IProviderInfo)>,
	public IDispatchImpl<IProviderInfo, &IID_IDataFeedProviderInfo, &LIBID_DataFeedProvidersLib, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
public:
	CDataFeedProviderInfo()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_DATAFEEDPROVIDERINFO)


BEGIN_COM_MAP(CDataFeedProviderInfo)
		COM_INTERFACE_ENTRY(IProviderInfo)
		COM_INTERFACE_ENTRY(IDispatch)
		COM_INTERFACE_ENTRY(ISupportErrorInfo)
END_COM_MAP()

// ISupportsErrorInfo
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);


	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}

	void FinalRelease()
	{
	}

public:
	STDMETHOD (get_ProviderID) (long * pVal);
	STDMETHOD (get_Provider) (enum ProviderRole Type,BSTR * pProgID);
	STDMETHOD (get_Description) (BSTR * pVal);
	STDMETHOD (get_IsGroup) (VARIANT_BOOL * pVal);
	STDMETHOD (get_NeedLogin) (VARIANT_BOOL * pVal);
	STDMETHOD (get_UseProxy) (enum ProviderUseProxyType * pVal);
	STDMETHOD (put_UseProxy) (enum ProviderUseProxyType pVal);
	STDMETHOD (get_ProxyName) (BSTR * pVal);
	STDMETHOD (put_ProxyName) (BSTR pVal);
	STDMETHOD (get_ProxyPort) (long * pVal);
	STDMETHOD (put_ProxyPort) (long pVal);
	STDMETHOD (get_UserAccount) (BSTR * pVal);
	STDMETHOD (put_UserAccount) (BSTR pVal);
	STDMETHOD (get_UserPassword) (BSTR * pVal);
	STDMETHOD (put_UserPassword) (BSTR pVal);
	STDMETHOD (get_ProxyAuthorization) (enum ProviderProxyAuthType * pVal);
	STDMETHOD (put_ProxyAuthorization) (enum ProviderProxyAuthType pVal);
	STDMETHOD (get_ProxyLogin) (BSTR * pVal);
	STDMETHOD (put_ProxyLogin) (BSTR pVal);
	STDMETHOD (get_ProxyPassword) (BSTR * pVal);
	STDMETHOD (put_ProxyPassword) (BSTR pVal);

};

OBJECT_ENTRY_AUTO(__uuidof(DataFeedProviderInfo), CDataFeedProviderInfo)

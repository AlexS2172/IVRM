// FixProvidersC.h : Declaration of the CFixProviders

#ifndef __PROVIDERS_H_
#define __PROVIDERS_H_

#include "resource.h"       // main symbols
#include <vector>
#include "fixproviderscache.h"

////////////////////////////////////////////////////////////////////////////////////
// Copy classes
struct _CopyFixProvider
{
public:
	static HRESULT copy(IFixProviderSpecifics** p1, IFixProviderSpecifics* const* p2) {if (*p1=*p2) (*p2)->AddRef(); return S_OK;}
	static void init(IFixProviderSpecifics** p) {}
	static void destroy(IFixProviderSpecifics** p) {if (*p) (*p)->Release();}
};

struct _CopyFixProviderToVariant
{
public:
	static HRESULT copy(VARIANT* p1, IFixProviderSpecifics* const* p2) {
		HRESULT hr = (*p2)->QueryInterface(IID_IDispatch,(void**)&p1->pdispVal);
		if (SUCCEEDED(hr))
			p1->vt = VT_DISPATCH;
		return hr;
	}
	static void init(VARIANT* p) {VariantInit(p);}
	static void destroy(VARIANT* p) {VariantClear(p);}
};

/////////////////////////////////////////////////////////////////////////////
// CFixProviders
class ATL_NO_VTABLE CFixProviders:
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CFixProviders, &CLSID_FixProviders>,
	public ICollectionOnSTLImpl	<
									IDispatchImpl<	IFixProviders,
													&IID_IFixProviders,
													&LIBID_FixProvidersLibrary
												 >,
									std::vector<IFixProviderSpecifics*>,
									IFixProviderSpecifics*,
									_CopyFixProvider,
									CComEnumOnSTL<	IEnumVARIANT,
													&IID_IEnumVARIANT,
													VARIANT,
													_CopyFixProviderToVariant,
													std::vector<IFixProviderSpecifics*>
												 >
								>,
	public ISupportErrorInfoImpl<&IID_IFixProviders>
{
private:
	typedef std::vector<IFixProviderSpecifics*> collection;
	HRESULT DeleteAtoms();

public:
	CFixProviders()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_PROVIDERS)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CFixProviders)
	COM_INTERFACE_ENTRY(IFixProviders)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
END_COM_MAP()

// ISupportsErrorInfo
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

// IFixProviders
public:
	HRESULT FinalConstruct();
	STDMETHOD(get_Specifics)(/*[in]*/ long FixProviderId, /*[out, retval]*/ IFixProviderSpecifics* *pVal);
	STDMETHOD(Initialize)();
	STDMETHOD(GetFixProvider)(/*[in]*/ long FixProviderId, /*[out, retval]*/ IFixProviderSpecifics** ppFixProvider);
	STDMETHOD(GetFixProviderInfo)(/*[in]*/ long FixProviderID, /*[in, out]*/ BSTR* FileName, /*[in, out]*/ BSTR* Version);

private:
	CComPtr<IFixProvidersCache> m_spCache;
	bool m_bInitialized;

	void Clear();
};

#endif //__PROVIDERS_H_

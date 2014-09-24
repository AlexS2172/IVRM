// ProvidersCache.h : Declaration of the CProvidersCache

#pragma once
#include "resource.h"       // main symbols

#include "PriceProviders.h"


// CProvidersCache
////////////////////////////////////////////////////////////////////////////////////
// Copy classes
struct _CopyProviderCache
{
public:
	static HRESULT copy(IProviderData** p1, CProviderData*const* p2) {if (*p1=*p2) (*p2)->AddRef(); return S_OK;}
	static void init(IProviderData** p) {}
	static void destroy(IProviderData** p) {if (*p) (*p)->Release();}
};

struct _CopyProviderCacheToVariant
{
public:
	static HRESULT copy(VARIANT* p1, CProviderData*const* p2) {
		HRESULT hr = (*p2)->QueryInterface(IID_IDispatch,(void**)&p1->pdispVal);
		if (SUCCEEDED(hr))
			p1->vt = VT_DISPATCH;
		return hr;
	}
	static void init(VARIANT* p) {VariantInit(p);}
	static void destroy(VARIANT* p) {VariantClear(p);}
};


class ATL_NO_VTABLE CProvidersCache : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CProvidersCache, &CLSID_ProvidersCache>,
	public ISupportErrorInfoImpl<&IID_IProvidersCache>,
	public ICollectionOnSTLImpl	<
		IDispatchImpl<IProvidersCache, &IID_IProvidersCache, &LIBID_PRICEPROVIDERSLib>,
		std::vector< CProviderData * >,
		IProviderData*,
		_CopyProviderCache,
		CComEnumOnSTL< IEnumVARIANT, &IID_IEnumVARIANT, VARIANT, _CopyProviderCacheToVariant, std::vector< CProviderData* > >
	>
{
	typedef std::vector< CProviderData* > collection;

public:
	CProvidersCache()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_PROVIDERSCACHE)
DECLARE_CLASSFACTORY_SINGLETON(CProvidersCache)

BEGIN_COM_MAP(CProvidersCache)
	COM_INTERFACE_ENTRY(IProvidersCache)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
END_COM_MAP()

	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		m_lClientsCount = 0L;
		m_bInitialized = false;
		return S_OK;
	}
	
	void FinalRelease() 
	{
		Clear(); 
	}

public:

	STDMETHOD(Attach)(void);
	STDMETHOD(Detach)(void);

private:
	volatile long     m_lClientsCount;
	bool			  m_bInitialized;

	void Clear(void)
	{
		collection::iterator theIterator;
		for (theIterator=m_coll.begin(); theIterator!=m_coll.end(); theIterator++) 
		{
			(*theIterator)->Release();
		}
		m_coll.clear();
		m_bInitialized = false;
	}
};

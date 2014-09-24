// FixProvidersCache.h : Declaration of the CFixProvidersCache

#pragma once
#include "resource.h"       // main symbols

#include "fixproviders.h"
#include "fixproviderspecifics.h"
#include <vector>
//#include "egRegistry.h"

#pragma comment(linker, "/defaultlib:version.lib")

// {98C1C057-1812-4069-B79E-7D6CFA1CCB60}
static const GUID CATID_FixProviders =
{ 0x98c1c057, 0x1812, 0x4069, { 0xb7, 0x9e, 0x7d, 0x6c, 0xfa, 0x1c, 0xcb, 0x60 } };

class CModuleVersion : public VS_FIXEDFILEINFO
{
protected:
    BYTE* m_pVersionInfo;   // all version info

    struct TRANSLATION
    {
        WORD langID;         // language ID
        WORD charset;        // character set (code page)
    } m_translation;

public:
    CModuleVersion();
    virtual ~CModuleVersion();

    BOOL	GetModuleVersionInfo(HINSTANCE hInstance);
    BOOL    GetModuleVersionInfo(LPCTSTR modulename);
    BOOL    GetFileVersionInfo(LPCTSTR filename);

	BOOL	GetValue(LPCTSTR lpKeyName, _bstr_t& strVal);
    static	BOOL DllGetVersion(LPCTSTR modulename, DLLVERSIONINFO& dvi);
};


class CCatInformation : public CComPtr<ICatInformation> {
public:
	CCatInformation() {
		CoCreateInstance(CLSID_StdComponentCategoriesMgr, NULL, CLSCTX_INPROC);
	}
};

// CFixProvidersCache
////////////////////////////////////////////////////////////////////////////////////
// Copy classes
struct _CopyFixProviderCache
{
public:
	static HRESULT copy(IFixProviderSpecifics** p1, CFixProviderSpecifics* const* p2) {if (*p1=*p2) (*p2)->AddRef(); return S_OK;}
	static void init(IFixProviderSpecifics** p) {}
	static void destroy(IFixProviderSpecifics** p) {if (*p) (*p)->Release();}
};

struct _CopyFixProviderCacheToVariant
{
public:
	static HRESULT copy(VARIANT* p1, CFixProviderSpecifics* const* p2) {
		HRESULT hr = (*p2)->QueryInterface(IID_IDispatch,(void**)&p1->pdispVal);
		if (SUCCEEDED(hr))
			p1->vt = VT_DISPATCH;
		return hr;
	}
	static void init(VARIANT* p) {VariantInit(p);}
	static void destroy(VARIANT* p) {VariantClear(p);}
};


class ATL_NO_VTABLE CFixProvidersCache :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CFixProvidersCache, &CLSID_FixProvidersCache>,
	public ISupportErrorInfoImpl<&IID_IFixProvidersCache>,
	public ICollectionOnSTLImpl	<
									IDispatchImpl<	IFixProvidersCache,
													&IID_IFixProvidersCache,
													&LIBID_FixProvidersLibrary>,
									std::vector< CFixProviderSpecifics* >,
									IFixProviderSpecifics*,
									_CopyFixProviderCache,
									CComEnumOnSTL<	IEnumVARIANT,
													&IID_IEnumVARIANT,
													VARIANT,
													_CopyFixProviderCacheToVariant,
													std::vector<CFixProviderSpecifics*>
												 >
								>
{
	typedef std::vector< CFixProviderSpecifics* > collection;

public:
	CFixProvidersCache()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_PROVIDERSCACHE)
DECLARE_CLASSFACTORY_SINGLETON(CFixProvidersCache)

BEGIN_COM_MAP(CFixProvidersCache)
	COM_INTERFACE_ENTRY(IFixProvidersCache)
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

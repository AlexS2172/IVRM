// Providers.h : Declaration of the CProviders

#ifndef __PROVIDERS_H_
#define __PROVIDERS_H_

#include "resource.h"       // main symbols
#include "Provider.h"
#include "ProvidersCache.h"
#pragma comment(linker, "/defaultlib:version.lib")


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

    BOOL     GetModuleVersionInfo(HINSTANCE hInstance);
    BOOL     GetModuleVersionInfo(LPCTSTR modulename);
    BOOL     GetFileVersionInfo(LPCTSTR filename);

	BOOL	 GetValue(LPCTSTR lpKeyName, _bstr_t& strVal);
    static BOOL DllGetVersion(LPCTSTR modulename, DLLVERSIONINFO& dvi);
};


typedef std::vector<_bstr_t> PROVIDERS;
class CCatInformation : public PROVIDERS {
public:
	CCatInformation(){}
	HRESULT Initialize()
	{
		clear();
		EgLib::CEgRegKey key;
		if(ERROR_SUCCESS!=key.Open(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\Egar\\Providers"), KEY_READ ))
			return E_FAIL;
		int i=0;
		TCHAR chBuffer[256]={0};
		DWORD dwSize = 255;
		while(ERROR_SUCCESS == key.EnumKey(i, chBuffer, &dwSize))
		{
			if(dwSize)
				insert(end(), chBuffer);
			dwSize= 255;
			i++;
		}
		if(!size())
			return E_FAIL;
		return S_OK;
	}
};

////////////////////////////////////////////////////////////////////////////////////
// Copy classes
struct _CopyProvider
{
public:
	static HRESULT copy(IProviderData** p1, IProviderData*const* p2) {if (*p1=*p2) (*p2)->AddRef(); return S_OK;}
	static void init(IProviderData** p) {}
	static void destroy(IProviderData** p) {if (*p) (*p)->Release();}
};

struct _CopyProviderToVariant
{
public:
	static HRESULT copy(VARIANT* p1, IProviderData*const* p2) {
		HRESULT hr = (*p2)->QueryInterface(IID_IDispatch,(void**)&p1->pdispVal);
		if (SUCCEEDED(hr))
			p1->vt = VT_DISPATCH;
		return hr;
	}
	static void init(VARIANT* p) {VariantInit(p);}
	static void destroy(VARIANT* p) {VariantClear(p);}
};

////////////////////////////////////////////////////////////////////////////////////
// CDividends
class ATL_NO_VTABLE CProviders:
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CProviders, &CLSID_Providers>,
	public ICollectionOnSTLImpl	<
		IDispatchImpl<IProviders, &IID_IProviders, &LIBID_PRICEPROVIDERSLib>,
		std::vector< IProviderData* >,
		IProviderData*,
		_CopyProvider,
		CComEnumOnSTL< IEnumVARIANT, &IID_IEnumVARIANT, VARIANT, _CopyProviderToVariant, std::vector< IProviderData* > >
	>,
	public ISupportErrorInfoImpl<&IID_IProviders>
{
private:
	typedef std::vector< IProviderData* > collection;
	HRESULT DeleteAtoms();

public:

   CProviders();
   ~CProviders();

   void  FinalRelease()
   {
	   if(m_pCache!=NULL)
	   {
		   m_pCache->Detach();
		   m_pCache->Release();
		   m_pCache = NULL;
	   }
	   Clear();
   }

DECLARE_REGISTRY_RESOURCEID(IDR_PROVIDERS)
//DECLARE_CLASSFACTORY_SINGLETON(CProviders)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CProviders)
	COM_INTERFACE_ENTRY2(IDispatch, IProviders)
    COM_INTERFACE_ENTRY(IProviders)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
END_COM_MAP()

public:
	STDMETHOD(GetProviderInfo)(/*[in]*/ long ProviderID, /*[out]*/ BSTR* FileName, /*[out]*/ BSTR* Version);
	STDMETHOD(GetProvider)(/*[in]*/ long ProviderID, /*[out]*/ IProviderData** ppProvider);
	STDMETHOD(Initialize)();

private:
	CComObject<CProvidersCache>* m_pCache;

	void Clear();

	bool m_bInitialized;
};

#endif //__PROVIDERS_H_

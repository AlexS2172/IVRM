// FixProviderSpecifics.h : Declaration of the CFixProviderSpecifics

#ifndef __PROVIDERSPECIFICS_H_
#define __PROVIDERSPECIFICS_H_

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CFixProviderSpecifics
class ATL_NO_VTABLE CFixProviderSpecifics :
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CFixProviderSpecifics, &CLSID_FixProviderSpecifics>,
	public ISupportErrorInfo,
	public IDispatchImpl<IFixProviderSpecifics, &IID_IFixProviderSpecifics, &LIBID_FixProvidersLibrary>
{
public:
	CFixProviderSpecifics()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_PROVIDERSPECIFICS)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CFixProviderSpecifics)
	COM_INTERFACE_ENTRY(IFixProviderSpecifics)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
END_COM_MAP()

// ISupportsErrorInfo
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

// IFixProviderSpecifics
public:
	bool m_bInitialized;
	CLSID m_clsidFixProvider;
	bstr_t m_bsDescription;
	long m_lID;
	STDMETHOD(GetSpecifics)(/*[in, out]*/ long* FixProviderID,
							/*[in, out]*/ BSTR* ProgId,
							/*[in, out]*/ BSTR* Description,
							/*[in, out]*/ VARIANT_BOOL * LoginNeeded,
							/*[in, out]*/ BSTR* UserAccount,
							/*[in, out]*/ BSTR* UserPassword);

	STDMETHOD(SetSpecifics)(/*[in, out]*/ long* FixProviderID,
							/*[in, out]*/ BSTR* ProgId,
							/*[in, out]*/ BSTR* Description,
							/*[in, out]*/ VARIANT_BOOL * LoginNeeded,
							/*[in, out]*/ BSTR* UserAccount,
							/*[in, out]*/ BSTR* UserPassword);
};

#endif //__PROVIDERSPECIFICS_H_

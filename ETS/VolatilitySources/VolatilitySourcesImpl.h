// VolatilitySourcesImpl.h : Declaration of the CVolatilitySources

#pragma once
#include "resource.h"       // main symbols

#include "VolatilitySources.h"



class ATL_NO_VTABLE CVolatilitySources : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CVolatilitySources, &CLSID_VolatilitySources>,
	public ISupportErrorInfo,
	public IDispatchImpl<IVolatilitySources, &__uuidof(IVolatilitySources), &LIBID_VolatilitySourcesLib, /* wMajor = */ 1, /* wMinor = */ 0>
/*
	public ICollectionOnSTLImpl	<
		IDispatchImpl<IVolatilitySources, &__uuidof(IVolatilitySources), &LIBID_VolatilitySourcesLib>,
		std::vector< CVSData* >,
		IVSData*,
		_CopyVSCache,
		CComEnumOnSTL< IEnumVARIANT, &IID_IEnumVARIANT, VARIANT, _CopyProviderCacheToVariant, std::vector< CProviderData* > >
	>*/


{
public:
	CVolatilitySources(){}

	DECLARE_REGISTRY_RESOURCEID(IDR_VOLATILITYSOURCES1)


	BEGIN_COM_MAP(CVolatilitySources)

		COM_INTERFACE_ENTRY2(IDispatch, IVolatilitySources)
		COM_INTERFACE_ENTRY(ISupportErrorInfo)
		COM_INTERFACE_ENTRY(IVolatilitySources)
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

protected:
	///////////////////////////////////////////////////////////
	// IVolatilitySources Methods
public:
	STDMETHOD(get_Count)(long *  pVal);
	STDMETHOD(GetVolatilitySourceInfo)(long Index, long* ID, BSTR* Title, BSTR* FileName, BSTR* Version, BSTR* ProgID);
	STDMETHOD(get_VolatilitySource)(LONG ID, IVolatilitySource** pVal);
	STDMETHOD(GetVolatilitySourceInfoByID)(long ID, BSTR* Title, BSTR* FileName, BSTR* Version, BSTR* ProgID);
};

OBJECT_ENTRY_AUTO(__uuidof(VolatilitySources), CVolatilitySources)

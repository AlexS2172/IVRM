// IvBlob.h : Declaration of the CIvBlob

#pragma once
#include "resource.h"       // main symbols
#include "MsgStruct.h"
#include "MsgPropImpl.h"

typedef MapF<WStringF, VariantF> type_OptionsData ;
typedef MapF<WStringF, VariantF> type_StocksData ;


struct __IVPackegeData
{
	BEGIN_PACKING
		FIELD_ENTRY(m_sSymbolPrice)
		FIELD_ENTRY(m_sOptionVola)
	END_PACKING
	
	StringF							m_sSymbolPrice ;
	StringF							m_sOptionVola ;
};


// CIvBlob

class ATL_NO_VTABLE CIvBlob : 
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CIvBlob, &CLSID_IvBlob>,
	public IDispatchImpl<IIvBlob, &IID_IIvBlob, &LIBID_MSGSTRUCTLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public INetPackingConstSizeDataImpl<CIvBlob, __IVPackegeData>
{
public:
	CIvBlob()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_IVBLOB)


BEGIN_COM_MAP(CIvBlob)
	COM_INTERFACE_ENTRY(IIvBlob)
	COM_INTERFACE_ENTRY2(IDispatch, IIvBlob)
	COM_INTERFACE_ENTRY(INetPacking)
END_COM_MAP()


	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}
	
	void FinalRelease() 
	{
	}

public:
	STDMETHOD(AddIvOption)(INT vOption , DOUBLE vVolatility);
	STDMETHOD(AddIvStock)(INT vStock, DOUBLE vPrice);
	IMPLEMENT_VARCHAR_PROP( SymbolPrice,m_sSymbolPrice) ;
	IMPLEMENT_VARCHAR_PROP( OptionVola ,m_sOptionVola) ;
	STDMETHOD(Clear)() ;
	STDMETHOD(Empty)(VARIANT_BOOL*Empty) ;
} ;

OBJECT_ENTRY_AUTO(__uuidof(IvBlob), CIvBlob)

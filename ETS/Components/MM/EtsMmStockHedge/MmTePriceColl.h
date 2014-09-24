// MmTePriceColl.h : Declaration of the CMmTePriceColl

#pragma once
#include "resource.h"       // main symbols

#include "EtsMmStockHedge.h"


typedef IDispatchImpl<IMmTePriceColl, &IID_IMmTePriceColl, &LIBID_EtsMmStockHedgeLib>				IMmTePriceCollDispImpl;
typedef ICollectionOnSTLMapExOfInterfacePtrImpl<IMmTePriceCollDispImpl, IMmTePriceAtom, LONG, LONG, BSTR, _bstr_t >	IMmTePriceCollCollImpl;

// CMmTePriceColl

class ATL_NO_VTABLE CMmTePriceColl : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CMmTePriceColl, &CLSID_MmTePriceColl>,
	public ISupportErrorInfoImpl<&IID_IMmTePriceColl>,
	public IMmTePriceCollCollImpl
{
public:
	CMmTePriceColl()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_MMTEPRICECOLL)


BEGIN_COM_MAP(CMmTePriceColl)
	COM_INTERFACE_ENTRY(IMmTePriceColl)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
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
	STDMETHOD(Add)(LONG Key, BSTR SortKey, IMmTePriceAtom* Value, IMmTePriceAtom** pRetVal);
};

OBJECT_ENTRY_AUTO(__uuidof(MmTePriceColl), CMmTePriceColl)

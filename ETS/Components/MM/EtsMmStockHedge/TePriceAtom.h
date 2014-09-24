// TePriceAtom.h : Declaration of the CTePriceAtom

#pragma once
#include "resource.h"       // main symbols

#include "EtsMmStockHedge.h"

struct __MmTePriceAtom
{
	DOUBLE						m_dPriceBid;
	DOUBLE						m_dPriceAsk;
	DOUBLE						m_dPriceLast;

	__MmTePriceAtom()
		:m_dPriceBid(0.0)
		,m_dPriceAsk(0.0)
		,m_dPriceLast(0.0)
	{}
};

// CTePriceAtom

class ATL_NO_VTABLE CMmTePriceAtom : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CMmTePriceAtom, &CLSID_MmTePriceAtom>,
	public ISupportErrorInfoImpl<&IID_IMmTePriceAtom>,
	public IDispatchImpl<IMmTePriceAtom, &IID_IMmTePriceAtom, &LIBID_EtsMmStockHedgeLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public __MmTePriceAtom
{
public:
	CMmTePriceAtom()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_TEPRICEATOM)


BEGIN_COM_MAP(CMmTePriceAtom)
	COM_INTERFACE_ENTRY(IMmTePriceAtom)
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
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, PriceBid, m_dPriceBid);
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, PriceAsk, m_dPriceAsk);
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, PriceLast, m_dPriceLast);

};

OBJECT_ENTRY_AUTO(__uuidof(MmTePriceAtom), CMmTePriceAtom)

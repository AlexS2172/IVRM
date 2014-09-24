// MmIhOptAtom.h : Declaration of the CMmIhOptAtom

#pragma once
#include "resource.h"       // main symbols

#include "EtsMmIndexHedge.h"

_COM_SMARTPTR_TYPEDEF(IMmIhOptAtom, IID_IMmIhOptAtom);

struct __MmIhOptAtom
{
	LONG						m_nID;
	CComBSTR					m_bstrSymbol;
	DATE						m_dtExpiry;
	DATE						m_dtExpiryOV;
	DATE						m_dtTradingClose;
	EtsOptionTypeEnum			m_enOptType;
	DOUBLE						m_dStrike;
	DOUBLE						m_dVola;
	DOUBLE						m_dRate;
	LONG						m_nQty;
	DOUBLE						m_dHTBRate;

	DOUBLE						m_dPriceBid;
	DOUBLE						m_dPriceAsk;
	DOUBLE						m_dPriceLast;

	DOUBLE						m_dDelta;

	__MmIhOptAtom()
		: m_nID(0L), m_dtExpiry(0.), m_enOptType(enOtPut), m_dStrike(BAD_DOUBLE_VALUE),
		m_dVola(BAD_DOUBLE_VALUE), m_dRate(BAD_DOUBLE_VALUE), m_nQty(BAD_LONG_VALUE),
		m_dPriceBid(BAD_DOUBLE_VALUE), m_dPriceAsk(BAD_DOUBLE_VALUE),
		m_dPriceLast(BAD_DOUBLE_VALUE), m_dDelta(BAD_DOUBLE_VALUE), m_dHTBRate(BAD_DOUBLE_VALUE),m_dtExpiryOV(0.),
		m_dtTradingClose(0.)
	{
	}
};

// CMmIhOptAtom

class ATL_NO_VTABLE CMmIhOptAtom : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CMmIhOptAtom, &CLSID_MmIhOptAtom>,
	public IDispatchImpl<IMmIhOptAtom, &IID_IMmIhOptAtom, &LIBID_EtsMmIndexHedgeLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public __MmIhOptAtom
{
public:
	CMmIhOptAtom()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_MMIHOPTATOM)


BEGIN_COM_MAP(CMmIhOptAtom)
	COM_INTERFACE_ENTRY(IMmIhOptAtom)
	COM_INTERFACE_ENTRY(IDispatch)
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

	IMPLEMENT_SIMPLE_PROPERTY(LONG, ID, m_nID)
	IMPLEMENT_BSTR_PROPERTY(Symbol, m_bstrSymbol)
	IMPLEMENT_SIMPLE_PROPERTY(DATE, Expiry, m_dtExpiry)
	IMPLEMENT_SIMPLE_PROPERTY(EtsOptionTypeEnum, OptType, m_enOptType)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, Strike, m_dStrike)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, Vola, m_dVola)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, Rate, m_dRate)
	IMPLEMENT_SIMPLE_PROPERTY(LONG, Qty, m_nQty)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, HTBRate, m_dHTBRate)

	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, PriceBid, m_dPriceBid)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, PriceAsk, m_dPriceAsk)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, PriceLast, m_dPriceLast)

	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, Delta, m_dDelta)
	IMPLEMENT_SIMPLE_PROPERTY(DATE, ExpiryOV, m_dtExpiryOV)
	IMPLEMENT_SIMPLE_PROPERTY(DATE, TradingClose, m_dtTradingClose)

};

OBJECT_ENTRY_AUTO(__uuidof(MmIhOptAtom), CMmIhOptAtom)

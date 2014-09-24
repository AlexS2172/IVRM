// MmShOptAtom.h : Declaration of the CMmShOptAtom

#pragma once
#include "resource.h"       // main symbols

#include "EtsMmStockHedge.h"
#include "MmShOptRootAtom.h"

// CMmShOptAtom
_COM_SMARTPTR_TYPEDEF(IMmShOptAtom, IID_IMmShOptAtom);

struct __MmShOptAtom
{
	LONG						m_nID;
	CComBSTR					m_bstrSymbol;
	DATE						m_dtExpiry;
	EtsOptionTypeEnum			m_enOptType;
	DOUBLE						m_dStrike;
	DOUBLE						m_dVola;
	DOUBLE						m_dRate;
	IMmShOptRootAtomPtr			m_spOptRoot;
	DOUBLE						m_dHTBRate;

	DOUBLE						m_dPriceBid;
	DOUBLE						m_dPriceAsk;
	DOUBLE						m_dPriceLast;

	DOUBLE						m_dDelta;
	DATE						m_dtExpiryOV;
	DATE						m_dtTradingClose;

	__MmShOptAtom()
		: m_nID(BAD_LONG_VALUE), m_dtExpiry(0.), m_enOptType(enOtPut), m_dStrike(BAD_DOUBLE_VALUE),
		m_dVola(BAD_DOUBLE_VALUE), m_dRate(BAD_DOUBLE_VALUE), m_dPriceBid(BAD_DOUBLE_VALUE),
		m_dPriceAsk(BAD_DOUBLE_VALUE), m_dPriceLast(BAD_DOUBLE_VALUE), m_dDelta(BAD_DOUBLE_VALUE),
		m_dHTBRate(BAD_DOUBLE_VALUE),
		m_dtExpiryOV(0.), m_dtTradingClose(0.)
	{
	}
};

class ATL_NO_VTABLE CMmShOptAtom : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CMmShOptAtom, &CLSID_MmShOptAtom>,
	public IDispatchImpl<IMmShOptAtom, &IID_IMmShOptAtom, &LIBID_EtsMmStockHedgeLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public __MmShOptAtom
{
public:
	CMmShOptAtom()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_MMSHOPTATOM)


BEGIN_COM_MAP(CMmShOptAtom)
	COM_INTERFACE_ENTRY(IMmShOptAtom)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()


	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}
	
	void FinalRelease() 
	{
		m_spOptRoot = NULL;
	}

public:
	IMPLEMENT_SIMPLE_PROPERTY(LONG, ID, m_nID)
	IMPLEMENT_BSTR_PROPERTY(Symbol, m_bstrSymbol)
	IMPLEMENT_SIMPLE_PROPERTY(DATE, Expiry, m_dtExpiry)
	IMPLEMENT_SIMPLE_PROPERTY(EtsOptionTypeEnum, OptType, m_enOptType)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, Strike, m_dStrike)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, Vola, m_dVola)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, Rate, m_dRate)
	IMPLEMENT_OBJECT_PROPERTY(IMmShOptRootAtom*, OptRoot, m_spOptRoot)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, HTBRate, m_dHTBRate)

	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, PriceBid, m_dPriceBid)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, PriceAsk, m_dPriceAsk)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, PriceLast, m_dPriceLast)

	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, Delta, m_dDelta)
	IMPLEMENT_SIMPLE_PROPERTY(DATE, ExpiryOV, m_dtExpiryOV)
	IMPLEMENT_SIMPLE_PROPERTY(DATE, TradingClose, m_dtTradingClose)
};

OBJECT_ENTRY_AUTO(__uuidof(MmShOptAtom), CMmShOptAtom)

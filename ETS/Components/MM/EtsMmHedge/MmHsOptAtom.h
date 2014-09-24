// MmHsOptAtom.h : Declaration of the CMmHsOptAtom

#pragma once
#include "resource.h"       // main symbols
#include "EtsMmHedge.h"


_COM_SMARTPTR_TYPEDEF(IMmHsOptAtom, IID_IMmHsOptAtom);

struct __MmHsOptAtom
{
	DOUBLE				m_dDelta;
	VARIANT_BOOL		m_vbEnabled;
	DATE				m_dtExpiry;
	LONG				m_nID;
	LONG				m_nLotSize;
	EtsOptionTypeEnum	m_enOptType;
	DOUBLE				m_dPriceAsk;
	DOUBLE				m_dPriceBid;
	DOUBLE				m_dPriceLast;
	LONG				m_nQty;
	DOUBLE				m_dRate;
	LONG				m_nUndID;
	DOUBLE				m_dStrike;
	CComBSTR			m_bstrSymbol;
	DOUBLE				m_dVega;
	DOUBLE				m_dVegaWeight;
	DOUBLE				m_dVola;
	DOUBLE				m_dHTBRate;

	DATE				m_dtExpiryOV;
	DATE				m_dtTradingClose;
	LONG				m_nOptionRootID;

	void ClearValues()
	{
		m_dDelta = BAD_DOUBLE_VALUE;
		m_vbEnabled = VARIANT_TRUE;
		m_dtExpiry = 0.;
		m_nID = 0L;
		m_nLotSize = 0L;
		m_enOptType = enOtPut;
		m_dPriceAsk = BAD_DOUBLE_VALUE;
		m_dPriceBid = BAD_DOUBLE_VALUE;
		m_dPriceLast = BAD_DOUBLE_VALUE;
		m_nQty = BAD_LONG_VALUE;
		m_dRate = 0.;
		m_nUndID = 0L;
		m_dStrike = 0.;
		m_dVega = BAD_DOUBLE_VALUE;
		m_dVegaWeight = 0.;
		m_dVola = 0.;
		m_dHTBRate = BAD_DOUBLE_VALUE;
		m_dtExpiryOV = 0;
		m_dtTradingClose = 0;
		m_nOptionRootID = 0;
	}

	__MmHsOptAtom()
	{
		ClearValues();
	}
};

// CMmHsOptAtom

class ATL_NO_VTABLE CMmHsOptAtom : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CMmHsOptAtom, &CLSID_MmHsOptAtom>,
	public ISupportErrorInfoImpl<&IID_IMmHsOptAtom>,
	public IDispatchImpl<IMmHsOptAtom, &IID_IMmHsOptAtom, &LIBID_EtsMmHedgeLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public __MmHsOptAtom
{
public:
	CMmHsOptAtom()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_MMHSOPTATOM)


BEGIN_COM_MAP(CMmHsOptAtom)
	COM_INTERFACE_ENTRY(IMmHsOptAtom)
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

	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE,				Delta,			m_dDelta)
	IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL,			Enabled,		m_vbEnabled)
	IMPLEMENT_SIMPLE_PROPERTY(DATE,					Expiry,			m_dtExpiry)
	IMPLEMENT_SIMPLE_PROPERTY(LONG,					ID,				m_nID)
	IMPLEMENT_SIMPLE_PROPERTY(LONG,					LotSize,		m_nLotSize)
	IMPLEMENT_SIMPLE_PROPERTY(EtsOptionTypeEnum,	OptType,		m_enOptType)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE,				PriceAsk,		m_dPriceAsk)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE,				PriceBid,		m_dPriceBid)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE,				PriceLast,		m_dPriceLast)
	IMPLEMENT_SIMPLE_PROPERTY(LONG,					Qty,			m_nQty)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE,				Rate,			m_dRate)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE,				HTBRate,		m_dHTBRate)
	IMPLEMENT_SIMPLE_PROPERTY(LONG,					UndID,			m_nUndID)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE,				Strike,			m_dStrike)
	IMPLEMENT_BSTR_PROPERTY(Symbol,		m_bstrSymbol)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE,				Vega,			m_dVega)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE,				VegaWeight,		m_dVegaWeight)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE,				Vola,			m_dVola)

	IMPLEMENT_SIMPLE_PROPERTY(DATE,					ExpiryOV,		m_dtExpiryOV)
	IMPLEMENT_SIMPLE_PROPERTY(DATE,					TradingClose,	m_dtTradingClose)
	IMPLEMENT_SIMPLE_PROPERTY(LONG,					OptionRootID,	m_nOptionRootID)

	STDMETHOD(ClearValues)();
};

OBJECT_ENTRY_AUTO(__uuidof(MmHsOptAtom), CMmHsOptAtom)

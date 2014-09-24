// MmVaOptAtom.h : Declaration of the CMmVaOptAtom
#ifndef __MMVAOPTATOM_H_
#define __MMVAOPTATOM_H_
#pragma once

#include "resource.h"       // main symbols
#include "EtsMmVolaAnalysis.h"

_COM_SMARTPTR_TYPEDEF(IMmVaOptAtom, IID_IMmVaOptAtom);

struct __MmVaOptAtom
{
	LONG				m_nID;
	CComBSTR			m_bstrSymbol;
	DATE				m_dtExpiry;
	EtsOptionTypeEnum	m_enOptType;
	DOUBLE				m_dStrike;
	DOUBLE				m_dVola;
	IExchAtomPtr		m_spExch;
	DOUBLE				m_dPriceBid;
	DOUBLE				m_dPriceAsk;
	DOUBLE				m_dPriceLast;
	DOUBLE				m_dPriceClose;
	DOUBLE				m_dPriceTheoClose;
	DOUBLE				m_dIVBid;
	DOUBLE				m_dIVAsk;
	DOUBLE				m_dVegaBid;
	DOUBLE				m_dVegaAsk;
	DOUBLE				m_dDeltaBid;
	DOUBLE				m_dDeltaAsk;
	DATE				m_dtExpiryOV;
	LONG				m_nOptionRootID;

	__MmVaOptAtom()
		: m_nID(0L), m_dtExpiry(0.), m_enOptType(enOtPut), m_dStrike(0.),
		m_dVola(0.), m_dPriceBid(BAD_DOUBLE_VALUE), m_dPriceAsk(BAD_DOUBLE_VALUE), m_dPriceLast(BAD_DOUBLE_VALUE),
		m_dPriceClose(BAD_DOUBLE_VALUE), m_dPriceTheoClose(BAD_DOUBLE_VALUE),
		m_dIVBid(BAD_DOUBLE_VALUE), m_dIVAsk(BAD_DOUBLE_VALUE), m_dVegaBid(BAD_DOUBLE_VALUE),
		m_dVegaAsk(BAD_DOUBLE_VALUE), m_dDeltaBid(BAD_DOUBLE_VALUE), m_dDeltaAsk(BAD_DOUBLE_VALUE), m_dtExpiryOV(0.)
		,m_nOptionRootID(0)
	{
	}

	void ClearValues(VARIANT_BOOL bWithIV)
	{
		if(VARIANT_TRUE == bWithIV)
		{
			m_dIVBid = BAD_DOUBLE_VALUE;
			m_dIVAsk = BAD_DOUBLE_VALUE;
		}

		m_dVegaBid = BAD_DOUBLE_VALUE;
		m_dVegaAsk = BAD_DOUBLE_VALUE;
		m_dDeltaBid = BAD_DOUBLE_VALUE;
		m_dDeltaAsk = BAD_DOUBLE_VALUE;
	}
};

// CMmVaOptAtom

class ATL_NO_VTABLE CMmVaOptAtom : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CMmVaOptAtom, &CLSID_MmVaOptAtom>,
	public ISupportErrorInfoImpl<&IID_IMmVaOptAtom>,
	public IDispatchImpl<IMmVaOptAtom, &IID_IMmVaOptAtom, &LIBID_EtsMmVolaAnalysisLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public __MmVaOptAtom
{
public:
	CMmVaOptAtom()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_MMVAOPTATOM)


BEGIN_COM_MAP(CMmVaOptAtom)
	COM_INTERFACE_ENTRY(IMmVaOptAtom)
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
		m_spExch = NULL;
	}

public:

	IMPLEMENT_SIMPLE_PROPERTY(LONG, ID, m_nID)
	IMPLEMENT_BSTR_PROPERTY(Symbol, m_bstrSymbol)
	IMPLEMENT_SIMPLE_PROPERTY(DATE, Expiry, m_dtExpiry)
	IMPLEMENT_SIMPLE_PROPERTY(EtsOptionTypeEnum, OptType, m_enOptType)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, Strike, m_dStrike)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, Vola, m_dVola)
	IMPLEMENT_OBJECT_PROPERTY(IExchAtom*, Exch, m_spExch)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, PriceBid, m_dPriceBid)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, PriceAsk, m_dPriceAsk)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, PriceLast, m_dPriceLast)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, PriceClose, m_dPriceClose)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, PriceTheoClose, m_dPriceTheoClose)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, IVBid, m_dIVBid)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, IVAsk, m_dIVAsk)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, VegaBid, m_dVegaBid)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, VegaAsk, m_dVegaAsk)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, DeltaBid, m_dDeltaBid)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, DeltaAsk, m_dDeltaAsk)
	IMPLEMENT_SIMPLE_PROPERTY(DATE, ExpiryOV, m_dtExpiryOV)
	IMPLEMENT_SIMPLE_PROPERTY(LONG, OptionRootID, m_nOptionRootID)

	STDMETHODIMP ClearValues(VARIANT_BOOL WithIV)
	{
		__MmVaOptAtom::ClearValues(WithIV);
		return S_OK;
	}
};

OBJECT_ENTRY_AUTO(__uuidof(MmVaOptAtom), CMmVaOptAtom)

#endif //__MMVAOPTATOM_H_
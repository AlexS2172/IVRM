// MmRpPosAtom.h : Declaration of the CMmRpPosAtom
#ifndef __MM_RP_POS_ATOM__
#define __MM_RP_POS_ATOM__
#pragma once

#include "resource.h"       // main symbols
#include "EtsMmReports.h"

_COM_SMARTPTR_TYPEDEF(IMmRpPosAtom, IID_IMmRpPosAtom);

struct __MmRpPosAtom
{
	LONG					m_nID;
	CComBSTR				m_bstrSymbol;
	DOUBLE					m_dQtyDailyPrevDateBuy;
	DOUBLE					m_dQtyDailyPrevDateSell;
	DOUBLE					m_dQtyDailyTodayBuy;
	DOUBLE					m_dQtyDailyTodaySell;
	DOUBLE					m_dQtyInSharesDailyPrevDateBuy;
	DOUBLE					m_dQtyInSharesDailyPrevDateSell;
	DOUBLE					m_dQtyInSharesDailyTodayBuy;
	DOUBLE					m_dQtyInSharesDailyTodaySell;
	DOUBLE					m_dPosDailyPrevDateBuy;
	DOUBLE					m_dPosDailyPrevDateSell;
	DOUBLE					m_dPosDailyTodayBuy;
	DOUBLE					m_dPosDailyTodaySell;

	DOUBLE					m_dQtyLTDBuy;
	DOUBLE					m_dQtyLTDSell;
	DOUBLE					m_dQtyInSharesLTDBuy;
	DOUBLE					m_dQtyInSharesLTDSell;
	DOUBLE					m_dPosLTDBuy;
	DOUBLE					m_dPosLTDSell;

	__MmRpPosAtom()
	{
		Clear();
	}

	HRESULT Clear()
	{
		m_nID = 0L;
		m_bstrSymbol.Empty();
		m_dQtyDailyPrevDateBuy	= 0.;
		m_dQtyDailyPrevDateSell = 0.;
		m_dQtyDailyTodayBuy		= 0.;
        m_dQtyDailyTodaySell	= 0.;
		m_dQtyInSharesDailyPrevDateBuy	= 0.;
		m_dQtyInSharesDailyPrevDateSell = 0.;
		m_dQtyInSharesDailyTodayBuy		= 0.;
        m_dQtyInSharesDailyTodaySell	= 0.;
		m_dPosDailyPrevDateBuy	= 0.;
		m_dPosDailyPrevDateSell = 0.;
		m_dPosDailyTodayBuy		= 0.;
		m_dPosDailyTodaySell	= 0.;

		m_dQtyLTDBuy = 0.;
		m_dQtyLTDSell = 0.;
		m_dQtyInSharesLTDBuy = 0.;
		m_dQtyInSharesLTDSell = 0.;
		m_dPosLTDBuy = 0.;
		m_dPosLTDSell = 0.;

		return S_OK;
	}
};

// CMmRpPosAtom

class ATL_NO_VTABLE CMmRpPosAtom : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CMmRpPosAtom, &CLSID_MmRpPosAtom>,
	public ISupportErrorInfoImpl<&IID_IMmRpPosAtom>,
	public IDispatchImpl<IMmRpPosAtom, &IID_IMmRpPosAtom, &LIBID_EtsMmReportsLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public __MmRpPosAtom
{
public:
	CMmRpPosAtom()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_MMRPPOSATOM)

DECLARE_NOT_AGGREGATABLE(CMmRpPosAtom)

BEGIN_COM_MAP(CMmRpPosAtom)
	COM_INTERFACE_ENTRY(IMmRpPosAtom)
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

	IMPLEMENT_SIMPLE_PROPERTY(LONG,			ID,						m_nID)
	IMPLEMENT_BSTR_PROPERTY(Symbol,		m_bstrSymbol)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE,		QtyDailyPrevDateBuy,	m_dQtyDailyPrevDateBuy)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE,		QtyDailyPrevDateSell,	m_dQtyDailyPrevDateSell)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE,		QtyDailyTodayBuy,		m_dQtyDailyTodayBuy)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE,		QtyDailyTodaySell,		m_dQtyDailyTodaySell)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE,		QtyInSharesDailyPrevDateBuy,	m_dQtyInSharesDailyPrevDateBuy)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE,		QtyInSharesDailyPrevDateSell,	m_dQtyInSharesDailyPrevDateSell)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE,		QtyInSharesDailyTodayBuy,		m_dQtyInSharesDailyTodayBuy)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE,		QtyInSharesDailyTodaySell,		m_dQtyInSharesDailyTodaySell)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE,		PosDailyPrevDateBuy,	m_dPosDailyPrevDateBuy)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE,		PosDailyPrevDateSell,	m_dPosDailyPrevDateSell)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE,		PosDailyTodayBuy,		m_dPosDailyTodayBuy)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE,		PosDailyTodaySell,		m_dPosDailyTodaySell)

	STDMETHOD(Clear)();
	STDMETHOD(Add)(IMmTradeInfoAtom* Trade);

	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE,		QtyLTDBuy,		m_dQtyLTDBuy)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE,		QtyLTDSell,		m_dQtyLTDSell)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE,		QtyInSharesLTDBuy,		m_dQtyInSharesLTDBuy)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE,		QtyInSharesLTDSell,		m_dQtyInSharesLTDSell)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE,		PosLTDBuy,		m_dPosLTDBuy)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE,		PosLTDSell,		m_dPosLTDSell)
};

OBJECT_ENTRY_AUTO(__uuidof(MmRpPosAtom), CMmRpPosAtom)

#endif //__MM_RP_POS_ATOM__
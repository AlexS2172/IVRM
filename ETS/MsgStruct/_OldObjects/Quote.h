// Quote.h : Declaration of the CQuote

#ifndef __QUOTE_H_
#define __QUOTE_H_

#include "resource.h"       // main symbols
#include "MsgPacking.h"
#include "MsgPropImpl.h"

struct __QuoteData
{
	BEGIN_PACKING
		FIELD_ENTRY(m_dBidPrice)
		FIELD_ENTRY(m_dAskPrice)
		FIELD_ENTRY(m_dLastPrice)
		FIELD_ENTRY(m_nBidSize)
		FIELD_ENTRY(m_nAskSize)
		FIELD_ENTRY(m_dClosePrice)
		FIELD_ENTRY(m_dLowPrice)
		FIELD_ENTRY(m_dHighPrice)
		FIELD_ENTRY(m_dOpenPrice)
		FIELD_ENTRY(m_dNetChange)
		FIELD_ENTRY(m_nVolume)
		FIELD_ENTRY(m_nOpenInterest)
		FIELD_ENTRY(m_nLotSize)
	END_PACKING

	DoubleF		m_dBidPrice;
	DoubleF		m_dAskPrice;
	DoubleF		m_dLastPrice;
	LongF		m_nBidSize;
	LongF		m_nAskSize;
	DoubleF		m_dClosePrice;
	DoubleF		m_dLowPrice;
	DoubleF		m_dHighPrice;
	DoubleF		m_dOpenPrice;
	DoubleF		m_dNetChange;
	LongF		m_nVolume;
	LongF		m_nOpenInterest;
	LongF		m_nLotSize;
};

/////////////////////////////////////////////////////////////////////////////
// CQuote
class ATL_NO_VTABLE CQuote : 
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CQuote, &CLSID_Quote>,
	public IDispatchImpl<IQuote, &IID_IQuote, &LIBID_MSGSTRUCTLib>,
	public INetPackingConstSizeDataImpl<CQuote, __QuoteData>
{
public:
	CQuote()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_QUOTE)
DECLARE_NOT_AGGREGATABLE(CQuote)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CQuote)
	COM_INTERFACE_ENTRY(IQuote)
	COM_INTERFACE_ENTRY2(IDispatch, IQuote)
	COM_INTERFACE_ENTRY(INetPacking)
END_COM_MAP()

// IQuote
public:
	IMPLEMENT_DOUBLE_PROP(BidPrice, m_dBidPrice)
	IMPLEMENT_DOUBLE_PROP(AskPrice, m_dAskPrice)
	IMPLEMENT_DOUBLE_PROP(LastPrice, m_dLastPrice)
	IMPLEMENT_LONG_PROP(BidSize, m_nBidSize)
	IMPLEMENT_LONG_PROP(AskSize, m_nAskSize)
	IMPLEMENT_DOUBLE_PROP(ClosePrice, m_dClosePrice)
	IMPLEMENT_DOUBLE_PROP(LowPrice, m_dLowPrice)
	IMPLEMENT_DOUBLE_PROP(HighPrice, m_dHighPrice)
	IMPLEMENT_DOUBLE_PROP(OpenPrice, m_dOpenPrice)
	IMPLEMENT_DOUBLE_PROP(NetChange, m_dNetChange)
	IMPLEMENT_LONG_PROP(Volume, m_nVolume)
	IMPLEMENT_LONG_PROP(OpenInterest, m_nOpenInterest)
	IMPLEMENT_LONG_PROP(LotSize, m_nLotSize)
};

#endif //__QUOTE_H_

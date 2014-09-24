// LastQuote.h : Declaration of the CLastQuote

#ifndef __LASTQUOTE_H_
#define __LASTQUOTE_H_

#include "resource.h"       // main symbols
#include "HHStruct.h"
#include "HHCommonProps.h"
#include "MsgPropImpl.h"

/////////////////////////////////////////////////////////////////////////////
// CLastQuote
class ATL_NO_VTABLE CLastQuote : 
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CLastQuote, &CLSID_LastQuote>,
	public IDispatchImpl<ILastQuote, &IID_ILastQuote, &LIBID_MSGSTRUCTLib>,
	public INetPackingConstSizeDataImpl<CLastQuote, __LastQuoteData>
{
public:
	CLastQuote()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_LASTQUOTE)
DECLARE_NOT_AGGREGATABLE(CLastQuote)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CLastQuote)
	COM_INTERFACE_ENTRY(ILastQuote)
	COM_INTERFACE_ENTRY2(IDispatch, ILastQuote)
	COM_INTERFACE_ENTRY(IHHCommon)
	COM_INTERFACE_ENTRY(INetPacking)
END_COM_MAP()

// ILastQuote
public:
	IMPLEMENT_HH_COMMON_PROP

	IMPLEMENT_VARCHAR_PROP(Symbol, m_Symbol)
	IMPLEMENT_VARCHAR_PROP(Exchange, m_Exchange)
	IMPLEMENT_SIMPLE_PROP(SYMBOL_TYPE, Type, m_Type)
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

#endif //__LASTQUOTE_H_

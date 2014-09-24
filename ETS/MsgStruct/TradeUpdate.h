// TradeUpdate.h : Declaration of the CTradeUpdate

#ifndef __TRADEUPDATE_H_
#define __TRADEUPDATE_H_

#include "resource.h"       // main symbols
#include "MsgPacking.h"
#include "MsgPropImpl.h"

struct __TradeUpdateData
{
	BEGIN_PACKING
		FIELD_ENTRY(m_nSeqNum)
		FIELD_ENTRY(m_nTradeID)
		FIELD_ENTRY(m_enAction)
		FIELD_ENTRY(m_enTradeStatus)
		FIELD_ENTRY(m_nMark)
		FIELD_ENTRY(m_dtTradeDate)
		FIELD_ENTRY(m_nTraderID)
		FIELD_ENTRY(m_nSymbolID)
		FIELD_ENTRY(m_nQuantity)
		FIELD_ENTRY(m_dPrice)
		FIELD_ENTRY(m_nIsBuy)
		FIELD_ENTRY(m_nStrategyID)
		FIELD_ENTRY(m_nBrokerID)
		FIELD_ENTRY(m_dBrokerCommission)
		FIELD_ENTRY(m_nClearingBrokerID)
		FIELD_ENTRY(m_dClearingBrokerCommission)
		FIELD_ENTRY(m_dSpotReference)
		FIELD_ENTRY(m_dTradedIV)
		FIELD_ENTRY(m_bIsPosition)

		FIELD_ENTRY(m_enSymbolType)
		FIELD_ENTRY(m_sSymbol)
		FIELD_ENTRY(m_nUndSymbolID)
		FIELD_ENTRY(m_sUndSymbol)
		FIELD_ENTRY(m_dtExpiry)
		FIELD_ENTRY(m_nIsCall)
		FIELD_ENTRY(m_dStrike)
		FIELD_ENTRY(m_dPriceClose)
		FIELD_ENTRY(m_dPriceTheoClose)
		FIELD_ENTRY(m_nLotSize)
		FIELD_ENTRY(m_nOptionRootID)
		FIELD_ENTRY(m_nFutID)
		FIELD_ENTRY(m_nFutRootID)
		FIELD_ENTRY(m_enMsState)
		FIELD_ENTRY(m_sImportID)
		FIELD_ENTRY(m_nOrderID)
		FIELD_ENTRY(m_nExecStatus)
		FIELD_ENTRY(m_dtExpiryOV)
		FIELD_ENTRY(m_dtTradingClose)
	END_PACKING

	// trade data
	LongF							m_nSeqNum;
	LongF							m_nTradeID;
	TStaticField<TRADE_ACTION>		m_enAction;
	TStaticField<TRADE_STATUS>		m_enTradeStatus;
	LongF							m_nMark;
	DateF							m_dtTradeDate;
	LongF							m_nTraderID;
	LongF							m_nSymbolID;
	LongF							m_nQuantity;
	DoubleF							m_dPrice;
	LongF							m_nIsBuy;
	LongF							m_nStrategyID;
	LongF							m_nBrokerID;
	DoubleF							m_dBrokerCommission;
	LongF							m_nClearingBrokerID;
	DoubleF							m_dClearingBrokerCommission;
	DoubleF							m_dSpotReference;
	DoubleF							m_dTradedIV;
	BoolF							m_bIsPosition;
	// auxilary data
	TStaticField<SYMBOL_TYPE>		m_enSymbolType;
	StringF							m_sSymbol;
	LongF							m_nUndSymbolID;
	StringF							m_sUndSymbol;
	DateF							m_dtExpiry;
	LongF							m_nIsCall;
	DoubleF							m_dStrike;
	DoubleF							m_dPriceClose;
	DoubleF							m_dPriceTheoClose;
	LongF							m_nLotSize;
	LongF							m_nOptionRootID;
	LongF							m_nFutID;
	LongF							m_nFutRootID;
	TStaticField<TRADE_MS_STATE>	m_enMsState;
	StringF							m_sImportID;
	LongF							m_nOrderID;
	LongF							m_nExecStatus;
	DateF							m_dtExpiryOV;
	DateF							m_dtTradingClose;
};

/////////////////////////////////////////////////////////////////////////////
// CTradeUpdate
class ATL_NO_VTABLE CTradeUpdate : 
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CTradeUpdate, &CLSID_TradeUpdate>,
	public IDispatchImpl<ITradeUpdate, &IID_ITradeUpdate, &LIBID_MSGSTRUCTLib>,
	public INetPackingConstSizeDataImpl<CTradeUpdate, __TradeUpdateData>
{
public:
	CTradeUpdate()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_TRADEUPDATE)
DECLARE_NOT_AGGREGATABLE(CTradeUpdate)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CTradeUpdate)
	COM_INTERFACE_ENTRY(ITradeUpdate)
	COM_INTERFACE_ENTRY2(IDispatch, ITradeUpdate)
	COM_INTERFACE_ENTRY(INetPacking)
END_COM_MAP()

// ITradeUpdate
public:
	IMPLEMENT_LONG_PROP(SeqNum, m_nSeqNum)
	IMPLEMENT_LONG_PROP(TradeID, m_nTradeID)
	IMPLEMENT_SIMPLE_PROP(TRADE_ACTION, Action, m_enAction)
	IMPLEMENT_SIMPLE_PROP(TRADE_STATUS, TradeStatus, m_enTradeStatus)
	IMPLEMENT_LONG_PROP(Mark, m_nMark)
	IMPLEMENT_DATE_PROP(TradeDate, m_dtTradeDate)
	IMPLEMENT_LONG_PROP(TraderID, m_nTraderID)
	IMPLEMENT_LONG_PROP(SymbolID, m_nSymbolID)
	IMPLEMENT_LONG_PROP(Quantity, m_nQuantity)
	IMPLEMENT_DOUBLE_PROP(Price, m_dPrice)
	IMPLEMENT_LONG_PROP(IsBuy, m_nIsBuy)
	IMPLEMENT_LONG_PROP(StrategyID, m_nStrategyID)
	IMPLEMENT_LONG_PROP(BrokerID, m_nBrokerID)
	IMPLEMENT_DOUBLE_PROP(BrokerCommission, m_dBrokerCommission)
	IMPLEMENT_LONG_PROP(ClearingBrokerID, m_nClearingBrokerID)
	IMPLEMENT_DOUBLE_PROP(ClearingBrokerCommission, m_dClearingBrokerCommission)
	IMPLEMENT_DOUBLE_PROP(SpotReference, m_dSpotReference)
	IMPLEMENT_DOUBLE_PROP(TradedIV, m_dTradedIV)
	IMPLEMENT_BOOL_PROP(IsPosition, m_bIsPosition)

	IMPLEMENT_SIMPLE_PROP(SYMBOL_TYPE, SymbolType, m_enSymbolType)
	IMPLEMENT_VARCHAR_PROP(Symbol, m_sSymbol)
	IMPLEMENT_LONG_PROP(UndSymbolID, m_nUndSymbolID)
	IMPLEMENT_VARCHAR_PROP(UndSymbol, m_sUndSymbol)
	IMPLEMENT_DATE_PROP(Expiry, m_dtExpiry)
	IMPLEMENT_LONG_PROP(IsCall, m_nIsCall)
	IMPLEMENT_DOUBLE_PROP(Strike, m_dStrike)
	IMPLEMENT_DOUBLE_PROP(PriceClose, m_dPriceClose)
	IMPLEMENT_DOUBLE_PROP(PriceTheoClose, m_dPriceTheoClose)
	IMPLEMENT_LONG_PROP(LotSize, m_nLotSize)
	IMPLEMENT_LONG_PROP(OptionRootID, m_nOptionRootID)
	IMPLEMENT_LONG_PROP(FutID, m_nFutID)
	IMPLEMENT_LONG_PROP(FutRootID, m_nFutRootID)
	IMPLEMENT_SIMPLE_PROP(TRADE_MS_STATE, MsState, m_enMsState)
	IMPLEMENT_VARCHAR_PROP(ImportID, m_sImportID)
	IMPLEMENT_LONG_PROP(OrderID, m_nOrderID)
	IMPLEMENT_LONG_PROP(ExecStatus, m_nExecStatus)
	IMPLEMENT_DATE_PROP(ExpiryOV, m_dtExpiryOV)
	IMPLEMENT_DATE_PROP(TradingClose, m_dtTradingClose)
};

#endif //__TRADEUPDATE_H_

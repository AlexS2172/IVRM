// Trade.h : Declaration of the CTrade

#ifndef __TRADE_H_
#define __TRADE_H_

#include "resource.h"       // main symbols
#include "hhstruct.h"
#include "HHCommonProps.h"
#include "MsgPropImpl.h"

/////////////////////////////////////////////////////////////////////////////
// CTrade
class ATL_NO_VTABLE CTrade : 
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CTrade, &CLSID_Trade>,
	public IDispatchImpl<ITrade, &IID_ITrade, &LIBID_MSGSTRUCTLib>,
	public INetPackingConstSizeDataImpl<CTrade, __TradeData>
{
public:
	CTrade()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_TRADE)
DECLARE_NOT_AGGREGATABLE(CTrade)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CTrade)
	COM_INTERFACE_ENTRY(ITrade)
	COM_INTERFACE_ENTRY2(IDispatch, ITrade)
	COM_INTERFACE_ENTRY(IHHCommon)
	COM_INTERFACE_ENTRY(INetPacking)
END_COM_MAP()

// ITrade
public:
	IMPLEMENT_HH_COMMON_PROP

	IMPLEMENT_LONG_PROP(TradeSeqNum, m_nTradeSeqNum);
	IMPLEMENT_LONG_PROP(TradeID, m_nTradeID);
	IMPLEMENT_LONG_PROP(ContractID, m_nContractID);
	IMPLEMENT_LONG_PROP(IsClose, m_IsClose);
	IMPLEMENT_DATE_PROP(TradeDate, m_dtTradeDate);
	IMPLEMENT_LONG_PROP(Quantity, m_nQuantity);
	IMPLEMENT_DOUBLE_PROP(Price, m_dPrice);
	IMPLEMENT_LONG_PROP(IsBuy, m_IsBuy);
	IMPLEMENT_LONG_PROP(BookID, m_nBookID);
	IMPLEMENT_LONG_PROP(StrategyID, m_nStrategyID);
	IMPLEMENT_LONG_PROP(TraderID, m_nTraderID);
	IMPLEMENT_LONG_PROP(BrokerID, m_nBrokerID);
	IMPLEMENT_LONG_PROP(ClearingBrokerID, m_nClearingBrokerID);
	IMPLEMENT_LONG_PROP(BrokerCommissionID, m_nBrokerCommissionID);
	IMPLEMENT_LONG_PROP(ClearingBrokerCommissionID, m_nClearingBrokerCommissionID);
	IMPLEMENT_DOUBLE_PROP(TradedIV, m_dTradedIV);
	IMPLEMENT_DOUBLE_PROP(SpotReference, m_dSpotReference);
	IMPLEMENT_LONG_PROP(IsPosition, m_nIsPosition)
	IMPLEMENT_SIMPLE_PROP(TRADE_STATUS, TradeStatus, m_enTradeStatus)
};

#endif //__TRADE_H_

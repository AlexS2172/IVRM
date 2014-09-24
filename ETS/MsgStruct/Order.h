// Order.h : Declaration of the COrder

#ifndef __ORDER_H__
#define __ORDER_H__

#include "resource.h"       // main symbols
#include "MsgPropImpl.h"

struct __OrderData
{
	BEGIN_PACKING
		FIELD_ENTRY(m_enFixProvider)
		FIELD_ENTRY(m_nSeqNum)
		FIELD_ENTRY(m_nId)
		FIELD_ENTRY(m_sRequestUid)

		FIELD_ENTRY(m_sClOrdID)
		FIELD_ENTRY(m_sOrigClOrdID)
		FIELD_ENTRY(m_enSymbolType)
		FIELD_ENTRY(m_sSymbol)
		FIELD_ENTRY(m_nIsCall)
		FIELD_ENTRY(m_dtExpiry)
		FIELD_ENTRY(m_dStrike)
		FIELD_ENTRY(m_sAccount)
		FIELD_ENTRY(m_nIsBuy)
		FIELD_ENTRY(m_sExecBroker)
		FIELD_ENTRY(m_enOrderType)
		FIELD_ENTRY(m_nQty)
		FIELD_ENTRY(m_dPrice)
		FIELD_ENTRY(m_dtSendingTime)
		FIELD_ENTRY(m_dtTransactTime)
		FIELD_ENTRY(m_sExDestination)
		FIELD_ENTRY(m_dAvgPrice)
		FIELD_ENTRY(m_nCumQty)
		FIELD_ENTRY(m_nLeavesQty)
		FIELD_ENTRY(m_sText)
		FIELD_ENTRY(m_enExecType)
		FIELD_ENTRY(m_enOrdStatus)
		FIELD_ENTRY(m_sOptionRootSymbol)

		FIELD_ENTRY(m_nUndID)
		FIELD_ENTRY(m_nOptionID)
		FIELD_ENTRY(m_nOptionRootID)
		FIELD_ENTRY(m_nTraderID)
		FIELD_ENTRY(m_nDestID)

	END_PACKING

	TStaticField<MsFixProviderEnum>	m_enFixProvider;

	LongF							m_nSeqNum;
	LongF							m_nId;
	StringF							m_sRequestUid;

	StringF							m_sClOrdID;
	StringF							m_sOrigClOrdID;
	TStaticField<SYMBOL_TYPE>		m_enSymbolType;
	StringF							m_sSymbol;
	LongF							m_nIsCall;
	DateF							m_dtExpiry;
	DoubleF							m_dStrike;
	StringF							m_sAccount;
	LongF							m_nIsBuy;
	StringF							m_sExecBroker;
	TStaticField<MsOrderTypeEnum>	m_enOrderType;
	LongF							m_nQty;
	DoubleF							m_dPrice;
	DateF							m_dtSendingTime;
	DateF							m_dtTransactTime;
	StringF							m_sExDestination;
	DoubleF							m_dAvgPrice;
	LongF							m_nCumQty;
	LongF							m_nLeavesQty;
	StringF							m_sText;
	TStaticField<MsOrderStatusEnum>	m_enExecType;
	TStaticField<MsOrderStatusEnum>	m_enOrdStatus;
	StringF							m_sOptionRootSymbol;

	LongF							m_nUndID;
	LongF							m_nOptionID;
	LongF							m_nOptionRootID;
	LongF							m_nTraderID;
	LongF							m_nDestID;
};

/////////////////////////////////////////////////////////////////////////////
// COrder
class ATL_NO_VTABLE COrder : 
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<COrder, &CLSID_Order>,
	public IDispatchImpl<IOrder, &IID_IOrder, &LIBID_MSGSTRUCTLib>,
    public IPersistImpl<COrder>,
	public INetPackingConstSizeDataImpl<COrder, __OrderData>,
	public IDispatchImpl<IUIDGenerator, &IID_IUIDGenerator, &LIBID_MSGSTRUCTLib>
{
public:
	COrder()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_ORDER)
DECLARE_NOT_AGGREGATABLE(COrder)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(COrder)
	COM_INTERFACE_ENTRY(IOrder)
	COM_INTERFACE_ENTRY2(IDispatch, IOrder)
	COM_INTERFACE_ENTRY(INetPacking)
	COM_INTERFACE_ENTRY(IUIDGenerator)
	COM_INTERFACE_ENTRY(IPersist)
END_COM_MAP()


public:
	IMPLEMENT_SIMPLE_PROP(MsFixProviderEnum, FixProvider, m_enFixProvider)

	IMPLEMENT_LONG_PROP(SeqNum, m_nSeqNum)
	IMPLEMENT_LONG_PROP(Id, m_nId)
	IMPLEMENT_VARCHAR_PROP(RequestUid, m_sRequestUid)

	IMPLEMENT_VARCHAR_PROP(ClOrdID, m_sClOrdID)
	IMPLEMENT_VARCHAR_PROP(OrigClOrdID, m_sOrigClOrdID)

	IMPLEMENT_SIMPLE_PROP(SYMBOL_TYPE, SymbolType, m_enSymbolType)
	IMPLEMENT_VARCHAR_PROP(Symbol, m_sSymbol)
	IMPLEMENT_LONG_PROP(IsCall, m_nIsCall)
	IMPLEMENT_DATE_PROP(Expiry, m_dtExpiry)
	IMPLEMENT_DOUBLE_PROP(Strike, m_dStrike)
	IMPLEMENT_VARCHAR_PROP(Account, m_sAccount)
	IMPLEMENT_LONG_PROP(IsBuy, m_nIsBuy)
	IMPLEMENT_VARCHAR_PROP(ExecBroker, m_sExecBroker)
	IMPLEMENT_SIMPLE_PROP(MsOrderTypeEnum, OrderType, m_enOrderType)
	IMPLEMENT_LONG_PROP(Qty, m_nQty)
	IMPLEMENT_DOUBLE_PROP(Price, m_dPrice)
	IMPLEMENT_DATE_PROP(SendingTime, m_dtSendingTime)
	IMPLEMENT_DATE_PROP(TransactTime, m_dtTransactTime)
	IMPLEMENT_VARCHAR_PROP(ExDestination, m_sExDestination)
	IMPLEMENT_DOUBLE_PROP(AvgPrice, m_dAvgPrice)
	IMPLEMENT_LONG_PROP(CumQty, m_nCumQty)
	IMPLEMENT_LONG_PROP(LeavesQty, m_nLeavesQty)
	IMPLEMENT_VARCHAR_PROP(Text, m_sText)
	IMPLEMENT_SIMPLE_PROP(MsOrderStatusEnum, ExecType, m_enExecType)
	IMPLEMENT_SIMPLE_PROP(MsOrderStatusEnum, OrdStatus, m_enOrdStatus)
	IMPLEMENT_VARCHAR_PROP(OptionRootSymbol, m_sOptionRootSymbol)

	IMPLEMENT_LONG_PROP(UndID, m_nUndID)
	IMPLEMENT_LONG_PROP(OptionID, m_nOptionID)
	IMPLEMENT_LONG_PROP(OptionRootID, m_nOptionRootID)
	IMPLEMENT_LONG_PROP(TraderID, m_nTraderID)
	IMPLEMENT_LONG_PROP(DestID, m_nDestID)

	STDMETHOD(GenerateUID)(BSTR* pUID);
};

#endif //__ORDER_H__

// ExecutionReport.h : Declaration of the CExecutionReport

#ifndef __EXECUTIONREPORT_H__
#define __EXECUTIONREPORT_H__

#include "resource.h"       // main symbols
#include "MsgPropImpl.h"

struct __ExecutionReportData
{
	BEGIN_PACKING
		FIELD_ENTRY(m_nSeqNum)
		FIELD_ENTRY(m_nId)
		FIELD_ENTRY(m_nOrderId)
		FIELD_ENTRY(m_sExecID)
		FIELD_ENTRY(m_sExecRefID)
		FIELD_ENTRY(m_enExecType)
		FIELD_ENTRY(m_enOrdStatus)
		FIELD_ENTRY(m_dLastPrice)
		FIELD_ENTRY(m_nLastShares)
		FIELD_ENTRY(m_dAvgPrice)
		FIELD_ENTRY(m_nCumQty)
		FIELD_ENTRY(m_nLeavesQty)
		FIELD_ENTRY(m_dtTransactTime)
		FIELD_ENTRY(m_sText)
	END_PACKING

	LongF	m_nSeqNum;
	LongF	m_nId;
	LongF	m_nOrderId;
	StringF	m_sExecID;
	StringF	m_sExecRefID;
	TStaticField<MsOrderStatusEnum>	m_enExecType;
	TStaticField<MsOrderStatusEnum>	m_enOrdStatus;
	DoubleF	m_dLastPrice;
	LongF	m_nLastShares;
	DoubleF	m_dAvgPrice;
	LongF	m_nCumQty;
	LongF	m_nLeavesQty;
	DateF	m_dtTransactTime;
	StringF	m_sText;
};

/////////////////////////////////////////////////////////////////////////////
// CExecutionReport
class ATL_NO_VTABLE CExecutionReport : 
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CExecutionReport, &CLSID_ExecutionReport>,
	public IDispatchImpl<IExecutionReport, &IID_IExecutionReport, &LIBID_MSGSTRUCTLib>,
	public IPersistImpl<CExecutionReport>,
	public INetPackingConstSizeDataImpl<CExecutionReport, __ExecutionReportData>
{
public:
	CExecutionReport()
	{
	}

	DECLARE_REGISTRY_RESOURCEID(IDR_EXECUTIONREPORT)
	DECLARE_NOT_AGGREGATABLE(CExecutionReport)

	DECLARE_PROTECT_FINAL_CONSTRUCT()

	BEGIN_COM_MAP(CExecutionReport)
		COM_INTERFACE_ENTRY(IExecutionReport)
		COM_INTERFACE_ENTRY2(IDispatch, IExecutionReport)
		COM_INTERFACE_ENTRY(INetPacking)
		COM_INTERFACE_ENTRY(IPersist)
	END_COM_MAP()

public:
		IMPLEMENT_LONG_PROP(SeqNum, m_nSeqNum)
		IMPLEMENT_LONG_PROP(Id, m_nId)
		IMPLEMENT_LONG_PROP(OrderId, m_nOrderId)

		IMPLEMENT_VARCHAR_PROP(ExecID, m_sExecID)
		IMPLEMENT_VARCHAR_PROP(ExecRefID, m_sExecRefID)
		IMPLEMENT_SIMPLE_PROP(MsOrderStatusEnum, ExecType, m_enExecType)
		IMPLEMENT_SIMPLE_PROP(MsOrderStatusEnum, OrdStatus, m_enOrdStatus)
		IMPLEMENT_DOUBLE_PROP(LastPrice, m_dLastPrice)
		IMPLEMENT_LONG_PROP(LastShares, m_nLastShares)
		IMPLEMENT_DOUBLE_PROP(AvgPrice, m_dAvgPrice)
		IMPLEMENT_LONG_PROP(CumQty, m_nCumQty)
		IMPLEMENT_LONG_PROP(LeavesQty, m_nLeavesQty)
		IMPLEMENT_DATE_PROP(TransactTime, m_dtTransactTime)
		IMPLEMENT_VARCHAR_PROP(Text, m_sText)
};

#endif //__EXECUTIONREPORT_H__

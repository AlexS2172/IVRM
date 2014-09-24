// Stock.h : Declaration of the CStock

#ifndef __STOCK_H_
#define __STOCK_H_

#include "resource.h"       // main symbols
#include "HHStruct.h"
#include "HHCommonProps.h"
#include "MsgPropImpl.h"


/////////////////////////////////////////////////////////////////////////////
// CStock
class ATL_NO_VTABLE CStock : 
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CStock, &CLSID_Stock>,
	public IDispatchImpl<IStock, &IID_IStock, &LIBID_MSGSTRUCTLib>,
	public INetPackingConstSizeDataImpl<CStock, __StockData>
{
public:
	CStock()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_STOCK)
DECLARE_NOT_AGGREGATABLE(CStock)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CStock)
	COM_INTERFACE_ENTRY(IStock)
	COM_INTERFACE_ENTRY2(IDispatch, IStock)
	COM_INTERFACE_ENTRY(IHHCommon)
	COM_INTERFACE_ENTRY(INetPacking)
END_COM_MAP()


// IStock
public:
	IMPLEMENT_HH_COMMON_PROP

	IMPLEMENT_LONG_PROP(ContractID, m_nID)
	IMPLEMENT_LONG_PROP(IsHTB, m_IsHTB)
	IMPLEMENT_LONG_PROP(PrimaryExchangeID, m_nPrimaryExchangeID)
	IMPLEMENT_LONG_PROP(DivFreq, m_nDivFreq)
	IMPLEMENT_DATE_PROP(DivDate, m_dtDivDate)
	IMPLEMENT_DOUBLE_PROP(DivAmt, m_dDivAmt)
	IMPLEMENT_LONG_PROP(IsDivCustom, m_nIsDivCustom)
	IMPLEMENT_LONG_PROP(DivFreqCustom, m_nDivFreqCustom)
	IMPLEMENT_DATE_PROP(DivDateCustom, m_dtDivDateCustom)
	IMPLEMENT_DOUBLE_PROP(DivAmtCustom, m_dDivAmtCustom)
	IMPLEMENT_LONG_PROP(CalcModelType, m_nCalcModelType)
	IMPLEMENT_LONG_PROP(CalcOptionType, m_nCalcOptionType)
	IMPLEMENT_LONG_PROP(IsActive, m_nIsActive)
	IMPLEMENT_DOUBLE_PROP(Skew, m_dSkew)
	IMPLEMENT_DOUBLE_PROP(Kurt, m_dKurt)
};

#endif //__STOCK_H_

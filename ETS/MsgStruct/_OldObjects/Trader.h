// Trader.h : Declaration of the CTrader

#ifndef __TRADER_H_
#define __TRADER_H_

#include "resource.h"       // main symbols
#include "hhstruct.h"
#include "HHCommonProps.h"
#include "MsgPropImpl.h"


/////////////////////////////////////////////////////////////////////////////
// CTrader
class ATL_NO_VTABLE CTrader : 
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CTrader, &CLSID_Trader>,
	public IDispatchImpl<ITrader, &IID_ITrader, &LIBID_MSGSTRUCTLib>,
	public INetPackingConstSizeDataImpl<CTrader, __TraderData>
{
public:
	CTrader()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_TRADER)
DECLARE_NOT_AGGREGATABLE(CTrader)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CTrader)
	COM_INTERFACE_ENTRY(ITrader)
	COM_INTERFACE_ENTRY2(IDispatch, ITrader)
	COM_INTERFACE_ENTRY(IHHCommon)
	COM_INTERFACE_ENTRY(INetPacking)
END_COM_MAP()

// ITrader
public:
	IMPLEMENT_HH_COMMON_PROP

	IMPLEMENT_LONG_PROP(TraderID, m_nID)
	IMPLEMENT_VARCHAR_PROP(TraderName, m_Name)
	IMPLEMENT_VARCHAR_PROP(Acronym, m_Acronym)
};

#endif //__TRADER_H_

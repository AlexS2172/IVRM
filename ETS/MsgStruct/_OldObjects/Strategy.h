// Strategy.h : Declaration of the CStrategy

#ifndef __STRATEGY_H_
#define __STRATEGY_H_

#include "resource.h"       // main symbols
#include "HHStruct.h"
#include "HHCommonProps.h"
#include "MsgPropImpl.h"

/////////////////////////////////////////////////////////////////////////////
// CStrategy
class ATL_NO_VTABLE CStrategy : 
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CStrategy, &CLSID_Strategy>,
	public IDispatchImpl<IStrategy, &IID_IStrategy, &LIBID_MSGSTRUCTLib>,
	public INetPackingConstSizeDataImpl<CStrategy, __StrategyData>
{
public:
	CStrategy()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_STRATEGY)
DECLARE_NOT_AGGREGATABLE(CStrategy)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CStrategy)
	COM_INTERFACE_ENTRY(IStrategy)
	COM_INTERFACE_ENTRY2(IDispatch, IStrategy)
	COM_INTERFACE_ENTRY(IHHCommon)
	COM_INTERFACE_ENTRY(INetPacking)
END_COM_MAP()

// IStrategy
public:
	IMPLEMENT_HH_COMMON_PROP

	IMPLEMENT_LONG_PROP(StrategyID, m_nID)
	IMPLEMENT_VARCHAR_PROP(StrategyName, m_Name)
	IMPLEMENT_VARCHAR_PROP(Description, m_Description)
};

#endif //__STRATEGY_H_

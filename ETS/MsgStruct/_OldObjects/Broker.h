// Broker.h : Declaration of the CBroker

#ifndef __BROKER_H_
#define __BROKER_H_

#include "resource.h"       // main symbols
#include "HHStruct.h"
#include "HHCommonProps.h"
#include "MsgPropImpl.h"

/////////////////////////////////////////////////////////////////////////////
// CBroker
class ATL_NO_VTABLE CBroker : 
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CBroker, &CLSID_Broker>,
	public IDispatchImpl<IBroker, &IID_IBroker, &LIBID_MSGSTRUCTLib>,
	public INetPackingConstSizeDataImpl<CBroker, __BrokerData>
{
public:
	CBroker()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_BROKER)
DECLARE_NOT_AGGREGATABLE(CBroker)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CBroker)
	COM_INTERFACE_ENTRY(IBroker)
	COM_INTERFACE_ENTRY2(IDispatch, IBroker)
	COM_INTERFACE_ENTRY(IHHCommon)
	COM_INTERFACE_ENTRY(INetPacking)
END_COM_MAP()

// IBroker
public:
	IMPLEMENT_HH_COMMON_PROP

	IMPLEMENT_LONG_PROP(BrokerID, m_nID)
	IMPLEMENT_VARCHAR_PROP(BrokerName, m_Name)
	IMPLEMENT_VARCHAR_PROP(Description, m_Description)
	IMPLEMENT_LONG_PROP(IsClearing, m_IsClearing)
};

#endif //__BROKER_H_

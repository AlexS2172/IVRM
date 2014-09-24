// BroadcastMessage.h : Declaration of the CBroadcastMessage

#ifndef __BROADCASTMESSAGE_H_
#define __BROADCASTMESSAGE_H_

#include "resource.h"       // main symbols
#include "MsgPacking.h"
#include "MsgPropImpl.h"

struct __BroadcastMessageData
{
	BEGIN_PACKING
		FIELD_ENTRY(m_enSeverity)
		FIELD_ENTRY(m_enFacility)
		FIELD_ENTRY(m_enPriority)
		FIELD_ENTRY(m_nCode)
		FIELD_ENTRY(m_sDescription)
	END_PACKING

	TStaticField<MSG_SEVERITY>	m_enSeverity;
	TStaticField<MSG_FACILITY>	m_enFacility;
	TStaticField<MSG_PRIORITY>	m_enPriority;
    LongF                   m_nCode;
    StringF                 m_sDescription;
};

/////////////////////////////////////////////////////////////////////////////
// CBroadcastMessage
class ATL_NO_VTABLE CBroadcastMessage : 
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CBroadcastMessage, &CLSID_BroadcastMessage>,
	public IDispatchImpl<IBroadcastMessage, &IID_IBroadcastMessage, &LIBID_MSGSTRUCTLib>,
	public INetPackingConstSizeDataImpl<CBroadcastMessage, __BroadcastMessageData>
{
public:
	CBroadcastMessage()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_BROADCASTMESSAGE)
DECLARE_NOT_AGGREGATABLE(CBroadcastMessage)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CBroadcastMessage)
	COM_INTERFACE_ENTRY(IBroadcastMessage)
	COM_INTERFACE_ENTRY2(IDispatch, IBroadcastMessage)
	COM_INTERFACE_ENTRY(INetPacking)
END_COM_MAP()

// IBroadcastMessage
public:
	IMPLEMENT_SIMPLE_PROP(MSG_SEVERITY, Severity, m_enSeverity)
	IMPLEMENT_SIMPLE_PROP(MSG_FACILITY, Facility, m_enFacility)
	IMPLEMENT_SIMPLE_PROP(MSG_PRIORITY, Priority, m_enPriority)
	IMPLEMENT_LONG_PROP(Code, m_nCode)
	IMPLEMENT_VARCHAR_PROP(Description, m_sDescription)
};

#endif //__BROADCASTMESSAGE_H_

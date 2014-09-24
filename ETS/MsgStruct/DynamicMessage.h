// DynamicMessage.h : Declaration of the CDynamicMessage

#ifndef __DynamicMessage_H_
#define __DynamicMessage_H_

#include "resource.h"       // main symbols
#include "MsgPropImpl.h"

struct __DynamicMessageData
{
	BEGIN_PACKING
		FIELD_ENTRY(m_Value)
	END_PACKING

    MapF<WStringF, VariantF>  m_Value;
};

/////////////////////////////////////////////////////////////////////////////
// CDynamicMessage
class ATL_NO_VTABLE CDynamicMessage : 
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CDynamicMessage, &CLSID_DynamicMessage>,
	public IDispatchImpl<IDynamicMessage, &IID_IDynamicMessage, &LIBID_MSGSTRUCTLib>,
	public INetPackingConstSizeDataImpl<CDynamicMessage, __DynamicMessageData>
{
public:
	CDynamicMessage()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_DYNAMICMESSAGE)
DECLARE_NOT_AGGREGATABLE(CDynamicMessage)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CDynamicMessage)
	COM_INTERFACE_ENTRY(IDynamicMessage)
	COM_INTERFACE_ENTRY2(IDispatch, IDynamicMessage)
	COM_INTERFACE_ENTRY(INetPacking)
END_COM_MAP()

// IDynamicMessage
public:
    IMPLEMENT_MAP_PROP(Value, VariantF, VARIANT)
};

#endif //__DynamicMessage_H_

// FixGatewayStatusRequest.h : Declaration of the CFixGatewayStatusRequest
#ifndef __FIXGATEWAYSTATUSREQUEST_H_
#define __FIXGATEWAYSTATUSREQUEST_H_

#include "resource.h"       // main symbols
#include "MsgPacking.h"
#include "MsgPropImpl.h"

struct __FixGatewayStatusRequestData
{
	BEGIN_PACKING
		FIELD_ENTRY(m_nClientType)
	END_PACKING

	LongF		m_nClientType;
};

// CFixGatewayStatusRequest

class ATL_NO_VTABLE CFixGatewayStatusRequest : 
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CFixGatewayStatusRequest, &CLSID_FixGatewayStatusRequest>,
	public IDispatchImpl<IFixGatewayStatusRequest, &IID_IFixGatewayStatusRequest, &LIBID_MSGSTRUCTLib>,
	public INetPackingConstSizeDataImpl<CFixGatewayStatusRequest, __FixGatewayStatusRequestData>
{
public:
	CFixGatewayStatusRequest()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_FIXGATEWAYSTATUSREQUEST)

DECLARE_NOT_AGGREGATABLE(CFixGatewayStatusRequest)

BEGIN_COM_MAP(CFixGatewayStatusRequest)
	COM_INTERFACE_ENTRY(IFixGatewayStatusRequest)
	COM_INTERFACE_ENTRY2(IDispatch, IFixGatewayStatusRequest)
	COM_INTERFACE_ENTRY(INetPacking)
END_COM_MAP()

public:
	IMPLEMENT_LONG_PROP(ClientType, m_nClientType)
};

#endif // __FIXGATEWAYSTATUSREQUEST_H_
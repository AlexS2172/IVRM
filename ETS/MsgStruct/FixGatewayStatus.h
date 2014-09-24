// FixGatewayStatus.h : Declaration of the CFixGatewayStatus
#ifndef __FIXGATEWAYSTATUS_H_
#define __FIXGATEWAYSTATUS_H_

#include "resource.h"       // main symbols
#include "MsgPacking.h"
#include "MsgPropImpl.h"

struct __FixGatewayStatusData
{
	BEGIN_PACKING
		FIELD_ENTRY(m_sIP)
		FIELD_ENTRY(m_sHost)
		FIELD_ENTRY(m_bIsOnline)
	END_PACKING

	StringF		m_sIP;
	StringF		m_sHost;
	BoolF		m_bIsOnline;
};


// CFixGatewayStatus

class ATL_NO_VTABLE CFixGatewayStatus : 
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CFixGatewayStatus, &CLSID_FixGatewayStatus>,
	public IDispatchImpl<IFixGatewayStatus, &IID_IFixGatewayStatus, &LIBID_MSGSTRUCTLib>,
	public INetPackingConstSizeDataImpl<CFixGatewayStatus, __FixGatewayStatusData>
{
public:
	CFixGatewayStatus()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_FIXGATEWAYSTATUS)

DECLARE_NOT_AGGREGATABLE(CFixGatewayStatus)

BEGIN_COM_MAP(CFixGatewayStatus)
	COM_INTERFACE_ENTRY(IFixGatewayStatus)
	COM_INTERFACE_ENTRY2(IDispatch, IFixGatewayStatus)
	COM_INTERFACE_ENTRY(INetPacking)
END_COM_MAP()

public:
	IMPLEMENT_VARCHAR_PROP(IP, m_sIP)
	IMPLEMENT_VARCHAR_PROP(Host, m_sHost)
	IMPLEMENT_BOOL_PROP(IsOnline, m_bIsOnline)
};

#endif // __FIXGATEWAYSTATUS_H_
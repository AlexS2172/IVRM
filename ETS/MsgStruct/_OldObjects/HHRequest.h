// HHRequest.h : Declaration of the CHHRequest

#ifndef __HHREQUEST_H_
#define __HHREQUEST_H_

#include "resource.h"       // main symbols
#include "HHStruct.h"
#include "HHCommonProps.h"
#include "MsgPropImpl.h"

/////////////////////////////////////////////////////////////////////////////
// CHHRequest
class ATL_NO_VTABLE CHHRequest : 
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CHHRequest, &CLSID_HHRequest>,
	public IDispatchImpl<IHHRequest, &IID_IHHRequest, &LIBID_MSGSTRUCTLib>,
	public INetPackingConstSizeDataImpl<CHHRequest, __HHRequestData>
{
public:
	CHHRequest()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_HHREQUEST)
DECLARE_NOT_AGGREGATABLE(CHHRequest)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CHHRequest)
	COM_INTERFACE_ENTRY(IHHRequest)
	COM_INTERFACE_ENTRY2(IDispatch, IHHRequest)
	COM_INTERFACE_ENTRY(INetPacking)
END_COM_MAP()

// IHHRequest

public:
	IMPLEMENT_VARCHAR_PROP(UID, m_chUID)
	IMPLEMENT_VARCHAR_PROP(TraderName, m_chTraderName)
	IMPLEMENT_LONG_PROP(TraderID, m_nTraderID)
	IMPLEMENT_DATE_PROP(LastUpdateDate, m_dtLastUpdateDate)
	IMPLEMENT_DATE_PROP(LastUpdateDate2, m_dtLastUpdateDate2)
	
	IMPLEMENT_LONG_PROP(Param1, m_nParam1)
	IMPLEMENT_LONG_PROP(Param2, m_nParam2)
	IMPLEMENT_VARCHAR_PROP(StrParam1, m_StrParam1)
	IMPLEMENT_VARCHAR_PROP(StrParam2, m_StrParam2)
	IMPLEMENT_SIMPLE_PROP(HH_REQUEST_TYPE, Type, m_nType)
};

#endif //__HHREQUEST_H_

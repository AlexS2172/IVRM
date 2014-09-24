// Expiry.h : Declaration of the CExpiry

#ifndef __EXPIRY_H_
#define __EXPIRY_H_

#include "resource.h"       // main symbols
#include "HHStruct.h"
#include "HHCommonProps.h"
#include "MsgPropImpl.h"


/////////////////////////////////////////////////////////////////////////////
// CExpiry
class ATL_NO_VTABLE CExpiry : 
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CExpiry, &CLSID_Expiry>,
	public IDispatchImpl<IExpiry, &IID_IExpiry, &LIBID_MSGSTRUCTLib>,
	public INetPackingConstSizeDataImpl<CExpiry, __ExpiryData>
{
public:
	CExpiry()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_EXPIRY)
DECLARE_NOT_AGGREGATABLE(CExpiry)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CExpiry)
	COM_INTERFACE_ENTRY(IExpiry)
	COM_INTERFACE_ENTRY2(IDispatch, IExpiry)
	COM_INTERFACE_ENTRY(IHHCommon)
	COM_INTERFACE_ENTRY(INetPacking)
END_COM_MAP()

// IExpiry
public:
	IMPLEMENT_HH_COMMON_PROP

	IMPLEMENT_LONG_PROP(ExpiryID, m_nExpiryID)
	IMPLEMENT_VARCHAR_PROP(ExpiryPeriod, m_ExpiryPeriod)
};

#endif //__EXPIRY_H_

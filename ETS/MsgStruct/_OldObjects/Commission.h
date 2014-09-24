// Commission.h : Declaration of the CCommission

#ifndef __COMMISSION_H_
#define __COMMISSION_H_

#include "resource.h"       // main symbols
#include "HHStruct.h"
#include "HHCommonProps.h"
#include "MsgPropImpl.h"

/////////////////////////////////////////////////////////////////////////////
// CCommission
class ATL_NO_VTABLE CCommission : 
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CCommission, &CLSID_Commission>,
	public IDispatchImpl<ICommission, &IID_ICommission, &LIBID_MSGSTRUCTLib>,
	public INetPackingConstSizeDataImpl<CCommission, __CommissionData>
{
public:
	CCommission()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_COMMISSION)
DECLARE_NOT_AGGREGATABLE(CCommission)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CCommission)
	COM_INTERFACE_ENTRY(ICommission)
	COM_INTERFACE_ENTRY2(IDispatch, ICommission)
	COM_INTERFACE_ENTRY(IHHCommon)
	COM_INTERFACE_ENTRY(INetPacking)
END_COM_MAP()

// ICommission
public:
	IMPLEMENT_HH_COMMON_PROP

	IMPLEMENT_LONG_PROP(CommissionID, m_nID)
	IMPLEMENT_LONG_PROP(BrokerID, m_nBrokerID)
	IMPLEMENT_DATE_PROP(Date, m_Date)
	IMPLEMENT_DOUBLE_PROP(Value, m_dValue)
};

#endif //__COMMISSION_H_

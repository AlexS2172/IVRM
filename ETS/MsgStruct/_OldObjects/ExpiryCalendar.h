// ExpiryCalendar.h : Declaration of the CExpiryCalendar

#ifndef __EXPIRYCALENDAR_H_
#define __EXPIRYCALENDAR_H_

#include "resource.h"       // main symbols
#include "HHStruct.h"
#include "HHCommonProps.h"
#include "MsgPropImpl.h"

/////////////////////////////////////////////////////////////////////////////
// CExpiryCalendar
class ATL_NO_VTABLE CExpiryCalendar : 
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CExpiryCalendar, &CLSID_ExpiryCalendar>,
	public IDispatchImpl<IExpiryCalendar, &IID_IExpiryCalendar, &LIBID_MSGSTRUCTLib>,
	public INetPackingConstSizeDataImpl<CExpiryCalendar, __ExpiryCalendarData>
{
public:
	CExpiryCalendar()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_EXPIRYCALENDAR)
DECLARE_NOT_AGGREGATABLE(CExpiryCalendar)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CExpiryCalendar)
	COM_INTERFACE_ENTRY(IExpiryCalendar)
	COM_INTERFACE_ENTRY2(IDispatch, IExpiryCalendar)
	COM_INTERFACE_ENTRY(IHHCommon)
	COM_INTERFACE_ENTRY(INetPacking)
END_COM_MAP()

// IExpiryCalendar
public:
	IMPLEMENT_HH_COMMON_PROP

	IMPLEMENT_LONG_PROP(CalendarID, m_nCalendarID)
	IMPLEMENT_VARCHAR_PROP(CalendarName, m_CalendarName)
	IMPLEMENT_VARCHAR_PROP(Description, m_Description)
};

#endif //__EXPIRYCALENDAR_H_

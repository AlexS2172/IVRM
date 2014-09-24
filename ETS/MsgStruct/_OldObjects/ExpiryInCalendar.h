// ExpiryInCalendar.h : Declaration of the CExpiryInCalendar

#ifndef __EXPIRYINCALENDAR_H_
#define __EXPIRYINCALENDAR_H_

#include "resource.h"       // main symbols
#include "HHStruct.h"
#include "HHCommonProps.h"
#include "MsgPropImpl.h"

/////////////////////////////////////////////////////////////////////////////
// CExpiryInCalendar
class ATL_NO_VTABLE CExpiryInCalendar : 
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CExpiryInCalendar, &CLSID_ExpiryInCalendar>,
	public IDispatchImpl<IExpiryInCalendar, &IID_IExpiryInCalendar, &LIBID_MSGSTRUCTLib>,
	public INetPackingConstSizeDataImpl<CExpiryInCalendar, __ExpiryInCalendarData>
{
public:
	CExpiryInCalendar()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_EXPIRYINCALENDAR)
DECLARE_NOT_AGGREGATABLE(CExpiryInCalendar)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CExpiryInCalendar)
	COM_INTERFACE_ENTRY(IExpiryInCalendar)
	COM_INTERFACE_ENTRY2(IDispatch, IExpiryInCalendar)
	COM_INTERFACE_ENTRY(IHHCommon)
	COM_INTERFACE_ENTRY(INetPacking)
END_COM_MAP()

// IExpiryInCalendar
public:
	IMPLEMENT_HH_COMMON_PROP

	IMPLEMENT_LONG_PROP(ExpiryID, m_nExpiryID)
	IMPLEMENT_LONG_PROP(CalendarID, m_nCalendarID)
	IMPLEMENT_DATE_PROP(ExpiryDate, m_dtExpiryDate)
	IMPLEMENT_DOUBLE_PROP(VegaWeight, m_dVegaWeight)
};

#endif //__EXPIRYINCALENDAR_H_

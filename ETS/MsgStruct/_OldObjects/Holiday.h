// Holiday.h : Declaration of the CHoliday

#ifndef __HOLIDAY_H_
#define __HOLIDAY_H_

#include "resource.h"       // main symbols
#include "HHStruct.h"
#include "HHCommonProps.h"
#include "MsgPropImpl.h"

/////////////////////////////////////////////////////////////////////////////
// CHoliday
class ATL_NO_VTABLE CHoliday : 
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CHoliday, &CLSID_Holiday>,
	public IDispatchImpl<IHoliday, &IID_IHoliday, &LIBID_MSGSTRUCTLib>,
	public INetPackingConstSizeDataImpl<CHoliday, __HolidayData>
{
public:
	CHoliday()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_HOLIDAY)
DECLARE_NOT_AGGREGATABLE(CHoliday)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CHoliday)
	COM_INTERFACE_ENTRY(IHoliday)
	COM_INTERFACE_ENTRY2(IDispatch, IHoliday)
	COM_INTERFACE_ENTRY(IHHCommon)
	COM_INTERFACE_ENTRY(INetPacking)
END_COM_MAP()

// IHoliday
public:
	IMPLEMENT_HH_COMMON_PROP

	IMPLEMENT_LONG_PROP(HolidayID, m_nID)
	IMPLEMENT_VARCHAR_PROP(HolidayName, m_Name)
	IMPLEMENT_DATE_PROP(Date, m_Date)
};

#endif //__HOLIDAY_H_

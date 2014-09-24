#ifndef __EXCHANGEHOLIDAY_H_
#define __EXCHANGEHOLIDAY_H_

#include "resource.h"       // main symbols
#include "HHStruct.h"
#include "HHCommonProps.h"
#include "MsgPropImpl.h"

/////////////////////////////////////////////////////////////////////////////
// CExchangeHoliday
class ATL_NO_VTABLE CExchangeHoliday : 
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CExchangeHoliday, &CLSID_ExchangeHoliday>,
	public IDispatchImpl<IExchangeHoliday, &IID_IExchangeHoliday, &LIBID_MSGSTRUCTLib>,
	public INetPackingConstSizeDataImpl<CExchangeHoliday, __ExchangeHolidayData>
{
public:
	CExchangeHoliday()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_EXCHANGEHOLIDAY)
DECLARE_NOT_AGGREGATABLE(CExchangeHoliday)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CExchangeHoliday)
	COM_INTERFACE_ENTRY(IExchangeHoliday)
	COM_INTERFACE_ENTRY2(IDispatch, IExchangeHoliday)
	COM_INTERFACE_ENTRY(IHHCommon)
	COM_INTERFACE_ENTRY(INetPacking)
END_COM_MAP()

// IExchangeHoliday
public:
	IMPLEMENT_HH_COMMON_PROP

	IMPLEMENT_LONG_PROP(HolidayID, m_nHolidayID)
	IMPLEMENT_LONG_PROP(ExchangeID, m_nExchangeID)
};

#endif //__EXCHANGEHOLIDAY_H_

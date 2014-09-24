// CustomStrikeSkewPoint.h : Declaration of the CCustomStrikeSkewPoint

#ifndef __CUSTOMSTRIKESKEWPOINT_H_
#define __CUSTOMSTRIKESKEWPOINT_H_

#include "resource.h"       // main symbols
#include "hhstruct.h"
#include "HHCommonProps.h"
#include "MsgPropImpl.h"


/////////////////////////////////////////////////////////////////////////////
// CCustomStrikeSkewPoint
class ATL_NO_VTABLE CCustomStrikeSkewPoint : 
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CCustomStrikeSkewPoint, &CLSID_CustomStrikeSkewPoint>,
	public IDispatchImpl<ICustomStrikeSkewPoint, &IID_ICustomStrikeSkewPoint, &LIBID_MSGSTRUCTLib>,
	public INetPackingConstSizeDataImpl<CCustomStrikeSkewPoint, __CustomStrikeSkewPointData>
{
public:
	CCustomStrikeSkewPoint()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_CUSTOMSTRIKESKEWPOINT)
DECLARE_NOT_AGGREGATABLE(CCustomStrikeSkewPoint)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CCustomStrikeSkewPoint)
	COM_INTERFACE_ENTRY(ICustomStrikeSkewPoint)
	COM_INTERFACE_ENTRY2(IDispatch, ICustomStrikeSkewPoint)
	COM_INTERFACE_ENTRY(IHHCommon)
	COM_INTERFACE_ENTRY(INetPacking)
END_COM_MAP()

// ICustomStrikeSkewPoint
public:
	IMPLEMENT_HH_COMMON_PROP

	IMPLEMENT_LONG_PROP(CustomStrikeSkewPointID, m_nCustomStrikeSkewPointID)
	IMPLEMENT_LONG_PROP(VolaSurfaceDataID, m_nVolaSurfaceDataID)
	IMPLEMENT_DOUBLE_PROP(Strike, m_dStrike)
	IMPLEMENT_LONG_PROP(Type, m_nType)
	IMPLEMENT_DOUBLE_PROP(Volatility, m_dVolatility)
	IMPLEMENT_LONG_PROP(ExpiryID, m_nExpiryID)
	IMPLEMENT_LONG_PROP(IsBasePoint, m_nIsBasePoint)
};

#endif //__CUSTOMSTRIKESKEWPOINT_H_

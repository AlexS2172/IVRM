// VolaSurfaceData.h : Declaration of the CVolaSurfaceData

#ifndef __VOLASURFACEDATA_H_
#define __VOLASURFACEDATA_H_

#include "resource.h"       // main symbols
#include "hhstruct.h"
#include "HHCommonProps.h"
#include "MsgPropImpl.h"


/////////////////////////////////////////////////////////////////////////////
// CVolaSurfaceData
class ATL_NO_VTABLE CVolaSurfaceData : 
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CVolaSurfaceData, &CLSID_VolaSurfaceData>,
	public IDispatchImpl<IVolaSurfaceData, &IID_IVolaSurfaceData, &LIBID_MSGSTRUCTLib>,
	public INetPackingConstSizeDataImpl<CVolaSurfaceData, __VolaSurfaceData>
{
public:
	CVolaSurfaceData()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_VOLASURFACEDATA)
DECLARE_NOT_AGGREGATABLE(CVolaSurfaceData)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CVolaSurfaceData)
	COM_INTERFACE_ENTRY(IVolaSurfaceData)
	COM_INTERFACE_ENTRY2(IDispatch, IVolaSurfaceData)
	COM_INTERFACE_ENTRY(IHHCommon)
	COM_INTERFACE_ENTRY(INetPacking)
END_COM_MAP()

// IVolaSurfaceData
public:
	IMPLEMENT_HH_COMMON_PROP

	IMPLEMENT_LONG_PROP(VolaSurfaceDataID, m_nVolaSurfaceDataID)
	IMPLEMENT_LONG_PROP(ContractID, m_nContractID)
	IMPLEMENT_DOUBLE_PROP(BaseUnderlinePrice, m_dBaseUnderlinePrice)
	IMPLEMENT_DOUBLE_PROP(SmileAccelerator, m_dSmileAccelerator)
	IMPLEMENT_DOUBLE_PROP(InterpolationFactor, m_dInterpolationFactor)
	IMPLEMENT_DOUBLE_PROP(StrikeMoneynessHi, m_dStrikeMoneynessHi)
	IMPLEMENT_DOUBLE_PROP(StrikeMoneynessLo, m_dStrikeMoneynessLo)
	IMPLEMENT_VARCHAR_PROP(SurfaceName, m_SurfaceName)
	IMPLEMENT_LONG_PROP(IsPriceOverride, m_nIsPriceOverride)
	IMPLEMENT_LONG_PROP(IsDiscreteAcceleration, m_nIsDiscreteAcceleration)
	IMPLEMENT_LONG_PROP(IsDefaultSurface, m_nIsDefaultSurface)
	IMPLEMENT_DOUBLE_PROP(ATMVolatility, m_dATMVolatility)
};

#endif //__VOLASURFACEDATA_H_

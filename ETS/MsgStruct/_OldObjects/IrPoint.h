// IrPoint.h : Declaration of the CIrPoint

#ifndef __IRPOINT_H_
#define __IRPOINT_H_

#include "resource.h"       // main symbols
#include "HHStruct.h"
#include "HHCommonProps.h"
#include "MsgPropImpl.h"

/////////////////////////////////////////////////////////////////////////////
// CIrPoint
class ATL_NO_VTABLE CIrPoint : 
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CIrPoint, &CLSID_IrPoint>,
	public IDispatchImpl<IIrPoint, &IID_IIrPoint, &LIBID_MSGSTRUCTLib>,
	public INetPackingConstSizeDataImpl<CIrPoint, __IrPointData>
{
public:
	CIrPoint()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_IRPOINT)
DECLARE_NOT_AGGREGATABLE(CIrPoint)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CIrPoint)
	COM_INTERFACE_ENTRY(IIrPoint)
	COM_INTERFACE_ENTRY2(IDispatch, IIrPoint)
	COM_INTERFACE_ENTRY(IHHCommon)
	COM_INTERFACE_ENTRY(INetPacking)
END_COM_MAP()

// IIrPoint
public:
	IMPLEMENT_HH_COMMON_PROP

	IMPLEMENT_LONG_PROP(PointID, m_nPointID)
	IMPLEMENT_LONG_PROP(CurveID, m_nCurveID)
	IMPLEMENT_LONG_PROP(PeriodTypeID, m_nPeriodTypeID)
	IMPLEMENT_LONG_PROP(Num, m_nNum)
	IMPLEMENT_DOUBLE_PROP(ShortRate, m_dShortRate)
	IMPLEMENT_DOUBLE_PROP(HTBRate, m_dHTBRate)
	IMPLEMENT_DOUBLE_PROP(LongRate, m_dLongRate)
};

#endif //__IRPOINT_H_

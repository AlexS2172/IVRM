// IrCurve.h : Declaration of the CIrCurve

#ifndef __IRCURVE_H_
#define __IRCURVE_H_

#include "resource.h"       // main symbols
#include "HHStruct.h"
#include "HHCommonProps.h"
#include "MsgPropImpl.h"

/////////////////////////////////////////////////////////////////////////////
// CIrCurve
class ATL_NO_VTABLE CIrCurve : 
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CIrCurve, &CLSID_IrCurve>,
	public IDispatchImpl<IIrCurve, &IID_IIrCurve, &LIBID_MSGSTRUCTLib>,
	public INetPackingConstSizeDataImpl<CIrCurve, __IrCurveData>
{
public:
	CIrCurve()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_IRCURVE)
DECLARE_NOT_AGGREGATABLE(CIrCurve)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CIrCurve)
	COM_INTERFACE_ENTRY(IIrCurve)
	COM_INTERFACE_ENTRY2(IDispatch, IIrCurve)
	COM_INTERFACE_ENTRY(IHHCommon)
	COM_INTERFACE_ENTRY(INetPacking)
END_COM_MAP()

// IIrCurve
public:
	IMPLEMENT_HH_COMMON_PROP

	IMPLEMENT_LONG_PROP(CurveID, m_nCurveID)
	IMPLEMENT_VARCHAR_PROP(CurveName, m_CurveName)
	IMPLEMENT_LONG_PROP(CurveTypeID, m_nCurveTypeID)
	IMPLEMENT_LONG_PROP(RuleTypeID, m_nRuleTypeID)
	IMPLEMENT_CURRENCY_PROP(PositionThreshold, m_cPositionThreshold)
};

#endif //__IRCURVE_H_

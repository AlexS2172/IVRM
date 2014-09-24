// RMScenario.h : Declaration of the CRMScenario

#ifndef __RMSCENARIO_H_
#define __RMSCENARIO_H_

#include "resource.h"       // main symbols
#include "HHStruct.h"
#include "HHCommonProps.h"
#include "MsgPropImpl.h"

/////////////////////////////////////////////////////////////////////////////
// CRMScenario
class ATL_NO_VTABLE CRMScenario : 
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CRMScenario, &CLSID_RMScenario>,
	public IDispatchImpl<IRMScenario, &IID_IRMScenario, &LIBID_MSGSTRUCTLib>,
	public INetPackingConstSizeDataImpl<CRMScenario, __RMScenarioData>
{
public:
	CRMScenario()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_RMSCENARIO)
DECLARE_NOT_AGGREGATABLE(CRMScenario)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CRMScenario)
	COM_INTERFACE_ENTRY(IRMScenario)
	COM_INTERFACE_ENTRY2(IDispatch, IRMScenario)
	COM_INTERFACE_ENTRY(IHHCommon)
	COM_INTERFACE_ENTRY(INetPacking)
END_COM_MAP()

// IRMScenario
public:
	IMPLEMENT_HH_COMMON_PROP

	IMPLEMENT_LONG_PROP(RMScenarioID, m_nRMScenarioID)
	IMPLEMENT_VARCHAR_PROP(ScenarioName, m_ScenarioName)
	IMPLEMENT_VARCHAR_PROP(Description, m_Description)
	IMPLEMENT_LONG_PROP(Points1, m_nPoints1)
	IMPLEMENT_DOUBLE_PROP(Step1, m_dStep1)
	IMPLEMENT_LONG_PROP(IsAbs1, m_nIsAbs1)
	IMPLEMENT_LONG_PROP(Type1, m_nType1)
	IMPLEMENT_LONG_PROP(Points2, m_nPoints2)
	IMPLEMENT_DOUBLE_PROP(Step2, m_dStep2)
	IMPLEMENT_LONG_PROP(IsAbs2, m_nIsAbs2)
	IMPLEMENT_LONG_PROP(Type2, m_nType2)
};

#endif //__RMSCENARIO_H_

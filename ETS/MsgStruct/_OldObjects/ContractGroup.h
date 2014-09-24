// ContractGroup.h : Declaration of the CContractGroup

#ifndef __CONTRACTGROUP_H_
#define __CONTRACTGROUP_H_

#include "resource.h"       // main symbols
#include "HHStruct.h"
#include "HHCommonProps.h"
#include "MsgPropImpl.h"

/////////////////////////////////////////////////////////////////////////////
// CContractGroup
class ATL_NO_VTABLE CContractGroup : 
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CContractGroup, &CLSID_ContractGroup>,
	public IDispatchImpl<IContractGroup, &IID_IContractGroup, &LIBID_MSGSTRUCTLib>,
	public INetPackingConstSizeDataImpl<CContractGroup, __ContractGroupData>
{
public:
	CContractGroup()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_CONTRACTGROUP)
DECLARE_NOT_AGGREGATABLE(CContractGroup)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CContractGroup)
	COM_INTERFACE_ENTRY(IContractGroup)
	COM_INTERFACE_ENTRY2(IDispatch, IContractGroup)
	COM_INTERFACE_ENTRY(IHHCommon)
	COM_INTERFACE_ENTRY(INetPacking)
END_COM_MAP()

// IContractGroup
public:
	IMPLEMENT_HH_COMMON_PROP

	IMPLEMENT_LONG_PROP(GroupID, m_nGroupID)
	IMPLEMENT_VARCHAR_PROP(GroupName, m_GroupName)
	IMPLEMENT_VARCHAR_PROP(Description, m_Description)
};

#endif //__CONTRACTGROUP_H_

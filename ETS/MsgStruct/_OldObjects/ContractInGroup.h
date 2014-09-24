// ContractInGroup.h : Declaration of the CContractInGroup

#ifndef __CONTRACTINGROUP_H_
#define __CONTRACTINGROUP_H_

#include "resource.h"       // main symbols
#include "HHStruct.h"
#include "HHCommonProps.h"
#include "MsgPropImpl.h"


/////////////////////////////////////////////////////////////////////////////
// CContractInGroup
class ATL_NO_VTABLE CContractInGroup : 
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CContractInGroup, &CLSID_ContractInGroup>,
	public IDispatchImpl<IContractInGroup, &IID_IContractInGroup, &LIBID_MSGSTRUCTLib>,
	public INetPackingConstSizeDataImpl<CContractInGroup, __ContractInGroupData>
{
public:
	CContractInGroup()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_CONTRACTINGROUP)
DECLARE_NOT_AGGREGATABLE(CContractInGroup)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CContractInGroup)
	COM_INTERFACE_ENTRY(IContractInGroup)
	COM_INTERFACE_ENTRY2(IDispatch, IContractInGroup)
	COM_INTERFACE_ENTRY(IHHCommon)
	COM_INTERFACE_ENTRY(INetPacking)
END_COM_MAP()

// IContractInGroup
public:
	IMPLEMENT_HH_COMMON_PROP

	IMPLEMENT_LONG_PROP(GroupID, m_nGroupID)
	IMPLEMENT_LONG_PROP(ContractID, m_nContractID)
};

#endif //__CONTRACTINGROUP_H_

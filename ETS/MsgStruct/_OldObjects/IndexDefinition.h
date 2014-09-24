// IndexDefinition.h : Declaration of the CIndexDefinition

#ifndef __INDEXDEFINITION_H_
#define __INDEXDEFINITION_H_

#include "resource.h"       // main symbols
#include "HHStruct.h"
#include "HHCommonProps.h"
#include "MsgPropImpl.h"


/////////////////////////////////////////////////////////////////////////////
// CIndexDefinition
class ATL_NO_VTABLE CIndexDefinition : 
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CIndexDefinition, &CLSID_IndexDef>,
	public IDispatchImpl<IIndexDef, &IID_IIndexDef, &LIBID_MSGSTRUCTLib>,
	public INetPackingConstSizeDataImpl<CIndexDefinition, __IndexDefinitionData>
{
public:
	CIndexDefinition()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_INDEXDEFINITION)
DECLARE_NOT_AGGREGATABLE(CIndexDefinition)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CIndexDefinition)
	COM_INTERFACE_ENTRY(IIndexDef)
	COM_INTERFACE_ENTRY2(IDispatch, IIndexDef)
	COM_INTERFACE_ENTRY(IHHCommon)
	COM_INTERFACE_ENTRY(INetPacking)
END_COM_MAP()

// IIndexDefinition
public:
	IMPLEMENT_HH_COMMON_PROP

	IMPLEMENT_LONG_PROP(IndexID, m_nID)
	IMPLEMENT_LONG_PROP(StockID, m_nStockID)
	IMPLEMENT_DOUBLE_PROP(Weight, m_dWeight)
	IMPLEMENT_DOUBLE_PROP(Beta, m_dBeta)
};

#endif //__INDEXDEFINITION_H_

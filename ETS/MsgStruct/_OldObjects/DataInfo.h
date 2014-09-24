// DataInfo.h : Declaration of the CDataInfo

#ifndef __DATAINFO_H_
#define __DATAINFO_H_

#include "resource.h"       // main symbols
#include "HHStruct.h"
#include "HHCommonProps.h"
#include "MsgPropImpl.h"


/////////////////////////////////////////////////////////////////////////////
// CDataInfo
class ATL_NO_VTABLE CDataInfo : 
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CDataInfo, &CLSID_DataInfo>,
	public IDispatchImpl<IDataInfo, &IID_IDataInfo, &LIBID_MSGSTRUCTLib>,
	public INetPackingConstSizeDataImpl<CDataInfo, __DataInfoData>
{
public:
	CDataInfo()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_DATAINFO)
DECLARE_NOT_AGGREGATABLE(CDataInfo)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CDataInfo)
	COM_INTERFACE_ENTRY(IDataInfo)
	COM_INTERFACE_ENTRY2(IDispatch, IDataInfo)
	COM_INTERFACE_ENTRY(IHHCommon)
	COM_INTERFACE_ENTRY(INetPacking)
END_COM_MAP()

// IDataInfo
public:
	IMPLEMENT_HH_COMMON_PROP

	IMPLEMENT_LONG_PROP(DataInfoID, m_nDataInfoID)
	IMPLEMENT_VARCHAR_PROP(KeyName, m_KeyName)
	IMPLEMENT_VARCHAR_PROP(KeyValue, m_KeyValue)
};

#endif //__DATAINFO_H_

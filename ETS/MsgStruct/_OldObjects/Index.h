// Index.h : Declaration of the CIndex

#ifndef __INDEX_H_
#define __INDEX_H_

#include "resource.h"       // main symbols
#include "HHStruct.h"
#include "HHCommonProps.h"
#include "MsgPropImpl.h"


/////////////////////////////////////////////////////////////////////////////
// CIndex
class ATL_NO_VTABLE CIndex : 
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CIndex, &CLSID_Index>,
	public IDispatchImpl<IIndex, &IID_IIndex, &LIBID_MSGSTRUCTLib>,
	public INetPackingConstSizeDataImpl<CIndex, __IndexData>
{
public:
	CIndex()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_INDEX)
DECLARE_NOT_AGGREGATABLE(CIndex)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CIndex)
	COM_INTERFACE_ENTRY(IIndex)
	COM_INTERFACE_ENTRY2(IDispatch, IIndex)
	COM_INTERFACE_ENTRY(IHHCommon)
	COM_INTERFACE_ENTRY(INetPacking)
END_COM_MAP()

// IIndex
public:
	IMPLEMENT_HH_COMMON_PROP

	IMPLEMENT_LONG_PROP(ContractID, m_nID)
	IMPLEMENT_DOUBLE_PROP(Yield, m_dYield)
	IMPLEMENT_LONG_PROP(IsHTB, m_IsHTB)
	IMPLEMENT_LONG_PROP(CalcModelType, m_nCalcModelType)
	IMPLEMENT_LONG_PROP(CalcOptionType, m_nCalcOptionType)
	IMPLEMENT_LONG_PROP(IsBasket, m_nIsBasket)
	IMPLEMENT_LONG_PROP(IsActive, m_nIsActive)
	IMPLEMENT_DOUBLE_PROP(Skew, m_dSkew)
	IMPLEMENT_DOUBLE_PROP(Kurt, m_dKurt)
};

#endif //__INDEX_H_

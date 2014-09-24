// PriceCloseUpdate.h : Declaration of the CPriceCloseUpdate


#ifndef __PRICECLOSEUPDATE_H_
#define __PRICECLOSEUPDATE_H_

#include "resource.h"       // main symbols
#include "MsgPacking.h"
#include "MsgPropImpl.h"

struct __PriceCloseUpdateData
{
	BEGIN_PACKING
		FIELD_ENTRY(m_nContractType)
		FIELD_ENTRY(m_nUndID)
		FIELD_ENTRY(m_nContractID)
		FIELD_ENTRY(m_dPriceClose)	
	END_PACKING

	LongF			m_nContractType;
	LongF			m_nUndID;
	LongF			m_nContractID;
	DoubleF			m_dPriceClose;
};

/////////////////////////////////////////////////////////////////////////////
// CPriceCloseUpdate

class ATL_NO_VTABLE CPriceCloseUpdate :
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CPriceCloseUpdate, &CLSID_PriceCloseUpdate>,
	public IDispatchImpl<IPriceCloseUpdate, &IID_IPriceCloseUpdate, &LIBID_MSGSTRUCTLib>,
	public INetPackingConstSizeDataImpl<CPriceCloseUpdate, __PriceCloseUpdateData>
{
public:
	CPriceCloseUpdate()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_PRICECLOSEUPDATE)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CPriceCloseUpdate)
	COM_INTERFACE_ENTRY(IPriceCloseUpdate)
	COM_INTERFACE_ENTRY2(IDispatch, IPriceCloseUpdate)
	COM_INTERFACE_ENTRY(INetPacking)
END_COM_MAP()

// IPriceCloseUpdate
public:
	IMPLEMENT_LONG_PROP(ContractType, m_nContractType)
	IMPLEMENT_LONG_PROP(UndID, m_nUndID)
	IMPLEMENT_LONG_PROP(ContractID, m_nContractID)
	IMPLEMENT_DOUBLE_PROP(PriceClose, m_dPriceClose)
};

#endif //__PriceCloseUpdate_H_
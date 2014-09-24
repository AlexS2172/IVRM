// TraderContracts.h : Declaration of the CTraderContracts

#ifndef __TRADERCONTRACTS_H_
#define __TRADERCONTRACTS_H_

#include "resource.h"       // main symbols
#include "hhstruct.h"
#include "HHCommonProps.h"
#include "MsgPropImpl.h"

/////////////////////////////////////////////////////////////////////////////
// CTraderContracts
class ATL_NO_VTABLE CTraderContract : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CTraderContract, &CLSID_TraderContract>,
	public IDispatchImpl<ITraderContract, &IID_ITraderContract, &LIBID_MSGSTRUCTLib>,
	public INetPackingConstSizeDataImpl<CTraderContract, __TraderContractData>
{
public:
	CTraderContract()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_TRADERCONTRACTS)
DECLARE_NOT_AGGREGATABLE(CTraderContract)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CTraderContract)
	COM_INTERFACE_ENTRY(ITraderContract)
	COM_INTERFACE_ENTRY2(IDispatch, ITraderContract)
	COM_INTERFACE_ENTRY(IHHCommon)
	COM_INTERFACE_ENTRY(INetPacking)
END_COM_MAP()

// ITraderContract
public:
	IMPLEMENT_HH_COMMON_PROP

	IMPLEMENT_LONG_PROP(ContractID, m_nContractID)
};

#endif //__TRADERCONTRACTS_H_

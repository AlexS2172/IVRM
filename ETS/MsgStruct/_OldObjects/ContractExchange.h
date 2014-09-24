// ContractContractExchange.h : Declaration of the CContractContractExchange

#ifndef __CONTRACTContractExchange_H_
#define __CONTRACTContractExchange_H_

#include "resource.h"       // main symbols
#include "HHStruct.h"
#include "HHCommonProps.h"
#include "MsgPropImpl.h"

/////////////////////////////////////////////////////////////////////////////
// CContractExchange
class ATL_NO_VTABLE CContractExchange : 
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CContractExchange, &CLSID_ContractExchange>,
	public IDispatchImpl<IContractExchange, &IID_IContractExchange, &LIBID_MSGSTRUCTLib>,
	public INetPackingConstSizeDataImpl<CContractExchange, __ContractExchangeData>
{
public:
	CContractExchange()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_CONTRACTEXCHANGE)
DECLARE_NOT_AGGREGATABLE(CContractExchange)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CContractExchange)
	COM_INTERFACE_ENTRY(IContractExchange)
	COM_INTERFACE_ENTRY2(IDispatch, IContractExchange)
	COM_INTERFACE_ENTRY(IHHCommon)
	COM_INTERFACE_ENTRY(INetPacking)
END_COM_MAP()

// IContractExchange
public:
	IMPLEMENT_HH_COMMON_PROP

	IMPLEMENT_LONG_PROP(ContractID, m_nContractID)
	IMPLEMENT_LONG_PROP(ExchangeID, m_nExchangeID)
	IMPLEMENT_LONG_PROP(IsUnderlying, m_nIsUnderlying)
	IMPLEMENT_LONG_PROP(IsOption, m_nIsOption)
};

#endif //__CONTRACTContractExchange_H_

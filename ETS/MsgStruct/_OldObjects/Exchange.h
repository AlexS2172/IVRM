// Exchange.h : Declaration of the CExchange

#ifndef __EXCHANGE_H_
#define __EXCHANGE_H_

#include "resource.h"       // main symbols
#include "HHStruct.h"
#include "HHCommonProps.h"
#include "MsgPropImpl.h"

/////////////////////////////////////////////////////////////////////////////
// CExchange
class ATL_NO_VTABLE CExchange : 
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CExchange, &CLSID_Exchange>,
	public IDispatchImpl<IExchange, &IID_IExchange, &LIBID_MSGSTRUCTLib>,
	public INetPackingConstSizeDataImpl<CExchange, __ExchangeData>
{
public:
	CExchange()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_EXCHANGE)
DECLARE_NOT_AGGREGATABLE(CExchange)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CExchange)
	COM_INTERFACE_ENTRY(IExchange)
	COM_INTERFACE_ENTRY2(IDispatch, IExchange)
	COM_INTERFACE_ENTRY(IHHCommon)
	COM_INTERFACE_ENTRY(INetPacking)
END_COM_MAP()

// IExchange
public:
	IMPLEMENT_HH_COMMON_PROP

	IMPLEMENT_LONG_PROP(ExchangeID, m_nID)
	IMPLEMENT_VARCHAR_PROP(ExchangeName, m_Name)
	IMPLEMENT_VARCHAR_PROP(Description, m_Description)
	IMPLEMENT_VARCHAR_PROP(Code, m_Code)
	IMPLEMENT_LONG_PROP(IsUnderlying, m_nIsUnderlying)
	IMPLEMENT_LONG_PROP(IsOption, m_nIsOption)
};

#endif //__EXCHANGE_H_

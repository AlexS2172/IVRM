// QuoteSubscription.h : Declaration of the CQuoteSubscription

#ifndef __QUOTESUBSCRIPTION_H_
#define __QUOTESUBSCRIPTION_H_

#include "resource.h"       // main symbols
#include "HHStruct.h"
#include "HHCommonProps.h"
#include "MsgPropImpl.h"

/////////////////////////////////////////////////////////////////////////////
// CQuoteSubscription
class ATL_NO_VTABLE CQuoteSubscription : 
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CQuoteSubscription, &CLSID_QuoteSubscription>,
	public IDispatchImpl<IQuoteSubscription, &IID_IQuoteSubscription, &LIBID_MSGSTRUCTLib>,
	public INetPackingConstSizeDataImpl<CQuoteSubscription, __QuoteSubscriptionData>
{
public:
	CQuoteSubscription()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_QUOTESUBSCRIPTION)
DECLARE_NOT_AGGREGATABLE(CQuoteSubscription)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CQuoteSubscription)
	COM_INTERFACE_ENTRY(IQuoteSubscription)
	COM_INTERFACE_ENTRY2(IDispatch, IQuoteSubscription)
	COM_INTERFACE_ENTRY(IHHCommon)
	COM_INTERFACE_ENTRY(INetPacking)
END_COM_MAP()

// IQuoteSubscription
public:
	IMPLEMENT_HH_COMMON_PROP
};

#endif //__QUOTESUBSCRIPTION_H_

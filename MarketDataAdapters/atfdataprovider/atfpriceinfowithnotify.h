// ATFPriceInfoWithNotify.h : Declaration of the CATFPriceInfoWithNotify

#pragma once
#include "resource.h"       // main symbols

#include "ATFBaseFeed.h"
#include "ATFPriceInfoBase.h"
#include "ATFDataProvider.h"

// CATFPriceInfoWithNotify

class ATL_NO_VTABLE CATFPriceInfoWithNotify : 
	public CATFPriceInfoBaseFeed<CATFPriceInfoWithNotify>,
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CATFPriceInfoWithNotify, &CLSID_ATFPriceInfoWithNotify>,
	public ISupportErrorInfoImpl<&IID_IPriceProvider>,
	public IConnectionPointContainerImpl<CATFPriceInfoWithNotify>,
	public IDispatchImpl<IPriceProvider, &IID_IPriceProvider, &LIBID_ATFDATAPROVIDERLib>,
	public IDispatchImpl<IGroupPriceWithNotify, &IID_IGroupPriceWithNotify, &LIBID_ATFDATAPROVIDERLib>,
	public IConnectionPointImpl<CATFPriceInfoWithNotify, &DIID__IPriceInfoWithNotifyEvents, CComDynamicUnkArray>
{
public:
	CATFPriceInfoWithNotify()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_ATFPRICEINFOWITHNOTIFY)


BEGIN_COM_MAP(CATFPriceInfoWithNotify)
	COM_INTERFACE_ENTRY(IPriceProvider)
	COM_INTERFACE_ENTRY(IGroupPriceWithNotify)
	COM_INTERFACE_ENTRY2(IDispatch, IGroupPriceWithNotify)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY(IConnectionPointContainer)
END_COM_MAP()

BEGIN_CONNECTION_POINT_MAP(CATFPriceInfoWithNotify)
	CONNECTION_POINT_ENTRY(DIID__IPriceInfoWithNotifyEvents)
END_CONNECTION_POINT_MAP()

	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}
	
	void FinalRelease() 
	{
	}
	HRESULT OnSubscribed(const CComVariant &varParams);

	// IPriceProvider
public:
	STDMETHOD(RequestLastQuote)(QuoteUpdateParams *Params);
	STDMETHOD(CancelLastQuote)(VARIANT Params);
	STDMETHOD(SubscribeQuote)(QuoteUpdateParams *Params);
	STDMETHOD(UnSubscribeQuote)(VARIANT Params);
	STDMETHOD(Connect)();
	STDMETHOD(Disconnect)();

	// IGroupPriceWithNotify
public:
	STDMETHOD(RequestLastGroupQuotes)(QuoteUpdateParams *Params);
	STDMETHOD(CancelLastGroupQuotes)(VARIANT Params);
	STDMETHOD(SubscribeGroupQuotes)(QuoteUpdateParams *Params);
	STDMETHOD(UnSubscribeGroupQuotes)(VARIANT Params);

};

OBJECT_ENTRY_AUTO(__uuidof(ATFPriceInfoWithNotify), CATFPriceInfoWithNotify)

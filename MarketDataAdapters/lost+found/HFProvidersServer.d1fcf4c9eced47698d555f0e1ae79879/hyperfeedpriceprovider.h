// HyperFeedPriceProvider.h : Declaration of the CHyperFeedPriceProvider

#ifndef __HYPERFEEDPRICEPROVIDER_H_
#define __HYPERFEEDPRICEPROVIDER_H_

#include "resource.h"       // main symbols
#include "HyperFeedBase.h"
#include "GroupPrice.h"

/////////////////////////////////////////////////////////////////////////////
// CHyperFeedPriceProvider
class ATL_NO_VTABLE CHyperFeedPriceProvider : 
	public CHyperFeedBase<CHyperFeedPriceProvider>,
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CHyperFeedPriceProvider, &CLSID_HyperFeedPriceProvider>,
	public ISupportErrorInfoImpl<&IID_IPriceProvider>,
	public IConnectionPointContainerImpl<CHyperFeedPriceProvider>,
	public IDispatchImpl<IPriceProvider, &IID_IPriceProvider, &LIBID_HYPERFEEDPROVIDERSLib>,
	public IConnectionPointImpl<CHyperFeedPriceProvider, &DIID__IPriceProviderEvents, CComDynamicUnkArray>,
	public IDispatchImpl<IGroupPrice, &IID_IGroupPrice, &LIBID_HYPERFEEDPROVIDERSLib>,
	public CGroupPrice
{ 
public:
	CHyperFeedPriceProvider()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_HYPERFEEDPRICEPROVIDER)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CHyperFeedPriceProvider)
	COM_INTERFACE_ENTRY(IPriceProvider)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY(IConnectionPointContainer)
	COM_INTERFACE_ENTRY_IMPL(IConnectionPointContainer)
	COM_INTERFACE_ENTRY2(IDispatch, IPriceProvider)
	COM_INTERFACE_ENTRY(IGroupPrice)
END_COM_MAP()

BEGIN_CONNECTION_POINT_MAP(CHyperFeedPriceProvider)
	CONNECTION_POINT_ENTRY(DIID__IPriceProviderEvents)
END_CONNECTION_POINT_MAP()

// IPriceProvider
	STDMETHOD(RequestLastQuote)(QuoteUpdateParams *Params);
	STDMETHOD(CancelLastQuote)(VARIANT Params);
	STDMETHOD(SubscribeQuote)(QuoteUpdateParams *Params);
	STDMETHOD(UnSubscribeQuote)(VARIANT Params);
	STDMETHOD(Connect)();
	STDMETHOD(Disconnect)();

	// IGroupPrice
	STDMETHOD(RequestGroup)(QuoteUpdateParams *Params,  GroupRequestType enOperationType);
	STDMETHOD(CancelGroup)(VARIANT Params);

	long Attach();
	long Detach();

	void OnResponse(CResponseBasePtr pResponse);
	void OnRequest(CRequestBasePtr pResponse);

private:
	HRESULT OnLastQuote(const CComVariant &varParams, const CComVariant &varResults);
	HRESULT OnQuoteUpdate(const CComVariant &varParams, const CComVariant &varResults);

	virtual void PublicResponse(CResponseBasePtr pResponse){ AddResponse(pResponse);}
	virtual void PublicLastQuote(const _QuoteUpdateParams& Params, const DBA_RECORD_3& rec);
	virtual void PublicQuoteUpdate(const _QuoteUpdateParams& Params, const DBA_RECORD_3& rec);
};

#endif //__HYPERFEEDPRICEPROVIDER_H_

// HyperFeedPriceInfoWithNotify.h : Declaration of the CHyperFeedPriceInfoWithNotify

#pragma once

#ifndef __HYPERFEEDPRICEINFOWITHNOTIFY_H_
#define __HYPERFEEDPRICEINFOWITHNOTIFY_H_

#include "resource.h"       // main symbols
#include "HyperFeedBase.h"
#include "GroupPrice.h"

// CHyperFeedPriceInfoWithNotify
class ATL_NO_VTABLE CHyperFeedPriceInfoWithNotify : 
	public CHyperFeedBase<CHyperFeedPriceInfoWithNotify>,
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CHyperFeedPriceInfoWithNotify, &CLSID_HyperFeedPriceInfoWithNotify>,
	public ISupportErrorInfoImpl<&IID_IPriceProvider>,
	public IConnectionPointContainerImpl<CHyperFeedPriceInfoWithNotify>,
	public IDispatchImpl<IPriceProvider, &IID_IPriceProvider, &LIBID_HYPERFEEDPROVIDERSLib>,
	public IDispatchImpl<IGroupPriceWithNotify, &IID_IGroupPriceWithNotify, &LIBID_HYPERFEEDPROVIDERSLib>,
	public IConnectionPointImpl<CHyperFeedPriceInfoWithNotify, &DIID__IPriceInfoWithNotifyEvents, CComDynamicUnkArray>,
	public CGroupPrice
{
public:
	CHyperFeedPriceInfoWithNotify()
	{
	}

	DECLARE_REGISTRY_RESOURCEID(IDR_HYPERFEEDPRICEINFOWITHNOTIFY)


	BEGIN_COM_MAP(CHyperFeedPriceInfoWithNotify)
		COM_INTERFACE_ENTRY(IPriceProvider)
		COM_INTERFACE_ENTRY(IGroupPriceWithNotify)
		COM_INTERFACE_ENTRY2(IDispatch, IGroupPriceWithNotify)
		COM_INTERFACE_ENTRY(ISupportErrorInfo)
		COM_INTERFACE_ENTRY(IConnectionPointContainer)
	END_COM_MAP()

	BEGIN_CONNECTION_POINT_MAP(CHyperFeedPriceInfoWithNotify)
		CONNECTION_POINT_ENTRY(DIID__IPriceInfoWithNotifyEvents)
	END_CONNECTION_POINT_MAP()

	DECLARE_PROTECT_FINAL_CONSTRUCT()

	// IPriceProvider
	STDMETHOD(RequestLastQuote)(QuoteUpdateParams *Params);
	STDMETHOD(CancelLastQuote)(VARIANT Params);
	STDMETHOD(SubscribeQuote)(QuoteUpdateParams *Params);
	STDMETHOD(UnSubscribeQuote)(VARIANT Params);
	STDMETHOD(Connect)();
	STDMETHOD(Disconnect)();

	// IGroupPriceWithNotify
	STDMETHOD(RequestLastGroupQuotes)(QuoteUpdateParams *Params);
	STDMETHOD(CancelLastGroupQuotes)(VARIANT Params);
	STDMETHOD(SubscribeGroupQuotes)(QuoteUpdateParams *Params);
	STDMETHOD(UnSubscribeGroupQuotes)(VARIANT Params);

	long Attach();
	long Detach();

	void OnResponse(CResponseBasePtr pResponse);
	void OnRequest(CRequestBasePtr pResponse);

private:
	HRESULT OnLastQuote(const CComVariant &varParams, const CComVariant &varResults);
	HRESULT OnQuoteUpdate(const CComVariant &varParams, const CComVariant &varResults);
	HRESULT OnSubscribed(const CComVariant &varParams);
	//HRESULT OnUnsubscribed();

	virtual void PublicResponse(CResponseBasePtr pResponse){ AddResponse(pResponse);}
	virtual void PublicLastQuote(const _QuoteUpdateParams* pParams, const CDBARecordPtr& pData);
	virtual void PublicQuoteUpdate(const _QuoteUpdateParams* pParams, const CSubscriptionInfoPtr& pRec);


};

#endif //__HYPERFEEDPRICEINFOWITHNOTIFY_H_

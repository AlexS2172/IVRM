// ISEPriceInfo.h : Declaration of the CISEPriceInfo

#ifndef __ISEPRICEINFO_H_
#define __ISEPRICEINFO_H_

#include "resource.h"       // main symbols
#include "ISEEvents.h"

/////////////////////////////////////////////////////////////////////////////
// CISEPriceInfo
class ATL_NO_VTABLE CISEPriceInfo : 
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CISEPriceInfo, &CLSID_ISEPriceInfo>,
	public ISupportErrorInfo,
	public IConnectionPointContainerImpl<CISEPriceInfo>,
	public IDispatchImpl<IPriceProvider, &IID_IPriceProvider, &LIBID_ISEPROVIDERLib>,
	public IDispatchImpl<IGroupPrice, &IID_IGroupPrice, &LIBID_ISEPROVIDERLib>,
	public IConnectionPointImpl<CISEPriceInfo, &DIID__IPriceInfoEvents, CComDynamicUnkArray>
{
public:
	CISEPriceInfo()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_ISEPRICEINFO)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CISEPriceInfo)
	COM_INTERFACE_ENTRY(IPriceProvider)
	COM_INTERFACE_ENTRY(IGroupPrice)
	COM_INTERFACE_ENTRY2(IDispatch, IGroupPrice)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY(IConnectionPointContainer)
END_COM_MAP()

BEGIN_CONNECTION_POINT_MAP(CISEPriceInfo)
	CONNECTION_POINT_ENTRY(DIID__IPriceInfoEvents)
END_CONNECTION_POINT_MAP()


//  CP
	HRESULT OnLastQuote(const CComVariant &varParams, const CComVariant &varResults);
	HRESULT OnQuoteUpdate(const CComVariant &varParams, const CComVariant &varResults);
	HRESULT OnError(ErrorNumberEnum enumError, _bstr_t bstrDescription, 
				RequestsTypeEnum enumRequest, const CComVariant &varRequest);


// ISupportsErrorInfo
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

// IPriceProvider
public:
	STDMETHOD(RequestLastQuote)(QuoteUpdateParams* Params);
	STDMETHOD(CancelLastQuote)(VARIANT Params);
	STDMETHOD(SubscribeQuote)(QuoteUpdateParams* Params);
	STDMETHOD(UnSubscribeQuote)(VARIANT Params);
	STDMETHOD(Connect)();
	STDMETHOD(Disconnect)();

// IGroupPrice
public:
	STDMETHOD(RequestGroup)(QuoteUpdateParams* Params, VARIANT_BOOL bSubscribe);
	STDMETHOD(CancelGroup)(VARIANT Params);
};

#endif //__ISEPRICEINFO_H_

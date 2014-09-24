// ISEStdPriceProvider.h : Declaration of the CISEStdPriceProvider

#ifndef __ISESTDPRICEPROVIDER_H_
#define __ISESTDPRICEPROVIDER_H_

#include "resource.h"       // main symbols
#include "ISEEvents.h"
#include "ObjectID.h"

/////////////////////////////////////////////////////////////////////////////
// CISEStdPriceProvider
class ATL_NO_VTABLE CISEStdPriceProvider : 
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CISEStdPriceProvider, &CLSID_ISEPriceProvider>,
	public ISupportErrorInfoImpl<&IID_IPriceProvider>,
	public IConnectionPointContainerImpl<CISEStdPriceProvider>,
	public IDispatchImpl<IPriceProvider, &IID_IPriceProvider, &LIBID_ISEPROVIDERLib>,
	public IDispatchImpl<IGroupPrice, &IID_IGroupPrice, &LIBID_ISEPROVIDERLib>,
	public IConnectionPointImpl<CISEStdPriceProvider, &DIID__IPriceProviderEvents, CComDynamicUnkArray>,
	public CISEPriceBase
{
public:
	CISEStdPriceProvider()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_ISESTDPRICEPROVIDER)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CISEStdPriceProvider)
	COM_INTERFACE_ENTRY(IPriceProvider)
	COM_INTERFACE_ENTRY(IGroupPrice)
	COM_INTERFACE_ENTRY2(IDispatch, IGroupPrice)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY(IConnectionPointContainer)
END_COM_MAP()

BEGIN_CONNECTION_POINT_MAP(CISEStdPriceProvider)
	CONNECTION_POINT_ENTRY(DIID__IPriceProviderEvents)
END_CONNECTION_POINT_MAP()

public:

	void FinalRelease() 
	{
		EgTrace("CISEStdPriceProvider::Release");
		Disconnect();
	}

// CP
	virtual HRESULT OnLastQuote(const CComVariant &varParams, const CComVariant &varResults);
	virtual HRESULT OnQuoteUpdate(const CComVariant &varParams, const CComVariant &varResults);
	virtual HRESULT OnError(ErrorNumberEnum enumError, _bstr_t bstrDescription, 
				RequestsTypeEnum enumRequest, const CComVariant &varRequest);
	virtual HRESULT	OnEvent(const EventNumberEnum EventNumber, _bstr_t Description);

// ISupportsErrorInfo
	//STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

// IPriceProvider
public:
	STDMETHOD(RequestLastQuote)(QuoteUpdateParams* Params);
	STDMETHOD(CancelLastQuote)(VARIANT Params);
	STDMETHOD(SubscribeQuote)(QuoteUpdateParams* Params);
	STDMETHOD(UnSubscribeQuote)(VARIANT Params);

	STDMETHOD(Connect)();
	STDMETHOD(Disconnect)();

//IGroupPrice
	STDMETHOD(RequestGroup)(QuoteUpdateParams* Params,/*[in]*/ GroupRequestType enTypeGroupRequest);
	STDMETHOD(CancelGroup)(VARIANT Params);
};

#endif //__ISESTDPRICEPROVIDER_H_

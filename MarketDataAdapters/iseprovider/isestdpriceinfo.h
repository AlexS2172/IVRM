// ISEStdPriceInfo.h : Declaration of the CISEStdPriceInfo

#ifndef __ISESTDPRICEINFO_H_
#define __ISESTDPRICEINFO_H_

#include "resource.h"       // main symbols
#include "ISEEvents.h"
#include "ObjectID.h"

/////////////////////////////////////////////////////////////////////////////
// CISEStdPriceInfo
class ATL_NO_VTABLE CISEStdPriceInfo : 
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CISEStdPriceInfo, &CLSID_ISEPriceProvider>,
	public ISupportErrorInfoImpl<&IID_IPriceProvider>,
	public IConnectionPointContainerImpl<CISEStdPriceInfo>,
	public IDispatchImpl<IPriceProvider, &IID_IPriceProvider, &LIBID_ISEPROVIDERLib>,
	public IDispatchImpl<IGroupPrice, &IID_IGroupPrice, &LIBID_ISEPROVIDERLib>,
	public IConnectionPointImpl<CISEStdPriceInfo, &DIID__IPriceInfoEvents, CComDynamicUnkArray>,
	public CISEPriceBase
{
public:
	CISEStdPriceInfo()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_ISESTDPRICEINFO)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CISEStdPriceInfo)
	COM_INTERFACE_ENTRY(IPriceProvider)
	COM_INTERFACE_ENTRY(IGroupPrice)
	COM_INTERFACE_ENTRY2(IDispatch, IGroupPrice)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY(IConnectionPointContainer)
END_COM_MAP()

BEGIN_CONNECTION_POINT_MAP(CISEStdPriceInfo)
	CONNECTION_POINT_ENTRY(DIID__IPriceInfoEvents)
END_CONNECTION_POINT_MAP()

public:

	void FinalRelease() 
	{
		EgTrace("CISEStdPriceInfo::Release");
		Disconnect();
	}

// CP
	virtual HRESULT OnLastQuote(const CComVariant &varParams, const CComVariant &varResults);
	virtual HRESULT OnQuoteUpdate(const CComVariant &varParams, const CComVariant &varResults);
	virtual HRESULT OnError(ErrorNumberEnum enumError, _bstr_t bstrDescription, 
				RequestsTypeEnum enumRequest, const CComVariant &varRequest);
	virtual HRESULT	OnEvent(const EventNumberEnum EventNumber, _bstr_t Description);

// ISupportsErrorInfo
//	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

// IPriceProvider
public:
	STDMETHOD(RequestLastQuote)(QuoteUpdateParams* Params);
	STDMETHOD(CancelLastQuote)(VARIANT Params);
	STDMETHOD(SubscribeQuote)(QuoteUpdateParams* Params);
	STDMETHOD(UnSubscribeQuote)(VARIANT Params);

	STDMETHOD(Connect)();
	STDMETHOD(Disconnect)();

//IGroupPrice
	STDMETHOD(RequestGroup)(QuoteUpdateParams* Params,/*[in]*/ GroupRequestType enOperationType);
	STDMETHOD(CancelGroup)(VARIANT Params);
};

#endif //__ISESTDPRICEINFO_H_

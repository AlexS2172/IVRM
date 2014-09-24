// HyperFeedPriceInfo.h : Declaration of the CHyperFeedPriceInfo

#ifndef __HYPERFEEDPRICEINFO_H_
#define __HYPERFEEDPRICEINFO_H_

#include "resource.h"       // main symbols
#include "HyperFeedBase.h"
#include "GroupPrice.h"

/////////////////////////////////////////////////////////////////////////////
// CHyperFeedPriceInfo
class ATL_NO_VTABLE CHyperFeedPriceInfo : 
	public CHyperFeedBase<CHyperFeedPriceInfo>,
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CHyperFeedPriceInfo, &CLSID_HyperFeedPriceInfo>,
	public ISupportErrorInfoImpl<&IID_IPriceProvider>,
	public IConnectionPointContainerImpl<CHyperFeedPriceInfo>,
	public IDispatchImpl<IPriceProvider, &IID_IPriceProvider, &LIBID_HYPERFEEDPROVIDERSLib>,
	public IConnectionPointImpl<CHyperFeedPriceInfo, &DIID__IPriceInfoEvents, CComDynamicUnkArray>,
	public IDispatchImpl<IUserInformation, &IID_IUserInformation, &LIBID_HYPERFEEDPROVIDERSLib>,
	public IDispatchImpl<IGroupPrice, &IID_IGroupPrice, &LIBID_HYPERFEEDPROVIDERSLib>,
	public CGroupPrice
{
public:
	CHyperFeedPriceInfo()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_HYPERFEEDPRICEINFO)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CHyperFeedPriceInfo)
	COM_INTERFACE_ENTRY(IPriceProvider)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY(IConnectionPointContainer)
	COM_INTERFACE_ENTRY_IMPL(IConnectionPointContainer)
	COM_INTERFACE_ENTRY2(IDispatch, IPriceProvider)
	COM_INTERFACE_ENTRY(IGroupPrice)
	COM_INTERFACE_ENTRY(IUserInformation)
	COM_INTERFACE_ENTRY2(IDispatch, IUserInformation)

END_COM_MAP()

BEGIN_CONNECTION_POINT_MAP(CHyperFeedPriceInfo)
	CONNECTION_POINT_ENTRY(DIID__IPriceInfoEvents)
END_CONNECTION_POINT_MAP()

// IPriceProvider
STDMETHOD(RequestLastQuote)(QuoteUpdateParams *Params);
STDMETHOD(CancelLastQuote)(VARIANT Params);
STDMETHOD(SubscribeQuote)(QuoteUpdateParams *Params);
STDMETHOD(UnSubscribeQuote)(VARIANT Params);
STDMETHOD(Connect)();
STDMETHOD(Disconnect)();
STDMETHOD(put_UserName)(BSTR userName);

// IGroupPrice	  v
STDMETHOD(RequestGroup)(QuoteUpdateParams *Params,  GroupRequestType enOperationType);
STDMETHOD(CancelGroup)(VARIANT Params);

long Attach();
long Detach();

void OnResponse(CResponseBasePtr pResponse);
void OnRequest(CRequestBasePtr pResponse);

private:
	HRESULT OnLastQuote(const CComVariant &varParams, const CComVariant &varResults);
	HRESULT OnQuoteUpdate(const CComVariant &varParams, const CComVariant &varResults);

	//CNamedLock m_hCustomerSyncro;

	virtual void PublicResponse(CResponseBasePtr pResponse){ AddResponse(pResponse);}
	virtual void PublicLastQuote(const _QuoteUpdateParams* pParams, const CDBARecordPtr& pData);
	virtual void PublicQuoteUpdate(const _QuoteUpdateParams* pParams, const CSubscriptionInfoPtr& pRec);
};

#endif //__HYPERFEEDPRICEINFO_H_

// HyperFeedStructureProvider.h : Declaration of the CHyperFeedStructureProvider

#ifndef __HYPERFEEDSTRUCTUREPROVIDER_H_
#define __HYPERFEEDSTRUCTUREPROVIDER_H_

#include "resource.h"       // main symbols
#include "HyperFeedBase.h"
#include "MasterOptions.h"
#include "Options.h"
#include "Underlyings.h"
#include "Futures.h"
#include "FuturesOptions.h"
#include "Price.h"

/////////////////////////////////////////////////////////////////////////////
// CHyperFeedStructureProvider
class ATL_NO_VTABLE CHyperFeedStructureProvider : 
	public CHyperFeedBase<CHyperFeedStructureProvider>,
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CHyperFeedStructureProvider, &CLSID_HyperFeedStructureProvider>,
	public ISupportErrorInfo,
	public IConnectionPointContainerImpl<CHyperFeedStructureProvider>,
	public IDispatchImpl<IStructureProvider, &IID_IStructureProvider, &LIBID_HYPERFEEDPROVIDERSLib>,
	public IDispatchImpl<IStructureProvider2, &IID_IStructureProvider2, &LIBID_HYPERFEEDPROVIDERSLib>,
	public IConnectionPointImpl<CHyperFeedStructureProvider, &DIID__IStructureProviderEvents, CComDynamicUnkArray>,
	public CFutures,
	public CFuturesOptions,
	public CMasterOptions,
	public COptions,
	public CUnderlyings,
	public CPrice
{
public:
	CHyperFeedStructureProvider()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_HYPERFEEDSTRUCTUREPROVIDER)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CHyperFeedStructureProvider)
	COM_INTERFACE_ENTRY(IStructureProvider2)
	COM_INTERFACE_ENTRY2(IStructureProvider, IStructureProvider2)
	COM_INTERFACE_ENTRY2(IDispatch, IStructureProvider2)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY(IConnectionPointContainer)
	COM_INTERFACE_ENTRY_IMPL(IConnectionPointContainer)
END_COM_MAP()

BEGIN_CONNECTION_POINT_MAP(CHyperFeedStructureProvider)
	CONNECTION_POINT_ENTRY(DIID__IStructureProviderEvents)
END_CONNECTION_POINT_MAP()

// IStructureProvider2
public:
	STDMETHOD(RequestStock)(StockParams *Params);
	STDMETHOD(CancelStock)(VARIANT Params);
	STDMETHOD(RequestOptions)(OptionParams *Params);
	STDMETHOD(CancelOptions)(VARIANT Params);
	STDMETHOD(Connect)();
	STDMETHOD(Disconnect)();
	STDMETHOD(RequestFuture)(FutureParams *Params);
	STDMETHOD(CancelFuture)(VARIANT Params);
	STDMETHOD(RequestFutures)(FutureRootParams *Params);
	STDMETHOD(CancelFutures)(VARIANT Params);
	STDMETHOD(RequestFuturesOptions)(FuturesOptionParams *Params);
	STDMETHOD(CancelFuturesOptions)(VARIANT Params);	
// ISupportsErrorInfo
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

	long Attach();
	long Detach();

	void OnResponse(CResponseBasePtr spResponse);
	void OnRequest(CRequestBasePtr spResponse);

public:

	void OnRemoveAll()
	{
		CPrice::OnRemoveAll();
		COptions::OnRemoveAll();
	}

private:
	HRESULT OnStock(const CComVariant &varParams, const CComVariant &varResults);
	HRESULT OnOption(const CComVariant &varParams, const CComVariant &varResults, const CComVariant &varIsLastOption);
	HRESULT OnFuture(const CComVariant &varParams, const CComVariant &varResults);
	HRESULT OnFutureByRoot(const CComVariant &varParams, const CComVariant &varResults, const CComVariant &varIsLastFutures);
	HRESULT OnFuturesOption(const CComVariant &varParams, const CComVariant &varResults, const CComVariant &varIsLastOption);

	virtual void PublicResponse(CResponseBasePtr pResponse){ AddResponse(pResponse);}

	virtual void PublicGetOptionError(ErrorNumberEnum nError, BSTR bsDescription);
	virtual void PublicLastOption();
	virtual void PublicOption(DBA_OPTIONS_FILTER_RECORD* pData);

	virtual void PublicGetFutureOptionError(ErrorNumberEnum nError, BSTR bsDescription);
	virtual void PublicLastFutureOption();
	virtual void PublicFutureOption(_FuturesOptionResults& results);

	virtual void PublicLastQuote(const _QuoteUpdateParams* pParams,  const CDBARecordPtr& pData);
	virtual void PublicQuoteUpdate(const _QuoteUpdateParams* pParams, const CSubscriptionInfoPtr& pRec){};

private:
	CComVariant m_vtCurrentRequest;
};


#endif //__HYPERFEEDSTRUCTUREPROVIDER_H_

// HyperFeedStructureProviderEx.h : Declaration of the CHyperFeedStructureProviderEx
#ifndef __HYPERFEEDSTRUCTUREPROVIDEREX_H_
#define __HYPERFEEDSTRUCTUREPROVIDEREX_H_

#include "resource.h"       // main symbols
#include "HyperFeedBase.h"
#include "MasterOptions.h"
#include "Options.h"
#include "Underlyings.h"
#include "Futures.h"
#include "FuturesOptions.h"
#include "Price.h"

/////////////////////////////////////////////////////////////////////////////
// CHyperFeedStructureProviderEx
class ATL_NO_VTABLE CHyperFeedStructureProviderEx : 
	public CHyperFeedBase<CHyperFeedStructureProviderEx>,
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CHyperFeedStructureProviderEx, &CLSID_HyperFeedStructureProviderEx>,
	public ISupportErrorInfo,
	public IConnectionPointContainerImpl<CHyperFeedStructureProviderEx>,
	public IDispatchImpl<IStructureProvider, &IID_IStructureProvider, &LIBID_HYPERFEEDPROVIDERSLib>,
	public IDispatchImpl<IStructureProvider2, &IID_IStructureProvider2, &LIBID_HYPERFEEDPROVIDERSLib>,
	public IConnectionPointImpl<CHyperFeedStructureProviderEx, &DIID__IStructureProviderExEvents, CComDynamicUnkArray>,
	public CFutures,
	public CFuturesOptions,
	public CMasterOptions,
	public COptions,
	public CUnderlyings,
	public CPrice
{
public:
	CHyperFeedStructureProviderEx()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_HYPERFEEDSTRUCTUREPROVIDEREX)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CHyperFeedStructureProviderEx)
	COM_INTERFACE_ENTRY(IStructureProvider2)
	COM_INTERFACE_ENTRY2(IStructureProvider, IStructureProvider2)
	COM_INTERFACE_ENTRY2(IDispatch, IStructureProvider2)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY(IConnectionPointContainer)
	COM_INTERFACE_ENTRY_IMPL(IConnectionPointContainer)
END_COM_MAP()

BEGIN_CONNECTION_POINT_MAP(CHyperFeedStructureProviderEx)
	CONNECTION_POINT_ENTRY(DIID__IStructureProviderExEvents)
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
	//virtual void PublicFutureOption(_FuturesOptionResults& results); 
	virtual void PublicFutureOption(_FuturesOptionResultsEx& results);
	virtual void PublicFuture(const _FutureRootParams& params, _FutureResultsEx& results);
	virtual void PublicFuture(const _FutureParams& params, _FutureResultsEx& results);
	double GetClosePrice(BSTR bsSymbol, InstrumentTypeEnum enType);
	virtual void PublicStock(const _StockParams& params, _StockResultsEx& results);

	virtual void PublicLastQuote(const _QuoteUpdateParams* pParams, const CDBARecordPtr& pData){};
	virtual void PublicQuoteUpdate(const _QuoteUpdateParams* pParams, const CSubscriptionInfoPtr& pRec){};

private:
	CComVariant m_vtCurrentRequest;
};


#endif //__HYPERFEEDSTRUCTUREPROVIDEREX_H_

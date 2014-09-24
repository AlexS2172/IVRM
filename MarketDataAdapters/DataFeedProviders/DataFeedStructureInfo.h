// DataFeedStructureInfo.h : Declaration of the CDataFeedStructureInfo

#pragma once
#include "resource.h"       // main symbols


#include "DataFeedProviders.h"
#include "PullThread.h"
#include "EgarTrace.h"
#include "DataFeedSettings.h"
#include "ParseHelper.h"
#include "_IStructureProviderExEvents_CP.H"

// CDataFeedStructureInfo
class ATL_NO_VTABLE CDataFeedStructureInfo :
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CDataFeedStructureInfo, &CLSID_DataFeedStructureInfo>,
	public ISupportErrorInfoImpl<&__uuidof(IStructureProvider)>,
	public IConnectionPointContainerImpl<CDataFeedStructureInfo>,
	public IDispatchImpl<IStructureProvider, &__uuidof(IStructureProvider), &LIBID_DataFeedProvidersLib>,
	public IDispatchImpl<IStructureProvider2, &__uuidof(IStructureProvider2), &LIBID_DataFeedProvidersLib>,
	public CProxy_IStructureProviderExEvents<CDataFeedStructureInfo>//,
	//public CDataFeedBaseClass
{
private:
	//CThreadPool<CPullThreadPool>	m_ThreadPool;
	HANDLE							m_hPullThread;

public:
	CDataFeedStructureInfo()
	{
	}

	DECLARE_REGISTRY_RESOURCEID(IDR_DATAFEEDSTRUCTUREINFO)


	BEGIN_COM_MAP(CDataFeedStructureInfo)
		COM_INTERFACE_ENTRY(IStructureProvider2)
		COM_INTERFACE_ENTRY2(IStructureProvider, IStructureProvider2)
		COM_INTERFACE_ENTRY2(IDispatch, IStructureProvider2)
		COM_INTERFACE_ENTRY(ISupportErrorInfo)
		COM_INTERFACE_ENTRY(IConnectionPointContainer)
		COM_INTERFACE_ENTRY_IMPL(IConnectionPointContainer)
	END_COM_MAP()

	BEGIN_CONNECTION_POINT_MAP(CDataFeedStructureInfo)
		CONNECTION_POINT_ENTRY(__uuidof(_IStructureProviderExEvents))
	END_CONNECTION_POINT_MAP()
	// ISupportsErrorInfo
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);


	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}

	void FinalRelease()
	{
	}

	void InsertUpdate(const VS::CResponseRecord& record, QuoteUpdateMap* req_map, long type){};
	void InsertOptionTypedUpdate(const VS::CResponseRecord& record, RequestMap* req_map, long type)
	{
	};
	void InsertWorkerUpdate(const VS::CResponseRecord& record, long type){};
	void Call_OnLastQuote(){};
	void Call_OnContracts(){};
	void Call_OnSubscribed(QuoteUpdateParamsPtr params){};
	void Call_OnSubscribed(QuoteUpdateParams* request) {};
	void Call_OnError(ErrorNumberEnum enumError, _bstr_t bstrDescription, RequestsTypeEnum enumRequest, CComVariant &varRequest)
	{
		Fire_OnError(enumError, bstrDescription, enumRequest,varRequest);
	};
	STDMETHODIMP OnSubscribed(const CComVariant &varParams)
	{
		return S_OK;
	};
	void SubscribeOption(CTickerPtr params){TRACEINFO(__FUNCTION__,"call");};
	void UnSubscribeOption(CTickerPtr params){TRACEINFO(__FUNCTION__,"call");};
	void SubscribeStock(CTickerPtr params){TRACEINFO(__FUNCTION__,"call");};
	void UnSubscribeStock(CTickerPtr params){TRACEINFO(__FUNCTION__,"call");};
	void SubscribeRisks_(CTickerPtr params){TRACEINFO(__FUNCTION__,"call");};
	void UnSubscribeRisks_(CTickerPtr params){TRACEINFO(__FUNCTION__,"call");};

	void Call_OnOption();
	void Call_OnStock();
public:

	STDMETHOD(CancelFuture)(VARIANT Params);
	STDMETHOD(CancelFutures)(VARIANT Params);
	STDMETHOD(CancelFuturesOptions)(VARIANT Params);	
	STDMETHOD(CancelOptions)(VARIANT Params);
	STDMETHOD(CancelStock)(VARIANT Params);
	STDMETHOD(Connect)();
	STDMETHOD(Disconnect)();
	STDMETHOD(RequestFuture)(FutureParams *Params);
	STDMETHOD(RequestFutures)(FutureRootParams *Params);
	STDMETHOD(RequestFuturesOptions)(FuturesOptionParams *Params);
	STDMETHOD(RequestStock)(StockParams *Params);
	STDMETHOD(RequestOptions)(OptionParams *Params);
	STDMETHOD(RequestDividends)(ITicker *Params, DividendsFullInfo* dividends);

	// IStructureProviderExEvents
	/*STDMETHOD(OnError)(ErrorNumberEnum enumError, _bstr_t bstrDescription, RequestsTypeEnum enumRequest, const CComVariant &varRequest);
	STDMETHOD(OnFuturesOption)(const CComVariant &varParams, const CComVariant &varResults, const CComVariant &varIsLastOption);
	STDMETHOD(OnFuture)(const CComVariant &varParams, const CComVariant &varResults);
	STDMETHOD(OnFutureByRoot)(const CComVariant &varParams, const CComVariant &varResults, const CComVariant &varIsLastFutures);
	STDMETHOD(OnStock)(const CComVariant &varParams, const CComVariant &varResults);
	STDMETHOD(OnOption)(const CComVariant &varParams, const CComVariant &varResults, const CComVariant &varIsLastOption);*/
};

OBJECT_ENTRY_AUTO(__uuidof(DataFeedStructureInfo), CDataFeedStructureInfo)

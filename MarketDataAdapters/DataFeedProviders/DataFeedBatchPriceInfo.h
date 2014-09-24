// DataFeedBatchPriceInfo.h : Declaration of the CDataFeedBatchPriceInfo

#pragma once
#include "resource.h"       // main symbols

#include "DataFeedProviders.h"
#include "PullThread.h"
#include "EgarTrace.h"
#include "DataFeedSettings.h"
#include "ParseHelper.h"
#include "_IBatchPriceInfoEvents_CP.H"
#include "_IRisksInfoEvents_CP.H"
#include "_IDataProviderInfoEvents_CP.H"
#include "DataFeedBaseClass.h"




// CDataFeedBatchPriceInfo

class ATL_NO_VTABLE CDataFeedBatchPriceInfo :
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CDataFeedBatchPriceInfo, &CLSID_DataFeedBatchPriceInfo>,
	public ISupportErrorInfoImpl<&__uuidof(IPriceProvider)>,
	public IConnectionPointContainerImpl<CDataFeedBatchPriceInfo>,
	public IDispatchImpl<IPriceProvider,					&__uuidof(IPriceProvider),		&CLSID_DataFeedBatchPriceInfo, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public IDispatchImpl<IBatchPriceProvider,				&__uuidof(IBatchPriceProvider), &CLSID_DataFeedBatchPriceInfo>,
	public IDispatchImpl<IUserInformation,					&__uuidof(IUserInformation),	&CLSID_DataFeedBatchPriceInfo>,
	public IDispatchImpl<IRisksProvider,					&__uuidof(IRisksProvider),		&CLSID_DataFeedBatchPriceInfo>,
	public IDispatchImpl<IDataProvider,						&__uuidof(IDataProvider),		&CLSID_DataFeedBatchPriceInfo>,
	public CProxy_IBatchPriceInfoEvents<CDataFeedBatchPriceInfo>,
	public CProxy_IRisksInfoEvents<CDataFeedBatchPriceInfo>,
	public CProxy_IDataProviderInfoEvents<CDataFeedBatchPriceInfo>,
	public CGatewayNotify
	
{
private:

	HANDLE	m_hStopPull;
	CDataFeedGateway::pointer m_gateway;

public:
	CDataFeedBatchPriceInfo()
	{
		m_pUnkMarshaler = NULL;
	}
	
	HANDLE& GetStopEvent() {return m_hStopPull;};

	DECLARE_REGISTRY_RESOURCEID(IDR_DATAFEEDBATCHPRICEINFO)


	BEGIN_COM_MAP(CDataFeedBatchPriceInfo)
		COM_INTERFACE_ENTRY(IPriceProvider)
		COM_INTERFACE_ENTRY(IBatchPriceProvider)
		COM_INTERFACE_ENTRY2(IDispatch, IBatchPriceProvider)
		COM_INTERFACE_ENTRY(IRisksProvider)
		COM_INTERFACE_ENTRY2(IDispatch, IRisksProvider)
		COM_INTERFACE_ENTRY(IDataProvider)
		COM_INTERFACE_ENTRY2(IDispatch, IDataProvider)
		COM_INTERFACE_ENTRY(IUserInformation)
		COM_INTERFACE_ENTRY2(IDispatch, IUserInformation)
		COM_INTERFACE_ENTRY(ISupportErrorInfo)
		COM_INTERFACE_ENTRY(IConnectionPointContainer)
		COM_INTERFACE_ENTRY_AGGREGATE(IID_IMarshal, m_pUnkMarshaler.p)
	END_COM_MAP()

	BEGIN_CONNECTION_POINT_MAP(CDataFeedBatchPriceInfo)
		CONNECTION_POINT_ENTRY(__uuidof(_IBatchPriceInfoEvents))
		CONNECTION_POINT_ENTRY(__uuidof(_IRisksInfoEvents))
		CONNECTION_POINT_ENTRY(__uuidof(_IDataProviderInfoEvents))
	END_CONNECTION_POINT_MAP()

	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);


	DECLARE_PROTECT_FINAL_CONSTRUCT()
	DECLARE_GET_CONTROLLING_UNKNOWN()

	HRESULT FinalConstruct()
	{
		CDataFeedSettings::Initialize();

		if (EgLib::CEgLibTraceManager::g_spTrace == NULL) {
			EgLib::CEgLibTraceManager::g_spTrace = 
				EgLib::CEgLibTraceManagerPtr(static_cast<EgLib::CEgLibTraceManager*>(new CEgTrace));
				
			if (CDataFeedSettings::log_directory.length()) {
				std::string my_log_directory = CDataFeedSettings::log_directory + "\\" + CDataFeedSettings::GetUserName();
				EgLib::CEgLibTraceManager::g_spTrace->SetPathType(EgLib::CEgLibTraceManager::enAbsolute);
				EgLib::CEgLibTraceManager::g_spTrace->SetFilePath(_bstr_t(my_log_directory.c_str()));									
			};
		};
		
		CFieldCollections::Initialize();

		m_gateway = CDataFeedGateway::pointer(new CDataFeedGateway(this));

		if (m_gateway){
			m_gateway->InitCorba();
			m_gateway->StartPulling();
		};

		EgLib::CEgLibTraceManager::g_spTrace->SetMinLogLevel(CDataFeedSettings::log_level);
		EgLib::CEgLibTraceManager::g_spTrace->SetLogLifeTime(CDataFeedSettings::log_lifetime);

		return CoCreateFreeThreadedMarshaler(
			GetControllingUnknown(), &m_pUnkMarshaler.p);
	}

	void FinalRelease()
	{
		if (m_gateway){

			m_gateway->StopPulling();
			m_gateway->DeinitCorba();

			delete m_gateway;
			m_gateway = NULL;
		};

		m_pUnkMarshaler.Release();
	}

	CComPtr<IUnknown> m_pUnkMarshaler;

	void InsertUpdate(const VS::CResponseRecord& record, QuoteUpdateMap* req_map, long type);
	void InsertWorkerUpdate(const VS::CResponseRecord& record, long type);

	void Call_OnLastQuote();
	void Call_OnSubscribed(CTickerPtr params);
	void Call_OnSubscribed(QuoteUpdateParams* request) 
	{
		if (request)
			Fire_OnSubscribed(request);
	};
	void Call_OnSubscribedRisks(CTickerPtr params);
	void Call_OnContracts()
	{
		Lock();
		Fire_OnContractsUpdate();
		Unlock();
	};

	void Call_OnError(ErrorNumberEnum enumError, _bstr_t bstrDescription, RequestsTypeEnum enumRequest, CComVariant &varRequest)
	{
		//m_OnErrorCalled++;
		Fire_OnError(enumError, bstrDescription, enumRequest, varRequest);
	};

	void SubscribeOption(CTickerPtr params);
	void UnSubscribeOption(CTickerPtr params);

	void SubscribeStock(CTickerPtr params);
	void UnSubscribeStock(CTickerPtr params);

	void SubscribeRisks_(CTickerPtr params);
	void UnSubscribeRisks_(CTickerPtr params);

	void Call_OnOption(){};
	void Call_OnStock(){};
public:

	STDMETHOD(RequestLastQuote)(QuoteUpdateParams *Params);
	STDMETHOD(CancelLastQuote)(VARIANT Params);

	STDMETHOD(SubscribeQuote)(QuoteUpdateParams *Params);
	STDMETHOD(UnSubscribeQuote)(VARIANT Params);

	STDMETHOD(Connect)();
	STDMETHOD(Disconnect)();

	// IBatchPriceProvider
	STDMETHOD(get_IsQuotesUpdated)(VARIANT_BOOL *pVal);

	STDMETHOD(GetQuotesUpdates)(SAFEARRAY** pRetVal);
	STDMETHOD(GetRisksUpdates)(SAFEARRAY** pRetVal);

	STDMETHOD(SubscribeRisks)(ITicker *Params);

	STDMETHOD(RequestMultipleQuotes)  (SAFEARRAY** pParams);
	STDMETHOD(SubscribeMultipleQuotes)(SAFEARRAY** pParams);

	STDMETHOD(UnNotifiedSubscribtionQuantity)(VARIANT_BOOL bSkipNotification,  LONG* pVal);

	STDMETHOD(SetRegionRate)(BSTR bstrRegion, DOUBLE dRate);

	//IUserInformation
	STDMETHOD(put_UserName)(BSTR userName);
    STDMETHOD(get_InProc)(VARIANT_BOOL* bMode);

	STDMETHOD(RequestDividends)(ITicker *Params, DividendsFullInfo* dividends);
	STDMETHOD(RequestRates)(ITicker *Params, IRatesFullInfo* Rates);
	STDMETHOD(RequestContracts)(ITicker *Params, IContractsFullInfo* Contracts);
	STDMETHOD(GetContractsUpdate)(SAFEARRAY** pRetVal);

public:
	//Client notification interface
	void	OnQuoteUpdate();
	void	OnLastQuote();
	void	OnSubscribed(CRequestKeyPtr& request);
	void	OnUnsubscribed(CRequestKeyPtr& request);

	void	OnSubscribedRisks(CRequestKeyPtr& request);
	void	OnUnsubscribedRisks(CRequestKeyPtr& request);
	void	OnRiskUpdate();

	void	OnError(CRequestKeyPtr& request, std::string& error, long code, long requestType);
};

OBJECT_ENTRY_AUTO(__uuidof(DataFeedBatchPriceInfo), CDataFeedBatchPriceInfo)

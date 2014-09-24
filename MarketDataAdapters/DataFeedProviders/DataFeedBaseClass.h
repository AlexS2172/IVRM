#pragma once

#include "stdafx.h"
#include "DataFeedSettings.h"
#include "ParseHelper.h"
#include "Provider.h"

//----------------------------------------------------------------------------------------------//
//-----CDataFeedGateway-----//
//----------------------------------------------------------------------------------------------//
class CDataFeedGateway: public CGateway
{
public:
	
	enum UpdateStatusEnum{
		enQuotesUpdates = 1,
		enRisksUpdates = 2
	};

	typedef CDataFeedGateway*	pointer;

	long	Connect();
	long	Disconnect();

	long	Subscribe(CRequestKeyPtr& spRequest, ResponseTypeEnum type);
	long	SubscribeMultiple(CRequestColl& request, ResponseTypeEnum type);
	long	Request(CRequestKeyPtr& request, ResponseTypeEnum type, CResponsePtr& response);
	long	RequestMultiple(CRequestColl& request, ResponseTypeEnum type, CResponseVector& response);
	long	GetUpdates(ResponseTypeEnum type, CResponseVector& response);

private:
	long	SubscribeQuote(CRequestKeyPtr& request);
	long	RequestQuote(CRequestKeyPtr& request, CResponsePtr& response);
	long	SubscribeQuoteMultiple(CRequestColl& request);
	long	RequestQuoteMultiple(CRequestColl& request, CResponseVector& response);
	long	CancelLastQuote();

	bool	IsQuotesUpdated();
	long	GetQuoteUpdates(CResponseVector& response);
	bool	SetRegionRate(double rate);
	long	UnNotifiedSubscribtionQuantity(bool skipNotification);
	
	long	RequestRisks(CRequestKeyPtr& request, CResponsePtr& response);
	long	RequestRisksMultiple(CRequestColl& request, CResponseVector& response);
	long	SubscribeRisks(CRequestKeyPtr& request);
	long	SubscribeRisksMultiple(CRequestColl& request);
	long	GetRiskUpdates(CResponseVector& response);

	long	RequestDividends(CRequestKeyPtr& request, CResponsePtr& response);
	long	RequestRates(CRequestKeyPtr& request, CResponsePtr& response);
	long	RequestContracts(CRequestKeyPtr& request, CResponsePtr& response);

public:
	CRequestKeyPtr ConvertRequest(QuoteUpdateParams* request);
	CRequestKeyPtr ConvertRequest(ITicker* request);
	void	Copy(CRequestKeyPtr& spRequest, QuoteUpdateParams& request);
	void	Copy(CRequestKeyPtr& spRequest, ITicker& request);
	void	Copy(CResponsePtr& spResponse, QuoteUpdateInfo& response);
	void	Copy(CResponsePtr& spResponse, IRisks& response);
	void	CopyRequestToVariant(CRequestKeyPtr& spRequest, CComVariant& varRequest);
	void	ProcessDataFeedResponse(const VS::CResponseRecord& response, long messageType);
	bool	IsBadRequest(CRequestKeyPtr& request);
	void	PackRequestToAny(CORBA::Any& anyMsg, const CRequestColl& request);

public:
	CDataFeedGateway(CGatewayNotify* client);
	virtual ~CDataFeedGateway();

public:
	long	InitCorba();
	long	DeinitCorba();
	void	StartPulling();
	void	StopPulling();
	CThreadPool<CTaskExecutor>		m_ThreadPool;
	bool	AddTask(CTaskInterface::pointer	newTask);
	ResponseTypeEnum	MessageTypeToResponse(long messageType);

private:
	CDataFeedGateway();

public:
	bool							__isConnected;
	DDS::IVDataService_var			__dataService;
	DDS::IVDataManager_var			__dataManager;
	DDS::IVSessionManager_var		__sessionManager;
	CLock							__pullLock;

	long							__updateStatus;
	inline void	SetUpdateStatus(UpdateStatusEnum status){
		__updateStatus = __updateStatus | static_cast<long>(status);
	};
	inline void ResetUpdateStatus(UpdateStatusEnum status){
		__updateStatus = __updateStatus ^ static_cast<long>(status);		
	};
	inline long GetUpdateStatus(){return __updateStatus;};
	
	static UINT WINAPI	PullingThread(LPVOID ptrData);
	HANDLE							m_hPullingThread;
};
//----------------------------------------------------------------------------------------------//
class CResponseParser
{
public:
	static CRequestKeyPtr GetRequestKey(const VS::CResponseRecord& record, long messageType);
	static void ParseResponse(const VS::CResponseRecord& record, long messageType, CResponsePtr& spResponse);

private:
	static void	ParseOptionQuote(const VS::CResponseRecord& record, CResponsePtr& spResponse);
	static void	ParseEquityQuote(const VS::CResponseRecord& record, CResponsePtr& spResponse);
	static void ParseFutureQuote(const VS::CResponseRecord& record, CResponsePtr& spResponse);
	static void ParseFutureOptionQuote(const VS::CResponseRecord& record, CResponsePtr& spResponse);
	static void ParseRisks(const VS::CResponseRecord& record, CResponsePtr& spResponse);

	static double round(float value);
};
//----------------------------------------------------------------------------------------------//

class CDataFeedBaseClass: public CDataFeedGateway
{
protected:
	bool m_isConnected;
	DDS::IVDataService_var			m_dataService;
	StockInfoMap					m_stockInfo;
	OptionInfoMap					m_optionInfo;
	CLock							m_csSyncer;

public:
	DDS::IVDataManager_var			m_dataManager;
	DDS::IVSessionManager_var		m_SessionManager;
	CLock							m_pullLock;

	static int m_OptionsSubs;
	static int m_StocksSubs;
	static int m_OnErrorCalled;
	static int m_OnLastQuoteCalled;
	static int m_RisksSubs;
	static int ID;
	int m_ID;

	CDataFeedBaseClass();

	void GetCORBAError(const CORBA::Exception & e, std::string& msg);
	void GetCORBAError(const CORBA::SystemException & e, std::string& msg);

	virtual void InsertUpdate(const VS::CResponseRecord& record, QuoteUpdateMap* req_map, long type) = 0;

	void InsertEquityTypedUpdate(const VS::CResponseRecord& record, RequestMap* req_map, long type);
	void InsertOptionTypedUpdate(const VS::CResponseRecord& record, RequestMap* req_map, long type);
	
	virtual void SubscribeOption(CTickerPtr params) = 0;
	virtual void UnSubscribeOption(CTickerPtr params) = 0;

	virtual void SubscribeStock(CTickerPtr params) = 0;
	virtual void UnSubscribeStock(CTickerPtr params) = 0;

	virtual void SubscribeRisks_(CTickerPtr params) = 0;	
	virtual void UnSubscribeRisks_(CTickerPtr params) = 0;

	virtual void Call_OnError(ErrorNumberEnum enumError, _bstr_t bstrDescription, RequestsTypeEnum enumRequest, CComVariant &varRequest) = 0;
	virtual void Call_OnOption() = 0;
	virtual void Call_OnStock() = 0;
	virtual void Call_OnContracts() = 0;
	virtual void Call_OnLastQuote() = 0;
	virtual void Call_OnSubscribed(QuoteUpdateParams* request) = 0;
	
	bool IsBadRequest(QuoteUpdateParams* request);
	static void CopyRequestToVariant(QuoteUpdateParams* request, CComVariant& varRequest);
	void OnError(QuoteUpdateParams *request, std::string& error_message, RequestsTypeEnum requestType);

	HRESULT InitializeCORBA();
};
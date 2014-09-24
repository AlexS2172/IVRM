#pragma once

#include "ActivContentPlatform/ActivFeedCommon/ActivFeedDefinitions.h"
#include "ActivContentPlatform/ActivFeedApi/ActivFeedApi.h"
//#include "ActivFeedConnection.h"

class CActivFeedConnection;

using namespace Activ;
using namespace ContentPlatform;
using namespace Feed;
using namespace FeedApi;

class CActivFeedContentGateway:	public	ContentGatewayClient,
								public	EgLib::CThread
{
public:
	CActivFeedContentGateway(Application &application, CActivFeedConnection* client, CSettings::pointer userSettings):
	ContentGatewayClient(application, application),
	m_Settings(userSettings)
	{
		Enable();
		m_nextRequestId = 0L;
		m_MaxUpdatesCount = 0L;

		m_bUserDisconnect = false;
		m_bConnected = false;
		m_hOnBreakEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
		m_hOnReconnectionEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
			
		m_client = client;
		
		m_nUpdate = 0;

		EgLib::CThread::Start();
	}

	virtual ~CActivFeedContentGateway(void)
	{
		Deactivate();
		Disable();
		EgLib::CThread::Stop(true, 10000);
		CloseHandle(m_hOnBreakEvent);
		CloseHandle(m_hOnReconnectionEvent);
	}

public:
	CSettings::pointer	m_Settings;
	bool				Activate();
	bool				Deactivate();

public: 
	virtual void OnGetEqualResponse(HeapMessage &response);
	virtual void OnGetMultiplePatternMatchResponse(HeapMessage &response);
	virtual void OnGetMultiplePatternMatchListResponse(HeapMessage &response);
	virtual void OnGetMultipleEqualResponse(HeapMessage &response);
	virtual void OnGetMatchResponse(HeapMessage &response);
	virtual void OnRecordUpdate(HeapMessage &update);

	template<typename T>
	void	OnResponse(HeapMessage &response,  T& Params,  RequestIdPtr spRequestID, bool bIsComplete, bool bIsValid);

	template<typename T>
	void	ParseResponseParameters(T& responseParameters, RequestIdPtr spRequestID, bool Single);

	//Service call back
	virtual StatusCode OnBreak(HeapMessage &breakMessage);
	virtual StatusCode OnDisconnect(HeapMessage &disconnectMessage);
	virtual StatusCode OnConnectFailed(HeapMessage &connectFailMessage);
	virtual StatusCode OnConnect(HeapMessage &connectMessage);

public:
	volatile long	m_nextRequestId;
	volatile long	m_UpdateCount;
	long			m_MaxUpdatesCount;
	StatusCode		Request(QuoteRequestPtr& spRequest, QuoteAtomPtr& spQuote);
	StatusCode		RequestMultiple(CRequestVector& request, CQuoteVector& response);
	StatusCode		Subscribe(QuoteRequestPtr& spRequest);
	StatusCode		SubscribeMultiple(CRequestVector& request);

public:
	//service functions
	StatusCode		ParseResponseBlock(const ResponseBlock& rb, QuoteInfo* pQuoteInfo, bool bRealTime, QuoteRequest* pRequest);
	void			UpdateField(const FieldListValidator::Field& filed, QuoteInfo* pQuoteInfo, QuoteRequest* pRequest);
	
	bool			GetQuote(QuoteRequestPtr& spRequest, QuoteAtomPtr& spQuote);
	void			RemoveQuote(QuoteRequestPtr& spRequest);
	bool			GetRequest(std::string Key, RequestIdPtr& spRequestID, QuoteRequestPtr& spRequest);
	void			AddRequest(std::string Key, QuoteRequestPtr& spRequest, RequestIdPtr& spRequestID);
	void			RemoveRequest(RequestIdPtr& spRequestID);
	void			RemoveRequestKey(RequestIdPtr& spRequestID, std::string Key);
	bool			IsSubscribed(QuoteRequestPtr& spRequest);
	bool			GetUpdates(CQuoteVector& quotes);
	bool			IsConnected(){return m_bConnected;}

	UINT			Run();
	void			Resubscribe();

private:
	
	typedef				bool (CActivFeedContentGateway::*FUNC)(HeapMessage &response);
	bool				ProcessSingleFutureSubscribeResponse(HeapMessage &response);
	typedef				std::map<RequestIdPtr, FUNC, RequestIdPtr_less>	Functions;
	Functions			m_functions;

	CRequestMap			m_RequestMap;
	CRequestQuoteMap	m_QuoteMap;
	CSubscribtionMap	m_SubscribtionMap;
	CLock				m_RequestLock;
	CLock				m_QuoteLock;

	StatusCode			RequestStock(QuoteRequestPtr spRequest);
	StatusCode			RequestIndex(QuoteRequestPtr spRequest);
	StatusCode			RequestOption(QuoteRequestPtr spRequest);
	StatusCode			RequestFuture(QuoteRequestPtr spRequest);
	StatusCode			RequestFutureOption(QuoteRequestPtr spRequest);
	StatusCode			RequestCurrency(QuoteRequestPtr spRequest);

	StatusCode			SubscribeStock(QuoteRequestPtr spRequest);
	StatusCode			SubscribeIndex(QuoteRequestPtr spRequest);
	StatusCode			SubscribeOption(QuoteRequestPtr spRequest);
	StatusCode			SubscribeFuture(QuoteRequestPtr spRequest);
	StatusCode			SubscribeFutureOption(QuoteRequestPtr spRequest);
	StatusCode			SubscribeCurrency(QuoteRequestPtr spRequest);

	StatusCode			RequestOptionMultiple(CRequestVector& request);
	StatusCode			SubscribeOptionMultiple(CRequestVector& request);
	StatusCode			SubscribeEquityMultiple(CRequestVector& request);

	HANDLE					m_hOnBreakEvent;
	HANDLE					m_hOnReconnectionEvent;
	bool					m_bUserDisconnect;
	CActivFeedConnection*	m_client;
	bool					m_bConnected;
	CSubsQuoteMap			m_subs;
	void				Filter(CRequestVector& request, InstrumentTypeEnum type, CRequestVector& result, CQuoteVector& quotes);

	long					m_nUpdate;
};
//------------------------------------------------------------------------------------------------//
class CRequestConverter
{
public:
	//--------------------------------------------------------------------------------------------//
	static	void EgarOption2Activ(QuoteRequestPtr& request, std::string& requestKey)
	{
		static Activ::ContentPlatform::Feed::UsEquityOptionHelper optionHelper;

		if (request->Exchange.length() == 0)
			request->Exchange = "O";

		if(	STATUS_CODE_FAILURE == 
			optionHelper.GetSymbolFromOsiSymbol(requestKey, request->Symbol, "O")){
			requestKey = request->Symbol + ".O";
		}
	};
	//--------------------------------------------------------------------------------------------//
	static	void EgarIndex2Activ(QuoteRequestPtr& request, std::string& requestKey)
	{
		requestKey = "=" + request->Symbol + "." + "*";
	};
	//--------------------------------------------------------------------------------------------//
	static	void EgarStock2Activ(QuoteRequestPtr& request, std::string& requestKey)
	{
		requestKey = request->Symbol + "." + request->Exchange;
	};
	//--------------------------------------------------------------------------------------------//
};

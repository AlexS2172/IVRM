#pragma once
#include "ActivProviderBase.h"
#include "ActivFeedContentGateway.h"
#include <map>

using namespace Activ;
using namespace std;
extern void	Activ2Egar( std::string& Ee, InstrumentTypeEnum t  );

class CAsyncNotification
{
public:
	typedef enum {enRequestLast, enSubscribe} CNotificationType;

	CAsyncNotification(_QuoteRequestPtr& spRequest)
	: m_spRequest(spRequest)
	, m_enType(enSubscribe)	{};

	CAsyncNotification(_QuoteRequestPtr& spRequest, _QuoteInfoPtr& spResponse)
	: m_spRequest (spRequest)
	, m_spResponse(spResponse)
	, m_enType(enRequestLast){};

public:
	CNotificationType         GetType()     const {return m_enType;}
	const _QuoteRequestPtr&	  GetRequest()  const {return m_spRequest;}
	const _QuoteInfoPtr&	  GetResponse() const {return m_spResponse;}
private:
	_QuoteInfoPtr		m_spResponse;
	_QuoteRequestPtr	m_spRequest;

	CNotificationType	  m_enType;	

private:
	CAsyncNotification(){};
	CAsyncNotification(const CAsyncNotification& ){};
};

typedef boost::shared_ptr<CAsyncNotification> CAsyncNotificationPtr;

class CActivFeedAppication: 
	public AgentApplication,
	public CStrategyT<CActivFeedAppication,	CAsyncNotificationPtr>
{
	CActivProviderBase*			m_pBase;			// ptr to encapsulated provider
													// ( CActivFeedStructureProvider or CActivFeedBatchPriceInfo )
	CActivFeedContentGateway	m_Gateway;			

protected:
	virtual StatusCode	OnMessage(HeapMessage &message);
	virtual void		OnShutdown();

public:
	void SwitchOffHourUpdateFilter( bool b ) { m_Gateway.SwitchOffHourUpdateFilter( b ) ; }
	// ctor/dtor, activation/deactivation
	CActivFeedAppication(CActivProviderBase* pBase, AgentApplication::Settings& settings)
		:AgentApplication(settings), m_Gateway(*this), m_pBase(pBase)
	{
		CStrategyT<CActivFeedAppication,	CAsyncNotificationPtr>::Start();
	}

	~CActivFeedAppication(void)
	{
		CStrategyT<CActivFeedAppication,	CAsyncNotificationPtr>::Stop(true, 1000);
		PostDiesToThreads();
		Shutdown();
	}

	bool		Connect() { 
		StatusCode SC =	STATUS_CODE_FAILURE;
		if( IsActive() ) return true;
		GetProcess()->SetState( IProcess::STATE_RUNNING );
		SC = StartThread();
		if( SC != STATUS_CODE_SUCCESS )
			EgLib::CComErrorWrapper::ThrowError(E_FAIL, StatusCodeToString( SC ).c_str() );
		return m_Gateway.Activate() ? true : false;
	}
	void		Disconnect() 
	{ 
		CStrategyT<CActivFeedAppication,	CAsyncNotificationPtr>::Stop(true, 1000);
		m_Gateway.Deactivate(); 
		//WaitForThreadsToExit(1500l);
		PostDiesToThreads();
	}
	bool		IsActive() const {return m_Gateway.IsActive();}
	
	// StructureProvider functions
	void		RequestFuture(FutureParams *Params)
	{
		EgLib::CEgLibTraceManager::Trace( LogInfo, __FUNCTION__ , _T("Request - %S"), SafeBSTR( Params->FutureSymbol ) );
		CRequest<FutureParams>::CRequestPtr spStockRequest = 
			CRequest<FutureParams>::CRequestPtr(new CRequest<FutureParams>(CRequestBase::_enRequestFuture, Params));
		if( spStockRequest!=NULL ) {
			StatusCode SC = m_Gateway.RequestFuture( spStockRequest );
			if ( SC != STATUS_CODE_SUCCESS ) {
				EgLib::CEgLibTraceManager::Trace( LogError, __FUNCTION__ , _T("Failed - %s"), StatusCodeToString( SC ).c_str());
				EgLib::CComErrorWrapper::ThrowError(E_FAIL, StatusCodeToString( SC ).c_str() );
			}
		}
	}

	DOUBLE 		SetRegionRate(BSTR bstrRegion, DOUBLE dRate)
	{
		return 1.0;
	}

	void		RequestFutures(FutureRootParams *Params)
	{
		EgLib::CEgLibTraceManager::Trace( LogInfo, __FUNCTION__ , _T("Request - %S"), SafeBSTR( Params->FuturesRootSymbol ) );
		CRequest<FutureRootParams>::CRequestPtr spStockRequest = 
			CRequest<FutureRootParams>::CRequestPtr( new CRequest<FutureRootParams>(CRequestBase::_enRequestFutures, Params) );
		if( spStockRequest != NULL ) {
			StatusCode SC = m_Gateway.RequestFutures(spStockRequest);
			if ( SC != STATUS_CODE_SUCCESS ) {
				EgLib::CEgLibTraceManager::Trace( LogError, __FUNCTION__ , _T("Failed - %s"), StatusCodeToString( SC ).c_str());
				EgLib::CComErrorWrapper::ThrowError(E_FAIL, StatusCodeToString( SC ).c_str() );
			}
		}
	}
	void		RequestFuturesOptions(FuturesOptionParams *Params)
	{
		EgLib::CEgLibTraceManager::Trace( LogInfo, __FUNCTION__ , _T("Request - %S.%S"), SafeBSTR( Params->FutureSymbol ) , SafeBSTR( Params->Exchange ) );
		CRequest<FuturesOptionParams>::CRequestPtr spOptRequest = 
			CRequest<FuturesOptionParams>::CRequestPtr(new CRequest<FuturesOptionParams>(CRequestBase::_enRequestFuturesOption, Params));
		if( spOptRequest != NULL ) {
			StatusCode SC = m_Gateway.RequestFuturesOptions(spOptRequest);
			if ( SC != STATUS_CODE_SUCCESS ) {
				EgLib::CEgLibTraceManager::Trace( LogError, __FUNCTION__ , _T("Failed - %s"), StatusCodeToString( SC ).c_str());
				EgLib::CComErrorWrapper::ThrowError(E_FAIL, StatusCodeToString( SC ).c_str() );
			}
		}
	}
	void		RequestLastQuote( _QuoteRequestPtr& spRequest, CActivFeedContentGateway::CRequestType enType)
	{ 
		EgLib::CEgLibTraceManager::Trace( LogInfo, __FUNCTION__ , _T("RLQ - %s.%s[%d]"), spRequest->Symbol.c_str(), spRequest->Exchange.c_str(), spRequest->Type );
		StatusCode SC = m_Gateway.RequestLastQuote( spRequest, enType);
		if ( SC != STATUS_CODE_SUCCESS  ) {
			EgLib::CEgLibTraceManager::Trace( LogError, __FUNCTION__ , _T("Failed - %s"), StatusCodeToString( SC ).c_str());
			EgLib::CComErrorWrapper::ThrowError( E_FAIL, StatusCodeToString( SC ).c_str() );
		}
	}
	void		RequestOptions(OptionParams *Params)
	{
		EgLib::CEgLibTraceManager::Trace( LogInfo, __FUNCTION__ , _T("Request - %S.%S"), SafeBSTR( Params->UnderlyingSymbol ) , SafeBSTR( Params->Exchange ) );
		CRequest<OptionParams>::CRequestPtr spOptRequest = 
			CRequest<OptionParams>::CRequestPtr(new CRequest<OptionParams>(CRequestBase::_enRequestOption, Params));
		if( spOptRequest != NULL ) {
			StatusCode SC = m_Gateway.RequestOptions(spOptRequest);
			if ( SC != STATUS_CODE_SUCCESS ) {
				EgLib::CEgLibTraceManager::Trace( LogError, __FUNCTION__ , _T("Failed - %s"), StatusCodeToString( SC ).c_str());
				EgLib::CComErrorWrapper::ThrowError(E_FAIL, StatusCodeToString( SC ).c_str() );
			}
		}
	}
	void		RequestStock(StockParams *Params)
	{
		EgLib::CEgLibTraceManager::Trace( LogInfo, __FUNCTION__ , _T("Request - %S.%S"), SafeBSTR( Params->Stock ) , SafeBSTR( Params->Exchange ) );
		CRequest<StockParams>::CRequestPtr spStockRequest = 
			CRequest<StockParams>::CRequestPtr(new CRequest<StockParams>(CRequestBase::_enRequestStock, Params));
		if( spStockRequest!=NULL ) {
			StatusCode SC = m_Gateway.RequestStock(spStockRequest);
			if ( SC != STATUS_CODE_SUCCESS ) {
				EgLib::CEgLibTraceManager::Trace( LogError, __FUNCTION__ , _T("Failed - %s"), StatusCodeToString( SC ).c_str());
				EgLib::CComErrorWrapper::ThrowError(E_FAIL, StatusCodeToString( SC ).c_str() );
			}
		}
	}
	void	CancelFuture(VARIANT Params) { m_Gateway.CancelFuture(Params); }
	void	CancelFutures(VARIANT Params) {  m_Gateway.CancelFutures(Params); }
	void	CancelFuturesOptions(VARIANT Params) { m_Gateway.CancelFuturesOptions(Params);	}
	void	CancelOptions(VARIANT Params) { m_Gateway.CancelOptions(Params); }
	void	CancelStock(VARIANT Params) { m_Gateway.CancelStock(Params); }

	// StructureProvider notification processing				
	void OnFuture( CRequest<FutureParams>::CRequestPtr& spRequest, CResponse<FutureResultsEx>::CResponsePtr& spResult )
	{
		if( spRequest != NULL && spResult != NULL && m_pBase ) {
			EgLib::CEgLibTraceManager::Trace( LogInfo, __FUNCTION__ , _T(" Future Response - %S, %S, cntr size - %d, opt cntr size - %d, exp date - %f,  cls prc - %f"), 
			SafeBSTR(spResult->m_respParams->Symbol),
			SafeBSTR(spResult->m_respParams->Description),
			spResult->m_respParams->ContractSize,
			spResult->m_respParams->OptionContractSize,
			spResult->m_respParams->ExpirationDate,
			spResult->m_respParams->ClosePrice );
			CComVariant vtRequest;
			CComVariant vtReSponse;
			spRequest->m_recParams.CopyTo(vtRequest);
			spResult->m_respParams.CopyTo(vtReSponse);
			m_pBase->OnFuture(vtRequest, vtReSponse);
		}
	}
	void OnFutureByRoot(CRequest<FutureRootParams>::CRequestPtr& spRequest, CResponse<FutureResultsEx>::CResponsePtr& spResult, bool bIsLastOption)
	{
		if( spRequest != NULL  && spResult != NULL && m_pBase ) {
			EgLib::CEgLibTraceManager::Trace( LogInfo, __FUNCTION__ , _T("FutureByRoot Response: - %S, %S, cntr size - %d, opt cntr size - %d, exp date - %f, cls prc - %f"), 
			SafeBSTR(spResult->m_respParams->Symbol), SafeBSTR(	spResult->m_respParams->Description ),
			spResult->m_respParams->ContractSize,
			spResult->m_respParams->OptionContractSize,
			spResult->m_respParams->ExpirationDate,
			spResult->m_respParams->ClosePrice );
			CComVariant vtRequest;
			CComVariant vtReSponse;
			spRequest->m_recParams.CopyTo(vtRequest);
			spResult->m_respParams.CopyTo(vtReSponse);
			m_pBase->OnFutureByRoot(vtRequest, vtReSponse, bIsLastOption);
		}
	}
	void OnFuturesOption(CRequest<FuturesOptionParams>::CRequestPtr& spRequest, CResponse<FuturesOptionResultsEx>::CResponsePtr& spResult, bool bIsLastOption)
	{
		if( spRequest != NULL && spResult != NULL && m_pBase ) {
			EgLib::CEgLibTraceManager::Trace( LogInfo, __FUNCTION__ , _T( "FuturesOption Response %s: %S, strk - %f, exp.date - %f, type - %c, lot size - %d, cls prc - %f "), 
				bIsLastOption ? _T("Last") : _T(" "), SafeBSTR(spResult->m_respParams->Series ),
				spResult->m_respParams->StrikePrice,
				spResult->m_respParams->ExpirationDate,
				spResult->m_respParams->OptionType == enPut ? 'P' : 'C',
				spResult->m_respParams->LotSize,
				spResult->m_respParams->ClosePrice );
			CComVariant vtRequest;
			CComVariant vtReSponse;
			spRequest->m_recParams.CopyTo(vtRequest);
			spResult->m_respParams.CopyTo(vtReSponse);
			m_pBase->OnFuturesOption(vtRequest, vtReSponse, bIsLastOption);
		}
	}
	void OnOption(CRequest<OptionParams>::CRequestPtr& spRequest, CResponse<OptionResultsEx>::CResponsePtr& spResult, bool bIsLastOption)
	{
		if( spRequest != NULL && spResult != NULL && m_pBase ) {
			EgLib::CEgLibTraceManager::Trace( LogInfo, __FUNCTION__ , _T("Response - %S.%S"), SafeBSTR( spRequest->m_recParams->UnderlyingSymbol ), SafeBSTR( spRequest->m_recParams->Exchange ) );
			CComVariant vtRequest;
			CComVariant vtReSponse;
			spRequest->m_recParams.CopyTo(vtRequest);
			spResult->m_respParams.CopyTo(vtReSponse);
			m_pBase->OnOption(vtRequest, vtReSponse, bIsLastOption);
		}
	}
	void OnSPError(ErrorNumberEnum enumError, _bstr_t bstrDescription, RequestsTypeEnum enumRequest, const CComVariant &varRequest)
	{
		if( m_pBase ) {
			m_pBase->OnError(enumError, bstrDescription, enumRequest, varRequest);
		}
	}
	void OnStock(CRequest<StockParams>::CRequestPtr& spRequest, CResponse<StockResultsEx>::CResponsePtr& spResult)
	{
		if( spRequest != NULL && spResult != NULL && m_pBase ) {
			EgLib::CEgLibTraceManager::Trace( LogInfo, __FUNCTION__ , _T("Response - %S.%S"), SafeBSTR( spRequest->m_recParams->Stock ), SafeBSTR( spRequest->m_recParams->Exchange ) );
			CComVariant vtRequest;
			CComVariant vtReSponse;
			spRequest->m_recParams.CopyTo(vtRequest);
			spResult->m_respParams.CopyTo(vtReSponse);
			m_pBase->OnStock(vtRequest, vtReSponse);
		}
	}

	// BatchPriceInfo functions
	
	bool SubscribeQuote( _QuoteRequestPtr& p, CActivFeedContentGateway::CRequestType enType) 
	{
		return m_Gateway.SubscribeQuote( p, enType ); 
	}

	void		Unsubscribe( _QuoteRequestPtr& p ) 
	{  
		if( p )
			EgLib::CEgLibTraceManager::Trace(LogInfo, __FUNCTION__ ,\
				_T("Unsubsription request for [%s.%s[%d]]"), p->Symbol.c_str(), p->Exchange.c_str(), p->Type );
		else
			EgLib::CEgLibTraceManager::Trace(LogInfo, __FUNCTION__ ,\
			_T("Unsubsription request for all subscriptions") );
		bool rc = m_Gateway.OnUnsubscribe( p );
		EgLib::CEgLibTraceManager::Trace( rc ? LogInfo : LogWarning , __FUNCTION__ ,\
			_T("%s"),  rc ? _T("Complete") : _T("Failed") );

	}
	void		SetMarketOpenTime( const unsigned int t ){ m_Gateway.SetMarketOpenTime( t );	}
	
	// BatchPriceInfo helpers
	void		InitQUI(QuoteUpdateInfo* pQuoteIndo, long LotSize = BAD_LONG_VALUE  )
	{
		pQuoteIndo->BidPrice     = BAD_DOUBLE_VALUE;
		pQuoteIndo->AskPrice     = BAD_DOUBLE_VALUE;
		pQuoteIndo->LastPrice    = BAD_DOUBLE_VALUE;
		pQuoteIndo->OpenInterest = BAD_DOUBLE_VALUE;
		pQuoteIndo->OpenPrice    = BAD_DOUBLE_VALUE;
		pQuoteIndo->ClosePrice   = BAD_DOUBLE_VALUE;
		pQuoteIndo->HighPrice	 = BAD_DOUBLE_VALUE;
		pQuoteIndo->LowPrice	 = BAD_DOUBLE_VALUE;
		pQuoteIndo->NetChange    = BAD_DOUBLE_VALUE;
		pQuoteIndo->SeqNum		 = BAD_LONG_VALUE;
		pQuoteIndo->BidSize		 = BAD_LONG_VALUE;
		pQuoteIndo->AskSize      = BAD_LONG_VALUE; 
		pQuoteIndo->Volume		 = BAD_LONG_VALUE;
		pQuoteIndo->Skipped		 = BAD_LONG_VALUE;
		pQuoteIndo->TotalRequests= BAD_LONG_VALUE;
		pQuoteIndo->WaitTime     = BAD_LONG_VALUE; 
		pQuoteIndo->PriceTick    = BAD_LONG_VALUE;
		pQuoteIndo->LotSize		 = LotSize;
		pQuoteIndo->UpdateDateTime = 0;
		pQuoteIndo->Currency	 = NULL;
		pQuoteIndo->Exchange	 = NULL;
		pQuoteIndo->BidExchange	 = NULL;
		pQuoteIndo->AskExchange	 = NULL;
	}
	bool		IsUpdated() const { return m_Gateway.Updates(); }
	long		GetUpdatesCount() const  {  return m_Gateway.GetUpdatesCount(); }
	void		CopyUpdatedQuotes( QuoteUpdateFullInfo *pqufi, UINT size ) { m_Gateway.CopyUpdatedQuotes( pqufi, size ); }

	// BatchPriceInfo notification processing
	void		OnBIError(ErrorNumberEnum enumError, const char *e, RequestsTypeEnum enumRequest, _QuoteRequestPtr& p )
	{
		if ( enumError == enConnectionWasDisconnected || enProviderConnected == enumError ) {
			if ( m_pBase ) {
				CComVariant vp;
				m_pBase->OnError(enumError, _bstr_t(e), enumRequest, vp );
			}
			return;
		}
		if ( enumError == enSymbolNotSupported && \
			( enumRequest == enRequestLastQuote || enumRequest == enSubscribeQuote ) && \
			( p->Type == enSTK || p->Type == enIDX ) ) {
				if( p->TryCount < 1 ) {
					p->TryCount++;
					switch( p->Type) {
					case enSTK:
						p->Type = enIDX;
						break;
					case enIDX:
					default:
						p->Type = enSTK;
					}
					enumRequest == enRequestLastQuote ? RequestLastQuote( p, CActivFeedContentGateway::enRegularRequest) : SubscribeQuote( p, CActivFeedContentGateway::enRegularRequest );
					return;
				}
				else{
					switch( p->Type) {
					case enSTK:
						p->Type = enIDX;
						break;
					case enIDX:
					default:
						p->Type = enSTK;
					}
				}
			}
			if ( m_pBase)
			{
				CComVariant vp;
				_QuoteUpdateParams rp;
				rp.Init();

				p->AssignToQuoteUpdateParams( rp );
				rp.CopyTo(vp);
				m_pBase->OnError( enumError, enumRequest == enSubscribeQuote  ?_bstr_t(_T("Couldn't subsribe to symbol") ) : _bstr_t(_T("Couldn't request last quote") ), enumRequest, vp );
				EgLib::CEgLibTraceManager::Trace(LogWarning, __FUNCTION__ ,\
					_T("BI Error[%d], Msg[%s], Type[%d], Symbol[%s.%s[%d]]"), enumError, e, enumRequest,\
					p->Symbol.c_str(), p->Exchange.c_str(), p->Type );
			}
	}
	void OnRequstLast( _QuoteRequestPtr& spRequest, _QuoteInfoPtr& spQuoteInfo )
	{
		CAsyncNotificationPtr spRequestLast(new CAsyncNotification(spRequest, spQuoteInfo));
		if(spRequestLast)
			CStrategyT<CActivFeedAppication,	CAsyncNotificationPtr>::PushData(spRequestLast);

//		if( m_pBase )
//		{
//			if( spRequest->TryCount )
//			{
//				switch( spRequest->Type)
//				{
//					case enSTK:
//						spRequest->Type = enIDX;
//						break;
//					case enIDX:
//					default:
//						spRequest->Type = enSTK;
//				}
//			}
//
//			CComVariant vp;
//			CComVariant vi;
//			_QuoteUpdateInfo ri;
//			_QuoteUpdateParams rp;
//			ri.Init();
//			rp.Init();
//
//			if( spQuoteInfo->AskExchange.length() )
//				Activ2Egar( spQuoteInfo->AskExchange, spRequest->Type );
//			if( spQuoteInfo->BidExchange.length() )
//				Activ2Egar( spQuoteInfo->BidExchange, spRequest->Type );
//			if( spQuoteInfo->Exchange.length() )
//				Activ2Egar( spQuoteInfo->Exchange, spRequest->Type );
//
//			InitQUI( ri );
//			spQuoteInfo->AssignToQuoteUpdateInfo( ri );
//			spRequest->AssignToQuoteUpdateParams( rp );
//
//			rp.CopyTo( vp );
//			ri.CopyTo( vi );
//			m_pBase->OnLastQuote( vp, vi );
//			EgLib::CEgLibTraceManager::Trace(LogInfo, __FUNCTION__ ,\
//				_T("RLQ Response QUI[%s.%s[%d]]"), spRequest->Symbol.c_str(), spRequest->Exchange.c_str(), spRequest->Type );
//		}
	}
	void		OnSubscribed( _QuoteRequestPtr& spRequest )
	{
		CAsyncNotificationPtr spRequestLast(new CAsyncNotification(spRequest));
		if(spRequestLast)
		{
			::InterlockedIncrement(&m_pBase->m_lUnnotifiedSubscriptions);
			CStrategyT<CActivFeedAppication,	CAsyncNotificationPtr>::PushData(spRequestLast);
		}
			EgLib::CEgLibTraceManager::Trace(LogInfo, __FUNCTION__ ,\
				_T("_SQU Response- OK: QUI[%s.%s[%d]]"), spRequest->Symbol.c_str(), spRequest->Exchange.c_str(), spRequest->Type );

//		if( m_pBase )
//		{
//			if( spRequest->TryCount )
//			{
//				switch( spRequest->Type) 
//				{
//					case enSTK:
//						spRequest->Type = enIDX;
//						break;
//					case enIDX:
//					default:
//						spRequest->Type = enSTK;
//				}
//			}
//			CComVariant vp;
//			_QuoteUpdateParams rp;
//			rp.Init();
//			spRequest->AssignToQuoteUpdateParams( rp );
//			rp.CopyTo( vp );
//			EgLib::CEgLibTraceManager::Trace(LogInfo, __FUNCTION__ ,\
//				_T("SQU Response- OK: QUI[%s.%s[%d]]"), spRequest->Symbol.c_str(), spRequest->Exchange.c_str(), spRequest->Type );
//			m_pBase->OnSubscribed( vp );
//		}
	}

	public:
		HRESULT OnStrategyInitialize(){return CoInitializeEx(NULL,COINIT_MULTITHREADED);}
		void    OnStrategyUninitialize(){CoUninitialize();}
		virtual void OnData(CAsyncNotificationPtr& Data);
};

typedef boost::shared_ptr<CActivFeedAppication> CActivFeedAppicationPtr;
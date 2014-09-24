#pragma once

#include "ActivContentPlatform/ActivFeedCommon/ActivFeedDefinitions.h"
#include "ActivContentPlatform/ActivFeedApi/ActivFeedApi.h"

#include "ActivProviderBase.h"

using namespace Activ;
using namespace ContentPlatform;
using namespace Feed;
using namespace FeedApi;

struct optionRootInfo
{
	optionRootInfo()
		:lotsize(0)
		,expirationType(0)
		,SettlementType(-1){}

	std::string rootSymbol;
	std::string currency;
	long lotsize;
	long SettlementType;
	long expirationType;
};

class CActivFeedAppication;

class CActivFeedContentGateway :
	public FeedApi::ContentGatewayClient
{
public:
	typedef enum 
	{
		enRegularRequest       =0,
		enStartMultipleRequest,
		enMultipleRequest,
		enEndMultipleRequest
	} CRequestType;

private:
	typedef std::map<RequestIdPtr, CRequestBasePtr, RequestIdPtr_less>  CRequestsContainer;
	typedef std::map<std::string, optionRootInfo> COptionRootInfoMap;
	typedef std::map<string, CResponse<FutureResultsEx>::CResponsePtr>	CFutMap;

	typedef CComRecPtr<FuturesOptionParams, &LIBID_PRICEPROVIDERSLib >	_FuturesOptionParams;
	typedef CComRecPtr<FutureRootParams, &LIBID_PRICEPROVIDERSLib >	_FutureRootParams;
	typedef CComRecPtr<FutureParams, &LIBID_PRICEPROVIDERSLib >	_FutureParams;
	typedef CComRecPtr<OptionParams, &LIBID_PRICEPROVIDERSLib >	_OptionParams;
	typedef CComRecPtr<StockParams, &LIBID_PRICEPROVIDERSLib >	_StockParams;


	typedef struct _QuoteAtom
	{
		_QuoteInfoPtr		m_spResponse;
		_QuoteRequestPtr	m_spRequest;
	};

	typedef boost::shared_ptr<_QuoteAtom>	_QuoteAtomPtr;

	typedef struct _RealtimeQuoteAtom
	{
		_QuoteRequestPtr		m_spRequest;
		_QuoteInfoPtr			m_spResponse;

		CLock					m_Lock;
		SubscriptionCookie		m_Cookie;
		bool					m_bUpdate;

		_RealtimeQuoteAtom()
			:m_spRequest( new _QuoteRequest ) 
			,m_spResponse( new _QuoteInfo )
			,m_Cookie(NULL)
			,m_bUpdate(false)
		{}
	}; 

	typedef boost::shared_ptr<_RealtimeQuoteAtom>	_RealtimeQuoteAtomPtr;

	typedef std::pair<std::string, _QuoteAtomPtr >					CLPair;
	typedef std::pair<std::string, _RealtimeQuoteAtomPtr >			CSPair;

	typedef std::map<std::string, _QuoteAtomPtr >					CLMap;
	typedef std::map<std::string, _RealtimeQuoteAtomPtr >			CSMap;

	typedef std::map<RequestIdPtr, CLMap, RequestIdPtr_less >		CLUMap;
	typedef std::map<RequestIdPtr, CSMap, RequestIdPtr_less >		CPSMap;

public:
	class FieldSpecification
	{
	public:
		/**
		*	@brief	Constructor.
		*
		*	@param	fieldId the field id.
		*/
		FieldSpecification(const FieldId fieldId = FID_UNDEFINED);

		FieldId	m_fieldId;			///< The field id.
	};

	typedef std::vector<FieldSpecification> FieldSpecificationList;	///< The field specification list type definition.

public: 
	virtual void OnGetEqualResponse(HeapMessage &response);
	virtual void OnGetMultiplePatternMatchResponse(HeapMessage &response);
	virtual void OnGetMultiplePatternMatchListResponse(HeapMessage &response);
	virtual void OnGetMultipleEqualResponse(HeapMessage &response);
	virtual void OnGetMatchResponse(HeapMessage &response);

public:
	CActivFeedContentGateway(Application &application) 
		: ContentGatewayClient(application, application)
		, m_nReconnects(0)
		, m_bActivated(false)
		, m_nextMultipleOptionRequestId(0L)
		, m_nextRequestId(0)
		, m_UpdateCount(0)
		,m_bReconnected(false)
		,m_bDisconnection(false)
	{ 
		memset( &m_MarketOpenTime, 0, sizeof( SYSTEMTIME ) );
		Enable(); 
	}

	void ConvertRequest(_QuoteRequestPtr request, std::string& key);
	void ConvertExchange(_QuoteRequestPtr request, std::string& exchange);

	virtual	~CActivFeedContentGateway(void)
	{
		Deactivate(); 
		Disable(); 
	}

	bool	Activate();
	void	Deactivate()
	{
		m_bActivated = false;
		m_bDisconnection = true;

		m_hOnDisconnect.Close();
		m_hOnDisconnect.Attach(::CreateEvent(NULL, FALSE, FALSE, NULL));

		if(Disconnect() == STATUS_CODE_SUCCESS)
			EgLib::WaitWithEvents(m_hOnDisconnect, 5000);
	}


	bool	IsActive() const { return m_bActivated;}

	StatusCode RequestStock(CRequest<StockParams>::CRequestPtr& spStockRequest);
	void		CancelStock(VARIANT Params);
	void		CancelOptions(VARIANT Params);
	void		CancelFuture(VARIANT Params);
	void		CancelFutures(VARIANT Params);
	void		CancelFuturesOptions(VARIANT Params);
	StatusCode RequestOptions(CRequest<OptionParams>::CRequestPtr& spOptionRequest);
	StatusCode RequestIndex(CRequest<StockParams>::CRequestPtr& spStockRequest, CResponse<StockResultsEx>::CResponsePtr spResponce);
	StatusCode RequestFuture(CRequest<FutureParams>::CRequestPtr& spStockRequest);
	StatusCode RequestFutures(CRequest<FutureRootParams>::CRequestPtr& spStockRequest);
	StatusCode RequestFuturesOptions(CRequest<FuturesOptionParams>::CRequestPtr& spOptionRequest);

	/* ActiveFeedPriceProvider public functions */
	bool		Updates()	const	{	return m_UpdateCount ? true : false; }
	long		GetUpdatesCount() const {	return m_UpdateCount; }
	void		CopyUpdatedQuotes( QuoteUpdateFullInfo *pqufi, UINT size );
	StatusCode	RequestLastQuote( _QuoteRequestPtr& spRequest, CActivFeedContentGateway::CRequestType enRequestType );
	bool		SubscribeQuote( _QuoteRequestPtr& p, CActivFeedContentGateway::CRequestType enType);
	bool		OnUnsubscribe( _QuoteRequestPtr& p );
	void		SetMarketOpenTime( const unsigned int t);
	void		SwitchOffHourUpdateFilter( bool bSwitch ) { m_bFilterOffHourUpdates = bSwitch; }
	/* End of ActiveFeedPriceProvider public functions */

private:

	CRequestBasePtr GetRequest(RequestIdPtr spRequestId)
	{
		CAutoLock lock(m_csContainer);
		CRequestsContainer::iterator itr =  m_Container.find(spRequestId);
		if(itr != m_Container.end())
			return itr->second;
		return CRequestBasePtr();
	}
	void   DeleteRequest(RequestIdPtr spRequestId)
	{
		CAutoLock lock(m_csContainer);
		CRequestsContainer::iterator itr =  m_Container.find(spRequestId);
		if(itr != m_Container.end())
			m_Container.erase(itr);
	}

	void CActivFeedContentGateway::AddLastQuoteRequestToMap(const std::string& sKey,  _QuoteRequestPtr& spRequest, RequestIdPtr& requestID )
	{
		_QuoteAtomPtr qufi( new _QuoteAtom );
		qufi->m_spResponse = _QuoteInfoPtr( new _QuoteInfo );
		qufi->m_spRequest = spRequest;
		CAutoLock l( m_csLU );
		m_LU[ requestID ].insert(CLPair(sKey, qufi));
	}

	void CActivFeedContentGateway::AddSubscribeRequestToMap(const std::string& sKey,  _RealtimeQuoteAtomPtr& spAtom, RequestIdPtr& requestID )
	{
		CAutoLock l( m_csPSMap );
		m_PSMap[ requestID ].insert(CSPair(sKey, spAtom));
	}


	void	OnStock(CRequest<StockParams>::CRequestPtr& spRequest, HeapMessage &response);
	void					OnRecordUpdate( Activ::HeapMessage &update );
	StatusCode	RequestSTK( _QuoteRequestPtr& spRequest, std::string& xch, const std::string& region );
	StatusCode	RequestOPT( _QuoteRequestPtr& spRequest, CActivFeedContentGateway::CRequestType enRequestType);
	StatusCode	RequestIDX( _QuoteRequestPtr& spRequest, std::string& As );
	StatusCode	RequestFUT( _QuoteRequestPtr& spRequest );
	StatusCode	RequestFOP( _QuoteRequestPtr& spRequest );
	StatusCode  RequestFX( _QuoteRequestPtr&  spRequest );

	bool					SubscribeSTK( _RealtimeQuoteAtomPtr& r, std::string& Ae, std::string& regions );
	bool					SubscribeIDX( _RealtimeQuoteAtomPtr& r, std::string& Idx );
	bool					SubscribeOPT( _RealtimeQuoteAtomPtr& r, CActivFeedContentGateway::CRequestType enRequestType);
	bool					SubscribeFUT( _RealtimeQuoteAtomPtr& r );
	bool					SubscribeFOP( _RealtimeQuoteAtomPtr& r );
	bool					SubscribeFX (  _RealtimeQuoteAtomPtr& r);


	template<typename T>
		void					OnResponse(HeapMessage &response, T& Params, RequestIdPtr pRequestID, bool bIsComplete, bool IsValid);

	template<typename _Request>
		CActivFeedAppication*	ApplyResponseToQUI( _Request &response, _QuoteInfo * p, 
		bool bRealTime = false, 
		_QuoteRequest *req = NULL, 
		ResponseBlock::Status* prbStatus = NULL);

	void					ApplyMPResponseToQUI( ResponseBlock& response, _QuoteInfo * p, bool bRealTime,  _QuoteRequest *pRequest);
	void					ApplyUpdateToQUI( const FieldListValidator::Field& fld, _QuoteInfo *p , _QuoteRequest *pRequest);
	//	void					ApplyMEResponseToQUI( ResponseBlock& rb, _QuoteInfo *p );

private:
	void					ParseResponseBlock(const ResponseBlock& rb, _QuoteInfo * pQuoteInfo, bool bRealTime, _QuoteRequest *pRequest = NULL);


private:
	/* ActiveFeedPriceProvider private data*/
	bool										m_bReconnected;
	CLock										m_csPSMap;			// Pending subscription map lock
	CPSMap										m_PSMap;			// Pending subscriptions
	CLock										m_csLU;				// Last quotes update map locker
	CLUMap										m_LU;				// Last quotes update map
	CLock										m_csSMap;			// Subscription map locker
	CSMap										m_SMap;				// Subscription map
	volatile long								m_UpdateCount;		// Count of updated quotes in map
	SYSTEMTIME									m_MarketOpenTime;
	bool										m_bFilterOffHourUpdates;
	/* end of ActiveFeedPriceProvider private data*/

	bool										m_bDisconnection;
	CHandle										m_hOnDisconnect;

	void OnIndex(CRequest<StockParams>::CRequestPtr& spRequest, HeapMessage &response);
	void OnOption(CRequest<OptionParams>::CRequestPtr& spRequest, HeapMessage &response, bool bIsLast);
	void OnFuturesOption(CRequest<FuturesOptionParams>::CRequestPtr& spRequest, HeapMessage &response, bool bIsLast);
	void OnFuture(CRequest<FutureParams>::CRequestPtr& spRequest, HeapMessage &response);
	void OnFutureByRoot(CRequest<FutureRootParams>::CRequestPtr& spRequest, HeapMessage &response, bool bIsLast);
	void OnFutureByRoot(CRequest<FutureRootParams>::CRequestPtr& spRequest, HeapMessage &response);
	StatusCode RequestOptionsByRoot(RequestIdPtr pRequestID);
	StatusCode RequestIndexOptions(RequestIdPtr pRequestID);
	StatusCode RequestFutureOptionsByRoot(RequestIdPtr pRequestID);
	StatusCode SaveOptionRoots(HeapMessage &response);
	StatusCode SaveIndexOptionRoots(HeapMessage &response);
	StatusCode SaveFutureOptionRoots(HeapMessage &response);

	bool                                        m_bActivated;
	TableNo						m_tableNumber;					///< The table number.
	std::string									m_symbol;						///< The symbol.
	FieldSpecificationList						m_fieldSpecificationList;		///< The field specification list.
	bool										m_isDelayed;					///< Indicates whether the updates should be delayed.
	bool										m_isExtendedTRationalDisplay;	///< Indicates whether the extended trended rational information should be displayed.
	size_t										m_maxReconnects;				///< The maximum number of reconnects.
	FieldListValidator	m_FieldListValidator;			///< The field list validator.
	RequestBlock		m_requestBlock;					///< The request block.
	std::string									m_lastSymbol;					///< The last symbol.
	size_t										m_nReconnects;					///< The number of reconnects.
	volatile long                               m_nextRequestId;  
	volatile long                               m_nextMultipleOptionRequestId;  
	CRequestsContainer							m_Container;
	CLock										m_csContainer;
	COptionRootInfoMap							m_optionRoots;
	map<string, long>							m_optionFutureRoots;
	CFutMap										m_futMap;

	GetMultipleEqual::RequestParameters			m_MultipleOptionsRequest;

	virtual StatusCode OnBreak(HeapMessage &breakMessage);
	virtual StatusCode OnDisconnect(HeapMessage &disconnectMessage);
	virtual StatusCode OnConnectFailed(HeapMessage &connectFailMessage);
	virtual StatusCode OnConnect(HeapMessage &connectMessage);
};

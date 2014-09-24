/**
 *  @file	ContentGatewayClient.h
 *
 *  @brief	Content gateway client.
 *
 *  $Log: $
 */

#if !defined (ACTIV_CONTENT_GATEWAY_CLIENT_H)
#define ACTIV_CONTENT_GATEWAY_CLIENT_H

#include "ActivContentPlatform/ActivContentGatewayApi/External.h"
#include "ActivContentPlatform/ActivContentGatewayApi/RealtimeRequestHelper.h"
#include "ActivContentPlatform/ActivContentGatewayApi/TimeSeriesRequestHelper.h"
#include "ActivContentPlatform/ActivContentGatewayApi/SymbolDirectoryRequestHelper.h"
#include "ActivContentPlatform/ActivContentGatewayApi/IRealtimeResponses.h"
#include "ActivContentPlatform/ActivContentGatewayApi/ITimeSeriesResponses.h"
#include "ActivContentPlatform/ActivContentGatewayApi/ISymbolDirectoryResponses.h"

namespace Activ
{

namespace ContentPlatform
{

/**
 *	@brief	Content gateway client class.
 */
class ContentGatewayClient : public Component, protected IRealtimeResponses, protected ITimeSeriesResponses, protected ISymbolDirectoryResponses
{
public:
	// Available realtime requests.
	typedef RealtimeRequestHelper<GATEWAY_REQUEST_GET_FIRST, TableNumberRequestParameters> GetFirst;
	typedef RealtimeRequestHelper<GATEWAY_REQUEST_GET_LAST, TableNumberRequestParameters> GetLast;
	typedef RealtimeRequestHelper<GATEWAY_REQUEST_GET_NEXT, SymbolIdCountRequestParameters> GetNext;
	typedef RealtimeRequestHelper<GATEWAY_REQUEST_GET_PREVIOUS, SymbolIdCountRequestParameters> GetPrevious;
	typedef RealtimeRequestHelper<GATEWAY_REQUEST_GET_EQUAL, SymbolIdRequestParameters> GetEqual;
	typedef RealtimeRequestHelper<GATEWAY_REQUEST_GET_MULTIPLE_EQUAL, SymbolIdListRequestParameters> GetMultipleEqual;
	typedef RealtimeRequestHelper<GATEWAY_REQUEST_GET_MULTIPLE_PATTERN_MATCH, PatternMatchRequestParameters> GetMultiplePatternMatch;
	typedef RealtimeRequestHelper<GATEWAY_REQUEST_GET_MULTIPLE_PATTERN_MATCH_LIST, PatternMatchListRequestParameters> GetMultiplePatternMatchList;
	typedef RealtimeRequestHelper<GATEWAY_REQUEST_GET_MATCH, MatchRequestParameters> GetMatch;

	// Available time series requests.
	typedef TimeSeriesRequestHelper<GATEWAY_REQUEST_GET_FIRST_HISTORY, GATEWAY_REQUEST_GET_NEXT_HISTORY, HistoryBarList> GetHistory;
	typedef TimeSeriesRequestHelper<GATEWAY_REQUEST_GET_FIRST_INTRADAY, GATEWAY_REQUEST_GET_NEXT_INTRADAY, IntradayBarList> GetIntraday;
	typedef TimeSeriesRequestHelper<GATEWAY_REQUEST_GET_FIRST_TICKS, GATEWAY_REQUEST_GET_NEXT_TICKS, TickList> GetTicks;

	// Available symbol directory requests.
	typedef SymbolDirectoryRequestHelper GetSymbols;

	/**
	 *	@brief	List of client states.
	 */
	#define ACTIV_CONTENT_GATEWAY_CLIENT_STATE_LIST(d)	\
		d(STATE_INITIALIZED, "initialized")				\
		d(STATE_W4_CONNECT, "w4 connect")				\
		d(STATE_LOGGED_ON, "logged on")					\
		d(STATE_W4_DISCONNECT, "w4 disconnect")

	/**
	 *	@brief	Enumeration of client states.
	 */
	enum State
	{
		ACTIV_CONTENT_GATEWAY_CLIENT_STATE_LIST(ACTIV_DECLARE_ELEMENT_1_WITH_COMMA)
	};

	/**
	 *	@brief	Settings for ContentGatewayClient class.
	 */
	class Settings
	{
	public:
		#define ACTIV_CONTENT_GATEWAY_CLIENT_DEFAULT_HEAP_SIZE	{ { 100, 100000 }, { 500, 20000 }, { 1000, 10000 }, { 25000, 500 } }

		/**
		 *	@brief	Default constructor.
		 *
		 *	Default settings are:
		 *		m_createMutex true
		 *		m_heapBucketInfoVector see ACTIV_CONTENT_GATEWAY_CLIENT_DEFAULT_HEAP_SIZE
		 *		m_pHeap 0 (create a heap of size m_heapBucketInfoVector). Set to non 0 to override.
		 */
		ACTIV_CONTENT_GATEWAY_API Settings();

		bool					m_createMutex;					///< Client methods to be called from multiple threads
		IHeap::BucketInfoVector	m_heapBucketInfoVector;			///< Size of heap to use for client
		IHeap *					m_pHeap;						///< Heap to use for messaging
	};

	/**
	 *	@brief	Parameters for Connect().
	 */
	class ConnectParameters
	{
	public:
		/**
		 *	@brief	Possible actions to take if connection to gateway is down.
		 */
		enum Policy
		{
			POLICY_RETRY,				///< Attempt to reconnect to same gateway
			POLICY_FIND_ALTERNATIVE,	///< Find an alternative gateway and attempt connect
		};

		/**
		 *	@brief	Default constructor.
		 */
		ACTIV_CONTENT_GATEWAY_API ConnectParameters();

		static const uint16_t DEFAULT_FAILOVER_SECONDS = 15;

		ServiceId	m_serviceId;		///< Id of service to connect to
		std::string	m_url;				///< Url of service
		std::string	m_userId;			///< User id
		std::string	m_password;			///< Password
		Policy		m_policy;			///< Action to take on a connection break
		uint16_t	m_failoverSeconds;	///< Find alternative server after this number of seconds if connection down
	};

	/**
	 *	@brief	Constructor.
	 *
	 *	@param	application reference to an application.
	 *	@param	managedEndPoint endpoint to attach to.
	 *	@param	settings settings.
	 *
	 *	@throw	Exception.
	 */
	ACTIV_CONTENT_GATEWAY_API ContentGatewayClient(Application &application, ManagedEndPoint &managedEndPoint, const Settings &settings = Settings());

	/**
	 *	@brief	Destructor.
	 */
	ACTIV_CONTENT_GATEWAY_API virtual ~ContentGatewayClient();

	/**
	 *	@brief	Get connection state.
	 *
	 *	@return	Connection state.
	 */
	ACTIV_CONTENT_GATEWAY_API State GetState() const;

	/**
	 *	@brief	Convert client state to a string representation.
	 *
	 *	@param	state a client state.
	 *
	 *	@return	String representation of \a state.
	 */
	ACTIV_CONTENT_GATEWAY_API static std::string StateToString(const State state);

	/**
	 *	@brief	Return the url of the service we are connected, or connecting, to.
	 *
	 *	@return	The service url.
	 */
	ACTIV_CONTENT_GATEWAY_API std::string GetServiceLocation() const;

	/**
	 *	@brief	Connect to a content gateway.
	 *
	 *	@param	connectParameters connection parameters.
	 *	@param	timeoutPeriod timeout in ms. A timeout of 0 implies an asynchronous connect.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_WRONG_STATE client must be in state STATE_INITIALIZED in order to connect
	 *	@retval	STATUS_CODE_INVALID_USER unknown user id
	 *	@retval	STATUS_CODE_INVALID_PASSWORD incorrect password for user id
	 *	@retval	STATUS_CODE_IN_USE provided user id is already logged on
	 */
	ACTIV_CONTENT_GATEWAY_API StatusCode Connect(const ConnectParameters &connectParameters, const TimeoutPeriod timeoutPeriod = Port::defaultTimeout);

	/**
	 *	@brief	Disconnect from a content gateway.
	 *
	 *	A disconnect is always asynchronous, as data from the current connection may be queued up on
	 *	the endpoint. An OnDisconnect() callback will be invoked when the connection is fully cleaned up, UNLESS
	 *	the connection has broken already, in which case an OnBreak() will be invoked, and not OnDisconnect().
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_NOT_CONNECTED
	 */
	ACTIV_CONTENT_GATEWAY_API StatusCode Disconnect();

	/**
	 *	@brief	Subscribe to updates on the feed.
	 *
	 *	If a previous SubscribeToFeed() had been made, the event type filter will be overwritten with the new parameters.
	 *
	 *	@param	subscribeFeedParameters subscription parameters.
	 *	@param	subscriptionCookie cookie returned on success that can be passed to Unsubscribe().
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_NOT_CONNECTED
	 *	@retval	...
	 */
	ACTIV_CONTENT_GATEWAY_API StatusCode SubscribeToFeed(const SubscribeFeedParameters &subscribeFeedParameters, SubscriptionCookie &subscriptionCookie) const;

	/**
	 *	@brief	Subscribe to updates to a table.
	 *
	 *	@param	subscribeTableParameters subscription parameters.
	 *	@param	subscriptionCookie cookie returned on success that can be passed to Unsubscribe().
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_NOT_CONNECTED
	 *	@retval	...
	 */
	ACTIV_CONTENT_GATEWAY_API StatusCode SubscribeToTable(const SubscribeTableParameters &subscribeTableParameters, SubscriptionCookie &subscriptionCookie) const;

	/**
	 *	@brief	Unsubscribe from the results of all requests. Feed level subscription is not affected.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_NOT_CONNECTED
	 *	@retval	...
	 */
	ACTIV_CONTENT_GATEWAY_API StatusCode Unsubscribe() const;

	/**
	 *	@brief	Unsubscribe from the results of a request.
	 *
	 *	All target symbols from the request corresponding to \subscriptionCookie are unsubscribed.
	 *
	 *	@param	subscriptionCookie SubscriptionCookie as returned by a realtime response.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_NOT_CONNECTED
	 *	@retval	...
	 */
	ACTIV_CONTENT_GATEWAY_API StatusCode Unsubscribe(const SubscriptionCookie subscriptionCookie) const;

	/**
	 *	@brief	Unsubscribe from the results of a request.
	 *
	 *	Only target symbols from the request corresponding to \subscriptionCookie where the source symbols
	 *	are \a sourceSymbolIdList are unsubscribed.
	 *
	 *	@param	subscriptionCookie SubscriptionCookie as returned by a realtime response.
	 *	@param	sourceSymbolIdList source symbol list.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_NOT_CONNECTED
	 *	@retval	...
	 */
	ACTIV_CONTENT_GATEWAY_API StatusCode Unsubscribe(const SubscriptionCookie subscriptionCookie, const SymbolIdList &sourceSymbolIdList) const;

	/**
	 *	@brief	Unsubscribe from the results of multiple requests.
	 *
	 *	All target symbols from the requests corresponding to \subscriptionCookieList are unsubscribed.
	 *
	 *	@param	subscriptionCookieList a list of SubscriptionCookie as returned by a realtime responses.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_NOT_CONNECTED
	 *	@retval	...
	 */
	ACTIV_CONTENT_GATEWAY_API StatusCode Unsubscribe(const SubscriptionCookieList &subscriptionCookieList) const;

	/**
	 *	@brief	Get info about current subscriptions at a particular permission level.
	 *
	 *	@param	permissionLevel the permission level to get subscription info at.
	 *	@param	subscriptionInfoList returned list of subscriptionInfo.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_NOT_CONNECTED
	 *	@retval	...
	 */
	ACTIV_CONTENT_GATEWAY_API StatusCode GetSubscriptionInfo(const Feed::PermissionLevel permissionLevel, SubscriptionInfoList &subscriptionInfoList) const;

	/**
	 *	@brief	Check if received response message is valid (complete or partially complete).
	 *
	 *	@param	response HeapMessage containing a serialized response.
	 *
	 *	@return	true if this is a valid response.
	 */
	ACTIV_CONTENT_GATEWAY_API static bool IsValidResponse(const HeapMessage &response);

	/**
	 *	@brief	Check if received response message is complete.
	 *
	 *	@param	response HeapMessage containing a serialized response.
	 *
	 *	@return	true if this is a complete response.
	 */
	ACTIV_CONTENT_GATEWAY_API static bool IsCompleteResponse(const HeapMessage &response);

	ACTIV_DECLARE_NESTED_CLASS(Private);						///< Private methods

protected:
	// IRealtimeResponses overrides
	ACTIV_CONTENT_GATEWAY_API virtual void OnRecordUpdate(HeapMessage &update);
	ACTIV_CONTENT_GATEWAY_API virtual void OnGetFirstResponse(HeapMessage &response);
	ACTIV_CONTENT_GATEWAY_API virtual void OnGetLastResponse(HeapMessage &response);
	ACTIV_CONTENT_GATEWAY_API virtual void OnGetNextResponse(HeapMessage &response);
	ACTIV_CONTENT_GATEWAY_API virtual void OnGetPreviousResponse(HeapMessage &response);
	ACTIV_CONTENT_GATEWAY_API virtual void OnGetEqualResponse(HeapMessage &response);
	ACTIV_CONTENT_GATEWAY_API virtual void OnGetMultipleEqualResponse(HeapMessage &response);
	ACTIV_CONTENT_GATEWAY_API virtual void OnGetMultiplePatternMatchResponse(HeapMessage &response);
	ACTIV_CONTENT_GATEWAY_API virtual void OnGetMultiplePatternMatchListResponse(HeapMessage &response);
	ACTIV_CONTENT_GATEWAY_API virtual void OnGetMatchResponse(HeapMessage &response);

	// ITimeSeriesResponses overrides
	ACTIV_CONTENT_GATEWAY_API virtual void OnGetFirstHistoryResponse(HeapMessage &response);
	ACTIV_CONTENT_GATEWAY_API virtual void OnGetNextHistoryResponse(HeapMessage &response);
	ACTIV_CONTENT_GATEWAY_API virtual void OnGetFirstIntradayResponse(HeapMessage &response);
	ACTIV_CONTENT_GATEWAY_API virtual void OnGetNextIntradayResponse(HeapMessage &response);
	ACTIV_CONTENT_GATEWAY_API virtual void OnGetFirstTicksResponse(HeapMessage &response);
	ACTIV_CONTENT_GATEWAY_API virtual void OnGetNextTicksResponse(HeapMessage &response);

	// ISymbolDirectoryResponses overrides
	ACTIV_CONTENT_GATEWAY_API virtual void OnGetSymbolsResponse(HeapMessage &response);

	// Component overrides
	ACTIV_CONTENT_GATEWAY_API virtual StatusCode OnConnect(HeapMessage &connectMessage);
	ACTIV_CONTENT_GATEWAY_API virtual StatusCode OnConnectFailed(HeapMessage &connectFailMessage);
	ACTIV_CONTENT_GATEWAY_API virtual StatusCode OnBreak(HeapMessage &breakMessage);
	ACTIV_CONTENT_GATEWAY_API virtual StatusCode OnDisconnect(HeapMessage &disconnectMessage);
	ACTIV_CONTENT_GATEWAY_API virtual StatusCode OnTimer(const Address &timerAddress, const BaseTime &time);
	ACTIV_CONTENT_GATEWAY_API virtual StatusCode OnResponse(HeapMessage &response);
	ACTIV_CONTENT_GATEWAY_API virtual StatusCode OnMessage(HeapMessage &message);

	/**
	 *	@brief	Callback on report message from gateway.
	 *
	 *	@param	reportMessage message from gateway.
	 */
	ACTIV_CONTENT_GATEWAY_API virtual void OnReportMessage(const std::string &reportMessage);

private:
	std::auto_ptr<Private>	m_pPrivate;
};

// ---------------------------------------------------------------------------------------------------------------------------------

inline bool ContentGatewayClient::IsValidResponse(const HeapMessage &response)
{
	const StatusCode statusCode = response.GetStatusCode();

	return ((STATUS_CODE_SUCCESS == statusCode) || (STATUS_CODE_PENDING == statusCode));
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline bool ContentGatewayClient::IsCompleteResponse(const HeapMessage &response)
{
	// won't get anymore responses after STATUS_CODE_SUCCESS, or any error
	return (STATUS_CODE_PENDING != response.GetStatusCode());
}

} // namespace ContentPlatform

} // namespace Activ

#endif // !defined (ACTIV_CONTENT_GATEWAY_CLIENT_H)

/**
 *  @file	TimeSeriesRequestHelper.h
 *
 *  @brief	Helper class for time series requests.
 *
 *  $Log: $
 */

#if (!defined ACTIV_FEED_API_TIME_SERIES_REQUEST_HELPER_H)
#define ACTIV_FEED_API_TIME_SERIES_REQUEST_HELPER_H

#include "ActivContentPlatform/ActivFeedApi/External.h"

namespace Activ
{

namespace ContentPlatform
{

namespace FeedApi
{

class ContentGatewayClient;

/**
 *	@brief	Time series helper class.
 *
 *	@param	FIRST_MESSAGE_TYPE_VALUE type of message for get first requests.
 *	@param	NEXT_MESSAGE_TYPE_VALUE type of message for get next requests.
 *	@param	FIELD_SET_TYPE type of data returned.
 */
template<MessageType FIRST_MESSAGE_TYPE_VALUE, MessageType NEXT_MESSAGE_TYPE_VALUE, typename FIELD_SET_TYPE>
class TimeSeriesRequestHelper
{
public:
	typedef TimeSeriesRequestParameters RequestParameters;
	typedef TimeSeriesResponseParametersTemplate<FIELD_SET_TYPE> ResponseParameters;

	/**
	 *	@brief	Get time series synchronously.
	 *
	 *	@param	client reference to ContentGatewayClient.
	 *	@param	requestParameters parameters describing the request.
	 *	@param	cookie reference to a TimeSeriesCookie object to receive the cookie that can be used in a GetNext().
	 *	@param	responseParameters to receive the returned data.
	 *	@param	timeoutPeriod timeout in ms.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_NOT_CONNECTED
	 *	@retval	STATUS_CODE_TIMEOUT
	 *	@retval	...
	 */
	ACTIV_FEED_API static StatusCode SendGetFirstRequest(const ContentGatewayClient &client, const RequestParameters &requestParameters, TimeSeriesCookie &cookie, ResponseParameters &responseParameters, const TimeoutPeriod timeoutPeriod = Port::defaultTimeout);

	/**
	 *	@brief	Get time series asynchronously.
	 *
	 *	@param	client reference to ContentGatewayClient.
	 *	@param	requestParameters parameters describing the request.
	 *	@param	cookie reference to a TimeSeriesCookie object to receive the cookie that can be used in a GetNext().
	 *	@param	responseParameters to receive the returned data.
	 *	@param	timeoutPeriod timeout in ms.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_NOT_CONNECTED
	 *	@retval	...
	 */
	ACTIV_FEED_API static StatusCode PostGetFirstRequest(const ContentGatewayClient &client, const RequestId &requestId, const RequestParameters &requestParameters);

	/**
	 *	@brief	Get next time series synchronously.
	 *
	 *	@param	client reference to ContentGatewayClient.
	 *	@param	cookie reference to a TimeSeriesCookie object.
	 *	@param	responseParameters to receive the returned data.
	 *	@param	timeoutPeriod timeout in ms.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_NOT_CONNECTED
	 *	@retval	STATUS_CODE_TIMEOUT
	 *	@retval	...
	 */
	ACTIV_FEED_API static StatusCode SendGetNextRequest(const ContentGatewayClient &client, TimeSeriesCookie &cookie, ResponseParameters &responseParameters, const TimeoutPeriod timeoutPeriod = Port::defaultTimeout);

	/**
	 *	@brief	Get next time series asynchronously.
	 *
	 *	@param	client reference to ContentGatewayClient.
	 *	@param	cookie reference to a TimeSeriesCookie from a previous time series request.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_NOT_CONNECTED
	 *	@retval	...
	 */
	ACTIV_FEED_API static StatusCode PostGetNextRequest(const ContentGatewayClient &client, const RequestId &requestId, const TimeSeriesCookie &cookie);

	/**
	 *	@brief	Deserialize a time series response.
	 *
	 *	@param	client reference to ContentGatewayClient.
	 *	@param	response HeapMessage as passed to ContentGatewayClient::On*Response() or returned by
	 *			SyncRequestId::GetResponse().
	 *	@param	cookie reference to a TimeSeriesCookie object to receive deserialized cookie.
	 *	@param	responseParameters reference to a ResponseParameters object to receive deserialized response.
	 *
	 *	@retval	STATUS_CODE_SUCCESS response successfully deserialized
	 *	@retval	STATUS_CODE_UNRECOGNIZED not a response of the correct type
	 *	@retval	... failed to deserialize response
	 */
	ACTIV_FEED_API static StatusCode Deserialize(const ContentGatewayClient &client, const HeapMessage &response, TimeSeriesCookie &cookie, ResponseParameters &responseParameters);

private:
	static const MessageType FIRST_MESSAGE_TYPE = FIRST_MESSAGE_TYPE_VALUE;	///< Value of get first request message type.
	static const MessageType NEXT_MESSAGE_TYPE = NEXT_MESSAGE_TYPE_VALUE;	///< Value of get next request message type.
};

} // namespace FeedApi

} // namespace ContentPlatform

} // namespace Activ

#endif // (!defined ACTIV_FEED_API_TIME_SERIES_REQUEST_HELPER_H)

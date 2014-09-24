/**
 *  @file	RealtimeRequestHelper.h
 *
 *  @brief	Helper class for realtime requests.
 *
 *  $Log: $
 */

#if !defined (ACTIV_FEED_API_REALTIME_REQUEST_HELPER_H)
#define ACTIV_FEED_API_REALTIME_REQUEST_HELPER_H

#include "ActivContentPlatform/ActivContentGatewayApi/External.h"

namespace Activ
{

namespace ContentPlatform
{

class ContentGatewayClient;

/**
 *	@brief	Realtime request helper class.
 *
 *	@param	MESSAGE_TYPE_VALUE type of message for requests.
 *	@param	REQUEST_PARAMETERS type of request parameters.
 */
template<MessageType MESSAGE_TYPE_VALUE, typename REQUEST_PARAMETERS>
class RealtimeRequestHelper
{
public:
	typedef REQUEST_PARAMETERS RequestParameters;				///< Request paramters type.
	typedef RealtimeResponseParameters ResponseParameters;		///< Response parameters type.

	/**
	 *	@brief	Get items synchronously.
	 *
	 *	@param	client reference to ContentGatewayClient.
	 *	@param	requestParameters parameters describing the request.
	 *	@param	responseParameters to receive the returned data.
	 *	@param	timeoutPeriod timeout in ms.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_NOT_CONNECTED
	 *	@retval	STATUS_CODE_TIMEOUT
	 *	@retval	...
	 */
	ACTIV_CONTENT_GATEWAY_API static StatusCode SendRequest(const ContentGatewayClient &client, const RequestParameters &requestParameters, ResponseParameters &responseParameters, const TimeoutPeriod timeoutPeriod = Port::defaultTimeout);

	/**
	 *	@brief	Get items synchronously without blocking.
	 *
	 *	@param	client reference to ContentGatewayClient.
	 *	@param	syncRequestId reference to a SyncRequestId object that can be used to poll for responses after this call.
	 *	@param	requestParameters parameters describing the request.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_NOT_CONNECTED
	 *	@retval	...
	 */
	ACTIV_CONTENT_GATEWAY_API static StatusCode SendRequest(const ContentGatewayClient &client, SyncRequestId &syncRequestId, const RequestParameters &requestParameters);

	/**
	 *	@brief	Get items asynchronously.
	 *
	 *	@param	client reference to ContentGatewayClient.
	 *	@param	requestId request id.
	 *	@param	requestParameters parameters describing the request.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_NOT_CONNECTED
	 *	@retval	...
	 */
	ACTIV_CONTENT_GATEWAY_API static StatusCode PostRequest(const ContentGatewayClient &client, const RequestId &requestId, const RequestParameters &requestParameters);

	/**
	 *	@brief	Wait or poll for a response message from a sync request id object.
	 *
	 *	@param	client reference to ContentGatewayClient.
	 *	@param	syncRequestId SyncRequestId object as returned by a successful \a Send() call.
	 *	@param	responseParameters to receive the returned data.
	 *
	 *	@retval	STATUS_CODE_SUCCESS responseParameters contains the deserialized data and there is no more to follow
	 *	@retval	STATUS_CODE_PENDING responseParameters contains the deserialized data and there more data to follow
	 *	@retval	STATUS_CODE_TIMEOUT
	 *	@retval	...
	 */
	ACTIV_CONTENT_GATEWAY_API static StatusCode GetResponse(const ContentGatewayClient &client, SyncRequestId &syncRequestId, ResponseParameters &responseParameters, const TimeoutPeriod timeoutPeriod = Port::defaultTimeout);

	/**
	 *	@brief	Deserialize a response.
	 *
	 *	This function should only be called if the received response is valid. This can be determined using
	 *	ContentGatewayClient::IsValidResponse(). Note the response block list is not cleared down by this function,
	 *	so it is possible to build a complete ResponseBlockList from multiple partial responses.
	 *
	 *	@param	client reference to ContentGatewayClient.
	 *	@param	response HeapMessage as passed to ContentGatewayClient::On*Response() or returned by
	 *			SyncRequestId::GetResponse().
	 *	@param	responseParameters reference to a ResponseParameters object to receive deserialized response blocks.
	 *
	 *	@retval	STATUS_CODE_SUCCESS response successfully deserialized.
	 *	@retval	STATUS_CODE_UNRECOGNIZED not a MESSAGE_TYPE response message.
	 *	@retval	... failed to deserialize response.
	 */
	ACTIV_CONTENT_GATEWAY_API static StatusCode Deserialize(const ContentGatewayClient &client, const HeapMessage &response, ResponseParameters &responseParameters);

private:
	static const MessageType MESSAGE_TYPE = MESSAGE_TYPE_VALUE;	///< Message type of this request type.
};

} // namespace ContentPlatform

} // namespace Activ

#endif // !defined (ACTIV_FEED_API_REALTIME_REQUEST_HELPER_H)

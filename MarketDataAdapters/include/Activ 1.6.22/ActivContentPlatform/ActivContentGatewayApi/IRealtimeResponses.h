/**
 *  @file	IRealtimeResponses.h
 *
 *  @brief	Callback interface for realtime data request responses.
 *
 *  $Log: $
 */

#if !defined (ACTIV_FEED_API_IREALTIME_RESPONSES_H)
#define ACTIV_FEED_API_IREALTIME_RESPONSES_H

#include "ActivContentPlatform/ActivContentGatewayApi/External.h"

namespace Activ
{

namespace ContentPlatform
{

/**
 *	@brief	Interface for realtime data request callbacks.
 */
class IRealtimeResponses
{
public:
	/**
	 *	@brief	Destructor.
	 */
	virtual ~IRealtimeResponses() = 0;

	/**
	 *	@brief	Called on receiving a record update.
	 *
	 *	The message can be deserialized using RecordUpdateHelper::Deserialize().
	 *
	 *	@param	update HeapMessage containing the serialized update message.
	 */
	virtual void OnRecordUpdate(HeapMessage &update) = 0;

	/**
	 *	@brief	Called on receiving an asynchronous GetFirst::PostRequest() response.
	 *
	 *	The response can be deserialized using GetFirst::Deserialize() if it is valid.
	 *	The validity of the response can be determined using GatewayClient::IsValidResponse().
	 *	If this is the final (or only) part of a response, GatewayClient::IsCompleteResponse() will return true.
	 *
	 *	@param	HeapMessage containing the serialized response.
	 */
	virtual void OnGetFirstResponse(HeapMessage &response) = 0;

	/**
	 *	@brief	Called on receiving an asynchronous GetLast::PostRequest() response.
	 *
	 *	The response can be deserialized using GetLast::Deserialize() if it is valid.
	 *	The validity of the response can be determined using GatewayClient::IsValidResponse().
	 *	If this is the final (or only) part of a response, GatewayClient::IsCompleteResponse() will return true.
	 *
	 *	@param	HeapMessage containing the serialized response.
	 */
	virtual void OnGetLastResponse(HeapMessage &response) = 0;

	/**
	 *	@brief	Called on receiving an asynchronous GetNext::PostRequest() response.
	 *
	 *	The response can be deserialized using GetNext::Deserialize() if it is valid.
	 *	The validity of the response can be determined using GatewayClient::IsValidResponse().
	 *	If this is the final (or only) part of a response, GatewayClient::IsCompleteResponse() will return true.
	 *
	 *	@param	HeapMessage containing the serialized response.
	 */
	virtual void OnGetNextResponse(HeapMessage &response) = 0;

	/**
	 *	@brief	Called on receiving an asynchronous GetPrevious::PostRequest() response.
	 *
	 *	The response can be deserialized using GetPrevious::Deserialize() if it is valid.
	 *	The validity of the response can be determined using GatewayClient::IsValidResponse().
	 *	If this is the final (or only) part of a response, GatewayClient::IsCompleteResponse() will return true.
	 *
	 *	@param	HeapMessage containing the serialized response.
	 */
	virtual void OnGetPreviousResponse(HeapMessage &response) = 0;

	/**
	 *	@brief	Called on receiving an asynchronous GetEqual::PostRequest() response.
	 *
	 *	The response can be deserialized using GetEqual::Deserialize() if it is valid.
	 *	The validity of the response can be determined using GatewayClient::IsValidResponse().
	 *	If this is the final (or only) part of a response, GatewayClient::IsCompleteResponse() will return true.
	 *
	 *	@param	HeapMessage containing the serialized response.
	 */
	virtual void OnGetEqualResponse(HeapMessage &response) = 0;

	/**
	 *	@brief	Called on receiving an asynchronous GetMultipleEqual::PostRequest() response.
	 *
	 *	The response can be deserialized using GetMultipleEqual::Deserialize() if it is valid.
	 *	The validity of the response can be determined using GatewayClient::IsValidResponse().
	 *	If this is the final (or only) part of a response, GatewayClient::IsCompleteResponse() will return true.
	 *
	 *	@param	HeapMessage containing the serialized response.
	 */
	virtual void OnGetMultipleEqualResponse(HeapMessage &response) = 0;

	/**
	 *	@brief	Called on receiving an asynchronous GetMultiplePatternMatch::PostRequest() response.
	 *
	 *	The response can be deserialized using GetMultiplePatternMatch::Deserialize() if it is valid.
	 *	The validity of the response can be determined using GatewayClient::IsValidResponse().
	 *	If this is the final (or only) part of a response, GatewayClient::IsCompleteResponse() will return true.
	 *
	 *	@param	HeapMessage containing the serialized response.
	 */
	virtual void OnGetMultiplePatternMatchResponse(HeapMessage &response) = 0;

	/**
	 *	@brief	Called on receiving an asynchronous GetMultiplePatternMatchList::PostRequest() response.
	 *
	 *	The response can be deserialized using GetMultiplePatternMatchList::Deserialize() if it is valid.
	 *	The validity of the response can be determined using GatewayClient::IsValidResponse().
	 *	If this is the final (or only) part of a response, GatewayClient::IsCompleteResponse() will return true.
	 *
	 *	@param	HeapMessage containing the serialized response.
	 */
	virtual void OnGetMultiplePatternMatchListResponse(HeapMessage &response) = 0;

	/**
	 *	@brief	Called on receiving an asynchronous GetMatch::PostRequest() response.
	 *
	 *	The response can be deserialized using GetMatch::Deserialize() if it is valid.
	 *	The validity of the response can be determined using GatewayClient::IsValidResponse().
	 *	If this is the final (or only) part of a response, GatewayClient::IsCompleteResponse() will return true.
	 *
	 *	@param	HeapMessage containing the serialized response.
	 */
	virtual void OnGetMatchResponse(HeapMessage &response) = 0;

	/**
	 *	@brief	Called on receiving an asynchronous GetMultipleMatch::PostRequest() response.
	 *
	 *	The response can be deserialized using GetMultipleMatch::Deserialize() if it is valid.
	 *	The validity of the response can be determined using GatewayClient::IsValidResponse().
	 *	If this is the final (or only) part of a response, GatewayClient::IsCompleteResponse() will return true.
	 *
	 *	@param	HeapMessage containing the serialized response.
	 */
	virtual void OnGetMultipleMatchResponse(HeapMessage &response) = 0;
};

// ---------------------------------------------------------------------------------------------------------------------------------

inline IRealtimeResponses::~IRealtimeResponses()
{
}

} // namespace ContentPlatform

} // namespace Activ

#endif // !defined (ACTIV_FEED_API_IREALTIME_RESPONSES_H)

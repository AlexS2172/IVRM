/**
 *  @file	INewsResponses.h
 *
 *  @brief	Callback interface for news responses.
 *
 *	$Log: $
 *
 */

#if (!defined ACTIV_FEED_API_INEWS_RESPONSES_H)
#define ACTIV_FEED_API_INEWS_RESPONSES_H

#include "ActivContentPlatform/ActivFeedApi/External.h"

namespace Activ
{

namespace ContentPlatform
{

namespace FeedApi
{

/**
 *	@brief	Interface for news response callbacks.
 */
class INewsResponses
{
public:
	/**
	 *	@brief	Destructor.
	 */
	virtual ~INewsResponses() = 0;

	/**
	 *	@brief	Called on receiving an asynchronous GetNewsStories::PostRequest() response.
	 *
	 *	The response can be deserialized using GetNewsStories::Deserialize() if it is valid.
	 *	The validity of the response can be determined using ContentGatewayClient::IsValidResponse().
	 *
	 *	@param	response HeapMessage containing the serialized response.
	 */
	virtual void OnGetNewsStoriesResponse(HeapMessage &response) = 0;


	/**
	 *	@brief	Called on receiving a news update.
	 *
	 *	The message can be deserialized using NewsUpdateHelper::Deserialize().
	 *
	 *	@param	update HeapMessage containing the serialized update message.
	 */
	virtual void OnNewsUpdate(HeapMessage &update) = 0;
};

// ---------------------------------------------------------------------------------------------------------------------------------

inline INewsResponses::~INewsResponses()
{
}

} // namespace FeedApi

} // namespace ContentPlatform

} // namespace Activ

#endif // (!defined ACTIV_FEED_API_INEWS_SERVER_RESPONSES_H)

/**
 *  @file	NewsUpdateHelper.h
 *
 *  @brief	News update helper class.
 *
 *  $Log: $
 */

#if (!defined ACTIV_FEED_API_NEWS_UPDATE_HELPER_H)
#define ACTIV_FEED_API_NEWS_UPDATE_HELPER_H

#include "ActivContentPlatform/ActivFeedApi/External.h"

namespace Activ
{

namespace ContentPlatform
{

namespace FeedApi
{

class ContentGatewayClient;

/**
 *	@brief	News update helper class.
 */
class NewsUpdateHelper
{
public:
	/**
	 *	@brief	Deserialize a news update.
	 *
	 *	@param	client reference to ContentGatewayClient.
	 *	@param	update HeapMessage containing record update.
	 *	@param	newsUpdate reference to a NewsUpdate object to receive deserialized data.
	 *
	 *	@retval	STATUS_CODE_SUCCESS response successfully deserialized.
	 *	@retval	STATUS_CODE_UNRECOGNIZED not a record update message.
	 *	@retval	... failed to deserialize response.
	 */
	ACTIV_FEED_API static StatusCode Deserialize(const ContentGatewayClient &client, const HeapMessage &update, NewsUpdate &newsUpdate);
};

} // namespace FeedApi

} // namespace ContentPlatform

} // namespace Activ

#endif // (!defined ACTIV_FEED_API_NEWS_UPDATE_HELPER_H)

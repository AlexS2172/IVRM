/**
 *  @file	RecordUpdateHelper.h
 *
 *  @brief	Record update helper class.
 *
 *  $Log: $
 */

#if (!defined ACTIV_FEED_API_RECORD_UPDATE_HELPER_H)
#define ACTIV_FEED_API_RECORD_UPDATE_HELPER_H

#include "ActivContentPlatform/ActivFeedApi/External.h"

namespace Activ
{

namespace ContentPlatform
{

namespace FeedApi
{

class ContentGatewayClient;

/**
 *	@brief	Record update helper class.
 */
class RecordUpdateHelper
{
public:
	/**
	 *	@brief	Deserialize a record update.
	 *
	 *	@param	client reference to ContentGatewayClient.
	 *	@param	update HeapMessage containing record update.
	 *	@param	recordUpdate reference to a RecordUpdate object to receive deserialized data.
	 *
	 *	@retval	STATUS_CODE_SUCCESS response successfully deserialized.
	 *	@retval	STATUS_CODE_UNRECOGNIZED not a record update message.
	 *	@retval	... failed to deserialize response.
	 */
	ACTIV_FEED_API static StatusCode Deserialize(const ContentGatewayClient &client, const HeapMessage &update, RecordUpdate &recordUpdate);
};

} // namespace FeedApi

} // namespace ContentPlatform

} // namespace Activ

#endif // (!defined ACTIV_FEED_API_RECORD_UPDATE_HELPER_H)

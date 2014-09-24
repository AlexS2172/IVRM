/**
 *  @file	ISymbolDirectoryResponses.h
 *
 *  @brief	Callback interface for symbol directory request responses.
 *
 *  $Log: $
 */

#if (!defined ACTIV_FEED_API_ISYMBOL_DIRECTORY_RESPONSES_H)
#define ACTIV_FEED_API_ISYMBOL_DIRECTORY_RESPONSES_H

#include "ActivContentPlatform/ActivFeedApi/External.h"

namespace Activ
{

namespace ContentPlatform
{

namespace FeedApi
{

/**
 *	@brief	Interface for symbol directory response callbacks.
 */
class ISymbolDirectoryResponses
{
public:
	/**
	 *	@brief	Destructor.
	 */
	virtual ~ISymbolDirectoryResponses() = 0;

	/**
	 *	@brief	Called on receiving an asynchronous GetSymbols::PostRequest() response.
	 *
	 *	The response can be deserialized using GetSymbols::Deserialize() if it is valid.
	 *	The validity of the response can be determined using ContentGatewayClient::IsValidResponse().
	 *
	 *	@param	response HeapMessage containing the serialized response.
	 */
	virtual void OnGetSymbolsResponse(HeapMessage &response) = 0;
};

// ---------------------------------------------------------------------------------------------------------------------------------

inline ISymbolDirectoryResponses::~ISymbolDirectoryResponses()
{
}

} // namespace FeedApi

} // namespace ContentPlatform

} // namespace Activ

#endif // (!defined ACTIV_FEED_API_ISYMBOL_DIRECTORY_RESPONSES_H)

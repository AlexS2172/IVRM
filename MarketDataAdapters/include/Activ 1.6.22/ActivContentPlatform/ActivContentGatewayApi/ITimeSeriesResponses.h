/**
 *  @file	ITimeSeriesResponses.h
 *
 *  @brief	Callback interface for time series request responses.
 *
 *  $Log: $
 */

#if !defined (ACTIV_FEED_API_ITIME_SERIES_RESPONSES_H)
#define ACTIV_FEED_API_ITIME_SERIES_RESPONSES_H

#include "ActivContentPlatform/ActivContentGatewayApi/External.h"

namespace Activ
{

namespace ContentPlatform
{

/**
 *	@brief	Interface for time series request callbacks.
 */
class ITimeSeriesResponses
{
public:
	/**
	 *	@brief	Destructor.
	 */
	virtual ~ITimeSeriesResponses() = 0;

	/**
	 *	@brief	Called on receiving an asynchronous GetHistory::PostGetFirstRequest() response.
	 *
	 *	The response can be deserialized using GetHistory::Deserialize() if it is valid.
	 *	The validity of the response can be determined using GatewayClient::IsValidResponse().
	 *
	 *	@param	HeapMessage containing the serialized response.
	 */
	virtual void OnGetFirstHistoryResponse(HeapMessage &response) = 0;

	/**
	 *	@brief	Called on receiving an asynchronous GetHistory::PostGetNextRequest() response.
	 *
	 *	The response can be deserialized using GetHistory::Deserialize() if it is valid.
	 *	The validity of the response can be determined using GatewayClient::IsValidResponse().
	 *
	 *	@param	HeapMessage containing the serialized response.
	 */
	virtual void OnGetNextHistoryResponse(HeapMessage &response) = 0;

	/**
	 *	@brief	Called on receiving an asynchronous GetIntraday::PostGetFirstRequest() response.
	 *
	 *	The response can be deserialized using GetIntraday::Deserialize() if it is valid.
	 *	The validity of the response can be determined using GatewayClient::IsValidResponse().
	 *
	 *	@param	HeapMessage containing the serialized response.
	 */
	virtual void OnGetFirstIntradayResponse(HeapMessage &response) = 0;

	/**
	 *	@brief	Called on receiving an asynchronous GetIntraday::PostGetNextRequest() response.
	 *
	 *	The response can be deserialized using GetIntraday::Deserialize() if it is valid.
	 *	The validity of the response can be determined using GatewayClient::IsValidResponse().
	 *
	 *	@param	HeapMessage containing the serialized response.
	 */
	virtual void OnGetNextIntradayResponse(HeapMessage &response) = 0;

	/**
	 *	@brief	Called on receiving an asynchronous GetTicks::PostGetFirstRequest() response.
	 *
	 *	The response can be deserialized using GetTicks::Deserialize() if it is valid.
	 *	The validity of the response can be determined using GatewayClient::IsValidResponse().
	 *
	 *	@param	HeapMessage containing the serialized response.
	 */
	virtual void OnGetFirstTicksResponse(HeapMessage &response) = 0;

	/**
	 *	@brief	Called on receiving an asynchronous GetTicks::PostGetNextRequest() response.
	 *
	 *	The response can be deserialized using GetTicks::Deserialize() if it is valid.
	 *	The validity of the response can be determined using GatewayClient::IsValidResponse().
	 *
	 *	@param	HeapMessage containing the serialized response.
	 */
	virtual void OnGetNextTicksResponse(HeapMessage &response) = 0;
};

// ---------------------------------------------------------------------------------------------------------------------------------

inline ITimeSeriesResponses::~ITimeSeriesResponses()
{
}

} // namespace ContentPlatform

} // namespace Activ

#endif // !defined (ACTIV_FEED_API_ITIME_SERIES_RESPONSES_H)

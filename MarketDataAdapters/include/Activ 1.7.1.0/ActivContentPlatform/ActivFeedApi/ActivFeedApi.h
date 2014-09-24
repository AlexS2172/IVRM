/**
 *  @file	ActivFeedApi.h
 *
 *  @brief	ActivFeedApi main header.
 *
 *  $Log: $
 */

#if (!defined ACTIV_FEED_API_H)
#define ACTIV_FEED_API_H

/**
 *	@brief	ActivFeedApi version value.
 */
#define ACTIV_FEED_API_VERSION_VALUE	ACTIV_MAKE_VERSION_VALUE(1, 7, 1, 0)

/**
 *	@brief	ActivFeedApi version string.
 */
#define ACTIV_FEED_API_VERSION_STRING	"1.7.1.0"

#include "ActivContentPlatform/ActivFeedApi/ContentGatewayClient.h"
#include "ActivContentPlatform/ActivFeedApi/RecordUpdateHelper.h"
#include "ActivContentPlatform/ActivFeedApi/NewsUpdateHelper.h"
#include "ActivContentPlatform/ActivFeedApi/FieldListValidator.h"

#endif // (!defined ACTIV_FEED_API_H)

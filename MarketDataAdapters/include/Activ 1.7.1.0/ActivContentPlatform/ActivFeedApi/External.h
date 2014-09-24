/**
 *  @file	External.h
 *
 *	@brief	Import rules for ActivFeedApi exports.
 *
 *  $Log: $
 */

#if (!defined ACTIV_FEED_API_EXTERNAL_H)
#define ACTIV_FEED_API_EXTERNAL_H

#if (!defined ACTIV_FEED_API)
#	if (!defined ACTIV_STATIC)
#		define ACTIV_FEED_API ACTIV_IMPORT_DLL
#	else
#		define ACTIV_FEED_API ACTIV_IMPORT_LIB
#	endif // (!defined ACTIV_STATIC)
#endif // (!defined ACTIV_FEED_API)

#include "ActivMiddleware/ActivFramework/ActivFramework.h"

#include "ActivContentPlatform/ActivFeedApiCommon/RealtimeTypes.h"
#include "ActivContentPlatform/ActivFeedApiCommon/TimeSeriesTypes.h"
#include "ActivContentPlatform/ActivFeedApiCommon/SymbolDirectoryTypes.h"
#include "ActivContentPlatform/ActivFeedApiCommon/NewsTypes.h"
#include "ActivContentPlatform/ActivFeedApiCommon/Tick.h"
#include "ActivContentPlatform/ActivFeedApiCommon/IntradayBar.h"
#include "ActivContentPlatform/ActivFeedApiCommon/HistoryBar.h"
#include "ActivContentPlatform/ActivFeedCommon/UpdateIds.h"
#include "ActivContentPlatform/ActivFeedCommon/EventTypes.h"

#endif // (!defined ACTIV_FEED_API_EXTERNAL_H)

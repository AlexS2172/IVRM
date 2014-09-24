/**
 *  @file	External.h
 *
 *	@brief	Import rules for ActivContentGatewayApi exports.
 *
 *  $Log: $
 */

#if !defined (ACTIV_CONTENT_GATEWAY_API_EXTERNAL_H)
#define ACTIV_CONTENT_GATEWAY_API_EXTERNAL_H

#if !defined (ACTIV_CONTENT_GATEWAY_API)
#	if !defined (ACTIV_STATIC)
#		define ACTIV_CONTENT_GATEWAY_API ACTIV_IMPORT_DLL
#	else
#		define ACTIV_CONTENT_GATEWAY_API ACTIV_IMPORT_LIB
#	endif // !defined (ACTIV_STATIC)
#endif // !defined (ACTIV_CONTENT_GATEWAY_API)

#include "ActivMiddleware/ActivFramework/ActivFramework.h"

#include "ActivContentPlatform/ActivContentGatewayCommon/RealtimeTypes.h"
#include "ActivContentPlatform/ActivContentGatewayCommon/TimeSeriesTypes.h"
#include "ActivContentPlatform/ActivContentGatewayCommon/SymbolDirectoryTypes.h"
#include "ActivContentPlatform/ActivContentGatewayCommon/Tick.h"
#include "ActivContentPlatform/ActivContentGatewayCommon/IntradayBar.h"
#include "ActivContentPlatform/ActivContentGatewayCommon/HistoryBar.h"
#include "ActivContentPlatform/ActivFeedCommon/UpdateIds.h"
#include "ActivContentPlatform/ActivFeedCommon/EventTypes.h"

#endif // !defined (ACTIV_CONTENT_GATEWAY_API_EXTERNAL_H)

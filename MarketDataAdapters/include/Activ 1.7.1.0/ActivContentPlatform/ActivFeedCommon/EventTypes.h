/**
 *	@file	EventTypes.h
 *
 *	@brief	Header file for the event types.
 *
 *	$Log: $
 */

#if !defined(ACTIV_FEED_EVENT_TYPES_H)
#define ACTIV_FEED_EVENT_TYPES_H

#include "ActivMiddleware/Activ.h"

#include "ActivContentPlatform/ActivFeedCommon/ActivFeedDefinitions.h"

namespace Activ
{

namespace ContentPlatform
{

namespace Feed
{

const EventType EVENT_TYPE_NONE = 0;
const EventType EVENT_TYPE_TRADE = 1;
const EventType EVENT_TYPE_TICK = 2;
const EventType EVENT_TYPE_BBO_QUOTE = 3;
const EventType EVENT_TYPE_TRADE_CORRECTION = 4;
const EventType EVENT_TYPE_TRADE_CANCEL = 5;
const EventType EVENT_TYPE_TRADE_NON_REGULAR = 6;
const EventType EVENT_TYPE_BBO_DEPTH = 7;
const EventType EVENT_TYPE_REFRESH = 8;
const EventType EVENT_TYPE_ORDER = 9;
const EventType EVENT_TYPE_QUOTE = 10;
const EventType EVENT_TYPE_IPO = 11;
const EventType EVENT_TYPE_IPO_CANCEL = 12;
const EventType EVENT_TYPE_DELIST = 13;
const EventType EVENT_TYPE_DELIST_CANCEL = 14;
const EventType EVENT_TYPE_DIVIDEND = 15;
const EventType EVENT_TYPE_DIVIDEND_CANCEL = 16;
const EventType EVENT_TYPE_DIVIDEND_CORRECTION = 17;
const EventType EVENT_TYPE_SPLIT = 18;
const EventType EVENT_TYPE_SPLIT_CANCEL = 19;
const EventType EVENT_TYPE_SPLIT_CORRECTION = 20;
const EventType EVENT_TYPE_SYMBOL_CHANGE = 21;
const EventType EVENT_TYPE_SYMBOL_CHANGE_CANCEL = 22;
const EventType EVENT_TYPE_OPTION_SYMBOL_CHANGE = 23;
const EventType EVENT_TYPE_OPTION_SPLIT = 24;
const EventType EVENT_TYPE_CLOSING_QUOTE = 25;
const EventType EVENT_TYPE_CLOSING_BBO_QUOTE = 26;
const EventType EVENT_TYPE_CLOSE = 27;
const EventType EVENT_TYPE_RESET = 28;
const EventType EVENT_TYPE_COMPOSITE_BBO_QUOTE = 29;
const EventType EVENT_TYPE_NEWS = 30;
const EventType EVENT_TYPE_TIME_SERIES_CORRECTION = 31;
const EventType EVENT_TYPE_TIME_SERIES_DELETE = 32;
const EventType EVENT_TYPE_PRICE_ADJUSTMENT = 33;
const EventType EVENT_TYPE_PRICE_ADJUSTMENT_CANCEL = 34;
const EventType EVENT_TYPE_OPEN = 35;
const EventType EVENT_TYPE_NEWS_DELETE = 36;
const EventType EVENT_TYPE_PURGE = 37;
const EventType EVENT_TYPE_ALERT = 38;
const EventType EVENT_TYPE_REFRESH_CYCLE = 39;
const EventType EVENT_TYPE_FORCE_PROCESS_REFRESH = 40;
const EventType EVENT_TYPE_OPTION_REFRESH = 41;
const EventType EVENT_TYPE_HALT_RESUME = 42;
const EventType EVENT_TYPE_IMBALANCE_VOLUME = 43;
const EventType EVENT_TYPE_PRICE_INDICATION = 44;
const EventType EVENT_TYPE_CACHE_FLUSH = 45;
const EventType EVENT_TYPE_MULTIPLE_DELETE = 46;

const EventType EVENT_TYPE_MARKET_DEPTH = EVENT_TYPE_BBO_DEPTH; ///< Deprecated.
const EventType EVENT_TYPE_TRADE_LATE = EVENT_TYPE_TRADE_NON_REGULAR;	///< Deprecated.

} // namespace Feed

} // namespace ContentPlatform

} // namespace Activ

#endif // !defined(ACTIV_FEED_EVENT_TYPES_H)

/**
 *	@file	EntityTypes.h
 *
 *	@brief	Header file for entity types.
 *
 *	$Log: $
 */

#if !defined(ACTIV_FEED_ENTITY_TYPES_H)
#define ACTIV_FEED_ENTITY_TYPES_H

#include "ActivMiddleware/Activ.h"

namespace Activ
{

namespace Feed
{

typedef uint8_t EntityType;	///< The entity type type definition.

const EntityType ENTITY_TYPE_NONE = 0;
const EntityType ENTITY_TYPE_EQUITY = 1;
const EntityType ENTITY_TYPE_EQUITY_OPTION = 2;
const EntityType ENTITY_TYPE_INDEX_OPTION = 3;
const EntityType ENTITY_TYPE_CURRENCY_OPTION = 4;
const EntityType ENTITY_TYPE_INDEX = 5;
const EntityType ENTITY_TYPE_CORPORATE_BOND = 6;
const EntityType ENTITY_TYPE_FUTURE = 7;
const EntityType ENTITY_TYPE_FUTURE_OPTION = 8;
const EntityType ENTITY_TYPE_FOREX = 9;
const EntityType ENTITY_TYPE_MUTUAL_FUND = 10;
const EntityType ENTITY_TYPE_MONEY_MARKET_FUND = 11;
const EntityType ENTITY_TYPE_BOND = 12;
const EntityType ENTITY_TYPE_MARKET_MAKER_QUOTE = 13;
const EntityType ENTITY_TYPE_GOVERNMENT_BOND = 14;
const EntityType ENTITY_TYPE_SPOT = 15;
const EntityType ENTITY_TYPE_CORPORATE_ACTION = 16;
const EntityType ENTITY_TYPE_UNIT_INVESTMENT_TRUST = 17;
const EntityType ENTITY_TYPE_EXCHANGE_TRADED_FUND = 18;
const EntityType ENTITY_TYPE_COMPANY = 19;
const EntityType ENTITY_TYPE_SECURITY = 20;
const EntityType ENTITY_TYPE_CONVERTIBLE = 21;
const EntityType ENTITY_TYPE_DEBENTURE = 22;
const EntityType ENTITY_TYPE_PREFERENCE = 23;
const EntityType ENTITY_TYPE_WARRANT = 24;
const EntityType ENTITY_TYPE_NEWS = 25;
const EntityType ENTITY_TYPE_ANALYTICS = 26;
const EntityType ENTITY_TYPE_ORDER = 27;
const EntityType ENTITY_TYPE_OPTION_ROOT = 28;
const EntityType ENTITY_TYPE_FUTURE_ROOT = 29;
const EntityType ENTITY_TYPE_FUTURE_OPTION_ROOT = 30;
const EntityType ENTITY_TYPE_NEWS_HEADLINE = 31;
const EntityType ENTITY_TYPE_STATISTICS = 32;
const EntityType ENTITY_TYPE_RANKINGS = 33;

} // namespace Feed

} // namespace Activ

#endif // !defined(ACTIV_FEED_ENTITY_TYPES_H)

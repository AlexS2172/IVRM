/**
 *	@file	Exchanges.h
 *
 *	@brief	Header file for the exchanges.
 *
 *	$Log: $
 */

#if !defined(ACTIV_FEED_EXCHANGES_H)
#define ACTIV_FEED_EXCHANGES_H

#include "ActivMiddleware/Activ.h"

#include "ActivMiddleware/Misc/StlHelper.h"

#include "ActivContentPlatform/ActivFeedCommon/ActivFeedDefinitions.h"

#include <vector>
#include <string>
#include <map>

namespace Activ
{

namespace ContentPlatform
{

namespace Feed
{

typedef std::pair<Feed::TableNo, std::string> CompositeExchangeMapKey;		///< Composite exchange map key type definition.
typedef std::map<CompositeExchangeMapKey, std::string> CompositeExchangeMap;	///< Composite exchange map type definition.

const char INDEX_IDENTIFIER('=');

const char CORPORATE_ACTION_SEPARATOR('%');
const char DEAD_INSTRUMENT_SEPARATOR('#');
const char EXCHANGE_SEPARATOR('.');
const char EXPIRATION_DATE_SEPARATOR('/');
const char FORWARD_CODE_SEPARATOR('/');
const char LEVEL2_SEPARATOR('-');
const char MARKET_SEPARATOR('/');
const char NEWS_SOURCE_SEPARATOR('%');
const char ORDER_SEPARATOR(';');
const char QUOTE_SEPARATOR(':');
const char RANKINGS_SEPARATOR('+');
const char SPREAD_FUTURES_SEPARATOR('-');
const char STATISTICS_SEPARATOR('+');
const char STRIKE_PRICE_SEPARATOR('/');
const char SYMBOL_SEPARATOR(' ');

const char ANALYTICS_SEPARATOR(LEVEL2_SEPARATOR);	///< Deprecated.

const char g_compositeSymbolSeparatorList[] = { EXCHANGE_SEPARATOR, QUOTE_SEPARATOR, ORDER_SEPARATOR, CORPORATE_ACTION_SEPARATOR, DEAD_INSTRUMENT_SEPARATOR, NEWS_SOURCE_SEPARATOR, '\0' };
const char g_exchangeSeparatorList[] = { EXCHANGE_SEPARATOR, CORPORATE_ACTION_SEPARATOR, DEAD_INSTRUMENT_SEPARATOR, NEWS_SOURCE_SEPARATOR, '\0'  };
const char g_level2SeparatorList[] = { QUOTE_SEPARATOR, ORDER_SEPARATOR, LEVEL2_SEPARATOR, '\0'  };

const std::string EXCHANGE_AMEX("A");
const std::string EXCHANGE_AMEX_OPTIONS("AO");
const std::string EXCHANGE_ARCA("EA");
const std::string EXCHANGE_ASX("AX");
const std::string EXCHANGE_BOSTON("B");
const std::string EXCHANGE_BOSTON_OPTIONS("BO");
const std::string EXCHANGE_BRUT("EB");
const std::string EXCHANGE_CBOT("CB");
const std::string EXCHANGE_CBOE("W");
const std::string EXCHANGE_CFE("CF");
const std::string EXCHANGE_CHICAGO("MW");
const std::string EXCHANGE_CME("CM");
const std::string EXCHANGE_CME_GLOBEX("CMG");
const std::string EXCHANGE_CNQ("CQ");
const std::string EXCHANGE_COMEX("CX");
const std::string EXCHANGE_COMEX_ACCESS("CXA");
const std::string EXCHANGE_E_CBOT("CBE");
const std::string EXCHANGE_INTERNATIONAL_SECURITIES("Y");
const std::string EXCHANGE_INET("EI");
const std::string EXCHANGE_KCBOT("KC");
const std::string EXCHANGE_KCBOT_ELECTRONIC("KCE");
const std::string EXCHANGE_MONTREAL("M");
const std::string EXCHANGE_NASD_ADF("QD");
const std::string EXCHANGE_NASDAQ("Q");
const std::string EXCHANGE_NASDAQ_BULLETIN_BOARD("QB");
const std::string EXCHANGE_NASDAQ_OTHER_OTC("QO");
const std::string EXCHANGE_NASDAQ_THIRD_MARKET("TH");
const std::string EXCHANGE_NATIONAL("C");
const std::string EXCHANGE_NYSE("N");
const std::string EXCHANGE_NYBOT("NB");
const std::string EXCHANGE_NYMEX("NX");
const std::string EXCHANGE_NYMEX_ACCESS("NXA");
const std::string EXCHANGE_NYMEX_CLEARPOINT("NXC");
const std::string EXCHANGE_ONE_CHICAGO("OC");
const std::string EXCHANGE_PACIFIC("PA");
const std::string EXCHANGE_PACIFIC_OPTIONS("PO");
const std::string EXCHANGE_PBOT("XB");
const std::string EXCHANGE_PHILADELPHIA("X");
const std::string EXCHANGE_PHILADELPHIA_OPTIONS("XO");
const std::string EXCHANGE_TENFORE("TF");
const std::string EXCHANGE_TSX("TO");
const std::string EXCHANGE_TSX_VENTURE("TV");
const std::string EXCHANGE_US_OPTIONS_COMPOSITE("O");

const std::string EXCHANGE_FEED_STATUS("FEED");
const std::string EXCHANGE_LATENCY("LTCY");

const std::string EXCHANGE_CINCINNATI(EXCHANGE_NATIONAL);				///< Deprecated.
const std::string EXCHANGE_NASDAQ_ADF(EXCHANGE_NASD_ADF);				///< Deprecated.
const std::string EXCHANGE_THIRD_MARKET(EXCHANGE_NASDAQ_THIRD_MARKET);	///< Deprecated.

/**
 *	@brief	Get exchange.
 *
 *	@param	symbol the symbol.
 *
 *	@return	the exchange of the symbol.
 */
inline std::string GetExchange(const std::string &symbol)
{
	const std::string::size_type exchangeIndex = symbol.find_last_of(g_exchangeSeparatorList);

	if (std::string::npos == exchangeIndex)
		return std::string("");
	
	const std::string::size_type level2Index = symbol.find_first_of(g_level2SeparatorList, exchangeIndex + 1);

	return (std::string::npos == level2Index) ? symbol.substr(exchangeIndex + 1) : symbol.substr(exchangeIndex + 1, level2Index - (exchangeIndex + 1));
}

/**
 *	@brief	Get the default composite symbol.
 *
 *	@param	symbol the symbol.
 *
 *	@return	the default composite symbol.
 */
inline std::string GetDefaultCompositeSymbol(const std::string &symbol)
{
	const std::string::size_type index = symbol.find_last_of(g_compositeSymbolSeparatorList);

	return (std::string::npos == index) ? symbol : symbol.substr(0, index + 1);
}

/**
 *	@brief	Get the composite symbol.
 *
 *	@param	compositeExchangeMap the composite exchange map.
 *	@param	tableNumber the table containing \a symbol.
 *	@param	symbol the symbol.
 *	@param	compositeSymbol to receive the composite symbol.
 *
 *	@return	true or false if the exchange is not in \a compositeExchangeMap.
 */
inline bool GetCompositeSymbol(const CompositeExchangeMap &compositeExchangeMap, const Feed::TableNo tableNumber, const std::string &symbol, std::string &compositeSymbol)
{
	const CompositeExchangeMapKey compositeExchangeMapKey(tableNumber, GetExchange(symbol));
	const CompositeExchangeMap::const_iterator iterator = compositeExchangeMap.find(compositeExchangeMapKey);
	if (compositeExchangeMap.end() == iterator)
		return false;

	compositeSymbol = GetDefaultCompositeSymbol(symbol) + iterator->second;
	return true;
}

} // namespace Feed

} // namespace ContentPlatform

} // namespace Activ

#endif // !defined(ACTIV_FEED_EXCHANGES_H)

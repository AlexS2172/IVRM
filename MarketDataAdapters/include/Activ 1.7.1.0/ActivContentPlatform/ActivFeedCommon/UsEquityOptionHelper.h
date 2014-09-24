/**
 *	@file	source/ActivContentPlatform/ActivFeedCommon/UsEquityOptionHelper.h
 *
 *	@brief	Header file for the Us equity option helper class.
 *
 *	$Log: $
 */

#if (!defined ACTIV_FEED_COMMON_US_EQUITY_OPTION_HELPER_H)
#define ACTIV_FEED_COMMON_US_EQUITY_OPTION_HELPER_H

#include "ActivMiddleware/StatusCodes.h"
#include "ActivMiddleware/ActivBase/MessageValidator.h"

#if (ACTIV_MIDDLEWARE_VERSION_VALUE >= ACTIV_MAKE_VERSION_VALUE(1, 8, 12, 0))
#	include "ActivMiddleware/ActivBase/FieldTypes/Date.h"
#	include "ActivMiddleware/ActivBase/FieldTypes/Rational.h"
#else
#	include "ActivMiddleware/ActivFieldTypes/Date.h"
#	include "ActivMiddleware/ActivFieldTypes/Rational.h"
#endif

#include "ActivContentPlatform/ActivFeedCommon/Exchanges.h"

#include <cstring>
#include <string>

namespace Activ
{

namespace ContentPlatform
{

namespace Feed
{

/**
 *	@brief	Us equity option helper class.
 *
 *	To ensure consistent results the strike price numerator and denominator
 *	should be compressed to the lowest value numerator. For example in the
 *	encoding of the value 0.1 there are several options two of which are:
 *		NUMERATOR = 1, DECIMAL-PLACES=1 or
 *		NUMERATOR = 10, DECIMAL-PLACES=2
 *
 *	These two examples will not result in consistent symbols. The correct
 *	procedure is to use the first example where the numerator has the lowest
 *	possible integer value. The Rational object provides a convenient
 *	interface to ensure the strike price is suitably compressed. For further
 *	details refer to the Rational::Compress.
 */
class UsEquityOptionHelper
{
public:
	// Blob format.
	// D = Day
	// M = Month
	// X = Numerator
	// Y = Year
	// Z = Denominator
	// BYTE-1:	Y6	Y5	Y4	Y3	Y2	Y1
	// BYTE-2:	Y0	M4	M3	M2	M1	M0
	// BYTE-3:	Z0	D4	D3	D2	D1	D0
	// BYTE-4:	X3	X2	X1	X0	Z2	Z1
	// BYTE-5:	X9	X8	X7	X6	X5	X4
	// BYTE-6;	X15	X14	X13	X12	X11	X10
	// BYTE-7;	X21	X20	X19	X18	X17	X16
	// BYTE-8;	X27	X26	X25	X24	X23	X22
	// BYTE-9;	-	-	X31	X30	X29	X28
	//
	// Blob will have a minimum length of 4 bytes and a maximum of 9 bytes.

	/**
	 *	@brief	Option type enumeration.
	 */
	enum OptionType
	{
		OPTION_TYPE_CALL,
		OPTION_TYPE_PUT
	};

	/**
	 *	@brief	Default constructor.
	 */
	UsEquityOptionHelper();

	/**
	 *	@brief	Constructor.
	 *
	 *	@param	pSymbol the symbol buffer to deserialize the data from.
	 *	@param	symbolLength the symbol buffer length.
	 *
	 *	@throw	StatusCodeException
	 */
	UsEquityOptionHelper(const void * const pSymbol, const size_t symbolLength);

	/**
	 *	@brief	Constructor.
	 *
	 *	@param	symbol the string to deserialize the data from.
	 *
	 *	@throw	StatusCodeException
	 */
	UsEquityOptionHelper(const std::string &symbol);

	/**
	 *	@brief	Serialize.
	 *
	 *	@param	pSymbol the buffer to contruct the symbol in.
	 *	@param	symbolLength the size of the buffer passed in (at least MAX_SYMBOL_LENGTH).
	 *			On success will contain the number of bytes used.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
	StatusCode Serialize(void * const pSymbol, size_t &symbolLength) const;

	/**
	 *	@brief	Serialize.
	 *
	 *	@param	symbol the string to contruct the symbol in.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
	StatusCode Serialize(std::string &symbol) const;

	/**
	 *	@brief	Deserialize.
	 *
	 *	@param	symbol the symbol buffer to deserialize the data from.
	 *	@param	symbolLength the symbol buffer length (no greater than MAX_SYMBOL_LENGTH).
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
	StatusCode Deserialize(const void * const pSymbol, const size_t symbolLength);

	/**
	 *	@brief	Deserialize.
	 *
	 *	@param	symbol the string to deserialize the data from.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
	StatusCode Deserialize(const std::string &symbol);

	/**
	 *	@brief	Set the expiration date and option type.
	 *
	 *	@param	expirationYear the expiration year.
	 *	@param	expirationMonth the expiration month.
	 *	@param	expirationDay the expiration day.
	 *	@param	optionType the option type.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
	StatusCode SetExpirationDateAndOptionType(const uint32_t expirationYear, const uint8_t expirationMonth, const uint8_t expirationDay, const OptionType optionType);

	/**
	 *	@brief	Set the expiration date and option type.
	 *
	 *	@param	expirationDate the expiration date.
	 *	@param	optionType the option type.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
	StatusCode SetExpirationDateAndOptionType(const Date &expirationDate, const OptionType optionType);

	/**
	 *	@brief	Get the expiration date and option type.
	 *
	 *	@param	expirationDate the expiration date.
	 *	@param	optionType the option type.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
	StatusCode GetExpirationDateAndOptionType(Date &expirationDate, OptionType &optionType) const;

	/**
	 *	@brief	Get the expiration date.
	 *
	 *	@param	expirationDate the expiration date.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
	StatusCode GetExpirationDate(Date &expirationDate) const;

	/**
	 *	@brief	Get the expiration date.
	 *
	 *	@return	the expiration date.
	 *
	 *	@throw	StatusCodeException
	 */
	const Date GetExpirationDate() const;

	/**
	 *	@brief	Get the option type.
	 *
	 *	@param	optionType the option type.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
	StatusCode GetOptionType(OptionType &optionType) const;

	/**
	 *	@brief	Get the option type.
	 *
	 *	@return	the option type.
	 */
	const OptionType GetOptionType() const;

	/**
	 *	@brief	Build alias symbol.
	 *
	 *	@param	aliasSymbol the constructed alias symbol.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
	StatusCode BuildAliasSymbol(std::string &aliasSymbol) const;

	/**
	 *	@brief	Build alias strike price.
	 *
	 *	@param	aliasStrikePrice the alias strike price.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
	StatusCode BuildAliasStrikePrice(std::string &aliasStrikePrice) const;

	/**
	 *	@brief	Build alias expiration date.
	 *
	 *	@param	aliasExpirationDate the alias expiration date.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
	StatusCode BuildAliasExpirationDate(std::string &aliasExpirationDate) const;

	/**
	 *	@brief	Serialize.
	 *
	 *	@param	pSymbol the buffer to contruct the symbol in.
	 *	@param	symbolLength the size of the buffer passed in (at least MAX_SYMBOL_LENGTH). On success will contain the number of bytes used.
	 *	@param	pRoot a pointer to the root.
	 *	@param	rootLength the root length.
	 *	@param	expirationYear the expiration year (0 - 99).
	 *	@param	expirationMonth the expiration month (Jan-Dec, A-L for CALLS & M-X for PUTS).
	 *	@param	expirationDay the expiration day.
	 *	@param	strikePriceNumerator the strike price numerator.
	 *	@param	strikePriceDenominator the strike price denominator.
	 *	@param	pExchange a pointer to the exchange.
	 *	@param	exchangeLength the exchange length.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
	static StatusCode Serialize(void * const pSymbol, size_t &symbolLength, const void * const pRoot, const size_t rootLength, const uint8_t expirationYear, const char expirationMonth, const uint8_t expirationDay, const uint32_t strikePriceNumerator, const Rational::Denominator strikePriceDenominator, const void * const pExchange, const size_t exchangeLength);

	/**
	 *	@brief	Serialize.
	 *
	 *	@param	pSymbol the buffer to contruct the symbol in.
	 *	@param	symbolLength the size of the buffer passed in (at least MAX_SYMBOL_LENGTH). On success will contain the number of bytes used.
	 *	@param	pRoot a pointer to the root.
	 *	@param	rootLength the root length.
	 *	@param	expirationYear the expiration year (0 - 99).
	 *	@param	expirationMonth the expiration month (Jan-Dec, A-L for CALLS & M-X for PUTS).
	 *	@param	expirationDay the expiration day.
	 *	@param	strikePrice the strike price.
	 *	@param	pExchange a pointer to the exchange.
	 *	@param	exchangeLength the exchange length.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
	static StatusCode Serialize(void * const pSymbol, size_t &symbolLength, const void * const pRoot, const size_t rootLength, const uint8_t expirationYear, const char expirationMonth, const uint8_t expirationDay, const Rational &strikePrice, const void * const pExchange, const size_t exchangeLength);

	/**
	 *	@brief	Serialize.
	 *
	 *	@param	pSymbol the buffer to contruct the symbol in.
	 *	@param	symbolLength the size of the buffer passed in (at least MAX_SYMBOL_LENGTH). On success will contain the number of bytes used.
	 *	@param	root the root string.
	 *	@param	expirationYear the expiration year (0 - 99).
	 *	@param	expirationMonth the expiration month (Jan-Dec, A-L for CALLS & M-X for PUTS).
	 *	@param	expirationDay the expiration day.
	 *	@param	strikePrice the strike price.
	 *	@param	exchange the exchange string.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
	static StatusCode Serialize(void * const pSymbol, size_t &symbolLength, const std::string &root, const uint8_t expirationYear, const char expirationMonth, const uint8_t expirationDay, const Rational &strikePrice, const std::string &exchange);

	/**
	 *	@brief	Serialize.
	 *
	 *	@param	pSymbol the buffer to contruct the symbol in.
	 *	@param	symbolLength the size of the buffer passed in (at least MAX_SYMBOL_LENGTH). On success will contain the number of bytes used.
	 *	@param	root the root string.
	 *	@param	expirationYear the expiration year.
	 *	@param	expirationMonth the expiration month.
	 *	@param	expirationDay the expiration day.
	 *	@param	optionType the option type.
	 *	@param	strikePrice the strike price.
	 *	@param	exchange the exchange string.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
	static StatusCode Serialize(void * const pSymbol, size_t &symbolLength, const std::string &root, const uint32_t expirationYear, const uint8_t expirationMonth, const uint8_t expirationDay, const OptionType optionType, const Rational &strikePrice, const std::string &exchange);

	/**
	 *	@brief	Serialize.
	 *
	 *	@param	pSymbol the buffer to contruct the symbol in.
	 *	@param	symbolLength the size of the buffer passed in (at least MAX_SYMBOL_LENGTH). On success will contain the number of bytes used.
	 *	@param	root the root string.
	 *	@param	expirationDate the expiration date.
	 *	@param	optionType the option type.
	 *	@param	strikePrice the strike price.
	 *	@param	exchange the exchange string.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
	static StatusCode Serialize(void * const pSymbol, size_t &symbolLength, const std::string &root, const Date &expirationDate, const OptionType optionType, const Rational &strikePrice, const std::string &exchange);

	/**
	 *	@brief	Serialize.
	 *
	 *	@param	symbol the string to contruct the symbol in.
	 *	@param	root the root string.
	 *	@param	expirationYear the expiration year (0 - 99).
	 *	@param	expirationMonth the expiration month (Jan-Dec, A-L for CALLS & M-X for PUTS).
	 *	@param	expirationDay the expiration day.
	 *	@param	strikePrice the strike price.
	 *	@param	exchange the exchange string.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
	static StatusCode Serialize(std::string &symbol, const std::string &root, const uint8_t expirationYear, const char expirationMonth, const uint8_t expirationDay, const Rational &strikePrice, const std::string &exchange);

	/**
	 *	@brief	Serialize.
	 *
	 *	@param	symbol the string to contruct the symbol in.
	 *	@param	root the root string.
	 *	@param	expirationYear the expiration year.
	 *	@param	expirationMonth the expiration month.
	 *	@param	expirationDay the expiration day.
	 *	@param	optionType the option type.
	 *	@param	strikePrice the strike price.
	 *	@param	exchange the exchange string.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
	static StatusCode Serialize(std::string &symbol, const std::string &root, const uint32_t expirationYear, const uint8_t expirationMonth, const uint8_t expirationDay, const OptionType optionType, const Rational &strikePrice, const std::string &exchange);

	/**
	 *	@brief	Serialize.
	 *
	 *	@param	symbol the string to contruct the symbol in.
	 *	@param	root the root string.
	 *	@param	expirationDate the expiration date.
	 *	@param	optionType the option type.
	 *	@param	strikePrice the strike price.
	 *	@param	exchange the exchange string.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
	static StatusCode Serialize(std::string &symbol, const std::string &root, const Date &expirationDate, const OptionType optionType, const Rational &strikePrice, const std::string &exchange);

	/**
	 *	@brief	Deserialize.
	 *
	 *	@param	pSymbol the symbol buffer to deserialize the data from.
	 *	@param	symbolLength the symbol buffer length (no greater than MAX_SYMBOL_LENGTH).
	 *	@param	root the root string.
	 *	@param	expirationYear the expiration year (0 - 99).
	 *	@param	expirationMonth the expiration month (Jan-Dec, A-L for CALLS & M-X for PUTS).
	 *	@param	expirationDay the expiration day.
	 *	@param	strikePrice the strike price code.
	 *	@param	exchange the exchange string.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
	static StatusCode Deserialize(const void * const pSymbol, const size_t symbolLength, std::string &root, uint8_t &expirationYear, char &expirationMonth, uint8_t &expirationDay, Rational &strikePrice, std::string &exchange);

	/**
	 *	@brief	Deserialize.
	 *
	 *	@param	symbol the string to deserialize the data from.
	 *	@param	root the root string.
	 *	@param	expirationYear the expiration year (0 - 99).
	 *	@param	expirationMonth the expiration month (Jan-Dec, A-L for CALLS & M-X for PUTS).
	 *	@param	expirationDay the expiration day.
	 *	@param	strikePrice the strike price code.
	 *	@param	exchange the exchange string.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
	static StatusCode Deserialize(const std::string &symbol, std::string &root, uint8_t &expirationYear, char &expirationMonth, uint8_t &expirationDay, Rational &strikePrice, std::string &exchange);

	/**
	 *	@brief	Deserialize.
	 *
	 *	@param	symbol the string to deserialize the data from.
	 *	@param	root the root string.
	 *	@param	expirationDate the expiration date.
	 *	@param	optionType the option type.
	 *	@param	strikePrice the strike price code.
	 *	@param	exchange the exchange string.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
	static StatusCode Deserialize(const std::string &symbol, std::string &root, Date &expirationDate, OptionType &optionType, Rational &strikePrice, std::string &exchange);

	/**
	 *	@brief	Get the maximum serialized length.
	 */
	static size_t GetMaxSerializedLength();

	/**
	 *	@brief	Get the maximum serialized length.
	 *
	 *	@param	root the root string length.
	 *	@param	exchange the exchange string length.
	 */
	static size_t GetMaxSerializedLength(const size_t rootLength, const size_t exchangeLength);

	/**
	 *	@brief	Get the maximum serialized length.
	 *
	 *	@param	root the root string.
	 *	@param	exchange the exchange string.
	 */
	static size_t GetMaxSerializedLength(const std::string &root, const std::string &exchange);

	/**
	 *	@brief	Get Activ date and option type from expiration year/month/day.
	 *
	 *	@param	expirationDate the expiration date.
	 *	@param	optionType the option type.
	 *	@param	expirationYear the expiration year (0 - 99).
	 *	@param	expirationMonth the expiration month (Jan-Dec, A-L for CALLS & M-X for PUTS).
	 *	@param	expirationDay the expiration day.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
	static StatusCode GetActivDateAndOptionTypeFromExpirationDate(Date &expirationDate, OptionType &optionType, const uint8_t expirationYear, const char expirationMonth, const uint8_t expirationDay);

	/**
	 *	@brief	Get expiration year/month/day from Activ date and option type.
	 *
	 *	@param	expirationDate the expiration date.
	 *	@param	optionType the option type.
	 *	@param	expirationYear the expiration year (0 - 99).
	 *	@param	expirationMonth the expiration month (Jan-Dec, A-L for CALLS & M-X for PUTS).
	 *	@param	expirationDay the expiration day.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
	static StatusCode GetExpirationDateFromActivDateAndOptionType(const Date &expirationDate, const OptionType optionType, uint8_t &expirationYear, char &expirationMonth, uint8_t &expirationDay);

	/**
	 *	@brief	Build alias symbol.
	 *
	 *	@param	aliasSymbol the constructed alias symbol.
	 *	@param	root the root string.
	 *	@param	expirationDate the expiration date.
	 *	@param	optionType the option type.
	 *	@param	strikePrice the strike price code.
	 *	@param	exchange the exchange string.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
	static StatusCode BuildAliasSymbol(std::string &aliasSymbol, const std::string &root, const Date &expirationDate, const OptionType optionType, const Rational &strikePrice, const std::string &exchange);

	/**
	 *	@brief	Build alias symbol.
	 *
	 *	@param	aliasSymbol the constructed alias symbol.
	 *	@param	root the root string.
	 *	@param	expirationYear the expiration year (0 - 99).
	 *	@param	expirationMonth the expiration month (Jan-Dec, A-L for CALLS & M-X for PUTS).
	 *	@param	expirationDay the expiration day.
	 *	@param	strikePrice the strike price code.
	 *	@param	exchange the exchange string.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
	static StatusCode BuildAliasSymbol(std::string &aliasSymbol, const std::string &root, const uint8_t expirationYear, const char expirationMonth, const uint8_t expirationDay, const Rational &strikePrice, const std::string &exchange);

	/**
	 *	@brief	Build alias strike price.
	 *
	 *	@param	aliasStrikePrice the alias strike price.
	 *	@param	strikePrice the strike price code.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
	static StatusCode BuildAliasStrikePrice(std::string &aliasStrikePrice, const Rational &strikePrice);

	/**
	 *	@brief	Build alias expiration date.
	 *
	 *	@param	aliasExpirationDate the alias expiration date.
	 *	@param	expirationDate the expiration date.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
	static StatusCode BuildAliasExpirationDate(std::string &aliasExpirationDate, const Date &expirationDate);

	/**
	 *	@brief	Build alias expiration date.
	 *
	 *	@param	aliasExpirationDate the alias expiration date.
	 *	@param	expirationYear the expiration year (0 - 99).
	 *	@param	expirationMonth the expiration month (Jan-Dec, A-L for CALLS & M-X for PUTS).
	 *	@param	expirationDay the expiration day.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
	static StatusCode BuildAliasExpirationDate(std::string &aliasExpirationDate, const uint8_t expirationYear, const char expirationMonth, const uint8_t expirationDay);

	/**
	 *	@brief	Get symbol from Osi symbol.
	 *
	 *	@param	symbol the symbol.
	 *	@param	osiSymbol the Osi symbol.
	 *	@param	exchange the exchange.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
	static StatusCode GetSymbolFromOsiSymbol(std::string &symbol, const std::string &osiSymbol, const std::string &exchange);

	/**
	 *	@brief	Get Osi symbol from symbol.
	 *
	 *	@param	osiSymbol the Osi symbol.
	 *	@param	symbol the symbol.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
	static StatusCode GetOsiSymbolFromSymbol(std::string &osiSymbol, const std::string &symbol);

	static const size_t		MAX_ROOT_LENGTH				= 6;		///< The maximum length of the root symbol.
	static const size_t		MAX_EXCHANGE_LENGTH			= 3;		///< The maximum length of the exchange code.
	static const size_t		MIN_BLOB_LENGTH				= 4;		///< The minimum length of the encoded blob.
	static const size_t		MAX_BLOB_LENGTH				= 9;		///< The maximum length of the encoded blob.
	static const size_t		MAX_SYMBOL_LENGTH			= MAX_ROOT_LENGTH + 1 + MAX_BLOB_LENGTH + 1 + MAX_EXCHANGE_LENGTH;	///< The maximum encoded symbol length including separators.

	static const size_t		ALIAS_STRIKE_PRICE_LENGTH	= 8;		///< The alias strike price string length.
	static const size_t		ALIAS_DATE_LENGTH			= 6;		///< The alias date string length.
	static const size_t		MAX_ALIAS_SYMBOL_LENGTH		= MAX_ROOT_LENGTH + 1 + ALIAS_DATE_LENGTH + 1 + ALIAS_STRIKE_PRICE_LENGTH + 1 + MAX_EXCHANGE_LENGTH;	///< The maximum encoded symbol length including separators.
	static const int64_t	MAX_ALIAS_STRIKE_PRICE		= 99999999;	///< The maximum alias strike price value.

	static const size_t		OSI_ROOT_LENGTH				= 6;		///< The osi root length.
	static const size_t		OSI_SYMBOL_LENGTH			= 21;		///< The osi symbol length.

	std::string				m_root;									///< The root.
	uint8_t					m_expirationYear;						///< The expiration year (valid values range from 0 - 99).
	char					m_expirationMonth;						///< The expiration month (Jan-Dec, A-L for CALLS & M-X for PUTS).
	uint8_t					m_expirationDay;						///< The expiration day of the month.
	Rational				m_strikePrice;							///< The strike price.
	std::string				m_exchange;								///< The exchange.

private:
	/**
	 *	@brief	Denominator enumeration.
	 */
	enum Denominator
	{
		DENOMINATOR_WHOLE,
		DENOMINATOR_1DP,
		DENOMINATOR_2DP,
		DENOMINATOR_3DP,
		DENOMINATOR_4DP,
		DENOMINATOR_5DP,
		DENOMINATOR_X5,
		DENOMINATOR_RESERVED
	};

	/**
	 *	@brief	Get the encoded strike price.
	 *
	 *	@param	strikePriceNumerator the strike price numerator.
	 *	@param	strikePriceDenominator the strike price denominator.
	 *	@param	encodedStrikePriceNumerator the encoded strike price.
	 *	@param	encodedStrikePriceDenominator then encoded strike price denominator.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
	static StatusCode GetEncodedStrikePrice(const uint32_t strikePriceNumerator, const Rational::Denominator strikePriceDenominator, uint32_t &encodedStrikePriceNumerator, Denominator &encodedStrikePriceDenominator);

	/**
	 *	@brief	Get the decoded strike price.
	 *
	 *	@param	strikePrice the strike price.
	 *	@param	encodedStrikePriceNumerator the encoded strike price.
	 *	@param	encodedStrikePriceDenominator then encoded strike price denominator.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
	static StatusCode GetDecodedStrikePrice(Rational &strikePrice, const uint32_t encodedStrikePriceNumerator, const Denominator encodedStrikePriceDenominator);

	static const char	CALL_MONTH_OFFSET			= 'A' - 1;	///< 'Call' month offset.
	static const char	PUT_MONTH_OFFSET			= 'M' - 1;	///< 'Put' month offset.
	static const char	ENCODED_MONTH_OFFSET		= 'A' - 4;	///< Offset applied to the encoded expiration month.

	static const byte_t	UD							= 255;		///< Undefined base64ToBinary table value.
};

// ---------------------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------------------

inline UsEquityOptionHelper::UsEquityOptionHelper() :
	m_expirationYear(0),
	m_expirationMonth(0),
	m_expirationDay(0)
{
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline UsEquityOptionHelper::UsEquityOptionHelper(const void * const pSymbol, const size_t symbolLength)
{
	ACTIV_THROW_RESULT_IF(STATUS_CODE_SUCCESS, !=, Deserialize(pSymbol, symbolLength));
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline UsEquityOptionHelper::UsEquityOptionHelper(const std::string &symbol)
{
	ACTIV_THROW_RESULT_IF(STATUS_CODE_SUCCESS, !=, Deserialize(symbol));
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode UsEquityOptionHelper::Serialize(void * const pSymbol, size_t &symbolLength) const
{
	return Serialize(pSymbol, symbolLength, m_root.c_str(), m_root.length(), m_expirationYear, m_expirationMonth, m_expirationDay, m_strikePrice, m_exchange.c_str(), m_exchange.length());
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode UsEquityOptionHelper::Serialize(std::string &symbol) const
{
	return Serialize(symbol, m_root, m_expirationYear, m_expirationMonth, m_expirationDay, m_strikePrice, m_exchange);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode UsEquityOptionHelper::Deserialize(const void * const pSymbol, const size_t symbolLength)
{
	return Deserialize(pSymbol, symbolLength, m_root, m_expirationYear, m_expirationMonth, m_expirationDay, m_strikePrice, m_exchange);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode UsEquityOptionHelper::Deserialize(const std::string &symbol)
{
	return Deserialize(symbol, m_root, m_expirationYear, m_expirationMonth, m_expirationDay, m_strikePrice, m_exchange);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode UsEquityOptionHelper::SetExpirationDateAndOptionType(const uint32_t expirationYear, const uint8_t expirationMonth, const uint8_t expirationDay, const OptionType optionType)
{
	m_expirationYear = expirationYear % 100;
	m_expirationMonth = expirationMonth + ((OPTION_TYPE_CALL == optionType) ? CALL_MONTH_OFFSET : PUT_MONTH_OFFSET);
	m_expirationDay = expirationDay;

	return STATUS_CODE_SUCCESS;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode UsEquityOptionHelper::SetExpirationDateAndOptionType(const Date &expirationDate, const OptionType optionType)
{
	return GetExpirationDateFromActivDateAndOptionType(expirationDate, optionType, m_expirationYear, m_expirationMonth, m_expirationDay);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode UsEquityOptionHelper::GetExpirationDateAndOptionType(Date &expirationDate, OptionType &optionType) const
{
	return GetActivDateAndOptionTypeFromExpirationDate(expirationDate, optionType, m_expirationYear, m_expirationMonth, m_expirationDay);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode UsEquityOptionHelper::GetExpirationDate(Date &expirationDate) const
{
	return expirationDate.Set(m_expirationYear + 2000, m_expirationMonth - ((m_expirationMonth <= PUT_MONTH_OFFSET) ? CALL_MONTH_OFFSET : PUT_MONTH_OFFSET), m_expirationDay);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline const Date UsEquityOptionHelper::GetExpirationDate() const
{
	return Date(m_expirationYear + 2000, m_expirationMonth - ((m_expirationMonth <= PUT_MONTH_OFFSET) ? CALL_MONTH_OFFSET : PUT_MONTH_OFFSET), m_expirationDay);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode UsEquityOptionHelper::GetOptionType(OptionType &optionType) const
{
	optionType = (m_expirationMonth <= PUT_MONTH_OFFSET) ? OPTION_TYPE_CALL : OPTION_TYPE_PUT;

	return STATUS_CODE_SUCCESS;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline const UsEquityOptionHelper::OptionType UsEquityOptionHelper::GetOptionType() const
{
	return (m_expirationMonth <= PUT_MONTH_OFFSET) ? OPTION_TYPE_CALL : OPTION_TYPE_PUT;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode UsEquityOptionHelper::BuildAliasSymbol(std::string &aliasSymbol) const
{
	return BuildAliasSymbol(aliasSymbol, m_root, m_expirationYear, m_expirationMonth, m_expirationDay, m_strikePrice, m_exchange);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode UsEquityOptionHelper::BuildAliasStrikePrice(std::string &aliasStrikePrice) const
{
	return BuildAliasStrikePrice(aliasStrikePrice, m_strikePrice);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode UsEquityOptionHelper::BuildAliasExpirationDate(std::string &aliasExpirationDate) const
{
	return BuildAliasExpirationDate(aliasExpirationDate, m_expirationYear, m_expirationMonth, m_expirationDay);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode UsEquityOptionHelper::Serialize(void * const pSymbol, size_t &symbolLength, const void * const pRoot, const size_t rootLength, const uint8_t expirationYear, const char expirationMonth, const uint8_t expirationDay, const uint32_t strikePriceNumerator, const Rational::Denominator strikePriceDenominator, const void * const pExchange, const size_t exchangeLength)
{
	static const byte_t binaryToBase64Table[64] =
	{
		'A',	'B',	'C',	'D',	'E',	'F',	'G',	'H',
		'I',	'J',	'K',	'L',	'M',	'N',	'O',	'P',
		'Q',	'R',	'S',	'T',	'U',	'V',	'W',	'X',
		'Y',	'Z',	'a',	'b',	'c',	'd',	'e',	'f',
		'g',	'h',	'i',	'j',	'k',	'l',	'm',	'n',
		'o',	'p',	'q',	'r',	's',	't',	'u',	'v',
		'w',	'x',	'y',	'z',	'0',	'1',	'2',	'3',
		'4',	'5',	'6',	'7',	'8',	'9',	'^',	'~'
	};

	if (symbolLength < GetMaxSerializedLength())
		return STATUS_CODE_FAILURE;

	byte_t * pBuffer = static_cast<byte_t *>(pSymbol);
	const byte_t * const pBufferStart = pBuffer;
	symbolLength = 0;

	uint32_t encodedStrikePriceNumerator;
	Denominator encodedStrikePriceDenominator;

	(void) GetEncodedStrikePrice(strikePriceNumerator, strikePriceDenominator, encodedStrikePriceNumerator, encodedStrikePriceDenominator);

	MemoryCopy(pBuffer, pRoot, rootLength);
	pBuffer += rootLength;

	*pBuffer++ = Feed::EXPIRATION_DATE_SEPARATOR;

	*pBuffer++ = binaryToBase64Table[(expirationYear >> 1) & 0x3F];
	*pBuffer++ = binaryToBase64Table[((expirationYear << 5) | (expirationMonth - ENCODED_MONTH_OFFSET)) & 0x3F];
	*pBuffer++ = binaryToBase64Table[((expirationDay) | (encodedStrikePriceDenominator << 5)) & 0x3F];
	*pBuffer++ = binaryToBase64Table[((encodedStrikePriceDenominator >> 1) | (encodedStrikePriceNumerator << 2)) & 0x3F];

	encodedStrikePriceNumerator >>= 4;

	while (0 != encodedStrikePriceNumerator)
	{
		*pBuffer++ = binaryToBase64Table[encodedStrikePriceNumerator & 0x3F];
		encodedStrikePriceNumerator >>= 6;
	}

	*pBuffer++ = Feed::EXCHANGE_SEPARATOR;

	MemoryCopy(pBuffer, pExchange, exchangeLength);

	symbolLength = (pBuffer - pBufferStart) + exchangeLength;

	return STATUS_CODE_SUCCESS;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode UsEquityOptionHelper::Serialize(void * const pSymbol, size_t &symbolLength, const void * const pRoot, const size_t rootLength, const uint8_t expirationYear, const char expirationMonth, const uint8_t expirationDay, const Rational &strikePrice, const void * const pExchange, const size_t exchangeLength)
{
	int64_t strikePriceNumerator;
	Rational::Denominator strikePriceDenominator;
	ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, strikePrice.Get(strikePriceNumerator, strikePriceDenominator));

	if ((strikePriceNumerator < 0) || (strikePriceNumerator > boost::integer_traits<uint32_t>::const_max))
		return STATUS_CODE_OUT_OF_RANGE;

	return Serialize(pSymbol, symbolLength, pRoot, rootLength, expirationYear, expirationMonth, expirationDay, static_cast<uint32_t>(strikePriceNumerator), strikePriceDenominator, pExchange, exchangeLength);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode UsEquityOptionHelper::Serialize(void * const pSymbol, size_t &symbolLength, const std::string &root, const uint8_t expirationYear, const char expirationMonth, const uint8_t expirationDay, const Rational &strikePrice, const std::string &exchange)
{
	return Serialize(pSymbol, symbolLength, root.c_str(), root.length(), expirationYear, expirationMonth, expirationDay, strikePrice, exchange.c_str(), exchange.length());
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode UsEquityOptionHelper::Serialize(void * const pSymbol, size_t &symbolLength, const std::string &root, const uint32_t expirationYear, const uint8_t expirationMonth, const uint8_t expirationDay, const OptionType optionType, const Rational &strikePrice, const std::string &exchange)
{
	char expirationMonthCode = expirationMonth + ((OPTION_TYPE_CALL == optionType) ? CALL_MONTH_OFFSET : PUT_MONTH_OFFSET);

	return Serialize(pSymbol, symbolLength, root.c_str(), root.length(), expirationYear % 100, expirationMonthCode, expirationDay, strikePrice, exchange.c_str(), exchange.length());
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode UsEquityOptionHelper::Serialize(void * const pSymbol, size_t &symbolLength, const std::string &root, const Date &expirationDate, const OptionType optionType, const Rational &strikePrice, const std::string &exchange)
{
	uint8_t expirationYear, expirationDay;
	char expirationMonth;

	ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, GetExpirationDateFromActivDateAndOptionType(expirationDate, optionType, expirationYear, expirationMonth, expirationDay));

	return Serialize(pSymbol, symbolLength, root, expirationYear, expirationMonth, expirationDay, strikePrice, exchange);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode UsEquityOptionHelper::Serialize(std::string &symbol, const std::string &root, const uint8_t expirationYear, const char expirationMonth, const uint8_t expirationDay, const Rational &strikePrice, const std::string &exchange)
{
	char symbolBuffer[MAX_SYMBOL_LENGTH];
	size_t symbolLength = MAX_SYMBOL_LENGTH;

	ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, Serialize(symbolBuffer, symbolLength, root, expirationYear, expirationMonth, expirationDay, strikePrice, exchange));

	symbol.assign(symbolBuffer, symbolLength);

	return STATUS_CODE_SUCCESS;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode UsEquityOptionHelper::Serialize(std::string &symbol, const std::string &root, const uint32_t expirationYear, const uint8_t expirationMonth, const uint8_t expirationDay, const OptionType optionType, const Rational &strikePrice, const std::string &exchange)
{
	char expirationMonthCode = expirationMonth + ((OPTION_TYPE_CALL == optionType) ? CALL_MONTH_OFFSET : PUT_MONTH_OFFSET);

	return Serialize(symbol, root, expirationYear % 100, expirationMonthCode, expirationDay, strikePrice, exchange);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode UsEquityOptionHelper::Serialize(std::string &symbol, const std::string &root, const Date &expirationDate, const OptionType optionType, const Rational &strikePrice, const std::string &exchange)
{
	uint8_t expirationYear, expirationDay;
	char expirationMonth;

	ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, GetExpirationDateFromActivDateAndOptionType(expirationDate, optionType, expirationYear, expirationMonth, expirationDay));

	return Serialize(symbol, root, expirationYear, expirationMonth, expirationDay, strikePrice, exchange);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode UsEquityOptionHelper::Deserialize(const void * const pSymbol, const size_t symbolLength, std::string &root, uint8_t &expirationYear, char &expirationMonth, uint8_t &expirationDay, Rational &strikePrice, std::string &exchange)
{
	static const byte_t base64ToBinaryTable[256] =
	{
		UD,	UD,	UD,	UD,	UD,	UD,	UD,	UD,	UD,	UD,	UD,	UD,	UD,	UD,	UD,	UD,
		UD,	UD,	UD,	UD,	UD,	UD,	UD,	UD,	UD,	UD,	UD,	UD,	UD,	UD,	UD,	UD,
		UD,	UD,	UD,	UD,	UD,	UD,	UD,	UD,	UD,	UD,	UD,	UD,	UD,	UD,	UD,	UD,
		52,	53,	54,	55,	56,	57,	58,	59,	60,	61,	UD,	UD,	UD,	UD,	UD,	UD,
		UD,	0,	1,	2,	3,	4,	5,	6,	7,	8,	9,	10,	11,	12,	13,	14,
		15,	16,	17,	18,	19,	20,	21,	22,	23,	24,	25,	UD,	UD,	UD,	62,	UD,
		UD,	26,	27,	28,	29,	30,	31,	32,	33,	34,	35,	36,	37,	38,	39,	40,
		41,	42,	43,	44,	45,	46,	47,	48,	49,	50,	51,	UD,	UD,	UD,	63,	UD,
		UD,	UD,	UD,	UD,	UD,	UD,	UD,	UD,	UD,	UD,	UD,	UD,	UD,	UD,	UD,	UD,
		UD,	UD,	UD,	UD,	UD,	UD,	UD,	UD,	UD,	UD,	UD,	UD,	UD,	UD,	UD,	UD,
		UD,	UD,	UD,	UD,	UD,	UD,	UD,	UD,	UD,	UD,	UD,	UD,	UD,	UD,	UD,	UD,
		UD,	UD,	UD,	UD,	UD,	UD,	UD,	UD,	UD,	UD,	UD,	UD,	UD,	UD,	UD,	UD,
		UD,	UD,	UD,	UD,	UD,	UD,	UD,	UD,	UD,	UD,	UD,	UD,	UD,	UD,	UD,	UD,
		UD,	UD,	UD,	UD,	UD,	UD,	UD,	UD,	UD,	UD,	UD,	UD,	UD,	UD,	UD,	UD,
		UD,	UD,	UD,	UD,	UD,	UD,	UD,	UD,	UD,	UD,	UD,	UD,	UD,	UD,	UD,	UD,
		UD,	UD,	UD,	UD,	UD,	UD,	UD,	UD,	UD,	UD,	UD,	UD,	UD,	UD,	UD,	UD
	};

	if (symbolLength > GetMaxSerializedLength())
		return STATUS_CODE_FAILURE;

	const byte_t * const pBuffer = static_cast<const byte_t*>(pSymbol);
	const byte_t *pBlobStart = static_cast<const byte_t*>(::memchr(pBuffer, Feed::EXPIRATION_DATE_SEPARATOR, symbolLength));

	if (NULL == pBlobStart)
		return STATUS_CODE_NOT_FOUND;

	const size_t rootLength = pBlobStart - pBuffer;
	size_t remainingSymbolLength = symbolLength - rootLength;

	// Skip past the separator.
	++pBlobStart;
	--remainingSymbolLength;

	const byte_t *pBlobEnd = static_cast<const byte_t*>(::memchr(pBlobStart, Feed::EXCHANGE_SEPARATOR, remainingSymbolLength));

	if (NULL == pBlobEnd)
		return STATUS_CODE_FAILURE;

	const size_t blobLength = pBlobEnd - pBlobStart;
	remainingSymbolLength -= blobLength;

	if ((remainingSymbolLength < 2) || (blobLength < MIN_BLOB_LENGTH))
		return STATUS_CODE_FAILURE;

	root.assign(reinterpret_cast<const char*>(pBuffer), rootLength);
	exchange.assign(reinterpret_cast<const char*>(pBlobEnd + 1), remainingSymbolLength - 1);

	expirationYear = (base64ToBinaryTable[*pBlobStart++] << 1);
	expirationMonth = base64ToBinaryTable[*pBlobStart++];

	expirationYear |= (expirationMonth >> 5);
	expirationMonth = (expirationMonth & 0x1F) + ENCODED_MONTH_OFFSET;

	expirationDay = base64ToBinaryTable[*pBlobStart++];

	uint8_t encodedStrikePriceDenominator = (expirationDay >> 5);
	expirationDay &= 0x1F;

	uint32_t encodedStrikePriceNumerator = base64ToBinaryTable[*pBlobStart++];

	encodedStrikePriceDenominator |= ((encodedStrikePriceNumerator & 0x3) << 1);
	encodedStrikePriceNumerator >>= 2;
	size_t numeratorOffset = 4;

	while (pBlobStart < pBlobEnd)
	{
		encodedStrikePriceNumerator |= (static_cast<uint32_t>(base64ToBinaryTable[*pBlobStart++]) << numeratorOffset);
		numeratorOffset += 6;
	}

	return GetDecodedStrikePrice(strikePrice, encodedStrikePriceNumerator, static_cast<Denominator>(encodedStrikePriceDenominator));
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode UsEquityOptionHelper::Deserialize(const std::string &symbol, std::string &root, uint8_t &expirationYear, char &expirationMonth, uint8_t &expirationDay, Rational &strikePrice, std::string &exchange)
{
	return Deserialize(symbol.data(), symbol.length(), root, expirationYear, expirationMonth, expirationDay, strikePrice, exchange);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode UsEquityOptionHelper::Deserialize(const std::string &symbol, std::string &root, Date &expirationDate, OptionType &optionType, Rational &strikePrice, std::string &exchange)
{
	uint8_t expirationYear, expirationDay;
	char expirationMonth;

	ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, Deserialize(symbol.data(), symbol.length(), root, expirationYear, expirationMonth, expirationDay, strikePrice, exchange));

	return GetActivDateAndOptionTypeFromExpirationDate(expirationDate, optionType, expirationYear, expirationMonth, expirationDay);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline size_t UsEquityOptionHelper::GetMaxSerializedLength()
{
	return MAX_SYMBOL_LENGTH;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline size_t UsEquityOptionHelper::GetMaxSerializedLength(const size_t rootLength, const size_t exchangeLength)
{
	static const size_t BLOB_AND_SEPARATOR_LENGTH = MAX_BLOB_LENGTH + 2;

	return rootLength + exchangeLength + BLOB_AND_SEPARATOR_LENGTH;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline size_t UsEquityOptionHelper::GetMaxSerializedLength(const std::string &root, const std::string &exchange)
{
	return GetMaxSerializedLength(root.length(), exchange.length());
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode UsEquityOptionHelper::GetActivDateAndOptionTypeFromExpirationDate(Date &expirationDate, OptionType &optionType, const uint8_t expirationYear, const char expirationMonth, const uint8_t expirationDay)
{
	if (expirationMonth <= PUT_MONTH_OFFSET)
	{
		optionType = OPTION_TYPE_CALL;
		return expirationDate.Set(expirationYear + 2000, expirationMonth - CALL_MONTH_OFFSET, expirationDay);
	}
	else
	{
		optionType = OPTION_TYPE_PUT;
		return expirationDate.Set(expirationYear + 2000, expirationMonth - PUT_MONTH_OFFSET, expirationDay);
	}
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode UsEquityOptionHelper::GetExpirationDateFromActivDateAndOptionType(const Date &expirationDate, const OptionType optionType, uint8_t &expirationYear, char &expirationMonth, uint8_t &expirationDay)
{
	uint32_t year, month, day;

	ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, expirationDate.Get(year, month, day));

	expirationYear = static_cast<uint8_t>(year % 100);
	expirationMonth = static_cast<char>(month) + ((OPTION_TYPE_CALL == optionType) ? CALL_MONTH_OFFSET : PUT_MONTH_OFFSET);
	expirationDay = static_cast<uint8_t>(day);

	return STATUS_CODE_SUCCESS;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode UsEquityOptionHelper::BuildAliasSymbol(std::string &aliasSymbol, const std::string &root, const Date &expirationDate, const OptionType optionType, const Rational &strikePrice, const std::string &exchange)
{
	uint8_t expirationYear, expirationDay;
	char expirationMonth;

	ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, GetExpirationDateFromActivDateAndOptionType(expirationDate, optionType, expirationYear, expirationMonth, expirationDay));

	return BuildAliasSymbol(aliasSymbol, root, expirationYear, expirationMonth, expirationDay, strikePrice, exchange);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode UsEquityOptionHelper::BuildAliasSymbol(std::string &aliasSymbol, const std::string &root, const uint8_t expirationYear, const char expirationMonth, const uint8_t expirationDay, const Rational &strikePrice, const std::string &exchange)
{
	std::string aliasExpirationDate;
	ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, BuildAliasExpirationDate(aliasExpirationDate, expirationYear, expirationMonth, expirationDay));

	std::string aliasStrikePrice;
	ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, BuildAliasStrikePrice(aliasStrikePrice, strikePrice));

	aliasSymbol.reserve(MAX_ALIAS_SYMBOL_LENGTH);
	aliasSymbol.append(root);
	aliasSymbol.append(1, Feed::EXPIRATION_DATE_SEPARATOR);
	aliasSymbol.append(aliasExpirationDate);
	aliasSymbol.append(1, Feed::STRIKE_PRICE_SEPARATOR);
	aliasSymbol.append(aliasStrikePrice);
	aliasSymbol.append((expirationMonth <= PUT_MONTH_OFFSET) ? "C" : "P");
	aliasSymbol.append(1, Feed::EXCHANGE_SEPARATOR);
	aliasSymbol.append(exchange);

	return STATUS_CODE_SUCCESS;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode UsEquityOptionHelper::BuildAliasStrikePrice(std::string &aliasStrikePrice, const Rational &strikePrice)
{
	int64_t strikePriceNumerator;
	ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, strikePrice.GetNumerator(strikePriceNumerator, Rational::DENOMINATOR_3DP));

	if ((strikePriceNumerator < 0) || (strikePriceNumerator > MAX_ALIAS_STRIKE_PRICE))
		return STATUS_CODE_OUT_OF_RANGE;

	char buffer[ALIAS_STRIKE_PRICE_LENGTH + 1];
	size_t bufferLength = ALIAS_STRIKE_PRICE_LENGTH + 1;
	bufferLength = Snprintf(buffer, bufferLength, "%08u", strikePriceNumerator);

	aliasStrikePrice.assign(buffer, bufferLength);

	return STATUS_CODE_SUCCESS;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode UsEquityOptionHelper::BuildAliasExpirationDate(std::string &aliasExpirationDate, const Date &expirationDate)
{
	uint32_t year, month, day;

	ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, expirationDate.Get(year, month, day));

	char buffer[ALIAS_DATE_LENGTH + 1];
	size_t bufferLength = ALIAS_DATE_LENGTH + 1;
	bufferLength = Snprintf(buffer, bufferLength, "%02u%02u%02u", (year % 100), month, day);

	aliasExpirationDate.assign(buffer, bufferLength);

	return STATUS_CODE_SUCCESS;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode UsEquityOptionHelper::BuildAliasExpirationDate(std::string &aliasExpirationDate, const uint8_t expirationYear, const char expirationMonth, const uint8_t expirationDay)
{
	char buffer[ALIAS_DATE_LENGTH + 1];
	size_t bufferLength = ALIAS_DATE_LENGTH + 1;
	bufferLength = Snprintf(buffer, bufferLength, "%02u%02u%02u", expirationYear, expirationMonth - ((expirationMonth <= PUT_MONTH_OFFSET) ? CALL_MONTH_OFFSET : PUT_MONTH_OFFSET), expirationDay);

	aliasExpirationDate.assign(buffer, bufferLength);

	return STATUS_CODE_SUCCESS;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode UsEquityOptionHelper::GetSymbolFromOsiSymbol(std::string &symbol, const std::string &osiSymbol, const std::string &exchange)
{
	const char * const osiSymbolBuffer = osiSymbol.c_str();
	const size_t osiSymboLength = osiSymbol.length();

	if (OSI_SYMBOL_LENGTH != osiSymboLength)
		return STATUS_CODE_FAILURE;

	UsEquityOptionHelper usEquityOptionHelper;
	usEquityOptionHelper.m_root.assign(osiSymbolBuffer, std::min(::strcspn(osiSymbolBuffer, " "), OSI_ROOT_LENGTH));

	MessageValidator messageValidator(osiSymbolBuffer + OSI_ROOT_LENGTH, OSI_SYMBOL_LENGTH - OSI_ROOT_LENGTH);

	uint8_t expirationMonthNumber;
	ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, messageValidator.ValidateUnsignedAsciiIntegral(2, &usEquityOptionHelper.m_expirationYear));
	ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, messageValidator.ValidateUnsignedAsciiIntegral(2, &expirationMonthNumber));
	ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, messageValidator.ValidateUnsignedAsciiIntegral(2, &usEquityOptionHelper.m_expirationDay));

	char callPutIndicator;
	ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, messageValidator.ValidateBytesCopy(1, &callPutIndicator));

	if ('C' == callPutIndicator)
		usEquityOptionHelper.m_expirationMonth = expirationMonthNumber + CALL_MONTH_OFFSET;
	else if ('P' == callPutIndicator)
		usEquityOptionHelper.m_expirationMonth = expirationMonthNumber + PUT_MONTH_OFFSET;
	else
		return STATUS_CODE_FAILURE;

	uint64_t strikePriceNumerator;
	ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, messageValidator.ValidateUnsignedAsciiIntegral(8, &strikePriceNumerator));

	ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, usEquityOptionHelper.m_strikePrice.Set(strikePriceNumerator, Rational::DENOMINATOR_3DP, true));

	usEquityOptionHelper.m_exchange = exchange;

	return usEquityOptionHelper.Serialize(symbol);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode UsEquityOptionHelper::GetOsiSymbolFromSymbol(std::string &osiSymbol, const std::string &symbol)
{
	UsEquityOptionHelper usEquityOptionHelper(symbol);

	std::string osiExpirationDate;
	ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, usEquityOptionHelper.BuildAliasExpirationDate(osiExpirationDate));

	std::string osiStrikePrice;
	ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, usEquityOptionHelper.BuildAliasStrikePrice(osiStrikePrice));

	char rootBuffer[OSI_ROOT_LENGTH + 1];
	size_t offsetLength = 0;

	offsetLength = Snprintf(rootBuffer, OSI_ROOT_LENGTH + 1, "%-6.6s", usEquityOptionHelper.m_root.c_str());

	osiSymbol.reserve(OSI_SYMBOL_LENGTH);
	osiSymbol.append(rootBuffer, offsetLength);
	osiSymbol.append(osiExpirationDate);
	osiSymbol.append((usEquityOptionHelper.m_expirationMonth <= PUT_MONTH_OFFSET) ? "C" : "P");
	osiSymbol.append(osiStrikePrice);

	return STATUS_CODE_SUCCESS;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode UsEquityOptionHelper::GetEncodedStrikePrice(const uint32_t strikePriceNumerator, const Rational::Denominator strikePriceDenominator, uint32_t &encodedStrikePriceNumerator, Denominator &encodedStrikePriceDenominator)
{
	encodedStrikePriceNumerator = strikePriceNumerator;

	if ((strikePriceDenominator == Rational::DENOMINATOR_WHOLE) && (0 == (encodedStrikePriceNumerator % 5)))
	{
		encodedStrikePriceNumerator /= 5;
		encodedStrikePriceDenominator = UsEquityOptionHelper::DENOMINATOR_X5;
	}
	else
	{
		encodedStrikePriceDenominator = static_cast<UsEquityOptionHelper::Denominator>(strikePriceDenominator);
	}

	return STATUS_CODE_SUCCESS;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode UsEquityOptionHelper::GetDecodedStrikePrice(Rational &strikePrice, const uint32_t encodedStrikePriceNumerator, const Denominator encodedStrikePriceDenominator)
{
	uint32_t strikePriceNumerator = encodedStrikePriceNumerator;
	Rational::Denominator strikePriceDenominator;

	if (encodedStrikePriceDenominator == UsEquityOptionHelper::DENOMINATOR_X5)
	{
		strikePriceNumerator *= 5;
		strikePriceDenominator = Rational::DENOMINATOR_WHOLE;
	}
	else
	{
		strikePriceDenominator = static_cast<Rational::Denominator>(encodedStrikePriceDenominator);
	}

	return strikePrice.Set(strikePriceNumerator, strikePriceDenominator);
}

} // namespace Feed

} // namespace ContentPlatform

} // namespace Activ

#endif // (!defined ACTIV_FEED_COMMON_US_EQUITY_OPTION_HELPER_H)

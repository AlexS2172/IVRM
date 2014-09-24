/**
 *	@file	Enumerations.h
 *
 *	@brief	Header file for the enumerations.
 *
 *	$Log: $
 */

#if !defined(ACTIV_FEED_ENUMERATIONS_H)
#define ACTIV_FEED_ENUMERATIONS_H

#include "ActivMiddleware/Activ.h"

namespace Activ
{

namespace Feed
{

typedef uint16_t State;	///< The state type definition.

const State STATE_BIT_CLOSED = 0;
const State STATE_BIT_HALTED = 1;
const State STATE_BIT_RESTRICTED = 2;

const State STATE_CLOSED = (1 << ((sizeof(State) * CHAR_BIT) - (STATE_BIT_CLOSED + 1)));
const State STATE_HALTED = (1 << ((sizeof(State) * CHAR_BIT) - (STATE_BIT_HALTED + 1)));
const State STATE_RESTRICTED = (1 << ((sizeof(State) * CHAR_BIT) - (STATE_BIT_RESTRICTED + 1)));

const State STATE_NONE = 0;

typedef uint8_t CloseStatus;	///< The close status type definition.

const CloseStatus CLOSE_STATUS_NONE = 0;
const CloseStatus CLOSE_STATUS_CALCULATED_UNOFFICIAL = 1;
const CloseStatus CLOSE_STATUS_CALCULATED_OFFICIAL = 2;
const CloseStatus CLOSE_STATUS_EXCHANGE_UNOFFICIAL = 3;
const CloseStatus CLOSE_STATUS_EXCHANGE_OFFICIAL = 4;

typedef uint8_t ContractType;	/// The contract type type definition.

const ContractType CONTRACT_TYPE_NORMAL = 0;
const ContractType CONTACT_TYPE_ADJUSTMENT = 1;
const ContractType CONTACT_TYPE_SPINOFF = 2;
const ContractType CONTACT_TYPE_MERGER = 3;
const ContractType CONTACT_TYPE_PERCENTAGE = 4;

typedef uint32_t DividendFrequencyCode;	///< The dividend frequency code type definition.

const DividendFrequencyCode DIVIDEND_FREQUENCY_CODE_ANNUALLY = 0;
const DividendFrequencyCode DIVIDEND_FREQUENCY_CODE_MONTHLY = 1;
const DividendFrequencyCode DIVIDEND_FREQUENCY_CODE_QUARTERLY = 2;
const DividendFrequencyCode DIVIDEND_FREQUENCY_CODE_SEMI_ANNUALLY = 3;
const DividendFrequencyCode DIVIDEND_FREQUENCY_CODE_OTHER = 4;

typedef uint8_t DividendStatus;	///< The dividend status type definition.

const DividendStatus DIVIDEND_STATUS_UNOFFICIAL = 0;
const DividendStatus DIVIDEND_STATUS_OFFICIAL = 1;

typedef char ExerciseStyle;		///< The exercise style type definition.

const char EXERCISE_STYLE_AMERICAN = 'A';
const char EXERCISE_STYLE_EUROPEAN = 'E';

typedef uint8_t ExMarker;		///< The ex marker type definition.

const size_t EX_MARKER_EX_DIV_BYTE_OFFSET = 0;

const ExMarker EX_MARKER_NONE = 0;
const ExMarker EX_MARKER_TODAY = 1;

typedef char OptionType;		///< The option type type definition.

const OptionType OPTION_TYPE_CALL = 'C';
const OptionType OPTION_TYPE_PUT = 'P';

typedef char SettlementType;		///< The settlement type type definition.

const SettlementType SETTLEMENT_TYPE_AM = 'A';
const SettlementType SETTLEMENT_TYPE_PM = 'P';

typedef uint8_t UnitOfMeasure;	///< The unit of measure type definition.

const UnitOfMeasure UNIT_OF_MEASURE_NONE = 0;
const UnitOfMeasure UNIT_OF_MEASURE_NOMINAL = 1;
const UnitOfMeasure UNIT_OF_MEASURE_BAGS = 2;
const UnitOfMeasure UNIT_OF_MEASURE_BUSHELS = 3;
const UnitOfMeasure UNIT_OF_MEASURE_BARRELS = 4;
const UnitOfMeasure UNIT_OF_MEASURE_GALLONS = 5;
const UnitOfMeasure UNIT_OF_MEASURE_GRAMS = 6;
const UnitOfMeasure UNIT_OF_MEASURE_KILOS = 7;
const UnitOfMeasure UNIT_OF_MEASURE_SHORT_TONS = 8;
const UnitOfMeasure UNIT_OF_MEASURE_LONG_TONS = 9;
const UnitOfMeasure UNIT_OF_MEASURE_METRIC_TONS = 10;
const UnitOfMeasure UNIT_OF_MEASURE_OUNCES = 11;
const UnitOfMeasure UNIT_OF_MEASURE_POUNDS = 12;
const UnitOfMeasure UNIT_OF_MEASURE_SQUARE_FEET = 13;
const UnitOfMeasure UNIT_OF_MEASURE_FINE_TROY_OUNCES = 14;
const UnitOfMeasure UNIT_OF_MEASURE_TROY_OUNCES = 15;
const UnitOfMeasure UNIT_OF_MEASURE_BOARD_FEET = 16;
const UnitOfMeasure UNIT_OF_MEASURE_COINS = 17;
const UnitOfMeasure UNIT_OF_MEASURE_BALES = 18;
const UnitOfMeasure UNIT_OF_MEASURE_BOX_CASES = 19;
const UnitOfMeasure UNIT_OF_MEASURE_BOTTLE_FLASKS = 20;
const UnitOfMeasure UNIT_OF_MEASURE_PERCENT = 21;
const UnitOfMeasure UNIT_OF_MEASURE_NON_FAT_DRY_MILK = 22;
const UnitOfMeasure UNIT_OF_MEASURE_BTU_MILLIONS = 23;
const UnitOfMeasure UNIT_OF_MEASURE_MEGA_WATT_HOURS = 24;
const UnitOfMeasure UNIT_OF_MEASURE_HUNDRED_WEIGHT = 25;
const UnitOfMeasure UNIT_OF_MEASURE_HUNDRED_WEIGHT_112LBS = 26;
const UnitOfMeasure UNIT_OF_MEASURE_ONES = 27;
const UnitOfMeasure UNIT_OF_MEASURE_TENS = 28;
const UnitOfMeasure UNIT_OF_MEASURE_DOZENS = 29;
const UnitOfMeasure UNIT_OF_MEASURE_HUNDREDS = 30;
const UnitOfMeasure UNIT_OF_MEASURE_THOUSANDS = 31;
const UnitOfMeasure UNIT_OF_MEASURE_MILLIONS = 32;
const UnitOfMeasure UNIT_OF_MEASURE_PIECES = 33;
const UnitOfMeasure UNIT_OF_MEASURE_TEN_PIECES = 34;
const UnitOfMeasure UNIT_OF_MEASURE_HUNDRED_PIECES = 35;
const UnitOfMeasure UNIT_OF_MEASURE_THOUSAND_PIECES = 36;
const UnitOfMeasure UNIT_OF_MEASURE_TEN_THOUSAND_PIECES = 37;
const UnitOfMeasure UNIT_OF_MEASURE_INDICES = 38;
const UnitOfMeasure UNIT_OF_MEASURE_FUTURES_CONTRACTS = 39;
const UnitOfMeasure UNIT_OF_MEASURE_CURRENCY = 40;
const UnitOfMeasure UNIT_OF_MEASURE_SHARES = 41;

} // namespace Feed

} // namespace Activ

#endif // !defined(ACTIV_FEED_ENUMERATIONS_H)

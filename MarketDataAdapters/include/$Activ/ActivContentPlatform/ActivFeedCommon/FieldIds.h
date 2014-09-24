/**
 *	@file	FieldIds.h
 *
 *	@brief	Header file for the field ids.
 *
 *	$Log: $
 */

#if !defined(ACTIV_FEED_FIELD_IDS_H)
#define ACTIV_FEED_FIELD_IDS_H

#include "ActivMiddleware/Activ.h"

#include "ActivMiddleware/ActivBase/ActivBase.h"

#include "ActivMiddleware/ActivFieldTypes/ActivFieldTypes.h"

#include "ActivContentPlatform/ActivFeedCommon/ActivFeedDefinitions.h"

#include <vector>
#include <string>

namespace Activ
{

namespace Feed
{

const FieldId FID_UNDEFINED = ~0;

const FieldId FID_BID = 0;
const FieldId FID_BID_SIZE = 1;
const FieldId FID_BID_CONDITION = 2;
const FieldId FID_BID_TIME = 3;
const FieldId FID_BID_EXCHANGE = 4;
const FieldId FID_ASK = 5;
const FieldId FID_ASK_SIZE = 6;
const FieldId FID_ASK_CONDITION = 7;
const FieldId FID_ASK_TIME = 8;
const FieldId FID_ASK_EXCHANGE = 9;
const FieldId FID_MARKET_MAKER_STATUS = 10;
const FieldId FID_QUOTE_DATE = 11;
const FieldId FID_TRADE = 12;
const FieldId FID_TRADE_SIZE = 13;
const FieldId FID_TRADE_CONDITION = 14;
const FieldId FID_TRADE_TIME = 15;
const FieldId FID_TRADE_DATE = 16;
const FieldId FID_TRADE_EXCHANGE = 17;
const FieldId FID_TRADE_HIGH = 18;
const FieldId FID_TRADE_HIGH_CONDITION = 19;
const FieldId FID_TRADE_HIGH_TIME = 20;
const FieldId FID_TRADE_HIGH_EXCHANGE = 21;
const FieldId FID_TRADE_LOW = 22;
const FieldId FID_TRADE_LOW_CONDITION = 23;
const FieldId FID_TRADE_LOW_TIME = 24;
const FieldId FID_TRADE_LOW_EXCHANGE = 25;
const FieldId FID_TRADE_COUNT = 26;
const FieldId FID_CURRENT = 27;
const FieldId FID_CURRENT_CONDITION = 28;
const FieldId FID_CURRENT_TIME = 29;
const FieldId FID_CURRENT_DATE = 30;
const FieldId FID_CURRENT_HIGH = 31;
const FieldId FID_CURRENT_HIGH_TIME = 32;
const FieldId FID_CURRENT_LOW = 33;
const FieldId FID_CURRENT_LOW_TIME = 34;
const FieldId FID_OPEN = 35;
const FieldId FID_OPEN_SIZE = 36;
const FieldId FID_OPEN_CONDITION = 37;
const FieldId FID_OPEN_TIME = 38;
const FieldId FID_OPEN_EXCHANGE = 39;
const FieldId FID_FIRST_TRADE = 40;
const FieldId FID_FIRST_TRADE_TIME = 41;
const FieldId FID_ORDER_PRICE = 42;
const FieldId FID_ORDER_SIZE = 43;
const FieldId FID_ORDER_TIME = 44;
const FieldId FID_ORDER_DATE = 45;
const FieldId FID_ORDER_BUY_SELL = 46;
const FieldId FID_ORDER_SEQUENCE_NUMBER = 47;
const FieldId FID_ORDER_STATE = 48;
const FieldId FID_ORDER_TYPE = 49;
const FieldId FID_CUMULATIVE_PRICE = 50;
const FieldId FID_CUMULATIVE_VALUE = 51;
const FieldId FID_CUMULATIVE_VOLUME = 52;
const FieldId FID_PRE_POST_MARKET_CUMULATIVE_VOLUME = 53;
const FieldId FID_NET_CHANGE = 54;
const FieldId FID_PERCENT_CHANGE = 55;
const FieldId FID_CLOSING_BID = 56;
const FieldId FID_CLOSING_BID_EXCHANGE = 57;
const FieldId FID_CLOSING_ASK = 58;
const FieldId FID_CLOSING_ASK_EXCHANGE = 59;
const FieldId FID_CLOSING_QUOTE_DATE = 60;
const FieldId FID_CLOSE = 61;
const FieldId FID_CLOSE_CONDITION = 62;
const FieldId FID_CLOSE_STATUS = 63;
const FieldId FID_CLOSE_TIME = 64;
const FieldId FID_CLOSE_DATE = 65;
const FieldId FID_CLOSE_EXCHANGE = 66;
const FieldId FID_CLOSE_CUMULATIVE_VOLUME = 67;
const FieldId FID_CLOSE_CUMULATIVE_VOLUME_STATUS = 68;
const FieldId FID_CLOSE_CUMULATIVE_VOLUME_DATE = 69;
const FieldId FID_CLOSE_CUMULATIVE_VALUE = 70;
const FieldId FID_CLOSE_CUMULATIVE_VALUE_STATUS = 71;
const FieldId FID_COMPOSITE_CLOSE = 72;
const FieldId FID_COMPOSITE_CLOSE_DATE = 73;
const FieldId FID_COMPOSITE_CLOSE_EXCHANGE = 74;
const FieldId FID_SETTLEMENT = 75;
const FieldId FID_SETTLEMENT_SIZE = 76;
const FieldId FID_SETTLEMENT_STATUS = 77;
const FieldId FID_SETTLEMENT_TIME = 78;
const FieldId FID_SETTLEMENT_DATE = 79;
const FieldId FID_BID1 = 80;
const FieldId FID_BID1_TIME = 81;
const FieldId FID_BID1_SIZE = 82;
const FieldId FID_BID1_COUNT = 83;
const FieldId FID_ASK1 = 84;
const FieldId FID_ASK1_TIME = 85;
const FieldId FID_ASK1_SIZE = 86;
const FieldId FID_ASK1_COUNT = 87;
const FieldId FID_BID2 = 88;
const FieldId FID_BID2_COUNT = 89;
const FieldId FID_BID2_SIZE = 90;
const FieldId FID_BID3 = 91;
const FieldId FID_ASK2 = 92;
const FieldId FID_ASK2_COUNT = 93;
const FieldId FID_ASK2_SIZE = 94;
const FieldId FID_ASK3 = 95;
const FieldId FID_BID3_SIZE = 96;
const FieldId FID_BID4 = 97;
const FieldId FID_BID3_COUNT = 98;
const FieldId FID_BID4_SIZE = 99;
const FieldId FID_ASK3_SIZE = 100;
const FieldId FID_ASK4 = 101;
const FieldId FID_ASK3_COUNT = 102;
const FieldId FID_ASK4_SIZE = 103;
const FieldId FID_BID4_COUNT = 104;
const FieldId FID_BID5_SIZE = 105;
const FieldId FID_BID5 = 106;
const FieldId FID_BID5_COUNT = 107;
const FieldId FID_ASK4_COUNT = 108;
const FieldId FID_ASK5_SIZE = 109;
const FieldId FID_ASK5 = 110;
const FieldId FID_ASK5_COUNT = 111;
const FieldId FID_PREVIOUS_TRADING_DATE = 112;
const FieldId FID_PREVIOUS_CLOSE = 113;
const FieldId FID_PREVIOUS_OPEN = 114;
const FieldId FID_PREVIOUS_TRADE_HIGH = 115;
const FieldId FID_PREVIOUS_TRADE_LOW = 116;
const FieldId FID_PREVIOUS_CURRENT_HIGH = 117;
const FieldId FID_PREVIOUS_CURRENT_LOW = 118;
const FieldId FID_PREVIOUS_NET_CHANGE = 119;
const FieldId FID_PREVIOUS_PERCENT_CHANGE = 120;
const FieldId FID_PREVIOUS_OPEN_INTEREST = 121;
const FieldId FID_PREVIOUS_OPEN_INTEREST_DATE = 122;
const FieldId FID_OPEN_RANGE_1 = 123;
const FieldId FID_OPEN_RANGE_1_CONDITION = 124;
const FieldId FID_OPEN_RANGE_2 = 125;
const FieldId FID_OPEN_RANGE_2_CONDITION = 126;
const FieldId FID_OPEN_RANGE_TIME = 127;
const FieldId FID_CLOSE_RANGE_1 = 128;
const FieldId FID_CLOSE_RANGE_1_CONDITION = 129;
const FieldId FID_CLOSE_RANGE_2 = 130;
const FieldId FID_CLOSE_RANGE_2_CONDITION = 131;
const FieldId FID_CLOSE_RANGE_TIME = 132;
const FieldId FID_LOCAL_PRICE = 133;
const FieldId FID_OPEN_INTEREST = 134;
const FieldId FID_OPEN_INTEREST_DATE = 135;
const FieldId FID_MARKET_PRICE = 136;
const FieldId FID_MARKET_PRICE_DATE = 137;
const FieldId FID_MARKET_PRICE_TIME = 138;
const FieldId FID_OFFER = 139;
const FieldId FID_OFFER_DATE = 140;
const FieldId FID_OFFER_TIME = 141;
const FieldId FID_NAV = 142;
const FieldId FID_NAV_TIME = 143;
const FieldId FID_NAV_DATE = 144;
const FieldId FID_WRAP = 145;
const FieldId FID_WRAP_TIME = 146;
const FieldId FID_WRAP_DATE = 147;
const FieldId FID_52_WEEK_HIGH = 148;
const FieldId FID_52_WEEK_HIGH_DATE = 149;
const FieldId FID_52_WEEK_LOW = 150;
const FieldId FID_52_WEEK_LOW_DATE = 151;
const FieldId FID_YEAR_HIGH = 152;
const FieldId FID_YEAR_HIGH_DATE = 153;
const FieldId FID_YEAR_LOW = 154;
const FieldId FID_YEAR_LOW_DATE = 155;
const FieldId FID_LIFETIME_HIGH = 156;
const FieldId FID_LIFETIME_HIGH_DATE = 157;
const FieldId FID_LIFETIME_LOW = 158;
const FieldId FID_LIFETIME_LOW_DATE = 159;
const FieldId FID_SESSION_1_CLOSE = 160;
const FieldId FID_SESSION_1_CLOSE_STATUS = 161;
const FieldId FID_SESSION_1_CLOSE_TIME = 162;
const FieldId FID_SESSION_1_CLOSE_CUMULATIVE_VALUE = 163;
const FieldId FID_SESSION_1_CLOSE_CUMULATIVE_VALUE_STATUS = 164;
const FieldId FID_SESSION_1_CLOSE_CUMULATIVE_VOLUME = 165;
const FieldId FID_SESSION_1_CLOSE_CUMULATIVE_VOLUME_STATUS = 166;
const FieldId FID_SESSION_1_CLOSING_ASK = 167;
const FieldId FID_SESSION_1_CLOSING_BID = 168;
const FieldId FID_SESSION_1_CURRENT_HIGH = 169;
const FieldId FID_SESSION_1_CURRENT_HIGH_TIME = 170;
const FieldId FID_SESSION_1_CURRENT_LOW = 171;
const FieldId FID_SESSION_1_CURRENT_LOW_TIME = 172;
const FieldId FID_SESSION_1_FIRST_TRADE = 173;
const FieldId FID_SESSION_1_FIRST_TRADE_TIME = 174;
const FieldId FID_SESSION_1_INDICATOR = 175;
const FieldId FID_SESSION_1_OPEN = 176;
const FieldId FID_SESSION_1_OPEN_TIME = 177;
const FieldId FID_SESSION_1_TRADE_HIGH = 178;
const FieldId FID_SESSION_1_TRADE_HIGH_TIME = 179;
const FieldId FID_SESSION_1_TRADE_LOW = 180;
const FieldId FID_SESSION_1_TRADE_LOW_TIME = 181;
const FieldId FID_SESSION_2_CLOSE = 182;
const FieldId FID_SESSION_2_CLOSE_STATUS = 183;
const FieldId FID_SESSION_2_CLOSE_TIME = 184;
const FieldId FID_SESSION_2_CLOSE_CUMULATIVE_VALUE = 185;
const FieldId FID_SESSION_2_CLOSE_CUMULATIVE_VALUE_STATUS = 186;
const FieldId FID_SESSION_2_CLOSE_CUMULATIVE_VOLUME = 187;
const FieldId FID_SESSION_2_CLOSE_CUMULATIVE_VOLUME_STATUS = 188;
const FieldId FID_SESSION_2_CLOSING_ASK = 189;
const FieldId FID_SESSION_2_CLOSING_BID = 190;
const FieldId FID_SESSION_2_CURRENT_HIGH = 191;
const FieldId FID_SESSION_2_CURRENT_HIGH_TIME = 192;
const FieldId FID_SESSION_2_CURRENT_LOW = 193;
const FieldId FID_SESSION_2_CURRENT_LOW_TIME = 194;
const FieldId FID_SESSION_2_FIRST_TRADE = 195;
const FieldId FID_SESSION_2_FIRST_TRADE_TIME = 196;
const FieldId FID_SESSION_2_INDICATOR = 197;
const FieldId FID_SESSION_2_OPEN = 198;
const FieldId FID_SESSION_2_OPEN_TIME = 199;
const FieldId FID_SESSION_2_TRADE_HIGH = 200;
const FieldId FID_SESSION_2_TRADE_HIGH_TIME = 201;
const FieldId FID_SESSION_2_TRADE_LOW = 202;
const FieldId FID_SESSION_2_TRADE_LOW_TIME = 203;
const FieldId FID_SESSION_3_CLOSE = 204;
const FieldId FID_SESSION_3_CLOSE_STATUS = 205;
const FieldId FID_SESSION_3_CLOSE_TIME = 206;
const FieldId FID_SESSION_3_CLOSE_CUMULATIVE_VALUE = 207;
const FieldId FID_SESSION_3_CLOSE_CUMULATIVE_VALUE_STATUS = 208;
const FieldId FID_SESSION_3_CLOSE_CUMULATIVE_VOLUME = 209;
const FieldId FID_SESSION_3_CLOSE_CUMULATIVE_VOLUME_STATUS = 210;
const FieldId FID_SESSION_3_CLOSING_ASK = 211;
const FieldId FID_SESSION_3_CLOSING_BID = 212;
const FieldId FID_SESSION_3_CURRENT_HIGH = 213;
const FieldId FID_SESSION_3_CURRENT_HIGH_TIME = 214;
const FieldId FID_SESSION_3_CURRENT_LOW = 215;
const FieldId FID_SESSION_3_CURRENT_LOW_TIME = 216;
const FieldId FID_SESSION_3_FIRST_TRADE = 217;
const FieldId FID_SESSION_3_FIRST_TRADE_TIME = 218;
const FieldId FID_SESSION_3_INDICATOR = 219;
const FieldId FID_SESSION_3_OPEN = 220;
const FieldId FID_SESSION_3_OPEN_TIME = 221;
const FieldId FID_SESSION_3_TRADE_HIGH = 222;
const FieldId FID_SESSION_3_TRADE_HIGH_TIME = 223;
const FieldId FID_SESSION_3_TRADE_LOW = 224;
const FieldId FID_SESSION_3_TRADE_LOW_TIME = 225;
const FieldId FID_7_DAY_YIELD = 226;
const FieldId FID_ACCRUED_INTEREST = 227;
const FieldId FID_ADDITIONAL_PAID_IN_CAPITAL = 228;
const FieldId FID_ANNUALIZED_7_DAY_YIELD = 229;
const FieldId FID_ASSETS = 230;
const FieldId FID_ATM_CALL_IMPLIED_VOLATILITY = 231;
const FieldId FID_ATM_PUT_IMPLIED_VOLATILITY = 232;
const FieldId FID_AVERAGE_MATURITY = 233;
const FieldId FID_AVERAGE_VOLUME = 234;
const FieldId FID_AVERAGE_VOLUME_100DAY = 235;
const FieldId FID_AVERAGE_VOLUME_22DAY = 236;
const FieldId FID_AVERAGE_VOLUME_250DAY = 237;
const FieldId FID_BETA = 238;
const FieldId FID_CAP_GAINS_DATE = 239;
const FieldId FID_CAP_GAINS_LONG = 240;
const FieldId FID_CAP_GAINS_OTHER = 241;
const FieldId FID_CAP_GAINS_SHORT = 242;
const FieldId FID_CAPITAL_DISTRIBUTION = 243;
const FieldId FID_CAPITAL_STOCK = 244;
const FieldId FID_CASH_FROM_FINANCING = 245;
const FieldId FID_CASH_FROM_INVESTMENTS = 246;
const FieldId FID_CASH_FROM_OPERATIONS = 247;
const FieldId FID_CASH_IN_LIEU_OF = 248;
const FieldId FID_CASH_NET_CHANGE = 249;
const FieldId FID_CASH_PER_CONTRACT = 250;
const FieldId FID_CLOSING_PRICE_MONTH = 251;
const FieldId FID_CLOSING_PRICE_QUARTER = 252;
const FieldId FID_CLOSING_PRICE_WEEK = 253;
const FieldId FID_CLOSING_PRICE_YEAR = 254;
const FieldId FID_COMMENTS = 255;
const FieldId FID_CONDITIONS = 256;
const FieldId FID_CONTRACT_MULTIPLIER = 257;
const FieldId FID_CONTRACT_TYPE = 258;
const FieldId FID_COUNTRY_CODE = 259;
const FieldId FID_CURRENCY = 260;
const FieldId FID_CURRENT_ASSETS = 261;
const FieldId FID_CURRENT_LIABILITIES = 262;
const FieldId FID_CURRENT_YIELD = 263;
const FieldId FID_DATE = 264;
const FieldId FID_DECLARE_DATE = 265;
const FieldId FID_DEPRECIATION_DEPLETION = 266;
const FieldId FID_DESCRIPTION = 267;
const FieldId FID_DISPLAY_DENOMINATOR = 268;
const FieldId FID_DIVIDEND = 269;
const FieldId FID_DIVIDEND_CURRENCY = 270;
const FieldId FID_DIVIDEND_FREQUENCY_CODE = 271;
const FieldId FID_DOWN = 272;
const FieldId FID_EMPLOYEES = 273;
const FieldId FID_ESTIMATED_CASH_AMOUNT_PER_UNIT = 274;
const FieldId FID_ESTIMATED_LONG_TERM_RETURN = 275;
const FieldId FID_EVENT_DATE = 276;
const FieldId FID_EX_DATE = 277;
const FieldId FID_EXCHANGE = 278;
const FieldId FID_EXERCISE_STYLE = 279;
const FieldId FID_EXPIRATION_DATE = 280;
const FieldId FID_EXPIRATION_TIME = 281;
const FieldId FID_FISCAL_YEAR_END = 282;
const FieldId FID_FOOTNOTES = 283;
const FieldId FID_GROSS_PROFITS = 284;
const FieldId FID_HIGHEST_SPREAD = 285;
const FieldId FID_INCOME_BEFORE_TAX = 286;
const FieldId FID_INCOME_DISTRIBUTION = 287;
const FieldId FID_INDICATED_ANNUAL_DIVIDEND = 288;
const FieldId FID_INDUSTRY_CODE = 289;
const FieldId FID_INSTITUTIONAL_SHAREHOLDERS = 290;
const FieldId FID_INTEREST_EXPENSES = 291;
const FieldId FID_INTEREST_INCOME = 292;
const FieldId FID_ISIN = 293;
const FieldId FID_ISSUER_NAME = 294;
const FieldId FID_LAST_12_MONTHS_EARNINGS = 295;
const FieldId FID_LAST_FISCAL_YEAR_EARNINGS = 296;
const FieldId FID_LAST_QUARTER_DATE = 297;
const FieldId FID_LAST_QUARTER_DATE_INCLUDED = 298;
const FieldId FID_LAST_QUARTER_EARNINGS = 299;
const FieldId FID_LAST_TRADING_DATE = 300;
const FieldId FID_LOCAL_CODE = 301;
const FieldId FID_LONG_TERM_DEBT = 302;
const FieldId FID_LONG_TERM_INVESTMENTS = 303;
const FieldId FID_LOWEST_SPREAD = 304;
const FieldId FID_MOVING_AVERAGE_100DAY = 305;
const FieldId FID_MOVING_AVERAGE_14DAY = 306;
const FieldId FID_MOVING_AVERAGE_21DAY = 307;
const FieldId FID_MOVING_AVERAGE_250DAY = 308;
const FieldId FID_MOVING_AVERAGE_50DAY = 309;
const FieldId FID_MOVING_AVERAGE_9DAY = 310;
const FieldId FID_NAME = 311;
const FieldId FID_NET_ACCRUED_DIVIDEND = 312;
const FieldId FID_NET_INCOME = 313;
const FieldId FID_NET_PROPERTY = 314;
const FieldId FID_NEXT_DIVIDEND_1 = 315;
const FieldId FID_NEXT_DIVIDEND_1_DATE = 316;
const FieldId FID_NEXT_DIVIDEND_1_STATUS = 317;
const FieldId FID_NEXT_DIVIDEND_2 = 318;
const FieldId FID_NEXT_DIVIDEND_2_DATE = 319;
const FieldId FID_NEXT_DIVIDEND_2_STATUS = 320;
const FieldId FID_NEXT_DIVIDEND_3 = 321;
const FieldId FID_NEXT_DIVIDEND_3_DATE = 322;
const FieldId FID_NEXT_DIVIDEND_3_STATUS = 323;
const FieldId FID_NEXT_DIVIDEND_4 = 324;
const FieldId FID_NEXT_DIVIDEND_4_DATE = 325;
const FieldId FID_NEXT_DIVIDEND_4_STATUS = 326;
const FieldId FID_NEXT_QUARTER_DATE = 327;
const FieldId FID_NUMBER_OF_QUARTERS_INCLUDED = 328;
const FieldId FID_OPTION_TYPE = 329;
const FieldId FID_OTHER_EXPENSES = 330;
const FieldId FID_OTHER_INCOME = 331;
const FieldId FID_PAR_VALUE_PER_UNIT = 332;
const FieldId FID_PAYMENT_DATE = 333;
const FieldId FID_PLANT_AND_EQUIPMENT = 334;
const FieldId FID_PREVIOUS_12_MONTHS_EARNINGS_PER_SHARE = 335;
const FieldId FID_PREVIOUS_7_QUARTERS_EARNINGS_PER_SHARE = 336;
const FieldId FID_PREVIOUS_CURRENT_DATE = 337;
const FieldId FID_PREVIOUS_DIVIDEND = 338;
const FieldId FID_PREVIOUS_DIVIDEND_DATE = 339;
const FieldId FID_PREVIOUS_FISCAL_YEAR_EARNINGS_PER_SHARE = 340;
const FieldId FID_PRIMARY_EXCHANGE = 341;
const FieldId FID_RATE_OF_RETURN_12MONTH = 342;
const FieldId FID_RATE_OF_RETURN_YEAR = 343;
const FieldId FID_RECORD_DATE = 344;
const FieldId FID_RETAINED_EARNINGS = 345;
const FieldId FID_RETURN_ON_CAPITAL = 346;
const FieldId FID_REVENUES_FROM_SALES = 347;
const FieldId FID_SELLING_AND_GENERAL_ADMIN_EXPENSES = 348;
const FieldId FID_SESSION_ID = 349;
const FieldId FID_SHAREHOLDERS = 350;
const FieldId FID_SHARES_AFTER = 351;
const FieldId FID_SHARES_BEFORE = 352;
const FieldId FID_SHARES_HELD_BY_INSTITUTIONS = 353;
const FieldId FID_SHARES_OUTSTANDING = 354;
const FieldId FID_SHARES_PER_CONTRACT = 355;
const FieldId FID_SIC_CODE = 356;
const FieldId FID_SPECIAL_DIVIDEND = 357;
const FieldId FID_SPECIAL_DIVIDEND_DATE = 358;
const FieldId FID_SPLIT_DATE = 359;
const FieldId FID_SPLIT_RATIO = 360;
const FieldId FID_STATE = 361;
const FieldId FID_STRIKE_PRICE = 362;
const FieldId FID_TICK_COUNT = 363;
const FieldId FID_TICK_PRICE = 364;
const FieldId FID_TICK_SIZE = 365;
const FieldId FID_TICK_TYPE = 366;
const FieldId FID_TIME = 367;
const FieldId FID_TOTAL_ASSETS = 368;
const FieldId FID_TOTAL_CASH = 369;
const FieldId FID_TOTAL_CASH_AMOUNT_PER_UNIT = 370;
const FieldId FID_UNALLOCATED = 371;
const FieldId FID_UNCHANGED = 372;
const FieldId FID_UP = 373;
const FieldId FID_VENDOR_CODE = 374;
const FieldId FID_VOLATILITY_01MONTH = 375;
const FieldId FID_VOLATILITY_02MONTH = 376;
const FieldId FID_VOLATILITY_03MONTH = 377;
const FieldId FID_VOLATILITY_04MONTH = 378;
const FieldId FID_VOLATILITY_05MONTH = 379;
const FieldId FID_VOLATILITY_06MONTH = 380;
const FieldId FID_VOLATILITY_07MONTH = 381;
const FieldId FID_VOLATILITY_08MONTH = 382;
const FieldId FID_VOLATILITY_09MONTH = 383;
const FieldId FID_VOLATILITY_10MONTH = 384;
const FieldId FID_VOLATILITY_11MONTH = 385;
const FieldId FID_VOLATILITY_12MONTH = 386;
const FieldId FID_VOLATILITY_20DAY = 387;
const FieldId FID_WEIGHT = 388;
const FieldId FID_YEAR_END_DATE = 389;
const FieldId FID_COMPANY_SYMBOL = 390;
const FieldId FID_COMPANY_TABLE_NO = 391;
const FieldId FID_CONSTITUENT_SYMBOL = 392;
const FieldId FID_CONSTITUENT_TABLE_NO = 393;
const FieldId FID_CONTEXT = 394;
const FieldId FID_CREATE_DATE = 395;
const FieldId FID_CUG_ID = 396;
const FieldId FID_DELAY_PERIOD = 397;
const FieldId FID_ENTITY_TYPE = 398;
const FieldId FID_EVENT_TYPE = 399;
const FieldId FID_FIELD_ID = 400;
const FieldId FID_FIELD_LENGTH = 401;
const FieldId FID_FIELD_ORDER = 402;
const FieldId FID_FIELD_TYPE = 403;
const FieldId FID_FUTURE_OPTION_ROOT_SYMBOL = 404;
const FieldId FID_FUTURE_OPTION_ROOT_TABLE_NO = 405;
const FieldId FID_FUTURE_ROOT_SYMBOL = 406;
const FieldId FID_FUTURE_ROOT_TABLE_NO = 407;
const FieldId FID_INDEX_NUMBER = 408;
const FieldId FID_INDEX_PROPERTIES = 409;
const FieldId FID_INDEX_SYMBOL = 410;
const FieldId FID_INDEX_TABLE_NO = 411;
const FieldId FID_INDEX_TYPE = 412;
const FieldId FID_LAST_UPDATE_DATE = 413;
const FieldId FID_LAST_UPDATE_TIME = 414;
const FieldId FID_LISTING_SYMBOL = 415;
const FieldId FID_LISTING_SYMBOL_LIST = 416;
const FieldId FID_LISTING_TABLE_NO = 417;
const FieldId FID_MESSAGE_TYPE = 418;
const FieldId FID_NAVIGATION_INDEX_NO = 419;
const FieldId FID_NAVIGATION_KEY_FIELD_ID = 420;
const FieldId FID_NAVIGATION_TABLE_NO = 421;
const FieldId FID_NEW_DESCRIPTION = 422;
const FieldId FID_NEW_ISIN = 423;
const FieldId FID_NEW_LISTING_PERMISSION_ID = 424;
const FieldId FID_NEW_LISTING_SYMBOL_LIST = 425;
const FieldId FID_NEW_SECURITY_SYMBOL = 426;
const FieldId FID_OLD_DESCRIPTION = 427;
const FieldId FID_OLD_ISIN = 428;
const FieldId FID_OLD_LISTING_PERMISSION_ID = 429;
const FieldId FID_OLD_LISTING_SYMBOL_LIST = 430;
const FieldId FID_OLD_SECURITY_SYMBOL = 431;
const FieldId FID_OPTION_DELIVERABLE_SYMBOL = 432;
const FieldId FID_OPTION_DELIVERABLE_TABLE_NO = 433;
const FieldId FID_OPTION_ROOT_SYMBOL = 434;
const FieldId FID_OPTION_ROOT_TABLE_NO = 435;
const FieldId FID_OUTPUT_KEY_FIELD_ID = 436;
const FieldId FID_OUTPUT_PERMISSION_ID_FIELD_ID = 437;
const FieldId FID_OUTPUT_TABLE_NO_FIELD_ID = 438;
const FieldId FID_PASSWORD = 439;
const FieldId FID_PERMISSION_BODY = 440;
const FieldId FID_PERMISSION_ID = 441;
const FieldId FID_PERMISSION_ID_0 = 442;
const FieldId FID_PERMISSION_ID_1 = 443;
const FieldId FID_PERMISSION_LEVEL_ID = 444;
const FieldId FID_PREVIOUS_SYMBOL = 445;
const FieldId FID_PROVIDER_NAME = 446;
const FieldId FID_RECORD_STATUS = 447;
const FieldId FID_RELATIONSHIP_ID = 448;
const FieldId FID_RESET_DATE = 449;
const FieldId FID_RESET_TIME = 450;
const FieldId FID_SECTOR_SYMBOL = 451;
const FieldId FID_SECTOR_TABLE_NO = 452;
const FieldId FID_SECURITY_SYMBOL = 453;
const FieldId FID_SECURITY_TABLE_NO = 454;
const FieldId FID_SOURCE_CONTEXT = 455;
const FieldId FID_SYMBOL = 456;
const FieldId FID_SYMBOL01 = 457;
const FieldId FID_SYMBOL02 = 458;
const FieldId FID_SYMBOL03 = 459;
const FieldId FID_SYMBOL04 = 460;
const FieldId FID_SYMBOL05 = 461;
const FieldId FID_SYMBOL06 = 462;
const FieldId FID_SYMBOL07 = 463;
const FieldId FID_SYMBOL08 = 464;
const FieldId FID_SYMBOL09 = 465;
const FieldId FID_SYMBOL10 = 466;
const FieldId FID_SYMBOL11 = 467;
const FieldId FID_SYMBOL12 = 468;
const FieldId FID_SYMBOL13 = 469;
const FieldId FID_SYMBOL14 = 470;
const FieldId FID_SYMBOL15 = 471;
const FieldId FID_SYMBOL16 = 472;
const FieldId FID_SYMBOL17 = 473;
const FieldId FID_SYMBOL18 = 474;
const FieldId FID_SYMBOL19 = 475;
const FieldId FID_SYMBOL20 = 476;
const FieldId FID_TABLE_ID = 477;
const FieldId FID_TABLE_NO = 478;
const FieldId FID_TABLE_TYPE = 479;
const FieldId FID_TABLE_VERSION = 480;
const FieldId FID_UNDERLYING_SYMBOL = 481;
const FieldId FID_UNDERLYING_TABLE_NO = 482;
const FieldId FID_UPDATE_ID = 483;
const FieldId FID_USER_BODY = 484;
const FieldId FID_USER_ID = 485;
const FieldId FID_USERNAME = 486;
const FieldId FID_LIMIT_HIGH_PRICE = 487;
const FieldId FID_LIMIT_LOW_PRICE = 488;
const FieldId FID_CURRENT_YIELD_TIME = 489;
const FieldId FID_CURRENT_YIELD_DATE = 490;
const FieldId FID_DIVIDEND_REINVEST_PRICE = 491;
const FieldId FID_CAP_GAINS_REINVEST_PRICE = 492;
const FieldId FID_DIVIDEND_REINVEST_PRICE_DATE = 493;
const FieldId FID_CAP_GAINS_REINVEST_PRICE_DATE = 494;
const FieldId FID_NAV_HIGH = 495;
const FieldId FID_NAV_HIGH_TIME = 496;
const FieldId FID_NAV_LOW = 497;
const FieldId FID_NAV_LOW_TIME = 498;
const FieldId FID_GENERAL_BUSINESS_EXPENSES_PLUS_COMMISSIONS = 499;
const FieldId FID_INTEREST_REVENUE = 500;
const FieldId FID_INVESTMENT_SECURITIES = 501;
const FieldId FID_NET_INTEREST_INCOME = 502;
const FieldId FID_NET_PROPERTY_PLANT_AND_EQUIPMENT = 503;
const FieldId FID_NON_INTEREST_EXPENSES = 504;
const FieldId FID_PAR_VALUE_OF_COMMON_STOCK = 505;
const FieldId FID_PREMIUMS_EARNED = 506;
const FieldId FID_PREMIUMS_EARNED_LESS_EXPENSES = 507;
const FieldId FID_SECURITIES = 508;
const FieldId FID_TOTAL_INTEREST_INCOME = 509;
const FieldId FID_TOTAL_INVESTED_ASSETS = 510;
const FieldId FID_TOTAL_NON_INTEREST_INCOME = 511;
const FieldId FID_TOTAL_NON_PREMIUM_INCOME = 512;
const FieldId FID_PREVIOUS_SETTLEMENT = 513;
const FieldId FID_7_DAY_YIELD_DATE = 514;
const FieldId FID_ANNUALIZED_7_DAY_YIELD_DATE = 515;
const FieldId FID_PREVIOUS_CUMULATIVE_PRICE = 516;
const FieldId FID_EX_MARKER = 517;
const FieldId FID_NEW_SYMBOL_LIST = 518;
const FieldId FID_OLD_SYMBOL_LIST = 519;
const FieldId FID_COMPOSITE_EXCHANGE = 520;
const FieldId FID_ANALYTICS_SYMBOL = 521;
const FieldId FID_ANALYTICS_TABLE_NO = 522;
const FieldId FID_STORY_DATE_TIME = 523;
const FieldId FID_HEADLINE = 524;
const FieldId FID_STORY_BODY = 525;
const FieldId FID_REPORT_DATE = 526;
const FieldId FID_INCOME_FROM_CONTINUING_OPERATIONS = 527;
const FieldId FID_ANNUAL_REVENUES_FROM_SALES = 528;
const FieldId FID_ANNUAL_GROSS_PROFITS = 529;
const FieldId FID_ANNUAL_SELLING_AND_GENERAL_ADMIN_EXPENSE = 530;
const FieldId FID_ANNUAL_DEPRECIATION_DEPLETION = 531;
const FieldId FID_ANNUAL_INTEREST_EXPENSE = 532;
const FieldId FID_ANNUAL_INCOME_BEFORE_TAX = 533;
const FieldId FID_ANNUAL_INCOME_FROM_CONTINUING_OPERATIONS = 534;
const FieldId FID_ANNUAL_NET_INCOME = 535;
const FieldId FID_ANNUAL_CURRENT_ASSETS = 536;
const FieldId FID_ANNUAL_NET_PROPERTY_PLANT_AND_EQUIPMENT = 537;
const FieldId FID_ANNUAL_TOTAL_ASSETS = 538;
const FieldId FID_ANNUAL_CURRENT_LIABILITIES = 539;
const FieldId FID_ANNUAL_TOTAL_LIABILITIES = 540;
const FieldId FID_ANNUAL_LONG_TERM_DEBT = 541;
const FieldId FID_ANNUAL_CAPITAL_STOCK = 542;
const FieldId FID_ANNUAL_ADDITIONAL_PAID_IN_CAPITAL = 543;
const FieldId FID_ANNUAL_RETAINED_EARNINGS = 544;
const FieldId FID_TOTAL_LIABILITIES = 545;
const FieldId FID_INDICATED_YIELD = 546;
const FieldId FID_PARTICIPANT = 547;
const FieldId FID_TIME_SERIES_TYPE = 548;
const FieldId FID_DATE_TIME = 549;
const FieldId FID_TIME_SERIES_SYMBOL = 550;
const FieldId FID_TIME_SERIES_TABLE_NO = 551;
const FieldId FID_SOURCE_PERMISSION_ID = 552;
const FieldId FID_NUMBER_OF_CONCURRENT_USERS = 553;
const FieldId FID_TARGET_SYMBOL = 554;
const FieldId FID_TARGET_TABLE_NO = 555;
const FieldId FID_PRICE_ADJUSTMENT = 556;
const FieldId FID_COMPOSITE_FLAG = 557;
const FieldId FID_SYMBOL_LIST = 558;
const FieldId FID_EXPIRATION_DATE_01 = 559;
const FieldId FID_EXPIRATION_DATE_02 = 560;
const FieldId FID_EXPIRATION_DATE_03 = 561;
const FieldId FID_EXPIRATION_DATE_04 = 562;
const FieldId FID_EXPIRATION_DATE_05 = 563;
const FieldId FID_EXPIRATION_DATE_06 = 564;
const FieldId FID_EXPIRATION_DATE_07 = 565;
const FieldId FID_EXPIRATION_DATE_08 = 566;
const FieldId FID_EXPIRATION_DATE_09 = 567;
const FieldId FID_EXPIRATION_DATE_10 = 568;
const FieldId FID_EXPIRATION_DATE_11 = 569;
const FieldId FID_EXPIRATION_DATE_12 = 570;
const FieldId FID_STRIKE_PRICE_01 = 571;
const FieldId FID_STRIKE_PRICE_02 = 572;
const FieldId FID_STRIKE_PRICE_03 = 573;
const FieldId FID_STRIKE_PRICE_04 = 574;
const FieldId FID_STRIKE_PRICE_05 = 575;
const FieldId FID_STRIKE_PRICE_06 = 576;
const FieldId FID_STRIKE_PRICE_07 = 577;
const FieldId FID_STRIKE_PRICE_08 = 578;
const FieldId FID_STRIKE_PRICE_09 = 579;
const FieldId FID_STRIKE_PRICE_10 = 580;
const FieldId FID_STRIKE_PRICE_11 = 581;
const FieldId FID_STRIKE_PRICE_12 = 582;
const FieldId FID_STRIKE_PRICE_13 = 583;
const FieldId FID_STRIKE_PRICE_14 = 584;
const FieldId FID_STRIKE_PRICE_15 = 585;
const FieldId FID_STRIKE_PRICE_16 = 586;
const FieldId FID_STRIKE_PRICE_17 = 587;
const FieldId FID_STRIKE_PRICE_18 = 588;
const FieldId FID_STRIKE_PRICE_19 = 589;
const FieldId FID_STRIKE_PRICE_20 = 590;
const FieldId FID_STRIKE_PRICE_21 = 591;
const FieldId FID_STRIKE_PRICE_22 = 592;
const FieldId FID_STRIKE_PRICE_23 = 593;
const FieldId FID_STRIKE_PRICE_24 = 594;
const FieldId FID_STRIKE_PRICE_25 = 595;
const FieldId FID_STRIKE_PRICE_26 = 596;
const FieldId FID_CONTRACT_SIZE = 597;
const FieldId FID_LOCAL_EXCHANGE = 598;
const FieldId FID_LOCAL_CALL_ROOT = 599;
const FieldId FID_LOCAL_PUT_ROOT = 600;
const FieldId FID_LOCAL_ROOT = 601;
const FieldId FID_MINIMUM_TICK = 602;
const FieldId FID_TRADING_DAY_START_TIME = 603;
const FieldId FID_TRADING_DAY_END_TIME = 604;
const FieldId FID_TRADING_DAYS_OF_WEEK = 605;
const FieldId FID_UNIT_OF_MEASURE = 606;
const FieldId FID_FUTURE_SYMBOL = 607;
const FieldId FID_FUTURE_TABLE_NO = 608;
const FieldId FID_FUTURE_OPTION_SYMBOL = 609;
const FieldId FID_FUTURE_OPTION_TABLE_NO = 610;
const FieldId FID_FUTURE_OPTION_EXPIRATION_SYMBOL = 611;
const FieldId FID_FUTURE_OPTION_EXPIRATION_TABLE_NO = 612;
const FieldId FID_HAS_DELIVERABLES = 613;
const FieldId FID_CATEGORY_CODE_LIST = 614;
const FieldId FID_UNIT_OF_MEASURE_CURRENCY = 615;
const FieldId FID_VOLUME_WEIGHTED_OPENING_PRICE = 616;
const FieldId FID_CONTRIBUTOR = 617;
const FieldId FID_BID6 = 618;
const FieldId FID_BID6_COUNT = 619;
const FieldId FID_BID6_SIZE = 620;
const FieldId FID_ASK6 = 621;
const FieldId FID_ASK6_COUNT = 622;
const FieldId FID_ASK6_SIZE = 623;
const FieldId FID_BID7 = 624;
const FieldId FID_BID7_COUNT = 625;
const FieldId FID_BID7_SIZE = 626;
const FieldId FID_ASK7 = 627;
const FieldId FID_ASK7_COUNT = 628;
const FieldId FID_ASK7_SIZE = 629;
const FieldId FID_BID8 = 630;
const FieldId FID_BID8_COUNT = 631;
const FieldId FID_BID8_SIZE = 632;
const FieldId FID_ASK8 = 633;
const FieldId FID_ASK8_COUNT = 634;
const FieldId FID_ASK8_SIZE = 635;
const FieldId FID_BID9 = 636;
const FieldId FID_BID9_COUNT = 637;
const FieldId FID_BID9_SIZE = 638;
const FieldId FID_ASK9 = 639;
const FieldId FID_ASK9_COUNT = 640;
const FieldId FID_ASK9_SIZE = 641;
const FieldId FID_BID10 = 642;
const FieldId FID_BID10_COUNT = 643;
const FieldId FID_BID10_SIZE = 644;
const FieldId FID_ASK10 = 645;
const FieldId FID_ASK10_COUNT = 646;
const FieldId FID_ASK10_SIZE = 647;
const FieldId FID_PE_RATIO = 648;
const FieldId FID_PERMISSION_ID_LIST = 649;

/**
 *@brief	Universal field helper class.
 */
class UniversalFieldHelper
{
public:
	/**
	 *	@brief	Default constructor.
	 */
	UniversalFieldHelper() :
		m_type(FIELD_TYPE_UNKNOWN), m_maxLength(0)
	{
	}

	/**
	 *	@brief	Constructor.
	 *
	 *	@param	pName the name of the field.
	 *	@param	pDescription a description of the field.
	 *	@param	type the type of the field.
	 */
	UniversalFieldHelper(const char * const pName, const char * const pDescription, const FieldType type, const size_t maxLength) :
		m_name(pName), m_description(pDescription), m_type(type), m_maxLength(maxLength)
	{
	}

	const std::string			m_name;				///< The name of the field.
	const std::string			m_description;		///< A description of the field.
	const Activ::FieldType		m_type;				///< The type of the field.
	const size_t				m_maxLength;		///< The maximum length.
};

const UniversalFieldHelper g_universalFieldHelperList[] =
{
	UniversalFieldHelper("Bid",	"Bid price",	FIELD_TYPE_TRATIONAL,	10),
	UniversalFieldHelper("BidSize",	"Bid size",	FIELD_TYPE_UINT,	4),
	UniversalFieldHelper("BidCondition",	"Bid condition",	FIELD_TYPE_BINARY_STRING,	4),
	UniversalFieldHelper("BidTime",	"Bid time",	FIELD_TYPE_TIME,	3),
	UniversalFieldHelper("BidExchange",	"Bid exchange",	FIELD_TYPE_TEXT_STRING,	2),
	UniversalFieldHelper("Ask",	"Ask price",	FIELD_TYPE_TRATIONAL,	10),
	UniversalFieldHelper("AskSize",	"Ask size",	FIELD_TYPE_UINT,	4),
	UniversalFieldHelper("AskCondition",	"Ask condition",	FIELD_TYPE_BINARY_STRING,	4),
	UniversalFieldHelper("AskTime",	"Ask time",	FIELD_TYPE_TIME,	3),
	UniversalFieldHelper("AskExchange",	"Ask exchange",	FIELD_TYPE_TEXT_STRING,	2),
	UniversalFieldHelper("MarketMakerStatus",	"Market maker status",	FIELD_TYPE_BINARY_STRING,	4),
	UniversalFieldHelper("QuoteDate",	"Quote date",	FIELD_TYPE_DATE,	3),
	UniversalFieldHelper("Trade",	"Trade price",	FIELD_TYPE_TRATIONAL,	10),
	UniversalFieldHelper("TradeSize",	"Trade size",	FIELD_TYPE_UINT,	4),
	UniversalFieldHelper("TradeCondition",	"Trade condition",	FIELD_TYPE_BINARY_STRING,	4),
	UniversalFieldHelper("TradeTime",	"Trade time",	FIELD_TYPE_TIME,	3),
	UniversalFieldHelper("TradeDate",	"Trade date",	FIELD_TYPE_DATE,	3),
	UniversalFieldHelper("TradeExchange",	"Trade exchange",	FIELD_TYPE_TEXT_STRING,	2),
	UniversalFieldHelper("TradeHigh",	"Trade high price",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("TradeHighCondition",	"Trade high condition",	FIELD_TYPE_BINARY_STRING,	4),
	UniversalFieldHelper("TradeHighTime",	"Trade high price time",	FIELD_TYPE_TIME,	3),
	UniversalFieldHelper("TradeHighExchange",	"Trade high price exchange",	FIELD_TYPE_TEXT_STRING,	2),
	UniversalFieldHelper("TradeLow",	"Trade low price",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("TradeLowCondition",	"Trade low condition",	FIELD_TYPE_BINARY_STRING,	4),
	UniversalFieldHelper("TradeLowTime",	"Trade low price time",	FIELD_TYPE_TIME,	3),
	UniversalFieldHelper("TradeLowExchange",	"Trade low price exchange",	FIELD_TYPE_TEXT_STRING,	2),
	UniversalFieldHelper("TradeCount",	"Trade count",	FIELD_TYPE_UINT,	4),
	UniversalFieldHelper("Current",	"Current price",	FIELD_TYPE_TRATIONAL,	10),
	UniversalFieldHelper("CurrentCondition",	"Current condition",	FIELD_TYPE_BINARY_STRING,	4),
	UniversalFieldHelper("CurrentTime",	"Current time",	FIELD_TYPE_TIME,	3),
	UniversalFieldHelper("CurrentDate",	"Current date",	FIELD_TYPE_DATE,	3),
	UniversalFieldHelper("CurrentHigh",	"Current high price",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("CurrentHighTime",	"Current high price time",	FIELD_TYPE_TIME,	3),
	UniversalFieldHelper("CurrentLow",	"Current low price",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("CurrentLowTime",	"Current low price time",	FIELD_TYPE_TIME,	3),
	UniversalFieldHelper("Open",	"Open price",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("OpenSize",	"Open price size",	FIELD_TYPE_UINT,	4),
	UniversalFieldHelper("OpenCondition",	"Open condition",	FIELD_TYPE_BINARY_STRING,	4),
	UniversalFieldHelper("OpenTime",	"Open price time",	FIELD_TYPE_TIME,	3),
	UniversalFieldHelper("OpenExchange",	"Open price exchange",	FIELD_TYPE_TEXT_STRING,	2),
	UniversalFieldHelper("FirstTrade",	"First trade",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("FirstTradeTime",	"First trade time",	FIELD_TYPE_TIME,	3),
	UniversalFieldHelper("OrderPrice",	"Limit price",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("OrderSize",	"Order size",	FIELD_TYPE_UINT,	4),
	UniversalFieldHelper("OrderTime",	"Order time",	FIELD_TYPE_TIME,	3),
	UniversalFieldHelper("OrderDate",	"Order date",	FIELD_TYPE_DATE,	3),
	UniversalFieldHelper("OrderBuySell",	"Buy or sell",	FIELD_TYPE_TEXT_ARRAY,	1),
	UniversalFieldHelper("OrderSequenceNumber",	"Order sequence number",	FIELD_TYPE_UINT,	4),
	UniversalFieldHelper("OrderState",	"Order state",	FIELD_TYPE_UINT,	4),
	UniversalFieldHelper("OrderType",	"Order type (short sale, limit market, at open, at close, etc.)",	FIELD_TYPE_UINT,	1),
	UniversalFieldHelper("CumulativePrice",	"Cumulative price",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("CumulativeValue",	"Cumulative value",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("CumulativeVolume",	"Cumulative volume",	FIELD_TYPE_UINT,	4),
	UniversalFieldHelper("PrePostMarketCumulativeVolume",	"Pre/post-market cumulative volume",	FIELD_TYPE_UINT,	4),
	UniversalFieldHelper("NetChange",	"Net change",	FIELD_TYPE_TRATIONAL,	6),
	UniversalFieldHelper("PercentChange",	"Percent change",	FIELD_TYPE_TRATIONAL,	6),
	UniversalFieldHelper("ClosingBid",	"Closing bid price",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("ClosingBidExchange",	"Closing bid price exchange",	FIELD_TYPE_TEXT_STRING,	2),
	UniversalFieldHelper("ClosingAsk",	"Closing ask price",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("ClosingAskExchange",	"Closing ask price exchange",	FIELD_TYPE_TEXT_STRING,	2),
	UniversalFieldHelper("ClosingQuoteDate",	"Closing quote date",	FIELD_TYPE_DATE,	3),
	UniversalFieldHelper("Close",	"Closing price",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("CloseCondition",	"Closing price condition",	FIELD_TYPE_BINARY_STRING,	4),
	UniversalFieldHelper("CloseStatus",	"Closing price status (exchange official, exchange unofficial, calculated official, calculated unofficial)",	FIELD_TYPE_UINT,	1),
	UniversalFieldHelper("CloseTime",	"Close time",	FIELD_TYPE_TIME,	3),
	UniversalFieldHelper("CloseDate",	"Close date",	FIELD_TYPE_DATE,	3),
	UniversalFieldHelper("CloseExchange",	"Closing price exchange",	FIELD_TYPE_TEXT_STRING,	2),
	UniversalFieldHelper("CloseCumulativeVolume",	"Closing cumulative volume",	FIELD_TYPE_UINT,	4),
	UniversalFieldHelper("CloseCumulativeVolumeStatus",	"Closing cumulative volume status (exchange official, exchange unofficial, calculated official, calculated unofficial)",	FIELD_TYPE_UINT,	1),
	UniversalFieldHelper("CloseCumulativeVolumeDate",	"Closing cumulative volume date",	FIELD_TYPE_DATE,	3),
	UniversalFieldHelper("CloseCumulativeValue",	"Closing cumulative value",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("CloseCumulativeValueStatus",	"Closing cumulative value status (exchange official, exchange unofficial, calculated official, calculated unofficial)",	FIELD_TYPE_UINT,	1),
	UniversalFieldHelper("CompositeClose",	"Composite close price",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("CompositeCloseDate",	"Composite close price date",	FIELD_TYPE_DATE,	3),
	UniversalFieldHelper("CompositeCloseExchange",	"Composite close price exchange",	FIELD_TYPE_TEXT_STRING,	2),
	UniversalFieldHelper("Settlement",	"Settlement price",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("SettlementSize",	"Settlement size",	FIELD_TYPE_UINT,	4),
	UniversalFieldHelper("SettlementStatus",	"Settlement price status (exchange official, exchange unofficial, calculated official, calculated unofficial)",	FIELD_TYPE_UINT,	1),
	UniversalFieldHelper("SettlementTime",	"Settlement time",	FIELD_TYPE_TIME,	3),
	UniversalFieldHelper("SettlementDate",	"Settlement date",	FIELD_TYPE_DATE,	3),
	UniversalFieldHelper("Bid1",	"Bid1",	FIELD_TYPE_TRATIONAL,	10),
	UniversalFieldHelper("Bid1Time",	"Bid1 time",	FIELD_TYPE_TIME,	3),
	UniversalFieldHelper("Bid1Size",	"Bid1 size",	FIELD_TYPE_UINT,	4),
	UniversalFieldHelper("Bid1Count",	"Bid1 count",	FIELD_TYPE_UINT,	2),
	UniversalFieldHelper("Ask1",	"Ask1 price",	FIELD_TYPE_TRATIONAL,	10),
	UniversalFieldHelper("Ask1Time",	"Ask1 time",	FIELD_TYPE_TIME,	3),
	UniversalFieldHelper("Ask1Size",	"Ask1 size",	FIELD_TYPE_UINT,	4),
	UniversalFieldHelper("Ask1Count",	"Ask1 count",	FIELD_TYPE_UINT,	2),
	UniversalFieldHelper("Bid2",	"Bid2 price",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("Bid2Count",	"Bid2 count",	FIELD_TYPE_UINT,	2),
	UniversalFieldHelper("Bid2Size",	"Bid2 size",	FIELD_TYPE_UINT,	4),
	UniversalFieldHelper("Bid3",	"Bid3 price",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("Ask2",	"Ask2 price",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("Ask2Count",	"Ask2 count",	FIELD_TYPE_UINT,	2),
	UniversalFieldHelper("Ask2Size",	"Ask2 size",	FIELD_TYPE_UINT,	4),
	UniversalFieldHelper("Ask3",	"Ask3 price",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("Bid3Size",	"Bid3 size",	FIELD_TYPE_UINT,	4),
	UniversalFieldHelper("Bid4",	"Bid4 price",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("Bid3Count",	"Bid3 count",	FIELD_TYPE_UINT,	2),
	UniversalFieldHelper("Bid4Size",	"Bid4 size",	FIELD_TYPE_UINT,	4),
	UniversalFieldHelper("Ask3Size",	"Ask3 size",	FIELD_TYPE_UINT,	4),
	UniversalFieldHelper("Ask4",	"Ask4 price",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("Ask3Count",	"Ask3 count",	FIELD_TYPE_UINT,	2),
	UniversalFieldHelper("Ask4Size",	"Ask4 size",	FIELD_TYPE_UINT,	4),
	UniversalFieldHelper("Bid4Count",	"Bid5 count",	FIELD_TYPE_UINT,	2),
	UniversalFieldHelper("Bid5Size",	"Bid5 size",	FIELD_TYPE_UINT,	4),
	UniversalFieldHelper("Bid5",	"Bid5 price",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("Bid5Count",	"Bid5 count",	FIELD_TYPE_UINT,	2),
	UniversalFieldHelper("Ask4Count",	"Ask4 count",	FIELD_TYPE_UINT,	2),
	UniversalFieldHelper("Ask5Size",	"Ask5 size",	FIELD_TYPE_UINT,	4),
	UniversalFieldHelper("Ask5",	"Ask5",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("Ask5Count",	"Ask5 count",	FIELD_TYPE_UINT,	2),
	UniversalFieldHelper("PreviousTradingDate",	"Previous trading date",	FIELD_TYPE_DATE,	3),
	UniversalFieldHelper("PreviousClose",	"Previous closing price",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("PreviousOpen",	"Previous open price",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("PreviousTradeHigh",	"Previous trade high",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("PreviousTradeLow",	"Previous trade low",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("PreviousCurrentHigh",	"Previous current high price",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("PreviousCurrentLow",	"Previous current low price",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("PreviousNetChange",	"Previous net change",	FIELD_TYPE_RATIONAL,	5),
	UniversalFieldHelper("PreviousPercentChange",	"Previous percent change",	FIELD_TYPE_RATIONAL,	5),
	UniversalFieldHelper("PreviousOpenInterest",	"Previous open interest",	FIELD_TYPE_UINT,	4),
	UniversalFieldHelper("PreviousOpenInterestDate",	"Previous open interest date",	FIELD_TYPE_DATE,	3),
	UniversalFieldHelper("OpenRange1",	"Open range price 1",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("OpenRange1Condition",	"Open range price 1 condition",	FIELD_TYPE_BINARY_STRING,	4),
	UniversalFieldHelper("OpenRange2",	"Open range price 2",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("OpenRange2Condition",	"Open range price 2 condition",	FIELD_TYPE_BINARY_STRING,	4),
	UniversalFieldHelper("OpenRangeTime",	"Open range time",	FIELD_TYPE_TIME,	3),
	UniversalFieldHelper("CloseRange1",	"Close range price 1",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("CloseRange1Condition",	"Close range price 1 condition",	FIELD_TYPE_BINARY_STRING,	4),
	UniversalFieldHelper("CloseRange2",	"Close range price 2",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("CloseRange2Condition",	"Close range price 2 condition",	FIELD_TYPE_BINARY_STRING,	4),
	UniversalFieldHelper("CloseRangeTime",	"Close range time",	FIELD_TYPE_TIME,	3),
	UniversalFieldHelper("LocalPrice",	"Local price",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("OpenInterest",	"Open interest",	FIELD_TYPE_UINT,	4),
	UniversalFieldHelper("OpenInterestDate",	"Open interest date",	FIELD_TYPE_DATE,	3),
	UniversalFieldHelper("MarketPrice",	"Market price",	FIELD_TYPE_TRATIONAL,	10),
	UniversalFieldHelper("MarketPriceDate",	"Market price date",	FIELD_TYPE_DATE,	3),
	UniversalFieldHelper("MarketPriceTime",	"Market price time",	FIELD_TYPE_TIME,	3),
	UniversalFieldHelper("Offer",	"Offer",	FIELD_TYPE_TRATIONAL,	10),
	UniversalFieldHelper("OfferDate",	"Offer date",	FIELD_TYPE_DATE,	3),
	UniversalFieldHelper("OfferTime",	"Offer time",	FIELD_TYPE_TIME,	3),
	UniversalFieldHelper("Nav",	"Net asset value",	FIELD_TYPE_TRATIONAL,	10),
	UniversalFieldHelper("NavTime",	"Net asset value time",	FIELD_TYPE_TIME,	3),
	UniversalFieldHelper("NavDate",	"Net asset value date",	FIELD_TYPE_DATE,	3),
	UniversalFieldHelper("Wrap",	"Wrap",	FIELD_TYPE_TRATIONAL,	10),
	UniversalFieldHelper("WrapTime",	"Wrap time",	FIELD_TYPE_TIME,	3),
	UniversalFieldHelper("WrapDate",	"Wrap date",	FIELD_TYPE_DATE,	3),
	UniversalFieldHelper("52WeekHigh",	"52 week high",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("52WeekHighDate",	"Date of 52 week high",	FIELD_TYPE_DATE,	3),
	UniversalFieldHelper("52WeekLow",	"52 week low",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("52WeekLowDate",	"Date of 52 week low",	FIELD_TYPE_DATE,	3),
	UniversalFieldHelper("YearHigh",	"Year High",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("YearHighDate",	"Year high date",	FIELD_TYPE_DATE,	3),
	UniversalFieldHelper("YearLow",	"Year low",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("YearLowDate",	"Year low date",	FIELD_TYPE_DATE,	3),
	UniversalFieldHelper("LifetimeHigh",	"Lifetime high",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("LifetimeHighDate",	"Date of lifetime high",	FIELD_TYPE_DATE,	3),
	UniversalFieldHelper("LifetimeLow",	"Lifetime low",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("LifetimeLowDate",	"Date of lifetime low",	FIELD_TYPE_DATE,	3),
	UniversalFieldHelper("Session1Close",	"Session 1 closing price",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("Session1CloseStatus",	"Session 1 closing price status (exchange official, exchange unofficial, calculated official, calculated unofficial)",	FIELD_TYPE_UINT,	1),
	UniversalFieldHelper("Session1CloseTime",	"Session 1 close time",	FIELD_TYPE_TIME,	3),
	UniversalFieldHelper("Session1CloseCumulativeValue",	"Session 1 closing cumulative value",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("Session1CloseCumulativeValueStatus",	"Session 1 closing cumulative value status (exchange official, exchange unofficial, calculated official, calculated unofficial)",	FIELD_TYPE_UINT,	1),
	UniversalFieldHelper("Session1CloseCumulativeVolume",	"Session 1 closing cumulative volume",	FIELD_TYPE_UINT,	4),
	UniversalFieldHelper("Session1CloseCumulativeVolumeStatus",	"Session 1 closing cumulative volume status (exchange official, exchange unofficial, calculated official, calculated unofficial)",	FIELD_TYPE_UINT,	1),
	UniversalFieldHelper("Session1ClosingAsk",	"Session 1 closing ask price",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("Session1ClosingBid",	"Session 1 closing bid price",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("Session1CurrentHigh",	"Session 1 current high price",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("Session1CurrentHighTime",	"Session 1 current high price time",	FIELD_TYPE_TIME,	3),
	UniversalFieldHelper("Session1CurrentLow",	"Session 1 current low price",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("Session1CurrentLowTime",	"Session 1 current low price time",	FIELD_TYPE_TIME,	3),
	UniversalFieldHelper("Session1FirstTrade",	"Session 1 first trade",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("Session1FirstTradeTime",	"Session 1 first trade time",	FIELD_TYPE_TIME,	3),
	UniversalFieldHelper("Session1Indicator",	"Session 1 indicator (morning, afternoon, day, evening, night)",	FIELD_TYPE_BINARY_ARRAY,	1),
	UniversalFieldHelper("Session1Open",	"Session 1 open price",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("Session1OpenTime",	"Session 1 open price time",	FIELD_TYPE_TIME,	3),
	UniversalFieldHelper("Session1TradeHigh",	"Session 1 trade high price",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("Session1TradeHighTime",	"Session 1 trade high price time",	FIELD_TYPE_TIME,	3),
	UniversalFieldHelper("Session1TradeLow",	"Session 1 trade low price",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("Session1TradeLowTime",	"Session 1 trade low price time",	FIELD_TYPE_TIME,	3),
	UniversalFieldHelper("Session2Close",	"Session 2 closing price",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("Session2CloseStatus",	"Session 2 closing price status (exchange official, exchange unofficial, calculated official, calculated unofficial)",	FIELD_TYPE_UINT,	1),
	UniversalFieldHelper("Session2CloseTime",	"Session 2 closing price time",	FIELD_TYPE_TIME,	3),
	UniversalFieldHelper("Session2CloseCumulativeValue",	"Session 2 closing cumulative value",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("Session2CloseCumulativeValueStatus",	"Session 2 closing cumulative value status (exchange official, exchange unofficial, calculated official, calculated unofficial)",	FIELD_TYPE_UINT,	1),
	UniversalFieldHelper("Session2CloseCumulativeVolume",	"Session 2 closing cumulative volume",	FIELD_TYPE_UINT,	4),
	UniversalFieldHelper("Session2CloseCumulativeVolumeStatus",	"Session 2 closing cumulative volume status (exchange official, exchange unofficial, calculated official, calculated unofficial)",	FIELD_TYPE_UINT,	1),
	UniversalFieldHelper("Session2ClosingAsk",	"Session 2 closing ask price",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("Session2ClosingBid",	"Session 2 closing bid price",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("Session2CurrentHigh",	"Session 2 current high price",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("Session2CurrentHighTime",	"Session 2 current high price time",	FIELD_TYPE_TIME,	3),
	UniversalFieldHelper("Session2CurrentLow",	"Session 2 current low price",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("Session2CurrentLowTime",	"Session 2 current low price time",	FIELD_TYPE_TIME,	3),
	UniversalFieldHelper("Session2FirstTrade",	"Session 2 first trade",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("Session2FirstTradeTime",	"Session 2 first trade time",	FIELD_TYPE_TIME,	3),
	UniversalFieldHelper("Session2Indicator",	"Session 2 indicator (morning, afternoon, day, evening, night)",	FIELD_TYPE_BINARY_ARRAY,	1),
	UniversalFieldHelper("Session2Open",	"Session 2 open price",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("Session2OpenTime",	"Session 2 open price time",	FIELD_TYPE_TIME,	3),
	UniversalFieldHelper("Session2TradeHigh",	"Session 2 trade high price",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("Session2TradeHighTime",	"Session 2 trade high price time",	FIELD_TYPE_TIME,	3),
	UniversalFieldHelper("Session2TradeLow",	"Session 2 trade low price",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("Session2TradeLowTime",	"Session 2 trade low price time",	FIELD_TYPE_TIME,	3),
	UniversalFieldHelper("Session3Close",	"Session 3 closing price",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("Session3CloseStatus",	"Session 3 closing price status (exchange official, exchange unofficial, calculated official, calculated unofficial)",	FIELD_TYPE_UINT,	1),
	UniversalFieldHelper("Session3CloseTime",	"Session 3 closing price time",	FIELD_TYPE_TIME,	3),
	UniversalFieldHelper("Session3CloseCumulativeValue",	"Session 3 closing cumulative value",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("Session3CloseCumulativeValueStatus",	"Session 3 closing cumulative value status (exchange official, exchange unofficial, calculated official, calculated unofficial)",	FIELD_TYPE_UINT,	1),
	UniversalFieldHelper("Session3CloseCumulativeVolume",	"Session 3 closing cumulative volume",	FIELD_TYPE_UINT,	4),
	UniversalFieldHelper("Session3CloseCumulativeVolumeStatus",	"Session 3 closing cumulative volume status (exchange official, exchange unofficial, calculated official, calculated unofficial)",	FIELD_TYPE_UINT,	1),
	UniversalFieldHelper("Session3ClosingAsk",	"Session 3 closing ask price",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("Session3ClosingBid",	"Session 3 closing bid price",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("Session3CurrentHigh",	"Session 3 current high price",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("Session3CurrentHighTime",	"Session 3 current high price time",	FIELD_TYPE_TIME,	3),
	UniversalFieldHelper("Session3CurrentLow",	"Session 3 current low price",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("Session3CurrentLowTime",	"Session 3 current low price time",	FIELD_TYPE_TIME,	3),
	UniversalFieldHelper("Session3FirstTrade",	"Session 3 first trade",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("Session3FirstTradeTime",	"Session 3 first trade time",	FIELD_TYPE_TIME,	3),
	UniversalFieldHelper("Session3Indicator",	"Session 3 indicator (morning, afternoon, day, evening, night)",	FIELD_TYPE_BINARY_ARRAY,	1),
	UniversalFieldHelper("Session3Open",	"Session 3 open price",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("Session3OpenTime",	"Session 3 open price time",	FIELD_TYPE_TIME,	3),
	UniversalFieldHelper("Session3TradeHigh",	"Session 3 trade high price",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("Session3TradeHighTime",	"Session 3 trade high price time",	FIELD_TYPE_TIME,	3),
	UniversalFieldHelper("Session3TradeLow",	"Session 3 trade low price",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("Session3TradeLowTime",	"Session 3 trade low price time",	FIELD_TYPE_TIME,	3),
	UniversalFieldHelper("7DayYield",	"7 day yield",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("AccruedInterest",	"Accrued interest",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("AdditionalPaidInCapital",	"Additional paid in capital",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("Annualized7DayYield",	"Annualized 7 day yield",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("Assets",	"Assets",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("AtmCallImpliedVolatility",	"At the money call implied volatility",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("AtmPutImpliedVolatility",	"At the money put implied volatility",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("AverageMaturity",	"Average maturity",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("AverageVolume",	"Average daily volume (100 or 200 day?)",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("AverageVolume100day",	"Average daily volume (100 day)",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("AverageVolume22day",	"Average daily volume (22 day)",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("AverageVolume250day",	"Average daily volume (250 day)",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("Beta",	"Beta relative to S&P 500",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("CapGainsDate",	"Cap gains date",	FIELD_TYPE_DATE,	3),
	UniversalFieldHelper("CapGainsLong",	"Long term capital gains",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("CapGainsOther",	"Other capital gains",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("CapGainsShort",	"Short term capital gains",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("CapitalDistribution",	"Capital distribution",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("CapitalStock",	"Capital stock",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("CashFromFinancing",	"Cash from financing",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("CashFromInvestments",	"Cash from investments",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("CashFromOperations",	"Cash from operations",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("CashInLieuOf",	"Cash in lieu of",	FIELD_TYPE_RATIONAL,	5),
	UniversalFieldHelper("CashNetChange",	"Cash net change",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("CashPerContract",	"Cash per contract",	FIELD_TYPE_RATIONAL,	5),
	UniversalFieldHelper("ClosingPriceMonth",	"Closing price for the month",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("ClosingPriceQuarter",	"Closing price for the quarter",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("ClosingPriceWeek",	"Closing price for the week",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("ClosingPriceYear",	"Closing price for the year",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("Comments",	"Comments",	FIELD_TYPE_TEXT_STRING,	64),
	UniversalFieldHelper("Conditions",	"Conditions",	FIELD_TYPE_BINARY_STRING,	4),
	UniversalFieldHelper("ContractMultiplier",	"Contract multiplier",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("ContractType",	"Contract type",	FIELD_TYPE_UINT,	1),
	UniversalFieldHelper("CountryCode",	"Country code",	FIELD_TYPE_TEXT_ARRAY,	2),
	UniversalFieldHelper("Currency",	"Currency",	FIELD_TYPE_TEXT_ARRAY,	3),
	UniversalFieldHelper("CurrentAssets",	"Current assets",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("CurrentLiabilities",	"Current liabilties",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("CurrentYield",	"Current yield",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("Date",	"Date",	FIELD_TYPE_DATE,	3),
	UniversalFieldHelper("DeclareDate",	"Declare date",	FIELD_TYPE_DATE,	3),
	UniversalFieldHelper("DepreciationDepletion",	"Depreciation / Depletion",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("Description",	"Description",	FIELD_TYPE_TEXT_STRING,	64),
	UniversalFieldHelper("DisplayDenominator",	"Display denominator",	FIELD_TYPE_UINT,	1),
	UniversalFieldHelper("Dividend",	"Current dividend amount",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("DividendCurrency",	"Dividend currency",	FIELD_TYPE_TEXT_ARRAY,	3),
	UniversalFieldHelper("DividendFrequencyCode",	"Dividend frequency code",	FIELD_TYPE_UINT,	4),
	UniversalFieldHelper("Down",	"Down",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("Employees",	"Employees",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("EstimatedCashAmountPerUnit",	"Estimated cash amount per unit",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("EstimatedLongTermReturn",	"Estimated long term return",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("EventDate",	"Event date",	FIELD_TYPE_DATE,	3),
	UniversalFieldHelper("ExDate",	"Ex dividend date",	FIELD_TYPE_DATE,	3),
	UniversalFieldHelper("Exchange",	"Exchange",	FIELD_TYPE_TEXT_STRING,	2),
	UniversalFieldHelper("ExerciseStyle",	"Exercise style (A-American/E-European)",	FIELD_TYPE_TEXT_ARRAY,	1),
	UniversalFieldHelper("ExpirationDate",	"Expiration date",	FIELD_TYPE_DATE,	3),
	UniversalFieldHelper("ExpirationTime",	"Expiration time",	FIELD_TYPE_TIME,	3),
	UniversalFieldHelper("FiscalYearEnd",	"Fiscal year end",	FIELD_TYPE_DATE,	3),
	UniversalFieldHelper("Footnotes",	"Footnotes",	FIELD_TYPE_TEXT_STRING,	5),
	UniversalFieldHelper("GrossProfits",	"Gross profits",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("HighestSpread",	"Highest spread",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("IncomeBeforeTax",	"Income before tax",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("IncomeDistribution",	"Income distribution",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("IndicatedAnnualDividend",	"Indicated annual dividend",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("IndustryCode",	"Industry code",	FIELD_TYPE_TEXT_STRING,	8),
	UniversalFieldHelper("InstitutionalShareholders",	"Institutional shareholders",	FIELD_TYPE_UINT,	4),
	UniversalFieldHelper("InterestExpenses",	"Interest expenses",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("InterestIncome",	"Interest income",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("Isin",	"ISIN",	FIELD_TYPE_TEXT_ARRAY,	12),
	UniversalFieldHelper("IssuerName",	"Issuer name",	FIELD_TYPE_TEXT_STRING,	64),
	UniversalFieldHelper("Last12MonthsEarnings",	"Last 12 months earnings",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("LastFiscalYearEarnings",	"Last fiscal year earnings",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("LastQuarterDate",	"Last quarter date",	FIELD_TYPE_DATE,	3),
	UniversalFieldHelper("LastQuarterDateIncluded",	"Last quarter dateI icluded",	FIELD_TYPE_DATE,	3),
	UniversalFieldHelper("LastQuarterEarnings",	"Last quarter earnings",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("LastTradingDate",	"Last trading date",	FIELD_TYPE_DATE,	3),
	UniversalFieldHelper("LocalCode",	"Local code",	FIELD_TYPE_TEXT_STRING,	12),
	UniversalFieldHelper("LongTermDebt",	"Long term debt",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("LongTermInvestMents",	"Long term investments",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("LowestSpread",	"Lowest spread",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("MovingAverage100day",	"100 day moving average",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("MovingAverage14day",	"21 day moving average",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("MovingAverage21day",	"21 day moving average",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("MovingAverage250day",	"250 day moving average",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("MovingAverage50day",	"50 day moving average",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("MovingAverage9day",	"9 day moving average",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("Name",	"Name",	FIELD_TYPE_TEXT_STRING,	64),
	UniversalFieldHelper("NetAccruedDividend",	"Net accrued dividend",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("NetIncome",	"Net income",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("NetProperty",	"Net property",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("NextDividend1",	"Next dividend 1",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("NextDividend1Date",	"Next dividend 1 date",	FIELD_TYPE_DATE,	3),
	UniversalFieldHelper("NextDividend1Status",	"Next dividend 1 status (Official, unofficial)",	FIELD_TYPE_UINT,	1),
	UniversalFieldHelper("NextDividend2",	"Next dividend 2",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("NextDividend2Date",	"Nextdividend 2 date",	FIELD_TYPE_DATE,	3),
	UniversalFieldHelper("NextDividend2Status",	"Next dividend 2 status (Official, unofficial)",	FIELD_TYPE_UINT,	1),
	UniversalFieldHelper("NextDividend3",	"Next dividend 3",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("NextDividend3Date",	"Next dividend 3 date",	FIELD_TYPE_DATE,	3),
	UniversalFieldHelper("NextDividend3Status",	"Next dividend 3 status (Official, unofficial)",	FIELD_TYPE_UINT,	1),
	UniversalFieldHelper("NextDividend4",	"Next dividend 4",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("NextDividend4Date",	"Next dividend 4 date",	FIELD_TYPE_DATE,	3),
	UniversalFieldHelper("NextDividend4Status",	"Next dividend 4 status (Official, unofficial)",	FIELD_TYPE_UINT,	1),
	UniversalFieldHelper("NextQuarterDate",	"Next quarter date",	FIELD_TYPE_DATE,	3),
	UniversalFieldHelper("NumberOfQuartersIncluded",	"Number of quarters included",	FIELD_TYPE_SINT,	1),
	UniversalFieldHelper("OptionType",	"Option type",	FIELD_TYPE_TEXT_ARRAY,	1),
	UniversalFieldHelper("OtherExpenses",	"Other expenses",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("OtherIncome",	"Other income",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("ParValuePerUnit",	"Par value per unit",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("PaymentDate",	"Payment date",	FIELD_TYPE_DATE,	3),
	UniversalFieldHelper("PlantAndEquipment",	"Plant and equipment",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("Previous12MonthsEarningsPerShare",	"Previous 12 months earnings per share",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("Previous7QuartersEarningsPerShare",	"Previous 7 quarters earnings per share",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("PreviousCurrentDate",	"Previous current date",	FIELD_TYPE_DATE,	3),
	UniversalFieldHelper("PreviousDividend",	"Previous dividend",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("PreviousDividendDate",	"Previous dividend date",	FIELD_TYPE_DATE,	3),
	UniversalFieldHelper("PreviousFiscalYearEarningsPerShare",	"Previous fiscal year earnings per share",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("PrimaryExchange",	"Primary exchange",	FIELD_TYPE_TEXT_STRING,	2),
	UniversalFieldHelper("RateOfReturn12month",	"Rate of return on the last 12 month period",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("RateOfReturnYear",	"Rate of return on the year",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("RecordDate",	"Last record date",	FIELD_TYPE_DATE,	3),
	UniversalFieldHelper("RetainedEarnings",	"Retained earnings",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("ReturnOnCapital",	"Return on capital",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("RevenuesFromSales",	"Revenues from sales",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("SellingAndGeneralAdminExpenses",	"Selling and general admin expenses",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("SessionId",	"Session id",	FIELD_TYPE_TEXT_ARRAY,	1),
	UniversalFieldHelper("Shareholders",	"Shareholders",	FIELD_TYPE_UINT,	4),
	UniversalFieldHelper("SharesAfter",	"Shares after",	FIELD_TYPE_UINT,	4),
	UniversalFieldHelper("SharesBefore",	"Shares before",	FIELD_TYPE_UINT,	4),
	UniversalFieldHelper("SharesHeldByInstitutions",	"Shares held by institutions",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("SharesOutstanding",	"Shares outstanding",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("SharesPerContract",	"Shares per contract",	FIELD_TYPE_UINT,	4),
	UniversalFieldHelper("SICCode",	"SIC code",	FIELD_TYPE_TEXT_ARRAY,	4),
	UniversalFieldHelper("SpecialDividend",	"Special dividend",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("SpecialDividendDate",	"Special dividend date",	FIELD_TYPE_DATE,	3),
	UniversalFieldHelper("SplitDate",	"Split date",	FIELD_TYPE_DATE,	3),
	UniversalFieldHelper("SplitRatio",	"Split ratio",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("State",	"State flags (closed, halted, deleted, pre-market, restricted, new record)",	FIELD_TYPE_UINT,	2),
	UniversalFieldHelper("StrikePrice",	"Strike price",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("TickCount",	"Tick count",	FIELD_TYPE_UINT,	4),
	UniversalFieldHelper("TickPrice",	"Tick price",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("TickSize",	"Tick size",	FIELD_TYPE_UINT,	4),
	UniversalFieldHelper("TickType",	"Tick type",	FIELD_TYPE_UINT,	1),
	UniversalFieldHelper("Time",	"Time",	FIELD_TYPE_TIME,	3),
	UniversalFieldHelper("TotalAssets",	"Total assets",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("TotalCash",	"Total cash",	FIELD_TYPE_RATIONAL,	5),
	UniversalFieldHelper("TotalCashAmountPerUnit",	"Total cash amount per unit",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("Unallocated",	"Unallocated distribution",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("Unchanged",	"Unchanged",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("Up",	"Up",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("VendorCode",	"Vendor code",	FIELD_TYPE_TEXT_STRING,	8),
	UniversalFieldHelper("Volatility01month",	"1 month historical volatility",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("Volatility02month",	"2 month historical volatility",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("Volatility03month",	"3 month historical volatility",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("Volatility04month",	"4 month historical volatility",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("Volatility05month",	"5 month historical volatility",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("Volatility06month",	"6 month historical volatility",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("Volatility07month",	"7 month historical volatility",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("Volatility08month",	"8 month historical volatility",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("Volatility09month",	"9 month historical volatility",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("Volatility10month",	"10 month historical volatility",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("Volatility11month",	"11 month historical volatility",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("Volatility12month",	"12 month historical volatility",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("Volatility20day",	"20 day historical volatility",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("Weight",	"Share weight",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("YearEndDate",	"Year end date",	FIELD_TYPE_DATE,	3),
	UniversalFieldHelper("CompanySymbol",	"Company symbol",	FIELD_TYPE_TEXT_STRING,	16),
	UniversalFieldHelper("CompanyTableNo",	"Company table number",	FIELD_TYPE_UINT,	2),
	UniversalFieldHelper("ConstituentSymbol",	"Constituent symbol",	FIELD_TYPE_TEXT_STRING,	16),
	UniversalFieldHelper("ConstituentTableNo",	"Constituent table number",	FIELD_TYPE_UINT,	2),
	UniversalFieldHelper("Context",	"Context",	FIELD_TYPE_UINT,	4),
	UniversalFieldHelper("CreateDate",	"Create date",	FIELD_TYPE_DATE,	3),
	UniversalFieldHelper("CugId",	"Cug id",	FIELD_TYPE_UINT,	2),
	UniversalFieldHelper("DelayPeriod",	"Delay period",	FIELD_TYPE_UINT,	2),
	UniversalFieldHelper("EntityType",	"Entity type",	FIELD_TYPE_UINT,	1),
	UniversalFieldHelper("EventType",	"Event type",	FIELD_TYPE_UINT,	1),
	UniversalFieldHelper("FieldId",	"Field id",	FIELD_TYPE_UINT,	2),
	UniversalFieldHelper("FieldLength",	"Field length",	FIELD_TYPE_UINT,	4),
	UniversalFieldHelper("FieldOrder",	"Field order",	FIELD_TYPE_UINT,	2),
	UniversalFieldHelper("FieldType",	"Field type",	FIELD_TYPE_UINT,	1),
	UniversalFieldHelper("FutureOptionRootSymbol",	"Future option root symbol",	FIELD_TYPE_TEXT_STRING,	16),
	UniversalFieldHelper("FutureOptionRootTableNo",	"Future option root table number",	FIELD_TYPE_UINT,	2),
	UniversalFieldHelper("FutureRootSymbol",	"Future root symbol",	FIELD_TYPE_TEXT_STRING,	16),
	UniversalFieldHelper("FutureRootTableNo",	"Future root table number",	FIELD_TYPE_UINT,	2),
	UniversalFieldHelper("IndexNumber",	"Index number",	FIELD_TYPE_UINT,	1),
	UniversalFieldHelper("IndexProperties",	"Index properties",	FIELD_TYPE_UINT,	1),
	UniversalFieldHelper("IndexSymbol",	"Index symbol",	FIELD_TYPE_TEXT_STRING,	16),
	UniversalFieldHelper("IndexTableNo",	"Index table number",	FIELD_TYPE_UINT,	2),
	UniversalFieldHelper("IndexType",	"Index type",	FIELD_TYPE_UINT,	1),
	UniversalFieldHelper("LastUpdateDate",	"Last update date",	FIELD_TYPE_DATE,	3),
	UniversalFieldHelper("LastUpdateTime",	"Last update time",	FIELD_TYPE_TIME,	3),
	UniversalFieldHelper("ListingSymbol",	"Listing symbol",	FIELD_TYPE_TEXT_STRING,	16),
	UniversalFieldHelper("ListingSymbolList",	"Listing symbol list",	FIELD_TYPE_TEXT_STRING,	128),
	UniversalFieldHelper("ListingTableNo",	"Listing table number",	FIELD_TYPE_UINT,	2),
	UniversalFieldHelper("MessageType",	"Message type",	FIELD_TYPE_UINT,	1),
	UniversalFieldHelper("NavigationIndexNumber",	"Navigation index number",	FIELD_TYPE_UINT,	1),
	UniversalFieldHelper("NavigationKeyFieldId",	"Navigation key field id",	FIELD_TYPE_UINT,	2),
	UniversalFieldHelper("NavigationTableNo",	"Navigation table number",	FIELD_TYPE_UINT,	2),
	UniversalFieldHelper("NewDescription",	"New description",	FIELD_TYPE_TEXT_STRING,	64),
	UniversalFieldHelper("NewIsin",	"New ISIN",	FIELD_TYPE_TEXT_ARRAY,	12),
	UniversalFieldHelper("NewListingPermissionId",	"New listing permission id",	FIELD_TYPE_UINT,	2),
	UniversalFieldHelper("NewListingSymbolList",	"New listing symbol list",	FIELD_TYPE_TEXT_STRING,	128),
	UniversalFieldHelper("NewSecuritySymbol",	"New security symbol",	FIELD_TYPE_TEXT_STRING,	16),
	UniversalFieldHelper("OldDescription",	"Old description",	FIELD_TYPE_TEXT_STRING,	64),
	UniversalFieldHelper("OldIsin",	"Old ISIN",	FIELD_TYPE_TEXT_ARRAY,	12),
	UniversalFieldHelper("OldListingPermissionId",	"Old listing permission id",	FIELD_TYPE_UINT,	2),
	UniversalFieldHelper("OldListingSymbolList",	"Old listing symbol list",	FIELD_TYPE_TEXT_STRING,	128),
	UniversalFieldHelper("OldSecuritySymbol",	"Old security symbol",	FIELD_TYPE_TEXT_STRING,	16),
	UniversalFieldHelper("OptionDeliverableSymbol",	"Option deliverable symbol",	FIELD_TYPE_TEXT_STRING,	16),
	UniversalFieldHelper("OptionDeliverableTableNo",	"Option deliverable table number",	FIELD_TYPE_UINT,	2),
	UniversalFieldHelper("OptionRootSymbol",	"Option root symbol",	FIELD_TYPE_TEXT_STRING,	16),
	UniversalFieldHelper("OptionRootTableNo",	"Option root table number",	FIELD_TYPE_UINT,	2),
	UniversalFieldHelper("OutputKeyFieldId",	"Output key field id",	FIELD_TYPE_UINT,	2),
	UniversalFieldHelper("OutputPermissionIdFieldId",	"Output permission id field id",	FIELD_TYPE_UINT,	2),
	UniversalFieldHelper("OutputTableNoFieldId",	"Output table number field id",	FIELD_TYPE_UINT,	2),
	UniversalFieldHelper("Password",	"Password",	FIELD_TYPE_BINARY_STRING,	16),
	UniversalFieldHelper("PermissionBody",	"Permission body",	FIELD_TYPE_BLOB,	256),
	UniversalFieldHelper("PermissionId",	"Permission id",	FIELD_TYPE_UINT,	2),
	UniversalFieldHelper("PermissionId0",	"Permission id 0",	FIELD_TYPE_UINT,	2),
	UniversalFieldHelper("PermissionId1",	"Permission id 1",	FIELD_TYPE_UINT,	2),
	UniversalFieldHelper("PermissionLevelId",	"Permission level id",	FIELD_TYPE_UINT,	1),
	UniversalFieldHelper("PreviousSymbol",	"Previous symbol",	FIELD_TYPE_TEXT_STRING,	16),
	UniversalFieldHelper("ProviderName",	"Provider name",	FIELD_TYPE_TEXT_STRING,	32),
	UniversalFieldHelper("RecordStatus",	"Record status",	FIELD_TYPE_UINT,	1),
	UniversalFieldHelper("RelationshipId",	"Relationship id",	FIELD_TYPE_UINT,	2),
	UniversalFieldHelper("ResetDate",	"Reset date",	FIELD_TYPE_DATE,	3),
	UniversalFieldHelper("ResetTime",	"Reset time",	FIELD_TYPE_TIME,	3),
	UniversalFieldHelper("SectorSymbol",	"Sector symbol",	FIELD_TYPE_TEXT_STRING,	16),
	UniversalFieldHelper("SectorTableNo",	"Sector table number",	FIELD_TYPE_UINT,	2),
	UniversalFieldHelper("SecuritySymbol",	"Security smbol",	FIELD_TYPE_TEXT_STRING,	16),
	UniversalFieldHelper("SecurityTableNo",	"Security table number",	FIELD_TYPE_UINT,	2),
	UniversalFieldHelper("SourceContext",	"Source context",	FIELD_TYPE_UINT,	4),
	UniversalFieldHelper("Symbol",	"Symbol",	FIELD_TYPE_TEXT_STRING,	16),
	UniversalFieldHelper("Symbol01",	"Symbol 1",	FIELD_TYPE_TEXT_STRING,	16),
	UniversalFieldHelper("Symbol02",	"Symbol 2",	FIELD_TYPE_TEXT_STRING,	16),
	UniversalFieldHelper("Symbol03",	"Symbol 3",	FIELD_TYPE_TEXT_STRING,	16),
	UniversalFieldHelper("Symbol04",	"Symbol 4",	FIELD_TYPE_TEXT_STRING,	16),
	UniversalFieldHelper("Symbol05",	"Symbol 5",	FIELD_TYPE_TEXT_STRING,	16),
	UniversalFieldHelper("Symbol06",	"Symbol 6",	FIELD_TYPE_TEXT_STRING,	16),
	UniversalFieldHelper("Symbol07",	"Symbol 7",	FIELD_TYPE_TEXT_STRING,	16),
	UniversalFieldHelper("Symbol08",	"Symbol 8",	FIELD_TYPE_TEXT_STRING,	16),
	UniversalFieldHelper("Symbol09",	"Symbol 9",	FIELD_TYPE_TEXT_STRING,	16),
	UniversalFieldHelper("Symbol10",	"Symbol 10",	FIELD_TYPE_TEXT_STRING,	16),
	UniversalFieldHelper("Symbol11",	"Symbol 11",	FIELD_TYPE_TEXT_STRING,	16),
	UniversalFieldHelper("Symbol12",	"Symbol 12",	FIELD_TYPE_TEXT_STRING,	16),
	UniversalFieldHelper("Symbol13",	"Symbol 13",	FIELD_TYPE_TEXT_STRING,	16),
	UniversalFieldHelper("Symbol14",	"Symbol 14",	FIELD_TYPE_TEXT_STRING,	16),
	UniversalFieldHelper("Symbol15",	"Symbol 15",	FIELD_TYPE_TEXT_STRING,	16),
	UniversalFieldHelper("Symbol16",	"Symbol 16",	FIELD_TYPE_TEXT_STRING,	16),
	UniversalFieldHelper("Symbol17",	"Symbol 17",	FIELD_TYPE_TEXT_STRING,	16),
	UniversalFieldHelper("Symbol18",	"Symbol 18",	FIELD_TYPE_TEXT_STRING,	16),
	UniversalFieldHelper("Symbol19",	"Symbol 19",	FIELD_TYPE_TEXT_STRING,	16),
	UniversalFieldHelper("Symbol20",	"Symbol 20",	FIELD_TYPE_TEXT_STRING,	16),
	UniversalFieldHelper("TableId",	"Table id",	FIELD_TYPE_UINT,	2),
	UniversalFieldHelper("TableNo",	"Table number",	FIELD_TYPE_UINT,	2),
	UniversalFieldHelper("TableType",	"Table type",	FIELD_TYPE_UINT,	1),
	UniversalFieldHelper("TableVersion",	"Table version",	FIELD_TYPE_UINT,	1),
	UniversalFieldHelper("UnderlyingSymbol",	"Underlying symbol",	FIELD_TYPE_TEXT_STRING,	16),
	UniversalFieldHelper("UnderlyingTableNo",	"Underlying table number",	FIELD_TYPE_UINT,	2),
	UniversalFieldHelper("UpdateId",	"Update id",	FIELD_TYPE_UINT,	2),
	UniversalFieldHelper("UserBody",	"User body",	FIELD_TYPE_BLOB,	4096),
	UniversalFieldHelper("UserId",	"User id",	FIELD_TYPE_TEXT_STRING,	16),
	UniversalFieldHelper("Username",	"Username",	FIELD_TYPE_TEXT_STRING,	16),
	UniversalFieldHelper("LimitHighPrice",	"Limit high price",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("LimitLowPrice",	"Limit low price",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("CurrentYieldTime",	"Current yield time",	FIELD_TYPE_TIME,	3),
	UniversalFieldHelper("CurrentYieldDate",	"Current Yield Date",	FIELD_TYPE_DATE,	3),
	UniversalFieldHelper("DividendReinvestPrice",	"Dividend reinvest price",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("CapGainsReinvestPrice",	"Capital gains reinvest price",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("DividendReinvestPriceDate",	"Dividend reinvest price date",	FIELD_TYPE_DATE,	3),
	UniversalFieldHelper("CapGainsReinvestPriceDate",	"Capital gains reinvest price date",	FIELD_TYPE_DATE,	3),
	UniversalFieldHelper("NavHigh",	"Net asset value high",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("NavHighTime",	"Net asset value high time",	FIELD_TYPE_TIME,	3),
	UniversalFieldHelper("NavLow",	"Net asset value low",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("NavLowTime",	"Net asset value low time",	FIELD_TYPE_TIME,	3),
	UniversalFieldHelper("GeneralBusinessExpensesPlusCommissions",	"General business expenses plus commissions",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("InterestRevenue",	"Interest revenue",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("InvestmentSecurities",	"Investment securities",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("NetInterestIncome",	"Net interest income",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("NetPropertyPlantAndEquipment",	"Net property plant and equipment",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("NonInterestExpenses",	"Non interest expenses",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("ParValueOfCommonStock",	"Par value of common stock",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("PermiumsEarned",	"Permiums earned",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("PremiumsEarnedLessExpenses",	"Premiums earned less expenses",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("Securities",	"Securities",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("TotalInterestIncome",	"Total interest income",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("TotalInvestedAssets",	"Total invested assets",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("TotalNonInterestIncome",	"Total non interest income",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("TotalNonPremiumIncome",	"Total non premium income",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("PreviousSettlement",	"Previous settlement price",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("7DayYieldDate",	"7 day yield date",	FIELD_TYPE_DATE,	3),
	UniversalFieldHelper("Annualized7DayYieldDate",	"Annualized 7 day yield date",	FIELD_TYPE_DATE,	3),
	UniversalFieldHelper("PreviousCumulativePrice",	"Previous cumulative price",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("ExMarker",	"Ex marker",	FIELD_TYPE_BINARY_STRING,	1),
	UniversalFieldHelper("NewSymbolList",	"New symbol list",	FIELD_TYPE_BLOB,	1024),
	UniversalFieldHelper("OldSymbolList",	"Old symbol list",	FIELD_TYPE_BLOB,	1024),
	UniversalFieldHelper("CompositeExchange",	"Composite exchange",	FIELD_TYPE_TEXT_STRING,	3),
	UniversalFieldHelper("AnalyticsSymbol",	"Analytics symbol",	FIELD_TYPE_TEXT_STRING,	16),
	UniversalFieldHelper("AnalyticsTableNo",	"Analytics table number",	FIELD_TYPE_UINT,	2),
	UniversalFieldHelper("StoryDateTime",	"Story date/time",	FIELD_TYPE_DATE_TIME,	6),
	UniversalFieldHelper("Headline",	"Headline",	FIELD_TYPE_TEXT_STRING,	255),
	UniversalFieldHelper("StoryBody",	"Story body",	FIELD_TYPE_CRC_BLOB,	4096),
	UniversalFieldHelper("ReportDate",	"Report date",	FIELD_TYPE_DATE,	3),
	UniversalFieldHelper("IncomeFromContinuingOperations",	"Income from continuing operations",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("AnnualRevenuesFromSales",	"Annual revenues from sales",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("AnnualGrossProfits",	"Annual gross profits",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("AnnualSellingAndGeneralAdminExpense",	"Annual selling and general admin expense",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("AnnualDepreciationDepletion",	"Annual depreciation depletion",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("AnnualInterestExpense",	"Annual interest expense",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("AnnualIncomeBeforeTax",	"Annual income before tax",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("AnnualIncomeFromContinuingOperations",	"Annual income from continuing operations",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("AnnualNetIncome",	"Annual net income",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("AnnualCurrentAssets",	"Annual current assets",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("AnnualNetPropertyPlantAndEquipment",	"Annual net property plant and equipment",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("AnnualTotalAssets",	"Annual total assets",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("AnnualCurrentLiabilities",	"Annual current liabilities",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("AnnualTotalLiabilities",	"Annual total liabilities",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("AnnualLongTermDebt",	"Annual long term debt",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("AnnualCapitalStock",	"Annual capital stock",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("AnnualAdditionalPaidInCapital",	"Annual additional paid in capital",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("AnnualRetainedEarnings",	"Annual retained earnings",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("TotalLiabilities",	"Total liabilities",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("IndicatedYield",	"Indicated yield",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("Participant",	"Participant",	FIELD_TYPE_TEXT_STRING,	6),
	UniversalFieldHelper("TimeSeriesType",	"Time series type",	FIELD_TYPE_UINT,	1),
	UniversalFieldHelper("DateTime",	"Date time",	FIELD_TYPE_DATE_TIME,	6),
	UniversalFieldHelper("TimeSeriesSymbol",	"Time series symbol",	FIELD_TYPE_TEXT_STRING,	16),
	UniversalFieldHelper("TimeSeriesTableNo",	"Time series table number",	FIELD_TYPE_UINT,	2),
	UniversalFieldHelper("SourcePermissionId",	"Source permission id",	FIELD_TYPE_UINT,	2),
	UniversalFieldHelper("NumberOfConcurrentUsers",	"Number of concurrent users",	FIELD_TYPE_UINT,	2),
	UniversalFieldHelper("TargetSymbol",	"Target symbol",	FIELD_TYPE_TEXT_STRING,	16),
	UniversalFieldHelper("TargetTableNo",	"Target table number",	FIELD_TYPE_UINT,	2),
	UniversalFieldHelper("PriceAdjustment",	"Price adjustment",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("CompositeFlag",	"Composite flag",	FIELD_TYPE_TEXT_ARRAY,	1),
	UniversalFieldHelper("SymbolList",	"Symbol list",	FIELD_TYPE_TEXT_STRING,	128),
	UniversalFieldHelper("ExpirationDate01",	"Expiration date 1",	FIELD_TYPE_DATE,	3),
	UniversalFieldHelper("ExpirationDate02",	"Expiration date 2",	FIELD_TYPE_DATE,	3),
	UniversalFieldHelper("ExpirationDate03",	"Expiration date 3",	FIELD_TYPE_DATE,	3),
	UniversalFieldHelper("ExpirationDate04",	"Expiration date 4",	FIELD_TYPE_DATE,	3),
	UniversalFieldHelper("ExpirationDate05",	"Expiration date 5",	FIELD_TYPE_DATE,	3),
	UniversalFieldHelper("ExpirationDate06",	"Expiration date 6",	FIELD_TYPE_DATE,	3),
	UniversalFieldHelper("ExpirationDate07",	"Expiration date 7",	FIELD_TYPE_DATE,	3),
	UniversalFieldHelper("ExpirationDate08",	"Expiration date 8",	FIELD_TYPE_DATE,	3),
	UniversalFieldHelper("ExpirationDate09",	"Expiration date 9",	FIELD_TYPE_DATE,	3),
	UniversalFieldHelper("ExpirationDate10",	"Expiration date 10",	FIELD_TYPE_DATE,	3),
	UniversalFieldHelper("ExpirationDate11",	"Expiration date 11",	FIELD_TYPE_DATE,	3),
	UniversalFieldHelper("ExpirationDate12",	"Expiration date 12",	FIELD_TYPE_DATE,	3),
	UniversalFieldHelper("StrikePrice01",	"Strike price 1",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("StrikePrice02",	"Strike price 2",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("StrikePrice03",	"Strike price 3",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("StrikePrice04",	"Strike price 4",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("StrikePrice05",	"Strike price 5",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("StrikePrice06",	"Strike price 6",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("StrikePrice07",	"Strike price 7",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("StrikePrice08",	"Strike price 8",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("StrikePrice09",	"Strike price 9",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("StrikePrice10",	"Strike price 10",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("StrikePrice11",	"Strike price 11",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("StrikePrice12",	"Strike price 12",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("StrikePrice13",	"Strike price 13",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("StrikePrice14",	"Strike price 14",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("StrikePrice15",	"Strike price 15",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("StrikePrice16",	"Strike price 16",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("StrikePrice17",	"Strike price 17",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("StrikePrice18",	"Strike price 18",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("StrikePrice19",	"Strike price 19",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("StrikePrice20",	"Strike price 20",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("StrikePrice21",	"Strike price 21",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("StrikePrice22",	"Strike price 22",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("StrikePrice23",	"Strike price 23",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("StrikePrice24",	"Strike price 24",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("StrikePrice25",	"Strike price 25",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("StrikePrice26",	"Strike price 26",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("ContractSize",	"Contract size",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("LocalExchange",	"Local exchange",	FIELD_TYPE_TEXT_STRING,	4),
	UniversalFieldHelper("LocalCallRoot",	"Local call root",	FIELD_TYPE_TEXT_STRING,	12),
	UniversalFieldHelper("LocalPutRoot",	"Local put root",	FIELD_TYPE_TEXT_STRING,	12),
	UniversalFieldHelper("LocalRoot",	"Local root",	FIELD_TYPE_TEXT_STRING,	12),
	UniversalFieldHelper("MinimumTick",	"Minimum tick",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("TradingDayStartTime",	"Trading day start time",	FIELD_TYPE_TIME,	3),
	UniversalFieldHelper("TradingDayEndTime",	"Trading day end time",	FIELD_TYPE_TIME,	3),
	UniversalFieldHelper("TradingDaysOfWeek",	"TradingDaysOfWeek",	FIELD_TYPE_TEXT_ARRAY,	7),
	UniversalFieldHelper("UnitOfMeasure",	"Unit of measure",	FIELD_TYPE_UINT,	1),
	UniversalFieldHelper("FutureSymbol",	"Future symbol",	FIELD_TYPE_TEXT_STRING,	16),
	UniversalFieldHelper("FutureTableNo",	"Future table number",	FIELD_TYPE_UINT,	2),
	UniversalFieldHelper("FutureOptionSymbol",	"Future option symbol",	FIELD_TYPE_TEXT_STRING,	16),
	UniversalFieldHelper("FutureOptionTableNo",	"Future option table number",	FIELD_TYPE_UINT,	2),
	UniversalFieldHelper("FutureOptionExpirationSymbol",	"Future option expiration symbol",	FIELD_TYPE_TEXT_STRING,	16),
	UniversalFieldHelper("FutureOptionExpirationTableNo",	"Future option expiration table number",	FIELD_TYPE_UINT,	2),
	UniversalFieldHelper("HasDeliverables",	"Has deliverables",	FIELD_TYPE_TEXT_ARRAY,	1),
	UniversalFieldHelper("CategoryCodeList",	"Category code list",	FIELD_TYPE_BLOB,	256),
	UniversalFieldHelper("UnitOfMeasureCurrency",	"Unit of measure currency",	FIELD_TYPE_TEXT_ARRAY,	3),
	UniversalFieldHelper("VolumeWeightedOpeningPrice",	"Volume weighted opening price",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("Contributor",	"Contributor",	FIELD_TYPE_TEXT_STRING,	4),
	UniversalFieldHelper("Bid6",	"Bid6 price",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("Bid6Count",	"Bid6 count",	FIELD_TYPE_UINT,	2),
	UniversalFieldHelper("Bid6Size",	"Bid6 size",	FIELD_TYPE_UINT,	4),
	UniversalFieldHelper("Ask6",	"Ask6",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("Ask6Count",	"Ask6 count",	FIELD_TYPE_UINT,	2),
	UniversalFieldHelper("Ask6Size",	"Ask6 size",	FIELD_TYPE_UINT,	4),
	UniversalFieldHelper("Bid7",	"Bid7",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("Bid7Count",	"Bid7 count",	FIELD_TYPE_UINT,	2),
	UniversalFieldHelper("Bid7Size",	"Bid7 size",	FIELD_TYPE_UINT,	4),
	UniversalFieldHelper("Ask7",	"Ask7",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("Ask7Count",	"Ask7 count",	FIELD_TYPE_UINT,	2),
	UniversalFieldHelper("Ask7Size",	"Ask7 size",	FIELD_TYPE_UINT,	4),
	UniversalFieldHelper("Bid8",	"Bid8",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("Bid8Count",	"Bid8 count",	FIELD_TYPE_UINT,	2),
	UniversalFieldHelper("Bid8Size",	"Bid8 size",	FIELD_TYPE_UINT,	4),
	UniversalFieldHelper("Ask8",	"Ask8",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("Ask8Count",	"Ask8 count",	FIELD_TYPE_UINT,	2),
	UniversalFieldHelper("Ask8Size",	"Ask8 size",	FIELD_TYPE_UINT,	4),
	UniversalFieldHelper("Bid9",	"Bid9",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("Bid9Count",	"Bid9 count",	FIELD_TYPE_UINT,	2),
	UniversalFieldHelper("Bid9Size",	"Bid9 size",	FIELD_TYPE_UINT,	4),
	UniversalFieldHelper("Ask9",	"Ask9",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("Ask9Count",	"Ask9 count",	FIELD_TYPE_UINT,	2),
	UniversalFieldHelper("Ask9Size",	"Ask9 size",	FIELD_TYPE_UINT,	4),
	UniversalFieldHelper("Bid10",	"Bid10",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("Bid10Count",	"Bid10 count",	FIELD_TYPE_UINT,	2),
	UniversalFieldHelper("Bid10Size",	"Bid10 size",	FIELD_TYPE_UINT,	4),
	UniversalFieldHelper("Ask10",	"Ask10",	FIELD_TYPE_RATIONAL,	9),
	UniversalFieldHelper("Ask10Count",	"Ask10 count",	FIELD_TYPE_UINT,	2),
	UniversalFieldHelper("Ask10Size",	"Ask10 size",	FIELD_TYPE_UINT,	4),
	UniversalFieldHelper("PeRatio",	"Price earnings ratio",	FIELD_TYPE_RATIONAL,	5),
	UniversalFieldHelper("PermissionIdList",	"Permission id list",	FIELD_TYPE_BINARY_STRING,	100),
};

/**
 *	@brief	Get the universal field helper.
 *
 *	@param	fieldId the field id.
 *
 *	@return	the universal field helper.
 */
inline UniversalFieldHelper GetUniversalFieldHelper(const FieldId fieldId)
{
	return ((ACTIV_ARRAY_SIZE(g_universalFieldHelperList) > fieldId) ? g_universalFieldHelperList[fieldId] : UniversalFieldHelper());
}

/**
 *	@brief	Get the universal field name.
 *
 *	@param	fieldId the field id.
 *
 *	@return	the name of the field.
 */
inline std::string GetUniversalFieldName(const FieldId fieldId)
{
	return ((ACTIV_ARRAY_SIZE(g_universalFieldHelperList) > fieldId) ? g_universalFieldHelperList[fieldId].m_name : TypeToString(fieldId));
}

/**
 *	@brief	Get the universal field description.
 *
 *	@param	fieldId the field id.
 *
 *	@return	the description of the field.
 */
inline std::string GetUniversalFieldDescription(const FieldId fieldId)
{
	return ((ACTIV_ARRAY_SIZE(g_universalFieldHelperList) > fieldId) ? g_universalFieldHelperList[fieldId].m_description : TypeToString(fieldId));
}

/**
 *	@brief	Get the universal field type.
 *
 *	@param	fieldId the field id.
 *
 *	@return	the type of the field.
 */
inline FieldType GetUniversalFieldType(const FieldId fieldId)
{
	return ((ACTIV_ARRAY_SIZE(g_universalFieldHelperList) > fieldId) ? g_universalFieldHelperList[fieldId].m_type : FIELD_TYPE_UNKNOWN);
}

/**
 *	@brief	Get the universal field maximum length.
 *
 *	@param	fieldId the field id.
 *
 *	@return	the maximum length of the field.
 */
inline size_t GetUniversalFieldMaxLength(const FieldId fieldId)
{
	return ((ACTIV_ARRAY_SIZE(g_universalFieldHelperList) > fieldId) ? g_universalFieldHelperList[fieldId].m_maxLength : 0);
}

} // namespace Feed

} // namespace Activ

#endif // !defined(ACTIV_FEED_FIELD_IDS_H)

/*
Copyright © 1999 International Securities Exchange LLC ("ISE").
All Rights Reserved.

USE OF THIS SOFTWARE IS SUBJECT TO THE TERMS AND CONDITIONS OF
THE LICENSE GRANTED TO THE RECIPIENT UNDER THE "DEVELOPERS AGREEMENT".
REDISTRIBUTION OF THIS SOFTWARE IS PROHIBITED.

THIS SOFTWARE IS PROVIDED "AS IS", WITHOUT A WARRANTY OF ANY KIND.
ALL EXPRESS OR IMPLIED CONDITIONS, REPRESENTATIONS AND WARRANTIES,
INCLUDING ANY EXPRESS OR IMPLIED WARRANTY OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE OR NON-INFRINGEMENT OF INTELLECTUAL PROPERTY
RIGHTS, ARE HEREBY EXCLUDED.  ISE AND ITS LICENSORS SHALL NOT BE LIABLE
FOR ANY DAMAGES SUFFERED BY LICENSEE AS A RESULT OF USING, MODIFYING OR
DISTRIBUTING THE SOFTWARE OR ITS DERIVATIVES.  IN NO EVENT WILL ISE OR
ITS LICENSORS BE LIABLE FOR ANY LOST REVENUE, PROFIT OR DATA, OR FOR
DIRECT, INDIRECT, SPECIAL, CONSEQUENTIAL, INCIDENTAL OR PUNITIVE DAMAGES,
HOWEVER CAUSED AND REGARDLESS OF THE THEORY OF LIABILITY, ARISING
OUT OF THE USE OF OR INABILITY TO USE THE SOFTWARE, EVEN IF ISE HAS
BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.


Title:	const.h
Author: Neil Cossor

    general constants for things in ISEsim...
*/
#ifndef CONST_H
#define CONST_H

// OM 15.7 compatible, 20000204, includes autoquoting, synthetics.. updated autoquoting, set,(p)mm by bin
// main split out into main.c, timestamp bug in logger
// added remainder of Solaris code, in particular thread creation etc
// various bugfixes to v2.08
// added more options in VOLUMES command (series.c), v2.09
// fix multi-orderbook queries for MQ101, MQ102, MQ104, IQ101
// added UQ11, UQ12, v2.10
// v3.x is compatable with om 15.7-3 (da203 fields -> da210)
// v4.x is compatible with om 16.3+ (32 bit qty etc)
// 20010411 NAC	Updated to use 16.x client-categories, and added extra locks
// 200203xx NAC Gemini enhancements (?17.1)
#define SIM_VERSION 			    "ISEsim v5.01"
// the year offset CLICK uses in its' bit-packing of dates
#define CLICK_BIT_YEAR_OFFSET		    1989


#define SECONDS_IN_A_DAY		    86400 // 60 * 60 * 24
#define NUL				    '\0'
#define BIDANDASK			    0
#define BID				    1
#define ASK				    2
#define CMD_SEPARATOR			    ",\n"   // commas, and any (sneaky little) newlines
#define ONE_SECOND			    1000 /* Milliseconds */
#define USER_DATA_FILENAME		    "ISESim.dat"
#define FILE_WRITE_ACCESS		    "w"
#define FILE_READ_ACCESS		    "r"
// values packed into validity time
// NB 16.5 adds 3 flavours each of FillAndStore and AllOrNothing, and new values for IOC and FOK (old to be deprecated)
#define VALIDITY_UNIT_OLD_MATCH		    0x00    // only during this match.. deprecated, one of the old FOK or IOC indicated by VALUE byte
#define VALIDITY_UNIT_OLD_DAY		    0x01
#define VALIDITY_UNIT_OLD_GTC		    0x02
#define VALIDITY_UNIT_IOC		    0x10    // new IOC value == 16, which is really FAS, with time of 0
#define VALIDITY_UNIT_FAS_DAY		    0x11    // new FillAndStore, Day == 17
#define VALIDITY_UNIT_FAS_GTC		    0x12    // new FillAndStore, GTC == 18
#define VALIDITY_UNIT_FAS_TIMED		    0x13    // new FillAndStore, time (in minutes) in VALUE byte, == 19
#define VALIDITY_UNIT_FOK		    0x20    // new FOK value == 32, which is really AON, with time of 0
#define VALIDITY_UNIT_AON_DAY		    0x21    // new AllOrNone, Day == 33
#define VALIDITY_UNIT_AON_GTC		    0x22    // new AllOrNone, GTC == 34
#define VALIDITY_UNIT_AON_TIMED		    0x23    // new AllOrNone, time (in minutes) in VALUE byte, == 35
// the next two values are only relevant if the UNIT byte contains 0 - deprecated in 17.0 probably
#define VALIDITY_VALUE_OLD_FOK		    0x01    // == all or nothing, immediate
#define VALIDITY_VALUE_OLD_IOC		    0x00    // == fill and cancel
// and some versions of these as a 2-byte value.. NB the _TIMED values need to have a non-zero added to them to be OK
#define VALIDITY_TIME_OLD_GTC		    0x0200  // GTC, superceded in future release by FAS_GTC and AON_GTC
#define VALIDITY_TIME_IOC		    0x1000
#define VALIDITY_TIME_FAS_DAY		    0x1100  // new FillAndStore, Day == 17
#define VALIDITY_TIME_FAS_GTC		    0x1200  // new FillAndStore, GTC == 18
#define VALIDITY_TIME_FAS_TIMED		    0x1300  // new FillAndStore, time (in minutes) in VALUE byte, == 19
#define VALIDITY_TIME_FOK		    0x2000  // new FOK value == 32, which is really AON, with time of 0
#define VALIDITY_TIME_AON_DAY		    0x2100  // new AllOrNone, Day == 33
#define VALIDITY_TIME_AON_GTC		    0x2200  // new AllOrNone, GTC == 34
#define VALIDITY_TIME_AON_TIMED		    0x2300  // new AllOrNone, time (in minutes) in VALUE byte, == 35

// values packed into client category
#define CUST_CATEGORY_MM		    0	    // erratically documented by OM - appears to be for MM orders
#define CUST_CATEGORY_CUSTOMER		    1	    // customer
#define CUST_CATEGORY_FIRM		    2	    // Firm/Proprietary
					//  3	    illegal/reserved, as of 16.3
#define CUST_CATEGORY_FARMM		    4	    // FARMM - was 3 before 16.3
#define CUST_CATEGORY_DISCRETIONARY	    5	    // customer discretionary - was 4 before 16.3
#define CUST_CATEGORY_BROKERDEALER	    6	    // Broker/Dealer - formerly handled as B/D in BrokerID field
// values packed into order_type_c / order category
#define ORDER_CAT_ORDER 		    0x0001  // ordinary order
#define ORDER_CAT_QUOTE 		    0x0002  // quote
#define ORDER_CAT_BLOCK 		    0x0004  // block order
#define ORDER_CAT_BLOCK_RESPONSE	    0x0008  // block response
#define ORDER_CAT_FACILITATION		    0x0010  // facilitation order
#define ORDER_CAT_FACILITATION_RESPONSE     0x0020  // facilitation response
#define ORDER_CAT_CAB			    0x0040  // CAB order
#define ORDER_CAT_DERIVED		    0x0080  // Derived order
#define ORDER_CAT_IMPROVE_QUOTE		    0x0100  // improvement quote.. NOT in ISE system
#define ORDER_CAT_COMBO_LIMIT		    0x0200  // limit combo i.e. executable combo
#define ORDER_CAT_COMBO_INTEREST	    0x0400  // interest-only combo
//					    0x800 reserved by OM
#define ORDER_CAT_SOLICITATION		    0x1000  // solicitation order
#define ORDER_CAT_SOLICITATION_RESPONSE	    0x2000  // solicitation response
#define ORDER_CAT_EXPOSURE		    0x4000  // exposure order
#define ORDER_CAT_EXPOSURE_RESPONSE	    0x8000  // exposure response
#define ORDER_CAT_PLAIN_ORDERS		    0xF07D  // all ordinary orders.. no quotes, no combos, no derived
						    // includes block,facilitation,solicitation,exposure
// values packed into order_select_n. used in MQ101, 102, 104
#define ORDER_SELECT_GTC		    0x0001  // all GTC orders
#define ORDER_SELECT_DAY		    0x0002  // all Day orders
#define ORDER_SELECT_MINUTES		    0x0004  // all 'minutes' i.e.intraday timed orders
#define ORDER_SELECT_IMMEDIATE		    0x0008  // all immediate orders
#define ORDER_SELECT_PART		    0x0010  // all part orders
#define ORDER_SELECT_WHOLE		    0x0020  // all whole orders
#define ORDER_SELECT_LOCKED		    0x0040  // all locked orders
#define ORDER_SELECT_NOTLOCKED		    0x0080  // all active (not-locked) orders
#define ORDER_SELECT_ALL		    0x00FF  // all orders.. only include 'defined' ones
						    // ah. .this includes locked on not-locked
// values packed into special_order_type_c - Gemini++
#define SPECIAL_ORDER_TYPE_UNDEFINED	    0
#define SPECIAL_ORDER_TYPE_FACILITATION	    1
#define SPECIAL_ORDER_TYPE_EXPOSURE	    2
#define SPECIAL_ORDER_TYPE_SOLICITATION	    9
#define SPECIAL_ORDER_TYPE_BLOCK	    17
// values packed into combo-only field interest_order_c
#define COMBO_INTEXEC_LIMIT		    1
#define COMBO_INTEXEC_INTEREST		    2
// values packed into strike_price_group_c
#define STRIKE_PRICE_GROUP_DEEPIN	    1
#define STRIKE_PRICE_GROUP_IN		    2
#define STRIKE_PRICE_GROUP_AT		    3
#define STRIKE_PRICE_GROUP_OUT		    4
#define STRIKE_PRICE_GROUP_DEEPOUT	    5
// and the max - allow for more.. rather than tying it to DEEPOUT being the highest
#define MAX_STRIKE_PRICE_GROUP		    5
// values packed into the cst_type_c
#define CUST_TYPE_PMM			    1
#define CUST_TYPE_MM			    2
// values packed into the order_type_c - slid up by one in Gemini
#define ORDER_TYPE_LIMIT		    1
#define ORDER_TYPE_MARKET		    2
// values packed into the lock_type_c
#define LOCK_TYPE_CAB			    0
#define LOCK_TYPE_AWAY_BETTER		    1
#define LOCK_TYPE_GUARANTEE		    2
#define LOCK_TYPE_FARMM_TICKS 		    4
#define LOCK_TYPE_INVERSION		    32
// values packed into series status (in query responses, bcasts, and PMM rotation commands/queries)
#define SERIES_STATUS_CLOSED		    0
#define SERIES_STATUS_QUEUING		    1
#define SERIES_STATUS_TRYING		    2
#define SERIES_STATUS_REGULAR		    3
#define SERIES_STATUS_HALT		    4
#define SERIES_STATUS_FAST_MARKET	    5
// values packed in rotation failure reasons (in MQ114/MA114)
#define ROTATION_REASON_AWAY_BETTER	    1
#define ROTATION_REASON_PRICES_MISSING	    2
#define ROTATION_REASON_MARKET_ORDERS	    3
#define ROTATION_REASON_ASK_BID_CROSS_LIMIT 4
// values packed in reason_order_quote_delete_c (in BO107)
#define REASON_DELETE_MATCH_ROTATION_ENDED  0
#define REASON_DELETE_TOO_SMALL_VOLUME	    1
#define REASON_DELETE_AUTO_MARKETPLACE	    2
#define REASON_DELETE_MANUAL_STAFF	    3
#define REASON_DELETE_PMM_RELEASED	    4
#define REASON_DELETE_QUOTE_CROSS_LOCKED    5
#define REASON_DELETE_BLOCK_FACIL_ENDED     6
#define REASON_DELETE_QUOTE_TICKED_ZERO     7
#define REASON_DELETE_BLOCK_FACIL_ENDED_2   8
// values given back as good statii for order entry (MO101..
#define ORDER_STATUS_NO_PART		    1	    // no part in orderbook, no part closed ??
#define ORDER_STATUS_MATCH		    2	    // no part in orderbook, all matched
#define ORDER_STATUS_MATCH_NO_PART	    3	    // part matched, no part left in orderbook
#define ORDER_STATUS_ENTERED		    4	    // whole order/quote entered
#define ORDER_STATUS_MATCH_PART 	    5	    // part matched, part left in orderbook
#define ORDER_STATUS_ENTERED_NOT_REGULAR    6	    // whole order entered, but series not in regular mode
#define ORDER_STATUS_FAR_AWAY_BETTER	    7	    // Far away better, wait (with PMM)
#define ORDER_STATUS_FARMM		    8	    // FARMM order, wait (with PMM)
#define ORDER_STATUS_TO_LIMIT		    9	    // whole market order converted to limit
#define ORDER_STATUS_MATCH_TO_LIMIT	    10	    // market order partially matched, remainder converted to limit order
#define ORDER_STATUS_TO_BLOCK		    11	    // market order converted to block

#define OPEN_CLOSE_REQUEST_DEFAULT	    0
#define OPEN_CLOSE_REQUEST_OPEN 	    1
#define OPEN_CLOSE_REQUEST_CLOSE_NET	    2
#define OPEN_CLOSE_REQUEST_CLOSE_MAND	    3
#define OPEN_CLOSE_MAX			    3
// values packed in away market status
#define AWAY_MKT_STS_REGULAR		    1
#define AWAY_MKT_STS_FAST_MARKET	    2
#define AWAY_MKT_STS_ROTATION		    3
#define AWAY_MKT_STS_HALTED		    4
#define AWAY_MKT_STS_AUTO_EXEC		    5
#define AWAY_MKT_STS_INACTIVE		    6
// values packed in the chg_type_c (change type),
// in BI202 series update, BI204 underlying update, BI210 instrument class update
#define CHANGE_TYPE_ADD			    1
#define CHANGE_TYPE_DELETE		    2
#define CHANGE_TYPE_MODIFY		    3
// values packed in the change_reason_c (change reason),
// in BO130 (member) orderbook update, BO131 (public) combobook update, BO132 (member) combobook update
#define CHANGE_REASON_DELETE		    1
#define CHANGE_REASON_STOP		    2
#define CHANGE_REASON_MATCH		    3
#define CHANGE_REASON_INACTIVATE	    4
#define CHANGE_REASON_MODIFY		    5
#define CHANGE_REASON_ADD		    6
// values used by OM for Yes/No
#define OM_YES				    1
#define OM_NO				    2

#define USER_INPUT_BUFFER_LEN		    128
#define TEXT_BUFFER_LEN 		    128
// how many command line arguments do we cater for in the parser
#define MAX_ARGC 40

/* Error returns - these MUST be -ve for normal logic to work correctly
    By inspection (no guarantees), the OM errors do NOT seem to go as low as -2000000000,
    which is down near the low end of the -ve range for a int32
*/

#define SUCCESSFUL			    0x00000000
#define USER_ABORTED			    -2000000000
#define COULD_NOT_LOG_ON_USER		    -2000000001
#define GATEWAY_NOT_READY		    -2000000002
#define INVALID_USER_FILE		    -2000000003
#define COULD_NOT_OPEN_USER_FILE	    -2000000004
#define UNHANDLED_CASE			    -2000000005
#define API_ERROR			    -2000000006
#define TOO_MANY_QUOTE_COMPONENTS	    -2000000007
#define ORDER_NOT_FOUND 		    -2000000008
#define LINKED_LIST_MANIPULATION_FAILED     -2000000009
#define INVALID_ARGUMENTS		    -2000000010
#define NOT_LOGGED_ON			    -2000000011


// define a MAXPATH value for file pathname length for Solaris, if we haven't already got a symbol with a value
#ifndef MAX_PATH
#define MAX_PATH 2048
#endif



#endif

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



Title : global.h

Function :

    This file is the global header file for the ISESim.exe program.
    It includes global types and variables used across multiple component
    modules of ISESim
    
Author:  Neil Cossor

Revision history :-

yymmdd	vers'n	author	    notes
------	------	------	    -----
9905xx	v0.00	Neil Cossor initial version
990907	v0.01		    fix value of MAX_API_MSGNO
			    add defns for handling ex_client_s [10] as two [5] fields
			    (as per OCC trade rectification)
990910			    add specific defn for iq101 query
990920			    fix bug - query_missing_trade_x referred to wrong type
990921			    update ref for mo113_mq114, to match new omex_usa.h file
			    NB new file also allows compiler to go back to 8-byte data align
			    due to correction to ma106 structure
000107	v0.02		    update for latest OM .h files (15.7 OM release)
000113			    add fields for autoquoting (by underlying, with global defaults)
			    and new fields for synthetics
000122			    mod to be source-portable between NT and Solaris.. thanks Joshua P.\
			    import info from runtime argv[] params
000204	v.03		    change TxnStatus_i to int32, in log_reply_x and bcast_reply_x (bugfix)
			    add size terminators on const's UNDEFINED_<> and REMOVE_<> consts
				for Solaris version.
000321	v0.04		    mod ise_bin_xt to allow for a bin being spread across multiple orderbook servers.
000403	v0.05		    add stuff for uq11 (query past BI general text), uq12 (query business date)
000422	v1.00		    OM release 15.7-3. Move data from instrument groups to instrument classes.
			    incompatable with previous release.
000601	v1.01		    changed sign of opra_best_ask_premium_i (to unsigned)
000606			    change opra month and strike fields to be char, not uint8
000620			    auto_logging_b now in global, modified by om_disconnect
0012xx	v2.00		    start updates for OM release 16.3 (from 15.7-3)
			    remove answer_status_t/x.. superceded by UQ15 functionality
			    update quantities to 32 bits, and fix couple of _n names to _u names
			    update order_category to 16 bits
0101xx			    add new stuff for trading states
			    supercede some old 15.7-3 logger constants for 16.3
010226			    more chars in username in 16.3
0105xx	v2.01		    add stuff for combos (16.5), including new fields in underlying,
				and resequence some defns (some extra dependencies)
			    more chars in series name in 16.5 
010706			    add stuff for DQ30 query (came with 16.5-1)
010709			    add stuff for set/get mm exposure limit (speedbump, came with 16.5-1)

*/
#ifndef GLOBAL_H    // have we been here before in compile of this module
#define GLOBAL_H    // note that we have been here now


// ---------- constants and type definitions used by almost all code ---------


// typedef a uint64 as a structure, so we can dump out the halves as a pair of 32-bit components
typedef struct
{
    uint32	l0;
    uint32	l1;
} uint64_xt;


    // pointers to strings supplied in the argv to the main () - with default NULL values
    // pointer to the log filename
    extern char *user_log_filename_ps;
    // pointer to the script filename
    extern char *user_script_filename_ps;
    // pointer to the User param filename
    extern char *user_param_filename_ps;


static quad_word blank = {'\0','\0','\0','\0','\0','\0','\0','\0'};

// ------------------------------------------------------------------------------

/* the following UNDEFINED_<> values are used to define the "we aren't supplying a value"
    cases. These equate to NOT supplying the field at all in a FIX-style interface
    The REMOVE_<> values are used to define the "we are removing a value"
    cases. These equate to supplying a TAG=| for the field in a FIX-style interface.
    They are all 1 less than the corresponding UNDEFINED_<> values */
#define UNDEFINED_UINT8     255
#define UNDEFINED_INT8	    127
#define REMOVE_UINT8	    254
#define REMOVE_INT8	    126

#define UNDEFINED_UINT16    65535
#define UNDEFINED_INT16     32767
#define REMOVE_UINT16	    65534
#define REMOVE_INT16	    32766

#define UNDEFINED_UINT32    4294967295UL
#define UNDEFINED_INT32     2147483647L
#define REMOVE_UINT32	    4294967294UL
#define REMOVE_INT32	    2147483646L

#ifdef WIN32
    // NT NT NT NT NT NT NT NT NT NT NT NT NT NT NT NT NT NT NT NT 

    // massive uint64 # - got this # from taking undefined_int64 x 2 + 1
    #define UNDEFINED_UINT64    (uint64)18446744073709551615
    // massive int64 # - got this # from Java docs
    #define UNDEFINED_INT64     (int64)9223372036854775807
    #define REMOVE_UINT64	(uint32)18446744073709551614
    #define REMOVE_INT64	(int32)9223372036854775806

    // NT NT NT NT NT NT NT NT NT NT NT NT NT NT NT NT NT NT NT NT 
#else
    // Solaris Solaris Solaris Solaris Solaris Solaris Solaris 

    #define UNDEFINED_UINT64    (18446744073709551615ULL)
    #define UNDEFINED_INT64     (9223372036854775807L)
    #define REMOVE_UINT64	18446744073709551614ULL
    #define REMOVE_INT64	9223372036854775806LL

    // Solaris Solaris Solaris Solaris Solaris Solaris Solaris 
#endif


#define MAX_UINT64_HEX_STR  17	// how long a uint64 can be, dumped out in 8x:8x form
#define MAX_UINT64_STR	    21	// how long an uint64 can be in string form
#define MAX_INT64_STR	    22	// 1.6 x 10^20, with a possible leading sign
#define MAX_UINT32_STR	    10	// how long an uint32 can be in string form : 4 billion
#define MAX_INT32_STR	    11	// ditto, but can have sign digit if -ve : -2 billion
#define MAX_UINT16_STR	    5	// and for 16 bit : 65535
#define MAX_INT16_STR	    6	// and for signed 16 : -32767
#define MAX_UINT8_STR	    3	// and for 8 bit : 255
#define MAX_INT8_STR	    4	// and for signed 8 : -127

// constants used to tell the logger what general type of message it is logging
#define CLICK_QUERY	1   // raw structure passed to an omniapi_query call
#define CLICK_TX	2   // as above, BUT for a omniapi_tx call
#define CLICK_IN	3   // raw structure received from CLICK API call
#define CLICK_BCAST	4   // individual bcast, rx'ed as part of omniapi_read_event_ext
#define LOG_TEXT	5   // log of 'text' message.. added for informational reading of log
			    // for example, to add verbose text message when we got an error

// how many extra bytes to log, as well as actual CLICK query, including the log_head_xt header
#define CLICK_Q_EXT_HDR_SIZE	    28
// as above, but NOT including the log_head_xt
#define CLICK_Q_INT_HDR_SIZE	    16
// same pair, but for CLICK responses, rather than for query
#define CLICK_R_EXT_HDR_SIZE	    36
// as above, but NOT including the log_head_xt
#define CLICK_R_INT_HDR_SIZE	    24
// as above, but for received broadcasts
#define CLICK_B_EXT_HDR_SIZE	    16

/* Note that the header of CLICK output msgs and CLICK input msgs
    are different - which is important for logging
    ??? update to reflect current structure ???
click_out :- 

| length | msg_code | req_id | fn_code, facility (4bytes),
			    CLICK header (int len, module, server, trans #),
			    then varying query structure
click_in :-
| length | msg_code | status | fn_code, secondary status (4bytes), if OK ->
			    TransID, OrderID, each 8 bytes
			    then varying response structure
click bcast :_
| length | msg_code | status | fn_code (4 bytes) |
			    then varying bcast structure, starting with trans header

text message
| length | 0 | 0 | then varying text string, NUL terminated
*/

// --------------------------------------------------------------

// structure for what is at head of every logged msg
typedef struct {    // log_head_xt
    uint32  msg_len_u;	    // how long the message is
    uint32  msg_code_u;     // which API function this msg holds : MC_CMD_<> or MC_RESP_<>
    int32   msg_status_i;   // status of the operation
}   log_head_xt;


/* ----------  fn_code constants for logging msgs --------------

    CLICK messages - i.e. as per OM spec / header files

    These are the internal fn_code values which we pass to the logger,
    so the logger(.h/.c) knows how to dump each msg.

    The logger, in turn, uses a definition file which has a description of each message.
    This description file MUST BE KEPT UP TO DATE RELATIVE TO THE OMEX_USA.H FILE

    We will precede the name of the
    queries with LOG_K_Q_
    responses with LOG_K_R_
    commands (e.g. logon, send order) with LOG_K_C_
    broadcasts with LOG_K_B_

    Note that raw CLICK messages use odd formats.
    For example strings are fixed length, and are NOT NUL terminated,
    and are in ISO-Latin-1.
    Numbers are in little endian order, as per the VAX / Intel CPUs.
    If we are on a big-endian system (SUN Sparc), we must reverse the bytes in logs etc.

********************************************************************************/

// input messages to CLICK
					    // corresponding omniapi structure name
#define LOG_K_C_LOGON		    2000    // omni_logon_message - answer is a LOG_K_STATUS
#define LOG_K_C_LOGOUT		    2001    // no structure - it is just a CLICK header
#define LOG_K_Q_SERIES_SEGMENTS     2002    // standard query, with series, in segments,
					    // == query_instrument_t, query_instrument_group_t,
					    // query_underlying_t, query_series_t, query_market_t,
					    // query_instrument_class
					    // query_mm_parameters_uq101_t, query_pmm_parameters_uq102_t
					    // query_dependencies_dq212_t, query_instrument_status_t
					    // query_mm_exposure_limit_uq103_t - query speedbump
#define LOG_K_Q_SERIES_BIN	    2003    // standard query, with series, and with Bin selector (segmented)
#define LOG_K_Q_BROKER		    2004    // query_broker - dq206
#define LOG_K_C_APPLICATION_STATUS  2005    // application_status_t - answer is a LOG_K_STATUS
//
#define LOG_K_Q_INACTIVE	    2006    // query_inactive mq102
#define LOG_K_Q_MISSING_TRADES	    2007    // query_missing_trade - cq1 (todays)
#define LOG_K_C_ORDER_TRANS	    2008    // order_trans - mo101
#define LOG_K_C_ALTER_TRANS	    2009    // alter_trans - mo103
#define LOG_K_C_DELETE_TRANS	    2010    // delete_trans - mo104
#define LOG_K_Q_SERIES_PRICES	    2011    // query_prices_iq101_t
#define LOG_K_C_QUOTE_TRANS	    2012    // quote_trans_mo106_t - price quotation,
					    // and delete old if still in)
#define LOG_K_C_BLOCK_TRANS	    2013    // block_order_mo107_t
#define LOG_K_C_FACILITATION_TRANS  2014    // facilitation_order_mo108_t
#define LOG_K_C_BLOCK_FACIL_RESPONSE 2015   // block_order_response_mo109_t
#define LOG_K_C_CAB_ORDER_TRANS     2016    // cab_order_mo110_t
#define LOG_K_Q_PAST_TRADES	    2017    // query_trade - cq2 (past days trades)
#define LOG_K_C_RECTIFY_TRADE_OCC   2018    // rectify_trade_occ_us_t
#define LOG_K_C_SET_MM_PARAMS	    2019    // set_mm_parameters_ui101_t
#define LOG_K_Q_BIN_PMM_MM	    2020    // query_bin_dq211_t
#define LOG_K_Q_ORDERBOOK	    2021    // query_ob_mq103_t
#define LOG_K_Q_QUOTE		    2022    // query_quote_mq104_t - like a query_order, but no order category
#define LOG_K_Q_LOCKED_ORDER	    2023    // query_locked_order_mq106_t
#define LOG_K_C_ROTATION	    2024    // mo113, rotation_request_mo113_mq114
#define LOG_K_Q_ROTATION	    2024    // mq114
#define LOG_K_C_SET_PMM_PARAMS	    2025    // set_pmm_parameters_ui102_t
#define LOG_K_C_TRADE_REPORT	    2026    // pmm_trade_report_trans_mo112_t
#define LOG_K_C_RELEASE_LOCK	    2027    // pmm_release_locked_order_mo111_t
#define LOG_K_Q_OPRA_PRICES	    2028    // query_opra_prices_iq111_t
#define LOG_K_Q_EACH_OPRA_PRICES    2029    // query_each_opra_prices_iq112_t
#define LOG_K_Q_PAST_GENERAL_TEXT   2030    // query_bi8_broadcasts_sent_t - UQ11
#define LOG_K_Q_BUSINESS_DATE	    2031    // query_business_date_t - UQ12
#define LOG_K_C_RECTIFY_TRADE_ISE   2032    // rectify_trade_ise_t - cd36, rectify by parts
#define LOG_K_Q_ORDER		    2033    // query_order - mq101 - no longer same as mq102
#define LOG_K_Q_COMBO		    2034    // query_combo_order_mq131_t, query_inactive_combo_mq132_t,
					    // query_combo_ob_mq133_t - all the same format
#define LOG_K_C_COMBO_TRANS	    2035    // combo_order_trans_mo131_t - mo131
#define LOG_K_C_ALTER_COMBO_TRANS   2036    // alter_combo_trans_mo133_t - mo133
#define LOG_K_C_HIT_COMBO_TRANS	    2037    // execute_combo_trans_mo136_t - mo136
#define LOG_K_C_SPEEDBUMP	    2038    // set_mm_exposure_limit_ui103_t - set speedbump params


// output messages from CLICK

#define LOG_K_STATUS		    2050    // just a status (with transID and orderID, e.g. a logon completion)
#define LOG_K_R_EXCHANGE	    2051    // answer_exchange - da24
#define LOG_K_R_SERIES		    2052    // answer_series_da202_t - da202
#define LOG_K_R_INSTRUMENTS	    2053    // answer_instrument_t - da203
#define LOG_K_R_UNDERLYING	    2054    // answer_underlying_t - da204
#define LOG_K_R_BROKER		    2055    // answer_broker - da206
#define LOG_K_R_MARKET		    2056    // answer_market_t - da207
#define LOG_K_R_INSTRUMENT_GROUPS   2057    // answer_instrument_group_t - da208
#define LOG_K_R_INSTRUMENT_CLASS    2058    // answer_instrument_class_da210_t - da210
#define LOG_K_R_BIN_PMM_MM	    2059    // answer_bin_da211_t
#define LOG_K_R_SERIES_PRICES	    2060    // answer_prices_ia101t
#define LOG_K_R_UNDERLYING_PRICES   2061    // answer_underlying_prices_ia121_t
					    // free - no answer_status_t in 16.3+
#define LOG_K_R_ORDER		    2063    // answer_order - ma101
#define LOG_K_R_INACTIVE	    2064    // answer_inactive - ma102
#define LOG_K_R_UNDERLYING_STATII   2065    // answer_underlying_status_us_t - ia132

#define LOG_K_R_MISSING_TRADES	    2066    // answer_missing_trade - ca1 (today)
#define LOG_K_R_PAST_TRADES	    2067    // answer_trade - ca2 (past days trades)
#define LOG_K_R_GET_MM_PARAMS	    2068    // answer_mm_parameters_ua101_t
#define LOG_K_R_GET_PMM_PARAMS	    2069    // answer_pmm_parameters_ua102_t
#define LOG_K_R_ORDERBOOK	    2070    // answer_ob_ma103_t
#define LOG_K_R_QUOTE		    2071    // answer_quote_ma104_t
#define LOG_K_R_LOCKED_ORDER	    2072    // answer_locked_order_ma106_t
#define LOG_K_R_ROTATION	    2073    // answer_rotation_ma114_t
#define LOG_K_R_OPRA_PRICES	    2074    // answer_opra_prices_ia111_t
#define LOG_K_R_EACH_OPRA_PRICES    2075    // answer_each_opra_prices_ia112_t
#define LOG_K_R_INSTR_STATUS	    2076    // answer_instrument_status_t
#define LOG_K_R_DEPENDENCIES	    2077    // answer_dependencies_da212_t
#define LOG_K_R_PAST_GENERAL_TEXT   2078    // answer_bi8_broadcasts_sent_t - UA11
#define LOG_K_R_BUSINESS_DATE	    2079    // answer_business_date_t - UA12

// new for 16.3
#define LOG_K_R_TRADING_STATES      2080    // answer_trading_state_da245_t
// new for 16.5
#define LOG_K_R_COMBO_ACTIVE	    2081    // answer_combo_order_ma131_t, member's active combos
#define LOG_K_R_COMBO_INACTIVE	    2082    // answer_inactive_combo_ma132_t, member's inactive combos
#define	LOG_K_R_COMBOBOOK	    2083    // answer_combo_ob_ma133_t, public combobook
#define LOG_K_R_USER_TYPE	    2084    // answer_user_type_info_t, da30 response to dq30
#define LOG_K_R_SPEEDBUMP	    2085    // answer_mm_exposure_limit_ua103_t - response to uq103 speedbump

// broadcasts sent out by CLICK
// spares 2045-49
#define LOG_K_B_HEADER		    2100    // just the header of the broadcast
// BD's
#define LOG_K_B_DEDICATED_TRADE     2101    // BD5, directed_trade_t
#define LOG_K_B_TRADE_INFO	    2102    // BD101, trade_info_bd101_t
#define LOG_K_B_OPRA_TRADE	    2103    // BD102, opra_trade_info_bd102_t
#define LOG_K_B_UNDERLYING_INFO     2104    // BD103, underlying_trade_info_bd103
#define LOG_K_B_DEAL_MARKET	    2105    // BD111, deal_user_bd111_t
// BI's
#define LOG_K_B_INSTRUMENT_STATUS   2110    // BI41, instrument_status_info_t, pre 16.3 used this # for BI2
#define LOG_K_B_INFO_HEARTBEAT	    2111    // BI9, info_heartbeat_t
#define LOG_K_B_GENERAL_TEXT	    2112    // BI101, general text, message_bi101_t
#define LOG_K_B_SERIES_STATUS_CHG   2113    // BI103, series_status_change_bi103_t
#define LOG_K_B_AWAY_MARKET_CHECK   2114    // BI114, away_market_flag_update_us_t
#define LOG_K_B_UNDER_STATUS_CHG    2115    // BI132, underlying_trading_status_bi132_t
#define LOG_K_B_SERIES_UPDATE	    2116    // BI202, series_update_bi202_t
#define LOG_K_B_UNDER_UPDATE	    2117    // BI204, underlying_update_bi204_t
#define LOG_K_B_INSTR_CLASS_UPDATE  2118    // BI210, instrument_class_update_bi210_t
// BO's
#define LOG_K_B_BEST_BID_OFFER	    2120    // BO101, best_bid_offer_bo101_t
#define LOG_K_B_EXT_BEST_BID_OFFER  2121    // BO102, best_bid_offer_extended_bo102_t
#define LOG_K_B_OPRA_BEST_BID_OFFER 2122    // BO103, opra_best_bid_offer_bo103_t
#define LOG_K_B_EACH_BEST_BID_OFFER 2123    // BO104, opra_all_best_bid_offer_bo104_t
#define LOG_K_B_CQS_BEST_BID_OFFER  2124    // BO105, underlying_best_bid_offer_bo105_t
#define LOG_K_B_BLOCK_FACIL_ORDER   2125    // BO106, block_facilitation_order_bo106_t
#define LOG_K_B_ORDER_QUOTE_DELETED 2126    // BO107, order_quote_deleted_bo107_t
#define LOG_K_B_QUOTE_REJECT_REASON 2127    // BO108, quote_rejection_reason_bo108_t
#define LOG_K_B_QUOTE_STEP_UP	    2128    // BO109, quote_stepped_up_bo109_t
#define LOG_K_B_QUOTE_TICK_WORSE    2129    // BO110, quote_tick_worse_bo110_t
#define LOG_K_B_BLOCK_FACIL_ENDED   2130    // BO112, block_order_ended_bo112_t
#define LOG_K_B_PMM_LOCKED_ORDER    2131    // BO115, locked_order_bo115_t
#define LOG_K_B_PMM_DERIVED_ORDER   2132    // BO116, derived_order_bo116_t
#define LOG_K_B_OB_CHANGES	    2133    // BO130, ob_changes_bo130_t
#define LOG_K_B_COMBO_OB_CHANGES    2134    // BO131, combo_ob_changes_bo131_t
#define LOG_K_B_COMBO_OB_CHANGE_OWN 2135    // BO132, combo_ob_member_bo132_t
// BN's
#define LOG_K_B_NETWORK_STATUS	    2140    // BN1, network status (in API docs NOT in SPM)
//
// the upper and lower values of above-defined fn_code values - used by the logger
#define MIN_API_MSGNO		    2000
#define MAX_API_MSGNO		    2140

 
#define MAX_ABBR_EXCHANGE_NAME	4   // the max length of an abbreviated exchange name
#define MAX_CMTA_MEMBER	    5	// max length of CMTA member ID string
#define MAX_COMBO_LEGS	    10	// max # of legs in a combo.. this is at the API level.. at business, limited to 4
#define MAX_COMBO_MESSAGE   100	// max length in chars in message text field of a combo
#define MAX_COUNTRYIDS	    2	// max chars in a CLICK country_id_s
/* how many simultaneous country/market combinations we support
    - ISE only has 1 as at 9905xx (derivatives).
    Maybe one for derivatives indices, which might (for example)
    run over different hours than the main market
*/
#define MAX_COUNTRY_MARKET  1
#define MAX_CURRENCY	    3	// max len of currency name e.g. USD, GBP
#define MAX_CUSTOMER_ID     5	// max chars in a CLICK customer_id string i.e.
				// an exchange customer.
#define MAX_DATE	    8	// max chars of purely date string, YYYYMMDD.
#define MAX_FREE_TEXT_80    80	// max text in a OM free_text field
#define MAX_ISIN	    12	// length of an ISIN / CUSIP code
#define MAX_MMS_IN_BIN	    20	// how many MMs per bin we cater for (in internal structures)
#define MAX_NAMES	    32	// max len of CLICK names e.g. Market name, exchange name, underlying name
#define MAX_OBS_FOR_BIN	    20	// how many orderbooks each bin can be spread across.. this is way too big
#define MAX_PRM_MM_CUST     7	// max len of customer, primary MM.
/* ???
prm_mm_customer_s == customer_s in the docs.. but in the omex_usa, is text, max 7.

NB ex_customer_s (Customer ID) is different from customer_s (Customer).
This latter appears to be recursive in SPM v1.63

NB this MAY be temp, as this looks like an API goofup.
*/
#define MAX_SERIES_NAME     32	// max length of CLICK series name.
#define MAX_SERIES_PREMIUM  9	// max length of report string of series premiums e.g. bid/ask/quote value
#define MAX_SUB_ACCOUNT_ID  5	// max length of CMTA member sub account ID string
#define MAX_SYMBOL_NAME     7	// max length of symbol name
#define MAX_TIME	    8	// max len of a purely time string.
				// this includes ':' chars. hh:mm:ss
#define MAX_TRADE_STATE	    20	// max length of name of a trading state - new 16.3
#define MAX_UNDER_ID	    6	// max len of short Underling ID name
#define MAX_UNDER_PRICE     9	// max length of report string of underlying price e.g. hi/low/last
#define MAX_USER_ID	    5	// max chars in a user_id_s - NB NOT same as user code
#define MAX_USER_TYPE_ID    5	// max chars in the name of a user-type
#define MAX_XREF	    15	// max chars in a XREF in an order - called free_text_s

typedef char abbr_exchange_name_st
				[MAX_ABBR_EXCHANGE_NAME +1];	// short (abbreviated) exchange name
typedef char combo_message_st	[MAX_COMBO_MESSAGE  +1];    // message field of combo
typedef char country_id_st	[MAX_COUNTRYIDS     +1];
typedef char currency_st	[MAX_CURRENCY	    +1];    // currency name
typedef char customer_id_st	[MAX_CUSTOMER_ID    +1];
typedef char date_st		[MAX_DATE	    +1];    // date, yyyymmdd
typedef char free_text_80_st	[MAX_FREE_TEXT_80   +1];    // free_text_80_s == text_line_s
typedef char isin_st		[MAX_ISIN	    +1];    // ISIN code
typedef char name_st		[MAX_NAMES	    +1];    // full OM name - series, underlying...
typedef char prm_mm_customer_st [MAX_PRM_MM_CUST    +1];    // primary MM name, incl country ??? DFI is recursive in v1.63
typedef char series_st		[MAX_SERIES_NAME    +1];    // series name
typedef char series_premium_st	[MAX_SERIES_PREMIUM +1];    // how large a string we may report a series premium as
typedef char trade_state_id_st	[MAX_TRADE_STATE    +1];    // how long the name of a trading state can be
typedef char symbol_name_st	[MAX_SYMBOL_NAME    +1];    // symbol name
typedef char time_st		[MAX_TIME	    +1];    // hh:mm:ss
typedef char under_id_st	[MAX_UNDER_ID	    +1];    // short underyling code name
typedef char under_price_st	[MAX_UNDER_PRICE    +1];    // how large a string we may report an underlying price as
typedef char user_id_st 	[MAX_USER_ID	    +1];
typedef char xref_st		[MAX_XREF	    +1];    // cross-reference string
typedef char cmta_member_st	[MAX_CMTA_MEMBER    +1];    // CMTA member ID string
typedef char sub_account_id_st	[MAX_SUB_ACCOUNT_ID +1];    // sub account ID of CMTA
typedef char user_type_id_st	[MAX_USER_TYPE_ID   +1];    // name of a user-type.. associated with transactions/queries/bcasts



typedef union {
	struct {
		unsigned long l1;
		unsigned long l2;
	};
	quad_word q;
}quad_conv_t;
// --------------------------------------------------------------

/*  structure for storing Q linking info. These are standard in ALL Q's and Q headers,
    and MUST be the first field in any structure to be inserted into a Q
*/
struct q_chain_t
{
    struct q_chain_t	    *prev;  // pointer to previous in the chain
    struct q_chain_t	    *next;  // pointer to next in the chain
};  // q_chain_t

// --------------------------------------------------------------

/* structure for storing a msg - or any block of data - queued in FIFO order
    This is a generic buffer node, which may be Q'd.
    How it is used it up to the code e.g. the *msg_p part may point to any piece
    of memory... a C string, a structure etc
*/
typedef struct {
    struct q_chain_t  chain;	    // Q link data - MUST be first field of structure
    char	      *msg_p;	    // pointer to the actual buffer data
    int 	      msg_len_i;    // size of the actual msg, pointed to by msg_p
    int 	      msg_code_u;   /* msg code of the block (optional)
					Actually, you can use this field for anything
					that you want to handshake between the
					code putting generating the block,
					and any code which consumes or examines
					the block
				    */
}   gen_buf_node_xt;
// and a pointer to one
typedef gen_buf_node_xt * gen_buf_node_pt;

// --------------------------------------------------------------

// structure for header of a Q of gen_buf_nodes
typedef struct node_header
{
    struct q_chain_t	    chain;	    // Q link data - MUST be first field of structure
    unsigned		    length_i;	    // size of the Q - updated in normal add/remove routines
    unsigned		    max_length_i;   // max size the Q reached - "  "  "

    // multi-thread synchronisation var for this structure
    // use single-entry sync variable - only one thread may access the header at a time
    ise_mutex_t hMutex;
} node_header_xt;

// --------------------------------------------------------------

typedef struct {
    // a non-string date. Required for the extensive date-based operations in CLICK
    // 2 bytes for year, and 1 each for month & day - leaves us a neat total size
    uint16  year_u;	    // absolute, i.e. since 0AD, not some weird 19xx offset
    uint8   month_u;
    uint8   day_u;
} date_xt;

// --------------------------------------------------------------

/* a local version of a OM defined series_t.
    NB series_t == orig_series_t == new_series_t
    == upper_level_series_t == other_series_t
    you get the picture...
*/
typedef struct {    // ise_series_xt
    // strike price - don't know why CLICK has this as signed.
    int32   strike_price_i;
    // commodity. This is the CLICK number of the underlying e.g. 3 = IBM
    uint16  underlying_code_u;
    // expiry date, exploded from the bit fields of CLICK, and in true terms
    date_xt expiry_date_x;
    // country code e.g. 20 == USA/ISE. NB these aren't really country codes - see country_market_xt
    uint8   country_u;
    // market code e.g. 1 == Derivatives Market, within this country code
    uint8   market_u;
    // instrument group. This is a CLICK allocated #, e.g. 7 == American Put option
    uint8   instrument_group_u;
    // modifier count. This is the # of times the series has been altered e.g. splits / restructures.
    // starts out as 0 for all series
    uint8   modifier_count_u;
    // there is an invisible 2-byte filler added by C - make it explicit, so we know about it
    uint16  filler;
} ise_series_xt;

// --------------------------------------------------------------

typedef struct // instrument_class_xt
/* Type for info about one instrument class. Info from DQ210
    These are within a particular country / market / underlying (commodity).
*/
{
    // description of series this instrument class describes
    ise_series_xt   series_x;
    // series (normally the equity) that the derivative is based on. Null if deriv level==0
    ise_series_xt   upper_level_series_x;
    int32	    price_quote_factor_i;
    int32	    contract_size_i;
    int32	    exerc_limit_i;
    int32	    bbo_volume_i;
    int32	    block_order_min_size_i;
    int32	    block_order_rsptime_i;
    int32	    fac_order_min_size_i;
    int32	    fac_order_rsptime_i;
    uint16	    derivative_level_u;
    uint16	    dec_in_strike_price_u;
    uint16	    dec_in_contr_size_u;
    int16	    spg1_low_limit_u;
    int16	    spg2_low_limit_u;
    int16	    spg3_low_limit_u;
    int16	    spg4_low_limit_u;
    int16	    spg5_low_limit_u;
    uint8	    exerc_limit_unit_u;
    uint8	    traded_u;
    /* derived from what the tick-table contains
	As at 2000/04/22, false if first tick is 625, second is 1250, true otherwise */
    uint8	    trades_in_decimals_u;

    // how many expiration date group records we have
    uint8	    exp_low_limit_count_u;
    // expiration date group, lower limit - not all are filled
    uint16	    exp_date_group_low_limit_u [20];

    // 2000/04/22 - om release 15.7-3.. fields moved from instrument group above
    // ranking type
    uint16  rnt_id_u;

    // count of how many tick-size records we have. As per block_size_u, only need 1 byte
    // note, this table is the valid price ticks for orders / quotes etc
    uint16  tick_count_u;   // max value of 12
    struct
    {
	int32	step_size_i;
	int32	lower_limit_i;
	int32	upper_limit_i;
    }	    tick_size_x [12];

    // count of how many MM rule tick-size records we have. From 16.3 on, we have
    // a secondary tick-table, which defines what the ticks are for the MM tick-worse,
    // derived orders, and match-away. The prices in this should be an integral
    // multiple of those in the normal price-tick table, say 1:1, or 1:5
    uint8   mm_rule_tick_count_u;   // max of 12
    struct
    {
	int32	step_size_i;
	int32	lower_limit_i;
	int32	upper_limit_i;
    }	    mm_rule_tick_size_x [12];

    // decimals in premium
    uint16  dec_in_premium_u;


    // the base currence (string.. e.g. USD, GBP)
    currency_st     base_cur_s;

    // current trading state number.. this may be defaulted by the instrument type,
    // or the market settings, or may override those values
    uint16		state_number_u;

}   instrument_class_xt;
// and a pointer to one
typedef instrument_class_xt *instrument_class_pt;

// --------------------------------------------------------------

// stuff we store about one underlying (from query_static_underlying, in static.c)
typedef struct {    // ise_underlying_xt
    // this field has no Detailed Field Information in the v1.63 doc draft
    int64_t	nominal_value_u;
    // position limit, in contracts, for one holder across the whole underlying
    int64_t	position_limit_u;
    // coupon interest. This has an implied 6 decimal pt e.g. 11% == 110000
    // ??? should this be signed - the DFI does NOT say
    uint32	coupon_interest_u;
    // now comes a block of data, for prices and volumes
    // from iq121, bo105, bd103
    int32	best_bid_premium_i;
    int32	best_ask_premium_i;
    int32	closing_price_i;
    int32	opening_price_i;
    int32	high_price_i;
    int32	low_price_i;
    int32	last_price_i;
    uint32	volume_u;
    uint32	turnover_u;
    // time we got last updates - from query or broadcast
    time_t	time_bbo_update_u,
		time_trade_update_u;
    

    // == commodity_n. This is the CLICK number of the underlying e.g. 40 = BHP
    uint16	commodity_code_u;
    // # of days in a year used for interest rate calculation
    uint16	day_count_u;
    // as above for day_count_u, but for the coupon period
    uint16	days_in_interest_year_u;
    // # of settlement days at coupon
    uint16	coupon_settlement_days_u;
    // == dec_in_nominal_n, # of decimal digits in the price
    uint16	dec_in_price_u;

    // now we can have up to 20 sets of coupon data, which describe (??) this underlying
    // count of coupons
    uint16	coupons_u;
    struct
    {
      // coupon date - massaged from CLICK yymmdd_s, as per release and
      // termination dates above
      date_st	coupon_yyyymmdd_s;
      uint32	dividend_u;
    } coupon_x [20];

    // and we can have up to 10 fast market levels. How many do we have..
    uint16	fast_levels_u;
    struct
    {
     int32	match_interval_i;   // in milliseconds
    } fast_level_x [10];

    // primary country code. Ref DFI for Country Number, Primary
    uint8	country_u;
    // bin #
    uint8	bin_u;
    // orderbook #
    uint8	orderbook_u;
    // underlying trading status - from bi132 bcast and iq132 query
    uint8	trading_status_u;
    // 1 == stock, 2 == currency... ref Detailed Field Information
    uint8	under_type_u;
    // 1 == money, 2 == interest, 3 == points, def Detailed Field Information
    uint8	under_price_unit_u;
    // NASDAQ tick value for latest BBO bid, 0 if non-NASDQA
    uint8	nasdaq_bid_tick_u;
    // NASDAQ UPC 11830 restricted security flag, 0 if unrestricted or non-NASDAQ
    uint8	nasdaq_upc_u;
    // the originator ID
    uint8	originator_id_u;

    // the local exchange code for the underlying - i.e. short name
    under_id_st under_id_s;
    // == name_s, full name of the underlying
    name_st	under_fullname_s;
    // the CUSIP code - I think this corresponds to ISIN code
    // NB clash in the defn - the SPM says 12 (OK for ISIN), while the docs say 15
    isin_st	cusip_code_s;
    // currency name e.g. USD, GBP (Great Britain Pounds), AUD (AUstralian Dollars)
    currency_st     currency_s;
    // (string) release and termination dates.
    date_st	release_yyyymmdd_s;
    date_st	termination_yyyymmdd_s;
    // == prm_mm_customer_s, ref DFI for Customer, Primary Market Maker
    prm_mm_customer_st	prm_mm_s;

    // the Q of series which relate to this underlying
    node_header_xt	under_series_hdr_x;
    /* pointer to first tradable instrument class for this underlying.
	Handy for some things for combos.
	Note that we have cross-checked all tradeable instr classes at 
	static-load for consistency, so one should be as good as another */
    instrument_class_pt	traded_instr_class_px;

    // the default yield and interest rate for autoquoting
    // NB these are c.f. 1, not a %.. so must * or / 100 for % e.g. 10% is stored as 0.10
    double	    volatility_f,
		    interest_f;
    // are we autoquoting, value TRUE if autoquoting
    int		    autoquoting_b;

    // the default quantity for a quote
    uint32 autoquote_quantity_u;
    // the default open quantity for a quote (only used in a rotation)
    uint32 autoquote_open_quantity_u;
    // default step-up buffer to set on a new quote
    uint16 autoquote_step_up_buffer_n;

}   ise_underlying_xt;

// and a pointer to a ise_underlying_xt
typedef ise_underlying_xt *ise_underlying_pt;

// --------------------------------------------------------------

typedef struct // instrument_xt
// Type for info about one instrument type. Info from DQ208 and DQ203.
// These are an instrument group within a particular country / market.
{
    int32   bbo_volume_i;
    int32   block_order_min_size_i;
    int32   block_order_rsptime_i;

    // country code - 15 == Oz, 20 == ISE
    uint8   country_u;
    // market within country. In ISE country, market 1 == Derivatives
    uint8   market_u;
    // the group # e.g. 5
    uint8   instrument_group_u;
    // the name of the instrument e.g. American Put options
    name_st instrname_s;

    // current trading state number.. this may be defaulted by the market settings, 
    // or may override that values, and the instrument class value may override this
    uint16  state_number_u;

}   instrument_xt;

// --------------------------------------------------------------

// we have added in all of the useful info that we care about, and have
// extracted from DQ queries, and any update broadcasts
typedef struct {    // ise_series_info_xt
    // the contract size
    uint32	    contract_size_u;
    /* the series info (massaged from the CLICK storage series_t)
	NB contains the country and market IDs for this series
    */
    ise_series_xt   ise_series_x;
    // the name of the series.
    // NB ins_id_s (of answer_series_da202_t) are the same
    series_st	    ise_name_s;
    // the symbol name
    symbol_name_st  ise_symbol_name_s;
    // now comes a block of data, for prices, volumes, status (from iq101, bo101, bd101, bi103)
    int32	    best_bid_premium_i;
    int32	    best_ask_premium_i;
    // the prices that we quoted at last (manual or autoquote)
    int32	    quote_bid_premium_i;
    int32	    quote_ask_premium_i;
    // quantity we quoted last (manual or autoquote)
    uint32	    quote_bid_qty_u,
		    quote_ask_qty_u;
    // quote ID (i.e. order ID) for the last quote (manual or autoquote)
    quad_word	    bid_quoteID_u,
		    ask_quoteID_u;

    uint32	    best_bid_size_u;		// # of contracts at BBO
    uint32	    best_ask_size_u;		// "
    uint32	    opra_best_bid_premium_i;
    uint32	    opra_best_ask_premium_i;
    uint32	    opra_best_bid_size_u;	// # of contracts at BBO
    uint32	    opra_best_ask_size_u;	// "
    // stuff in preopen - agregated 
    uint32	    bid_market_order_size_u;	// # of contracts, with "at Market" flavour, buy side
    uint32	    ask_market_order_size_u;	// ditto, on the sell side

    int32	    closing_price_i;		// yesterdays close price.. presume this is a buy-sell midpoint
    int32	    opening_price_i;		/* price we opened at.. actually the last opening price
						    as it is possible to open more than one time per day */
    int32	    high_price_i;		// day high buy
    int32	    low_price_i;		// day low sell
    int32	    last_price_i;		// last trade price
    uint32	    volume_u;			// volume in the last trade
    uint32	    turnover_u; 		// total traded volume today
    uint32	    open_balance_u;
    uint16	    mm_number_u;		// preopen, # of MMs quoting in side with most quotes
    uint16	    fast_market_interval_u;
    uint16	    fast_market_level_u;	// current fast-market level - must be 1..8
    // series status is IMPORTANT !!!
    uint8	    series_status_u;
//  fast_market_indicator_u removed 20000105, no longer required
    uint8	    opra_best_bid_exchange_u,	// and the market where the last opra BBO came from
		    opra_best_ask_exchange_u,
		    opra_best_bid_exchange_status_u,	// market status of wherever the last opra BBO came from
		    opra_best_ask_exchange_status_u;

    // time the BBO price / volumes were last updated. If zero, then a command to show them
    // must do a query. Should be updated on successful query or bcast, zeroed on logoff
    time_t	    time_bbo_update_u,
    // and time the (overall) OPRA bbo figures were updated
		    time_opra_bbo_update_u;

    // the month code and strike codes - NB these are NOT adequately described in the OM SPM
    char	    ise_month_c;    // 'A'..'L', calls Jan to Dec... 'M'..'X', puts Jan to Dec
    char	    ise_strike_c;   // 'orribly messy/complicated.. A == $5, $105, $205.. except for splits etc. urk.

    // current active block/facilitation order ID - from bo106 broadcast
    quad_word	    block_facil_orderID_u;

    // a pointer to the parent underlying
    ise_underlying_pt	parent_under_px;
    // a pointer to the descriptive instrument class
    instrument_class_pt instrument_class_px;
    // pointer to the instrument group for this series
    instrument_xt	*instr_group_px;

    // some extra things for autoquoting
    // the strike price, as a double
    double	    strike_f;
    // the # of years from now until the expiry, as a double
    // NB as we are kicked off daily, this data will be generated anew each day
    double	    years_to_expiry_f;
} ise_series_info_xt;

// and a pointer to a ise_series_info_xt
typedef ise_series_info_xt *ise_series_info_pt;

// --------------------------------------------------------------

/* stuff we keep about a bin, within a country/market
    A bin is a collection of underlying (and their related series).
    While not technically required, ISE should enforce (hopefully
    official rules updated to that effect) that all underlying in
    a bin are processed by the same orderbook server.
    If this is NOT done, there are some interesting implications
    to some of the 'big' queries, which can roll across multiple
    bins and orderbooks...
*/
typedef struct {    // ise_bin_xt
    // the bin number
    uint8		bin_u;
    // how many orderbooks this bin is spread across
    uint8		orderbooks_u;
    // and the orderbooks which handles this
    // NB 20000321.. this used to assume only one OB per bin
    uint8		orderbook_u[MAX_OBS_FOR_BIN];

    // a chain of the underlying in the bin.. we use the msg_p of a standard
    // gen_buf_node_xt to point sideways to the actual underlying records
    node_header_xt	bin_under_hdr_x;

    // the info about the PMM/MMs for the bin - from dq211/da211
    customer_id_st	pmm_id_s;
    // how many MMs there are in the bin
    uint8		mms_u;
    // and the array of MM ids for the bin.. be generous
    customer_id_st	mm_ids_s[MAX_MMS_IN_BIN];
} ise_bin_xt;

// --------------------------------------------------------------

/* stuff we keep about an exchange. These are include both underlying
    and derivatives exchanges. Filled in from a dq24 query, at logon
*/
typedef struct {    // ise_exchange_xt
    // the OPRA indicator
    uint8		    opra_indicator_u;
    // the country code.. these should (hopefully) correspond to numbers in omnifact.h
    uint8		    country_u;
    // the full name of the exchange - rarely used
    name_st		    long_name_s;
    // the abbreviated name - which is what everyone knows and loves
    abbr_exchange_name_st   short_name_s;
} ise_exchange_xt;

// --------------------------------------------------------------

typedef struct {
// ISEsim equiv of user_code_t == trading_code_t == countersign_code_t
    country_id_st   country_id_s;
    customer_id_st  customer_id_s;	// == broker firm
    user_id_st	    user_id_s;		// == operator ID
} ise_user_code_xt;

// --------------------------------------------------------------

// what we store about one order
typedef struct { // order_entry_xt
    // pointer to series
    ise_series_info_xt	*series_px;
    // order ID - 0 if not sent in yet
    quad_word		orderID_u;
    // premium (implied decimals buried in Instrument Class info)
    int32		premium_i;
    // quantity
    uint32		quantity_u;
    // bid / ask qualifier
    int8		bid_ask_u;
    // cross-reference string
    xref_st		userXRef_s;
    // in OM, the brokerID.. otherwise known as operator ID
    user_id_st		broker_oper_id_s;
    uint16		localNumber_n;	        // local order #
    uint16		validity_time_u;    // GTC/ROD/IOC/FOK - NOT in facilitation orders
    uint8		order_type_u;	    // limit/market - only in plain orders
    uint8		client_category_u;  // cust/Firm/FARMM
    uint8		ignore_away_u;	    // 1 == ignore - only in plain orders
    uint8		lock_type_u;	    // hidden type
    uint16		order_category_u;   // plain, block, facilitation etc
    uint8		expose_flag_u;	    // only for block & facilitation orders
    uint8		open_close_u;	    // behaviour for OCC settlement account
    uint8		broker_percentage_u;// for facilitation order
    cmta_member_st	cmta_member_s;	    // CMTA member ID string
    sub_account_id_st	sub_ac_s;	    // sub account ID string
    ise_user_code_xt	user_code_x;	    // whose order is this (could be another user in the member)
} order_entry_xt;

// --------------------------------------------------------------

// what OM packs about one combo leg. As of 16.5, these are always the same format
// in MA131, MA132, BO131, BO132, MO131 although this could change.
typedef struct { // OM_one_combo_leg
   series_t series;
   uint16 multiplier_n;
   uint8 bid_or_ask_c;
   uint8 open_close_req_c;  // not set in public MA131, BO131
} OM_one_combo_leg_xt;

// what we store about one combo leg
typedef struct { // combo_leg_xt
    // pointer to series
    ise_series_info_xt	*series_px;
    // bid / ask qualifier
    int8		bid_ask_u;
    // the ratio between this leg and others in the combo.. typically 1
    uint16		multiplier_u;
    // behaviour for OCC settlement - unknown for public combos
    uint8		open_close_u;
} combo_leg_xt;

// what we store about one combo (order)
typedef struct { // combo_entry_xt
    // pointer to underlying - all items of a combo are in same under, or nominated to be (if no legs)
    ise_underlying_xt	*underlying_px;
    // order ID - 0 if not sent in yet
    quad_word		orderID_u;
    // premium (implied decimals buried in Instrument Class info) - can be negative
    int32		premium_i;
    // quantity
    uint32		quantity_u;
    // cross-reference string
    xref_st		userXRef_s;
    // in OM, the brokerID.. otherwise known as operator ID
    user_id_st		broker_oper_id_s;
    uint16		localNumber_n;	    // local order #
    uint16		validity_time_u;    // GTC/ROD/IOC/FOK/FAS/AON
    uint16		order_category_u;   // limitCombo vs interestCombo
    uint8		client_category_u;  // cust/Firm/FARMM/custDisc/brokerDealer
    uint8		is_ours_u;	    // if TRUE, then it is our members, and we know more stuff about it
    cmta_member_st	cmta_member_s;	    // CMTA member ID string
    sub_account_id_st	sub_ac_s;	    // sub account ID string
    ise_user_code_xt	user_code_x;	    // whose order is this (could be another user in the member)
    // couple of flags (only for hit-combos). Store the actual values used in the API
    uint8		step_to_bbo_u,
			step_odd_tick_u;
    uint8		no_of_legs_u;	    // how many legs, 0-4
    combo_leg_xt	leg_x[MAX_COMBO_LEGS];	    // allow for same # as OM message defs
    combo_message_st	combo_message_s;    // large chunk of free text
	int32_t ul_price_low_i; //as of 17.1
	int32_t ul_price_high_i; //as of 17.1
	char ul_price_method_c; //as of 17.1

} combo_entry_xt;

// --------------------------------------------------------------

/* types for storing sorted lists of structures.
    The arrays are sized using mallocs, knowing the number of items, once we have
    downloaded the static data for each country / market.
    We then load the pointers in both structures to point to the actual data records,
    in whatever order they arrived, and then call the stdlib fn "qsort", to reorganise these
    arrays to be in order.
    
    To do this, we must provide comparison routines, one for each array structure.
    What we actually compare is the structures that they point at, but we rearrange
    the elements of these arrays. If, for example, we wish to keep the items sorted
    by N different ways (say by name string, or by commodity code), then we should
    provide N different comparison routines, allowing us to set up N different
    sorted arrays.
    
    Having done this, we can use the standard library fn "bsearch" to find matches
    in the (sorted) arrays. For ISE, with 40,000+ series, this will allow us to
    reference the series info with ~16 comparisons vs an ave of 20,000 (without
    the presorted arrays), or about 1000 times faster.

    NOTE that in the ISE version of the OM system, there are broadcasts which may
    add, change, or delete the known series list on the fly. A change of a series
    may require us to re-sort these structures. An add or delete will require us
    to discard this structure, allocate a new one of the correct size, and sort
    the pointers. As this is a rare event, this is not a significant performance
    issue HOWEVER the data structures must be locked from access during the
    update...
*/

// --------------------------------------------------------------

/* structure for storing array of pointers, sorted
    This is an array of pointers to gen_buf_node_xt,
    which in turn contain pointers to ise_series_info_xt blocks
*/
typedef struct sorted_array_ap * sorted_array_pt;
struct sorted_array_ap {
    // pointer to gen_buf_node. The msg field of each of these points to
    // a ise_series_info_xt.
    gen_buf_node_xt    *orig_data_px [1]; //VARIABLE_LENGTH_ARRAY not n 17.1
};

// ---------------------------------------------------------------------------

/*
    The stuff we store about each Country/Market.
*/
typedef struct {    // country_market_xt
    /* country code. In much of the world, there is only one
	equities exchange for each country, and hence a single code
	allocated to each exchange was 1:1 with country...
	That model fails in the US, so the name of this field is
	somewhat misleading...the field would, perhaps (now) be more
	accurate if it was called exchange, rather than country...
	Some examples include :-
	    10 == AMEX, American Stock Exchange/Nasdaq
	    15 == ASX, Australian Stock Exchange (national)
	    20 == ISE, International Securities Exchange
    */
    uint8	country_u;
    /* market within country. In many places, there is a national equities
	market and a national derivatives market, run by the same
	corporation / exchange.
	Typically this means that they allocate
	market 1 == equities
	market 2 == Derivatives

	ISE appears to be sticking with this common OM convention, and
	allocating market #2 to Derivatives.. although there is
	no technical reason why they didn't start with #1...as these
	numbers are unique within country...
    */
    uint8	market_u;

    // the name of the country / market
    name_st	name_s;

    // current trading state number.. this may be defaulted by the market settings, 
    // or may override that values, and the instrument class value may override this
    // from UQ15 and BI41
    uint16	state_number_u;

    // pointer to the array of (sorted) pointers to the series info, by alphabetic name
    sorted_array_pt  series_sorted_by_name_p;
    // and the same, but this array of pointers is sorted by numeric fields in ise_series_x
    // with UNDERLYING as the first key (all series with same underlying will be together)
    sorted_array_pt  series_sorted_by_ise_series_under_p;
    // and the same, but this array of pointers is sorted by numeric fields in ise_series_x
    // with INSTRUMENT as the first key (all series with same underlying will be together)
    sorted_array_pt  series_sorted_by_ise_series_instr_p;

    // pointer to the array of (sorted) pointers to the underlying info, by alphabetic name
    sorted_array_pt  underlying_sorted_by_name_p;
    // and the same, but this array of pointers is sorted by the commodity code (#)
    sorted_array_pt  underlying_sorted_by_commodity_p;

    // pointer to array of (sorted) pointers to the instrument classes, sorted by the country, market, under, instr group
    sorted_array_pt  instr_class_sorted_p;

    // how many deals we know about
    uint32	todays_deals_u;


    // q of known series - in whatever order CLICK hands 'em over
    node_header_xt	known_series_q;
    // Q of known underlying 
    node_header_xt	known_underlying_q;
    // q of known instruments
    node_header_xt	known_instruments_q;
    // q of known orders
    node_header_xt	known_orders_q;
    // q of known combos
    node_header_xt	known_combos_q;
    // q of known instrument classes
    node_header_xt	known_classes_q;
    // q of known bins
    node_header_xt	known_bins_q;
    // q of known exchanges
    node_header_xt	known_exchanges_q;
    // q of known synthetics
    node_header_xt	known_synthetic_q;
    // q of know trading states
    node_header_xt	known_trading_states_q;

    /*
	info about the range of orderbooks and bins.
	Where the request to do something which requires an orderbook and bin
	as the selector, but the series we wish to operate on is ALL
	i.e. NOT related to one underlying, then we must send of a series
	or requests to the system, looping thru all of the range of
	orderbook/bin combinations that the input selectors allow.

	For example, if we wish to delete all orders (MO104/MO105) in ALL
	underlyings with a country/market, and there are 2 orderbooks,
	then we must send up 2 delete commands, as there is only a orderbook
	selector.

	Another example, is the query on all orders (MQ101 active, MQ102 inactive)
	where the input selectors are both orderbook AND bin. If there were
	10 bins in 2 orderbooks (say 5 in each), then we would have to send
	up 10 queries to get (say) all active orders

	It is not sensible for there to be more orderbooks than bins.. or there
	are empty orderbooks.. so <# orderbooks> <= <# bins>

	It SHOULD be that all underlying in a bin are in the same orderbook.
	This should mean that the # of bin/orderbook combinations is the
	# of bins...although this may NOT be enforced...

	Consider, however, a case of 5 bins, and 2 orderbooks.
	This could be simply set up with bins 1-3 in OB 1, and bins 4-5 in OB2
	OR
	bins 1-2 in OB1, bins 4-5 in OB2, and bin 3 split across both OBs.
	OR
	more of the bins split across the OBs.

	Hence the max value of bin/OB combinations ranges from the # of bins,
	up to the # of bins x # of OBs...
    */
    // the 'max' values for these are set as we rx the records about the underlying
    unsigned		max_bin_u;
    unsigned		max_orderbook_u;

    // default bin - set automatically, if we are signed in as a PMM/CMM
    uint8		default_bin_u;

    /* pointer to first tradable instrument class that we found.
	This isn't really sensible.. as we have different tick-rules
	in different underlying.. but we need it to try to test
	some back-end things with combos. When we were pure fractional
	trading this worked, and while we are pure Nickel/Dime trading
	this works too..
	Actually, all we use this for is to get the number of decimals
	in premiums, which is 4 in all instrument classes, even for
	trading in pennies, nickels, or dimes..
	One should be as good as another */
    instrument_class_pt	traded_instr_class_px;

} country_market_xt;

// ---------------------------------------------------------------------

typedef struct // generic_txn_xt
  {
    // additional fields which could be used for CLICK logging
    uint32	    fn_code;	// copy of the function code - needed for logging
    uint32	    facility_u; // copy of the facility code - "	"   "

    // common part of all generic CLICK transactions
    omni_message    header_x;

    union
     {
	omni_login_message	    login_x;			// }	these are specials
	transaction_type_t	    logout_x;			// }	no trans IDs
	transaction_type_t	    plain_trans_x;		// }	plain jane flavour - just the header

	query_missing_trade_t	    query_missing_trade_x;	// cq1	    todays trades
	query_trade_t		    query_past_trade_x; 	// cq2	    historical trades
	rectify_trade_occ_us_t	    rectify_trade_x;		// cd33	    change clearing detail of whole trade
	rectify_trade_ise_t	    rectify_trade_parts_x;	// cd36	    " of trade in parts

	query_exchange_dq24_t	    query_exchange_x;		// dq24
	query_user_type_info_t	    query_user_type_x;		// dq30
	query_series_t		    query_series_x;		// dq202
	query_instrument_t	    query_instrument_x; 	// dq203
	query_underlying_t	    query_underlying_x; 	// dq204
	query_broker_t		    query_broker_x;		// dq206
	query_market_t		    query_market_x;		// dq207
	query_instrument_group_t    query_instrument_group_x;	// dq208
	query_instrument_class_t    query_instrument_class_x;	// dq210
	query_bin_dq211_t	    query_bin_x;		// dq211
	query_trading_state_dq245_t query_trading_states_x;	// dq245

	query_prices_iq101_t	    query_series_prices_x;	// iq101
	query_opra_prices_iq111_t   query_opra_prices_x;	// iq111
	query_each_opra_prices_iq112_t
				    query_each_opra_prices_x;	// iq112
	query_underlying_prices_iq121_t
				    query_underlying_prices_x;	// iq121
	query_underlying_status_us_t
				    query_underlying_status_x;	// iq132

	order_trans_mo101_t	    order_trans_x;		// mo101
	alter_trans_mo103_t	    alter_trans_x;		// mo103
	delete_trans_mo104_mo105_t  delete_trans_x;		// mo104, mo105
	quote_trans_mo106_t	    quote_trans_x;		// mo106
	block_order_mo107_t	    block_trans_x;		// mo107
	crossing_order_mo108_t  facilitation_trans_x;	// mo108
	special_order_response_mo109_t
				    block_facil_response_x;	// mo109
	cab_order_mo110_t	    cab_trans_x;		// mo110
	pmm_release_locked_order_mo111_t
				    release_locked_x;		// mo111
	pmm_trade_report_trans_mo112_t
				    trade_report_x;		// mo112, PMM only
	rotation_request_mo113_mq114_t
				    rotation_x; 		// mo113 and mq114, PMM only
	combo_order_trans_mo131_t   combo_trans_x;		// mo131, 16.5+
	alter_combo_trans_mo133_t   alter_combo_trans_x;	// mo133, 16.5+
	execute_combo_trans_mo136_t hit_combo_trans_x;		// mo136, 16.5+

	application_status_t	    application_status_x;	// ui1
	set_mm_parameters_ui101_t   set_mm_parameters_x;	// ui101, MM only
	set_pmm_parameters_ui102_t  set_pmm_parameters_x;	// ui102, PMM only
	set_mm_exposure_limit_ui103_t
				    set_mm_exposure_limit_x;	// ui103, CMM/PMM, 16.5-1+
	query_mm_parameters_uq101_t query_mm_parameters_x;	// uq101, MM only
	query_pmm_parameters_uq102_t
				    query_pmm_parameters_x;	// uq102, PMM only
	query_mm_exposure_limit_uq103_t
				    query_mm_exposure_limit_x;	// uq103, CMM/PMM, 16.5-1+

	query_order_mq101_t	    query_order_x;		// mq101 - changed field order in 16.5
	query_inactive_mq102_t	    query_inactive_x;		// mq102
	query_quote_mq104_t	    query_quote_x;		// mq104, MM and PMM only
	query_ob_mq103_t	    query_ob_x; 		// mq103, PMM only
	query_locked_order_mq106_t  query_locked_x;		// mq106, PMM only
	query_combo_order_mq131_t   query_combo_x;		// mq131, 16.5+ active combos, my member
	query_inactive_combo_mq132_t query_inactive_combo_x;	// mq132, 16.5+ inactive combos, my member
	query_combo_ob_mq133_t	    query_combo_public_x;	// mq133, 16.5+ all combos, all members

	query_bi8_broadcasts_sent_t
				    query_past_general_text_x;	// uq11
	query_business_date_t	    query_business_date_x;	// uq12
	query_instrument_status_t   query_instr_status_x;	// uq15
     } tx_x;
} generic_txn_xt;

// ---------------------------------------------------------------------

// how many underlying we allow in one specified list.. as a bin is 60, this number is generous
// note if we are doing a bcast subscription for 'all', then we just have one entry, with a 0 as the commodity code
#define MAX_UNDERLYING_PER_LIST	    300

// ---------------------------------------------------------------------------

/* a type for what commodities a user / script is specifying in a list.
    NB the original form is an argc/argv[] setup of underlying names, with some
    specials like %bin.
    We convert that into a count and array of commodity codes.
*/
typedef struct {
    // count of underlying we are adding to our subcription list for the particular broadcast type
    uint16		underlying_count_u;
    // and the entries. If an entry with a commodity code of 0 is present, then we are subscribed to all
    uint16		commodity_code_u [MAX_UNDERLYING_PER_LIST];
} ise_underlying_list_xt;

// ---------------------------------------------------------------------------

/*
a type for storing info about a synthetic.
It will contain things like its own commodity code, a cash-offset
and a list of pointers to the component underlyings.

Also, a guess at a reasonable # of underlying items for one synthetic.
We are waiting for more info from OM about whether we can build a fixed # like this.
It is not really optimal, but then how many synthetic is ISE going to be trading
options on ?
Presumably, it will only be for post-merger synthetics, for a limited time in each.
*/
#define MAX_UNDERLYING_PER_SYNTHETIC 10
typedef struct {
    // the commodity code of the synthetic
    uint16		commodity_code_u;
    // and a pointer to the 'underlying' that corresponds to
    ise_underlying_xt	*synthetic_under_data_px;
    // the cash offset for the synthetic
    uint32		cash_amount_u;
    // how many underyling items there are for this synthetic.. each has a pointer, and a # of shares
    uint8		underlying_count_u;
    // and the underlying entries
    ise_underlying_xt	*dependent_under_data_px [MAX_UNDERLYING_PER_SYNTHETIC];
    // the number of shares
    uint32		number_of_shares_u [MAX_UNDERLYING_PER_SYNTHETIC];
} ise_synthetic_xt;


// info we know about a trading state
typedef struct {
   trade_state_id_st    state_id_s;			// string name of state
   uint16		state_number_u;			// key between static defnm query, bcasts
   uint16		fast_market_level_u;		//
   uint8		type_of_matching_u;		//
   uint8		trading_end_u;			//
   uint8		price_quotation_required_u;	//
   uint8		market_orders_allowed_u;	//
   uint8		fill_kill_allowed_u;		//
   uint8		edited_ob_changes_avail_u;	//
   uint8		ob_changes_avail_u;		//
} ise_trading_state_xt;

// ---------------------------------------------------------------------------
/*
info we know about one transaction/query/broadcast.
Comes from DQ30. The numeric field for the transaction type
is in local-endian order
*/
typedef struct {
    // transaction like "M" "O" 101, or query like "D" "Q" 202 or bcast like "B" "D" 5
    transaction_type_t transaction_type;
    // flag if a transaction/query, vs a broadcast.
    // TRUE means bcast, FALSE means transaction/query
    uint8 is_bcast_u;
} ise_user_type_t;

/* how many transactions / queries / broadcasts we are authorised for
    As the (rough count) number across all user types as at 2001/07/06 is
    21 transactions
    34 queries
    30 broadcasts
    then a value of 200 should be plenty
*/
#define MAX_TRANS_QUERY_BCAST 200


// ---------------------------------------------------------------------------

// the fundamental global data we keep about a CLICK channel
typedef struct click_details_t {    // click_details_xt.
    /* the lock we keep on the whole global, for interthread updates.
	The command line thread only needs to dive into the global when there is
	a command to process..BUT when it wants to access it, it assumes the global is
	stable and consistent.

	The broadcast thread mostly finds no broadcasts.
	When it finds one though, it generally accesses the global.
	Also, there are some maintenance broadcasts (such as BI202, 204, 210)
	which alter the global....

	To this end, we have a lock, which anyone accessing the global MUST have
    */
    ise_mutex_t global_lock_mutex;


    // transaction vector, used in calls to omniapi_tx. We can only have one call outstanding
    // at a time, so only need one
    omni_message    *OM_tx_vector_px[2];

    int32	    logged_on_i;		    // boolean for logged-in
    // NB the constants in this next bunch are so cast in concrete (OM systems for years)
    // that no really worth #defining them and referencing. NB one extra space for trailing NUL
    char	    user_name_s[33];		    // username - changed size in 16.3
    char	    password_s[33];		    // user password
    char	    gateway_name_s[129];	    // gateway node name
    char	    gw_socket_name_s[21];	    // gateway Socket name
    user_type_id_st user_type_name_s;		    // name of user type, from dq30
    // flag is we are internal or external user. Internal users are ISE control tools, in-house.
    unsigned	    internal_user_u;		    // default 0 == false.. most API users are external
    // are we considered by system to be trader (more important than this is are we allowed trading transactions)
    unsigned	    non_trader_u;		    // default 0 == false, i.e. we are a trader
    /* whether we are in auto-logging mode
	If Auto-logging is on, then we turn logging ON after we log on,
	and turn logging off when we log off
    */
    int32	    auto_logging_b;
    // the current business date - returned by UQ12 once logged on
    date_st	    current_business_date_s;
    // the omex version - returned by UQ12
    char	    omex_version_s[17];
    // the flag of our default user/member choice. Default memory value 0 says member.
    unsigned	    user_member_default_u;

    // header for dumping msgs being sent out to CLICK to logger. All sync calls, so only need one
    // header for dumping msgs rxed from CLICK to logger. All sync calls, so only need one

    // header for dumping msgs being sent out to CLICK to logger.
    // All sync calls, so only need one. NB this contains the status of the last op
    log_head_xt     log_out_head_x;
    // header for dumping msgs rxed from CLICK to logger, as per log_out_head above
    log_head_xt     log_in_head_x;

    // size of what we just received
    uint32 	    rx_length_i;

    /*
	synchronisation object, for handshake between bcast thread and main thread.
	This object should only allow ONE thread to own it.
	When a thread is about to do an OMNIAPI IO call,
	it grabs the lock, does the IO, and release it.
    */
    ise_mutex_t omni_io_active_mutex;

    // how many transactions / queries / broadcasts we are authorised for,
    // and the list of the actual transactions.
    unsigned	    trans_query_bcast_u;
    ise_user_type_t ise_trans_query_bcast_x[MAX_TRANS_QUERY_BCAST];

    /*
	CLICK-format response msg to accept response data, wrapped in a loggable struct
	standard, except for the final (struct) field, which must be customised to the query
	Can only have one query at a time, so only need one. This also means
	that we can use a standardised routine to check the return statii, report
	errors etc

	Note however, that as the thread handles broadcasts runs in the
	background, while the main (command line and script file) thread
	whizzes around playing with the stuff in here, we must use a
	separate buffer for broadcasts...

	For example, the main thread may wish to do a particular query.
	It sets up the request, and queues it to the IO thread, which is normally
	whizzing around checking for broadcasts. It then signals the broadcast
	thread, and wait on another event.

	The broadcast thread is, meanwhile, looping around looking for things
	to do. In each loop, it
	- does a check on the signal from the other thread
	- tries to get a block of broadcasts from the main system (read gateway)
	If there is something from the other thread (infrequent), it does it,
	and then signals the other thread back. It then continues around the loop.

	Note then, that the broadcast thread may then get another block of
	broadcast data from the gateway, while the other thread is still
	processing (or has not yet woken up to start processing) the last
	response from a normal omniapi_tx or omniapi_query.

	Note also, that any NT multithread synchronisation, such as waiting
	on an event, unless the object is immediately available,
	inherently costs ONE clock tick i.e. 10 milliseconds.

	This restrict the (doubly-synchronised) throughput between our
	two cooperating threads, to a maximum of 50 operations a second.

	As the IO time on a typical request with the main system, via the gateway,
	is typically about 10 times slower than this, this should NOT incur
	a signicant penalty.

	The response is put into this standard log_reply_x, and a 
    */
    struct {
      // standard header fields, ready to be logged
      // copy of the function code, values as per global.h
      uint32	    fn_code;
      // secondary status from the omniapi_tx or omniapi_query call
      int32	    TxnStatus_i;
      // transaction ID
      uint64_t	    TxnIdent_u;
      // Order ID - not set unless the operation is actually Order related
      quad_word	    OrderIdent_u;

  
      // where omniapi actually puts the output results
      union
      {
	// the static market data
	answer_series_da202_t		    answer_series_x;		    // ISE flavour, dq202
	answer_instrument_da203_t	    answer_instrument_x;	    // ISE, dq203
	answer_underlying_da204_t	    answer_underlying_x;	    // ISE, dq204
	answer_broker_da206_t		    answer_broker_x;		    // ISE, dq206
	answer_market_da207_t		    answer_market_x;		    // ISE, dq207
	answer_instrument_group_da208_t     answer_instrument_group_x;	    // ISE, dq208
	answer_instrument_class_da210_t     answer_instrument_class_x;	    // ISE, dq210
	answer_bin_da211_t		    answer_bin_x;		    // ISE, dq211
	answer_dependencies_da212_t	    answer_synthetic_x;		    // ISE, dq212
	answer_exchange_da24_t		    answer_exchange_x;		    // ISE, dq24
	answer_trading_state_da245_t	    answer_trading_states_x;	    // 16.3+, dq245
	answer_user_type_info_t		    answer_user_type_x;		    // 16.5-1+, dq30

	// the market snapshots / queries
	answer_missing_trade_t		    answer_missing_trade_x;	    // cq1/ca1
	answer_trade_t			    answer_past_trade_x;	    // cq2/ca2
	answer_order_ma101_t		    answer_order_x;		    // ISE, mq101
	answer_inactive_ma102_t 	    answer_inactive_x;		    // ISE, mq102
	answer_ob_ma103_t		    answer_ob_x;		    // ISE, mq103, PMM only
	answer_quote_ma104_t		    answer_quote_x;		    // ISE, mq104
	answer_locked_order_ma106_t	    answer_locked_order_x;	    // ISE, mq106, PMM only
	answer_rotation_ma114_t 	    answer_rotation_x;		    // ISE, mq114, PMM only
	answer_combo_order_ma131_t	    answer_combo_active_x;	    // 16.5+, mq131, member's active combos
	answer_inactive_combo_ma132_t	    answer_combo_inactive_x;	    // 16.5+, mq132, member's inactive combos
	answer_combo_ob_ma133_t		    answer_combobook_x;		    // 16.5+, mq133, public combobook
	answer_prices_ia101_t		    answer_prices_x;		    // ISE, iq101
	answer_opra_prices_ia111_t	    answer_opra_prices_x;	    // ISE, iq111
	answer_each_opra_prices_ia112_t     answer_each_opra_prices_x;	    // ISE, iq112
	answer_underlying_prices_ia121_t    answer_underlying_prices_x;     // ISE, iq121
	answer_underlying_status_us_t	    answer_underlying_status_x;     // ISE, iq132
	answer_bi8_broadcasts_sent_t	    answer_past_general_text_x;	    // uq11
	answer_business_date_t		    answer_business_date_x;	    // uq12
	answer_instrument_status_t	    answer_instrument_status_x;	    // ISE, uq15
	answer_mm_parameters_ua101_t	    answer_mm_parameters_x;	    // ISE, uq101
	answer_pmm_parameters_ua102_t	    answer_pmm_parameters_x;	    // ISE, uq102, PMM only
	answer_mm_exposure_limit_ua103_t    answer_mm_exposure_limit_x;	    // ISE, uq103, CMM/PMM, 16.5-1+

	
	// directives / commands / statii
	transaction_type_t		    transaction_type_x;
	transaction_type_t		    plain_trans_x;		    // plainjane flavour - useful for seeing hdr only
      } trans_reply_x;
    } log_reply_x;



    struct {
      // standard header fields, ready to be logged
      // copy of the function code, values as per global.h
      uint32	    fn_code;
      // secondary status from the omniapi_tx or omniapi_query call
      int32	    TxnStatus_i;
      // transaction ID
      uint64	    TxnIdent_u;
      // Order ID - not set unless the operation is actually Order related
      uint64	    OrderIdent_u;

  
      // where omniapi actually puts the output results
      union
      {
	// broadcast stuff
	// as a raw buffer
	char				  raw_buffer [/*MAX_RESPONSE_SIZE 64000*/ 62000];
	// or just as formatted bcast header - so we can examine it, say in debug
	broadcast_type_t		  broadcast_type_x;
      } trans_reply_x;
    } bcast_reply_x;


    // our user code - returned by omniapi_get_info, in utility.c, at login time
    ise_user_code_xt	    user_code_x;
    uint16		    maxLocalOrderNumber_u;  // number of the most recent local order
    uint16		    maxLocalComboNumber_u;  // number of the hi'est known combo


    /* what is the facility # for the E0 facility, in this CLICK system.
    This could be ET0 or EP0. We just enforce that it must be ONE of the
    defined ISE systems. We are NOT programmed for any of the other OM/CLICK variations.
    */
    uint32	    facility_no_u;

    /* our country and market.
    At present, the ISE is a 'unique country', with each of the other
    N American exchanges (e.g. CBO) allocated a different country # (ref dq24),
    and ISE will run exactly ONE market i.e. derivatives.
    If ISE is to run multiple markets, or ISE is to provide gateway services
    to the other exchanges, then a chain of known country / markets should
    be used, with a default pointer in here to our primary country / market.
    */
    country_market_xt	    our_market_x;

    // a dummy orderID, all 0's, which can be PUTQUAD into msgs as necessary
    quad_word		    null_orderID_u;

    /* we user can set a default underlying and series, which then allows
	for the use of the substitation symbols %U and %s in the commands.

	To allow scripts to specify series and underlying outside of OM
	sessions (i.e. while we do not actually know the available underlying
	and series choices), we store the string names in the commands,
	and then look up the underlying and series at actual use.

	This also allows for the series/underlying to remain the same
	across multiple OM sessions (in which the underlying/series data
	may actually change).
    */
    under_id_st 	    default_under_name_s;
    series_st		    default_series_name_s;

    /*	Copy of the last order, as generated by any of the
	plain, block, facilitation, block/facil response, and
	cab orders...

	This local copy is updated on every call to any of these
	routines.

	If the user wishes to be able to use the data in a particular
	order, then they should use the
	??? command
	which will snapshot from this local copy into an identical
	structure (saved_order_x) in the click_details_xt.

	The fields in that snapshot-save can then be referred by
	specifying the %O (saved order) string. For example,
	in Amend, if %O is put in the <seriesname> field, then
	the seriesname from the last snapshot-saved order is used
	in the amend.

	Note that we save this on EVERY order command, regardless
	of the status that we get back on the OMNIAPI call
    */
    order_entry_xt	    last_order_x;
    // similar deal with last combo
    combo_entry_xt	    last_combo_x;

    // the last order that was saved with a snapshot-save command
    order_entry_xt	    saved_order_x;
    // the last combo that was saved with a snapshot-save command
    combo_entry_xt	    saved_combo_x;
    /* must save the series name at the time of the save,
	and check this at the time of reference to series,
	ensure that we have the same series name..
	if we log out and in, the series_px pointer in the
	saved_order_x will be invalid
    */
    series_st		    saved_order_series_name_s;
    // for the same reasoning, we must save the underlying name
    // and series names for the saved combo...
    name_st		    saved_combo_underlying_name_s;
    series_st		    saved_combo_series_name_s[MAX_COMBO_LEGS];

    // q of underlying price change records - where the broadcast thread hands 'em over
    node_header_xt	authorised_autoquote_q;

    // the next fields may be overridden by values in the underlying
    // the default quantity for a quote
    uint32 autoquote_quantity_u;
    // the default open quantity for a quote (only used in a rotation)
    uint32 autoquote_open_quantity_u;
    // default step-up buffer to set on a new quote
    uint16 autoquote_step_up_buffer_n;

}   click_details_xt;


/* structure to hand a price change record to the autoquote thread
    Note, these records are only generatede for underlyings with autoquote ON
    AND where one of bid and ask is different from last time
*/
typedef struct {
    ise_underlying_xt	*ise_underlying_px;
    // if either of these is 0, then that particular price did NOT change since last time
    int32		new_under_bid_premium_i;
    int32		new_under_ask_premium_i;
}   autoquote_price_change_xt;


#endif


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


Title : trades.c
Author: Neil Cossor
Function :

    This file provides basic deal/trade handling to the ISESim.exe program.
    This includes querying about deals that have occurred on past days
    
Revision history :-

yymmdd	vers'n	author	    notes
------	------	------	    -----
9907xx	v0.00	Neil Cossor initial version
990928	v0.01		    added some doc notes.
			    Apply additional programming info supplied by OM about the cq2 query.
			    more detail reported by got_a_cl_trade_api
991006	v0.02		    fix bug in got_a_cl_trade_api.. I had used OM string directly, without extraction,
			        which meant strings were not NUL terminated and overran in the printf
			    enhance got_a_cl_trade_api to include trade type and state
000403	v0.03		    mod query_todays_deals to use business date retrived in uq12 (misc.c)
000405			    mod query_historical_deals - report (and count) non-matching trades
000422	v1.00		    update to match OM release 15.7-3 (move of some fields from da203 to da210).
			    got_a_cl_trade_api
010313	v1.01		    update for 16.3, added trade_rectify_parts
010614	v1.02		    mod go_a_cl_trade_api to report "amend"
010712			    fix couple of bugs in trade_rectify_parts (logged wrong transaction, size to wrong)

Required work :-

General Notes about Trades and Deals :-

    Deal sequencing is within instrument type, across all Members.
    The ISE version of the OM system does not support queries relating
    to deal numbers. The deal number (for a particular trade) is, however,
    contained in the trade data returned in the BD5 broadcast,
    and in the CA1 and CA2 responses to the CQ1 and CQ2 queries.

    Trade sequencing is within instrument type, for each Member.
    A member does not see the trade information relating to other
    Members.

    If a Member want to know about all (of their own) trades, they must collect
    the trade sequences for all (tradable instrument groups). As ISE does not trade in the Spot
    instrument group, that means there are only 2 deal sequences (American Call and
    American Put).. at least at this time.

    Note (as OM docs indicate) that a deal may consist of two or more
    trades.

    For example :-

    If customer A places a sell order for 5 options for $10
    when we already had a market with
	customer B had a buy order for 5 options for $10
	and customer C had a buy order for 5 options for $10
    then we would have one new deal in this instrument type,
    and there would be three new trades.
*/
#include "stdincl.h"

#include "error.h"
#include "instrument.h"
#include "series.h"
#include "underlying.h"
#include "utility.h"

#include "trades.h"

// -------------------- local consts and typedefs ----------------------------

void got_a_cl_trade_api ( click_details_xt * click_px,
			    cl_trade_api_t *trade_px )
/*
Function:   got_a_cl_trade_api
Author:     Neil Cossor
Modified:   20010614
Description:

    We got info about a trade (from CQ1, CQ2, BD5), in particular
    an OM structure cl_trade_api_t.

    Grab the info about the trade.

    (optionally) debug dump the info out onto stdout

Input Params:

    Standard pointer to all things we know about the central system / market
    Pointer to the individual trade record

Output Params:

    on screen (stdout, actually)

Note:

    As well as new trades coming from BD5, we may also receive trade reversal,
    and overtaking trades, in the case where a trade has been altered by DCPC
    by exchange staff.
*/
{
    // the string form of a premium, with appropriate # of decimal places
    series_premium_st	    premium_s;
    ise_series_xt	    ise_series_x;	// temp, the series in ISE form
    ise_series_info_pt	    found_series_px;	// pointer to info about a series
    // (correct endian) fields from the trade
    uint32		    quantity_u,
			    premium_i;
    // date, in string form, NUL terminated
    date_st		    date_s;
    // time, in string form, NUL terminated
    time_st		    time_s;
    // ISE version of a user_code | countersign_code
    ise_user_code_xt	    countersign_user_x;
    char		    attention_s[60];
    
    // get the series, in "normal" form
    get_click_series ( &ise_series_x,
			&trade_px->series );
    // find the record with our data about the series
    found_series_px = find_series_by_ise_series ( click_px,
						    &ise_series_x );
    // format the premium appropriately
    PUTLONG ( premium_i, trade_px->deal_price_i );
    format_whole_decimal_s ( (char *)&premium_s,
				premium_i,
				found_series_px->instrument_class_px->dec_in_premium_u,
				MAX_SERIES_PREMIUM );	// allowed width
    // grab the quantity, correct endian
    PUTLONG ( quantity_u, trade_px->trade_quantity_i );
    // extract creation date and time
    rx_click_string ( &date_s,
			trade_px->created_date_s,
			sizeof ( trade_px->created_date_s ));
    rx_click_string ( &time_s,
			trade_px->created_time_s,
			sizeof ( trade_px->created_time_s ));
    // grab the countersign info, into usable form
    get_ise_user_code_xt ( &countersign_user_x,
			    (struct user_code *)&trade_px->countersign_code );
    // turn the attention code into a string.. since 16.5 we can tell it this is amend (overtaking to follow)
    switch ( trade_px->attention_c )
    {
    case 2: 
	    sprintf (attention_s, "trade error in backed log");
	    break;
    case 4:
	    sprintf (attention_s, "trade date no business date");
	    break;
	case 8:
		sprintf (attention_s, "last trade for order; remaining order balance, if any, is cancelled.");
    case 64:
	    sprintf (attention_s, "amend, overtake to follow");
	    break;
    default:
	    sprintf (attention_s, "unknown attn %u", trade_px->attention_c );
    }	// switch on attention flag

    printf("%-16s%9u%10s %3s %8ld %-5s %-5s (type %s, state %s) %6ld \n    %8s:%6s %5d %9u %8d, with %s,%s [%s]\n",
	    &found_series_px->ise_name_s,
	    quantity_u,
	    &premium_s,
	    bid_ask_to_string ( trade_px->bought_or_sold_c ),
	    trade_px->trade_number_i,
	    "",//&trade_px->trading_code.ex_customer_s,
	    "",//&trade_px->trading_code.user_id_s,
	    trade_type_to_string (trade_px->trade_type_c),
	    trade_state_to_string (trade_px->trade_state_c),
	    trade_px->sequence_number_i,
	    &date_s,
	    &time_s,
	    trade_px->ext_seq_nbr_i,
	    trade_px->global_deal_no_u,
	    trade_px->deal_number_i,
	    &countersign_user_x.customer_id_s,
	    &countersign_user_x.user_id_s,
	    &attention_s
	    );
}   // got_a_cl_trade_api

// ---------------------- external functions ---------------------------------

int32 query_todays_deals (click_details_xt * click_px,
			    int argc,
			    char *argv[ ] )
/*
Function : query_todays_deals
Author	 : Neil Cossor
Modified : 20000403
Description : 

    We wish to know something about todays deals.

    Deal sequencing is within instrument group, so if you want to know
    about all deals, you must make a number of queries, at least one per instrument
    group.

    Also, the answer is NOT segmented.. instead, if the max # of deals that can
    fit in a response are supplied, then (at least) one more query should be
    done, using a start sequence # of 1 higher than the last deal that was
    in this response

Input Params:

    argv[0]	TRADES
    argv[1]	TODAY (if not TODAY, then we will have called query_historical_deals
    <optional>
    argv[2]	<instrument group | *>	default to * - get all instrument groups
    argv[3]	<start sequence #>  default 0, only allowed if an instrument group is selected
    argv[4]	<end sequence #>    default 0,	"		"	 "		"	 "

Output Params:
*/
{
// how many trades in a CA1 message
#define MAX_TRADES_IN_CA1 200

    // pointer to one trade, in a block response
    cl_trade_api_t	    *missing_trade_px;
    int32		    status_i = SUCCESSFUL;
    // what instrument group the user selected - 0 means all
    uint8		    instr_group_u;
    // pointer to static data about instrument groups
    instrument_xt	    *instr_group_px;
    // pointer to (Q'ed) block, containing pointer to an instrument group
    gen_buf_node_xt	    *instr_node_px;
    uint16		    item_index_u,   // local loop var
			    item_limit_u;   // limit on loop - may be endian-swapped from response
    // how many trades satisfied with this query
    uint32		    trades_found_u = 0;


    // the message we will query with
    generic_txn_xt	    OMmsg_x;
    // size of our actual CLICK query/command
    uint32		    tx_length_u = sizeof ( OMmsg_x.tx_x.query_missing_trade_x );
    // start and end sequence #s supplied by user.. default to 0
    uint32		    start_seq_u,
			    end_seq_u;
    // temp, used in endian conversion of data from response
    uint32		    temp_32_u;


    
    
    if (!click_px->logged_on_i)
    {
	prompt_logged_on ("Trade,Today query ");
	return NOT_LOGGED_ON;
    }

    /* enuff arguments - actually a bit pointless, given we have figured
	out the TRADES, TODAY part, as the rest may be defaulted
    */
    if ( argc < 2 )
    {
	printf ( "Error\n Trades,Today[,<instr group>[,<start seq #>[,<end seq #>]]]\n");
	return SUCCESSFUL;
    }


    // figure out what optional parameters were supplied - start with defaults for everything
    start_seq_u = 0;
    end_seq_u = 0;
    instr_group_u = 0;

    if ( argc >= 3 )
    {
	// caller specified an instrument group - can now accept start and end seq #s
	instr_group_u = read_unsigned_limited ( argv[2], UNDEFINED_UINT8 );

	// how about start seq #
	if ( argc >= 4 )
	{
	    // user supplied a start sequence #
	    start_seq_u = read_unsigned_limited ( argv[3], UNDEFINED_UINT32 );

	    // how about an end sequence #
	    if ( argc >= 5 )
	    {
		// user supplied an end sequence #
		end_seq_u = read_unsigned_limited ( argv [ 4], UNDEFINED_UINT32 );
	    }
	}   // test if start seq # supplied
    }
    else
    {
	// user did NOT select a instrument group - we must loop thru all
	instr_node_px = (gen_buf_node_xt *)click_px->our_market_x.known_instruments_q.chain.next;
	instr_group_px = (instrument_xt *)instr_node_px->msg_p;
    }	// test if instrument group # supplied



    // zot the whole query message - this includes the series
    memset( &OMmsg_x.tx_x.query_missing_trade_x, 0, tx_length_u );

    // set the message header
    pack_OM_header ( &OMmsg_x, tx_length_u, 'C', 'Q', 1 );

    OMmsg_x.tx_x.query_missing_trade_x.series.country_c = click_px->our_market_x.country_u;
    OMmsg_x.tx_x.query_missing_trade_x.series.market_c = click_px->our_market_x.market_u;
    // sequence #s - endian
    PUTLONG ( OMmsg_x.tx_x.query_missing_trade_x.sequence_first_i,
		start_seq_u );
    PUTLONG ( OMmsg_x.tx_x.query_missing_trade_x.sequence_last_i,
		end_seq_u );
  
    // use current business date retrieved in UQ12, at login
    tx_click_string ( &OMmsg_x.tx_x.query_missing_trade_x.date_s,
			&click_px->current_business_date_s,
			sizeof ( OMmsg_x.tx_x.query_missing_trade_x.date_s));

    // save the facility code and fn code for loggin
    OMmsg_x.fn_code = LOG_K_Q_MISSING_TRADES;
    OMmsg_x.facility_u = click_px->facility_no_u + 3;	// EP3/ET3

    printf ("Sending Missing Trades Query\n");

    if ( instr_group_u != 0 )
    {
	// user selected a particular instrument group - use that
	OMmsg_x.tx_x.query_missing_trade_x.series.instrument_group_c = instr_group_u;
    }
    else
    {
	// we are doing all instruments, and instr_group_px pts to first known instrument
	instr_node_px = (gen_buf_node_xt *)click_px->our_market_x.known_instruments_q.chain.next;
	instr_group_px = (instrument_xt *)instr_node_px->msg_p;
	OMmsg_x.tx_x.query_missing_trade_x.series.instrument_group_c =
		instr_group_px->instrument_group_u;
    }

    do
    {
	status_i = do_click_query_rx ( click_px,
					&OMmsg_x,
					tx_length_u,
					LOG_K_R_MISSING_TRADES);


	if ( status_i < SUCCESSFUL )
	{
	    // do nothing, error already reported by do_click_query
	    return status_i;
	}

	// how many items in the msg - endian
	PUTSHORT ( item_limit_u, click_px->log_reply_x.trans_reply_x.answer_missing_trade_x.items_n );

	// now look at the actual trade data
	for ( item_index_u = 0; item_index_u < item_limit_u; item_index_u++ )
	{
	    // point to info about one trade
	    missing_trade_px = &click_px->log_reply_x.trans_reply_x.answer_missing_trade_x.item[item_index_u].cl_trade_api;

	    // and do things with that info

	    // do we need to spit out a header ?
	    if ( trades_found_u == 0)
	    {
		printf("Series           Quantity   Premium B/S  Trade # Membr User  Sequ. yyyymmdd:hhmmss X.Seq\n" );
	    }

	    // report the trade
	    got_a_cl_trade_api ( click_px, missing_trade_px );

	    // note one more trades we found in this query
	    trades_found_u++;
	}   // for loop, processing all the components in one reply

	// could there be more trades ?
	if ( item_limit_u == MAX_TRADES_IN_CA1 )
	{
	    // yes, could be more - just copy the next seq # from response to query
	    PUTLONG ( temp_32_u,
			missing_trade_px->sequence_number_i );
	    temp_32_u++;
	    PUTLONG ( OMmsg_x.tx_x.query_missing_trade_x.sequence_first_i,
			temp_32_u );
	}
	else
	{
	    // done with this instrument group - no more trades

	    // are we doing all instrument groups ?
	    if ( instr_group_u == 0 )
	    {
		// we are supposed to be looping thru all instrument groups - try for the next
		instr_node_px = (gen_buf_node_xt *)instr_node_px->chain.next;
		if ( instr_node_px )
		{
		    instr_group_px = (instrument_xt *)instr_node_px->msg_p;
		    OMmsg_x.tx_x.query_missing_trade_x.series.instrument_group_c =
			    instr_group_px->instrument_group_u;
		    OMmsg_x.tx_x.query_missing_trade_x.sequence_first_i = 0;
		    OMmsg_x.tx_x.query_missing_trade_x.sequence_last_i = 0;
		}
		else
		{
		    break;
		}
	    }	// test if user selected all instrument groups
	    else
	    {
		// we are only collecting for one instr group, bail out of loop
		break;
	    }
	}   // test if we are finished with this trades
    }
    while (TRUE);

    printf ( "Found a total of %u trades matching the query parameters\n",
		    trades_found_u );

    return status_i;
}   // query_todays_deals

// ---------------------------------------------------------------------------

int32 query_historical_deals (click_details_xt * click_px,
			    int argc,
			    char *argv[ ] )
/*
Function : query_historical_deals
Author	 : Neil Cossor
Modified : 20000405
Description : 

    We wish to know something about a past days deals. Uses cq2.

    This query uses an odd combination of both segmented AND partitioned,
    although the partition is not in the usual sense (of which MP server).

Input Params:

    argv[0]	TRADES
    argv[1]	HISTORICAL
    argv[2]	<start date - yyyymmdd>
    argv[3]	<end date - yyyymmdd>
    <optional>
    argv[4]	<start time> - hhmmss, default 000000
    argv[5]	<end time> - hhmmss, default 235959 ????
    argv[6]	<instrument group | *>	default to * - get all instrument groups

Output Params:
*/
{
// how many trades in a CA2 message
#define MAX_TRADES_IN_CA2 200

    // pointer to one trade, in a block response
    cl_trade_api_t	    *missing_trade_px;
    int32		    status_i = SUCCESSFUL;
    // what instrument group the user selected - 0 means all, which is default
    uint8		    instr_group_u = 0;
    // pointer to static data about instrument groups
    instrument_xt	    *instr_group_px;
    // pointer to (Q'ed) block, containing pointer to an instrument group
    gen_buf_node_xt	    *instr_node_px;
    uint16		    item_index_u,   // local loop var
			    item_limit_u;   // limit on loop - may be endian-swapped from response
    // how many trades satisfied with this query
    uint32		    trades_found_u = 0;
    // and ones which didn't match our request
    uint32		    trades_others_u = 0;
    // the segment # we are up to..., with an initial value
    uint16		    segment_u = 0;


    // the message we will query with
    generic_txn_xt	    OMmsg_x;
    // size of our actual CLICK query/command
    uint32		    tx_length_u = sizeof ( OMmsg_x.tx_x.query_past_trade_x );


    
    
    if (!click_px->logged_on_i)
    {
	prompt_logged_on ("Trade,Past query");
	return NOT_LOGGED_ON;
    }

    // enuff arguments	?
    if ( argc < 4 )
    {
	printf ( "Error\n Trades,Past,<start yyyymmdd>,<end yyyymmdd>[,<start hhmmss>[,<end hhmmss>[,<instr group>]]]\n");
	return SUCCESSFUL;
    }

    // zot the whole query message - this includes the series
    memset( &OMmsg_x.tx_x.query_past_trade_x, 0, tx_length_u );

    // put in the start and end dates - mandatory fields
    tx_click_string ( &OMmsg_x.tx_x.query_past_trade_x.from_date_s,
			argv[2],
			sizeof (OMmsg_x.tx_x.query_past_trade_x.from_date_s));
    tx_click_string ( &OMmsg_x.tx_x.query_past_trade_x.to_date_s,
			argv[3],
			sizeof (OMmsg_x.tx_x.query_past_trade_x.to_date_s));

    // a nominated start time ?
    if (( argc > 4 ) && ( argv[4] != NULL ))
    {
	// have a start time param
	tx_click_string ( &OMmsg_x.tx_x.query_past_trade_x.from_time_s,
			    argv[4],
			    sizeof (OMmsg_x.tx_x.query_past_trade_x.from_time_s));
    }
    else
	// default to starting at midnight
	tx_click_string ( &OMmsg_x.tx_x.query_past_trade_x.from_time_s,
			    "000000",
			    sizeof (OMmsg_x.tx_x.query_past_trade_x.from_time_s));

    // a nominated end time ?
    if (( argc > 5 ) && ( argv[5] != NULL ))
    {
	// have an end time param
	tx_click_string ( &OMmsg_x.tx_x.query_past_trade_x.to_time_s,
			    argv[5],
			    sizeof (OMmsg_x.tx_x.query_past_trade_x.to_time_s));
    }
    else
	// default to ending at last moment of the day
	tx_click_string ( &OMmsg_x.tx_x.query_past_trade_x.to_time_s,
			    "235959",
			    sizeof (OMmsg_x.tx_x.query_past_trade_x.to_time_s));

    // series is specified, to get the query thru the routing
    // but the search series qualitied what data we want back.. it is a filter

    // does the user want a specific instrument type, or all
    if ( argc > 6 )
    {
	// caller specified an instrument group
	instr_group_u = read_unsigned_limited ( argv[6], UNDEFINED_UINT8 );
	// check if we know about such an instrument group
	if ( find_instr_group ( click_px, instr_group_u ) == NULL )
	{
	    printf ("Cannot query trades with that instrument type(%u), unknown\n",
			instr_group_u );
	    return INVALID_ARGUMENTS;
	}   // test for valid instr group # supplied

	// specify the instrument group.. we should specify the specific value in series
	// and put a non-zero value in the search_series, to show we are NOT wildcarding
	OMmsg_x.tx_x.query_past_trade_x.search_series.instrument_group_c = instr_group_u;
	OMmsg_x.tx_x.query_past_trade_x.series.instrument_group_c = instr_group_u;
    }
    else
    {
	// user did NOT select a instrument group.

	// we are doing all instruments, and instr_group_px pts to first known instrument
	// we need one valid instrument group to get the query routed to a valid server
	instr_node_px = (gen_buf_node_xt *)click_px->our_market_x.known_instruments_q.chain.next;
	instr_group_px = (instrument_xt *)instr_node_px->msg_p;

	// series is specified, to get the query thru the routing
	OMmsg_x.tx_x.query_past_trade_x.series.instrument_group_c =
		instr_group_px->instrument_group_u;

	// leave the search series as 0..we want all instrument groups

    }	// test if instrument group # supplied


    // set the message header - this includes zeroing the header and search series structures
    pack_OM_header ( &OMmsg_x, tx_length_u, 'C', 'Q', 2 );

    // specify country, market
    OMmsg_x.tx_x.query_past_trade_x.series.country_c = click_px->our_market_x.country_u;
    OMmsg_x.tx_x.query_past_trade_x.series.market_c = click_px->our_market_x.market_u;
  
    // save the facility code and fn code for loggin
    OMmsg_x.fn_code = LOG_K_Q_PAST_TRADES;
    OMmsg_x.facility_u = click_px->facility_no_u + 5;	// EP5/ET5

    printf ("Sending Trade Information (historical) Query\n");

    /* ??? what are we supposed to do with segment and partitions here...
	We don't seem to specify any partition info.. what we put in
	series,search is not described... and referring to other OM
	queries doesn't help much.. this partitioned / segmented combination
	is weird...
    */
    do
    {
	// next segment # - NB initially 0
	segment_u++;
	PUTSHORT ( OMmsg_x.tx_x.query_past_trade_x.segment_number_n, segment_u );

	status_i = do_click_query_rx ( click_px,
					&OMmsg_x,
					tx_length_u,
					LOG_K_R_PAST_TRADES);


	if ( status_i < SUCCESSFUL )
	{
	    // do nothing, error already reported by do_click_query
	    return status_i;
	}

	// how many items in the msg - endian
	PUTSHORT ( item_limit_u, click_px->log_reply_x.trans_reply_x.answer_past_trade_x.items_n );

	// now look at the actual trade data
	for ( item_index_u = 0; item_index_u < item_limit_u; item_index_u++ )
	{
	    // point to info about one trade
	    if (( trades_found_u == 0) && ( trades_others_u == 0))
	    {
		printf("Series           Quantity    Premium B/S Customer  Trd No Sequ. yyyymmdd:hhmmss X.Seq\n" );
	    }

	    missing_trade_px = &click_px->log_reply_x.trans_reply_x.answer_past_trade_x.item[item_index_u].cl_trade_api;

	    // count ones that match the instrument group we queried about
	    // if we asked for 'all' then every counts as in
	    if (( missing_trade_px->series.instrument_group_c
		    == OMmsg_x.tx_x.query_past_trade_x.series.instrument_group_c )
		    || ( instr_group_u == 0 ))
	    {
		// note how many more trades we found in this query
		trades_found_u++;
	    }
	    else
		trades_others_u++;

	    // do we need to spit out a header ?
	    got_a_cl_trade_api ( click_px, missing_trade_px );

	}   // for loop, processing all the components in one reply

	// could there be more trades ?
	/* regardless of whether there were any items in it, if the segment # in the response
	    is 0, then we got all the data there is. NB although this is endian, a binary
	    0 is still a 0 - don't have to do a PUTSHORT
	*/
	if (( item_limit_u == 0 )
	    || ( click_px->log_reply_x.trans_reply_x.answer_past_trade_x.segment_number_n == 0 ))
	    // no more data - out of here
	    break;
    }
    while (TRUE);

    printf ( "Got %u trades matching the query parameters, and %u others (not matching)\n",
		    trades_found_u,
		    trades_others_u );

    return status_i;
}   // query_historical_deals

// ---------------------------------------------------------------------------

int32 trade_rectify ( click_details_xt * click_px,
			int argc,
			char *argv[] )
/*
Function:   trade_rectify
Author:     Neil Cossor
Modified:   9908xx
Description:

    We have (previously) sent an order, which has traded.
    Unfortunately, some of the details relating to settlement
    were incorrect. Presumably OCC has failed the settlement,
    which the B/D knows about (by some other means).

    This transaction (CD33) allows the owner Broker/Dealer
    to 'fix' those incorrect things. The corrections are then
    forwarded to OCC (probably an overnight batch thing).

Input Params:

    the command line stuff (argc,argv[])
    argv[0]	TRADE
    argv[1]	RECTIFY
    argv[2]	<series name>
    argv[3]	<trade #>
    argv[4]	<CMTA member>
    argv[5]	<sub account ID>
    argv[6]	<account type>
    argv[7]	<open / close flag>
    argv[8]	<customer info> i.e. free text / Xref

Output Params:

Notes:

    Can you, as per amending an order, specify to NOT replace some fields,
    by putting NULs in those fields ? If so, then (for example), if the
    caller specifies 0 for the open / close flag, it will NOT change the
    setting for the trade...
*/
{
    // the message we will query with
    generic_txn_xt	    OMmsg_x;
    // size of our actual CLICK query/command
    uint32		    tx_length_u = sizeof ( OMmsg_x.tx_x.rectify_trade_x );
    int32		    status_i = SUCCESSFUL;
    // pointer to selected series
    ise_series_info_pt	    series_data_px = NULL;


    // zot the whole message
    memset( &OMmsg_x.tx_x.rectify_trade_x, 0, tx_length_u );


    // enuff args ?
    if ( argc < 9 )
    {
	printf ("Insufficient args :-\n"
		"  Trade,Rectify,<series name>,<trade #>,<CMTA #>,<sub a/c ID>,<a/c type>,<open/close>,<Xref>\n");
	return SUCCESSFUL;
    }

    // OK, get all the fields from the command line args

    // go find the series info for nominated series, and specify that
    series_data_px = find_series_by_name_s ( click_px,
						argv [2] );

    // did we find the series ?
    if (!series_data_px)
    {
	printf ("Cannot rectify trade - unknown series %s\n",
		    argv[2] );
	return SUCCESSFUL;
    }

    // save the series
    put_click_series ( &OMmsg_x.tx_x.rectify_trade_x.series,
			&series_data_px->ise_series_x );

    // the trade #
    OMmsg_x.tx_x.rectify_trade_x.trade_number_i = read_unsigned_limited ( argv[3], UNDEFINED_UINT32 );
    // the CMTA account and sub account ID are actually the two halves of client ID in the original order
    tx_click_string ( &OMmsg_x.tx_x.rectify_trade_x.cmta_member_s,
			argv[4],
			sizeof (OMmsg_x.tx_x.rectify_trade_x.cmta_member_s));
    tx_click_string ( &OMmsg_x.tx_x.rectify_trade_x.sub_account_id_s,
			argv[5],
			sizeof (OMmsg_x.tx_x.rectify_trade_x.sub_account_id_s));
    // the account type
    OMmsg_x.tx_x.rectify_trade_x.account_type_c = read_unsigned_limited ( argv[6],
										UNDEFINED_UINT8 );
    // the open / close flag for the account
    OMmsg_x.tx_x.rectify_trade_x.open_close_req_c = read_unsigned_limited ( argv[7],
										    OPEN_CLOSE_MAX );
    // and a new chunk of freetext / xref / client info
    tx_click_string ( &OMmsg_x.tx_x.rectify_trade_x.customer_info_s,
			argv[8],
			sizeof (OMmsg_x.tx_x.rectify_trade_x.customer_info_s));

    // set the message header - this includes zeroing the header and search series structures
    pack_OM_header ( &OMmsg_x, tx_length_u, 'C', 'D', 33 );


    // save the facility code and fn code for loggin
    OMmsg_x.fn_code = LOG_K_C_RECTIFY_TRADE_OCC;
    OMmsg_x.facility_u = click_px->facility_no_u + 3;	// EP3/ET3

    printf ("Sending Trade Rectify\n");

    status_i = do_click_tx_rx ( click_px,
				    &OMmsg_x,
				    tx_length_u );

    // any errors have already been reported in do_click_tx_rx
    return status_i;
}   // trade_rectify

// ---------------------------------------------------------------------------

int32 trade_rectify_parts ( click_details_xt * click_px,
			    int argc,
			    char *argv[] )
/*
Function:   trade_rectify_parts
Author:     Neil Cossor
Modified:   010712
Description:

    This transaction is similar to the CD33, for altering insensitive
    details of a trade, after the fact (clearing details etc).

    However, the CD36 transaction (new for 16.3) allows the client to
    change the attributes of the trade in pieces.. if the original trade was
    for 100 contracts, the trade could be altered differently for
    a set of 20, 30 and 50.

    Obviously the total quantity specified in all parts cannot exceed the
    total quantity of the trade.

    I presume that if the original was 100, and changes are done on
    lots of 20 and 30, then 50 remain with their original values.

Input Params:

    the command line stuff (argc,argv[])
    argv[0]	TRADE
    argv[1]	RECTIFY
    argv[2]	<series name>
    argv[3]	<trade #>

    argv[4]     <quantity we are changing details for part 1>
    argv[5]	<CMTA member for this qty>
    argv[6]	<sub account ID for this qty>
    argv[7]	<account type for this qty>
    argv[8]	<open / close flag for this qty>
    argv[9]	<customer info for this qty> i.e. free text / Xref

    argv[10]	<quantity we are changing details for part 2>
    ...

Output Params:

Notes:

    Can you, as per amending an order, specify to NOT replace some fields,
    by putting NULs in those fields ? If so, then (for example), if the
    caller specifies 0 for the open / close flag, it will NOT change the
    setting for the trade...
*/
{
    // the message we will query with
    generic_txn_xt	    OMmsg_x;
    // size of our actual CLICK query/command.. actually, this is a max length, for 100 parts
    uint32		    tx_length_u = sizeof ( OMmsg_x.tx_x.rectify_trade_parts_x );
    uint32		    part_count_u,	    // how many parts are we actually packing
			    part_index_u;	    // index for packing
    int32		    status_i = SUCCESSFUL;
    // pointer to selected series
    ise_series_info_pt	    series_data_px = NULL;


    // zot the whole message
    memset( &OMmsg_x.tx_x.rectify_trade_parts_x, 0, tx_length_u );


    // enuff args ?
    if ( argc < 10 )
    {
	printf ("Insufficient args :-\n"
		"  Trade,Partrectify,<series name>,<trade #>,<qty 1>,<CMTA # 1>,<sub a/c ID 1>,<a/c type 1>,<open/close 1>,<Xref 1>,[<qty 2>...]\n");
	return SUCCESSFUL;
    }

    // are the argument sets for the later parts complete
    // there are 6 args per part, and 4 fields that are common to all
    if ((( argc - 4 ) % 6 ) != 0)
    {
	// bad number of args
	printf ("bad args for Trade,Parts, must be 6 args for each part, after Trade,Parts,<series name>,<trade #>\n"
		"  for <qty>, <CMTA #>, <sub a/c ID>, <a/c type>, <open/close>, <Xref >\n");
	return SUCCESSFUL;
    }	// test for good # of args for components of parts

    part_count_u = ( argc - 4 ) / 6;

    // OK, get all the fields from the command line args

    // go find the series info for nominated series, and specify that
    series_data_px = find_series_by_name_s ( click_px,
						argv [2] );

    // did we find the series ?
    if (!series_data_px)
    {
	printf ("Cannot rectify trade - unknown series %s\n",
		    argv[2] );
	return SUCCESSFUL;
    }

    // save the series
    put_click_series ( &OMmsg_x.tx_x.rectify_trade_parts_x.series,
			&series_data_px->ise_series_x );

    // the trade #
    OMmsg_x.tx_x.rectify_trade_parts_x.trade_number_i = read_unsigned_limited ( argv[3], UNDEFINED_UINT32 );

    for ( part_index_u = 0;
	    part_index_u < part_count_u;
	    part_index_u++ )
    {
	// the quantity for the this part
	OMmsg_x.tx_x.rectify_trade_parts_x.item[part_index_u].trade_quantity_i
			= read_unsigned_limited ( argv[4 + 6*part_index_u], UNDEFINED_UINT32 );

	// the CMTA account and sub account ID are actually the two halves of client ID in the original order
	tx_click_string ( &OMmsg_x.tx_x.rectify_trade_parts_x.item[part_index_u].cmta_member_s,
			    argv[5 + 6*part_index_u],
			    sizeof (OMmsg_x.tx_x.rectify_trade_parts_x.item[part_index_u].cmta_member_s));
	tx_click_string ( &OMmsg_x.tx_x.rectify_trade_parts_x.item[part_index_u].sub_account_id_s,
			    argv[6 + 6*part_index_u],
			    sizeof (OMmsg_x.tx_x.rectify_trade_parts_x.item[part_index_u].sub_account_id_s));
	// the account type
	OMmsg_x.tx_x.rectify_trade_parts_x.item[part_index_u].account_type_c 
			= read_unsigned_limited ( argv[7 + 6*part_index_u], UNDEFINED_UINT8 );
	// the open / close flag for the account
	OMmsg_x.tx_x.rectify_trade_parts_x.item[part_index_u].open_close_req_c 
			= read_unsigned_limited ( argv[8 + 6*part_index_u], OPEN_CLOSE_MAX );
	// and a new chunk of freetext / xref / client info
	tx_click_string ( &OMmsg_x.tx_x.rectify_trade_parts_x.item[part_index_u].customer_info_s,
			    argv[9 + 6*part_index_u],
			    sizeof (OMmsg_x.tx_x.rectify_trade_parts_x.item[part_index_u].customer_info_s));
    }	// loop, collecting args for all parts

    // correct the length of the msg, based on the # parts we actually filled in
//    tx_length_u -= ( ( 100 - part_count_u ) * sizeof ( rectify_trade_ise_item_t ));
    // note the number of parts in the message
    OMmsg_x.tx_x.rectify_trade_parts_x.items_c = part_count_u;
    // set the message header
    pack_OM_header ( &OMmsg_x, tx_length_u, 'C', 'D', 36 );


    // save the facility code and fn code for loggin
    OMmsg_x.fn_code = LOG_K_C_RECTIFY_TRADE_ISE;
    OMmsg_x.facility_u = click_px->facility_no_u + 3;	// EP3/ET3

    printf ("Sending Trade Rectify by parts\n");

    status_i = do_click_tx_rx ( click_px,
				    &OMmsg_x,
				    tx_length_u );

    // any errors have already been reported in do_click_tx_rx
    return status_i;
}   // trade_rectify_parts
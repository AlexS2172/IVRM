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


Title : underlying.c
Author: Neil Cossor
Function :

    This file provides handling of basic info about underyling to the ISESim.exe program.
    Typically the underlying are equities, although this is NOT a requirement.

    This module now provides (Jan 2000) basic functionality on synthetics.. which are closer
    to underlying than anything else.

Revision history :-

yymmdd	vers'n	author	    notes
------	------	------	    -----
9905xx	v0.00	Neil Cossor initial version
990910	v0.01		    fixed bug in query_all_underlying_statii, which made the
			    same query N times, where N=# of bins..
990913			    add release_under_sort_tables (), mod clear_underlying_from_memory to use it
			    mod sort_underlying_tables (extra param) to selectively use it
990915			    fix bug.. we were not releasing the chain of series attached to each underlying
			    mod for extra param to free_gen_buf_node_q
990928			    add routine to check that all underlying have all instrument classes
			    fix bugs - we did NOT correctly halt on segment query, when returned a 0 segment #
990929			    mod display_one_underlying to have 'detail' option.. lots of stuff
990930				include instr class stuff in detailed display_one_underlying
991022	v0.02		    mod query_underlying_price to return # of records if OK status
			    mod enter_underlying_price_query to report record count, if query OK
991111	v0.03		    mod display_one_underlying to report primary exchange\
991127	v0.04		    add new routine commodity_code_to_string
000105			    update display_one_under_bbo to include nasdaq tick indicator
			    update query_underlying_price to grab new nasdaq fields
000118			    add find_synthetic_by_commodity and display_synthetics
000403	v0.05		    mod find_under_by_name_s to handle null-string input
000422	v1.00		    update to match OM release 15.7-3 (move of some fields from da203 to da210).
			    display_one_underlying (now reports if decimal / fraction)
			    Rename check_all_underlying_have_instr_classes -> check_all_underlying_instr_classes,
000424			    and add some more internal instrument class consistency checks.
000425	v1.01		    add list_underlyings (dumps file list of underlying)				
000505	v1.02		    fix bug in check_all_underlying_instr_classes.. reported country and market
				names in opposite locations
010305	v1.03		    add originator_id in display_one_under_bbo,
			    adjust some display field widths
010313			    mod display_one_underlying to display mm-rule tickable, fix some comments
010529	v1.04		    change check_all_underlying_instr_classes, we now keep pointer to one
				tradeable instr class for each underlying
*/
#include "stdincl.h"

#include "error.h"
#include "instrument.h"
#include "misc.h"
#include "underlying.h"
#include "utility.h"


// ---------------------------------------------------------------------------

char * originator_id_to_string ( uint8 originator_id_u )
/*
Function:   originator_id_to_string
Author:	    Neil Cossor
Modified:   20010304
Description

    We have an Originator ID as a single byte.
    Convert from this to readable string.
*/
{
    static char *originator_id_s [] =
    {
	"Unknown 0",
	"NASDAQ L1",
	"NASDAQ ntl",
	"American",
	"Boston",
	"Chicago",
	"NYSE",
	"Pacific",
	"Philadelphia",
	"Cincinatti",
	"         "	// space reserved to put string form of number value, for unknown case
    };

    switch (originator_id_u)
    {
    case 0:
	    return originator_id_s[0];
	    break;
    case 1:
	    return originator_id_s[1];
	    break;
    case 2:
	    return originator_id_s[2];
	    break;
    case 3:
	    return originator_id_s[3];
	    break;
    case 4:
	    return originator_id_s[4];
	    break;
    case 5:
	    return originator_id_s[5];
	    break;
    case 6:
	    return originator_id_s[6];
	    break;
    case 7:
	    return originator_id_s[7];
	    break;
    case 8:
	    return originator_id_s[8];
	    break;
    case 9:
	    return originator_id_s[9];
	    break;
    default:
	    sprintf ( originator_id_s[10],
			"?%3d?   ",
			originator_id_u );
	    return originator_id_s[10];
    }
}   // originator_id_to_string

// ---------------------------------------------------------------------------

char * underlying_status_to_string ( uint8 trading_status_u )
/*
Function:   underlying_status_to_string
Author:     Neil Cossor
Modified:   9907xx
Description:

    We have a underlying status number, which we wish to output.
    Convert from the number into a readable string

Input Params:

    The underlying trading status #

Output Params:

    Points to the resultant string
*/
{
    static char *trading_status_s [] =
    {
	"Closed  ",
	"Open    ",
	"Suspend ",
	"Resumed ",
	"Fast Mkt",
	"        "	// space reserved to put string form of number value, for unknown case
    };

    switch (trading_status_u)
    {
    case 0:
	    return trading_status_s[0];
	    break;
    case 1:
	    return trading_status_s[1];
	    break;
    case 2:
	    return trading_status_s[2];
	    break;
    case 3:
	    return trading_status_s[3];
	    break;
    case 4:
	    return trading_status_s[4];
	    break;
    default:
	    sprintf ( trading_status_s[5],
			"?%3d?   ",
			trading_status_u );
	    return trading_status_s[5];
    }
}   // underlying_status_to_string

// ---------------- internal routines ------------------------------------

static int cmp_under_ptr_by_name ( gen_buf_node_xt ** keybuf,
				    gen_buf_node_xt ** datumbuf )
/*
    routine needed to compare two the data pointed to by the supplied
    params, and return a -1, 0, +1 value for whether the data pointed
    by *keyval is <, ==, > the data pointed to by *datum.

    Comparison is of the string name of the underlying. These should be unique
    within a country / market.

    Used by qsort and bsearch.

    The input parameters are individual pointers to pointers to gen_buf_node_xt.
    We are comparing the ise_underlying_xt which are pointed to by the <msg_p> field
    of the gen_buf_node_xt's.
*/
{
    // pointers to ise_underlying_xt component of the gen_buf_node's we were given
    ise_underlying_pt	keyval, datum;

    // *keybuf == pointer to a gen_buf_node_xt (an element of the sorted array)
    // **keybuf == a gen_buf_node_xt
    gen_buf_node_xt * keybuf_ptr, * datumbuf_ptr;
    keybuf_ptr = *keybuf;
    datumbuf_ptr = *datumbuf;

    keyval = (ise_underlying_pt) keybuf_ptr->msg_p;
    datum = (ise_underlying_pt) datumbuf_ptr->msg_p;

    // compare the underlying names in the bottom structure
    return ( strcmp ( keyval->under_id_s,
			datum->under_id_s ) );
}   // cmp_under_ptr_by_name

// ---------------------------------------------------------------------------

static int cmp_under_ptr_by_commodity ( gen_buf_node_xt ** keybuf,
					gen_buf_node_xt ** datumbuf )
/*
    routine needed to compare two the data pointed to by the supplied
    params, and return a -1, 0, +1 value for whether the data pointed
    by *keyval is <, ==, > the data pointed to by *datum.

    Comparison is of the commodity number of the underlying.

    Used by qsort and bsearch.

    The input parameters are individual pointers to pointers to gen_buf_node_xt.
    We are comparing the ise_underlying_xt which are pointed to by the <msg_p> field
    of the gen_buf_node_xt's.
*/
{
    // pointers to ise_underlying_xt component of the gen_buf_node's we were given
    ise_underlying_pt	keyval, datum;

    // *keybuf == pointer to a gen_buf_node_xt (an element of the sorted array)
    // **keybuf == a gen_buf_node_xt
    gen_buf_node_xt * keybuf_ptr, * datumbuf_ptr;
    keybuf_ptr = *keybuf;
    datumbuf_ptr = *datumbuf;

    keyval = (ise_underlying_pt) keybuf_ptr->msg_p;
    datum = (ise_underlying_pt) datumbuf_ptr->msg_p;

    // compare the underlying commodity numbers in the bottom structure
    // use 3-way evaluate
    return ( ( keyval->commodity_code_u < datum->commodity_code_u ) ? -1 :
		( keyval->commodity_code_u > datum->commodity_code_u ) ? 1 :
		0 );
}   // cmp_under_ptr_by_commodity

// ---------------------------------------------------------------------------

static int cmp_under_ptr_by_bin ( gen_buf_node_xt ** keybuf,
					gen_buf_node_xt ** datumbuf )
/*
    routine needed to compare two the data pointed to by the supplied
    params, and return a -1, 0, +1 value for whether the data pointed
    by *keyval is <, ==, > the data pointed to by *datum.

    Comparison is of the bin # of the underlying.
    Actually, to allow 'nice' processing of the sorted array, we actually
    do a further comparison by underlying name, if the bin #s are the same...

    Used by qsort and bsearch.

    The input parameters are individual pointers to pointers to gen_buf_node_xt.
    We are comparing the ise_underlying_xt which are pointed to by the <msg_p> field
    of the gen_buf_node_xt's.
*/
{
    // pointers to ise_underlying_xt component of the gen_buf_node's we were given
    ise_underlying_pt	keyval, datum;

    // *keybuf == pointer to a gen_buf_node_xt (an element of the sorted array)
    // **keybuf == a gen_buf_node_xt
    gen_buf_node_xt * keybuf_ptr, * datumbuf_ptr;
    keybuf_ptr = *keybuf;
    datumbuf_ptr = *datumbuf;

    keyval = (ise_underlying_pt) keybuf_ptr->msg_p;
    datum = (ise_underlying_pt) datumbuf_ptr->msg_p;

    // compare the underlying commodity numbers in the bottom structure
    // use 3-way evaluate
    if ( keyval->bin_u < datum->bin_u )
	return -1;
    if ( keyval->bin_u > datum->bin_u )
	return 1;

    return ( ( keyval->commodity_code_u < datum->commodity_code_u ) ? -1 :
		( keyval->commodity_code_u > datum->commodity_code_u ) ? 1 :
		0 );
    // same bin #, now compare the underlying names
}   // cmp_under_ptr_by_bin

// -------------------------------------------------------------------------------------

void display_one_under_bbo ( ise_underlying_xt *under_data_px )
/*
Function : display_one_under_bbo
Author	 : Neil Cossor
Modified : 20010305
Description:

    We have a BBO for an underlying.
    This could be from a broadcast BO105, or a IQ121 query.

    In either case, spit out the interesting info onto stdout
*/
{
    // temp (formatted) prices for underlying
    under_price_st	bid_price_s,
			ask_price_s,
			high_price_s,
			low_price_s,
			last_price_s;
    // string for nasdaq tick up / down / flat indicator
    char		nasdaq_tick_s [10];
    // format the prices with the correct # of decimals
    format_whole_decimal_s ( (char *)&bid_price_s,
			    under_data_px->best_bid_premium_i,
			    under_data_px->dec_in_price_u,
			    MAX_UNDER_PRICE );	// total width for underlying price string
    format_whole_decimal_s ( (char *)&ask_price_s,
			    under_data_px->best_ask_premium_i,
			    under_data_px->dec_in_price_u,
			    MAX_UNDER_PRICE );	// total width for underlying price string
    format_whole_decimal_s ( (char *)&high_price_s,
			    under_data_px->high_price_i,
			    under_data_px->dec_in_price_u,
			    MAX_UNDER_PRICE );	// total width for underlying price string
    format_whole_decimal_s ( (char *)&low_price_s,
			    under_data_px->low_price_i,
			    under_data_px->dec_in_price_u,
			    MAX_UNDER_PRICE );	// total width for underlying price string
    format_whole_decimal_s ( (char *)&last_price_s,
			    under_data_px->last_price_i,
			    under_data_px->dec_in_price_u,
			    MAX_UNDER_PRICE );	// total width for underlying price string

    /* set up a nasdaq tick indicator. Note that these actually require very particular
	display formats (required by NASDAQ). As this is a demo program, and should never
	be connected to a production system, it will never have true live NASDAQ data,
	so it is not a particular concern.
    */
    switch ( under_data_px->nasdaq_bid_tick_u )
    {
    case 1:
	// down tick
	sprintf ( nasdaq_tick_s, "NASDAQ v" );
	break;
    case 2:
	// flat
	sprintf ( nasdaq_tick_s, "NASDAQ -" );
	break;
    case 3:
	// up tick
	sprintf ( nasdaq_tick_s, "NASDAQ ^" );
	break;
    default:
	sprintf ( nasdaq_tick_s, "" );
    }	// switch on NASDAQ tick indicator

    // as we inquired on exactly ONE underlying, we only have one line to report
    printf( "%-7s%8s%10s%10s%10s%10s%10s%10d %s %s\n",
	    &under_data_px->under_id_s,
	    underlying_status_to_string ( under_data_px->trading_status_u),
	    &bid_price_s,
	    &ask_price_s,
	    &high_price_s,
	    &low_price_s,
	    &last_price_s,
	    under_data_px->turnover_u,
	    &nasdaq_tick_s,
	    originator_id_to_string (under_data_px->originator_id_u)
	    );
}   // display_one_under_bbo

// -------------------------------------------------------------------------------------

static int32 query_underlying_price ( click_details_xt * click_px,
					ise_underlying_xt *under_data_px )
/*
Function : query_underlying_price
Author	 : Neil Cossor
Modified : 20010304
Description:

    Query prices (and volume) for an underlying, or all underlying
    Uses iq121

    The results (if successful) are written into the structure for
    the underlying. Response records are written out to stdout.
    
    Note that the info in that structure MAY also be updated by broadcasts,
    if the particular broadcast is subscribed to (which would be normal).

Output Params:

    If an error occurs, returns this (-ve) error status
    otherwise returns # of records found, which may be 0.
*/
{
    // the query msg
    generic_txn_xt	OMmsg_x;
    // pointer into the data - the response is always in an rx buffer in click_px->
    answer_underlying_prices_ia121_t
			*txn_reply_px;

    uint32		tx_length_u;
    // segment # - note we set initial value
    uint16		segment_u = 0;
    int32		status_i;	// temp, status of call
    uint16		item_index_u,	// loop var, extracting response data
			item_limit_u;	// limit on loop - may be endian-swapped from response
    // the commodity code of one underlying record in the response
    uint16		found_commodity_code_u;
    // the underlying record for one underlying record in the response
    ise_underlying_xt	*found_under_data_px;
    // how many items did we find
    uint32		found_items_u = 0;


    tx_length_u = sizeof ( OMmsg_x.tx_x.query_underlying_prices_x );
    // zot the request...
    memset( &OMmsg_x.tx_x.query_underlying_prices_x, NUL, tx_length_u );
    // put in our header
    pack_OM_header ( &OMmsg_x, tx_length_u, 'I', 'Q', 121 );
    // we MUST supply country and market
    OMmsg_x.tx_x.query_underlying_prices_x.series.country_c =
	click_px->our_market_x.country_u;
    OMmsg_x.tx_x.query_underlying_prices_x.series.market_c =
	click_px->our_market_x.market_u;


    // save the facility code and fn code
    OMmsg_x.fn_code = LOG_K_Q_SERIES_BIN;
    OMmsg_x.facility_u = click_px->facility_no_u + 1;	// EP1/ET1

    /* now qualify our selection - if the caller asked us to
	otherwise we are leaving a 0 wildcard (all underlying) and 0 for bin (all bins ??)
    */
    if ( under_data_px != NULL )
    {
	// put in the commodity code
	// NB endian order
	PUTSHORT ( OMmsg_x.tx_x.query_underlying_prices_x.series.commodity_n,
		    under_data_px->commodity_code_u );

	// and put in the Bin - 1 byte, not endian
	OMmsg_x.tx_x.query_underlying_prices_x.bin_c = under_data_px->bin_u;
    }

    // we either are doing one underlying - i.e. exactly one bin/commodity
    // OR we are doing all bins, for all commodities.
    do
    {
	segment_u++; // The segment # we wish to retrieve
	PUTSHORT( OMmsg_x.tx_x.query_underlying_prices_x.segment_number_n, segment_u );

	// go try to get some data
	status_i = do_click_query_rx ( click_px,
					&OMmsg_x,
					tx_length_u,
					LOG_K_R_UNDERLYING_PRICES );
	if ( status_i < SUCCESSFUL )
	
	    // do nothing, error already reported - just break from loop
	    break;

	// all OK - point to interesting part of data in standard rx bufer
	txn_reply_px = &click_px->log_reply_x.trans_reply_x.answer_underlying_prices_x;

	/* get # of items in msg - may be 0, if we have no prices
	    for the underlying - endian
	*/
	PUTSHORT ( item_limit_u,
		    txn_reply_px->items_n );
	// have we previously found items, and do we have any now i.e. do we need a header
	if (( found_items_u == 0 ) && ( item_limit_u != 0 ))
	    // now have current info in memory (from now or previously), display it
	    printf ("Stock  Status         Bid       Ask      High       Low      Last  Turnover\n");
	found_items_u += item_limit_u;

	// prices supplied for underlying - NB may have been supplied with 0 records
	for ( item_index_u = 0;
		item_index_u < item_limit_u;
		item_index_u++ )
	{
	    // extract the (endian) commodity code for this record
	    PUTSHORT ( found_commodity_code_u,
			txn_reply_px->item[item_index_u].commodity_n );
	    found_under_data_px = find_under_data_by_commodity ( click_px,
							    found_commodity_code_u );

	    // and update the fields in the common, for this underlying
	    PUTLONG ( found_under_data_px->best_bid_premium_i,
			txn_reply_px->item [item_index_u].best_bid_premium_i );
	    PUTLONG ( found_under_data_px->best_ask_premium_i,
			txn_reply_px->item [item_index_u].best_ask_premium_i );
	    PUTLONG ( found_under_data_px->closing_price_i,
			txn_reply_px->item [item_index_u].closing_price_i );
	    PUTLONG ( found_under_data_px->opening_price_i,
			txn_reply_px->item [item_index_u].opening_price_i );
	    PUTLONG ( found_under_data_px->high_price_i,
			txn_reply_px->item [item_index_u].high_price_i );
	    PUTLONG ( found_under_data_px->low_price_i,
			txn_reply_px->item [item_index_u].low_price_i );
	    PUTLONG ( found_under_data_px->last_price_i,
			txn_reply_px->item [item_index_u].last_price_i );
	    PUTLONG ( found_under_data_px->volume_u,
			txn_reply_px->item [item_index_u].volume_u );
	    PUTLONG ( found_under_data_px->turnover_u,
			txn_reply_px->item [item_index_u].turnover_u );
	    // note the NASDAQ fields, which will be 0 for non-NASDAQ i.e. CQS feed
	    found_under_data_px->nasdaq_bid_tick_u = txn_reply_px->item [item_index_u].nasdaq_short_sale_bid_tick_c;
	    // don't understand why the UPC indicator comes with each bid.. should be attribute of underlying
	    found_under_data_px->nasdaq_upc_u = txn_reply_px->item [item_index_u].nasdaq_upc_indicator_c;
	    // the originating exchange.. this is NOT the same as country-codes
	    found_under_data_px->originator_id_u = txn_reply_px->item [item_index_u].originator_id_c;


	    // note that we have update trade and BBO data
	    /* for reasons unknown.. Microsoft C does NOT seem to recognize 
		double assignments, like a,b = c. It just does the b = c part.
	    */
	    found_under_data_px->time_bbo_update_u = time(NULL);
	    found_under_data_px->time_trade_update_u = time(NULL);

	    // display the data
	    display_one_under_bbo ( found_under_data_px );

	}   // for loop, processing each underlying response record

	/* regardless of whether there were any items in it, if the segment # in the response
	    is 0, then we got all the data there is. NB although this is endian, a binary
	    0 is still a 0 - don't have to do a PUTSHORT
	*/
	if ( txn_reply_px->segment_number_n == 0 )
	    // no more data - out of here
	    break;
    } while ( TRUE );

    if ( status_i < 0 )
	return status_i;
    else
	return found_items_u;
}   // query_underlying_price

// -------------------------------------------------------------------------------------

/*
Function : enter_underlying_price_query
Author	 : Neil Cossor
Modified : 9905xx
Description:

    Inquire on the price (and volume) on a particular underlying
    If we have current information (from prior iq121, plus bcast updates)
    then just display the info we have.

    Calls query_underlying_price, which uses iq121.
    Triggered by a PRICES command.

Input Params:

    The command line, with any arguments.
    Note that the original command has been stripped from the command line.

Output Params:

    The price data in the underlying has been update in the common.
    Note that if no price info for the underlying is available,
    then the prices will probably have been set to 0.
*/
int32 enter_underlying_price_query ( click_details_xt * click_px,
					int argc,
					char *argv [] )
{
    int32		status_i;
    // pointer into the data about the underlying - if, of course, one was qualified
    ise_underlying_xt	*under_data_px;


    if (click_px->logged_on_i != TRUE)
    {
	prompt_logged_on ( "Prices" );
	return NOT_LOGGED_ON;
    }
    // enuff arguments supplied ?
    if ( argc <= 1 )
    {
	printf ("PRICES,<underlyingname>   OR    PRICES,*\n");
	return INVALID_ARGUMENTS;
    }

    // did the caller specify one underlying, or wildcard *
    if ( strcmp ( argv[1], "*") == 0 )
    {
	// wildcard, all underlying
	// this will display the results it gets
	status_i = query_underlying_price ( click_px,
						NULL );
    }
    else
    {
	// specific underlying.. see if we know that underlying
	// convert stock name to to commodity number
	under_data_px = find_under_by_name_s ( click_px,
						    argv[1] );

	if ( under_data_px == NULL )
	{
	    printf ("ERROR - Underlying cannot be found\n");
	    return INVALID_ARGUMENTS;
	}   // test if nominated underlying is known

	// found the underlying - have we already got current info i.e. do we need to query ?
	if (( under_data_px->time_bbo_update_u == 0 ) ||
	    ( under_data_px->time_trade_update_u == 0))
	{
	    // need to do query - go get the prices / volume for this underlying
	    // this will display the results it gets
	    status_i = query_underlying_price ( click_px,
						    under_data_px );
	}
	else
	{
	    // already have current data in stock.. display it
	    printf ("Stock           Status  \tBid\t      Ask      High       Low      Last  Turnover\n");
	    display_one_under_bbo ( under_data_px );
	    status_i = 1;
	}   // test if we already had current data for this underlying
    }	// test if specific underlying, or all

    if ( status_i >= 0 )
	printf ("Found %d underlying price records\n",
		    status_i );

    return status_i;
}   // enter_underlying_price_query

// -------------------------------------------------------------------------------------

int32 query_all_underlying_statii ( click_details_xt * click_px )
/*
Function : query_all_underlying_statii
Author	 : Neil Cossor
Modified : 9906xx
Description:

    Query trading status for all underlying in all bins.
    Uses iq132

    The results (if successful) are written into the structure for
    the underlying.
    
    Note that the info in that structure MAY also be updated by broadcasts,
    if the particular broadcast is subscribed to (which would be normal)

    As we can get up to 5000 statii in a single response for each bin,
    it will only take a very short set to get all data.. so don't bother
    with doing individual underlying, or just one bin

    Note that this query is only done AFTER loading the complete CDB info
    i.e. all of the dq queries, so we know about all underlying, instr classes etc
*/
{
    // the query msg
    generic_txn_xt	OMmsg_x;
    // pointer into the data - the response is always in an rx buffer in click_px->
    answer_underlying_status_us_t
			*txn_reply_px;

    uint32		tx_length_u;
    // segment #
    uint16		segment_u;
    int32		status_i;	// temp, status of call
    uint16		item_index_u,	// loop var, extracting response data
			item_limit_u;	// limit on loop - may be endian-swapped from response
    // the commodity code of one underlying record in the response
    uint16		found_commodity_code_u;
    // the underlying record for one underlying record in the response
    ise_underlying_xt	*found_under_data_px;


    tx_length_u = sizeof ( OMmsg_x.tx_x.query_underlying_status_x );
    // zot the request...   which includes commodity code of 0
    memset( &OMmsg_x.tx_x.query_underlying_status_x, NUL, tx_length_u );
    // put in our header
    pack_OM_header ( &OMmsg_x, tx_length_u, 'I', 'Q', 132 );
    // we MUST supply country and market
    OMmsg_x.tx_x.query_underlying_status_x.series.country_c =
	click_px->our_market_x.country_u;
    OMmsg_x.tx_x.query_underlying_status_x.series.market_c =
	click_px->our_market_x.market_u;


    // save the facility code and fn code
    OMmsg_x.fn_code = LOG_K_Q_SERIES_BIN;
    OMmsg_x.facility_u = click_px->facility_no_u + 1;	// EP1/ET1


    // we are doing all commodities.
    segment_u = 0;

    do
    {
	segment_u++; // The segment # we wish to retrieve
	PUTSHORT( OMmsg_x.tx_x.query_underlying_status_x.segment_number_n, segment_u );

	// go try to get some data
	status_i = do_click_query_rx ( click_px,
					&OMmsg_x,
					tx_length_u,
					LOG_K_R_UNDERLYING_STATII );
	if ( status_i < SUCCESSFUL )
	{
	    // do nothing, error already reported - we will drop from loop
	}
	else
	{
	    // all OK - point to interesting part of data in standard rx buffer
	    txn_reply_px = &click_px->log_reply_x.trans_reply_x.answer_underlying_status_x;

	    /* get # of items in msg, endian - may be 0, if we have no prices
		for the underlying
	    */
	    PUTSHORT ( item_limit_u,
			txn_reply_px->items_n );

	    // prices supplied for underlying - NB may have been supplied with 0 records
	    for ( item_index_u = 0;
		    item_index_u < item_limit_u;
		    item_index_u++ )
	    {
		// extract the (endian) commodity code for this record
		PUTSHORT ( found_commodity_code_u,
			    txn_reply_px->item[item_index_u].commodity_n );
		found_under_data_px = find_under_data_by_commodity ( click_px,
								found_commodity_code_u );

		// what if we get an update for an underlying we don't know about
		if ( found_under_data_px )
		{
		    // and update the status in the common, for this underlying - 1 byte, not endian
		    found_under_data_px->trading_status_u =
				txn_reply_px->item [item_index_u].underlying_trading_status_c;
		}
		else
		{

		    printf ("Warning - received unknown commodity code %hu in iq132\n",
			    found_commodity_code_u );

		}
	    }   // loop, extracting individual underlying data

	    /* regardless of whether there were any items in it, if the segment # in the response
		is 0, then we got all the data there is. NB although this is endian, a binary
		0 is still a 0 - don't have to do a PUTSHORT
	    */
	    if ( txn_reply_px->segment_number_n == 0 )
		// no more data - out of here
		break;
	}	// test if good status getting a block of data
    } while (( status_i >= SUCCESSFUL ) &&
		( item_limit_u > 0 ));

    return status_i;
}   // query_all_underlying_statii

// -------------------------------------------------------------------------------------

void release_underlying_series_chains ( country_market_xt *country_market_px )
/*
Function : release_under_sort_tables
Author   : Neil Cossor
Modified : 9905xx
Description:

    We are discarding our links from underlying to child series.
    This could be becoz we have gone offline, or becoz we just received
    update data which invalidates our old info
*/
{
    // pointer to chained block, which has a pointer (msg_p) to the info about a underlying
    gen_buf_node_xt	    *under_node_px;
    ise_underlying_xt	    *under_data_px = NULL;	// ptr to info about underlying

    // point to first block in the underlying chain
    under_node_px = (gen_buf_node_xt *)country_market_px->known_underlying_q.chain.next;
    while ( under_node_px != NULL )
    {
	// point to the actual underlying info
	under_data_px = (ise_underlying_xt *)under_node_px->msg_p;

	// release the list of series associated with the underlying
	free_gen_buf_node_q ( &under_data_px->under_series_hdr_x, FALSE );

	// advance to next.. which may of course not exist...
	under_node_px = (gen_buf_node_xt *)under_node_px->chain.next;
    }	// while ( under_node_px != NULL )
}   // release_underlying_series_chains

// -------------------------------------------------------------------------------------

static void release_under_sort_tables ( country_market_xt * country_market_px )
/*
Function : release_under_sort_tables
Author   : Neil Cossor
Modified : 9905xx
Description:

    We have are discarding our known (sorted) tables relating to underlying.
    This could be becoz we have gone offline, or becoz we just received
    update data which invalidates our old sorted info
*/
{
    free ( country_market_px->underlying_sorted_by_name_p );
    country_market_px->underlying_sorted_by_name_p = 0;
    free ( country_market_px->underlying_sorted_by_commodity_p );
    country_market_px->underlying_sorted_by_commodity_p = 0;
}   // release_under_sort_tables

// -------------------------------------------------------------------------------------

void clear_underlying_from_memory ( click_details_xt *click_px )
/*
Function : clear_underlying_from_memory
Author   : Neil Cossor
Modified : 9905xx
Description:

    We have just gone offline (or are exiting).
    Release all structures related to underlying.
*/
{
    // release all the links from the underlying to child series
    release_underlying_series_chains ( &click_px->our_market_x );

    // empty the whole q, zot the count of loaded underlying,
    // and release the sort structures we have pointing into them
    free_gen_buf_node_q ( &click_px->our_market_x.known_underlying_q, TRUE );

    release_under_sort_tables ( &click_px->our_market_x );
}   // clear_underlying_from_memory

// -------------------------------------------------------------------------------------

void display_one_underlying ( click_details_xt * click_px,
				ise_underlying_xt *under_data_px,
				unsigned detailed_u )
/*
Function:   display_one_underlying
Author:     Neil Cossor
Modified:   20010313
Description:

    Spit out (on stdout) info about one (known) underlying

Input Params:

    pointer to data about everything we know about one country market
    pointer to data about the underlying in question
    flag, of whether we want detailed info (value TRUE), or just summary (value FALSE)
*/
{
    // local loop var
    unsigned	loop_u;
    // temp, ptr to (chained) data about one instrument group, default NULL
    gen_buf_node_xt	    *instr_node_px = NULL;
    // our block of info about one instrument group
    instrument_xt	    *instr_data_px = NULL;
    // local series, which we use in search for instr class.
    // we only put in country, market, commodity code, and instr group for search
    ise_series_xt	    series_x;
    // pointer to data about one instrument class for this underlying
    instrument_class_pt	    instr_class_data_px;
    // string vars for display tick info, nicely formatted with appropriate decimals
    under_price_st	    tick_size_s,
			    tick_lower_s,
			    tick_upper_s;

    // display the stock in columns, all left justified

    // underlying code (usually 3-6 chars),
    // full underlying name
    // and underlying code i.e. commodity code
    // trading status (string)
    // bin #
    // orderbook #
    // # of series
    printf ( "%-6s\t%-32s\t%-8s\t%-4hu\t%d\t%d\t%u\n",
		&under_data_px->under_id_s,
		&under_data_px->under_fullname_s,
		underlying_status_to_string ( under_data_px->trading_status_u),
		under_data_px->commodity_code_u,
		under_data_px->bin_u,
		under_data_px->orderbook_u,
		under_data_px->under_series_hdr_x.length_i
	    );
    // should we be spitting out more detailed info
    if ( detailed_u == TRUE )
    {
	// CUSIP
	// currency name
	// decimals in price
	// 
	printf ("    CUSIP='%-12s', %-3s, %s, %hu decimals, primary exchange %s\n",
		&under_data_px->cusip_code_s,
		&under_data_px->currency_s,
		underlying_type_to_string(under_data_px->under_type_u),
		under_data_px->dec_in_price_u,
		exchange_to_string ( click_px, under_data_px->country_u )
		);
#ifdef WIN32
	// display the nominal value and position limit.. don't know the formatting strings for
	// C other than MS DevStudio
	printf ("    Posn limit %u, Nominal value %u\n",
		    under_data_px->position_limit_u,
		    under_data_px->nominal_value_u );
#endif
	for ( loop_u = 0;
		loop_u < under_data_px->fast_levels_u;
		loop_u++ )
	{
	    printf ( "    FM%hu=%-5.3fs",
			loop_u+1,
			(float)(under_data_px->fast_level_x[loop_u].match_interval_i / 1000 )
		    );
	}   // for loop, dumping out fast market info
	printf ("\n");


	// now show info in the instrument classes for this underlying

	// zot all fields of our search series
	memset ( &series_x, 0, sizeof ( series_x ));
	// and grab the country and market we are looking for
	series_x.country_u = click_px->our_market_x.country_u;
	series_x.market_u = click_px->our_market_x.market_u;
	series_x.underlying_code_u = under_data_px->commodity_code_u;

	// point to first entry in chain of known instruments - may be empty
	instr_node_px = (gen_buf_node_xt *)click_px->our_market_x.known_instruments_q.chain.next;
	do	// while (TRUE)
	{
	    // have we got a block of data, containing info about an instrument group
	    if ( instr_node_px == NULL )
	    {
		break;
	    }
	    else
	    {
		// point into the actual instrument group data
		// this is NOT allowed to be NULL
		instr_data_px = (instrument_xt *)instr_node_px->msg_p;

		// grab the instrument group, for instr class search
		series_x.instrument_group_u = instr_data_px->instrument_group_u;

		// see if we have an instrument class for this underlying / instr group
		instr_class_data_px = find_instrument_class_by_series ( &click_px->our_market_x,
									&series_x );
		if ( instr_class_data_px != NULL )
		{
		    // report info about this instrument class
		    printf ( "\n    %s, %hu premium decimals, %hu strike decimals, %hu contract decimals, traded=%s\n",
				&instr_data_px->instrname_s,
				instr_class_data_px->dec_in_premium_u,
				instr_class_data_px->dec_in_strike_price_u,
				instr_class_data_px->dec_in_contr_size_u,
				(instr_class_data_px->traded_u==1 ) ? "Yes" : "No"
			    );
		    // report contract size (no val in instr type)
		    printf ( "    Contract size %d, derivative level %hu",
				    instr_class_data_px->contract_size_i,
				    instr_class_data_px->derivative_level_u );
		    // report about BBO vols
		    if ( instr_class_data_px->bbo_volume_i == 0)
			// val from instr type, not overridden by class
			printf ( ", BBO vol %d (instr type)\n",
				    instr_data_px->bbo_volume_i );
		    else
			// val from instr class (override instr type)
			printf ( ", BBO vol %d (instr class)\n",
					instr_class_data_px->bbo_volume_i );

		    // report about block order params
		    if ( instr_class_data_px->block_order_min_size_i == 0)
			// val from instr type, not overridden by class
			printf ( "    Block min %d (instr type)",
					instr_data_px->block_order_min_size_i );
		    else
			// val from instr class (override instr type)
			printf ( "    Block min %d (instr class)",
					instr_class_data_px->block_order_min_size_i );
		    if ( instr_class_data_px->block_order_rsptime_i == 0)
			// val from instr type, not overridden by class
			printf ( ", Block resp %.3f secs (instr type)",
				    (float)(instr_data_px->block_order_rsptime_i/1000) );
		    else
			// val from instr class (override instr type)
			printf ( ", Block resp %.3f secs (instr class)",
				    (float)(instr_class_data_px->block_order_rsptime_i/1000) );

		    // report about facilitation order params
		    // vals from instr class (there is no param for these in instr type)
		    printf ( ", Facil min %d, Facil resp %.3f secs\n",
				instr_class_data_px->fac_order_min_size_i,
				(float)(instr_class_data_px->fac_order_rsptime_i/1000)
			    );

		    // report on ranking type, and price tick table
		    printf ( "    Ranking=%-8s, Price Ticktable has %-3hu entries",
				 ( instr_class_data_px->rnt_id_u == 1 ) ? "Normal" : "Inverse",
				 instr_class_data_px->tick_count_u );
		    if ( instr_class_data_px->tick_count_u != 0 )
		    {
			// decimals versus fractional trading
			printf (", traded in %s\n",
				(instr_class_data_px->trades_in_decimals_u==TRUE) ? "decimals":"fractions"
				);

			for ( loop_u = 0;
				loop_u < instr_class_data_px->tick_count_u;
				loop_u++ )
			{
			    // format tick info nicely w.r.t. implied decimals
			    format_whole_decimal_s ( (char *)&tick_size_s,
							instr_class_data_px->tick_size_x [loop_u].step_size_i,
							instr_class_data_px->dec_in_premium_u,
							10 );	// width of 10
			    format_whole_decimal_s ( (char *)&tick_lower_s,
							instr_class_data_px->tick_size_x [loop_u].lower_limit_i,
							instr_class_data_px->dec_in_premium_u,
							10 );	// width of 10
			    format_whole_decimal_s ( (char *)&tick_upper_s,
							instr_class_data_px->tick_size_x [loop_u].upper_limit_i,
							instr_class_data_px->dec_in_premium_u,
							10 );	// width of 10
			    printf ( "    Tick%hu size %s, lower %s, upper %s\n",
					loop_u+1,
					&tick_size_s,
					&tick_lower_s,
					&tick_upper_s
				    );
			}   // for loop, dumping out price ticktable

			printf ("\n");
		    }	// test if any price ticktable to report on

		    // report MM-rule tick table
		    printf ( "    MM rule ticktable has %-3hu entries\n",
				 instr_class_data_px->mm_rule_tick_count_u );
		    if ( instr_class_data_px->mm_rule_tick_count_u != 0 )
		    {
			for ( loop_u = 0;
				loop_u < instr_class_data_px->tick_count_u;
				loop_u++ )
			{
			    // format tick info nicely w.r.t. implied decimals
			    format_whole_decimal_s ( (char *)&tick_size_s,
							instr_class_data_px->mm_rule_tick_size_x [loop_u].step_size_i,
							instr_class_data_px->dec_in_premium_u,
							10 );	// width of 10
			    format_whole_decimal_s ( (char *)&tick_lower_s,
							instr_class_data_px->mm_rule_tick_size_x [loop_u].lower_limit_i,
							instr_class_data_px->dec_in_premium_u,
							10 );	// width of 10
			    format_whole_decimal_s ( (char *)&tick_upper_s,
							instr_class_data_px->mm_rule_tick_size_x [loop_u].upper_limit_i,
							instr_class_data_px->dec_in_premium_u,
							10 );	// width of 10
			    printf ( "    Tick%hu size %s, lower %s, upper %s\n",
					loop_u+1,
					&tick_size_s,
					&tick_lower_s,
					&tick_upper_s
				    );
			}   // for loop, dumping out mm-rule ticktable

			printf ("\n");
		    }	// test if any mm-rule ticktable to report on

		    printf ( "    StkPrcGrp1 %hd StkPrcGrp2 %hd StkPrcGrp3 %hd StkPrcGrp4 %hd StkPrcGrp5 %hd \n",
				instr_class_data_px->spg1_low_limit_u,
				instr_class_data_px->spg2_low_limit_u,
				instr_class_data_px->spg3_low_limit_u,
				instr_class_data_px->spg4_low_limit_u,
				instr_class_data_px->spg5_low_limit_u
			    );
		    if ( instr_class_data_px->exp_low_limit_count_u != 0 )
		    {
			for ( loop_u = 0;
				loop_u < instr_class_data_px->exp_low_limit_count_u;
				loop_u++ )
			{
			    printf ( "    ExpDateGrp%hu=%hu",
					loop_u+1,
					instr_class_data_px->exp_date_group_low_limit_u[loop_u]
				    );
			}   // for loop, dumping out expiration date group info

			printf ("\n");
		    }	// test if any expiry date groups to report on
		}   // test if we found an instr class for this under, instr group

		// try the next instrument - which may be NULL
		instr_node_px = (gen_buf_node_xt *)instr_node_px->chain.next;
	    }   // test if we had run out of instrument groups
	} while (TRUE);

    }	// test if we are supposed to spit out more detailed info
}   // display_one_underlying

// -------------------------------------------------------------------------------------

/*
Function : display_underlyings
Author	 : Neil Cossor
Modified : 9905xx
Description:

    Displays the list of known underlying on the standard output device.
    This is in response to the STOCKS command.

    The underlyings are a combination of the static data retrieved
    at logon by dq204, and the broadcasts bi204.

Input Params:-

    argv[0]	command that called us
    (optional)
    argv[1]	string selector, which we filter the output with
		If supplied, only show underlyings whose name start with that string,
		otherwise show all underlying
    argv[2]	DETAIL | BRIEF.. default BRIEF
*/
void display_underlyings ( click_details_xt *click_px,
			    int argc,
			    char *argv [] )
{
    // pointer to actual ise info about a series
    ise_underlying_xt	*under_data_px;
    // pointer to chained block, which has a pointer (msg_p) to the info about a series
    gen_buf_node_xt	*under_node_px;
    // loop vars
    unsigned		item_index_u;
    // what detailed / brief flag we give to lower routine (default FALSE)
    unsigned		detailed_u = FALSE;
    // the selection from a list of string choices
    int     choice_i;

    // declaration of string choices for brief/detail option
    static char *brief_detail_s [] =
    {
	"BRIEF",
	"DETAIL",
	"Unk"
    };
    // enumerated list, corresponding 1:1 (and in same order) as string list
    enum brief_detail_et { brief_detail_brief_e, brief_detail_detail_e };


    
    // did the caller specify detailed | brief
    if ( argc > 2 )
    {
	// enough args for them to have specified DETAIL | BRIEF
	choice_i = select_string_option ( 2, brief_detail_s, argv[2] );
	switch ( choice_i )
	{
	case brief_detail_brief_e:
	    detailed_u = FALSE;
	    break;
	case brief_detail_detail_e:
	    detailed_u = TRUE;
	    break;
	default:
	    printf ("One of BRIEF or DETAIL must be specified\n");
	    return;
	}
    }	// test if we were given a param for brief / detail

    // dump them in alphabetical order (by short underlying code)
    if ( click_px->our_market_x.underlying_sorted_by_name_p != NULL )
    {
	// we have a (sorted) array of pointers into the underlying.
	// whiz thru them in a for loop

	printf ( "Name  \tFull Name\t\t\t\tStatus   \tCode\tBin\tOrderbk\t#series\n");

	for (item_index_u=0;
		item_index_u<click_px->our_market_x.known_underlying_q.length_i;
		item_index_u++)
	{
	    // point to actual block in the series chain
	    under_node_px = (gen_buf_node_pt)click_px->our_market_x
				    .underlying_sorted_by_name_p
					->orig_data_px [ item_index_u ];
	    // point to the actual series info
	    under_data_px = (ise_underlying_xt *)under_node_px->msg_p;

	    // were we supplied with a selection filter ?
	    // If so, only use underlying which start with given string
	    if ( ( argv[1] == NULL) ||
		    // we were supplied with a 'start' of underlying to filter with
		    ( strstr ( under_data_px->under_id_s, argv[1] ) ==
			(char *)&under_data_px->under_id_s ) )
	    {
		display_one_underlying ( click_px,
					    under_data_px,
					    detailed_u );
	    }	// test if this underlying matches any supplied underlying name filter
	}   // for loop, thru all stocks

	printf( "\n" );
    }
    else
    {
	printf (" There are no underlying in memory\n");
    }
}   // display_underlyings

// -------------------------------------------------------------------------------------

void list_underlyings ( click_details_xt *click_px,
			    int argc,
			    char *argv [] )
/*
Function : list_underlyings
Author	 : Neil Cossor
Modified : 20000425
Description:

    Opens an output file (underlying.txt), and lists the known underlying,
    and closes the files.
    The underlying are listed with just the symbol, one per line

    The underlyings are a combination of the static data retrieved
    at logon by dq204, and the broadcasts bi204.

Input Params:-

    argv[0]	LIST command that called us
    argv[1]	UNDERLYING, that got us here
*/
{
    // pointer to actual ise info about an underlying
    ise_underlying_xt	*under_data_px;
    // pointer to chained block, which has a pointer (msg_p) to the info about an underlying
    gen_buf_node_xt	*under_node_px;
    // loop vars
    unsigned		item_index_u;
    FILE *listfile_px;		// pointer to file for output

    // nothing to do if no underlying in memory
    if ( click_px->our_market_x.underlying_sorted_by_name_p == NULL )
    {
	printf (" There are no underlying in memory\n");
	return;
    }

    // output file for underlying list - wipe existing file
    listfile_px = fopen ( "underlying.txt", "w");

    if ( listfile_px == 0 )
    {
	printf ("error creating output underlying listfile\n");
	return;
    }

    
    // dump them in alphabetical order (by short underlying code)
    for (item_index_u=0;
	    item_index_u<click_px->our_market_x.known_underlying_q.length_i;
	    item_index_u++)
    {
	// point to actual block in the series chain
	under_node_px = (gen_buf_node_pt)click_px->our_market_x
				.underlying_sorted_by_name_p
				    ->orig_data_px [ item_index_u ];
	// point to the actual series info
	under_data_px = (ise_underlying_xt *)under_node_px->msg_p;

	fprintf ( listfile_px, "%s\n",
		    under_data_px->under_id_s );

    }   // for loop, thru all stocks

    printf ("Listed %u items\n", click_px->our_market_x.known_underlying_q.length_i );

    fclose ( listfile_px );

}   // list_underlyings

//-----------------------------------------------------------------------------------

ise_underlying_xt * find_under_by_name_s ( click_details_xt * click_px,
						name_st search_under_s )
/*
Function:   find_under_by_name_s
Author:	    Neil Cossor
Modified:   20000403
Description:

    routine which searches in the known underlying for all country / markets
    to find the ise_underlying_xt which matches the underlying name supplied.

    Returns pointer to that structure, or NULL if no match found.

    NB this fn used the sorted array of pointers to underlying.
    This does order (n log 2) comparisons, rather than order (n/2).

    If the supplied search string is (exactly) "%U",
    then we return the default value (presuming it has previously been set)
*/
{
    // pointer to a component of the known_underlying_q
    gen_buf_node_xt * under_node_px = NULL;
    // result from bsearch, which is a pointer to a pointer to a gen_buf_node_xt, which contains the underlying
    gen_buf_node_xt ** underlying_data_ppx;
    // pointer to the ISE format data about one underlying, from the known_underlying_q
    ise_underlying_xt	* ise_under_info_px = NULL;
    // pointer to structure for a single country / market.
    country_market_xt	* country_market_px;

    // we need to create a temporary ise_underlying_xt, and a pointer to it, so we
    // can call bsearch. Just have one here on the stack - saves allocate / deallocate.
    ise_underlying_xt	ise_search_underlying_x;
    // and a gen_buf_node_xt which contains a pointer to the ise_search_underlying_x;
    gen_buf_node_xt gen_buf_node_search_underlying;

    // and a pointer to that gen_buf_node_xt
    gen_buf_node_xt *gen_buf_node_search_underlying_px = &gen_buf_node_search_underlying;

    // did we get a string at all ?
    if (strlen (search_under_s) == 0)
    {
	printf ("No <underlying name> specified\n");
	return NULL;
    }

    // is this the special.. the default string
    if (strcmp ( search_under_s, "%U" ) == 0 )
    {
	// yes, we should use the default series - is there one
	ise_under_info_px = find_under_by_name_s ( click_px, click_px->default_under_name_s );
	if ( ise_under_info_px )
	{
	    // found the underlying - return to caller
	    return ise_under_info_px;
	}
	else
	{
	    printf ("Cannot use that default underlying [%s]- no such underlying\n",
			&click_px->default_under_name_s );
	    return NULL;
	}   // test if we could find the default underlying
    }	// test if user wanted to use default underlying

    // copy the search underlying into search key structure
    strcpy ( (char *)&ise_search_underlying_x.under_id_s,
		search_under_s );
    // and set the <msg> pointer from the gen_buf_node_xt to point to the ise_search_underlying_x
    gen_buf_node_search_underlying.msg_p = (char *)&ise_search_underlying_x;

    /* we don't know which country / market a underlying (by name) is in.
	While name is MAY not be unforced as unique (by OM) within one
	country / market, it is almost certainly not enforced across
	multiple markets. ISE may choose operationally (they choose
	the underlying names) to make products uniquely named across
	multiple markets.. they may not.

	If this is the case, then searching by underlying name is NOT
	possible, without a market qualifier.

	For now, there is ONE country / market.
    */

    // point to the ONE known country market
    country_market_px = &click_px->our_market_x;

    // Do fast search within the sorted chain
    underlying_data_ppx = bsearch ( 
				&gen_buf_node_search_underlying_px,		    // pointer to search record
				&country_market_px->underlying_sorted_by_name_p     // pointer to first element of array
				    ->orig_data_px [ 0 ],
				country_market_px->known_underlying_q.length_i,     // # of entries in the array
				sizeof (ise_underlying_pt),		    // size of each element in the array
				(int (*) (const void *, const void *)) cmp_under_ptr_by_name ); // comparison fn
    // did we find a match ?
    if ( underlying_data_ppx != NULL )
    {
	// match.

	// what we have though is a pointer to an entry in the sorted array,
	// which is in itself a pointer to the original gen_buf_node_xt
	under_node_px = *underlying_data_ppx;
	
	// point to the underlying data in this Q entry
	ise_under_info_px = (ise_underlying_pt)under_node_px->msg_p;

	// returning pointer to this entry
	return ise_under_info_px;
    }	// test if bsearch found a match

    // oops, no match - report this
    return NULL;
}   // find_under_by_name_s

//-----------------------------------------------------------------------------------

gen_buf_node_xt * find_under_block_by_commodity ( click_details_xt * click_px,
						    uint16 search_under_code_u )
/*
    routine which searches in the known underlying for all country / markets
    to find the gen_buf_node_xt which matches the commodity code supplied.

    Returns pointer to that Q'ed structure, or NULL if no match found.

    NB as per find_under_by_name_s, this fn used the sorted array of pointers to underlying.
    This does order (n log 2) comparisons, rather than order (n/2).
*/
{
    // pointer to a component of the known_underlying_q
    gen_buf_node_xt * under_node_px = NULL;
    // result from bsearch, which is a pointer to a pointer to a gen_buf_node_xt, which contains the underlying
    gen_buf_node_xt ** underlying_data_ppx;
    // pointer to the ISE format data about one underlying, from the known_underlying_q
    ise_underlying_xt	* ise_under_info_px = NULL;
    // pointer to structure for a single country / market.
    country_market_xt	* country_market_px;

    // we need to create a temporary ise_underlying_xt, and a pointer to it, so we
    // can call bsearch. Just have one here on the stack - saves allocate / deallocate.
    ise_underlying_xt	ise_search_underlying_x;
    // and a gen_buf_node_xt which contains a pointer to the ise_search_underlying_x;
    gen_buf_node_xt gen_buf_node_search_underlying;

    // and a pointer to that gen_buf_node_xt
    gen_buf_node_xt *gen_buf_node_search_underlying_px = &gen_buf_node_search_underlying;
    // copy the search underlying commodity code into search key structure
    ise_search_underlying_x.commodity_code_u = search_under_code_u;
    // and set the <msg> pointer from the gen_buf_node_xt to point to the ise_search_underlying_x
    gen_buf_node_search_underlying.msg_p = (char *)&ise_search_underlying_x;

    /* we don't know which country / market a underlying is in.
	For now, there is ONE country / market.
    */

    // point to the ONE known country market
    country_market_px = &click_px->our_market_x;

    // Do fast search within the sorted chain
    underlying_data_ppx = bsearch ( 
				&gen_buf_node_search_underlying_px,		    // pointer to search record
				// pointer to first element of array
				&country_market_px->underlying_sorted_by_commodity_p
				    ->orig_data_px [ 0 ],
				country_market_px->known_underlying_q.length_i,     // # of entries in the array
				sizeof (ise_underlying_pt),			    // size of each element in the array
				(int (*) (const void *, const void *)) cmp_under_ptr_by_commodity );	// comparison fn
    // did we find a match ?
    if ( underlying_data_ppx != NULL )
    {
	// match.

	// what we have though is a pointer to an entry in the sorted array,
	// which is in itself a pointer to the original gen_buf_node_xt
	under_node_px = *underlying_data_ppx;
	
	// returning pointer to this entry
	return under_node_px;
    }	// test if bsearch found a match

    // oops, no match - report this
    return NULL;
}   // find_under_block_by_commodity

//-----------------------------------------------------------------------------------

ise_underlying_xt * find_under_data_by_commodity ( click_details_xt * click_px,
						    uint16 search_under_code_u )
/*
    routine which searches in the known underlying for all country / markets
    to find the ise_underlying_xt which matches the commodity code supplied.

    Returns pointer to that structure, or NULL if no match found.

    This just calls frind_under_block_by_commodity, but returns just the
    actual data part, not the Q'ed block which contains the pointer to that data.

    This routine is the most commonly used one, as most ISEsim internals
    aren't manipulating the Q of known underlying...
*/
{
    // pointer to a component of the known_underlying_q
    gen_buf_node_xt * under_node_px = NULL;
    // pointer to the ISE format data about one underlying, from the known_underlying_q
    ise_underlying_xt	* ise_under_info_px = NULL;

    under_node_px = find_under_block_by_commodity ( click_px, search_under_code_u );

    // did we find a match ?
    if ( under_node_px != NULL )
    {
	// match.

	// point to the underlying data in this Q entry
	ise_under_info_px = (ise_underlying_pt)under_node_px->msg_p;

	// returning pointer to this entry
	return ise_under_info_px;
    }	// test if bsearch found a match

    // oops, no match - report this
    return NULL;
}   // find_under_data_by_commodity

// -------------------------------------------------------------------------------------

void sort_underlying_tables ( country_market_xt * country_market_px,
				int replace_tables )
/*
Function:   sort_underlying_tables
Author:	    Neil Cossor
Modified:   990913
Description:

    We have just finished loading all info about underlying
    within a particular country and market
    OR
    we just got a broadcast, which changed our previous tables.
    This could include an add or delete (so the # of entries in our sorted chains
    have changed), or a modify (so the sort order may be different).

    Generate the internal (sorted) lookup tables, which allow fast lookup
    of underlying.

Input Params:

    pointer to everything we know about one country / market
    flag if we are replacing the tables. TRUE means replacing, FALSE means new tables
*/
{
    // how big we need each of the pointer arrays to be - NB they are the same size at present
    int size_of_ptr_array;
    int size_of_ptr;
    // temp loop var
    unsigned i;
    // temp, ptr to data about one underlying, default NULL
    gen_buf_node_xt	* under_node_px = NULL;

    if (replace_tables)
    {
	// first, must delete the old tables
	release_under_sort_tables ( country_market_px );
    }

    // how big the array of pointers needs to be. NB all pointers are the same size, so pick one for sizeof
    size_of_ptr = sizeof ( country_market_px->underlying_sorted_by_name_p );
    size_of_ptr_array = size_of_ptr * country_market_px->known_underlying_q.length_i;

    // allocate the space for the underlying pointer arrays
    country_market_px->underlying_sorted_by_name_p = (sorted_array_pt) malloc ( size_of_ptr_array );
    country_market_px->underlying_sorted_by_commodity_p = (sorted_array_pt) malloc ( size_of_ptr_array );

    // now fill each of these arrays with pointers to the unsorted underlying data
    // point to first in Q
    under_node_px = (gen_buf_node_xt *)country_market_px->known_underlying_q.chain.next;
    for ( i = 0; i < country_market_px->known_underlying_q.length_i; i++ )
    {
	// save ptr to underlying data, in the "by name" array of ptrs
	country_market_px->underlying_sorted_by_name_p->orig_data_px [i] = under_node_px;

	// save ptr to underlying data, in the "by commodity", underlying array of ptrs
	country_market_px->underlying_sorted_by_commodity_p->orig_data_px [i] = under_node_px;

	// advance to the following Q entry, which may be NULL
	under_node_px = (gen_buf_node_xt *)under_node_px->chain.next;
    }

    // calculate how far apart our pointers are in the array
    size_of_ptr = sizeof (ise_underlying_pt);

    // now sort each of the arrays. First by name.
    qsort ( &country_market_px->underlying_sorted_by_name_p	    // pointer to first element of array
			->orig_data_px [ 0 ],
		    country_market_px->known_underlying_q.length_i, // # of entries in the array
		    size_of_ptr,				    // size of each element in the array
		    (int (*) (const void *, const void *)) cmp_under_ptr_by_name ); // comparison fn

    // and then by commodity number
    qsort ( &country_market_px->underlying_sorted_by_commodity_p    // pointer to first element of array
			->orig_data_px [ 0 ],
		    country_market_px->known_underlying_q.length_i, // # of entries in the array
		    size_of_ptr,				    // size of each element in the array
		    (int (*) (const void *, const void *)) cmp_under_ptr_by_commodity );    // comparison fn


}   // sort_underlying_tables

// -----------------------------------------------------------------------------------

void update_one_underlying ( click_details_xt *click_px,
				ise_underlying_xt *under_data_px,
				da204_t *answer_underlying_px )
/*
Function : update_one_underlying
Author	 : Neil Cossor
Modified : 20000105
Description:

    We have a pointer to a da204 record (from a dq204 query, or a bi204 bcast),
    and we have a pointer to a ISEsim internal record to hold the data for this.

    The item may be an add (dq204, or bi204 add), where we have just allocated space
    or a modify (bi204 modify), where we have found the original block.

Input Params:

    pointer to structure containing everything we currently know about the central system
    pointer to the (ISEsim internal) structure to hold everything about one underlying
    pointer to the record containing everything about one underlying that CLICK told us
	in the latest msg
*/
{
    // temp index var, whizzing through the repetitive data for one underlying
    uint16		loop_u,
			temp_uint16;

    // rip off the data - done field by field, for byte order,
    // and CLICK string handling

    // grab short code name e.g. INTC
    // Do this early, so can use name in error reporting
    rx_click_string ( &under_data_px->under_id_s,
		    &answer_underlying_px->com_id_s,
		    sizeof ( answer_underlying_px->com_id_s ));
    // sanity check - has an underlying name ?
    if ( strlen ( under_data_px->under_id_s ) == 0 )
	printf ("Warning, underlying received with NUL name\n" );

    // grab the nominal value
    PUTLONG ( under_data_px->nominal_value_u,
		answer_underlying_px->nominal_value_q );
    // and the position limit.. BIG numbers
    PUTLONG ( under_data_px->position_limit_u,
		answer_underlying_px->position_limit_q );

    PUTLONG ( under_data_px->coupon_interest_u,
		answer_underlying_px->coupon_interest_i );
    PUTSHORT ( under_data_px->commodity_code_u,
		answer_underlying_px->commodity_n );
    // sanity check - has a good commodity code ?
    if ( under_data_px->commodity_code_u == 0 )
	printf ("Warning, underlying %s received with %d commodity code\n",
		    &under_data_px->under_id_s,
		    under_data_px->commodity_code_u );
    PUTSHORT ( under_data_px->day_count_u,
		answer_underlying_px->day_count_n );
    PUTSHORT ( under_data_px->days_in_interest_year_u,
		answer_underlying_px->days_in_interest_year_n );
    PUTSHORT ( under_data_px->coupon_settlement_days_u,
		answer_underlying_px->coupon_settlement_days_n );
    PUTSHORT ( under_data_px->dec_in_price_u,
		answer_underlying_px->dec_in_price_n );
    // grab 1-byte values - no byte swaps here
    under_data_px->country_u = answer_underlying_px->prm_country_c;
    under_data_px->bin_u = answer_underlying_px->bin_c;
    // note this bin #, if highest
    if ( under_data_px->bin_u > click_px->our_market_x.max_bin_u )
	click_px->our_market_x.max_bin_u = under_data_px->bin_u;
    // sanity check - is it non-zero
    if ( under_data_px->bin_u == 0 )
	printf ("Warning, underlying %s has bin # of %d\n",
		    &under_data_px->under_id_s,
		    under_data_px->bin_u );

    // note this orderbook #, if highest
    under_data_px->orderbook_u = answer_underlying_px->orderbook_c;
    if ( under_data_px->orderbook_u > click_px->our_market_x.max_orderbook_u )
	click_px->our_market_x.max_orderbook_u = under_data_px->orderbook_u;
    // sanity check - is it non-zero
    if ( under_data_px->orderbook_u == 0 )
	printf ("Warning, underlying %s has bin # of %d\n",
		    &under_data_px->under_id_s,
		    under_data_px->orderbook_u );

    under_data_px->under_type_u = answer_underlying_px->underlying_type_c;
    under_data_px->under_price_unit_u = answer_underlying_px->price_unit_c;

    // grab full underlying name
    rx_click_string ( &under_data_px->under_fullname_s,
		    &answer_underlying_px->name_s,
		    sizeof ( answer_underlying_px->name_s ));

    // grab CUSIP / isin code - may be NUL/blank e.g. AU0000AMC3
    rx_click_string ( &under_data_px->cusip_code_s,
		    &answer_underlying_px->cusip_s,
		    sizeof ( answer_underlying_px->cusip_s ));

    // grab the release date
    rx_click_string ( &under_data_px->release_yyyymmdd_s,
		    &answer_underlying_px->date_release_s,
		    sizeof (answer_underlying_px->date_release_s));
    // grab the termination date
    rx_click_string ( &under_data_px->termination_yyyymmdd_s,
		    &answer_underlying_px->date_termination_s,
		    sizeof (answer_underlying_px->date_termination_s));
    // grab the currency name
    rx_click_string ( &under_data_px->currency_s,
		    &answer_underlying_px->base_cur_s,
		    sizeof (answer_underlying_px->base_cur_s));
    // grab the currency name
    rx_click_string ( &under_data_px->prm_mm_s,
		    &answer_underlying_px->prm_mm_customer_s,
		    sizeof (answer_underlying_px->prm_mm_customer_s));
    // now grab the coupon info etc
    // the count of components N NB 1 < N <= 20. 1 byte field, so no endian issues
    under_data_px->coupons_u = answer_underlying_px->items_c;
    for ( loop_u = 0;
	loop_u < under_data_px->coupons_u;
	loop_u++ );
    {
	// grab the coupon date
	rx_click_string ( &under_data_px->coupon_x [loop_u].coupon_yyyymmdd_s,
			    &answer_underlying_px->
				coupon [ loop_u ]
				    .date_coupdiv_s,
			    sizeof (answer_underlying_px->
					coupon [ loop_u ]
					    .date_coupdiv_s));
    }   // loop, ripping out the coupon data

    // rip out the fast-market level data
    under_data_px->fast_levels_u = answer_underlying_px->items2_c;
    for ( loop_u = 0;
	    loop_u < under_data_px->fast_levels_u;
	    loop_u++ )
    {
	PUTLONG ( under_data_px->fast_level_x[loop_u].match_interval_i,
		    answer_underlying_px->fast_market_lvl[loop_u].match_interval_i
		);
	// are the 'levels' (endian) actually indexed in order, and with non-zero times
	PUTSHORT ( temp_uint16, answer_underlying_px->fast_market_lvl[loop_u].level_n );
	if (( under_data_px->fast_level_x[loop_u].match_interval_i <= 0 )
	    || ( temp_uint16 != (loop_u+1) ))
	{
	    printf ("Underlying %s has odd FM level %hu, time %d(milliseconds) in slot %hu\n",
			&under_data_px->under_id_s,
			temp_uint16,
			under_data_px->fast_level_x[loop_u].match_interval_i,
			loop_u );
	}   // test for 'sensible' FM level #s and interval values
    }   // for loop, dumping out fast market info

}   // update_one_underlying
		
// -------------------------------------------------------------------------------------

void check_all_underlying_instr_classes ( click_details_xt *click_px,
						country_market_xt * country_market_px )
/*
Function:   check_all_underlying_instr_classes
Author:	    Neil Cossor
Modified:   20010529
Description:

    We have complete data from the CDB.
    Check if we have instrument classes for all underlying.

    We also check that they have matching info, such as ranking types,
    tick tables, (and hence decimal trading vs fractions).
    Should have same ranking type, same decimals / fractions, same tick table.
    Should probably have same block / facilitation params

    We do this by looking through all known underlying, and for each look for
    an instrument class for all known instrument groups.

    Update :- 2001/10/17
    We do not require that each underlying have an instrument class for each defined
    isntument type.. this fails for underlyings defined purely as a component
    of a synthetic, AND if the ISE defines additional instrument groups (say Euro Calls
    and Puts), which might be traded (say) in a different ISE Market (different hours etc).

    To still provide some cross-checking, it can be expected (if the system is correctly
    configured.. ignoring the interim point of new underlying and classes being added)
    that each underlying should have an even number of tradable instrument classes.. 0 or 2.
    I ignore (for the present) the possibility of (say) both Euro and American options
    on the same underlying...
*/
{
    // pointer to chained block, which has a pointer (msg_p) to the info about a underlying
    gen_buf_node_xt	    *under_node_px;
    ise_underlying_xt	    *under_data_px = NULL;	// ptr to info about underlying

    // temp, ptr to (chained) data about one instrument group, default NULL
    gen_buf_node_xt	    *instr_node_px = NULL;
    // our block of info about one instrument group
    instrument_xt	    *instr_data_px = NULL;
    // pointer to instr class we find for one instr group / underlying combo
    instrument_class_pt	    ise_instr_class_info_px;
    // local series, which we use in search for instr class.
    // we only put in country, market, commodity code, and instr group for search
    ise_series_xt	    series_x;
    // count of how many tradable instrument classes we have on the current underlying we are checking
    unsigned		    tradable_classes_u;


    // zot all fields of our search series
    memset ( &series_x, 0, sizeof ( series_x ));
    // and grab the country and market we are looking for
    series_x.country_u = country_market_px->country_u;
    series_x.market_u = country_market_px->market_u;

    // point to first block in the underlying chain
    under_node_px = (gen_buf_node_xt *)country_market_px->known_underlying_q.chain.next;
    while ( under_node_px != NULL )
    {
	// point to the actual underlying info
	under_data_px = (ise_underlying_xt *)under_node_px->msg_p;

	// reset count of tradable classes for this underlying
	tradable_classes_u = 0;

	// grab the commodity code for this underlying, for instr class search
	series_x.underlying_code_u = under_data_px->commodity_code_u;

	// check that we have an instrument class, for all known instrument groups

	// point to first entry in chain of known instruments - may be empty
	instr_node_px = (gen_buf_node_xt *)country_market_px->known_instruments_q.chain.next;
	do	// while (TRUE)
	{
	    // have we got a block of data, containing info about an instrument group
	    if ( instr_node_px == NULL )
	    {
		break;
	    }
	    else
	    {
		// point into the actual instrument group data
		// this is NOT allowed to be NULL
		instr_data_px = (instrument_xt *)instr_node_px->msg_p;

		// grab the instrument group, for instr class search
		series_x.instrument_group_u = instr_data_px->instrument_group_u;

		// see if we have an instrument class for this underlying / instr group
		ise_instr_class_info_px = find_instrument_class_by_series ( country_market_px,
									    &series_x );
		if ( ise_instr_class_info_px == NULL )
		{
		    /* missing this instrument class
			This is OK, if say the underlying was purely defined by the ISE
			so that the broadcasts of prices for a component of a synthetic
			will occur e.g. at least one component of a synthetic is an underlying
			which the ISE does NOT trade options on, but at least one
			leg is/was traded by the ISE */
		}
		else
		{
		    // we have instrument class for this instrument group / underlying combo
		    // is this a traded instrument class ?
		    if (ise_instr_class_info_px->traded_u == 1)
		    {
			// yup, tradable. Have we saved info about a traded instr class for this under ?
			tradable_classes_u++;

			if ( under_data_px->traded_instr_class_px )
			{
			    // compare the instr class stuff from the 'saved' instr class
			    // NB this only does basic checks w.r.t tick table.. same #, and did we decide on same decimals/fractions
			    // it probably should check that the contents of the tick tables are identical
			    if (( under_data_px->traded_instr_class_px->bbo_volume_i != ise_instr_class_info_px->bbo_volume_i )
			    || ( under_data_px->traded_instr_class_px->contract_size_i != ise_instr_class_info_px->contract_size_i )
			    || ( under_data_px->traded_instr_class_px->dec_in_contr_size_u != ise_instr_class_info_px->dec_in_contr_size_u )
			    || ( under_data_px->traded_instr_class_px->dec_in_premium_u != ise_instr_class_info_px->dec_in_premium_u )
			    || ( under_data_px->traded_instr_class_px->dec_in_strike_price_u != ise_instr_class_info_px->dec_in_strike_price_u )
			    || ( under_data_px->traded_instr_class_px->rnt_id_u != ise_instr_class_info_px->rnt_id_u )
			    || ( under_data_px->traded_instr_class_px->tick_count_u != ise_instr_class_info_px->tick_count_u )
			    || ( under_data_px->traded_instr_class_px->trades_in_decimals_u != ise_instr_class_info_px->trades_in_decimals_u )
			    || ( under_data_px->traded_instr_class_px->block_order_min_size_i != ise_instr_class_info_px->block_order_min_size_i )
			    || ( under_data_px->traded_instr_class_px->block_order_rsptime_i != ise_instr_class_info_px->block_order_rsptime_i )
			    || ( under_data_px->traded_instr_class_px->fac_order_min_size_i != ise_instr_class_info_px->fac_order_min_size_i )
			    || ( under_data_px->traded_instr_class_px->fac_order_rsptime_i != ise_instr_class_info_px->fac_order_rsptime_i ))
			    printf ("Warning, instr classes for underlying %s with different settings. Use UNDER,<name>,DETAIL to examine\n",
					under_data_px->under_id_s );
				    
			}
			else
			{
			    // no, this is the first traded instr class for this under
			    // note that we are saving this as a shortcut for later lookups for (say) dec_in_premium_u
			    under_data_px->traded_instr_class_px = ise_instr_class_info_px;
			}   // test if we have saved pointed to another traded class for this underlying

			// have we saved info about any traded class for the market.. fallback option
			if ( click_px->our_market_x.traded_instr_class_px == NULL )
			    click_px->our_market_x.traded_instr_class_px = ise_instr_class_info_px;
		    }	// test if this instr class is traded
		    // ignore untraded instr class.. this should just be an avista
		}   // test for instrument class for this instrument group / underlying combo

		// try the next instrument - which may be NULL
		instr_node_px = (gen_buf_node_xt *)instr_node_px->chain.next;
	    }   // test if we had run out of instrument groups
	} while (TRUE);	// break, when we run out of instrument groups

	// before we kick onto the next underlying, did we get an even # of tradable instrument classes
	if ( (tradable_classes_u & 1) == 1)
	{
	    // oops, odd # of tradable classes
	    printf ( "warning, underlying %s, in country %s, market %s, has odd # (%u) of tradable classes\n",
			&under_data_px->under_id_s,
			exchange_to_string ( click_px,
						series_x.country_u ),
			&country_market_px->name_s,
			tradable_classes_u
		    );
	}

	// advance to next.. which may of course not exist...
	under_node_px = (gen_buf_node_xt *)under_node_px->chain.next;
    }	// while ( under_node_px != NULL )
}   // check_all_underlying_instr_classes

// ---------------------------------------------------------------------------

char * commodity_code_to_string ( click_details_xt * click_px,
				    uint16 search_under_code_u )
/*
Function:   commodity_code_to_string
Author:	    Neil Cossor
Modified:   991127
Description:

    We have a commodity code #, and with to have the underlying symbol (ID) for that.
    
    This just calls find_under_data_by_commodity, but returns just the
    pointer to the under_id_s

Input Params:

    pointer to all the stuff we know about the central system
    the commodity code we are looking for

Output Params:

    returns pointer to (short) string name if we find a match
    returns pointer to string "*" if the commodity code is 0
    returns pointer to (static) string "Unk under" if we don't find a match
*/
{
    // pointer to the ISE format data about one underlying, from the known_underlying_q
    ise_underlying_xt	* ise_under_info_px = NULL;

    static char *wildcard_under_s = "*";
    static char *unknown_under_s = "Unk under";


    if ( search_under_code_u == 0 )
	return wildcard_under_s;

    ise_under_info_px = find_under_data_by_commodity ( click_px, search_under_code_u );

    // did we find a match ?
    if ( ise_under_info_px != NULL )
    {
	// match.
	return ((char *)&ise_under_info_px->under_id_s );
    }	// test if bsearch found a match

    // oops, no match
    return unknown_under_s;
}   // commodity_code_to_string

// ---------------------------------------------------------------------------

ise_synthetic_xt * find_synthetic_by_commodity ( click_details_xt * click_px,
						    uint16 commodity_u )
/*
Function:   find_synthetic_by_commodity
Author:	    Neil Cossor
Date:	    20000114
Description:

    We wish to find a synthetic with a particular commodity code (of the synthetic).

    If we find a match, we return a pointer to the struct, otherwise return a NULL

    As there will typically be a very small (possible empty) list of synthetic,
    just do a linear search.

Input Params:

    - Pointer to everything we know about the CLICK system / trading environment
    - the commodity code of the synthetic we are looking for
*/
{
    // temp, ptr to Q'able block, with pointer to data about one synthetic, default NULL
    gen_buf_node_xt		* synthetic_node_px = NULL;
    // temp, ptr to an ISEsim block of data about one synthetic
    ise_synthetic_xt		* synthetic_data_px = NULL;

    // search for synthetic which has the nominated commodity code
    synthetic_node_px = (gen_buf_node_xt *)click_px->our_market_x.known_synthetic_q.chain.next;
    do	// while (TRUE)
    {
	// have we got a block of data, containing info about a synthetic
	if ( synthetic_node_px == NULL )
	{
	    // end of list, no match
	    return NULL;
	}
	else
	{
	    // point into the actual synthetic data
	    // this is NOT allowed to be NULL
	    synthetic_data_px = (ise_synthetic_xt *)synthetic_node_px->msg_p;

	    if ( synthetic_data_px->commodity_code_u == commodity_u )
	    {
		// found the match - just break out of the loop, and return ptr
		break;
	    }
	    else
	    {
		// no go, try the next synthetic - which may be NULL
		synthetic_node_px = (gen_buf_node_xt *)synthetic_node_px->chain.next;
	    }
	}
    } while (TRUE);

    // return our value to the caller, which may be NULL
    return synthetic_data_px;
}   // find_synthetic_by_commodity

// ---------------------------------------------------------------------------

void display_synthetics ( click_details_xt * click_px )
/*
Function:   display_synthetics
Author:	    Neil Cossor
Date:	    20000118
Description:

    The caller wishes to see the info on the known synthetics

    Display them on stdout
*/
{
    // temp, ptr to Q'able block, with pointer to data about one synthetic, default NULL
    gen_buf_node_xt		* synthetic_node_px = NULL;
    // temp, ptr to an ISEsim block of data about one synthetic
    ise_synthetic_xt		* synthetic_data_px = NULL;
    // item to loop thru the components of one synthetic
    unsigned			item_u;
    // ptr to one underlying which belongs to the synthetic
    ise_underlying_xt		*under_data_px = NULL;

    // search for synthetic which has the nominated commodity code
    synthetic_node_px = (gen_buf_node_xt *)click_px->our_market_x.known_synthetic_q.chain.next;
    while ( synthetic_node_px != NULL )
    {
	// point into the actual synthetic data
	// this is NOT allowed to be NULL
	synthetic_data_px = (ise_synthetic_xt *)synthetic_node_px->msg_p;

	// report on this one synthetic.
	// do we know the synthetic underlying for this ?
	if ( synthetic_data_px->synthetic_under_data_px != NULL )
	    printf ("Synthetic %s, cash amount %u, has %u items\n",
			synthetic_data_px->synthetic_under_data_px->under_id_s,
			synthetic_data_px->cash_amount_u,
			synthetic_data_px->underlying_count_u );
	else
	    printf ("Synthetic code %u (unknown underlying), cash amount %.2f, has %u item(s)\n",
		synthetic_data_px->commodity_code_u,
		((float)synthetic_data_px->cash_amount_u / 100),
		synthetic_data_px->underlying_count_u );


	for ( item_u = 0;
		item_u < synthetic_data_px->underlying_count_u;
		item_u++ )
	{
	    printf ( "    Underlying %s, %.2f shares\n",
			    synthetic_data_px->dependent_under_data_px [ item_u ]->under_id_s,
			    ((float) synthetic_data_px->number_of_shares_u [ item_u ] / 100)
		    );
	}   // for loop, reporting components of synthetics

	// advance to the next synthetic - which may be NULL
	synthetic_node_px = (gen_buf_node_xt *)synthetic_node_px->chain.next;
    }	// while ( synthetic_node_px != NULL )

    printf ("%u known synthetic(s)\n",
		click_px->our_market_x.known_synthetic_q.length_i );
}   // display_synthetics


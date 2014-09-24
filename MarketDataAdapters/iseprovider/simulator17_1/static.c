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


Title : static.c
Author: Neil Cossor
Function :

    This module provides facilities to collect all static data from
    the central system, to the ISESim.exe program.
    
Revision history :-

yymmdd	vers'n	author	    notes
------	------	------	    -----
9905xx	v0.00	Neil Cossor initial version
990913	v0.01		    mod query_static_underlying to use update_one_underlying (underlying.h/.c)
			    fix bug in query_instrument_classes.. referenced initialised item_index_u
990928			    fix bugs - we did NOT correctly halt on segment query, when returned a 0 segment #
			    remove silly complete_b flags - untidy C code
990929			    upgrade code processing da203 to extract tick_size info
			    upgrade query_markets (da207) to extract more info, and display to screen
000113	v0.02		    add new DQ212 Query Dependencies (i.e. synthetics)
000118			    add automatic call to query_static_synthetic
000122			    change sequence in load_static_data.. simplifies some internal updates
000210	v0.03		    fix bugs in query_static_synthetic, when adding new synthetic (',' instead of '=')
			    and not using PUTSHORT in saving commodity code. thanks Nick
000420	v0.04		    enhance query_static_synthetic to handle incomplete synthetic info better.
				If central system has top-level or component underlying of synthetic without
				any instrument classes, then we can get a synthetic in DQ212 with
				references to underyling which did not come along with DQ204.
000422	v1.00		    update to match OM release 15.7-3 (move of some fields from da203 to da210).
				query_instrument_groups, load_static_data (additional checks on da210)
000505	v1.01		    update query_static_synthetic.. cash amount should only be in 1st item, rest == 0
			    update load_static_data to call 

0012xx	v2.00		    start updates for OM release 16.3 (from 15.7-3)
			    simpler DQ207
			    new DQ245, session states
			    move from market status -> instrument status
010604	v2.01		    remove calc of default bin from query_bins_mms (now done in misc.c)

Known areas for additional work :-

*/

#include "stdincl.h"

#include "error.h"
#include "instrument.h"
#include "misc.h"
#include "series.h"
#include "static.h"
#include "underlying.h"
#include "utility.h"

// ----------------------- locally defined types etc -------------------------

static int32 send_series_query ( click_details_xt * click_px,
				series_t *series_px)
/*
Function:   send_series_query
Author :    Neil Cossor
Modified :  9905xx
Description:
    Sends a query transaction to find all static info about all series
    within a specified series qualifier. Uses dq202.
    Note that this series specifier allows us (among other things) to
    - ask for all series with a particular instrument group
    - ask for all series with a particular commodity

Input params:

    pointer to a "series", which qualifies the query.
    Ref 5.1.1 of ISE OM system Programmers Manual.
    Allowed choices are
    - all markets.. all zeroes in series
    - country # and market code
    - specific instrument type.

output params:

    -ve if error (status value)
    otherwise the # of series we retrived from the central system

implicit params:

    modifies the (module private) list of known series
    (which should have previously been empty)
*/
{
    // the query msg
    generic_txn_xt	OMmsg_x;
    // pointer to the response - the response is always in an rx buffer in click_px->
    answer_series_da202_t   * txn_reply_px;
    // pointer into the response - one series record
    da202_t		    * answer_series_px;
    // temp, ptr to data about one series, default NULL
    gen_buf_node_xt	    * series_data_px = NULL;
    // temp, ptr to ISE version of data about one series - destination record
    ise_series_info_xt	    * ise_series_info_px;

    int32		status_i;
    int32		ItemFound_i;
    uint32		tx_length_u;
    uint16		segment_u;
    uint16		item_index_u,	// local loop var
			item_limit_u;	// limit on loop - may be endian-swapped from response


    // do our static query based on whatever param we where given
    OMmsg_x.tx_x.query_series_x.series = *series_px;
    segment_u = 0;
    ItemFound_i = 0;

    // save the facility code and fn code
    OMmsg_x.fn_code = LOG_K_Q_SERIES_SEGMENTS;
    OMmsg_x.facility_u = click_px->facility_no_u;   // EP0/ET0


    // Multi-segment data - loop until error, or no more data
    while (TRUE)
    {
	segment_u++; // The segment # we wish to retrieve
	PUTSHORT( OMmsg_x.tx_x.query_series_x.segment_number_n, segment_u );
	tx_length_u = sizeof ( query_series_t );
	pack_OM_header ( &OMmsg_x, sizeof ( query_series_t ), 'D', 'Q', 202 );

	status_i = do_click_query_rx ( click_px,
					&OMmsg_x,
					tx_length_u,
					LOG_K_R_SERIES );

	if ( status_i < OMNIAPI_SUCCESS)
	{
	    printf( "< Series Query Failed >" );
	    return status_i;
	}
	else
	{
	    // all OK - point to interesting part of data in standard rx bufer
	    txn_reply_px = &click_px->log_reply_x.trans_reply_x.answer_series_x;

	    // get item count in correct endian order
	    PUTSHORT (item_limit_u, txn_reply_px->items_n );

	    if ( item_limit_u == 0 )
		break;

	    // note how many more we found - maybe 0. NB DON'T use the raw rx - endian issues
	    ItemFound_i += ( int32 ) item_limit_u;

	    /* look at the items */
	    for ( item_index_u = 0;
		item_index_u < item_limit_u;
		item_index_u++ )
	    {

		// allocate space to store info about one series record
		series_data_px = new_gen_buf_node (sizeof (ise_series_info_xt));
		// grab typecast copy of pointer into dest storage for series details
		(char *)ise_series_info_px = series_data_px->msg_p;
		// and point into source record about 1 series
		answer_series_px = &txn_reply_px->item[ item_index_u ].da202;

		update_one_series ( click_px,
				    ise_series_info_px,
				    answer_series_px );

		// and add to our chain of known series.
		add_to_q_tail ( &click_px->our_market_x.known_series_q, 
				&series_data_px->chain );
	    }	// for loop on one series in response

	    /* regardless of whether there were any items in it, if the segment # in the response
		is 0, then we got all the data there is. NB although this is endian, a binary
		0 is still a 0 - don't have to do a PUTSHORT
	    */
	    if ( txn_reply_px->segment_number_n == 0 )
		// no more data - out of here
		break;
	}   // test if success getting another segment of data
    }	// test if we have finished yet
    
    return ItemFound_i;
}   // send_series_query

// -------------------------------------------------------------------------------------

static int32 query_all_series ( click_details_xt * click_px )
/*
Function:   query_all_series
Author :    Neil Cossor
Modified :  9906xx
Description:

    Inquires on all series, using a call to send_series_query.
    
    We could get info on all series by querying instrument by instrument,
    but as we want to know about all of them anyway, this will probably be more
    efficient.

Input params:

    pointer to a structure which contains everything we know about this OM market.
    Ref 5.1.6 of ISE OM system Programmers Manual.

output params:

    -ve if error (status value)
    otherwise the # of series we retrieved from the central system

implicit params:

    modifies the (module private) list of known series
    (which should have previously been empty)
*/
{
    int32    status_i;
    series_t SeriesID_x;
    int8     Error_b = FALSE;


    Error_b = FALSE;

    // zot all qualifiers in the series - i.e. wildcard all
    memset( &SeriesID_x, 0, sizeof ( series_t ) );
    // specify the country and market
    SeriesID_x.country_c = click_px->our_market_x.country_u;
    SeriesID_x.market_c = click_px->our_market_x.market_u;

    status_i = send_series_query( click_px,
				    &SeriesID_x );
    if ( status_i < SUCCESSFUL )
    {
	Error_b = TRUE; 
    }
    else
    {
	printf ("Found %d Series (from %d Instrument Group%s)\n",
		 click_px->our_market_x.known_series_q.length_i, 
		 click_px->our_market_x.known_instruments_q.length_i, 
		 (click_px->our_market_x.known_instruments_q.length_i == 1) ? "" : "s" );
    }

    return (status_i);

}   // query_all_series

// -------------------------------------------------------------------------------------

static int32 query_brokers ( click_details_xt * click_px )
/*
    Load the information about Brokers, within our customer, and our country.
    This uses dq206.

    In ISE parlance, this loads info about operators within your broking firm (I think, NAC).

    NB this query takes customer and country as an input. The only query we can do is
    on our own brokers, within our customer. We got our customer ID and country in
    the omniapi_get_info, immediately after logoff. Use those values.
*/
{
    // status var, to return to caller
    int32		status_i;

    // CLICK-format request msg to make query
    generic_txn_xt	OMmsg_x;

    // multi-segment query, var used to specify which segment of data we want next
    uint16	    segment_u;
    uint16	    item_index_u,   // local loop var
		    item_limit_u;   // limit on loop - may be endian-swapped from response

    // size of our actual CLICK query - we seem to need this repetitively
    uint32	    tx_length_u;
    // ptr to the interesting part of the CLICK response
    answer_broker_da206_t	* txn_reply_px;



    tx_length_u = sizeof ( query_broker_t );
    segment_u = 0;
    pack_OM_header ( &OMmsg_x, tx_length_u, 'D', 'Q', 206 );
    // zot all subfields of the series qualifier for the query - we want all info
    memset(&OMmsg_x.tx_x.query_broker_x.series,0,sizeof(series_t));
    // specify the customer ID from info retrieved by omniapi_get_info earlier
    tx_click_string ( &OMmsg_x.tx_x.query_broker_x.ex_customer_s,
			&click_px->user_code_x.customer_id_s,
			sizeof ( OMmsg_x.tx_x.query_broker_x.ex_customer_s ));
    // and specify the country from the same omni_get_info
    tx_click_string ( &OMmsg_x.tx_x.query_broker_x.country_id_s,
			&click_px->user_code_x.country_id_s,
			sizeof ( OMmsg_x.tx_x.query_broker_x.country_id_s ));

    // save the facility code and fn code
    OMmsg_x.fn_code = LOG_K_Q_BROKER;
    OMmsg_x.facility_u = click_px->facility_no_u;   // EP0/ET0

    do	// while (TRUE)
    {
	// segment we are about to try to retrieve (NB must start from 1)
	segment_u++;
	PUTSHORT( OMmsg_x.tx_x.query_broker_x.segment_number_n, segment_u );

	// do the query/rx. Reports error if problem, returns SUCCESSFUL (0) if good
	status_i = do_click_query_rx ( click_px,
		    &OMmsg_x,
		    tx_length_u,
		    LOG_K_R_BROKER);
	if ( status_i < SUCCESSFUL )
	{
	    // problem - already reported. Return bad status (out of here)
	    return status_i;
	}
	else
	    {
	    // all OK. Point to the interesting bit of the answer.
	    txn_reply_px = &click_px->log_reply_x.trans_reply_x.answer_broker_x;

	    // we may NOT, however have rxed any data in this query
	    // in which case bail out of the loop
	    // grab the record count, in local endian order
	    PUTSHORT ( item_limit_u, txn_reply_px->items_n );
	    if ( item_limit_u == 0 )
		break;

	    // need some code here to do something with the info about
	    // the brokers(operators) we just got
	    for ( item_index_u = 0; 
	      item_index_u < item_limit_u; 
	      item_index_u++ )
	    {
		// ???? need some code here, doing something with data about each broker
	    }	// for loop, extracting entries from this buffer

	    /* regardless of whether there were any items in it, if the segment # in the response
		is 0, then we got all the data there is. NB although this is endian, a binary
		0 is still a 0 - don't have to do a PUTSHORT
	    */
	    if ( txn_reply_px->segment_number_n == 0 )
		// no more data - out of here
		break;
	}   // test from success getting a buffer of response
    }
    while ( TRUE );

    return status_i;
}   // query_brokers

// -------------------------------------------------------------------------------------

static int32 query_markets ( click_details_xt * click_px )
/*
Function:   query_markets
Author:	    Neil Cossor
Modified:   200012xx
Description:

    Load the information about known markets.
    This uses dq207.

    NB since release 16.3,the query contains little of use.. 
    more stuff comes from new DQ245, but more complex to interpret.
*/
{
    // status var, to return to caller
    int32	    status_i;

    // CLICK-format request msg to make query
    generic_txn_xt  OMmsg_x;

    // multi-segment query, var used to specify which segment of data we want next
    uint16	    segment_u;
    uint16	    item_index_u,   // local loop var
		    item_limit_u;   // limit on loop - may be endian-swapped from response

    // size of our actual CLICK query - we seem to need this repetitively
    uint32	    tx_length_u;
    // ptr to the interesting part of the CLICK response - info on one market
    da207_t   
		    * txn_reply_px;



    tx_length_u = sizeof ( query_market_t );
    segment_u = 0;
    pack_OM_header ( &OMmsg_x, tx_length_u, 'D', 'Q', 207 );
    // zot all subfields of the series qualifier for the query - we want all info
    memset(&OMmsg_x.tx_x.query_market_x.series,0,sizeof(series_t));

    // save the facility code and fn code
    OMmsg_x.fn_code = LOG_K_Q_SERIES_SEGMENTS;
    OMmsg_x.facility_u = click_px->facility_no_u;   // EP0/ET0

    do	// while (TRUE)
    {
	// segment we are about to try to retrieve (NB must start from 1)
	segment_u++;
	PUTSHORT( OMmsg_x.tx_x.query_market_x.segment_number_n, segment_u );

	// do the query/rx. Reports error if problem, returns SUCCESSFUL (0) if good
	status_i = do_click_query_rx ( click_px,
		    &OMmsg_x,
		    tx_length_u,
		    LOG_K_R_MARKET );
	if ( status_i < SUCCESSFUL )
	{
	    // problem - already reported. Return status, and bail out
	    return status_i;
	}
	else
	    {
	    // all OK.

	    // we may NOT, however have rxed any data in this query
	    // in which case bail out of the loop
	    // grab the record count, in local endian order
	    PUTSHORT ( item_limit_u, click_px->log_reply_x.trans_reply_x.answer_market_x.items_n );
	    if ( item_limit_u == 0 )
		break;

	    // save the info about the markets we just got.. at present, assume only one
	    for ( item_index_u = 0; 
	      item_index_u < item_limit_u; 
	      item_index_u++ )
	    {
		// extract data about each market
		// Point to the interesting bit of the answer.
		txn_reply_px = &click_px->log_reply_x.trans_reply_x.answer_market_x.item[item_index_u].da207;

		rx_click_string ( click_px->our_market_x.name_s,
				    txn_reply_px->name_s,
				    sizeof ( txn_reply_px->name_s ));
		// for now, we just grab the country and market code from last (== 1st) market
		  // and store that in our country/market structures
		click_px->our_market_x.country_u = txn_reply_px->country_c;
		click_px->our_market_x.market_u = txn_reply_px->market_c;

		/* as there are a limited # of markets (1 initially for ISE), just report
		    what we find onto the screen
		*/
		printf ("Market '%s'\n",
			    &click_px->our_market_x.name_s );
	    }	// for loop, extracting entries from this buffer

	    /* regardless of whether there were any items in it, if the segment # in the response
		is 0, then we got all the data there is. NB although this is endian, a binary
		0 is still a 0 - don't have to do a PUTSHORT
	    */
	    if ( click_px->log_reply_x.trans_reply_x.answer_market_x.segment_number_n == 0 )
		// no more data - out of here
		break;
	}   // test from success getting a buffer of response
    }
    while ( TRUE );

    return status_i;
}   // query_markets

// -------------------------------------------------------------------------------------

static int32 query_exchanges ( click_details_xt * click_px )
/*
    Load the information about known exchanges.
    This uses dq24.
*/
{
    // status var, to return to caller
    int32	    status_i;

    // CLICK-format request msg to make query
    generic_txn_xt  OMmsg_x;

    // multi-segment query, var used to specify which segment of data we want next
    uint16	    segment_u;
    uint16	    item_index_u,   // local loop var
		    item_limit_u;   // limit on loop - may be endian-swapped from response

    // size of our actual CLICK query - we seem to need this repetitively
    uint32	    tx_length_u;
    // ptr to the interesting part of the CLICK response
    da24_t	    * txn_reply_px;
    // pointer to the data stuff we hang onto about an exchange
    ise_exchange_xt *exchange_data_px;
    // pointer to the Q'able node, containing the ptr to data about one exchange
    gen_buf_node_xt *exchange_node_px;



    tx_length_u = sizeof ( query_exchange_dq24_t );
    segment_u = 0;
    pack_OM_header ( &OMmsg_x, tx_length_u, 'D', 'Q', 24 );
    // zot all subfields of the series qualifier for the query - we want all info
    memset(&OMmsg_x.tx_x.query_exchange_x.series,0,sizeof(series_t));

    // save the facility code and fn code
    OMmsg_x.fn_code = LOG_K_Q_SERIES_SEGMENTS;
    OMmsg_x.facility_u = click_px->facility_no_u;   // EP0/ET0

    do	// while (TRUE)
    {
	// segment we are about to try to retrieve (NB must start from 1)
	segment_u++;
	PUTSHORT( OMmsg_x.tx_x.query_exchange_x.segment_number_n, segment_u );

	// do the query/rx. Reports error if problem, returns SUCCESSFUL (0) if good
	status_i = do_click_query_rx ( click_px,
		    &OMmsg_x,
		    tx_length_u,
		    LOG_K_R_EXCHANGE );
	if ( status_i < SUCCESSFUL )
	{
	    // problem - already reported. Return status, and bail out
	    return status_i;
	}
	else
	    {
	    // all OK. Point to the interesting bit of the answer.

	    // we may NOT, however have rxed any data in this query
	    // in which case bail out of the loop
	    // grab the record count, in local endian order
	    PUTSHORT ( item_limit_u, click_px->log_reply_x.trans_reply_x.answer_exchange_x.items_n );
	    if ( item_limit_u == 0 )
		break;

	    // save the info about the exchanges we just got
	    for ( item_index_u = 0; 
	      item_index_u < item_limit_u; 
	      item_index_u++ )
	    {
		// need to save stuff about one more exchange - point into the interesting stuff
		txn_reply_px = &click_px->log_reply_x.trans_reply_x.answer_exchange_x.item [item_index_u].da24;

		// create Q'able node to save info, with substructure for actual data about one exchange
		exchange_node_px = new_gen_buf_node ( sizeof (ise_exchange_xt));
		// point to the 
		exchange_data_px = (ise_exchange_xt *)exchange_node_px->msg_p;

		// grab the fields from the OM response
		exchange_data_px->country_u = txn_reply_px->country_c;
		exchange_data_px->opra_indicator_u = txn_reply_px->opra_indicator_c;
		rx_click_string ( exchange_data_px->long_name_s,
				    txn_reply_px->name_s,
				    sizeof (txn_reply_px->name_s));
		rx_click_string ( exchange_data_px->short_name_s,
				    txn_reply_px->exchange_short_s,
				    sizeof (txn_reply_px->exchange_short_s));

		// save the data block in the chain of known exchanges
		add_to_q_tail ( &click_px->our_market_x.known_exchanges_q,
				&exchange_node_px->chain );
	    }	// for loop, extracting entries from this buffer

	    /* regardless of whether there were any items in it, if the segment # in the response
		is 0, then we got all the data there is. NB although this is endian, a binary
		0 is still a 0 - don't have to do a PUTSHORT
	    */
	    if ( click_px->log_reply_x.trans_reply_x.answer_exchange_x.segment_number_n == 0 )
		// no more data - out of here
		break;
	}   // test from success getting a buffer of response
    }
    while ( TRUE );

    printf ("Found %d exchanges\n",
		click_px->our_market_x.known_exchanges_q.length_i );
    return status_i;
}   // query_exchanges

// -------------------------------------------------------------------------------------

static int32 query_instrument_groups ( click_details_xt * click_px )
/*
Function : query_instrument_groups
Author	 : Neil Cossor
Modified : 20000422
Description:

    Loads all instruments into memory.
    Note that this assumes that the instrument structures have been allocated,
    in a (successful) previous call to query_instrument_names.
    We are just filling in the extra data about the instrument classes
    i.e. the numbers which are the attributes describing the instruments

    Uses dq203
*/
{
    generic_txn_xt	    OMmsg_x;
    // pointer to one instr type record in the response - the response is always in an rx buffer in click_px->
    da203_t		    * txn_reply_px;
    // temp, ptr to (internal info) ISE version of data about one instrument
    // - destination record where we put the results of the query
    instrument_xt	    * ise_instr_info_px;

    int32		    status_i;
    uint32		    tx_length_u;
    uint16		    segment_u;
    uint16		    item_index_u,   // local loop var
			    item_limit_u;   // how many data items we got in 1 query - endian
    uint16		    InstrIgnored_n;
    int32		    TxnAttempt_i;
    int8		    Error_b = FALSE;


    // specify nothing i.e. we want to know about ALL instruments
    memset( &OMmsg_x.tx_x.query_instrument_x.series, 0, sizeof ( series_t ) );

    // save the facility code and fn code
    OMmsg_x.fn_code = LOG_K_Q_SERIES_SEGMENTS;
    OMmsg_x.facility_u = click_px->facility_no_u;   // EP0/ET0

    segment_u = 0;
    InstrIgnored_n = 0;
    TxnAttempt_i = 0;

    do
    {
	segment_u++; // The segment # we wish to retrieve
	PUTSHORT ( OMmsg_x.tx_x.query_instrument_x.segment_number_n,
		    segment_u );
	// Size of the buffer to receive the reply
	tx_length_u = sizeof ( query_instrument_t );
	pack_OM_header ( &OMmsg_x, tx_length_u, 'D', 'Q', 203 );

	status_i = do_click_query_rx ( click_px,
					&OMmsg_x,
					tx_length_u,
					LOG_K_R_INSTRUMENTS );

	if ( status_i < SUCCESSFUL )
	{
	    printf ( "< Instrument Group Query Failed >" );
	    // return error, and bail out of loop
	    return status_i;
	}
	else
	{
	    // all OK 

	    // For each valid segment, there is >= 1 item(s) to retrieve

	    // get item count, 1-byte, so is in correct endian order
	    item_limit_u = click_px->log_reply_x.trans_reply_x.answer_instrument_x.items_c;
	    if ( item_limit_u != 0 )
	    {
		// Load the instrument items
		for ( item_index_u = 0; 
		      (item_index_u < item_limit_u); 
		      item_index_u++ )
		{
		    // point to interesting part of data in standard rx bufer
		    txn_reply_px = &click_px->log_reply_x.trans_reply_x.answer_instrument_x.item [item_index_u].da203;

		    // find the data block which relates to this instrument group
		    ise_instr_info_px = find_instr_group ( click_px,
							    txn_reply_px->series.instrument_group_c );
		    if (ise_instr_info_px != NULL)
		    {
			// we found the appropriate data block for this instrument group
			// now grab as much data about the instrument group as we want to keep

			// 1 byte fields - no endian issues
			ise_instr_info_px->market_u =
				txn_reply_px->series.market_c;
			PUTLONG ( ise_instr_info_px->bbo_volume_i,
				    txn_reply_px->bbo_volume_i );
			PUTLONG ( ise_instr_info_px->block_order_min_size_i,
				    txn_reply_px->block_order_min_size_i );
			PUTLONG ( ise_instr_info_px->block_order_rsptime_i,
				    txn_reply_px->block_order_rsptime_i );
		    }
		    else
		    {
			InstrIgnored_n++;
		    }	// test if we found the instr group corresponding to this instr type
		}   // for loop, extracting data about each instr type in the response
	    }
	    else
	    {
		// out of data - out of here
		break;
	    }	// test if we got any data records about instr types in the response

	    /* regardless of whether there were any items in it, if the segment # in the response
		is 0, then we got all the data there is. NB although this is endian, a binary
		0 is still a 0 - don't have to do a PUTSHORT
	    */
	    if ( click_px->log_reply_x.trans_reply_x.answer_instrument_x.segment_number_n == 0 )
		// no more data - out of here
		break;
	}	

    }
    while ( (item_limit_u != 0) 
	    && (status_i >= SUCCESSFUL)
	  );


    if (status_i >= SUCCESSFUL)
    {
	if ( InstrIgnored_n>0)
	{
	    printf ("%d Instrument%s Ignored.\n", 
		     InstrIgnored_n, 
		     ( InstrIgnored_n == 1 ) ? " Was" : "s Were" );
	}

	printf ("\n");
    }
    else
    {
	// do nothing - error reported earlier
    }

    return (status_i);
}   // query_instrument_groups

// -------------------------------------------------------------------------------------

/*
Function : query_instrument_names
Author	 : Neil Cossor
Modified : 9905xx
Description:

    Loads all instrument names. This uses dq208
*/
static int32 query_instrument_names ( click_details_xt * click_px )
{
    generic_txn_xt	      OMmsg_x;
    // pointer to the response - the response is always in an rx buffer in click_px->
    answer_instrument_group_da208_t
			    * txn_reply_px;
    // temp, ptr to (chained) data about one instrument, default NULL
    gen_buf_node_xt	    * instr_node_px = NULL;
    // temp, ptr to (internal info) ISE version of data about one instrument
    // - destination record where we put the results of the query
    instrument_xt	    * ise_instr_info_px;

    int32		    status_i;
    uint32		    tx_length_u;
    uint16		    segment_u;
    uint16		    item_index_u,   // local loop var
			    item_limit_u;   // how many data items we got in 1 query - endian;


    // zot query series - we want info on ALL instruments
    memset( &OMmsg_x.tx_x.query_instrument_group_x.series, 0, sizeof ( series_t ) );
    segment_u = 0;
    item_limit_u = 0;

    // save the facility code and fn code
    OMmsg_x.fn_code = LOG_K_Q_SERIES_SEGMENTS;
    OMmsg_x.facility_u = click_px->facility_no_u;   // EP0/ET0

    do
    {
	segment_u++;
	PUTSHORT ( OMmsg_x.tx_x.query_instrument_group_x.segment_number_n,
		    segment_u );
	tx_length_u = sizeof ( query_instrument_group_t );
	pack_OM_header ( &OMmsg_x, tx_length_u, 'D', 'Q', 208 );
 
	status_i = do_click_query_rx ( click_px,
					&OMmsg_x,
					tx_length_u,
					LOG_K_R_INSTRUMENT_GROUPS);

	if ( status_i < SUCCESSFUL )
	{
	    printf ( "< Instrument Query Failed >" );
	    // return status, and bail out of the loop
	    return status_i;
	}
	else
	{
	    // all OK - point to interesting part of data in standard rx bufer
	    txn_reply_px = &click_px->log_reply_x.trans_reply_x.answer_instrument_group_x;

	    // grab # if items returned - endian
	    PUTSHORT ( item_limit_u, txn_reply_px->items_n );
	    for ( item_index_u = 0; 
		  (item_index_u < item_limit_u ); 
		  item_index_u++ )
	    {
		// allocate space to store info about one instrument record
		instr_node_px = new_gen_buf_node (sizeof (instrument_xt));
		// grab typecast copy of pointer into dest storage for instrument details
		(char *)ise_instr_info_px = instr_node_px->msg_p;


		ise_instr_info_px->instrument_group_u =
			txn_reply_px->item[item_index_u].da208.instrument_group_c;
		
		rx_click_string(&ise_instr_info_px->instrname_s[0], 
				txn_reply_px->item[item_index_u].da208.name_s,
				sizeof (txn_reply_px->item[item_index_u].da208.name_s));

		// save the block of data about the instrument, and note one more in list
		// and add to our chain of known instrument groups.
		add_to_q_tail ( &click_px->our_market_x.known_instruments_q, 
				&instr_node_px->chain );
	    }	// for loop, extracting each exchange info

	    /* regardless of whether there were any items in it, if the segment # in the response
		is 0, then we got all the data there is. NB although this is endian, a binary
		0 is still a 0 - don't have to do a PUTSHORT
	    */
	    if ( txn_reply_px->segment_number_n == 0 )
		// no more data - out of here
		break;
	}   // check on status from query
    }	// loop until we didn't get any more info
    while ( (item_limit_u != 0)
	    && (status_i >= SUCCESSFUL)
	  );

    printf ("%d Instrument Groups were found.\n",
		click_px->our_market_x.known_instruments_q.length_i );

    return status_i;
}   // query_instrument_names

// -------------------------------------------------------------------------------------

/*
Function : query_instrument_classes
Author	 : Neil Cossor
Modified : 9905xx
Description:

    Loads all instrument classes.
    This uses dq210

    Don't know whether these can be down to the series...
    or to the underlying/instrument group.

    They appear to be (as expected) down to underlying/instrument
    group combinations BUT technically (at the message level) there
    is nothing stopping them being at the series level
*/
static int32 query_instrument_classes ( click_details_xt * click_px )
{
    generic_txn_xt	    OMmsg_x;
    // pointer to the one data record of response - the response is always in an rx buffer in click_px->
    da210_t		    * txn_reply_px;
    // temp, ptr to (chained) data about one instrument, default NULL
    gen_buf_node_xt	    * instr_node_px = NULL;
    // temp, ptr to (internal info) ISE version of data about one instrument
    // - destination record where we put the results of the query
    instrument_class_xt     * ise_instr_class_info_px;

    int32		    status_i;
    uint32		    tx_length_u = sizeof ( OMmsg_x.tx_x.query_instrument_class_x );
    uint16		    segment_u;
    uint16		    item_index_u,   // local loop var
			    item_limit_u;   // how many data items we got in 1 query - endian;


    // zot query series - we want info on ALL instruments
    memset( &OMmsg_x.tx_x.query_instrument_class_x.series, 0, tx_length_u );
    segment_u = 0;

    item_limit_u = 0;

    // save the facility code and fn code
    OMmsg_x.fn_code = LOG_K_Q_SERIES_SEGMENTS;	    // standard segmented, with series qualifier
    OMmsg_x.facility_u = click_px->facility_no_u;   // EP0/ET0

    do
    {
	segment_u++;
	PUTSHORT ( OMmsg_x.tx_x.query_instrument_class_x.segment_number_n,
		    segment_u );
	pack_OM_header ( &OMmsg_x, tx_length_u, 'D', 'Q', 210 );
 
	status_i = do_click_query_rx ( click_px,
					&OMmsg_x,
					tx_length_u,
					LOG_K_R_INSTRUMENT_CLASS);

	if ( status_i < SUCCESSFUL )
	{
	    printf ( "< Instrument Class Query Failed >" );
	    // return error status, and bail out of loop
	    return status_i;
	}
	else
	{
	    // all OK

	    // grab # if items returned - endian
	    PUTSHORT ( item_limit_u,
			click_px->log_reply_x.trans_reply_x
			    .answer_instrument_class_x.items_n );
	    for ( item_index_u = 0; 
		  (item_index_u < item_limit_u ); 
		  item_index_u++ )
	    {
		// point to interesting part of data in standard rx buffer
		txn_reply_px = &click_px->log_reply_x.trans_reply_x
				.answer_instrument_class_x.item [ item_index_u ].da210;

		// allocate space to store info about one instrument class record
		instr_node_px = new_gen_buf_node (sizeof (instrument_class_xt));
		// grab typecast copy of pointer into dest storage for instrument class details
		(char *)ise_instr_class_info_px = instr_node_px->msg_p;

		// fill in the contents of our ISEsim internal structure with info from msg
		update_one_instrument_class ( ise_instr_class_info_px,
						txn_reply_px );

		// save the block of data about the instrument, and note one more in list
		// and add to our chain of known instrument classes.
		add_to_q_tail ( &click_px->our_market_x.known_classes_q, 
				&instr_node_px->chain );
	    }	// for loop extracting data about one instrument class

	    /* regardless of whether there were any items in it, if the segment # in the response
		is 0, then we got all the data there is. NB although this is endian, a binary
		0 is still a 0 - don't have to do a PUTSHORT
	    */
	    if ( click_px->log_reply_x.trans_reply_x.answer_instrument_class_x.segment_number_n == 0 )
		// no more data - out of here
		break;
	}   // test for good status getting a block of instrument class data
    }	// loop, getting blocks until error or run out of data
    while ( (item_limit_u != 0)
	    && (status_i >= SUCCESSFUL)
	  );

    printf ( "%d Instrument Classes were found.\n",
		click_px->our_market_x.known_classes_q.length_i );

    return status_i;
}   // query_instrument_classes

// ---------------------------------------------------------------------------

static int32 query_bins_mms ( click_details_xt * click_px )
/*
Function : query_bins_mms
Author	 : Neil Cossor
Modified : 20010604
Description:

    Loads the bin/mm/pmm information.
    This uses dq211
*/
{
    generic_txn_xt	    OMmsg_x;
    // pointer to one data record of the response - the response is always in an rx buffer in click_px->
    da211_t		    *txn_reply_px;
    // pointer to one (chained) node, in the known_bins_q
    gen_buf_node_xt	    *bin_node_px = NULL;
    // pointer to the data component of the node in the known_bins_q
    ise_bin_xt		    *bin_data_px = NULL;

    int32		    status_i;
    uint32		    tx_length_u = sizeof ( OMmsg_x.tx_x.query_bin_x );
    uint16		    segment_u;
    uint16		    item_index_u,   // local loop var
			    item_limit_u;   // how many data items we got in 1 query - endian;


    // zot the query info - we want all info
    memset( &OMmsg_x.tx_x.query_bin_x, 0, tx_length_u );
    segment_u = 0;

    // save the facility code and fn code
    OMmsg_x.fn_code = LOG_K_Q_BIN_PMM_MM;
    OMmsg_x.facility_u = click_px->facility_no_u;   // EP0/ET0

    do
    {
	segment_u++;
	PUTSHORT ( OMmsg_x.tx_x.query_bin_x.segment_number_n,
		    segment_u );
	pack_OM_header ( &OMmsg_x, tx_length_u, 'D', 'Q', 211 );
 
	status_i = do_click_query_rx ( click_px,
					&OMmsg_x,
					tx_length_u,
					LOG_K_R_BIN_PMM_MM );

	if ( status_i < SUCCESSFUL )
	{
	    printf ( "< DQ211 query Bin/PMM/MM Query Failed >" );
	    // break out of here
	    return status_i;
	}
	else
	{
	    // all OK - process the records

	    // grab # if items returned - endian
	    PUTSHORT ( item_limit_u, click_px->log_reply_x.trans_reply_x.answer_bin_x.items_n );

	    // break out of the query loop, if we have run out of data
	    if ( item_limit_u == 0 )
		break;

	    for ( item_index_u = 0; 
		  (item_index_u < item_limit_u ); 
		  item_index_u++ )
	    {
		// point to interesting part of data in standard rx bufer
		txn_reply_px = &click_px->log_reply_x.trans_reply_x.answer_bin_x
				.item[item_index_u].da211;

		// we have NOT done anything with any previous bin record...
		// lets see if we know about this bin ?
		bin_node_px = find_bin ( &click_px->our_market_x,
					    txn_reply_px->bin_c );

		// if we now aren't pointing to a bin block, then we must create one
		if ( !bin_node_px )
		{
		    // we are creating a new bin record
		    bin_node_px = new_gen_buf_node ( sizeof ( ise_bin_xt ));
		    bin_data_px = (ise_bin_xt *)bin_node_px->msg_p;

		    // note the bin #
		    bin_data_px->bin_u = txn_reply_px->bin_c;

		    // add to Q
		    add_to_q_tail ( &click_px->our_market_x.known_bins_q,
				    &bin_node_px->chain );
		}   // did we know about this bin previously
		else
		{
		    // we now MUST be pointing at a bin record
		    bin_data_px = (ise_bin_xt *)bin_node_px->msg_p;
		}

		/* right... now we are definitely pointing to the correct bin info
		    Add the PMM/MM that we found in this response buffer record
		    Is it a PMM or MM we found ?
		*/
		switch ( txn_reply_px->cst_type_c )
		{
		case CUST_TYPE_PMM :
		    // we are adding the PMM to this bin definition
		    // make sure we don't already have a PMM set up
		    if ( strlen ( bin_data_px->pmm_id_s ) > 0)
		    {
			printf ("Warning, we are replacing the PMM %s for bin %d - multiple PMMs\n",
				    &bin_data_px->pmm_id_s,
				    bin_data_px->bin_u );
		    }

		    // store the PMM we have in this item
		    rx_click_string ( bin_data_px->pmm_id_s,
					txn_reply_px->ex_customer_s,
					sizeof (txn_reply_px->ex_customer_s));

		    break;
		case CUST_TYPE_MM :
		    //we are adding another MM to this bin definition
		    if ( bin_data_px->mms_u > MAX_MMS_IN_BIN )
		    {
			printf ("Error : too many MMs in bin %d, max value %d\n",
				    bin_data_px->bin_u,
				    (MAX_MMS_IN_BIN ));
		    }
		    else
		    {
			// should probably check for duplicates, like we do for PMMs
			rx_click_string ( bin_data_px->mm_ids_s [bin_data_px->mms_u],
					    txn_reply_px->ex_customer_s,
					    sizeof ( txn_reply_px->ex_customer_s ));

			// note one more stored
			bin_data_px->mms_u++;
		    }
		    break;

		default:
		    printf ( "Error : in da211, received illegal customer type %d, for bin %d\n",
				txn_reply_px->cst_type_c,
				bin_data_px->bin_u );
		}   // switch on customer type
	    }	// for loop, handling component records of a response buffer

	    /* regardless of whether there were any items in it, if the segment # in the response
		is 0, then we got all the data there is. NB although this is endian, a binary
		0 is still a 0 - don't have to do a PUTSHORT
	    */
	    if ( click_px->log_reply_x.trans_reply_x.answer_bin_x.segment_number_n == 0 )
		// no more data - out of here
		break;
	}   // test for success in getting a block of response data
    }
    while (status_i >= SUCCESSFUL);

    return status_i;
}   // query_bins_mms

// ---------------------------------------------------------------------------

int32 query_static_underlying ( click_details_xt * click_px )
/*
    Queries and loads underlying stock names and their corresponding 
    commodity numbers and creates/puts them into an unsorted list,
    for the nominated country and market.
    
    Uses dq204.
*/
{
    // CLICK-format request msg to make query - on the stack - no (de)allocation req'd
    generic_txn_xt	OMmsg_x;

    // multi-segment query, var used to specify which segment of data we want next
    uint16		segment_u;
    uint16		item_index_u,	// local loop var
			item_limit_u;	// limit on loop - may be endian-swapped from response

    // size of our actual CLICK query - we seem to need this repetitively
    uint32		tx_length_u;

    // temp, ptr to data about one underlying, default NULL
    gen_buf_node_xt		* underlying_px = NULL;
    // temp, ptr to an ISEsim block of data about one underlying
    ise_underlying_xt		* ise_underlying_px;
    // ptr to the interesting part of the CLICK response - data about 1 underlying
    da204_t			* txn_reply_px;


    tx_length_u = sizeof ( query_underlying_t );
    segment_u = 0;
    pack_OM_header ( &OMmsg_x, tx_length_u, 'D', 'Q', 204 );
    // zot the series specifier
    memset(&OMmsg_x.tx_x.query_underlying_x.series,0,sizeof(series_t));
    // set the country
    OMmsg_x.tx_x.query_underlying_x.series.country_c = click_px->our_market_x.country_u;
    // and the market
    OMmsg_x.tx_x.query_underlying_x.series.market_c = click_px->our_market_x.market_u;

    // save the facility code and fn code
    OMmsg_x.fn_code = LOG_K_Q_SERIES_SEGMENTS;
    OMmsg_x.facility_u = click_px->facility_no_u;   // EP0/ET0

    // debug
    do
    {
	// segment we are about to try to retrieve (NB must start from 1)
	segment_u++;
	PUTSHORT ( OMmsg_x.tx_x.query_underlying_x.segment_number_n, 
		    segment_u );

	// do the query/rx. Reports error if problem, returns 0 if good
	if ( do_click_query_rx ( click_px,
				&OMmsg_x,
				tx_length_u,
				LOG_K_R_UNDERLYING ) == 0 )
	{
	    // all OK - got a response - may be empty

	    // grab item count of underlying, 1 byte, so is in correct endian order
	    item_limit_u = click_px->log_reply_x.trans_reply_x.answer_underlying_x.items_c;

	    // got success from call - we may NOT, however have rxed any data in this query
	    // in which case bail out of the loop
	    if ( item_limit_u == 0 )
		break;

	    for ( item_index_u = 0; 
	      item_index_u < item_limit_u; 
	      item_index_u++ )
	    {
		//  Point to the interesting bit of the answer - data about one underlying
		txn_reply_px = &click_px->log_reply_x.trans_reply_x
					.answer_underlying_x.item [ item_index_u ].da204;

		// allocate space to store info about one underlying - control part first
		underlying_px = new_gen_buf_node (sizeof(ise_underlying_xt));
		// allocate enuff space for the the actual data
		(char *)ise_underlying_px = underlying_px->msg_p;

		// fill in the contents of our ISEsim internal structure with info from msg
		update_one_underlying ( click_px,
					ise_underlying_px,
					txn_reply_px );

		// NB, the default binary 0'ing of the structure leaves floating-point values
		// in our autoquote settings
		ise_underlying_px->interest_f = 0.0;
		ise_underlying_px->volatility_f = 0.0;

		// we now know everything (static) about this underlying,
		// for this country / market

		// and add to our chain of known underlyings.
		add_to_q_tail ( &click_px->our_market_x.known_underlying_q,
				&underlying_px->chain );
	    }	// for loop, extracting entries from this buffer

	    /* regardless of whether there were any items in it, if the segment # in the response
		is 0, then we got all the data there is. NB although this is endian, a binary
		0 is still a 0 - don't have to do a PUTSHORT
	    */
	    if ( click_px->log_reply_x.trans_reply_x.answer_underlying_x.segment_number_n == 0 )
		// no more data - out of here
		break;
	}
	else
	{
	    // an error occurred. bail out of the loop
	    break;
	}   // test from success getting a buffer of response
    }
    while ( TRUE );

    // debug/info - report how many we found
    printf ("%d CLICK Underlying were found.\n",
		click_px->our_market_x.known_underlying_q.length_i );

    return 0;
}   // query_static_underlying

// ---------------------------------------------------------------------------

int32 query_static_synthetic ( click_details_xt * click_px )
/*
Function:   query_static_synthetic
Author:	    Neil Cossor
Date:	    20000505
Description:

    Queries and loads synthetic dependencies.
    
    Uses dq212.

    The response data contains a list of all dependency items.
    Note, however, that each dependency item merely ties one component
    of an actual underlying to the synthetic. A normal synthetic contains
    two or more dependent underlying, possibly with a cash offset.

    Technically (i.e. at the message level), it is possible to have a synthetic
    with one component, which just has a cash offset... but I can't see how
    this could ever occur in the real world.

    I presume that the commodity code of the synthetic are mutually exclusive
    with the commodity codes of the actual underlying...

    Note that at this time, we are lacking some programming information
    about the response data.
    - is there a max number of components for one synthetic, and if so, 
    what is that limit ?
    - if there is more than one synthetic in the system, and hence there
    are components relating to multiple components in the response,
    are the components of each synthetic packed in adjacent items ?
    - assuming the above is true, and there are many synthetic / components,
    if the adjacent items of one synthetic won't fit into a response, will they
    be split across the response segments ? I.e. when we get to the end of
    a segment, are we guaranteed to have finished with the current synthetic,
    or should we continue adding components until we hit components
    for a different synthetic.

Assumptions:

    Requires that we have previously loaded the normal underlying (query_static_underlying),
    and we have run our internal sort stuff.. so we can search for existing commodities / underyling
*/
{
    // CLICK-format request msg to make query - on the stack - no (de)allocation req'd
    generic_txn_xt	OMmsg_x;

    // multi-segment query, var used to specify which segment of data we want next
    uint16		segment_u;
    uint16		item_index_u,	// local loop var
			item_limit_u;	// limit on loop - may be endian-swapped from response

    // size of our actual CLICK query - we seem to need this repetitively
    uint32		tx_length_u;
    // tmp - the value of the cash amount, in local endian order
    uint32		cash_amount_u;

    // temp, ptr to Q'able block, with pointer to data about one synthetic, default NULL
    gen_buf_node_xt		* synthetic_node_px = NULL;
    // temp, ptr to an ISEsim block of data about one synthetic
    ise_synthetic_xt		* synthetic_data_px;
    // temp, ptr to an ISEsim block of data about one underlying (one for dependent component, one for synthetic itself)
    ise_underlying_xt		* under_data_px, *synthetic_under_data_px;
    // ptr to the interesting part of the CLICK response - data about 1 dependency entry
    da212_t			* txn_reply_px;
    // temp commodity.. in local byte order
    uint16	    tmp_commodity_u;


    tx_length_u = sizeof ( query_underlying_t );
    segment_u = 0;
    pack_OM_header ( &OMmsg_x, tx_length_u, 'D', 'Q', 212 );
    // zot the series specifier
    memset(&OMmsg_x.tx_x.query_underlying_x.series,0,sizeof(series_t));
    // set the country
    OMmsg_x.tx_x.query_underlying_x.series.country_c = click_px->our_market_x.country_u;
    // and the market
    OMmsg_x.tx_x.query_underlying_x.series.market_c = click_px->our_market_x.market_u;

    // save the facility code and fn code
    OMmsg_x.fn_code = LOG_K_Q_SERIES_SEGMENTS;
    OMmsg_x.facility_u = click_px->facility_no_u;   // EP0/ET0

    // debug
    do
    {
	// segment we are about to try to retrieve (NB must start from 1)
	segment_u++;
	PUTSHORT ( OMmsg_x.tx_x.query_underlying_x.segment_number_n, 
		    segment_u );

	// do the query/rx. Reports error if problem, returns 0 if good
	if ( do_click_query_rx ( click_px,
				&OMmsg_x,
				tx_length_u,
				LOG_K_R_DEPENDENCIES) == 0 )
	{
	    // all OK - got a response - may be empty

	    // grab item count of underlying, 2 byte, so endian order
	    PUTSHORT ( item_limit_u, 
			click_px->log_reply_x.trans_reply_x.answer_synthetic_x.items_n );

	    // got success from call - we may NOT, however have rxed any data in this query
	    // in which case bail out of the loop
	    if ( item_limit_u == 0 )
		break;

	    for ( item_index_u = 0; 
	      item_index_u < item_limit_u; 
	      item_index_u++ )
	    {
		//  Point to the interesting bit of the answer - data about one synthetic component
		txn_reply_px = &click_px->log_reply_x.trans_reply_x
					.answer_synthetic_x.item [ item_index_u ].da212;
		// find the underlying which this dependency refers to
		PUTSHORT ( tmp_commodity_u, txn_reply_px->commodity_n );
		synthetic_under_data_px = find_under_data_by_commodity ( click_px,
									    tmp_commodity_u );

		if (synthetic_under_data_px == NULL)
		{
		    // we have an synthetic which points to an underlying which we don't know about
		    printf ( "Warning, synthetic (commodity code %u) refers to unknown underlying\n",
				tmp_commodity_u );
		}
		else
		{

		    // find the underlying which this dependency refers to
		    PUTSHORT ( tmp_commodity_u, txn_reply_px->depends_on_commodity_n );
		    under_data_px = find_under_data_by_commodity ( click_px,
								    tmp_commodity_u );
		    if (under_data_px == NULL)
		    {
			// we have an synthetic which depends on an underlying which we don't know about
			printf ( "Warning, synthetic (commodity code %u) depends on unknown underlying (commodity code %u)\n",
				    synthetic_under_data_px->commodity_code_u,
				    tmp_commodity_u );
		    }   // test if we are dependent on a unknown underlying
		    else
		    {

			// do we already know anything about this synthetic ?
			synthetic_data_px = find_synthetic_by_commodity ( click_px,
									    txn_reply_px->commodity_n );
			if ( synthetic_data_px == NULL )
			{
			    // new synthetic.. create an entry
			    // allocate space to store info about one synthetic - control part first
			    synthetic_node_px = new_gen_buf_node (sizeof(ise_synthetic_xt));
			    // point to the actual data
			    (char *)synthetic_data_px = synthetic_node_px->msg_p;

			    // save the synthetic
			    PUTSHORT ( synthetic_data_px->commodity_code_u,
					txn_reply_px->commodity_n );
			    // and the pointer to the 'underlying' this corresponds to
			    synthetic_data_px->synthetic_under_data_px = synthetic_under_data_px;
			    // note the first dependency
			    synthetic_data_px->dependent_under_data_px [0] = under_data_px;
			    PUTLONG ( synthetic_data_px->number_of_shares_u [ 0 ], txn_reply_px->number_of_share_i );
			    synthetic_data_px->underlying_count_u = 1;

			    // save the cash amount offset - only the first item should contain a cash amount
			    PUTLONG ( synthetic_data_px->cash_amount_u, txn_reply_px->cash_amount_i );

			    // and add to our chain of known synthetics
			    add_to_q_tail ( &click_px->our_market_x.known_synthetic_q,
					    &synthetic_node_px->chain );
			}
			else
			{
			    // just need to add this new underlying to the list for the known synthetic
			    synthetic_data_px->dependent_under_data_px [synthetic_data_px->underlying_count_u] = under_data_px;
			    synthetic_data_px->number_of_shares_u [synthetic_data_px->underlying_count_u]= txn_reply_px->number_of_share_i;

			    synthetic_data_px->underlying_count_u += 1;
			    // confirm that the cash specified in this item matches what we already had
			    PUTLONG ( cash_amount_u, txn_reply_px->cash_amount_i );
			    if (( cash_amount_u != synthetic_data_px->cash_amount_u )
				&& ( cash_amount_u != 0 ))
			    {
				// mismatch in cash amount in successive records
				printf ("Mismatch in cash amount for synthetic %s, old %u, new %u\n",
					    synthetic_under_data_px->under_id_s,
					    synthetic_data_px->cash_amount_u,
					    cash_amount_u );
			    }
			}   // test if we were adding components to known synthetic, or a new synthetic
		    }	// test if known underlying for component
		}   // test if known underlying for top-level synthetic
	    }	// for loop, extracting entries from this buffer

	    /* regardless of whether there were any items in it, if the segment # in the response
		is 0, then we got all the data there is. NB although this is endian, a binary
		0 is still a 0 - don't have to do a PUTSHORT
	    */
	    if ( click_px->log_reply_x.trans_reply_x.answer_synthetic_x.segment_number_n == 0 )
		// no more data - out of here
		break;
	}
	else
	{
	    // an error occurred. bail out of the loop
	    break;
	}   // test from success getting a buffer of response
    }
    while ( TRUE );

    // debug/info - report how many we found
    printf ("%d good synthetic(s) found.\n",
		click_px->our_market_x.known_synthetic_q.length_i );

    return 0;
}   // query_static_synthetic

// ---------------------------------------------------------------------------

int32 query_trading_states ( click_details_xt * click_px )
/*
Function:   query_trading_states
Author:	    Neil Cossor
Modified:   200101xx
Description:

    Queries and loads the defined trading states, from the CDB.
    This is new for the ISE in 16.3, and supercedes the older simplisitic
    'Market Open' / 'Market Close', with all sorts of by-transaction time controls.
    The data we retrieve here is required for understanding the results of
    the UQ15 query, and the BI41 broadcast.

    Uses dq245.
*/
{
    // CLICK-format request msg to make query - on the stack - no (de)allocation req'd
    generic_txn_xt	OMmsg_x;

    // multi-segment query, var used to specify which segment of data we want next
    uint16		segment_u;
    uint16		item_index_u,	// local loop var
			item_limit_u;	// limit on loop - may be endian-swapped from response

    // size of our actual CLICK query - we seem to need this repetitively
    uint32		tx_length_u;

    // temp, ptr to data about one trading state, default NULL
    gen_buf_node_xt			* trading_state_node_px = NULL;
    // temp, ptr to an ISEsim block of data about one trading state
    ise_trading_state_xt		* trading_state_data_px;
    // ptr to the interesting part of the CLICK response - data about 1 trading state
    answer_trading_state_da245_item_t	* txn_reply_px;


    tx_length_u = sizeof ( query_trading_state_dq245_t );
    segment_u = 0;
    pack_OM_header ( &OMmsg_x, tx_length_u, 'D', 'Q', 245 );
    // zot the series specifier
    memset(&OMmsg_x.tx_x.query_trading_states_x.series,0,sizeof(series_t));
    // set the country
    OMmsg_x.tx_x.query_trading_states_x.series.country_c = click_px->our_market_x.country_u;
    // and the market
    OMmsg_x.tx_x.query_trading_states_x.series.market_c = click_px->our_market_x.market_u;

    // save the facility code and fn code
    OMmsg_x.fn_code = LOG_K_Q_SERIES_SEGMENTS;
    OMmsg_x.facility_u = click_px->facility_no_u;   // EP0/ET0

    // debug
    do
    {
	// segment we are about to try to retrieve (NB must start from 1)
	segment_u++;
	PUTSHORT ( OMmsg_x.tx_x.query_trading_states_x.segment_number_n, 
		    segment_u );

	// do the query/rx. Reports error if problem, returns 0 if good
	if ( do_click_query_rx ( click_px,
				&OMmsg_x,
				tx_length_u,
				LOG_K_R_TRADING_STATES ) == 0 )
	{
	    // all OK - got a response - may be empty

	    // grab item count of trading states
	    PUTSHORT ( item_limit_u,
			click_px->log_reply_x.trans_reply_x.answer_trading_states_x.items_n );

	    // got success from call - we may NOT, however have rxed any data in this query
	    // in which case bail out of the loop
	    if ( item_limit_u == 0 )
		break;

	    for ( item_index_u = 0; 
	      item_index_u < item_limit_u; 
	      item_index_u++ )
	    {
		//  Point to the interesting bit of the answer - data about one underlying
		txn_reply_px = &click_px->log_reply_x.trans_reply_x
					.answer_trading_states_x.item [ item_index_u ];

		// allocate space to store info about one underlying - control part first
		trading_state_node_px = new_gen_buf_node (sizeof(ise_trading_state_xt));
		// point to the actual data part of this chainable buffer entry
		(char *)trading_state_data_px = trading_state_node_px->msg_p;

		// grab the info from the item
		// the string name of the state
		rx_click_string ( trading_state_data_px->state_id_s,
				    txn_reply_px->state_name_s,
				    sizeof (txn_reply_px->state_name_s));
		// the state #.. which is the key between this data, and the UQ15 and BI41
		PUTSHORT ( trading_state_data_px->state_number_u,
			    txn_reply_px->state_number_n );
		// the fast-market level relevant to this state
		PUTSHORT ( trading_state_data_px->fast_market_level_u,
			    txn_reply_px->fast_market_level_n );
		// and all of the flags / attributes of this trading state
		trading_state_data_px->type_of_matching_u = txn_reply_px->type_of_matching_c;
		trading_state_data_px->trading_end_u = txn_reply_px->trading_end_c;
		trading_state_data_px->price_quotation_required_u = txn_reply_px->price_quotation_required_c;
		trading_state_data_px->market_orders_allowed_u = txn_reply_px->market_orders_allowed_c;
		trading_state_data_px->fill_kill_allowed_u = txn_reply_px->fill_or_kill_allowed_c;
		trading_state_data_px->edited_ob_changes_avail_u = txn_reply_px->edited_ob_changes_avail_c;
		trading_state_data_px->ob_changes_avail_u = txn_reply_px->ob_changes_avail_c;

		// we now know everything (static) about this trading state,
		// for this country / market

		// and add to our chain of known trading states.
		add_to_q_tail ( &click_px->our_market_x.known_trading_states_q,
				&trading_state_node_px->chain );
	    }	// for loop, extracting entries from this buffer

	    /* regardless of whether there were any items in it, if the segment # in the response
		is 0, then we got all the data there is. NB although this is endian, a binary
		0 is still a 0 - don't have to do a PUTSHORT
	    */
	    if ( click_px->log_reply_x.trans_reply_x.answer_trading_states_x.segment_number_n == 0 )
		// no more data - out of here
		break;
	}
	else
	{
	    // an error occurred. bail out of the loop
	    break;
	}   // test from success getting a buffer of response
    }
    while ( TRUE );

    // debug/info - report how many we found
    printf ("%d Trading States were found.\n",
		click_px->our_market_x.known_trading_states_q.length_i );

    return 0;
}   // query_trading_states

// -------------------------------------------------------------------------------------

int32 query_user_type ( click_details_xt * click_px )
/*
Function:   query_user_type
Author:	    Neil Cossor
Modified:   20010706
Description:

    Load the information about our user ??
    This uses dq30

    This returns information about the list of transactions, queries
    and broadcasts which are enabled for our particular user.

    The data is displayable with the WHOAMI command
*/
{
#define NON_TRADER	    2	// 1 in is_trader_c means they are a trader
#define INT_EXT_INTERNAL    2	// 1 in ext_or_int_c is external
#define TRANS_BCAST_BCAST   2	// 1 is transaction/query

    // status var, to return to caller
    int32	    status_i;

    // CLICK-format request msg to make query
    generic_txn_xt  OMmsg_x;

    // multi-segment query, var used to specify which segment of data we want next
    uint16	    segment_u;
    uint16	    item_index_u,   // local loop var
		    item_limit_u;   // limit on loop - may be endian-swapped from response

    // size of our actual CLICK query - we seem to need this repetitively
    uint32	    tx_length_u;
    // ptr to the interesting part of the CLICK response
    answer_user_type_info_item_t	    * txn_reply_px;
    // local endian copy of the transaction number
    int temp_transaction_no_u;



    tx_length_u = sizeof ( query_user_type_info_t );
    segment_u = 0;
    pack_OM_header ( &OMmsg_x, tx_length_u, 'D', 'Q', 30 );
    // zot all subfields of the series qualifier for the query - we want all info
    memset(&OMmsg_x.tx_x.query_user_type_x.series,0,sizeof(series_t));

    // save the facility code and fn code
    OMmsg_x.fn_code = LOG_K_Q_SERIES_SEGMENTS;
    OMmsg_x.facility_u = click_px->facility_no_u;   // EP0/ET0

    do	// while (TRUE)
    {
	// segment we are about to try to retrieve (NB must start from 1)
	segment_u++;
	PUTSHORT( OMmsg_x.tx_x.query_user_type_x.segment_number_n, segment_u );

	// do the query/rx. Reports error if problem, returns SUCCESSFUL (0) if good
	status_i = do_click_query_rx ( click_px,
		    &OMmsg_x,
		    tx_length_u,
		    LOG_K_R_USER_TYPE );
	if ( status_i < SUCCESSFUL )
	{
	    // problem - already reported. Return status, and bail out
	    return status_i;
	}
	else
	    {
	    // all OK.

	    /* If there is more than 1 segment of data, I presume that the
		data in the header is repeated in the follow-on segments.
		If not, then we should clear the values we have saved before the
		first query, and only update our saved value if we find a data
		value in a segment. That would cover the header data being in
		any segment, repeated or not
	    */


	    // grab the name of the user type.. typically something like "PMM" or "EAM" or "MM" (== CMM)
	    rx_click_string ( click_px->user_type_name_s,
				click_px->log_reply_x.trans_reply_x.answer_user_type_x.ust_id_s,
				sizeof (click_px->log_reply_x.trans_reply_x.answer_user_type_x.ust_id_s));
	    // are we considered an internal or external user. Most API users are external.
	    click_px->internal_user_u = ( click_px->log_reply_x.trans_reply_x.answer_user_type_x.ext_or_int_c
					== INT_EXT_INTERNAL );
	    // are we are trader or not. Most API users are traders.
	    click_px->non_trader_u = ( click_px->log_reply_x.trans_reply_x.answer_user_type_x.is_trader_c
					== NON_TRADER );

	    
	    // Point to the itemised bit of the answer.

	    // we may NOT, however have rxed any itemised data in this query
	    // in which case bail out of the loop
	    // grab the record count, in local endian order
	    PUTSHORT ( item_limit_u, click_px->log_reply_x.trans_reply_x.answer_user_type_x.items_n );
	    if ( item_limit_u == 0 )
		break;

	    // save the info we just got
	    for ( item_index_u = 0; 
	      item_index_u < item_limit_u; 
	      item_index_u++ )
	    {
		// need to save stuff about one more exchange - point into the interesting stuff
		txn_reply_px = &click_px->log_reply_x.trans_reply_x.answer_user_type_x.item [item_index_u];

		// pull out the list of transactions / queries / broadcasts we can use
		PUTSHORT ( temp_transaction_no_u,
			    txn_reply_px->transaction_type.transaction_number_n );

		if (click_px->trans_query_bcast_u >= MAX_TRANS_QUERY_BCAST )
		{
		    // we have a full list, just spit out this one on the console
		    printf ( "trans/query/bcast table full, enabled for %c%c%u\n",
				txn_reply_px->transaction_type.server_type_c,
				txn_reply_px->transaction_type.central_module_c,
				temp_transaction_no_u );
		}
		else
		{
		    // room for another - save the data
		    click_px->ise_trans_query_bcast_x [click_px->trans_query_bcast_u].transaction_type.server_type_c
			= txn_reply_px->transaction_type.server_type_c;
		    click_px->ise_trans_query_bcast_x [click_px->trans_query_bcast_u].transaction_type.central_module_c
			= txn_reply_px->transaction_type.central_module_c;
		    click_px->ise_trans_query_bcast_x [click_px->trans_query_bcast_u].transaction_type.transaction_number_n
			= temp_transaction_no_u;
		    // is this a bcast ?
		    click_px->ise_trans_query_bcast_x [click_px->trans_query_bcast_u].is_bcast_u = 
			( txn_reply_px->trans_or_bdx_c == TRANS_BCAST_BCAST );
		    click_px->trans_query_bcast_u++;
		}
	    }	// for loop, extracting entries from this buffer

	    /* regardless of whether there were any items in it, if the segment # in the response
		is 0, then we got all the data there is. NB although this is endian, a binary
		0 is still a 0 - don't have to do a PUTSHORT
	    */
	    if ( click_px->log_reply_x.trans_reply_x.answer_user_type_x.segment_number_n == 0 )
		// no more data - out of here
		break;
	}   // test from success getting a buffer of response
    }
    while ( TRUE );

    printf ("Permissioned for %u message types\n",
		click_px->trans_query_bcast_u );

    return status_i;
}   // query_user_type

// ---------------------- externally accessible routines -----------------------

int32 load_static_data ( click_details_xt * click_px )
/*
Function : load_static_data
Author   : Neil Cossor
Modified : 200012xx
Description:

    Loads the standing data for the user
*/
{
    int32 status_i	= SUCCESSFUL;
    unsigned		item_index_u;

    /* OM can be configured for authority setting for inidividual msgs
	for a user, preventing them from loading some parts of the static data
	set. In the practical case, all users authorised to use the API
	(as distinct from the Trader Workstation) will be authorised
	for all static data queries, so we can ignore the problem.
     */
    status_i = query_instrument_names ( click_px );
    if ( status_i < SUCCESSFUL)
    {
	// do nothing - error status is already in status_i
	return status_i;
    }

    if ( (status_i = query_instrument_groups ( click_px )) < SUCCESSFUL)
    {
	// do nothing - error status is already in status_i
	return status_i;
    }

    // OK so far, try to info about known markets.. NB we need these BEFORE instr classes
    if ( (status_i = query_markets (click_px) ) < SUCCESSFUL )
    {
	// do nothing - error status is already in status_i
	return status_i;
    }

    // OK so far, try to info about known exchanges
    if ( (status_i = query_exchanges (click_px) ) < SUCCESSFUL )
    {
	// do nothing - error status is already in status_i
	return status_i;
    }

    /* OK so far, try to get info about Trading State (new for 16.3)
	which combined with UQ15 and BI41 tells about what the state of
	play for whats in pre-opening (not tradable) etc
    */
    if ( (status_i = query_trading_states (click_px) ) < SUCCESSFUL )
    {
	// do nothing - error status already in status_i
    }

    // OK so far, try to collect info about underlying
	/* Note that it is a pre-requisite of series being loaded
	    that instruments are loaded, as it is from the instruments
	    that we decide which series to load. This is NOT a true
	    requirement, however the particular load we have chosen
	    to demonstrate is based on instruments being loaded first.
	 */
    if ( (status_i = query_static_underlying( click_px)) < SUCCESSFUL)
    {
	// do nothing - error status is already in status_i
	return status_i;
    }

    // load and sort the links for underlyings
    sort_underlying_tables ( &click_px->our_market_x, FALSE );

    // attemp to query the synthetics.. added in 15.7.
    // As they may not have been enabled for a particular user, this may fail
    if (( status_i = query_static_synthetic ( click_px ) ) < SUCCESSFUL)
    {
	// failed.. do nothing at present. Once 15.7 permissions all set up,
	// then a failure for this query is a major problem.
    }


    // OK so far, try to info about known instrument classes
    if ( (status_i = query_instrument_classes (click_px) ) < SUCCESSFUL )
    {
	// do nothing - error status is already in status_i
	return status_i;
    }

    // load and sort the links for instrument classes
    sort_instrument_classes ( &click_px->our_market_x, FALSE );
    // check if we have instrument classes for all underlying.
    // also check internal consistency of info we have
    check_all_underlying_instr_classes ( click_px,
						&click_px->our_market_x );




    // OK so far, try to collect info about all series
    if ( (status_i = query_all_series ( click_px) ) < SUCCESSFUL)
    {
	// do nothing - error status is already in status_i
	return status_i;
    }

    // load and sort the links for series
    sort_series_tables ( &click_px->our_market_x, FALSE );

    // update links from all series to parent underlyings,
    // and attach list of series to each underlying
    point_series_at_underlying ( click_px,
				    &click_px->our_market_x,
				    FALSE );



    
    // OK so far, try to collect all info about the Users within our Customer,
    //	    and the instrument groups they trade
    if ( (status_i = query_brokers (click_px) ) < SUCCESSFUL )
    {
	// do nothing - error status is already in status_i
	return status_i;
    }


    // OK so far, try to info about bins/PMMs/MMs
    if ( (status_i = query_bins_mms (click_px) ) < SUCCESSFUL )
    {
	// do nothing - error status is already in status_i
	return status_i;
    }

    
    // all loads worked OK... do any further internal setup

    // figure out how many bin / orderbook combinations there are
    calc_bin_orderbook_combos ( &click_px->our_market_x );

    // strictly speaking, getting underlying statii is NOT static.. but do it anyway
    query_all_underlying_statii ( click_px );

    // similarly, querying the instrument statii isn't static data
    query_instrument_status ( click_px );

    // try to update the 'saved' last order info, as we have changed the internal series data
    if ( click_px->saved_order_x.series_px )
	click_px->saved_order_x.series_px = find_series_by_name_s ( click_px, click_px->saved_order_series_name_s );

    // same deal with combo.. restore anything we had for a saved combo
    if ( click_px->saved_combo_x.underlying_px )
	click_px->saved_combo_x.underlying_px = find_under_by_name_s ( click_px, click_px->saved_combo_underlying_name_s );
    for ( item_index_u = 0;
	    item_index_u < click_px->saved_combo_x.no_of_legs_u;
	    item_index_u++ )
    {
	click_px->saved_combo_x.leg_x[item_index_u].series_px = 
	    find_series_by_name_s ( click_px, click_px->saved_combo_series_name_s [ item_index_u ] );
    }

    // query todays business date
    if ( (status_i = query_business_date ( click_px )) < SUCCESSFUL)
    {
	// do nothing - error status is already in status_i
	return status_i;
    }

    // query for missed BI8 broadcasts
    if ( (status_i = query_missed_general_text ( click_px )) < SUCCESSFUL)
    {
	// do nothing - error status is already in status_i
	return status_i;
    }

    // hand back whatever the worst status we hit along the way
    return ( status_i );
}   // load_static_data



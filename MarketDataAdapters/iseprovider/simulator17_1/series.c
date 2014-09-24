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


Title : series.c
Author: Neil Cossor
Function :

    This file is series.c, which provides series handling facilities to
    the ISESim.exe program, except for the static CDB collection,
    which is in static.h/.c
    
Revision history :-

yymmdd	vers'n	author	    notes
------	------	------	    -----
9905xx	v0.00	Neil Cossor initial version
990910	v0.01		    fix query_series_prices to be segmented
990915	v0.02		    create update_one_series, used in static CDB load and bi202 bcast
			    add release_series_sort_tables (), mod clear_series_from_memory to use it
			    mod sort_series_tables (extra param) to selectively use it.
			    similar deal with point_series_at_underlying.
			    fix bug.. we were not releasing the chain of series attached to each underlying
			    mod for extra param to free_gen_buf_node_q
990928			    fix bug - we did NOT correctly halt on segment query, when returned a 0 segment #
000105	v0.03		    update for latest OM .h files (15.7 OM release)
000122			    mod update_one_series to update ptr to instr_group, and calc autoquote doubles
000320	v0.04		    enhance query_series_prices to allow for wildcard underlying,
			    change params to have orderbook, and optional bin
			    change series_volume_prices_query to match
000331	v0.05		    fix series_volume_prices_query, which did not handle 'all' case correctly
				(series NULL, work through all orderbooks)
000403			    mod find_series_by_name_s to handle null input string
000422	v1.00		    update to match OM release 15.7-3 (move of some fields from da203 to da210).
			    update_one_series, query_series_prices
000425	v1.01		    add list_series (dumps file list of series)
000521	v1.02		    mod cmp_series_ptr_by_ise_series_instr to compare series fields in same
			    order that OM does..
010304	v1.03		    minor reformat for IA101 data

*/

#include "stdincl.h"

#include "error.h"
#include "instrument.h"
#include "series.h"
#include "underlying.h"
#include "utility.h"

//------------------------- externally accessable routines -------------------

void update_one_series ( click_details_xt *click_px,
			    ise_series_info_xt *ise_series_info_px,
			    da202_t *answer_series_px )
/*
Function:   update_one_series
Author:	    Neil Cossor
Modified:   20000422
Description:

    We have a da202 record, from either the initial CDB load (at logon)
    or from a BI202 broadcast.

    We have an ISEsim internal record to store that info in...
    so we just need to move the data from the message into that storage
*/
{
    // pointer to the instrument class record
    instrument_class_pt     instrument_class_px;

    
    
    
    // grab all of the plain series components from the OM msg data
    get_click_series ( &ise_series_info_px->ise_series_x,
			&answer_series_px->series );

    // grab the series name e.g. XYZ0DEC90.0C
    rx_click_string ( &ise_series_info_px->ise_name_s,
		    &answer_series_px->ins_id_s,
		    sizeof ( answer_series_px->ins_id_s ));
    // grab the symbol name
    rx_click_string ( &ise_series_info_px->ise_symbol_name_s,
		    &answer_series_px->symbol_s,
		    sizeof ( answer_series_px->symbol_s ));
    // grab the contract size, in correct byte order
    PUTLONG ( ise_series_info_px->contract_size_u,
		answer_series_px->contract_size_i );
    // grab the month code, and strike code - 1 byte vals, so no byte swaps
    ise_series_info_px->ise_strike_c = answer_series_px->strike_code_c;
    ise_series_info_px->ise_month_c = answer_series_px->month_code_c;


    // find the instrument group which matches this series
    ise_series_info_px->instr_group_px = find_instr_group ( click_px,
							    ise_series_info_px->ise_series_x.instrument_group_u );
    // find the instrument class which matches this series.
    instrument_class_px = find_instrument_class_by_series ( &click_px->our_market_x,
							    &ise_series_info_px->ise_series_x );
    // did we find an instrument class which matches this series ?
    if ( instrument_class_px )
    {
	// found the instrument class - save it for future reference
	ise_series_info_px->instrument_class_px = instrument_class_px;
    }
    else
    {
	// oops - no instrument class for this series
	printf ("\tWarning - no instrument class found for series %s\n",
		    &ise_series_info_px->ise_name_s );
    }   // test if we found the instrument class for this series



    
    
    // calculate the strike price and days until expiry as doubles.. required for autoquoting

    // Move the strike price into a double (black & scholes input type)
    ise_series_info_px->strike_f = double_from_whole_decimal (
				ise_series_info_px->ise_series_x.strike_price_i,
				ise_series_info_px->instrument_class_px->dec_in_premium_u);

    // get days from now to expiry as double, and recalc that into years
    ise_series_info_px->years_to_expiry_f = double_days_from_now_to_date_xt (
                                &ise_series_info_px->ise_series_x.expiry_date_x) / 365.0;

}   // update_one_series

// ---------------------------------------------------------------------------

void get_click_series ( ise_series_xt * ise_series_x,
			series_t * click_series )
/*
Function:   get_click_series
Author:     Neil Cossor
Modified:   9907xx
Description:

    fn which transfers the attribute of a CLICK series, complete with weird bit-packing,
    endian issues etc, into a local ise_series_x, in "sensible" format
*/
{
    // 1-byte numbers can just be assigned
    ise_series_x->country_u = click_series->country_c;
    ise_series_x->market_u = click_series->market_c;
    ise_series_x->instrument_group_u = click_series->instrument_group_c;
    ise_series_x->modifier_count_u = click_series->modifier_c;
    // grab the commodity code, in correct byte order
    PUTSHORT ( ise_series_x->underlying_code_u,
		click_series->commodity_n );
    // grab the strike price, in correct byte order
    PUTLONG ( ise_series_x->strike_price_i,
		click_series->strike_price_i );
    // unpack the 3 fields of the expiry date
    unpack_click_bit_date ( click_series->expiration_date_n,
			    &ise_series_x->expiry_date_x.year_u,
			    &ise_series_x->expiry_date_x.month_u,
			    &ise_series_x->expiry_date_x.day_u );
}   // get_click_series

//-----------------------------------------------------------------------------------

void put_click_series ( series_t * click_series,
			ise_series_xt * ise_series_x )
/*
Function:   put_click_series
Author:     Neil Cossor
Modified:   9907xx
Description:

    fn which packs a CLICK series, complete with weird bit-packing,
    endian issues etc, from a local ise_series_xt.
*/
{
    // 1-byte numbers can just be assigned
    click_series->country_c = ise_series_x->country_u;
    click_series->market_c = ise_series_x->market_u;
    click_series->instrument_group_c = ise_series_x->instrument_group_u;
    click_series->modifier_c = ise_series_x->modifier_count_u;

    // pack the commodity code, in correct byte order
    PUTSHORT ( click_series->commodity_n,
		ise_series_x->underlying_code_u );

    // grab the strike price, in correct byte order
    PUTLONG ( click_series->strike_price_i,
		ise_series_x->strike_price_i );

    // pack the 3 fields of the expiry date
    pack_click_bit_date ( &click_series->expiration_date_n,
			    ise_series_x->expiry_date_x.year_u,
			    ise_series_x->expiry_date_x.month_u,
			    ise_series_x->expiry_date_x.day_u );
}   // put_click_series

// ---------------------------------------------------------------------------

char * series_status_to_string ( uint8 series_status_u )
/*
Function:   series_status_to_string
Author:     Neil Cossor
Modified:   9907xx
Description:

    We have a series status number, which we wish to output.
    Convert from the number into a readable string.
    NB there are a bundle of _to_string routines in utility.c,
    but this routine is very specifically series related,
    so...put it here.

Input Params:

    The series status #

Output Params:

    Pointer to string containing string for that status
*/
{
    static char *series_status_s [] =
    {
	"Closed  ",
	"Queueing",
	"Trying  ",
	"Regular ",
	"Halted  ",
	"Fast Mkt",
	"Unkn sts"
    };

    switch ( series_status_u )
    {
    case SERIES_STATUS_CLOSED:
	return series_status_s[0];
	break;
    case SERIES_STATUS_QUEUING:
	return series_status_s[1];
	break;
    case SERIES_STATUS_TRYING:
	return series_status_s[2];
	break;
    case SERIES_STATUS_REGULAR:
	return series_status_s[3];
	break;
    case SERIES_STATUS_HALT:
	return series_status_s[4];
	break;
    case SERIES_STATUS_FAST_MARKET:
	return series_status_s[5];
	break;
    default:
	return series_status_s[6];
    }	// switch on numeric value
}   // series_status_to_string

// -------------------------------------------------------------------------------------

static int32 query_series_prices ( click_details_xt * click_px,
				    ise_underlying_xt *under_px,
				    uint8 orderbook_u,
				    uint8 bin_u )
/*
Function : query_series_prices
Author	 : Neil Cossor
Modified : 20010304
Description:

    Query prices (and volume) for series with a common underlying instrument
    Uses iq101

    Note that this is a segmented query.
    Presumably this is so that we can get info from many countries
    (i.e. from multiple equities exchanges). Provided the # of markets is small,
    then the answer MAY be segmented, but probably wont be...

    hmmm... apparently there are about 400 series in Amazon, so I still
    don't see we will exceed the 1000 records that can be in one response

    This is called from a command for VOLUMES
*/
{
    // the query msg
    generic_txn_xt	    OMmsg_x;
    uint32		    tx_length_u;
    // segment # - note we set initial value
    uint16		    segment_u = 0;
    int 		    status_i;
    uint16		    item_index_u,   // local loop var
			    item_limit_u;   // limit on loop - may be endian-swapped from response
    ise_series_xt	    ise_series_x;   // temp, the series in ISE form
    ise_series_info_pt	    found_series_px;	// pointer to info about a series
    int 		    first_series_b  // flag if this is info for first series
			    = TRUE;	    // with initial value - used to spit out header
    // somewhere to put the intermediate strings, for a formatted prices
    char		    bid_price_s[20],
			    ask_price_s[20],
			    high_price_s[20],
			    low_price_s[20],
			    last_price_s[20];

    // save the facility code and fn code
    OMmsg_x.fn_code = LOG_K_Q_SERIES_PRICES;
    OMmsg_x.facility_u = click_px->facility_no_u + 7;	// EP7/ET7

    tx_length_u = sizeof ( query_prices_iq101_t );
    pack_OM_header ( &OMmsg_x, tx_length_u, 'I', 'Q', 101 );
    // zot the series selector
    memset( &OMmsg_x.tx_x.query_series_prices_x.series, NUL, sizeof ( series_t ) );

    // now qualify our selection.

    // and use our country / market
    OMmsg_x.tx_x.query_series_prices_x.series.country_c = click_px->our_market_x.country_u;
    OMmsg_x.tx_x.query_series_prices_x.series.market_c = click_px->our_market_x.market_u;

    /* and our orderbook and bin, as supplied
	NB, if a specific underlying is supplied, the orderbook and bin params should have
	been supplied from the underlying's info.

	Note that as it is possible for a user to supply a orderbook/bin combination
	which is invalid, we can get an error.

    */
    OMmsg_x.tx_x.query_series_prices_x.orderbook_c = orderbook_u;
    OMmsg_x.tx_x.query_series_prices_x.bin_c = bin_u;

    // Was a specific underlying supplied ?
    if ( under_px )
    {
	// user specified an underlying.
	PUTSHORT ( OMmsg_x.tx_x.query_series_prices_x.series.commodity_n,
		    under_px->commodity_code_u );
    }	// test if underlying supplied

    do	// while (TRUE)
    {
	segment_u++; // The segment # we wish to retrieve
	PUTSHORT( OMmsg_x.tx_x.query_series_prices_x.segment_number_n, segment_u );

	// go do the query
	status_i = do_click_query_rx ( click_px,
					&OMmsg_x,
					tx_length_u,
					LOG_K_R_SERIES_PRICES );

	if ( status_i >= SUCCESSFUL )
	{
	    PUTSHORT ( item_limit_u, click_px->log_reply_x.trans_reply_x.answer_prices_x.items_n );

	    if (item_limit_u == 0)
		break;	// kick out of loop collecting segmented answer

	    for (item_index_u =0;
		    item_index_u < item_limit_u;
		    item_index_u++)
	    {
		// should we spit out our display header ?
		if ( first_series_b )
		{
		    printf( "\nSERIES..........STATUS........BID..qty......ASK..qty.....HIGH......LOW.....LAST....VOLUME...TURNOVER\n"); 
		    // and note we have thrown out the display header
		    first_series_b = FALSE;
		}

		// get the series, in "normal" form
		get_click_series ( &ise_series_x,
				    &click_px->log_reply_x.trans_reply_x.answer_prices_x.item[item_index_u].series);
		// find the record with the static data about the series
		found_series_px = find_series_by_ise_series ( click_px,
								&ise_series_x );
		// we better know about this series
		if ( found_series_px )
		{
		    // note when we got this info
		    found_series_px->time_bbo_update_u = time(NULL);

		    // update status / prices for the series - remembering endian issues
		    PUTLONG ( found_series_px->best_bid_premium_i,
				click_px->log_reply_x.trans_reply_x.answer_prices_x
				    .item[item_index_u].best_bid_premium_i );
		    PUTLONG ( found_series_px->best_ask_premium_i,
				click_px->log_reply_x.trans_reply_x.answer_prices_x
				    .item[item_index_u].best_ask_premium_i );
		    PUTLONG ( found_series_px->best_bid_size_u,
				click_px->log_reply_x.trans_reply_x.answer_prices_x
				    .item[item_index_u].best_bid_size_u );
		    PUTLONG ( found_series_px->best_ask_size_u,
				click_px->log_reply_x.trans_reply_x.answer_prices_x
				    .item[item_index_u].best_ask_size_u );
		    PUTLONG ( found_series_px->closing_price_i,
				click_px->log_reply_x.trans_reply_x.answer_prices_x
				    .item[item_index_u].closing_price_i );
		    PUTLONG ( found_series_px->opening_price_i,
				click_px->log_reply_x.trans_reply_x.answer_prices_x
				    .item[item_index_u].opening_price_i );
		    PUTLONG ( found_series_px->high_price_i,
				click_px->log_reply_x.trans_reply_x.answer_prices_x
				    .item[item_index_u].high_price_i );
		    PUTLONG ( found_series_px->low_price_i,
				click_px->log_reply_x.trans_reply_x.answer_prices_x
				    .item[item_index_u].low_price_i );
		    PUTLONG ( found_series_px->last_price_i,
				click_px->log_reply_x.trans_reply_x.answer_prices_x
				    .item[item_index_u].last_price_i );
		    PUTLONG ( found_series_px->volume_u,
				click_px->log_reply_x.trans_reply_x.answer_prices_x
				    .item[item_index_u].volume_u );
		    PUTLONG ( found_series_px->turnover_u,
				click_px->log_reply_x.trans_reply_x.answer_prices_x
				    .item[item_index_u].turnover_u );
		    PUTLONG ( found_series_px->turnover_u,
				click_px->log_reply_x.trans_reply_x.answer_prices_x
				    .item[item_index_u].turnover_u );
		    PUTLONG ( found_series_px->open_balance_u,
				click_px->log_reply_x.trans_reply_x.answer_prices_x
				    .item[item_index_u].open_balance_u );
		    PUTSHORT ( found_series_px->fast_market_level_u,
				click_px->log_reply_x.trans_reply_x.answer_prices_x
				    .item[item_index_u].fast_market_level_n );
		    found_series_px->series_status_u =
			click_px->log_reply_x.trans_reply_x.answer_prices_x
				    .item[item_index_u].series_status_c;
		    // grab the fast market level
		    PUTSHORT ( found_series_px->fast_market_level_u,
				click_px->log_reply_x.trans_reply_x.answer_prices_x.item[item_index_u].fast_market_level_n );

		    /*
			NOTE OM has made efficient use of the fact that
			there is already a query/message in the pipe, so they pack
			as many extra series from the same bin as will fit
			in one message.

			Filter out the display of the extras - it wont hurt to
			update the internal data for all of the extra series though.

			as of 20000320, we allow for no underlying having been supplied..
			so we cannot, of course, filter on data we don't have.

			NB relies on C short-circuit evaluation for test to be OK (under_data_px==NULL)
		    */
		    if ((under_px == NULL)
			|| ( found_series_px->ise_series_x.underlying_code_u
				== under_px->commodity_code_u ))
		    {
			// format the prices with the correct # of decimals
			format_whole_decimal_s ( (char *)&bid_price_s,
						found_series_px->best_bid_premium_i,
						found_series_px->instrument_class_px->dec_in_premium_u,
						9 );	// total width for strike-price string
			format_whole_decimal_s ( (char *)&ask_price_s,
						found_series_px->best_ask_premium_i,
						found_series_px->instrument_class_px->dec_in_premium_u,
						9 );	// total width for strike-price string
			format_whole_decimal_s ( (char *)&high_price_s,
						found_series_px->high_price_i,
						found_series_px->instrument_class_px->dec_in_premium_u,
						9 );	// total width for strike-price string
			format_whole_decimal_s ( (char *)&low_price_s,
						found_series_px->low_price_i,
						found_series_px->instrument_class_px->dec_in_premium_u,
						9 );	// total width for strike-price string
			format_whole_decimal_s ( (char *)&last_price_s,
						found_series_px->last_price_i,
						found_series_px->instrument_class_px->dec_in_premium_u,
						9 );	// total width for strike-price string

			printf( "%-16s%-8s%s%5d%s%5d%s%s%s %9d  %9d\n",
				  &found_series_px->ise_name_s,
				  series_status_to_string ( found_series_px->series_status_u),
				  &bid_price_s,
				  found_series_px->best_bid_size_u,
				  &ask_price_s,
				  found_series_px->best_ask_size_u,
				  &high_price_s,
				  &low_price_s,
				  &last_price_s,
				  found_series_px->volume_u,
				  found_series_px->turnover_u );
		    }   // test if this is a series which matched our query
		}	// test if we know about this series
		else
		{
		    // oops - we just got status / prices for a series we don't know about
		    printf ( "ERROR - got Price / Status for an unknown series\n" );
		}	// test if error finding the series we got info for
	    }   // for loop, working thru as many series records we got

	    /* regardless of whether there were any items in it, if the segment # in the response
		is 0, then we got all the data there is. NB although this is endian, a binary
		0 is still a 0 - don't have to do a PUTSHORT
	    */
	    if ( click_px->log_reply_x.trans_reply_x.answer_prices_x.segment_number_n == 0 )
		// no more data - out of here
		break;
	}	// test success from IO
    }
    while (TRUE);

    return status_i;
}   // query_series_prices

//-----------------------------------------------------------------------------------

int32 series_volume_prices_query ( click_details_xt * click_px,
				    int argc,
				    char *argv[ ])
/*
Function : series_volume_prices_query
Author	 : Neil Cossor
Modified : 20000331
Description :

    Finds out the current snapshot of price / volume figures
    for series.
    
    This can be
    - all series with the a nominated underlying.
    - all series with a nominated orderbook (all bins in that orderbook)
    - all series with a nominated orderbook, with a nominated bin 
    - all series in all bins in all orderbooks

    Dumps them out to screen.
*/
{
    ise_underlying_pt	    under_data_px = NULL;	// ptr to info about underlying
    int32		    status_i;
    // orderbook and bin, if the caller supplied them
    uint8		    orderbook_u,
			    bin_u;

    if (click_px->logged_on_i != TRUE)
    {
	prompt_logged_on ( "Volumes query" );
	return NOT_LOGGED_ON;
    }

    // sufficient arguments ?
    if ( argc < 2 )
    {
	printf ("VOLUMES,<underlying name>\n  or\nVOLUMES,*[,<orderbook #>[,<bin #>]]\n");
	return SUCCESSFUL;
    }

    // set default vals for orderbook and bin
    orderbook_u = 0;
    bin_u = 0;

    // convert stock name to to commodity number
    under_data_px = find_under_by_name_s ( click_px,
					    argv[1] );

    if ( under_data_px != NULL )
    {
	// use orderbook and bin for the known underlying

	// go get the prices and statii for the series with this underlying
	// the output data (if successful) is updated info in the common
	// that common should also continue to be updated by broadcast ??? NOT YET
	status_i = query_series_prices ( click_px,
					    under_data_px,
					    under_data_px->orderbook_u,
					    under_data_px->bin_u );
    }
    else
    {
	// test for '*' - in which case we might have orderbook / bin supplied
	if ( strcmp ( argv[1],"*" ) != 0 )
	{
	    // not a known specific underlying, and not underlying wildcard
	    printf ("ERROR - Underlying [%s] cannot be found\n", 
			argv[1]);

	    // bail out of the routine
	    return SUCCESSFUL;
	}
	else
	{
	    // wildcard underlying

	    // caller supplied a bin ?
	    if (argc >= 4)
	    {
		// have bin param - read it
		bin_u = read_unsigned_limited ( argv[3], click_px->our_market_x.max_bin_u );
	    }	// did the user supply a bin ?

	    // has the caller supplied an orderbook bin ?
	    if (argc >= 3)
	    {
		// have orderbook param - read it
		orderbook_u = read_unsigned_limited ( argv[2], click_px->our_market_x.max_orderbook_u );
	    }

	    /* have we a wildcard for orderbook ? 
		If so, then specific bin is irrelevant (erroneous), but we must loop thru orderbooks
		If not, then bin MUST be a zero (will fail for all orderbook / bin combinations but one)
	    */
	    if (orderbook_u == 0)
	    {
		if (bin_u!=0)
		{
		    // error
		    printf ("Bin specifier [%u] invalid with orderbook wildcard, 0 used\n", bin_u );
		}

		for ( orderbook_u = 1;
			orderbook_u <= click_px->our_market_x.max_orderbook_u;
			orderbook_u++ )
		{
		    status_i = query_series_prices ( click_px,
							NULL,
							orderbook_u,
							0 );	// bin must be zero if orderbook wildcard
		}   // loop on orderbooks
	    }
	    else
	    {
		// specific underlying.. user whatever orderbook & bin they provided
		status_i = query_series_prices ( click_px,
						    under_data_px,
						    orderbook_u,
						    bin_u );
	    }	// see if wildcard orderbook, within wildcard underlying
	}   // test if wildcard underlying
    }	// test if known specific underlying

    return status_i;
}   // series_volume_prices_query

//-----------------------------------------------------------------------------------

void release_series_sort_tables ( country_market_xt * country_market_px )
/*
Function : release_series_sort_tables
Author   : Neil Cossor
Modified : 9905xx
Description:

    We have are discarding our known (sorted) tables relating to series.
    This could be becoz we have gone offline, or becoz we just received
    update data which invalidates our old sorted info
*/
{
    free ( country_market_px->series_sorted_by_name_p );
    country_market_px->series_sorted_by_name_p = 0;
    free ( country_market_px->series_sorted_by_ise_series_under_p );
    country_market_px->series_sorted_by_ise_series_under_p = 0;
    free ( country_market_px->series_sorted_by_ise_series_instr_p );
    country_market_px->series_sorted_by_ise_series_instr_p = 0;
}   // release_series_sort_tables

//-----------------------------------------------------------------------------------

void clear_series_from_memory( click_details_xt *click_px )
/*
Function : clear_series_from_memory
Author	 : Neil Cossor
Modified : 9905xx
Description:

    Removes the loaded series from memory
*/
{
    // empty the whole q, zot the count of loaded series,
    // and release the sort structures we have pointing into them
    free_gen_buf_node_q ( &click_px->our_market_x.known_series_q, TRUE );

    release_series_sort_tables ( &click_px->our_market_x );
}   // clear_series_from_memory

//-----------------------------------------------------------------------------------

static void display_one_series ( ise_series_info_xt * series_data_px )
/*
Function:   display_one_series
Author:     Neil Cossor
Modified:   20000606
Description:

    Display the summary info about a series
*/
{
    // somewhere to put the intermediate string, for a formatted strike price
    char		strike_s[20];

    format_whole_decimal_s ( (char *)&strike_s,
			    series_data_px->ise_series_x.strike_price_i,
			    series_data_px->instrument_class_px->dec_in_strike_price_u,
			    10 );	// total width for strike-price string

    // dump the series name, strike price, and expiry date
    printf ( "%-16s %s %02d-%02d-%04d %d \"%s\" %c %c  ", 
		&series_data_px->ise_name_s,
		&strike_s,
		series_data_px->ise_series_x.expiry_date_x.month_u,
		series_data_px->ise_series_x.expiry_date_x.day_u,
		series_data_px->ise_series_x.expiry_date_x.year_u,
		series_data_px->ise_series_x.instrument_group_u,
		&series_data_px->ise_symbol_name_s,
		series_data_px->ise_month_c,
		series_data_px->ise_strike_c
		);
}   // display_one_series

//-----------------------------------------------------------------------------------

void display_series ( click_details_xt * click_px,
			int argc,
			char *argv[] )
/*
Function : display_series
Author	 : Neil Cossor
Modified : 9906xx
Description:

    Presents the list of known series to the user, collected
    previously (at logon).
    It displays series with a particular underlying.

    Triggered by one of the commands
    Series,*		NB you DON'T want to do this in production - 40-60,000 series...
    Series,<underlying name>

Input Params:

    standard argc/argv[] C-style breakdown of command line.
    argv[0]	SERIES
    argv[1]	<underlying name> or *
*/
{
    // pointer to actual ise info about a series
    ise_series_info_xt	*series_data_px;
    // pointer to chained block, which has a pointer (msg_p) to the info about a series
    gen_buf_node_xt	*series_node_px;
    ise_underlying_pt	    under_data_px = NULL;	// ptr to info about underlying

    uint16		NextLine_n; 
    uint16		TestLine_n;
    under_id_st 	underlying_name_s;
    // commodity code we are matching - 0 for all, or found from the underlying name
    uint16		commodity_u = 0;

    // decide if enuff arguments
    if ( argc <= 1 )
    {
	// not enuff, prompt the user
	printf ("Series,[*|<underlying name>]\n");
	return;
    };

    // if we get here, we had enuff arguments - at least 2
    if ( *argv[1] == '*' )  // show all series
    {
	// do nothing - leave selected commodity code with 0
    }
    else
    {
	// we were given an underlying name - find the specific commodity code
	strcpy ( (char *) &underlying_name_s, argv[1] );

	// find the underlying with the nominated name
	// convert stock name to to commodity number
	under_data_px = find_under_by_name_s ( click_px,
						underlying_name_s );

	if ( under_data_px == NULL )
	{
	    printf ("ERROR - display_series - Underlying cannot be found\n");
	    return;
	}
	else
	    commodity_u = under_data_px->commodity_code_u;
    }

    if ( !empty_q ( &click_px->our_market_x.known_series_q ) )
    {
	// point to first block in the series chain
	series_node_px = (gen_buf_node_xt *)click_px->our_market_x.known_series_q.chain.next;

	printf( "\n" );

	TestLine_n = 0;

	while ( series_node_px != NULL )
	{
	    // point to the actual series info
	    series_data_px = (ise_series_info_xt *)series_node_px->msg_p;
	    if ( commodity_u > 0 ) // looking for the series for one stock
	    {
		if ( commodity_u == series_data_px->parent_under_px->commodity_code_u )
		{
		    display_one_series ( series_data_px );

		    TestLine_n++;
		    NextLine_n = ( ((TestLine_n / 2 ) * 2) ==  TestLine_n );

		    if (NextLine_n )
		    {
			printf ("\n");
		    }
		}
		else
		{
		    // doesn't match, keep looking
		}
	    }
	    else // display all series for all stocks
	    {
		display_one_series ( series_data_px );

		TestLine_n++;
		NextLine_n = ( ( TestLine_n / 2 ) * 2 ==  TestLine_n );

		if (NextLine_n )
		{
		    printf ("\n");
		}
	    }
	    
	    series_node_px = (gen_buf_node_xt *)series_node_px->chain.next;
	}

	if ( !NextLine_n )
	{
	     printf ("\n");
	}
    }
    else
    {
	printf (" There are no series in memory! \n");
    }
}   // display_series

// -------------------------------------------------------------------------------------

static int cmp_series_ptr_by_name ( gen_buf_node_xt ** keybuf,
				gen_buf_node_xt ** datumbuf )
/*
    routine needed to compare two the data pointed to by the supplied
    params, and return a -1, 0, +1 value for whether the data pointed
    by *keyval is <, ==, > the data pointed to by *datum.

    Comparison is of the string name of the series. These should be unique
    within a country / market.

    Used by qsort and bsearch.

    The input parameters are individual pointers to pointers to gen_buf_node_xt.
    We are comparing the ise_series_xt which are pointed to by the <msg> field
    of the gen_buf_node_xt's.
*/
{
    // pointers to ise_series_xt component of the gen_buf_node's we were given
    ise_series_info_pt	keyval, datum;

    // *keybuf == pointer to a gen_buf_node_xt (an element of the sorted array)
    // **keybuf == a gen_buf_node_xt
    gen_buf_node_xt * keybuf_ptr, * datumbuf_ptr;
    keybuf_ptr = *keybuf;
    datumbuf_ptr = *datumbuf;

    keyval = (ise_series_info_pt) keybuf_ptr->msg_p;
    datum = (ise_series_info_pt) datumbuf_ptr->msg_p;

    // compare the series names in the bottomr structure
    return ( strcmp ( keyval->ise_name_s,
			datum->ise_name_s ) );
}   // cmp_series_ptr_by_name

// ---------------------------------------------------------------------------

int cmp_series_by_ise_series_under ( ise_series_xt *first_series_px,
					ise_series_xt *second_series_px )
/*
Function:   cmp_series_by_ise_series_under
Author:     Neil Cossor
Modified:   9907xx
Description:

    routine needed to compare two records of data pointed to by the supplied
    params, and return a -1, 0, +1 value for whether the data pointed
    by first_series_px is <, ==, > the data pointed to by second_series_px

    Comparison is of the series ID. This has several fields. The order we sort / compare
    the fields in is not important, provided that we always do them in the same order.

    HOWEVER, this fn is used to sort with underlying as the first critical key,
    so all series with the same underlying will be grouped together.

    These are unique within a country / market - actually they are unique
    within a particular CLICK system.

    Used by cmp_series_ptr_by_ise_series_under, which are
    in turn called by qsort and bsearch.
    It is also used to compare the series in a query response msg to the
    series in the query request msg... as OM sometimes responds with
    data which is different to that in the request... (weird)
    
    Our order of comparison is
	country
	market
	underlying (== commodity, typically == an equity)   // hence _under
	instrument group
	expiry_date year
	expiry date month
	expiry date day
	strike price
	modifier
	
    With that order in mind...at each level,
    if the first_series_px->field < second_series_px->field,
    then we return -1.
    if the first_series_px->field > second_series_px->field
    then we return +1.
    If we are still here, then the field values are the same,
    and we drop to the next field for comparison.

    If we get to the end, we are equal, and we return a 0
*/
{
    // compare country codes
    if ( first_series_px->country_u < second_series_px->country_u )
	return -1;
    else if ( first_series_px->country_u > second_series_px->country_u )
	return +1;
    // if we get here, we have equal field values. Progress to the next field

    // compare market codes
    if ( first_series_px->market_u < second_series_px->market_u )
	return -1;
    else if ( first_series_px->market_u > second_series_px->market_u )
	return +1;
    // if we get here, we have equal field values. Progress to the next field

    // NB this is the OPPOSITE comparison order to cmp_series_ptr_by_ise_series_instr
    // compare underlying == commodity_code (typically == equity)
    if ( first_series_px->underlying_code_u < second_series_px->underlying_code_u )
	return -1;
    else if ( first_series_px->underlying_code_u > second_series_px->underlying_code_u )
	return +1;
    // if we get here, we have equal field values. Progress to the next field

    // compare instrument group (e.g. American Calls, vs European puts...)
    if ( first_series_px->instrument_group_u < second_series_px->instrument_group_u )
	return -1;
    else if ( first_series_px->instrument_group_u > second_series_px->instrument_group_u )
	return +1;
    // if we get here, we have equal field values. Progress to the next field


    // compare expiry date year
    if ( first_series_px->expiry_date_x.year_u < second_series_px->expiry_date_x.year_u )
	return -1;
    else if ( first_series_px->expiry_date_x.year_u > second_series_px->expiry_date_x.year_u )
	return +1;
    // if we get here, we have equal field values. Progress to the next field

    // compare expiry date month
    if ( first_series_px->expiry_date_x.month_u < second_series_px->expiry_date_x.month_u )
	return -1;
    else if ( first_series_px->expiry_date_x.month_u > second_series_px->expiry_date_x.month_u )
	return +1;
    // if we get here, we have equal field values. Progress to the next field

    // compare expiry date day (of month)
    if ( first_series_px->expiry_date_x.day_u < second_series_px->expiry_date_x.day_u )
	return -1;
    else if ( first_series_px->expiry_date_x.day_u > second_series_px->expiry_date_x.day_u )
	return +1;
    // if we get here, we have equal field values. Progress to the next field

    // compare strike price (typically in cents)
    if ( first_series_px->strike_price_i < second_series_px->strike_price_i )
	return -1;
    else if ( first_series_px->strike_price_i > second_series_px->strike_price_i )
	return +1;
    // if we get here, we have equal field values. Progress to the next field

    // compare modifier count. Typically 0, oaccasionally 1 or 2.
    if ( first_series_px->modifier_count_u < second_series_px->modifier_count_u )
	return -1;
    else if ( first_series_px->modifier_count_u > second_series_px->modifier_count_u )
	return +1;
    // if we get here, we have equal field values. Progress to the next field

    // except we have run out of fields.. all field values match, so return "equal" value.
    return 0;
}   // cmp_series_by_ise_series_under

// ---------------------------------------------------------------------------

static int cmp_series_ptr_by_ise_series_under ( gen_buf_node_xt ** keybuf,
						gen_buf_node_xt ** datumbuf )
/*
    routine needed to compare two records of data pointed to by the supplied
    params, and return a -1, 0, +1 value for whether the data pointed
    by *keyval is <, ==, > the data pointed to by *datum

    Comparison is of the series ID. This has several fields. The order we sort / compare
    the fields in is not important, provided that we always do them in the same order.

    HOWEVER, this fn is used to sort with underlying as the first critical key,
    so all series with the same underlying will be grouped together.

    These are unique within a country / market - actually they are unique
    within a particular CLICK system.

    Used by qsort and bsearch.
    
    Our order of comparison is
	country
	market
	underlying (== commodity, typically == an equity)   // hence _under
	instrument group
	expiry_date year
	expiry date month
	expiry date day
	strike price
	modifier
	
    With that order in mind...at each level,
    if the keyval->field < datum->field,
    then we return -1.
    if the keyval->field > datum->field
    then we return +1.
    If we are still here, then the field values are the same,
    and we drop to the next field for comparison.
*/
{
    // pointers to ise_series_xt component of the gen_buf_node's we were given
    ise_series_info_pt	keyval, datum;
    
    // *keybuf == pointer to a gen_buf_node_xt (an element of the sorted array)
    // **keybuf == a gen_buf_node_xt
    gen_buf_node_xt * keybuf_ptr, * datumbuf_ptr;
    keybuf_ptr = *keybuf;
    keyval = (ise_series_info_pt) keybuf_ptr->msg_p;
    datumbuf_ptr = *datumbuf;
    datum = (ise_series_info_pt) datumbuf_ptr->msg_p;

    return cmp_series_by_ise_series_under ( &keyval->ise_series_x,
					    &datum->ise_series_x );
}   // cmp_series_ptr_by_ise_series_under

// -------------------------------------------------------------------------------------

static int cmp_series_ptr_by_ise_series_instr ( gen_buf_node_xt ** keybuf,
					gen_buf_node_xt ** datumbuf )
/*
Function:   cmp_series_ptr_by_ise_series_instr
Author:	    Neil Cossor
Modified:   20000521
Description:

    routine needed to compare two the data pointed to by the supplied
    params, and return a -1, 0, +1 value for whether the data pointed
    by *keyval is <, ==, > the data pointed to by *datum

    Comparison is of the series ID. This has several fields. The order we sort / compare
    the fields in is not important, provided that we always do them in the same order.

    HOWEVER, this fn is used to sort with instrument_group as the first critical key,
    so all series with the same instrument_group will be grouped together.

    These are unique within a country / market - actually they are unique
    within a particular CLICK system.

    Used by qsort and bsearch.
    
    Our order of comparison is
	country
	market
	instrument group		    // hence _instr
	underlying (== commodity, typically == an equity)
	modifier
	expiry_date year
	expiry date month
	expiry date day
	strike price
	
    With that order in mind...at each level,
    if the keyval->field < datum->field,
    then we return -1.
    if the keyval->field > datum->field
    then we return +1.
    If we are still here, then the field values are the same,
    and we drop to the next field for comparison.
*/
{
    // pointers to ise_series_xt component of the gen_buf_node's we were given
    ise_series_info_pt	keyval, datum;
    
    // *keybuf == pointer to a gen_buf_node_xt (an element of the sorted array)
    // **keybuf == a gen_buf_node_xt
    gen_buf_node_xt * keybuf_ptr, * datumbuf_ptr;
    keybuf_ptr = *keybuf;
    keyval = (ise_series_info_pt) keybuf_ptr->msg_p;
    datumbuf_ptr = *datumbuf;
    datum = (ise_series_info_pt) datumbuf_ptr->msg_p;

    // compare country codes
    if ( keyval->ise_series_x.country_u < datum->ise_series_x.country_u )
	return -1;
    else if ( keyval->ise_series_x.country_u > datum->ise_series_x.country_u )
	return +1;
    // if we get here, we have equal field values. Progress to the next field

    // compare market codes
    if ( keyval->ise_series_x.market_u < datum->ise_series_x.market_u )
	return -1;
    else if ( keyval->ise_series_x.market_u > datum->ise_series_x.market_u )
	return +1;
    // if we get here, we have equal field values. Progress to the next field

    // NB this is the OPPOSITE comparison order to cmp_series_ptr_by_ise_series_under
    // compare instrument group (e.g. American Calls, vs European puts...)
    if ( keyval->ise_series_x.instrument_group_u < datum->ise_series_x.instrument_group_u )
	return -1;
    else if ( keyval->ise_series_x.instrument_group_u > datum->ise_series_x.instrument_group_u )
	return +1;
    // if we get here, we have equal field values. Progress to the next field

    // compare underlying == commodity_code (typically == equity)
    if ( keyval->ise_series_x.underlying_code_u < datum->ise_series_x.underlying_code_u )
	return -1;
    else if ( keyval->ise_series_x.underlying_code_u > datum->ise_series_x.underlying_code_u )
	return +1;
    // if we get here, we have equal field values. Progress to the next field

    // compare modifier count. Typically 0, oaccasionally 1 or 2.
    if ( keyval->ise_series_x.modifier_count_u < datum->ise_series_x.modifier_count_u )
	return -1;
    else if ( keyval->ise_series_x.modifier_count_u > datum->ise_series_x.modifier_count_u )
	return +1;
    // if we get here, we have equal field values. Progress to the next field


    // compare expiry date year
    if ( keyval->ise_series_x.expiry_date_x.year_u < datum->ise_series_x.expiry_date_x.year_u )
	return -1;
    else if ( keyval->ise_series_x.expiry_date_x.year_u > datum->ise_series_x.expiry_date_x.year_u )
	return +1;
    // if we get here, we have equal field values. Progress to the next field

    // compare expiry date month
    if ( keyval->ise_series_x.expiry_date_x.month_u < datum->ise_series_x.expiry_date_x.month_u )
	return -1;
    else if ( keyval->ise_series_x.expiry_date_x.month_u > datum->ise_series_x.expiry_date_x.month_u )
	return +1;
    // if we get here, we have equal field values. Progress to the next field

    // compare expiry date day (of month)
    if ( keyval->ise_series_x.expiry_date_x.day_u < datum->ise_series_x.expiry_date_x.day_u )
	return -1;
    else if ( keyval->ise_series_x.expiry_date_x.day_u > datum->ise_series_x.expiry_date_x.day_u )
	return +1;
    // if we get here, we have equal field values. Progress to the next field

    // compare strike price (typically in cents)
    if ( keyval->ise_series_x.strike_price_i < datum->ise_series_x.strike_price_i )
	return -1;
    else if ( keyval->ise_series_x.strike_price_i > datum->ise_series_x.strike_price_i )
	return +1;
    // if we get here, we have equal field values. Progress to the next field

    // except we have run out of fields.. all field values match, so return "equal" value.
    return 0;
}   // cmp_series_ptr_by_ise_series_instr

//-----------------------------------------------------------------------------------

gen_buf_node_xt * find_series_block_by_ise_series ( click_details_xt * click_px,
						    ise_series_xt * search_series_px )
/*
    routine which searches in the known series for all country / markets
    to find the gen_buf_node_xt which matches the series supplied.

    The _by_ise_series means that the search series data is already in ISE format,
    not raw CLICK format.

    Returns pointer to that structure, or NULL if no match found.

    NB this fn used the sorted array of pointers to series.
    This is set up when we finish LoadSeries.
    It uses this to use the stdlib fn bsearch, instead of a linear search.
    On average, CLICK has (in testing, and worse in prod'n presumably) about
    60000 series. This means it does 40000 comparisons, on average.
    By using bsearch (stdlib.h), this should reduce to approximately
    to 15 (n log 2) comparisons, or about 3000 times faster.
*/
{
    // pointer to a component of the known_series_q
    gen_buf_node_xt	* series_node_px;
    // result from bsearch, which is a pointer to a pointer to a gen_buf_node_xt, which contains the series
    gen_buf_node_xt	** series_data_ppx;
    // pointer to the ISE format data about one series, from the known_series_q
    ise_series_info_xt	* series_data_px = NULL;
    // pointer to structure for a single country / market.
    country_market_xt	* country_market_px;
    // we need to create a temporary ise_series_info_xt, and a pointer to it, so we
    // can call bsearch. Just have one here on the stack - saves allocate / deallocate.
    ise_series_info_xt	ise_search_series_x;
    // and a gen_buf_node_xt which contains a pointer to the ise_search_series_x;
    gen_buf_node_xt	gen_buf_node_search_series;

    // and a pointer to that gen_buf_node_xt
    gen_buf_node_xt *gen_buf_node_search_series_px = &gen_buf_node_search_series;
    // copy the search series into search key structure
    ise_search_series_x.ise_series_x = *search_series_px;
    // and set the <msg> pointer from the gen_buf_node_xt to point to the ise_search_series_x
    gen_buf_node_search_series.msg_p = (char *)&ise_search_series_x;

    // search for country-market structure which has the matching country and market codes
    // we only have one in ISE for now - so hardcode it
    country_market_px = &click_px->our_market_x;
    if ( country_market_px != NULL )
    {
	// match, we have the country market. Now do fast search within the sorted chain
	series_data_ppx = bsearch ( 
		    &gen_buf_node_search_series_px,			    // pointer to search record
		    &country_market_px->series_sorted_by_ise_series_under_p // pointer to first element of array
			->orig_data_px [ 0 ],
		    country_market_px->known_series_q.length_i, 	    // # of entries in the array
		    sizeof (ise_series_info_pt),			    // size of each element in the array
		    (int (*) (const void *, const void *)) cmp_series_ptr_by_ise_series_under );    // comparison fn
	// did we find a match ?
	if ( series_data_ppx != NULL )
	{
	    // match.

	    // what we have though is a pointer to an entry in the sorted array,
	    // which is in itself a pointer to the original gen_buf_node_xt
	    series_node_px = *series_data_ppx;

	    // returning pointer to this entry
	    return series_node_px;
	}   // test if bsearch found a match
    }	// test if we found a matching country / market

    // oops, no match - report this
    return NULL;
}   // find_series_block_by_ise_series

//-----------------------------------------------------------------------------------

ise_series_info_xt * find_series_by_ise_series ( click_details_xt * click_px,
						    ise_series_xt * search_series_px )
/*
    routine which searches in the known series for all country / markets
    to find the ise_series_xt which matches the series supplied.

    The _by_ise_series means that the search series data is already in ISE format,
    not raw CLICK format.

    Returns pointer to that structure, or NULL if no match found.

    Uses find_series_block_by_ise_series
*/
{
    // pointer to a component of the known_series_q
    gen_buf_node_xt	* series_node_px;
    // pointer to the ISE format data about one series, from the known_series_q
    ise_series_info_xt	* series_data_px = NULL;

    series_node_px = find_series_block_by_ise_series ( click_px, search_series_px );
    // did we find a match ?
    if ( series_node_px != NULL )
    {
	// match.

	// point to the series data in this Q entry
	series_data_px = (ise_series_info_pt)series_node_px->msg_p;

	// returning pointer to this entry
	return series_data_px;
    }   // test if bsearch found a match

    // oops, no match - report this
    return NULL;
}   // find_series_by_ise_series

//-----------------------------------------------------------------------------------

ise_series_info_xt * find_series_by_name_s ( click_details_xt * click_px,
						series_st search_series_s )
/*
Function:   find_series_by_name_s
Author:	    Neil Cossor
Modified:   20000403
Description:

    routine which searches in the known series for all country / markets
    to find the ise_series_xt which matches the series_s supplied.

    Returns pointer to that structure, or NULL if no match found.

    NB as per find_series_by_ise_series, this fn used the sorted array of pointers to series.
    This does order (n log 2) comparisons, rather than order (n/2).

    There are some specials...for supplied exact search string
    "%S"    use the default underlying name value (presuming it has previously been set)
    "%O"    use the series in the last order that was snapshot-saved
    "%T"    use the series in the last trade that was snapshot-saved


*/
{
    // pointer to a component of the known_series_q
    gen_buf_node_xt * series_node_px = NULL;
    // result from bsearch, which is a pointer to a pointer to a gen_buf_node_xt, which contains the series
    gen_buf_node_xt ** series_data_ppx;
    // pointer to the ISE format data about one series, from the known_series_q
    ise_series_info_xt	* series_data_px = NULL;
    // pointer to structure for a single country / market.
    country_market_xt	* country_market_px;

    // we need to create a temporary ise_series_info_xt, and a pointer to it, so we
    // can call bsearch. Just have one here on the stack - saves allocate / deallocate.
    ise_series_info_xt	ise_search_series_x;
    // and a gen_buf_node_xt which contains a pointer to the ise_search_series_x;
    gen_buf_node_xt gen_buf_node_search_series;
    // and a pointer to that gen_buf_node_xt
    gen_buf_node_xt *gen_buf_node_search_series_px = &gen_buf_node_search_series;

    // did we get a string to search for at all
    if (strlen (search_series_s)==0)
    {
	printf ("No <series name> specified\n");
	return NULL;
    }

    // is this the special.. the default series string
    if (strcmp ( search_series_s, "%S" ) == 0 )
    {
	// yes, we should use the default series
	series_data_px = find_series_by_name_s ( click_px, click_px->default_series_name_s );
	if ( series_data_px )
	{
	    // found the series - return to caller
	    return series_data_px;
	}
	else
	{
	    printf ("Invalid default series [%s] - no such series\n",
			&click_px->default_series_name_s );
	    return NULL;
	}   // test if could find the default series
    }	// test if the user wanted to use default series

    // is this the special.. series from the last snapshot-saved order
    if (strcmp ( search_series_s, "%O" ) == 0 )
    {
	// yes, we should use the default series - which may be NULL
	series_data_px = click_px->saved_order_x.series_px;
	return series_data_px;
    }	// test if the user wanted to use default series

    // copy the search series into search key structure
    strcpy ( (char *)&ise_search_series_x.ise_name_s,
		search_series_s );
    // and set the <msg> pointer from the gen_buf_node_xt to point to the ise_search_series_x
    gen_buf_node_search_series.msg_p = (char *)&ise_search_series_x;

    /* we don't know which country / market a series (by name) is in.
	While name MAY not be enforced as unique (by OM) within one
	country / market, it is almost certainly not enforced across
	multiple markets. ISE may choose operationally (they choose
	the series names) to make products uniquely named across
	multiple markets.. they may not. Verbally I have been told that
	this uniqueness WILL be enforced.. I have suggested that it
	be written in as such, and the external world told of this..

	If this uniqueness is not enforced, then searching by series name
	is NOT possible, without a market qualifier.

	For now, there is ONE country / market.
    */

    // point to the ONE known country market
    country_market_px = &click_px->our_market_x;

    // Do fast search within the sorted chain
    series_data_ppx = bsearch ( 
				&gen_buf_node_search_series_px, 		// pointer to search record
				&country_market_px->series_sorted_by_name_p	// pointer to first element of array
				    ->orig_data_px [ 0 ],
				country_market_px->known_series_q.length_i,	// # of entries in the array
				sizeof (ise_series_info_pt),			// size of each element in the array
				(int (*) (const void *, const void *)) cmp_series_ptr_by_name );    // comparison fn
    // did we find a match ?
    if ( series_data_ppx != NULL )
    {
	// match.

	// what we have though is a pointer to an entry in the sorted array,
	// which is in itself a pointer to the original gen_buf_node_xt
	series_node_px = *series_data_ppx;
	
	// point to the series data in this Q entry
	series_data_px = (ise_series_info_pt)series_node_px->msg_p;

	// returning pointer to this entry
	return series_data_px;
    }	// test if bsearch found a match

    // oops, no match - report this
    return NULL;
}   // find_series_by_name_s

// -------------------------------------------------------------------------------------

void sort_series_tables ( country_market_xt * country_market_px,
			    int replace_tables )
/*
Function:   sort_series_tables
Author:	    Neil Cossor
Modified:   990913
Description:

    We have just finished loading all info about series
    within a particular country and market.
    OR
    we just got a broadcast, which changed our previous tables.
    This could include an add or delete (so the # of entries in our sorted chains
    have changed), or a modify (so the sort order may be different).
    
    Generate the internal (sorted) lookup tables, which allow fast lookup
    of series.

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
    // temp, ptr to data about one series, default NULL
    gen_buf_node_xt	* series_node_px = NULL;

    if (replace_tables)
    {
	// first, must delete the old tables
	release_series_sort_tables ( country_market_px );
    }

    // how big the array of pointers needs to be. NB all pointers are the same size, so pick one for sizeof
    size_of_ptr = sizeof ( country_market_px->series_sorted_by_name_p );
    size_of_ptr_array = size_of_ptr * country_market_px->known_series_q.length_i;

    // allocate the space for the series pointer arrays
    country_market_px->series_sorted_by_name_p = (sorted_array_pt) malloc ( size_of_ptr_array );
    country_market_px->series_sorted_by_ise_series_under_p = (sorted_array_pt) malloc ( size_of_ptr_array );
    country_market_px->series_sorted_by_ise_series_instr_p = (sorted_array_pt) malloc ( size_of_ptr_array );

    // now fill each of these arrays with pointers to the unsorted series data
    // point to first in Q
    series_node_px = (gen_buf_node_xt *)country_market_px->known_series_q.chain.next;
    for ( i = 0; i < country_market_px->known_series_q.length_i; i++ )
    {
	// save ptr to series data, in the "by name" array of ptrs
	country_market_px->series_sorted_by_name_p->orig_data_px [i] = series_node_px;

	// save ptr to series data, in the "by ise_series_xt", underlying array of ptrs
	country_market_px->series_sorted_by_ise_series_under_p->orig_data_px [i] = series_node_px;

	// save ptr to series data, in the "by ise_series_xt", instrument_group array of ptrs
	country_market_px->series_sorted_by_ise_series_instr_p->orig_data_px [i] = series_node_px;

	// advance to the following Q entry, which may be NULL
	series_node_px = (gen_buf_node_xt *)series_node_px->chain.next;
    }

    // calculate how far apart our pointers are in the array
    size_of_ptr = sizeof (ise_series_info_pt);

    // now sort each of the arrays. First by name.
    qsort ( &country_market_px->series_sorted_by_name_p     // pointer to first element of array
		    ->orig_data_px [ 0 ],
		country_market_px->known_series_q.length_i,	    // # of entries in the array
		size_of_ptr,				    // size of each element in the array
		(int (*) (const void *, const void *)) cmp_series_ptr_by_name );    // comparison fn

    // and then by ise_series_xt, underlying as first key
    qsort ( &country_market_px->series_sorted_by_ise_series_under_p // pointer to first element of array
		    ->orig_data_px [ 0 ],
		country_market_px->known_series_q.length_i,	    // # of entries in the array
		size_of_ptr,				    // size of each element in the array
		(int (*) (const void *, const void *)) cmp_series_ptr_by_ise_series_under );	// comparison fn

    // and then by ise_series_xt, instrument group as first key
    qsort ( &country_market_px->series_sorted_by_ise_series_instr_p // pointer to first element of array
		    ->orig_data_px [ 0 ],
		country_market_px->known_series_q.length_i,	    // # of entries in the array
		size_of_ptr,				    // size of each element in the array
		(int (*) (const void *, const void *)) cmp_series_ptr_by_ise_series_instr );	// comparison fn

}   // sort_series_tables

// -------------------------------------------------------------------------------------

void point_series_at_underlying ( click_details_xt *click_px,
				    country_market_xt * country_market_px,
				    int replace_tables )
/*
Function:   point_series_at_underlying
Author:     Neil Cossor
Modified:   9907xx
Description:

    We have just finished loading all info about series AND underlying
    within a particular country and market
    OR
    we have just had an update to the series / underlying info (BI202, 204, 210 bcasts)

    Update pointer in each series, to point back at parent underlyings.
    This will save us having to look it up every time we reference things
    thru the series. For example, all ORDER functions require BIN and ORDERBOOK #.
    These are associated with the underlying.. but the orders are related to series..

    We can also report any series for which we do not know the underlying...

    We also add a link to the series into a chain hanging off the underlying.

    Note that in adding the links from the underlying TO the series, we are
    going thru the series in 'sorted by name' order, which means that if we
    then later go thru the series hanging off an underlying, we will be doing so
    in alphabetical order...

Input Params:

    pointer to everything we know about the CDB
    pointer to everything we know about one particular country / market
    flag if we are replacing the tables. TRUE means replacing, FALSE means new tables
*/
{
    // pointer to actual ise info about a series
    ise_series_info_xt	*series_data_px = NULL;
    // pointer to chained block, which has a pointer (msg_p) to the info about a series
    gen_buf_node_xt	*series_node_px = NULL;
    // pointer to the underlying data block related to the series...
    ise_underlying_pt	underlying_px = NULL;
    // Q'able node, which contains a msg_p pointer to an series, which we chain into the underlying
    gen_buf_node_xt	    *under_series_node_px = NULL;
    // local loop var
    unsigned		    item_index_u;

    if (replace_tables)
    {
	// first, must delete the old lists of series from the underlying
	release_underlying_series_chains ( country_market_px );
    }

    for ( item_index_u = 0;
	    item_index_u < country_market_px->known_series_q.length_i;
	    item_index_u++ )
    {
	// point to first known series
	series_node_px = country_market_px->series_sorted_by_name_p->orig_data_px [ item_index_u ];

	// point to the interesting (i.e. series part) of the Q'ed block
	series_data_px = (ise_series_info_xt *)series_node_px->msg_p;

	// find the underlying related to this series
	underlying_px = find_under_data_by_commodity ( click_px,
							series_data_px
							    ->ise_series_x.underlying_code_u );
	// update the pointer from the series to the parent underlying - if there was one
	if ( underlying_px )
	{
	    // point the series back at the underlying
	    series_data_px->parent_under_px = underlying_px;

	    // now, link a pointer to the series into a Q belonging to the underlying
	    // allocate a (Q'able) entry to store pointer to series
	    under_series_node_px = new_gen_buf_node ( 0 );
	    // and make its msg_p pointer actually point to the series
	    under_series_node_px->msg_p = (char *)series_data_px;
	    // and add this node to the Q for this bin
	    add_to_q_tail ( &underlying_px->under_series_hdr_x,
				&under_series_node_px->chain );
	}
	else
	{
	    printf ("\tERROR (point_series_at_underlying) - Underlying code %d NOT found\n",
			series_data_px->ise_series_x.underlying_code_u );
	}   // test if we found a related underlying
    }	// while we have not run out of Q'ed chain of known series

}   // point_series_at_underlying

// -------------------------------------------------------------------------------------

ise_series_info_xt *user_selected_series ( click_details_xt * click_px,
					    char *user_series_argv_s )
/*
Function:   user_selected_series
Author:     Neil Cossor
Modified:   9907xx
Description:

    The user has supplied (in a command) a series selector (or is supposed to have).
    Try to find this series, and return a pointer to our (internal) data
    about that series.

    This routine can handle a series string of "%s",
    which means it will use the default series previously selected
    with the SET command (which will be NUL, if no default selected).

Input Params:

    The string which the user supplied in the command line.
    This should be a series name OR "%s" OR "%o"

Output Params:

    The pointer to the selected series, if found,
    otherwise NULL.

    Reports an error to stdout, if the series is unknown

To do:

    substitution for default "%s" series selector
*/
{
    ise_series_info_xt	*series_info_px;

    series_info_px = find_series_by_name_s ( click_px, user_series_argv_s );
    if (series_info_px == NULL)
    {
	// couldn't find that series - report error
	printf ("Error - unknown series %s\n",
			user_series_argv_s );
    }

    return series_info_px;
}   // user_selected_series

// -------------------------------------------------------------------------------------

void list_series ( click_details_xt *click_px,
		    int argc,
		    char *argv [] )
/*
Function : list_series
Author	 : Neil Cossor
Modified : 20000425
Description:

    Opens an output file (series.txt), and lists the known series,
    and closes the files.
    The series are listed with the ISE symbol, and OPRA symbol
    (with expiry and strike chars), one per line.

    The series are a combination of the static data retrieved
    at logon by dq202, and the broadcasts bi202.

Input Params:-

    argv[0]	LIST command that called us
    argv[1]	SERIES, that got us here
*/
{
    // pointer to actual ise info about a series
    ise_series_info_xt	*series_data_px;
    // pointer to chained block, which has a pointer (msg_p) to the info about a series
    gen_buf_node_xt	*series_node_px;
    // loop vars
    unsigned		item_index_u;
    FILE *listfile_px;		// pointer to file for output

    // nothing to do if no series in memory
    if ( click_px->our_market_x.series_sorted_by_name_p == NULL )
    {
	printf (" There are no series in memory\n");
	return;
    }

    // output file for series list - wipe existing file
    listfile_px = fopen ( "series.txt", "w");

    if ( listfile_px == 0 )
    {
	printf ("error creating output series listfile\n");
	return;
    }

    
    // dump them in alphabetical order (by ISE series ID)
    for (item_index_u=0;
	    item_index_u<click_px->our_market_x.known_series_q.length_i;
	    item_index_u++)
    {
	// point to actual block in the series chain
	series_node_px = (gen_buf_node_pt)click_px->our_market_x
				.series_sorted_by_name_p
				    ->orig_data_px [ item_index_u ];
	// point to the actual series info
	series_data_px = (ise_series_info_xt *)series_node_px->msg_p;

	// spit out one line.. ISE symbol is in fixed space, with a space
	fprintf ( listfile_px, "%-*s %s%c%c\n",
		    sizeof ( series_data_px->ise_name_s ),
		    series_data_px->ise_name_s,
		    series_data_px->ise_symbol_name_s,
		    series_data_px->ise_month_c,
		    series_data_px->ise_strike_c );

    }   // for loop, thru all stocks

    printf ("Listed %u items\n", click_px->our_market_x.known_series_q.length_i );

    fclose ( listfile_px );

}   // list_series


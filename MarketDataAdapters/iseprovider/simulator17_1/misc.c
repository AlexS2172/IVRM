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


Title : misc.c
Author: Neil Cossor

Function :

    This module handles some minor aspects of the OM/ISE data for the ISEsim.exe,
    which do not really belong to any of the major components, do not really
    belong in the utility.h/c, and are too small to require a module each.

    This includes things like routines for handling
    - bins
    - exchanges

Revision history :-

yymmdd	vers'n	author	    notes
------	------	------	    -----
9907xx	v0.00	Neil Cossor initial version
990913	v0.01		    fixed bug in send_query_opra_each, which did not init last_series_data_px
990928			    fix bug - we did NOT correctly halt on segment query, when returned a 0 segment #
			    enhance display_bin to show underlying status
991201	v0.02		    enhance display_bin to show commodity code of underlying
000110			    add code to use new uq15 query
000321	v0.03		    enhance display_bin and calc_bin_orderbook_combos to allow for bins
			    being split across multiple orderbooks servers.
000403	v0.04		    add fn's to use uq11 (query_missed_general_text, which queries previous BI8s)
			    and uq12 (query_business_date)..now we know they are in. Still lacking OM docs.
			    fix a couple of minor things in query_market_status.
			    mod send_query_opra_each, send_query_opra_best to report all records found
			    mod query_opra to allow for query for all series in 1 bin, in all bins
000422	v1.00		    update to match OM release 15.7-3 (move of some fields from da203 to da210).
				update send_query_opra_best, send_query_opra_each
000505	v1.01		    update query_market_status.. better info, now I know how it works properly
0102xx	v1.02		    updates for 16.3
			    change name of query_market_state to query_instrument_status
010604	v1.03		    merger in 15.7 retro calc_our_default_bin
*/
#include "stdincl.h"

#include "instrument.h"
#include "misc.h"
#include "series.h"
#include "underlying.h"
#include "utility.h"

// ---------------------------------------------------------------------------

gen_buf_node_xt * find_bin ( country_market_xt *country_market_px,
				uint8 search_bin_u )
/*
Function:   find_bin
Author:     Neil Cossor
Modified:   9907xx
Description:

    We wish to find the (internal) record about a nominated bin, within
    a nominated country/market.

    Return a pointer to the found record,
    unless we can't find it, in which case return NULL

    As we expect a very small number of bins (i.e. 10),
    just do a linear search, which will check against half of the bins, on average.
*/
{
    gen_buf_node_xt	*bin_node_px;
    ise_bin_xt		*bin_data_px;

    bin_node_px = (gen_buf_node_xt *)country_market_px->known_bins_q.chain.next;
    while ( bin_node_px )
    {
	// break out if we find a match.. the forward block of the last block will be NULL
	bin_data_px = (ise_bin_xt *)bin_node_px->msg_p;

	if (bin_data_px->bin_u == search_bin_u )
	// a match, out of the loop
	    break;

	bin_node_px = (gen_buf_node_xt *)bin_node_px->chain.next;
    }

    return bin_node_px;
}   // find_bin

// ---------------------------------------------------------------------------

void display_exchanges ( click_details_xt * click_px )
/*
Function:   display_exchanges
Author:     Neil Cossor
Modified:   9907xx
Description:

    Show the info we have about known exchanges.
    This includes equities exchanges,
    as well as derivaties exchanges
*/
{
    // pointer to a (Q'ed node), containing a pointer to info about one exchange
    gen_buf_node_xt	*exchange_node_px;
    // point to actual info about one exchange
    ise_exchange_xt	*exchange_data_px;

    // point to first known exchange node
    exchange_node_px = (gen_buf_node_xt *)click_px->our_market_x.known_exchanges_q.chain.next;
    if ( exchange_node_px)
	printf ("Exchange  Full name                        Country OPRA\n"
		"                                           code    indicator\n" );
    // loop thru all known exchanges, which may, of course, be none
    while ( exchange_node_px )
    {
	// point to actual data about one exchange
	exchange_data_px = (ise_exchange_xt *)exchange_node_px->msg_p;

	// spit out the info about one exchange
	printf ("%-10s%-32s %hu      %hu\n",
		&exchange_data_px->short_name_s,
		&exchange_data_px->long_name_s,
		exchange_data_px->country_u,
		exchange_data_px->opra_indicator_u );
	// advance to the next known exchange - and, of course, there mightn't be a next
	exchange_node_px = (gen_buf_node_xt *)exchange_node_px->chain.next;
    }	// while ( exchange_node_px )

    printf ("\n");
}   // display_exchanges

// ---------------------------------------------------------------------------

ise_exchange_xt * find_exchange_by_country ( click_details_xt * click_px,
						uint8 find_country_u )
/*
Function:   find_exchange_by_country
Author:     Neil Cossor
Modified:   9908xx
Description:

    We have a country #, and we need something else (say the abbreviated
    exchange name) for that... say, for display.

    Search for the appropriate exchange data (loaded at logon), and return
    pointer to that data.

    I assume that we only know about a small # of exchanges.. so just do linear
    search each time...

Input params:

    All the data we know about the click system
    The country # we are looking for.

Output params:

    Pointer to a block of exchange data, if we can find a match
    NULL if not.
*/
{
    // pointer to a (Q'ed node), containing a pointer to info about one exchange
    gen_buf_node_xt	*exchange_node_px;
    // point to actual info about one exchange
    ise_exchange_xt	*exchange_data_px;

    // point to first known exchange node
    exchange_node_px = (gen_buf_node_xt *)click_px->our_market_x.known_exchanges_q.chain.next;
    // loop thru all known exchanges, which may, of course, be none
    while ( exchange_node_px )
    {
	// point to actual data about one exchange
	exchange_data_px = (ise_exchange_xt *)exchange_node_px->msg_p;

	if ( exchange_data_px->country_u == find_country_u )
	    return exchange_data_px;

	// advance to the next known exchange - and, of course, there mightn't be a next
	exchange_node_px = (gen_buf_node_xt *)exchange_node_px->chain.next;
    }	// while ( exchange_node_px )

    // if we got here, we didn't find a match
    return NULL;
}   // find_exchange_by_country

// ---------------------------------------------------------------------------

char * exchange_to_string ( click_details_xt * click_px,
			    uint8 find_country_u )
/*
Function:   exchange_to_string
Author:     Neil Cossor
Modified:   9908xx
Description:

    We have a country #, and we want the abbreviated exchange name say, for display.

    Search for the appropriate exchange data (loaded at logon), and return
    pointer to string in that data.

Input params:

    All the data we know about the click system
    The country # we are looking for.

Output params:

    Pointer to a (short-form) string name, if we can find a match
    pointer to "Unknown exchange" if not.
*/
{
    // point to actual info about one exchange
    ise_exchange_xt	*exchange_data_px;

    exchange_data_px = find_exchange_by_country ( click_px, find_country_u );
    if (exchange_data_px)
	return (char *)&exchange_data_px->short_name_s;
    else
	return "Unknown exchange";
}   // find_exchange_by_country

// -------------------------------------------------------------------------------------

void calc_bin_orderbook_combos ( country_market_xt * country_market_px )
/*
Function: calc_bin_orderbook_combos
Author:   Neil Cossor
Modified: 20000321
Description:

    We have just finished loading all info about underlying
    within a particular country and market.
    
    Figure out how many bins/orderbook combinations there are in this country/market.

    This info can be used in wildcard loops, where we must make multiple
    requests to the various orderbook / bin combinations.

    We also update the (previously created) bin records...

Input Parameters:

    Address of block of data about one country/market. The info
    about the known underlyings, and the max bin and orderbook # we
    have seen in the static load are in here.

Output Parameters:

    Modifies data in the (given) country/market data block,
    in updating the field of the # of known bin/orderbook combinations,
    and in generating an array of data which contains the actual combos.

    We also update the (internal) bin records, so each has a Q of structures
    pointing to the component underlying...

Notes:

    We go thru the known underlying, using the array of pointers, sorted by name.
    This means that any stuff we generate as we go thru will be generated in
    that same order, i.e. alphabetic
*/
{
    // pointer to a (Q'ed) block of known underlyings,
    // which each contain a pointer to underlying data
    gen_buf_node_xt	*under_node_px;
    // and pointer to actual underlying data
    ise_underlying_xt	*under_data_px;

    // pointer to one (chained) node, in the known_bins_q
    gen_buf_node_xt	    *bin_node_px = NULL;
    // pointer to the data component of the node in the known_bins_q
    ise_bin_xt		    *bin_data_px = NULL;
    // Q'able node, which contains a msg_p pointer to an underlying, which we link into the bin
    gen_buf_node_xt	    *bin_under_node_px = NULL;
    // local loop var
    unsigned		    item_index_u;
    // for looping thry OB # in bin
    uint8		    ob_index_u;

    // and which we add to the Q for a particular bin

    if ( country_market_px->max_orderbook_u > country_market_px->max_bin_u )
    {
	// suspect
	printf ("\nWarning - more orderbooks [%d] than bins [%d]\n",
		country_market_px->max_orderbook_u,
		country_market_px->max_bin_u );
    }

    printf ("Found %d orderbooks, and %d bins\n",
		country_market_px->max_orderbook_u,
		country_market_px->max_bin_u );
	
    /* figure out how many orderbook / bin combinations there are.
	Should (in a 'normal' system) be the number of bins...refer to notes
	in global.h

	For now, assume the 'normal', and warn if not.
	If this is NOT the case, then we could (for example) make a worst
	case guess at bins x orderbooks combos, scan the underlyings filling
	in the combos we found, count how many non-zero combos we found,
	use that counted # as the actual #.
    */

    for ( item_index_u = 0;
	    item_index_u < country_market_px->known_underlying_q.length_i;
	    item_index_u++ )
    {
	// point to first known underlying
	under_node_px = country_market_px->underlying_sorted_by_name_p->orig_data_px [ item_index_u ];

	// point to the actual series info
	under_data_px = (ise_underlying_xt *)under_node_px->msg_p;

	// find the bin data, as nominated by the underlying
	bin_node_px = find_bin ( country_market_px,
				    under_data_px->bin_u );
	if ( !bin_node_px )
	{
	    // we have NOT heard anything about this bin, in the dq211 bins/pmms/mms... oops
	    printf ("Underlying %s is in bin %d, but we have no PMM/MM info for that bin\n",
		    &under_data_px->under_id_s,
		    under_data_px->bin_u );
	}
	else
	{
	    // point to actual data about this bin
	    bin_data_px = (ise_bin_xt *)bin_node_px->msg_p;

	    // search for this OB number in this bin
	    for ( ob_index_u = 0;
		    ob_index_u < MAX_OBS_FOR_BIN;
		    ob_index_u++ )
	    {
		// try another OB saved with this bin

		// compare the orderbook of this bin with the orderbook of this underlying..
		if ( bin_data_px->orderbook_u [ob_index_u] == under_data_px->orderbook_u )
		{
		    // found our OB (for this underlying) already in this bin's info.. done
		    break;
		}
		else
		{
		    // not matching OB.. is it some other OB, or are we adding an OB for the bin
		    if ( bin_data_px->orderbook_u [ob_index_u] == 0 )
		    {
			// we are adding an OB for the bin.. is there room ?
			if (bin_data_px->orderbooks_u < MAX_OBS_FOR_BIN)
			{
			    bin_data_px->orderbook_u [ob_index_u] = under_data_px->orderbook_u;
			    bin_data_px->orderbooks_u++;

			    // we are done.. get out of the loop
			    break;
			}
			else
			{
			    printf ("Warning, unable to add OB %u for underlying %s to bin %u, table full [%u entries]\n",
					under_data_px->orderbook_u,
					&under_data_px->under_id_s,
					bin_data_px->bin_u,
					MAX_OBS_FOR_BIN );
			}
		    }
		    else
		    {
			// different OB for this bin.. skip it, and continue looking thru list for this OB
		    }   // test if we actually had already set an orderbook #
		}	// test if matching orderbook as what the bin already had
	    }	// loop, searching in this bin for this OB
	}   // test if we knew PMMs/MMs etc for the bin nominated in the underlying

	// allocate a (Q'able) entry to store pointer to underlying
	bin_under_node_px = new_gen_buf_node ( 0 );
	// and make its msg_p pointer actually point to the underlying
	bin_under_node_px->msg_p = (char *)under_data_px;
	// and add this node to the Q for this bin
	add_to_q_tail ( &bin_data_px->bin_under_hdr_x,
			    &bin_under_node_px->chain );
    }	// while have ptr to another (Q'ed) node, containing ptr to underlying data


    // whiz around reporting any bins which are split across multiple orderbook servers
    bin_node_px = (gen_buf_node_xt *)country_market_px->known_bins_q.chain.next;
    while ( bin_node_px )
    {
	// break out if we find a match.. the forward block of the last block will be NULL
	bin_data_px = (ise_bin_xt *)bin_node_px->msg_p;

	// report any bins which have more one orderbook
	if (bin_data_px->orderbooks_u > 1)
	{
	    printf ("Warning - bin %d distributed over multiple orderbooks",
			bin_data_px->bin_u );

	    for ( ob_index_u = 0;
		    ob_index_u < bin_data_px->orderbooks_u;
		    ob_index_u++ )
	    {
		printf (", %d",
			bin_data_px->orderbook_u [ ob_index_u ] );
	    }
	    printf ("\n");
	}

	// move onto next bin
	bin_node_px = (gen_buf_node_xt *)bin_node_px->chain.next;
    }
}   // calc_bin_orderbook_combos

// -------------------------------------------------------------------------------------

int32 display_bin ( click_details_xt * click_px,
		    int argc,
		    char *argv [] )
/*
Function:   display_bin
Author:     Neil Cossor
Modified:   20000321
Description:

    The user wishes to know about bins

    We can tell them the list of underlying in a bin,
    and for each underlying, how many series are defined.

    We can also tell them who the PMM and MMs are.

Input Params:

    argv[0]	command that called us
    argv[1]	the bin # we wish to display
*/
{
    // pointer to one (chained) node, in the known_bins_q
    gen_buf_node_xt	    *bin_node_px = NULL;
    // pointer to the data component of the node in the known_bins_q
    ise_bin_xt		    *bin_data_px = NULL;
    // the bin # the caller wants to sus out
    uint8		    bin_u;
    uint16		    item_index_u;
    // Q'able node, which contains a msg_p pointer to an underlying,
    gen_buf_node_xt	    *bin_under_node_px = NULL;
    // and pointer to actual underlying data
    ise_underlying_xt	    *under_data_px;
    // counter, used to figure out when to spit out line breaks
    unsigned		    test_line_u = 0;
    // for looping thry OB # in bin
    uint8		    ob_index_u;
#define UNDER_PER_LINE 3


    // enuff params ?
    if ( argc < 2 )
    {
	printf ("BIN,<bin #>\n" );
	return SUCCESSFUL;
    }
    else
	bin_u = (uint8) read_unsigned_limited ( argv[1],
						click_px->our_market_x.max_bin_u );

    // lets see if we know about this bin ?
    bin_node_px = find_bin ( &click_px->our_market_x,
				bin_u );

    // if we now aren't pointing to a bin block, then we must create one
    if ( !bin_node_px )
    {
	printf ("Unknown bin %d\n",
		    bin_u );
	return SUCCESSFUL;
    }

    // we found the bin - dump out the interesting stuff
    bin_data_px = (ise_bin_xt *)bin_node_px->msg_p;

    printf ("Bin %d, PMM %s, CMMs",
		bin_u,
		&bin_data_px->pmm_id_s );
    for ( item_index_u = 0;
	    item_index_u <= bin_data_px->mms_u;
	    item_index_u++ )
    {
	printf (" %s",
		    &bin_data_px->mm_ids_s [ item_index_u ] );
    }
    printf ( "\n" );

    // spit out headers for underlying info
    for (item_index_u = 0;
	    item_index_u < UNDER_PER_LINE;
	    item_index_u++ )
    {
	printf("%-15s %-6s %-8s %-5s   ",
		"Underyling",
		"Series",
		"Status",
		"Ccode");
    }
    printf ( "\n" );

    // now, display the underlying in the bin - point to the first
    bin_under_node_px = (gen_buf_node_xt *)bin_data_px->bin_under_hdr_x.chain.next;
    while ( bin_under_node_px )
    {
	// point to the underlying
	under_data_px = (ise_underlying_xt *)bin_under_node_px->msg_p;

	printf ("%-15s %6u %-8s %5d   ",
		&under_data_px->under_id_s,
		under_data_px->under_series_hdr_x.length_i,
		underlying_status_to_string (under_data_px->trading_status_u),
		under_data_px->commodity_code_u
		);

	test_line_u++;
	if ( ((test_line_u / UNDER_PER_LINE ) * UNDER_PER_LINE ) ==  test_line_u )
	{
	    printf ("\n");
	}
	// try for the next
	bin_under_node_px = (gen_buf_node_xt *)bin_under_node_px->chain.next;
    }	// while we have a underlying node in the bin's chain


    printf ("\n" );


    // now report the orderbook(s) involved
    printf ("Orderbook server # for this bin",
		bin_data_px->bin_u );

    for ( ob_index_u = 0;
	    ob_index_u < bin_data_px->orderbooks_u;
	    ob_index_u++ )
    {
	printf (", %d",
		bin_data_px->orderbook_u [ ob_index_u ] );
    }
    printf ("\n");

    return SUCCESSFUL;
}   // display_bin

// ---------------------------------------------------------------------------

static int32 send_query_opra_best ( click_details_xt * click_px,
				    ise_series_xt *ise_series_px,
				    uint8 bin_u )
/*
Function:   send_query_opra_best
Author:     Neil Cossor
Modified:   20010304
Description:

    Does the query OPRA prices iq111 transaction.

Input Params:

    - pointer to structure with everything we know about the CLICK system
    - pointer to series selector to use
    - bin # to query

Output Params:

    the status / count of records found
    -ve means a bad status
    +ve (or 0) means a count of records found
*/
{
    int 		status_i = SUCCESSFUL;
    // temp loop var, rolling thru series within a response
    uint16		item_count_u,
			item_limit_u;
    // segmented query, get the next segment
    uint16		segment_u = 0;
    // the message we send to central system
    generic_txn_xt	OMmsg_x;
    // size of our actual CLICK query/command
    uint32		tx_length_u = sizeof ( OMmsg_x.tx_x.query_opra_prices_x );
    // pointer to one record of interesting info in the response
    answer_opra_prices_ia111_item_t *txn_reply_px;
    // the series we got in an answer record
    ise_series_xt	ise_series_x;
    // pointer to data about the series in response
    ise_series_info_xt	*series_data_px = NULL;
    // the premium (in local endian order) - used for bid, ask, hi, lo, last
    int32		premium_u;
    // same thing, in string form
    series_premium_st	premium_s;

    // the turnover
    uint32		turnover_u,
    // and the volume of the last transaction
			volume_u;
    int32		items_found_i = 0;



    // zot our complete query - this includes setting a 'series' selector of 0's
    memset ( &OMmsg_x.tx_x, 0, tx_length_u );

    // save the facility code and fn code
    OMmsg_x.fn_code = LOG_K_Q_OPRA_PRICES;		// OPRA prices (summary) query
    OMmsg_x.facility_u = click_px->facility_no_u + 1;	// EP1/ET1

    // put in the series selector as specified
    put_click_series ( (struct series *)&OMmsg_x.tx_x.query_opra_prices_x.search_series,
			ise_series_px );
    // the series in the header must contain the underlying from the series, search
    put_click_series ( (struct series *)&OMmsg_x.tx_x.query_opra_prices_x.series,
			ise_series_px );
/*    OMmsg_x.tx_x.query_opra_prices_x.series.country_c =
	    ise_series_px->country_u;
    OMmsg_x.tx_x.query_opra_prices_x.series.market_c =
	    ise_series_px->market_u;
    PUTSHORT( OMmsg_x.tx_x.query_opra_prices_x.series.commodity_n,
	    ise_series_px->underlying_code_u );
*/
    // whatever bin the caller gave us - from the relevant underlying
    OMmsg_x.tx_x.query_opra_prices_x.bin_c = bin_u;

    pack_OM_header ( &OMmsg_x, tx_length_u, 'I', 'Q', 111 );

    // loop thru, getting data, until we run out
    do
    {
	segment_u++;
	PUTSHORT ( OMmsg_x.tx_x.query_opra_prices_x.segment_number_n,
		    segment_u );
	status_i = do_click_query_rx ( click_px,
					&OMmsg_x,
					tx_length_u,
					LOG_K_R_OPRA_PRICES);
	if ( status_i < SUCCESSFUL)
	{
	    // do nothing, error already reported by do_click_tx_rx
	    // just bail out of loop
	    break;
	}
	else
	{
	    // report results from query opra - how many items in response ?
	    PUTSHORT ( item_limit_u,
			click_px->log_reply_x.trans_reply_x.answer_opra_prices_x.items_n );
	    if ( item_limit_u )
	    {
		// note how many more we found
		items_found_i += item_limit_u;

		for ( item_count_u = 0;
			item_count_u < item_limit_u;
			item_count_u++ )
		{
		    txn_reply_px = &click_px->log_reply_x.trans_reply_x
				    .answer_opra_prices_x.item[item_count_u];
		    get_click_series ( &ise_series_x,
					&txn_reply_px->series );

		    // the response may roll on into other unrelated data - report it all

		    // find the series info
		    series_data_px = find_series_by_ise_series ( click_px,
								&ise_series_x );
		    // now have stuff we can report
		    printf ( "%-15s ",
				&series_data_px->ise_name_s );

		    // do the BBO bid - if there is one
		    if ( txn_reply_px->country_bid_c )
		    {
			PUTLONG ( premium_u, txn_reply_px->best_bid_premium_i );
//			PUTLONG ( quantity_u, txn_reply_px->best_bid_size_u ); NOT IN 17.1
			format_whole_decimal_s ( (char *)&premium_s,
						    premium_u,
						    series_data_px->instrument_class_px->dec_in_premium_u,
						    10 );	// width of 10
			printf ("Bid @%s @ %s (%s), ",
				&premium_s,
				exchange_to_string ( click_px, txn_reply_px->country_bid_c ),
				away_market_status_to_string ( txn_reply_px->away_market_status_bid_c )
				);
		    }   // test if any data
		    else
			printf ("Bid (no OPRA data) ");

		    // do the BBO ask - if there is one
		    if ( txn_reply_px->country_ask_c )
		    {
			PUTLONG ( premium_u, txn_reply_px->best_ask_premium_i );
//			PUTLONG ( quantity_u, txn_reply_px->best_ask_size_u ); NOT IN 17.1
			format_whole_decimal_s ( (char *)&premium_s,
						    premium_u,
						    series_data_px->instrument_class_px->dec_in_premium_u,
						    10 );	// width of 10
			printf ("Ask @%s @ %s (%s), ",
				&premium_s,
				exchange_to_string ( click_px, txn_reply_px->country_ask_c ),
				away_market_status_to_string ( txn_reply_px->away_market_status_ask_c )
				);
		    }   // test if any data
		    else
			printf ("Ask (no OPRA data) ");

		    // do the BBO high - if there is one
		    if ( txn_reply_px->country_high_c )
		    {
			PUTLONG ( premium_u, txn_reply_px->high_price_i );
			format_whole_decimal_s ( (char *)&premium_s,
						    premium_u,
						    series_data_px->instrument_class_px->dec_in_premium_u,
						    10 );	// width of 10
			printf ("Hi %s @ %s (%s), ",
				&premium_s,
				exchange_to_string ( click_px, txn_reply_px->country_high_c ),
				away_market_status_to_string ( txn_reply_px->away_market_status_high_c )
				);
		    }   // test if any data
		    else
			printf ("Hi (no OPRA data) ");

		    // do the BBO low - if there is one
		    if ( txn_reply_px->country_low_c )
		    {
			PUTLONG ( premium_u, txn_reply_px->low_price_i );
			format_whole_decimal_s ( (char *)&premium_s,
						    premium_u,
						    series_data_px->instrument_class_px->dec_in_premium_u,
						    10 );	// width of 10
			printf ("Lo %s @ %s (%s), ",
				&premium_s,
				exchange_to_string ( click_px, txn_reply_px->country_low_c ),
				away_market_status_to_string ( txn_reply_px->away_market_status_low_c )
				);
		    }   // test if any data
		    else
			printf ("Lo (no OPRA data) ");

		    // do the BBO last - if there is one
		    if ( txn_reply_px->country_last_c )
		    {
			// NB this also had a volume
			PUTLONG ( premium_u, txn_reply_px->last_price_i );
			format_whole_decimal_s ( (char *)&premium_s,
						    premium_u,
						    series_data_px->instrument_class_px->dec_in_premium_u,
						    10 );	// width of 10
			PUTLONG ( volume_u, txn_reply_px->volume_u );
			printf ("Last (%u) %s @ %s (%s), ",
				volume_u,
				&premium_s,
				exchange_to_string ( click_px, txn_reply_px->country_last_c ),
				away_market_status_to_string ( txn_reply_px->away_market_status_last_c )
				);
		    }   // test if any data
		    else
			printf ("Last (no OPRA data) ");

		    // and the turnover, with the EOL
		    PUTLONG ( turnover_u, txn_reply_px->turnover_u );
		    printf ("turnover %u\n",
				turnover_u );
		}   // for loop, processing response record items
	    }
	    else
	    {
		// retrieved no records, time to bail out
		break;
	    }	// test if any items in this response
	}   // test if successful query status

	/* regardless of whether there were any items in it, if the segment # in the response
	    is 0, then we got all the data there is. NB although this is endian, a binary
	    0 is still a 0 - don't have to do a PUTSHORT
	*/
	if ( click_px->log_reply_x.trans_reply_x.answer_opra_prices_x.segment_number_n == 0 )
	    // no more data - out of here
	    break;
    }	// loop, collecting segments of data
    while ( TRUE );

    if ( status_i < SUCCESSFUL)
	return status_i;
    else
	return items_found_i;
}   // send_query_opra_best

// ---------------------------------------------------------------------------

static int32 send_query_opra_each ( click_details_xt * click_px,
				    ise_series_xt *ise_series_px,
				    uint8 bin_u )
/*
Function:   send_query_opra_each
Author:     Neil Cossor
Modified:   20010304
Description:

    Does the query each OPRA prices iq112 transaction.

Input Params:

    - pointer to structure with everything we know about the CLICK system
    - pointer to series selector to use
    - bin # to query

Output Params:

    the status / count of records found
    -ve means a bad status
    +ve (or 0) means a count of records found
*/
{
    int 		status_i = SUCCESSFUL;
    // temp loop var, rolling thru series within a response
    uint16		item_count_u,
			item_limit_u;
    // segmented query, get the next segment
    uint16		segment_u = 0;
    // the message we send to central system
    generic_txn_xt	OMmsg_x;
    // size of our actual CLICK query/command
    uint32		tx_length_u = sizeof ( OMmsg_x.tx_x.query_each_opra_prices_x );
    // pointer to one record of interesting info in the response
    answer_each_opra_prices_ia112_item_t    *txn_reply_px;
    // the series we got in an answer record
    ise_series_xt	ise_series_x;
    // pointer to data about the series in response
    ise_series_info_xt	*series_data_px = NULL;
    // pointer to the last series we reported on..
    ise_series_info_xt	*last_series_data_px = NULL;
    // the premium (in local endian order) - used for bid, ask, hi, lo, last
    int32		premium_u;
    // same thing, in string form
    series_premium_st	premium_s;
    // the turnover
    uint32		turnover_u,
    // and the volume of the last transaction
			volume_u;
    int32		items_found_i = 0;



    // zot our complete query - this includes setting a 'series' selector of 0's
    memset ( &OMmsg_x.tx_x, 0, tx_length_u );

    // save the facility code and fn code
    OMmsg_x.fn_code = LOG_K_Q_EACH_OPRA_PRICES; 	// each OPRA prices
    OMmsg_x.facility_u = click_px->facility_no_u + 1;	// EP1/ET1

    // put in the series selector as specified
    put_click_series ( (struct series *)&OMmsg_x.tx_x.query_each_opra_prices_x.search_series,
			ise_series_px );
    // the series in the header must contain the underlying from the series, search
    OMmsg_x.tx_x.query_each_opra_prices_x.series.country_c =
	    ise_series_px->country_u;
    OMmsg_x.tx_x.query_each_opra_prices_x.series.market_c =
	    ise_series_px->market_u;
    PUTSHORT( OMmsg_x.tx_x.query_each_opra_prices_x.series.commodity_n,
	    ise_series_px->underlying_code_u );
    // whatever bin the caller gave us - from the relevant underlying
    OMmsg_x.tx_x.query_each_opra_prices_x.bin_c = bin_u;

    pack_OM_header ( &OMmsg_x, tx_length_u, 'I', 'Q', 112 );

    // loop thru, getting data, until we run out
    do
    {
	segment_u++;
	PUTSHORT ( OMmsg_x.tx_x.query_each_opra_prices_x.segment_number_n,
		    segment_u );
	status_i = do_click_query_rx ( click_px,
					&OMmsg_x,
					tx_length_u,
					LOG_K_R_EACH_OPRA_PRICES );
	if ( status_i < SUCCESSFUL)
	{
	    // do nothing, error already reported by do_click_tx_rx
	    // just bail out of loop
	    break;
	}
	else
	{
	    // report results from query opra - how many items in response ?
	    PUTSHORT ( item_limit_u,
			click_px->log_reply_x.trans_reply_x.answer_each_opra_prices_x.items_n );
	    if ( item_limit_u )
	    {
		// note how many more we found
		items_found_i += item_limit_u;

		for ( item_count_u = 0;
			item_count_u < item_limit_u;
			item_count_u++ )
		{
		    txn_reply_px = &click_px->log_reply_x.trans_reply_x
				    .answer_each_opra_prices_x.item[item_count_u];
		    get_click_series ( &ise_series_x,
					&txn_reply_px->series );

		    // the response may roll on into other unrelated data - report it all

		    // find the series info
		    series_data_px = find_series_by_ise_series ( click_px,
								&ise_series_x );
		    // now have stuff we can report.. is this the same as the last one
		    if ( last_series_data_px != series_data_px )
		    {
			// new series, put out the series name
			printf ( "%-16s",
				    &series_data_px->ise_name_s );
			// save the 'last' series with this new value
			last_series_data_px = series_data_px;
		    }
		    else
			printf ("%16s","");

		    if (txn_reply_px->country_c)
		    {
			// we have a country code
			printf ("%4s (%-12s) ",
				exchange_to_string ( click_px, txn_reply_px->country_c ),
				away_market_status_to_string ( txn_reply_px->away_market_status_c )
				);
		    }
		    else
		    {
			// no country code.. status is probably suspect too - spit it out as numeric
			printf ("Country code 0, status %u, ",
				    txn_reply_px->away_market_status_c );
		    }   // test if a country was specified

		    // do the BBO bid
		    PUTLONG ( premium_u, txn_reply_px->best_bid_premium_i );
//		    PUTLONG ( quantity_u, txn_reply_px->best_bid_size_u ); NOT IN 17.1
		    format_whole_decimal_s ( (char *)&premium_s,
						premium_u,
						series_data_px->instrument_class_px->dec_in_premium_u,
						10 );   // width of 10
		    printf ("Bid @%s, ",
			    &premium_s
			    );

		    // do the BBO ask
		    PUTLONG ( premium_u, txn_reply_px->best_ask_premium_i );
//		    PUTLONG ( quantity_u, txn_reply_px->best_ask_size_u ); NOT IN 17.1
		    format_whole_decimal_s ( (char *)&premium_s,
						premium_u,
						series_data_px->instrument_class_px->dec_in_premium_u,
						10 );   // width of 10
		    printf ("Ask @%s, ",
			    &premium_s
			    );

		    // do the BBO high
		    PUTLONG ( premium_u, txn_reply_px->high_price_i );
		    format_whole_decimal_s ( (char *)&premium_s,
						premium_u,
						series_data_px->instrument_class_px->dec_in_premium_u,
						10 );   // width of 10
		    printf ("Hi %s, ",
			    &premium_s
			    );

		    // do the BBO low
		    PUTLONG ( premium_u, txn_reply_px->low_price_i );
		    format_whole_decimal_s ( (char *)&premium_s,
						premium_u,
						series_data_px->instrument_class_px->dec_in_premium_u,
						10 );   // width of 10
		    printf ("Lo %s, ",
			    &premium_s
			    );

		    // do the BBO last
		    // NB this also had a volume
		    PUTLONG ( premium_u, txn_reply_px->last_price_i );
		    format_whole_decimal_s ( (char *)&premium_s,
						premium_u,
						series_data_px->instrument_class_px->dec_in_premium_u,
						10 );   // width of 10
		    PUTLONG ( volume_u, txn_reply_px->volume_u );
		    printf ("Last (%u) %s, ",
			    volume_u,
			    &premium_s
			    );

		    // and the turnover, with the EOL
		    PUTLONG ( turnover_u, txn_reply_px->turnover_u );
		    printf ("turnover %u\n",
				turnover_u );
		}   // for loop, processing response record items
	    }
	    else
	    {
		// retrieved no records, time to bail out
		break;
	    }	// test if any items in this response
	}   // test if successful query status

	/* regardless of whether there were any items in it, if the segment # in the response
	    is 0, then we got all the data there is. NB although this is endian, a binary
	    0 is still a 0 - don't have to do a PUTSHORT
	*/
	if ( click_px->log_reply_x.trans_reply_x.answer_each_opra_prices_x.segment_number_n == 0 )
	    // no more data - out of here
	    break;
    }	// loop, collecting segments of data
    while ( TRUE );

    if ( status_i < SUCCESSFUL)
	return status_i;
    else
	return items_found_i;
}   // send_query_opra_each

// ---------------------------------------------------------------------------

int32 query_opra ( click_details_xt * click_px,
			int best_u,
			int argc,
			char *argv[] )
/*
Function:   query_opra
Author:     Neil Cossor
Modified:   20000403
Description:

    We wish to know the current OPRA prices.. one of
    -the amalgamated best BBOs and last trade info of all exchanges),
	using the iq111 transaction.
    -BBOs and last trade info from each exchange
	using the iq112 transaction.


Input Params:

    -ptr to all the data we know about CLICK
    -whether we are collecting amalgamated best (iq111) or each (iq112)
	if this param (best_u) is TRUE, then do iq111
    -normal argc,argv[] params from command line

    argv[0]	OPRA
    argv[1]	BEST|EACH
    argv[2]	Underlying|Series
    argv[3]	<underlying name>|<series name>
    (optional)
    argv[4]	<instrument group>, if argv[2] is Underlying.. this -> instrument class

Output Params;

    Results put out on stdout.
*/
{
    int 		status_i = SUCCESSFUL;
    // pointer to the specific series we are querying on
    ise_series_info_xt	*series_data_px;
    // pointer to the specific underlying we are querying all the series on
    ise_underlying_xt	*under_data_px;
    // temp var, for selecting from possible options on cmd line
    int 		choice_i;
    // the series selector for our query
    ise_series_xt	ise_series_x;
    // a particular bin selected
    uint8		bin_u;

    // logged on ?
    if (!click_px->logged_on_i )
    {
	if (best_u)
	    prompt_logged_on ("OPRA iq111 query");
	else
	    prompt_logged_on ("Each OPRA iq112 query");
	return NOT_LOGGED_ON;
    }

    // enuff arguments ?
    if ( argc < 3 )
    {
	printf ("OPRA,%s,S[eries]|U[nderlying]|B[in]|*,<series name>|<underlying name>|<bin#>[,<instr group>]\n",
	    (best_u==TRUE) ? "BEST" : "EACH");
	return SUCCESSFUL;
    }

    memset ( &ise_series_x,
		0,
		sizeof (ise_series_x));
    // and which series / underlying we wish to work on...
    choice_i = select_string_option ( 4, &bin_under_series_s [0], argv[2] );
    switch ( choice_i )
    {
    case bin_under_series_series_e :
	// one series - search for that series
	series_data_px = find_series_by_name_s ( click_px, argv[3] );
	if (!series_data_px)
	{
	    printf ("Error - unknown series specified [%s]\n",
			argv[3] );
	    return SUCCESSFUL;
	}

	// copy the exact series qualifier
	memcpy ( &ise_series_x,
		    &series_data_px->ise_series_x,
		    sizeof (ise_series_x));
	// go do the query - this does appropriate dumping to stdout
	if (best_u)
	    status_i = send_query_opra_best ( click_px,
						&ise_series_x,
						series_data_px->parent_under_px->bin_u );
	else
	    status_i = send_query_opra_each ( click_px,
						&ise_series_x,
						series_data_px->parent_under_px->bin_u );

	break;
    case bin_under_series_under_e :
	// all series in one underlying - search for that underlying
	under_data_px = find_under_by_name_s ( click_px, argv[3] );
	if (!under_data_px)
	{
	    printf ("Error - unknown underlying specified [%s]\n",
			argv[3] );
	    return SUCCESSFUL;
	}

	ise_series_x.country_u = click_px->our_market_x.country_u;
	ise_series_x.market_u = click_px->our_market_x.market_u;
	ise_series_x.underlying_code_u = under_data_px->commodity_code_u;

	// did the caller specify an instrument group ? i.e. instrument class query, rather than underlying
	if (( argc >= 5) && ( argv[4] != NULL ))
	    ise_series_x.instrument_group_u = read_unsigned_limited ( argv[4], UNDEFINED_UINT8);
	// NB, default is wildcard, all instrument classes within this underlying

	// go do the opra query - this does appropriate dumping to stdout
	if (best_u)
	    status_i = send_query_opra_best ( click_px,
						&ise_series_x,
						under_data_px->bin_u );
	else
	    status_i = send_query_opra_each ( click_px,
						&ise_series_x,
						under_data_px->bin_u );

	break;

    case bin_under_series_bin_e :
	// all of one bins.. leave search series NULs

	// read the bin #
	bin_u = read_unsigned_limited ( argv[3], click_px->our_market_x.max_bin_u );
	if ( bin_u == 0 )
	{
	    printf ("Invalid bin selection %s\n",
			&argv[3] );
	    return INVALID_ARGUMENTS;
	}

	// go do the query - this does appropriate dumping to stdout
	if (best_u)
	    status_i = send_query_opra_best ( click_px,
						&ise_series_x,
						bin_u );
	else
	    status_i = send_query_opra_each ( click_px,
						&ise_series_x,
						bin_u );

	break;

    case bin_under_series_all_e :
	// all of all bins.. leave search series NULs

	// go do the query - this does appropriate dumping to stdout
	if (best_u)
	    status_i = send_query_opra_best ( click_px,
						&ise_series_x,
						0 );
	else
	    status_i = send_query_opra_each ( click_px,
						&ise_series_x,
						0 );

	break;

    default:
	printf ("Error - user must select one of S[eries] | U[nderlying] | B[in] | * \n");
    }	// switch on Series / Underlying selector

    // if we got some, report it.. NB errors (-ve value) have already been reported
    if ( status_i >= SUCCESSFUL )
	printf ("Query OPRA %s returned %d records\n",
	    (best_u==TRUE) ? "BEST" : "EACH",
		    status_i );

    return status_i;
}   // query_opra

// ---------------------------------------------------------------------------

ise_trading_state_xt * find_trading_state_by_number ( click_details_xt *click_px,
							uint16 trade_state_number_u )
/*
Function:   find_trading_state_by_number
Author:	    Neil Cossor
Modified:   20010301
Description:

    find data about a nominated trading state number
*/
{
    ise_trading_state_xt    *ise_trading_state_px = NULL;
    gen_buf_node_xt	    *node_px = NULL;

    node_px = (gen_buf_node_xt *)click_px->our_market_x.known_trading_states_q.chain.next;
    while ( node_px )
    {
	// break out if we find a match.. the forward block of the last block will be NULL
	ise_trading_state_px = (ise_trading_state_xt *)node_px->msg_p;

	if (ise_trading_state_px->state_number_u == trade_state_number_u )
	// a match, out of the loop
	    break;

	node_px = (gen_buf_node_xt *)node_px->chain.next;
    }

    return ise_trading_state_px;
}   // find_trading_state_by_number

// ---------------------------------------------------------------------------

char * state_number_to_name ( click_details_xt *click_px,
				uint16 trade_state_number_u )
/*
Function:   state_number_to_name
Author:	    Neil Cossor
Modified:   20010301
Description:

    For a given trading state number, return a pointer to the string name
*/
{
    // pointer to the  corresponding trading state
    ise_trading_state_xt    *ise_trading_state_px;
    // buffer to put in string to report unknown state
    char		    static out_buffer_s[60];

    // search for corresponding trading state
    ise_trading_state_px = find_trading_state_by_number ( click_px, trade_state_number_u );

    if (ise_trading_state_px == NULL)
    {
	sprintf ( out_buffer_s, "%u",
		    trade_state_number_u );
	return (char *)&out_buffer_s;
    }
    else
	return (char *)&ise_trading_state_px->state_id_s;
}   // state_number_to_name

// ---------------------------------------------------------------------------

void update_trading_state ( click_details_xt *click_px,
				ise_series_xt *ise_series_px,
				uint16 trade_state_number_u )
/*
Function:   update_trading_state
Author:	    Neil Cossor
Modified:   20010301
Description:

    We have update information (from a UQ15 query or a BI41 broadcast)
    for trading states.

    The calling code knows which the data came from, and calls this routine
    as many times as necessary, once for each 'series' which we have info for.

    The 'series' qualifier can be for
    - a market (country and market specified)
    - an instrument type (country, market, and instrument group specified)
    - an instrumnet class (country, market, instr group and commdity code specified)

Input:

    - pointer to everything we know about the market
    - the ISE-format 'series' which qualifies market, instr type, or instr class
    - the new trade state number (in local-endian format). These correspond
	to the state data we retrieved with DQ245.
*/
{
    // previous state number.. if 0, was not previously set
    uint16		previous_state_number_u;

    // only three legal possibilities.. market, instr type, instr class
    if ( ise_series_px->underlying_code_u == 0 )
    {
	// no underlying, must be instrument type or market
	if ( ise_series_px->instrument_group_u == 0 )
	{
	    // market - is it our primary market
	    if (( click_px->our_market_x.country_u == ise_series_px->country_u )
		&& ( click_px->our_market_x.market_u == ise_series_px->market_u ))
	    {
		// yay! Note the old status, and save the new status
		previous_state_number_u = click_px->our_market_x.state_number_u;
		click_px->our_market_x.state_number_u = trade_state_number_u;
	    }
	}
	else
	{
	    // instrument type
	    instrument_xt *ise_instrument_type_px;
	    ise_instrument_type_px = find_instr_group ( click_px,
							ise_series_px->instrument_group_u );

	    // note the old status, and save the new status
	    previous_state_number_u = ise_instrument_type_px->state_number_u;
	    ise_instrument_type_px->state_number_u = trade_state_number_u;
	}	// test for instrument group being specified
    }
    else
    {
	// new value for instrument class
	instrument_class_xt *ise_instr_class_px;

	ise_instr_class_px = find_instrument_class_by_series ( &click_px->our_market_x,
								ise_series_px );

	// Note the old status, and save the new status
	previous_state_number_u = ise_instr_class_px->state_number_u;
	ise_instr_class_px->state_number_u = trade_state_number_u;
    }   // test for underyling commodity code being specified

    // can we tidily report the state as more than a number.. maybe the name ?

    // report what we now know
    if ( previous_state_number_u == 0 )
    {
	// new state.. probably just did UQ15
	printf ("Trading state %s for %s\n",
		    state_number_to_name (click_px,trade_state_number_u),
		    series_wildcard_to_string ( click_px,
						ise_series_px ));
    }
    else
    {
	// change of state - probably from BI41
	printf ("Trading state for %s, from %s to %s\n",
		    series_wildcard_to_string ( click_px,
						ise_series_px ),
		    state_number_to_name (click_px, previous_state_number_u),
		    state_number_to_name(click_px, trade_state_number_u)
		);
    }	// test if change of state, or didn't have an old state
}   // update_trading_state

// ---------------------------------------------------------------------------

void display_trading_states ( click_details_xt *click_px,
				char *start_string_s )
/*
Function:   display_trading_states
Author:	    Neil Cossor
Modified:   20010301
Description:

    We want to show (some of) the known trading states.. and the flags
    that are controlled by that
*/
{
    // pointer to the actual data in the chain of known states (from the DQ245)
    ise_trading_state_xt    *ise_trading_state_px = NULL;
    // pointer to one node in chained (saved) data of known states
    gen_buf_node_xt	    *node_px = NULL;

    // start at the beginning
    node_px = (gen_buf_node_xt *)click_px->our_market_x.known_trading_states_q.chain.next;
    while ( node_px )
    {
	// break out if we find a match.. the forward block of the last block will be NULL
	ise_trading_state_px = (ise_trading_state_xt *)node_px->msg_p;

	// report if no select string specified
	// OR the state name starts with the nominated string
	if (( start_string_s == NULL )
	|| ( strstr ( (char *)&ise_trading_state_px->state_id_s, start_string_s) == 
		(char *)&ise_trading_state_px->state_id_s))
	{
	    // one to report
	    printf ("state #%u, name [%s], matching %u, FM lvl %u, EOT %s, FoK %s, Mkt Orders %s, Quotes %s, OB %s, Edited OB %s\n",
			ise_trading_state_px->state_number_u,
			&ise_trading_state_px->state_id_s,
			ise_trading_state_px->type_of_matching_u,
			ise_trading_state_px->fast_market_level_u,
			( ise_trading_state_px->trading_end_u == OM_YES ) ? "True" : "False",
			( ise_trading_state_px->fill_kill_allowed_u == OM_YES ) ? "True" : "False",
			( ise_trading_state_px->market_orders_allowed_u == OM_YES ) ? "True" : "False",
			( ise_trading_state_px->price_quotation_required_u == OM_YES ) ? "True" : "False",
			( ise_trading_state_px->ob_changes_avail_u == OM_YES ) ? "True" : "False",
			( ise_trading_state_px->edited_ob_changes_avail_u == OM_YES ) ? "True" : "False"
		    );

	}   // check if to report this trading state

	node_px = (gen_buf_node_xt *)node_px->chain.next;
    }

}   // display_trading_states

// ---------------------------------------------------------------------------

int32 query_instrument_status ( click_details_xt *click_px )
/*
Function:   query_instrument_status
Author:	    Neil Cossor
Modified:   20010226
Description:

    Query to find out the current instrument status.. this can include
    values for markets, for instrument types, or for instrument classes.

    We already have all of the DQ data, which defines the static definitions for
    markets, instrument types and instrument classes.

    Once we have the current statii from this query,
    we then have the BI41 broadcast to maintain the info...

    The UQ15 is meant to complement these.
*/
{
    int32 status_i = SUCCESSFUL;
    // temp loop var, rolling thru series within a response
    uint16		item_count_u,
			item_limit_u;
    // segmented query, get the next segment
    uint16		segment_u = 0;
    // the message we send to central system
    generic_txn_xt	OMmsg_x;
    // size of our actual CLICK query/command
    uint32		tx_length_u = sizeof ( OMmsg_x.tx_x.query_instr_status_x );
    // pointer to one record of interesting info in the response
    answer_instrument_status_item_t *txn_reply_px;
    // get the trading state #
    uint16		trade_state_number_u;
    // local copy of the 'series' we got the info for, in local byte-order etc
    ise_series_xt	ise_series_x;

    // zot our complete query
    memset ( &OMmsg_x.tx_x, 0, tx_length_u );

    // save the facility code and fn code
    OMmsg_x.fn_code = LOG_K_Q_SERIES_SEGMENTS;		// instrument status query, standard segmented series-based
    OMmsg_x.facility_u = click_px->facility_no_u + 1;	// EP1/ET1

    // put in the country and market as specified
    OMmsg_x.tx_x.query_instr_status_x.series.country_c =
	    click_px->our_market_x.country_u;
    OMmsg_x.tx_x.query_instr_status_x.series.market_c =
	    click_px->our_market_x.market_u;

    pack_OM_header ( &OMmsg_x, tx_length_u, 'U', 'Q', 15 );

    // loop thru, getting data, until we run out
    do
    {
	segment_u++;
	PUTSHORT ( OMmsg_x.tx_x.query_instr_status_x.segment_number_n,
		    segment_u );
	status_i = do_click_query_rx ( click_px,
					&OMmsg_x,
					tx_length_u,
					LOG_K_R_INSTR_STATUS);
	if ( status_i < SUCCESSFUL)
	{
	    // do nothing, error already reported by do_click_tx_rx
	    // just bail out of loop
	    break;
	}
	else
	{
	    // report results from query instrument status - how many items in response ?
	    PUTSHORT ( item_limit_u,
			click_px->log_reply_x.trans_reply_x.answer_instrument_status_x.items_n );
	    for ( item_count_u = 0;
		    item_count_u < item_limit_u;
		    item_count_u++ )
	    {
		// point to the stuff about one instrument
		txn_reply_px = &click_px->log_reply_x.trans_reply_x
				.answer_instrument_status_x.item[item_count_u];
		// grab the trading state number in local-endian order
		PUTSHORT ( trade_state_number_u, txn_reply_px->state_number_n );

		// grab all the stuff out of the series.. will contain some wildcards
		get_click_series ( &ise_series_x,
				    &txn_reply_px->series );
		// update saved data
		update_trading_state ( click_px,
					&ise_series_x,
					trade_state_number_u );
	    }   // loop, processing response records
	}   // check on status from query

	/* regardless of whether there were any items in it, if the segment # in the response
	    is 0, then we got all the data there is. NB although this is endian, a binary
	    0 is still a 0 - don't have to do a PUTSHORT
	*/
	if ( click_px->log_reply_x.trans_reply_x.answer_instrument_status_x.segment_number_n == 0 )
	    // no more data - out of here
	    break;
    }	// loop.. I doubt this would ever happen, as this would require more than 3000 instrument groups per market...
    while ( TRUE );

    return status_i;
}   // query_instrument_status

// ---------------------------------------------------------------------------

char * market_name ( click_details_xt *click_px,
			uint8 country_u,
			uint8 market_u )
/*
Function:   market_name
Author:	    Neil Cossor
Modified:   20010301
Description:

    The caller wants the name of the OM market (as qualified by the 
    country code == exchange code, and the market code)

    For the moment, where the ISE only has one market,
    we only have one to look for.
*/
{
    // static area into which we can write some interim output, and return the
    // address of to the caller. Potential problem if multiple threads call the code
    // but this is unlikely. A hack.. I know.. but useful
    static char out_buffer_s [60];

    /* this should really 'search' for the country and market, amongst
	those retrieved from the DQ207/DA207.
    */
    if (( click_px->our_market_x.country_u == country_u )
	&& ( click_px->our_market_x.market_u == market_u ))
    {
	return (char *)&click_px->our_market_x.name_s;
    }
    else
    {
	// unknown country / market
	sprintf ( out_buffer_s, "country %u market %u",
		    country_u,
		    market_u );

	return (char *)&out_buffer_s;
    }	// known country / market
}   // market_name

// ---------------------------------------------------------------------------

int32 query_missed_general_text ( click_details_xt *click_px )
/*
Function:   query_missed_general_text
Author:	    Neil Cossor
Modified:   200012xx
Description:

    Query the system, and retrieve previous BI8 (general text messages).
    These do not appear to be sequenced or timestamps.. so other than knowing
    they are previous.. we don't know from how long ago.
*/
{
    int32 status_i = SUCCESSFUL;
    // temp loop var, rolling thru items in packed arrays within a response
    uint16		bcast_count_u,
			line_limit_u,
			line_count_u;
    // segmented query, get the next segment
    uint16		segment_u = 0;
    // the message we send to central system
    generic_txn_xt	OMmsg_x;
    // size of our actual CLICK query/command
    uint32		tx_length_u = sizeof ( OMmsg_x.tx_x.query_past_general_text_x );
    // pointer to one record of interesting info in the response
    answer_bi8_broadcasts_sent_item1_t *txn_reply_px;
    // temp string, where we put text from msg.. OM strings NOT NUL-terminated
    free_text_80_st		temp_string;


    // zot our complete query
    memset ( &OMmsg_x.tx_x, 0, tx_length_u );

    // save the facility code and fn code
    OMmsg_x.fn_code = LOG_K_Q_PAST_GENERAL_TEXT;	// missed BI8, standard segmented series-based
    OMmsg_x.facility_u = click_px->facility_no_u + 1;	// EP1/ET1

    // put in the country and market as specified
    OMmsg_x.tx_x.query_instr_status_x.series.country_c =
	    click_px->our_market_x.country_u;
    OMmsg_x.tx_x.query_instr_status_x.series.market_c =
	    click_px->our_market_x.market_u;

    pack_OM_header ( &OMmsg_x, tx_length_u, 'U', 'Q', 11 );

    // loop thru, getting data, until we run out
    do
    {
	segment_u++;
	PUTSHORT ( OMmsg_x.tx_x.query_past_general_text_x.segment_number_n,
		    segment_u );
	status_i = do_click_query_rx ( click_px,
					&OMmsg_x,
					tx_length_u,
					LOG_K_R_PAST_GENERAL_TEXT);
	if ( status_i < SUCCESSFUL)
	{
	    // do nothing, error already reported by do_click_tx_rx
	    // just bail out of loop
	    break;
	}
	else
	{
	    // report results - how many items in response ?
	    for ( bcast_count_u = 0;
		    bcast_count_u < click_px->log_reply_x.trans_reply_x.answer_past_general_text_x.items_c;
		    bcast_count_u++ )
	    {
		// point to the stuff about one broadcast
		txn_reply_px = &click_px->log_reply_x.trans_reply_x
				.answer_past_general_text_x.item1[bcast_count_u];
		// how many text lines in this BI8
		PUTSHORT ( line_limit_u,
			    txn_reply_px->items_n );

		printf ( "Missed (i.e. past) text b'cast # %u, country %u, market %u, containing %u lines\n",
			    txn_reply_px->broadcast_number_n,
			    txn_reply_px->market_c,
			    txn_reply_px->country_c,
			    txn_reply_px->items_n );
		for ( line_count_u = 0;
			line_count_u < line_limit_u;
			line_count_u++ )
		{
		    rx_click_string ( temp_string,
					&txn_reply_px->item2 [line_count_u],
					sizeof ( txn_reply_px->item2 [line_count_u]));
		    printf ( "\t\t%s\n",
			    &temp_string );
		}   // for loop, processing lines in one missed BI8
	    }   // loop, processing response records i.e. BI8s
	}   // check on status from query

	/* regardless of whether there were any items in it, if the segment # in the response
	    is 0, then we got all the data there is. NB although this is endian, a binary
	    0 is still a 0 - don't have to do a PUTSHORT
	*/
	if ( click_px->log_reply_x.trans_reply_x.answer_past_general_text_x.segment_number_n == 0 )
	    // no more data - out of here
	    break;
    }	// loop.. I doubt this would ever happen, as this would require more than 3000 instrument groups per market...
    while ( TRUE );

    return status_i;
}   // query_missed_general_text

// ---------------------------------------------------------------------------

int32 query_business_date ( click_details_xt *click_px )
/*
Function:   query_business_date
Author:	    Neil Cossor
Modified:   20000403
Description:

    Query the system, and retrieve what date the central system thinks it is.
    Save this for use in CQ1 (query todays date), giving a warning if the
    central system has a different date from our local date.
*/
{
    int32 status_i = SUCCESSFUL;
    // the message we send to central system
    generic_txn_xt	OMmsg_x;
    // size of our actual CLICK query/command
    uint32		tx_length_u = sizeof ( OMmsg_x.tx_x.query_business_date_x );
    // temps, for getting the current date, to compare with response data
    time_t		tempTime_x;
    struct tm		*Now_px;
    date_st		current_date_s;


    // zot our complete query
    memset ( &OMmsg_x.tx_x, 0, tx_length_u );

    // save the facility code and fn code
    OMmsg_x.fn_code = LOG_K_Q_BUSINESS_DATE;		// business date.. no segments, no series
    OMmsg_x.facility_u = click_px->facility_no_u + 1;	// EP1/ET1

    // put in the country and market as specified
    OMmsg_x.tx_x.query_instr_status_x.series.country_c =
	    click_px->our_market_x.country_u;
    OMmsg_x.tx_x.query_instr_status_x.series.market_c =
	    click_px->our_market_x.market_u;

    pack_OM_header ( &OMmsg_x, tx_length_u, 'U', 'Q', 12 );

    // no segments, no series.. in fact, no data fields at all
    status_i = do_click_query_rx ( click_px,
				    &OMmsg_x,
				    tx_length_u,
				    LOG_K_R_BUSINESS_DATE );
    if ( status_i < SUCCESSFUL)
    {
	// do nothing, error already reported by do_click_tx_rx
	// just bail out of loop
	return status_i;
    }
    else
    {
	// extract date from response
	rx_click_string ( &click_px->current_business_date_s,
			    &click_px->log_reply_x.trans_reply_x.answer_business_date_x.business_date_s,
			    sizeof ( click_px->log_reply_x.trans_reply_x.answer_business_date_x.business_date_s ));
	// and extract omex version
	rx_click_string ( &click_px->omex_version_s,
			    &click_px->log_reply_x.trans_reply_x.answer_business_date_x.omex_version_s,
			    sizeof (&click_px->log_reply_x.trans_reply_x.answer_business_date_x.omex_version_s ));

	// report results

	// get today's date in YYYYMMDD format
	time(&tempTime_x);
	Now_px = localtime(&tempTime_x);
	strftime((char *)&current_date_s , 9,"%Y%m%d",Now_px );

	if ( strcmp ( current_date_s, click_px->current_business_date_s ) == 0 )
	    printf ("Current OM/ISE business date %s\n",
			&click_px->current_business_date_s );
	else
	{
	    // mismatch between local date and the system date
	    printf ("Warning - current OM/ISE business date %s is different from local date %s\n",
			&click_px->current_business_date_s,
			&current_date_s );
	}

	printf ("Omex version %s\n", &click_px->omex_version_s );
    }	// test for success of query

    return status_i;
}   // query_business_date

// ---------------------------------------------------------------------------

unsigned calc_our_default_bin ( click_details_xt *click_px )
/*
Function:   calc_our_default_bin
Author:	    Neil Cossor
Modified:   20010601
Description:

    We have all of the bin/MM data loaded, and we are have just logged on.
    Figure out the default bin for this member.

    If the same member is defined as a MM in multiple bins, it will find
    the first and use that as the default.

    The result bin is displayed, set in the global, and returned to the caller.

    If we are not configured as a MM in any bin, we will come up with a 0.
    No warning will be displayed, as this is normal for an EAM.

    This calculation of default used to be done as an integral part of
    the DQ211, which collects the bin/MM data. This turns out to be a problem
    if the autocdb flag is off, and we log on as a different member.
*/
{
    // pointer to Q'ed item with pointer to data about one bin
    gen_buf_node_xt	*bin_node_px;
    // pointer to data about one bin
    ise_bin_xt		*bin_data_px;
    // loop var for skimming thru CMMs
    unsigned		item_index_u;

    // start out without a default bin.. hopefully we will find one
    click_px->our_market_x.default_bin_u = 0;

    bin_node_px = (gen_buf_node_xt *)click_px->our_market_x.known_bins_q.chain.next;
    while ( bin_node_px )
    {
	// break out if we find a match.. the forward block of the last block will be NULL
	bin_data_px = (ise_bin_xt *)bin_node_px->msg_p;

	// if the PMM is us, then make this our default bin
	if ( strcmp ( bin_data_px->pmm_id_s, click_px->user_code_x.customer_id_s )== 0 )
	{
	    // this is us - report it
	    printf ("\nYou (%s) are PMM for bin %d\n",
			&click_px->user_code_x.customer_id_s,
			bin_data_px->bin_u );
	    // set our default bin
	    click_px->our_market_x.default_bin_u = bin_data_px->bin_u;

	    // we are done
	    return click_px->our_market_x.default_bin_u;
	}   // check if we are the PMM

	// nope, we weren't the PMM.. are we a CMM for the bin ?
	for ( item_index_u = 0;
		item_index_u <= bin_data_px->mms_u;
		item_index_u++ )
	{
	    // if this is us, then make this our default bin
	    if ( strcmp ( bin_data_px->mm_ids_s [item_index_u],
			    click_px->user_code_x.customer_id_s )== 0 )
	    {
		// this is us - report it
		printf ("\nYou (%s) are CMM for bin %d\n",
			    &click_px->user_code_x.customer_id_s,
			    bin_data_px->bin_u );

		// set our default bin
		click_px->our_market_x.default_bin_u = bin_data_px->bin_u;

		// we are done
		return click_px->our_market_x.default_bin_u;
	    }	// check if we are a CMM
	}   // loop, whipping thru the CMMs for this bin

	// try another bin
	bin_node_px = (gen_buf_node_xt *)bin_node_px->chain.next;
    }	// loop, chaining thru bin nodes

    // we failed.. we don't have a bin
    return 0;
}   // calc_our_default_bin


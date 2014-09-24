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


Title : mm.c
Author: Neil Cossor

Function :

    This file provides MarketMaker specific facilities to the ISESim.exe program.
    This includes getting / setting MM parameters,
    sending quotes, reporting Trades
    
Revision history :-

yymmdd	vers'n	author	    notes
------	------	------	    -----
9907xx	v0.00	Neil Cossor initial version
990907	v0.01		    fix some display bugs for quotes / MM / PMM parms, mixing up farm/firm/open qtys
990921	v0.02		    fix prompting for mm,set and pmm,set cmd prompts, which did not have double %%
990928			    fix bug - we did NOT correctly halt on segment query, when returned a 0 segment #
991019	v0.03		    fix bug in send_query_quote.. did NOT fill in whose field, so no quotes found
991021			    add warning in query_orderbook when underlying is known, but no series
991102	v0.04		    mod to provide default value to cmd_strike_price_group
991201	v0.05		    add query_locked_orders
000105			    15.7, update max # of components in block quote
000204	v0.06		    move include of process.h into stdincl.h, but only if WinNT
000210			    fix bugs in set_mm_parameters, set_pmm_parameters which started with incorrect
			    index when setting params for single underlying.. how did this work before ?
000321	v0.07		    change query_quote, for * (all underlying) to cycle thru all orderbooks.
			    Same bug as in query_orders, in order.c
000328	v0.08		    fix query_quote, for *, which incorrectly reported # found based on last query
				same bug as query_orders, in order.c. Fix args prompt.
				mod to update in-memory quote stuff based on response data.
			    add routine to update quote details (in memory) for a single MO106 item
				(update_quoteIDs_sent)
			    new routine send_block_quote, called for both manual and autoquote.
			    new routine delete_quote
000330			    mod query_quote to remove query by underlying (as per order.c).
				This style of query is not really supported properly.
			    Fix specific-series query, which could roll into other orderbooks,
				due to common logic with all-series query.
000404			    fix send_query_locked_orders for multiple OB servers (as per changes to
				quote query above). Also, an old hard-coded debug value for OB #
				was lurking in here.
			    fix query_locked_orders to remove query by underlying option.
000422	v1.00		    update to match OM release 15.7-3 (move of some fields from da203 to da210).
				query_possible_opening, trade_report, send_query_quote,
				send_query_locked_orders, send_query_orderbook,
				add_quote_component, display_quote_table
000426	v1.01		    update send_query_orderbook to show order/quote ID of pieces
000505	v1.02		    fix bugs in set_pmm_parameters_under, set_mm_parameters_under
				which did NOT zero the buffer before packing
0012xx	v2.00		    start updates for OM release 16.3 (from 15.7-3)
010305	v2.01		    fix bug in query_pmm_parameters, which did NOT follow the underlying
				list in the bin, to retrieve the PMM params.
010320			    split request_opening, to call new send_opening, to cater for underyling
				with large # of series -> more than can be done in one transaction
010612	v2.02		    enhance send_query_orderbook to be able to do either a normal MQ103
				(external query) or the MQ359 (internal CIBOIS query). Also
				where to send the information controlled by extra param.
010618		    	    Separate count_gtc functionality (including BIN option) from
				normal query orderbook. Need new command.c option to call it.
010620			    mod count_gtc to include the seriesname/undername/bin# to filename if provided
010620			    mod request_opening to allow opening our whole bin (*)
010709			    add set_speed_parameters, get_speed_parameters for CMM/PMM
011015	v2.03		    update send_query_orderbook, count_gtc to be in step with enhancements
				made in 15.7 version (for counting by series, underlying, or bin)


To do :-
*/
#include "stdincl.h"

#include "error.h"
#include "instrument.h"
#include "series.h"
#include "underlying.h"
#include "utility.h"

#include "mm.h"


// -------------------- local consts and typedefs ----------------------------
/*
    the # of quote components that can be sent in one quote block.
    Note that while this SHOULD be in step with the # defined in the
    structure in the omex_usa.h file, if we set a higher # here,
    we may test some interesting boundaries of the OM system.
    As of OM 15.7, correct limit is now 192
*/
#define MAX_QUOTE_COMPONENTS	200
// how many items in MO113 / MQ114
#define MAX_OPEN_COMPONENTS	100

    // 'selectors' used in mm|pmm,set
    static char *set_under_selectors_s [] =
    {
	"%BIN",	    // my default bin (auto set for MM, can be manually set for EAM)
	"%FOLIO"    // a portfolio previously specified
		    // if we fall of end of list.. must be a specific underlying specified
    };
    // enumerated list, corresponding 1:1 (and in same order) as string list
    enum set_under_selectors_et { set_under_bin_e, set_under_folio_e };


// ---------------------- module (static) variables --------------------------

// the block quote we are building up to send.
// this is actually where we build up the "table" of quote components.
static generic_txn_xt	quote_msg_x;
/* the number of quote components we currently have
    Note that this has an initial value, which is critical to
    the initialisation logic for the structure.
*/
static unsigned 	quote_components_u = 0;

// -------------------------- internal routines ------------------------------

static int32 set_mm_parameters_under ( click_details_xt * click_px,
					ise_underlying_xt *under_data_px,
					uint8 strike_group_u,
					uint8 expiration_group_u,
					uint8 instr_group_u,
					uint8 isemm_fraction_u,
					uint8 firm_fraction_u,
					uint8 farmm_fraction_u,
					uint16 step_up_u,
					uint16 isemm_limit_absolute_u,
					uint16 firm_limit_absolute_u,
					uint16 farmm_limit_absolute_u,
					uint16 tick_worse_volume_an [8]
					)
/*
Function:   set_mm_parameters_under
Author:     Neil Cossor
Modified:   20010315
Description:

    MM wishes to set their MM parameters for one underlying
    Uses ui101.

    This normally sets the parameters for exactly one underlying, one strike group, one expiry group

    If wildcards (*) are used for expiry group or strike group, then we generate one
    SET record with the correct params, and then duplicate it the appropriate # of times,
    (picking up the firm, farm, step etc), and go and backfill the strike/expiry in each
    of those records.

    For example, a wildcard strike, with expiry group 1 would send 5 records, for strike DeepIn..DeepOut
    For a specific strike, with a wildcard expiry group, we would send 3 records (if there are 3
	expiry groups defined for the class).
    If both expiry group and strike group are both wildcarded, then we would send 15 records
	(for 3 expiry groups for the class)

Input Params:

    pointer to everything we know about the market etc
    pointer to the structure for the one underlying we are setting parms for
    the strike group # the user specified
    the expiration group #
    the instrument group #
    instr_group_u #
    the percentage limit for isemm quotes
    the percentage limit for firm quotes
    the percentage limit for farmm quotes
    the size of the step-up buffer regen
    the absolute limit for ismm quotes
    the absolute limit for firm quotes
    the absolute limit for farmm quotes
*/
{
    // the msg we will send to central system
    generic_txn_xt	OMmsg_x;
    // size of our actual CLICK query/command
    uint32		tx_length_u = sizeof ( OMmsg_x.tx_x.set_mm_parameters_x );

    int 		status_i = SUCCESSFUL;
    // pointer to the series we are doing this for
    ise_series_info_xt	*series_data_px = NULL;
    uint16		
    // loop var for processing the 8 levels of tick worse
			item_count_u,
    // how many items we will send in the command
			send_items_u = 0;

    // zot our complete query - this includes setting a 'series' selector of 0's
    memset ( &OMmsg_x.tx_x, 0, tx_length_u );

    // and the various parameters
    PUTSHORT ( OMmsg_x.tx_x.set_mm_parameters_x.item[0].mm_parameters.step_up_after_regen_buffer_n,
		step_up_u );
    PUTSHORT ( OMmsg_x.tx_x.set_mm_parameters_x.item[0].mm_parameters.isemm_trade_limit_absolute_n,
		isemm_limit_absolute_u );
    PUTSHORT ( OMmsg_x.tx_x.set_mm_parameters_x.item[0].mm_parameters.firm_trade_limit_absolute_n,
		firm_limit_absolute_u );
    PUTSHORT ( OMmsg_x.tx_x.set_mm_parameters_x.item[0].mm_parameters.farmm_trade_limit_absolute_n,
		farmm_limit_absolute_u );
    // 1-byte (non-endian) values
    OMmsg_x.tx_x.set_mm_parameters_x.item[0].mm_parameters.isemm_trade_limit_fraction_c = isemm_fraction_u;
    OMmsg_x.tx_x.set_mm_parameters_x.item[0].mm_parameters.firm_trade_limit_fraction_c = firm_fraction_u;
    OMmsg_x.tx_x.set_mm_parameters_x.item[0].mm_parameters.farmm_trade_limit_fraction_c = farmm_fraction_u;
    // read the 8 tick-worse values
    for ( item_count_u = 0;
	    item_count_u < 8;
	    item_count_u++ )
    {
	PUTSHORT ( OMmsg_x.tx_x.set_mm_parameters_x.item[0].mm_parameters.tick_worse_volume_an[item_count_u],
		    tick_worse_volume_an [item_count_u] );
    }

    // set the series
    OMmsg_x.tx_x.set_mm_parameters_x.item[0].series.country_c = click_px->our_market_x.country_u;
    OMmsg_x.tx_x.set_mm_parameters_x.item[0].series.market_c = click_px->our_market_x.market_u;
    PUTSHORT ( OMmsg_x.tx_x.set_mm_parameters_x.item[0].series.commodity_n,
		under_data_px->commodity_code_u );
    OMmsg_x.tx_x.set_mm_parameters_x.item[0].series.instrument_group_c = instr_group_u;

    // set the strike and expiration group - which may be 0 if wildcarded
    OMmsg_x.tx_x.set_mm_parameters_x.item[0].strike_price_group_c = strike_group_u;
    OMmsg_x.tx_x.set_mm_parameters_x.item[0].expiration_group_c = expiration_group_u;

    // how many items we are setting up...start with 1, and multiple accordingly
    send_items_u = 1;
    // ? wildcard strike group ?
    if ( strike_group_u == 0 )
    {
	// multiply by the # of strike groups - this is fixed
	send_items_u *= MAX_STRIKE_PRICE_GROUP;
    }
    // ? wildcard expiration group ?
    if ( expiration_group_u == 0 )
    {
	// multiply by the # of expiry groups - this is determined by the instrument classes
	// use one.. if the # in the classes were different, we have already warned the operator
	send_items_u *= 3;	// must pull this from the instrument classes ????
		// ???? must do more stuff here to get the instrument class ????
    }
    // so after all that, how many records will we send
    PUTSHORT ( OMmsg_x.tx_x.set_mm_parameters_x.items_n,
		send_items_u );

    // if we have more than one record, then duplicate all the info from the first,
    for ( item_count_u = send_items_u;
	    item_count_u > 1;
	    item_count_u-- )
    {
	// copy the unmodified data
	memcpy ( &OMmsg_x.tx_x.set_mm_parameters_x.item[item_count_u-1],
		    &OMmsg_x.tx_x.set_mm_parameters_x.item[0],
		    sizeof (OMmsg_x.tx_x.set_mm_parameters_x.item[0]));
    }	// for loop, setting up other SET records

    item_count_u = send_items_u - 1;
    // then go and backfill the expiry group / strike group items in each record
    if ( strike_group_u == 0 )
    {
	// wildcard within strike group
	for ( strike_group_u = MAX_STRIKE_PRICE_GROUP;
		strike_group_u > 0;
		strike_group_u-- )
	{
	    // how about wildcard within expiration group ?
	    if ( expiration_group_u == 0 )
	    {
		// wildcard for both expiration and strike group - 15 combinations
		for ( expiration_group_u = 3;
			expiration_group_u > 0;
			expiration_group_u-- )
		{
		    OMmsg_x.tx_x.set_mm_parameters_x.item[item_count_u].expiration_group_c = expiration_group_u;
		    OMmsg_x.tx_x.set_mm_parameters_x.item[item_count_u].strike_price_group_c = strike_group_u;

		    item_count_u--;
		}
	    }
	    else
	    {
		// wildcard for strike, fixed expiration group - 5 combinations
		OMmsg_x.tx_x.set_mm_parameters_x.item[item_count_u].expiration_group_c = expiration_group_u;
		OMmsg_x.tx_x.set_mm_parameters_x.item[item_count_u].strike_price_group_c = strike_group_u;

		item_count_u--;
	    }
	}   // for loop, thru strikes
    }
    else
    {
	// fixed strike group, how about wildcard within expiration group ?
	if ( expiration_group_u == 0 )
	{
	    // wildcard for expiration, fixed strike group - 3 combinations
	    for ( expiration_group_u = 3;
		    expiration_group_u > 0;
		    expiration_group_u-- )
	    {
		OMmsg_x.tx_x.set_mm_parameters_x.item[item_count_u].expiration_group_c = expiration_group_u;
		OMmsg_x.tx_x.set_mm_parameters_x.item[item_count_u].strike_price_group_c = strike_group_u;

		item_count_u--;
	    }
	}
	else
	{
	    // fixed for strike, fixed expiration group - 1 combination
	    OMmsg_x.tx_x.set_mm_parameters_x.item[item_count_u].expiration_group_c = expiration_group_u;
	    OMmsg_x.tx_x.set_mm_parameters_x.item[item_count_u].strike_price_group_c = strike_group_u;
	}
    }

    pack_OM_header ( &OMmsg_x, tx_length_u, 'U', 'I', 101 );

    // save the facility code and fn code
    OMmsg_x.fn_code = LOG_K_C_SET_MM_PARAMS;		// Set MM params command
    OMmsg_x.facility_u = click_px->facility_no_u;	// EP0/ET0

    /* copy the series from the first record into the series for the message
	This should NOT be necessary.. as you can send things in for multiple series,
	the series in the header should be NUL
    */
    memcpy ( &OMmsg_x.tx_x.set_mm_parameters_x.series,
		&OMmsg_x.tx_x.set_mm_parameters_x.item[0].series,
		sizeof (OMmsg_x.tx_x.set_mm_parameters_x.series));
    status_i = do_click_tx_rx ( click_px,
				&OMmsg_x,
				tx_length_u );
    if ( status_i < SUCCESSFUL)
    {
	// do nothing, error already reported by do_click_tx_rx
    }
    else
    {
	// successful
	printf ( "Successful Set MM parameters command for %s\n",
		    under_data_px->under_id_s );
    }

    return status_i;
}   // set_mm_parameters_under

// ---------------------------------------------------------------------------

static int32 set_pmm_parameters_under ( click_details_xt * click_px,
					ise_underlying_xt *under_data_px,
					uint8 strike_group_u,
					uint8 expiration_group_u,
					uint8 instr_group_u,
					uint8 isemm_fraction_u,
					uint8 firm_fraction_u,
					uint8 farmm_fraction_u,
					uint16 step_up_u,
					uint16 isemm_limit_absolute_u,
					uint16 firm_limit_absolute_u,
					uint16 farmm_limit_absolute_u,
					uint16 tick_worse_volume_an [8],
					uint16 derived_order_max_size_an [8],
					uint16 match_away_market_max_size_an [8]
				)
/*
Function:   set_pmm_parameters_under
Author:     Neil Cossor
Modified:   20000505
Description:

    PMM wishes to set their PMM parameters.
    Uses ui102.

    Exactly as per set_mm_parameters
    BUT
    there are an extra 16 params - the derived order quantities match-away quantities
    for 8 levels
*/
{
    // the msg we will send to central system
    generic_txn_xt	OMmsg_x;
    // size of our actual CLICK query/command
    uint32		tx_length_u = sizeof ( OMmsg_x.tx_x.set_pmm_parameters_x );

    int 		status_i = SUCCESSFUL;
    // pointer to the series we are doing this for
    ise_series_info_xt	*series_data_px = NULL;
    uint16		
			// loop var
			item_count_u,
			// how many items we will send in the command
			send_items_u = 0;

    // zot our complete query - this includes setting a 'series' selector of 0's
    memset ( &OMmsg_x.tx_x, 0, tx_length_u );


    // and the various parameters
    PUTSHORT ( OMmsg_x.tx_x.set_pmm_parameters_x.item[0].mm_parameters.step_up_after_regen_buffer_n,
		step_up_u );
    PUTSHORT ( OMmsg_x.tx_x.set_pmm_parameters_x.item[0].mm_parameters.isemm_trade_limit_absolute_n,
		isemm_limit_absolute_u );
    PUTSHORT ( OMmsg_x.tx_x.set_pmm_parameters_x.item[0].mm_parameters.firm_trade_limit_absolute_n,
		firm_limit_absolute_u );
    PUTSHORT ( OMmsg_x.tx_x.set_pmm_parameters_x.item[0].mm_parameters.farmm_trade_limit_absolute_n,
		farmm_limit_absolute_u );
    // 1-byte (non-endian) values
    OMmsg_x.tx_x.set_pmm_parameters_x.item[0].mm_parameters.isemm_trade_limit_fraction_c = isemm_fraction_u;
    OMmsg_x.tx_x.set_pmm_parameters_x.item[0].mm_parameters.firm_trade_limit_fraction_c = firm_fraction_u;
    OMmsg_x.tx_x.set_pmm_parameters_x.item[0].mm_parameters.farmm_trade_limit_fraction_c = farmm_fraction_u;
    for ( item_count_u = 0;
	    item_count_u < 8;
	    item_count_u++ )
    {
	// read the tick-worse values
	PUTSHORT ( OMmsg_x.tx_x.set_pmm_parameters_x.item[0].mm_parameters.tick_worse_volume_an[item_count_u],
		    tick_worse_volume_an [item_count_u] );
	// the 8 derived quantities
	PUTSHORT ( OMmsg_x.tx_x.set_pmm_parameters_x.item[0].pmm_parameters.derived_order_max_size_an [item_count_u],
		    derived_order_max_size_an [item_count_u] );
	// and the 8 match-away quantities
	PUTSHORT ( OMmsg_x.tx_x.set_pmm_parameters_x.item[0].pmm_parameters.match_away_market_max_size_an [item_count_u],
		    match_away_market_max_size_an [item_count_u] );
    }

    // set the series
    OMmsg_x.tx_x.set_pmm_parameters_x.item[0].series.country_c = click_px->our_market_x.country_u;
    OMmsg_x.tx_x.set_pmm_parameters_x.item[0].series.market_c = click_px->our_market_x.market_u;
    PUTSHORT ( OMmsg_x.tx_x.set_pmm_parameters_x.item[0].series.commodity_n,
		under_data_px->commodity_code_u );
    OMmsg_x.tx_x.set_pmm_parameters_x.item[0].series.instrument_group_c = instr_group_u;

    // set the strike and expiration group - which may be 0 if wildcarded
    OMmsg_x.tx_x.set_pmm_parameters_x.item[0].strike_price_group_c = strike_group_u;
    OMmsg_x.tx_x.set_pmm_parameters_x.item[0].expiration_group_c = expiration_group_u;

    // how many items we are setting up...start with 1, and multiple accordingly
    send_items_u = 1;
    // ? wildcard strike group ?
    if ( strike_group_u == 0 )
    {
	// multiply by the # of strike groups - this is fixed
	send_items_u *= MAX_STRIKE_PRICE_GROUP;
    }
    // ? wildcard expiration group ?
    if ( expiration_group_u == 0 )
    {
	// multiply by the # of expiry groups - this is determined by the instrument classes
	// use one.. if the # in the classes were different, we have already warned the operator
	send_items_u *= 3;	// must pull this from the instrument classes ????
		// ???? must do more stuff here to get the instrument class ????
    }
    // so after all that, how many records will we send
    PUTSHORT ( OMmsg_x.tx_x.set_pmm_parameters_x.items_n,
		send_items_u );

    // if we have more than one record, then duplicate all the info from the first,
    for ( item_count_u = send_items_u;
	    item_count_u > 1;
	    item_count_u-- )
    {
	// copy the unmodified data
	memcpy ( &OMmsg_x.tx_x.set_pmm_parameters_x.item[item_count_u-1],
		    &OMmsg_x.tx_x.set_pmm_parameters_x.item[0],
		    sizeof (OMmsg_x.tx_x.set_pmm_parameters_x.item[0]));
    }	// for loop, setting up other SET records

    item_count_u = send_items_u - 1;
    // then go and backfill the expiry group / strike group items in each record
    if ( strike_group_u == 0 )
    {
	// wildcard within strike group
	for ( strike_group_u = MAX_STRIKE_PRICE_GROUP;
		strike_group_u > 0;
		strike_group_u-- )
	{
	    // how about wildcard within expiration group ?
	    if ( expiration_group_u == 0 )
	    {
		// wildcard for both expiration and strike group - 15 combinations
		for ( expiration_group_u = 3;
			expiration_group_u > 0;
			expiration_group_u-- )
		{
		    OMmsg_x.tx_x.set_pmm_parameters_x.item[item_count_u].expiration_group_c = expiration_group_u;
		    OMmsg_x.tx_x.set_pmm_parameters_x.item[item_count_u].strike_price_group_c = strike_group_u;

		    item_count_u--;
		}
	    }
	    else
	    {
		// wildcard for strike, fixed expiration group - 5 combinations
		OMmsg_x.tx_x.set_pmm_parameters_x.item[item_count_u].expiration_group_c = expiration_group_u;
		OMmsg_x.tx_x.set_pmm_parameters_x.item[item_count_u].strike_price_group_c = strike_group_u;

		item_count_u--;
	    }
	}   // for loop, thru strikes
    }
    else
    {
	// fixed strike group, how about wildcard within expiration group ?
	if ( expiration_group_u == 0 )
	{
	    // wildcard for expiration, fixed strike group - 3 combinations
	    for ( expiration_group_u = 3;
		    expiration_group_u > 0;
		    expiration_group_u-- )
	    {
		OMmsg_x.tx_x.set_pmm_parameters_x.item[item_count_u].expiration_group_c = expiration_group_u;
		OMmsg_x.tx_x.set_pmm_parameters_x.item[item_count_u].strike_price_group_c = strike_group_u;

		item_count_u--;
	    }
	}
	else
	{
	    // fixed for strike, fixed expiration group - 1 combination
	    OMmsg_x.tx_x.set_pmm_parameters_x.item[item_count_u].expiration_group_c = expiration_group_u;
	    OMmsg_x.tx_x.set_pmm_parameters_x.item[item_count_u].strike_price_group_c = strike_group_u;
	}
    }

    pack_OM_header ( &OMmsg_x, tx_length_u, 'U', 'I', 102 );

    // save the facility code and fn code
    OMmsg_x.fn_code = LOG_K_C_SET_PMM_PARAMS;		// Set MM params command
    OMmsg_x.facility_u = click_px->facility_no_u;	// EP0/ET0

    /* copy the series from the first record into the series for the message
	This should NOT be necessary.. as you can send things in for multiple series,
	the series in the header should be NUL
    */
    memcpy ( &OMmsg_x.tx_x.set_pmm_parameters_x.series,
		&OMmsg_x.tx_x.set_pmm_parameters_x.item[0].series,
		sizeof (OMmsg_x.tx_x.set_pmm_parameters_x.series));
    status_i = do_click_tx_rx ( click_px,
				&OMmsg_x,
				tx_length_u );
    if ( status_i < SUCCESSFUL)
    {
	// do nothing, error already reported by do_click_tx_rx
    }
    else
    {
	// successful
	printf ( "Successful Set PMM parameters command for %s\n",
		    under_data_px->under_id_s );
    }

    return status_i;
}   // set_pmm_parameters_under

// ---------------------------------------------------------------------------

static int32 set_speedbump_parameters_under ( click_details_xt * click_px,
						ise_underlying_xt *under_data_px,
						uint32 trade_limit_u,
						uint16 spread_widen_percentage_u
					    )
/*
Function:   set_speedbump_parameters_under
Author:     Neil Cossor
Modified:   20010709
Description:

    CMM/PMM wishes to set their speedbump (mm exposure limit) parameters for one underlying
    Uses ui103.

    This normally sets the parameters for exactly one underlying

Input Params:

    pointer to everything we know about the market etc
    pointer to the structure for the one underlying we are setting parms for
    the # of traded contracts the user specified
    the widen % # the user specified
*/
{
    // the msg we will send to central system
    generic_txn_xt	OMmsg_x;
    // size of our actual CLICK query/command
    uint32		tx_length_u = sizeof ( OMmsg_x.tx_x.set_mm_exposure_limit_x );

    int 		status_i = SUCCESSFUL;
    // pointer to the series we are doing this for
    ise_series_info_xt	*series_data_px = NULL;

    // zot our complete query - this includes setting a 'series' selector of 0's
    memset ( &OMmsg_x.tx_x, 0, tx_length_u );

    // and the various parameters
    PUTLONG ( OMmsg_x.tx_x.set_mm_exposure_limit_x.item[0].curtailment_trading_threshold_u,
		trade_limit_u );
    PUTSHORT ( OMmsg_x.tx_x.set_mm_exposure_limit_x.item[0].curtailment_spread_factor_n,
		spread_widen_percentage_u );

    // set the series
    OMmsg_x.tx_x.set_mm_exposure_limit_x.item[0].series.country_c = click_px->our_market_x.country_u;
    OMmsg_x.tx_x.set_mm_exposure_limit_x.item[0].series.market_c = click_px->our_market_x.market_u;
    PUTSHORT ( OMmsg_x.tx_x.set_mm_exposure_limit_x.item[0].series.commodity_n,
		under_data_px->commodity_code_u );

    // one item
    OMmsg_x.tx_x.set_mm_exposure_limit_x.items_n = 1;

    pack_OM_header ( &OMmsg_x, tx_length_u, 'U', 'I', 103 );

    // save the facility code and fn code
    OMmsg_x.fn_code = LOG_K_C_SPEEDBUMP;		// Set speedbump params command
    OMmsg_x.facility_u = click_px->facility_no_u;	// EP0/ET0

    /* copy the series from the first record into the series for the message
	This should NOT be necessary.. as you can send things in for multiple series,
	the series in the header should be NUL
    memcpy ( &OMmsg_x.tx_x.set_mm_exposure_limit_x.series,
		&OMmsg_x.tx_x.set_mm_exposure_limit_x.item[0].series,
		sizeof (OMmsg_x.tx_x.set_mm_exposure_limit_x.series));
    */
    status_i = do_click_tx_rx ( click_px,
				&OMmsg_x,
				tx_length_u );
    if ( status_i < SUCCESSFUL)
    {
	// do nothing, error already reported by do_click_tx_rx
    }
    else
    {
	// successful
	printf ( "Successful Set Speedbump parameters command for %s\n",
		    under_data_px->under_id_s );
    }

    return status_i;
}   // set_speedbump_parameters_under

// -------------------------- external routines ------------------------------

int32 set_mm_parameters ( click_details_xt * click_px,
			    int argc,
			    char *argv[] )
/*
Function:   set_mm_parameters
Author:     Neil Cossor
Modified:   200101xx
Description:

    MM wishes to set their MM parameters.
    Uses ui101.

    This normally sets the parameters for exactly one underlying, one strike group, one expiry group

    If wildcards (*) are used for expiry group or strike group, then we generate one
    SET record with the correct params, and then duplicate it the appropriate # of times,
    (picking up the firm, farm, step etc), and go and backfill the strike/expiry in each
    of those records.

    For example, a wildcard strike, with expiry group 1 would send 5 records, for strike DeepIn..DeepOut
    For a specific strike, with a wildcard expiry group, we would send 3 records (if there are 3
	expiry groups defined for the class).
    If both expiry group and strike group are both wildcarded, then we would send 15 records
	(for 3 expiry groups for the class)

Input Params:

    standard argc/argv from the command line.. params are
    argv[0]	    MM
    argv[1]	    SET
    argv[2]	    <underlying name>, or *
    argv[3]	    <instrument group>, or *
    argv[4]	    <strike group == DI | IN | AT | OT | DO>   DeepIn, In, At, Out, DeepOut
    argv[5]	    <expiration group == 1..3, 1 == near future, 2 == later, 3 == furthest away
		    there is probably some data in some table which tells use what this means
    argv[6]	    step-up buffer size
    argv[7]	    ISEMM absolute value
    argv[8]	    ISEMM absolute value
    argv[9]	    Firm absolute value
    argv[10]	    Firm % value
    argv[11]	    FARMM absolute value
    argv[12]	    FARMM % value
    argv[13..20]    TickWorse 1..8 NB Torque calls these tick 0..7.. which is misleading
		    as level 0 is the value for Ticking worse one level
*/
{
    int 		status_i = SUCCESSFUL;
    // pointer to the underlying we are doing this for
    ise_underlying_xt	*under_data_px = NULL;
    // the strike group # the user specified (as a string)
    uint8		strike_group_u,
    // the expiration group #
			expiration_group_u,
    // the instrument group #
			instr_group_u,
    // the percentage limit for isemm, firm and farmm quotes
			isemm_fraction_u,
			firm_fraction_u,
			farmm_fraction_u = 0;
    // the size of the step-up buffer regan
    uint16		step_up_u,
    // the absolute limit for isemm, firm and farmm quotes
			isemm_limit_absolute_u,
			firm_limit_absolute_u,
			farmm_limit_absolute_u,
    // loop var for processing the 8 levels of tick worse
			item_count_u,
    // how many items we will send in the command
			send_items_u = 0;
    // array of tick-worse values to pack into the msg to central system
    uint16		tick_worse_volume_an [8];
    // if %bin is specified, we need somewhere to translate that into
    ise_underlying_list_xt	bin_under_list_x;
    // which 'case' did the caller specify for each entry in the argv list.. *, %bin, <underlying>
    int			under_selected_i;


    if ( argc < 21 )
    {
	printf ("MM,SET,<underlying name>|%%BIN,<instr group>,<strike group>,<exp group>,<stepup>,\n"
		"      <ISEMM abs>,<ISEMM %%>,<firm abs>,<firm %%>,<FARMM abs>,<FARMM %%>,<TickWorse1>,..<TickWorse8>\n"); 
	return status_i;
    }

    // try to translate the params

    // start out with an 'empty' list of specific underlying to subscribe to :- 0 count, all entries 0
    memset ( &bin_under_list_x, 0, sizeof (bin_under_list_x));

    // first, try to figure out which underlying we are setting params for
    under_selected_i = select_string_option ( 2, set_under_selectors_s, argv[2] );
    switch ( under_selected_i )
    {
    case set_under_bin_e:
	/* Must substitute in all of the underlying for this bin
	    Of course, we must be a CMM / PMM for this to make sense.

	    If we are NOT an MM, then we are NOT allowed to autoquote anyway..
	*/

	status_i = subscr_list_bin ( click_px,
					&bin_under_list_x,
					click_px->our_market_x.default_bin_u );
	// did that work ?
	if (status_i != SUCCESSFUL)
	{
	    printf ("Error doing SET cmd using bin %u\n",
			click_px->our_market_x.default_bin_u );
	    return status_i;
	}

	break;

    case set_under_folio_e:
	// %folio
	not_yet_implemented ("folio selectors in SET command");
	return INVALID_ARGUMENTS;

    default:
	// must be a real underlying (or %u)
	// search for the underlying.. if unknown, then skip it from list with a warning to user
	under_data_px = find_under_by_name_s ( click_px,
						    argv [2 ] );
	// did we find a match
	if ( under_data_px )
	{
	    // known underlying.. add the commodity code to the list
	    bin_under_list_x.commodity_code_u [ bin_under_list_x.underlying_count_u ]
		= under_data_px->commodity_code_u;
	    bin_under_list_x.underlying_count_u = 1;
	}
	else
	{
	    // unknown underlying - report it.. and keep going, skipping this underlying
	    printf ("SET for unknown underlying %s\n",
			argv [ 2 ] );
	    return INVALID_ARGUMENTS;
	}   // test if we found specific underlying (by name)
    }	// switch, on the type of a particular argv[]


    // get the instrument group - which may be 0 (both classes within the underlying)
    instr_group_u = read_unsigned_limited ( argv[3], UNDEFINED_UINT8 );

    // need the strike group - default zero (all strike price groups)
    strike_group_u = cmd_strike_price_group ( argv[4], 0 );
    if ( strike_group_u == UNDEFINED_UINT8 )
    {
	// we have already reported error in cmd selection - bail out
	return status_i;
    }

    // need the expiration group - max value 3
    expiration_group_u = read_unsigned_limited ( argv[5], 3 );
    if ( expiration_group_u == 0 )
    {
	// we have already reported error in cmd selection - bail out
//	return status_i;
    }

    step_up_u = read_unsigned_limited ( argv[6], UNDEFINED_UINT16 );
    isemm_limit_absolute_u = read_unsigned_limited ( argv[7], UNDEFINED_UINT16 );
    isemm_fraction_u = read_unsigned_limited ( argv[8], UNDEFINED_UINT8 );
    firm_limit_absolute_u = read_unsigned_limited ( argv[9], UNDEFINED_UINT16 );
    firm_fraction_u = read_unsigned_limited ( argv[10], UNDEFINED_UINT8 );
    farmm_limit_absolute_u = read_unsigned_limited ( argv[11], UNDEFINED_UINT16 );
    farmm_fraction_u = read_unsigned_limited ( argv[12], UNDEFINED_UINT8 );

    // read the 8 tick-worse values
    for ( item_count_u = 0;
	    item_count_u < 8;
	    item_count_u++ )
    {
	tick_worse_volume_an [item_count_u] = read_unsigned_limited ( argv[13 + item_count_u], UNDEFINED_UINT16 );
    }

    for ( item_count_u = 0;
	  item_count_u < bin_under_list_x.underlying_count_u;
	  item_count_u++ )
    {
	// call the routine to set the params for one underlying
	under_data_px = find_under_data_by_commodity ( click_px,
							bin_under_list_x.commodity_code_u [item_count_u] );
	status_i = set_mm_parameters_under ( click_px,
						under_data_px,
						strike_group_u,
						expiration_group_u,
						instr_group_u,
						isemm_fraction_u,
						firm_fraction_u,
						farmm_fraction_u,
						step_up_u,
						isemm_limit_absolute_u,
						firm_limit_absolute_u,
						farmm_limit_absolute_u,
						tick_worse_volume_an
						);
    }	// for loop, setting params for our selected underlying

    return status_i;
}   // set_mm_parameters

// ---------------------------------------------------------------------------

int32 set_pmm_parameters ( click_details_xt * click_px,
			    int argc,
			    char *argv[] )
/*
Function:   set_pmm_parameters
Author:     Neil Cossor
Modified:   200101xx
Description:

    PMM wishes to set their PMM parameters.
    Uses ui102.

    Exactly as per set_mm_parameters
    BUT
    there are an extra 16 params - the derived order quantities match-away quantities
    for 8 levels
*/
{
    int 		status_i = SUCCESSFUL;
    // pointer to the series we are doing this for
    ise_series_info_xt	*series_data_px = NULL;
    // pointer to the underlying we are doing this for
    ise_underlying_xt	*under_data_px = NULL;
    // the strike group # the user specified (as a string)
    uint8		strike_group_u,
    // the expiration group #
			expiration_group_u,
    // the instrument group #
			instr_group_u,
    // the percentage limit for isemm, firm and farmm quotes
			isemm_fraction_u,
			firm_fraction_u,
			farmm_fraction_u = 0;
    // the size of the step-up buffer regan
    uint16		step_up_u,
    // the absolute limit for isemm, firm and farmm quotes
			isemm_limit_absolute_u,
			firm_limit_absolute_u,
			farmm_limit_absolute_u,
    // loop var for processing the 8 levels of tick worse
			item_count_u,
    // how many items we will send in the command
			send_items_u = 0;
    // array of tick-worse values to pack into the msg to central system
    uint16		tick_worse_volume_an [8];
    // ditto, for derived order max size
    uint16		derived_order_max_size_an [8];
    // ditto for match away market max size
    uint16		match_away_market_max_size_an [8];
    // if %bin is specified, we need somewhere to translate that into
    ise_underlying_list_xt	bin_under_list_x;
    // which 'case' did the caller specify for each entry in the argv list.. *, %bin, <underlying>
    int			under_selected_i;


    if ( argc < 37 )
    {
	printf ("PMM,SET,<underlying name>|%%bin,<instr group>,<strike group>,<exp group>,<stepup>,\n"
		"      <ISEMM abs>,<ISEMM %%>,<firm abs>,<firm %%>,<FARMM abs>,<FARMM %%>,<TickWorse1>,..<TickWorse8>,\n"
		"      <Derive1>,..<Derive8>,<MatchAway1>,..<MatchAway8>\n\n" ); 
	return status_i;
    }

    // try to translate the params

    // start out with an 'empty' list of specific underlying to subscribe to :- 0 count, all entries 0
    memset ( &bin_under_list_x, 0, sizeof (bin_under_list_x));

    // first, try to figure out which underlying we are setting params for
    under_selected_i = select_string_option ( 2, set_under_selectors_s, argv[2] );
    switch ( under_selected_i )
    {
    case set_under_bin_e:
	/* Must substitute in all of the underlying for this bin
	    Of course, we must be a CMM / PMM for this to make sense.

	    If we are NOT an MM, then we are NOT allowed to autoquote anyway..
	*/

	status_i = subscr_list_bin ( click_px,
					&bin_under_list_x,
					click_px->our_market_x.default_bin_u );
	// did that work ?
	if (status_i != SUCCESSFUL)
	{
	    printf ("Error doing SET cmd using bin %u\n",
			click_px->our_market_x.default_bin_u );
	    return status_i;
	}

	break;

    case set_under_folio_e:
	// %folio
	not_yet_implemented ("folio selectors in SET command");
	return INVALID_ARGUMENTS;

    default:
	// must be a real underlying (or %u)
	// search for the underlying.. if unknown, then skip it from list with a warning to user
	under_data_px = find_under_by_name_s ( click_px,
						    argv [2 ] );
	// did we find a match
	if ( under_data_px )
	{
	    // known underlying.. add the commodity code to the list
	    bin_under_list_x.commodity_code_u [ bin_under_list_x.underlying_count_u ]
		= under_data_px->commodity_code_u;
	    bin_under_list_x.underlying_count_u = 1;
	}
	else
	{
	    // unknown underlying - report it.. and keep going, skipping this underlying
	    printf ("SET for unknown underlying %s\n",
			argv [ 2 ] );
	    return INVALID_ARGUMENTS;
	}   // test if we found specific underlying (by name)
    }	// switch, on the type of a particular argv[]

    // get the instrument group - which may be 0 (both classes within the underlying)
    instr_group_u = read_unsigned_limited ( argv[3], UNDEFINED_UINT8 );

    // need the strike group - default zero (all strike price groups)
    strike_group_u = cmd_strike_price_group ( argv[4], 0 );
    if ( strike_group_u == UNDEFINED_UINT8 )
    {
	// we have already reported error in cmd selection - bail out
	return status_i;
    }

    // need the expiration group - max value 3
    expiration_group_u = read_unsigned_limited ( argv[5], 3 );
    if ( expiration_group_u == 0 )
    {
	// we have already reported error in cmd selection - bail out
//	return status_i;
    }

    step_up_u = read_unsigned_limited ( argv[6], UNDEFINED_UINT16 );
    isemm_limit_absolute_u = read_unsigned_limited ( argv[7], UNDEFINED_UINT16 );
    isemm_fraction_u = read_unsigned_limited ( argv[8], UNDEFINED_UINT8 );
    firm_limit_absolute_u = read_unsigned_limited ( argv[9], UNDEFINED_UINT16 );
    firm_fraction_u = read_unsigned_limited ( argv[10], UNDEFINED_UINT8 );
    farmm_limit_absolute_u = read_unsigned_limited ( argv[11], UNDEFINED_UINT16 );
    farmm_fraction_u = read_unsigned_limited ( argv[12], UNDEFINED_UINT8 );

    // read the 8 tick-worse values
    // and the 8 derived quantities
    // and the 8 match-away quantities
    for ( item_count_u = 0;
	    item_count_u < 8;
	    item_count_u++ )
    {
	tick_worse_volume_an [item_count_u] = read_unsigned_limited ( argv[13 + item_count_u], UNDEFINED_UINT16 );
	derived_order_max_size_an [item_count_u] = read_unsigned_limited ( argv[21 + item_count_u], UNDEFINED_UINT16 );
	match_away_market_max_size_an [item_count_u] = read_unsigned_limited ( argv[29 + item_count_u], UNDEFINED_UINT16 );
    }

    for ( item_count_u = 0;
	  item_count_u < bin_under_list_x.underlying_count_u;
	  item_count_u++ )
    {
	// call the routine to set the params for one underlying
	under_data_px = find_under_data_by_commodity ( click_px,
							bin_under_list_x.commodity_code_u [item_count_u] );
	status_i = set_pmm_parameters_under ( click_px,
						under_data_px,
						strike_group_u,
						expiration_group_u,
						instr_group_u,
						isemm_fraction_u,
						firm_fraction_u,
						farmm_fraction_u,
						step_up_u,
						isemm_limit_absolute_u,
						firm_limit_absolute_u,
						farmm_limit_absolute_u,
						tick_worse_volume_an,
						derived_order_max_size_an,
						match_away_market_max_size_an
						);
    }	// for loop, setting params for our selected underlying

    return status_i;
}   // set_pmm_parameters

// ---------------------------------------------------------------------------

static int32 send_opening ( click_details_xt * click_px,
			    generic_txn_xt *OMmsg_px,
			    uint16 item_count_u )
/*
Function:   send_opening
Author:	    Neil Cossor
Modified:   20010320
Description:

    We know we wish to send an MO113 opening transaction.
    However, there is a limit on how many items can be in one of these messages,
    which is a lot lower than the # of series in many underlying.
    We (externally) have logic to build the message, but want to get
    rid of the common 'send' logic, to simplify the outer block.
*/
{
    int32   status_i = SUCCESSFUL;

    // initial stab at length.. size for completely full request
    unsigned	tx_length_u  = sizeof ( OMmsg_px->tx_x.rotation_x );

    // adjust the length according to the actual number of entries
    tx_length_u -= ( MAX_OPEN_COMPONENTS - item_count_u ) * sizeof ( OMmsg_px->tx_x.rotation_x.item [0] );

    // save the facility code and fn code
    OMmsg_px->fn_code = LOG_K_C_ROTATION; 		// rotation command
    OMmsg_px->facility_u = click_px->facility_no_u + 7;	// EP7/ET7

    // how many items
    PUTSHORT (OMmsg_px->tx_x.rotation_x.items_n, item_count_u );

    pack_OM_header ( OMmsg_px, tx_length_u, 'M', 'O', 113 );

    status_i = do_click_tx_rx ( click_px,
				OMmsg_px,
				tx_length_u );
    if ( status_i < SUCCESSFUL)
    {
	// do nothing, error already reported by do_click_tx_rx
    }
    else
    {
	// rotation / open successful
	printf ( "Successful rotation for %hu items\n", item_count_u );
    }

    return status_i;
}   // send_opening

// ---------------------------------------------------------------------------

int32 request_opening ( click_details_xt * click_px,
			int argc,
			char *argv[] )
/*
Function:   request_opening
Author:     Neil Cossor
Modified:   20010620
Description:

    PMM wishes to open the nominated list of series / underlying.
    Uses mo113..

    How do we handle the more generic case, where we wish to open a number of series,
    which are all independent .e.g maybe not all of an underlying, and across multiple
    underlying...

Input Params:

    standard C-style arcg/argv params from the command line
    argv[0]	OPEN
    argv[1]	REQUEST
    argv[2]	S[eries] | U[nderlying] | * (all in bin)
    argv[3]	<series name> | <underlying name> | <ignored if argv[2]=='*'
    argv[4]	<ignore away market flag> 0== consider away, 1== ignore away
    (optional)
    argv[5]	<target series status> 3 == regular, 5 == fast market, default regular
    argv[6]	<fast market level>, ignored if argv[5] is anything other than fast market
*/
{
    // the message we send to central system
    generic_txn_xt	OMmsg_x;
    // size of our actual CLICK query/command
    uint32		tx_length_u = sizeof ( OMmsg_x.tx_x.rotation_x );
    // pointer to Q'able node, containing pointer to info about one series
    gen_buf_node_xt	*series_node_px;
    // pointer to data about the series specified (if supplied)
    ise_series_info_xt	*series_data_px = NULL;
    // pointer to Q'ed item, which contains pointer to the data about the bin we are looking in
    gen_buf_node_xt	*bin_node_px;
    // pointer to the node pointing to the bin we are looking in (for wildcard underlying)
    ise_bin_xt		*bin_data_px;
    // pointer to Q'ed item, which contains pointer to data about one underlying
    gen_buf_node_xt	*under_node_px;
    // pointer to data about the underlying specified (if supplied)
    ise_underlying_xt	*under_data_px = NULL;
    // return status, with default value
    int 		status_i = SUCCESSFUL;
    // the target series status the caller specified, with default
    uint8		series_status_u = SERIES_STATUS_REGULAR;
    // the fast-market level the caller specified - ignore unless desired series status is fast-market
    uint16		fast_market_level_u = 0;
    // ignore-away market flag the caller specified, default = consider away market
    uint8		ignore_away_flag_u = 0;
    // how many series items we are putting into the command/request, and loop var for packing
    uint16		item_limit_u,
			item_count_u;
    // temp var, for selecting from possible options on cmd line
    int 		choice_i;


    // zot our complete query - this includes setting a 'series' selector of 0's
    memset ( &OMmsg_x.tx_x, 0, tx_length_u );

    // check our parameters
    if ( argc < 5 )
    {
	printf ("OPEN,REQUEST,S[eries]|U[nderlying],<series name>|<underlying name>,\n"
		"   <ignore away market>[,<series status>,<fast mkt lvl (if fast)>]\n");
	return SUCCESSFUL;
    }

    // now try to read our parameters
    // read the desired ignore-away market flag - legal values 0 and 1
    ignore_away_flag_u = read_unsigned_limited ( argv[4], 1 );

    // read the desired series status, max value is Fast Market
    if ( argc >= 6 )
	series_status_u = read_unsigned_limited ( argv[5], SERIES_STATUS_FAST_MARKET );

    // if fast-market, read the desired fast-market level
    if (( series_status_u == SERIES_STATUS_FAST_MARKET )
	&& ( argc >= 7 ))
    {
	fast_market_level_u = read_unsigned_limited ( argv[6], UNDEFINED_UINT8 );
    }

    // the desired market status
    OMmsg_x.tx_x.rotation_x.series_status_c = series_status_u;

    // the fast market level - which should be irrelevant unless asking for fast market status
    PUTSHORT ( OMmsg_x.tx_x.rotation_x.fast_market_level_n,
		fast_market_level_u );
    

    // and which series we wish to work on...
    choice_i = select_string_option ( 3, &under_series_s [0], argv[2] );
    switch ( choice_i )
    {
    case under_series_series_e :
	// one series - search for that series
	series_data_px = find_series_by_name_s ( click_px, argv[3] );
	if (!series_data_px)
	{
	    printf ("Error - cannot open series specified [%s], unknown\n",
			argv[3] );
	    return SUCCESSFUL;
	}

	// how many series we will attempt to open
	item_limit_u = 1;
	// and the series
	put_click_series ( &OMmsg_x.tx_x.rotation_x.item[0].series,
			    &series_data_px->ise_series_x );
	// and ignore-away flag
	OMmsg_x.tx_x.rotation_x.item[0].ignore_away_market_c = ignore_away_flag_u;

	// set the orderbook
	OMmsg_x.tx_x.rotation_x.orderbook_c = series_data_px->parent_under_px->orderbook_u;

	status_i = send_opening ( click_px, &OMmsg_x, item_limit_u );

	break;
    case under_series_under_e :
	// all series in one underlying - search for that underlying
	under_data_px = find_under_by_name_s ( click_px, argv[3] );
	if (!under_data_px)
	{
	    printf ("Error - cannot open underlying specified [%s], unknown\n",
			argv[3] );
	    return SUCCESSFUL;
	}

	// any series ?
	if ( under_data_px->under_series_hdr_x.length_i == 0 )
	{
	    printf ("Underlying %s has 0 series, no rotation attempted\n",
			argv[3] );
	    break;
	}

	// set the orderbook
	OMmsg_x.tx_x.rotation_x.orderbook_c = under_data_px->orderbook_u;

	// we might have to do this in pieces - lets start at the beginning
	series_node_px = (gen_buf_node_xt *)under_data_px->under_series_hdr_x.chain.next;
	item_count_u = 0;

	while ( series_node_px != NULL )
	{
	    series_data_px = (ise_series_info_xt *)series_node_px->msg_p;
	    // stash the series to open
	    put_click_series ( &OMmsg_x.tx_x.rotation_x.item[item_count_u].series,
				&series_data_px->ise_series_x );
	    // and ignore-away flag
	    OMmsg_x.tx_x.rotation_x.item[item_count_u].ignore_away_market_c = ignore_away_flag_u;
	    series_node_px = (gen_buf_node_xt *)series_node_px->chain.next;

	    item_count_u++;
	    if (item_count_u >= MAX_OPEN_COMPONENTS )
	    {
		// full, send one, and reset for any more
		status_i = send_opening ( click_px, &OMmsg_x, item_count_u );

		// zot the buffer, no leftover garbage
		memset ( &OMmsg_x.tx_x.rotation_x.item [0 ], 0, 
			    ( sizeof ( OMmsg_x.tx_x.rotation_x.item[0] ) * item_count_u ));

		item_count_u = 0;
	    }	// check if need to wrap, to send full buffer, ann start from beginning
	}   // while loop, thru series in underyling

	// did we have any leftovers items
	if (item_count_u != 0 )
	{
	    // send whatever is left
	    status_i = send_opening ( click_px, &OMmsg_x, item_count_u );
	}

	break;
    case under_series_all_e :
	// all series in all underlying in our bin
	if (click_px->our_market_x.default_bin_u == 0 )
	{
	    printf ("Not logged on as PMM, no opening rotations attempted\n" );
	    return INVALID_ARGUMENTS;
	}

	// point to our bin
	bin_node_px = find_bin ( &click_px->our_market_x,
				    click_px->our_market_x.default_bin_u );

	// we found the bin - point to the interesting stuff
	bin_data_px = (ise_bin_xt *)bin_node_px->msg_p;

	// now, display the underlying in the bin - point to the first
	under_node_px = (gen_buf_node_xt *)bin_data_px->bin_under_hdr_x.chain.next;
	while ( under_node_px )
	{
	    // point to the underlying
	    under_data_px = (ise_underlying_xt *)under_node_px->msg_p;

	    // any series ?
	    if ( under_data_px->under_series_hdr_x.length_i == 0 )
	    {
		printf ("Skipped underlying %s, has 0 series, no rotation attempted\n",
			    under_data_px->under_id_s );
	    }
	    else
	    {
		// set the orderbook
		OMmsg_x.tx_x.rotation_x.orderbook_c = under_data_px->orderbook_u;

		// we might have to do this in pieces - lets start at the beginning
		series_node_px = (gen_buf_node_xt *)under_data_px->under_series_hdr_x.chain.next;
		item_count_u = 0;

		while ( series_node_px != NULL )
		{
		    series_data_px = (ise_series_info_xt *)series_node_px->msg_p;
		    // stash the series to open
		    put_click_series ( &OMmsg_x.tx_x.rotation_x.item[item_count_u].series,
					&series_data_px->ise_series_x );
		    // and ignore-away flag
		    OMmsg_x.tx_x.rotation_x.item[item_count_u].ignore_away_market_c = ignore_away_flag_u;
		    series_node_px = (gen_buf_node_xt *)series_node_px->chain.next;

		    item_count_u++;
		    if (item_count_u >= MAX_OPEN_COMPONENTS )
		    {
			// full, send one, and reset for any more
			status_i = send_opening ( click_px, &OMmsg_x, item_count_u );

			// zot the buffer, no leftover garbage
			memset ( &OMmsg_x.tx_x.rotation_x.item [0 ], 0, 
				    ( sizeof ( OMmsg_x.tx_x.rotation_x.item[0] ) * item_count_u ));

			item_count_u = 0;
		    }	// check if need to wrap, to send full buffer, ann start from beginning
		}   // while loop, thru series in underyling

		// did we have any leftovers items
		if (item_count_u != 0 )
		{
		    // send whatever is left
		    status_i = send_opening ( click_px, &OMmsg_x, item_count_u );
		}
	    }

	    // and the next in this bin
	    under_node_px = (gen_buf_node_xt *)under_node_px->chain.next;
	}   // while loop, for all underlying in the bin

	break;

    default:
	printf ("Error - user must select one of S[eries] | U[nderlying]\n");
    }	// switch on Series / Underlying selector

    return status_i;
}   // request_opening

// ---------------------------------------------------------------------------

static char * rotation_reason_to_string ( uint8 rotation_failure_reason_u )
/*
Function:   rotation_reason_to_string
Author:     Neil Cossor
Modified:   9907xx
Description:

    We have a rotation reason failure number, which we wish to output.
    Convert from the number into a readable string

Input Params:

    The rotation reason failure #

Output Params:

    Pointer to string containing string for that status
*/
{
    static char *rotation_failure_reason_s [] =
    {
	"Away Market Better          ",
	"Prices missing on one side  ",
	"Market Orders remaining     ",
	"Ask/Bid matching Cross limit",
	"        "	// space reserved to put string form of number value, for unknown case
    };

    switch ( rotation_failure_reason_u )
    {
    case ROTATION_REASON_AWAY_BETTER:
	return rotation_failure_reason_s[0];
	break;
    case ROTATION_REASON_PRICES_MISSING:
	return rotation_failure_reason_s[1];
	break;
    case ROTATION_REASON_MARKET_ORDERS:
	return rotation_failure_reason_s[2];
	break;
    case ROTATION_REASON_ASK_BID_CROSS_LIMIT:
	return rotation_failure_reason_s[3];
	break;
    default:
	sprintf ( rotation_failure_reason_s[4], "%3d", rotation_failure_reason_u );
	return rotation_failure_reason_s[4];
    }	// switch on numeric value
}   // rotation_reason_to_string

// ---------------------------------------------------------------------------

static int32 send_query_opening ( click_details_xt * click_px,
				    generic_txn_xt *OMmsg_px,
				    uint16 item_count_u,
				    uint8 series_status_u )
/*
Function:   send_query_opening
Author:	    Neil Cossor
Modified:   20010320
Description:

    We know we wish to send an MQ114 query opening transaction.
    Refer to the reason for the send_opening fn.. same reasoning here.

Input Params:

    - pointer to all we know about the market / system etc
    - pointer to the packed query
    - count of items in the query
    - the desired 
*/
{
    int32   status_i = SUCCESSFUL;
    // pointer to one record of interesting info in the response
    answer_rotation_ma114_item_t	*txn_reply_px;
    uint16		item_limit_u;
    // string and local endian value for the open premium
    series_premium_st	open_premium_s;
    uint32		open_premium_u;
    // local endian values of the open volume / imbalance volume
    uint32		open_volume_u,
			imbalance_volume_u;
    // the series we got in an answer record
    ise_series_xt	ise_series_x;
    // pointer to data about the series specified (if supplied)
    ise_series_info_xt	*series_data_px = NULL;

    // initial stab at length.. size for completely full request
    unsigned	tx_length_u  = sizeof ( OMmsg_px->tx_x.rotation_x );


    // adjust the length according to the actual number of entries
    tx_length_u -= ( MAX_OPEN_COMPONENTS - item_count_u ) * sizeof ( OMmsg_px->tx_x.rotation_x.item [0] );

    // save the facility code and fn code
    OMmsg_px->fn_code = LOG_K_Q_ROTATION; 		// rotation query
    OMmsg_px->facility_u = click_px->facility_no_u + 7;	// EP7/ET7

    // how many items
    PUTSHORT (OMmsg_px->tx_x.rotation_x.items_n, item_count_u );

    pack_OM_header ( OMmsg_px, tx_length_u, 'M', 'Q', 114 );

    status_i = do_click_query_rx ( click_px,
				    OMmsg_px,
				    tx_length_u,
				    LOG_K_R_ROTATION);
    if ( status_i < SUCCESSFUL)
    {
	// do nothing, error already reported by do_click_tx_rx
    }
    else
    {
	// report results from query rotation - how many items in response ?
	PUTSHORT ( item_limit_u,
		    click_px->log_reply_x.trans_reply_x.answer_rotation_x.items_n );
	if ( item_limit_u )
	{
	    // spit out the header for the output columns
	    printf ("%-16s%-9s%12s%10s%10s %-s\n",
			"Series",
			"Status",
			"Open price",
			"Volume",
			"Imbalance",
			"Reason"
		    );

	    for ( item_count_u = 0;
		    item_count_u < item_limit_u;
		    item_count_u++ )
	    {
		// point to record about one series
		txn_reply_px = &click_px->log_reply_x.trans_reply_x.answer_rotation_x.item [ item_count_u ];
		// extract the series component (date, commodity code, strike etc)
		get_click_series ( &ise_series_x,
				    &txn_reply_px->series );
		// find this series
		series_data_px = find_series_by_ise_series ( click_px, &ise_series_x );
		if ( !series_data_px )
		{
		    printf ( "Got rotation response for unknown series\n" );
		}
		else
		{
		    // if it will open, then report different things from if it wont.
		    if ( txn_reply_px->series_status_c != series_status_u )
		    {
			// will NOT open
			// report the imbalance volume, and the rotation reason
			PUTLONG ( imbalance_volume_u, txn_reply_px->volume_left_market_orders_u );

			printf ("%-16s%-9s%12s%10s%10u %-s\n",
				    &series_data_px->ise_name_s,
				    series_status_to_string ( txn_reply_px->series_status_c ),
				    " ",
				    " ",
				    imbalance_volume_u,
				    rotation_reason_to_string ( txn_reply_px->rotation_reason_c )
				);
		    }
		    else
		    {
			// will open
			// report the opening price and volume
			// format the open price, with correct implied decimals
			PUTLONG ( open_premium_u, txn_reply_px->opening_price_i );
			format_whole_decimal_s ( (char *)&open_premium_s,
						    open_premium_u,
						    series_data_px->instrument_class_px->dec_in_premium_u,
						    10 );   // width of 10
			PUTLONG ( open_volume_u, txn_reply_px->volume_u );

			printf ("%-16s%-9s%12s%10u\n",
				    &series_data_px->ise_name_s,
				    series_status_to_string ( txn_reply_px->series_status_c ),
				    &open_premium_s,
				    open_volume_u
				);
		    }
		}   // test if we got rotation record for unknown series
	    }	// for loop, processing records about series
	}
	else
	{
	    printf ( "Received no rotation records ?!\n\n" );
	}   // test if we got any rotation records
    }	// test if successful status from query

    return status_i;
}   // send_query_opening

// ---------------------------------------------------------------------------

int32 query_possible_opening ( click_details_xt * click_px,
				int argc,
				char *argv[] )
/*
Function:   query_possible_opening
Author:     Neil Cossor
Modified:   20000422
Description:

    PMM wishes to find out what would happen if the PMM tried to open.
    Response tells the PMM about any imbalances etc, which need to be
    addressed before a successful opening. This would typically be
    used by the PMM in the period leading up to the Market being opened.
    Uses mq114.

Input Params:

    Same input selectors as request opening (above)

    EXCEPT..

    Fast Market Level is definitely redundant
    ? is Target series status also redundant..???
*/
{
    // the message we send to central system
    generic_txn_xt	OMmsg_x;
    // size of our actual CLICK query/command
    uint32		tx_length_u = sizeof ( OMmsg_x.tx_x.rotation_x );
    // pointer to Q'able node, containing pointer to info about one series
    gen_buf_node_xt	*series_node_px;
    // pointer to data about the series specified (if supplied)
    ise_series_info_xt	*series_data_px = NULL;
    // pointer to data about the underlying specified (if supplied)
    ise_underlying_xt	*under_data_px = NULL;
    // return status, with default value
    int 		status_i = SUCCESSFUL;
    // the target series status the caller specified - should NOT be necessary
    uint8		series_status_u = SERIES_STATUS_REGULAR;
    // the fast-market level the caller specified - ignore unless desired series status is fast-market
    uint16		fast_market_level_u = 0;
    // ignore-away market flag the caller specified, default = consider away market
    uint8		ignore_away_flag_u = 0;
    // loop var for packing query
    uint16		item_count_u;
    // temp var, for selecting from possible options on cmd line
    int 		choice_i;


    // zot our complete query - this includes setting a 'series' selector of 0's
    memset ( &OMmsg_x.tx_x, 0, tx_length_u );

    // check our parameters
    if ( argc < 5 )
    {
	printf ("OPEN,QUERY,S[eries]|U[nderlying],<series name>|<underlying name>,\n"
		"   <ignore away market>,<series status>,<fast market lvl (if fast)>\n");
	return SUCCESSFUL;
    }

    // now try to read our parameters
    // read the desired ignore-away market flag - legal values 0 and 1
    ignore_away_flag_u = read_unsigned_limited ( argv[4], 1 );


    // the desired market status
    OMmsg_x.tx_x.rotation_x.series_status_c = series_status_u;

    // the fast market level - which should be irrelevant unless asking for fast market status
    PUTSHORT ( OMmsg_x.tx_x.rotation_x.fast_market_level_n,
		fast_market_level_u );
    
    // and which series we wish to work on...
    choice_i = select_string_option ( 2, &under_series_s [0], argv[2] );
    switch ( choice_i )
    {
    case under_series_series_e:
	// one series - search for that series
	series_data_px = find_series_by_name_s ( click_px, argv[3] );
	if (!series_data_px)
	{
	    printf ("Error - cannot open series specified [%s], unknown\n",
			argv[3] );
	    return SUCCESSFUL;
	}

	// how many series we will attempt to open
	item_count_u = 1;
	// and the series
	put_click_series ( &OMmsg_x.tx_x.rotation_x.item[0].series,
			    &series_data_px->ise_series_x );
	// and ignore-away flag
	OMmsg_x.tx_x.rotation_x.item[0].ignore_away_market_c = ignore_away_flag_u;

	// set the orderbook
	OMmsg_x.tx_x.rotation_x.orderbook_c = series_data_px->parent_under_px->orderbook_u;

	status_i = send_query_opening ( click_px, &OMmsg_x, item_count_u, series_status_u );

	break;
    case under_series_under_e:
	// all series in one underlying - search for that underlying
	under_data_px = find_under_by_name_s ( click_px, argv[3] );
	if (!under_data_px)
	{
	    printf ("Error - cannot open underlying specified [%s], unknown\n",
			argv[3] );
	    return SUCCESSFUL;
	}

	// any series ?
	if ( under_data_px->under_series_hdr_x.length_i == 0 )
	{
	    printf ("Underlying %s has 0 series, no query sent\n",
			argv[3] );
	    break;
	}

	// set the orderbook
	OMmsg_x.tx_x.rotation_x.orderbook_c = under_data_px->orderbook_u;

	// we might have to do this in pieces - lets start at the beginning
	series_node_px = (gen_buf_node_xt *)under_data_px->under_series_hdr_x.chain.next;
	item_count_u = 0;

	while ( series_node_px != NULL )
	{
	    series_data_px = (ise_series_info_xt *)series_node_px->msg_p;
	    // stash the series to open
	    put_click_series ( &OMmsg_x.tx_x.rotation_x.item[item_count_u].series,
				&series_data_px->ise_series_x );
	    // and ignore-away flag
	    OMmsg_x.tx_x.rotation_x.item[item_count_u].ignore_away_market_c = ignore_away_flag_u;
	    series_node_px = (gen_buf_node_xt *)series_node_px->chain.next;

	    item_count_u++;
	    if (item_count_u >= MAX_OPEN_COMPONENTS )
	    {
		// full, send one, and reset for any more
		status_i = send_query_opening ( click_px, &OMmsg_x, item_count_u, series_status_u );

		// zot the buffer, no leftover garbage
		memset ( &OMmsg_x.tx_x.rotation_x.item [0 ], 0, 
			    ( sizeof ( OMmsg_x.tx_x.rotation_x.item[0] ) * item_count_u ));

		item_count_u = 0;
	    }	// check if need to wrap, to send full buffer, ann start from beginning
	}   // while we have series to go in this underlying

	break;
    default:
	printf ("Error - user must select one of S[eries] | U[nderlying]\n");
    }	// switch on Series / Underlying selector


    return status_i;
}   // query_possible_opening

// ---------------------------------------------------------------------------

int32 release_locked_order ( click_details_xt * click_px,
				int argc,
				char *argv[] )
/*
Function:   release_locked_order
Author:     Neil Cossor
Modified:   9907xx
Description:

    PMM wishes to release a locked order. They can only release
    Away-better Market Better and FARMM locks...
    Uses mo111..

Input Params:

    standard C-style arcg/argv params from the command line
    argv[0]	RELEASE
    argv[1]	BID/ASK 	- can use %O
    argv[2]	series name	- ditto
    argv[3]	order number	- ditto
    argv[4]	lock_type (Away-Market better, or FARMM)
*/
{
    int 		status_i = SUCCESSFUL;
    // the message we send to central system
    generic_txn_xt	OMmsg_x;
    // size of our actual CLICK query/command
    uint32		tx_length_u = sizeof ( OMmsg_x.tx_x.release_locked_x );
    // pointer to data about the series specified (if supplied)
    ise_series_info_xt	*series_data_px = NULL;
    // the bid/ask specified
    uint8		bid_ask_u;
    // the orderID specified
    quad_word		orderID_u;
    // the lock-type specified
    uint8		lock_type_u;
    // pointer to uint64 as a structure, so we can dump out each half of order ID
    uint64_xt		*uint64_pxu;



    // zot everything in our inbound msg
    memset ( &OMmsg_x, 0, tx_length_u );

    // put in the correct header
    pack_OM_header ( &OMmsg_x, tx_length_u, 'M', 'O', 111);

    // enough arguments ?
    if ( argc < 5 )
    {
	printf ("Error.. usage :-\n"
		"RElease,Bid|Ask,<seriesname>,<orderID>,<lock type>\n"
		);
	return status_i;
    }


    // process commandline

    // first argument is bid/ask - can be %O
    bid_ask_u = cmd_bid_ask ( click_px, argv[1] );
    // specify the bid/ask type as supplied
    OMmsg_x.tx_x.release_locked_x.bid_or_ask_c = bid_ask_u;


    // series name -> a series (can be %S, %O)
    series_data_px = user_selected_series ( click_px, argv[2] );
    if (series_data_px == NULL)
    {
	// couldn't find that series - error already reported - bail out
	return SUCCESSFUL;
    }

    // specify the series as selected
    put_click_series ( &OMmsg_x.tx_x.release_locked_x.series,
			&series_data_px->ise_series_x );
    // specify the orderbook, to match the series
    OMmsg_x.tx_x.release_locked_x.orderbook_c =
		    series_data_px->parent_under_px->orderbook_u;

    
    // get the order ID - can be %O
    cmd_orderID ( click_px,
				argv[3], &orderID_u );
    // specify the original order ID
    PUTQUAD ( OMmsg_x.tx_x.release_locked_x.order_number_u,
		orderID_u );

    // get the lock-type - just do it by number for now (allows us to try ANY value)
    lock_type_u = read_unsigned_limited ( argv[4], UNDEFINED_UINT8 );
    OMmsg_x.tx_x.release_locked_x.lock_type_c = lock_type_u;

    // save the facility code and fn code
    OMmsg_x.fn_code = LOG_K_C_RELEASE_LOCK;
    OMmsg_x.facility_u = click_px->facility_no_u + 7;	// EP7/ET7

    // point structure pointer at the 64bit order ID
    uint64_pxu = (uint64_xt *)&(orderID_u);

    printf ("Release lock, %s in %s against Order Number: [%08lx.%08lx], lock type \n",
	    bid_ask_to_string ( bid_ask_u ),
	    &series_data_px->ise_name_s,
	    uint64_pxu->l1,
	    uint64_pxu->l0,
	    lock_type_to_string (lock_type_u )
	    );

    status_i = do_click_tx_rx ( click_px,
				    &OMmsg_x,
				    tx_length_u );


    if ( status_i > SUCCESSFUL )
    {
    }	// test if successful Release Lock

    return status_i;
}   // release_locked_order

// ---------------------------------------------------------------------------

int32 trade_report ( click_details_xt *click_px,
			int argc,
			char *argv[] )
/*
Function:   trade_report
Author:     Neil Cossor
Modified:   200012xx
Description:

    PMM wishes to (manually) trade with against an existing order in the market.
    Uses MO112.

    While technically PMMs can trade against any order, the Legal and Surveillance
    issues are not so simple. Fortunately, this is an SEP - Someone Else's Problem

    Note that if we are using the same ISEsim session as whichever BD/operator
    sent the original order, and that order was snapshot-saved (using the SAVE command),
    then we can use %O to give us the shortcut to the series, orderID, and bid-ask of
    the original order.
    NB are we supposed to be putting in the bid/ask the same as the original order,
    or the opposite (which would be the most 'normal'/obvious thing to do)

Input Params:

    standard C-style arcg/argv params from the command line
    argv[0]	TRADE
    argv[1]	REPORT
    argv[2]	BID/ASK 	- can get %O, but is this correct (see note above)
    argv[3]	series name	- can use %O
    argv[4]	order number	- ditto
    argv[5]	quantity
    argv[6]	premium
    argv[7]	order category (must be CAB or plain order)
*/
{
    // return var, with default status
    int 		status_i = SUCCESSFUL;
    // the message we send to central system
    generic_txn_xt	OMmsg_x;
    // size of our actual CLICK query/command
    uint32		tx_length_u = sizeof ( OMmsg_x.tx_x.trade_report_x );
    // pointer to data about the series specified (if supplied)
    ise_series_info_xt	*series_data_px = NULL;
    // the bid/ask specified
    uint8		bid_ask_u;
    // the orderID specified
    quad_word		orderID_u;
    // pointer to uint64 as a structure, so we can dump out each half of order ID
    uint64_xt		*uint64_pxu;
    // the premium specified
    int32		order_premium_u;
    // the quantity specified
    uint32		order_quantity_u;
    series_premium_st	premium_s;	    // premium, in string form
    // the order category specified
    uint16		order_category_u;



    // zot everything in our inbound msg
    memset ( &OMmsg_x, 0, tx_length_u );

    // put in the correct header
    pack_OM_header ( &OMmsg_x, tx_length_u, 'M', 'O', 112);

    // enough arguments ?
    if ( argc < 8 )
    {
	printf ("Error.. usage :-\n"
		"Trade,REPort,Bid|Ask,<seriesname>,<orderID>,<quantity>,<premium>,<order category>\n"
		);
	return status_i;
    }


    // process commandline

    // first argument is bid/ask - can be %O
    bid_ask_u = cmd_bid_ask ( click_px, argv[2] );
    // specify the bid/ask type as supplied
    OMmsg_x.tx_x.trade_report_x.bid_or_ask_c = bid_ask_u;


    // series name -> a series (can be %S, %O)
    series_data_px = user_selected_series ( click_px, argv[3] );
    if (series_data_px == NULL)
    {
	// couldn't find that series - error already reported - bail out
	return SUCCESSFUL;
    }

    // specify the series as selected
    put_click_series ( &OMmsg_x.tx_x.trade_report_x.series,
			&series_data_px->ise_series_x );
    // specify the orderbook, to match the series
    OMmsg_x.tx_x.trade_report_x.orderbook_c =
		    series_data_px->parent_under_px->orderbook_u;

    
    // get the order ID - can be %O
    cmd_orderID ( click_px,
				argv[4], &orderID_u );
    // specify the original order ID
    PUTQUAD ( OMmsg_x.tx_x.trade_report_x.order_number_u,
		orderID_u );

    // Read the order quantity
    order_quantity_u = (uint32)read_unsigned_limited (argv [5], UNDEFINED_UINT32);
    PUTLONG ( OMmsg_x.tx_x.trade_report_x.quantity_u, order_quantity_u );

    // Read the order premium
    order_premium_u = read_decimal_string_int ( argv[6],
				series_data_px->instrument_class_px->dec_in_premium_u );
    PUTLONG ( OMmsg_x.tx_x.trade_report_x.premium_i, order_premium_u );

    // Read the order category - this should support %O... ???
    if ( strcmp ( argv[7], "%O" ) == 0 )
	order_category_u = click_px->saved_order_x.order_category_u;
    else
	order_category_u = read_unsigned_limited ( argv[7], UNDEFINED_UINT16 );
    PUTSHORT ( OMmsg_x.tx_x.trade_report_x.order_category_n,
		order_category_u );

    // save the facility code and fn code
    OMmsg_x.fn_code = LOG_K_C_TRADE_REPORT;
    OMmsg_x.facility_u = click_px->facility_no_u + 7;	// EP7/ET7

    // point structure pointer at the 64bit order ID
    uint64_pxu = (uint64_xt *)&(orderID_u);

    format_whole_decimal_s ( (char *)&premium_s,
				order_premium_u,
				series_data_px->instrument_class_px->dec_in_premium_u,
				MAX_SERIES_PREMIUM );	// allowed width

    printf ("Trade Report, %hu@%s, %s in %s against Order Number: [%08lx.%08lx], category %s\n",
	    order_quantity_u,
	    &premium_s,
	    bid_ask_to_string (bid_ask_u),
	    series_data_px->ise_name_s,
	    uint64_pxu->l1,
	    uint64_pxu->l0,
	    order_category_to_string ( order_category_u )
	    );

    status_i = do_click_tx_rx ( click_px,
				    &OMmsg_x,
				    tx_length_u );


    if ( status_i > SUCCESSFUL )
    {
    }	// test if successful Trade Report

    return status_i;
}   // trade_report

// ---------------------------------------------------------------------------

static int32 send_query_mm_parameters ( click_details_xt * click_px,
					ise_series_xt *ise_series_px )
/*
Function:   send_query_mm_parameters
Author:     Neil Cossor
Modified:   200012xx
Description:

    MM wishes to get their MM parameters.
    Uses uq101.

    The query can only be done at the underlying or instrument class level.

    For an instrument class, we will get a number of records.. one per combination
    of strike-price-group (deep in, in, at, out, deep out) and expiration group
    (near expiries, later expiries, much later expiries).

    If it is done at the underlying level, we might get back a response with
    the combined records for each instrument class under the underlying
    uh.. except for spots...which aren't tradeable...

Input Params:

    pointer to the standard 'everything about CLICK' structure
    pointer to the series qualifier we will query on.
	This may contain just a commodity code, or instr class (commodity + instr group)
*/
{
    // pointer to one 'line' of info in the response
    answer_mm_parameters_ua101_item_t	*txn_reply_px;
    // our query msg
    generic_txn_xt	OMmsg_x;
    // size of our actual CLICK query/command
    uint32		tx_length_u = sizeof ( OMmsg_x.tx_x.query_mm_parameters_x );
    // status var to return, with good default initial value
    int 		status_i = SUCCESSFUL;
    uint16		segment_u = 0;
    uint16		item_index_u,	// local loop var
			item_limit_u;	// limit on loop - may be endian-swapped from response
    uint32		items_found_u = 0;	// total # we found from all the records and segments
    unsigned		tick_level_u;	// loop var, for dumping tick-worse across tick levels
    // the commodity code of underlying in the bcast
    uint16		found_commodity_code_u;
    // the underlying record for the underlying
    ise_underlying_xt	*found_under_data_px;
    // the size of the step-up buffer regen
    uint16		step_up_u,
    // the absolute limit for ISEmm, firm and farmm quotes
			isemm_limit_absolute_u,
			firm_limit_absolute_u,	// other than ISE MMs
			farmm_limit_absolute_u;


    // zot our complete query - this includes setting a 'series' selector of 0's
    memset ( &OMmsg_x.tx_x, 0, tx_length_u );


    // save the facility code and fn code
    OMmsg_x.fn_code = LOG_K_Q_SERIES_SEGMENTS;	    // standard seres & segment query
    OMmsg_x.facility_u = click_px->facility_no_u;   // EP0/ET0

    pack_OM_header ( &OMmsg_x, sizeof ( query_series_t ), 'U', 'Q', 101 );
    OMmsg_x.tx_x.query_series_x.series.country_c = click_px->our_market_x.country_u;
    OMmsg_x.tx_x.query_series_x.series.market_c = click_px->our_market_x.market_u;
    // and our selected underlying
    OMmsg_x.tx_x.query_series_x.series.commodity_n = ise_series_px->underlying_code_u;
    // and instrument group selector - possibly 0
    OMmsg_x.tx_x.query_series_x.series.instrument_group_c = ise_series_px->instrument_group_u;

    // Multi-segment data - loop until error, or no more data
    do
    {
	// advance to next segment
	segment_u++; // The segment # we wish to retrieve
	PUTSHORT( OMmsg_x.tx_x.query_series_x.segment_number_n, segment_u );

	status_i = do_click_query_rx ( click_px,
					&OMmsg_x,
					tx_length_u,
					LOG_K_R_GET_MM_PARAMS );

	if ( status_i >= SUCCESSFUL )
	{
	    // all OK - did we actually get any component items ?

	    // get item count in correct endian order
	    PUTSHORT (item_limit_u, click_px->log_reply_x.trans_reply_x.answer_mm_parameters_x.items_n );

	    // break out of the do loop, if we got no records - either there aren't any.
	    // or we finished the last in the previous segment
	    if ( item_limit_u == 0 )
		// 
		break;

	    // spit out header, if we haven't already for this query
	    if ( items_found_u == 0 )
		printf ("Underlying      Instrument name      Strike ExpGrp Step ISEMM#   %% Firm#   %% FARM#   %% "
			"TW 1    2    3    4    5    6    7    8\n"
			);

	    // note how many more items in this response
	    items_found_u += item_limit_u;

	    // look at the items
	    for ( item_index_u = 0;
		item_index_u < item_limit_u;
		item_index_u++ )
	    {
		// point to interesting part of data in standard rx bufer
		txn_reply_px = &click_px->log_reply_x.trans_reply_x.answer_mm_parameters_x.item[item_index_u];

		// find the relevant underlying
		PUTSHORT ( found_commodity_code_u,
			    txn_reply_px->series.commodity_n );
		PUTSHORT ( step_up_u, txn_reply_px->mm_parameters.step_up_after_regen_buffer_n );
		PUTSHORT ( isemm_limit_absolute_u, txn_reply_px->mm_parameters.isemm_trade_limit_absolute_n );
		PUTSHORT ( firm_limit_absolute_u, txn_reply_px->mm_parameters.firm_trade_limit_absolute_n );
		PUTSHORT ( farmm_limit_absolute_u, txn_reply_px->mm_parameters.farmm_trade_limit_absolute_n );
		// find the underlying record
		found_under_data_px = find_under_data_by_commodity ( click_px,
								found_commodity_code_u );


		// ? will this fit on screen with 2 extra ISEMM fields
		printf ("%-16s%-20s %-4s %6d %6d %6d %3d %5d %3d %5d %3d ",
			found_under_data_px->under_id_s,
			instr_group_name ( click_px, 
					    txn_reply_px->series.instrument_group_c ),
			strike_price_group_to_string ( txn_reply_px->strike_price_group_c ),
			txn_reply_px->expiration_group_c,
			step_up_u,
			isemm_limit_absolute_u,
			txn_reply_px->mm_parameters.isemm_trade_limit_fraction_c,
			firm_limit_absolute_u,
			txn_reply_px->mm_parameters.firm_trade_limit_fraction_c,
			farmm_limit_absolute_u,
			txn_reply_px->mm_parameters.farmm_trade_limit_fraction_c
			);
		for (tick_level_u = 0; tick_level_u <8; tick_level_u++ )
		{
		    printf ("%4d ",
				txn_reply_px->mm_parameters.tick_worse_volume_an [ tick_level_u ] );
		}
		printf ("\n");
	    }	// for loop on one series in response

	    /* regardless of whether there were any items in it, if the segment # in the response
		is 0, then we got all the data there is. NB although this is endian, a binary
		0 is still a 0 - don't have to do a PUTSHORT
	    */
	    if ( click_px->log_reply_x.trans_reply_x.answer_mm_parameters_x.segment_number_n == 0 )
		// no more data - out of here
		break;
	}   // test if success getting another segment of data
    } while ( status_i >= SUCCESSFUL );
 
    return status_i;
}   // send_query_mm_parameters

// ---------------------------------------------------------------------------

int32 query_mm_parameters ( click_details_xt * click_px,
			    int argc,
			    char *argv[] )
/*
Function:   query_mm_parameters
Author:     Neil Cossor
Modified:   9907xx
Description:

    MM wishes to get their MM parameters.
    Uses uq101, by calling send_query_mm_parameters

    The query can only be done at the underlying or instrument class level.

Input Params:

    argv[0]	MM
    argv[1]	GET
    argv[2]	underlying name for query
    (optionally)
    argv[3]	instrument group.. combined with underlying, this gives us instrument class
*/
{
    // status var to return, with good default initial value
    int 		status_i = SUCCESSFUL;
    // local selector series - can be underlying, or instrument class
    ise_series_xt	ise_series_x;
    // the underlying record for the underlying
    ise_underlying_xt	*found_under_data_px;
    // pointer to (Q'ed) node containing pointer to info about one underlying
    gen_buf_node_xt	*found_under_node_px;
    // the nominated instrument group - default to all
    uint8		instr_group_u = 0;
    // pointer to the node pointing to the bin we are looking in (for wildcard underlying)
    ise_bin_xt		*bin_data_px;
    // and pointer to the data about the bin we are looking in
    gen_buf_node_xt	*bin_node_px;


    // zot the series selector - all underlying, all instrument groups
    memset ( &ise_series_x, 0, sizeof ( ise_series_x ));

    
    // can't do this unless we are logged in
    if ( !click_px->logged_on_i )
    {
	prompt_logged_on ("MM,GET parameters");
	return NOT_LOGGED_ON;
    }

    // have we enuff arguments ?
    if ( argc < 3)
    {
	printf ("Insufficent params :- MM,GET,<underlying|*>[,<instrument group>]\n");
	return SUCCESSFUL;
    }

    // did we get valid optional params ?
    if ( argc >= 4 )
    {
	// read the instrument group param
	instr_group_u = (uint8) read_unsigned_limited ( argv[3],
							UNDEFINED_UINT8 );

	// and apply this subfilter
	// - we are now querying on a instrument classes, rather than underlying
	ise_series_x.instrument_group_u = instr_group_u;
    }

    if ( *argv[2]== '*' )
    {
	// for wildcard underlying, we MUST have an allocated bin ?
	if ( click_px->our_market_x.default_bin_u == 0 )
	{
	    printf ("Error - this user is NOT allocated as MM for any bins - wildcard NOT valid\n");
	    return SUCCESSFUL;
	}

	bin_node_px = find_bin ( &click_px->our_market_x,
			    click_px->our_market_x.default_bin_u );
	bin_data_px = (ise_bin_xt *)bin_node_px->msg_p;

	// work our way thru all underlyings in our bin (which are in alphabetical order)
	found_under_node_px = (gen_buf_node_xt *)bin_data_px->bin_under_hdr_x.chain.next;
	while (found_under_node_px)
	{
	    found_under_data_px = (ise_underlying_xt *)found_under_node_px->msg_p;

	    // only query those which are in our bin, and which have series
	    if ( found_under_data_px->under_series_hdr_x.length_i > 0 )
	    {
		// put in the commodity code
		ise_series_x.underlying_code_u = found_under_data_px->commodity_code_u;

		status_i = send_query_mm_parameters ( click_px, &ise_series_x );
	    }	// test for if this underlying is in our bin

	    // and the next in this bin
	    found_under_node_px = (gen_buf_node_xt *)found_under_node_px->chain.next;
	}   // for loop, doing all underlying
    }
    else
    {
	// one underlying - find the relevant underlying
	found_under_data_px = find_under_by_name_s ( click_px,
							argv[2] );
	if ( !found_under_data_px )
	{
	    // no such underlying
	    printf ("Error : unable to query on unknown underlying %s\n",
			argv[2] );
	    return SUCCESSFUL;
	}   // test for finding this underlying

	// put in the commodity code
	ise_series_x.underlying_code_u = found_under_data_px->commodity_code_u;

	status_i = send_query_mm_parameters ( click_px, &ise_series_x );
    };
  
    return status_i;
}   // query_mm_parameters

// ---------------------------------------------------------------------------

int32 send_query_pmm_parameters ( click_details_xt * click_px,
				    ise_series_xt *ise_series_px )
/*
Function:   send_query_pmm_parameters
Author:     Neil Cossor
Modified:   200012xx
Description:

    PMM wishes to get their PMM parameters.
    Uses uq102.

    This is exactly the same as send_query_mm_parameters
    EXCEPT that it uses a uq102, instead of a uq101...
    and gets responses which contain additional fields for each component
    response record.

Input Params:

    pointer to the standard 'everything about CLICK' structure
    pointer to the series qualifier we will query on.
	This may contain just a commodity code, or instr class (commodity + instr group)
*/
{
    // pointer to one 'line' of info in the response
    answer_pmm_parameters_ua102_item_t	*txn_reply_px;
    // our query msg
    generic_txn_xt	OMmsg_x;
    // size of our actual CLICK query/command
    uint32		tx_length_u = sizeof ( OMmsg_x.tx_x.query_pmm_parameters_x );
    // status var to return, with good default initial value
    int 		status_i = SUCCESSFUL;
    uint16		segment_u = 0;
    uint16		item_index_u,	// local loop var
			item_limit_u;	// limit on loop - may be endian-swapped from response
    uint32		items_found_u = 0;	// total # we found from all the records and segments
    unsigned		tick_level_u;	// loop var, for dumping tick-worse, derived, and match-away across tick levels
    // the commodity code of underlying in the bcast
    uint16		found_commodity_code_u;
    // the underlying record for the underlying
    ise_underlying_xt	*found_under_data_px;
    // the size of the step-up buffer regen
    uint16		step_up_u,
    // the absolute limit for ISEmm, firm and farmm quotes
			isemm_limit_absolute_u,
			firm_limit_absolute_u,	// other than ISE MMs
			farmm_limit_absolute_u;


    // zot our complete query - this includes setting a 'series' selector of 0's
    memset ( &OMmsg_x.tx_x, 0, tx_length_u );


    // save the facility code and fn code
    OMmsg_x.fn_code = LOG_K_Q_SERIES_SEGMENTS;	    // standard seres & segment query
    OMmsg_x.facility_u = click_px->facility_no_u;   // EP0/ET0

    pack_OM_header ( &OMmsg_x, sizeof ( query_series_t ), 'U', 'Q', 102 );
    OMmsg_x.tx_x.query_series_x.series.country_c = click_px->our_market_x.country_u;
    OMmsg_x.tx_x.query_series_x.series.market_c = click_px->our_market_x.market_u;
    // and our selected underlying
    OMmsg_x.tx_x.query_series_x.series.commodity_n = ise_series_px->underlying_code_u;
    // and instrument group selector - possibly 0
    OMmsg_x.tx_x.query_series_x.series.instrument_group_c = ise_series_px->instrument_group_u;

    // Multi-segment data - loop until error, or no more data
    do
    {
	// advance to next segment
	segment_u++; // The segment # we wish to retrieve
	PUTSHORT( OMmsg_x.tx_x.query_series_x.segment_number_n, segment_u );

	status_i = do_click_query_rx ( click_px,
					&OMmsg_x,
					tx_length_u,
					LOG_K_R_GET_PMM_PARAMS );

	if ( status_i >= SUCCESSFUL )
	{
	    // all OK - did we actually get any component items ?

	    // get item count in correct endian order
	    PUTSHORT (item_limit_u, click_px->log_reply_x.trans_reply_x.answer_pmm_parameters_x.items_n );

	    // break out of the do loop, if we got no records - either there aren't any.
	    // or we finished the last in the previous segment
	    if ( item_limit_u == 0 )
		// 
		break;

	    // spit out header, if we haven't already for this query
	    if ( items_found_u == 0 )
		printf ("Underlying      Instrument name      Strike ExpGrp Step ISEMM#   %% Firm#   %% FARM#   %%\n"
			"TW 1    2    3    4    5    6    7    8 Drv1    2    3    4    5    6    7    8 MA 1    2    3    4    5    6    7    8\n\n"
			);

	    // note how many more items in this response
	    items_found_u += item_limit_u;

	    // look at the items
	    for ( item_index_u = 0;
		item_index_u < item_limit_u;
		item_index_u++ )
	    {
		// point to interesting part of data in standard rx bufer
		txn_reply_px = &click_px->log_reply_x.trans_reply_x.answer_pmm_parameters_x.item[item_index_u];

		// find the relevant underlying
		PUTSHORT ( found_commodity_code_u,
			    txn_reply_px->series.commodity_n );
		PUTSHORT ( step_up_u, txn_reply_px->mm_parameters.step_up_after_regen_buffer_n );
		PUTSHORT ( isemm_limit_absolute_u, txn_reply_px->mm_parameters.isemm_trade_limit_absolute_n );
		PUTSHORT ( firm_limit_absolute_u, txn_reply_px->mm_parameters.firm_trade_limit_absolute_n );
		PUTSHORT ( farmm_limit_absolute_u, txn_reply_px->mm_parameters.farmm_trade_limit_absolute_n );
		// find the underlying record - allow that central system may give us more than we asked for
		found_under_data_px = find_under_data_by_commodity ( click_px,
								found_commodity_code_u );

		printf ("%-16s%-20s %-4s %6d %6d %6d %3d %5d %3d %5d %3d \n",
			found_under_data_px->under_id_s,
			instr_group_name ( click_px,
					    txn_reply_px->series.instrument_group_c ),
			strike_price_group_to_string ( txn_reply_px->strike_price_group_c ),
			txn_reply_px->expiration_group_c,
			step_up_u,
			isemm_limit_absolute_u,
			txn_reply_px->mm_parameters.isemm_trade_limit_fraction_c,
			firm_limit_absolute_u,
			txn_reply_px->mm_parameters.firm_trade_limit_fraction_c,
			farmm_limit_absolute_u,
			txn_reply_px->mm_parameters.farmm_trade_limit_fraction_c
			);
		for (tick_level_u = 0; tick_level_u <8; tick_level_u++ )
		{
		    printf ("%4d ",
				txn_reply_px->mm_parameters.tick_worse_volume_an [ tick_level_u ] );
		}
		for (tick_level_u = 0; tick_level_u <8; tick_level_u++ )
		{
		    printf ("%4d ",
				txn_reply_px->pmm_parameters.derived_order_max_size_an [ tick_level_u ] );
		}
		for (tick_level_u = 0; tick_level_u <8; tick_level_u++ )
		{
		    printf ("%4d ",
				txn_reply_px->pmm_parameters.match_away_market_max_size_an [ tick_level_u ] );
		}
		printf ("\n\n");
	    }	// for loop on one series in response

	    /* regardless of whether there were any items in it, if the segment # in the response
		is 0, then we got all the data there is. NB although this is endian, a binary
		0 is still a 0 - don't have to do a PUTSHORT
	    */
	    if ( click_px->log_reply_x.trans_reply_x.answer_pmm_parameters_x.segment_number_n == 0 )
		// no more data - out of here
		break;
	}   // test if success getting another segment of data
    } while ( status_i >= SUCCESSFUL );
 
    return status_i;
}   // send_query_pmm_parameters

// ---------------------------------------------------------------------------

int32 query_pmm_parameters ( click_details_xt * click_px,
			    int argc,
			    char *argv[] )
/*
Function:   query_pmm_parameters
Author:     Neil Cossor
Modified:   20010305
Description:

    PMM wishes to get their PMM parameters.
    Uses uq102.

    This is exactly the same as query_mm_parameters
    EXCEPT that it uses a uq102, instead of a uq101...
    and gets responses which contain additional fields for each component
    response record.

    Note, if a * wildcard is provided for the underlying, then we do the whole
    bin. The bin is determined by the dq211/da211 data.. whichever bin had our customer_id_s
    as the PMM.

    If we are doing the whole bin though, we do NOT ask about parameters in underlying for
    which there are no series defined. The use can, of course, explicitly query one
    of those underlying.. just not with the wildcard.

Input Params:

    argv[0]	MM
    argv[1]	GET
    argv[2]	underlying name for query
    (optionally)
    argv[3]	instrument group.. combined with underlying, this gives us instrument class
*/
{
    // status var to return, with good default initial value
    int 		status_i = SUCCESSFUL;
    // local selector series - can be underlying, or instrument class
    ise_series_xt	ise_series_x;
    // the underlying record for the underlying
    ise_underlying_xt	*found_under_data_px;
    // pointer to (Q'ed) node containing pointer to info about one underlying
    gen_buf_node_xt	*found_under_node_px;
    // the nominated instrument group - default to all
    uint8		instr_group_u = 0;
    // pointer to the node pointing to the bin we are looking in (for wildcard underlying)
    ise_bin_xt		*bin_data_px;
    // and pointer to the data about the bin we are looking in
    gen_buf_node_xt	*bin_node_px;


    // zot the series selector - all underlying, all instrument groups
    memset ( &ise_series_x, 0, sizeof ( ise_series_x ));

    
    // can't do this unless we are logged in
    if ( !click_px->logged_on_i )
    {
	prompt_logged_on ("PMM, GET parameters");
	return NOT_LOGGED_ON;
    }

    // have we enuff arguments ?
    if ( argc < 3)
    {
	printf ("Insufficent params :- PMM,GET,<underlying|*>[,<instrument group>]\n");
	return SUCCESSFUL;
    }

    // did we get valid optional params ?
    if ( argc >= 4 )
    {
	// read the instrument group param
	instr_group_u = (uint8) read_unsigned_limited ( argv[3],
							UNDEFINED_UINT8 );

	// and apply this subfilter
	// - we are now querying on a instrument classes, rather than underlying
	ise_series_x.instrument_group_u = instr_group_u;
    }

    if ( *argv[2]== '*' )
    {
	// for wildcard underlying, we MUST have an allocated bin ?
	if ( click_px->our_market_x.default_bin_u == 0 )
	{
	    printf ("Error - this user is NOT allocated as PMM for any bins - wildcard NOT valid\n");
	    return SUCCESSFUL;
	}

	bin_node_px = find_bin ( &click_px->our_market_x,
			    click_px->our_market_x.default_bin_u );
	bin_data_px = (ise_bin_xt *)bin_node_px->msg_p;

	// work our way thru all underlyings in our bin (which are in alphabetical order)
	found_under_node_px = (gen_buf_node_xt *)bin_data_px->bin_under_hdr_x.chain.next;
	while (found_under_node_px)
	{
	    found_under_data_px = (ise_underlying_xt *)found_under_node_px->msg_p;

	    // only query those which are in our bin, and which have series
	    if ( found_under_data_px->under_series_hdr_x.length_i > 0 )
	    {
		// put in the commodity code
		ise_series_x.underlying_code_u = found_under_data_px->commodity_code_u;

		status_i = send_query_pmm_parameters ( click_px, &ise_series_x );
	    }	// test for if this underlying is in our bin

	    // and the next in this bin
	    found_under_node_px = (gen_buf_node_xt *)found_under_node_px->chain.next;
	}   // for loop, doing all underlying
    }
    else
    {
	// one underlying - find the relevant underlying
	found_under_data_px = find_under_by_name_s ( click_px,
							argv[2] );
	if ( !found_under_data_px )
	{
	    // no such underlying
	    printf ("Error : unable to query on unknown underlying %s\n",
			argv[2] );
	    return SUCCESSFUL;
	}   // test for finding this underlying

	// put in the commodity code
	ise_series_x.underlying_code_u = found_under_data_px->commodity_code_u;

	status_i = send_query_pmm_parameters ( click_px, &ise_series_x );
    };
 
    return status_i;
}   // query_pmm_parameters

// ---------------------------------------------------------------------------

static int32 send_query_speed_parameters ( click_details_xt * click_px,
					    ise_series_xt *ise_series_px )
/*
Function:   send_query_speed_parameters
Author:     Neil Cossor
Modified:   20010709
Description:

    MM wishes to get their speed parameters.
    Uses uq103.

    The query can only be done at the underlying level.


Input Params:

    pointer to the standard 'everything about CLICK' structure
    pointer to the series qualifier we will query on.
	This may contain just a commodity code
*/
{
    // pointer to one 'line' of info in the response
    answer_mm_exposure_limit_ua103_item_t	*txn_reply_px;
    // our query msg
    generic_txn_xt	OMmsg_x;
    // size of our actual CLICK query/command
    uint32		tx_length_u = sizeof ( OMmsg_x.tx_x.query_mm_exposure_limit_x );
    // status var to return, with good default initial value
    int 		status_i = SUCCESSFUL;
    uint16		segment_u = 0;
    uint16		item_index_u,	// local loop var
			item_limit_u;	// limit on loop - may be endian-swapped from response
    // the commodity code of underlying in the bcast
    uint16		found_commodity_code_u;
    // the underlying record for the underlying
    ise_underlying_xt	*found_under_data_px;
    // the quantity in contracts before we bump
    uint32		trade_limit_u;
    // the percentage we widen spread by when we bump
    uint16		spread_widen_percentage_u;

    // zot our complete query - this includes setting a 'series' selector of 0's
    memset ( &OMmsg_x.tx_x, 0, tx_length_u );


    // save the facility code and fn code
    OMmsg_x.fn_code = LOG_K_Q_SERIES_SEGMENTS;	    // standard seres & segment query
    OMmsg_x.facility_u = click_px->facility_no_u;   // EP0/ET0

    pack_OM_header ( &OMmsg_x, sizeof ( query_series_t ), 'U', 'Q', 103 );
    OMmsg_x.tx_x.query_series_x.series.country_c = click_px->our_market_x.country_u;
    OMmsg_x.tx_x.query_series_x.series.market_c = click_px->our_market_x.market_u;
    // and our selected underlying
    OMmsg_x.tx_x.query_series_x.series.commodity_n = ise_series_px->underlying_code_u;

    // Multi-segment data - loop until error, or no more data
    do
    {
	// advance to next segment
	segment_u++; // The segment # we wish to retrieve
	PUTSHORT( OMmsg_x.tx_x.query_series_x.segment_number_n, segment_u );

	status_i = do_click_query_rx ( click_px,
					&OMmsg_x,
					tx_length_u,
					LOG_K_R_SPEEDBUMP );

	if ( status_i >= SUCCESSFUL )
	{
	    // all OK - did we actually get any component items ?

	    // get item count in correct endian order
	    PUTSHORT (item_limit_u, click_px->log_reply_x.trans_reply_x.answer_mm_exposure_limit_x.items_n );

	    // look at the items
	    for ( item_index_u = 0;
		item_index_u < item_limit_u;
		item_index_u++ )
	    {
		// point to interesting part of data in standard rx bufer
		txn_reply_px = &click_px->log_reply_x.trans_reply_x.answer_mm_exposure_limit_x.item[item_index_u];

		// find the relevant underlying
		PUTSHORT ( found_commodity_code_u,
			    txn_reply_px->series.commodity_n );
		PUTLONG ( trade_limit_u, txn_reply_px->curtailment_trading_threshold_u );
		PUTSHORT ( spread_widen_percentage_u, txn_reply_px->curtailment_spread_factor_n );
		// find the underlying record
		found_under_data_px = find_under_data_by_commodity ( click_px,
								found_commodity_code_u );


		// NB, speedbump params can be set for all commodities, whether they have any instrument
		// classes or not. If they do not have any instrument classes, then we know little
		// about them...
		if (found_under_data_px)
		    // display this
		    printf ("Underlying %-16s will widen by %u%% when %u contracts trade in interval\n",
			    &found_under_data_px->under_id_s,
			    spread_widen_percentage_u,
			    trade_limit_u
			    );
	    }	// for loop on one series in response

	    /* regardless of whether there were any items in it, if the segment # in the response
		is 0, then we got all the data there is. NB although this is endian, a binary
		0 is still a 0 - don't have to do a PUTSHORT
	    */
	    if ( click_px->log_reply_x.trans_reply_x.answer_mm_exposure_limit_x.segment_number_n == 0 )
		// no more data - out of here
		break;
	}   // test if success getting another segment of data
    } while ( status_i >= SUCCESSFUL );
 
    return status_i;
}   // send_query_speed_parameters

// ---------------------------------------------------------------------------

int32 query_speedbump_parameters ( click_details_xt * click_px,
				    int argc,
				    char *argv[] )
/*
Function:   query_speed_parameters
Author:     Neil Cossor
Modified:   20010709
Description:

    CMM/PMM wishes to get their speedbump parameters (aka mm_exposure_limit).
    Uses uq103, by calling send_query_speed_parameters

    The query can only be done at the underlying level.

Input Params:

    argv[0]	SPEED
    argv[1]	GET
    argv[2]	underlying name for query | "*"
*/
{
    // status var to return, with good default initial value
    int 		status_i = SUCCESSFUL;
    // local selector series - can be underlying, or instrument class
    ise_series_xt	ise_series_x;
    // the underlying record for the underlying
    ise_underlying_xt	*found_under_data_px;


    // zot the series selector - all underlying, all instrument groups
    memset ( &ise_series_x, 0, sizeof ( ise_series_x ));

    
    // can't do this unless we are logged in
    if ( !click_px->logged_on_i )
    {
	prompt_logged_on ("SPEED,GET parameters");
	return NOT_LOGGED_ON;
    }

    // have we enuff arguments ?
    if ( argc < 3)
    {
	printf ("Insufficent params :- SPEED,GET,<underlying|*>\n");
	return SUCCESSFUL;
    }

    if ( *argv[2]== '*' )
    {
	// for wildcard underlying, we MUST have an allocated bin ?
	if ( click_px->our_market_x.default_bin_u == 0 )
	{
	    printf ("Error - this user is NOT allocated as MM for any bins - wildcard NOT valid\n");
	    return SUCCESSFUL;
	}

	ise_series_x.country_u = click_px->our_market_x.country_u;
	ise_series_x.market_u = click_px->our_market_x.market_u;
	// leave the commodity code as 0 - all underlying in our market

	status_i = send_query_speed_parameters ( click_px, &ise_series_x );
    }
    else
    {
	// one underlying - find the relevant underlying
	found_under_data_px = find_under_by_name_s ( click_px,
							argv[2] );
	if ( !found_under_data_px )
	{
	    // no such underlying
	    printf ("Error : unable to query on unknown underlying %s\n",
			argv[2] );
	    return SUCCESSFUL;
	}   // test for finding this underlying

	// put in the commodity code
	ise_series_x.underlying_code_u = found_under_data_px->commodity_code_u;

	status_i = send_query_speed_parameters ( click_px, &ise_series_x );
    };
  
    return status_i;
}   // query_speed_parameters

// ---------------------------------------------------------------------------

int32 send_query_quote ( click_details_xt * click_px,
			    ise_series_xt *ise_series_px,
			    uint8 orderbook_u,
			    uint8 bin_u )
/*
Function:   send_query_quote
Author:     Neil Cossor
Modified:   200012xx
Description:

    The user has requested the current quote position for this operator
    in the central markeplace. Go get the info,
    using the mq104. 

    Note, this is VERY similar to mq101, query order.. just that it
    is quotes, rather than orders that come back in the response.

Input Params:

    - the series qualifier we are to query on
    - the orderbook we are to query on
    - the bin we are to query on

Output Params:

    If we get an error, we return an error status (-ve),
    otherwise we return the # of quotes found (which may be 0)

    Displays the quotes in some simple form on stdout
*/
{
    generic_txn_xt	OMmsg_x;
    // size of our actual CLICK query/command
    uint32		tx_length_u = sizeof ( OMmsg_x.tx_x.query_quote_x );
    // status var to return, with good default initial value
    int 		status_i = SUCCESSFUL;
    // pointer to the interesting info about one quote in the response
    answer_quote_ma104_item_t	    *txn_reply_px;

    // local copy of the series record we got back in a quote
    ise_series_xt	ise_series_x;
    // pointer to selected series
    ise_series_info_pt	series_data_px = NULL;
    // count of how many quotes we found which matched the caller selection
    int 		quotes_found_i = 0;
    // and the count of how many items we found in other series in the response
    int			quotes_other_series_i = 0;
    uint16		item_index_u,	// local loop var
			items_found_u;	// limit on loop - may be endian-swapped from response
    // order ID of quote we got back from query
    quad_word		search_orderID_u;

    // local-endian copies of fields from query response
    int32		quote_premium_i;
    // the various quantities
    uint32		cust_quantity_u,    // in orders or quotes
			open_quantity_u;
    uint16		isemm_quantity_u,
			firm_quantity_u,
			farmm_quantity_u;

    uint8		bid_ask_u;
    // the current value of the step-up buffer
    uint16		step_up_buffer_u;
    // the current number of levels we have ticked down ?
    uint16		number_of_tick_down_levels_u;
    series_premium_st	premium_s;	    // premium, in string form


    /* set up query to have 0 in all fields - which includes selection qualifiers
	This implies
	- both bids and asks
	- all instrument groups
	- no specific order #
	- all order categories
    */
    memset( &OMmsg_x.tx_x.query_quote_x, 0, tx_length_u );

    // MQ101
    pack_OM_header ( &OMmsg_x, tx_length_u, 'M', 'Q', 104 );

    // put in the specified series selector.. unless we are wildcarding
    if ( ise_series_px != NULL )
	put_click_series ( &OMmsg_x.tx_x.query_quote_x.series,
			    ise_series_px );

    // the specified quotebook
    OMmsg_x.tx_x.query_quote_x.orderbook_c = orderbook_u;
    // and specified bin
    OMmsg_x.tx_x.query_quote_x.bin_c = bin_u;


    // and the whose qualifier - all things for all users in our member
    tx_click_string ( &OMmsg_x.tx_x.query_quote_x.whose.trading_code.country_id_s,
			    &click_px->user_code_x.country_id_s,
			    sizeof (OMmsg_x.tx_x.query_quote_x.whose.trading_code.country_id_s));
    tx_click_string ( &OMmsg_x.tx_x.query_quote_x.whose.trading_code.ex_customer_s,
			    &click_px->user_code_x.customer_id_s,
			    sizeof (OMmsg_x.tx_x.query_quote_x.whose.trading_code.ex_customer_s));
    // NOTE leave the whose.ex_client_s as NULL. If you put anything else in there,
    // the search selects out quotes with exactly that client string...

    // we would like ALL order types - so value 0 (from memset) is correct

    // save the facility code and fn code for logging
    OMmsg_x.fn_code = LOG_K_Q_QUOTE;
    OMmsg_x.facility_u = click_px->facility_no_u + 7;	// EP7/ET7

    
    // now whiz around, doing calls until we run out of matching quotes from the central system
    do // while ( quotes_other_series_i == 0 )
    {
	// active quotes
	status_i = do_click_query_rx ( click_px,
					&OMmsg_x,
					tx_length_u,
					LOG_K_R_QUOTE);
	if ( status_i < SUCCESSFUL)
	{
	    // note no items found - bail out of loop
	    break;
	}
	else
	{
	    // good status - how many quotes did we retrieve - endian
	    PUTSHORT ( items_found_u,
			click_px->log_reply_x.trans_reply_x.answer_quote_x.items_n );

	    /* if we got no items, then we may have to try the next orderbook
		(bin could be split).. except if specific series, which can't be in next OB
	    */
	    if ((items_found_u == 0) && (ise_series_px))
		// none matching, and specific series
		break;

	    // loop thru the quotes we found
	    for ( item_index_u = 0;
		    item_index_u < items_found_u;
		    item_index_u++ )
	    {
		// point to the interesting info about this quote
		txn_reply_px = &click_px->log_reply_x.trans_reply_x.answer_quote_x
					.item[item_index_u];
		// first need to extract copy of the series, in local form
		get_click_series ( &ise_series_x,
				    &txn_reply_px->series );

		// extract one quote - in correct endian order
		PUTQUAD(search_orderID_u,
			    txn_reply_px->order_number_u);

		// get (local-endian) data out of the quote
		PUTLONG ( quote_premium_i,
			    txn_reply_px->premium_i );
		// all of quantities
		PUTLONG ( cust_quantity_u,
			    txn_reply_px->quantity_u );
		PUTSHORT ( farmm_quantity_u,
			    txn_reply_px->farmm_trade_limit_absolute_n );
		PUTSHORT ( firm_quantity_u,
			    txn_reply_px->firm_trade_limit_absolute_n );
		PUTSHORT ( isemm_quantity_u,
			    txn_reply_px->isemm_trade_limit_absolute_n );
		PUTLONG ( open_quantity_u,
			    txn_reply_px->open_quantity_u );
		// what is in the step up buffer at present
		PUTSHORT ( step_up_buffer_u,
			    txn_reply_px->step_up_buffer_n );
		PUTSHORT ( number_of_tick_down_levels_u,
			    txn_reply_px->number_of_tick_down_levels_n );
		bid_ask_u = txn_reply_px->bid_or_ask_c;

		// find the series data for this particular are
		series_data_px = find_series_by_ise_series ( click_px,
								&ise_series_x );

		// format the premium, with correct implied decimals
		format_whole_decimal_s ( (char *)&premium_s,
					    quote_premium_i,
					    series_data_px->instrument_class_px->dec_in_premium_u,
					    MAX_SERIES_PREMIUM );   // width

		// report this quote
		printf ("%s, %s $%s (cust %u, isemm %hu firm %hu, farmm %hu, open %u), step %hu, tickdown %hu\n",
			    &series_data_px->ise_name_s,
			    bid_ask_to_string ( bid_ask_u ),
			    &premium_s,
			    cust_quantity_u,
			    isemm_quantity_u,
			    firm_quantity_u,
			    farmm_quantity_u,
			    open_quantity_u,
			    step_up_buffer_u,
			    number_of_tick_down_levels_u
			);

		// update our info about quotes
		if ( bid_ask_u == BID)
		{
		    // updating bid info
		    series_data_px->bid_quoteID_u = search_orderID_u;
		    series_data_px->quote_bid_premium_i = quote_premium_i;
		    series_data_px->quote_bid_qty_u = cust_quantity_u;
		}
		else
		{
		    // updating ask info
		    series_data_px->ask_quoteID_u = search_orderID_u;
		    series_data_px->quote_ask_premium_i = quote_premium_i;
		    series_data_px->quote_ask_qty_u = cust_quantity_u;
		}

		/* Is this record for the series we asked for ?
		    If not, then don't count it as a 'found' record.
		    NB if we asked for all, we will count all records
		*/
		if (( ise_series_px != NULL ) && ( cmp_series_by_ise_series_under ( &ise_series_x,
										    ise_series_px ) != 0 ))
		{
		    /* we are looking for specific series, and this order is not in that.
			wrong series.

			NB provided that OM packs 'extra' data AFTER stuff that we asked for
			(and no extra data before what matches our specific series)
			then a non-zero value in this var is a good point to exit our
			omniapi_query loop.. as we only increment this var 
		    */
		    quotes_other_series_i++;
		}
		else
		{
		    // note one more found
		    quotes_found_i++;
		}   // test if quote was matching our querying, or 'extra' data packed by OM
	    }	// loop thru items in a response buffer

	    /* If the Orderbook, Next field in the response is 0,
		then we have finished our query.
		
		If that field is NOT zero, then there are more queries to do,
		and we need to shuffle the Next fields into the query, and continue
	    */
	    if ( click_px->log_reply_x.trans_reply_x.answer_quote_x.orderbook_next_c != 0 )
	    {
		/* There is another query to go, according to MP server...
		    We must transfer the "up to here" fields from the response
		    into the next request
		*/
		OMmsg_x.tx_x.query_quote_x.orderbook_c =
		    click_px->log_reply_x.trans_reply_x.answer_quote_x.orderbook_next_c;
		memcpy ( &OMmsg_x.tx_x.query_quote_x.series,
			    &click_px->log_reply_x.trans_reply_x.answer_quote_x.series_next,
			    sizeof (series_t));
		PUTQUAD(OMmsg_x.tx_x.query_quote_x.order_number_u,
		    click_px->log_reply_x.trans_reply_x.answer_quote_x.order_number_next_u);
		OMmsg_x.tx_x.query_quote_x.bid_or_ask_c =
		    click_px->log_reply_x.trans_reply_x.answer_quote_x.bid_or_ask_next_c;
	    }	// test if we maxed out on quotes
	    else
	    {
		/* There are no more quotes. Stop doing queries.
		*/
		break;
	    }
	}   // test if success status from OMNIAPI call
    } while ( quotes_other_series_i == 0 );

    if ( status_i >= SUCCESSFUL )
	return quotes_found_i;
    else
	return status_i;
}   // send_query_quote

// ---------------------------------------------------------------------------

int32 query_quote ( click_details_xt * click_px,
		    int argc,
		    char *argv[] )
/*
Function:   query_quote
Author:     Neil Cossor
Modified:   20000330
Description:

    MM wishes to find what quotes they have in the market
    Uses mq104, by calling send_query_quote

    This was triggered by a 
    QUOTE,QUERY,<*|S>[,<series name>]

Input Params:

    The usual argc/argv from the command line
    argv[2] may have values of
    - '*'   all quotes in all series (and hence all underlying) in the users bin
    - 'S'   specifies that argv[3] is a series name,
	    i.e. collect all quotes in that particular series
*/
{
    // pointer to selected series
    ise_series_info_pt	series_data_px = NULL;
    // pointer to Q'ed block, which contains pointer to series under selected underlying
    gen_buf_node_xt	*series_node_px = NULL;
    // pointer to selected underlying
    ise_underlying_xt	*under_data_px = NULL;
    int 		status_i;
    // how many quotes did we find, overall
    int 		total_quotes_u = 0;
    // temp var, for selecting from possible options on cmd line
    int 		choice_i;
    // pointer to one (chained) node, in the known_bins_q
    gen_buf_node_xt	    *bin_node_px = NULL;
    // pointer to the data component of the node in the known_bins_q
    ise_bin_xt		    *bin_data_px = NULL;

    // were all the required parameters supplied
    // need 3 args, unless wildcard (series)
    if (( argc >= 4)
	|| ( (argc >= 3) && ( *argv[2]='*')) )
    {
	// and which series we wish to work on...
	choice_i = select_string_option ( 2, &under_series_s [0], argv[2] );
	switch ( choice_i )
	{
	case under_series_series_e :
	    // all quotes in series supplied in argv[3]
	    // go find the series info for that series, and specify that
	    series_data_px = find_series_by_name_s ( click_px,
							argv [3] );
	    if (!series_data_px)
	    {
		// oops - did NOT know about that series
		printf ("\tERROR - Unknown Series [%s] in selection for query_quote.\n",
			    argv[3] );
		return SUCCESSFUL;
	    }

	    // we know that all series have an underlying.. so we MUST have found the underlying
	    printf ("Query quote, series %s\n",
			&series_data_px->ise_name_s );
	    status_i = send_query_quote ( click_px,
					    &series_data_px->ise_series_x,  // series qualifier,
					    series_data_px->parent_under_px->orderbook_u,
					    series_data_px->parent_under_px->bin_u );
	    if (status_i > SUCCESSFUL)
		total_quotes_u += status_i;

	    break;

	case under_series_all_e :
	    // all quotes in all series please
	    printf ("Query quote, all series in bin %u\n",
			click_px->our_market_x.default_bin_u );

	    // if we don't have a default bin.. we can't do the query
	    if (click_px->our_market_x.default_bin_u == 0)
	    {
		printf ("Cannot do query quotes wildcard without bin. User is NOT a PMM/CMM\n");
		return SUCCESSFUL;
	    }

	    // find the data about this bin.. including the list of relevant orderbooks
	    bin_node_px = find_bin ( &click_px->our_market_x,
				click_px->our_market_x.default_bin_u );
	    if ( bin_node_px == 0)
	    {
		printf ("Have no bin information for bin %u. OB servers to query unknown\n");
		return SUCCESSFUL;
	    }

	    bin_data_px = (ise_bin_xt *)bin_node_px->msg_p;

	    // orderbook next field in response rolls across OBs
	    // this would be smarter if we figured out which OBs deal with our bin,
	    // and started with lowest, and stopped when we get to OB higher than ours.
	    status_i = send_query_quote ( click_px,
					    NULL,		// no series qualifier
					    1,			// start with OB 1
					    0 );		// do NOT specify a bin
	    if (status_i > SUCCESSFUL)
		total_quotes_u += status_i;

	    break;

	default:
	    {
		// illegal selection qualifier
		printf ("\tERROR - Illegal qualifier %s in selection for query_quote..[*|S]\n",
			    &argv[2] );
		return SUCCESSFUL;
	    }
	}   // end of switch

	// report how many we found
	printf ("Found %u active quotes\n\n",
		    total_quotes_u );

	return status_i;
    }
    else
    {
	printf ("\tERROR - 	QUOTE,QUERY,<*|S>[,<series name>]\n");
	return SUCCESSFUL;
    }
}   // query_quote

// ---------------------------------------------------------------------------

int32 send_query_locked_orders ( click_details_xt * click_px,
				ise_series_info_xt *series_data_px )
/*
Function:   send_query_locked_orders
Author:     Neil Cossor
Modified:   200012xx
Description:

    MM wishes to find what locked orders exist.
    Uses mq106.

    Only the PMM can send this request

    It is very similar in behaviour to the mq101, query order...
    including the weird use of series,next.. and if you ask for a specific
    series, you can get info about other series
*/
{
    generic_txn_xt	OMmsg_x;
    // size of our actual CLICK query/command
    uint32		tx_length_u = sizeof ( OMmsg_x.tx_x.query_locked_x );
    // status var to return, with good default initial value
    int 		status_i = SUCCESSFUL;
    // pointer to one interesting record in the response
    answer_locked_order_ma106_item_t	*txn_reply_px;

    // local copy of the series record we got back in an order
    ise_series_xt	ise_series_x;
    // pointer to selected series
    ise_series_info_pt	series_info_px = NULL;
    // count of how many orders we found which matched the caller selection
    int 		orders_found_i = 0;
    uint16		item_index_u,	// local loop var
			items_found_u;	// limit on loop - may be endian-swapped from response
    // order ID of order we got back from query
    quad_word		search_orderID_u;
    // pointer to uint64 as a structure, so we can dump out each half of order ID
   // uint64_xt		    *uint64_pxu;

    // local-endian copies of fields from query response
    int32		order_premium_i;
    uint32		quantity_u;

    uint8		bid_ask_u;
    uint16		validity_time_u;
    uint8		client_category_u;  // cust/Firm/FARMM
    uint8		lock_type_u;	    // hidden type
    quad_word		order_id_next_u;    // the next order ID
    uint16		order_category_u;   // order, quote, block, facil, cab etc
    // the string form of a premium, with appropriate # of decimal places
    series_premium_st	premium_s;
    // the trading code for an order/quote
    ise_user_code_xt	    user_code_x;

	quad_conv_t * convOrderNo;

    /* set up query to have 0 in all fields - which includes selection qualifiers
	This implies
	- both bids and asks
	- all instrument groups
	- no specific order #
	- all order categories
    */
    memset( &OMmsg_x.tx_x.query_locked_x, 0, tx_length_u );

    // point structure pointer at the 64bit order ID
//    (uint64 *)uint64_pxu = &(search_orderID_u);

    // MQ106
    pack_OM_header ( &OMmsg_x, tx_length_u, 'M', 'Q', 106 );

    // if we were supplied with a series, then use the info for that
    if ( series_data_px )
    {
	// put in the specified series selector
	put_click_series ( &OMmsg_x.tx_x.query_locked_x.series,
			    &series_data_px->ise_series_x );
	// the specified orderbook
	OMmsg_x.tx_x.query_locked_x.orderbook_c = series_data_px->parent_under_px->orderbook_u;
	// and the corresponding bin
	OMmsg_x.tx_x.query_locked_x.bin_c = series_data_px->parent_under_px->bin_u;
    }
    else
    {
	// we want locked orders for ALL series.. this can only be done for MM's bin

	// series is already all 0s i.e. ALL wildcard

	// use orderbook 1, our bin (if we are a CMM or PMM, this is already set at logon from DQ211)
	OMmsg_x.tx_x.query_locked_x.orderbook_c = 1;
	OMmsg_x.tx_x.query_locked_x.bin_c = 0;// click_px->our_market_x.default_bin_u;
//	OMmsg_x.tx_x.query_locked_x.series.country_c = click_px->our_market_x.country_u;
//	OMmsg_x.tx_x.query_locked_x.series.market_c = click_px->our_market_x.market_u;
    }	// test if caller supplied a specific series

    // we want to know about bids and asks - later queries will use the bid/ask from the response
    OMmsg_x.tx_x.query_locked_x.bid_or_ask_c = 0;

    // NOTE leave the whose.ex_client_s as NULL. If you put anything else in there,
    // the search selects out orders with exactly that client string...

    // we would like ALL order types - so value 0 (from memset) is correct

    // save the facility code and fn code for logging
    OMmsg_x.fn_code = LOG_K_Q_LOCKED_ORDER;
    OMmsg_x.facility_u = click_px->facility_no_u + 7;	// EP7/ET7

    
    // now whiz around, doing calls until we run out of orders from the central system
    do
    {
	// active orders
	status_i = do_click_query_rx ( click_px,
					&OMmsg_x,
					tx_length_u,
					LOG_K_R_LOCKED_ORDER);
	if ( status_i >= SUCCESSFUL)
	{
	    // good status - how many orders did we retrieve - endian
	    PUTSHORT ( items_found_u,
			click_px->log_reply_x.trans_reply_x.answer_locked_order_x.items_n );

	    /* if we got no items, then we may have to try the next orderbook
		(bin could be split).. except if specific series, which can't be in next OB
	    */
	    if ((items_found_u == 0) && (series_data_px))
		// none matching, and specific series
		break;

	    // loop thru the orders we found
	    for ( item_index_u = 0;
		    item_index_u < items_found_u;
		    item_index_u++ )
	    {
		// point to this unit of interesting info
		txn_reply_px = &click_px->log_reply_x.trans_reply_x.answer_locked_order_x.item [item_index_u];

		// extract one order - in correct endian order
		PUTQUAD(search_orderID_u,
			    txn_reply_px->order_number_u);

		// get (local-endian) data out of the order
		PUTLONG ( order_premium_i,
			    txn_reply_px->premium_i );
		// all of quantities
		PUTLONG ( quantity_u,
			    txn_reply_px->quantity_u );
		// buy or sell
		bid_ask_u = txn_reply_px->bid_or_ask_c;
		// validity time (irrelevant if quote)
		PUTSHORT ( validity_time_u,
			    txn_reply_px->time_validity_n );
		client_category_u = txn_reply_px->client_category_c;
		lock_type_u = txn_reply_px->lock_type_c;
		// order category
		PUTSHORT ( order_category_u,
			    txn_reply_px->order_category_n );

		// find the series data for this particular are
		// first need to extract copy of the series, in local form
		get_click_series ( &ise_series_x,
				    &txn_reply_px->series );
		series_info_px = find_series_by_ise_series ( click_px,
								&ise_series_x );
		// format the premium for output
		format_whole_decimal_s ( (char *)&premium_s,
					    order_premium_i,
					    series_info_px->instrument_class_px->dec_in_premium_u,
					    MAX_SERIES_PREMIUM );   // allowed width
		// grab the trading code info == user_code info
		get_ise_user_code_xt ( &user_code_x,
					(user_code_t *)&txn_reply_px->trading_code );

		switch ( order_category_u )
		{
		    // ???? probably need to display some of these orders in different formats...
		case ORDER_CAT_ORDER:
		case ORDER_CAT_BLOCK:
		case ORDER_CAT_BLOCK_RESPONSE:
		case ORDER_CAT_FACILITATION:
		case ORDER_CAT_FACILITATION_RESPONSE:
		case ORDER_CAT_CAB:
		case ORDER_CAT_DERIVED:
		    if ( txn_reply_px->order_type_c == ORDER_TYPE_MARKET )
			// market price - show MARKET, and not price
			printf ( "%-16s%-16s %-s %hu@%-s",
				    order_category_to_string ( order_category_u ),
				    &series_info_px->ise_name_s,
				    bid_ask_to_string ( bid_ask_u ),
				    quantity_u,
				    order_type_to_string (txn_reply_px->order_type_c )
				);
		    else    // limit price
			printf ( "%-16s%-16s %-s %hu@%-s",
				    order_category_to_string ( order_category_u ),
				    &series_info_px->ise_name_s,
				    bid_ask_to_string ( bid_ask_u ),
				    quantity_u,
				    &premium_s
				);
		    break;

		case ORDER_CAT_IMPROVE_QUOTE:
			printf ("IMPROVEMENT QUOTE - illegal for ISE");

		    break;

		default:
		    printf ( "%Warning - Unknown order category %hu, %-16s %-s %hu@%-s",
				order_category_u,
				&series_info_px->ise_name_s,
				bid_ask_to_string ( bid_ask_u ),
				quantity_u,
				&premium_s
			    );
		}   // switch on order type
		convOrderNo = (quad_conv_t *) &(search_orderID_u.quad_word);

		printf ( " ID %08lX:%08lX, %-s %-s, for %s BD %s oper %s",
				convOrderNo->l2,
				convOrderNo->l1,
			    validity_time_to_string ( validity_time_u ),
			    client_category_to_string ( txn_reply_px->client_category_c ),
			    &user_code_x.country_id_s,
			    &user_code_x.customer_id_s,
			    &user_code_x.user_id_s
			);
		if ( txn_reply_px->lock_type_c != 0 )
		    printf ( ", lock %s\n",
			    lock_type_to_string ( txn_reply_px->lock_type_c )
			    );
		else
		    printf ("\n");

		// note one more found
		orders_found_i++;
	    }

	    /* If we got no records then we have finished our query.
		
		If that field is NOT zero, then there are more queries to do,
		and we need to shuffle the Next fields into the query, and continue
	    */
	    PUTQUAD(order_id_next_u,
			click_px->log_reply_x.trans_reply_x.answer_ob_x.order_number_next_u);
	    if ( memcmp(&(order_id_next_u.quad_word), &blank, sizeof(blank)) == 0 )
	    {
		/* There is another query to go, according to MP server...
		    We must transfer the "up to here" fields from the response
		    into the next request
		*/
		memcpy ( &OMmsg_x.tx_x.query_locked_x.series,
			    &click_px->log_reply_x.trans_reply_x.answer_ob_x.series_next,
			    sizeof (series_t));
		PUTQUAD(OMmsg_x.tx_x.query_locked_x.order_number_u,
		    click_px->log_reply_x.trans_reply_x.answer_ob_x.order_number_next_u);
		OMmsg_x.tx_x.query_locked_x.bid_or_ask_c =
		    click_px->log_reply_x.trans_reply_x.answer_ob_x.bid_or_ask_next_c;
	    }	// test if we maxed out on orders
	    else
	    {
		// There are no more orders. Stop doing queries.
		break;
	    }
	}   // test if success status from OMNIAPI call
    } while ( status_i >= SUCCESSFUL );

    return status_i;
}   // send_query_locked_orders

// ---------------------------------------------------------------------------

int32 query_locked_orders ( click_details_xt * click_px,
			    int argc,
			    char *argv[] )
/*
Function:   query_locked_orders
Author:     Neil Cossor
Modified:   20000521
Description:

    MM wishes to find what locked orders there are - for ALL users.
    Uses mq106, by calling send_query_locked_orders

Input Params:

    the usual command line argc, argv[]
    argv[0]	command that got us here
    argv[1]	S[eries] | U[nderlying] | * (all in bin) - all in bin not supported yet
    argv[2]	<series name> | <underlying name> we are querying on

*/
{
    int 		status_i = SUCCESSFUL;
    // pointer to the specific series we are querying on
    ise_series_info_xt	*series_data_px;
    // and the number found from query locks (mq106)
    unsigned		total_locks_found_u = 0;
    // temp var, for selecting from possible options on cmd line
    int 		choice_i;

    // logged on ?
    if (!click_px->logged_on_i )
    {
	prompt_logged_on ("LOCKED query");
	return NOT_LOGGED_ON;
    }

    // enuff arguments ?.. just the command, or the command with something other than * as the 2nd
    if (( argc == 1 )
	|| (( argc == 2 ) && (strcmp (argv[1],"*")!=0)))
    {
	printf ("LOCKED,S[eries]|*[,<series name>]\n");
	return SUCCESSFUL;
    }

    // and which series we wish to work on...
    choice_i = select_string_option ( 3, &under_series_s [0], argv[1] );
    switch ( choice_i )
    {
    case under_series_series_e :
	// one series - search for that series
	series_data_px = find_series_by_name_s ( click_px, argv[2] );
	if (!series_data_px)
	{
	    printf ("Error - unknown series specified [%s]\n",
			argv[2] );
	    return SUCCESSFUL;
	}

	// go do the query - this does appropriate dumping to stdout
	status_i = send_query_locked_orders ( click_px,
					    series_data_px );

	if (status_i > 0)
	    total_locks_found_u += status_i;

	break;
    case under_series_all_e:
	// all series in bin - nothing to set up.. rest is done by routine

	// go do the query - this does appropriate dumping to stdout
	// NB do NOT supply a series pointer for the query
	status_i = send_query_locked_orders ( click_px, NULL );

	if (status_i > 0)
	    total_locks_found_u += status_i;

	break;
    default:
	printf ("Error - user must select one of S[eries] | *\n");
    }	// switch on Series / Underlying selector



    printf ("Found %u locks matching query selection\n",
		total_locks_found_u );

    return status_i;
}   // query_locked_orders

// ---------------------------------------------------------------------------

int32 send_query_orderbook ( click_details_xt * click_px,
				ise_series_info_xt *series_data_px,
				unsigned gtc_internal_u,
#ifdef WIN32
				HANDLE outputfile_px
#else
				FILE *outputfile_px
#endif
			    )
/*
Function:   send_query_orderbook
Author:     Neil Cossor
Modified:   20011015
Description:

    MM wishes to find what the market orderbook looks like - for ALL users.
    Uses mq103

    Only the PMM can send this request

    Note that this includes ordinary and derived orders.
    It does NOT included any locked orders, which are available using
    the mq106, PMM Query Locked Order transaction.

    It is very similar in behaviour to the mq101, query order...
    including the weird use of series,next.. and if you ask for a specific
    series, you can get info about other series

Input Params:

    - the standard everything-we-know-about-click structure
    - pointer to the data structure for the specific series we are querying on
    - flag.. if TRUE then we are doing internal MQ359 query (to count GTCs)
    - file-pointer of where to spit out the info we retrieved. This can be stdout.

Output Params:

    spits out (on stdout) any records returned.

    If an error occurs (-ve) then returns that status,
    otherwise returns the # of records found, which may be 0
*/
{
	quad_conv_t * convOrderNo;
    generic_txn_xt	OMmsg_x;
    // size of our actual CLICK query/command
    uint32		tx_length_u = sizeof ( OMmsg_x.tx_x.query_ob_x );
    // status var to return, with good default initial value
    int 		status_i = SUCCESSFUL;
    // pointer to one interesting record in the response
    answer_ob_ma103_item_t  *txn_reply_px;

    // local copy of the series record we got back in an order
    ise_series_xt	ise_series_x;
    // pointer to selected series
    ise_series_info_pt	series_info_px = NULL;
    // count of how many orders we found which matched the caller selection
    int 		orders_found_i = 0;
    // count of GTC orders for this call
    int			gtc_found_i = 0;
    uint16		item_index_u,	// local loop var
			items_found_u;	// limit on loop - may be endian-swapped from response
    // order ID of order/quote we got back from query
    quad_word		search_orderID_u;
    // pointer to uint64 as a structure, so we can dump out each half of order/quote ID
    //uint64_xt		    *uint64_pxu;

    // local-endian copies of fields from query response
    int32		order_premium_i;
    // the various quantities
    uint32		cust_quantity_u,    // in orders or quotes
			open_quantity_u;
    uint16		isemm_quantity_u,
			firm_quantity_u,
			farmm_quantity_u;

    uint8		bid_ask_u;
    uint16		validity_time_u;
    uint8		order_type_u;	    // limit/market
    uint16		order_category_u;   // order, quote, block, cab etc
    uint8		client_category_u;  // cust/Firm/FARMM
    uint8		lock_type_u;	    // hidden type
    quad_word		order_id_next_u;    // the next order ID
    // the string form of a premium, with appropriate # of decimal places
    series_premium_st	premium_s;
    // the trading code for an order/quote
    ise_user_code_xt	    user_code_x;
    // string buffer we write our output into, so we can use it for Win32 specific, or generic write
    char		text_buffer_s[256];
    // pointer where we are up to in writing into the text buffer
    unsigned		text_buffer_px;
#ifdef WIN32
    // target var for how much data is written, required for WriteFile operation
    LPDWORD		bytes_written_u = 0;
#endif


    /* set up query to have 0 in all fields - which includes selection qualifiers
	This implies
	- both bids and asks
	- all instrument groups
	- no specific order #
	- all order categories
    */
    memset( &OMmsg_x.tx_x.query_ob_x, 0, tx_length_u );
    // point structure pointer at the 64bit order ID
    //(uint64 *)uint64_pxu = (uint64 *)&search_orderID_u;

    // are we doing external PMM query (MQ103) or internal query (MQ103+256)
    if ( gtc_internal_u == TRUE )
    {
	// MQ103+256
	pack_OM_header ( &OMmsg_x, tx_length_u, 'M', 'Q', 359 );
    }
    else
    {
	// MQ103
	pack_OM_header ( &OMmsg_x, tx_length_u, 'M', 'Q', 103 );
    }	// test if specific series or underlying

    // put in the specified series selector
    put_click_series ( &OMmsg_x.tx_x.query_ob_x.series,
			&series_data_px->ise_series_x );
    // the specified orderbook
    OMmsg_x.tx_x.query_ob_x.orderbook_c = series_data_px->parent_under_px->orderbook_u;

    // we want to know about bids and asks - later queries will had the bid/ask from the response
    OMmsg_x.tx_x.query_ob_x.bid_or_ask_c = 0;

    // NOTE leave the whose.ex_client_s as NULL. If you put anything else in there,
    // the search selects out orders with exactly that client string...

    // we would like ALL order types - so value 0 (from memset) is correct

    // save the facility code and fn code for logging
    OMmsg_x.fn_code = LOG_K_Q_ORDERBOOK;
    OMmsg_x.facility_u = click_px->facility_no_u + 7;	// EP7/ET7

    
    // now whiz around, doing calls until we run out of orders from the central system
    do
    {
	// active orders
	status_i = do_click_query_rx ( click_px,
					&OMmsg_x,
					tx_length_u,
					LOG_K_R_ORDERBOOK );
	if ( status_i >= SUCCESSFUL)
	{
	    // good status - how many orders did we retrieve - endian
	    PUTSHORT ( items_found_u,
			click_px->log_reply_x.trans_reply_x.answer_ob_x.items_n );

	    // if we got no orders, then we are done.. this query is only one series
	    // can NOT flow onto next OB
	    if ( items_found_u == 0 )
		break;

	    // loop thru the orders we found
	    for ( item_index_u = 0;
		    item_index_u < items_found_u;
		    item_index_u++ )
	    {
		// point to this unit of interesting info
		txn_reply_px = &click_px->log_reply_x.trans_reply_x.answer_ob_x.item [item_index_u];
		// set pointer into text buffer for output
		text_buffer_px = (unsigned)&text_buffer_s;

		// extract one order - in correct endian order
		PUTQUAD( search_orderID_u,
			    txn_reply_px->order_number_u);

		// get (local-endian) data out of the order
		PUTLONG ( order_premium_i,
			    txn_reply_px->premium_i );
		// all of quantities
		PUTLONG ( cust_quantity_u,
			    txn_reply_px->quantity_u );
		PUTSHORT ( farmm_quantity_u,
			    txn_reply_px->farmm_trade_limit_absolute_n );
		PUTSHORT ( isemm_quantity_u,
			    txn_reply_px->isemm_trade_limit_absolute_n );
		PUTSHORT ( firm_quantity_u,
			    txn_reply_px->firm_trade_limit_absolute_n );
		PUTLONG ( open_quantity_u,
			    txn_reply_px->open_quantity_u );
		// buy or sell
		bid_ask_u = txn_reply_px->bid_or_ask_c;
		// validity time (irrelevant if quote)
		PUTSHORT ( validity_time_u,
			    txn_reply_px->time_validity_n );
		// is this a GTC
		if (( validity_time_u == VALIDITY_TIME_OLD_GTC ) ||
		    ( validity_time_u == VALIDITY_TIME_FAS_GTC ) ||
		    ( validity_time_u == VALIDITY_TIME_AON_GTC ))
		    // one more found
		    gtc_found_i++;

		client_category_u = txn_reply_px->client_category_c;
		order_type_u = txn_reply_px->order_type_c;
		lock_type_u = txn_reply_px->lock_type_c;
		PUTSHORT ( order_category_u, 
			    txn_reply_px->order_category_n );

		// find the series data for this particular are
		// first need to extract copy of the series, in local form
		get_click_series ( &ise_series_x,
				    &txn_reply_px->series );
		series_info_px = find_series_by_ise_series ( click_px,
								&ise_series_x );
		// format the premium for output
		format_whole_decimal_s ( (char *)&premium_s,
					    order_premium_i,
					    series_info_px->instrument_class_px->dec_in_premium_u,
					    MAX_SERIES_PREMIUM );   // allowed width
		// grab the trading code info == user_code info
		get_ise_user_code_xt ( &user_code_x,
					(user_code_t *)&txn_reply_px->trading_code );

		convOrderNo = (quad_conv_t *) &(search_orderID_u.quad_word);

		switch ( order_category_u )
		{
		    // ???? probably need to display some of these orders in different formats...
		case ORDER_CAT_ORDER:
		case ORDER_CAT_BLOCK:
		case ORDER_CAT_BLOCK_RESPONSE:
		case ORDER_CAT_FACILITATION:
		case ORDER_CAT_FACILITATION_RESPONSE:
		case ORDER_CAT_CAB:
		case ORDER_CAT_DERIVED:
		    if ( txn_reply_px->order_type_c == ORDER_TYPE_MARKET )
			// market price - show MARKET, and not price
			text_buffer_px += sprintf ( (char *)(text_buffer_px),
						    "%-16s%-16s %-s %hu@%-s",
						    order_category_to_string ( order_category_u ),
						    &series_info_px->ise_name_s,
						    bid_ask_to_string ( bid_ask_u ),
						    cust_quantity_u,
						    order_type_to_string (txn_reply_px->order_type_c )
						    );
		    else    // limit price
			text_buffer_px += sprintf ( (char *)(text_buffer_px),
						    "%-16s%-16s %-s %hu@%-s",
						    order_category_to_string ( order_category_u ),
						    &series_info_px->ise_name_s,
						    bid_ask_to_string ( bid_ask_u ),
						    cust_quantity_u,
						    &premium_s
						    );
			convOrderNo = (quad_conv_t *) &(search_orderID_u.quad_word);
		    text_buffer_px += sprintf ( (char *)(text_buffer_px),
						", orderID %08lX:%08lX %-s %-s, for %s BD %s oper %s",
						convOrderNo->l2,
						convOrderNo->l1,
						validity_time_to_string ( validity_time_u ),
						client_category_to_string ( txn_reply_px->client_category_c ),
						&user_code_x.country_id_s,
						&user_code_x.customer_id_s,
						&user_code_x.user_id_s
						);
		    if ( txn_reply_px->lock_type_c != 0 )
			text_buffer_px += sprintf ( (char *)(text_buffer_px),
						    ", lock %s",
						    lock_type_to_string ( txn_reply_px->lock_type_c )
						    );
		    break;

		case ORDER_CAT_QUOTE:
			convOrderNo = (quad_conv_t *) &(search_orderID_u.quad_word);
			text_buffer_px += sprintf ( (char *)(text_buffer_px),
				    "%-16s%-16s %-s %hu@%-s, %s open qty %hu, firm %hu, farmm %hu, orderID %08lX:%08lX, for %s MM %s oper %s",
				    order_category_to_string ( order_category_u ),
				    &series_info_px->ise_name_s,
				    bid_ask_to_string ( bid_ask_u ),
				    cust_quantity_u,
				    &premium_s,
				    validity_time_to_string ( validity_time_u ),
				    open_quantity_u,
				    firm_quantity_u,
				    farmm_quantity_u,
				    convOrderNo->l2,
					convOrderNo->l1,
				    &user_code_x.country_id_s,
				    &user_code_x.customer_id_s,
				    &user_code_x.user_id_s
				);
		    break;
		default:
		    text_buffer_px += sprintf ( (char *)(text_buffer_px),
						"%Warning - Unknown order category %hu, %-16s %-s %hu@%-s, orderID %08lX:%08lX, for %s MM %s oper %s",
						order_category_u,
						&series_info_px->ise_name_s,
						bid_ask_to_string ( bid_ask_u ),
						cust_quantity_u,
						&premium_s,
						convOrderNo->l2,
						convOrderNo->l1,
						&user_code_x.country_id_s,
						&user_code_x.customer_id_s,
						&user_code_x.user_id_s
						);
		}   // switch on order type

		// note one more found
		orders_found_i++;

		// dump the formatted text out to the file / screen
#ifdef WIN32
		if ( gtc_internal_u == TRUE )
		{
		    // add \r\n for file, otherwise we get weird EOL chars
		    sprintf ( (char *)(text_buffer_px), "\r\n" );
		    WriteFile ( outputfile_px,
				&text_buffer_s,
				strlen (text_buffer_s),
				(unsigned *)&bytes_written_u,
				NULL    // no "overlap" control
				);
		}
		else
		    printf ( "%s\n", &text_buffer_s );
#else
		fprintf ( outputfile_px, "%s\n", &text_buffer_s );
#endif
	    }

	    /* If we got no records then we have finished our query.
		
		If that field is NOT zero, then there are more queries to do,
		and we need to shuffle the Next fields into the query, and continue
	    */
	    PUTQUAD(order_id_next_u, 
			click_px->log_reply_x.trans_reply_x.answer_ob_x.order_number_next_u);
	    if ( memcmp(&order_id_next_u, &blank, sizeof(blank)) == 0 )
	    {
		/* There is another query to go, according to MP server...
		    We must transfer the "up to here" fields from the response
		    into the next request
		*/
		memcpy ( &OMmsg_x.tx_x.query_ob_x.series,
			    &click_px->log_reply_x.trans_reply_x.answer_ob_x.series_next,
			    sizeof (series_t));
		PUTQUAD(OMmsg_x.tx_x.query_ob_x.order_number_u,
		    click_px->log_reply_x.trans_reply_x.answer_ob_x.order_number_next_u);
		OMmsg_x.tx_x.query_ob_x.bid_or_ask_c =
		    click_px->log_reply_x.trans_reply_x.answer_ob_x.bid_or_ask_next_c;
	    }	// test if we maxed out on orders
	    else
	    {
		// There are no more orders. Stop doing queries.
		break;
	    }
	}   // test if success status from OMNIAPI call
    } while ( status_i >= SUCCESSFUL );

    if ( status_i < SUCCESSFUL )
	return status_i;
    else
    {
	if ( gtc_internal_u == TRUE )
	    return gtc_found_i;
	else
	    return orders_found_i;
    }
}   // send_query_orderbook

// ---------------------------------------------------------------------------

int32 query_orderbook ( click_details_xt * click_px,
			int argc,
			char *argv[] )
/*
Function:   query_orderbook
Author:     Neil Cossor
Modified:   20010618
Description:

    MM wishes to find what the market orderbook looks like - for ALL users.
    Uses mq103, by calling send_query_orderbook
    and mq106, by calling send_query_locked_orders

Input Params:

    the usual command line argc, argv[]
    argv[0]	command that got us here
    argv[1]	S[eries] | U[nderlying] | * (all in bin) - all in bin not supported yet
    argv[2]	<series name> | <underlying name> we are querying on

To do:

    Is a wildcard option allowing a query on all underlying (which have series)
    for which this user is the PMM, a good idea.. would take a while
*/
{
    int 		status_i = SUCCESSFUL;
    // pointer to Q'ed node, which contains pointer to info about a series
    gen_buf_node_xt	*series_node_px;
    // pointer to the specific series we are querying on
    ise_series_info_xt	*series_data_px;
    // pointer to the specific underlying we are querying all the series on
    ise_underlying_xt	*under_data_px;
    // total # of orders & quotes we found (mq103)
    unsigned		    total_orders_found_u = 0;
    // and the number found from query locks (mq106)
    unsigned		total_locks_found_u = 0;
    // temp var, for selecting from possible options on cmd line
    int 		choice_i;


    // logged on ?
    if (!click_px->logged_on_i )
    {
	prompt_logged_on ("ORDERBOOK query");
	return NOT_LOGGED_ON;
    }

    // enuff arguments ?
    if ( argc < 3 )
    {
	printf ("ORDERBOOK,S[eries]|U[nderlying],<series name>|<underlying name>\n");
	return SUCCESSFUL;
    }

    // and which series we wish to work on...series or underlying
    choice_i = select_string_option ( 2, &under_series_s [0], argv[1] );
    switch ( choice_i )
    {
    case under_series_series_e :
	// one series - search for that series
	series_data_px = find_series_by_name_s ( click_px, argv[2] );
	if (!series_data_px)
	{
	    printf ("Error - unknown series specified [%s]\n",
			argv[2] );
	    return SUCCESSFUL;
	}

	// go do the query - this does appropriate dumping to stdout
	status_i = send_query_orderbook ( click_px,
					    series_data_px,
					    FALSE,
					    stdout );

	if ( status_i > 0 )
	{
	    total_orders_found_u += status_i;

	    // go do the locked order query - this does appropriate dumping to stdout
	    status_i = send_query_locked_orders ( click_px,
						    series_data_px );

	    if (status_i > 0)
		total_locks_found_u += status_i;
	}

	break;
    case under_series_under_e :
	// all series in one underlying - search for that underlying
	under_data_px = find_under_by_name_s ( click_px, argv[2] );
	if (!under_data_px)
	{
	    printf ("Error - unknown underlying specified [%s]\n",
			argv[2] );
	    return SUCCESSFUL;
	}

	// loop thru the series in that underlying
	series_node_px = (gen_buf_node_xt *)under_data_px->under_series_hdr_x.chain.next;
	if ( series_node_px == NULL )
	    printf ("Warning - no series in underlying [%s]for orderbook query\n",
			&under_data_px->under_id_s );

	while ( series_node_px )
	{
	    series_data_px = (ise_series_info_xt *)series_node_px->msg_p;

	    // go do the query - this does appropriate dumping to stdout
	    status_i = send_query_orderbook ( click_px,
						series_data_px,
						FALSE,
						stdout );

	    if ( status_i < 0 )
		// error has been reported, but bail out of trying others
		break;
	    else
	    {
		// good status - may have found some more records
		total_orders_found_u += status_i;

		// go do the locked order query - this does appropriate dumping to stdout
		status_i = send_query_locked_orders ( click_px,
							series_data_px );

		if (status_i > 0)
		    total_locks_found_u += status_i;
	    }

	    // advance to the next series for this underlying
	    series_node_px = (gen_buf_node_xt *)series_node_px->chain.next;
	}

	break;

    default:
	printf ("Error - user must select one of S[eries] | U[nderlying]\n");
    }	// switch on Series / Underlying selector



    printf ("Found %u items in the orderbook, and %u locks matching query selection\n",
		total_orders_found_u,
		total_locks_found_u );

    return status_i;
}   // query_orderbook

// ---------------------------------------------------------------------------

int32 count_gtc ( click_details_xt * click_px,
		    int argc,
		    char *argv[] )
/*
Function:   count_gtc
Author:     Neil Cossor
Modified:   20011015
Description:

    MM wishes to count GTC orders
    Uses mq359, by calling send_query_orderbook with count_gtc flag set true

Input Params:

    the usual command line argc, argv[]
    argv[0]	command that got us here i.e. COUNT
    argv[1]	GTC
    argv[2]	S[eries] | U[nderlying] | BIN | * (all in bin)
    argv[3]	<series name> | <underlying name> | bin # we are querying on

*/
{
    // filename of our orderbook file
    char		filename_s[256];
    int 		status_i = SUCCESSFUL;
    // pointer to Q'ed node, which contains pointer to info about a series
    gen_buf_node_xt	*series_node_px;
    // pointer to the specific series we are querying on
    ise_series_info_xt	*series_data_px;
    // pointer to Q'ed node, which contanis pointer to info about one underlying
    gen_buf_node_xt	*under_node_px;
    // pointer to the specific underlying we are querying all the series on
    ise_underlying_xt	*under_data_px;
    // pointer to one (chained) node, in the known_bins_q
    gen_buf_node_xt	    *bin_node_px = NULL;
    // pointer to the data component of the node in the known_bins_q
    ise_bin_xt		    *bin_data_px = NULL;
    // total # of orders & quotes we found (mq103)
    unsigned		    total_orders_found_u = 0;
    // temp var, for selecting from possible options on cmd line
    int 		choice_i;
#ifdef WIN32
    // we wish to create a exclusive-write file (interaction with monitoring code for parallel counting)
    HANDLE		orderbook_px;
    // and WriteFile requires a buffer to write from
    char		text_buffer_s[256];
    // target var for how much data is written, required for WriteFile operation
    LPDWORD		bytes_written_u = 0;
#else
    // file used for logging the orderbook (MQ359)
    FILE *orderbook_px;
#endif


    // logged on ?
    if (!click_px->logged_on_i )
    {
	prompt_logged_on ("Internal Orderbook (GTC) query");
	return NOT_LOGGED_ON;
    }

    // enuff arguments ?
    if ( argc < 3 )
    {
	printf ("COUNT,GTC,S[eries]|U[nderlying]|B[in]|*,<series name>|<underlying name>|<bin #>\n");
	return SUCCESSFUL;
    }

    // create a new output file to put the info into.. name includes business date
    strcpy ( filename_s, "orderbook_");
    strcat ( filename_s, click_px->current_business_date_s );
    // for anything except *, we should append the last argument to the filename
    // so we get a name which includes the seriesname, underyling name or bin#
    if ((argc >= 4) && (argv[3]!=NULL))
    {
	strcat ( filename_s, "_" );
	strcat ( filename_s, argv[3] );
    }

    strcat ( filename_s, ".txt");
#ifdef WIN32
    // open file, create new, exclusive write
    orderbook_px = CreateFile ( (char *)&filename_s, 
				GENERIC_WRITE,	    // we wish to be able to write to the file
				0,		    // no sharing on the file, no readers allowed to peak
				NULL,		    // default security
				CREATE_ALWAYS,	    // new file, delete old if exists
				FILE_ATTRIBUTE_NORMAL,	// nothing special about the file type
				NULL );		    // no template file for special attributes
    if (orderbook_px == INVALID_HANDLE_VALUE)
    {
	printf ("error creating orderbook file %s\n", filename_s );
	return -1;
    }
#else
    orderbook_px = fopen ( filename_s, "w" );
    if (orderbook_px == NULL )
    {
	printf ("error creating orderbook file %s\n", filename_s );
	return -1;
    }
#endif

    // and which series we wish to work on...
    choice_i = select_string_option ( 4, &bin_under_series_s [0], argv[2] );
    switch ( choice_i )
    {
    case bin_under_series_series_e :
	// one series - search for that series
	series_data_px = find_series_by_name_s ( click_px, argv[3] );
	if (!series_data_px)
	{
	    printf ("Error - unknown series specified [%s]\n",
			argv[2] );
	    return SUCCESSFUL;
	}

	// go do the query - this does appropriate dumping to stdout
	status_i = send_query_orderbook ( click_px,
					    series_data_px,
					    TRUE,
					    orderbook_px );

	if ( status_i < 0 )
	    // error has been reported, but bail out of trying others
	    break;
	else
	{
	    // good status - may have found some more records
	    total_orders_found_u += status_i;
	}

	printf ( "Series %s has %u GTC orders\n",
		    series_data_px->ise_name_s,
		    total_orders_found_u );

	sprintf ( (char *)&text_buffer_s,
		    "Series %s has %u GTC orders\r\n",
		    series_data_px->ise_name_s,
		    total_orders_found_u );
#ifdef WIN32
	WriteFile ( orderbook_px,
		    &text_buffer_s,
		    strlen (text_buffer_s),
		    (unsigned *)&bytes_written_u,
		    NULL    // no "overlap" control
		    );
#else
	fprintf ( orderbook_px, "%s", &text_buffer_s );
#endif

	break;

    case bin_under_series_under_e :
	// all series in one underlying - search for that underlying
	under_data_px = find_under_by_name_s ( click_px, argv[3] );
	if (!under_data_px)
	{
	    printf ("Error - unknown underlying specified [%s]\n",
			argv[2] );
	    return SUCCESSFUL;
	}

	// loop thru the series in that underlying
	series_node_px = (gen_buf_node_xt *)under_data_px->under_series_hdr_x.chain.next;
	if ( series_node_px == NULL )
	    printf ("Warning - no series in underlying [%s] for orderbook query\n",
			&under_data_px->under_id_s );

	while ( series_node_px )
	{
	    series_data_px = (ise_series_info_xt *)series_node_px->msg_p;

	    // go do the query - this does appropriate dumping to stdout
	    status_i = send_query_orderbook ( click_px,
						series_data_px,
						TRUE,
						orderbook_px );

	    if ( status_i < 0 )
		// error has been reported, but bail out of trying others
		break;
	    else
		total_orders_found_u += status_i;

	    // advance to the next series for this underlying
	    series_node_px = (gen_buf_node_xt *)series_node_px->chain.next;
	}

	printf ( "Underlying %s has %u GTC orders\n",
		    under_data_px->under_id_s,
		    total_orders_found_u );
	sprintf ( (char *)&text_buffer_s, 
		    "Underlying %s has %u GTC orders\r\n",
		    under_data_px->under_id_s,
		    total_orders_found_u );
#ifdef WIN32
	WriteFile ( orderbook_px,
		    &text_buffer_s,
		    strlen (text_buffer_s),
		    (unsigned *)&bytes_written_u,
		    NULL    // no "overlap" control
		    );
#else
	fprintf ( orderbook_px, "%s", &text_buffer_s );
#endif

	break;

    case bin_under_series_bin_e:
	// all series in all underlying in one bins
	choice_i = read_unsigned_limited ( argv[3], click_px->our_market_x.max_bin_u );

	// lets see if we know about this bin ?
	bin_node_px = find_bin ( &click_px->our_market_x,
				    (uint8)choice_i );

	// if we now aren't pointing to a bin block, then we must create one
	if ( !bin_node_px )
	{
	    printf ("Unknown bin [%s]\n",
			argv[3] );
	    return SUCCESSFUL;
	}

	// we found the bin - dump out the interesting stuff
	bin_data_px = (ise_bin_xt *)bin_node_px->msg_p;

	// now, display the underlying in the bin - point to the first
	under_node_px = (gen_buf_node_xt *)bin_data_px->bin_under_hdr_x.chain.next;
	while ( under_node_px )
	{
	    // point to the underlying
	    under_data_px = (ise_underlying_xt *)under_node_px->msg_p;

	    // zero the count of GTCs for this underlying
	    total_orders_found_u = 0;

	    // loop thru the series in that underlying
	    series_node_px = (gen_buf_node_xt *)under_data_px->under_series_hdr_x.chain.next;
	    if ( series_node_px == NULL )
		printf ("Warning - no series in underlying [%s]for orderbook query\n",
			    &under_data_px->under_id_s );

	    while ( series_node_px )
	    {
		series_data_px = (ise_series_info_xt *)series_node_px->msg_p;

		// go do the query - this does appropriate dumping to nominated file
		status_i = send_query_orderbook ( click_px,
						    series_data_px,
						    TRUE,
						    orderbook_px );

		if ( status_i < 0 )
		    // error has been reported, but bail out of trying others
		    break;
		else
		{
		    // good status - may have found some more records
		    total_orders_found_u += status_i;
		}

		// advance to the next series for this underlying
		series_node_px = (gen_buf_node_xt *)series_node_px->chain.next;
	    }

	    printf ( "Underlying %s has %u GTC orders\n",
			under_data_px->under_id_s,
			total_orders_found_u );
	    sprintf ( (char *)&text_buffer_s,
			"Underlying %s has %u GTC orders\r\n",
			under_data_px->under_id_s,
			total_orders_found_u );

#ifdef WIN32
	    WriteFile ( orderbook_px,
			&text_buffer_s,
			strlen (text_buffer_s),
			(unsigned *)&bytes_written_u,
			NULL    // no "overlap" control
			);
#else
	    fprintf ( orderbook_px, "%s", &text_buffer_s );
#endif
	    // try for the next
	    under_node_px = (gen_buf_node_xt *)under_node_px->chain.next;
	}	// while we have a underlying node in the bin's chain

	break;

    case bin_under_series_all_e :
	// all series in all underlying in all bins

	// point to first in Q
	under_node_px = (gen_buf_node_xt *)click_px->our_market_x.known_underlying_q.chain.next;
	while ( under_node_px )
	{
	    under_data_px = (ise_underlying_xt *)under_node_px->msg_p;

	    // zero the count of GTCs for this underlying
	    total_orders_found_u = 0;

	    // loop thru the series in that underlying
	    series_node_px = (gen_buf_node_xt *)under_data_px->under_series_hdr_x.chain.next;
	    if ( series_node_px == NULL )
		printf ("Warning - no series in underlying [%s]for orderbook query\n",
			    &under_data_px->under_id_s );

	    while ( series_node_px )
	    {
		series_data_px = (ise_series_info_xt *)series_node_px->msg_p;

		// go do the query - this does appropriate dumping to nominated file
		status_i = send_query_orderbook ( click_px,
						    series_data_px,
						    TRUE,
						    orderbook_px );

		if ( status_i < 0 )
		    // error has been reported, but bail out of trying others
		    break;
		else
		{
		    // good status - may have found some more records
		    total_orders_found_u += status_i;
		}

		// advance to the next series for this underlying
		series_node_px = (gen_buf_node_xt *)series_node_px->chain.next;
	    }

	    printf ( "Underlying %s has %u GTC orders\n",
			under_data_px->under_id_s,
			total_orders_found_u );
	    sprintf ( (char *)&text_buffer_s,
			"Underlying %s has %u GTC orders\r\n",
			under_data_px->under_id_s,
			total_orders_found_u );

#ifdef WIN32
	    WriteFile ( orderbook_px,
			&text_buffer_s,
			strlen (text_buffer_s),
			(unsigned *)&bytes_written_u,
			NULL    // no "overlap" control
			);
#else
	    fprintf ( orderbook_px, "%s", &text_buffer_s );
#endif

	    // advance to the following Q entry, which may be NULL
	    under_node_px = (gen_buf_node_xt *)under_node_px->chain.next;
	}

    default:
	printf ("Error - user must select one of S[eries] | U[nderlying] | B[in] | *\n");
    }	// switch on Series / Underlying selector

    // close our log file
#ifdef WIN32
    CloseHandle ( orderbook_px );
#else
    fclose ( orderbook_px );
#endif

    printf ("Found %u items in the orderbook\n",
		total_orders_found_u);

    return status_i;
}   // count_gtc

// ---------------------------------------------------------------------------

void clear_quote_table ()
/*
Function:   clear_quote_table
Author:     Neil Cossor
Modified:   9907xx
Description:

    We wish to clear out the components in the (in-memory) quote table.
    This can be because of a QUOTE,CLEAR user command, or
    because we succeeded in doing a QUOTE,SEND
*/
{
    // zot the quote table i.e. the quote message
    memset ( &quote_msg_x, 0, sizeof ( quote_msg_x ));
    // note that we have no quote components now
    quote_components_u = 0;
}   // clear_quote_table

// ---------------------------------------------------------------------------

void update_quoteIDs_sent ( click_details_xt *click_px,
			    quad_word quoteID_u,
			    quote_trans_mo106_item_t *quote_msg_px )
/*
Function:   update_quoteIDs_sent
Author:	    Neil Cossor
Modified:   200012xx
Description:

    Go update the quote ID, quantities for all bid/asks for all series
    which we just quoted in.

    This could be because of a manual quote, or an autoquote.

Input params:

    pointer to everything we know about OM/CLICK
    (local-endian) quote (order) ID
    pointer to one item in a block quote (that was accepted)
*/
{
    // the series for a quote component
    ise_series_xt	ise_series_x;
    // pointer to the full info for a series for a quote component
    ise_series_info_xt	*series_data_px;

    // grab the series info
    get_click_series ( &ise_series_x,
			&quote_msg_px->series
			);
    series_data_px = find_series_by_ise_series ( click_px,
						    &ise_series_x );
    // ignore the 'unknown series' issue.. in theory (handling BI202/4/10) this can't happen
    // is this a bid or ask ?
    if ( quote_msg_px->bid_or_ask_c == BID )
    {
	// updating BID stuff

	// quoteID.. will this work for SUN/G++, where a quadword / uint is a struct ?
	series_data_px->bid_quoteID_u = quoteID_u;
	// premium.. NB byte order is already in central site byte order
	PUTLONG ( series_data_px->quote_bid_premium_i,
		    quote_msg_px->premium_i );
	// quantity
	PUTLONG ( series_data_px->quote_bid_qty_u,
		    quote_msg_px->quantity_u );
    }
    else
    {
	// updating ASK stuff

	// quoteID.. will this work for SUN/G++, where a quadword / uint is a struct ?
	series_data_px->ask_quoteID_u = quoteID_u;
	// premium.. NB byte order is already in central site byte order
	PUTLONG ( series_data_px->quote_ask_premium_i,
		    quote_msg_px->premium_i );
	// quantity
	PUTLONG ( series_data_px->quote_ask_qty_u,
		    quote_msg_px->quantity_u );
    }	// test if bid / ask
}   // update_quoteIDs_sent

// ---------------------------------------------------------------------------

int32 send_block_quote ( click_details_xt *click_px,
			    generic_txn_xt *block_quote_px )
/*
Function:   send_block_quote
Author:	    Neil Cossor
Modified:   20000328
Description:

    We have a block quote to send (from manual or autoquote).
    Send it. If all is OK, then update the internals with
    the quoteIDs, quantities and premiums (for later quote
    deletions etc).
*/
{
    int     status_i = SUCCESSFUL;
    // actual (local-endian) quote (order) ID for the accepted block quote
    quad_word		quoteID_u;
    // size of our actual CLICK command.
    uint32		tx_length_u;
    // how many actual components in block quote
    uint16		quote_items_u;


    // how many actual items are there in the block quote
    PUTSHORT ( quote_items_u,
		block_quote_px->tx_x.quote_trans_x.items_n );
    // set the size from that.. max size, adjusted back to how many actually present
    tx_length_u = sizeof ( block_quote_px->tx_x.quote_trans_x )
			- sizeof ( quote_trans_mo106_item_t ) * ( 192 - quote_items_u );

    pack_OM_header ( block_quote_px, tx_length_u, 'M', 'O', 106);
    // save the facility code and fn code
    block_quote_px->fn_code = LOG_K_C_QUOTE_TRANS;
    block_quote_px->facility_u = click_px->facility_no_u + 7;   // EP7/ET7

    // try to send it
    status_i = do_click_tx_rx ( click_px,
				block_quote_px,
				tx_length_u );

    if (status_i >= SUCCESSFUL)
    {
	// first note order (quote)ID in correct byte order
	memcpy( &quoteID_u,
		    &(click_px->log_reply_x.OrderIdent_u), sizeof(quoteID_u) );
	for ( quote_items_u--;	// start with index of last item
		quote_items_u > 0;
		quote_items_u-- )
	{
	    // update quoteID, price and qty for each bid/ask side of series involved
	    update_quoteIDs_sent ( click_px,
				    quoteID_u,
				    &block_quote_px->tx_x.quote_trans_x.item [ quote_items_u ] );
	}   // loop thru, updating our saved qty, price and quote ID
    }	// test if success.. if failure, already reported in omniapi_tx

    return status_i;
}   // send_block_quote

// ---------------------------------------------------------------------------

int32 send_quote ( click_details_xt * click_px )
/*
Function:   send_quote
Author:     Neil Cossor
Modified:   20000328
Description:

    MM wishes to update their quotes.
    Uses mo106.

    This wraps up the quote table components (previously set by
    QUOTE,ASK or QUOTE,BID commands), and sends them as a block
    to the central system.

    If successful, then clears out the quote table.

    It may be noted that ALL components in one block quote MUST be
    accepted by the central system for a success status... it is an
    all or none transaction.
*/
{
    int     status_i = SUCCESSFUL;


    if ( !click_px->logged_on_i )
    {
	prompt_logged_on ( "Quote,Send" );
	return NOT_LOGGED_ON;
    }

    status_i = send_block_quote ( click_px,
				    &quote_msg_x );
    if ( status_i < SUCCESSFUL)
    {
	// do nothing, error already reported by do_click_tx_rx
    }
    else
    {
	// quote sent OK
	clear_quote_table ();
	printf ( "Quote(s) accepted by central system\n" );
    }

    // wrap up the data to send
    return status_i;
}   // send_quote

// ---------------------------------------------------------------------------

int32 add_quote_component ( click_details_xt * click_px,
			    int argc,
			    char *argv[] )
/*
Function:   add_quote_component
Author:     Neil Cossor
Modified:   200012xx
Description:

    MM wishes to add one component to our table, preparatory to
    sending the quote

Input Params:

    normal argc,argv[] params from command line

    (mandatory) arguments are
    argv[0] QUOTE
    argv[1] BID or ASK
    argv[2] <series name>
    argv[3] quantity
    argv[4] premium
    argv[5] open quantity
    argv[6] step up buffer
*/
{
    int 		status_i = SUCCESSFUL;
    // pointer to selected series
    ise_series_info_pt	series_info_px = NULL;
    /* temp, long number.. when reading a number, C only provides functions
	which put the output in a longword.. we need a word.
	Also used for handling premium, where we need to juggle the number,
	and then do an endian push
    */
    int 		temp_i;


    /* if there are currently no quote components, then it would be a good
	time to zot all of the table.
    */

    // sufficient arguments ?
    if ( argc < 7 )
    {
	// no - report error, and return
	printf ( "Quote components require QUOTE,BID|ASK,<seriesname>,<qty>,<premium>,<open qty>,<step up>\n");
	return SUCCESSFUL;
    }

    // which series
    series_info_px = find_series_by_name_s ( click_px,
						argv [2] );
    if (!series_info_px)
    {
	// oops - did NOT know about that series
	printf ("\tERROR - Unknown Series [%s] in selection for adding quote component.\n",
		    argv[2] );
	return SUCCESSFUL;
    }

    // should we initialise the table, and have we room for another component
    if (quote_components_u == 0)
    {
	// first quote component into the block quote

	/* need to put in an orderbook...all quote components in the block are
	    supposed to be in the same orderbook.. so if we pick the first one,
	    we should be fine. Could save the orderbook from when we added the first
	    quote component, but more straightforward (if less efficient),
	    to get it when we are about to send it
	*/
	quote_msg_x.tx_x.quote_trans_x.orderbook_c =
	    series_info_px->parent_under_px->orderbook_u;

	// zot the quote table i.e. the quote message
	clear_quote_table ();
    }
    else
    {
	// we are adding another component, and it is NOT the first

	// will the addition of this quote component blow the number we can handle
	if ( quote_components_u >= MAX_QUOTE_COMPONENTS )
	{
	    // error - too many script components, bail out of this routine
	    return TOO_MANY_QUOTE_COMPONENTS;
	}   // test if the msg is already full

	// does the orderbook of this component match that of the first
	if ( quote_msg_x.tx_x.quote_trans_x.orderbook_c !=
	     series_info_px->parent_under_px->orderbook_u )
	{
	    // oops - different orderbook - warn the operator
	    printf ("Warning, orderbook (%hu) for quote component (%s) is not same as first (%hu)\n",
			series_info_px->parent_under_px->orderbook_u,
			&series_info_px->ise_name_s,
			quote_msg_x.tx_x.quote_trans_x.orderbook_c );
	}
    }	// test if we are putting in the first entry into the table (msg)


    // convert the input arguments into usable form - if, of course, they are valid

    // bid or ask
    quote_msg_x.tx_x.quote_trans_x.item[quote_components_u].bid_or_ask_c =
	    cmd_bid_ask ( click_px, argv[1] );
    // legal selection ?
    if ( quote_msg_x.tx_x.quote_trans_x.item[quote_components_u].bid_or_ask_c == 0 )
    {
	printf ("Illegal quote component qualifier - BID | ASK\n");
	return SUCCESSFUL;
    }

    // save the series info
    put_click_series ( &quote_msg_x.tx_x.quote_trans_x
				.item[quote_components_u].series,
			&series_info_px->ise_series_x );

    // warn the user if the orderbook for this latest quote component is NOT the same
    // as the first series in the block quote
    quote_msg_x.tx_x.quote_trans_x.orderbook_c =
	series_info_px->parent_under_px->orderbook_u;


    // what quantity
    temp_i = read_unsigned_limited ( argv[3], UNDEFINED_UINT32 );
    PUTLONG ( quote_msg_x.tx_x.quote_trans_x
		.item[quote_components_u].quantity_u,
		temp_i );

    // what premium
    temp_i = read_decimal_string_int ( argv[4],
					series_info_px->instrument_class_px->dec_in_premium_u );
    PUTLONG ( quote_msg_x.tx_x.quote_trans_x
		.item[quote_components_u].premium_i,
		temp_i );

    // what open quantity
    temp_i = read_unsigned_limited ( argv[5], UNDEFINED_UINT32 );
    PUTSHORT ( quote_msg_x.tx_x.quote_trans_x
		.item[quote_components_u].open_quantity_u,
		temp_i );

    // what step up quantity
    temp_i = read_unsigned_limited ( argv[6], UNDEFINED_UINT16 );
    PUTSHORT ( quote_msg_x.tx_x.quote_trans_x
		.item[quote_components_u].step_up_buffer_n,
		temp_i );


    // if we got this far, everything was OK, so up the number of quote components
    quote_components_u++;
    // and update the (endian) value in the msg
    PUTSHORT ( quote_msg_x.tx_x.quote_trans_x.items_n,
		quote_components_u );

    return status_i;
}   // add_quote_component

// ---------------------------------------------------------------------------

void display_quote_table ( click_details_xt * click_px )
/*
Function:   display_quote_table
Author:     Neil Cossor
Modified:   200012xx
Description:

    The user would like to see what is in the (in-memory) quote table..
    display it on stdout
*/
{
    // loop var
    uint16		item_index_u;
    // the series for a quote component
    ise_series_xt	ise_series_x;
    // pointer to the full info for a series for a quote component
    ise_series_info_xt	*series_data_px;
    series_premium_st	premium_s;	    // premium, in string form

    if (quote_components_u==0)
    {
	printf("In-memory quote table is empty\n");
	return;
    }

    for ( item_index_u = 0;
	    item_index_u < quote_components_u;
	    item_index_u++ )
    {
	// dump the info about one quote component
	// grab the series info
	get_click_series ( &ise_series_x,
			    &quote_msg_x.tx_x.quote_trans_x
				    .item[item_index_u].series
			    );
	series_data_px = find_series_by_ise_series ( click_px,
							&ise_series_x );
	// format the premium, with correct implied decimals
	format_whole_decimal_s ( (char *)&premium_s,
				    quote_msg_x.tx_x.quote_trans_x.item[item_index_u].premium_i,
				    series_data_px->instrument_class_px->dec_in_premium_u,
				    MAX_SERIES_PREMIUM );   // width

	// Quote series <name> <bid/ask> <qty> at <premium>, open qty <qty>
	printf ( "Quote series %-s %s qty %hu at %s, open qty %hu\n",
		    series_data_px->ise_name_s,
		    bid_ask_to_string ( quote_msg_x.tx_x.quote_trans_x.item[item_index_u].bid_or_ask_c),
		    quote_msg_x.tx_x.quote_trans_x.item[item_index_u].quantity_u,
		    &premium_s,
		    quote_msg_x.tx_x.quote_trans_x.item[item_index_u].open_quantity_u
		);
    }	// loop, on # of quote components in the table / msg
}   // display_quote_table

// ---------------------------------------------------------------------------

int32 delete_quote ( click_details_xt * click_px,
			int argc,
			char *argv[] )
/*
Function:   delete_quote
Author:     Neil Cossor
Modified:   200012xx
Description:

    MM wishes to delete a quote

Input Params:

    normal argc,argv[] params from command line

    (mandatory) arguments are
    argv[0] QUOTE
    argv[1] DELETE
    argv[2] BID|ASK
    argv[3] <series name>

Implicit params:

    the quoting code (sending, querying and broadcasts) maintain the
    quoteID for each series/bid/ask combo.
*/
{
    // the bid/ask specified
    uint8		bid_ask_u;
    // the message we send to central system
    generic_txn_xt	OMmsg_x;
    // pointer to the full info for a series for a quote component
    ise_series_info_xt	*series_data_px;
    // size of our actual CLICK query/command
    uint32		tx_length_u = sizeof ( delete_trans_mo104_mo105_t );
    // status from call
    int32		status_i;


    // enough arguments ?
    if ( argc < 4)
    {
	printf ("Insufficient args :- QUOTE, DELETE, BID|ASK|BOTH, <seriesname>\n");
	return SUCCESSFUL;
    }

    if ( !click_px->logged_on_i )
    {
	prompt_logged_on ( "Quote,Delete" );
	return NOT_LOGGED_ON;
    }

    // zot the message we are packing - all 0s / NULs
    memset( &OMmsg_x.tx_x.delete_trans_x, 0, tx_length_u );

    // default to using 105 delete, unless we are specifying my user ID
    pack_OM_header ( &OMmsg_x, tx_length_u, 'M', 'O', 105 );

    // pack in the caller supplied fields

    // figure out bid/ask/both
    bid_ask_u = cmd_bid_ask ( click_px, argv[2] );
    // specify the bid/ask type as supplied
    OMmsg_x.tx_x.delete_trans_x.bid_or_ask_c = bid_ask_u;
    // specify it is a quote we are deleting
    PUTSHORT ( OMmsg_x.tx_x.delete_trans_x.order_category_n,
		(uint16)ORDER_CAT_QUOTE );
    // need to pack 'whose'
    // fixed country ID for now, from the info retrieved for this user (at logon)
    tx_click_string ( OMmsg_x.tx_x.delete_trans_x.whose.trading_code.country_id_s,
			&click_px->user_code_x.country_id_s,
			sizeof (OMmsg_x.tx_x.delete_trans_x.whose.trading_code.country_id_s));

    // use whatever customer ID the caller specified
    tx_click_string ( OMmsg_x.tx_x.delete_trans_x.whose.trading_code.ex_customer_s,
			&click_px->user_code_x.customer_id_s,
			sizeof (OMmsg_x.tx_x.delete_trans_x.whose.trading_code.ex_customer_s));

    // find the series
    // series name -> a series (can be %S, %O)
    series_data_px = user_selected_series ( click_px, argv[3] );
    if (series_data_px == NULL)
    {
	// couldn't find that series - error already reported - bail out
	return SUCCESSFUL;
    }

    // specify the series as selected
    put_click_series ( &OMmsg_x.tx_x.delete_trans_x.series,
			&series_data_px->ise_series_x );
    // specify the orderbook, to match the series
    OMmsg_x.tx_x.delete_trans_x.orderbook_c =
		    series_data_px->parent_under_px->orderbook_u;


    // put in the quoteID.. this must be non-zero to have a chance
    if ( bid_ask_u == BID )
    {
//	PUTQUAD ( OMmsg_x.tx_x.delete_trans_x.order_number_u,
//		    series_data_px->bid_quoteID_u );
    }
    else if ( bid_ask_u == ASK )
    {
//	PUTQUAD ( OMmsg_x.tx_x.delete_trans_x.order_number_u,
//		    series_data_px->ask_quoteID_u );
    }	// BOTH uses 0 for orderID

    // save the facility code and fn code for logging
    OMmsg_x.fn_code = LOG_K_C_DELETE_TRANS;
    OMmsg_x.facility_u = click_px->facility_no_u + 7;	// EP7/ET7

    status_i = do_click_tx_rx ( click_px,
				    &OMmsg_x,
				    tx_length_u );

    if ( status_i < SUCCESSFUL )
    {
	// do nothing, error already reported by do_click_tx_rx
    }
    else
    {
	// got back a 0+ value.. did it report the number of contracts deleted
	printf ("Deleted %u %s contracts in series %s\n",
		    status_i,
		    bid_ask_to_string (bid_ask_u),
		    series_data_px->ise_name_s );

	// zot the saved quote info.. the quote doesn't exist anymore
	if ( bid_ask_u == BID )
	{
	    memcpy(&series_data_px->bid_quoteID_u, &blank, sizeof(blank));
	    series_data_px->quote_bid_premium_i = 0;
	    series_data_px->quote_bid_qty_u = 0;
	}
	else
	{
	    memcpy(&(series_data_px->ask_quoteID_u), &blank, sizeof(blank));
	    series_data_px->quote_ask_premium_i = 0;
	    series_data_px->quote_ask_qty_u = 0;
	}
    }

    return status_i;
}   // delete_quote

// ---------------------------------------------------------------------------

int32 panic_delete_quote ( click_details_xt * click_px,
			int argc,
			char *argv[] )
/*
Function:   panic_delete_quote
Author:     Neil Cossor
Modified:   20010524
Description:

    MM wishes to delete all quotes for the member in the bin, both sides

Input Params:

    normal argc,argv[] params from command line

    (mandatory) arguments are
    argv[0] QUOTE
    argv[1] PANIC

Implicit params:

Possible enhancements:

    could have a USER|MEMBER argumemt, which determines whether to pull for
    this user, or for all users for the member. The immediate need is for all
    users, just a matter of whether all bins (multiple members) or one bin.
*/

{
    // the message we send to central system
    generic_txn_xt	OMmsg_x;
    // size of our actual CLICK query/command
    uint32		tx_length_u = sizeof ( delete_trans_mo104_mo105_t );
    // status from call
    int32		status_i;
    // pointer to one (chained) node, in the known_bins_q
    gen_buf_node_xt	    *bin_node_px = NULL;
    // pointer to the data component of the node in the known_bins_q
    ise_bin_xt		    *bin_data_px = NULL;
    // index into orderbook data for the bin
    unsigned		ob_index_u;


    // no optional arguments at this point...
    if ( !click_px->logged_on_i )
    {
	prompt_logged_on ( "Quote,Panic" );
	return NOT_LOGGED_ON;
    }

    // if we are a MM, we have a default bin
    if (click_px->our_market_x.default_bin_u == 0)
    {
	// no default bin
	printf ("No default bin - we are not a MM - no deletions done\n");
	return INVALID_ARGUMENTS;
    }

    // point to the (chained) node which contains pointer to data about our bin
    bin_node_px = find_bin ( &click_px->our_market_x,
				click_px->our_market_x.default_bin_u );

    // point to the data about our bin
    bin_data_px = (ise_bin_xt *)bin_node_px->msg_p;

    
    // zot the message we are packing - all 0s / NULs
    memset( &OMmsg_x.tx_x.delete_trans_x, 0, tx_length_u );

    // we are only allowed to use 105 delete
    pack_OM_header ( &OMmsg_x, tx_length_u, 'M', 'O', 105 );

    // pack everything - NB message contains all zeroes, so already default to all stocks, bid&ask

    // specify it is a quotes only we are deleting - leave orders etc
    PUTSHORT ( OMmsg_x.tx_x.delete_trans_x.order_category_n,
		ORDER_CAT_QUOTE );

    // need to pack 'whose'
    // fixed country ID for now, from the info retrieved for this user (at logon)
    tx_click_string ( OMmsg_x.tx_x.delete_trans_x.whose.trading_code.country_id_s,
			&click_px->user_code_x.country_id_s,
			sizeof (OMmsg_x.tx_x.delete_trans_x.whose.trading_code.country_id_s));

    // use whatever customer ID the caller specified
    tx_click_string ( OMmsg_x.tx_x.delete_trans_x.whose.trading_code.ex_customer_s,
			&click_px->user_code_x.customer_id_s,
			sizeof (OMmsg_x.tx_x.delete_trans_x.whose.trading_code.ex_customer_s));
    // leave the user as NULs, so all users in the member

    // save the facility code and fn code for logging
    OMmsg_x.fn_code = LOG_K_C_DELETE_TRANS;
    OMmsg_x.facility_u = click_px->facility_no_u + 7;	// EP7/ET7

    // all underlying, we need to do all orderbooks for the bin
    for ( ob_index_u = 0;
	    ob_index_u < bin_data_px->orderbooks_u;
	    ob_index_u++ )
    {
	OMmsg_x.tx_x.delete_trans_x.orderbook_c =
		    bin_data_px->orderbook_u [ ob_index_u ];

	status_i = do_click_tx_rx ( click_px,
					&OMmsg_x,
					tx_length_u );

	if ( status_i < SUCCESSFUL )
	{
	    // do nothing, error already reported by do_click_tx_rx
	}
	else
	{
	    // got back a 0+ value.. we SHOULD have the number of series in
	    // which contracts were deleted, but this seems to be unreliable.
	    printf ("Deleted contracts in %u series in orderbook %u\n",
			status_i,
			bin_data_px->orderbook_u [ob_index_u] );

	    /* we should update our internal knowledge of our quotes, however
		this command is being written to bailout, on behalf of other
		users, and we probably didn't know anything about those quotes
		in the first place */
	}
    }	// for loop, whizzing around the orderbooks for the bin

    return status_i;
}   // panic_delete_quote

// ---------------------------------------------------------------------------

int32 set_speedbump_parameters ( click_details_xt * click_px,
				int argc,
				char *argv[] )
/*
Function:   set_speedbump_parameters
Author:     Neil Cossor
Modified:   20010709
Description:

    CMM/PMM wishes to set their mm exposure limits (speedbump)
    Uses ui103.

    This normally sets the parameters for exactly one underlying, or whole bin

Input Params:

    standard argc/argv from the command line.. params are
    argv[0]	    SPEED
    argv[1]	    SET
    argv[2]	    <underlying name>, or "%BIN"
    argv[3]	    <number of contracts>   must be >0
    argv[4]	    <widen percentage>	    must be >100
*/
{
    int 		status_i = SUCCESSFUL;
    // pointer to the underlying we are doing this for
    ise_underlying_xt	*under_data_px = NULL;
    // value for # of contracts
    uint32		trade_limit_u;
    // value for widen %
    uint16		spread_widen_percentage_u;
    // # of underlying we set for
    uint16		item_count_u = 0;
    // if %bin is specified, we need somewhere to translate that into
    ise_underlying_list_xt	bin_under_list_x;
    // which 'case' did the caller specify for each entry in the argv list.. *, %bin, <underlying>
    int			under_selected_i;


    if ( argc < 5 )
    {
	printf ("SPEED,SET,<underlying name>|%%BIN,<# contracts>,<widen %%>\n");
	return status_i;
    }

    // try to translate the params

    // start out with an 'empty' list of specific underlying to subscribe to :- 0 count, all entries 0
    memset ( &bin_under_list_x, 0, sizeof (bin_under_list_x));

    // first, try to figure out which underlying we are setting params for
    under_selected_i = select_string_option ( 2, set_under_selectors_s, argv[2] );
    switch ( under_selected_i )
    {
    case set_under_bin_e:
	/* Must substitute in all of the underlying for this bin
	    Of course, we must be a CMM / PMM for this to make sense.

	    If we are NOT an MM, then we are NOT allowed to autoquote anyway..
	*/

	status_i = subscr_list_bin ( click_px,
					&bin_under_list_x,
					click_px->our_market_x.default_bin_u );
	// did that work ?
	if (status_i != SUCCESSFUL)
	{
	    printf ("Error doing SPEED,SET cmd using bin %u\n",
			click_px->our_market_x.default_bin_u );
	    return status_i;
	}

	break;

    case set_under_folio_e:
	// %folio
	not_yet_implemented ("folio selectors in SPEED,SET command");
	return INVALID_ARGUMENTS;

    default:
	// must be a real underlying (or %u)
	// search for the underlying.. if unknown, then skip it from list with a warning to user
	under_data_px = find_under_by_name_s ( click_px,
						    argv [2 ] );
	// did we find a match
	if ( under_data_px )
	{
	    // known underlying.. add the commodity code to the list
	    bin_under_list_x.commodity_code_u [ bin_under_list_x.underlying_count_u ]
		= under_data_px->commodity_code_u;
	    bin_under_list_x.underlying_count_u = 1;
	}
	else
	{
	    // unknown underlying - report it.. and keep going, skipping this underlying
	    printf ("SET for unknown underlying %s\n",
			argv [ 2 ] );
	    return INVALID_ARGUMENTS;
	}   // test if we found specific underlying (by name)
    }	// switch, on the type of a particular argv[]


    trade_limit_u = read_unsigned_limited ( argv[3], UNDEFINED_UINT32 );
    spread_widen_percentage_u = read_unsigned_limited ( argv[4], UNDEFINED_UINT16 );

    for ( item_count_u = 0;
	  item_count_u < bin_under_list_x.underlying_count_u;
	  item_count_u++ )
    {
	// call the routine to set the params for one underlying
	under_data_px = find_under_data_by_commodity ( click_px,
							bin_under_list_x.commodity_code_u [item_count_u] );
	status_i = set_speedbump_parameters_under ( click_px,
						    under_data_px,
						    trade_limit_u,
						    spread_widen_percentage_u
						    );
    }	// for loop, setting params for our selected underlying

    return status_i;
}   // set_speedbump_parameters

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


Title : autoqute.c
Author: Scott Collins
Function :

    This file provides logic to produce auto quoting based on the Black & Scholes
    pricing model.
    

Revision history :-

yymmdd	vers'n	author		notes
------	------	------		-----
000110	v0.00	Scott Collins	initial version
000120	v0.01	Scott Collins	add smarts to strip out multiple underlying updates for same underlying
		Neil Cossor
000122		Neil Cossor	moved double_from_whole_decimal, uint_from_double_with_round
				    and double_days_from_now_to_date_xt into utility.c/.h
				calc several things (like strike price) into doubles at series creation
000123				remove disable_autoquote from aq_calc_underlying, if error sending quote
				fix chain-walking bug in get_latest_quote_from_queue
000204	v0.02			move include of process.h into stdincl, only if WinNT
				mod init_autoquote_io_thread to call ISE_THREAD_CREATE
000328	v0.03			mod aq_calc_underlying to call send_block_quote, which as well
				    as sending, updates in-memory quote info if successful
000422	v1.00			update to match OM release 15.7-3 (move of some fields from da203 to da210).
				    update aq_calc_underlying
000505	v1.01			fix disable_autoquote.. set wrong value, which left autoquoting on
				fix get_latest_quote_from_queue.. did not lock Q before chain walk
000620	v1.02			fix autoquote_change and autoquote_show, to reflect change of how we store
				the vol and int % ( 10% now stored as 0.1, so is in format useable for quote calc)
0012xx	v2.00			updates for OM release 16.3 (from 15.7-3)
010319				replace ref to uint_from_double_with_round with fract_from_double_with_round,
				add split to call new decimal_from_double_with_round.

To do:
------

- we could just use the price in the actual underlying record, rather than the price in the update msg.
    If we are stripping out any later updates for the same underlying, then the price should be the same anyway.
- we could attempt to pack quotes for multiple underlying into one MO106. This does, of course, require
    that the underlying are handled by the same orderbook server.. (we know that they are in the same bin,
    because we can't quote for anything which is outside our bin anyway).
*/
#include "stdincl.h"
#include "utility.h"
#include "autoqute.h"
#include "mm.h"
#include "series.h"		// needed for put_click_series
#include "underlying.h"
#include <math.h>		// mathematical library




static unsigned 	KeepAutoquoting = FALSE;
// the instrument group number for American Calls.. NB this CAN change within the CDB
// but is not likely to, so #define is an almost acceptable cheat
#define INSTR_GROUP_CALL    6

// ---------------------------------------------------------------------------

double N(double z)
/*
Function:   N
Author:     Scott Collins, mods Neil Cossor
Modified:   000119
Description:

    Supporting function for the computation of Black & Scholes prices

*/
{
    // Neil Cossor - make these static
    static double b1 =	0.31938153;
    static double b2 = -0.356563782;
    static double b3 =	1.781477937;
    static double b4 = -1.821255978;
    static double b5 =	1.330274429;
    static double p  =	0.2316419;
    static double c2 =	0.3989423;

    double a = fabs(z); 

    double t; 
    double b; 
    double n; 


    if (a > 6.0)
    {
	return 1.0;
    } // this guards against overflow 
    t = 1.0 / (1.0 + a * p); 
    b = c2 * exp((-z) * (z / 2.0)); 
    n = ((((b5 * t + b4) * t + b3) * t + b2) * t + b1) * t; 
    n = 1.0 - b * n; 
    if ( z < 0.0 ) n = 1.0 - n; 
	return n; 
} // N

// ---------------------------------------------------------------------------

double option_price_call_black_scholes( double S,   // spot price
					double X,   // Strike (exercise) price
					double r,   // interest rate
					double sigma,
					double time) 
/*
Function:   option_price_call_black_scholes
Author:     Scott Collins
Modified:   000110
Description:

    Calculate the call for a series using the Black & Scholes pricing model

Input Params:

    Spot Price
    Strike Price
    Interest Rate
    Sigma (volatility)
    Days to expiration

Return Value:

    Call price
    
*/
{  
    double time_sqrt = sqrt(time);
    double d1 = (log(S / X) + r * time) / (sigma * time_sqrt) + 0.5 * sigma * time_sqrt; 
    double d2 = d1 - (sigma * time_sqrt);
    double c = S * N(d1) - X * exp(-r * time) * N(d2);
    if (c < 0.0 )
	return 0.0;
    else
	return c;
} // option_price_call_black_scholes

// ---------------------------------------------------------------------------

double option_price_put_black_scholes(	double S,   // spot price
					double X,   // Strike (exercise) price
					double r,   // interest rate
					double sigma,
					double time) 
/*
Function:   option_price_put_black_scholes
Author:     Scott Collins
Modified:   000110
Description:

    Calculate the put for a series using the Black & Scholes pricing model

Input Params:

    Spot Price
    Strike Price
    Interest Rate
    Sigma (volatility)
    Days to expiration

Return Value:
    Put price
    
*/
{  
    double time_sqrt = sqrt(time);
    double d1 = (log(S / X) + r * time) / (sigma * time_sqrt) + 0.5 * sigma * time_sqrt;
    double d2 = d1 - (sigma * time_sqrt);
    double p =	X * exp(-r * time) * N(-d2) - S * N(-d1); 
    if ( p < 0.0 )
	return 0.0;
    else
	return p;
} // option_price_put_black_scholes

// ---------------------------------------------------------------------------

void fill_mo106_item (	quote_trans_mo106_item_t    *mo106item_px,
			click_details_xt	    *click_px,
			ise_underlying_xt	    *under_data_px,
			ise_series_xt		    *series_data_px,
			uint32			    new_premium_i,
			uint8			    bid_or_ask_c)
/*
Function:   fill_mo106_item
Author:     Scott Collins
Modified:   001215
Description:

    Fill a mo106 item

Input Params:

    pointer to click details
    pointer to series data
    premium
    bid or ask

Output Params:

    mo106 data item
*/
{
    put_click_series (	&mo106item_px->series, series_data_px);

    PUTLONG  ( mo106item_px->premium_i, 	new_premium_i);
    mo106item_px->bid_or_ask_c =		bid_or_ask_c;
    // for the quantities, if there is a value set for the underlying, use that
    // otherwise use the values in click_px
    if ( under_data_px->autoquote_quantity_u )
	PUTLONG ( mo106item_px->quantity_u,	    under_data_px->autoquote_quantity_u);
    else
	PUTLONG ( mo106item_px->quantity_u,	    click_px->autoquote_quantity_u);

    if ( under_data_px->autoquote_open_quantity_u )
	PUTLONG ( mo106item_px->open_quantity_u,   under_data_px->autoquote_open_quantity_u);
    else
	PUTLONG ( mo106item_px->open_quantity_u,   click_px->autoquote_open_quantity_u);

    if ( under_data_px->autoquote_step_up_buffer_n )
	PUTSHORT ( mo106item_px->step_up_buffer_n,  under_data_px->autoquote_step_up_buffer_n);
    else
	PUTSHORT ( mo106item_px->step_up_buffer_n,  click_px->autoquote_step_up_buffer_n);
/*
    printf ("%s %d\n",
	    bid_ask_to_string ( bid_or_ask_c ),
	    new_premium_i
	    );
*/
} // fill_mo106_item

// ---------------------------------------------------------------------------
 
void update_from_our_mo106 ( click_details_xt *click_px,
				quote_trans_mo106_t *mo106_px )
/*
Function:   update_from_our_mo106
Author:	    Neil Cossor
Modified:   20010620
Description:

    We just sent an MO106, which was accepted.
    Update our in-memory view of what we have quoted at.
    We can only do this after we know the whole quote was accepted.
    We need this info to be up to date, so that a requote will
    correctly know whether we have generated a different price
    from a previous quote (and not resend at the old price)

    We actually do a bit of work unpacking what we just packed,
    but as we can't do the update until after we know the quote was
    accepted, its unpack, of keep a parallel structure as we pack
    the MO106, and update our memory from that. Not nice either way.
*/
{
    uint16  no_of_items_u;
    unsigned item_index_u;
    uint8   bid_ask_u;
    ise_series_xt	    ise_series_x;	// temp, the series in ISE form
    ise_series_info_pt	    series_data_px;	// pointer to info about a series

    PUTSHORT ( no_of_items_u, mo106_px->items_n );

    for ( item_index_u = 0;
	    item_index_u < no_of_items_u;
	    item_index_u++ )
    {
	// get the series, in "normal" form
	get_click_series ( &ise_series_x,
			    &mo106_px->item[item_index_u].series );
	// find the record with our data about the series
	series_data_px = find_series_by_ise_series ( click_px,
							&ise_series_x );
	bid_ask_u = mo106_px->item[item_index_u].bid_or_ask_c;
	if ( bid_ask_u == BID )
	{
	    // bid updated
	    series_data_px->quote_bid_premium_i = mo106_px->item[item_index_u].premium_i;
	}
	else
	{
	    // ask updated
	    series_data_px->quote_ask_premium_i = mo106_px->item[item_index_u].premium_i;;
	}
    }
}   // update_from_our_mo106

// ---------------------------------------------------------------------------
 
void aq_calc_underlying (click_details_xt	    *click_px,	
			 autoquote_price_change_xt  *autoquote_price_change_px)
/*
Function:   aq_calc_underlying
Author:     Scott Collins
Modified:   20000422
Description:

    Evaluate and recalculate the series premiums for an underlying at a new price
    Create and send MO106 for premium changes

Input Params:

    Underlying and the new bid and/or ask 

Output Params:

    N/A
*/
{
    ise_series_info_xt	*series_data_px = NULL;
    gen_buf_node_xt	*series_node_px = NULL;
    ise_underlying_xt	*under_data_px = NULL;

    double  new_under_bid_d,
	    new_under_ask_d,
	    new_bid_premium_d,
	    new_ask_premium_d,
	    new_average_premium_d;

    int32   new_bid_premium_i,
	    new_ask_premium_i;

    int     status_i,
	    quote_components_u;
    unsigned	quotes_sent_u;

    quote_trans_mo106_t *mo106_px;
    generic_txn_xt	msg_tx;
    
    mo106_px = &msg_tx.tx_x.quote_trans_x;

    // initialize message with all NULLs
    memset (&msg_tx, (char)0, sizeof (generic_txn_xt));

    // we have generated no quote components
    quotes_sent_u = 0;


    // initialize the mo106
    quote_components_u =	0;
    mo106_px->orderbook_c =	autoquote_price_change_px->ise_underlying_px->orderbook_u;

    // save pointer into underlying data
    under_data_px = autoquote_price_change_px->ise_underlying_px;

    // convert appropriate underlying values to doubles (black & scholes input types)

    // Read new underlying bid/ask prices into doubles
    new_under_bid_d = double_from_whole_decimal (
				autoquote_price_change_px->new_under_bid_premium_i,
				under_data_px->dec_in_price_u);

    new_under_ask_d = double_from_whole_decimal (
				autoquote_price_change_px->new_under_ask_premium_i,
				under_data_px->dec_in_price_u);


    // for each series...
    series_node_px = (gen_buf_node_xt *)under_data_px->under_series_hdr_x.chain.next;
    while ( series_node_px )
    {

	// point to the actual info about the series
	series_data_px = (ise_series_info_xt *)series_node_px->msg_p;


	new_bid_premium_i = 0;
	new_ask_premium_i = 0;

	/* Compute new series bid & ask, regardless of underlying price change
	    NOTE in Calls, the premium bid is calculated from the underlying bid
	    and the ask premium from the underlying ask..
	    but in the Puts, this is reversed
	*/
	if ( series_data_px->ise_series_x.instrument_group_u == INSTR_GROUP_CALL )
	{
	    new_bid_premium_d = option_price_call_black_scholes (   new_under_bid_d,
								    series_data_px->strike_f,
								    under_data_px->interest_f, 
								    under_data_px->volatility_f,
								    series_data_px->years_to_expiry_f
								    );
	    new_ask_premium_d = option_price_call_black_scholes (   new_under_ask_d,
								    series_data_px->strike_f,
								    under_data_px->interest_f, 
								    under_data_px->volatility_f,
								    series_data_px->years_to_expiry_f
								    );
	}
	else
	{
	    new_bid_premium_d = option_price_put_black_scholes (    new_under_ask_d,
								    series_data_px->strike_f,
								    under_data_px->interest_f, 
								    under_data_px->volatility_f,
								    series_data_px->years_to_expiry_f
								    );
	    new_ask_premium_d = option_price_put_black_scholes (    new_under_bid_d,
								    series_data_px->strike_f,
								    under_data_px->interest_f, 
								    under_data_px->volatility_f,
								    series_data_px->years_to_expiry_f
								    );
	}

	if ( series_data_px ->instrument_class_px->trades_in_decimals_u )
	{
	    // decimal rounding

	    // convert and round the bid
	    new_bid_premium_i = decimal_from_double_with_round ( new_bid_premium_d,
								series_data_px->instrument_class_px,
								0); // round down

	    
	    // convert and round the ask
	    new_ask_premium_i = decimal_from_double_with_round ( new_ask_premium_d,
								series_data_px->instrument_class_px,
								1); // round up

	}
	else
	{
	    // fractional rounding

	    // convert and round the bid
	    new_bid_premium_i = fract_from_double_with_round ( new_bid_premium_d,
								series_data_px->instrument_class_px->dec_in_premium_u,
								0); // round down

	    
	    // convert and round the ask
	    new_ask_premium_i = fract_from_double_with_round ( new_ask_premium_d,
								series_data_px->instrument_class_px->dec_in_premium_u,
								1); // round up

	    /* we have now calculated a new bid and ask quote.. are they sensible, or have
		the approximations in the Black-Scholes model resulted in a bid/ask quote
		that is too close together, or indeed a cross of the bid/ask prices
	    */
	    if (( new_bid_premium_i >= new_ask_premium_i ))
	    {

		printf ("Warning Will Robinson, bid %.4f > ask %.4f in series %s\n",
			    new_bid_premium_d,
			    new_ask_premium_d,
			    &series_data_px->ise_name_s
			);

		// first pass - take the average, and bump out $0.125 each way, so spread of $0.25
		new_average_premium_d = ( new_bid_premium_d + new_ask_premium_d ) / 2.0;

		// convert and round the bid
		new_bid_premium_i = fract_from_double_with_round ( (new_average_premium_d - 0.125),
								    series_data_px->instrument_class_px->dec_in_premium_u,
								    0); // round down
		// convert and round the ask
		new_ask_premium_i = fract_from_double_with_round ( (new_average_premium_d + 0.125),
								    series_data_px->instrument_class_px->dec_in_premium_u,
								    1); // round up
	    }
	}   // check if decimal or fractional rounding



	// after all that, did we end up with a different quote price than our previous quote ?
	// newer bid ?
	if ((new_bid_premium_i != series_data_px->quote_bid_premium_i)
	    && ( new_bid_premium_i > 0 ))
	{
	    // bid has changed...
	    //	add the change to mo106
	    fill_mo106_item (	&mo106_px->item[quote_components_u],
				click_px,
				under_data_px,
				&series_data_px->ise_series_x,
				new_bid_premium_i,
				BID);
	    quote_components_u++;
	} // series bid premium has changed

	// newer ask ?
	if ((new_ask_premium_i != series_data_px->quote_ask_premium_i)
	    && ( new_ask_premium_i > 0 ))
	{
	    // ask has changed...
	    //	add the change to mo106
	    fill_mo106_item (	&mo106_px->item[quote_components_u],
				click_px,
				under_data_px,
				&series_data_px->ise_series_x,
				new_ask_premium_i,
				ASK);
	    quote_components_u++;
	} // series ask premium has changed

	// NB shortcut is use 190, not 192, which ensures a paired bid/ask can fit in one block
	if (quote_components_u > 190)
	{
	    // the mo106 is full... send it

	    // note how many we generated
	    quotes_sent_u += quote_components_u;

	    PUTSHORT ( mo106_px->items_n, quote_components_u);

	    // send the block quote (and update in-memory info, if it succeeds)
	    status_i = send_block_quote ( click_px,
					    &msg_tx );
	    if ( status_i < SUCCESSFUL)
	    {
		// do nothing.. most likely reason for failure is that we are offline,
		// in which case we won't receive any more underlying changes until we logon again
		return;
	    }
	    else
	    {
		update_from_our_mo106 ( click_px, mo106_px );
	    }

	    // reset the mo106's count
	    quote_components_u = 0;

	} // mo106 is full

	// try for the next series
	series_node_px = (gen_buf_node_xt *)series_node_px->chain.next;
    } // while series

    // any components in the current quote block ?
    if (quote_components_u)
    {
	// note how many we generated
	quotes_sent_u += quote_components_u;

	// send the mo106
	PUTSHORT ( mo106_px->items_n, quote_components_u);

	// send the block quote (and update in-memory info, if it succeeds)
	status_i = send_block_quote ( click_px,
					&msg_tx );
	if ( status_i < SUCCESSFUL)
	{
	    // do nothing.. most likely reason for failure is that we are offline,
	    // in which case we won't receive any more underlying changes until we logon again
	}
	else
	{
	    update_from_our_mo106 ( click_px, mo106_px );
	}

    } // send the mo106

    printf ("Generated %u quote components for underlying %s, curr Q %u, max %u\n",
		quotes_sent_u,
		&under_data_px->under_id_s,
		click_px->authorised_autoquote_q.length_i,
		click_px->authorised_autoquote_q.max_length_i
	    );
} // aq_calc_underlying

// ---------------------------------------------------------------------------

static gen_buf_node_xt *get_latest_quote_from_queue ( node_header_xt *node_header_px )
/*
Function:   get_latest_quote_from_queue
Author:     Scott Collins & Neil Cossor
Date:	    20000505
Description:

    Pull an underlying quote change from the Q (if there is one in the Q)

    If we found one at the front of the Q, then we hunt down the Q
    looking for any later ones for that same underlying.
    If we find any, then each time we release the older one, and switch
    to the newer one, which we have removed from the Q (i.e. made into a free node).

    This way, if we are getting multiple updates for the same underlying,
    we are always just generating quotes from the latest underyling BBO.

    If we are getting a Q backlog for differing underlying, there is nothing
    better we can do. It had better be a temporary burst for our autoquote
    portofolio, or we will continue to drift further behind.
*/
{
    gen_buf_node_xt	    *msg_node;
    autoquote_price_change_xt	*autoquote_price_change_px;
    ise_underlying_xt	    *ise_underlying_px;

    gen_buf_node_xt	    *temp_msg_node;
    autoquote_price_change_xt	*temp_autoquote_price_change_px;
    
    // initialize, this is synchronised
    msg_node = (gen_buf_node_xt *)remove_front_from_q ( node_header_px );
    autoquote_price_change_px = NULL;
    ise_underlying_px =     NULL;

    // did we find a block
    if (!msg_node)
	// nothing to do
	return NULL;

    lock_q ( node_header_px );

    // walk the queue to see if a later entry for the same underlying exists
    autoquote_price_change_px = (autoquote_price_change_xt *)msg_node->msg_p;
    ise_underlying_px = autoquote_price_change_px->ise_underlying_px;
    // point to first in Q
    temp_msg_node = (gen_buf_node_xt *)msg_node->chain.next;
    while (temp_msg_node)
    {
	// point to data component of this current node
	temp_autoquote_price_change_px = (autoquote_price_change_xt *)temp_msg_node->msg_p;

	// is this later block (in the q) for the same underlying as the first one we found
	if (temp_autoquote_price_change_px->ise_underlying_px == ise_underlying_px)
	{
	    // a later copy for the same underlying was found in the queue... use it instead

	    // release the older node
	    free_gen_buf_node ( msg_node );

	    // switch to this 'newer' node being the one to use (i.e. return to caller)
	    msg_node = temp_msg_node;

	    // remove the found item from the queue, so it is a 'loose' node
	    // this remove does NOT change the next,prev fields of the block being removed
	    remove_buff_from_q (node_header_px, &temp_msg_node->chain);
	}

	// check the next item
	temp_msg_node = (gen_buf_node_xt *)temp_msg_node->chain.next;
    } // while

    unlock_q ( node_header_px );

    // return the 'latest' block we found, for the caller to go and autoquote on.. may be NULL
    return msg_node;
}   // get_latest_quote_from_queue

// ---------------------------------------------------------------------------

static void autoquote_thrd (click_details_xt *click_px )
/*
Function:   autoquote_thrd
Author:     Scott Collins
Modified:   20000123
Description:

    Thread to read queue of changed underlyings and recalculate and send
    series premiums

Input Params:

    Pointer to the click_details

Output Params:

    N/A
*/
{
    gen_buf_node_xt	*msg_node;

    // spin until the process terminates
    while (TRUE)
    {
	if ( KeepAutoquoting )
	{
	    if ( empty_q ( &click_px->authorised_autoquote_q ) )
	    {
		// and have a little sleep, so we don't hog all of the CPU
		Sleep (100);	// in milliseconds
	    }
	    else
	    {
		autoquote_price_change_xt   *autoquote_price_change_px;
		// Q is not empty, grab (synchronised) the front msg from the Q
		msg_node = get_latest_quote_from_queue ( &click_px->authorised_autoquote_q );

		autoquote_price_change_px = (autoquote_price_change_xt *)msg_node->msg_p;
		aq_calc_underlying ( click_px, autoquote_price_change_px );

		// release the message, and its component parts
		free_gen_buf_node ( msg_node );
	    }	// else case of test if log Q is empty
	}   // while ( keep_autoquoting ) loop
	else
	{
	    // autoquoting is OFF - strip the Q of whatever is in there (releasing child data)
	    flush_gen_buf_node_q ( &click_px->authorised_autoquote_q, TRUE );

	    // and wait a little bit.. maybe someone will turn autoquoting back on
	    Sleep (100);
	}
    }	// while (TRUE)

    // release any buffers that are in the Q, and any resources for the Q header itself
    free_gen_buf_node_q ( &click_px->authorised_autoquote_q , TRUE);

}   // autoquote_thrd

// ---------------------------------------------------------------------------

void enable_autoquote ()
/*
Function:   enable_autoquote
Author:     Scott Collins
Modified:   000110
Description:

    Enable autoquoting 

Input Params:

    N/A

Output Params:

    N/A
*/
{
    KeepAutoquoting = TRUE;
} // enable_autoquote

// ---------------------------------------------------------------------------

void disable_autoquote ()
/*
Function:   disable_autoquote
Author:     Scott Collins
Modified:   000505
Description:

    Disable autoquoting 

Input Params:

    N/A

Output Params:

    N/A
*/
{
    KeepAutoquoting = FALSE;
} // disable_autoquote

// ---------------------------------------------------------------------------

int32 init_autoquote_io_thread (click_details_xt    *click_px)
/*
Function:   init_autoquote_io_thread
Author:     Scott Collins
Modified:   20000203
Description:

    This fn, called once we have logged on, handles quotes and generates 
    autoquotes based on underlying changes found on its queue.


Input Params:

    Pointer to click_details_xt

Output Params:

    pointer the the Queue that is read by the thread


*/
{
    // our return status, with an initial default value
    int     status_i = SUCCESSFUL;

    // initialise the module-private structures
    init_q ( &click_px->authorised_autoquote_q );

    // start up the autoquote thread 
    status_i = ISE_THREAD_CREATE ( autoquote_thrd,     // main fn of created thread
				    click_px );		// 32 bit value passed to thread
							

    // a +ve status means that it worked, and the status is the handle for the thread
    // BUT we want to return the success status.. NOT some arbitrary handle value
    if (status_i > 0)
    {
	status_i = SUCCESSFUL;
	// default to autoquoting ON
	enable_autoquote ();
    }

    return status_i;
}   // init_autoquote_io_thread

// --------------------------------------------------------------------------

void autoquote_show ( click_details_xt *click_px )
/*
Function : autoquote_show
Author	 : Neil Cossor
Modified : 20001229
Description:

    We have wish to display the autoquote settings

    Whiz thru the underlying, showing those underlying which have autoquote
    values (turned on or non-zero interest or volatility)

    No point looking at any underlying which are NOT our bin,
    as we will not be able to quote them anyway.
*/
{
    // pointer to one (chained) node, in the known_bins_q
    gen_buf_node_xt	    *bin_node_px = NULL;
    // pointer to the data component of the node in the known_bins_q
    ise_bin_xt		    *bin_data_px = NULL;
    // Q'able node, which contains a msg_p pointer to an underlying,
    gen_buf_node_xt	    *bin_under_node_px = NULL;
    // and pointer to actual underlying data
    ise_underlying_xt	    *under_data_px;



    if ( click_px->our_market_x.default_bin_u == 0 )
    {
	printf ("Warning, no default bin.. autoquoting not possible\n");
	return;
    }

    // point to the data we know about for that bin
    bin_node_px = find_bin ( &click_px->our_market_x,
				click_px->our_market_x.default_bin_u );

    // we found the bin - dump out the interesting stuff
    bin_data_px = (ise_bin_xt *)bin_node_px->msg_p;

    printf ("Autoquote settings for Bin %d, autoquoting (overall) is %s\n",
		click_px->our_market_x.default_bin_u,
		( KeepAutoquoting == TRUE ) ? "enabled" : "disabled"
	    );

    // now, display the underlying in the bin - point to the first
    bin_under_node_px = (gen_buf_node_xt *)bin_data_px->bin_under_hdr_x.chain.next;
    while ( bin_under_node_px )
    {
	// point to the underlying
	under_data_px = (ise_underlying_xt *)bin_under_node_px->msg_p;

	// are we autoquoting, or does it have any autoquoting params etc
	if (( under_data_px->autoquoting_b == TRUE )
	|| ( under_data_px->interest_f != 0 )
	|| ( under_data_px->volatility_f != 0 ))
	{
	    printf ("%-15s autoquote %s, interest %.2f%%, volatility %.2f%%, qty %u, openqty %u, stepup %u\n",
		    &under_data_px->under_id_s,
		    ( under_data_px->autoquoting_b == TRUE ) ? "ON" : "OFF",
		    under_data_px->interest_f * 100,
		    under_data_px->volatility_f * 100,
		    under_data_px->autoquote_quantity_u,
		    under_data_px->autoquote_open_quantity_u,
		    under_data_px->autoquote_step_up_buffer_n
		    );
	}

	// try for the next
	bin_under_node_px = (gen_buf_node_xt *)bin_under_node_px->chain.next;
    }	// while we have a underlying node in the bin's chain

    printf ("\nDefaults :- qty %u, openqty %u, stepup %u\n",
		click_px->autoquote_quantity_u,
		click_px->autoquote_open_quantity_u,
		click_px->autoquote_step_up_buffer_n
	    );
}   // autoquote_show

// --------------------------------------------------------------------------

int32 autoquote_change ( click_details_xt *click_px,
			    int choice_i,
			    int32 new_value_i,
			    double new_value_f,
			    int argc,
			    char *argv[] )
/*
Function : autoquote_change
Author	 : Neil Cossor
Modified : 20001229
Description:

    We have just had a change in the autoquote settings

    This could be setting interest rate, volatility, qty, open_qty or stepup,
    or turning autoquoting on or off

    All flavours have a common tail.. a list of items we are changing the settings for

Input Params:

    - pointer to everything we know about the current click session / system
    - which particular operation we are doing in changing the autoquote settings
    - an int32, which we are to use if the operation is a set of an integer e.g. step-up buffer
    - a double precision floating point, used if setting a double e.g.interest or volatility
    - C-style count of how many string args which follow
    - array of pointers to strings, each of which specifies an underlying / wildcard 
	we are to apply the autoquote settings to
*/
{
    int32   status_i = SUCCESSFUL;
    // local var, for looping thru arrays
    uint16		item_u;
    // which 'case' did the caller specify for each entry in the argv list.. *, %bin, <underlying>
    int			under_selected_i;
    // our list of commodities etc the caller specified.. list of commodities, and a count
    ise_underlying_list_xt	auto_under_list_x;
    // pointer to data about one underlying
    ise_underlying_xt * ise_under_data_px;


    // some 'selectors' used in autoquote commands
    static char *autoquote_under_selectors_s [] =
    {
	"*",	    // all underlying
	"%BIN",	    // my default bin (auto set for MM, can be manually set for EAM)
	"%FOLIO"    // a portfolio previously specified
    };
    // enumerated list, corresponding 1:1 (and in same order) as string list
    enum autoquote_under_selectors_et { auto_under_all_e, auto_under_bin_e, auto_under_folio_e };


    // start out with an 'empty' list of specific underlying to subscribe to :- 0 count, all entries 0
    memset ( &auto_under_list_x, 0, sizeof (auto_under_list_x));


    // the argc and argv[] should give the list of underlying we are changing the settings for.

    if ( argc < 1 )
    {
	printf ("Insufficent arguments.. AUTOQUOTE requires one of *|%%bin|%%folio|<under>[,...]\n" );
	return INVALID_ARGUMENTS;
    }

    for ( item_u = 0;
	    item_u < argc; 
	    item_u++ )
    {
	under_selected_i = select_string_option ( 3, autoquote_under_selectors_s, argv[item_u] );
	switch ( under_selected_i )
	{
	case auto_under_all_e:
	case auto_under_bin_e:
	    /* '*' and %bin are equivalent in an AUTOQUOTE command.. you are only allowed
		to quote in your own bin anyway

		Must substitute in all of the underlying for this bin
		Of course, we must be a CMM / PMM for this to make sense.

		If we are NOT an MM, then we are NOT allowed to autoquote anyway..
	    */

	    // illegal with any other underlying in this list
	    if ( argc != 1 )
	    {
		printf ("Error - wildcard *|%%BIN in autoquote, with other items also specified\n" );
		return INVALID_ARGUMENTS;
	    }	// oops - all underlying wildcard, along with specific underlying

	    status_i = subscr_list_bin ( click_px,
					    &auto_under_list_x,
					    click_px->our_market_x.default_bin_u );
	    // did that work ?
	    if (status_i != SUCCESSFUL)
	    {
		printf ("Error doing AUTOQUOTE cmd using bin %u\n",
			    click_px->our_market_x.default_bin_u );
		return status_i;
	    }

	    break;

	case auto_under_folio_e:
	    // %folio
	    not_yet_implemented ("folio selectors in AUTOQUOTE command");
	    return INVALID_ARGUMENTS;

	default:
	    // must be a real underlying (or %u)
	    // search for the underlying.. if unknown, then skip it from list with a warning to user
	    ise_under_data_px = find_under_by_name_s ( click_px,
							argv [item_u ] );
	    // did we find a match
	    if ( ise_under_data_px )
	    {
		// known underlying.. add the commodity code to the list
		auto_under_list_x.commodity_code_u [ auto_under_list_x.underlying_count_u ]
		    = ise_under_data_px->commodity_code_u;
		auto_under_list_x.underlying_count_u++;
	    }
	    else
	    {
		// unknown underlying - report it.. and keep going, skipping this underlying
		printf ("AUTOQUOTE for unknown underlying %s\n",
			    argv [ item_u ] );
	    }   // test if we found specific underlying (by name)
	}	// switch, on the type of a particular argv[]
    }	// for loop, scanning the underlying list


    // after all that, skipping repeat etc, do we have at least one entry in the underlying list ?
    // have we at least one commodity-code entry (which could be 0 == all commodity codes)
    if ( auto_under_list_x.underlying_count_u == 0 )
    {
	printf ( "AUTOQUOTE setting with no commodity selection remaining\n" );
	// bail out of this routine
	return INVALID_ARGUMENTS;
    }	// test if we actually got at least one entry after all that

    // now whiz around, setting the appropriate values for the underlying list
    for ( item_u = 0;
	    item_u < auto_under_list_x.underlying_count_u;
	    item_u++ )
    {
	// find the original underlying
	ise_under_data_px = find_under_data_by_commodity ( click_px,
							    auto_under_list_x.commodity_code_u [ item_u ] );
	switch ( choice_i )
	{
	    case autoquote_interest_e:
		// set the interest rate
		ise_under_data_px->interest_f = new_value_f / 100;
		break;
	    case autoquote_off_e:
		// turn autoquoting off
		ise_under_data_px->autoquoting_b = FALSE;
		break;
	    case autoquote_on_e:
		// turn autoquoting on - warn the user if they don't have a reasonable interest & volatility
		ise_under_data_px->autoquoting_b = TRUE;

		if (( ise_under_data_px->interest_f <= 0.0 )
		    || ( ise_under_data_px->volatility_f <= 0.0 ))
		{
		    printf ("Warning, autoquote ON for %s, with interest %.2f%% and volatility %.2f%%\n",
				ise_under_data_px->under_id_s,
				ise_under_data_px->interest_f * 100,
				ise_under_data_px->volatility_f * 100
			    );
		}

		break;
	    case autoquote_openqty_e :
		// set the qty for quotes in opening rotations
		ise_under_data_px->autoquote_open_quantity_u = new_value_i;
		break;
	    case autoquote_qty_e :
		// set the normal (regular series status) quote qty
		ise_under_data_px->autoquote_quantity_u = new_value_i;
		break;
	    case autoquote_stepup_e :
		// set the step-up buffer qty for new quotes
		ise_under_data_px->autoquote_step_up_buffer_n = new_value_i;
		break;
	    case autoquote_volatility_e:
		// set the volatility
		ise_under_data_px->volatility_f = new_value_f / 100;
		break;
	    default:
		printf ("Illegal choice %u for autoquote_change\n",
			    choice_i );
	    return INVALID_ARGUMENTS;
	}
    }	// for loop, updating the autoquote settings for the underlying list

    return status_i;
}   // autoquote_change


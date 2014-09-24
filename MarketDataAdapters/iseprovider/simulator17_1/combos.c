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


Title : combos.c
Author: Neil Cossor

Function :

    This file provides basic combo handling to the ISESim.exe program.
    This includes
    - MO131 sending a combo, (interest-only and executable flavours)
    - MO133 amending a combo (including changing an interest-only to executable)
    - MO104/105 deleting a combo

    It will also maintain info about combos from this member, including
    - MQ131 and MQ132 querying the system re combos from this member
    - BO132 broadcast updates re member's combos

    Functionality is also provided to maintain a local copy of the public
    combobook, using
    - MQ133 query combo orderbook
    - BO131 broadcast updates we public combo orderbook

    Functionality is also provided for
    - MO136 hitting a combo in the public combo orderbook

Revision history :-

yymmdd	vers'n	author	    notes
------	------	------	    -----
0105xx	v0.00	Neil Cossor initial version - just a skeleton for now
010604			    added routines to delete a combo, create and remove a
				local combo item, update info about a combo from
				data from a broadcast
010605			    added routines to query the combobook, and member's combos
010627			    added routines to amend a combo
010709			    fix 'too much cut and past'. All combo queries failed
				to roll onto next orderbook if none found in this query.

To do :-

- more (and smarter) stuff to maintain a local combobook properly...
- maybe some smarts to allow chaining of the combos in general, and by underlying
    although these shouldn't be many of these at a time, just with large qty's

*/
#include "stdincl.h"

#include "error.h"
#include "series.h"
#include "underlying.h"
#include "utility.h"

#include "combos.h"


// ---------------------- module local constants etc -------------------------

// used for combo entry, and combo amend
static char *limit_interest_s [] =
{
    "LIMIT",
    "INTEREST"
};


// ---------------------- module (static) variables --------------------------

// the combo we are building up to send.
// this is actually where we build up the "table" of combo components legs etc
static combo_entry_xt	combo_msg_x;

// -------------------------- internal routines ---------------------------

static void remove_combo_from_list ( click_details_xt * click_px,
				gen_buf_node_xt *combo_node_px )
/*

Function : remove_combo_from_list
Author	 : Neil Cossor
Modified : 9905xx
Description :

    A combo has been deleted (at the central system level).
    It is currently in the known_combos_q.

    Remove it from the Q, and update counters etc
*/
{
    // and remove the combo from the current combo chain
    // this adjusts the Q length
    remove_buff_from_q ( &click_px->our_market_x.known_combos_q,
			    &combo_node_px->chain );

    // release the memory allocated to the combo
    free_gen_buf_node ( combo_node_px );

    // if the Q is now empty, then reset the counter for local combo ref #s
    if ( click_px->our_market_x.known_combos_q.length_i == 0 )
	click_px->maxLocalComboNumber_u = 0;
}

// ---------------------------------------------------------------------------


static gen_buf_node_xt *create_combo_entry ( uint16 localNumber_n )
/*
Function : create_combo_entry
Author	 : Neil Cossor
Modified : 20010604
Description:

    Creates and initialises a combo entry
    The combo_entry is NOT added to the chain of known combos
    - this is the responsibility of the caller
*/
{
    // pointer to the Q'able block, which we will contain a ptr to actual combo
    gen_buf_node_xt	*combo_node_px;
    // point to the actual combo data
    combo_entry_xt	*combo_data_px;

    // allocate space to for the Q'able container, and the actual combo data
    combo_node_px = new_gen_buf_node ( sizeof ( combo_entry_xt ));
    // point to the combo data block of what we just got
    combo_data_px = (combo_entry_xt *)combo_node_px->msg_p;

    combo_data_px->localNumber_n = localNumber_n;

    return combo_node_px;
}   // create_combo_entry

// ---------------------------------------------------------------------------

void clear_combo_legs ()
/*
Function:   clear_combo_legs
Author:     Neil Cossor
Modified:   20010607
Description:

    We wish to clear out the components in the (in-memory) combo table.
    This can be because of a COMBO,CLEAR user command, or
    because we succeeded in doing a COMBO,SEND
*/
{
    // zot the combo table i.e. the combo message
    // this implicitly clears the # of legs
    memset ( &combo_msg_x, 0, sizeof ( combo_msg_x ));
}   // clear_combo_legs

// -----------------------------------------------------------------------

void display_one_combo ( combo_entry_xt * combo_data_px )
/*
Function:   display_one_combo
Author:     Neil Cossor
Modified:   20010529
Description:

    Display all info about one combo order.

    Use this common routine, for displaying all combo orders, for displaying
    a combo which has been modified (say partially traded, or amended) etc
*/
{
    // the string form of a premium, with appropriate # of decimal places
    series_premium_st	    premium_s;
    // pointer to uint64 as a structure, so we can dump out each half of order ID
    quad_conv_t * convOrderNo;
    unsigned		    item_index_u;


    // point structure pointer at the 64bit order ID
//    (uint64 *)uint64_pxu = &(combo_data_px->orderID_u);

    format_whole_decimal_s ( (char *)&premium_s,
				combo_data_px->premium_i,
				combo_data_px->underlying_px->traded_instr_class_px->dec_in_premium_u,
				MAX_SERIES_PREMIUM );	// allowed width

    // if it a combo from another user, some fields are hidden, which can foul up the various
    // <>_to_string routines. If it is ours, we will know who the owning member is
	convOrderNo = (quad_conv_t *) &(combo_data_px->orderID_u);
    if ( combo_data_px->is_ours_u == TRUE )
	
	// our combo
	printf ( "%-15s %6d  %-6s %5d %10s  %08lX:%08lX  %-8s %-8s %4u %-5s %-15s\n",
		    order_category_to_string (combo_data_px->order_category_u ),
		    combo_data_px->localNumber_n,
		    &combo_data_px->underlying_px->under_id_s, 
		    combo_data_px->quantity_u,
		    &premium_s,
		    convOrderNo->l2,
			convOrderNo->l1,
		    validity_time_to_string ( combo_data_px->validity_time_u ),
		    client_category_to_string ( combo_data_px->client_category_u ),
		    combo_data_px->no_of_legs_u,
		    combo_data_px->broker_oper_id_s,
		    &combo_data_px->userXRef_s
		 );

    else
	// public combo - we don't know some fields, like client category, the Xref
	printf ( "%-15s %6d  %-6s %5d %10s  %08lX:%08lX  %-8s %-8s %4u %-5s %-15s\n",
		    order_category_to_string (combo_data_px->order_category_u ),
		    combo_data_px->localNumber_n,
		    &combo_data_px->underlying_px->under_id_s, 
		    combo_data_px->quantity_u,
		    &premium_s,
			convOrderNo->l2,
			convOrderNo->l1,
		    validity_time_to_string ( combo_data_px->validity_time_u ),
		    "",	    // client category is private
		    combo_data_px->no_of_legs_u,
		    "",	    // brokerID is private
		    ""	    // Xref is private
		 );

    // if there is a message text in it, display it
    if ( strlen ( combo_data_px->combo_message_s ) != 0 )
	printf (" msg [%s]\n", combo_data_px->combo_message_s );

    // if there are any legs, then report the legs - bid/ask, series, and ratio
    if ( combo_data_px->no_of_legs_u != 0 )
	for ( item_index_u = 0; item_index_u < combo_data_px->no_of_legs_u; item_index_u++ )
	{
	    printf (" %s %-16s ratio %-3u",
			bid_ask_to_string ( combo_data_px->leg_x [item_index_u].bid_ask_u ),
			combo_data_px->leg_x [item_index_u].series_px->ise_name_s,
			combo_data_px->leg_x [item_index_u].multiplier_u );
	    if (combo_data_px->is_ours_u)
		printf ( "%s\n",
			    open_close_to_string ( combo_data_px->leg_x [item_index_u].open_close_u ));
	    else
		printf ("\n");

	}

}   // display_one_combo

// ------------------------------------------------------------------------

void view_combo_legs ()
/*
Function:   view_combo_legs
Author:	    Neil Cossor
Modified:   20010608
Description:

    Display the combo leg information that we have in memory at present
*/
{
    unsigned item_index_u;

    // if there are any legs, then report the legs - bid/ask, series, and ratio
    if ( combo_msg_x.no_of_legs_u == 0 )
	printf ("No combo leg data currently defined\n" );
    else
	for ( item_index_u = 0; item_index_u < combo_msg_x.no_of_legs_u; item_index_u++ )
	{
	    printf (" %s %-16s ratio %-3u %s\n",
			bid_ask_to_string ( combo_msg_x.leg_x [item_index_u].bid_ask_u ),
			combo_msg_x.leg_x [item_index_u].series_px->ise_name_s,
			combo_msg_x.leg_x [item_index_u].multiplier_u,
			open_close_to_string (combo_msg_x.leg_x [item_index_u].open_close_u) );

	}

}   // view_combo_legs

// ------------------------------------------------------------------------

static gen_buf_node_xt * find_combo_by_orderID (click_details_xt * click_px,
						quad_word orderID_u )
/*
Function : find_combo_by_orderID
Author	 : Neil Cossor
Modified : 20010529
Description:

    Finds the combo order from the known combos, by the
    supplied orderID

Input Params:

    pointer to the standard CLICK common block
    the quadword orderID to search for

Output Params:

    If found, a pointer to the (Q'able) block, which in turn contains a pointer
	to the block containing the actual info about the combo
    otherwise NULL
*/
{
    int32	    status_i = SUCCESSFUL;
    // pointer to (Q'ed) block, containing pointer to an order
    gen_buf_node_xt *combo_node_px;
    // pointer to the actual order data
    combo_entry_xt  *combo_data_px;

    /*
	Simply search the list.. not efficient, but
	considering the (typically) small # of combos... not really a problem
    */
    // point to first entry in chain of known combos - may be empty
    combo_node_px = (gen_buf_node_xt *)click_px->our_market_x.known_combos_q.chain.next;
    while (combo_node_px)
    {
	// point into the actual combo data
	// this is NOT allowed to be NULL
	combo_data_px = (combo_entry_xt *)combo_node_px->msg_p;

	if ( !memcmp(&(combo_data_px->orderID_u), &orderID_u, sizeof(combo_data_px->orderID_u)) )
	{
	    // found the match - just drop out of the loop, and return ptr
	    break;
	}
	else
	{
	    // no go, try the next combo - which may be NULL, and drop us from loop
	    combo_node_px = (gen_buf_node_xt *)combo_node_px->chain.next;
	}
    }	// while (combo_node_px)

    // return NULL or the node we found
    return combo_node_px;
}   // find_combo_by_orderID

// ------------------------------------------------------------------------

static gen_buf_node_xt * find_combo_by_combobook_no (click_details_xt * click_px,
							unsigned local_ref_no )
/*
Function : find_combo_by_combobook_no
Author	 : Neil Cossor
Modified : 20010614
Description:

    Finds the combo order from the known combos,
    by the supplied local combobook #.

Input Params:

    pointer to the standard CLICK common block
    the combobook reference # to search for

Output Params:

    If found, a pointer to the (Q'able) block, which in turn contains a pointer
	to the block containing the actual info about the combo
    otherwise NULL
*/
{
    int32	    status_i = SUCCESSFUL;
    // pointer to (Q'ed) block, containing pointer to an order
    gen_buf_node_xt *combo_node_px;
    // pointer to the actual order data
    combo_entry_xt  *combo_data_px;

    /*
	Simply search the list.. not efficient, but
	considering the (typically) small # of combos... not really a problem
    */
    // point to first entry in chain of known combos - may be empty
    combo_node_px = (gen_buf_node_xt *)click_px->our_market_x.known_combos_q.chain.next;
    while (combo_node_px)
    {
	// point into the actual combo data
	// this is NOT allowed to be NULL
	combo_data_px = (combo_entry_xt *)combo_node_px->msg_p;

	if ( combo_data_px->localNumber_n == local_ref_no )
	{
	    // found the match - just drop out of the loop, and return ptr
	    break;
	}
	else
	{
	    // no go, try the next combo - which may be NULL, and drop us from loop
	    combo_node_px = (gen_buf_node_xt *)combo_node_px->chain.next;
	}
    }	// while (combo_node_px)

    // return NULL or the node we found
    return combo_node_px;
}   // find_combo_by_combobook_no

// ---------------------------------------------------------------------------

void delete_combo_by_orderID ( click_details_xt * click_px,
				quad_word delete_orderID_u,
				unsigned local_byte_order)
/*
Function:   delete_combo_by_orderID
Author:	    Neil Cossor
Modified:   20010529
Description:

    We know a particular combo has been deleted.. delete it from memory,
    if we know about it.

    This occured because we received a broadcast about an combo delete.
    If we delete one with a command locally, we wait for the bcast
    to update our combobook.

Input Params:

    - pointer to the structure with everything we know about the OM/ISE system
    - the orderID to attempt to delete.. in local-endian order UNLESS
    - the local_byte_order param indicates FALSE i.e. need to swap the bytes pre search
*/
{
    // pointer to Q'able node, containing pointer to an combo
    gen_buf_node_xt	*combo_node_px;
    // local-endian copy of orderID, if it wasn't to start with
    quad_word		local_orderID_u;

    if (local_byte_order == FALSE)
		PUTQUAD(local_orderID_u, delete_orderID_u);
		//memcpy(&(local_orderID_u.quad_word), &delete_orderID_u, sizeof(delete_orderID_u) );
    else
		PUTQUAD(local_orderID_u, delete_orderID_u);

    // delete node from our local combobook, if we knew about it
    combo_node_px = find_combo_by_orderID ( click_px,
					    local_orderID_u );
    if ( combo_node_px )
	remove_combo_from_list ( click_px,
				    combo_node_px );
    // if it wasn't in local combobook, we either mightn't have done the initial queries,
    // of might be subscribed to both bo131 (public) and bo132 (member) bcast
    // and get both bcasts. NB it appears the public precedes the member

}   // delete_combo_by_orderID

// ---------------------------------------------------------------------------

void update_combo_qty_by_orderID ( click_details_xt * click_px,
				    quad_word change_orderID_u,
				    unsigned local_byte_order,
				    uint32 new_qty_u )
/*
Function:   update_combo_qty_by_orderID
Author:	    Neil Cossor
Modified:   20011017
Description:

    We know a particular combo has had the qty change, so update it,
    if we know about it.

    This occured because we received a broadcast about an combo match (with remaining qty >0).

Input Params:

    - pointer to the structure with everything we know about the OM/ISE system
    - the orderID to attempt to update.. in local-endian order UNLESS
    - the local_byte_order param indicates FALSE i.e. need to swap the bytes pre search
    - the new qty (already in local endian order)
*/
{
    // pointer to Q'able node, containing pointer to an combo
    gen_buf_node_xt	*combo_node_px;
    // pointer to the actual combo data
    combo_entry_xt  *combo_data_px;
    // local-endian copy of orderID, if it wasn't to start with
    quad_word		local_orderID_u;

	//endian consideration no longer necesary with orderIDs since they are arrays of chars as of 17.1 -Eric Salem
    //if (local_byte_order == FALSE)
	memcpy ( &local_orderID_u, &change_orderID_u, sizeof(change_orderID_u) );
    //else
	//local_orderID_u = change_orderID_u;

    // find node in our local combobook, if we knew about it
    combo_node_px = find_combo_by_orderID ( click_px,
					    local_orderID_u );
    if ( combo_node_px )
    {
	// we know about it - update the qty

	// point into the actual combo data
	// this is NOT allowed to be NULL
	combo_data_px = (combo_entry_xt *)combo_node_px->msg_p;

	combo_data_px->quantity_u = new_qty_u;
    }
    // if it wasn't in local combobook, we either mightn't have done the initial queries,
    // or might be subscribed to both bo131 (public) and bo132 (member) bcast
    // and get both bcasts. NB it appears the public precedes the member

}   // update_combo_qty_by_orderID

// -----------------------------------------------------------------------

void display_my_combobook (click_details_xt *click_px)
/*
Function : display_my_combobook
Author	 : Neil Cossor
Modified : 200105xx
Description :

    Displays the contents of the local combobook.

    The local combobook is updated by
    - use of the MQ queries
    - broadcasts about changes to the combobook
*/
{
    // pointer to (Q'ed) block, containing pointer to an combo
    gen_buf_node_xt *combo_node_px;
    // pointer to the actual combo data
    combo_entry_xt  *combo_data_px;
   
    // point to first entry in chain of known combos - may be empty
    combo_node_px = (gen_buf_node_xt *)click_px->our_market_x.known_combos_q.chain.next;
    // are there any combos - i.e. should we spit out some column headers
    if ( combo_node_px )
    {
	printf ("\nCategory        Local#  Under  Qutty    Premium  ORDER ID           Validity Client   Legs BkrID Xref\n" );
	printf (  "_____________________________________________________________________________________________________\n");

	do  // while ( combo_node_px )
	{
	    // point into the actual combo data
	    // this is NOT allowed to be NULL
	    combo_data_px = (combo_entry_xt *)combo_node_px->msg_p;

	    display_one_combo ( combo_data_px );

	    // point to the next one, which may be NULL
	    combo_node_px = (gen_buf_node_xt *)combo_node_px->chain.next;
	} while ( combo_node_px );

	// end of the list
	printf ("\n\n");
    }
    else
    {
	printf ("The Local combobook contains no combos\n");
    }

    return;
}   // display_my_combobook

// -----------------------------------------------------------------------

void clear_local_combobook (click_details_xt *click_px)
/*
Function : clear_local_combobook
Author	 : Neil Cossor
Modified : 20000521
Description :

    Clears out the local combo book, release memory involved etc.

    This happens when we logout / disconnect, or do a query for all combos.

    NOTE this does NOT release the resources in the actual Q header
*/
{
    // release all of the Q of known combos, which also zots the account
    flush_gen_buf_node_q ( &click_px->our_market_x.known_combos_q, TRUE );

    // and the highest local combo number ??? what is the relevance of this
    click_px->maxLocalComboNumber_u = 0;
}   // clear_local_combobook

// -----------------------------------------------------------------------

void external_update_combo ( click_details_xt *click_px,
				ise_underlying_xt   *under_data_px,	// all combos are associated with ONE underlying
				quad_word		    OM_orderID_u,
				int32		    OM_premium_i,	// price
				uint32		    OM_quantity_u,
				char		    userXRef_s[],	// free text
				uint16		    OM_validity_time_u, // GTC/ROD/IOC/FOK/FAS/AON
				uint16		    order_category_u,	// interest-only vs executable - local endian
				uint8		    client_category_u,  // cust/Firm/FARMM
				user_id_st	    broker_oper_id_s,   // who is the 'owner'
				cmta_member_st	    cmta_member_s,	// the CMTA account
				sub_account_id_st   sub_account_id_s,   // the sub account ID
				combo_message_st    message_text_s,	// message text - mandatory in 0 leg combos
				trading_code_t	    *OM_trading_code_px,// which user (and member) this combo belongs to
				// need some legs here...
				uint8		    no_legs_u,		// how many legs - may be none
				int32_t ul_price_high_i,
				int32_t ul_price_low_i,
				int32_t ul_price_method_c,
				OM_one_combo_leg_xt leg_x[4]		// and the legs, in OM format
				)
/*
Function:   external_update_combo
Author:	    Neil Cossor
Modified:   20010604
Description:

    We have info to update an item in our local combobook with.

    If we don't know about the combo at all (search by orderID)
    then we create a combo entry item, and add it into the local combobook.

    This could be triggered by a MQ131 or MQ132 query,
    a BO131 or BO132 combobook broadcast (with an add/modify flavour).

    In any case, we know about the change from an OM-formatted external message,
    rather than from us generating the combo (from ISEsim command),
    so the input fields are in OM central-system order.

    This routine is hence very similar to update_combo, BUT
    does the necessary byte-swapping.
    
    The conversion of OM string formats to C-style NUL-terminated strings
    is done by the caller, as it is messier keeping track of the field sizes here,
    and the CMTA and sub-account fields are sometimes packed as a
    single 10-char field, and sometimes as 2 5-char fields

    Note that if we get the public data first, it will have various 0 values
    and null-strings, which will be overriden by values in the member-only data.
    If, however, we get real values from the member-only version first, we
    don't want to stomp over the top of them.
*/
{
    // order ID of combo we got, in local-endian byte order
    quad_word		search_orderID_u;
    // pointer to (Q'ed) block, containing pointer to a combo
    gen_buf_node_xt	*combo_node_px;
    // pointer to the actual combo data
    combo_entry_xt	*combo_data_px;
    // pointer to selected series
    ise_series_info_pt	series_data_px = NULL;
    // local loop var
    unsigned		item_idx_u;
    // local copy of the series record we got back in an combo
    ise_series_xt	ise_series_x;


    // figure out whether we know about this combo (by orderID)
    memcpy ( &search_orderID_u, &OM_orderID_u, sizeof(OM_orderID_u) );

    // add it to the local list of known combos, if we don't already know about it
    combo_node_px = find_combo_by_orderID ( click_px,
					    search_orderID_u );

    // did we know about this combo already ?
    if ( !combo_node_px )
    {
	// did NOT know about this combo... must add it to the list of known

	// create an combo_entry block
	combo_node_px = create_combo_entry ( ++click_px->maxLocalComboNumber_u );
	   
	// and add it to the Q of known combos - note that most of the contents not set yet
	add_to_q_tail ( &click_px->our_market_x.known_combos_q,
			&combo_node_px->chain );

    };  // check if we already knew about this combo

    combo_data_px = (combo_entry_xt *)combo_node_px->msg_p;
    // save the pointer to the underlying data for this particular
    combo_data_px->underlying_px = under_data_px;
	
	combo_data_px->ul_price_high_i = ul_price_high_i;
	combo_data_px->ul_price_low_i = ul_price_high_i;
	combo_data_px->ul_price_method_c = ul_price_method_c;

    // all of the byte-swapped fields first - note we have already byte swapped orderID above
    combo_data_px->orderID_u = search_orderID_u;
    PUTLONG ( combo_data_px->quantity_u, OM_quantity_u );
    PUTLONG ( combo_data_px->premium_i, OM_premium_i );
    PUTSHORT ( combo_data_px->validity_time_u, OM_validity_time_u );

    // order category is local-endian already
    combo_data_px->order_category_u = order_category_u;

    // and the single-bytes, which don't need to be rearranged
    combo_data_px->no_of_legs_u = no_legs_u;
    // NOTE we do not stamp the client category over the top if a 0 is supplied
    // if we got the member BO132 bcast first, we already have a value for this field,
    // so why throw it away. If we got the BO131 first, we will now have 
    if (client_category_u != 0 )
	combo_data_px->client_category_u = client_category_u;

    // and the string fields - if no value in this update, leave values alone
    if ( userXRef_s != NULL )
	strcpy(&combo_data_px->userXRef_s[0], &userXRef_s[0]);
    // NB there may not have been a user string (broker_id) supplied
    if ( broker_oper_id_s != NULL )
	strcpy((char *)&combo_data_px->broker_oper_id_s, &broker_oper_id_s[0]);
    // NB there may not have been a CMTA account string supplied
    if ( cmta_member_s != NULL )
	strcpy ((char *)&combo_data_px->cmta_member_s, cmta_member_s );
    // NB there may not have been a CMTA subaccount string supplied - normally only for MM orders
    if ( sub_account_id_s != NULL )
	strcpy ((char *)&combo_data_px->sub_ac_s, sub_account_id_s );
    // extract the info about which user sent in the combo, if supplied
    if (OM_trading_code_px )
    {
	// we know that it belongs to our member.. note this for choices in displaying thing
	combo_data_px->is_ours_u = TRUE;
	get_ise_user_code_xt ( &combo_data_px->user_code_x,
				(struct user_code *)OM_trading_code_px );
    }
    // and the message text
    if ( message_text_s != NULL )
	strcpy ((char *)&combo_data_px->combo_message_s, message_text_s );

    // the data for the component legs
    for ( item_idx_u = 0; item_idx_u < no_legs_u; item_idx_u++ )
    {
	combo_data_px->leg_x[item_idx_u].bid_ask_u = leg_x[item_idx_u].bid_or_ask_c;
	// NB the open/close byte might not be filled, if it is a public combo..
	// so if BO131 is after BO131, we may lost info about open/close attributes.
	combo_data_px->leg_x[item_idx_u].open_close_u = leg_x[item_idx_u].open_close_req_c;
	// and the ratio data
	PUTSHORT ( combo_data_px->leg_x[item_idx_u].multiplier_u,
		    leg_x[item_idx_u].multiplier_n );
	// extract the series info, in local form
	get_click_series ( &ise_series_x,
			    &leg_x[item_idx_u].series );
	// save pointer to info for this series
	combo_data_px->leg_x[item_idx_u].series_px = find_series_by_ise_series ( click_px,
										 &ise_series_x );
    }	// for loop extracting the leg data

}   // external_update_combo

// ---------------------------------------------------------------------------

unsigned query_public_combos ( click_details_xt *click_px )
/*
Function:   query_public_combos
Author:	    Neil Cossor
Modified:   20010709
Description:

    Inquires on public combobook for all combos.
    Uses mq133.

    The resulting combos are added to the local list of known combos,
    if they were not already in that list.

Input Params:

    - pointer to all data we know about the OM / ISE central system

    We start with the orderbook 1, and loop through the lot.
    We specify bin 0 (all bins)
    We select order category to include interest-only and executable combos

Output Params:

    If we get an error, we return an error status (-ve),
    otherwise we return the # of combos found (which may be 0)

    Updates the internal list of combos.
*/
{
    generic_txn_xt	OMmsg_x;
    // size of our actual CLICK query/command
    uint32		tx_length_u = sizeof ( OMmsg_x.tx_x.query_combo_public_x );
    // status var to return, with good default initial value
    int 		status_i = SUCCESSFUL;
    // pointer to one interesting chunk of the response msg
    answer_combo_ob_ma133_item_t	*txn_reply_px;
    // the commodity code of underlying in the bcast
    uint16		found_commodity_code_u;
    // the underlying record for the underlying
    ise_underlying_xt	*under_data_px;
    // count of how many combos we found
    int 		combos_found_i = 0;
    uint16		item_index_u,	// local loop var
			items_found_u;	// limit on loop - may be endian-swapped from response

    // local-endian copies of fields from query response
    uint16		order_category_u;
    combo_message_st	message_text_s;


    /* set up query to have 0 in all fields - which includes selection qualifiers
	This implies
	- both bids and asks
	- all instrument groups
	- no specific combo/order #
	- all order categories
    */
    memset( &OMmsg_x.tx_x.query_combo_public_x, 0, tx_length_u );

    // MQ133
    pack_OM_header ( &OMmsg_x, tx_length_u, 'M', 'Q', 133 );

    // wildcarding series - do nothing.. all fields already 0

    // we are doing all orderbooks, so start with #1
    OMmsg_x.tx_x.query_combo_public_x.orderbook_c = 1;
    // and specified bin
    OMmsg_x.tx_x.query_combo_public_x.bin_c = 0;

    // whose qualifier - leave all nulls.. already done by memset above

    // put in order category for interest-only and limit combos
    PUTSHORT ( OMmsg_x.tx_x.query_combo_public_x.order_category_n,
		(ORDER_CAT_COMBO_LIMIT | ORDER_CAT_COMBO_INTEREST) );

    // save the facility code and fn code for logging
    OMmsg_x.fn_code = LOG_K_Q_COMBO;
    OMmsg_x.facility_u = click_px->facility_no_u + 7;	// EP7/ET7

    
    /* now whiz around, doing calls until we run out of combos from the central system
	that match our query. This means we loop until we have been thru all orderbooks.
    */
    do // while ( TRUE )
    {
	status_i = do_click_query_rx ( click_px,
					&OMmsg_x,
					tx_length_u,
					LOG_K_R_COMBOBOOK);
	if ( status_i < SUCCESSFUL)
	{
	    // note no items found - bail out of loop
	    break;
	}
	else
	{
	    // good status - how many combos did we retrieve - endian
	    PUTSHORT ( items_found_u,
			click_px->log_reply_x.trans_reply_x.answer_combobook_x.items_n );

	    /* if we got no items, then we may have to try the next orderbook
		(bin could be split)
	    */

	    // loop thru the combos we found - maybe none
	    for ( item_index_u = 0;
		    item_index_u < items_found_u;
		    item_index_u++ )
	    {
		// point to this chunk of info about one combo
		txn_reply_px = &click_px->log_reply_x.trans_reply_x.answer_combobook_x
					.item[item_index_u];

		switch ( txn_reply_px->interest_order_c )
		{
		case COMBO_INTEXEC_LIMIT:
		    order_category_u = ORDER_CAT_COMBO_LIMIT;
		    break;
		case COMBO_INTEXEC_INTEREST:
		    order_category_u = ORDER_CAT_COMBO_INTEREST;
		    break;
		default:
		    printf (" Error, unknown interest_only_c value %u in item %u\n",
				txn_reply_px->interest_order_c,
				item_index_u );
		    order_category_u = 0;
		}	// switch on values interest_only field

		// extract copy of the underyling, in local byte-order
		PUTSHORT ( found_commodity_code_u, txn_reply_px->commodity_n );
		// and search for the underlying data for this code
		under_data_px = find_under_data_by_commodity ( click_px,
								found_commodity_code_u );
		rx_click_string ( &message_text_s,
				    (char *)txn_reply_px->message_s,
				    sizeof (txn_reply_px->message_s) );

		// now go and update/create node with data about this combo
		external_update_combo ( click_px,
					under_data_px,
					txn_reply_px->order_number_u,	// orderID is public - needed for HIT
					txn_reply_px->premium_i,
					txn_reply_px->quantity_u,
					NULL,				// can't see freetext in public data
					txn_reply_px->time_validity_n,
					order_category_u,
					0,				// can't see client category in public data
					NULL,				// can't see brokerID in public data
					NULL,				//   "    "  CMTA account in public data
					NULL,				//   "    "  sub account ID in public data
					message_text_s,
					NULL,				// can't see who sent original combo in public data
					txn_reply_px->items_c,		// how many legs
					txn_reply_px->ul_price_high_i,
					txn_reply_px->ul_price_low_i,
					txn_reply_px->ul_price_method_c,
					(OM_one_combo_leg_xt *)&txn_reply_px->item[0]		// pointer to first leg
					);

		// note one more found
		combos_found_i++;
	    }	// for loop, processing individual combo records in one response

	    /* If the Orderbook, Next field in the response is 0,
		then we have finished our query.
		
		If that field is NOT zero, then there are more queries to do,
		and we need to shuffle the Next fields into the query, and continue
	    */
	    if ( click_px->log_reply_x.trans_reply_x.answer_combobook_x.orderbook_next_c != 0 )
	    {
		/* There is another query to go, according to MP server...
		    We must transfer the "up to here" fields from the response
		    into the next request
		*/
		OMmsg_x.tx_x.query_combo_public_x.orderbook_c =
		    click_px->log_reply_x.trans_reply_x.answer_combobook_x.orderbook_next_c;
		memcpy ( &OMmsg_x.tx_x.query_combo_public_x.series,
			    &click_px->log_reply_x.trans_reply_x.answer_combobook_x.series_next,
			    sizeof (series_t));
		PUTQUAD(OMmsg_x.tx_x.query_combo_public_x.order_number_u,
		    click_px->log_reply_x.trans_reply_x.answer_combobook_x.order_number_next_u);
		// NB unlike order queries, a combo doesn't inherently have a bid/ask..
		// that belongs with the legs
	    }	// test if we maxed out on combos
	    else
	    {
		/* There are no more combos. Stop doing queries.
		*/
		break;
	    }
	}   // test if success status from OMNIAPI call
    } while ( TRUE );

    if ( status_i >= SUCCESSFUL )
    {
	printf ("%u combos in public combobook\n", combos_found_i );
	return combos_found_i;
    }
    else
	return status_i;

}   // query_public_combos

// ---------------------------------------------------------------------------

unsigned query_member_active_combos ( click_details_xt *click_px )
/*
Function:   query_member_active_combos
Author:	    Neil Cossor
Modified:   20010709
Description:

    Inquires on private (member) combobook for active combos.
    Uses mq131.

    The resulting combos are added to the local list of known combos,
    if they were not already in that list. If they were, then we update
    the details.

Input Params:

    - pointer to all data we know about the OM / ISE central system

    We start with the orderbook 1, and loop through the lot.
    We specify bin 0 (all bins)
    We select order category to include interest-only and executable combos

Output Params:

    If we get an error, we return an error status (-ve),
    otherwise we return the # of combos found (which may be 0)

    Updates the internal list of combos.
*/
{
    generic_txn_xt	OMmsg_x;
    // size of our actual CLICK query/command
    uint32		tx_length_u = sizeof ( OMmsg_x.tx_x.query_combo_x );
    // status var to return, with good default initial value
    int 		status_i = SUCCESSFUL;
    // pointer to one interesting chunk of the response msg
    answer_combo_order_ma131_item_t	*txn_reply_px;
    // the commodity code of underlying in the bcast
    uint16		found_commodity_code_u;
    // the underlying record for the underlying
    ise_underlying_xt	*under_data_px;
    // count of how many combos we found
    int 		combos_found_i = 0;
    uint16		item_index_u,	// local loop var
			items_found_u;	// limit on loop - may be endian-swapped from response

    // local-endian copies of fields from query response
    uint16		order_category_u;
    xref_st		free_text_s;
    user_id_st		broker_oper_id_s;   // the broker ID who owns this combo
    cmta_member_st	cmta_member_s;	    // CMTA member ID string
    sub_account_id_st	sub_ac_s;	    // and sub account ID string
    combo_message_st	message_text_s;


    /* set up query to have 0 in all fields - which includes selection qualifiers
	This implies
	- both bids and asks
	- all instrument groups
	- no specific combo/order #
	- all order categories
    */
    memset( &OMmsg_x.tx_x.query_combo_x, 0, tx_length_u );

    // MQ133
    pack_OM_header ( &OMmsg_x, tx_length_u, 'M', 'Q', 131 );

    // wildcarding series - do nothing.. al fields already 0

    // we are doing all orderbooks, so start with #1
    OMmsg_x.tx_x.query_combo_x.orderbook_c = 1;
    // and specified bin
    OMmsg_x.tx_x.query_combo_x.bin_c = 0;

    // whose qualifier - specify our country and member, but leave user empty (whole firm)
    put_ise_user_code_xt ( (user_code_t *)&OMmsg_x.tx_x.query_combo_x.whose.trading_code,
			    &click_px->user_code_x );
    memset ( &OMmsg_x.tx_x.query_combo_x.whose.trading_code.user_id_s,
		0,
		sizeof (OMmsg_x.tx_x.query_combo_x.whose.trading_code.user_id_s));
    // NOTE leave the whose.ex_client_s as NULL. If you put anything else in there,
    // the search selects out combos with exactly that client string...

    // put in order category for interest-only and limit combos
    PUTSHORT ( OMmsg_x.tx_x.query_combo_x.order_category_n,
		(ORDER_CAT_COMBO_LIMIT | ORDER_CAT_COMBO_INTEREST) );

    // save the facility code and fn code for logging
    OMmsg_x.fn_code = LOG_K_Q_COMBO;
    OMmsg_x.facility_u = click_px->facility_no_u + 7;	// EP7/ET7

    
    /* now whiz around, doing calls until we run out of combos from the central system
	that match our query. This means we loop until we have been thru all orderbooks.
    */
    do // while ( TRUE )
    {
	status_i = do_click_query_rx ( click_px,
					&OMmsg_x,
					tx_length_u,
					LOG_K_R_COMBO_ACTIVE);
	if ( status_i < SUCCESSFUL)
	{
	    // note no items found - bail out of loop
	    break;
	}
	else
	{
	    // good status - how many combos did we retrieve - endian
	    PUTSHORT ( items_found_u,
			click_px->log_reply_x.trans_reply_x.answer_combo_active_x.items_n );

	    // if we got no items, then we may have to try the next orderbook
	    //	(bin could be split)

	    // loop thru the combos we found - maybe none
	    for ( item_index_u = 0;
		    item_index_u < items_found_u;
		    item_index_u++ )
	    {
		// point to this chunk of info about one combo
		txn_reply_px = &click_px->log_reply_x.trans_reply_x.answer_combo_active_x
					.item[item_index_u];

		switch ( txn_reply_px->interest_order_c )
		{
		case COMBO_INTEXEC_LIMIT:
		    order_category_u = ORDER_CAT_COMBO_LIMIT;
		    break;
		case COMBO_INTEXEC_INTEREST:
		    order_category_u = ORDER_CAT_COMBO_INTEREST;
		    break;
		default:
		    printf (" Error, unknown interest_only_c value %u in item %u\n",
				txn_reply_px->interest_order_c,
				item_index_u );
		    order_category_u = 0;
		}	// switch on values interest_only field

		// extract copy of the underyling, in local byte-order
		PUTSHORT ( found_commodity_code_u, txn_reply_px->commodity_n );
		// and search for the underlying data for this code
		under_data_px = find_under_data_by_commodity ( click_px,
								found_commodity_code_u );

		rx_click_string ( &message_text_s,
				    (char *)txn_reply_px->message_s,
				    sizeof (txn_reply_px->message_s) );
		rx_click_string ( &free_text_s,
				    (char *)txn_reply_px->free_text_s,
				    sizeof (txn_reply_px->free_text_s) );
		rx_click_string ( &broker_oper_id_s,
				    txn_reply_px->broker_id_s,
				    sizeof (txn_reply_px->broker_id_s));
		// grab the two parts of the client ID, which are actually the CMTA a/c and sub a/c IDs
		rx_click_string ( &cmta_member_s,
				    &txn_reply_px->ex_client_s [0],
				    MAX_CMTA_MEMBER );
		rx_click_string ( &sub_ac_s,
				    &txn_reply_px->ex_client_s [MAX_CMTA_MEMBER],	// start offset
				    MAX_SUB_ACCOUNT_ID );

		external_update_combo ( click_px,
					under_data_px,
					txn_reply_px->order_number_u,	// orderID
					txn_reply_px->premium_i,
					txn_reply_px->quantity_u,
					free_text_s,			// can't see freetext in public data
					txn_reply_px->time_validity_n,
					order_category_u,
					txn_reply_px->client_category_c, // client category
					broker_oper_id_s,		// brokerID - used to have special "B/D"
					cmta_member_s,			// CMTA account
					sub_ac_s,			// sub account ID
					message_text_s,
					&txn_reply_px->trading_code,	// who sent original combo in public data
					txn_reply_px->items_c,		// how many legs
					txn_reply_px->ul_price_high_i,
					txn_reply_px->ul_price_low_i,
					txn_reply_px->ul_price_method_c,
					(OM_one_combo_leg_xt *)&txn_reply_px->item[0]		// pointer to first leg
					);

		// note one more found
		combos_found_i++;
	    }	// for loop, processing individual combo records in one response

	    /* If the Orderbook, Next field in the response is 0,
		then we have finished our query.
		
		If that field is NOT zero, then there are more queries to do,
		and we need to shuffle the Next fields into the query, and continue
	    */
	    if ( click_px->log_reply_x.trans_reply_x.answer_combo_active_x.orderbook_next_c != 0 )
	    {
		/* There is another query to go, according to MP server...
		    We must transfer the "up to here" fields from the response
		    into the next request
		*/
		OMmsg_x.tx_x.query_combo_x.orderbook_c =
		    click_px->log_reply_x.trans_reply_x.answer_combo_active_x.orderbook_next_c;
		memcpy ( &OMmsg_x.tx_x.query_combo_x.series,
			    &click_px->log_reply_x.trans_reply_x.answer_combo_active_x.series_next,
			    sizeof (series_t));
		PUTQUAD(OMmsg_x.tx_x.query_combo_x.order_number_u,
		    click_px->log_reply_x.trans_reply_x.answer_combo_active_x.order_number_next_u);
		// NB unlike order queries, a combo doesn't inherently have a bid/ask..
		// that belongs with the legs
	    }	// test if we maxed out on combos
	    else
	    {
		/* There are no more combos. Stop doing queries.
		*/
		break;
	    }
	}   // test if success status from OMNIAPI call
    } while ( TRUE );

    if ( status_i >= SUCCESSFUL )
    {
	printf ("%u combos in combobook belong to this member\n", combos_found_i );
	return combos_found_i;
    }
    else
	return status_i;

}   // query_member_active_combos

// ---------------------------------------------------------------------------

unsigned query_member_inactive_combos ( click_details_xt *click_px )
/*
Function:   query_member_inactive_combos
Author:	    Neil Cossor
Modified:   20010709
Description:

    Inquires on private (member) combobook for inactive combos.
    Uses mq132.

    The resulting combos are NOT added to the local list of known combos,
    but are simply displayed on the screen.

Input Params:

    - pointer to all data we know about the OM / ISE central system

    We start with the orderbook 1, and loop through the lot.
    We specify bin 0 (all bins)
    We select order category to include interest-only and executable combos

Output Params:

    If we get an error, we return an error status (-ve),
    otherwise we return the # of combos found (which may be 0)

    No change made to local combobook.
*/
{
    generic_txn_xt	OMmsg_x;
    // size of our actual CLICK query/command
    uint32		tx_length_u = sizeof ( OMmsg_x.tx_x.query_combo_x );
    // status var to return, with good default initial value
    int 		status_i = SUCCESSFUL;
    // pointer to one interesting chunk of the response msg
    answer_inactive_combo_ma132_item_t	*txn_reply_px;
    // the commodity code of underlying in the bcast
    uint16		found_commodity_code_u;
    // the underlying record for the underlying
    ise_underlying_xt	*under_data_px;
    // pointer to (Q'ed) block, containing pointer to a combo
    gen_buf_node_xt	*combo_node_px;
    // count of how many combos we found
    int 		combos_found_i = 0;
    // order ID of combo we got, in local-endian byte order
    quad_word		search_orderID_u;
    // pointer to uint64 as a structure, so we can dump out each half of order ID
//    uint64_xt		*uint64_pxu;

    uint16		item_index_u,	// local loop var
			items_found_u;	// limit on loop - may be endian-swapped from response

    // local-endian copies of fields from query response
    uint16		order_category_u;
    // and of quantity and premium
    uint32		quantity_u,
			premium_i;
    // the string form of a premium, with appropriate # of decimal places
    series_premium_st	premium_s;

	quad_conv_t * convOrderNo;


    /* set up query to have 0 in all fields - which includes selection qualifiers
	This implies
	- both bids and asks
	- all instrument groups
	- no specific combo/order #
	- all order categories
    */
    memset( &OMmsg_x.tx_x.query_combo_x, 0, tx_length_u );

    // MQ133
    pack_OM_header ( &OMmsg_x, tx_length_u, 'M', 'Q', 132 );

    // wildcarding series - do nothing.. al fields already 0

    // we are doing all orderbooks, so start with #1
    OMmsg_x.tx_x.query_combo_x.orderbook_c = 1;
    // and specified bin
    OMmsg_x.tx_x.query_combo_x.bin_c = 0;

    // whose qualifier - specify our country and member, but leave user empty (whole firm)
    put_ise_user_code_xt ( (user_code_t *)&OMmsg_x.tx_x.query_combo_x.whose.trading_code,
			    &click_px->user_code_x );
    memset ( &OMmsg_x.tx_x.query_combo_x.whose.trading_code.user_id_s,
		0,
		sizeof (OMmsg_x.tx_x.query_combo_x.whose.trading_code.user_id_s));
    // NOTE leave the whose.ex_client_s as NULL. If you put anything else in there,
    // the search selects out combos with exactly that client string...

//    put in order category for interest-only and limit combos
    PUTSHORT ( OMmsg_x.tx_x.query_combo_x.order_category_n,
		(ORDER_CAT_COMBO_LIMIT | ORDER_CAT_COMBO_INTEREST) );

    // save the facility code and fn code for logging
    OMmsg_x.fn_code = LOG_K_Q_COMBO;
    OMmsg_x.facility_u = click_px->facility_no_u + 7;	// EP7/ET7

//    (uint64 *)uint64_pxu = &search_orderID_u;
    
    /* now whiz around, doing calls until we run out of combos from the central system
	that match our query. This means we loop until we have been thru all orderbooks.
    */
    do // while ( TRUE )
    {
	status_i = do_click_query_rx ( click_px,
					&OMmsg_x,
					tx_length_u,
					LOG_K_R_COMBO_INACTIVE );
	if ( status_i < SUCCESSFUL)
	{
	    // note no items found - bail out of loop
	    break;
	}
	else
	{
	    // good status - how many combos did we retrieve - endian
	    PUTSHORT ( items_found_u,
			click_px->log_reply_x.trans_reply_x.answer_combo_inactive_x.items_n );

	    /* if we got no items, then we may have to try the next orderbook
		(bin could be split)..
	    */

	    // loop thru the combos we found - maybe none
	    for ( item_index_u = 0;
		    item_index_u < items_found_u;
		    item_index_u++ )
	    {
		// point to this chunk of info about one combo
		txn_reply_px = &click_px->log_reply_x.trans_reply_x.answer_combo_inactive_x
					.item[item_index_u];

		switch ( txn_reply_px->interest_order_c )
		{
		case COMBO_INTEXEC_LIMIT:
		    order_category_u = ORDER_CAT_COMBO_LIMIT;
		    break;
		case COMBO_INTEXEC_INTEREST:
		    order_category_u = ORDER_CAT_COMBO_INTEREST;
		    break;
		default:
		    printf (" Error, unknown interest_only_c value %u in item %u\n",
				txn_reply_px->interest_order_c,
				item_index_u );
		    order_category_u = 0;
		}	// switch on values interest_only field

		// extract copy of the underyling, in local byte-order
		PUTSHORT ( found_commodity_code_u, txn_reply_px->commodity_n );
		// and search for the underlying data for this code
		under_data_px = find_under_data_by_commodity ( click_px,
								found_commodity_code_u );

		// grab local-endian copy of orderID etc
		PUTQUAD(search_orderID_u, txn_reply_px->order_number_u);
		PUTLONG ( quantity_u, txn_reply_px->quantity_u );
		PUTLONG ( premium_i, txn_reply_px->premium_i );
		// and reformat premium with appropriate decimals
		format_whole_decimal_s ( (char *)&premium_s,
					    premium_i,
					    under_data_px->traded_instr_class_px->dec_in_premium_u,
					    MAX_SERIES_PREMIUM );	// allowed width


		// display SOME info about this item
		convOrderNo = (quad_conv_t *) &(search_orderID_u.quad_word);
		printf ( "inactive %s in %s, orderID [%08lX:%08lX] %u@%s with %u legs\n",
			order_category_to_string ( order_category_u ),
			&under_data_px->under_id_s,
			convOrderNo->l2,
			convOrderNo->l1,
			quantity_u,
			premium_s,
			txn_reply_px->items_c
			);


		/* if this combo is in our known combobook, then we need to 
		    'inactivate' it i.e. delete it. Technically this is unlikely, 
		    unless we are not subscribed to the bo131 member combo bcast,
		    which is pretty silly if we care about our member's combos */

		// figure out whether we know about this combo (by orderID)
		combo_node_px = find_combo_by_orderID ( click_px,
							search_orderID_u );
		if (combo_node_px)
		    delete_combo_by_orderID ( click_px, search_orderID_u, TRUE );

		// note one more found
		combos_found_i++;
	    }	// for loop, processing individual combo records in one response

	    /* If the Orderbook, Next field in the response is 0,
		then we have finished our query.
		
		If that field is NOT zero, then there are more queries to do,
		and we need to shuffle the Next fields into the query, and continue
	    */
	    if ( click_px->log_reply_x.trans_reply_x.answer_combo_inactive_x.orderbook_next_c != 0 )
	    {
		/* There is another query to go, according to MP server...
		    We must transfer the "up to here" fields from the response
		    into the next request
		*/
		OMmsg_x.tx_x.query_combo_x.orderbook_c =
		    click_px->log_reply_x.trans_reply_x.answer_combo_inactive_x.orderbook_next_c;
		memcpy ( &OMmsg_x.tx_x.query_combo_x.series,
			    &click_px->log_reply_x.trans_reply_x.answer_combo_inactive_x.series_next,
			    sizeof (series_t));
		PUTQUAD(OMmsg_x.tx_x.query_combo_x.order_number_u,
		    click_px->log_reply_x.trans_reply_x.answer_combo_inactive_x.order_number_next_u);
		// NB unlike order queries, a combo doesn't inherently have a bid/ask..
		// that belongs with the legs
	    }	// test if we maxed out on combos
	    else
	    {
		/* There are no more combos. Stop doing queries.
		*/
		break;
	    }
	}   // test if success status from OMNIAPI call
    } while ( TRUE );

    if ( status_i >= SUCCESSFUL )
    {
	printf ("%u inactivated combos for this member\n", combos_found_i );
	return combos_found_i;
    }
    else
	return status_i;

}   // query_member_inactive_combos

// ---------------------------------------------------------------------------

static unsigned send_combo ( click_details_xt *click_px,
				combo_entry_xt *combo_data_px )	
/*
Function : send_combo
Author	 : Neil Cossor
Modified : 20010605
Description:

    For the supplied combo information, sends a combo to the system
    Uses mo131

    If the combo is accepted, then we update the orderID.
*/
{
    // local buffer, containing actual msg we send
    generic_txn_xt	OMmsg_x;

    int32		status_i	     =	 SUCCESSFUL;
    // size of our actual CLICK query/command
    uint32		tx_length_u = sizeof ( OMmsg_x.tx_x.combo_trans_x );
    unsigned		item_index_u;


    // zero everything in our tx buffer
    memset( &OMmsg_x.tx_x.combo_trans_x, 0, tx_length_u );

    // save the facility code and fn code
    OMmsg_x.fn_code = LOG_K_C_COMBO_TRANS;
    OMmsg_x.facility_u = click_px->facility_no_u + 7;	// EP7/ET7

    // now we can specify orderbook_c - if no underlying nominated (and no legs)
    if ( combo_data_px->underlying_px )
	OMmsg_x.tx_x.combo_trans_x.orderbook_c = combo_data_px->underlying_px->orderbook_u;
	
	OMmsg_x.tx_x.combo_trans_x.ul_price_method_c = combo_data_px->ul_price_method_c;
    // endian fields
	PUTLONG ( OMmsg_x.tx_x.combo_trans_x.ul_price_high_i, combo_data_px->ul_price_high_i);
	PUTLONG ( OMmsg_x.tx_x.combo_trans_x.ul_price_low_i, combo_data_px->ul_price_low_i);
    PUTLONG ( OMmsg_x.tx_x.combo_trans_x.premium_i,
		combo_data_px->premium_i );
    PUTLONG ( OMmsg_x.tx_x.combo_trans_x.quantity_u,
		combo_data_px->quantity_u );
    // validity time - GTC / ROD / IOC / FOK
    PUTSHORT ( OMmsg_x.tx_x.combo_trans_x.time_validity_n,
		combo_data_px->validity_time_u );
    // limit always set (can't have Market combo)
    OMmsg_x.tx_x.combo_trans_x.order_type_c = ORDER_TYPE_LIMIT;
    // Interest-only, or executable. If more types introduced later, will need a switch
    if ( combo_data_px->order_category_u == ORDER_CAT_COMBO_LIMIT )
	OMmsg_x.tx_x.combo_trans_x.interest_order_c = COMBO_INTEXEC_LIMIT;
    else
	OMmsg_x.tx_x.combo_trans_x.interest_order_c = COMBO_INTEXEC_INTEREST;

    // EAM/FIRM/FARMM/CustDics/BrokerDealer
    OMmsg_x.tx_x.combo_trans_x.client_category_c = combo_data_px->client_category_u;

    // put in the two parts of the client ID, which are actually the CMTA a/c and sub a/c IDs
    tx_click_string ( &OMmsg_x.tx_x.combo_trans_x.ex_client_s [0],
			&combo_data_px->cmta_member_s,
			MAX_CMTA_MEMBER );
    tx_click_string ( &OMmsg_x.tx_x.combo_trans_x.ex_client_s [MAX_CMTA_MEMBER],
			&combo_data_px->sub_ac_s,
			MAX_SUB_ACCOUNT_ID );
    // need to set the broker_id_s
    tx_click_string ( &OMmsg_x.tx_x.combo_trans_x.broker_id_s,
			&combo_data_px->broker_oper_id_s,
			sizeof (OMmsg_x.tx_x.combo_trans_x.broker_id_s));
    // and Xref field
    tx_click_string ( &OMmsg_x.tx_x.combo_trans_x.free_text_s[0],
		     combo_data_px->userXRef_s, 
		     sizeof (OMmsg_x.tx_x.combo_trans_x.free_text_s ) );
    // and any message text
    tx_click_string ( &OMmsg_x.tx_x.combo_trans_x.message_s[0],
		     combo_data_px->combo_message_s, 
		     sizeof (OMmsg_x.tx_x.combo_trans_x.message_s ) );

    // and the leg data
    OMmsg_x.tx_x.combo_trans_x.items_c = combo_data_px->no_of_legs_u;
    if ( combo_data_px->no_of_legs_u != 0 )
    {
	for ( item_index_u = 0; item_index_u < combo_data_px->no_of_legs_u; item_index_u++ )
	{
	    // save the series data
	    put_click_series ( &OMmsg_x.tx_x.combo_trans_x.item [item_index_u].series,
				&combo_data_px->leg_x [item_index_u].series_px->ise_series_x );
	    // bid / ask
	    OMmsg_x.tx_x.combo_trans_x.item [item_index_u].bid_or_ask_c = 
		    combo_data_px->leg_x [item_index_u].bid_ask_u;
	    // open/close setting for a/c during OCC settlement
	    OMmsg_x.tx_x.combo_trans_x.item [item_index_u].open_close_req_c = 
		    combo_data_px->leg_x [item_index_u].open_close_u;
	    // and the multiplier / ratio for this leg - should be 1 <= N <= 100
	    PUTSHORT ( OMmsg_x.tx_x.combo_trans_x.item [item_index_u].multiplier_n,
			combo_data_px->leg_x [item_index_u].multiplier_u );
	}   // for loop, packing the legs
    }
    else
    {
	// if there are zero legs, then we must pack a country, market and underlying
	// into the header series, otherwise leave as NULs
	OMmsg_x.tx_x.combo_trans_x.series.country_c = combo_data_px->underlying_px->country_u;
	OMmsg_x.tx_x.combo_trans_x.series.market_c = click_px->our_market_x.market_u;
	PUTSHORT ( OMmsg_x.tx_x.combo_trans_x.series.commodity_n,
		    combo_data_px->underlying_px->commodity_code_u );
    }	// test if zero legs

    // adjust the size of the tx down for the actual # of legs in the msg
    tx_length_u -= ( ( 10 - combo_data_px->no_of_legs_u ) *
			sizeof ( OMmsg_x.tx_x.combo_trans_x.item [0] ));

    pack_OM_header ( &OMmsg_x, tx_length_u, 'M', 'O', 131);
    status_i = do_click_tx_rx ( click_px,
				    &OMmsg_x,
				    tx_length_u );
    if ( status_i < SUCCESSFUL)
    {
	// do nothing, error already reported by do_click_tx_rx
    }
    else
    {
	// update the order ID of the combo
		memcpy( &(combo_data_px->orderID_u),   &(click_px->log_reply_x.OrderIdent_u), sizeof(click_px->log_reply_x.OrderIdent_u));
    }

    // save this combo as new last
    memcpy ( &click_px->last_combo_x,
		combo_data_px,
		sizeof ( click_px->last_combo_x ));
    // we also need to save the user/member of who we are logged on at present
    strcpy ( click_px->last_combo_x.user_code_x.country_id_s,
		click_px->user_code_x.customer_id_s );
    strcpy ( click_px->last_combo_x.user_code_x.user_id_s,
		click_px->user_code_x.user_id_s );

    return (status_i);
}   // send_combo

// ---------------------------------------------------------------------------

int32 parse_send_combo ( click_details_xt * click_px,
			    int argc,
			    char *argv[] )
/*
Function:   parse_send_combo
Author:     Neil Cossor
Modified:   20010607
Description:

    User wishes to enter a combo order.
    Any legs (if any) shoulf have previously been set up with COMBO,ASK or COMBO,BID
    Uses mo131.

    This wraps up the combo table components (previously set by
    COMBO,ASK or COMBO,BID commands), parses the extra fields for
    the overall combo (like premium, quantity, message text etc, 
    and sends it to the central system.

    If successful, then clears out the combo table.

    (mandatory) arguments are
    argv[0] COMBO
    argv[1] SEND
    argv[2] <underlying name>	ignored unless there are no legs
    argv[3] quantity
    argv[4] premium
    argv[5] INTEREST|LIMIT
	argv[6] Underlying Price Method
	argv[7] Underlying Price High
	argv[8] Underlying Price Low

    optional arguments are
    argv[9] validity time, default FAS day
    argv[10] client category : customer|firm|farmm etc, default customer
    argv[11] brokerID
    argv[12] cmta
    argv[13] sub account
    argv[14] message text (up to 100 chars).. mandatory if 0 legs
    argv[15] cross-reference (gets passed through OCC)
*/
{
    int			status_i = SUCCESSFUL;
    /* temp, long number.. when reading a number, C only provides functions
	which put the output in a longword.. we need a word sometimes.
	Also used for handling premium, where we need to juggle the number,
	and then do an endian push
    */
    int 		temp_i;

    if ( !click_px->logged_on_i )
    {
	prompt_logged_on ( "Combo,Send" );
	return NOT_LOGGED_ON;
    }

    // have we got enough arguments to pack the rest of the header ?
    if ( argc < 9 )
    {
	// no - report error, and return
	printf ( "Combo header require COMBO,SEND,<underlying|0>,<qty>,<premium>,<INTEREST|LIMIT>,<Underlying Price Method>,<Underlying Price High>,<Underlying Price Low>\n"
	         " [,<validity>[,<client cat>[,<brokerID>[,<cmta>[,<sub a/c>[,<message>[,<Xref>]]]]]]]\n" );
	return SUCCESSFUL;
    }

    /* if there are any legs present, extract the underyling from the first series.
	If not, ensure that we had one nominated here as an argument (that exists)
	NB we have already done basic tests, and found that the underlying field
	in the header is ignored unless it is zero legs.. */
    if ( combo_msg_x.no_of_legs_u != 0 )
    {
	// grab underlying from 1st leg
	combo_msg_x.underlying_px = combo_msg_x.leg_x [0].series_px->parent_under_px;
    }
    else
    {
	// find if we know about the given underlying
	combo_msg_x.underlying_px = find_under_by_name_s ( click_px,
							    argv[2] );
	if (!combo_msg_x.underlying_px )
	{
	    printf ( " warning - unknown underlying [%s]\n",
			argv[2] );
	}
    }	// see whether to get underlying (for header) from a leg or argv[]


    // what quantity
    combo_msg_x.quantity_u = read_unsigned_limited ( argv[3], UNDEFINED_UINT32 );

    /* what premium - NB no premium is required for interest-only.. 
	but do you have to specify a 0 premium then.. lets find out
	Also, if we allow the flexibility of allowing a 0-leg without a commmodity code
	in the header (to test that the back-end validates correctly), we don't have
	any idea what # of decimals in the premium.. so we have to make a guess */
    if ( combo_msg_x.underlying_px == NULL )
	// use our "default" tradable instrument class for decimals
	combo_msg_x.premium_i = read_decimal_string_int ( argv[4],
				    click_px->our_market_x.traded_instr_class_px->dec_in_premium_u );
    else
	combo_msg_x.premium_i = read_decimal_string_int ( argv[4],
				    combo_msg_x.underlying_px->traded_instr_class_px->dec_in_premium_u );

    // interest-only or limit combo
    // by adding 1, and ordering our search string array, we get
    // legal value used for the interest_order_c field
    temp_i = select_string_option ( 2, &limit_interest_s[0], argv[5] ) + 1;
    switch ( temp_i )
    {
    case COMBO_INTEXEC_LIMIT:
	combo_msg_x.order_category_u = ORDER_CAT_COMBO_LIMIT;
	break;
    case COMBO_INTEXEC_INTEREST:
	combo_msg_x.order_category_u = ORDER_CAT_COMBO_INTEREST;
	break;
    default:
	printf ("warning - illegal value supplied for LIMIT|INTEREST, defaulted to interest\n" );
	combo_msg_x.order_category_u = ORDER_CAT_COMBO_LIMIT;
    }
	
	combo_msg_x.ul_price_method_c = cmd_ul_price_method(argv[6]);
	if (combo_msg_x.ul_price_method_c == -1) {
		//already reported error
		return SUCCESSFUL;
	}
	combo_msg_x.ul_price_high_i = atoi(argv[7]);
	combo_msg_x.ul_price_low_i = atoi(argv[8]);


    // optional fields.. with defaults supplied by us...

    // validity time
    if ( argc >= 10 )
    {
	// customer specified a validity time
	combo_msg_x.validity_time_u = cmd_validity_time ( argv[9], VALIDITY_TIME_FAS_DAY );
    }
    else
	// default to GTC
	combo_msg_x.validity_time_u = cmd_validity_time ( "", VALIDITY_TIME_FAS_DAY );

    // client category
    if ( argc >= 11 )
    {
	combo_msg_x.client_category_u = cmd_client_category ( argv[10], CUST_CATEGORY_CUSTOMER );
    }
    else
	// default to client category
	combo_msg_x.client_category_u = CUST_CATEGORY_CUSTOMER;

    // broker / operator ID string
    if (( argc >= 12 ) && (argv[11]))
	// customer specified the broker ID
	strncpy ( combo_msg_x.broker_oper_id_s, argv[11], (sizeof (combo_msg_x.broker_oper_id_s)-1) );
    else
	// customer did not specify a broker ID - use our own
	strcpy ( combo_msg_x.broker_oper_id_s,
		    click_px->user_code_x.customer_id_s );

    // see if supplied CMTA account
    if (( argc >= 13 ) && (argv[12]))
	// customer specified a value for CMTA member ID
	strncpy ( combo_msg_x.cmta_member_s, argv[12], (sizeof (combo_msg_x.cmta_member_s)-1) );
    else
	// if not specified, no default (most trades are not transferred via CMTA to other OCC firms)
	strcpy ( combo_msg_x.cmta_member_s, "" );

    // see if supplied sub account
    if (( argc >= 14 ) && (argv[13]))
	// customer specified a value for OCC sub account ID
	strncpy ( combo_msg_x.sub_ac_s, argv[13], (sizeof (combo_msg_x.sub_ac_s)-1) );
    else
	/* if non specified, no default (normally only trades from MMs have a sub-account ID,
	    and all MM users have a default sub-account ID in CDB
	*/
	strcpy ( combo_msg_x.sub_ac_s, "" );

    // see if supplied free text - mandatory in central system for 0 legs
    if (( argc >= 15 ) && (argv[14]))
	// customer specified a value for OCC sub account ID
	strncpy ( combo_msg_x.combo_message_s, argv[14], (sizeof (combo_msg_x.combo_message_s)-1) );
    else
	/* if non specified, no default (normally only trades from MMs have a sub-account ID,
	    and all MM users have a default sub-account ID in CDB
	*/
	strcpy ( combo_msg_x.combo_message_s, "" );

    // And finally, see if there is an order xref
    if (( argc >= 16 ) && (argv[15]))
	// customer specified a value for OCC sub account ID
	strncpy(combo_msg_x.userXRef_s, argv[15], (sizeof (combo_msg_x.userXRef_s)-1));
    else
	strcpy ( combo_msg_x.userXRef_s, "" );



    status_i = send_combo ( click_px,
			    &combo_msg_x );
    if ( status_i < SUCCESSFUL)
    {
	// do nothing, error already reported by do_click_tx_rx
    }
    else
    {
	// combo sent OK
	clear_combo_legs ();
	printf ( "combo accepted by central system\n" );
    }

    // wrap up the data to send
    return status_i;
}   // parse_send_combo

// ---------------------------------------------------------------------------

static unsigned send_hit_combo ( click_details_xt *click_px,
				    combo_entry_xt *combo_data_px, 
				    combo_entry_xt *hit_data_px )
/*
Function:   send_hit_combo
Author:	    Neil Cossor
Modified:   20010605
Description:

    There is a combo in the public combobook which we wish to hit.
    The combo must be executable (not interest-only),
    and the price of the combo must fit within the pricing rules
    for the component series in the current BBO,
    but these are all central system validation issues.

    We cannot specify a different price for one combo,
    the leg series, bid/ask or ratios.. these should be packed
    (but with reverse bid/ask) from the original combo we are hitting.

    We can specify a smaller quantity than the original,
    and can specify our open/close attributes per leg,
    and our own CMTA, sub account, give-up, Xref,
    and we have different choices for validity than for a
    standing (executable) combo.

    This uses the MO136 transaction.

Input Parameters

    - pointer to everything we know about main system
    - pointer to data with info about the combo we are hitting
    - pointer to data with info about the combo we are hitting with..
	This does not have fields filled in which we can get from
	the original combo (e.g. the leg series), but
	does include things specific to the hit.
	This includes
	- the quantity of the hit
	- the validity time of the hit
	- the Xref of the hit
	- the open/close attributes of the legs of the hit
*/
{
    // local buffer, containing actual msg we send
    generic_txn_xt	OMmsg_x;

    int32		status_i	     =	 SUCCESSFUL;
    // size of our actual CLICK query/command
    uint32		tx_length_u = sizeof ( OMmsg_x.tx_x.hit_combo_trans_x );
    unsigned		item_index_u;

    // zero everything in our tx buffer
    memset( &OMmsg_x.tx_x.hit_combo_trans_x, 0, tx_length_u );


    pack_OM_header ( &OMmsg_x, tx_length_u, 'M', 'O', 136);
    // save the facility code and fn code
    OMmsg_x.fn_code = LOG_K_C_HIT_COMBO_TRANS;
    OMmsg_x.facility_u = click_px->facility_no_u + 7;	// EP7/ET7

    // pack in the fields - most come from the combo we are hitting

    // number of legs
    OMmsg_x.tx_x.hit_combo_trans_x.items_c = combo_data_px->no_of_legs_u;
    // the series of the legs
    for ( item_index_u = 0; item_index_u < combo_data_px->no_of_legs_u; item_index_u++ )
    {
	// save the series data
	put_click_series ( &OMmsg_x.tx_x.hit_combo_trans_x.item [item_index_u].series,
			    &combo_data_px->leg_x [item_index_u].series_px->ise_series_x );

	// we get the open-close attributes from the hit combo (not from the one being hit)
	// open/close setting for a/c during OCC settlement
	OMmsg_x.tx_x.hit_combo_trans_x.item [item_index_u].open_close_req_c = 
		hit_data_px->leg_x [item_index_u].open_close_u;
    }

    // we need to specify the orderbook #
    // we are not supposed to be able to HIT a 0-leg combo.. but allow for some flexibility
    if ( combo_data_px->no_of_legs_u )
    {
	// at least one leg.. grab OB from first series
	OMmsg_x.tx_x.hit_combo_trans_x.orderbook_c = combo_data_px->leg_x[0].series_px->parent_under_px->orderbook_u;
    }
    else
    {
	// hmm.. we need to pack a commodity code into the header anyway
	OMmsg_x.tx_x.hit_combo_trans_x.orderbook_c = combo_data_px->underlying_px->orderbook_u;
	PUTSHORT ( OMmsg_x.tx_x.hit_combo_trans_x.series.commodity_n,
		    combo_data_px->underlying_px->commodity_code_u );
    }

    // orderID of original combo
    PUTQUAD ( OMmsg_x.tx_x.hit_combo_trans_x.order_number_u,
		combo_data_px->orderID_u );
    // put in the hit quantity - should be less than original combo qty
    PUTLONG ( OMmsg_x.tx_x.hit_combo_trans_x.quantity_u,
		hit_data_px->quantity_u );
    // validity time - must be one of the immediate flavours
    PUTSHORT ( OMmsg_x.tx_x.hit_combo_trans_x.time_validity_n,
		hit_data_px->validity_time_u );
    // EAM/FIRM/FARMM/CustDisc/BrokerDealer
    OMmsg_x.tx_x.hit_combo_trans_x.client_category_c = hit_data_px->client_category_u;

    // put in the two parts of the client ID, which are actually the CMTA a/c and sub a/c IDs
    tx_click_string ( &OMmsg_x.tx_x.hit_combo_trans_x.ex_client_s [0],
			&hit_data_px->cmta_member_s,
			MAX_CMTA_MEMBER );
    tx_click_string ( &OMmsg_x.tx_x.hit_combo_trans_x.ex_client_s [MAX_CMTA_MEMBER],
			&hit_data_px->sub_ac_s,
			MAX_SUB_ACCOUNT_ID );
    // need to set the broker_id_s
    tx_click_string ( &OMmsg_x.tx_x.hit_combo_trans_x.broker_id_s,
			&hit_data_px->broker_oper_id_s,
			sizeof (OMmsg_x.tx_x.hit_combo_trans_x.broker_id_s));
    // and Xref field
    tx_click_string ( &OMmsg_x.tx_x.hit_combo_trans_x.free_text_s[0],
		     hit_data_px->userXRef_s, 
		     sizeof (OMmsg_x.tx_x.hit_combo_trans_x.free_text_s ) );
    // need code for give-up ????

    // extra fields for hit.. whether to step to BBO, or tick an odd contract
    OMmsg_x.tx_x.hit_combo_trans_x.step_up_to_bbo_c = hit_data_px->step_to_bbo_u;
    OMmsg_x.tx_x.hit_combo_trans_x.step_up_odd_tick_c = hit_data_px->step_odd_tick_u;



    status_i = do_click_tx_rx ( click_px,
				    &OMmsg_x,
				    tx_length_u );
    if ( status_i < SUCCESSFUL)
    {
	// do nothing, error already reported by do_click_tx_rx
    }
    else
    {
	// need to something with various positive status values
    }

    return (status_i);
}   // send_hit_combo

// ---------------------------------------------------------------------------

int32 parse_send_hit ( click_details_xt * click_px,
			unsigned argc,
			char *argv[] )
/*
Function:   parse_send_hit
Author:	    Neil Cossor
Modified:   20010614
Description:

    User is specifying details to hit a standing combo.
    To do the hit, we have many details from the combo being hit,
    as well as some details for the hitting combo.

Input params:
    - pointer to everything about the click system/market
    - count of args from the commandline/script
    - the actual string args from " / "

    (mandatory) command line arguments are
    argv[0] COMBO
    argv[1] HIT
    argv[2] localcombo#|%O  # in local combobook (maintained by combo bcasts)
			    or %O == orderID of combo we saved (possibly under different logon)
    optional arguments are
    argv[3] quantity	    defaults to whole quantity in combo being hit
    argv[4] validity time   defaults to IOC
    argv[5] client category : customer|firm|farmm etc, defaults to customer
    argv[6] brokerID	    default my userID
    argv[7] cmta	    default ""
    argv[8] sub account	    default ""
    argv[9] cross-reference (gets passed through OCC), default ""
    argv[10] step up to bbo	1=step, 0=no step, default no step
    argv[11] step up odd tick	1=step, 0=no step, default no step
    argv[12].... open/close	for leg 1.. default 0 == default o/c
*/
{
    unsigned status_i = SUCCESSFUL;
    // queueable item iwth pointer to temp local node, into which we pack hit details, and use for send
    gen_buf_node_xt *hit_node_px,
    // and one for pointer to existing combobook item we are hitting
		    *combo_node_px;
    // and pointer to actual data component of the hit combo
    combo_entry_xt *hit_data_px,
    // pointer to the actual data of the combo being hit
		    *combo_data_px;
    // local combobook reference # we are told to look for
    unsigned	    local_ref_no;
    unsigned	    item_index_u;


    if ( !click_px->logged_on_i )
    {
	prompt_logged_on ( "Combo,Hit" );
	return NOT_LOGGED_ON;
    }

    // have we got enough arguments to do a hit
    if ( argc < 3 )
    {
	// no - report error, and return
	printf ( " COMBO,HIT,<loc.combo |%%O>,<qty>[,<validity>[,<client cat>[,<brokerID>\n"
	         "  [,<cmta>[,<sub a/c>[,<Xref>[,<bbo 1|2>[,<odd tick 1|2>[,<o/c leg1>[...]]]]]]]]]]\n" );
	return SUCCESSFUL;
    }

    /* we must be sure we can find the combo order specified in argv[2]
	This can be a local combobook reference #
	OR
	it can be the symbol %O, which will use the orderID of the combo
	we last saved with a COMBO,SAVE command. That may have been done
	while logged on as a different user/member
    */
    if ( strcmp ( argv[2], "%O") == 0 )
	// we are using the orderID of the "saved combo"
	combo_node_px = find_combo_by_orderID ( click_px,
						click_px->saved_combo_x.orderID_u );
    else
    {
	// we should have a local combobook # in the argv[2]
	local_ref_no = read_unsigned_limited ( argv[2], UNDEFINED_UINT16 );
	combo_node_px = find_combo_by_combobook_no ( click_px,
							local_ref_no );
    }

    // did we find the combo specified that we wish to hit ?
    if ( combo_node_px == NULL )
    {
	printf ( "Error - combo specified with %s not found.. unable to hit\n",
		    argv[2] );
	return INVALID_ARGUMENTS;
    }
    else
	combo_data_px = (combo_entry_xt *)combo_node_px->msg_p;

    // we now know that we have found the combo being hit.. the rest can be defaulted

    // create temp local combo data node, into which we pack stuff from argv[]
    hit_node_px = create_combo_entry ( 0 );
    hit_data_px = (combo_entry_xt *)hit_node_px->msg_p;

    // quantity supplied ?
    if (( argc >= 4 ) && (argv[3]))
	hit_data_px->quantity_u = read_unsigned_limited ( argv[3], UNDEFINED_UINT32 );
    else
	// no value supplied, default to # in combo being hit
	hit_data_px->quantity_u = combo_data_px->quantity_u;

    // validity time specified ?
    if (( argc >= 5) && (argv[4]))
	hit_data_px->validity_time_u = cmd_validity_time ( argv[4], VALIDITY_TIME_IOC );
    else
	hit_data_px->validity_time_u = VALIDITY_TIME_IOC;

    // client category
    if (( argc >= 6 ) && (argv[5]))
    {
	hit_data_px->client_category_u = cmd_client_category ( argv[5], CUST_CATEGORY_CUSTOMER );
    }
    else
	// default to client category
	hit_data_px->client_category_u = CUST_CATEGORY_CUSTOMER;

    // broker / operator ID string.. rely on C having short-circuit evaluation
    if (( argc >= 7 ) && (argv[6]))
	// customer specified the broker ID
	strncpy ( hit_data_px->broker_oper_id_s, argv[6], (sizeof (hit_data_px->broker_oper_id_s)-1) );
    else
	// customer did not specify a broker ID - use our own
	strcpy ( hit_data_px->broker_oper_id_s,
		    click_px->user_code_x.customer_id_s );

    // see if supplied CMTA account
    if (( argc >= 8 ) && (argv[7]))
	// customer specified a value for CMTA member ID
	strncpy ( hit_data_px->cmta_member_s, argv[7], (sizeof (hit_data_px->cmta_member_s)-1) );
    else
	// if not specified, no default (most trades are not transferred via CMTA to other OCC firms)
	strcpy ( hit_data_px->cmta_member_s, "" );

    // see if supplied sub account
    if (( argc >= 9 ) && (argv[8]))
	// customer specified a value for OCC sub account ID
	strncpy ( hit_data_px->sub_ac_s, argv[8], (sizeof (hit_data_px->sub_ac_s)-1) );
    else
	/* if non specified, no default (normally only trades from MMs have a sub-account ID,
	    and all MM users have a default sub-account ID in CDB
	*/
	strcpy ( hit_data_px->sub_ac_s, "" );

    // And finally, see if there is an order xref
    if (( argc >= 10 ) && (argv[9]))
	// customer specified a value for OCC sub account ID
	strncpy(hit_data_px->userXRef_s, argv[9], (sizeof (hit_data_px->userXRef_s)-1));
    else
	strcpy ( hit_data_px->userXRef_s, "" );

    // was step-up-to-bbo specified
    if ( argc >= 11 )
	// customer specified a value for step-up
	hit_data_px->step_to_bbo_u = read_unsigned_limited ( argv[10], UNDEFINED_UINT8 );
    else
	// set default - no step-up
	hit_data_px->step_to_bbo_u = OM_NO;

    // was step-up-odd-tick specified
    if ( argc >= 12 )
	// customer specified a value for step-up
	hit_data_px->step_odd_tick_u = read_unsigned_limited ( argv[11], UNDEFINED_UINT8 );
    else
	// set detault - no tick odd contract
	hit_data_px->step_odd_tick_u = OM_NO;

    /* now, we may have value specified for open-close for the legs.
	If no value is specified, the default in the structure is a 0,
	which is the default value we wish anyway, so we don't have to do anything
    */
    for ( item_index_u = 0; item_index_u < hit_data_px->no_of_legs_u; item_index_u++ )
	if (argc >= ( 13 + item_index_u) )
	    hit_data_px->leg_x [item_index_u].open_close_u = 
		read_unsigned_limited ( argv[12 + item_index_u], UNDEFINED_UINT8 );

    status_i = send_hit_combo ( click_px,
				combo_data_px,
				hit_data_px );

    // don't have to do anything with status - errors already reported

    // release the memory allocated to the hit (it isn't in a queue or anything)
    free_gen_buf_node ( hit_node_px );

    return status_i;
}   // parse_send_hit

// ---------------------------------------------------------------------------

static unsigned send_amend_combo ( click_details_xt *click_px,
				    combo_entry_xt *combo_data_px, 
				    combo_entry_xt *amend_data_px )
/*
Function:   send_amend_combo
Author:	    Neil Cossor
Modified:   20010627
Description:

    There is a combo in the public combobook which we wish to amend.
    We can change things like
    - validity time (with some restrictions)
    - from interest-only to limit (must specify price for this)
    - quantity.. maybe, perhaps reduce only ?
    - open/close attributes of the legs
    - CMTA
    - sub account
    - give-up
    - Xref

    We cannot
    - change the series in the legs
    - change the ratios of the legs
    - change the premium of the combo
    I guess that this is to avoid someone hitting the combo,
    and getting something that they didn't expect.

    This uses the MO133 transaction.

Input Parameters

    - pointer to everything we know about main system
    - pointer to data with info about the combo we are amending
    - pointer to data with info about the combo values we would like.
	We don't just modify our view of the original combo, as the amend
	might fail, and that would corrupt our info on the original combo.
*/
{
    // local buffer, containing actual msg we send
    generic_txn_xt	OMmsg_x;

    int32		status_i	     =	 SUCCESSFUL;
    // size of our actual CLICK query/command
    uint32		tx_length_u = sizeof ( OMmsg_x.tx_x.alter_combo_trans_x );
    unsigned		item_index_u;

    // zero everything in our tx buffer
    memset( &OMmsg_x.tx_x.alter_combo_trans_x, 0, tx_length_u );


    pack_OM_header ( &OMmsg_x, tx_length_u, 'M', 'O', 133);
    // save the facility code and fn code
    OMmsg_x.fn_code = LOG_K_C_ALTER_COMBO_TRANS;
    OMmsg_x.facility_u = click_px->facility_no_u + 7;	// EP7/ET7

    // pack in the fields - most come from the combo we are amending

    // number of legs
    OMmsg_x.tx_x.alter_combo_trans_x.items_c = combo_data_px->no_of_legs_u;
    // the series of the legs
    for ( item_index_u = 0; item_index_u < combo_data_px->no_of_legs_u; item_index_u++ )
    {
	// save the series data
	put_click_series ( &OMmsg_x.tx_x.alter_combo_trans_x.item [item_index_u].series,
			    &combo_data_px->leg_x [item_index_u].series_px->ise_series_x );

	// we get the open-close attributes from the amend combo (not from the original)
	// open/close setting for a/c during OCC settlement
	OMmsg_x.tx_x.alter_combo_trans_x.item [item_index_u].open_close_req_c = 
		amend_data_px->leg_x [item_index_u].open_close_u;
    }

    // we need to specify the orderbook #
    if ( combo_data_px->no_of_legs_u )
    {
	// at least one leg.. grab OB from first series
	OMmsg_x.tx_x.alter_combo_trans_x.orderbook_c = combo_data_px->leg_x[0].series_px->parent_under_px->orderbook_u;
    }
    else
    {
	// hmm.. we need to pack a commodity code into the header anyway
	OMmsg_x.tx_x.alter_combo_trans_x.orderbook_c = combo_data_px->underlying_px->orderbook_u;
	PUTSHORT ( OMmsg_x.tx_x.alter_combo_trans_x.series.commodity_n,
		    combo_data_px->underlying_px->commodity_code_u );
    }
	
	PUTLONG( OMmsg_x.tx_x.alter_combo_trans_x.ul_price_high_i, combo_data_px->ul_price_high_i);
	PUTLONG( OMmsg_x.tx_x.alter_combo_trans_x.ul_price_low_i, combo_data_px->ul_price_low_i);
	OMmsg_x.tx_x.alter_combo_trans_x.ul_price_method_c = combo_data_px->ul_price_method_c;
    // orderID of original combo
    PUTQUAD ( OMmsg_x.tx_x.alter_combo_trans_x.order_number_u,
		combo_data_px->orderID_u );
    // put in the premium - are we allowed to change this in an amend ?
    // NB if interest -> limit combo, then this is where premium is 1st taken
    PUTLONG ( OMmsg_x.tx_x.alter_combo_trans_x.premium_i,
		amend_data_px->premium_i );
    // put in the new quantity - should be less than original combo qty
    PUTLONG ( OMmsg_x.tx_x.alter_combo_trans_x.quantity_u,
		amend_data_px->quantity_u );
    // validity time
    PUTSHORT ( OMmsg_x.tx_x.alter_combo_trans_x.time_validity_n,
		amend_data_px->validity_time_u );
    // limit always set (can't have Market combo), which makes it somewhat redundant
    OMmsg_x.tx_x.alter_combo_trans_x.order_type_c = ORDER_TYPE_LIMIT;
    // Interest-only, or executable. We are allowed to change from interest to limit
    // however, if the value is unchanged, we should send a zero for this field
    // we note this in the amend_data_px, by having an order category of 0
    if ( amend_data_px->order_category_u )
	if ( amend_data_px->order_category_u == ORDER_CAT_COMBO_LIMIT )
	    // if we re changing from interest->limit, premium is now taken
	    // if it used to be limit, then I don't think we are allowed to change it
	    OMmsg_x.tx_x.alter_combo_trans_x.interest_order_c = COMBO_INTEXEC_LIMIT;
	else
	    OMmsg_x.tx_x.alter_combo_trans_x.interest_order_c = COMBO_INTEXEC_INTEREST;

    // EAM/FIRM/FARMM/CustDisc/BrokerDealer
    OMmsg_x.tx_x.alter_combo_trans_x.client_category_c = amend_data_px->client_category_u;

    // put in the two parts of the client ID, which are actually the CMTA a/c and sub a/c IDs
    tx_click_string ( &OMmsg_x.tx_x.alter_combo_trans_x.ex_client_s [0],
			&amend_data_px->cmta_member_s,
			MAX_CMTA_MEMBER );
    tx_click_string ( &OMmsg_x.tx_x.alter_combo_trans_x.ex_client_s [MAX_CMTA_MEMBER],
			&amend_data_px->sub_ac_s,
			MAX_SUB_ACCOUNT_ID );
    // set the broker_id_s
    tx_click_string ( &OMmsg_x.tx_x.alter_combo_trans_x.broker_id_s,
			&amend_data_px->broker_oper_id_s,
			sizeof (OMmsg_x.tx_x.alter_combo_trans_x.broker_id_s));
    // and Xref field
    tx_click_string ( &OMmsg_x.tx_x.alter_combo_trans_x.free_text_s[0],
		     amend_data_px->userXRef_s, 
		     sizeof (OMmsg_x.tx_x.alter_combo_trans_x.free_text_s ) );
    // need code for give-up ????



    status_i = do_click_tx_rx ( click_px,
				    &OMmsg_x,
				    tx_length_u );
    if ( status_i < SUCCESSFUL)
    {
	// do nothing, error already reported by do_click_tx_rx
    }
    else
    {
	// the +ve number is the number of contracts before we amended it
	printf ( "Combo amend in %s, old qty %u changed to %u\n",
		    combo_data_px->underlying_px->under_id_s,
		    status_i,
		    amend_data_px->quantity_u );
    }

    return (status_i);
}   // send_amend_combo

// ---------------------------------------------------------------------------

int32 parse_send_amend_combo ( click_details_xt * click_px,
				unsigned argc,
				char *argv[] )
/*
Function:   parse_send_amend_combo
Author:	    Neil Cossor
Modified:   20010627
Description:

    User is specifying details to amend a standing combo.
    To do the amend, we have many details from the combo being amended,
    as well as some details of what is changing.

Input params:
    - pointer to everything about the click system/market
    - count of args from the commandline/script
    - the actual string args from " / "

    (mandatory) command line arguments are
    argv[0] COMBO
    argv[1] AMEND
    argv[2] localcombo#|%O  # in local combobook (maintained by combo bcasts)
			    or %O == orderID of combo we saved (possibly under different logon)
	argv[3] Underlying Price Method
	argv[4] Underlying Price High
	argv[5] Underlying Price Low

    optional arguments (if not specified, default to old combo values)
    argv[6] premium	    ignored for interest-only combo, I guess
    argv[7] INTEREST|LIMIT  can't change limit -> interest
    argv[8] quantity	    new quantity. ? can you increase this ?
    argv[9] validity time   there are limits on how you can change this
    argv[10] client category
    argv[11] brokerID	    new brokerID
    argv[12] cmta	    new CMTA
    argv[13] sub account	    new sub-account
    argv[14] cross-reference new xref (gets passed through OCC)
    argv[15].... open/close	for leg 1..
*/
{
    unsigned status_i = SUCCESSFUL;
    // queueable item iwth pointer to temp local node, into which we pack amend details, and use for send
    gen_buf_node_xt *amend_node_px,
    // and one for pointer to existing combobook item we are amending
		    *combo_node_px;
    // and pointer to actual data component of the amend combo
    combo_entry_xt *amend_data_px,
    // pointer to the actual data of the combo being amend
		    *combo_data_px;
    // local combobook reference # we are told to look for
    unsigned	    local_ref_no;
    unsigned	    item_index_u;
    int 	    temp_i;


    if ( !click_px->logged_on_i )
    {
	prompt_logged_on ( "Combo,Amend" );
	return NOT_LOGGED_ON;
    }

    // have we got enough arguments to do a amend
    if ( argc < 6 )
    {
	// no - report error, and return
	printf ( " COMBO,AMEND,<loc.combo |%%O>,<Underlying Price Method>,<Underlying Price High>,<Underlying Price Low>[,<premium>[,LIMIT|INTEREST[,<qty>[,<validity>\n"
	         "  [,<client cat>[,<brokerID>[,<cmta>[,<sub a/c>[,<Xref>[,<o/c leg1>[...]]]]]]]]]]]\n" );
	return SUCCESSFUL;
    }

    /* we must be sure we can find the combo order specified in argv[2]
	This can be a local combobook reference #
	OR
	it can be the symbol %O, which will use the orderID of the combo
	we last saved with a COMBO,SAVE command. That may have been done
	while logged on as a different user/member
    */
    if ( strcmp ( argv[2], "%O") == 0 )
	// we are using the orderID of the "saved combo"
	combo_node_px = find_combo_by_orderID ( click_px,
						click_px->saved_combo_x.orderID_u );
    else
    {
	// we should have a local combobook # in the argv[2]
	local_ref_no = read_unsigned_limited ( argv[2], UNDEFINED_UINT16 );
	combo_node_px = find_combo_by_combobook_no ( click_px,
							local_ref_no );
    }

    // did we find the combo specified that we wish to amend ?
    if ( combo_node_px == NULL )
    {
	printf ( "Error - combo specified with %s not found.. unable to amend\n",
		    argv[2] );
	return INVALID_ARGUMENTS;
    }
    else
	combo_data_px = (combo_entry_xt *)combo_node_px->msg_p;

    // warn user if the combo is not ours (our members).. system should reject,
    // and much of our 'defaulted' info will be junk, as it will be hidden
    if (!combo_data_px->is_ours_u)
	printf ("Warning, combo selected for amend does not belong to this member\n" );

    /* we now know that we have found the combo being amended..
	There should be at least one field changed.. but let it thru.. 
	see what happens with no change */

    // create temp local combo data node, into which we pack stuff from argv[]
    amend_node_px = create_combo_entry ( 0 );
    amend_data_px = (combo_entry_xt *)amend_node_px->msg_p;

	amend_data_px->ul_price_method_c = cmd_ul_price_method(argv[3]);
	if (amend_data_px->ul_price_method_c == -1) {
		//already reported error
		return SUCCESSFUL;
	}
	amend_data_px->ul_price_high_i = atoi(argv[4]);
	amend_data_px->ul_price_low_i = atoi(argv[5]);

    /* what premium - NB no premium is required for interest-only.. 
	but do you have to specify a 0 premium then.. lets find out
	Also, if we allow the flexibility of allowing a 0-leg without a commmodity code
	in the header (to test that the back-end validates correctly), we don't have
	any idea what # of decimals in the premium.. so we have to make a guess */
    if ((argc >= 7) && (argv[6]))
    {
	if ( combo_data_px->underlying_px == NULL )
	    // use our "default" tradable instrument class for decimals
	    amend_data_px->premium_i = read_decimal_string_int ( argv[6],
					click_px->our_market_x.traded_instr_class_px->dec_in_premium_u );
	else
	    amend_data_px->premium_i = read_decimal_string_int ( argv[6],
					combo_data_px->underlying_px->traded_instr_class_px->dec_in_premium_u );
    }
    else
	// no new value specified, use old premium
	amend_data_px->premium_i = combo_data_px->premium_i;

    // interest-only or limit combo
    // by adding 1, and ordering our search string array, we get
    // legal value used for the interest_order_c field
    if ((argc >= 8) && (argv[7]))
    {
	temp_i = select_string_option ( 2, &limit_interest_s[0], argv[7] ) + 1;
	switch ( temp_i )
	{
	case COMBO_INTEXEC_LIMIT:
	    amend_data_px->order_category_u = ORDER_CAT_COMBO_LIMIT;
	    break;
	case COMBO_INTEXEC_INTEREST:
	    amend_data_px->order_category_u = ORDER_CAT_COMBO_INTEREST;
	    break;
	default:
	    printf ("warning - illegal value supplied for LIMIT|INTEREST, defaulted to original value\n" );
	    amend_data_px->order_category_u = combo_data_px->order_category_u;
	}
    }
    else
	// no argument supplied - unchanged from original, tag this with 0 order-category
	// NB this is default in the amend_data_px, so don't really have to do anything
	amend_data_px->order_category_u = 0;

    // quantity supplied ?
    if (( argc >= 9 ) && (argv[8]))
	amend_data_px->quantity_u = read_unsigned_limited ( argv[8], UNDEFINED_UINT32 );

    // validity time specified ?
    if (( argc >= 10) && (argv[9]))
	amend_data_px->validity_time_u = cmd_validity_time ( argv[9], VALIDITY_TIME_IOC );

    // client category
    if (( argc >= 11 ) && (argv[10]))
    {
	amend_data_px->client_category_u = cmd_client_category ( argv[10], CUST_CATEGORY_CUSTOMER );
    }

    // broker / operator ID string.. rely on C having short-circuit evaluation
    if (( argc >= 12 ) && (argv[11]))
	// customer specified the broker ID
	strncpy ( amend_data_px->broker_oper_id_s, argv[11], (sizeof (amend_data_px->broker_oper_id_s)-1) );

    // see if supplied CMTA account
    if (( argc >= 13 ) && (argv[12]))
	// customer specified a value for CMTA member ID
	strncpy ( amend_data_px->cmta_member_s, argv[12], (sizeof (amend_data_px->cmta_member_s)-1) );

    // see if supplied sub account
    if (( argc >= 12 ) && (argv[13]))
	// customer specified a value for OCC sub account ID
	strncpy ( amend_data_px->sub_ac_s, argv[13], (sizeof (amend_data_px->sub_ac_s)-1) );

    // And finally, see if there is an order xref
    if (( argc >= 15 ) && (argv[14]))
	// customer specified a value for OCC sub account ID
	strncpy(amend_data_px->userXRef_s, argv[14], (sizeof (amend_data_px->userXRef_s)-1));

    /* now, we may have value specified for open-close for the legs.
	If no value is specified, use the values in the original combo
    */
    for ( item_index_u = 0; item_index_u < amend_data_px->no_of_legs_u; item_index_u++ )
	// value specified for this leg, or pull from original ?
	if ((argc >= ( 16 + item_index_u)) && (argv[15+item_index_u]))
	    amend_data_px->leg_x [item_index_u].open_close_u = 
		read_unsigned_limited ( argv[15 + item_index_u], UNDEFINED_UINT8 );
	else
	    amend_data_px->leg_x [item_index_u].open_close_u =
		combo_data_px->leg_x [item_index_u].open_close_u;

    status_i = send_amend_combo ( click_px,
				combo_data_px,
				amend_data_px );

    // don't have to do anything with status - errors already reported
    // if we succeeded, we should get bcast which updates the details of the combo

    // release the memory allocated to the amend (it isn't in a queue or anything)
    free_gen_buf_node ( amend_node_px );

    return status_i;
}   // parse_send_amend_combo

// ---------------------------------------------------------------------------

int32 add_combo_leg ( click_details_xt * click_px,
			    int argc,
			    char *argv[] )
/*
Function:   add_combo_leg
Author:     Neil Cossor
Modified:   20010607
Description:

    User is specifying the details of one combo leg, preparatory to sending the whole thing.
    The whole shebang is finalised and sent to the main system by a COMBO,SEND command

Input params:
    - pointer to everything about the click system/market
    - count of args from the commandline/script
    - the actual string args from " / "

    (mandatory) arguments are
    argv[0] COMBO
    argv[1] BID|ASK
    argv[2] <series name>

    optional arguments are
    argv[3] ratio, default value 1
    argv[4] open/close, default open
*/
{
    int			status_i = SUCCESSFUL;

    // have we got enough arguments to pack the rest of the header ?
    if ( argc < 3 )
    {
	// no - report error, and return
	printf ( "COMBO,BID|ASK,<series name>[,<ratio>[,<OPEN|CLOSE>]]\n" );
	return INVALID_ARGUMENTS;
    }

    // have we already got too many legs - both at the offical max of 4, 
    // and at the number defined in the OM message
    if ( combo_msg_x.no_of_legs_u >= 10 )
    {
	printf ("already have 10 legs in combo.. can't add any more\n" );
	return INVALID_ARGUMENTS;
    }
    if ( combo_msg_x.no_of_legs_u >= 4 )
    {
	printf ("warning - already have %u legs in combo.. max is 4\n", 
		combo_msg_x.no_of_legs_u );
	// we allow this to continue, to see what the back end does for 5-10...
    }

    // was it bid or ask
    combo_msg_x.leg_x [combo_msg_x.no_of_legs_u ].bid_ask_u = cmd_bid_ask ( click_px, argv[1] );

    // extract the series name
    // find if we know about the given underlying
    combo_msg_x.leg_x [combo_msg_x.no_of_legs_u ].series_px = user_selected_series ( click_px,
										    argv[2] );
    // did we find that series ?
    if (!combo_msg_x.leg_x [combo_msg_x.no_of_legs_u ].series_px )
    {
	printf ( "warning - unknown series [%s]\n", argv[2] );
	return INVALID_ARGUMENTS;
    }

    // OK, have a series.. we can default the rest, so we know we can definitely add this leg
    // were we told a multiplier ?
    if ( argc >= 4 )
    {
	combo_msg_x.leg_x [ combo_msg_x.no_of_legs_u ].multiplier_u = read_unsigned_limited ( argv[3], UNDEFINED_UINT16 );
	// this might have defaulted, if we had a open/close supplied, but no value for argv[3]
	if (combo_msg_x.leg_x [ combo_msg_x.no_of_legs_u ].multiplier_u == 0)
	    combo_msg_x.leg_x [ combo_msg_x.no_of_legs_u ].multiplier_u = 1;
    }	
    else
	combo_msg_x.leg_x [ combo_msg_x.no_of_legs_u ].multiplier_u = 1;

    // were we told an open/close choice for this leg
    if ( argc >= 5 )
	combo_msg_x.leg_x [ combo_msg_x.no_of_legs_u ].open_close_u = read_unsigned_limited ( argv[4], UNDEFINED_UINT8 );
    else
	combo_msg_x.leg_x [ combo_msg_x.no_of_legs_u ].open_close_u = OPEN_CLOSE_REQUEST_DEFAULT;

    // note that we added one more leg.. do this at the end, so our leg index works till now
    combo_msg_x.no_of_legs_u++;

    return (status_i);
}   // add_combo_leg

// -----------------------------------------------------------------------

void snapshot_save_last_combo ( click_details_xt *click_px )
/*
Function:   snapshot_save_last_combo
Author:     Neil Cossor
Modified:   20010618
Description:

    The user has asked us to snapshot the details from the last combo,
    into the 'saved' last combo. The fields in the saved last order
    can then be referenced by using the %O string.

    This could be done automagically.. but while slightly more
    tedious (at the scripting level), this mechanism gives us more flexibility,
    for example, allowing us to amend an older order after doing some
    other order-related things... or logging out and coming back in under
    another user
*/
{
    unsigned item_index_u;

    // save the last order as the 'saved' order, for reference by %O
    memcpy ( &click_px->saved_combo_x,
		&click_px->last_combo_x,
		sizeof ( click_px->last_combo_x ));

    if ( click_px->saved_combo_x.underlying_px == NULL )
    {
	printf ("Warning, combo saved has no underlying\n");
    }
    else
    {
	// underlying is set up OK

	// stash the underlying name, to make sure we use right series across logons
	strcpy ( click_px->saved_combo_underlying_name_s,
		    click_px->last_combo_x.underlying_px->under_id_s );
	// save any legs we have
	for ( item_index_u = 0;
		item_index_u < click_px->last_combo_x.no_of_legs_u;
		item_index_u++ )
	{
	    strcpy ( click_px->saved_combo_series_name_s [item_index_u],
			click_px->last_combo_x.leg_x[item_index_u].series_px->ise_name_s );
	}

	printf ("Saved :");
	display_one_combo ( &click_px->saved_combo_x );
    }
}   // snapshot_save_last_combo

// ---------------------------------------------------------------------------

static void prompt_delete_combo_command ()
/*
    User tried to Delete orders with screwy parameters.
    Prompt them with legal options
*/
{
    printf ("usage :-\n"
	    "COMBO,Delete,L[ocal],<#>               one combo, using local combo number\n"
	    "COMBO,Delete,U[nderlying],<undername>  ALL combos for one underlying\n"
	    "COMBO,Delete,*                         ALL combos for ALL underlying\n"
	    "COMBO,Delete,%%O                        delete last saved combo (COMBO,SAVE command)\n\n"
	    );
}

// -----------------------------------------------------------------------

static int send_delete_combo (click_details_xt * click_px,
				ise_underlying_xt *under_data_px,
				quad_word orderID_u,
				uint16 order_category_u,
				uint8 orderbook_u,
				customer_id_st  *customer_id_ps,
				user_id_st *user_id_ps
)
/*

Function : send_delete_combo
Author	 : Neil Cossor
Modified : 20010618
Description :

    This functions send a delete combo to the marketplace.

    It packs the delete message with the supplied series qualifier
    which can be for
    - one underlying (country, market, and commodity code)
    - one market (country and market)  With only one market, I do this as all series
    - all series (wildcard everything)
    - one combo - specify everything (no wildcarding)

    For combos, the bid/ask field in the header is ignored

    It packs the OrderID with the supplied value.
    This should be 0 unless a specific combo is being deleted.

    It packs the Orderbook with the supplied value

    It always specifies the "whose" field as the current member, and the user
    as passed in as a parameter (if supplied). The user may be NULL, in which case
    (hopefully) we should be doing a wildcard, and the OrderID should be 0.

    It specifies the Order Category as specified. This should
    be a bitmask of LimitCombo and InterestCombo (i.e. one or both bits on)

    Maintenance of the local combobook is not done in this routine.
    Actually, it is likely that the calling routings ignore that too, 
    and rely on the combo broadcasts.

    We default to using MO104 (specific combo, bid-ask, underyling etc).
    If OrderID == 0, are not specifying one underyling,
    or 'user' is not specified, or specified a different user(from this session),
    then we use MO105.

Input Params:

    Pointer to standard click common
    Pointer to underlying we are to delete in. May be NULL.
    OrderID to delete.. may be zero for wildcard delete.
    Orderbook # - must be supplied for specific order, or multiple orders in one series
	or one underlying... not sure about generic (all underlying) delete
    Customer ID (member id). This is only allowed to be your own.. however this is provided
	as a parameter for use by ISE QA for checking Central System validation
    User ID. Put into 'whose' as supplied, may be NULL.

To do:

*/
{
    // our msg we send up to CLICK to delete the order
    generic_txn_xt	OMmsg_x;

    int32		status_i;
    // flag is we are using MO104. False means MO105.
    int32		mo104_b = TRUE;

    // size of our actual CLICK query/command
    uint32		tx_length_u = sizeof ( delete_trans_mo104_mo105_t );

    /* set up msg to have ALL wildcard i.e. 0 in all fields in the selection qualifier
	actually - zero ALL fields in the message to start with
	This implies
	- both bids and asks
	- all instrument groups
	- no specific order #
	- all order categories
    */
    memset( &OMmsg_x.tx_x.delete_trans_x, 0, tx_length_u );

    // pack in the caller supplied fields

    // the header series.. one underlying, or more than one if NULL
    if (under_data_px)
	PUTSHORT ( OMmsg_x.tx_x.delete_trans_x.series.commodity_n,
		    under_data_px->commodity_code_u );
    else
	// commodity code wildcard
	mo104_b = FALSE;

    // was an orderID specified
    if ( orderID_u.quad_word[0] == '\0' )
	mo104_b = FALSE;
    else
	// put in the supplied order ID
	PUTQUAD ( OMmsg_x.tx_x.delete_trans_x.order_number_u,
		    orderID_u );

    // bid ask is ignored for combo deletes.. leave as 0 from memset above

    // specify supplied orderbook_c
    OMmsg_x.tx_x.delete_trans_x.orderbook_c = orderbook_u;

    // fixed country ID for now, from the info retrieved for this user (at logon)
    tx_click_string ( OMmsg_x.tx_x.delete_trans_x.whose.trading_code.country_id_s,
			&click_px->user_code_x.country_id_s,
			sizeof (OMmsg_x.tx_x.delete_trans_x.whose.trading_code.country_id_s));

    // use whatever customer ID the caller specified
    tx_click_string ( OMmsg_x.tx_x.delete_trans_x.whose.trading_code.ex_customer_s,
			customer_id_ps,
			sizeof (OMmsg_x.tx_x.delete_trans_x.whose.trading_code.ex_customer_s));

    // do we have a legal value for order category specified.. 
    // must be one or both bits of the combos, and no others
    // mask out bits we don't care about for combos
    order_category_u = order_category_u & ( ORDER_CAT_COMBO_LIMIT | ORDER_CAT_COMBO_INTEREST );
    if ( order_category_u == 0 )
    {
	// illegal order category.. must include one or more of these
	printf ("Error - call to send_delete_combo did not specify either of Limit/Interest combos\n");
	return INVALID_ARGUMENTS;
    }

    // if we are wildcarding which combo type, then must use mo105
    if ((order_category_u != ORDER_CAT_COMBO_LIMIT) || (order_category_u != ORDER_CAT_COMBO_INTEREST))
	// as we masked out all other bits, and rejected the neither case
	// if it is not exactly one, then it is both i.e. wildcard
	mo104_b = FALSE;

    PUTSHORT ( OMmsg_x.tx_x.delete_trans_x.order_category_n,
		order_category_u );

    // use whatever user ID the caller specified.. if NULL, or actual string
    if (( (char *)user_id_ps == NULL ) || ( strlen ( (char *)user_id_ps ) == 0 ))
    {
	// no user ID specified, provide NULLs in the field
	memset ( &OMmsg_x.tx_x.delete_trans_x.whose.trading_code.user_id_s,
		    0,
		    sizeof (OMmsg_x.tx_x.delete_trans_x.whose.trading_code.user_id_s));
	mo104_b = FALSE;
    }
    else
    {
	// user supplied a user ID string - use it
	tx_click_string ( OMmsg_x.tx_x.delete_trans_x.whose.trading_code.user_id_s,
			    user_id_ps,
			    sizeof (OMmsg_x.tx_x.delete_trans_x.whose.trading_code.user_id_s));

	// if it is not our user, then do MO105
	if ( strcmp ( (char *)user_id_ps, (char *)&click_px->user_code_x.user_id_s ) == 0 )
	    // user ID is me i.e. who I am signed on as
	    mo104_b = FALSE;
    }	// test if we have a user ID provided

    if (mo104_b == TRUE)
	pack_OM_header ( &OMmsg_x, tx_length_u, 'M', 'O', 104 );
    else
	// default to using 105 delete, unless we are specifying my user ID
	pack_OM_header ( &OMmsg_x, tx_length_u, 'M', 'O', 105 );


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

    return status_i;
}   // send_delete_combo

// ---------------------------------------------------------------------------

int32 parse_delete_combo ( click_details_xt *click_px,
			    unsigned argc,
			    char *argv[] )
/*
Function:   parse_delete_combo
Author:	    Neil Cossor
Modified:   20010619
Description:

    User is deleting one or more standing combos.

Input params:
    - pointer to everything about the click system/market
    - count of args from the commandline/script
    - the actual string args from " / "

    (mandatory) command line arguments are
    argv[0] COMBO
    argv[1] DELETE
    argv[2] Local|Underlying|*|%O
    argv[3] localcombo#		    # in local combobook (maintained by combo bcasts)
	    |<underlying name>	    .. uh.. guess
	    |"*"		    all combos in all underlying
	    |%O			    orderID of combo we saved (possibly under different logon)
    optional arguments are
    argv[4] LIMIT|INTEREST|BOTH	    which type of combos, default BOTH (irrelevant unless wildcard)
    argv[5] MEMBER|USER		    whether we are deleting for user or whole member (irrelevant unless wildcard)

    i.e. flavours include
    COMBO,Delete,L[ocal],<local order #>	one order, using local order number
    COMBO,Delete,U[nderlying],<undername>	ALL local orders for one underlying
    COMBO,Delete,*				ALL local orders for ALL underlying
    COMBO,Delete,%O				the order which we last snapshot-saved

    Note that the order deletion flavour of by-series is irrelevant for combos.
    I have found little user for delete by Xref, so have not implemented it for combos.

Output Params:

    Central combobook should be updated. Local combobook will be updated by bcasts.
*/
{
    unsigned status_i = SUCCESSFUL;
    // queueable item with pointer to temp local node, with data about one combo
    gen_buf_node_xt *combo_node_px;
    // and pointer to actual data component of the combo
    combo_entry_xt  *combo_data_px;
    // pointer to info about nominated underlying (if nominated)
    ise_underlying_xt *under_data_px;
    int 		choice_i;	    // enumeration of which delete subcommand was selected
    static char *delete_subcommands_s [] =
    {
	"LOCAL",
	"UNDERLYING",
	"*",
	"%O"
    };
    // enumerated list which corresponds to the above list of choices
    enum delete_subcommands_et { delete_subcommand_local_e,
				    delete_subcommand_underlying_e,
				    delete_subcommand_wildcard_e,
				    delete_subcommand_last_saved_e };
    // local combobook reference # we are told to look for
    unsigned	    local_ref_no;
    uint8	    item_index_u;



    if ( !click_px->logged_on_i )
    {
	prompt_logged_on ( "Combo,Delete" );
	return NOT_LOGGED_ON;
    }

    // have we got enough arguments to do a delete
    if ( argc < 3 )
    {
	// no - report error, and return
	prompt_delete_combo_command();
	return SUCCESSFUL;
    }


    // which flavour of delete ?
    choice_i = select_string_option ( 4, &delete_subcommands_s [0], argv[2] );
    switch (choice_i)
    {
    case delete_subcommand_local_e:
	// delete by local reference #
	local_ref_no = read_unsigned_limited ( argv[3], UNDEFINED_UINT16 );
	combo_node_px = find_combo_by_combobook_no ( click_px,
							local_ref_no );

	// did we find that combo ?
	if (!combo_node_px)
	{
	    printf ("Unable to find combo %s\n", argv[3] );
	    return INVALID_ARGUMENTS;
	}

	combo_data_px = (combo_entry_xt *)combo_node_px->msg_p;

	// is it our (member's) combo ? Can't delete otherwise ?
	if ( combo_data_px->is_ours_u )
	    status_i = send_delete_combo ( click_px,
					    combo_data_px->underlying_px,
					    combo_data_px->orderID_u,
					    combo_data_px->order_category_u,
					    combo_data_px->underlying_px->orderbook_u,
					    &combo_data_px->user_code_x.customer_id_s,
					    &combo_data_px->user_code_x.user_id_s );
	    // if delete works, combobook will be updated by bcast
	else
	    printf ("Unable to delete that combo - not from our member\n");
			    
	break;

    case delete_subcommand_underlying_e:
	// delete by underlying

	/* They have entered a underlying for which they want
	    all orders to be deleted.
	 */
	under_data_px = find_under_by_name_s ( click_px,
						argv[3] );
	if (!under_data_px )
	{
	    printf ( " warning - unknown underlying [%s]\n",
			argv[3] );
	    return INVALID_ARGUMENTS;
	}

	status_i = send_delete_combo ( click_px,
					under_data_px,
					click_px->null_orderID_u,
					// for now assume we are removing all combo types
					ORDER_CAT_COMBO_LIMIT | ORDER_CAT_COMBO_INTEREST,
					under_data_px->orderbook_u,
					// for now, assume combos by my user (not my member)
					&click_px->user_code_x.customer_id_s,
					&click_px->user_code_x.user_id_s );
	// if delete works, combobook will be updated by bcast

	break;

    case delete_subcommand_wildcard_e:
	// delete all combos in all underlying

	printf ("Deleting all combos\n\n");

	for (item_index_u = 1;
		item_index_u <= click_px->our_market_x.max_orderbook_u;
		item_index_u++ )
	{
	    // must we loop around all known orderbooks ??
	    status_i = send_delete_combo ( click_px,
					    NULL,
					    click_px->null_orderID_u,   // zero for order ID
					    // for now assume we are removing all combo types
					    ORDER_CAT_COMBO_LIMIT | ORDER_CAT_COMBO_INTEREST,
					    // the orderbook we are up to
					    item_index_u,
					    // for now, assume combos by my user (not my member)
					    &click_px->user_code_x.customer_id_s,
					    &click_px->user_code_x.user_id_s
					    );
	    if (status_i < SUCCESSFUL )
		// error doing one of the deletes, bail out
		break;
	    else if (status_i == SUCCESSFUL)
		printf ("Warning, deleting orders in OB server %u - no contracts deleted\n",
			    item_index_u );
	}

	break;

    case delete_subcommand_last_saved_e:
	// delete the last combo we snapshot-saved.. use the orderID of that
	// NB this might not have been sent by this user/member

	// do we have a saved combo ? To have saved one, must have underyling
	if ( click_px->saved_combo_x.underlying_px == NULL )
	{
	    printf ("We have no saved combo\n" );
	    return INVALID_ARGUMENTS;
	}

	status_i = send_delete_combo ( click_px,
					click_px->saved_combo_x.underlying_px,
					click_px->saved_combo_x.orderID_u,
					click_px->saved_combo_x.order_category_u,
					click_px->saved_combo_x.underlying_px->orderbook_u,
					&click_px->saved_combo_x.user_code_x.customer_id_s,
					&click_px->saved_combo_x.user_code_x.user_id_s );
	// if delete works, combobook will be updated by bcast
			    
	break;

    default:
	prompt_delete_combo_command();
    }



    return status_i;
}   // parse_combo_delete
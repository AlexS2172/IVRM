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


Title : order.c
Author: Neil Cossor

Function :

    This file provides basic order handling to the ISESim.exe program.
    This includes sending a simple order, amending an order, and
    various flavours of deleting orders.
    
Revision history :-

yymmdd	vers'n	author	    notes
------	------	------	    -----
9905xx	v0.00	Neil Cossor initial version
9907xx			    added the rest of the orders (Block, Facilitation, CAB) etc
990907  v0.01		    added code for specifying CMTA and sub a/c info for all
				order types (and responses)
			    Modified on-screen order deletion messages to be more informative
990909			    Modified delete_orders_by_xref to delete all orders with xref, not first
				plus name change (_orders_ not _order_)
			    Improved deletion reporting
			    Added routine to delete by series
990915	v0.02		    mod for extra param to free_gen_buf_node_q
990930			    mod query_order, query_active_order, query_inactive_order to pass through
				order_category selection criteria, and to return (and report) # of
				orders found
991005	v0.03		    fix bug in delete_order, which used wrong argv[] for local order delete
991102	v0.04		    add default values to some cmd_<> routines, skipping cmd-line fields still gives defaults
			    mod create_order_entry, generate_order, generate_block_order, generate_facilitation_order,
				generate_cab_order to cope with string arguments not being supplied (with following args)
991103			    remove redundant local series in delete_orders_by_series_name
			    mod send_delete_order (and all calls to it) to take customer and user ID params.
				If user ID supplied and IS our user, then use MO104, otherwise MO105.
			    mod query_active_order, query_inactive_order to handle 'all series' correctly,
				and query_orders for parameter change to match
991111	v0.05		    fix bug in generate_facilitation_order - did not call send_facilitation_order
			    fix bug in generate_block_facil_response - looked up wrong argv[] reading series
000105	v0.06		    update query_inactive_order to make use of ma102 now having orderID
000107			    update for broker_percentage / expose_flag in facilitation order
000117			    update prompt_facilitation_command " "
000204	v0.07		    style change from &" to " (Microsoft C is happy, but Solaris C isn't)
				in generate_order, generate_block_order, generate_facilitation_order
000321	v0.08		    fix query_orders, for * (all underlying) to cycle thru all orderbooks.
			    I incorrectly assumed that the orderbook_next in the response, starting
				from OB 1, would cycle thru all orderbooks. It appears that that
				mechanism is only used for data in the same orderbook (make that
				field in the response somewhat redundant), when the data will not
				fit into one response.
000327	v0.09		    fix query_orders, for *. Original logic in query_orders was OK,
				but logic in query_active_order and query_inactive_orders incorrectly
				bailed out when an item count of 0 was in a response. This was
				incorrect when the query was a wildcard on series.
				Once done, this showed that logic for specific series would continue
				querying across other OB servers.. put in new block against that.
				update to use string parser, to select from *|Series|Underlying
000328			    fix amend_order, no longer change orderID after to amend..
				the original orderID is correct (despite amend returning a different orderID)
				Use 0 as default value for validity_time if none supplied (i.e. leave unchanged)
000329			    change generate_order, generate_block_order, generate_facilitation_order,
				generate_block_facil_response, generate_cab_order.
				default for CMTA and sub-account are "". Neither are used frequently,
				and should only be set when a user / script specifies one.
			    Change amend_order. If a value is not supplied, then don't change value
				in the order (both in-memory and in orderbook).
000330			    remove 'by underlying' from query_orders.. this selection is not really
				supported in the OM query, and was done by using a loop.. which would
				generate redundant (overlapping) data in the responses. If the OM
				packing order was rigidly defined, then it would be possible to
				do this by doing a query by series, specifying the first series
				that OM packs.. and stop processing when we hit an order which
				is in a different commodity.
			    Fix specific-series query in query_active_order and query_inactive_order,
				which could roll into other orderbooks, due to common logic with
				all-series query.
000404			    add new routine delete_order_by_orderID,
				and mod delete_last_saved_order to use it
000422	v1.00		    update to match OM release 15.7-3 (move of some fields from da203 to da210).
			    display_one_order, delete_orders_by_xref, generate_order, 
			    query_inactive_order, generate_block_order, generate_facilitation_order,
			    generate_block_facil_response
000521	v1.01		    update query_orders to clear the internal orderbook, before a query all
			    update query_active_order to filter out any quotes, and query_orders to show them
			    fix clear_local_orderbook... released the Q header as well as the orders (wrong)
000601			    bugfix generate_facilitation_order, used argv[9] not argv[12] for expose_flag_u
			    also, >= rather than > for seeing if broker % and expose flag were supplied
000606			    bugfix delete_order_by_local_no, which didn't format order price correctly
000628			    bugfix send_facilitation_order.. did not pack broker % or expose flag
			    and %%  bugs in prompt_facilitation_command, prompt_block_facil_response_command
			    bugfix generate_block_order, default validity now IOC (GTC not allowed)
010411			    fix send_delete_order to use mo104, but use mo105 when any of these occur
			    - user not specified, or specified but not me (one who logged in)
			    - bid/ask wildcard specified as wildcard
			    - no orderID specified
			    - 'series' does not nominate a specific (known) series
010526			    update for new 16.5 validity times
			    restructure create_order_entry, and new update_order, to
				allow for better maintenance of local orderbook. A good idea
				anyway, but especially relevant to new BO130 broadcast.
010531			    mod delete_order_by_orderID to not report error if unknown order..
				maybe we hadn't queried yet, or are not subscribing to BO130.
010601			    mod order_entry to include trading code, mod both update_order routines
				to set this correctly, update callers for extra params
			    mod calls to send_delete_order to use actual trading code of known
				order where possible, and use a nominated order category,
				and to specify the order_category (plain, CAB etc)
010607			    fix some internal field names to be sub_acc_ps. When I first coded this
				I incorrectly assumed that the sub-account was tied to the CMTA,
				whereas they are quite independent. The sub-account could be for
				the default OCC clearing account, or for a nominated CMTA
010619			    Add new delete,exact command, to allow deletion with specific series, orderID, bid/ask
010620			    create snapshot_last_order, to save the last (good) order send
				and to note the user/member who sent it. Mod all routines
				which send orders (plain, block etc etc) to call it.
			    mod query_active_order and query_inactive_order to be selective of
				query by user / by member based on flag set (set,member|user)
			    mod delete_orders_by_under_name, delete_orders_by_series_name,
				delete_all_orders to use wildcard for user, if default
				setting is for by-member (SET,MEMBER command).
010702			    mod amend_order to tighten up checks on optional args

To do :-

more (and smarter) stuff to maintain a local orderbook properly...
*/
#include "stdincl.h"

#include "error.h"
#include "series.h"
#include "underlying.h"
#include "utility.h"

#include "order.h"



// -------------------------- internal routines ---------------------------

static void update_order ( click_details_xt *click_px,
			    order_entry_xt *order_data_px,
			    ise_series_info_xt	*seriesEntry_px,
			    quad_word		orderID_u,
			    int32		premium_i,	    // price
			    uint32		quantity_u,
			    int8		bid_ask_u,	    // bid/ask
			    char		userXRef_s[],	    // free text
			    uint16		validity_time_u,    // GTC/ROD/IOC/FOK
			    uint8		order_type_u,	    // limit/market
			    uint8		client_category_u,  // cust/Firm/FARMM
			    uint8		ignore_away_u,	    // 1 == ignore
			    user_id_st		broker_oper_id_s,   // who is the 'owner'
			    uint8		lock_type_u,	    // hidden type
			    uint16		order_cat_u,	    // plain, block, facil etc
			    uint8		expose_flag_u,	    // bitmask of what to show of block / facilitation order
			    uint8		open_close_u,	    // behaviour on OCC a/c during settlement
			    cmta_member_st	cmta_member_s,	    // the CMTA account
			    sub_account_id_st	sub_account_id_s,   // the sub account ID
			    uint8		broker_percentage_u // for facilitation orders
			    )
/*
Function:   update_order
Author:	    Neil Cossor
Modified:   20010526
Description:

    We have info to update an item in our local orderbook with.

    The caller has already found the pointer to the correct order,
    so we just have to update it.

    This is always triggered by this user sending in an order.
    Updates from the main system, including from queries and broadcasts
    may include other users, however those updates are done through
    external_update_order.
*/
{
    order_data_px->series_px = seriesEntry_px;
    order_data_px->bid_ask_u = bid_ask_u;
    memcpy( &(order_data_px->orderID_u), &orderID_u, sizeof(orderID_u) );
    order_data_px->quantity_u = quantity_u;
    order_data_px->premium_i = premium_i;
    strcpy(&order_data_px->userXRef_s[0], &userXRef_s[0]);
    order_data_px->validity_time_u = validity_time_u;
    order_data_px->order_type_u = order_type_u;
    order_data_px->ignore_away_u = ignore_away_u;
    order_data_px->client_category_u = client_category_u;
    order_data_px->broker_percentage_u = broker_percentage_u;
    // NB there may not have been a user string (broker_id) supplied
    if ( broker_oper_id_s != NULL )
	strcpy((char *)&order_data_px->broker_oper_id_s, &broker_oper_id_s[0]);
    order_data_px->lock_type_u = lock_type_u;
    order_data_px->order_category_u = order_cat_u;
    order_data_px->expose_flag_u = expose_flag_u;
    order_data_px->open_close_u = open_close_u;
    // NB there may not have been a CMTA account string supplied
    if ( cmta_member_s != NULL )
	strcpy ((char *)&order_data_px->cmta_member_s, cmta_member_s );
    // NB there may not have been a CMTA subaccount string supplied - normally only for MM orders
    if ( sub_account_id_s != NULL )
	strcpy ((char *)&order_data_px->sub_ac_s, sub_account_id_s );
    // the trading code of the order is always this user
    strcpy ( order_data_px->user_code_x.country_id_s, click_px->user_code_x.country_id_s );
    strcpy ( order_data_px->user_code_x.customer_id_s, click_px->user_code_x.customer_id_s );
    strcpy ( order_data_px->user_code_x.user_id_s, click_px->user_code_x.user_id_s );

}   // update_order

// ---------------------------------------------------------------------------


static gen_buf_node_xt *create_order_entry ( uint16 localNumber_n )
/*
Function : create_order_entry
Author	 : Neil Cossor
Modified : 20000107
Description:

    Creates and initialises an order entry
    The order_entry is NOT added to the chain of known orders
    - this is the responsibility of the caller
*/
{
    // pointer to the Q'able block, which we will contain a ptr to actual order
    gen_buf_node_xt	*order_node_px;
    // point to the actual order data
    order_entry_xt	*order_data_px;

    // allocate space to for the Q'able container, and the actual order data
    order_node_px = new_gen_buf_node ( sizeof ( order_entry_xt ));
    // point to the order data block of what we just got
    order_data_px = (order_entry_xt *)order_node_px->msg_p;

    order_data_px->localNumber_n = localNumber_n;

    return order_node_px;
}   // create_order_entry

// -----------------------------------------------------------------------

void remove_order_from_list ( click_details_xt * click_px,
				gen_buf_node_xt *order_node_px )
/*

Function : remove_order_from_list
Author	 : Neil Cossor
Modified : 9905xx
Description :

    An order has been deleted (at the central system level).
    It is currently in the known_orders_q.

    Remove it from the Q, and update counters etc
*/
{
    // and remove the order from the current order chain
    // this adjusts the Q length
    remove_buff_from_q ( &click_px->our_market_x.known_orders_q,
			    &order_node_px->chain );

    // release the memory allocated to the order
    free_gen_buf_node ( order_node_px );
}

// -----------------------------------------------------------------------

/*
    A bunch of short routines, which can be called to prompt a user
    with what the correct format for a particular command is.
*/
static void prompt_delete_command ()
/*
    User tried to Delete orders with screwy parameters.
    Prompt them with legal options
*/
{
    printf ("usage :-\n"
	    "Delete,E[xact],<seriesname>,<orderID>,<bid/ask> delete one order, using values supplied\n"
	    "Delete,L[ocal],<#>               ONE order, using local order number\n"
	    "Delete,R[eference],<xref>        Local order with a cross-reference\n"
	    "Delete,S[eries],<seriesname>     ALL local orders for one series\n"
	    "Delete,U[nderlying],<undername>  ALL local orders for one underlying\n"
	    "Delete,*                         ALL local orders for ALL stocks\n"
	    "Delete,%%O                        delete last saved order (see SAVE command)\n\n" 
	    );
}

static void prompt_block_command ()
/*
    User tried to send a Block command, with screwy parameters.
    Prompt them with legal options
*/
{
    printf ("Block,Bid/Ask,<seriesname>,<qty>,<premium>,<Xref>[,<validity>[,<client category>\n"
	    "    [,<expose flag>[,<bkr id>[,<open/close flag>[,<CMTA ID>[,<sub a/c>]]]]]]]]\n"
	);
}   // prompt_block_command

static void prompt_bid_ask ( char *bid_ask_ps )
/*
    User tried to send a Bid or Ask command, with screwy parameters.
    Prompt them with legal options
*/
{
    printf ("%-3s,<seriesname>,<qty>,<premium>,<Xref>[,<validity>[,<client cat>[,<type>[,<ignoreaway>\n"
	    "        [,<bkr id>[,<open/close flag>[,<CMTA ID>[,<sub a/c>]]]]]]]\n",
	    bid_ask_ps
	    );
}   // prompt_bid_ask

static void prompt_facilitation_command ()
/*
    User tried to send a Facilitation command, with screwy parameters.
    Prompt them with legal options
*/
{
    printf ("Facilitation,Bid/Ask,<seriesname>,<qty>,<premium>,<Xref>[,<client category>\n"
	    "        [,<bkr id>[,<open/close flag>[,<CMTA ID>[,<sub a/c>[,<broker %%>\n"
	    "        [,<expose flag>]]]]]]\n"
	);
}   // prompt_facilitation_command

static void prompt_block_facil_response_command ( char *block_facil_response_ps )
/*
    User tried to send a Block or Facilitation Response command, with screwy parameters.
    Prompt them with legal options
*/
{
    printf ("%s,Bid|Ask,<seriesname>,<qty>,<premium>,<Xref>[,<orderID>\n"
	    "     [,<bkr id>[,<open/close flag>[,<CMTA ID>[,<sub a/c>\n"
	    "     [,<broker %%>[,<expose flag>]]]]]]\n",
	    block_facil_response_ps
	);
}   // prompt_block_facil_response_command

static void prompt_cab_command ()
/*
    User tried to send a CAB command, with screwy parameters.
    Prompt them with legal options
*/
{
    printf ("CAB,Bid/Ask,<seriesname>,<qty>,<Xref>[,<validity>[,<client category>\n"
	    "     [,<bkr id>[,<open/close flag>[,<CMTA ID>[,<sub a/c>]]]]]]\n"
	);
}   // prompt_cab_command


// ---------------------------------------------------------------------------

void snapshot_last_order ( click_details_xt *click_px,
			    order_entry_xt *order_data_px )
/*
Function:   snapshot_last_order
Author:     Neil Cossor
Modified:   20010620
Description:

    Snapshot order that just successfully sent into last_order_x.
    Called by all fns which generate an order (of any type)
*/
{
    // save this order as new last
    memcpy ( &click_px->last_order_x,
		order_data_px,
		sizeof ( click_px->last_order_x ));
    // we also need to save the user/member of who we are logged on at present
    strcpy ( click_px->last_order_x.user_code_x.country_id_s,
		click_px->user_code_x.customer_id_s );
    strcpy ( click_px->last_order_x.user_code_x.user_id_s,
		click_px->user_code_x.user_id_s );
}   // snapshot_last_order

// ---------------------------------------------------------------------------

void display_one_order ( order_entry_xt * order_data_px )
/*
Function:   display_one_order
Author:     Neil Cossor
Modified:   20000422
Description:

    Display all info about one order.

    Use this common routine, for displaying all orders, for displaying
    an order which has been modified (say partially traded, or amended) etc
*/
{
	quad_conv_t * convOrderNo;

    // the string form of a premium, with appropriate # of decimal places
    series_premium_st	    premium_s;
    // pointer to uint64 as a structure, so we can dump out each half of order ID
//    uint64_xt		    *uint64_pxu;


    format_whole_decimal_s ( (char *)&premium_s,
				order_data_px->premium_i,
				order_data_px->series_px->instrument_class_px->dec_in_premium_u,
				MAX_SERIES_PREMIUM );	// allowed width


    // point structure pointer at the 64bit order ID
//    (uint64 *)uint64_pxu = &(order_data_px->orderID_u);
	convOrderNo = (quad_conv_t *) &(order_data_px->orderID_u.quad_word);

    printf ( "%-15s %-4s %6d  %-16s %6d %10s  %08lX:%08lX  %-5s %-8s %-6s %s %s\n",
		order_category_to_string (order_data_px->order_category_u ),
		bid_ask_to_string(order_data_px->bid_ask_u),
		order_data_px->localNumber_n,
		&order_data_px->series_px->ise_name_s, 
		order_data_px->quantity_u,
		&premium_s,
		convOrderNo->l2,
		convOrderNo->l1,
		validity_time_to_string ( order_data_px->validity_time_u ),
		client_category_to_string ( order_data_px->client_category_u ),
		order_type_to_string ( order_data_px->order_type_u ),
		&order_data_px->userXRef_s,
		open_close_to_string ( order_data_px->open_close_u )
	     );

}   // display_one_order

// ------------------------------------------------------------------------

static gen_buf_node_xt * find_order_by_xref (click_details_xt * click_px,
						char *OrderXReference )
/*
Function : find_order_by_xref
Author	 : Neil Cossor
Modified : 9906xx
Description:

    Finds the order from the known orders, by the
    supplied order cross reference

Input Params:

    pointer to the standard CLICK common block
    the string Order Xref to search for

Output Params:

    If found, a pointer to the (Q'able) block, which in turn contains a pointer
	to the block containing the actual info about the order
    otherwise NULL
*/
{
    int32	    status_i = SUCCESSFUL;
    // pointer to (Q'ed) block, containing pointer to an order
    gen_buf_node_xt *order_node_px;
    // pointer to the actual order data
    order_entry_xt  *order_data_px;

    /*
	Find the FIRST order with the supplied cross reference...

	Simply search the list.. not efficient, but
	considering the (typically) small # of orders... not really a problem
    */
    // point to first entry in chain of known orders - may be empty
    order_node_px = (gen_buf_node_xt *)click_px->our_market_x.known_orders_q.chain.next;
    while (order_node_px)
    {
	// point into the actual order data
	// this is NOT allowed to be NULL
	order_data_px = (order_entry_xt *)order_node_px->msg_p;

	if ( strcmp (order_data_px->userXRef_s, 
			OrderXReference)
		== 0 )
	{
	    // found the match - just drop out of the loop, and return ptr
	    break;
	}
	else
	{
	    // no go, try the next order - which may be NULL
	    order_node_px = (gen_buf_node_xt *)order_node_px->chain.next;
	}
    }	// while (order_node_px)

    // return NULL or the node we found
    return order_node_px;
}   // find_order_by_xref

// ------------------------------------------------------------------------

static gen_buf_node_xt * find_order_by_orderID (click_details_xt * click_px,
						quad_word orderID_u )
/*
Function : find_order_by_orderID
Author	 : Neil Cossor
Modified : 9906xx
Description:

    Finds the order from the known orders, by the
    supplied orderID

Input Params:

    pointer to the standard CLICK common block
    the quadword orderID to search for

Output Params:

    If found, a pointer to the (Q'able) block, which in turn contains a pointer
	to the block containing the actual info about the order
    otherwise NULL
*/
{
    int32	    status_i = SUCCESSFUL;
    // pointer to (Q'ed) block, containing pointer to an order
    gen_buf_node_xt *order_node_px;
    // pointer to the actual order data
    order_entry_xt  *order_data_px;

    /*
	Find the FIRST order with the supplied orderID...

	Simply search the list.. not efficient, but
	considering the (typically) small # of orders... not really a problem
    */
    // point to first entry in chain of known orders - may be empty
    order_node_px = (gen_buf_node_xt *)click_px->our_market_x.known_orders_q.chain.next;
    while (order_node_px)
    {
	// point into the actual order data
	// this is NOT allowed to be NULL
	order_data_px = (order_entry_xt *)order_node_px->msg_p;

	if ( ! memcmp(&(order_data_px->orderID_u), &orderID_u, sizeof(orderID_u)) )
	{
	    // found the match - just drop out of the loop, and return ptr
	    break;
	}
	else
	{
	    // no go, try the next order - which may be NULL, and drop us from loop
	    order_node_px = (gen_buf_node_xt *)order_node_px->chain.next;
	}
    }	// while (order_node_px)

    // return NULL or the node we found
    return order_node_px;
}   // find_order_by_orderID

// -----------------------------------------------------------------------

void external_update_order ( click_details_xt *click_px,
				ise_series_xt	    *ise_series_px,
				quad_word		    OM_orderID_u,
				int32		    OM_premium_i,	// price
				uint32		    OM_quantity_u,
				int8		    bid_ask_u,		// bid/ask
				char		    OM_userXRef_s[],    // free text
				uint16		    OM_validity_time_u, // GTC/ROD/IOC/FOK/FAS/AON
				uint8		    order_type_u,	// limit/market
				uint8		    client_category_u,  // cust/Firm/FARMM
				uint8		    ignore_away_u,	// 1 == ignore
				user_id_st	    broker_oper_id_s,   // who is the 'owner'
				uint8		    lock_type_u,	// hidden type
				uint16		    OM_order_cat_u,	// plain, block, facil etc
				uint8		    expose_flag_u,	// bitmask of what to show of block / facilitation order
				uint8		    open_close_u,	// behaviour on OCC a/c during settlement
				cmta_member_st	    cmta_member_s,	// the CMTA account
				sub_account_id_st   sub_account_id_s,   // the sub account ID
				uint8		    broker_percentage_u,// for facilitation orders
				trading_code_t	    OM_trading_code_x	// which user (and member) this order belongs to
				)
/*
Function:   external_update_order
Author:	    Neil Cossor
Modified:   20010531
Description:

    We have info to update an item in our local orderbook with.

    If we don't know about the order at all (search by orderID)
    then we create an order entry item, and add it into the local orderbook.

    This could be triggered by a MQ101 query,
    a BO130 orderbook broadcast (with an add/modify flavour), 
    or maybe just a locktype change (BO115).

    In any case, we know about the change from an OM-formatted external message,
    rather than from us generating the order (from ISEsim command),
    so the input fields are in OM central-system order.

    This routine is hence very similar to update_order, BUT
    does the necessary byte-swapping.
    
    The conversion of OM string formats to C-style NUL-terminated strings
    is done by the caller, as it is messier keeping track of the field sizes here,
    and the CMTA and sub-account fields are sometimes packed as a
    single 10-char field, and sometimes as 2 5-char fields
*/
{
    // order ID of order we got, in local-endian byte order
    quad_word		search_orderID_u;
    // pointer to (Q'ed) block, containing pointer to an order
    gen_buf_node_xt	*order_node_px;
    // pointer to the actual order data
    order_entry_xt	*order_data_px;
    // pointer to selected series
    ise_series_info_pt	series_data_px = NULL;


    // figure out whether we know about this order (by orderID)
    // extract one order - in correct endian order
    memcpy ( &search_orderID_u, &OM_orderID_u, sizeof(OM_orderID_u) );

    // add it to the local list of known orders, if we don't already know about it
    order_node_px = find_order_by_orderID ( click_px,
					    search_orderID_u );

    // did we know about this order already ?
    if ( !order_node_px )
    {
	// did NOT know about this order... must add it to the list of known

	// create an order_entry block
	order_node_px = create_order_entry ( ++click_px->maxLocalOrderNumber_u );
	    
	// and add it to the Q of known orders - note that most of the contents not set yet
	add_to_q_tail ( &click_px->our_market_x.known_orders_q,
			&order_node_px->chain );

    };  // check if we already knew about this order

    order_data_px = (order_entry_xt *)order_node_px->msg_p;
    // find the series data for this particular are
    series_data_px = find_series_by_ise_series ( click_px,
						    ise_series_px );

    order_data_px->series_px = series_data_px;

    // all of the byte-swapped fields first - note we have already byte swapped orderID above
    order_data_px->orderID_u = search_orderID_u;
    PUTLONG ( order_data_px->quantity_u, OM_quantity_u );
    PUTLONG ( order_data_px->premium_i, OM_premium_i );
    PUTSHORT ( order_data_px->validity_time_u, OM_validity_time_u );
    PUTSHORT ( order_data_px->order_category_u, OM_order_cat_u );

    // and the single-bytes, which don't need to be rearranged
    order_data_px->bid_ask_u = bid_ask_u;
    order_data_px->order_type_u = order_type_u;
    order_data_px->ignore_away_u = ignore_away_u;
    order_data_px->client_category_u = client_category_u;
    order_data_px->broker_percentage_u = broker_percentage_u;
    order_data_px->lock_type_u = lock_type_u;
    order_data_px->expose_flag_u = expose_flag_u;
    order_data_px->open_close_u = open_close_u;

    // and the string fields
    strcpy(&order_data_px->userXRef_s[0], &OM_userXRef_s[0]);
    // NB there may not have been a user string (broker_id) supplied
    if ( broker_oper_id_s != NULL )
	strcpy((char *)&order_data_px->broker_oper_id_s, &broker_oper_id_s[0]);
    // NB there may not have been a CMTA account string supplied
    if ( cmta_member_s != NULL )
	strcpy ((char *)&order_data_px->cmta_member_s, cmta_member_s );
    // NB there may not have been a CMTA subaccount string supplied - normally only for MM orders
    if ( sub_account_id_s != NULL )
	strcpy ((char *)&order_data_px->sub_ac_s, sub_account_id_s );
    // extract the info about which user sent in the order
    get_ise_user_code_xt ( &order_data_px->user_code_x,
			    (struct user_code *)&OM_trading_code_x );

}   // external_update_order

// -----------------------------------------------------------------------

static int send_delete_order (click_details_xt * click_px,
				ise_series_xt *ise_series_px,
				quad_word orderID_u,
				uint16 order_category_u,
				uint8 bid_ask_u,
				uint8 orderbook_u,
				customer_id_st  *customer_id_ps,
				user_id_st *user_id_ps
)
/*

Function : send_delete_order
Author	 : Neil Cossor
Modified : 20010411
Description :

    This functions send a delete order to the marketplace.

    It packs the delete message with the supplied series qualifier
    which can be for
    - one series (all series fields set)
    - one underlying (country, market, and commodity code)
    - one market (country and market)
    - all series (wildcard everything)
    Note that if a specific order is to be deleted, then the series
    must be fully specified (can't do only some of expiration, strike etc)
    In theory deletions can be done for a particular expiration of
    an underlying, but I don't know if the back-end supports this,
    and it seems a somewhat unlikely thing to do.. if you want out,
    you probably want completely out (like your vols are wrong...)

    It packs the bid/ask qualifier with the supplied value,
    which can be
    - 0 (both bid and ask)
    - or the specific bid or ask value.
    Note that if a specific order/quote is to be deleted,
    the correct bid/ask value MUST be supplied.

    It packs the OrderID with the supplied value.
    This should be 0 unless a specific order is being deleted.

    It packs the Orderbook with the supplied value

    It always specifies the "whose" field as the current member, and the user
    as passed in as a parameter (if supplied). The user may be NULL, in which case
    (hopefully) we should be doing a wildcard, and the OrderID should be 0.

    It specifies the Order Category as specified (which may be 0 - or all)

    If the order is in the list of known orders, then the caller
    should do the appropriate updates if the deletion works OK.

    We default to using MO104 (specific order, bid-ask, series etc).
    If OrderID == 0, we have a wildcard bid-ask, are not specifying one-series,
    or 'user' is not specified, or specified a different user(from this session),
    then we use MO105.

Input Params:

    Pointer to standard click common
    Pointer to (Q'ed) block of data, which has a pointer to the order in question
    Order Number (ID) to delete.. may be zero for wildcard delete.
    Bid / Ask to delete. Must have specific (correct) value for deleting one order,
	may be bid or ask filter or 0 (wildcard) if deleting multiple orders
    Orderbook # - must be supplied for specific order, or multiple orders in one series
	or one underlying... not sure about generic (all underlying) delete
    Customer ID (member id). This is only allowed to be your own.. however this is provided
	as a parameter for use by ISE QA for checking Central System validation
    User ID. Put into 'whose' as supplied, may be NULL.

To do:

    For the same QA validation reasons, a parameter specifying the user's country
    should be used.

    We should also use a 'client ID' parameter, allowing selective deletes of orders
    with a particular client ID, rather than the NULLs (all client IDs) as is currently
    implemented.
*/
{
    // our msg we send up to CLICK to delete the order
    generic_txn_xt	OMmsg_x;

    int32		status_i;
    // pointer to selected series
    ise_series_info_pt	series_data_px;

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

    // the series
    put_click_series ( &OMmsg_x.tx_x.delete_trans_x.series,
			ise_series_px );
    // if this series doesn't pinpoint a single series, then use MO105 wildcard
    // try find series data for one series
    series_data_px = find_series_by_ise_series ( click_px,
						    ise_series_px );
    if (series_data_px == NULL)
	mo104_b = FALSE;


    // was an orderID specified
    if ( orderID_u.quad_word[0] == '\0' )
	mo104_b = FALSE;
    else
	// put in the supplied order ID
	PUTQUAD(OMmsg_x.tx_x.delete_trans_x.order_number_u,
		    orderID_u);

    // bid, ask, or both
    if (bid_ask_u == 0)
	mo104_b = FALSE;
    else
	OMmsg_x.tx_x.delete_trans_x.bid_or_ask_c = bid_ask_u;

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

    // use whatever order category was specified
    if (order_category_u == 0)
	mo104_b = FALSE;
    else
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
}   // send_delete_order

// ---------------------------------------------------------------------------

void delete_order_by_orderID ( click_details_xt * click_px,
				quad_word delete_orderID_u )
/*
Function:   delete_order_by_orderID
Author:	    Neil Cossor
Modified:   20010531
Description:

    We know a particular order has been deleted.. delete it from memory,
    if we know about it.

    This could occur because we explicitly deleted an order, or because
    we received a broadcast about an order delete.

Input Params:

    - pointer to the structure with everything we know about the OM/ISE system
    - the orderID to attempt to delete.. in local-endian order
*/
{
    // pointer to Q'able node, containing pointer to an order
    gen_buf_node_xt	*order_node_px;
    // pointer to uint64 as a structure, so we can dump out each half of order ID
//    uint64_xt		    *uint64_pxu;


    // delete node from our local orderbook, if we knew about it
    order_node_px = find_order_by_orderID ( click_px,
					    delete_orderID_u );
    if ( order_node_px )
	remove_order_from_list ( click_px,
				    order_node_px );
}   // delete_order_by_orderID

// ------------------------------------------------------------------------

int32 delete_last_saved_order ( click_details_xt *click_px )
/*
Function:
Author:     Neil Cossor
Modified:   20010620
Description:

    The user wishes to delete the last order which they chose to
    snapshot save (with the SAVE,ORDER) command.

    Send in specific delete request, with all the requisite params
*/
{
    int32	status_i;

    if ( click_px->saved_order_x.series_px )
    {
	// we have a last order, with sensible series pointer
	status_i = send_delete_order ( click_px,
					&click_px->saved_order_x.series_px->ise_series_x,
					click_px->saved_order_x.orderID_u,
					click_px->saved_order_x.order_category_u,
					click_px->saved_order_x.bid_ask_u,
					click_px->saved_order_x.series_px->parent_under_px->orderbook_u,
					&click_px->saved_order_x.user_code_x.customer_id_s,
					&click_px->saved_order_x.user_code_x.user_id_s
					);
	if (status_i > SUCCESSFUL)
	{
	    printf ("Successfully deleted saved order, %d contracts removed\n",
		    status_i
		    );
	    // delete node from our local orderbook, if we knew about it
	    delete_order_by_orderID ( click_px,
					click_px->saved_order_x.orderID_u );
	}
	else
	    printf ("Warning, deleting specific order - no contracts deleted\n");
	return status_i;
    }
    else
    {
	printf ("Unable to delete last saved order - no series pointer.. perhaps no SAVE done\n");
	return SUCCESSFUL;
    }
				
}   // delete_last_saved_order

// ------------------------------------------------------------------------

static int32 delete_orders_by_xref (click_details_xt * click_px,
				      char *OrderXReference )
/*
Function : delete_orders_by_xref
Author	 : Neil Cossor
Modified : 20010620
Description:

    Delete all orders from the known orders (in local orderbook), by the
    supplied order cross reference.

    There should be at least one, or we produce a warning
*/
{
    int32	    status_i = SUCCESSFUL;
    // pointer to (Q'ed) block, containing pointer to an order
    gen_buf_node_xt *order_node_px;
    // pointer to the actual order data
    order_entry_xt  *order_data_px;
    // formatted string version of premium
    series_premium_st	    premium_s;

    order_node_px = find_order_by_xref ( click_px,
					    OrderXReference );
    // now - did we find at least one match ?
    if ( !order_node_px )
    {
	printf( "Order Reference %s was not found!\n",
		    OrderXReference );
	status_i = ORDER_NOT_FOUND;
    }
    else
    {
	// loop, removing all orders with this xref
	while ( order_node_px )
	{
	    // point into the actual order data
	    // this is NOT allowed to be NULL
	    order_data_px = (order_entry_xt *)order_node_px->msg_p;

	    // format the premium, with correct implied decimals
	    format_whole_decimal_s ( (char *)&premium_s,
					order_data_px->premium_i,
					order_data_px->series_px->instrument_class_px->dec_in_premium_u,
					10 );   // width of 10
	    printf ("Deleting %s order #%d (%d %s contracts at %s), Xref [%s]\n",
			bid_ask_to_string (order_data_px->bid_ask_u),
			order_data_px->localNumber_n,
			order_data_px->quantity_u,
			&order_data_px->series_px->ise_name_s,
			&premium_s,
			order_data_px->userXRef_s );

	    status_i = send_delete_order ( click_px,
					    &order_data_px->series_px->ise_series_x,
					    order_data_px->orderID_u,
					    order_data_px->order_category_u,
					    order_data_px->bid_ask_u,
					    order_data_px->series_px->parent_under_px->orderbook_u,
					    &order_data_px->user_code_x.customer_id_s,
					    &order_data_px->user_code_x.user_id_s
					    );

	    if ( status_i > SUCCESSFUL )
	    {
		printf ("%d contracts deleted\n",
			    status_i );
		remove_order_from_list ( click_px,
					    order_node_px );
	    }
	    else
		printf ("Warning, deleting specific order - no contracts deleted\n");

	    // see if we can find another order with this xref
	    order_node_px = find_order_by_xref ( click_px,
						    OrderXReference );
	}   // while ( order_node_px )
    }	// test if we found at least one order with this xref

    return (status_i);
}   // delete_orders_by_xref

// ------------------------------------------------------------------------

static int32 delete_order_by_local_no (click_details_xt * click_px,
					int32 WantedLocalNum_i )
/*
Function : delete_order_by_local_no
Author	 : Neil Cossor
Modified : 20000606
Description:

    Delete the order from the nominated list by the
    supplied local order number
*/
{
    int32	    status_i = SUCCESSFUL;
    // pointer to (Q'ed) block, containing pointer to an order
    gen_buf_node_xt *order_node_px;
    // pointer to the actual order data
    order_entry_xt  *order_data_px;
    // formatted string version of premium
    series_premium_st	    premium_s;


    // point to first entry in chain of known orders - may be empty
    order_node_px = (gen_buf_node_xt *)click_px->our_market_x.known_orders_q.chain.next;
    while ( order_node_px )
    {
	// point into the actual order data
	// this is NOT allowed to be NULL
	order_data_px = (order_entry_xt *)order_node_px->msg_p;

	if ( order_data_px->localNumber_n == WantedLocalNum_i ) 
	{
	    // found the match - just drop out of the loop, and return ptr
	    break;
	}
	else
	{
	    // no go, try the next order - which may be NULL
	    order_node_px = (gen_buf_node_xt *)order_node_px->chain.next;
	}
    }

    // now - did we find a match ??
    if ( order_node_px )
    {
	// we know that order_data_px is already pointing at the guts of the order
	status_i = send_delete_order ( click_px,
					&order_data_px->series_px->ise_series_x,
					order_data_px->orderID_u,
					order_data_px->order_category_u,
					order_data_px->bid_ask_u,
					order_data_px->series_px->parent_under_px->orderbook_u,
					&order_data_px->user_code_x.customer_id_s,
					&order_data_px->user_code_x.user_id_s
					);

	if ( status_i > SUCCESSFUL )
	{
	    // format the premium, with correct implied decimals
	    format_whole_decimal_s ( (char *)&premium_s,
					order_data_px->premium_i,
					order_data_px->series_px->instrument_class_px->dec_in_premium_u,
					10 );   // width of 10

	    printf ( "%s order #%d (%d %s @ %s), Xref [%s] deleted\n",
			bid_ask_to_string (order_data_px->bid_ask_u ),
			order_data_px->localNumber_n,
			order_data_px->quantity_u, 
			&order_data_px->series_px->ise_name_s,
			&premium_s,
			order_data_px->userXRef_s );

	    remove_order_from_list ( click_px,
					order_node_px );
	}
	else
	    printf ("Warning, deleting specific order - no contracts deleted\n");

	status_i = SUCCESSFUL;
    }
    else
    {
	printf ( "Local Order Number %d is no longer in your Local Orderbook\n",
		    WantedLocalNum_i );
	status_i = ORDER_NOT_FOUND;
    }
    return (status_i);
}   // delete_order_by_local_no

// ------------------------------------------------------------------------

static int32 delete_orders_by_under_name (click_details_xt * click_px,
					    char *under_name_s )
/*
Function : delete_orders_by_under_name
Author	 : Neil Cossor
Modified : 20010620
Description:

    Delete all orders from the nominated list with the
    supplied underlying name

    Note that we are deleting ALL orders with the matching underlying name,
    NOT just the first.

    Note that at this point, we only attempt to delete orders which are from this
    user, not those which are from this member.
*/
{
    int32	    status_i = SUCCESSFUL;
    // pointer to (Q'ed) block, containing pointer to an order
    gen_buf_node_xt *order_node_px,
		    *next_order_px;
    // pointer to the actual order data
    order_entry_xt  *order_data_px;
    // pointer to underlying, for a particular
    ise_underlying_xt	*under_data_px;
    // local series to qualify our deletion
    ise_series_xt	ise_series_x;


    // find if we know about the given underlying
    under_data_px = find_under_by_name_s ( click_px,
					    under_name_s );
    if (!under_data_px )
    {
	printf ("ERROR - Cannot delete orders in %s, unknown Underlying\n",
		under_name_s );
	return SUCCESSFUL;
    }

    // we know that underlying. Go and delete any orders in that...

    // specify an underlying.. country, market, and commodity code
    memset ( &ise_series_x, 0, sizeof (ise_series_x));
    ise_series_x.country_u = click_px->our_market_x.country_u;
    ise_series_x.market_u = click_px->our_market_x.market_u;
    ise_series_x.underlying_code_u = under_data_px->commodity_code_u;

    // if we are set to use "by member" as default, then don't supply userID
    if ( click_px->user_member_default_u == user_member_member_e )
	status_i = send_delete_order ( click_px,
					&ise_series_x,
					click_px->null_orderID_u,	// zero for order ID
					ORDER_CAT_PLAIN_ORDERS,		// mask for plain orders, no quotes, no combos
					BIDANDASK,			// bids and asks
					// the orderbook which handles this underlying
					under_data_px->orderbook_u,
					&click_px->user_code_x.customer_id_s,
					NULL );
    else
	status_i = send_delete_order ( click_px,
					&ise_series_x,
					click_px->null_orderID_u,	// zero for order ID
					ORDER_CAT_PLAIN_ORDERS,	    // mask for plain orders, no quotes, no combos
					BIDANDASK,			// bids and asks
					// the orderbook which handles this underlying
					under_data_px->orderbook_u,
					&click_px->user_code_x.customer_id_s,
					&click_px->user_code_x.user_id_s
				    );
    if (status_i > SUCCESSFUL )
    {
	// now update our known_orders_q for any orders with that underlying

	// point to first entry in chain of known orders - may be empty
	order_node_px = (gen_buf_node_xt *)click_px->our_market_x.known_orders_q.chain.next;
	while (order_node_px)
	{
	    // point into the actual order data
	    // this is NOT allowed to be NULL
	    order_data_px = (order_entry_xt *)order_node_px->msg_p;

	    if ( order_data_px->series_px->parent_under_px->commodity_code_u ==
		    under_data_px->commodity_code_u )
	    {
		// found the match - we need to delete it.
		// NB we MUST have saved the forward pointer from it, BEFORE the delete
		next_order_px = (gen_buf_node_xt *)order_node_px->chain.next;
		remove_order_from_list ( click_px,
					    order_node_px );
		order_node_px = next_order_px;
	    }
	    else
	    {
		// no go, try the next order - which may be NULL
		order_node_px = (gen_buf_node_xt *)order_node_px->chain.next;
	    }	// test if order had same commodity code
	}   // while (order_node_px)

	// the +ve status value is the number of orders deleted
	printf ( "Deleted %d orders in %s.\n",
		    status_i,
		    &under_data_px->under_id_s );
    }	// test success of request to delete
    else
	printf ("Warning, deleting orders in underlying %s - none deleted\n",
		    &under_data_px->under_id_s );


    return SUCCESSFUL;
}   // delete_orders_by_under_name

// ------------------------------------------------------------------------

static int32 delete_orders_by_series_name (click_details_xt * click_px,
					    char *series_name_s )
/*
Function : delete_orders_by_series_name
Author	 : Neil Cossor
Modified : 20010620
Description:

    Delete all orders from the nominated list with the
    supplied series name

    Note that we are deleting ALL orders with the matching series name,
    NOT just the first.

    Note that at this point, we only attempt to delete orders which are from this
    user, not those which are from this member.
*/
{
    int32	    status_i = SUCCESSFUL;
    // pointer to (Q'ed) block, containing pointer to an order
    gen_buf_node_xt *order_node_px,
		    *next_order_px;
    // pointer to the actual order data
    order_entry_xt  *order_data_px;
    // pointer to selected series
    ise_series_info_pt	series_data_px = NULL;

    // find if we know about the given series
    series_data_px = find_series_by_name_s ( click_px,
					    series_name_s );
    if (!series_data_px )
    {
	printf ("ERROR - Cannot delete orders in %s, unknown Series\n",
		series_name_s );
	return SUCCESSFUL;
    }

    // we know that series. Go and delete any orders in that...

    // if we are set to use "by member" as default, then don't supply userID
    if ( click_px->user_member_default_u == user_member_member_e )
	status_i = send_delete_order ( click_px,
					&series_data_px->ise_series_x,
					click_px->null_orderID_u,	// zero for order ID
					ORDER_CAT_PLAIN_ORDERS,	    // mask for plain orders, no quotes, no combos
					BIDANDASK,			// bids and asks
					// the orderbook which handles this series/underlying
					series_data_px->parent_under_px->orderbook_u,
					&click_px->user_code_x.customer_id_s,
					NULL
					);
    else
	status_i = send_delete_order ( click_px,
					&series_data_px->ise_series_x,
					click_px->null_orderID_u,	// zero for order ID
					ORDER_CAT_PLAIN_ORDERS,	    // mask for plain orders, no quotes, no combos
					BIDANDASK,			// bids and asks
					// the orderbook which handles this series/underlying
					series_data_px->parent_under_px->orderbook_u,
					&click_px->user_code_x.customer_id_s,
					&click_px->user_code_x.user_id_s
					);
    if (status_i > SUCCESSFUL )
    {
	// now update our known_orders_q for any orders with that underlying

	// point to first entry in chain of known orders - may be empty
	order_node_px = (gen_buf_node_xt *)click_px->our_market_x.known_orders_q.chain.next;
	while (order_node_px)
	{
	    // point into the actual order data
	    // this is NOT allowed to be NULL
	    order_data_px = (order_entry_xt *)order_node_px->msg_p;

	    if ( cmp_series_by_ise_series_under ( &series_data_px->ise_series_x,
						    &order_data_px->series_px->ise_series_x ) == 0 )
	    {
		// found the match - we need to delete it.
		// NB we MUST have saved the forward pointer from it, BEFORE the delete
		next_order_px = (gen_buf_node_xt *)order_node_px->chain.next;
		remove_order_from_list ( click_px,
					    order_node_px );
		order_node_px = next_order_px;
	    }
	    else
	    {
		// no go, try the next order - which may be NULL
		order_node_px = (gen_buf_node_xt *)order_node_px->chain.next;
	    }	// test if order had same commodity code
	}   // while (order_node_px)

	// the +ve status value is the number of orders deleted
	printf ( "Deleted %d orders in %s.\n",
		    status_i,
		    &series_data_px->ise_name_s );
    }	// test success of request to delete
    else
	printf ("Warning, deleting orders in underlying %s - none deleted\n",
		    &series_data_px->ise_name_s );


    return SUCCESSFUL;
}   // delete_orders_by_series_name

// -----------------------------------------------------------------------

void delete_all_orders (click_details_xt * click_px)
/*
Function : delete_all_orders
Author	 : Neil Cossor
Modified : 20010620
Description :

    This function removes all orders from the marketplace,
    in all underlying / series.

    This is done with a wildcard delete (specifying 0 for
    all series fields in the delete selection....

    Note that at this point, we only attempt to delete orders which are from this
    user, not those which are from this member.
*/
{
    int32	    status_i = SUCCESSFUL;
    // pointer to Q'ed block, with pointer to data about one order
    gen_buf_node_xt *order_node_px;
    // local series to qualify our deletion
    ise_series_xt	ise_series_x;
    // loop var, to whiz around all orderbooks
    uint8		loop_i;

    // all in this country and market specify an underlying.. country, market
    memset ( &ise_series_x, 0, sizeof (ise_series_x));
    ise_series_x.country_u = click_px->our_market_x.country_u;
    ise_series_x.market_u = click_px->our_market_x.market_u;

    for (loop_i = 1;
	    loop_i <= click_px->our_market_x.max_orderbook_u;
	    loop_i++ )
    {
	// must we loop around all known orderbooks ??

	// if we are set to use "by member" as default, then don't supply userID
	if ( click_px->user_member_default_u == user_member_member_e )
	    status_i = send_delete_order ( click_px,
					    &ise_series_x,
					    click_px->null_orderID_u,   // zero for order ID
					    ORDER_CAT_PLAIN_ORDERS,	    // mask for plain orders, no quotes, no combos
					    BIDANDASK,		    // bids and asks
					    // the orderbook which handles this underlying
					    loop_i,
					    &click_px->user_code_x.customer_id_s,
					    NULL
					    );
	else
	    status_i = send_delete_order ( click_px,
					    &ise_series_x,
					    click_px->null_orderID_u,   // zero for order ID
					    ORDER_CAT_PLAIN_ORDERS,	    // mask for plain orders, no quotes, no combos
					    BIDANDASK,		    // bids and asks
					    // the orderbook which handles this underlying
					    loop_i,
					    &click_px->user_code_x.customer_id_s,
					    &click_px->user_code_x.user_id_s
					    );
	if (status_i < SUCCESSFUL )
	    // error doing one of the deletes, bail out
	    break;
	else if (status_i == SUCCESSFUL)
	    printf ("Warning, deleting orders in OB server %u - no contracts deleted\n",
			loop_i );
    }

    // update our orderbook if all the deletes went OK
    /* NB this logic is probably NOT correct - we should specifically remove
	the orders in the orderbooks that had good deletes... rather than all or none
	but then again, we shouldn't get any errors doing deletes anyway.
    */
    if (status_i >= SUCCESSFUL )
    {
	// remove all known orders from the list
	do
	{
	    order_node_px = (gen_buf_node_xt *)click_px->our_market_x.known_orders_q.chain.next;
	    // did we find an order block to delete ?
	    if ( order_node_px )
	    {
		// remove the order at the front of the order list
		remove_order_from_list ( click_px,
					    order_node_px );
	    }	// did we find an entry to delete
	}
	while ( order_node_px != NULL);
    }
}   // delete_all_orders

// -----------------------------------------------------------------------

int32 delete_order(click_details_xt * click_px,
		    int argc,
		    char *argv[ ] )
/*
Function : delete_order
Author	 : Neil Cossor
Modified : 20010627
Description:

    We have been asked to delete one/some orders.

    Calls the appropriate routine to carry this out, including the deletion
    from both the local orderbook, and the market (i.e. central orderbook).

Input Params:

    standard argc/argv params that got us here i.e. the command.
    Choices of command are:-

    Delete,E[xact],<seriesname>,<orderID>,<bid/ask> delete one order, using values supplied
    Delete,L[ocal],<local order #>	one order, using local order number
    Delete,R[eference],<xref>		(one) local order, with particular cross-reference
    Delete,S[eries],<seriesname>	ALL local orders for one series
    Delete,U[nderlying],<undername>	ALL local orders for one underlying
    Delete,*				ALL local orders for ALL underlying
    Delete,%O				the order which we last snapshot-saved

    Note that each argv[] string has had leading whitespace stripped

Output Params:

    updated local and central orderbook
*/
{
    int 		choice_i;	    // enumeration of which delete subcommand was selected
    int32		WantedNumber_n;     // order number to be deleted
    // pointer to data about one series - used for DELETE,EXACT flavour
    ise_series_info_xt	*series_data_px = NULL;
    // bid-ask value specified - used for DELETE,EXACT flavour
    uint8		bid_ask_u;
    // return status from order delete. -ve means error, +ve is #orders/#contracts deleted
    int32		status_i;
    // orderID of a specific order we wish to delete
    quad_word		delete_orderID_u;

    static char *delete_subcommands_s [] =
    {
	"EXACT",
	"LOCAL",
	"REFERENCE",
	"SERIES",
	"UNDERLYING",
	"*",
	"%O"
    };
    // enumerated list which corresponds to the above list of choices
    enum delete_subcommands_et { delete_subcommand_exact_e,
				    delete_subcommand_local_e,
				    delete_subcommand_reference_e,
				    delete_subcommand_series_e,
				    delete_subcommand_underlying_e,
				    delete_subcommand_wildcard_e,
				    delete_subcommand_last_saved_e };

    // enough arguments ?
    if ( argc < 2 )
    {
	prompt_delete_command ();
	return INVALID_ARGUMENTS;
    }

    // which flavour of delete ?
    choice_i = select_string_option ( 7, &delete_subcommands_s [0], argv[1] );
    switch (choice_i)
    {
    case delete_subcommand_exact_e:
	// delete a specific order.. we have the series, orderID and side, but not the user

	// enough arguments ? We require all arguments to be qualified
	if (( argc < 5 ) || (argv[2] == NULL) || (argv[3] == NULL) || (argv[4] == NULL))
	{
	    prompt_delete_command ();
	    return INVALID_ARGUMENTS;
	}

	// read the series, and try to find it
	series_data_px = user_selected_series ( click_px, argv[2] );
	if (series_data_px == NULL)
	{
	    // couldn't find that series - error already reported - bail out
	    return INVALID_ARGUMENTS;
	}

	// read the orderID
	cmd_orderID ( click_px, argv[3], &delete_orderID_u );

	// read the side
	bid_ask_u = cmd_bid_ask ( click_px, argv[4] );
	if (( bid_ask_u != BID ) && ( bid_ask_u != ASK ))
	{
	    // didn't understand the bid/ask side
	    printf ("%s did not specify one of BID|ASK\n", argv[4] );
	    return INVALID_ARGUMENTS;
	}

	status_i = send_delete_order ( click_px,
					&series_data_px->ise_series_x,
					delete_orderID_u,
					ORDER_CAT_PLAIN_ORDERS,	    // mask for plain orders, no quotes, no combos
					bid_ask_u,
					series_data_px->parent_under_px->orderbook_u,
					&click_px->user_code_x.customer_id_s,
					NULL );

	if ( status_i > 0 )
	    printf ("deleted %u %s contracts in %s\n",
			status_i,
			argv[4],
			series_data_px->ise_name_s
		    );
	break;

    case delete_subcommand_local_e:
	// delete by local reference #

	if ( empty_q (&click_px->our_market_x.known_orders_q ) )
	{
	    printf ("There Are No Orders In The Local Orderbook To Delete.\n");
	    return SUCCESSFUL;
	}

	WantedNumber_n = read_unsigned_limited ( argv[2],
						    click_px->maxLocalOrderNumber_u );
	// silly number ?
	if ( WantedNumber_n > click_px->maxLocalOrderNumber_u )
	{
	    printf( "The Maximum Local Order Number is %d.\n",
			click_px->maxLocalOrderNumber_u );

	    return SUCCESSFUL;
	}

	// go and attemp to delete that order
	delete_order_by_local_no ( click_px,
				    WantedNumber_n );
	break;
    case delete_subcommand_reference_e:
	// delete by cross-Reference

	if ( empty_q (&click_px->our_market_x.known_orders_q ) )
	{
	    printf ("There Are No Orders In The Local Orderbook To Delete.\n");
	    return SUCCESSFUL;
	}

	delete_orders_by_xref (click_px,
				argv[2] );
	break;
    case delete_subcommand_series_e:
	// delete by series
	delete_orders_by_series_name ( click_px, argv[2] );
	break;

    case delete_subcommand_underlying_e:
	// delete by underlying

	/* They have entered a underlying for which they want
	    all orders to be deleted.
	 */
	delete_orders_by_under_name ( click_px, argv[2] );
	break;

    case delete_subcommand_wildcard_e:
	// delete all in all underlying

	printf ("Deleting all local orders \n\n");
	delete_all_orders ( click_px );
	break;

    case delete_subcommand_last_saved_e:
	// delete the last order we snapshot-saved

	delete_last_saved_order ( click_px );
	break;

    default:
	prompt_delete_command();
    }

    return SUCCESSFUL;
}   // delete_order

// -----------------------------------------------------------------------

static int32 send_plain_order (click_details_xt * click_px,
			 gen_buf_node_xt    *order_node_px )	
/*
Function : send_plain_order
Author	 : Neil Cossor
Modified : 20010620
Description:

    For the supplied order information, sends an order to the system
    Uses mo101

    If the order is accepted, then we update the orderID.
*/
{
    // local buffer, containing actual msg we send
    generic_txn_xt	OMmsg_x;
    // pointer to the actual order data
    order_entry_xt	*order_data_px;

    int32		status_i	     =	 SUCCESSFUL;
    // size of our actual CLICK query/command
    uint32		tx_length_u = sizeof ( order_trans_mo101_t );


    // point to the order data block
    order_data_px = (order_entry_xt *)order_node_px->msg_p;

    pack_OM_header ( &OMmsg_x, tx_length_u, 'M', 'O', 101);
    // save the facility code and fn code
    OMmsg_x.fn_code = LOG_K_C_ORDER_TRANS;
    OMmsg_x.facility_u = click_px->facility_no_u + 7;	// EP7/ET7

    put_click_series ( &OMmsg_x.tx_x.order_trans_x.series,
			&order_data_px->series_px->ise_series_x );

    // now we can specify orderbook_c
    OMmsg_x.tx_x.order_trans_x.orderbook_c = order_data_px->series_px->parent_under_px->orderbook_u;
    // endian fields
    PUTLONG ( OMmsg_x.tx_x.order_trans_x.premium_i,
		order_data_px->premium_i );
    PUTLONG ( OMmsg_x.tx_x.order_trans_x.quantity_u,
		order_data_px->quantity_u );
    // validity time - GTC / ROD / IOC / FOK
    PUTSHORT ( OMmsg_x.tx_x.order_trans_x.time_validity_n,
		order_data_px->validity_time_u );
    // bid / ask
    OMmsg_x.tx_x.order_trans_x.bid_or_ask_c = order_data_px->bid_ask_u;
    // limit/market
    OMmsg_x.tx_x.order_trans_x.order_type_c = order_data_px->order_type_u;
    // ignore away flag
    OMmsg_x.tx_x.order_trans_x.ignore_away_market_c = order_data_px->ignore_away_u;
    // EAM/FIRM/FARMM
    OMmsg_x.tx_x.order_trans_x.client_category_c = order_data_px->client_category_u;
    // open/close setting for a/c during OCC settlement
    OMmsg_x.tx_x.order_trans_x.open_close_req_c = order_data_px->open_close_u;

    // put in the two parts of the client ID, which are actually the CMTA a/c and sub a/c IDs
    tx_click_string ( &OMmsg_x.tx_x.order_trans_x.ex_client_s [0],
			&order_data_px->cmta_member_s,
			MAX_CMTA_MEMBER );
    tx_click_string ( &OMmsg_x.tx_x.order_trans_x.ex_client_s [MAX_CMTA_MEMBER],
			&order_data_px->sub_ac_s,
			MAX_SUB_ACCOUNT_ID );
    // need to set the broker_id_s
    tx_click_string ( &OMmsg_x.tx_x.order_trans_x.broker_id_s,
			&order_data_px->broker_oper_id_s,
			sizeof (OMmsg_x.tx_x.order_trans_x.broker_id_s));
    tx_click_string ( &OMmsg_x.tx_x.order_trans_x.free_text_s[0],
		     order_data_px->userXRef_s, 
		     sizeof (OMmsg_x.tx_x.order_trans_x.free_text_s ) );

    status_i = do_click_tx_rx ( click_px,
				    &OMmsg_x,
				    tx_length_u );
    if ( status_i < SUCCESSFUL)
    {
	// do nothing, error already reported by do_click_tx_rx
    }
    else
    {
	// update the order ID of the order
	PUTQUAD ( order_data_px->orderID_u,
		    click_px->log_reply_x.OrderIdent_u );

	// there are some success status values where we do NOT have
	// anything left in the orderbook.

	// one special case is where the order was converted to block..
	// so the order category must changed
	switch ( status_i )
	{
	case ORDER_STATUS_TO_BLOCK:
		// convert to block (needed to MO104 delete to work correctly)
		order_data_px->order_category_u = ORDER_CAT_BLOCK;
		// NB drop through into the other Market -> Limit cases
	case ORDER_STATUS_TO_LIMIT:
	case ORDER_STATUS_MATCH_TO_LIMIT:
		// change from market order type to limit order type
		order_data_px->order_type_u = ORDER_TYPE_LIMIT;
		break;
	default: {};
	    // nothing special to do
	}

    }

    // save this order into last_order_x, and note the customer and member
    snapshot_last_order ( click_px, order_data_px );

    return (status_i);
}   // send_plain_order

// -----------------------------------------------------------------------

int32 generate_order (click_details_xt * click_px,
		    int argc,
		    char *argv[ ],
		    int8 bid_ask_u )
/*
Function : generate_order
Author	 : Neil Cossor
Modified : 20000422
Description:

    This function generates a (local) order block, and sends
    the order to the central MP server (by calling send_plain_order).
    This is for everyday plain orders
    NOT block, facilitation, or CAB orders

    If the order is accepted, then we add the order into the
    known order Q, otherwise we release the (local) order block.

    It also retains the series, premium, and quantity to use as defaults
    next time this function is called during this session.

    It is triggered by a call to BID or ASK.

Input Params:

    argv[0]	"BID"|"ASK"
    argv[1]	series name
    argv[2]	quantity
    argv[3]	premium
    argv[4]	X-reference == freetext
    (optionally)
    argv[5]	validity time.. G[tc] / R[od] / I[oc] / F[ok], default GTC
    argv[6]	client category.. C[ustomer] | FI[rm\ | FA[rmm], default Client
    argv[7]	order type.. L[imit] / M[arket], default Limit
    argv[8]	ignore away market 0==consider | 1 == ignore, default 1 (Ignore)
    argv[9]	broker ID (i.e. operator), default of this user
    argv[10]	open/close setting for OCC account, default is "use default for a/c"
    argv[11]	CMTA account, default is ""
    argv[12]	" sub account, default is ""
*/
{
    // pointer to the Q'able block, which we will contain a ptr to actual order
    gen_buf_node_xt	*order_node_px;
    // pointer to the actual order data
    order_entry_xt	*order_data_px;
    // pointer to relevant series
    ise_series_info_xt	*series_data_px = NULL;

    // local status var
    int 		status_i;

    // basic data for transaction
    int32		order_premium_u;
    uint16		order_quantity_u;
    xref_st		order_xref_s;
    uint16		validity_time_u;
    uint8		order_type_u;	    // limit/market
    uint8		client_category_u;  // cust/Firm/FARMM
    uint8		ignore_away_u;	    // 1 == ignore
    user_id_st		*broker_oper_id_ps; // pointer to broker/operator ID
    uint8		open_close_u;	    // OCC settlement setting
    char		*cmta_member_ps;    // pointer to CMTA member ID string
    char		*sub_ac_ps;    // pointer to a sub account ID string

    if ( click_px->logged_on_i != TRUE )
    {
	prompt_logged_on ( bid_ask_to_string ( bid_ask_u ));
	return NOT_LOGGED_ON;
    }

    strcpy (order_xref_s, "");

    // have we enuff arguments - minimum of 4
    if ( argc < 5)
    {
	// insufficient arguments - prompt the user, and return
	prompt_bid_ask ( bid_ask_to_string (bid_ask_u) );
	return SUCCESSFUL;
    }

    // we have the minumum argument list

    // series name in argv[1] - use that directly in search for ise_series_info_xt
    series_data_px = user_selected_series ( click_px, argv[1] );
    if (series_data_px == NULL)
    {
	// couldn't find that series - error already reported - bail out
	return SUCCESSFUL;
    }

    // Read the order quantity
    order_quantity_u = (uint16)read_unsigned_limited ( argv [2], UNDEFINED_UINT16 );

    // Read the order premium
    order_premium_u = read_decimal_string_int ( argv[3],
				series_data_px->instrument_class_px->dec_in_premium_u );
    // And finally, see if there is an order xref
    strncpy(order_xref_s, argv[4], sizeof (order_xref_s));

    // optional parameters

    // validity time
    if ( argc >= 6 )
    {
	// customer specified a validity time
	validity_time_u = cmd_validity_time ( argv[5], VALIDITY_TIME_FAS_GTC );
	if ( validity_time_u == UNDEFINED_UINT16 )
	{
	    // illegal validity time selector - reported, just return to called
	    return SUCCESSFUL;
	}
    }
    else
	// default to GTC
	validity_time_u = cmd_validity_time ( "", VALIDITY_TIME_FAS_GTC );

    // client category
    if ( argc >= 7 )
    {
	client_category_u = cmd_client_category ( argv[6], CUST_CATEGORY_CUSTOMER );
    }
    else
	// default to client category
	client_category_u = CUST_CATEGORY_CUSTOMER;

    // order type
    if ( argc >= 8 )
    {
	order_type_u = cmd_order_type ( argv[7], ORDER_TYPE_LIMIT );
	if ( order_type_u == UNDEFINED_UINT8 )
	    // illegal selection in command line - error reported
	    return SUCCESSFUL;
    }
    else
	// default to Limit order
	order_type_u = ORDER_TYPE_LIMIT;

    // ignore away market flag
    if ( argc >= 9 )
    {
	// customer specified an ignore away market flag - max legal value 1
	ignore_away_u = read_unsigned_limited ( argv[8], 1 );
    }
    else
	// default to ignore away market
	ignore_away_u = 1;

    // broker / operator ID string
    if ( argc >= 10 )
	// customer specified the broker ID
	(char *)broker_oper_id_ps = argv[9];
    else
	// customer did not specify a broker ID - use our own
	broker_oper_id_ps = &click_px->user_code_x.customer_id_s;

    // open/close behaviour for OCC account during settlement
    if ( argc >= 11 )
	// customer specified a value for open/close
	open_close_u = read_unsigned_limited ( argv[10], UNDEFINED_UINT8 );
    else
	open_close_u = OPEN_CLOSE_REQUEST_DEFAULT;

    // see if supplied CMTA account
    if ( argc >= 12 )
	// customer specified a value for CMTA member ID
	cmta_member_ps = argv[11];
    else
	// if non specified, no default (most trades are not transferred via CMTA to other OCC firms)
	cmta_member_ps = "";

    // see if supplied CMTA sub account
    if ( argc >= 13 )
	// customer specified a value for CMTA sub account ID
	sub_ac_ps = argv[12];
    else
	/* if non specified, no default (normally only trades from MMs have a sub-account ID,
	    and all MM users have a default sub-account ID in CDB
	*/
	sub_ac_ps = "";


    // create an order_entry block. No Order ID yet...
    order_node_px = create_order_entry ( ++click_px->maxLocalOrderNumber_u );
    // point to the order data block of what we just got
    order_data_px = (order_entry_xt *)order_node_px->msg_p;
    update_order ( click_px,
		    order_data_px,
		    series_data_px,
		    click_px->null_orderID_u,	// no orderID, until accepted
		    order_premium_u,
		    order_quantity_u,
		    bid_ask_u,
		    order_xref_s,
		    validity_time_u,
		    order_type_u,
		    client_category_u,
		    ignore_away_u,
		    (char *)broker_oper_id_ps,
		    0,			    // lock type - none, not locked
		    ORDER_CAT_ORDER,	    // ordinary order
		    0,			    // expose flag - none, not block order
		    open_close_u,	    // a/c behaviour during OCC settlement
		    cmta_member_ps,	    // CMTA account
		    sub_ac_ps,	    // and sub account ID
		    0			    // broker percentage
		    );


    // and attempt to send it
    status_i = send_plain_order(click_px,
			    order_node_px );
    // if order went in OK, then add it to the Q of known orders
    if (status_i >= SUCCESSFUL)
    {
	// display the order
	display_one_order ( order_data_px );
	printf ("%s\n",
		order_status_to_string ( status_i )
		);
	// 20010529 - need something to switch on different statii - like locks
	if (( status_i != ORDER_STATUS_MATCH ) && ( status_i != ORDER_STATUS_MATCH_NO_PART ))
	{
	    add_to_q_tail ( &click_px->our_market_x.known_orders_q,
			    &order_node_px->chain );
	}
	else
	{
	    // it traded (partially or fully), and there is no remainder in the orderbook
	    click_px->maxLocalOrderNumber_u--;
	    // release the memory allocated to the order
	    free_gen_buf_node ( order_node_px );
	}
    }
    else
    {
	// release the memory allocated to the order
	free_gen_buf_node ( order_node_px );
	click_px->maxLocalOrderNumber_u--;
	// error already reported
    }

    return status_i;
}   // generate_order

// -----------------------------------------------------------------------

int32 amend_order (click_details_xt * click_px,
		    int argc,
		    char *argv[ ] )
/*
Function : amend_order
Author	 : Neil Cossor
Modified : 20010702
Description :

    This function collects the data for a change of quantity to
    an existing order, to do a MO103 transaction.

    the input (to be sent), which we must get from somewhere include
    - series, which MUST be specified
    - orderID, which MUST be specified
    - bid / ask, as per original order
    - the orderbook #, which we can derive from the series->underlying->orderbook

    The fields which may be changed are
    - quantity
    - free text (Xref)
    - validity time
    - ignore away market
    - broker id
    - open / close flag

    Note ISE does NOT allow an amend operation to change the premium.

Input Params:

    the usual C-style argc/argv[] from the command line
    If any of these are supplied with NULL pointers, or 0 (string) values,
    then NUL is packed into the MO103, which tells the MP server to leave
    the old value as it was for that field.

    argv[0]	AMEND
    argv[1]	<Bid|Ask>	or %O (bid/ask from last order)
    argv[2]	<series name>	or %O (series name from last order)
    argv[3]	<orderID>	or %O (orderID from last order)
    (optional, although at least ONE of these must be supplied, or there was no point amending)
	argv[4] <action>
	argv[5] <new Bid|Ask>
	argv[6] <new series name>
    argv[7]	<new quantity>
    argv[8]	<new free-text (Xref)>
    argv[9]	<new validity-time>
    argv[10]	<new ignore-away market setting>
    argv[11]	<new broker-id>
    argv[12]	<new open-close flag value>
	argv[13] <new order type>
	argv[14] <new premium>
    argv[15]	client category.. C[ustomer] | FI[rm\ | FA[rmm], default Client
	argv[16] <new CMTA ID>
	argv[17] <new <sub a/c>
	argv[18] <quantiy - condition>

*/
{
    generic_txn_xt	OMmsg_x;	// our alter request msg
    // size of our actual CLICK query/command
    uint32		tx_length_u = sizeof ( alter_trans_mo103_t );

    uint32		order_quantity_u;



    // pointer to info about the specified series
    ise_series_info_xt	*series_data_px;
	ise_series_info_xt	*new_series_data_px;
    // pointer to info about entry in local orderbook, if we know about that order
    order_entry_xt	*order_data_px = NULL;
    // pointer to (Q'ed) block, containing pointer to an order
    gen_buf_node_xt	*order_node_px = NULL;

    // status var, with default val
    int32		status_i = SUCCESSFUL;
    // the orderID specified
    quad_word		orderID_u;
    // the new validity time specified
    uint16		validity_time_u;
    // the new ignore-away market setting
    uint8		ignore_away_u;
    // the new open-close flag value
    uint8		open_close_u;
    // the bid/ask specified - can have come from %O selector
    uint8		bid_ask_u;

   	quad_conv_t * convOrderNo;

    // zot everything in our inbound msg
    memset ( &OMmsg_x, 0, tx_length_u );

    // put in the correct header
    pack_OM_header ( &OMmsg_x, tx_length_u, 'M', 'O', 103);

    // enough arguments ?
    if ( argc < 6 )
    {
	printf ("Error.. usage :-\n"
		"AMEND,Bid|Ask,<seriesname>,<orderID>,<action>[<new Bid|Ask>[,<new series name>[,<new qty>][,<new xref>[,<new valid.time>\n"
		"[,<new ign.away>[,<new bkr ID>[,<new open/close>[,<new order type[,<new premium>[,<new client category>\n"
		",<new CMTA ID>[,<new sub a/c>[,<quantity-condition>]]]]]]]]]]]]\n"
		);
	return status_i;
    }


    // process commandline

    // first argument is bid/ask - can be %O
    bid_ask_u = cmd_bid_ask ( click_px, argv[1] );
    // specify the bid/ask type as supplied
    OMmsg_x.tx_x.alter_trans_x.bid_or_ask_c = bid_ask_u;


    // series name -> a series (can be %S, %O)
    series_data_px = user_selected_series ( click_px, argv[2] );
    if (series_data_px == NULL)
    {
	// couldn't find that series - error already reported - bail out
	return SUCCESSFUL;
    }


    // specify the series as selected
    put_click_series ( &OMmsg_x.tx_x.alter_trans_x.series,
			&series_data_px->ise_series_x );
    // specify the orderbook, to match the series
    OMmsg_x.tx_x.alter_trans_x.orderbook_c =
		    series_data_px->parent_under_px->orderbook_u;

    
    // get the order ID - can be %O
	cmd_orderID ( click_px,
				argv[3],     &orderID_u  );
    // specify the original order ID
    PUTQUAD ( OMmsg_x.tx_x.alter_trans_x.order_number_u,
		orderID_u );


    // do we know about an order with that ID, in the local orderbook ?
    order_node_px = find_order_by_orderID ( click_px,
					    orderID_u );
    if (order_node_px)
	order_data_px = (order_entry_xt *)order_node_px->msg_p;


	OMmsg_x.tx_x.alter_trans_x.action_c = cmd_action(argv[4]);
	if (OMmsg_x.tx_x.alter_trans_x.action_c == 250 ) {
		// couldn't find that series - error already reported - bail out
		return SUCCESSFUL;
	}
		
	if (argc >= 6 && argv[5] && argv[5][0] != '0') {
		OMmsg_x.tx_x.alter_trans_x.bid_or_ask_next_c =  cmd_bid_ask ( click_px, argv[5] );
		if (OMmsg_x.tx_x.alter_trans_x.bid_or_ask_next_c == -1) {
			// couldn't find that series - error already reported - bail out
			return SUCCESSFUL;
		}
	}
	

		
    if (argc >= 7 && argv[6] && argv[6][0] != '0') {
	    new_series_data_px = user_selected_series ( click_px, argv[6] );
	    if (new_series_data_px == NULL)
		{
			// couldn't find that series - error already reported - bail out
			return SUCCESSFUL;
		}

		// specify the series as selected
		put_click_series ( (series_t *) &OMmsg_x.tx_x.alter_trans_x.series_new,
				&new_series_data_px->ise_series_x );
	}
    
	// new quantity, if supplied
    if (( argc >= 8 ) && (argv[7]))
    {
	// new quantity - can't change to 0 (which would be unchanged), only allowed to decrease
	order_quantity_u =  (uint32)read_unsigned_limited ( argv[7], UNDEFINED_UINT16 );

	// specify the quantity. If it has NOT altered, then should specify 0
	PUTLONG ( OMmsg_x.tx_x.alter_trans_x.quantity_u,
		    order_quantity_u );
	// update the local orderbook, if we knew about that order
	if ((order_data_px) && (order_quantity_u))
	    order_data_px->quantity_u = order_quantity_u;
    }	// test if new qty supplied

    // new cross-reference, if supplied
    if (( argc >= 9 ) && (argv[8]))
    {
	// update to Xref

	// respecify the xref - can alter it - NB CANNOT make it empty.. NULs means leave alone
	tx_click_string (&OMmsg_x.tx_x.alter_trans_x.free_text_s[0],
			 argv[8],
			 sizeof (OMmsg_x.tx_x.alter_trans_x.free_text_s ) );
	// update the local orderbook, if we knew about that order
	if ((order_data_px) && (strlen(argv[8])!=0))
	    strncpy ( order_data_px->userXRef_s,
			argv[8],
			MAX_XREF );
    }	// test if new xref supplied

    // new validity, if supplied
    if (( argc >= 10 ) && (argv[9]))
    {
	/* update to validity time..
	    NB there being more args does NOT mean a value was supplied.
	    As only GTC->Day is allowed, user value as is by default, i.e. 0 (no change).
	    Thanks Jim P.
	*/
	validity_time_u = cmd_validity_time ( argv[9], 0 );


	// respecify the validity time NB some things aren't allowed, and can't ->IOC, as this == 0
	PUTSHORT ( OMmsg_x.tx_x.alter_trans_x.time_validity_n,
		    validity_time_u );
	// update the local orderbook, if we knew about that order
	if ((order_data_px) && (validity_time_u))
	    order_data_px->validity_time_u = validity_time_u;
    }	// test if new validity supplied

    // new ignore_away, if supplied
    if (( argc >= 11 ) && (argv[10]))
    {
	// update to ignore_away.. default to no change.. NB can't change to FALSE (0), so def of 0 is OK
	ignore_away_u = read_unsigned_limited ( argv[10], 0 );

	// respecify the ignore_away
	OMmsg_x.tx_x.alter_trans_x.ignore_away_market_c = ignore_away_u;
	// update the local orderbook, if we knew about that order
	if ((order_data_px) && (ignore_away_u))
	    order_data_px->ignore_away_u = ignore_away_u;
    }	// test if new ignore_away supplied

    // new brokerID if supplied
    if (( argc >= 12 ) && (argv[11]))
    {
	tx_click_string (&OMmsg_x.tx_x.alter_trans_x.broker_id_s,
			 argv[11],
			 sizeof (OMmsg_x.tx_x.alter_trans_x.broker_id_s ) );
	// update the local orderbook, if we knew about that order
	if ((order_data_px) && (strlen(argv[11])!=0))
	    strncpy ( order_data_px->broker_oper_id_s,
			argv[11],
			MAX_CUSTOMER_ID );
    }	// test if new brokerID supplied

    // new open/close, if supplied
    if (( argc >= 13 ) && (argv[12]))
    {
	// update to open/close flag.. can't change to 0 (default for account), so default here is OK
	open_close_u = read_unsigned_limited  ( argv[12], UNDEFINED_UINT8 );

	// respecify the open/close flag
	OMmsg_x.tx_x.alter_trans_x.open_close_req_c = open_close_u;
	// update the local orderbook, if we knew about that order
	if ((order_data_px) && ( open_close_u))
	    order_data_px->open_close_u = open_close_u;
    }	// test if new open/close supplied
	if (argc >= 14 && argv[13]) {
		//as of 17.1 order_type
		OMmsg_x.tx_x.alter_trans_x.order_type_c = cmd_order_type ( argv[13], 0 );			
	}
	if (argc >= 15 && argv[14]) {
		//as of 17.1 premium
		if (argv[6]) { //if a new series was specified, use the new series' underlying
			OMmsg_x.tx_x.alter_trans_x.premium_i = read_decimal_string_int ( argv[14],
				new_series_data_px->instrument_class_px->dec_in_premium_u );
		}
		else {
			OMmsg_x.tx_x.alter_trans_x.premium_i = read_decimal_string_int ( argv[14],
				series_data_px->instrument_class_px->dec_in_premium_u );
		}

	}
	if (argc >= 16 && argv[15]) {
		//as of 17.1 client category
		OMmsg_x.tx_x.alter_trans_x.client_category_c = cmd_client_category ( argv[15], CUST_CATEGORY_CUSTOMER );	
	}
	if (argc >= 17 && argv[16]) {

		// put in the two parts of the client ID, which are actually the CMTA a/c and sub a/c IDs as of 17.1
		tx_click_string ( &OMmsg_x.tx_x.alter_trans_x.ex_client_s [0],
				&argv[16], MAX_CMTA_MEMBER );
	}
	if (argc >= 18 && argv[17]) {
		tx_click_string ( &OMmsg_x.tx_x.alter_trans_x.ex_client_s [MAX_CMTA_MEMBER],
				&argv[16],
				MAX_SUB_ACCOUNT_ID );
	}

	if (argc >= 19 && argv[18]) {
		PUTLONG(OMmsg_x.tx_x.alter_trans_x.condition_quantity_u, atol(argv[18]));
	}
	
	// save the facility code and fn code
    OMmsg_x.fn_code = LOG_K_C_ALTER_TRANS;
    OMmsg_x.facility_u = click_px->facility_no_u + 7;	// EP7/ET7

    // point structure pointer at the 64bit order ID for debug display
    //uint64_pxu = (uint64_xt *)&(orderID_u);
	convOrderNo = (quad_conv_t *) &(orderID_u.quad_word);

    printf ("Changing Order Number: [%08lX:%08lX]\n",
	     convOrderNo->l2,
		convOrderNo->l1);

    status_i = do_click_tx_rx ( click_px,
				    &OMmsg_x,
				    tx_length_u );


    if ( status_i > SUCCESSFUL )
    {
	printf ( "Order altered, formerly %u contracts\n",
		    status_i );

	// report details of the order, if we knew the details before the amend
	if ( order_data_px )
	{
	    // 20000328. DO NOT replace the orderID of original order.. the orderID of
	    // amend omniapi_tx does NOT appear to be relevant.

	    display_one_order ( order_data_px );
	}
    }
    else if ( status_i == 0 )
    {
	printf ("Warning - Order not known in MarketPlace to amend\n");
    }	// test if amend was accepted

    return status_i;

}   // amend_order

// -----------------------------------------------------------------------

void display_my_orderbook (click_details_xt *click_px)
/*
Function : display_my_orderbook
Author	 : Neil Cossor
Modified : 9905xx
Description :

    Displays the contents of the local order book.

    The local orderbook is updated by
    - use of the GET command
    - entry of new orders (ASK, BID commands)
    - amends and deletes from the local orderbook
*/
{
    // pointer to (Q'ed) block, containing pointer to an order
    gen_buf_node_xt *order_node_px;
    // pointer to the actual order data
    order_entry_xt  *order_data_px;
   
    // point to first entry in chain of known orders - may be empty
    order_node_px = (gen_buf_node_xt *)click_px->our_market_x.known_orders_q.chain.next;
    // are there any orders - i.e. should we spit out some column headers
    if ( order_node_px )
    {
	printf ("\nCategory        Type Local#  Series Name    Quantity    Premium  ORDER ID           Valid Client   O'type Xref\n" );
	printf (  "______________________________________________________________________________________________________________\n");

	do  // while ( order_node_px )
	{
	    // point into the actual order data
	    // this is NOT allowed to be NULL
	    order_data_px = (order_entry_xt *)order_node_px->msg_p;

	    display_one_order ( order_data_px );

	    // point to the next one, which may be NULL
	    order_node_px = (gen_buf_node_xt *)order_node_px->chain.next;
	} while ( order_node_px );

	// end of the list
	printf ("\n\n");
    }
    else
    {
	printf ("The Local Orderbook contains no orders\n");
    }

    return;
}   // display_my_orderbook

// -----------------------------------------------------------------------

/*
Function : clear_local_orderbook
Author	 : Neil Cossor
Modified : 20000521
Description :

    Clears out the local order book, release memory involved etc.

    This happens when we logout / disconnect, or do a query for all orders.

    NOTE this does NOT release the resources in the actual Q header
*/
void clear_local_orderbook (click_details_xt *click_px)
{
    // release all of the Q of known orders, which also zots the account
    flush_gen_buf_node_q ( &click_px->our_market_x.known_orders_q, TRUE );

    // and the highest local order number ??? what is the relevance of this
    click_px->maxLocalOrderNumber_u = 0;
}   // clear_local_orderbook

// -----------------------------------------------------------------------

int32 query_active_order ( click_details_xt * click_px,
			    ise_series_xt *ise_series_px,
			    uint8 orderbook_u,
			    uint8 bin_u,
			    uint16 query_order_category_u,
			    uint32 *quotes_skipped_u,
				uint16_t order_select)
/*
Function : query_active_order
Author	 : Neil Cossor
Modified : 20010620
Description :

    Inquires on active orderbook.
    Uses mq101.

    The resulting orders are added to the local list of known orders,
    if they were not already in that list.

    Note that the format of the query (mq101 and mq102) were the same
    until 16.5, but the format of the responses was different.
    Under 16.5, the queries are different formats too.

Input Params:

    - pointer to all data we know about the OM / ISE central system
    - the series qualifier we are to query on - NULL if we are doing wildcard all
    - the orderbook we are to query on
    - the bin we are to query on
    - the order category we are to query on (0 = all, can't query on quotes)
    - the # of quotes we skipped in this call (initialised to 0 by this fn)

Implicit params:
    - our flag if we are playing by user or by member

Output Params:

    If we get an error, we return an error status (-ve),
    otherwise we return the # of orders found (which may be 0)

    Updates the internal list of orders.
    Optional (?debug?) displays the orders in some simple form.
*/
{
    generic_txn_xt	OMmsg_x;
    // size of our actual CLICK query/command
    uint32		tx_length_u = sizeof ( OMmsg_x.tx_x.query_order_x );
    // status var to return, with good default initial value
    int 		status_i = SUCCESSFUL;
    // pointer to one interesting chunk of the response msg
    answer_order_ma101_item_t	*txn_reply_px;
    // local copy of the series record we got back in an order
    ise_series_xt	ise_series_x;
    // count of how many orders we found which matched the caller selection
    int 		orders_found_i = 0;
    // and the count of how many items we found in other series in the response
    int			orders_other_series_i = 0;
    uint16		item_index_u,	// local loop var
			items_found_u;	// limit on loop - may be endian-swapped from response

    // local-endian copies of fields from query response
    uint16		order_category_u;
    xref_st		free_text_s;
    user_id_st		broker_oper_id_s;   // the broker ID who owns this order
    cmta_member_st	cmta_member_s;	    // CMTA member ID string
    sub_account_id_st	sub_ac_s;	    // and sub account ID string


    /* set up query to have 0 in all fields - which includes selection qualifiers
	This implies
	- both bids and asks
	- all instrument groups
	- no specific order #
	- all order categories
    */
    memset( &OMmsg_x.tx_x.query_order_x, 0, tx_length_u );

    // MQ101
    pack_OM_header ( &OMmsg_x, tx_length_u, 'M', 'Q', 101 );

    // put in the specified series selector.. unless we are wildcarding
    if ( ise_series_px != NULL )
	put_click_series ( &OMmsg_x.tx_x.query_order_x.series,
			    ise_series_px );
    // the specified orderbook
    OMmsg_x.tx_x.query_order_x.orderbook_c = orderbook_u;
    // and specified bin
    OMmsg_x.tx_x.query_order_x.bin_c = bin_u;

    // note that we have not skipped any quotes yet
    *quotes_skipped_u = 0;

    // and the whose qualifier - ask for ours - our firm, my user
    put_ise_user_code_xt ( (user_code_t *)&OMmsg_x.tx_x.query_order_x.whose.trading_code,
			    &click_px->user_code_x );
    // are we querying by member ?
    if (click_px->user_member_default_u == user_member_member_e )
	// override the my-user, so we get all orders for member
	memset ( &OMmsg_x.tx_x.query_order_x.whose.trading_code.user_id_s,
		    0,
		    sizeof (OMmsg_x.tx_x.query_order_x.whose.trading_code.user_id_s));
    // NOTE leave the whose.ex_client_s as NULL. If you put anything else in there,
    // the search selects out orders with exactly that client string...

    // put in whatever order category the caller specified
    PUTSHORT ( OMmsg_x.tx_x.query_order_x.order_category_n,
		query_order_category_u );

    // save the facility code and fn code for logging
    OMmsg_x.fn_code = LOG_K_Q_ORDER;
    OMmsg_x.facility_u = click_px->facility_no_u + 7;	// EP7/ET7
	
	OMmsg_x.tx_x.query_order_x.order_select_n = order_select;
    
    /* now whiz around, doing calls until we run out of orders from the central system
	that match our query. For * (all series) all orders match, so we loop until we have
	been thru all orderbooks.

	For specific series, provided OM only packs non-matching data AFTER the series
	we asked for (and there may be none which exactly match our query),
	then we can exit the loop after processing all records in one MA101
	response IF we received any orders with non-matching series.
    */
    do // while ( orders_other_series_i == 0 )
    {
	// active orders
	status_i = do_click_query_rx ( click_px,
					&OMmsg_x,
					tx_length_u,
					LOG_K_R_ORDER);
	if ( status_i < SUCCESSFUL)
	{
	    // note no items found - bail out of loop
	    break;
	}
	else
	{
	    // good status - how many orders did we retrieve - endian
	    PUTSHORT ( items_found_u,
			click_px->log_reply_x.trans_reply_x.answer_order_x.items_n );

	    /* if we got no items, then we may have to try the next orderbook
		(bin could be split).. except if specific series, which can't be in next OB
	    */
	    if ((items_found_u == 0) && (ise_series_px))
		// none matching, and specific series
		break;

	    // loop thru the orders we found
	    for ( item_index_u = 0;
		    item_index_u < items_found_u;
		    item_index_u++ )
	    {
		// point to this chunk of info about one order
		txn_reply_px = &click_px->log_reply_x.trans_reply_x.answer_order_x
					.item[item_index_u];

		// is this a quote.. recently, MA101 seems to be including them...
		PUTSHORT ( order_category_u,
			    txn_reply_px->order_category_n );
		if ( order_category_u == ORDER_CAT_QUOTE )
		    // skipped another quote
		    *quotes_skipped_u++;
		else
		{
		    // first need to extract copy of the series, in local form
		    // we need this for seeing if this corresponds to what we queried about
		    get_click_series ( &ise_series_x,
					&txn_reply_px->series );

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

		    external_update_order ( click_px,
					    &ise_series_x,
					    txn_reply_px->order_number_u,
					    txn_reply_px->premium_i,
					    txn_reply_px->quantity_u,
					    txn_reply_px->bid_or_ask_c,
					    free_text_s,
					    txn_reply_px->time_validity_n,
					    txn_reply_px->order_type_c,
					    txn_reply_px->client_category_c,
					    txn_reply_px->ignore_away_market_c,
					    broker_oper_id_s,
					    txn_reply_px->lock_type_c,
					    txn_reply_px->order_category_n,
					    0,				// expose flag - none, not block order
					    txn_reply_px->open_close_req_c,	// a/c behaviour during OCC settlement
					    cmta_member_s,		// CMTA account
					    sub_ac_s,			// and sub account ID
					    0,				// can't get broker % in an MA101
					    txn_reply_px->trading_code
					    );

		    /* Is this record for the series we asked for ?
			If not, then don't count it as a 'found' record.
			NB if we asked for all, we will process all records
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
			orders_other_series_i++;
		    }
		    else
		    {
			// note one more found
			orders_found_i++;
		    }   // test if response series matched our initial query
		} // test if item was a quote
	    }	// for loop, processing individual inactive order records

	    /* If the Orderbook, Next field in the response is 0,
		then we have finished our query.
		
		If that field is NOT zero, then there are more queries to do,
		and we need to shuffle the Next fields into the query, and continue

		unless, of course, we were querying a specific series, and we have done everything
		for that series.. i.e. we are up to a series 'greater' than what we were looking
		for ('greater' in OM's sort order).
	    */
	    if ( click_px->log_reply_x.trans_reply_x.answer_order_x.orderbook_next_c != 0 )
	    {
		/* There is another query to go, according to MP server...
		    We must transfer the "up to here" fields from the response
		    into the next request
		*/
		OMmsg_x.tx_x.query_order_x.orderbook_c =
		    click_px->log_reply_x.trans_reply_x.answer_order_x.orderbook_next_c;
		memcpy ( &OMmsg_x.tx_x.query_order_x.series,
			    &click_px->log_reply_x.trans_reply_x.answer_order_x.series_next,
			    sizeof (series_t));
		PUTQUAD(OMmsg_x.tx_x.query_order_x.order_number_u,
		    click_px->log_reply_x.trans_reply_x.answer_order_x.order_number_next_u);
		OMmsg_x.tx_x.query_order_x.bid_or_ask_c =
		    click_px->log_reply_x.trans_reply_x.answer_order_x.bid_or_ask_next_c;
	    }	// test if we maxed out on orders
	    else
	    {
		/* There are no more orders. Stop doing queries.
		*/
		break;
	    }
	}   // test if success status from OMNIAPI call
    } while ( orders_other_series_i == 0 );

    if ( status_i >= SUCCESSFUL )
	return orders_found_i;
    else
	return status_i;

}   // query_active_order

// -----------------------------------------------------------------------

int32 query_inactive_order ( click_details_xt * click_px,
			    ise_series_xt *ise_series_px,
			    uint8 orderbook_u,
			    uint8 bin_u,
			    uint16 query_order_category_u )
/*
Function : query_inactive_order
Author	 : Neil Cossor
Modified : 20010620
Description :

    Inquires on inactive orderbook.
    Uses mq102 (inactive orders).

    As you can't actually do anything with them, we do NOT add them
    to the local list of known orders.

    In the real world, if you find ANY orders in this list,
    you should figure out whether to either
    - contact ISE to get manual reactivation of the orders done,
	which I think keeps the time-priority of the order
    - enter replacement orders into the market. These will, of course,
    definitely NOT retain priority. It must also be noted that
    as the market may have changed, the orders may require alteration
    before sending into the active Market again.

    Note that the format of the query (mq101 and mq102) were the same
    until 16.5, but the format of the responses was different.
    Under 16.5, the queries are different formats too.

Input Params:

    - pointer to all data we know about the OM / ISE central system
    - the series qualifier we are to query on - NULL if we are doing wildcard all
    - the orderbook we are to query on
    - the bin we are to query on
    - the order category we are to query on (0 = all, can't query on quotes)

Implicit params:
    - our flag if we are playing by user or by member

Output Params:

    If we get an error, we return an error status (-ve),
    otherwise we return the # of orders found (which may be 0)

    (?debug?) displays the inactive orders in some simple form.
*/
{
    // formatted string version of premium
    series_premium_st	    premium_s;
    // the query we are sending up
    generic_txn_xt	    OMmsg_x;
    // size of our actual CLICK query/command
    uint32		    tx_length_u = sizeof ( OMmsg_x.tx_x.query_inactive_x );
    // status var to return, with good default initial value
    int 		    status_i = SUCCESSFUL;
    // pointer to one interesting chunk of the response msg
    answer_inactive_ma102_item_t	*txn_reply_px;

    // count of how many orders we found which matched the caller selection
    int 		    orders_found_i = 0;
    uint16		    item_index_u,	// local loop var
			    items_found_u;	// limit on loop - may be endian-swapped from response

    // local-endian copies of fields from query response
    int32		    order_premium_u;
    uint32		    order_quantity_u;
    uint16		    order_category_u;
    uint8		    bid_ask_u;
    xref_st		    free_text_s;
    // local copy of the series for the order
    ise_series_xt	    ise_series_x;
    // and pointer to the record describing the series for the order
    ise_series_info_pt	    series_data_px;
    // pointer to uint64 as a structure, so we can dump out each half of order ID
    uint64_xt		    *uint64_pxu;


    /* set up query to have 0 in all fields - which includes selection qualifiers
	This implies
	- both bids and asks
	- all instrument groups
	- no specific order #
	- all order categories
    */
    memset( &OMmsg_x.tx_x.query_inactive_x, 0, tx_length_u );

    // specify our query type - mq102
    pack_OM_header ( &OMmsg_x, tx_length_u, 'M', 'Q', 102 );

    // put in the specified series selector.. unless we are wildcarding
    if ( ise_series_px != NULL )
	put_click_series ( &OMmsg_x.tx_x.query_inactive_x.series,
			    ise_series_px );
    // the specified orderbook
    OMmsg_x.tx_x.query_inactive_x.orderbook_c = orderbook_u;
    // and specified bin
    OMmsg_x.tx_x.query_inactive_x.bin_c = bin_u;


    // and the whose qualifier - ask for ours - our firm, my user
    put_ise_user_code_xt ( (user_code_t *)&OMmsg_x.tx_x.query_inactive_x.whose.trading_code,
			    &click_px->user_code_x );
    // are we querying by member ?
    if (click_px->user_member_default_u == user_member_member_e )
	// override the my-user, so we get all orders for member
	memset ( &OMmsg_x.tx_x.query_inactive_x.whose.trading_code.user_id_s,
		    0,
		    sizeof (OMmsg_x.tx_x.query_inactive_x.whose.trading_code.user_id_s));

    // put in whatever order category the caller specified
    PUTSHORT ( OMmsg_x.tx_x.query_inactive_x.order_category_n,
		query_order_category_u );

    // save the facility code and fn code for logging
    OMmsg_x.fn_code = LOG_K_Q_ORDER;
    OMmsg_x.facility_u = click_px->facility_no_u + 7;	// EP7/ET7

    
    // now whiz around, doing calls until we run out of orders from the central system, and break
    do
    {
	// inactive orders
	status_i = do_click_query_rx ( click_px,
					&OMmsg_x,
					tx_length_u,
					LOG_K_R_INACTIVE );
	if ( status_i < SUCCESSFUL)
	{
	    // note no items found - bail out of loop
	    break;
	}
	else
	{
	    // good status - how many orders did we retrieve - endian
	    PUTSHORT ( items_found_u,
			click_px->log_reply_x.trans_reply_x.answer_inactive_x.items_n );

	    /* if we got no items, then we may have to try the next orderbook
		(bin could be split).. except if specific series, which can't be in next OB
	    */
	    if ((items_found_u == 0) && (ise_series_px))
		// none matching, and specific series
		break;

	    // loop thru the orders we found
	    for ( item_index_u = 0;
		    item_index_u < items_found_u;
		    item_index_u++ )
	    {
		// extract one order - in correct endian order
		// NOTE there is no orderID in an inactive order

		// point to this chunk of info about one order
		txn_reply_px = &click_px->log_reply_x.trans_reply_x.answer_inactive_x
					.item[item_index_u];

		PUTLONG ( order_premium_u,
			    txn_reply_px->premium_i );
		PUTLONG ( order_quantity_u,
			    txn_reply_px->quantity_u );
		PUTSHORT ( order_category_u,
			    txn_reply_px->order_category_n );
		bid_ask_u = txn_reply_px->bid_or_ask_c;
		rx_click_string ( &free_text_s,
				    (char *)txn_reply_px->free_text_s,
				    sizeof (txn_reply_px->free_text_s) );
				    
		// report the inactive order

		// grab (in local format) the binary series description
		get_click_series ( &ise_series_x,
				    &txn_reply_px->series );
		// find the series for the order
		series_data_px = find_series_by_ise_series ( click_px,
								&ise_series_x );

		// format the premium, with correct implied decimals
		format_whole_decimal_s ( (char *)&premium_s,
					    order_premium_u,
					    series_data_px->instrument_class_px->dec_in_premium_u,
					    10 );   // width of 10
		// point structure pointer at the 64bit order ID
		(uint64 *)uint64_pxu = (uint64 *)&(txn_reply_px->order_number_u);

		printf ("%-15s %-4s %-17s %6d@%s %-8s %s, orderID %08lx:%08lx\n",
			order_category_to_string ( order_category_u ),
			bid_ask_to_string (bid_ask_u),
			&series_data_px->ise_name_s, 
			order_quantity_u,
			&premium_s,
			&free_text_s,
			client_category_to_string ( txn_reply_px->client_category_c ),
			uint64_pxu->l1,
			uint64_pxu->l0
			);

		// note one more found
		orders_found_i++;
	    }	// for loop, processing inactive orders

	    /* If the Orderbook, Next field in the response is 0,
		then we have finished our query.
		
		If that field is NOT zero, then there are more queries to to,
		and we need to shuffle the Next fields into the query, and continue
	    */
	    if ( click_px->log_reply_x.trans_reply_x.answer_inactive_x.orderbook_next_c != 0 )
	    {
		/* There is another query to go, according to MP server...
		    We must transfer the "up to here" fields from the response
		    into the next request
		*/
		OMmsg_x.tx_x.query_inactive_x.orderbook_c =
		    click_px->log_reply_x.trans_reply_x.answer_inactive_x.orderbook_next_c;
		memcpy ( &OMmsg_x.tx_x.query_inactive_x.series,
			    &click_px->log_reply_x.trans_reply_x.answer_inactive_x.series_next,
			    sizeof (series_t));
		PUTQUAD(OMmsg_x.tx_x.query_inactive_x.order_number_u,
		    click_px->log_reply_x.trans_reply_x.answer_inactive_x.order_number_next_u);
		OMmsg_x.tx_x.query_inactive_x.bid_or_ask_c =
		    click_px->log_reply_x.trans_reply_x.answer_inactive_x.bid_or_ask_next_c;
	    }	// test if we maxed out on orders
	    else
	    {
		/* There are no more orders. Stop doing queries.
		*/
		break;
	    }
	}   // test if success status from OMNIAPI call
    } while ( TRUE );

    printf ("Found %d inactive orders matching query selection\n\n",
		orders_found_i );

    if ( status_i >= SUCCESSFUL )
	return orders_found_i;
    else
	return status_i;

}   // query_inactive_order

// ---------------------------------------------------------------------------

int32 query_orders ( click_details_xt * click_px,
			int argc,
			char *argv[] )
/*
Function:   query_orders
Author:     Neil Cossor
Modified:   20000521
Description:

    Go find out about orders from the system orderbook.
    If we are inquiring on active orders, then update the local orderbook.

    Get the subset qualifier by the argc/argv parameters

    This was triggered by a 
    GET,<ACTIVE|INACTIVE>,<*|S>[,<series name>]

Input Params:

    Standard argc/argv of the command line.

    argv[0]	    GET
    argv[1]	    Active|Inactive
	#IF ACTIVE
	argv[2] = Order Selection
    argv[3] may have values of
    - "*"	    all orders in all series (and hence all underlying)
    - Series	    specifies that argv[3] is a series name,
		    i.e. collect all orders in that particular series
    argv[4]	    <series name>
	#ELSE
	argv[2] may have values of
    - "*"	    all orders in all series (and hence all underlying)
    - Series	    specifies that argv[3] is a series name,
		    i.e. collect all orders in that particular series
    argv[3]	    <series name>
	
	

Output Params:

    If collecting active orders. updates the local orderbook
    If not, then displays the inactive orders.
*/
{
    // pointer to selected series
    ise_series_info_pt	series_data_px = NULL;
    // pointer to Q'ed block, which contains pointer to series under selected underlying
    gen_buf_node_xt	*series_node_px = NULL;
    // pointer to selected underlying
    ise_underlying_xt	*under_data_px = NULL;
    // active or inactive
    int 		inactive_u;
    // return status from lower level calls
    int 		status_i;
    // temp var, for selecting from possible options on cmd line
    int 		choice_i;
    // the order category we wish to query on. Default 0 (all categories)
    uint16		order_category_u = 0;
    // the orderbook # we are up to.. needed for querying orders in all bins
    uint8		orderbook_u = 0;
    // how many quotes we skipped in the response data
    uint32		quotes_skipped_u;
	// order_selection
	uint16_t order_select = 0;

	char insufficientArgs[] = "\tInsufficient args    GET,Active,<Order Selection>,*|Series[,<seriesname>]\n";
	char * series_name;
	char * series_type; //points to *|series argv
    if ( !click_px->logged_on_i )
    {
	prompt_logged_on ( "Query order" );
	return NOT_LOGGED_ON;
    }
	
    // were all the required parameters supplied
    // need 3 args, unless wildcard (series)
	// now check what the selection criteria was, and refine query based on that
	if (argc <= 1) {
		printf(insufficientArgs);
		return SUCCESSFUL;
	}
	switch ( *argv[1] )
	{
	case  'A':
		if (!(argc == 4 && argv[3][0] == '*') && argc != 5) {
			printf (insufficientArgs);
			return SUCCESSFUL;
		}
	    inactive_u = FALSE;
		choice_i = select_string_option ( 2, &under_series_s [0], argv[3] );
		series_name = argv[4];
		series_type = argv[3];

	    break;
	case  'I':
		if (argc != 4 || (argc != 3 && argv[2][0] != '*')) {
			printf ("\tInsufficient args    GET,Inactive,*|Series[,<seriesname>]\n");
			return SUCCESSFUL;
		}
		inactive_u = TRUE;
		series_name = argv[3];
		series_type = argv[2];
		choice_i = select_string_option ( 2, &under_series_s [0], argv[2] );

		break;
	default:
		// illegal selection qualifier
		printf ("\tERROR - Illegal qualifier %s in GET..[ACTIVE|INACTIVE]\n",
			    argv[1] );
		return SUCCESSFUL;
	}   // end of switch

	
	// and which series we wish to work on...
	switch ( choice_i )
	{
	case under_series_series_e :
	    // all orders in series supplied in argv[2]
	    // go find the series info for that series, and specify that
	    series_data_px = find_series_by_name_s ( click_px,
							series_name );
	    if (!series_data_px)
	    {
		// oops - did NOT know about that series
		printf ("\tERROR - Unknown Series [%s] in selection for query_order.\n",
			    series_name );
		return SUCCESSFUL;
	    }

	    // we know that all series have an underlying.. so we MUST have found the underlying

	    if (inactive_u)
		// query inactive orders, telling it we want specific series
		status_i = query_inactive_order ( click_px,
						    &series_data_px->ise_series_x,  // series qualifier,
						    series_data_px->parent_under_px->orderbook_u,
						    series_data_px->parent_under_px->bin_u,
						    order_category_u );
		else {
			order_select = cmd_order_select(argv[2]);
			if (order_select == 250) {
				//already printed error.  Bail.
				return SUCCESSFUL;
			}
			// query active orders, telling it we want specific series
			status_i = query_active_order ( click_px,
							&series_data_px->ise_series_x,  // series qualifier,
							series_data_px->parent_under_px->orderbook_u,
							series_data_px->parent_under_px->bin_u,
							order_category_u,
							&quotes_skipped_u,
							order_select);
			// bail out if we get an error
		}
	    if (status_i >= SUCCESSFUL)
	    {
		printf ("Found %d orders in %s\n",
			    status_i,
			    &series_data_px->ise_name_s );
	    }

	    return status_i;

	    break;  // redundant, after return

	case under_series_all_e :
	    // all orders in all series please
	    // clear out known orders first..
	    clear_local_orderbook ( click_px );

	    if (inactive_u)
		// query inactive orders, telling it we want all series
		// orderbook next field in response rolls across OBs
		status_i = query_inactive_order ( click_px,
						    NULL,		// no series supplied
						    1,			// orderbook # to query, start with 1
						    0,			// and all bins in that orderbook
						    order_category_u
						    );
		else {
			// query active orders, telling it we want all series
			// orderbook next field in response rolls across OBs
			order_select = cmd_order_select(argv[2]);

			status_i = query_active_order ( click_px,
								NULL,		// no series supplied
								1,			// orderbook # to query, start with 1
								0,			// and all bins in that orderbook
								order_category_u,
								&quotes_skipped_u,
								order_select
								);
		}
	    // did we find any orders ?
	    if (status_i >= SUCCESSFUL)
	    {
		printf ("Found %d orders, skipped %u quotes\n",
			    status_i,
			    quotes_skipped_u );
	    }

	    return status_i;

	    break;  // redundant, after return

	default:
	    {
		// illegal selection qualifier
		printf ("\tERROR - Illegal qualifier %s in selection for query_active_order..[*|Series]\n",
			    series_type );
		return SUCCESSFUL;
	    }
	}   // end of switch
}   // query_orders

// -----------------------------------------------------------------------

static int32 send_block_order (click_details_xt * click_px,
				gen_buf_node_xt *order_node_px )    
/*
Function : send_block_order
Author	 : Neil Cossor
Modified : 20010620
Description:

    For the supplied order information, sends a block order to the system
    Uses mo107

    If the order is accepted, then we update the orderID.
*/
{
    // local buffer, containing actual msg we send
    generic_txn_xt	OMmsg_x;
    // pointer to the actual order data
    order_entry_xt	*order_data_px;

    int32		status_i	    =	SUCCESSFUL;
    // size of our actual CLICK query/command
    uint32		tx_length_u = sizeof ( OMmsg_x.tx_x.block_trans_x );


    // point to the order data block
    order_data_px = (order_entry_xt *)order_node_px->msg_p;

    pack_OM_header ( &OMmsg_x, tx_length_u, 'M', 'O', 107);
    // save the facility code and fn code (for the logger, and generic send routine)
    OMmsg_x.fn_code = LOG_K_C_BLOCK_TRANS;
    OMmsg_x.facility_u = click_px->facility_no_u + 7;	// EP7/ET7

    put_click_series ( &OMmsg_x.tx_x.block_trans_x.series,
			&order_data_px->series_px->ise_series_x );

    // now we can specify orderbook_c
    OMmsg_x.tx_x.block_trans_x.orderbook_c = order_data_px->series_px->parent_under_px->orderbook_u;
    // endian fields
    PUTLONG ( OMmsg_x.tx_x.block_trans_x.premium_i,
		order_data_px->premium_i );
    PUTLONG ( OMmsg_x.tx_x.block_trans_x.quantity_u,
		order_data_px->quantity_u );
    // validity time - GTC / ROD / IOC / FOK
    PUTSHORT ( OMmsg_x.tx_x.block_trans_x.time_validity_n,
		order_data_px->validity_time_u );
    // bid / ask
    OMmsg_x.tx_x.block_trans_x.bid_or_ask_c = order_data_px->bid_ask_u;
    // EAM/FIRM/FARMM
    OMmsg_x.tx_x.block_trans_x.client_category_c =
	    order_data_px->client_category_u;
    OMmsg_x.tx_x.block_trans_x.open_close_req_c = order_data_px->open_close_u;

    // put in the two parts of the client ID, which are actually the CMTA a/c and sub a/c IDs
    tx_click_string ( &OMmsg_x.tx_x.block_trans_x.ex_client_s [0],
			&order_data_px->cmta_member_s,
			MAX_CMTA_MEMBER );
    tx_click_string ( &OMmsg_x.tx_x.block_trans_x.ex_client_s [MAX_CMTA_MEMBER],
			&order_data_px->sub_ac_s,
			MAX_SUB_ACCOUNT_ID );
    // need to set the broker_id_s
    tx_click_string ( &OMmsg_x.tx_x.block_trans_x.broker_id_s,
			&order_data_px->broker_oper_id_s,
			sizeof (OMmsg_x.tx_x.block_trans_x.broker_id_s));
    // the xref
    tx_click_string ( &OMmsg_x.tx_x.block_trans_x.free_text_s[0],
		     order_data_px->userXRef_s, 
		     sizeof (OMmsg_x.tx_x.block_trans_x.free_text_s ) );
    // the expose flag
    OMmsg_x.tx_x.block_trans_x.expose_flag_c = order_data_px->expose_flag_u;

    status_i = do_click_tx_rx ( click_px,
				    &OMmsg_x,
				    tx_length_u );
    if ( status_i < SUCCESSFUL)
    {
	// do nothing, error already reported by do_click_tx_rx
    }
    else
    {
	click_px->maxLocalOrderNumber_u++;
	// update the order ID of the order
	PUTQUAD ( order_data_px->orderID_u,
		    click_px->log_reply_x.OrderIdent_u );
	// and the 'local' order number of the order
	order_data_px->localNumber_n = click_px->maxLocalOrderNumber_u;

	printf ( "Local Block Order Number: %ld\n",
		     click_px->maxLocalOrderNumber_u );
    }

    // save this order into last_order_x, and note the customer and member
    snapshot_last_order ( click_px, order_data_px );

    return (status_i);
}   // send_block_order

// ---------------------------------------------------------------------------

int32 generate_block_order ( click_details_xt * click_px,
			    int argc,
			    char *argv[] )
/*
Function:   generate_block_order
Author:     Neil Cossor
Modified:   20000628
Description:

    Client wishes to send a block order
    This is sent with an MO107, done by calling send_block_order

    This is similar to an ordinary order (in params etc)
    EXCEPT
    there is NO order_type ( limit, market)
    there is NO ignore-away-market flag
    BUT
    there IS and exposure flag

    If the order is accepted, then we add the order into the
    known order Q, otherwise we release the (local) order block.

    It is triggered by a BLOCK command

Input Params:

    argv[0]	"BLOCK"
    argv[1]	"BID"|"ASK"
    argv[2]	series name
    argv[3]	quantity
    argv[4]	premium
    argv[5]	X-reference == freetext
    (optionally)
    argv[6]	validity time.. G[tc] / R[od] / I[oc] / F[ok], default IOC
    argv[7]	client category.. C[ustomer] | FI[rm\ | FA[rmm], default Client
    argv[8]	expose flag, default 0 (show nothing)
    argv[9]	broker ID (i.e. operator), default of this user
    argv[10]	open/close setting for OCC account, default is "use default for a/c"
    argv[11]	CMTA account, default is ""
    argv[12]	" sub account, default is ""
*/
{
    int     status_i = SUCCESSFUL;

    // pointer to the Q'able block, which we will contain a ptr to actual order
    gen_buf_node_xt	*order_node_px;
    // pointer to the actual order data
    order_entry_xt	*order_data_px;
    // pointer to relevant series
    ise_series_info_xt	*series_data_px = NULL;

    // basic data for transaction - defaulted, or converted from argv params
    int32		order_premium_u;
    uint16		order_quantity_u;
    xref_st		order_xref_s;
    uint16		validity_time_u;
    uint8		client_category_u;  // cust/Firm/FARMM
    uint8		bid_ask_u;	    // if the order is bid or ask
    uint8		expose_flag_u;	    // mask of what to show to MMs etc
    uint32		temp_u; 	    // temp, used for tfering values from cmd line
    user_id_st		*broker_oper_id_ps; // pointer to broker/operator ID
    uint8		open_close_u;	    // OCC settlement setting
    char		*cmta_member_ps;    // pointer to CMTA member ID string
    char		*sub_ac_ps;    // pointer to sub account ID string

    if ( !click_px->logged_on_i )
    {
	prompt_logged_on ( "Block" );
	return NOT_LOGGED_ON;
    }

    // have we enuff arguments - minimum of 6
    if ( argc < 6)
    {
	// insufficient arguments - prompt the user, and return
	prompt_block_command ();
	return SUCCESSFUL;
    }

   // we have the minumum argument list

    // what did the command select re BID vs ASK
    bid_ask_u = cmd_bid_ask ( click_px, argv[1] );
    // legal selection ?
    if ( bid_ask_u > 2 )
    {
	printf ("Illegal block order type qualifier - BID | ASK\n");
	return SUCCESSFUL;
    }

    // series name in argv[2] - use that directly in search for ise_series_info_xt
    series_data_px = user_selected_series ( click_px, argv[2] );
    if (series_data_px == NULL)
    {
	// couldn't find that series - error already reported - bail out
	return SUCCESSFUL;
    }

    // Read the order quantity
    order_quantity_u = (uint16)read_unsigned_limited (argv [3], UNDEFINED_UINT16 );

    // Read the order premium
    order_premium_u = read_decimal_string_int ( argv[4],
				series_data_px->instrument_class_px->dec_in_premium_u );

    // And finally, see if there is an order xref
    strncpy(order_xref_s, argv[5], sizeof (order_xref_s));

    // optional parameters

    // validity time
    if ( argc >= 7 )
    {
	validity_time_u = cmd_validity_time ( argv[6], VALIDITY_TIME_IOC );
	if ( validity_time_u == UNDEFINED_UINT16 )
	{
	    // illegal validity time selector - reported, just return to called
	    return SUCCESSFUL;
	}
    }
    else
	// default to IOC
	validity_time_u = cmd_validity_time ( "", VALIDITY_TIME_IOC );

    // client category..
    // NB for block orders, FARMM is NOT allowed.. let central system reject it
    if ( argc >= 8 )
    {
	client_category_u = cmd_client_category ( argv[7], CUST_CATEGORY_CUSTOMER );
    }
    else
	// default to client category
	client_category_u = CUST_CATEGORY_CUSTOMER;


    // expose flag
    if ( argc >= 9 )
    {
	/* customer specified an exposure flag.
	    This is a mask (ref DFI)
	    1 == expose premium
	    2 == expose quantity
	    4 == expose bid/ask
	    8 == expose validity time
	    16 == expose broker flag
	*/
	// For now, read it as a number
	    temp_u = (uint8) read_unsigned_limited ( argv [8], UNDEFINED_UINT8 );
	    if ( temp_u > UNDEFINED_UINT8 )
	    {
		printf ("Illegal value for expose-mask, max value 255\n");
		return SUCCESSFUL;
	    }
	    // if we got here, reasonable value for expose flag
	    expose_flag_u = temp_u;
    }
    else
	// default to expose nothing
	expose_flag_u = 0;

    // broker / operator ID string
    if ( argc >= 10 )
	// customer specified the broker ID
	(char *)broker_oper_id_ps = argv[9];
    else
	// customer did not specify a broker ID - use our own
	broker_oper_id_ps = &click_px->user_code_x.customer_id_s;

    // open/close behaviour for OCC account during settlement
    if ( argc >= 11 )
	// customer specified a value for open/close
	open_close_u = read_unsigned_limited ( argv[10], UNDEFINED_UINT8 );
    else
	open_close_u = OPEN_CLOSE_REQUEST_DEFAULT;

    // see if supplied CMTA account
    if ( argc >= 12 )
	// customer specified a value for CMTA member ID
	cmta_member_ps = argv[11];
    else
	// if non specified, no default (most trades are not transferred via CMTA to other OCC firms)
	cmta_member_ps = "";

    // see if supplied CMTA sub account
    if ( argc >= 13 )
	// customer specified a value for CMTA sub account ID
	sub_ac_ps = argv[12];
    else
	/* if non specified, no default (normally only trades from MMs have a sub-account ID,
	    and all MM users have a default sub-account ID in CDB
	*/
	sub_ac_ps = "";


    // create an order_entry block. No Order ID yet...
    order_node_px = create_order_entry ( ++click_px->maxLocalOrderNumber_u );
    // point to the order data block of what we just got
    order_data_px = (order_entry_xt *)order_node_px->msg_p;
    update_order ( click_px,
		    order_data_px,
		    series_data_px,
		    click_px->null_orderID_u,
		    order_premium_u,
		    order_quantity_u,
		    bid_ask_u,
		    order_xref_s,
		    validity_time_u,
		    0,			// limit-market irrelevant in block order
		    client_category_u,
		    0,			// ignore-away irrelevant in block order    
		    (char *)broker_oper_id_ps,
		    0,			// lock type - none, not locked (yet)
		    ORDER_CAT_BLOCK,
		    expose_flag_u,	// exposure flag to other BDs
		    open_close_u,	// a/c behaviour during OCC settlement
		    cmta_member_ps,	// CMTA account
		    sub_ac_ps,	// and sub account ID
		    0			// no broker % in a block order
		);

    // and attempt to send it
    status_i = send_block_order ( click_px,
				    order_node_px );

    // if order went in OK, then add it to the Q of known orders
    if (status_i >= SUCCESSFUL)
    {
	// optional debug / on-screen report
	display_one_order ( order_data_px );

	add_to_q_tail ( &click_px->our_market_x.known_orders_q,
			&order_node_px->chain );
    }
    else
    {
	// error already reported

	// release the memory allocated to the order
	free_gen_buf_node ( order_node_px );
	click_px->maxLocalOrderNumber_u--;
    }

    return status_i;
}   // generate_block_order

// -----------------------------------------------------------------------

static int32 send_facilitation_order (click_details_xt	* click_px,
					gen_buf_node_xt *order_node_px )    
/*
Function : send_facilitation_order
Author	 : Neil Cossor
Modified : 20010620
Description:

    For the supplied order information, sends a facilitation order to the system
    Uses mo108

    If the order is accepted, then we update the orderID.
*/
{
    // local buffer, containing actual msg we send
    generic_txn_xt	OMmsg_x;
    // pointer to the actual order data
    order_entry_xt	*order_data_px;

    int32		status_i	    =	SUCCESSFUL;
    // size of our actual CLICK query/command
    uint32		tx_length_u = sizeof ( OMmsg_x.tx_x.facilitation_trans_x );


    // point to the order data block
    order_data_px = (order_entry_xt *)order_node_px->msg_p;

    pack_OM_header ( &OMmsg_x, tx_length_u, 'M', 'O', 108);
    // save the facility code and fn code (for the logger, and generic send routine)
    OMmsg_x.fn_code = LOG_K_C_FACILITATION_TRANS;
    OMmsg_x.facility_u = click_px->facility_no_u + 7;	// EP7/ET7

    put_click_series ( &OMmsg_x.tx_x.facilitation_trans_x.series,
			&order_data_px->series_px->ise_series_x );

    // now we can specify orderbook_c
    OMmsg_x.tx_x.facilitation_trans_x.orderbook_c = order_data_px->series_px->parent_under_px->orderbook_u;
    // endian fields
    PUTLONG ( OMmsg_x.tx_x.facilitation_trans_x.premium_i,
		order_data_px->premium_i );
    PUTLONG ( OMmsg_x.tx_x.facilitation_trans_x.originating_side.quantity_u,
		order_data_px->quantity_u );
    // unlike other order types - no validity time

    // bid / ask
    OMmsg_x.tx_x.facilitation_trans_x.originating_side.bid_or_ask_c = order_data_px->bid_ask_u;
    // EAM/FIRM/FARMM
    OMmsg_x.tx_x.facilitation_trans_x.originating_side.client_category_c =
	    order_data_px->client_category_u;
    OMmsg_x.tx_x.facilitation_trans_x.originating_side.open_close_req_c = order_data_px->open_close_u;

    // put in the two parts of the client ID, which are actually the CMTA a/c and sub a/c IDs
    tx_click_string ( &OMmsg_x.tx_x.facilitation_trans_x.originating_side.ex_client_s [0],
			&order_data_px->cmta_member_s,
			MAX_CMTA_MEMBER );
    tx_click_string ( &OMmsg_x.tx_x.facilitation_trans_x.originating_side.ex_client_s [MAX_CMTA_MEMBER],
			&order_data_px->sub_ac_s,
			MAX_SUB_ACCOUNT_ID );
    // need to set the broker_id_s
    tx_click_string ( &OMmsg_x.tx_x.facilitation_trans_x.originating_side.broker_id_s,
			&order_data_px->broker_oper_id_s,
			sizeof (OMmsg_x.tx_x.facilitation_trans_x.originating_side.broker_id_s));
    // the xref
    tx_click_string ( &OMmsg_x.tx_x.facilitation_trans_x.originating_side.free_text_s[0],
		     order_data_px->userXRef_s, 
		     sizeof (OMmsg_x.tx_x.facilitation_trans_x.originating_side.free_text_s ) );
    // and there is a second open-or-close, for the EAM flag
    OMmsg_x.tx_x.facilitation_trans_x.counter_party_side.open_close_req_c = order_data_px->open_close_u;
    // put in the broker % field
    OMmsg_x.tx_x.facilitation_trans_x.counter_party_side.broker_percentage_c = order_data_px->broker_percentage_u;
    // put in the expose flag
    OMmsg_x.tx_x.facilitation_trans_x.expose_flag_c = order_data_px->expose_flag_u;

    status_i = do_click_tx_rx ( click_px,
				    &OMmsg_x,
				    tx_length_u );
    if ( status_i < SUCCESSFUL)
    {
	// do nothing, error already reported by do_click_tx_rx
    }
    else
    {
	click_px->maxLocalOrderNumber_u++;
	// update the order ID of the order
	PUTQUAD ( order_data_px->orderID_u,
		    click_px->log_reply_x.OrderIdent_u );
	// and the 'local' order number of the order
	order_data_px->localNumber_n = click_px->maxLocalOrderNumber_u;

	printf ("Local Facilitation Order Number: %ld\n",
		 click_px->maxLocalOrderNumber_u );
    }

    // save this order into last_order_x, and note the customer and member
    snapshot_last_order ( click_px, order_data_px );

    return (status_i);
}   // send_facilitation_order

// ---------------------------------------------------------------------------

int32 generate_facilitation_order ( click_details_xt * click_px,
				    int argc,
				    char *argv[] )
/*
Function:   generate_facilitation_order
Author:     Neil Cossor
Modified:   20000601
Description:

    Client wishes to send a facilitation order
    This is sent with an MO108, by calling send_facilitation_order

    This is similar to an ordinary order (in params etc)
    EXCEPT
    there is NO order_type ( limit, market)
    there is NO ignore-away-market flag
    there is no validity time
    BUT
    there IS an open_close_req value for the EAM
    (as distinct from the value for the facilitating broker)
    and a broker percentage and expose flag (for the broker percentage)

    If the order is accepted, then we add the order into the
    known order Q, otherwise we release the (local) order block.

    It is triggered by a FACILITATION command

Input Params:

    argv[0]	"FACILITATION"
    argv[1]	"BID"|"ASK"
    argv[2]	series name
    argv[3]	quantity
    argv[4]	premium
    argv[5]	X-reference == freetext
    (optionally)
    argv[6]	client category.. C[ustomer] | FI[rm\ | FA[rmm], default Client
    argv[7]	broker ID (i.e. operator), default of this user
    argv[8]	open/close setting for OCC account, default is "use default for a/c"
    argv[9]	CMTA account, default is ""
    argv[10]	" sub account, default is ""
    argv[11]	broker percentage, default 
    argv[12]	expose_flag, default is expose
*/
{
    int     status_i = SUCCESSFUL;

    // pointer to the Q'able block, which we will contain a ptr to actual order
    gen_buf_node_xt	*order_node_px;
    // pointer to the actual order data
    order_entry_xt	*order_data_px;
    // pointer to relevant series
    ise_series_info_xt	*series_data_px = NULL;

    // basic data for transaction - defaulted, or converted from argv params
    int32		order_premium_u;
    uint16		order_quantity_u;
    xref_st		order_xref_s;
    uint8		client_category_u;  // cust/Firm/FARMM
    uint8		bid_ask_u;	    // if the order is bid or ask
    user_id_st		*broker_oper_id_ps; // pointer to broker/operator ID
    series_premium_st	premium_s;	    // premium, in string form
    uint8		open_close_u;	    // OCC settlement setting
    char		*cmta_member_ps;    // pointer to CMTA member ID string
    char		*sub_ac_ps;    // pointer sub account ID string
    uint8		broker_percentage_u;// what broker percentage the caller specified
    uint8		expose_flag_u;	    // expose flag.. 0 and 16 are only legal vals for facilitation

    if ( !click_px->logged_on_i )
    {
	prompt_logged_on ( "Facilitation" );
	return NOT_LOGGED_ON;
    }

    // have we enuff arguments - minimum of 6
    if ( argc < 6)
    {
	// insufficient arguments - prompt the user, and return
	prompt_facilitation_command ();
	return SUCCESSFUL;
    }

	// we have the minumum argument list

    // what did the command select re BID vs ASK
    bid_ask_u = cmd_bid_ask ( click_px, argv[1] );
    // legal selection ?
    if ( bid_ask_u == 3 )
    {
	printf ("Illegal facilitation order type qualifier - Bid | Ask\n");
	return SUCCESSFUL;
    }

    // series name in argv[2] - use that directly in search for ise_series_info_xt
    series_data_px = user_selected_series ( click_px, argv[2] );
    if (series_data_px == NULL)
    {
	// couldn't find that series - error already reported - bail out
	return SUCCESSFUL;
    }

    // Read the order quantity
    order_quantity_u = (uint16)read_unsigned_limited (argv [3], UNDEFINED_UINT16);

    // Read the order premium
    order_premium_u = read_decimal_string_int ( argv[4],
				series_data_px->instrument_class_px->dec_in_premium_u );

    // And finally, see if there is an order xref
    strncpy(order_xref_s, argv[5], sizeof (order_xref_s));

    // optional parameters

    // client category
    if ( argc >= 7 )
    {
	client_category_u = cmd_client_category ( argv[6], CUST_CATEGORY_CUSTOMER );
    }
    else
	// default to client category
	client_category_u = CUST_CATEGORY_CUSTOMER;

    // broker / operator ID string
    if ( argc >= 8 )
	// customer specified the broker ID
	(char *)broker_oper_id_ps = argv[7];
    else
	// customer did not specify a broker ID - use our own
	broker_oper_id_ps = &click_px->user_code_x.customer_id_s;

    // open/close behaviour for OCC account during settlement
    if ( argc >= 9 )
	// customer specified a value for open/close
	open_close_u = read_unsigned_limited ( argv[8], UNDEFINED_UINT8 );
    else
	open_close_u = OPEN_CLOSE_REQUEST_DEFAULT;

    // see if supplied CMTA account
    if ( argc >= 10 )
	// customer specified a value for CMTA member ID
	cmta_member_ps = argv[9];
    else
	// if non specified, no default (most trades are not transferred via CMTA to other OCC firms)
	cmta_member_ps = "";

    // see if supplied CMTA sub account
    if ( argc >= 11 )
	// customer specified a value for CMTA sub account ID
	sub_ac_ps = argv[10];
    else
	/* if non specified, no default (normally only trades from MMs have a sub-account ID,
	    and all MM users have a default sub-account ID in CDB
	*/
	sub_ac_ps = "";

    // see if supplied broker percentage
    if ( argc >= 12 )
	// customer supplied a value for broker percentage - real limit should be 100, but test BE validation
	broker_percentage_u = read_unsigned_limited ( argv[11], UNDEFINED_UINT8 );
    else
	// non specified, put in default
	broker_percentage_u = 100;

    // see if supplied expose_flag
    if ( argc >= 13 )
	// customer supplied a value for expose_flag
	/* customer specified an exposure flag.
	    This is a mask (ref DFI).. only two values are allowed for facilitation,
	    0 == hide everything
	    16 == expose broker percentage

	    For now, read it as a number
	*/
	expose_flag_u = read_unsigned_limited ( argv [12], UNDEFINED_UINT8 );
    else
	// no value supplied, set default - hide everything i.e. the broker percentage
	expose_flag_u = 0;

    // create an order_entry block. No Order ID yet...
    order_node_px = create_order_entry ( ++click_px->maxLocalOrderNumber_u );
    // point to the order data block of what we just got
    order_data_px = (order_entry_xt *)order_node_px->msg_p;
    update_order ( click_px,
		    order_data_px,
		    series_data_px,
		    click_px->null_orderID_u,
		    order_premium_u,
		    order_quantity_u,
		    bid_ask_u,
		    order_xref_s,
		    0,			// validity time - irrelevant in facilitation order
		    0,			// limit/market irrelevant in Facil order
		    client_category_u,
		    0,			// ignore away irrelevant in Facil order
		    (char *)broker_oper_id_ps,
		    0,			// lock type - none, not locked
		    ORDER_CAT_FACILITATION,
		    expose_flag_u,	// expose flag
		    open_close_u,	// a/c behaviour during OCC settlement
		    cmta_member_ps,	// CMTA account
		    sub_ac_ps,	// and sub account ID
		    broker_percentage_u	// um.. guess.. broker percentage
		);

    // format the premium, with correct implied decimals
    format_whole_decimal_s ( (char *)&premium_s,
			order_premium_u,
			series_data_px->instrument_class_px->dec_in_premium_u,
			MAX_SERIES_PREMIUM );	// width

    printf ( "Facilitation %s %d %s for %s\n",
	     bid_ask_to_string ( bid_ask_u ),
	     order_quantity_u,
	     &series_data_px->ise_name_s,
	     &premium_s );

    // and attempt to send it
    status_i = send_facilitation_order ( click_px,
					order_node_px );


    // if order went in OK, then add it to the Q of known orders
    if (status_i >= SUCCESSFUL)
	add_to_q_tail ( &click_px->our_market_x.known_orders_q,
			    &order_node_px->chain );
    else
    {
	// release the memory allocated to the order
	free_gen_buf_node ( order_node_px );
	click_px->maxLocalOrderNumber_u--;
	// error already reported
    }

    return status_i;
}   // generate_facilitation_order

// -----------------------------------------------------------------------

static int32 send_block_facil_response ( click_details_xt   * click_px,
					    gen_buf_node_xt *order_node_px,
					    uint16 order_category_u,
					    quad_word ref_orderID_u )    
/*
Function : send_block_facil_response
Author	 : Neil Cossor
Modified : 20010620
Description:

    For the supplied order information, sends a block/facilitaion response
    order to the system.
    Uses mo109

    If the order is accepted, then we update the orderID.
*/
{
    // local buffer, containing actual msg we send
    generic_txn_xt	OMmsg_x;
    // pointer to the actual order data
    order_entry_xt	*order_data_px;

    int32		status_i	    =	SUCCESSFUL;
    // size of our actual CLICK query/command
    uint32		tx_length_u = sizeof ( OMmsg_x.tx_x.block_facil_response_x );


    // point to the order data block
    order_data_px = (order_entry_xt *)order_node_px->msg_p;

    pack_OM_header ( &OMmsg_x, tx_length_u, 'M', 'O', 109);
    // save the facility code and fn code (for the logger, and generic send routine)
    OMmsg_x.fn_code = LOG_K_C_BLOCK_TRANS;
    OMmsg_x.facility_u = click_px->facility_no_u + 7;	// EP7/ET7

    put_click_series ( &OMmsg_x.tx_x.block_facil_response_x.series,
			&order_data_px->series_px->ise_series_x );

    // now we can specify orderbook_c
    OMmsg_x.tx_x.block_facil_response_x.orderbook_c = order_data_px->series_px->parent_under_px->orderbook_u;
    // endian fields
    PUTLONG ( OMmsg_x.tx_x.block_facil_response_x.premium_i,
		order_data_px->premium_i );
    PUTLONG ( OMmsg_x.tx_x.block_facil_response_x.quantity_u,
		order_data_px->quantity_u );
    // the reference orderID i.e. order # of active block/facilitation order we are responding to
    PUTQUAD(OMmsg_x.tx_x.block_facil_response_x.order_number_u,
		ref_orderID_u);
    // bid / ask
    OMmsg_x.tx_x.block_facil_response_x.bid_or_ask_c = order_data_px->bid_ask_u;
    // open/close flag
    OMmsg_x.tx_x.block_facil_response_x.open_close_req_c = order_data_px->open_close_u;
    // order category
    PUTSHORT ( OMmsg_x.tx_x.block_facil_response_x.order_category_n,
		order_category_u );

    // put in the two parts of the client ID, which are actually the CMTA a/c and sub a/c IDs
    tx_click_string ( &OMmsg_x.tx_x.block_facil_response_x.ex_client_s [0],
			&order_data_px->cmta_member_s,
			MAX_CMTA_MEMBER );
    tx_click_string ( &OMmsg_x.tx_x.block_facil_response_x.ex_client_s [MAX_CMTA_MEMBER],
			&order_data_px->sub_ac_s,
			MAX_SUB_ACCOUNT_ID );
    // need to set the broker_id_s
    tx_click_string ( &OMmsg_x.tx_x.block_facil_response_x.broker_id_s,
			&order_data_px->broker_oper_id_s,
			sizeof (OMmsg_x.tx_x.block_facil_response_x.broker_id_s));
    // the xref
    tx_click_string ( &OMmsg_x.tx_x.block_facil_response_x.free_text_s[0],
		     order_data_px->userXRef_s, 
		     sizeof (OMmsg_x.tx_x.block_facil_response_x.free_text_s ) );
    // no validity time
    // no client category (EAM/FIRM/FARMM)
    // no expose flag

    status_i = do_click_tx_rx ( click_px,
				    &OMmsg_x,
				    tx_length_u );
    if ( status_i < SUCCESSFUL)
    {
	// do nothing, error already reported by do_click_tx_rx
    }
    else
    {
	click_px->maxLocalOrderNumber_u++;
	// update the order ID of the order
	PUTQUAD ( order_data_px->orderID_u,
		    click_px->log_reply_x.OrderIdent_u );
	// and the 'local' order number of the order
	order_data_px->localNumber_n = click_px->maxLocalOrderNumber_u;

	printf ("Local %s Order Number: %ld\n",
		order_category_to_string ( order_category_u ),
		click_px->maxLocalOrderNumber_u );
    }

    // save this order into last_order_x, and note the customer and member
    snapshot_last_order ( click_px, order_data_px );

    return (status_i);
}   // send_block_facil_response

// ---------------------------------------------------------------------------

int32 generate_block_facil_response ( click_details_xt * click_px,
					uint16 order_category_u,
					int argc,
					char *argv[ ] )
/*
Function:   generate_block_facil_response
Author:     Neil Cossor
Modified:   200012xx
Description:

    Client wishes to send a block/facilitation order response
    This is sent with an MO109

    This is similar to an ordinary order (in params etc)
    EXCEPT
    there is NO order_type ( limit, market)
    there is NO ignore-away-market flag
    BUT
    there IS a reference orderID

    If the order is accepted, then we add the order into the
    known order Q, otherwise we release the (local) order block.

    It is triggered by a BLOCK,RESPONSE command,
    or a FACILITATION,RESPONSE command

Input Params:

    order_category_u	== 8 if block response command
			== 16 if facilitation response command
			(as per DFI)

    argv[0]	"BLOCK" || "FACILITATION"
    argv[1]	"RESPONSE"
    argv[2]	"BID"|"ASK"
    argv[3]	series name
    argv[4]	quantity
    argv[5]	premium
    argv[6]	X-reference == freetext
    (optionally)
    argv[7]	orderID we are responding to
		defaults to the current active block/facilitation for the
		nominated series, for which there can only be ONE..
		and if we are supposed to be able to response, we should
		have received the broadcast about it
    argv[8]	broker ID (i.e. operator), default of this user
    argv[9]	open/close setting for OCC account, default is "use default for a/c"
    argv[10]	CMTA account, default is ""
    argv[11]	" sub account, default is ""
*/
{
    int     status_i = SUCCESSFUL;

    // pointer to the Q'able block, which we will contain a ptr to actual order
    gen_buf_node_xt	*order_node_px;
    // pointer to the actual order data
    order_entry_xt	*order_data_px;
    // pointer to relevant series
    ise_series_info_xt	*series_data_px = NULL;

    // basic data for transaction - defaulted, or converted from argv params
    int32		order_premium_u;
    uint16		order_quantity_u;
    xref_st		order_xref_s;
    uint8		bid_ask_u;	    // if the order is bid or ask
    quad_word		resp_orderID_u;     // order ID we are responding to
    user_id_st		*broker_oper_id_ps; // pointer to broker/operator ID
    uint8		open_close_u;	    // OCC settlement setting
    char		*cmta_member_ps;    // pointer to CMTA member ID string
    char		*sub_ac_ps;    // pointer to and sub account ID string

    if ( !click_px->logged_on_i )
    {
	prompt_logged_on ((  order_category_u == ORDER_CAT_BLOCK_RESPONSE ) ? "Block,Response" : "Facilitation,Response" );
	return NOT_LOGGED_ON;
    }

    // have we enuff arguments - minimum of 7
    if ( argc < 7)
    {
	// insufficient arguments - prompt the user, and return
	prompt_block_facil_response_command (
		( order_category_u == ORDER_CAT_BLOCK_RESPONSE ) ? "Block,Response" : "Facilitation,Rsponse"
	    );
	return SUCCESSFUL;
    }

   // we have the minumum argument list

    // what did the command select re BID vs ASK
    bid_ask_u = cmd_bid_ask ( click_px, argv[2] );
    // legal selection ?
    if ( bid_ask_u == 3 )
    {
	printf ("Illegal order type qualifier - BID | ASK\n");
	return SUCCESSFUL;
    }

    // series name in argv[3] - use that directly in search for ise_series_info_xt
    series_data_px = user_selected_series ( click_px, argv[3] );
    if (series_data_px == NULL)
    {
	// couldn't find that series - error already reported - bail out
	return SUCCESSFUL;
    }

    // Read the order quantity
    order_quantity_u = (uint16)read_unsigned_limited (argv [4], UNDEFINED_UINT16);

    // Read the order premium
    order_premium_u = read_decimal_string_int ( argv[5],
				series_data_px->instrument_class_px->dec_in_premium_u );

    // And finally, see if there is an order xref
    strncpy(order_xref_s, argv[6], sizeof (order_xref_s));

    // optional parameters

    // order ID
    if ( argc >= 8 )
    {
	cmd_orderID ( click_px,
					argv[7], &resp_orderID_u );
	if ( memcmp(&resp_orderID_u, &(click_px->null_orderID_u), sizeof(resp_orderID_u)) == 0 )
	{
	    // illegal order ID selector - reported, just return to called
	    return SUCCESSFUL;
	}
    }
    else
	// default to value for current outstanding block/facilitation for this series
	resp_orderID_u = series_data_px->block_facil_orderID_u;

    // broker / operator ID string
    if ( argc >= 9 )
	// customer specified the broker ID
	(char *)broker_oper_id_ps = argv[8];
    else
	// customer did not specify a broker ID - use our own
	broker_oper_id_ps = &click_px->user_code_x.customer_id_s;

    // open/close behaviour for OCC account during settlement
    if ( argc >= 10 )
	// customer specified a value for open/close
	open_close_u = read_unsigned_limited ( argv[9], UNDEFINED_UINT8 );
    else
	open_close_u = OPEN_CLOSE_REQUEST_DEFAULT;

    // see if supplied CMTA account
    if ( argc >= 11 )
	// customer specified a value for CMTA member ID
	cmta_member_ps = argv[10];
    else
	// if non specified, no default (most trades are not transferred via CMTA to other OCC firms)
	cmta_member_ps = "";

    // see if supplied CMTA sub account
    if ( argc >= 12 )
	// customer specified a value for CMTA sub account ID
	sub_ac_ps = argv[11];
    else
	/* if non specified, no default (normally only trades from MMs have a sub-account ID,
	    and all MM users have a default sub-account ID in CDB
	*/
	sub_ac_ps = "";


    // create an order_entry block. No Order ID yet...
    order_node_px = create_order_entry ( ++click_px->maxLocalOrderNumber_u );
    // point to the order data block of what we just got
    order_data_px = (order_entry_xt *)order_node_px->msg_p;
    update_order ( click_px,
		    order_data_px,
		    series_data_px,
		    click_px->null_orderID_u,
		    order_premium_u,
		    order_quantity_u,
		    bid_ask_u,
		    order_xref_s,
		    0,			// validity time, irrelevant in Bl/Facil response
		    0,			// limit/market ireelevant in Bl/Facil response
		    0,			// client category, irrelevant in Bl/Facil response
		    0,			// ignore away - irrelevant in Bl/Facil response
		    (char *)broker_oper_id_ps,
		    0,			// lock type - none, not locked
		    order_category_u,
		    0,			// expose flag - irrelevant, not Block order
		    open_close_u,	// a/c behaviour during OCC settlement
		    cmta_member_ps,	// CMTA account
		    sub_ac_ps,	// and sub account ID
		    0			// no broker % in a broker / facilitation response
		);

    // and attempt to send it
    status_i = send_block_facil_response ( click_px,
					    order_node_px,
					    order_category_u,
					    resp_orderID_u );

    // if order went in OK, then add it to the Q of known orders
    if (status_i >= SUCCESSFUL)
    {
	// optional debug / on-screen report
	display_one_order ( order_data_px );

	add_to_q_tail ( &click_px->our_market_x.known_orders_q,
			&order_node_px->chain );
    }
    else
    {
	// error already reported

	// release the memory allocated to the order
	free_gen_buf_node ( order_node_px );
	click_px->maxLocalOrderNumber_u--;
    }

    return status_i;
}   // generate_block_facil_response

// -----------------------------------------------------------------------

static int32 send_cab_order (click_details_xt	* click_px,
				gen_buf_node_xt *order_node_px )    
/*
Function : send_cab_order
Author	 : Neil Cossor
Modified : 20010620
Description:

    For the supplied order information, sends a block order to the system
    Uses mo110

    If the order is accepted, then we update the orderID.
*/
{
    // local buffer, containing actual msg we send
    generic_txn_xt	OMmsg_x;
    // pointer to the actual order data
    order_entry_xt	*order_data_px;

    int32		status_i	    =	SUCCESSFUL;
    // size of our actual CLICK query/command
    uint32		tx_length_u = sizeof ( OMmsg_x.tx_x.cab_trans_x );


    // point to the order data block
    order_data_px = (order_entry_xt *)order_node_px->msg_p;

    pack_OM_header ( &OMmsg_x, tx_length_u, 'M', 'O', 110);
    // save the facility code and fn code (for the logger, and generic send routine)
    OMmsg_x.fn_code = LOG_K_C_CAB_ORDER_TRANS;
    OMmsg_x.facility_u = click_px->facility_no_u + 7;	// EP7/ET7

    put_click_series ( &OMmsg_x.tx_x.cab_trans_x.series,
			&order_data_px->series_px->ise_series_x );

    // now we can specify orderbook_c
    OMmsg_x.tx_x.cab_trans_x.orderbook_c = order_data_px->series_px->parent_under_px->orderbook_u;
    // endian fields
    // there is NO premium in a CAB order
    // NB this SHOULD have needed a change to PUTLONG for  16.3.. already was, so was a bug
    PUTLONG ( OMmsg_x.tx_x.cab_trans_x.quantity_u,
		order_data_px->quantity_u );
    // validity time - GTC / ROD / IOC / FOK
    PUTSHORT ( OMmsg_x.tx_x.cab_trans_x.time_validity_n,
		order_data_px->validity_time_u );
    // bid / ask
    OMmsg_x.tx_x.cab_trans_x.bid_or_ask_c = order_data_px->bid_ask_u;
    // EAM/FIRM/FARMM
    OMmsg_x.tx_x.cab_trans_x.client_category_c =
	    order_data_px->client_category_u;
    OMmsg_x.tx_x.cab_trans_x.open_close_req_c = order_data_px->open_close_u;

    // put in the two parts of the client ID, which are actually the CMTA a/c and sub a/c IDs
    tx_click_string ( &OMmsg_x.tx_x.cab_trans_x.ex_client_s [0],
			&order_data_px->cmta_member_s,
			MAX_CMTA_MEMBER );
    tx_click_string ( &OMmsg_x.tx_x.cab_trans_x.ex_client_s [MAX_CMTA_MEMBER],
			&order_data_px->sub_ac_s,
			MAX_SUB_ACCOUNT_ID );
    // need to set the broker_id_s
    tx_click_string ( &OMmsg_x.tx_x.cab_trans_x.broker_id_s,
			&order_data_px->broker_oper_id_s,
			sizeof (OMmsg_x.tx_x.cab_trans_x.broker_id_s));
    // the xref
    tx_click_string ( &OMmsg_x.tx_x.cab_trans_x.free_text_s[0],
		     order_data_px->userXRef_s, 
		     sizeof (OMmsg_x.tx_x.cab_trans_x.free_text_s ) );

    status_i = do_click_tx_rx ( click_px,
				    &OMmsg_x,
				    tx_length_u );
    if ( status_i < SUCCESSFUL)
    {
	// do nothing, error already reported by do_click_tx_rx
    }
    else
    {
	click_px->maxLocalOrderNumber_u++;
	// update the order ID of the order
	PUTQUAD ( order_data_px->orderID_u,
		    click_px->log_reply_x.OrderIdent_u );
	// and the 'local' order number of the order
	order_data_px->localNumber_n = click_px->maxLocalOrderNumber_u;

	printf ("Local CAB Order Number: %ld\n",
		 click_px->maxLocalOrderNumber_u );
    }

    // save this order into last_order_x, and note the customer and member
    snapshot_last_order ( click_px, order_data_px );

    return (status_i);
}   // send_cab_order

// ---------------------------------------------------------------------------

int32 generate_cab_order ( click_details_xt * click_px,
			    int argc,
			    char *argv[ ] )
/*
Function:   generate_cab_order
Author:     Neil Cossor
Modified:   20000329
Description:

    Client wishes to send a CAB order.
    This is sent with an mo110

    This is similar to an ordinary order (in params etc)
    EXCEPT
    there is NO premium
    there is NO order_type ( limit, market)
    there is NO ignore-away-market flag

    If the order is accepted, then we add the order into the
    known order Q, otherwise we release the (local) order block.

    It is triggered by a CAB command

Input Params:

    argv[0]	"CAB"
    argv[1]	"BID"|"ASK"
    argv[2]	series name
    argv[3]	quantity
    argv[4]	X-reference == freetext
    (optionally)
    argv[5]	validity time.. G[tc] / R[od] / I[oc] / F[ok], default GTC
    argv[6]	client category.. C[ustomer] | FI[rm\ | FA[rmm], default Client
    argv[7]	broker ID (i.e. operator), default of this user
    argv[8]	open/close setting for OCC account, default is "use default for a/c"
    argv[9]	CMTA account, default is ""
    argv[10]	" sub account, default is ""
*/
{
    int     status_i = SUCCESSFUL;

    // pointer to the Q'able block, which we will contain a ptr to actual order
    gen_buf_node_xt	*order_node_px;
    // pointer to the actual order data
    order_entry_xt	*order_data_px;
    // pointer to relevant series
    ise_series_info_xt	*series_data_px = NULL;

    // basic data for transaction - defaulted, or converted from argv params
    uint16		order_quantity_u;
    xref_st		order_xref_s;
    uint16		validity_time_u;
    uint8		client_category_u;  // cust/Firm/FARMM
    uint8		bid_ask_u;	    // if the order is bid or ask
    user_id_st		*broker_oper_id_ps; // pointer to broker/operator ID
    uint8		open_close_u;	    // OCC settlement setting
    char		*cmta_member_ps;    // pointer to CMTA member ID string
    char		*sub_ac_ps;    // pointer to and sub account ID string

    if ( !click_px->logged_on_i )
    {
	prompt_logged_on ( "CAB" );
	return NOT_LOGGED_ON;
    }

    // have we enuff arguments - minimum of 5
    if ( argc < 5)
    {
	// insufficient arguments - prompt the user, and return
	prompt_cab_command ();
	return SUCCESSFUL;
    }

   // we have the minumum argument list

    // what did the command select re BID vs ASK
    bid_ask_u = cmd_bid_ask ( click_px, argv[1] );
    // legal selection ?
    if ( bid_ask_u == 3 )
    {
	printf ("Illegal CAB order type qualifier - BID | ASK\n");
	return SUCCESSFUL;
    }

    // series name in argv[2] - use that directly in search for ise_series_info_xt
    series_data_px = user_selected_series ( click_px, argv[2] );
    if (series_data_px == NULL)
    {
	// couldn't find that series - error already reported - bail out
	return SUCCESSFUL;
    }

    // Read the order quantity
    order_quantity_u = (uint16)read_unsigned_limited (argv [3], UNDEFINED_UINT16);

    // And finally, get the order xref
    strncpy(order_xref_s, argv[4], sizeof (order_xref_s));

    // optional parameters

    // validity time
    /*
	NB there is a clash in the OM SPM - it says in one place for mo110 that
	time MUST be GTC.. and in another says it must be one of ROD and GTC
    */
    if ( argc >= 6 )
    {
	validity_time_u = cmd_validity_time ( argv[5], VALIDITY_TIME_FAS_GTC );
	if ( validity_time_u == UNDEFINED_UINT16 )
	{
	    // illegal validity time selector - reported, just return to called
	    return SUCCESSFUL;
	}
    }
    else
	// default to GTC
	validity_time_u = cmd_validity_time ( "", VALIDITY_TIME_FAS_GTC );

    // client category..
    if ( argc >= 7 )
    {
	client_category_u = cmd_client_category ( argv[6], CUST_CATEGORY_CUSTOMER );
    }
    else
	// default to client category
	client_category_u = CUST_CATEGORY_CUSTOMER;

    // broker / operator ID string
    if ( argc >= 8 )
	// customer specified the broker ID
	(char *)broker_oper_id_ps = argv[7];
    else
	// customer did not specify a broker ID - use our own
	broker_oper_id_ps = &click_px->user_code_x.customer_id_s;

    // open/close behaviour for OCC account during settlement
    if ( argc >= 9 )
	// customer specified a value for open/close
	open_close_u = read_unsigned_limited ( argv[8], UNDEFINED_UINT8 );
    else
	open_close_u = OPEN_CLOSE_REQUEST_DEFAULT;

    // see if supplied CMTA account
    if ( argc >= 10 )
	// customer specified a value for CMTA member ID
	cmta_member_ps = argv[9];
    else
	// if non specified, no default (most trades are not transferred via CMTA to other OCC firms)
	cmta_member_ps = "";

    // see if supplied CMTA sub account
    if ( argc >= 11 )
	// customer specified a value for CMTA sub account ID
	sub_ac_ps = argv[10];
    else
	/* if non specified, no default (normally only trades from MMs have a sub-account ID,
	    and all MM users have a default sub-account ID in CDB
	*/
	sub_ac_ps = "";

    // create an order_entry block. No Order ID yet...
    order_node_px = create_order_entry ( ++click_px->maxLocalOrderNumber_u );
    // point to the order data block of what we just got
    order_data_px = (order_entry_xt *)order_node_px->msg_p;
    update_order ( click_px,
		    order_data_px,
		    series_data_px,
		    click_px->null_orderID_u,
		    0,			// premium, irrelevant in CAB order
		    order_quantity_u,
		    bid_ask_u,
		    order_xref_s,
		    validity_time_u,
		    0,			// limit/market, irrelevant in CAB order
		    client_category_u,
		    0,			// ignore away, irrelevant in CAB order
		    (char *)broker_oper_id_ps,
		    0,			// lock type - none, not locked
		    ORDER_CAT_CAB,
		    0,			// expose flag, irrelevant, not Block order
		    open_close_u,	// a/c behaviour during OCC settlement
		    cmta_member_ps,	// CMTA account
		    sub_ac_ps,	// and sub account ID
		    0			// no broker % in a CAB order
		);

    printf ("CAB order %s %d %s\n",
	     bid_ask_to_string ( bid_ask_u ),
	     order_quantity_u,
	     &series_data_px->ise_name_s );

    // and attempt to send it
    status_i = send_cab_order ( click_px,
				order_node_px );

    // if order went in OK, then add it to the Q of known orders
    if (status_i >= SUCCESSFUL)
	add_to_q_tail ( &click_px->our_market_x.known_orders_q,
			&order_node_px->chain );
    else
    {
	// release the memory allocated to the order
	free_gen_buf_node ( order_node_px );
	click_px->maxLocalOrderNumber_u--;
	// error already reported
    }

    return status_i;
}   // generate_cab_order

// -----------------------------------------------------------------------

void snapshot_save_last_order ( click_details_xt *click_px )
/*
Function:   snapshot_save_last_order
Author:     Neil Cossor
Modified:   9908xx
Description:

    The user has asked us to snapshot the details from the last order,
    into the 'saved' last order. The fields in the saved last order
    can then be referenced by using the %O string.

    This could be done automagically.. but while slightly more
    tedious (at the scripting level), this mechanism gives us more flexibility,
    for example, allowing us to amend an older order after doing some
    other order-related things...
*/
{
    // save the last order as the 'saved' order, for reference by %O
    memcpy ( &click_px->saved_order_x,
		&click_px->last_order_x,
		sizeof ( click_px->last_order_x ));

    if ( click_px->saved_order_x.series_px == NULL )
    {
	printf ("Warning, order saved has no series\n");
    }
    else
    {
	// series is set up OK

	// stash the series name, to make sure we use right series across logons
	strcpy ( click_px->saved_order_series_name_s,
		    click_px->last_order_x.series_px->ise_name_s );

	printf ("Saved :");
	display_one_order ( &click_px->saved_order_x );
    }
}   // snapshot_save_last_order
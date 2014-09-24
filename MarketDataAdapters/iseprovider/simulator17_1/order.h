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


Title : order.h
Author: Neil Cossor

Function :

    This file is the header file for order.c,
    which provides basic order handling to the ISESim.exe program.
    

Revision history :-

yymmdd	vers'n	author	    notes
------	------	------	    -----
9905xx	v0.00	Neil Cossor initial version
000404	v0.01		    add delete_order_by_orderID

*/
#ifndef ORDER_H
#define ORDER_H

// -----------------------------------------------------------------------

int32 delete_order(click_details_xt * click_px,
		    int argc,
		    char *argv[ ] );
/*
Function : delete_order
Author	 : Neil Cossor
Modified : 9905xx
Description:

    We have been asked to delete one/some orders.

    Calls the appropriate routine to carry this out, including the deletion
    from both the local orderbook, and the market (i.e. central orderbook).

Input Params:

    standard argc/argv params that got us here i.e. the command.
    Choices of command are:-

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

// ---------------------------------------------------------------------------

int32 generate_order (click_details_xt * click_px,
			int argc,
			char *argv[ ],
			int8 BidAsk_c );

int32 amend_order (click_details_xt * click_px,
		    int argc,
		    char *argv[ ] );
/*
Function : amend_order
Author	 : Neil Cossor
Modified : 9908xx
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
    argv[4]	<new quantity>
    argv[5]	<new free-text (Xref)>
    argv[6]	<new validity-time>
    argv[7]	<new ignore-away market setting>
    argv[8]	<new broker-id>
    argv[9]	<new open-close flag value>
*/

// ---------------------------------------------------------------------------

void display_my_orderbook ( click_details_xt *click_px );

// ---------------------------------------------------------------------------

void clear_local_orderbook ( click_details_xt *click_px );

// ---------------------------------------------------------------------------

int32 query_orders ( click_details_xt * click_px,
			    int argc,
			    char *argv[] );
/*
Function:   query_orders
Author:     Neil Cossor
Modified:   9907xx
Description:

    Go find out about orders from the system orderbook.
    If we are inquiring on active orders, then update the local orderbook.

    Get the subset qualifier by the argc/argv parameters

    This was triggered by a 
    GET,<ACTIVE|INACTIVE>,<*|S|U>[,<series name>|>underlying name>]

Input Params:

    Standard argc/argv of the command line.
    Note that this command does NOT do interactive prompting for
    params if the arguments are not supplied.

    argv[2] may have values of
    - "*"	    all orders in all series (and hence all underlying)
    - Series	    specifies that argv[3] is a series name,
		    i.e. collect all orders in that particular series
    - Underlying    specifies that argv[3] is an underlying name
		    i.e. collect all orders for all series under a particular underlying

Output Params:

    If collecting active orders. updates the local orderbook
    If not, then displays the inactive orders.
*/

// ---------------------------------------------------------------------------

int32 generate_block_order ( click_details_xt * click_px,
			    int argc,
			    char *argv[] );
/*
Function:   generate_block_order
Author:     Neil Cossor
Modified:   9907xx
Description:

    Client wishes to send a block order
    This is sent with an MO107

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

    argv[0] == "BLOCK"
    argv[1] == "BID"|"ASK"
    argv[2] == series name
    argv[3] == quantity
    argv[4] == premium
    argv[5] == X-reference == freetext
    (optionally)
    argv[6] == validity time.. G[tc] / R[od] / I[oc] / F[ok], default GTC
    argv[7] == client category.. C[ustomer] | FI[rm\ | FA[rmm], default Client
    argv[8] == expose flag, default 0
*/

// ---------------------------------------------------------------------------

int32 generate_facilitation_order ( click_details_xt * click_px,
				    int argc,
				    char *argv[] );
/*
Function:   generate_facilitation_order
Author:     Neil Cossor
Modified:   9907xx
Description:

    Client wishes to send a facilitation order
    This is sent with an MO108

    This is similar to an ordinary order (in params etc)
    EXCEPT
    there is NO order_type ( limit, market)
    there is NO ignore-away-market flag
    there is no validity time
    BUT
    there IS an open_close_req value for the EAM
    (as distinct from the value for the facilitating broker)

    If the order is accepted, then we add the order into the
    known order Q, otherwise we release the (local) order block.

    It is triggered by a FACILITATION command

Input Params:

    argv[0] == "FACILITATION"
    argv[1] == "BID"|"ASK"
    argv[2] == series name
    argv[3] == quantity
    argv[4] == premium
    argv[5] == X-reference == freetext
    (optionally)
    argv[6] == client category.. C[ustomer] | FI[rm\ | FA[rmm], default Client
*/

// ---------------------------------------------------------------------------

int32 generate_cab_order ( click_details_xt * click_px,
			    int argc,
			    char *argv[ ] );
/*
Function:   generate_cab_order
Author:     Neil Cossor
Modified:   9907xx
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

    argv[0] == "CAB"
    argv[1] == "BID"|"ASK"
    argv[2] == series name
    argv[3] == quantity
    argv[4] == X-reference == freetext
    (optionally)
    argv[5] == validity time.. G[tc] / R[od] / I[oc] / F[ok], default GTC
    argv[6] == client category.. C[ustomer] | FI[rm\ | FA[rmm], default Client
*/

// ---------------------------------------------------------------------------

int32 generate_block_facil_response ( click_details_xt * click_px,
					uint16 order_category_u,
					int argc,
					char *argv[ ] );
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

    argv[0] == "BLOCK" || "FACILITATION"
    argv[1] == "RESPONSE"
    argv[2] == "BID"|"ASK"
    argv[3] == series name
    argv[4] == quantity
    argv[5] == premium
    argv[6] == X-reference == freetext
    (optionally)
    argv[7] == orderID we are responding to
		defaults to the current active block/facilitation for the
		nominated series, for which there can only be ONE..
		and if we are supposed to be able to response, we should
		have received the broadcast about it
    argv[8] == broker ID (i.e. operator), default of this user
*/

// -----------------------------------------------------------------------

void snapshot_save_last_order ( click_details_xt *click_px );
/*
Function:   snapshot_save_last_order
Author:     Neil Cossor
Modified:   9908xx
Description:

    The user has asked us to snapshot the details from the last order,
    into the 'saved' last order. The fields in the saved last order
    can then be referenced by using the %O string.

    This could be done automagically.. but while slightly more
    tedious (at the scripting level), this mechanism gives us more flexibility.
*/

// ---------------------------------------------------------------------------

void delete_order_by_orderID ( click_details_xt * click_px,
				quad_word delete_orderID_u );
/*
Function:   delete_order_by_orderID
Author:	    Neil Cossor
Modified:   20000404
Description:

    We know a particular order has been deleted.. delete it from memory,
    if we know about it.

    This could occur because we explicitly deleted an order, or because
    we received a broadcast about an order delete.

Input Params:

    - pointer to the structure with everything we know about the OM/ISE system
    - the orderID to attempt to delete.. in local-endian order
*/

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
				);
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



#endif

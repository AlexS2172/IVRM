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


Title : combos.h
Author: Neil Cossor

Function :

    This file is the header file for combos.c,
    which provides basic combo handling to the ISESim.exe program.
    

Revision history :-

yymmdd	vers'n	author	    notes
------	------	------	    -----
0106xx	v0.00	Neil Cossor initial version
010627			    added routine to amend a combo
011017	v0.01	Neil Cossor	add routine to change qty of existing combo (required for Match)

*/
#ifndef COMBO_H
#define COMBO_H

// ---------------------------------------------------------------------------

void display_my_combobook ( click_details_xt *click_px );

// ---------------------------------------------------------------------------

void clear_local_combobook ( click_details_xt *click_px );

// ---------------------------------------------------------------------------

void delete_combo_by_orderID ( click_details_xt * click_px,
				quad_word delete_orderID_u,
				unsigned local_byte_order);
/*
Function:   delete_combo_by_orderID
Author:	    Neil Cossor
Modified:   20010529
Description:

    We know a particular combo has been deleted.. delete it from memory,
    if we know about it.

    This could occur because we explicitly deleted a combo, or because
    we received a broadcast about an combo delete.

Input Params:

    - pointer to the structure with everything we know about the OM/ISE system
    - the orderID to attempt to delete.. in local-endian order UNLESS
    - the local_byte_order param indicates FALSE i.e. need to swap the bytes pre search
*/
// -----------------------------------------------------------------------

void external_update_combo ( click_details_xt *click_px,
				ise_underlying_xt   *under_data_px,	// all combos are associated with ONE underlying
				quad_word		    OM_orderID_u,
				int32		    OM_premium_i,	// price
				uint32		    OM_quantity_u,
				char		    OM_userXRef_s[],    // free text
				uint16		    OM_validity_time_u, // GTC/ROD/IOC/FOK/FAS/AON
				uint16		    order_category_u,	// interest-only vs executable - local endian
				uint8		    client_category_u,  // cust/Firm/FARMM
				user_id_st	    broker_oper_id_s,   // who is the 'owner'
				cmta_member_st	    cmta_member_s,	// the CMTA account
				sub_account_id_st   sub_account_id_s,   // the sub account ID
				combo_message_st    message_text_s,	// message text - mandatory in 0 leg combos
				trading_code_t	    *OM_trading_code_px,// which user (and member) this order belongs to
				// need some legs here...
				uint8		    no_legs_u,		// how many legs - may be none
				int32_t ul_price_high_i,
				int32_t ul_price_low_i,
				int32_t ul_price_method_c,
				OM_one_combo_leg_xt leg_x[4]		// and the legs, in OM format
				);
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

    Note the fields in the legs have been filled with local-format data already
*/

// ---------------------------------------------------------------------------

unsigned query_public_combos ( click_details_xt *click_px );
/*
Function:   query_public_combos
Author:	    Neil Cossor
Modified:   20010605
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

// ---------------------------------------------------------------------------

unsigned query_member_active_combos ( click_details_xt *click_px );
/*
Function:   query_member_active_combos
Author:	    Neil Cossor
Modified:   20010605
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

// ---------------------------------------------------------------------------

unsigned query_member_inactive_combos ( click_details_xt *click_px );
/*
Function:   query_member_inactive_combos
Author:	    Neil Cossor
Modified:   20010605
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

// ---------------------------------------------------------------------------

void clear_combo_legs ();
/*
Function:   clear_combo_legs
Author:     Neil Cossor
Modified:   20010607
Description:

    We wish to clear out the components in the (in-memory) combo table.
    This can be because of a COMBO,CLEAR user command, or
    because we succeeded in doing a COMBO,SEND
*/


// ---------------------------------------------------------------------------

int32 parse_send_combo ( click_details_xt * click_px,
			    int argc,
			    char *argv[] );
/*
Function:   parse_send_combo
Author:     Neil Cossor
Modified:   20010607
Description:

    MM wishes to update their quotes.
    Uses mo106.

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

    optional arguments are
    argv[6] validity time
    argv[7] client category : customer|firm|farmm etc
    argv[8] brokerID
    argv[9] cmta
    argv[10] sub account
    argv[11] message text (up to 100 chars).. mandatory if 0 legs
    argv[12] cross-reference (gets passed through OCC)
*/

// ------------------------------------------------------------------------

void view_combo_legs ();
/*
Function:   view_combo_legs
Author:	    Neil Cossor
Modified:   20010608
Description:

    Display the combo leg information that we have in memory at present
*/

// ---------------------------------------------------------------------------

int32 add_combo_leg ( click_details_xt * click_px,
			int argc,
			char *argv[] );
/*
Function:   add_combo_leg
Author:     Neil Cossor
Modified:   20010607
Description:

    User is specifying the details of one combo leg, preparatory to sending the whole thing.
    The whole shebang is finalised and sent to the main system by a COMBO,SEND command

    (mandatory) arguments are
    argv[0] COMBO
    argv[1] BID|ASK
    argv[2] <series name>
    optional arguments are
    argv[3] ratio, default value 1
    argv[4] open/close, default open
*/

// ---------------------------------------------------------------------------

int32 parse_send_hit ( click_details_xt * click_px,
			unsigned argc,
			char *argv[] );
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

// -----------------------------------------------------------------------

void snapshot_save_last_combo ( click_details_xt *click_px );
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

// ---------------------------------------------------------------------------

int32 parse_delete_combo ( click_details_xt *click_px,
			    unsigned argc,
			    char *argv[] );
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

// ---------------------------------------------------------------------------

int32 parse_send_amend_combo ( click_details_xt * click_px,
				unsigned argc,
				char *argv[] );
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
    optional arguments (if not specified, default to old combo values)
    argv[3] premium	    ignored for interest-only combo, I guess
    argv[4] INTEREST|LIMIT  can't change limit -> interest
    argv[5] quantity	    new quantity. ? can you increase this ?
    argv[6] validity time   there are limits on how you can change this
    argv[7] client category
    argv[8] brokerID	    new brokerID
    argv[9] cmta	    new CMTA
    argv[10] sub account	    new sub-account
    argv[11] cross-reference new xref (gets passed through OCC)
    argv[12].... open/close	for leg 1..
*/

// ---------------------------------------------------------------------------

void update_combo_qty_by_orderID ( click_details_xt * click_px,
				    quad_word change_orderID_u,
				    unsigned local_byte_order,
				    uint32 new_qty_u );
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



#endif

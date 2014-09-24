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


Title : underlying.h
Author: Neil Cossor

Function :

    This file is the header for underlying.c, which provides
    basic handling of underyling (e.g. equitities) to the ISESim.exe program.
    
Revision history :-

yymmdd	vers'n	author	    notes
------	------	------	    -----
9905xx	v0.00	Neil Cossor initial version
990928	v0.01		    added new routine check_all_underlying_instr_classes
000118	v0.02		    added new routine display_synthetics
000425	v0.03		    added new routine list_underlyings
*/

#ifndef UNDERLYING_H
#define UNDERLYING_H


// -------------------------------------------------------------------------------------

int32 enter_underlying_price_query(click_details_xt * click_px,
			   int argc,
			   char *argv[] );

// -------------------------------------------------------------------------------------

void release_underlying_series_chains ( country_market_xt *country_market_px );
/*
Function : release_under_sort_tables
Author   : Neil Cossor
Modified : 9905xx
Description:

    We are discarding our links from underlying to child series.
    This could be becoz we have gone offline, or becoz we just received
    update data which invalidates our old info
*/

// -------------------------------------------------------------------------------------

void clear_underlying_from_memory ( click_details_xt *click_px );

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
    argv[1]	(optional) string selector, which we filter the output with
		If supplied, only show underlyings whose name start with that string
*/
void display_underlyings ( click_details_xt *click_px,
			    int argc,
			    char *argv [] );

// -------------------------------------------------------------------------------------

ise_underlying_xt * find_under_by_name_s (click_details_xt *click_px,
						name_st search_name_s );
/*
    routine which searches in the known underlying for all country / markets
    to find the ise_underlying_xt which matches the underlying name supplied.

    Returns pointer to that structure, or NULL if no match found.
*/

//-----------------------------------------------------------------------------------

gen_buf_node_xt * find_under_block_by_commodity ( click_details_xt * click_px,
						    uint16 search_under_code_u );
/*
    routine which searches in the known underlying for all country / markets
    to find the gen_buf_node_xt which matches the commodity code supplied.

    Returns pointer to that Q'ed structure, or NULL if no match found.

    NB as per find_under_by_name_s, this fn used the sorted array of pointers to underlying.
    This does order (n log 2) comparisons, rather than order (n/2).
*/
// -------------------------------------------------------------------------------------

ise_underlying_xt * find_under_data_by_commodity ( click_details_xt * click_px,
						    uint16 search_under_code_u );
/*
    routine which searches in the known underlying for all country / markets
    to find the ise_underlying_xt which matches the commodity code supplied.

    Returns pointer to that structure, or NULL if no match found.
*/

// -------------------------------------------------------------------------------------

void sort_underlying_tables ( country_market_xt * country_market_px,
				int replace_tables );
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

// -------------------------------------------------------------------------------------

int32 query_all_underlying_statii ( click_details_xt * click_px );
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
*/

// -------------------------------------------------------------------------------------

char * underlying_status_to_string ( uint8 trading_status_u );
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

// -------------------------------------------------------------------------------------

void display_one_under_bbo ( ise_underlying_xt *under_data_px );
/*
    We have a BBO for an underlying.
    This could be from a broadcast BO105, or a IQ121 query.

    In either case, spit out the interesting info onto stdout
*/

// -----------------------------------------------------------------------------------

void update_one_underlying ( click_details_xt *click_px,
				ise_underlying_xt *ise_underlying_px,
				da204_t *answer_underlying_px );
/*
Function : update_one_underlying
Author	 : Neil Cossor
Modified : 9909xx
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

// -------------------------------------------------------------------------------------

void check_all_underlying_instr_classes ( click_details_xt *click_px,
						country_market_xt * country_market_px );
/*
Function:   check_all_underlying_instr_classes
Author:	    Neil Cossor
Modified:   990928
Description:

    We have complete data from the CDB.
    Check if we have instrument classes for all underlying.

    We also check that they have matching info, such as ranking types,
    tick tables, (and hence decimal trading vs fractions).

    We do this by looking through all known underlying, and for each look for
    an instrument class for all known instrument groups.

    This may not be valid where an OM exchange runs multiple markets,
    and only some instrument groups exist in each market.

    For the present time (one Options market within ISE),
    this will work correctly.
*/

// -------------------------------------------------------------------------------------

char * commodity_code_to_string ( click_details_xt * click_px,
				    uint16 search_under_code_u );
/*
Function:   commodity_code_to_string
Author:	    Neil Cossor
Modified:   991127
Description:

    We have a commodity code #, and with to have the underlying symbol (ID) for that.

Input Params:

    pointer to all the stuff we know about the central system
    the commodity code we are looking for

Output Params:

    returns pointer to (short) string name if we find a match
    returns pointer to string "*" if the commodity code is 0
    returns pointer to (static) string "Unk under" if we don't find a match
*/

// ---------------------------------------------------------------------------

ise_synthetic_xt * find_synthetic_by_commodity ( click_details_xt * click_px,
						    uint16 commodity_u );
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

// ---------------------------------------------------------------------------

void display_synthetics ( click_details_xt * click_px );
/*
Function:   display_synthetics
Author:	    Neil Cossor
Date:	    20000118
Description:

    The caller wishes to see the info on the known synthetics

    Display them on stdout
*/

// -------------------------------------------------------------------------------------

void list_underlyings ( click_details_xt *click_px,
			    int argc,
			    char *argv [] );
/*
Function : list_underlying
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



#endif

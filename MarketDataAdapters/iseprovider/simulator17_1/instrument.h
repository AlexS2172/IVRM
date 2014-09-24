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


Title : instrument.h

Function :

    This file is the header for instrument.c, which provides
    basic instrument handling to the ISESim.exe program.
    
Author:  Neil Cossor

Revision history :-

yymmdd	vers'n	author	    notes
------	------	------	    -----
9905xx	v0.00	Neil Cossor initial version
990923	v0.01		    added routine update_one_instrument_class, fix location of #endif
			    make find_instrument_class_by_series accessible to other code modules
			    added routine find_instrument_class_block_by_series
*/

/**********************************************************************************/
#ifndef INSTRUMENT_H
#define INSTRUMENT_H

#define INSTRUMENT_NAME_LEN   32


// ---------------------------------------------------------------------------

instrument_xt * find_instr_group ( click_details_xt *click_px,
				    uint8 search_instr_group_u );
/*
Function : find_instr_group
Author	 : Neil Cossor
Modified : 9906xx
Description:

    routine which searches within the click data we have, to find the
    block of data about an instrument group, and return a pointer to it

Input Params :

    pointer to the block of CLICK data
    the number of the instrument group we wish to find

Output Params :

    pointer to the block of instrument_xt data if found,
    or NULL
*/

// ---------------------------------------------------------------------------

void sort_instrument_classes ( country_market_xt * country_market_px,
				int replace_tables );
/*
Function:   sort_instrument_classes
Author:     Neil Cossor
Modified:   9907xx
Description:

    We have just received all the class info (or had an update).

    Generate the (sorted) structures pointing to these.

    Keeping in mind that the defining properties of an instrument class are
    country, market, underlying, and instrument group,
    we just sort on those. We should NEVER have duplicates.

Input Params:

    pointer to everything we know about one country / market
    flag if we are replacing the tables. TRUE means replacing, FALSE means new tables
*/

// ---------------------------------------------------------------------------

void clear_instrument_classes_from_memory ( click_details_xt * click_px );
/*
Function:   clear_instrument_classes_from_memory
Author:     Neil Cossor
Modified:   9907xx
Description:

    release all things to do with instrument classes

    We are exiting, or we have just logged off
    (and the data is no longer known to be valid)
*/

// ---------------------------------------------------------------------------

void display_instrument_groups ( click_details_xt * click_px );
/*
Function:   display_instrument_groups
Author:     Neil Cossor
Modified:   9907xx
Description:

    Display info about known instrument groups
*/

// ---------------------------------------------------------------------------

char * instr_group_name ( click_details_xt * click_px,
			    uint8 search_instr_group_u );
/*
Function:   instr_group_name
Author:     Neil Cossor
Modified:   9907xx
Description:

    Returns (pointer to) string name of the nominated instrument group (by number)
    NULL. if none found with that number
*/

// -------------------------------------------------------------------------------------

void update_one_instrument_class ( instrument_class_xt *ise_instr_class_info_px,
				    da210_t *answer_instr_class_px );
/*
Function : update_one_instrument_class
Author	 : Neil Cossor
Modified : 9909xx
Description:

    We have a pointer to a da210 record (from a dq210 query, or a bi210 bcast),
    and we have a pointer to a ISEsim internal record to hold the data for this.

    The item may be an add (dq210, or bi210 add), where we have just allocated space
    or a modify (bi210 modify), where we have found the original block.

Input Params:

    pointer to structure containing everything we currently know about the central system
    pointer to the (ISEsim internal) structure to hold everything about one instrument class
    pointer to the record containing everything about one instrument class that CLICK told us
	in the latest msg
*/

// ---------------------------------------------------------------------------

gen_buf_node_xt * find_instrument_class_block_by_series ( country_market_xt *country_market_px,
							    ise_series_xt * ise_series_x );
/*
Function:   find_instrument_class_block_by_series
Author:     Neil Cossor
Modified:   990923
Description:

    routine which searches in the known instrument class
    to find the gen_buf_node_xt, which contains a pointer to a instrument class
    data block, which matches the series supplied.

    We are checking for matching
    - country
    - market
    - underlying
    - instrument group

    Returns pointer to that structure, or NULL if no match found.
*/

// ---------------------------------------------------------------------------

instrument_class_pt find_instrument_class_by_series ( country_market_xt *country_market_px,
								ise_series_xt * ise_series_x );
/*
Function:   find_instrument_class_by_series
Author:     Neil Cossor
Modified:   9907xx
Description:

    routine which searches in the known instrument class
    to find the instrument_class_xt which matches the series supplied.

    We are checking for matching
    - country
    - market
    - underlying
    - instrument group

    Returns pointer to that structure, or NULL if no match found.
*/


#endif


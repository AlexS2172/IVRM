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

  
Title : series.h
Author: Neil Cossor

Function :

    This file the header for series.c, which provides series handling
    facilities to the ISESim.exe program.


Revision history :-

yymmdd	vers'n	author	    notes
------	------	------	    -----
9905xx	v0.00	Neil Cossor initial version
000425	v1.00		    added list_series

*/
#ifndef SERIES_H
#define SERIES_H


// -------------------------------------------------------------------------------------

void update_one_series ( click_details_xt *click_px,
			    ise_series_info_xt *ise_series_info_px,
			    da202_t *answer_series_px );
/*
Function:   update_one_series
Author:	    Neil Cossor
Modified:   990915
Description:

    We have a da202 record, from either the initial CDB load (at logon)
    or from a BI202 broadcast.

    We have an ISEsim internal record to store that info in...
    so we just need to move the data from the message into that storage
*/

// -------------------------------------------------------------------------------------

void get_click_series ( ise_series_xt * ise_series_x,
			series_t * click_series );
/*
    fn which transfers the attribute of a CLICK series, complete with weird bit-packing,
    endian issues etc, into a local ise_series_x, in "sensible" format
*/

// -------------------------------------------------------------------------------------

void put_click_series ( series_t * click_series,
			ise_series_xt * ise_series_x );
/*
    fn which packs a CLICK series, complete with weird bit-packing,
    endian issues etc, from a local ise_series_xt.
*/

//-----------------------------------------------------------------------------------

gen_buf_node_xt * find_series_block_by_ise_series ( click_details_xt * click_px,
						    ise_series_xt * search_series_px );
/*
    routine which searches in the known series for all country / markets
    to find the gen_buf_node_xt which matches the series supplied.

    The _by_ise_series means that the search series data is already in ISE format,
    not raw CLICK format.

    Returns pointer to that structure, or NULL if no match found.

*/

// -------------------------------------------------------------------------------------

ise_series_info_xt * find_series_by_ise_series ( click_details_xt * click_px,
						    ise_series_xt * search_series_px );
/*
    routine which searches in the known series for all country / markets
    to find the ise_series_xt which matches the series supplied.

    The _by_ise_series means that the search series data is already in ISE format,
    not raw CLICK format.

    Returns pointer to that structure, or NULL if no match found.
*/

// -------------------------------------------------------------------------------------

ise_series_info_xt * find_series_by_name_s ( click_details_xt * click_px,
						series_st search_series_s );
/*
    routine which searches in the known series for all country / markets
    to find the ise_series_xt which matches the name in the series_s supplied.

    Returns pointer to that structure, or NULL if no match found.
*/


// -------------------------------------------------------------------------------------

void clear_series_from_memory( click_details_xt *click_px );
/*
Function : clear_series_from_memory
Author	 : Neil Cossor
Modified : 9905xx
Description:

    Removes the loaded series from memory
*/

// -------------------------------------------------------------------------------------

// display the current known list of series
void display_series ( click_details_xt *click_px,
			int argc,
			char *argv[] );

// -------------------------------------------------------------------------------------

void sort_series_tables ( country_market_xt * country_market_px,
				int replace_tables );
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

// -------------------------------------------------------------------------------------

int32 series_volume_prices_query ( click_details_xt * click_px,
				    int argc,
				    char *argv[ ]);
/*
Function : series_volume_prices_query
Author	 : Neil Cossor
Modified : 9905xx
Description :

    Finds out the current snapshot of price / volume figures
    for all series with the same underlying. Dumps them out to screen.
*/

// -------------------------------------------------------------------------------------

void point_series_at_underlying ( click_details_xt *click_px,
				    country_market_xt * country_market_px,
				    int replace_tables );
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

// -------------------------------------------------------------------------------------

char * series_status_to_string ( uint8 series_status_u );
/*
Function:   series_status_to_string
Author:     Neil Cossor
Modified:   9907xx
Description:

    We have a series status number, which we wish to output.
    Convert from the number into a readable string

Input Params:

    The series status #

Output Params:

    Pointer to string containing string for that status
*/

// -------------------------------------------------------------------------------------

ise_series_info_xt *user_selected_series ( click_details_xt * click_px,
			char *user_series_argv_s );
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
    This should be a series name OR "%s"

Output Params:

    The pointer to the selected series, if found,
    otherwise NULL.

    Reports an error to stdout, if the series is unknown
*/

// ---------------------------------------------------------------------------

int cmp_series_by_ise_series_under ( ise_series_xt *first_series_px,
					ise_series_xt *second_series_px );
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

// -------------------------------------------------------------------------------------

void list_series ( click_details_xt *click_px,
		    int argc,
		    char *argv [] );
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


#endif

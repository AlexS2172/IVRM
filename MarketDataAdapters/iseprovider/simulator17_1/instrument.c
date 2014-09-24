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


Title : instrument.c
Author:  Neil Cossor
Description :

    This file provides basic instrument handling to the ISESim.exe program.
    This includes inquiring on all available instruments, instrument names,
    and instrument classes.
    
Revision history :-

yymmdd	vers'n	author	    notes
------	------	------	    -----
9905xx	v0.00	Neil Cossor initial version
990915	v0.01		    mod for extra param to free_gen_buf_node_q
990923	v0.02		    create update_one_instrument_class so it can be called from
			    query_instrument_classes (old code), and for bi210 processing (new code)
			    added routine find_instrument_class_block_by_series,
				modify find_instrument_class_by_series to use it
			    added fn release_instrument_class_sort_tables,
				mod clear_instrument_classes_from_memory to use it
990928			    mod display_instrument_groups to show more (BBO, block params,
				implied decimals, ranking type)
990929			    mod update_one_instrument_class to grab 'traded' flag,
				and derivative level
000114	v0.03		    fix bug in find_instr_group
000122			    delete point_series_at_instr_classes.. now done automagically
			    in update_one_series as we update things for series
000422	v1.00		    update to match OM release 15.7-3 (move of some fields from da203 to da210).
				update update_one_instrument_class, display_instrument_groups
010312	v1.01		    update to match OM release 16.3
				update update_one_instrument_class to grab MM rule tick-size info
*/
#include "stdincl.h"

#include "error.h"
#include "instrument.h"
#include "series.h"
#include "utility.h"

// ---------------------------------------------------------------------------

static int cmp_instr_class_ptr_by_series ( gen_buf_node_xt ** keybuf,
					    gen_buf_node_xt ** datumbuf )
/*
    routine needed to compare two the data pointed to by the supplied
    params, and return a -1, 0, +1 value for whether the data pointed
    by *keyval is <, ==, > the data pointed to by *datum.

    Comparison is of the four key fields of the instrument class.
    These should be unique within a country / market.

    As there are multiple fields, we are comparing the components in the following
    order
    - country
    - market
    - underlying
    - instrument group

    Used by qsort and bsearch.

    The input parameters are individual pointers to pointers to gen_buf_node_xt.
    We are comparing the instrument_class_xt which are pointed to by the <msg_p> field
    of the gen_buf_node_xt's.
*/
{
    // pointers to ise_underlying_xt component of the gen_buf_node's we were given
    instrument_class_pt keyval, datum;

    // *keybuf == pointer to a gen_buf_node_xt (an element of the sorted array)
    // **keybuf == a gen_buf_node_xt
    gen_buf_node_xt * keybuf_ptr, * datumbuf_ptr;

    keybuf_ptr = *keybuf;
    datumbuf_ptr = *datumbuf;

    keyval = (instrument_class_pt) keybuf_ptr->msg_p;
    datum = (instrument_class_pt) datumbuf_ptr->msg_p;

    // compare the component fields in the bottom structure, in the defined order
    // compare country codes
    if ( keyval->series_x.country_u < datum->series_x.country_u )
	return -1;
    else if ( keyval->series_x.country_u > datum->series_x.country_u )
	return +1;
    // if we get here, we have equal field values. Progress to the next field

    // compare market codes
    if ( keyval->series_x.market_u < datum->series_x.market_u )
	return -1;
    else if ( keyval->series_x.market_u > datum->series_x.market_u )
	return +1;
    // if we get here, we have equal field values. Progress to the next field

    // NB this is the OPPOSITE comparison order to cmp_series_ptr_by_ise_series_instr
    // compare underlying == commodity_code (typically == equity)
    if ( keyval->series_x.underlying_code_u < datum->series_x.underlying_code_u )
	return -1;
    else if ( keyval->series_x.underlying_code_u > datum->series_x.underlying_code_u )
	return +1;
    // if we get here, we have equal field values. Progress to the next field

    // compare instrument group (e.g. American Calls, vs European puts...)
    if ( keyval->series_x.instrument_group_u < datum->series_x.instrument_group_u )
	return -1;
    else if ( keyval->series_x.instrument_group_u > datum->series_x.instrument_group_u )
	return +1;
    // if we get here, we have equal field values. Progress to the next field

    // except there aren't any... the records match
    return 0;
}   // cmp_instr_class_ptr_by_series

// ---------------------------------------------------------------------------

instrument_xt * find_instr_group ( click_details_xt *click_px,
				    uint8 search_instr_group_u )
/*
Function : find_instr_group
Author	 : Neil Cossor
Modified : 20000114
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
{
    // temp, ptr to (chained) data about one instrument, default NULL
    gen_buf_node_xt	* instrument_data_px = NULL;
    // our search val, which we return to the caller
    instrument_xt	*instr_group_px = NULL;

    // point to first entry in chain of known instruments - may be empty
    instrument_data_px = (gen_buf_node_xt *)click_px->our_market_x.known_instruments_q.chain.next;
    do	// while (TRUE)
    {
	// have we got a block of data, containing info about an instrument group
	if ( instrument_data_px == NULL )
	{
	    // end of list, no match
	    return NULL;
	}
	else
	{
	    // point into the actual instrument group data
	    // this is NOT allowed to be NULL
	    instr_group_px = (instrument_xt *)instrument_data_px->msg_p;

	    if ( instr_group_px->instrument_group_u == search_instr_group_u )
	    {
		// found the match - just break out of the loop, and return ptr
		break;
	    }
	    else
	    {
		// no go, try the next instrument - which may be NULL
		instrument_data_px = (gen_buf_node_xt *)instrument_data_px->chain.next;
	    }
	}
    } while (TRUE);

    // return our value to the caller
    return instr_group_px;
}   // find_instr_group

// ---------------------------------------------------------------------------

char * instr_group_name ( click_details_xt * click_px,
			    uint8 search_instr_group_u )
/*
Function:   instr_group_name
Author:     Neil Cossor
Modified:   9907xx
Description:

    Returns (pointer to) string name of the nominated instrument group (by number)
    NULL. if none found with that number
*/
{
    instrument_xt * instr_group_px;

    instr_group_px = find_instr_group ( click_px,
					search_instr_group_u );

    if (instr_group_px)
	return (char *)&instr_group_px->instrname_s;
    else
	return NULL;
}   // instr_group_name

// ---------------------------------------------------------------------------

static void release_instrument_class_sort_tables ( country_market_xt * country_market_px )
/*
Function:   release_instrument_class_sort_tables
Author:     Neil Cossor
Modified:   9907xx
Description:

    release all things to do with instrument classes

    We are exiting, or we have just logged off
    (and the data is no longer known to be valid)
*/
{
    // release the array of (sorted) pointers to the classes
    free ( country_market_px->instr_class_sorted_p );
    country_market_px->instr_class_sorted_p = 0;

}   // release_instrument_class_sort_tables

// ---------------------------------------------------------------------------

void clear_instrument_classes_from_memory ( click_details_xt * click_px )
/*
Function:   clear_instrument_classes_from_memory
Author:     Neil Cossor
Modified:   9907xx
Description:

    release all things to do with instrument classes

    We are exiting, or we have just logged off
    (and the data is no longer known to be valid)
*/
{
    // release the array of (sorted) pointers to the classes
    release_instrument_class_sort_tables ( &click_px->our_market_x );

    // release the block of known instrument classes
    free_gen_buf_node_q ( &click_px->our_market_x.known_classes_q, TRUE );
}   // clear_instrument_classes_from_memory

// ---------------------------------------------------------------------------

void sort_instrument_classes ( country_market_xt * country_market_px,
				int replace_tables )
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
{
    // how big we need each of the pointer arrays to be - NB they are the same size at present
    int size_of_ptr_array;
    int size_of_ptr;
    // temp loop var
    unsigned i;
    // temp, ptr to data about one instrument class, default NULL
    gen_buf_node_xt	* instr_class_node_px = NULL;

    if (replace_tables)
    {
	// first, must delete the old tables
	release_instrument_class_sort_tables ( country_market_px );
    }

    // how big the array of pointers needs to be. NB all pointers are the same size, so pick one for sizeof
    size_of_ptr = sizeof ( instrument_class_pt );
    size_of_ptr_array = size_of_ptr * country_market_px->known_classes_q.length_i;

    // allocate the space for the series pointer arrays
    country_market_px->instr_class_sorted_p = (sorted_array_pt) malloc ( size_of_ptr_array );

    // now fill each of these arrays with pointers to the unsorted instrument class data
    // point to first in Q
    instr_class_node_px = (gen_buf_node_xt *)country_market_px->known_classes_q.chain.next;
    for ( i = 0; i < country_market_px->known_classes_q.length_i; i++ )
    {
	// save ptr to series data, in the "by name" array of ptrs
	country_market_px->instr_class_sorted_p->orig_data_px [i] = instr_class_node_px;

	// advance to the following Q entry, which may be NULL
	instr_class_node_px = (gen_buf_node_xt *)instr_class_node_px->chain.next;
    }

    // calculate how far apart our pointers are in the array
    size_of_ptr = sizeof (instrument_class_pt);

    // now sort each of the arrays. First by name.
    qsort ( &country_market_px->instr_class_sorted_p	    // pointer to first element of array
		    ->orig_data_px [ 0 ],
		country_market_px->known_classes_q.length_i,	// # of entries in the array
		size_of_ptr,				    // size of each element in the array
		// comparison fn
		(int (*) (const void *, const void *)) cmp_instr_class_ptr_by_series );
}   // sort_instrument_classes

// ---------------------------------------------------------------------------

gen_buf_node_xt * find_instrument_class_block_by_series ( country_market_xt *country_market_px,
							    ise_series_xt * ise_series_x )
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

    NB this fn used the sorted array of pointers to instrument class.
    This is set up when we finish query_instrument_class
    It uses this to use the stdlib fn bsearch, instead of a linear search.
    On average, CLICK has (in testing, and worse in prod'n presumably) about
    1800 instrument classes. This means it does 900 comparisons, on average.
    By using bsearch (stdlib.h), this should reduce to approximately
    to 10 (n log 2) comparisons, or about 90 times faster.
*/
{
    // pointer to instrument class node record
    gen_buf_node_xt	    *instr_class_node_px;
    // result from bsearch, which is a pointer to a pointer to a gen_buf_node_xt, which contains the instrument class
    gen_buf_node_xt	    ** instr_class_node_ppx;

    // we need to create a temporary instrument_class_xt, and a pointer to it, so we
    // can call bsearch. Just have one here on the stack - saves allocate / deallocate.
    instrument_class_xt     search_instr_class_x;
    // and a gen_buf_node_xt which contains a pointer to the ise_search_series_x;
    gen_buf_node_xt	    gen_buf_node_search_instr_class;
    // and a pointer to that gen_buf_node_xt
    gen_buf_node_xt	    *gen_buf_node_search_instr_class_px = &gen_buf_node_search_instr_class;
    // copy the search series into search key structure
    search_instr_class_x.series_x = *ise_series_x;
    // and set the <msg> pointer from the gen_buf_node_xt to point to the instrument_class_xt
    gen_buf_node_search_instr_class.msg_p = (char *)&search_instr_class_x;


    // Now do fast search within the sorted chain
    instr_class_node_ppx = bsearch ( 
		&gen_buf_node_search_instr_class_px,			// pointer to search record
		&country_market_px->instr_class_sorted_p		// pointer to first element of array
		    ->orig_data_px [ 0 ],
		country_market_px->known_classes_q.length_i,		// # of entries in the array
		sizeof (instrument_class_pt),				// size of each element in the array
		// comparison fn
		(int (*) (const void *, const void *)) cmp_instr_class_ptr_by_series );
    // did we find a match ?
    if ( instr_class_node_ppx != NULL )
    {
	// match.

	// what we have though is a pointer to an entry in the sorted array,
	// which is in itself a pointer to the original gen_buf_node_xt
	instr_class_node_px = *instr_class_node_ppx;

	// returning pointer to this entry
	return instr_class_node_px;
    }	// test if bsearch found a match

    // oops, no match - report this
    return NULL;
}   // find_instrument_class_block_by_series

// ---------------------------------------------------------------------------

instrument_class_pt find_instrument_class_by_series ( country_market_xt *country_market_px,
								ise_series_xt * ise_series_x )
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

    This calls find_instrument_class_block_by_series
*/
{
    // pointer to the instrument class record
    instrument_class_pt     instrument_class_px;
    // pointer to instrument class node record
    gen_buf_node_xt	    *instr_class_node_px;

    // we need to create a temporary instrument_class_xt, and a pointer to it, so we
    // can call bsearch. Just have one here on the stack - saves allocate / deallocate.
    instrument_class_xt     search_instr_class_x;
    // and a gen_buf_node_xt which contains a pointer to the ise_search_series_x;
    gen_buf_node_xt	    gen_buf_node_search_instr_class;
    // and a pointer to that gen_buf_node_xt
    gen_buf_node_xt	    *gen_buf_node_search_instr_class_px = &gen_buf_node_search_instr_class;
    // copy the search series into search key structure
    search_instr_class_x.series_x = *ise_series_x;
    // and set the <msg> pointer from the gen_buf_node_xt to point to the instrument_class_xt
    gen_buf_node_search_instr_class.msg_p = (char *)&search_instr_class_x;

    instr_class_node_px = find_instrument_class_block_by_series ( country_market_px,
								    ise_series_x );
    // did we find a match ?
    if ( instr_class_node_px != NULL )
    {
	// match.

	// point to the series data in this Q entry
	instrument_class_px = (instrument_class_pt)instr_class_node_px->msg_p;

	// returning pointer to this entry
	return instrument_class_px;
    }	// test if bsearch found a match

    // oops, no match - report this
    return NULL;
}   // find_instrument_class_by_series

// ---------------------------------------------------------------------------

void display_instrument_groups ( click_details_xt * click_px )
/*
Function:   display_instrument_groups
Author:     Neil Cossor
Modified:   20000422
Description:

    Display info about known instrument groups
*/
{
    // temp, ptr to (chained) data about one instrument, default NULL
    gen_buf_node_xt	    * instr_node_px = NULL;
    // temp, ptr to (internal info) ISE version of data about one instrument
    // - destination record where we put the results of the query
    instrument_xt	    * ise_instr_info_px;

    instr_node_px = (gen_buf_node_xt *)click_px->our_market_x.known_instruments_q.chain.next;
    // need to put out column headers ?
    if ( !instr_node_px )
    {
	printf ("No known instrument groups !\n");
	return;
    }	// test if we know any instrument groups

    // we have at least one instrument group - spit out column headers
    printf ( "  # %-32s %-10s %-12s %-13s\n",
		"Instrument name",
		"BBO vol",
		"Block min",
		"Block resp(s)"
	    );
    // now spit out what we know about each of the instruments
    while ( instr_node_px != NULL )
    {
	ise_instr_info_px = (instrument_xt *)instr_node_px->msg_p;
	printf ("%3d %-32s %-10d %-12d %-13.3f\n",
		 ise_instr_info_px->instrument_group_u,
		 &ise_instr_info_px->instrname_s,
		 ise_instr_info_px->bbo_volume_i,
		 ise_instr_info_px->block_order_min_size_i,
		 (float)(ise_instr_info_px->block_order_rsptime_i/1000)
		 );

	// advance to next instrument group - which may be NULL
	instr_node_px = (gen_buf_node_xt *)instr_node_px->chain.next;
    }	// while we have not run out of known instrument groups

}   // display_instrument_groups

// -------------------------------------------------------------------------------------

void update_one_instrument_class ( instrument_class_xt *ise_instr_class_info_px,
				    da210_t *answer_instr_class_px )
/*
Function : update_one_instrument_class
Author	 : Neil Cossor
Modified : 20010313
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
{
    uint16  item_index_u,   // loop var, for extracting tick info for one instr class
	    item_index_2_u; // loop var, for extracting data about expiry date groups

    // OK - all set up to grab the data - remember endianness
    get_click_series ( &ise_instr_class_info_px->series_x,
			&answer_instr_class_px->series );
    // grab the decimals in the strike price
    PUTSHORT ( ise_instr_class_info_px->dec_in_strike_price_u,
		answer_instr_class_px->dec_in_strike_price_n );
    // and the base currency
    rx_click_string(&ise_instr_class_info_px->base_cur_s, 
		    answer_instr_class_px->base_cur_s,
		    sizeof (answer_instr_class_px->base_cur_s));

    // grab the contract size
    PUTLONG ( ise_instr_class_info_px->contract_size_i,
		answer_instr_class_px->contract_size_i );
    // grab the decimals in the contract size
    PUTSHORT ( ise_instr_class_info_px->dec_in_contr_size_u,
		answer_instr_class_px->dec_in_contr_size_n );
    // grab the block order minimum size and response time
    PUTLONG ( ise_instr_class_info_px->block_order_min_size_i,
		answer_instr_class_px->block_order_min_size_i );
    PUTLONG ( ise_instr_class_info_px->block_order_rsptime_i,
		answer_instr_class_px->block_order_rsptime_i );
    // grab the facilitation order minimum size and response time
    //	PUTLONG ( ise_instr_class_info_px->fac_order_min_size_i,
	//	answer_instr_class_px->fac_order_min_size_i ); -not in 17.1
	//  PUTLONG ( ise_instr_class_info_px->fac_order_rsptime_i,
	//	answer_instr_class_px->fac_order_rsptime_i ); -not in 17.1
    // grab the exercise limit and limit unit
    PUTLONG ( ise_instr_class_info_px->exerc_limit_i,
		answer_instr_class_px->exerc_limit_i );
    ise_instr_class_info_px->exerc_limit_unit_u =
		answer_instr_class_px->exerc_limit_unit_c;
    // grab the BBO guaranteed volume
    PUTLONG ( ise_instr_class_info_px->bbo_volume_i,
		answer_instr_class_px->bbo_volume_i );
    // grab the lower limits for the 5 strike price groups
    PUTSHORT ( ise_instr_class_info_px->spg1_low_limit_u,
		answer_instr_class_px->spg1_low_limit_n );
    PUTSHORT ( ise_instr_class_info_px->spg2_low_limit_u,
		answer_instr_class_px->spg2_low_limit_n );
    PUTSHORT ( ise_instr_class_info_px->spg3_low_limit_u,
		answer_instr_class_px->spg3_low_limit_n );
    PUTSHORT ( ise_instr_class_info_px->spg4_low_limit_u,
		answer_instr_class_px->spg4_low_limit_n );
    PUTSHORT ( ise_instr_class_info_px->spg5_low_limit_u,
		answer_instr_class_px->spg5_low_limit_n );
    // whether the instrument class is traded
    ise_instr_class_info_px->traded_u = answer_instr_class_px->traded_c;
    // the derivative level (0=underlying, 1 == derivative on underlying, 2 == derivative on derivative)
    PUTSHORT ( ise_instr_class_info_px->derivative_level_u,
		answer_instr_class_px->derivate_level_n );
    // grab the array of expiration groups - max of 20... commonly 3
    ise_instr_class_info_px->exp_low_limit_count_u =
		answer_instr_class_px->items2_c;
    for ( item_index_2_u = 0;
	    item_index_2_u < ise_instr_class_info_px->exp_low_limit_count_u;
	    item_index_2_u++ )
    {
	PUTSHORT ( ise_instr_class_info_px->exp_date_group_low_limit_u [ item_index_2_u ],
		    answer_instr_class_px->exp_date_group [item_index_2_u].edg_low_limit_n );
    }

    // and the rest of the fields
    PUTSHORT ( ise_instr_class_info_px->rnt_id_u,
		answer_instr_class_px->rnt_id_n );
    PUTSHORT ( ise_instr_class_info_px->dec_in_premium_u,
		answer_instr_class_px->dec_in_premium_n );
    // how many tick_size_t entries do we have - now 2 bytes, so endian
    PUTSHORT ( ise_instr_class_info_px->tick_count_u,
		answer_instr_class_px->items_n );
    // now loop around, transferring in the information about the ticks
    for ( item_index_u = 0;
	    item_index_u < ise_instr_class_info_px->tick_count_u;
	    item_index_u++ )
    {
	// grab the step size, lower limit, and upper limit.. all endian
	PUTLONG ( ise_instr_class_info_px->tick_size_x [ item_index_u ].step_size_i,
		    answer_instr_class_px->item[item_index_u].tick_size.step_size_i );
	PUTLONG ( ise_instr_class_info_px->tick_size_x [ item_index_u ].lower_limit_i,
		    answer_instr_class_px->item[item_index_u].tick_size.lower_limit_i );
	PUTLONG ( ise_instr_class_info_px->tick_size_x [ item_index_u ].upper_limit_i,
		    answer_instr_class_px->item[item_index_u].tick_size.upper_limit_i );
    }   // loop, extracting price tick info about one instrument class

    // and extract the MM-rule tick information - nicely named OM field
    ise_instr_class_info_px->mm_rule_tick_count_u = answer_instr_class_px->items_c;
    for ( item_index_u = 0;
	    item_index_u < ise_instr_class_info_px->mm_rule_tick_count_u;
	    item_index_u++ )
    {
	// grab the step size, lower limit, and upper limit.. all endian
	PUTLONG ( ise_instr_class_info_px->mm_rule_tick_size_x [ item_index_u ].step_size_i,
		    answer_instr_class_px->tss_mm[item_index_u].tick_size.step_size_i );
	PUTLONG ( ise_instr_class_info_px->mm_rule_tick_size_x [ item_index_u ].lower_limit_i,
		    answer_instr_class_px->tss_mm[item_index_u].tick_size.lower_limit_i );
	PUTLONG ( ise_instr_class_info_px->mm_rule_tick_size_x [ item_index_u ].upper_limit_i,
		    answer_instr_class_px->tss_mm[item_index_u].tick_size.upper_limit_i );
    }   // loop, extracting MM-rule tick info about one instrument class

    // as per OM/ISE advice.. traded in fractions if first entry is 625, second is 1250
    // hopefully US industry gets on with it, and we don't run hybrid market for long
    if (( ise_instr_class_info_px->tick_size_x [0].step_size_i == 625 )
	&& ( ise_instr_class_info_px->tick_size_x [1].step_size_i == 1250 ))
    {
	ise_instr_class_info_px->trades_in_decimals_u = FALSE;
    }
    else
    {
	ise_instr_class_info_px->trades_in_decimals_u = TRUE;
    }	// check if fractional trading

}   // update_one_instrument_class


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


Title:	misc.h
Author: Neil Cossor
Description:

    Header file for misc.c, miscellaneous routines for the ISEsim application.

    These are things that aren't really utility routines, but don't really
    justify a whole .h/.c module each... things like bin / exchange related
    functions.

Revision history :-

yymmdd	vers'n	author	    notes
------	------	------	    -----
99xxxx	v0.00	Neil Cossor initial version
000403	v0.01		    add query_missed_general_text, query_business_date
0102xx	V0.02		    change query_market_status to query_instrument_status
			    added market_name, update_trading_state, display_trading_state
*/

#ifndef MISC_H
#define MISC_H




// ---------------------------------------------------------------------------

gen_buf_node_xt * find_bin ( country_market_xt *country_market_px,
				uint8 search_bin_u );
/*
Function:   find_bin
Author:     Neil Cossor
Modified:   9907xx
Description:

    We wish to find the (internal) record about a nominated bin, within
    a nominated country/market.

    Return a pointer to the found record,
    unless we can't find it, in which case return NULL

    As we expect a very small number of bins (i.e. 10),
    just do a linear search, which will check against half of the bins, on average.
*/

// -------------------------------------------------------------------------------------

int32 display_bin ( click_details_xt * click_px,
		    int argc,
		    char *argv [] );
/*
Function:   display_bin
Author:     Neil Cossor
Modified:   9907xx
Description:

    The user wishes to know about bins

    We can tell them the list of underlying in a bin,
    and for each underlying, how many series are defined.

    We can also tell them who the PMM and MMs are.

Input Params:

    argv[0]	command that called us
    argv[1]	the bin # we wish to display
*/

// ---------------------------------------------------------------------------

void display_exchanges ( click_details_xt * click_px );
/*
Function:   display_exchanges
Author:     Neil Cossor
Modified:   9907xx
Description:

    Show the info we have about known exchanges.
    This includes equities exchanges,
    as well as derivaties exchanges
*/

// -------------------------------------------------------------------------------------

void calc_bin_orderbook_combos ( country_market_xt * country_market_px );
/*
Function: calc_bin_orderbook_combos
Author:   Neil Cossor
Modified: 9907xx
Description:

    We have just finished loading all info about underlying
    within a particular country and market.
    
    Figure out how many bins/orderbook combinations there are in this country/market.

    This info can be used in wildcard loops, where we must make multiple
    requests to the various orderbook / bin combinations.

Input Parameters:

    Address of block of data about one country/market. The info
    about the known underlyings, and the max bin and orderbook # we
    have seen in the static load are in here.

Output Parameters:

    Modifies data in the (given) country/market data block,
    in updating the field of the # of known bin/orderboko combinations,
    and in generating an array of data which contains the actual combos.
*/

// ---------------------------------------------------------------------------

ise_exchange_xt * find_exchange_by_country ( click_details_xt * click_px,
						uint8 find_country_u );
/*
Function:   find_exchange_by_country
Author:     Neil Cossor
Modified:   9908xx
Description:

    We have a country #, and we need something else (say the abbreviated
    exchange name) for that... say, for display.

    Search for the appropriate exchange data (loaded at logon), and return
    pointer to that data.

Input params:

    All the data we know about the click system
    The country # we are looking for.

Output params:

    Pointer to a block of exchange data, if we can find a match
    NULL if not.
*/

// ---------------------------------------------------------------------------

char * exchange_to_string ( click_details_xt * click_px,
			    uint8 find_country_u );
/*
Function:   exchange_to_string
Author:     Neil Cossor
Modified:   9908xx
Description:

    We have a country #, and we want the abbreviated exchange name say, for display.

    Search for the appropriate exchange data (loaded at logon), and return
    pointer to string in that data.

Input params:

    All the data we know about the click system
    The country # we are looking for.

Output params:

    Pointer to a (short-form) string name, if we can find a match
    pointer to "Unknown exchange" if not.
*/

// ---------------------------------------------------------------------------

int32 query_opra ( click_details_xt * click_px,
		    int best_u,
		    int argc,
		    char *argv[] );
/*
Function:   query_opra
Author:     Neil Cossor
Modified:   9908xx
Description:

    We wish to know the current OPRA prices.. one of
    -the amalgamated best BBOs and last trade info of all exchanges),
	using the iq111 transaction.
    -BBOs and last trade info from each exchange
	using the iq112 transaction.

Input Params:

    -ptr to all the data we know about CLICK
    -whether we are collecting amalgamated best (iq111) or each (iq112)
	if this param (best_u) is TRUE, then do iq111
    -normal argc,argv[] params from command line

    argv[0]	OPRA
    argv[1]	BEST|EACH
    argv[2]	Underlying|Series
    argv[3]	<underlying name>|<series name>
    (optional)
    argv[4]	<instrument group>, if argv[2] is Underlying.. this -> instrument class

Output Params;

    Results put out on stdout.
*/

// ---------------------------------------------------------------------------

int32 query_instrument_status ( click_details_xt *click_px );
/*
Function:   query_instrument_status
Author:	    Neil Cossor
Modified:   20010226
Description:

    Query to find out the current instrument status.. this can include
    values for markets, for instrument types, or for instrument classes.
*/

// ---------------------------------------------------------------------------

int32 query_missed_general_text ( click_details_xt *click_px );
/*
Function:   query_missed_general_text
Author:	    Neil Cossor
Modified:   20000403
Description:

    Query the system, and retrieve previous BI8 (general text messages).
    These do not appear to be sequenced or timestamps.. so other than knowing
    they are previous.. we don't know from how long ago.
*/
// ---------------------------------------------------------------------------

int32 query_business_date ( click_details_xt *click_px );
/*
Function:   query_business_date
Author:	    Neil Cossor
Modified:   20000403
Description:

    Query the system, and retrieve what date the central system thinks it is.
    Save this for use in CQ1 (query todays date), giving a warning if the
    central system has a different date from our local date.
*/

// ---------------------------------------------------------------------------

void update_trading_state ( click_details_xt *click_px,
				ise_series_xt *ise_series_px,
				uint16 trade_state_number_u );
/*
Function:   update_trading_state
Author:	    Neil Cossor
Modified:   20010301
Description:

    We have update information (from a UQ15 query or a BI41 broadcast)
    for trading states.

    The calling code knows which the data came from, and calls this routine
    as many times as necessary, once for each 'series' which we have info for.

    The 'series' qualifier can be for
    - a market (country and market specified)
    - an instrument type (country, market, and instrument group specified)
    - an instrumnet class (country, market, instr group and commdity code specified)

Input:

    - pointer to everything we know about the market
    - the ISE-format 'series' which qualifies market, instr type, or instr class
    - the new trade state number (in local-endian format). These correspond
	to the state data we retrieved with DQ245.
*/

// ---------------------------------------------------------------------------

char * market_name ( click_details_xt *click_px,
			uint8 country_u,
			uint8 market_u );
/*
Function:   market_name
Author:	    Neil Cossor
Modified:   20010301
Description:

    The caller wants the name of the OM market (as qualified by the 
    country code == exchange code, and the market code)

    For the moment, where the ISE only has one market,
    we only have one to look for.
*/

// ---------------------------------------------------------------------------

void display_trading_states ( click_details_xt *click_px,
				char *start_string_s );
/*
Function:   display_trading_states
Author:	    Neil Cossor
Modified:   20010301
Description:

    We want to show (some of) the known trading states.. and the flags
    that are controlled by that
*/

// ---------------------------------------------------------------------------

unsigned calc_our_default_bin ( click_details_xt *click_px );
/*
Function:   calc_our_default_bin
Author:	    Neil Cossor
Modified:   20010601
Description:

    We have all of the bin/MM data loaded, and we are have just logged on.
    Figure out the default bin for this member.

    If the same member is defined as a MM in multiple bins, it will find
    the first and use that as the default.

    The result bin is displayed, set in the global, and returned to the caller.

    If we are not configured as a MM in any bin, we will come up with a 0.
    No warning will be displayed, as this is normal for an EAM.
*/

#endif

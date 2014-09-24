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


Title : mm.h
Author: Neil Cossor

Function :

    This file is the header file for mm.c,
    which provides MarketMaker specific facilities to the ISESim.exe program.
    This includes things required only by PMMs, and those required
    by PMMs and CMMs.

Revision history :-

yymmdd	vers'n	author	    notes
------	------	------	    -----
9907xx	v0.00	Neil Cossor initial version
991201	v0.01		    add query_locked_orders
000328	v0.02		    add send_block_quote (ref'd by autoqute.c)
			    add delete_quote
010618	v.03		    add count_gtc
010709			    add query_speedbump_parameters
*/
#ifndef MARKETM_H
#define MARKETM_H

// ---------------------------------------------------------------------------

int32 add_quote_component ( click_details_xt * click_px,
			    int argc,
			    char *argv[] );
/*
Function:   add_quote_component
Author:     Neil Cossor
Modified:   9907xx
Description:

    MM wishes to add one component to our table, preparatory to
    sending the quote

    (mandatory) arguments are
    argv[0] QUOTE
    argv[1] BID or ASK
    argv[2] <series name>
    argv[3] premium
    argv[4] quantity
    argv[5] open quantity
    argv[6] step up buffer
*/

// ---------------------------------------------------------------------------

void clear_quote_table ();
/*
Function:   clear_quote_table
Author:     Neil Cossor
Modified:   9907xx
Description:

    We wish to clear out the components in the (in-memory) quote table.
    This can be because of a QUOTE,CLEAR user command, or
    because we succeeded in doing a QUOTE,SEND
*/

// ---------------------------------------------------------------------------

int32 send_quote ( click_details_xt * click_px );
/*
Function:   send_quote
Author:     Neil Cossor
Modified:   9907xx
Description:

    MM wishes to update their quotes.
    Uses mo104.

    This wraps up the quote table components (previously set by
    QUOTE,ASK or QUOTE,BID commands), and sends them as a block
    to the central system.

    If successful, then clears out the quote table.
*/

// ---------------------------------------------------------------------------

int32 query_mm_parameters ( click_details_xt * click_px,
			    int argc,
			    char *argv[] );
/*
Function:   query_mm_parameters
Author:     Neil Cossor
Modified:   9907xx
Description:

    MM wishes to get their MM parameters.
    Uses uq101, by calling send_query_mm_parameters

    The query can only be done at the underlying or instrument class level.

Input Params:

    argv[0]	MM
    argv[1]	GET
    argv[2]	underlying name for query
    (optionally)
    argv[3]	instrument group.. combined with underlying, this gives us instrument class
*/

// ---------------------------------------------------------------------------

int32 query_pmm_parameters ( click_details_xt * click_px,
			    int argc,
			    char *argv[] );
/*
Function:   query_pmm_parameters
Author:     Neil Cossor
Modified:   9907xx
Description:

    PMM wishes to get their PMM parameters.
    Uses uq102.

    This is exactly the same as query_mm_parameters
    EXCEPT that it uses a uq102, instead of a uq101...
    and gets responses which contain additional fields for each component
    response record.

Input Params:

    argv[0]	PMM
    argv[1]	GET
    argv[2]	underlying name for query
    (optionally)
    argv[3]	instrument group.. combined with underlying, this gives us instrument class
*/

// ---------------------------------------------------------------------------

int32 query_locked_orders ( click_details_xt * click_px,
			    int argc,
			    char *argv[] );
/*
Function:   query_locked_orders
Author:     Neil Cossor
Modified:   991201
Description:

    MM wishes to find what locked orders there are - for ALL users.

Input Params:

    the usual command line argc, argv[]
    argv[0]	command that got us here
    argv[1]	S[eries] | U[nderlying] | * (all in bin) - all in bin not supported yet
    argv[2]	<series name> | <underlying name> we are querying on

*/
// ---------------------------------------------------------------------------

int32 query_orderbook ( click_details_xt * click_px,
			int argc,
			char *argv[] );
/*
Function:   query_orderbook
Author:     Neil Cossor
Modified:   9907xx
Description:

    MM wishes to find what the market orderbook looks like - for ALL users.

Input Params:

    the usual command line argc, argv[]
    argv[0]	command that got us here
    argv[1]	series name we are querying on
    (optional)
    argv[2]	bid/ask.. default to get both
*/

// ---------------------------------------------------------------------------

int32 query_quote ( click_details_xt * click_px,
		    int argc,
		    char *argv[] );
/*
Function:   query_quote
Author:     Neil Cossor
Modified:   9907xx
Description:

    MM wishes to find what quotes they have in the market
    Uses mq104, by calling send_query_quote

Input Params:

    The usual argc/argv from the command line

*/

// ---------------------------------------------------------------------------

void display_quote_table ( click_details_xt * click_px );
/*
Function:   display_quote_table
Author:     Neil Cossor
Modified:   9907xx
Description:

    The user would like to see what is in the (in-memory) quote table..
    display it on stdout
*/

// ---------------------------------------------------------------------------

int32 set_mm_parameters ( click_details_xt * click_px,
			    int argc,
			    char *argv[] );
/*
Function:   set_mm_parameters
Author:     Neil Cossor
Modified:   9907xx
Description:

    MM wishes to set their MM parameters.
    Uses ui101.
*/

// ---------------------------------------------------------------------------

int32 set_pmm_parameters ( click_details_xt * click_px,
			    int argc,
			    char *argv[] );
/*
Function:   set_pmm_parameters
Author:     Neil Cossor
Modified:   9907xx
Description:

    PMM wishes to set their PMM parameters.
    Uses ui102.
*/

// ---------------------------------------------------------------------------

int32 request_opening ( click_details_xt * click_px,
			int argc,
			char *argv[] );
/*
Function:   request_opening
Author:     Neil Cossor
Modified:   9908xx
Description:

    PMM wishes to open the nominated list of series / underlying.
    Uses mo113..

Input Params:

    standard C-style arcg/argv params from the command line
    argv[0]	OPEN
    argv[1]	REQUEST
    argv[2]	S[eries] | U[nderlying] | * (all in bin) - all in bin not supported yet
    argv[3]	<series name> | <underlying name> | <ignored if argv[2]=='*'
    argv[4]	<target series status> 2 == queuing & trying to open, 5 == fast market
    argv[5]	<fast market level>, ignored if argv[4] is anything other than fast market
    argv[6]	<ignore away market flag> 0== consider away, 1== ignore away
*/

// ---------------------------------------------------------------------------

int32 query_possible_opening ( click_details_xt * click_px,
				int argc,
				char *argv[] );
/*
Function:   query_possible_opening
Author:     Neil Cossor
Modified:   9908xx
Description:

    PMM wishes to find out what would happen if the PMM tried to open.
    Response tells the PMM about any imbalances etc, which need to be
    addressed before a successful opening. This would typically be
    used by the PMM in the period leading up to the Market being opened.
    Uses mq114.

Input Params:

    Same input selectors as request opening (above)

    EXCEPT..

    Fast Market Level is definitely redundant
    ? is Target series status also redundant..???
*/

// ---------------------------------------------------------------------------

int32 trade_report ( click_details_xt *click_px,
			int argc,
			char *argv[] );
/*
Function:   trade_report
Author:     Neil Cossor
Modified:   9908xx
Description:

    PMM wishes to (manually) trade with against an existing order in the market.
    Uses MO112.

    While technically PMMs can trade against any order, the Legal and Surveillance
    issues are not so simple. Fortunately, this is an SEP - Someone Else's Problem

    Note that if we are using the same ISEsim session as whichever BD/operator
    sent the original order, and that order was snapshot-saved (using the SAVE command),
    then we can use %O to give us the shortcut to the series, orderID, and bid-ask of
    the original order.
    NB are we supposed to be putting in the bid/ask the same as the original order,
    or the opposite (which would be the most 'normal'/obvious thing to do)

Input Params:

    standard C-style arcg/argv params from the command line
    argv[0]	TRADE
    argv[1]	REPORT
    argv[2]	BID/ASK 	- can get %O, but is this correct (see note above)
    argv[3]	series name	- can use %O
    argv[4]	order number	- ditto
    argv[5]	quantity
    argv[6]	premium
    argv[7]	order category (must be CAB or plain order)
*/

// ---------------------------------------------------------------------------

int32 release_locked_order ( click_details_xt * click_px,
				int argc,
				char *argv[] );
/*
Function:   release_locked_order
Author:     Neil Cossor
Modified:   9907xx
Description:

    PMM wishes to release a locked order. They can only release
    Away-better Market Better and FARMM locks...
    Uses mo111..

Input Params:

    standard C-style arcg/argv params from the command line
    argv[0]	RELEASE
    argv[1]	BID/ASK 	- can use %O
    argv[2]	series name	- ditto
    argv[3]	order number	- ditto
    argv[4]	lock_type (Away-Market better, or FARMM)
*/

// ---------------------------------------------------------------------------

int32 send_block_quote ( click_details_xt *click_px,
			    generic_txn_xt *block_quote_px );
/*
Function:   send_block_quote
Author:	    Neil Cossor
Modified:   20000328
Description:

    We have a block quote to send (from manual or autoquote).
    Send it. If all is OK, then update the internals with
    the quoteIDs, quantities and premiums (for later quote
    deletions etc).
*/

// ---------------------------------------------------------------------------

int32 delete_quote ( click_details_xt * click_px,
			int argc,
			char *argv[] );
/*
Function:   delete_quote
Author:     Neil Cossor
Modified:   20000328
Description:

    MM wishes to delete a quote

Input Params:

    normal argc,argv[] params from command line

    (mandatory) arguments are
    argv[0] QUOTE
    argv[1] DELETE
    argv[2] BID|ASK
    argv[3] <series name>

Implicit params:

    the quoting code (sending, querying and broadcasts) maintain the
    quoteID for each series/bid/ask combo.
*/

// ---------------------------------------------------------------------------

int32 panic_delete_quote ( click_details_xt * click_px,
			int argc,
			char *argv[] );
/*
Function:   panic_delete_quote
Author:     Neil Cossor
Modified:   20010524
Description:

    MM wishes to delete all quotes for the member in the bin, both sides

Input Params:

    normal argc,argv[] params from command line

    (mandatory) arguments are
    argv[0] QUOTE
    argv[1] PANIC

Implicit params:

Possible enhancements:

    could have a USER|MEMBER argumemt, which determines whether to pull for
    this user, or for all users for the member. The immediate need is for all
    users, just a matter of whether all bins (multiple members) or one bin.
*/

// ---------------------------------------------------------------------------

int32 count_gtc ( click_details_xt * click_px,
		    int argc,
		    char *argv[] );
/*
Function:   count_gtc
Author:     Neil Cossor
Modified:   20010618
Description:

    MM wishes to count GTC orders
    Uses mq359, by calling send_query_orderbook with count_gtc flag set true

Input Params:

    the usual command line argc, argv[]
    argv[0]	command that got us here i.e. COUNT
    argv[1]	GTC
    argv[2]	S[eries] | U[nderlying] | BIN | * (all in bin)
    argv[3]	<series name> | <underlying name> | bin # we are querying on

*/

// ---------------------------------------------------------------------------

int32 query_speedbump_parameters ( click_details_xt * click_px,
				int argc,
				char *argv[] );
/*
Function:   query_speed_parameters
Author:     Neil Cossor
Modified:   20010709
Description:

    CMM/PMM wishes to get their speedbump parameters (aka mm_exposure_limit).
    Uses uq103, by calling send_query_speed_parameters

    The query can only be done at the underlying level.

Input Params:

    argv[0]	SPEED
    argv[1]	GET
    argv[2]	underlying name for query | "*"
*/

// ---------------------------------------------------------------------------

int32 set_speedbump_parameters ( click_details_xt * click_px,
				int argc,
				char *argv[] );
/*
Function:   set_speedbump_parameters
Author:     Neil Cossor
Modified:   20010709
Description:

    CMM/PMM wishes to set their mm exposure limits (speedbump)
    Uses ui103.

    This normally sets the parameters for exactly one underlying, or whole bin

Input Params:

    standard argc/argv from the command line.. params are
    argv[0]	    SPEED
    argv[1]	    SET
    argv[2]	    <underlying name>, or "BIN"
    argv[3]	    <number of contracts>   must be >0
    argv[4]	    <widen percentage>	    must be >100
*/


#endif

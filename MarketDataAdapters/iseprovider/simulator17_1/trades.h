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


Title : trades.h
Author: Neil Cossor

Function :

    This file is the header file for trades.c,
    which provides basic deal/trade handling to the ISESim.exe program.
    
Revision history :-

yymmdd	vers'n	author	    notes
------	------	------	    -----
9907xx	v0.00	Neil Cossor initial version
010313	v1.00		    update for 16.3, added trade_rectify_parts

*/
#ifndef TRADES_H
#define TRADES_H

// ---------------------------------------------------------------------------

int32 query_todays_deals ( click_details_xt * click_px,
			    int argc,
			    char *argv[ ]);


// ---------------------------------------------------------------------------

void got_a_cl_trade_api ( click_details_xt * click_px,
			    cl_trade_api_t *trade_px );
/*
Function:   got_a_cl_trade_api
Author:     Neil Cossor
Modified:   9907xx
Description:

    We got info about a trade (from CQ1, CQ2, BD5), in particular
    an OM structure cl_trade_api_t.

    Grab the info about the trade.

    (optionally) debug dump the info out onto stdout

Input Params:

    Standard pointer to all things we know about the central system / market
    Pointer to the individual trade record

Output Params:
*/

// ---------------------------------------------------------------------------

int32 query_historical_deals (click_details_xt * click_px,
			    int argc,
			    char *argv[ ] );
/*
Function : query_historical_deals
Author	 : Neil Cossor
Modified : 9907xx
Description : 

    We wish to know something about a past days deals

    Deal sequencing is within instrument group, so if you want to know
    about all deals, you must make a number of queries, at least one per instrument
    group.

    Also, the answer is NOT segmented.. instead, if the max # of deals that can
    fit in a response are supplied, then (at least) one more query should be
    done, using a start sequence # of 1 higher than the last deal that was
    in this response

Input Params:

    argv[0]	TRADES
    argv[1]	HISTORICAL
    argv[2]	<start date - yyyymmdd>
    argv[3]	<end date - yyyymmdd>
    <optional>
    argv[4]	<start time> - hhmmss, default 000000
    argv[5]	<end time> - hhmmss, default 235959 ????
    argv[6]	<instrument group | *>	default to * - get all instrument groups

Output Params:
*/

// ---------------------------------------------------------------------------

int32 trade_rectify ( click_details_xt * click_px,
			int argc,
			char *argv[] );
/*
Function:   trade_rectify
Author:     Neil Cossor
Modified:   9908xx
Description:

    We have (previously) sent an order, which has traded.
    Unfortunately, some of the details relating to settlement
    were incorrect. Presumably OCC has failed the settlement,
    which the B/D knows about (by some other means).

    This transaction (CD33) allows the owner Broker/Dealer
    to 'fix' those incorrect things. The corrections are then
    forwarded to OCC (probably an overnight batch thing).

Input Params:

    the command line stuff (argc,argv[])
    argv[0]	TRADE
    argv[1]	RECTIFY
    argv[2]	<series name>
    argv[3]	<trade #>
    argv[4]	<CMTA member>
    argv[5]	<sub account ID>
    argv[6]	<account type>
    argv[7]	<open / close flag>
    argv[8]	<customer info> i.e. free text / Xref

Output Params:

Notes:

    Can you, as per amending an order, specify to NOT replace some fields,
    by putting NULs in those fields ? If so, then (for example), if the
    caller specifies 0 for the open / close flag, it will NOT change the
    setting for the trade...
*/

// ---------------------------------------------------------------------------

int32 trade_rectify_parts ( click_details_xt * click_px,
			    int argc,
			    char *argv[] );
/*
Function:   trade_rectify_parts
Author:     Neil Cossor
Modified:   0103xx
Description:

    This transaction is similar to the CD33, for altering insensitive
    details of a trade, after the fact (clearing details etc).

    However, the CD36 transaction (new for 16.3) allows the client to
    change the attributes of the trade in pieces.. if the original trade was
    for 100 contracts, the trade could be altered differently for
    a set of 20, 30 and 50.

    Obviously the total quantity specified in all parts cannot exceed the
    total quantity of the trade.

    I presume that if the original was 100, and changes are done on
    lots of 20 and 30, then 50 remain with their original values.

Input Params:

    the command line stuff (argc,argv[])
    argv[0]	TRADE
    argv[1]	RECTIFY
    argv[2]	<series name>
    argv[3]	<trade #>

    argv[4]     <quantity we are changing details for part 1>
    argv[5]	<CMTA member for this qty>
    argv[6]	<sub account ID for this qty>
    argv[7]	<account type for this qty>
    argv[8]	<open / close flag for this qty>
    argv[9]	<customer info for this qty> i.e. free text / Xref

    argv[10]	<quantity we are changing details for part 2>
    ...

Output Params:

*/



#endif

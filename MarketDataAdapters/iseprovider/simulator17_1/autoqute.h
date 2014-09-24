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


Title : autoqute.h
Author: Scott Collins
Function :

    This file is the header for autoqute.c, which provides logic to produce 
    auto quoting based on the Black & Scholes pricing model.
    

Revision history :-

yymmdd	vers'n	author		notes
------	------	------		-----
000110	v0.00	Scott Collins	initial version
000505	v0.01	Neil Cossor	move in autoquote command info, and some headers for fns moved
				from command.c to autoqute.c
*/
#ifndef AUTOQUOTE_H
#define AUTOQUOTE_H

    // declaration of string choices for AUTOQUOTE, argv[1]..
    // where we say all underlying, we mean for our bin
    static char *autoquote_subcommands_s [] =
    {
	"DISABLE",	// turn overall autoquoting off
	"ENABLE",	// turn overall autoquoting on (on by default)
	"INTEREST",	// set the interest rate for some / all underlying
	"OFF",		// turn autoquoting off for some / all underlying
	"ON",		// turn autoquoting on for some / all underlying
	"OPEN_QUANTITY",// sets the quantity, used in opening rotations
	"QUANTITY",	// sets the quantity, used in normal trading
	"SHOW",		// show autoquote settings
	"STEPUP",	// set the step-up buffer quantity
	"VOLATILITY"	// set the volatility rate for some / all underlying
    };
    // enumerated list, corresponding 1:1 (and in same order) as string list
    enum autoquote_subcommands_et { autoquote_disable_e, autoquote_enable_e,
				    autoquote_interest_e, autoquote_off_e, autoquote_on_e,
				    autoquote_openqty_e, autoquote_qty_e,
				    autoquote_show_e, autoquote_stepup_e,
				    autoquote_volatility_e };

// -------------------------------------------------------------------------

void enable_autoquote ();
/*
Function:   enable_autoquote
Author:     Scott Collins
Modified:   000110
Description:

    Enable autoquoting 

Input Params:

    N/A

Output Params:

    N/A
*/

// -------------------------------------------------------------------------

void disable_autoquote ();
/*
Function:   disable_autoquote
Author:     Scott Collins
Modified:   000110
Description:

    Disable autoquoting 

Input Params:

    N/A

Output Params:

    N/A
*/

// -------------------------------------------------------------------------

int32 init_autoquote_io_thread (click_details_xt    *click_px);
/*
Function:   init_autoquote_io_thread
Author:     Scott Collins
Modified:   000110
Description:

    This fn, called once we have logged on, handles quotes and generates 
    autoquotes based on underlying changes found on its queue.


Input Params:

    Pointer to click_details_xt

Output Params:

    pointer the the Queue that is read by the thread


*/

// --------------------------------------------------------------------------

void autoquote_show ( click_details_xt *click_px );
/*
Function : autoquote_show
Author	 : Neil Cossor
Modified : 20000110
Description:

    We have wish to display the autoquote settings

    Whiz thru the underlying, showing those underlying which have autoquote
    values (turned on or non-zero interest or volatility)

    No point looking at any underlying which are NOT our bin,
    as we will not be able to quote them anyway.
*/

// --------------------------------------------------------------------------

int32 autoquote_change ( click_details_xt *click_px,
			    int choice_i,
			    int32 new_value_i,
			    double new_value_f,
			    int argc,
			    char *argv[] );
/*
Function : autoquote_change
Author	 : Neil Cossor
Modified : 20000110
Description:

    We have just had a change in the autoquote settings

    This could be setting interest rate, volatility, qty, open_qty or stepup,
    or turning autoquoting on or off

    All flavours have a common tail.. a list of items we are changing the settings for

Input Params:

    - pointer to everything we know about the current click session / system
    - which particular operation we are doing in changing the autoquote settings
    - an int32, which we are to use if the operation is a set of an integer e.g. step-up buffer
    - a double precision floating point, used if setting a double e.g.interest or volatility
    - C-style count of how many string args which follow
    - array of pointers to strings, each of which specifies an underlying / wildcard 
	we are to apply the autoquote settings to
*/

#endif

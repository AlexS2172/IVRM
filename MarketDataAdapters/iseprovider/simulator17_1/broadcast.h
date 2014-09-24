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


Title : broadcast.h

Function :

    This file is the header for broadcast.c, which provides
    broadcast handling to the ISESim.exe program.
    
Author:  Neil Cossor

Revision history :-

yymmdd	vers'n	author	    notes
------	------	------	    -----
9905xx	v0.00	Neil Cossor initial version
991127	v0.01		    add display_subscriptions, remove subscribe_for_events,
			    add add_one_subscription
000601	v0.02		    add open_tradefile
*/
#ifndef BROADCAST_H
#define BROADCAST_H


// ---------------------------------------------------------------------------

int32 init_bcast_io_thread ( click_details_xt *click_px );
/*
Description:

    This fn, called once we have logged on (and subscribed to the appropriate
    list of broadcasts), starts a thread which handles all subsequent broadcast
    IO with the OMAPI, until we are logged off.

    OMAPI IOs are synchronised between the threads, using the lock
    object click_px->omni_io_active_mutex.
*/

// ---------------------------------------------------------------------------

void enable_optional_bcasts ( click_details_xt * click_px );
/*
Function : enable_optional_bcasts
Author	 : Neil Cossor
Modified : 991112
Description:

    Change our overall condition of paying attention to (optional)
    broadcasts, to not ignore. Note, however, that this does NOT
    actually request (subscribe) to any of the actual broadcast
    type that are available.
*/

// ---------------------------------------------------------------------------

void disable_optional_bcasts ( click_details_xt * click_px );
/*
Function : disable_optional_bcasts
Author	 : Neil Cossor
Modified : 991112
Description:

    Change our overall condition of paying attention to (optional)
    broadcasts, to ignore all
*/

// -------------------------------------------------------------------------

int32 display_subscriptions ( click_details_xt * click_px );
/*
Function : display_subscriptions
Author	 : Neil Cossor
Modified : 991127
Description:

    For the broadcast system, we wish to display what subscriptions we have

    A precondition is that we have already found out the list of authorised optional
    broadcasts (at logon, with query_bcast_events).

Input Params:

    pointer to all the stuff we know about the central system.. we need this so that
    we can translate the commodity codes to underlying symbols.
*/

// -------------------------------------------------------------------------

int32 add_one_subscription ( click_details_xt * click_px,
				char * bcast_type_s,
				int argc,
				char *argv[]);
/*
Function : add_one_subscription
Author	 : Neil Cossor
Modified : 991127
Description:

    We had a command to add a subscription..

Input Params:

    pointer to structure containing everything we know about central system / market
    pointer to broadcast type string, e.g. "BD101"
    count of the # of string arguments, presented in
    pointer to array of strings, containing the list of underlying

Output Params:

    returns INVALID_ARGUMENTS status if bad broadcast type specified or underlying params
    return other error statii from lower routines
*/

// ---------------------------------------------------------------------------

void open_tradefile ( click_details_xt *click_px );
/*
Function:   open_tradefile
Author:	    Neil Cossor
Modified:   20000601
Description:

    Open the trade logfile, for BD111 and BD102 hacks above.. NB they flush after
    each trade write (trades are uncommon c.f. BBOs, OPRA prices etc).

    We don't actually have an explicit file close.. but provided the flushes
    were timely, the implicit close in the application exit should do the trick
    anyway.
*/

// ---------------------------------------------------------------------------

void bcast_set_poll_delay ( uint32 new_poll_delay_u );
/*
Function:   bcast_set_poll_delay
Author:	    Neil Cossor
Modified:   20001219
Description:

    Set a new interval to set for the delay between polls
    for bcasts (in milliseconds).
*/

// ---------------------------------------------------------------------------

void bcast_set_poll_limit ( uint32 new_poll_limit_u );
/*
Function:   bcast_set_poll_limit
Author:	    Neil Cossor
Modified:   20011017
Description:

    We have been given a new interval to set for the limit of the time taken
    for one bcast poll (in milliseconds), before adding warning into log.
*/

// ---------------------------------------------------------------------------

void bcast_set_system_poll_limit ( uint32 new_system_limit_u );
/*
Function:   bcast_set_system_poll_limit
Author:	    Neil Cossor
Modified:   20011017
Description:

    We have been given a new interval to set for the limit of the time taken
    in one interpoll loop by the system (on top of actual IO time, and voluntary Sleep),
    before adding warning into log.
*/


#endif

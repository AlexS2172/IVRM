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


Title : ISESim.c
Author:  Neil Cossor
Description :

    This file contains the startup / shutdown components of the ISESim.exe program.
    The file, main.c, contains the routine which starts things up.. if, of course,
    we are in a pure C, command-line environment... there are other options,
    which could call these routines.
    
    ISESim is a demonstration program for users of the ISE CLICK system,
    as supplied by the OM Gruppen.
    
    It demonstrates connection / logon and disconnection / logoff with the
    ISE CLICK Trading System, the basic transactions of the system, and an example of the
    processing of the broadcasts from the system.
    
    Now modified to be portable on either Windows NT V4 (Intel, Service Pack 3 and higher)
    and Solaris (Sparc). Testing of the Sparc version is, at present, outside
    ISE facilities.
   
Revision history :

yymmdd	vers'n	author	    notes
------	------	------	    -----
9908xx	v0.00	Neil Cossor initial version
990915	v0.01		    mod for extra param to free_gen_buf_node_q
990929	v0.02		    tell process_commands calls are interactive
000111	v0.03		    add call to start up ISE-internal autoquote thread
000122			    mod to be source-portable between NT and Solaris.. thanks Joshua P.
000127	v0.04		    split out main into main.c, reduce #includes
000213	v0.05		    remove redundant =0 assignment after mutex destroy in simulator_shutdown
0012xx	v2.00		    start updates for OM release 16.3 (from 15.7-3)
0101xx			    more of the same
0106xx	v2.01		    updates for OM release 16.5 - init/release Qs for combos
010706			    mod release_market_structures to clear out data collected from previous dq30


*/
// OM header files
#include "stdincl.h"

// and all of our ISEsim stuff
#include "autoqute.h"
#include "command.h"
#include "connect.h"
#include "instrument.h"
#include "logger.h"
#include "series.h"
#include "underlying.h"
#include "utility.h"

// ------------ variables private to this module -----------------------------

// access by other fns is be explicit parameter passing

click_details_xt *click_px;


// -------------- globals, which are directly accessed by other routines -----

    /* the actual storage for user_param filename
	Unlike the other strings, which are either static (from argv),
	the user can logon/off using multiple user_param files, so we can't 
	just have a pointer.. need some actual storage
	These are extern'ed in global.h.. need to restructure */
    static char user_param_filename_s[127];
    // pointer to the User param filename
    char *user_param_filename_ps = (char *)&user_param_filename_s;
    // pointer to the user script filename
    char *user_script_filename_ps;


// ---------------------------------------------------------------------------

void release_market_structures ( click_details_xt * click_px )
/*
Function:   release_market_structures
Author:     Neil Cossor
Modified:   20010608
Description:

    We are about to shut down, or have just logged off.
    We should release all structures about things like
    - known underlying
    - known instrument groups
    - known instrument classes
    - known series
    - known orders
    - known quotes

    etc.. i.e. all things that may change while we are logged off
*/
{
    // clear out the list of known series - this also releases the known_series_q
    clear_series_from_memory ( click_px );
    // clear out the list of known underlyings - this also releases the known_underlying_q
    clear_underlying_from_memory ( click_px );
    // clear out the known instrument classes
    clear_instrument_classes_from_memory ( click_px );
    // the block of known instrument groups
    free_gen_buf_node_q ( &click_px->our_market_x.known_instruments_q, TRUE );
    // the block of known orders
    free_gen_buf_node_q ( &click_px->our_market_x.known_orders_q, TRUE );
    // the block of known combos
    free_gen_buf_node_q ( &click_px->our_market_x.known_combos_q, TRUE );
    // the block of known bin info
    free_gen_buf_node_q ( &click_px->our_market_x.known_bins_q, TRUE );
    // the block of known exchanges info
    free_gen_buf_node_q ( &click_px->our_market_x.known_exchanges_q, TRUE );
    // the block of known synthetics
    free_gen_buf_node_q ( &click_px->our_market_x.known_synthetic_q, TRUE );
    // the block of known trading states
    free_gen_buf_node_q ( &click_px->our_market_x.known_trading_states_q, TRUE );
    // set default market state back to 0
    click_px->our_market_x.state_number_u = 0;
    // clear default tradable instrument class
    click_px->our_market_x.traded_instr_class_px = NULL;
    // wipe the stuff we got from DQ30
    // we are not authorised for any messages
    click_px->trans_query_bcast_u = 0;
    click_px->user_type_name_s[0] = 0;	// zot first byte
    click_px->internal_user_u = 0;
    click_px->non_trader_u = 0;
}   // release_market_structures

// ---------------------------------------------------------------------------

void simulator_shutdown ( void )
/*
Function : simulator_shutdown
Author	 : Neil Cossor 
Modified : 20000213
Description:

    Called on exit from the simulator
    o unloads any script left in memory, 
    o releases all structures, such as our knowledge of underlying, series etc
	i.e. don't trust Windoze to do it correctly...
    o logs out from OMnet (if logged on)
    o tells the logger to close down
*/
{
    // go clear a script, if there is one
    clear_script_from_memory();

    // log off, if we are on.. just to be nice (of course, it may not work)
    if ( click_px->logged_on_i == TRUE)
    {
	// NB if we have a broadcast thread running, it should exit imminently after this anyway
	om_disconnect ( click_px );
    }

    // discard memory structures containing things like underlying, etc
    release_market_structures ( click_px );

    // close the log file, shut down the logger thread etc
    close_logger ();

    // release our synchronisation primative, that was avoiding simultaneous calls to OM API
    ISE_MUTEX_DESTROY ( click_px->omni_io_active_mutex );
}   // simulator_shutdown

// -------------------------------------------------------------------------------

click_details_xt * ISESim_init (int argc, char * argv[])
/*
Function:   ISESim_init
Author :    Neil Cossor
Modified :  20010529
Description:
    
    Initialisation routine for ISESim.

    Tries to do startup.
    
    This includes :-
    
    Trying to startup the logger thread.
    Trying allocate memory for (a structure of data) about a connection
    with OM Click. If the allocation is successful, then sets up the internals
    of that structure

Input params:

    none

output params:

    NULL, is allocation fails,
    otherwise the pointer to the data we just allocated and initialised

implicit params:

*/
{
    // attempt to start the logger - abort if it fails to start up OK
    if ( !init_logger ( user_log_filename_ps  ))
    {
	// abort - error opening log file
	return 0;
    }

    // attempt to allocate memory for the structure containing everything about this CLICK thread / session
    click_px = (click_details_xt *)malloc (sizeof (click_details_xt));
    // did malloc give us some memory for the requisite structure ?
    if (click_px)
    {
	// got memory for the CLICK control structure - zot everything - this will zero
	// all counters etc
	memset ( click_px, 0, sizeof (click_details_xt));

	// initialise all queues in the common. Note that they MUST be initialised,
	// not just zeroed, or the synchronisation calls will fail.

	// the block of known instrument groups
	init_q ( &click_px->our_market_x.known_instruments_q );
	// the block of known series
	init_q ( &click_px->our_market_x.known_series_q );
	// the block of known underlying
	init_q ( &click_px->our_market_x.known_underlying_q );
	// the block of known orders
	init_q ( &click_px->our_market_x.known_orders_q );
	// the block of known combos
	init_q ( &click_px->our_market_x.known_combos_q );
	// the block of known instrument classes
	init_q ( &click_px->our_market_x.known_classes_q );
	// the block of known bins
	init_q ( &click_px->our_market_x.known_bins_q );
	// the block of known exchanges
	init_q ( &click_px->our_market_x.known_exchanges_q );
	// the block of known synthetics
	init_q ( &click_px->our_market_x.known_synthetic_q );
	// the block of known trading states
	init_q ( &click_px->our_market_x.known_trading_states_q );

	// zot the Transaction vectors, which are used in the (synchronised) calls to omniapi_tx
	// vector[0] is set appropriately by the various fns in this module
	click_px->OM_tx_vector_px[0] = NULL;
	click_px->OM_tx_vector_px[1] = NULL;

	// init the structures for storing static data, logging etc
	click_px->log_in_head_x.msg_code_u = CLICK_IN;
	// note that the value in the log_out_head_x is set as required
	// by the calls to do_click_tx_rx and do_click_query

	// set default facility code to 0
	click_px->facility_no_u = 0;

	/* create the synchronisation lock for handshaking between the bcast_io_thread
	    and the main thread...
	*/
	ISE_MUTEX_INIT ( click_px->omni_io_active_mutex );

	// set default autoquote numbers, if not overriden by settings at the underlying level
	click_px->autoquote_quantity_u = 20;
	click_px->autoquote_open_quantity_u = 15;
	click_px->autoquote_step_up_buffer_n = 10;

	// turn autologging ON by default
	click_px->auto_logging_b = TRUE;
	// start up the ISE-internal autoquote thread
	init_autoquote_io_thread ( click_px );
    };	// did malloc give us some memory for the mandatory structure ?

    // things went wrong - hand back a NULL, so the caller knows it failed.
    return click_px;
}   // ISESim_init ()

/******************* End Of File ************************************************************/

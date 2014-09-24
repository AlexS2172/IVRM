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


Title : connect.c
Author:  Neil Cossor

Function :

    This file provides basic connection facilities to the ISESim.exe program.
    This includes connecting, logging on, disconnecting, notifying CLICK of
    readiness to Trade etc
    
Revision history :-

yymmdd	vers'n	author	    notes
------	------	------	    -----
9905xx	v0.00	Neil Cossor initial version
990910	v0.01		    fixed some handling in om_connect, to give better statii back on failure
990914	v0.02		    added code to allow 'weird' mode, where we don't reload CDB on logon
991007	v0.03		    changed code in om_disconnect to always clear the logged_on_i flag
			    and tidy up, regardless of the logoff failing. Caters for attempted
			    logoff, triggered by network failure.
991127	v0.04		    change om_connect.. mechanics of initial subscription handled in broadcast.c
991202			    fix notify_ready_to_trade to set value using PUTLONG (Solaris problem)
000117			    mod log_on, to report the username while logging on
000213	v0.05		    fix.. ref should be ISEsim not ISESim (significant on Solaris)
000620	v0.06		    mod om_disconnect to flip logging off, if autologging ON,
			    and om_connect to flip logging on,   "    "    "
0102xx	v0.07		    zero the omni_login_message before packing various fields
010604	v0.08		    add in retro 15.7 call to calc_our_default_bin
*/
#include "stdincl.h"

#include "broadcast.h"
#include "connect.h"
#include "error.h"
#include "ISEsim.h"
#include "logger.h"
#include "misc.h"
#include "static.h"
#include "underlying.h"
#include "utility.h"

/* whether we are in auto-reload mode for the CDB, default TRUE.

    This is a special flavour for in-house ISE QA testing,
    where we are doing massively repetitive script runs, where
    we keep logging in and out again (as different users), and each
    time we are reloading the CDB data with the DQ queries.

    If we know we are doing this, and our QA scripting is NOT related
    to dynamic alterations to the CDB (BI202, 204, 210 broadcasts),
    then we can force this mode to false, and only load the CDB
    data on the first logon.

    A dirty cheat, which should NEVER be used in any software for
    production purposes...
*/
static auto_cdb_load			= TRUE;
static cdb_loaded			= FALSE;

// -------------------------------------------------------------------------------------

static int32 send_log_on ( click_details_xt * click_px,
			    int8 *Identity_ps )
/*
Function : send_log_on
Author	 : Neil Cossor
Modified : 20010226
Description:

    Assembles and executes the transaction which
    logs the user onto OMNet.

Input Params:

    Pointer to structure which contains everything we know about the CLICK
    system.. including the username, password, gateway etc 
*/
{
    generic_txn_xt	OMmsg_x;    // the CLICK structure we send - contains CLICK and logging headers
    // size of our actual CLICK query - we seem to need this repetitively
    uint32	    click_query_size_u;

	// zero all bytes of the logon msg before we put anything else in
	memset ( &OMmsg_x, 0, sizeof (OMmsg_x));

    click_query_size_u = sizeof (omni_login_message);


    // set up the standard header. NB no central module, server, or trans number
    pack_OM_header ( &OMmsg_x, click_query_size_u, NUL, NUL, 0 );

    // now, we need to put in the required logon fields, like userID and pwd
    // NB the login is the ONLY OM/CLICK transaction where NUL termination is expected
    // so we can transfer from the internal fields to the CLICK fields using simple strcpy
    strcpy (  &OMmsg_x.tx_x.login_x.user_s[0], click_px->user_name_s);
    strcpy (  &OMmsg_x.tx_x.login_x.pass_s[0], click_px->password_s);
    strcpy (  &OMmsg_x.tx_x.login_x.node_s[0], click_px->gateway_name_s);
    strcpy (  &OMmsg_x.tx_x.login_x.sock_s[0], click_px->gw_socket_name_s);
    strcpy (  &OMmsg_x.tx_x.login_x.idnt_s[0], "ISESIM v000" );

    // save the facility code and fn code
    OMmsg_x.facility_u = OMNI_FACTYP_LOGIN;    // fill in standard CLICK IN logger header for input response
    OMmsg_x.fn_code = LOG_K_C_LOGON;
    // and set the message code

    // do the query/rx. Reports error if problem, returns 0 if good
    if ( do_click_tx_rx ( click_px,
			    &OMmsg_x,
			    click_query_size_u ) < 0 )
    {
	printf ("Problem logging onto CLICK\n");
    }

    // set return status and exit fn
    return click_px->log_in_head_x.msg_status_i;
}   // send_log_on

// -------------------------------------------------------------------------------------

static int32 log_on (click_details_xt * click_px,
			char *ProgramName_ps)
/*
Function : log_on
Author	 : Neil Cossor
Modified : 20000117
Description:

    With the supplied user, password, host etc
    performs a synchronous logon to OMNet. Note that 
    this routine does not return successfully until the gateway 
    has appropriately initialised and is ready to
    receive transactions/queries etc
*/
{

    int32   status_i	    =	SUCCESSFUL;
    int32   APIStatus_i     =	SUCCESSFUL;
    uint32  BufferLength_u  =	0;

    assert ( ( click_px->user_name_s != NULL ) 
	     && ( click_px->password_s != NULL )
	     && ( click_px->gateway_name_s != NULL ) 
	     && ( click_px->gw_socket_name_s != NULL )
	     && (ProgramName_ps != NULL )
	   );
    
    
    printf ("Logging on to '%s' as Username '%s' using Socket '%s'...\n", 
	     click_px->gateway_name_s,
	     click_px->user_name_s,
	     click_px->gw_socket_name_s
	     );

    if ((APIStatus_i = send_log_on ( click_px,
					ProgramName_ps)) < SUCCESSFUL)
    {
	printf( "\nLogon Failed!!\n  Check what you typed in, or make sure the system is running.\n");
	status_i = COULD_NOT_LOG_ON_USER;
    }
    else
    {
	// We were able to connect, it may have been a reconnection
	if ( APIStatus_i == OMNIAPI_RECONNECT )
	{
	    printf ("Reconnected to a Previous OMnet Session\n");
	}
	else
	{
	    printf ("Log On Successful\n");
	}

	/* It can take the gateway an indeterminate time to gather its thoughts 
	    after we logon. Whilst it is doing this, it will report OMNIAPI_NOT_READY
	    to all transactions - so we spin on the gateway until it is ready.
	    The actual transaction we do is irrelevant - might as well get
	    the user code and facility code, which we need anyway
	*/
	do
	{
	    APIStatus_i = get_user_information ( click_px );

	    if (APIStatus_i == OMNIAPI_NOT_READY)
	    {
		/* So we don't swamp the gateway with requests, sleep
		    for a little while to let it gather its thoughts
		*/
		Sleep (200);	// milliseconds
	    }
	} while (APIStatus_i == OMNIAPI_NOT_READY);


	if (APIStatus_i >= SUCCESSFUL)
	{
	    // We're ready to roll
	    click_px->logged_on_i = TRUE;

	    // Now that we've successfully logged on, set up an exit function
	    atexit ( simulator_shutdown );
	}
	else
	{
	    /* Something went wrong - we're logged in, but the gateway is
		not responding as we would expect
	     */
	    status_i = GATEWAY_NOT_READY;
	}
    }


    return (status_i);
}   // log_on

// -------------------------------------------------------------------------------------

/*
Function : fetch_cmd_line_logon_info 
Author	 : Neil Cossor
Modified : 9905xx
Description:

    Fetches the Logon information from the command line.

    The results are stored in the data block, in which we have
    everything we know about use and the CLICK system.
*/
static int32 fetch_cmd_line_logon_info ( click_details_xt *click_px,
					    int argc,
					    char *argv[],
					    char *user_param_filename_ps )
{
    int32   status_i = SUCCESSFUL;

    // commandline mode - extract params

    // extract Username
    strcpy ( click_px->user_name_s, argv[1] );

    // extract Password
    strcpy ( click_px->password_s, argv[2] );

    // extract Gateway name
    strcpy ( click_px->gateway_name_s, argv[3] );

    // extract Gateway Socket name/# (as a string)
    strcpy ( click_px->gw_socket_name_s, argv[4] );

    // extract the user parameter filename
    strcpy ( user_param_filename_ps, argv[5] );

    return (status_i);
}   // fetch_cmd_line_logon_info

// -------------------------------------------------------------------------------------

static void prompt_logon_options ()
/*
    User selected something dumb about logon options.. prompt them of their choices
*/
{
    printf ("LOGON                                     OR\n"
	    "LOGON,<user profile filename>             OR\n"
	    "LOGON,<username>,<password>,<gatewayname>,<port>,<user profile filename>\n");
}   // prompt_logon_options

// ---------------------------------------------------------------------------

int32 notify_ready_to_trade (click_details_xt * click_px)
/*
Function : notify_ready_to_trade
Author	 : Neil Cossor
Modified : 991202
Description:

    The last stage in the startup/connect process, we tell the central system
    that we are ready to trade.

    Uses ui1 transaction.
*/
{
    // the CLICK format msg we must pack contents into send
    generic_txn_xt	OMmsg_x;
    // size of our actual CLICK query/command
    uint32	    click_query_size_u;

    // zot the contents of out input buffer - all zeroes
    memset (&OMmsg_x, 
	    (char)0, 
	    sizeof (generic_txn_xt));

    click_query_size_u = sizeof ( application_status_t );

    // set up the header
    pack_OM_header ( &OMmsg_x, click_query_size_u, 'U', 'I', 1);
    // set the status, to notify CLICK that we are ready to rock and roll
    // NB only one value is allowed (1), and that does NOT have a constant / #define
    PUTLONG ( OMmsg_x.tx_x.application_status_x.application_status_i, 1);

    // set the transaction vector to point to the message we wish to send
    click_px->OM_tx_vector_px[0] = &OMmsg_x.header_x;

    // save the facility code and fn code
    OMmsg_x.fn_code = LOG_K_C_APPLICATION_STATUS;
    OMmsg_x.facility_u = click_px->facility_no_u;   // EP0/ET0

    // do the query/rx. Reports error if problem, returns 0 if good
    if ( do_click_tx_rx ( click_px,
	    &OMmsg_x,
	    click_query_size_u ) < 0 )
    {
	// problem - already reported. Report more info and terminate.
	printf ("Could not notify central system that we are ready to trade\n" );
    }
    else
    {
	printf ("Central System notified that we are ready to trade\n");    
    }

    return (click_px->log_in_head_x.msg_status_i);
}   // notify_ready_to_trade

// -------------------------------------------------------------------------------------

int32 om_connect (click_details_xt * click_px,
	       int argc,
	       char *argv [] )
/*
Function : om_connect
Author	 : Neil Cossor / Neil Cossor
Modified : 9905xx
Description:
 
    Collects user information and then logs that user on. 
    Once done, the standing data for that user is loaded. 
*/
{
    /* We perform the following steps:
     * o Collect data from user
     * o Perform actual logon
     * o Get information from system about user
     * o Load standing data for user. 
     */
    int32   status_i	=   SUCCESSFUL;

    if (click_px->logged_on_i == TRUE)
    {
	printf("You are already logged on!\n");
	return COULD_NOT_LOG_ON_USER;
    }

    switch ( argc )
    {
    case 2:
	    // user has supplied the name of a different user_param file
	    strcpy ( user_param_filename_ps, argv[1] );

	    // do NOT break.. drop thru to case 1, where we try to open the file

    case 1: // NOTE case order is deliberate

	    // plain logon - we MUST already have a user_param file, supplied at exe start
	    // OR we have dropped thru from case above...
	    if ( strlen ( user_param_filename_ps ) == 0 )
	    {
		printf ("Error - no user profile file to open\n");
		prompt_logon_options ();
		// set bad status - drop out of routine
		status_i = INVALID_ARGUMENTS;
	    }
	    else
	    {
		/* attempt to load a user param file, with the whole box and dice
		    - username, password, gateway name and port #
		*/
		status_i = load_previous_user ( click_px,
						user_param_filename_ps );
		if ( status_i != SUCCESSFUL)
		{
		    if ( status_i == COULD_NOT_OPEN_USER_FILE )
			printf ("Error opening user file %s\n",
				    user_param_filename_ps );
		    else
			printf ("Invalid user file contents\n");
		}
	    }
	    break;
    case 6:
	    // user has supplied username, password, gateway name, and port name/number
	    status_i = fetch_cmd_line_logon_info ( click_px,
						    argc,
						    argv,
						    user_param_filename_ps
						    );
	    break;
    default:
	    // illegal argument list
	    prompt_logon_options ();
	    status_i = INVALID_ARGUMENTS;
    }

    // still OK, after trying to get usernames etc ?
    if (status_i != SUCCESSFUL)
	return status_i;

    // OK, try to logon
    status_i = log_on (click_px, SIM_VERSION);
    if ( status_i != SUCCESSFUL)
    {
	// do nothing - the reason for aborting is recorded in status_i
	return status_i;
    }

    // Logon was OK - save the file if the client supplied new values
    if (argc == 6 )
	save_user_param_file ( click_px,
				user_param_filename_ps ); // for possible use next time

    // always go and query about our user-type, and allowed transactions
    // OK so far, try to info about our user - what transactions and queries are we allowed
    query_user_type (click_px);

    /* load the CDB data - unless, of course, we are in 'weird' mode, where we
	are doing ISE QA tests with a static CDB.. and improving our QA run times.
	We must, of course, load the CDB once, even in this odd mode
    */
    if (( cdb_loaded == FALSE ) || ( auto_cdb_load == TRUE ))
    {
	// attempt to load the CDB, and note if it worked
	status_i = load_static_data (click_px);
	if ( status_i == SUCCESSFUL)
	    cdb_loaded = TRUE;
    }


    // we know about the bin/mm allocations, but we might be a user/member
    // in a different bin.. go figure it out
    calc_our_default_bin (click_px );

    // are we still OK to continue with the logon sequence ?
    if ( status_i != SUCCESSFUL)
    {
	// do nothing - the reason for aborting is recorded in status_i
    }
    else if ( status_i = init_bcast_io_thread ( click_px ) != SUCCESSFUL ) 	// kick off broadcast thread. It dies once we log off
    {
	// do nothing - the reason for aborting is recorded in status_i
    }
    else if ( (status_i = notify_ready_to_trade (click_px)) != SUCCESSFUL)
    {
	// do nothing - the reason for aborting is recorded in status_i
    }
    else
    {
	// if autologging, turn on logging, provided that logon worked
	if ( click_px->auto_logging_b == TRUE )
	{
	    logging_on ();
	    printf ("Logging is now ON (auto)\n");
	}   // test if autologging is on.. if so, we should turn logging on
    }

    return (status_i);
}   // om_connect

// -------------------------------------------------------------------------------------

int32 om_disconnect (click_details_xt * click_px)
/*
Function : om_disconnect
Author	 : Neil Cossor
Modified : 20000620
Description:

    Logs the user off and disconnects the session to the exchange gateway
*/
{
    // the CLICK format msg we must pack contents into send
    generic_txn_xt	OMmsg_x;
    // size of our actual CLICK query/command - we seem to need this repetitively
    uint32	    click_query_size_u;


    if (click_px->logged_on_i)
    {
	// report that we are trying to Log off from CLICK - report node and port
	printf ("Attempting to Log off CLICK\n" );

	click_query_size_u = sizeof (transaction_type_t);

	// set up the standard header. NB no central module, server, or trans number
	pack_OM_header ( &OMmsg_x, click_query_size_u, NUL, NUL, 0 );

	// save the facility code and fn code
	OMmsg_x.facility_u = OMNI_FACTYP_LOGOUT;
	OMmsg_x.fn_code = LOG_K_C_LOGOUT;

	// do the query/rx. Reports error if problem, returns 0 if good
	if ( do_click_tx_rx ( click_px,
		&OMmsg_x,
		click_query_size_u ) < 0 )
	{
	    printf_time ();
	    printf ("Problem logging out of CLICK\n");
	}
	else
	{
	    // the logoff worked, we need to generate a "success" msg back to the client

	    // report success
    	    printf_time ();
	    printf ("CLICK logoff successful\n" );
	}

	click_px->logged_on_i = FALSE;

	// if autologging, turn logger off, regardless of success / failure
	if ( click_px->auto_logging_b == TRUE )
	{
	    logging_off ();
	    printf ("Logging is now OFF (auto)\n");
	}


	// unload the known orders, series, underlyings, instrument classes etc
	// unless, of course, we are in 'weird' mode, where we do not reloading CDB on each logon
	if ( auto_cdb_load )
	{
	    // release CDB data, and note that we have
	    release_market_structures ( click_px );
	    cdb_loaded = FALSE;
	}   // test if we should be ditching our known CDB data
    }
    else
    {
	printf ("\nLogoff invalid - not currently signed on !\n");
    }	// test if we think we are currently logged on

    // set return status and exit fn
    return (click_px->log_in_head_x.msg_status_i);
}   // om_disconnect

// ---------------------------------------------------------------------------

void turn_auto_cdb_load_off ()
/*
Function:   turn_auto_cdb_load_off
Author:	    Neil Cossor
Modified:   990914
Description:

    We are doing QA script testing with static CDB.. so we will crank up
    our testing by NOT reloading the CDB at each logon...
*/
{
    // move into 'weird' mode
    auto_cdb_load = FALSE;
}   // turn_auto_cdb_load_off

// ---------------------------------------------------------------------------

void turn_auto_cdb_load_on ()
/*
Function:   turn_auto_cdb_load_on
Author:	    Neil Cossor
Modified:   990914
Description:

    We wish to switch back the default behaviour.. reloading the CDB at each logon...
*/
{
    // move out of 'weird' mode - back to normal
    auto_cdb_load = TRUE;
}   // turn_auto_cdb_load_on


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


Title:	error.c
Author: Neil Cossor
Revision history :-

yymmdd	vers'n	author	    notes
------	------	------	    -----
9907xx	v0.00	Neil Cossor initial version
000122	v0.04		    mod to be source-portable between NT and Solaris.. thanks Joshua P.
000321	v0.05		    mod to report special OMNIAPI_VER_FIELD_ERROR case better.
*/
#include "stdincl.h"

#include "utility.h"
#include "error.h"
#include "logger.h"

//-----------------------------------------------------------------------------------

void report_click_error (click_details_xt * click_px,
			    int32 error_code,
			    int32 secondary_code)
			    //quad_word trans_id)not referenced anywhere -eric
/*
Function:   report_click_error
Author:	    Neil Cossor
Modified:   20000321
Description:

    got a bad status return from omniapi_<X> function call.
    Go get some text details for it, and report it into the log

    NOTE we cannot call omni_get_message, to translate codes to strings,
    until we are logged in. Brilliant eh.

    The secondary status code (and error string) is included if non-zero
*/
{
    int32 call_status_i;	// temp for catching statii from called fns

    // buffer to put the string error into NB result is NOT NUL terminated
    char error_string_buf[255];
    // var for specifying size of string buf on input, and resulting len on call complete
    uint32 error_string_len = sizeof (error_string_buf);

    // buffer to put the string error into NB result is NOT NUL terminated
    char sec_error_string_buf[255];
    // var for specifying size of string buf on input, and resulting len on call complete
    uint32 sec_error_string_len = sizeof (sec_error_string_buf);

    // overall output buffer, which we can add to the logfile - result is NUL terminated
    char output_buf[512];

    // default the strings to NUL - we don't try to manually
    // generate the string for the secondary if we are offline
    memset ( &error_string_buf[0], 0, sizeof (error_string_buf));
    memset ( &sec_error_string_buf[0], 0, sizeof (sec_error_string_buf));
    memset ( &output_buf[0], 0, sizeof (output_buf));

    // test if we are in good state to translate error codes to strings
    if ( click_px->logged_on_i == TRUE )
    {
	// we are logged in, we can go get the text for the nominated error

	// grab exclusive access to OMNIAPI - i.e. block out any other threads
	ISE_MUTEX_LOCK ( click_px->omni_io_active_mutex );

	call_status_i = omniapi_get_message ( error_code,
			    (char *)&error_string_buf,
			    &error_string_len,	
			    FALSE );	    // always collect complex info

	// get text for secondary code.
	// special case.. if primary status is OMNIAPI_VER_FIELD_ERROR, then secondary is the offset of the erroneous bytes
	if (error_code == OMNIAPI_VER_FIELD_ERROR)
	{
	    sprintf ( (char *)&sec_error_string_buf,
			"Field error offset %u bytes, from start of msg\n",
			secondary_code
		    );
	}
	else
	{
	    // not special OMNIAPI_VER_FIELD_ERROR

	    // NB, not much point translating a secondary code if it is 0.. this just translates to "SUCCESSFUL"
	    if ( secondary_code != 0)
	    {
		call_status_i = omniapi_get_message ( secondary_code,
				    (char *)&sec_error_string_buf,
				    &sec_error_string_len,	
				    FALSE );	// always collect complex info
	    }
	}   // test if "special" primary error, which has different interpretation of secondary status

	// release the synchronisation lock so other threads can do OMNIAPI IOs
	ISE_MUTEX_UNLOCK ( click_px->omni_io_active_mutex );

	// how successful were we in getting the error text
	switch (call_status_i)
	{
	    // no error code translation found - oops
	    case OMNIAPI_NOT_FOUND:
		strcpy ( (char *)&error_string_buf, "No text translation found for code" );
	    break;

	    // we didn't give it enuff buffer. Truncate it and put a "|" as the final char
	    case OMNIAPI_TRUNCATED:
		// reduce the length by 1 - don't want to overwrite following memory
		error_string_len--;
		error_string_buf [error_string_len] = NUL;
		error_string_buf [error_string_len - 1] = '|';
	    break;

	    // got the whole she-bang. Put a terminating NUL into the right spot
	    case OMNIAPI_SUCCESS:
		error_string_buf [error_string_len] = NUL;
	    break;
	}
    }	// test if we are in good state to translate error codes to strings
    else
    {
	// not logged in, we cannot ask omniapi for error_code->text translation. Clever, eh ?
	// produce text based on "known" error - code vs text from omniapi.h
	switch (error_code)
	{
	case OMNIAPI_FAILURE :
	    strcpy ( (char *)&error_string_buf, "Failure completion" );
	    break;
	case OMNIAPI_NOT_READY :
	    strcpy ( (char *)&error_string_buf, "OAPI not ready" );
	    break;
	case OMNIAPI_FACID_NOT_VALID :
	    strcpy ( (char *)&error_string_buf, "Facility Id not in range" );
	    break;
	case OMNIAPI_INVALID_TABLE :
	    strcpy ( (char *)&error_string_buf, "Invalid table type" );
	    break;
	case OMNIAPI_NOT_LOGGED_IN :
	    strcpy ( (char *)&error_string_buf, "OAPI not logged in" );
	    break;
	case OMNIAPI_LOCKED :
	    strcpy ( (char *)&error_string_buf, "OAPI locked" );
	    break;
	case OMNIAPI_ALREADY_LOCKED :
	    strcpy ( (char *)&error_string_buf, "OAPI already locked" );
	    break;
	case OMNIAPI_NET_INIT_FAIL :
	    strcpy ( (char *)&error_string_buf, "OAPI network initialization failed" );
	    break;
	case OMNIAPI_NOT_INITIALIZED :
	    strcpy ( (char *)&error_string_buf, "OAPI not initialized" );
	    break;
	case OMNIAPI_NO_INFO_RCVD :
	    strcpy ( (char *)&error_string_buf, "No network information received" );
	    break;
	case OMNIAPI_NORTR :
	    strcpy ( (char *)&error_string_buf, "VAX RTR connection failed" );
	    break;
	case OMNIAPI_TX_ABORTED :
	    strcpy ( (char *)&error_string_buf, "Transaction aborted" );
	    break;
	case OMNIAPI_TRUNCATED :
	    strcpy ( (char *)&error_string_buf, "Data truncated" );
	    break;
	case OMNIAPI_CNV_NO_RANGE :
	    strcpy ( (char *)&error_string_buf, "Table conversion range error" );
	    break;
	case OMNIAPI_CNV_NOT_SORTED :
	    strcpy ( (char *)&error_string_buf, "Table conversion sort error" );
	    break;
	case OMNIAPI_CNV_OFFS_ERROR :
	    strcpy ( (char *)&error_string_buf, "Table conversion offset error" );
	    break;
	case OMNIAPI_NO_SUCH_ID :
	    strcpy ( (char *)&error_string_buf, "Invalid transaction type" );
	    break;
	case OMNIAPI_VER_FIELD_ERROR :
	    strcpy ( (char *)&error_string_buf, "Transaction verification error" );
	    break;
	case OMNIAPI_VER_INT_ERROR :
	    strcpy ( (char *)&error_string_buf, "Message verific. internal error" );
	    break;
	case OMNIAPI_VER_TABLE_ERROR :
	    strcpy ( (char *)&error_string_buf, "Table verification error" );
	    break;
	case OMNIAPI_TX_TIMEOUT :
	    strcpy ( (char *)&error_string_buf, "Transaction timeout" );
	    break;
	case OMNIAPI_TX_DECLFAIL :
	    strcpy ( (char *)&error_string_buf, "Transaction server/requester declaration failure" );
	    break;
	case OMNIAPI_TX_FAILURE :
	    strcpy ( (char *)&error_string_buf, "Transaction failure" );
	    break;
	case OMNIAPI_DYNMEM :
	    strcpy ( (char *)&error_string_buf, "Error obtaining dynamic memory" );
	    break;
	case OMNIAPI_INVARG :
	    strcpy ( (char *)&error_string_buf, "Invalid argument" );
	    break;
	case OMNIAPI_NOT_FOUND :
	    strcpy ( (char *)&error_string_buf, "Requested data not found" );
	    break;
	case OMNIAPI_ITV_ERROR :
	    strcpy ( (char *)&error_string_buf, "Information table verification error" );
	    break;
	case OMNIAPI_NO_USR_OR_PASSW :
	    strcpy ( (char *)&error_string_buf, "Username and/or Password missing" );
	    break;
	case OMNIAPI_NO_NET_PATH :
	    strcpy ( (char *)&error_string_buf, "Net Path data missing" );
	    break;
	case OMNIAPI_INVEVT :
	    strcpy ( (char *)&error_string_buf, "Invalid event type" );
	    break;
	case OMNIAPI_NOT_LOCKED :
	    strcpy ( (char *)&error_string_buf, "OAPI not locked" );
	    break;
	case OMNIAPI_INVTXTYPE :
	    strcpy ( (char *)&error_string_buf, "Invalid transaction type" );
	    break;
	case OMNIAPI_NOFACILITY :
	    strcpy ( (char *)&error_string_buf, "OMnet facility not found" );
	    break;
	case OMNIAPI_HIGHRTRCHN :
	    strcpy ( (char *)&error_string_buf, "Expected RTR channel # exceeded" );
	    break;
	case OMNIAPI_FATAL :
	    strcpy ( (char *)&error_string_buf, "Fatal OAPI error" );
	    break;
	case OMNIAPI_NOORDERID :
	    strcpy ( (char *)&error_string_buf, "No order id" );
	    break;
	case OMNIAPI_APINORSP :
	    strcpy ( (char *)&error_string_buf, "Api not responding, GBLSEC reinitialized" );
	    break;
	case OMNIAPI_APIDISCON :
	    strcpy ( (char *)&error_string_buf, "Api disconnected, GBLSEC reinitialized" );
	    break;
	case OMNIAPI_NOTAUTH :
	    strcpy ( (char *)&error_string_buf, "Subscription not authorized, or failed" );
	    break;
	case OMNIAPI_RECONNECT :
	    strcpy ( (char *)&error_string_buf, "successful reconnection" );
	    break;
	case OMNIAPI_SUCCESS :
	    strcpy ( (char *)&error_string_buf, "normal OMNIAPI completion" );
	    break;
	case OMNIAPI_PROBLEM :
	    strcpy ( (char *)&error_string_buf, "error in OMNIAPI internal call" );
	    break;
	case OMNIAPI_INTFAILURE :
	    strcpy ( (char *)&error_string_buf, "internal OMNIAPI error" );
	    break;
	case OMNIAPI_BADNARGS :
	    strcpy ( (char *)&error_string_buf, "bad number of arguments" );
	    break;
	case OMNIAPI_BADARGVAL :
	    strcpy ( (char *)&error_string_buf, "bad argument value" );
	    break;
	case OMNIAPI_NONETWORK :
	    strcpy ( (char *)&error_string_buf, "no network is present for IPC" );
	    break;
	case OMNIAPI_OSBADCONFIG :
	    strcpy ( (char *)&error_string_buf, "O/S incorrectly configured" );
	    break;
	case OMNIAPI_OAPI :
	    strcpy ( (char *)&error_string_buf, "problem in the internal OM API" );
	    break;
	case OMNIAPI_NOTCONNECTED :
	    strcpy ( (char *)&error_string_buf, "invalid operation before LOGIN" );
	    break;
	case OMNIAPI_NOGWYSRV :
	    strcpy ( (char *)&error_string_buf, "no gateway service defined" );
	    break;
	case OMNIAPI_BADHOSTNAME :
	    strcpy ( (char *)&error_string_buf, "host name could not be translated" );
	    break;
	case OMNIAPI_ERRSOCKET :
	    strcpy ( (char *)&error_string_buf, "socket could not be allocated" );
	    break;
	case OMNIAPI_ERRCONNECT :
	    strcpy ( (char *)&error_string_buf, "system error occured on 'connect'" );
	    break;
	case OMNIAPI_ERRBIND :
	    strcpy ( (char *)&error_string_buf, "system error occured on 'bind'" );
	    break;
	case OMNIAPI_NOSESSION :
	    strcpy ( (char *)&error_string_buf, "session is aborted" );
	    break;
	case OMNIAPI_ERRSEND :
	    strcpy ( (char *)&error_string_buf, "error on send()" );
	    break;
	case OMNIAPI_ERRMEM :
	    strcpy ( (char *)&error_string_buf, "error on malloc()" );
	    break;
	case OMNIAPI_APIOLD :
	    strcpy ( (char *)&error_string_buf, "OMNIAPI too old for the gateway" );
	    break;
	case OMNIAPI_RECONREJ :
	    strcpy ( (char *)&error_string_buf, "reconnection rejected" );
	    break;
	case OMNIAPI_SESINUSE :
	    strcpy ( (char *)&error_string_buf, "gateway API session already in use" );
	    break;


	default:
	    // unknown error - one NOT defined in omniapi.h
	    // we can only report our current mode, and the hex value of the error code
	    sprintf ( (char *)&error_string_buf,
			"Unknown error 0x%08x, untranslatable while offline",
			error_code );
	    break;

	};  // switch on error code, while not logged in
    }	// test if we are logged in

    // report the error number, decimal and hex, with string.. NOT including newline
    // NB we are building up a string which we can add to the logfile...
    sprintf ( (char *)&output_buf,
		"        CLICK error [%d,0x%08x], [%s]",
		error_code, error_code, error_string_buf );
    // spit out onto display, including newline
    printf ( "%s\n", &output_buf );

    // report the secondary status string, if non-zero
    if (secondary_code != 0)
    {
	// report the secondary error number, decimal and hex, with string, onto display
	printf ( "\tsecondary status [%d,0x%08x], [%s]\n",
		    secondary_code, secondary_code, sec_error_string_buf );
	// add to the string which we will log
	sprintf ( (char *)&output_buf + strlen (output_buf),
		    "        secondary status [%d,0x%08x], [%s]",
		    secondary_code, secondary_code, sec_error_string_buf );
    };

    // log both our output string
    log_text ( (char *)&output_buf );
}   // report_click_error


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


Title : logger.c
Author: Neil Cossor

Revision History :-

Date	Version Initials    Comments
9906xx	V1.00	NAC	    initial write
990815	v1.01	NAC	    add extensions to allow for orderID/transID being known type
			    and new fns which allow for suppression of bcasts, orderIDs
990908		NAC	    changed log_bcast to do more stuff internally, don't need header in click_details_t
			    added new routine log_text, to allow addition of text msgs to log
			    altered dump_msg_ise_header to always include primary status
			    altered dump_<>_block to put out header in different order (header after fn)
990915	v1.02		    mod for extra param to free_gen_buf_node_q
991005	v1.03		    new routines log_header_on, log_header_off to change suppression of log headers
991013	v1.04		    fix bug in dumping click string which as defined as longer than end of msg
			    fix bugs in dump_substruct_msg - calc of bytes_left_in_msg_i was incorrect (could go -ve)
991018			    check for reasonable string length data in msg_def file (types 'a' and 's')
			    change dump_click_string to still spit out the "" chars, when the strlen () == 0
991019			    enhance to expand CLICK expiry dates (in all binary series) into readable form
991124	v1.05		    enhance dump_timestamp to include milliseconds, if on WinNT platform
000122	v1.06		    mod to be source-portable between NT and Solaris.. thanks Joshua P.
000127	v1.07		    fix bug in dump_timestamp, in reporting detailed (millisecond) timestamps
000204	v1.08		    remove explicit includes of process.h and sys/timeb.h.. in stdincl.h if required
			    add #undef's for FALSE and TRUE before #defines (Microsoft C doesn't case, Solaris does)
				(Solaris compat)
			    mod read_msg_defs to always declare space for filename (const.h caters for Solaris path)
			    mod address passing method in a couple of places in dump_substruct_msg
			    mod logger_init address ref of LOGFILENAME
000213	v1.09		    fix initialisation bug for loggerTerminateEvent,
			    add mutex unlock into close_logger, and lock in dump_log_msgs
			    enhance text-scan processing in read_msg_defs, to check for invalid input file
000612	v1.10		    turn timestamps ON by default (now we are playing prodn games)
010523	v1.11		    change max # fields per message from 100 to 150 (OM release 16.5 takes ca2->102 fields)

Description:

    This file provides all code for a logging thread for the ISESim
    simulator / test bed / utility. This involves

    (init)
	- open the log file, read the log file defns etc,
	- create a multi-thread sychronised Q for msgs to log
	- create a thread which (forever) looks (synchronised) at that Q.
	    When it finds a msg, it (synchronised) removes that (whole) msg, and writes it to
	    the log file. etc...
    (forever)
    - a request to log, by the other application threads, involves allocating a buffer,
	putting in the log data, and (synchronised) adding it to the logger Q.
    (until)
    - a call to log_exit

    The logger 
    - handles different size repeat count fields (e.g. item_c / item_n)
    - handles nested repeat structures
    - handles fixed # of repeats (0 sized repeat) i.e. fixed repeated array

    Note that as ALL messages come and go from OM, with the standard 3-field header
    e.g. 'B' 'O' 1, they are assumed to be at the start of EVERY message, and
    should NOT be defined in each msg in our defs file.

To do:

    Smarter read_thread, which is runs until it has an empty Q, and is somehow
    woken when a new msg is inserted into the Q.
    Currently has a some sleep fn, to stop the thread hogging the CPU,
    which restricts the rate at which msgs can be logged...

    Log facilities routines logger.c (built under Visual C++, should be ANSI C)
    The fn's in this module allow for the text logging of all messages handled by
    the ISEsim system.

    Mod to improve durability while logging, for weird data and / or incorrect
    data defs, which mean the logger is NOT handling the formats correctly.
    We wish the logger to hang in there, and not disturb the active client session,
    by crashing, and taking out the main threads.

    NB some things are different for NT and UNIX..so conditional ifdef/ifndef compilation.
    Notably, support for native 64-bit numbers is missing from standard C (Solaris).
    Thread support code is very different between NT and Solaris.
    Fine-granularity timestamps are available from NT services - don't know about
    Solaris fns yet.

    Routines which are private to this module are declared static, which makes them
    module-private and avoids the possibility of link clashes, as well as outsiders
    calling the fns directly.
    
    We probably should differentiate between an internal and external log_exit.
    One is called becoz we have an unrecoverable internal logger error (e.g. can't get
    a memory buffer), and we must exit immediately, just flushing and close the files.
    The external one is a more polite one, called when the external system (ISEsim)
    just wishes to close. In this case, we should force the logging of
    all messages currently queued for that, before flushing and closing the log file.
    This saves us missing out on logging the tail end messages, which will probably
    dump perfectly.

    Should enhance, to allow one message to be identical to another, with just the different
    (internal) message codes. This would allow for the (not uncommon) case where
    the messages which must be processed differently all have the same data format.
    At present we just repeat the message defn, which means any change to the common
    format means repeating the same correction to several defns in the message defn file.

    The way of storing / looking up the information about a particular msg defn,
    uses a fixed array. While it uses an lower msg # offset, and an upper limit,
    to reduce the wasted memory (size MAX_API_MSGNO - MIN_API_MSGNO), it is still inefficient
    (on memory). It should use a much smaller array (size MAXMSGDEFS). Ref notes
    on the array "msgs_in_use_i". As it is only 4-bytes per entry, it is not
    that much waste, so the extra code / debug is probably unjustified.
*/



// ---------------------------------- includes -------------------------------------

#include <stdlib.h>		// includes the memory allocation fns

#include "stdincl.h"	    /* standard includes, which in turn brings in
	    stdio.h		includes printf fn etc, used to report interesting things
	    time.h		includes time functions - needed for timestamping, sleep etc
	    limits.h
	    assert.h

	    omni.h
	    omniapi.h
	    omnifact.h
	    omex_usa.h

	    const.h
	    global.h
			    */
#include "utility.h"	    // includes the required (synchronised) Q management routines



// ----------------------------- constants, structures etc ----------------------------

#define LOGFILENAME "LOGFILE.TXT"   // name of the logfile, in whatever is our default directory
#define IDLE_WAIT 100		    // how many msecs to wait if we found nothing to log


/* --------- manually defined ref to printf_time, which resides in utility.c -------------
    Don't want to create circular references within header files
*/
void printf_time ();
/*
    fn which grabs the current date/time, and spits out the result in readable form to stdout
    It does NOT add any newlines etc, just a space after the date/time.
    This way, we can get date/time at the start of lines of text logged.
*/


// defined constants which satisfy the normal C 0 == false, non-zero == true
#undef FALSE
#define FALSE 0
#undef TRUE
#define TRUE 1

// program constants

// log files are open-if-exist / create with extend
#define logfilemode "a+"		    // output log files are "append" opened
#define msgdeffilemode "r"		    // file mode to read msg defns is readonly.
#define MSGDEFFILENAME "ISE_MSG_DEFS.DAT"   // name of message defn file

#define MAX_LINE_LEN	132	// length of our output line in the log file - chosen for standard old-style printers
#define LEN_SPECIAL_STR 3	// length of a "special, including leading space e.g. \0 or \1
#define LEN_EXPDATE_STR	11	// length of a expiry date, including leading space and separators " yyyy-mm-dd"

// constants to do with logging msgs (or the data we keep about them)
#define MAXMSGNAME	64	// max length of the "name" we have for a message
#define MAXFIELDNAME	64	// max length of the "name" we have for a field
#define MAXMSGDEFS	150	// max number of msgs which we may define

#define MAXFIELDSPERMSG 150	/* number of fields per API msg ..currently need about 102 for ca2 in 16.5 OM release
				    This determines the size of some NxM tables, so it changes the
				    memory requirements of this module fairly dramatically.

				    The message defn parse should probably report the max #
				    that we actually find, so there is some warning when a rebuild
				    is required.
				*/

// internal values used to store the info about the known message formats
#define FIELDUNDEFINED	0	// field not defined
#define FIELDCHAR	1	// field is a character, of length 1
#define FIELDNULSTRING	2	// field is a character string, with a length, incl room
				// for a \0 terminator. E.g. len 7 = field of 6, plus null
#define FIELDPAD	3	// a pad count - 1..3
#define FIELDUNSIGNED	4	// field is binary, unsigned, of some length
#define FIELDINTEGER	5	// field is binary, signed (normal integer) of some length
#define FIELDREPEAT	6	// this is a 4-byte binary repeat count of following fields
#define FIELDSTRING	7	// field is character string, with a length, which is an exact
				// size, as is NOT NUL terminated. Required for CLICK.
#define FIELDORDERID	8	// orderID/transID.. always 8 bytes long
#define FIELDEXPDATE	9	// CLICK expiry date - year, month, and date bitwise packed into 2 bytes


// ------------------------- typedefs and struct declarations -----------------------------
    // allowed lengths for a field, values 1 .. 8 - each field is 1 if allowed
    // bit [0] only used for 0-size repeats
    struct allowed_lens_x {
	unsigned int bit[8+1];
    };	// allowed_lens_x

    // descriptive definition of a field, within a msg
    struct msg_field {
	unsigned short int
		field_type_i,		// the field type..one of the #defined values
		field_count_i,		// how big the field is..number of bytes if a number - 1/2/4/8
					// or number of characters if a string
		max_repeats_i,		// the number of repeats we can have of the remaining
					// fields. If +ve, the space is always there,
					// if -ve, then don't have to skip any unfilled space
					// NB can't nest a -ve one inside a +ve one.. couldn't set a repeat_size_i
		repeat_size_i,		// how big the repeat part of the sub-structure is, in bytes
		non_repeat_size_i,	// same deal, for the non-repeat fields, which precede the repeat part
		repeat_fields_i,	// # of fields repeated. If there are nested repeats, and outer repeat,
					// should only count an inner repeat as 1 field
		non_repeat_fields_i;	// how many pad fields there are between the repeat count and start of repeat data
	// we can store the field name, if we wish..it is in the defn file...
	// for now, save the storage space
    }; // msg_field

    // definition of a msg
    struct msg_with_fields {
	int msg_no_i,		// the number of the msg
	    num_fields_i;	// how many of the array of fields, which follows, are used
	char msg_name_s [MAXMSGNAME];	// the name of the current message
	// array of structures to take this - allow 1 more, so 0th entry can store msg data
	struct msg_field msg_fields[MAXFIELDSPERMSG + 1];
    }; // msg_with_fields


// ------------------------------- local private vars ---------------------------------

// NB vars declared at this level are implied static, and initialisation values are
//  set before any of the fn's may be called


    // where we store the messages pending logging. NB this is a synchronised structure
    node_header_xt log_pending_q;

    int log_file_open = FALSE;	    // control flag used (close_logger) to control log thread termination

    // termination lock, to allow close_logger to not return until the logger thread
    // has terminated.
    ise_mutex_t loggerTerminateEvent;	    // apparently Solaris doesn't like initialisation to NULL

    FILE *logfile_px = NULL;		    // file for logging raw logs, initially not allocated
    int log_msg_len_i = 0;		    // current length of line being prepared for log
    char log_buffer[MAX_LINE_LEN];	    // line being prepared for log
    char *log_buff_ps;			    // pointer to our fill spot in the buffer

    int suppress_timestamp = 1;     // TRUE   by default, don't put out date/timestamps with each msg
    int suppress_header = 0;	    // FALSE   by default, do dump the internal header with each msg
    int suppress_load_debug = 1;    // TRUE    by default, don't put out debug info at load, like msg sizes
    int suppress_bcast = 0;	    // FALSE   by default, include bcasts in the output log
    int logging_is_on = 0;	    // TRUE    by default, logging is ON
    int suppress_orderid = 0;	    /* FALSE   by default, leave OrderID (special type of UINT64) and transIDs in file
						if TRUE, then turn these into 00000000:00000000. Useful for
						regression comparisons of the output files
				    */

    // structure to store all info about dump'able msgs
    // these are filled in, in the order of the msg def file.
    // the msgs_in_use_i structure indexes into these
    struct msg_with_fields msg_data_x [MAXMSGDEFS];

    /* structure to directly index which ones of msg_data are used
	This is offset by the lowest possible #, and limited to (highest - lowest+1) members
	Say our API msgs range is 1001 to 1700, then we keep 700 slots.
	Then we check for msg code >=1001 && <= 1700...or error
	If the msg code is in range, then look in msgs_in_use [ (msg_code_u - 1001) ].
	This will tell us the slot of the msg_data which contains info about that msg
    */
    int msgs_in_use_i [( MAX_API_MSGNO - MIN_API_MSGNO + 1)];

    // vars set for reading, checking and reporting on the msg definition file
    int defline_i;		// line number we are up to in the definition file
				// this is used by various checking / reporting fns, hence is global

    /* the masks of which lengths are allowed for each type
	e.g, we only support binary numbers with have a byte-length of powers of 2 (1, 2, 4, 8)
	while PADs may only be in the range 1-7
    */
    struct allowed_lens_x
	allowed_char_x,
	allowed_unsigned_x,
	allowed_integer_x,
	allowed_pad_x,
	allowed_repeat_x,
	allowed_orderid_x,
	allowed_expdate_x;


// -------------------------------------- functions -----------------------------------

static void report_status_if_bad (int status_i, char * error_string )
{
    /*	routine to check the value of a status var status_i.
	If the value is FALSE (0), then we printf the error_string
    */
    if (!status_i)
    {
	/* bad / FALSE / 0 status - print the supplied error_string, plus a newline */
	printf_time ();
	printf ( "%s\n", error_string );
    }	// test for TRUE / FALSE status value
}   // report_status_if_bad

// -----------------------------------------------------------------------------------------

void log_exit (int exit_flag)

/* 
    routine called to close things down, tidy up etc, before application exits
    May be called internally, or externally. Closes logfile etc

    If exit param is TRUE (non-zero), then do the exit from here
*/
{
    int call_stat_i;	    // var for getting statii back from called fns

    //flush out all file stream buffers
    call_stat_i = fflush (NULL);

    // close the logfile - if, of course, it was open
    if (logfile_px != NULL)
    {
	call_stat_i = fclose ( logfile_px );

	/* how did the file close go ? NB fclose returns 0 if OK, which is the opposite sense
	    that repotr_status_if_bad uses
	*/
	if (call_stat_i)
	    report_status_if_bad ( FALSE,"bad logfile close");
    }	// test if file was open

    // get here if logfile closed OK - note file is now CLOSED (even if close returned error)
    logfile_px = NULL;

    // did caller want us to exit from here, or does it have more tidying up to do
    if (exit_flag)
	exit (0);
}   // log_exit

// ----------------------------------------------------------------------------

void log_bcast ( uint32 log_fn_code_u,
		    void *bcast_px,
		    uint32 rx_length_i )
/*
Function:   log_bcast
Author:     Neil Cossor
Modified:   9906xx
Description:

    We have received a CLICK broadcast. Log it.

    These are different from ordinary messages, in that they are NOT received
    one at a time, but in a block. This means that we can NOT use the easy
    logging from the normal tx/rx or query, where we know that the standard
    internal header (which has a fn_code, TxnStatus, TxnIdent and OrderIdent
    at the start) precede the actual body of the msg.

Input Params:

    the (internal) fn codes of the msgs for the logger. Ref global.h
    pointer to the broadcast we wish to log
    the size (in bytes) of the broadcast

Output Params

    None
*/
{
    // node where we store the data to be logged.
    gen_buf_node_xt *msg_node;

    // header for dumping bcasts - note that as these are received
    // with multiple individual msgs in the log_reply_x buffer, we cant
    // just do the ordinary logging NOTE this is neat 16 byte - 8 byte aligned
    struct {
	// standard header fields, ready to be logged
	// the logger header
	log_head_xt	bcast_head_x;	
	// standard internal beginning of all CLICK in and out msgs
	// copy of the function code, values as per global.h
	uint32		fn_code;
    } bcast_log_head_x;


    // is the log file open - or do we just discard the msg
    if ((log_file_open) && ( logging_is_on ) && ( suppress_bcast == FALSE))
    {
	// the log file is open, it is worth us doing something

	// allocate the msg node, with the appropriate amount of room for the data
	msg_node = new_gen_buf_node ( sizeof (bcast_log_head_x) + rx_length_i );


	memset ( &bcast_log_head_x, 0, sizeof (bcast_log_head_x));
	// and the overall type of the message we are trying to log
	bcast_log_head_x.bcast_head_x.msg_code_u = CLICK_BCAST;
	// note the fn code of this particular broadcast
	bcast_log_head_x.fn_code = log_fn_code_u;


	// and note the ISEsim msg code - makes it easier for the actual logging code
	msg_node->msg_code_u = bcast_log_head_x.bcast_head_x.msg_code_u;
	// this is combination of size of msg_hdr, and how much data is in the buffer
	msg_node->msg_len_i = sizeof (log_head_xt) + rx_length_i;

	// note the length of the msg we are logging - our response (empty), plus the fixed header stuff
	bcast_log_head_x.bcast_head_x.msg_len_u = rx_length_i + CLICK_B_EXT_HDR_SIZE;

	// put the msg header into the msg_node
	memcpy ( msg_node->msg_p, &bcast_log_head_x, sizeof (bcast_log_head_x));

	// append supplied buffer, into the msg_node, if there is any data in buffer to log
	if (rx_length_i)    // != 0
	    memcpy ( msg_node->msg_p + sizeof (bcast_log_head_x), bcast_px, rx_length_i );

	// and insert into (synchronised) Q, to be logged
	add_to_q_tail ( &log_pending_q, &msg_node->chain );
    }	// test if the log file is open
}   // log_bcast

// ----------------------------------------------------------------------------

void log_text ( char *text_msg_s )
/*
Function:   log_text
Author:     Neil Cossor
Modified:   990908
Description:

    We have had some text we wish to add to the log. Log it.

    These don't have statii, secondary function codes etc.
    Initially, they have been added to allow the calling code to add more info,
    such as when we have had a bad status on a CLICK operation, then
    add the decoded text of the error statii.

Input Params:

    pointer to the text / string we wish to log - NUL terminated

Output Params

    None
*/
{
    // node where we store the data to be logged.
    gen_buf_node_xt *msg_node;

    // header for dumping text- NOTE this is neat 16 byte - 8 byte aligned
    struct {
	// standard header fields, ready to be logged
	// the logger header.. we only use one field of this
	log_head_xt	log_head_x;	
    } text_log_head_x;
    // the size of the string - used several times.. includes room for terminating NUL
    int	    rx_length_i;

    // how long is the text string, including the NUL
    rx_length_i = strlen ( text_msg_s ) + 1;

    // is the log file open - or do we just discard the msg
    if ((log_file_open) && ( logging_is_on ) && (rx_length_i > 1)/* && ( suppress_bcast == FALSE) */)
    {
	// the log file is open, it is worth us doing something

	// allocate the msg node, with the appropriate amount of room for the data
	msg_node = new_gen_buf_node ( sizeof (text_log_head_x) + rx_length_i );

	memset ( &text_log_head_x, 0, sizeof (text_log_head_x));
	// and the overall type of the message we are trying to log
	text_log_head_x.log_head_x.msg_code_u = LOG_TEXT;


	// and note the ISEsim msg code - makes it easier for the actual logging code
	msg_node->msg_code_u = LOG_TEXT;
	// this is combination of size of msg_hdr, and how much data is in the buffer - allow for terminating NUL
	msg_node->msg_len_i = sizeof (log_head_xt) + rx_length_i;

	// put the msg header into the msg_node
	memcpy ( msg_node->msg_p, &text_log_head_x, sizeof (text_log_head_x));

	// append supplied string, into the msg_node
	memcpy ( msg_node->msg_p + sizeof (text_log_head_x), text_msg_s, rx_length_i );

	// and insert into (synchronised) Q, to be logged
	add_to_q_tail ( &log_pending_q, &msg_node->chain );
    }	// test if the log file is open
}   // log_text

// ----------------------------------------------------------------------------

void log_msg ( log_head_xt *msg_hdr, char * buffer_px, int buf_len )
/*
    this fn is responsible for queueing a msg to log.
    Called by any of the threads of ISEsim.

    It must force synchronisation restrictions, as it may be called by different threads,
    and we must NOT corrupt the stream of data being pumped into the log file.

    The parameters are:-
	a msg header, which MUST be supplied
	the address of a buffer to log
	and the size of that buffer. If the size if 0, then do NOT log any of the address

    NOTE
	If the log file is NOT open, then the message is discarded - as the Q which
	is associated with the file is defunct anyway.
*/
{
    // node where we store the data to be logged.
    gen_buf_node_xt *msg_node;
    // fn code from the data - just used for data validity check
    uint32 msg_fn_u;


    // is the log file open, and are we logging - or do we just discard the msg
    if ( (log_file_open) && ( logging_is_on ))
    {
	// log file is open - we can Q the msg for logging

	// point at the internal data component
	msg_fn_u = *(uint32 *)buffer_px;
	// debug check
	if (msg_hdr->msg_code_u == 0)
	{
	    printf_time ();
	    printf ("urk - 0 msg code in log_msg\n" );
	}
	else if ( (msg_fn_u < MIN_API_MSGNO ) || ( msg_fn_u > MAX_API_MSGNO ) )
	{
	    printf_time ();
	    printf ("urk - call to log_msg, with illegal msg code %d\n",
		    msg_fn_u );

	    // escape back to the caller
	    return;
	}

	// allocate the msg node
	msg_node = (gen_buf_node_xt *) malloc (sizeof (gen_buf_node_xt));
	// note how much data we want logged
	// this is combination of size of msg_hdr, and how much data is in the buffer
	msg_node->msg_len_i = sizeof (log_head_xt) + buf_len;

	// allocate the nominated amount of space for the contents of the actual msg
	msg_node->msg_p = (char *) malloc (msg_node->msg_len_i);
	if (msg_node->msg_p == NULL)
	{
	    printf_time ();
	    printf ("urk - no mem allocated by malloc in log_msg\n" );

	    log_exit (TRUE);
	}

	// and note the ISEsim msg code i.e. logon, get_cmd etc - makes it easier for the actual logging code
	msg_node->msg_code_u = msg_hdr->msg_code_u;


	    
	    
	// put the msg header into the msg_node
	memcpy ( msg_node->msg_p, msg_hdr, sizeof (log_head_xt));

	// append supplied buffer, into the msg_node, if there is any data in buffer to log
	if (buf_len)	// != 0
	    memcpy ( msg_node->msg_p + sizeof (log_head_xt), buffer_px, buf_len );

	// and insert into (synchronised) Q, to be logged
	add_to_q_tail ( &log_pending_q, &msg_node->chain );
    }	// test if the log file is open
}   // log_msg

// ----------------------------------------------------------------------

void close_logger ()
/*
Function:   close_logger
Author:	    Neil Cossor
Modified:   20000213
Description:

    close down the logger thread etc, close the file...

    This is a polite close - which tidily completes dumping any messages
    in the Q, and waits for the main thread to signal us
*/
{
    if (log_file_open)
    {
	// close the logger file - do NOT close the process from here - other cleanup may be required
	log_exit ( FALSE );

	// clear file-open flag to close the log thread - it is looking at this
	log_file_open = FALSE;

	// wait for the termination of the logger thread before returning to main
	ISE_MUTEX_LOCK (loggerTerminateEvent);
	// Solaris apparently requires the mutex to be unlocked, for destroy
	ISE_MUTEX_UNLOCK ( loggerTerminateEvent );

	// release the logger termination event handle
	ISE_MUTEX_DESTROY (loggerTerminateEvent);
    }	// close the file, if it was open
}   // close_logger

// ---------------------------------------------------------------------------

static void check_field_width ( struct allowed_lens_x *mask_x,
				int field_len_i )
/*
    routine to check if the field width is an allowed one, as determined by
    the allowed mask, which has a 1 value for each allowed field width

    The var defline_i is expected to be globally accessible, and contain input line #
    and field_len_i " " ", and contain the current specified field length

    The fn is only called while we are parsing the message defn file (startup),
    NOT while processing msgs to log (ongoing runtime).
*/
{   // check_field_width
    if ( ( mask_x->bit [ field_len_i ] ) != 1 )
    {
	// error in msg defn file during parsing - field width not supported by this logger code

	// illegal width, report this
	printf_time ();
	printf ("Line %d : Unsupported field width (%d) supplied for type\n",
		    defline_i, field_len_i );

	// bail out - as politely as we can
	log_exit (TRUE);
    };	// test if legal width
};  // check_field_width

// ---------------------------------------------------------------------------

static void check_string_field_width ( int field_len_i )
/*
    routine to check if the field width for a string type is reasonable
    -ve or 0 is obviously silly, and we know the biggest OM buffer is 64Kb.

    The var defline_i is expected to be globally accessible, and contain input line #
    and field_len_i " " ", and contain the current specified field length

    The fn is only called while we are parsing the message defn file (startup),
    NOT while processing msgs to log (ongoing runtime).
*/
{   // check_string_field_width
    if (( field_len_i < 1 ) || ( field_len_i > 65535 ))
    {
	// error in msg defn file during parsing - field width silly

	// illegal width, report this
	printf_time ();
	printf ("Line %d : Unsupported string field width (%d) supplied\n",
		    defline_i, field_len_i );

	// bail out - as politely as we can
	log_exit (TRUE);
    };	// test if legal width
};  // check_string_field_width

// ---------------------------------------------------------------------------

static int calc_substruct_size ( int data_idx_i,
				    int field_no_i )
/*
    we wish to calculate the size of a repeat substructure of a message.

    This routine can handle
	a number of non-repeated fields ( i.e. fields_to_go_i > repeat_fields_i )
	followed by a number of repeated fields.
	If it hits a nested sub-struct, it recurses, finding the size of the substruct.
    The net result is that it may be called to find the size of a whole msg, or a sub-component,
    and when it finds a nested repeat structure, it recurses to figure out that substruct.

    Params are :-
	the index into our (previously read) msg defn data
	the field # of the structure we are up to in this msg, at start of substruct

    A repeat substructure has
	a max repeat count value
	a number of fields we must dump before we get to the repeating part
	a number of fields which we must repetitively dump
	a (byte) size of all of the plain fields.

    It may be noted, that a message is actual a repeat message, with
	an initial field no 0
	a max and actual repeat count of 1
	a pre-repeat field count of 0

    It may also be noted, than we can handle a fixed repeat (rather than a max, which has
    an actual repeat count embedded in the actual message).

    Upon exit, the repeat_size_i and non_repeat_size_i for this field [ field_no_i ] has been
    updated with the size we calculated.

    The fn returns an int value, which is the sum of those two fields.
*/
{
    // save the initial value of the field_no, so we can look up things like
    // the max_repeat value, even after we have manipulated it
    int init_field_no_i = field_no_i;
    // count down fields to go, with some iterations, till we run out
    int fields_to_go_i = msg_data_x [ data_idx_i].msg_fields [ field_no_i].repeat_fields_i
			    + msg_data_x [ data_idx_i].msg_fields [ field_no_i].non_repeat_fields_i;
    // temp - the size for the particular component we are looking at
    int size_i;

    while ( fields_to_go_i-- > 0 )
    {
	// advance to the next field - in a recurse case, this puts us to the field following the repeat count
	// in a outer call (where the parent is a whole msg), this should have initially been set to 0
	field_no_i++;

	// msg_data_x [ data_idx_i].msg_fields [ field_no_i] describes this field
	switch (msg_data_x [ data_idx_i].msg_fields [ field_no_i].field_type_i)
	{
	    case FIELDUNSIGNED :
	    case FIELDINTEGER :
	    case FIELDCHAR :
	    case FIELDNULSTRING :
	    case FIELDSTRING :
	    case FIELDPAD :
	    case FIELDORDERID:
	    case FIELDEXPDATE:
		size_i = msg_data_x [ data_idx_i].msg_fields [ field_no_i].field_count_i;

		break;	    // end of all the simple field sizes

	    case FIELDREPEAT :
		// we just hit a nested repeat. This is the real guts of pseudo-elegantly handling
		// complex C structures, containing counted arrays of structures

		// and recurse, calculating the component sizes of the substructure
		calc_substruct_size ( 
				    data_idx_i, 	// index into our msg defn data for this fn - unchanged
				    field_no_i		// field to start sub from NB recurse starts 1 field after this val
				    );

		// find the overall size of that substructure, now the component parts are known
		size_i = (  // the repeat part, assuming max repeats - never 0
			    msg_data_x [ data_idx_i ].msg_fields [field_no_i].repeat_size_i
			    * msg_data_x [ data_idx_i].msg_fields [ field_no_i ].max_repeats_i )
			    // the non-repeat part - may be 0
			    + msg_data_x [ data_idx_i ].msg_fields [field_no_i].non_repeat_size_i
			    // don't forget the size of the repeat count itself - may be 0
			    + msg_data_x [ data_idx_i].msg_fields [ field_no_i].field_count_i;

		// advance the "next field" index, to just after this whole repeat substructure defn
		field_no_i += msg_data_x [ data_idx_i ].msg_fields [field_no_i].non_repeat_fields_i + 
					msg_data_x [ data_idx_i ].msg_fields [field_no_i].repeat_fields_i;

		// NB while we skip through the defns to after this substruct, this only counts as 1 field
		// in our fields_to_go_i counter.. so that is decremented as per a single field shortly

		break;	// all done, with handling a nested repeat

	    default :
		printf_time ();
		printf ("Fatal - Field %d of msg %d has undefined type (%d)\n",
			(field_no_i + 1),
			msg_data_x [ data_idx_i ].msg_no_i,
			msg_data_x [ data_idx_i].msg_fields [ field_no_i].field_type_i
			);

		// startup error, bail out - as politely as we can
		log_exit (TRUE);
		break;
	};  // switch on the msg field type

	// now, depending on which field we are up to, whether we add the size of the field
	// we just looked at into the repeat or non-repeat size field
	if ( ( field_no_i - init_field_no_i ) >
		msg_data_x [ data_idx_i ].msg_fields [init_field_no_i].non_repeat_fields_i )
	    // this one is in the repeat part
	    msg_data_x [ data_idx_i ].msg_fields [init_field_no_i].repeat_size_i += size_i;
	else
	    // this one is in the non-repeat part
	    msg_data_x [ data_idx_i ].msg_fields [init_field_no_i].non_repeat_size_i += size_i;
    }	// while ( fields_to_go_i-- > 0 );

    // once we get here, we have worked out the sizes of the repeat and non-repeat part
    // of this structure, assuming a single iteration of the repeat part

    // and return to the next level out.. which may be the end of msg, or an outer recurse
    size_i = msg_data_x [ data_idx_i ].msg_fields [init_field_no_i].repeat_size_i
		+ msg_data_x [ data_idx_i ].msg_fields [init_field_no_i].non_repeat_size_i;

    return size_i;
};  // calc_substruct_size

// ----------------------------------------------------------------------------------------


static void check_end_msg_def (int include_startup,
			       int msg_data_i,
			       int field_no_i)
/*
    routine to check the consistency of # of fields read from def file vs
    # defined in msg header, number of repeat fields etc.
    It also does some finalising of data w.r.t. internal repeats etc.

    Params:-
	include_startup. If TRUE (non-zero) then report into about this msg def
	the actual fn #
	the index into the fn data (which ties back to the original fn
	field_no_i tells us the field # we just parsed - check against the overall
	    msg def info. They must be an exact match.
*/
{
    int size_i; 	// size of the repeat structure we are calculating
    /* we must check the # of fields we read about in the msg, versus the numbers
	defined in the "M" message def, and any "F R " definitions.
	the number in the M, plus the pad_field and repeat_field counts in all of the repeat
	defs, should add up to the number of fields we read definitions for
	NB we run the count of fields up to, and including, all fields we actually
	read.. so we don't have some weird repeat def, which goes past the end of
	the actual valid msg def
    */
    int count_fields_i = msg_data_x [ msg_data_i ].num_fields_i;
    int i;

    for ( i=0; i < field_no_i; i++ )
    {
	// if the field is a repeat, then add in the # of fields it describes
	if ( msg_data_x [ msg_data_i ].msg_fields [ i ].field_type_i == FIELDREPEAT )
	{
	    count_fields_i += ( msg_data_x [ msg_data_i ].msg_fields [ i ].non_repeat_fields_i
				+ msg_data_x [ msg_data_i ].msg_fields [ i ].repeat_fields_i );
	}   // check if we have to add extra fields
    }	// loop through all fields read

    if (count_fields_i != field_no_i )
    {
	printf_time ();
	printf ("Line %d : Incorrect # of fields (%d) supplied for msg %d, should be %d\n",
		    defline_i, 
		    field_no_i,
		    msg_data_x [ msg_data_i ].msg_no_i, 
		    count_fields_i );

	// bail out - invalid msg defn file
	log_exit (TRUE);
    }	// test if we had enough fields defined in the previous message

    // set up the field 0, as a repeat struct, with some specific params
    msg_data_x [ msg_data_i ].msg_fields [ 0 ].field_type_i = FIELDREPEAT;
    // max repeat count of 1
    msg_data_x [ msg_data_i ].msg_fields [ 0 ].max_repeats_i = 1;
    // repeat fields == # of fields
    msg_data_x [ msg_data_i ].msg_fields [ 0 ].repeat_fields_i = msg_data_x [ msg_data_i ].num_fields_i;
    // 0 non-repeat fields before the repeat fields
    msg_data_x [ msg_data_i ].msg_fields [ 0 ].non_repeat_fields_i = 0;
    // NB, the size of the msg has been built up in msg_data_x [ msg_data_i ].msg_fields [ 0 ] as we parsed it

    /* now must back-fill the sizes of repeat structures for any repeat defns
	as the size of an outer repeat must include the size of any inner repeats, we
	must figure out the size of the inner repeats first.
    */
    if ( field_no_i )
	size_i = calc_substruct_size ( msg_data_i, 0 );
 
    // have we just finished a previous msg, or are we at start of file
    if ( size_i != 0)
    {
	// we have a msg length, so NOT at start of file

	// good place to check things from previous msg def

	// report length of previous msg, if we were asked to report at all
	if (include_startup)
	    printf ( "Msg \"%s\", code %d, contained %d bytes\n",
			msg_data_x [ msg_data_i ].msg_name_s, 
			msg_data_x [ msg_data_i ].msg_no_i, 
			size_i );

    };	// test if we are at start of msg def file

};  // void check_end_msg_def

// ----------------------------------------------------------------------------------------

static int read_msg_defs (int include_startup)
{
/*
Function:   read_msg_defs
Author:	    Neil Cossor
Modified:   20000213
Description:

    routine to read the msg defs file, so we know how to format the msgs

Input Params:

    If include_startup is true (non-zero), then report the info as we read.
    The format of the file is documented in the file..but here is a chunk cut from the file
*/
//
// file format is (space separated fields)
//
//	Start line character	Significance
//	--------------------	------------
//	"/"			comment, ignore whole line..so we can read as C comment
//	"M" or "m"		Message defn, 2nd field is message code
//				3rd field is number of fields
//				- which MUST be present before next "M" line
//				optional 4th field is string name of message (no spaces)
//	"F" of "f"		field of a API message,
//				2nd field is I or i for signed number,
//				    or U or u for unsigned number,
//				    or C or c for one-byte character, no \0 trailer
//				    or S or s for \0 terminated string, len includes \0
//				    or P or p for pad, len = 1..3 (for quad alignment)
//				    or R or r for repeat count field, # of repeat fields
//					(implied) always occupies 4 bytes
//				    or O or o for orderID / transID 8-byte number
//				    or D or d for CLICK expiry date (date bit-packed into 2 bytes)
//				3rd field is number of bytes for field if number,
//				    or # chars if string - type S
//				    always 1 if type C
//				    always 1/2/4 if R
//				    always 1/2/4/8 if type U/I
//				    always 1..3 if type P
//				if it is a REPEAT count (type R), then 4th field is a max count (e.g. 400)
//				optional final field is string name of field (no spaces - underscores are allowed)
//
//	e.g.
//	M 2001 2 msg_order_action   Msg, code 2001, 2 fields to follow, name msg_order_action
//	F I 2 trade_type	    2 byte signed numeric field, name trade_type
//	F S 7 ise_code		    6 character string field, \0 terminated, name ise_code
//	M .... next message etc
//

    int call_stat_i;			// status var
    FILE *msg_deffile_px;		// pointer to file for defns of msgs we will have to log
    #define MAXMSGDEFLINE 512		// max length of an individual line of msg def file
    int known_msg_count_i = -1; 	// how many msg_data elements are in use
					// - one less than used, courtesy of C starting indices from 0
    char line_buff_s [MAXMSGDEFLINE];	// max contents of a msg def file line
    char c;				// first char on each line
    
    char field_name_s [MAXFIELDNAME],	// the name of the current msg field
	field_type_s [2];		//  indicates the type of the field - char, signed/unsigned #
					// apparently GNU C on Solaris requires 2, not 1 byte ?
    int i, j;				// local vars for loop initialisation
    int field_no_i;			// the current field we are currently processing


    char filepath[MAX_PATH];	// how much space we allow for getting filepath

    // initialise our msg_data_x structure
    for (i = 0; (i < MAXMSGDEFS); i++ )
    {
	// this entry does not contain a msg, zot the # of the msg
	msg_data_x [ i ].msg_no_i = 0;
	// and contains no fields
	msg_data_x [ i ].num_fields_i = 0;

	// zot the fields structure array
	for ( j = 1; (j<=MAXFIELDSPERMSG); j++ )
	{
	    // undefined field type
	    msg_data_x [ i ].msg_fields [ j ].field_type_i = FIELDUNDEFINED;
	    // with no length
	    msg_data_x [ i ].msg_fields [ j ].field_count_i = 0;
	};	// inner loop, zotting the internal fields structure of one msg def
    };	// for loop, initializing the msg_data_x structure

    // init out forward indexing info - put "unused" value in
    for ( i = 0; ( i <= (MAX_API_MSGNO - MIN_API_MSGNO)); i++ )
	msgs_in_use_i [ i ] = -1;

    // debug - extract the full filename of the msg def file
#ifdef WIN32
	GetCurrentDirectory ( MAX_PATH, filepath );
	printf ("Current file path %s\n", filepath );
#endif 

    // open the msg defn file
    msg_deffile_px = fopen ( MSGDEFFILENAME, msgdeffilemode );

    // how did the file open go ?
    if (msg_deffile_px == NULL)
    {
	printf_time ();
	printf ("Error opening msg def file [%s]\n", MSGDEFFILENAME );
	return FALSE;
    };

    // start on line 1 of the file
    // note that we index the parsed data through the known_message_count_i
    defline_i = 1;	// file definition line #1
    field_no_i = 0;	// how many fields have we seen - none so far

    // read the contents of the file
    do	// until c == EOF, c is first non-blank char found from here
    {
	// skip along looking for non-whitespace, with a EOF bailout clause
	do
	    c = fgetc (msg_deffile_px);
	// NB this && condition will fail the evaluation if EOF, without doing the second check
	while (( c != EOF ) && ( isspace (c)));

	// break out of the innermost loop i.e. the EOF check
	if ( c == EOF )
	    // found EOF before a non-whitespace char - we are done reading this last msg def
	    break;

	// read the rest of the line..unless the line is too long
	fgets (line_buff_s, sizeof (line_buff_s), msg_deffile_px );

	switch (toupper(c))
	{
	    case '/' : // skip till next line - already covered by fgets
//		printf ( "Skipping comment line %d\n", defline_i );
		break;
	    case 'M' : // msg def line, line_buff_s contains # fields, and name
		// one more msg

		// before processing this message defn, check that we got enough
		// fields in the previous message NB checked at EOF too
		if (known_msg_count_i >= 0)
		    check_end_msg_def (include_startup, known_msg_count_i, field_no_i );

		// one more message definition slot used up
		known_msg_count_i++;

		// is this too many msgs for our compile-time limits ?
		if (known_msg_count_i >= MAXMSGDEFS)
		{
		    // our msg table is full, but we have another msg - report the error
		    printf_time ();
		    printf ("Fatal - maximum compile-time # of messages allowed (%d) exceeded on line %d\n",
				MAXMSGDEFS,
				defline_i );
		    // and bail out
		    log_exit (TRUE);
		}   // test if too many msgs defined w.r.t. compile-time #defines

		msg_data_x [ known_msg_count_i ].num_fields_i = 0;	// initially we know of no fields

		// from the string we read from the line, extract the Msg #,
		// # of fields, and the (non-whitespace) name of the msg
		{
		    int match =
			sscanf ( line_buff_s, "%d%d%s",
				    &msg_data_x [ known_msg_count_i ].msg_no_i, 
				    &msg_data_x [ known_msg_count_i ].num_fields_i, 
				    &msg_data_x [ known_msg_count_i ].msg_name_s );
		    // did we read enough fields on this line ?
		    if ( match != 3 )
		    {
			// oops - not enough fields in the M message definition line
			printf_time ();
			printf ( "Line %d : expecting <msg #> <# fields> <msg name>, got %d fields\n",
				    defline_i,
				    match );
			// abort
			log_exit (TRUE);
		    } // test for enough fields in parse of fields for M line
		}   // scan of remaining fields on a M message definition line

		// how many fields we have seen for this msg so far
		field_no_i = 0;

		// update forward index from msg code into our actual msg_data_x structure
		msgs_in_use_i [ (msg_data_x [ known_msg_count_i ].msg_no_i - MIN_API_MSGNO) ]
		    = known_msg_count_i;

		// length of the current msg - add in all the field lengths as we go
		msg_data_x [ known_msg_count_i ].msg_fields [0].repeat_size_i = 0;

		break;
	    case 'F' : // field def line, line_buff_s contains format and name
		// NB we increment field_no_i here b4 the test
		field_no_i++;

		/* is this too many fields (for this msg) for our compile-time limits ?
		    NB the 0th entry is for info about the msg, so the array is defined
		    from 0..MAXFIELDSPERMSG+1, so we test > rather than >=,
		    as there is a msg_fields [ MAXFIELDSPERMSG] entry
		*/
		if (field_no_i > MAXFIELDSPERMSG)
		{
		    // our table of fields for this msg, but we have another field - report the error
		    printf_time ();
		    printf ("Fatal - maximum compile-time # of fields allowed (%d) exceeded on line %d\n",
				MAXFIELDSPERMSG,
				defline_i );
		    // and bail out
		    log_exit (TRUE);
		}   // test if too many fields for a msg defined w.r.t. compile-time #defines

		// from the string we read from the line, extract the field type,
		// # of bytes in the field, and the (non-whitespace) name of the field
		{
		    int matches = 
		    sscanf ( line_buff_s, "%s%d%s", 
				&field_type_s, 
				&msg_data_x [ known_msg_count_i ].msg_fields [field_no_i].field_count_i, 
				&field_name_s );
		    // enough fields on tail of a F field definition line ?
		    if ( matches != 3 )
		    {
			// oops - not enough fields in the M message definition line
			printf_time ();
			printf ( "Line %d : expecting <field type> <field width> <field name>, got %d fields\n",
				    defline_i,
				    matches );
			// abort
			log_exit (TRUE);
		    } // test for enough fields in parse of fields for F line
		}   // scan of remaining fields on a F field definition line

		// expect field_type_s to be u/U/i/I/c/C/s/S/a/A/r/R/p/P
		switch (toupper(field_type_s[0])) {
		case 'U' :
		    // note the type of this field #
		    msg_data_x [ known_msg_count_i ].msg_fields [field_no_i]
			.field_type_i = FIELDUNSIGNED;

		    // check the supplied vs allowed field length
		    check_field_width ( &allowed_unsigned_x, 
					msg_data_x [ known_msg_count_i ].msg_fields [field_no_i].field_count_i );

		    break;  // u/U field_type
		case 'I' :
		    // note the type of this field #
		    msg_data_x [ known_msg_count_i ].msg_fields [field_no_i]
			.field_type_i = FIELDINTEGER;

		    // check the supplied vs allowed field length
		    check_field_width ( &allowed_integer_x, 
					msg_data_x [ known_msg_count_i ].msg_fields [field_no_i].field_count_i );

		    break;  // i/I field_type
		case 'O' :
		    // note the type of this field #
		    msg_data_x [ known_msg_count_i ].msg_fields [field_no_i]
			.field_type_i = FIELDORDERID;

		    // check the supplied vs allowed field length
		    check_field_width ( &allowed_orderid_x, 
					msg_data_x [ known_msg_count_i ].msg_fields [field_no_i].field_count_i );

		    break;  // o/O field_type
		case 'C' :
		    // note the type of this field # - NB this is fixed width - a width of 1 is the only legal value
		    msg_data_x [ known_msg_count_i ].msg_fields [field_no_i]
			.field_type_i = FIELDCHAR;

		    // check the supplied vs allowed field length
		    check_field_width ( &allowed_char_x, 
					msg_data_x [ known_msg_count_i ].msg_fields [field_no_i].field_count_i );

		    break;  // c/C field_type
		case 'D' :
		    // note the type of this field # - NB this is fixed width - a width of 2 is the only legal value
		    msg_data_x [ known_msg_count_i ].msg_fields [field_no_i]
			.field_type_i = FIELDEXPDATE;

		    // check the supplied vs allowed field length
		    check_field_width ( &allowed_expdate_x, 
					msg_data_x [ known_msg_count_i ].msg_fields [field_no_i].field_count_i );

		    break;  // d/D field_type
		case 'S' :
		    // note the type of this field #
		    msg_data_x [ known_msg_count_i ].msg_fields [field_no_i]
			.field_type_i = FIELDNULSTRING;

		    // reasonable string size ?
		    check_string_field_width ( msg_data_x [ known_msg_count_i ].msg_fields [field_no_i].field_count_i );

		    break;  // s/S field_type
		case 'A' :
		    // note the type of this field #
		    msg_data_x [ known_msg_count_i ].msg_fields [field_no_i]
			.field_type_i = FIELDSTRING;

		    // reasonable string size ?
		    check_string_field_width ( msg_data_x [ known_msg_count_i ].msg_fields [field_no_i].field_count_i );

		    break;  // a/A field_type
		case 'P' :
		    // note the type of this field - a pad
		    msg_data_x [ known_msg_count_i ].msg_fields [field_no_i]
			.field_type_i = FIELDPAD;

		    // check the supplied vs allowed field length
		    check_field_width ( &allowed_pad_x, 
					msg_data_x [ known_msg_count_i ].msg_fields [field_no_i].field_count_i );

		    break;  // p/P field_type
		case 'R' :
		    /* there is are extra numeric params on a repeat :-
			the max # of repeats.
			Rescan the fields, including the extra fields.
			We previously read the tail fields as part of the field string name
		    */
		    sscanf ( line_buff_s, "%s%hd%hd%hd%hd%s", 
				field_type_s,		    // no change from last sscanf
				&msg_data_x [ known_msg_count_i ].msg_fields [field_no_i].field_count_i,   // no change "  "	"
				&msg_data_x [ known_msg_count_i ].msg_fields [field_no_i].max_repeats_i,   // new field
				&msg_data_x [ known_msg_count_i ].msg_fields [field_no_i].non_repeat_fields_i,	  // new field
				&msg_data_x [ known_msg_count_i ].msg_fields [field_no_i].repeat_fields_i,   // new field
				&field_name_s );

		    // need to calculate repeat_size_i for each repeat struct, in check_end_msg_def

		    // note the type of this field - a repeat counter
		    msg_data_x [ known_msg_count_i ].msg_fields [field_no_i]
			.field_type_i = FIELDREPEAT;

		    // check the supplied vs allowed field length
		    check_field_width ( &allowed_repeat_x, 
					msg_data_x [ known_msg_count_i ].msg_fields [field_no_i].field_count_i );

		    break;  // r/R field_type
		default :
		    printf_time ();
		    printf ( "Illegal field type [%s] on line %d of file %s\n",
				field_type_s, defline_i, MSGDEFFILENAME );

		    // startup error, bail out - as politely as we can
		    log_exit (TRUE);

		    break;  // illegal field_type
		};  // switch on field_type_s

		break;
	    default :	// error, line did NOT start with a legal character
		printf_time ();
		printf ( "Illegal char <%2.2xX> on line %d of file %s\n", c, defline_i, MSGDEFFILENAME );

		// startup error, bail out - as politely as we can
		log_exit (TRUE);

		break;	// redundant
	}   //switch on the first char on the next line

	defline_i++;	// next line - initially was 1

    }
    while ( c != EOF );

    // wrap up the last msg
    check_end_msg_def (include_startup, known_msg_count_i, field_no_i );

    // and close the definition file
    call_stat_i = fclose ( msg_deffile_px );

    return (call_stat_i == 0);
}   // read_msg_defs

// -----------------------------------------------------------------------------------------

static void dump_log_buffer ()
    /*
    routine to dump out the contents of the existing log buffer, and zot the length
    back to zero, so we can start filling it up again

    If, of course, there is anything in the log_buffer - indicated by non-zero log_msg_len_i
    */
{   // dump_log_buffer
    if (log_msg_len_i != 0)
    {
	fprintf ( logfile_px, "%.*s\n", log_msg_len_i, log_buffer );

	// empty buffer now
	log_msg_len_i = 0;
    };	// was there anything in the buffer
};  // dump_log_buffer

// -----------------------------------------------------------------------------------------

static void ensure_log_buffer_room (int bytes_needed_i)
/*
    ensure that there is enough room in the log buffer for the required # of bytes.
    If there is not enough room, then dump this buffer, and start with the buffer
    containing just a TAB "\t" character..i.e. we are on a continuation line
*/
{   // ensure_log_buffer_room
    if ( (log_msg_len_i + bytes_needed_i ) >= sizeof ( log_buffer))
    {
	dump_log_buffer ();	// log_msg_len_i = 0 upon return
	log_msg_len_i = sprintf ( log_buff_ps + log_msg_len_i, "\t" );

	// no need to flush - there is a background thread for that
    };	// is there enough room in the buffer
};  // ensure_log_buffer_room

// -----------------------------------------------------------------------------------------

static int check_log_buffer_room (int bytes_needed_i)
/*
    check if there is enough room in the log buffer for the required # of bytes.
    Returns TRUE (1) if so, or FALSE (0) if not.
*/
{   // check_log_buffer_room
    if ( (log_msg_len_i + bytes_needed_i ) >= sizeof ( log_buffer))
	// not enough room
	return FALSE;
    else
	// enough room
	return TRUE;
};  // check_log_buffer_room

// -----------------------------------------------------------------------------------------

static int dump_timestamp ()

/*
Routine:    dump_timestamp
Author:	    Neil Cossor
Modified:   20000204
Description:

    routine to dump out the current date/timestamp.
    Will do nothing if the global flag supress_timestamp has been set.
    This flag is set up in logger_init.

    If we are on a WinNT system we will dump out the timestamps with milliseconds
    If we are on a Solaris system we will skip the milliseconds (don't know the Solaris-specific calls)

Input parameters
	none
*/
{   // dump_timestamp
#ifdef WIN32
    // NT NT NT NT NT NT NT NT NT NT NT NT NT NT NT NT NT NT 
    #define	MAXDATETIMESTR 21   // yyyymmdd:hh:mm:ss.fff

    static Timeb_t localtime_x;

    // pointer to current date/time, internal format
    static Timeb_t *currtime_px;

    // NT NT NT NT NT NT NT NT NT NT NT NT NT NT NT NT NT NT 
#else
    // Solaris Solaris Solaris Solaris Solaris Solaris Solaris 
    #define	MAXDATETIMESTR 17   // yyyymmdd:hh:mm:ss

    static time_t localtime_x;	    // where we store the current date/time, internal format

    // pointer to current date/time, internal format
    static time_t *currtime_px;

    // Solaris Solaris Solaris Solaris Solaris Solaris Solaris 
#endif

    /* temp for storing string version of current date/time,
	with trailing space for the \0 character */
    char    datetime_s[MAXDATETIMESTR+1];
    // current time, but in a struct
    struct tm *currtime_x;




    // are we suppressing date/timestamps
    if (!suppress_timestamp)
    {	 
	// point to local storage of current (internal format) time
	currtime_px = &localtime_x;

#ifdef WIN32
	// get the current date/time
	ftime ( &localtime_x );
    
	// convert into structure form
	currtime_x = localtime ( &localtime_x.time );

	// and convert into string form YYYYMMDD:HH:MM:SS
	strftime (datetime_s, sizeof (datetime_s), "%Y%m%d:%H:%M:%S",
				currtime_x);

	// add in the trailing ".fff" milliseconds info
	sprintf ( (char *)((int)&datetime_s + strlen (datetime_s)), ".%03d",
		    localtime_x.millitm );

#else
	// get the current date/time
	time ( &localtime_x );
    
	// convert into structure form
	currtime_x = localtime ( &localtime_x );

	// and convert into string form YYYYMMDD:HH:MM:SS
	strftime (datetime_s, sizeof (datetime_s), "%Y%m%d:%H:%M:%S",
				currtime_x);

#endif

	/* write out the date/timestamp (trailing space)
	    NB this is always at start of line, so we KNOW this will fit in the line buffer
		so don't need to call ensure_log_buffer_room
	*/
	log_msg_len_i += sprintf ( log_buff_ps + log_msg_len_i, "%s ", datetime_s );

	if (log_msg_len_i <= 0)
	{
	    /* report status failure NB specifically call report_status_if_bad with a failure status */
	    report_status_if_bad ( FALSE, "Error doing sprintf with timestamp data" );
	};
    }	// test if we are suppressing timestamp
    
    return log_msg_len_i;
};  // dump_timestamp

// -----------------------------------------------------------------------------------------

static int dump_msg_ise_header (log_head_xt *socket_msg_px)

/*
routine to dump out the ISEsim internal header of a msg.
If the global flag suppress_header has been set, then do nothing.
This global flags was set in the initial call to logger_init

Input parameters
    pointer to a log_head_xt header, which starts the msg
    always..
	the header (3x4byte fields)
*/
{   // dump_msg_ise_header
    // have we been asked to suppress ISEsim internal headers ?
    if (!suppress_header)
    {	 
	// write out the date/time, and the header of this msg
	//	    include len, msg_code_u (internal), status, channel # (always 0 for now)
	//	    NB this is always at start of line, so we KNOW this will fit in the line buffer
	//		so don't need to call ensure_log_buffer_room
	log_msg_len_i += sprintf ( log_buff_ps + log_msg_len_i, " L=%2d M=%d S=%d",
				    socket_msg_px->msg_len_u,
				    socket_msg_px->msg_code_u,
				    socket_msg_px->msg_status_i );

	if (log_msg_len_i <= 0)
	{
	    /* report status failure NB specifically call report_status_if_bad with a failure status */
	    report_status_if_bad ( FALSE, "Error doing sprintf with header data" );
	};
    }	// test if we are suppressing internal ISEsim headers
    else
    {
	// write out the status
	log_msg_len_i += sprintf ( log_buff_ps + log_msg_len_i, " %d",
				    socket_msg_px->msg_status_i );
    }

    return log_msg_len_i;
};  // dump_msg_ise_header

// ----------------------------------------------------------------------------------

static void dump_block_hex ( unsigned char *data_px,
				    int bytes_to_dump )
/*
    local fn to dump a block of data, in HEX form. This routine will probably only
    be called when some major error has been found while trying to dump out some
    structure. That means that the data is wacko (and hence won't dump easily)
    OR (more likely) the message defn file is wrong. That could mean the definition
    for the message is wrong, or is missing.

    By calling this fn, there will still be a log of what we got, albeit messy,
    but the logger can probably continue correctly with the next message.

    Params are
	pointer to first byte of string to dump
	# of bytes to dump (exactly, no NUL-termination shortcuts)

    It puts out the data in a <xx[..]X> format, i.e. as a series of hex digits, wrapped
    inside a <> pair, with a 'X' character just before the end '>'.

    It puts as many bytes as it can on each line, and advances to the next output line
    as many times as are necessary.

    As it just hex dumps, it can dump any type of message.
*/

{   // dump_block_hex

    int     have_done_initial = FALSE;

    // in simple form, we could just put as many lines as are required, with no spacing.
    // while this is easy to code, it is hard to read.
    // perhaps 4 bytes (8 hex chars), then a space ?

    /* loop, dumping out the chars of the string, until we run out of characters.
	Test at start of loop, to allow that the bytes_to_dump may have a 0 value when this fn is called */
    while (bytes_to_dump != 0)
    {
	// we haven't run out of data yet

	// do we need to put out the leading chars
	if (!have_done_initial)
	{
	    /* we have not yet put out our leading chars, for this line.
		BUT we won't bother putting out our wrap characters, unless
		there is enough room for our leading '<', trailing 'X' and '>',
		and at least enough room for one byte of data (2 hex digits)
		This will cause a newline, if there is insufficient room on this line.
	    */
	    ensure_log_buffer_room (5);

	    // put out our leading character - this may but onto our newline+tab
	    log_msg_len_i += sprintf (log_buff_ps + log_msg_len_i,
					"<" );

	    // note that we have put out the initial leadin (leading space, and '<')
	    have_done_initial = TRUE;
	}
	else
	{
	    /* we are NOT doing the first hex dump on this output line.
		If we haven't got room for another, then we spit out our trailer,
		and wrap to the next line.

		We need enough room for 2 hex digits, and a trailing X>
	    */
	    if (check_log_buffer_room (4))
	    {
		// room for another hex character. Spit it out, and go around and try for another
		log_msg_len_i += sprintf ( log_buff_ps + log_msg_len_i,
					    "%2.2x", *data_px);

		data_px += 1;		// advance local pointer to input data by one byte
		bytes_to_dump -= 1;	// one less input char to go
	    }
	    else
	    {
		/* no room for another hex char. Spit out the trailer, and go around
		    and have another go at this character.
		    NOTE that the previous pass, we made sure there was enough for a
		    initial character with trailer OR enough for another charater with trailer,
		    so we know there is enough room for the trailer, either way.
		*/
		log_msg_len_i += sprintf ( log_buff_ps + log_msg_len_i,
					    "X>");
		/*
		    if we set the have_done_initial back, the next attempt will check / force
		    enough room for a <xxX>. We know there won't be room,
		    so this will force out a newline, and initial tab for the next line.
		*/
		have_done_initial = FALSE;
	    }	// have we room to add another hex digit to this line (we have already done some)
	}   // have we spat out any hex digits on this line
    }	// while (bytes_to_dump != 0)

    /* while it is likely that the calling routine who was trying to dump (and found a problem)
	calls dump_log_buffer at the end anyway, but calling it twice doesn't hurt anyway,
	and guarantees that we have a tidy EOL.
    */
    dump_log_buffer ();
};  // dump_block_hex

// ----------------------------------------------------------------------------------

static void dump_string_printable ( unsigned char *data_px,
				    int bytes_to_dump )
/*
    local fn to dump a string, in printable form.

    Params are
	pointer to first byte of string to dump
	# of bytes to dump (exactly, no NUL-termination shortcuts)

    As the length of the string to dump (which is done one char at a time) is a parameter,
    it may be called with null-terminated C-style strings, OR fixed-length CLICK-style strings.
    If C-style, then the length param is probably from a strlen() on the data.
    If CLICK-style, then the length param will be the known fixed length.
    
    It does NOT spit out any leading space characters, or any enclosing "" characters :-
    the calling fn should do that.

    It DOES convert non-printable characters into their <XX> hex format,
    and handles whether the original / "hexed" character will fit on the current line.

    On each output (actual, or "hexed") it checks if there is room to fit the output,
    and advances to the next output line if it won't fit.

    This fn / design has some flaws, in that any enclosing characters (added by calling fns)
    may not be tidily formatted w.r.t. EOLs.. but, it will handle most cases pretty elegantly.

*/

{   // dump_string_printable

    // no leaders added by this fn - wrappers in the output stream must be added by
    // the caller

    /* loop, dumping out the chars of the string, until we run out of characters.
	Test at start of loop, to allow that the bytes_to_dump may have a 0 value when this fn is called */
    while (bytes_to_dump != 0)
    {
	// we haven't run out of string yet

	// is the next char printable ? This includes NULs.
	if ((*data_px < 32) || (*data_px > 126))
	{
	    // not printable - convert to <XX>, two hex digits with <> chars
	    // required space just went up
	    ensure_log_buffer_room (4);
	    log_msg_len_i += sprintf ( log_buff_ps + log_msg_len_i,
					"<%2.2x>", *data_px);
	}
	else
	{
	    // printable - is there room for one byte in the output line ?
	    ensure_log_buffer_room (1);
	    log_msg_len_i += sprintf ( log_buff_ps + log_msg_len_i,
					"%.1s", (data_px));
	};  // is the next char printable ?

	data_px += 1;		// advance local pointer to input data by one byte
	bytes_to_dump -= 1;	// one less input char to go
    }	// while (bytes_to_dump != 0)

};  // dump_string_printable


// ---------------------------------------------------------------

static int dump_hdr_only (log_head_xt *socket_msg_px)
/*
    routine to dump an ISEsim header - no data. Used to dump things
    like MC_RESP_GET or MC_RESP_QUIT msgs.
*/
{
    // dump timestamp (fn will skip timestamp if global suppress flag set)
    dump_timestamp ();
    
    // dump internal header (fn will skip dump if global suppress flag set)
    dump_msg_ise_header (socket_msg_px);

    // we have done with this msg, force flush of dump buffer
    dump_log_buffer ();

    return 1;
}   // dump_hdr_only

// ----------------------------------------------------------------------------

static void dump_uint16 (uint16 * uint16_pu)
// dump out a uint16 into the log
{
    // was it a special ?
    switch (*uint16_pu)
    {
	case UNDEFINED_UINT16 :
	    ensure_log_buffer_room ( LEN_SPECIAL_STR );
	    log_msg_len_i += sprintf ( log_buff_ps + log_msg_len_i," \\0" );
	    break;
	case REMOVE_UINT16 :
	    ensure_log_buffer_room ( LEN_SPECIAL_STR );
	    log_msg_len_i += sprintf ( log_buff_ps + log_msg_len_i," \\1" );
	    break;
	default :
	// add to buffer - is there room - allow 1 for leading space
	ensure_log_buffer_room ( MAX_UINT16_STR + 1 );
	log_msg_len_i += sprintf ( log_buff_ps + log_msg_len_i,
				    " %u", *uint16_pu );
    }

//  printf ( " %u", *uint16_pu );
}   // dump_uint16

// ----------------------------------------------------------------------------

static void dump_uint8 (uint8 * uint8_pu)
// dump out a uint8 into the log
{
    // was it a special ?
    switch (*uint8_pu)
    {
	case UNDEFINED_UINT8 :
	    ensure_log_buffer_room ( LEN_SPECIAL_STR );
	    log_msg_len_i += sprintf ( log_buff_ps + log_msg_len_i," \\0" );
	    break;
	case REMOVE_UINT8 :
	    ensure_log_buffer_room ( LEN_SPECIAL_STR );
	    log_msg_len_i += sprintf ( log_buff_ps + log_msg_len_i," \\1" );
	    break;
	default :
	// add to buffer - is there room - allow 1 for leading space
	ensure_log_buffer_room ( MAX_UINT8_STR + 1 );
	log_msg_len_i += sprintf ( log_buff_ps + log_msg_len_i,
				    " %u", *uint8_pu );
    }
//			printf ( " %u", *uint8_pu );
}   // dump_uint8

// ----------------------------------------------------------------------------

static void dump_expiry_date (uint16 * uint16_pu)
/*
Function:   dump_expiry_date
Author:	    Neil Cossor
Modified:   991019
Description:

    dump out a uint16 CLICK expiry date into the log

    This field is bitwise packed.
    It may be all zero, OR should contain a valid date, which start
    from 1989, as the year is an offset since 1989.

    There is a routine in utility.c which breaks the structure down into
    the appropriate bits, so use that.
*/
{
    uint16  yyyy_u;
    uint8   mm_u,
	    dd_u;

    unpack_click_bit_date ( *uint16_pu,
			    &yyyy_u,
			    &mm_u,
			    &dd_u );

    ensure_log_buffer_room ( LEN_EXPDATE_STR );
    log_msg_len_i += sprintf ( log_buff_ps + log_msg_len_i," %04u-%02u-%02u",
				yyyy_u,
				mm_u,
				dd_u
				);

}   // dump_expiry_date

// ----------------------------------------------------------------------------

static void dump_uint32 (uint32 * uint32_pu)
// dump out a uint32 into the log
{
    // was it a special ?
    switch (*uint32_pu)
    {
	case UNDEFINED_UINT32 :
	    ensure_log_buffer_room ( LEN_SPECIAL_STR );
	    log_msg_len_i += sprintf ( log_buff_ps + log_msg_len_i," \\0" );
	    break;
	case REMOVE_UINT32 :
	    ensure_log_buffer_room ( LEN_SPECIAL_STR );
	    log_msg_len_i += sprintf ( log_buff_ps + log_msg_len_i," \\1" );
	    break;
	default :
	// add to buffer - is there room - allow 1 for leading space
	ensure_log_buffer_room ( MAX_UINT32_STR + 1 );
	log_msg_len_i += sprintf ( log_buff_ps + log_msg_len_i,
				    " %u", *uint32_pu );
    }

//  printf ( " %u", *uint32_pu );
}   // dump_uint32

// ----------------------------------------------------------------------------

//uint64s have been effectively removed from 17.1 In most cases (orderID) they have become quad_words
//I am attempting to change this function to hanlde a quad_word struct

static void dump_uint64 (quad_word * qw, int suppress_orderid )
/*
    dump out a uint64 into the log
    If it is an orderID/transID (supplied flag set to TRUE),
    then it is to be spat out as 8hex:8hex,
    otherwise it is spat out as a very big number..

    Note that the suppress_orderid flag may be set, in which case
    orderIDs/transIDs are spat out as xxxxxxxx:xxxxxxxx,
    which makes for simpler regression comparison of logger output.
*/
{
    // for now, the only "fixed" 8-byte format we have, is a CLICK
    // order-id..and they don't tell us the byte order. Pick one,
    // and fix this to match the internal structure, once we have
    // tested this with CLICK

  //  uint64_xt *uint64_pxu;  // pointer to uint64 as a structure

    // set up pointer to it as a structure, so we can dump it out as 2 x 4-byte nums, in hex
    //(uint64 *)uint64_pxu = uint64_pu;

    // add to buffer - is there room - allow 1 for leading space
    //if	(is_orderID)
    //{
	// it is an orderID/transID
	quad_conv_t * convOrderNo;
	if (suppress_orderid)
	{
	    // yup, suppressing them

	    // add dummy buffer - is there room - allow 1 for leading space
	    ensure_log_buffer_room ( MAX_UINT64_HEX_STR + 1 );
	    // xxxxxxxx:xxxxxxxx - same format as real value
	    log_msg_len_i += sprintf ( log_buff_ps + log_msg_len_i,
					" xxxxxxxx:xxxxxxxx" );
	}
	else
	{
	    // was it a special ?
	    /* I dont really know whats going on here, but REMOVE_UINT64 and UNDEFINED_UINT64 is not used anywhere but here
			so I am commenting this oddness out
		switch (*uint64_pu)
	    {
		case UNDEFINED_UINT64 :
		    ensure_log_buffer_room ( LEN_SPECIAL_STR );
		    log_msg_len_i += sprintf ( log_buff_ps + log_msg_len_i," \\0" );
		    break;
		case REMOVE_UINT64 :
		    ensure_log_buffer_room ( LEN_SPECIAL_STR );
		    log_msg_len_i += sprintf ( log_buff_ps + log_msg_len_i," \\1" );
		    break;
		
		default :
		*/
		    // add to buffer - is there room - allow 1 for leading space
		    ensure_log_buffer_room ( MAX_UINT64_HEX_STR + 1 );
		    // high longword first, separating blank, leading zeroes,
		    // 8 chars fixed, separating ":", low longword..same format
			convOrderNo = (quad_conv_t *) &(qw->quad_word);				
		    log_msg_len_i += sprintf ( log_buff_ps + log_msg_len_i,
						" %08lX:%08lX", 	
						convOrderNo->l2,
						convOrderNo->l1						
						);
	    //}	// switch on 'specials' packed into uint64 orderID
	}   // see if we are suppressing orderIDs/transIDs
    //}
    /*else
    {
	// just an ordinary 8-byte unsigned number.. spit it out
	// add to buffer - is there room - allow 1 for leading space
	ensure_log_buffer_room ( MAX_UINT64_STR + 1 );
	// high longword first, separating blank, leading zeroes,
	// 8 chars fixed, separating ":", low longword..same format
	log_msg_len_i += sprintf ( log_buff_ps + log_msg_len_i,
				    " %I64u", *uint64_pu );
    }	// test if ordinrary number, or order/transID
	*/
}   // dump_uint64


// ----------------------------------------------------------------------------

static void dump_int8 (int8 * int8_pi)
// dump out a int8 into the log
{
    // was it a special ?
    switch (*int8_pi)
    {
	case UNDEFINED_INT8 :
	    ensure_log_buffer_room ( LEN_SPECIAL_STR );
	    log_msg_len_i += sprintf ( log_buff_ps + log_msg_len_i," \\0" );
	    break;
	case REMOVE_INT8 :
	    ensure_log_buffer_room ( LEN_SPECIAL_STR );
	    log_msg_len_i += sprintf ( log_buff_ps + log_msg_len_i," \\1" );
	    break;
	default :
	// add to buffer - is there room - allow 1 for leading space
	ensure_log_buffer_room ( MAX_INT8_STR + 1 );
	log_msg_len_i += sprintf ( log_buff_ps + log_msg_len_i,
				    " %d", *int8_pi );
    }

//  printf ( " %d", *int8_pi );
}   // dump_int8

// ----------------------------------------------------------------------------

static void dump_int16 (int16 * int16_pi)
// dump out a int16 into the log
{
    // was it a special ?
    switch (*int16_pi)
    {
	case UNDEFINED_INT16 :
	    ensure_log_buffer_room ( LEN_SPECIAL_STR );
	    log_msg_len_i += sprintf ( log_buff_ps + log_msg_len_i," \\0" );
	    break;
	case REMOVE_INT16 :
	    ensure_log_buffer_room ( LEN_SPECIAL_STR );
	    log_msg_len_i += sprintf ( log_buff_ps + log_msg_len_i," \\1" );
	    break;
	default :
	// add to buffer - is there room - allow 1 for leading space
	ensure_log_buffer_room ( MAX_INT16_STR + 1 );
	log_msg_len_i += sprintf ( log_buff_ps + log_msg_len_i,
				    " %d", *int16_pi );
    }

//  printf ( " %d", *int16_pi );
}   // dump_int16

// ----------------------------------------------------------------------------

static void dump_int32 (int32 * int32_pi)
// dump out a int32 into the log
{
    // was it a special ?
    switch (*int32_pi)
    {
	case UNDEFINED_INT32 :
	    ensure_log_buffer_room ( LEN_SPECIAL_STR );
	    log_msg_len_i += sprintf ( log_buff_ps + log_msg_len_i," \\0" );
	    break;
	case REMOVE_INT32 :
	    ensure_log_buffer_room ( LEN_SPECIAL_STR );
	    log_msg_len_i += sprintf ( log_buff_ps + log_msg_len_i," \\1" );
	    break;
	default :
	// add to buffer - is there room - allow 1 for leading space
	ensure_log_buffer_room ( MAX_INT32_STR + 1 );
	log_msg_len_i += sprintf ( log_buff_ps + log_msg_len_i,
				    " %d", *int32_pi );
    }

//  printf ( " %d", *int32_pi );
}   // dump_int32

// ----------------------------------------------------------------------------

static void dump_int64 (int64 * int64_pi)
// dump out a int64 into the log

// hmm.. Bennett just sprang 8-byte signed on me..
// for now, ignore solaris..use Windows native int64 : NAC 971127
{
    // was it a special ?
	//UNDEFINED_INT64 and REMOVE_INT64 dont seem to be used anywhere in this project so i am removing this stuff -eric
    /*if(*int64_pi == UNDEFINED_INT64 ) {
	    ensure_log_buffer_room ( LEN_SPECIAL_STR );
	    log_msg_len_i += sprintf ( log_buff_ps + log_msg_len_i," \\0" );
	} else if(*int64_pi == REMOVE_INT64) {
	    ensure_log_buffer_room ( LEN_SPECIAL_STR );
	    log_msg_len_i += sprintf ( log_buff_ps + log_msg_len_i," \\1" );
	}
	else {*/
		// add to buffer - is there room - allow 1 for leading space
		ensure_log_buffer_room ( MAX_INT64_STR + 1 );
		log_msg_len_i += sprintf ( log_buff_ps + log_msg_len_i,
						" %I64d", *int64_pi );
    //}

//  printf ( log_buff_ps + log_msg_len_i, " %ld", *int64_pi );
}   // dump_int64

// ----------------------------------------------------------------------------

static void dump_s_char (char * data_px)
// dump out a single character field - ? no weird CLICK ISO-Latin1 conversions

// need to modify to handle the char being a non-printable character..or the text log file
// will be unreadable
{
    // add to buffer - is there room - allow leading space and "" chars
    ensure_log_buffer_room ( 4 );
    log_msg_len_i += sprintf ( log_buff_ps + log_msg_len_i, 
				" \"%.1s\"", (data_px));
//  printf ( " \"%.1s\"", (data_px));
}   // dump_s_char

// ----------------------------------------------------------------------------

static void dump_null_string (char * data_px)
/*
    dump out a null-terminated string
*/
{
    // add to buffer - is there room - allow leading space and "" chars

    // was it a special ?
    switch (*data_px)
    {
	case NUL :  // put out a " \0"
	    ensure_log_buffer_room ( LEN_SPECIAL_STR );
	    log_msg_len_i += sprintf ( log_buff_ps + log_msg_len_i," \\0" );
	    break;
/*
	case SOH :  // put out a " \1"
	    ensure_log_buffer_room ( LEN_SPECIAL_STR );
	    log_msg_len_i += sprintf ( log_buff_ps + log_msg_len_i," \\1" );
	    break;
*/
	default :   // put out the string, with a leading space, and surrounded by "" characters
	    // now we have dealt with "specials", spit out leading wrapper, i.e. leading space + "
	    ensure_log_buffer_room (2);
	    log_msg_len_i += sprintf ( log_buff_ps + log_msg_len_i, 
					    " \"");
	    /* and dump out the NUL terminated string, with line-wrap, and hex conversions for non-printable
		as we know it is NUL terminated, get the length from standard C routine.
	    */
	    dump_string_printable ( data_px, strlen (data_px) );

	    // spit out trailing wrapper, i.e. trailing ". NB it is possible this is on the next line
	    ensure_log_buffer_room (1);
	    log_msg_len_i += sprintf ( log_buff_ps + log_msg_len_i, 
					    "\"");
    }	// switch on first char of string - was it a "special" ?
}   // dump_null_string

// ----------------------------------------------------------------------------

static void dump_click_string (char * data_px, int width)
/*
    dump out a CLICK string. These are NOT null-terminated (well.. they might be, but not guaranteed)
    and may require weird ISO-Latin1 conversions. Unders a Windoze environment, I don't
    see that character set conversion is required - NAC 9906xx.

    Need to handle the string containing non-printable characters..or the text log file
    will be unreadable.
*/
{
    // is the width sensible ? This indicates formatting (in msg def or msg packing) or coding error
    if ( width < 0 )
	return;

    // spit out leading wrapper, i.e. leading space"
    ensure_log_buffer_room (2);
    log_msg_len_i += sprintf ( log_buff_ps + log_msg_len_i, 
				    " \"");
    /* and dump out the fixed length string, with line-wrap, and hex conversions for non-printable
	NOTE that if there are NULs etc in the string, they will be spat out in HEX,
	as will the following characters, as the called routine spits out ALL characters
    */
    dump_string_printable ( data_px, width );

    // spit out trailing wrapper, i.e. trailing "
    ensure_log_buffer_room (1);
    log_msg_len_i += sprintf ( log_buff_ps + log_msg_len_i, 
				    "\"");
}   // dump_click_string

// ------------------------------------------------------------------------------------

static int dump_substruct_msg ( char ** data_px, 
				uint32 msg_fn_u,
				int data_idx_i,
				int field_no_i,
				int actual_repeat_count_i,
				int * bytes_left_in_msg_i )
/*
Function    dump_substruct_msg
Author	    Neil Cossor
Modified    20000204
Description

    we wish to dump a repeat substructure of a message.
    This routine can handle
	a number of non-repeated fields ( i.e. fields_to_go_i > repeat_fields_i )
	followed by a number of repeated fields.
	If it hits a nested sub-struct, it dumps out the count value,
	    and recurses, dumping the substruct. Dumping a substruct counts as 1 field
	    w.r.t. this substruct.
    The net result is, it is may be called to dump a whole msg, or a sub-component,
    and when it finds a nested repeat structure, it recurses to dump out that substruct.

    Params are :-
	pointer into var, with address of next byte of the raw data
	the fn we are dumping (for reporting)
	the index into our (previously read) msg defn data
	the field # of the structure we are up to in this msg, at start of substruct
	the number of repeats we should do (actual). This # is also important in skipping extra space.
	pointer to var, which indicates the remaining bytes in the msg. This should be adjusted
	    b4 recursing to substructures. Note that the actual var is adjusted, so that there
	    is exactly one variable indicating the correct figure. The var is required so that
	    a sensible number can be passed into dump_block_hex, if something disastrous
	    goes wrong.


    Implicit params :-
	the size of the various data fields for this msg
	the # of non-repeat fields before the repeat part	}
	the # of repeat fields, after the non-repeat part	}   all in msg_data_x [ data_idx_i].msg_fields []
	the max number of repeats.				}   for the starting value of field_no_i

    A repeat substructure has
	an actual repeat count value
	a max repeat count value
	a number of fields we must dump before we get to the repeating part
	a number of fields which we must repetitively dump
	a (byte) size of those repetitive fields (which we have previously calc'ed from the field defs)

    It may be noted, that a message is actual a repeat message, with
	an initial field no 0
	a max and actual repeat count of 1
	a pre-repeat field count of 0
	the fields to dump set from the # of fields in the actual msg
	and, w.r.t. dumping, the implicit actual repeat count of 1 is considered done b4 we hit this routine

    The fn returns a success / failure status - TRUE (1) == OK, FALSE (0) == error.
    If a failure occurred (e.g. illegal repeat count), then rest of the msg has
    been dumped in hex, and no further dump for this msg should be attempted,
    i.e. we should just recurse all the way out to the outer msg layer.
*/
{
    // how many repeat iterations we should do. It varies in the loop,
    // but we need the initial value at a later point, so make a copy
    int repeat_count_i = actual_repeat_count_i;
    // some fields we need to call ourselves recursively when we hit a nested repeat
    int sub_repeat_count_i;	// the actual instance count in the msg for the nested substruct
    // save the initial value of the field_no, so we can look up things like
    // the max_repeat value, even after we have manipulated it
    int init_field_no_i = field_no_i;
    /* save the original end-of-message address, so we can correctly adjust the bytes_left_in_msg_i
	before recursing, calling dump_block_hex (on errors), or returning to outer

	initial end address is the start address + # bytes in msg
    */
    unsigned end_data_px = (unsigned)*data_px + *bytes_left_in_msg_i;

    /* count down fields to go, with some iterations, till we run out
	Note that there may be no non-repeat fields, and the actual count may be 0 -> no fields,
	just skip over the allocated structure space */
    int fields_to_go_i = msg_data_x [ data_idx_i].msg_fields [ field_no_i].non_repeat_fields_i;
    if ( actual_repeat_count_i > 0 )
	fields_to_go_i += msg_data_x [ data_idx_i].msg_fields [ field_no_i].repeat_fields_i;

    while ( fields_to_go_i > 0 )
    {
	// advance to the next field - in a recurse case, this puts us to the field following the repeat count
	// in a outer call (where the parent is a whole msg), this should have initially been set to 0
	field_no_i++;

	// msg_data_x [ data_idx_i].msg_fields [ field_no_i] describes this field
	switch (msg_data_x [ data_idx_i].msg_fields [ field_no_i].field_type_i)
	{
	    case FIELDORDERID :
		switch (msg_data_x [ data_idx_i].msg_fields [ field_no_i].field_count_i)
		{
		    case 8:
			dump_uint64 ( (quad_word *)*data_px, TRUE );   // orderID/transID

			break;	// 8-byte unsigned
		    default :
			printf_time ();
			printf ( "Illegal numeric field len %d, for orderID field %d\n",
				msg_data_x [ data_idx_i].msg_fields [ field_no_i]
				    .field_count_i,
				field_no_i + 1 );
			/* Msg COULD be corrupted, but the most probable cause is that
			    we have the wrong msg defn. Try to dump the rest of the msg out (in hex),
			    and advance to the next msg. This means ISEsim stays up.

			    First, adjust the count of bytes remaining, to exclude those we have
			    done in this recurse of this fn
			*/
			*bytes_left_in_msg_i = (end_data_px - (unsigned)*data_px);
			// dump in hex
			dump_block_hex ( *data_px, *bytes_left_in_msg_i );
			// return & report error, so outer does NOT continue to attempt dumping of this msg
			return FALSE;
		};  // switch on numeric length of orderID field

		*data_px += msg_data_x [ data_idx_i].msg_fields [ field_no_i]
				    .field_count_i;

		break;	// end of FIELDORDERID case
	    case FIELDUNSIGNED :
		switch (msg_data_x [ data_idx_i].msg_fields [ field_no_i].field_count_i)
		{
		    case 1 :
			dump_uint8 ( (uint8 *)*data_px);

			break;	// 1-byte unsigned
		    case 2 :
			dump_uint16 ( (uint16 *)*data_px);

			break;	// 2-byte unsigned
		    case 4:
			dump_uint32 ( (uint32 *)*data_px );

			break;	// 4-byte unsigned
		    case 8:
			dump_uint64 ( (quad_word *)*data_px, FALSE );  // not orderID/transID

			break;	// 8-byte unsigned
		    default :
			printf_time ();
			printf ( "Illegal numeric field len %d, for unsigned field %d\n",
				msg_data_x [ data_idx_i].msg_fields [ field_no_i]
				    .field_count_i,
				field_no_i + 1 );
			/* Msg COULD be corrupted, but the most probable cause is that
			    we have the wrong msg defn. Try to dump the rest of the msg out (in hex),
			    and advance to the next msg. This means ISEsim stays up.

			    First, adjust the count of bytes remaining, to exclude those we have
			    done in this recurse of this fn
			*/
			*bytes_left_in_msg_i = (end_data_px - (unsigned)*data_px);
			// dump in hex
			dump_block_hex ( *data_px, *bytes_left_in_msg_i );
			// return & report error, so outer does NOT continue to attempt dumping of this msg
			return FALSE;
		};  // switch on numeric length of unsigned field

		*data_px += msg_data_x [ data_idx_i].msg_fields [ field_no_i]
				    .field_count_i;

		break;	// end of FIELDUNSIGNED case
	    case FIELDINTEGER :
		switch (msg_data_x [ data_idx_i].msg_fields [ field_no_i].field_count_i)
		{
		    case 1 :
			dump_int8 ( (int8 *)*data_px);

			break;	// 1-byte integer
		    case 2 :
			dump_int16 ( (int16 *)*data_px);

			break;	// 2-byte integer
		    case 4:
			dump_int32 ( (int32 *)*data_px);

			break;	// 4-byte integer
		    case 8:
			dump_int64 ( (int64 *)*data_px);

			break;	// 8-byte integer
		    default :
			printf_time ();
			printf ( "Illegal numeric field len %d, for signed field %d\n",
				msg_data_x [ data_idx_i].msg_fields [ field_no_i]
				    .field_count_i,
				field_no_i + 1 );
			/* Msg COULD be corrupted, but the most probable cause is that
			    we have the wrong msg defn. Try to dump the rest of the msg out (in hex),
			    and advance to the next msg. This means ISEsim stays up.

			    First, adjust the count of bytes remaining, to exclude those we have
			    done in this recurse of this fn
			*/
			*bytes_left_in_msg_i = (end_data_px - (unsigned)*data_px);
			// dump in hex
			dump_block_hex ( *data_px, *bytes_left_in_msg_i );
			// return & report error, so outer does NOT continue to attempt dumping of this msg
			return FALSE;
		};  // switch on numeric length of (signed) integer field

		*data_px += msg_data_x [ data_idx_i].msg_fields [ field_no_i]
				    .field_count_i;

		break;	    // end of FIELDINTEGER case

	    case FIELDEXPDATE :
		switch (msg_data_x [ data_idx_i].msg_fields [ field_no_i].field_count_i)
		{
		    case 2:
			dump_expiry_date ( (uint16 *)*data_px );   // expiry date

			break;	// 8-byte unsigned
		    default :
			printf_time ();
			printf ( "Illegal numeric field len %d, for expiry date %d\n",
				msg_data_x [ data_idx_i].msg_fields [ field_no_i]
				    .field_count_i,
				field_no_i + 1 );
			/* Msg COULD be corrupted, but the most probable cause is that
			    we have the wrong msg defn. Try to dump the rest of the msg out (in hex),
			    and advance to the next msg. This means ISEsim stays up.

			    First, adjust the count of bytes remaining, to exclude those we have
			    done in this recurse of this fn
			*/
			*bytes_left_in_msg_i = (end_data_px - (unsigned)*data_px);
			// dump in hex
			dump_block_hex ( *data_px, *bytes_left_in_msg_i );
			// return & report error, so outer does NOT continue to attempt dumping of this msg
			return FALSE;
		};  // switch on numeric length of expiry date field

		*data_px += msg_data_x [ data_idx_i].msg_fields [ field_no_i]
				    .field_count_i;

		break;	// end of FIELDEXPDATE case

	    case FIELDCHAR :
		// 1 byte char
		    dump_s_char ( *data_px );

		    *data_px += 1;

		break;	    // end of FIELDCHAR case
	    case FIELDNULSTRING :
		// N bytes string - always null terminated. The type ISEsim uses internally,
		// and is the native C way of storing strings.
		dump_null_string ( *data_px );

		*data_px += msg_data_x [ data_idx_i].msg_fields [ field_no_i]
				    .field_count_i;


		break;	    // end of FIELDNULSTRING
	    case FIELDSTRING :
		// N bytes string - fixed length string, NOT null terminated. A CLICK string
		/* NOTE while we have a nominated string length.. it is possible we are going
		    to run out of message first.. say where we know that there is a string
		    in the msg def, but the length is not defined - like an OM BN1 network broadcast
		*/
		*bytes_left_in_msg_i = (end_data_px - (unsigned)*data_px);
		dump_click_string ( *data_px, min ( msg_data_x [ data_idx_i].msg_fields [ field_no_i]
							.field_count_i,
							*bytes_left_in_msg_i )
				    );

		*data_px += msg_data_x [ data_idx_i].msg_fields [ field_no_i]
				    .field_count_i;


		break;	    // end of FIELDSTRING
	    case FIELDPAD :
		// skip N bytes - so do nothing here
		/* ? should we instead spit out the pad characters, noted as such.
		    That way, if we had the format defn wrong, the raw information is still there
		    in the log, and we can still see what really was there ?
		*/
		*data_px += msg_data_x [ data_idx_i].msg_fields [ field_no_i]
				    .field_count_i;

		break;	    // end of FIELDPAD

	    case FIELDREPEAT :
		// we just hit a nested repeat. This is the real guts of pseudo-elegantly handling
		// complex C structures, containing counted arrays of structures

		// dump the value of this repeat field
		switch ( msg_data_x [ data_idx_i].msg_fields [ field_no_i].field_count_i )
		{
		case 0:
		    // special case - fixed # of repeats
		    sub_repeat_count_i = msg_data_x [ data_idx_i].msg_fields [ field_no_i].max_repeats_i;
		    
		    // don't have to advance the data_px - this is a "invisible field"
		    
		    // and don't dump a repeat count - there is no corresponding field
		    
		    break;	// 0 length repeat field - i.e. fixed # of repeats
		case 1:
		    // single byte repeat count - commmonly followed by a 1 byte filler
		    sub_repeat_count_i = **(uint8 **)data_px;	    // read the 1 byte
		    // report the repeat count - just the low 8 bits
		    dump_uint8 ( (uint8*)&sub_repeat_count_i );

		    *data_px += 1;
		    break;
		case 2:
		    // two byte repeat count
		    sub_repeat_count_i = **(uint16 **)data_px;	  // read the 2 bytes
		    // report the repeat count - just the low 16 bits
		    dump_uint16 ( (uint16*)&sub_repeat_count_i );

		    *data_px += 2;
		    break;
		case 4:
		    // 4 byte repeat count
		    sub_repeat_count_i = **(uint32 **)data_px;	  // read the 4 bytes
		    // report the repeat count - whole 32 bits
		    dump_uint32 ( &sub_repeat_count_i );

		    *data_px += 4;
		    break;
		default:
		    printf_time ();
		    printf ("Illegal repeat size %d in %d msg\n",
				msg_data_x [ data_idx_i].msg_fields [ field_no_i].field_count_i,
				msg_fn_u );
		    /* Msg COULD be corrupted, but the most probable cause is that
			we have the wrong msg defn. Try to dump the rest of the msg out (in hex),
			and advance to the next msg. This means ISEsim stays up.

			First, adjust the count of bytes remaining, to exclude those we have
			done in this recurse of this fn
		    */
		    *bytes_left_in_msg_i = (end_data_px - (unsigned)*data_px);
		    // dump in hex
		    dump_block_hex ( *data_px, *bytes_left_in_msg_i );
		    // return & report error, so outer does NOT continue to attempt dumping of this msg
		    return FALSE;
		};  // switch, on size of repeat count

		// is the repeat value reasonable
		if ((sub_repeat_count_i < 0) ||
			(sub_repeat_count_i > msg_data_x [ data_idx_i].msg_fields [ field_no_i].max_repeats_i))
		{
		    /* illegal repeat count - do what, John ? Now we are in trouble.. either our field
			definition (in the ise_msg_defs.dat file) is wrong OR whoever filled in
			the message broke the rules
		    */
		    printf_time ();
		    printf ( "Illegal repeat count %d in msg, fn %d, max allowed %d\n",
				sub_repeat_count_i,
				msg_fn_u,
				msg_data_x [ data_idx_i].msg_fields [ field_no_i].max_repeats_i );

		    /* Msg COULD be corrupted, but the most probable cause is that
			we have the wrong msg defn. Try to dump the rest of the msg out (in hex),
			and advance to the next msg. This means ISEsim stays up.

			First, adjust the count of bytes remaining, to exclude those we have
			done in this recurse of this fn
		    */
		    *bytes_left_in_msg_i = (end_data_px - (unsigned)*data_px);
		    // dump in hex
		    dump_block_hex ( *data_px, *bytes_left_in_msg_i );
		    // return & report error, so outer does NOT continue to attempt dumping of this msg
		    return FALSE;
		}   // test for reasonable repeat count in the actual msg

		/* and recurse, dumping the substructure,
		    using the params from the msg and the msg defn
		    NOTE that if dumping the substruct returns an error (FALSE value),
		    then the remainder of the msg has been dumped in hex, and we should
		    NOT continue to dump this msg - just return that error condition to this
		    recurse's parent so we can wind out
		*/
		if (dump_substruct_msg ( data_px,		// where the dump data is - continue from where we are up to
					msg_fn_u,		// msg fn - it hasn't changed
					data_idx_i, 	// index into our msg defn data for this fn - also unchanged
					field_no_i, 	// field to start sub from NB recurse starts 1 field after this val
					sub_repeat_count_i, // number of repeat iterations just got this out of the msg
					bytes_left_in_msg_i // current (non-dumped) part of the total msg
					))
		{
		    // all went well with the substruct, continue
		    // NB data_px is updated to the end of the inner substruct as part of the call to dump_substruct_msg

		    // advance the "next field" index, to just after this whole repeat structure defn
		    field_no_i += msg_data_x [ data_idx_i ].msg_fields [field_no_i].non_repeat_fields_i + 
					    msg_data_x [ data_idx_i ].msg_fields [field_no_i].repeat_fields_i;

		    // NB while we skip through the defns to after this substruct, this only counts as 1 field
		    // in our fields_to_go_i counter.. so that is decremented as per a single field shortly
		}
		else
		    // problem with substruct, report problem to our parent
		    return FALSE;

		break;	// all done, with handling a nested repeat

	    default :
		printf_time ();
		printf ("Field %d has undefined type\n", (field_no_i + 1));

		/* Msg COULD be corrupted, but the most probable cause is that
		    we have the wrong msg defn. Try to dump the rest of the msg out (in hex),
		    and advance to the next msg. This means ISEsim stays up.

		    First, adjust the count of bytes remaining, to exclude those we have
		    done in this recurse of this fn
		*/
		*bytes_left_in_msg_i = (end_data_px - (unsigned)*data_px);
		// dump in hex
		dump_block_hex ( *data_px, *bytes_left_in_msg_i );
		// return & report error, so outer does NOT continue to attempt dumping of this msg
		return FALSE;
	};  // switch on the msg field type

	// one less field to do, working through this sub-struct
	fields_to_go_i--;

	// have we run out of fields in this sub-struct ?
	if (fields_to_go_i == 0)
	{
	    // yup, at end of sub-struct. Have we reached the end of our repeat count ?
	    repeat_count_i--;
	    // NB, if repeat_count==0, then we will drop out of our do_while
	    if (repeat_count_i > 0)
	    {
		// we have finished one repeat iteration, but there are more to go
		// set the "current" dump field back
		field_no_i = init_field_no_i +
			    msg_data_x [ data_idx_i].msg_fields [ init_field_no_i].non_repeat_fields_i;
		// and the number of fields to go up by the same amount field_no_i went down
		fields_to_go_i += msg_data_x [ data_idx_i].msg_fields [ init_field_no_i].repeat_fields_i;
	    }	// test if we have done all of our iterations of this sub-struct
	}   // test if re ran out of fields in this iteration of the sub-struct
    }	// while ( fields_to_go_i > 0 )

    // once we get here, we have finished the actual repeat iterations.

    // If the actual count is not the same as the max allowed, then there is waste space in the struct we were given
    // which we now skip over. NB if actual == max, then this does nothing
    (int) *data_px += (( msg_data_x [ data_idx_i].msg_fields [ init_field_no_i ].max_repeats_i -
			    actual_repeat_count_i ) * 
			    msg_data_x [ data_idx_i ].msg_fields [init_field_no_i].repeat_size_i );
    /* and return to the next level out.. which may be the end of msg, or an outer recurse
	If we got here, this substruct dumped OK, so return good status

	b4 we exit, adjust the count of bytes remaining, to exclude those we have
	done in this (successful) dump-recurse of this fn
    */
    *bytes_left_in_msg_i = (end_data_px - (unsigned)*data_px);
    return TRUE;

};  // dump_substruct_msg

// -----------------------------------------------------------------------------------

static void dump_struct_msg ( char * data_px, uint32 msg_fn_u,
				int length_i )

/*
    common core used to dump a C structure msg.
    The msg_fn_u points us to the format data, which must have already been read by
    logger_init. In the case of CLICK to/from msgs, then the fn may be one purely
    used by ISEsim internals.

    Input parameters
	pointer to the actual structure data. The structure it points to is
	    dependent on the fn code.
	the function code of the msg, e.g. MC_G_ACKNOWLEGDEMENT
	the length (in bytes) of the structured msg we are dumping

    This module is purely internal to mc_log.c, and is called to
    dump the ISEsim structures that we pass to/from ISEsim clients, and to
    dump the CLICK structures that we pass into omniapi calls.

*/
{
    int data_idx_i;	// index of actual msg definition data - after validation, passed to lower fn

    // is the msg_code_u one within the allowable range ?
    if ((msg_fn_u < MIN_API_MSGNO)
	|| (msg_fn_u > MAX_API_MSGNO))
    {
	printf_time ();
	printf ( "Msg code %d is out of known range, %d to %d\n",
		    msg_fn_u,
		    MIN_API_MSGNO,
		    MAX_API_MSGNO );

	/* we probably have the wrong msg defn file. Try to dump the rest of the
	    msg out (in hex), and advance to the next msg. This means ISEsim stays up.
	*/
	dump_block_hex ( data_px, length_i );

	// bail out - we have dumped the msg (in hex)
	return;
    };	// test for msg_code_u in reasonable range 

    // do we have msg definition data for this type of msg
    // figure out which msg_data_x this msg_code_u points us to
    data_idx_i = msgs_in_use_i [(msg_fn_u - MIN_API_MSGNO)];
    // and is that one we know about .. value of -1 if not define
    if ( data_idx_i < 0)
    {
	// illegel - we DO NOT know about this msg code, although in correct range
	printf_time ();
	printf ( "Msg code %d is in legal range, but no format data defined\n",
		    msg_fn_u );

	/* we probably have the wrong msg defn file. Try to dump the rest of the
	    msg out (in hex), and advance to the next msg. This means ISEsim stays up.
	*/
	dump_block_hex ( data_px, length_i );

	// bail out - we have dumped the msg (in hex)
	return;
    };	// test if that msg code is one that was defined in our file

    // OK, we have a good msg definition record.
    // dump this msg, as if it was a substructure

    // and recurse, dumping the substructure, using the params from the msg and the msg defn
    dump_substruct_msg ( &data_px,		// start at beginning of the msg
			msg_fn_u,		// the msg fn, used for reporting errors in lower fn
			data_idx_i,		// the index into our msg defn data for this fn, which we just found
			0,			// field to start at NB fn starts 1 field after this repeat count i.e. 1
			1,			// actual repeat count - i.e. no repeats, just a single pass
			&length_i );		// total length of msg

}   // dump_struct_msg


// ------------------------------------------------------------------------------------

static void dump_click_in_block (log_head_xt *socket_msg_px,
				    unsigned char *data_px,
				    int length_i )
/*
    routine to dump out to the log file a CLICK response i.e. in from CLICK
*/
{
    // the fn code, in the msg being logged - need this to dump the structure
    uint32 msg_fn_u;

    // dump timestamp (fn will skip timestamp if global suppress flag set)
    dump_timestamp ();
    
    // write out the fn_code - 32bit unsigned
    msg_fn_u = *(uint32 *)data_px;
    dump_uint32 ( (uint32 *) data_px);
    data_px += sizeof (uint32);

    // if supplied, dump internal header (fn will skip dump if global suppress flag set)
    // but ALWAYS spits out status from header
    dump_msg_ise_header (socket_msg_px);

    // write out the 2ndary status - 32bit signed
    dump_int32 ( (int32 *) data_px);
    data_px += sizeof (int32);

    // write out the transID - 64bit unsigned
    dump_uint64 ( (quad_word *) data_px, TRUE );	// is transID/orderID
    data_px += sizeof (uint64);

    // write out the OrderID - 64bit unsigned
    dump_uint64 ( (quad_word *) data_px, TRUE );	// is transID/orderID
    data_px += sizeof (uint64);


    // now need to dump varying structure, fn_code based, description in ise_msg_defs.dat
    // data_px points to start of CLICK C structure
    dump_struct_msg ( data_px, msg_fn_u, length_i );

    // we have done with this msg, force flush of dump buffer
    dump_log_buffer ();
}   // dump_click_in_block

// ----------------------------------------------------------------------------------------

static void dump_click_out_block (log_head_xt *socket_msg_px,
				    unsigned char *data_px,
				    int length_i )
/*
    routine to dump out to the log file a CLICK query / command i.e. sent to CLICK
*/
{
    // the fn code, in the msg being logged - need this to dump the structure
    uint32 msg_fn_u;

    // dump timestamp (fn will skip timestamp if global suppress flag set)
    dump_timestamp ();
    
    // write out the fn_code - 32bit unsigned
    msg_fn_u = *(uint32 *)data_px;
    dump_uint32 ( (uint32 *) data_px);
    data_px += sizeof (uint32);

    // if supplied, dump internal header (fn will skip dump if global suppress flag set)
    // but ALWAYS spits out status from header
    dump_msg_ise_header (socket_msg_px);

    // write out the facility code - 32bit unsigned
    dump_uint32 ( (uint32 *) data_px);
    data_px += sizeof (uint32);

    /* dump out the standard CLICK header. This consists of
	a 32bit unsigned, CLICK structure length
	a single character central module specifier e.g. 'B'
	a single character server type fields e.g. 'O'
	a 16bit unsigned transaction type # e.g. 1
    */
    // write out the CLICK structure length - 32bit unsigned
    dump_uint32 ( (uint32 *) data_px);
    data_px += sizeof (uint32);

    // write out the single character central module specifier
    dump_s_char ( data_px);
    data_px++;

    // write out the single character server type specifier
    dump_s_char ( data_px);
    data_px++;

    // write out the 16bit unsigned transaction type
    dump_uint16 ( (uint16 *) data_px);
    data_px += sizeof (uint16);

    // now need to dump varying structure, fn_code based, description in ise_msg_defs.dat
    // data_px points to start of CLICK C structure
    dump_struct_msg ( data_px, msg_fn_u, length_i );



    // we have done with this msg, force flush of dump buffer
    dump_log_buffer ();
}   // dump_click_out_block

// ------------------------------------------------------------------------------------

static void dump_click_bcast_block (log_head_xt *socket_msg_px,
				    unsigned char *data_px,
				    int length_i )
/*
    routine to dump out to the log file a CLICK bcast in from CLICK
*/
{
    // the fn code, in the msg being logged - need this to dump the structure
    uint32 msg_fn_u;

    // dump timestamp (fn will skip timestamp if global suppress flag set)
    dump_timestamp ();
    
    // write out the fn_code - 32bit unsigned
    msg_fn_u = *(uint32 *)data_px;
    dump_uint32 ( (uint32 *) data_px);
    data_px += sizeof (uint32);

    // if supplied, dump internal header (fn will skip dump if global suppress flag set)
    // but ALWAYS spits out status from header
    dump_msg_ise_header (socket_msg_px);

    // spit out the standard CLICK start of message
    // write out the single character central module specifier
    dump_s_char ( data_px);
    data_px++;

    // write out the single character server type specifier
    dump_s_char ( data_px);
    data_px++;

    // write out the 16bit unsigned transaction type
    dump_uint16 ( (uint16 *) data_px);
    data_px += sizeof (uint16);


    // now need to dump varying structure, fn_code based, description in ise_msg_defs.dat
    // data_px points to start of CLICK C structure
    dump_struct_msg ( data_px, msg_fn_u, length_i );

    // we have done with this msg, force flush of dump buffer
    dump_log_buffer ();
}   // dump_click_bcast_block

// ------------------------------------------------------------------------------------

static void dump_text_block ( unsigned char *data_px )
/*
    routine to dump out to the log file a text msg

    the input param is the pointer to the NUL terminated string that we wish to log
*/
{
    // dump timestamp (fn will skip timestamp if global suppress flag set)
    dump_timestamp ();

    // spit out the text (with line wraps etc added as necessary)
    dump_string_printable ( data_px,
			    strlen ( data_px ));

    // we have done with this msg, force flush of dump buffer
    dump_log_buffer ();
}   // dump_text_block

// -------------------------------------------------------------------------------------------

static int logger_init (char * log_filename_ps,
			int no_timestamp,
			int no_header,
			int incl_startup)
/*
Function:   logger_init
Author:     Neil Cossor
Modified:   20000204
Description:

    fn which initialises all of the low level logging routines.
    This includes reading the msg definition file (so we know how
    to dump out ISEsim-defined data msgs), and opening the output log file.
    
    It returns a TRUE (non-zero) if all OK,
    and a FALSE (zero) otherwise.
    
    It presumes that it may report things to stdout, with printf's.

    Flush to output is done externally, by using fflush (NULL) i.e. flush
    all file buffers.
    
Input Params are:-
    the name of the logfile, where we will dump the output
	If strlen()==0, then the default name LOGFILE.TXT is used.
    whether to suppress date/timestamps with each msg
    whether to suppress ISEsim internal headers with each msg
    whether to include the startup diagnostic messages
*/
{
    char * local_log_filename_ps;   // pointer to default or supplied log filename

    // we assume that we can write to stdout - it has been reassigned as necessary
    // before this initialisation routine was called

    // set up the masks of the allowed lengths for each field type, before we read the msg defs
    // we support unsigned lens of 1, 2, 4, 8
    allowed_unsigned_x.bit[1]=1;
    allowed_unsigned_x.bit[2]=1;
    allowed_unsigned_x.bit[4]=1;
    allowed_unsigned_x.bit[8]=1;
    // integer lens of 1, 2, 4, 8
    allowed_integer_x.bit[1]=1;
    allowed_integer_x.bit[2]=1;
    allowed_integer_x.bit[4]=1;
    allowed_integer_x.bit[8]=1;
    // pad lens of 1..8
    allowed_pad_x.bit[1]=1;
    allowed_pad_x.bit[2]=1;
    allowed_pad_x.bit[3]=1;
    allowed_pad_x.bit[4]=1;
    allowed_pad_x.bit[5]=1;
    allowed_pad_x.bit[6]=1;
    allowed_pad_x.bit[7]=1;
    allowed_pad_x.bit[8]=1;
    // repeat lens of 0, 1, 2, 4
    allowed_repeat_x.bit[0]=1;
    allowed_repeat_x.bit[1]=1;
    allowed_repeat_x.bit[2]=1;
    allowed_repeat_x.bit[4]=1;
    // char lens of 1
    allowed_char_x.bit[1]=1;
    // only allow length of 8 for orderID / transID
    allowed_orderid_x.bit[8]=1;
    // only allowed length of 2 for expiry date
    allowed_expdate_x.bit[2]=1;

    // assign pointer to where we build up our output log data
    log_buff_ps = (char *)&log_buffer;	// (char *) to avoid compiler complaining char * != char *[132]

    // read the msg definition file, so we can format the output
    if (!read_msg_defs (incl_startup))
    {
	printf_time ();
	printf ("Error reading ISEsim msg def file\n");
	return FALSE;
    };

    // did the caller supply a potentially reasonable filename ?
    if ((!log_filename_ps)
	|| (strlen (log_filename_ps) == 0))
    {
	// not supplied with a filename, use default
	local_log_filename_ps = LOGFILENAME;
    }
    else
	// user supplied log_filename_ps, use it
	local_log_filename_ps = log_filename_ps;

    // open the logfile
    logfile_px = fopen ( local_log_filename_ps, logfilemode );

    // how did the file open go ?
    if (logfile_px == NULL)
    {
	printf_time ();
	printf ("Error opening log output file %s\n", local_log_filename_ps);
	return FALSE;
    };
    
    // set dump flags for lower routines, based on our supplied parameters
    suppress_timestamp = no_timestamp;
    suppress_header = no_header;

    // all good if we get to here
    return TRUE;
}   // logger_init


// ----------------------------------------------------------------------

static void record_log_buffer ( gen_buf_node_xt *msg_node )

/*
Function:   record_log_buffer
Author:     Neil Cossor
Modified:   9908xx
Description:

    Routine to save a complete message buffer into the log file.
    Called by looping routine of logger thread, once for each msg
    inserted into the log Q by log_msg, log_bcast and log_text calls
*/
{
    //pointer to the header of the msg, as a structured internal header
    log_head_xt *int_msg_px;
    // pointer to the data component of the msg, after the log_head_xt header
    char * data_px;

    // point to data, as a structured header - cuts down on repetitive casts through the switch
    int_msg_px = (log_head_xt *)msg_node->msg_p;
    // data following the fixed header
    data_px = msg_node->msg_p + sizeof (log_head_xt);

    // process it
    switch (msg_node->msg_code_u)
    {
    case CLICK_TX :
    case CLICK_QUERY :
	    // these both have a CLICK facility, then a standard CLICK request header,
	    // then a varying structure containing the actual buffer of the query
	    dump_click_out_block ( int_msg_px, data_px,
				    (msg_node->msg_len_i - (sizeof (log_head_xt))) );
	break;
    case CLICK_IN :
	    // CLICK_IN has a secondary CLICK status, with following transID and orderID,
	    // then a varying structure containing the actual buffer of the response
	    dump_click_in_block ( int_msg_px, data_px,
				    (msg_node->msg_len_i - (sizeof (log_head_xt))) );
	break;
    case CLICK_BCAST :
	    // CLICK_BCAST then has a varying structure containing the actual buffer of the response
	    dump_click_bcast_block ( int_msg_px, data_px,
				    (msg_node->msg_len_i - (sizeof (log_head_xt))) );
	break;
    case LOG_TEXT :
	    // text message.. NB there is nothing interesting in the header, other than
	    // it is a text message, which we have already figured out
	    dump_text_block ( data_px );
	break;
    default :	// default msg_code_u
// debug ?? not really, but need better error handling here
	    printf_time ();
	    printf ( "Logging illegal msg code %d\n", msg_node->msg_code_u );
	break;
    };	// switch on msg_code_u

}   // record_log_buffer

// ----------------------------------------------------------------------

static void dump_log_msgs (void *dummy)
/*
Function:   dump_log_msgs
Author:     Neil Cossor
Modified:   200000113
Description:

    Main fn of the logger thread. This fn is started at application startup,
    and continuously whizzes around seeing if there is anything in the Q
    to be dumped.

    The fn is static i.e. may NOT be called by any external module.
*/
{
    // msg we grabbed off the Q, which must be written to the log file
    gen_buf_node_xt *msg_node;

    // hold the termination lock, until the file has been closed, and we drop thru
    // to the unlock at bottom, which will let the close_logger get control again
    ISE_MUTEX_LOCK ( loggerTerminateEvent );

    // spin until the close_logger fn is called
    while (log_file_open)
    {
	if ( empty_q ( &log_pending_q ))
	{
	    // Q is currently empty - flush the log file (for observers)
	    fflush (logfile_px);
	    // and have a little sleep, so we don't hog all of the CPU
	    Sleep (100);    // in milliseconds
	}
	else
	{
	    // Q is not empty, grab (synchronised) the front msg from the Q
	    msg_node = (gen_buf_node_xt *)remove_front_from_q ( &log_pending_q );

	    // log the msg
	    record_log_buffer ( msg_node );

	    // release the message, and its component parts
	    free_gen_buf_node ( msg_node );
	}   // else case of test if log Q is empty
    }	// while (log_file_open) loop

    // release any buffers that are in the Q, and any resources for the Q header itself
    free_gen_buf_node_q ( &log_pending_q , TRUE);

    // signal the termination event, so the waiting close_logger can return to the caller
    ISE_MUTEX_UNLOCK ( loggerTerminateEvent );

    // force nice cleanup of thread resources (and related library resources)
//    _endthread ();
}   // dump_log_msgs

// ----------------------------------------------------------------------

int init_logger(char *logfile_name_ps)
/*
Function:   init_logger
Author:	    Neil Cossor
Modified:   20000612
Description:

    initialise the logger thread etc.

    We assume that the default directory has been set already, hence we may open the logfile etc.

    Returns TRUE (non-zero) if everything OK, FALSE (0) otherwise.
    Parent process MUST terminate if NOT OK.
*/
{
    unsigned long status_i;

    // initialise logging routines
    status_i = logger_init ( logfile_name_ps,	// pass the name supplied - NULL if none supplied
				FALSE,		// do we suppress timestamps
				TRUE,		// do we suppress internal headers
				FALSE );	// do we include startup diagnostics

    // set bad return status if log routines did NOT init OK - parent process should terminate
    if (!status_i)
	return FALSE;

    // init the Q of pending log msgs
    // this Q is insert into by log_msg, and checked by our read thread
    init_q ( &log_pending_q );

    // note that the file is OPEN
    log_file_open = TRUE;

    // create the logger termination event
    ISE_MUTEX_INIT ( loggerTerminateEvent );
    // Solaris mutex create cannot fail, but check Win32 -  thanks Joshua
#ifdef WIN32
    if (!loggerTerminateEvent)
    {
	printf_time ();
	printf ("Fatal error creating Logger termination event object\n");
	exit (0);
    }
#endif

    // start the thread which looks at the Q of msgs to be logged
    status_i = ISE_THREAD_CREATE ( dump_log_msgs,	// main routine of created thread
				    0 ); 		// 32 bit value passed to thread - nothing
    if (status_i < 0)
    {
	printf_time ();
	printf ("Fatal error creating Logger thread, status %u\n", status_i );
	exit (0);
    }
    else
	// give the logger thread a moments peace to get itself together
	Sleep (100);	// milliseconds


    // If we got this far, everything looks good
//  printf_time ();
//  printf("Successful Logger startup\n");
    return TRUE;
}   // init_logger

// ---------------------- external control functions --------------------

void logging_on (void)
/*
    turn logging on
*/
{
    // turn logging on
    logging_is_on = TRUE;
}   // logging_on

// -----------------------------------------------------------------------

void logging_off (void)
/*
    turn logging off
*/
{
    // turn logging off
    logging_is_on = FALSE;
}   // logging_off

// -----------------------------------------------------------------------

void log_timestamps_on (void)
/*
    turn timestamps on for logging
*/
{
    suppress_timestamp = FALSE;
}   // log_timestamps_on

// -----------------------------------------------------------------------

void log_timestamps_off (void)
/*
    turn timestamps off for logging
*/
{
    suppress_timestamp = TRUE;
}   // log_timestamps_off

// -----------------------------------------------------------------------

void log_bcasts_on (void)
/*
    turn bcasts on for logging.
    Finer granularity (like particular broadcasts) may come later...
*/
{
    suppress_bcast = FALSE;
}   // log_bcasts_on

// -----------------------------------------------------------------------

void log_bcasts_off (void)
/*
    turn bcasts off for logging
*/
{
    suppress_bcast = TRUE;
}   // log_bcasts_off

// -----------------------------------------------------------------------

void log_header_on (void)
/*
    turn headers on for logging.
*/
{
    suppress_header = FALSE;
}   // log_header_on

// -----------------------------------------------------------------------

void log_header_off (void)
/*
    turn headers off for logging
*/
{
    suppress_header = TRUE;
}   // log_header_off

// -----------------------------------------------------------------------

void log_orderids_on (void)
/*
    turn orderID (and transID) on for logging, i.e. stop suppression of them
*/
{
    suppress_orderid = FALSE;
}   // log_orderids_on

// -----------------------------------------------------------------------

void log_orderids_off (void)
/*
    turn orderID (and transID) off for logging, i.e. suppress them

    This would typically be done if the caller wished to be able to compare the
    logfiles from different runs, for regression testing.

    The default behaviour would have unique transIDs and orderIDs, and the
    files would fail to compare...
*/
{
    suppress_orderid = TRUE;
}   // log_orderids_off

// ---------------------------------------------------------------------------

int32 change_logger_file (char *new_logger_filename_s)
/*
    We wish to switch logger files.

    Open the new one, and if it opens OK, then switch to logging to it.

    Note that this must be synchronised with the actual logging,
    which is running in a different thread from the one which called this fn..
    and would probably clash/crash otherwise.
*/
{
    FILE *new_logfile_px = NULL;		    // file for logging raw logs, initially not allocated

    // attempt to open the new logfile
    new_logfile_px = fopen ( new_logger_filename_s, logfilemode );

    // how did the file open go ?
    if (new_logfile_px == NULL)
    {
	printf_time ();
	printf ("Error opening new log output file %s\n",
		    new_logger_filename_s);
	// strictly speaking, the file open may have failed for other reasons (say disk full), but unlikely
	return INVALID_ARGUMENTS;
    }
    else
    {
	// success opening new file.

	/* lock out the logger thread from trying to access the logfile
	    As the logger main thread gets its data from a locked Q, we
	    can use that as our synchronisation primitive
	*/
	lock_q ( &log_pending_q );

	// Close old file
	fclose ( logfile_px );

	// switch to new file
	logfile_px = new_logfile_px;

	// unlock the main logger thread so it can resume logging
	unlock_q ( &log_pending_q );
    }

    return SUCCESSFUL;
}   // change_logger_file
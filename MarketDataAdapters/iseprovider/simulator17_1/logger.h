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


Title:	logger.h.
Author: Neil Cossor

Description:

    Defines fns, types, consts etc, provided by logger.c
    This module is the interface to the logger for the ISESim program.
    The logger runs as an independent thread, with a (inter-thread
    synchronised) Q, into which other threads can put things they wish
    logged.
*/

// ---------------------------------------------------------------------------

void log_msg ( log_head_xt *msg_hdr, char * buffer, int buf_len );
/*
    this fn is responsible for logging a msg.
    It must force synchronisation restrictions, as it may be called by different threads,
    and we must NOT corrupt the stream of data being pumped into the logger thread

    The parameters are:-
    a msg header, which MUST be supplied
    the address of a buffer to log
    and the size of that buffer. If the size if 0, then do NOT log any
    of the data in the buffer
*/

// ---------------------------------------------------------------------------

int init_logger(char *logfile_name_ps);
/*
    initialise the logger thread etc.

    We assume that the default directory has been set already,
    hence we may open the logfile etc.

    Optionally can supply a logfile name, default used if none supplied

    Returns TRUE (non-zero) if everything OK, FALSE (0) otherwise.
    Parent process MUST terminate if NOT OK.
*/

// ---------------------------------------------------------------------------

void close_logger ();
/*
    close down the logger thread etc, close the log file...
*/

// ---------------------------------------------------------------------------

void log_bcast ( uint32 log_fn_code_u,
		    void *bcast_px,
		    uint32 rx_length_i );
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

// ----------------------------------------------------------------------------

void log_text ( char *text_msg_s );
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

// --------------------------- control functions -----------------------------

void logging_on (void);
/*
    turn logging on
*/

// ---------------------------------------------------------------------------

void logging_off (void);
/*
    turn logging off
*/

// ---------------------------------------------------------------------------

void log_timestamps_on (void);
/*
    turn timestamps on for logging
*/

// ---------------------------------------------------------------------------

void log_timestamps_off (void);
/*
    turn timestamps off for logging
*/

// -----------------------------------------------------------------------

void log_bcasts_on (void);
/*
    turn bcasts on for logging.
    Finer granularity (like particular broadcasts) may come later...
*/

// -----------------------------------------------------------------------

void log_bcasts_off (void);
/*
    turn bcasts off for logging
*/

// -----------------------------------------------------------------------

void log_orderids_on (void);
/*
    turn orderID (and transID) on for logging, i.e. stop suppression of them
*/

// -----------------------------------------------------------------------

void log_orderids_off (void);
/*
    turn orderID (and transID) off for logging, i.e. suppress them

    This would typically be done if the caller wished to be able to compare the
    logfiles from different runs, for regression testing.

    The default behaviour would have unique transIDs and orderIDs, and the
    files would fail to compare...
*/

// ---------------------------------------------------------------------------

int32 change_logger_file (char *new_logger_filename_s);
/*
    We wish to switch logger files.

    Open the new one, and if it opens OK, then switch to logging to it.

    Note that this must be synchronised with the actual logging,
    which is almost certainly running in a different thread from the
    one which called this fn.. and could clash
*/

// -----------------------------------------------------------------------

void log_header_on (void);
/*
    turn headers on for logging.
*/

// -----------------------------------------------------------------------

void log_header_off (void);
/*
    turn headers off for logging
*/


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


Title : command.c
Function :

    This file provides command handling to the ISESim.exe program.
    This includes interactive operation, and being driven by a script
    
Author:  Neil Cossor

Revision history :-

yymmdd	vers'n	author	    notes
------	------	------	    -----
9905xx	v0.00	Neil Cossor initial version
990910	v0.01		    added logger,file
			    added logger,auto
			    added quit (abort) command.. for ISE QA only
990914	v0.02		    added lock/unlock to sync access to global
			    added AUTOCDB ON|OFF control, where we don't reload CDB on logon
990917			    imported sim_pause from utility.c/.h, fix NT / Solaris #ifdef
990920			    settle on new commands WAIT,BOUNDARY and WAIT,UNTIL to call
			    new fns synch_time_offset and delay_until respectively
			    fix some ifdef WIN32 stuff, to do with fine-granularity time calls
990922			    fix bug in display_arglist.. change process_commands to return command 'index'
			    fix command count in run_script_file
990929			    mod process_commands to know whether in script or interactive
991005	v.03		    add logger,header commands
991019			    enhance prompting when QUOTE is issued with invalid subcommand
991104	v0.04		    add commands for bcast control, and loops (count or time based)
991127	v0.05		    added more of bcast control...
991130			    remove incorrect (irrelevant) reference to control.h
			    move not_yet_implemented routine to utility.h/.c
			    mod process_commands to report which command it didn't understand (default: in switch)
991201			    add command to query (just) locked orders.. formerly, retrieved as part of query_orderbook
000110			    add autoquote command (and subcommands), for ISE in-house back-end load testing
000118			    add SYNTHETIC command, to show known synthetics
			    upgrade set_defaults to show old values, when doing SET command
000122			    mod to be source-portable between NT and Solaris.. thanks Joshua P.
000204			    add _strupr, and change load_script_file to use it as required (Solaris compat.)
			    mod sim_pause, run_script_file, delay_until, synch_time_offset to use 'generic'
				time types and calls (set up in stdincl.h - Solaris compat.)
			    fix set_defaults (used hardcode ref to value #defined in const.h)
000213			    move _strupr (if not WIN32) to utility.h/.c
000328	v0.07		    added QUOTE,DELETE command
			    fix some argc checking in process_commands
000330			    fix a couple of minor things in load_script_file
000425	v0.08		    add LIST command (for underlyings)
000426	v0.09		    update parse_cmd_line to strip trailing whitespace
000505	v0.10		    move autoquote fns into autoqute.c
000601			    add TRADE,LOG command to trigger logging of trades (ISE, and away)
000620			    mod process_commands, autologger flip of log->off done by om_disconnect now
001219	v0.11		    add command to specify bcast poll delay
010301	v0.12		    add code to call new display_trading_states
0106xx			    add code for combo commands
010618			    add code for count,gtc command
010620			    add options in set_defaults for setting user vs member default selection
				for deletes, queries, broadcast subscriptions
010627			    add code for combo,amend
010709			    add code for speed,set|get.. speedbump params for PMM/CMM
010711			    add trade,partrectify command
011017	v0.13		    add commands to specify time limits for warnings of long bcast poll,
				and for unexpected system holdups between bcast polls
*/
#include "stdincl.h"

#include "autoqute.h"
#include "broadcast.h"
#include "error.h"
#include "connect.h"
#include "combos.h"
#include "command.h"
#include "instrument.h"
#include "logger.h"
#include "misc.h"
#include "mm.h"
#include "order.h"
#include "series.h"
#include "static.h"
#include "trades.h"
#include "underlying.h"
#include "utility.h"

// --------------------------- local type definitions ----------------------------

#define MAX_CMD_LINE	255	    // how big we allow input command lines to be
// storage of one command line, with forward link to the next one
typedef struct _command_record
{
   struct _command_record   *nextCommand_px;		    // pointer to next command
   int8 		    cmdLine_s[MAX_CMD_LINE];	    // the text of the current command line
} command_record_xt;


// the (static) private list of commands we are handling in a script

command_record_xt *command_chain_px	= NULL;     // file command list, initially empty
command_record_xt *command_tail_px	= NULL;     // command list tail, initially empty

/* delay between execution of individual script commands.
	On NT, in milliseconds, default 0, see SET,INTERVAL command
	On Solaris, in seconds
*/
static unsigned script_interval_u	= 0;

    // list of supported keywords, for primary commands (argv[0])
    // this MUST be in same order as the enumerated list below, for parsing/command search to work correctly
    char    *commands_s[] =
    {
	"AMEND",
	"ASK",
	"AUTOCDB",
	"AUTOQUOTE",
	"BCAST",
	"BID",
	"BIN",
	"BLOCK",
	"CAB",
	"CLEAR",
	"COMBO",
	"COUNT",
	"DELETE",
	"EXCHANGES",
	"EXIT",
	"FACILITATION",
	"GET",
	"HELP",
	"?",
	"INSTRUMENTS",
	"LIST",
	"LOAD",
	"LOCKED",
	"LOGGER",
	"LOGOFF",
	"LOGON",
	"LOOP",
	"MM",
	"NEW",
	"OPEN",
	"OPRA",
	"ORDERBOOK",
	"ORDERS",
	"PAUSE",
	"PMM",
	"PRICES",
	"PROMPT",
	"QUIT",
	"QUOTE",
	"RELEASE",
	"RUN",
	"SAVE",
	"SERIES",
	"SET",
	"SPEED",
	"STATE",
	"SYNTHETIC",
	"TRADE",
	"UNDERLYING",
	"VERSION",
	"VIEW",
	"VOLUMES",
	"WAIT",
	"WHOAMI",
	"~",	    // these comments aren't actually used.. as we can't use exact substring match to look for them
	"!"	    // ditto.. it just means the lists are the same length
    };
    // and enumerated list which corresponds - MUST be in the same order as strings above
    // NOTE however, there is an additional tail-end type - comment_e. While comments are NOT
    // recognized in the same way (exact substring match)
    enum command_et {
	    amend_e,
	    ask_e,
	    autocdb_e,
	    autoquote_e,
	    bcast_e,
	    bid_e,
	    bin_e,
	    block_e,
	    cab_e,
	    clear_e,
	    combo_e,
	    count_e,
	    delete_e,
	    exchanges_e,
	    exit_e,
	    facilitation_e,
	    get_e,
	    help_e,
	    help2_e,
	    instruments_e,
	    list_e,
	    load_e,
	    locked_e,
	    logger_e,
	    logoff_e,
	    logon_e,
	    loop_e,
	    mm_e,
	    new_e,
	    open_e,
	    opra_e,
	    orderbook_e,
	    orders_e,
	    pause_e,
	    pmm_e,
	    prices_e,
	    prompt_e,
	    quit_e,
	    quote_e,
	    release_e,
	    run_e,
	    save_e,
	    series_e,
	    set_e,
	    speed_e,
	    state_e,
	    synthetic_e,
	    trades_e,
	    underlying_e,
	    version_e,
	    view_e,
	    volumes_e,
	    wait_e,
	    whoami_e,
	    title_comment_e,
	    comment_e
    };


// ----------------------- Internal routines ---------------------------------

static void parse_cmd_line ( char *commandline_ps,
				int *argc,
				char *argv[] )
/*
Function:   parse_cmd_line
Author:     Neil Cossor
Modified:   20000426
Description:

    We have an input command line (which we know to exist, and has at least 1 char).
    The command line consists of a stream of COMMA separated strings.

    Parse them (using strtok), generating a C-style argc/argv version of the cmd line.

    We also strip out leading whitespace in each argument.
    strip trailing whitespace from each argument
    We also handle parameters NOT being supplied, while there are
    following params e.g. TRADES,PAST,19990101,19990801,,120000, where
    argv[4] == NULL
    argv[5] == &"120000"

Input Params:

    the original string
    pointer to the argc, the count of arguments we find in the line
    pointer to the array of string pointers, which we update to point to the
	appropriate parts of the original string

Output Params:

    the argc and argv are updated

Implicit Params: (IMPORTANT NOTE)

    As the parsing is done using strtok, it change the original string.
    Each time strtok recognises another token, it places a NUL in the source
    memory area, at the point it found the token separator.

    As a result, if you wish to rerun the same command line, you must make
    each call using a COPY of the command line string.
*/
{
    char    *one_arg_ps;
    char    *end_command_ps = commandline_ps + strlen ( commandline_ps );

    // init the argument array - NB this leaves argc with a value of 0
    for ( *argc = MAX_ARGC; *argc > 0; )
	argv [--(*argc)] = NULL;

    // start out pointing to the start of the command line
    one_arg_ps = commandline_ps;

    // while there is some commandline left
    while ( one_arg_ps < end_command_ps )
    {
	// is current char whitespace
	while ( isspace ( *one_arg_ps ))
	{
	    // whitespace - advance to next, but bail out if we ran out of chars
	    if ( ++one_arg_ps >= end_command_ps )
		return;
	}

	// we know we are pointing at non-whitespace, and have some commandline left
	// if we are now pointing at a delimiter, then we have a NULL parameter
	if ( *one_arg_ps == CMD_SEPARATOR[0] )
	{
	    // null command - more to go
	    argv[*argc] = NULL;
	    (*argc)++;
	    one_arg_ps++;
	}
	else
	{
	    // parse one token
	    one_arg_ps = strtok ( one_arg_ps, CMD_SEPARATOR );

	    // save pointer to the last token we found
	    argv [*argc] = one_arg_ps;

	    // advance to point just after the NUL put in by strtok
	    one_arg_ps += strlen ( one_arg_ps ) + 1;

	    // we may have some trailing whitespace in this token
	    while (isspace (argv [*argc][strlen(argv[*argc]) - 1]))
		argv [*argc][strlen(argv[*argc]) - 1] = NUL;

	    // note that we got one more token (after saving the last one)
	    (*argc)++;
	}   // test if we are currently pointing at delimiter - a NUL parameter
    }	// test if we have run out of input line

}   // parse_cmd_line

// ------------------------- externally accessible routines ----------------------

void clear_script_from_memory ( void )
/*
Function : clear_script_from_memory
Author	 : Neil Cossor 
Modified : 9905xx
Description:

    Removes the script file from memory
*/
{
    command_record_xt *LocalCmdCopy_px = NULL;	  /* command list tail */

    // code
    if ( command_chain_px != NULL )
    {
	do
	{
	    LocalCmdCopy_px = command_chain_px;
	    command_chain_px = command_chain_px->nextCommand_px;
	    free(LocalCmdCopy_px);

	} while (command_chain_px != NULL);
 
	command_tail_px = NULL;

	printf ("Script unloaded\n");
    }
}   // clear_script_from_memory

// ---------------------------------------------------------------------------

int32 run_script_file ( click_details_xt * click_px,
			int argc,
			char *argv[ ] )
/*
Function : run_script_file 
Author	 : Neil Cossor 
Modified : 20000204
Description:

    This function will run an already loaded script file( *.SIM).  
    It reads the command line to find out if the script is to be 
    run multiple times and  if so, runs the script that many times.

Input Parameters:

    The command line, with any arguments, in C-style argc/argv[] form.
    argv[0]	RUN
    argv[1]	<# of times to run>

Output Parameter:

    In theory returns the status of the run.
    In practice, none of the lower layers really return sensible statii,
    so in turn this doesn't return anything sensible.
*/
{
    // pointer to the current (script) command in the loaded list
    command_record_xt	    *ThisCommand_px = NULL;
    int8		    cmd_copy_s[255];
    // how many times we wish to run - default 1
    int32		    times_to_run_i;
    // number of times we have run - NOTE initial value set here
    int16		    times_run_i = 0;
    // the command 'index' of which command we processed.. actually it is the posn in the enumeration list
    int			    command_e;

    // some stats info
    Timeb_t		TimeBuffer1_x;
    Timeb_t		TimeBuffer2_x;

    float		  StartTime_i; 
    float		  FinishTime_i; 
    float		  ElapsedTime_i;
    int32		  NumOfTxns_i;


    if ( command_chain_px != NULL )
    {
	// if there is an argument, it is the repeat count

	if ( argc > 1 )
	{
	    times_to_run_i = read_unsigned_limited ( argv[1], UNDEFINED_INT32 );

	    // sensible number ?
	}
	else
	{
	    times_to_run_i = 1;
	}

	// note the start time of our run, and how many transactions we did
	ftime( &TimeBuffer1_x );

	NumOfTxns_i = 0;

	for ( times_run_i = 0; times_run_i < times_to_run_i; times_run_i++ )
	{
	    // do one run thru the loaded script

	    // point to start of command list
	    ThisCommand_px = command_chain_px;
	    // repeat until we run out of command list
	    while (ThisCommand_px != NULL)
	    {
		// make a copy of the command
		// so string tokenising doesn't foul us
		strncpy ( cmd_copy_s,
			    &ThisCommand_px->cmdLine_s[0],
			    sizeof ( cmd_copy_s ) - 1);
		// and execute it - noting that we are in a script
		command_e = process_commands ( click_px,
						cmd_copy_s,
						TRUE );

		//don't count comments etc
		switch ( command_e )
		{
		case comment_e:
		case title_comment_e:
		case pause_e:
		    break;

		default:
		    // all the rest of the commands, which we count
		    NumOfTxns_i = NumOfTxns_i + 1;
		}

		// advance to the next command in list
		ThisCommand_px = ThisCommand_px->nextCommand_px;

		// snooze for the nominated sleep interval (which is in milliseconds)
		Sleep(script_interval_u);
	    }

	    // note one more pass through the loop
	    times_run_i += 1;
	}   // loop, doing the required # of runs

	// report stats on total run
	ftime( &TimeBuffer2_x );

	StartTime_i = (float)(TimeBuffer1_x.time*1000) + TimeBuffer1_x.millitm;
	FinishTime_i = (float)(TimeBuffer2_x.time*1000) + TimeBuffer2_x.millitm;
	ElapsedTime_i = (float)((FinishTime_i - StartTime_i)/1000);

	printf( "\nElapsed Time for Run:\t\t %.3f seconds\n", ElapsedTime_i);

	printf( "Number of Transactions Processed  = %d\n", NumOfTxns_i);

	printf( "Number of Transactions per Second = %.1f\n", (NumOfTxns_i/ElapsedTime_i));

	return SUCCESSFUL;
    }
    else
    {
	printf ("\n There isn't a script loaded yet!\n");
    }

    return SUCCESSFUL;

}   // run_script_file

// ---------------------------------------------------------------------------

void view_script_file_in_memory(void)
/*
Function : view_script_file_in_memory
Author	 : Neil Cossor 
Modified : 9905xx
Description:

    This is just a debug tool for seeing what is in the list 
    populated by loading a sim file
*/
{
    command_record_xt	    *ThisCommand_px = NULL;


    if ( command_chain_px != NULL )
    {
	ThisCommand_px = command_chain_px; 
	while (ThisCommand_px != NULL)
	{
	    printf ( "%s\n", &ThisCommand_px->cmdLine_s[0] );
	    ThisCommand_px = ThisCommand_px->nextCommand_px;
	}

	printf ("","\n");
    }
    else
    {
	printf ( "No Sim script file loaded into memory to view.\n");
    }
}   // view_script_file_in_memory

// ---------------------------------------------------------------------------

int32 load_script_file ( char *script_filename_ps )
/*
Function : load_script_file
Author	 : Neil Cossor 
Modified : 20000330
Description:

    Loads an existing script file into memory if the file exists.
    This is appended to whatever is already in memory.

Input Params:

    The name of the script file (which may include the directory path).
    Note that the .SIM suffix is ALWAYS applied to the file.. any
    existing suffix is ignored.

Output params:

    Status of success / failure of opening and loading the script.

To do:

    Ensure that nested script files don't loop (no file recursion
    or inter-file loops).

Note: this fn is written to be recursive, so script files can nest.
*/
{
    FILE *ScriptFile_px;	    // scriptfile we are reading
    int8  Seperators_s[]=" ";	
    int8  InputFileName_s[80];
    int8  FileCmd_s[MAX_CMD_LINE];
    // and the arguments array - this is an array of pointers to strings
    // in particular to the component strings in the original command line
    char    *argv [MAX_ARGC];
    // and how many arguments
    int     argc;



    // we have already stripped off the command line arguments,
    // and have the filename - if the caller supplied one

    // arguments supplied, or interactive mode
    if ( ( script_filename_ps != NULL )
	&& ( strlen ( script_filename_ps ) != 0 ) )
    {
	// filename argument supplied
	strcpy ( InputFileName_s, script_filename_ps );
    }
    else
    {
	// no arguments supplied
	printf ( "invalid args :-  LOAD,<filename>\n");
	return -1;
    }

    // if user supplied suffix, then zot it with a NUL - may shorten filename
    strtok ( (char *)&InputFileName_s, ".");
    // and append the MANDATORY suffix, now we removed the old one
    strcat ( InputFileName_s, ".SIM");


    // attempt to open the file
    if (( ScriptFile_px = fopen( InputFileName_s, "r" )) == NULL )
    {
	// error opening the script file
	if ( InputFileName_s != NUL )
	{
	    printf ( "Script File < %s > not found\n",InputFileName_s);
	    return -1;
	}
	else
	{
	    printf ( "*** No File Name Entered! ***\n");
	    return -1;
	}
    }
    else
    {
	// file opened OK - now read the contents
	do
	{
	    // read the command line
	    if ( fgets ( FileCmd_s, sizeof (FileCmd_s), ScriptFile_px)!= NULL)
	    {
		if (( strlen(FileCmd_s) > 1)) 
		{

		    /* on all platforms, convert to upper case.. command recognition is by 
			matching against fixed uppercase string lists */
		    _strupr(FileCmd_s);

		    //handle "special" commands
		    /* NAC 20000204.. this second MUST be replaced soon.. it should NOT be looking
			for exact matches.. but using the parsing and string matching routines
			and then using the enumerated vale that is returned.. very old code */
		    if (( strncmp(FileCmd_s, "LOAD", 4))== 0)
		    {
			// as we are NOT storing the command to load the file, the destructive parse is OK
			parse_cmd_line ( FileCmd_s,
					    &argc,
					    &argv[0] );
			load_script_file ( argv[1] );
		    }
		    else if ((( strncmp(FileCmd_s, "RUN", 3))== 0)||(( strncmp(&FileCmd_s[0], "R", 1))== 0))
			printf ("RUN Command found in sim file....Ignoring it!\n");
		    else if ((( strncmp(FileCmd_s, "CLE", 3))== 0))
			printf ("CLEAR Command found in sim file....Ignoring it!\n");
		    else if (strncmp(&FileCmd_s[0], "*", 1)== 0)
		    {
//			  printf ("Ignoring comments\n");
		    }
		    else
		    {
			// a useful command, add it to the list of known commands
			if (command_tail_px == NULL)
			{
			    command_tail_px = ( command_record_xt * ) malloc ( sizeof ( command_record_xt ) );
			    if ( command_tail_px == NULL )
			    {
				printf ("command_tail_px Memory Allocation Error for File Command\n");
				return -1;
			    }
			    command_chain_px = command_tail_px;
			}
			else
			{
			    command_tail_px->nextCommand_px = ( command_record_xt * )
							     malloc ( sizeof ( command_record_xt ) );
			    if ( command_tail_px->nextCommand_px == NULL )
			    {
				printf ("command_tail_px Memory Allocation Error\n");
				return -1;
			    }
			    command_tail_px = command_tail_px->nextCommand_px;
			}

			// copy data to tail
			command_tail_px->nextCommand_px = NULL;
			// scan to end of line into the buffer
			strtok(FileCmd_s, "\n");
			// and save the script command
			if ( strlen ( FileCmd_s ) > MAX_CMD_LINE )
			    printf ("Input command truncated, greater than %u characters\n",
					MAX_CMD_LINE );
			// copy the (possibly truncated) command line
			strncpy(&command_tail_px->cmdLine_s[0], FileCmd_s, MAX_CMD_LINE );
		    }
		} /* end of reading lines */
	    }
	} while ( !feof(ScriptFile_px));  // ( Check_i < 1)
    }

    if (fclose ( ScriptFile_px ))
    {
	printf (" WARNING - error closing Script File %s\n",
		    InputFileName_s );
    }

    printf ( "File %s Loaded\n",InputFileName_s);

    return SUCCESSFUL;
}  // load_script_file

// ---------------------------------------------------------------------------

/*
Function : display_arglist
Author	 : Neil Cossor 
Modified : 20000122
Description:

    This function is used to print an argument list.
    For Command.c, this is used to spit out a 'comment title' line,
    whereas tandard comments do not appear on screen

    The ISEsim mainline uses it do report what the command=line arguments it was called with.
*/
int32 display_arglist ( int argc,
			char *argv[ ] )
{
   int	loop_i;

    // keep adding the strings from the argument line into the output line
    for ( loop_i = 0; loop_i < argc;  loop_i++)
	// add string arg, and separating space to output
	printf ( "%s ",argv[loop_i] );

    // add terminating string i.e. EOL
    printf ("\n"); 

    return SUCCESSFUL;
}   // display_arglist

// --------------------------------------------------------------------------

static void set_defaults ( click_details_xt * click_px,
			    int argc,
			    char *argv[] )

/*
Function : set_defaults
Author	 : Neil Cossor
Modified : 20010620
Description:

    we got a SET command.
    Process the various flavours, and set the appropriate values

Input Params:

    Standard argc/argv of the command line.

    Input formats include:-
    SET,UNderlying,<underlying name>
    SET,Series,<series name>
    SET,Interval,<# of seconds>	 - gap
    SET,USer
    SET,Member

Output Params:

    None

Implicit Params:

    Changes the default settings, stored in click_px->
*/
{
    char *set_options_s[] = {
	"INTERVAL",
	"MEMBER",
	"SERIES",
	"UNDERLYING",
	"USER"
    };
    enum set_options_e {
	set_interval_e,
	set_options_member_e,
	set_options_series_e,
	set_options_underlying_e,
	set_options_user_e,
    };

    // which selector did the caller specify
    int     choice_i;
    if ( argc < 2 )
    {
	printf ("SET,UNderlying,<undername> OR SET,Series,<series name> OR SET,Interval,<milliseconds>\n"
	        " OR SET,Member OR SET,USer\n");
	return;
    }

    // which option is the caller setting
    choice_i = select_string_option ( 5, set_options_s, argv[1] );
    switch ( choice_i )
    {
    case set_interval_e:
	// setting the inter-command script delay interval - BIG (arbitrary limit.. 1 hour)
	if ( argc < 3 )
	{
	    printf ("usage:- SET,Interval,<milliseconds>\n" );
	    return;
	}
	script_interval_u = read_unsigned_limited ( argv[2], 3600000 );
	printf ( "New inter-command script delay of %.3f seconds\n",
		    (float) script_interval_u / ONE_SECOND );

	break;

    case set_options_member_e:
	// we wish future queries and subscriptions to be for this member (not this user)
	click_px->user_member_default_u = user_member_member_e;

	break;

    case set_options_series_e :
	// setting series
	if ( argc < 3 )
	{
	    printf ("usage:- SET,Series,<series name>\n" );
	    return;
	}
	if ( strlen ( argv[2] ) > MAX_SERIES_NAME )
	{
	    // illegal length on command line for series name length
	    printf ("Series name too long - max of %d\n",
			MAX_SERIES_NAME );
	    return;
	}

	printf ("New default series %s, formerly '%s'\n",
		    argv[2],
		    &click_px->default_series_name_s );

	strcpy ( click_px->default_series_name_s,
		    argv[2] );

	break;

    case set_options_underlying_e :
	// setting underlying
	if ( argc < 3 )
	{
	    printf ("usage:- SET,UNderlying,<undername>\n" );
	    return;
	}
	if ( strlen ( argv[2] ) > MAX_UNDER_ID )
	{
	    // illegal length on command line for underlying length
	    printf ("Underlying name too long - max of %d\n",
			MAX_UNDER_ID );
	    return;
	}

	printf ("New default underyling %s, formerly '%s'\n",
		    argv[2],
		    &click_px->default_under_name_s );

	strcpy ( click_px->default_under_name_s,
		    argv[2] );

	break;

    case set_options_user_e:
	// we wish future queries and subscriptions to be for this user (not this member)
	click_px->user_member_default_u = user_member_user_e;

	break;

    default:
	printf ("Illegal SET qualifier %s, must be one of Underlying | Series\n",
		    argv[1] );
    }
}   // set_defaults

// ----------------------------------------------------------------------------

void  sim_pause ( int argc,
		    char *argv[1] )
/*
Function : sim_pause
Author	 : Neil Cossor 
Modified : 20000204
Description:

    Takes a decimal and pauses the program that many seconds.
    Reports how long it actually waited.

Input Params:

    standard C-style argc/argv[] params

    argv[0]	PAUSE command
    argv[1]	<pause> in seconds
*/
{
    int8	    *StopString_ps;
    double	    SleepTime_d;
    // time before and after we pause
    Timeb_t	    TimeBuffer1_x;
    Timeb_t	    TimeBuffer2_x;
    double	    StartTime_i; 
    double	    FinishTime_i; 
    double	    ElapsedTime_i;


    // reasonable pause param
    if ( (argv[1]==NULL) || !strlen ( argv[1] ))
    {
	printf ( "\tERROR - No interval value supplied to sim_pause.\n");
    }
    else
    {
	SleepTime_d = strtod ( argv[1], &StopString_ps);
	if (SleepTime_d > 0)
	{
	    printf ("Paused (%s) for .. ",
			argv[1] );

	    // note the exact time, before we sleep
	    ftime( &TimeBuffer1_x );

	    // do the sleep
	    Sleep((int32)(SleepTime_d * ONE_SECOND));

	    // note the exact time, after we sleep
	    ftime( &TimeBuffer2_x );
	    StartTime_i = (double)(TimeBuffer1_x.time*ONE_SECOND) + TimeBuffer1_x.millitm;
	    FinishTime_i = (double)(TimeBuffer2_x.time*ONE_SECOND) + TimeBuffer2_x.millitm;
	    ElapsedTime_i = (double)((FinishTime_i - StartTime_i)/ONE_SECOND);

	    printf( " %.3f seconds\n",
			ElapsedTime_i);
	}
	else
	{
	    printf ("Pausing for Zero seconds or less doesn't make sense!\n");
	}
    }	// test if we got a pause param
}   // sim_pause

// --------------------------------------------------------------------------

void delay_until ( char *target_time_s,
		    int roll_forward_day )
/*
Function : delay_until
Author	 : Neil Cossor
Modified : 20000204
Description:

    the operator / script wishes to delay until a nominated time, which
    is specified by the string parameter.

    Note that other threads of ISEsim are unaffected by the sleep,
    so the broadcast thread (running if logged on) and the logger
    should still be cruising along.

    As the input to Sleep is 32 bit, in milliseconds, then
    we can have a max delay of about 49 days.. which is more than adequate.

Input Params:

    The input string is in the format hh[:mm[:ss]].
	Note that with our standard parsing, leading and trailing whitespace
	should have been stripped out.
    Whether we wish to roll forward to tomorrow's date if the time is past

    If the time has already expired:
	if roll_forward_day is FALSE, then we don't wait,
	otherwise we use that time tomorrow as the wake-up point
    If the time has not expired, then we ignore the roll_forward_day flag
*/
{
    // time we wish to wake up
    time_t	wakeup_time_u,
    // time now
		current_time_u;
    // the time offset between our target time and the present time
    double	time_offset_d;
    // the target time, broken down into individual hh, mm, ss components etc..
    // this is what we must build up from the nominated input string
    struct tm	wakeup_time_x;
    // status from sscanf convert.. EOF is error, or no fields converted
    int		sscanf_status_i;

    // get the current time, in seconds, since some arbitrary point
    current_time_u = time ( NULL );
    // fill in the target time (struct) with all the details for today
    memcpy ( &wakeup_time_x, localtime ( &current_time_u ), sizeof ( wakeup_time_x ));
    // but zotting the time of day part.. so we default to 0 mins, 0 secs
    wakeup_time_x.tm_hour = 0;
    wakeup_time_x.tm_min = 0;
    wakeup_time_x.tm_sec = 0;
    // try to read hh:mm:ss
    sscanf_status_i = sscanf ( target_time_s,
				"%hu:%hu:%hu",
				&wakeup_time_x.tm_hour,
				&wakeup_time_x.tm_min,
				&wakeup_time_x.tm_sec );

    // did the user give us silly values ? Must have at least an hour field.
    if ( ( wakeup_time_x.tm_hour > 23 ) ||
	    ( wakeup_time_x.tm_min > 59 ) ||
	    ( wakeup_time_x.tm_sec > 59 ) ||
	    sscanf_status_i < 1
	)
    {
	printf ("Error - bad input time.. hh[:mm:[ss]]\n" );
	return;
    }

    // convert our component wakeup time into a time format,
    // we can use to figure out how long to wait
    wakeup_time_u = mktime ( &wakeup_time_x );
    time_offset_d = difftime ( wakeup_time_u, current_time_u );

    // has the time already elapsed today ?
    if ( time_offset_d < 0 )
    {
	// time has elapsed - are we supposed to roll forward to tomorrow
	if ( !roll_forward_day )
	{
	    // yes, roll forward one day
	    printf ("Sync time of %s has elapsed - no delay incurred\n",
			asctime ( &wakeup_time_x ) );

	    // break out of here - return to caller
	    return;
	}

	// adjust our wait time forwards - add the seconds in a day
	wakeup_time_u += SECONDS_IN_A_DAY;
	// fix our time record for displaying the target time
	memcpy ( &wakeup_time_x, localtime ( &wakeup_time_u ), sizeof ( wakeup_time_x ));
	// and fix up our delay time
	time_offset_d = difftime ( wakeup_time_u, current_time_u );
    }
    else
    {
	// time has NOT elapsed yet today.. just wait
    }

    printf ("delaying until %s\n",
		asctime ( &wakeup_time_x ) );

    Sleep ( (int32) time_offset_d * ONE_SECOND );
}   // delay_until

// --------------------------------------------------------------------------

void synch_time_offset ( char *target_time_s )
/*
Function : synch_time_offset
Author	 : Neil Cossor
Modified : 20000204
Description:

    The operator / script wishes to delay until a nominated neat
    multiple time offset from the normal clock hour, as specified by the
    string parameter.

    Note that other threads of ISEsim are unaffected by the sleep,
    so the broadcast thread (running if logged on) and the logger
    should still be cruising along.

    For example, if the param was 15.. then we would pause until
    the next neat multiple of 15 from the clean hour.. i.e. until
    hh:00, hh:15, hh:30, hh:45 to come along. What we are actually looking
    for, is the next time at which the minutes in the hour is mod 0 with
    our parameter time.

Input Params:

    The input string is the # of minutes as the next neat multiple of minutes.
	Note that with our standard parsing, leading and trailing whitespace
	should have been stripped out.

    While the number can be anything from 1 to 59, it will most likely
    be a multiple of 5, larger than 1, and less than 31.
*/
{
    // time we wish to wake up
    time_t	wakeup_time_u,
    // time now 
		current_time_u;
    // the target minute #, converted from the supplied string param
    unsigned	minute_u;
    // the time offset between our target time and the present time
    double	time_offset_d;
    // pointer to the target time, in broken-down struct form
    struct tm	wakeup_time_x;

    // get the current time, in seconds, since some arbitrary point
    current_time_u = time ( NULL );
    // fill in the target time (struct) with all the details for NOW
    memcpy ( &wakeup_time_x, localtime ( &current_time_u ), sizeof ( wakeup_time_x ));
    // read the supplied input param, max value 59
    minute_u = read_unsigned_limited ( target_time_s, 59 );
    // did the user give us a silly value ?
    if ( ( minute_u > 59 ) ||
	    ( minute_u <= 0 ))
    {
	printf ("Error - target time (valid range 1-59)" );
	return;
    }

    // bring the target minute up to the next multiple of the nominated interval
    wakeup_time_x.tm_min = ( (int) ( wakeup_time_x.tm_min / minute_u ) + 1 ) * minute_u;
    wakeup_time_x.tm_sec = 0;
    // did we just go past the end of this hour
    if ( wakeup_time_x.tm_min >= 60 )
    {
	// yes, past end of this hour... make it the start of the next
	wakeup_time_x.tm_min = 0;
	wakeup_time_x.tm_hour++;
	// did we just go past the current day ?
	if ( wakeup_time_x.tm_hour >= 24 )
	{
	    /* yes, go to start of next day.. urk, this is getting messy
		as there is no 'easy' way to wrap to the next day, as the
		mktime fn ignores the yday. Instead, force ourselves to midnight,
		by going to midnight of today, and adding the seconds in a day
	    */
	    wakeup_time_x.tm_hour = 0;
	    // convert our component wakeup time into a time format,
	    wakeup_time_u = mktime ( &wakeup_time_x );
	    wakeup_time_u += (60 * 60 * 24 );
	}
	else
	{
	    // did NOT roll into the next day
	    wakeup_time_u = mktime ( &wakeup_time_x );
	}   // test if we rolled into the next day
    }
    else
    {
	// did NOT roll  into the next hour
	wakeup_time_u = mktime ( &wakeup_time_x );
    }	// test if we rolled to the next hour

    // figure out the offset to wait
    time_offset_d = difftime ( wakeup_time_u, current_time_u );

    // update our target time (struct) with all the details, so we can report out it
    memcpy ( &wakeup_time_x, localtime ( &wakeup_time_u ), sizeof ( wakeup_time_x ));
    // report when we will wait until
    printf ("delaying until %s\n",
		asctime ( &wakeup_time_x ) );
    // and wait
    Sleep ( (int32) time_offset_d * ONE_SECOND );
}   // synch_time_offset

// --------------------------------------------------------------------------

int32 process_commands ( click_details_xt * click_px,
			    char *commandline_ps,
			    unsigned is_script_u )
/*
Function : process_commands
Author	 : Neil Cossor
Modified : 20010709
Description:

    This function maintains the command line prompt, parses
    any supplied input and calls the appropriate function

    This does do by parsing the command into an array of strings,
    and passing the info to the called fns...
    much like the argc/argv of C main() programs.

    All commands and parameters are space separated.
    The initial command which is the primary selector is argc[0] etc,
    as per normal argc/argv.

Input Params:

    address of the main click data block. This is needed to pass to most
	ISEsim routines which actually handle the particular functions.
    address of the command line string
    flag, which indicates if we are executing a line in a script
	TRUE - in a script
	FALSE - running from interactive terminal (stdin)

Implicit Params:

    Note that in parsing the command line, we use the standard C parsing
    routine (strtok) inserts a NUL character at the end of each 'parsed'
    parameter, on each call. The interesting side-effect is that this
    function hence implicitly alters the original string.

    Accordingly, if the caller wishes to be able to process a command
    more than once (say, in repeat scripting), the caller MUST call
    this routine with a copy of the string.
*/
{

    int32   status_i = SUCCESSFUL;
    // the selections from a list of string choices
    int     choice_i,
	    choice_2_i;	    // a secondary choice e.g. used for bcast commands

    // and the arguments array - this is an array of pointers to strings
    // in particular to the component strings in the original command line
    char    *argv [MAX_ARGC];
    // and how many arguments
    int     argc;
    // dummy buffer, used when manual PROMPT is done (in script, PROMPT command)
    int8    dummy_buffer_s[40];
    // local temp int32, if we need one
    int32   tmp_int32_i;
    // local temp double, if we need one
    double tmp_double_f;


    // declaration of string choices for BCAST, argv[1]
    static char *bcast_subcommands_s [] =
    {
	"ALL",		// we are turning overall bcasts on | off
	"ELAPSELIMIT",	// set time limit for one bcast poll
	"MM",		// for MMs, to select Bcasts by bin only.. NYI
	"POLL",		// to set our bcast poll delay
	"SHOW",		// show subscriptions
	"SYSTEMLIMIT"	// set time limit for max "missing" time between bcast polls
    };
    // enumerated list, corresponding 1:1 (and in same order) as string list
    enum bcast_subcommands_et { bcast_all_e, bcast_elapselimit_e, bcast_mm_e, 
				bcast_poll_e, bcast_show_e, bcast_systemlimit_e };

    // declaration of string choices for BLOCK and FACILITATION
    static char *block_facil_subcommand_s [] =
    {
	"BID",
	"ASK",
	"RESPONSE"
    };
    // enumerated list, corresponding 1:1 (and in same order) as string list
    enum block_facil_subcommands_et { block_facil_bid_e, block_facil_ask_e, block_facil_response_e };

    // declaration of string choices for COMBO, argv[1]
    static char *combo_subcommands_s [] =
    {
	"AMEND",    // we wish to amend a combo which is in the central combobook
	"ASK",	    // we are adding an ASK leg to the in-memory combo (to enter with SEND)
	"BID",	    // we are adding a BID leg to the in-memory combo (to enter with SEND)
	"BOOK",	    // display what we have in the local combobook
	"CLEAR",    // clear the in-memory combo we had been building
	"DELETE",   // delete one (or more) combos from central system (and local copy)
	"HIT",	    // hit a combo which is in the combobook
	"LOAD",	    // load local copy of combobook (member and public )
	"SAVE",	    // save data from last combo send (can reference some fields with %O in HIT, AMEND, DELETE)
	"SEND",	    // send the in-memory combo. This also specifies values for the header of the combo
		    // the legs were already built up with COMBO,BID|ASK
	"VIEW"	    // display the in-memory combo we have been building
    };
    // enumerated list, corresponding 1:1 (and in same order) as string list
    enum combo_subcommands_et { combo_amend_e, combo_ask_e, combo_bid_e, combo_book_e, combo_clear_e,
				combo_delete_e, combo_hit_e, combo_load_e, combo_save_e, combo_send_e, combo_view_e };

    // declaration of string choices for get/set option
    static char *get_set_s [] =
    {
	"GET",
	"SET",
	"Unk"
    };
    // enumerated list, corresponding 1:1 (and in same order) as string list
    enum get_set_et { get_set_get_e, get_set_set_e };

    // declaration of string choices for logger subcommands
    // NB put on,off as first two, so can use same list (but with only 2 choices) for handling lower subcommands
    static char *logger_subcommands_s []=
    {
	"OFF",
	"ON",
	"AUTO",
	"BCASTS",
	"FILE",
	"HEADER",
	"ORDERIDS",
	"TIMESTAMPS"
    };
    enum logger_subcommands_et {
	logger_off_e,
	logger_on_e,
	logger_auto_e,
	logger_bcasts_e,
	logger_file_e,
	logger_header_e,
	logger_orderids_e,
	logger_timestamps_e
    };

    // same deal for string choices for open
    static char *open_subcommands_s [] =
    {
	"QUERY",
	"REQUEST"
    };
    enum open_subcommands_et { open_query_e, open_request_e };

    // same deal for string choices for opra
    static char *opra_subcommands_s [] =
    {
	"BEST",
	"EACH"
    };
    enum opra_subcommands_et { opra_best_e, opra_each_e };

    // declaration of string choices for quote subcommands
    static char *quote_subcommands_s [] =
    {
	"ASK",
	"BID",
	"CLEAR",
	"DELETE",
	"PANIC",
	"QUERY",
	"SEND",
	"VIEW"
    };
    // enumerated list, corresponding 1:1 (and in same order) as string list
    enum quote_subcommands_et { quote_ask_e,
				quote_bid_e,
				quote_clear_e,
				quote_delete_e,
				quote_panic_e,
				quote_query_e,
				quote_send_e,
				quote_view_e
				};
    // same deal for string choices for TRADES
    static char *trade_subcommands_s [] =
    {
	"TODAY",
	"PARTRECTIFY",
	"PAST",
	"RECTIFY",
	"REPORT",
	"LOG"
    };
    enum trade_subcommands_et { trade_today_e, trade_partrectify_e, trade_past_e, trade_rectify_e,
				trade_report_e, trade_log_e  };

    // same deal for string choices for WAIT
    static char *wait_subcommands_s [] =
    {
	"BOUNDARY",
	"UNTIL",
    };
    enum wait_subcommands_et { wait_boundary_e, wait_until_e };



    // anything meaningful in the command line
    if ( commandline_ps == NULL )
	return -1;	    // eof detected

    if ( strlen ( commandline_ps ) == 0 )
	return SUCCESSFUL;  // return key pressed

    // parse the command line
    parse_cmd_line ( commandline_ps,
			&argc,
			&argv[0] );
    /* comments are special cases - we just care that the command line starts with one
	We are NOT looking for a substring match, as per the other commands
	i.e. if we have  "! this is a comment" on the input line, the normal
	matching would require that our input, up to the first separator (a comma)
	be an exact substring match of a known list.. which is not sensible for comments
    */
    switch ( *argv[0] )
    {
	case '~':
		// line started with '~' character. Rest of line ignored, but dumped to screen
		status_i = display_arglist ( argc,
						&argv[0]);
		// return our 'command' type
		return title_comment_e;

		break;	// redundant after a return

	case '!':
		// ordinary comment: do nothing, just return our 'command' type
		return comment_e;

		break;	// redundant after a return

	// default here does nothing.. we want to look for commands
    }

    choice_i = select_string_option ( 56, &commands_s[0], argv[0] );
    switch ( choice_i )
    {
	case ask_e:

		    // grab the global - we are about to change it
		    lock_global ( click_px );

		    // ASK,<series name>,<# contracts>,<premium>,<Xref string>[,<validity>[,<Type>[,<client category>[,<ignore_away>[,<bkr id>]]]]]
		    status_i = generate_order ( click_px,
						argc,
						&argv[0],
						ASK );

		    // release the global - we are done with changes
		    unlock_global ( click_px );

		    break;

	case amend_e:
		    // AMEND
		    // grab the global - we are about to change it
		    lock_global ( click_px );

		    amend_order( click_px,
				    argc,
				    &argv [0]);
		    // release the global - we are done with changes
		    unlock_global ( click_px );

		    break;  

	case autocdb_e:
		    // AUTOCDB, ON | OFF
		    /* we cheat here.. we need an on/off choice.. and we know that the logger_subcommand_s
			choices start with ON | OFF
		    */
		    choice_i = select_string_option ( 2, logger_subcommands_s, argv[1] );

		    switch ( choice_i )
		    {
		    case logger_off_e:
			turn_auto_cdb_load_off ();
			printf ("AutoCDB loading is now OFF\n");
			break;
		    case logger_on_e:
			turn_auto_cdb_load_on ();
			printf ("AutoCDB loading is now ON\n");
			break;
		    default:
			printf ("For AUTOCdb, one of ON | OFF must be specified\n");
			break;
		    }   // end switch on {AUTOCDB} ON|OFF

		    break;

	case autoquote_e:
		    /*
			except for show, disable, enable..
			
			the rest are setting params which are by underlying, and hence take a <underlying list>
			<underlying list> == *|%bin|%folio|<under>[,...]

			AUTOQUOTE, DISABLE					kill all autoquoting (flag.. params unchanged)
			AUTOQUOTE, ENABLE					enable autoquoting (doesn't check params)
			AUTOQUOTE, INTEREST, <int%>, <underlying list>		sets interest rates for one or more underlying
										int% can contain 2 decimal places
			AUTOQUOTE, OFF, <underlying list>			turns autoquoting off " " " "
			AUTOQUOTE, ON, <underlying list>			turns autoquoting on " " " "
			AUTOQUOTE, OPEN_QUANTITY, <open_qty>, <underlying list>	sets the quantity. used in opening rotations
			AUTOQUOTE, QUANTITY, <quantity>, <underlying list>	sets the quantity, used in normal trading
			AUTOQUOTE, SHOW						shows autoquoting settings for the bin
			AUTOQUOTE, STEPUP, <underlying list>			set the step-up buffer quantity
			AUTOQUOTE, VOLATILITY, <vol%>, <underlying list>	sets volatility for one or more underlying
										vol% can contain 2 decimal places
		    */
		    choice_i = select_string_option ( 10, autoquote_subcommands_s, argv[1] );

		    switch ( choice_i )
		    {
    			case autoquote_disable_e:
			    // turn off all autoquoting
			    disable_autoquote ();
			    break;
			case autoquote_enable_e:
			    // turn on all autoquoting
			    enable_autoquote ();
			    break;

			case autoquote_openqty_e :
			case autoquote_qty_e :

			    // both of these, we need to read the argv[2] - unsigned (no decimals), target is uint32
			    tmp_int32_i = read_unsigned_limited ( argv [ 2 ], UNDEFINED_UINT32 );

			    autoquote_change ( click_px,
						choice_i,
						tmp_int32_i,
						0.0,
						(argc - 3),
						&argv[3] );

			    break;

			case autoquote_stepup_e :

			    // we need to read the argv[2] - unsigned (no decimals), target is uint16
			    tmp_int32_i = read_unsigned_limited ( argv [ 2 ], UNDEFINED_UINT16 );

			    autoquote_change ( click_px,
						choice_i,
						tmp_int32_i,
						0.0,
						(argc - 3),
						&argv[3] );

			    break;

			case autoquote_interest_e:
			    // attempt to set the interest rate
			case autoquote_volatility_e:
			    // attempt to set the volatility

			    // must have a number setting and a underlying selection.
			    if (argc <4)
			    {
				printf ("insufficient arguments for AUTOQUOTE,Interest|Volatility,<value>,<under.list>\n");
				break;
			    }

			    // for both interest and volatility, we need to read the argv[2]
			    tmp_double_f = atof ( argv[2] );

			    autoquote_change ( click_px,
						choice_i,
						0,
						tmp_double_f,
						(argc - 3),
						&argv[3] );

			    break;

			case autoquote_off_e:
			    // attempt to turn autoquoting off
			case autoquote_on_e:
			    // attempt to turn autoquoting on
			    autoquote_change ( click_px,
						choice_i,
						0,
						0.0,
						(argc - 2),
						&argv[2] );

			    break;

			case autoquote_show_e:
			    // show the current autoquote settings
			    autoquote_show ( click_px );
			    break;

			default:
			    printf ("Error, AUTOQuote choices DISABLE | ENABLE | INTEREST | OFF | ON | OPEN_QUANTITY\n"
				    "    | QUANTITY | STEPUP | SHOW | VOLATILITY must be specified\n");
			    break;
		    }   // end switch on {AUTOQUOTE} choices

		    break;

	case bcast_e:
		    /*	BCAST, ALL, ON | OFF	    en(dis)ables all bcasts, or kills all bcasts..
							if offline, this prevents / enables subscribe-all at logon,
							if online, then cancels / enables immediately
			BCAST,ELAPSELIMIT,<delay>   set limit (milliseconds) for one bcast poll, b4 warning added to log
			BCAST, MM, ON | OFF	    (MM only) toggles bcasts to be for underlying this MM has responsibility for
			BCAST, POLL, <delay>	    sets delay (in milliseconds) between bcast polls
			BCAST, SHOW [,<bcastID>]    displays which authorised subscriptions we are actually listening to
						    ?? will we add option of just seeing info about one bcast ?
			BCAST, <bcastID>, ON | OFF, <*|%bin|<under>[,...]..  enables / disables individual bcasts
						    NB can't say * with anything else, can't repeat %bin, only MM can use %bin
			BCAST,SYSTEMLIMIT,<delay>   set limit (milliseconds) for system time loss between polls
							b4 warning added to log
		    */
		    // find what was was selected in argv[1] .. ALL, MM, SHOW
		    choice_i = select_string_option ( 6, bcast_subcommands_s, argv[1] );
		    // for all cases other than SHOW & POLL, we need to know ON | OFF for argv [2]
		    if (( choice_i != bcast_show_e ) && ( choice_i != bcast_poll_e ))
		    {
			// read next arg (may not be present, which select_string_option can handle)
			// we know it is OFF | ON.. cheat, and reuse logger subcommands list
			choice_2_i = select_string_option ( 2, logger_subcommands_s, argv[2] );
		    }	// see if we needed to read ON | OFF argv[2]

		    switch ( choice_i )
		    {
		    case bcast_all_e:	// one of OFF / ON
			// we have already parsed argv[2]
			switch ( choice_2_i )
			{
			case logger_off_e:
			    disable_optional_bcasts ( click_px );
			    printf ("Optional broadcasts now all OFF\n");
			    break;
			case logger_on_e:
			    enable_optional_bcasts ( click_px );
			    printf ("Optional broadcasts now all ON\n");
			    break;
			default:
			    printf ("Illegal BCAST,ALL option. Must be one of OFF | ON\n");
			    break;
			}   // end switch on {BCAST,ALL} ON|OFF

			break;

		    case bcast_elapselimit_e:
			// we are setting the upper limit of time for one bcast poll, in milliseconds

			// read the value from argv[2] - unsigned (no decimals), target is uint32
			tmp_int32_i = read_unsigned_limited ( argv [ 2 ], UNDEFINED_UINT16 );

			bcast_set_poll_limit ( tmp_int32_i );

			printf ("Bcast poll limit set to %.3f seconds\n", ((float) tmp_int32_i/1000));

			break;

		    case bcast_mm_e:

			not_yet_implemented ( "BCAST, MM" );

			break;

		    case bcast_poll_e:
			// we are setting the bcast poll interval, in milliseconds

			// read the value from argv[2] - unsigned (no decimals), target is uint32
			tmp_int32_i = read_unsigned_limited ( argv [ 2 ], UNDEFINED_UINT16 );

			bcast_set_poll_delay ( tmp_int32_i );

			printf ("Bcast poll delay set to %.3f seconds\n", ((float) tmp_int32_i/1000));

			break;

		    case bcast_show_e:

			display_subscriptions ( click_px );

			break;

		    case bcast_systemlimit_e:
			// we are setting the upper limit for system time-loss between bcast polls, in milliseconds

			// read the value from argv[2] - unsigned (no decimals), target is uint32
			tmp_int32_i = read_unsigned_limited ( argv [ 2 ], UNDEFINED_UINT16 );

			bcast_set_system_poll_limit ( tmp_int32_i );

			printf ("Bcast system delay limit set to %.3f seconds\n", ((float) tmp_int32_i/1000));

			break;

		    default:
			// this includes erroneous commands, and actual individual subscribe / cancel by bcast type
			// call lower routine, leaving out the BCAST, and the ON | OFF selector
			// we have already parsed argv[2]
			switch ( choice_2_i )
			{
			case logger_off_e:
			    not_yet_implemented ("BCAST, <bcast_type>, OFF");
			    break;
			case logger_on_e:
			    // argv[1] is the broadcast type (as a string)
			    // skip over the first 3 args
			    add_one_subscription ( click_px,
						    argv[1],
						    (argc - 3),
						    &argv[3] );
			    break;
			default:
			    printf ("Illegal BCAST,%s,%s arguments.\n",
					argv[1],
					argv[2] );
			    break;
			}   // switch on BCAST, argv[2]
		    }   // end switch on BCAST, argv[1]

		    break;
	case bid_e :
		    // BID... as per ASK
		    // grab the global - we are about to change it
		    lock_global ( click_px );

		    status_i = generate_order ( click_px,
						argc,
						&argv[0],
						BID );
		    // release the global - we are done with changes
		    unlock_global ( click_px );

		    break;
	case bin_e :
		    // BIN,<bin #>
		    // grab the global - we require consistent common
		    lock_global ( click_px );

		    status_i = display_bin ( click_px,
						argc,
						&argv[0] );
		    // release the global - we are done with changes
		    unlock_global ( click_px );

		    break;
	case block_e :
		    // BLOCK,BID or BLOCK,ASK, or BLOCK,RESPONSE
		    // grab the global - we require consistent common
		    lock_global ( click_px );

		    choice_i = select_string_option ( 3, block_facil_subcommand_s, argv[1] );
		    switch ( choice_i )
		    {
		    case block_facil_bid_e:
		    case block_facil_ask_e:
			// BLOCK,BID/ASK,<seriesname>,<qty>,<premium>,<Xref>[,<validity>[,<client category>[,<expose flag>[,<bkr id>]]]]]]
			status_i = generate_block_order ( click_px,
							    argc,
							    &argv[0] );
			break;
		    case block_facil_response_e:
			// BLOCK,RESPONSE,BID/ASK,<seriesname>,<qty>,<premium>,<Xref>[,<orderID>[,<bkr id>]]
			status_i = generate_block_facil_response ( click_px,
								    ORDER_CAT_BLOCK_RESPONSE,
								    argc,
								    &argv[0] );
			break;
		    default:
			printf ("Illegal BLOCK command, must be one of ASK | BID | RESPONSE\n");
		    }

		    // release the global - we are done with changes
		    unlock_global ( click_px );

		    break;

	case cab_e :
		    // CAB,BID/ASK,<seriesname>,<qty>,<Xref>[,<validity>[,<client category>[,<bkr id>]]]
		    // grab the global - we are about to change it
		    lock_global ( click_px );

		    status_i = generate_cab_order ( click_px,
						    argc,
						    &argv[0] );
		    // release the global - we are done with changes
		    unlock_global ( click_px );

		    break;
	case clear_e :
		    // CLEAR (the in-memory script) - no arguments
		    clear_script_from_memory();
		    break;

	case combo_e:
		    /* COMBO sub commands, including

			BOOK,MEMBER|PUBLIC|USER	    show combobook.. filtered in later release
			LOAD			    queries system to load local combobook

			ASK,<details for one leg>   set up a combo leg for next combo,send
			BID,<details for one leg>
			CLEAR			    clear out the stuff we were building up for combo
			SEND,<details for overall combo>    set up overall details, and send incl. legs
			VIEW			    show what is in memory in the current combo

			HIT,<details for overall hit, including clear details for our legs>

			AMEND,<details for overal amend, including clear details for legs>

			DELETE by local # | <underyling> | * .. with filter on interest/executable
								?? filter on user vs member ??

			SAVE			    take snapshot of last combo.
						    Can reference data from this with %O
						    Used in COMBO AMEND|DELETE|HIT
		    */
		    choice_i = select_string_option ( 11, combo_subcommands_s, argv[1] );
		    switch ( choice_i )
		    {
		    case combo_amend_e:
			parse_send_amend_combo ( click_px,
						    argc,
						    &argv[0] );
			break;

		    case combo_ask_e:
		    case combo_bid_e:
			add_combo_leg ( click_px,
					argc,
					&argv[0] );
			break;

		    case combo_book_e:
			// COMBO,BOok[,Member|Public|User]	// just whole local combobook for now
			display_my_combobook ( click_px );
			break;

		    case combo_clear_e:
			// clear out anything we had built up in legs
			clear_combo_legs ();
			break;

		    case combo_delete_e:
			parse_delete_combo ( click_px,
						argc,
						&argv[0] );
			break;

		    case combo_hit_e:
			// we wish to hit a combo that is in the combobook
			parse_send_hit ( click_px,
					    argc,
					    &argv[0] );
			break;

		    case combo_load_e:
			// query the back-end for info about public and member's combos
			clear_local_combobook ( click_px );
			query_public_combos (click_px );
			query_member_active_combos ( click_px );
			query_member_inactive_combos ( click_px );
			break;

		    case combo_save_e:
			// snapshot the last combo we entered into a 'saved' slot for later user
			snapshot_save_last_combo ( click_px );
			break;

		    case combo_send_e:
			// pack up the legs (possibly none) and send into back-end
			parse_send_combo ( click_px,
					    argc,
					    &argv[0] );
			break;

		    case combo_view_e:
			// view the legs we had built up in memory for this combo
			view_combo_legs ();
			break;

		    default:
			printf ("Illegal COMBO command, choose ASk|AMend|BId|BOok|Clear|Delete|Hit|Load|SAve|SEnd|View\n");
		    }

		    break;
	case count_e:
		    // at present just have COUNT,GTC
		    status_i = count_gtc ( click_px,
					    argc,
					    &argv[0] );

		    break;
	case delete_e :
		    // DELETE
		    // grab the global - we are about to change it
		    lock_global ( click_px );

		    status_i = delete_order ( click_px,
						argc,
						&argv[0] );
		    // release the global - we are done with changes
		    unlock_global ( click_px );

		    break;
	case exchanges_e:
		    // EXCHANGES
		    // grab the global - we require consistent common
		    lock_global ( click_px );

		    display_exchanges (click_px);

		    // release the global - we are done with common
		    unlock_global ( click_px );

		    break;
	case exit_e:
		    // EXIT
		    exit ( 1 );     // NB this will go thru SimulatorShutdown
		    break;

	case facilitation_e :
		    // FACILITATION,BID or FACILITATION,ASK, or FACILITATION,RESPONSE
		    // grab the global - we are about to change it
		    lock_global ( click_px );

		    choice_i = select_string_option ( 3, block_facil_subcommand_s, argv[1] );
		    switch ( choice_i )
		    {
		    case block_facil_bid_e:
		    case block_facil_ask_e:
			// FACILITATION,BID/ASK,<seriesname>,<qty>,<premium>,<Xref>[,<client category>[,<bkr id>]]
			status_i = generate_facilitation_order ( click_px,
								    argc,
								    &argv[0] );
			break;
		    case block_facil_response_e:
			// FACILITATION,RESPONSE,BID/ASK,<seriesname>,<qty>,<premium>,<Xref>[,<orderID>[,<bkr id>]]
			status_i = generate_block_facil_response ( click_px,
								    ORDER_CAT_FACILITATION_RESPONSE,
								    argc,
								    &argv[0] );
			break;
		    default:
			printf ("Illegal FACILITATION command, must be one of ASK | BID | RESPONSE\n");
		    }

		    // release the global - we are done with common
		    unlock_global ( click_px );

		    break;

	case get_e:
		    // Get,<ACTIVE|INACTIVE>,<*|S>[,<series name>]
		    // get (query) orders
		    // grab the global - we are about to change it
		    lock_global ( click_px );

		    status_i = query_orders ( click_px,
						argc,
						&argv[0] );

		    // release the global - we are done with common
		    unlock_global ( click_px );

		    break;
	case help_e:
	case help2_e:
		    // HELP or ?
		    display_help_screen();	// print a list of implemented commands
		    break;

	case instruments_e:
		    // INSTRUMENTS, no arguments
		    // grab the global - we are about to change it
		    lock_global ( click_px );

		    display_instrument_groups ( click_px );

		    // release the global - we are done with common
		    unlock_global ( click_px );

		    break;
	case list_e : // LIST
		    lock_global ( click_px );

		    // one of LIST,[SERIES|UNDERLYING]
		    choice_i = select_string_option ( 2, &under_series_s [0], argv[1] );
		    switch ( choice_i )
		    {
		    case under_series_series_e :
			list_series ( click_px,
					argc,
					&argv[0] );
			break;
		    case under_series_under_e:
			list_underlyings ( click_px,
					    argc,
					    &argv[0] );
			break;
		    default:
			printf ("For LIST, one of SERIES|UNDERLYING\n" );
		    }	// end of selector within LIST command

		    unlock_global ( click_px );
		    break;
	case load_e :
		    // LOAD
		    // NB ignores return status from load ???
		    status_i = load_script_file ( argv[1] );
		    break;
	case locked_e :
		    // LOCKED,<UNDERLYING|SERIES>,<undername>|<seriesname>
		    // hmm.. do we want a flavour LOCKED,* ?
		    // grab the global - we require consistent common
		    lock_global ( click_px );

		    status_i = query_locked_orders ( click_px,
							argc,
							&argv[0] );
		    // release the global - we are done with common
		    unlock_global ( click_px );

		    break;
	case logger_e :
		    /* LOGGer choices are :-
			LOGGER,ON|OFF			turn (all) logging on or off
			LOGGER,AUTO,ON|OFF		turn auto logging on or off
			LOGGER,BCAST,ON|OFF		turn logging of (all) broadcasts on or off
			LOGGER,FILE,<filename>		change to using new logger output file, name supplied
			LOGGER,ORDERIDS,ON|OFF		turn orderID (and transID) logging on or off
			LOGGER,TIMESTAMPS,ON|OFF	turn timestamp logging on or off
		    */
		    choice_i = select_string_option ( 8, logger_subcommands_s, argv[1] );
		    switch ( choice_i )
		    {
		    case logger_off_e:
			// LOGGER, OFF
			logging_off ();
			printf ("Logging is now OFF\n");
			break;
		    case logger_on_e:
			// LOGGER, ON
			logging_on ();
			printf ("Logging is now ON\n");
			break;
		    case logger_auto_e:
			// LOGGER, AUTO, ON | OFF
			choice_i = select_string_option ( 2, logger_subcommands_s, argv[2] );
			switch ( choice_i )
			{
			case logger_off_e:
			    click_px->auto_logging_b = FALSE;
			    printf ("Auto-Logging is now OFF\n");
			    break;
			case logger_on_e:
			    click_px->auto_logging_b = TRUE;
			    printf ("Auto-Logging is now ON\n");
			    break;
			default:
			    printf ("For LOGGER,Auto, one of ON | OFF must be specified\n");
			    break;
			}   // end switch on {LOGGER, AUTO,} ON|OFF
			break;

		    case logger_bcasts_e:
			// LOGGER, BCASTS, ON | OFF
			choice_i = select_string_option ( 2, logger_subcommands_s, argv[2] );
			switch ( choice_i )
			{
			case logger_off_e:
			    log_bcasts_off ();
			    printf ("Logging is no longer includes broadcasts\n");
			    break;
			case logger_on_e:
			    log_bcasts_on ();
			    printf ("Logging now includes broadcasts\n");
			    break;
			default:
			    printf ("For LOGGER,Bcasts, one of ON | OFF must be specified\n");
			    break;
			}   // end switch on {LOGGER, BCASTS,} ON|OFF
			break;

		    case logger_file_e:
			// user wants to change log files
			status_i = change_logger_file ( argv[2] );

			if ( status_i == SUCCESSFUL )
			    printf ("Logger output now going to file %s\n",
					argv[2] );
			else
			    printf ("Error attempting to switch log output to file %s\n",
					argv[2] );
			break;

		    case logger_header_e:
			// LOGGER, HEADER, ON | OFF
			choice_i = select_string_option ( 2, logger_subcommands_s, argv[2] );
			switch ( choice_i )
			{
			case logger_off_e:
			    log_header_off ();
			    printf ("Logging is no longer includes headers\n");
			    break;
			case logger_on_e:
			    log_header_on ();
			    printf ("Logging now includes headers\n");
			    break;
			default:
			    printf ("For LOGGER,Header, one of ON | OFF must be specified\n");
			    break;
			}   // end switch on {LOGGER, HEADER,} ON|OFF
			break;

		    case logger_orderids_e:
			// LOGGER, ORDERIDS, ON | OFF
			choice_i = select_string_option ( 2, logger_subcommands_s, argv[2] );
			switch ( choice_i )
			{
			case logger_off_e:
			    log_orderids_off ();
			    printf ("Logging is no longer includes OrderIDs or TransIDs\n");
			    break;
			case logger_on_e:
			    log_orderids_on ();
			    printf ("Logging now includes OrderIDs and TransIDs\n");
			    break;
			default:
			    printf ("For LOGGer,ORderids, one of ON | OFF must be specified\n");
			    break;
			}   // end switch on {LOGGER, ORDERIDS,} ON|OFF
			break;

		    case logger_timestamps_e:
			// LOGGER, TIMESTAMPS, ON | OFF
			choice_i = select_string_option ( 2, logger_subcommands_s, argv[2] );
			switch ( choice_i )
			{
			case logger_off_e:
			    log_timestamps_off ();
			    printf ("Logging is no longer includes timestamps\n");
			    break;
			case logger_on_e:
			    log_timestamps_on ();
			    printf ("Logging now includes timestamps\n");
			    break;
			default:
			    printf ("For LOGGER,Timestamps, one of ON | OFF must be specified\n");
			}   // end switch on {LOGGER, TIMESTAMPS,} ON|OFF
			break;

		    default:
			printf ("One of LOGGer,ON | OFf | Auto | Bcast | File | Header | ORderids | Timestamps must be specified\n");
			break;
		    }	// switch on logger subcommand
		    break;
	case logoff_e :
		    // LOGOFF

		    // grab the global - we are about to change it
		    lock_global ( click_px );

		    clear_local_orderbook(click_px);
		    om_disconnect ( click_px );

		    // release the global - we are done with common
		    unlock_global ( click_px );

		    break;

	case logon_e :
		    // LOGON

		    // grab the global - we are about to change it
		    lock_global ( click_px );

		    status_i = om_connect(click_px,
					    argc,
					    &argv[0]);

		    // release the global - we are done with common
		    unlock_global ( click_px );

		    break;
	case loop_e:
		    /*	LOOP, <label>, COUNT, <count>	    start of loop, with label, and count. Count MUST be +ve.
			LOOP, <label>, TIME, <hh:mm>	    start of loop, with label, and timelimit. Don't execute if past time.
			LOOP, <label>, END		    check loop condition (count or time), and branch to label if OK
		    */
		    not_yet_implemented ( "LOOP" );

		    break;
	case mm_e:
		    // MM GET or SET commands
		    // grab the global - we require consistent common
		    lock_global ( click_px );

		    choice_i = select_string_option ( 2, get_set_s, argv[1] );
		    switch ( choice_i )
		    {
		    case get_set_get_e:
			// MM,GET
			status_i = query_mm_parameters ( click_px,
							    argc,
							    &argv[0] );
			break;
		    case get_set_set_e:
			// MM,SET
			status_i = set_mm_parameters ( click_px,
							argc,
							&argv[0] );
			break;
		    default:
			printf ("One of MM,GET or MM,SET must be specified\n");
			break;
		    }	// end of switch on get/set option

		    // release the global - we are done with common
		    unlock_global ( click_px );

		    break;
	case new_e:
		    // NEW - call whatever fn it is we are debugging

		    // grab the global - we require consistent common
		    lock_global ( click_px );
		    // release the global - we are done with common
		    unlock_global ( click_px );
//		    printf ("New == nothin\n" );
		    break;

	case open_e:
		    // OPEN, QUERY or OPEN, REQUEST
		    // grab the global - we require consistent common
		    lock_global ( click_px );

		    choice_i = select_string_option ( 2, open_subcommands_s, argv[1] );
		    switch ( choice_i )
		    {
		    case open_query_e:
			// OPEN,QUERY
			status_i = query_possible_opening ( click_px,
							    argc,
							    &argv[0] );
			break;
		    case open_request_e:
			// OPEN, REQUEST
			status_i = request_opening ( click_px,
							argc,
							&argv[0] );
			break;
		    default:
			printf ("One of OPEN,REQUEST or OPEN,QUERY must be specified\n");
		    }

		    // release the global - we are done with common
		    unlock_global ( click_px );

		    break;
	case opra_e:
		    // OPRA, BEST or OPRA, EACH
		    // grab the global - we require consistent common
		    lock_global ( click_px );

		    choice_i = select_string_option ( 2, opra_subcommands_s, argv[1] );
		    switch ( choice_i )
		    {
		    case opra_best_e:
			// OPRA, BEST
			status_i = query_opra ( click_px,
						TRUE,
						argc,
						&argv[0] );
			break;
		    case opra_each_e:
			// OPRA, EACH
			status_i = query_opra ( click_px,
						FALSE,
						argc,
						&argv[0] );
			break;
		    default:
			printf ("One of OPEN,BEST or OPEN,EACH must be specified\n");
		    }

		    // release the global - we are done with common
		    unlock_global ( click_px );

		    break;
	case orderbook_e:
		    // ORDERBOOK
		    // grab the global - we require consistent common
		    lock_global ( click_px );

		    status_i = query_orderbook ( click_px,
						    argc,
						    &argv[0] );
		    // release the global - we are done with common
		    unlock_global ( click_px );

		    break;

	case orders_e:
		    // ORDERS - i.e. show my own orders
		    // grab the global - we require consistent common
		    lock_global ( click_px );

		    display_my_orderbook (click_px);

		    // release the global - we are done with common
		    unlock_global ( click_px );

		    break;
	
	case pause_e:
		    // PAUSE[,<# of seconds>]
		    sim_pause ( argc,
				&argv[0] );
		    break;

	case pmm_e:
		    // PMM,GET or PMM,SET
		    // grab the global - we require consistent common
		    lock_global ( click_px );

		    choice_i = select_string_option ( 2, get_set_s, argv[1] );
		    switch ( choice_i )
		    {
		    case get_set_get_e:
			// PMM,GET
			status_i = query_pmm_parameters ( click_px,
							    argc,
							    &argv[0] );
			break;
		    case get_set_set_e:
			// PMM,SET
			status_i = set_pmm_parameters ( click_px,
							argc,
							&argv[0] );
			break;
		    default:
			printf ("One of PMM,GET or PMM,SET must be specified\n");
			break;
		    }	// end of switch on get/set option

		    // release the global - we are done with common
		    unlock_global ( click_px );

		    break;

	case prompt_e:
		    /* PROMPT,<prompt text>.. this ONLY works in a script file,
			spits out a prompt to the command line, and waits for
			the user to hit RETURN.

			This is to allow for scripts which require some external
			event to be manually triggered (or confirmed), and then
			hit return (resuming the script) when the user is ready.
		    */
		    if ( is_script_u == TRUE )
		    {
			// spit out the prompt.. if they gave us one
			if ( argv[1] != NULL )
			    printf ( "%s [hit return]> ", argv[1] );
			else
			    printf("Manual prompt [hit return]> ");

			// get a command from the user - actually we don't care about it
			read_kbd_input ( &dummy_buffer_s[0] );
		    }
		    else
		    {
			printf ("PROMPT cmd only allowed in scripts - ignored\n");
		    }
		    break;

	case prices_e:
		    // PRICES[,<underlying>]
		    // grab the global - we require consistent common
		    lock_global ( click_px );

		    status_i = enter_underlying_price_query(click_px,
							    argc,
							    &argv[0] );
		    // release the global - we are done with common
		    unlock_global ( click_px );

		    break;
	
	case quit_e:
		    /* QUIT - this is for ISE internal testing only.
			This will do a hard abort, which should exercise some
			interesting things in the gateway, and things like automatic
			order inactivation for EAMs (if they are set up that way)
		    */
		    abort ();

		    break;
	case quote_e:
		    // all of the QUOTE commands
		    // grab the global - we require consistent common
		    lock_global ( click_px );

		    choice_i = select_string_option ( 7, quote_subcommands_s, argv[1] );
		    switch ( choice_i )
		    {
		    case quote_ask_e:
		    case quote_bid_e:
			// QUOTE,ASK,...    add an ask quote component to the in-memory table
			// QUOTE,BID,...    add a bid quote component to the in-memory table
			status_i = add_quote_component ( click_px,
							    argc,
							    &argv[0] );
			break;
		    case quote_clear_e:
			// QUOTE,CLEAR	    clear the in-memory table
			clear_quote_table ();
			break;
		    case quote_delete_e:
			// QUOTE,DELETE	    send quote deletion
			delete_quote ( click_px,
					argc,
					&argv[0] );
			break;
		    case quote_panic_e:
			// QUOTE,PANIC	    delete all quotes (bid and ask, all underlying in this bin) for this member
			panic_delete_quote ( click_px,
						argc,
						&argv[0] );
			break;
		    case quote_query_e:
			// QUOTE,QUERY	    find out the PMM/MMs standing in the central system
			status_i = query_quote ( click_px,
						    argc,
						    &argv[0] );
			break;
		    case quote_send_e:
			// QUOTE,SEND	    send the in-memory table as a single block quote, and clear
			status_i = send_quote ( click_px );
			break;
		    case quote_view_e:
			// QUOTE,VIEW	    show the in-memory table
			display_quote_table ( click_px );
			break;
		    default:
			// QUOTE, unknown
			printf ("Error - unknown QUOTE command '%s', must be one of ASK | BID | CLEAR | QUERY | SEND | VIEW\n",
				    argv[1] );
			break;
		    }	// switch on start of second argument

		    // release the global - we are done with common
		    unlock_global ( click_px );

		    break;
	
	case release_e:
		    // RELEASE locked order
		    // grab the global - we require consistent common
		    lock_global ( click_px );

		    status_i = release_locked_order ( click_px,
							argc,
							&argv[0] );
		    // release the global - we are done with common
		    unlock_global ( click_px );

		    break;
	case run_e:
		    // RUN[,<# of times>]
		    status_i = run_script_file ( click_px,
						    argc,
						    &argv [0]);
		    break;

	case save_e:
		    // SAVE,ORDER   .. later we might do a SAVE,TRADE command
		    snapshot_save_last_order ( click_px );
		    break;

	case series_e :
		    // SERIES[,<underlying>]
		    // grab the global - we require consistent common
		    lock_global ( click_px );

		    display_series ( click_px,
					argc,
					&argv [0]);
		    // release the global - we are done with common
		    unlock_global ( click_px );

		    break;

	case set_e :
		    // SET,Underlying,<underlying name>
		    // SET,Series,<series name>
		    set_defaults ( click_px,
				    argc,
				    &argv[0] );
		    break;

	case speed_e:
		    // SPEED,GET or SET commands
		    // grab the global - we require consistent common
		    lock_global ( click_px );

		    choice_i = select_string_option ( 2, get_set_s, argv[1] );
		    switch ( choice_i )
		    {
		    case get_set_get_e:
			// SPEED,GET
			status_i = query_speedbump_parameters ( click_px,
								    argc,
								    &argv[0] );
			break;
		    case get_set_set_e:
			// SPEED,SET
			status_i = set_speedbump_parameters ( click_px,
								argc,
								&argv[0] );
			break;
		    default:
			printf ("One of SPEED,GET or SPEED,SET must be specified\n");
			break;
		    }	// end of switch on get/set option

		    // release the global - we are done with common
		    unlock_global ( click_px );

		    break;

	case state_e:
		    // STATE,<start string>
		    display_trading_states ( click_px,
						argv [1] );

		    break;

	case synthetic_e:
		    /* SYNTHETIC    maybe we will add some other flavours later, which show
				    the prices of the synthetic etc
		    */
		    display_synthetics ( click_px );
		    break;

	case trades_e :
		    // TRADE, TODAY or TRADE, PAST or TRADE,RECTIFY or TRADE,REPORT
		    // grab the global - we require consistent common
		    lock_global ( click_px );

		    choice_i = select_string_option ( 6, trade_subcommands_s, argv[1] );
		    switch ( choice_i )
		    {
		    case trade_today_e:
			status_i = query_todays_deals ( click_px,
							argc,
							&argv [0]);
			break;
		    case trade_past_e:
			status_i = query_historical_deals ( click_px,
							    argc,
							    &argv[0] );
			break;
		    case trade_rectify_e:
			status_i = trade_rectify ( click_px,
						    argc,
						    &argv[0] );
			break;
		    case trade_partrectify_e:
			status_i = trade_rectify_parts ( click_px,
							    argc,
							    &argv[0] );
			break;
		    case trade_report_e:
			status_i = trade_report ( click_px,
						    argc,
						    &argv[0] );
			break;
		    case trade_log_e:
			open_tradefile ( click_px );
			break;
		    default:
			printf ("Error, unknown TRADES command, must be one of TODAY|PARTRECTIFY|PAST|RECTIFY|REPORT|LOG\n");
		    }	// switch on trades subcommand

		    // release the global - we are done with common
		    unlock_global ( click_px );

		    break;
	case underlying_e :
		    // UNDERLYING[,<underlying string>]
		    // grab the global - we require consistent common
		    lock_global ( click_px );

		    display_underlyings ( click_px,
					    argc,
					    &argv [0] );
		    // release the global - we are done with common
		    unlock_global ( click_px );

		    break;
	case version_e:
		    // VERSION - no parameters
		    printf ("Software version '%s'\n",
				SIM_VERSION );
		    break;
	case view_e:
		    // VIEW - no parameters
		    view_script_file_in_memory();
		    break;
       
	case volumes_e:
		    // VOLUMES
		    // grab the global - we require consistent common
		    lock_global ( click_px );

		    status_i = series_volume_prices_query ( click_px,
							    argc,
							    &argv[0] );
		    // release the global - we are done with common
		    unlock_global ( click_px );

		    break;
	case wait_e :
		    // WAIT,UNTIL,<time hh[:mm[:ss]]> or WAIT,BOUNDARY,<minutes>
		    choice_i = select_string_option ( 2, wait_subcommands_s, argv[1] );
		    switch ( choice_i )
		    {
		    case wait_until_e:
			/* wait until a nominated actual time hh[:mm[:ss]]
			    If the time has elapsed today, then user can tell us they
			    would like to roll to tomorrow... by specifying ROLL as
			    the 4th param
			*/
			if ( argc < 4 )
			    // no ROLL param.. 
			    delay_until ( argv[2], FALSE );
			else
			    // 4th param - assume that it is ROLL (for now)
			    delay_until ( argv[2], TRUE );
			break;
		    case wait_boundary_e:
			// wait until the next N minute boundary from the hour.. N is the last argument
			synch_time_offset ( argv[2] );
			break;
		    default:
			printf ("Error, unknown WAIT command, must be one of BOUNDARY | UNTIL\n");
		    }	// case on wait subcommands

		    break;

	case whoami_e:
		    // WHOAMI - no arguments
		    status_i = who_am_i ( click_px );
		    break;

	default:
		    printf ("\nUnknown Command (%s), Help is Available by Typing HELP\n",
				argv[0] );
		    break;
    }

    return choice_i;
}   // process_commands


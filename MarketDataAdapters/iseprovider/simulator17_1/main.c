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


Title : main.c
Author:  Neil Cossor
Description :

    This file is the main module of the ISESim.exe program.
    
    ISESim is a demonstration program for users of the ISE CLICK system,
    as supplied by the OM Gruppen.

    This module was split from the ISEsim.c file of earlier releases,
    to make the code more flexible for developers building test libraries
    based on our code, rather than just a simple stand-alone application
   
Revision history :

yyyymmdd    vers'n	author	    notes
--------    ------	------	    -----
20000126    v0.00	Neil Cossor initial version - split from ISEsim.c
*/
// OM header files
#include "stdincl.h"

// and all of our ISEsim stuff.. well, as much as we need access to from here
#include "command.h"
#include "ISEsim.h"
#include "utility.h"

// ------------ variables private to this module -----------------------------

// access by other fns is be explicit parameter passing

click_details_xt *click_px;
// pointer to the log filename
char *user_log_filename_ps = NULL;


// -------------- globals, which are directly accessed by other routines -----


// -------------------------------------------------------------------------------

int main ( int argc,
	    char *argv[ ] )
/*
Function:   main
Author:     Neil Cossor
Modified:   20000122

Description:

   This function starts the program, gathers the log on data, logs on to the
   system, downloads various data, sets up the exit procedure,
   and kicks off the main loop.

Input Parameters:

    (generic C) The first string (argv[0]) is the program name, and each
    following string is an argument passed to the program from the command
    line. The last pointer (argv[argc]) is NULL.

    (app specific, if supplied.. may be NULLed by specifying "" in command line)
    argv[1] == name of logfile for output. Default LOGFILE.TXT
    argv[2] == name of User file, containing CLICK username, gateway name etc
	defaults to name (in const.h) #defined by USER_DATA_FILENAME
    argv[3] == name of script file to run. No default.
	If supplied, then it is
	- loaded immediately
	- run
	- and then the application exits
	If not supplied, then we run in interactive mode. Note that a script
	file can be LOADed and RUN from interactive mode.
*/
{
    int32   status_i		=   SUCCESSFUL;
    int8    command_line_s[USER_INPUT_BUFFER_LEN];


	click_details_xt z;
	int k = sizeof(z.log_reply_x.trans_reply_x );

    /* code */
    printf("\n\nWelcome to Version %s of the simulator.\n",SIM_VERSION);
    printf("______________________________________________\n\n");
    printf("\nThis session started on " );
    printf_time ();
    printf ("\n");
    printf ("Commandline args are :-\n");
    display_arglist ( argc, &argv[0] );

    // stash the supplied command line params
    // try for 4th param - script filename
    if ( argc > 3)
    {
	// have enough arguments for a script filename
	user_script_filename_ps = argv[3];
    }
    if ( argc > 2)
    {
	// have enough arguments for a user param filename
	strcpy ( user_param_filename_ps, argv[2] );
    }
    if ( argc > 1)
    {
	// have more arguments than just the program name - try for logfile name
	user_log_filename_ps = argv[1];
    }	// test if enuff params for a logfile name to be supplied

    // try to set everything up... including allocation of memory for generic data
    click_px = ISESim_init ( argc, &argv[0] );
    if (click_px == NULL)   // 0
    {
	printf("\nAborting - error starting logger\n");
	exit(0);
    }

    // if the caller supplied a SIM script file, then we just load that,
    // execute it, and exit
    if ( ( user_script_filename_ps != NULL )
	&& ( strlen ( user_script_filename_ps ) != 0 ) )
    {
	// load and run the script
	status_i = load_script_file ( user_script_filename_ps );
	// did the load work OK
	if (status_i == SUCCESSFUL)
	{
	    // attempt to actually run the script
	    // 1 argument, no "parameter", so it makes it default to 1 run
	    status_i = run_script_file ( click_px, 1, NULL );
	}   // test if script load worked OK
    }
    else
    {
	// print out help prompt
	printf("Commands useful at this point are:- \n\tLOAD LOGON HELP EXIT QUIT.\n\n");
	printf("Please Type '?' or Help for Help on Commands\n");

	status_i = SUCCESSFUL;
	while (status_i >= SUCCESSFUL)
	{
	    // zot the input line
	    memset (&command_line_s[0],
		   (char)0,
		    sizeof (command_line_s));

	    // process keyboard input
	    printf("ISESim> ");
	    // get a command from the user
	    read_kbd_input ( &command_line_s[0] );
	    // and process it
	    status_i = process_commands (click_px,
					    &command_line_s[0],
					    FALSE );
	}   // while no errors processing commands, and no exit/quit request
    };	// test if running a script from initial program arguments

    exit (status_i);
}   // main

/******************* End Of File ************************************************************/

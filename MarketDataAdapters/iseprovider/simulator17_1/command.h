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

  
Title : command.h

Function :

    This file is the header for command.c, which provides
    command handling to the ISESim.exe program.
    This includes a command-line interface (DOS prompt)
    and running from a script file (which can, in turn, load
    other script files).
    
Author:  Neil Cossor

Revision history :-

yymmdd	vers'n	author	    notes
------	------	------	    -----
9905xx	v0.00	Neil Cossor initial version
990929	v0.01		    mod process_commands to know whether in script or interactive
000122	v0.02		    make former static display_comment() into visible display_arglist ()
*/

#ifndef COMMAND_H
#define COMMAND_H


// ---------------------------------------------------------------------------

int32 process_commands ( click_details_xt * click_px,
			    char *commandline_ps,
			    unsigned is_script_u );
/*
Function : process_commands
Author	 : Neil Cossor
Modified : 9905xx
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

// --------------------------- scripting routines ----------------------------

void clear_script_from_memory ( void );
/*
Function : clear_script_from_memory
Author	 : Neil Cossor 
Modified : 9905xx
Description:

    Removes the script file from memory
*/

// ---------------------------------------------------------------------------

int32 run_script_file ( click_details_xt * click_px,
			int argc,
			char *argv[ ] );
/*
Function : run_script_file 
Author	 : Neil Cossor 
Modified : 9905xx
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

// ---------------------------------------------------------------------------

void view_script_file_in_memory(void);
/*
Function : view_script_file_in_memory
Author	 : Neil Cossor 
Modified : 9905xx
Description:

    This is just a debug tool for seeing what is in the list 
    populated by loading a sim file
*/

// ---------------------------------------------------------------------------

int32 load_script_file ( char *script_filename_ps );
/*
Function : load_script_file
Author	 : Neil Cossor 
Modified : 9906xx
Description:

    Loads an existing script file into memory if the file exists.

Input Params:

    The name of the script file (which may include the directory path).
    If not supplied, then the user will be prompted for the filename.
    Note that the .SIM suffix is ALWAYS applied to the file.. any
    existing suffix is ignored.

Output params:

    Status of success / failure of opening and loading the script.

To do:

    Allow ensure that nested script files don't loop (no file recursion
    or inter-file loops).
*/

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
			char *argv[ ] );



#endif

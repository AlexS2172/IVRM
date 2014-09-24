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


Title:	ISEsim.h
Author: Neil Cossor

    Header file, to allow some other ISEsim modules to access a couple
    of routines which belong to the the ISEsim.c, which also contains
    the C main() fn.

Revision history :

yyyymmdd    vers'n	author	    notes
--------    ------	------	    -----
20000127    v0.00	Neil Cossor Mod to have argc, argv to ISEsim_init
*/
#ifndef SIM_H
#define SIM_H


// ---------------------------------------------------------------------------

void simulator_shutdown ( void );
/*
Function : simulator_shutdown
Author	 : Neil Cossor 
Modified : 9905xx
Description:

    Called on exit from the simulator
    o unloads any script left in memory, 
    o releases all structures, such as our knowledge of underlying, series etc
	i.e. don't trust Windoze to do it correctly...
    o logs out from OMnet (if logged on)
    o tells the logger to close down
*/

// ---------------------------------------------------------------------------

void release_market_structures ( click_details_xt * click_px );
/*
Function:   release_market_structures
Author:     Neil Cossor
Modified:   9907xx
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

// -------------------------------------------------------------------------------

click_details_xt * ISESim_init (int argc, char * argv[]);
/*
Function:   ISESim_init
Author :    Neil Cossor
Modified :  20000122
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


#endif

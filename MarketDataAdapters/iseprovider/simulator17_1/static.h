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


Title : static.h
Author:  Neil Cossor

Function :

    This file the header for static.c, which provides facilities to
    collect all static data from the central system, to the ISESim.exe program.

    Note that only ONE routine is provided at this time, which loads ALL
    subsidiary static data.

    If some smaller granularity is required, then we will either have to add
    some selection criteria to that routine, or provide access to the lower
    level routines, which are currently private to the module.
    
Revision history :-

yymmdd	vers'n	author	    notes
------	------	------	    -----
9905xx	v0.00	Neil Cossor initial version

*/
#ifndef STATIC_H
#define STATIC_H


int32 load_static_data (click_details_xt * click_px);
/*
Function:   load_static_data
Author:	    Neil Cossor
Modified:   2001
Description:

    Loads all of the CDB data, using the DQ queries.

    Also does a couple of basic market queries,
    like the underlying statii, and the instrument type statii.
*/

// -------------------------------------------------------------------------------------

int32 query_user_type ( click_details_xt * click_px );
/*
Function:   query_user_type
Author:	    Neil Cossor
Modified:   20010706
Description:

    Load the information about our user

    This returns information about the list of transactions, queries
    and broadcasts which are enabled for our particular user.

    The data is displayable with the WHOAMI command
*/

#endif

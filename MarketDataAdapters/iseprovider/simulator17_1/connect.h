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


Title:	connect.h
Author: Neil Cossor
Description:

    This is the header file for connect.c, which provides logon/logoff
    functionality for the ISEsim application.
*/
#ifndef CONNECT_H
#define CONNECT_H

// Public functions

// -------------------------------------------------------------------------------------

int32 om_connect ( click_details_xt * click_px,
		   int argc,
		   char *argv [] );
/*
Function : om_connect
Author	 : Neil Cossor / Neil Cossor
Modified : 9905xx
Description:
 
    Collects user information and then logs that user on. 
    Once done, the standing data for that user is loaded. 
*/

// -------------------------------------------------------------------------------------

int32 om_disconnect ( click_details_xt * click_px);
/*
Function : Disconnnect
Author	 : Neil Cossor
Modified : 9905xx
Description:

    Logs the user off and disconnects the session to the exchange gateway
*/

// ---------------------------------------------------------------------------

void turn_auto_cdb_load_off ();
/*
Function:   turn_auto_cdb_load_off
Author:	    Neil Cossor
Modified:   990914
Description:

    We are doing QA script testing with static CDB.. so we will crank up
    our testing by NOT reloading the CDB at each logon...
*/

// ---------------------------------------------------------------------------

void turn_auto_cdb_load_on ();
/*
Function:   turn_auto_cdb_load_on
Author:	    Neil Cossor
Modified:   990914
Description:

    We wish to switch back the default behaviour.. reloading the CDB at each logon...
*/


#endif

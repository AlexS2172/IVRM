/*

Title:	global.c
Author:	Neil Cossor
 *
 *	Copyright (c) 1999 International Securities Exchange Limited.
 *	ALL RIGHTS RESERVED.
 *
 *	This software is the exclusive property of the International Securities 
 *	Exchange Limited.
 *
 *	Any reproduction of this software in whole or part is forbidden without 
 *	the written consent of the International Securities Exchange Limited and the
 *	inclusion of the above copyright notice.
 *
 *	The information in this software is subject to change without notice 
 *	and should not be construed as a commitment by the International Securities 
 *	Exchange Limited. 
 *
 *	The International Securities Exchange Limited cannot accept any responsibility, 
 *	financial or otherwise, for any consequences arising from the use of 
 *	this material.
 *
 */
#include "stdincl.h"
#include "global.h"

int32                 LoggedOn_i;					// boolean for logged-in
int8	              HostName_s[129];				// VAX gateway name
int8                  SocketName_s[21];				// NT Socket name
// FILE					*LogFile_px;                // name of file used for logging
// int32				Logging_i = FALSE;         // is logging turned

// trading_code_t UserCode_x;						// returned user code






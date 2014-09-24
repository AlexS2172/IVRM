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


Title:	stdincl.h
Author: Neil Cossor
Description:

    just an include file, to call in the common include list
    that all modules.

Revision history :-

yymmdd	vers'n	author	    notes
------	------	------	    -----
000122	v0.01		    mod to be source-portable between NT and Solaris.. thanks Joshua P.
000204	v0.02		    more Solaris stuff.. again, thanks Joshua P.
000210	v0.03		    slight change to Solaris stuff.. thanks Joseph
000213	v0.04		    remove ISE_COND_<X> macros.. unused.
			    Add string.h and stdlib.h ref for Solaris
			    fix #define of ISE_THREAD_CREATE in Solaris (remove erroneous space)
			    add #define of SWAP for Solaris.. required for big-endian conversions
			    add pragma pack directives, to avoid alignment issues (hopefully)

*/
#ifndef STDINCL_H
#define STDINCL_H

// standard C header files (OS and h/w independent)
#include <stdio.h>
#include <time.h>
#include <sys/timeb.h>
#include <limits.h>
#include <assert.h>

// standard C Windows header files, and thread stuff
#ifdef WIN32
    // NT NT NT NT NT NT NT NT NT NT NT NT NT NT 
    #include <windows.h>
    #include <process.h>			// nt thread stuff.. must be multithread compile

    // thread stuff
    #define ISE_THREAD_CREATE(fn,arg) _beginthread (fn, 0, arg)

    // mutex stuff, for interthread sync
    typedef HANDLE ise_mutex_t;
    #define ISE_MUTEX_INIT(m) m=CreateMutex(0,0,0)
    #define ISE_MUTEX_DESTROY(m) CloseHandle(m); m=0
    #define ISE_MUTEX_LOCK(m) WaitForSingleObject(m,INFINITE)
    #define ISE_MUTEX_UNLOCK(m) ReleaseMutex(m)

    // time stuff
    typedef struct _timeb Timeb_t;
    #define ftime _ftime


    // NT NT NT NT NT NT NT NT NT NT NT NT NT NT 
#else
    // Solaris Solaris Solaris Solaris Solaris 
    #include <pthread.h>
    #include <string.h>
    #include <stdlib.h>

    // thread stuff
    static pthread_t DummyTID;	// wasteful to be static global; oh well
    #define ISE_THREAD_CREATE(fn, arg) pthread_create (&DummyTID, 0, (void*(*)(void*))fn, arg)

    // mutex stuff, for interthread sync
    typedef pthread_mutex_t ise_mutex_t;
    #define ISE_MUTEX_INIT(m) pthread_mutex_init(&m,0)
    #define ISE_MUTEX_DESTROY(m) pthread_mutex_destroy(&m)
    #define ISE_MUTEX_LOCK(m) pthread_mutex_lock(&m)
    #define ISE_MUTEX_UNLOCK(m) pthread_mutex_unlock(&m)

    // time stuff
    typedef struct timeb Timeb_t;
    #define Sleep(ms) poll (0,0,ms)

    // enforce byte-swapping (OM uses little endian, Sparc-Solaris is big-endian)
    #define SWAP 1

    // Solaris Solaris Solaris Solaris Solaris 
#endif

// source code feature config
#if defined (__GNUC__)
    #define LONGLONG 1
#endif

// OM header files
// first off, force 1-byte alignment - avoid problems with invisible padding chars
#pragma pack (1)

#include "omni.h"
#include "omniapi.h"
#include "omnifact.h"
#include "omex_usa.h"

// restore structure packing alignment to whatever is optimal
#pragma pack()

// ISE header files
#include "const.h"
#include "global.h"

#endif

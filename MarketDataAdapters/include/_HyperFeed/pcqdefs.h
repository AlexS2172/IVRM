/*
 *		pcqdefs.h		Date = 930526
 *
 *		System dependant defines and structures
 *
 *
 *		written 5/12/93	rmh
 *
 *    modified 5/26/93  rmh
 *       better NT support
 *
 */

/*=====================================================================*/

#ifdef INCL_AGAIN
	#undef INCL_ETI
	#undef ETI
	#undef _PCQ_INCL_DEFS
	#undef INCL_DBAX
	#undef __UDJNDV_H
	#undef __PCQT1111__
	#undef __STRUCS_1111__
	#undef __MGUIDE_H
	#undef __INCL_DBHEADER
	#undef INCL_PCQT1400H
	#undef INCL_1400RECORD
	#undef	__DBAX__
	#undef __INCL_DBA_CREATE
	#ifdef __INCL_PCQUDEV
			#undef __INCL_PCQUDEV
	#endif
	#ifdef __INCL_PCQPRIV
			#undef __INCL_PCQPRIV
	#endif
	#ifdef _INCL_PCQUOTEX
			#undef _INCL_PCQUOTEX
			#undef _EDSKQQ_DEF
	#endif
	#ifdef INCL_AUTHFILE
			#undef INCL_AUTHFILE
	#endif
	#ifdef _INCL_TEXTP
			#undef _INCL_TEXTP
	#endif
	#ifdef _INCL_ENT_UDEV
		   #undef _INCL_ENT_UDEV
		   #undef _INCL_ENT_DATA
	#endif
	#undef INCL_AGAIN
#endif

// only once 
#ifndef _PCQ_INCL_DEFS									
	#define _PCQ_INCL_DEFS

	#if !defined(_MSC_VER) && !defined(__IBMC__) && !defined(__IBMCPP__)
		#define INCL_NOPACKING
	#endif

	// C++ compiler is being used
	#ifdef __cplusplus
		#ifndef __unix
		extern "C" 
		{
	#endif
#endif
			
#ifdef _MSC_VER
			
#if (_MSC_VER == 600)
#ifndef __PCQWIN
#ifndef __OS2__
#define __OS2__
#endif
#endif
#endif
			
			// must be OS/2.
#ifdef _MT 											
			
#ifndef __OS2__
			//				#define __OS2__
#endif
			
#endif
			
#if (_MSC_VER == 800)
			
#ifndef _M_I86
			
#ifndef __NT__
#define __NT__
#endif
			
#ifdef __OS2__
#undef __OS2__
#endif
			
#endif
#endif
#endif
			
#ifdef __BORLANDC__
			
#ifndef BOOL
#define BOOL int
#endif
			
			//      #define FAR			far
#define far16 		FAR
#define FAR16 		far16
#define SEG16
			//		#define CDECL		cdecl
#define SEGPTR		FAR
#define ENTRY 		short _loadds cdecl FAR
#define LOADDS
			
#endif
			
			// this is an OS/2 compiler
			// OS/2 has already been included
#ifdef __OS2__											
#ifndef OS2_INCLUDED 							
#ifndef INCL_DOS
#define INCL_DOS
#endif
#ifndef(_WIN32_WCE)
#include <os2.h>
#endif
#endif
			
#if defined(__IBMC__) || defined(__IBMCPP__)
#define far16		_Far16
#define SEG16		_Seg16
#define FAR
#define CDECL		_Cdecl
#define LOADDS
#define SEGPTR		
#endif
			
#ifdef _MSC_VER
#define far16 		_far
#define SEG16
#define CDECL		_cdecl
#define LOADDS 		_loadds
#define SEGPTR		_far
#define ENTRY 		short _loadds _cdecl far
#define PASCAL16	_pascal
#endif
			
#define FAR16 far16
			
#ifndef LONGLONG
			typedef long LONGLONG;
#endif
			
#ifndef ULONGLONG
			typedef unsigned long ULONGLONG;
#endif
			
			typedef unsigned long DWORD;
			
#elif defined _MSC_VER         
#include <windows.h>
			
#ifndef PASCAL
#define PASCAL 	pascal
#endif
			
#ifndef PASCAL16
#define PASCAL16	pascal
#endif
			
#ifndef FAR
#define FAR		_far
#endif
			
#ifndef __export
#define __export	_export
#endif
			
#ifndef PID
#define PID 	unsigned short
#endif
			
#if (_MSC_VER > 800)
#ifndef _ncalloc
			/****** C runtime porting macros ****************************************/
			
#define _ncalloc    calloc
#define _nexpand    _expand
#define _ffree      free
#define _fmalloc    malloc
#define _fmemccpy   _memccpy
#define _fmemchr    memchr
#define _fmemcmp    memcmp
#define _fmemcpy    memcpy
#define _fmemicmp   _memicmp
#define _fmemmove   memmove
#define _fmemset    memset
#define _fmsize     _msize
#define _frealloc   realloc
#define _fstrcat    strcat
#define _fstrchr    strchr
#define _fstrcmp    strcmp
#define _fstrcpy    strcpy
#define _fstrcspn   strcspn
#define _fstrdup    _strdup
#define _fstricmp   _stricmp
#define _fstrlen    strlen
#define _fstrlwr    _strlwr
#define _fstrncat   strncat
#define _fstrncmp   strncmp
#define _fstrncpy   strncpy
#define _fstrnicmp  _strnicmp
#define _fstrnset   _strnset
#define _fstrpbrk   strpbrk
#define _fstrrchr   strrchr
#define _fstrrev    _strrev
#define _fstrset    _strset
#define _fstrspn    strspn
#define _fstrstr    strstr
#define _fstrtok    strtok
#define _fstrupr    _strupr
#define _nfree      free
#define _nmalloc    malloc
#define _nmsize     _msize
#define _nrealloc   realloc
#define _nstrdup    _strdup
#define hmemcpy     memcpy
#endif
			
#ifndef DLLEXPORT
#define DLLEXPORT __declspec(dllexport)
#endif
			
#ifndef PCHAR
			typedef char FAR * PCHAR;
#endif
#define far16 
#define SEG16		
#define SEGPTR		
#define FAR16 
#define LOADDS
#define HMTX HANDLE
#define HEV  HANDLE
#else
#ifndef PCHAR
			typedef char FAR * PCHAR;
#endif
#define far16 		_far
#define SEG16
#define CDECL		_cdecl
#define LOADDS 		_loadds
#define SEGPTR		_far
#define FAR16 		far16
#define ENTRY 		short _loadds _cdecl far
#endif
#endif
			
			// These are defined only in OS/2 so add them for other platforms
#ifndef OS2_INCLUDED
			
#ifndef PASCAL
#define PASCAL
#define PASCAL16   PASCAL
#define _Pascal    PASCAL
#endif
			
#ifndef CDECL
#define CDECL
#endif
			
#ifndef FAR
#define FAR
#define far16 
#define FAR16
#define far
#endif
			
#ifndef CHAR
			typedef char CHAR;
			typedef char FAR * PCHAR;
#endif
			
#ifndef UCHAR
			typedef unsigned char UCHAR;
			typedef unsigned char FAR * PUCHAR;
#endif
			
#ifndef BYTE
			typedef unsigned char BYTE;
			typedef unsigned char FAR * PBYTE;
#endif
			
#ifndef INT
			typedef int INT;
#ifndef __BORLANDC__
			typedef int FAR * PINT; 
#endif
#endif
			
#ifndef UINT
			typedef unsigned int UINT;
			typedef unsigned int FAR * PUINT;
#endif
			
#ifndef SHORT
			typedef short SHORT;
			typedef short FAR * PSHORT;
#endif
			
#ifndef USHORT
			typedef unsigned short USHORT;
			typedef unsigned short FAR * PUSHORT;
#endif
			
#ifndef LONG
			typedef long LONG;
#ifndef __BORLANDC__
			typedef long FAR * PLONG;
#endif
#endif
			
#ifndef ULONG
			typedef unsigned long ULONG;
			typedef unsigned long FAR * PULONG;
#endif
			
#ifndef DWORD
			typedef unsigned long DWORD;
#endif
			
#ifndef VOID
			typedef void VOID;
			typedef void FAR * PVOID;
#endif
			
#ifndef BOOL
			typedef int BOOL;
#endif
			
#ifndef PSZ
			typedef char far *PSZ;
#endif
			
#ifndef TRUE
#define TRUE 1
#define FALSE 0
#endif
			
#ifndef _MSC_VER
#ifndef HANDLE
#define HANDLE     UINT
#endif
#ifndef HMODULE
#define HMODULE	   ULONG
#endif
#endif
			
#ifndef HMTX
#define HMTX       HANDLE
#endif
			
#ifndef HANDLE
#define HEV        HANDLE
#endif
			
#ifndef HFILE
#define HFILE      INT
#endif
			
#ifndef HSEM
#define HSEM	   ULONG
#endif
			
#ifndef SOCKET
#define SOCKET     UINT
#endif
			
#ifndef PID
#define PID 	   USHORT
#endif
			
#ifndef SEG16
#define SEG16		
#endif
			
#ifndef SEGPTR
#define SEGPTR		
#endif
			
#ifndef LOADDS
#define LOADDS
#endif
			
			
#ifndef DOSFSRSEM
			// fast save ram semaphore
			typedef struct _DOSFSRSEM 
			{	            
				USHORT	cb;
				USHORT	pid;
				USHORT	tid;
				USHORT	cUsage;
				USHORT	client;
				ULONG	   sem;
			} DOSFSRSEM;
#endif
			
/* Extract high and low order parts of 16 and 32 bit quantity */
			
#ifndef	LOUCHAR
#define LOUCHAR(w)	((UCHAR)(USHORT)(w))
#define HIUCHAR(w)	((UCHAR)(((USHORT)(w) >> 8) & 0xff))
#define LOUSHORT(l)	((USHORT)(ULONG)(l))
#define HIUSHORT(l)	((USHORT)(((ULONG)(l) >> 16) & 0xffff))
			
#ifndef __BORLANDC__
#if (_MSC_VER < 900)
#define LOBYTE(w)	LOUCHAR(w)
#define HIBYTE(w)	HIUCHAR(w)
#endif
#endif
#endif
#endif
			
#ifdef __NT__
#define SEG16		
#define SEGPTR		
#define FAR16 
#define LOADDS
#endif
			
			// define bit fields for various compilers
#ifdef INCL_NOPACKING
			typedef UINT BIT_FIELD;
#elif _MSC_VER
			typedef USHORT BIT_FIELD;
#else
			typedef UINT BIT_FIELD;
#endif
			
#ifdef __PCQWIN
#define _PCQENTRY FAR16 PASCAL
#else
#define _PCQENTRY FAR16 CDECL
#endif
			
// Endian definitions.  Test for HARDWARE types here, not
// compiler types.
/*this section was reformatted for compatability with GNU compilers -ksr*/
#if defined(_M_IX86) || defined(vax) || defined(ns32000)  \
	|| defined(sun386) || defined(BIT_ZERO_ON_RIGHT) \
	|| defined(__OS2__) || defined(i386) || defined(ARM) \
	|| defined(_WIN32_WCE)
#define PQTENDIAN_LITTLE
#elif defined(_M_ALPHA) || defined(_M_PPC) || defined(_M_MPPC) \
	|| defined(_M_MRX000) || defined(sel) || defined(pyr) \
	|| defined(mc68000) || defined(sparc) || defined(is68k) \
	|| defined(tahoe) ||  defined(sun) || defined(_POWER) \
	|| defined(RISC6000) || defined(aiws) || defined(_IBMESA) || defined(BIT_ZERO_ON_LEFT)
#define PQTENDIAN_BIG
#endif
			
// C++ compiler is being used
#ifdef __cplusplus
#ifndef __unix		   
		}
		#endif
	#endif

#endif

#ifndef DBA_KEY_DEFINE
#define DBA_KEY_DEFINE

typedef struct _DBA_KEY {
	USHORT	type;
	UCHAR 	symbol[12];
	UCHAR 	countryCode[2];
	UCHAR 	exchangeCode[2];
	UCHAR 	currencyCode[3];
	UCHAR 	reserved;
} DBA_KEY;

typedef struct _DBA_ITEM {
	USHORT	type;
	UCHAR 	symbol[12];
	UCHAR 	countryCode[2];
	UCHAR 	exchangeCode[2];
} DBA_ITEM;

typedef struct _DBA_MMKEY {
	USHORT		type;
	UCHAR 		symbol[12];
	UCHAR 		countryCode[2];
	UCHAR 		exchangeCode[2];
	UCHAR 		currencyCode[3];
	UCHAR 		mmid[4];
	UCHAR 		reserved;
} DBA_MMKEY;

typedef	union  _DBA_MAXKEY {
	DBA_KEY		dbaKey;
	DBA_MMKEY	mmKey;
} DBA_MAXKEY;

typedef struct _TICKER_KEY {
	USHORT	type;
	UCHAR 	symbol[12];
	UCHAR 	countryCode[2];
	UCHAR 	exchangeCode[2];
	UCHAR 	currencyCode[3];
	UCHAR 	reserved;
} TICKER_KEY;

typedef struct _TICKER_ITEM {
	USHORT	type;
	UCHAR 	symbol[12];
	UCHAR 	countryCode[2];
	UCHAR 	exchangeCode[2];
} TICKER_ITEM;

typedef struct _TICKER_MM_KEY {
	USHORT		type;
	UCHAR 		symbol[12];
	UCHAR 		countryCode[2];
	UCHAR 		exchangeCode[2];
	UCHAR 		currencyCode[3];
	UCHAR 		mmid[4];
	UCHAR 		reserved;
} TICKER_MM_KEY;

typedef struct _TICKER_MM_ITEM {
	USHORT		type;
	UCHAR 		symbol[12];
	UCHAR 		countryCode[2];
	UCHAR 		exchangeCode[2];
	UCHAR 		currencyCode[3];
	UCHAR 		mmid[4];
	UCHAR 		reserved;
} TICKER_MM_ITEM;

typedef struct _TICKER_OPTION_KEY {
	USHORT type;
	UCHAR symbol[7];
	UCHAR month;						// Binary 1 - 12 = Call, 13 - 24 = Put
	UCHAR year;							// Binary
	UCHAR strike[3];					// Binary Big Endian
	UCHAR countryCode[2];
	UCHAR exchangeCode[2];
	UCHAR currencyCode[3];
	UCHAR reserved;
} TICKER_OPTION_KEY;

#define	TICKER_FUTURE_KEY TICKER_OPTION_KEY

typedef struct _TICKER_OPTION_ITEM {
	USHORT type;
	UCHAR symbol[7];
	UCHAR month;						// null = spot Binary 1 - 12 = Future, Call, 13 - 24 = Put
	UCHAR year;							// Binary
	UCHAR strike[3];					// Binary Big Endian
	UCHAR countryCode[2];
	UCHAR exchangeCode[2];
} TICKER_OPTION_ITEM;

#define	TICKER_FUTURE_ITEM TICKER_OPTION_ITEM

typedef union _STIC {					// Smart Ticker Instrument Code
	TICKER_KEY			stKey;
	TICKER_MM_KEY		stMKey;
	TICKER_OPTION_KEY   stOKey;
	TICKER_FUTURE_KEY   stFKey;
	TICKER_ITEM			stItem;
	TICKER_MM_ITEM		stMItem;
	TICKER_OPTION_ITEM  stOItem;
	TICKER_FUTURE_ITEM  stFItem;
} STIC;

#endif
/*=====================================================================*/

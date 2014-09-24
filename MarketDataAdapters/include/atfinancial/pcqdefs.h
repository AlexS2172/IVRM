/*
 *		pcqdefs.h		Date=930526
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

#ifndef _PCQ_INCL_DEFS									// only once
	#define _PCQ_INCL_DEFS

   #ifdef __cplusplus									// C++ compiler is being used
      extern "C" {
   #endif

	#ifdef _MSC_VER

		#if (_MSC_VER == 600)
			#ifndef __PCQWIN
				#ifndef __OS2__
					#define __OS2__
				#endif
			#endif
		#endif

		#ifdef _MT 											// must be OS/2.

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

//      #define FAR       far
		#define far16 		FAR
		#define FAR16 		far16
		#define SEG16
//		#define CDECL		cdecl
		#define SEGPTR		FAR
		#define ENTRY 		short _loadds cdecl FAR
		#define LOADDS

   #endif


	#ifdef __OS2__											// this is an OS/2 compiler

		#ifndef OS2_INCLUDED 							// OS/2 has already been included
         #ifndef INCL_DOS
            #define INCL_DOS
         #endif
			#include <os2.h>
		#endif

		#if defined (__IBMC__) || defined (__IBMCPP__)
			
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
			#define LOADDS 	_loadds
			#define SEGPTR		_far
			#define ENTRY 		short _loadds _cdecl far
			#define PASCAL16	_pascal
		#endif

		#define FAR16 far16
		
	#else         
      #include <windows.h>

		#ifdef _MSC_VER
			#ifndef PASCAL
				#define PASCAL 	pascal
			#endif

			#ifndef PASCAL16
				#define PASCAL16	pascal
			#endif

			#ifndef FAR
				#define FAR			_far
			#endif

			#ifndef __export
				#define __export	_export
			#endif
			
			#ifndef PID
				#define PID 	unsigned short
			#endif

		   #if ( _MSC_VER > 800 )
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
				#define LOADDS 	_loadds
				#define SEGPTR		_far
				#define FAR16 		far16
				#define ENTRY 		short _loadds _cdecl far
			#endif

		#endif
		

	#endif
	
	#ifdef _INC_WINDOWS
		#define OS2_INCLUDED

		#ifndef UCHAR
			typedef unsigned char UCHAR;
		#endif

		#ifndef CHAR
			typedef char CHAR;
		#endif

		#ifndef PCHAR
			typedef char FAR * PCHAR;
		#endif

		#ifndef PUCHAR
			typedef unsigned char FAR * PUCHAR;
		#endif

		#ifndef BYTE
			typedef unsigned char BYTE;
		#endif

		#ifndef INT
			typedef int INT;
		#endif

		#ifndef UINT
			typedef unsigned int UINT;
		#endif

		#ifndef SHORT
			typedef short SHORT;
		#endif

		#ifndef PSHORT
			typedef short FAR * PSHORT;
		#endif

		#ifndef USHORT
			typedef unsigned short USHORT;
		#endif

		#ifndef PUSHORT
			typedef unsigned short FAR * PUSHORT;
		#endif

		#ifndef LONG
			typedef long LONG;
		#endif

		#ifndef ULONG
			typedef unsigned long ULONG;
		#endif

		#ifndef PULONG
			typedef unsigned long FAR * PULONG;
		#endif

		#ifndef VOID
			typedef void VOID;
		#endif

		#ifndef PVOID
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
		#endif

		#ifndef FALSE
			#define FALSE 0
		#endif

		#ifndef DOSFSRSEM
			typedef struct _DOSFSRSEM {	            // fast save ram semaphore
				USHORT	cb;
				USHORT	pid;
				USHORT	tid;
				USHORT	cUsage;
				USHORT	client;
				ULONG	   sem;
				} DOSFSRSEM;
		#endif


	#endif

	#ifndef OS2_INCLUDED 							// OS/2 has already been included

		#ifndef CHAR
			typedef char CHAR;
		#endif

		#ifndef PCHAR
			typedef char FAR * PCHAR;
		#endif

		#ifndef UCHAR
			typedef unsigned char UCHAR;
		#endif

		#ifndef PUCHAR
			typedef unsigned char FAR * PUCHAR;
		#endif

		#ifndef BYTE
			typedef unsigned char BYTE;
		#endif

		#ifndef INT
			typedef int INT;
		#endif

		#ifndef PINT
			#ifndef __BORLANDC__
				typedef int FAR * PINT; 
			#endif
		#endif

		#ifndef UINT
			typedef unsigned int UINT;
		#endif

		#ifndef SHORT
			typedef short SHORT;
		#endif

		#ifndef PSHORT
			typedef short FAR * PSHORT;
		#endif

		#ifndef USHORT
			typedef unsigned short USHORT;
		#endif

		#ifndef PUSHORT
			typedef unsigned short FAR * PUSHORT;
		#endif

		#ifndef LONG
			typedef long LONG;
		#endif

		#ifndef PLONG
			#ifndef __BORLANDC__
				typedef long FAR * PLONG;
			#endif
		#endif

		#ifndef ULONG
			typedef unsigned long ULONG;
		#endif

		#ifndef PULONG
			typedef unsigned long FAR * PULONG;
		#endif

		#ifndef VOID
			typedef void VOID;
		#endif

		#ifndef PVOID
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
		#endif

		#ifndef FALSE
			#define FALSE 0
		#endif

		#ifndef DOSFSRSEM
			typedef struct _DOSFSRSEM {	            // fast save ram semaphore
				USHORT	cb;
				USHORT	pid;
				USHORT	tid;
				USHORT	cUsage;
				USHORT	client;
				ULONG	   sem;
				} DOSFSRSEM;
	  #endif

	  /* Extract high and low order parts of 16 and 32 bit quantity */
	  
		#ifndef __BORLANDC__
			#if (_MSC_VER < 900)
	  	  		#define LOBYTE(w)	LOUCHAR(w)
	  	  		#define HIBYTE(w)	HIUCHAR(w)
	  	 	#endif
	  	#endif
	  	
	  #define LOUCHAR(w)	((UCHAR)(USHORT)(w))
	  #define HIUCHAR(w)	((UCHAR)(((USHORT)(w) >> 8) & 0xff))
	  #define LOUSHORT(l)	((USHORT)(ULONG)(l))
	  #define HIUSHORT(l)	((USHORT)(((ULONG)(l) >> 16) & 0xffff))

	#endif

   #ifdef __NT__

		#define SEG16		
		#define SEGPTR		
		#define FAR16 
		#define LOADDS

	#endif


	// define bit fields for various compilers

	#if defined (__IBMC__) || defined (__IBMCPP__)

		typedef UINT BIT_FIELD;

	#endif

	#ifdef _MSC_VER

		typedef USHORT BIT_FIELD;

	#endif

	#ifdef __BORLANDC__

		typedef UINT BIT_FIELD;

	#endif
	
	#ifdef __PCQWIN
   	#define _PCQENTRY FAR16 PASCAL
   #else
		#define _PCQENTRY FAR16 CDECL
   #endif
   
	#if defined (__IBMC__) || defined (__IBMCPP__)

		#ifndef DOSFSRSEM
			typedef struct _DOSFSRSEM {	            // fast save ram semaphore
				USHORT	cb;
				USHORT	pid;
				USHORT	tid;
				USHORT	cUsage;
				USHORT	client;
				ULONG	   sem;
				} DOSFSRSEM;
		#endif

   #endif

	#ifdef __cplusplus									// C++ compiler is being used
		}
   #endif

#endif

/*=====================================================================*/

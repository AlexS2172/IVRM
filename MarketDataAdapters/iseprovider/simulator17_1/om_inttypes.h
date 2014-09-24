#ifndef OM_INTTYPES
#define	OM_INTTYPES

/* determine if OS And compiler is know to support inttypes.h */

#if defined(vms)
/* 7.2 vms includes inttypes.h  */
#define NATIVE_INTYPES
#endif

#if defined(_WIN32)
/* vsc++ 6.0 does NOT contain inttypes.h */
/* vsc++ uses __int64 but not long long */

#endif

#if defined(sun)
/* Solaris at levels higher than 
 * SunOS ussun02 5.6 Generic_105181-15 sun4u sparc SUNW,Ultra-Enterprise
 * cc: WorkShop Compilers 4.2 30 Oct 1996 C 4.2
 * do provide inttypes.h 
 */

#define NATIVE_INTYPES
#endif

#if defined(__osf__)
/*
 * OSF1 gamma.omgroup.com V4.0 878 alpha
 * does NOT provide inttypes.h 
 */
#endif

#if defined(__hpux)
/*
 * HP provides inttypes.h
 * cpp: HP92453-01 A.10.32.03 HP C Preprocessor
 *	ccom: HP92453-01 G.10.32.05 HP C Compiler
 */
#define NATIVE_INTYPES
#endif


#if defined(_AIX)
#define NATIVE_INTYPES
#endif


#if defined(__linux__) && defined(__GNUC__)
/*
 * Linux provides inttypes.h
 * Linux 2.2.18pre21
 *	gcc version 2.95.4 20011006 (Debian prerelease)
 */
#define NATIVE_INTYPES 
#endif


#if defined(USG)
#endif


#if defined(SEQUENT)
#endif


#ifdef NATIVE_INTYPES 
/* If the plattform  and compiler supports the 
 * C9X 'proposed'  standard for interger types, 
 * use the provided inttypes.h
 * otherwise use our own defintions of the same types
 */
#include <inttypes.h> 
#else 
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/*
 * Basic / Extended integer types
 *
 * The following defines the basic fixed-size integer types.
 *
 */
typedef signed char		int8_t;
typedef signed short		int16_t;
typedef signed int		int32_t;
#if defined(_WIN32)&& ( defined(_MSC_VER) || defined(__BORLANDC__) )
typedef	signed __int64	int64_t;
#else
typedef	signed long long	int64_t;
#endif /* WIN32 */
typedef unsigned char		uint8_t;
typedef unsigned short		uint16_t;
typedef unsigned int		uint32_t;
#if defined(_WIN32)&& ( defined(_MSC_VER) || defined(__BORLANDC__) )
typedef	unsigned __int64	uint64_t;
#else
typedef unsigned long long	uint64_t;
#endif /* WIN32 */
#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* else NATIVE_INTTYPES  */

#endif /* OM_INTTYPES */




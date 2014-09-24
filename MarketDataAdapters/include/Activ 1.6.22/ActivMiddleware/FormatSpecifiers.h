/**
 *	@file	FormatSpecifiers.h
 *
 *	@brief	Platform independant c-style printf format specifiers.
 *
 *	$Log: $
 */

#if (!defined ACTIV_FORMAT_SPECIFIERS_H)
#define ACTIV_FORMAT_SPECIFIERS_H

#if (defined ACTIV_PLATFORM_BSD) || defined (ACTIV_PLATFORM_DARWIN) || (defined ACTIV_PLATFORM_LINUX)
#define __SIZE_T_PREFIX "z"
#elif defined (ACTIV_PLATFORM_SOLARIS)
#define __SIZE_T_PREFIX "l" // lowercase 'el'
#elif (defined ACTIV_PLATFORM_WINDOWS)
#if (defined ACTIV_COMPILER_VISUAL_C) || (defined ACTIV_COMPILER_INTEL) || ((defined ACTIV_COMPILER_GCC) && (!defined __CYGWIN__))
// NB when using -mno-cygwin with GCC on Windows, we use MS c runtime, hence I for size_t format specifiers
#define ACTIV_USE_MICROSOFT_RUNTIME
#define __SIZE_T_PREFIX "I" // uppercase 'aye'
#elif (defined ACTIV_COMPILER_GCC)
#define __SIZE_T_PREFIX "z"
#else
#error Configure for this compiler
#endif
#else
#error Configure for this platform
#endif

// size_t printing macros
#define PRIdSIZE		__SIZE_T_PREFIX "d"
#define PRIiSIZE		__SIZE_T_PREFIX "i"
#define PRIoSIZE		__SIZE_T_PREFIX "o"
#define PRIuSIZE		__SIZE_T_PREFIX "u"
#define PRIxSIZE		__SIZE_T_PREFIX "x"
#define PRIXSIZE		__SIZE_T_PREFIX "X"

// size_t scanning macros
#define SCNdSIZE		__SIZE_T_PREFIX "d"
#define SCNiSIZE		__SIZE_T_PREFIX "i"
#define SCNuSIZE		__SIZE_T_PREFIX "u"
#define SCNoSIZE		__SIZE_T_PREFIX "o"
#define SCNxSIZE		__SIZE_T_PREFIX "x"

#if (defined ACTIV_HAS_INTTYPES_H)
#include <inttypes.h>
#else

#if (defined ACTIV_USE_MICROSOFT_RUNTIME)
// NB when using -mno-cygwin with GCC on Windows, we use MS c runtime, hence I64 for 64-bit format specifiers
#define __PRI64_PREFIX	"I64"
#define __PRIPTR_PREFIX
#elif (defined ACTIV_COMPILER_GCC)
#if (ACTIV_PROCESSOR_WORD_SIZE == 64)
#define __PRI64_PREFIX	"l"
#define __PRIPTR_PREFIX	"l"
#elif (ACTIV_PROCESSOR_WORD_SIZE == 32)
#define __PRI64_PREFIX	"ll"
#define __PRIPTR_PREFIX
#else
#error Configure for this word size on GCC
#endif // ACTIV_PROCESSOR_WORD_SIZE
#else
#error Configure for this compiler
#endif

// printing macros
#define PRId8			"d"
#define PRId16			"d"
#define PRId32			"d"
#define PRId64			__PRI64_PREFIX "d"

#define PRIdLEAST8		"d"
#define PRIdLEAST16		"d"
#define PRIdLEAST32		"d"
#define PRIdLEAST64		__PRI64_PREFIX "d"

#define PRIdFAST8		"d"
#define PRIdFAST16		"d"
#define PRIdFAST32		"d"
#define PRIdFAST64		__PRI64_PREFIX "d"

#define PRIi8			"i"
#define PRIi16			"i"
#define PRIi32			"i"
#define PRIi64			__PRI64_PREFIX "i"

#define PRIiLEAST8		"i"
#define PRIiLEAST16		"i"
#define PRIiLEAST32		"i"
#define PRIiLEAST64		__PRI64_PREFIX "i"

#define PRIiFAST8		"i"
#define PRIiFAST16		"i"
#define PRIiFAST32		"i"
#define PRIiFAST64		__PRI64_PREFIX "i"

#define PRIo8			"o"
#define PRIo16			"o"
#define PRIo32			"o"
#define PRIo64			__PRI64_PREFIX "o"

#define PRIoLEAST8		"o"
#define PRIoLEAST16		"o"
#define PRIoLEAST32		"o"
#define PRIoLEAST64		__PRI64_PREFIX "o"

#define PRIoFAST8		"o"
#define PRIoFAST16		"o"
#define PRIoFAST32		"o"
#define PRIoFAST64		__PRI64_PREFIX "o"

#define PRIu8			"u"
#define PRIu16			"u"
#define PRIu32			"u"
#define PRIu64			__PRI64_PREFIX "u"

#define PRIuLEAST8		"u"
#define PRIuLEAST16		"u"
#define PRIuLEAST32		"u"
#define PRIuLEAST64		__PRI64_PREFIX "u"

#define PRIuFAST8		"u"
#define PRIuFAST16		"u"
#define PRIuFAST32		"u"
#define PRIuFAST64		__PRI64_PREFIX "u"

#define PRIx8			"x"
#define PRIx16			"x"
#define PRIx32			"x"
#define PRIx64			__PRI64_PREFIX "x"

#define PRIxLEAST8		"x"
#define PRIxLEAST16		"x"
#define PRIxLEAST32		"x"
#define PRIxLEAST64		__PRI64_PREFIX "x"

#define PRIxFAST8		"x"
#define PRIxFAST16		"x"
#define PRIxFAST32		"x"
#define PRIxFAST64		__PRI64_PREFIX "x"

#define PRIX8			"X"
#define PRIX16			"X"
#define PRIX32			"X"
#define PRIX64			__PRI64_PREFIX "X"

#define PRIXLEAST8		"X"
#define PRIXLEAST16		"X"
#define PRIXLEAST32		"X"
#define PRIXLEAST64		__PRI64_PREFIX "X"

#define PRIXFAST8		"X"
#define PRIXFAST16		"X"
#define PRIXFAST32		"X"
#define PRIXFAST64		__PRI64_PREFIX "X"

// scanning macros
// NB Visual C++ seemingly doesn't support scanning 8 bit integer value.
#define SCNd16			"hd"
#define SCNd32			"d"
#define SCNd64			__PRI64_PREFIX "d"

#define SCNdLEAST16		"hd"
#define SCNdLEAST32		"d"
#define SCNdLEAST64		__PRI64_PREFIX "d"

#define SCNdFAST16		__PRIPTR_PREFIX "d"
#define SCNdFAST32		__PRIPTR_PREFIX "d"
#define SCNdFAST64		__PRI64_PREFIX "d"

#define SCNi16			"hi"
#define SCNi32			"i"
#define SCNi64			__PRI64_PREFIX "i"

#define SCNiLEAST16		"hi"
#define SCNiLEAST32		"i"
#define SCNiLEAST64		__PRI64_PREFIX "i"

#define SCNiFAST16		__PRIPTR_PREFIX "i"
#define SCNiFAST32		__PRIPTR_PREFIX "i"
#define SCNiFAST64		__PRI64_PREFIX "i"

#define SCNu16			"hu"
#define SCNu32			"u"
#define SCNu64			__PRI64_PREFIX "u"

#define SCNuLEAST16		"hu"
#define SCNuLEAST32		"u"
#define SCNuLEAST64		__PRI64_PREFIX "u"

#define SCNuFAST16		__PRIPTR_PREFIX "u"
#define SCNuFAST32		__PRIPTR_PREFIX "u"
#define SCNuFAST64		__PRI64_PREFIX "u"

#define SCNo16			"ho"
#define SCNo32			"o"
#define SCNo64			__PRI64_PREFIX "o"

#define SCNoLEAST16		"ho"
#define SCNoLEAST32		"o"
#define SCNoLEAST64		__PRI64_PREFIX "o"

#define SCNoFAST16		__PRIPTR_PREFIX "o"
#define SCNoFAST32		__PRIPTR_PREFIX "o"
#define SCNoFAST64		__PRI64_PREFIX "o"

#define SCNx16			"hx"
#define SCNx32			"x"
#define SCNx64			__PRI64_PREFIX "x"

#define SCNxLEAST16		"hx"
#define SCNxLEAST32		"x"
#define SCNxLEAST64		__PRI64_PREFIX "x"

#define SCNxFAST16		__PRIPTR_PREFIX "x"
#define SCNxFAST32		__PRIPTR_PREFIX "x"
#define SCNxFAST64		__PRI64_PREFIX "x"

#endif // (defined ACTIV_HAS_INTTYPES_H)

#endif // (!defined ACTIV_FORMAT_SPECIFIERS_H)

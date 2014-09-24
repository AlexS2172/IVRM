/**
 *	@file	Config.h
 *
 *	@brief	Processor, compiler and platform selection header file.
 *
 *	$Log: $
 */

#if (!defined ACTIV_CONFIG_H)
#define ACTIV_CONFIG_H

// select processor
#if (defined __i386__) || (defined __i386) || (defined _M_IX86)
#define ACTIV_PROCESSOR_X86
#define ACTIV_PROCESSOR_NAME		"x86"
#define ACTIV_PROCESSOR_WORD_SIZE	32
#elif (defined __amd64__) || (defined __x86_64__) || (defined _M_AMD64)
#define ACTIV_PROCESSOR_X86_64
#define ACTIV_PROCESSOR_NAME		"x86-64"
#define ACTIV_PROCESSOR_WORD_SIZE	64
#elif (defined __powerpc__) || (defined __ppc__)
#define ACTIV_PROCESSOR_PPC
#define ACTIV_PROCESSOR_NAME		"PowerPC"
#define ACTIV_PROCESSOR_WORD_SIZE	32
#elif (defined __sparcv9) || ((defined __sparc__) && (defined __arch64__))
#define ACTIV_PROCESSOR_SPARC64
#define ACTIV_PROCESSOR_NAME		"Sparc64"
#define ACTIV_PROCESSOR_WORD_SIZE	64
#define ACTIV_REQUIRE_ALIGNED_MEMORY_ACCESS
#elif (defined __sparc) || ((defined __sparc__) && (!defined __arch64__))
#define ACTIV_PROCESSOR_SPARC
#define ACTIV_PROCESSOR_NAME		"Sparc"
#define ACTIV_PROCESSOR_WORD_SIZE	32
#define ACTIV_REQUIRE_ALIGNED_MEMORY_ACCESS
#elif (defined __ia64__) || (defined _M_IA64)
#define ACTIV_PROCESSOR_IA64
#define ACTIV_PROCESSOR_NAME		"Itanium"
#define ACTIV_PROCESSOR_WORD_SIZE	64
#define ACTIV_REQUIRE_ALIGNED_MEMORY_ACCESS
#else
#error Need to configure for this processor
#endif

// select compiler
#if (defined __GNUC__)
#define ACTIV_COMPILER_GCC
#elif (defined __INTEL_COMPILER)
#define ACTIV_COMPILER_INTEL
#elif (defined _MSC_VER)
#define ACTIV_COMPILER_VISUAL_C
#elif (defined __SUNPRO_CC)
#define ACTIV_COMPILER_SUNPRO
#else
#error Need to configure for this compiler
#endif

// select platform

namespace Activ
{

/**
 *	@brief	Enumeration of platforms.
 *
 *	Network constant values, do not reorder or remove entries!
 */
enum PlatformType
{
	PLATFORM_TYPE_UNKNOWN,
	PLATFORM_TYPE_LINUX,
	PLATFORM_TYPE_WINDOWS,
	PLATFORM_TYPE_BSD,
	PLATFORM_TYPE_DARWIN,
	PLATFORM_TYPE_SOLARIS
};

} // namespace Activ

#if (defined linux) || (defined __linux) || (defined __linux__)
#define ACTIV_PLATFORM_LINUX
#elif (defined _WIN64)
#define ACTIV_PLATFORM_WINDOWS	64
#elif (defined _WIN32)
#define ACTIV_PLATFORM_WINDOWS	32
#elif (defined __FreeBSD__)
#define ACTIV_PLATFORM_BSD
#elif (defined __APPLE__)
#define ACTIV_PLATFORM_DARWIN
#elif (defined sun) || (defined __sun)
#define ACTIV_PLATFORM_SOLARIS
#else
#error Need to configure for this platform
#endif

// enable c99 options
#if (!defined __STDC_LIMIT_MACROS)
#define __STDC_LIMIT_MACROS
#endif

#if (!defined __STDC_FORMAT_MACROS)
#define __STDC_FORMAT_MACROS
#endif

#if (!defined ACTIV_COMPILER_SUNPRO) && (!defined __STDC_CONSTANT_MACROS)
#define __STDC_CONSTANT_MACROS
#endif

// compiler specific includes
#if (defined ACTIV_COMPILER_GCC)
#include "Compilers/GCC.h"
#elif (defined ACTIV_COMPILER_INTEL)
#include "Compilers/Intel.h"
#elif (defined ACTIV_COMPILER_VISUAL_C)
#include "Compilers/VisualC.h"
#elif (defined ACTIV_COMPILER_SUNPRO)
#include "Compilers/SunPro.h"
#endif

// platform specific includes
#if (defined ACTIV_PLATFORM_LINUX)
#include "Platforms/Linux.h"
#elif (defined ACTIV_PLATFORM_WINDOWS)
#include "Platforms/Windows.h"
#elif (defined ACTIV_PLATFORM_BSD)
#include "Platforms/Bsd.h"
#elif (defined ACTIV_PLATFORM_DARWIN)
#include "Platforms/Darwin.h"
#elif (defined ACTIV_PLATFORM_SOLARIS)
#include "Platforms/Solaris.h"
#endif

#endif // (!defined ACTIV_CONFIG_H)

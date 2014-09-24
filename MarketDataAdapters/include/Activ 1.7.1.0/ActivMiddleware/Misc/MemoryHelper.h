/**
 *  @file	MemoryHelper.h
 *
 *	@brief	Header file for the memory helper functions.
 *
 *	$Log: $
 */

#if (!defined ACTIV_MISC_MEMORY_HELPER_H)
#define ACTIV_MISC_MEMORY_HELPER_H

#include "ActivMiddleware/Activ.h"

#include <memory.h>

namespace Activ
{

/**
 *	@brief	Get built-in type value from a (potentially) unaligned pointer.
 *
 *	@param	p pointer to copy from.
 *
 *	@return	Value at location p.
 */
template<typename T>
inline T GetValueFromUnalignedPointer(const void *p)
{
#if (defined ACTIV_REQUIRE_ALIGNED_MEMORY_ACCESS)

	T tmp;

	// NB not using MemoryCopy() to allow special compiler optimizations
	::memcpy(&tmp, p, sizeof(T));

	return tmp;

#else

	return *reinterpret_cast<const T *>(p);

#endif
}

// --------------------------------------------------------------------------------------------------------------------------------

/**
 *	@brief	Copy a built-in type value to a (potentially) unaligned pointer.
 *
 *	@param	p pointer to copy to.
 *	@param	value value to copy.
 */
template <typename T>
inline void CopyValueToUnalignedPointer(void *p, const T &value)
{
#if (defined ACTIV_REQUIRE_ALIGNED_MEMORY_ACCESS)

	// NB not using MemoryCopy() to allow special compiler optimizations
	::memcpy(p, &value, sizeof(T));

#else

	*reinterpret_cast<T *>(p) = value;

#endif
}

// --------------------------------------------------------------------------------------------------------------------------------

/**
 *	@brief	Asymmetrical memory swap.
 *
 *	@param	pBuffer pointer to memory to reverse.
 *	@param	bufferSize number of bytes to reverse.
 */
inline void MemoryReverse(void *pBuffer, size_t bufferSize)
{
	for (byte_t *ptr = static_cast<byte_t *>(pBuffer) + bufferSize - 1; static_cast<byte_t *>(pBuffer) < ptr;
		pBuffer = (static_cast<byte_t *>(pBuffer)) + sizeof(byte_t), --ptr)
	{
		*static_cast<byte_t *>(pBuffer) ^= *ptr;
		*ptr ^= *static_cast<byte_t *>(pBuffer);
		*static_cast<byte_t *>(pBuffer) ^= *ptr;
	}
}

// --------------------------------------------------------------------------------------------------------------------------------

/**
 *	@brief	Asymmetrical memory copy.
 *
 *	@param	pDestination pointer to target to copy to.
 *	@param	pSource pointer to source to copy from.
 *	@param	sourceSize number of bytes to copy from @pSource to @pDestination.
 */
inline void MemoryReverseCopy(void *pDestination, const void *pSource, size_t sourceSize)
{
	for (size_t i = 0; i < sourceSize; ++i)
	{
		static_cast<byte_t *>(pDestination)[i] = static_cast<const byte_t *>(pSource)[sourceSize - i - 1];
	}
}

// --------------------------------------------------------------------------------------------------------------------------------

/**
 *	@brief	Fast memory copy.
 *
 *	Note: When @a sourceSize is known at compile time, it will most likely be better to use memcpy() directly as the compiler
 *	can optimize better (see CopyValueToUnalignedPointer(), for instance).
 *
 *	@param	pDestination pointer to target to copy to.
 *	@param	pSource pointer to source to copy from.
 *	@param	sourceSize number of bytes to copy from @pSource to @pDestination.
 */
inline void MemoryCopy(void *pDestination, const void *pSource, size_t sourceSize)
{
// HACK magic numbers where bytewise copy becomes slower than memcpy

#if (defined ACTIV_PROCESSOR_X86)

#if (defined ACTIV_COMPILER_VISUAL_C)
#define ACTIV_MEMORY_HELPER_MEMORY_COPY_LIMIT	10
#elif (defined ACTIV_COMPILER_GCC) || (defined ACTIV_COMPILER_SUNPRO)
#define ACTIV_MEMORY_HELPER_MEMORY_COPY_LIMIT	20
#else
#error Configure for this compiler on x86
#endif // compilers for x86

#elif (defined ACTIV_PROCESSOR_X86_64)

// memcpy is as fast as bytewise with x86-64

#elif (defined ACTIV_PROCESSOR_SPARC) || (defined ACTIV_PROCESSOR_SPARC64)

// use memcpy for sparcs, too

#elif (defined ACTIV_PROCESSOR_IA64)

#if (defined ACTIV_COMPILER_VISUAL_C)
#define ACTIV_MEMORY_HELPER_MEMORY_COPY_LIMIT	14
#elif (defined ACTIV_COMPILER_GCC)
#define ACTIV_MEMORY_HELPER_MEMORY_COPY_LIMIT	5
#else
#error Configure for this compiler on ia64
#endif

#elif (defined ACTIV_PROCESSOR_PPC)

#if (defined ACTIV_COMPILER_GCC)
#define ACTIV_MEMORY_HELPER_MEMORY_COPY_LIMIT	10
#else
#error Configure for this compiler on ppc
#endif

#else
#error Configure for this processor
#endif // processors

#if (defined ACTIV_MEMORY_HELPER_MEMORY_COPY_LIMIT)

	if (sourceSize >= ACTIV_MEMORY_HELPER_MEMORY_COPY_LIMIT)
	{
		::memcpy(pDestination, pSource, sourceSize);
	}
	else
	{
		for (size_t i = 0; i < sourceSize; ++i)
		{
			static_cast<byte_t *>(pDestination)[i] = static_cast<const byte_t *>(pSource)[i];
		}
	}
		
#else

	::memcpy(pDestination, pSource, sourceSize);

#endif
}

} // namespace Activ

#endif // (!defined ACTIV_MISC_MEMORY_HELPER_H)

/**
 *  @file	MemoryReverse.h
 *
 *	@brief	Header file for the memory reverse function(s).
 *
 *	$Log: $
 */

#if !defined (ACTIV_MEMORY_REVERSE_H)
#define ACTIV_MEMORY_REVERSE_H

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
#if defined (ACTIV_REQUIRE_ALIGNED_MEMORY_ACCESS)

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
#if defined (ACTIV_REQUIRE_ALIGNED_MEMORY_ACCESS)

	// NB not using MemoryCopy() to allow special compiler optimizations
	::memcpy(p, &value, sizeof(T));

#else

	*reinterpret_cast<T *>(p) = value;

#endif
}

// --------------------------------------------------------------------------------------------------------------------------------

/**
 *	@brief	Asymmetrical memory swap
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
 *	@brief	Asymmetrical memory copy
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
 *	@brief	Memory copy
 */
inline void MemoryCopy(void *pDestination, const void *pSource, size_t sourceSize)
{
#if defined (ACTIV_COMPILER_VISUAL_C)

	if (sourceSize > 20) // HACK: magic number - loop is quicker under Visual Studio .NET
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

#endif // !defined (ACTIV_MEMORY_REVERSE_H)

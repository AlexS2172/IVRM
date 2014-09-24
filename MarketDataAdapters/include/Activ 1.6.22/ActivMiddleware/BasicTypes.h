/**
 *	@file	BasicTypes.h
 *
 *	@brief	Basic types (as per C99 <stdint.h> standard).
 *
 *	$Log: $
 */

#if !defined(ACTIV_BASIC_TYPES_H)
#define ACTIV_BASIC_TYPES_H

#include "boost/cstdint.hpp"

#include <cmath>

namespace Activ
{

using boost::int8_t;
using boost::int_least8_t;
using boost::int_fast8_t;
using boost::uint8_t;
using boost::uint_least8_t;
using boost::uint_fast8_t;

using boost::int16_t;
using boost::int_least16_t;
using boost::int_fast16_t;
using boost::uint16_t;
using boost::uint_least16_t;
using boost::uint_fast16_t;

using boost::int32_t;
using boost::int_least32_t;
using boost::int_fast32_t;
using boost::uint32_t;
using boost::uint_least32_t;
using boost::uint_fast32_t;

using boost::int64_t;
using boost::int_least64_t;
using boost::int_fast64_t;
using boost::uint64_t;
using boost::uint_least64_t;
using boost::uint_fast64_t;

using boost::intmax_t;
using boost::uintmax_t;

typedef uint8_t byte_t;
typedef uint32_t TimeoutPeriod;

// ---------------------------------------------------------------------------------------------------------------------------------

#if !defined (INT8_MAX)
#define INT8_MAX			0x7F
#endif

#if !defined (INT8_MIN)
#define INT8_MIN			(-(INT8_MAX) - 1)
#endif

#if !defined (INT_LEAST8_MAX)
#define INT_LEAST8_MAX		0x7F
#endif

#if !defined (INT_LEAST8_MIN)
#define INT_LEAST8_MIN		(-(INT8_MAX) - 1)
#endif

#if !defined (INT_FAST8_MAX)
#define INT_FAST8_MAX		0x7F
#endif

#if !defined (INT_FAST8_MIN)
#define INT_FAST8_MIN		(-(INT8_MAX) - 1)
#endif

#if !defined (UINT8_MAX)
#define UINT8_MAX			0xFF
#endif

#if !defined (UINT_LEAST8_MAX)
#define UINT_LEAST8_MAX		0xFF
#endif

#if !defined (UINT_FAST8_MAX)
#define UINT_FAST8_MAX		0xFF
#endif

#if !defined (INT16_MAX)
#define INT16_MAX			0x7FFF
#endif

#if !defined (INT16_MIN)
#define INT16_MIN			(-(INT16_MAX) - 1)
#endif

#if !defined (INT_LEAST16_MAX)
#define INT_LEAST16_MAX		0x7FFF
#endif

#if !defined (INT_LEAST16_MIN)
#define INT_LEAST16_MIN		(-(INT16_MAX) - 1)
#endif

#if !defined (INT_FAST16_MAX)
#define INT_FAST16_MAX		0x7FFF
#endif

#if !defined (INT_FAST16_MIN)
#define INT_FAST16_MIN		(-(INT16_MAX) - 1)
#endif

#if !defined (UINT16_MAX)
#define UINT16_MAX			0xFFFF
#endif

#if !defined (UINT_LEAST16_MAX)
#define UINT_LEAST16_MAX	0xFFFF
#endif

#if !defined (UINT_FAST16_MAX)
#define UINT_FAST16_MAX		0xFFFF
#endif

#if !defined (INT32_MAX)
#define INT32_MAX			0x7FFFFFFF
#endif

#if !defined (INT32_MIN)
#define INT32_MIN			(-(INT32_MAX) - 1)
#endif

#if !defined (INT_LEAST32_MAX)
#define INT_LEAST32_MAX		0x7FFFFFFF
#endif

#if !defined (INT_LEAST32_MIN)
#define INT_LEAST32_MIN		(-(INT32_MAX) - 1)
#endif

#if !defined (INT_FAST32_MAX)
#define INT_FAST32_MAX		0x7FFFFFFF
#endif

#if !defined (INT_FAST32_MIN)
#define INT_FAST32_MIN		(-(INT32_MAX) - 1)
#endif

#if !defined (UINT32_MAX)
#define UINT32_MAX			0xFFFFFFFF
#endif

#if !defined (UINT_LEAST32_MAX)
#define UINT_LEAST32_MAX	0xFFFFFFFF
#endif

#if !defined (UINT_FAST32_MAX)
#define UINT_FAST32_MAX		0xFFFFFFFF
#endif

#if !defined (INT64_MAX)
#define INT64_MAX			INT64_C(0x7FFFFFFFFFFFFFFF)
#endif

#if !defined (INT64_MIN)
#define INT64_MIN			(-(INT64_MAX) - 1)
#endif

#if !defined (INT_LEAST64_MAX)
#define INT_LEAST64_MAX		INT64_C(0x7FFFFFFFFFFFFFFF)
#endif

#if !defined (INT_LEAST64_MIN)
#define INT_LEAST64_MIN		(-(INT64_MAX) - 1)
#endif

#if !defined (INT_FAST64_MAX)
#define INT_FAST64_MAX		INT64_C(0x7FFFFFFFFFFFFFFF)
#endif

#if !defined (INT_FAST64_MIN)
#define INT_FAST64_MIN		(-(INT64_MAX) - 1)
#endif

#if !defined (UINT64_MAX)
#define UINT64_MAX			UINT64_C(0xFFFFFFFFFFFFFFFF)
#endif

#if !defined (UINT_LEAST64_MAX)
#define UINT_LEAST64_MAX	UINT64_C(0xFFFFFFFFFFFFFFFF)
#endif

#if !defined (UINT_FAST64_MAX)
#define UINT_FAST64_MAX		UINT64_C(0xFFFFFFFFFFFFFFFF)
#endif

#if !defined (INTMAX_MAX)
#define INTMAX_MAX			INT64_C(0x7FFFFFFFFFFFFFFF)
#endif

#if !defined (INTMAX_MIN)
#define INTMAX_MIN			(-(INT64_MAX) - 1)
#endif

#if !defined (UINTMAX_MAX)
#define UINTMAX_MAX			UINT64_C(0xFFFFFFFFFFFFFFFF)
#endif

#define BYTE_MAX			UINT8_MAX

#if defined(BYTE_ORDER)
#    if (BYTE_ORDER == LITTLE_ENDIAN)
#        define ACTIV_LITTLE_ENDIAN 0x0123
#        define ACTIV_BYTE_ORDER ACTIV_LITTLE_ENDIAN
#    elif (BYTE_ORDER == BIG_ENDIAN)
#        define ACTIV_BIG_ENDIAN 0x3210
#        define ACTIV_BYTE_ORDER ACTIV_BIG_ENDIAN
#    else
#        error: unknown BYTE_ORDER!
#    endif
#elif defined(__BYTE_ORDER)
#    if (__BYTE_ORDER == __LITTLE_ENDIAN)
#        define ACTIV_LITTLE_ENDIAN 0x0123
#        define ACTIV_BYTE_ORDER ACTIV_LITTLE_ENDIAN
#    elif (__BYTE_ORDER == __BIG_ENDIAN)
#        define ACTIV_BIG_ENDIAN 0x3210
#        define ACTIV_BYTE_ORDER ACTIV_BIG_ENDIAN
#    else
#        error: unknown __BYTE_ORDER!
#    endif
#else
#    if (defined ACTIV_PROCESSOR_X86) || (defined ACTIV_PROCESSOR_X86_64) || (defined ACTIV_PROCESSOR_IA64)
#        define ACTIV_LITTLE_ENDIAN 0x0123
#        define ACTIV_BYTE_ORDER ACTIV_LITTLE_ENDIAN
#    elif (defined ACTIV_PROCESSOR_SPARC) || (defined ACTIV_PROCESSOR_SPARC64) || (defined ACTIV_PROCESSOR_PPC)
#        define ACTIV_BIG_ENDIAN 0x3210
#        define ACTIV_BYTE_ORDER ACTIV_BIG_ENDIAN
#	 else
#		 error Configure for this processor
#    endif
#endif

// ---------------------------------------------------------------------------------------------------------------------------------

/**
 *	@brief	Reverse the order of bytes in an unsigned 16 bit value.
 *
 *	@param	value the value to byte swap.
 *
 *	@return	The byte swapped value.
 */
inline uint16_t ByteSwap(const uint16_t value)
{
	return (((value & 0xff00) >> 8) |
			((value & 0x00ff) << 8));
}

// ---------------------------------------------------------------------------------------------------------------------------------

/**
 *	@brief	Reverse the order of bytes in a signed 16 bit value.
 *
 *	@param	value the value to byte swap.
 *
 *	@return	The byte swapped value.
 */
inline int16_t ByteSwap(const int16_t value)
{
	return ByteSwap(static_cast<uint16_t>(value));
}

// ---------------------------------------------------------------------------------------------------------------------------------

/**
 *	@brief	Reverse the order of bytes in an unsigned 32 bit value.
 *
 *	@param	value the value to byte swap.
 *
 *	@return	The byte swapped value.
 */
inline uint32_t ByteSwap(const uint32_t value)
{
	return (((value & 0xff000000) >> 24) |
			((value & 0x00ff0000) >> 8) |
			((value & 0x0000ff00) << 8) |
			((value & 0x000000ff) << 24));
}

// ---------------------------------------------------------------------------------------------------------------------------------

/**
 *	@brief	Reverse the order of bytes in a signed 32 bit value.
 *
 *	@param	value the value to byte swap.
 *
 *	@return	The byte swapped value.
 */
inline int32_t ByteSwap(const int32_t value)
{
	return ByteSwap(static_cast<uint32_t>(value));
}

// ---------------------------------------------------------------------------------------------------------------------------------

/**
 *	@brief	Reverse the order of bytes in an unsigned 64 bit value.
 *
 *	@param	value the value to byte swap.
 *
 *	@return	The byte swapped value.
 */
inline uint64_t ByteSwap(const uint64_t value)
{
	return ((static_cast<uint64_t>(ByteSwap(static_cast<uint32_t>(value) & UINT32_MAX)) << 32) |
			(ByteSwap(static_cast<uint32_t>((value >> 32) & UINT32_MAX))));
}

// ---------------------------------------------------------------------------------------------------------------------------------

/**
 *	@brief	Reverse the order of bytes in a signed 64 bit value.
 *
 *	@param	value the value to byte swap.
 *
 *	@return	The byte swapped value.
 */
inline int64_t ByteSwap(const int64_t value)
{
	return ByteSwap(static_cast<uint64_t>(value));
}

// ---------------------------------------------------------------------------------------------------------------------------------

/**
 *	@brief	Convert a value to network byte order from local endian.
 *
 *	@param	value the value to convert.
 *
 *	@return	The converted value.
 */
template <typename T>
inline T HostToNetworkOrder(const T value)
{
#if defined (ACTIV_BIG_ENDIAN)
	return value;
#elif defined (ACTIV_LITTLE_ENDIAN)
	return ByteSwap(value);
#else
#error Either ACTIV_BIG_ENDIAN or ACTIV_LITTLE_ENDIAN must be #defined.
#endif
}

// ---------------------------------------------------------------------------------------------------------------------------------

/**
 *	@brief	Convert a value to little endian byte order from local endian.
 *
 *	@param	value the value to convert.
 *
 *	@return	The converted value.
 */
template <typename T>
inline T HostToLittleEndianOrder(const T value)
{
#if defined (ACTIV_BIG_ENDIAN)
	return ByteSwap(value);
#elif defined (ACTIV_LITTLE_ENDIAN)
	return value;
#else
#error Either ACTIV_BIG_ENDIAN or ACTIV_LITTLE_ENDIAN must be #defined.
#endif
}

// ---------------------------------------------------------------------------------------------------------------------------------

/**
 *	@brief	Convert a value from network byte order to local endian.
 *
 *	@param	value the value to convert.
 *
 *	@return	The converted value.
 */
template <typename T>
inline T NetworkToHostOrder(const T value)
{
	return HostToNetworkOrder(value);
}

// ---------------------------------------------------------------------------------------------------------------------------------

/**
 *	@brief	Convert a value from little endian byte order to local endian.
 *
 *	@param	value the value to convert.
 *
 *	@return	The converted value.
 */
template <typename T>
inline T LittleEndianToHostOrder(const T value)
{
	return HostToLittleEndianOrder(value);
}

// ---------------------------------------------------------------------------------------------------------------------------------

/**
 *	@brief	Return the absolute value of the parameter.
 *
 *	@param	val value to return the absolute value of.
 *
 *	@return	Absolute value of \a val.
 */
template<typename T>
inline T GetAbsolute(const T &val)
{
	return (val < 0) ? -val : val;
}

inline double GetAbsolute(const double &val)
{
	return std::fabs(val);
}

} // namespace Activ

#endif // !defined(ACTIV_BASIC_TYPES_H)

/**
 *	@file	IntegerHelper.h
 *
 *	@brief	Miscellaneous helper functions and classes for integer types.
 *
 *	$Log: $
 */

#if !defined (ACTIV_INTEGER_HELPER_H)
#define ACTIV_INTEGER_HELPER_H

#include "boost/integer_traits.hpp"

namespace Activ
{

/**
 *	@brief	Get the maximum value a type can hold.
 *
 *	@param	value.
 *
 *	@return	The maximum value that \a value can hold, given its type.
 */
template <typename T>
inline T GetMaximumValueOfIntegralType(const T &value)
{
	return boost::integer_traits<T>::const_max;
}

// ---------------------------------------------------------------------------------------------------------------------------------

/**
 *	@brief	Get the minimum value a type can hold.
 *
 *	@param	value.
 *
 *	@return	The minimum value that \a value can hold, given its type.
 */
template <typename T>
inline T GetMinimumValueOfIntegralType(const T &value)
{
	return boost::integer_traits<T>::const_min;
}

// ---------------------------------------------------------------------------------------------------------------------------------

/**
 *	@brief	Make an unsigned 64 bit value.
 *
 *	@param	high the high 32 bits of the result.
 *	@param	low the low 32 bits of the result.
 *
 *	@return	The 64 bit value.
 */
template<typename A, typename B>
inline uint64_t MakeUnsigned64BitValue(A high, B low)
{
	return (static_cast<uint64_t>(high) << 32) | static_cast<uint64_t>(low);
}

// ---------------------------------------------------------------------------------------------------------------------------------

/**
 *	@brief	Get the high 32 bits of a 64 bit value.
 *
 *	@return	The high 32 bits.
 */
inline uint32_t GetHigh32BitValue(uint64_t value)
{
	return static_cast<uint32_t>(value >> 32);
}

// ---------------------------------------------------------------------------------------------------------------------------------

/**
 *	@brief	Get the low 32 bits of a 64 bit value.
 *
 *	@return	The low 32 bits.
 */
inline uint32_t GetLow32BitValue(uint64_t value)
{
	return static_cast<uint32_t>(value & UINT32_MAX);
}

} // namespace Activ

#endif // !defined (ACTIV_INTEGER_HELPER_H)

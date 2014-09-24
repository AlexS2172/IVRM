/**
 *	@file	Atomic.h
 *
 *	@brief	Header file for atomic classes.
 *
 *  $Log: $
 */

#if (!defined ACTIV_BASE_ATOMIC_H)
#define ACTIV_BASE_ATOMIC_H

#include "ActivMiddleware/ActivBase/Detail/AtomicDetail.h"

#include <cassert>

namespace Activ
{

/**
 *	@brief	Atomic integer class.
 */
class AtomicInteger
{
public:
	typedef long Type;

	/**
	 *	@brief	Default constructor.
	 */
	AtomicInteger();

	/**
	 *	@brief	Constructor.
	 *
	 *	@param	value initial value of the integer.
	 */
	explicit AtomicInteger(const Type value);

	/**
	 *	@brief	Atomically exchange the value of the integer with a new value.
	 *
	 *	@param	value new value.
	 *
	 *	@param	The original value.
	 */
	Type Exchange(const Type value);

	/**
	 *	@brief	Get the value.
	 *
	 *	@return	The current value.
	 */
	Type Get() const;

	/**
	 *	@brief	Pre-increment operator.
	 *
	 *	@return	The value of the integer after the increment.
	 */
	Type operator++();

	/**
	 *	@brief	Post-increment operator.
	 *
	 *	@return	The value of the integer before the increment.
	 */
	Type operator++(int);

	/**
	 *	@brief	Pre-decrement operator.
	 *
	 *	@return	The value of the integer after the decrement.
	 */
	Type operator--();

	/**
	 *	@brief	Pre-decrement operator.
	 *
	 *	@return	The value of the integer before the decrement.
	 */
	Type operator--(int);

	/**
	 *	@brief	Cast to underlying type operator.
	 */
	operator Type() const;

private:
	volatile Type	m_value;
};

// ---------------------------------------------------------------------------------------------------------------------------------

inline AtomicInteger::AtomicInteger()
{
	(void) Exchange(0);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline AtomicInteger::AtomicInteger(const Type value)
{
	(void) Exchange(value);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline AtomicInteger::Type AtomicInteger::Exchange(const Type value)
{
	return AtomicExchange(&m_value, value);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline AtomicInteger::Type AtomicInteger::Get() const
{
	return m_value;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline AtomicInteger::Type AtomicInteger::operator++()
{
	return AtomicIncrement(&m_value);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline AtomicInteger::Type AtomicInteger::operator++(int)
{
	return ++(*this) - 1;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline AtomicInteger::Type AtomicInteger::operator--()
{
	return AtomicDecrement(&m_value);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline AtomicInteger::Type AtomicInteger::operator--(int)
{
	return --(*this) + 1;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline AtomicInteger::operator AtomicInteger::Type() const
{
	return Get();
}

// ---------------------------------------------------------------------------------------------------------------------------------

/**
 *	@brief	Atomic pointer class.
 *
 *	@param	T type to point to.
 */
template<typename T>
class AtomicPointer
{
public:
	typedef T *Pointer;

	/**
	 *	@brief	Default constructor.
	 */
	AtomicPointer();

	/**
	 *	@brief	Constructor.
	 *
	 *	@param	value initial value of the pointer.
	 */
	explicit AtomicPointer(const Pointer pointer);

	/**
	 *	@brief	Atomically exchange the value of the pointer with a new value.
	 *
	 *	@param	value new value.
	 *
	 *	@param	The original value.
	 */
	Pointer Exchange(const Pointer value);

	/**
	 *	@brief	Set the value.
	 *
	 *	@param	value new value.
	 *
	 *	@return	*this.
	 */
	AtomicPointer &operator=(const Pointer value);

	/**
	 *	@brief	Indirection operator.
	 *
	 *	@return	Underlying pointer.
	 */
	Pointer operator->() const;

	/**
	 *	@brief	Cast to underlying type operator.
	 */
	operator Pointer() const;

private:
	volatile Pointer	m_value;
};

// ---------------------------------------------------------------------------------------------------------------------------------

template<typename T>
inline AtomicPointer<T>::AtomicPointer()
{
	*this = 0;
}

// ---------------------------------------------------------------------------------------------------------------------------------

template<typename T>
inline AtomicPointer<T>::AtomicPointer(const Pointer value)
{
	*this = value;
}

// ---------------------------------------------------------------------------------------------------------------------------------

template<typename T>
inline T *AtomicPointer<T>::Exchange(const Pointer value)
{

#if (defined ACTIV_COMPILER_VISUAL_C)
// silly VC++ problems with __w64
#pragma warning(disable : 4311)
#pragma warning(disable : 4312)
#endif

#if (defined ACTIV_PROCESSOR_X86) || (defined ACTIV_PROCESSOR_PPC) || (defined ACTIV_PROCESSOR_SPARC) || (defined ACTIV_PROCESSOR_SPARC64) || ((defined ACTIV_PROCESSOR_X86_64) && (!defined ACTIV_PLATFORM_WINDOWS))

	assert(sizeof(long) == sizeof(Pointer));
	return reinterpret_cast<Pointer>(AtomicExchange(reinterpret_cast<volatile long *>(&m_value), reinterpret_cast<long>(value)));

#elif ((defined ACTIV_PROCESSOR_X86_64) || (defined ACTIV_PROCESSOR_IA64)) && (defined ACTIV_PLATFORM_WINDOWS)

	// on Windows long is still 32-bit
	assert(sizeof(int64_t) == sizeof(Pointer));
	return reinterpret_cast<Pointer>(AtomicExchange(reinterpret_cast<volatile int64_t *>(&m_value), reinterpret_cast<int64_t>(value)));

#else
#error Configure
#endif

#if (defined ACTIV_COMPILER_VISUAL_C)
#pragma warning(default : 4311)
#pragma warning(default : 4312)
#endif
}

// ---------------------------------------------------------------------------------------------------------------------------------

template<typename T>
inline AtomicPointer<T> &AtomicPointer<T>::operator=(const Pointer value)
{
	(void) Exchange(value);
	return *this;
}

// ---------------------------------------------------------------------------------------------------------------------------------

template<typename T>
inline AtomicPointer<T>::operator T *() const
{
	return m_value;
}

// ---------------------------------------------------------------------------------------------------------------------------------

template<typename T>
inline T *AtomicPointer<T>::operator->() const
{
	return m_value;
}

} // namespace Activ

#endif // (!defined ACTIV_BASE_ATOMIC_H)

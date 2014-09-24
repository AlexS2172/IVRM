/**
 *	@file	AtomicDetail.h
 *
 *	@brief	Header file for implementation details for atomic operations.
 *
 *  $Log: $
 */

#if (!defined ACTIV_BASE_ATOMIC_DETAIL_H)
#define ACTIV_BASE_ATOMIC_DETAIL_H

#include "ActivMiddleware/ActivBase/External.h"

#include <cassert>

namespace Activ
{

#if (defined ACTIV_COMPILER_SUNPRO)

/**
 *	@brief	Interlocked increment by 1 a value and return resulting value.
 *
 *	@param	pAddend pointer to the variable to increment.
 *
 *	@return	The resulting incremented value.
 */
ACTIV_BASE_API long AtomicIncrement(volatile long *pAddend);

/**
 *	@brief	Interlocked decrement by 1 a value and return resulting value.
 *
 *	@param	pAddend pointer to the variable to decrement.
 *
 *	@return	The resulting decremented value.
 */
ACTIV_BASE_API long AtomicDecrement(volatile long *pAddend);

/**
 *	@brief	Interlocked exchange value.
 *
 *	@param	pTarget pointer to the variable to set.
 *	@param	value the value to set.
 *
 *	@return	The original value pointed to by \a pTarget.
 */
ACTIV_BASE_API long AtomicExchange(volatile long *pTarget, const long value);

#else

/**
 *	@brief	Interlocked increment by 1 a value and return resulting value.
 *
 *	@param	pAddend pointer to the variable to increment.
 *
 *	@return	The resulting incremented value.
 */
inline long AtomicIncrement(volatile long *pAddend)
{
#if (defined ACTIV_PLATFORM_WINDOWS)

	return ::InterlockedIncrement(pAddend);

#elif (defined ACTIV_COMPILER_GCC)

	long res;

#if (defined ACTIV_PROCESSOR_X86) || (defined ACTIV_PROCESSOR_X86_64)

	__asm__ volatile
	(
	"		mov		$1, %0		\n"
	"		lock				\n"
	"		xadd	%0, (%1)	\n"
	"		inc		%0			\n"
	: "=&q" (res)
	: "q" (pAddend)
	: "memory"
	);

#elif (defined ACTIV_PROCESSOR_PPC)

	__asm__ volatile
	(

	"1:		lwarx	%0, 0, %1	\n"
	"		addic	%0, %0, 1	\n"
	"		stwcx.	%0, 0, %1	\n"
	"		bne-	1b			\n"
	: "=&r" (res)
	: "r" (pAddend)
	: "memory", "cc"
	);

#elif (defined ACTIV_PROCESSOR_SPARC)

	long value = 0;

    __asm__ volatile
	(
	"		ld			[%2], %1			\n"					// value = *pAddend
	"1:		add			%1, 1, %0			\n"					// res = value + 1
	"		cas			[%2], %1, %0		\n"					// if (*pAddend == value) swap(*pAddend, res) else res = *pAddend
	"		cmp			%0, %1				\n"					// if (res != value) { value = *pAddend; goto 1 }
	"		bne,a,pn	%%icc, 1b			\n"
	"		ld			[%2], %1			\n"
	"		add			%1, 1, %0			\n"					// res = value + 1
	: "=&r" (res)
	: "r" (value), "r" (pAddend)
	: "memory", "cc"
	);

#elif (defined ACTIV_PROCESSOR_SPARC64)

	long value = 0;

    __asm__ volatile
	(
	"		ldx			[%2], %1			\n"					// value = *pAddend
	"1:		add			%1, 1, %0			\n"					// res = value + 1
	"		casx		[%2], %1, %0		\n"					// if (*pAddend == value) swap(*pAddend, res) else res = *pAddend
	"		cmp			%0, %1				\n"					// if (res != value) { value = *pAddend; goto 1 }
	"		bne,a,pn	%%icc, 1b			\n"
	"		ldx			[%2], %1			\n"
	"		add			%1, 1, %0			\n"					// res = value + 1
	: "=&r" (res)
	: "r" (value), "r" (pAddend)
	: "memory", "cc"
	);

#elif (defined ACTIV_PROCESSOR_IA64)

	__asm__ volatile
	(
	"		mf								\n"
	"		fetchadd8.acq	%0 = [%1], 1	\n"
	: "=&r" (res)
	: "r" (pAddend)
	: "memory"
	);
	
	++res;

#else
#error Implement for this processor
#endif

	return res;

#else
#error Implement for this compiler
#endif
}

// ---------------------------------------------------------------------------------------------------------------------------------

/**
 *	@brief	Interlocked decrement by 1 a value and return resulting value.
 *
 *	@param	pAddend pointer to the variable to decrement.
 *
 *	@return	The resulting decremented value.
 */
inline long AtomicDecrement(volatile long *pAddend)
{
#if (defined ACTIV_PLATFORM_WINDOWS)

	return ::InterlockedDecrement(pAddend);

#elif (defined ACTIV_COMPILER_GCC)

	long res;

#if (defined ACTIV_PROCESSOR_X86) || (defined ACTIV_PROCESSOR_X86_64)

	__asm__ volatile
	(
	"		mov		$-1, %0			\n"
	"		lock					\n"
	"		xadd	%0, (%1)		\n"
	"		dec		%0				\n"
	: "=&q" (res)
	: "q" (pAddend)
	: "memory"
	);

#elif (defined ACTIV_PROCESSOR_PPC)

	__asm__ volatile
	(
	"1:		lwarx	%0, 0, %1		\n"
	"		addic	%0, %0, -1		\n"
	"		stwcx.	%0, 0, %1		\n"
	"		bne-	1b				\n"
	: "=&r" (res)
	: "r" (pAddend)
	: "memory", "cc"
	);

#elif (defined ACTIV_PROCESSOR_SPARC)

	long value = 0;

    __asm__ volatile
	(
	"		ld			[%2], %1			\n"					// value = *pAddend
	"1:		sub			%1, 1, %0			\n"					// res = value - 1
	"		cas			[%2], %1, %0		\n"					// if (*pAddend == value) swap(*pAddend, res) else res = *pAddend
	"		cmp			%0, %1				\n"					// if (res != value) { value = *pAddend; goto 1 }
	"		bne,a,pn	%%icc, 1b			\n"
	"		ld			[%2], %1			\n"
	"		sub			%1, 1, %0			\n"					// res = value - 1
	: "=&r" (res)
	: "r" (value), "r" (pAddend)
	: "memory", "cc"
	);

#elif (defined ACTIV_PROCESSOR_SPARC64)

	long value = 0;

    __asm__ volatile
	(
	"		ldx			[%2], %1			\n"					// value = *pAddend
	"1:		sub			%1, 1, %0			\n"					// res = value - 1
	"		casx		[%2], %1, %0		\n"					// if (*pAddend == value) swap(*pAddend, res) else res = *pAddend
	"		cmp			%0, %1				\n"					// if (res != value) { value = *pAddend; goto 1 }
	"		bne,a,pn	%%icc, 1b			\n"
	"		ldx			[%2], %1			\n"
	"		sub			%1, 1, %0			\n"					// res = value - 1
	: "=&r" (res)
	: "r" (value), "r" (pAddend)
	: "memory", "cc"
	);

#elif (defined ACTIV_PROCESSOR_IA64)

	__asm__ volatile
	(
	"		mf								\n"
	"		fetchadd8.acq	%0 = [%1], -1	\n"
	: "=&r" (res)
	: "r" (pAddend)
	: "memory"
	);
	
	--res;

#else
#error Implement for this processor
#endif

	return res;

#else
#error Implement for this compiler
#endif
}

// ---------------------------------------------------------------------------------------------------------------------------------

/**
 *	@brief	Interlocked exchange value of variable.
 *
 *	@param	pTarget pointer to the variable to set.
 *	@param	value the value to set.
 *
 *	@return	The original value pointed to by \a pTarget.
 */
inline long AtomicExchange(volatile long *pTarget, const long value)
{
#if (defined ACTIV_PLATFORM_WINDOWS)

	return ::InterlockedExchange(pTarget, value);

#elif (defined ACTIV_COMPILER_GCC)

	long res;

#if (defined ACTIV_PROCESSOR_X86) || (defined ACTIV_PROCESSOR_X86_64)

	__asm__ volatile
	(
	"		mov			(%1), %0	\n"							// res = *pTarget
	"1:		lock					\n"
	"		cmpxchg		%2, (%1)	\n"							// if (*pTarget == res) *pTarget = value else res = *pTarget
	"		jne			1b			\n"
	: "=&a" (res)
	: "q" (pTarget), "q" (value)
	: "memory", "cc"
	);

#elif (defined ACTIV_PROCESSOR_PPC)

	__asm__ volatile
	(
	"1:		lwarx	%0, 0, %1		\n"
	"		stwcx.	%2, 0, %1		\n"
	"		bne-	1b				\n"
	: "=&r" (res)
	: "r" (pTarget), "r" (value)
	: "memory", "cc"
	);

#elif (defined ACTIV_PROCESSOR_SPARC)

	long tmp;
    
	__asm__ volatile
	(
	"		ld			[%2], %1			\n"					// tmp = *pTarget
	"1:		mov			%3, %0				\n"					// res = value
	"		cas			[%2], %1, %0		\n"					// if (*pTarget == tmp) swap(*pTarget, res) else res = *pAddend
	"		cmp			%0, %1				\n"					// if (res != tmp)
	"		bne,a,pn	%%icc, 1b			\n"					//     tmp = *pTarget; goto 1
	"		ld			[%2], %1			\n"					// executed before the branch (delay slot)
	: "=&r" (res), "=&r" (tmp)
	: "r" (pTarget), "r" (value)
	: "memory", "cc"
	);

#elif (defined ACTIV_PROCESSOR_SPARC64)

	long tmp;

    __asm__ volatile
	(
	"		ldx			[%2], %1			\n"					// tmp = *pTarget
	"1:		mov			%3, %0				\n"					// res = value
	"		casx		[%2], %1, %0		\n"					// if (*pTarget == tmp) swap(*pTarget, res) else res = *pAddend
	"		cmp			%0, %1				\n"					// if (res != tmp)
	"		bne,a,pn	%%icc, 1b			\n"					//     tmp = *pTarget; goto 1
	"		ldx			[%2], %1			\n"					// executed before the branch (delay slot)
	: "=&r" (res), "=&r" (tmp)
	: "r" (pTarget), "r" (value)
	: "memory", "cc"
	);

#elif (defined ACTIV_PROCESSOR_IA64)

	__asm__ volatile
	(
	"		mf								\n"
	"		xchg8		%0 = [%1], %2		\n"
	: "=&r" (res)
	: "r" (pTarget), "r" (value)
	: "memory"
	);
	
#else
#error Implement for this processor
#endif

	return res;

#else
#error Implement for this compiler
#endif
}

// ---------------------------------------------------------------------------------------------------------------------------------

#if ((defined ACTIV_PROCESSOR_X86_64) || (defined ACTIV_PROCESSOR_IA64)) && (defined ACTIV_PLATFORM_WINDOWS)
// NB on Windows 64-bit long is still 32-bit, hence require a 64-bit exchange too

/**
 *	@brief	Interlocked exchange value of variable, 64-bit.
 *
 *	@param	pTarget pointer to the variable to set.
 *	@param	value the value to set.
 *
 *	@return	The original value pointed to by \a pTarget.
 */
inline int64_t AtomicExchange(volatile int64_t *pTarget, const int64_t value)
{
	return ::InterlockedExchange64(pTarget, value);
}

#endif // ((defined ACTIV_PROCESSOR_X86_64) || (defined ACTIV_PROCESSOR_IA64)) && (defined ACTIV_PLATFORM_WINDOWS)

#endif // (!defined ACTIV_COMPILER_SUNPRO)

} // namespace Activ

#endif // (!defined ACTIV_BASE_ATOMIC_DETAIL_H)

/**
 *	@file	Timeout.h
 *
 *	@brief	Header file for the timeout class.
 *
 *	$Log: $
 */

#if (!defined ACTIV_BASE_TIMEOUT_H)
#define ACTIV_BASE_TIMEOUT_H

#include "ActivMiddleware/ActivBase/External.h"

namespace Activ
{

/**
 *	@brief	Timeout handler function object class.
 */
class Timeout
{
public:
	/**
	 *	@brief	Default constructor.
	 *
	 *	@param	maxTimeout the maximum timeout period, in ms.
	 */
	ACTIV_BASE_API Timeout(TimeoutPeriod maxTimeout);

	/**
	 *	@brief	Suspend current thread if any period is left in the timeout.
	 *
	 *	@param	sleepPeriod how long to sleep for, in ms.
	 *
	 *	@retval	true if there is no period left in the timeout
	 *	@retval	false the current thread was suspended
	 */
	ACTIV_BASE_API bool operator()(TimeoutPeriod sleepPeriod = 500);

	/**
	 *	@brief	Return the timeout period left.
	 *
	 *	@return	The amount of timeout left, in ms.
	 */
	ACTIV_BASE_API TimeoutPeriod GetPeriodLeft() const;

	/**
	 *	@brief	Get a Timeout object representing an infinite timeout.
	 *
	 *	@return	Reference to a static Timeout object.
	 */
	static Timeout &GetInfinite();

	/**
	 *	@brief	Get a Timeout object with a zero timeout.
	 *
	 *	@return	Reference to a static Timeout object.
	 */
	static Timeout &GetZero();

private:
	TimeoutPeriod	m_timeout;									///< the amount of timeout period left

	ACTIV_BASE_API static Timeout	m_infinite;					///< timeout object with an infinite period
	ACTIV_BASE_API static Timeout	m_zero;						///< timeout object with a zero period
};

// ---------------------------------------------------------------------------------------------------------------------------------

inline Timeout &Timeout::GetInfinite()
{
	return m_infinite;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline Timeout &Timeout::GetZero()
{
	return m_zero;
}

} // namespace Activ

#endif // (!defined ACTIV_BASE_TIMEOUT_H)

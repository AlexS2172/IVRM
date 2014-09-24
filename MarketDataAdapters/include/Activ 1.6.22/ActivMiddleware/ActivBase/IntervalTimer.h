/**
 *	@file	IntervalTimer.h
 *
 *	@brief	Header file for the interval timer class.
 *
 *	$Log: $
 */

#if !defined(ACTIV_INTERVAL_TIMER_H)
#define ACTIV_INTERVAL_TIMER_H

#include "ActivMiddleware/ActivBase/External.h"
#include "ActivMiddleware/ActivBase/ActivBase.h"

#include "ActivMiddleware/Misc/IntegerHelper.h"

namespace Activ
{

/**
 *	@brief	Interval timer class
 */
class IntervalTimer
{
public:
    /**
     *	@brief	Default constructor.
     */
    ACTIV_BASE_API IntervalTimer();

	/**
     *	@brief	Get the timer frequency.
	 *
	 *	@return	Returns the timer frequency.
     */
	uint64_t GetFrequency() const;

	/**
     *	@brief	Get the current time stamp.
	 *
	 *	@return	Returns the current time stamp.
     */
	ACTIV_BASE_API uint64_t GetTimeStamp() const;

	/**
     *	@brief	Get the current time.
	 *
	 *	@return	Returns the current time, in seconds.
     */
	double GetTime() const;

	/**
     *	@brief	Scale a timestamp to a time.
	 *
	 *	@return	Returns the scaled time stamp.
     */
	double ScaleTimeStamp(const uint64_t time) const;

	/**
	 *	@brief	Get the number of milliseconds that have elapsed since the system was started.
	 *
	 *	@return	Time since system started, in ms.
	 */
	ACTIV_BASE_API static uint32_t GetTickCount();

private:
	uint64_t  m_frequency;										///< System timer frequency
};

// ---------------------------------------------------------------------------------------------------------------------------------

inline uint64_t IntervalTimer::GetFrequency() const
{
	return m_frequency;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline double IntervalTimer::GetTime() const
{
	return ScaleTimeStamp(GetTimeStamp());
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline double IntervalTimer::ScaleTimeStamp(const uint64_t time) const
{
	return static_cast<double>(time) / static_cast<double>(m_frequency);
}

} // namespace Activ

#endif // !defined(ACTIV_INTERVAL_TIMER_H)

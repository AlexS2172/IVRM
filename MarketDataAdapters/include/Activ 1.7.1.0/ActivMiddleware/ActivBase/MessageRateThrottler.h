/**
 *  @file	 MessageRateThrottler.h
 *
 *  @brief   Header file for the message rate throttler class.
 *
 *  $Log: $
 */

#if (!defined ACTIV_BASE_MESSAGE_RATE_THROTTLER_H)
#define ACTIV_BASE_MESSAGE_RATE_THROTTLER_H

#include "ActivMiddleware/ActivBase/External.h"
#include "ActivMiddleware/ActivBase/IntervalTimer.h"

namespace Activ
{

class IMutex;	///< Forward declaration of the IMutex interface class.

/**
 *	@brief	Message rate throttler class.
 */
class MessageRateThrottler
{
public:
	static const size_t	PERIOD_LENGTH_MINIMUM	= 25;		///< The minimum period length.
	static const size_t	PERIOD_LENGTH_MAXIMUM	= 1000;		///< The maximum period length.

	/**
	 *	@brief	Default constructor.
	 *
	 *	@param	messageRate the message rate.
	 *	@param	shouldCreateMutex indicates whether a mutex should be created.
	 */
	ACTIV_BASE_API MessageRateThrottler(const size_t messageRate = 0, const bool shouldCreateMutex = false);

	/**
	 *	@brief	Constructor.
	 *
	 *	@param	messageRate the message rate.
	 *	@param	periodLength the period length (in milliseconds). Needs to be divisible by 1000 and periods per second has to be less than the message rate.
	 *	@param	shouldCreateMutex indicates whether a mutex should be created.
	 *
	 *	@throw	StatusCodeException
	 */
	ACTIV_BASE_API MessageRateThrottler(const size_t messageRate, const size_t periodLength, const bool shouldCreateMutex = false);

	/**
	 *	@brief	Destructor.
	 */
	ACTIV_BASE_API ~MessageRateThrottler();

	/**
	 *	@brief	Initialize.
	 *
	 *	@param	messageRate the message rate.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 */
	ACTIV_BASE_API StatusCode Initialize(const size_t messageRate);

	/**
	 *	@brief	Initialize.
	 *
	 *	@param	messageRate the message rate.
	 *	@param	periodLength the period length (in milliseconds). Needs to be divisible by 1000 and periods per second has to be less than the message rate.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 */
	ACTIV_BASE_API StatusCode Initialize(const size_t messageRate, const size_t periodLength);

	/**
	 *	@brief	Reset.
	 */
	ACTIV_BASE_API void Reset();

	/**
	 *	@brief	Suspend.
	 */
	ACTIV_BASE_API void Suspend();

private:
	static const size_t	MILLISECONDS_PER_SECOND		= 1000;		///< The number of milliseconds in a second.

	const IntervalTimer	m_intervalTimer;			///< The interval timer.
	const uint64_t		m_intervalTimerFrequency;	///< The interval timer frequency.
	IMutex * const		m_pMutex;					///< Optional mutex.

	size_t				m_messageRate;				///< The message rate.
	size_t				m_periodLength;				///< The period length.
	size_t				m_totalPeriods;				///< The total number of periods (in a second).

	size_t				m_nPeriod;					///< The number of this period.
	size_t				m_nMessagesInPeriod;		///< The number of messages in this period.
	size_t				m_maxMessagesInPeriod;		///< The maximum number of messages in this period.
	uint64_t			m_startTimeStamp;			///< The start time stamp.
	uint64_t			m_lastTimeStamp;			///< The last time stamp.
};

} // namespace Activ

#endif // (!defined ACTIV_BASE_MESSAGE_RATE_THROTTLER_H)

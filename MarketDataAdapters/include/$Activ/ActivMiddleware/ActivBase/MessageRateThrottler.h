/**
 *  @file	 MessageRateThrottler.h
 *
 *  @brief   Header file for the message rate throttler class.
 *
 *  $Log: $
 */

#if !defined(ACTIV_MESSAGE_RATE_THROTTLER_H)
#define ACTIV_MESSAGE_RATE_THROTTLER_H

#include "ActivMiddleware/ActivBase/External.h"
#include "ActivMiddleware/ActivBase/IntervalTimer.h"

namespace Activ
{

/**
 *	@brief	Message rate throttler class.
 */
class ACTIV_BASE_API MessageRateThrottler
{
public:
	/**
	 *	@brief	Default constructor.
	 *
	 *	@param	messageRate the message rate.
	 */
	MessageRateThrottler(const size_t messageRate = 0);

	/**
	 *	@brief	Destructor.
	 */
	~MessageRateThrottler();

	/**
	 *	@brief	Initialize.
	 *
	 *	@param	messageRate the message rate.
	 */
	void Initialize(const size_t messageRate);

	/**
	 *	@brief	Reset.
	 */
	void Reset();

	/**
	 *	@brief	Suspend.
	 */
	void Suspend();

private:
	size_t			m_messageRate;		///< The message rate.
	uint64_t		m_nMessages;		///< The number of messages.
	IntervalTimer	m_intervalTimer;	///< The interval timer.
	const uint64_t	m_frequency;		///< The frequency.
	uint64_t		m_startTimeStamp;	///< The start time stamp.
};

} // namespace Activ

#endif // !defined(ACTIV_MESSAGE_RATE_THROTTLER_H)

/**
 *	@file	TimerServiceApi.h
 *
 *	@brief	Header file for the timer service api.
 *
 *	$Log: $
 */

#if !defined (ACTIV_FRAMEWORK_TIMER_SERVICE_API_H)
#define ACTIV_FRAMEWORK_TIMER_SERVICE_API_H

#include "ActivMiddleware/ActivFramework/External.h"
#include "ActivMiddleware/ActivFramework/TimeHelper.h"

namespace Activ
{

class Component;
class Time;
class Date;
class DateTime;

/**
 *	@brief	Api to the agent's timer service.
 */
class TimerServiceApi
{
public:
	/**
	 *	@brief	Set a new timer to occur at a given time.
	 *
	 *	@param	component the component wanting to set the timer.
	 *	@param	time the date/time of the timer, local or utc.
	 *	@param	timerAddress reference to an Address object to receive the address of the new timer.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_TIMEOUT
	 *	@retval	...
	 */
	ACTIV_FRAMEWORK_API static StatusCode SetTimer(const Component &component, const BaseTime &time, Address &timerAddress);

	/**
	 *	@brief	Set a new timer to occur in a given period.
	 *
	 *	Since there is no source time for this call, the OnTimer() callback will pass back a UTC time object.
	 *
	 *	@param	component the component wanting to set the timer.
	 *	@param	duration the duration (in ms) until the timer.
	 *	@param	timerAddress reference to an Address object to receive the address of the new timer.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_TIMEOUT
	 *	@retval	...
	 */
	ACTIV_FRAMEWORK_API static StatusCode SetTimer(const Component &component, const BaseTime::Duration &duration, Address &timerAddress);

	/**
	 *	@brief	Set a new timer to occur at a given time and then at regular intervals.
	 *
	 *	@param	component the component wanting to set the timer.
	 *	@param	time the date/time of the first timer, local or utc.
	 *	@param	duration the duration (in ms) until the next timer.
	 *	@param	timerAddress reference to an Address object to receive the address of the new timer.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
	ACTIV_FRAMEWORK_API static StatusCode SetTimer(const Component &component, const BaseTime &time, const BaseTime::Duration &duration, Address &timerAddress);

	/**
	 *	@brief	Cancel a timer.
	 *
	 *	@param	component the component that set the timer.
	 *	@param	timerAddress reference to an Address to holding the address of a timer. This is cleared down on success.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_PARAMETER timerAddress is not valid
	 *	@retval	...
	 */
	ACTIV_FRAMEWORK_API static StatusCode CancelTimer(const Component &component, Address &timerAddress);

	/**
	 *	@brief	Cancel outstanding timers set by a component.
	 *
	 *	@param	component the component wanting to cancel.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
	ACTIV_FRAMEWORK_API static StatusCode CancelTimers(const Component &component);

	/**
	 *	@brief	Get current date (utc).
	 *
	 *	@param	component.
	 *	@param	date reference to a Date object to set.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
	ACTIV_FRAMEWORK_API static StatusCode GetCurrentUtcDate(const Component &component, Date &date);

	/**
	 *	@brief	Get current time (utc).
	 *
	 *	@param	component.
	 *	@param	time reference to a Time object to set.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
	ACTIV_FRAMEWORK_API static StatusCode GetCurrentUtcTime(const Component &component, Time &time);

	/**
	 *	@brief	Get current date & time (utc).
	 *
	 *	@param	component.
	 *	@param	dateTime reference to a DateTime object to set.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
	ACTIV_FRAMEWORK_API static StatusCode GetCurrentUtcDateTime(const Component &component, DateTime &dateTime);

	/**
	 *	@brief	Get current date (local).
	 *
	 *	@param	component.
	 *	@param	date reference to a Date object to set.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
	ACTIV_FRAMEWORK_API static StatusCode GetCurrentLocalDate(const Component &component, Date &date);

	/**
	 *	@brief	Get current time (local).
	 *
	 *	@param	component.
	 *	@param	time reference to a Time object to set.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
	ACTIV_FRAMEWORK_API static StatusCode GetCurrentLocalTime(const Component &component, Time &time);

	/**
	 *	@brief	Get current date & time (local).
	 *
	 *	@param	component.
	 *	@param	dateTime reference to a DateTime object to set.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
	ACTIV_FRAMEWORK_API static StatusCode GetCurrentLocalDateTime(const Component &component, DateTime &dateTime);

	/**
	 *	@brief	Adjust the system time.
	 *
	 *	May adjust the system time immediately, or gradually, depending on the underlying platform.
	 *	The TimerService may need to be running in a process with root privileges, depending on the platform.
	 *
	 *	@param	component.
	 *	@param	milliseconds milliseconds to adjust by.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
	ACTIV_FRAMEWORK_API static StatusCode AdjustSystemTime(const Component &component, const int32_t milliseconds);
};

} // namespace Activ

#endif // !defined(ACTIV_FRAMEWORK_TIMER_SERVICE_API_H)

/**
 *	@file	IEvent.h
 *
 *	@brief	Header file for the event interface class.
 *
 *	$Log: $
 */

#if (!defined ACTIV_BASE_IEVENT_H)
#define ACTIV_BASE_IEVENT_H

#include "ActivMiddleware/ActivBase/External.h"

namespace Activ
{

/**
 *	@brief	Abstract base event class.
 */
class IEvent
{
public:
	/**
	 *	@brief	Flags for event creation.
	 */
	typedef uint32_t Flags;
	
	static const Flags FLAG_NONE = 0x00000000;
	static const Flags FLAG_MANUAL = 0x00000001;				///< Manual reset event

	/**
     *	@brief	Destructor.
     */
	virtual ~IEvent() = 0;

	/**
     *	@brief	Set the event.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_FAILURE
     */
	virtual StatusCode Set() = 0;

    /**
     *	@brief	Clear the event.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_FAILURE
     */
	virtual StatusCode Clear() = 0;

    /**
     *	@brief	Wait for the event to be signaled.
	 *
	 *	@param	timeout how long to wait for the event, in ms.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_FAILURE
	 *	@retval	STATUS_CODE_TIMEOUT
     */
	virtual StatusCode Wait(const TimeoutPeriod timeout = ACTIV_WAIT_INFINITE) = 0;

	/**
	 *	@brief	Get the underlying handle, if available.
	 *
	 *	@param	handle reference to receive the handle.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_NOT_SUPPORTED the object does not support an exportable handle.
	 */
	virtual StatusCode GetHandle(HANDLE &handle) const = 0;
};

// ---------------------------------------------------------------------------------------------------------------------------------

inline IEvent::~IEvent()
{
}

} // namespace Activ

#endif // (!defined ACTIV_BASE_IEVENT_H)

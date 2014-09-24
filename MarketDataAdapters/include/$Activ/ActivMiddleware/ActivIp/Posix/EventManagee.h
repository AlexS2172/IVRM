/**
 *	@file	Posix/EventManagee.h
 *
 *	@brief	Header file for class managed by EventManager.
 *
 *	$Log: $
 */

#if !defined (ACTIV_IP_POSIX_EVENT_MANAGEE_H)
#define ACTIV_IP_POSIX_EVENT_MANAGEE_H

#include "ActivMiddleware/ActivIp/External.h"

namespace Activ
{

class EventManager;
class IMutex;
	
/**
 *	@brief	EventManager managed class.
 */
class EventManagee
{
public:
	typedef uint32_t Events;									///< Event type
	
	static const Events EVENT_NONE = 0x00000000;
	static const Events EVENT_READ = 0x00000001;				///< Data to read
	static const Events EVENT_WRITE = 0x00000002;				///< Ok to write
	static const Events EVENT_CLOSE = 0x00000004;				///< Close has been requested
	
	/**
	 *	@brief	Constructor.
	 *
	 *	@param	eventManager reference to EventManager to attach to.
	 */
	EventManagee(EventManager &eventManager);

	/**
	 *	@brief	Destructor.
	 */
	virtual ~EventManagee();
	
	/**
	 *	@brief	Add fd to event manager.
	 *
	 *	NB can be called from any thread.
	 *
	 *	@param	events set of events to get notification on initially.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
	StatusCode AddFd(const Events events);
	
	/**
	 *	@brief	Remove fd from event manager.
	 *
	 *	NB this *must* be called from the context of the event manager's thread.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
	 StatusCode RemoveFd();
	
	/**
	 *	@brief	Add events to wait for.
	 *
	 *	NB can be called from any thread.
	 *
	 *	@param	addEvents events to add to the wait set.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
	StatusCode AddEvents(const Events addEvents);
	
protected:
	/**
	 *	@brief	Event processing.
	 *
	 *	@param	events the set of events that have occured.
	 */
	virtual void OnEvents(const Events events);
	
	EventManager &	m_eventManager;								///< Hook back to the event manager
		 
	IMutex * const	m_pMutex;
	SOCKET			m_socket;
	Events			m_events;									///< Current set of events
	
	friend class EventManager;
	friend class PollManager;
	friend class EpollManager;
};

} // namespace Activ

#endif // !defined (ACTIV_IP_POSIX_EVENT_MANAGEE_H)

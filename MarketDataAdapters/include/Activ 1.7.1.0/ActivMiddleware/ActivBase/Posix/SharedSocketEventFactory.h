/**
 *	@file	SharedSocketEventFactory.h
 *
 *	@brief	Header file for the shared socket based event factory class.
 *
 *	$Log: $
 */

#if (!defined ACTIV_BASE_SHARED_SOCKET_EVENT_FACTORY_H)
#define ACTIV_BASE_SHARED_SOCKET_EVENT_FACTORY_H

#include "ActivMiddleware/ActivBase/External.h"
#include "ActivMiddleware/ActivBase/IEvent.h"
#include "ActivMiddleware/ActivBase/Timeout.h"
#include "ActivMiddleware/ActivBase/IMutex.h"

#include <string>

namespace Activ
{

/**
 *	@brief	Shared socket based event factory class.
 */
class SharedSocketEventFactory
{
public:
	/**
     *	@brief	Create an event.
	 *
	 *	@param	name the event name.
	 *	@param	flags creation flags.
	 *	@param	initialState whether the event is initially set upon creation.
	 *
     *	@return	pointer to an object providing an IEvent interface.
	 *
	 *	@throw	Exception
     */
	ACTIV_BASE_API IEvent *Create(const std::string &name, const IEvent::Flags flags = IEvent::FLAG_NONE, const bool initialState = false);
	
	/**
     *	@brief	Open an existing event.
	 *
	 *	@param	name the event name.
	 *	@param	timeout how long to wait for the object to exist, in ms.
	 *
     *	@return	pointer to an object providing an IEvent interface.
	 *
	 *	@throw	Exception
     */
	ACTIV_BASE_API IEvent *Open(const std::string &name, Timeout &timeout = Timeout::GetInfinite());
	
	/**
	 *	@brief	Get instance of factory.
	 *
	 *	@return	Instance of factory.
	 */
	ACTIV_BASE_API static SharedSocketEventFactory &GetInstance();

private:
    /**
     *	@brief	Default constructor.
     */
	SharedSocketEventFactory();
		
	class Thread;

	static SharedSocketEventFactory	m_factory;							///< Singleton shared event factory
	
	std::string				m_socketName;								///< name of the socket handling socket cloning
	IMutex * const			m_pMutex;									///< mutex over m_thread
	std::auto_ptr<Thread>	m_pThread;									///< thread for cloning socket handles into processes
};

} // namespace Activ

#endif // (!defined ACTIV_BASE_SHARED_SOCKET_EVENT_FACTORY_H)

/**
 *	@file	SharedEventFactory.h
 *
 *	@brief	Header file for the shared event factory class.
 *
 *	$Log: $
 */

#if !defined (ACTIV_BASE_SHARED_EVENT_FACTORY_H)
#define ACTIV_BASE_SHARED_EVENT_FACTORY_H

#include "ActivMiddleware/ActivBase/External.h"
#include "ActivMiddleware/ActivBase/IEvent.h"
#include "ActivMiddleware/ActivBase/Timeout.h"

#include <string>

namespace Activ
{

/**
 *	@brief	Shared event factory class.
 */
class ACTIV_BASE_API SharedEventFactory
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
	IEvent *Create(const std::string &name, const IEvent::Flags flags = IEvent::FLAG_NONE, const bool initialState = false);
	
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
	IEvent *Open(const std::string &name, Timeout &timeout = Timeout::GetInfinite());
	
	/**
	 *	@brief	Get instance of factory.
	 *
	 *	@return	Instance of factory.
	 */
	static SharedEventFactory &GetInstance();

private:
    /**
     *	@brief	Default constructor.
     */
	SharedEventFactory();

	static SharedEventFactory	m_factory;							///< Singleton shared event factory
};

} // namespace Activ

#endif // !defined (ACTIV_BASE_SHARED_EVENT_FACTORY_H)

/**
 *	@file	PrivateEventFactory.h
 *
 *	@brief	Header file for the private event factory class.
 *
 *	$Log: $
 */

#if !defined (ACTIV_BASE_PRIVATE_EVENT_FACTORY_H)
#define ACTIV_BASE_PRIVATE_EVENT_FACTORY_H

#include "ActivMiddleware/ActivBase/External.h"
#include "ActivMiddleware/ActivBase/IEvent.h"

namespace Activ
{

/**
 *	@brief	Private event factory class.
 *
 *	A private event cannot be shared across process boundaries.
 */
class PrivateEventFactory
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
     */
	ACTIV_BASE_API IEvent *Create(const IEvent::Flags flags = IEvent::FLAG_NONE, const bool initialState = false);
	
	/**
	 *	@brief	Get instance of factory.
	 *
	 *	@return	Instance of factory.
	 */
	ACTIV_BASE_API static PrivateEventFactory &GetInstance();

private:
    /**
     *	@brief	Default constructor.
     */
	PrivateEventFactory();
		
	static PrivateEventFactory	m_factory;						///< singleton private event factory
};

} // namespace Activ

#endif // !defined (ACTIV_BASE_PRIVATE_EVENT_FACTORY_H)

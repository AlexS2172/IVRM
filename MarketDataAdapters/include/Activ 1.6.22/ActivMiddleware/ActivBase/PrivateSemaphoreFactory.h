/**
 *	@file	PrivateSemaphoreFactory.h
 *
 *	@brief	Header file for the private semaphore factory class.
 *
 *	$Log: $
 */

#if !defined (ACTIV_BASE_PRIVATE_SEMAPHORE_FACTORY_H)
#define ACTIV_BASE_PRIVATE_SEMAPHORE_FACTORY_H

#include "ActivMiddleware/ActivBase/External.h"
#include "ActivMiddleware/ActivBase/ISemaphore.h"

namespace Activ
{

/**
 *	@brief	Private semaphore factory class.
 *
 *	A private semaphore cannot be shared across process boundaries.
 */
class PrivateSemaphoreFactory
{
public:
	/**
	 *	@brief	Create a private semaphore.
	 *
	 *	@return	pointer to an object providing an ISemaphore interface.
	 *
	 *	@throw	Exception
	 */
	ACTIV_BASE_API ISemaphore *Create();
	
	/**
	 *	@brief	Get the instance of the factory object.
	 */
	ACTIV_BASE_API static PrivateSemaphoreFactory &GetInstance();

private:
	/**
	 *	@brief	Default constructor.
	 */
	PrivateSemaphoreFactory();
	
	static PrivateSemaphoreFactory	m_factory;					///< Singleton
};

} // namespace Activ

#endif // !defined (ACTIV_BASE_PRIVATE_SEMAPHORE_FACTORY_H)

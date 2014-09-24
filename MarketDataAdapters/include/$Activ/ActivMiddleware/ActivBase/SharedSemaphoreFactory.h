/**
 *	@file	SharedSemaphoreFactory.h
 *
 *	@brief	Header file for the shared semaphore factory class.
 *
 *	$Log: $
 */

#if !defined (ACTIV_BASE_SHARED_SEMAPHORE_FACTORY_H)
#define ACTIV_BASE_SHARED_SEMAPHORE_FACTORY_H

#include "ActivMiddleware/ActivBase/External.h"
#include "ActivMiddleware/ActivBase/ISemaphore.h"
#include "ActivMiddleware/ActivBase/Timeout.h"

#include <string>

namespace Activ
{

/**
 *	@brief	Shared semaphore factory class.
 */
class ACTIV_BASE_API SharedSemaphoreFactory
{
public:
	/**
	 *	@brief	Create a named shared semaphore.
	 *
	 *	@param	name semaphore name.
	 *
	 *	@return	pointer to an object providing an ISemaphore interface.
	 *
	 *	@throw	Exception
	 */
	ISemaphore *Create(const std::string &name);
	
	/**
	 *	@brief	Open an existing named shared semaphore.
	 *
	 *	@param	pName semaphore name.
	 *	@param	timeout how long to wait for the object to exist, in ms.
	 *
	 *	@return	pointer to an object providing an ISemaphore interface.
	 *
	 *	@throw	Exception
	 */
	ISemaphore *Open(const std::string &name, Timeout &timeout = Timeout::GetInfinite());
	
	/**
	 *	@brief	Get the instance of the factory object.
	 */
	static SharedSemaphoreFactory &GetInstance();

private:
	/**
	 *	@brief	Default constructor.
	 */
	SharedSemaphoreFactory();
	
	static SharedSemaphoreFactory	m_factory;					///< Singleton shared semaphore factory
};

} // namespace Activ

#endif // !defined (ACTIV_BASE_SHARED_SEMAPHORE_FACTORY_H)

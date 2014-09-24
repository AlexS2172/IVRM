/**
 *  @file	PrivateMutexFactory.h
 *
 *	@brief	Header file for the private mutex factory class.
 *
 *	$Log: $
 */

#if (!defined ACTIV_BASE_PRIVATE_MUTEX_FACTORY_H)
#define ACTIV_BASE_PRIVATE_MUTEX_FACTORY_H

#include "ActivMiddleware/ActivBase/IMutex.h"

namespace Activ
{

/**
 *	@brief	Private mutex factory class.
 *
 *  A private mutex cannot be shared across process boundaries.
 */
class PrivateMutexFactory
{
public:
	/**
     *	@brief	Create a mutex.
	 *
	 *	@param	isInitialOwner whether the mutex is initially locked on creation.
	 *
	 *	@return	pointer to an object providing an IMutex interface.
	 *
	 *	@throw	Exception.
     */
	ACTIV_BASE_API IMutex *Create(const bool isInitialOwner = false);

	/**
	 *	@brief	Get the instance of the factory object.
	 */
	ACTIV_BASE_API static PrivateMutexFactory &GetInstance();

private:
    /**
     *	@brief	Destructor.
     */
	~PrivateMutexFactory();

	static PrivateMutexFactory	m_factory;						///< Singleton private mutex factory
};

} // namespace Activ

#endif // (!defined ACTIV_BASE_PRIVATE_MUTEX_FACTORY_H)

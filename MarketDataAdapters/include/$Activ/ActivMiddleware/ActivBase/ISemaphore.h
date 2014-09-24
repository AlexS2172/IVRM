/**
 *	@file	ISemaphore.h
 *
 *	@brief	Header file for the semaphore interface class.
 *
 *	$Log: $
 */

#if !defined (ACTIV_ISEMAPHORE_H)
#define ACTIV_ISEMAPHORE_H

#include "ActivMiddleware/ActivBase/External.h"

namespace Activ
{

/**
 *	@brief	Abstract base semaphore class.
 */
class ACTIV_BASE_API ISemaphore
{
public:
	/**
	 *	@brief	Virtual destructor.
	 */
	virtual ~ISemaphore();

	/**
	 *	@brief	Acquire the semaphore.
	 *
	 *	@param	timeout how long to wait for the semaphore to be signalled, in ms.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_TIMEOUT
	 *	@retval	STATUS_CODE_FAILURE
	 */
	virtual StatusCode Acquire(const TimeoutPeriod timeout = ACTIV_WAIT_INFINITE) = 0;

	/**
	 *	@brief	Release the semaphore.
	 *
	 *	@param	count number of times to release the semaphore.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_FAILURE
	 */
	virtual StatusCode Release(const size_t count = 1) = 0;
};

// ---------------------------------------------------------------------------------------------------------------------------------

inline ISemaphore::~ISemaphore()
{
}

} // namespace Activ

#endif // !defined (ACTIV_ISEMAPHORE_H)

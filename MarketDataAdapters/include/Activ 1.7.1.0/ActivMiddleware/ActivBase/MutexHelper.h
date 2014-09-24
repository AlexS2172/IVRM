/**
 *	@file	MutexHelper.h
 *
 *	@brief	Header file for the mutex helper class.
 *
 *	$Log: $
 */

#if (!defined ACTIV_BASE_MUTEX_HELPER_H)
#define ACTIV_BASE_MUTEX_HELPER_H

#include "ActivMiddleware/ActivBase/IMutex.h"

#include <cassert>

namespace Activ
{

/**
 *	@brief	Mutex helper class.
 *
 *	The MutexHelper class wraps up locking / unlocking a mutex.
 */
class MutexHelper
{
public:
    /**
     *	@brief	Default constructor which obtains the mutex lock.
	 *
	 *	@param	pMutex pointer to the mutex to be locked.
     */
	MutexHelper(const IMutex * const pMutex);

    /**
     *	@brief	Destructor which releases the mutex lock.
     */
	~MutexHelper();

private:
	const IMutex * const	m_pMutex;							///< the mutex object
};

// ---------------------------------------------------------------------------------------------------------------------------------

inline MutexHelper::MutexHelper(const IMutex * const pMutex) :
	m_pMutex(pMutex)
{
	if (0 != m_pMutex)
		m_pMutex->Acquire();
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline MutexHelper::~MutexHelper()
{
	if (0 != m_pMutex)
		m_pMutex->Release();
}

} // namespace Activ

#endif // (!defined ACTIV_BASE_MUTEX_HELPER_H)

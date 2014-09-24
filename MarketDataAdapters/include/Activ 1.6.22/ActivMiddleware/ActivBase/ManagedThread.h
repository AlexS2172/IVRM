/**
 *	@file	ManagedThread.h
 *
 *	@brief	Header file for the managed thread class.
 *
 *	$Log: $
 */

#if !defined (ACTIV_MANAGED_THREAD_H)
#define ACTIV_MANAGED_THREAD_H

#include "ActivMiddleware/ActivBase/Thread.h"

namespace Activ
{

class ThreadManager;

/**
 *	@brief	Thread class managed by ThreadManager.
 */
class ManagedThread : public Thread
{
public:
	/**
	 *	@brief	Set the manager responsible for this thread.
	 *
	 *	@param	pManager pointer to the thread manager object.
	 */
	ACTIV_BASE_API void SetManager(ThreadManager *pManager);

protected:
	/**
     *	@brief	Thread start function used for any special thread initialization.
	 *
	 *	Always call the base class OnStart() when overriding, and check the return value.
	 *
	 *	@return	false to stop the thread executing if initialization fails.
     */
	ACTIV_BASE_API virtual bool OnStart();

	/**
     *	@brief	Thread stop function used for any special thread cleanup.
	 *
	 *	ManagedThread::OnStop() removes the thread from the metered section, so always
	 *	call the base class OnStop() when overriding.
     */
	ACTIV_BASE_API virtual void OnStop();

private:
	ThreadManager	*m_pManager;	///< Pointer to the owning thread manager object.
};

} // namespace Activ

#endif // !defined (ACTIV_MANAGED_THREAD_H)

/**
 *	@file	ThreadManager.h
 *
 *	@brief	Header file for the thread manager class.
 *
 *	$Log: $
 */

#if !defined(ACTIV_THREAD_MANAGER_H)
#define ACTIV_THREAD_MANAGER_H

#include "ActivMiddleware/ActivBase/IMeteredSection.h"

namespace Activ
{

class ManagedThread;

/**
 *	@brief	Thread manager class.
 */
class ACTIV_BASE_API ThreadManager
{
public:
	/**
	 *	@brief	Thread function type.
	 */
	typedef void (ThreadFunction)(void *);

    /**
     *	@brief	Constructor.
	 *
	 *	@param	maxCount max thread count.
	 *
	 *	@throw	Exception
     */
    ThreadManager(const size_t maxCount);

    /**
     *	@brief	Destructor.
     */
	virtual ~ThreadManager();

	/**
     *	@brief	Start a thread.
	 *
	 *	@param	thread reference to a ManagedThread object to run.
	 *	@param	pData function parameter.
	 *	@param	stackSize stack size.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_FAILURE
	 *	@retval	ACTUV_STATUS_CODE_FULL
     */
	StatusCode StartThread(ManagedThread &thread, void *pData = 0, const size_t stackSize = 0);

	/**
     *	@brief	Start a thread
	 *
	 *	@param	pFunction function ptr to call after starting the thread.
	 *	@param	pData function parameter.
	 *	@param	stackSize stack size.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_FAILURE
	 *	@retval	STATUS_CODE_HEAP_EMPTY
	 *	@retval	ACTUV_STATUS_CODE_FULL
     */
	StatusCode StartThread(ThreadFunction *pFunction, void *pData = 0, const size_t stackSize = 0);

    /**
     *	@brief	Wait until all threads have finished.
	 *
	 *	@param	timeout timeout period, in ms.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_FAILURE
	 *	@retval	STATUS_CODE_TIMEOUT
	 *	@retval	STATUS_CODE_NOT_INITIALIZED
     */
	StatusCode WaitOnEmpty(const TimeoutPeriod timeout = ACTIV_WAIT_INFINITE);

	/**
     *	@brief	Get the metered section count.
	 *
	 *	@retval	threadCount the current thread count.
     */
	size_t GetCount() const;

protected:
	/**
     *	@brief	Thread start function used for any special thread initialization.
	 *
	 *	The default OnThreadStart() does nothing.
	 *
	 *	@param	thread the thread being started.
	 *	@param	pData user data as passed to StartThread().
	 *
	 *	@return	false to stop the thread executing if initialization fails.
     */
	virtual bool OnThreadStart(ManagedThread &thread, void *pData);

	/**
     *	@brief	Thread stop function used for any special thread cleanup.
 	 *
	 *	The default OnThreadStop() leaves the metered section. If overriding OnThreadStop(), make sure the base class
	 *	OnThreadStop() is called last - the integrity of the metered section cannot be guaranteed otherwise.
	 *
	 *	@param	thread the thread stopping.
     */
	virtual void OnThreadStop(ManagedThread &thread);

private:
	IMeteredSection * const m_pSection;					///< Used to count threads

	friend class ManagedThread;							///< The managed thread needs access to the manager
};

} // namespace Activ

#endif // !defined(ACTIV_THREAD_MANAGER_H)



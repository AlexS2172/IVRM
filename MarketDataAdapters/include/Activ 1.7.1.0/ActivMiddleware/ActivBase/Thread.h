/**
 *	@file	Thread.h
 *
 *	@brief	Header file for the thread class.
 *
 *	$Log: $
 */

#if (!defined ACTIV_BASE_THREAD_H)
#define ACTIV_BASE_THREAD_H

#include "ActivMiddleware/ActivBase/External.h"

#include "boost/shared_ptr.hpp"

#include <string>
#include <vector>

namespace Activ
{

#if (defined ACTIV_PLATFORM_WINDOWS)
// to stop the friend declarations getting confused about linkage
extern "C" unsigned WINAPI ActivThreadGlobalThreadProc(void *pData);
#elif (defined ACTIV_PLATFORM_POSIX)
extern "C" void *ActivThreadGlobalThreadProc(void *pData);
extern "C" void ActivThreadGlobalCleanupProc(void *pData);
class IEvent;
#else
#error Configure for this platform
#endif

/**
 *	@brief	Thread base class.
 */
class Thread
{
public:
#if (defined ACTIV_PLATFORM_WINDOWS)
	typedef DWORD Id;											///< thread id type for Windows
#elif (defined ACTIV_PLATFORM_POSIX)
	typedef pthread_t Id;										///< thread id type for Posix
#else
#error Configure for this platform
#endif
	
	typedef std::vector<uint16_t> ProcessorList;				///< list of 0-based processor ids

    /**
     *	@brief	Default constructor.
     */
    ACTIV_BASE_API Thread();

    /**
     *	@brief	Destructor.
     */
	ACTIV_BASE_API virtual ~Thread();

	/**
     *	@brief	Start the thread.
	 *
	 *	@param	pData thread parameter.
	 *	@param	stackSize optional stack size.
	 *
	 *	@throw	Exception.
     */
	ACTIV_BASE_API void Start(void *pData = 0, const size_t stackSize = 0);

	/**
	 *	@brief	End the current thread.
	 *
	 *	@param	statusCode the value to return from the thread.
	 */
	ACTIV_BASE_API void End(StatusCode statusCode = STATUS_CODE_SUCCESS);

	/**
	 *	@brief	Wait for the thread to exit.
	 *
	 *	@param	timeoutPeriod the timeout period to wait, in ms.
	 *
	 *	@return	The return value from the thread Process() function.
	 *
	 *	@throw	Exception.
	 */
	ACTIV_BASE_API StatusCode WaitForCompletion(const TimeoutPeriod timeoutPeriod = ACTIV_WAIT_INFINITE) const;

	/**
     *	@brief	Get the thread id of the thread managed by this object.
	 *
     *	@return	Thread id.
     */
	ACTIV_BASE_API Id GetId() const;

	/**
	 *	@brief	Get the current thread id.
	 *
	 *	@return	The id of the calling thread.
	 */
	ACTIV_BASE_API static Id GetCurrentId();
	
	/**
	 *	@brief	Get the priority of the thread managed by this object.
	 *
	 *	@param	priority thread priority.
	 *
	 *	@retval	STATUS_CODE_SUCCESS	
	 *	@retval	STATUS_CODE_FAILURE
	 */
	ACTIV_BASE_API StatusCode GetPriority(uint32_t &priority) const;

	/**
	 *	@brief	Set the priority of the thread managed by this object.
	 *
	 *	@param	priority new thread priority.
	 *
	 *	@retval	STATUS_CODE_SUCCESS	
	 *	@retval	STATUS_CODE_FAILURE
	 */
	ACTIV_BASE_API StatusCode SetPriority(uint32_t priority);

	/**
	 *	@brief	Get the priority of any thread.
	 *
	 *	@param	priority thread priority.
	 *	@param	id id of the thread to get the priority for, default the calling thread.
	 *
	 *	@retval	STATUS_CODE_SUCCESS	
	 *	@retval	...
	 */
	ACTIV_BASE_API static StatusCode GetThreadPriority(uint32_t &priority, const Id id = GetCurrentId());

	/**
	 *	@brief	Set the priority of any thread.
	 *
	 *	@param	priority new thread priority.
	 *	@param	id id of the thread to set the priority for, default the calling thread.
	 *
	 *	@retval	STATUS_CODE_SUCCESS	
	 *	@retval	...
	 */
	ACTIV_BASE_API static StatusCode SetThreadPriority(const uint32_t priority, const Id id = GetCurrentId());

	/**
	 *	@brief	Convert string to thread priority. Valid priority strings are 'Idle', 'Lowest', 'BelowNormal', 'Normal', 
	 *			'AboveNormal', 'Highest' and 'TimeCritical'.
	 *
	 *	@param	str string to convert. Note case is ignored.
	 *	@param	priority converted thread priority.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_FAILURE
	 */
	ACTIV_BASE_API static StatusCode StringToPriority(const std::string &str, uint32_t &priority);

	/**
	 *	@brief	Set the processors a thread can be scheduled on.
	 *
	 *	@param	processorList list of 0-indexed processors this thread can run on. If the list is empty, the
	 *			thread will be set to have no affinity to particular processors.
	 *	@param	id id of the thread to set the affinities for, default the calling thread.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
	ACTIV_BASE_API static StatusCode SetProcessorAffinity(const ProcessorList &processorList, const Id id = GetCurrentId());

	/**
	 *	@brief	Suspend the execution of the current thread for the specified interval.
	 *
	 *	@param	period the period to sleep for, in ms.
	 */
	ACTIV_BASE_API static void Suspend(const TimeoutPeriod period);

	/**
	 *	@brief	Set the name of a thread.
	 *
	 *	Certain platforms may allow the setting of a 'name' for a thread, viewable in debuggers.
	 *
	 *	@param	threadName the name to give the thread.
	 *	@param	id the id of the thread to name (or default is current thread).
	 */
	ACTIV_BASE_API static void SetName(const std::string &threadName, const Id id = GetCurrentId());

protected:
	/**
     *	@brief	Actual thread function (override this for your own processing).
	 *
	 *	@param	pData thread data pointer as passed to Start().
     */
	virtual StatusCode Process(void *pData) = 0;

private:
#if (defined ACTIV_PLATFORM_WINDOWS)
	friend unsigned WINAPI ActivThreadGlobalThreadProc(void *pData);
#elif (defined ACTIV_PLATFORM_POSIX)
	friend void *ActivThreadGlobalThreadProc(void *pData);
	friend void ActivThreadGlobalCleanupProc(void *pData);
#else
#error Configure for this platform
#endif

	void *						m_pData;						///< application defined data pointer
	Id							m_id;							///< id of the created thread

#if (defined ACTIV_PLATFORM_WINDOWS)
	HANDLE						m_handle;						///< handle of the created thread
#elif (defined ACTIV_PLATFORM_POSIX)
	boost::shared_ptr<IEvent>	m_pEvent;						///< cleanup event
#endif
};

} // namespace Activ

#endif // (!defined ACTIV_BASE_THREAD_H)

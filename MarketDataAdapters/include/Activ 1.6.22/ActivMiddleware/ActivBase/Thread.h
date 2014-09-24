/**
 *	@file	Thread.h
 *
 *	@brief	Header file for the thread class.
 *
 *	$Log: $
 */

#if !defined (ACTIV_THREAD_H)
#define ACTIV_THREAD_H

#include "ActivMiddleware/ActivBase/External.h"

namespace Activ
{

/**
 *	@brief	Thread base class.
 */
class Thread
{
public:
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
	 *	@brief	Wait for the thread to exit.
	 *
	 *	@return	The return value from the thread Process() function.
	 *
	 *	@throw	Exception.
	 */
	ACTIV_BASE_API StatusCode WaitForCompletion() const;

	/**
     *	@brief	Get the thread ID.
	 *
     *	@return	Thread id.
     */
	ACTIV_BASE_API ThreadId GetId() const;

	/**
	 *	@brief	Get the current thread priority.
	 *
	 *	@param	priority thread priority.
	 *
	 *	@retval	STATUS_CODE_SUCCESS	
	 *	@retval	STATUS_CODE_FAILURE
	 */
	ACTIV_BASE_API StatusCode GetPriority(uint32_t &priority) const;

	/**
	 *	@brief	Set the current thread priority.
	 *
	 *	@param	priority new thread priority.
	 *
	 *	@retval	STATUS_CODE_SUCCESS	
	 *	@retval	STATUS_CODE_FAILURE
	 */
	ACTIV_BASE_API StatusCode SetPriority(uint32_t priority);

	/**
	 *	@brief	End the current thread.
	 *
	 *	@param	statusCode the value to return from the thread.
	 */
	ACTIV_BASE_API void End(StatusCode statusCode = STATUS_CODE_SUCCESS);

protected:
	/**
     *	@brief	Thread start function used for any special thread initialization.
	 *
	 *	The default OnStart() does nothing. Always call the base class OnStart() when overriding,
	 *	and check the return value.
	 *
	 *	@return	false to stop the thread executing if initialization fails.
     */
	ACTIV_BASE_API virtual bool OnStart();

	/**
     *	@brief	Thread stop function used for any special thread cleanup.
	 *
	 *	Always call the base class OnStop() when overriding.
     */
	ACTIV_BASE_API virtual void OnStop();

	void	*m_pData;							///< Application defined data pointer.

private:
	/**
     *	@brief	Actual thread function (override this for your own processing).
	 *
	 *	@param	pData thread data pointer.
     */
	virtual StatusCode Process(void *pData) = 0;

	/**
     *	@brief	Static method that is started by the system thread start function.
	 *
	 *	@param	pData thread data pointer.
     */
#if defined (ACTIV_PLATFORM_WINDOWS)
	static unsigned WINAPI StaticThreadProc(void *pData);
#else
	static void *StaticThreadProc(void *pData);
#endif

	/**
     *	@brief	Thread member function that calls OnStart(), Process() (the overloaded process method), and OnStop().
	 *
	 *	@return Application defined.
     */
	StatusCode ThreadProc();

	ThreadId m_id;								///< Thread id.

#if defined (ACTIV_PLATFORM_WINDOWS)
	HANDLE	 m_handle;							///< Thread handle for Win32.
#endif // defined (ACTIV_PLATFORM_WINDOWS)
};

} // namespace Activ

#endif // !defined (ACTIV_THREAD_H)

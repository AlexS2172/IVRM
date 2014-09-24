/**
 *	@file	CtrlHandler.h
 *
 *	@brief	Header file for the ctrl handler class.
 *
 *	$Log: $
 */

#if (!defined ACTIV_BASE_CTRL_HANDLER_H)
#define ACTIV_BASE_CTRL_HANDLER_H

#include "ActivMiddleware/ActivBase/External.h"

#if (defined ACTIV_PLATFORM_WINDOWS)
#include "ActivMiddleware/ActivBase/Atomic.h"
#elif (defined ACTIV_PLATFORM_POSIX)
#include "ActivMiddleware/ActivBase/Thread.h"

#include <signal.h>
#else
#error Configure for this platform
#endif

namespace Activ
{

/**
 *	@brief	Ctrl handler class.
 *
 *	Note only one instance of CtrlHandler is allowed on the Windows platform (per process).
 */
class CtrlHandler
{
public:
	/**
	 *	@brief	List of ctrl event types.
	 */
	enum EventType
	{
		CTRL_C,
		CTRL_BREAK,
		CTRL_EXIT_PROCESS,
		CTRL_LOGOFF_USER,
		CTRL_SHUTDOWN_MACHINE
	};

	/**
	 *	@brief	Constructor.
	 *
	 *	@throw	Exception if the handler could not be installed.
	 */
	ACTIV_BASE_API CtrlHandler();

	/**
	 *	@brief	Destructor.
	 */
	ACTIV_BASE_API virtual ~CtrlHandler();

protected:
	/**
	 *	@brief	Callback when a ctrl event occurs.
	 *
	 *	No guarantees are provided about which thread will invoke this callback.
	 *
	 *	@param	eventType the type of ctrl event that has occured.
	 *
	 *	@retval	true if the event has been handled.
	 *	@retval	false if the event has not been handled.
	 */
	ACTIV_BASE_API virtual bool OnEvent(const EventType eventType);

private:
#if (defined ACTIV_PLATFORM_WINDOWS)

	static AtomicPointer<CtrlHandler>	m_pCtrlHandler;			///< can only have one ctrl handler per process
	
	static BOOL WINAPI HandlerFunction(DWORD ctrlType);

#elif (defined ACTIV_PLATFORM_POSIX)
	
	/**
	 *	@brief	Thread to process signals.
	 */
	class Thread : public Activ::Thread
	{
	public:
		Thread(CtrlHandler &ctrlHandler);
		
	private:
		virtual StatusCode Process(void *pData);
		
		CtrlHandler &m_ctrlHandler;
	};

	Thread			m_thread;									///< thread waiting for signals
	volatile bool	m_isShuttingDown;
	sigset_t		m_originalSigSet;
	sigset_t		m_blockSigSet;

#else
#error Configure for this platform
#endif
};

} // namespace Activ

#endif // (!defined ACTIV_BASE_CTRL_HANDLER_H)

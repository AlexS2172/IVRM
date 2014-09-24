/**
 *	@file	CtrlHandler.h
 *
 *	@brief	Header file for the ctrl handler class.
 *
 *	$Log: $
 */

#if !defined (ACTIV_CTRL_HANDLER_H)
#define ACTIV_CTRL_HANDLER_H

#include "ActivMiddleware/ActivBase/External.h"
#include "ActivMiddleware/ActivBase/Atomic.h"

#if defined (ACTIV_PLATFORM_POSIX)
#include <signal.h>
#endif // defined (ACTIV_PLATFORM_POSIX)

namespace Activ
{

/**
 *	@brief	Ctrl handler class.
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
	 *	@brief	Callback when a ctrl event occurs
	 *
	 *	@param	eventType the type of ctrl event that has occured.
	 *
	 *	@retval	true if the event has been handled.
	 *	@retval	false if the event has not been handled.
	 */
	ACTIV_BASE_API virtual bool OnEvent(const EventType eventType);

private:
	static AtomicPointer<CtrlHandler>	m_pCtrlHandler;			///< Can only have one ctrl handler per process

#if defined (ACTIV_PLATFORM_WINDOWS)

	static BOOL WINAPI HandlerFunction(DWORD ctrlType);

#elif defined (ACTIV_PLATFORM_POSIX)

	struct sigaction	m_oldIntSignalAction;					///< Saved previous signal handler to restore to for SIGINT
	struct sigaction	m_oldQuitSignalAction;					///< Saved previous signal handler to restore to for SIGQUIT
	struct sigaction	m_oldTermSignalAction;					///< Saved previous signal handler to restore to for SIGTERM
	struct sigaction	m_oldPowerSignalAction;					///< Saved previous signal handler to restore to for SIGPWR

	static void HandlerFunction(int signalNumber);

#else
#error Implement for this platform
#endif
};

} // namespace Activ

#endif // !defined (ACTIV_CTRL_HANDLER_H)

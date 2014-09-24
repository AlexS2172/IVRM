/**
 *	@file	Process.h
 *
 *	@brief	Header file for the process class.
 *
 *	$Log: $
 */

#if (!defined ACTIV_BASE_PROCESS_H)
#define ACTIV_BASE_PROCESS_H

#include "ActivMiddleware/ActivBase/External.h"

#include <string>

namespace Activ
{

/**
 *	@brief	Process class.
 *
 *	Provides OS-abstractions for common process-level operations.
 */
class Process
{
public:
#if (defined ACTIV_PLATFORM_WINDOWS)
	typedef DWORD Id;											///< process id type for Windows
#elif (defined ACTIV_PLATFORM_POSIX)
	typedef pid_t Id;											///< process id type for Posix
#else
#error Configure for this platform
#endif

	/**
	 *	@brief	Run a command to completion and return any stdout output of the process.
	 *
	 *	@param	command the command (and arguments) to run.
	 *	@param	output any output from @command is append to @output.
	 *
	 *	@return	STATUS_CODE_SUCCESS or an error explaining why the command could not be run.
	 *	@retval	...
	 */
	ACTIV_BASE_API static StatusCode Run(const std::string &command, std::string &output);

	/**
	 *	@brief	Launch a process in the background.
	 *
	 *	@param	command the command (and arguments) to run.
	 *	@param	id reference to an @a Id variable to receive the process id of the new process.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_FAILURE
	 *	@retval	STATUS_CODE_HEAP_EMPTY
	 *	@retval	STATUS_CODE_INVALID_FORMAT @a command is incorrectly formatted
	 *	@retval	STATUS_CODE_NOT_FOUND @a command not found
	 */
	ACTIV_BASE_API static StatusCode Start(const std::string &command, Id &id);

	/**
	 *	@brief	Kill a process.
	 *
	 *	@param	id the process id to kill.
	 *
	 *	@return	STATUS_CODE_SUCCESS or an error explaining why the process could not be killed.
	 */
	ACTIV_BASE_API static StatusCode Kill(const Id id);

	/**
	 * 	@brief	Wait for a process to exit.
	 *
	 *	@param	id the process id to wait for.
	 *	@param	timeoutPeriod the timeout period to wait, in ms.
	 * 
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_TIMEOUT
 	 *	@retval	STATUS_CODE_FAILURE
	 */
	ACTIV_BASE_API static StatusCode WaitForCompletion(const Id id, const TimeoutPeriod timeoutPeriod = ACTIV_WAIT_INFINITE);

	/**
	 *	@brief	Does a process exist?
	 *
	 *	@param	is the process id to check for.
	 *
	 *	@return	true if the process exists.
	 */
	ACTIV_BASE_API static bool DoesExist(const Id id);

	/**
	 *	@brief	Get the current process id.
	 *
	 *	@return	The current running process id.
	 */
	ACTIV_BASE_API static Id GetCurrentId();
};

} // namespace Activ

#endif // (!defined ACTIV_BASE_PROCESS_H)

/**
 *	@file	Shell.h
 *
 *	@brief	Header file for the shell class.
 *
 *	$Log: $
 */

#if (!defined ACTIV_BASE_SHELL_H)
#define ACTIV_BASE_SHELL_H

#include "ActivMiddleware/ActivBase/External.h"

#include <string>

namespace Activ
{

/**
 *	@brief	Command-line abstraction.
 */
class Shell
{
public:
	/**
	 *	@brief	Default constructor.
	 */
	ACTIV_BASE_API Shell();

	/**
	 *	@brief	Destructor.
	 *
	 *	If the shell is still running, it will be terminated.
	 */
	ACTIV_BASE_API virtual ~Shell();

	/**
	 *	@brief	Initialize the shell.
	 *
	 *	@return	STATUS_CODE_SUCCESS or some error code.
	 */
	ACTIV_BASE_API StatusCode Initialize();

	/**
	 *	@brief	Poll for i/o for the shell.
	 *
	 *	Loops endlessly, checking for the shell process to have exited.
	 *
	 *	Calls OnOutputString() when the shell has written some output.
	 *
	 *	Calls OnInputString() to check for text to send to the shell.
	 */
	ACTIV_BASE_API void Run();

protected:
	/**
	 *	@brief	Called when the shell has outputted some text.
	 *
	 *	@param	outputString the text output by the shell.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_EXIT terminate the shell process (causes Run() to return)
	 */
	virtual StatusCode OnOutputString(const std::string &outputString) = 0;

	/**
	 *	@brief	Called during Run() to check for input to send to the shell.
	 *
	 *	An override of this call should ideally block for a while whilst (for example) waiting for
	 *	user input, to stop Run() from busy-looping.
	 *
	 *	@param	inputString to receive the input text.
	 *
	 *	@retval	STATUS_CODE_SUCCESS inputString will be sent to the shell
	 *	@retval	STATUS_CODE_EXIT terminate the shell process (causes Run() to return)
	 */
	virtual StatusCode OnInputString(std::string &intputString) = 0;

private:
#if (defined ACTIV_PLATFORM_WINDOWS)

	PROCESS_INFORMATION	m_processInfo;							///< from CreateProcess()

	HANDLE				m_shellOutputParentHandle;				///< handle to our end of the stdout handle for the shell
	HANDLE				m_shellOutputChildHandle;				///< handle to the shell's end of the stdout handle

	HANDLE				m_shellInputParentHandle;				///< handle to our end of the stdin handle for the shell
	HANDLE				m_shellInputChildHandle;				///< handle to the shell's end of the stdin handle

#endif
};

} // namespace Activ

#endif // (!defined ACTIV_BASE_SHELL_H)

/**
 *	@file	ActivBase.h
 *
 *	@brief	Global functions from ActivBase.
 *
 *  ActivBase provides low-level os independant primatives.
 *
 *	$Log: $
 */

#if !defined (ACTIV_BASE_H)
#define ACTIV_BASE_H

#include "ActivMiddleware/ActivBase/External.h"

#include "boost/format.hpp"

#include <sstream>
#include <iostream>
#include <string>
#include <cstdarg>

/**
 *	@brief	Log types.
 *
 *	These are network constant values.
 */
#define ACTIV_LOG_TYPE_LIST(d)					\
	d(0,	LOG_TYPE_DEBUG,		"Debug")		\
	d(1,	LOG_TYPE_INFO,		"Info")			\
	d(2,	LOG_TYPE_WARNING,	"Warning")		\
	d(3,	LOG_TYPE_ERROR,		"Error")

namespace Activ
{

/**
 *	@brief	Log type.
 */
typedef byte_t LogType;

/**
 *	@brief	Macro to declare constant log type.
 */
#define ACTIV_DECLARE_LOG_TYPE(value, id, name)	const LogType id = value;

/**
 *	@brief	Log type constants.
 */
ACTIV_LOG_TYPE_LIST(ACTIV_DECLARE_LOG_TYPE)

/**
 *	@brief	Type of log message function.
 */
typedef void (LogMessageFunction)(const LogType type, const char *message);

// ---------------------------------------------------------------------------------------------------------------------------------

/**
 *	@brief	Log a string. A newline is appended. By default the output is sent to stdout.
 *
 *	@param	type
 *	@param	message string to log.
 */
ACTIV_BASE_API void LogString(const LogType type, const char *message);

/**
 *	@brief	Log a string. A newline is appended. By default the output is sent to stdout.
 *
 *	@param	type
 *	@param	message string to log.
 */
ACTIV_BASE_API void LogString(const LogType type, const std::string &message);

/**
 *	@brief	Log a boost::format string. A newline is appended. By default the output is sent to stdout.
 *
 *	@param	type
 *	@param	format reference to boost::format object containing the string to log.
 */
ACTIV_BASE_API void LogString(const LogType type, const boost::format &format);

/**
 *	@brief	Log a message. A newline is appended. By default the output is sent to stdout.
 *
 *	@param	type
 *	@param	format pointer to a sprintf-style format string.
 *	@param	... variable arguments for the format string.
 */
ACTIV_BASE_API void LogMessage(const LogType type, const char *format, ...);

/**
 *	@brief	Log a message. A newline is appended. By default the output is sent to stdout.
 *
 *	@param	type
 *	@param	format pointer to a sprintf-style format string.
 *	@param	args variable arguments for the format string.
 */
ACTIV_BASE_API void LogMessageV(const LogType type, const char *format, std::va_list args);

/**
 *	@brief	Return log type as a string.
 *
 *	@param	logType.
 *
 *	@return	String representation of \a logType.
 */
ACTIV_BASE_API std::string LogTypeToString(const LogType logType);

/**
 *	@brief	Get the current global message logger.
 *
 *	@return	Pointer to the current set global log message function.
 */
ACTIV_BASE_API LogMessageFunction *GetLogMessageFunction();

/**
 *	@brief	Set the global message logger.
 *
 *	@param	pFunction pointer to a function of type LogMessageFunction to be used by LogMessage().
 *			If pFunction == 0, the LogMessage() call will do nothing.
 */
ACTIV_BASE_API void SetLogMessageFunction(LogMessageFunction *pFunction);

/**
 *	@brief	Printf style log message function. The output of LogMessage() is sent to stdout.
 *
 *	@param	type
 *	@param	message string to write to stdout.
 */
ACTIV_BASE_API void PrintfLogMessage(const LogType type, const char *message);

/**
 *	@brief	Launch a process.
 *
 *	@param	command the command (& arguments) to run.
 *	@param	osProcessId reference to a @a OsProcessId to receive the new process id.
 *
 *	@retval	STATUS_CODE_SUCCESS
 *	@retval	STATUS_CODE_FAILURE
 *	@retval	STATUS_CODE_HEAP_EMPTY
 */
ACTIV_BASE_API StatusCode StartProcess(const std::string &command, OsProcessId &osProcessId);

/**
 *	@brief	Kill a process.
 *
 *	@param	osProcessId the process id to kill.
 */
ACTIV_BASE_API void KillProcess(const OsProcessId osProcessId);

/**
 *	@brief	Get the current process id.
 *
 *	@return	The current running process id.
 */
ACTIV_BASE_API OsProcessId GetCurrentOsProcessId();

/**
 *	@brief	Get the current thread id.
 *
 *	@return	The current running thread id.
 */
ACTIV_BASE_API ThreadId GetThreadId();

/**
 *	@brief	Set the name of a thread.
 *
 *	@param	threadName the name to give the thread.
 *	@param	threadId the id of the thread to name (or default is current thread).
 */
ACTIV_BASE_API void SetThreadName(const std::string &threadName, const ThreadId threadId = GetThreadId());

/**
 *	@brief	Convert a status code to a string.
 *
 *	@param	statusCode see StatusCodes.h.
 *
 *	@return	the string corresponding to \a statusCode or "unknown error".
 */
ACTIV_BASE_API std::string StatusCodeToString(const StatusCode statusCode);

/**
 *	@brief	Return a string from an os error
 *
 *	@return	A string representation of an os error.
 */
ACTIV_BASE_API std::string OsErrorToString(const uint32_t errorCode);

/**
 *	@brief	Convert os-dependent error code to Activ StatusCode.
 *
 *	@param	errorCode os error code.
 *	@param	defaultStatusCode the Activ StatusCode to return if there is no suitable conversion of \a errorCode.
 *
 *	@return	Activ StatusCode matching the os error code, or \a defaultStatusCode if there is no mapping.
 */
ACTIV_BASE_API StatusCode OsErrorToStatusCode(const uint32_t errorCode, const StatusCode defaultStatusCode = STATUS_CODE_FAILURE);

/**
 *	@brief	Suspend the execution of the current thread for the specified interval.
 *
 *	@param	period the period to sleep for, in ms.
 */
ACTIV_BASE_API void Suspend(const TimeoutPeriod period);

/**
 *	@brief	Replace any environment variables in a string with the value of the environment variable.
 *
 *	@param	source source string.
 *	@param	target reference to a string to receive the expanded string.
 *
 *	@return	true if conversion succeeded.
 */
ACTIV_BASE_API bool ReplaceEnvironmentVariables(const std::string &source, std::string &target);

/**
 *	@brief	Safe sprintf-like function that will not overrun the supplied buffer,
 *			and will always 0 terminate the formatted string.
 *
 *	@param	buff the pointer to the buffer to write the formatted string.
 *	@param	buffLen the total length of the buffer pointed to by \a buff (including space for a terminating 0).
 *	@param	format pointer to a sprintf-style format string.
 *	@param	... variable arguments for the format string.
 *
 *	@return	the number of characters written to buff.
 */
ACTIV_BASE_API size_t Snprintf(char *buff, const size_t buffLen, const char *format, ...);

/**
 *	@brief	Safe vsnprintf-like function that will not overrun the supplied buffer,
 *			and will always 0 terminate the formatted string.
 *
 *	@param	buff the pointer to the buffer to write the formatted string.
 *	@param	buffLen the total length of the buffer pointed to by \a buff (including space for a terminating 0).
 *	@param	format pointer to a sprintf-style format string.
 *	@param	args variable arguments for the format string.
 *
 *	@return	the number of characters written to buff.
 */
ACTIV_BASE_API size_t Vsnprintf(char *buff, const size_t buffLen, const char *format, std::va_list args);

/**
 *	@brief	Build a hex dump of a byte buffer.
 *
 *	@param	pBuffer the pointer to the buffer to dump.
 *	@param	bufferLength size of the buffer to dump.
 *	@param	bytesPerLine number of bytes to display per line.
 *
 *	@return	Formatted hex dump of the input buffer.
 */
ACTIV_BASE_API std::string BufferToHexDumpString(const void * const pBuffer, const size_t bufferLength, const size_t bytesPerLine = 16);

/**
 *	@brief	Build a hex dump of a byte buffer and write to an output stream.
 *
 *	@param	pBuffer the pointer to the buffer to dump.
 *	@param	bufferLength size of the buffer to dump.
 *	@param	outStream file stream to dump to. Default is standard out.
 */
ACTIV_BASE_API void HexDump(const void * const pBuffer, const size_t bufferLength, std::ostream &outStream = std::cout);

/**
 *	@brief	Convert a type to a string.
 *
 *	@return	the type as a string.
 */
template <typename T>
inline std::string TypeToString(const T &t)
{
    std::ostringstream os;
    os << t;
    return os.str();
}

#if (defined ACTIV_COMPILER_VISUAL_C) && (defined ACTIV_PROCESSOR_X86)

inline std::string TypeToString(const size_t &t)
{
	// this works around some warnings with Visual C++ and __w64
	return TypeToString(static_cast<uint32_t>(t));
}

#endif

inline std::string TypeToString(const char &c)
{
	// otherwise c gets converted to ascii char
	return TypeToString(static_cast<uint32_t>(c));
}

inline std::string TypeToString(const byte_t &c)
{
	// otherwise c gets converted to ascii char
	return TypeToString(static_cast<uint32_t>(c));
}

/**
 *	@brief	Convert a string to a basic type.
 *
 *	@param	str the string to conver.
 *	@param	t to receive the value.
 *
 *	@return	true if conversion was successful.
 */
template <typename T>
inline bool StringToType(const std::string &str, T &t)
{
	std::istringstream istream(str);

	return (istream >> t) ? true : false;
}

/**
 *	@brief	Swap two values.
 *
 *	@param	lhs the left hand side value.
 *	@param	rhs the right hand side value.
 */
template <typename T>
inline void SwapValues(T &lhs, T &rhs)
{
	T temp(lhs);
	lhs = rhs;
	rhs = temp;
}

/**
 *	@brief	Template function for returning a string from a table.
 *
 *	@param	index index into the table of strings \a array.
 *	@param	array table of c strings.
 *	@param	arraySize number of entries in \a array.
 *
 *	@return	The entry at index \a index, or if out of bounds, "unknown (0x<index>").
 */
template <typename T>
std::string IndexToString(const T index, const char * const array[], const size_t arraySize)
{
	const size_t actualIndex = static_cast<size_t>(index);

	if (actualIndex < arraySize)
		return array[actualIndex];

	char buff[30];

	(void) Snprintf(buff, ACTIV_ARRAY_SIZE(buff), "unknown (0x%0*x)", sizeof(index) * 2, index);
	return buff;
}

/**
 *	@brief	Hash a variable length key into a 32-bit value.
 *
 *	Every bit of the key affects every bit of the return value.
 *	Every 1-bit and 2-bit delta achieves avalanche. About 6*len+35
 *	instructions. The best hash table sizes are powers of 2. There
 *	is no need to do mod a prime (mod is sooo slow!). If you need
 *	less than 32 bits, use a bitmask. For example, if you need only
 *	10 bits, do h = (h & hashmask(10));, in which case, the hash table
 *	should have hashsize(10) elements. If you are hashing n strings
 *	(ub1 **)k, do it like this:
 *		for (i=0, h=0; i<n; ++i) h = hash( k[i], len[i], h);
 *	By Bob Jenkins, 1996.  bob_jenkins@burtleburtle.net.  You may use
 *	this code any way you wish, private, educational, or commercial.  It's free.
 *	See http://burtleburtle.net/bob/evahash.html
 *	Use for hash table lookup, or anything where one collision in 2^^32 is
 *	acceptable.  Do NOT use for cryptographic purposes.
 *
 *	@param	pKeyBuffer the key buffer to hash.
 *	@param	keyLength the length of the key buffer.
 *	@param	initialValue the initial value of the hash.
 *
 *	@return the calculated hash value.
 */
ACTIV_BASE_API uint32_t GenericHash(const void *pKeyBuffer, const size_t keyLength, const uint32_t initialValue);

// ---------------------------------------------------------------------------------------------------------------------------------

#if !defined (ACTIV_PLATFORM_WINDOWS)

/**
 *	@brief	Convert a string to uppercase.
 *
 *	@param	pString the pointer to the string to convert.
 *
 *	@return	a pointer to the converted string.
 */
ACTIV_BASE_API char *strupr(char *pString);

/**
 *	@brief	Convert a string to lowercase.
 *
 *	@param	pString the pointer to the string to convert.
 *
 *	@return	a pointer to the converted string.
 */
ACTIV_BASE_API char *strlwr(char *pString);

#endif

} // namespace Activ

#endif // !defined (ACTIV_BASE_H)

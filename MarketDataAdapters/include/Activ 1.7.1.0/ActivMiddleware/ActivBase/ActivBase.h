/**
 *	@file	ActivBase.h
 *
 *	@brief	Global functions from ActivBase.
 *
 *  ActivBase provides low-level os independant primatives.
 *
 *	$Log: $
 */

#if (!defined ACTIV_BASE_H)
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

// ---------------------------------------------------------------------------------------------------------------------------------

/**
 *	@brief	Log a string. A newline is appended. By default the output is sent to stdout.
 *
 *	@param	type
 *	@param	message string to log.
 */
ACTIV_BASE_API void LogMessage(const LogType type, const char *message);

/**
 *	@brief	Log a string. A newline is appended. By default the output is sent to stdout.
 *
 *	@param	type
 *	@param	message string to log.
 */
ACTIV_BASE_API void LogMessage(const LogType type, const std::string &message);

/**
 *	@brief	Log a boost::format message. A newline is appended. By default the output is sent to stdout.
 *
 *	@param	type
 *	@param	format reference to boost::format object containing the string to log.
 */
ACTIV_BASE_API void LogMessage(const LogType type, const boost::format &format);

/**
 *	@brief	Return log type as a string.
 *
 *	@param	logType.
 *
 *	@return	String representation of \a logType.
 */
ACTIV_BASE_API std::string LogTypeToString(const LogType logType);

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

/**
 *	@brief	Play a sound sample.
 *
 *	Not available on all platforms. Will return STATUS_CODE_NOT_SUPPORTED in those cases.
 *
 *	@param	pData pointer to the sound data.
 *	@param	dataLength length of the sound data pointed to by @a pData.
 *	@param	shouldPlayAsync whether or not to wait for the sound to finish playing before returning.
 *			If true, the @pData buffer must not be deleted before the sound finishes playing.
 *
 *	@return	STATUS_CODE_SUCCESS or some error status.
 */
ACTIV_BASE_API StatusCode PlaySound(const void *pData, const size_t dataLength, const bool shouldPlayAsync = true);

} // namespace Activ

#endif // (!defined ACTIV_BASE_H)

/**
 *	@file	LoggerServiceApi.h
 *
 *	@brief	Header file for the logger service api class.
 *
 *	$Log: $
 */

#if !defined (ACTIV_FRAMEWORK_LOGGER_SERVICE_API_H)
#define ACTIV_FRAMEWORK_LOGGER_SERVICE_API_H

#include "ActivMiddleware/ActivFramework/External.h"
#include "ActivMiddleware/ActivBaseServices/LoggerDefinitions.h"

namespace Activ
{

class Component;

/**
 *	@brief	Api to the agent's logger service.
 */
class LoggerServiceApi
{
public:
	/**
	 *	@brief	Open a log file.
	 *
	 *	@param	component that wishes to open the log file.
	 *	@param	fileName the name of the log file.
	 *	@param	fileType the type of the log file.
	 *	@param	isUniqueFileName whether the filename should be unique.
	 *	@param	logAddress reference to an Address object to receive the address of the new log file.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
	ACTIV_FRAMEWORK_API static StatusCode OpenLogFile(const Component &component,
													  const std::string &fileName,
													  const LoggerDefinitions::FileType fileType,
													  const bool isUniqueFileName,
													  Address &logAddress);

	/**
	 *	@brief	Open a rolling log file.
	 *
	 *	@param	component that wishes to open the log file.
	 *	@param	fileName the name of the log file.
	 *	@param	fileType the type of the log file.
	 *	@param	maxFileSize the maximum size of the file.
	 *	@param	nBackups the number of backup files.
	 *	@param	logAddress reference to an Address object to receive the address of the new log file.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
	ACTIV_FRAMEWORK_API static StatusCode OpenLogFile(const Component &component,
													  const std::string &fileName,
													  const LoggerDefinitions::FileType fileType,
													  const uint64_t maxFileSize,
													  const size_t nBackups,
													  Address &logAddress);

	/**
	 *	@brief	Open a curbed log file.
	 *
	 *	@param	component that wishes to open the log file.
	 *	@param	fileName the name of the log file.
	 *	@param	fileType the type of the log file.
	 *	@param	maxFileSize the maximum size of the file.
	 *	@param	nBackups the number of backup files.
	 *	@param	isUniqueFileName whether the filename should be unique.
	 *	@param	logAddress reference to an Address object to receive the address of the new log file.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	... 
	 */
	ACTIV_FRAMEWORK_API static StatusCode OpenLogFile(const Component &component,
													  const std::string &fileName,
													  const LoggerDefinitions::FileType fileType,
													  const uint64_t maxFileSize,
													  const size_t nBackups,
													  const bool isUniqueFileName,
													  Address &logAddress);

	/**
	 *	@brief	Set header for a log file.
	 *
	 *	The header will be appended to the log file now if the log is empty and whenever a new log
	 *	is created (e.g. overnight, weekly, etc.). Note a newline will automatically be appended.
	 *
	 *	@param	component that wishes to close the log file.
	 *	@param	logAddress the address of a log file.
	 *	@param	header.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
	ACTIV_FRAMEWORK_API static StatusCode SetLogHeader(const Component &component, const Address &logAddress, const std::string &header);

	/**
	 *	@brief	Close a log file.
	 *
	 *	@param	component that wishes to close the log file.
	 *	@param	logAddress the address of a log file.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
	ACTIV_FRAMEWORK_API static StatusCode CloseLogFile(const Component &component, const Address &logAddress);

	/**
	 *	@brief	Log a message to a log file.
	 *
	 *	@param	component that wishes to log.
	 *	@param	logAddress the address of a log file.
	 *	@param	message the message to log.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_HEAP_EMPTY
	 *	@retval	...
	 */
	ACTIV_FRAMEWORK_API static StatusCode LogMessage(const Component &component, const Address &logAddress, const std::string &message);

	/**
	 *	@brief	Log a message to a log file.
	 *
	 *	@param	component that wishes to log.
	 *	@param	logAddress the address of a log file.
	 *	@param	message the message to log.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_HEAP_EMPTY
	 *	@retval	...
	 */
	ACTIV_FRAMEWORK_API static StatusCode LogMessage(const Component &component, const Address &logAddress, const void *pMessage,
													 const size_t messageLength);

	/**
	 *	@brief	Log a message to a log file.
	 *
	 *	@param	component that wishes to log.
	 *	@param	logAddress the address of a log file.
	 *	@param	heapMessage a HeapMessage containing the message to log.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
	ACTIV_FRAMEWORK_API static StatusCode LogMessage(const Component &component, const Address &logAddress, HeapMessage &heapMessage);
};

} // namespace Activ

#endif // !defined (ACTIV_FRAMEWORK_LOGGER_SERVICE_API_H)

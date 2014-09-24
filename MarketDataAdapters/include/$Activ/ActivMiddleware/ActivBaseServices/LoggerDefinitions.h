/**
 *	@file	LoggerDefinitions.h
 *
 *	@brief	Header file for the logger definitions class.
 *
 *	$Log: $
 */

#if !defined (ACTIV_LOGGER_DEFINITIONS_H)
#define ACTIV_LOGGER_DEFINITIONS_H

namespace Activ
{

/**
 *	@brief	The list of possible log file types and their string representation.
 */
#define	FILE_TYPE_LIST(d)							\
	d(	FILE_TYPE_DAILY, "Daily")					\
	d(	FILE_TYPE_WEEKLY, "Weekly")					\
	d(	FILE_TYPE_MONTHLY, "Monthly")				\
	d(	FILE_TYPE_ROLLING, "Rolling")

/**
 *	@brief	Logger definitions class.
 */
class LoggerDefinitions
{
public:
	static const PortId		servicePortId				= 0;							///< Fixed port number of logger service.

	static const uint64_t	MAX_FILE_SIZE_UNDEFINED		= static_cast<uint64_t>(~0);
	static const size_t		NUMBER_OF_BACKUPS_UNDEFINED	= static_cast<size_t>(~0);

	/**
	 *	@brief	Log file type enumeration.
	 */
	enum FileType
	{
		FILE_TYPE_LIST(ACTIV_DECLARE_ELEMENT_1_WITH_COMMA)	///< The list of log file types.
		NUMBER_OF_FILE_TYPES,								///< The number of log file types.
	};

	/**
	 *	@brief	Get the type as a string.
	 *
	 *	@param	logFileType the log file type.
	 *
	 *	@return the type as a string.
	 */
	static std::string TypeToString(const FileType logFileType);
};

// ---------------------------------------------------------------------------------------------------------------------------------

inline std::string LoggerDefinitions::TypeToString(const FileType logFileType)
{
	static char *strs[] = { FILE_TYPE_LIST(ACTIV_DECLARE_ELEMENT_2_WITH_COMMA) };

	if ((logFileType >= 0) && (logFileType < static_cast<FileType>(ACTIV_ARRAY_SIZE(strs))))
		return std::string(strs[logFileType]);

	return std::string("Unknown");
}

} // namespace Activ

#endif // !defined (ACTIV_LOGGER_DEFINITIONS_H)

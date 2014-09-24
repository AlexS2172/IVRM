/**
 *	@file	UpdateDescription.h
 *
 *	@brief	Header file for the update description class.
 *
 *	$Log: $
 */

#if !defined (ACTIV_UPDATE_DESCRIPTION_H)
#define ACTIV_UPDATE_DESCRIPTION_H

#include "ActivMiddleware/ActivUpdate/External.h"
#include "ActivMiddleware/ActivUpdate/UpdateTarget.h"

namespace Activ
{

/**
 *	@brief	Update description class.
 */
class UpdateDescription
{
public:
	/**
	 *	@brief	Default constructor.
	 */
	ACTIV_UPDATE_API UpdateDescription();

	/**
	 *	@brief	Destructor.
	 */
	ACTIV_UPDATE_API ~UpdateDescription();

	/**
	 *	@brief	Read settings from an ini file section.
	 *
	 *	@param	sectionIterator iterator to an ini file section.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
	ACTIV_UPDATE_API StatusCode Read(const IniFileParser::ConstSectionIterator &sectionIterator);

	/**
	 *	@brief	Read from ini file.
	 *
	 *	@param	iniFileName the ini file name.
	 *	@param	sectionName the section name.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
	ACTIV_UPDATE_API StatusCode ReadFromIniFile(const std::string &iniFileName, const std::string &sectionName = GetDefaultSectionName());

	/**
	 *	@brief	Get the default section name.
	 *
	 *	@return	the default section name.
	 */
	ACTIV_UPDATE_API static const std::string &GetDefaultSectionName();

	/**
	 *	@brief	Get the file extension.
	 *
	 *	@return	the file extension.
	 */
	ACTIV_UPDATE_API static const std::string &GetFileExtension();

	/**
	 *	@brief	Has file name got an file extension?
	 *
	 *	@param	fileName the file name.
	 *
	 *	@return	true if file name has file extension.
	 */
	ACTIV_UPDATE_API static bool HasFileExtension(const std::string &fileName);

	/**
	 *	@brief	Get system type.
	 */
	ACTIV_UPDATE_API static const std::string &GetSystemType();

	DateTime		m_uploadDateTime;							///< The date/time to upload the update.
	DateTime		m_applyDateTime;							///< The date/time to apply the update.
	std::string		m_updateFileName;							///< The update file name.
	std::string		m_executeFileName;							///< The execute file name.
	bool			m_shutdownOnExecute;						///< Should shutdown on execute?
	uint32_t		m_uploadAttempts;							///< The number of attempts to upload the update.
	uint32_t		m_uploadAttemptInterval;					///< The interval between upload attempts.
	UpdateTarget	m_updateTarget;								///< The update target.
	std::string		m_updateType;								///< The update type.
};

} // namespace Activ

#endif // !defined (ACTIV_UPDATE_DESCRIPTION_H)

/**
 *	@file	Timeband.h
 *
 *	@brief	Header file for the timeband class..
 *
 *	$Log: $
 */

#if !defined (ACTIV_TIMEBAND_H)
#define ACTIV_TIMEBAND_H

#include "ActivMiddleware/ActivFramework/External.h"

#include "ActivMiddleware/ActivBase/IniFileParser.h"
#include "ActivMiddleware/ActivFramework/TimeHelper.h"

#include <bitset>
#include <string>

namespace Activ
{

/**
 *	@brief	Timeband class.
 */
class Timeband
{
public:
	/**
	 *	@brief	Settings class.
	 */
	class Settings
	{
	public:
		/**
		 *	@brief	Default constructor.
		 */
		ACTIV_FRAMEWORK_API Settings();

		/**
		 *	@brief	Destructor.
		 */
		ACTIV_FRAMEWORK_API virtual ~Settings();

		/*	@brief	Read settings from an ini file section.
		 *
		 *	@param	sectionIterator iterator to an ini file section.
		 *
		 *	@retval	STATUS_CODE_SUCCESS
		 *	@retval	...
		 */
		ACTIV_FRAMEWORK_API virtual StatusCode Read(const std::string &keyNamePrefix, const IniFileParser::ConstSectionIterator &sectionIterator);

		typedef std::bitset<7>	EnabledDaysBitmap;				///< enabled days bitmap type

		EnabledDaysBitmap	m_enabledDaysBitmap;				///< enabled days bitmap
		Time				m_startTime;						///< start time of timeband
		Time				m_endTime;							///< end time of timeband
	};

	/**
	 *	@brief	Default constructor.
	 */
	ACTIV_FRAMEWORK_API Timeband();

	/**
	 *	@brief	Constructor.
	 *
	 *	@param	settings object's settings.
	 */
	ACTIV_FRAMEWORK_API Timeband(const Settings &settings);

	/**
	 *	@brief	is the time within this timeband.
	 *
	 *	@param	localTime time to test if in this timeband.
	 */
	ACTIV_FRAMEWORK_API bool IsIn(const LocalTime &localTime) const;

private:
	const Settings	m_settings;									///< object's settings
};

} // namespace Activ

#endif // !defined (ACTIV_TIMEBAND_H)

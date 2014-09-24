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
	 *	@brief	Enabled date flag enum.
	 */
	enum EnabledDateFlag
	{
		ENABLED_DATE_END_TIME,
		ENABLED_DATE_START_TIME,
	};

	typedef std::set<Date>	DateSet;						///< date set type
	typedef std::bitset<7>	EnabledDaysBitmap;				///< enabled days bitmap type

	/**
	 *	@brief	Settings class.
	 *
	 *	Deprecated, use the Timeband class directly.
	 */
	class Settings
	{
	public:
		/**
		 *	@brief	Default constructor.
		 */
		ACTIV_FRAMEWORK_API Settings();

		/**
		 *	@brief	Constructor.
		 *
		 *	@param	startTime start time of the timeband.
		 *	@param	endTime end time of the timeband.
		 *	@param	enabledDaysBitmapString enabled days bitmap string, e.g. "-mtwtf-".
		 *	@param	endabledDateFlag the endabled date flag.
		 *	@param	disabledDateSet the disabled date set.
		 */
		ACTIV_FRAMEWORK_API Settings(const Time &startTime, const Time &endTime, const std::string &enabledDaysBitmapString,
									 const EnabledDateFlag enabledDateFlag = ENABLED_DATE_END_TIME, const DateSet &disabledDateSet = DateSet());

		/*	@brief	Read settings from an ini file section.
		 *
		 *	@param	sectionIterator iterator to an ini file section.
		 *
		 *	@retval	STATUS_CODE_SUCCESS
		 *	@retval	...
		 */
		ACTIV_FRAMEWORK_API StatusCode Read(const std::string &keyNamePrefix, const IniFileParser::ConstSectionIterator &sectionIterator);

		EnabledDaysBitmap	m_enabledDaysBitmap;				///< enabled days bitmap
		DateSet				m_disabledDateSet;					///< disabled date set
		Time				m_startTime;						///< start time of timeband
		Time				m_endTime;							///< end time of timeband
		EnabledDateFlag		m_enabledDateFlag;					///< enabled date flag
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
	 *	@brief	Constructor.
	 *
	 *	@param	startTime start time of the timeband.
	 *	@param	endTime end time of the timeband.
	 *	@param	enabledDaysBitmapString enabled days bitmap string, e.g. "-mtwtf-".
	 *	@param	endabledDateFlag the endabled date flag.
	 *	@param	disabledDateSet the disabled date set.
	 */
	ACTIV_FRAMEWORK_API Timeband(const Time &startTime, const Time &endTime, const std::string &enabledDaysBitmapString,
								 const EnabledDateFlag enabledDateFlag = ENABLED_DATE_END_TIME, const DateSet &disabledDateSet = DateSet());

	/**
	 *	@brief	Is the time within this timeband.
	 *
	 *	@param	dateTime date/time to test if in this timeband.
	 */
	ACTIV_FRAMEWORK_API bool IsIn(const DateTime &dateTime) const;

	/**
	 *	@brief	Is the time within this timeband.
	 *
	 *	Deprecated, use version of IsIn() taking a @a DateTime.
	 *
	 *	@param	localTime the local time to test if in this timeband.
	 */
	bool IsIn(const LocalTime &localTime) const;

	/*	@brief	Read settings from an ini file section.
	 *
	 *	@param	sectionIterator iterator to an ini file section.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
	StatusCode Read(const std::string &keyNamePrefix, const IniFileParser::ConstSectionIterator &sectionIterator);

	/**
	 *	@brief	Get the start time.
	 *
	 *	@return	const reference to the start time.
	 */
	const Time &GetStartTime() const;

	/**
	 *	@brief	Get the end time.
	 *
	 *	@return	const reference to the end time.
	 */
	const Time &GetEndTime() const;

	/**
	 *	@brief	Get the enabled days string.
	 *
	 *	@return	the enabled days string.
	 */
	ACTIV_FRAMEWORK_API const std::string GetEnabledDaysString() const;

	/**
	 *	@brief	Get the disabled date set.
	 *
	 *	@return	the disabled date set.
	 */
	const DateSet &GetDisabledDateSet() const;

	/**
	 *	@brief	Get the enabled date flag.
	 *
	 *	@return	the enabled date flag.
	 */
	EnabledDateFlag GetEnabledDateFlag() const;

	/**
	 *	@brief	Set the start time.
	 *
	 *	@param	startTime the new start time.
	 */
	void SetStartTime(const Time &startTime);

	/**
	 *	@brief	Set the end time.
	 *
	 *	@param	endTime the new end time.
	 */
	void SetEndTime(const Time &endTime);

	/**
	 *	@brief	Set the enabled days string.
	 *
	 *	@param	enabledDaysString the new enabled days string.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
	StatusCode SetEnabledDaysString(const std::string &enabledDaysString);

	/**
	 *	@brief	Set the disbaled date set.
	 *
	 *	@param	disabledDateSet the new disabled date set.
	 */
	void SetDisabledDateSet(const DateSet &disabledDateSet);

	/**
	 *	@brief	Set the enabled date flag.
	 *
	 *	@param	enabledDateFlag the new enabled date flag.
	 */
	void SetEnabledDateFlag(const EnabledDateFlag enabledDateFlag);

	/**
	 *	@brief	Add disabled date.
	 *
	 *	@param	disabledDate the disabled date to add.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
	ACTIV_FRAMEWORK_API StatusCode AddDisabledDate(const Date &disabledDate);

	/**
	 *	@brief	Remove disabled date.
	 *
	 *	@param	disabledDate the disabled date to remove.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
	ACTIV_FRAMEWORK_API StatusCode RemoveDisabledDate(const Date &disabledDate);

	/**
	 *	@brief	Convert string to enabled days bitmap.
	 *
	 *	@param	enabledDaysBitmapString the string to convert.
	 *	@param	enabledDaysBitmap the enabled days bitmap to receive the result.
	 */
	ACTIV_FRAMEWORK_API static StatusCode ConvertStringToEnabledDaysBitmap(const std::string enabledDaysBitmapString, 
																		   EnabledDaysBitmap &enabledDaysBitmap);

private:
	/**
	 *	@brief	Is enabled day?
	 *
	 *	@param	date the date to test.
	 */
	bool IsEnabledDay(const Date &date) const;

	/**
	 *	@brief	Get day chars string.
	 *
	 *	@return	days chars string.
	 */
	static const std::string &GetDayCharsString();

	Settings	m_settings;										///< object's settings
};

// ---------------------------------------------------------------------------------------------------------------------------------

inline bool Timeband::IsIn(const LocalTime &localTime) const
{
	return IsIn(localTime.Get());
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode Timeband::Read(const std::string &keyNamePrefix, const IniFileParser::ConstSectionIterator &sectionIterator)
{
	return m_settings.Read(keyNamePrefix, sectionIterator);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline const Time &Timeband::GetStartTime() const
{
	return m_settings.m_startTime;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline const Time &Timeband::GetEndTime() const
{
	return m_settings.m_endTime;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline const Timeband::DateSet &Timeband::GetDisabledDateSet() const
{
	return m_settings.m_disabledDateSet;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline Timeband::EnabledDateFlag Timeband::GetEnabledDateFlag() const
{
	return m_settings.m_enabledDateFlag;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline void Timeband::SetStartTime(const Time &startTime)
{
	m_settings.m_startTime = startTime;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline void Timeband::SetEndTime(const Time &endTime)
{
	m_settings.m_endTime = endTime;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode Timeband::SetEnabledDaysString(const std::string &enabledDaysString)
{
	return ConvertStringToEnabledDaysBitmap(enabledDaysString, m_settings.m_enabledDaysBitmap);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline void Timeband::SetDisabledDateSet(const DateSet &disabledDateSet)
{
	m_settings.m_disabledDateSet = disabledDateSet;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline void Timeband::SetEnabledDateFlag(const EnabledDateFlag enabledDateFlag)
{
	m_settings.m_enabledDateFlag = enabledDateFlag;
}

} // namespace Activ

#endif // !defined (ACTIV_TIMEBAND_H)

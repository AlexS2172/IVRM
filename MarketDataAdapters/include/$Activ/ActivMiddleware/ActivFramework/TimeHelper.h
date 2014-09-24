/**
 *	@file	TimeHelper.h
 *
 *	@brief	Header file for time helper classes.
 *
 *  $Log: $
 */

#if !defined (ACTIV_FRAMEWORK_TIME_HELPER_H)
#define ACTIV_FRAMEWORK_TIME_HELPER_H

#include "ActivMiddleware/ActivFramework/External.h"

#include "ActivMiddleware/ActivFieldTypes/DateTime.h"

#include <string>

namespace Activ
{

/**
 *	@brief	Base time class.
 *
 *	This class maintains both time in utc format and "formatted" format (local or utc).
 */
class BaseTime
{
public:
	typedef uint8_t DayOfWeek;									///< day of week type

	static const DayOfWeek SUNDAY = 0;
	static const DayOfWeek MONDAY = 1;
	static const DayOfWeek TUESDAY = 2;
	static const DayOfWeek WEDSDAY = 3;
	static const DayOfWeek THURSDAY = 4;
	static const DayOfWeek FRIDAY = 5;
	static const DayOfWeek SATURDAY = 6;

	static const uint32_t secondsPerMinute = 60;
	static const uint32_t secondsPerHour = 60 * secondsPerMinute;
	static const uint32_t secondsPerDay = 24 * secondsPerHour;

	static const uint32_t millisecondsPerSecond = 1000;
	static const uint32_t millisecondsPerMinute = secondsPerMinute * millisecondsPerSecond;
	static const uint32_t millisecondsPerHour = secondsPerHour * millisecondsPerSecond;
	static const uint32_t millisecondsPerDay = secondsPerDay * millisecondsPerSecond;

	/**
	 *	@brief	Duration class.
	 */
	class Duration
	{
	public:
		/**
		 *	@brief	Constructor.
		 *
		 *	@param	duration in milliseconds.
		 */
		ACTIV_FRAMEWORK_API Duration(const int64_t duration);

		/**
		 *	@brief	Constructor.
		 *
		 *	@param	days total days of duration.
		 *	@param	hours additional or total hours of duration.
		 *	@param	minutes additional or total minutes of duration.
		 *	@param	seconds additional or total seconds of duration.
		 *	@param	milliseconds additional or total milliseconds of duration.
		 */
		ACTIV_FRAMEWORK_API Duration(const int32_t days, const int32_t hours, const int32_t minutes = 0, const int32_t seconds = 0,
									 const int32_t milliseconds = 0);

		/**
		 *	@brief	Get the duration in milliseconds.
		 */
		ACTIV_FRAMEWORK_API int64_t GetTotalMilliseconds() const;

		/**
		 *	@brief	Convert the object to a string.
		 *
		 *	@return	the object as a string.
		 */
		ACTIV_FRAMEWORK_API std::string ToString() const;

	private:
		const int64_t	m_duration;
	};

	/**
	 *	@brief	Destructor.
	 */
	ACTIV_FRAMEWORK_API virtual ~BaseTime();

	/**
	 *	@brief	Get the year.
	 *
	 *	@return	The year.
	 */
	ACTIV_FRAMEWORK_API uint16_t GetYear() const;

	/**
	 *	@brief	Get the month.
	 *
	 *	@return	The month.
	 */
	ACTIV_FRAMEWORK_API uint8_t GetMonth() const;

	/**
	 *	@brief	Get the day.
	 *
	 *	@return	The day.
	 */
	ACTIV_FRAMEWORK_API uint8_t GetDay() const;

	/**
	 *	@brief	Get the day of the week.
	 *
	 *	@return	The day of the week.
	 */
	ACTIV_FRAMEWORK_API DayOfWeek GetDayOfWeek() const;

	/**
	 *	@brief	Get the hour.
	 *
	 *	@return	The hour.
	 */
	ACTIV_FRAMEWORK_API uint8_t GetHour() const;

	/**
	 *	@brief	Get the minute.
	 *
	 *	@return	The minute.
	 */
	ACTIV_FRAMEWORK_API uint8_t GetMinute() const;

	/**
	 *	@brief	Get the second.
	 *
	 *	@return	The second.
	 */
	ACTIV_FRAMEWORK_API uint8_t GetSecond() const;

	/**
	 *	@brief	Get the millisecond.
	 *
	 *	@return	The millisecond.
	 */
	ACTIV_FRAMEWORK_API uint16_t GetMillisecond() const;

	/**
	 *	@brief	Get julian time (second resolution).
	 *
	 *	@return	Julian time.
	 */
	ACTIV_FRAMEWORK_API uint32_t GetJulianTime() const;

	/**
	 *	@brief	Is utc time?
	 *
	 *	@return	True if Utc time, false for local.
	 */
	bool IsUtc() const;

	/**
	 *	@brief	Assignment operator.
	 *
	 *	@param	rhs to copy from.
	 *
	 *	@return	*this.
	 */
	ACTIV_FRAMEWORK_API BaseTime &operator=(const BaseTime &rhs);
	
	/**
	 *	@brief	== operator.
	 *
	 *	@param	rhs BaseTime object to compare with.
	 *
	 *	@return	boolean.
	 */
	ACTIV_FRAMEWORK_API bool operator==(const BaseTime &rhs) const;

	/**
	 *	@brief	!= operator.
	 *
	 *	@param	rhs BaseTime object to compare with.
	 *
	 *	@return	boolean.
	 */
	ACTIV_FRAMEWORK_API bool operator!=(const BaseTime &rhs) const;

	/**
	 *	@brief	< operator.
	 *
	 *	@param	rhs BaseTime object to compare with.
	 *
	 *	@return	boolean.
	 */
	ACTIV_FRAMEWORK_API bool operator<(const BaseTime &rhs) const;

	/**
	 *	@brief	<= operator.
	 *
	 *	@param	rhs BaseTime object to compare with.
	 *
	 *	@return	boolean.
	 */
	ACTIV_FRAMEWORK_API bool operator<=(const BaseTime &rhs) const;

	/**
	 *	@brief	> operator.
	 *
	 *	@param	rhs BaseTime object to compare with.
	 *
	 *	@return	boolean.
	 */
	ACTIV_FRAMEWORK_API bool operator>(const BaseTime &rhs) const;

	/**
	 *	@brief	>= operator.
	 *
	 *	@param	rhs BaseTime object to compare with.
	 *
	 *	@return	boolean.
	 */
	ACTIV_FRAMEWORK_API bool operator>=(const BaseTime &rhs) const;

	/**
	 *	@brief	+= operator.
	 *
	 *	@param	numberOfMilliseconds number of milliseconds to add to the time.
	 *
	 *	@return	boolean.
	 */
	ACTIV_FRAMEWORK_API BaseTime &operator+=(const uint64_t numberOfMilliseconds);

	/**
	 *	@brief	+= operator.
	 *
	 *	@param	duration duration to add to the time.
	 *
	 *	@return	boolean.
	 */
	ACTIV_FRAMEWORK_API BaseTime &operator+=(const Duration &duration);

	/**
	 *	@brief	-= operator.
	 *
	 *	@param	numberOfMilliseconds number of milliseconds to subtract from the time.
	 *
	 *	@return	boolean.
	 */
	ACTIV_FRAMEWORK_API BaseTime &operator-=(const uint64_t numberOfMilliseconds);

	/**
	 *	@brief	-= operator.
	 *
	 *	@param	duration duration to subtract from the time.
	 *
	 *	@return	boolean.
	 */
	ACTIV_FRAMEWORK_API BaseTime &operator-=(const Duration &duration);

	/**
	 *	@brief	Convert to a string.
	 *
	 *	@return	A string containing data and time in current locale.
	 */
	ACTIV_FRAMEWORK_API std::string ToString() const;

	/**
	 *	@brief	Convert date to a string.
	 *
	 *	@return	A string containing date in current locale format.
	 */
	ACTIV_FRAMEWORK_API std::string DateToString() const;

	/**
	 *	@brief	Convert time to a string.
	 *
	 *	@return	A string containing time.
	 */
	ACTIV_FRAMEWORK_API std::string TimeToString() const;

	/**
	 *	@brief	Get day of week in string form.
	 *
	 *	@param	dayOfWeek.
	 *	@param	isShort return short form of day of week.
	 *
	 *	@return	String representation of \a dayOfWeek.
	 */
	ACTIV_FRAMEWORK_API static std::string DayOfWeekToString(const DayOfWeek dayOfWeek, bool isShort = false);

	/**
	 *	@brief	Get month in string form.
	 *
	 *	@param	month.
	 *	@param	isShort return short form of month.
	 *
	 *	@return	String representation of \a month.
	 */
	ACTIV_FRAMEWORK_API static std::string MonthToString(const uint8_t month, bool isShort = false);

	/**
	 *	@brief	Serialize.
	 *
	 *	@param	messageBuilder builder to serialize with.
	 *
	 *	@retval	STATUS_CODE_SUCCESS.
	 *	@retval	...
	 */
	ACTIV_FRAMEWORK_API StatusCode Serialize(MessageBuilder &messageBuilder) const;

	/**
	 *	@brief	Deserialize.
	 *
	 *	@param	messageValidater validater to deserialize with.
	 *
	 *	@retval	STATUS_CODE_SUCCESS.
	 *	@retval	...
	 */
	ACTIV_FRAMEWORK_API StatusCode Deserialize(MessageValidater &messageValidater);

private:
	/**
	 *	@brief	Default constructor.
	 */
	BaseTime(const bool isUtc);

	/**
	 *	@brief	Make formatted time from utc time.
	 */
	void MakeFormattedTime();

	/**
	 *	@brief	Make utc time from formatted local time.
	 *
	 *	@param	millisecond.
	 */
	void MakeUtcTime(const uint16_t millisecond);

	/**
	 *	@brief	Get current time.
	 */
	void GetCurrent();

	/**
	 *	@brief	Adjust current time.
	 *
	 *	@param	numberOfMilliseconds to adjust by.
	 */
	void AdjustTime(const int64_t numberOfMilliseconds);

	tm			m_formattedTime;								///< local or utc depending on m_isUtc
	uint64_t	m_utcTime;										///< seconds * 1000 + milli, utc
	bool		m_isUtc;

	friend class UtcTime;
	friend class LocalTime;
};

// ---------------------------------------------------------------------------------------------------------------------------------

/**
 *	@brief	Utc time class.
 */
class UtcTime : public BaseTime
{
public:
	/**
	 *	@brief	Default constructor.
	 */
	ACTIV_FRAMEWORK_API UtcTime();

	/**
	 *	@brief	Copy constructor.
	 *
	 *	@param	baseTime time to copy from (can be LocalTime or UtcTime).
	 */
	ACTIV_FRAMEWORK_API UtcTime(const BaseTime &baseTime);

	/**
	 *	@brief	Get current utc time.
	 *
	 *	@return	UtcTime initialized with the current time.
	 */
	ACTIV_FRAMEWORK_API static UtcTime GetCurrent();
};

// ---------------------------------------------------------------------------------------------------------------------------------

/**
 *	@brief	Local time class.
 */
class LocalTime : public BaseTime
{
public:
	/**
	 *	@brief	Default constructor.
	 */
	ACTIV_FRAMEWORK_API LocalTime();

	/**
	 *	@brief	Constructor.
	 *
	 *	Construct from specific local time.
	 *
 	 *	@param	year the year.
	 *	@param	month the month.
	 *	@param	day the day.
	 *	@param	hour the hour.
	 *	@param	minute the minute.
	 *	@param	second the second.
	 *	@param	millisecond the millisecond.
	 */
	explicit ACTIV_FRAMEWORK_API LocalTime(const uint16_t year, const uint8_t month = 1, const uint8_t day = 1, const uint8_t hour = 0, const uint8_t minute = 0, const uint8_t second = 0, uint16_t millisecond = 0);

	/**
	 *	@brief	Constructor.
	 *
	 *	Construct from specific local time.
	 *
 	 *	@param	dateTime the date/time.
	 *	@param	millisecond the millisecond.
	 */
	ACTIV_FRAMEWORK_API LocalTime(const DateTime &dateTime, uint16_t millisecond = 0);

	/**
	 *	@brief	Constructor.
	 *
	 *	Construct from specific local time.
	 *
 	 *	@param	date the date.
 	 *	@param	time the time.
	 *	@param	millisecond the millisecond.
	 */
	ACTIV_FRAMEWORK_API LocalTime(const Date &date, const Time &time, uint16_t millisecond = 0);

	/**
	 *	@brief	Copy constructor.
	 *
	 *	@param	baseTime time to copy from (can be LocalTime or UtcTime).
	 */
	ACTIV_FRAMEWORK_API LocalTime(const BaseTime &baseTime);

	/**
	 *	@brief	Get.
	 *
	 *	@return	the date/time.
	 */
	ACTIV_FRAMEWORK_API DateTime Get() const;

	/**
	 *	@brief	Get current local time.
	 *
	 *	@return	LocalTime initialized with the current time.
	 */
	ACTIV_FRAMEWORK_API static LocalTime GetCurrent();

	/**
	 *	@brief	Create a LocalTime object representing the next midnight.
	 *
	 *	@return	LocalTime initialized with the next midnight time.
	 */
	ACTIV_FRAMEWORK_API static LocalTime GetMidnight();


};

// ---------------------------------------------------------------------------------------------------------------------------------

inline uint16_t BaseTime::GetYear() const
{
	return static_cast<uint16_t>(m_formattedTime.tm_year) + 1900;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline uint8_t BaseTime::GetMonth() const
{
	return static_cast<uint8_t>(m_formattedTime.tm_mon) + 1;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline BaseTime::DayOfWeek BaseTime::GetDayOfWeek() const
{
	return static_cast<DayOfWeek>(m_formattedTime.tm_wday);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline uint8_t BaseTime::GetDay() const
{
	return static_cast<uint8_t>(m_formattedTime.tm_mday);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline uint8_t BaseTime::GetHour() const
{
	return static_cast<uint8_t>(m_formattedTime.tm_hour);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline uint8_t BaseTime::GetMinute() const
{
	return static_cast<uint8_t>(m_formattedTime.tm_min);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline uint8_t BaseTime::GetSecond() const
{
	return static_cast<uint8_t>(m_formattedTime.tm_sec);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline uint16_t BaseTime::GetMillisecond() const
{
	return static_cast<uint16_t>(m_utcTime % UINT64_C(1000));
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline uint32_t BaseTime::GetJulianTime() const
{
	return static_cast<uint32_t>(GetSecond()) + (secondsPerMinute * static_cast<uint32_t>(GetMinute())) + (secondsPerHour * static_cast<uint32_t>(GetHour()));
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline bool BaseTime::IsUtc() const
{
	return m_isUtc;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline DateTime LocalTime::Get() const
{
	return DateTime(GetYear(), GetMonth(), GetDay(), GetHour(), GetMinute(), GetSecond());
}

// ---------------------------------------------------------------------------------------------------------------------------------


/**
 *	@brief	Addition operator for the utc time object.
 *
 *	@param	lhs the left hand side value.
 *	@param	numberOfMilliseconds number of milliseconds to add to the time.
 *
 *	@return	an object containing the result of the operation.
 *
 *	@throw	ActivException
 */
inline const UtcTime operator+(const UtcTime &lhs, const uint64_t numberOfMilliseconds)
{
	return UtcTime(lhs) += numberOfMilliseconds;
}

// ---------------------------------------------------------------------------------------------------------------------------------

/**
 *	@brief	Addition operator for the utc time object.
 *
 *	@param	lhs the left hand side value.
 *	@param	duration the duration to add to the time.
 *
 *	@return	an object containing the result of the operation.
 *
 *	@throw	ActivException
 */
inline const UtcTime operator+(const UtcTime &lhs, const BaseTime::Duration &duration)
{
	return UtcTime(lhs) += duration;
}

// ---------------------------------------------------------------------------------------------------------------------------------

/**
 *	@brief	Addition operator for the utc time object.
 *
 *	@param	lhs the left hand side value.
 *	@param	numberOfMilliseconds number of milliseconds to add to the time.
 *
 *	@return	an object containing the result of the operation.
 *
 *	@throw	ActivException
 */
inline const LocalTime operator+(const LocalTime &lhs, const uint64_t numberOfMilliseconds)
{
	return LocalTime(lhs) += numberOfMilliseconds;
}

// ---------------------------------------------------------------------------------------------------------------------------------

/**
 *	@brief	Addition operator for the utc time object.
 *
 *	@param	lhs the left hand side value.
 *	@param	duration the duration to add to the time.
 *
 *	@return	an object containing the result of the operation.
 *
 *	@throw	ActivException
 */
inline const LocalTime operator+(const LocalTime &lhs, const BaseTime::Duration &duration)
{
	return LocalTime(lhs) += duration;
}

} // namespace Activ

#endif // !defined (ACTIV_FRAMEWORK_TIME_HELPER_H)

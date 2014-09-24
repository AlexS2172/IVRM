/**
 *	@file	DateTime.h
 *
 *	@brief	Header file for the date / time class.
 *
 *	$Log: $
 */

#if !defined (ACTIV_DATE_TIME_H)
#define ACTIV_DATE_TIME_H

#include "ActivMiddleware/ActivFieldTypes/External.h"
#include "ActivMiddleware/ActivFieldTypes/IFieldType.h"
#include "ActivMiddleware/ActivFieldTypes/Date.h"
#include "ActivMiddleware/ActivFieldTypes/Time.h"

#include "ActivMiddleware/ActivBase/StatusCodeException.h"

namespace Activ
{

class MessageBuilder;
class MessageValidator;

/**
 *	@brief	Date / time class.
 */
class DateTime : public IFieldType
{
public:
	static const FieldType FIELD_TYPE = FIELD_TYPE_DATE_TIME; ///< The field type.

	using IFieldType::FromString;
	using IFieldType::Serialize;
	using IFieldType::SerializeBody;
	using IFieldType::SerializeLengthAndBody;
	using IFieldType::Deserialize;
	using IFieldType::DeserializeBody;
	using IFieldType::DeserializeLengthAndBody;

	/**
	 *	@brief	Default constructor.
	 */
	DateTime();

	/**
	 *	@brief	Constructor.
	 *
	 *	@param	year the year.
	 *	@param	month the month.
	 *	@param	day the day.
	 *	@param	hour the hour.
	 *	@param	minute the minute.
	 *	@param	second the second.
	 *
	 *	@throw	StatusCodeException
	 */
	DateTime(const uint32_t year, const uint32_t month, const uint32_t day, const uint32_t hour, const uint32_t minute, const uint32_t second);

	/**
	 *	@brief	Constructor.
	 *
	 *	@param	julianDateTime the julian date / time.
	 *
	 *	@throw	StatusCodeException
	 */
	DateTime(const int64_t julianDateTime);

	/**
	 *	@brief	Constructor.
	 *
	 *	Uses the tm_year, tm_mon, tm_mday, tm_hour, tm_min and tm_sec fields of the provided tm structure.
	 *
	 *	@param	tm reference to a tm structure specifying the time.
	 *
	 *	@throw	StatusCodeException
	 */
	DateTime(const std::tm &tm);

	/**
	 *	@brief	Constructor.
	 *
	 *	@param	date the date.
	 *	@param	time the time.
	 *
	 *	@throw	StatusCodeException
	 */
	DateTime(const Date &date, const Time &time);

	/**
	 *	@brief	Destructor.
	 */
	virtual ~DateTime();

	/**
	 *	@brief	Date conversion operator.
	 */
	operator const Date&() const;

	/**
	 *	@brief	Time conversion operator.
	 */
	operator const Time&() const;

	/**
	 *	@brief	Equality operator.
	 *
	 *	@param	rhs the object to compare with.
	 *
	 *	@return	the result of the test.
	 *
	 *	@throw	StatusCodeException
	 */
	bool operator==(const DateTime &rhs) const;

	/**
	 *	@brief	Inequality operator.
	 *
	 *	@param	rhs the object to compare with.
	 *
	 *	@return	the result of the test.
	 *
	 *	@throw	StatusCodeException
	 */
	bool operator!=(const DateTime &rhs) const;

	/**
	 *	@brief	Less than operator.
	 *
	 *	@param	rhs the object to compare with.
	 *
	 *	@return	the result of the test.
	 *
	 *	@throw	StatusCodeException
	 */
	bool operator<(const DateTime &rhs) const;

	/**
	 *	@brief	Less than or equal to operator.
	 *
	 *	@param	rhs the object to compare with.
	 *
	 *	@return	the result of the test.
	 *
	 *	@throw	StatusCodeException
	 */
	bool operator<=(const DateTime &rhs) const;

	/**
	 *	@brief	Greater than operator.
	 *
	 *	@param	rhs the object to compare with.
	 *
	 *	@return	the result of the test.
	 *
	 *	@throw	StatusCodeException
	 */
	bool operator>(const DateTime &rhs) const;

	/**
	 *	@brief	Greater than or equal to operator.
	 *
	 *	@param	rhs the object to compare with.
	 *
	 *	@return	the result of the test.
	 *
	 *	@throw	StatusCodeException
	 */
	bool operator>=(const DateTime &rhs) const;

	/**
	 *	@brief	Prefix increment operator.
	 *
	 *	@return	Reference to the updated object.
	 *
	 *	@throw	StatusCodeException
	 */
	DateTime &operator++();

	/**
	 *	@brief	Postfix increment operator.
	 *
	 *	@return	Copy of the original object.
	 *
	 *	@throw	StatusCodeException
	 */
	const DateTime operator++(int);

	/**
	 *	@brief	Addition operator.
	 *
	 *	@param	nSeconds the number of seconds to add to this object.
	 *
	 *	@return	an object containing the result of the operation.
	 *
	 *	@throw	StatusCodeException
	 */
	const DateTime operator+(const int32_t nSeconds) const;

	/**
	 *	@brief	Addition assignment operator.
	 *
	 *	@param	nSeconds the number of seconds to add to this object.
	 *
	 *	@return	a reference to this object.
	 *
	 *	@throw	StatusCodeException
	 */
	DateTime& operator+=(const int32_t nSeconds);

	/**
	 *	@brief	Prefix decrement operator.
	 *
	 *	@return	Reference to the updated object.
	 *
	 *	@throw	StatusCodeException
	 */
	DateTime &operator--();

	/**
	 *	@brief	Postfix decrement operator.
	 *
	 *	@return	Copy of the original object.
	 *
	 *	@throw	StatusCodeException
	 */
	const DateTime operator--(int);

	/**
	 *	@brief	Subtraction operator.
	 *
	 *	@param	nSeconds the number of seconds to subtract from this object.
	 *
	 *	@return	an object containing the result of the operation.
	 *
	 *	@throw	StatusCodeException
	 */
	const DateTime operator-(const int32_t nSeconds) const;

	/**
	 *	@brief	Subtraction assignment operator.
	 *
	 *	@param	nSeconds the number of seconds to subtract from this object.
	 *
	 *	@return	a reference to this object.
	 *
	 *	@throw	StatusCodeException
	 */
	DateTime& operator-=(const int32_t nSeconds);

	/**
	 *	@brief	Store a new date / time.
	 *
	 *	@param	year the year.
	 *	@param	month the month.
	 *	@param	day the day.
	 *	@param	hour the hour.
	 *	@param	minute the minute.
	 *	@param	second the second.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 */
	StatusCode Set(const uint32_t year, const uint32_t month, const uint32_t day, const uint32_t hour, const uint32_t minute, const uint32_t second);

	/**
	 *	@brief	Store a new date / time.
	 *
	 *	@param	julianDateAndTime the julian date and time.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 */
	StatusCode Set(const int64_t julianDateAndTime);

	/**
	 *	@brief	Store a new date / time.
	 *
	 *	Uses the tm_year, tm_mon, tm_mday, tm_hour, tm_min and tm_sec fields of the provided tm structure.
	 *
	 *	@param	tm reference to a tm structure specifying the date and time.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 */
	StatusCode Set(const std::tm &tm);

	/**
	 *	@brief	Store a new date / time.
	 *
	 *	@param	date the date.
	 *	@param	time the time.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 */
	StatusCode Set(const Date &date, const Time &time);

	/**
	 *	@brief	Get the date / time.
	 *
	 *	@param	year the year.
	 *	@param	month the month.
	 *	@param	day the day.
	 *	@param	hour the hour.
	 *	@param	minute the minute.
	 *	@param	second the second.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_NOT_INITIALIZED
	 */
	StatusCode Get(uint32_t &year, uint32_t &month, uint32_t &day, uint32_t &hour, uint32_t &minute, uint32_t &second) const;

	/**
	 *	@brief	Get the julian date / time.
	 *
	 *	@retval	the julian date / time.
	 *
	 *	@throw	StatusCodeException
	 */
	int64_t Get() const;

	/**
	 *	@brief	Get the date / time.
	 *
	 *	Only sets the tm_year, tm_mon, tm_mday, tm_hour, tm_min and tm_sec fields of the provided tm structure.
	 *
	 *	@param	tm reference to a tm structure to receive the date / time.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_NOT_INITIALIZED
	 */
	StatusCode Get(std::tm &tm) const;

	/**
	 *	@brief	Get the date.
	 *
	 *	@return	the date.
	 *
	 *	@throw	StatusCodeException
	 */
	const Date &GetDate() const;

	/**
	 *	@brief	Get the time.
	 *
	 *	@return	the time.
	 *
	 *	@throw	StatusCodeException
	 */
	const Time &GetTime() const;

	/**
	 *	@brief	Get the year.
	 *
	 *	@return	the year.
	 *
	 *	@throw	StatusCodeException
	 */
	uint32_t GetYear() const;

	/**
	 *	@brief	Get the month.
	 *
	 *	@return	the month.
	 *
	 *	@throw	StatusCodeException
	 */
	uint32_t GetMonth() const;

	/**
	 *	@brief	Get the day.
	 *
	 *	@return	the day.
	 *
	 *	@throw	StatusCodeException
	 */
	uint32_t GetDay() const;

	/**
	 *	@brief	Get the day of the week.
	 *
	 *	@return	the day of the week.
	 *
	 *	@throw	StatusCodeException
	 */
	uint32_t GetDayOfWeek() const;

	/**
	 *	@brief	Get the day of the year.
	 *
	 *	@return	the day of the year.
	 *
	 *	@throw	StatusCodeException
	 */
	uint32_t GetDayOfYear() const;

	/**
	 *	@brief	Get the week of the year.
	 *
	 *	@return	the week of the year.
	 *
	 *	@throw	StatusCodeException
	 */
	uint32_t GetWeekOfYear() const;

	/**
	 *	@brief	Get the hour.
	 *
	 *	@return	the hour.
	 *
	 *	@throw	StatusCodeException
	 */
	uint32_t GetHour() const;

	/**
	 *	@brief	Get the minute.
	 *
	 *	@return	the minute.
	 *
	 *	@throw	StatusCodeException
	 */
	uint32_t GetMinute() const;

	/**
	 *	@brief	Get the second.
	 *
	 *	@return	the second.
	 *
	 *	@throw	StatusCodeException
	 */
	uint32_t GetSecond() const;

	/**
	 *	@brief	Add days.
	 *
	 *	@param	nDays the number of days to add to this object.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_NOT_INITIALIZED
	 */
	ACTIV_FIELD_TYPES_API StatusCode AddDays(const int32_t nDays);

	/**
	 *	@brief	Add seconds.
	 *
	 *	@param	nSeconds the number of seconds to add to this object.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_NOT_INITIALIZED
	 */
	ACTIV_FIELD_TYPES_API StatusCode AddSeconds(const int32_t nSeconds);

	/**
	 *	@brief	Add time.
	 *
	 *	@param	rhs the value to add to this object.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_NOT_INITIALIZED
	 */
	ACTIV_FIELD_TYPES_API StatusCode AddTime(const Time &rhs);

	/**
	 *	@brief	Subtract days.
	 *
	 *	@param	nDays the number of days to subtract from this object.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_NOT_INITIALIZED
	 */
	ACTIV_FIELD_TYPES_API StatusCode SubtractDays(const int32_t nDays);

	/**
	 *	@brief	Subtract seconds.
	 *
	 *	@param	nSeconds the number of seconds to subtract from this object.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_NOT_INITIALIZED
	 */
	ACTIV_FIELD_TYPES_API StatusCode SubtractSeconds(const int32_t nSeconds);

	/**
	 *	@brief	Subtract time.
	 *
	 *	@param	rhs the value to subtract from this object.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_NOT_INITIALIZED
	 */
	ACTIV_FIELD_TYPES_API StatusCode SubtractTime(const Time &rhs);

	/**
	 *	@brief	Reset the object.
	 */
	virtual void Reset();

	/**
	 *	@brief	Clear the object.
	 */
	virtual void Clear();

	/**
	 *	@brief	Compare.
	 *
	 *	@param	rhs the object to compare with.
	 *	@param	result the result of the comparison.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 */
	ACTIV_FIELD_TYPES_API StatusCode Compare(const DateTime &rhs, int &result) const;

	/**
	 *	@brief	Compare.
	 *
	 *	@param	pRhsSerializedBody the buffer containing the rhs serialized version of the object's body in.
	 *	@param	rhsSerializedBodyLength the rhs serialized body length.
	 *	@param	result the result of the comparison.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
	ACTIV_FIELD_TYPES_API virtual StatusCode Compare(const void * const pRhsSerializedBody, const size_t rhsSerializedBodyLength, int &result) const;

	/**
	 *	@brief	Convert another IFieldType object to this type.
	 *
	 *	@param	pIFieldType the value to convert from.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
	ACTIV_FIELD_TYPES_API virtual StatusCode Convert(const IFieldType * const pIFieldType);

	/**
	 *	@brief	Convert the object to a string.
	 *
	 *	@return	the object as a string.
	 */
	ACTIV_FIELD_TYPES_API virtual std::string ToString() const;

	/**
	 *	@brief	Convert the object from a string.
	 *
	 *	@param	messageValidator the message validator.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_LENGTH
	 *	@retval	STATUS_CODE_INVALID_FORMAT
	 *	@retval	...
	 */
	ACTIV_FIELD_TYPES_API virtual StatusCode FromString(MessageValidator &messageValidator);

	/**
	 *	@brief	Serialize the object into the supplied message builder.
	 *
	 *	@param	messageBuilder the message builder to store a serialized version of the object in.
	 *	@param	maxSerializedBodyLength the maximum serialized body length.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_NOT_INITIALIZED
	 *	@retval	...
	 */
	ACTIV_FIELD_TYPES_API virtual StatusCode Serialize(MessageBuilder &messageBuilder, const size_t maxSerializedBodyLength) const;

	/**
	 *	@brief	Serialize the object's body into the supplied data buffer.
	 *
	 *	@param	pSerializedBody the buffer to store a serialized version of the object's body in.
	 *	@param	maxSerializedBodyLength the maximum serialized body length.
	 *	@param	serializedBodyLength the serialized body length.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_NOT_INITIALIZED
	 *	@retval	...
	 */
	ACTIV_FIELD_TYPES_API virtual StatusCode SerializeBody(void *pSerializedBody, const size_t maxSerializedBodyLength, size_t &serializedBodyLength) const;

	/**
	 *	@brief	Serialize the object's length and body into the supplied message builder.
	 *
	 *	@param	messageBuilder the message builder to store a serialized version of the object in.
	 *	@param	maxSerializedBodyLength the maximum serialized body length.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_NOT_INITIALIZED
	 *	@retval	...
	 */
	ACTIV_FIELD_TYPES_API virtual StatusCode SerializeLengthAndBody(MessageBuilder &messageBuilder, const size_t maxSerializedBodyLength = MAX_SERIALIZED_BODY_LENGTH_UNDEFINED) const;

	/**
	 *	@brief	Deserialize the object from the supplied message validator.
	 *
	 *	@param	messageValidator the message validator from which the object will be extracted.
	 *	@param	maxSerializedBodyLength the maximum serialized body length.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	...
	 */
	ACTIV_FIELD_TYPES_API virtual StatusCode Deserialize(MessageValidator &messageValidator, const size_t maxSerializedBodyLength);

	/**
	 *	@brief	Deserialize the object's body from the supplied data buffer.
	 *
	 *	@param	pSerializedBody the buffer from which the object's body will be extracted.
	 *	@param	serializedBodyLength the serialized body length.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	...
	 */
	ACTIV_FIELD_TYPES_API virtual StatusCode DeserializeBody(const void * const pSerializedBody, const size_t serializedBodyLength);

	/**
	 *	@brief	Deserialize the object's length and body from the supplied message validator.
	 *
	 *	@param	messageValidator the message validator from which the object will be extracted.
	 *	@param	maxSerializedBodyLength the maximum serialized body length.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	...
	 */
	ACTIV_FIELD_TYPES_API virtual StatusCode DeserializeLengthAndBody(MessageValidator &messageValidator, const size_t maxSerializedBodyLength = MAX_SERIALIZED_BODY_LENGTH_UNDEFINED);

	/**
	 *	@brief	Get the serialized length of the object.
	 *
	 *	@return	the serialized length of the object.
	 */
	virtual size_t GetSerializedLength() const;

	/**
	 *	@brief	Get the serialized length of the object's body.
	 *
	 *	@return	the serialized length of the object's body.
	 */
	virtual size_t GetSerializedBodyLength() const;

	/**
	 *	@brief	Is different.
	 *
	 *	@param	pRhsSerializedBody the buffer containing the rhs serialized version of the object's body in.
	 *	@param	rhsSerializedBodyLength the rhs serialized body length.
	 *	@param	isDifferent indicates whether the two objects are different.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
	ACTIV_FIELD_TYPES_API virtual StatusCode IsDifferent(const void * const pRhsSerializedBody, const size_t rhsSerializedBodyLength, bool &isDifferent) const;

	/**
	 *	@brief	Get the maximum serialized length of the object.
	 *
	 *	@return	the maximum serialized length of the object.
	 */
	static size_t GetMaxSerializedLength();

	/**
	 *	@brief	Get the maximum serialized length of the object's body.
	 *
	 *	@return	the maximum serialized length of the object's body.
	 */
	static size_t GetMaxSerializedBodyLength();

	/**
	 *	@brief	Compare two objects serialized bodies.
	 *
	 *	@param	pLhsSerializedBody the buffer containing the lhs serialized version of the object's body in.
	 *	@param	lhsSerializedBodyLength the lhs serialized body length.
	 *	@param	pRhsSerializedBody the buffer containing the rhs serialized version of the object's body in.
	 *	@param	rhsSerializedBodyLength the rhs serialized body length.
	 *	@param	result the result of the comparison.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 */
	ACTIV_FIELD_TYPES_API static StatusCode Compare(const void * const pLhsSerializedBody, const size_t lhsSerializedBodyLength, const void * const pRhsSerializedBody, const size_t rhsSerializedBodyLength, int &result);

	/**
	 *	@brief	Get the time zone offset (local from UTC).
	 *
	 *	@return	the time zone offset.
	 */
	ACTIV_FIELD_TYPES_API static int32_t GetTimeZoneOffset();

	/**
	 *	@brief	Create an object intitialised with the current UTC date / time.
	 *
	 *	@return	the initialized object.
	 */
	ACTIV_FIELD_TYPES_API static DateTime CreateUtcDateTime();

	/**
	 *	@brief	Create an object intitialised with the current local date / time.
	 *
	 *	@return	the initialized object.
	 */
	ACTIV_FIELD_TYPES_API static DateTime CreateLocalDateTime();

	/**
	 *	@brief	Convert julian to gregorian date / time. 
	 *
	 *	@param	julianDateTime the julian date / time.
	 *	@param	year the year.
	 *	@param	month the month.
	 *	@param	day the day.
	 *	@param	hour the hour.
	 *	@param	minute the minute.
	 *	@param	second the second.
	 */
	ACTIV_FIELD_TYPES_API static void ConvertJulianToGregorianDateTime(const int64_t julianDateTime, int32_t &year, int32_t &month, int32_t &day, int32_t &hour, int32_t &minute, int32_t &second);

	/**
	 *	@brief	Convert gregorian to julian date / time.
	 *
	 *	@param	year the year.
	 *	@param	month the month.
	 *	@param	day the day.
	 *	@param	hour the hour.
	 *	@param	minute the minute.
	 *	@param	second the second.
	 *
	 *	@return	the julian date / time.
	 */
	ACTIV_FIELD_TYPES_API static int64_t ConvertGregorianToJulianDateTime(const int32_t year, const int32_t month, const int32_t day, const int32_t hour, const int32_t minute, const int32_t second);

	/**
	 *	@brief	Get the julian date. 
	 *
	 *	@param	julianDateTime the julian date / time.
	 *
	 *	@return	the julian date.
	 */
	ACTIV_FIELD_TYPES_API static int32_t GetJulianDate(const int64_t julianDateTime);

	/**
	 *	@brief	Get the julian time. 
	 *
	 *	@param	julianDateTime the julian date / time.
	 *
	 *	@return	the julian time.
	 */
	ACTIV_FIELD_TYPES_API static int32_t GetJulianTime(const int64_t julianDateTime);

	/**
	 *	@brief	Validate the serialized length and body.
	 *
	 *	@param	messageValidator the message validator.
	 *	@param	serializedBodyOffset the serialized body offset.
	 *	@param	serializedBodyLength the serialized body length.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
	ACTIV_FIELD_TYPES_API static StatusCode ValidateSerializedLengthAndBody(MessageValidator &messageValidator, size_t &serializedBodyOffset, size_t &serializedBodyLength);

	/**
	 *	@brief	Is valid serialized body length.
	 *
	 *	@param	serializedBodyLength the serialized body length.
	 *
	 *	@return	whether the serialized body length is valid.
	 */
	static bool IsValidSerializedBodyLength(const size_t serializedBodyLength);

private:
	Date	m_date;		///< The date.
	Time	m_time;		///< The time.
};

// ---------------------------------------------------------------------------------------------------------------------------------

inline DateTime::DateTime() :
	IFieldType(FIELD_TYPE)
{
	m_isInitialized = false;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline DateTime::DateTime(const uint32_t year, const uint32_t month, const uint32_t day, const uint32_t hour, const uint32_t minute, const uint32_t second) :
	IFieldType(FIELD_TYPE),
	m_date(year, month, day),
	m_time(hour, minute, second)
{
	m_isInitialized = true;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline DateTime::DateTime(const int64_t julianDateTime) :
	IFieldType(FIELD_TYPE),
	m_date(GetJulianDate(julianDateTime)),
	m_time(GetJulianTime(julianDateTime))
{
	m_isInitialized = true;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline DateTime::DateTime(const std::tm &tm) :
	IFieldType(FIELD_TYPE),
	m_date(tm),
	m_time(tm)
{
	m_isInitialized = true;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline DateTime::DateTime(const Date &date, const Time &time) :
	IFieldType(FIELD_TYPE),
	m_date(date),
	m_time(time)
{
	if (m_date.IsInitialized() != m_time.IsInitialized())
		ACTIV_THROW(StatusCodeException, STATUS_CODE_INVALID_PARAMETER);

	m_isInitialized = m_date.IsInitialized();
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline DateTime::~DateTime()
{
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline DateTime::operator const Date&() const
{
	return m_date;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline DateTime::operator const Time&() const
{
	return m_time;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline bool DateTime::operator==(const DateTime &rhs) const
{
	int result;
	ACTIV_THROW_RESULT_IF(STATUS_CODE_SUCCESS, !=, Compare(rhs, result));

	return (0 == result);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline bool DateTime::operator!=(const DateTime &rhs) const
{
	return !((*this) == rhs);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline bool DateTime::operator<(const DateTime &rhs) const
{
	int result;
	ACTIV_THROW_RESULT_IF(STATUS_CODE_SUCCESS, !=, Compare(rhs, result));

	return (result < 0);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline bool DateTime::operator<=(const DateTime &rhs) const
{
	return !((*this) > rhs);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline bool DateTime::operator>(const DateTime &rhs) const
{
	int result;
	ACTIV_THROW_RESULT_IF(STATUS_CODE_SUCCESS, !=, Compare(rhs, result));

	return (result > 0);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline bool DateTime::operator>=(const DateTime &rhs) const
{
	return !((*this) < rhs);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline DateTime &DateTime::operator++()
{
	return (*this) += 1;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline const DateTime DateTime::operator++(int)
{
	const DateTime oldValue = *this;

	++(*this);

	return oldValue;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline const DateTime DateTime::operator+(const int32_t nSeconds) const
{
	return DateTime(*this) += nSeconds;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline DateTime& DateTime::operator+=(const int32_t nSeconds)
{
	ACTIV_THROW_RESULT_IF(STATUS_CODE_SUCCESS, !=, AddSeconds(nSeconds));

	return (*this);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline DateTime &DateTime::operator--()
{
	return (*this) -= 1;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline const DateTime DateTime::operator--(int)
{
	const DateTime oldValue = *this;

	--(*this);

	return oldValue;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline const DateTime DateTime::operator-(const int32_t nSeconds) const
{
	return DateTime(*this) -= nSeconds;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline DateTime& DateTime::operator-=(const int32_t nSeconds)
{
	ACTIV_THROW_RESULT_IF(STATUS_CODE_SUCCESS, !=, SubtractSeconds(nSeconds));

	return (*this);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode DateTime::Set(const uint32_t year, const uint32_t month, const uint32_t day, const uint32_t hour, const uint32_t minute, const uint32_t second)
{
	Date date;
	ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, date.Set(year, month, day));

	Time time;
	ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, time.Set(hour, minute, second));

	return Set(date, time);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode DateTime::Set(const int64_t julianDateTime)
{
	Date date;
	ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, date.Set(GetJulianDate(julianDateTime)));

	Time time;
	ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, time.Set(GetJulianTime(julianDateTime)));

	return Set(date, time);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode DateTime::Set(const std::tm &tm)
{
	Date date;
	ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, date.Set(tm));

	Time time;
	ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, time.Set(tm));

	return Set(date, time);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode DateTime::Set(const Date &date, const Time &time)
{
	m_date = date;
	m_time = time;
	m_isInitialized = true;

	return STATUS_CODE_SUCCESS;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode DateTime::Get(uint32_t &year, uint32_t &month, uint32_t &day, uint32_t &hour, uint32_t &minute, uint32_t &second) const
{
	ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, m_date.Get(year, month, day));
	ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, m_time.Get(hour, minute, second));

	return STATUS_CODE_SUCCESS;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline int64_t DateTime::Get() const
{
	if (!IsInitialized())
		ACTIV_THROW(StatusCodeException, STATUS_CODE_NOT_INITIALIZED);

	return ConvertGregorianToJulianDateTime(m_date.m_year, m_date.m_month, m_date.m_day, m_time.m_hour, m_time.m_minute, m_time.m_second);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode DateTime::Get(std::tm &tm) const
{
	ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, m_date.Get(tm));
	ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, m_time.Get(tm));

	return STATUS_CODE_SUCCESS;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline const Date &DateTime::GetDate() const
{
	return m_date;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline const Time &DateTime::GetTime() const
{
	return m_time;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline uint32_t DateTime::GetYear() const
{
	return m_date.GetYear();
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline uint32_t DateTime::GetMonth() const
{
	return m_date.GetMonth();
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline uint32_t DateTime::GetDay() const
{
	return m_date.GetDay();
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline uint32_t DateTime::GetDayOfWeek() const
{
	return m_date.GetDayOfWeek();
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline uint32_t DateTime::GetDayOfYear() const
{
	return m_date.GetDayOfYear();
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline uint32_t DateTime::GetWeekOfYear() const
{
	return m_date.GetWeekOfYear();
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline uint32_t DateTime::GetHour() const
{
	return m_time.GetHour();
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline uint32_t DateTime::GetMinute() const
{
	return m_time.GetMinute();
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline uint32_t DateTime::GetSecond() const
{
	return m_time.GetSecond();
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline void DateTime::Reset()
{
	m_date.Reset();
	m_time.Reset();
	m_isInitialized = false;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline void DateTime::Clear()
{
	m_date.Clear();
	m_time.Clear();
	m_isInitialized = false;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline size_t DateTime::GetSerializedLength() const
{
	return (m_date.GetSerializedLength() + m_time.GetSerializedLength());
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline size_t DateTime::GetSerializedBodyLength() const
{
	return (m_date.GetSerializedBodyLength() + m_time.GetSerializedBodyLength());
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline size_t DateTime::GetMaxSerializedLength()
{
	return (Date::GetMaxSerializedLength() + Time::GetMaxSerializedLength());
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline size_t DateTime::GetMaxSerializedBodyLength()
{
	return (Date::GetMaxSerializedBodyLength() + Time::GetMaxSerializedBodyLength());
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline bool DateTime::IsValidSerializedBodyLength(const size_t serializedBodyLength)
{
	return (GetMaxSerializedBodyLength() == serializedBodyLength);
}

} // namespace Activ

#endif // !defined (ACTIV_DATE_TIME_H)

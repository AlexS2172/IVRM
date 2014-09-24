/**
 *	@file	Time.h
 *
 *	@brief	Header file for the time class.
 *
 *	$Log: $
 */

#if !defined (ACTIV_TIME_H)
#define ACTIV_TIME_H

#include "ActivMiddleware/ActivFieldTypes/External.h"
#include "ActivMiddleware/ActivFieldTypes/IFieldType.h"

#include "ActivMiddleware/ActivBase/ActivException.h"

#include <ctime>

namespace Activ
{

class MessageBuilder;
class MessageValidater;

/**
 *	@brief	Time class.
 */
class Time : public IFieldType
{
public:
	static const FieldType FIELD_TYPE = FIELD_TYPE_TIME; ///< The field type.

	static const uint32_t	HOURS_PER_DAY		= 24;										///< The number of hours in a day.
	static const uint32_t	MINUTES_PER_HOUR	= 60;										///< The number of minutes in an hour.
	static const uint32_t	MINUTES_PER_DAY		= (HOURS_PER_DAY * MINUTES_PER_HOUR);		///< The number of minutes in a day.
	static const uint32_t	SECONDS_PER_MINUTE	= 60;										///< The number of seconds in a minute.
	static const uint32_t	SECONDS_PER_HOUR	= (MINUTES_PER_HOUR * SECONDS_PER_MINUTE);	///< The number of seconds in an hour.
	static const uint32_t	SECONDS_PER_DAY		= (MINUTES_PER_DAY * SECONDS_PER_MINUTE);	///< The number of seconds in a day.

	using IFieldType::FromString;

	/**
	 *	@brief	Default constructor.
	 */
	Time();

	/**
	 *	@brief	Constructor.
	 *
	 *	@param	hour the hour.
	 *	@param	minute the minute.
	 *	@param	second the second.
	 *
	 *	@throw	ActivException
	 */
	Time(const uint32_t hour, const uint32_t minute, const uint32_t second);

	/**
	 *	@brief	Constructor.
	 *
	 *	@param	julianTime the julian time.
	 *
	 *	@throw	ActivException
	 */
	Time(const int32_t julianTime);

	/**
	 *	@brief	Constructor.
	 *
	 *	Uses the tm_hour, tm_min and tm_sec fields of the provided tm structure.
	 *
	 *	@param	tm reference to a tm structure specifying the time.
	 *
	 *	@throw	ActivException
	 */
	Time(const std::tm &tm);

	/**
	 *	@brief	Destructor.
	 */
	virtual ~Time();

	/**
	 *	@brief	Equality operator.
	 *
	 *	@param	rhs the object to compare with.
	 *
	 *	@return	the result of the test.
	 *
	 *	@throw	ActivException
	 */
	bool operator==(const Time &rhs) const;

	/**
	 *	@brief	Inequality operator.
	 *
	 *	@param	rhs the object to compare with.
	 *
	 *	@return	the result of the test.
	 *
	 *	@throw	ActivException
	 */
	bool operator!=(const Time &rhs) const;

	/**
	 *	@brief	Less than operator.
	 *
	 *	@param	rhs the object to compare with.
	 *
	 *	@return	the result of the test.
	 *
	 *	@throw	ActivException
	 */
	bool operator<(const Time &rhs) const;

	/**
	 *	@brief	Less than or equal to operator.
	 *
	 *	@param	rhs the object to compare with.
	 *
	 *	@return	the result of the test.
	 *
	 *	@throw	ActivException
	 */
	bool operator<=(const Time &rhs) const;

	/**
	 *	@brief	Greater than operator.
	 *
	 *	@param	rhs the object to compare with.
	 *
	 *	@return	the result of the test.
	 *
	 *	@throw	ActivException
	 */
	bool operator>(const Time &rhs) const;

	/**
	 *	@brief	Greater than or equal to operator.
	 *
	 *	@param	rhs the object to compare with.
	 *
	 *	@return	the result of the test.
	 *
	 *	@throw	ActivException
	 */
	bool operator>=(const Time &rhs) const;

	/**
	 *	@brief	Prefix increment operator.
	 *
	 *	@return	Reference to the updated object.
	 *
	 *	@throw	ActivException
	 */
	Time &operator++();

	/**
	 *	@brief	Postfix increment operator.
	 *
	 *	@return	Copy of the original object.
	 *
	 *	@throw	ActivException
	 */
	const Time operator++(int);

	/**
	 *	@brief	Addition operator.
	 *
	 *	@param	rhs the value to add to this object.
	 *
	 *	@return	an object containing the result of the operation.
	 *
	 *	@throw	ActivException
	 */
	const Time operator+(const Time &rhs) const;

	/**
	 *	@brief	Addition operator.
	 *
	 *	@param	nSeconds the number of seconds to add to this object.
	 *
	 *	@return	an object containing the result of the operation.
	 *
	 *	@throw	ActivException
	 */
	const Time operator+(const int32_t nSeconds) const;

	/**
	 *	@brief	Addition assignment operator.
	 *
	 *	@param	rhs the value to add to this object.
	 *
	 *	@return	a reference to this object.
	 *
	 *	@throw	ActivException
	 */
	Time &operator+=(const Time &rhs);

	/**
	 *	@brief	Addition assignment operator.
	 *
	 *	@param	nSeconds the number of seconds to add to this object.
	 *
	 *	@return	a reference to this object.
	 *
	 *	@throw	ActivException
	 */
	Time &operator+=(const int32_t nSeconds);

	/**
	 *	@brief	Prefix decrement operator.
	 *
	 *	@return	Reference to the updated object.
	 *
	 *	@throw	ActivException
	 */
	Time &operator--();

	/**
	 *	@brief	Postfix decrement operator.
	 *
	 *	@return	Copy of the original object.
	 *
	 *	@throw	ActivException
	 */
	const Time operator--(int);

	/**
	 *	@brief	Subtraction operator.
	 *
	 *	@param	rhs the value to subtract from this object.
	 *
	 *	@return	an object containing the result of the operation.
	 *
	 *	@throw	ActivException
	 */
	const Time operator-(const Time &rhs) const;

	/**
	 *	@brief	Subtraction operator.
	 *
	 *	@param	nSeconds the number of seconds to subtract from this object.
	 *
	 *	@return	an object containing the result of the operation.
	 *
	 *	@throw	ActivException
	 */
	const Time operator-(const int32_t nSeconds) const;

	/**
	 *	@brief	Subtraction assignment operator.
	 *
	 *	@param	rhs the value to subtract from this object.
	 *
	 *	@return	a reference to this object.
	 *
	 *	@throw	ActivException
	 */
	Time &operator-=(const Time &rhs);

	/**
	 *	@brief	Subtraction assignment operator.
	 *
	 *	@param	nSeconds the number of seconds to subtract from this object.
	 *
	 *	@return	a reference to this object.
	 *
	 *	@throw	ActivException
	 */
	Time &operator-=(const int32_t nSeconds);

	/**
	 *	@brief	Store a new time.
	 *
	 *	@param	hour the hour.
	 *	@param	minute the minute.
	 *	@param	second the second.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 */
	StatusCode Set(const uint32_t hour, const uint32_t minute, const uint32_t second);

	/**
	 *	@brief	Store a new time.
	 *
	 *	@param	julianTime the julian time.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 */
	StatusCode Set(const int32_t julianTime);

	/**
	 *	@brief	Store a new time.
	 *
	 *	Uses the tm_hour, tm_min and tm_sec fields of the provided tm structure.
	 *
	 *	@param	tm reference to a tm structure specifying the time.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 */
	StatusCode Set(const std::tm &tm);

	/**
	 *	@brief	Get the time.
	 *
	 *	@param	hour the hour.
	 *	@param	minute the minute.
	 *	@param	second the second.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_NOT_INITIALIZED
	 */
	StatusCode Get(uint32_t &hour, uint32_t &minute, uint32_t &second) const;

	/**
	 *	@brief	Get the julian time.
	 *
	 *	@return	the julian time.
	 *
	 *	@throw	ActivException
	 */
	int32_t Get() const;

	/**
	 *	@brief	Get the time.
	 *
	 *	Only sets the tm_hour, tm_min and tm_sec fields of the provided tm structure.
	 *
	 *	@param	tm reference to a tm structure to receive the time.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_NOT_INITIALIZED
	 */
	StatusCode Get(std::tm &tm) const;

	/**
	 *	@brief	Get the hour.
	 *
	 *	@return	the hour.
	 *
	 *	@throw	ActivException
	 */
	uint32_t GetHour() const;

	/**
	 *	@brief	Get the minute.
	 *
	 *	@return	the minute.
	 *
	 *	@throw	ActivException
	 */
	uint32_t GetMinute() const;

	/**
	 *	@brief	Get the second.
	 *
	 *	@return	the second.
	 *
	 *	@throw	ActivException
	 */
	uint32_t GetSecond() const;

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
	ACTIV_FIELD_TYPES_API StatusCode Compare(const Time &rhs, int &result) const;

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
	 *	@param	messageValidater the message validater.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_LENGTH
	 *	@retval	...
	 */
	ACTIV_FIELD_TYPES_API virtual StatusCode FromString(MessageValidater &messageValidater);

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
	 *	@brief	Deserialize the object from the supplied message validater.
	 *
	 *	@param	messageValidater the message validater from which the object will be extracted.
	 *	@param	maxSerializedBodyLength the maximum serialized body length.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	...
	 */
	ACTIV_FIELD_TYPES_API virtual StatusCode Deserialize(MessageValidater &messageValidater, const size_t maxSerializedBodyLength);

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
	 *	@brief	Deserialize the object's length and body from the supplied message validater.
	 *
	 *	@param	messageValidater the message validater from which the object will be extracted.
	 *	@param	maxSerializedBodyLength the maximum serialized body length.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	...
	 */
	ACTIV_FIELD_TYPES_API virtual StatusCode DeserializeLengthAndBody(MessageValidater &messageValidater, const size_t maxSerializedBodyLength = MAX_SERIALIZED_BODY_LENGTH_UNDEFINED);

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
	 *	@brief	Create an object intitialised with the current UTC time.
	 *
	 *	@return	the initialized object.
	 */
	ACTIV_FIELD_TYPES_API static Time CreateUtcTime();

	/**
	 *	@brief	Create an object intitialised with the current local time.
	 *
	 *	@return	the initialized object.
	 */
	ACTIV_FIELD_TYPES_API static Time CreateLocalTime();

	/**
	 *	@brief	Convert julian to gregorian time.
	 *
	 *	@param	julianTime the julian time.
	 *	@param	hour the hour.
	 *	@param	minute the minute.
	 *	@param	second the second.
	 */
	ACTIV_FIELD_TYPES_API static void ConvertJulianToGregorianTime(const int32_t julianTime, int32_t &hour, int32_t &minute, int32_t &second);

	/**
	 *	@brief	Convert gregorian to julian time.
	 *
	 *	@param	hour the hour.
	 *	@param	minute the minute.
	 *	@param	second the second.
	 *
	 *	@return	the julian time.
	 */
	ACTIV_FIELD_TYPES_API static int32_t ConvertGregorianToJulianTime(const int32_t hour, const int32_t minute, const int32_t second);

	/**
	 *	@brief	Validate the serialized length and body.
	 *
	 *	@param	messageValidater the message validater.
	 *	@param	serializedBodyOffset the serialized body offset.
	 *	@param	serializedBodyLength the serialized body length.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
	ACTIV_FIELD_TYPES_API static StatusCode ValidateSerializedLengthAndBody(MessageValidater &messageValidater, size_t &serializedBodyOffset, size_t &serializedBodyLength);

	/**
	 *	@brief	Is valid serialized body length.
	 *
	 *	@param	serializedBodyLength the serialized body length.
	 *
	 *	@return	whether the serialized body length is valid.
	 */
	static bool IsValidSerializedBodyLength(const size_t serializedBodyLength);

private:
	uint8_t		m_hour;		///< The hour.
	uint8_t		m_minute;	///< The minute.
	uint8_t		m_second;	///< The second.

	friend class DateTime;
};

// ---------------------------------------------------------------------------------------------------------------------------------

inline Time::Time() :
	IFieldType(FIELD_TYPE)
{
	m_isInitialized = false;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline Time::Time(const uint32_t hour, const uint32_t minute, const uint32_t second) :
	IFieldType(FIELD_TYPE)
{
	ACTIV_THROW_RESULT_IF(STATUS_CODE_SUCCESS, !=, Set(hour, minute, second));
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline Time::Time(const int32_t julianTime) :
	IFieldType(FIELD_TYPE)
{
	ACTIV_THROW_RESULT_IF(STATUS_CODE_SUCCESS, !=, Set(julianTime));
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline Time::Time(const std::tm &tm) :
	IFieldType(FIELD_TYPE)
{
	ACTIV_THROW_RESULT_IF(STATUS_CODE_SUCCESS, !=, Set(tm));
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline Time::~Time()
{
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline bool Time::operator==(const Time &rhs) const
{
	int result;
	ACTIV_THROW_RESULT_IF(STATUS_CODE_SUCCESS, !=, Compare(rhs, result));

	return (0 == result);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline bool Time::operator!=(const Time &rhs) const
{
	return !((*this) == rhs);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline bool Time::operator<(const Time &rhs) const
{
	int result;
	ACTIV_THROW_RESULT_IF(STATUS_CODE_SUCCESS, !=, Compare(rhs, result));

	return (result < 0);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline bool Time::operator<=(const Time &rhs) const
{
	return !((*this) > rhs);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline bool Time::operator>(const Time &rhs) const
{
	int result;
	ACTIV_THROW_RESULT_IF(STATUS_CODE_SUCCESS, !=, Compare(rhs, result));

	return (result > 0);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline bool Time::operator>=(const Time &rhs) const
{
	return !((*this) < rhs);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline Time &Time::operator++()
{
	return (*this) += 1;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline const Time Time::operator++(int)
{
	const Time oldValue = *this;

	++(*this);

	return oldValue;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline const Time Time::operator+(const Time &rhs) const
{
	return Time(*this) += rhs;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline const Time Time::operator+(const int32_t nSeconds) const
{
	return Time(*this) += nSeconds;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline Time& Time::operator+=(const Time &rhs)
{
	ACTIV_THROW_RESULT_IF(STATUS_CODE_SUCCESS, !=, AddTime(rhs));

	return (*this);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline Time& Time::operator+=(const int32_t nSeconds)
{
	ACTIV_THROW_RESULT_IF(STATUS_CODE_SUCCESS, !=, AddSeconds(nSeconds));

	return (*this);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline Time &Time::operator--()
{
	return (*this) -= 1;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline const Time Time::operator--(int)
{
	const Time oldValue = *this;

	--(*this);

	return oldValue;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline const Time Time::operator-(const Time &rhs) const
{
	return Time(*this) -= rhs;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline const Time Time::operator-(const int32_t nSeconds) const
{
	return Time(*this) -= nSeconds;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline Time& Time::operator-=(const Time &rhs)
{
	ACTIV_THROW_RESULT_IF(STATUS_CODE_SUCCESS, !=, SubtractTime(rhs));

	return (*this);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline Time& Time::operator-=(const int32_t nSeconds)
{
	ACTIV_THROW_RESULT_IF(STATUS_CODE_SUCCESS, !=, SubtractSeconds(nSeconds));

	return (*this);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode Time::Set(const uint32_t hour, const uint32_t minute, const uint32_t second)
{
	if ((hour >= HOURS_PER_DAY) || (minute >= MINUTES_PER_HOUR) || (second >= SECONDS_PER_MINUTE))
		return STATUS_CODE_INVALID_PARAMETER;

	m_hour = static_cast<uint8_t>(hour);
	m_minute = static_cast<uint8_t>(minute);
	m_second = static_cast<uint8_t>(second);
	m_isInitialized = true;

	return STATUS_CODE_SUCCESS;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode Time::Set(const int32_t julianTime)
{
	int32_t hour, minute, second;

	ConvertJulianToGregorianTime(julianTime, hour, minute, second);

	if ((hour < 0) || (hour >= static_cast<int32_t>(HOURS_PER_DAY)) || (minute < 0) || (minute >= static_cast<int32_t>(MINUTES_PER_HOUR)) || (second < 0) || (second >= static_cast<int32_t>(SECONDS_PER_MINUTE)))
		return STATUS_CODE_INVALID_PARAMETER;

	m_hour = static_cast<uint8_t>(hour);
	m_minute = static_cast<uint8_t>(minute);
	m_second = static_cast<uint8_t>(second);
	m_isInitialized = true;

	return STATUS_CODE_SUCCESS;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode Time::Set(const std::tm &tm)
{
	return Set(static_cast<uint32_t>(tm.tm_hour), static_cast<uint32_t>(tm.tm_min), static_cast<uint32_t>(tm.tm_sec));
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode Time::Get(uint32_t &hour, uint32_t &minute, uint32_t &second) const
{
	if (!IsInitialized())
		return STATUS_CODE_NOT_INITIALIZED;

	hour = m_hour;
	minute = m_minute;
	second = m_second;

	return STATUS_CODE_SUCCESS;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline int32_t Time::Get() const
{
	if (!IsInitialized())
		ACTIV_THROW(ActivException, STATUS_CODE_NOT_INITIALIZED);

	return ConvertGregorianToJulianTime(m_hour, m_minute, m_second);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode Time::Get(std::tm &tm) const
{
	if (!IsInitialized())
		return STATUS_CODE_NOT_INITIALIZED;

	tm.tm_hour = static_cast<int>(m_hour);
	tm.tm_min = static_cast<int>(m_minute);
	tm.tm_sec = static_cast<int>(m_second);

	return STATUS_CODE_SUCCESS;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline uint32_t Time::GetHour() const
{
	if (!IsInitialized())
		ACTIV_THROW(ActivException, STATUS_CODE_NOT_INITIALIZED);

	return static_cast<uint32_t>(m_hour);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline uint32_t Time::GetMinute() const
{
	if (!IsInitialized())
		ACTIV_THROW(ActivException, STATUS_CODE_NOT_INITIALIZED);

	return static_cast<uint32_t>(m_minute);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline uint32_t Time::GetSecond() const
{
	if (!IsInitialized())
		ACTIV_THROW(ActivException, STATUS_CODE_NOT_INITIALIZED);

	return static_cast<uint32_t>(m_second);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline void Time::Reset()
{
	m_isInitialized = false;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline void Time::Clear()
{
	m_isInitialized = false;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline size_t Time::GetSerializedLength() const
{
	return (IsInitialized() ? GetMaxSerializedLength() : 0);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline size_t Time::GetSerializedBodyLength() const
{
	return (IsInitialized() ? GetMaxSerializedBodyLength() : 0);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline size_t Time::GetMaxSerializedLength()
{
	return GetMaxSerializedBodyLength();
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline size_t Time::GetMaxSerializedBodyLength()
{
	return (sizeof(uint8_t) + sizeof(uint8_t) + sizeof(uint8_t));
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline bool Time::IsValidSerializedBodyLength(const size_t serializedBodyLength)
{
	return (GetMaxSerializedBodyLength() == serializedBodyLength);
}

} // namespace Activ

#endif // !defined (ACTIV_TIME_H)

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

namespace Activ
{

class MessageBuilder;
class MessageValidater;

/**
 *	@brief	Date / time class.
 */
class ACTIV_FIELD_TYPES_API DateTime : public IFieldType
{
public:
	static const FieldType FIELD_TYPE = FIELD_TYPE_DATE_TIME; ///< The field type.

	using IFieldType::FromString;

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
	 *	@throw	ActivException
	 */
	DateTime(const uint32_t year, const uint32_t month, const uint32_t day, const uint32_t hour, const uint32_t minute, const uint32_t second);

	/**
	 *	@brief	Constructor.
	 *
	 *	@param	julianDateTime the julian date / time.
	 *
	 *	@throw	ActivException
	 */
	DateTime(const int64_t julianDateTime);

	/**
	 *	@brief	Constructor.
	 *
	 *	@param	date the date.
	 *	@param	time the time.
	 *
	 *	@throw	ActivException
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
	 *	@throw	ActivException
	 */
	bool operator==(const DateTime &rhs) const;

	/**
	 *	@brief	Inequality operator.
	 *
	 *	@param	rhs the object to compare with.
	 *
	 *	@return	the result of the test.
	 *
	 *	@throw	ActivException
	 */
	bool operator!=(const DateTime &rhs) const;

	/**
	 *	@brief	Less than operator.
	 *
	 *	@param	rhs the object to compare with.
	 *
	 *	@return	the result of the test.
	 *
	 *	@throw	ActivException
	 */
	bool operator<(const DateTime &rhs) const;

	/**
	 *	@brief	Less than or equal to operator.
	 *
	 *	@param	rhs the object to compare with.
	 *
	 *	@return	the result of the test.
	 *
	 *	@throw	ActivException
	 */
	bool operator<=(const DateTime &rhs) const;

	/**
	 *	@brief	Greater than operator.
	 *
	 *	@param	rhs the object to compare with.
	 *
	 *	@return	the result of the test.
	 *
	 *	@throw	ActivException
	 */
	bool operator>(const DateTime &rhs) const;

	/**
	 *	@brief	Greater than or equal to operator.
	 *
	 *	@param	rhs the object to compare with.
	 *
	 *	@return	the result of the test.
	 *
	 *	@throw	ActivException
	 */
	bool operator>=(const DateTime &rhs) const;

	/**
	 *	@brief	Prefix increment operator.
	 *
	 *	@return	Reference to the updated object.
	 *
	 *	@throw	ActivException
	 */
	DateTime &operator++();

	/**
	 *	@brief	Postfix increment operator.
	 *
	 *	@return	Copy of the original object.
	 *
	 *	@throw	ActivException
	 */
	const DateTime operator++(int);

	/**
	 *	@brief	Addition operator.
	 *
	 *	@param	nSeconds the number of seconds to add to this object.
	 *
	 *	@return	an object containing the result of the operation.
	 *
	 *	@throw	ActivException
	 */
	const DateTime operator+(const int32_t nSeconds) const;

	/**
	 *	@brief	Addition assignment operator.
	 *
	 *	@param	nSeconds the number of seconds to add to this object.
	 *
	 *	@return	a reference to this object.
	 *
	 *	@throw	ActivException
	 */
	DateTime& operator+=(const int32_t nSeconds);

	/**
	 *	@brief	Prefix decrement operator.
	 *
	 *	@return	Reference to the updated object.
	 *
	 *	@throw	ActivException
	 */
	DateTime &operator--();

	/**
	 *	@brief	Postfix decrement operator.
	 *
	 *	@return	Copy of the original object.
	 *
	 *	@throw	ActivException
	 */
	const DateTime operator--(int);

	/**
	 *	@brief	Subtraction operator.
	 *
	 *	@param	nSeconds the number of seconds to subtract from this object.
	 *
	 *	@return	an object containing the result of the operation.
	 *
	 *	@throw	ActivException
	 */
	const DateTime operator-(const int32_t nSeconds) const;

	/**
	 *	@brief	Subtraction assignment operator.
	 *
	 *	@param	nSeconds the number of seconds to subtract from this object.
	 *
	 *	@return	a reference to this object.
	 *
	 *	@throw	ActivException
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
	 *	@throw	ActivException
	 */
	int64_t Get() const;

	/**
	 *	@brief	Get the date.
	 *
	 *	@return	the date.
	 *
	 *	@throw	ActivException
	 */
	const Date &GetDate() const;

	/**
	 *	@brief	Get the time.
	 *
	 *	@return	the time.
	 *
	 *	@throw	ActivException
	 */
	const Time &GetTime() const;

	/**
	 *	@brief	Get the year.
	 *
	 *	@return	the year.
	 *
	 *	@throw	ActivException
	 */
	uint32_t GetYear() const;

	/**
	 *	@brief	Get the month.
	 *
	 *	@return	the month.
	 *
	 *	@throw	ActivException
	 */
	uint32_t GetMonth() const;

	/**
	 *	@brief	Get the day.
	 *
	 *	@return	the day.
	 *
	 *	@throw	ActivException
	 */
	uint32_t GetDay() const;

	/**
	 *	@brief	Get the day of the week.
	 *
	 *	@return	the day of the week.
	 *
	 *	@throw	ActivException
	 */
	uint32_t GetDayOfWeek() const;

	/**
	 *	@brief	Get the day of the year.
	 *
	 *	@return	the day of the year.
	 *
	 *	@throw	ActivException
	 */
	uint32_t GetDayOfYear() const;

	/**
	 *	@brief	Get the week of the year.
	 *
	 *	@return	the week of the year.
	 *
	 *	@throw	ActivException
	 */
	uint32_t GetWeekOfYear() const;

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
	 *	@brief	Add days.
	 *
	 *	@param	nDays the number of days to add to this object.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_NOT_INITIALIZED
	 */
	StatusCode AddDays(const int32_t nDays);

	/**
	 *	@brief	Add seconds.
	 *
	 *	@param	nSeconds the number of seconds to add to this object.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_NOT_INITIALIZED
	 */
	StatusCode AddSeconds(const int32_t nSeconds);

	/**
	 *	@brief	Add time.
	 *
	 *	@param	rhs the value to add to this object.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_NOT_INITIALIZED
	 */
	StatusCode AddTime(const Time &rhs);

	/**
	 *	@brief	Subtract days.
	 *
	 *	@param	nDays the number of days to subtract from this object.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_NOT_INITIALIZED
	 */
	StatusCode SubtractDays(const int32_t nDays);

	/**
	 *	@brief	Subtract seconds.
	 *
	 *	@param	nSeconds the number of seconds to subtract from this object.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_NOT_INITIALIZED
	 */
	StatusCode SubtractSeconds(const int32_t nSeconds);

	/**
	 *	@brief	Subtract time.
	 *
	 *	@param	rhs the value to subtract from this object.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_NOT_INITIALIZED
	 */
	StatusCode SubtractTime(const Time &rhs);

	/**
	 *	@brief	Reset the object.
	 */
	virtual void Reset();

	/**
	 *	@brief	Compare.
	 *
	 *	@param	rhs the object to compare with.
	 *	@param	result the result of the comparison.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 */
	StatusCode Compare(const DateTime &rhs, int &result) const;

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
	virtual StatusCode Compare(const void * const pRhsSerializedBody, const size_t rhsSerializedBodyLength, int &result) const;

	/**
	 *	@brief	Convert another IFieldType object to this type.
	 *
	 *	@param	pIFieldType the value to convert from.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
	virtual StatusCode Convert(const IFieldType * const pIFieldType);

	/**
	 *	@brief	Convert the object to a string.
	 *
	 *	@return	the object as a string.
	 */
	virtual std::string ToString() const;

	/**
	 *	@brief	Convert the object from a string.
	 *
	 *	@param	messageValidater the message validater.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_LENGTH
	 *	@retval	STATUS_CODE_INVALID_FORMAT
	 *	@retval	...
	 */
	virtual StatusCode FromString(MessageValidater &messageValidater);

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
	virtual StatusCode Serialize(MessageBuilder &messageBuilder, const size_t maxSerializedBodyLength) const;

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
	virtual StatusCode SerializeBody(void *pSerializedBody, const size_t maxSerializedBodyLength, size_t &serializedBodyLength) const;

	/**
	 *	@brief	Serialize the object's length and body into the supplied message builder.
	 *
	 *	@param	messageBuilder the message builder to store a serialized version of the object in.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
	virtual StatusCode SerializeLengthAndBody(MessageBuilder &messageBuilder) const;

	/**
	 *	@brief	Deserialize the object from the supplied message validater.
	 *
	 *	@param	messageValidater the message validater from which the object will be extracted.
	 *	@param	maxSerializedBodyLength the maximum serialized body length.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
	virtual StatusCode Deserialize(MessageValidater &messageValidater, const size_t maxSerializedBodyLength);

	/**
	 *	@brief	Deserialize the object's body from the supplied data buffer.
	 *
	 *	@param	pSerializedBody the buffer from which the object's body will be extracted.
	 *	@param	serializedBodyLength the serialized body length.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
	virtual StatusCode DeserializeBody(const void * const pSerializedBody, const size_t serializedBodyLength);

	/**
	 *	@brief	Deserialize the object's length and body from the supplied message validater.
	 *
	 *	@param	messageValidater the message validater from which the object will be extracted.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
	virtual StatusCode DeserializeLengthAndBody(MessageValidater &messageValidater);

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
	virtual StatusCode IsDifferent(const void * const pRhsSerializedBody, const size_t rhsSerializedBodyLength, bool &isDifferent) const;

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
	static StatusCode Compare(const void * const pLhsSerializedBody, const size_t lhsSerializedBodyLength, const void * const pRhsSerializedBody, const size_t rhsSerializedBodyLength, int &result);

	/**
	 *	@brief	Get the time zone offset (local from UTC).
	 *
	 *	@return	the time zone offset.
	 */
	static int32_t GetTimeZoneOffset();

	/**
	 *	@brief	Create an object intitialised with the current UTC date / time.
	 *
	 *	@return	the initialized object.
	 */
	static DateTime CreateUtcDateTime();

	/**
	 *	@brief	Create an object intitialised with the current local date / time.
	 *
	 *	@return	the initialized object.
	 */
	static DateTime CreateLocalDateTime();

	/**
	 *	@brief	Convert julian to gregorian date / time. 
	 *
	 *	@param	julianDateTime the julian date / time.
	 *	@param	year the year.
	 *	@param	month the month.
	 *	@param	day the day.
	 */
	static void ConvertJulianToGregorianDateTime(const int64_t julianDateTime, int32_t &year, int32_t &month, int32_t &day, int32_t &hour, int32_t &minute, int32_t &second);

	/**
	 *	@brief	Convert gregorian to julian date / time.
	 *
	 *	@param	year the year.
	 *	@param	month the month.
	 *	@param	day the day.
	 *
	 *	@return	the julian date / time.
	 */
	static int64_t ConvertGregorianToJulianDateTime(const int32_t year, const int32_t month, const int32_t day, const uint32_t hour, const uint32_t minute, const uint32_t second);

	/**
	 *	@brief	Get the julian date. 
	 *
	 *	@param	julianDateTime the julian date / time.
	 *
	 *	@return	the julian date.
	 */
	static int32_t GetJulianDate(const int64_t julianDateTime);

	/**
	 *	@brief	Get the julian time. 
	 *
	 *	@param	julianDateTime the julian date / time.
	 *
	 *	@return	the julian time.
	 */
	static int32_t GetJulianTime(const int64_t julianDateTime);

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
	static StatusCode ValidateSerializedLengthAndBody(MessageValidater &messageValidater, size_t &serializedBodyOffset, size_t &serializedBodyLength);

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

} // namespace Activ

#endif // !defined (ACTIV_DATE_TIME_H)

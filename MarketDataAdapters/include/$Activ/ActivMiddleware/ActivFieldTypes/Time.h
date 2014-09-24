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

namespace Activ
{

class MessageBuilder;
class MessageValidater;

/**
 *	@brief	Time class.
 */
class ACTIV_FIELD_TYPES_API Time : public IFieldType
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
	Time& operator+=(const Time &rhs);

	/**
	 *	@brief	Addition assignment operator.
	 *
	 *	@param	nSeconds the number of seconds to add to this object.
	 *
	 *	@return	a reference to this object.
	 *
	 *	@throw	ActivException
	 */
	Time& operator+=(const int32_t nSeconds);

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
	Time& operator-=(const Time &rhs);

	/**
	 *	@brief	Subtraction assignment operator.
	 *
	 *	@param	nSeconds the number of seconds to subtract from this object.
	 *
	 *	@return	a reference to this object.
	 *
	 *	@throw	ActivException
	 */
	Time& operator-=(const int32_t nSeconds);

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
	StatusCode Compare(const Time &rhs, int &result) const;

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
	 *	@brief	Create an object intitialised with the current UTC time.
	 *
	 *	@return	the initialized object.
	 */
	static Time CreateUtcTime();

	/**
	 *	@brief	Create an object intitialised with the current local time.
	 *
	 *	@return	the initialized object.
	 */
	static Time CreateLocalTime();

	/**
	 *	@brief	Convert julian to gregorian time.
	 *
	 *	@param	julianTime the julian time.
	 *	@param	hour the hour.
	 *	@param	minute the minute.
	 *	@param	second the second.
	 */
	static void ConvertJulianToGregorianTime(const int32_t julianTime, int32_t &hour, int32_t &minute, int32_t &second);

	/**
	 *	@brief	Convert gregorian to julian time.
	 *
	 *	@param	hour the hour.
	 *	@param	minute the minute.
	 *	@param	second the second.
	 *
	 *	@return	the julian time.
	 */
	static int32_t ConvertGregorianToJulianTime(const int32_t hour, const int32_t minute, const int32_t second);

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
	uint8_t		m_hour;		///< The hour.
	uint8_t		m_minute;	///< The minute.
	uint8_t		m_second;	///< The second.

	friend class DateTime;
};

} // namespace Activ

#endif // !defined (ACTIV_TIME_H)

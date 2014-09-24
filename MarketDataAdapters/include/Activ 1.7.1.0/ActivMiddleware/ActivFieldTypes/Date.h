/**
 *	@file	Date.h
 *
 *	@brief	Header file for the date class.
 *
 *	$Log: $
 */

#if !defined (ACTIV_DATE_H)
#define ACTIV_DATE_H

#include "ActivMiddleware/ActivFieldTypes/External.h"
#include "ActivMiddleware/ActivFieldTypes/IFieldType.h"

#include "ActivMiddleware/ActivBase/StatusCodeException.h"

#include <ctime>

namespace Activ
{

/**
*	@brief	The day list.
*/
#define ACTIV_DAY_LIST(d)			\
	d(SUNDAY,		"Sunday")		\
	d(MONDAY,		"Monday")		\
	d(TUESDAY,		"Tuesday")		\
	d(WEDNESDAY,	"Wednesday")	\
	d(THURSDAY,		"Thursday")		\
	d(FRIDAY,		"Friday")		\
	d(SATURDAY,		"Saturday")

/**
*	@brief	The month list.
*/
#define ACTIV_MONTH_LIST(d)			\
	d(UNDEFINED,	"Undefined")	\
	d(JANUARY,		"January")		\
	d(FEBRUARY,		"February")		\
	d(MARCH,		"March")		\
	d(APRIL,		"April")		\
	d(MAY,			"May")			\
	d(JUNE,			"June")			\
	d(JULY,			"July")			\
	d(AUGUST,		"August")		\
	d(SEPTEMBER,	"September")	\
	d(OCTOBER,		"October")		\
	d(NOVEMBER,		"November")		\
	d(DECEMBER,		"December")

class MessageBuilder;
class MessageValidator;

/**
 *	@brief	Date class.
 */
class Date : public IFieldType
{
public:
	static const FieldType FIELD_TYPE = FIELD_TYPE_DATE; ///< The field type.

	static const uint32_t MONTHS_PER_YEAR	= 12;	///< The number of months in an year.
	static const uint32_t DAYS_PER_WEEK		= 7;	///< The number of days in a week.
	static const uint32_t DAYS_PER_MONTH	= 31;	///< The number of days in a month.

	using IFieldType::FromString;
	using IFieldType::Serialize;
	using IFieldType::SerializeBody;
	using IFieldType::SerializeLengthAndBody;
	using IFieldType::Deserialize;
	using IFieldType::DeserializeBody;
	using IFieldType::DeserializeLengthAndBody;

	/**
	*	@brief	Day enumeration.
	*/
	enum
	{
		ACTIV_DAY_LIST(ACTIV_DECLARE_ELEMENT_1_WITH_COMMA)
		NUM_DAYS
	};

	/**
	*	@brief	Month enumeration.
	*/
	enum
	{
		ACTIV_MONTH_LIST(ACTIV_DECLARE_ELEMENT_1_WITH_COMMA)
		NUM_MONTHS
	};

	/**
	 *	@brief	Default constructor.
	 */
	Date();

	/**
	 *	@brief	Constructor.
	 *
	 *	@param	year the year.
	 *	@param	month the month.
	 *	@param	day the day.
	 *
	 *	@throw	StatusCodeException
	 */
	Date(const uint32_t year, const uint32_t month, const uint32_t day);

	/**
	 *	@brief	Constructor.
	 *
	 *	@param	julianDate the julian date.
	 *
	 *	@throw	StatusCodeException
	 */
	Date(const int32_t julianDate);

	/**
	 *	@brief	Constructor.
	 *
	 *	Uses the tm_year, tm_mon and tm_mday fields of the provided tm structure.
	 *
	 *	@param	tm reference to a tm structure specifying the date.
	 *
	 *	@throw	StatusCodeException
	 */
	Date(const std::tm &tm);

	/**
	 *	@brief	Destructor.
	 */
	virtual ~Date();

	/**
	 *	@brief	Equality operator.
	 *
	 *	@param	rhs the object to compare with.
	 *
	 *	@return	the result of the test.
	 *
	 *	@throw	StatusCodeException
	 */
	bool operator==(const Date &rhs) const;

	/**
	 *	@brief	Inequality operator.
	 *
	 *	@param	rhs the object to compare with.
	 *
	 *	@return	the result of the test.
	 *
	 *	@throw	StatusCodeException
	 */
	bool operator!=(const Date &rhs) const;

	/**
	 *	@brief	Less than operator.
	 *
	 *	@param	rhs the object to compare with.
	 *
	 *	@return	the result of the test.
	 *
	 *	@throw	StatusCodeException
	 */
	bool operator<(const Date &rhs) const;

	/**
	 *	@brief	Less than or equal to operator.
	 *
	 *	@param	rhs the object to compare with.
	 *
	 *	@return	the result of the test.
	 *
	 *	@throw	StatusCodeException
	 */
	bool operator<=(const Date &rhs) const;

	/**
	 *	@brief	Greater than operator.
	 *
	 *	@param	rhs the object to compare with.
	 *
	 *	@return	the result of the test.
	 *
	 *	@throw	StatusCodeException
	 */
	bool operator>(const Date &rhs) const;

	/**
	 *	@brief	Greater than or equal to operator.
	 *
	 *	@param	rhs the object to compare with.
	 *
	 *	@return	the result of the test.
	 *
	 *	@throw	StatusCodeException
	 */
	bool operator>=(const Date &rhs) const;

	/**
	 *	@brief	Prefix increment operator.
	 *
	 *	@return	Reference to the updated object.
	 *
	 *	@throw	StatusCodeException
	 */
	Date &operator++();

	/**
	 *	@brief	Postfix increment operator.
	 *
	 *	@return	Copy of the original object.
	 *
	 *	@throw	StatusCodeException
	 */
	const Date operator++(int);

	/**
	 *	@brief	Addition operator.
	 *
	 *	@param	nDays the number of days to add to this object.
	 *
	 *	@return	an object containing the result of the operation.
	 *
	 *	@throw	StatusCodeException
	 */
	const Date operator+(const int32_t nDays) const;

	/**
	 *	@brief	Addition assignment operator.
	 *
	 *	@param	nDays the number of days to add to this object.
	 *
	 *	@return	a reference to this object.
	 *
	 *	@throw	StatusCodeException
	 */
	Date& operator+=(const int32_t nDays);

	/**
	 *	@brief	Prefix decrement operator.
	 *
	 *	@return	Reference to the updated object.
	 *
	 *	@throw	StatusCodeException
	 */
	Date &operator--();

	/**
	 *	@brief	Postfix decrement operator.
	 *
	 *	@return	Copy of the original object.
	 *
	 *	@throw	StatusCodeException
	 */
	const Date operator--(int);

	/**
	 *	@brief	Subtraction operator.
	 *
	 *	@param	nDays the number of days to subtract from this object.
	 *
	 *	@return	an object containing the result of the operation.
	 *
	 *	@throw	StatusCodeException
	 */
	const Date operator-(const int32_t nDays) const;

	/**
	 *	@brief	Subtraction assignment operator.
	 *
	 *	@param	nDays the number of days to subtract from this object.
	 *
	 *	@return	a reference to this object.
	 *
	 *	@throw	StatusCodeException
	 */
	Date& operator-=(const int32_t nDays);

	/**
	 *	@brief	Store a new date.
	 *
	 *	@param	year the year.
	 *	@param	month the month.
	 *	@param	day the day.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 */
	StatusCode Set(const uint32_t year, const uint32_t month, const uint32_t day);

	/**
	 *	@brief	Store a new date.
	 *
	 *	@param	julianDate the julian date.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 */
	StatusCode Set(const int32_t julianDate);

	/**
	 *	@brief	Store a new date.
	 *
	 *	Uses the tm_year, tm_mon and tm_mday fields of the provided tm structure.
	 *
	 *	@param	tm reference to a tm structure specifying the date.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 */
	StatusCode Set(const std::tm &tm);

	/**
	 *	@brief	Get the date.
	 *
	 *	@param	year the year.
	 *	@param	month the month.
	 *	@param	day the day.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_NOT_INITIALIZED
	 */
	StatusCode Get(uint32_t &year, uint32_t &month, uint32_t &day) const;

	/**
	 *	@brief	Get the julian date.
	 *
	 *	@return	the julian date.
	 *
	 *	@throw	StatusCodeException
	 */
	int32_t Get() const;

	/**
	 *	@brief	Get the date.
	 *
	 *	Only sets the tm_year, tm_mon and tm_mday fields of the provided tm structure.
	 *
	 *	@param	tm reference to a tm structure to receive the date.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_NOT_INITIALIZED
	 */
	StatusCode Get(std::tm &tm) const;

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
	ACTIV_FIELD_TYPES_API StatusCode Compare(const Date &rhs, int &result) const;

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
	 *	@brief	Get the number of days in a month.
	 *
	 *	@param	year the year.
	 *	@param	month the month.
	 *
	 *	@return	the number of days in the month.
	 */
	ACTIV_FIELD_TYPES_API static uint32_t GetDaysInMonth(const uint32_t year, const uint32_t month);

	/**
	 *	@brief	Get the day of the week.
	 *
	 *	@param	year the year.
	 *	@param	month the month.
	 *	@param	day the day.
	 *
	 *	@return	the day of the week (Sunday = 0, Monday = 1, ..., Saturday = 6).
	 */
	ACTIV_FIELD_TYPES_API static uint32_t GetDayOfWeek(const uint32_t year, const uint32_t month, const uint32_t day);

	/**
	 *	@brief	Get the day of the year.
	 *
	 *	@param	year the year.
	 *	@param	month the month.
	 *	@param	day the day.
	 *
	 *	@return	the day of the year (Jan 1st = 1, Jan 2nd = 2, ..., Dec 31st = 365).
	 */
	ACTIV_FIELD_TYPES_API static uint32_t GetDayOfYear(const uint32_t year, const uint32_t month, const uint32_t day);
	
	/**
	 *	@brief	Get the week of the year.
	 *
	 *	@param	year the year.
	 *	@param	month the month.
	 *	@param	day the day.
	 *
	 *	@return	the week of the year (the week containing January 4th = 1)
	 */
	ACTIV_FIELD_TYPES_API static uint32_t GetWeekOfYear(const uint32_t year, const uint32_t month, const uint32_t day);

	/**
	 *	@brief	Is the year a leap year.
	 *
	 *	@param	year the year.
	 *
	 *	@return	whether the year is a leap year.
	 */
	ACTIV_FIELD_TYPES_API static bool IsLeapYear(const uint32_t year);

	/**
	 *	@brief	Create an object intitialized with the current UTC date.
	 *
	 *	@return	the initialized object.
	 */
	ACTIV_FIELD_TYPES_API static Date CreateUtcDate();

	/**
	 *	@brief	Create an object intitialized with the current local date.
	 *
	 *	@return	the initialized object.
	 */
	ACTIV_FIELD_TYPES_API static Date CreateLocalDate();

	/**
	 *	@brief	Convert julian to gregorian date. 
	 *
	 *	Algorithm only valid for non-negative julian date number (gregorian date on or after -4713/11/24).
	 *
	 *	@param	julianDate the julian date.
	 *	@param	year the year.
	 *	@param	month the month.
	 *	@param	day the day.
	 */
	ACTIV_FIELD_TYPES_API static void ConvertJulianToGregorianDate(const int32_t julianDate, int32_t &year, int32_t &month, int32_t &day);

	/**
	 *	@brief	Convert gregorian to julian date.
	 *
	 *	Algorithm only valid for non-negative julian date number (gregorian date on or after -4713/11/24).
	 *
	 *	@param	year the year.
	 *	@param	month the month.
	 *	@param	day the day.
	 *
	 *	@return	the julian date.
	 */
	ACTIV_FIELD_TYPES_API static int32_t ConvertGregorianToJulianDate(const int32_t year, const int32_t month, const int32_t day);

	/**
	 *	@brief	Convert the day of the week to a string.
	 *
	 *	@param	dayOfWeek the day of the week to convert.
	 *
	 *	@return	the day of the week as a string.
	 */
	ACTIV_FIELD_TYPES_API static std::string DayOfWeekToString(const uint32_t dayOfWeek);

	/**
	 *	@brief	Convert the month to a string.
	 *
	 *	@param	month the month to convert.
	 *
	 *	@return	the month as a string.
	 */
	ACTIV_FIELD_TYPES_API static std::string MonthToString(const uint32_t month);

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
	static const size_t DAY_OFFSET		= 0;								///< The day offset.
	static const size_t DAY_LENGTH		= 5;								///< The day length.
	static const size_t MONTH_OFFSET	= (DAY_OFFSET + DAY_LENGTH);		///< The month offset.
	static const size_t MONTH_LENGTH	= 4;								///< The month length.
	static const size_t YEAR_OFFSET		= (MONTH_OFFSET + MONTH_LENGTH);	///< The year offset.
	static const size_t YEAR_LENGTH		= 15;								///< The year length.
	static const size_t MAX_OFFSET		= (YEAR_OFFSET + YEAR_LENGTH);		///< The maximum offset.

	static const size_t MAX_DAY			= ((1 << DAY_LENGTH) - 1);			///< The maximum day.
	static const size_t MAX_MONTH		= ((1 << MONTH_LENGTH) - 1);		///< The maximum month.
	static const size_t MAX_YEAR		= ((1 << YEAR_LENGTH) - 1);			///< The maximum year.

	/**
	 *	@brief	Get an unsigned 32 bit value.
	 *
	 *	@param	pData pointer to the data.
	 *	@param	size the size of the data.
	 *	@param	value the value to be copied to.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 */
	static StatusCode GetUnsignedInt32(const void * const pData, const size_t size, uint32_t &value);

	uint32_t	m_year;		///< The year.
	uint8_t		m_month;	///< The month.
	uint8_t		m_day;		///< The day.

	friend class DateTime;
};

// ---------------------------------------------------------------------------------------------------------------------------------

inline Date::Date() :
	IFieldType(FIELD_TYPE)
{
	m_isInitialized = false;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline Date::Date(const uint32_t year, const uint32_t month, const uint32_t day) :
	IFieldType(FIELD_TYPE)
{
	ACTIV_THROW_RESULT_IF(STATUS_CODE_SUCCESS, !=, Set(year, month, day));
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline Date::Date(const int32_t julianDate) :
	IFieldType(FIELD_TYPE)
{
	ACTIV_THROW_RESULT_IF(STATUS_CODE_SUCCESS, !=, Set(julianDate));
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline Date::Date(const std::tm &tm) :
	IFieldType(FIELD_TYPE)
{
	ACTIV_THROW_RESULT_IF(STATUS_CODE_SUCCESS, !=, Set(tm));
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline Date::~Date()
{
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline bool Date::operator==(const Date &rhs) const
{
	int result;
	ACTIV_THROW_RESULT_IF(STATUS_CODE_SUCCESS, !=, Compare(rhs, result));

	return (0 == result);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline bool Date::operator!=(const Date &rhs) const
{
	return !((*this) == rhs);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline bool Date::operator<(const Date &rhs) const
{
	int result;
	ACTIV_THROW_RESULT_IF(STATUS_CODE_SUCCESS, !=, Compare(rhs, result));

	return (result < 0);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline bool Date::operator<=(const Date &rhs) const
{
	return !((*this) > rhs);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline bool Date::operator>(const Date &rhs) const
{
	int result;
	ACTIV_THROW_RESULT_IF(STATUS_CODE_SUCCESS, !=, Compare(rhs, result));

	return (result > 0);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline bool Date::operator>=(const Date &rhs) const
{
	return !((*this) < rhs);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline Date &Date::operator++()
{
	return (*this) += 1;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline const Date Date::operator++(int)
{
	const Date oldValue = *this;

	++(*this);

	return oldValue;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline const Date Date::operator+(const int32_t nDays) const
{
	return Date(*this) += nDays;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline Date& Date::operator+=(const int32_t nDays)
{
	ACTIV_THROW_RESULT_IF(STATUS_CODE_SUCCESS, !=, AddDays(nDays));

	return (*this);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline Date &Date::operator--()
{
	return (*this) -= 1;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline const Date Date::operator--(int)
{
	const Date oldValue = *this;

	--(*this);

	return oldValue;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline const Date Date::operator-(const int32_t nDays) const
{
	return Date(*this) -= nDays;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline Date& Date::operator-=(const int32_t nDays)
{
	ACTIV_THROW_RESULT_IF(STATUS_CODE_SUCCESS, !=, SubtractDays(nDays));

	return (*this);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode Date::Set(const uint32_t year, const uint32_t month, const uint32_t day)
{
	if ((year > MAX_YEAR) || (month < 1) || (month > MONTHS_PER_YEAR) || (day > GetDaysInMonth(year, month)))
		return STATUS_CODE_INVALID_PARAMETER;

	m_year = static_cast<uint32_t>(year);
	m_month = static_cast<uint8_t>(month);
	m_day = static_cast<uint8_t>(day);
	m_isInitialized = true;

	return STATUS_CODE_SUCCESS;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode Date::Set(const int32_t julianDate)
{
	int32_t year, month, day;

	ConvertJulianToGregorianDate(julianDate, year, month, day);

	if ((year < 0) || (year > static_cast<int32_t>(MAX_YEAR)) || (month < 1) || (month > static_cast<int32_t>(MONTHS_PER_YEAR)) || (day < 1) || (static_cast<uint32_t>(day) > GetDaysInMonth(static_cast<uint32_t>(year), static_cast<uint32_t>(month))))
		return STATUS_CODE_INVALID_PARAMETER;

	m_year = static_cast<uint32_t>(year);
	m_month = static_cast<uint8_t>(month);
	m_day = static_cast<uint8_t>(day);
	m_isInitialized = true;

	return STATUS_CODE_SUCCESS;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode Date::Set(const std::tm &tm)
{
	return Set(static_cast<uint32_t>(tm.tm_year + 1900), static_cast<uint32_t>(tm.tm_mon + 1), static_cast<uint32_t>(tm.tm_mday));
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode Date::Get(uint32_t &year, uint32_t &month, uint32_t &day) const
{
	if (!IsInitialized())
		return STATUS_CODE_NOT_INITIALIZED;

	year = m_year;
	month = m_month;
	day = m_day;

	return STATUS_CODE_SUCCESS;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline int32_t Date::Get() const
{
	if (!IsInitialized())
		ACTIV_THROW(StatusCodeException, STATUS_CODE_NOT_INITIALIZED);

	return ConvertGregorianToJulianDate(m_year, m_month, m_day);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode Date::Get(std::tm &tm) const
{
	if (!IsInitialized())
		return STATUS_CODE_NOT_INITIALIZED;

	tm.tm_year = static_cast<int>(m_year - 1900);
	tm.tm_mon = static_cast<int>(m_month - 1);
	tm.tm_mday = static_cast<int>(m_day);

	return STATUS_CODE_SUCCESS;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline uint32_t Date::GetYear() const
{
	if (!IsInitialized())
		ACTIV_THROW(StatusCodeException, STATUS_CODE_NOT_INITIALIZED);

	return static_cast<uint32_t>(m_year);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline uint32_t Date::GetMonth() const
{
	if (!IsInitialized())
		ACTIV_THROW(StatusCodeException, STATUS_CODE_NOT_INITIALIZED);

	return static_cast<uint32_t>(m_month);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline uint32_t Date::GetDay() const
{
	if (!IsInitialized())
		ACTIV_THROW(StatusCodeException, STATUS_CODE_NOT_INITIALIZED);

	return static_cast<uint32_t>(m_day);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline uint32_t Date::GetDayOfWeek() const
{
	if (!IsInitialized())
		ACTIV_THROW(StatusCodeException, STATUS_CODE_NOT_INITIALIZED);

	return GetDayOfWeek(m_year, m_month, m_day);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline uint32_t Date::GetDayOfYear() const
{
	if (!IsInitialized())
		ACTIV_THROW(StatusCodeException, STATUS_CODE_NOT_INITIALIZED);

	return GetDayOfYear(m_year, m_month, m_day);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline uint32_t Date::GetWeekOfYear() const
{
	if (!IsInitialized())
		ACTIV_THROW(StatusCodeException, STATUS_CODE_NOT_INITIALIZED);

	return GetWeekOfYear(m_year, m_month, m_day);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline void Date::Reset()
{
	m_isInitialized = false;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline void Date::Clear()
{
	m_isInitialized = false;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline size_t Date::GetSerializedLength() const
{
	return (IsInitialized() ? GetMaxSerializedLength() : 0);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline size_t Date::GetSerializedBodyLength() const
{
	return (IsInitialized() ? GetMaxSerializedBodyLength() : 0);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline size_t Date::GetMaxSerializedLength()
{
	return GetMaxSerializedBodyLength();
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline size_t Date::GetMaxSerializedBodyLength()
{
	return (((MAX_OFFSET - 1) / CHAR_BIT) + 1);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline bool Date::IsValidSerializedBodyLength(const size_t serializedBodyLength)
{
	return (GetMaxSerializedBodyLength() == serializedBodyLength);
}

} // namespace Activ

#endif // !defined (ACTIV_DATE_H)

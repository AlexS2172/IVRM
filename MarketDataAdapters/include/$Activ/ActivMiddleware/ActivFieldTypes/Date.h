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
class MessageValidater;

/**
 *	@brief	Date class.
 */
class ACTIV_FIELD_TYPES_API Date : public IFieldType
{
public:
	static const FieldType FIELD_TYPE = FIELD_TYPE_DATE; ///< The field type.

	static const uint32_t MONTHS_PER_YEAR	= 12;	///< The number of months in an year.
	static const uint32_t DAYS_PER_WEEK		= 7;	///< The number of days in a week.
	static const uint32_t DAYS_PER_MONTH	= 31;	///< The number of days in a month.

	using IFieldType::FromString;

	/**
	*	@brief	Day enumeration.
	*/
	enum
	{
		ACTIV_DAY_LIST(ACTIV_DECLARE_ELEMENT_1_WITH_COMMA)
	};

	/**
	*	@brief	Month enumeration.
	*/
	enum
	{
		ACTIV_MONTH_LIST(ACTIV_DECLARE_ELEMENT_1_WITH_COMMA)
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
	 *	@throw	ActivException
	 */
	Date(const uint32_t year, const uint32_t month, const uint32_t day);

	/**
	 *	@brief	Constructor.
	 *
	 *	@param	julianDate the julian date.
	 *
	 *	@throw	ActivException
	 */
	Date(const int32_t julianDate);

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
	 *	@throw	ActivException
	 */
	bool operator==(const Date &rhs) const;

	/**
	 *	@brief	Inequality operator.
	 *
	 *	@param	rhs the object to compare with.
	 *
	 *	@return	the result of the test.
	 *
	 *	@throw	ActivException
	 */
	bool operator!=(const Date &rhs) const;

	/**
	 *	@brief	Less than operator.
	 *
	 *	@param	rhs the object to compare with.
	 *
	 *	@return	the result of the test.
	 *
	 *	@throw	ActivException
	 */
	bool operator<(const Date &rhs) const;

	/**
	 *	@brief	Less than or equal to operator.
	 *
	 *	@param	rhs the object to compare with.
	 *
	 *	@return	the result of the test.
	 *
	 *	@throw	ActivException
	 */
	bool operator<=(const Date &rhs) const;

	/**
	 *	@brief	Greater than operator.
	 *
	 *	@param	rhs the object to compare with.
	 *
	 *	@return	the result of the test.
	 *
	 *	@throw	ActivException
	 */
	bool operator>(const Date &rhs) const;

	/**
	 *	@brief	Greater than or equal to operator.
	 *
	 *	@param	rhs the object to compare with.
	 *
	 *	@return	the result of the test.
	 *
	 *	@throw	ActivException
	 */
	bool operator>=(const Date &rhs) const;

	/**
	 *	@brief	Prefix increment operator.
	 *
	 *	@return	Reference to the updated object.
	 *
	 *	@throw	ActivException
	 */
	Date &operator++();

	/**
	 *	@brief	Postfix increment operator.
	 *
	 *	@return	Copy of the original object.
	 *
	 *	@throw	ActivException
	 */
	const Date operator++(int);

	/**
	 *	@brief	Addition operator.
	 *
	 *	@param	nDays the number of days to add to this object.
	 *
	 *	@return	an object containing the result of the operation.
	 *
	 *	@throw	ActivException
	 */
	const Date operator+(const int32_t nDays) const;

	/**
	 *	@brief	Addition assignment operator.
	 *
	 *	@param	nDays the number of days to add to this object.
	 *
	 *	@return	a reference to this object.
	 *
	 *	@throw	ActivException
	 */
	Date& operator+=(const int32_t nDays);

	/**
	 *	@brief	Prefix decrement operator.
	 *
	 *	@return	Reference to the updated object.
	 *
	 *	@throw	ActivException
	 */
	Date &operator--();

	/**
	 *	@brief	Postfix decrement operator.
	 *
	 *	@return	Copy of the original object.
	 *
	 *	@throw	ActivException
	 */
	const Date operator--(int);

	/**
	 *	@brief	Subtraction operator.
	 *
	 *	@param	nDays the number of days to subtract from this object.
	 *
	 *	@return	an object containing the result of the operation.
	 *
	 *	@throw	ActivException
	 */
	const Date operator-(const int32_t nDays) const;

	/**
	 *	@brief	Subtraction assignment operator.
	 *
	 *	@param	nDays the number of days to subtract from this object.
	 *
	 *	@return	a reference to this object.
	 *
	 *	@throw	ActivException
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
	 *	@throw	ActivException
	 */
	int32_t Get() const;

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
	StatusCode Compare(const Date &rhs, int &result) const;

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
	 *	@brief	Get the number of days in a month.
	 *
	 *	@param	year the year.
	 *	@param	month the month.
	 *
	 *	@return	the number of days in the month.
	 */
	static uint32_t GetDaysInMonth(const uint32_t year, const uint32_t month);

	/**
	 *	@brief	Get the day of the week.
	 *
	 *	@param	year the year.
	 *	@param	month the month.
	 *	@param	day the day.
	 *
	 *	@return	the day of the week (Sunday = 0, Monday = 1, ..., Saturday = 6).
	 */
	static uint32_t GetDayOfWeek(const uint32_t year, const uint32_t month, const uint32_t day);

	/**
	 *	@brief	Get the day of the year.
	 *
	 *	@param	year the year.
	 *	@param	month the month.
	 *	@param	day the day.
	 *
	 *	@return	the day of the year (Jan 1st = 1, Jan 2nd = 2, ..., Dec 31st = 365).
	 */
	static uint32_t GetDayOfYear(const uint32_t year, const uint32_t month, const uint32_t day);
	
	/**
	 *	@brief	Get the week of the year.
	 *
	 *	@param	year the year.
	 *	@param	month the month.
	 *	@param	day the day.
	 *
	 *	@return	the week of the year (the week containing January 4th = 1)
	 */
	static uint32_t GetWeekOfYear(const uint32_t year, const uint32_t month, const uint32_t day);

	/**
	 *	@brief	Is the year a leap year.
	 *
	 *	@param	year the year.
	 *
	 *	@return	whether the year is a leap year.
	 */
	static bool IsLeapYear(const uint32_t year);

	/**
	 *	@brief	Create an object intitialized with the current UTC date.
	 *
	 *	@return	the initialized object.
	 */
	static Date CreateUtcDate();

	/**
	 *	@brief	Create an object intitialized with the current local date.
	 *
	 *	@return	the initialized object.
	 */
	static Date CreateLocalDate();

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
	static void ConvertJulianToGregorianDate(const int32_t julianDate, int32_t &year, int32_t &month, int32_t &day);

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
	static int32_t ConvertGregorianToJulianDate(const int32_t year, const int32_t month, const int32_t day);

	/**
	 *	@brief	Convert the day of the week to a string.
	 *
	 *	@param	dayOfWeek the day of the week to convert.
	 *
	 *	@return	the day of the week as a string.
	 */
	static std::string DayOfWeekToString(const uint32_t dayOfWeek);

	/**
	 *	@brief	Convert the month to a string.
	 *
	 *	@param	month the month to convert.
	 *
	 *	@return	the month as a string.
	 */
	static std::string MonthToString(const uint32_t month);

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

} // namespace Activ

#endif // !defined (ACTIV_DATE_H)

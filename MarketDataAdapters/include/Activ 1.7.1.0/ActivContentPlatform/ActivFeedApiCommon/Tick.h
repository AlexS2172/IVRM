/**
 *	@file	Tick.h
 *
 *	@brief	Tick Field Set.
 *
 *	$Log: $
 */

#if (!defined ACTIV_FEED_API_COMMON_TICK_H)
#define ACTIV_FEED_API_COMMON_TICK_H

#include "ActivContentPlatform/ActivFeedApiCommon/External.h"
#include "ActivContentPlatform/ActivFeedApiCommon/FieldSet.h"

#include "ActivMiddleware/ActivFieldTypes/Date.h"
#include "ActivMiddleware/ActivFieldTypes/Time.h"
#include "ActivMiddleware/ActivFieldTypes/Rational.h"
#include "ActivContentPlatform/ActivFeedCommon/UpdateIds.h"

namespace Activ
{

namespace ContentPlatform
{

namespace FeedApi
{

#define ACTIV_DECLARE_BYTES(value, id)	static const byte_t id = value;

/**
 *	@brief	Tick types.
 */
#define TICK_TYPE_LIST(d)										\
	d(	TICK_TYPE_NONE,				"None"					)	\
	d(	TICK_TYPE_TRADE,			"Trade"					)	\
	d(	TICK_TYPE_CORRECTED_TRADE,	"Corrected Trade"		)	\
	d(	TICK_TYPE_DELETED_TRADE,	"Deleted Trade"			)	\
	d(	TICK_TYPE_FORM_T_TRADE,		"Form T Trade"			)	\
	d(	TICK_TYPE_FILTERED_TRADE,	"Filtered Trade"		)	\
	d(	TICK_TYPE_BID,				"Bid"					)	\
	d(	TICK_TYPE_ASK,				"Ask"					)	\
	d(	TICK_TYPE_RESERVED1,		"not used"				)	\
	d(	TICK_TYPE_RESERVED2,		"not used"				)	\
	d(	TICK_TYPE_TRADE_INFO,		"Detailed trade info"	)	

enum TickType
{
	TICK_TYPE_LIST(ACTIV_DECLARE_ELEMENT_1_WITH_COMMA)
	NUM_TICK_TYPES,
};

// ---------------------------------------------------------------------------------------------------------------------------------

/**
 *	@brief	Tick Field Set class.
 */
class Tick : public FieldSet
{
public:
	/**
	 *	@brief	Constructor.
	 */
	ACTIV_FEED_API_COMMON_API Tick();

	/**
	 *	@brief	Destructor.
	 */
	ACTIV_FEED_API_COMMON_API virtual ~Tick();

	/**
	 *	@brief	Reset all values in the field set.
	 */
	ACTIV_FEED_API_COMMON_API virtual void Reset();

	/**
	 *	@brief	Get a field.
	 *
	 *	@param	fieldId field's id.
	 *	@param	field actual field.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_FIELD
	 *	@retval	STATUS_CODE_UNDEFINED_FIELD
	 */
	ACTIV_FEED_API_COMMON_API virtual StatusCode GetField(const Feed::FieldId fieldId, IFieldType &field) const;

	/**
	 *	@brief	Get a field.
	 *
	 *	@param	fieldId field's id.
	 *	@param	field actual field.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_FIELD
	 *	@retval	STATUS_CODE_UNDEFINED_FIELD
	 */
	ACTIV_FEED_API_COMMON_API virtual StatusCode SetField(const Feed::FieldId fieldId, const IFieldType &field);

	/**
	 *	@brief	Get the field set type.
	 *
	 *	@return	the field set type.
	 */
	ACTIV_FEED_API_COMMON_API virtual FieldSetType GetType() const;

	/**
	 *	@brief	Convert the object to a string.
	 *
	 *	@return	the object as a string.
	 */
	ACTIV_FEED_API_COMMON_API virtual std::string ToString(const std::string &deliminator = DEFAULT_STRING_FIELD_SEPARATER) const;

	/**
	 *	@brief	Convert the object to a string.  This one w/o date and updateId.
	 *
	 *	@return	the object as a string.
	 */
	ACTIV_FEED_API_COMMON_API virtual std::string ToShortString(const std::string &deliminator = DEFAULT_STRING_FIELD_SEPARATER) const;

	/**
	 *	@brief	Set the object value from a string.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *  @retval ......
	 */
	ACTIV_FEED_API_COMMON_API virtual StatusCode FromString(const std::string &inputStr, const std::string &deliminator = DEFAULT_STRING_FIELD_SEPARATER);

	/**
	 *	@brief	Serialize the object into the supplied message builder.
	 *
	 *	@param	messageBuilder the message builder to store a serialized version of the object in.
	 *	@param	maxSerializedBodyLength the maximum serialized body length.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
	ACTIV_FEED_API_COMMON_API virtual StatusCode Serialize(MessageBuilder &messageBuilder) const;

	/**
	 *	@brief	Deserialize the object from the supplied message validator.
	 *
	 *	@param	messageValidator the message validator from which the object will be extracted.
	 *	@param	maxSerializedBodyLength the maximum serialized body length.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
	ACTIV_FEED_API_COMMON_API virtual StatusCode Deserialize(MessageValidator &messageValidator);

	/**
	 *	@brief Get the serialized length of the object.
	 *
	 *	@return	the serialized length of the object.
	 */
	ACTIV_FEED_API_COMMON_API virtual size_t GetSerializedLength() const;

	/**
	 *	@brief	Get the maximum serialized length.
	 *
	 *	@return	the maximum serialized length.
	 */
	ACTIV_FEED_API_COMMON_API static size_t GetMaxSerializedLength();

	/**
	 *	@brief	compares all fields except tick type.
	 *
	 *	@param	rhs the object to compare with.
	 *
	 *	@return	true if the same, otherwise false
	 */
	virtual bool Compare(const Tick &rhs) const;

	/**
	 *	@brief	compares all fields except tick type and datetime.
	 *
	 *	@param	rhs the object to compare with.
	 *
	 *	@return	true if the same, otherwise false
	 */
	virtual bool CompareContentOnly(const Tick &rhs) const;

	/**
	 *	@brief	compares dateTime
	 *
	 *	@param	rhs the object to compare with.
	 *
	 *	@return	true if the same, otherwise false
	 */
	virtual StatusCode Compare(const DateTime &dateTime, int &result) const;

	/**
	 *	@brief	Compare dateTime only.
	 *
	 *	@param	rhs the object to compare with.
	 *	@param	result the result of the comparison. 0 date time is the same. 1 date time is later than rhs's.  -1 date time is earlier than rhs's
	 *
	 *	@retval STATUS_CODE_SUCCESS
	 *	@retval ...
	 */
	virtual StatusCode Compare(const Tick &rhs, int &result) const;

	/**
	 *	@brief	Equality operator.  Comparing date time only.  Use Compare and CompareContent if content comapre is needed
	 *
	 *	@param	rhs the object to compare with.
	 *
	 *	@return	the result of the test.
	 */
	bool operator==(const Tick &rhs) const;

	/**
	 *	@brief	Inequality operator.  Comparing date time only. 
	 *
	 *	@param	rhs the object to compare with.
	 *
	 *	@return	the result of the test.
	 */
	bool operator!=(const Tick &rhs) const;

	/**
	 *	@brief	Less than operator (date and time only).
	 *
	 *	@param	rhs the object to compare with.
	 *
	 *	@return	the result of the test.
	 */
	bool operator<(const Tick &rhs) const;

	/**
	 *	@brief	Less than or equal operator (date and time only).
	 *
	 *	@param	rhs the object to compare with.
	 *
	 *	@return	the result of the test.
	 */
	virtual bool operator<=(const Tick &rhs) const;

	/**
	 *	@brief	Greater than operator (date and time only).
	 *
	 *	@param	rhs the object to compare with.
	 *
	 *	@return	the result of the test.
	 */
	bool operator>(const Tick &rhs) const;

	/**
	 *	@brief	Greater than or equal operator (date and time only).
	 *
	 *	@param	rhs the object to compare with.
	 *
	 *	@return	the result of the test.
	 */
	bool operator>=(const Tick &rhs) const;
	
	/**
	 *	@brief	<= operator
	 *
	 *	@param	dateTime dateTime to compare
	 *
	 *	@retval true/false
	 */
	bool operator <(const DateTime &dateTime) const;
	bool operator <=(const DateTime &dateTime) const;

	/**
	 *	@brief	== operator
	 *
	 *	@param	dateTime dateTime to compare
	 *
	 *	@retval true/false
	 */
	bool operator ==(const DateTime &dateTime) const;

	/**
	 *	@brief	>= operator
	 *
	 *	@param	dateTime dateTime to compare
	 *
	 *	@retval true/false
	 */
	bool operator >=(const DateTime &dateTime) const;
	bool operator >(const DateTime &dateTime) const;

	/**
	 *	@brief	Get the conditions.
	 *
	 *	@param	pConditions conditions buffer.
	 *	@param	conditionsLength size of conditions buffer.
	 *	@param	conditionsSize length of conditions.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_LENGTH
	 */
	StatusCode GetConditions(byte_t *pConditions, const size_t conditionsLength, size_t &conditionsSize) const;

	/**
	 *	@brief	Set the conditions.
	 *
	 *	@param	pConditions conditions buffer.
	 *	@param	conditionsLength length of conditions.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_LENGTH
	 */
	StatusCode SetConditions(const byte_t *pConditions, const size_t conditionsLength);

	/**
	 *	@brief	Set the conditions.
	 *
	 *	@param	field field data
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_LENGTH
	 */
	StatusCode SetConditions(const IFieldType &field);

	/**
	 *	@brief	Get the price.
	 *
	 *	@param	price the price of the tick.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_LENGTH
	 */
	StatusCode GetPrice(double &price) const;

	/**
	 *	@brief	Get the price as a 64 bit signed integer.
	 *
	 *	@return	price.
	 */
	StatusCode GetPrice(Rational &price) const;

	/**
	 *	@brief	Set the price.
	 *
	 *	@param	price the price of the tick.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_LENGTH
	 */
	StatusCode SetPrice(const Rational &price);

	/**
	 *	@brief	Set price
	 *
	 *	@param	field field data
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_LENGTH
	 */
	StatusCode SetPrice(const IFieldType &field);

	/**
	 *	@brief	Get the exchange.
	 *
	 *	@param	exchange the exchange of the tick.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_LENGTH
	 */	
	StatusCode GetExchange(std::string &exchange) const;

	/**
	 *	@brief	Get the exchange.
	 *
	 *	@param	exchange the exchange of the tick.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_LENGTH
	 */	
	const std::string & GetExchange() const;

	/**
	 *	@brief	Set the exchange.
	 *
	 *	@param	exchange the exchange of the tick.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_LENGTH
	 */	
	StatusCode SetExchange(const std::string &exchange);

	/**
	 *	@brief	Get the tick's type.
	 *
	 *	@return	return the tick type.
	 */	
	TickType GetTickType() const;

	/**
	 *	@brief	Set the tick's type.
	 *
	 *	@param	type the tick's type.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_LENGTH
	 */	
	StatusCode SetTickType(const TickType type);

	/**
	 *	@brief	Get the time.
	 *
	 *	@param	time the time of the tick.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_LENGTH
	 */
	StatusCode GetDateTime(DateTime &dateTime) const;

	/**
	 *	@brief	Set the time.
	 *
	 *	@param	time the time of the tick.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_LENGTH
	 */
	StatusCode SetDateTime(const DateTime &dateTime);
	
	/**
	 *	@brief	Set the time.
	 *
	 *	@param	time the time of the tick.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_LENGTH
	 */
	StatusCode SetDateTime(const Date &date, const Time &time);

	/**
	 *	@brief	Set the time.
	 *
	 *	@param	time the time of the tick.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_LENGTH
	 */
	StatusCode SetDate(const Date &date);

	/**
	 *	@brief	Set the time.
	 *
	 *	@param	time the time of the tick.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_LENGTH
	 */
	StatusCode SetTime(const Time &time);
	
	/**
	 *	@brief	Get the tick's size.
	 *
	 *	@param	size the size of the tick.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_LENGTH
	 */
	StatusCode GetSize(Rational &size) const;

	/**
	 *	@brief	Set the tick's size.
	 *
	 *	@param	size the size of the tick.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_LENGTH
	 */
	StatusCode SetSize(const Rational &size);

	/**
	 *	@brief	Set size
	 *
	 *	@param	field field data
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_LENGTH
	 */
	StatusCode SetSize(const IFieldType &field);
	
	/**
	 *	@brief	Get the tick's size.
	 *
	 *	@param	size the size of the tick.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_LENGTH
	 */
	StatusCode GetSize(uint32_t &size) const;

	/**
	 *	@brief	Set the tick's size.
	 *
	 *	@param	size the size of the tick.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_LENGTH
	 */
	StatusCode SetSize(const uint32_t size);

	/**
	 *	@brief	Get the sequence number of the tick.
	 *
	 *	@param	sequence the sequence number of the tick.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_LENGTH
	 */
	StatusCode GetUpdateId(Feed::UpdateId &updateId) const;

	/**
	 *	@brief	Get the sequence number of the tick.
	 *
	 *	@param	sequence the sequence number of the tick.
	 *
	 */
	Feed::UpdateId GetUpdateId() const;

	/**
	 *	@brief	Set the sequence number of the tick.
	 *
	 *	@param	sequence the sequence number of the tick.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_LENGTH
	 */
	StatusCode SetUpdateId(const Feed::UpdateId updateId);

	/**
	 *	@brief	check if this tick is a regular hour trade
	 *
	 *	@retval	true/false
	 */
	bool IsRegularHourTrade() const;

	/**
	 *	@brief	check if this tick is a trade
	 *
	 *	@retval	true/false
	 */
	bool IsTrade() const;

	/**
	 *	@brief	check if this tick is out of sequence in terms of time
	 *
	 *	@retval	true/false
	 */
	bool IsOutOfSequence() const;

	/**
	 *	@brief	check if this tick is out of sequence in terms of time
	 *
	 *	@retval	true/false
	 */
	void SetOutOfSequence();

	/**
	 *	@brief	split addjust
	 *
	 *	@param	sharesBefore Shares before adjusting
	 *	@param	sharesAfter Shares after adjusting
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 */
	ACTIV_FEED_API_COMMON_API StatusCode ProcessSplit(const int sharesBefore, const int sharesAfter);

	/**
	 *	@brief	Compress a field set.
	 *
	 *	@param	previous previous field set in this stream.
	 *	@param	bitStream bit stream buffer for compression.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_BUFFER_OVERFLOW
	 *	@retval	...
	 */
	//ACTIV_FEED_API_COMMON_API virtual StatusCode Compress(const FieldSet &previous, BitBuilder &builder) const;

	/**
	 *	@brief	Decompress a field set.
	 *
	 *	@param	previous previous field set in this stream.
	 *	@param	bitStream bit stream buffer for compression.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_BUFFER_OVERFLOW
	 *	@retval	...
	 */
	//ACTIV_FEED_API_COMMON_API virtual StatusCode Decompress(const FieldSet &previous, BitValidator &validator);

protected:
	static const size_t CONDITION_LENGTH = 4;

	#define ACTIV_TICK_MASK_LIST(d)																				\
		d(0x0F,	TICK_TYPE_MASK)				/** used to share flags with ticktype byte for serialized data */	\
		d(0xF0,	TICK_CONDITION_LENGTH_MASK)																		\
		d(0x80,	TICK_OUT_SEQUENCE_MASK)																			\
		d(0x7F,	TICK_DAY_MASK)

	ACTIV_TICK_MASK_LIST(ACTIV_DECLARE_BYTES)

	byte_t                 m_conditions[CONDITION_LENGTH];		///< tick conditions.
	DateTime			   m_dateTime;			///< date of the tick.
	Rational               m_price;				///< tick price.
	Rational               m_size;				///< size.
	std::string            m_exchange;			///< exchange code.
	TickType               m_tickType;			///< tick type.
	Feed::UpdateId         m_updateId;			///< sequence number of the tick.
	bool				   m_outSequence;		///< if the time is out of sequence
};

typedef FieldSetList<Tick, uint16_t> TickList;

// ---------------------------------------------------------------------------------------------------------------------------------

inline bool Tick::operator==(const Tick &rhs) const
{
	return m_dateTime == rhs.m_dateTime;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline bool Tick::operator!=(const Tick &rhs) const
{
	return m_dateTime != rhs.m_dateTime;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline bool Tick::operator<(const Tick &rhs) const
{
	return m_dateTime < rhs.m_dateTime;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline bool Tick::operator<=(const Tick &rhs) const
{
	return m_dateTime <= rhs.m_dateTime;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline bool Tick::operator>(const Tick &rhs) const
{
	return m_dateTime > rhs.m_dateTime;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline bool Tick::operator>=(const Tick &rhs) const
{
	return (m_dateTime >= rhs.m_dateTime);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode Tick::Compare(const DateTime &dateTime, int &result) const
{
	return m_dateTime.Compare(dateTime, result);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode Tick::Compare(const Tick &rhs, int &result) const
{
	return m_dateTime.Compare(rhs.m_dateTime, result);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline bool Tick::Compare(const Tick &rhs) const
{
	return CompareContentOnly(rhs) && (m_dateTime == rhs.m_dateTime); 
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline bool Tick::CompareContentOnly(const Tick &rhs) const
{
	if (m_price != rhs.m_price
		|| m_size != rhs.m_size
		|| m_exchange != rhs.m_exchange)
		return false;
	return true;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline bool Tick::operator<(const DateTime &dateTime) const
{
	return m_dateTime < dateTime;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline bool Tick::operator<=(const DateTime &dateTime) const
{
	return m_dateTime <= dateTime;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline	bool Tick::operator ==(const DateTime &dateTime) const
{
	return m_dateTime == dateTime;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline bool Tick::operator >(const DateTime &dateTime) const
{
	return m_dateTime > dateTime;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline bool Tick::operator >=(const DateTime &dateTime) const
{
	return m_dateTime >= dateTime;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode Tick::GetConditions(byte_t *pConditions, const size_t conditionsLength, size_t &conditionsSize) const
{
	if (conditionsLength < CONDITION_LENGTH)
		return STATUS_CODE_INVALID_LENGTH;

	::memcpy(pConditions, m_conditions, CONDITION_LENGTH);
	conditionsSize = CONDITION_LENGTH;
	return STATUS_CODE_SUCCESS;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode Tick::SetConditions(const byte_t *pConditions, const size_t conditionsLength)
{
	if (conditionsLength > CONDITION_LENGTH)
		return STATUS_CODE_INVALID_LENGTH;

	::memset(m_conditions, 0, CONDITION_LENGTH);
	::memcpy(m_conditions, pConditions, conditionsLength);
	return STATUS_CODE_SUCCESS;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode Tick::SetConditions(const IFieldType &field)
{
	if (FIELD_TYPE_BINARY_STRING != field.GetType()) 
		return STATUS_CODE_INVALID_FIELD_TYPE;

	const byte_t  *pData;
	size_t        size;

	static_cast<const BinaryString&>(field).Get(pData, size);
	return SetConditions(pData, size);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode Tick::GetPrice(Rational &price) const
{
	price = m_price;
	return STATUS_CODE_SUCCESS;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode Tick::GetPrice(double &price) const
{
	return m_price.GetDouble(price);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode Tick::SetPrice(const Rational &price)
{
	m_price = price;
	return STATUS_CODE_SUCCESS;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline	StatusCode Tick::SetPrice(const IFieldType &field)
{
	if (FIELD_TYPE_TRATIONAL == field.GetType())
		return SetPrice(static_cast<const TRational&>(field).GetRational());
	else if (FIELD_TYPE_RATIONAL == field.GetType())
		return SetPrice(static_cast<const Rational&>(field));
	else
		return STATUS_CODE_INVALID_FIELD_TYPE;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode Tick::GetExchange(std::string &exchange) const
{
	exchange = m_exchange;
	return STATUS_CODE_SUCCESS;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline const std::string & Tick::GetExchange() const
{
	return m_exchange;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode Tick::SetExchange(const std::string &exchange)
{
	m_exchange = exchange;
	return STATUS_CODE_SUCCESS;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline TickType Tick::GetTickType() const
{
	return m_tickType;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode Tick::SetTickType(const TickType type)
{
	m_tickType = type;
	return STATUS_CODE_SUCCESS;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode Tick::GetDateTime(DateTime &dateTime) const
{
	dateTime = m_dateTime;
	return STATUS_CODE_SUCCESS;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode Tick::SetDateTime(const DateTime &dateTime)
{
	m_dateTime = dateTime;
	return STATUS_CODE_SUCCESS;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode Tick::SetDateTime(const Date &date, const Time &time)
{
	return m_dateTime.Set(date,time);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode Tick::SetDate(const Date &date)
{
	return m_dateTime.Set(date, m_dateTime.GetTime());
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode Tick::SetTime(const Time &time)
{
	return m_dateTime.Set(m_dateTime.GetDate(), time);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode Tick::GetSize(Rational &size) const
{
	size = m_size;
	return STATUS_CODE_SUCCESS;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode Tick::SetSize(const Rational &size)
{
	m_size = size;
	return STATUS_CODE_SUCCESS;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode Tick::SetSize(const IFieldType &field)
{

	if (FIELD_TYPE_UINT == field.GetType())
		return SetSize((uint32_t) static_cast<const UInt&>(field).Get());
	else if (FIELD_TYPE_RATIONAL == field.GetType())
		return SetSize(static_cast<const Rational&>(field));
	return STATUS_CODE_INVALID_FIELD_TYPE;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode Tick::GetSize(uint32_t &size) const
{
	if(Rational::DENOMINATOR_WHOLE == m_size.GetDenominator())
		size = static_cast<uint32_t>(m_size.GetNumerator());
	else
	{
		double dSize;
		ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, m_size.GetDouble(dSize));
		size = static_cast<uint32_t>(dSize);
	}
	return STATUS_CODE_SUCCESS;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode Tick::SetSize(const uint32_t size)
{
	return m_size.Set(size, Rational::DENOMINATOR_WHOLE);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode Tick::GetUpdateId(Feed::UpdateId &updateId) const
{
	updateId = m_updateId;
	return STATUS_CODE_SUCCESS;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline Feed::UpdateId Tick::GetUpdateId() const
{
	return m_updateId;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode Tick::SetUpdateId(const Feed::UpdateId updateId)
{
	m_updateId = updateId;
	return STATUS_CODE_SUCCESS;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline bool Tick::IsRegularHourTrade() const
{
	return m_tickType == TICK_TYPE_TRADE;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline bool Tick::IsTrade() const
{
	return m_tickType == TICK_TYPE_TRADE || m_tickType == TICK_TYPE_FORM_T_TRADE;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline bool Tick::IsOutOfSequence() const
{
	return m_outSequence;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline void Tick::SetOutOfSequence()
{
	m_outSequence = true;
}

#undef ACTIV_DECLARE_BYTES

} // namespace FeedApi

} // namespace ContentPlatform

} // namespace Activ

#endif // (!defined ACTIV_FEED_API_COMMON_TICK_H)

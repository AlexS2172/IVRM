/**
 *	@file	Tick.h
 *
 *	@brief	Tick Field Set.
 *
 *	$Log: $
 */

#if !defined (ACTIV_CONTENT_GATEWAY_COMMON_TICK_H)
#define ACTIV_CONTENT_GATEWAY_COMMON_TICK_H

#include "ActivContentPlatform/ActivContentGatewayCommon/External.h"
#include "ActivContentPlatform/ActivContentGatewayCommon/FieldSet.h"

#include "ActivMiddleware/ActivFieldTypes/Date.h"
#include "ActivMiddleware/ActivFieldTypes/Time.h"
#include "ActivMiddleware/ActivFieldTypes/Rational.h"
#include "ActivContentPlatform/ActivFeedCommon/UpdateIds.h"

namespace Activ
{

namespace ContentPlatform
{

//	@brief	Tick types.

#define TICK_TYPE_LIST(d)										\
	d(	TICK_TYPE_NONE,				"None"					)	\
	d(	TICK_TYPE_TRADE,			"Trade"					)	\
	d(	TICK_TYPE_CORRECTED_TRADE,	"Corrected Trade"		)	\
	d(	TICK_TYPE_DELETED_TRADE,	"Deleted Trade"			)	\
	d(	TICK_TYPE_FORM_T_TRADE,		"Form T Trade"			)	\
	d(	TICK_TYPE_FILTERED_TRADE,	"Filtered Trade"		)	\
	d(	TICK_TYPE_BID,				"Bid"					)	\
	d(	TICK_TYPE_ASK,				"Ask"					)	\
	d(	TICK_TYPE_MID,				"Mid"					)	\
	d(	TICK_TYPE_SPREAD,			"Spread"				)	\
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
	ACTIV_CONTENT_GATEWAY_COMMON_API Tick();

	/**
	 *	@brief	Destructor.
	 */
	ACTIV_CONTENT_GATEWAY_COMMON_API virtual ~Tick();

	/**
	 *	@brief	Reset all values in the field set.
	 */
	ACTIV_CONTENT_GATEWAY_COMMON_API virtual void Reset();

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
	ACTIV_CONTENT_GATEWAY_COMMON_API virtual StatusCode GetField(const Feed::FieldId fieldId, IFieldType &field) const;

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
	ACTIV_CONTENT_GATEWAY_COMMON_API virtual StatusCode SetField(const Feed::FieldId fieldId, const IFieldType &field);

	/**
	 *	@brief	Get the field set type.
	 *
	 *	@return	the field set type.
	 */
	ACTIV_CONTENT_GATEWAY_COMMON_API virtual FieldSetType GetType() const;

	/**
	 *	@brief	Convert the object to a string.
	 *
	 *	@return	the object as a string.
	 */
	ACTIV_CONTENT_GATEWAY_COMMON_API virtual std::string ToString(const std::string &deliminator = DEFAULT_STRING_FIELD_SEPARATER) const;

	/**
	 *	@brief	Set the object value from a string.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *  @retval ......
	 */
	ACTIV_CONTENT_GATEWAY_COMMON_API virtual StatusCode FromString(const std::string &inputStr, const std::string &deliminator = DEFAULT_STRING_FIELD_SEPARATER);

	/**
	 *	@brief	Serialize the object into the supplied message builder.
	 *
	 *	@param	messageBuilder the message builder to store a serialized version of the object in.
	 *	@param	maxSerializedBodyLength the maximum serialized body length.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
	ACTIV_CONTENT_GATEWAY_COMMON_API virtual StatusCode Serialize(MessageBuilder &messageBuilder) const;

	/**
	 *	@brief	Deserialize the object from the supplied message validater.
	 *
	 *	@param	messageValidater the message validater from which the object will be extracted.
	 *	@param	maxSerializedBodyLength the maximum serialized body length.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
	ACTIV_CONTENT_GATEWAY_COMMON_API virtual StatusCode Deserialize(MessageValidater &messageValidater);

	/**
	 *	@brief Get the serialized length of the object.
	 *
	 *	@return	the serialized length of the object.
	 */
	ACTIV_CONTENT_GATEWAY_COMMON_API virtual size_t GetSerializedLength() const;

	/**
	 *	@brief	Get the maximum serialized length.
	 *
	 *	@return	the maximum serialized length.
	 */
	ACTIV_CONTENT_GATEWAY_COMMON_API static size_t GetMaxSerializedLength();

	/**
	 *	@brief	compares all fields except tick type.
	 *
	 *	@param	rhs the object to compare with.
	 *
	 *	@return	true if the same, otherwise false
	 */
	ACTIV_CONTENT_GATEWAY_COMMON_API bool Compare(const Tick &rhs) const;

	/**
	 *	@brief	compares dateTime
	 *
	 *	@param	rhs the object to compare with.
	 *
	 *	@return	true if the same, otherwise false
	 */
	ACTIV_CONTENT_GATEWAY_COMMON_API StatusCode Compare(const DateTime &dateTime, int &result) const;

	/**
	 *	@brief	Equality operator.
	 *
	 *	@param	rhs the object to compare with.
	 *
	 *	@return	the result of the test.
	 */
	ACTIV_CONTENT_GATEWAY_COMMON_API bool operator==(const Tick &rhs) const;

	/**
	 *	@brief	Inequality operator.
	 *
	 *	@param	rhs the object to compare with.
	 *
	 *	@return	the result of the test.
	 */
	ACTIV_CONTENT_GATEWAY_COMMON_API bool operator!=(const Tick &rhs) const;

	/**
	 *	@brief	Less than operator (date and time only).
	 *
	 *	@param	rhs the object to compare with.
	 *
	 *	@return	the result of the test.
	 */
	ACTIV_CONTENT_GATEWAY_COMMON_API bool operator<(const Tick &rhs) const;

	/**
	 *	@brief	Less than or equal operator (date and time only).
	 *
	 *	@param	rhs the object to compare with.
	 *
	 *	@return	the result of the test.
	 */
	ACTIV_CONTENT_GATEWAY_COMMON_API bool operator<=(const Tick &rhs) const;

	/**
	 *	@brief	Greater than operator (date and time only).
	 *
	 *	@param	rhs the object to compare with.
	 *
	 *	@return	the result of the test.
	 */
	ACTIV_CONTENT_GATEWAY_COMMON_API bool operator>(const Tick &rhs) const;

	/**
	 *	@brief	Greater than or equal operator (date and time only).
	 *
	 *	@param	rhs the object to compare with.
	 *
	 *	@return	the result of the test.
	 */
	ACTIV_CONTENT_GATEWAY_COMMON_API bool operator>=(const Tick &rhs) const;
	
	/**
	 *	@brief	<= operator
	 *
	 *	@param	dateTime dateTime to compare
	 *
	 *	@retval true/false
	 */
	ACTIV_CONTENT_GATEWAY_COMMON_API bool operator <(const DateTime &dateTime) const;
	ACTIV_CONTENT_GATEWAY_COMMON_API bool operator <=(const DateTime &dateTime) const;

	/**
	 *	@brief	== operator
	 *
	 *	@param	dateTime dateTime to compare
	 *
	 *	@retval true/false
	 */
	ACTIV_CONTENT_GATEWAY_COMMON_API bool operator ==(const DateTime &dateTime) const;

	/**
	 *	@brief	>= operator
	 *
	 *	@param	dateTime dateTime to compare
	 *
	 *	@retval true/false
	 */
	ACTIV_CONTENT_GATEWAY_COMMON_API bool operator >=(const DateTime &dateTime) const;
	ACTIV_CONTENT_GATEWAY_COMMON_API bool operator >(const DateTime &dateTime) const;

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
	ACTIV_CONTENT_GATEWAY_COMMON_API StatusCode GetConditions(byte_t *pConditions, const size_t conditionsLength, size_t &conditionsSize) const;

	/**
	 *	@brief	Set the conditions.
	 *
	 *	@param	pConditions conditions buffer.
	 *	@param	conditionsLength length of conditions.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_LENGTH
	 */
	ACTIV_CONTENT_GATEWAY_COMMON_API StatusCode SetConditions(const byte_t *pConditions, const size_t conditionsLength);

	/**
	 *	@brief	Get the price.
	 *
	 *	@param	price the price of the tick.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_LENGTH
	 */
	ACTIV_CONTENT_GATEWAY_COMMON_API StatusCode GetPrice(double &price) const;

	/**
	 *	@brief	Get the price as a 64 bit signed integer.
	 *
	 *	@return	price.
	 */
	ACTIV_CONTENT_GATEWAY_COMMON_API StatusCode GetPrice(Rational &price) const;

	/**
	 *	@brief	Set the price.
	 *
	 *	@param	price the price of the tick.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_LENGTH
	 */
	ACTIV_CONTENT_GATEWAY_COMMON_API StatusCode SetPrice(const Rational &price);

	/**
	 *	@brief	Get the exchange.
	 *
	 *	@param	exchange the exchange of the tick.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_LENGTH
	 */	
	ACTIV_CONTENT_GATEWAY_COMMON_API StatusCode GetExchange(std::string &exchange) const;

	/**
	 *	@brief	Get the exchange.
	 *
	 *	@param	exchange the exchange of the tick.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_LENGTH
	 */	
	ACTIV_CONTENT_GATEWAY_COMMON_API const std::string & GetExchange() const;

	/**
	 *	@brief	Set the exchange.
	 *
	 *	@param	exchange the exchange of the tick.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_LENGTH
	 */	
	ACTIV_CONTENT_GATEWAY_COMMON_API StatusCode SetExchange(const std::string &exchange);

	/**
	 *	@brief	Get the tick's type.
	 *
	 *	@return	return the tick type.
	 */	
	ACTIV_CONTENT_GATEWAY_COMMON_API TickType GetTickType() const;

	/**
	 *	@brief	Set the tick's type.
	 *
	 *	@param	type the tick's type.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_LENGTH
	 */	
	ACTIV_CONTENT_GATEWAY_COMMON_API StatusCode SetTickType(const TickType type);

	/**
	 *	@brief	Get the time.
	 *
	 *	@param	time the time of the tick.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_LENGTH
	 */
	ACTIV_CONTENT_GATEWAY_COMMON_API StatusCode GetDateTime(DateTime &dateTime) const;

	/**
	 *	@brief	Set the time.
	 *
	 *	@param	time the time of the tick.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_LENGTH
	 */
	ACTIV_CONTENT_GATEWAY_COMMON_API StatusCode SetDateTime(const DateTime &dateTime);
	
	/**
	 *	@brief	Set the time.
	 *
	 *	@param	time the time of the tick.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_LENGTH
	 */
	ACTIV_CONTENT_GATEWAY_COMMON_API StatusCode SetDateTime(const Date &date, const Time &time);

	/**
	 *	@brief	Set the time.
	 *
	 *	@param	time the time of the tick.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_LENGTH
	 */
	ACTIV_CONTENT_GATEWAY_COMMON_API StatusCode SetDate(const Date &date);

	/**
	 *	@brief	Set the time.
	 *
	 *	@param	time the time of the tick.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_LENGTH
	 */
	ACTIV_CONTENT_GATEWAY_COMMON_API StatusCode SetTime(const Time &time);
	
	/**
	 *	@brief	Get the tick's size.
	 *
	 *	@param	size the size of the tick.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_LENGTH
	 */
	ACTIV_CONTENT_GATEWAY_COMMON_API StatusCode GetSize(Rational &size) const;

	/**
	 *	@brief	Set the tick's size.
	 *
	 *	@param	size the size of the tick.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_LENGTH
	 */
	ACTIV_CONTENT_GATEWAY_COMMON_API StatusCode SetSize(const Rational size);
	
	/**
	 *	@brief	Get the tick's size.
	 *
	 *	@param	size the size of the tick.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_LENGTH
	 */
	ACTIV_CONTENT_GATEWAY_COMMON_API StatusCode GetSize(uint32_t &size) const;

	/**
	 *	@brief	Set the tick's size.
	 *
	 *	@param	size the size of the tick.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_LENGTH
	 */
	ACTIV_CONTENT_GATEWAY_COMMON_API StatusCode SetSize(const uint32_t size);

	/**
	 *	@brief	Get the sequence number of the tick.
	 *
	 *	@param	sequence the sequence number of the tick.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_LENGTH
	 */
	ACTIV_CONTENT_GATEWAY_COMMON_API StatusCode GetUpdateId(Feed::UpdateId &updateId) const;

	/**
	 *	@brief	Set the sequence number of the tick.
	 *
	 *	@param	sequence the sequence number of the tick.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_LENGTH
	 */
	ACTIV_CONTENT_GATEWAY_COMMON_API StatusCode SetUpdateId(const Feed::UpdateId updateId);

	/**
	 *	@brief	check if this tick is a regular hour trade
	 *
	 *	@retval	true/false
	 */
	ACTIV_CONTENT_GATEWAY_COMMON_API bool IsRegularHourTrade() const;

	/**
	 *	@brief	check if this tick is a trade
	 *
	 *	@retval	true/false
	 */
	ACTIV_CONTENT_GATEWAY_COMMON_API bool IsTrade() const;

	/**
	 *	@brief	check if this tick is out of sequence in terms of time
	 *
	 *	@retval	true/false
	 */
	ACTIV_CONTENT_GATEWAY_COMMON_API bool IsOutOfSequence() const;

	/**
	 *	@brief	check if this tick is out of sequence in terms of time
	 *
	 *	@retval	true/false
	 */
	ACTIV_CONTENT_GATEWAY_COMMON_API void SetOutOfSequence();

	/**
	 *	@brief	split addjust
	 *
	 *	@param	sharesBefore Shares before adjusting
	 *	@param	sharesAfter Shares after adjusting
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 */
	ACTIV_CONTENT_GATEWAY_COMMON_API StatusCode ProcessSplit(const int sharesBefore, const int sharesAfter);

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
	//ACTIV_CONTENT_GATEWAY_COMMON_API virtual StatusCode Compress(const FieldSet &previous, BitBuilder &builder) const;

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
	//ACTIV_CONTENT_GATEWAY_COMMON_API virtual StatusCode Decompress(const FieldSet &previous, BitValidater &validater);

private:
	static const byte_t  TICK_TYPE_MASK			= 0x0F; //used to share flages with ticktype byte for seriealized data
	static const byte_t  TICK_CONDITION_LENGTH_MASK		= 0xF0;

	static const byte_t  TICK_OUT_SEQUENCE_MASK			= 0x80;
	static const byte_t  TICK_DAY_MASK					= 0x7F;


	byte_t                 m_conditions[4];		///< tick conditions.
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
	if (conditionsLength < sizeof(m_conditions))
		return STATUS_CODE_INVALID_LENGTH;

	::memcpy(pConditions, m_conditions, sizeof(m_conditions));
	conditionsSize = sizeof(m_conditions);
	return STATUS_CODE_SUCCESS;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode Tick::SetConditions(const byte_t *pConditions, const size_t conditionsLength)
{
	::memcpy(m_conditions, pConditions, sizeof(m_conditions));
	if (conditionsLength < sizeof(m_conditions))
		::memset(&m_conditions[conditionsLength], 0, sizeof(m_conditions)-conditionsLength);
	return STATUS_CODE_SUCCESS;
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

inline StatusCode Tick::SetSize(const Rational size)
{
	m_size = size;
	return STATUS_CODE_SUCCESS;
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

} // namespace ContentPlatform

} // namespace Activ

#endif // !defined (ACTIV_CONTENT_GATEWAY_COMMON_TICK_H)

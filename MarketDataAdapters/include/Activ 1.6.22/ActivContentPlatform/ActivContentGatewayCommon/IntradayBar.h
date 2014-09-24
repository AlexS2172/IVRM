/**
 *	@file	IntradayBar.h
 *
 *	@brief	Intraday Bar Field Set.
 *
 *	$Log: $
 */

#if !defined (ACTIV_CONTENT_GATEWAY_COMMON_INTRADAY_BAR_H)
#define ACTIV_CONTENT_GATEWAY_COMMON_INTRADAY_BAR_H

#include "External.h"
#include "ActivContentPlatform/ActivContentGatewayCommon/FieldSet.h"

#include "ActivMiddleware/ActivFieldTypes/Date.h"
#include "ActivMiddleware/ActivFieldTypes/Time.h"
#include "ActivMiddleware/ActivFieldTypes/Rational.h"

#include "ActivMiddleware/Misc/SerializableContainerHelper.h"

namespace Activ
{

namespace ContentPlatform
{

// ---------------------------------------------------------------------------------------------------------------------------------

// forward reference

class Tick;

// ---------------------------------------------------------------------------------------------------------------------------------

/**
 *	@brief	Intraday Bar Field Set class.
 */
class IntradayBar : public FieldSet
{
public:	
	static const byte_t	NONE_REGULAR_HOUR_TRADE	= 0x01;

	/**
	 *	@brief	Constructor.
	 */
	ACTIV_CONTENT_GATEWAY_COMMON_API IntradayBar();

	/**
	 *	@brief	Destructor.
	 */
	ACTIV_CONTENT_GATEWAY_COMMON_API virtual ~IntradayBar();

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
	 *	@brief	Full compare.
	 *
	 *	@param	rhs the object to compare with.
	 *
	 *	@return	the result of the test.
	 */
	ACTIV_CONTENT_GATEWAY_COMMON_API bool Compare(const IntradayBar &rhs) const;

	/**
	 *	@brief compares dateTime only
	 *
	 *	@param rhs the object to compare with.
	 *	@param result the result of the comparison. 0 date time is the same. 1 date time is later than rhs's.  -1 date time is earlier than rhs's
	 *
	 *	@retval STATUS_CODE_SUCCESS
	 *	@retval ...
	 */
	StatusCode Compare(const IntradayBar &rhs, int &result) const;

	/**
	 *	@brief	Equality operator.
	 *
	 *	@param	rhs the object to compare with.
	 *
	 *	@return	the result of the test.
	 */
	bool operator==(const IntradayBar &rhs) const;

	/**
	 *	@brief	Inequality operator.
	 *
	 *	@param	rhs the object to compare with.
	 *
	 *	@return	the result of the test.
	 */
	bool operator!=(const IntradayBar &rhs) const;

	/**
	 *	@brief	Less than operator (date only).
	 *
	 *	@param	rhs the object to compare with.
	 *
	 *	@return	the result of the test.
	 */
	bool operator<(const IntradayBar &rhs) const;

	/**
	 *	@brief	Less than or equal operator (date only).
	 *
	 *	@param	rhs the object to compare with.
	 *
	 *	@return	the result of the test.
	 */
	bool operator<=(const IntradayBar &rhs) const;

	/**
	 *	@brief	Greater than operator (date only).
	 *
	 *	@param	rhs the object to compare with.
	 *
	 *	@return	the result of the test.
	 */
	bool operator>(const IntradayBar &rhs) const;

	/**
	 *	@brief	Greater than or equal operator (date only).
	 *
	 *	@param	rhs the object to compare with.
	 *
	 *	@return	the result of the test.
	 */
	bool operator>=(const IntradayBar &rhs) const;

	/**
	 *	@brief	< operator
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
	 *	@brief	> operator
	 *
	 *	@param	dateTime dateTime to compare
	 *
	 *	@retval true/false
	 */
	bool operator >(const DateTime &dateTime) const;
	bool operator >=(const DateTime &dateTime) const;

	/**
	 *	@brief	Set the date.
	 *
	 *	@param	date the date of the bar.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_LENGTH
	 */
	StatusCode SetDate(const Date &date);

	/**
	 *	@brief	Set the time.
	 *
	 *	@param	time the time of the bar.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_LENGTH
	 */
	StatusCode SetTime(const Time &time);
		
	/**
	 *	@brief	Get the time.
	 *
	 *	@param	time the time of the bar.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_LENGTH
	 */
	StatusCode GetDateTime(DateTime &dateTime) const;
		
	/**
	 *	@brief	Get the time.
	 *
	 *	@param	time the time of the bar.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_LENGTH
	 */
	StatusCode SetDateTime(const DateTime &dateTime);

	/**
	 *	@brief	Get the open price.
	 *
	 *	@param	price the open price of the bar.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_LENGTH
	 */
	StatusCode GetOpenPrice(Rational &price) const;

	/**
	 *	@brief	Get the open price.
	 *
	 *	@param	price the open price of the bar.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_LENGTH
	 */
	StatusCode GetOpenPrice(double &price) const;

	/**
	 *	@brief	Set the open price.
	 *
	 *	@param	price the open price of the bar.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_LENGTH
	 */
	StatusCode SetOpenPrice(const Rational &price);

	/**
	 *	@brief	Get the high price.
	 *
	 *	@param	price the high price of the bar.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_LENGTH
	 */
	StatusCode GetHighPrice(Rational &price) const;

	/**
	 *	@brief	Get the high price.
	 *
	 *	@param	price the high price of the bar.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_LENGTH
	 */
	StatusCode GetHighPrice(double &price) const;

	/**
	 *	@brief	Set the high price.
	 *
	 *	@param	price the high price of the bar.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_LENGTH
	 */
	StatusCode SetHighPrice(const Rational &price);

	/**
	 *	@brief	Get the low price.
	 *
	 *	@param	price the low price of the bar.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_LENGTH
	 */
	StatusCode GetLowPrice(Rational &price) const;

	/**
	 *	@brief	Get the low price.
	 *
	 *	@param	price the low price of the bar.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_LENGTH
	 */
	StatusCode GetLowPrice(double &price) const;

	/**
	 *	@brief	Set the low price.
	 *
	 *	@param	price the low price of the bar.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_LENGTH
	 */
	StatusCode SetLowPrice(const Rational &price);

	/**
	 *	@brief	Get the close price.
	 *
	 *	@param	price the close price of the bar.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_LENGTH
	 */
	StatusCode GetClosePrice(Rational &price) const;

	/**
	 *	@brief	Get the close price.
	 *
	 *	@param	price the close price of the bar.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_LENGTH
	 */
	StatusCode GetClosePrice(double &price) const;

	/**
	 *	@brief	Set the close price.
	 *
	 *	@param	price the close price of the bar.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_LENGTH
	 */
	StatusCode SetClosePrice(const Rational &price);

	/**
	 *	@brief	Get the bar's total volume.
	 *
	 *	@param	volume the total volume of the bar.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_LENGTH
	 */
	StatusCode GetTotalVolume(uint32_t &volume) const;

	/**
	 *	@brief	Get the bar's total volume.
	 *
	 *	@param	volume the total volume of the bar.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_LENGTH
	 */
	StatusCode SetTotalVolume(const uint32_t volume);

	/**
	 *	@brief	Get the bar's total volume.
	 *
	 *	@param	volume the total volume of the bar.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_LENGTH
	 */
	StatusCode GetTotalVolume(Rational &volume) const;

	/**
	 *	@brief	Set the bar's total volume.
	 *
	 *	@param	volume the volume of the bar.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_LENGTH
	 */
	StatusCode SetTotalVolume(const Rational& volume);

	/**
	 *	@brief	Get the bar's total price.
	 *
	 *	@param	totalPrice the total price of the bar.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_LENGTH
	 */
	StatusCode GetTotalPrice(Rational &totalPrice) const;

	/**
	 *	@brief	Get the bar's total price.
	 *
	 *	@param	totalPrice the total price of the bar.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_LENGTH
	 */
	StatusCode GetTotalPrice(double &totalPrice) const;

	/**
	 *	@brief	Set the bar's total price.
	 *
	 *	@param	totalPrice the total price of the bar.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_LENGTH
	 */
	StatusCode SetTotalPrice(Rational &totalPrice);

	/**
	 *	@brief	Get the bar's total value.
	 *
	 *	@param	totalValue the total value of the bar.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_LENGTH
	 */
	StatusCode GetTotalValue(Rational &totalValue) const;

	/**
	 *	@brief	Get the bar's total value.
	 *
	 *	@param	totalValue the total value of the bar.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_LENGTH
	 */
	StatusCode GetTotalValue(double &totalValue) const;

	/**
	 *	@brief	Set the bar's total value.
	 *
	 *	@param	totalValue the total value of the bar.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_LENGTH
	 */
	StatusCode SetTotalValue(Rational &totalValue);

	/**
	 *	@brief	Get the bar's tick count.
	 *
	 *	@param	count the tick count of the bar.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_LENGTH
	 */
	StatusCode GetTickCount(uint32_t &count) const;

	/**
	 *	@brief	Set the bar's tick count.
	 *
	 *	@param	count the tick count of the bar.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_LENGTH
	 */
	StatusCode SetTickCount(const uint32_t count);

	/**
	 *	@brief	Set the bar's flag.
	 *
	 *	@param	flag flag to set
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 */
	StatusCode SetFlag(const byte_t flag);

	/**
	 *	@brief	Get the bar's flag.
	 *
	 *	@param	flag retrived flag
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 */
	StatusCode GetFlag(byte_t &flag) const;

	/**
	 *	@brief	Get the bar's flag.
	 *
	 *	@param	flag retrived flag
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 */
	bool IsRegularHour() const;

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

	/**
	 *	@brief	Build/update a bar with this tick.
	 *
	 *	@param	period target bar's period.
	 *	@param	tick tick to apply.
	 *
	 *	@retval	UPDATE_STATUS_OK
	 *	@retval	UPDATE_STATUS_FLUSH
	 *	@retval	UPDATE_STATUS_IGNORE
	 */
	ACTIV_CONTENT_GATEWAY_COMMON_API FieldSet::UpdateFieldSetStatus Update(const unsigned period, Tick &tick);

	/**
	 *	@brief	add bar to an existing bar, e.g., building a 5 minute bar from 1 minute bars
	 *
	 *	@param	period target bar's period
	 *	@param	bar new bar to be added.
	 *
	 *	@retval	UPDATE_STATUS_OK
	 *	@retval	UPDATE_STATUS_FLUSH
	 *	@retval	UPDATE_STATUS_IGNORE
	 */
	ACTIV_CONTENT_GATEWAY_COMMON_API FieldSet::UpdateFieldSetStatus Update(const unsigned period, const IntradayBar &bar);

	/**
	 *	@brief	remove a trade (for canceled trade.
	 *
	 *	@param	canceledTrade trade to remove
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 */
	ACTIV_CONTENT_GATEWAY_COMMON_API StatusCode RemoveTrade(const Tick &canceledTrade);

	/**
	 *	@brief	Set the extra value fields to 0
	 */
	void SetToMiniBar();

	/**
	 *	@brief	Check if the bar has any trade in it
	 */
	bool HasTick() const;

private:
#if 0
	enum BaseType
	{
		BASE_TYPE_LIST(ACTIV_DECLARE_ELEMENT_1_WITH_COMMA)
		NUM_BASE_TYPES,
	};

	/**
	 *	@brief	Get the base price (lowest price of open, high, low, close).
	 *
	 *	@return	the base price.
	 */
	int64_t GetBasePrice() const;
	
	/**
	 *	@brief	Get the base price (lowest price of open, high, low, close).
	 *
	 *	@param	basePrice base price.
	 *	@param	baseType type of base price.
	 */
	void GetBasePrice(int64_t &basePrice, BaseType &baseType) const;

	/**
	 *	@brief	Scale the prices.
	 *
	 *	@param	price new price.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 */
	StatusCode ScalePrice(Rational &price);
	
	/**
	 *	@brief	Scale all prices.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 */
	StatusCode ScalePrices(const Rational::Denominator denominator);
#endif

private:
	DateTime               m_dateTime;			///< date and time.
	Rational               m_open;				///< open price.
	Rational               m_high;				///< high price.
	Rational               m_low;				///< low price.
	Rational               m_close;				///< closing price.
	Rational               m_totalPrice;		///< total price traded in the period.
	Rational               m_totalValue;		///< total value traded in the period.
	Rational               m_totalVolume;		///< total volume.
	uint32_t               m_tickCount;			///< tick count.
	byte_t				   m_flag;				///< None regular hour trade?
};

typedef FieldSetList<IntradayBar, uint16_t> IntradayBarList;

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode IntradayBar::Compare(const IntradayBar &rhs, int &result) const
{
	return m_dateTime.Compare(rhs.m_dateTime, result);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline bool IntradayBar::operator==(const IntradayBar &rhs) const
{
	return m_dateTime == rhs.m_dateTime;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline bool IntradayBar::operator!=(const IntradayBar &rhs) const
{
	return m_dateTime != rhs.m_dateTime;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline bool IntradayBar::operator<(const IntradayBar &rhs) const
{
	return m_dateTime < rhs.m_dateTime;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline bool IntradayBar::operator<=(const IntradayBar &rhs) const
{
	return m_dateTime <= rhs.m_dateTime;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline bool IntradayBar::operator>(const IntradayBar &rhs) const
{
	return m_dateTime > rhs.m_dateTime;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline bool IntradayBar::operator>=(const IntradayBar &rhs) const
{
	return m_dateTime >= rhs.m_dateTime;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline bool IntradayBar::operator<(const DateTime &dateTime) const
{
	return m_dateTime < dateTime;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline bool IntradayBar::operator<=(const DateTime &dateTime) const
{
	return m_dateTime <= dateTime;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline	bool IntradayBar::operator ==(const DateTime &dateTime) const
{
	return m_dateTime == dateTime;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline bool IntradayBar::operator >(const DateTime &dateTime) const
{
	return m_dateTime > dateTime;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline bool IntradayBar::operator >=(const DateTime &dateTime) const
{
	return m_dateTime >= dateTime;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode IntradayBar::GetDateTime(DateTime &dateTime) const
{
	dateTime = m_dateTime;
	return STATUS_CODE_SUCCESS;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode IntradayBar::SetDateTime(const DateTime &dateTime)
{
	m_dateTime = dateTime;
	return STATUS_CODE_SUCCESS;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode IntradayBar::SetDate(const Date &date)
{
	m_dateTime.Set(date, m_dateTime.GetTime());
	return STATUS_CODE_SUCCESS;
}
// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode IntradayBar::SetTime(const Time &time)
{
	m_dateTime.Set(m_dateTime.GetDate(), time);
	return STATUS_CODE_SUCCESS;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode IntradayBar::GetOpenPrice(Rational &price) const
{
	price = m_open;
	return STATUS_CODE_SUCCESS;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode IntradayBar::GetOpenPrice(double &price) const
{
	return m_open.GetDouble(price);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode IntradayBar::SetOpenPrice(const Rational &price)
{
	m_open = price;
	return STATUS_CODE_SUCCESS;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode IntradayBar::GetHighPrice(Rational &price) const
{
	price = m_high;
	return STATUS_CODE_SUCCESS;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode IntradayBar::GetHighPrice(double &price) const
{
	return m_high.GetDouble(price);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode IntradayBar::SetHighPrice(const Rational &price)
{
	m_high = price;
	return STATUS_CODE_SUCCESS;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode IntradayBar::GetLowPrice(Rational &price) const
{
	price = m_low;
	return STATUS_CODE_SUCCESS;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode IntradayBar::GetLowPrice(double &price) const
{
	return m_low.GetDouble(price);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode IntradayBar::SetLowPrice(const Rational &price)
{
	m_low = price;
	return STATUS_CODE_SUCCESS;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode IntradayBar::GetClosePrice(Rational &price) const
{
	price = m_close;
	return STATUS_CODE_SUCCESS;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode IntradayBar::GetClosePrice(double &price) const
{
	return m_close.GetDouble(price);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode IntradayBar::SetClosePrice(const Rational &price)
{
	m_close = price;
	return STATUS_CODE_SUCCESS;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode IntradayBar::GetTotalVolume(uint32_t &volume) const
{
	if(Rational::DENOMINATOR_WHOLE == m_totalVolume.GetDenominator())
		volume = static_cast<uint32_t>(m_totalVolume.GetNumerator());
	else
	{
		double dSize;
		ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, m_totalVolume.GetDouble(dSize));
		volume = static_cast<uint32_t>(dSize);
	}
	return STATUS_CODE_SUCCESS;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode IntradayBar::SetTotalVolume(const uint32_t volume)
{
	return m_totalVolume.Set(volume, Rational::DENOMINATOR_WHOLE);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode IntradayBar::GetTotalVolume(Rational &volume) const
{
	volume = m_totalVolume;
	return STATUS_CODE_SUCCESS;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode IntradayBar::SetTotalVolume(const Rational &volume)
{
	m_totalVolume = volume;
	return STATUS_CODE_SUCCESS;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode IntradayBar::GetTotalPrice(Rational &totalPrice) const
{
	totalPrice = m_totalPrice;
	return STATUS_CODE_SUCCESS;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode IntradayBar::GetTotalPrice(double &totalPrice) const
{
	return m_totalPrice.GetDouble(totalPrice);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode IntradayBar::SetTotalPrice(Rational &totalPrice)
{
	m_totalPrice = totalPrice;
	return STATUS_CODE_SUCCESS;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode IntradayBar::GetTotalValue(Rational &totalValue) const
{
	totalValue = m_totalValue;
	return STATUS_CODE_SUCCESS;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode IntradayBar::GetTotalValue(double &totalValue) const
{
	return m_totalValue.GetDouble(totalValue);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode IntradayBar::SetTotalValue(Rational &totalValue)
{
	m_totalValue = totalValue;
	return STATUS_CODE_SUCCESS;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode IntradayBar::GetTickCount(uint32_t &count) const
{
	count = m_tickCount;
	return STATUS_CODE_SUCCESS;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode IntradayBar::SetTickCount(const uint32_t count)
{
	m_tickCount = count;
	return STATUS_CODE_SUCCESS;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode IntradayBar::SetFlag(const byte_t flag)
{
	m_flag = flag;
	return STATUS_CODE_SUCCESS;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode IntradayBar::GetFlag(byte_t &flag) const
{
	flag = m_flag;
	return STATUS_CODE_SUCCESS;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline bool IntradayBar::IsRegularHour() const
{
	return 0 == (m_flag & NONE_REGULAR_HOUR_TRADE);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline void IntradayBar::SetToMiniBar()
{
	m_totalPrice = 0;
	m_totalValue = 0;
	m_tickCount = 0;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline bool IntradayBar::HasTick() const
{
	return m_tickCount > 0;
}

// ---------------------------------------------------------------------------------------------------------------------------------

#if 0
inline int64_t IntradayBar::GetBasePrice() const
{
	// start at the most likely
	int64_t  basePrice = m_low;

	if (m_high < basePrice)
		basePrice = m_high;
	if (m_open < basePrice)
		basePrice = m_open;
	if (m_close < basePrice)
		basePrice = m_close;

	return basePrice;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline void IntradayBar::GetBasePrice(int64_t &basePrice, BaseType &baseType) const
{
	// start at the most likely
	basePrice = m_low;
	baseType = BASE_TYPE_LOW;

	if (m_high < basePrice)
	{
		basePrice = m_high;
		baseType = BASE_TYPE_HIGH;
	}
	if (m_open < basePrice)
	{
		basePrice = m_open;
		baseType = BASE_TYPE_OPEN;
	}
	if (m_close < basePrice)
	{
		basePrice = m_close;
		baseType = BASE_TYPE_CLOSE;
	}
}

#endif

} // namespace ContentPlatform

} // namespace Activ

#endif // !defined (ACTIV_CONTENT_GATEWAY_COMMON_INTRADAY_BAR_H)

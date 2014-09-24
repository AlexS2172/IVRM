/**
 *	@file	HistoryBar.h
 *
 *	@brief	History Bar Field Set.
 *
 *	$Log: $
 */

#if !defined (ACTIV_CONTENT_GATEWAY_COMMON_HISTORY_BAR_H)
#define ACTIV_CONTENT_GATEWAY_COMMON_HISTORY_BAR_H

#include "External.h"
#include "ActivContentPlatform/ActivContentGatewayCommon/FieldSet.h"
#include "ActivContentPlatform/ActivContentGatewayCommon/TimeSeriesTypes.h"

#include "ActivMiddleware/ActivDatabase/FieldSpecification.h"

#include "ActivMiddleware/ActivFieldTypes/Date.h"
#include "ActivMiddleware/ActivFieldTypes/Time.h"
#include "ActivMiddleware/ActivFieldTypes/Rational.h"

#include "ActivMiddleware/Misc/SerializableContainerHelper.h"

namespace Activ
{

namespace Database
{
	class Record;
}

namespace ContentPlatform
{

/**
 *	@brief	History Bar Field Set class.
 */
class HistoryBar : public FieldSet
{
public:
	/**
	 *	@brief	Constructor.
	 */
	ACTIV_CONTENT_GATEWAY_COMMON_API HistoryBar();

	/**
	 *	@brief	Destructor.
	 */
	ACTIV_CONTENT_GATEWAY_COMMON_API virtual ~HistoryBar();

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
	 *	@brief	Set the object value from a string.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *  @retval ......
	 */
	ACTIV_CONTENT_GATEWAY_COMMON_API virtual StatusCode FromStringSpecial(const std::string &inputStr);

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
	ACTIV_CONTENT_GATEWAY_COMMON_API bool Compare(const HistoryBar &rhs) const;

	/**
	 *	@brief	Equality operator.
	 *
	 *	@param	rhs the object to compare with.
	 *
	 *	@return	the result of the test.
	 */
	ACTIV_CONTENT_GATEWAY_COMMON_API bool operator==(const HistoryBar &rhs) const;

	/**
	 *	@brief	Inequality operator.
	 *
	 *	@param	rhs the object to compare with.
	 *
	 *	@return	the result of the test.
	 */
	ACTIV_CONTENT_GATEWAY_COMMON_API bool operator!=(const HistoryBar &rhs) const;

	/**
	 *	@brief	Less than operator (date only).
	 *
	 *	@param	rhs the object to compare with.
	 *
	 *	@return	the result of the test.
	 */
	ACTIV_CONTENT_GATEWAY_COMMON_API bool operator<(const HistoryBar &rhs) const;

	/**
	 *	@brief	Less than or equal operator (date only).
	 *
	 *	@param	rhs the object to compare with.
	 *
	 *	@return	the result of the test.
	 */
	ACTIV_CONTENT_GATEWAY_COMMON_API bool operator<=(const HistoryBar &rhs) const;

	/**
	 *	@brief	Greater than operator (date only).
	 *
	 *	@param	rhs the object to compare with.
	 *
	 *	@return	the result of the test.
	 */
	ACTIV_CONTENT_GATEWAY_COMMON_API bool operator>(const HistoryBar &rhs) const;

	/**
	 *	@brief	Greater than or equal operator (date only).
	 *
	 *	@param	rhs the object to compare with.
	 *
	 *	@return	the result of the test.
	 */
	ACTIV_CONTENT_GATEWAY_COMMON_API bool operator>=(const HistoryBar &rhs) const;

	/**
	 *	@brief	< operator
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
	ACTIV_CONTENT_GATEWAY_COMMON_API bool operator >(const DateTime &dateTime) const;
	ACTIV_CONTENT_GATEWAY_COMMON_API bool operator >=(const DateTime &dateTime) const;

	/**
	 *	@brief	Get the date.
	 *
	 *	@param	date the date of the bar.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_LENGTH
	 */
	ACTIV_CONTENT_GATEWAY_COMMON_API StatusCode GetDate(Date &date) const;

	/**
	 *	@brief	Set the date.
	 *
	 *	@param	date the date of the bar.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_LENGTH
	 */
	ACTIV_CONTENT_GATEWAY_COMMON_API StatusCode SetDate(const Date &date);

	/**
	 *	@brief	Get the date.
	 *
	 *	@param	date the date of the bar.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_LENGTH
	 */
	ACTIV_CONTENT_GATEWAY_COMMON_API StatusCode GetDateTime(DateTime &dateTime) const;
	
	/**
	 *	@brief	Get the open price.
	 *
	 *	@param	price the open price of the bar.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_LENGTH
	 */
	ACTIV_CONTENT_GATEWAY_COMMON_API StatusCode GetOpenPrice(Rational &price) const;

	/**
	 *	@brief	Get the open price.
	 *
	 *	@param	price the open price of the bar.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_LENGTH
	 */
	ACTIV_CONTENT_GATEWAY_COMMON_API StatusCode GetOpenPrice(double &price) const;

	/**
	 *	@brief	Set the open price.
	 *
	 *	@param	price the open price of the bar.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_LENGTH
	 */
	ACTIV_CONTENT_GATEWAY_COMMON_API StatusCode SetOpenPrice(const Rational &price);

	/**
	 *	@brief	Get the high price.
	 *
	 *	@param	price the high price of the bar.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_LENGTH
	 */
	ACTIV_CONTENT_GATEWAY_COMMON_API StatusCode GetHighPrice(Rational &price) const;

	/**
	 *	@brief	Get the high price.
	 *
	 *	@param	price the high price of the bar.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_LENGTH
	 */
	ACTIV_CONTENT_GATEWAY_COMMON_API StatusCode GetHighPrice(double &price) const;

	/**
	 *	@brief	Set the high price.
	 *
	 *	@param	price the high price of the bar.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_LENGTH
	 */
	ACTIV_CONTENT_GATEWAY_COMMON_API StatusCode SetHighPrice(const Rational &price);

	/**
	 *	@brief	Get the low price.
	 *
	 *	@param	price the low price of the bar.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_LENGTH
	 */
	ACTIV_CONTENT_GATEWAY_COMMON_API StatusCode GetLowPrice(Rational &price) const;

	/**
	 *	@brief	Get the low price.
	 *
	 *	@param	price the low price of the bar.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_LENGTH
	 */
	ACTIV_CONTENT_GATEWAY_COMMON_API StatusCode GetLowPrice(double &price) const;

	/**
	 *	@brief	Set the low price.
	 *
	 *	@param	price the low price of the bar.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_LENGTH
	 */
	ACTIV_CONTENT_GATEWAY_COMMON_API StatusCode SetLowPrice(const Rational &price);

	/**
	 *	@brief	Get the close price.
	 *
	 *	@param	price the close price of the bar.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_LENGTH
	 */
	ACTIV_CONTENT_GATEWAY_COMMON_API StatusCode GetClosePrice(Rational &price) const;

	/**
	 *	@brief	Get the close price.
	 *
	 *	@param	price the close price of the bar.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_LENGTH
	 */
	ACTIV_CONTENT_GATEWAY_COMMON_API StatusCode GetClosePrice(double &price) const;

	/**
	 *	@brief	Set the close price.
	 *
	 *	@param	price the close price of the bar.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_LENGTH
	 */
	ACTIV_CONTENT_GATEWAY_COMMON_API StatusCode SetClosePrice(const Rational &price);

	/**
	 *	@brief	Get the bar's total volume.
	 *
	 *	@param	volume the total volume of the bar.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_LENGTH
	 */
	ACTIV_CONTENT_GATEWAY_COMMON_API StatusCode GetTotalVolume(uint64_t &volume) const;

	/**
	 *	@brief	Set the bar's total volume.
	 *
	 *	@param	volume the volume of the bar.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_LENGTH
	 */
	ACTIV_CONTENT_GATEWAY_COMMON_API StatusCode SetTotalVolume(const uint64_t volume);

	/**
	 *	@brief	Get the bar's total volume.
	 *
	 *	@param	volume the total volume of the bar.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_LENGTH
	 */
	ACTIV_CONTENT_GATEWAY_COMMON_API StatusCode GetTotalVolume(Rational &volume) const;

	/**
	 *	@brief	Set the bar's total volume.
	 *
	 *	@param	volume the volume of the bar.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_LENGTH
	 */
	ACTIV_CONTENT_GATEWAY_COMMON_API StatusCode SetTotalVolume(const Rational& volume);

	/**
	 *	@brief	Get the bar's total price.
	 *
	 *	@param	totalPrice the total price of the bar.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_LENGTH
	 */
	ACTIV_CONTENT_GATEWAY_COMMON_API StatusCode GetTotalPrice(Rational &totalPrice) const;

	/**
	 *	@brief	Get the bar's total price.
	 *
	 *	@param	totalPrice the total price of the bar.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_LENGTH
	 */
	ACTIV_CONTENT_GATEWAY_COMMON_API StatusCode GetTotalPrice(double &totalPrice) const;

	/**
	 *	@brief	Set the bar's total price.
	 *
	 *	@param	totalPrice the total price of the bar.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_LENGTH
	 */
	ACTIV_CONTENT_GATEWAY_COMMON_API StatusCode SetTotalPrice(Rational &totalPrice);

	/**
	 *	@brief	Get the bar's total value.
	 *
	 *	@param	totalValue the total value of the bar.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_LENGTH
	 */
	ACTIV_CONTENT_GATEWAY_COMMON_API StatusCode GetTotalValue(Rational &totalValue) const;

	/**
	 *	@brief	Get the bar's total value.
	 *
	 *	@param	totalValue the total value of the bar.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_LENGTH
	 */
	ACTIV_CONTENT_GATEWAY_COMMON_API StatusCode GetTotalValue(double &totalValue) const;

	/**
	 *	@brief	Set the bar's total value.
	 *
	 *	@param	totalValue the total value of the bar.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_LENGTH
	 */
	ACTIV_CONTENT_GATEWAY_COMMON_API StatusCode SetTotalValue(Rational &totalValue);

	/**
	 *	@brief	Get the bar's total volume.
	 *
	 *	@param	openInterest the open interest of the bar.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_LENGTH
	 */
	ACTIV_CONTENT_GATEWAY_COMMON_API StatusCode GetOpenInterest(uint32_t &openInterest) const;

	/**
	 *	@brief	Set the bar's total volume.
	 *
	 *	@param	openInterest the open interest of the bar.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_LENGTH
	 */
	ACTIV_CONTENT_GATEWAY_COMMON_API StatusCode SetOpenInterest(const uint32_t openInterest);

	/**
	 *	@brief	Get the bar's tick count.
	 *
	 *	@param	count the tick count of the bar.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_LENGTH
	 */
//	ACTIV_CONTENT_GATEWAY_COMMON_API StatusCode GetTickCount(unsigned &count) const;

	/**
	 *	@brief	Set the bar's tick count.
	 *
	 *	@param	count the tick count of the bar.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_LENGTH
	 */
//	ACTIV_CONTENT_GATEWAY_COMMON_API StatusCode SetTickCount(const unsigned count);

	/**
	 *	@brief	Build/update a bar with this bar.
	 *
	 *	@param	period target bar's period.
	 *	@param	bar daily bar to be added.
	 *
	 *	@retval	UPDATE_STATUS_OK
	 *	@retval	UPDATE_STATUS_FLUSH
	 *	@retval	UPDATE_STATUS_IGNORE
	 */
	ACTIV_CONTENT_GATEWAY_COMMON_API FieldSet::UpdateFieldSetStatus Update(const SeriesType period, const HistoryBar &dailyBar);

	/**
	 *	@brief	Set the extra value fields to 0
	 */
	ACTIV_CONTENT_GATEWAY_COMMON_API void SetToMiniBar();

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

public:
	ACTIV_CONTENT_GATEWAY_COMMON_API static StatusCode BuildBarFromRecord(const Database::Record& record, HistoryBar& dailyBar);
	ACTIV_CONTENT_GATEWAY_COMMON_API static StatusCode BuildClosedSessionBarFromRecord(const Database::Record& record, HistoryBar& dailyBar);

private:
	static const byte_t  HISTORY_BAR_TOTAL_PRICE	= 0x004;
	static const byte_t  HISTORY_BAR_TOTAL_VALUE	= 0x008;
	static const byte_t  HISTORY_BAR_TOTAL_VOLUME	= 0x010;
	static const byte_t  HISTORY_BAR_OPEN_INTEREST	= 0x020;
	static const byte_t  HISTORY_BAR_TICK_COUNT		= 0x040;

	enum BaseType
	{
		BASE_TYPE_LIST(ACTIV_DECLARE_ELEMENT_1_WITH_COMMA)
		NUM_BASE_TYPES,
	};
#if 0
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
	Date                   m_date;				///< date.
	Rational               m_open;				///< open price.
	Rational               m_high;				///< high price.
	Rational               m_low;				///< low price.
	Rational               m_close;				///< closing price.
	Rational               m_totalPrice;		///< total price.
	Rational               m_totalValue;		///< total value.
	Rational               m_totalVolume;		///< total volume.
	uint32_t               m_openInterest;		///< open interest.
//	uint32_t               m_tickCount;			///< tick count.

};

typedef FieldSetList<HistoryBar, uint16_t> HistoryBarList;

// ---------------------------------------------------------------------------------------------------------------------------------

inline bool HistoryBar::operator==(const HistoryBar &rhs) const
{
	return m_date == rhs.m_date;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline bool HistoryBar::operator!=(const HistoryBar &rhs) const
{
	return m_date != rhs.m_date;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline bool HistoryBar::operator<(const HistoryBar &rhs) const
{
	return m_date < rhs.m_date;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline bool HistoryBar::operator<=(const HistoryBar &rhs) const
{
	return m_date <= rhs.m_date;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline bool HistoryBar::operator>(const HistoryBar &rhs) const
{
	return m_date > rhs.m_date;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline bool HistoryBar::operator>=(const HistoryBar &rhs) const
{
	return m_date >= rhs.m_date;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline bool HistoryBar::operator<(const DateTime &dateTime) const
{
	return m_date < dateTime.GetDate();
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline bool HistoryBar::operator<=(const DateTime &dateTime) const
{
	return m_date <= dateTime.GetDate();
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline bool HistoryBar::operator ==(const DateTime &dateTime) const
{
	return m_date == dateTime.GetDate();
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline bool HistoryBar::operator >(const DateTime &dateTime) const
{
	return m_date > dateTime.GetDate();
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline bool HistoryBar::operator >=(const DateTime &dateTime) const
{
	return m_date >= dateTime.GetDate();
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode HistoryBar::GetDate(Date &date) const
{
	date = m_date;
	return STATUS_CODE_SUCCESS;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode HistoryBar::GetDateTime(DateTime &dateTime) const
{
	dateTime.Set(m_date, Time(23,59,59));
	return STATUS_CODE_SUCCESS;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode HistoryBar::SetDate(const Date &date)
{
	m_date = date;
	return STATUS_CODE_SUCCESS;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode HistoryBar::GetOpenPrice(Rational &price) const
{
	price = m_open;
	return STATUS_CODE_SUCCESS;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode HistoryBar::GetOpenPrice(double &price) const
{
	return m_open.GetDouble(price);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode HistoryBar::SetOpenPrice(const Rational &price)
{
	m_open = price;
	return STATUS_CODE_SUCCESS;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode HistoryBar::GetHighPrice(Rational &price) const
{
	price = m_high;
	return STATUS_CODE_SUCCESS;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode HistoryBar::GetHighPrice(double &price) const
{
	return m_high.GetDouble(price);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode HistoryBar::SetHighPrice(const Rational &price)
{
	m_high = price;
	return STATUS_CODE_SUCCESS;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode HistoryBar::GetLowPrice(Rational &price) const
{
	price = m_low;
	return STATUS_CODE_SUCCESS;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode HistoryBar::GetLowPrice(double &price) const
{
	return m_low.GetDouble(price);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode HistoryBar::SetLowPrice(const Rational &price)
{
	m_low = price;
	return STATUS_CODE_SUCCESS;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode HistoryBar::GetClosePrice(Rational &price) const
{
	price = m_close;
	return STATUS_CODE_SUCCESS;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode HistoryBar::GetClosePrice(double &price) const
{
	return m_close.GetDouble(price);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode HistoryBar::SetClosePrice(const Rational &price)
{
	m_close = price;
	return STATUS_CODE_SUCCESS;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode HistoryBar::GetTotalVolume(uint64_t &volume) const
{
	if(Rational::DENOMINATOR_WHOLE == m_totalVolume.GetDenominator())
		volume = m_totalVolume.GetNumerator();
	else
	{
		double dSize;
		ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, m_totalVolume.GetDouble(dSize));
		volume = static_cast<uint64_t>(dSize);
	}
	return STATUS_CODE_SUCCESS;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode HistoryBar::SetTotalVolume(const uint64_t volume)
{
	return m_totalVolume.Set(volume, Rational::DENOMINATOR_WHOLE);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode HistoryBar::GetTotalVolume(Rational &volume) const
{
	volume = m_totalVolume;
	return STATUS_CODE_SUCCESS;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode HistoryBar::SetTotalVolume(const Rational &volume)
{
	m_totalVolume = volume;
	return STATUS_CODE_SUCCESS;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode HistoryBar::GetTotalPrice(Rational &totalPrice) const
{
	totalPrice = m_totalPrice;
	return STATUS_CODE_SUCCESS;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode HistoryBar::GetTotalPrice(double &totalPrice) const
{
	return m_totalPrice.GetDouble(totalPrice);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode HistoryBar::SetTotalPrice(Rational &totalPrice)
{
	m_totalPrice = totalPrice;
	return STATUS_CODE_SUCCESS;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode HistoryBar::GetTotalValue(Rational &totalValue) const
{
	totalValue = m_totalValue;
	return STATUS_CODE_SUCCESS;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode HistoryBar::GetTotalValue(double &totalValue) const
{
	return m_totalValue.GetDouble(totalValue);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode HistoryBar::SetTotalValue(Rational &totalValue)
{
	m_totalValue = totalValue;
	return STATUS_CODE_SUCCESS;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode HistoryBar::GetOpenInterest(uint32_t &openInterest) const
{
	openInterest = m_openInterest;
	return STATUS_CODE_SUCCESS;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode HistoryBar::SetOpenInterest(const uint32_t openInterest)
{
	m_openInterest = openInterest;
	return STATUS_CODE_SUCCESS;
}

// ---------------------------------------------------------------------------------------------------------------------------------
/*
inline StatusCode HistoryBar::GetTickCount(unsigned &count) const
{
	count = m_tickCount;
	return STATUS_CODE_SUCCESS;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode HistoryBar::SetTickCount(const unsigned count)
{
	m_tickCount = count;
	return STATUS_CODE_SUCCESS;
}
*/
// ---------------------------------------------------------------------------------------------------------------------------------

inline void HistoryBar::SetToMiniBar()
{
	m_totalPrice = 0;
	m_totalValue = 0;
//	m_tickCount = 0;
}

// ---------------------------------------------------------------------------------------------------------------------------------
#if 0
inline int64_t HistoryBar::GetBasePrice() const
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

inline void HistoryBar::GetBasePrice(int64_t &basePrice, BaseType &baseType) const
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

#endif // !defined (ACTIV_CONTENT_GATEWAY_COMMON_HISTORY_BAR_H)

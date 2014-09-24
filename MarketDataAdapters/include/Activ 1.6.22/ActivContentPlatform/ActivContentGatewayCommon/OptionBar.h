/**
 *	@file	OptionBar.h
 *
 *	@brief	Option Bar Field Set.
 *
 *	$Log: $
 */

#if !defined (ACTIV_CONTENT_GATEWAY_COMMON_OPTION_BAR_H)
#define ACTIV_CONTENT_GATEWAY_COMMON_OPTION_BAR_H

#include "ActivContentPlatform/ActivContentGatewayCommon/External.h"
#include "ActivContentPlatform/ActivContentGatewayCommon/FieldSet.h"

#include "ActivMiddleware/ActivFieldTypes/Date.h"
#include "ActivMiddleware/ActivFieldTypes/Time.h"
#include "ActivMiddleware/ActivFieldTypes/Rational.h"

#include "ActivMiddleware/Misc/SerializableVector.h"

namespace Activ
{

namespace ContentPlatform
{

/**
 *	@brief	Option Bar Field Set class.
 */
class OptionBar : public FieldSet
{
public:
	/**
	 *	@brief	Constructor.
	 */
	OptionBar();

	/**
	 *	@brief	Destructor.
	 */
	virtual ~OptionBar();

	/**
	 *	@brief	Reset all values in the field set.
	 */
	virtual void Reset();

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
	virtual StatusCode GetField(const Feed::FieldId fieldId, IFieldType &field) const;

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
	virtual StatusCode SetField(const Feed::FieldId fieldId, const IFieldType &field);

	/**
	 *	@brief	Get the field type.
	 *
	 *	@return	the field type.
	 */
	virtual FieldSetType GetType() const;

	/**
	 *	@brief	Convert the object to a string.
	 *
	 *	@return	the object as a string.
	 */
	virtual std::string ToString() const;

	/**
	 *	@brief	Serialize the object into the supplied message builder.
	 *
	 *	@param	messageBuilder the message builder to store a serialized version of the object in.
	 *	@param	maxSerializedBodyLength the maximum serialized body length.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
	virtual StatusCode Serialize(MessageBuilder &messageBuilder) const;

	/**
	 *	@brief	Deserialize the object from the supplied message validater.
	 *
	 *	@param	messageValidater the message validater from which the object will be extracted.
	 *	@param	maxSerializedBodyLength the maximum serialized body length.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
	virtual StatusCode Deserialize(MessageValidater &messageValidater);

	/**
	 *	@brief Get the serialized length of the object.
	 *
	 *	@return	the serialized length of the object.
	 */
	virtual size_t GetSerializedLength() const;

	/**
	 *	@brief	Get the maximum serialized length.
	 *
	 *	@return	the maximum serialized length.
	 */
	static size_t GetMaxSerializedLength();

	/**
	 *	@brief	Full compare.
	 *
	 *	@param	rhs the object to compare with.
	 *
	 *	@return	the result of the test.
	 */
	bool Compare(const OptionBar &rhs) const;

	/**
	 *	@brief	Equality operator.
	 *
	 *	@param	rhs the object to compare with.
	 *
	 *	@return	the result of the test.
	 */
	bool operator==(const OptionBar &rhs) const;

	/**
	 *	@brief	Inequality operator.
	 *
	 *	@param	rhs the object to compare with.
	 *
	 *	@return	the result of the test.
	 */
	bool operator!=(const OptionBar &rhs) const;

	/**
	 *	@brief	Less than operator (date only).
	 *
	 *	@param	rhs the object to compare with.
	 *
	 *	@return	the result of the test.
	 */
	bool operator<(const OptionBar &rhs) const;

	/**
	 *	@brief	Less than or equal operator (date only).
	 *
	 *	@param	rhs the object to compare with.
	 *
	 *	@return	the result of the test.
	 */
	bool operator<=(const OptionBar &rhs) const;

	/**
	 *	@brief	Greater than operator (date only).
	 *
	 *	@param	rhs the object to compare with.
	 *
	 *	@return	the result of the test.
	 */
	bool operator>(const OptionBar &rhs) const;

	/**
	 *	@brief	Greater than or equal operator (date only).
	 *
	 *	@param	rhs the object to compare with.
	 *
	 *	@return	the result of the test.
	 */
	bool operator>=(const OptionBar &rhs) const;

	/**
	 *	@brief	Reduce all prices.
	 *
	 *	@param	minDenominator minimum denominator to reduce to.
	 */
	void Reduce(const Rational::Denominator minDenominator = Rational::DENOMINATOR_2DP);

	/**
	 *	@brief	Get the denominator.
	 *
	 *	@return	the denominator.
	 */
	Rational::Denominator GetDenominator() const;

	/**
	 *	@brief	Set the denominator.
	 *
	 *	@param	denominator the denominator of various prices in the bar.
	 *
	 *	@param	date the date of the tick.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 */
	StatusCode SetDenominator(const Rational::Denominator denominator);

	/**
	 *	@brief	Get the date.
	 *
	 *	@param	date the date of the bar.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_LENGTH
	 */
	StatusCode GetDate(Date &date) const;

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
	 *	@brief	Get the underlying price.
	 *
	 *	@param	price the underlying price of the bar.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_LENGTH
	 */
	StatusCode GetUnderlyingPrice(Rational &price) const;

	/**
	 *	@brief	Get the underlying price.
	 *
	 *	@param	price the underlying price of the bar.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_LENGTH
	 */
	StatusCode GetUnderlyingPrice(double &price) const;

	/**
	 *	@brief	Set the underlying price.
	 *
	 *	@param	price the underlying price of the bar.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_LENGTH
	 */
	StatusCode SetUnderlyingPrice(const Rational &price);

	/**
	 *	@brief	Get the bid price.
	 *
	 *	@param	price the bid price of the bar.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_LENGTH
	 */
	StatusCode GetBidPrice(Rational &price) const;

	/**
	 *	@brief	Get the bid price.
	 *
	 *	@param	price the bid price of the bar.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_LENGTH
	 */
	StatusCode GetBidPrice(double &price) const;

	/**
	 *	@brief	Set the bid price.
	 *
	 *	@param	price the bid price of the bar.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_LENGTH
	 */
	StatusCode SetBidPrice(const Rational &price);

	/**
	 *	@brief	Get the ask price.
	 *
	 *	@param	price the ask price of the bar.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_LENGTH
	 */
	StatusCode GetAskPrice(Rational &price) const;

	/**
	 *	@brief	Get the ask price.
	 *
	 *	@param	price the ask price of the bar.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_LENGTH
	 */
	StatusCode GetAskPrice(double &price) const;

	/**
	 *	@brief	Set the ask price.
	 *
	 *	@param	price the ask price of the bar.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_LENGTH
	 */
	StatusCode SetAskPrice(const Rational &price);

	/**
	 *	@brief	Get the bar's total volume.
	 *
	 *	@param	volume the total volume of the bar.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_LENGTH
	 */
	StatusCode GetTotalVolume(unsigned &volume) const;

	/**
	 *	@brief	Set the bar's total volume.
	 *
	 *	@param	volume the volume of the bar.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_LENGTH
	 */
	StatusCode SetTotalVolume(const unsigned volume);

	/**
	 *	@brief	Get the bar's total volume.
	 *
	 *	@param	openInterest the open interest of the bar.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_LENGTH
	 */
	StatusCode GetOpenInterest(unsigned &openInterest) const;

	/**
	 *	@brief	Set the bar's total volume.
	 *
	 *	@param	openInterest the open interest of the bar.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_LENGTH
	 */
	StatusCode SetOpenInterest(const unsigned openInterest);

	/**
	 *	@brief	Get the expiration date.
	 *
	 *	@param	date the expiration date of the option.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_LENGTH
	 */
	StatusCode GetExpirationDate(Date &date) const;

	/**
	 *	@brief	Set the expiration date.
	 *
	 *	@param	date the expiration date of the option.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_LENGTH
	 */
	StatusCode SetExpirationDate(const Date &date);
	
	/**
	 *	@brief	Get the strike price.
	 *
	 *	@param	price the strike price of the bar.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_LENGTH
	 */
	StatusCode GetStrikePrice(Rational &price) const;

	/**
	 *	@brief	Get the strike price.
	 *
	 *	@param	price the strike price of the bar.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_LENGTH
	 */
	StatusCode GetStrikePrice(double &price) const;

	/**
	 *	@brief	Set the strike price.
	 *
	 *	@param	price the strike price of the bar.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_LENGTH
	 */
	StatusCode SetStrikePrice(const Rational &price);

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
	//virtual StatusCode Compress(const FieldSet &previous, BitBuilder &builder) const;

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
	//virtual StatusCode Decompress(const FieldSet &previous, BitValidater &validater);

private:
	static const byte_t  OPTION_BAR_TOTAL_VOLUME	= 0x010;
	static const byte_t  OPTION_BAR_OPEN_INTEREST	= 0x020;

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
	
	Date                   m_date;				///< date.
	Date                   m_expirationDate;	///< expiration date.
	int64_t                m_bid;				///< bid price.
	int64_t                m_ask;				///< ask price.
	int64_t                m_underlyingPrice;	///< price of the underlying.
	Rational::Denominator  m_denominator;		///< price denominator.
	Rational               m_strikePrice;		///< strike price.
	uint32_t               m_totalVolume;		///< total volume.
	uint32_t               m_openInterest;		///< open interest.
};

typedef Activ::SerializableVector<OptionBar, uint16_t> OptionBarList;

// ---------------------------------------------------------------------------------------------------------------------------------

inline bool OptionBar::operator==(const OptionBar &rhs) const
{
	return Compare(rhs);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline bool OptionBar::operator!=(const OptionBar &rhs) const
{
	return !Compare(rhs);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline bool OptionBar::operator<(const OptionBar &rhs) const
{
	if (m_date < rhs.m_date)
		return true;

	return false;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline bool OptionBar::operator<=(const OptionBar &rhs) const
{
	if (m_date <= rhs.m_date)
		return true;

	return false;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline bool OptionBar::operator>(const OptionBar &rhs) const
{
	if (m_date > rhs.m_date)
		return true;

	return false;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline bool OptionBar::operator>=(const OptionBar &rhs) const
{
	if (m_date >= rhs.m_date)
		return true;

	return false;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline Rational::Denominator OptionBar::GetDenominator() const
{
	return m_denominator;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode OptionBar::SetDenominator(const Rational::Denominator denominator)
{
	// need to scale the prices up here
	if (m_denominator != denominator)
		ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, ScalePrices(denominator));

	m_denominator = denominator;
	return STATUS_CODE_SUCCESS;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode OptionBar::GetDate(Date &date) const
{
	date = m_date;
	return STATUS_CODE_SUCCESS;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode OptionBar::SetDate(const Date &date)
{
	m_date = date;
	return STATUS_CODE_SUCCESS;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode OptionBar::GetUnderlyingPrice(Rational &price) const
{
	return price.Set(m_underlyingPrice, m_denominator);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode OptionBar::GetUnderlyingPrice(double &price) const
{
	Rational::DenominatorType  *pDenominatorType;
	
	ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, Rational::GetDenominatorType(m_denominator, pDenominatorType));

	if (pDenominatorType->IsMultiplier())
		price = (double)m_underlyingPrice*pDenominatorType->GetMultiplier();
	else if (pDenominatorType->IsDivisor())
		price = (double)m_underlyingPrice/pDenominatorType->GetDivisor();
	else
		return STATUS_CODE_UNDEFINED_FIELD;

	return STATUS_CODE_SUCCESS;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode OptionBar::SetUnderlyingPrice(const Rational &price)
{
	Rational  temp = price;
	
	// scale the price
	if (m_denominator != price.GetDenominator())
		ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, ScalePrice(temp));

	m_underlyingPrice = temp.GetNumerator();
	return STATUS_CODE_SUCCESS;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode OptionBar::GetBidPrice(Rational &price) const
{
	return price.Set(m_bid, m_denominator);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode OptionBar::GetBidPrice(double &price) const
{
	Rational::DenominatorType  *pDenominatorType;
	
	ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, Rational::GetDenominatorType(m_denominator, pDenominatorType));

	if (pDenominatorType->IsMultiplier())
		price = (double)m_bid*pDenominatorType->GetMultiplier();
	else if (pDenominatorType->IsDivisor())
		price = (double)m_bid/pDenominatorType->GetDivisor();
	else
		return STATUS_CODE_UNDEFINED_FIELD;

	return STATUS_CODE_SUCCESS;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode OptionBar::SetBidPrice(const Rational &price)
{
	Rational  temp = price;
	
	// scale the price
	if (m_denominator != price.GetDenominator())
		ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, ScalePrice(temp));

	m_bid = temp.GetNumerator();
	return STATUS_CODE_SUCCESS;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode OptionBar::GetAskPrice(Rational &price) const
{
	return price.Set(m_ask, m_denominator);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode OptionBar::GetAskPrice(double &price) const
{
	Rational::DenominatorType  *pDenominatorType;
	
	ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, Rational::GetDenominatorType(m_denominator, pDenominatorType));

	if (pDenominatorType->IsMultiplier())
		price = (double)m_ask*pDenominatorType->GetMultiplier();
	else if (pDenominatorType->IsDivisor())
		price = (double)m_ask/pDenominatorType->GetDivisor();
	else
		return STATUS_CODE_UNDEFINED_FIELD;

	return STATUS_CODE_SUCCESS;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode OptionBar::SetAskPrice(const Rational &price)
{
	Rational  temp = price;
	
	// scale the price
	if (m_denominator != price.GetDenominator())
		ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, ScalePrice(temp));

	m_ask = temp.GetNumerator();
	return STATUS_CODE_SUCCESS;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode OptionBar::GetTotalVolume(unsigned &volume) const
{
	volume = m_totalVolume;
	return STATUS_CODE_SUCCESS;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode OptionBar::SetTotalVolume(const unsigned volume)
{
	m_totalVolume = volume;
	return STATUS_CODE_SUCCESS;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode OptionBar::GetOpenInterest(unsigned &openInterest) const
{
	openInterest = m_openInterest;
	return STATUS_CODE_SUCCESS;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode OptionBar::SetOpenInterest(const unsigned openInterest)
{
	m_openInterest = openInterest;
	return STATUS_CODE_SUCCESS;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode OptionBar::GetExpirationDate(Date &date) const
{
	date = m_date;
	return STATUS_CODE_SUCCESS;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode OptionBar::SetExpirationDate(const Date &date)
{
	m_date = date;
	return STATUS_CODE_SUCCESS;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode OptionBar::GetStrikePrice(Rational &price) const
{
	price = m_strikePrice;
	return STATUS_CODE_SUCCESS;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode OptionBar::GetStrikePrice(double &price) const
{
	Rational::DenominatorType  *pDenominatorType;
	
	ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, Rational::GetDenominatorType(m_strikePrice.GetDenominator(), pDenominatorType));

	if (pDenominatorType->IsMultiplier())
		price = (double)m_strikePrice.GetNumerator()*pDenominatorType->GetMultiplier();
	else if (pDenominatorType->IsDivisor())
		price = (double)m_strikePrice.GetNumerator()/pDenominatorType->GetDivisor();
	else
		return STATUS_CODE_UNDEFINED_FIELD;

	return STATUS_CODE_SUCCESS;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode OptionBar::SetStrikePrice(const Rational &price)
{
	m_strikePrice = price;
	return STATUS_CODE_SUCCESS;
}

} // namespace ContentPlatform

} // namespace Activ

#endif // !defined (ACTIV_CONTENT_GATEWAY_COMMON_OPTION_BAR_H)

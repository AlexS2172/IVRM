/**
 *	@file	TRational.h
 *
 *	@brief	Header file for the trended rational class.
 *
 *	$Log: $
 */

#if !defined (ACTIV_TRATIONAL_H)
#define ACTIV_TRATIONAL_H

#include "ActivMiddleware/ActivFieldTypes/External.h"
#include "ActivMiddleware/ActivFieldTypes/IFieldType.h"
#include "ActivMiddleware/ActivFieldTypes/Rational.h"

namespace Activ
{

/**
*	@brief	The tick list.
*/
#define ACTIV_TICK_LIST(d)			\
	d(TICK_UNCHANGED,		"=")	\
	d(TICK_UP,				"+")	\
	d(TICK_DOWN,			"-")	\
	d(TICK_UNDEFINED,		" ")

/**
*	@brief	The trend list.
*/
#define ACTIV_TREND_LIST(d)			\
	d(TREND_UNCHANGED,		"=")	\
	d(TREND_UP,				"+")	\
	d(TREND_DOWN,			"-")	\
	d(TREND_UNDEFINED,		" ")

class MessageBuilder;
class MessageValidater;

/**
 *	@brief	Trended rational class.
 */
class ACTIV_FIELD_TYPES_API TRational : public IFieldType
{
public:
	static const FieldType FIELD_TYPE = FIELD_TYPE_TRATIONAL; ///< The field type.

	using IFieldType::FromString;

	/**
	*	@brief	Tick enumeration.
	*/
	enum Tick
	{
		ACTIV_TICK_LIST(ACTIV_DECLARE_ELEMENT_1_WITH_COMMA)
		NUMBER_OF_TICKS,
	};

	/**
	*	@brief	Trend enumeration.
	*/
	enum Trend
	{
		ACTIV_TREND_LIST(ACTIV_DECLARE_ELEMENT_1_WITH_COMMA)
		NUMBER_OF_TRENDS,
	};

	/**
	 *	@brief	Default constructor.
	 */
	TRational();

	/**
	 *	@brief	Constructor.
	 *
	 *	@param	numerator the numerator portion of the value.
	 *	@param	denominator the denominator portion of the value.
	 *	@param	tick the tick.
	 *	@param	trendOnDay the trend on the day.
	 *	@param	trendOnPrevious the trend on the previous.
	 *	@param	shouldCompress indicates whether the numerator and denominator should be compressed.
	 *
	 *	@throw	ActivException
	 */
	TRational(const int64_t numerator, const Rational::Denominator denominator = Rational::DENOMINATOR_WHOLE, const Tick tick = TICK_UNDEFINED, const Trend trendOnDay = TREND_UNDEFINED, const Trend trendOnPrevious = TREND_UNDEFINED, bool shouldCompress = false);

	/**
	 *	@brief	Constructor.
	 *
	 *	@param	value the value.
	 *	@param	tick the tick.
	 *	@param	trendOnDay the trend on the day.
	 *	@param	trendOnPrevious the trend on the previous.
	 *
	 *	@throw	ActivException
	 */
	TRational(const Rational &value, const Tick tick = TICK_UNDEFINED, const Trend trendOnDay = TREND_UNDEFINED, const Trend trendOnPrevious = TREND_UNDEFINED);

	/**
	 *	@brief	Constructor.
	 *
	 *	@param	numerator the numerator portion of the value.
	 *	@param	denominator the denominator portion of the value.
	 *	@param	pPreviousValue the previous value.
	 *	@param	pPreviousDayValue the previous day's value.
	 *	@param	shouldCompress indicates whether the numerator and denominator should be compressed.
	 *
	 *	@throw	ActivException
	 */
	TRational(const int64_t numerator, const Rational::Denominator denominator, const TRational *pPreviousValue, const Rational *pPreviousDayValue, bool shouldCompress = false);

	/**
	 *	@brief	Constructor.
	 *
	 *	@param	value the value.
	 *	@param	pPreviousValue the previous value.
	 *	@param	pPreviousDayValue the previous day's value.
	 *
	 *	@throw	ActivException
	 */
	TRational(const Rational &value, const TRational *pPreviousValue, const Rational *pPreviousDayValue);

	/**
	 *	@brief	Constructor.
	 *
	 *	@param	numerator the numerator portion of the value.
	 *	@param	denominator the denominator portion of the value.
	 *	@param	pPreviousTick the previous tick.
	 *	@param	pPreviousValue the previous value.
	 *	@param	pPreviousDayValue the previous day's value.
	 *	@param	shouldCompress indicates whether the numerator and denominator should be compressed.
	 *
	 *	@throw	ActivException
	 */
	TRational(const int64_t numerator, const Rational::Denominator denominator, const TRational *pPreviousTick, const Rational *pPreviousValue, const Rational *pPreviousDayValue, bool shouldCompress = false);

	/**
	 *	@brief	Constructor.
	 *
	 *	@param	value the value.
	 *	@param	pPreviousTick the previous tick.
	 *	@param	pPreviousValue the previous value.
	 *	@param	pPreviousDayValue the previous day's value.
	 *
	 *	@throw	ActivException
	 */
	TRational(const Rational &value, const TRational *pPreviousTick, const Rational *pPreviousValue, const Rational *pPreviousDayValue);

	/**
	 *	@brief	Destructor.
	 */
	virtual ~TRational();

	/**
	 *	@brief	Rational conversion operator.
	 *
	 *	@throw	ActivException
	 */
	operator const Rational&() const;

	/**
	 *	@brief	Set a new value.
	 *
	 *	@param	numerator the numerator portion of the value.
	 *	@param	denominator the denominator portion of the value.
	 *	@param	tick the tick.
	 *	@param	trendOnDay the trend on the day.
	 *	@param	trendOnPrevious the trend on the previous.
	 *	@param	shouldCompress indicates whether the numerator and denominator should be compressed.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 */
	StatusCode Set(const int64_t numerator, const Rational::Denominator denominator, const Tick tick = TICK_UNDEFINED, const Trend trendOnDay = TREND_UNDEFINED, const Trend trendOnPrevious = TREND_UNDEFINED, bool shouldCompress = false);

	/**
	 *	@brief	Set a new value.
	 *
	 *	@param	value the value.
	 *	@param	tick the tick.
	 *	@param	trendOnDay the trend on the day.
	 *	@param	trendOnPrevious the trend on the previous.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_NOT_INITIALIZED
	 */
	StatusCode Set(const Rational &value, const Tick tick = TICK_UNDEFINED, const Trend trendOnDay = TREND_UNDEFINED, const Trend trendOnPrevious = TREND_UNDEFINED);

	/**
	 *	@brief	Set a new value.
	 *
	 *	@param	numerator the numerator portion of the value.
	 *	@param	denominator the denominator portion of the value.
	 *	@param	pPreviousValue the previous value.
	 *	@param	pPreviousDayValue the previous day's value.
	 *	@param	shouldCompress indicates whether the numerator and denominator should be compressed.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_NOT_INITIALIZED
	 */
	StatusCode Set(const int64_t numerator, const Rational::Denominator denominator, const TRational *pPreviousValue, const Rational *pPreviousDayValue, bool shouldCompress = false);

	/**
	 *	@brief	Set a new value.
	 *
	 *	@param	value the value.
	 *	@param	pPreviousValue the previous value.
	 *	@param	pPreviousDayValue the previous day's value.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_NOT_INITIALIZED
	 */
	StatusCode Set(const Rational &value, const TRational *pPreviousValue, const Rational *pPreviousDayValue);

	/**
	 *	@brief	Set a new value.
	 *
	 *	@param	numerator the numerator portion of the value.
	 *	@param	denominator the denominator portion of the value.
	 *	@param	pPreviousTick the previous tick.
	 *	@param	pPreviousValue the previous value.
	 *	@param	pPreviousDayValue the previous day's value.
	 *	@param	shouldCompress indicates whether the numerator and denominator should be compressed.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_NOT_INITIALIZED
	 */
	StatusCode Set(const int64_t numerator, const Rational::Denominator denominator, const TRational *pPreviousTick, const Rational *pPreviousValue, const Rational *pPreviousDayValue, bool shouldCompress = false);

	/**
	 *	@brief	Set a new value.
	 *
	 *	@param	value the value.
	 *	@param	pPreviousTick the previous tick.
	 *	@param	pPreviousValue the previous value.
	 *	@param	pPreviousDayValue the previous day's value.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_NOT_INITIALIZED
	 */
	StatusCode Set(const Rational &value, const TRational *pPreviousTick, const Rational *pPreviousValue, const Rational *pPreviousDayValue);

	/**
	 *	@brief	Get the numerator, denominator and trending.
	 *
	 *	@param	numerator the numerator portion of the value.
	 *	@param	denominator the denominator portion of the value.
	 *	@param	tick the tick on the previous trend.
	 *	@param	trendOnDay the trend on the day.
	 *	@param	trendOnPrevious the trend on the previous.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_NOT_INITIALIZED
	 */
	StatusCode Get(int64_t &numerator, Rational::Denominator &denominator, Tick &tick, Trend &trendOnDay, Trend &trendOnPrevious) const;

	/**
	 *	@brief	Get the rational and trending.
	 *
	 *	@param	value the value.
	 *	@param	tick the tick on the previous trend.
	 *	@param	trendOnDay the trend on the day.
	 *	@param	trendOnPrevious the trend on the previous.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_NOT_INITIALIZED
	 */
	StatusCode Get(Rational &value, Tick &tick, Trend &trendOnDay, Trend &trendOnPrevious) const;

	/**
	 *	@brief	Get double.
	 *
	 *	@return	value.
	 *
	 *	@throw	Exception if not initialized or out of range.
	 */
	double Get() const;

	/**
	 *	@brief	Get the numerator and denominator.
	 *
	 *	@param	numerator the numerator portion of the value.
	 *	@param	denominator the denominator portion of the value.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_NOT_INITIALIZED
	 */
	StatusCode GetNumeratorAndDenominator(int64_t &numerator, Rational::Denominator &denominator) const;

	/**
	 *	@brief	Get the rational.
	 *
	 *	@return	the rational.
	 *
	 *	@throw	ActivException
	 */
	const Rational &GetRational() const;

	/**
	 *	@brief	Get the numerator portion of the value stored.
	 *
	 *	@return	the value of the numerator.
	 *
	 *	@throw	ActivException
	 */
	int64_t GetNumerator() const;

	/**
	 *	@brief	Get the numerator (in the specificied denominator).
	 *
	 *	@param	numerator the numerator.
	 *	@param	denominator the denominator.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_NOT_INITIALIZED
	 *	@retval	STATUS_CODE_OUT_OF_RANGE
	 */
	StatusCode GetNumerator(int64_t &numerator, const Rational::Denominator denominator) const;

	/**
	 *	@brief	Get the numerator and divisor.
	 *
	 *	@param	numerator the numerator.
	 *	@param	divisor the divisor.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_NOT_INITIALIZED
	 *	@retval	STATUS_CODE_OUT_OF_RANGE
	 */
	StatusCode GetNumeratorAndDivisor(int64_t &numerator, uint64_t &divisor) const;

	/**
	 *	@brief	Set the denominator portion of the value stored.
	 *
	 *	This function will attempt to scale the existing numerator in the record.
	 *
	 *	@param	denominator the denominator portion of the value.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_NOT_INITIALIZED
	 *	@retval	STATUS_CODE_OUT_OF_RANGE
	 */
	StatusCode SetDenominator(const Rational::Denominator denominator);

	/**
	 *	@brief	Get the denominator portion of the value stored.
	 *
	 *	@return	the value of the denominator.
	 *
	 *	@throw	ActivException
	 */
	Rational::Denominator GetDenominator() const;

	/**
	 *	@brief	Compress the numerator and denominator without losing precision.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_NOT_INITIALIZED
	 */
	StatusCode Compress();

	/**
	 *	@brief	Set the tick.
	 *
	 *	@param	tick the tick.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_NOT_INITIALIZED
	 */
	StatusCode SetTick(const Tick tick);

	/**
	 *	@brief	Set the tick.
	 *
	 *	@param	previousValue the previous value.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_NOT_INITIALIZED
	 */
	StatusCode SetTick(const TRational &previousValue);

	/**
	 *	@brief	Set the tick and trend on previous.
	 *
	 *	@param	previousValue the previous value.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_NOT_INITIALIZED
	 */
	StatusCode SetTickAndTrendOnPrevious(const TRational &previousValue);

	/**
	 *	@brief	Get the tick.
	 *
	 *	@return	the tick.
	 *
	 *	@throw	ActivException
	 */
	Tick GetTick() const;

	/**
	 *	@brief	Set the trend on day.
	 *
	 *	@param	trend the trend.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_NOT_INITIALIZED
	 */
	StatusCode SetTrendOnDay(const Trend trend);

	/**
	 *	@brief	Set the trend on day.
	 *
	 *	@param	previousDayValue the previous day's value.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_NOT_INITIALIZED
	 */
	StatusCode SetTrendOnDay(const Rational &previousDayValue);

	/**
	 *	@brief	Get the trend on day.
	 *
	 *	@return	the trend.
	 *
	 *	@throw	ActivException
	 */
	Trend GetTrendOnDay() const;

	/**
	 *	@brief	Set the trend on previous.
	 *
	 *	@param	trend the trend.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_NOT_INITIALIZED
	 */
	StatusCode SetTrendOnPrevious(const Trend trend);

	/**
	 *	@brief	Set the trend on previous.
	 *
	 *	@param	previousValue the previous value.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_NOT_INITIALIZED
	 */
	StatusCode SetTrendOnPrevious(const Rational &previousValue);

	/**
	 *	@brief	Get the trend on previous.
	 *
	 *	@return	the trend.
	 *
	 *	@throw	ActivException
	 */
	Trend GetTrendOnPrevious() const;

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
	 *	@retval	STATUS_CODE_OUT_OF_RANGE
	 */
	StatusCode Compare(const TRational &rhs, int &result) const;

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
	 *	@retval	STATUS_CODE_INVALID_FORMAT
	 *	@retval	STATUS_CODE_OUT_OF_RANGE
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
	 *	@param	size the size of the numerator.
	 *
	 *	@return	the maximum serialized length of the object.
	 */
	static size_t GetMaxSerializedLength(const size_t size);

	/**
	 *	@brief	Get the maximum serialized length of the object's body.
	 *
	 *	@param	size the size of the numerator.
	 *
	 *	@return	the maximum serialized length of the object's body.
	 */
	static size_t GetMaxSerializedBodyLength(const size_t size);

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
	 *	@retval	STATUS_CODE_OUT_OF_RANGE
	 */
	static StatusCode Compare(const void * const pLhsSerializedBody, const size_t lhsSerializedBodyLength, const void * const pRhsSerializedBody, const size_t rhsSerializedBodyLength, int &result);

	/**
	 *	@brief	Check whether a tick is valid.
	 *
	 *	@param	tick the tick to check.
	 *
	 *	@return	whether the tick is valid.
	 */
	static bool IsValidTick(const Tick tick);

	/**
	 *	@brief	Check whether a trend is valid.
	 *
	 *	@param	trend the trend to check.
	 *
	 *	@return	whether the trend is valid.
	 */
	static bool IsValidTrend(const Trend trend);

	/**
	 *	@brief	Convert the tick to a string.
	 *
	 *	@param	tick the tick to convert.
	 *
	 *	@return	the tick as a string.
	 */
	static std::string TickToString(const Tick tick);

	/**
	 *	@brief	Convert the trend to a string.
	 *
	 *	@param	trend the trend to convert.
	 *
	 *	@return	the trend as a string.
	 */
	static std::string TrendToString(const Trend trend);

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
	/**
	 *	@brief	Serialized trend definitions.
	 */
	class SerializedTrendDefinitions
	{
	public:
		typedef byte_t SerializedTrend; ///< The serialized trend typedef.

		static const size_t	RESERVED_OFFSET				= 0;											///< The ireserved offset.
		static const size_t	RESERVED_LENGTH				= 2;											///< The ireserved length.
		static const size_t	TICK_OFFSET					= (RESERVED_OFFSET + RESERVED_LENGTH);			///< The tick offset.
		static const size_t	TICK_LENGTH					= 2;											///< The tick length.
		static const size_t	TREND_ON_DAY_OFFSET			= (TICK_OFFSET + TICK_LENGTH);					///< The trend on day offset.
		static const size_t	TREND_ON_DAY_LENGTH			= 2;											///< The trend on day length.
		static const size_t	TREND_ON_PREVIOUS_OFFSET	= (TREND_ON_DAY_OFFSET + TREND_ON_DAY_LENGTH);	///< The trend on previous offset.
		static const size_t	TREND_ON_PREVIOUS_LENGTH	= 2;											///< The trend on previous length.

		/**
		 *	@brief	Get the serialized trend.
		 *
		 *	@param	tick the tick.
		 *	@param	trendOnDay the trend on the day.
		 *	@param	trendOnPrevious the trend on the previous.
		 *
		 *	@return	the serialized trend.
		 */
		static SerializedTrend GetSerializedTrend(const Tick tick, const Trend trendOnDay, const Trend trendOnPrevious);

		/**
		 *	@brief	Get the tick.
		 *
		 *	@param	serializedTrend the serialized trend.
		 *
		 *	@return	the tick.
		 */
		static Tick GetTick(const SerializedTrend serializedTrend);

		/**
		 *	@brief	Get the trend on day.
		 *
		 *	@param	serializedTrend the serialized trend.
		 *
		 *	@return	the trend on day.
		 */
		static Trend GetTrendOnDay(const SerializedTrend serializedTrend);

		/**
		 *	@brief	Get the trend on previous.
		 *
		 *	@param	serializedTrend the serialized trend.
		 *
		 *	@return	the trend on previous.
		 */
		static Trend GetTrendOnPrevious(const SerializedTrend serializedTrend);
	};

	Rational	m_value;			///< The value.
	Tick		m_tick;				///< The tick.
	Trend		m_trendOnDay;		///< The trend on the day.
	Trend		m_trendOnPrevious;	///< The trend on the previous.
};

// ---------------------------------------------------------------------------------------------------------------------------------

inline TRational::SerializedTrendDefinitions::SerializedTrend TRational::SerializedTrendDefinitions::GetSerializedTrend(const Tick tick, const Trend trendOnDay, const Trend trendOnPrevious)
{
	return static_cast<SerializedTrend>((tick << ((sizeof(SerializedTrend) * CHAR_BIT) - (TICK_OFFSET + TICK_LENGTH))) | (trendOnDay << ((sizeof(SerializedTrend) * CHAR_BIT) - (TREND_ON_DAY_OFFSET + TREND_ON_DAY_LENGTH))) | (trendOnPrevious << ((sizeof(SerializedTrend) * CHAR_BIT) - (TREND_ON_PREVIOUS_OFFSET + TREND_ON_PREVIOUS_LENGTH))));
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline TRational::Tick TRational::SerializedTrendDefinitions::GetTick(const SerializedTrend serializedTrend)
{
	return static_cast<Tick>((serializedTrend >> ((sizeof(SerializedTrend) * CHAR_BIT) - (TICK_OFFSET + TICK_LENGTH))) & ((1 << TICK_LENGTH) - 1));
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline TRational::Trend TRational::SerializedTrendDefinitions::GetTrendOnDay(const SerializedTrend serializedTrend)
{
	return static_cast<Trend>((serializedTrend >> ((sizeof(SerializedTrend) * CHAR_BIT) - (TREND_ON_DAY_OFFSET + TREND_ON_DAY_LENGTH))) & ((1 << TREND_ON_DAY_LENGTH) - 1));
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline TRational::Trend TRational::SerializedTrendDefinitions::GetTrendOnPrevious(const SerializedTrend serializedTrend)
{
	return static_cast<Trend>((serializedTrend >> ((sizeof(SerializedTrend) * CHAR_BIT) - (TREND_ON_PREVIOUS_OFFSET + TREND_ON_PREVIOUS_LENGTH))) & ((1 << TREND_ON_PREVIOUS_LENGTH) - 1));
}

} // namespace Activ

#endif // !defined (ACTIV_TRATIONAL_H)

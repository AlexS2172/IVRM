/**
 *	@file	Rational.h
 *
 *	@brief	Header file for the rational class.
 *
 *	$Log: $
 */

#if !defined (ACTIV_RATIONAL_H)
#define ACTIV_RATIONAL_H

#include "ActivMiddleware/ActivFieldTypes/External.h"
#include "ActivMiddleware/ActivFieldTypes/IFieldType.h"

#include "ActivMiddleware/Misc/MemoryHelper.h"

#include "ActivMiddleware/ActivBase/ActivBase.h"
#include "ActivMiddleware/ActivBase/ActivException.h"

namespace Activ
{

/**
*	@brief	The denominator list.
*/
#define ACTIV_DENOMINATOR_LIST(d)												\
	d(DENOMINATOR_WHOLE,		1,			1,			0,		0,		0)		\
	d(DENOMINATOR_1DP,			10,			0,			1,		1,		0)		\
	d(DENOMINATOR_2DP,			100,		0,			2,		2,		0)		\
	d(DENOMINATOR_3DP,			1000,		0,			3,		3,		0)		\
	d(DENOMINATOR_4DP,			10000,		0,			4,		4,		0)		\
	d(DENOMINATOR_5DP,			100000,		0,			5,		5,		0)		\
	d(DENOMINATOR_6DP,			1000000,	0,			6,		6,		0)		\
	d(DENOMINATOR_7DP,			10000000,	0,			7,		7,		0)		\
	d(DENOMINATOR_8DP,			100000000,	0,			8,		8,		0)		\
	d(DENOMINATOR_9DP,			1000000000,	0,			9,		9,		0)		\
	d(DENOMINATOR_X1E1,			0,			10,			0,		0,		1)		\
	d(DENOMINATOR_X1E2,			0,			100,		0,		0,		2)		\
	d(DENOMINATOR_X1E3,			0,			1000,		0,		0,		3)		\
	d(DENOMINATOR_X1E4,			0,			10000,		0,		0,		4)		\
	d(DENOMINATOR_X1E5,			0,			100000,		0,		0,		5)		\
	d(DENOMINATOR_X1E6,			0,			1000000,	0,		0,		6)		\
	d(DENOMINATOR_X1E7,			0,			10000000,	0,		0,		7)		\
	d(DENOMINATOR_X1E8,			0,			100000000,	0,		0,		8)		\
	d(DENOMINATOR_X1E9,			0,			1000000000,	0,		0,		9)		\
	d(DENOMINATOR_HALF,			2,			0,			1,		1,		0)		\
	d(DENOMINATOR_QUARTER,		4,			0,			2,		1,		0)		\
	d(DENOMINATOR_8TH,			8,			0,			3,		1,		0)		\
	d(DENOMINATOR_16TH,			16,			0,			4,		2,		0)		\
	d(DENOMINATOR_32ND,			32,			0,			5,		2,		0)		\
	d(DENOMINATOR_64TH,			64,			0,			6,		2,		0)		\
	d(DENOMINATOR_128TH,		128,		0,			7,		3,		0)		\
	d(DENOMINATOR_256TH,		256,		0,			8,		3,		0)		\
	d(DENOMINATOR_512TH,		512,		0,			9,		3,		0)		\
	d(DENOMINATOR_CABINET,		1,			0,			0,		0,		0)		\
	d(DENOMINATOR_UNDEFINED,	0,			0,			0,		0,		0)

/**
 *	@brief	The denominator enumeration declaration.
 */
#define ACTIV_DENOMINATOR_ENUMERATION(enumeration, divisor, multiplier, nDecimalPlaces, nDivisorDigits, multiplierPower)	enumeration,

/**
 *	@brief	The denominator specification declaration.
 */
#define ACTIV_DENOMINATOR_SPECIFICATION(enumeration, divisor, multiplier, nDecimalPlaces, nDivisorDigits, multiplierPower)	Rational::DenominatorType(divisor, multiplier, nDecimalPlaces, nDivisorDigits, multiplierPower),

// ---------------------------------------------------------------------------------------------------------------------------------

class MessageBuilder;
class MessageValidater;

/**
 *	@brief	Rational class.
 */
class Rational : public IFieldType
{
public:
	static const FieldType FIELD_TYPE = FIELD_TYPE_RATIONAL; ///< The field type.

	static const int64_t MIN_NUMERATOR = -INT64_MAX;	///< The minimum value for the numerator.
	static const int64_t MAX_NUMERATOR = INT64_MAX;		///< The maximum value for the numerator.

	using IFieldType::FromString;

	/**
	 *	@brief	Denominator enumeration.
	 */
	enum Denominator
	{
		ACTIV_DENOMINATOR_LIST(ACTIV_DENOMINATOR_ENUMERATION)
		NUMBER_OF_DENOMINATORS
	};

	/**
	 *	@brief	Denominator type.
	 */
	class DenominatorType
	{
	public:
		/**
		 *	@brief	Constructor.
		 *
		 *	@param	divisor the divisor.
		 *	@param	multiplier the multiplier.
		 *	@param	nDecimalPlaces the number of decimal places.
		 *	@param	nDivisorDigits the number of divisor digits.
		 *	@param	multiplierPower the multiplier power.
		 */
		DenominatorType(const uint64_t divisor, const uint64_t multiplier, const size_t nDecimalPlaces, const size_t nDivisorDigits, const size_t multiplierPower);

		/**
		 *	@brief	Get the divisor.
		 *
		 *	@return	the divisor.
		 */
		uint64_t GetDivisor() const;

		/**
		 *	@brief	Get the multiplier.
		 *
		 *	@return	the multiplier.
		 */
		uint64_t GetMultiplier() const;

		/**
		 *	@brief	Get the number of decimal places.
		 *
		 *	@return	the number of decimal places.
		 */
		size_t GetDecimalPlaces() const;

		/**
		 *	@brief	Get the number of divisor digits.
		 *
		 *	@return	the number of divisor digits.
		 */
		size_t GetDivisorDigits() const;

		/**
		 *	@brief	Get the multiplier power.
		 *
		 *	@return	the multiplier power.
		 */
		size_t GetMultiplierPower() const;

		/**
		 *	@brief	Check whether the denominator type is a divisor.
		 *
		 *	@return	whether the denominator type is a divisor.
		 */
		bool IsDivisor() const;

		/**
		 *	@brief	Check whether the denominator type is a binary divisor.
		 *
		 *	@return	whether the denominator type is a binary divisor.
		 */
		bool IsDivisorBinary() const;

		/**
		 *	@brief	Check whether the denominator type is a decimal divisor.
		 *
		 *	@return	whether the denominator type is a decimal divisor.
		 */
		bool IsDivisorDecimal() const;

		/**
		 *	@brief	Check whether the denominator type is a multiplier.
		 *
		 *	@return	whether the denominator type is a multiplier.
		 */
		bool IsMultiplier() const;

	private:
		const uint64_t	m_divisor;			///< The divisor.
		const uint64_t	m_multiplier;		///< The multiplier.
		const size_t	m_nDecimalPlaces;	///< The number of decimal places.
		const size_t	m_nDivisorDigits;	///< The number of divisor digits.
		const size_t	m_multiplierPower;	///< The multiplier power.
	};

	/**
	 *	@brief	Default constructor.
	 */
	Rational();

	/**
	 *	@brief	Constructor.
	 *
	 *	@param	numerator the numerator portion of the value.
	 *	@param	denominator the denominator portion of the value.
	 *	@param	shouldCompress indicates whether the numerator and denominator should be compressed.
	 *
	 *	@throw	ActivException
	 */
	Rational(const int64_t numerator, const Denominator denominator = DENOMINATOR_WHOLE, bool shouldCompress = false);

	/**
	 *	@brief	Destructor.
	 *
	 *	@throw	ActivException
	 */
	virtual ~Rational();

	/**
	 *	@brief	Equality operator.
	 *
	 *	@param	rhs the object to compare with.
	 *
	 *	@return	the result of the test.
	 *
	 *	@throw	ActivException
	 */
	bool operator==(const Rational &rhs) const;

	/**
	 *	@brief	Inequality operator.
	 *
	 *	@param	rhs the object to compare with.
	 *
	 *	@return	the result of the test.
	 *
	 *	@throw	ActivException
	 */
	bool operator!=(const Rational &rhs) const;

	/**
	 *	@brief	Less than operator.
	 *
	 *	@param	rhs the object to compare with.
	 *
	 *	@return	the result of the test.
	 *
	 *	@throw	ActivException
	 */
	bool operator<(const Rational &rhs) const;

	/**
	 *	@brief	Less than or equal to operator.
	 *
	 *	@param	rhs the object to compare with.
	 *
	 *	@return	the result of the test.
	 *
	 *	@throw	ActivException
	 */
	bool operator<=(const Rational &rhs) const;

	/**
	 *	@brief	Greater than operator.
	 *
	 *	@param	rhs the object to compare with.
	 *
	 *	@return	the result of the test.
	 *
	 *	@throw	ActivException
	 */
	bool operator>(const Rational &rhs) const;

	/**
	 *	@brief	Greater than or equal to operator.
	 *
	 *	@param	rhs the object to compare with.
	 *
	 *	@return	the result of the test.
	 *
	 *	@throw	ActivException
	 */
	bool operator>=(const Rational &rhs) const;

	/**
	 *	@brief	Addition assignment operator.
	 *
	 *	@param	rhs the value to add to this object.
	 *
	 *	@return	a reference to this object.
	 *
	 *	@throw	ActivException
	 */
	Rational& operator+=(const Rational &rhs);

	/**
	 *	@brief	Subtraction assignment operator.
	 *
	 *	@param	rhs the value to subtract from this object.
	 *
	 *	@return	a reference to this object.
	 *
	 *	@throw	ActivException
	 */
	Rational& operator-=(const Rational &rhs);

	/**
	 *	@brief	Multiplication assignment operator.
	 *
	 *	@param	rhs the value to multiply this object by.
	 *
	 *	@return	a reference to this object.
	 *
	 *	@throw	ActivException
	 */
	Rational& operator*=(const Rational &rhs);

	/**
	 *	@brief	Division assignment operator.
	 *
	 *	@param	rhs the value to divide this object by.
	 *
	 *	@return	a reference to this object.
	 *
	 *	@throw	ActivException
	 */
	Rational& operator/=(const Rational &rhs);

	/**
	 *	@brief	Set a new value using numerator and denominator.
	 *
	 *	@param	numerator the numerator portion of the value.
	 *	@param	denominator the denominator portion of the value.
	 *	@param	shouldCompress indicates whether the numerator and denominator should be compressed.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 */
	StatusCode Set(const int64_t numerator, const Denominator denominator, bool shouldCompress = false);

	/**
	 *	@brief	Get the numerator and denominator.
	 *
	 *	@param	numerator the numerator portion of the value.
	 *	@param	denominator the denominator portion of the value.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_NOT_INITIALIZED
	 */
	StatusCode Get(int64_t &numerator, Denominator &denominator) const;

	/**
	 *	@brief	Get double.
	 *
	 *	@return	value.
	 *
	 *	@throw	Exception if not initialized or out of range.
	 */
	double Get() const;

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
	StatusCode GetNumerator(int64_t &numerator, const Denominator denominator) const;

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
	ACTIV_FIELD_TYPES_API StatusCode GetNumeratorAndDivisor(int64_t &numerator, uint64_t &divisor) const;

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
	StatusCode SetDenominator(const Denominator denominator);

	/**
	 *	@brief	Get the denominator portion of the value stored.
	 *
	 *	@return	the value of the denominator.
	 *
	 *	@throw	ActivException
	 */
	Denominator GetDenominator() const;

	/**
	 *	@brief	Get double.
	 *
	 *	@param	value the value.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_NOT_INITIALIZED
	 *	@retval	STATUS_CODE_OUT_OF_RANGE
	 */
	StatusCode GetDouble(double &value) const;

	/**
	 *	@brief	Compress the numerator and denominator without losing precision.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_NOT_INITIALIZED
	 */
	StatusCode Compress();

	/**
	 *	@brief	Add.
	 *
	 *	@param	rhs the value to add to this object.
	 *	@param	isExactCalculation whether loss of precision is allowed.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_NOT_INITIALIZED
	 *	@retval	STATUS_CODE_OUT_OF_RANGE
	 */
	ACTIV_FIELD_TYPES_API StatusCode Add(const Rational &rhs, bool isExactCalculation = true);

	/**
	 *	@brief	Subtract.
	 *
	 *	@param	rhs the value to subtract from this object.
	 *	@param	isExactCalculation whether loss of precision is allowed.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_NOT_INITIALIZED
	 *	@retval	STATUS_CODE_OUT_OF_RANGE
	 */
	ACTIV_FIELD_TYPES_API StatusCode Subtract(const Rational &rhs, bool isExactCalculation = true);

	/**
	 *	@brief	Multiply.
	 *
	 *	@param	rhs the value to multiply this object by.
	 *	@param	isExactCalculation whether loss of precision is allowed.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_NOT_INITIALIZED
	 *	@retval	STATUS_CODE_OUT_OF_RANGE
	 */
	ACTIV_FIELD_TYPES_API StatusCode Multiply(const Rational &rhs, bool isExactCalculation = true);

	/**
	 *	@brief	Divide.
	 *
	 *	@param	rhs the value to divide this object by.
	 *	@param	isExactCalculation whether loss of precision is allowed.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_NOT_INITIALIZED
	 *	@retval	STATUS_CODE_OUT_OF_RANGE
	 */
	ACTIV_FIELD_TYPES_API StatusCode Divide(const Rational &rhs, bool isExactCalculation = true);

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
	 *	@retval	STATUS_CODE_OUT_OF_RANGE
	 */
	ACTIV_FIELD_TYPES_API StatusCode Compare(const Rational &rhs, int &result) const;

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
	 *	@param	messageValidater the message validater.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_LENGTH
	 *	@retval	...
	 */
	ACTIV_FIELD_TYPES_API virtual StatusCode FromString(MessageValidater &messageValidater);

	/**
	 *	@brief	Serialize the object into the supplied message builder.
	 *
	 *	@param	messageBuilder the message builder to store a serialized version of the object in.
	 *	@param	maxSerializedBodyLength the maximum serialized body length.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_NOT_INITIALIZED
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
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
	 *	@retval	STATUS_CODE_NOT_INITIALIZED
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
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
	 *	@retval	...
	 */
	ACTIV_FIELD_TYPES_API virtual StatusCode SerializeLengthAndBody(MessageBuilder &messageBuilder, const size_t maxSerializedBodyLength = MAX_SERIALIZED_BODY_LENGTH_UNDEFINED) const;

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
	ACTIV_FIELD_TYPES_API virtual StatusCode Deserialize(MessageValidater &messageValidater, const size_t maxSerializedBodyLength);

	/**
	 *	@brief	Deserialize the object's body from the supplied data buffer.
	 *
	 *	@param	pSerializedBody the buffer from which the object's body will be extracted.
	 *	@param	serializedBodyLength the serialized body length.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
	ACTIV_FIELD_TYPES_API virtual StatusCode DeserializeBody(const void * const pSerializedBody, const size_t serializedBodyLength);

	/**
	 *	@brief	Deserialize the object's length and body from the supplied message validater.
	 *
	 *	@param	messageValidater the message validater from which the object will be extracted.
	 *	@param	maxSerializedBodyLength the maximum serialized body length.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
	ACTIV_FIELD_TYPES_API virtual StatusCode DeserializeLengthAndBody(MessageValidater &messageValidater, const size_t maxSerializedBodyLength = MAX_SERIALIZED_BODY_LENGTH_UNDEFINED);

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
	ACTIV_FIELD_TYPES_API static StatusCode Compare(const void * const pLhsSerializedBody, const size_t lhsSerializedBodyLength, const void * const pRhsSerializedBody, const size_t rhsSerializedBodyLength, int &result);

	/**
	 *	@brief	Get the denominator type.
	 *
	 *	@param	denominator the denominator.
	 *	@param	pDenominatorType a pointer to the denominator type.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 */
	static StatusCode GetDenominatorType(const Denominator denominator, const DenominatorType *&pDenominatorType);

	/**
	 *	@brief	Get the denominator by binary power.
	 *
	 *	@param	binaryPower the binary power (divisor = 2^binaryPower).
	 *	@param	denominator the denominator.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 */
	ACTIV_FIELD_TYPES_API static StatusCode GetDenominatorByBinaryPower(const size_t binaryPower, Denominator &denominator);

	/**
	 *	@brief	Get the denominator by decimal places.
	 *
	 *	@param	nDecimalPlaces the number of decimal places.
	 *	@param	denominator the denominator.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 */
	ACTIV_FIELD_TYPES_API static StatusCode GetDenominatorByDecimalPlaces(const size_t nDecimalPlaces, Denominator &denominator);

	/**
	 *	@brief	Get the denominator by decimal power.
	 *
	 *	@param	decimalPower the decimal power (divisor = 10^decimalPower).
	 *	@param	denominator the denominator.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 */
	ACTIV_FIELD_TYPES_API static StatusCode GetDenominatorByDecimalPower(const size_t decimalPower, Denominator &denominator);

	/**
	 *	@brief	Get the denominator by divisor.
	 *
	 *	@param	divisor the divisor.
	 *	@param	denominator the denominator.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_OUT_OF_RANGE
	 */
	ACTIV_FIELD_TYPES_API static StatusCode GetDenominatorByDivisor(const uint64_t divisor, Denominator &denominator);

	/**
	 *	@brief	Get the denominator by multiplier power.
	 *
	 *	@param	multiplierPower the multiplier power (multiplier = 10^multiplierPower).
	 *	@param	denominator the denominator.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 */
	ACTIV_FIELD_TYPES_API static StatusCode GetDenominatorByMultiplierPower(const size_t multiplierPower, Denominator &denominator);

	/**
	 *	@brief	Get the denominator by multiplier.
	 *
	 *	@param	multiplier the multiplier.
	 *	@param	denominator the denominator.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_OUT_OF_RANGE
	 */
	ACTIV_FIELD_TYPES_API static StatusCode GetDenominatorByMultiplier(const uint64_t multiplier, Denominator &denominator);

	/**
	 *	@brief	Check whether a numerator is valid.
	 *
	 *	@param	numerator the numerator to check.
	 *
	 *	@return	whether the numerator is valid.
	 */
	static bool IsValidNumerator(int64_t numerator);

	/**
	 *	@brief	Check whether a denominator is valid.
	 *
	 *	@param	denominator the denominator to check.
	 *
	 *	@return	whether the denominator is valid.
	 */
	static bool IsValidDenominator(Denominator denominator);

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
	ACTIV_FIELD_TYPES_API static StatusCode ValidateSerializedLengthAndBody(MessageValidater &messageValidater, size_t &serializedBodyOffset, size_t &serializedBodyLength);

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
	 *	@brief	Serialized header definitions.
	 */
	class SerializedHeaderDefinitions
	{
	public:
		typedef byte_t SerializedHeader; ///< The serialized header typedef.

		static const size_t	IS_NEGATIVE_OFFSET	= 0;											///< The is negative offset.
		static const size_t	IS_NEGATIVE_LENGTH	= 1;											///< The is negative length.
		static const size_t	LENGTH_OFFSET		= (IS_NEGATIVE_OFFSET + IS_NEGATIVE_LENGTH);	///< The length offset.
		static const size_t	LENGTH_LENGTH		= 2;											///< The length length.
		static const size_t	DENOMINATOR_OFFSET	= (LENGTH_OFFSET + LENGTH_LENGTH);				///< The denominator offset.
		static const size_t	DENOMINATOR_LENGTH	= 5;											///< The denominator length.

		/**
		 *	@brief	Get the serialized header.
		 *
		 *	@param	isNegative whether the numerator is negative.
		 *	@param	length the length of the numerator.
		 *	@header	denominator the denominator of the numerator.
		 *
		 *	@return	the serialized header.
		 */
		static SerializedHeader GetSerializedHeader(const bool isNegative, const size_t length, const Denominator denominator);

		/**
		 *	@brief	Is negative.
		 *
		 *	@param	serializedHeader the serialized header.
		 *
		 *	@return	whether the numerator is negative.
		 */
		static bool IsNegative(const SerializedHeader serializedHeader);
 
		/**
		 *	@brief	Get the length.
		 *
		 *	@param	serializedHeader the serialized header.
		 *
		 *	@return	the length of the numerator.
		 */
		static size_t GetLength(const SerializedHeader serializedHeader);

		/**
		 *	@brief	Get the denominator.
		 *
		 *	@param	serializedHeader the serialized header.
		 *
		 *	@return	the denominator of the numerator.
		 */
		static Denominator GetDenominator(const SerializedHeader serializedHeader);
	};

	/**
	 *	@brief	Get the sign and absolute value.
	 *
	 *	@param	absoluteValue the absolute value of the numerator.
	 *
	 *	@return	whether the numerator is negative.
	 */
	bool GetSignAndAbsoluteValue(uint64_t &absoluteValue) const;

	/**
	 *	@brief	Get the compressed numerator size.
	 *
	 *	@return	the compressed numerator size.
	 */
	size_t GetCompressedNumeratorSize() const;

	/**
	 *	@brief	Serialize the object's body into the supplied data buffer.
	 *
	 *	This will serialize to a fixed size buffer (GetMaxSerializedLength<T>()).
	 *
	 *	@param	pSerializedBody the buffer to store a serialized version of the object in.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 */
	template <typename T> StatusCode SerializeBody(void *pSerializedBody) const;

	/**
	 *	@brief	Deserialize the object's body from the supplied data buffer.
	 *
	 *	This will deserialize from a fixed size buffer (GetMaxSerializedLength<T>()).
	 *
	 *	@param	pSerializedBody the buffer from which the object will be extracted.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_FORMAT
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_OUT_OF_RANGE
	 */
	template <typename T> StatusCode DeserializeBody(const void * const pSerializedBody);

	/**
	 *	@brief	Scale the numerator.
	 *
	 *	@param	numerator the numerator.
	 *	@param	sourceDenominator the source denominator.
	 *	@param	targetDenominator the target denominator.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_OUT_OF_RANGE
	 */
	ACTIV_FIELD_TYPES_API static StatusCode ScaleNumerator(int64_t &numerator, const Denominator sourceDenominator, const Denominator targetDenominator);

	/**
	 *	@brief	Scale the numerator.
	 *
	 *	@param	numerator the numerator.
	 *	@param	divisor the divisor.
	 *	@param	denominator the denominator.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_OUT_OF_RANGE
	 */
	ACTIV_FIELD_TYPES_API static StatusCode ScaleNumerator(int64_t &numerator, const uint64_t divisor, Denominator &denominator);

	/**
	 *	@brief	Scale the numerator to the best possible precision.
	 *
	 *	@param	numerator the numerator.
	 *	@param	sourceDenominator the source denominator.
	 *	@param	targetDenominator the target denominator.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_OUT_OF_RANGE
	 */
	static StatusCode ScaleNumeratorToBestPrecision(int64_t &numerator, const Denominator sourceDenominator, Denominator &targetDenominator);

	/**
	 *	@brief	Scale the numerator to the best possible precision.
	 *
	 *	@param	numerator the numerator.
	 *	@param	divisor the divisor.
	 *	@param	denominator the denominator.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_OUT_OF_RANGE
	 */
	static StatusCode ScaleNumeratorToBestPrecision(int64_t &numerator, const uint64_t divisor, Denominator &denominator);

	/**
	 *	@brief	Increment the numerator.
	 *
	 *	@param	numerator the numerator.
	 *	@param	increment the increment.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_OUT_OF_RANGE
	 */
	static StatusCode IncrementNumerator(int64_t &numerator, int64_t increment);

	/**
	 *	@brief	Increment the numerator.
	 *
	 *	@param	numerator the numerator.
	 *	@param	increment the increment.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_OUT_OF_RANGE
	 */
	static StatusCode IncrementNumerator(uint64_t &numerator, uint64_t increment);

	/**
	 *	@brief	Decrement the numerator.
	 *
	 *	@param	numerator the numerator.
	 *	@param	decrement the decrement.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_OUT_OF_RANGE
	 */
	static StatusCode DecrementNumerator(int64_t &numerator, int64_t decrement);

	/**
	 *	@brief	Decrement the numerator.
	 *
	 *	@param	numerator the numerator.
	 *	@param	decrement the decrement.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_OUT_OF_RANGE
	 */
	static StatusCode DecrementNumerator(uint64_t &numerator, uint64_t decrement);

	/**
	 *	@brief	Mulitply the numerator.
	 *
	 *	@param	numerator the numerator.
	 *	@param	multiplier the multiplier.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_OUT_OF_RANGE
	 */
	static StatusCode MultiplyNumerator(int64_t &numerator, int64_t multiplier);

	/**
	 *	@brief	Mulitply the numerator.
	 *
	 *	@param	numerator the numerator.
	 *	@param	multiplier the multiplier.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_OUT_OF_RANGE
	 */
	static StatusCode MultiplyNumerator(uint64_t &numerator, uint64_t multiplier);

	/**
	 *	@brief	Divide the numerator.
	 *
	 *	@param	numerator the numerator.
	 *	@param	divisor the divisor.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_OUT_OF_RANGE
	 */
	static StatusCode DivideNumerator(int64_t &numerator, int64_t divisor);

	/**
	 *	@brief	Divide the numerator.
	 *
	 *	@param	numerator the numerator.
	 *	@param	divisor the divisor.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_OUT_OF_RANGE
	 */
	static StatusCode DivideNumerator(uint64_t &numerator, uint64_t divisor);

	/**
	 *	@brief	Compress the numerator and denominator.
	 *
	 *	@param	numerator the numerator.
	 *	@param	denominator the denominator.
	 */
	ACTIV_FIELD_TYPES_API static void CompressNumeratorAndDenominator(int64_t &numerator, Denominator &denominator);

	/**
	 *	@brief	Compress the numerator and divisor.
	 *
	 *	@param	numerator the numerator.
	 *	@param	divisor the divisor.
	 */
	ACTIV_FIELD_TYPES_API static void CompressNumeratorAndDivisor(int64_t &numerator, uint64_t &divisor);

	/**
	 *	@brief	Get the absolute value.
	 *
	 *	@param	value the signed value.
	 *
	 *	@return	the absolute value.
	 */
	static uint64_t GetAbsoluteValue(const int64_t value);

	/**
	 *	@brief	Get the compressed value size.
	 *
	 *	@param	value the value.
	 *
	 *	@return	the compressed value size.
	 */
	static size_t GetCompressedValueSize(const uint64_t value);

	/**
	 *	@brief	Get the denominator by divisor or multiple of divisor.
	 *
	 *	This can return a denominator whose divisor is a multiple of the provided divisor.
	 *
	 *	@param	divisor the divisor.
	 *	@param	denominator the denominator.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_OUT_OF_RANGE
	 */
	static StatusCode GetDenominatorByDivisorOrMultiple(const uint64_t divisor, Denominator &denominator);

	/**
	 *	@brief	Get the highest common factor.
	 *
	 *	@param	firstValue the first value.
	 *	@param	secondValue the second value.
	 *
	 *	@return	the highest common factor.
	 */
	static uint64_t GetHighestCommonFactor(const uint64_t firstValue, const uint64_t secondValue);

	/**
	 *	@brief	Get the lowest common denominator.
	 *
	 *	@param	firstDenominator the first denominator.
	 *	@param	secondDenominator the second denominator.
	 *
	 *	@return	the lowest common denominator.
	 */
	static Denominator GetLowestCommonDenominator(const Denominator firstDenominator, const Denominator secondDenominator);

	/**
	 *	@brief	Get the lowest common divisor.
	 *
	 *	@param	lowestCommonDivisor the lowest common divisor.
	 *	@param	firstValue the first value.
	 *	@param	secondValue the second value.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_OUT_OF_RANGE
	 */
	static StatusCode GetLowestCommonDivisor(uint64_t &lowestCommonDivisor, const uint64_t firstValue, const uint64_t secondValue);

	/**
	 *	@brief	Get lower denominator.
	 *
	 *	This is used in calculation if loss of accuracy is permitted (i.e. 2DP -> 1DP, 1/4 -> 1/2).
	 *
	 *	@param	denominator the denominator.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_OUT_OF_RANGE
	 */
	static StatusCode GetLowerDenominator(Denominator &denominator);

	int64_t		m_numerator;	///< The numerator portion of the value.
	Denominator	m_denominator;	///< The denominator portion of the value.

	ACTIV_FIELD_TYPES_API static const DenominatorType	m_denominatorTypeList[NUMBER_OF_DENOMINATORS];	///< The denominator type list.

	friend class TRational;
};

// ---------------------------------------------------------------------------------------------------------------------------------

inline Rational::Rational() :
	IFieldType(FIELD_TYPE)
{
	m_isInitialized = false;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline Rational::Rational(const int64_t numerator, const Denominator denominator, bool shouldCompress) :
	IFieldType(FIELD_TYPE)
{
	ACTIV_THROW_RESULT_IF(STATUS_CODE_SUCCESS, !=, Set(numerator, denominator, shouldCompress));
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline Rational::~Rational()
{
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline bool Rational::operator==(const Rational &rhs) const
{
	int result;
	ACTIV_THROW_RESULT_IF(STATUS_CODE_SUCCESS, !=, Compare(rhs, result));

	return (0 == result);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline bool Rational::operator!=(const Rational &rhs) const
{
	return !((*this) == rhs);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline bool Rational::operator<(const Rational &rhs) const
{
	int result;
	ACTIV_THROW_RESULT_IF(STATUS_CODE_SUCCESS, !=, Compare(rhs, result));

	return (result < 0);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline bool Rational::operator<=(const Rational &rhs) const
{
	return !((*this) > rhs);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline bool Rational::operator>(const Rational &rhs) const
{
	int result;
	ACTIV_THROW_RESULT_IF(STATUS_CODE_SUCCESS, !=, Compare(rhs, result));

	return (result > 0);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline bool Rational::operator>=(const Rational &rhs) const
{
	return !((*this) < rhs);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline Rational& Rational::operator+=(const Rational &rhs)
{
	ACTIV_THROW_RESULT_IF(STATUS_CODE_SUCCESS, !=, Add(rhs, false));

	return (*this);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline Rational& Rational::operator-=(const Rational &rhs)
{
	ACTIV_THROW_RESULT_IF(STATUS_CODE_SUCCESS, !=, Subtract(rhs, false));

	return (*this);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline Rational& Rational::operator*=(const Rational &rhs)
{
	ACTIV_THROW_RESULT_IF(STATUS_CODE_SUCCESS, !=, Multiply(rhs, false));

	return (*this);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline Rational& Rational::operator/=(const Rational &rhs)
{
	ACTIV_THROW_RESULT_IF(STATUS_CODE_SUCCESS, !=, Divide(rhs, false));

	return (*this);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode Rational::Set(const int64_t numerator, const Denominator denominator, bool shouldCompress)
{
	if ((!IsValidNumerator(numerator)) || (!IsValidDenominator(denominator)))
		return STATUS_CODE_INVALID_PARAMETER;

	m_numerator = (((DENOMINATOR_UNDEFINED == denominator) || (DENOMINATOR_CABINET == denominator)) ? 0 : numerator);
	m_denominator = denominator;

	if (shouldCompress) CompressNumeratorAndDenominator(m_numerator, m_denominator);

	m_isInitialized = true;

	return STATUS_CODE_SUCCESS;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode Rational::Get(int64_t &numerator, Denominator &denominator) const
{
	if (!IsInitialized())
		return STATUS_CODE_NOT_INITIALIZED;

	numerator = m_numerator;
	denominator = m_denominator;

	return STATUS_CODE_SUCCESS;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline double Rational::Get() const
{
	double value;

	ACTIV_THROW_RESULT_IF(STATUS_CODE_SUCCESS, !=, GetDouble(value));

	return value;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline int64_t Rational::GetNumerator() const
{
	if (!IsInitialized())
		ACTIV_THROW(ActivException, STATUS_CODE_NOT_INITIALIZED);

	return m_numerator;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode Rational::GetNumerator(int64_t &numerator, const Denominator denominator) const
{
	if (!IsInitialized())
		return STATUS_CODE_NOT_INITIALIZED;

	numerator = m_numerator;

	ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, ScaleNumerator(numerator, m_denominator, denominator));

	return STATUS_CODE_SUCCESS;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode Rational::SetDenominator(const Denominator denominator)
{
	if (!IsInitialized())
		return STATUS_CODE_NOT_INITIALIZED;

	if (!IsValidDenominator(denominator))
		return STATUS_CODE_INVALID_PARAMETER;

	int64_t numerator = m_numerator;

	ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, ScaleNumerator(numerator, m_denominator, denominator));

	m_numerator = numerator;
	m_denominator = denominator;

	return STATUS_CODE_SUCCESS;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline Rational::Denominator Rational::GetDenominator() const
{
	if (!IsInitialized())
		ACTIV_THROW(ActivException, STATUS_CODE_NOT_INITIALIZED);

	return m_denominator;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode Rational::GetDouble(double &value) const
{
	int64_t numerator;
	uint64_t divisor;
	ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, GetNumeratorAndDivisor(numerator, divisor));

	value = static_cast<double>(numerator) / static_cast<double>(divisor);

	return STATUS_CODE_SUCCESS;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode Rational::Compress()
{
	if (!IsInitialized())
		return STATUS_CODE_NOT_INITIALIZED;

	CompressNumeratorAndDenominator(m_numerator, m_denominator);

	return STATUS_CODE_SUCCESS;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline void Rational::Reset()
{
	m_isInitialized = false;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline void Rational::Clear()
{
	m_isInitialized = false;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline size_t Rational::GetSerializedLength() const
{
	return (IsInitialized() ? GetMaxSerializedLength(GetCompressedNumeratorSize()) : 0);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline size_t Rational::GetSerializedBodyLength() const
{
	return (IsInitialized() ? GetMaxSerializedBodyLength(GetCompressedNumeratorSize()) : 0);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline size_t Rational::GetMaxSerializedLength(const size_t size)
{
	return GetMaxSerializedBodyLength(size);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline size_t Rational::GetMaxSerializedBodyLength(const size_t size)
{
	return sizeof(SerializedHeaderDefinitions::SerializedHeader) + size;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode Rational::GetDenominatorType(const Denominator denominator, const DenominatorType *&pDenominatorType)
{
	if (!IsValidDenominator(denominator))
		return STATUS_CODE_INVALID_PARAMETER;

	pDenominatorType = m_denominatorTypeList + denominator;

	return STATUS_CODE_SUCCESS;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline bool Rational::IsValidNumerator(int64_t numerator)
{
	return (numerator >= MIN_NUMERATOR); // NOTE: (MAX_NUMERATOR == INT64_MAX) so no need to check.
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline bool Rational::IsValidDenominator(Denominator denominator)
{
	return (denominator < NUMBER_OF_DENOMINATORS);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline bool Rational::IsValidSerializedBodyLength(const size_t serializedBodyLength)
{
	return ((GetMaxSerializedBodyLength(sizeof(int64_t)) == serializedBodyLength) || (GetMaxSerializedBodyLength(sizeof(int32_t)) == serializedBodyLength) || (GetMaxSerializedBodyLength(sizeof(int16_t)) == serializedBodyLength) || (GetMaxSerializedBodyLength(sizeof(int8_t)) == serializedBodyLength));
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline bool Rational::GetSignAndAbsoluteValue(uint64_t &absoluteValue) const
{
	if (m_numerator >= 0)
	{
		absoluteValue = static_cast<uint64_t>(m_numerator);
		return false;
	}
	else
	{
		absoluteValue = static_cast<uint64_t>(-m_numerator);
		return true;
	}
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline size_t Rational::GetCompressedNumeratorSize() const
{
	return GetCompressedValueSize(GetAbsoluteValue(m_numerator));
}

// ---------------------------------------------------------------------------------------------------------------------------------

template <typename T> inline StatusCode Rational::SerializeBody(void *pSerializedBody) const
{
	uint64_t absoluteValue;
	const bool isNegative = GetSignAndAbsoluteValue(absoluteValue);

	*(static_cast<SerializedHeaderDefinitions::SerializedHeader *>(pSerializedBody)) = SerializedHeaderDefinitions::GetSerializedHeader(isNegative, sizeof(T), m_denominator);

	CopyValueToUnalignedPointer<T>(static_cast<byte_t *>(pSerializedBody) + sizeof(SerializedHeaderDefinitions::SerializedHeader), static_cast<T>(absoluteValue));

#if !defined(ACTIV_LITTLE_ENDIAN)
	MemoryReverse(static_cast<byte_t *>(pSerializedBody) + sizeof(SerializedHeaderDefinitions::SerializedHeader), sizeof(T));
#endif

	return STATUS_CODE_SUCCESS;
}

// ---------------------------------------------------------------------------------------------------------------------------------

template <typename T> inline StatusCode Rational::DeserializeBody(const void * const pSerializedBody)
{
	const SerializedHeaderDefinitions::SerializedHeader serializedHeader = *(static_cast<const SerializedHeaderDefinitions::SerializedHeader *>(pSerializedBody));

	if (sizeof(T) != SerializedHeaderDefinitions::GetLength(serializedHeader))
		return STATUS_CODE_INVALID_PARAMETER;

	uint64_t unsignedValue = static_cast<uint64_t>(GetValueFromUnalignedPointer<T>(static_cast<const byte_t *>(pSerializedBody) + sizeof(SerializedHeaderDefinitions::SerializedHeader)));

#if !defined(ACTIV_LITTLE_ENDIAN)
	MemoryReverse(&unsignedValue, sizeof(unsignedValue));
#endif

	const Denominator denominator = SerializedHeaderDefinitions::GetDenominator(serializedHeader);

	if (!IsValidDenominator(denominator))
		return STATUS_CODE_INVALID_FORMAT;

	if ((DENOMINATOR_UNDEFINED == denominator) || (DENOMINATOR_CABINET == denominator))
	{
		if (0 != unsignedValue)
			return STATUS_CODE_OUT_OF_RANGE;

		m_numerator = 0;
	}
	else
	{
		if (SerializedHeaderDefinitions::IsNegative(serializedHeader))
		{
			if (unsignedValue > static_cast<uint64_t>(MAX_NUMERATOR))
				return STATUS_CODE_OUT_OF_RANGE;

			m_numerator = -static_cast<int64_t>(unsignedValue);
		}
		else
		{
			if (unsignedValue > static_cast<uint64_t>(-MIN_NUMERATOR))
				return STATUS_CODE_OUT_OF_RANGE;

			m_numerator = static_cast<int64_t>(unsignedValue);
		}
	}

	m_denominator = denominator;
	m_isInitialized = true;

	return STATUS_CODE_SUCCESS;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline uint64_t Rational::GetAbsoluteValue(const int64_t value)
{
	return static_cast<uint64_t>((value >= 0) ? value : -value);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline size_t Rational::GetCompressedValueSize(const uint64_t value)
{
	if (value <= UINT8_MAX)
		return sizeof(uint8_t);

	if (value <= UINT16_MAX)
		return sizeof(uint16_t);

	if (value <= UINT32_MAX)
		return sizeof(uint32_t);

	return sizeof(uint64_t);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline Rational::DenominatorType::DenominatorType(const uint64_t divisor, const uint64_t multiplier, const size_t nDecimalPlaces, const size_t nDivisorDigits, const size_t multiplierPower) :
	m_divisor(divisor),
	m_multiplier(multiplier),
	m_nDecimalPlaces(nDecimalPlaces),
	m_nDivisorDigits(nDivisorDigits),
	m_multiplierPower(multiplierPower)
{
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline uint64_t Rational::DenominatorType::GetDivisor() const
{
	return m_divisor;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline uint64_t Rational::DenominatorType::GetMultiplier() const
{
	return m_multiplier;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline size_t Rational::DenominatorType::GetDecimalPlaces() const
{
	return m_nDecimalPlaces;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline size_t Rational::DenominatorType::GetDivisorDigits() const
{
	return m_nDivisorDigits;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline size_t Rational::DenominatorType::GetMultiplierPower() const
{
	return m_multiplierPower;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline bool Rational::DenominatorType::IsDivisor() const
{
	return (0 != m_divisor);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline bool Rational::DenominatorType::IsDivisorBinary() const
{
	// HACK: should really check a power of 2.
	return ((IsDivisor()) && (0 != (m_divisor % 10)));
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline bool Rational::DenominatorType::IsDivisorDecimal() const
{
	// HACK: should really check a power of 10.
	return ((IsDivisor()) && (0 == (m_divisor % 10)));
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline bool Rational::DenominatorType::IsMultiplier() const
{
	return (0 != m_multiplier);
}

// ---------------------------------------------------------------------------------------------------------------------------------

/**
 *	@brief	Addition operator for the rational object.
 *
 *	@param	lhs the left hand side value.
 *	@param	rhs the right hand side value.
 *
 *	@return	an object containing the result of the operation.
 *
 *	@throw	ActivException
 */
inline const Rational operator+(const Rational &lhs, const Rational &rhs)
{
	return Rational(lhs) += rhs;
}

// ---------------------------------------------------------------------------------------------------------------------------------

/**
 *	@brief	Subtraction operator for the rational object.
 *
 *	@param	lhs the left hand side value.
 *	@param	rhs the right hand side value.
 *
 *	@return	an object containing the result of the operation.
 *
 *	@throw	ActivException
 */
inline const Rational operator-(const Rational &lhs, const Rational &rhs)
{
	return Rational(lhs) -= rhs;
}

// ---------------------------------------------------------------------------------------------------------------------------------

/**
 *	@brief	Multiplication operator for the rational object.
 *
 *	@param	lhs the left hand side value.
 *	@param	rhs the right hand side value.
 *
 *	@return	an object containing the result of the operation.
 *
 *	@throw	ActivException
 */
inline const Rational operator*(const Rational &lhs, const Rational &rhs)
{
	return Rational(lhs) *= rhs;
}

// ---------------------------------------------------------------------------------------------------------------------------------

/**
 *	@brief	Division operator for the rational object.
 *
 *	@param	lhs the left hand side value.
 *	@param	rhs the right hand side value.
 *
 *	@return	an object containing the result of the operation.
 *
 *	@throw	ActivException
 */
inline const Rational operator/(const Rational &lhs, const Rational &rhs)
{
	return Rational(lhs) /= rhs;
}

} // namespace Activ

#endif // !defined (ACTIV_RATIONAL_H)

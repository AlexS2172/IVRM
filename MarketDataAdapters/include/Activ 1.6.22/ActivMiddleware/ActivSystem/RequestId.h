/**
 *	@file	RequestId.h
 *
 *	@brief	Header file for request id class.
 *
 *  $Log: $
 */

#if !defined (ACTIV_REQUEST_ID_H)
#define ACTIV_REQUEST_ID_H

#include "ActivMiddleware/ActivSystem/External.h"
#include "ActivMiddleware/ActivSystem/Address.h"
#include "ActivMiddleware/ActivBase/ActivBase.h"
#include "ActivMiddleware/Misc/MemoryHelper.h"

#include <cassert>
#include <string>

namespace Activ
{

class SyncRequestId;

/**
 *	@brief	Request id class.
 */
class RequestId
{
public:
	typedef uint32_t SendId;									///< request id type for synchronous calls
	typedef byte_t Attribute;									///< attribute type

	static const Attribute ATTRIBUTE_NONE = 0x00;
	static const Attribute ATTRIBUTE_ON = 0x01;
	static const Attribute ATTRIBUTE_RESPONSE = 0x02;
	static const Attribute ATTRIBUTE_USE_RESPONSE_ADDRESS = 0x04;

	static const byte_t MAX_LENGTH = 0x10;

	/**
	 *	@brief	Default constructor.
	 */
	RequestId();

	/**
	 *	@brief	Constructor.
	 *
	 *	@param	pData data to set
	 *	@param	dataLength size of data in pData.
	 */
	RequestId(const void *pData, const size_t dataSize);

	/**
	 *	@brief	Constructor.
	 *
	 *	@param	pData data to set
	 *	@param	dataLength size of data in pData.
	 *	@param	responseAddress address to route response to request.
	 */
	RequestId(const void *pData, const size_t dataSize, const Address &responseAddress);

	/**
	 *	@brief	Copy constructor.
	 *
	 *	@param	requestId to copy from.
	 */
	RequestId(const RequestId &requestId);

	/**
	 *	@brief	Clear the id.
	 */
	void Clear();

	/**
	 *	@brief	Get the length of data.
	 *
	 *	@return	Length of request id data.
	 */
	size_t GetLength() const;

	/**
	 *	@brief	Set the length of data. Truncated if over maxLength.
	 *
	 *	@param	dataLength length to set to.
	 */
	void SetLength(const size_t dataLength);

	/**
	 *	@brief	Get the data.
	 *
	 *	@param	Pointer to request id data.
	 */
	const byte_t *Get() const;

	/**
	 *	@brief	Set the data.
	 *
	 *	@param	pData data to set
	 *	@param	dataLength size of data in pData.
	 */
	void Set(const void *pData, const size_t dataLength);

	/**
	 *	@brief	Set the data.
	 *
	 *	@param	pData data to set
	 *	@param	dataLength size of data in pData.
	 *	@param	responseAddress address to route for response to request.
	 */
	void Set(const void *pData, const size_t dataLength, const Address &responseAddress);

	/**
	 *	@brief	Serialize the request id.
	 *
	 *	@param	messageBuilder reference to MessageBuilder to serialize with.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
	StatusCode Serialize(MessageBuilder &messageBuilder) const;

	/**
	 *	@brief	Deserialize the request id.
	 *
	 *	@param	messageValidater reference to MessageValidater to deserialize with.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
	StatusCode Deserialize(MessageValidater &messageValidater);

	/**
	 *	@brief	Equality operator.
	 *
	 *	@return	true if this is equal to rhs.
	 */
	bool operator==(const RequestId &rhs) const;

	/**
	 *	@brief	Inequality operator.
	 *
	 *	@return	true if this is not equal to rhs.
	 */
	bool operator!=(const RequestId &rhs) const;

	/**
	 *	@brief	Less than operator.
	 *
	 *	@param	rhs RequestId to compare with.
	 *
	 *	@retval	true if this is less than rhs.
	 *	@retval	false if this is greater than or equal to rhs.
	 */
	bool operator<(const RequestId &rhs) const;

	/**
	 *	@brief	Assignment operator.
	 *
	 *	@param	rhs to assign from.
	 */
	void operator=(const RequestId &rhs);

	/**
	 *	@brief	Get the attribute bits.
	 *
	 *	@return	The attributes.
	 */
	Attribute GetAttributes() const;

	/**
	 *	@brief	Set the attribute bits.
	 *
	 *	@param	attributes the attributes.
	 */
	void SetAttributes(Attribute attributes);

	/**
	 *	@brief	Get the response address.
	 *
	 *	@return	Reference to the response address.
	 */
	const Address &GetResponseAddress() const;

	/**
	 *	@brief	Set the response address.
	 *
	 *	@param	responseAddress the address to set.
	 */
	void SetResponseAddress(const Address &responseAddress);

	/**
	 *	@brief	Convert the request id to a printable string.
	 *
	 *	@return	String of the form "aa bb cc dd".
	 */
	std::string ToString() const;

	/**
	 *	@brief	Get a string representation of the message model (req/resp or other).
	 *
	 *	@return	String representation of the message model.
	 */
	std::string MessageModelToString() const;

	/**
	 *	@brief	Get a string representation of the message model (req/resp or other).
	 *
	 *	@param	attributes attributes from a request id.
	 *
	 *	@return	String representation of the message model.
	 */
	static std::string MessageModelToString(const Attribute attributes);

	ACTIV_SYSTEM_API static const RequestId emptyRequestId;		///< an empty request id

private:
	/**
	 *	@brief	Set the data.
	 *
	 *	@param	pData data to set
	 *	@param	dataLength size of data in pData.
	 */
	void CommonSet(const void *pData, const size_t dataSize);

	/**
	 *	@brief	Compare this with another RequestId.
	 *
	 *	@param	rhs Requestid to compare with.
	 *
	 *	@retval	-ve if this is less than rhs.
	 *	@retval 0 if this equal to rhs.
	 *	@retval +ve if this is greater than rhs.
	 */
	int Compare(const RequestId &rhs) const;

	Attribute	m_attributes;									///< attribute byte
	Address		m_responseAddress;								///< optional response address
	byte_t		m_length;										///< bytes used in buffer
	byte_t		m_buffer[MAX_LENGTH];							///< the request id buffer

	friend class SyncRequestId;
};

// ---------------------------------------------------------------------------------------------------------------------------------

inline RequestId::RequestId()
{
	Clear();
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline RequestId::RequestId(const void *pData, const size_t dataSize)
{
	Set(pData, dataSize);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline RequestId::RequestId(const void *pData, const size_t dataLength, const Address &responseAddress)
{
	Set(pData, dataLength, responseAddress);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline RequestId::RequestId(const RequestId &requestId)
{
	*this = requestId;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline void RequestId::Clear()
{
	m_attributes = 0;
	m_responseAddress.Clear();
	m_length = 0;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline size_t RequestId::GetLength() const
{
	return m_length;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline void RequestId::SetLength(const size_t dataLength)
{
	m_length = static_cast<byte_t>(std::min<size_t>(dataLength, sizeof(m_buffer)));
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline const byte_t *RequestId::Get() const
{
	return m_buffer;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline void RequestId::Set(const void *pData, const size_t dataLength)
{
	CommonSet(pData, dataLength);

	m_responseAddress.Clear();
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline void RequestId::Set(const void *pData, const size_t dataLength, const Address &responseAddress)
{
	CommonSet(pData, dataLength);

	if (responseAddress.IsValid())
	{
		m_attributes |= ATTRIBUTE_USE_RESPONSE_ADDRESS;
		m_responseAddress = responseAddress;
	}
	else
	{
		m_responseAddress.Clear();
	}
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode RequestId::Serialize(MessageBuilder &messageBuilder) const
{
	ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, messageBuilder.Append(m_attributes, MessageBuilder::ENDIAN_LITTLE));
	ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, m_responseAddress.Serialize(messageBuilder));
	ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, messageBuilder.Append(m_length, MessageBuilder::ENDIAN_LITTLE));
	ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, messageBuilder.Append(m_buffer, m_length));

	return STATUS_CODE_SUCCESS;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode RequestId::Deserialize(MessageValidater &messageValidater)
{
	ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, messageValidater.ValidateUnsignedBinaryIntegral(sizeof(m_attributes), MessageValidater::ENDIAN_LITTLE, &m_attributes));
	ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, m_responseAddress.Deserialize(messageValidater));
	ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, messageValidater.ValidateUnsignedBinaryIntegral(sizeof(m_length), MessageValidater::ENDIAN_LITTLE, &m_length));
	ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, messageValidater.ValidateBytesCopy(m_length, &m_buffer));

	return STATUS_CODE_SUCCESS;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline bool RequestId::operator==(const RequestId &rhs) const
{
	return 0 == Compare(rhs);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline bool RequestId::operator!=(const RequestId &rhs) const
{
	return 0 != Compare(rhs);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline bool RequestId::operator<(const RequestId &rhs) const
{
	return Compare(rhs) < 0;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline void RequestId::operator=(const RequestId &rhs)
{
	if (this == &rhs)
		return;

	m_attributes = rhs.m_attributes;
	m_responseAddress = rhs.m_responseAddress;
	m_length = rhs.m_length;
	MemoryCopy(m_buffer, rhs.m_buffer, m_length);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline RequestId::Attribute RequestId::GetAttributes() const
{
	return m_attributes;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline void RequestId::SetAttributes(Attribute attributes)
{
	m_attributes = attributes;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline const Address &RequestId::GetResponseAddress() const
{
	return m_responseAddress;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline void RequestId::SetResponseAddress(const Address &responseAddress)
{
	m_responseAddress = responseAddress;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline std::string RequestId::ToString() const
{
	char buff[4];
	std::string output;

	for (size_t i = 0; i < m_length; ++i)
	{
		(void) Snprintf(buff, ACTIV_ARRAY_SIZE(buff), "%s%02x", (i > 0) ? " " : "", m_buffer[i]);

		output += buff;
	}

	return output;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline std::string RequestId::MessageModelToString() const
{
	return MessageModelToString(m_attributes);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline std::string RequestId::MessageModelToString(const Attribute attributes)
{
	if (attributes & ATTRIBUTE_ON)
	{
		if (attributes & ATTRIBUTE_RESPONSE)
			return "response";
		else
			return "request";
	}
	else
	{
		return "message";
	}
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline void RequestId::CommonSet(const void *pData, const size_t dataLength)
{
	assert(dataLength <= sizeof(m_buffer));

	// clear down attributes. request / response is set by Port Send/Post methods
	m_attributes = ATTRIBUTE_NONE;

	m_length = static_cast<byte_t>(std::min<size_t>(dataLength, sizeof(m_buffer)));
	MemoryCopy(m_buffer, pData, m_length);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline int RequestId::Compare(const RequestId &rhs) const
{
	// the attribute bits do not take part in the comparison operation (a returned request id may have
	// different bits set). Same applies for response address. Comparison is only on the body of the request id.

    if (m_length != rhs.m_length)
	{
		const size_t compareLength = std::min(m_length, rhs.m_length);
		const int compareValue = ::memcmp(m_buffer, rhs.m_buffer, compareLength);

		if (0 == compareValue)
			return (m_length == compareLength) ? -1 : 1;
		else
			return compareValue;
	}
	else
	{
		return ::memcmp(m_buffer, rhs.m_buffer, m_length);
	}
}

} // namespace Activ

#endif // !defined (ACTIV_REQUEST_ID_H)

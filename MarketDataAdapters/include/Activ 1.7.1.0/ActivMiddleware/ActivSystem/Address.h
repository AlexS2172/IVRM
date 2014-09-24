/**
 *	@file	Address.h
 *
 *	@brief	Address header file.
 *
 *	$Log: $
 */

#if !defined (ACTIV_ADDRESS_H)
#define ACTIV_ADDRESS_H

#include "ActivMiddleware/ActivSystem/External.h"
#include "ActivMiddleware/ActivBase/MessageBuilder.h"
#include "ActivMiddleware/ActivBase/MessageValidator.h"
#include "ActivMiddleware/ActivBase/ActivBase.h"

namespace Activ
{

/**
 *	@brief	Address.
 */
class Address
{
public:
	/**
	 *	@brief	Default constructor.
	 *
	 *	The endpoint id is set to INVALID_ENDPOINT_ID and the port id to INVALID_PORT_ID.
	 */
	Address();

	/**
	 *	@brief	Constructor.
	 */
	Address(const EndPointId endPointId, const PortId portId);

	/**
	 *	@brief	Serialize the address.
	 */
	StatusCode Serialize(MessageBuilder &messageBuilder) const;

	/**
	 *	@brief	Deserialize the address.
	 */
	StatusCode Deserialize(MessageValidator &messageValidator);

	/**
	 *	@brief	Get the serialized length of an Address object.
	 */
	static size_t GetSerializedLength();

	/**
	 *	@brief	Clear the address.
	 *
	 *	The endpoint id is set to INVALID_ENDPOINT_ID and the port id to INVALID_PORT_ID.
	 */
	void Clear();

	/**
	 *	@brief	Convert the address to a string.
	 */
	std::string ToString() const;

	/**
	 *	@brief	Is this a valid address?
	 */
	bool IsValid() const;

	/**
	 *	@brief	Get the endpoint id.
	 *
	 *	@return	the endpoint id.
	 */
	EndPointId GetEndPointId() const;

	/**
	 *	@brief	Set the endpoint id.
	 *
	 *	@param	endPointId the endpoint id.
	 */
	void SetEndPointId(const EndPointId endPointId);

	/**
	 *	@brief	Get the port id.
	 *
	 *	@return	the port id.
	 */
	PortId GetPortId() const;

	/**
	 *	@brief	Set the port id.
	 *
	 *	@param	portId the port id.
	 */
	void SetPortId(const PortId portId);

	/**
	 *	@brief	Set the endpoint and port id.
	 *
	 *	@param	endPointId the endpoint id.
	 *	@param	portId the port id.
	 */
	void Set(const EndPointId endPointId, const PortId portId);

	/**
	 *	@brief	'==' operator.
	 *
	 *	@param	rhs Address to compare.
	 *
	 *	@return	bool.
	 */
	bool operator==(const Address &rhs) const;

	/**
	 *	@brief	'!=' operator.
	 *
	 *	@param	rhs Address to compare.
	 *
	 *	@return	bool.
	 */
	bool operator!=(const Address &rhs) const;

	/**
	 *	@brief	'<' operator.
	 *
	 *	@param	rhs Address to compare.
	 *
	 *	@return	bool.
	 */
	bool operator<(const Address &rhs) const;

private:
	EndPointId	m_endPointId;
	PortId		m_portId;
};

// ---------------------------------------------------------------------------------------------------------------------------------

inline Address::Address()
{
	Clear();
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline Address::Address(const EndPointId endPointId, const PortId portId)
{
	SetEndPointId(endPointId);
	SetPortId(portId);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode Address::Serialize(MessageBuilder &messageBuilder) const
{
	ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, messageBuilder.Append(m_endPointId, MessageBuilder::ENDIAN_LITTLE));
	ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, messageBuilder.Append(m_portId, MessageBuilder::ENDIAN_LITTLE));

	return STATUS_CODE_SUCCESS;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode Address::Deserialize(MessageValidator &messageValidator)
{
	ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, messageValidator.ValidateUnsignedBinaryIntegral(sizeof(m_endPointId), MessageValidator::ENDIAN_LITTLE, &m_endPointId));
	ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, messageValidator.ValidateUnsignedBinaryIntegral(sizeof(m_portId), MessageValidator::ENDIAN_LITTLE, &m_portId));

	return STATUS_CODE_SUCCESS;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline size_t Address::GetSerializedLength()
{
	return ACTIV_FIELD_SIZE(Address, m_endPointId) + ACTIV_FIELD_SIZE(Address, m_portId);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline void Address::Clear()
{
	SetPortId(INVALID_PORT_ID);
	SetEndPointId(INVALID_ENDPOINT_ID);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline EndPointId Address::GetEndPointId() const
{
	return m_endPointId;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline void Address::SetEndPointId(EndPointId endPointId)
{
	m_endPointId = endPointId;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline PortId Address::GetPortId() const
{
	return m_portId;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline void Address::SetPortId(PortId portId)
{
	m_portId = portId;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline void Address::Set(const EndPointId endPointId, const PortId portId)
{
	m_endPointId = endPointId;
	m_portId = portId;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline std::string Address::ToString() const
{
	char buffer[16];

	Snprintf(buffer, ACTIV_ARRAY_SIZE(buffer), "%u:%u", GetEndPointId(), GetPortId());
	return buffer;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline bool Address::IsValid() const
{
	return (INVALID_ENDPOINT_ID != m_endPointId) && (INVALID_PORT_ID != m_portId);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline bool Address::operator==(const Address &rhs) const
{
	return (m_endPointId == rhs.m_endPointId) && (m_portId == rhs.m_portId);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline bool Address::operator!=(const Address &rhs) const
{
	return !(*this == rhs);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline bool Address::operator<(const Address &rhs) const
{
	if (m_endPointId < rhs.m_endPointId)
		return true;

	if (m_endPointId > rhs.m_endPointId)
		return false;

	return m_portId < rhs.m_portId;
}

} // namespace Activ

#endif // !defined (ACTIV_ADDRESS_H)

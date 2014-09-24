/**
 *	@file	AddressInfo.h
 *
 *	@brief	Header file for address info.
 *
 *  $Log: $
 */

#if !defined (ACTIV_ROUTER_ADDRESS_INFO_H)
#define ACTIV_ROUTER_ADDRESS_INFO_H

#include "ActivMiddleware/ActivIp/IpAddress.h"

#include <string>

namespace Activ
{

/**
 *	@brief	Class wrapping up local, remote address and protocol info for a connection.
 */
class AddressInfo
{
public:
	/**
	 *	@brief	Equality operator.
	 *
	 *	@param	rhs reference to an AddressInfo object to compare.
	 *
	 *	@retval	true if this is equal to rhs.
	 *	@retval	false if this is not equal to rhs.
	 */
	bool operator==(const AddressInfo &rhs) const;

	/**
	 *	@brief	Inequality operator.
	 *
	 *	@param	rhs reference to an AddressInfo object to compare.
	 *
	 *	@retval	true if this is not equal to rhs.
	 *	@retval	false if this is equal to rhs.
	 */
	bool operator!=(const AddressInfo &rhs) const;

	/**
	 *	@brief	Less than operator.
	 *
	 *	@param	rhs reference to a ClientInfo to compare with.
	 *
	 *	@retval	true this is less than rhs.
	 *	@retval	false this is greater than or equal to rhs.
	 */
	bool operator<(const AddressInfo &rhs) const;

	/**
	 *	@brief	Compare two ClientInfo objects.
	 *
	 *	@param	rhs reference to a ClientInfo to compare with.
	 *
	 *	@retval	-ve this is less than rhs.
	 *	@retval	0 this is equal to rhs.
	 *	@retval	+ve this is greater than or equal to rhs.
	 */
	int Compare(const AddressInfo &rhs) const;

	/**
	 *	@brief	Serialize.
	 *
	 *	@param	messageBuilder.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
	StatusCode Serialize(MessageBuilder &messageBuilder) const;

	/**
	 *	@brief	Deserialize.
	 *
	 *	@param	messageValidater.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
	StatusCode Deserialize(MessageValidater &messageValidater);

	std::string		m_protocol;									///< protocol
	IpAddress		m_localAddress;								///< local IP address
	IpAddress		m_remoteAddress;							///< remote IP address
};

// ---------------------------------------------------------------------------------------------------------------------------------

inline bool AddressInfo::operator==(const AddressInfo &rhs) const
{
	return 0 == Compare(rhs);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline bool AddressInfo::operator!=(const AddressInfo &rhs) const
{
	return 0 != Compare(rhs);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline bool AddressInfo::operator<(const AddressInfo &rhs) const
{
	return Compare(rhs) < 0;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline int AddressInfo::Compare(const AddressInfo &rhs) const
{
	if (m_protocol < rhs.m_protocol)
		return -1;

	if (m_protocol > rhs.m_protocol)
		return 1;

	int result;

	result = m_localAddress.Compare(rhs.m_localAddress);
	if (0 != result)
		return result;

	result = m_remoteAddress.Compare(rhs.m_remoteAddress);
	if (0 != result)
		return result;

	return 0;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode AddressInfo::Serialize(MessageBuilder &messageBuilder) const
{
	ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, messageBuilder.Append(m_protocol));
	ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, m_localAddress.Serialize(messageBuilder));
	ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, m_remoteAddress.Serialize(messageBuilder));

	return STATUS_CODE_SUCCESS;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode AddressInfo::Deserialize(MessageValidater &messageValidater)
{
	ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, messageValidater.ValidateString(&m_protocol));
	ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, m_localAddress.Deserialize(messageValidater));
	ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, m_remoteAddress.Deserialize(messageValidater));

	return STATUS_CODE_SUCCESS;
}

} // namespace Activ

#endif // !defined (ACTIV_ROUTER_ADDRESS_INFO_H)

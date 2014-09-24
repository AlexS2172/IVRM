/**
 *	@file	IpAddress.h
 *
 *	@brief	Header file for ip address class.
 *
 *	$Log: $
 */

#if !defined (ACTIV_IP_ADDRESS_H)
#define ACTIV_IP_ADDRESS_H

#include "ActivMiddleware/ActivIp/External.h"
#include "ActivMiddleware/ActivBase/MessageValidator.h"
#include "ActivMiddleware/ActivBase/MessageBuilder.h"

#include <string>
#include <vector>

#if defined (ACTIV_PLATFORM_POSIX)
#include <sys/socket.h>
#endif

namespace Activ
{

/**
 *	@brief	Protocol independant ip address information.
 *
 *	Currently supports ipv4 and ipv6 addresses.
 */
class IpAddress
{
public:
	/**
	 *	@brief	Constructor.
	 *
	 *	The socket type and protocol are initialized to 0.
	 *
	 *	@param	family the address family.
	 */
	IpAddress(const int family = AF_UNSPEC);

	/**
	 *	@brief	Constructor.
	 *
	 *	@param	socketType the socket type.
	 *	@param	protocol the socket protocol.
	 *	@param	family the address family.
	 */
	IpAddress(const int socketType, const int protocol, const int family = AF_UNSPEC);

	/**
	 *	@brief	Compare two IpAddress objects.
	 *
	 *	@param	rhs reference to an IpAddress object to compare.
	 *
	 *	@retval	-ve if this is less than rhs.
	 *	@retval 0 if this equal to rhs.
	 *	@retval +ve if this is greater than rhs.
	 */
	int Compare(const IpAddress &rhs) const;

	/**
	 *	@brief	Equality operator.
	 *
	 *	@param	rhs reference to an IpAddress object to compare.
	 *
	 *	@retval	true if this is equal to rhs.
	 *	@retval	false if this is not equal to rhs.
	 */
	bool operator==(const IpAddress &rhs) const;

	/**
	 *	@brief	Inequality operator.
	 *
	 *	@param	rhs reference to an IpAddress object to compare.
	 *
	 *	@retval	true if this is not equal to rhs.
	 *	@retval	false if this is equal to rhs.
	 */
	bool operator!=(const IpAddress &rhs) const;

	/**
	 *	@brief	Less than operator.
	 *
	 *	@param	rhs reference to an IpAddress object to compare.
	 *
	 *	@retval	true if this is less than rhs.
	 *	@retval	false if this is greater than or equal to rhs.
	 */
	bool operator<(const IpAddress &rhs) const;

	/**
	 *	@brief	Convert address to a string.
	 *
	 *	@return	The string form of the address, or an empty string is returned if the address cannot be converted.
	 */
	ACTIV_IP_API std::string ToString() const;

	/**
	 *	@brief	Get the interface index of a local ipv6 address.
	 *
	 *	@param	index to receive the interface index.
	 *
	 *	@throw	Exception.
	 */
	ACTIV_IP_API uint32_t GetInterfaceIndex() const;

	/**
	 *	@brief	Serialize.
	 *
	 *	@param	messageBuilder.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
	ACTIV_IP_API StatusCode Serialize(MessageBuilder &messageBuilder) const;

	/**
	 *	@brief	Deserialize.
	 *
	 *	@param	messageValidator.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
	ACTIV_IP_API StatusCode Deserialize(MessageValidator &messageValidator);

	socklen_t			m_addressLength;						///< Size of useful info in m_address
	union
	{
		sockaddr		Address;
		sockaddr_in		AddressIn;
		sockaddr_in6	AddressIn6;
	}					m_address;
	uint32_t			m_type;									///< Socket type
	uint32_t			m_protocol;								///< Protocol specific to address family
};

// ---------------------------------------------------------------------------------------------------------------------------------

/**
 *	@brief	Type of list of ip addresses.
 */
typedef std::vector<IpAddress> IpAddressList;

// ---------------------------------------------------------------------------------------------------------------------------------

inline IpAddress::IpAddress(const int family) :
	m_addressLength(0),
	m_type(0),
	m_protocol(0)
{
	std::memset(&m_address, 0, sizeof(m_address));
	m_address.Address.sa_family = family;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline IpAddress::IpAddress(const int socketType, const int protocol, const int family) :
	m_addressLength(0),
	m_type(socketType),
	m_protocol(protocol)
{
	std::memset(&m_address, 0, sizeof(m_address));
	m_address.Address.sa_family = family;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline int IpAddress::Compare(const IpAddress &rhs) const
{
    if (m_addressLength != rhs.m_addressLength)
	{
		const socklen_t compareLength = std::min(m_addressLength, rhs.m_addressLength);
		const int compareValue = std::memcmp(&m_address, &rhs.m_address, compareLength);

		if (0 == compareValue)
			return (m_addressLength == compareLength) ? -1 : 1;
		else
			return compareValue;
	}
	else
	{
		return std::memcmp(&m_address, &rhs.m_address, m_addressLength);
	}
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline bool IpAddress::operator==(const IpAddress &rhs) const
{
	return 0 == Compare(rhs);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline bool IpAddress::operator!=(const IpAddress &rhs) const
{
	return 0 != Compare(rhs);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline bool IpAddress::operator<(const IpAddress &rhs) const
{
	return Compare(rhs) < 0;
}

} // namespace Activ

#endif // !defined (ACTIV_IP_ADDRESS_H)

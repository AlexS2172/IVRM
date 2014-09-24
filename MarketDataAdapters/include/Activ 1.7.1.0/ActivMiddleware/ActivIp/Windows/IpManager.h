/**
 *	@file	Win32/IpManager.h
 *
 *	@brief	Header file for base ip manager class.
 *
 *	$Log: $
 */

#include "ActivMiddleware/ActivIp/External.h"
#include "ActivMiddleware/ActivIp/IpAddress.h"
#include "ActivMiddleware/ActivIp/SocketWrapper.h"
#include "ActivMiddleware/ActivBase/Windows/IoCompletionManager.h"
#include "ActivMiddleware/ActivBase/IniFileParser.h"

#include <string>
#include <vector>

namespace Activ
{

/**
 *	@brief	Ip manager base class.
 */
class IpManager : public IoCompletionManager
{
public:
	/**
	 *	@brief	Settings.
	 */
	class Settings
	{
	public:
		/**
		 *	@brief	Default constructor.
		 */
		ACTIV_IP_API Settings();

		/**
		 *	@brief	Read settings from an ini file.
		 *
		 *	@param	sectionIterator iterator to an ini file section.
		 *
		 *	@retval	STATUS_CODE_SUCCESS
		 *	@retval	...
		 */
		ACTIV_IP_API StatusCode Read(const IniFileParser::ConstSectionIterator &sectionIterator);

		size_t	m_numberOfIoThreads;							///< default 0, let the system decide
		bool	m_enableWindowsOverlappedOperations;			///< default true
	};

	/**
	 *	@brief	Information on a network interface.
	 */
	class InterfaceInfo
	{
	public:
		/**
		 *	@brief	Default constructor.
		 */
		ACTIV_IP_API InterfaceInfo();

		std::string		m_name;
		std::string		m_description;
		std::string		m_macAddress;
		size_t			m_bitsPerSecond;						///< 0 is unknown
		size_t			m_mtu;
		IpAddressList	m_ipAddressList;						///< list of configured ip addresses on this interface
	};

	typedef std::vector<InterfaceInfo> InterfaceInfoList;		///< type of list of InterfaceInfo objects

	/**
	 *	@brief	Default constructor.
	 *
	 *	@param	settings reference to a Settings object.
	 *
	 *	@throw	Exception
	 */
	ACTIV_IP_API IpManager(const Settings &settings);

	/**
	 *	@brief	Destructor.
	 */
	ACTIV_IP_API ~IpManager();

	/**
 	 *	@brief	Get reference to settings.
	 *
	 *	@return	Reference to settings as passed to constructor.
	 */
	const Settings &GetSettings() const;

	/**
	 *	@brief	Resolve a string into an ip address.
	 *
	 *	The string can be a hostname or ip address.
	 *
	 *	@param	nodeName the name of the node.
	 *	@param	portNumber the port number.
	 *	@param	ipAddress reference to an IpAddress object to store the ip address.
	 *	@param	flags AI_PASSIVE, AI_CANONNAME, AI_NUMERICHOST.
	 *
	 *	@throw	Exception
	 */
	ACTIV_IP_API static void GetIpAddress(const std::string &nodeName, const uint16_t portNumber, IpAddress &ipAddress, const int flags = 0);

	/**
	 *	@brief	Get a list of local address.
	 *
	 *	@param	localAddress the address to resolve. If \a localAddress is empty or the hostname, all configured interfaces
	 *			are returned.
	 *			If \a localAddress is one of the "any" addresses (i.e. "0.0.0.0" or "::"), all configured interfaces
	 *			for that protocol (i.e. ipv4 or ipv6) are returned.
	 *			\a localaddress can also be one of the configured interface addresses (eg "172.16.16.2") then only that
	 *			address will be returned.
	 *	@param	portNumber optional port to give the returned addresses. Can be 0.
	 *	@param	ipAddressList to receive the results.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
	ACTIV_IP_API static StatusCode GetLocalIpAddressList(const std::string &localAddress, const uint16_t portNumber, IpAddressList &ipAddressList);

	/**
	 *	@brief	Get a list of information for network interfaces in the machine.
	 *
	 *	@param	family AF_INET, AF_INET6 or AF_UNSPEC to not filter results.
	 *	@param	interfaceInfoList to receive the list of interface information.
	 *
	 *	@return	STATUS_CODE_SUCCESS or an error status code.
	 */
	ACTIV_IP_API static StatusCode GetInterfaceInfoList(const int family, InterfaceInfoList &interfaceInfoList);

	/**
	 *	@brief	Get the hostname and port from a string of the form "host:port".
	 *
	 *	If there is no "port" portion of the input string, portNumber is left unchanged.
	 *
	 *	@param	hostAndPort a string of the form "host:port".
	 *	@param	host reference to a string to receive the host portion of the input string.
	 *	@param	portNumber reference to a variable to receive the port number portion of the input string.
	 *
	 *	@retval	true.
	 *	@retval	false hostAndPort could not be parsed.
	 */
	ACTIV_IP_API static bool GetHostAndPort(const std::string &hostAndPort, std::string &host, uint16_t &portNumber);

	/**
	 *	@brief	Get the hostname.
	 *
	 *	@return	The hostname.
	 */
	ACTIV_IP_API static std::string GetHostName();

	/**
	 *	@brief	Get the address of a winsock extension function.
	 *
	 *	@param	socket the socket to find the extension function for.
	 *	@param	fnStorage pointer to place to store the function pointer.
	 *	@param	fnStorageSize the size of the function pointer storage.
	 *	@param	fnGuid pointer to the guid for the function to find.
	 *	@param	fnGuidSize size of the guid.
	 *
	 *	@throw	Exception
	 */
	ACTIV_IP_API static void GetExtensionFunction(const SOCKET socket, void *fnStorage, const DWORD fnStorageSize, void *fnGuid, const DWORD fnGuidSize);

	/**
	 *	@brief	Create a non-inheritable socket.
	 *
	 *	@param	ipAddress describing type of socket to create.
	 *	@param	socketWrapper to reveive the socket.
	 *
	 *	@throw	Exception.
	 */
	ACTIV_IP_API static void CreateNonInheritableSocket(const IpAddress &ipAddress, SocketWrapper &socketWrapper);

private:
	const Settings	m_settings;									///< settings as passed to constructor
};

} // namespace Activ

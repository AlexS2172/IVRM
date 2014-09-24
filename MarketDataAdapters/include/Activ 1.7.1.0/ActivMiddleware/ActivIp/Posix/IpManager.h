/**
 *	@file	Posix/IpManager.h
 *
 *	@brief	Header file for base ip manager class.
 *
 *	$Log: $
 */

#if !defined (ACTIV_IP_POSIX_IP_MANAGER_H)
#define ACTIV_IP_POSIX_IP_MANAGER_H

#include "ActivMiddleware/ActivIp/External.h"
#include "ActivMiddleware/ActivIp/IpAddress.h"
#include "ActivMiddleware/ActivBase/IniFileParser.h"
#include "ActivMiddleware/ActivBase/Thread.h"

#include "boost/scoped_ptr.hpp"

#include <string>
#include <vector>
#include <netdb.h>

namespace Activ
{

class EventManager;

/**
 *	@brief	Ip manager base class.
 */
class IpManager
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
		Settings();

		/**
		 *	@brief	Read settings from an ini file.
		 *
		 *	@param	sectionIterator iterator to an ini file section.
		 *
		 *	@retval	STATUS_CODE_SUCCESS
		 *	@retval	...
		 */
		StatusCode Read(const IniFileParser::ConstSectionIterator &sectionIterator);

		size_t	m_numberOfIoThreads;							///< ignored on Posix systems
		bool	m_enableWindowsOverlappedOperations;			///< ignored on Posix systems
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
		IpAddressList	m_ipAddressList;
	};

	typedef std::vector<InterfaceInfo> InterfaceInfoList;		///< type of list of InterfaceInfo objects

	/**
	 *	@brief	Default constructor.
	 *
	 *	@param	settings reference to a Settings object.
	 *
	 *	@throw	Exception
	 */
	IpManager(const Settings &settings);

	/**
	 *	@brief	Destructor.
	 */
	~IpManager();

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
	 *	@param	flags AI_PASSIVE, AI_CANNONNAME, AI_NUMERICHOST.
	 *
	 *	@throw	Exception
	 */
	static void GetIpAddress(const std::string &nodeName, const uint16_t portNumber, IpAddress &ipAddress, const int flags = 0);

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
	static StatusCode GetLocalIpAddressList(const std::string &localAddress, const uint16_t portNumber, IpAddressList &ipAddressList);

	/**
	 *	@brief	Get a list of information for network interfaces in the machine.
	 *
	 *	@param	family AF_INET, AF_INET6 or AF_UNSPEC to not filter results.
	 *	@param	interfaceInfoList to receive the list of interface information.
	 *
	 *	@return	STATUS_CODE_SUCCESS or an error status code.
	 */
	static StatusCode GetInterfaceInfoList(const int family, InterfaceInfoList &interfaceInfoList);
							 
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
	static bool GetHostAndPort(const std::string &hostAndPort, std::string &host, uint16_t &portNumber);

	/**
	 *	@brief	Get the hostname.
	 *
	 *	@return	The hostname.
	 */
	static std::string GetHostName();
	
	/**
	 *	@brief	Get reference to event manager.
	 *
	 *	@return	Event manager.
	 */
	EventManager &GetEventManager();

	/**
	 *	@brief	Get max io vecs allowed to sendmsg() and recvmsg().
	 *
	 *	@return	Max value.
	 */
	size_t GetMaxIoVecs() const;

private:
	/**
	 *	@brief	Io thread.
	 */
	class IoThread : public Thread
	{
	public:
		/**
		 *	@brief	Constructor.
		 *
		 *	@param	ipManager reference back to parent IpManager.
		 */
		IoThread(IpManager &ipManager);
		
	protected:
		virtual StatusCode Process(void *pData);
		
	private:
		IpManager &m_ipManager;									///< reference back to parent IpManager	
	};
	
	/**
	 *	@brief	Create event manager.
	 *
	 *	Will try epoll on linux, kevent on bsd and fallback to poll if not available.
	 *
	 *	@return	EventManager pointer.
	 */
	static EventManager *CreateEventManager();

	/**
	 *	@brief	Get a list of information for network interfaces in the machine.
	 *
	 *	@param	family AF_INET, AF_INET6 or AF_UNSPEC to not filter results.
	 *	@param	interfaceInfoList to receive the list of interface information.
	 *
	 *	@return	STATUS_CODE_SUCCESS or an error status code.
	 */
	static StatusCode InternalGetInterfaceInfoList(const int family, InterfaceInfoList &interfaceInfoList);
	
	/**
	 *	@brief	Shutdown io threads.
	 *
	 *	@param	shutdownTimeout how long to wait for the threads to die, in ms.
	 */
	void ShutdownIoThreads(const TimeoutPeriod shutdownTimeout);

	/**
	 *	@brief	Io worker thread entry point.
	 */
	void IoThreadProc();

	const Settings					m_settings;					///< settings as passed to constructor

	IoThread						m_ioThread;
	boost::scoped_ptr<EventManager>	m_pEventManager;
	size_t							m_maxIoVecs;				///< max io vecs to send/recvmsg()
};

// ---------------------------------------------------------------------------------------------------------------------------------

inline EventManager &IpManager::GetEventManager()
{
	return *m_pEventManager;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline size_t IpManager::GetMaxIoVecs() const
{
	return m_maxIoVecs;
}

} // namespace Activ

#endif // !defined (ACTIV_IP_POSIX_IP_MANAGER_H)

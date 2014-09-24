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
#include "ActivMiddleware/ActivBase/ThreadManager.h"

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
class ACTIV_IP_API IpManager : private ThreadManager
{
public:
	/**
	 *	@brief	Information on a network interface.
	 */
	class InterfaceInfo
	{
	public:
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
	 *	Note the constructor calls InitializeSocketLayer() so it isn't necessary for user code to do so.
	 *
	 *	@param	numberOfIoThreads the number of io threads to launch. If 0, let the object decide how many threads to launch.
	 *
	 *	@throw	Exception
	 */
	IpManager(const size_t numberOfIoThreads = 1);

	/**
	 *	@brief	Destructor.
	 *
	 *	Note the destructor calls CleanupSocketLayer() so it isn't necessary for user code to do so.
	 */
	~IpManager();

	/**
	 *	@brief	Initialize the socket layer of the os.
	 *
	 *	This call is provided as a platform independent way of initializing the os socket layer, if
	 *	you do not wish to use the full IpManager (as the IpManager constructor calls InitializeSocketLayer anyway).
	 *	Under Windows, this initializes winsock.
	 *
	 *	@throw	Exception.
	 */
	static void InitializeSocketLayer();

	/**
	 *	@brief	Cleanup the socket layer of the os.
	 *
	 *	This call is provided as a platform independent way of cleaning up the os socket layer, if
	 *	you do not wish to use the full IpManager (as the IpManager destructor calls CleanupSocketLayer anyway).
	 */
	static void CleanupSocketLayer();

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

protected:
	using ThreadManager::OnThreadStart;
	using ThreadManager::OnThreadStop;

private:
	/**
	 *	@brief	Create event manager.
	 *
	 *	Will try epoll on linux, kevent on bsd and fallback to poll if not available.
	 *
	 *	@return	EventManager pointer.
	 */
	static EventManager *CreateEventManager();
	
	/**
	 *	@brief	Shutdown io threads.
	 *
	 *	@param	shutdownTimeout how long to wait for the threads to die, in ms.
	 */
	void ShutdownIoThreads(const TimeoutPeriod shutdownTimeout);

	/**
	 *	@brief	Static io worker thread entry point.
	 *
	 *	@param	p pointer back to the owning manager class.
	 */
	static void IoThread(void *p);

	/**
	 *	@brief	Io worker thread entry point.
	 */
	void IoThread();

	boost::scoped_ptr<EventManager>	m_pEventManager;
	size_t							m_maxIoVecs;				///< Max io vecs to send/recvmsg()
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

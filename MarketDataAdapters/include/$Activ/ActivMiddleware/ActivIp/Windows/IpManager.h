/**
 *	@file	Win32/IpManager.h
 *
 *	@brief	Header file for base ip manager class.
 *
 *	$Log: $
 */

#include "ActivMiddleware/ActivIp/External.h"
#include "ActivMiddleware/ActivIp/IpAddress.h"
#include "ActivMiddleware/ActivBase/ThreadManager.h"

#include <string>

namespace Activ
{

/**
 *	@brief	Ip manager base class.
 */
class IpManager : private ThreadManager
{
public:
	/**
	 *	@brief	Default constructor.
	 *
	 *	Note the constructor calls InitializeSocketLayer() so it isn't necessary for user code to do so.
	 *
	 *	@param	numberOfIoThreads the number of io threads to launch. If 0, let the object decide how many threads to launch.
	 *
	 *	@throw	Exception
	 */
	ACTIV_IP_API IpManager(const size_t numberOfIoThreads = 0);

	/**
	 *	@brief	Destructor.
	 *
	 *	Note the destructor calls CleanupSocketLayer() so it isn't necessary for user code to do so.
	 */
	ACTIV_IP_API ~IpManager();

	/**
	 *	@brief	Initialize the socket layer of the os.
	 *
	 *	This call is provided as a platform independent way of initializing the os socket layer, if
	 *	you do not wish to use the full IpManager (as the IpManager constructor calls InitializeSocketLayer anyway).
	 *	Under Windows, this initializes winsock.
	 *
	 *	@throw	Exception.
	 */
	ACTIV_IP_API static void InitializeSocketLayer();

	/**
	 *	@brief	Cleanup the socket layer of the os.
	 *
	 *	This call is provided as a platform independent way of cleaning up the os socket layer, if
	 *	you do not wish to use the full IpManager (as the IpManager destructor calls CleanupSocketLayer anyway).
	 */
	ACTIV_IP_API static void CleanupSocketLayer();

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
	 *	@brief	Attach a socket to the completion port.
	 *
	 *	@param	socket the socket to attach.
	 *
	 *	@throw	Exception
	 */
	ACTIV_IP_API void AttachSocketToCompletionPort(const SOCKET socket) const;

	/**
	 *	@brief	Post a completion to the completion port. See Win32 PostQueuedCompletionStatus().
	 *
	 *	@param	numberOfBytes.
	 *	@param	completionKey.
	 *	@param	pOverlapped.
	 *
	 *	@return	boolean.
	 */
	ACTIV_IP_API bool PostQueuedCompletion(DWORD numberOfBytes, ULONG_PTR completionKey, OVERLAPPED *pOverlapped);

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
	 *	@brief	Register that an object is being managed.
	 */
	void RegisterManagedObject();
	
	/**
	 *	@brief	Deregister that an object is no longer being managed.
	 */
	void DeregisterManagedObject();

protected:
	using ThreadManager::OnThreadStart;
	using ThreadManager::OnThreadStop;

private:
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

	size_t	m_wsaStartupCount;									///< How many successful WSAStartup() have been made
	size_t	m_numberOfIoThreads;								///< The number of io threads launched
	DWORD	m_concurrency;										///< The number of processors in the system
	HANDLE	m_completionPort;									///< Io completion port
	long	m_numberOfObjects;									///< The number of objects the manager is managing
};

// ---------------------------------------------------------------------------------------------------------------------------------

inline void IpManager::RegisterManagedObject()
{
	::InterlockedIncrement(&m_numberOfObjects);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline void IpManager::DeregisterManagedObject()
{
	::InterlockedDecrement(&m_numberOfObjects);
}

// ---------------------------------------------------------------------------------------------------------------------------------

} // namespace Activ

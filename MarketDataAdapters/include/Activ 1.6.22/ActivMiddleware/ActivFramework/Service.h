/**
 *	@file	Service.h
 *
 *	@brief	Header file for service class.
 *
 *  $Log: $
 */

#if !defined (ACTIV_SERVICE_H)
#define ACTIV_SERVICE_H

#include "ActivMiddleware/ActivFramework/External.h"
#include "ActivMiddleware/ActivFramework/Component.h"
#include "ActivMiddleware/ActivFramework/Messages.h"
#include "ActivMiddleware/ActivBase/Uuid.h"

#include <list>

namespace Activ
{

/**
 *	@brief	Service class.
 */
class Service : public Component
{
public:
	/**
	 *	@brief	An empty access point list for a service.
	 */
	ACTIV_FRAMEWORK_API static const ServiceAccessPointList emptyServiceAccessPointList;

	/**
	 *	@brief	Constructor.
	 *
	 *	Attaches this service to a managed endpoint at the next available slot. The service will get allocated
	 *	a uuid.
	 *
	 *	@param	application Application object to attach to.
	 *	@param	managedEndPoint reference to a ManagedEndPoint object to which this port is to be attached.
	 *	@param	name name of the service.
	 *	@param	serviceInstance service id and access points for this service.
	 *	@param	pHeap heap to use for messaging, or 0 for the system heap.
	 *
	 *	@throw	Exception.
	 */
	ACTIV_FRAMEWORK_API Service(Application &application, ManagedEndPoint &managedEndPoint, const std::string &name,
								const ServiceInstance &serviceInstance, IHeap * const pHeap = 0);

	/**
	 *	@brief	Destructor.
	 */
	ACTIV_FRAMEWORK_API ~Service();

	/**
	 *	@brief	Get the service instance info.
	 *
	 *	@return	Reference to the service instance info.
	 */
	ACTIV_FRAMEWORK_API const ServiceInstance &GetServiceInstance() const;

	/**
	 *	@brief	Register the service with any configured directory services.
	 *
	 *	@retval	STATUS_CODE_SUCCESS.
	 *	@retval	...
	 */
	ACTIV_FRAMEWORK_API virtual StatusCode Register();

	/**
	 *	@brief	Deregister the service from any directory services.
	 *
	 *	@retval	STATUS_CODE_SUCCESS.
	 *	@retval	...
	 */
	ACTIV_FRAMEWORK_API virtual StatusCode Deregister();

	/**
	 *	@brief	Close all access points and accepted connections.
	 */
	ACTIV_FRAMEWORK_API void Close();

protected:
	/**
	 *	@brief	A client is attempting to connect.
	 *
	 *	The default behaviour is to call ProcessOpenServiceRequest(). See Component::OnAccept() for more information.
	 *
	 *	@param	acceptMessage.
	 *
	 *	@return	Unused by the framework.
	 */
	ACTIV_FRAMEWORK_API virtual StatusCode OnAccept(HeapMessage &acceptMessage);

	/**
	 *	@brief	Process an open service request.
	 *
	 *	This method is protected to allow processing of extended open service requests. By default it validates
	 *	again the default open service request and builds a default open service response.
	 *
	 *	@param	acceptMessage as passed to OnAccept().
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
	ACTIV_FRAMEWORK_API virtual StatusCode ProcessOpenServiceRequest(HeapMessage &acceptMessage);

	/**
	 *	@brief	Validate a received open service message.
	 *
	 *	An open service message is contained in the body of the message passed to OnAccept().
	 *
	 *	@param	acceptMessage as passed to OnAccept().
	 *	@param	openServiceRequest message to deserialize into.
	 *
	 *	@retval	STATUS_CODE_SUCCESS.
	 *	@retval	...
	 */
	ACTIV_FRAMEWORK_API StatusCode ValidateOpenServiceRequest(const HeapMessage &acceptMessage, OpenServiceRequest &openServiceRequest) const;

	/**
	 *	@brief	Build a response to an open service request.
	 *
	 *	@param	acceptMessage as passed to OnAccept() and validated by ValidateOpenServiceRequest().
	 *	@param	openServiceResponse message to serialize as the response.
	 *	@param	openServiceVersion version of the open service message.
	 *
	 *	@retval	STATUS_CODE_SUCCESS.
	 *	@retval	...
	 */
	ACTIV_FRAMEWORK_API StatusCode BuildOpenServiceResponse(HeapMessage &acceptMessage, OpenServiceResponse &openServiceResponse, const byte_t openServiceVersion) const;

private:
	/**
	 *	@brief	List of servers.
	 */
	class AccessList
	{
	public:
		/** 
		 *	@brief	Constructor.
		 *
		 *	@param	service.
		 */
		AccessList(Service &service);

		/**
		 *	@brief	Destructor.
		 */
		~AccessList();

		/**
		 *	@brief	Add a server address to the list.
		 *
		 *	@param	serverAddress address of server to add.
		 */
		void Add(const Address &serverAddress);

		/**
		 *	@brief	Clear access list. Stop listening and close accepted connections.
		 */
		void Clear();

	private:
		typedef std::list<Address> ServerList;

		Service		&m_service;
		ServerList	m_serverList;
	};

	/**
	 *	@brief	Make ServiceInstance object with normalized access points.
	 *
	 *	@param	serviceInstance source.
	 *
	 *	@return	Normalized ServiceInstance.
	 */
	ServiceInstance MakeServiceInstance(const ServiceInstance &serviceInstance);

	AccessList				m_accessList;						///< list of servers this service is available on

	const ServiceInstance	m_serviceInstance;					///< service id, access points
	const Uuid				m_uuid;								///< unique id for this service
};

} // namespace Activ

#endif // !defined (ACTIV_SERVICE_H)

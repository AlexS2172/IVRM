/**
 *	@file	ServiceApi.h
 *
 *	@brief	Header file for service access api.
 *
 *  $Log: $
 */

#if !defined (ACTIV_FRAMEWORK_SERVICE_API_H)
#define ACTIV_FRAMEWORK_SERVICE_API_H

#include "ActivMiddleware/ActivFramework/External.h"
#include "ActivMiddleware/ActivFramework/Messages.h"

namespace Activ
{

class Component;

/**
 *	@brief	Api to the access services.
 */
class ServiceApi
{
public:
	/**
	 *	@brief	Find all instances of a service by service id and return all access points.
	 *
	 *	@param	component the component doing the find.
	 *	@param	serviceId id of the service to find.
	 *	@param	agentScope scope of the services to find (within this agent space or globally).
	 *	@param	attributeQuery an LDAP style attribute query. Pass empty string to not filter by attributes.
	 *	@param	serviceInstanceList to receive the results of the find.
	 *	@param	timeout how long to wait for the response, in ms.
	 *
	 *	@retval	STATUS_CODE_SUCCESS.
	 *	@retval	STATUS_CODE_NOT_FOUND.
	 *	@retval	...
	 */
	ACTIV_FRAMEWORK_API static StatusCode FindServices(const Component &component, const ServiceId &serviceId, const AgentScope agentScope,
													   const std::string &attributeQuery, ServiceInstanceList &serviceInstanceList,
													   const TimeoutPeriod timeout = Port::defaultTimeout);

	/**
	 *	@brief	Open a service.
	 *
	 *	The m_serviceId member must contain the id of the service you wish to open and the m_url member must
	 *	contain the access point of the service you wish to use.
	 *
	 *	On a successful return, the m_serviceAddress member will contain the proxied address for communction with the
	 *	service.
	 *
	 *	If doing an asynchronous open (with timeout == 0), when the connection is established (OnConnect() invoked
	 *	with circuitAddress == m_serviceAddress of \a serviceHandle), ValidateOpenServiceResponse() should be called.
	 *	This verifies the service connected to is of the right type.
	 *
	 *	@param	component reference to Component wishing to open service.
	 *	@param	serviceHandle reference to a ServiceHandle object.
	 *	@param	timeout how long to wait for the connection to be established, in ms.
	 *	@param	pOpenServiceRequest optional user data to send in the open service message.
	 *	@param	pOpenServiceResponse optional data to receive from a sync response.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
	ACTIV_FRAMEWORK_API static StatusCode OpenService(Component &component, ServiceHandle &serviceHandle,
													  const TimeoutPeriod timeout = Port::defaultTimeout,
													  OpenServiceRequest * const pOpenServiceRequest = 0,
													  OpenServiceResponse * const pOpenServiceResponse = 0);

	/**
	 *	@brief	Close a service.
	 *
	 *	The close is handled asynchronously, as there may be data queued up on the endpoint from the service.
	 *	At some point an OnDisconnect() callback will be invoked with circuitAddress == serviceHandle.m_serviceAddress.
	 *
	 *	@param	component reference to Component that opened the service.
	 *	@param	serviceHandle reference to a ServiceHandle object as filled in by a successful OpenService().
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_NOT_CONNECTED
	 *	@retval	...
	 */
	ACTIV_FRAMEWORK_API static StatusCode CloseService(Component &component, ServiceHandle &serviceHandle);

	/**
	 *	@brief	Validate an open service response.
	 *
	 *	@param	connectMessage as passed to Component::OnConnect().
	 *	@param	openServiceResponse to deserialize into.
	 *
	 *	@retval	STATUS_CODE_SUCCESS.
	 *	@retval	...
	 */
	ACTIV_FRAMEWORK_API static StatusCode ValidateOpenServiceResponse(const HeapMessage &connectMessage, OpenServiceResponse &openServiceResponse);

private:
	/**
	 *	@brief	Initiate an asynchronous open service.
	 *
	 *	@param	component reference to Component wishing to open service.
	 *	@param	serviceHandle the service to open.
	 *	@param	pOpenServiceRequest open service request data.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
	static StatusCode OpenServiceAsync(Component &component, ServiceHandle &serviceHandle, const OpenServiceRequest * const pOpenServiceRequest);

	/**
	 *	@brief	Synchronously open a service.
	 *
	 *	@param	component reference to Component wishing to open service.
	 *	@param	serviceHandle the service to open.
	 *	@param	pOpenServiceRequest open service request data.
	 *	@param	pOpenServiceResponse to receive open response data.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
	static StatusCode OpenServiceSync(Component &component, ServiceHandle &serviceHandle, const TimeoutPeriod timeout, const OpenServiceRequest * const pOpenServiceRequest, OpenServiceResponse * const pOpenServiceResponse);

	/**
	 *	@brief	Attempt to open a service the router couldn't.
	 *
	 *	@param	component reference to Component wishing to open service.
	 *	@param	serviceHandle the service to open.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
	static StatusCode InternalOpenService(Component &component, ServiceHandle &serviceHandle);
};

} // namespace Activ

#endif // !defined (ACTIV_FRAMEWORK_SERVICE_API_H)

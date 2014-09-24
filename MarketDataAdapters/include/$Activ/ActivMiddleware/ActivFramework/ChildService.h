/**
 *	@file	Service.h
 *
 *	@brief	Header file for service class.
 *
 *  $Log: $
 */

#if !defined (ACTIV_CHILD_SERVICE_H)
#define ACTIV_CHILD_SERVICE_H

#include "ActivMiddleware/ActivFramework/External.h"
#include "ActivMiddleware/ActivFramework/Service.h"

namespace Activ
{

/**
 *	@brief	Child service class
 *
 *	A child service is attached to another service. The child service is given the same service id as the parent.
 */
template <typename T>
class ChildService : public Service
{
public:
	/**
	 *	@brief	Constructor.
	 *
	 *	Attaches the child service to a managed endpoint at the next available slot with the service of the parent service.
	 *
	 *	@param	parentService reference to the parent service.
	 *	@param	managedEndPoint reference to a ManagedEndPoint object to which this port is to be attached.
	 *	@param	pHeap heap to use for messaging, or 0 for the parent service heap.
	 *
	 *	@throw	Exception.
	 */
	ChildService(T &parentService, ManagedEndPoint &managedEndPoint, IHeap * const pHeap = 0);

	T	&m_parentService;										///< Reference back to the parent service
};

// ---------------------------------------------------------------------------------------------------------------------------------

template <typename T>
ChildService<T>::ChildService(T &parentService, ManagedEndPoint &managedEndPoint, IHeap * const pHeap) :
	Service(parentService.GetApplication(), managedEndPoint, Port::anonymousName, ServiceInstance(parentService.GetServiceInstance().m_serviceId, Service::emptyServiceAccessPointList, ""), (0 == pHeap) ? parentService.GetHeap() : pHeap),
	m_parentService(parentService)
{
}

} // namespace Activ

#endif // !defined (ACTIV_CHILD_SERVICE_H)

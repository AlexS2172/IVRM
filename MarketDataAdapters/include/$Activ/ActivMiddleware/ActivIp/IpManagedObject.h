/**
 *	@file	IpManagedObject.h
 *
 *	@brief	Header file for managed object class.
 *
 *	$Log: $
 */

#if !defined (ACTIV_IP_MANAGED_OBJECT_H)
#define ACTIV_IP_MANAGED_OBJECT_H

#include "ActivMiddleware/ActivIp/External.h"
#include "ActivMiddleware/ActivIp/IpManager.h"

namespace Activ
{

/**
 *	@brief	Ip managed object class.
 */
class ACTIV_IP_API IpManagedObject
{
public:
	/**
	 *	@brief	Constructor.
	 *
	 *	@param	manager Reference to the IpManager object.
	 */
	IpManagedObject(IpManager &manager);

	/**
	 *	@brief	Destructor.
	 */
	~IpManagedObject();

	/**
	 *	@brief	Get a reference to the manager.
	 */
	IpManager &GetManager();

private:
	IpManager	&m_manager;					///< The manager object
};

// ---------------------------------------------------------------------------------------------------------------------------------

inline IpManagedObject::IpManagedObject(IpManager &manager) :
	m_manager(manager)
{
	m_manager.RegisterManagedObject();
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline IpManagedObject::~IpManagedObject()
{
	m_manager.DeregisterManagedObject();
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline IpManager &IpManagedObject::GetManager()
{
	return m_manager;
}

} // namespace Activ

#endif // !defined (ACTIV_IP_MANAGED_OBJECT_H)

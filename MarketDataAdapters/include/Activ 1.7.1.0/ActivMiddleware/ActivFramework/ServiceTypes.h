/**
 *	@file	ServiceTypes.h
 *
 *	@brief	Header file for various service related classes.
 *
 *  $Log: $
 */

#if !defined (ACTIV_SERVICE_TYPES_H)
#define ACTIV_SERVICE_TYPES_H

#include "ActivMiddleware/ActivFramework/External.h"
#include "ActivMiddleware/ActivBase/Uuid.h"
#include "ActivMiddleware/Misc/SerializableContainerHelper.h"

namespace Activ
{

/**
 *	@brief	List of MESSAGE_CLASS_SERVICE message types.
 */
ACTIV_DECLARE_MESSAGE_TYPE(0,	MESSAGE_CLASS_SERVICE,	SERVICE_MESSAGE_REGISTER);
ACTIV_DECLARE_MESSAGE_TYPE(1,	MESSAGE_CLASS_SERVICE,	SERVICE_MESSAGE_DEREGISTER);

// ---------------------------------------------------------------------------------------------------------------------------------

/**
 *	@brief	Service access point class.
 */
class ServiceAccessPoint
{
public:
	/**
	 *	@brief	Default constructor.
	 */
	ACTIV_FRAMEWORK_API ServiceAccessPoint();

	/**
	 *	@brief	Constructor.
	 *
	 *	@param	url url string.
	 *	@param	agentScope.
	 */
	ACTIV_FRAMEWORK_API ServiceAccessPoint(const std::string &url, const AgentScope agentScope);

	/**
	 *	@brief	Read service access point from an ini file key.
	 *
	 *	Key is of the form url = scope:url
	 *	or url = url
	 *
	 *	E.g.  url = local:ams://:9005
	 *	      url = global:ams://:9006
	 *	      url = ams://:9007
	 *
	 *	@param	keyIterator iterator to ini file key.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
	ACTIV_FRAMEWORK_API StatusCode Read(const IniFileParser::ConstKeyIterator &keyIterator);

	/**
	 *	@brief	Clear members in access point.
	 */
	ACTIV_FRAMEWORK_API void Clear();

	/**
	 *	@brief	Convert to a string for display purposes.
	 *
	 *	@return	String.
	 */
	ACTIV_FRAMEWORK_API std::string ToString() const;

	std::string	m_url;
	AgentScope	m_agentScope;
};

// ---------------------------------------------------------------------------------------------------------------------------------

/**
 *	@brief	List of service access points.
 */
typedef std::vector<ServiceAccessPoint> ServiceAccessPointList;

/**
 *	@brief	Read service access point list from an ini file section.
 *
 *	Note the ServiceAccessPointList is NOT cleared initially, and ServiceAccessPoints are only append to this
 *	object if the whole section is parsed successfully.
 *
 *	@param	sectionIterator iterator to an ini file section.
 *	@param	serviceAccessPointList list to populate.
 *
 *	@retval	STATUS_CODE_SUCCESS.
 *	@retval	...
 */
ACTIV_FRAMEWORK_API StatusCode ReadServiceAccessPointList(const IniFileParser::ConstSectionIterator &sectionIterator, ServiceAccessPointList &serviceAccessPointList);

// ---------------------------------------------------------------------------------------------------------------------------------

/**
 *	@brief	Service id class.
 *
 *	ServiceId uniquely identifies a particular type of service.
 */
class ServiceId
{
public:
	class Serializer;											///< Helper class to (de)serialize

	/**
	 *	@brief	Default constructor.
	 */
	ACTIV_FRAMEWORK_API ServiceId();

	/**
	 *	@brief	Constructor.
	 *
	 *	@param	id service id in c-string form.
	 */
	ACTIV_FRAMEWORK_API ServiceId(const char * const id);

	/**
	 *	@brief	Constructor.
	 *
	 *	@param	id service id in string form.
	 */
	ACTIV_FRAMEWORK_API ServiceId(const std::string &id);
	
	/**
	 *	@brief	Set the service id.
	 *
	 *	@param	id service id in string form.
	 */
	ACTIV_FRAMEWORK_API void Set(const std::string &id);

	/**
	 *	@brief	Get service id in string form.
	 */
	ACTIV_FRAMEWORK_API std::string ToString() const;

	/**
	 *	@brief	Clear down.
	 */
	ACTIV_FRAMEWORK_API void Clear();

	/**
	 *	@brief	Equality operator.
	 */
	ACTIV_FRAMEWORK_API bool operator==(const ServiceId &rhs) const;

	/**
	 *	@brief	Inequality operator.
	 */
	ACTIV_FRAMEWORK_API bool operator!=(const ServiceId &rhs) const;

	/**
	 *	@brief	Less than operator.
	 */
	ACTIV_FRAMEWORK_API bool operator<(const ServiceId &rhs) const;

private:
	std::string	m_id;
};

// ---------------------------------------------------------------------------------------------------------------------------------

/**
 *	@brief	Class representing an instance of a service and its locations.
 */
class ServiceInstance
{
public:
	/**
	 *	@brief	Default constructor.
	 */
	ACTIV_FRAMEWORK_API ServiceInstance();

	/**
	 *	@brief	Constructor.
	 *
	 *	@param	serviceId.
	 *	@param	serviceAccessPointList.
	 *	@param	attributes.
	 */
	ACTIV_FRAMEWORK_API ServiceInstance(const ServiceId &serviceId, const ServiceAccessPointList &serviceAccessPointList, const std::string &attributes);

	/**
	 *	@brief	Clear down.
	 */
	ACTIV_FRAMEWORK_API void Clear();

	ServiceId				m_serviceId;						///< Type of the service
	ServiceAccessPointList	m_serviceAccessPointList;			///< All access points for this service
	std::string				m_attributes;						///< Attributes for this service
};

// ---------------------------------------------------------------------------------------------------------------------------------

/**
 *	@brief	List of service instances.
 */
typedef std::vector<ServiceInstance> ServiceInstanceList;

// ---------------------------------------------------------------------------------------------------------------------------------

/**
 *	@brief	Parameters to OpenService() apis.
 */
class ServiceHandle
{
public:
	/**
	 *	@brief	Default constructor.
	 */
	ACTIV_FRAMEWORK_API ServiceHandle();

	/**
	 *	@brief	Constructor.
	 *
	 *	@param	serviceId id for the service.
	 *	@param	url.
	 */
	ACTIV_FRAMEWORK_API ServiceHandle(const ServiceId &serviceId, const std::string &url);

	/**
	 *	@brief	Clear down.
	 */
	ACTIV_FRAMEWORK_API void Clear();

	ServiceId	m_serviceId;									///< Id of the service
	std::string	m_url;											///< Url to the service
	Address		m_serviceAddress;								///< Local proxied address of the service
};

// ---------------------------------------------------------------------------------------------------------------------------------

/**
 *	@brief	Service subject type. By subscribing to this subject, a component can receive
 *			notifications of service registration / deregistration.
 */
class ServiceSubject : public Subject
{
public:
	/**
	 *	@brief	Constructor.
	 */
	ACTIV_FRAMEWORK_API ServiceSubject();
};

} // namespace Activ

#endif // !defined (ACTIV_SERVICE_TYPES_H)

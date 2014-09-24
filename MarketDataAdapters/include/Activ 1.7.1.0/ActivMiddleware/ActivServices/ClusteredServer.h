/**
 *	@file	ClusteredServer.h
 *
 *	@brief	Header file for the clustered server class.
 *
 *	$Log: $
 */

#if !defined (ACTIV_SERVICES_CLUSTERED_SERVER_H)
#define ACTIV_SERVICES_CLUSTERED_SERVER_H

#include "ActivMiddleware/ActivServices/External.h"

// ActivBase includes
#include "ActivMiddleware/ActivBase/IniFileParser.h"

// ActivFramework includes
#include "ActivMiddleware/ActivFramework/Application.h"

// ActivSystem includes
#include "ActivMiddleware/ActivSystem/IHeap.h"

#include "boost/scoped_ptr.hpp"

#include <string>

namespace Activ
{

/**
 *	@brief	Clustered server class.
 */
class ClusteredServer
{
public:
	/**
	 *	@brief	State enumeration.
	 */
	enum State
	{
		STANDBY,
		LIVE
	};

	// Forward declaration of the failover manager implementation class.
	class FailoverManagerImpl;

	/**
	 *	@brief	Failover manager class.
	 */
	class FailoverManager
	{
	public:
		typedef std::string ServerId;							///< Server id type.

		/**
		 *	@brief	Settings class.
		 */
		class Settings
		{
		public:
			/**
			 *	@brief	Constructor.
			 */
			ACTIV_SERVICES_API Settings();

			/**
			 *	@brief	Read settings from an ini file section.
			 *
			 *	@param	sectionIterator iterator to an ini file section.
			 *
			 *	@retval	STATUS_CODE_SUCCESS
			 *	@retval	...
			 */
			ACTIV_SERVICES_API StatusCode Read(const IniFileParser::ConstSectionIterator &sectionIterator);

			/**
			 *	@brief	Get settings from an ini file.
			 *
			 *	@param	iniFileName the name of the ini file.
			 *	@param	sectionName the section name within the ini file.
			 *
			 *	@return	a copy of a settings object.
			 *
			 *	@throw	StatusCodeException
			 */
			ACTIV_SERVICES_API static Settings GetFromIniFile(const std::string &iniFileName, const std::string &sectionName);

			std::string				m_name;								///< The name of the component.
			ServerId				m_serverId;							///< The id of the server.
			std::string				m_initialState;						///< The initial state.
			uint32_t				m_heartbeatInterval;				///< The interval in ms between sending heartbeats.
			uint32_t				m_heartbeatTimeout;					///< The timeout in ms after a heartbeat that a server is considered to have stopped sending them.
			uint32_t				m_connectionTimerInterval;			///< The interval between attempts to the create udp socket.
			std::string				m_rxInterfaceAddress;				///< The rx interface address of the udp socket.
			std::string				m_txInterfaceAddress;				///< The tx interface address of the udp socket.
			std::string				m_multicastGroup;					///< The multicast group to join.
			std::string				m_targetAddress;					///< The target address, used if multicast group is empty.
			uint16_t				m_port;								///< The port to listen/receive on.
			uint16_t				m_maximumTransmissionUnitSize;		///< The mtu size of the udp socket.
			int32_t					m_multicastTtl;						///< The multicast ttl.
			bool					m_isMulticastLoopbackEnabled;		///< Is multicast loopback enabled?
			int32_t					m_rxBufferSize;						///< The rx buffer size of the udp socket.
			int32_t					m_txBufferSize;						///< The tx buffer size of the udp socket.
			IHeap::BucketInfoVector	m_bucketInfoVector;					///< The heap bucket info vector.
		};

		/**
		 *	@brief	Constructor.
		 *
		 *	@param	application the application to attach to.
		 *	@param	managedEndPoint the managed end point to attach to.
		 *	@param	clusteredServer the clustered server to manage.
		 *	@param	settings the object's settings.
		 */
		ACTIV_SERVICES_API FailoverManager(Application &application, ManagedEndPoint &managedEndPoint, 
										   ClusteredServer &clusteredServer, const Settings &settings);

		/**
		 *	@brief	Destructor.
		 */
		ACTIV_SERVICES_API ~FailoverManager();

	private:
		boost::scoped_ptr<FailoverManagerImpl>	m_pFailoverManagerImpl;	///< The failover manager implementation.
	};

	/**
	 *	@brief	Constructor.
	 */
	ACTIV_SERVICES_API ClusteredServer();

	/**
	 *	@brief	Destructor.
	 */
	ACTIV_SERVICES_API virtual ~ClusteredServer();

	/**
	 *	@brief	On state change callback.
	 *
	 *	Default behaviour is to do nothing, so no need to call parent implementation.
	 *
	 *	@param	previousState the previous state.
	 */
	ACTIV_SERVICES_API virtual void OnStateChange(const State previousState);

	/**
	 *	@brief	Get state.
	 */
	State GetState() const;

	/**
	 *	@brief	Get state string.
	 */
	ACTIV_SERVICES_API const std::string &GetStateString() const;

private:
	/**
	 *	@brief	Set state.
	 *
	 *	@param	state the state to change to.
	 */
	void SetState(const State state);

	State	m_state;											///< The server's state.
};

// ---------------------------------------------------------------------------------------------------------------------------------

inline ClusteredServer::State ClusteredServer::GetState() const
{
	return m_state;
}

} // namespace Activ

#endif // !defined (ACTIV_SERVICES_CLUSTERED_SERVER_H)

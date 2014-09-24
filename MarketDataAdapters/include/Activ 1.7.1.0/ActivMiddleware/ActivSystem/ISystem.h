/**
 *	@file	ISystem.h
 *
 *	@brief	Header file for abstract system class.
 *
 *	$Log: $
 */

#if !defined (ACTIV_ISYSTEM_H)
#define ACTIV_ISYSTEM_H

#include "ActivMiddleware/ActivSystem/External.h"
#include "ActivMiddleware/ActivSystem/IProcess.h"
#include "ActivMiddleware/ActivSystem/IEndPoint.h"
#include "ActivMiddleware/ActivSystem/IHeap.h"
#include "ActivMiddleware/ActivBase/Timeout.h"
#include "ActivMiddleware/ActivBase/SystemInfo.h"
#include "ActivMiddleware/ActivBase/IniFileParser.h"
#include "ActivMiddleware/ActivFieldTypes/DateTime.h"

namespace Activ
{

/**
 *	@brief	Abstract system interface class.
 *
 *  Provides functionality to create new or open existing endpoints and heaps.
 */
class ISystem
{
public:

	/**
	 *	@brief	Settings.
	 */
	class Settings
	{
	public:
		enum MemoryModel
		{
			MEMORY_MODEL_SHARED,								///< System exists in shared memory
			MEMORY_MODEL_PRIVATE								///< System exists in private memory
		};

		enum SystemLogMode
		{
			SYSTEM_LOG_MODE_OFF,								///< System log off for this process
			SYSTEM_LOG_MODE_TO_CONSOLE,							///< System log goes to local console for this process
			SYSTEM_LOG_MODE_TO_AGENT							///< System log messages published to agent for this process
		};

		/**
		 *	@brief	Default constructor.
		 */
		ACTIV_SYSTEM_API Settings();

		/**
		 *	@brief	Read settings from an ini file section.
		 *
		 *	@param	sectionIterator iterator to an ini file section.
		 *
		 *	@retval	STATUS_CODE_SUCCESS
		 *	@retval	...
		 */
		ACTIV_SYSTEM_API StatusCode Read(const IniFileParser::ConstSectionIterator &sectionIterator);

		std::string		m_systemName;							///< Name of the system to create / open (default "System")
		MemoryModel		m_memoryModel;							///< Shared or private memory system (default shared)
		std::string		m_processVersion;						///< Process version string
		bool			m_setConsoleTitle;						///< Set console title (default false)
		SystemLogMode	m_systemLogMode;						///< System log mode for this process (default to agent)
		uint32_t		m_smallBlockHeapThreshold;				///< Windows only: see _set_sbh_threshold() (default is 0, off)
		uint32_t		m_minimumTimerResolution;				///< Windows only: see timeBeginPeriod() (default is 0, don't call timeBeginPeriod)

	protected:
		ACTIV_SYSTEM_API static const std::string sectionName;	///< All settings are in this section
	};

	/**
	 *	@brief	System information.
	 */
	class SystemInfo : public Activ::SystemInfo
	{
	public:
		/**
		 *	@brief	Default constructor.
		 */
		ACTIV_SYSTEM_API SystemInfo();

		/**
		 *	@brief	Serialize this SystemInfo to the latest version.
		 *
		 *	@param	messageBuilder to serialize into.
		 *
		 *	@return	STATUS_CODE_SUCCESS or error code.
		 */
		ACTIV_SYSTEM_API StatusCode Serialize(MessageBuilder &messageBuilder) const;

		/**
		 *	@brief	Serialize this SystemInfo to the version of the SystemInfo parameter.
		 *
		 *	@param	messageBuilder to serialize into.
		 *	@param	systemInfo containing the version to serialize to.
		 *
		 *	@return	STATUS_CODE_SUCCESS or error code.
		 */
		ACTIV_SYSTEM_API StatusCode Serialize(MessageBuilder &messageBuilder, const SystemInfo &systemInfo) const;

		/**
		 *	@brief	Deserialize system info.
		 *
		 *	@param	messageValidator to deserialize from.
		 *
		 *	@return	STATUS_CODE_SUCCESS or error code.
		 */
		ACTIV_SYSTEM_API StatusCode Deserialize(MessageValidator &messageValidator);

		std::string				m_systemName;					///< System name
		Settings::MemoryModel	m_memoryModel;					///< Shared or private memory system (default shared)
		DateTime				m_utcStartupDateTime;			///< System startup time, utc
		size_t					m_upTimeSeconds;				///< System uptime in seconds

	private:
		/**
		 *	@brief	Serialize this SystemInfo to a specific version.
		 *
		 *	@param	messageBuilder to serialize into.
		 *	@param	version containing the version to serialize to.
		 *
		 *	@return	STATUS_CODE_SUCCESS or error code.
		 */
		StatusCode Serialize(MessageBuilder &messageBuilder, const byte_t version) const;

		static const byte_t		currentVersion = 1;				///< Current version of serialized format

		byte_t					m_version;
	};

	/**
	 *	@brief	Process enumerator callback interface.
	 */
	class IProcessEnumerator
	{
	public:
		/**
		 *	@brief	Destructor.
		 */
		virtual ~IProcessEnumerator() = 0;

		/**
		 *	@brief	Callback on a process instance.
		 *
		 *	@param	pProcess pointer to an object providing an IProcess interface.
		 *
		 *	@retval	true to continue enumerating.
		 *	@retval	false to stop enumerating.
		 */
		virtual bool OnObject(IProcess * const pProcess) = 0;
	};

	/**
	 *	@brief	Endpoint enumerator callback interface.
	 */
	class IEndPointEnumerator
	{
	public:
		/**
		 *	@brief	Destructor.
		 */
		virtual ~IEndPointEnumerator() = 0;

		/**
		 *	@brief	Callback on an endpoint instance.
		 *
		 *	@param	pEndPoint pointer to an object providing an ISyncEndPoint interface.
		 *
		 *	@retval	true to continue enumerating.
		 *	@retval	false to stop enumerating.
		 */
		virtual bool OnObject(ISyncEndPoint * const pEndPoint) = 0;
	};

	/**
	 *	@brief	Heap enumerator callback interface.
	 */
	class IHeapEnumerator
	{
	public:
		/**
		 *	@brief	Destructor.
		 */
		virtual ~IHeapEnumerator() = 0;

		/**
		 *	@brief	Callback on a heap instance.
		 *
		 *	@param	pHeap pointer to an object providing an IHeap interface.
		 *
		 *	@retval	true to continue enumerating.
		 *	@retval	false to stop enumerating.
		 */
		virtual bool OnObject(IHeap * const pHeap) = 0;
	};

	/**
	 *	@brief	Destructor.
	 */
	virtual ~ISystem() = 0;

	/**
	 *	@brief	Create a private, local memory endpoint.
	 *
	 *	@param	hasMutex true if the endpoint is to be protected by a mutex, else false.
	 *	@param	notificationType primitive to use for data available notification on the endpoint.
	 *
	 *	@return	Pointer to an object providing an IEndPoint interface.
	 *
	 *	@throw	Exception
	 */
	virtual IEndPoint *CreatePrivateEndPoint(const bool hasMutex, const IEndPoint::NotificationType notificationType) = 0;

	/**
	 *	@brief	Create a system endpoint.
	 *
	 *	The endpoint will exist in shared memory if the system was created with MEMORY_MODEL_SHARED and will be
	 *	available to other processes in the system. Otherwise it will be created in local memory and can only be
	 *	opened by this process (using OpenEndPoint()).
	 *
	 *	@param	name the name of the endpoint which will exist within the parent system namespace.
	 *			If name == "", the endpoint is given a name based on a unique id.
	 *	@param	hasMutex true if the endpoint is to be protected by a mutex, else false.
	 *	@param	notificationType primitive to use for data available notification on the endpoint.
	 *
	 *	@return	Pointer to an object providing an ISyncEndPoint interface.
	 *
	 *	@throw	Exception
	 */
	virtual ISyncEndPoint *CreateEndPoint(const std::string &name, const bool hasMutex, const IEndPoint::NotificationType notificationType) = 0;

	/**
	 *	@brief	Open a system endpoint by name.
	 *
	 *	@param	name the name of the endpoint which exists within the parent system namespace.
	 *	@param	timeout a Timeout object specifying how long to wait for the endpoint to exist.
	 *
	 *	@return	Pointer to an object providing an ISyncEndPoint interface.
	 *
	 *	@throw	Exception
	 */
	virtual ISyncEndPoint *OpenEndPoint(const std::string &name, Timeout &timeout = Timeout::GetInfinite()) = 0;

	/**
	 *	@brief	Open a system endpoint by id.
	 *
	 *	@param	endPointId the system unique id of the endpoint to open.
	 *	@param	timeout a Timeout object specifying how long to wait for the endpoint to exist.
	 *
	 *	@return	Pointer to an object providing an ISyncEndPoint interface.
	 *
	 *	@throw	Exception
	 */
	virtual ISyncEndPoint *OpenEndPoint(EndPointId endPointId, Timeout &timeout = Timeout::GetInfinite()) = 0;

	/**
	 *	@brief	Create a named memory heap.
	 *
	 *	The heap will exist in shared memory if the system was created with MEMORY_MODEL_SHARED and will be
	 *	available to other processes in the system. Otherwise it will be created in local memory and can only be
	 *	opened by this process (using OpenHeap()).
	 *
	 *	@param	name the name of the heap which will exist within the parent system namespace.
	 *	@param	bucketInfoVector a vector of BucketInfo classes describing the layout of the heap. Only required
	 *			if the system was created with MEMORY_MODEL_SHARED, otherwise ignored.
	 *
	 *	@return	Pointer to an object providing an IHeap interface.
	 *
	 *	@throw	Exception
	 */
	virtual IHeap *CreateHeap(const std::string &name, const IHeap::BucketInfoVector &bucketInfoVector = IHeap::emptyBucketInfoVector) = 0;

	/**
	 *	@brief	Open an existing named memory heap.
	 *
	 *	@param	name the name of the heap which exists within the parent system namespace.
	 *	@param	timeout a Timeout object specifying how long to wait for the heap to exist.
	 *
	 *	@return	Pointer to an object providing an IHeap interface.
	 *
	 *	@throw	Exception
	 */
	virtual IHeap *OpenHeap(const std::string &name, Timeout &timeout = Timeout::GetInfinite()) = 0;

	/**
	 *	@brief	Open an existing named memory heap.
	 *
	 *	@param	heapId the system unique id of the heap to open.
	 *	@param	timeout a Timeout object specifying how long to wait for the heap to exist.
	 *
	 *	@return	Pointer to an object providing an IHeap interface.
	 *
	 *	@throw	Exception
	 */
	virtual IHeap *OpenHeap(const HeapId heapId, Timeout &timeout = Timeout::GetInfinite()) = 0;

	/**
	 *	@brief	Open an existing process within the shared memory system.
	 *
	 *	@param	name the name of the process.
	 *	@param	timeout a Timeout object specifying how long to wait for the process to exist, in ms.
	 *
	 *	@return	An object providing an IProcess interface.
	 *
	 *	@throw	Exception.
	 */
	virtual IProcess *OpenProcess(const std::string &name, Timeout &timeout = Timeout::GetInfinite()) = 0;

	/**
	 *	@brief	Open an existing process within the shared memory system.
	 *
	 *	@param	processId the id of the process within the system.
	 *	@param	timeout a Timeout object specifying how long to wait for the process to exist, in ms.
	 *
	 *	@return	An object providing an IProcess interface.
	 *
	 *	@throw	Exception.
	 */
	virtual IProcess *OpenProcess(const ProcessId processId, Timeout &timeout = Timeout::GetInfinite()) = 0;

	/**
	 *	@brief	Get the name of the system.
	 *
	 *	@return	A string object containing the name of the system.
	 */
	virtual std::string GetSystemName() const = 0;

	/**
	 *	@brief	Get the system heap.
	 *
	 *	@return	An object providing an IHeap interface.
	 */
	virtual IHeap *GetSystemHeap() const = 0;

	/**
	 *	@brief	Get this process.
	 *
	 *	@return	An IProcess interface to this process.
	 */
	virtual IProcess *GetProcess() const = 0;

	/**
	 *	@brief	Get system info.
	 *
	 *	@return	SystemInfo.
	 *
	 *	@throw	Exception.
	 */
	virtual SystemInfo GetSystemInfo() const = 0;

	/**
	 *	@brief	Enumerate all opened processes.
	 *
	 *	@param	enumerator reference to an object providing an IProcessEnumerator interface to
	 *			be called back on a process instance.
	 */
	virtual void EnumerateObjects(IProcessEnumerator &enumerator) const = 0;

	/**
	 *	@brief	Enumerate all opened endpoints.
	 *
	 *	@param	enumerator reference to an object providing an IEndPointEnumerator interface to
	 *			be called back on an endpoint instance.
	 */
	virtual void EnumerateObjects(IEndPointEnumerator &enumerator) const = 0;

	/**
	 *	@brief	Enumerate all opened heaps.
	 *
	 *	@param	enumerator reference to an object providing an IHeapEnumerator interface to
	 *			be called back on a process instance.
	 */
	virtual void EnumerateObjects(IHeapEnumerator &enumerator) const = 0;
};

// ---------------------------------------------------------------------------------------------------------------------------------

inline ISystem::~ISystem()
{
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline ISystem::IProcessEnumerator::~IProcessEnumerator()
{
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline ISystem::IEndPointEnumerator::~IEndPointEnumerator()
{
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline ISystem::IHeapEnumerator::~IHeapEnumerator()
{
}

} // namespace Activ

#endif // !defined (ACTIV_ISYSTEM_H)

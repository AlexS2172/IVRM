/**
 *	@file	IProcess.h
 *
 *	@brief	Header file for process interface class.
 *
 *  $Log: $
 */

#if !defined (ACTIV_SYSTEM_IPROCESS_H)
#define ACTIV_SYSTEM_IPROCESS_H

#include "ActivMiddleware/ActivSystem/External.h"
#include "ActivMiddleware/ActivMessageHandler/MessageValidater.h"
#include "ActivMiddleware/ActivMessageHandler/MessageBuilder.h"
#include "ActivMiddleware/ActivBase/Timeout.h"

#include <string>

/**
 *	@brief	List of process states.
 */
#define ACTIV_IPROCESS_STATE_LIST(d)				\
	d(STATE_INITIALIZING, "initializing")			\
	d(STATE_RUNNING, "running")						\
	d(STATE_SHUTTING_DOWN, "shutting down")			\
	d(STATE_EXITED, "exited")						\
	d(STATE_DEAD, "dead")

namespace Activ
{

class ISystem;

/**
 *	@brief	Process interface class.
 */
class IProcess
{
public:
	/**
	 *	@brief	Process states.
	 */
	enum State
	{
		ACTIV_IPROCESS_STATE_LIST(ACTIV_DECLARE_ELEMENT_1_WITH_COMMA)
		NUM_STATES
	};

	/**
	 *	@brief	Process stats.
	 */
	class Stats
	{
	public:
		/**
		 *	@brief	Serialize.
		 *
		 *	@param	messageBuilder.
		 *
		 * 	@retval	STATUS_CODE_SUCCESS
		 *	@retval	...
		 */
		ACTIV_SYSTEM_API StatusCode Serialize(MessageBuilder &messageBuilder) const;

		/**
		 *	@brief	Deserialize.
		 *
		 *	@param	messageValidater.
		 *
		 *	@retval	STATUS_CODE_SUCCESS
		 *	@retval	...
		 */
		ACTIV_SYSTEM_API StatusCode Deserialize(MessageValidater &messageValidater);

		ProcessId	m_id;										///< system wide id of the process
		OsProcessId	m_osId;										///< os wide id if the process
		std::string	m_name;										///< name of the process in the system
		std::string	m_version;									///< process version string
		State		m_state;									///< state of the process

	private:
		static const byte_t	currentSerializedVersion = 1;
	};

	/**
	 *	@brief	Destructor.
	 */
	virtual ~IProcess() = 0;

	/**
	 *	@brief	Get the system unique process id for this process.
	 *
	 *	@return	The system unique process id.
	 */
	virtual ProcessId GetId() const = 0;

	/**
	 *	@brief	Get the os process id for this process.
	 *
	 *	@return	The os process id.
	 */
	virtual OsProcessId GetOsId() const = 0;

	/**
	 *	@brief	Get the name of this process.
	 *
	 *	@return	The name of this process.
	 */
	virtual std::string GetName() const = 0;

	/**
	 *	@brief	Get the system that created the process.
	 *
	 *	@return	An ISystem interface.
	 */
	virtual ISystem *GetSystem() const = 0;

	/**
	 *	@brief	Get process stats.
	 *
	 *	@param	stats reference to a Stats object.
	 */
	virtual void GetStats(Stats &stats) const = 0;

	/**
	 *	@brief	Get the process state.
	 *
	 *	@return	The current state of the process.
	 */
	virtual State GetState() const = 0;

	/**
	 *	@brief	Set the state of the process.
	 *
	 *	@param	state new state.
	 */
	virtual void SetState(const State state) = 0;

	/**
	 *	@brief	Wait for the state of the process to be a given value.
	 *
	 *	@param	state to wait for.
	 *	@param	timeout how long to wait.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_TIMEOUT
	 */
	virtual StatusCode WaitForState(const State state, Timeout &timeout = Timeout::GetInfinite()) = 0;

	/**
	 *	@brief	Convert process state to string.
	 *
	 *	@param	state state to convert.
	 *
	 *	@return	string form of system state.
	 */
	ACTIV_SYSTEM_API static std::string StateToString(const State state);
};

// ---------------------------------------------------------------------------------------------------------------------------------

inline IProcess::~IProcess()
{
}

} // namespace Activ

#endif // !defined (ACTIV_SYSTEM_IPROCESS_H)

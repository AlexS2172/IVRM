/**
 *	@file	SystemFactory.h
 *
 *	@brief	Header file for system factory class.
 *
 *  $Log: $
 */

#if !defined (ACTIV_SYSTEM_FACTORY_H)
#define ACTIV_SYSTEM_FACTORY_H

#include "ActivMiddleware/ActivSystem/External.h"
#include "ActivMiddleware/ActivSystem/ISystem.h"

namespace Activ
{

/**
 *	@brief	System factory class.
 */
class SystemFactory
{
public:
	/**
	 *	@brief	Create a new shared memory system. The state of the system will be set to ISystem::STATE_INITIALIZING.
	 *			When all initialization is complete, call ISystem::SetState(ISystem::STATE_RUNNING) to allow processes
	 *			to open the system.
	 *
	 *	@param	settings settings for the system & process.
	 *	@param	bucketInfoVector bucket info for the system heap.
	 *
	 *	@return	Pointer to an object providing an ISystem interface
	 *
	 *	@throw	Exception
	 */
	ACTIV_SYSTEM_API ISystem *Create(const ISystem::Settings &settings, const IHeap::BucketInfoVector &bucketInfoVector);

	/**
	 *	@brief	Open an existing shared memory system. It is only possible to link to a system when the
	 *			system is in state ISystem::STATE_RUNNING.
	 *
	 *	@param	settings settings for the system & process.
	 *	@param	processName the name of the process with the system, empty string gives a name based on the process id.
	 *	@param	Timeout object specifying how long to wait for the system to exist.
	 *	
	 *	@return	Pointer to an object providing an ISystem interface
	 *
	 *	@throw	Exception
	 */
	ACTIV_SYSTEM_API ISystem *Open(const ISystem::Settings &settings, const std::string &processName, Timeout &timeout = Timeout::GetInfinite());

	/**
	 *	@brief	Get a reference to the singleton factory object.
	 */
	ACTIV_SYSTEM_API static SystemFactory &GetInstance();

private:
	/**
	 *	@brief	Destructor.
	 */
	~SystemFactory();

	static SystemFactory	m_factory;		///< Singleton system factory object.
};

} // namespace Activ

#endif // !defined (ACTIV_SYSTEM_FACTORY_H)

/**
 *	@file	Fiber.h
 *
 *	@brief	Header file for the fiber class.
 *
 *	$Log: $
 */

#if (!defined ACTIV_BASE_FIBER_H)
#define ACTIV_BASE_FIBER_H

#include "ActivMiddleware/ActivBase/External.h"

#if (defined ACTIV_POSIX_HAS_CONTEXT_API)
#include <ucontext.h>
#include "boost/scoped_array.hpp"
#elif (!defined ACTIV_PLATFORM_WINDOWS)
#include "ActivMiddleware/ActivBase/Thread.h"
#endif

namespace Activ
{

class IEvent;

/**
 *	@brief	Fiber class.
 */
class Fiber
{
public:
	static const size_t defaultStackSize = 102400;

    /**
     *	@brief	Constructor.
 	 *
	 *	@param	pData user data that will get passed to Process() when the fiber is initially switched to.
 	 *	@param	stackSize initial size of stack to commit.
	 *
	 *	@throw	Exception.
	 */
    ACTIV_BASE_API Fiber(void * const pData = 0, const size_t stackSize = defaultStackSize);

    /**
     *	@brief	Destructor.
     */
	ACTIV_BASE_API virtual ~Fiber();

	/**
	 *	@brief	Switch to this fiber.
	 */
	ACTIV_BASE_API void Switch();

	/**
	 *	@brief	Return to the calling fiber.
	 *
	 *	This call just returns if this object has not been Switch()ed to.
	 */
	ACTIV_BASE_API void Return();

protected:
	/**
     *	@brief	Actual fiber function (override this for your own processing).
	 *
	 *	@param	pData user data pointer.
     */
	virtual void Process(void * const pData) = 0;

private:
	enum State
	{
		STATE_NOT_ACTIVE,										///< fiber isn't running
		STATE_ACTIVE,											///< fiber has been Switch() to and is running
		STATE_DONE												///< Process() has exited
	};

	State			m_state;									///< current state of fiber

#if (defined ACTIV_PLATFORM_WINDOWS)

	typedef void *FiberId;
	static void CALLBACK StaticFiberProc(void *pData);
	
#elif (defined ACTIV_POSIX_HAS_CONTEXT_API)

	typedef ucontext_t FiberId;
	static void StaticFiberProc(const uint32_t highFiberAddress, const uint32_t lowFiberAddress);
	
	const size_t	m_stackSize;
	char * const	m_pStack;									///< stack for fiber

#else // fallback to a thread based implementation

	class FiberThread : public Thread
	{
	public:
		FiberThread(Fiber &fiber);
		
	private:
		virtual StatusCode Process(void *pData);
		
		Fiber   &m_fiber;
	};
	
	typedef IEvent *FiberId;

	FiberThread		m_fiberThread;

#endif

	void * const	m_pData;									///< user data
	FiberId			m_id;										///< this fiber id
	FiberId			m_returnId;									///< fiber to return to
};

} // namespace Activ

#endif // (!defined ACTIV_BASE_FIBER_H)

/**
 *	@file	IoCompletionManager.h
 *
 *	@brief	Header file for Windows I/O completion manager classes.
 *
 *	$Log: $
 */

#if (!defined ACTIV_BASE_IO_COMPLETION_MANAGER_H)
#define ACTIV_BASE_IO_COMPLETION_MANAGER_H

#include "ActivMiddleware/ActivBase/External.h"
#include "ActivMiddleware/ActivBase/ThreadManager.h"

namespace Activ
{

class IMutex;

/** 
 *	@brief	I/O completion manager class.
 */
class IoCompletionManager : private ThreadManager
{
public:
	/**
	 *	@brief	Overlapped class.
	 *
	 *	Extends the Windows OVERLAPPED structure and adds a virtual callback for i/o completions.
	 */
	class Overlapped : public OVERLAPPED
	{
	public:
		/**
		 *	@brief	Constructor.
		 *
		 *	@param	ioCompletionManager reference to manager.
		 *	@param	pMutex mutex to acquire before invoking OnIoCompletion() callback, or 0 if not required.
		 */
		Overlapped(IoCompletionManager &ioCompletionManager, IMutex * const pMutex);

		/**
		 *	@brief	Post an i/o completion to the completion port. See Windows PostQueuedCompletionStatus().
		 *
		 *	Causes OnIoCompletion() to be invoked from a manager thread.
		 *
		 *	@param	numberOfBytes.
		 *
		 *	@return	boolean.
		 */
		bool PostIoCompletion(DWORD numberOfBytes);

		/**
		 *	@brief	Check if i/o is pending for this object.
		 *
		 *	@return	boolean.
		 */
		bool IsPending() const;

		/**
		 *	@brief	Set the pending status for this object.
		 *
		 *	SetPending(true) after an i/o operation has successfully been initiated using this object.
		 *
		 *	@param	isPending.
		 */
		void SetPending(const bool isPending);

		/**
		 *	@brief	Check if i/o was successful.
		 *
		 *	Should be called when processing an OnIoCompletion() callback.
		 *
		 *	@return	boolean.
		 */
		bool WasSuccessfulCompletion() const;

		/**
		 *	@brief	Get number of bytes in completion.
		 *
		 *	Should be called when processing an OnIoCompletion() callback.
		 *
		 *	@return	Number of bytes transferred in i/o operation.
		 */
		DWORD GetNumberOfBytes() const;

	protected:
		/**
		 *	@brief	Overlapped i/o has completed.
		 *
		 *	Note if a mutex was provided to the constructor of the object, it will be acquired before
		 *	invoking this callback.
		 */
		virtual void OnIoCompletion() = 0;

	private:
		IoCompletionManager &	m_ioCompletionManager;			///< hook back to manager
		IMutex * const			m_pMutex;						///< mutex, or 0 if none

		bool					m_isPending;
		bool					m_wasSuccessfulCompletion;
		DWORD					m_numberOfBytes;

		friend class IoCompletionManager;						///< allow manager access
	};

	/**
	 *	@brief	Constructor.
	 *
	 *	@param	numberOfIoThreads the number of io threads to launch. If 0, let the object decide how many threads to launch.
	 *
	 *	@throw	Exception
	 */
	ACTIV_BASE_API IoCompletionManager(const size_t numberOfIoThreads = 0);

	/**
	 *	@brief	Destructor.
	 */
	ACTIV_BASE_API ~IoCompletionManager();

	/**
	 *	@brief	Attach a handle to the completion port.
	 *
	 *	@param	handle the handle to attach.
	 *
	 *	@throw	OsException.
	 */
	ACTIV_BASE_API void AttachHandle(const HANDLE handle) const;

protected:
	using ThreadManager::OnThreadStart;
	using ThreadManager::OnThreadStop;

private:
	/**
	 *	@brief	Static io worker thread entry point.
	 *
	 *	@param	p pointer back to the owning manager class.
	 */
	static void IoThread(void *p);

	/**
	 *	@brief	Io worker thread entry point.
	 */
	void IoThread();

	size_t	m_numberOfIoThreads;								///< number of io threads launched
	DWORD	m_concurrency;										///< number of processors in the system
	HANDLE	m_completionPort;									///< io completion port
};

// ---------------------------------------------------------------------------------------------------------------------------------

inline IoCompletionManager::Overlapped::Overlapped(IoCompletionManager &ioCompletionManager, IMutex * const pMutex) :
	m_ioCompletionManager(ioCompletionManager),
	m_pMutex(pMutex),
	m_isPending(false)
{
	OVERLAPPED *pOverlapped = static_cast<OVERLAPPED *>(this);
	
	::memset(pOverlapped, 0, sizeof(OVERLAPPED));
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline bool IoCompletionManager::Overlapped::PostIoCompletion(DWORD numberOfBytes)
{
	return (FALSE != ::PostQueuedCompletionStatus(m_ioCompletionManager.m_completionPort, numberOfBytes, 0, this));
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline bool IoCompletionManager::Overlapped::IsPending() const
{
	return m_isPending;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline void IoCompletionManager::Overlapped::SetPending(const bool isPending)
{
	m_isPending = isPending;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline bool IoCompletionManager::Overlapped::WasSuccessfulCompletion() const
{
	return m_wasSuccessfulCompletion;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline DWORD IoCompletionManager::Overlapped::GetNumberOfBytes() const
{
	return m_numberOfBytes;
}

} // namespace Activ

#endif // (!defined ACTIV_BASE_IO_COMPLETION_MANAGER_H)

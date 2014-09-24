/**
 *	@file	ThreadSpecificData.h
 *
 *	@brief	Header file for thread specific data storage class.
 *
 *	$Log: $
 */

#if (!defined ACTIV_BASE_THREAD_SPECIFIC_DATA_H)
#define ACTIV_BASE_THREAD_SPECIFIC_DATA_H

#include "ActivMiddleware/ActivBase/External.h"
#include "ActivMiddleware/Misc/FunctionObjects.h"

namespace Activ
{

/**
 *	@brief	Thread specific data helper base class.
 *
 *	Stores pointers of TYPE *.
 *
 *	This class makes use of the curiously-recurring template pattern to implement
 *	per-thread cleanup of data.
 *
 *	This class can be used as follows (for storing pointers of type MyData *):
 *
 *	class MyThreadSpecificData : public ThreadSpecificDataBase<MyData, MyThreadSpecificData>
 *	{
 *	public:
 *		void OnThreadExit(MyData * const pMyData)
 *		{
 *			// do whatever per-thread cleanup you require, e.g.
 *			delete pMyData;
 *		}
 *	};
 *
 *	The OnThreadExit() method of the derived class will be called whenever a thread that has
 *	a non-NULL value for the stored pointer exits.
 *
 *	Most likely the kind of cleanup to be carried out here will be deleting of newed per-thread data -
 *	see the ThreadSpecificData class defined below for such simple cases.
 *
 *	@param	TYPE the type to store in thread-local storage.
 *	@param	DERIVED the class deriving from this class.
 */
template <typename TYPE, typename DERIVED>
class ThreadSpecificDataBase
{
public:
	/**
	 *	@brief	Constructor. Allocate thread specific storage.
	 *
	 *	@throw	Exception.
	 */
	ThreadSpecificDataBase();

	/**
	 *	@brief	Destructor. Free the specific storage.
	 */
	~ThreadSpecificDataBase();
	
	/**
	 *	@brief	Get the current value of thread specific data for this thread.
	 *
	 *	@return	Current value (0 if not set yet).
	 */
	TYPE *Get() const;

	/**
	 *	@brief	Set the current value of thread specific data for this thread.
	 *
	 *	@param	pValue to set.
	 */
	void Set(TYPE * const pValue);
	
	/**
	 *	@brief	Method invoked whenever a thread exits for all non-NULL thread-specific datas.
	 *
	 *	The default implementation of this method does nothing.
	 *
	 *	@param	pValue current value for the exiting thread.
	 */
	void OnThreadExit(TYPE * const pValue);

private:
	/**
	 *	@brief	Data stored per-thread.
	 *
	 *	For cleanup purposes, we need to store a hook back to the ThreadSpecificData and the value itself.
	 */
	class PerThreadInfo
	{
	public:
		PerThreadInfo(ThreadSpecificDataBase<TYPE, DERIVED> &threadSpecificData) :
			m_threadSpecificData(threadSpecificData),
			m_pValue(0)
		{
		}
		
		ThreadSpecificDataBase<TYPE, DERIVED> &	m_threadSpecificData;
		TYPE *									m_pValue;
	};
	
	static void StaticDestructor(void *pData);		

#if (defined ACTIV_PLATFORM_WINDOWS)

	const DWORD		m_index;

#elif (defined ACTIV_PLATFORM_POSIX)
		
	pthread_key_t	m_key;
	
#else
#error Configure for this platform
#endif
};

// ---------------------------------------------------------------------------------------------------------------------------------

/**
 *	@brief	Thread specific data helper class.
 *
 *	Stores pointers of TYPE *. Invokes a function object for per-thread cleanup.
 *
 *	This class can be used as follows (for storing pointers of type MyData *) which deletes
 *	the objects on thread exit:
 *
 *	ThreadSpecificData<MyData, DeleteObject>	m_myThreadSpecificData;
 *
 *	Or to do nothing on thread exit:
 *
 *	ThreadSpecificData<MyData, NullObject>		m_myThreadSpecificData;
 *
 *	@param	TYPE the type to store in thread-local storage.
 *	@param	CLEANUP function object type to invoke.
 */
template <typename TYPE, typename CLEANUP>
class ThreadSpecificData : public ThreadSpecificDataBase<TYPE, ACTIV_TEMPLATE_SELF ThreadSpecificData<TYPE, CLEANUP> >
{
public:
	void OnThreadExit(TYPE * const pValue)
	{
		CLEANUP()(pValue);
	}
};

} // namespace Activ

#include "ActivMiddleware/ActivBase/Detail/ThreadSpecificDataDetail.h"

#endif // (!defined ACTIV_BASE_THREAD_SPECIFIC_DATA_H)

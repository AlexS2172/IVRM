/**
 *  @file	IntrusivePtrHelper.h
 *
 *  @brief	Helper for using intrusive pointer class in a single threaded environment.
 *
 *  $Log: $
 */

#if !defined (ACTIV_INTRUSIVE_PTR_HELPER_H)
#define ACTIV_INTRUSIVE_PTR_HELPER_H

#include "ActivMiddleware/Activ.h"

#include "boost/intrusive_ptr.hpp"

/**
 *	@brief	Kind of unique name for reference counter member.
 */
#define ACTIV_INTRUSIVE_PTR_HELPER_REFERENCE_COUNTER_NAME	____m_referenceCounter__activ____

// ---------------------------------------------------------------------------------------------------------------------------------

/**
 *	@brief	Global function to add ref to object.
 *
 *	@param	pObject to add reference to.
 */
template<typename T>
inline void intrusive_ptr_add_ref(const T * const pObject)
{
	++pObject->ACTIV_INTRUSIVE_PTR_HELPER_REFERENCE_COUNTER_NAME;
}

// ---------------------------------------------------------------------------------------------------------------------------------

/**
 *	@brief	Global function to release object.
 *
 *	@param	pObject to release.
 */
template<typename T>
inline void intrusive_ptr_release(const T * const pObject)
{
	if (0 == --pObject->ACTIV_INTRUSIVE_PTR_HELPER_REFERENCE_COUNTER_NAME)
		delete pObject;
}

// ---------------------------------------------------------------------------------------------------------------------------------

namespace Activ
{

/**
 *	@brief	Intrusive ptr reference counter.
 */
class ReferenceCounter
{
public:
	/**
	 *	@brief	Constructor.
	 */
	ReferenceCounter();

	/**
	 *	@brief	Increment the reference count.
	 *
	 *	@return	Resulting incremented value.
	 */
	uint32_t operator++();

	/**
	 *	@brief	Decrement the reference count.
	 *
	 *	@return	Resulting decremented value.
	 */
	uint32_t operator--();

	/**
	 *	@brief	Get the reference count value.
	 *
	 *	@return	Current reference count.
	 */
	uint32_t Get() const;

private:
	uint32_t	m_referenceCount;
};

// ---------------------------------------------------------------------------------------------------------------------------------

inline ReferenceCounter::ReferenceCounter() :
	m_referenceCount(0)
{
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline uint32_t ReferenceCounter::operator++()
{
	return ++m_referenceCount;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline uint32_t ReferenceCounter::operator--()
{
	return --m_referenceCount;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline uint32_t ReferenceCounter::Get() const
{
	return m_referenceCount;
}

// ---------------------------------------------------------------------------------------------------------------------------------

/**
 *	@brief	Macro to insert reference counter into an object.
 *
 *	This macro needs to be used in the public section of a class.
 */
#define ACTIV_INTRUSIVE_PTR_HELPER_ADD_REFERENCE_COUNTER								\
	mutable Activ::ReferenceCounter ACTIV_INTRUSIVE_PTR_HELPER_REFERENCE_COUNTER_NAME;	\
	Activ::uint32_t GetReferenceCount() const											\
	{																					\
		return ACTIV_INTRUSIVE_PTR_HELPER_REFERENCE_COUNTER_NAME.Get();					\
	}

// ---------------------------------------------------------------------------------------------------------------------------------

/**
 *	@brief	Declare typedef for intrusive ptr.
 *
 *	@param	T is the type to be pointed to.
 */
#define ACTIV_DECLARE_INTRUSIVE_PTR_TYPEDEF(T)	typedef boost::intrusive_ptr<T>

} // namespace Activ

#endif // !defined (ACTIV_INTRUSIVE_PTR_HELPER_H)

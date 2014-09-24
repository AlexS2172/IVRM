/**
 *	@file	HeapPool.h
 *
 *	@brief	Header file for heap pool class.
 *
 *  $Log: $
 */

#if !defined (ACTIV_SYSTEM_HEAP_POOL_H)
#define ACTIV_SYSTEM_HEAP_POOL_H

#include "ActivMiddleware/ActivSystem/External.h"
#include "ActivMiddleware/ActivSystem/IHeap.h"

#include <vector>

namespace Activ
{

/**
 *	@brief	Heap pool.
 */
class HeapPool
{
public:
	/**
	 *	@brief	Constructor.
	 *
	 *	@param	pSystem system to create heaps in.
	 *	@param	bucketInfoVector size of heaps to create.
	 *	@param	initialSize number of heaps to create initially.
	 *	@param	namePrefix prefix to give to heap names.
	 */
	ACTIV_SYSTEM_API HeapPool(ISystem * const pSystem, const IHeap::BucketInfoVector &bucketInfoVector, const size_t initialSize, const std::string &namePrefix);

	/**
	 *	@brief	Get a message from the pool of heaps.
	 *
	 *	@param	dataLength.
	 *	@param	pData.
	 *
	 *	@return	HeapMessage.
	 */
	ACTIV_SYSTEM_API HeapMessage GetMessage(const size_t dataLength, const void * const pData);

	/**
	 *	@brief	Get a heap from the pool.
	 *
	 *	If no heaps are in the pool, a new heap is created.
	 *
	 *	@return	Pointer to a heap, or 0 if failed to create a heap.
	 */
	ACTIV_SYSTEM_API IHeap *Get();

	/**
	 *	@brief	Return a heap to the pool.
	 *
	 *	@param	pHeap the heap to return.
	 */
	ACTIV_SYSTEM_API void Return(IHeap * const pHeap);

	/**
	 *	@brief	Extend the number of heaps in the pool.
	 *
	 *	@param	numberOfHeaps number of heaps to create.
	 *
	 *	@return	Number of heaps actually created.
	 */
	ACTIV_SYSTEM_API size_t Extend(const size_t numberOfHeaps);

private:
	/**
	 *	@brief	Create an individual heap.
	 *
	 *	@return	pHeap the heap, or 0 on error.
	 */
	IHeap *CreateHeap();

	typedef std::vector<IHeap *> HeapList;						///< Type of heap list

	ISystem * const					m_pSystem;					///< System to create heaps in
	const IHeap::BucketInfoVector	m_bucketInfoVector;			///< Size of heaps to create
	const std::string				m_namePrefix;				///< Prefix to give to heap names
	HeapList						m_heapList;					///< Free heaps in the pool
	size_t							m_nextHeapNumber;			///< Increasing count to make unique heap names
};

} // namespace Activ

#endif // !defined (ACTIV_SYSTEM_HEAP_POOL_H)

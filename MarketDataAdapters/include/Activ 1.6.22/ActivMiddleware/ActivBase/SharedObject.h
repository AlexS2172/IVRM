/**
 *	@file	SharedObject.h
 *
 *	@brief	Header file for the template shared object class.
 *
 *	$Log: $
 */

#if !defined (ACTIV_SHARED_OBJECT_H)
#define ACTIV_SHARED_OBJECT_H

#include "ActivMiddleware/ActivBase/PagingFileMappedMemoryFactory.h"

#include <string>

namespace Activ
{

/**
 *	@brief	Templatized shared object class.
 */
template <class T>
class SharedObject
{
public:
	/**
	 *	@brief	Default constructor.
	 */
	SharedObject();

	/**
	 *	@brief	Default destructor.
	 */
	~SharedObject();

	/**
	 *	@brief	Is the shared object open?
	 */
	bool IsOpen() const;

	/**
	 *	@brief	Create shared memory object.
	 *
	 *	@param	name the name of the object.
	 *	@param	mappingFlags flags indicating how the memory segment should be mapped.
	 *	@param	size the size of the object, by default the size of the class T.
	 *
	 *	@throw	OsException
	 *	@throw	BaseException
	 */
	void Create(const std::string &name, const IMappedMemory::MappingFlags mappingFlags = IMappedMemory::MAPPING_FLAG_UNIQUE, const uint64_t size = sizeof(T));

	/**
	 *	@brief	Open existing shared memory object.
	 *
	 *	@param	name the name of the object.
	 *	@param	mappingFlags flags indicating how the memory segment should be mapped.
	 *	@param	timeout object specifying how long to wait for the object to exist, in ms.
	 *
	 *	@throw	OsException.
	 *	@throw	BaseException
	 */
	void Open(const std::string &name, const IMappedMemory::MappingFlags mappingFlags = IMappedMemory::MAPPING_FLAG_NONE, Timeout &timeout = Timeout::GetInfinite());

	/**
	 *	@brief	Get pointer to shared object.
	 */
	T *GetPointer() const;

private:
	T				*m_pData;		///< Pointer to mapped portion.
	IMappedMemory	*m_pMem;		///< Pointer to shared memory object.
};

// ---------------------------------------------------------------------------------------------------------------------------------

template<class T>
inline SharedObject<T>::SharedObject() :
	m_pData(0),
	m_pMem(0)
{
}

// ---------------------------------------------------------------------------------------------------------------------------------

template<class T>
inline SharedObject<T>::~SharedObject()
{
	if (0 != m_pMem)
		delete m_pMem;
}

// ---------------------------------------------------------------------------------------------------------------------------------

template<class T>
inline bool SharedObject<T>::IsOpen() const
{
	return m_pData != 0;
}

// ---------------------------------------------------------------------------------------------------------------------------------

template<class T>
inline void SharedObject<T>::Create(const std::string &name, const IMappedMemory::MappingFlags mappingFlags, const uint64_t size)
{
	m_pMem = PagingFileMappedMemoryFactory::GetInstance().Create(name, size, mappingFlags);
	m_pData = (T *)m_pMem->GetPointer();
}

// ---------------------------------------------------------------------------------------------------------------------------------

template<class T>
inline void SharedObject<T>::Open(const std::string &name, const IMappedMemory::MappingFlags mappingFlags, Timeout &timeout)
{
	m_pMem = PagingFileMappedMemoryFactory::GetInstance().Open(name, mappingFlags, timeout);
	m_pData = (T *)m_pMem->GetPointer();
}

// ---------------------------------------------------------------------------------------------------------------------------------

template<class T>
inline T *SharedObject<T>::GetPointer() const
{
	return m_pData;
}

} // namespace Activ

#endif // !defined (ACTIV_SHARED_OBJECT_H)

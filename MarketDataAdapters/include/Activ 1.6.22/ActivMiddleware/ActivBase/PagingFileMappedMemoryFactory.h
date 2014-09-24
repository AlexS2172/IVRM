/**
 *	@file	PagingFileMappedMemoryFactory.h
 *
 *	@brief	Header file for the paging file mapped memory factory class.
 *
 *	$Log: $
 */

#if !defined (ACTIV_PAGING_FILE_MAPPED_MEMORY_FACTORY_H)
#define ACTIV_PAGING_FILE_MAPPED_MEMORY_FACTORY_H

#include "ActivMiddleware/ActivBase/External.h"
#include "ActivMiddleware/ActivBase/IMappedMemory.h"
#include "ActivMiddleware/ActivBase/Timeout.h"

#include <string>

namespace Activ
{

/**
 *	@brief	Paging file mapped memory factory class.
 */
class PagingFileMappedMemoryFactory
{
public:
	/**
	 *	@brief	Create a paging file mapped memory segment that can be shared via name.
	 *
	 *	@param	name the name of the mapped memory segment.
	 *	@param	size the size of the mapped memory segment.
	 *	@param	mappingFlags creation flags.
	 *
	 *	@return	Pointer to an object providing an IMappedMemory interface.
	 *
	 *	@throw	Exception
	 */
	ACTIV_BASE_API IMappedMemory *Create(const std::string &name, const uint64_t size, const IMappedMemory::MappingFlags mappingFlags = IMappedMemory::MAPPING_FLAG_NONE);

	/**
	 *	@brief	Create an anonymous paging file mapped memory segment.
	 *
	 *	@param	size the size of the mapped memory segment.
	 *	@param	mappingFlags creation flags.
	 *
	 *	@return	Pointer to an object providing an IMappedMemory interface.
	 *
	 *	@throw	Exception
	 */
	ACTIV_BASE_API IMappedMemory *Create(const uint64_t size, const IMappedMemory::MappingFlags mappingFlags = IMappedMemory::MAPPING_FLAG_NONE);

	/**
	 *	@brief	Open an existing paging file mapped memory segment.
	 *
	 *	@param	name the name of the mapped memory segment.
	 *	@param	mappingFlags flags that indicate how the memory segment should be mapped.
	 *	@param	timeout object specifying how long to wait for the segment to exist, in ms.
	 *
	 *	@return	pointer to an object providing an IMappedMemory interface.
	 *
	 *	@throw	OsException
	 *	@throw	BaseException
	 */
	ACTIV_BASE_API IMappedMemory *Open(const std::string &name, const IMappedMemory::MappingFlags mappingFlags = IMappedMemory::MAPPING_FLAG_NONE, Timeout &timeout = Timeout::GetInfinite());

	/**
	 *	@brief	Get the instance of the factory object.
	 */
	ACTIV_BASE_API static PagingFileMappedMemoryFactory &GetInstance();

private:
    /**
     *	@brief	Default constructor.
     */
	PagingFileMappedMemoryFactory();	

	static PagingFileMappedMemoryFactory	m_factory;	///< Singleton paging file mapped memory factory.
};

} // namespace Activ

#endif // !defined (ACTIV_PAGING_FILE_MAPPED_MEMORY_FACTORY_H)

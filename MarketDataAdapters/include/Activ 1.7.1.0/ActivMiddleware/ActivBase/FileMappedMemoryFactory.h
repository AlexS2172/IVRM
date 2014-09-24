/**
 *	@file	FileMappedMemoryFactory.h
 *
 *	@brief	Header file for the file mapped memory factory class.
 *
 *	$Log: $
 */

#if (!defined ACTIV_BASE_FILE_MAPPED_MEMORY_FACTORY_H)
#define ACTIV_BASE_FILE_MAPPED_MEMORY_FACTORY_H

#include "ActivMiddleware/ActivBase/External.h"
#include "ActivMiddleware/ActivBase/IFileMappedMemory.h"
#include "ActivMiddleware/ActivBase/Timeout.h"

#include <string>

namespace Activ
{

/**
 *	@brief	File mapped memory factory class.
 */
class FileMappedMemoryFactory
{
public:
	/**
	 *	@brief	Create a memory mapped file that can be shared via name.
	 *
	 *	@param	fileName the name of the file to map into memory.
	 *	@param	size the size to the memory segment to be mapped.
	 *	@param	shared flag indicating whether the memory mapped file can be shared.
	 *	@param	fileFlags flags to use when creating or opening the file.
	 *	@param	mappingFlags flags to use when creating the mapped memory segment.
	 *
	 *	@return	Pointer to an object providing an IFileMappedMemory interface.
	 *
	 *	@throw	Exception
	 */
	ACTIV_BASE_API IFileMappedMemory *Create(const std::string &fileName, const uint64_t size, const bool shared, const IFileMappedMemory::FileFlags fileFlags = IFileMappedMemory::FILE_FLAG_NONE, const IMappedMemory::MappingFlags mappingFlags = IMappedMemory::MAPPING_FLAG_NONE);

	/**
	 *	@brief	Open an existing memory mapped file.
	 *
	 *	@param	fileName the name of the file to map into memory.
	 *	@param	fileFlags flags to use when creating or opening the file.
	 *	@param	mappingFlags flags to use when creating the mapped memory segment.
	 *	@param	timeout object specifying how long to wait for the memory mapped file to exist, in ms.
	 *
	 *	@return	pointer to an object providing an IFileMappedMemory interface.
	 *
	 *	@throw	Exception
	 */
	ACTIV_BASE_API IFileMappedMemory *Open(const std::string &fileName, const IFileMappedMemory::FileFlags fileFlags = IFileMappedMemory::FILE_FLAG_NONE, const IMappedMemory::MappingFlags mappingFlags = IMappedMemory::MAPPING_FLAG_NONE, Timeout &timeout = Timeout::GetInfinite());

	/**
	 *	@brief	Get the instance of the factory object.
	 */
	ACTIV_BASE_API static FileMappedMemoryFactory &GetInstance();

private:
    /**
     *	@brief	Default constructor.
     */
	FileMappedMemoryFactory();	

	static FileMappedMemoryFactory	m_factory;	///< Singleton file mapped memory factory.
};

} // namespace Activ

#endif // (!defined ACTIV_BASE_FILE_MAPPED_MEMORY_FACTORY_H)

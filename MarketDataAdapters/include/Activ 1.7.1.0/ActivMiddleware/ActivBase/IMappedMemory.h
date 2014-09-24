/**
 *  @file	IMappedMemory.h
 *
 *	@brief	Header file for the mapped memory interface class.
 *
 *	$Log: $
 */

#if (!defined ACTIV_BASE_IMAPPED_MEMORY_H)
#define ACTIV_BASE_IMAPPED_MEMORY_H

#include "ActivMiddleware/ActivBase/External.h"

namespace Activ
{

/**
 *	@brief	Abstract base mapped memory class.
 */
class IMappedMemory
{
public:
	typedef uint32_t MappingFlags;

	static const MappingFlags MAPPING_FLAG_NONE = 0x00000000;
	static const MappingFlags MAPPING_FLAG_READ_ONLY = 0x00000001;	///< Read only access to the mapped memory segment.
	static const MappingFlags MAPPING_FLAG_UNIQUE = 0x00000002;		///< A named segment cannot already exist when creating.
		
	/**
	 *	@brief	Default destructor.
	 */
	virtual ~IMappedMemory() = 0;

	/**
	 *	@brief	Get pointer to mapped memory.
	 */
	virtual void *GetPointer() const = 0;
};

// ---------------------------------------------------------------------------------------------------------------------------------

inline IMappedMemory::~IMappedMemory()
{
}

} // namespace Activ

#endif // (!defined ACTIV_BASE_IMAPPED_MEMORY_H)

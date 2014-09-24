/**
 *  @file	IMappedMemory.h
 *
 *	@brief	Header file for the mapped memory interface class.
 *
 *	$Log: $
 */

#if !defined (ACTIV_IMAPPED_MEMORY_H)
#define ACTIV_IMAPPED_MEMORY_H

#include "ActivMiddleware/ActivBase/External.h"

namespace Activ
{

/**
 *	@brief	Abstract base mapped memory class.
 */
class IMappedMemory
{
public:
	/**
	 *	@brief	Flags to indicate how the memory segment should be mapped.
	 */
	enum
	{
		MAPPING_FLAG_NONE = 0x00000000,
		MAPPING_FLAG_READ_ONLY = 0x00000001,	///< Read only access to the mapped memory segment.
		MAPPING_FLAG_UNIQUE = 0x00000002		///< A named segment cannot already exist when creating.
	};
	typedef uint32_t MappingFlags;

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

#endif // !defined (ACTIV_IMAPPED_MEMORY_H)

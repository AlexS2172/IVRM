/**
 *  @file	IFileMappedMemory.h
 *
 *	@brief	Header file for the file mapped memory interface class.
 *
 *	$Log: $
 */

#if !defined (ACTIV_IFILE_MAPPED_MEMORY_H)
#define ACTIV_IFILE_MAPPED_MEMORY_H

#include "ActivMiddleware/ActivBase/External.h"
#include "ActivMiddleware/ActivBase/IMappedMemory.h"

namespace Activ
{

/**
 *	@brief	Abstract base file mapped memory class.
 */
class IFileMappedMemory : public IMappedMemory
{
public:
	static const uint64_t USE_CURRENT_FILE_SIZE = 0;		///< Used for creating a mapping of the current file size.

	/**
	 *	@brief	File flags.
	 */
	enum
	{
		FILE_FLAG_NONE = 0x00000000,
		FILE_FLAG_CREATE_ALWAYS = 0x00000001,				///< Always create new file.
		FILE_FLAG_OPEN_EXISTING = 0x00000002,				///< Open existing file, fail if file doesn't exist.
 		FILE_FLAG_READ_ONLY = 0x00000004,					///< Open file in read only mode.
		FILE_FLAG_DELETE_AT_CLOSE = 0x00000008				///< Delete the file when the handle to it is closed.
	};
	typedef uint32_t FileFlags;

	/**
	 *	@brief	Default destructor.
	 */
	virtual ~IFileMappedMemory() = 0;

	/**
	 *	@brief	Resize a memory mapped file.
	 *
	 *	@param	size the new size of the memory mapped file.
	 *
	 *	@return	a pointer to the new mapped memory region.
	 */
	virtual void *Resize(const uint64_t size) = 0;

	/**
	 *	@brief	Flush the memory mapped file to the disk.
	 */
	virtual void Flush() const = 0;

	/**
	 *	@brief	Get size of file used for memory mapping.
	 */
	virtual uint64_t GetFileSize() const = 0;
};

// ---------------------------------------------------------------------------------------------------------------------------------

inline IFileMappedMemory::~IFileMappedMemory()
{
}

} // namespace Activ

#endif // !defined (ACTIV_IFILE_MAPPED_MEMORY_H)

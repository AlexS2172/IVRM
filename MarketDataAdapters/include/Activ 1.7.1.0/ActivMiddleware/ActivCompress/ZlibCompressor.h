/**
 *  @file	ZlibCompressor.h
 *
 *	@brief
 *
 *	$Log: $
 */

#if (!defined ACTIV_COMPRESS_ZLIB_COMPRESSOR_H)
#define ACTIV_COMPRESS_ZLIB_COMPRESSOR_H

#include "ActivMiddleware/ActivCompress/External.h"
#include "ActivMiddleware/ActivCompress/ICompressor.h"

#include "ActivMiddleware/ActivBase/ThreadSpecificData.h"

#if ((defined ACTIV_PLATFORM_WINDOWS) && (defined ACTIV_COMPILER_VISUAL_C)) || (defined ACTIV_PLATFORM_LINUX)
#define ACTIV_COMPRESS_USE_STOR_COMPRESS
#endif

namespace Activ
{

/**
 *	@brief	Zlib compression and decompression class (see http://www.gzip.org/zlib/).
 */
class ZlibCompressor : public ICompressor
{
public:
	/**
	 *	@brief	Default constructor.
	 *
	 *	Checks for StorCompress card and uses if available and @useHardwareZlib is true.
	 *
	 *	@param	useHardwareZlib use hardware zlib card if available?
	 */
	ACTIV_COMPRESS_API ZlibCompressor(const bool useHardwareZlib = true);

	/**
	 *	@brief	Destructor.
	 */
	ACTIV_COMPRESS_API virtual ~ZlibCompressor();

	ACTIV_COMPRESS_API virtual StatusCode Compress(void *pDestination, const size_t destinationLength, const void *pSource, const size_t sourceLength, size_t &compressedLength);
	ACTIV_COMPRESS_API virtual StatusCode Decompress(void *pDestination, const size_t destinationLength, const void *pSource, const size_t sourceLength, size_t &decompressedLength);

private:
	/**
	 *	@brief	Is the StorCompress card available?
	 *
	 *	@return	boolean.
	 */
	bool IsStorCompressAvailable() const;

#if (defined ACTIV_PLATFORM_WINDOWS)

	typedef char ThreadInfo;

#elif (defined ACTIV_PLATFORM_LINUX)

	/**
	 *	@brief	Per-thread info.
	 */
	class ThreadInfo
	{
	public:
		/**
		 *	@brief	Constructor.
		 *
		 *	@throw	OsException if fails to mmap() buffers.
		 */
		ThreadInfo();

		/**
		 *	@brief	Destructor.
		 *
		 *	munmap()s buffers.
		 */
		~ThreadInfo();

		HANDLE		m_storCompressHandle;						///< handle to card
		void *		m_pInputBuffer;								///< mmap()ed src buffer
		void *		m_pOutputBuffer;							///< mmap()ed tgt buffer
		uint32_t	m_pipelineIndex;							///< next buffer to use
	};

	/**
	 *	@brief	(De)compress a buffer with StorCompress.
	 *
	 *	@param	destination target buffer for data.
	 *	@param	destinationLength max bytes to copy into @a destination.
	 *	@param	source source buffer to act on.
	 *	@param	sourceLength length of data at @a source to act on.
	 *	@param	resultLength length of (de)compressed data.
	 *
	 *	@return	STATUS_CODE_SUCCESS or error status.
	 */
	StatusCode StorFunction(const uint32_t request, void *destination, const size_t destinationLength, const void *source, const size_t sourceLength, size_t &resultLength);

#endif

#if (defined ACTIV_COMPRESS_USE_STOR_COMPRESS)

	/**
	 *	@brief	Compress a buffer with StorCompress.
	 *
	 *	@param	destination target buffer for data.
	 *	@param	destinationLength max bytes to copy into @a destination.
	 *	@param	source source buffer to act on.
	 *	@param	sourceLength length of data at @a source to act on.
	 *	@param	resultLength length of compressed data.
	 *
	 *	@return	STATUS_CODE_SUCCESS or error status.
	 */
	StatusCode StorCompress(void *destination, const size_t destinationLength, const void *source, const size_t sourceLength, size_t &resultLength);

	/**
	 *	@brief	Decompress a buffer with StorCompress.
	 *
	 *	@param	destination target buffer for data.
	 *	@param	destinationLength max bytes to copy into @a destination.
	 *	@param	source source buffer to act on.
	 *	@param	sourceLength length of data at @a source to act on.
	 *	@param	resultLength length of decompressed data.
	 *
	 *	@return	STATUS_CODE_SUCCESS or error status.
	 */
	StatusCode StorDecompress(void *destination, const size_t destinationLength, const void *source, const size_t sourceLength, size_t &resultLength);

	typedef ThreadSpecificData<ThreadInfo, DeleteObject> ThreadInfoThreadSpecificData;

	ThreadInfoThreadSpecificData	m_threadInfo;				///< thread local info

#endif // (defined ACTIV_COMPRESS_USE_STOR_COMPRESS)

	HANDLE							m_storCompressHandle;		///< handle to StorCompress card
};

} // namespace Activ

#endif // (!defined ACTIV_COMPRESS_ZLIB_COMPRESSOR_H)

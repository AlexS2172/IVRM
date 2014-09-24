/**
 *  @file	ICompressor.h
 *
 *	@brief
 *
 *	$Log: $
 */

#if !defined (ACTIV_ICOMPRESSOR_H)
#define ACTIV_ICOMPRESSOR_H

#include "ActivMiddleware/ActivCompress/External.h"

namespace Activ
{

/**
 *	@brief	Calculate size of destination buffer needed to compress a buffer of length sourceLength.
 *
 *	To support Zlib, destination buffer must be at least 0.1% + 12 bytes the size of the source buffer.
 *
 *	@param	sourceLength length of the source buffer.
 *
 *	@return size of destination buffer needed.
 */
#define ACTIV_COMPRESS_GET_DESTINATION_BUFFER_LENGTH(sourceLength)	\
	(static_cast<size_t>(((static_cast<uint64_t>((sourceLength)) * UINT64_C(1001)) / UINT64_C(1000)) + 1 + 12))

// ---------------------------------------------------------------------------------------------------------------------------------

/**
 *	@brief	Abstract base block compression and decompression class.
 */
class ICompressor
{
public:
	/**
	 *	@brief	Destructor.
	 */
	ACTIV_COMPRESS_API virtual ~ICompressor() = 0;

	/**
	 *	@brief	Compress a buffer.
	 *
	 *	@param	pDestination the resultant compressed buffer.
	 *	@param	destinationLength the length of the destination buffer.
	 *	@param	pSource the buffer to be compressed.
	 *	@param	sourceLength length of the source buffer.
	 *	@param	compressedLength the number of bytes used in the destination buffer.
	 *
	 *	@retval STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_BUFFER_OVERFLOW
	 *	@retval STATUS_CODE_FAILURE
	 *	@retval	STATUS_CODE_INVALID_FORMAT
	 *	@retval STATUS_CODE_HEAP_EMPTY
	 */
	ACTIV_COMPRESS_API virtual StatusCode Compress(void *pDestination, const size_t destinationLength, const void *pSource, const size_t sourceLength, size_t &compressedLength) = 0;

	/**
	 *	@brief	Decompress a buffer.
	 *
	 *	@param	pDestination the resultant decompressed buffer.
	 *	@param	destinationLength the length of the destination buffer.
	 *	@param	pSource the buffer to be decompressed.
	 *	@param	sourceLength length of the source buffer.
	 *	@param	decompressedLength the number of bytes used in the destination buffer.
	 *
	 *	@retval STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_BUFFER_OVERFLOW
	 *	@retval STATUS_CODE_FAILURE
	 *	@retval	STATUS_CODE_INVALID_FORMAT
	 *	@retval STATUS_CODE_HEAP_EMPTY
	 */
	ACTIV_COMPRESS_API virtual StatusCode Decompress(void *pDestination, const size_t destinationLength, const void *pSource, const size_t sourceLength, size_t &decompressedLength) = 0;

	/**
	 *	@brief	Get compress statistics.
	 *
	 *	@param	apiCount total number of API calls made.
	 *	@param	inputBytes total number of input bytes.
	 *	@param	outputBytes total number of output bytes.
	 */
	ACTIV_COMPRESS_API void GetCompressStats(uint64_t &apiCount, uint64_t &inputBytes, uint64_t &outputBytes) const;

	/**
	 *	@brief	Get decompress statistics.
	 *
	 *	@param	apiCount total number of API calls made.
	 *	@param	inputBytes total number of input bytes.
	 *	@param	outputBytes total number of output bytes.
	 */
	ACTIV_COMPRESS_API void GetDecompressStats(uint64_t &apiCount, uint64_t &inputBytes, uint64_t &outputBytes) const;

	/**
	 *	@brief	Clear compress statistics.
	 */
	ACTIV_COMPRESS_API void ClearCompressStats();

	/**
	 *	@brief	Clear decompress statistics.
	 */
	ACTIV_COMPRESS_API void ClearDecompressStats();

private:
	/**
	*	@brief	Compression and decompression statistics class.
	*/
	class Stats
	{
	public:
		/**
		 *	@brief	Constructor.
		 */
		ACTIV_COMPRESS_API Stats();

		/**
		 *	@brief	Update compression or decompression statistics.
		 *
		 *	@param	inputBytes number of input bytes.
		 *	@param	outputBytes number of output bytes.
		 */
		ACTIV_COMPRESS_API void UpdateStats(uint64_t inputBytes, uint64_t outputBytes);

		/**
		 *	@brief	Get compression or decompression statistics.
		 *
		 *	@param	apiCount total number of API calls made.
		 *	@param	inputBytes total number of input bytes.
		 *	@param	outputBytes total number of output bytes.
		 */
		ACTIV_COMPRESS_API void GetStats(uint64_t &apiCount, uint64_t &inputBytes, uint64_t &outputBytes) const;

		/**
		 *	@brief	Clear compression or decompression statistics.
		 */
		ACTIV_COMPRESS_API void ClearStats();
	
	private:
		uint64_t  m_apiCount;											///< number of API calls made.
		uint64_t  m_inputBytes;											///< total number of input bytes.
		uint64_t  m_outputBytes;										///< total number of output bytes.
	};

protected:
	Stats	m_compressStats;											///< compression statistics.
	Stats	m_decompressStats;											///< decompression statistics.
};

} // namespace Activ

#endif // !defined (ACTIV_ICOMPRESSOR_H)

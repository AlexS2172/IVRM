/**
 *  @file	NullCompressor.h
 *
 *	@brief
 *
 *	$Log: $
 */

#if !defined (ACTIV_NULL_COMPRESSOR_H)
#define ACTIV_NULL_COMPRESSOR_H

#include "ActivMiddleware/ActivCompress/External.h"
#include "ActivMiddleware/ActivCompress/ICompressor.h"

namespace Activ
{

/**
 *	@brief	Null compression and decompression class.
 */
class NullCompressor : public ICompressor
{
public:
	/**
	 *	@brief	Default constructor.
	 */
	ACTIV_COMPRESS_API NullCompressor();
	
	/**
	 *	@brief	Destructor.
	 */
	ACTIV_COMPRESS_API virtual ~NullCompressor();

	ACTIV_COMPRESS_API virtual StatusCode Compress(void *pDestination, const size_t destinationLength, const void *pSource, const size_t sourceLength, size_t &compressedLength);
	ACTIV_COMPRESS_API virtual StatusCode Decompress(void *pDestination, const size_t destinationLength, const void *pSource, const size_t sourceLength, size_t &decompressedLength);
};

} // namespace Activ

#endif // !defined (ACTIV_NULL_COMPRESSOR_H)

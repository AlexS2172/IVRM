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
class ACTIV_COMPRESS_API NullCompressor : public ICompressor
{
public:
	/**
	 *	@brief	Destructor.
	 */
	virtual ~NullCompressor();

	virtual StatusCode Compress(void *destination, const size_t destinationLength, const void *source, const size_t sourceLength, size_t &compressedLength);
	virtual StatusCode Decompress(void *destination, const size_t destinationLength, const void *source, const size_t sourceLength, size_t &decompressedLength);
};

} // namespace Activ

#endif // !defined (ACTIV_NULL_COMPRESSOR_H)

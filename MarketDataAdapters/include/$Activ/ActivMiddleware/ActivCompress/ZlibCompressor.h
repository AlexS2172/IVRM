/**
 *  @file	ZlibCompressor.h
 *
 *	@brief
 *
 *	$Log: $
 */

#if !defined (ACTIV_ZLIB_COMPRESSOR_H)
#define ACTIV_ZLIB_COMPRESSOR_H

#include "ActivMiddleware/ActivCompress/External.h"
#include "ActivMiddleware/ActivCompress/ICompressor.h"

namespace Activ
{

/**
 *	@brief	Zlib compression and decompression class (see http://www.gzip.org/zlib/).
 */
class ACTIV_COMPRESS_API ZlibCompressor : public ICompressor
{
public:
	/**
	 *	@brief	Destructor.
	 */
	virtual ~ZlibCompressor();

	virtual StatusCode Compress(void *destination, const size_t destinationLength, const void *source, const size_t sourceLength, size_t &compressedLength);
	virtual StatusCode Decompress(void *destination, const size_t destinationLength, const void *source, const size_t sourceLength, size_t &decompressedLength);
};

} // namespace Activ

#endif // !defined (ACTIV_ZLIB_COMPRESSOR_H)

/**
 *  @file	RdcCompressor.h
 *
 *	@brief
 *
 *	$Log: $
 */

#if !defined (ACTIV_RDC_COMPRESSOR_H)
#define ACTIV_RDC_COMPRESSOR_H

#include "ActivMiddleware/ActivCompress/External.h"
#include "ActivMiddleware/ActivCompress/ICompressor.h"

namespace Activ
{

/**
 *	@brief	RDC compression and decompression class (see code archive of Oct '92 edition of CUJ at http://www.cuj.com/code/archive.htm).
 */
class ACTIV_COMPRESS_API RdcCompressor : public ICompressor
{
public:
	/**
	 *	@brief	Destructor.
	 */
	virtual ~RdcCompressor();

	virtual StatusCode Compress(void *destination, const size_t destinationLength, const void *source, const size_t sourceLength, size_t &compressedLength);
	virtual StatusCode Decompress(void *destination, const size_t destinationLength, const void *source, const size_t sourceLength, size_t &decompressedLength);

private:
	static const size_t		HASH_LEN = 4096;							///< The number of hash table entries

	byte_t	*m_hashTable[HASH_LEN];										///< The hash table
};

} // namespace Activ

#endif // !defined (ACTIV_RDC_COMPRESSOR_H)

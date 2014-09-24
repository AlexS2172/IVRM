/**
 *  @file	Stemmer.h
 *
 *	@brief
 *
 *	$Log: $
 */

#if !defined (ACTIV_INDEXER_STEMMER_H)
#define ACTIV_INDEXER_STEMMER_H

#include "ActivMiddleware/ActivIndexer/External.h"

#include <string>

// Forward reference
struct sb_stemmer;

namespace Activ
{

namespace Indexer
{

/**
 *	@brief	Stemmer Class
 *
 *	NOTE: The class is not thread safe.  You should create a stemmer for each thread that needs one
 *	or provide suitable locking around calls to StemWord()
 */
class Stemmer
{
public:
	/**
	 *	@brief	Default constructor
	 *
	 *  @param algorithm The algorithm name.  This is either the english name of the algorithm,
	 *	or the 2 or 3 letter ISO 639 codes for the language.  Valid algorithm names are:
	 *	danish, dutch, english, finnish, french, german, italian, norwegian,
	 *	porter (legacy english algorith), portuguese, spanish, swedish, russian
	 *
	 *  @param characterEncoding The character encoding.  NULL may be passed as
	 *  this value, in which case UTF-8 encoding will be assumed. Otherwise,
	 *  the argument may be one of "UTF_8", "ISO_8859_1" (ie, Latin 1),
	 *  "CP850" (ie, MS-DOS Latin 1) or "KOI8_R" (Russian).  Note that
	 *  case is significant in this parameter.
	 */
	ACTIV_INDEXER_API Stemmer(const std::string &algorithm, const std::string &characterEncoding);

	/**
	 *	@brief	Default destructor
	 */
	ACTIV_INDEXER_API ~Stemmer();

	/**
	 *	@brief	Stem a word. The word should be converted to lowercase before stemming
	 *
	 *	If you are using a single stemmer in multiple threads, then this function will need a suitable
	 *	locking mechanism around it.  You may also want to copy the return value into a more
	 *	"permanent" variable rather than using the returned reference.
	 */
	ACTIV_INDEXER_API const std::string &StemWord(const std::string &word);

private:
	sb_stemmer *	m_stemmer;
	std::string		m_stemmedWord;
};


} // namespace Indexer

} // namespace Activ

#endif // !defined (ACTIV_INDEXER_STEMMER_H)

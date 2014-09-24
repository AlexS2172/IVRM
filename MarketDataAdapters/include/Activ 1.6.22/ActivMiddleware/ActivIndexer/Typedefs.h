/**
 *  @file	Typedefs.h
 *
 *	@brief
 *
 *	$Log: $
 */

#if !defined (ACTIV_INDEXER_TYPEDEFS_H)
#define ACTIV_INDEXER_TYPEDEFS_H

#include "ActivMiddleware/Activ.h"
#include "ActivMiddleware/Misc/StlHelper.h"

#include <algorithm>
#include <limits>
#include <locale>
#include <set>
#include <string>
#include <vector>

namespace Activ
{

namespace Indexer
{

typedef uint32_t DocumentId;
typedef std::vector<DocumentId> DocumentList;
typedef std::set<std::string> StringSet;
typedef Activ::hash_set<std::string, StringHashCompare<> > StringHashSet;
typedef std::vector<byte_t> ByteVector;

static const DocumentId DOCUMENT_ID_UNKNOWN = std::numeric_limits<DocumentId>::max();

/**
 *	@brief Functor wrapper for std::tolower (so it can be used with std::transform)
 */
struct ToLower : public std::binary_function<std::string::value_type, std::locale, std::string::value_type>
{
	std::string::value_type operator () (const std::string::value_type &c, const std::locale &loc) const
	{
		return std::tolower(c, loc);
	}
};

/**
 *	@brief Convert a string to lowercase with respect to a given locale
 */
inline void StringToLowerWithLocale(std::string &str, const std::locale &loc)
{
	std::transform(str.begin(), str.end(), str.begin(), std::bind2nd(ToLower(), loc));
}

} // namespace Indexer

} // namespace Activ

#endif // !defined (ACTIV_INDEXER_TYPEDEFS_H)

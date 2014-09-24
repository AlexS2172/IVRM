/**
 *  @file	StlHelper.h
 *
 *	@brief	Work arounds for stl disparities across platforms and various helpers.
 *
 *	$Log: $
 */

#if !defined (ACTIV_STL_HELPER_H)
#define ACTIV_STL_HELPER_H

#include "ActivMiddleware/ActivBase/ActivBase.h"

#include <vector>
#include <string>
#include <list>

#if defined (_STLPORT_VERSION)
// STLport stl
#include <hash_map>
#include <hash_set>
#define ACTIV_STL_HELPER_EXT_NAMESPACE	_STLP_STD
#define ACTIV_STL_HELPER_DEFINE_HASH_COMPARE

// ---------------------------------------------------------------------------------------------------------------------------------

#elif (defined ACTIV_COMPILER_VISUAL_C) || (defined ACTIV_COMPILER_INTEL)
// Dinkumware stl
#include <hash_map>
#include <hash_set>
#if (_CPPLIB_VER > 310)
#define ACTIV_STL_HELPER_EXT_NAMESPACE	stdext
#else
#define ACTIV_STL_HELPER_EXT_NAMESPACE	std
#endif
#define ACTIV_STL_HELPER_NONSTD_VECTOR_CLEAR

// ---------------------------------------------------------------------------------------------------------------------------------

#elif defined (ACTIV_COMPILER_GCC)
// GCC stl
#include <ext/hash_map>
#include <ext/hash_set>
#define ACTIV_STL_HELPER_EXT_NAMESPACE	__gnu_cxx
#define ACTIV_STL_HELPER_DEFINE_HASH_COMPARE

// ---------------------------------------------------------------------------------------------------------------------------------

#else
#error Add for this compiler / stl implementation.
#endif

// ---------------------------------------------------------------------------------------------------------------------------------

#if defined (ACTIV_STL_HELPER_DEFINE_HASH_COMPARE)

namespace ACTIV_STL_HELPER_EXT_NAMESPACE
{

/**
 *	@brief	Dinkumware STL uses hash_compare class. Provide a similiar class for STLs without it.
 *
 *	We have to do it this way around as hash_compare has more template parameters than hash.
 */
template<typename KEY, typename TRAITS>
class hash_compare : public hash<KEY>
{
public:
	/**
	 *	@brief	Function operator for less than comparison.
	 *
	 *	@param	k1 key to compare.
	 *	@param	k2 key to compare.
	 *
	 *	@return	true if k1 < k2.
	 */
	bool operator()(const KEY &k1, const KEY &k2) const
	{
		return TRAITS(k1, k2);
	}
};

} // namespace ACTIV_STL_HELPER_EXT_NAMESPACE

#endif // !defined (ACTIV_STL_HELPER_DEFINE_HASH_COMPARE)

// ---------------------------------------------------------------------------------------------------------------------------------

namespace Activ
{

/**
 *	@brief	Empty a vector but do not deallocate reserved storage.
 *
 *	Dinkumware STL vector::clear() will clear down storage in the vector. This doesn't
 *	and behaves the same on all platforms.
 *
 *	@param	v the vector to clear.
 */
template <typename T>
inline void EmptyVector(std::vector<T> &v)
{
#if defined (ACTIV_STL_HELPER_NONSTD_VECTOR_CLEAR)
	while (!v.empty())
		v.pop_back();
#else
	v.clear();
#endif
}

// ---------------------------------------------------------------------------------------------------------------------------------

// bring the various hash classes into the Activ namespace
using ACTIV_STL_HELPER_EXT_NAMESPACE::hash_map;
using ACTIV_STL_HELPER_EXT_NAMESPACE::hash_set;
using ACTIV_STL_HELPER_EXT_NAMESPACE::hash_compare;

// ---------------------------------------------------------------------------------------------------------------------------------

/**
 *	@brief	Hash compare class for stl hash containers where the key is a string.
 *
 *	@param	TRAITS less than comparison class.
 */
template<typename TRAITS = std::less<std::string> >
class StringHashCompare : public hash_compare<std::string, TRAITS>
{
public:
	/**
	 *	@brief	Hashing function operator.
	 *
	 *	@param	key key to hash.
	 *
	 *	@return	hash value.
	 */
	size_t operator()(const std::string &key) const;

	/**
	 *	@brief	Unhide operator() in base.
	 */
	bool operator()(const std::string &k1, const std::string &k2) const;
};

// ---------------------------------------------------------------------------------------------------------------------------------

template<typename TRAITS>
inline size_t StringHashCompare<TRAITS>::operator()(const std::string &key) const
{
	return GenericHash(key.c_str(), key.length(), 0);
}

// ---------------------------------------------------------------------------------------------------------------------------------

template<typename TRAITS>
inline bool StringHashCompare<TRAITS>::operator()(const std::string &k1, const std::string &k2) const
{
	// can't just use using BASE::operator(), VC++ complains
	return hash_compare<std::string, TRAITS>::operator()(k1, k2);
}

// ---------------------------------------------------------------------------------------------------------------------------------

/**
 *	@brief	Unary function object to return the first element of a pair.
 *
 *	@param	PAIR pair type (or any type with a first member and a first_type typedef).
 */
template <typename P>
class SelectFirst : public std::unary_function<P, typename P::first_type>
{
public:
	typedef P ArgumentType;
	typedef typename P::first_type ResultType;
	
	/**
	 *	@brief	Function operator.
	 *
	 *	@param	p reference to a PAIR object.
	 *
	 *	@return	p.first
	 */
	const ResultType &operator()(const ArgumentType &p) const
	{
		return p.first;
	}
};

// ---------------------------------------------------------------------------------------------------------------------------------

/**
 *	@brief	Sort a container and check for duplicates.
 *
 *	@param	c container.
 *
 *	@return	true if there are no duplicates.
 */
template<typename T>
inline bool SortUnique(T &c)
{
	std::sort(c.begin(), c.end());
	return c.end() == std::unique(c.begin(), c.end());
}

// ---------------------------------------------------------------------------------------------------------------------------------

/**
 *	@brief	Make an stl string lowercase.
 *
 *	@param	str the string to make lowercase.
 */
inline void StringToLower(std::string &str)
{
	for (size_t i = 0, size = str.length(); i < size; ++i)
		str[i] = ::tolower(str[i]);
}

// ---------------------------------------------------------------------------------------------------------------------------------

/**
 *	@brief	Make an stl string uppercase.
 *
 *	@param	str the string to make uppercase.
 */
inline void StringToUpper(std::string &str)
{
	for (size_t i = 0, size = str.length(); i < size; ++i)
		str[i] = ::toupper(str[i]);
}

// ---------------------------------------------------------------------------------------------------------------------------------

/**
 *	@brief	Find the iterator to an item in a map, or the point at which it can be inserted.
 *
 *	@param	map the map to search.
 *	@param	key key value to search for.
 *	@param	iter reference to an iterator for the map.
 *
 *	@retval	true \a iter refers to the entry with key value \a key.
 *	@retval	false \a iter refers to the location at which an insert should be done.
 */
template<typename MAP_TYPE, typename KEY_TYPE>
inline bool MapFind(MAP_TYPE &map, const KEY_TYPE &key, typename MAP_TYPE::iterator &iter)
{
	// returning bool and taking iterator reference is the fastest implementation (faster than
	// returning a std::pair<iter, bool> or taking std::pair<iter, bool> by reference) at least with VC++
	iter = map.lower_bound(key);
	
	// if lower_bound() doesn't return end(), it returns an iterator to an item that is
	// !map.key_comp()(iter->first, key), so to check for equivalence it is only necessary to
	// check for !map.key_comp()(key, iter->first).
	return ((iter != map.end()) && !map.key_comp()(key, iter->first));
}

// ---------------------------------------------------------------------------------------------------------------------------------

/**
 *	@brief	Insert a (key, value) pair into a map.
 *
 *	The std::map insert() method can incur the const of construction of the value_type parameter. If
 *	the mapped_type for the map is non-trivial, this can add unnecessary overhead if keys are always
 *	inserted into the map with the same, "default", mapped value.
 *
 *	Using this helper function, it is possible to reuse an existing value_type value to save this construction overhead.
 *	Example usage:
 *
 *		typdef std::map<std::string, NonTrivialType> MyMap;
 *
 *		MyMap::value_type	m_defaultMapValue;	// member of a class, for instance
 *		MyMap				m_map;				// ditto
 *
 *	In some function:
 *		std::pair<MyMap::iterator, bool> res = MapInsert(m_map, "Hello", m_defaultMapValue);
 *
 *	@param	map the map to insert into.
 *	@param	key the key to insert.
 *	@param	reference to a value_type containing the value to insert into the map with key \a key.
 *
 *	@return	A pair, the first member is an iterator to the inserted or found element. The second member
 *			is true if an insert took place, or false if \a key was already in the map.
 */
template<typename MAP_TYPE, typename KEY_TYPE>
inline std::pair<typename MAP_TYPE::iterator, bool> MapInsert(MAP_TYPE &map, const KEY_TYPE &key, typename MAP_TYPE::value_type &value)
{
	const_cast<typename MAP_TYPE::key_type &>(value.first) = key;

	return map.insert(value);
}

// ---------------------------------------------------------------------------------------------------------------------------------

/**
 *	@brief	Insert a (key, value) pair into a map.
 *
 *	The std::map insert() method can incur the const of construction of the value_type parameter. If
 *	the mapped_type for the map is non-trivial, this can add unnecessary overhead if keys are always
 *	inserted into the map with the same, "default", mapped value.
 *
 *	Using this helper function, it is possible to reuse an existing value_type value to save this construction overhead.
 *	Example usage:
 *
 *		typdef std::map<std::string, NonTrivialType> MyMap;
 *
 *		MyMap::value_type	m_defaultMapValue;	// member of a class, for instance
 *		MyMap				m_map;				// ditto
 *
 *	In some function:
 *		std::string key("Hello");
 *		MyMap::iterator iter;
 *
 *		if (!MapFind(m_map, key, iter))
 *			iter = MapInsert(m_map, iter, key, m_defaultMapValue);
 *
 *	@param	map the map to insert into.
 *	@param	hint an iterator hinting at where to start the insert.
 *	@param	key the key to insert.
 *	@param	reference to a value_type containing the value to insert into the map with key \a key.
 *
 *	@return	Iterator to the inserted item.
 */
template<typename MAP_TYPE, typename KEY_TYPE>
inline typename MAP_TYPE::iterator MapInsert(MAP_TYPE &map, typename MAP_TYPE::iterator hint, const KEY_TYPE &key, typename MAP_TYPE::value_type &value)
{
	const_cast<typename MAP_TYPE::key_type &>(value.first) = key;

	return map.insert(hint, value);
}

} // namespace Activ

#endif // !defined (ACTIV_STL_HELPER_H)

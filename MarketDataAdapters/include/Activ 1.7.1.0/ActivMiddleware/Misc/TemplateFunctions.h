/**
 *	@file	TemplateFunctions.h
 *
 *	@brief	Header file for miscellaneous template functions with no other home.
 *
 *	$Log: $
 */

#if (!defined ACTIV_MISC_TEMPLATE_FUNCTIONS_H)
#define ACTIV_MISC_TEMPLATE_FUNCTIONS_H

#include "ActivMiddleware/Activ.h"

#include <string>

namespace Activ
{

/**
 *	@brief	Template function for returning a string from a table.
 *
 *	@param	index index into the table of strings @a array.
 *	@param	array table of c strings.
 *	@param	arraySize number of entries in @a array.
 *
 *	@return	The entry at index @a index, or if out of bounds, "unknown (0x<index>").
 */
template <typename T>
std::string IndexToString(const T index, const char * const array[], const size_t arraySize)
{
	const size_t actualIndex = static_cast<size_t>(index);

	if (actualIndex < arraySize)
		return array[actualIndex];

	return std::string("unknown index ") + TypeToString(index);
}

} // namespace Activ

#endif // (!defined ACTIV_MISC_TEMPLATE_FUNCTIONS_H)

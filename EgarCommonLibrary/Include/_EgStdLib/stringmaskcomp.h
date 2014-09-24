#ifndef __EGSTDLIB_STRING_MASK_COMPARISION_H__
#define __EGSTDLIB_STRING_MASK_COMPARISION_H__

#include <EgStdLib\EgStdLibCommon.h>

namespace EgStd
{

/********************************************************************************
	Routine:
		CompareStringByMask

		Case sensitive comparition by mask.

	Params:
		szString	- string where mask is applied
		sdMask		- applying mask

	Description:
		Symbols in a mask string replaces symbol(s) in an input string by the 
		following conditions.
		
		'?' - any one symbol in the input string
		'*' - any count (zero included) of symbols sequence

		If input string is empty but mask in not function returns false 
		independed from mask content.
	
		If input string and mask are empty the routines always returns true.
********************************************************************************/
BOOL EGSTD_EXT_FUNC CompareStringByMask(LPCTSTR szString, LPCTSTR szMask);

}

#endif //__EGSTDLIB_STRING_MASK_COMPARISION_H__
#include "stdafx.h"

/**************************************************************************************
	
	Copyright (C) Eugene Karnygin, 2002
	
	String extention routines
	
	File:	StringMaskComp.cpp
	Author: Eugene Karnygin
	Date:	6 feb 2002
**************************************************************************************/

#include <string.h>

bool CompareStringByMask(LPCTSTR szString, LPCTSTR szMask)
{
	bool IsMatch = true;

	while (IsMatch && *szString && *szMask && _tcscmp(szString, szMask) != 0)
	{
		// What is the mask symbol ?
		switch (*szMask)	
		{
		default:	
		// This is odinary symbol. 
			IsMatch = (*szString == *szMask); // Check it matched with currenct string symbol

		case _T('?'): 
		// This is '?' (any one symbol in the string)
			szMask++;	// Skip one symbol of the mask
			szString++;	// Skip one symbol of the string
			break;

		case _T('*'): 
		// This is '*' any (zero count included)
			szMask++;	// Skip '*' symbol

			// The next code row checks : 
			// if nothing more in the mask or 
			// estimated symbols in the mask and in the string are equal sequence
			// then it matched
			IsMatch = (*szMask == _T('\0') || _tcscmp(szString, szMask) == 0);

			while (!IsMatch && *szString) // while not matched or string has symbols
			{
				IsMatch = CompareStringByMask(szString, szMask); // recursive call of the function

				if (!IsMatch)	// If not matched
					szString++;	// skip one more symbol from string ('*' eats it)
			}
			
			// There is string and mask matched
			szString += _tcslen(szString);	// go to end of string
			szMask += _tcslen(szMask);		// go to end of mask 
			break;
		}
	}

	return (IsMatch && _tcscmp(szString, szMask) == 0);
}


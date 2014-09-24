/**************************************************************************************
	
	Copyright (C) Eugene Karnygin, 2002
	
	String extention routines
	
	File:	StringMaskComp.h
	Author: Eugene Karnygin
	Date:	6 feb 2002
**************************************************************************************/

#ifndef __STRING_MASK_COMP_H__
#define __STRING_MASK_COMP_H__

/********************************************************************************
	Routine:
		CompareStringByMask

		Case sensitive comparation by mask.

	Params:
		szString	- string that mask is applied
		sdMask		- applying mask

	Comparation rules:
		Symbols in the mask replaces symbol(s) in the input string by the 
		following conditions.
		
		'?' - any one symbol in the input string
		'*' - any count (zero included) of symbols sequence

	NOTICE:
		1)	If input string is empty but mask in not 
			function returns false independed from mask content.
		2)	If input string and mask is empty it returns true.
********************************************************************************/
bool CompareStringByMask(LPCTSTR szString, LPCTSTR szMask);

#endif	__STRING_MASK_COMP_H__
// stdafx.cpp : source file that includes just the standard includes
//  stdafx.pch will be the pre-compiled header
//  stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"

IMPLEMENT_PRICETRACE()

long _bstr_cmp(const _bstr_t& cmp1, const _bstr_t& cmp2)
{
	long nRes = 0;

	if (cmp1.length() && cmp2.length())
	{
		nRes = wcscmp(cmp1, cmp2);
	}
	else if (cmp1.length() == 0)
	{
		if (cmp2.length() == 0)
		{
			nRes = 0;
		}
		else
		{
			nRes = -1;
		}
	}
	else
	{
		nRes = 1;
	}

	return nRes;
}


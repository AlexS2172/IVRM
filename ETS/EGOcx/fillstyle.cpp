// Machine generated IDispatch wrapper class(es) created by Microsoft Visual C++

// NOTE: Do not modify the contents of this file.  If this class is regenerated by
//  Microsoft Visual C++, your modifications will be overwritten.


#include "stdafx.h"
#include "fillstyle.h"

// Dispatch interfaces referenced by this interface
#include "interior.h"


/////////////////////////////////////////////////////////////////////////////
// CFillStyle properties

long CFillStyle::GetPattern()
{
	long result;
	GetProperty(0x1, VT_I4, (void*)&result);
	return result;
}

void CFillStyle::SetPattern(long propVal)
{
	SetProperty(0x1, VT_I4, propVal);
}

LPDISPATCH CFillStyle::GetParent()
{
	LPDISPATCH result;
	GetProperty(0x3, VT_DISPATCH, (void*)&result);
	return result;
}

CInterior CFillStyle::GetInterior()
{
	LPDISPATCH pDispatch;
	GetProperty(0x4, VT_DISPATCH, (void*)&pDispatch);
	return CInterior(pDispatch);
}

BOOL CFillStyle::GetIsDefault()
{
	BOOL result;
	GetProperty(0x5, VT_BOOL, (void*)&result);
	return result;
}

void CFillStyle::SetIsDefault(BOOL propVal)
{
	SetProperty(0x5, VT_BOOL, propVal);
}

/////////////////////////////////////////////////////////////////////////////
// CFillStyle operations
// MmTradeMatchAtom.cpp : Implementation of CMmTradeMatchAtom

#include "stdafx.h"
#include "MmTradeMatchAtom.h"


// CMmTradeMatchAtom

STDMETHODIMP CMmTradeMatchAtom::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* arr[] = 
	{
		&IID_IMmTradeMatchAtom
	};

	for (int i=0; i < sizeof(arr) / sizeof(arr[0]); i++)
	{
		if (InlineIsEqualGUID(*arr[i],riid))
			return S_OK;
	}
	return S_FALSE;
}

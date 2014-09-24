// ActiveFuturesChange.cpp : Implementation of CActiveFuturesChange

#include "stdafx.h"
#include "ActiveFuturesChange.h"


// CActiveFuturesChange

STDMETHODIMP CActiveFuturesChange::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* arr[] = 
	{
		&IID_IActiveFuturesChange
	};

	for (int i=0; i < sizeof(arr) / sizeof(arr[0]); i++)
	{
		if (InlineIsEqualGUID(*arr[i],riid))
			return S_OK;
	}
	return S_FALSE;
}

// UnderlyingUpdate.cpp : Implementation of CUnderlyingUpdate

#include "stdafx.h"
#include "UnderlyingUpdate.h"


// CUnderlyingUpdate

STDMETHODIMP CUnderlyingUpdate::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* arr[] = 
	{
		&IID_IUnderlyingUpdate
	};

	for (int i=0; i < sizeof(arr) / sizeof(arr[0]); i++)
	{
		if (InlineIsEqualGUID(*arr[i],riid))
			return S_OK;
	}
	return S_FALSE;
}

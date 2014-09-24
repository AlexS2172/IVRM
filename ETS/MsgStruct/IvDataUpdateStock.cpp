// IvDataUpdateStock.cpp : Implementation of CIvDataUpdateStock

#include "stdafx.h"
#include "IvDataUpdateStock.h"


// CIvDataUpdateStock

STDMETHODIMP CIvDataUpdateStock::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* arr[] = 
	{
		&IID_IIvDataUpdateStock
	};

	for (int i=0; i < sizeof(arr) / sizeof(arr[0]); i++)
	{
		if (InlineIsEqualGUID(*arr[i],riid))
			return S_OK;
	}
	return S_FALSE;
}

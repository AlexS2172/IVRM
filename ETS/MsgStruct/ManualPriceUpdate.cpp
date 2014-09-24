// ManualPriceUpdate.cpp : Implementation of CManualPriceUpdate

#include "stdafx.h"
#include "ManualPriceUpdate.h"


// CManualPriceUpdate
STDMETHODIMP CManualPriceUpdate::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* arr[] = 
	{
		&IID_IPriceUpdate
	};

	for (int i=0; i < sizeof(arr) / sizeof(arr[0]); i++)
	{
		if (InlineIsEqualGUID(*arr[i],riid))
			return S_OK;
	}
	return S_FALSE;
}

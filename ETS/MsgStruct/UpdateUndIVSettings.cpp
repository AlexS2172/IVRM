// UpdateUndIVSettings.cpp : Implementation of CUpdateUndIVSettings

#include "stdafx.h"
#include "UpdateUndIVSettings.h"


// CUpdateUndIVSettings

STDMETHODIMP CUpdateUndIVSettings::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* arr[] = 
	{
		&IID_IUpdateUndIVSettings
	};

	for (int i=0; i < sizeof(arr) / sizeof(arr[0]); i++)
	{
		if (InlineIsEqualGUID(*arr[i],riid))
			return S_OK;
	}
	return S_FALSE;
}

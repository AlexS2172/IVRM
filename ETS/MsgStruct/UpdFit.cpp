// UpdFit.cpp : Implementation of CUpdFit

#include "stdafx.h"
#include "UpdFit.h"


// CUpdFit

STDMETHODIMP CUpdFit::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* arr[] = 
	{
		&IID_IUpdFit
	};

	for (int i=0; i < sizeof(arr) / sizeof(arr[0]); i++)
	{
		if (InlineIsEqualGUID(*arr[i],riid))
			return S_OK;
	}
	return S_FALSE;
}

// FlexOption.cpp : Implementation of CFlexOption

#include "stdafx.h"
#include "MsgStruct.h"
#include "FlexOption.h"


// CFlexOption

STDMETHODIMP CFlexOption::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* arr[] = 
	{
		&IID_IFlexOption
	};

	for (int i=0; i < sizeof(arr) / sizeof(arr[0]); i++)
	{
		if (InlineIsEqualGUID(*arr[i],riid))
			return S_OK;
	}
	return S_FALSE;
}

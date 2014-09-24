// EtsCustDivAtom.cpp : Implementation of CEtsCustDivAtom

#include "stdafx.h"
#include "EtsCustDivAtom.h"


// CEtsCustDivAtom

STDMETHODIMP CEtsCustDivAtom::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* arr[] = 
	{
		&IID_IEtsCustDivAtom
	};

	for (int i=0; i < sizeof(arr) / sizeof(arr[0]); i++)
	{
		if (InlineIsEqualGUID(*arr[i],riid))
			return S_OK;
	}
	return S_FALSE;
}

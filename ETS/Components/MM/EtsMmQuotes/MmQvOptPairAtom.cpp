// MmQvOptPairAtom.cpp : Implementation of CMmQvOptPairAtom

#include "stdafx.h"
#include "MmQvOptPairAtom.h"

/////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CMmQvOptPairAtom::get_Opt(EtsOptionTypeEnum enOptType, IMmQvOptAtom** pVal)
{
	if(!pVal)
	{
		return E_POINTER;
	}

	if (enOptType != enOtPut && enOptType != enOtCall)
		return E_INVALIDARG;

	if(*pVal)
	{
		(*pVal)->Release();
		*pVal = NULL;
	}

	LONG nIndex = (enOptType != enOtPut ? 1L : 0L);
	ObjectLock lock(this);
	*pVal = (IMmQvOptAtom*)m_spOpt[nIndex];
	if(*pVal)
	{
		(*pVal)->AddRef();
	}
	return S_OK;
}													

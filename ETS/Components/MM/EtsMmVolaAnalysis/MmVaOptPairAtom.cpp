// MmVaOptPairAtom.cpp : Implementation of CMmVaOptPairAtom

#include "stdafx.h"
#include "MmVaOptPairAtom.h"

/////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CMmVaOptPairAtom::get_Opt(EtsOptionTypeEnum enOptType, IMmVaOptAtom** pVal)
{
	if(!pVal)
	{
		return E_POINTER;
	}

	if(*pVal)
	{
		(*pVal)->Release();
		*pVal = NULL;
	}

	LONG nIndex = (enOptType != enOtPut ? 1L : 0L);
	ObjectLock lock(this);
	*pVal = (IMmVaOptAtom*)m_spOpt[nIndex];
	if(*pVal)
	{
		(*pVal)->AddRef();
	}
	return S_OK;
}													

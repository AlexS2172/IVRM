// MmRvGroupingAtom.cpp : Implementation of CMmRvGroupingAtom

#include "stdafx.h"
#include "MmRvGroupingAtom.h"

STDMETHODIMP CMmRvGroupingAtom::CalcCV(void)
{
	try
	{
		if ( m_spRTContext != NULL && m_nPoolID != BAD_LONG_VALUE ) 
			_CHK(m_spRTContext->Recalculate(m_nPoolID));
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_IMmRvGroupingAtom, e.Error());
	}

	return S_OK;
}


// IcvDataProvider
STDMETHODIMP CMmRvGroupingAtom::raw_GetData(LONG lDataID, VARIANT* Value)
{
	return E_NOTIMPL;
}

STDMETHODIMP CMmRvGroupingAtom::raw_CallFunction(LONG lFunctionID, SAFEARRAY** arrParameters, VARIANT *Value)
{
	return E_NOTIMPL;
}

STDMETHODIMP CMmRvGroupingAtom::raw_Check(SAFEARRAY **arrSysVars, SAFEARRAY **arrSysFuncs, LONG *pRetVal)
{
	return E_NOTIMPL;
}
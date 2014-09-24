// MmQvFutRootAtom.cpp : Implementation of CMmQvFutRootAtom

#include "stdafx.h"
#include "MmQvFutRootAtom.h"

/////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CMmQvFutRootAtom::CleanUp()
{
	try
	{
		m_nID = 0L;
		m_bstrSymbol.Empty();
		m_bstrName.Empty();
		m_nMatCalendarID = 0L;
		m_nFutLotSize = 0L;
		m_nOptLotSize = 0L;
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)CComErrorWrapper::ErrorDescription(e), IID_IMmQvFutRootAtom, e.Error());
	}
	return S_OK;
}

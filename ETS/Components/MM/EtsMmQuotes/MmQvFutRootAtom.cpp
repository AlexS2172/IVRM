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
		m_bstrSymbol = _bstr_t();
		m_bstrName= _bstr_t();
		m_nFutLotSize = 0L;
		m_nOptLotSize = 0L;
		m_enQuotationUnitID = enMmQuNone;
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)CComErrorWrapper::ErrorDescription(e), IID_IMmQvFutRootAtom, e.Error());
	}
	return S_OK;
}

double CMmQvFutRootAtom::GetDeltaGammaKoeff()
{
	double dK = 1.;
	if(m_dTickValue > 0 && m_dTickSize > 0 && m_dPriceQuotationUnit > 0)
		dK = m_dTickValue / (m_dTickSize * m_dPriceQuotationUnit * ((m_lQuoteFormat==1)?100.0:1.0));
	return dK;
}

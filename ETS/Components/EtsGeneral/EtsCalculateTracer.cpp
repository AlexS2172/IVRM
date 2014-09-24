// EtsCalculateTracer.cpp : Implementation of CEtsCalculateTracer

#include "stdafx.h"
//#include "EtsCalculateTracer.h"


// CEtsCalculateTracer

STDMETHODIMP CEtsCalculateTracer::Trace (/*[in]*/EtsLogLevels lLogLevel, /*[in]*/BSTR sCategory, /*[in]*/BSTR sMsg)
{
	_bstr_t bstrCategory (sCategory);
	_bstr_t bstrMsg (sMsg);

	if (m_pTrace)
		m_pTrace->Trace(lLogLevel, bstrCategory, bstrMsg, NULL);

	_ASSERT (m_pTrace);
	return S_OK;
}


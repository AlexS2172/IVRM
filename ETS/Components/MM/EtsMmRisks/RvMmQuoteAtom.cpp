// RvMmQuoteAtom.cpp : Implementation of CRvMmQuoteAtom

#include "stdafx.h"
#include "RvMmQuoteAtom.h"


// CRvMmQuoteAtom

STDMETHODIMP CRvMmQuoteAtom::put_IsDirty(VARIANT_BOOL bDirty)
{
	HRESULT hr = S_OK;
	
	m_bDirty = bDirty;
	if(m_pPrice)
		m_pPrice->m_bDirty = bDirty;
	if(m_pSuPrice)
		m_pSuPrice->m_bDirty = bDirty;

	return hr;
}
STDMETHODIMP CRvMmQuoteAtom::get_IsDirty(VARIANT_BOOL* pDirty)
{
	HRESULT hr = S_OK;
	if(!pDirty)
		return E_POINTER;
	*pDirty = IsDirty()?VARIANT_TRUE:VARIANT_FALSE;
	return hr;
}

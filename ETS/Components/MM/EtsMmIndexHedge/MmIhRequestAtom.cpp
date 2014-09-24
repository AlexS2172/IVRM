// MmIhRequestAtom.cpp : Implementation of CMmIhRequestAtom

#include "stdafx.h"
#include "MmIhRequestAtom.h"
#include ".\mmihrequestatom.h"


// CMmIhRequestAtom


STDMETHODIMP CMmIhRequestAtom::get_Und2(LONG UndIndex, IMmIhUndAtom** pVal)
{
	if(UndIndex >2 || UndIndex<1)
		return _ERRSET(E_FAIL, L"IMmIhRequestAtom", L"Wrong index value"); 
	if(!pVal)
		return E_POINTER;
	*pVal = m_spUnd2[UndIndex-1];
	if(*pVal)
	{
		(*pVal)->AddRef();
		return S_OK;
	}
	else
		return E_POINTER;
}

STDMETHODIMP CMmIhRequestAtom::putref_Und2(LONG UndIndex, IMmIhUndAtom* newVal)
{
	if(UndIndex >2 || UndIndex<1)
		return _ERRSET(E_FAIL, L"IMmIhRequestAtom", L"Wrong index value"); 

	m_spUnd2[UndIndex-1] = newVal;
	return S_OK;
}

STDMETHODIMP CMmIhRequestAtom::GetQuoteUpdateParam(QuoteUpdateParams* pRetVal)
{
	HRESULT hr = S_OK;
	__CHECK_POINTER(pRetVal);

	try
	{
		if(pRetVal->Exchange)
			::SysFreeString(pRetVal->Exchange);
		if(pRetVal->Symbol)
			::SysFreeString(pRetVal->Symbol);

		EtsContractTypeEnum enContractType = enCtNone;
		if(m_spOptAtom !=NULL)
		{
			pRetVal->Type = enOPT;
			m_spOptAtom->get_Symbol(&pRetVal->Symbol);
		}else
		if(m_spUndAtom != NULL)
		{
			pRetVal->Type = enSTK;
			m_spUndAtom->get_Symbol(&pRetVal->Symbol);
		}else
		if(m_spUnd2[0] != NULL)
		{
			pRetVal->Type = enSTK;
			m_spUnd2[0]->get_Symbol(&pRetVal->Symbol);
		}else
		if(m_spUnd2[1] != NULL)
		{
			pRetVal->Type = enSTK;
			m_spUnd2[1]->get_Symbol(&pRetVal->Symbol);
		}else
		if(m_spIndexAtom != NULL)	
		{
			pRetVal->Type = enIDX;
			m_spIndexAtom->get_Symbol(&pRetVal->Symbol);
		}else
            hr = _ERRSET(E_FAIL, L"IMmIhRequestAtom", L"m_spOpt and m_spUnd is NULL at ");
	}
	_CATCH_COM_EXCEPTION_RETURN_COM(L"Method failed")
	_CATCH_UNHANDLED_EXCEPTION_RETURN_COM;

	return hr;
}

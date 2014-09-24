// MmQvRequestAtom.cpp : Implementation of CMmQvRequestAtom

#include "stdafx.h"
#include "MmQvRequestAtom.h"


// CMmQvRequestAtom
STDMETHODIMP CMmQvRequestAtom::get_QuoteUpdateParam(struct QuoteUpdateParams* pQuoteUpdate)
{
	HRESULT hr = S_OK;
	if(!pQuoteUpdate)
		return E_POINTER;

	try
	{
		m_spExch->get_Code(&pQuoteUpdate->Exchange);
		if(m_spFut == NULL)
		{
			if(m_spOpt != NULL)
			{
				pQuoteUpdate->Type = enOPT;
				m_spOpt->get_Symbol(&pQuoteUpdate->Symbol);
			}
			else
			{
				EtsContractTypeEnum enContractType;
				m_spUnd->get_UndType(&enContractType);
				pQuoteUpdate->Type =  enContractType == enCtStock? enSTK: enIDX;
				m_spUnd->get_Symbol(&pQuoteUpdate->Symbol);
			}
		}
		else
		{
			if(m_spOpt != NULL)
			{
				pQuoteUpdate->Type = enFOP;
				m_spOpt->get_Symbol(&pQuoteUpdate->Symbol);
			}
			else
			{
				pQuoteUpdate->Type = enFUT;
				m_spFut->get_Symbol(&pQuoteUpdate->Symbol);
			}
		}
	}
	catch(_com_error& e)
	{
		hr =  Error((PTCHAR)CComErrorWrapper::ErrorDescription(e), IID_IMmQvRequestAtom, e.Error());
	}
	return hr;
}

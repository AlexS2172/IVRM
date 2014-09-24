// MmShRequestAtom.cpp : Implementation of CMmShRequestAtom

#include "stdafx.h"
#include "MmShRequestAtom.h"


// CMmShRequestAtom
STDMETHODIMP CMmShRequestAtom::GetQuoteUpdateParam(QuoteUpdateParams* pRetVal)
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

		if(m_spUndAtom != NULL)
		{
			_CHK(m_spUndAtom->get_UndType(&enContractType), _T("Fail to underlying contract type."));
			pRetVal->Type = _ContractTypeToPpInstrumentType(enContractType);
			_CHK(m_spUndAtom->get_Symbol(&pRetVal->Symbol), _T("Fail to underlying symbol."));
		}
		else
			if(m_spOptAtom != NULL)
			{
				pRetVal->Type =	enOPT;
				_CHK(m_spOptAtom->get_Symbol(&pRetVal->Symbol), _T("Fail to underlying symbol."));
			}
			else
				hr = _ERRSET(E_FAIL, L"IMmShRequestAtom", L"m_spOpt and m_spUnd is NULL at ");
	}
	_CATCH_COM_EXCEPTION_RETURN_COM(L"Method failed")
	_CATCH_UNHANDLED_EXCEPTION_RETURN_COM;

	return hr;

}

InstrumentTypeEnum CMmShRequestAtom::_ContractTypeToPpInstrumentType(EtsContractTypeEnum enContractType)
{
	switch(enContractType)
	{
	case enCtOption:
		return enOPT;

	case enCtStock:
		return enSTK;

	case enCtIndex:
		return enIDX;

	case enCtFuture:
		return enFUT;

	case enCtFutOption:
		return enFOP;

	default:
		return (InstrumentTypeEnum)0L;
	}
}

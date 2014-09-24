// MmRvReqAtom.cpp : Implementation of CMmRvReqAtom
#include "stdafx.h"
#include "MmRvReqAtom.h"

/////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CMmRvReqAtom::GetQuoteUpdateParam(QuoteUpdateParams* pRetVal)
{
	HRESULT hr = S_OK;

	__CHECK_POINTER(pRetVal);
	
	try
	{
		::SysFreeString(pRetVal->Exchange);
		::SysFreeString(pRetVal->Symbol);
		EtsContractTypeEnum enContractType = enCtNone;

		if(m_spPos != NULL)
		{
			_CHK(m_spPos->get_ContractType(&enContractType), _T("Fail to position contract type."));
			pRetVal->Type = _ContractTypeToPpInstrumentType(enContractType);
			_CHK(m_spPos->get_Symbol(&pRetVal->Symbol), _T("Fail to position symbol."));
			if (enContractType==enCtStock||enContractType==enCtIndex)
				_CHK(m_spUnd->get_PrimaryExchangeCode(&pRetVal->Exchange), _T("Fail to get underlying PrimExch code."));
		}
		else if(m_spFut != NULL)
		{
			pRetVal->Type = _ContractTypeToPpInstrumentType(enCtFuture);
			_CHK(m_spFut->get_Symbol(&pRetVal->Symbol), _T("Fail to futures symbol."));
		}
		else if(m_spUnd != NULL)
		{
			_CHK(m_spUnd->get_ContractType(&enContractType), _T("Fail to underlying contract type."));
			pRetVal->Type = _ContractTypeToPpInstrumentType(enContractType);
			_CHK(m_spUnd->get_Symbol(&pRetVal->Symbol), _T("Fail to underlying symbol."));
			_CHK(m_spUnd->get_PrimaryExchangeCode(&pRetVal->Exchange), _T("Fail to get underlying PrimExch code."));
		}
		else
			hr = _ERRSET(E_FAIL, L"IID_IMmRvReqAtom", L"m_spPos and m_spFut and m_spUnd is NULL at ");
	}
	_CATCH_COM_EXCEPTION_RETURN_COM(L"Method failed")
	_CATCH_UNHANDLED_EXCEPTION_RETURN_COM;

	return hr;
}

/////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CMmRvReqAtom::GetGroupQuoteUpdateParam(QuoteUpdateParams* pRetVal)
{
	HRESULT hr = S_OK;
	__CHECK_POINTER(pRetVal);

	try
	{
		::SysFreeString(pRetVal->Exchange);
		::SysFreeString(pRetVal->Symbol);
		EtsContractTypeEnum enContractType = enCtNone;

		if(m_spUnd != NULL)
		{
			_CHK(m_spUnd->get_ContractType(&enContractType), _T("Fail to underlying contract type."));
			pRetVal->Type = (enContractType == enCtStock ? enGrSTK : enGrIDX);
			_CHK(m_spUnd->get_Symbol(&pRetVal->Symbol), _T("Fail to underlying symbol."));
		}
		else
			hr = _ERRSET(E_FAIL, L"IID_IMmRvReqAtom", L"m_spUnd is NULL at ");

	}
	_CATCH_COM_EXCEPTION_RETURN_COM(L"Method failed")
	_CATCH_UNHANDLED_EXCEPTION_RETURN_COM;

	return hr;
}

/////////////////////////////////////////////////////////////////////////////
//
InstrumentTypeEnum CMmRvReqAtom::_ContractTypeToPpInstrumentType(EtsContractTypeEnum enContractType)
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

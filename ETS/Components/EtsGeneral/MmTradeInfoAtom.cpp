// MmTradeInfoAtom.cpp : Implementation of CMmTradeInfoAtom

#include "stdafx.h"
#include "MmTradeInfoAtom.h"

_variant_t CMmTradeInfoAtom::bsTradeTypeActual(L"Actual");
_variant_t CMmTradeInfoAtom::bsTradeTypeSimulated(L"Simulated");
_variant_t CMmTradeInfoAtom::bsTradeTypeManual(L"Manual");
_variant_t CMmTradeInfoAtom::bsTradeTypeUnknown(L"<Unknown>");
_variant_t CMmTradeInfoAtom::bsEmpty(L"");
_variant_t CMmTradeInfoAtom::bsNA(L"--");


/////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CMmTradeInfoAtom::CopyTo(IMmTradeInfoAtom* pDest)
{
	try
	{
		if(!pDest)
			EgLib::CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Invalid argument"));

		HRESULT hr = S_OK;
		IMmTradeInfoAtomPtr spDest(pDest);

		__CHECK_HRESULT3(spDest->put_TradeID(m_nTradeID));
		__CHECK_HRESULT3(spDest->put_SeqNum(m_nSeqNum));		
		__CHECK_HRESULT3(spDest->put_Price(m_dPrice));	
		__CHECK_HRESULT3(spDest->put_Quantity(m_nQuantity));	
		__CHECK_HRESULT3(spDest->put_IsBuy(m_bIsBuy));	
		__CHECK_HRESULT3(spDest->put_TradeDate(m_dtTradeDate));		
		__CHECK_HRESULT3(spDest->put_SpotReference(m_dSpotReference));
		__CHECK_HRESULT3(spDest->put_TradedIV(m_dTradedIV));
		__CHECK_HRESULT3(spDest->put_IsPosition(m_bIsPosition));
		__CHECK_HRESULT3(spDest->put_Status(m_enStatus));
		__CHECK_HRESULT3(spDest->put_BrokerCommission(m_dBrokerCommission));
		__CHECK_HRESULT3(spDest->put_ClearingBrokerCommission(m_dClearingBrokerCommission));
		__CHECK_HRESULT3(spDest->put_Mark(m_nMark));

		__CHECK_HRESULT3(spDest->putref_Und(m_spUnd));
		__CHECK_HRESULT3(spDest->putref_OptRoot(m_spOptRoot));
		__CHECK_HRESULT3(spDest->putref_Trader(m_spTrader));
		__CHECK_HRESULT3(spDest->putref_Broker(m_spBroker));
		__CHECK_HRESULT3(spDest->putref_ClearingBroker(m_spClearingBroker));
		__CHECK_HRESULT3(spDest->putref_Strategy(m_spStrategy));
		__CHECK_HRESULT3(spDest->putref_FutRoot(m_spFutRoot));
		__CHECK_HRESULT3(spDest->putref_Fut(m_spFut));
		__CHECK_HRESULT3(spDest->putref_Contract(m_spContract));

		if(m_spOpt != NULL)
		{
			IEtsOptAtomPtr spDestOpt;
			__CHECK_HRESULT3(spDest->get_Opt(&spDestOpt));

			if(spDestOpt == NULL)
			{
				__CHECK_HRESULT(spDestOpt.CreateInstance(CLSID_EtsOptAtom), 
					_T("Fail to create option data."));
				__CHECK_HRESULT3(spDest->putref_Opt(spDestOpt));
			}

			__CHECK_HRESULT(m_spOpt->CopyTo(spDestOpt),_T("Fail to copy option data."));
		}
		else
			__CHECK_HRESULT3(spDest->putref_Opt(NULL));

		if(m_spFutOpt != NULL)
		{
			IEtsFutOptAtomPtr spDestFutOpt;
			__CHECK_HRESULT3(spDest->get_FutOpt(&spDestFutOpt));

			if(spDestFutOpt == NULL)
			{
				__CHECK_HRESULT(spDestFutOpt.CreateInstance(CLSID_EtsFutOptAtom), 
					_T("Fail to create futures option data."));
				__CHECK_HRESULT3(spDest->putref_FutOpt(spDestFutOpt));
			}

			__CHECK_HRESULT(m_spFutOpt->CopyTo(spDestFutOpt),_T("Fail to copy futures option data."));
		}
		else
			__CHECK_HRESULT3(spDest->putref_FutOpt(NULL));

	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_IMmTradeInfoAtom, e.Error());
	}

	return S_OK;
}

/////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CMmTradeInfoAtom::GetTradeTypeString(BSTR* pDest)
{
	try
	{
		if(!pDest)
			EgLib::CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Invalid argument"));

		if(*pDest)
			::SysFreeString(*pDest);

		*pDest = TradeTypeString().copy();
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_IMmTradeInfoAtom, e.Error());
	}

	return S_OK;
}

/////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CMmTradeInfoAtom::GetTradeMatchedString(BSTR* pDest)
{
	try	
	{
		if(!pDest)
			EgLib::CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Invalid argument"));

		if(*pDest)
			::SysFreeString(*pDest);

		switch(m_enStatus)
		{
		case enTrsMatchedRealtime:
			*pDest = ::SysAllocString(L"Yes");
			break;

		case enTrsRealtime:
			*pDest = ::SysAllocString(L"No");
			break;

		default:
			*pDest = SysAllocString(L"");
		}
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_IMmTradeInfoAtom, e.Error());
	}

	return S_OK;
}

/////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CMmTradeInfoAtom::get_ContractID(LONG* pRetVal)
{
	try
	{
		HRESULT hr = S_OK;

		if(!pRetVal)
			EgLib::CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Invalid argument"));

		if(m_spOpt != NULL)
		{
			__CHECK_HRESULT3(m_spOpt->get_ID(pRetVal));
		}
		else if(m_spFutOpt != NULL)
		{
			__CHECK_HRESULT3(m_spFutOpt->get_ID(pRetVal));
		}
		else if(m_spFut != NULL)
		{
			__CHECK_HRESULT3(m_spFut->get_ID(pRetVal));
		}
		else if(m_spUnd != NULL)
		{
			__CHECK_HRESULT3(m_spUnd->get_ID(pRetVal));
		}
		else
			*pRetVal = 0;
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_IMmTradeInfoAtom, e.Error());
	}

	return S_OK;
}

/////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CMmTradeInfoAtom::get_ContractType(enum EtsContractTypeEnum* pRetVal)
{
	try
	{
		HRESULT hr = S_OK;

		if(!pRetVal)
			EgLib::CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Invalid argument"));

		if(m_spOpt != NULL)
		{
			*pRetVal = enCtOption;
		}
		else if(m_spFutOpt != NULL)
		{
			*pRetVal = enCtFutOption;
		}
		else if(m_spFut != NULL)
		{
			*pRetVal = enCtFuture;
		}
		else if(m_spUnd != NULL)
		{
			__CHECK_HRESULT3(m_spUnd->get_UndType(pRetVal));
		}
		else
			*pRetVal = static_cast<EtsContractTypeEnum>(0);
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_IMmTradeInfoAtom, e.Error());
	}

	return S_OK;
}

/////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CMmTradeInfoAtom::get_Symbol(BSTR* pRetVal)
{
	try
	{
		HRESULT hr = S_OK;

		if(!pRetVal)
			EgLib::CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Invalid argument"));

		if(*pRetVal)
			::SysFreeString(*pRetVal);

		if(m_spOpt != NULL)
		{
			__CHECK_HRESULT3(m_spOpt->get_Symbol(pRetVal));
		}
		else if(m_spFutOpt != NULL)
		{
			__CHECK_HRESULT3(m_spFutOpt->get_Symbol(pRetVal));
		}
		else if(m_spFut != NULL)
		{
			__CHECK_HRESULT3(m_spFut->get_Symbol(pRetVal));
		}
		else if(m_spUnd != NULL)
		{
			__CHECK_HRESULT3(m_spUnd->get_Symbol(pRetVal));
		}
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_IMmTradeInfoAtom, e.Error());
	}

	return S_OK;
}

/////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CMmTradeInfoAtom::get_UndID(LONG* pRetVal)
{
	try
	{
		HRESULT hr = S_OK;

		if(!pRetVal)
			EgLib::CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Invalid argument"));

		if(m_spUnd != NULL){
			__CHECK_HRESULT3(m_spUnd->get_ID(pRetVal));
		}
		else
			*pRetVal = 0;
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_IMmTradeInfoAtom, e.Error());
	}

	return S_OK;
}

/////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CMmTradeInfoAtom::get_OptRootID(LONG* pRetVal)
{
	try
	{
		HRESULT hr = S_OK;

		if(!pRetVal)
			EgLib::CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Invalid argument"));

		if(m_spOptRoot != NULL)
		{
			__CHECK_HRESULT3(m_spOptRoot->get_ID(pRetVal));
		}
		else
			*pRetVal = 0;
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_IMmTradeInfoAtom, e.Error());
	}

	return S_OK;
}

/////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CMmTradeInfoAtom::get_FutRootID(LONG* pRetVal)
{
	try
	{
		HRESULT hr = S_OK;

		if(!pRetVal)
			EgLib::CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Invalid argument"));

		if(m_spFutRoot != NULL)
		{
			__CHECK_HRESULT3(m_spFutRoot->get_ID(pRetVal));
		}
		else
			*pRetVal = 0;
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_IMmTradeInfoAtom, e.Error());
	}

	return S_OK;
}

/////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CMmTradeInfoAtom::get_FutID(LONG* pRetVal)
{
	try
	{
		HRESULT hr = S_OK;

		if(!pRetVal)
			EgLib::CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Invalid argument"));

		if(m_spFut != NULL)
		{
			__CHECK_HRESULT3(m_spFut->get_ID(pRetVal));
		}
		else
			*pRetVal = 0;
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_IMmTradeInfoAtom, e.Error());
	}

	return S_OK;
}

/////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CMmTradeInfoAtom::get_LotSize(LONG* pRetVal)
{
	try
	{
		HRESULT hr = S_OK;

		if(!pRetVal)
			EgLib::CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Invalid argument"));

		if(m_spOptRoot != NULL)
		{
			__CHECK_HRESULT3(m_spOptRoot->get_LotSize(pRetVal));
		}
		else if(m_spFutRoot != NULL)
		{
			if(m_spFutOpt != NULL)
			{
				LONG nFutLotSize = 0L;
				__CHECK_HRESULT3(m_spFutRoot->get_FutLotSize(&nFutLotSize));
				__CHECK_HRESULT3(m_spFutRoot->get_OptLotSize(pRetVal));
				*pRetVal = *pRetVal * nFutLotSize;
			}
			else if(m_spFut != NULL)
			{
				__CHECK_HRESULT3(m_spFutRoot->get_FutLotSize(pRetVal));
			}
		}
		else if(m_spUnd != NULL)
		{
			__CHECK_HRESULT3(m_spUnd->get_LotSize(pRetVal));
		}
		else
			*pRetVal = 0;
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_IMmTradeInfoAtom, e.Error());
	}

	return S_OK;
}

/////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CMmTradeInfoAtom::get_TotalCommission(DOUBLE* pRetVal)
{
	try
	{
		HRESULT hr = S_OK;

		if(!pRetVal)
			EgLib::CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Invalid argument"));

		*pRetVal = m_dBrokerCommission + m_dClearingBrokerCommission;
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_IMmTradeInfoAtom, e.Error());
	}

	return S_OK;
}

/////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CMmTradeInfoAtom::get_TotalPrice(DOUBLE* pRetVal)
{
	try
	{
		HRESULT hr = S_OK;

		if(!pRetVal)
			EgLib::CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Invalid argument"));

		LONG nLotSize = 0L;
		__CHECK_HRESULT3(get_LotSize(&nLotSize));
		*pRetVal = m_dPrice * m_nQuantity * nLotSize;
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_IMmTradeInfoAtom, e.Error());
	}

	return S_OK;
}

/////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CMmTradeInfoAtom::get_StrategyID(LONG* pRetVal)
{
	try
	{
		HRESULT hr = S_OK;

		if(!pRetVal)
			EgLib::CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Invalid argument"));

		if(m_spStrategy != NULL)
		{
			__CHECK_HRESULT3(m_spStrategy->get_ID(pRetVal));
		}
		else
			*pRetVal = 0;
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_IMmTradeInfoAtom, e.Error());
	}

	return S_OK;
}

/////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CMmTradeInfoAtom::get_ContractName(BSTR* pRetVal)
{
	try
	{
		HRESULT hr = S_OK;

		if(!pRetVal)
			EgLib::CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Invalid argument"));

		if(*pRetVal)
			::SysFreeString(*pRetVal);

		if(m_spOpt != NULL)
		{
			__CHECK_HRESULT3(m_spOpt->get_Series(pRetVal));
		}
		else if(m_spFutOpt != NULL)
		{
			__CHECK_HRESULT3(m_spFutOpt->get_Series(pRetVal));
		}
		else if(m_spFut != NULL)
		{
			__CHECK_HRESULT3(m_spFut->get_ContractName(pRetVal));
		}
		else if(m_spUnd != NULL)
		{
			__CHECK_HRESULT3(m_spUnd->get_ContractName(pRetVal));
		}
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_IMmTradeInfoAtom, e.Error());
	}

	return S_OK;
}

/////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CMmTradeInfoAtom::get_BrokerID(LONG* pRetVal)
{
	try
	{
		HRESULT hr = S_OK;

		if(!pRetVal)
			EgLib::CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Invalid argument"));

		if(m_spBroker != NULL)
		{
			__CHECK_HRESULT3(m_spBroker->get_ID(pRetVal));
		}
		else
			*pRetVal = 0;
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_IMmTradeInfoAtom, e.Error());
	}

	return S_OK;
}

/////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CMmTradeInfoAtom::get_ClearingBrokerID(LONG* pRetVal)
{
	try
	{
		HRESULT hr = S_OK;

		if(!pRetVal)
			EgLib::CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Invalid argument"));

		if(m_spClearingBroker != NULL)
		{
			__CHECK_HRESULT3(m_spClearingBroker->get_ID(pRetVal));
		}
		else
			*pRetVal = 0;
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_IMmTradeInfoAtom, e.Error());
	}

	return S_OK;
}


/////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CMmTradeInfoAtom::get_ImportID(BSTR* pRetVal)
{
	try
	{
		HRESULT hr = S_OK;

		if(!pRetVal)
			EgLib::CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Invalid argument"));

		if(*pRetVal)
			::SysFreeString(*pRetVal);

		if(m_spOpt != NULL)
		{
			__CHECK_HRESULT3(m_spOpt->get_ImportID(pRetVal));
		}
		else if(m_spFutOpt != NULL)
		{
			__CHECK_HRESULT3(m_spFutOpt->get_ImportID(pRetVal));
		}
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_IMmTradeInfoAtom, e.Error());
	}

	return S_OK;
}
HRESULT CMmTradeInfoAtom::Load(EtsContractTypeEnum enType, CClientRecordset& rc)
{
	m_nTradeID    = rc[L"iTradeID"];
	m_dPrice      = rc[L"fPrice"];
	m_nQuantity   = rc[L"iQuantity"];
	m_bIsBuy      = long(rc[L"tiIsBuy"]) != 0 ? VARIANT_TRUE:VARIANT_FALSE;
	m_bIsPosition = long(rc[L"tiIsPosition"]) != 0 ? VARIANT_TRUE:VARIANT_FALSE;

	EgLib::CGMTTimeZone tz;
	m_dtTradeDate = tz.GMTTimeZoneToLocal(rc[L"dtTradeDate"]);

	m_enStatus = static_cast<EtsTradeStatusEnum>(long(rc[L"tiStatus"]));
	m_dBrokerCommission			=	rc[L"fBrokerCommission"];
	m_dClearingBrokerCommission =	rc[L"fClearingBrokerCommission"];
	m_nMark					    =   rc[L"iMark"];
	m_nOrderID					=	rc[L"iOrderID"];
	m_nExecStatus				=	rc[L"iExecStatus"];

	if (static_cast<bool>(m_spMain) == false) 
		EgLib::CComErrorWrapper::ThrowError(E_POINTER, _T("Main is not initialized."));

	CComObject<CUndAtom>* pAsset = 
		dynamic_cast<CComObject<CUndAtom>*>(m_spUnd.GetInterfacePtr());

	switch(enType)
	{
	case enCtOption:
		{
			long lOptionRootID	= rc[L"iOptionRootID"];

			CComObject<CEtsOptRootColl>* pRoots = 
				dynamic_cast<CComObject<CEtsOptRootColl>*>(pAsset->m_spRoots.GetInterfacePtr());
			
			IEtsOptRootAtomPtr spOptionRoot = NULL;
			_CHK(pRoots->get_Item(lOptionRootID, &m_spOptRoot), _T("Fail to get OptionRoot."));

			CComObject<CEtsOptRootAtom>* pOptionRoot = 
				dynamic_cast<CComObject<CEtsOptRootAtom>*>(spOptionRoot.GetInterfacePtr());

			long lContractID			= rc[L"iContractID"];

			//initialize contract's cache
			_CHK(m_spMain->GetContract(lContractID, &m_spContract), _T("Fail to get Contract."));

			m_spOpt.Attach(dynamic_cast<IEtsOptAtom*>(m_spContract.GetInterfacePtr()), true);
		}
		break;
	case enCtFuture:
		{

			_CHK(pAsset->m_spFutRoots->get_Item(rc[L"iFutureRootID"], &m_spFutRoot), _T("Fail to get future root."));			

			CComObject<CEtsFutRootAtom>* pFutureRoot = 
				dynamic_cast<CComObject<CEtsFutRootAtom>*>(m_spFutRoot.GetInterfacePtr());

			//initialize contract's 
			long lFutureID = rc[L"iFutureID"];
			_CHK(m_spMain->GetContract(lFutureID, &m_spContract), _T("Fail to get Contract."));
			m_spFut.Attach(dynamic_cast<IEtsFutAtom*>(m_spContract.GetInterfacePtr()), true);


		}break;
	case enCtFutOption:
		{
			long lFutureRootID			= rc[L"iFutureRootID"];
			long lFutureID				= rc[L"iFutureID"];

			_CHK(m_spMain->GetContract(lFutureID, &m_spContract), _T("Fail to get Contract."));
			m_spFut.Attach(dynamic_cast<IEtsFutAtom*>(m_spContract.GetInterfacePtr()), true);

			_CHK(pAsset->m_spFutRoots->get_Item(lFutureRootID, &m_spFutRoot), _T("Fail to get future root."));			

			CComObject<CEtsFutRootAtom>* pFutureRoot = 
				dynamic_cast<CComObject<CEtsFutRootAtom>*>(m_spFutRoot.GetInterfacePtr());

			/*_CHK(pFutureRoot->m_spFutures->get_Item(lFutureID, &m_spFut), _T("Fail to get future."));

			CComObject<CEtsFutAtom>* pFuture = 
				dynamic_cast<CComObject<CEtsFutAtom>*>(m_spFut.GetInterfacePtr());*/


			m_dSpotReference			= rc[L"fSpotReference"];
			m_dTradedIV					= rc[L"fTradedIV"];

			//initialize contract's cache
			long lContractID			= rc[L"iContractID"];
			_CHK(m_spMain->GetContract(lContractID, &m_spContract), _T("Fail to get Contract.")); 

			m_spFutOpt.Attach(dynamic_cast<IEtsFutOptAtom*>(m_spContract.GetInterfacePtr()), true);
		}
	case enCtIndex:
	case enCtStock:
		{
			CSpotContract* pEquitySpotContract = 
				dynamic_cast<CSpotContract*>(pAsset->m_spSpot.GetInterfacePtr());

			if(pEquitySpotContract->m_nTotalQtyInShares == BAD_LONG_VALUE)
				pEquitySpotContract->m_nTotalQtyInShares =  m_nQuantity * (m_bIsBuy!=VARIANT_FALSE?1:-1);
			else
				pEquitySpotContract->m_nTotalQtyInShares +=  m_nQuantity * (m_bIsBuy!=VARIANT_FALSE?1:-1);

			if(m_enStatus != enTrsSimulated && pEquitySpotContract->m_dClosePrice >= DBL_EPSILON){
				pEquitySpotContract->m_dUndPosForRates += 
					(m_nQuantity * pEquitySpotContract->m_dClosePrice * (m_bIsBuy!=VARIANT_FALSE?1:-1));
			}

			//initialize contract's cache
			_CHK(m_spMain->GetContract(pAsset->m_nID, &m_spContract), _T("Fail to get Contract.")); 
		} break;
	}
	return S_OK;
}
//----------------------------------------------------------------------------------------------------------------//
STDMETHODIMP CMmTradeInfoAtom::GetField(enum TradesListColumnEnum enField,
										VARIANT_BOOL Compare, 
										VARIANT *pValue, 
										VARIANT_BOOL* HasValue)
{
	if (!pValue || !HasValue)
			return E_POINTER;

	_variant_t varValue;
	bool bHasVale = GetField(enField, varValue, Compare == VARIANT_TRUE ? true : false);

	VariantCopy(pValue, &varValue.GetVARIANT());

	bHasVale == true ? *HasValue = VARIANT_TRUE : *HasValue == VARIANT_FALSE;

	return S_OK;
};
//----------------------------------------------------------------------------------------------------------------//
bool CMmTradeInfoAtom::GetField(TradesListColumnEnum field, _variant_t& vtRet, bool bForCompare)
{
	vtRet = vtMissing;
	switch(field)
	{
	case TLC_KEY:
		vtRet = _variant_t(m_nTradeID);
		break;
	case TLC_TYPE:
		vtRet = TradeTypeString();
		break;
	case TLC_UNDERLYING:
		if(!m_bsUndSymbol.length()){
			m_spUnd->get_Symbol(&m_bsUndSymbol.GetBSTR());
		}
		vtRet = m_bsUndSymbol;
		break;
	case TLC_SYMBOL:
		if(!m_bsTlcSymbol.length())
		{
			CComBSTR bsSymbol;
			if(SUCCEEDED(get_Symbol(&bsSymbol)))
				m_bsTlcSymbol = (BSTR)bsSymbol;
		}
		vtRet = m_bsTlcSymbol;
		break;
	case TLC_OPT_TYPE:
		{
			EtsContractTypeEnum enType; 
			get_ContractType(&enType);
			switch(enType)
			{
			case enCtOption:
				if(m_spOpt != NULL){
					EtsOptionTypeEnum enType;
					m_spOpt->get_OptType(&enType);
					vtRet = enType == enOtCall ? _bstr_t(L"C") : _bstr_t(L"P");
				}
				break;
			case enCtFutOption:
				if(m_spFutOpt != NULL){
					EtsOptionTypeEnum enType;
					m_spFutOpt->get_OptType(&enType);
					vtRet = enType == enOtCall ? _bstr_t(L"C") : _bstr_t(L"P");
				}
				break;
			default:
				vtRet = bsEmpty;
			}
		}break;
	case TLC_DTE:
		{

			EtsContractTypeEnum enType; 
			get_ContractType(&enType);
			DATE dtExpiryOV = -1.;
			bool bIsOptionContract = true;
			vt_date dtTimeToExp = vt_date(0.);

			typedef boost::date_time::local_adjustor<ptime, -5, us_dst> us_eastern;
			ptime	ptNYNow = us_eastern::utc_to_local(second_clock::universal_time());
			tm		tmNYNow = to_tm(ptNYNow);
			vt_date dtToday(tmNYNow.tm_year + 1900,
				tmNYNow.tm_mon + 1,
				tmNYNow.tm_mday,
				tmNYNow.tm_hour,
				tmNYNow.tm_min);

			switch(enType)
			{
			case enCtOption:
				{
					if( m_spOpt != NULL ){
						m_spOpt->get_ExpiryOV(&dtExpiryOV);
						dtTimeToExp = vt_date( dtExpiryOV - (DATE)dtToday > 0 ? dtExpiryOV - (DATE)dtToday : 0. );
					}
				}break;
			case enCtFutOption:
				{
					if( m_spFutOpt!= NULL ){
						m_spFutOpt->get_ExpiryOV(&dtExpiryOV);
						dtTimeToExp = vt_date( dtExpiryOV - (DATE)dtToday > 0 ? dtExpiryOV - (DATE)dtToday : 0. );
					}
				}break;
			default:
				{
					vtRet = bsEmpty;
					bIsOptionContract = false;
				}
			}

			if (bForCompare)
			{
				if ( bIsOptionContract )
					vtRet = dtTimeToExp;
				else
					vtRet = _variant_t(-1.);
			}
			else
			{
				if ( bIsOptionContract )
				{
					//is it option contract
					std::wstring wsFormat;
					wchar_t buffer[1024];

					wsFormat = L"%d day(s) %02d:%02d";

					_snwprintf_s(buffer, sizeof(buffer), wsFormat.c_str(), static_cast<LONG>((DATE)dtTimeToExp), dtTimeToExp.get_hour(), dtTimeToExp.get_minute() );
					vtRet =(wchar_t*)buffer;
				}
				else 
				{
					//stock, index and etc
					vtRet = bsEmpty;
				}
			}
		}break;
	case TLC_EXPIRY:
		{
			EtsContractTypeEnum enType; 
			get_ContractType(&enType);
			switch(enType)
			{
			case enCtOption:
				if(m_spOpt!=NULL){
					DATE dtExpiry = 0.;
					m_spOpt->get_Expiry(&dtExpiry);
					vtRet = vt_date(dtExpiry).GetVtDate();
				}
				break;
			case enCtFutOption:
				{
					if(m_spFutOpt!=NULL){
						DATE dtExpiry = 0.;
						m_spFutOpt->get_Expiry(&dtExpiry);
						vtRet = vt_date(dtExpiry).GetVtDate();
					}
				}break;
			default:
				if(bForCompare)
					vtRet = vt_date().GetVtDate();
				else
					vtRet = bsEmpty;
			}
		}break;
	case TLC_STRIKE:
		{
			EtsContractTypeEnum enType; 
			get_ContractType(&enType);
			switch(enType)
			{
			case enCtOption:
				{
					if(m_spOpt != NULL){
						DOUBLE dStrike = 0.;
						m_spOpt->get_Strike(&dStrike);
						vtRet = dStrike;
					}
				}break;
			case enCtFutOption:
				{
					if(m_spFutOpt != NULL){
						DOUBLE dStrike = 0.;
						m_spFutOpt->get_Strike(&dStrike);
						vtRet = dStrike;
					}
				}break;
			default:
				if(bForCompare)
					vtRet = _variant_t(-1.);
				else
					vtRet = bsEmpty;
			}
		}break;
	case TLC_BS:
		vtRet = _variant_t(m_bIsBuy);
		break;
	case TLC_PRICE:
		if(m_bIsPosition)
		{
			if(bForCompare)
				vtRet =  _variant_t(0.0);
			else
				vtRet =  _variant_t(L"--");
		}
		else
			vtRet = _variant_t(m_dPrice);
		break;
	case TLC_QTY:
		vtRet = _variant_t(m_nQuantity);
		break;
	case TLC_TRADETYPE:
		{
			if(m_bIsPosition)
				vtRet = _variant_t(L"Position");
			else
				vtRet = _variant_t(L"Trade");
		}break;
	case TLC_LOTSIZE:
		{
			long lLotPrice = 0;
			get_LotSize(&lLotPrice);
			vtRet = _variant_t(lLotPrice);
		}
		break;
	case TLC_TOT_PRICE:
		{
			if(m_bIsPosition)
			{
				if(bForCompare)
					vtRet =  _variant_t(0.0);
				else
					vtRet =  _variant_t(L"--");
			}
			else
			{
				double dTotalPrice = 0;
				get_TotalPrice(&dTotalPrice);
				vtRet = _variant_t(dTotalPrice);
			}
		}break;
	case TLC_DATE:
		{
			DATE dtTrade = 0;
			get_TradeDate(&dtTrade);
			vtRet = vt_date(dtTrade).GetVtDate();
		}break;
	case TLC_TRADER:
		{
			CComBSTR bsName;
			m_spTrader->get_Name(&bsName);
			vtRet = bsName;
		}
		break;
	case TLC_STRATEGY:
		{
			if(m_spStrategy != NULL){
				CComBSTR bsName;
				m_spStrategy->get_Name(&bsName);
				vtRet = bsName;
			}
			else
				vtRet = bsEmpty;
		}break;
	case TLC_BROKER:
		{
			if(m_spBroker!=NULL){
				CComBSTR bsName;
				m_spBroker->get_Name(&bsName);
				vtRet = bsName;
			}
			else
				vtRet = bsEmpty;
		}break;
	case TLC_CLR_BROKER:
		{
			if(m_spClearingBroker!=NULL){
				CComBSTR bsName;
				m_spClearingBroker->get_Name(&bsName);
				vtRet = bsName;
			}
			else
				vtRet = bsEmpty;
		}break;
	case TLC_COMMISSION:
		{
			double dTotalCommission = 0;
			get_TotalCommission(&dTotalCommission);
			vtRet = dTotalCommission;
		}break;
	case TLC_MATCHED:
		{
			CComBSTR bsMatched;
			GetTradeMatchedString(&bsMatched);
			vtRet = (BSTR)bsMatched;
		}
		break;
	case TLC_MARKED:
		vtRet = _variant_t(m_nMark);
		break;
	case TLC_FUT_ROOT:
		{
			EtsContractTypeEnum enType; 
			get_ContractType(&enType);
			if((enCtFuture == enType || enCtFutOption == enType) && m_spFutRoot!=NULL){
				CComBSTR bsSymbol;
				m_spFutRoot->get_Symbol(&bsSymbol);
				vtRet = bsSymbol;
			}
			else
				vtRet = bsEmpty;

		}break;
	case TLC_FUTURES:
		{
			EtsContractTypeEnum enType; 
			get_ContractType(&enType);
			if((enCtFuture == enType || enCtFutOption == enType) && m_spFut!=NULL){
				CComBSTR bsSymbol;
				m_spFut->get_Symbol(&bsSymbol);
				vtRet = bsSymbol;
			}
			else
				vtRet = bsEmpty;
		}break;
	case TLC_FUT_MATURITY:
		{
			EtsContractTypeEnum enType; 
			get_ContractType(&enType);
			if((enCtFuture == enType || enCtFutOption == enType) && m_spFut!=NULL){
				DATE dtMaturity;
				m_spFut->get_MaturityDate(&dtMaturity);
				vtRet = vt_date(dtMaturity).GetVtDate(); 
			}
			else
				vtRet = bForCompare?vt_date(0).GetVtDate():bsEmpty;

		}break;
	case TLC_STOCKREFPRICE:
		{
			EtsContractTypeEnum enType; 
			get_ContractType(&enType);
			if(enCtOption == enType || enCtFutOption == enType)
				vtRet = m_dSpotReference; 
			else
			{
				if(bForCompare)
					vtRet = _variant_t(-10000);
				else
					vtRet = bsEmpty;
			}

		}break;
	case TLC_TRADED_IV:
		{
			EtsContractTypeEnum enType; 
			get_ContractType(&enType);
			if(enCtOption == enType || enCtFutOption == enType)
				vtRet = m_dTradedIV*100.; 
			else
			{
				if(bForCompare)
					vtRet = _variant_t(-10000);
				else
					vtRet = bsEmpty;
			}
		}break;
	}
	return utils::HasValue(vtRet);
}

STDMETHODIMP CMmTradeInfoAtom::get_ExecStatusString(BSTR* pRetVal)
{
	try
	{
		if(!pRetVal)
			EgLib::CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Invalid argument"));

		EtsOrderExecutionStatusEnum enExecStatus = static_cast<EtsOrderExecutionStatusEnum>(m_nExecStatus);

		if(*pRetVal)
			::SysFreeString(*pRetVal);

		switch(enExecStatus)
		{
		case enOesNew:
			*pRetVal = ::SysAllocString(L"NEW");
			break;

		case enOesPartiallyFilled:
			*pRetVal = ::SysAllocString(L"PART");
			break;

		case enOesFilled:
			*pRetVal = ::SysAllocString(L"FILL");
			break;

		case enOesDoneForDay:
			*pRetVal = ::SysAllocString(L"DFD");
			break;

		case enOesCanceled:
			*pRetVal = ::SysAllocString(L"CXL");
			break;

		case enOesReplaced:
			*pRetVal = ::SysAllocString(L"CXLR");
			break;

		case enOesPendingCancelReplace:
			*pRetVal = ::SysAllocString(L"@CXLR");
			break;

		case enOesStopped:
			*pRetVal = ::SysAllocString(L"STOP");
			break;

		case enOesRejected:
			*pRetVal = ::SysAllocString(L"RJCT");
			break;

		case enOesSuspended:
			*pRetVal = ::SysAllocString(L"SPND");
			break;

		case enOesPendingNew:
			*pRetVal = ::SysAllocString(L"@NEW");
			break;

		case enOesCalculated:
			*pRetVal = ::SysAllocString(L"CALC");
			break;

		case enOesExpired:
			*pRetVal = ::SysAllocString(L"EXP");
			break;

		default:
			*pRetVal = ::SysAllocString(L"???");
		}
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_IMmTradeInfoAtom, e.Error());
	}

	return S_OK;
}
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
bool CMmTradeInfoAtom::IsToday( bool bAdnSimulated = false) 
{

	if ( !bAdnSimulated  )
	{	
		if(enTrsSimulated == m_enStatus)
			return false;

		if(m_bIsPosition == VARIANT_TRUE)
			return false;
	}

	if(static_cast<int>(m_dtTradeDate) != static_cast<int>((DATE)vt_date::GetCurrentDate()))
		return false;

	return true ;
}
//////////////////////////////////////////////////////////////////////////
bool CMmTradeInfoAtom::IsSimulated () 
{
	if(enTrsSimulated != m_enStatus) 
		return false ;
	return true ;
}
//////////////////////////////////////////////////////////////////////////
bool CMmTradeInfoAtom::IsOpenPos ( bool bAdnSimulated = false) 
{
	if ( !bAdnSimulated  )
	{	
		if(enTrsSimulated == m_enStatus)
			return false ;
	}

	bool bReturn = false;
	if(m_bIsPosition == VARIANT_FALSE)	bReturn = true;

	if ( bReturn )
	{	

		if(static_cast<int>(m_dtTradeDate) == static_cast<int>((DATE)vt_date::GetCurrentDate()))
			return false ;
	}

	return true ;
}
//////////////////////////////////////////////////////////////////////////
HRESULT CMmTradeInfoAtom::CheckByFilter(IEtsFilterData* pFilter, VARIANT_BOOL* pRetVal)
{
	if (pFilter == NULL || pRetVal == NULL) return E_POINTER;

	*pRetVal = VARIANT_TRUE;
	IEtsFilterDataPtr   spFilter(pFilter);

	long lFilterUndID = 0; 
		spFilter->get_Data(enFtSymbol, &lFilterUndID);

	long lFilterTypeID = 0;
		spFilter->get_Data(enFtTrades, &lFilterTypeID);

	long lFilterGroupID = 0;
		spFilter->get_Data(enFtGroups, &lFilterGroupID);

	long lFilterTraderGroupID = 0;
		spFilter->get_Data(enFtTraderGroups, &lFilterTraderGroupID); 

	long lFilterTraderID = 0;
		spFilter->get_Data(enFtTrader, &lFilterTraderID);

	long lFilterStrategyID = 0;
		spFilter->get_Data(enFtStrategy, &lFilterStrategyID);

	long lValue = 0;

	try
	{
		//underlying id check
		if (lFilterUndID > 0)
		{
			_CHK(get_UndID(&lValue), _T("Fail to get UndID"));
			if (lFilterUndID != lValue) {
				*pRetVal = VARIANT_FALSE; return S_OK;
			}
		}
		//trade type check
		if(lFilterTypeID >= 0)
		{
			switch(lFilterTypeID)
			{
			case 0:
				break;
			case 1: // Today's
				{
					if ( !IsToday() ){
						*pRetVal = VARIANT_FALSE; return S_OK;
					}
				}break;
			case 2: // OpenPos
				{
					if ( !IsOpenPos( ) ){
						*pRetVal = VARIANT_FALSE; return S_OK;
					}
				}break;
			case 3: // NetPos
				{
					if(enTrsSimulated == m_enStatus){
						*pRetVal = VARIANT_FALSE; return S_OK;
					}

				}  break;
			case 4: // ' Simulated
				{
					if ( !IsSimulated()){
						*pRetVal = VARIANT_FALSE; return S_OK;
					}
				}   break;
			case 5: // Today's & Sim
				{
					if ( !IsToday( true )  ){
						*pRetVal = VARIANT_FALSE; return S_OK;
					}
				}break;
			case 6: // OpenPos & Sim
				{
					if(static_cast<int>(m_dtTradeDate) == static_cast<int>((DATE)vt_date::GetCurrentDate())){
						*pRetVal = VARIANT_FALSE; return S_OK;
					}

				}break;
			case 7: // Manual
				{

					if(enTrsManual != m_enStatus){
						*pRetVal = VARIANT_FALSE; return S_OK;
					}

				} break;
			case 8: //Manual & Sim
				{
					if(enTrsManual == m_enStatus || enTrsSimulated == m_enStatus)
						break;
					*pRetVal = VARIANT_FALSE; return S_OK;

				}break;
			}
		}
		//Trader field check
		if (lFilterTraderID > 0)
		{
			long lTraderId = 0;
			IEtsTraderAtomPtr spTrader; 
			get_Trader(&spTrader);
			spTrader->get_ID(&lTraderId);

			if (lTraderId != lFilterTraderID) {
				*pRetVal = VARIANT_FALSE; return S_OK;
			}
		}

		//strategy check
		if (lFilterStrategyID > 0)
		{
			LONG lStrategyId = 0;
			get_StrategyID(&lStrategyId);
			if(lStrategyId != (lFilterStrategyID  > 0? lFilterStrategyID: 0)){
				*pRetVal = VARIANT_FALSE; return S_OK;
			}
		}
		//group ... next time

	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_IMmTradeInfoAtom, e.Error());
	}

	return S_OK;
}
//////////////////////////////////////////////////////////////////////////

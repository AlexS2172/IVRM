// UndAtom.cpp : Implementation of CUndAtom

#include "stdafx.h"
#include "UndAtom.h"

STDMETHODIMP CUndAtom::get_TotalQtyInShares(LONG* pRetVal)
{
	try
	{
		if(!pRetVal)
			EgLib::CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Invalid argument"));

		if (static_cast<bool>(m_spSpot))
			_CHK(m_spSpot->get_TotalQtyInShares(pRetVal));
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_IUndAtom, e.Error());
	}

	return S_OK;
};

STDMETHODIMP CUndAtom::put_TotalQtyInShares(LONG NewVal)
{
	try
	{
		if (static_cast<bool>(m_spSpot))
			_CHK(m_spSpot->put_TotalQtyInShares(NewVal));
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_IUndAtom, e.Error());
	}
	return S_OK;
};

STDMETHODIMP CUndAtom::get_bShouldMultOptDeltaEq(VARIANT_BOOL* pRetVal)
{
	try
	{
		if(!pRetVal)
			EgLib::CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Invalid argument"));

		if (static_cast<bool>(m_spSpot))
			_CHK(m_spSpot->get_bShouldMultOptDeltaEq(pRetVal));
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_IUndAtom, e.Error());
	}
	return S_OK;
};

STDMETHODIMP CUndAtom::put_bShouldMultOptDeltaEq(VARIANT_BOOL NewVal)
{
	try
	{
		if (static_cast<bool>(m_spSpot))
			_CHK(m_spSpot->put_bShouldMultOptDeltaEq(NewVal));
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_IUndAtom, e.Error());
	}
	return S_OK;
};

STDMETHODIMP CUndAtom::get_dKOptDeltaEq(DOUBLE* pRetVal)
{
	try
	{
		if(!pRetVal)
			EgLib::CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Invalid argument"));

		if (static_cast<bool>(m_spSpot))
			_CHK(m_spSpot->get_dKOptDeltaEq(pRetVal));
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_IUndAtom, e.Error());
	}
	return S_OK;
};

STDMETHODIMP CUndAtom::put_dKOptDeltaEq(DOUBLE NewVal)
{
	try
	{
		if (static_cast<bool>(m_spSpot))
			_CHK(m_spSpot->put_dKOptDeltaEq(NewVal));
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_IUndAtom, e.Error());
	}
	return S_OK;
};

STDMETHODIMP CUndAtom::get_PriceClose(DOUBLE* pRetVal)
{
	try
	{
		if(!pRetVal)
			EgLib::CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Invalid argument"));

		if (static_cast<bool>(m_spSpot))
			_CHK(m_spSpot->get_ClosePrice(pRetVal));
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_IUndAtom, e.Error());
	}
	return S_OK;
};

STDMETHODIMP CUndAtom::put_PriceClose(DOUBLE NewVal)
{
	try
	{
		if (static_cast<bool>(m_spSpot))
			_CHK(m_spSpot->put_ClosePrice(NewVal));
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_IUndAtom, e.Error());
	}
	return S_OK;
};

STDMETHODIMP CUndAtom::get_PriceTheoClose(DOUBLE* pRetVal)
{
	try
	{
		if(!pRetVal)
			EgLib::CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Invalid argument"));

		if (static_cast<bool>(m_spSpot))
			_CHK(m_spSpot->get_TheoClosePrice(pRetVal));
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_IUndAtom, e.Error());
	}
	return S_OK;
};

STDMETHODIMP CUndAtom::put_PriceTheoClose(DOUBLE NewVal)
{
	try
	{
		if (static_cast<bool>(m_spSpot))
			_CHK(m_spSpot->put_TheoClosePrice(NewVal));
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_IUndAtom, e.Error());
	}
	return S_OK;
};

STDMETHODIMP CUndAtom::get_LotSize(LONG* pRetVal)
{
	try
	{
		if(!pRetVal)
			EgLib::CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Invalid argument"));

		if (static_cast<bool>(m_spSpot))
			_CHK(m_spSpot->get_LotSize(pRetVal));
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_IUndAtom, e.Error());
	}
	return S_OK;
};

STDMETHODIMP CUndAtom::put_LotSize(LONG NewVal)
{
	try
	{
		if (static_cast<bool>(m_spSpot))
			_CHK(m_spSpot->put_LotSize(NewVal));
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_IUndAtom, e.Error());
	}
	return S_OK;
};

STDMETHODIMP CUndAtom::get_Skew(DOUBLE* pRetVal)
{
	try
	{
		if(!pRetVal)
			EgLib::CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Invalid argument"));

		if (static_cast<bool>(m_spSpot))
			_CHK(m_spSpot->get_Skew(pRetVal));
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_IUndAtom, e.Error());
	}
	return S_OK;
};

STDMETHODIMP CUndAtom::put_Skew(DOUBLE NewVal)
{
	try
	{
		if (static_cast<bool>(m_spSpot))
			_CHK(m_spSpot->put_Skew(NewVal));
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_IUndAtom, e.Error());
	}
	return S_OK;
};

STDMETHODIMP CUndAtom::get_Kurt(DOUBLE* pRetVal)
{
	try
	{
		if(!pRetVal)
			EgLib::CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Invalid argument"));

		if (static_cast<bool>(m_spSpot))
			_CHK(m_spSpot->get_Kurt(pRetVal));
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_IUndAtom, e.Error());
	}
	return S_OK;
};

STDMETHODIMP CUndAtom::put_Kurt(DOUBLE NewVal)
{
	try
	{
		if (static_cast<bool>(m_spSpot))
			_CHK(m_spSpot->put_Kurt(NewVal));
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_IUndAtom, e.Error());
	}
	return S_OK;
};

STDMETHODIMP CUndAtom::get_PrimaryExchangeID(LONG* pRetVal)
{
	try
	{
		if(!pRetVal)
			EgLib::CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Invalid argument"));

		if (static_cast<bool>(m_spSpot))
			_CHK(m_spSpot->get_PrimaryExchangeID(pRetVal));
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_IUndAtom, e.Error());
	}
	return S_OK;
};

STDMETHODIMP CUndAtom::put_PrimaryExchangeID(LONG NewVal)
{
	try
	{
		if (static_cast<bool>(m_spSpot))
			_CHK(m_spSpot->put_PrimaryExchangeID(NewVal));
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_IUndAtom, e.Error());
	}
	return S_OK;
};

STDMETHODIMP CUndAtom::get_ManualActivePrice(DOUBLE* pRetVal)
{
	try
	{
		if(!pRetVal)
			EgLib::CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Invalid argument"));

		if (static_cast<bool>(m_spSpot))
			_CHK(m_spSpot->get_ManualPrice(pRetVal));
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_IUndAtom, e.Error());
	}
	return S_OK;
};

STDMETHODIMP CUndAtom::put_ManualActivePrice(DOUBLE NewVal)
{
	try
	{
		if (static_cast<bool>(m_spSpot))
			_CHK(m_spSpot->put_ManualPrice(NewVal));
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_IUndAtom, e.Error());
	}
	return S_OK;
};

STDMETHODIMP CUndAtom::get_PrimaryExchImportID(BSTR* pRetVal)
{
	try
	{
		if(!pRetVal)
			EgLib::CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Invalid argument"));

		if (static_cast<bool>(m_spSpot))
			_CHK(m_spSpot->get_PrimaryExchImportID(pRetVal));
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_IUndAtom, e.Error());
	}
	return S_OK;
};

STDMETHODIMP CUndAtom::put_PrimaryExchImportID(BSTR pNewVal)
{
	try
	{
		if (static_cast<bool>(m_spSpot))
			_CHK(m_spSpot->put_PrimaryExchImportID(pNewVal));
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_IUndAtom, e.Error());
	}
	return S_OK;
};

STDMETHODIMP CUndAtom::get_UndPosForRates(DOUBLE* pRetVal)
{
	try
	{
		if(!pRetVal)
			EgLib::CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Invalid argument"));

		if (static_cast<bool>(m_spSpot))
			_CHK(m_spSpot->get_UndPosForRates(pRetVal));
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_IUndAtom, e.Error());
	}
	return S_OK;
};

STDMETHODIMP CUndAtom::put_UndPosForRates(DOUBLE NewVal)
{
	try
	{
		if (static_cast<bool>(m_spSpot))
			_CHK(m_spSpot->put_UndPosForRates(NewVal));
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_IUndAtom, e.Error());
	}
	return S_OK;
};

//spot contract initialization
STDMETHODIMP CUndAtom::get_Symbol(BSTR* pRetVal)
{
	try
	{
		if(!pRetVal)
			EgLib::CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Invalid argument"));
		
		if(*pRetVal)														
		{																
			::SysFreeString(*pRetVal);										
		}																
		
		ObjectLock lock(this);											
		*pRetVal = m_bstrSymbol.copy();

	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_IUndAtom, e.Error());
	}
	return S_OK;
};

STDMETHODIMP CUndAtom::put_Symbol(BSTR pNewVal)
{
	try
	{
		m_bstrSymbol = pNewVal;
		if (static_cast<bool>(m_spSpot))
			_CHK(m_spSpot->put_Symbol(pNewVal));
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_IUndAtom, e.Error());
	}
	return S_OK;
};

STDMETHODIMP CUndAtom::get_ContractName(BSTR* pRetVal)
{
	try
	{
		if(!pRetVal)
			EgLib::CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Invalid argument"));

		if(*pRetVal)														
		{																
			::SysFreeString(*pRetVal);										
		}																

		ObjectLock lock(this);											
		*pRetVal = m_bstrContractName.copy();

	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_IUndAtom, e.Error());
	}
	return S_OK;
};

STDMETHODIMP CUndAtom::put_ContractName(BSTR pNewVal)
{
	try
	{
		m_bstrContractName = pNewVal;
		if (static_cast<bool>(m_spSpot))
			_CHK(m_spSpot->put_ContractName(pNewVal));
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_IUndAtom, e.Error());
	}
	return S_OK;
};

STDMETHODIMP CUndAtom::get_ID(LONG* pRetVal)
{
	try
	{
		if(!pRetVal)
			EgLib::CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Invalid argument"));
		
		*pRetVal = m_nID;
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_IUndAtom, e.Error());
	}
	return S_OK;
};

STDMETHODIMP CUndAtom::put_ID(LONG NewVal)
{
	try
	{
		m_nID = NewVal;
		if (static_cast<bool>(m_spSpot))
			_CHK(m_spSpot->put_ID(NewVal));
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_IUndAtom, e.Error());
	}
	return S_OK;
};

STDMETHODIMP CUndAtom::get_UndType(EtsContractTypeEnum* pRetVal)
{
	try
	{
		if(!pRetVal)
			EgLib::CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Invalid argument"));

		*pRetVal = m_enUndType;
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_IUndAtom, e.Error());
	}
	return S_OK;
};

STDMETHODIMP CUndAtom::put_UndType(EtsContractTypeEnum NewVal)
{
	try
	{
		m_enUndType = NewVal;
		if (static_cast<bool>(m_spSpot))
			_CHK(m_spSpot->put_ContractType(NewVal));
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_IUndAtom, e.Error());
	}
	return S_OK;
};


// MmGaGroupAtom.cpp : Implementation of CMmGaGroupAtom

#include "stdafx.h"
#include "MmGaGroupAtom.h"


/////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CMmGaGroupAtom::CalcUnderlying(IMmGaUndAtom* aUnd, EtsCalcModelTypeEnum enCalcModel,
											VARIANT_BOOL bUseTheoVolatility, VARIANT_BOOL bUseTheoVolaNoBid, 
											VARIANT_BOOL bUseTheoVolaBadMarket, DOUBLE dUndPriceTolerance, 
											EtsPriceRoundingRuleEnum enPriceRoundingRule,
											ICalculationParametrs* pParams)
{
	if(aUnd == NULL)
	{
		return Error(L"Invalid underlying passed.", IID_IMmGaGroupAtom, E_INVALIDARG);
	}

	try
	{
		IMmGaUndAtomPtr spUnd(aUnd);
		__CHECK_HRESULT(spUnd->CalcPositions(m_spUnd, enCalcModel, bUseTheoVolatility, 
									bUseTheoVolaNoBid, bUseTheoVolaBadMarket, dUndPriceTolerance, 
									enPriceRoundingRule, pParams), _T("Fail to calculate underlying positions."));
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)CComErrorWrapper::ErrorDescription(e), IID_IMmGaGroupAtom, e.Error());
	}

	return S_OK;
}

/////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CMmGaGroupAtom::CalcTotals()
{
	try
	{
		HRESULT hr;

		__MmGaGroupAtom::ClearValues();

		IUnknownPtr spUnk;
		_variant_t varItem;
		ULONG nFetched = 0L;
		IMmGaExpTotalAtomPtr spExpTotal;

		__CHECK_HRESULT(m_spExpiryTotal->get__NewEnum(&spUnk), _T("Fail to get expiry totals collection."));

		IEnumVARIANTPtr spExpTotalEnum(spUnk);
		__CHECK_HRESULT(spExpTotalEnum->Reset(), _T("Fail to reset expiry totals collection."));
		while((hr = spExpTotalEnum->Next(1L, &varItem, &nFetched)) == S_OK)
		{
			ATLASSERT(varItem.vt == VT_DISPATCH);
			spExpTotal = varItem;
			if(nFetched > 0 && spExpTotal != NULL)
			{
				__CHECK_HRESULT(spExpTotal->CalcTotals(), _T("Fail to calculate expiry totals."));

				DOUBLE dTemp = 0.;

				// DeltaInShares total
				__CHECK_HRESULT3(spExpTotal->get_DeltaInShares(&dTemp));
				if(dTemp > BAD_DOUBLE_VALUE)
				{
					if(m_dDeltaInShares <= BAD_DOUBLE_VALUE) m_dDeltaInShares = 0.;
					m_dDeltaInShares += dTemp;
				}

				if(VARIANT_FALSE == m_bBadDeltaInShares)
				{
					__CHECK_HRESULT3(spExpTotal->get_BadDeltaInShares(&m_bBadDeltaInShares));
				}

				if(VARIANT_FALSE == m_bBadRhoInShares)
				{
					__CHECK_HRESULT3(spExpTotal->get_BadRhoInShares(&m_bBadRhoInShares));
				}

				// DeltaEq total
				__CHECK_HRESULT3(spExpTotal->get_DeltaEq(&dTemp));
				if(dTemp > BAD_DOUBLE_VALUE)
				{
					if(m_dDeltaEq <= BAD_DOUBLE_VALUE) m_dDeltaEq = 0.;
					m_dDeltaEq += dTemp;
				}

				if(VARIANT_FALSE == m_bBadDeltaEq)
				{
					__CHECK_HRESULT3(spExpTotal->get_BadDeltaEq(&m_bBadDeltaEq));
				}

				// GammaInShares total
				__CHECK_HRESULT3(spExpTotal->get_GammaInShares(&dTemp));
				if(dTemp > BAD_DOUBLE_VALUE)
				{
					if(m_dGammaInShares <= BAD_DOUBLE_VALUE) m_dGammaInShares = 0.;
					m_dGammaInShares += dTemp;
				}

				if(VARIANT_FALSE == m_bBadGammaInShares)
				{
					__CHECK_HRESULT3(spExpTotal->get_BadGammaInShares(&m_bBadGammaInShares));
				}

				// GammaEq total
				__CHECK_HRESULT3(spExpTotal->get_GammaEq(&dTemp));
				if(dTemp > BAD_DOUBLE_VALUE)
				{
					if(m_dGammaEq <= BAD_DOUBLE_VALUE) m_dGammaEq = 0.;
					m_dGammaEq += dTemp;
				}

				// RhoEq total
				__CHECK_HRESULT3(spExpTotal->get_RhoInShares(&dTemp));
				if(dTemp > BAD_DOUBLE_VALUE)
				{
					if(m_dRhoInShares <= BAD_DOUBLE_VALUE) m_dRhoInShares = 0.;
					m_dRhoInShares += dTemp;
				}

				if(VARIANT_FALSE == m_bBadGammaEq)
				{
					__CHECK_HRESULT3(spExpTotal->get_BadGammaEq(&m_bBadGammaEq));
				}

				// VegaInShares total
				__CHECK_HRESULT3(spExpTotal->get_VegaInShares(&dTemp));
				if(dTemp > BAD_DOUBLE_VALUE)
				{
					if(m_dVegaInShares <= BAD_DOUBLE_VALUE) m_dVegaInShares = 0.;
					m_dVegaInShares += dTemp;
				}

				if(VARIANT_FALSE == m_bBadVegaInShares)
				{
					__CHECK_HRESULT3(spExpTotal->get_BadVegaInShares(&m_bBadVegaInShares));
				}

				// TimeValueInShares total
				__CHECK_HRESULT3(spExpTotal->get_TimeValueInShares(&dTemp));
				if(dTemp > BAD_DOUBLE_VALUE)
				{
					if(m_dTimeValueInShares <= BAD_DOUBLE_VALUE) m_dTimeValueInShares = 0.;
					m_dTimeValueInShares += dTemp;
				}

				if(VARIANT_FALSE == m_bBadTimeValueInShares)
				{
					__CHECK_HRESULT3(spExpTotal->get_BadTimeValueInShares(&m_bBadTimeValueInShares));
				}
			}
			spExpTotal = NULL;
			varItem.Clear();
		}
		__CHECK_HRESULT(hr, _T("Fail to get next expiry total."));
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)CComErrorWrapper::ErrorDescription(e), IID_IMmGaGroupAtom, e.Error());
	}

	return S_OK;
}

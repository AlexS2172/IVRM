// MmGaExpTotalAtom.cpp : Implementation of CMmGaExpTotalAtom

#include "stdafx.h"
#include "MmGaExpTotalAtom.h"


/////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CMmGaExpTotalAtom::CalcTotals()
{
	try
	{
		HRESULT hr;

		__MmGaExpTotalAtom::ClearValues();

		IUnknownPtr spUnk;
		_variant_t varItem;
		ULONG nFetched = 0L;
		IMmGaExpAtomPtr spExp;

		__CHECK_HRESULT(m_spExpiry->get__NewEnum(&spUnk), _T("Fail to get expiry collection."));

		IEnumVARIANTPtr spExpEnum(spUnk);
		__CHECK_HRESULT(spExpEnum->Reset(), _T("Fail to reset expiry collection."));
		while((hr = spExpEnum->Next(1L, &varItem, &nFetched)) == S_OK)
		{
			ATLASSERT(varItem.vt == VT_DISPATCH);
			spExp = varItem;
			if(nFetched > 0 && spExp != NULL)
			{
				DOUBLE dTemp = 0.;

				// DeltaInShares total
				__CHECK_HRESULT3(spExp->get_DeltaInShares(&dTemp));
				if(dTemp > BAD_DOUBLE_VALUE)
				{
					if(m_dDeltaInShares <= BAD_DOUBLE_VALUE) m_dDeltaInShares = 0.;
					m_dDeltaInShares += dTemp;
				}

				if(VARIANT_FALSE == m_bBadDeltaInShares)
				{
					__CHECK_HRESULT3(spExp->get_BadDeltaInShares(&m_bBadDeltaInShares));
				}

				if(VARIANT_FALSE == m_bBadRhoInShares)
				{
					__CHECK_HRESULT3(spExp->get_BadRhoInShares(&m_bBadRhoInShares));
				}

				// DeltaEq total
				__CHECK_HRESULT3(spExp->get_DeltaEq(&dTemp));
				if(dTemp > BAD_DOUBLE_VALUE)
				{
					if(m_dDeltaEq <= BAD_DOUBLE_VALUE) m_dDeltaEq = 0.;
					m_dDeltaEq += dTemp;
				}

				if(VARIANT_FALSE == m_bBadDeltaEq)
				{
					__CHECK_HRESULT3(spExp->get_BadDeltaEq(&m_bBadDeltaEq));
				}

				// GammaInShares total
				__CHECK_HRESULT3(spExp->get_GammaInShares(&dTemp));
				if(dTemp > BAD_DOUBLE_VALUE)
				{
					if(m_dGammaInShares <= BAD_DOUBLE_VALUE) m_dGammaInShares = 0.;
					m_dGammaInShares += dTemp;
				}

				if(VARIANT_FALSE == m_bBadGammaInShares)
				{
					__CHECK_HRESULT3(spExp->get_BadGammaInShares(&m_bBadGammaInShares));
				}

				// GammaEq total
				__CHECK_HRESULT3(spExp->get_GammaEq(&dTemp));
				if(dTemp > BAD_DOUBLE_VALUE)
				{
					if(m_dGammaEq <= BAD_DOUBLE_VALUE) m_dGammaEq = 0.;
					m_dGammaEq += dTemp;
				}

				// RhoEq total
				__CHECK_HRESULT3(spExp->get_RhoInShares(&dTemp));
				if(dTemp > BAD_DOUBLE_VALUE)
				{
					if(m_dRhoInShares <= BAD_DOUBLE_VALUE) m_dRhoInShares = 0.;
					m_dRhoInShares += dTemp;
				}

				if(VARIANT_FALSE == m_bBadGammaEq)
				{
					__CHECK_HRESULT3(spExp->get_BadGammaEq(&m_bBadGammaEq));
				}

				// VegaInShares total
				__CHECK_HRESULT3(spExp->get_VegaInShares(&dTemp));
				if(dTemp > BAD_DOUBLE_VALUE)
				{
					if(m_dVegaInShares <= BAD_DOUBLE_VALUE) m_dVegaInShares = 0.;
					m_dVegaInShares += dTemp;
				}

				if(VARIANT_FALSE == m_bBadVegaInShares)
				{
					__CHECK_HRESULT3(spExp->get_BadVegaInShares(&m_bBadVegaInShares));
				}

				// TimeValueInShares total
				__CHECK_HRESULT3(spExp->get_TimeValueInShares(&dTemp));
				if(dTemp > BAD_DOUBLE_VALUE)
				{
					if(m_dTimeValueInShares <= BAD_DOUBLE_VALUE) m_dTimeValueInShares = 0.;
					m_dTimeValueInShares += dTemp;
				}

				if(VARIANT_FALSE == m_bBadTimeValueInShares)
				{
					__CHECK_HRESULT3(spExp->get_BadTimeValueInShares(&m_bBadTimeValueInShares));
				}

				VARIANT_BOOL bHasSynth = VARIANT_FALSE;
				IMmGaSynthGreekCollPtr spSynthGreeks;

				__CHECK_HRESULT3(spExp->get_HasSynth(&bHasSynth));
				__CHECK_HRESULT(spExp->get_SynthGreek(&spSynthGreeks), _T("Fail to get expiry synthetic greeks."));

				if(VARIANT_FALSE != bHasSynth && spSynthGreeks != NULL)
				{
					IMmGaSynthGreekAtomPtr spSynthGreek;
					__CHECK_HRESULT(spSynthGreeks->get__NewEnum(&spUnk), _T("Fail to get expiry synthetic greeks."));

					IEnumVARIANTPtr spSynthGreeksEnum(spUnk);
					__CHECK_HRESULT(spSynthGreeksEnum->Reset(), _T("Fail to reset expiry synthetic greeks."));
					while((hr = spSynthGreeksEnum->Next(1L, &varItem, &nFetched)) == S_OK)
					{
						ATLASSERT(varItem.vt == VT_DISPATCH);
						spSynthGreek = varItem;
						if(nFetched > 0 && spSynthGreek != NULL)
						{
							// DeltaInShares total
							__CHECK_HRESULT3(spSynthGreek->get_DeltaInShares(&dTemp));
							if(dTemp > BAD_DOUBLE_VALUE)
							{
								if(m_dDeltaInShares <= BAD_DOUBLE_VALUE) m_dDeltaInShares = 0.;
								m_dDeltaInShares += dTemp;
							}

							if(VARIANT_FALSE == m_bBadDeltaInShares)
							{
								__CHECK_HRESULT3(spSynthGreek->get_BadDeltaInShares(&m_bBadDeltaInShares));
							}

							if(VARIANT_FALSE == m_bBadRhoInShares)
							{
								__CHECK_HRESULT3(spSynthGreek->get_BadRhoInShares(&m_bBadRhoInShares));
							}

							// DeltaEq total
							__CHECK_HRESULT3(spSynthGreek->get_DeltaEq(&dTemp));
							if(dTemp > BAD_DOUBLE_VALUE)
							{
								if(m_dDeltaEq <= BAD_DOUBLE_VALUE) m_dDeltaEq = 0.;
								m_dDeltaEq += dTemp;
							}

							if(VARIANT_FALSE == m_bBadDeltaEq)
							{
								__CHECK_HRESULT3(spSynthGreek->get_BadDeltaEq(&m_bBadDeltaEq));
							}

							// GammaInShares total
							__CHECK_HRESULT3(spSynthGreek->get_GammaInShares(&dTemp));
							if(dTemp > BAD_DOUBLE_VALUE)
							{
								if(m_dGammaInShares <= BAD_DOUBLE_VALUE) m_dGammaInShares = 0.;
								m_dGammaInShares += dTemp;
							}

							if(VARIANT_FALSE == m_bBadGammaInShares)
							{
								__CHECK_HRESULT3(spSynthGreek->get_BadGammaInShares(&m_bBadGammaInShares));
							}

							// GammaEq total
							__CHECK_HRESULT3(spSynthGreek->get_GammaEq(&dTemp));
							if(dTemp > BAD_DOUBLE_VALUE)
							{
								if(m_dGammaEq <= BAD_DOUBLE_VALUE) m_dGammaEq = 0.;
								m_dGammaEq += dTemp;
							}

							// RhoEq total
							__CHECK_HRESULT3(spSynthGreek->get_RhoInShares(&dTemp));
							if(dTemp > BAD_DOUBLE_VALUE)
							{
								if(m_dRhoInShares <= BAD_DOUBLE_VALUE) m_dRhoInShares = 0.;
								m_dRhoInShares += dTemp;
							}

							if(VARIANT_FALSE == m_bBadGammaEq)
							{
								__CHECK_HRESULT3(spSynthGreek->get_BadGammaEq(&m_bBadGammaEq));
							}

						}
						spSynthGreek = NULL;
						varItem.Clear();
					}
					__CHECK_HRESULT(hr, _T("Fail to get next expiry synthetic greek."));
				}
			}
			spExp = NULL;
			varItem.Clear();
		}
		__CHECK_HRESULT(hr, _T("Fail to get next expiry."));
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)CComErrorWrapper::ErrorDescription(e), IID_IMmGaExpTotalAtom, e.Error());
	}

	return S_OK;
}

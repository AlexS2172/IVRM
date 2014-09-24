// MmRvGroupingTotalAtom.cpp : Implementation of CMmRvGroupingTotalAtom

#include "stdafx.h"
#include "MmRvGroupingTotalAtom.h"

/////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CMmRvGroupingTotalAtom::CalcTotals()
{
	try
	{
		HRESULT hr;

		__MmRvGroupingTotalAtom::ClearValues();

		IUnknownPtr spUnk;
		_variant_t varItem;
		ULONG nFetched = 0L;
		IMmRvGroupingAtomPtr spGrouping;

		__CHECK_HRESULT(m_spGrouping->get__NewEnum(&spUnk), _T("Fail to get expiry collection."));

		IEnumVARIANTPtr spGroupingEnum(spUnk);
		__CHECK_HRESULT(spGroupingEnum->Reset(), _T("Fail to reset expiry collection."));
		while((hr = spGroupingEnum->Next(1L, &varItem, &nFetched)) == S_OK)
		{
			ATLASSERT(varItem.vt == VT_DISPATCH);
			spGrouping = varItem;
			if(nFetched > 0 && spGrouping != NULL)
			{
				DOUBLE dTemp = 0.;

				// DeltaInShares total
				__CHECK_HRESULT3(spGrouping->get_DeltaInShares(&dTemp));
				if(dTemp > BAD_DOUBLE_VALUE)
				{
					if(m_dDeltaInShares <= BAD_DOUBLE_VALUE) m_dDeltaInShares = 0.;
					m_dDeltaInShares += dTemp;
				}

				if(VARIANT_FALSE == m_bBadDeltaInShares)
				{
					__CHECK_HRESULT3(spGrouping->get_BadDeltaInShares(&m_bBadDeltaInShares));
				}

				// DeltaEq total
				__CHECK_HRESULT3(spGrouping->get_DeltaEq(&dTemp));
				if(dTemp > BAD_DOUBLE_VALUE)
				{
					if(m_dDeltaEq <= BAD_DOUBLE_VALUE) m_dDeltaEq = 0.;
					m_dDeltaEq += dTemp;
				}

				if(VARIANT_FALSE == m_bBadDeltaEq)
				{
					__CHECK_HRESULT3(spGrouping->get_BadDeltaEq(&m_bBadDeltaEq));
				}

				// NetDeltaInShares total
				__CHECK_HRESULT3(spGrouping->get_NetDeltaInShares(&dTemp));
				if(dTemp > BAD_DOUBLE_VALUE)
				{
					if(m_dNetDeltaInShares <= BAD_DOUBLE_VALUE) m_dNetDeltaInShares = 0.;
					m_dNetDeltaInShares += dTemp;
				}

				if(VARIANT_FALSE == m_bBadNetDeltaInShares)
				{
					__CHECK_HRESULT3(spGrouping->get_BadNetDeltaInShares(&m_bBadNetDeltaInShares));
				}

				// NetDeltaEq total
				__CHECK_HRESULT3(spGrouping->get_NetDeltaEq(&dTemp));
				if(dTemp > BAD_DOUBLE_VALUE)
				{
					if(m_dNetDeltaEq <= BAD_DOUBLE_VALUE) m_dNetDeltaEq = 0.;
					m_dNetDeltaEq += dTemp;
				}

				if(VARIANT_FALSE == m_bBadNetDeltaEq)
				{
					__CHECK_HRESULT3(spGrouping->get_BadNetDeltaEq(&m_bBadNetDeltaEq));
				}

				// GammaInShares total
				__CHECK_HRESULT3(spGrouping->get_GammaInShares(&dTemp));
				if(dTemp > BAD_DOUBLE_VALUE)
				{
					if(m_dGammaInShares <= BAD_DOUBLE_VALUE) m_dGammaInShares = 0.;
					m_dGammaInShares += dTemp;
				}

				if(VARIANT_FALSE == m_bBadGammaInShares)
				{
					__CHECK_HRESULT3(spGrouping->get_BadGammaInShares(&m_bBadGammaInShares));
				}

				// GammaEq total
				__CHECK_HRESULT3(spGrouping->get_GammaEq(&dTemp));
				if(dTemp > BAD_DOUBLE_VALUE)
				{
					if(m_dGammaEq <= BAD_DOUBLE_VALUE) m_dGammaEq = 0.;
					m_dGammaEq += dTemp;
				}

				if(VARIANT_FALSE == m_bBadGammaEq)
				{
					__CHECK_HRESULT3(spGrouping->get_BadGammaEq(&m_bBadGammaEq));
				}

				// VegaInShares total
				__CHECK_HRESULT3(spGrouping->get_VegaInShares(&dTemp));
				if(dTemp > BAD_DOUBLE_VALUE)
				{
					if(m_dVegaInShares <= BAD_DOUBLE_VALUE) m_dVegaInShares = 0.;
					m_dVegaInShares += dTemp;
				}

				if(VARIANT_FALSE == m_bBadVegaInShares)
				{
					__CHECK_HRESULT3(spGrouping->get_BadVegaInShares(&m_bBadVegaInShares));
				}

				// TimeValueInShares total
				__CHECK_HRESULT3(spGrouping->get_TimeValueInShares(&dTemp));
				if(dTemp > BAD_DOUBLE_VALUE)
				{
					if(m_dTimeValueInShares <= BAD_DOUBLE_VALUE) m_dTimeValueInShares = 0.;
					m_dTimeValueInShares += dTemp;
				}

				if(VARIANT_FALSE == m_bBadTimeValueInShares)
				{
					__CHECK_HRESULT3(spGrouping->get_BadTimeValueInShares(&m_bBadTimeValueInShares));
				}

				VARIANT_BOOL bHasSynth = VARIANT_FALSE;
				IMmRvSynthGreeksCollPtr spSynthGreeks;

				__CHECK_HRESULT3(spGrouping->get_HasSynth(&bHasSynth));
				__CHECK_HRESULT(spGrouping->get_SynthGreek(&spSynthGreeks), _T("Fail to get expiry synthetic greeks."));

				if(VARIANT_FALSE != bHasSynth && spSynthGreeks != NULL)
				{
					IMmRvSynthGreeksAtomPtr spSynthGreek;
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
								__CHECK_HRESULT3(spSynthGreek->get_BadDelta(&m_bBadDeltaInShares));
							}

							 // DeltaEq total
							__CHECK_HRESULT3(spSynthGreek->get_DeltaInMoney(&dTemp));
							if(dTemp > BAD_DOUBLE_VALUE)
							{
								if(m_dDeltaEq <= BAD_DOUBLE_VALUE) m_dDeltaEq = 0.;
								m_dDeltaEq += dTemp;
							}

							if(VARIANT_FALSE == m_bBadDeltaEq)
							{
								__CHECK_HRESULT3(spSynthGreek->get_BadDeltaInMoney(&m_bBadDeltaEq));
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
								__CHECK_HRESULT3(spSynthGreek->get_BadGamma(&m_bBadGammaInShares));
							}

							// GammaEq total
							__CHECK_HRESULT3(spSynthGreek->get_NetGamma(&dTemp));
							if(dTemp > BAD_DOUBLE_VALUE)
							{
								if(m_dGammaEq <= BAD_DOUBLE_VALUE) m_dGammaEq = 0.;
								m_dGammaEq += dTemp;
							}

							if(VARIANT_FALSE == m_bBadGammaEq)
							{
								__CHECK_HRESULT3(spSynthGreek->get_BadNetGamma(&m_bBadGammaEq));
							}

						}
						spSynthGreek = NULL;
						varItem.Clear();
					}
					__CHECK_HRESULT(hr, _T("Fail to get next expiry synthetic greek."));
				}
			}
			spGrouping = NULL;
			varItem.Clear();
		}
		__CHECK_HRESULT(hr, _T("Fail to get next expiry."));
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)CComErrorWrapper::ErrorDescription(e), IID_IMmRvGroupingTotalAtom, e.Error());
	}

	return S_OK;
}


STDMETHODIMP CMmRvGroupingTotalAtom::CalcCV(void)
{
	try
	{
		if ( m_spRTContext != NULL && m_nPoolID != BAD_LONG_VALUE ) 
			_CHK(m_spRTContext->Recalculate(m_nPoolID));
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_IMmRvGroupingTotalAtom, e.Error());
	}

	return S_OK;
}

// IcvDataProvider
STDMETHODIMP CMmRvGroupingTotalAtom::raw_GetData(LONG lDataID, VARIANT* Value)
{
	return E_NOTIMPL;
}

STDMETHODIMP CMmRvGroupingTotalAtom::raw_CallFunction(LONG lFunctionID, SAFEARRAY** arrParameters, VARIANT *Value)
{
	return E_NOTIMPL;
}

STDMETHODIMP CMmRvGroupingTotalAtom::raw_Check(SAFEARRAY **arrSysVars, SAFEARRAY **arrSysFuncs, LONG *pRetVal)
{
	return E_NOTIMPL;
}
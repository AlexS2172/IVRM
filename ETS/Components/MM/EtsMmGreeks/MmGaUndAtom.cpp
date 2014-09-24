// MmGaUndAtom.cpp : Implementation of CMmGaUndAtom

#include "stdafx.h"
#include "MmGaUndAtom.h"
#include "MmGaUndColl.h"

#define USD_ID			0L


/////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CMmGaUndAtom::CalcPositions(IMmGaUndColl* collUnd, EtsCalcModelTypeEnum enCalcModel,
										 VARIANT_BOOL bUseTheoVolatility, VARIANT_BOOL bUseTheoVolaNoBid, 
										 VARIANT_BOOL bUseTheoVolaBadMarket, DOUBLE dUndPriceTolerance, 
										 EtsPriceRoundingRuleEnum enPriceRoundingRule,
										 ICalculationParametrs* pParams)
{
	if(collUnd == NULL)
	{
		return Error(L"Invalid underlings passed.", IID_IMmGaUndAtom, E_INVALIDARG);
	}

	try
	{
		HRESULT hr;
		IMmGaUndCollPtr spCollUnd(collUnd);

		__MmGaUndAtom::ClearValues();

		LPSAFEARRAY psaDates = NULL;
		LPSAFEARRAY psaAmounts = NULL;
		DOUBLE * pdDates = NULL;	
		DOUBLE * pdAmounts = NULL;
		LPVOID lpDateData = NULL;
		LPVOID lpAmountData = NULL;



		IUnknownPtr spUnk;
		_variant_t varItem;
		ULONG nFetched = 0L;
		IMmGaExpAtomPtr spExp;
		EtsReplacePriceStatusEnum enMidPriceStatus = enRpsNone;

		DOUBLE dUndPriceMid = m_spUndPriceProfile->GetUndPriceMid(m_dPriceBid, m_dPriceAsk, m_dPriceLast, dUndPriceTolerance, enPriceRoundingRule, &enMidPriceStatus, FALSE);

		// clear underlying synthetic components values
		if(VARIANT_FALSE != m_bHasSynth && m_spSynthGreek != NULL)
		{
			IMmGaSynthGreekAtomPtr spSynthGreek;
			__CHECK_HRESULT(m_spSynthGreek->get__NewEnum(&spUnk), _T("Fail to get synthetic greeks."));

			IEnumVARIANTPtr spSynthGreeksEnum(spUnk);
			__CHECK_HRESULT(spSynthGreeksEnum->Reset(), _T("Fail to reset synthetic greeks."));
			while((hr = spSynthGreeksEnum->Next(1L, &varItem, &nFetched)) == S_OK)
			{
				ATLASSERT(varItem.vt == VT_DISPATCH);
				spSynthGreek = varItem;
				if(nFetched > 0 && spSynthGreek != NULL)
				{
					__CHECK_HRESULT3(spSynthGreek->ClearValues());
				}
				spSynthGreek = NULL;
				varItem.Clear();
			}
			__CHECK_HRESULT(hr, _T("Fail to get next synthetic greek."));
		}

		// walk through expiries
		__CHECK_HRESULT(m_spExpiry->get__NewEnum(&spUnk), _T("Fail to get expiry collection."));

		IEnumVARIANTPtr spExpEnum(spUnk);
		__CHECK_HRESULT(spExpEnum->Reset(), _T("Fail to reset expiry collection."));
		while((hr = spExpEnum->Next(1L, &varItem, &nFetched)) == S_OK)
		{
			ATLASSERT(varItem.vt == VT_DISPATCH);
			spExp = varItem;
			if(nFetched > 0 && spExp != NULL)
			{
				__CHECK_HRESULT3(spExp->ClearValues());

				IMmGaSynthGreekCollPtr spExpSynthGreeks;
				IMmGaPosCollPtr spCollPos;
				IMmGaPosAtomPtr spPos;
				VARIANT_BOOL bExpHasSynth = VARIANT_FALSE;
				__CHECK_HRESULT3(spExp->get_HasSynth(&bExpHasSynth));
				__CHECK_HRESULT(spExp->get_SynthGreek(&spExpSynthGreeks), _T("Fail to get expiry synthetic greeks."));

				DOUBLE dTemp = 0., dTemp2 = 0.;
				DOUBLE dExpDeltaInShares = BAD_DOUBLE_VALUE, dExpGammaInShares = BAD_DOUBLE_VALUE;
				DOUBLE dExpDeltaEq = BAD_DOUBLE_VALUE, dExpGammaEq = BAD_DOUBLE_VALUE;
				DOUBLE dExpVegaInShares = BAD_DOUBLE_VALUE, dExpTimeValueInShares = BAD_DOUBLE_VALUE;
				DOUBLE dExpRhoInShares = BAD_DOUBLE_VALUE ;
				VARIANT_BOOL bExpBadDeltaInShares = VARIANT_FALSE, bExpBadGammaInShares = VARIANT_FALSE;
				VARIANT_BOOL bExpBadDeltaEq = VARIANT_FALSE, bExpBadGammaEq = VARIANT_FALSE;
				VARIANT_BOOL bExpBadVegaInShares = VARIANT_FALSE, bExpBadTimeValueInShares = VARIANT_FALSE;
				VARIANT_BOOL bExpBadRhoInShares = VARIANT_FALSE ;

				// clear expiry synthetic components values
				if(VARIANT_FALSE != bExpHasSynth && spExpSynthGreeks != NULL)
				{
					IMmGaSynthGreekAtomPtr spExpSynthGreek;
					__CHECK_HRESULT(spExpSynthGreeks->get__NewEnum(&spUnk), _T("Fail to get expiry synthetic greeks."));

					IEnumVARIANTPtr spExpSynthGreeksEnum(spUnk);
					__CHECK_HRESULT(spExpSynthGreeksEnum->Reset(), _T("Fail to reset expiry synthetic greeks."));
					while((hr = spExpSynthGreeksEnum->Next(1L, &varItem, &nFetched)) == S_OK)
					{
						ATLASSERT(varItem.vt == VT_DISPATCH);
						spExpSynthGreek = varItem;
						if(nFetched > 0 && spExpSynthGreek != NULL)
						{
							__CHECK_HRESULT3(spExpSynthGreek->ClearValues());
						}
						spExpSynthGreek = NULL;
						varItem.Clear();
					}
					__CHECK_HRESULT(hr, _T("Fail to get next expiry synthetic greek."));
				}

				// walk through expiry positions
				__CHECK_HRESULT(spExp->get_Pos(&spCollPos), _T("Fail to get expiry positions."));
				__CHECK_HRESULT(spCollPos->get__NewEnum(&spUnk), _T("Fail to get expiry positions collection."));

				IEnumVARIANTPtr spPosEnum(spUnk);
				__CHECK_HRESULT(spPosEnum->Reset(), _T("Fail to reset expiry positions collection."));
				while((hr = spPosEnum->Next(1L, &varItem, &nFetched)) == S_OK)
				{
					ATLASSERT(varItem.vt == VT_DISPATCH);
					spPos = varItem;
					if(nFetched > 0 && spPos != NULL)
					{
						VARIANT_BOOL bPosIsSynth = VARIANT_FALSE;
						IMmGaSynthGreekCollPtr spPosSynthGreeks;

						__CHECK_HRESULT3(spPos->get_IsSynth(&bPosIsSynth));
						__CHECK_HRESULT(spPos->get_SynthGreek(&spPosSynthGreeks), _T("Fail to get position synthetic greeks."));

						// calculate expiry position
						__CHECK_HRESULT(_CalcPosition(spPos, spExp, collUnd, dUndPriceMid, enCalcModel, 
							bUseTheoVolatility, bUseTheoVolaNoBid, bUseTheoVolaBadMarket, 
							dUndPriceTolerance, enPriceRoundingRule, pParams), _T("Fail to calculate expiry position."));

						// DeltaInShares total
						__CHECK_HRESULT3(spPos->get_DeltaInShares(&dTemp));
						if(dTemp > BAD_DOUBLE_VALUE)
						{
							if(dExpDeltaInShares <= BAD_DOUBLE_VALUE) dExpDeltaInShares = 0.;
							dExpDeltaInShares += dTemp;
						}
						else
						{
							bExpBadDeltaInShares = VARIANT_TRUE;
						}

						// DeltaEq total
						__CHECK_HRESULT3(spPos->get_DeltaEq(&dTemp));
						if(dTemp > BAD_DOUBLE_VALUE)
						{
							if(dExpDeltaEq <= BAD_DOUBLE_VALUE) dExpDeltaEq = 0.;
							dExpDeltaEq += dTemp;
						}
						else
						{
							bExpBadDeltaEq = VARIANT_TRUE;
						}

						// GammaInShares total
						__CHECK_HRESULT3(spPos->get_GammaInShares(&dTemp));
						if(dTemp > BAD_DOUBLE_VALUE)
						{
							if(dExpGammaInShares <= BAD_DOUBLE_VALUE) dExpGammaInShares = 0.;
							dExpGammaInShares += dTemp;
						}
						else
						{
							bExpBadGammaInShares = VARIANT_TRUE;
						}

						// GammaEq total
						__CHECK_HRESULT3(spPos->get_GammaEq(&dTemp));
						if(dTemp > BAD_DOUBLE_VALUE)
						{
							if(dExpGammaEq <= BAD_DOUBLE_VALUE) dExpGammaEq = 0.;
							dExpGammaEq += dTemp;
						}
						else
						{
							bExpBadGammaEq = VARIANT_TRUE;
						}

						// VegaInShares total
						__CHECK_HRESULT3(spPos->get_VegaInShares(&dTemp));
						if(dTemp > BAD_DOUBLE_VALUE)
						{
							if(dExpVegaInShares <= BAD_DOUBLE_VALUE) dExpVegaInShares = 0.;
							dExpVegaInShares += dTemp;
						}
						else
						{
							bExpBadVegaInShares = VARIANT_TRUE;
						}

						// RhoInShares total
						__CHECK_HRESULT3(spPos->get_RhoInShares(&dTemp));
						if(dTemp > BAD_DOUBLE_VALUE)
						{
							if(dExpRhoInShares <= BAD_DOUBLE_VALUE) dExpRhoInShares = 0.;
							dExpRhoInShares += dTemp;
						}
						else
						{
							bExpBadRhoInShares = VARIANT_TRUE;
						}

						// TimeValueInShares total
						__CHECK_HRESULT3(spPos->get_TimeValueInShares(&dTemp));
						if(dTemp > BAD_DOUBLE_VALUE)
						{
							if(dExpTimeValueInShares <= BAD_DOUBLE_VALUE) dExpTimeValueInShares = 0.;
							dExpTimeValueInShares += dTemp;
						}
						else
						{
							bExpBadTimeValueInShares = VARIANT_TRUE;
						}

						// expiry synthetic components
						if(VARIANT_FALSE != bPosIsSynth && spPosSynthGreeks != NULL && spExpSynthGreeks != NULL)
						{
							IMmGaSynthGreekAtomPtr spExpSynthGreek;
							IMmGaSynthGreekAtomPtr spPosSynthGreek;
							__CHECK_HRESULT(spPosSynthGreeks->get__NewEnum(&spUnk), _T("Fail to get position synthetic greeks."));

							IEnumVARIANTPtr spPosSynthGreeksEnum(spUnk);
							__CHECK_HRESULT(spPosSynthGreeksEnum->Reset(), _T("Fail to reset position synthetic greeks."));
							while((hr = spPosSynthGreeksEnum->Next(1L, &varItem, &nFetched)) == S_OK)
							{
								ATLASSERT(varItem.vt == VT_DISPATCH);
								spPosSynthGreek = varItem;
								if(nFetched > 0 && spPosSynthGreek != NULL)
								{
									LONG nSynthUndID = 0L;
									__CHECK_HRESULT3(spPosSynthGreek->get_UndID(&nSynthUndID));
									__CHECK_HRESULT(spExpSynthGreeks->get_Item(nSynthUndID, &spExpSynthGreek), _T("Fail to get expiry synthetic greeks."));

									if(spExpSynthGreek != NULL)
									{
										// DeltaInShares total
										__CHECK_HRESULT3(spPosSynthGreek->get_DeltaInShares(&dTemp));
										if(dTemp > BAD_DOUBLE_VALUE)
										{
											__CHECK_HRESULT3(spExpSynthGreek->get_DeltaInShares(&dTemp2));
											if(dTemp2 <= BAD_DOUBLE_VALUE) dTemp2 = 0.;
											__CHECK_HRESULT3(spExpSynthGreek->put_DeltaInShares(dTemp2 + dTemp));
										}
										else
										{
											__CHECK_HRESULT3(spExpSynthGreek->put_BadDeltaInShares(VARIANT_TRUE));
										}

										// DeltaEq total
										__CHECK_HRESULT3(spPosSynthGreek->get_DeltaEq(&dTemp));
										if(dTemp > BAD_DOUBLE_VALUE)
										{
											__CHECK_HRESULT3(spExpSynthGreek->get_DeltaEq(&dTemp2));
											if(dTemp2 <= BAD_DOUBLE_VALUE) dTemp2 = 0.;
											__CHECK_HRESULT3(spExpSynthGreek->put_DeltaEq(dTemp2 + dTemp));
										}
										else
										{
											__CHECK_HRESULT3(spExpSynthGreek->put_BadDeltaEq(VARIANT_TRUE));
										}

										// GammaInShares total
										__CHECK_HRESULT3(spPosSynthGreek->get_GammaInShares(&dTemp));
										if(dTemp > BAD_DOUBLE_VALUE)
										{
											__CHECK_HRESULT3(spExpSynthGreek->get_GammaInShares(&dTemp2));
											if(dTemp2 <= BAD_DOUBLE_VALUE) dTemp2 = 0.;
											__CHECK_HRESULT3(spExpSynthGreek->put_GammaInShares(dTemp2 + dTemp));
										}
										else
										{
											__CHECK_HRESULT3(spExpSynthGreek->put_BadGammaInShares(VARIANT_TRUE));
										}

										// GammaEq total
										__CHECK_HRESULT3(spPosSynthGreek->get_GammaEq(&dTemp));
										if(dTemp > BAD_DOUBLE_VALUE)
										{
											__CHECK_HRESULT3(spExpSynthGreek->get_GammaEq(&dTemp2));
											if(dTemp2 <= BAD_DOUBLE_VALUE) dTemp2 = 0.;
											__CHECK_HRESULT3(spExpSynthGreek->put_GammaEq(dTemp2 + dTemp));
										}
										else
										{
											__CHECK_HRESULT3(spExpSynthGreek->put_BadGammaEq(VARIANT_TRUE));
										}
									}
								}
								spPosSynthGreek = NULL;
								varItem.Clear();
							}
							__CHECK_HRESULT(hr, _T("Fail to get next position synthetic greek."));
						}
					}
					spPos = NULL;
					varItem.Clear();
				}
				__CHECK_HRESULT(hr, _T("Fail to get next expiry position."));

				// DeltaInShares total
				__CHECK_HRESULT3(spExp->put_DeltaInShares(dExpDeltaInShares));
				__CHECK_HRESULT3(spExp->put_BadDeltaInShares(bExpBadDeltaInShares));
				if(dExpDeltaInShares > BAD_DOUBLE_VALUE)
				{
					if(m_dDeltaInShares <= BAD_DOUBLE_VALUE) m_dDeltaInShares = 0.;
					m_dDeltaInShares += dExpDeltaInShares;
				}

				if(VARIANT_FALSE == m_bBadDeltaInShares && VARIANT_FALSE != bExpBadDeltaInShares)
				{
					m_bBadDeltaInShares = bExpBadDeltaInShares;
				}

				// DeltaEq total
				__CHECK_HRESULT3(spExp->put_DeltaEq(dExpDeltaEq));
				__CHECK_HRESULT3(spExp->put_BadDeltaEq(bExpBadDeltaEq));
				if(dExpDeltaEq > BAD_DOUBLE_VALUE)
				{
					if(m_dDeltaEq <= BAD_DOUBLE_VALUE) m_dDeltaEq = 0.;
					m_dDeltaEq += dExpDeltaEq;
				}

				if(VARIANT_FALSE == m_bBadDeltaEq && VARIANT_FALSE != bExpBadDeltaEq)
				{
					m_bBadDeltaEq = bExpBadDeltaEq;
				}


				// GammaInShares total
				__CHECK_HRESULT3(spExp->put_GammaInShares(dExpGammaInShares));
				__CHECK_HRESULT3(spExp->put_BadGammaInShares(bExpBadGammaInShares));
				if(dExpGammaInShares > BAD_DOUBLE_VALUE)
				{
					if(m_dGammaInShares <= BAD_DOUBLE_VALUE) m_dGammaInShares = 0.;
					m_dGammaInShares += dExpGammaInShares;
				}

				if(VARIANT_FALSE == m_bBadGammaInShares && VARIANT_FALSE != bExpBadGammaInShares)
				{
					m_bBadGammaInShares = bExpBadGammaInShares;
				}

				// GammaEq total
				__CHECK_HRESULT3(spExp->put_GammaEq(dExpGammaEq));
				__CHECK_HRESULT3(spExp->put_BadGammaEq(bExpBadGammaEq));
				if(dExpGammaEq > BAD_DOUBLE_VALUE)
				{
					if(m_dGammaEq <= BAD_DOUBLE_VALUE) m_dGammaEq = 0.;
					m_dGammaEq += dExpGammaEq;
				}

				if(VARIANT_FALSE == m_bBadGammaEq && VARIANT_FALSE != bExpBadGammaEq)
				{
					m_bBadGammaEq = bExpBadGammaEq;
				}

				// VegaInShares total
				__CHECK_HRESULT3(spExp->put_VegaInShares(dExpVegaInShares));
				__CHECK_HRESULT3(spExp->put_BadVegaInShares(bExpBadVegaInShares));
				if(dExpVegaInShares > BAD_DOUBLE_VALUE)
				{
					if(m_dVegaInShares <= BAD_DOUBLE_VALUE) m_dVegaInShares = 0.;
					m_dVegaInShares += dExpVegaInShares;
				}

				if(VARIANT_FALSE == m_bBadVegaInShares && VARIANT_FALSE != bExpBadVegaInShares)
				{
					m_bBadVegaInShares = bExpBadVegaInShares;
				}

				// RhoInShares total
				__CHECK_HRESULT3(spExp->put_RhoInShares(dExpRhoInShares));
				__CHECK_HRESULT3(spExp->put_BadVegaInShares(bExpBadRhoInShares));
				if(dExpRhoInShares > BAD_DOUBLE_VALUE)
				{
					if(m_dRhoInShares <= BAD_DOUBLE_VALUE) m_dRhoInShares = 0.;
					m_dRhoInShares += dExpRhoInShares;
				}

				if(VARIANT_FALSE == m_bBadRhoInShares && VARIANT_FALSE != bExpBadRhoInShares)
				{
					m_bBadRhoInShares = bExpBadRhoInShares;
				}

				// TimeValueInShares total
				__CHECK_HRESULT3(spExp->put_TimeValueInShares(dExpTimeValueInShares));
				__CHECK_HRESULT3(spExp->put_BadTimeValueInShares(bExpBadTimeValueInShares));
				if(dExpTimeValueInShares > BAD_DOUBLE_VALUE)
				{
					if(m_dTimeValueInShares <= BAD_DOUBLE_VALUE) m_dTimeValueInShares = 0.;
					m_dTimeValueInShares += dExpTimeValueInShares;
				}

				if(VARIANT_FALSE == m_bBadTimeValueInShares && VARIANT_FALSE != bExpBadTimeValueInShares)
				{
					m_bBadTimeValueInShares = bExpBadTimeValueInShares;
				}

				// synthetic components
				if(VARIANT_FALSE != m_bHasSynth && spExpSynthGreeks != NULL && m_spSynthGreek != NULL)
				{
					IMmGaSynthGreekAtomPtr spSynthGreek;
					IMmGaSynthGreekAtomPtr spExpSynthGreek;
					__CHECK_HRESULT(spExpSynthGreeks->get__NewEnum(&spUnk), _T("Fail to get expiry synthetic greeks."));

					IEnumVARIANTPtr spPosSynthGreeksEnum(spUnk);
					__CHECK_HRESULT(spPosSynthGreeksEnum->Reset(), _T("Fail to reset expiry synthetic greeks."));
					while((hr = spPosSynthGreeksEnum->Next(1L, &varItem, &nFetched)) == S_OK)
					{
						ATLASSERT(varItem.vt == VT_DISPATCH);
						spExpSynthGreek = varItem;
						if(nFetched > 0 && spExpSynthGreek != NULL)
						{
							LONG nSynthUndID = 0L;
							__CHECK_HRESULT3(spExpSynthGreek->get_UndID(&nSynthUndID));
							__CHECK_HRESULT(m_spSynthGreek->get_Item(nSynthUndID, &spSynthGreek), _T("Fail to get synthetic greeks."));

							if(spExpSynthGreeks != NULL)
							{
								// DeltaInShares total
								__CHECK_HRESULT3(spExpSynthGreek->get_DeltaInShares(&dTemp));
								if(dTemp > BAD_DOUBLE_VALUE)
								{
									__CHECK_HRESULT3(spSynthGreek->get_DeltaInShares(&dTemp2));
									if(dTemp2 <= BAD_DOUBLE_VALUE) dTemp2 = 0.;
									__CHECK_HRESULT3(spSynthGreek->put_DeltaInShares(dTemp2 + dTemp));
								}
								else
								{
									__CHECK_HRESULT3(spSynthGreek->put_BadDeltaInShares(VARIANT_TRUE));
								}

								// DeltaEq total
								__CHECK_HRESULT3(spExpSynthGreek->get_DeltaEq(&dTemp));
								if(dTemp > BAD_DOUBLE_VALUE)
								{
									__CHECK_HRESULT3(spSynthGreek->get_DeltaEq(&dTemp2));
									if(dTemp2 <= BAD_DOUBLE_VALUE) dTemp2 = 0.;
									__CHECK_HRESULT3(spSynthGreek->put_DeltaEq(dTemp2 + dTemp));
								}
								else
								{
									__CHECK_HRESULT3(spSynthGreek->put_BadDeltaEq(VARIANT_TRUE));
								}

								// GammaInShares total
								__CHECK_HRESULT3(spExpSynthGreek->get_GammaInShares(&dTemp));
								if(dTemp > BAD_DOUBLE_VALUE)
								{
									__CHECK_HRESULT3(spSynthGreek->get_GammaInShares(&dTemp2));
									if(dTemp2 <= BAD_DOUBLE_VALUE) dTemp2 = 0.;
									__CHECK_HRESULT3(spSynthGreek->put_GammaInShares(dTemp2 + dTemp));
								}
								else
								{
									__CHECK_HRESULT3(spSynthGreek->put_BadGammaInShares(VARIANT_TRUE));
								}

								// GammaEq total
								__CHECK_HRESULT3(spExpSynthGreek->get_GammaEq(&dTemp));
								if(dTemp > BAD_DOUBLE_VALUE)
								{
									__CHECK_HRESULT3(spSynthGreek->get_GammaEq(&dTemp2));
									if(dTemp2 <= BAD_DOUBLE_VALUE) dTemp2 = 0.;
									__CHECK_HRESULT3(spSynthGreek->put_GammaEq(dTemp2 + dTemp));
								}
								else
								{
									__CHECK_HRESULT3(spSynthGreek->put_BadGammaEq(VARIANT_TRUE));
								}
							}
						}
						spExpSynthGreek = NULL;
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
		return Error((PTCHAR)CComErrorWrapper::ErrorDescription(e), IID_IMmGaUndAtom, e.Error());
	}

	return S_OK;
}

/////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CMmGaUndAtom::_CalcPosition(IMmGaPosAtom* pPos, IMmGaExpAtom* pExp, IMmGaUndColl* pCollUnd,
										 DOUBLE dUndPriceMid, EtsCalcModelTypeEnum enCalcModel,
										 VARIANT_BOOL bUseTheoVolatility, VARIANT_BOOL bUseTheoVolaNoBid, 
										 VARIANT_BOOL bUseTheoVolaBadMarket, DOUBLE dUndPriceTolerance, 
										 EtsPriceRoundingRuleEnum enPriceRoundingRule,
										 ICalculationParametrs* pParams)
{
	try
	{
		HRESULT hr;
		IMmGaUndCollPtr spCollUnd(pCollUnd);
		IMmGaPosAtomPtr spPos(pPos);
		IMmGaExpAtomPtr spExp(pExp);
		ISynthRootAtomPtr spSynthRoot;
		IMmGaSynthGreekCollPtr spSynthGreeks;
		LONG nOptRootID = 0L; DOUBLE dCashValue = 0., dSynthUndPriceMid = 0.;
		DOUBLE dSynthUndPriceBid = 0., dSynthUndPriceAsk = 0., dSynthUndPriceLast = 0.;
		VARIANT_BOOL bIsSynth = VARIANT_FALSE;
		EtsContractTypeEnum enContractType = enCtOption;

		IUnknownPtr spUnk;
		_variant_t varItem;
		ULONG nFetched = 0L;


		CSafeArrayWrapper<double> saDates;
		CSafeArrayWrapper<double> saAmounts;

		__CHECK_HRESULT3(spPos->get_ContractType(&enContractType));

		if(enCtOption != enContractType)
		{
			__CHECK_HRESULT(E_FAIL, _T("Invalid position type in underlying positions."));
		}

		__CHECK_HRESULT3(spPos->get_IsSynth(&bIsSynth));
		__CHECK_HRESULT3(spPos->get_RootID(&nOptRootID));
		__CHECK_HRESULT(spPos->get_SynthGreek(&spSynthGreeks), _T("Fail to get position synthetic greeks."));
		__CHECK_HRESULT3(spPos->ClearValues());

		// clear synthetic components values
		if(VARIANT_FALSE != bIsSynth && spSynthGreeks != NULL)
		{
			IMmGaSynthGreekAtomPtr spSynthGreek;
			__CHECK_HRESULT(spSynthGreeks->get__NewEnum(&spUnk), _T("Fail to get position synthetic greeks."));

			IEnumVARIANTPtr spSynthGreeksEnum(spUnk);
			__CHECK_HRESULT(spSynthGreeksEnum->Reset(), _T("Fail to reset position synthetic greeks."));
			while((hr = spSynthGreeksEnum->Next(1L, &varItem, &nFetched)) == S_OK)
			{
				ATLASSERT(varItem.vt == VT_DISPATCH);
				spSynthGreek = varItem;
				if(nFetched > 0 && spSynthGreek != NULL)
				{
					__CHECK_HRESULT3(spSynthGreek->ClearValues());
				}
				spSynthGreek = NULL;
				varItem.Clear();
			}
			__CHECK_HRESULT(hr, _T("Fail to get next position expiry synthetic greek."));
		}

		// get underlying price for synthetic
		if(VARIANT_FALSE != bIsSynth && VARIANT_FALSE != m_bHasSynth && m_spSynthRoot != NULL)
		{
			__CHECK_HRESULT3(m_spSynthRoot->get_Item(nOptRootID, &spSynthRoot));
			if(spSynthRoot != NULL)
			{
				__CHECK_HRESULT3(_GetSyntheticUnderlyingPrice(pCollUnd, spSynthRoot, dSynthUndPriceBid, dSynthUndPriceAsk, dSynthUndPriceLast));
				dSynthUndPriceMid = m_spUndPriceProfile->GetUndPriceMid(dSynthUndPriceBid, dSynthUndPriceAsk, dSynthUndPriceLast, 
					dUndPriceTolerance, enPriceRoundingRule, NULL, FALSE);

				__CHECK_HRESULT3(spSynthRoot->get_CashValue(&dCashValue));
			}
		}

		if((VARIANT_FALSE == bIsSynth && dUndPriceMid > 0.) || (VARIANT_FALSE != bIsSynth && dSynthUndPriceMid > 0.))
		{
			DOUBLE dOptPriceBid = 0., dOptPriceAsk = 0., dOptPriceLast = 0., dOptPriceMid = 0.;
			LONG nQtyInShares = 0L; VARIANT_BOOL bIsBasket = VARIANT_FALSE;
			LONG nBaskDivCount = 0L;

			__CHECK_HRESULT3(spPos->get_PriceBid(&dOptPriceBid));
			__CHECK_HRESULT3(spPos->get_PriceAsk(&dOptPriceAsk));
			__CHECK_HRESULT3(spPos->get_PriceLast(&dOptPriceLast));
			__CHECK_HRESULT3(spPos->get_QtyInShares(&nQtyInShares));

			EtsReplacePriceStatusEnum enRepStatus = enRpsNone ;
			dOptPriceMid = m_spOptPriceProfile->GetOptPriceMid(dOptPriceBid, dOptPriceAsk, dOptPriceLast, enPriceRoundingRule, bUseTheoVolatility, 0, &enRepStatus);

			GREEKS aGreeks;
			aGreeks.nMask = GT_DELTA | GT_GAMMA | GT_VEGA | GT_RHO ;
			DOUBLE dPosDelta = BAD_DOUBLE_VALUE, dPosGamma = BAD_DOUBLE_VALUE;
			DOUBLE dPosVega = BAD_DOUBLE_VALUE, dPosTimeValue = BAD_DOUBLE_VALUE;
			DOUBLE dPosRho = BAD_DOUBLE_VALUE ;
			DATE dtTemp = 0.;

			LONG nModel = static_cast<LONG>(enCalcModel);
			LONG nIsAmerican = (m_bIsAmerican ? 1L : 0L);

			DOUBLE dtExpiry, dtExpiryOV, dtTradingClose, dtCalcDate;
			DOUBLE dYTE;
			
			__CHECK_HRESULT3(spPos->get_Expiry(&dtExpiry));
			__CHECK_HRESULT3(spPos->get_ExpiryOV(&dtExpiryOV));
			__CHECK_HRESULT3(spPos->get_TradingClose(&dtTradingClose));

			::GetNYDateTimeAsDATE(&dtCalcDate);
			::GetCalculationParams(dtCalcDate, dtExpiryOV, dtTradingClose, pParams->UseTimePrecision != VARIANT_FALSE, &dtCalcDate, &dtExpiryOV, &dtTradingClose, &dYTE);

			LONG nDivCount = 0L, nRetCount = 0L;
			DOUBLE dYield = 0.;

			DOUBLE dRate = 0.;
			DOUBLE dHTBRate = BAD_DOUBLE_VALUE;
			DOUBLE dStrike = 0., dVola = 0.;
			EtsOptionTypeEnum enOptType = enOtPut;

			__CHECK_HRESULT3(spExp->get_Rate(&dRate));
			__CHECK_HRESULT3(spExp->get_HTBRate(&dHTBRate));
			__CHECK_HRESULT3(spPos->get_Strike(&dStrike));
			__CHECK_HRESULT3(spPos->get_OptType(&enOptType));

			// calc common position
			if(VARIANT_FALSE == bIsSynth && dUndPriceMid > 0.)
			{

				EtsDivTypeEnum enDivType = enDivCustomStream;
				IEtsIndexDivAtomPtr spDiv = m_spDividend;
				if(spDiv)
					spDiv->get_DivType(&enDivType);

				switch(enDivType)
				{
				case enDivMarket:
				case enDivCustomPeriodical:
				case enDivCustomStream:
					{
						if (m_spDividend != NULL)
							nDivCount = 0;
						m_spDividend->GetDividendCount2(dtCalcDate, dtExpiryOV, dtTradingClose, &nDivCount);
						if (nDivCount< 0)
							nDivCount = 0;

						if (nDivCount> 0)
						{
							LPSAFEARRAY psaDates = NULL;
							LPSAFEARRAY psaAmounts = NULL;

							m_spDividend->GetDividends2(dtCalcDate, dtExpiryOV, dtTradingClose, nDivCount, &psaAmounts, &psaDates, &nDivCount);

							saDates.Attach(psaDates);
							saAmounts.Attach(psaAmounts); 
						}

					}
					break;
				case enDivStockBasket:
					{
						VARIANT_BOOL bIsBasket = VARIANT_FALSE;
						if(m_spBasketIndex != NULL)
						{
							nDivCount = 0;
							IEtsIndexDivCollPtr spDivColl = NULL;
							m_spBasketIndex->get_BasketDivs(&spDivColl);
							_CHK(m_spBasketIndex->get_IsBasket(&bIsBasket));

							if(bIsBasket && spDivColl != NULL)
							{
								spDivColl->GetDividendCount2(dtCalcDate, dtExpiryOV, dtTradingClose, &nDivCount);
								if(nDivCount > 0L)
								{
									LPSAFEARRAY psaDates = NULL;
									LPSAFEARRAY psaAmounts = NULL;
									spDivColl->GetDividends2(dtCalcDate, dtExpiryOV, dtTradingClose, nDivCount, &psaAmounts, &psaDates, &nDivCount);

									saDates.Attach(psaDates);
									saAmounts.Attach(psaAmounts);

								}
							}
						}
					}
					break;
				case enDivIndexYield:
					{
						dYield = m_dYield;
					}
					break;	
				}


				if(nDivCount < 0)
					nDivCount = 0;

				if(VARIANT_FALSE != bUseTheoVolatility)
				{
					__CHECK_HRESULT3(spPos->get_Vola(&dVola));
				}
				else
				{
					if(VARIANT_FALSE == bUseTheoVolaNoBid || VARIANT_FALSE != bUseTheoVolaNoBid && dOptPriceBid > 0.)
					{
						LONG	nFlag = 0L;
						if(dOptPriceMid > 0.)
						{
							dVola = ::CalcVolatilityMM3(dRate, dYield, dHTBRate, dUndPriceMid, dOptPriceMid, dStrike, 
								dYTE, enOptType, nIsAmerican, nDivCount, saAmounts.GetPlainData(), saDates.GetPlainData(), 
								100L, m_dSkew, m_dKurt, nModel, &nFlag);


							if (bUseTheoVolaBadMarket == VARIANT_TRUE && nFlag != VF_OK)
							{
								__CHECK_HRESULT3(spPos->get_Vola(&dVola));
							}
						}
						else if (bUseTheoVolaBadMarket == VARIANT_TRUE)
						{
							__CHECK_HRESULT3(spPos->get_Vola(&dVola));
						}
					}
					else
					{
						__CHECK_HRESULT3(spPos->get_Vola(&dVola));
					}
				}

				nRetCount = ::CalcGreeksMM2(dRate, dYield, dHTBRate, dUndPriceMid, dStrike, dVola, dYTE,
					enOptType, nIsAmerican, nDivCount, saAmounts.GetPlainData(), saDates.GetPlainData(), 100L, m_dSkew, m_dKurt, nModel, &aGreeks);

				if (DoubleEQZero(dOptPriceMid) || DoubleEQZero(dOptPriceBid)|| DoubleEQZero(dOptPriceAsk) || DoubleEQZero(dOptPriceLast))
					dOptPriceMid = m_spOptPriceProfile->GetOptPriceMid(dOptPriceBid, dOptPriceAsk, dOptPriceLast, enPriceRoundingRule, bUseTheoVolatility, aGreeks.dTheoPrice, NULL);


				if(nRetCount != 0L)
				{
					if((aGreeks.nMask & GT_DELTA) && _finite(aGreeks.dDelta))
						dPosDelta = aGreeks.dDelta * nQtyInShares;

					if((aGreeks.nMask & GT_GAMMA) && _finite(aGreeks.dGamma))
						dPosGamma = aGreeks.dGamma * nQtyInShares;

					if((aGreeks.nMask & GT_VEGA) && _finite(aGreeks.dVega))
						dPosVega = aGreeks.dVega * nQtyInShares;

					if((aGreeks.nMask & GT_RHO) && _finite(aGreeks.dRho))
						dPosRho = aGreeks.dRho * nQtyInShares;
				}

				dOptPriceMid = m_spOptPriceProfile->GetOptPriceMid(dOptPriceBid, dOptPriceAsk, dOptPriceLast, enPriceRoundingRule, bUseTheoVolatility, aGreeks.dTheoPrice, &enRepStatus);

				DOUBLE dOptPricePnL = 0.;
				VARIANT_BOOL bUseMidMarketForPnL = m_spOptPriceProfile->UseMidMarketForPnL;
				VARIANT_BOOL bUseZeroBidRule = m_spOptPriceProfile->UseZeroBidRule;

				if (bUseMidMarketForPnL == VARIANT_TRUE && bUseZeroBidRule != VARIANT_TRUE)
					dOptPricePnL = dOptPriceMid;
				else
				{
					if(nQtyInShares > 0.)
						dOptPricePnL = m_spOptPriceProfile->GetOptPriceBidForPnL(dOptPriceBid, dOptPriceAsk, dOptPriceLast, enPriceRoundingRule, bUseTheoVolatility, aGreeks.dTheoPrice, NULL);
					else
						dOptPricePnL = m_spOptPriceProfile->GetOptPriceAskForPnL(dOptPriceBid, dOptPriceAsk, dOptPriceLast, enPriceRoundingRule, bUseTheoVolatility, aGreeks.dTheoPrice, NULL);
				}

				if(dOptPricePnL > 0.)
				{
					if(enOptType == enOtCall)
						dPosTimeValue = dOptPricePnL - (dUndPriceMid - dStrike > 0 ? dUndPriceMid - dStrike : 0.);
					else
						dPosTimeValue = dOptPricePnL - (dStrike - dUndPriceMid > 0 ? dStrike - dUndPriceMid : 0.);

					dPosTimeValue = max(0., dPosTimeValue) * nQtyInShares;
				}

				if(dPosDelta > BAD_DOUBLE_VALUE)
				{
					__CHECK_HRESULT3(spPos->put_DeltaInShares(dPosDelta));
					__CHECK_HRESULT3(spPos->put_DeltaEq(dUndPriceMid * dPosDelta));
				}

				if(dPosGamma > BAD_DOUBLE_VALUE)
				{
					__CHECK_HRESULT3(spPos->put_GammaInShares(dPosGamma));
					__CHECK_HRESULT3(spPos->put_GammaEq(dUndPriceMid * dUndPriceMid * dPosGamma / 100.));
				}
			}
			// calc synthetic position
			else if(spSynthRoot != NULL && dSynthUndPriceMid > 0.)
			{
				__CHECK_HRESULT3(spSynthRoot->get_Basket(&bIsBasket));
				if(VARIANT_FALSE != bIsBasket)
				{
					LONG nBaskDivCount = 0L; VARIANT_BOOL bIsBasket = VARIANT_FALSE;
					nDivCount = 0;
					IEtsIndexDivCollPtr spDivColl = NULL;
					spSynthRoot->get_BasketDivs(&spDivColl);
					if (spDivColl != NULL)
					{
						spDivColl->GetDividendCount2(dtCalcDate, dtExpiryOV, dtTradingClose, &nDivCount);
						if(nDivCount > 0L)
						{
							LPSAFEARRAY psaDates = NULL;
							LPSAFEARRAY psaAmounts = NULL;
							spDivColl->GetDividends2(dtCalcDate, dtExpiryOV, dtTradingClose, nDivCount, &psaAmounts, &psaDates, &nRetCount);

							saDates.Attach(psaDates);
							saAmounts.Attach(psaAmounts);
						}
					}
					dYield = bIsBasket?0.0:m_dYield;
				}
				else
					__CHECK_HRESULT3(spSynthRoot->get_Yield(&dYield));

				if(nDivCount < 0)
					nDivCount = 0;

				DOUBLE dSkew = 0., dKurt = 0.;
				__CHECK_HRESULT3(spSynthRoot->get_Skew(&dSkew));
				__CHECK_HRESULT3(spSynthRoot->get_Kurt(&dKurt));

				if(VARIANT_FALSE != bUseTheoVolatility)
				{
					__CHECK_HRESULT3(spPos->get_Vola(&dVola));
				}
				else
				{
					if(VARIANT_FALSE == bUseTheoVolaNoBid || VARIANT_FALSE != bUseTheoVolaNoBid && dOptPriceBid > 0.)
					{
						LONG	nFlag = 0L;
						if(dOptPriceMid > 0.)
						{
							dVola = ::CalcVolatilityMM3(dRate, dYield, dHTBRate, dSynthUndPriceMid, dOptPriceMid, dStrike, 
								dYTE, enOptType, nIsAmerican, nDivCount, saAmounts.GetPlainData(), saDates.GetPlainData(), 
								100L, dSkew, dKurt, nModel, &nFlag);


							if (bUseTheoVolaBadMarket == VARIANT_TRUE && nFlag != VF_OK)
							{
								__CHECK_HRESULT3(spPos->get_Vola(&dVola));
							}
						}
						else if (bUseTheoVolaBadMarket == VARIANT_TRUE)
						{
							__CHECK_HRESULT3(spPos->get_Vola(&dVola));
						}
					}
					else
					{
						__CHECK_HRESULT3(spPos->get_Vola(&dVola));
					}
				}

				nRetCount = ::CalcGreeksMM2(dRate, dYield, dHTBRate, dSynthUndPriceMid, dStrike, dVola, dYTE,
					enOptType, nIsAmerican, nDivCount, saAmounts.GetPlainData(), saDates.GetPlainData(), 100L, dSkew, dKurt, nModel, &aGreeks);

				if (DoubleEQZero(dOptPriceMid) || DoubleEQZero(dOptPriceBid)|| DoubleEQZero(dOptPriceAsk) || DoubleEQZero(dOptPriceLast))
					dOptPriceMid = m_spOptPriceProfile->GetOptPriceMid(dOptPriceBid, dOptPriceAsk, dOptPriceLast, enPriceRoundingRule, bUseTheoVolatility, aGreeks.dTheoPrice, NULL);


				if(nRetCount != 0L)
				{
					if((aGreeks.nMask & GT_DELTA) && _finite(aGreeks.dDelta))
						dPosDelta = aGreeks.dDelta * nQtyInShares;

					if((aGreeks.nMask & GT_GAMMA) && _finite(aGreeks.dGamma))
						dPosGamma = aGreeks.dGamma * nQtyInShares;

					if((aGreeks.nMask & GT_VEGA) && _finite(aGreeks.dVega))
						dPosVega = aGreeks.dVega * nQtyInShares;

					if((aGreeks.nMask & GT_RHO) && _finite(aGreeks.dRho))
						dPosRho = aGreeks.dRho * nQtyInShares;
				}

				DOUBLE dOptPricePnL = 0.;
				VARIANT_BOOL bUseMidMarketForPnL = m_spOptPriceProfile->UseMidMarketForPnL;
				if (bUseMidMarketForPnL == VARIANT_TRUE)
					dOptPricePnL = dOptPriceMid;
				else
				{
					if(nQtyInShares > 0.)
						dOptPricePnL = m_spOptPriceProfile->GetOptPriceBidForPnL(dOptPriceBid, dOptPriceAsk, dOptPriceLast, enPriceRoundingRule, bUseTheoVolatility, aGreeks.dTheoPrice, NULL);
					else
						dOptPricePnL = m_spOptPriceProfile->GetOptPriceAskForPnL(dOptPriceBid, dOptPriceAsk, dOptPriceLast, enPriceRoundingRule, bUseTheoVolatility, aGreeks.dTheoPrice, NULL);
				}

				if(dOptPricePnL > 0.)
				{
					if(enOptType == enOtCall)
						dPosTimeValue = dOptPricePnL - (dSynthUndPriceMid - dStrike > 0 ? dSynthUndPriceMid - dStrike : 0.);
					else
						dPosTimeValue = dOptPricePnL - (dStrike - dSynthUndPriceMid > 0 ? dStrike - dSynthUndPriceMid : 0.);

					dPosTimeValue = max(0., dPosTimeValue) * nQtyInShares;
				}

				DOUBLE dWeight = 0.;
				ISynthRootCompCollPtr spSyntRootComps;
				ISynthRootCompAtomPtr spSyntRootComp;
				__CHECK_HRESULT3(spSynthRoot->get_SynthRootComponents(&spSyntRootComps));

				if(spSyntRootComps != NULL)
				{
					__CHECK_HRESULT3(spSyntRootComps->get_Item(m_nID, &spSyntRootComp));

					if(spSyntRootComp != NULL && dUndPriceMid > 0.)
					{
						__CHECK_HRESULT3(spSyntRootComp->get_Weight(&dWeight));
						if(dPosDelta > BAD_DOUBLE_VALUE)
						{
							__CHECK_HRESULT3(spPos->put_DeltaInShares(dPosDelta * dWeight));
							__CHECK_HRESULT3(spPos->put_DeltaEq(dUndPriceMid * dPosDelta * dWeight));
						}

						if(dPosGamma > BAD_DOUBLE_VALUE)
						{
							__CHECK_HRESULT3(spPos->put_GammaInShares(dPosGamma * dWeight));
							__CHECK_HRESULT3(spPos->put_GammaEq(dUndPriceMid * dUndPriceMid * dPosGamma * dWeight / 100.));
						}
					}

					IMmGaSynthGreekAtomPtr spSynthGreek;
					__CHECK_HRESULT(spSynthGreeks->get__NewEnum(&spUnk), _T("Fail to get position synthetic greeks."));

					IEnumVARIANTPtr spSynthGreeksEnum(spUnk);
					__CHECK_HRESULT(spSynthGreeksEnum->Reset(), _T("Fail to reset position synthetic greeks."));
					while((hr = spSynthGreeksEnum->Next(1L, &varItem, &nFetched)) == S_OK)
					{
						ATLASSERT(varItem.vt == VT_DISPATCH);
						spSynthGreek = varItem;
						if(nFetched > 0 && spSynthGreek != NULL)
						{
							LONG nSynthUndID;
							__CHECK_HRESULT3(spSynthGreek->get_UndID(&nSynthUndID));

							if(nSynthUndID != USD_ID)
							{
								__CHECK_HRESULT3(spSyntRootComps->get_Item(nSynthUndID, &spSyntRootComp));
								if(spSyntRootComp != NULL)
								{
									__CHECK_HRESULT3(spSyntRootComp->get_Weight(&dWeight));
									if(dPosDelta > BAD_DOUBLE_VALUE)
										__CHECK_HRESULT3(spSynthGreek->put_DeltaInShares(dPosDelta * dWeight));
									if(dPosGamma > BAD_DOUBLE_VALUE)
										__CHECK_HRESULT3(spSynthGreek->put_GammaInShares(dPosGamma * dWeight));

									IMmGaUndAtomPtr spUnd;
									__CHECK_HRESULT3(spCollUnd->get_Item(nSynthUndID, &spUnd));
									if(spUnd != NULL)
									{
										__CHECK_HRESULT3(spUnd->get_PriceBid(&dSynthUndPriceBid));
										__CHECK_HRESULT3(spUnd->get_PriceAsk(&dSynthUndPriceAsk));
										__CHECK_HRESULT3(spUnd->get_PriceLast(&dSynthUndPriceLast));										
										dSynthUndPriceMid = m_spUndPriceProfile->GetUndPriceMid(dSynthUndPriceBid, dSynthUndPriceAsk, dSynthUndPriceLast, dUndPriceTolerance, enPriceRoundingRule, NULL, FALSE);

										if(dSynthUndPriceMid > 0.)
										{
											if(dPosDelta > BAD_DOUBLE_VALUE)
												__CHECK_HRESULT3(spSynthGreek->put_DeltaEq(dPosDelta * dWeight * dSynthUndPriceMid));
											if(dPosGamma > BAD_DOUBLE_VALUE)
												__CHECK_HRESULT3(spSynthGreek->put_GammaEq(dPosGamma * dWeight * dSynthUndPriceMid * dSynthUndPriceMid / 100.));
										}
									}
								}
							}
							else
							{
								if(dPosDelta > BAD_DOUBLE_VALUE)
								{
									__CHECK_HRESULT3(spSynthGreek->put_DeltaInShares(dPosDelta * dCashValue));
									__CHECK_HRESULT3(spSynthGreek->put_DeltaEq(dPosDelta * dCashValue));
								}

								if(dPosGamma > BAD_DOUBLE_VALUE)
								{
									__CHECK_HRESULT3(spSynthGreek->put_GammaInShares(dPosGamma * dCashValue));
									__CHECK_HRESULT3(spSynthGreek->put_GammaEq(dPosGamma * dCashValue / 100.));
								}
							}
						}
						spSynthGreek = NULL;
						varItem.Clear();
					}
					__CHECK_HRESULT(hr, _T("Fail to get next position expiry synthetic greek."));
				}
			}

			if(dPosTimeValue > BAD_DOUBLE_VALUE)
			{
				__CHECK_HRESULT3(spPos->put_TimeValueInShares(dPosTimeValue));
			}

			if(dPosVega > BAD_DOUBLE_VALUE)
			{
				__CHECK_HRESULT3(spPos->put_VegaInShares(dPosVega));
			}

			if(dPosRho > BAD_DOUBLE_VALUE)
			{
				__CHECK_HRESULT3(spPos->put_RhoInShares(dPosRho));
			}
		}    
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)CComErrorWrapper::ErrorDescription(e), IID_IMmGaUndAtom, e.Error());
	}

	return S_OK;
}

/////////////////////////////////////////////////////////////////////////////
//
HRESULT CMmGaUndAtom::_GetSyntheticUnderlyingPrice(IMmGaUndColl* pCollUnd, ISynthRootAtom* pSynthRoot,
													DOUBLE& dSpotBid, DOUBLE& dSpotAsk, DOUBLE& dSpotLast)
{
	try
	{
		HRESULT hr;
		IMmGaUndCollPtr spCollUnd(pCollUnd);
		IMmGaUndAtomPtr spSynthUnd;
		ISynthRootAtomPtr spSynthRoot(pSynthRoot);
		ISynthRootCompCollPtr spSynthRootCompColl;
		ISynthRootCompAtomPtr spSynthComp;
		bool bBadSpotBid = false, bBadSpotAsk = false, bBadSpotLast = false;
		
		dSpotBid = 0.;
		dSpotAsk = 0.;
		dSpotLast = 0.;

		__CHECK_HRESULT3(spSynthRoot->get_CashValue(&dSpotBid));
		dSpotAsk = dSpotBid;
		dSpotLast = dSpotBid;

		IUnknownPtr spUnk;
		_variant_t varItem;
		ULONG nFetched = 0L;
			
		__CHECK_HRESULT(spSynthRoot->get_SynthRootComponents(&spSynthRootCompColl), _T("Fail to get synthetic underlying component collection."));

		__CHECK_HRESULT(spSynthRootCompColl->get__NewEnum(&spUnk), _T("Fail to get synthetic underlying component collection enum."));
		IEnumVARIANTPtr spSynthRootCompEnum(spUnk);
		__CHECK_HRESULT(spSynthRootCompEnum->Reset(), _T("Fail to reset synthetic underlying component collection."));
		while((hr = spSynthRootCompEnum->Next(1L, &varItem, &nFetched)) == S_OK)
		{
			ATLASSERT(varItem.vt == VT_DISPATCH);
			spSynthComp = varItem;
			if(nFetched > 0 && spSynthComp != NULL)
			{
				LONG nUndID = 0L;
				DOUBLE dWeight = 0., dPrice = 0.;

				spSynthComp->get_UndID(&nUndID);
				spSynthComp->get_Weight(&dWeight);

				if(nUndID != m_nID)
				{
					__CHECK_HRESULT(spCollUnd->get_Item(nUndID, &spSynthUnd),_T("Fail to get synthetic underlying."));
					
					if(spSynthUnd != NULL)
					{
						__CHECK_HRESULT3(spSynthUnd->get_PriceBid(&dPrice));
						if(!bBadSpotBid && dPrice > 0.)
						{
							dSpotBid += dPrice * dWeight;
						}
						else
						{
							bBadSpotBid = true;
							dSpotBid = 0.;
						}

						__CHECK_HRESULT3(spSynthUnd->get_PriceAsk(&dPrice));
						if(!bBadSpotAsk && dPrice > 0.)
						{
							dSpotAsk += dPrice * dWeight;
						}
						else
						{
							bBadSpotAsk = true;
							dSpotAsk = 0.;
						}

						__CHECK_HRESULT3(spSynthUnd->get_PriceLast(&dPrice));
						if(!bBadSpotLast && dPrice > 0.)
						{
							dSpotLast += dPrice * dWeight;
						}
						else
						{
							bBadSpotLast = true;
							dSpotLast = 0.;
						}
					}
				}
				else
				{
					if(!bBadSpotBid && m_dPriceBid > 0.)
					{
						dSpotBid += m_dPriceBid * dWeight;
					}
					else
					{
						bBadSpotBid = true;
						dSpotBid = 0.;
					}

					if(!bBadSpotAsk && m_dPriceAsk > 0.)
					{
						dSpotAsk += m_dPriceAsk * dWeight;
					}
					else
					{
						bBadSpotAsk = true;
						dSpotAsk = 0.;
					}

					if(!bBadSpotLast && m_dPriceLast > 0.)
					{
						dSpotLast += m_dPriceLast * dWeight;
					}
					else
					{
						bBadSpotLast = true;
						dSpotLast = 0.;
					}
				}
			}
			spSynthComp = NULL;
			varItem.Clear();
		}
		__CHECK_HRESULT(hr, _T("Fail to get next synthetic underlying component."));
		return S_OK;
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)CComErrorWrapper::ErrorDescription(e), IID_IMmGaUndAtom, e.Error());
	}
}

/////////////////////////////////////////////////////////////////////////////
//
void CMmGaUndAtom::_GetBasketIndexDividends(REGULAR_DIVIDENDS* pDivs, LONG nMaxCount) throw()
{
	EtsRegularDividend aDiv;

	for(LONG i = 0; i < nMaxCount; i++)
	{
		__CHECK_HRESULT(m_spBasketIndex->get_BasketDiv(i, &aDiv), _T("Fail to get basket dividend."));
		pDivs[i].nLastDivDate = static_cast<LONG>(aDiv.LastDate);
		pDivs[i].nFrequency = aDiv.Freq;
		pDivs[i].dAmount = aDiv.Amt;
	}
}

/////////////////////////////////////////////////////////////////////////////
//
void CMmGaUndAtom::_GetSyntheticRootBasketDividends(ISynthRootAtom* pSynthRoot, REGULAR_DIVIDENDS* pDivs, 
													LONG nMaxCount) throw()
{
	ISynthRootAtomPtr spSynthRoot(pSynthRoot);
	EtsRegularDividend aDiv;

	for(LONG i = 0; i < nMaxCount; i++)
	{
		__CHECK_HRESULT(spSynthRoot->get_BasketDiv(i, &aDiv), _T("Fail to get basket dividend."));
		pDivs[i].nLastDivDate = static_cast<LONG>(aDiv.LastDate);
		pDivs[i].nFrequency = aDiv.Freq;
		pDivs[i].dAmount = aDiv.Amt;
	}
}

// MmVaUndAtom.cpp : Implementation of CMmVaUndAtom

#include "stdafx.h"
#include "MmVaUndAtom.h"


/////////////////////////////////////////////////////////////////////////////
//
void CMmVaUndAtom::_GetBasketIndexDividends(REGULAR_DIVIDENDS* pDivs, LONG nMaxCount) throw()
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
STDMETHODIMP CMmVaUndAtom::CalcCurrentValues(DOUBLE dVegaAmount, EtsCalcModelTypeEnum enCalcModel,
											VARIANT_BOOL bUseTheoVolatility,
											ICalculationParametrs* pParams)
{
	try
	{
		CSafeArrayWrapper<double> saDates;
		CSafeArrayWrapper<double> saAmounts;

		if(m_spCurExpiry != NULL)
		{
			HRESULT hr = S_OK;

			IMmVaStrikeAtomPtr spAtmStr;
			__CHECK_HRESULT3(m_spCurExpiry->get_AtmStrike(&spAtmStr));

			if(spAtmStr != NULL)
			{
				DOUBLE dStrike = 0.;
				__CHECK_HRESULT3(spAtmStr->get_Strike(&dStrike));

				DATE	dtTemp = 0., dTemp = 0., dtExpiryOV = 0., tmCloseTime = 0., dtNow = 0.;
				DOUBLE	dYTE = 0.;
				bool	bUseTimePrecision = true;
				GREEKS	aGreeks;

				__CHECK_HRESULT3(m_spCurExpiry->get_ExpiryOV(&dtExpiryOV));
				__CHECK_HRESULT3(m_spCurExpiry->get_TradingClose(&tmCloseTime));
				if (pParams)
					bUseTimePrecision = (pParams->UseTimePrecision != VARIANT_FALSE);

				::GetNYDateTimeAsDATE(&dtNow);
				::GetCalculationParams(dtNow, dtExpiryOV, tmCloseTime, bUseTimePrecision, &dtNow, &dtExpiryOV, &tmCloseTime, &dYTE);

				LONG nDivCount = 0L, nRetCount = 0L;
				DOUBLE dYield = 0.;

				EtsDivTypeEnum enDivType = enDivCustomStream;
				IEtsIndexDivAtomPtr spDiv = m_spDividend;
				if (spDiv)
					spDiv->get_DivType(&enDivType);

				switch(enDivType)
				{
				case enDivMarket:
				case enDivCustomPeriodical:
				case enDivCustomStream:
					{
						if (m_spDividend != NULL)
							nDivCount = 0;
						m_spDividend->GetDividendCount2(dtNow, dtExpiryOV, tmCloseTime, &nDivCount);
						if (nDivCount< 0)
							nDivCount = 0;

						if (nDivCount> 0)
						{
							LPSAFEARRAY psaDates = NULL;
							LPSAFEARRAY psaAmounts = NULL;
							m_spDividend->GetDividends2(dtNow, dtExpiryOV, tmCloseTime, nDivCount, &psaAmounts, &psaDates, &nRetCount);
							saDates.Attach(psaDates);
							saAmounts.Attach(psaAmounts);
						}
					}
					break;
				case enDivStockBasket:
					{
						dYield = m_dYield;
						if(m_spBasketIndex != NULL)
						{
							LONG nBaskDivCount = 0L; VARIANT_BOOL bIsBasket = VARIANT_FALSE;
							nDivCount = 0;
							IEtsIndexDivCollPtr spDivColl = NULL;
							m_spBasketIndex->get_BasketDivs(&spDivColl);
							_CHK(m_spBasketIndex->get_IsBasket(&bIsBasket));

							if(bIsBasket)
								dYield = 0.0;

							if ( bIsBasket && spDivColl != NULL)
							{
								spDivColl->GetDividendCount2(dtNow, dtExpiryOV, tmCloseTime, &nDivCount);
								if(nDivCount > 0L)
								{
									LPSAFEARRAY psaDates = NULL;
									LPSAFEARRAY psaAmounts = NULL;
									spDivColl->GetDividends2(dtNow, dtExpiryOV, tmCloseTime, nDivCount, &psaAmounts, &psaDates, &nDivCount);
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

				if(nDivCount <= 0)
					nDivCount = 0;


				DOUBLE dRate = 0.;
				__CHECK_HRESULT3(m_spCurExpiry->get_Rate(&dRate));

				DOUBLE dHTBRate = BAD_DOUBLE_VALUE;
				__CHECK_HRESULT3(m_spCurExpiry->get_HTBRate(&dHTBRate));

				LONG nModel = static_cast<LONG>(enCalcModel);
				LONG nIsAmerican = (m_bIsAmerican ? 1L : 0L);

				IUnknownPtr spUnk;
				_variant_t varItem;
				ULONG nFetched = 0L;

				IMmVaOptPairAtomPtr spOptPair;
				IMmVaOptPairCollPtr spOptPairs;
				__CHECK_HRESULT(spAtmStr->get_OptPair(&spOptPairs), _T("Fail to get option pairs collection."));
				__CHECK_HRESULT(spOptPairs->get__NewEnum(&spUnk), _T("Fail to get option pairs collection."));

				IEnumVARIANTPtr spOptPairEnum(spUnk);
				__CHECK_HRESULT(spOptPairEnum->Reset(), _T("Fail to reset option pairs collection."));

				while((hr = spOptPairEnum->Next(1L, &varItem, &nFetched)) == S_OK)
				{
					ATLASSERT(varItem.vt == VT_DISPATCH);
					spOptPair = varItem;
					if(nFetched > 0 && spOptPair != NULL)
					{
						IMmVaOptAtomPtr spOptCall;
						IMmVaOptAtomPtr spOptPut;

						__CHECK_HRESULT3(spOptPair->ClearValues());
						__CHECK_HRESULT3(spOptPair->get_Opt(enOtCall, &spOptCall));
						__CHECK_HRESULT3(spOptPair->get_Opt(enOtPut, &spOptPut));

						LONG nOptCallID = 0L, nOptPutID = 0L;
						if(spOptCall != NULL)
						{
							__CHECK_HRESULT3(spOptCall->get_ID(&nOptCallID));
						}

						if(spOptPut != NULL)
						{
							__CHECK_HRESULT3(spOptPut->get_ID(&nOptPutID));
						}

						LONG nLotSize = BAD_LONG_VALUE;
						__CHECK_HRESULT3(spOptPair->get_LotSize(&nLotSize));

						if(nLotSize > 0L && nOptCallID != 0L && nOptPutID != 0L)
						{
							DOUBLE dOptCallBid = 0., dOptCallAsk = 0., dOptCallLast = 0.;
							DOUBLE dOptPutBid = 0., dOptPutAsk = 0., dOptPutLast = 0.;
							DOUBLE dIvCallBid = 0., dIvCallAsk = 0.;
							DOUBLE dIvPutBid = 0., dIvPutAsk = 0.;
							DOUBLE dVegaCallBid = BAD_DOUBLE_VALUE, dVegaPutBid = BAD_DOUBLE_VALUE;
							DOUBLE dVegaCallAsk = BAD_DOUBLE_VALUE, dVegaPutAsk = BAD_DOUBLE_VALUE;
							DOUBLE dDeltaCallBid = BAD_DOUBLE_VALUE, dDeltaPutBid = BAD_DOUBLE_VALUE;
							DOUBLE dDeltaCallAsk = BAD_DOUBLE_VALUE, dDeltaPutAsk = BAD_DOUBLE_VALUE;
							LONG nContractsBid = BAD_LONG_VALUE, nContractsAsk = BAD_LONG_VALUE;

							__CHECK_HRESULT3(spOptCall->get_PriceBid(&dOptCallBid));
							__CHECK_HRESULT3(spOptCall->get_PriceAsk(&dOptCallAsk));
							__CHECK_HRESULT3(spOptCall->get_PriceLast(&dOptCallLast));
							__CHECK_HRESULT3(spOptCall->ClearValues(VARIANT_TRUE));

							__CHECK_HRESULT3(spOptPut->get_PriceBid(&dOptPutBid));
							__CHECK_HRESULT3(spOptPut->get_PriceAsk(&dOptPutAsk));
							__CHECK_HRESULT3(spOptPut->get_PriceLast(&dOptPutLast));
							__CHECK_HRESULT3(spOptPut->ClearValues(VARIANT_TRUE));

							// calc straddle bid
							if(dOptCallBid >= 0. && dOptPutBid >= 0.)
							{
								__CHECK_HRESULT3(spOptPair->put_StraddleBid(dOptCallBid + dOptPutBid));
							}

							// calc straddle ask
							if(dOptCallAsk >= 0. && dOptPutAsk >= 0.)
							{
								__CHECK_HRESULT3(spOptPair->put_StraddleAsk(dOptCallAsk + dOptPutAsk));
							}

							if(m_dPriceLast > 0.)
							{
								// calc iv bid for call
								if(dOptCallBid > 0.)
								{
									dIvCallBid = ::CalcVolatilityMM2(dRate, dYield, dHTBRate, m_dPriceLast, dOptCallBid, dStrike, dYTE,
												enOtCall, nIsAmerican, nDivCount, saAmounts.GetPlainData(), saDates.GetPlainData(), 100L, m_dSkew, m_dKurt, nModel);
									if(dIvCallBid > 0.)
									{
										__CHECK_HRESULT3(spOptCall->put_IVBid(dIvCallBid));
									}
								}

								// calc iv bid for ask
								if(dOptPutBid > 0.)
								{
									dIvPutBid = ::CalcVolatilityMM2(dRate, dYield, dHTBRate, m_dPriceLast, dOptPutBid, dStrike, dYTE,
												enOtPut, nIsAmerican, nDivCount, saAmounts.GetPlainData(), saDates.GetPlainData(), 100L, m_dSkew, m_dKurt, nModel);
									if(dIvPutBid > 0.)
									{
										__CHECK_HRESULT3(spOptPut->put_IVBid(dIvPutBid));
									}
								}

								// calc straddle iv bid
								if(dIvCallBid > 0. && dIvPutBid > 0.)
								{
									__CHECK_HRESULT3(spOptPair->put_StraddleIVBid((dIvCallBid + dIvPutBid) * 100. / 2.));
								}

								// calc vega & delta bid for call
								if(dIvCallBid > 0.)
								{
									aGreeks.nMask = GT_VEGA | GT_DELTA;
									nRetCount = ::CalcGreeksMM2(dRate, dYield, dHTBRate, m_dPriceLast, dStrike, dIvCallBid, dYTE,
												enOtCall, nIsAmerican, nDivCount, saAmounts.GetPlainData(), saDates.GetPlainData(), 100L, m_dSkew, m_dKurt, nModel, &aGreeks);
									if(nRetCount != 0L)
									{
										if((aGreeks.nMask & GT_VEGA) && _finite(aGreeks.dVega))
										{
											dVegaCallBid = aGreeks.dVega;
											__CHECK_HRESULT3(spOptCall->put_VegaBid(dVegaCallBid));
										}

										if((aGreeks.nMask & GT_DELTA) && _finite(aGreeks.dDelta))
										{
											dDeltaCallBid = aGreeks.dDelta;
											__CHECK_HRESULT3(spOptCall->put_DeltaBid(dDeltaCallBid));
										}
									}
								}

								// calc vega & delta bid for put
								if(dIvPutBid > 0.)
								{
									aGreeks.nMask = GT_VEGA | GT_DELTA;
									nRetCount = ::CalcGreeksMM2(dRate, dYield, dHTBRate, m_dPriceLast, dStrike, dIvPutBid, dYTE,
												enOtPut, nIsAmerican, nDivCount, saAmounts.GetPlainData(), saDates.GetPlainData(), 100L, m_dSkew, m_dKurt, nModel, &aGreeks);
									if(nRetCount != 0L)
									{
										if((aGreeks.nMask & GT_VEGA) && _finite(aGreeks.dVega))
										{
											dVegaPutBid = aGreeks.dVega;
											__CHECK_HRESULT3(spOptPut->put_VegaBid(dVegaPutBid));
										}

										if((aGreeks.nMask & GT_DELTA) && _finite(aGreeks.dDelta))
										{
											dDeltaPutBid = aGreeks.dDelta;
											__CHECK_HRESULT3(spOptPut->put_VegaBid(dDeltaPutBid));
										}
									}
								}

								// calc total vega bid
								if(dVegaCallBid > BAD_DOUBLE_VALUE && dVegaPutBid > BAD_DOUBLE_VALUE)
								{
									dTemp = dVegaCallBid + dVegaPutBid;
									__CHECK_HRESULT3(spOptPair->put_TotalVegaBid(dTemp * 100.));
									
									if(DoubleNEQZero(dTemp))
									{
										nContractsBid = (LONG)(floor(dVegaAmount / (dTemp * nLotSize) + .5));
										__CHECK_HRESULT3(spOptPair->put_ContractsBid(nContractsBid));
									}
								}

								// calc net delta bid
								if(dDeltaCallBid > BAD_DOUBLE_VALUE && dDeltaPutBid > BAD_DOUBLE_VALUE)
								{
									dTemp = -(dDeltaCallBid + dDeltaPutBid);
									__CHECK_HRESULT3(spOptPair->put_NetDeltaBid(dTemp * 100.));
									
									if(nContractsBid > BAD_LONG_VALUE)
									{
										__CHECK_HRESULT3(spOptPair->put_DeltaInSharesBid((LONG)floor(nContractsBid * nLotSize * dTemp + .5)));
									}
								}

								// calc iv ask for call
								if(dOptCallAsk > 0.)
								{
									dIvCallAsk = ::CalcVolatilityMM2(dRate, dYield, dHTBRate, m_dPriceLast, dOptCallAsk, dStrike, dYTE,
												enOtCall, nIsAmerican, nDivCount, saAmounts.GetPlainData(), saDates.GetPlainData(), 100L, m_dSkew, m_dKurt, nModel);
									if(dIvCallAsk > 0.)
									{
										__CHECK_HRESULT3(spOptCall->put_IVAsk(dIvCallAsk));
									}
								}

								// calc iv ask for put
								if(dOptPutAsk > 0.)
								{
									dIvPutAsk = ::CalcVolatilityMM2(dRate, dYield, dHTBRate, m_dPriceLast, dOptPutAsk, dStrike, dYTE,
												enOtPut, nIsAmerican, nDivCount, saAmounts.GetPlainData(), saDates.GetPlainData(), 100L, m_dSkew, m_dKurt, nModel);
									if(dIvPutAsk > 0.)
									{
										__CHECK_HRESULT3(spOptPut->put_IVAsk(dIvPutAsk));
									}
								}

								// calc straddle iv ask
								if(dIvCallAsk > 0. && dIvPutAsk > 0.)
								{
									__CHECK_HRESULT3(spOptPair->put_StraddleIVAsk((dIvCallAsk + dIvPutAsk) * 100. / 2.));
								}

								// calc vega & delta ask for call
								if(dIvCallAsk > 0.)
								{
									aGreeks.nMask = GT_VEGA | GT_DELTA;
									nRetCount = ::CalcGreeksMM2(dRate, dYield, dHTBRate, m_dPriceLast, dStrike, dIvCallAsk, dYTE,
												enOtCall, nIsAmerican, nDivCount, saAmounts.GetPlainData(), saDates.GetPlainData(), 100L, m_dSkew, m_dKurt, nModel, &aGreeks);
									if(nRetCount != 0L)
									{
										if((aGreeks.nMask & GT_VEGA) && _finite(aGreeks.dVega))
										{
											dVegaCallAsk = aGreeks.dVega;
											__CHECK_HRESULT3(spOptCall->put_VegaAsk(dVegaCallAsk));
										}

										if((aGreeks.nMask & GT_DELTA) && _finite(aGreeks.dDelta))
										{
											dDeltaCallAsk = aGreeks.dDelta;
											__CHECK_HRESULT3(spOptCall->put_DeltaAsk(dDeltaCallAsk));
										}
									}
								}

								// calc vega & delta ask for put
								if(dIvPutAsk > 0.)
								{
									aGreeks.nMask = GT_VEGA | GT_DELTA;
									nRetCount = ::CalcGreeksMM2(dRate, dYield, dHTBRate, m_dPriceLast, dStrike, dIvPutAsk, dYTE,
												enOtPut, nIsAmerican, nDivCount, saAmounts.GetPlainData(), saDates.GetPlainData(), 100L, m_dSkew, m_dKurt, nModel, &aGreeks);
									if(nRetCount != 0L)
									{
										if((aGreeks.nMask & GT_VEGA) && _finite(aGreeks.dVega))
										{
											dVegaPutAsk = aGreeks.dVega;
											__CHECK_HRESULT3(spOptPut->put_VegaAsk(dVegaPutAsk));
										}

										if((aGreeks.nMask & GT_DELTA) && _finite(aGreeks.dDelta))
										{
											dDeltaPutAsk = aGreeks.dDelta;
											__CHECK_HRESULT3(spOptPut->put_DeltaAsk(dDeltaPutAsk));
										}
									}
								}

								// calc total vega ask
								if(dVegaCallAsk > BAD_DOUBLE_VALUE && dVegaPutAsk > BAD_DOUBLE_VALUE)
								{
									dTemp = dVegaCallAsk + dVegaPutAsk;
									__CHECK_HRESULT3(spOptPair->put_TotalVegaAsk(dTemp * 100.));
									
									if(DoubleNEQZero(dTemp))
									{
										nContractsAsk = (LONG)(floor(dVegaAmount / (dTemp * nLotSize) + .5));
										__CHECK_HRESULT3(spOptPair->put_ContractsAsk(nContractsAsk));
									}
								}

								// calc net delta ask
								if(dDeltaCallAsk > BAD_DOUBLE_VALUE && dDeltaPutAsk > BAD_DOUBLE_VALUE)
								{
									dTemp = dDeltaCallAsk + dDeltaPutAsk;
									__CHECK_HRESULT3(spOptPair->put_NetDeltaAsk(dTemp * 100.));
									
									if(nContractsAsk > BAD_LONG_VALUE)
									{
										__CHECK_HRESULT3(spOptPair->put_DeltaInSharesAsk((LONG)floor(nContractsAsk * nLotSize * dTemp + .5)));
									}
								}
							}
						}
						spOptPair = NULL;
						varItem.Clear();
					}
					__CHECK_HRESULT(hr, _T("Fail to get next option pair."));
				}
			}
		}

	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)CComErrorWrapper::ErrorDescription(e), IID_IMmVaUndAtom, e.Error());
	}

	return S_OK;
}

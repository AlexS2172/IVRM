// EtsMmQuotesView.cpp : Implementation of CEtsMmQuotesView

#include "stdafx.h"
#include "EtsMmQuotesView.h"
#include "Boost/bind.hpp"

const double  STRIKE_LESS_SIGNIFICANT_VALUE  = 0.0001;
const long    USD_ID                         = 0;
const _bstr_t USD_SYMBOL(L"$");                  
const WCHAR* bsMonth[12] =
{
	L"JAN",
	L"FEB",
	L"MAR",
	L"APR",
	L"MAY",
	L"JUN",
	L"JUL",
	L"AUG",
	L"SEP",
	L"OCT",
	L"NOV",
	L"DEC"
};

typedef CComRecPtr<QuoteUpdateFullInfo, &LIBID_PRICEPROVIDERSLib> _QuoteUpdateFullInfo;

// CEtsMmQuotesView

HRESULT CEtsMmQuotesView::OnProgress(LONG lProgress,  LONG lMaxProgress)
{
	HRESULT hr = S_OK;
	int cConnections = m_vec.GetSize();

	for (int iConnection = 0; iConnection < cConnections; iConnection++)
	{
		Lock();
		CComPtr<IUnknown> punkConnection = m_vec.GetAt(iConnection);
		Unlock();

		IDispatch * pConnection = static_cast<IDispatch *>(punkConnection.p);

		if (pConnection)
		{
			CComVariant avarParams[2];
			avarParams[1] = lProgress;		avarParams[1].vt = VT_I4;
			avarParams[0] = lMaxProgress;	avarParams[0].vt = VT_I4;
			CComVariant varResult;

			DISPPARAMS params = { avarParams, NULL, 2, 0 };
			hr = pConnection->Invoke(1, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &params, &varResult, NULL, NULL);
		}
	}
	return hr;
}

STDMETHODIMP CEtsMmQuotesView::Load(long lGroupID)
{
	HRESULT hr = S_OK;
	try
	{
		::ResetEvent(m_hCancelEvent);
		m_Connection.Close();

		if(!m_Connection.IsOpened())
			m_Connection.Open(m_bsConnectionString, 10, 120, 300, 300);

		put_ProcessorsLoad(m_nProcessors);
		hr = LoadOptions(lGroupID);
		if(SUCCEEDED(hr))
			RenumberItems();

		m_Connection.Close();
	}
	catch (_com_error& err)
	{
		hr = Error((PTCHAR)CComErrorWrapper::ErrorDescription(err), IID_IEtsMmQuotesView, err.Error());
	}
	/*catch(...)
	{
		DWORD dwErrorCode = GetLastError();
		hr = HRESULT_FROM_WIN32(dwErrorCode);
		if(FAILED(hr))
		{
			_bstr_t bsMessage(L"Error ");
			bsMessage += _bstr_t(dwErrorCode);

			hr = Error((LPCSTR)bsMessage, IID_IEtsMmQuotesView, hr);
		}
		else
			hr = Error(_T("Unknown Error"), IID_IEtsMmQuotesView, E_FAIL);
	}*/
	return hr;
}
STDMETHODIMP CEtsMmQuotesView::Cancel()
{
	HRESULT hr = S_OK;
	try
	{
		::SetEvent(m_hCancelEvent);
	}
	catch (_com_error& e)
	{
		hr = Error((PTCHAR)CComErrorWrapper::ErrorDescription(e), IID_IEtsMmQuotesView, e.Error());
	}
	return hr;
}
	

HRESULT CEtsMmQuotesView::LoadOptions(long lGroupID)
{
	HRESULT hr   = S_OK;
	long lLoaded = 0L;
	WCHAR buffer[1024] = {0};

	try
	{
		//m_CachedExchanges.clear();
		CExchanges CachedExchanges;

		if(lGroupID)
		{
			IEtsContractCollPtr spContractAll = m_spEtsMain->ContractAll;
			IEtsContractAtomPtr spContract = spContractAll->Item[lGroupID];
			bool bHasSynteticRoots =  spContract->Und->HaveSyntheticRoots != VARIANT_FALSE;
			ISynthRootCollPtr spSynthRoots;
			if(bHasSynteticRoots)
				spSynthRoots = spContract->Und->SyntheticRoots;


			CStoredProc<> sp(m_Connection, L"usp_MmOptionByUnderlying_Get");
			sp << lGroupID;
			sp << vt_date::GetCurrentDate();

			CClientRecordset rsOpt;
			rsOpt.Open(sp);
			long lRecordCount = rsOpt.GetRecordCount();
			if(lRecordCount)
			{
				long lCurrentProgress = -1;
				IMmQvUndAtomPtr spUndAtom = m_pGrp->GetUndAtom();
				CComObject<CMmQvUndAtom>* pUndAtom = dynamic_cast<CComObject<CMmQvUndAtom>*>(spUndAtom.GetInterfacePtr());

				for(rsOpt.MoveFirst(); !rsOpt.IsEOF(); ++rsOpt, ++lLoaded)
				{
					if(WaitForSingleObject(m_hCancelEvent,0)!=WAIT_TIMEOUT)
						break;


					vt_date dtExpiry = rsOpt[L"dtExpiry"];
					vt_date dtExpiryOV		= rsOpt[L"dtExpiryOV"];
					vt_date dtTC			= rsOpt[L"dtTradingClose"];
					DOUBLE	dTradingClose	= (DATE)(dtTC);
					vt_date dtTradingClose	= vt_date(dTradingClose - floor(dTradingClose));


					vt_date	dtExpiryMonth(dtExpiry.get_year(), dtExpiry.get_month(), dtExpiry.get_day());

					CComObject<CMmQvExpAtom>* pExpAtom = NULL;
					IMmQvExpAtomPtr spExpiry = pUndAtom->GetExp()->GetExpiry(dtExpiryMonth);

					if(spExpiry == NULL)
					{
						spExpiry = pUndAtom->GetExp()->AddNew(dtExpiryMonth, &pExpAtom);
						if(pExpAtom)
						{
							pExpAtom->m_dtExpiry = dtExpiry;
							pExpAtom->m_dtExpiryOV		= dtExpiryOV;
							pExpAtom->m_dtTradingClose	= dtTradingClose;
						}
					}
					else
						pExpAtom = dynamic_cast<CComObject<CMmQvExpAtom>*>((IMmQvExpAtom*)spExpiry);


					IEtsMmEntityAtomPtr spEnt = m_spCustRates->Item[_bstr_t(static_cast<long>((DATE)dtExpiryMonth))];
					if(spEnt != NULL)
						pExpAtom->m_dRateCust = spEnt->Data3;

					long nOptRootID = rsOpt[L"iOptionRootID"];
					IMmQvOptRootAtomPtr spOptRoot = pUndAtom->GetOptRoot()->GetItem(nOptRootID);
					CComObject<CMmQvOptRootAtom>* pOptRootAtom = NULL;
					if(spOptRoot == NULL)
					{
						_bstr_t bsRootName =  rsOpt[L"vcOptionRootSymbol"];

						spOptRoot = pUndAtom->m_pOptRoot->AddNew(nOptRootID, bsRootName, &pOptRootAtom);


						pOptRootAtom->m_nLotSize = rsOpt[L"iLotSize"];
						pOptRootAtom->m_bIsFit   = (long)rsOpt[L"tiIsFitEnabled"] !=0? VARIANT_TRUE: VARIANT_FALSE;
					
						if(pOptRootAtom->m_nLotSize <= 0L)  
							pOptRootAtom->m_nLotSize = 100L;

						ISynthRootAtomPtr aSynthRoot;
						if(bHasSynteticRoots)
							aSynthRoot = spSynthRoots->Item[nOptRootID];
						
						pOptRootAtom->m_bSynthetic = aSynthRoot!=NULL ?VARIANT_TRUE:VARIANT_FALSE;

						if (pOptRootAtom->m_bSynthetic)
						{
							pOptRootAtom->m_spSynthOptRoot = aSynthRoot;
							double dCashValue = aSynthRoot->CashValue;

							double dDPC = pOptRootAtom->m_nLotSize * dCashValue;
							if (dDPC != 0)
								pOptRootAtom->m_bstrDPC = _bstr_t(dDPC)+ _bstr_t(L" USD");
							if(dCashValue >0.)
							{
								CComObject<CMmQvUndColl>* pSundColl = NULL;
								if(pUndAtom->m_spSynthUnd == NULL)
								{
									CComObject<CMmQvUndColl>::CreateInstance(&pSundColl);
									pUndAtom->m_spSynthUnd.Attach(pSundColl, TRUE);
								}
								else
									pSundColl = dynamic_cast<CComObject<CMmQvUndColl>*>((IMmQvUndColl*)pUndAtom->m_spSynthUnd);

								IMmQvUndAtomPtr spUsdUnd = pSundColl->GetUnd(USD_ID);
								if(spUsdUnd == NULL)
								{
									CComObject<CMmQvUndAtom>* _pUnd = NULL;
									pSundColl->AddNew(USD_ID, USD_SYMBOL, &_pUnd);
									if(_pUnd)
									{
										IMmQvOptRootAtomPtr spTmp;

										_pUnd->m_bstrSymbolName = USD_SYMBOL;
										_pUnd->m_enUndType		= enCtStock;
										_pUnd->m_pOptRoot->Add(nOptRootID, bsRootName, spOptRoot, &spTmp);
									}

									IExchAtomPtr spCasheAtom =_pUnd->m_spUndExch->Add(0L, L"", NULL );
									{
										spCasheAtom->Name = L"<Primary>";
										CComObject<CMmQvQuoteAtom>* pQuoteDef = NULL;

										IMmQvQuoteAtomPtr spQuoteDef =	_pUnd->m_pQuote->AddNew(0, L"",&pQuoteDef);
										if(pQuoteDef)
										{
											pQuoteDef->m_spExch   = spCasheAtom;
											pQuoteDef->m_nLotSize = pOptRootAtom->m_nLotSize;
											pQuoteDef->m_dPriceAsk = 1.;
											pQuoteDef->m_dPriceBid = 1.;
											pQuoteDef->m_dPriceLast = 1.;
										}
									}
								}
							}


							ISynthRootCompCollPtr  spSynthRootComponents = aSynthRoot->SynthRootComponents;
							if(spSynthRootComponents!=NULL)
							{
								_variant_t varItem;
								ULONG nFetched = 0L;
								IEnumVARIANTPtr spSynthRootEnum(spSynthRootComponents->_NewEnum);


								_CHK(spSynthRootEnum->Reset(), _T("Fail to reset  Syntetic Roots collection."));
								while((hr = spSynthRootEnum->Next(1L, &varItem, &nFetched)) == S_OK)
								{
									ATLASSERT(varItem.vt == VT_DISPATCH);
									if(varItem.vt == VT_DISPATCH)
									{
										ISynthRootCompAtomPtr spSynthAtom = varItem;
										IUndAtomPtr  spGUAtom = m_spEtsMain->UnderlyingAll->Item[spSynthAtom->UndID];
										if(spGUAtom!=NULL)
										{
											bool bFirst = (pOptRootAtom->m_bstrDPC.length()==0); 
											if(bFirst)
												pOptRootAtom->m_bstrDPC = _bstr_t(pOptRootAtom->m_nLotSize * spSynthAtom->Weight) + _bstr_t(L" ") + spGUAtom->Symbol;
											else
												pOptRootAtom->m_bstrDPC += _bstr_t(L" + ") + _bstr_t(pOptRootAtom->m_nLotSize * spSynthAtom->Weight) + _bstr_t(L" ") + spGUAtom->Symbol;

											if(spSynthAtom->UndID != pUndAtom->m_nID)
											{
												if(pUndAtom->m_spSynthUnd==NULL)
													pUndAtom->m_spSynthUnd = IMmQvUndCollPtr(__uuidof(MmQvUndColl)); 

												IMmQvUndAtomPtr spSynthUndAtom;
												pUndAtom->m_spSynthUnd->get_Item(spSynthAtom->UndID, &spSynthUndAtom);
												if(spSynthUndAtom == NULL)
												{

													CComObject<CMmQvUndAtom>* pSynthUnd = NULL;
													_CHK(CComObject<CMmQvUndAtom>::CreateInstance(&pSynthUnd), _T("Unable to create Synth Und Atom"));
													spSynthUndAtom.Attach(pSynthUnd, TRUE);

													spSynthAtom->get_UndID(&pSynthUnd->m_nID);

													BSTR bsSymbol= NULL;
													spGUAtom->get_Symbol(&bsSymbol);
													pSynthUnd->m_bstrSymbol.Attach(bsSymbol);

													BSTR bsContractName = NULL;
													spGUAtom->get_ContractName(&bsContractName);
													pSynthUnd->m_bstrSymbolName.Attach(bsContractName);

													spGUAtom->get_UndType(&pSynthUnd->m_enUndType);
													{
														IMmQvOptRootAtomPtr spTmp;
														pSynthUnd->m_pOptRoot->Add(pOptRootAtom->m_nID, pOptRootAtom->m_bstrName, spOptRoot, &spTmp);
													}

													IExchAtomPtr spCasheAtom = pSynthUnd->m_spUndExch->Add(0L, L"", NULL );
													{
														spCasheAtom->Name = L"<Primary>";
														CComObject<CMmQvQuoteAtom>* pQuoteDef = NULL;

														IMmQvQuoteAtomPtr spQuoteDef =	pSynthUnd->m_pQuote->AddNew(0, L"",&pQuoteDef);
														if(pQuoteDef)
														{
															pQuoteDef->m_spExch   = spCasheAtom;
															pQuoteDef->m_nLotSize = pOptRootAtom->m_nLotSize;
															pQuoteDef->m_dPriceAsk = 1.;
															pQuoteDef->m_dPriceBid = 1.;
															pQuoteDef->m_dPriceLast = 1.;
														}
													}
													CStoredProc<CClientRecordset> spUndExchanges(m_Connection, L"usp_MmUnderlyingExchanges_Get");
													spUndExchanges << spSynthAtom->UndID;
													spUndExchanges.Open();
													if(spUndExchanges.GetRecordCount())
													{
														for(spUndExchanges.MoveFirst(); !spUndExchanges.IsEOF(); ++spUndExchanges)
														{
															long lExchangeID = spUndExchanges[L"iExchangeID"];
															 IExchAtomPtr spExchAtom =  m_spEtsMain->Exch->Item[lExchangeID];
															 if(spExchAtom)
															 {
																 long lIsUnderlying = spUndExchanges[L"tiIsUnderlying"];
																 if(lIsUnderlying)
																 {
																	 if(m_pGrp->m_spUndExchAll->Item[lExchangeID]==NULL)
																		 AddExch(m_pGrp->m_spUndExchAll, lExchangeID,spExchAtom->Code, spExchAtom->Name);
																	 if(pSynthUnd->m_spUndExch->Item[lExchangeID]==NULL)
																		 AddExch(pSynthUnd->m_spUndExch, lExchangeID, spExchAtom->Code, spExchAtom->Name);
																 }
															 }
														}
													}
													if(pSynthUnd->m_spUndExch != NULL)
													{
														m_nUndQuotesCount += pSynthUnd->m_spUndExch->GetCount();

														IUnknownPtr spUnk;
														_variant_t  varItem;
														ULONG nFetched = 0;

														_CHK(pSynthUnd->m_spUndExch->get__NewEnum(&spUnk), _T("Fail to get Underlying Exchanges collection enum."));
														IEnumVARIANTPtr spUndExchEnum(spUnk);

														_CHK(spUndExchEnum->Reset(), _T("Fail to reset  Exchanges collection."));
														while((hr = spUndExchEnum->Next(1L, &varItem, &nFetched)) == S_OK)
														{
															IExchAtomPtr spSyntExchange = varItem;
															if(spSyntExchange!=NULL)
															{
																CComObject<CMmQvQuoteAtom>* _pAtom= NULL;
																long    lSynExchangeID    = spSyntExchange->ID;
																_bstr_t bsSynExchangeCode = lSynExchangeID?spSyntExchange->Code:L"";
																IMmQvQuoteAtomPtr spNewQuoteAtom = pSynthUnd->m_pQuote->GetQuote(lSynExchangeID);
																if(spNewQuoteAtom == NULL)
																   spNewQuoteAtom = pSynthUnd->m_pQuote->AddNew(lSynExchangeID, bsSynExchangeCode, &_pAtom);
																else
																	_pAtom = dynamic_cast<CComObject<CMmQvQuoteAtom>*>(spNewQuoteAtom.GetInterfacePtr());
																if(_pAtom!=NULL)
																{
																	_pAtom->m_spExch   = spSyntExchange;
																	_pAtom->m_nLotSize = pOptRootAtom->m_nLotSize;
																	_pAtom->m_dPriceAsk = BAD_DOUBLE_VALUE;
																	_pAtom->m_dPriceBid = BAD_DOUBLE_VALUE;
																	_pAtom->m_dPriceLast = BAD_DOUBLE_VALUE;


																	_bstr_t sQuoteKey = pSynthUnd->m_bstrSymbol;
																	if (lSynExchangeID)
																		sQuoteKey += _bstr_t(L".")+bsSynExchangeCode;
																
																	IMmQvQuoteAtomPtr spAllQuote = m_pQuoteRequestAll->GetQuote(sQuoteKey);
																	if(spAllQuote==NULL)
																	{
																		 CComObject<CMmQvRequestAtom>* __pAtom = NULL;
																		m_pQuoteRequestAll->AddNew(sQuoteKey, &__pAtom);
																		if(__pAtom)
																		{
																			__pAtom->m_spExch = spSyntExchange;
																			__pAtom->m_spUnd  = spSynthUndAtom;

																		}
																	}
																}

															}
														}
													}
													{
														IMmQvUndAtomPtr _tmp;
														pUndAtom->m_spSynthUnd->Add(spSynthAtom->GetUndID(), pSynthUnd->m_bstrSymbol, spSynthUndAtom, &_tmp);
													}
												}
											}
										}
									}

								}
							}
						}
						else
							pOptRootAtom->m_bstrDPC = _bstr_t(pOptRootAtom->m_nLotSize)+ _bstr_t(L" ") + pUndAtom->m_bstrSymbol;
					}
					else
						pOptRootAtom = dynamic_cast<CComObject<CMmQvOptRootAtom>*>((IMmQvOptRootAtom*)spOptRoot);

					
					IMmQvExpAtomPtr aExpAll = m_pGrp->m_pExpAll->GetExpiry(dtExpiryMonth);
					if(aExpAll == NULL)
					{
						CComObject<CMmQvExpAtom>* pExpAtom = NULL ;
						aExpAll = m_pGrp->m_pExpAll->AddNew((DATE)static_cast<long>(dtExpiryMonth), &pExpAtom);
						if(pExpAtom)
						{
							pExpAtom->m_dtExpiry = dtExpiry;
							pExpAtom->m_dtExpiryOV = dtExpiryOV;
							pExpAtom->m_dtTradingClose = dtTradingClose;
							if (/*!m_bInitializeUndOpt &&*/ m_spVisibleExp->Item[_bstr_t(static_cast<long>(pExpAtom->m_dtExpiry))] == NULL)
								pExpAtom->m_bVisible = VARIANT_FALSE;
						}
					}
					pExpAtom->AddRoot(pOptRootAtom->m_bstrName);
					

					double dStrike = EgNumeric::EgRound<double>(rsOpt[L"fStrike"], STRIKE_LESS_SIGNIFICANT_VALUE);
					
					IMmQvStrikeAtomPtr aStr = pExpAtom->m_pStrike->GetStrike(dStrike);
					CComObject<CMmQvStrikeAtom>* pStrikeAtom = NULL;

					if(aStr == NULL)
						 aStr = pExpAtom->m_pStrike->AddNew(dStrike, &pStrikeAtom);
					else
						pStrikeAtom = dynamic_cast<CComObject<CMmQvStrikeAtom>*>((IMmQvStrikeAtom*)aStr);

					IMmQvStrikeAtomPtr  aStrAll;
					if(m_pGrp && m_pGrp->m_pStrikeAll)
						aStrAll = m_pGrp->m_pStrikeAll->GetStrike(dStrike);

					if(aStrAll == NULL)
					{
						IMmQvStrikeAtomPtr sTmp;
						m_pGrp->m_pStrikeAll->Add(dStrike, aStr, &sTmp);
					}

					CComObject<CMmQvOptPairAtom>* pPairAtom = NULL;
					IMmQvOptPairAtomPtr spOptPair = pStrikeAtom->m_pOptPair->GetPair(pOptRootAtom->m_nID);
					if(spOptPair == NULL)
						spOptPair = pStrikeAtom->m_pOptPair->AddNew(pOptRootAtom->m_nID, pOptRootAtom->m_bstrName, &pPairAtom);
					else
						pPairAtom = dynamic_cast<CComObject<CMmQvOptPairAtom>*>((IMmQvOptPairAtom*)spOptPair);

					EtsOptionTypeEnum enOptType = (long)rsOpt[L"tiIsCall"] != 0L? enOtCall: enOtPut;

					IMmQvOptAtomPtr spOptionAtom = pPairAtom->GetOpt(enOptType);
					CComObject<CMmQvOptAtom>* pOptionAtom = dynamic_cast<CComObject<CMmQvOptAtom>*>(spOptionAtom.GetInterfacePtr());
					if(pOptionAtom && !pOptionAtom->m_nID)
					{
						pOptionAtom->m_nID        = rsOpt[L"iContractID"];
						pOptionAtom->m_bstrSymbol = rsOpt[L"vcSymbol"];
						pOptionAtom->m_nIsFlex	  = rsOpt[L"iIsFlex"];
						pOptionAtom->m_enOptType  = enOptType;
						pOptionAtom->m_dtExpiry   = dtExpiry;
						pOptionAtom->m_dtExpiryOV = dtExpiryOV;
						pOptionAtom->m_dtTradingClose = dtTradingClose;
						pOptionAtom->m_dStrike    = dStrike;
						pOptionAtom->m_nRootID    =	pOptRootAtom->m_nID;
						
						if(pPairAtom->IsSetVolaCache())
							pOptionAtom->m_dVola = pPairAtom->GetVola();
						else
						{
							LONG lSurfaceID = pUndAtom->m_spVolaSrv->GetSurfaceByRoot(pOptionAtom->m_nRootID);
							pOptionAtom->m_dVola  = pUndAtom->m_spVolaSrv->GetOptionVola(dtExpiryOV, dStrike, lSurfaceID);
							if(pOptionAtom->m_dVola<0.)
								pOptionAtom->m_dVola = BAD_DOUBLE_VALUE;
							pPairAtom->SetVola(pOptionAtom->m_dVola);
						}

						IMmQvOptAtomPtr spOptAtom;
						pUndAtom->m_pOpt->Add(pOptionAtom->m_nID, pOptionAtom,  &spOptAtom);
					}


					IUnknownPtr spUnk;
					_variant_t varItem;
					ULONG nFetched = 0L;

					if(CachedExchanges.empty())
					{
						_CHK(pUndAtom->m_spOptExch->get__NewEnum(&spUnk), _T("Fail to get Exchanges collection enum."));
						IEnumVARIANTPtr spQuotesCompEnum(spUnk);

						_CHK(spQuotesCompEnum->Reset(), _T("Fail to reset  Exchanges collection."));
						while((hr = spQuotesCompEnum->Next(1L, &varItem, &nFetched)) == S_OK)
						{
							ATLASSERT(varItem.vt == VT_DISPATCH);
							if(varItem.vt == VT_DISPATCH)
							{
								IExchAtomPtr spExchAtom = varItem;
								AddQuoteExchange(spExchAtom, dtExpiry, spExpiry, pOptionAtom, pOptRootAtom);
								CachedExchanges[spExchAtom->ID] = spExchAtom;
							}
						}
					}
					else
					{
						for(CExchanges::iterator itr = CachedExchanges.begin();
																   itr != CachedExchanges.end();
																   ++itr)
						{
							AddQuoteExchange(itr->second, dtExpiry, spExpiry, pOptionAtom, pOptRootAtom);
						}



					}
					long lCurrent = static_cast<long>((double)(20. * double(lLoaded)/lRecordCount));
					if(lCurrentProgress != lCurrent)
					{
						lCurrentProgress = lCurrent;
						OnProgress(lCurrentProgress, 20);
					}
				}
				m_nOptQuotesCount = static_cast<long>(pUndAtom->m_pOpt->GetItemsCount()) * pUndAtom->m_spOptExch->Count;
			}
			else
				m_nOptQuotesCount = 0L;

		}
	}
	catch (_com_error& e)
	{
		hr = Error((PTCHAR)CComErrorWrapper::ErrorDescription(e), IID_IEtsMmQuotesView, e.Error());
	}
	/*catch (...)
	{
		hr = Error((PTCHAR)_T("Unhandled Error"), IID_IEtsMmQuotesView, E_FAIL);
	}*/

	return hr;
}

HRESULT CEtsMmQuotesView::AddExch(IExchCollPtr aExchColl, long lID, _bstr_t sCode, _bstr_t sName)
{
	HRESULT hr = S_OK;
	try
	{
		IExchAtomPtr spExchAtom = aExchColl->Add(lID, sCode, NULL);
		if(spExchAtom!=NULL)
			spExchAtom->Name = sName;
	}
	catch (_com_error& e)
	{
		hr = Error((PTCHAR)CComErrorWrapper::ErrorDescription(e), IID_IEtsMmQuotesView, e.Error());
	}
	return hr;
}

void CEtsMmQuotesView::AddQuoteExchange(IExchAtomPtr spExchAtom, 
										vt_date dtExpiry,
										IMmQvExpAtomPtr spExpiry,
										CComObject<CMmQvOptAtom>* pOptionAtom,
										CComObject<CMmQvOptRootAtom>* pOptRootAtom)
{
	WCHAR buffer[1024] = {0};

	if (spExchAtom!=NULL)
	{
		long    lExchangeID    = spExchAtom->ID;
		_bstr_t bsExchangeCode = spExchAtom->Code;

		CComObject<CMmQvQuoteAtom>* pQuoteAtom = NULL;
		IMmQvQuoteAtomPtr spQuote = pOptionAtom->m_pQuote->AddNew(lExchangeID, bsExchangeCode, &pQuoteAtom);
		if(pQuoteAtom)
		{
			pQuoteAtom->m_spExch   = spExchAtom;
			pQuoteAtom->m_nLotSize = pOptRootAtom->m_nLotSize;

			if(pOptionAtom->m_dStrike == (double)static_cast<long>(pOptionAtom->m_dStrike))
				_snwprintf_s(buffer, sizeof(buffer)/sizeof(WCHAR),L"%02d%s%d",
				dtExpiry.get_year()-2000, bsMonth[dtExpiry.get_month()-1], static_cast<long>(pOptionAtom->m_dStrike));
			else
				_snwprintf_s(buffer, sizeof(buffer)/sizeof(WCHAR),L"%02d%s%.02f",
				dtExpiry.get_year()-2000, bsMonth[dtExpiry.get_month()-1], pOptionAtom->m_dStrike);
			pQuoteAtom->m_bstrSeries = buffer;

			_bstr_t bsSymbolKey = pOptionAtom->m_bstrSymbol;
			if (!lExchangeID)
				pOptionAtom->SetDefaultQuote(spQuote); 
			else
			{
				pQuoteAtom->m_bstrSeries +=  _bstr_t(L" ")+ bsExchangeCode;
				bsSymbolKey				 +=  _bstr_t(L".")+ bsExchangeCode;
			}

			if(pOptionAtom->m_nIsFlex == 0)
			{
				IMmQvRequestAtomPtr spQuote = m_pQuoteRequestAll->GetQuote(bsSymbolKey);
				if(spQuote == NULL)
				{
					CComObject<CMmQvRequestAtom>* pQuote = NULL;

					spQuote = m_pQuoteRequestAll->AddNew(bsSymbolKey, &pQuote);
					if(pQuote)
					{
						pQuote->m_spExch = spExchAtom;
						pQuote->m_spOpt  = pOptionAtom;
						pQuote->m_spExp  = spExpiry;
					}
				}
			}
		}									
	}

}

HRESULT CEtsMmQuotesView::LoadQuoteUpdates()
{
	HRESULT hr   = S_OK;
	try
	{
		m_nUpdatedOptCount = 0L;
		m_nUpdatedFutCount = 0L;
		m_nUpdatedUndCount = 0L;

		SAFEARRAY* pSafeArray =	m_spBatchPriceProvider->GetQuotesUpdates();
		if(pSafeArray)
		{
			QuoteUpdateFullInfo* pvData = NULL;
			if( SUCCEEDED( hr = ::SafeArrayAccessData( pSafeArray, (void**)&pvData) ) )
			{
				long lLBound = 0;
				long lUBound = 0;
				HRESULT hRes = SafeArrayGetLBound(pSafeArray, 1L, &lLBound);
				hRes = SafeArrayGetUBound(pSafeArray, 1L, &lUBound);
				long lArraySize = lUBound - lLBound ;

				for(int i = 0; i<= lArraySize; ++i)
					AcceptQuote(pvData[i].Params, pvData[i].Info);
				//TODO: change this
				m_bMarketDataChanged = VARIANT_TRUE;

				::SafeArrayUnaccessData(pSafeArray);
			}
			::SafeArrayDestroy(pSafeArray);
		}
	}
	catch (_com_error& e)
	{
		hr = Error((PTCHAR)CComErrorWrapper::ErrorDescription(e), IID_IEtsMmQuotesView, e.Error());
	}
	/*catch(...)
	{
		hr =  Error(_T("Unknown Error"), IID_IEtsMmQuotesView, E_FAIL);
	}*/
	return hr;
}

STDMETHODIMP CEtsMmQuotesView::put_Quote(QuoteUpdateParams* Params, QuoteUpdateInfo* Info)
{
	HRESULT hr = S_OK;
	if(!Params || !Info)
		return E_POINTER;
	try
	{
		hr = AcceptQuote(*Params, *Info);
	}
	catch(_com_error& e)
	{
		hr = Error((PTCHAR)CComErrorWrapper::ErrorDescription(e), IID_IEtsMmQuotesView, e.Error());
	}
	/*catch(...)
	{
		hr =  Error(_T("Unknown Error"), IID_IEtsMmQuotesView, E_FAIL);
	}*/

	return hr;
}

HRESULT CEtsMmQuotesView::AcceptQuote(const QuoteUpdateParams& Params, const QuoteUpdateInfo& Info)
{
	_bstr_t bsParamsSymbol(Params.Symbol);
	_bstr_t bsParamsExchange(Params.Exchange);
	_bstr_t bsQuoteKey = bsParamsSymbol;

	if(bsParamsExchange.length())
		bsQuoteKey += _bstr_t(L".") + bsParamsExchange;
	IMmQvRequestAtomPtr spRequest = m_pQuoteRequestAll->GetQuote(bsQuoteKey);
	CComObject<CMmQvRequestAtom>* pRequest = dynamic_cast<CComObject<CMmQvRequestAtom>*>((IMmQvRequestAtom*)spRequest);
	if(pRequest)
	{
		switch(Params.Type) {
		case enOPT:
		case enFOP:
			{
				IMmQvOptAtomPtr spOptAtom = pRequest->m_spOpt;
				CComObject<CMmQvOptAtom>* pOptAtom = dynamic_cast<CComObject<CMmQvOptAtom>*>((IMmQvOptAtom*)spOptAtom);

				IMmQvQuoteAtomPtr spExchQuote = pOptAtom->m_pQuote->GetQuote(pRequest->m_spExch->ID);
				CComObject<CMmQvQuoteAtom>* pExchQuote = dynamic_cast<CComObject<CMmQvQuoteAtom>*>((IMmQvQuoteAtom*)spExchQuote);
				if(pExchQuote)
				{
					if(Info.HighPrice > BAD_DOUBLE_VALUE && Info.HighPrice > 0. )
						pExchQuote->m_dPriceHigh = Info.HighPrice;

					if(Info.LowPrice > BAD_DOUBLE_VALUE && Info.LowPrice > 0. )
						pExchQuote->m_dPriceLow = Info.LowPrice;

					if(Info.OpenPrice > BAD_DOUBLE_VALUE && Info.OpenPrice > 0. )
						pExchQuote->m_dPriceOpen = Info.OpenPrice;

					if(Info.Volume > BAD_LONG_VALUE )
						pExchQuote->m_nVolume = Info.Volume;

					if(Info.NetChange > BAD_DOUBLE_VALUE )
						pExchQuote->m_dNetChange = Info.NetChange;

					if(Info.BidSize > BAD_LONG_VALUE )
						pExchQuote->m_nSizeBid = Info.BidSize;
					if(Info.AskSize > BAD_LONG_VALUE )
						pExchQuote->m_nSizeAsk = Info.AskSize;

					pExchQuote->m_dtUpdateTime = vt_date::GetCurrentDate();
					if(Info.OpenInterest > BAD_DOUBLE_VALUE)
						pExchQuote->m_nOpenInterest = static_cast<long>(Info.OpenInterest);

					if(Info.BidPrice > BAD_DOUBLE_VALUE && Info.BidPrice > 0. && pExchQuote->m_dPriceBid > BAD_DOUBLE_VALUE)
						pExchQuote->m_dPriceBidTick = Info.BidPrice - pExchQuote->m_dPriceBid;
					if(Info.AskPrice > BAD_DOUBLE_VALUE && Info.AskPrice > 0. &&  pExchQuote->m_dPriceAsk > BAD_DOUBLE_VALUE)
						pExchQuote->m_dPriceAskTick = Info.AskPrice - pExchQuote->m_dPriceAsk;

					if( (Info.BidPrice > BAD_DOUBLE_VALUE && Info.BidPrice   != pExchQuote->m_dPriceBid)  ||
						(Info.AskPrice > BAD_DOUBLE_VALUE && Info.AskPrice   != pExchQuote->m_dPriceAsk)  ||
						(Info.LastPrice > BAD_DOUBLE_VALUE && Info.LastPrice != pExchQuote->m_dPriceLast)  )
					{
						if(Info.BidPrice >  BAD_DOUBLE_VALUE) pExchQuote->m_dPriceBid  = Info.BidPrice;
						if(Info.AskPrice >  BAD_DOUBLE_VALUE) pExchQuote->m_dPriceAsk  = Info.AskPrice;
						if(Info.LastPrice > BAD_DOUBLE_VALUE) pExchQuote->m_dPriceLast = Info.LastPrice;
						pExchQuote->m_sAskExchange = Info.AskExchange;
						pExchQuote->m_sBidExchange = Info.BidExchange;
						pExchQuote->m_enPriceUpdateStatus = enMmQvPusChanged;

						InterlockedIncrement(&m_nUpdatedOptCount);
					}
					else
						pExchQuote->m_enPriceUpdateStatus = enMmQvPusChanged;

					pExchQuote->SetDirty();
				}
			}
			break;
		case enFUT:
			{
				IMmQvFutAtomPtr spFutAtom = pRequest->m_spFut;
				CComObject<CMmQvFutAtom>* pFutAtom = dynamic_cast<CComObject<CMmQvFutAtom>*>((IMmQvFutAtom*)spFutAtom);

				IMmQvQuoteAtomPtr spExchQuote = pFutAtom->m_pQuote->GetQuote(pRequest->m_spExch->ID);
				CComObject<CMmQvQuoteAtom>* pExchQuote = dynamic_cast<CComObject<CMmQvQuoteAtom>*>((IMmQvQuoteAtom*)spExchQuote);
				if(pExchQuote)
				{
					if(Info.HighPrice > BAD_DOUBLE_VALUE && Info.HighPrice > 0. )
						pExchQuote->m_dPriceHigh = Info.HighPrice;

					if(Info.LowPrice > BAD_DOUBLE_VALUE && Info.LowPrice > 0. )
						pExchQuote->m_dPriceLow = Info.LowPrice;

					if(Info.OpenPrice > BAD_DOUBLE_VALUE && Info.OpenPrice > 0. )
						pExchQuote->m_dPriceOpen = Info.OpenPrice;

					if(Info.Volume > BAD_LONG_VALUE )
						pExchQuote->m_nVolume = Info.Volume;

					if(Info.NetChange > BAD_DOUBLE_VALUE )
						pExchQuote->m_dNetChange = Info.NetChange;

					if(Info.BidSize > BAD_LONG_VALUE )
						pExchQuote->m_nSizeBid = Info.BidSize;
					if(Info.AskSize > BAD_LONG_VALUE )
						pExchQuote->m_nSizeAsk = Info.AskSize;

					pExchQuote->m_dtUpdateTime = vt_date::GetCurrentDate();

					if(Info.BidPrice > BAD_DOUBLE_VALUE && Info.BidPrice > 0. && pExchQuote->m_dPriceBid > BAD_DOUBLE_VALUE)
						pExchQuote->m_dPriceBidTick = Info.BidPrice - pExchQuote->m_dPriceBid;
					if(Info.AskPrice > BAD_DOUBLE_VALUE && Info.AskPrice > 0. && pExchQuote->m_dPriceAsk > BAD_DOUBLE_VALUE)
						pExchQuote->m_dPriceAskTick = Info.AskPrice - pExchQuote->m_dPriceAsk;

					if( (Info.BidPrice > BAD_DOUBLE_VALUE && Info.BidPrice   != pExchQuote->m_dPriceBid)  ||
						(Info.AskPrice > BAD_DOUBLE_VALUE && Info.AskPrice   != pExchQuote->m_dPriceAsk)  ||
						(Info.LastPrice > BAD_DOUBLE_VALUE && Info.LastPrice != pExchQuote->m_dPriceLast)  )
					{
						if(Info.BidPrice >  BAD_DOUBLE_VALUE) pExchQuote->m_dPriceBid  = Info.BidPrice;
						if(Info.AskPrice >  BAD_DOUBLE_VALUE) pExchQuote->m_dPriceAsk  = Info.AskPrice;
						if(Info.LastPrice > BAD_DOUBLE_VALUE) pExchQuote->m_dPriceLast = Info.LastPrice;
						pExchQuote->m_sAskExchange = Info.AskExchange;
						pExchQuote->m_sBidExchange = Info.BidExchange;
						pExchQuote->m_enPriceUpdateStatus = enMmQvPusChanged;

						InterlockedIncrement(&m_nUpdatedFutCount);
					}
					else
						pExchQuote->m_enPriceUpdateStatus = enMmQvPusChanged;
					pExchQuote->SetDirty();
				}
			}
			break;
		default:
			{
				IMmQvUndAtomPtr spUndAtom = pRequest->m_spUnd;
				CComObject<CMmQvUndAtom>* pUndAtom = dynamic_cast<CComObject<CMmQvUndAtom>*>((IMmQvUndAtom*)spUndAtom);

				IMmQvQuoteAtomPtr spExchQuote = pUndAtom->m_pQuote->GetQuote(pRequest->m_spExch->ID);
				CComObject<CMmQvQuoteAtom>* pExchQuote = dynamic_cast<CComObject<CMmQvQuoteAtom>*>((IMmQvQuoteAtom*)spExchQuote);

				if(pExchQuote)
				{
					if(Info.HighPrice > BAD_DOUBLE_VALUE && Info.HighPrice > 0. )
						pExchQuote->m_dPriceHigh = Info.HighPrice;

					if(Info.LowPrice > BAD_DOUBLE_VALUE && Info.LowPrice > 0. )
						pExchQuote->m_dPriceLow = Info.LowPrice;

					if(Info.OpenPrice > BAD_DOUBLE_VALUE && Info.OpenPrice > 0. )
						pExchQuote->m_dPriceOpen = Info.OpenPrice;

					if(Info.Volume > BAD_LONG_VALUE )
						pExchQuote->m_nVolume = Info.Volume;

					if(Info.NetChange > BAD_DOUBLE_VALUE )
						pExchQuote->m_dNetChange = Info.NetChange;

					if(Info.BidSize > BAD_LONG_VALUE )
						pExchQuote->m_nSizeBid = Info.BidSize;
					if(Info.AskSize > BAD_LONG_VALUE )
						pExchQuote->m_nSizeAsk = Info.AskSize;

					pExchQuote->m_dtUpdateTime = vt_date::GetCurrentDate();

					if(Info.BidPrice > BAD_DOUBLE_VALUE && Info.BidPrice > 0. && pExchQuote->m_dPriceBid > BAD_DOUBLE_VALUE)
						pExchQuote->m_dPriceBidTick = Info.BidPrice - pExchQuote->m_dPriceBid;
					if(Info.AskPrice > BAD_DOUBLE_VALUE && Info.AskPrice > 0. &&  pExchQuote->m_dPriceAsk > BAD_DOUBLE_VALUE)
						pExchQuote->m_dPriceAskTick = Info.AskPrice - pExchQuote->m_dPriceAsk;

					if( (Info.BidPrice > BAD_DOUBLE_VALUE && Info.BidPrice   != pExchQuote->m_dPriceBid)  ||
						(Info.AskPrice > BAD_DOUBLE_VALUE && Info.AskPrice   != pExchQuote->m_dPriceAsk)  ||
						(Info.LastPrice > BAD_DOUBLE_VALUE && Info.LastPrice != pExchQuote->m_dPriceLast)  )
					{
						if(Info.BidPrice >  BAD_DOUBLE_VALUE) pExchQuote->m_dPriceBid  = Info.BidPrice;
						if(Info.AskPrice >  BAD_DOUBLE_VALUE) pExchQuote->m_dPriceAsk  = Info.AskPrice;
						if(Info.LastPrice > BAD_DOUBLE_VALUE) pExchQuote->m_dPriceLast = Info.LastPrice;
						pExchQuote->m_sAskExchange = Info.AskExchange;
						pExchQuote->m_sBidExchange = Info.BidExchange;
						pExchQuote->m_enPriceUpdateStatus = enMmQvPusChanged;
						if(pExchQuote->GetExchangeID() == 0L && pUndAtom->m_nID == m_pGrp->m_pUnd->m_nID )
						{
							pUndAtom->m_spVolaSrv->UnderlyingPrice = PriceMidEx(pExchQuote->m_dPriceBid, pExchQuote->m_dPriceAsk, pExchQuote->m_dPriceLast);
						}
						InterlockedIncrement(&m_nUpdatedUndCount);
					}
					else
						pExchQuote->m_enPriceUpdateStatus = enMmQvPusChanged;
					pExchQuote->SetDirty();
				}
			}
		}
	}
	return S_OK;
}

double CEtsMmQuotesView::PriceMidEx(double dPriceBid, double dPriceAsk, double dPriceLast, bool bUseLast) 
{
	if(dPriceBid >0. && dPriceAsk >0.)
		return (dPriceBid+dPriceAsk)/2.;

	if (bUseLast && dPriceLast >0.)
			return dPriceLast;

	if(dPriceBid >0. || dPriceAsk >0.)
	{
		if(dPriceBid >0. && dPriceAsk <=0.)
		{
			if(bUseLast)
				return dPriceBid;
			else
				return dPriceBid/2.;
		}
		else
		{
			if(bUseLast)
				return dPriceAsk;
			else
				return dPriceAsk/2.;
		}
	}
	return BAD_DOUBLE_VALUE;
}


STDMETHODIMP CEtsMmQuotesView::StartRealtime()
{
	return m_pQuoteRequestAll->StartRealtime(m_spBatchPriceProvider);
}

STDMETHODIMP CEtsMmQuotesView::RequestLastQuotes()
{
	return m_pQuoteRequestAll->RequestLastQuotes(m_spBatchPriceProvider);
}

STDMETHODIMP CEtsMmQuotesView::put_OptsColumnsOrder(SAFEARRAY** colOrder)
{
	//try
	//{
		void* pData = reinterpret_cast<LPVOID> (*colOrder);
		if(pData!=NULL)
		{
			m_lVisibleColumns = 0L;
			CComSafeArray<LONG> lArray(*colOrder);
			long lUpperBound = lArray.GetUpperBound();
			long lLowerBound = lArray.GetLowerBound();
			int j=0;
			memset(m_ColumnsEncoding,-1,sizeof(m_ColumnsEncoding));
			for(long i = lLowerBound; i<lUpperBound; ++i)
			{
				m_ColumnsEncoding[j++] = lArray[i];
				if(lArray[i]>=0)
					m_lVisibleColumns++;

				if(j >= QOC_COLUMN_COUNT)
					break;
			}
		}
	//}
	//catch (...) {}
	return S_OK;

}

HRESULT CEtsMmQuotesView::InitVisibleOptExchanges(bool bInitialize, CExchanges& vecVisibleExchanges, bool& bAllExchanges)
{
	HRESULT hr = S_OK;
	try
	{
		bool bShowOnlyDefExch = false;
 		     bAllExchanges    = true;

		vecVisibleExchanges.clear();
		_variant_t varItem;
		ULONG nFetched = 0L;
		if(bInitialize)
		{
			if(m_spVisibleOptExch->Count)
			{
				IEnumVARIANTPtr spSOptExchEnum(m_pGrp->m_pUnd->m_spOptExch->_NewEnum);
				_CHK(spSOptExchEnum->Reset(), _T("Fail to reset  Options Exchanges collection."));
				while((hr = spSOptExchEnum->Next(1L, &varItem, &nFetched)) == S_OK)
				{
					ATLASSERT(varItem.vt == VT_DISPATCH);
					if(varItem.vt == VT_DISPATCH)
					{
						IExchAtomPtr spAtom = varItem;
						long lExchID = spAtom->ID;
						IEtsMmFilterAtomPtr spFilter =  m_spVisibleOptExch->Item[_bstr_t(lExchID)];
						if(spFilter == NULL)
						{
							bAllExchanges = false;
							break;
						}
					}
				}
				bShowOnlyDefExch = !bAllExchanges;
			}

			IEnumVARIANTPtr spSOptExchEnum = m_pGrp->m_spOptExchAll->_NewEnum;

			_CHK(spSOptExchEnum->Reset(), _T("Fail to reset  Options Exchanges collection."));
			while((hr = spSOptExchEnum->Next(1L, &varItem, &nFetched)) == S_OK)
			{
				ATLASSERT(varItem.vt == VT_DISPATCH);
				if(varItem.vt == VT_DISPATCH)
				{
					IExchAtomPtr spAtom = varItem;
					long lExchID = spAtom->ID;
					VARIANT_BOOL vbVisible = VARIANT_FALSE; 

					if(!bAllExchanges)
						vbVisible = (m_spVisibleOptExch->Item[_bstr_t(lExchID)] != NULL?VARIANT_TRUE:VARIANT_FALSE);
					else
						vbVisible = ((!bShowOnlyDefExch || (bShowOnlyDefExch && !lExchID ))?VARIANT_TRUE:VARIANT_FALSE);
					if(vbVisible)
						vecVisibleExchanges[lExchID] = spAtom;
					spAtom->Visible = vbVisible;
				}
			}
		}
		else
		{
			IEnumVARIANTPtr spSOptExchEnum;

			VARIANT_BOOL bIsStockOrIndex = VARIANT_FALSE;
			m_pGrp->get_IsStockOrIndex(&bIsStockOrIndex);

			if(bIsStockOrIndex)
				spSOptExchEnum = m_pGrp->m_pUnd->m_spOptExch->_NewEnum;
			else
				spSOptExchEnum = m_pGrp->m_pFut->m_spOptExch->_NewEnum;

			_CHK(spSOptExchEnum->Reset(), _T("Fail to reset  Options Exchanges collection."));
			while((hr = spSOptExchEnum->Next(1L, &varItem, &nFetched)) == S_OK)
			{
				ATLASSERT(varItem.vt == VT_DISPATCH);
				if(varItem.vt == VT_DISPATCH)
				{
					IExchAtomPtr spAtom = varItem;
					long lExchID = spAtom->ID;
					if(spAtom->Visible)
						vecVisibleExchanges[lExchID] = spAtom;
				}
			}
		}
	}
	catch (_com_error& err) 
	{
		hr = Error((PTCHAR)CComErrorWrapper::ErrorDescription(err), IID_IEtsMmQuotesView, err.Error());
	}
	/*catch(...)
	{
		HRESULT _hr = HRESULT_FROM_WIN32(::GetLastError());
		if(SUCCEEDED(_hr)) _hr = E_FAIL;
		hr = Error((PTCHAR)"System Error", IID_IEtsMmQuotesView, _hr);
	}*/

	return S_OK;
}

HRESULT CEtsMmQuotesView::InitVisibleFutOptExchanges(bool bInitialize, CExchanges& vecVisibleExchanges, bool& bAllExchanges)
{
	HRESULT hr = S_OK;
	try
	{
		bool bShowOnlyDefExch = false;
		bAllExchanges    = true;

		vecVisibleExchanges.clear();
		_variant_t varItem;
		ULONG nFetched = 0L;
		if(bInitialize)
		{
			if(m_spVisibleOptExch->Count)
			{
				IEnumVARIANTPtr spSOptExchEnum(m_pGrp->m_pFut->m_spOptExch->_NewEnum);
				_CHK(spSOptExchEnum->Reset(), _T("Fail to reset Future Options Exchanges collection."));
				while((hr = spSOptExchEnum->Next(1L, &varItem, &nFetched)) == S_OK)
				{
					ATLASSERT(varItem.vt == VT_DISPATCH);
					if(varItem.vt == VT_DISPATCH)
					{
						IExchAtomPtr spAtom = varItem;
						long lExchID = spAtom->ID;
						IEtsMmFilterAtomPtr spFilter =  m_spVisibleOptExch->Item[_bstr_t(lExchID)];
						if(spFilter == NULL)
						{
							bAllExchanges = false;
							break;
						}
					}
				}
				bShowOnlyDefExch = bAllExchanges;
			}

			IEnumVARIANTPtr spSOptExchEnum = m_pGrp->m_spOptExchAll->_NewEnum;

			_CHK(spSOptExchEnum->Reset(), _T("Fail to reset  Options Exchanges collection."));
			while((hr = spSOptExchEnum->Next(1L, &varItem, &nFetched)) == S_OK)
			{
				ATLASSERT(varItem.vt == VT_DISPATCH);
				if(varItem.vt == VT_DISPATCH)
				{
					IExchAtomPtr spAtom = varItem;
					long lExchID = spAtom->ID;
					VARIANT_BOOL vbVisible = VARIANT_FALSE; 

					if(!bAllExchanges)
						vbVisible = (m_spVisibleOptExch->Item[_bstr_t(lExchID)] != NULL?VARIANT_TRUE:VARIANT_FALSE);
					else
						vbVisible = ((!bShowOnlyDefExch || (bShowOnlyDefExch && !lExchID ))?VARIANT_TRUE:VARIANT_FALSE);
					if(vbVisible)
						vecVisibleExchanges[lExchID] = spAtom;
					spAtom->Visible = vbVisible;
				}
			}
		}
		else
		{
			IEnumVARIANTPtr spSOptExchEnum;

			VARIANT_BOOL bIsStockOrIndex = VARIANT_FALSE;
			m_pGrp->get_IsStockOrIndex(&bIsStockOrIndex);

			if(bIsStockOrIndex)
				spSOptExchEnum = m_pGrp->m_pUnd->m_spOptExch->_NewEnum;
			else
				spSOptExchEnum = m_pGrp->m_pFut->m_spOptExch->_NewEnum;

			_CHK(spSOptExchEnum->Reset(), _T("Fail to reset  Options Exchanges collection."));
			while((hr = spSOptExchEnum->Next(1L, &varItem, &nFetched)) == S_OK)
			{
				ATLASSERT(varItem.vt == VT_DISPATCH);
				if(varItem.vt == VT_DISPATCH)
				{
					IExchAtomPtr spAtom = varItem;
					long lExchID = spAtom->ID;
					if(spAtom->Visible)
						vecVisibleExchanges[lExchID] = spAtom;
				}
			}
		}
	}
	catch (_com_error& err) 
	{
		hr = Error((PTCHAR)CComErrorWrapper::ErrorDescription(err), IID_IEtsMmQuotesView, err.Error());
	}
	/*catch(...)
	{
		HRESULT _hr = HRESULT_FROM_WIN32(::GetLastError());
		if(SUCCEEDED(_hr)) _hr = E_FAIL;
		hr = Error((PTCHAR)"System Error", IID_IEtsMmQuotesView, _hr);
	}*/

	return S_OK;
}

HRESULT CEtsMmQuotesView::RefreshStockOptions(bool bInitialize)
{
	HRESULT hr = S_OK;
	try
	{
		CExchanges vecVisibleExchanges;


		bool bAllExchanges = true;
		bool bAllExpirations = !m_spVisibleExp->Count;
		bool bAllRoots = !m_spVisibleRoot->Count;
		bool bAllStrikes = !m_spVisibleStr->Count;

		_CHK(InitVisibleOptExchanges(bInitialize, vecVisibleExchanges, bAllExchanges), "Fail to init Visible Exchanges");

		CMmQvExpColl::IterType expItBegin = m_pGrp->m_pUnd->m_pExp->m_coll.begin();
		CMmQvExpColl::IterType expItEnd   = m_pGrp->m_pUnd->m_pExp->m_coll.end();
		for(CMmQvExpColl::IterType expIt = expItBegin; expIt!=expItEnd; ++expIt)
		{
			IMmQvExpAtomPtr				spExpiry = expIt->second;
			CComObject<CMmQvExpAtom>*	pExpiry = dynamic_cast<CComObject<CMmQvExpAtom>*>(spExpiry.GetInterfacePtr());

			IEtsMmFilterAtomPtr spFilterAtom;
			if(!bAllExpirations && bInitialize)
				spFilterAtom = m_spVisibleExp->Item[_bstr_t(static_cast<long>(pExpiry->m_dtExpiryMonth))];
			if(!bInitialize ||  bAllExpirations  || spFilterAtom != NULL)
			{
				if(bInitialize)
					pExpiry->m_bVisible = VARIANT_TRUE;
				else
					if(pExpiry->m_bVisible == VARIANT_FALSE)
						continue;

				CMmQvStrikeColl::IterType strikeItBegin = pExpiry->m_pStrike->m_coll.begin();
				CMmQvStrikeColl::IterType strikeItEnd	= pExpiry->m_pStrike->m_coll.end();
				for (CMmQvStrikeColl::IterType strikeIt = strikeItBegin; strikeIt != strikeItEnd; ++strikeIt) 
				{
					IMmQvStrikeAtomPtr				spStrike = strikeIt->second;
					CComObject<CMmQvStrikeAtom>*	pStrike  = dynamic_cast<CComObject<CMmQvStrikeAtom>*>(spStrike.GetInterfacePtr());
					IEtsMmEntityAtomPtr spEntity;
					if(!bAllStrikes && bInitialize)
						spEntity = m_spVisibleStr->Item[_bstr_t(pStrike->m_dStrike)];

					if(!bInitialize || bAllStrikes || spEntity != NULL)
					{
						if(bInitialize)
							pStrike->m_bVisible = VARIANT_TRUE;
						else
							if(pStrike->m_bVisible==VARIANT_FALSE)
								continue;

						m_pGrp->m_pStrikeAll->GetStrike(pStrike->m_dStrike)->put_Visible(VARIANT_TRUE);

						CMmQvOptPairColl::EnumCollType::iterator opItBegin = pStrike->m_pOptPair->m_coll.begin();
						CMmQvOptPairColl::EnumCollType::iterator opItEnd   = pStrike->m_pOptPair->m_coll.end();
						for(CMmQvOptPairColl::EnumCollType::iterator opIt = opItBegin; opIt!=opItEnd; ++opIt)
						{
							IMmQvOptPairAtomPtr spPairAtom = opIt->second;
							CComObject<CMmQvOptPairAtom>* pPairAtom = dynamic_cast<CComObject<CMmQvOptPairAtom>*>(spPairAtom.GetInterfacePtr());
							IMmQvOptRootAtomPtr spOptRoot = m_pGrp->m_pUnd->m_pOptRoot->GetItem(pPairAtom->m_nRootID);

							if(spOptRoot!=NULL)
							{
								CComObject<CMmQvOptRootAtom>* pOptRoot = dynamic_cast<CComObject<CMmQvOptRootAtom>*>(spOptRoot.GetInterfacePtr());
								// TODO:
								//if(bInitialize && !bAllRoots )
								//	spFilterAtom = m_spVisibleRoot->Item[pOptRoot->m_bstrName];
								//if(!bInitialize || bAllRoots || spFilterAtom != NULL )
								{
									//if(bInitialize)
									//	pOptRoot->m_bVisible = VARIANT_TRUE;
									//else
										if(pOptRoot->m_bVisible==VARIANT_FALSE)
											continue;

									for(CExchanges::iterator itrExch = vecVisibleExchanges.begin(); itrExch != vecVisibleExchanges.end(); ++itrExch)
									{
										IExchAtomPtr spExchange = m_pGrp->m_pUnd->m_spOptExch->Item[itrExch->first];
										if(spExchange!=NULL)
										{
											CComObject<CMmQvRowData>* pRowData = NULL;
											CComObject<CMmQvRowData>::CreateInstance(&pRowData);
											if(pRowData)
											{
												IMmQvRowDataPtr spRowData;
												spRowData.Attach(pRowData, TRUE);

												pRowData->m_spExp		= spExpiry;
												pRowData->m_spExch		= spExchange;
												pRowData->m_spOptRoot	= spOptRoot;	 
												pRowData->m_spStrike    = spStrike;

												pRowData->m_pExp		= pExpiry;
												pRowData->m_pOptRoot	= pOptRoot;	 
												pRowData->m_pStrike     = pStrike;
												pRowData->m_pGrp        = m_pGrp;

												if(pPairAtom->m_pOpt[enOtCall]->m_nID)
												{
													pRowData->m_spOpt[enOtCall] = pPairAtom->m_spOpt[enOtCall];
													pRowData->m_spOptQuote[enOtCall] = pPairAtom->m_pOpt[enOtCall]->m_pQuote->GetQuote(itrExch->first);

													//pRowData->m_pOpt[enOtCall] = pPairAtom->m_pOpt[enOtCall];
													pRowData->m_pOpt[enOtCall] = dynamic_cast<CComObject<CMmQvOptAtom>*>(pRowData->m_spOpt[enOtCall].GetInterfacePtr());
													pRowData->m_pOptQuote[enOtCall] = dynamic_cast<CComObject<CMmQvQuoteAtom>*>(pRowData->m_spOptQuote[enOtCall].GetInterfacePtr());
												}

												if(pPairAtom->m_pOpt[enOtPut]->m_nID)
												{
													pRowData->m_spOpt[enOtPut] = pPairAtom->m_spOpt[enOtPut];
													pRowData->m_spOptQuote[enOtPut] = pPairAtom->m_pOpt[enOtPut]->m_pQuote->GetQuote(itrExch->first);

													//pRowData->m_pOpt[enOtPut]      = pPairAtom->m_pOpt[enOtPut]!=NULL ? pPairAtom->m_pOpt[enOtPut]:NULL;
													pRowData->m_pOpt[enOtPut] = dynamic_cast<CComObject<CMmQvOptAtom>*>(pRowData->m_spOpt[enOtPut].GetInterfacePtr());
													pRowData->m_pOptQuote[enOtPut] = dynamic_cast<CComObject<CMmQvQuoteAtom>*>(pRowData->m_spOptQuote[enOtPut].GetInterfacePtr());
												}

												//m_Rows.insert(m_Rows.end(), spRowData);
												m_Rows.push_back(spRowData);
											}
										}
									}

								}
								//else
								//	pOptRoot->m_bVisible = VARIANT_FALSE;
							}
						}
					}
					else
					{
						pStrike->m_bVisible = VARIANT_FALSE;
						m_pGrp->m_pStrikeAll->GetStrike(pStrike->m_dStrike)->put_Visible(VARIANT_FALSE);
					}
				}
			}
			else
				pExpiry->m_bVisible = VARIANT_FALSE;
		}
	}
	catch (_com_error& err)
	{
		hr = Error((PTCHAR)CComErrorWrapper::ErrorDescription(err), IID_IEtsMmQuotesView, err.Error());
	}
	/*catch(...)
	{
		HRESULT _hr = HRESULT_FROM_WIN32(::GetLastError());
		if(SUCCEEDED(_hr)) _hr = E_FAIL;
		hr = Error((PTCHAR)"System Error", IID_IEtsMmQuotesView, _hr);
	}*/

	return hr;
}

HRESULT CEtsMmQuotesView::RefreshFuturesOptions(bool bInitialize)
{
	HRESULT hr = S_OK;
	try
	{
		CExchanges vecVisibleExchanges;


		bool bAllExchanges = true;
		bool bAllExpirations = !m_spVisibleExp->Count;
		bool bAllRoots = !m_spVisibleRoot->Count;
		bool bAllStrikes = !m_spVisibleStr->Count;

		_CHK(InitVisibleFutOptExchanges(bInitialize, vecVisibleExchanges, bAllExchanges), "Fail to init Visible Exchanges");


		CMmQvExpColl::IterType expItBegin = m_pGrp->m_pFut->m_pExp->m_coll.begin();
		CMmQvExpColl::IterType expItEnd   = m_pGrp->m_pFut->m_pExp->m_coll.end();
		for(CMmQvExpColl::IterType expIt = expItBegin; expIt!=expItEnd; ++expIt)
		{
			IMmQvExpAtomPtr				spExpiry = expIt->second;
			CComObject<CMmQvExpAtom>*	pExpiry = dynamic_cast<CComObject<CMmQvExpAtom>*>(spExpiry.GetInterfacePtr());

			IEtsMmFilterAtomPtr spFilterAtom;
			if(!bAllExpirations && bInitialize)
				spFilterAtom = m_spVisibleExp->Item[_bstr_t(static_cast<long>(pExpiry->m_dtExpiryMonth))];
			if(!bInitialize ||  bAllExpirations  || spFilterAtom != NULL)
			{
				if(bInitialize)
					pExpiry->m_bVisible = VARIANT_TRUE;
				else
					if(pExpiry->m_bVisible == VARIANT_FALSE)
						continue;

				CMmQvStrikeColl::IterType strikeItBegin = pExpiry->m_pStrike->m_coll.begin();
				CMmQvStrikeColl::IterType strikeItEnd	= pExpiry->m_pStrike->m_coll.end();
				for (CMmQvStrikeColl::IterType strikeIt = strikeItBegin; strikeIt != strikeItEnd; ++strikeIt) 
				{
					IMmQvStrikeAtomPtr				spStrike = strikeIt->second;
					CComObject<CMmQvStrikeAtom>*	pStrike  = dynamic_cast<CComObject<CMmQvStrikeAtom>*>(spStrike.GetInterfacePtr());
					IEtsMmEntityAtomPtr spEntity;
					if(!bAllStrikes && bInitialize)
						spEntity = m_spVisibleStr->Item[_bstr_t(pStrike->m_dStrike)];

					if(!bInitialize || bAllStrikes || spEntity != NULL)
					{
						if(bInitialize)
							pStrike->m_bVisible = VARIANT_TRUE;
						else
							if(pStrike->m_bVisible==VARIANT_FALSE)
								continue;

						m_pGrp->m_pStrikeAll->GetStrike(pStrike->m_dStrike)->put_Visible(VARIANT_TRUE);

						CMmQvOptPairColl::EnumCollType::iterator opItBegin = pStrike->m_pOptPair->m_coll.begin();
						CMmQvOptPairColl::EnumCollType::iterator opItEnd   = pStrike->m_pOptPair->m_coll.end();
						for(CMmQvOptPairColl::EnumCollType::iterator opIt = opItBegin; opIt!=opItEnd; ++opIt)
						{
							IMmQvOptPairAtomPtr spPairAtom = opIt->second;
							CComObject<CMmQvOptPairAtom>* pPairAtom = dynamic_cast<CComObject<CMmQvOptPairAtom>*>(spPairAtom.GetInterfacePtr());
							for(CExchanges::iterator itrExch = vecVisibleExchanges.begin(); itrExch != vecVisibleExchanges.end(); ++itrExch)
							{
								IExchAtomPtr spExchange = m_pGrp->m_pFut->m_spOptExch->Item[itrExch->first];
								if(spExchange!=NULL)
								{
									CComObject<CMmQvRowData>* pRowData = NULL;
									CComObject<CMmQvRowData>::CreateInstance(&pRowData);
									if(pRowData)
									{
										IMmQvRowDataPtr spRowData;
										spRowData.Attach(pRowData, TRUE);

										pRowData->m_spExp		= spExpiry;
										pRowData->m_spExch		= spExchange;
										pRowData->m_spStrike    = spStrike;
										pRowData->m_spFut		= m_pGrp->m_spFut;
										pRowData->m_spFutRoot	= m_pGrp->m_spFutRoot;

										pRowData->m_pExp		= pExpiry;
										pRowData->m_pStrike     = pStrike;
										pRowData->m_pGrp        = m_pGrp;
										pRowData->m_pFut        = m_pGrp->m_pFut;
										pRowData->m_pFutRoot    = m_pGrp->m_pFutRoot;

										if(pPairAtom->m_pOpt[enOtCall]->m_nID)
										{
											pRowData->m_spOpt[enOtCall] = pPairAtom->m_spOpt[enOtCall];
											pRowData->m_spOptQuote[enOtCall] = pPairAtom->m_pOpt[enOtCall]->m_pQuote->GetQuote(itrExch->first);

											//pRowData->m_pOpt[enOtCall] = pPairAtom->m_pOpt[enOtCall];
											pRowData->m_pOpt[enOtCall] = dynamic_cast<CComObject<CMmQvOptAtom>*>(pRowData->m_spOpt[enOtCall].GetInterfacePtr());
											pRowData->m_pOptQuote[enOtCall] = dynamic_cast<CComObject<CMmQvQuoteAtom>*>(pRowData->m_spOptQuote[enOtCall].GetInterfacePtr());
										}

										if(pPairAtom->m_pOpt[enOtPut]->m_nID)
										{
											pRowData->m_spOpt[enOtPut] = pPairAtom->m_spOpt[enOtPut];
											pRowData->m_spOptQuote[enOtPut] = pPairAtom->m_pOpt[enOtPut]->m_pQuote->GetQuote(itrExch->first);

											//pRowData->m_pOpt[enOtPut]      = pPairAtom->m_pOpt[enOtPut]!=NULL ? pPairAtom->m_pOpt[enOtPut]:NULL;
											pRowData->m_pOpt[enOtPut] = dynamic_cast<CComObject<CMmQvOptAtom>*>(pRowData->m_spOpt[enOtPut].GetInterfacePtr());
											pRowData->m_pOptQuote[enOtPut] = dynamic_cast<CComObject<CMmQvQuoteAtom>*>(pRowData->m_spOptQuote[enOtPut].GetInterfacePtr());
										}

										//m_Rows.insert(m_Rows.end(), spRowData);
										m_Rows.push_back(spRowData);
									}
								}
							}
						}
					}
				}
			}
		}
	}
	catch (_com_error& err)
	{
		hr = Error((PTCHAR)CComErrorWrapper::ErrorDescription(err), IID_IEtsMmQuotesView, err.Error());
	}
	/*catch(...)
	{
		HRESULT _hr = HRESULT_FROM_WIN32(::GetLastError());
		if(SUCCEEDED(_hr)) _hr = E_FAIL;
		hr = Error((PTCHAR)"System Error", IID_IEtsMmQuotesView, _hr);
	}*/

	return hr;
}

STDMETHODIMP CEtsMmQuotesView::OptsRefresh(VARIANT_BOOL vbInitialize)
{
	HRESULT hr = S_OK;
	try
	{
		m_Rows.clear();
		m_Rows.reserve(m_nOptQuotesCount);

		VARIANT_BOOL bIsStockOrIndex = VARIANT_FALSE;
		m_pGrp->get_IsStockOrIndex(&bIsStockOrIndex);
		if(bIsStockOrIndex)
			hr = RefreshStockOptions(vbInitialize!=VARIANT_FALSE);
		else
			hr = RefreshFuturesOptions(vbInitialize!=VARIANT_FALSE);
		RenumberItems();

	}
	catch (_com_error& err)
	{
		hr = Error((PTCHAR)CComErrorWrapper::ErrorDescription(err), IID_IEtsMmQuotesView, err.Error());
	}
	/*catch(...)
	{
		HRESULT _hr = HRESULT_FROM_WIN32(::GetLastError());
		if(SUCCEEDED(_hr)) _hr = E_FAIL;
		hr = Error((PTCHAR)"System Error", IID_IEtsMmQuotesView, _hr);
	}*/
	return hr;
}


STDMETHODIMP CEtsMmQuotesView::get_OptsRowData(long lRow , struct IMmQvRowData **ppData)
{
	HRESULT hr = S_OK;
	try
	{  
		if(ppData)
		{
			*ppData = NULL;
			if(lRow > 0 )
			{
				size_t  lPos = static_cast<size_t>(lRow-1);
				if(m_Rows.size()>lPos)
				{
					IMmQvRowDataPtr spAtom = m_Rows[lPos];
					if(spAtom!=NULL)
						*ppData = spAtom.Detach();
				}
			}
		}
	}
	catch (_com_error& err) 
	{
		hr = Error((PTCHAR)CComErrorWrapper::ErrorDescription(err), IID_IEtsMmQuotesView, err.Error());
	}
	/*catch(...)
	{
		HRESULT _hr = HRESULT_FROM_WIN32(::GetLastError());
		if(SUCCEEDED(_hr)) _hr = E_FAIL;
		hr = Error((PTCHAR)"System Error", IID_IEtsMmQuotesView, _hr);
	}*/
	return hr;
}

STDMETHODIMP CEtsMmQuotesView::OptsSort(enum MmQvOptColumnEnum enField, enum EtsSortingEnum enSort)
{
	bool bResort = false;
	long enFieldID = static_cast<long>(enField)-2;
	if(m_enSortField != static_cast<MmQvOptColumnEnum>(m_ColumnsEncoding[enFieldID]))
	{
		m_enSortField = static_cast<MmQvOptColumnEnum>(m_ColumnsEncoding[enFieldID]);
		bResort		= true;
	}
	if(m_enSortType != enSort)
	{
		m_enSortType = enSort;
		bResort	   = true;
	}
	if(bResort)
		PerformSorting();
	return S_OK;

}
STDMETHODIMP CEtsMmQuotesView::raw_GetFieldCount ( long * pFields )
{
	*pFields = m_lVisibleColumns+2;
	return S_OK;
}
STDMETHODIMP CEtsMmQuotesView::raw_GetRecordCount(long * pRecords )
{
	*pRecords  = static_cast<long>(m_Rows.size());
	return S_OK;
}
STDMETHODIMP CEtsMmQuotesView::raw_GetFieldName( long Field, BSTR * pFieldName )
{
	*pFieldName = SysAllocString(L"");
	return S_OK;
}

STDMETHODIMP CEtsMmQuotesView::raw_GetData ( long Field, long Record, BSTR * pData )
{
	HRESULT hr = S_OK;

	*pData = NULL;
	long lFieldID = Field-2;
	MmQvOptColumnEnum enCol = QOC_NONE;
	try
	{
		if(Field > 0 && m_ColumnsEncoding[lFieldID]>=0 && static_cast<size_t>(Record) < m_Rows.size())
			enCol = static_cast<MmQvOptColumnEnum>(m_ColumnsEncoding[lFieldID]);
		
		if(Field == QOC_C_KEY && static_cast<size_t>(Record) < m_Rows.size())
			enCol = static_cast<MmQvOptColumnEnum>(Field);

		if(enCol != QOC_NONE)
		{
			IMmQvRowDataPtr spAtom = m_Rows[Record];
			CMmQvRowData* pAtom = static_cast<CMmQvRowData*>(spAtom.GetInterfacePtr());

			_variant_t  vt; 
			if(pAtom->GetField(enCol, vt))
			{
				if(vt.vt == VT_BSTR)
					*pData = SysAllocString(vt.bstrVal);
				else
				{
					_variant_t vtOut;
					vtOut.ChangeType(VT_BSTR, &vt);
					*pData = SysAllocString(vtOut.bstrVal);
				}
			}		
			else
			{
				*pData = ::SysAllocString(L"Unknown");
			}
		}

		if(!*pData)
			*pData = SysAllocString(L"Undefined");
	}
	catch (_com_error& err )
	{
		hr = Error((PTCHAR)CComErrorWrapper::ErrorDescription(err), IID_IEtsMmQuotesView, err.Error());
	}
	/*catch(...)
	{
		HRESULT _hr = HRESULT_FROM_WIN32(::GetLastError());
		if(SUCCEEDED(_hr)) _hr = E_FAIL;
		hr = Error((PTCHAR)"System Error", IID_IEtsMmQuotesView, _hr);
	}*/
	return hr;

}
STDMETHODIMP CEtsMmQuotesView::raw_SetData		 (/*[in]*/ long Field,	/*[in]*/ long Record,/*[in]*/ BSTR newData )
{
	long lFieldID = Field-2;
	HRESULT hr = S_OK;
	try
	{
		long lColumnId = m_ColumnsEncoding[lFieldID];
		switch (lColumnId) 
		{
			case QOC_C_BID:
			case QOC_C_ASK:
			case QOC_C_VOLA:
			case QOC_P_BID:
			case QOC_P_ASK:
			case QOC_P_VOLA:
				{
					IMmQvRowDataPtr spAtom = m_Rows[Record];
					CMmQvRowData* pAtom = static_cast<CMmQvRowData*>(spAtom.GetInterfacePtr());
					if(pAtom)
					{
						pAtom->SetField(lColumnId, _variant_t(_bstr_t(newData)));
					}
				}
				break;
			case QOC_C_LAST:
				{
					IMmQvRowDataPtr spAtom = m_Rows[Record];
					CMmQvRowData* pAtom = static_cast<CMmQvRowData*>(spAtom.GetInterfacePtr());
					_variant_t help = _bstr_t(newData);
	
					if(pAtom)
					{
						pAtom->SetField(QOC_C_LAST,help);
						pAtom->SetField(QOC_C_BID, help);
						pAtom->SetField(QOC_C_ASK, help);
					}
				}
				break;
			case QOC_P_LAST:
				{
					IMmQvRowDataPtr spAtom = m_Rows[Record];
					CMmQvRowData* pAtom = static_cast<CMmQvRowData*>(spAtom.GetInterfacePtr());
					_variant_t help =_bstr_t(newData);
					if(pAtom)
					{
						pAtom->SetField(QOC_P_LAST, help);
						pAtom->SetField(QOC_P_BID, help);
						pAtom->SetField(QOC_P_ASK, help);
					}
				}
				break;
		}
	}
	catch (_com_error& err )
	{
		hr = Error((PTCHAR)CComErrorWrapper::ErrorDescription(err), IID_IEtsMmQuotesView, err.Error());
	}
	/*catch(...)
	{
		HRESULT _hr = HRESULT_FROM_WIN32(::GetLastError());
		if(SUCCEEDED(_hr)) _hr = E_FAIL;
		hr = Error((PTCHAR)"System Error", IID_IEtsMmQuotesView, _hr);
	}*/

//	QOC_C_BID, QOC_C_ASK, QOC_C_VOLA
//	QOC_P_BID, QOC_P_ASK, QOC_P_VOLA
	return hr;
}


STDMETHODIMP CEtsMmQuotesView::put_ProcessorsLoad(LONG nProcessors)
{
	HRESULT hr = S_OK;
	try
	{
		if(m_pGrp && m_pGrp->m_pUnd && nProcessors)
		{
			m_nProcessors = nProcessors;
			m_pGrp->m_pUnd->SetProcessorsNum(nProcessors);
		}
	}
	catch (_com_error& e)
	{
		hr = Error((PTCHAR)CComErrorWrapper::ErrorDescription(e), IID_IEtsMmQuotesView, e.Error());
	}
	/*catch(...)
	{
		HRESULT _hr = HRESULT_FROM_WIN32(::GetLastError());
		if(SUCCEEDED(_hr)) _hr = E_FAIL;
		hr = Error((PTCHAR)"System Error", IID_IEtsMmQuotesView, _hr);
	}*/
	return hr;
}

STDMETHODIMP CEtsMmQuotesView::CleanUp(VARIANT_BOOL vbAll)
{
	HRESULT hr = S_OK;
	try
	{
		if(m_pGrp)
		{
			m_pGrp->CleanUp(vbAll);
			if(m_pQuoteRequestAll)
				m_pQuoteRequestAll->Clear();
			m_Rows.clear();
		}
	}
	catch (_com_error& e)
	{
		hr = Error((PTCHAR)CComErrorWrapper::ErrorDescription(e), IID_IEtsMmQuotesView, e.Error());
	}
	/*catch(...)
	{
		HRESULT _hr = HRESULT_FROM_WIN32(::GetLastError());
		if(SUCCEEDED(_hr)) _hr = E_FAIL;
		hr = Error((PTCHAR)"System Error", IID_IEtsMmQuotesView, _hr);
	}*/
	return hr;

}

// EtsMain.cpp : Implementation of CEtsMain

#include "stdafx.h"
#include "EtsMain.h"
#include "SpotOptionContract.h"
#include "Curve.h"

#include "CalculationManager.h"
#include "BatchCollector.h"
#include "DataPacker.h"

// CEtsMain
STDMETHODIMP CEtsMain::Clear()
{
	try
	{
		_Clear();

	}
	catch(_com_error& e )
	{
		return Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_IEtsMain, e.Error());

	}
	catch(...)
	{
		return Error(_T("Unknown Error in Main::Clear "), IID_IEtsMain, E_FAIL);
	}
	return S_OK;
}

STDMETHODIMP CEtsMain::LoadBetas()
{
	try
	{
		_bstr_t bsStatus(L"Loading index underlying betas...");

		Fire_Progress(bsStatus, -1);
		InitializeDB();
		CStoredProc<> sp(m_DbConnection, L"usp_MmIndexUnderlyingBeta_Get");
		CClientRecordset rs;
		rs.Open(sp);

		long lTotalCount = rs.GetRecordCount();
		long lLoaded = 0;
		long lLastPerc = -1;

		if(lTotalCount && m_pIndex)
		{
			for(rs.MoveFirst(); !rs.IsEOF(); ++rs, ++lLoaded)
			{
				double dBeta = rs[L"fBeta"];
				if(dBeta!=0.0)
				{
					long nIdxID = rs[L"iIndexID"];
					long nID    = rs[L"iContractID"];
					
					IUndAtomPtr spUndAtom;
					m_pUnderlyingAll->get_Item(nIdxID, &spUndAtom);
					if(spUndAtom)
					{
						VARIANT_BOOL bIsHedgeSymbol = VARIANT_FALSE;
						spUndAtom->get_IsHedgeSymbol(&bIsHedgeSymbol);
						if(bIsHedgeSymbol!=VARIANT_FALSE)
						{
							_bstr_t bsIndexSymbol = rs[L"vcIndexSymbol"];
							// Hedge Symbol
							IIndexAtomPtr spHedgeIndex = m_pHedgeSymbols->GetIndex(nIdxID);
							if(spHedgeIndex==NULL)
							{
								// Add New	
								m_pHedgeSymbols->Add(nIdxID, bsIndexSymbol,NULL, &spHedgeIndex);
							}
							if(spHedgeIndex!=NULL)
							{
								spHedgeIndex->put_ID(nIdxID);
								
								if ((bool)m_spContractsCache)
									m_spContractsCache->GetContract(nIdxID);

								spHedgeIndex->put_Symbol(bsIndexSymbol);

								IEtsPriceProfileAtomPtr spProfile;
								spUndAtom->get_UndPriceProfile(&spProfile);
								spHedgeIndex->putref_UndPriceProfile(spProfile);

								spProfile = NULL;
								spUndAtom->get_OptPriceProfile(&spProfile);
								spHedgeIndex->putref_OptPriceProfile(spProfile);

								IIndexCompCollPtr spIdxCompColl;
								spHedgeIndex->get_Components(&spIdxCompColl);
								if(spIdxCompColl != NULL)
								{
									IIndexCompAtomPtr spCompAtom;
									spIdxCompColl->get_Item(nID, &spCompAtom);
									if(spCompAtom==NULL)
										spIdxCompColl->Add(nID, NULL, &spCompAtom);

									if(spCompAtom!=NULL)
									{
										spCompAtom->put_Beta(dBeta);
										spHedgeIndex->put_HaveComponentBetas(VARIANT_TRUE);
									}
								}
							}
						}
					}

					IIndexAtomPtr spIdx = m_pIndex->GetIndex(nIdxID);
					if(spIdx)
					{
						IIndexCompCollPtr spIdxCompColl;
						spIdx->get_Components(&spIdxCompColl);
						if(spIdxCompColl != NULL)
						{
							IIndexCompAtomPtr spCompAtom;
							spIdxCompColl->get_Item(nID, &spCompAtom);
							if(spCompAtom==NULL)
								spIdxCompColl->Add(nID, NULL, &spCompAtom);

							if(spCompAtom!=NULL)
							{
								spCompAtom->put_Beta(dBeta);
								spIdx->put_HaveComponentBetas(VARIANT_TRUE);
							}						

						}
					}
				}
				long lPerc = static_cast<long>( 100.* double(lLoaded)/ double(lTotalCount));
				if(lPerc != lLastPerc)
				{
					lLastPerc = lPerc;
					Fire_Progress(bsStatus,lLastPerc);
				}
			}
		}
	}
	catch(_com_error& e )
	{
		return Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_IEtsMain, e.Error());

	}
	catch(...)
	{
		return Error(_T("Unknown Error in CEtsMain::LoadStrategy "), IID_IEtsMain, E_FAIL);
	}
	return S_OK;

}

STDMETHODIMP CEtsMain::LoadStrategy()
{
	try
	{
		Fire_Progress(L"Loading strategies data...", -1);

		m_pStrategy->Clear();
		InitializeDB();
		CStoredProc<> sp(m_DbConnection, L"usp_Strategy_Get");
		CClientRecordset rs;
		rs.Open(sp);
		if(rs.GetRecordCount())
		{
			rs.MoveFirst();
			while(!rs.IsEOF())
			{
				IEtsStrategyAtomPtr spStrategy;
				long lStrategyID = rs[L"iStrategyID"];

				m_pStrategy->get_Item(lStrategyID, &spStrategy);
				if(spStrategy == NULL)
				{
					_bstr_t bsStrategyName = rs[L"vcStrategyName"];
					_bstr_t bsDescription  = rs[L"vcDescription"];

					spStrategy = m_pStrategy->AddNew(lStrategyID, bsStrategyName, bsDescription);
				}
				rs.MoveNext();
			}
		}
	}
	catch(_com_error& e )
	{
		return Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_IEtsMain, e.Error());

	}
	catch(...)
	{
		return Error(_T("Unknown Error in CEtsMain::LoadStrategy "), IID_IEtsMain, E_FAIL);
	}
	return S_OK;
}
STDMETHODIMP CEtsMain::LoadBroker()
{
	try
	{
		Fire_Progress(L"Loading brokers data...", -1);

		m_pBroker->Clear();
		m_pClearingBroker->Clear();
		InitializeDB();

		CStoredProc<CClientRecordset> rs(m_DbConnection, L"usp_Broker_Get");
		rs.Open();
		if(rs.GetRecordCount())
		{
			rs.MoveFirst();
			while(!rs.IsEOF())
			{
				long lID = rs[L"iBrokerID"];
				bool bIsClearing = rs[L"tiIsClearing"];

				IEtsBrokerAtomPtr spBroker;
				CComObject<CEtsBrokerAtom>* pAtom= NULL;
				if(!bIsClearing)
				{
					m_pBroker->get_Item(lID, &spBroker);
					if(spBroker == NULL)
					{
						IEtsBrokerAtomPtr spAtom = m_pBroker->AddNew(lID, rs[L"vcBrokerName"], rs[L"vcDescription"], &pAtom);
						if(spAtom!=NULL && pAtom!=NULL)
						{
							pAtom->m_bIsClearing = VARIANT_FALSE;
							pAtom->m_dUndComm    = rs[L"fUndComm"];
							pAtom->m_dOptRegComm = rs[L"fOptRegComm"];
							pAtom->m_dOptSpreadComm = rs[L"fOptSpreadComm"];
							pAtom->m_dLowPremComm = rs[L"fLowPremComm"];
						}
					}
				}
				else
				{
					m_pClearingBroker->get_Item(lID, &spBroker);
					if(spBroker == NULL)
					{
						IEtsBrokerAtomPtr spAtom = m_pClearingBroker->AddNew(lID, rs[L"vcBrokerName"], rs[L"vcDescription"], &pAtom);
						if(spAtom!=NULL && pAtom!=NULL)
						{
							pAtom->m_bIsClearing = VARIANT_TRUE;
							pAtom->m_dUndComm    = rs[L"fUndComm"];
							pAtom->m_dOptRegComm = rs[L"fOptRegComm"];
							pAtom->m_dOptSpreadComm = rs[L"fOptSpreadComm"];
							pAtom->m_dLowPremComm = rs[L"fLowPremComm"];
						}
					}
				}
				rs.MoveNext();
			}
		}
	}
	catch(_com_error& e )
	{
		return Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_IEtsMain, e.Error());

	}
	catch(...)
	{
		return Error(_T("Unknown Error in CEtsMain::LoadBroker "), IID_IEtsMain, E_FAIL);
	}
	return S_OK;
}


STDMETHODIMP CEtsMain::LoadTraderGroup()
{
	HRESULT hr = S_OK;
	try
	{
		Fire_Progress(L"Loading trader groups data...",-1);

		m_pTraderGroup->Clear();
		InitializeDB();

		// Add Default Data
		//m_pTraderGroup->AddNew(0, L"<Defualt>", L"Default Trader Group");

		CStoredProc<CClientRecordset> rs(m_DbConnection, L"usp_TraderGroup_Get");
		rs.Open();
		if(rs.GetRecordCount())
		{
			rs.MoveFirst();
			while(!rs.IsEOF())
			{
				long nID = rs[L"iTraderGroupID"];
				IEtsTraderGroupAtomPtr spTraderGroup;
				CComObject<CEtsTraderGroupAtom> pTraderGroup = NULL;
				m_pTraderGroup->get_Item(nID, &spTraderGroup);
				if(spTraderGroup == NULL)
					m_pTraderGroup->AddNew(nID, rs[L"vcGroupName"], rs[L"vcGroupDesc"]);

				rs.MoveNext();
			}
		}
	}
	catch(_com_error& e )
	{
		hr = Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_IEtsMain, e.Error());

	}
	catch(...)
	{
		hr = Error(_T("Unknown Error in CEtsMain::LoadStrategy "), IID_IEtsMain, E_FAIL);
	}
	return hr;
}

STDMETHODIMP CEtsMain::LoadTrader(LONG lTraderID)
{
	HRESULT hr = S_OK;
	try
	{
		Fire_Progress(L"Loading traders data...",-1);
		m_pTraderColl->Clear();
		InitializeDB();

		CStoredProc<CClientRecordset> rs(m_DbConnection, L"usp_Trader_Get");
		if(lTraderID)
			rs << lTraderID;
		rs.Open();
		if(rs.GetRecordCount())
		{
			rs.MoveFirst();
			while (!rs.IsEOF()) 
			{
				IEtsTraderAtomPtr spTrader;
				CComObject<CEtsTraderAtom>* pTrader = NULL;

				long nID = rs[L"iTraderID"];
				m_pTraderColl->get_Item(nID, &spTrader);
				if(spTrader == NULL)
				{
					spTrader = m_pTraderColl->AddNew(nID, rs[L"vcAcronym"], &pTrader);
					if(spTrader != NULL && pTrader!=NULL)
					{
						pTrader->m_nGroupID = rs[L"iTraderGroupID"];

						IEtsTraderGroupAtomPtr spTraderGroup;
						m_pTraderGroup->get_Item(pTrader->m_nGroupID, &spTraderGroup);
						if(spTraderGroup!=NULL)
						{
							IEtsTraderCollPtr spTraderColl;
							spTraderGroup->get_Trader(&spTraderColl);
							if(spTraderColl!=NULL)
							{
								IEtsTraderAtomPtr spNew;
								spTraderColl->Add(nID, pTrader->m_bstrName, spTrader, &spNew);
							}
						}
					}
				}
				rs.MoveNext();
			}

			CStoredProc<CClientRecordset> rsTr(m_DbConnection, L"usp_TraderContractByTrader_Get");
			if(lTraderID)
				rsTr << lTraderID;
			rsTr.Open();
			if(rsTr.GetRecordCount())
			{
				rsTr.MoveFirst();
				while(!rsTr.IsEOF())
				{
					long lTraderId = rsTr[L"iTraderID"];
					IEtsTraderAtomPtr spTrader;
					m_pTraderColl->get_Item(lTraderId, &spTrader);
					if(spTrader!=NULL)
					{
						long lContractID = rsTr[L"iContractID"];
						IUndAtomPtr spUndAtom;
						m_pUnderlying->get_Item(lContractID, &spUndAtom);
						if(spUndAtom!=NULL)
						{
							IUndAtomPtr				spTraderUndAtom;
							IEtsTraderUndCollPtr	spTraderUndColl;
							spTrader->get_Und(&spTraderUndColl);
							if(spTraderUndColl!=NULL)
							{
								IEtsTraderUndAtomPtr spTrUndAtom;
								spTraderUndColl->get_Item(lContractID, &spTrUndAtom);
								if(spTrUndAtom == NULL)
								{	CComBSTR bsSymbol;
								spUndAtom->get_Symbol(&bsSymbol);
								IEtsTraderUndAtomPtr spNew;
								spTraderUndColl->Add(lContractID, bsSymbol, NULL, &spNew);
								if(spNew != NULL)
									spNew->putref_Und(spUndAtom);
								}
							}
						}
					}   

					rsTr.MoveNext();
				}
			}

		}
	}
	catch(_com_error& e )
	{
		hr = Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_IEtsMain, e.Error());

	}
	catch(...)
	{
		hr = Error(_T("Unknown Error in CEtsMain::LoadStrategy "), IID_IEtsMain, E_FAIL);
	}
	return hr;
}
//---------------------------------------------------------------------------------------------//
void	CEtsMain::CreatePricingAsset()
{
	try
	{
		CComObject<CUndAtom> *pUSD  = NULL;
		m_pUnderlying->AddNew(0, _bstr_t(L"USDAsset"), &pUSD);

		IEtsRateCollPtr	spCurve = NULL;
		GetBorrowingRatesCurve(0, &spCurve);
		pUSD->m_spBorrowingRatesCurve =	spCurve;

	}
	catch (_com_error&)
	{
		throw;
	}
};
//---------------------------------------------------------------------------------------------//
STDMETHODIMP CEtsMain::LoadOptionRoots()
{
	try
	{
		_bstr_t bsStatus("Loading Option Roots data...");
		Fire_Progress(bsStatus,-1);

		InitializeDB();
		CStoredProc<> db(m_DbConnection, L"usp_MmOptionRoot_Get");
		CClientRecordset rs;
		rs.Open(db);

		long lTotalCount = rs.GetRecordCount();
		long lLoaded = 0;
		long lLastPerc = -1;

		if(lTotalCount)
		{
			for(rs.MoveFirst(); !rs.IsEOF(); ++rs, ++lLoaded)
			{

				long lAssetID = rs[L"iUnderlyingID"];

				IUndAtomPtr spAsset = NULL;
				m_pUnderlyingAll->get_Item(lAssetID, &spAsset);

				if(static_cast<bool>(spAsset))
				{
					long lRootID = rs[L"iRootID"];

					IEtsOptRootCollPtr spRoots = NULL;
					spAsset->get_Roots(&spRoots);

					if (static_cast<bool>(spRoots)) 
					{
						IEtsOptRootAtomPtr spRoot = NULL;
						spRoots->get_Item(lRootID, &spRoot);
						if (spRoot == NULL)
						{
							_bstr_t vcSymbol = rs[L"vcSymbol"];
							spRoots->Add(lRootID, vcSymbol, NULL, &spRoot);

							if (static_cast<bool>(spRoot))
							{
								//standard root load rooutine
								_CHK(spRoot->put_Name(vcSymbol), _T(""));
								_CHK(spRoot->put_ID(lRootID), _T(""));
								_CHK(spRoot->put_UndID(lAssetID), _T(""));
								_CHK(spRoot->put_LotSize(static_cast<long>(rs[L"iLotSize"])), _T(""));

								_CHK(spRoot->put_ExerciseStyle((ExerciseStyleEnum)static_cast<LONG>(rs[L"iExerciseStyle"])),
									_T("Can't put ExerciseStyle value."));

								_CHK(spRoot->put_SettlementValueTime(static_cast<vt_date>(rs[L"dtSettlementValueTime"])),
									_T("Can't put SettlementValueTime."));

																
								_CHK(spRoot->put_IsFit(((bool)rs[L"tiIsFitEnabled"])?VARIANT_TRUE:VARIANT_FALSE), _T(""));
								_CHK(spRoot->put_IsSynth(((bool)rs[L"tiIsSynthetic"])?VARIANT_TRUE:VARIANT_FALSE), _T(""));
								

								_CHK(spAsset->put_HaveOptions(VARIANT_TRUE), _T(""));
								
								//synthetic root load routine
								VARIANT_BOOL bIsSynth = VARIANT_FALSE;
								_CHK(spRoot->get_IsSynth(&bIsSynth), _T(""));
								if ( bIsSynth == VARIANT_TRUE )
								{
									ISynthRootCollPtr spSynthRoots = NULL;
									_CHK(spAsset->get_SyntheticRoots(&spSynthRoots), _T("Fail to get Synhtetic Roots Coll."));
									if (spSynthRoots == NULL)
									{
										_CHK(spAsset->put_HaveSyntheticRoots(VARIANT_TRUE), _T(""));

										CComObject<CSynthRootColl>* pSynthRootColl = NULL;
										_CHK(CComObject<CSynthRootColl>::CreateInstance(&pSynthRootColl), _T("Fail to add synthetic root."));
										spSynthRoots.Attach(pSynthRootColl, TRUE);
										
										_CHK(spAsset->putref_SyntheticRoots(spSynthRoots), _T(""));
									}

									ISynthRootAtomPtr spSynthRoot = NULL;
									_CHK(spSynthRoots->Add(lRootID, NULL, &spSynthRoot), _T(""));

									if (static_cast<bool>(spSynthRoot))
									{
										spSynthRoot->put_Basket(static_cast<VARIANT_BOOL>(rs[L"tiSyntheticIsBasket"]));
										spSynthRoot->put_CashValue(static_cast<double>(rs[L"fCash"]));
										spSynthRoot->put_Skew(static_cast<double>(rs[L"fSyntheticSkew"]));
										spSynthRoot->put_Kurt(static_cast<double>(rs[L"fSyntheticKurt"]));
										spSynthRoot->put_Yield(static_cast<double>(rs[L"fSyntheticYield"]));
										spSynthRoot->put_OptRootID(lRootID);
									}
								}
							};

							//init collection of root's by symbol
							IEtsOptRootAtomPtr spItem = NULL;
							m_pOptRootByName->get_Item(vcSymbol, &spItem);
							if (spItem == NULL)
							{
								IEtsOptRootAtomPtr spRet = NULL;
								m_pOptRootByName->Add(vcSymbol, spItem, &spRet);
							}
						}
					}
				}

				long lPerc = static_cast<long>( 100.* double(lLoaded)/ double(lTotalCount));
				
				if(lPerc != lLastPerc)
				{
					lLastPerc = lPerc;
					Fire_Progress(bsStatus,lLastPerc);
				}
			}
		}
	}
	catch (_com_error& e)
	{
		return Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_IEtsMain, e.Error());
	}
	catch (...)
	{
		return Error(_T("Unknown Error in CEtsMain::LoadOptionRoot()"), IID_IEtsMain, E_FAIL);
	};
	return S_OK;
};
//---------------------------------------------------------------------------------------------//
STDMETHODIMP CEtsMain::LoadGroups(LONG lCurrentTraderID)
{
	try
	{
		m_pUnderlyingGroup->Clear();

		_bstr_t bsStatus("Loading Contract Groups data...");
		Fire_Progress(bsStatus,-1);

		InitializeDB();
		CStoredProc<> db(m_DbConnection, L"usp_ContractGroup_Get");
		CClientRecordset rsCG;
		rsCG.Open(db);

		long lTotalCount = rsCG.GetRecordCount();
		long lLoaded = 0;
		long lLastPerc = -1;

		if(lTotalCount)
		{
			for(rsCG.MoveFirst(); !rsCG.IsEOF(); ++rsCG, ++lLoaded)
			{

				long lGroupID = static_cast<long>(rsCG[L"iGroupID"]);

				IEtsUndGroupAtomPtr spGroup = NULL;
				m_pUnderlyingGroup->get_Item(lGroupID, &spGroup);

				if (spGroup == NULL)
				{
					_bstr_t vcGroupName = rsCG[L"vcGroupName"];
					_bstr_t vcDescription = rsCG[L"vcDescription"];

					m_pUnderlyingGroup->Add(lGroupID, vcGroupName, NULL, &spGroup);

					if (static_cast<bool>(spGroup))
					{
						spGroup->put_Name(vcGroupName);
						spGroup->put_Desc(vcDescription);
						spGroup->put_ID(lGroupID);
					};

					//init underlying coll of current group
					CStoredProc<> dbCIG(m_DbConnection, L"usp_ContractInGroup_Get");
					CClientRecordset rsCIG;
					dbCIG<<lGroupID;
					rsCIG.Open(dbCIG);

					//parse recordset
					if (rsCIG.GetRecordCount())
					{
						IUndAtomPtr spAsset = NULL;
						for(rsCIG.MoveFirst(); !rsCIG.IsEOF(); ++rsCIG)
						{
							long lContractID = rsCIG[L"iContractID"];

							if (lCurrentTraderID == 0) 
								m_pUnderlyingAll->get_Item(lContractID, &spAsset);
							else
								m_pUnderlying->get_Item(lContractID, &spAsset);
								
						   if (static_cast<bool>(spAsset))
						   {
							   IUndCollPtr spAssetsGroupColl = NULL;
							   spGroup->get_Und(&spAssetsGroupColl);
							   if (static_cast<bool>(spAssetsGroupColl))
							   {
								   IUndAtomPtr spItem = NULL;
								   spAssetsGroupColl->get_Item(lContractID, &spItem);

								   if (spItem == NULL)
								   {
									   BSTR bsSymbol = NULL;
									   spAsset->get_Symbol(&bsSymbol);
									   IUndAtomPtr spRet = NULL;
									   spAssetsGroupColl->Add(lContractID, bsSymbol, spAsset, &spRet);
								   }
							   }
						   }
						}
					}
					rsCIG.Close();

					//-----------------------------------//
					//if (lCurrentTraderID == 0)
					//{
					//	IEtsUndGroupAtomPtr spRet = NULL;
					//	m_pUnderlyingGroup->Add(lGroupID, vcGroupName, spGroup, &spRet);
					//}
				}

				//Update status
				long lPerc = static_cast<long>( 100.* double(lLoaded)/ double(lTotalCount));
				if(lPerc != lLastPerc)
				{
					lLastPerc = lPerc;
					Fire_Progress(bsStatus,lLastPerc);
				}
			}
		}
	}
	catch (_com_error& e)
	{
		return Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_IEtsMain, e.Error());
	}
	catch (...)
	{
		return Error(_T("Unknown Error in CEtsMain::LoadGroups()"), IID_IEtsMain, E_FAIL);
	}
	return S_OK;
};
//---------------------------------------------------------------------------------------------//
STDMETHODIMP CEtsMain::LoadUnderlying(LONG lTraderID)
{
	try
	{

		CCalculatorPtr calculator = m_spContractsCache->GetCalculator();

		calculation_mgr_ = new CalculationManager(	new BatchCollector, 
													calculator.get(),
													new DataPacker,
													this);

		CGenegalSettings::SetConnectionString(m_bstrDatabaseString);
		
		m_spContractsCache->SetMain(dynamic_cast<IEtsMain*>(this));
		m_spContractsCache->SetConnectionString(CGenegalSettings::GetDBConnectionString());

		CContractRootCache::getInstance()->SetMain(dynamic_cast<IEtsMain*>(this));
		CContractRootCache::getInstance()->SetConnectionString(CGenegalSettings::GetDBConnectionString());

		CAssetCorrelation::getInstance()->SetConnectionString(CGenegalSettings::GetDBConnectionString());


		LoadHolidays();

		m_pUnderlying->Clear();
		m_pUnderlyingAll->Clear();
		m_pStock->Clear();
		m_pOptRootByName->Clear();
		m_pContract->Clear();
		m_pContractAll->Clear();
		_bstr_t bsStatus("Loading Underlying data...");

		InitializeDB();
		Fire_Progress(bsStatus,-1);
		CStoredProc<> db(m_DbConnection, L"usp_MmUnderlying_Get");
		
		if(lTraderID)
			db << lTraderID;
		
		CClientRecordset rs;
		rs.Open(db);
		long lTotalCount = rs.GetRecordCount();
		long lLoaded = 0;
		long lLastPerc = -1;
		if(lTotalCount)
		{
			for(rs.MoveFirst(); !rs.IsEOF(); ++rs, ++lLoaded)
			{
				long lID = rs[L"iContractID"];
				IUndAtomPtr spAtom = NULL;
				m_pUnderlyingAll->get_Item(lID,&spAtom);

				
				if(spAtom == NULL)
				{
					_bstr_t bsSymbol = rs[L"vcSymbol"];
					CComObject<CUndAtom> *pObject  = NULL;
					
					spAtom = m_pUnderlyingAll->AddNew(lID, bsSymbol, &pObject);
					if(pObject)
					{
						EtsContractTypeEnum enContractType = static_cast<EtsContractTypeEnum>((long)rs[L"iContractTypeID"]);
						pObject->put_UndType(enContractType);

						pObject->price_option_using_fwd_curve = (long(rs[L"price_option_using_forward_curve"]) != 0);

						pObject->m_bIsAmerican		= ((bool)rs[L"tiCalcOptionType"])?VARIANT_TRUE:VARIANT_FALSE;
						pObject->m_bIsHTB			= ((bool)rs[L"tiIsHTB"])?VARIANT_TRUE:VARIANT_FALSE;

						if(pObject->m_bIsHTB)
							pObject->m_bIsAllowLending = ((bool)rs[L"tiIsAllowLending"]) ? VARIANT_TRUE : VARIANT_FALSE;
						else
							pObject->m_bIsAllowLending = VARIANT_FALSE;
						
						_bstr_t bsContractName = rs[L"vcContractName"];
						pObject->put_ContractName(bsContractName); 

						pObject->put_LotSize(1);
						pObject->put_PriceClose(rs[L"fPriceClose"]);
						pObject->put_PriceTheoClose(rs[L"fPriceTheoClose"]);
						pObject->put_Skew(rs[L"fSkew"]);
						pObject->put_Kurt(rs[L"fKurt"]);
						pObject->put_PrimaryExchangeID(rs[L"iPrimaryExchangeID"]);
						pObject->put_ManualActivePrice(rs[L"dManualActivePrice"]);

						pObject->m_bIsHedgeSymbol = ((bool)rs[L"tiIsHedgeSymbol"]) ? VARIANT_TRUE : VARIANT_FALSE;

						long lUndTraderID = rs[L"iTraderID"];
						pObject->m_bIsTraderContract	= (lUndTraderID ==	lTraderID)?VARIANT_TRUE:VARIANT_FALSE;
						pObject->m_bIsActive			= (bool)rs[L"tiIsActive"]?VARIANT_TRUE:VARIANT_FALSE;
						pObject->m_nVolume10Day			= rs[L"iVolume10Day"];
						pObject->m_nVolume5Expiration	= rs[L"iVolume5Expiration"];

						pObject->m_fIsManualVol = (bool)rs[L"fManualVol"]?VARIANT_TRUE:VARIANT_FALSE;

						long lUndPriceProfileID =rs[L"iUndPriceProfileID"];
						long lOptPriceProfileID =rs[L"iOptPriceProfileID"];
						m_pPriceProfile->get_Item(lUndPriceProfileID, &pObject->m_spUndPriceProfile);
						m_pPriceProfile->get_Item(lOptPriceProfileID, &pObject->m_spOptPriceProfile);


						switch(enContractType)
						{
						case enCtStock:
							{

								CComObject<CEtsIndexDivAtom>* pDivAtom = NULL;
								CComObject<CEtsIndexDivAtom>::CreateInstance(&pDivAtom);
								IEtsIndexDivAtomPtr spDivAtom;
								if(pDivAtom)
								{
									spDivAtom.Attach(pDivAtom, TRUE);

									pDivAtom->m_enDivType		= static_cast<EtsDivTypeEnum>((long)rs[L"tiIsDivCustom"]);
									pDivAtom->m_nDivFreq		= rs[L"iDivFreq"];
									pDivAtom->m_dtDivDate		= (vt_date) rs[L"dtDivDate"];
									pDivAtom->m_dDivAmt			= rs[L"fDivAmt"];
									pDivAtom->m_nDivFreqCust	= rs[L"iDivFreqCustom"];
									pDivAtom->m_dtDivDateCust   = (vt_date) rs[L"dtDivDateCustom"];
									pDivAtom->m_dDivAmtCust     = rs[L"fDivAmtCustom"];
									pDivAtom->m_spHolidays		= m_spHolidays;
									pDivAtom->m_nAssetID		= lID;

									pObject->m_spDividend		= spDivAtom;
								}
								if(pObject->m_spUndPriceProfile == NULL)
									pObject->m_spUndPriceProfile = m_spDefStkPriceProfile;

								IUndAtomPtr spUndAtom;
								m_pStock->Add(lID, bsSymbol, spAtom, &spUndAtom);

							}
							break;
						case enCtIndex:
							{
								pObject->m_dYield = rs[L"fYield"];
								pObject->m_bIsBasketIndex = VARIANT_TRUE;//((bool)rs[L"tiIsBasket"])?VARIANT_TRUE:VARIANT_FALSE;

								CComObject<CEtsIndexDivAtom>* pDivAtom = NULL;
								CComObject<CEtsIndexDivAtom>::CreateInstance(&pDivAtom);
								IEtsIndexDivAtomPtr spDivAtom;
								if(pDivAtom)
								{
									spDivAtom.Attach(pDivAtom, TRUE);

									pDivAtom->m_enDivType		= static_cast<EtsDivTypeEnum>((long)rs[L"tiIsDivCustom"]);
									pDivAtom->m_nDivFreq		= rs[L"iDivFreq"];
									pDivAtom->m_dtDivDate		= (vt_date) rs[L"dtDivDate"];
									pDivAtom->m_dDivAmt			= rs[L"fDivAmt"];
									pDivAtom->m_nDivFreqCust	= rs[L"iDivFreqCustom"];
									pDivAtom->m_dtDivDateCust   = (vt_date) rs[L"dtDivDateCustom"];
									pDivAtom->m_dDivAmtCust     = rs[L"fDivAmtCustom"];
									pDivAtom->m_spHolidays		= m_spHolidays;
									pDivAtom->m_nAssetID		= lID;

									pObject->m_spDividend		= spDivAtom;
								}

								if(pObject->m_spUndPriceProfile == NULL)
									pObject->m_spUndPriceProfile = m_spDefIdxPriceProfile;
								
								pObject->m_dSOQ = rs[L"SOQ"];
							}
							break;
						}
						if(pObject->m_spOptPriceProfile == NULL)
							pObject->m_spOptPriceProfile = m_spDefOptPriceProfile;


						if(pObject->m_bIsTraderContract)
						{	
							IUndAtomPtr spNewAtom;
							m_pUnderlying->Add(lID, bsSymbol, spAtom, &spNewAtom);
						}

						IEtsContractAtomPtr	spContractAtom;
						m_pContractAll->get_Item(lID, &spContractAtom);
						if(spContractAtom==NULL)
						{
							CComObject<CEtsContractAtom>* pObj = NULL;
							spContractAtom = m_pContractAll->AddNew(lID, bsSymbol, &pObj);
							if(pObj)
								pObj->m_spUnd = spAtom;
							if(pObject->m_bIsTraderContract)
							{
								IEtsContractAtomPtr  spNA;
								m_pContract->Add(lID, bsSymbol,spContractAtom, &spNA);
							}
						}
						//----------------------------------------------------------------//
						//----------------------------------------------------------------//
						IEtsRateCollPtr	spCurve = NULL;
						GetBorrowingRatesCurve(lID, &spCurve);
						pObject->m_spBorrowingRatesCurve =	spCurve;					
						//----------------------------------------------------------------//
					}
				}
				long lPerc = static_cast<long>( 100.* double(lLoaded)/ double(lTotalCount));
				if(lPerc != lLastPerc)
				{
					lLastPerc = lPerc;
					Fire_Progress(bsStatus,lLastPerc);
				}
			}
		}
		
		CreatePricingAsset();
		LoadAssetGroup();
		LoadRate();
		LoadCustomDividends();
		LoadBetas();

		initialize_curves();
		m_spContractsCache->InitializeFromTrades();
	}
	catch(_com_error& e )
	{
		return Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_IEtsMain, e.Error());

	}
	catch(...)
	{
		return Error(_T("Unknown Error in CEtsMain::LoadUnderlying "), IID_IEtsMain, E_FAIL);
	}
	return S_OK;
}

STDMETHODIMP CEtsMain::LoadRate()
{
	try
	{
		m_pRates->Clear();
		Fire_Progress(L"Loading interest rates data...", -1);
		InitializeDB();

		CStoredProc<CClientRecordset> rs(m_DbConnection, L"usp_IRPoint_Get");
		rs << CDBNull()
		   << false;

		rs.Open();

		if(rs.GetRecordCount() == 0)
			return S_OK;

		rs.MoveFirst();

		while (!rs.IsEOF()) 
		{
			IEtsRateAtomPtr spRate;
			CComObject<CEtsRateAtom>* pRate = NULL;

			EtsPeriodTypeEnum enPeriodType	= static_cast<EtsPeriodTypeEnum>((LONG)rs[L"iPeriodTypeID"]);
			LONG nPeriodAmount	= rs[L"iNum"];
			LONG nPeriodInDays = 0;
			m_pRates->GetPeriodInDays(vt_date::GetCurrentDate(true), enPeriodType, nPeriodAmount, &nPeriodInDays);

			LONG nContractID	= rs[L"iContractID"];

			if(nContractID == 0)
			{	// global interest rate
				spRate = m_pRates->AddNew(nPeriodInDays, &pRate);
				if(spRate != NULL && pRate != NULL)
				{
					pRate->m_nID			= rs[L"iIRPointID"];
					pRate->m_dLongRate		= rs[L"fLongRate"];
					pRate->m_dShortRate		= rs[L"fShortRate"];
					pRate->m_dNeutralRate	= rs[L"NeutralRate"];
					
					pRate->m_enPeriodType	= enPeriodType;
					pRate->m_nPeriodAmount	= nPeriodAmount;
				}
			}
			else
			{	// HTB rate for specific underlying
				IUndAtomPtr spUndAtom;
				m_pUnderlying->get_Item(nContractID, &spUndAtom);
				if(spUndAtom != NULL)
				{
					VARIANT_BOOL bIsHTB = VARIANT_FALSE;
					spUndAtom->get_IsHTB(&bIsHTB);

					//if(bIsHTB) //always load HTB rates
					{
						IEtsRateCollPtr spRateColl;
						spUndAtom->get_HTBRates(&spRateColl);

						if(spRateColl == NULL)
						{
							CComObject<CEtsRateColl>* pRateColl = NULL;
							_CHK(CComObject<CEtsRateColl>::CreateInstance(&pRateColl), _T("Fail to create rate collection."));
							spRateColl.Attach(pRateColl, TRUE);
							
							spUndAtom->putref_HTBRates(spRateColl);
						}

						_CHK(spRateColl->Add(nPeriodInDays, NULL, &spRate), _T("Fail to add new rate."));

						spRate->put_ID(			rs[L"iIRPointID"]	);
						spRate->put_LongRate(	rs[L"fLongRate"]	);
						spRate->put_ShortRate(	rs[L"fShortRate"]	);

						spRate->put_PeriodType(enPeriodType);
						spRate->put_PeriodAmount(nPeriodAmount);
					}
				}	
			}
			rs.MoveNext();
		}
		
	}
	catch(_com_error& e )
	{
		return Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_IEtsMain, e.Error());
	}
	catch(...)
	{
		return Error(_T("Unknown Error in CEtsMain::LoadRate"), IID_IEtsMain, E_FAIL);
	}

	return S_OK;
}
STDMETHODIMP CEtsMain::LoadAssetGroup()
{
	try
	{

		_bstr_t bsStatus("Loading Asset Group data...");

		InitializeDB();
		Fire_Progress(bsStatus,-1);
		CStoredProc<> db(m_DbConnection, L"usp_AllGroupAssets_Get");

		CClientRecordset rs;
		rs.Open(db);
		long lTotalCount = rs.GetRecordCount();
		long lLoaded = 0;
		long lLastPerc = -1;

		if(lTotalCount)
		{
			for(rs.MoveFirst(); !rs.IsEOF(); ++rs, ++lLoaded)
			{
				long	lHeadID			= rs[L"iHeadID"];
				long	lElementID		= rs[L"iElementID"];
				bool	bPriceByHead	= rs[L"bPriceByHead"];
				double	dCoeff			= rs[L"fCoeff"];


				IUndAtomPtr spHead;
				m_pUnderlyingAll->get_Item(lHeadID, &spHead);
				if(spHead)
				{
					spHead->put_IsHead( VARIANT_TRUE );
					spHead->put_Coeff( 1.0 );

					IUndAtomPtr spElement;
					m_pUnderlyingAll->get_Item(lElementID, &spElement);

					if(spElement)
					{
						spElement->put_PriceByHead(bPriceByHead ? VARIANT_TRUE : VARIANT_FALSE);
						spElement->put_Coeff(dCoeff);
						spElement->putref_HeadComponent(spHead);
					}

				}

				long lPerc = static_cast<long>( 100.* double(lLoaded)/ double(lTotalCount));
				if(lPerc != lLastPerc)
				{
					lLastPerc = lPerc;
					Fire_Progress(bsStatus, lLastPerc);
				}
			}
		}
	}
	catch(_com_error& e )
	{
		return Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_IEtsMain, e.Error());

	}
	catch(...)
	{
		return Error(_T("Unknown Error in CEtsMain::LoadAssetGroup "), IID_IEtsMain, E_FAIL);
	}
	return S_OK;
}

//load price profile
STDMETHODIMP CEtsMain::LoadPriceProfile()
{
	try
	{
		_bstr_t bsStatus("Loading Price Profile data...");

		InitializeDB();
		Fire_Progress(bsStatus,-1);

		CStoredProc<> dbPriceProfileDefault(m_DbConnection, L"usp_PriceProfile_CheckDefaults");
		CClientRecordset rsDefPriceProfile;
		rsDefPriceProfile.Open(dbPriceProfileDefault);

		CStoredProc<> db(m_DbConnection, L"usp_PriceProfile_Get");
		CClientRecordset rs;
		rs.Open(db);

		long lTotalCount = rs.GetRecordCount();
		long lLoaded = 0;
		long lLastPerc = -1;

		if(lTotalCount)
		{
			long	lPriceProfileID;
			for(rs.MoveFirst(); !rs.IsEOF(); ++rs, ++lLoaded)
			{
				
				lPriceProfileID = rs[L"iPriceProfileID"];
				_bstr_t bsProfileName = rs[L"vcPriceProfileName"];
				_bstr_t	bsDescription = rs[L"vcPriceProfileDesc"];

				IEtsPriceProfileAtomPtr	spPriceProfileAtom;
				m_pPriceProfile->get_Item(lPriceProfileID, &spPriceProfileAtom);

				CComObject<CEtsPriceProfileAtom> *pObject  = NULL;
				if (spPriceProfileAtom == NULL)
					m_pPriceProfile->AddNew(lPriceProfileID, bsProfileName, &pObject);
				else
					pObject = dynamic_cast<CComObject<CEtsPriceProfileAtom>*>((IEtsPriceProfileAtom*)spPriceProfileAtom);
				
				if(/*spPriceProfileAtom == NULL*/ true)
				{
					if( pObject != NULL )
					{
						pObject->m_nID				= lPriceProfileID;
						pObject->m_bstrName			= CComBSTR(bsProfileName.GetBSTR());
						pObject->m_bstrDescription	= CComBSTR(bsDescription.GetBSTR());
						pObject->m_nDefaultID		= rs[L"tiDefaultID"];
						pObject->m_bIsOptionProfile = ((bool)rs[L"tiIsOptionProfile"]) ? VARIANT_TRUE : VARIANT_FALSE;
						pObject->m_bUseMidMarketForPnL = ((bool)rs[L"tiMidmarketPnL"]) ? VARIANT_TRUE : VARIANT_FALSE;

						if (pObject->m_bIsOptionProfile == VARIANT_TRUE) 
						{
							pObject->m_bAlwaysUseLast			= VARIANT_FALSE;
							pObject->m_enBadOptSinglePriceRule	= static_cast<EtsOptBadSinglePriceRuleEnum>((long)rs[L"tiSinglePriceRule"]);
							pObject->m_enBadOptBothPriceRule	= static_cast<EtsOptBadBothPricesRuleEnum>((long)rs[L"tiBothPriceRule"]);
							pObject->m_bUseZeroBidRule			= ((bool)rs[L"tiZeroBidRuleForPnL"]) ? VARIANT_TRUE : VARIANT_FALSE;

							if (pObject->m_enBadOptSinglePriceRule != enObsprReplaceWithLast &&
								pObject->m_enBadOptSinglePriceRule != enObsprReplaceWithOpposite &&
								pObject->m_enBadOptSinglePriceRule != enObsprReplaceWithTheo &&
								pObject->m_enBadOptSinglePriceRule != enObsprReplaceWithZero) 
							{
								 pObject->m_enBadOptSinglePriceRule = enObsprReplaceWithLast;
							}

							if (pObject->m_enBadOptBothPriceRule != enObbprUseLast &&
								pObject->m_enBadOptBothPriceRule != enObbprUseTheo )
							{
								pObject->m_enBadOptBothPriceRule = enObbprUseLast;
							}
						}
						else
						{
							pObject->m_bAlwaysUseLast		= ((bool)rs[L"tiAlwaysLast"])		? VARIANT_TRUE : VARIANT_FALSE; 
							pObject->m_enBadUndPriceRule	= static_cast<EtsUndBadPriceRuleEnum>((long)rs[L"tiSinglePriceRule"]); 

							if (pObject->m_enBadUndPriceRule != enUbprCheckTolerance &&
								pObject->m_enBadUndPriceRule != enUbprDoNotCheckTolerance)
							{
								pObject->m_enBadUndPriceRule = enUbprDoNotCheckTolerance;
							}
						}
						
						if (m_spDefStkPriceProfile == NULL)
						{
							if (pObject->m_nDefaultID == 1 && pObject->m_bIsOptionProfile == VARIANT_FALSE)
							{
								m_spDefStkPriceProfile.Attach(pObject, true);
							}
						}

						if (m_spDefIdxPriceProfile == NULL)
						{
							if (pObject->m_nDefaultID == 3 && pObject->m_bIsOptionProfile == VARIANT_FALSE)
							{
								m_spDefIdxPriceProfile.Attach(pObject, true);
							}
						}
						if (m_spDefOptPriceProfile == NULL)
						{
							if (pObject->m_nDefaultID == 1 && pObject->m_bIsOptionProfile == VARIANT_TRUE)
							{
								m_spDefOptPriceProfile.Attach(pObject, true);
							}
						}

						
						if(pObject->m_bIsOptionProfile == VARIANT_TRUE) 
						{
							if(pObject->m_nDefaultID == 1)
								pObject->m_bUseMidMarketForPnL = VARIANT_TRUE;
							else if (pObject->m_nDefaultID == 2)
								pObject->m_bUseMidMarketForPnL = VARIANT_FALSE;
						}
						else
						{
							
							if (pObject->m_nDefaultID == 1)
							{
								pObject->m_bAlwaysUseLast		= VARIANT_FALSE;
								pObject->m_bUseMidMarketForPnL	= VARIANT_TRUE;
							}
							else if (pObject->m_nDefaultID == 2)
							{
								pObject->m_bAlwaysUseLast		= VARIANT_FALSE;
								pObject->m_bUseMidMarketForPnL	= VARIANT_FALSE;
							}
							else if (pObject->m_nDefaultID == 3)
							{
								pObject->m_bAlwaysUseLast		= VARIANT_TRUE;
								pObject->m_bUseMidMarketForPnL	= VARIANT_FALSE;
								pObject->m_enBadUndPriceRule	= enUbprDoNotCheckTolerance;
							}
						}
					}
				}
				//status update
				long lPerc = static_cast<long>( 100.* double(lLoaded)/ double(lTotalCount));
				if(lPerc != lLastPerc)
				{
					lLastPerc = lPerc;
					Fire_Progress(bsStatus, lLastPerc);
				}
			}
		}
	}
	catch(_com_error& e )
	{
		return Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_IEtsMain, e.Error());

	}
	catch(...)
	{
		return Error(_T("Unknown Error in CEtsMain::LoadPriceProfile() "), IID_IEtsMain, E_FAIL);
	}
	return S_OK;
}
//---------------------------------------------------------------------------------------------//
//load custom dividends
STDMETHODIMP CEtsMain::LoadCustomDividends()
{	
	try
	{

		_bstr_t bsStatus("Loading Custom Dividends data...");

		InitializeDB();
		Fire_Progress(bsStatus,-1);
		CStoredProc<> db(m_DbConnection, L"usp_MmCustomDividend_Get");

		CClientRecordset rs;
		rs.Open(db);
		long lTotalCount = rs.GetRecordCount();
		long lLoaded = 0;
		long lLastPerc = -1;

		double	dDividendAmmount = 0.;
		DATE	dtDividendDate = 0.;

		if(lTotalCount)
		{
			for(rs.MoveFirst(); !rs.IsEOF(); ++rs, ++lLoaded)
			{
				long	lAssetID		= rs[L"StockID"];
				
				IUndAtomPtr spAsset = NULL;
				m_pUnderlyingAll->get_Item(lAssetID, &spAsset);

				if (spAsset != NULL)
				{
					CComObject<CUndAtom>* pAsset = NULL;					
					pAsset = dynamic_cast<CComObject<CUndAtom>*>(spAsset.GetInterfacePtr());

					if (pAsset != NULL)
					{
						CComObject<CEtsIndexDivAtom>* pDivAtom = NULL;
						if (static_cast<bool>(pAsset->m_spDividend))
							pDivAtom = dynamic_cast<CComObject<CEtsIndexDivAtom>*>(pAsset->m_spDividend.GetInterfacePtr());
						else
						{
							CComObject<CEtsIndexDivAtom>::CreateInstance(&pDivAtom);
							IEtsIndexDivAtomPtr spDivAtom;
							if(pDivAtom)
							{
								spDivAtom.Attach(pDivAtom, TRUE);
								pAsset->m_spDividend = spDivAtom;
							}
						};

						pDivAtom->m_nAssetID = lAssetID;

						//check for holidays initialized
						if (!static_cast<bool>(pDivAtom->m_spHolidays))
							pDivAtom->m_spHolidays = m_spHolidays;
						
						//check for custom stream dividends initalized
						CComObject<CEtsDivColl>* pDividendStream = NULL;
						if (static_cast<bool>(pDivAtom->m_spCustomDivs))
							pDividendStream = dynamic_cast<CComObject<CEtsDivColl>*>(pDivAtom->m_spCustomDivs.GetInterfacePtr());
						else
						{
							CComObject<CEtsDivColl>::CreateInstance(&pDividendStream);
							IEtsDivCollPtr spDividendStream;
							if(pDividendStream)
							{
								spDividendStream.Attach(pDividendStream, TRUE);
								pDivAtom->m_spCustomDivs = spDividendStream;
							}
						}
						//check for holidays initialized for StreamDividends
						if (!static_cast<bool>(pDividendStream->m_spHolidays))
							pDividendStream->m_spHolidays = m_spHolidays;

						//now initialize stream
						dtDividendDate = (vt_date)rs[L"DivYtes"];
						dDividendAmmount = rs[L"DivAmnt"];
						
						pDividendStream->m_nAssetID = lAssetID;
						_CHK(pDividendStream->AddNonUnique(dtDividendDate, dDividendAmmount), _T("Fail to Add Dividend payment date."));
					}
				};
				//update status
				long lPerc = static_cast<long>( 100.* double(lLoaded)/ double(lTotalCount));
				if(lPerc != lLastPerc)
				{
					lLastPerc = lPerc;
					Fire_Progress(bsStatus, lLastPerc);
				}
			}
		}
	}
	catch(_com_error& err )
	{
		TRACE_COM_ERROR(err);
		return Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(err), IID_IEtsMain, err.Error());
	}
	catch(...)
	{
		TRACE_UNKNOWN_ERROR();
		return Error(_T("Unknown Error in CEtsMain::CustomDividends()"), IID_IEtsMain, E_FAIL);
	}
	return S_OK;
}
//---------------------------------------------------------------------------------------------//
//load holidays
STDMETHODIMP CEtsMain::LoadHolidays()
{
	try
	{

		_bstr_t bsStatus("Loading Holidays data...");

		InitializeDB();
		Fire_Progress(bsStatus,-1);
		CStoredProc<> db(m_DbConnection, L"usp_Holiday_Get");

		CClientRecordset rs;
		rs.Open(db);
		long lTotalCount = rs.GetRecordCount();
		long lLoaded = 0;
		long lLastPerc = -1;

		if(lTotalCount)
		{
			for(rs.MoveFirst(); !rs.IsEOF(); ++rs, ++lLoaded)
			{
				long	lActionID		= rs[L"tiActionID"];
				DATE	dtHolidayDate	= (vt_date)rs[L"dtHolidayDate"];

				if (lActionID < 2)
					m_pHolidays->Add(dtHolidayDate);

				long lPerc = static_cast<long>( 100.* double(lLoaded)/ double(lTotalCount));
				if(lPerc != lLastPerc)
				{
					lLastPerc = lPerc;
					Fire_Progress(bsStatus, lLastPerc);
				}
			}
		}
	}
	catch(_com_error& e )
	{
		return Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_IEtsMain, e.Error());

	}
	catch(...)
	{
		return Error(_T("Unknown Error in CEtsMain::LoadHolidays() "), IID_IEtsMain, E_FAIL);
	}
	return S_OK;
}
//---------------------------------------------------------------------------------------------//
//reload dividends for asset
STDMETHODIMP CEtsMain::ReloadDividends(LONG lAssetID)
{
	try
	{
		if (m_spUnderlyingAll)
		{	
			IUndAtomPtr spUnd = NULL;
			_CHK(m_spUnderlyingAll->get_Item(lAssetID, &spUnd), _T("Fail to get Underlying"));

			if (static_cast<bool>(spUnd))
			{
				IAssetPtr	spAsset = NULL;
				spAsset.Attach(dynamic_cast<IAsset*>(spUnd.GetInterfacePtr()), true);

				IEtsIndexDivAtomPtr spDividends = NULL;
				_CHK(spAsset->get_Dividend(&spDividends), _T("Fail to get Asset Dividends for reload call."));

				if (spDividends)
					spDividends->Reload();
			}
		}
	}
	catch (_com_error& e)
	{
		Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_IEtsMain, e.Error());	
	}
	catch (...)
	{
		return Error(_T("Unknown Error in CEtsMain::GetContract() "), IID_IEtsMain, E_FAIL);
	}
	return S_OK;
}
//---------------------------------------------------------------------------------------------//
//return contract from contract cache
STDMETHODIMP CEtsMain::GetContract(LONG lContractID, IContract** pRetVal)
{
	try
	{
		if ((bool)m_spContractsCache)
		{
			m_spContractsCache->SetConnectionString(m_bstrDatabaseString);
			m_spContractsCache->SetMain(dynamic_cast<IEtsMain*>(this));
			IContractPtr spRet = NULL;
			spRet = m_spContractsCache->GetContract(lContractID);
			if (static_cast<bool>(spRet))
			{
				*pRetVal = (IContract*)spRet;						
				if( *pRetVal ){
					(*pRetVal)->AddRef();
					return S_OK;
				}									
			}			
		}
	}
	catch (_com_error& e)
	{
		Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_IEtsMain, e.Error());	
	}
	catch (...)
	{
		return Error(_T("Unknown Error in CEtsMain::GetContract() "), IID_IEtsMain, E_FAIL);
	}
	return S_OK;
}
//---------------------------------------------------------------------------------------------//
//return rates curve from curves cache
STDMETHODIMP CEtsMain::GetBorrowingRatesCurve(LONG lCurveID, IEtsRateColl** pRetVal)
{
	try
	{
		if ((bool)m_spBorrowingRatesCache)
		{
			m_spBorrowingRatesCache->SetConnectionString(m_bstrDatabaseString);
			m_spBorrowingRatesCache->SetMain(dynamic_cast<IEtsMain*>(this));

			IEtsRateCollPtr spRet = NULL;
			spRet = m_spBorrowingRatesCache->GetCurve(lCurveID);
			if (static_cast<bool>(spRet))
			{
				*pRetVal = (IEtsRateColl*)spRet;						
				if( *pRetVal ){
					(*pRetVal)->AddRef();
					return S_OK;
				}									
			}			
		}
	}
	catch (_com_error& e)
	{
		Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_IEtsMain, e.Error());	
	}
	catch (...)
	{
		return Error(_T("Unknown Error in CEtsMain::GetRatesCurve() "), IID_IEtsMain, E_FAIL);
	}
	return S_OK;
}
//---------------------------------------------------------------------------------------------//
// send email
STDMETHODIMP CEtsMain::SendMail(BSTR sAddress, BSTR sSender, BSTR sSubject, BSTR sMessage, BSTR sSMTPServer, BSTR sFilePath) {

	try {
		BOOL bRet = FALSE;
		wchar_t *ptrToken = NULL;
		wchar_t *ptrAddres = NULL;
		wchar_t *ptrNextTok = NULL;

		CSMTPConnection SMTPConnection;
		bRet = SMTPConnection.Connect((char*)_bstr_t(sSMTPServer));
		if (!bRet) 
			return E_FAIL;

		CMimeMessage Message;
		_bstr_t sbsFilePath = sFilePath;
		if (sbsFilePath != _bstr_t(""))
			Message.AttachFile( (char*)sbsFilePath );

		Message.SetSender((const char*)_bstr_t(sSender));
		Message.SetSubject((const char*)_bstr_t(sSubject));
		Message.AddText((const char*)_bstr_t(sMessage));
		
		ptrAddres = (wchar_t*)_bstr_t(sAddress);
		ptrToken = wcstok_s(ptrAddres, (wchar_t*)(";"), &ptrNextTok);
		while (ptrToken != NULL)
		{
			Message.AddRecipient( (char*)_bstr_t(ptrToken) );
			ptrToken = wcstok_s( NULL, (wchar_t*)(";"), &ptrNextTok); 
		}

		bRet = SMTPConnection.SendMessage(Message, /*(char*)_bstr_t(sAddress)*/ NULL, (char*)_bstr_t(sSender));

		if( !bRet ) {
			SMTPConnection.Disconnect();
			return E_FAIL;
		}

		SMTPConnection.Disconnect();
	}
	catch(_com_error& e ) {
		return Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_IEtsMain, e.Error());
	}
	catch(...){
		return Error(_T("Unknown Error in CEtsMain::SendMail() "), IID_IEtsMain, E_FAIL);
	}

	return S_OK;
}


//---------------------------------------------------------------------------------------------//
SAFEARRAY* CEtsMain::PackRisks(ContractsVector& contract)
{
	try
	{
		CComRecPtr<IRisksFullInfo, &LIBID_PRICEPROVIDERSLib> RisksFullInfo;
		RisksFullInfo.Init();

		long outSize = static_cast<long>(contract.size());
		SAFEARRAYBOUND sab = { outSize, 0 };
		LPSAFEARRAY psa = ::SafeArrayCreateEx( VT_RECORD, 1, &sab, (PVOID) RisksFullInfo.GetRecordInfo() );

		if(psa)
		{
			IRisksFullInfo* pvOutData = NULL;
			if( SUCCEEDED(::SafeArrayAccessData( psa, (void**)&pvOutData) ) )
			{
				memset(pvOutData, 0, outSize * sizeof( IRisksFullInfo ));
				int j = 0;
				CAbstractContract* pBaseContract = NULL;
				for (ContractsVector::iterator it = contract.begin(); it != contract.end(); ++it)
				{
					CAbstractContract* pContract = *it;

					if (pContract->m_spBaseContract){

						pBaseContract = 
							dynamic_cast<CAbstractContract*>(pContract->m_spBaseContract.GetInterfacePtr());

						if (pBaseContract != NULL){
							pBaseContract->GetTicker()->CopyTo(pvOutData[j].BaseTicker);
						}
					}
					else{
						pvOutData[j].BaseTicker.Symbol	= _com_util::ConvertStringToBSTR("");
						pvOutData[j].BaseTicker.Exchange = _com_util::ConvertStringToBSTR("");
						pvOutData[j].BaseTicker.Type = enSTK;
					};

					pContract->m_spRisks->CopyTo(pvOutData[j].Risks);
					pContract->GetTicker()->CopyTo(pvOutData[j].Ticker);

					j++;
				}
				::SafeArrayUnaccessData(psa);

				TRACE_INFO("Contract's packed: [%d]", outSize);
				
				return psa;
			}
		}
	}
	catch (_com_error& err)
	{
		TRACE_COM_ERROR(err);
		ATLASSERT(FALSE);
	}
	catch (...)
	{
		TRACE_UNKNOWN_ERROR();
		ATLASSERT(FALSE);
	}
	return NULL;
};
//---------------------------------------------------------------------------------------------//
STDMETHODIMP CEtsMain::SetQuote(ITicker* Ticker, QuoteUpdateInfo* Quote) {
	
	CTicker symbol(*Ticker);
	CQuote	quote(*Quote);

	try
	{
		if (m_spContractsCache->SetQuote(&symbol, &quote) != NULL)
			return S_OK;
		
		// the contract is not in the contract cache. search for the contract id by the symbol
		long contractId = m_spContractsCache->GetContractID(&symbol);

		// if no contract id found for ticked then throw exception
		if (contractId <= 0) 
			FOCL_THROW_EXCEPTION(IvRmException::ContractNotFound, "No contract id found for symbol " << symbol.m_sSymbol);

		// load contract data to the cache
		if (m_spContractsCache->GetContract(contractId) == NULL) // Contract has been found try to set quote again
			FOCL_THROW_EXCEPTION(IvRmException::ContractNotFound, "Could not load contract (id = " << contractId << ")");
		
		if (m_spContractsCache->SetQuote(&symbol, &quote) == NULL)
			FOCL_THROW_EXCEPTION(IvRmException::ContractNotFound, "Unknown situation in attempt to set contract quote");			
	}
	catch (IvRmException::AbstractException& e)
	{
		return EgLib::__ErrSet(E_FAIL, L"Financial Object Library", _bstr_t(e.toString().c_str()), __FILE__, __FUNCTION__ ,__LINE__);
	}
	catch (_com_error& err)
	{
		TRACE_COM_ERROR(err);
		ATLASSERT(FALSE);
		return E_FAIL;
	}
	catch (...)
	{
		TRACE_UNKNOWN_ERROR();
		ATLASSERT(FALSE);
		return E_FAIL;
	}
	
	

	return S_OK;
};
//---------------------------------------------------------------------------------------------//
STDMETHODIMP CEtsMain::LoadContracts(ITicker* Asset)
{
	CTicker __ticker(*Asset);
	
	m_spContractsCache->InitializeForAsset(&__ticker);

	return S_OK;
};
//---------------------------------------------------------------------------------------------//
STDMETHODIMP 
CEtsMain::CalculateUpdatedContracts(SAFEARRAY** pRetVal) {

	if( !pRetVal ) 
		return E_POINTER;
	
	if( *pRetVal ) {
		::SafeArrayDestroy(*pRetVal);
		*pRetVal = NULL;
	};
	
	ContractsVector contracts;

	TRACE_INFO(_TEXT("calculate forward curve: [%s]"), _TEXT("Start"));
	RecalculateForwardCurves();
	TRACE_INFO(_TEXT("calculate forward curve: [%s]"), _TEXT("Complete"));
	
	//calculation_mgr_->on_timer();

	TRACE_INFO(_TEXT("get recalculation neded contracts: [%s]"), _TEXT("Start"));
	m_spContractsCache->GetRecalculationNeededContracts(contracts);
	TRACE_INFO(_TEXT("get recalculation neded contracts: [%s]"), _TEXT("Complete"));

	TRACE_INFO(_TEXT("calculate: [%s]"), _TEXT("Start"));
	m_spContractsCache->GetCalculator()->Calculate(contracts);
	TRACE_INFO(_TEXT("calculate: [%s]"), _TEXT("Complete"));
	
	TRACE_INFO(_TEXT("pack response to safearray: [%s]"), _TEXT("Start"));	
	*pRetVal = PackRisks(contracts);
	TRACE_INFO(_TEXT("pack response to safearray: [%s]"), _TEXT("Complete"));
	
	contracts.clear();
	
	return S_OK;
};
//---------------------------------------------------------------------------------------------//
//TODO: remove this function
STDMETHODIMP CEtsMain::DoCalc(SAFEARRAY** pParams, SAFEARRAY** pRetVal){
	return E_NOTIMPL;
};
//----------------------------------------------------------------------------------------------------------//
//setup working mode for general
STDMETHODIMP 
CEtsMain::SetWorkingMode(WorkingModeEnum enMode) {
	HRESULT hr = S_OK;
	try	{
		m_enWorkingMode = enMode;
		if (enMode == enWmClient)
			m_spContractsCache->Connect();
	}
	catch (_com_error& err)	{
		TRACE_COM_ERROR(err);
		hr = E_FAIL;
	}
	return hr;
};
//----------------------------------------------------------------------------------------------------------//
//setup log level for library
STDMETHODIMP 
CEtsMain::SetLogLevel(LONG lLevel) {
	HRESULT hr = S_OK;
	try	{
	
		if(EgLib::CEgLibTraceManager::g_spTrace != NULL) {
			EgLib::CEgLibTraceManager::g_spTrace->MinLogLevel = lLevel;

			std::string logInfo;
			logInfo.resize(32, char(0));
			_itoa_s(lLevel, const_cast<char*>(logInfo.c_str()), logInfo.size(), 10);
			TRACE_SYSTEM(_T("LogLevel = %s"), logInfo.c_str());
		}
	}
	catch (_com_error& err) {
		TRACE_COM_ERROR(err);
		hr = E_FAIL;
	}
	catch (...) {
		TRACE_UNKNOWN_ERROR();
		hr = E_FAIL;
	};
	return hr;
};
//----------------------------------------------------------------------------------------------------------//
/*setup log directory another from MyDocuments*/
STDMETHODIMP
CEtsMain::SetLogDirectory(BSTR directory) {
	HRESULT hr = S_OK;
	try {
	
		_bstr_t log_directory(directory);
		
		if (EgLib::CEgLibTraceManager::g_spTrace != NULL) {
		
			if (log_directory.length() > 0) {
								
				EgLib::CEgLibTraceManager::g_spTrace->SetPathType(EgLib::CEgLibTraceManager::enAbsolute);
				EgLib::CEgLibTraceManager::g_spTrace->SetFilePath(log_directory);
				
				TRACE_SYSTEM(_T("New Log Directory is: %s"), (char*)log_directory);
			};
		};
	}
	catch (_com_error err) {
		TRACE_COM_ERROR(err);
		hr = E_FAIL;
	}
	catch (...) {
		TRACE_UNKNOWN_ERROR();
		hr = E_FAIL;	
	};
	return hr;
};	
//----------------------------------------------------------------------------------------------------------//
//setup log level for library
STDMETHODIMP 
CEtsMain::SetLogLifetime(LONG Lifetime) {
	HRESULT hr = S_OK;
	
	try	{
	
		if(EgLib::CEgLibTraceManager::g_spTrace != NULL) {
		
			EgLib::CEgLibTraceManager::g_spTrace->LogLifetime = Lifetime;			

			std::string logInfo;
			logInfo.resize(32, char(0));
			_itoa_s(Lifetime, const_cast<char*>(logInfo.c_str()), logInfo.size(), 10);
			TRACE_SYSTEM(_T("LogLifetime = %s"), logInfo.c_str());
		}
	}
	catch (_com_error& err) {
		TRACE_COM_ERROR(err);
		hr = E_FAIL;
	}
	catch (...) {
		TRACE_UNKNOWN_ERROR();
		hr = E_FAIL;
	};
	return hr;
};
//----------------------------------------------------------------------------------------------------------//
//create flex contract by template
STDMETHODIMP CEtsMain::CreateFlexContract(LONG					AssetID,
										  DATE					Expiration,
										  DOUBLE				Strike,
										  EtsOptionTypeEnum		Type,
										  DATE					TradingClose,
										  DATE					SettlementValueTime,
										  ExerciseStyleEnum		ExerciseStyle,
										  IContract** pRetVal) {

	HRESULT hr = S_OK;
	try {

		IContractPtr spRet = NULL;

		spRet = m_spContractsCache->CreateFlexContract(	AssetID,
														Expiration,
														Strike,
														Type,
														TradingClose,
														SettlementValueTime,
														ExerciseStyle);

		if (static_cast<bool>(spRet)){
			*pRetVal = (IContract*)spRet;						
			if( *pRetVal ){
				(*pRetVal)->AddRef();
				return S_OK;
			}									
		}
	}
	catch (_com_error& err){
		TRACE_COM_ERROR(err);
		hr = E_FAIL;
	}
	catch (...){
		TRACE_UNKNOWN_ERROR();
		hr = E_FAIL;
	}
	return hr;
}
//----------------------------------------------------------------------------------------------------------//
STDMETHODIMP CEtsMain::putref_VolatilitySource(IVolatilitySource* NewVal)								
{																	
	ObjectLock lock(this);											

	m_spVolatilitySource = NewVal;
	CComGITPtr<IVolatilitySource> ptrVolatilitySourceGIT(m_spVolatilitySource.GetInterfacePtr());
	m_dwVolatilitySourceCooke = ptrVolatilitySourceGIT.Detach();

	return S_OK;													
};																	
//----------------------------------------------------------------------------------------------------------//
STDMETHODIMP CEtsMain::get_VolatilitySource(IVolatilitySource** pVal)									
{																	
	if(!pVal)														
	{																
		return E_POINTER;											
	}																

	if(*pVal)														
	{																
		(*pVal)->Release();											
		*pVal = NULL;												
	}																

	ObjectLock lock(this);

	try
	{
		CComGITPtr<IVolatilitySource> ptrVolatilitySourceGIT(m_dwVolatilitySourceCooke);	
		ptrVolatilitySourceGIT.CopyTo(pVal);
		ptrVolatilitySourceGIT.Detach();
	}
	catch (_com_error& e)
	{
		e.Description();
		ATLASSERT(false);
		return E_FAIL;
	}
	catch (...)
	{
		ATLASSERT(false);
		return E_FAIL;
	}
															
	return S_OK;													
};
//----------------------------------------------------------------------------------------------------------//
STDMETHODIMP 
CEtsMain::RecalculateForwardCurves(){
	HRESULT hr = S_OK;
	
	try {

		CUndColl::CollType::iterator itr = 
			m_pUnderlyingAll->m_collRef.begin();

		CUndColl::CollType::iterator itr_end = 
			m_pUnderlyingAll->m_collRef.end();
		
		for(; itr != itr_end; ++itr){
			CAsset* asset_ = 
				dynamic_cast<CAsset*>(itr->second->second);

			ATLASSERT(asset_ != 0);

			asset_->forward_curve->recalculate();
		};
	}
	catch (_com_error& err){
		TRACE_COM_ERROR(err);
		hr = E_FAIL;
	}
	catch (...){
		TRACE_UNKNOWN_ERROR();
		hr = E_FAIL;
	}
	return hr;
};
//----------------------------------------------------------------------------------------------------------//
long
CEtsMain::initialize_curves(){
	long curve_counter = 0;
	
	try {
		
		InitializeDB();
		
		CStoredProc<> db(m_DbConnection, L"usp_CurveBuildingBlocks_Get");
		CClientRecordset rs;
		rs.Open(db);

		long recordset_size = rs.GetRecordCount();
		
		if(recordset_size > 0){

			for(rs.MoveFirst(); !rs.IsEOF(); ++rs){

				long asset_id = rs[L"asset_id"];
				long contract_id = rs[L"contract_id"];

				IUndAtomPtr spAsset = NULL;
				m_pUnderlyingAll->get_Item(asset_id, &spAsset);
				
				if (spAsset){
					CAsset* asset = dynamic_cast<CAsset*>(spAsset.GetInterfacePtr());	
					ATLASSERT(asset);
					
					IContractPtr spContract = m_spContractsCache->GetContract(contract_id);
					if (spContract){

						CAbstractContract* contract = 
							dynamic_cast<CAbstractContract*>(spContract.GetInterfacePtr());

						ATLASSERT(contract);

						asset->forward_curve->add_contract(*contract);
					}
				}
			}
		}
	}
	catch (_com_error& err){
		TRACE_COM_ERROR(err);
	}
	catch (...){
		TRACE_UNKNOWN_ERROR();
	};
	return curve_counter;
};
//----------------------------------------------------------------------------------------------------------//

// MmTradeChannel.cpp : Implementation of CMmTradeChannel

#include "stdafx.h"
#include "MmTradeChannel.h"
#include "EtsProcessDelay.h"
#include "EtsTraderColl.h"
#include "EtsBrokerColl.h"
#include "EtsContractColl.h"
//#include "MmTradeByUndAtom.h"

// CMmTradeChannel


STDMETHODIMP CMmTradeChannel::UpdatePriceInfo(LONG lTraderId, IEtsMain* pMain)
{
	HRESULT hr = S_OK;

	try
	{
		__CHECK_POINTER(pMain);

		IEtsProcessDelayPtr spDelay;
		_CHK(spDelay.CreateInstance(CLSID_EtsProcessDelay), _T("Fail to create delay object."));
		spDelay->put_Frequency(10);
		spDelay->put_Duration(1);
		
		if(!m_Connection.IsOpened())
			m_Connection.Open(m_bstrDatabase, 10, 120, 300, 300);

		// update trade collection
		CStoredProc<CClientRecordset> rsTrade(m_Connection, L"usp_MmTradeByTrader_PriceInfo_Get");
		if(lTraderId)
			rsTrade << lTraderId;
		rsTrade.Open();

		if(rsTrade.GetRecordCount()) 
		{
			for(rsTrade.MoveFirst(); !rsTrade.IsEOF(); ++rsTrade) 
			{
				long nTradeID = rsTrade[L"iTradeID"];
				double dPriceTheoClose = rsTrade[L"fPriceTheoClose"];

				IMmTradeInfoAtomPtr spTradeInfo;
				_CHK(m_pTrd->get_Item(nTradeID, &spTradeInfo), _T("Unable to get trade."));
				CMmTradeInfoAtom*  pTradeInfo = static_cast<CMmTradeInfoAtom*>(spTradeInfo.GetInterfacePtr());
				
				if(pTradeInfo)
				{
					if(pTradeInfo->m_spOpt)
						pTradeInfo->m_spOpt->put_PriceTheoClose(dPriceTheoClose);

					if(pTradeInfo->m_spFutOpt)
						pTradeInfo->m_spFutOpt->put_PriceTheoClose(dPriceTheoClose);
				}

				spDelay->Sleep();
			}
		}
		rsTrade.Close();


		// update underlying collection
		IUndCollPtr spUndColl = NULL;
		m_spMain->get_UnderlyingAll(&spUndColl);

		CStoredProc<CClientRecordset> rsUnd(m_Connection, L"usp_MmUnderlying_PriceInfo_Get");
		if(lTraderId)
			rsUnd << lTraderId;
		rsUnd.Open();

		if(rsUnd.GetRecordCount() && spUndColl) 
		{
			for(rsUnd.MoveFirst(); !rsUnd.IsEOF(); ++rsUnd) 
			{
				long nUndID = rsUnd[L"iContractID"];
				double dPriceTheoClose = rsUnd[L"fPriceTheoClose"];

				IUndAtomPtr			spUnd;
				_CHK(spUndColl->get_Item(nUndID, &spUnd), _T("Unable to get Underlying"));
				if(spUnd)
					spUnd->put_PriceTheoClose(dPriceTheoClose);

				spDelay->Sleep();
			}
		}
		rsUnd.Close();


	}
	catch (_com_error& err)
	{
		hr = Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(err), __uuidof(IMmTradeChannel), err.Error());

	}
	catch(...)
	{
		hr = HRESULT_FROM_WIN32(GetLastError());
		hr = Error(_T("Fail to load Trades"), __uuidof(IMmTradeChannel), FAILED(hr)?hr:E_FAIL);
	}

	return hr;
}


STDMETHODIMP CMmTradeChannel::UpdateManualActivePrices()
{
		/*CMmTradeInfoColl::EnumCollType::iterator itrX    = m_pTrd->m_coll.begin();
		CMmTradeInfoColl::EnumCollType::iterator itrXend = m_pTrd->m_coll.end();

		for(; itrX != itrXend; ++itrX)
		{
			IMmTradeInfoAtomPtr spTradeInfoAtom = itrX->second;
			CMmTradeInfoAtom*  pTradeInfoAtom = static_cast<CMmTradeInfoAtom*>((IMmTradeInfoAtom*)spTradeInfoAtom);
			pTradeInfoAtom->m_dManualActivePrice = 0;
		}

		IEnumVARIANTPtr _cts = this->m_spMain->Contract->_NewEnum;
		
		_cts->Reset();

		_variant_t var;

		while(_cts->Next(1L, &var, NULL) == S_OK)
		{
			IEtsContractAtomPtr ctr = var;

			if(ctr->Und != NULL)
			{
				ctr->Und->ManualActivePrice = 0;
			}
			
			if (ctr->Fut != NULL)
			{
				ctr->Fut->ManualActivePrice = 0;
			}
		}

		_cts = NULL;

		CStoredProc<CClientRecordset> rs(m_Connection, L"usp_MmManualPriceForTrade_Get");
		rs.Open();
		if(rs.GetRecordCount())
		{
			rs.MoveFirst();

			while (!rs.IsEOF()) 
			{
				IMmTradeInfoAtom* pTradeInfo = NULL;

				m_pTrd->get_Item((LONG)rs[L"TradeID"], &pTradeInfo);

				if(pTradeInfo != NULL)
				{
					static_cast<CMmTradeInfoAtom*>(pTradeInfo)->m_dManualActivePrice = rs[L"ManualPrice"];

					pTradeInfo->Release();
				}else 
				{
					LONG lCtrID = rs[L"ContractID"];

					IEtsContractAtomPtr ctr = this->m_spMain->Contract->GetItem((LONG)rs[L"ContractID"]);

					if (ctr != NULL)
					{
						if (ctr->Fut != NULL)
						{
							ctr->Fut->ManualActivePrice = rs[L"ManualPrice"];
						}
						else
						{
							ctr->Und->ManualActivePrice = rs[L"ManualPrice"];
						}
					}
				}

				rs.MoveNext();
			}
		}

		rs.Close();*/

		return S_OK;
}

STDMETHODIMP CMmTradeChannel::LoadTrades(LONG lTraderId, 
										 IEtsMain* pMain,
										 VARIANT_BOOL bShowProgress)
{
	HRESULT hr = S_OK;
	try
	{
		__CHECK_POINTER(pMain);
		m_spMain.Attach(pMain, TRUE);

		IUndCollPtr			spUndColl = NULL;
		m_spMain->get_Underlying(&spUndColl);

		IEtsTraderCollPtr	spTraderColl = NULL;
		m_spMain->get_Trader(&spTraderColl); 

		IEtsBrokerCollPtr	spBrokerColl = NULL;
		m_spMain->get_Broker(&spBrokerColl);

		IEtsBrokerCollPtr	spClearingBrokerColl = NULL;
		m_spMain->get_ClearingBroker(&spClearingBrokerColl);

		IEtsStrategyCollPtr spStrategyColl = NULL;
		m_spMain->get_Strategy(&spStrategyColl);

		if(!m_Connection.IsOpened())
			m_Connection.Open(m_bstrDatabase, 10, 120, 300, 300);

		m_pTrdForMatch->Clear();
		m_pTrdByUndMan->Clear();
		m_pTrdByUnd->Clear();
		m_pTrdByTraderAndContract->Clear();
		m_pTrdByFut->Clear();
		m_pTrd->Clear();

		CStoredProc<> spTrades(m_Connection, L"usp_MmTradeByTrader_Get");
		if(lTraderId)
			spTrades << lTraderId;
		CClientRecordset spRcTrades;
		spRcTrades.Open(spTrades);
		long lTradesCount = spRcTrades.GetRecordCount();
		long lLoaded = 0;
		long lLastPercent= -1;
		long lPercent = 0;
		_bstr_t bsNotificationText(L"Loading trades data...");

		if(lTradesCount)
		{
			Fire_Notification(bsNotificationText, -1);
			for(spRcTrades.MoveFirst(); !spRcTrades.IsEOF(); spRcTrades.MoveNext(), ++lLoaded)
			{
				long lUndId		= spRcTrades[L"iUnderlyingContractID"];
				long lTraderId  = spRcTrades[L"iTraderID"]; 

				IUndAtomPtr			spUnd;
				IEtsTraderAtomPtr	spTrader; 
				

				_CHK(spUndColl->get_Item(lUndId, &spUnd),		 _T("Unable to get Underlying"));
				_CHK(spTraderColl->get_Item(lTraderId, &spTrader), _T("Unable to get Trader"));

				if(spUnd!=NULL && spTrader!=NULL)
				{
					EtsContractTypeEnum enType = static_cast<EtsContractTypeEnum>(long(spRcTrades[L"iContractTypeID"]));
					IMmTradeInfoAtomPtr spTradeInfo;

					CComObject<CMmTradeInfoAtom>* pTradeInfo = NULL;
					_CHK(CComObject<CMmTradeInfoAtom>::CreateInstance(&pTradeInfo),_T("Unable to create Trade info"));
					spTradeInfo.Attach(pTradeInfo, true);
					pTradeInfo->m_spUnd		= spUnd;
					pTradeInfo->m_spTrader	= spTrader;
					pTradeInfo->m_spMain	= m_spMain;				

					if(spBrokerColl!=NULL)
						_CHK(spBrokerColl->get_Item(spRcTrades[L"iBrokerID"], &pTradeInfo->m_spBroker),_T("Unable to get  Broker info")); 
					if(spClearingBrokerColl!=NULL)
						_CHK(spClearingBrokerColl->get_Item(spRcTrades[L"iClearingBrokerID"], &pTradeInfo->m_spClearingBroker),_T("Unable to get Clearing Broker info")); 

					if(spStrategyColl!=NULL)
						_CHK(spStrategyColl->get_Item(spRcTrades[L"iStrategyID"], &pTradeInfo->m_spStrategy),_T("Unable to get Strategy info")); 
					
					pTradeInfo->Load(enType, spRcTrades);

					AdjustTraderContractAndStrategy(pTradeInfo);
					IMmTradeInfoAtomPtr spNewTrade;
					m_pTrd->Add( pTradeInfo->m_nTradeID, pTradeInfo->m_dtTradeDate, spTradeInfo, &spNewTrade);

					AddTradeToTradesByUnd(pTradeInfo);
					AddTradeToTradesByTraderAndContract(pTradeInfo);
					AddManualTradeToTradesByUndMan(pTradeInfo);
					AddTradeToTradesByFut(pTradeInfo);
					lPercent = static_cast<long>(100. * double(lLoaded)/double(lTradesCount));

					if(lLastPercent != lPercent)
					{
						lLastPercent = lPercent;
						Fire_Notification(bsNotificationText, lLastPercent);
					}

				}
			}   
		}   
	}
	catch (_com_error& err)
	{
		hr = Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(err), __uuidof(IMmTradeChannel), err.Error());

	}
	catch(...)
	{
		hr = HRESULT_FROM_WIN32(GetLastError());
		hr = Error(_T("Fail to load Trades"), __uuidof(IMmTradeChannel), FAILED(hr)?hr:E_FAIL);
	}
	return hr;
}


void CMmTradeChannel::AddTradeToTradesByUnd(CComObject<CMmTradeInfoAtom>* pTrd)
{
	long lFutureID = 0L;
	if (pTrd->m_spFut)
		pTrd->m_spFut->get_ID(&lFutureID);

	if(pTrd->m_spFut == NULL || !lFutureID)
	{
		long lAssetID = 0L;
		pTrd->m_spUnd->get_ID(&lAssetID);
		_bstr_t	bsKey(lAssetID);

		IMmTradeInfoCollPtr aTrdByUnd;
		m_pTrdByUnd->get_Item(bsKey, &aTrdByUnd);
		if(aTrdByUnd == NULL)
			m_pTrdByUnd->Add(bsKey, NULL, &aTrdByUnd);

		IMmTradeInfoAtomPtr spTrade;
		IMmTradeInfoAtomPtr spNewTrade;
		
		spTrade.Attach(pTrd, TRUE);
		aTrdByUnd->Add(pTrd->m_nTradeID, pTrd->m_dtTradeDate, spTrade,&spNewTrade);
	}
}
void CMmTradeChannel::AddTradeToTradesByTraderAndContract(CComObject<CMmTradeInfoAtom>* pTrd)
{
	long lTraderID = 0L;
	pTrd->m_spTrader->get_ID(&lTraderID);

	_bstr_t bsTraderKey(lTraderID);

	long lContractID = 0L;
	pTrd->get_ContractID(&lContractID);

	_bstr_t bsContractKey(lContractID);
	_bstr_t bsKey = bsTraderKey + _bstr_t(L"_") +bsContractKey;

	IMmTradeInfoCollPtr aTrdFind;
	m_pTrdByTraderAndContract->get_Item(bsKey, &aTrdFind);
	if(aTrdFind == NULL)
		m_pTrdByTraderAndContract->Add(bsKey, NULL, &aTrdFind);

	IMmTradeInfoAtomPtr spTrade;
	IMmTradeInfoAtomPtr spNewTrade;

	spTrade.Attach(pTrd, TRUE);
	aTrdFind->Add(pTrd->m_nTradeID, pTrd->m_dtTradeDate, spTrade,&spNewTrade);

}
void	CMmTradeChannel::AddManualTradeToTradesByUndMan(CComObject<CMmTradeInfoAtom>* pTrd)
{
	if(pTrd->m_enStatus == enTrsManual)
	{
		long lAssetID = 0L;
		pTrd->m_spUnd->get_ID(&lAssetID);
		_bstr_t	bsKey(lAssetID);

		long lTraderID = 0L;
		pTrd->m_spTrader->get_ID(&lTraderID);

		bsKey+= L"_" + _bstr_t(lTraderID);

		IMmTradeInfoCollPtr aTrdFind;
		m_pTrdByUndMan->get_Item(bsKey, &aTrdFind);
		if(NULL == aTrdFind)
			m_pTrdByUndMan->Add(bsKey, NULL, &aTrdFind);

		IMmTradeInfoAtomPtr spTrade;
		IMmTradeInfoAtomPtr spNewTrade;

		spTrade.Attach(pTrd, TRUE);
		aTrdFind->Add(pTrd->m_nTradeID, pTrd->m_dtTradeDate, spTrade,&spNewTrade);
	}

}
void	CMmTradeChannel::AddTradeToTradesByFut(CComObject<CMmTradeInfoAtom>* pTrd)
{
	long lFutID = pTrd->m_spFut != NULL ? pTrd->m_spFut->get_ID(&lFutID) : 0L;
	if(lFutID)
	{
		_bstr_t bsKey(lFutID);
		IMmTradeInfoCollPtr aTrdFind;
		m_pTrdByFut->get_Item(bsKey, &aTrdFind);
		if(aTrdFind == NULL)
			m_pTrdByFut->Add(bsKey, NULL, &aTrdFind);

		IMmTradeInfoAtomPtr spTrade;
		IMmTradeInfoAtomPtr spNewTrade;

		spTrade.Attach(pTrd, TRUE);
		aTrdFind->Add(pTrd->m_nTradeID, pTrd->m_dtTradeDate, spTrade,&spNewTrade);
	}
}
STDMETHODIMP CMmTradeChannel::LoadStrategy(LONG lStrategyID, IEtsStrategyAtom** pStrategyAtom, VARIANT_BOOL* bNew)
{
	if(!bNew)
		*bNew = false;

	if(m_spMain != NULL)	
	{
		IEtsStrategyCollPtr spStrategys = NULL;
		m_spMain->get_Strategy(&spStrategys);
		IEtsStrategyAtomPtr spStrategy = NULL;
		spStrategys->get_Item(lStrategyID, &spStrategy);

		if(spStrategy !=NULL)
			*pStrategyAtom = spStrategy.Detach();
		else
		{
			if(!m_Connection.IsOpened())
				m_Connection.Open(m_bstrDatabase, 10, 120, 300, 300);

			CStoredProc<> sp(m_Connection, L"usp_Strategy_Get");
			sp << lStrategyID;
			
			CClientRecordset rs;
			rs.Open(sp);
			if(rs.GetRecordCount())
			{
				IEtsStrategyAtomPtr spNewStrategy = NULL;
				spStrategys->Add(lStrategyID,(_bstr_t)rs[L"vcStrategyName"], NULL, &spNewStrategy);
				spNewStrategy->put_Desc(_bstr_t(rs[L"vcDescription"]).Detach());
				*pStrategyAtom = spNewStrategy.Detach();
                if(!bNew) *bNew = true;
			}
		}
	}
	return S_OK;
}

HRESULT CMmTradeChannel::AdjustTraderContractAndStrategy(CComObject<CMmTradeInfoAtom>* pTrade, bool bFireEvents )
{
 	long lUndId = 0;
	pTrade->get_UndID(&lUndId);
	
	IUndCollPtr spAssetColl = NULL;
	m_spMain->get_UnderlyingAll(&spAssetColl);

	IUndAtomPtr spUnd = NULL;
	spAssetColl->get_Item(lUndId, &spUnd);

	if(spUnd != NULL)
	{
		IEtsTraderAtomPtr spTrader;
		spUnd->put_IsTraderContract(VARIANT_TRUE);

		IEtsTraderCollPtr spTraderColl = NULL;
		m_spMain->get_Trader(&spTraderColl);

		long lTraderID = 0L;
		pTrade->m_spTrader->get_ID(&lTraderID);
		spTraderColl->get_Item(lTraderID, &spTrader);

		if(spTrader != NULL)
		{
			IEtsStrategyAtomPtr spStrategy;
			VARIANT_BOOL bNewStrategy = VARIANT_FALSE;
			if(pTrade->m_spStrategy!=NULL){
				long lStrategyID = 0L;
				pTrade->m_spStrategy->get_ID(&lStrategyID);
				LoadStrategy(lStrategyID, &spStrategy, &bNewStrategy);
			}

			_bstr_t bsSymbol;
			spUnd->get_Symbol(&bsSymbol.GetBSTR());
			
			IEtsTraderUndCollPtr spTraderAssetsColl = NULL;
			spTrader->get_Und(&spTraderAssetsColl);

			IEtsTraderUndAtomPtr aTrdUnd = NULL;
			spTraderAssetsColl->get_Item(lUndId, &aTrdUnd);

			if(aTrdUnd == NULL){
				spTraderAssetsColl->Add(lUndId, bsSymbol, NULL, &aTrdUnd);
			}
			 aTrdUnd->putref_Und(spUnd);

			 if(spStrategy != NULL)
			 {
				long lStrategyId = 0L;
				spStrategy->get_ID(&lStrategyId);
				IEtsStrategyCollPtr spStrategys = NULL;
				aTrdUnd->get_Strategy(&spStrategys);

				IEtsStrategyAtomPtr spStrategyItem = NULL;
				spStrategys->get_Item(lStrategyId, &spStrategyItem);
				if(spStrategyItem == NULL){
					BSTR bsStrategyName = NULL;
					spStrategy->get_Name(&bsStrategyName);
					IEtsStrategyAtomPtr spTemporaryPtr = NULL;
					spStrategys->Add(lStrategyId, bsStrategyName, spStrategy, &spTemporaryPtr);
				};
			 }
			 IEtsContractCollPtr spContractsAll = NULL;
			 m_spMain->get_ContractAll(&spContractsAll);

			 IEtsContractAtomPtr spContractAllAtom = NULL;
			 spContractsAll->get_Item(lUndId, &spContractAllAtom);

			 IEtsContractCollPtr spContracts = NULL;
			 m_spMain->get_Contract(&spContracts);

			 IEtsContractAtomPtr spContractAtom = NULL;
			 spContracts->get_Item(lUndId, &spContractAtom);

			 if(spContractAllAtom != NULL && spContractAtom == NULL){
				 IEtsContractAtomPtr spTemporaryPtr = NULL;
				 spContracts->Add(lUndId, bsSymbol, spContractAllAtom, &spTemporaryPtr);
			 }
		}
	}

	return S_OK;
}

//---------------------------------------------------------------------------------------------//
STDMETHODIMP CMmTradeChannel::GetData(IEtsFilterData* ipFilter, IMmTradeInfoColl** ipResult)
{
	try
	{
		__CHECK_POINTER(ipFilter);
		IEtsFilterDataPtr spFilter = NULL;
		spFilter.Attach(ipFilter, true);

		if (!ipResult)
			return E_POINTER;

		if(*ipResult){																
			(*ipResult)->Release();										
			*ipResult = NULL;											
		};
		
		IMmTradeInfoCollPtr spResult = IMmTradeInfoCollPtr(__uuidof(MmTradeInfoColl));
		*ipResult = (IMmTradeInfoColl*)spResult;
		if (*ipResult)
			(*ipResult)->AddRef();

		if (*ipResult){
			
			CTradeInfoViewData	filteredTrades;
			FilterData(spFilter, filteredTrades);

			if (filteredTrades.size() > 0){
				CTradeInfoViewData::iterator it = filteredTrades.begin();
				CTradeInfoViewData::iterator itEnd = filteredTrades.end();

				for (; it != itEnd; it++)
				{
					long lTradeID = 0L;
					_CHK((*it)->get_TradeID(&lTradeID), _T("Fail to get trade id."));

					DATE dtTradeDate = 0.;
					_CHK((*it)->get_TradeDate(&dtTradeDate), _T("Fail to get TradeDate."));

					IMmTradeInfoAtomPtr spReturnPtr = NULL;
					IMmTradeInfoAtomPtr spTrade = (*it);
					spResult->Add(lTradeID, dtTradeDate, spTrade, &spReturnPtr);
					/*_CHK((*ipResult)->Add(lTradeID, dtTradeDate, spTrade, &spReturnPtr),
						_T("Fail to Add trade to collection."));*/
				}
			}
		};
	}
	catch (_com_error& err)
	{
		TRACE_COM_ERROR(err);		
		ATLASSERT(FALSE);
		throw;
	}
	catch (...)
	{
		TRACE_UNKNOWN_ERROR();
		ATLASSERT(FALSE);
		throw;
	};
	return S_OK;
};
//---------------------------------------------------------------------------------------------//
//STDMETHODIMP CMmTradeChannel::OpenTradeView(IMmTradeView** pTradeView)
//{
//	HRESULT hr = S_OK;
//	try
//	{
//		CComObject<CMmTradeView>* pView = NULL;
//		CComObject<CMmTradeView>::CreateInstance(&pView);
//		if(pView)
//		{
//			pView->AddRef();
//			*pTradeView = pView;
//			pView->SetTradeChannel(this);
//		}   
//	}
//	catch (_com_error& err)
//	{
//		hr = Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(err), __uuidof(IMmTradeChannel), err.Error());
//	}
//	return hr;
//}



